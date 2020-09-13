//-----------------------------------------------------------------------------
//!\file world_session.cpp
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief ����������ͻ����������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "world_session.h"
#include "login_session.h"
#include "player_session.h"
#include "world.h"
#include "game_guarder.h"
#include "TimeChecker.h"
#include "..\WorldDefine\selectrole.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/msg_serverinfo.h"
#include "../ServerDefine/msg_common.h"
#include "../ServerDefine/msg_log.h"
#include "att_res.h"
// Jason 2010-1-27 v1.3.2 ��һع�/���߽������
#include "player_account.h"
#include "activity_mgr.h"
#include "Role.h"
#include <fstream>


const INT MAX_RECV_WAIT_UNIT_NUM = 64;		// �ͻ��˵ȴ�����İ�������������������������ߵ�
const INT MAX_SEND_CAST_UNIT_NUM = 1000;	// ���͸��ͻ��˵���û���յ���Ӧ�İ��������������������ߵ�
const INT MAX_SEND_CAST_SIZE = 2*1024*1024;	// ���͸��ͻ��˵���û���յ���Ӧ���ֽ����������������ߵ�

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
WorldSession::WorldSession(): m_Trunk(this), m_lfPlayerNumFactor(1.0) , m_bIsStartPrint(FALSE), m_bIsPrintInfo(FALSE)
{
	m_dwLastUpdateTime = GetCurrentDWORDTime();
	m_mapAccountLoginCount.Clear();
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
WorldSession::~WorldSession()
{

}

//-------------------------------------------------------------------------------
// �õ�һ��session��Ҫע��ָ��õ���İ�ȫ�ԣ�
//-------------------------------------------------------------------------------
PlayerSession* WorldSession::FindSession(DWORD dwID)
{
	m_AllSessionMutex.Acquire();
	PlayerSession* pSession = m_mapAllSessions.Peek(dwID);
	m_AllSessionMutex.Release();

	return pSession;
}

//--------------------------------------------------------------------------------
// �õ�һ��ѡ�˽����session��ֻ�������߳��е��ã�
//--------------------------------------------------------------------------------
PlayerSession* WorldSession::FindGlobalSession(DWORD dwID)
{
	m_GlobalSessionMutex.Acquire();
	PlayerSession* pSession = m_mapGlobalSessions.Peek(dwID);
	m_GlobalSessionMutex.Release();

	return pSession;
}

//--------------------------------------------------------------------------------
// ���һ��sessionID�Ƿ����
//--------------------------------------------------------------------------------
BOOL WorldSession::IsSessionExist(DWORD dwID)
{
	m_AllSessionMutex.Acquire();
	BOOL bExist = m_mapAllSessions.IsExist(dwID);
	m_AllSessionMutex.Release();

	return bExist;
}

//--------------------------------------------------------------------------------
// ����һ���µ�session
//-------------------------------------------------------------------------------
VOID WorldSession::AddSession(PlayerSession* pSession)
{ 
	if( P_VALID(pSession) )
	{
		m_AllSessionMutex.Acquire();
		m_mapAllSessions.Add(pSession->GetSessionID(), pSession);
		m_AllSessionMutex.Release();
	}
}
//-------------------------------------------------------------------------------
// ɾ��һ��session
//-------------------------------------------------------------------------------
VOID WorldSession::RemoveSession(UINT32 dwID)
{
	m_AllSessionMutex.Acquire();
	m_mapAllSessions.Erase(dwID);
	m_AllSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// �ߵ�һ������
//-------------------------------------------------------------------------------
VOID WorldSession::Kick(DWORD dwInternalIndex)
{
	m_pNetSession->Kick(dwInternalIndex);
}
//-------------------------------------------------------------------------------
// ��ѡ�˽���session�м���һ��session
//-------------------------------------------------------------------------------
VOID WorldSession::AddGlobalSession(PlayerSession* pSession)
{
	if( P_VALID(pSession) )
	{
		if( m_GlobalSessionMutex.TryAcquire() )
		{
			m_mapGlobalSessions.Add(pSession->GetSessionID(), pSession);
			m_GlobalSessionMutex.Release();
		}
		else
		{
			m_InsertPoolMutex.Acquire();
			m_listInsertPool.PushBack(pSession);
			m_InsertPoolMutex.Release();
		}
	}
}

//-------------------------------------------------------------------------------
// ��ѡ�˽���session�б���ɾ��һ��session
//-------------------------------------------------------------------------------
VOID WorldSession::RemoveGlobalSession(DWORD dwSessionID)
{
	m_GlobalSessionMutex.Acquire();
	m_mapGlobalSessions.Erase(dwSessionID);
	m_GlobalSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
BOOL WorldSession::Init()
{
	// vEngine����
	m_pUtil = "Util";
	m_pNetSession = "XServer";

	// ���������ļ�
	TObjRef<VarContainer> pVar = "VarContainer";

	m_nPlayerNumLimit = (INT)pVar->GetDword(_T("player_num_limit"), _T("world"));
	m_nPort = (INT)pVar->GetDword(_T("port"), _T("server"));

	m_nMaxRecvWaitUnitNum	= (INT)pVar->GetDword(_T("max_recv_wait_unit_num"), _T("house_keeping"), 64);		// �ͻ��˵ȴ�����İ�������������������������ߵ�
	m_nMaxSendCastUnitNum	= (INT)pVar->GetDword(_T("max_send_cast_unit_num"), _T("house_keeping"), 1000);		// ���͸��ͻ��˵���û���յ���Ӧ�İ��������������������ߵ�
	m_nMaxSendCastSize		= (INT)pVar->GetDword(_T("max_send_cast_size"), _T("house_keeping"), 2097152);		// ���͸��ͻ��˵���û���յ���Ӧ���ֽ����������������ߵ�


	m_bEnableExpressPay = (BOOL)pVar->GetDword(_T("enableexpresspay worldinfo"));
	if(m_bEnableExpressPay)
		_tcscpy(m_szExpressPayUrl, pVar->GetString(_T("expresspayurl worldinfo")));
	
	// �������ŻURL
	_tcscpy(m_szTodayHotActivityUrl, pVar->GetString(_T("today_hot_activity_url worldinfo")));
	// ս���һ�ս��ʩ�ż��ܺ�ʹ����Ʒʱ����
	m_dwAutoFightTimeInterval = pVar->GetDword(_T("time_interval auto_fight"));
	m_bGuildWarOpen = pVar->GetDword(_T("open guild_war"));

	m_dwLoginMode = pVar->GetDword(_T("loginmode worldinfo"));

	m_dwMediaOnlineRecordInterval = pVar->GetDword(_T("record_interval media_online_num"));
	m_bDBQBOpenState[0] = pVar->GetDword(_T("Sunday duo_bao_qi_bing"));
	m_bDBQBOpenState[1] = pVar->GetDword(_T("Monday duo_bao_qi_bing"));
	m_bDBQBOpenState[2] = pVar->GetDword(_T("Tuesday duo_bao_qi_bing"));
	m_bDBQBOpenState[3] = pVar->GetDword(_T("Wednesday duo_bao_qi_bing"));
	m_bDBQBOpenState[4] = pVar->GetDword(_T("Thursday duo_bao_qi_bing"));
	m_bDBQBOpenState[5] = pVar->GetDword(_T("Friday duo_bao_qi_bing"));
	m_bDBQBOpenState[6] = pVar->GetDword(_T("Saturday duo_bao_qi_bing"));
	m_dwWardrobeOn = pVar->GetDword(_T("on wardrobe"));
	// Kick player Log������־
	m_dwKickPlayerLogOn = pVar->GetDword(_T("on KickPlayerLog"));
	// Kick role��Log�⿪����־��1������0�ر�
	m_dwKillRoleLogOn = pVar->GetDword(_T("on KillRoleLog"));

	// �趨����ײ�
	tagXServerConfig InitParam;
	InitParam.fnLogIn		=	(XLOGINCALLBACK)m_Trunk.sfp2(&WorldSession::LoginCallBack);
	InitParam.fnLogOut		=	(XLOGOUTCALLBACK)m_Trunk.sfp1(&WorldSession::LogoutCallBack);
	InitParam.bReusePort	=	true;
	InitParam.nPort			=	m_nPort;

	m_pNetSession->Init(InitParam);
	m_nPort = m_pNetSession->GetConfig()->nPort;
	m_nTreasureSum = 0;

#ifdef ON_GAMEGUARD
	m_dwLastUpdateCStime = timeGetTime();
#endif

	// ע�����пͻ�������
	PlayerSession::RegisterAllPlayerCmd();
	// ע�����з������˷��͵�����
	PlayerSession::RegisterALLSendCmd();

	return TRUE;
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID WorldSession::Update()
{
	CHECK_TIME()
	InterlockedExchange((LPLONG)&m_nMsgSendThisTick, 0);
	InterlockedExchange((LPLONG)&m_nMsgProceedThisTick, 0);
	InterlockedExchange((LPLONG)&m_nMsgRecvWait, 0);

	// ��InsertPool���ó�Ҫ�����session
	m_InsertPoolMutex.Acquire();
	PlayerSession* pSession = m_listInsertPool.PopFront();
	while( P_VALID(pSession) )
	{
		AddGlobalSession(pSession);
		pSession = m_listInsertPool.PopFront();
	}
	m_InsertPoolMutex.Release();

	// ����ѡ�˽����session
	UpdateSession();

	// ���¸���ý�����������ͳ�ƽ��
	UpdateMediaOnlineNum();

	if(g_world.IsPrintRoleInfoOpen())
	{
		// ��ӡ��ǰ�����˽�ɫid���˺�
		tagDWORDTime now = GetCurrentDWORDTime();

		if ( !m_bIsStartPrint && now.hour == 21 && now.min == 0  && now.sec == 0)
		{
			m_bIsStartPrint = FALSE;	//9��ʱ����
		}
		// ÿ������9��20��ӡ
		if ( !m_bIsStartPrint && now.hour == 21 && now.min == 20  && now.sec == 0)
		{
			m_bIsPrintInfo = TRUE;
			m_mapAllSessions.ResetIterator();
			m_bIsStartPrint = TRUE;
		}

		if(m_bIsPrintInfo)
		{
			PrintRoleInfo();
		}

	}

	// ����Ǻ����汾������������ֵ֤�����͸��ͻ���
#ifdef ON_GAMEGUARD
	// ÿ��3��������CS��ֵ֤�����͸��ͻ���
	if( timeGetTime() - m_dwLastUpdateCStime >= 3*60*1000 )
	{ 
		m_dwLastUpdateCStime = timeGetTime();
		CreateSendCS();
	}	
#endif
}

//-------------------------------------------------------------------------------
// ���¸����ͻ���session
//-------------------------------------------------------------------------------
VOID WorldSession::UpdateSession()
{
	// ��������ѡ�˽����session
	m_GlobalSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	m_mapGlobalSessions.ResetIterator();

	while( m_mapGlobalSessions.PeekNext(pSession) )
	{
		if( CON_LOST == pSession->Update())
		{
			// IMSG(_T("player logout in worldsession, sessionid=%u, internalid=%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex());

			RemoveSession(pSession->GetSessionID());
			m_mapGlobalSessions.Erase(pSession->GetSessionID());
			g_loginSession.SendPlayerLogout(pSession->GetSessionID());

			PlayerLogout();

			g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
			SAFE_DEL(pSession);
		}
	}

	m_GlobalSessionMutex.Release();
}

#ifdef ON_GAMEGUARD
//-------------------------------------------------------------------------------
// �����汾����ÿ���ͻ��ˣ�����CS��ֵ֤������Client
//-------------------------------------------------------------------------------
VOID WorldSession::CreateSendCS()
{
	g_world.LockNetWork();

	// ��������ѡ�˽����session
	m_AllSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	UINT32 uRetPackSize = 0;
	m_mapAllSessions.ResetIterator();

	while( m_mapAllSessions.PeekNext(pSession) )
	{
		UINT32 uRet = pSession->m_CS.Get( &uRetPackSize );

		if( uRet >= 3000 )	
		{		
			pSession->m_CS.Close();

			// �Ͽ��������
			ILOG->Write(_T("����CS��֤��ʧ��,������:%u,Kick Player: %u\r\n"), uRet, pSession->GetSessionID());
			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
			continue;
		}

		// ����CS��ֵ֤
		DWORD dwMsgSize = sizeof(tagNS_KoreaCSApprove) + uRetPackSize;
		BYTE* p = new BYTE[dwMsgSize];
		memset(p,0,dwMsgSize);
		tagNS_KoreaCSApprove* pSend = (tagNS_KoreaCSApprove*)p;
		pSend->dwID        = pSend->Crc32("NS_KoreaCSApprove");
		pSend->dwSize      = dwMsgSize;

		memcpy(pSend->byData, pSession->m_CS.packet, uRetPackSize);
		pSession->SendMessage(pSend, pSend->dwSize);
		SAFE_DEL_ARRAY(p);
		// test
		//ILOG->Write(_T("����CS��֤�� Player: %u\r\n"), pSession->GetSessionID());
	}

	m_AllSessionMutex.Release();

	g_world.UnlockNetWork();
}
#endif

//-------------------------------------------------------------------------------
// ��һЩ������������
//-------------------------------------------------------------------------------
VOID WorldSession::DoHouseKeeping()
{
	CHECK_TIME();

	// �����ײ�����㿪ʼ����
	m_pNetSession->ActiveSend();

	g_world.LockNetWork();

	// ��ѯ����session
	m_AllSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	TMap<DWORD, PlayerSession*>::TMapIterator it = m_mapAllSessions.Begin();

	INT nAllSendCast = 0;
	INT nMaxChokeSize = 0;
	DWORD dwMaxChokeSessionID = GT_INVALID;	
	
	
	while( m_mapAllSessions.PeekNext(it, pSession) )
	{
		// ���ͻ��˽��ܶ��е���Ϣ����
		if( pSession->GetMsgWaitNum() >= GetMaxRecvWaitUnitNum() && !pSession->IsKicked() )
		{
			IMSG(_T("Kick Too Fast Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			ITRACE(_T("Kick Too Fast Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			ILOG->Write(_T("Kick Too Fast Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			
			// ����������ң���Log
			if (m_dwKickPlayerLogOn == 1)
			{
				DWORD dwSize = 0;
				int nMsgNum = 0;	
				static TCHAR tempLog[2*1024] = {0};

				bool isLog = false;
				memset(tempLog, 0, sizeof(tempLog));
				LPBYTE pMsg = m_pNetSession->Recv(pSession->GetInternalIndex(), dwSize, nMsgNum);
				while (pMsg != NULL)
				{
					tagNetCmd* pCmd = (tagNetCmd*)pMsg;				
					_stprintf(tempLog, _T("%s  %u"), tempLog, pCmd->dwID);
					isLog =true;
					pMsg = m_pNetSession->Recv(pSession->GetInternalIndex(), dwSize, nMsgNum);
					if (!pMsg)
					{
						break;
					}
				}

				// ����������ң���Log	
				if (isLog)
				{
					ILOG->Write(_T("kick client log:%s\r\n"),tempLog);
				}	
			}			

			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
			continue;
		}

		// ���ÿͻ���δ�յ�ȷ�ϵİ�������
		INT nCastSize = GetSendCastSize(pSession->GetInternalIndex());
		nAllSendCast += nCastSize;

		if( nCastSize >= GetMaxSendCastSize() && !pSession->IsKicked() )
		{
			IMSG(_T("Kick Choke Player[%u, %u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex(), nCastSize,g_world.GetCurTick());
			ITRACE(_T("Kick Choke Player[%u, %u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex(), nCastSize,g_world.GetCurTick());
			ILOG->Write(_T("Kick Choke Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
		}
		else if( nCastSize > nMaxChokeSize )
		{
			nMaxChokeSize = nCastSize;
			dwMaxChokeSessionID = pSession->GetSessionID();
		}
	}

	// todo: ��������ڴ治�����������ߵ�������

	// ���ñ�Tick��SendCast��
	SetMsgSendCast(nAllSendCast);

	m_AllSessionMutex.Release();

	g_world.UnlockNetWork();
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID WorldSession::Destroy()
{
	// ɾ�����е�session����Ϊ�����߳��Ѿ�ֹͣ���������ﲻ��Ҫ������
	PlayerSession* pSession = m_listInsertPool.PopFront();
	while( P_VALID(pSession) )
	{
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);
		pSession = m_listInsertPool.PopFront();
	}

	m_mapGlobalSessions.Clear();
	
	m_mapAllSessions.ResetIterator();
	while( m_mapAllSessions.PeekNext(pSession) )
	{
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);
	}
	m_mapAllSessions.Clear();

	m_pNetSession->Destroy();

	// ��������ע������������GM����
	PlayerSession::UnRegisterALL();
}

//-------------------------------------------------------------------------------
// ��½�ص�����
//-------------------------------------------------------------------------------
DWORD WorldSession::LoginCallBack(LPVOID p1, LPVOID p2)
{
	static DWORD dwJoinGameCmdCrc = m_pUtil->Crc32("NC_JoinGame");

	if( g_world.IsShutingdown() )
	{
		ILOG->Write(_T("Kick,world is shuting down!!!\r\n"));
		return GT_INVALID;
	}

	// �鿴��һ����Ϣ
	tagUnit* pUnit = (tagUnit*)p1;
	tagXLoginParam* pParam = (tagXLoginParam*)p2;
	
	tagNC_JoinGame* pCmd = (tagNC_JoinGame*)pUnit->pBuffer;

	if( pCmd->dwID != dwJoinGameCmdCrc )
	{
		ILOG->Write(_T("Kick,pCmd->dwID != dwJoinGameCmdCrc Error\r\n"));
		return GT_INVALID;
	}

	if( g_world.IsWillShutdown() )
	{
		tagNS_JoinGame msg_back;
		msg_back.dwErrorCode = E_JoinGame_WorldWillShutdown;
		SendMsg(pParam->dwHandle, (LPBYTE)&msg_back, msg_back.dwSize);
		m_pNetSession->DisconnectFriendly(pParam->dwHandle);
		return 0;
	}

	if( !P_VALID(pParam->dwHandle) )
	{
		ILOG->Write(_T("Kick,vEngine Client Hanlder Error\r\n"));
		return GT_INVALID;
	}

	// ��ס����
	g_world.LockNetWork();

	// ��½�������ж��ʺ��Ƿ�Ϸ�
	BYTE byPrivilege = 0;
	BOOL bGuard = true;
	DWORD dwAccOLSec = 0;
	char szAccount[X_SHORT_NAME] = {0};
	char szKrUserID[X_SHORT_NAME] = {0};

	tagDWORDTime dwPreLoginTime;
	DWORD	dwPreLoginIP = 0;
	DWORD   dwChannelID = 0;
	if( FALSE == g_loginSession.IsAccountValid(pCmd->dwAccountID, pCmd->dwVerifyCode, byPrivilege, bGuard, 
												dwAccOLSec, szAccount,szKrUserID,dwPreLoginTime,dwPreLoginIP,dwChannelID) )
	{
		ILOG->Write(_T("Kick, Account is inValid,accountid=%d\r\n"),pCmd->dwAccountID);
		g_world.UnlockNetWork();
		return GT_INVALID;
	}

	if( IsSessionExist(pCmd->dwAccountID) )
	{
		ILOG->Write(_T("Kick, Session Already Exist!!!,accountid=%d\r\n"),pCmd->dwAccountID);
		g_world.UnlockNetWork();
		return GT_INVALID;
	}
    AddAccountLoginCount(pCmd->dwAccountID,dwPreLoginTime);
	// ����һ���µ�session
	PlayerSession* pSession = new PlayerSession(pCmd->dwAccountID, pParam->dwHandle, pParam->dwAddress, byPrivilege,
												bGuard, dwAccOLSec, szAccount,szKrUserID,dwPreLoginTime,dwPreLoginIP,dwChannelID, pCmd->szMac);
#ifdef ON_GAMEGUARD
	// ����Ǻ����汾����ʼ��ÿ��Player��CCSAuth3��Ա����
	pSession->m_CS.Init(true);
#endif
    
	// �ֱ���뵽��session��globlesession��
	AddSession(pSession);
	AddGlobalSession(pSession);

	// ���õ�½
	PlayerLogin();

	// ֪ͨ��½�������ʺŵ�½�ɹ�
	g_loginSession.SendPlayerLogin(pCmd->dwAccountID, pParam->dwAddress, E_Success, pCmd->szMac);

	// ����Ҽ�¼
	g_gameGuarder.Login(pCmd->dwAccountID, szAccount, pParam->dwAddress);

	// ����
	g_world.UnlockNetWork();

	// ���ͷ�������Ϣ
	tagNS_ServerInfo send;
	_tcscpy(send.szServer, g_world.GetWorldName());
	send.dwLoginMode = m_dwLoginMode;
	send.bEnableExpressPay = m_bEnableExpressPay;
	send.bEnableSpiritSys = g_attRes.IsSpiritSysOn();
	send.bEnableCitySys = g_activityMgr.GetCityStruggleState() == 1;
	if(m_bEnableExpressPay)
		_tcscpy(send.szExpressPayUrl, m_szExpressPayUrl);

	// �������ŻURL
	_tcscpy(send.szTodayHotActivityUrl, m_szTodayHotActivityUrl);
	// ս���һ�ս��ʩ�ż��ܺ�ʹ����Ʒʱ����
	send.dwAutoFightTimeInterval = m_dwAutoFightTimeInterval;
	
	// ����ս����
	send.bGuildWarOpen = m_bGuildWarOpen;

	send.dwServerFunctionSwitch = g_attRes.GetServerFunctionSwitch();

	pSession->SendMessage((LPBYTE)&send, send.dwSize);

	// Jason 2010-1-27 v1.3.2 ��һع�/���߽������
	g_PlayerAccountMgr.RoleLogin(pCmd->dwAccountID);

	return pSession->GetSessionID();
}

//--------------------------------------------------------------------------------------
// �ǳ��ص�����
//--------------------------------------------------------------------------------------
DWORD WorldSession::LogoutCallBack(LPVOID p)
{
	DWORD dwSessionID = (DWORD)p;

	ITRACE(_T("Log out callback, sessionid=%u\r\n"), dwSessionID);

	if( g_world.IsShutingdown() )
		return GT_INVALID;

	// ��ס����
	g_world.LockNetWork();

	PlayerSession* pSession = FindSession(dwSessionID);

	if( P_VALID(pSession) )
	{ 
        SetAccountLoginCount(pSession->GetSessionID(),pSession->GetCreatedRoleNumLimit());    
		pSession->SessionLogout();
		// Jason 2010-1-27 v1.3.2 ��һع�/���߽������
		g_PlayerAccountMgr.RoleLogout  (pSession->GetSessionID  ());
		Role *pRole = pSession->GetRole();
		if(P_VALID(pRole))
		{
			ILOG->Write(_T("Logout client, roleid is:%u\r\n"),pRole->GetID());
		}
	}

	// ��������
	g_world.UnlockNetWork();

	return 0;
}

//--------------------------------------------------------------------------------------
// ���������俪��������һ
//--------------------------------------------------------------------------------------
VOID WorldSession::IncTreasureSum()
{
	InterlockedExchangeAdd((LPLONG)&m_nTreasureSum, 1);
	if (m_nTreasureSum > SERVER_CHEST_RATE)
		InterlockedExchange((LPLONG)&m_nTreasureSum, 1);
}

//--------------------------------------------------------------------------------------
// ���÷��������俪������
//--------------------------------------------------------------------------------------
VOID WorldSession::SetTreasureSum(INT nSum)
{
	if (nSum > SERVER_CHEST_RATE)
		nSum = 1;
	InterlockedExchange((LPLONG)&m_nTreasureSum, nSum);
}

//--------------------------------------------------------------------------------------
// ��ȡ�����������AccountId
//--------------------------------------------------------------------------------------
INT WorldSession::GetAllOLAccountID( DWORD* pAccountIDs )
{
	std::list<PlayerSession*> listPlayerSessions;
	m_AllSessionMutex.Acquire();

	m_mapAllSessions.ExportAllValue(listPlayerSessions);

	INT i=0;
	std::list<PlayerSession*>::iterator itr = listPlayerSessions.begin();
	while (itr != listPlayerSessions.end())
	{
		if (P_VALID(*itr) && (*itr)->GetFatigueGuarder().IsGuard())
		{
			pAccountIDs[i++] = (*itr)->GetSessionID();
		}
		++itr;
	}

	m_AllSessionMutex.Release();

	return i;
}

VOID WorldSession::AddAccountLoginCount(DWORD dwAccountId,const tagDWORDTime& dwPreLoginTime)
{    

	  
	  INT result = m_mapAccountLoginCount.Peek(dwAccountId);
	  if ( GT_VALID(result))
	  {
		  tagDWORDTime dwCurrentDWORDTime = GetCurrentDWORDTime();
		 // DWORD dwTimeRange  =   CalcTimeDiff(dwCurrentDWORDTime,dwPreLoginTime);

		  if( dwCurrentDWORDTime.day != dwPreLoginTime.day) //�������һ��
		  {
			 
			  TObjRef<VarContainer> pVar = "VarContainer";
			  int  nLoginCount = pVar->GetDword(_T("max_role_num create_role"));
              SetAccountLoginCount(dwAccountId,nLoginCount); 
			 
		  }
		  return;
	  }

	   TObjRef<VarContainer> pVar = "VarContainer";
	   int  nLoginCount = pVar->GetDword(_T("max_role_num create_role"));
	   m_mapAccountLoginCount.Add(dwAccountId,nLoginCount);  

	  
}

INT  WorldSession::GetAccountLoginCount(DWORD dwAccountId)
{
	
      return m_mapAccountLoginCount.Peek(dwAccountId);  
	 
}

VOID   WorldSession::SetAccountLoginCount(DWORD dwAccountId,INT  nLoginCount)
{
	  
      m_mapAccountLoginCount.ChangeValue(dwAccountId,nLoginCount);
	  
}

VOID WorldSession::UpdateMediaOnlineNum()
{
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	if (CalcTimeDiff(dwCurTime, m_dwLastUpdateTime) > GetMediaOnlineRecordInterval()*60)
	{
		m_dwLastUpdateTime = dwCurTime;
		SaveOnlineNum2DB();
	}
}

VOID WorldSession::SaveOnlineNum2DB()
{
	m_mapMediaOnlineNumMutex.Acquire();

	DWORD dwDataNum = m_mapMediaOnlineNum.Size();
	if (dwDataNum == 0)
	{
		m_mapMediaOnlineNumMutex.Release();
		return;
	}

	DWORD dwMsgSize = sizeof(tagNDBC_SaveMediaOnlineNum)+(dwDataNum-1)*sizeof(tagMediaOnlineNum);
	MCREATE_MSG(pSend, dwMsgSize, NDBC_SaveMediaOnlineNum);
	if (!P_VALID(pSend))
	{
		m_mapMediaOnlineNumMutex.Release();
		return;
	}

	pSend->dwNum = dwDataNum;

	TMap<INT, DWORD>::TMapIterator iterator = m_mapMediaOnlineNum.Begin();
	DWORD dwOnlineNum = 0;
	INT nDistributionID = iterator->first;
	DWORD dwIdx = 0;
	while(m_mapMediaOnlineNum.PeekNext(iterator, dwOnlineNum))
	{
		pSend->pData[dwIdx].dwOnlineNum = dwOnlineNum;
		pSend->pData[dwIdx].nDistributionID = nDistributionID;
		if (iterator != m_mapMediaOnlineNum.End())
			nDistributionID = iterator->first;
		dwIdx++;
	}
	g_dbSession.Send(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	m_mapMediaOnlineNumMutex.Release();
}

VOID WorldSession::OnRoleLogin(INT nDistributionID)
{
	m_mapMediaOnlineNumMutex.Acquire();
	DWORD dwOnlineNum = m_mapMediaOnlineNum.Peek(nDistributionID);
	if (dwOnlineNum != GT_INVALID)
		m_mapMediaOnlineNum.ChangeValue(nDistributionID, ++dwOnlineNum);
	else
		m_mapMediaOnlineNum.Add(nDistributionID, 1);
	m_mapMediaOnlineNumMutex.Release();
}

VOID WorldSession::OnRoleLogout(INT nDistributionID)
{
	m_mapMediaOnlineNumMutex.Acquire();
	DWORD dwOnlineNum = m_mapMediaOnlineNum.Peek(nDistributionID);
	if (dwOnlineNum != GT_INVALID && dwOnlineNum > 0)
		m_mapMediaOnlineNum.ChangeValue(nDistributionID, --dwOnlineNum);
	m_mapMediaOnlineNumMutex.Release();
}

BOOL WorldSession::GetDBQBOpenState(DWORD dwWeekDay)
{
	if (dwWeekDay >= 0 && dwWeekDay < DAYS_ONE_WEEK)
	{
		return m_bDBQBOpenState[dwWeekDay];
	}
	return FALSE;
}
VOID WorldSession::SendMsg(DWORD dwInternalIndex, LPBYTE pMsg, DWORD dwSize)
{
	InterlockedExchangeAdd((LPLONG)&m_nMsgSendThisTick, 1);
	m_pNetSession->Send(dwInternalIndex, pMsg, dwSize);
	g_world.CountNetMsg( pMsg, dwSize );
}
VOID WorldSession::PrintRoleInfo()
{
	PlayerSession* temp;
	ofstream wFileStream;
	CHAR sFileName[128] = {0};
	tagDWORDTime now = GetCurrentDWORDTime();
	_snprintf(sFileName, sizeof(sFileName)/sizeof(CHAR),  "log\\RoleInfo_20%d-%d-%d-%d-%d-%d.txt", now.year, now.month, now.day, now.hour, now.min, now.sec);
	wFileStream.open(sFileName,ios::out|ios::app/*|ios::trunc*/);
	for(int i = 0; i < 100; i++)
	{
		if(m_mapAllSessions.PeekNext(temp))
		{
			if (wFileStream)
			{
				Role* pRole = temp->GetRole();
				if(P_VALID(pRole))
					wFileStream << "Account:" <<temp->GetAccount() << "  RoleId:" << pRole->GetID()<< "  Level:" << pRole->GetLevel() << "  Class:" << pRole->GetClass() << "  MapCrc:"<< pRole->GetMapID() <<endl;
			}
		}
		else
		{
			m_bIsPrintInfo = FALSE;
			break;
		}
	}
	wFileStream.close();
}
WorldSession g_worldSession;
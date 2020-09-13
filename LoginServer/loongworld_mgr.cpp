//-----------------------------------------------------------------------------
//!\file loongworld_mgr.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief �����������ӽ���LoongWorld,ӵ��StreamServer,������Ϣ������LoongWorld���͵İ�
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_rt_errorcode.h"
#include "../ServerDefine/login_issue.h"
#include "../ServerDefine/login_define.h"
#include "../WorldDefine/msg_family.h"

#include "loongworld_mgr.h"
#include "loongworld.h"
#include "login_server.h"
#include "db.h"
#include "fatigue_mgr.h"
#include "proof_policy.h"
#include "proof_policy_orange.h"

//------------------------------------------------------------------------------
// ���캯��
//------------------------------------------------------------------------------
LoongWorldMgr::LoongWorldMgr():m_Trunk(this)
{

}

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
LoongWorldMgr::~LoongWorldMgr()
{
    Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL LoongWorldMgr::Init()
{
    m_pUtil = "Util";

    // �������ļ�����LoongWorld������
	TObjRef<VarContainer> pVar = "LoginServerConfig";

	INT nWorldNum = pVar->GetDword(_T("num zone_server"));

    for(INT n = 0; n < nWorldNum; ++n)
    {
		// ����world
        LoongWorld* pWorld = new LoongWorld;

		if( FALSE == pWorld->Init(n) )
		{
			SAFE_DEL(pWorld);
			return FALSE;
		}

        m_mapLoongWorld.Add(pWorld->GetID(), pWorld);
		
		//��ʼ�����ݿ�����  ȫ��״̬���-1 songg
		BOOL bRet = sBeton.InsertWorldState(pWorld->GetWorldID());
		if(!bRet)
		{
			sBeton.UpdateWorldState(pWorld->GetWorldID(),0,-1);
		}

		//��¼log
		sBeton.InsertWorldStateLog(pWorld->GetWorldID(),0,-1,pWorld->GetQueueRoleNum());
    }

    if( m_mapLoongWorld.Empty() )
        return FALSE;

    // ��ȡ��ɫ����
    m_dwLoongWorldGoldenCode = pVar->GetDword(_T("zone_server golden_code"));
	m_dwLoginServerGoldenCode = pVar->GetDword(_T("login_server golden_code"));

	// �����������������
	CreateObj("NetCmdMgrWorld", "NetCmdMgr");
	m_pMsgCmdMgr = "NetCmdMgrWorld";
	if( !P_VALID(m_pMsgCmdMgr) ) return FALSE;

	// ���ɷ���������ӿ�
	CreateObj("StreamServerWorld", "StreamServer");
	m_pSession = "StreamServerWorld";
	if( !P_VALID(m_pSession) ) return FALSE;

	// ע����������
	RegisterLoongWorldMsg();

	// ��ʼ������
	INT nPort = pVar->GetInt(_T("port"), _T("zone_session"));
	m_pSession->Init((TCPLOGINCALLBACK)m_Trunk.sfp2(&LoongWorldMgr::LoginCallBack),
		(TCPLOGOUTCALLBACK)m_Trunk.sfp1(&LoongWorldMgr::LogoutCallBack), nPort);

	return TRUE;
}

//-------------------------------------------------------------------------------
// ���ٺ���
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::Destroy()
{
    // ���LoongWorld����
    LoongWorld* pLoongWorld = NULL;


	//�������״̬��
	sBeton.ClearWorldStateTable();

    m_mapLoongWorld.ResetIterator();
    while( m_mapLoongWorld.PeekNext(pLoongWorld) )
    {
        SAFE_DEL(pLoongWorld);
    }
	m_mapLoongWorld.Clear();

	// ע������
	m_pSession->Destroy();
	KillObj("StreamServerWorld");

	// ע�����е���������
	UnRegisterLoongWorldMsg();
	KillObj("NetCmdMgrWorld");
}

//-------------------------------------------------------------------------------
// ���º���
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::Update()
{
	// ����ÿһ��world
	LoongWorld* pLoongWorld = NULL;

	m_mapLoongWorld.ResetIterator();
	while( m_mapLoongWorld.PeekNext(pLoongWorld) )
	{
		pLoongWorld->Update();
		//��������״̬
		UpdateWorldState(pLoongWorld);
		
	}
	UpdatePKPunishmentMap();
}
//-------------------------------------------------------------------------------
// ��������״̬
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::UpdateWorldState(LoongWorld* pLoongWorld)
{
	//��5�����һ�����ݿ� songg
	DWORD dwCurTime = timeGetTime();
	if( (dwCurTime - pLoongWorld->GetDBUpdateTime()) > sBeton.GetWorldStateUpdateTime() )
	{
		pLoongWorld->SetDBUpdateTime(dwCurTime);

		if(pLoongWorld->GetStatus() == EWS_Well && pLoongWorld->IsValid()) //��½��û�ǳ�
		{
			SHORT  sState = ((DOUBLE)pLoongWorld->GetCurrOnlineDisplay()/pLoongWorld->GetMaxOnlineNum())*100; //�ٷֱ�

			//ˢ������״̬��DB
			sBeton.UpdateWorldState(pLoongWorld->GetWorldID(),pLoongWorld->GetCurrOnlineDisplay(),sState);
		}
		else
		{
			//ˢ������״̬��DB
			sBeton.UpdateWorldState(pLoongWorld->GetWorldID(),0,-1);
		}

	}
	if( (dwCurTime - pLoongWorld->GetDBInsertTime()) > sBeton.GetWorldStateInsertTime() )
	{
		pLoongWorld->SetDBInsertTime(dwCurTime);

		if(pLoongWorld->GetStatus() == EWS_Well && pLoongWorld->IsValid()) //��½��û�ǳ�
		{
			SHORT  sState = ((DOUBLE)pLoongWorld->GetCurrOnlineDisplay()/pLoongWorld->GetMaxOnlineNum())*100; //�ٷֱ�

			//����״̬��log
			sBeton.InsertWorldStateLog(pLoongWorld->GetWorldID(),pLoongWorld->GetCurrOnlineDisplay(),sState, pLoongWorld->GetQueueRoleNum());
		}
		else
		{
			//����״̬��log
			sBeton.InsertWorldStateLog(pLoongWorld->GetWorldID(),0,-1,pLoongWorld->GetQueueRoleNum());
		}

	}
}
//-------------------------------------------------------------------------------
// ע��������Ϣ
//-------------------------------------------------------------------------------
VOID LoongWorldMgr::RegisterLoongWorldMsg()
{
	m_pMsgCmdMgr->Register("NWL_CreateGuild",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleCreateGuild),	    _T("NWL_CreateGuild") );
	m_pMsgCmdMgr->Register("NWL_CreateFamily",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleCreateFamily),	    _T("NWL_CreateFamily") );
    m_pMsgCmdMgr->Register("NWL_CreateRole",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleCreateRole),		    _T("NWL_CreateRole") );
	m_pMsgCmdMgr->Register("NWL_Certification",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleCertification),		_T("NWL_Certification") );
	m_pMsgCmdMgr->Register("NWL_WorldStatus",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleZoneServerStatus),	_T("NWL_WorldStatus") );
	m_pMsgCmdMgr->Register("NWL_PlayerWillLogin",	(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerWillLogin),	_T("NWL_PlayerWillLogin") );
	m_pMsgCmdMgr->Register("NWL_PlayerLogin",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerLogin),		_T("NWL_PlayerLogin") );
	m_pMsgCmdMgr->Register("NWL_PlayerLogout",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerLogout),		_T("NWL_PlayerLogout") );
	m_pMsgCmdMgr->Register("NWL_KickLog",			(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleKickLog),			_T("NWL_KickLog") );
	m_pMsgCmdMgr->Register("NWL_WorldColsed",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleWorldClosed),		_T("NWL_WorldColsed") );
	m_pMsgCmdMgr->Register("NWL_ForbidAccount",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleForbidAccount),		_T("NWL_ForbidAccount") );
	// Jason 2010-4-27 v2.1.0
	m_pMsgCmdMgr->Register("NWL_PlayerPKRunaway",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandlePlayerPKRunaway),		_T("NWL_PlayerPKRunaway") );
	m_pMsgCmdMgr->Register("NWL_FatigueTimer",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleFatigueTimer),		_T("NWL_FatigueTimer") );
	// Jason 2010-11-24 v1.2.2
	m_pMsgCmdMgr->Register("NWL_GetVipNetbarInfo",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleGetNetbarInfo),		_T("NWL_GetVipNetbarInfo") );

	m_pMsgCmdMgr->Register("NWL_SaveTelNum",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleSaveTelNum),		_T("NWL_SaveTelNum") );

	m_pMsgCmdMgr->Register("NWL_NotSaveTelNum",		(NETMSGPROC)m_Trunk.sfp2(&LoongWorldMgr::HandleNotSaveTelNum),		_T("NWL_NotSaveTelNum") );	

}

//------------------------------------------------------------------------
// ��ע��������Ϣ
//------------------------------------------------------------------------
VOID LoongWorldMgr::UnRegisterLoongWorldMsg()
{
	m_pMsgCmdMgr->Destroy();
}

//------------------------------------------------------------------------
// ��½�ص�
//------------------------------------------------------------------------
DWORD LoongWorldMgr::LoginCallBack(LPBYTE pByte, DWORD dwSize)
{
	tagNWL_Certification* pRecv = (tagNWL_Certification*)pByte;

	// ��֤LoongWorld,�Ƚ������ļ���ɫ����
	if( pRecv->dwGoldenCode != m_dwLoongWorldGoldenCode )
		return GT_INVALID;

	// ��ס������
	g_login.LockUpdate();

	// �õ�world��id
	DWORD dwNameCrc = m_pUtil->Crc32(pRecv->szWorldName);

	// �ҵ�World
	LoongWorld* pWorld = GetLoongWorld(dwNameCrc);
	if( !P_VALID(pWorld) )
	{
		IMSG(_T("one invalid world login, name=%s\r\n"), pRecv->szWorldName);

		// �ͷ���
		g_login.UnlockUpdate();

		return GT_INVALID;
	}

	// ���Գ�ʼ��
	if( !pWorld->WorldLogin(pRecv->dwIP, pRecv->dwPort, pRecv->dwOLAccountID, pRecv->nOLAccountNum) )
	{
		IMSG(_T("one world login which is already online, name=%s\r\n"), pRecv->szWorldName);

		// �ͷ���
		g_login.UnlockUpdate();

		return GT_INVALID;
	}

	//ALEX 2010/06/03 �Ϸ����б���
    //pWorld����Ӹ���������
	  std::vector<tstring> tokens;
	  m_pUtil->StringToToken(tokens, pWorld->GetszAttachServers()  ,_T('^')); 
      pWorld->ClearChildLoongWorld();
	  for (INT n=0;n<tokens.size();n++)
	  {
		  LPCTSTR sz=tokens[n].c_str();
		  DWORD dwChildNameCrc = m_pUtil->Crc32(sz);
		  LoongWorld* pw = GetLoongWorld(dwChildNameCrc);
		  if(P_VALID(pw))
		  { 
			  pWorld->AddChildLoongWorld(pw); 
			  pw->SetParentServerCrc(dwNameCrc); 
		  }
	  }
 

	// ��ʼ���ɹ�
	IMSG(_T("Hello World����%s\r\n"),pRecv->szWorldName);

	// �ͷ���
	g_login.UnlockUpdate();

	// ����ID
	return dwNameCrc;
}

//----------------------------------------------------------------------------
// �ǳ��ص�
//----------------------------------------------------------------------------
DWORD LoongWorldMgr::LogoutCallBack(DWORD dwParam)
{
	DWORD dwWorldID = dwParam;

	// ��ס������
	g_login.LockUpdate();

	// ����world
	LoongWorld* pWorld = GetLoongWorld(dwWorldID);
	if( !P_VALID(pWorld) )
	{
		// �ͷ���
		g_login.UnlockUpdate();

		return 0;
	}

	// ���Եǳ�
	pWorld->WorldLogout();
	IMSG(_T("Bye LoongWorld����%s\r\n"), pWorld->GetName());

	// �ͷ���
	g_login.UnlockUpdate();

	return 0;
}

//------------------------------------------------------------------------------
// ��ӵ���Ϸ����
//------------------------------------------------------------------------------
VOID LoongWorldMgr::AddToWorld(Player* pPlayer, DWORD dwWorldNameCrc)
{
	if( !P_VALID(pPlayer) ) return;

	LoongWorld* pWorld = GetLoongWorld(dwWorldNameCrc);
	if( !P_VALID(pWorld) ) return;

	pWorld->AddPlayer(pPlayer);

	// д�����ݿ�
	sBeton.PlayerLogin(pPlayer->GetAccountID(), dwWorldNameCrc);
}

//------------------------------------------------------------------------------
// ������֤�����Ƿ��ظ���Ϣ
//------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandleCreateGuild(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandleCreateGuild \r\n"));

	  tagNWL_CreateGuild* pRecv = (tagNWL_CreateGuild*)pMsg;

	  tagNLW_CreateGuild Msg;
	  BOOL bRet = sBeton.InsertGuildName(pRecv->dwAccountID,pRecv->szGuildName);
	  if (!bRet)
	  {
		  Msg.dwErrorCode = 26;
	  }
	  else
	  {
		  Msg.dwErrorCode = E_Success;
	  }
	  Msg.dwAccountID = pRecv->dwAccountID;
	  Msg.dwNPCID     = pRecv->dwNPCID;
	  Msg.dwRoleID    = pRecv->dwRoleID;
	  _tcscpy_s(Msg.szGuildName,X_SHORT_NAME,pRecv->szGuildName);
	   SendMsg(pWorld->GetID(),&Msg,Msg.dwSize);

	   ILOG->Write(_T("HandleCreateGuild end \r\n"));
	  return 0;
}

DWORD		LoongWorldMgr::HandleCreateFamily(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandleCreateFamily \r\n"));

	tagNWL_CreateFamily* pRecv = (tagNWL_CreateFamily*)pMsg;

	tagNLW_CreateFamily Msg;
	BOOL bRet = sBeton.InsertFamilyName(pRecv->dwAccountID,pRecv->szFamilyName);
	if (!bRet)
	{
		Msg.dwErrorCode = E_CreateFamily_NameExist;
	}
	else
	{
		Msg.dwErrorCode = E_FAMILY_SUCCESS;
	}
	Msg.dwAccountID = pRecv->dwAccountID;
	Msg.dwNPCID     = pRecv->dwNPCID;
	Msg.dwRoleID    = pRecv->dwRoleID;
	_tcscpy_s(Msg.szFamilyName,X_SHORT_NAME,pRecv->szFamilyName);
	SendMsg(pWorld->GetID(),&Msg,Msg.dwSize);

	ILOG->Write(_T("HandleCreateFamily end \r\n"));

	return 0;
}

//------------------------------------------------------------------------------
// ������֤�����Ƿ��ظ���Ϣ
//------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandleCreateRole(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandleCreateRole \r\n"));

     tagNWL_CreateRole* pRecv = (tagNWL_CreateRole*)pMsg;    
	

     tagNLW_CreateRole  Msg;
	 BOOL bRet = sBeton.InsertRoleName(pRecv->dwAccountID,pRecv->szRoleName);
	 if (!bRet)
	 {
		 Msg.dwErrorCode = E_DBInsert_RoleNameExist;
	 }
	 else
	 {
		 Msg.dwErrorCode = E_Success;
	 }
	 Msg.dwAccountID = pRecv->dwAccountID;
	 Msg.eStatus     = pRecv->eStatus;
	 memcpy(&Msg.AvatarAtt,&pRecv->AvatarAtt,sizeof(pRecv->AvatarAtt));
	 _tcscpy_s(Msg.szRoleName,X_SHORT_NAME,pRecv->szRoleName);
	 SendMsg(pWorld->GetID(),&Msg,Msg.dwSize);

	 ILOG->Write(_T("HandleCreateRole end \r\n"));
     return  0;

}

// ��������ֻ���
DWORD LoongWorldMgr::HandleSaveTelNum(tagNetCmd * pMsg, LoongWorld * pWorld)
{
	ILOG->Write(_T("HandleSaveTelNum \r\n"));

	tagNWL_SaveTelNum* pRecv = (tagNWL_SaveTelNum*)pMsg;    


	tagNLW_SaveTelNum  Msg;
	Msg.dwRoleID = pRecv->dwRoleID;	

	BOOL bRet = sBeton.InsertTelNum(pRecv->dwAccountID, pRecv->szAccount, pRecv->n64TelNum);
	if (!bRet)
	{
		Msg.dwErrorCode = E_DBInsert_TelNumFailed;
	}
	else
	{
		Msg.dwErrorCode = E_Success;
	}	
	
	SendMsg(pWorld->GetID(),&Msg,Msg.dwSize);

	// �޸���ҵı����ֻ��ű�־
	sBeton.UpdateSaveNumFlag(pRecv->dwAccountID, 0);


	ILOG->Write(_T("HandleSaveTelNum end \r\n"));
	return  0;
}

// ��Ҳ��洢�ֻ���
DWORD		LoongWorldMgr::HandleNotSaveTelNum(tagNetCmd * pMsg, LoongWorld * pWorld)
{
	ILOG->Write(_T("HandleNotSaveTelNum \r\n"));

	tagNWL_NotSaveTelNum* pRecv = (tagNWL_NotSaveTelNum*)pMsg; 

	// �޸���ҵı����ֻ��ű�־
	sBeton.UpdateSaveNumFlag(pRecv->dwAccountID, 0);

	ILOG->Write(_T("HandleNotSaveTelNum end \r\n"));
	return  0;
}


//------------------------------------------------------------------------------
// ������֤��Ϣ
//------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandleCertification(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandleCertification \r\n"));

	tagNWL_Certification* pRecv = (tagNWL_Certification*)pMsg;

	// ���ͷ�����Ϣ
	tagNLW_Certification send;
	send.dwGoldenCode = m_dwLoginServerGoldenCode;

	//̨����֤
	if(sPlayerMgr.GetProofPolicy() == EPP_Orange)
	{
		send.bOrangeVipBar = TRUE;
	}

	SendMsg(pWorld->GetID(), &send, send.dwSize);

	ILOG->Write(_T("HandleCertification end \r\n"));
	return 0;
}

//-------------------------------------------------------------------------------
// �����������Ϣ��Ϣ
//-------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandleZoneServerStatus(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	//ILOG->Write(_T("HandleZoneServerStatus \r\n"));

	tagNWL_WorldStatus* pRecv = (tagNWL_WorldStatus*)pMsg;

	pWorld->UpdateStatus(pRecv->eStatus, pRecv->nCurPlayerNum, pRecv->nPlayerNumLimit, pRecv->nCurPlayerDisplay);
	pWorld->UpdateChildLoongServerStatus(pRecv->eStatus, pRecv->nCurPlayerNum, pRecv->nPlayerNumLimit, pRecv->nCurPlayerDisplay); 

	//ILOG->Write(_T("HandleZoneServerStatus end \r\n"));
    
 
	
	
	return 0;
}

//--------------------------------------------------------------------------------
// ��ҵ�½
//--------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandlePlayerLogin(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandlePlayerLogin \r\n"));

	tagNWL_PlayerLogin* pRecv = (tagNWL_PlayerLogin*)pMsg;

	// �����½���ɹ�
	if( E_Success != pRecv->dwErrorCode )
	{
		ILOG->Write(_T("Player Login Failure! AccountID:%u ErrorCode=%d\r\n"), pRecv->dwAccountID, pRecv->dwErrorCode);

		// �������ݿ�
		sBeton.PlayerLogout(pRecv->dwAccountID);
		if (g_login.IsEnableFatigueGuard())
			sFatigueMgr.LogoutNotify(pRecv->dwAccountID);
	}
	// �����¼�ɹ�
	else
	{
		// �������ݿ�
		sBeton.PlayerEnterWorld(pRecv->dwAccountID, pWorld->GetID());

		// login log
		const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pRecv->dwAccountID);
		if (!P_VALID(pAccountData))
		{
			ASSERT(0);
			ILOG->Write(_T("HandlePlayerLogin end1 \r\n"));
			return GT_INVALID;
		}

		if (g_login.IsEnableFatigueGuard() && sPlayerMgr.GetProofPolicy() != EPP_Vietnam)
			sFatigueMgr.LoginNotify(pRecv->dwAccountID, pWorld->GetID(), pAccountData->bGuard);

		//��¼���ε�¼��ip��time ���´�ʹ��
		sBeton.UpdateAccountLoginInfo(pRecv->dwAccountID, pRecv->dwIP);

		//дlogin��log
		sBeton.LogPlayerAction(pRecv->dwAccountID, pAccountData->szAccountName, pRecv->dwIP, "login",pRecv->szMac, pRecv->dwWorldID);
		//sBeton.LogPlayerAction(pRecv->dwAccountID, pAccountData->szAccountName, pRecv->dwIP, "login",pRecv->szMac);
		sPlayerMgr.CacheIpAddres(pRecv->dwAccountID, pRecv->dwIP);
		sPlayerMgr.CacheMac(pRecv->dwAccountID, pRecv->szMac);
	}

	ILOG->Write(_T("HandlePlayerLogin end2 \r\n"));

	return 0;
}

//-----------------------------------------------------------------------------------
// ��ҵǳ�
//-----------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandlePlayerLogout(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandlePlayerLogout \r\n"));

	tagNWL_PlayerLogout* pRecv = (tagNWL_PlayerLogout*)pMsg;

	// �������ݿ�
	sBeton.PlayerOutWorld(pRecv->dwAccountID, pWorld->GetID());

	// logout log
	const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pRecv->dwAccountID);
	if (!P_VALID(pAccountData))
	{
		ASSERT(0);
		ILOG->Write(_T("HandlePlayerLogout end1 \r\n"));
		return GT_INVALID;
	}

	//orange����Ҫ�ǳ���ʱ�򷢸���Ϣ����
	if(sPlayerMgr.GetProofPolicy() == EPP_Orange)
	{
		ProofPolicy* pPolicy =	sPlayerMgr.GetProofPolicyPtr();
		if(P_VALID(pPolicy))
		{
			pPolicy->SigPlayerLogoutEvent(pRecv->dwAccountID);
		}
	}

	sBeton.LogPlayerAction(pRecv->dwAccountID, pAccountData->szAccountName, pAccountData->dwIp, "logout", pAccountData->szMac, pRecv->dwWorldID);
	//sBeton.LogPlayerAction(pRecv->dwAccountID, pAccountData->szAccountName, pAccountData->dwIp, "logout", pAccountData->szMac);

	if (g_login.IsEnableFatigueGuard() && sPlayerMgr.GetProofPolicy() != EPP_Vietnam)
		sFatigueMgr.LogoutNotify(pRecv->dwAccountID);

	// Jason 2010-4-27 v2.1.0
	stdext::hash_map<DWORD,DWORD>::iterator i = m_hmapPlyPKPunishmentState.find(pRecv->dwAccountID);
	if( i != m_hmapPlyPKPunishmentState.end() )
	{
		m_hmapPlyPKPunishmentState.erase(i);
	}

	ILOG->Write(_T("HandlePlayerLogout end2 \r\n"));
	return 0;
}

//------------------------------------------------------------------------------------
// LoongWorld֪ͨ������Ϸ�������󷵻ؽ��
//-------------------------------------------------------------------------------------
DWORD LoongWorldMgr::HandlePlayerWillLogin(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandlePlayerWillLogin \r\n"));

	tagNWL_PlayerWillLogin* pRecv = (tagNWL_PlayerWillLogin*)pMsg;

	pWorld->PlayerWillLoginRet(pRecv->dwAccountID, pRecv->dwErrorCode);

	ILOG->Write(_T("HandlePlayerWillLogin end \r\n"));

	return 0;
}
//-----------------------------------------------------------------------
// LoongWorld�����ر�
//-----------------------------------------------------------------------
DWORD LoongWorldMgr::HandleWorldClosed(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandleWorldClosed \r\n"));
	//�ı��Ӧ��ҵĵ�¼״̬
	sBeton.FixOneWorldPlayerLoginStatus(pWorld->GetID(), EPLS_Null, EPLS_OffLine);

	tagNLW_WorldColsed send;
	SendMsg(pWorld->GetID(), &send, send.dwSize);

	ILOG->Write(_T("HandleWorldClosed end \r\n"));
	return 0;
}
//-----------------------------------------------------------------------
// LoongWorld�߳����
//-----------------------------------------------------------------------
DWORD LoongWorldMgr::HandleKickLog(tagNetCmd* pMsg, LoongWorld* pWorld)
{
	ILOG->Write(_T("HandleKickLog \r\n"));

	tagNWL_KickLog* pRecv = (tagNWL_KickLog*)pMsg;

	const tagAccountData* pAccountData = sPlayerMgr.GetCachedAccountData(pRecv->dwAccountID);
	if (!P_VALID(pAccountData))
	{
		ASSERT(0);
		ILOG->Write(_T("HandleKickLog end1 \r\n"));
		return GT_INVALID;
	}

// 	ILOG->Write(_T("Kick Name: %s  AccountID: %d  Time: %d  Seal: %d ErrCode: %d\r\n "), 
// 				pAccountData->szAccountName, pRecv->dwAccountID, pRecv->dwKickTime, pRecv->bySeal, pRecv->u16ErrCode);

	sBeton.InsertKickLog(pAccountData->szAccountName, pRecv->dwAccountID, pRecv->dwKickTime, pRecv->u16ErrCode, pRecv->bySeal);
	if (pRecv->bySeal && pWorld->IsAutoSeal())
	{
		//
		DWORD dwErrorCode = sBeton.ForbidAccount(pRecv->dwAccountID, EPLM_WaiGua);
		if(dwErrorCode == E_RT_Success)
		{
			if( P_VALID(pWorld) )
			{
				tagNLW_KickPlayer send;
				send.dwAccountID = pRecv->dwAccountID;
				pWorld->SendKickPlayerMsg(send);
			}
		}
	}

	ILOG->Write(_T("HandleKickLog end2 \r\n"));
	return 0;
}

//-----------------------------------------------------------------------
// LoongWorld�߳����
//-----------------------------------------------------------------------
DWORD LoongWorldMgr::HandleForbidAccount( tagNetCmd* pMsg, LoongWorld* pWorld )
{
	ILOG->Write(_T("HandleForbidAccount \r\n"));

	tagNWL_ForbidAccount* pRecv = (tagNWL_ForbidAccount*)pMsg;

	//
	DWORD dwErrorCode = sBeton.ForbidAccount(pRecv->dwAccountID, pRecv->dwForbidCode);
	if(dwErrorCode == E_RT_Success)
	{
		if( P_VALID(pWorld) )
		{
			tagNLW_KickPlayer send;
			send.dwAccountID = pRecv->dwAccountID;
			pWorld->SendKickPlayerMsg(send);
		}
	}

	ILOG->Write(_T("HandleForbidAccount end \r\n"));

	return E_Success;
}
// Jason 2010-4-27 v2.1.0
DWORD		LoongWorldMgr::HandlePlayerPKRunaway(tagNetCmd*pMsg,LoongWorld*pWord)
{
	ILOG->Write(_T("HandlePlayerPKRunaway \r\n"));

	tagNWL_PlayerPKRunaway * p = (tagNWL_PlayerPKRunaway*)pMsg;
	if( !P_VALID(p) )
	{
		ILOG->Write(_T("HandlePlayerPKRunaway end1 \r\n"));
		return GT_INVALID;
	}
	m_hmapPlyPKPunishmentState[p->dwAccountID] = p->dwTicksWantedToWait;

	ILOG->Write(_T("HandlePlayerPKRunaway end2 \r\n"));
	return E_Success;
}

VOID LoongWorldMgr::UpdatePKPunishmentMap(VOID)
{
	for(stdext::hash_map<DWORD, DWORD>::iterator i = m_hmapPlyPKPunishmentState.begin(); i != m_hmapPlyPKPunishmentState.end(); )
	{
		i->second = i->second - 1;
		if( i->second <= 0 )
		{
			m_hmapPlyPKPunishmentState.erase(i++);
		}
		else
			++i;
	}
}
BOOL	LoongWorldMgr::IsInPKPunishmentState(DWORD dwAccountID)
{
	stdext::hash_map<DWORD,DWORD>::iterator i = m_hmapPlyPKPunishmentState.find(dwAccountID);
	if( i != m_hmapPlyPKPunishmentState.end() )
	{
		return TRUE;
	}
	return FALSE;
}


DWORD LoongWorldMgr::HandleFatigueTimer(tagNetCmd*pMsg, LoongWorld*pWorld)
{
	ILOG->Write(_T("HandleFatigueTimer \r\n"));

	if (!g_login.IsEnableFatigueGuard())
	{
		ILOG->Write(_T("HandleFatigueTimer end1\r\n"));
		return GT_INVALID;
	}

	if (sPlayerMgr.GetProofPolicy() != EPP_Vietnam)
	{
		ILOG->Write(_T("HandleFatigueTimer end2\r\n"));
		return GT_INVALID;
	}

	tagNWL_FatigueTimer* pRecv = (tagNWL_FatigueTimer*)pMsg;
	if (!P_VALID(pRecv))
	{
		ILOG->Write(_T("HandleFatigueTimer end3\r\n"));
		return GT_INVALID;
	}

	// ֪ͨ�����Է��������˺ŵ�½����ʼ��ʱ
	if (pRecv->byEnterMap == 1)
	{
		sFatigueMgr.LoginNotify(pRecv->dwAccountID, pWorld->GetID(), TRUE);
	}
	// ֪ͨ�����Է��������˺ŵǳ���Ϸ���磬������ʱ
	else if (pRecv->byEnterMap == 0)
	{
		sFatigueMgr.LogoutNotify(pRecv->dwAccountID);
	}

	ILOG->Write(_T("HandleFatigueTimer end4\r\n"));
	return E_Success;
}

DWORD	LoongWorldMgr::HandleGetNetbarInfo(tagNetCmd * pMsg, LoongWorld * pWorld)
{
	ILOG->Write(_T("HandleGetNetbarInfo \r\n"));

	tagNWL_GetVipNetbarInfo * p = (tagNWL_GetVipNetbarInfo*)pMsg;
	if( !P_VALID(p) )
	{
		ILOG->Write(_T("LoongWorldMgr::HandleGetNetbarInfo-->tagNWL_GetVipNetbarInfo msg is invalid!!\r\n"));
		return GT_INVALID;
	}
	tagNLW_IssueVipNetbarInfo * msg = NULL;
	if( sBeton.GetNetbarInfo(msg) && P_VALID(msg) )
	{
		sLoongWorldMgr.SendMsg(pWorld->GetID(),msg,msg->dwSize);
		sBeton.FreeNetbarInfo(msg);
	}

	ILOG->Write(_T("HandleGetNetbarInfo end\r\n"));
	return E_Success;
}

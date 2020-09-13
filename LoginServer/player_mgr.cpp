//-----------------------------------------------------------------------------
//!\file player_mgr.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ��ҹ�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/login.h"

#include "player_mgr.h"
#include "player.h"
#include "loongworld_mgr.h"
#include "loongworld.h"
#include "db.h"
#include "proof_policy_mgr.h"
#include "proof_policy.h"
#include "login_server.h"
#include "yauthLib.h"
#include "kr_util.h"
#include "rijndael.h"
#include "event_mgr.h"
#include "proof_policy_cornEx.h"
Rijndael  g_KrUtil;
using namespace namepolicy;

//-------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------
PlayerMgr::PlayerMgr() : m_Trunk(this), m_nPlayerNum(0), m_nPlayerLoging(0), m_nProofResultNum(0),
						m_dwClientIDGen(0), m_dwCurrVer(0), m_nPort(0), m_ePolicy(EPP_Null),m_pPolicy(NULL),m_bUseGMAccess(TRUE),
						m_bYYMixSerOn(false),m_hInstYYDLL(NULL),m_pYAuth_init(NULL),m_pYAuth_cleanUp(NULL),m_YAuth_genRequest(NULL),
						m_pYAuth_verifyToken(NULL),m_pYAuth_HexToString(NULL),m_pYAuth_GetAttribute(NULL),m_bWebLoginModeOn(false),m_dwChannelNum(0)
						
{
}

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
PlayerMgr::~PlayerMgr()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL PlayerMgr::Init()
{
	// vEngine
	m_pUtil = "Util";

	// ��ȡ�����ļ�
	TObjRef<VarContainer> pVar = "LoginServerConfig";

	//��ʼ��
	if(!InitConfig())
	{
		return FALSE;
	}

	sBeton.LoadCacheAccountData(m_mapAccountData, m_mapAccountNameCrc2AccountID);

	// ��ȡ��֤��ʽ
	m_ePolicy = (enum EProofPolicy)pVar->GetInt(_T("policy"), _T("proof"));
	if( m_ePolicy < EPP_Test || m_ePolicy >= EPP_End || 8 == m_ePolicy || 9 == m_ePolicy)
	{
		return FALSE;
	}

	m_bYYMixSerOn = (pVar->GetInt(_T("YY_On"), _T("mixedserver")) == 0) ? false : true;
     
	m_bWebLoginModeOn = (pVar->GetInt(_T("WebLogin_On"), _T("web_loginmode")) == 0) ? false : true;

	// ������֤��ʽ
	m_pPolicy = sProofPolicyMgr.CreateProofPolicy(m_ePolicy);
	if( !P_VALID(m_pPolicy) )
	{
		return FALSE;
	}
	if( !m_pPolicy->Init((PROOFCALLBACK)m_Trunk.sfp2(&PlayerMgr::ProofCallBack)) )
	{
		return FALSE;
	}
	g_KrUtil.init(Rijndael::CBC, Rijndael::Decrypt, key, Rijndael::Key32Bytes, iv);

	// ��ȡ�汾��
	LPCTSTR szVerNum = pVar->GetString(_T("version version"));
	m_dwCurrVer = m_pUtil->Crc32(szVerNum);

	// ��ȡ�˿ں�
	m_nPort = pVar->GetInt(_T("port player_session"));

	// ��ʼ������
	m_pNetSession = "XServer";

	tagXServerConfig InitParam;
	InitParam.fnLogIn			=	(XLOGINCALLBACK)m_Trunk.sfp2(&PlayerMgr::LoginCallBack);
	InitParam.fnLogOut			=	(XLOGOUTCALLBACK)m_Trunk.sfp1(&PlayerMgr::LogoutCallBack);
	InitParam.bReusePort		=	true;
	InitParam.nPort				=	m_nPort;
	InitParam.nMaxServerLoad	=	20000;
	InitParam.nAcceptExNum		=	128;

	m_pNetSession->Init(InitParam);
	m_nPort = m_pNetSession->GetConfig()->nPort;

	// ע��������Ϣ
	Player::RegisterPlayerMsg();



	//YY�Ľӿڳ�ʼ��
	if ( m_bYYMixSerOn )
	{
		m_hInstYYDLL = LoadLibrary(_T("yauth.conf\\yauthlib.dll"));
		if ( NULL == m_hInstYYDLL)
		{
			::MessageBox(0,_T("ȱ�ٻ��YY����ƽ̨��̬��!"),_T("����"),0);
			return FALSE;
		}
		m_pYAuth_init = (PtrInitType)GetProcAddress(m_hInstYYDLL,"YAuth_init");

		m_pYAuth_cleanUp = (PtrCleanUpType)GetProcAddress(m_hInstYYDLL,"YAuth_cleanUp");

		m_YAuth_genRequest = (PtrGenRequestTypeEx)GetProcAddress(m_hInstYYDLL,"YAuth_genRequest");

		m_pYAuth_verifyToken = (PtrVerifyTokenType)GetProcAddress(m_hInstYYDLL,"YAuth_verifyToken");

		m_pYAuth_HexToString = (PtrHexToStringType)GetProcAddress(m_hInstYYDLL,"YAuth_fromHexToStr");

		m_pYAuth_GetAttribute = (PtrGetAttributeType)GetProcAddress(m_hInstYYDLL,"YAuth_getAttribute");

		if ( NULL == m_pYAuth_init || NULL == m_pYAuth_cleanUp || NULL == m_YAuth_genRequest || 
			NULL == m_pYAuth_verifyToken || NULL == m_pYAuth_HexToString || NULL == m_pYAuth_HexToString)
		{
			::MessageBox(0,_T("��ȡYY��̬�⵼������ʧ��!"),_T("����"),1);
			return FALSE;
		}

		if (  NULL != m_pYAuth_init )
		{
			DWORD dwEorror = m_pYAuth_init();
			if (  1 != dwEorror)
			{
				::MessageBox(0,_T("YY��̬���ʼ��ʧ��!"),_T("����"),1); 
				return FALSE;
			}
		}
	}

	if ( m_bWebLoginModeOn )
	{
		InitWebNoteInfo();
	}

	return TRUE;
}

//-------------------------------------------------------------------------
VOID PlayerMgr::InitWebNoteInfo()
{
	// ��ȡ�����ļ�
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	m_dwChannelNum	 = pVar->GetDword(_T("Num"), _T("web_loginmode"));
	for (int i = 0; i < (INT)m_dwChannelNum; i++ )
	{
		TCHAR szURL[30] =  {0};
		_stprintf(szURL,_T("URL%d"),i+1);
		tstring tstrURL = pVar->GetString(szURL, _T("web_loginmode"));
		string  strURL = IUTIL->UnicodeToAnsi(tstrURL.c_str());

		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("RechargeURL%d"),i+1);
		tstring tstrRechargeURL = pVar->GetString(szURL, _T("web_loginmode"));
		string  strRechargeURL = IUTIL->UnicodeToAnsi(tstrRechargeURL.c_str());

		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("DescName%d"),i+1);
		tstring tstrDescName = pVar->GetString(szURL, _T("web_loginmode"));

		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("LoginURL%d"),i+1);
		tstring tstrLoginURL = pVar->GetString(szURL, _T("web_loginmode"));
		string  strLoginURL = IUTIL->UnicodeToAnsi(tstrLoginURL.c_str());


		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("CropID%d"),i+1);
		DWORD dwCropID = pVar->GetDword(szURL, _T("web_loginmode"));
		tagWebNoteInfo* p = new tagWebNoteInfo;
		if( !P_VALID(p) )
		{     
			::MessageBox(0,_T("tagWebNoteInfo �ڴ����ʧ��!"),_T("����"),1); 
			return;
		}
		if ( tstrDescName.length() >= CROP_NAME_MAX_LEN)
		{
			memset(szURL,0,sizeof(szURL));
			_stprintf(szURL,_T("descname : %s ������%d ���ַ�������������!"),tstrDescName.c_str(),(CROP_NAME_MAX_LEN-1));
			::MessageBox(0,szURL,_T("����"),1); 
			SAFE_DEL(p);
			return;    
		}
		p->dwCropID			= dwCropID;
		p->strURL			= strURL;
		p->strRechargeURL	= strRechargeURL;
		p->strLoginURL		= strLoginURL;
		p->tstrDescName		= tstrDescName;
		if ( m_MapURL.IsExist(p->dwCropID))
		{
			memset(szURL,0,sizeof(szURL));
			_stprintf(szURL,_T("dwCropID : %u �ظ�����!"),p->dwCropID);
			::MessageBox(0,szURL,_T("����"),1); 
			SAFE_DEL(p);
			continue;
		}
		m_MapURL.Add(p->dwCropID,p);		
	}

}
//-------------------------------------------------------------------------
VOID PlayerMgr::DestroyWebNoteInfo()
{
	tagWebNoteInfo* pNote = NULL;
	m_MapURL.ResetIterator();
	while( m_MapURL.PeekNext( pNote ) )
	{
		if( P_VALID(pNote) )
		{
			SAFE_DEL(pNote);
		}

	}
	m_MapURL.Clear();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------
BOOL PlayerMgr::InitConfig()
{
	TObjRef<VarContainer> pVar = "GMAccess";
	if( !P_VALID(pVar) ) return FALSE;

	std::set <tstring> setAccountName;
	
	m_bUseGMAccess = pVar->GetInt(_T("on switch"));//�Ƿ�ʹ��gm��ַ����

	//��ȡaccount��
	INT iAccountNum = pVar->GetInt(_T("num account"));

	TCHAR szTemp[X_SHORT_NAME] = {_T('\0')};
	for(int iLoop = 1; iLoop<=iAccountNum; ++iLoop)
	{
		_stprintf(szTemp,_T("name%d"),iLoop);
		setAccountName.insert(pVar->GetString(szTemp,_T("account")));
	}
	CHAR cIP[X_IP_LEN]= {'\0'};
	std::set <tstring>::iterator itLoop = setAccountName.begin();
	for(;itLoop != setAccountName.end();++itLoop)
	{
		INT iIPNum = pVar->GetInt(_T("num"), (*itLoop).c_str());
		for(int iLoop = 1; iLoop<=iIPNum; ++iLoop)
		{
			_stprintf(szTemp,_T("ip%d"),iLoop);

			//ת��ansi
			tstring strIP = pVar->GetString(szTemp,(*itLoop).c_str());
			WideCharToMultiByte(CP_OEMCP,NULL,strIP.c_str(),-1,cIP,X_IP_LEN,NULL,FALSE);

			//ת��DWORD
			DWORD dwIP = inet_addr(cIP);
			m_mapGMvsIP.insert( make_pair((*itLoop).c_str(),dwIP) );

		}		
	}
	// ̨�� ��ȡ�����ļ�
	TObjRef<VarContainer> pVarTemp = "LoginServerConfig";
	if( !P_VALID(pVarTemp) ) return FALSE;

	m_bGMLogin = (pVarTemp->GetInt(_T("on"), _T("gm_login")) == 0) ? false : true;

	if ( m_bGMLogin )
	{
		INT dwChannelNum	 = pVarTemp->GetDword(_T("num"), _T("gm_login"));
		for (INT i = 0; i < dwChannelNum; i++ )
		{
			TCHAR szTemp[30] =  {_T('\0')};
			_stprintf(szTemp,_T("ip%d"),i+1);
			tstring tstrTemp = pVarTemp->GetString(szTemp, _T("gm_login"));
			string  strURL = IUTIL->UnicodeToAnsi(tstrTemp.c_str());

			DWORD dwIP = inet_addr(strURL.c_str());
			m_setGMIP.insert(dwIP);
		}		
	}

	return TRUE;
}
//-------------------------------------------------------------------------------
// ���ٺ���
//-------------------------------------------------------------------------------
VOID PlayerMgr::Destroy()
{
	// ȡ��ע��������Ϣ
	Player::UnRegisterPlayerMsg();

	// ע������
	m_pNetSession->Destroy();

    // ������ڵ�¼�����
    Player* pPlayer = NULL;
    m_mapLoginingPlayer.ResetIterator();
    while( m_mapLoginingPlayer.PeekNext(pPlayer) )
    {
        SAFE_DEL(pPlayer);
    }

	// ��֤��������
	m_pPolicy->Destroy();

	// ɾ����֤����
	sProofPolicyMgr.DestroyProofPolicy(m_ePolicy, m_pPolicy);

	// ����˺�������
	CleanCachedAccountDatas();

	//YY�Ľӿ�����
	if ( 0 != m_bYYMixSerOn )
	{
       if ( NULL != m_pYAuth_cleanUp )
       {
		   m_pYAuth_cleanUp();
       }
       
	   FreeLibrary(m_hInstYYDLL);
	}

	if ( m_bWebLoginModeOn )
	{
		DestroyWebNoteInfo();
	}

}

//-------------------------------------------------------------------------------
// ���º���
//-------------------------------------------------------------------------------
VOID PlayerMgr::Update()
{
	// ���������Ϣ
    UpdateSession();

	// ������֤���
	UpdateProofResult();

	// �õ�ͳ����Ϣ
	InterlockedExchange((LPLONG)&m_nPlayerNum,		m_mapAllPlayer.Size());
	InterlockedExchange((LPLONG)&m_nPlayerLoging,	m_mapLoginingPlayer.Size());
	InterlockedExchange((LPLONG)&m_nProofResultNum,	m_listProofResult.Size());
}

//-------------------------------------------------------------------------------
// ���������Ϣ
//-------------------------------------------------------------------------------
VOID PlayerMgr::UpdateSession()
{
	Player* pPlayer = NULL;
	m_mapLoginingPlayer.ResetIterator();
	while( m_mapLoginingPlayer.PeekNext(pPlayer) )
	{
		INT nRet = pPlayer->HandleMessage();

		if( CON_LOST == nRet )	// ����
		{
			m_mapLoginingPlayer.Erase(pPlayer->GetClientID());
			PlayerLogout(pPlayer);
		}
	}
}

//-------------------------------------------------------------------------------
// ������֤����
//-------------------------------------------------------------------------------
VOID PlayerMgr::UpdateProofResult()
{
	// ÿ��Tick��ദ��20����
	INT nNum = 20;

	// ȡ����֤���
	tagProofResultFull* pResult = m_listProofResult.PopFront();

	while( P_VALID(pResult) )
	{
		// �ҵ����
		Player* pPlayer = m_mapLoginingPlayer.Peek(pResult->dwClientID);
		if( !P_VALID(pPlayer) )
		{
#ifdef  KOREA_LOG
			SPE_LOG->Write(_T("pPlayer not find.client id is:%d.\r\n"),pResult->dwClientID);
#endif
			ILOG->Write(_T("pPlayer not find.client id is:%d.\r\n"),pResult->dwClientID);
			SAFE_DEL(pResult);
			pResult = m_listProofResult.PopFront();
			continue;
		}

		if(m_ePolicy == EPP_QuanQuan)
		{
			pPlayer->SetReservedData(pResult->byReserved[0]);
		}

		if( E_Success != pResult->nRet )
		{
			// ��������ʺ��Ѿ���½
			if( E_ProofResult_Account_In_Use == pResult->nRet )
			{
				// ������ݿ�������״̬������world����������
				if( EPLS_Online == pResult->eLoginStatus )
				{
					LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pResult->dwWorldNameCrc);
					if( P_VALID(pWorld) )
					{
						tagNLW_KickPlayer send;
						send.dwAccountID = pResult->dwAccountID;
						pWorld->SendKickPlayerMsg(send);
					}
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// ע������ط�
					}
				}
				else if(EPLS_Unknown == pResult->eLoginStatus)	//�����⵽ʱδ֪Ҳ�ߵ��ɣ��е�ʱ����һᱻ��������Ϸ~~
				{
					LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pResult->dwWorldNameCrc);
					if( P_VALID(pWorld) )
					{
						tagNLW_KickPlayer send;
						send.dwAccountID = pResult->dwAccountID;
						pWorld->SendKickPlayerMsg(send);
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
					}
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// ע������ط�
					}
				}
				// �����ʱ���ʺŴ��ڵ�½��
				else if( EPLS_Loging == pResult->eLoginStatus )
				{
					// �ڱ��ز����Ƿ���˺���login���棬����У�������
					if( IsAccountExist(pResult->dwAccountID) )
					{

					}
					// ���û�У����������Ϊ��Щ������²�ͬ������ô��ֱ��������ݿ⣬�����óɹ�
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// ע������ط�
					}
				}
			}
			//�����GM���߷�ͣ ��world_forbid��������һ��
			else if( pResult->nRet == E_ProofResult_Forbid_GMTool )
			{
				BOOL bForbid = sBeton.IfWorldForbid(pResult->dwAccountID,pPlayer->GetLoginWorldNameCrc());

				// ���ʾ���˺ſ����Ǳ����Ϸ���类��ͣ��
				if( bForbid == FALSE )
				{
					pResult->nRet = E_Success;	// ע������ط�
				}
			}
			// �������
			else
			{

			}
		}
		// ��֤�ɹ�
		else
		{
			// �����Ȩ��Ϊgm��m_mapGMvsIP������
			if( m_bUseGMAccess && (pResult->byPrivilege != 0) )
			{
				BOOL bFind = FALSE;
				typedef std::multimap<tstring, DWORD>::iterator itFinder;

				itFinder beg = m_mapGMvsIP.lower_bound(pPlayer->GetAccoutNameT()),
					end = m_mapGMvsIP.upper_bound(pPlayer->GetAccoutNameT());
				while( beg != end )
				{
					if(beg->first == pPlayer->GetAccoutNameT() 
						&& beg->second == pPlayer->GetPlayerData().dwIP )
					{
						bFind = TRUE;
						break;
					}
					++beg;
				}
				if( bFind == FALSE )
				{
					// Ŀǰ��Ӧ�����
					pResult->nRet = E_ProofResult_Account_No_Match;	// ע������ط�

					TObjRef<StreamTransport> pTrans;
					pTrans = "StreamTransport";
					char szIP[X_IP_LEN];
					strcpy(szIP,pTrans->IP2String(pPlayer->GetPlayerData().dwIP));

					TCHAR tszIP[X_IP_LEN];
#ifdef UNICODE
					MultiByteToWideChar(CP_ACP,NULL,szIP,-1,tszIP,X_IP_LEN);
#else
					strcpy(tszIP,szIP);
#endif
					ILOG->Write(_T("GM:%s�Ƿ���¼������!!	IP:%s\r\n"),pPlayer->GetAccoutNameT(),tszIP);
#ifdef  KOREA_LOG
					SPE_LOG->Write(_T("GM:%s�Ƿ���¼������!!	IP:%s\r\n"),pPlayer->GetAccoutNameT(),tszIP);
#endif
				}	
			}
		}

		// �����Ӧ�ͻ��˵�ip������
		if( TRUE == sBeton.IfIPForbid(pPlayer->GetPlayerData().dwIP) || TRUE == sBeton.IfMacForbid(pPlayer->GetPlayerData().szMac) )
		{
			//Ŀǰ��Ӧ�����
			pResult->nRet = E_ProofResult_Account_No_Match;	// ע������ط�
#ifdef  KOREA_LOG
			SPE_LOG->Write(_T("��Ӧ�ͻ��˵�ip������"));
#endif
		}

		// �����֤�ɹ�����ʱ����֤һ����Ϸ����
		if( E_Success == pResult->nRet )
		{
			// �ҵ������Ҫ����������Ϸ����
			LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pPlayer->GetLoginWorldNameCrc());
			if( !P_VALID(pWorld) || EWS_Well != pWorld->GetStatus() )
			{
				pResult->nRet = E_SelectWorld_Server_Maintenance;
			}
		}

		if( sLoongWorldMgr.IsInPKPunishmentState(pResult->dwAccountID) )
			pResult->nRet = E_PrrofResult_PKRunawayPunishmentState;

		// �����֤���
		if( E_Success == pResult->nRet )
		{
			//ILOG->Write(_T("Proof Result:%d!\r\n"),pResult->nRet);
			// ���������֤���
			pPlayer->ProofReturn(pResult);

			//��������ϴε�¼ip��ʱ��
			pPlayer->SetPreLoginIP(pResult->dwPreLoginIP);
			pPlayer->SetPreLoginTime(pResult->dwPreLoginTime);

			if ( GetGMLoginOn() )
			{
				std::set<DWORD>::iterator it = m_setGMIP.find(pPlayer->GetCurrIP());
				bool bIsGM = false;
				if ( it != m_setGMIP.end() )
				{
					bIsGM = true;
				}
				pPlayer->SetGM(bIsGM);
			}

			if( EPP_Korea != GetProofPolicy() )
			{	
				// �������һ���Ҫ�ܱ�
				if( pPlayer->IsNeedMibao() )
				{
					tagNLS_MiBao send;
					pPlayer->GenerateMibao(send.szMiBao);	// �����ܱ�
					pPlayer->SendMessage(&send, send.dwSize);
				}
				// ����Ҫ�ܱ�������뵽��Ϸ�����б��еȴ�
				else
				{
					RemovePlayerFromLogining(pPlayer->GetClientID());

					// �����ʺ�
					AddAccount(pPlayer->GetAccountID(), pPlayer->GetClientID());

					// ����Ҽ��뵽LoongWorld������б���
					sLoongWorldMgr.AddToWorld(pPlayer, pPlayer->GetLoginWorldNameCrc());

					//ILOG->Write(_T("Proof Result AddToWorld"));
				}
			}
			else
			{
					tagNLS_SecondPsdProof send;
					send.bIsFirstLogin	= pResult->bIsFirstLogin;
					pPlayer->SendMessage(&send, send.dwSize);
			}
		}
		// �������û����֤�ɹ�
		else
		{
			ILOG->Write(_T("Proof Result:%d!\r\n"),pResult->nRet);

			tagNLS_ProofResult send;
			// Jason 2010-4-26 v2.1.0
			//if( E_PlayerLogin_PKRunawayPunishmentState == pResult->nRet )
			//	send.dwErrorCode = E_PrrofResult_PKRunawayPunishmentState;
			//else
				send.dwErrorCode = pResult->nRet;
#ifdef  KOREA_LOG
				SPE_LOG->Write(_T("-----------------------------------PlayerMgr ProofResult failed-----------------------------------\r\n"));
				SPE_LOG->Write(_T("ErrorCode: %u\r\n"), send.dwErrorCode);   
#endif

			pPlayer->SendMessage(&send, send.dwSize);

			//ILOG->Write(_T("ProofResult: AcountID:%d,ErrorCode:%d!\r\n"),pPlayer->GetAccountID(),pResult->nRet);
		}

		SAFE_DEL(pResult);

		// ��һ���ˣ��������0���˳�
		if( --nNum <= 0 ) break;

		pResult = m_listProofResult.PopFront();
	}
}

//-------------------------------------------------------------------------------
// ��¼�ص�����
//-------------------------------------------------------------------------------
DWORD PlayerMgr::LoginCallBack(LPVOID p1, LPVOID p2)
{
    //�鿴��һ����Ϣ�Ƿ���ȷ
    static DWORD dwMsgCrc = m_pUtil->Crc32("NLC_SafeProof");

	tagUnit* pUnit = (tagUnit*)p1;
	tagXLoginParam* pParam = (tagXLoginParam*)p2;

	tagNLC_SafeProof* pCmd = (tagNLC_SafeProof*)pUnit->pBuffer;

#ifdef  KOREA_LOG
	SPE_LOG->Write(_T("------------------------------------Login Call Back-----------------------------------\r\n"));
#endif

	// �����Ϣ��Crc�Ƿ���ͬ
	if( pCmd->dwID != dwMsgCrc )
	{
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------dwID != dwMsgCrc  -----------------------------------\r\n"));
#endif
		return GT_INVALID;
	}

	//������֤key
	DWORD dwProofKey = 0;
	ProofKey(dwProofKey,pCmd);
	if(dwProofKey != pCmd->dwProofKey)
	{
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------������֤key  -----------------------------------\r\n"));
#endif
		return GT_INVALID; //��֤key����
	}

	// ��סUpdate
	g_login.LockUpdate();

	// ����һ���µĿͻ���ID
	DWORD dwClientID = GenerateClientID();




    
	// �������
	
	Player* pPlayer ;
	LoongWorld* pWorld=sLoongWorldMgr.GetLoongWorld(pCmd->dwWorldNameCrc);
	if(!P_VALID(pWorld))
	{
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------!P_VALID(pWorld)  -----------------------------------\r\n"));
#endif
		g_login.UnlockUpdate();
		return GT_INVALID;
	}
    DWORD dwWorldID=pWorld->GetParentServerCrc();
	pPlayer	= new Player(dwClientID, pParam->dwHandle, pParam->dwAddress, dwWorldID, pCmd->szMac); 


	//Player* pPlayer	= new Player(dwClientID, pParam->dwHandle, pParam->dwAddress, pCmd->dwWorldNameCrc);
	
	if( !P_VALID(pPlayer) )
	{
		g_login.UnlockUpdate();
		return GT_INVALID;
	}

	// ����Ҽ��뵽�б���
	AddPlayerToAll(pPlayer);
	// ����Ҽ��뵽��½����б���
	AddPlayerToLogining(pPlayer);

	//ILOG->Write(_T("AddPlayerToLogining.client id is:%d\r\n"),dwClientID);

	// �ͷ�Update
	g_login.UnlockUpdate();

    return dwClientID;
}

//-------------------------------------------------------------------------------
// �ǳ��ص�����
//-------------------------------------------------------------------------------
DWORD PlayerMgr::LogoutCallBack(LPVOID p)
{
	// ��סUpdate
	g_login.LockUpdate();

	// �õ�ID
    DWORD dwClientID = (DWORD)p;

    // ����������в������
    Player* pPlayer = m_mapAllPlayer.Peek(dwClientID);
    if( P_VALID(pPlayer) )
    {
        // ���ô���ҶϿ����ӱ�־
		pPlayer->SetConnLost();
    }

	// �ͷ�Update
	g_login.UnlockUpdate();


	if ( EPP_Korea == m_ePolicy)
	{
		ProofPolicyKorea* pKorea = (ProofPolicyKorea*)m_pPolicy;
		pKorea->AddEvent(dwClientID,EVT_PlayerLoginOut,0,NULL);
#ifdef  KOREA_LOG		
		SPE_LOG->Write(_T("-----------------------------------delete proof data -----------------------------------\r\n"));		
#endif
	}

	

    return 0;
}

//---------------------------------------------------------------------------------
// ��ҵǳ�
//---------------------------------------------------------------------------------
VOID PlayerMgr::PlayerLogout(Player* pPlayer)
{
	BOOL bLogoutFromDB = TRUE;		// �Ƿ��������ݿ��и����Ϊ����

	if( !pPlayer->IsProofEnd() || GT_INVALID == pPlayer->GetAccountID() )	// ��û����֤�ɹ������ݿ������û�в�����
	{
		bLogoutFromDB = FALSE;
	}
	else if( pPlayer->IsSelectWorldOk() )	// world�Ѿ����˻�Ӧ��˵�����Ͼͻ���뵽world��
	{
		bLogoutFromDB = FALSE;
	}

	// �����Ҫ�������ݿ�
	if( bLogoutFromDB )
	{
		sBeton.PlayerLogout(pPlayer->GetAccountID());
	}

	// ����ʺźϷ������Ƴ��ʺ�
	if( GT_INVALID != pPlayer->GetAccountID() )
	{
		RemoveAccount(pPlayer->GetAccountID());
	}

	// ���б���ɾ�������
	RemovePlayerFromAll(pPlayer->GetClientID());

	// ɾ�����
	SAFE_DEL(pPlayer);
}

//------------------------------------------------------------------------------------
// ������֤
//------------------------------------------------------------------------------------
VOID PlayerMgr::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID, const CHAR *pszMac, LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if ( EPP_Korea == GetProofPolicy() )
	{
		if ( !P_VALID(szGUID) )
		{
			return ;
		}
	}
	else
	{

		if( !P_VALID(szAccoutName) || !P_VALID(szPsd) || !P_VALID(szGUID) )
			return;
	}
	

	// ���͸���Ӧ����֤���Խ�����֤
	m_pPolicy->Proof(dwClientID, szAccoutName, szPsd, szGUID, dwIP,dwWorldNameCrc,dwChannelID,pszMac,szUserID,szAppKey,szToken);
}

//--------------------------------------------------------------------------------------
// ��֤�ص�����
//--------------------------------------------------------------------------------------
VOID PlayerMgr::ProofCallBack(INT nRet, tagProofResult* pResult)
{
	tagProofResultFull* pResultFull = new tagProofResultFull;
	memcpy(pResultFull, pResult, sizeof(tagProofResult));
	pResultFull->nRet = nRet;

	m_listProofResult.PushBack(pResultFull);
}

VOID PlayerMgr::CacheAccountName( DWORD dwAccountID, const CHAR* szAccountName)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	memcpy(pAccountData->szAccountName, szAccountName, X_SHORT_NAME);
}

VOID PlayerMgr::CacheIpAddres( DWORD dwAccountID, DWORD dwIp)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	pAccountData->dwIp = dwIp;
}

VOID PlayerMgr::CacheGuard( DWORD dwAccountID, BOOL bGuard)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	pAccountData->bGuard = bGuard;
}

VOID PlayerMgr::EraseCachedAccountData( DWORD dwAccountID )
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);
	if (P_VALID(pAccountData))
	{
		SAFE_DEL(pAccountData);
		m_mapAccountData.Erase(dwAccountID);
	}
}

VOID PlayerMgr::CleanCachedAccountDatas()
{
	std::list<DWORD> listIDs;
	m_mapAccountData.ExportAllKey(listIDs);
	std::list<DWORD>::iterator itr = listIDs.begin();
	while(itr != listIDs.end())
	{
		EraseCachedAccountData(*itr);
		++itr;
	}
}

VOID PlayerMgr::MapAccountName2AccountID( LPCSTR szAccountName, DWORD dwAccountID )
{
	DWORD dwNameCrc = m_pUtil->Crc32(szAccountName);
	if (!GT_VALID(m_mapAccountNameCrc2AccountID.Peek(dwNameCrc)))
	{
		m_mapAccountNameCrc2AccountID.Add(dwNameCrc, dwAccountID);
	}
}

DWORD PlayerMgr::GetAccountIDByAccountName( LPCSTR szAccountName )
{
	DWORD dwNameCrc = m_pUtil->Crc32(szAccountName);
	return m_mapAccountNameCrc2AccountID.Peek(dwNameCrc);
}

VOID PlayerMgr::CacheMac( DWORD dwAccountID, LPCSTR szMac)
{
    tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
    if (!P_VALID(pAccountData))
    {
        pAccountData = new tagAccountData;
        m_mapAccountData.Add(dwAccountID, pAccountData);
    }
    memcpy(pAccountData->szMac, szMac, MAX_MAC_LENGTH);
} 

LPCSTR    PlayerMgr::RequestTokenID(DWORD dwConnID,string& strTokenID)
{
	if ( NULL != m_YAuth_genRequest )
	{

		strTokenID = m_YAuth_genRequest(dwConnID);
		return strTokenID.c_str();
	}
	else
	{
		ILOG->Write(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);  
		IMSG(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);
		return NULL;
	}
}

static char* ConvertUtf8ToGBK(LPCSTR strUtf8); 
static char* ConvertUtf8ToGBK(LPCSTR strUtf8) 
{
	int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL,0);
	unsigned short * wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
	delete [] wszGBK; 
	return szGBK;
} 

bool   PlayerMgr::VerifyToken(LPVOID pLoginCmdInfo,DWORD dwCDIndex)
{
	DWORD dwConnID = (DWORD)dwCDIndex;

	if ( NULL != m_pYAuth_verifyToken)
	{
		tagNLC_SafeProof* pLoginCMD =  (tagNLC_SafeProof*)pLoginCmdInfo;
		if ( pLoginCMD->dwReserved <= 0 || pLoginCMD->dwReserved >= 1024)
		{
#ifdef  YYLogin_LOG	
			ILOG->Write(_T("����waring:�ַ�������������Χ dwReserved: %d\r\n"),(INT)pLoginCMD->dwReserved);
#endif
			return false;
		}

		PBYTE pByte = new BYTE[pLoginCMD->dwReserved+3];
		if( !P_VALID(pByte))
		{
			return false;
		}
		memset(pByte,0,pLoginCMD->dwReserved+3);
		CHAR* pLoginInfo = (CHAR*)pByte;
		memcpy(pLoginInfo,pLoginCMD->byData,pLoginCMD->dwReserved);
		*((char*)pByte+pLoginCMD->dwReserved+2) = '\0';
		int nRet = m_pYAuth_verifyToken(pLoginInfo,dwConnID) ;

		bool bSuc = false;
		if ( 1 == nRet )
		{
			bSuc = true;
		}

		string strGUID;
		if ( bSuc )
		{
			//ILOG->Write(_T("PlayerMgr::VerifyToken Begin /r/n"));
			char szBuff[1024] = {0}; 
			char szBuff_2[1024] = {0}; 
			//��ϵ��ʽ��������֤���ţ���Ҫʮ������ת����UID����Ҫ��
			// ȡ��YY��uid
			m_pYAuth_GetAttribute( pLoginInfo, "UserID", szBuff, sizeof(szBuff));
			szBuff[1023] = '\0';
			strGUID += szBuff;
			strGUID += ',';

			//ILOG->Write(_T("PlayerMgr::VerifyToken1 %s /r/n"), strGUID.c_str());
			memset(szBuff,0,sizeof(szBuff));
			memset(szBuff_2,0,sizeof(szBuff_2));

			// ȡ������
			m_pYAuth_GetAttribute(pLoginInfo, "IDCardName", szBuff, sizeof(szBuff) );
			szBuff[1023]	= '\0';
			m_pYAuth_HexToString( szBuff, szBuff_2, sizeof(szBuff_2) ); 
			szBuff_2[1023]	= '\0';
			char * p = ConvertUtf8ToGBK(szBuff_2); 
			strGUID += p;
			strGUID +=  ',';
			delete [] p; 

			//ILOG->Write(_T("PlayerMgr::VerifyToken2 %s /r/n"), strGUID.c_str());
			memset(szBuff,0,sizeof(szBuff));
			memset(szBuff_2,0,sizeof(szBuff_2));

			// ȡ֤��
			m_pYAuth_GetAttribute( pLoginInfo, "IDCardNumber", szBuff, sizeof(szBuff) );
			szBuff[1023]	= '\0';
			m_pYAuth_HexToString( szBuff, szBuff_2, sizeof(szBuff_2) );
			szBuff_2[1023]  = '\0';
			strGUID += szBuff_2;
			strGUID +=  ',';

			//ILOG->Write(_T("PlayerMgr::VerifyToken3 %s /r/n"), strGUID.c_str());
			memset(szBuff,0,sizeof(szBuff));
			memset(szBuff_2,0,sizeof(szBuff_2));

			// ��ϵ��ʽ
			m_pYAuth_GetAttribute( pLoginInfo, "ContactInfo", szBuff, sizeof(szBuff) );
			szBuff[1023]    = '\0';
			m_pYAuth_HexToString( szBuff, szBuff_2, sizeof(szBuff_2) );
			szBuff_2[1023]  = '\0';
			strGUID +=  szBuff_2;
			strGUID +=  ',';
			

			//ILOG->Write(_T("PlayerMgr::VerifyToken4 %s /r/n"), strGUID.c_str());
			if ( strGUID.length() > X_USERNAME_LENEX)
			{
				ILOG->Write(_T("-----------------------YY �˺���Ϣ����50���ַ�-------------------\r\n"),dwConnID);  
				IMSG(_T("-----------------------YY �˺���Ϣ����50���ַ�-------------------\r\n"),dwConnID);
				SAFE_DEL_ARRAY(pByte);
				return false;
			}
		
			memset(pLoginCMD->szGUID,0,sizeof(pLoginCMD->szGUID));
			memcpy(pLoginCMD->szGUID,strGUID.c_str(),strGUID.length());

			//ILOG->Write(_T("PlayerMgr::VerifyToken End /r/n"));
		}

#ifdef  YYLogin_LOG	
		TCHAR accoutName[1024];
		memset(accoutName,0,sizeof(accoutName));
		MultiByteToWideChar(CP_ACP,NULL,strGUID.c_str(),-1,accoutName,strGUID.length()*2);
		accoutName[strGUID.length()*2+1] = _T('\0');

		ILOG->Write(_T("-----------------------------------Get User information -----------------------------------\r\n"));
		ILOG->Write(_T("szGUID: %s,bSuc: %d,ClientID: %d \r\n"),accoutName,(INT)bSuc,dwCDIndex);
#endif
		SAFE_DEL_ARRAY(pByte);

		//ILOG->Write(_T("PlayerMgr::VerifyToken End1 /r/n"));
		return bSuc;    
	}
	else
	{
		ILOG->Write(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);  
		IMSG(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);
		return false;    
	}

}

bool	PlayerMgr::GetWebLoginInfo(LPVOID pWebLoginInfo,DWORD& dwMsgSize)
{
	//ILOG->Write(_T("PlayerMgr::GetWebLoginInfo Start"));
	tagWebNoteInfo* pWebNoteInfo = NULL;
	m_MapURL.ResetIterator();
	PBYTE pByte = (PBYTE)pWebLoginInfo;
	DWORD dwNum = 0;
	dwMsgSize = 0;
	while( m_MapURL.PeekNext(pWebNoteInfo) )
	{
		if ( P_VALID(pWebNoteInfo) )
		{
			if ( dwNum > m_dwChannelNum)
			{
				break;
			}
			DWORD dwSize      = 0 ;
			dwSize			  = sizeof(DWORD)*2 + CROP_NAME_MAX_LEN*sizeof(TCHAR) + pWebNoteInfo->strURL.length()*sizeof(CHAR) +1;
			(*(DWORD*)pByte)  = dwSize;
			pByte			  = pByte + sizeof(DWORD);
			tagWeb_URLInfo* p = (tagWeb_URLInfo*)pByte;
			p->dwCropID       =  pWebNoteInfo->dwCropID;
			DWORD dwLen = pWebNoteInfo->tstrDescName.length();
			memcpy(p->szDescName,pWebNoteInfo->tstrDescName.c_str(),dwLen*sizeof(TCHAR));
			p->szDescName[dwLen] = _T('\0');
			p->dwSize		  = pWebNoteInfo->strURL.length()*sizeof(CHAR) +1;
			memcpy(p->szURL,pWebNoteInfo->strURL.c_str(),p->dwSize-1);
			p->szURL[p->dwSize-1] = '\0';
			pByte			  = pByte + dwSize;
			dwMsgSize += (dwSize + sizeof(DWORD)); 
		}
	}
	//ILOG->Write(_T("PlayerMgr::GetWebLoginInfo End"));
	return true;

}

bool	PlayerMgr::GetRechargeURL(OUT char* pLoginURLBuff,DWORD& dwLoginURLSize,OUT char* pBuff,DWORD& dwURLSize,DWORD dwCropID)
{
	//ILOG->Write(_T("PlayerMgr::GetRechargeURL Start"));
	tagWebNoteInfo* pWebNoteInfo	 = NULL;
	if ( !m_MapURL.IsExist( dwCropID ))
	{
		return false;
	}
	pWebNoteInfo = m_MapURL.Peek(dwCropID);
	if (!P_VALID( pWebNoteInfo) )
	{
		return false;
	}
	dwURLSize						 = pWebNoteInfo->strRechargeURL.length() + 1;
	dwLoginURLSize					 = pWebNoteInfo->strLoginURL.length() + 1;
	memcpy(pBuff,pWebNoteInfo->strRechargeURL.c_str(),pWebNoteInfo->strRechargeURL.length());
	memcpy(pLoginURLBuff,pWebNoteInfo->strLoginURL.c_str(),pWebNoteInfo->strLoginURL.length());
	pBuff[dwURLSize-1]				 = '\0';
	pLoginURLBuff[dwLoginURLSize-1] = '\0';
	//ILOG->Write(_T("PlayerMgr::GetRechargeURL End"));
	return true;
}
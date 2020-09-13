/********************************************************************
	created:	2012/05/07
	created:	7:5:2012   17:18
	filename: 	e:\code\World\internet_cafes_session.cpp
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	��Ϸ�������뺫������Time�������Ի�
*********************************************************************/
#include "Stdafx.h"
#include "..\ServerDefine\base_define.h"
#include "..\ServerDefine\login_define.h"
#include "..\ServerDefine\login_issue.h"
#include "internet_cafes_session.h"
#include "world.h"
#include "world_session.h"
#include "player_session.h"
#include "vip_netbar.h"
#include "TimeChecker.h"
#include "vip_netbar.h"
#include "role.h"
#ifdef ON_INTERNET_CAFE

InternetCafeSession g_InternetCafeSession;
//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
InternetCafeSession::InternetCafeSession() : m_Trunk(this)
{
	m_dwInternetCafePort	= GT_INVALID;
	m_bTermConnect	= FALSE;

	m_bOn = FALSE;
	m_bVerify = FALSE;
	m_bUseSSL = FALSE;
	ZeroMemory(m_szInternetCafeIP, sizeof(m_szInternetCafeIP));
	ZeroMemory(m_szCertName, sizeof(m_szCertName));
	ZeroMemory(m_szKeyName, sizeof(m_szKeyName));
	ZeroMemory(m_szCACertName, sizeof(m_szCACertName));
	ZeroMemory(m_szKeyPasswd, sizeof(m_szKeyPasswd));

	//DummyTS�ر��¼�
	m_hDummyTSClosed = CreateEvent(NULL, TRUE, FALSE, NULL);
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
InternetCafeSession::~InternetCafeSession()
{
}
//-----------------------------------------------------------------------------
// ��ʼ������
//-----------------------------------------------------------------------------
BOOL InternetCafeSession::Init()
{
	// �ò��ֳ�Ա���¸�ֵ����Ϊ���������һ��ȫ�ֱ�����
	m_pUtil = "Util";
	m_pThread = "Thread";

	// ������Ϣ������
	
	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToLoongDummyTS", "StreamTransport");
	m_pTran = "ToLoongDummyTS";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create ToLoongDummyTS(StreamTransport) obj failed!\r\n"));
		return FALSE;
	}

	// ��ȡ�ļ�, ��ʼ����Ա
	if( !InitConfig() )
	{
		ERR(_T("Init File Read Failed! Please Check......\r\n"));
		return FALSE;
	}
	m_pTran->Init(TRUE, ESTM_Stream, m_bUseSSL, m_bVerify, m_szCertName, m_szKeyName, m_szCACertName, m_szKeyPasswd);

	// ע��������������
	RegisterAllDummyTSCommand();

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectLoongDummyTS"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&InternetCafeSession::ThreadConnectDummyTS), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectLoongDummyTS")) )
	{
		continue;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ȡ�ļ�, ��ʼ����Ա
//-----------------------------------------------------------------------------
BOOL InternetCafeSession::InitConfig()
{
	TObjRef<VarContainer> pVar = "VarContainer";
	
	m_bOn = pVar->GetDword(_T("on InternetCafes"));
	m_bUseSSL = pVar->GetDword(_T("usessl InternetCafes"));
	m_bVerify = pVar->GetDword(_T("verify InternetCafes"));

	// ��ȡDummyTS�������Ķ˿ں�
	m_dwInternetCafePort	= pVar->GetDword(_T("port InternetCafes"));

	// ��ȡDummyTS������IP��ַ
	TCHAR szBuf[X_LONG_NAME];
	
	memset(szBuf, 0, sizeof(szBuf));
	_tcsncpy(szBuf, pVar->GetString(_T("ip InternetCafes")), MCalTCharArrayNum(szBuf) - 1);
	IFASTCODE->MemCpy(m_szInternetCafeIP, m_pUtil->UnicodeToUnicode8(szBuf), sizeof(m_szInternetCafeIP) - 1);

	memset(szBuf, 0, sizeof(szBuf));
	_tcsncpy(szBuf, pVar->GetString(_T("certname InternetCafes")), MCalTCharArrayNum(szBuf) - 1);
	IFASTCODE->MemCpy(m_szCertName, m_pUtil->UnicodeToUnicode8(szBuf), sizeof(m_szCertName) - 1);

	memset(szBuf, 0, sizeof(szBuf));
	_tcsncpy(szBuf, pVar->GetString(_T("keyname InternetCafes")), MCalTCharArrayNum(szBuf) - 1);
	IFASTCODE->MemCpy(m_szKeyName, m_pUtil->UnicodeToUnicode8(szBuf), sizeof(m_szKeyName) - 1);

	memset(szBuf, 0, sizeof(szBuf));
	_tcsncpy(szBuf, pVar->GetString(_T("cacertname InternetCafes")), MCalTCharArrayNum(szBuf) - 1);
	IFASTCODE->MemCpy(m_szCACertName, m_pUtil->UnicodeToUnicode8(szBuf), sizeof(m_szCACertName) - 1);

	memset(szBuf, 0, sizeof(szBuf));
	_tcsncpy(szBuf, pVar->GetString(_T("keypassword InternetCafes")), MCalTCharArrayNum(szBuf) - 1);
	IFASTCODE->MemCpy(m_szKeyPasswd, m_pUtil->UnicodeToUnicode8(szBuf), sizeof(m_szKeyPasswd) - 1);

	struct hostent* hptr;
	if ((hptr = gethostbyname(m_szInternetCafeIP)) == NULL)
	{
		return FALSE;
	}
	switch (hptr->h_addrtype)
	{
	case AF_INET:
		{
			if (*hptr->h_addr_list != NULL)
			{
				char* pCh = inet_ntoa(*(struct in_addr*)*hptr->h_addr_list);
				memset(m_szInternetCafeIP, 0, sizeof(m_szInternetCafeIP));
				memcpy(m_szInternetCafeIP, pCh, strlen(pCh));
			}
			else
			{
				return FALSE;
			}
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID InternetCafeSession::Destroy()
{
	//��DummyTS������Ϸ����رյ���Ϣ
	//�Ա㽫������ҵĵ�½״̬��λ
	tagNWL_WorldColsed send;
	m_pTran->Send(&send,send.dwSize);
	//�ȴ�DummyTS������Ϣ
	while(WAIT_TIMEOUT == WaitForSingleObject(m_hDummyTSClosed,100))
	{
		//����Ѿ�ʧȥ����
		if( !m_pTran->IsConnected())
			break;

		Update();
	}

	// �ȴ������߳̽���
	InterlockedExchange((LPLONG)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectLoongDummyTS"), INFINITE);

	m_pTran->Destroy();
	KillObj("ToLoongDummyTS");

	// ע����Ϣ����

	if(m_hDummyTSClosed)
		CloseHandle(m_hDummyTSClosed);
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID InternetCafeSession::Update()
{
	CHECK_TIME()
	UpdateSession();
}

BOOL InternetCafeSession::IsOn()
{
	return m_bOn;
}

//-----------------------------------------------------------------------------
// ����DummyTS��������Ϣ
//-----------------------------------------------------------------------------
VOID InternetCafeSession::UpdateSession()
{
	if( NULL == m_pTran )
		return;

	if( !m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectLoongDummyTS")) )
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectLoongDummyTS"), INFINITE);

		// ����������½�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectLoongDummyTS"), (vEngine::THREADPROC)m_Trunk.sfp0(&InternetCafeSession::ThreadConnectDummyTS), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectLoongDummyTS")))
		{
			continue;
		}

		return;
	}

	// ��������DummyTS����������Ϣ
	while( m_pTran->IsConnected() )
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);

		if( !P_VALID(pRecv) )
		{
			return;
		}

		// ������Ϣ
		ProcessNetworkMsg((tagT2W_Package*)pRecv, dwSize);

		// ������Դ
		m_pTran->FreeRecved(pRecv);
	}
}

//-----------------------------------------------------------------------------
// �����߳�(����DummyTS������)
//-----------------------------------------------------------------------------
DWORD InternetCafeSession::ThreadConnectDummyTS()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif
	INT nConCounter = 0;
	while( FALSE == m_bTermConnect )
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szInternetCafeIP, m_dwInternetCafePort);
			}

			Sleep(100);
			if( ++nConCounter > 60 )
				return 0;
			continue;	// ���¼������
		}
		nConCounter = 0;

		IMSG(_T("Contected to DummyTS Server at %s: %u\r\n"), m_pUtil->Unicode8ToUnicode(m_szInternetCafeIP), m_dwInternetCafePort);
		break;
	}
#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}

//----------------------------------------------------------------------------------------------
// �����ʺŵ�½���
//----------------------------------------------------------------------------------------------
VOID InternetCafeSession::SendPlayerLogin(CHAR* szAccountName, DWORD dwRoleID, DWORD dwIP)
{
	if (FALSE == IsWell())
	{
		return;
	}

	CHAR szIP[X_IP_LEN] = {0};
	strncpy(szIP, inet_ntoa((*(in_addr*)&dwIP)), X_IP_LEN);
	CHAR buf[2048];
	memset(buf, 0, sizeof(buf));
	tagW2T_Package* pICMsg = (tagW2T_Package*)buf;
	pICMsg->byStart = 0x0e;

	CHAR szPrefix[40] = "vaan_";	
	strcat(szPrefix, szAccountName);

	CHAR* pData = (CHAR*)pICMsg->byData;
	sprintf(pData, "%d|%d|%s|%s|%d|%d", EICMT_Start, SERVECE_TYPE, szIP, szPrefix, dwRoleID, GAME_SSN);

	pICMsg->wLength = htons((WORD)strlen(pData));
	Send(pICMsg, sizeof(tagW2T_Package) + strlen(pData) - 1);
}

//----------------------------------------------------------------------------------------------
// �����ʺŵǳ����
//----------------------------------------------------------------------------------------------
VOID InternetCafeSession::SendPlayerLogout(CHAR* szAccountName, DWORD dwRoleID, DWORD dwIP)
{
	if (FALSE == IsWell())
	{
		return;
	}

	CHAR szIP[X_IP_LEN] = {0};
	strncpy(szIP, inet_ntoa((*(in_addr*)&dwIP)), X_IP_LEN);
	CHAR buf[2048];
	memset(buf, 0, sizeof(buf));
	tagW2T_Package* pICMsg = (tagW2T_Package*)buf;
	pICMsg->byStart = 0x0e;

	CHAR szPrefix[40] = "vaan_";	
	strcat(szPrefix, szAccountName);

	CHAR* pData = (CHAR*)pICMsg->byData;
	sprintf(pData, "%d|%d|%s|%s|%d|%d", EICMT_Logout, SERVECE_TYPE, szIP, szPrefix, dwRoleID, GAME_SSN);

	pICMsg->wLength = htons((WORD)strlen(pData));
	Send(pICMsg, sizeof(tagW2T_Package) + strlen(pData) - 1);
}

VOID InternetCafeSession::SendPing(CHAR* szAccountName, DWORD dwRoleID, DWORD dwIP)
{
	if (FALSE == IsWell())
	{
		return;
	}

	CHAR szIP[X_IP_LEN] = {0};
	strncpy(szIP, inet_ntoa((*(in_addr*)&dwIP)), X_IP_LEN);
	CHAR buf[2048];
	memset(buf, 0, sizeof(buf));
	tagW2T_Package* pICMsg = (tagW2T_Package*)buf;
	pICMsg->byStart = 0x0e;

	CHAR szPrefix[40] = "vaan_";	
	strcat(szPrefix, szAccountName);

	CHAR* pData = (CHAR*)pICMsg->byData;
	sprintf(pData, "%d|%d|%s|%s|%d|%d", EICMT_Ping, SERVECE_TYPE, szIP, szPrefix, dwRoleID, GAME_SSN);

	pICMsg->wLength = htons((WORD)strlen(pData));
	Send(pICMsg, sizeof(tagW2T_Package) + strlen(pData) - 1);
}

VOID InternetCafeSession::GetLeftTime(DWORD dwIP)
{
	if (FALSE == IsWell())
	{
		return;
	}

	CHAR szIP[X_IP_LEN] = {0};
	strncpy(szIP, inet_ntoa((*(in_addr*)&dwIP)), X_IP_LEN);

	CHAR buf[2048];
	memset(buf, 0, sizeof(buf));
	tagW2T_Package* pICMsg = (tagW2T_Package*)buf;
	pICMsg->byStart = 0x0e;
	CHAR* pData = (CHAR*)pICMsg->byData;
	sprintf(pData, "%d|%d|%s", EICMT_LeftTime, SERVECE_TYPE, szIP);

	pICMsg->wLength = htons((WORD)strlen(pData));
	Send(pICMsg, sizeof(tagW2T_Package) + strlen(pData) - 1);
}

//----------------------------------------------------------------------------------------------
// ע�����е���������
//----------------------------------------------------------------------------------------------
VOID InternetCafeSession::RegisterAllDummyTSCommand()
{
	m_mapProc.insert(make_pair(EICMT_Start,		&InternetCafeSession::HandlePlayerLogin));
	m_mapProc.insert(make_pair(EICMT_Logout,	&InternetCafeSession::HandlePlayerLogout));
	m_mapProc.insert(make_pair(EICMT_Ping,		&InternetCafeSession::HandlePing));
	m_mapProc.insert(make_pair(EICMT_LeftTime,	&InternetCafeSession::HandleLeftTime));
}

VOID InternetCafeSession::ProcessNetworkMsg(tagT2W_Package* pMsg, DWORD dwMsgSize)
{
	if (!P_VALID(pMsg) || 0x0e != pMsg->byStart)
	{
		return;
	}

	CHAR* buf = (CHAR*)pMsg->byData;
	pMsg->wLength = ntohs(pMsg->wLength);

	if((pMsg->wLength + sizeof(tagT2W_Package) - 1) != dwMsgSize )
	{
		// ֪ͨ�ͻ��ˣ�����ȷǷ�
		IMSG(_T("Invalid dummyTS command size[<size>%d]\r\n"), 
			pMsg->wLength);
	}

	CHAR* p;
	CHAR* next;
	p = strtok_s(buf, "|", &next);

	if (!P_VALID(p))
	{
		return;
	}
	INT nCmd = atoi(p);
	if (EICMT_Start != nCmd &&
		EICMT_Logout != nCmd &&
		EICMT_Ping != nCmd &&
		EICMT_LeftTime != nCmd)
	{
		return;
	}

	icfunc fn;
	std::map<DWORD, icfunc>::iterator it = m_mapProc.find(nCmd);
	if( it == m_mapProc.end() )
		return;
	else
		fn = it->second;
	if (NULL == fn)
	{
		return;
	}

	(this->*fn)(next, pMsg->wLength);
}

DWORD InternetCafeSession::HandlePlayerLogin(CHAR* pMsg, WORD)
{
	//MSG_TYPE(00)|USER_ID|USN|RETURN_CODE|TIME  : �ɹ�
	//MSG_TYPE(00)|USER_ID|USN|RETURN_CODE : ʧ��

	if (!P_VALID(pMsg))
	{
		return 0;
	}
	DWORD dwRoleID, dwRetCode;
	CHAR* szAccName;
	CHAR* szRoleID;
	CHAR* szRet;
	szAccName = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szAccName))
	{
		return 0;
	}
	szRoleID = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szRoleID))
	{
		return 0;
	}

	szRet = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szRet))
	{
		return 0;
	}
	dwRoleID = atoi(szRoleID);
	dwRetCode = atoi(szRet);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	switch (dwRetCode)
	{
	case EICRC_SUCCESS:
		{
			// ��ʼ���ɽ���
			DWORD dwTime;
			CHAR* szTime;
			szTime = strtok_s(NULL, "|", &pMsg);
			if (!P_VALID(szTime))
			{
				return 0;
			}

			dwTime = atoi(szTime);
			pRole->EnableInternetCafe();
		}
		break;
	case EICRC_FAILED:
	case EICRC_LACK_TIME:
	case EICRC_INVALID_IP:
	case EICRC_PROTOCOL_ERR:
		{
			pRole->EndInternetCafe();
		}
		break;
	case EICRC_INVALID_USER:
		{
			// USER_START �ٴη���
			if (!P_VALID(pRole->GetSession()))
			{
				return 0;
			}
			SendPlayerLogin(szAccName+5, dwRoleID, pRole->GetSession()->GetGetIP());
		}
		break;
	}

	return 0;
}

DWORD InternetCafeSession::HandlePlayerLogout(CHAR* pMsg, WORD)
{
	//MSG_TYPE(01)|USER_ID|USN|RETURN_CODE|TIME : �ɹ�
	//MSG_TYPE(01)|USER_ID|USN|RETURN_CODE : ʧ��
	if (!P_VALID(pMsg))
	{
		return 0;
	}
	DWORD dwRoleID, dwRetCode;
	CHAR* szAccName;
	CHAR* szRoleID;
	CHAR* szRet;
	szAccName = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szAccName))
	{
		return 0;
	}
	szRoleID = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szRoleID))
	{
		return 0;
	}

	szRet = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szRet))
	{
		return 0;
	}
	dwRoleID = atoi(szRoleID);
	dwRetCode = atoi(szRet);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	switch (dwRetCode)
	{
	case EICRC_SUCCESS:
		break;
	case EICRC_FAILED:
		break;
	case EICRC_LACK_TIME:
		break;
	case EICRC_INVALID_IP:
		break;
	case EICRC_PROTOCOL_ERR:
		break;
	case EICRC_INVALID_USER:
		break;
	}

	pRole->CancelInternetCafe();
	pRole->EndInternetCafe();

	return 0;
}

DWORD InternetCafeSession::HandlePing(CHAR* pMsg, WORD)
{
	//MSG_TYPE(02)|USER_ID|USN|RETURN_CODE|TIME : �ɹ�  
	//MSG_TYPE(02)|USER_ID|USN|RETURN_CODE : ʧ��

	if (!P_VALID(pMsg))
	{
		return 0;
	}
	DWORD dwRoleID, dwRetCode;
	CHAR* szAccName;
	CHAR* szRoleID;
	CHAR* szRet;
	szAccName = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szAccName))
	{
		return 0;
	}
	szRoleID = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szRoleID))
	{
		return 0;
	}

	szRet = strtok_s(NULL, "|", &pMsg);
	if (!P_VALID(szRet))
	{
		return 0;
	}
	dwRoleID = atoi(szRoleID);
	dwRetCode = atoi(szRet);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	switch (dwRetCode)
	{
	case EICRC_SUCCESS:
		{
			//DWORD dwTime;
			//CHAR* szTime;
			//szTime = strtok_s(NULL, "|", &pMsg);
			//if (!P_VALID(szTime))
			//{
			//	return 0;
			//}

			//dwTime = atoi(szTime);
			//pRole->EableInternetCafe();
			pRole->SetInternetCafePingLostTime(0);
		}
		break;
	case EICRC_LACK_TIME:
	case EICRC_INVALID_IP:
		{
			pRole->CancelInternetCafe();
			pRole->EndInternetCafe();
		}
		break;
	case EICRC_FAILED:
	case EICRC_PROTOCOL_ERR:
		{
			pRole->SetInternetCafePingLostTime(pRole->GetInternetCafePingLostTime() + 1);
			if (pRole->GetInternetCafePingLostTime() > 3)
			{
				pRole->CancelInternetCafe();
				pRole->EndInternetCafe();
			}
		}
		break;
	case EICRC_INVALID_USER:
		{
			SendPlayerLogin(szAccName+5, dwRoleID, pRole->GetSession()->GetGetIP());
		}
		break;
	default:
		pRole->SetInternetCafePingLostTime(pRole->GetInternetCafePingLostTime() + 1);
		if (pRole->GetInternetCafePingLostTime() > 1)
		{
			pRole->CancelInternetCafe();
			pRole->EndInternetCafe();
		}
		break;
	}

	return 0;
}

DWORD InternetCafeSession::HandleLeftTime(CHAR* pMsg, WORD)
{

	return 0;
}


#endif



//-----------------------------------------------------------------------------
// Copyright (c) 2010 KONGZHONG Entertainment All rights reserved.
// filename: keycode_bridge_session.cpp
// data: 2010-09-06
// last:
// brief: ��Ϸ��������KeyCodeBridgeServer�ĶԻ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "keycode_bridge_session.h"
#include "..\ServerDefine\base_define.h"
#include "world.h"
#include "TimeChecker.h"
#include "role_mgr.h"
#include "role.h"
#include "..\WorldDefine\msg_func_npc.h"

#define X_SINGLE_DIVIDE_SIGN ';' //������Ϣ�ķָ���
#define X_RECEIVE_DIVIDE_SIGN "\r\n" //������Ϣ�ķָ���

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
KeyCodeBridgeSession g_keyCodeBridgeSession;

KeyCodeBridgeSession::KeyCodeBridgeSession() : m_Trunk(this), m_dwWorldID(0)
{
	m_dwPort	= GT_INVALID;
	m_bTermConnect	= FALSE;

	m_bInitOK = FALSE;
	m_dwOn = 0;

	InitializeCriticalSection(&m_ListLock); 
	InitializeCriticalSection(&m_RetListLock); 

	ZeroMemory(m_szIP, sizeof(m_szIP));
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
KeyCodeBridgeSession::~KeyCodeBridgeSession()
{
}

//-----------------------------------------------------------------------------
// ��ʼ������
//-----------------------------------------------------------------------------
BOOL KeyCodeBridgeSession::Init()
{
	// �ò��ֳ�Ա���¸�ֵ����Ϊ���������һ��ȫ�ֱ�����
	m_pUtil = "Util";
	m_pThread = "Thread";

	// �������Ӷ��󣬲���ʼ��
	CreateObj("ToKeyCodeBridge", "StreamTransport");
	m_pTran = "ToKeyCodeBridge";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create KeyCodeBridge(StreamTransport) obj failed!"));
		return FALSE;
	}
	//m_pTran->Init(TRUE,ESTM_Stream);

	if( !m_pTran->Init(TRUE,ESTM_Stream) )
	{
		ERR(_T("KeyCodeBridge Init failed!"));
		return FALSE;
	}

	// ��ȡ�ļ�, ��ʼ����Ա
	if(!InitConfig())
	{
		ERR(_T("Init File Read Failed! Please Check......"));
		return FALSE;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ConnectKeyCodeBridge"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&KeyCodeBridgeSession::ThreadConnectKeyCodeBridge), NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ConnectKeyCodeBridge")))
	{
		continue;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ȡ�ļ�, ��ʼ����Ա
//-----------------------------------------------------------------------------
BOOL KeyCodeBridgeSession::InitConfig()
{
	//CreateObj("LoongDBVar", "VarContainer");
	TObjRef<VarContainer> pVar = "VarContainer";
	if(!P_VALID(pVar))
	{
		ERR(_T("Create KeyCodeBridgeVar(VarContainer) obj failed!"));
		return FALSE;
	}

	// ��ȡ���ݿ�������Ķ˿ں�
	m_dwPort	= pVar->GetDword(_T("port keycode_recharge_bridge_server"));
	m_dwOn = pVar->GetDword(_T("on keycode_recharge_bridge_server"));

	// ��ȡ���ݷ�����IP��ַ
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip keycode_recharge_bridge_server")), MCalTCharArrayNum(szIP) - 1);
	IFASTCODE->MemCpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	// ��ȡServerID
	m_dwWorldID = pVar->GetDword(_T("id world"));

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID KeyCodeBridgeSession::Destroy()
{
	// �ȴ������߳̽���
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectKeyCodeBridge"), INFINITE);

	m_pTran->Destroy();

	KillObj("ToKeyCodeBridge");

	EnterCriticalSection(&m_ListLock);

	// ɾ������list�е�����
	tagKeyCodeInfo* pData = m_listKeyCodeInfo.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listKeyCodeInfo.PopFront();
	}
	LeaveCriticalSection(&m_ListLock);

	EnterCriticalSection(&m_RetListLock);
	pData = m_listRetKeyCodeInfo.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listRetKeyCodeInfo.PopFront();
	}
	LeaveCriticalSection(&m_RetListLock);

	DeleteCriticalSection(&m_ListLock); 
	DeleteCriticalSection(&m_RetListLock); 
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID KeyCodeBridgeSession::Update()
{
	if (m_dwOn == 0)
		return;

	CHECK_TIME();
	if( NULL == m_pTran )
		return;

	if(!m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectKeyCodeBridge")))
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectKeyCodeBridge"), INFINITE);

		// �����������ݿ�����������߳�
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectKeyCodeBridge"), 
			(vEngine::THREADPROC)m_Trunk.sfp0(&KeyCodeBridgeSession::ThreadConnectKeyCodeBridge),
			NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectKeyCodeBridge")))
		{
			continue;
		}

		return;
	}

	// ������֤�б�
	UpdateProofList();

	// ��������KeyCodeBridge����������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTran->Recv(dwSize);
	while( P_VALID(pRecv) && dwSize > 0 && m_pTran->IsConnected())
	{	
  		HandleRecvResult(pRecv);

		//Խ����֤�ķ�����Ϣ
		m_pTran->FreeRecved(pRecv);

		pRecv = m_pTran->Recv(dwSize);
	}
}

//-----------------------------------------------------------------------------
// �����߳�(�������ݿ������)
//-----------------------------------------------------------------------------
DWORD KeyCodeBridgeSession::ThreadConnectKeyCodeBridge()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	if (m_dwOn == 0)
		return 0;

	while(FALSE == m_bTermConnect)
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szIP, m_dwPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to KeyCodeBridge Server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szIP), m_dwPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}

VOID KeyCodeBridgeSession::Proof(LPCSTR szAccount, DWORD dwRoleID, LPCSTR szKeyCode, DWORD dwClientIP)
{
	if (m_dwOn == 0)
		return;

	if(!P_VALID(szAccount) || !P_VALID(dwRoleID) ||  !P_VALID(szKeyCode) || !P_VALID(dwClientIP)) return;

	// ����һ���µ���֤��Ϣ
	tagKeyCodeInfo* pData = new tagKeyCodeInfo(m_dwWorldID, szAccount, dwRoleID, szKeyCode, dwClientIP);

	EnterCriticalSection(&m_ListLock);
	m_listKeyCodeInfo.PushBack(pData);
	LeaveCriticalSection(&m_ListLock);
}

//-------------------------------------------------------------------------
// ������֤�б�
//-------------------------------------------------------------------------
VOID KeyCodeBridgeSession::UpdateProofList()
{
	if(!m_pTran->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	EnterCriticalSection(&m_ListLock);
	// ���б���ȡ����֤����
	tagKeyCodeInfo* pData = m_listKeyCodeInfo.PopFront();
	LeaveCriticalSection(&m_ListLock);
	int iNum = 0;

	//һ�δ���50��
	while( P_VALID(pData) && iNum<50)
	{
		++iNum;

		//////////////////////////////////////////////////////////////////////////
		//�������ݶ���ansi
		//ƴ��֤����
		stringstream strStream;
		strStream<<pData->dwGameID;
		strStream<<X_SINGLE_DIVIDE_SIGN;
		strStream<<pData->dwServerID; 
		strStream<<X_SINGLE_DIVIDE_SIGN;
		CHAR szIP[X_IP_LEN] = {0};
		strncpy(szIP, inet_ntoa((*(in_addr*)&pData->dwClientIP)), X_IP_LEN);
		strStream<<szIP;
		strStream<<X_SINGLE_DIVIDE_SIGN;
		strStream<<pData->strAccount;
		strStream<<X_SINGLE_DIVIDE_SIGN;
		strStream<<pData->strKeyCode;
		strStream<<"\r\n";
		//��������
		m_pTran->Send((void *)strStream.str().c_str(),strStream.str().length());

		EnterCriticalSection(&m_RetListLock);
		m_listRetKeyCodeInfo.PushBack(pData);
		LeaveCriticalSection(&m_RetListLock);

		EnterCriticalSection(&m_ListLock);
		pData = m_listKeyCodeInfo.PopFront();		// ȡ����һ��
		LeaveCriticalSection(&m_ListLock);
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
}

/* result�Ķ��� 
* 0-���ܴ���  
* 1-���ѳ�ֵ��   
* 2-����Ч���ѹ�
* 3-�����Ͳ���NPC��
* 4-�����������
* 5-������������
* 6-����ʧ��
* 7-�ǡ�ʥħ֮Ѫ��
* 8-�ǡ�ʥħ֮Ѫ��
* 9-������޵���
* 10-��ֵ�ɹ�
* 11-����ʧ��
* 12-�������Ѿ�ʹ�ù�
* 13-�ʺŷǻ�Ա����
*/
VOID KeyCodeBridgeSession::HandleRecvResult(LPBYTE pRecv)
{
	// ֱ��ת
	string strRecv = (CHAR*)pRecv;
	vector <tagRecvResult> vecOrangeData;

	// ��������
	if (ParseProofData(strRecv, vecOrangeData) == FALSE)
		return;

	INT nVectSize = vecOrangeData.size();
	if (nVectSize <= 0)
		return;

	// ���ͻ��˷��ؽ��
 	EnterCriticalSection(&m_RetListLock);
	for (INT nCnt = 0; nCnt < nVectSize; nCnt++)
	{
		TList<tagKeyCodeInfo*>::TListIterator iterator = m_listRetKeyCodeInfo.Begin();
		tagKeyCodeInfo* pData = NULL;

		while( m_listRetKeyCodeInfo.PeekNext(iterator, pData) )
		{
			if (!P_VALID(pData))
				continue;
			if (!pData->strKeyCode.compare((vecOrangeData[nCnt]).strKeyCode))
			{
				Role* pRole = g_roleMgr.GetRolePtrByID(pData->dwRoleID);
				if (P_VALID(pRole))
				{
					tagNS_KeyCode send;
					send.nErrorCode = (vecOrangeData[nCnt]).nResult;
					pRole->SendMessage(&send, send.dwSize);
				}
				m_listRetKeyCodeInfo.Erase(pData);
				SAFE_DEL(pData);
				break;
			}
		}
	}

 	LeaveCriticalSection(&m_RetListLock);
}

BOOL KeyCodeBridgeSession::ParseProofData(const string &strFrameString, vector<tagRecvResult> &vecOrangeData)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(X_RECEIVE_DIVIDE_SIGN,iprepos);
		//���һ��
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//������һ���ַ���
			CheckString(strTemp);

			tagRecvResult recvResult;
			ParseSingleData(strTemp, recvResult.strKeyCode, recvResult.nResult);

			vecOrangeData.push_back(recvResult);
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);

		tagRecvResult recvResult;
		ParseSingleData(strTemp, recvResult.strKeyCode, recvResult.nResult);

		vecOrangeData.push_back(recvResult);
		iprepos = icurpos + 1;
	}
	return TRUE;
}

BOOL KeyCodeBridgeSession::ParseSingleData(const string &strFrameString, string &strKeyCode, INT &nResult)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(X_SINGLE_DIVIDE_SIGN,iprepos);
		//���һ��
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//������һ���ַ���
			CheckString(strTemp);
			nResult = atoi(strTemp.c_str());
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);
		strKeyCode = strTemp;
		iprepos = icurpos + 1;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------------------------
// ȥ���س�����
//-----------------------------------------------------------------------------------------------
BOOL KeyCodeBridgeSession::CheckString(string& str)
{
	// �Իس����н���
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// ����û����Ƿ�Ϊ��
	return str.empty();
}
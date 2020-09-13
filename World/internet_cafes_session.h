/********************************************************************
	created:	2012/05/07
	created:	7:5:2012   17:17
	filename: 	e:\code\World\internet_cafes_session.h
	file ext:	h
	author:		zhangzhihua
	
	purpose:	��Ϸ�������뺫������Time�������Ի�
*********************************************************************/
#pragma once

#include "StdAfx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/time.h"
#include "internet_cafes_define.h"

#ifdef ON_INTERNET_CAFE

//--------------------------------------------------------------------------------
// Login Session
//--------------------------------------------------------------------------------
class InternetCafeSession
{
public:
	InternetCafeSession();
	~InternetCafeSession();

	BOOL Init();
	VOID Destroy();
	VOID Update();
	BOOL IsOn();
	BOOL IsWell() { return (m_pTran->IsConnected() && IsOn()); }

	VOID Send(LPVOID pMsg, DWORD dwMsgSize)	{ if(FALSE == IsWell() ) return;		m_pTran->Send(pMsg, dwMsgSize); }

	INT	GetUnsendPackageNum() { return m_pTran->GetUnsendPackageNum(); }
	INT	GetReceivedPackageNum() { return m_pTran->GetReceivedPackageNum(); }

public:
	VOID SendPlayerLogin(CHAR* szAccountName, DWORD dwRoleID, DWORD dwIP);
	VOID SendPlayerLogout(CHAR* szAccountName, DWORD dwRoleID, DWORD dwIP);
	VOID SendPing(CHAR* szAccountName, DWORD dwRoleID, DWORD dwIP);
	VOID GetLeftTime(DWORD dwIP);
private:
	VOID UpdateSession();

private:
	// ��ȡ�ļ�, ��ʼ����Ա
	BOOL InitConfig();
	// ע�����е���������
	VOID RegisterAllDummyTSCommand();

	typedef DWORD (InternetCafeSession::*icfunc)(CHAR*, WORD);
	std::map<DWORD, icfunc>	m_mapProc;

	VOID ProcessNetworkMsg(tagT2W_Package* pMsg, DWORD dwMsgSize);

private:
	// ����DummyTS������
	DWORD ThreadConnectDummyTS();

private:
	DWORD HandlePlayerLogin(CHAR* pMsg, WORD);
	DWORD HandlePlayerLogout(CHAR* pMsg, WORD);
	DWORD HandlePing(CHAR* pMsg, WORD);
	DWORD HandleLeftTime(CHAR* pMsg, WORD);
	
private:
	TSFPTrunk<InternetCafeSession>		m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<StreamTransport>			m_pTran;
	TObjRef<Thread>						m_pThread;

	// ���Ӳ���
	CHAR								m_szInternetCafeIP[X_LONG_NAME];		// Time������ip
	DWORD								m_dwInternetCafePort;				// Time������port
	CHAR								m_szCertName[X_LONG_NAME];
	CHAR								m_szKeyName[X_LONG_NAME];
	CHAR								m_szCACertName[X_LONG_NAME];
	CHAR								m_szKeyPasswd[X_LONG_NAME];


	volatile BOOL						m_bTermConnect;				// ��¼��ǰ����״̬
	BOOL								m_bOn;						// �Ƿ������ɽ���
	BOOL								m_bUseSSL;
	BOOL								m_bVerify;

	HANDLE								m_hDummyTSClosed;
};

extern InternetCafeSession g_InternetCafeSession;		// InternetCafe Sessionȫ�ֶ���

#endif
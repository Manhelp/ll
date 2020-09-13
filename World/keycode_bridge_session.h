//-----------------------------------------------------------------------------
// Copyright (c) 2010 KONGZHONG Entertainment All rights reserved.
// filename: keycode_bridge_session.h
// data: 2010-09-06
// last:
// brief: ��Ϸ��������KeyCodeBridgeServer�ĶԻ���
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"

#define GAME_ID 1300000	// ʥħ֮Ѫ�ڻ�Ա�����������ϷID

struct tagKeyCodeInfo
{
	DWORD	dwGameID;
	DWORD	dwServerID;	
	string		strAccount;
	DWORD	dwRoleID;
	string		strKeyCode;
	DWORD  dwClientIP;
	tagKeyCodeInfo(DWORD dwSerID, LPCSTR szAccount, DWORD dwRID, LPCSTR pKeyCode, DWORD dwIP)
	{
		dwGameID = GAME_ID;
		dwServerID = dwSerID;
		strAccount = szAccount;
		dwRoleID = dwRID;
		strKeyCode = pKeyCode;
		dwClientIP = dwIP;
	}
};

struct tagRecvResult 
{
	string strKeyCode;
	INT nResult;
};

//-----------------------------------------------------------------------------
class KeyCodeBridgeSession
{
public:
	KeyCodeBridgeSession();
	~KeyCodeBridgeSession();

public:
	BOOL Init();
	VOID Destroy();

	VOID Update();
	BOOL IsWell() { return (m_bInitOK && m_pTran->IsConnected()); }

	VOID Proof(LPCSTR szAccount, DWORD dwRoleID, LPCSTR szKeyCode, DWORD dwClientIP);

private:
	// ��ȡ�ļ�, ��ʼ����Ա
	BOOL InitConfig();

	// �������ݿ������
	DWORD ThreadConnectKeyCodeBridge();

	// ������֤�б�
	VOID UpdateProofList();

	VOID HandleRecvResult(LPBYTE pRecv);

	BOOL ParseProofData(const string &strFrameString, vector<tagRecvResult> &vecOrangeData);
	BOOL ParseSingleData(const string &strFrameString, string &strKeyCode, INT &nResult);

	BOOL CheckString(string& str);

private:
	TSFPTrunk<KeyCodeBridgeSession>			m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<StreamTransport>	m_pTran;
	TObjRef<Thread>					m_pThread;

	// ���Ӳ���
	CHAR										m_szIP[X_IP_LEN];	// ������ip
	DWORD									m_dwPort;				// ������port
	DWORD									m_dwOn;					// key���ֵ����

	DWORD									m_dwWorldID;				// ������ID

	volatile BOOL				m_bTermConnect;			// ��¼��ǰ����״̬
	volatile BOOL				m_bInitOK;						// �Ƿ��ʼ�����

	TList<tagKeyCodeInfo*>		m_listKeyCodeInfo;			// ��֤�û�����
	TList<tagKeyCodeInfo*>		m_listRetKeyCodeInfo;	// �����������²㴦��

	CRITICAL_SECTION	m_ListLock;
	CRITICAL_SECTION	m_RetListLock;
};

extern KeyCodeBridgeSession g_keyCodeBridgeSession;		// KeyCodeBridge sessionȫ�ֶ���

//-----------------------------------------------------------------------------
//!\file proof_policy_vietnam.h
//!\author lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief Խ�ϵ����� ��֤���� 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.


#pragma once
#include "proof_policy.h"

#define X_OG_RECEIVE_DIVIDE_SIGN ';' //����������Ϣ�ķָ���
#define X_OG_PROOF_SIGN "1"    //��֤��Ϣ

//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPolicyVietnamDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyVietnamDB() {}
	~ProofPolicyVietnamDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// ��ѯ
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);

private:
	DataBase		m_DB;
};

class ProofPolicyVietnam : public ProofPolicy
{
public:
	ProofPolicyVietnam(void): m_Trunk(this), m_nLoginPort(0), m_bTerminateLoginUpdate(FALSE), m_bTerminateLoginConnect(FALSE){}
	virtual ~ProofPolicyVietnam(void){}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��֤�ӿ�
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID,DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*�����Ӫ��ID*/,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	//-------------------------------------------------------------------------
	// ��֤����������״̬
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// ��ҵǳ��¼�
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID) {  ; }

private:

	//-----------------------------------------------------------------------------------------------
	// �õ����������� �����ŷָ�
	//-----------------------------------------------------------------------------------------------
	VOID ParseVietnamProofData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign);

	//-------------------------------------------------------------------------
	// ����login���������߳�
	//-------------------------------------------------------------------------
	VOID	ThreadLoginServerUpdate();

	//-------------------------------------------------------------------------
	// ���ӶԷ�login�������߳�
	//-------------------------------------------------------------------------
	VOID	ThreadConnectLoginServer();

	//-------------------------------------------------------------------------
	// ��ʼ������
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

	//-------------------------------------------------------------------------
	// ������֤�б�
	//-------------------------------------------------------------------------
	VOID	UpdateProofList();

	//-------------------------------------------------------------------------
	// ����login��������Ϣ
	//-------------------------------------------------------------------------
	VOID	UpdateLoginSession();


	//-------------------------------------------------------------------------
	// ����Orange��֤��Ϣ
	//-------------------------------------------------------------------------
	VOID	HandleVietnamLogin(LPBYTE pRecv);


	//-----------------------------------------------------------------------------------------------
	// ȥ���س�����
	//-----------------------------------------------------------------------------------------------
	BOOL CheckString(string& str);

private:

	TSFPTrunk<ProofPolicyVietnam>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	//����
	TObjRef<StreamTransport>	m_pTransLogin;
	//��ThreadUpdate���� ����Ҫ�����̰߳�ȫ
	std::map<DWORD, string>		m_mapClientIDVsAccount;	// ClientID������˺ŵĶ��ձ�
	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateLoginUpdate;
	volatile BOOL				m_bTerminateLoginConnect;

	//-------------------------------------------------------------------------
	// ���Ӳ���
	//-------------------------------------------------------------------------
	CHAR						m_szLoginIP[X_IP_LEN];		// ����Խ����֤��������IP
	INT							m_nLoginPort;				// ����Խ����֤�������Ķ˿�
	//-----------------------------------------------------------------------
	// ��֤����
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		string		strAccoutName;
		string		strPsd;
		BYTE		byAccountData[X_LONG_NAME];		//
		DWORD		dwIP;				//ip
		DWORD		dwWorldNameCrc;
		string		strMac;

		tagPlayerProofData(DWORD _dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID,DWORD _dwIP,DWORD _dwWorldNameCrc, LPCSTR szMac)
		{
			dwClientID		=	_dwClientID;
			strAccoutName	=	szAccoutName;
			strPsd			=	szPsd;
			memcpy(byAccountData,szGUID,X_LONG_NAME);
			dwIP			=	_dwIP;
			dwWorldNameCrc	=	_dwWorldNameCrc;
			strMac			=	szMac;
		}
	};

	TSafeList<tagPlayerProofData*>		m_listProofData;		// ��֤�û�����
	TSafeList<tagPlayerProofData*>		m_listProofSolvedData;	// �����������²㴦��

	
	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPolicyVietnamDB					m_ProofDB;				// ���ݿ�ӿ�
};

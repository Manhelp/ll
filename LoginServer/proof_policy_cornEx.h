//-----------------------------------------------------------------------------
//!\file proof_policy_cornEx.h
//!\author zxzhang
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief ����������֤���� 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.


#pragma once
#include "proof_policy.h"
#include "proof_policy_corn.h"
#include "event_mgr.h"


//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPolicyKoreaDB 
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyKoreaDB() {}
	~ProofPolicyKoreaDB() {}

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

class ProofPolicyKorea : public ProofPolicy ,public namepolicy::EventMgr<ProofPolicyKorea>
{
public:
	ProofPolicyKorea(void): m_Trunk(this), m_nLoginPort(0), m_bTerminateLoginUpdate(FALSE), m_bTerminateLoginConnect(FALSE){}
	virtual ~ProofPolicyKorea(void){}

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


	VOID   PlayerLoginOut(DWORD dwSenderID, LPVOID pEventMessage);

	//-------------------------------------------------------------------------
	// ��ҵǳ��¼�
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID) {  ; }

private:

	//-----------------------------------------------------------------------------------------------
	// �õ����������� �����ŷָ�
	//-----------------------------------------------------------------------------------------------
	VOID ParseKoreaProofData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign);

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
	VOID	HandleKoreaLogin(LPBYTE pRecv);


	//-----------------------------------------------------------------------------------------------
	// ȥ���س�����
	//-----------------------------------------------------------------------------------------------
	BOOL CheckString(string& str);

private:

	TSFPTrunk<ProofPolicyKorea>	m_Trunk;
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
		string	    strKRUseName;   //����ʹ��
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
			strKRUseName.clear();
		}
	};

	

	TSafeList<tagPlayerProofData*>		m_listProofData;		// ��֤�û�����
	TSafeMap<DWORD,tagPlayerProofData*>	m_mapProofSolvedData;	// �����������²㴦��
	TMap<DWORD,DWORD>					m_mapClientID;


	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPocilyCornDB					m_ProofDB;				// ���ݿ�ӿ�
};

#pragma once

#include "proof_policy.h"

//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPolicyQuanQuanDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyQuanQuanDB() {}
	~ProofPolicyQuanQuanDB() {}

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

struct ISDOAUserInfoAuthen;
struct ISDOAMsg;
//-----------------------------------------------------------------------------
// ʢ��ȦȦ��֤����
//-----------------------------------------------------------------------------
class ProofPolicyQuanQuan :
	public ProofPolicy
{
	typedef ISDOAUserInfoAuthen* (*pFuncCreateUserInfoAuthen)(void);
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyQuanQuan(void): m_Trunk(this), m_bTerminateUpdate(FALSE){}
	~ProofPolicyQuanQuan(void){}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��֤�ӿ�
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP, DWORD dwWorldNameCrc,DWORD dwChannelID/*�����Ӫ��ID*/,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	//-------------------------------------------------------------------------
	// ��֤����������״̬
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// ��ҵǳ��¼�
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID){}

private:
	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	VOID	ThreadUpdate();

	//-------------------------------------------------------------------------
	// �����߳�
	//-------------------------------------------------------------------------
	VOID	ThreadConnectServer();

	//-------------------------------------------------------------------------
	// ������֤�б�
	//-------------------------------------------------------------------------
	VOID	UpdateProofList();

	//-------------------------------------------------------------------------
	// ������Ϣ
	//-------------------------------------------------------------------------
	VOID	UpdateSession();

	//-------------------------------------------------------------------------
	// ���ͽ����˺���Ϣ
	//-------------------------------------------------------------------------
	VOID	ThreadUpdateGoldenList();

	//-------------------------------------------------------------------------
	// ���ͽ����˺���Ϣ��������
	//-------------------------------------------------------------------------
	bool	SendGloden2Server(string account, DWORD sectionID, DWORD worldID); // UDP


	int OnUserInfoCallback(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg);

	char* IP2String(DWORD dwIP)const;
	string GetConfigFile()const;
private:
	HINSTANCE					m_hShengDaDll; //shengda dll
	pFuncCreateUserInfoAuthen	m_pFuncCreate;

	TSFPTrunk<ProofPolicyQuanQuan>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	TObjRef<StreamTransport>	m_pTransport;
	TObjRef<Thread>				m_pThreadGolden;

	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;
	
	ISDOAUserInfoAuthen*		m_pAuth;

	//-------------------------------------------------------------------------
	// ��֤����
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		BYTE		byAccountData[1024];		//���ܰ�	��ʢ��˾�ṩ�ĵ�¼������
		DWORD		dwIP;
		DWORD		dwWordNameCrc;
		string		strMac;

		tagPlayerProofData(DWORD _dwClientID, LPCSTR szGUID, DWORD IP, DWORD _dwWordNameCrc, LPCSTR szMac)
		{
			dwClientID		=	_dwClientID;
			dwIP			=	IP;
			strcpy_s((char*)byAccountData,1024, szGUID);
			byAccountData[1023] = 0;
			dwWordNameCrc = _dwWordNameCrc;
			strMac			=	szMac;
		}
	};

	enum ProofResult
	{
		E_Success,
		E_ConnectServer,
		E_EmptyField,
		E_AlreadyAuthrized,
		E_AuthFailed,
		E_WrongFormat,
		E_OutOfTime,
		E_SysError,
		E_SocketError,
	};

	struct tagPlayerProofResult
	{
		DWORD		dwClientID;
		string strAccountName;
		ProofResult eResult;
		bool bGuard;
		BYTE	byNeedShengDaInfo;
	};

	struct tagGoldenMember
	{
		string strAccountName;
		DWORD	dwWorldNameCrc;
	};

	TSafeList<tagPlayerProofData*>		m_listProofData;		// ��֤�û�����
	TSafeList<tagPlayerProofResult*>	m_listProofResult;		// ��֤�������
	TSafeList<tagGoldenMember*>	m_listGoldenMemberList;		// �����û�����
	TSafeMap<DWORD, DWORD>		m_mapClientID2World;

	SOCKET                      m_hSockClient; 
	SOCKADDR_IN                 m_addrSrv; 

	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPolicyQuanQuanDB					m_ProofDB;				// ���ݿ�ӿ�
};

inline char* ProofPolicyQuanQuan::IP2String(DWORD dwIP)const
{
	in_addr addr;
	addr.S_un.S_addr = dwIP;
	return inet_ntoa(addr);
}
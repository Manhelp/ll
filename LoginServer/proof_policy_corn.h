//-----------------------------------------------------------------------------
//!\file proof_policy_corn.h
//!\author lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief ���������� ��֤���� 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.

#pragma once

#include "proof_policy_corn_define.h"
//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPocilyCornDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPocilyCornDB() {}
	~ProofPocilyCornDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// ��ѯ
	//-------------------------------------------------------------------------
	BOOL    QueryAccount(const string& name,string& seckey); 
	BOOL	QueryAccount(LPCSTR szAccountName,LPCSTR szPsd, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);

private:
	DataBase		m_DB;
};

//-----------------------------------------------------------------------------
// ��֤����
//-----------------------------------------------------------------------------
class ProofPolicyCorn : public ProofPolicy
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyCorn() : m_Trunk(this),m_nTimeOut(0) {}
	virtual ~ProofPolicyCorn() { }

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��֤�ӿ�
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	//-------------------------------------------------------------------------
	// ��֤����������״̬
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	VOID	ThreadUpdate();

	//-------------------------------------------------------------------------
	// ��ҵǳ��¼�
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID){}

	INT     GetTimeOut()   { return m_nTimeOut ;}
	//-------------------------------------------------------------------------
	//�Ѽ�������ת�����ַ���
	//-------------------------------------------------------------------------
	BOOL    DecryptDataToString(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc,const CHAR* pPsd);
    
	//-------------------------------------------------------------------------
	//�Ӽ������ݽ������ַ���
	//-------------------------------------------------------------------------
	BOOL    ParseStringFromDecryptData(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc);
private:
	TSFPTrunk<ProofPolicyCorn>		m_Trunk;
	TObjRef<Thread>					m_pThread;

	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL					m_bTerminateUpdate;
    
	INT                             m_nTimeOut;
	//-------------------------------------------------------------------------
	// ��֤����
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		string		strAccoutName;
		string		strPsd; //
		string      strGUID;
		string      strMac;



		tagPlayerProofData(DWORD _dwClientID, LPCSTR szAccoutName,LPCSTR szPsd,LPCSTR szGUID,LPCSTR szMac)
		{
			dwClientID		=	_dwClientID;
			strAccoutName	=	szAccoutName;
			strPsd	=	szPsd;
			strGUID = szGUID;
			strMac = szMac;
		}
	};

	TSafeMutexList<tagPlayerProofData*>	m_listProofData;		// ��֤�û�����

	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPocilyCornDB					m_ProofDB;				// ���ݿ�ӿ�
};

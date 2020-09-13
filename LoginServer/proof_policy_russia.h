//-----------------------------------------------------------------------------
//!\file proof_policy_russia.h
//!\author liutingting
//!
//!\date 2011-5-10
//! last 2011-5-10
//!
//!\brief ����˹��֤����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "proof_policy.h"
#include "game_net_web.h"

//-----------------------------------------------------------------------------
// ����֤���Ե����ݿ�
//-----------------------------------------------------------------------------
class ProofPolicyRussiaDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyRussiaDB() {}
	~ProofPolicyRussiaDB() {}

	//-------------------------------------------------------------------------
	// ��ʼ��
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// ��ѯ
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	//BOOL	UpdatePsd(LPCSTR szAccountName, LPCSTR szNewPsd);
	//BOOL	UpdateMibao(LPCSTR szAccountName, LPCSTR szMibao);

private:
	DataBase		m_DB;
};

//-----------------------------------------------------------------------------
// ��֤����
//-----------------------------------------------------------------------------
class ProofPolicyRussia : public ProofPolicy
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyRussia() : m_Trunk(this), m_bTerminateUpdate(FALSE){}
	~ProofPolicyRussia() {}

public:
	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// ��֤�ӿ�
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*�����Ӫ��ID*/,LPCSTR szMac, LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

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
	// ��ʼ������
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

private:
	TSFPTrunk<ProofPolicyRussia>	m_Trunk;
	TObjRef<Thread>					m_pThread;
	TObjRef<Util>					m_pUtil;


	//-------------------------------------------------------------------------
	// ��֤����
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		CHAR		szUserID[X_SHORT_NAME];
		CHAR		szAppKey[48];
		CHAR		szToken[48]; 

		tagPlayerProofData( DWORD dwClientIDIn, LPCSTR szInUserID,LPCSTR szInAppKey, LPCSTR szInToken )
		{
			dwClientID = dwClientIDIn;

			if(P_VALID(szInUserID))
				lstrcpynA( szUserID,szInUserID,X_SHORT_NAME);

			if(P_VALID(szInAppKey))
				lstrcpynA( szAppKey, szInAppKey,48);

			if(P_VALID(szInToken))
				lstrcpynA( szToken, szInToken, 48);
		}
	};

	TSafeMutexList<tagPlayerProofData*>	m_listProofData;		// ��֤�û�����

	//-------------------------------------------------------------------------
	// �߳̽�����־
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;

	//--------------------------------------------------------------------------
	// ���ݿ�
	//--------------------------------------------------------------------------
	ProofPolicyRussiaDB					m_ProofDB;				// ���ݿ�ӿ�
	GameNetWeb							m_GameNetWeb;			// http������
	CHAR								m_szUrl[256];			// ����GameNetWeb�ĵ�ַ
};
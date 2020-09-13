//-----------------------------------------------------------------------------
//!\file player_mgr.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ��ҹ�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Mutex.h"
#include "Singleton.h"
#include "player.h"

class Player;
class ProofPolicy;

//-----------------------------------------------------------------------------
// ��ҹ�����
//-----------------------------------------------------------------------------
class PlayerMgr : public Singleton<PlayerMgr>  
{
public:
    //-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
    PlayerMgr();
    ~PlayerMgr();

    //--------------------------------------------------------------------------
	// ��ʼ�������º�����
	//--------------------------------------------------------------------------
    BOOL			Init();
    VOID			Update();
    VOID			Destroy();

	//--------------------------------------------------------------------------
	// ����Get
	//--------------------------------------------------------------------------
	DWORD			GetCurrVer()		{ return m_dwCurrVer; }
	INT				GetPlayerNum()		{ return m_nPlayerNum; }
	INT				GetPlayerLoging()	{ return m_nPlayerLoging; }
	INT				GetProofResultNum()	{ return m_nProofResultNum; }
	EProofPolicy	GetProofPolicy()	{ return m_ePolicy; }
	ProofPolicy*	GetProofPolicyPtr()	{ return m_pPolicy; }
	bool            GetYYMinSerOn()      {return m_bYYMixSerOn;}
	bool			GetGMLoginOn()		{ return m_bGMLogin;}
	bool			GetWebLoginOn()		 { return m_bWebLoginModeOn; }

	//--------------------------------------------------------------------------
	// ��������
	//--------------------------------------------------------------------------
    LPBYTE			RecvMsg(DWORD& dwSize, INT& nMsgNum, DWORD dwCDIndex);
	VOID			ReturnMsg(LPBYTE pMsg);
	VOID			SendMsg(DWORD dwCDIndex, LPBYTE pMsg, DWORD dwSize);

	VOID			StartSendAllMsg()	{ m_pNetSession->ActiveSend(); }

	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
    VOID			Kick(DWORD dwCDIndex)		{ m_pNetSession->Kick(dwCDIndex); }

	//--------------------------------------------------------------------------
	// ID���� ��1��0xFFFFѭ�� Ϊ�˱�֤Ѹ����֤�����е�clientid������ָ�ֵ���
	//--------------------------------------------------------------------------
	DWORD			GenerateClientID()				
	{ 
		++m_dwClientIDGen; 
		if(m_dwClientIDGen > 0xFFFF)
		{
			m_dwClientIDGen = 1;
		}
		return m_dwClientIDGen; 
	}
    
	//--------------------------------------------------------------------------
	// ������
	//--------------------------------------------------------------------------
	VOID			AddPlayerToAll(Player* pPlayer)				{ m_mapAllPlayer.Add(pPlayer->GetClientID(), pPlayer); }
	VOID			AddPlayerToLogining(Player* pPlayer)		{ m_mapLoginingPlayer.Add(pPlayer->GetClientID(), pPlayer); }
	VOID			RemovePlayerFromAll(DWORD dwClientID)		{ m_mapAllPlayer.Erase(dwClientID); }
	VOID			RemovePlayerFromLogining(DWORD dwClientID)	{ m_mapLoginingPlayer.Erase(dwClientID); }
	VOID			PlayerLogout(Player* pPlayer);

	VOID			AddAccount(DWORD dwAccountID, DWORD dwClientID);
	VOID			RemoveAccount(DWORD dwAccountID);
	BOOL			IsAccountExist(DWORD dwAccountID)			{ return m_mapAccountIDClientID.IsExist(dwAccountID); }

	const tagAccountData*		GetCachedAccountData(DWORD dwAccountID)	{	return m_mapAccountData.Peek(dwAccountID);		}
	VOID			CacheAccountName(DWORD dwAccountID, const CHAR* szAccountName);
	VOID			CacheIpAddres( DWORD dwAccountID, DWORD dwIp);
	VOID			CacheGuard( DWORD dwAccountID, BOOL bGuard);
	VOID			EraseCachedAccountData(DWORD dwAccountID);
	VOID			CleanCachedAccountDatas();
	VOID			MapAccountName2AccountID(LPCSTR szAccountName, DWORD dwAccountID);
	DWORD			GetAccountIDByAccountName(LPCSTR szAccountName);
	DWORD			GetChannelNum() { return m_dwChannelNum; }

	//--------------------------------------------------------------------------
	// ��֤���
	//--------------------------------------------------------------------------
	VOID			Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID, const CHAR *pszMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	VOID			CacheMac( DWORD dwAccountID, LPCSTR szMac);
	LPCSTR          RequestTokenID(DWORD dwConnID,string& strTokenID);
    bool            VerifyToken(LPVOID pLoginCmdInfo,DWORD dwCDIndex);


	//--------------------------------------------------------------------------
	// ��ȡ���ص�ַ
	//--------------------------------------------------------------------------
	bool			GetWebLoginInfo(LPVOID pWebLoginInfo,DWORD& dwMsgSize);
	bool			GetRechargeURL(OUT char* pLoginURLBuff ,DWORD& dwLoginURLSize,OUT char* pBuff,DWORD& dwURLSize,DWORD dwCropID);

private:
	//--------------------------------------------------------------------------
	// ��ʼ�������ļ�
	//--------------------------------------------------------------------------
	BOOL			InitConfig();
	BOOL			InitOrangeConfig();

	//--------------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------------
	VOID			UpdateSession();
	VOID			UpdateProofResult();

	//--------------------------------------------------------------------------
	// ��½�ǳ��ص�
	//--------------------------------------------------------------------------
    DWORD			LoginCallBack(LPVOID, LPVOID);
    DWORD			LogoutCallBack(LPVOID);

	//--------------------------------------------------------------------------
	// ��֤����ص�
	//--------------------------------------------------------------------------
	VOID			ProofCallBack(INT nRet, tagProofResult* pResult);

private:
    TSFPTrunk<PlayerMgr>		m_Trunk;
    TObjRef<Util>				m_pUtil;

	//--------------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------------
    TObjRef<XServer>			m_pNetSession;
    INT							m_nPort;

	//--------------------------------------------------------------------------
	// ID������
	//--------------------------------------------------------------------------
    DWORD						m_dwClientIDGen;

	//--------------------------------------------------------------------------
	// �汾�ź�����
	//--------------------------------------------------------------------------
    DWORD						m_dwCurrVer;

	//--------------------------------------------------------------------------
	// ͳ����Ϣ
	//--------------------------------------------------------------------------
	volatile INT				m_nPlayerNum;
	volatile INT				m_nPlayerLoging;
	volatile INT				m_nProofResultNum;

	//--------------------------------------------------------------------------
	// ������
	//--------------------------------------------------------------------------
	TSafeMap<DWORD, Player*>			m_mapAllPlayer;			// ���е�Player,��ClientID Ϊ key
	TMap<DWORD, Player*>				m_mapLoginingPlayer;	// ���ڵ�¼�����
	TMap<DWORD, DWORD>					m_mapAccountIDClientID;	// ���ߵ�AccountID��ClientID��Ӧ��
	TSafeList<tagProofResultFull*>		m_listProofResult;		// ���سɹ������

	TSafeMap<DWORD, tagAccountData*>	m_mapAccountData;		// ���е�AccountID�Ļ������ݣ�����AccountName��guard��ip ��ʼ�����أ���̬����
	TSafeMap<DWORD, DWORD>				m_mapAccountNameCrc2AccountID;	// accountid -> namecrc	��ʼ��ʱ���أ����Ҷ�̬���

	//--------------------------------------------------------------------------
	// ��֤����
	//--------------------------------------------------------------------------
	EProofPolicy						m_ePolicy;				// ��֤��ʽ
	ProofPolicy*						m_pPolicy;				// ��֤����

	std::multimap<tstring, DWORD>		m_mapGMvsIP;				//gmָ��ip
	BOOL								m_bUseGMAccess;				//�Ƿ�ʹ��gm��ַ����
	bool								m_bGMLogin;					//GM�������Ŷӿ���
	std::set<DWORD>						m_setGMIP;
	bool                                m_bYYMixSerOn;              //�Ƿ���YY���
	HINSTANCE                           m_hInstYYDLL;               //YY�Ķ�̬��
	typedef   int (*PtrInitType)();     
	typedef   void (*PtrCleanUpType)(); 
	typedef const char* (*PtrGenRequestTypeEx)(int);  
	typedef int (*PtrVerifyTokenType)(const char*,int);
	typedef int (*PtrHexToStringType)(const char*, char*, int);
	typedef int (*PtrGetAttributeType)(const char*, const char*, char*, int);
	PtrInitType                                m_pYAuth_init;              //YY���ʼ��
	PtrCleanUpType                             m_pYAuth_cleanUp;           //YY������
	PtrGenRequestTypeEx                        m_YAuth_genRequest;         //YY���ȡTokenID�Ľӿ�   
	PtrVerifyTokenType                         m_pYAuth_verifyToken;       //�յ�YY�ĵ�¼��Ϣ�󣬽���У��
	PtrHexToStringType                         m_pYAuth_HexToString;       //ת��16����
	PtrGetAttributeType                        m_pYAuth_GetAttribute;      //��ȡ��Ϣ



	//--------------------------------------------------------------------------
	// �Ϻ�Web���뷽��
	//--------------------------------------------------------------------------
	struct tagWebNoteInfo
	{
		string  strURL;
		string  strRechargeURL;
		string  strLoginURL;
		tstring tstrDescName;
		DWORD   dwCropID;
	};
	VOID InitWebNoteInfo();
	VOID DestroyWebNoteInfo();
	bool								m_bWebLoginModeOn;			//Web�����Ƿ���
	DWORD								m_dwChannelNum;				//Web��������
	TMap<DWORD,tagWebNoteInfo*>			m_MapURL;					//URL��ַ	

};

//----------------------------------------------------------------------------
// ����Ϣ
//----------------------------------------------------------------------------
inline LPBYTE PlayerMgr::RecvMsg(DWORD& dwSize, INT& nMsgNum, DWORD dwCDIndex)
{
	return m_pNetSession->Recv(dwCDIndex, dwSize, nMsgNum);
}

//----------------------------------------------------------------------------
// �黹��Ϣ
//----------------------------------------------------------------------------
inline VOID PlayerMgr::ReturnMsg(LPBYTE pMsg)
{
	m_pNetSession->FreeRecved(pMsg);
}

//----------------------------------------------------------------------------
// ������Ϣ
//----------------------------------------------------------------------------
inline VOID PlayerMgr::SendMsg(DWORD dwCDIndex, LPBYTE pMsg, DWORD dwSize)
{
	m_pNetSession->Send(dwCDIndex, pMsg, dwSize);
}

//-----------------------------------------------------------------------------
// ����һ���ʺ�
//-----------------------------------------------------------------------------
inline VOID PlayerMgr::AddAccount(DWORD dwAccountID, DWORD dwClientID)
{
	if( !P_VALID(dwAccountID) || !P_VALID(dwClientID) )
		return;

	m_mapAccountIDClientID.Add(dwAccountID, dwClientID);
}

//------------------------------------------------------------------------------
// �Ƴ�һ���ʺ�
//------------------------------------------------------------------------------
inline VOID PlayerMgr::RemoveAccount(DWORD dwAccountID)
{
	if( !P_VALID(dwAccountID) ) return;

	m_mapAccountIDClientID.Erase(dwAccountID);
}

#define sPlayerMgr PlayerMgr::getSingleton()


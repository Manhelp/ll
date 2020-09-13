//-----------------------------------------------------------------------------
//!\file world_session.h
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief ����������������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Mutex.h"

//-----------------------------------------------------------------------------
// ���������ܳ��ܵ��������������ǿ�����ƣ�
//-----------------------------------------------------------------------------
const INT MAX_PLAYER_NUM = 5000;

const INT DAYS_ONE_WEEK = 7;

class PlayerSession;
class Role;

class WorldSession
{
public:
	WorldSession();
	~WorldSession();

	//-------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//-------------------------------------------------------------------------
	BOOL			Init();
	VOID			Update();
	VOID			Destroy();

	BOOL			LoadGameConfig();

	//-------------------------------------------------------------------------
	// ����Get
	//-------------------------------------------------------------------------
	DWORD			GetPort()				{ return m_nPort; }

	//-------------------------------------------------------------------------
	// �ͻ����������
	//-------------------------------------------------------------------------
	PlayerSession*	FindSession(DWORD dwID);
	PlayerSession*	FindGlobalSession(DWORD dwID);
	BOOL			IsSessionExist(DWORD dwID);

	VOID			AddSession(PlayerSession* pSession);
	VOID			RemoveSession(UINT32 dwID);
	VOID			Kick(DWORD dwInternalIndex);

	VOID			AddGlobalSession(PlayerSession* pSession);
	VOID			RemoveGlobalSession(DWORD dwSessionID);

	VOID			UpdateSession();
	VOID			DoHouseKeeping();

	INT				GetAllOLAccountID(DWORD* pAccountIDs);

	//---------------------------------------------------------------------------
	// ��Ϣ���
	//---------------------------------------------------------------------------
	__forceinline LPBYTE RecvMsg(DWORD& dwSize, INT& nMsgNum, DWORD dwInternalIndex)
	{
		return m_pNetSession->Recv(dwInternalIndex, dwSize, nMsgNum);
	}

	__forceinline VOID ReturnMsg(LPBYTE pMsg)
	{
		m_pNetSession->FreeRecved(pMsg);
	}

	VOID SendMsg(DWORD dwInternalIndex, LPBYTE pMsg, DWORD dwSize);

	__forceinline INT GetSendCast(DWORD dwInternalIndex)
	{
		return m_pNetSession->GetSendCast(dwInternalIndex);
	}

	__forceinline INT GetSendCastSize(DWORD dwInternalIndex)
	{
		return m_pNetSession->GetSendCastSize(dwInternalIndex);
	}

	//---------------------------------------------------------------------------
	// ͳ����Ϣ
	//---------------------------------------------------------------------------
	INT	GetPlayerNumLimit()				{ return m_nPlayerNumLimit; }
	INT GetPlayerNumCurrent()			{ return m_nPlayerNumCurrent; }
	INT GetPlayerNumDisplay()			{ INT display = (int)(m_nPlayerNumCurrent * m_lfPlayerNumFactor); return display>m_nPlayerNumLimit?m_nPlayerNumLimit:display;}
	INT GetPlayerNumPeek()				{ return m_nPlayerNumPeek; }
	INT GetPlayerLoginTimes()			{ return m_nPlayerIn; }
	INT GetPlayerLogoutTimes()			{ return m_nPlayerOut; }
	INT GetMsgSendCast()				{ return m_nSendCast; }
	INT GetMsgSendThisTick()			{ return m_nMsgSendThisTick; }
	INT GetMsgProceedThisTick()			{ return m_nMsgProceedThisTick; }
	INT GetMsgRecvWait()				{ return m_nMsgRecvWait; }

	INT64 GetTotalSendSize()			{ return m_pNetSession->GetSendSize(); }
	INT64 GetTotalRealSendSize()		{ return m_pNetSession->GetRealSendSize(); }
	INT GetSendComRatio();

	VOID SetMsgSendCast(INT nNum)			{ InterlockedExchange((LPLONG)&m_nSendCast, nNum); }
	VOID SetUnitSendThisTick(INT nNum)		{ InterlockedExchange((LPLONG)&m_nMsgSendThisTick, nNum); }
	VOID SetUnitProceedThisTick(INT nNum)	{ InterlockedExchange((LPLONG)&m_nMsgProceedThisTick, nNum); }
	VOID SetUnitRecvWait(INT nNum)			{ InterlockedExchange((LPLONG)&m_nMsgRecvWait, nNum); }

	VOID SetPlayerNumLimit(INT nPlayerLimit)
	{
		if( nPlayerLimit < 0 ) return;
		if( nPlayerLimit > MAX_PLAYER_NUM ) nPlayerLimit = MAX_PLAYER_NUM;

		InterlockedExchange((LPLONG)&m_nPlayerNumLimit, nPlayerLimit);
	}
	VOID PlayerLogin()
	{
		InterlockedExchangeAdd((LPLONG)&m_nPlayerNumCurrent, 1);
		InterlockedExchangeAdd((LPLONG)&m_nPlayerIn, 1);

		// �����˷�ֵ
		if( GetPlayerNumDisplay() > m_nPlayerNumPeek )
		{
			InterlockedExchange((LPLONG)&m_nPlayerNumPeek, GetPlayerNumDisplay());
		}
	}
	VOID PlayerLogout()
	{
		InterlockedExchangeAdd((LPLONG)&m_nPlayerNumCurrent, -1);
		InterlockedExchangeAdd((LPLONG)&m_nPlayerOut, 1);
	}

	VOID SetPlayNumberFactor(float f)
	{
		if( f <=0.0) return;
		m_lfPlayerNumFactor = f;

		// �����˷�ֵ
		if( GetPlayerNumDisplay() > m_nPlayerNumPeek )
		{
			InterlockedExchange((LPLONG)&m_nPlayerNumPeek, GetPlayerNumDisplay());
		}
	}

	VOID UpdateMediaOnlineNum();
	VOID OnRoleLogin(INT nDistributionID);
	VOID OnRoleLogout(INT nDistributionID);
	VOID SaveOnlineNum2DB();
	BOOL GetDBQBOpenState(DWORD dwWeekDay);
	VOID PrintRoleInfo();
	BOOL m_bIsPrintInfo;
	BOOL m_bIsStartPrint;

private:
	TSFPTrunk<WorldSession>				m_Trunk;

	TObjRef<Util>						m_pUtil;
	//TObjRef<Thread>						m_pThread;

	TMap<DWORD, PlayerSession*>			m_mapAllSessions;		// �������ӵ��������Ŀͻ������ӣ��ʺ�Ϊ������
	Mutex								m_AllSessionMutex;		// m_mapAllSessions����

	TMap<DWORD, PlayerSession*>			m_mapGlobalSessions;	// ���л�û�н��뵽��Ϸ�еĿͻ������ӣ��ʺ�Ϊ������
	Mutex								m_GlobalSessionMutex;	// m_mapGlobalSessions����

	TList<PlayerSession*>				m_listInsertPool;		// Ҫ���뵽Global Session�е�Session
	Mutex								m_InsertPoolMutex;		// m_listInsertPool����

	TMap<INT, DWORD>		m_mapMediaOnlineNum;		// ����ý�����������ͳ��map
	Mutex								m_mapMediaOnlineNumMutex;		// m_mapMediaOnlineNum����
	tagDWORDTime				m_dwLastUpdateTime;	// ��һ�������ݿ��и��� ����ý�������������ʱ��

	TObjRef<XServer>					m_pNetSession;			// ��ɶ˿�
	INT									m_nPort;				// �����˿�
	double								m_lfPlayerNumFactor;	// ��ʾ��������������

	volatile INT						m_nPlayerIn;			// ����ҵ�������
	volatile INT						m_nPlayerOut;			// ����ҵǳ�����

	volatile INT						m_nPlayerNumLimit;		// ��������������
	volatile INT						m_nPlayerNumCurrent;	// ��ǰ��������

	volatile INT						m_nPlayerNumPeek;		// ��ֵ����

	volatile INT						m_nSendCast;			// ����ײ㻹û���յ�����ȷ�ϵķ�����Ϣ����
	volatile INT						m_nMsgSendThisTick;		// ����ײ㱾tick���͵���Ϣ����
	volatile INT						m_nMsgProceedThisTick;	// ��Tick�����˶�����Ϣ
	volatile INT						m_nMsgRecvWait;			// ���ж��ٽ�����Ϣ�ڵȴ�����
	BOOL								m_bDBQBOpenState[DAYS_ONE_WEEK]; // �ᱦ�������״̬�������տ�ʼ

private:
	DWORD LoginCallBack(LPVOID, LPVOID);						// �ͻ��˵�½���������ص�����
	DWORD LogoutCallBack(LPVOID);								// �ͻ��˵ǳ����������ص�����

private:
	volatile INT						m_nTreasureSum;			// �������ܿ���������
public:
	INT GetTreasureSum()				{ return m_nTreasureSum; }
	VOID IncTreasureSum();
	VOID SetTreasureSum(INT nSum);

public:
	INT GetMaxRecvWaitUnitNum() const { return m_nMaxRecvWaitUnitNum; }
	INT GetMaxSendCastUnitNum() const { return m_nMaxSendCastUnitNum; }
	INT GetMaxSendCastSize() const { return m_nMaxSendCastSize; }
	DWORD GetMediaOnlineRecordInterval() const { return m_dwMediaOnlineRecordInterval; }
	BOOL IsWardrobeOn() const
	{
		if (m_dwWardrobeOn == 0)
			return FALSE;
		else
			return TRUE;
	}

	BOOL IsKillRoleLogOn() const
	{
		if (m_dwKillRoleLogOn == 0)
			return FALSE;
		else
			return TRUE;
	}

private:
	INT m_nMaxRecvWaitUnitNum;								// �ͻ��˵ȴ�����İ�������������������������ߵ�
	INT m_nMaxSendCastUnitNum;								// ���͸��ͻ��˵���û���յ���Ӧ�İ��������������������ߵ�
	INT m_nMaxSendCastSize;									// ���͸��ͻ��˵���û���յ���Ӧ���ֽ����������������ߵ�
	BOOL m_bEnableExpressPay;								// �Ƿ���ֱ��
	TCHAR m_szExpressPayUrl[256];							// ֱ��URL	
	DWORD m_dwLoginMode;									// ֱ��URL	
	TCHAR m_szTodayHotActivityUrl[256];						// �������ŻURL
	DWORD m_dwAutoFightTimeInterval;						// ս���һ�ս��ʩ�ż��ܺ�ʹ����Ʒʱ����
	DWORD m_dwMediaOnlineRecordInterval;				// ͳ�Ƹ���ý�������������ʱ��������λΪ����
	BOOL m_bGuildWarOpen;									// ����ս����
	DWORD m_dwWardrobeOn;								// �³����ܿ�����־��1������0�ر�
	DWORD m_dwKickPlayerLogOn;							// Kick player Log������־��1������0�ر�
	DWORD m_dwKillRoleLogOn;							// Kill Role��Log�⿪����־��1������0�ر�

public:
    VOID                                AddAccountLoginCount(DWORD dwAccountId,const tagDWORDTime& dwPreLoginTime);
	INT                                 GetAccountLoginCount(DWORD dwAccountId);
	VOID                                SetAccountLoginCount(DWORD dwAccountId,INT  nLoginCount);

#ifdef ON_GAMEGUARD
	// �����汾����CS��ֵ֤�����͸��ͻ���
	VOID								CreateSendCS();
#endif

private:
    TSafeMap<DWORD,INT>                     m_mapAccountLoginCount;        //�ʻ��������

#ifdef ON_GAMEGUARD
	DWORD									   m_dwLastUpdateCStime;		   // ������һ��������ֵ֤�����͸��ͻ��˵�ʱ��
#endif

};

//--------------------------------------------------------------------------------------------
// �õ�ѹ����
//--------------------------------------------------------------------------------------------
inline INT WorldSession::GetSendComRatio()
{
	INT64 nSendSize = m_pNetSession->GetSendSize();
	INT64 nRealSendSize = m_pNetSession->GetRealSendSize();

	if( 0 >= nSendSize ) return 0;

	FLOAT fRatio = FLOAT(nSendSize - nRealSendSize) / FLOAT(nSendSize);

	return INT(fRatio * 100.0f);
}

extern WorldSession g_worldSession;

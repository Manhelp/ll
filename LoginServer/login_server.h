//-----------------------------------------------------------------------------
//!\file login_server.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief LoginServer�������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Mutex.h"

//-----------------------------------------------------------------------------
// LoginServer�������
//-----------------------------------------------------------------------------
class LoginServer
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	LoginServer();
	~LoginServer();

	//-------------------------------------------------------------------------
	// ��ʼ������ѭ��������
	//-------------------------------------------------------------------------
	BOOL			Init(HINSTANCE hInst);
	VOID			Mainloop();
	VOID			Destroy();

	//-------------------------------------------------------------------------
	// �̺߳���
	//-------------------------------------------------------------------------
	VOID			ThreadUpdate();

	//-------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------
	VOID			LockUpdate()		{ m_UpdateMutex.Acquire(); }
	VOID			UnlockUpdate()		{ m_UpdateMutex.Release(); }
	VOID            LockGetTokenID()    { m_Lock.Acquire();}
	VOID            UnlockGetTokenID()  { m_Lock.Release();}

	//-------------------------------------------------------------------------
	// �رշ�����
	//-------------------------------------------------------------------------
	VOID			Shutdown()			{ InterlockedExchange((long*)(&m_bTerminateUpdate), TRUE); }

	//-------------------------------------------------------------------------
	// �õ�ID
	//-------------------------------------------------------------------------
	DWORD			GetSectionID()		{ return m_dwID; }

	//-------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------
	TObjRef<Log>&	GetLog()	{ return m_pLog; }
	TObjRef<Util>&	GetUtil()	{ return m_pUtil; }
	BOOL			IsSimFatigueServer()	{	return m_bSimFatigueServer;		}
	BOOL			IsEnableFatigueGuard()	{	return m_bEnableFatigueGuard;	}
	
	//-------------------------------------------------------------------------
	// ������ϵͳ����
	//-------------------------------------------------------------------------
	DWORD OpenFatigueNetCmd(DWORD);
	DWORD CloseFatigueNetCmd(DWORD);

    DWORD Broadcast(LPCTSTR);

	TCHAR*  GetstrBroadcastMsg(){return strBroadcastMsg;}
	

private:
	//-------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------
	VOID			UpdateMemoryUsage();

private:
	TSFPTrunk<LoginServer>	m_Trunk;
	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindow;
	TObjRef<GUISystem>		m_pGUI;
	IGUIRenderNative*		m_pGUIRender;
	TObjRef<Thread>			m_pThread;
	TObjRef<Console>		m_pConsole;
	TObjRef<Log>			m_pLog;
	TObjRef<Util>			m_pUtil;

	//--------------------------------------------------------------------------
	// ����ID
	//--------------------------------------------------------------------------
	DWORD					m_dwID;

	//--------------------------------------------------------------------------
	// �߳����
	//--------------------------------------------------------------------------
	VOLATILE BOOL			m_bTerminateUpdate;		// �߳�ֹͣ��־
	Mutex					m_UpdateMutex;			// ������
	Mutex                   m_Lock;                 // ָ��YY���TOKENID��
	//--------------------------------------------------------------------------
	// ϵͳ��Ϣ
	//--------------------------------------------------------------------------
	INT						m_nCpuNum;				// CPU ����
	DWORD					m_dwTotalPhys;			// �ܹ������ڴ�
	DWORD					m_dwAvailPhys;			// ���������ڴ�
	DWORD					m_dwTotalRunMinute;		// ���������е�ʱ��

	BOOL					m_bSimFatigueServer;	// ģ������Է�����
	BOOL					m_bEnableFatigueGuard;	// ����������

	//--------------------------------------------------------------------------
	// ͳ����Ϣ
	//--------------------------------------------------------------------------
	volatile DWORD			m_dwIdleTime;			// ����ʱ��
	volatile DWORD			m_dwTick;				// ����������


	TCHAR   strBroadcastMsg[X_LONG_NAME];  
};

extern LoginServer g_login;

#define ILOG	(g_login.GetLog())
#define IUTIL	(g_login.GetUtil())


#ifdef KOREA_LOG

class  SPE_LogException
{	
	TCHAR					    m_szDefaultName[1024];
	tagDWORDTime				m_InitTime;
	TObjRef<Log>				m_pLog;
	void	CreateDir(const TCHAR* Path);
	LPTSTR	CreateLogFileName();

	SPE_LogException();
	SPE_LogException(const SPE_LogException&);
	SPE_LogException& operator=(const SPE_LogException&);
public:
	static SPE_LogException& getInstance();
	BOOL   Init();
	VOID   Update();
	TObjRef<Log>&	GetLog()	{ return m_pLog; }
};

#define SPE_LOG	(SPE_LogException::getInstance().GetLog())
#endif
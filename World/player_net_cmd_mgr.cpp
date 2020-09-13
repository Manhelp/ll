//-----------------------------------------------------------------------------
//!\file player_net_cmd_mgr.h
//!\author Aslan
//!
//!\date 2008-06-13
//! last 2005-06-13
//!
//!\brief �ͻ������������
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "player_net_cmd_mgr.h"
#include "player_session.h"
#include "world.h"

BOOL PlayerNetCmdMgr::m_bNetCmdLog = FALSE;
DWORD PlayerNetCmdMgr::m_dwNetCmdLogTime = 0;

//-----------------------------------------------------------------------------
// construct
//-----------------------------------------------------------------------------
PlayerNetCmdMgr::PlayerNetCmdMgr()
{
	m_dwNetLogTimes = timeGetTime();
	m_mapRecvProc.Clear();
	m_mapSendProc.Clear();
}

//-----------------------------------------------------------------------------
// destruct
//-----------------------------------------------------------------------------
PlayerNetCmdMgr::~PlayerNetCmdMgr()
{
	Destroy();
}

VOID PlayerNetCmdMgr::Init()
{
	TObjRef<VarContainer> pVar = "VarContainer";
	m_bNetCmdLog = (BOOL)pVar->GetDword(_T("on NetCmdLog"));
	m_dwNetCmdLogTime = (BOOL)pVar->GetDword(_T("LogTimeInterval NetCmdLog"));
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID PlayerNetCmdMgr::Destroy()
{
}

//-----------------------------------------------------------------------------
// ��ӡ���е�������Ϣͳ����Ϣ��log
//-----------------------------------------------------------------------------
VOID PlayerNetCmdMgr::LogAllMsg()
{
	tagPlayerCmd* pCmd = NULL;

	m_pLog = "Log";
	m_pUtil = "Util";

	m_pLog->Write(_T("\r\n\r\n"));

	// ���ȴ�ӡ���еĿͻ�����Ϣ
	m_pLog->Write(_T("Client Msg Statistics:\r\n"));

	m_mapRecvProc.ResetIterator();
	while( m_mapRecvProc.PeekNext(pCmd) )
	{
		m_pLog->Write(_T("%s\t\t%u\r\n"), m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pCmd->nTimes);
	}
	m_pLog->Write(_T("\r\n\r\n"));

	// �ٴ�ӡ�������˵���Ϣ
	m_pLog->Write(_T("Server Msg Statistics:\r\n"));

	m_mapSendProc.ResetIterator();
	while( m_mapSendProc.PeekNext(pCmd) )
	{
		m_pLog->Write(_T("%s\t\t%u\r\n"), m_pUtil->Unicode8ToUnicode(pCmd->strCmd.c_str()), pCmd->nTimes);
	}
	m_pLog->Write(_T("\r\n\r\n"));
}

VOID PlayerNetCmdMgr::LogNetAnalyze(INT nFrom)
{
	if (TRUE == m_bNetCmdLog && m_dwRecordTime < timeGetTime())
	{
		LARGE_INTEGER nFreq;
		QueryPerformanceFrequency(&nFreq);
		TMap<DWORD, tagPlayerCmd*>::TMapIterator it = m_mapRecvProc.Begin();
		tagPlayerCmd* pTempCmd;
		while(m_mapRecvProc.PeekNext(it, pTempCmd))
		{
			ILOGNETANALYZE->Write(_T("%d\t%s\t%u\t%u\t%f\t%f\r\n"), nFrom, IUTIL->Unicode8ToUnicode(pTempCmd->strCmd.c_str()),
				pTempCmd->nTimesPer, pTempCmd->nTotalSize, (float)pTempCmd->nTotalTime.QuadPart / (float)nFreq.QuadPart,
				(float)pTempCmd->nMaxTime / (float)nFreq.QuadPart);
			// ��� [6/11/2012 zhangzhihua]
			InterlockedExchange((LPLONG)&pTempCmd->nTimesPer, 0);
			InterlockedExchange((LPLONG)&pTempCmd->nTotalSize, 0);
			InterlockedExchange((LPLONG)&pTempCmd->nMaxTime, 0);
			InterlockedExchange((LPLONG)&pTempCmd->nTotalTime.QuadPart, 0);
		}

		m_dwRecordTime = timeGetTime() + m_dwNetCmdLogTime * 1000;
	}
}

//-----------------------------------------------------------------------------
// ע�������Ϣ
//-----------------------------------------------------------------------------
BOOL PlayerNetCmdMgr::RegisterRecvProc(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize, DWORD dwTimeInterval)
{
	DWORD dwID = m_pUtil->Crc32(szCmd);

	tagPlayerCmd* pCmd = m_mapRecvProc.Peek(dwID);

	if( P_VALID(pCmd) )
	{
		if( pCmd->strCmd != szCmd )
		{
			ASSERT(0);	// ��������ӵ����ͬ��CRC
			return FALSE;
		}
	}
	else
	{
		pCmd = new tagPlayerCmd;
		pCmd->dwTimeInterval = dwTimeInterval;
		pCmd->nTimes = 0;
		pCmd->dwSize = dwSize;
		pCmd->handler = fp;
		pCmd->strCmd = szCmd;
		pCmd->strDesc = szDesc;
		pCmd->nTimesPer = 0;
		pCmd->nTotalSize = 0;
		pCmd->nMaxTime = 0;
		pCmd->nTotalTime.QuadPart = 0;
		m_mapRecvProc.Add(dwID, pCmd);
	}

	return TRUE;
}

//------------------------------------------------------------------------------
// ע�ᷢ����Ϣ
//------------------------------------------------------------------------------
BOOL PlayerNetCmdMgr::RegisterSendProc(LPCSTR szCmd)
{
	DWORD dwID = m_pUtil->Crc32(szCmd);

	tagPlayerCmd* pCmd = m_mapSendProc.Peek(dwID);

	if( P_VALID(pCmd) )
	{
		if( pCmd->strCmd != szCmd )
		{
			ASSERT(0);
			return FALSE;
		}
	}
	else
	{
		pCmd = new tagPlayerCmd;
		pCmd->dwTimeInterval = 0;
		pCmd->nTimes = 0;
		pCmd->dwSize = 0;
		pCmd->handler = NULL;
		pCmd->strCmd = szCmd;
		pCmd->nTimesPer = 0;
		pCmd->nTotalSize = 0;
		pCmd->nMaxTime = 0;
		pCmd->nTotalTime.QuadPart = 0;
		m_mapSendProc.Add(dwID, pCmd);
	}

	return TRUE;
}

//------------------------------------------------------------------------------
// ȡ��ע��
//------------------------------------------------------------------------------
VOID PlayerNetCmdMgr::UnRegisterAll()
{
	tagPlayerCmd* pCmd = NULL;

	m_mapRecvProc.ResetIterator();
	while( m_mapRecvProc.PeekNext(pCmd) )
	{
		SAFE_DEL(pCmd);
	}
	m_mapRecvProc.Clear();

	m_mapSendProc.ResetIterator();
	while( m_mapSendProc.PeekNext(pCmd) )
	{
		SAFE_DEL(pCmd);
	}
	m_mapSendProc.Clear();
}

//------------------------------------------------------------------------------
// �õ�ĳ����ϢID��Ӧ�Ĵ�����
//------------------------------------------------------------------------------
NETMSGHANDLER PlayerNetCmdMgr::GetHandler(tagNetCmd* pMsg, UINT32 nMsgSize, DWORD dwLastTime)
{
	tagPlayerCmd* pCmd = m_mapRecvProc.Peek(pMsg->dwID);
	if( !P_VALID(pCmd) )
	{
		IMSG(_T("Unknow player command recved[<cmdid>%u <size>%d]\r\n"), pMsg->dwID, nMsgSize);
		return NULL;
	}

	if( pMsg->dwSize != nMsgSize || nMsgSize > GT_MAX_PACKAGE_LEN || nMsgSize < pCmd->dwSize )
	{
		IMSG(_T("Invalid net command size[<cmd>%u <size>%d <rec>%d]\r\n"), pMsg->dwID, pMsg->dwSize,nMsgSize);
		return NULL;
	}

	if (P_VALID(dwLastTime) && dwLastTime + pCmd->dwTimeInterval > timeGetTime())
	{
		IMSG(_T("High frequence net command size[<cmd>%u <size>%d <rec>%d]\r\n"), pMsg->dwID, pMsg->dwSize,nMsgSize);
		return NULL;
	}

	InterlockedExchangeAdd((LPLONG)&pCmd->nTimes, 1);
	if (TRUE == m_bNetCmdLog)
	{
		InterlockedExchangeAdd((LPLONG)&pCmd->nTimesPer, 1);
		InterlockedExchangeAdd((LPLONG)&pCmd->nTotalSize, nMsgSize);
	}

	return pCmd->handler;
}

//------------------------------------------------------------------------------------------
// ִ����Ϣ������
//------------------------------------------------------------------------------------------
BOOL PlayerNetCmdMgr::HandleCmd(tagNetCmd* pMsg, DWORD nMsgSize, PlayerSession* pSession)
{
	if( !P_VALID(pSession) ) return FALSE;

	NETMSGHANDLER fp = GetHandler(pMsg, nMsgSize, pSession->GetLastRecvTime(pMsg->dwID));
	if( NULL == fp ) return FALSE;

	//pSession->SetLastRecvTime(pMsg->dwID);
	DWORD dwRet = (pSession->*fp)(pMsg);
	if (E_Success == dwRet)
	{
		pSession->SetLastRecvTime(pMsg->dwID);
	}
	return TRUE;
}

//-------------------------------------------------------------------------------------------
// �������˵ķ�������
//-------------------------------------------------------------------------------------------
VOID PlayerNetCmdMgr::CountServerMsg(DWORD dwMsgID)
{
	tagPlayerCmd* pCmd = m_mapSendProc.Peek(dwMsgID);

	if( P_VALID(pCmd) )
	{
		InterlockedExchangeAdd((LPLONG)&pCmd->nTimes, 1);
	}
}

//-------------------------------------------------------------------------------------------
// ȡ�ý�������ִ�д���
//-------------------------------------------------------------------------------------------
UINT32 PlayerNetCmdMgr::GetRecvCmdRunTimes( DWORD dwMsgID )
{
	tagPlayerCmd* pCmd = m_mapRecvProc.Peek(dwMsgID);

	if (P_VALID(pCmd))
	{
		return pCmd->nTimes;
	}

	return GT_INVALID;
}

VOID PlayerNetCmdMgr::AddCmdRunTime(DWORD dwMsgID, LARGE_INTEGER nTime)
{
	if (TRUE == m_bNetCmdLog)
	{
		tagPlayerCmd* pCmd = m_mapRecvProc.Peek(dwMsgID);

		if (P_VALID(pCmd))
		{
			LARGE_INTEGER nCmdEndTime;
			QueryPerformanceCounter(&nCmdEndTime);

			UINT32 uInterval = (UINT32)(nCmdEndTime.QuadPart - nTime.QuadPart);
			if (uInterval > pCmd->nMaxTime)
			{
				InterlockedExchange((LPLONG)&pCmd->nMaxTime, nCmdEndTime.QuadPart - nTime.QuadPart);
			}
			InterlockedExchangeAdd((LPLONG)&pCmd->nTotalTime.QuadPart, nCmdEndTime.QuadPart - nTime.QuadPart);
		}
	}
}


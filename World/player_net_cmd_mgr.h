//-----------------------------------------------------------------------------
//!\file player_net_cmd_mgr.h
//!\author Aslan
//!
//!\date 2008-06-13
//! last 2008-06-13
//!
//!\brief �ͻ����������������Ҫ���NetCmdMgr�޷���Զ������Ϣ�����ȱ��
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

class PlayerSession;

typedef DWORD (PlayerSession::*NETMSGHANDLER)(tagNetCmd*);

//-----------------------------------------------------------------------------
// �ͻ������������
//-----------------------------------------------------------------------------
class PlayerNetCmdMgr
{
public:
	PlayerNetCmdMgr();
	~PlayerNetCmdMgr();

	static VOID Init();
	VOID Destroy();

	VOID LogAllMsg();

	VOID LogNetAnalyze(INT nFrom);

	BOOL RegisterRecvProc(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize, DWORD dwTimeInterval = 0);
	BOOL RegisterSendProc(LPCSTR szCmd);
	VOID UnRegisterAll();

	NETMSGHANDLER GetHandler(tagNetCmd* pMsg, UINT32 nMsgSize, DWORD dwLastTime);
	VOID CountServerMsg(DWORD dwMsgID);

	BOOL HandleCmd(tagNetCmd* pMsg, DWORD nMsgSize, PlayerSession* pSession);

	// ȡ����Ϣִ�д���
	UINT32 GetRecvCmdRunTimes(DWORD dwMsgID);

	VOID AddCmdRunTime(DWORD dwMsgID, LARGE_INTEGER nTime);

protected:
	typedef struct tagPlayerCmd
	{
		std::string				strCmd;			// ������
		tstring					strDesc;		// ����
		DWORD					dwSize;			// ��Ϣ��С
		NETMSGHANDLER			handler;		// ����ָ��
		DWORD					dwTimeInterval;	// ����ִ��ʱ���� [7/4/2012 zhangzhihua]
		volatile UINT32			nTimes;			// ������Ĵ���
		volatile UINT32			nTimesPer;		// ������Ĵ���
		volatile UINT32			nTotalSize;		// ������Ĵ�С��
		volatile UINT32			nMaxTime;		// ������ĵ���ִ��ʱ�����ֵ
		volatile LARGE_INTEGER	nTotalTime;		// ������ʱ����ڵ�ִ��ʱ���
	} tagPlayerCmd;

	TObjRef<Util>				m_pUtil;
	TObjRef<Log>				m_pLog;

	TMap<DWORD, tagPlayerCmd*>	m_mapRecvProc;	// ������Ϣ�Ĵ���ͳ��
	TMap<DWORD,	tagPlayerCmd*>	m_mapSendProc;	// ������Ϣ�Ĵ���ͳ��

private:
	// �������¼��� [6/11/2012 zhangzhihua]
	static BOOL					m_bNetCmdLog;	// �Ƿ���log
	static DWORD				m_dwNetCmdLogTime;// ��־��¼ʱ����
	DWORD						m_dwNetLogTimes;// ��־��¼����
	DWORD						m_dwRecordTime;	// ��¼ʱ��
};
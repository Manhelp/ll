//-----------------------------------------------------------------------------
//!\file shutdown_server.h
//!\author Jason
//!
//!\date 2010-10-8
//! last 
//!
//!\brief �Ѻùط�������
//!
//!	Copyright (c) 2004 DACHENG Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#ifndef shutdown_server_h__
#define shutdown_server_h__

class Role ;
/************************************************************************/
/* ÿ���ӹ㲥���ͻ��ˣ��ط�����ʱtick�����һ����ÿ5s�㲥һ��           */
/* ���һ���ӣ��㲥ʱ��Ҫ������                                         */
/************************************************************************/
class FriendlyShutdownServer
{
	volatile LONG		m_nShutdownTick;			// �ط�����ʱtick
	LONG		m_nLastBroadcastTick;		// �ϴι㲥tick
public:
	FriendlyShutdownServer();
	~FriendlyShutdownServer();

	VOID ShutDownTick(LONG nTicks){ ::InterlockedExchange( (LPLONG)&m_nShutdownTick , nTicks );m_nLastBroadcastTick=GT_INVALID; }
	LONG	ShutDownTick(VOID)	const { return m_nShutdownTick; }

	// ÿtick��world::threadupdate������
	VOID	Update(VOID);
	BOOL	CanEnterGame() const;

	VOID	NotifyNewJoinGamePly(Role * ply);

private:
	VOID	TryBroadcastShutdownTimes(VOID);
	VOID	BroadcastShutdownTimes(DWORD dwBroadcastID,INT nSeconds);
};

extern FriendlyShutdownServer g_FriendlyShutdownServer;

#endif // shutdown_server_h__



//-----------------------------------------------------------------------------
//!\file loongworld_mgr.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief �����������ӽ���LoongWorld,ӵ��StreamServer,������Ϣ������LoongWorld���͵İ�
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Mutex.h"
#include "Singleton.h"
#include <hash_map>


class Player;
class LoongWorld;

class LoongWorldMgr: public Singleton<LoongWorldMgr>
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
    LoongWorldMgr();
    ~LoongWorldMgr();

	//-------------------------------------------------------------------------
	// ��ʼ�������º�����
	//-------------------------------------------------------------------------
    BOOL		Init();
    VOID		Update();
    VOID		Destroy();
	VOID		UpdateWorldState(LoongWorld* pLoongWorld);
	//-------------------------------------------------------------------------
	// ������Ϣ���
	//-------------------------------------------------------------------------
	LPBYTE		RecvMsg(DWORD dwID, DWORD& dwMsgSize, INT& nUnRecved)	{ return m_pSession->Recv(dwID, dwMsgSize, nUnRecved); }
	VOID		ReturnMsg(DWORD dwID, LPBYTE pMsg)						{ m_pSession->FreeRecved(dwID, pMsg); }
	VOID		SendMsg(DWORD dwID, LPVOID pMsg, DWORD dwSize)			{ m_pSession->Send(dwID, pMsg, dwSize); }
	VOID		HandleCmd(LPVOID pMsg, DWORD dwSize, DWORD dwParam)		{ m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pMsg, dwSize, dwParam); }

	//-------------------------------------------------------------------------
	// �õ���Ϸ����
	//-------------------------------------------------------------------------
    LoongWorld* GetLoongWorld(DWORD dwNameCrc)	{ return m_mapLoongWorld.Peek(dwNameCrc); }
	DWORD		GetLoongWorldNum()				{ return m_mapLoongWorld.Size();		}
	//-----------------------------------------------------------------------
	// ������
	//-----------------------------------------------------------------------
	VOID		AddToWorld(Player* pPlayer, DWORD dwWorldNameCrc);

private:
	//-----------------------------------------------------------------------
	// ������Ϣ���
	//-----------------------------------------------------------------------
	VOID		RegisterLoongWorldMsg();
	VOID		UnRegisterLoongWorldMsg();

	//-----------------------------------------------------------------------
	// ����ǳ��ص�
	//-----------------------------------------------------------------------
	DWORD		LoginCallBack(LPBYTE pByte, DWORD dwSize);
	DWORD		LogoutCallBack(DWORD dwParam);

	//-----------------------------------------------------------------------
	// ��Ϣ����
	//-----------------------------------------------------------------------
	DWORD		HandleCreateGuild(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleCreateFamily(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleCreateRole(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleCertification(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleZoneServerStatus(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandlePlayerLogin(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandlePlayerLogout(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandlePlayerWillLogin(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleKickLog(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleForbidAccount(tagNetCmd* pMsg, LoongWorld* pWorld);
	DWORD		HandleWorldClosed(tagNetCmd* pMsg, LoongWorld* pWorld);
	// Jason 2010-4-27 v2.1.0
	DWORD		HandlePlayerPKRunaway(tagNetCmd*pMsg,LoongWorld*pWord);
	DWORD		HandleFatigueTimer(tagNetCmd*pMsg,LoongWorld*pWord);

	// Jason 2010-11-24 v1.2.2
	DWORD		HandleGetNetbarInfo(tagNetCmd * pMsg, LoongWorld * pWorld);

	// ��������ֻ���
	DWORD		HandleSaveTelNum(tagNetCmd * pMsg, LoongWorld * pWorld);

	// ��Ҳ��洢�ֻ���
	DWORD		HandleNotSaveTelNum(tagNetCmd * pMsg, LoongWorld * pWorld);
	
private:
	TSFPTrunk<LoongWorldMgr>		m_Trunk;
    TObjRef<Util>					m_pUtil;

	TObjRef<StreamServer>			m_pSession;					// ����ײ�
	TObjRef<NetCmdMgr>				m_pMsgCmdMgr;				// ������Ϣ������

    TMap<DWORD, LoongWorld*>		m_mapLoongWorld;			// ��Ϸ�����б�

	DWORD							m_dwLoongWorldGoldenCode;	// LoongWorld�Ľ�ɫ����
	DWORD							m_dwLoginServerGoldenCode;	// LoginServer�Ľ�ɫ����
	// Jason 2010-4-27 v2.1.0��û������Ϸ������أ�����ȫ���ģ��Ƚ��ϸ��
	stdext::hash_map<DWORD, DWORD>		m_hmapPlyPKPunishmentState;	// ����pk�ͷ�״̬������б�
private:
	VOID UpdatePKPunishmentMap(VOID);
public:
	BOOL	IsInPKPunishmentState(DWORD dwAccountID);
};

#define sLoongWorldMgr LoongWorldMgr::getSingleton()



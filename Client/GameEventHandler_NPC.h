#pragma once
#include "CombatEvent.h"

class FSM_NPC;
/** \class GameEventHandler_NPC
	\brief NPC��Ϸ�¼�������
*/
class GameEventHandler_NPC
{
public:
	GameEventHandler_NPC(void);
	virtual ~GameEventHandler_NPC(void);
	void SetFSM(FSM_NPC* pFSM){m_pFSM=pFSM;}

	virtual void OnGameEvent(tagGameEvent* pEvent) =0;
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;

protected:	
	// add by hpy
	TObjRef<Util>				m_pUtil;
	FSM_NPC*					m_pFSM;
};





/** \class HitTargetEventHandler_NPC
	\brief NPC����Ŀ����Ϸ�¼�������
*/
class HitTargetEventHandler_NPC : public GameEventHandler_NPC
{
public:
	HitTargetEventHandler_NPC(void);
	virtual ~HitTargetEventHandler_NPC(void);

	//--GameEventHandler_NPC
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

	//--
	tagHitTargetEvent* GetLastHitTargetEvent(DWORD roleID);
protected:
	map<DWORD,tagHitTargetEvent> m_eventMap;
};

/** \class EffectHPChangeEventHandler_NPC
	\brief NPC���б�����Ч��Ѫ�¼�������
*/
class EffectHPChangeEventHandler_NPC : public GameEventHandler_NPC
{
public:
	EffectHPChangeEventHandler_NPC(void);
	virtual ~EffectHPChangeEventHandler_NPC(void);

	//--GameEventHandler_LP
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

	//--
	tagEffectHPChangeEvent* GetLastEffectHPChangeEvent(DWORD roleID);
protected:
	map<DWORD,tagEffectHPChangeEvent> m_eventMap;
};


/** \class SkipFrameEventHandler_NPC
\brief ����NPC��������Ϸ�¼�,��֡
*/
class SkipFrameEventHandler_NPC : public GameEventHandler_NPC
{
	struct tagData
	{
		DWORD recvTime;
		DWORD maxChannelIndex;
		tagNS_HitTarget cmd;
	};

public:
	SkipFrameEventHandler_NPC( void );
	virtual ~SkipFrameEventHandler_NPC( void );

	virtual void OnGameEvent( tagGameEvent* pEvent );
	virtual void OnNetCmd( tagNetCmd* pNetCmd );
	virtual void Update();

protected:
	void CacheCmd( tagNS_HitTarget* pCmd );
	bool IfNeedClearCmd( tagNS_HitTarget* pCacheCmd, tagHitTargetEvent* pEvent );
	bool IfNeedHandleCmd( tagNS_HitTarget* pCacheCmd, tagHitTargetEvent* pEvent );

	/* ���汻�����¼�����Ҫ��Ϊ������Ⱥ������ʱ��֡����
	*/
	list<tagData> m_cache;

};
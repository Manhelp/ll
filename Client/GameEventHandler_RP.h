#pragma once
#include "CombatEvent.h"

class FSM_RP;
/** \class GameEventHandler_RP
	\brief Զ�������Ϸ�¼�������
*/
class GameEventHandler_RP
{
public:
	GameEventHandler_RP(void);
	virtual ~GameEventHandler_RP(void);
	void SetFSM(FSM_RP* pFSM){m_pFSM=pFSM;}

	virtual void OnGameEvent(tagGameEvent* pEvent) =0;
	virtual void OnNetCmd(tagNetCmd* pNetCmd) =0;

protected:
	FSM_RP*					m_pFSM;
};





/** \class HitTargetEventHandler_RP
	\brief Զ���������Ŀ����Ϸ�¼�������
*/
class HitTargetEventHandler_RP : public GameEventHandler_RP
{
public:
	HitTargetEventHandler_RP(void);
	virtual ~HitTargetEventHandler_RP(void);

	//--GameEventHandler_RP
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

	//--
	tagHitTargetEvent* GetLastHitTargetEvent(DWORD roleID);
protected:
	map<DWORD,tagHitTargetEvent> m_eventMap;
};

/** \class EffectHPChangeEventHandler_RP
	\brief Զ��������б�����Ч��Ѫ�¼�������
*/
class EffectHPChangeEventHandler_RP : public GameEventHandler_RP
{
public:
	EffectHPChangeEventHandler_RP(void);
	virtual ~EffectHPChangeEventHandler_RP(void);

	//--GameEventHandler_LP
	virtual void OnGameEvent(tagGameEvent* pEvent);
	virtual void OnNetCmd(tagNetCmd* pNetCmd);

	//--
	tagEffectHPChangeEvent* GetLastEffectHPChangeEvent(DWORD roleID);
protected:
	map<DWORD,tagEffectHPChangeEvent> m_eventMap;
};
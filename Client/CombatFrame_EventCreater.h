#pragma once
#include "gameframe.h"

struct tagRolePickEvent;
struct tagScenePickEvent;
struct tagGroundItemPickEvent;
struct tagMouseMoveEvent;
struct tagRoleRClickEvent;
class Role;

/** \class CombatFrame_EventCreater
	\brief ս�������Ϸ�¼�������
*/

// 6�����
const INT SelectTargetClearTime = 6000;

class CombatFrame_EventCreater :
	public GameFrame
{
public:
	CombatFrame_EventCreater(void);
	virtual ~CombatFrame_EventCreater(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();

	//--
	bool IsSelectEnemy(){ return m_bSelectEnemy;}
	DWORD GetCurTargetID(){ return m_curTargetRoleID;}

private:
	DWORD OnKey(DWORD dwID, BOOL bDown);
	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
	DWORD OnRoleRClickEvent(tagRoleRClickEvent* pEvent );
	DWORD OnScenePickEvent(tagScenePickEvent* pEvent);
	DWORD OnGroundItemPickEvent(tagGroundItemPickEvent* pEvent);
	DWORD OnMouseMoveEvent(tagMouseMoveEvent* pEvent);
	bool  CanSwitchTargetRole(Role* pRole,const Frustum* pFrustum);

	// TAB�������
	VOID  ClearAttackingRoleMap();
	// �Զ���չ������б�
	VOID  UpdateAttackerMap();
public:
	// ����µĹ������ٹ�����map�У���ӳɹ�����true
	BOOL  AddNewAttacker(DWORD dwSrcRoleID,DWORD dwTarRoleID);
	BOOL  DeleteNewAttacker(DWORD dwSrcRoleID);
	DWORD GetNextTarget();

private:
	TSFPTrunk<CombatFrame_EventCreater>	m_trunk;
	TObjRef<GameInputMap>	m_pKeyMap;

	DWORD					m_curTargetRoleID;
	bool					m_bSelectEnemy;

	BOOL					m_bFrontKeyDown;
	BOOL					m_bBackKeyDown;
	BOOL					m_bLeftKeyDown;
	BOOL					m_bRightKeyDown;

	map<DWORD,Role*>		m_mapAttackingRoleMap;  //!< �洢Ŀǰ���ڹ���������ҵ�NPC����RL
	DWORD					m_dwAutoClearTimeMark;

	DWORD					m_dwLastTabTime;
};

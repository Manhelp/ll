#include "StdAfx.h"
#include "LowerStateMouseWaterWalk_LP.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "WorldPickerFrame.h"
#include "PreState.h"
#include "CombatSysUtil.h"
#include "MapMgr.h"

LowerStateMouseWaterWalk_LP::LowerStateMouseWaterWalk_LP(void) :
m_finishedOnExit(false)
{}

LowerStateMouseWaterWalk_LP::~LowerStateMouseWaterWalk_LP(void)
{}

void LowerStateMouseWaterWalk_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole,tagPreState* pPreState )
{
	ASSERT(pEvent->strEventName==_T("tagMouseMoveEvent"));
	tagMouseMoveEvent* pMoveEvent=(tagMouseMoveEvent*)pEvent;

	//--保存event
	m_event=*pMoveEvent;
	m_finishedOnExit=true;
	//--
	m_actMouseWaterWalk.Active(pMoveEvent->start,pMoveEvent->end,pMoveEvent->validDist,pNav,pRole);
}

void LowerStateMouseWaterWalk_LP::Update(NavMap* pNav,LocalPlayer* pRole)
{
	m_actMouseWaterWalk.Update(pNav,pRole);
	if(m_actMouseWaterWalk.IsClosed())
	{
		if(m_actMouseWaterWalk.GetResult()==ActionMouseWaterWalk_LP::ER_Land)
		{
			tagMouseMoveEvent event;
			event.start=pRole->GetPos();
			event.end=m_event.end;
			event.bUserCtrl=false;

			m_finishedOnExit=false;
			m_pFSM->Change2LowerState(FSM_LP::ELowerState_MouseMove,&event);
		}
		else if(m_actMouseWaterWalk.GetResult()==ActionMouseWaterWalk_LP::ER_Swim)
		{
			tagMouseMoveEvent event;
			event.start=pRole->GetPos();
			event.end=m_event.end;
			event.bUserCtrl=false;

			m_finishedOnExit=false;
			m_pFSM->Change2LowerState(FSM_LP::ELowerState_MouseSwim,&event);
		}
		else
		{
			m_pFSM->Change2LowerState_WaterStand();
		}
	}
}

void LowerStateMouseWaterWalk_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))//键盘移动游戏事件
	{
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardWaterWalk,pEvent);
	}
	else if(pEvent->strEventName==_T("tagMouseMoveEvent"))//鼠标移动事件
	{
		tagMouseMoveEvent* pMouseMoveEvent=(tagMouseMoveEvent*)pEvent;
		m_finishedOnExit=pMouseMoveEvent->bUserCtrl;
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseWaterWalk,pEvent);
	}
	else if(pEvent->strEventName==_T("tagJumpEvent"))//跳跃事件
	{
		LocalPlayer* pSelf=m_pFSM->GetLocalPlayer();

		tagJumpEvent* pJumpEvent=(tagJumpEvent*)pEvent;
		pJumpEvent->start=pSelf->GetPos();
		Yaw2Dir(pSelf->GetYaw(),pJumpEvent->dir);

		tagPreState prestate;
		prestate.state=tagPreState::EState_MouseMove;
		prestate.mouseMove=m_event;

		m_finishedOnExit=false;
		m_pFSM->Change2LowerState(FSM_LP::ELowerState_Jump,pEvent,&prestate);
	}
	else if(pEvent->strEventName==_T("tagLPRoleStateChangeEvent"))
	{
		LocalPlayer* pSelf=m_pFSM->GetLocalPlayer();
		if(!pSelf->CanWaterWalk())//水上行走效果消除
		{
			CombatSysUtil::Inst()->PullLocalPlayer();

			tagMouseMoveEvent evt = m_event;
			evt.start=pSelf->GetPos();

			m_finishedOnExit=false;
			if(MapMgr::Inst()->GetNavMap()->GetCollider()->IfWillSwim(pSelf->GetPos(),pSelf->GetRoleSize().y))
				m_pFSM->Change2LowerState(FSM_LP::ELowerState_MouseSwim,&evt);
			else
				m_pFSM->Change2LowerState(FSM_LP::ELowerState_MouseMove,&evt);
		}
	}
}

void LowerStateMouseWaterWalk_LP::ShowDecal( bool bShow )
{
	tagShowScenePickDecalEvent event(_T("tagShowScenePickDecalEvent"),NULL);
	event.bShow=bShow;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

void LowerStateMouseWaterWalk_LP::SendFinishEvent( bool bSuccessed )
{
	tagMouseMoveFinishEvent event;
	event.bSuccessed=bSuccessed;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

void LowerStateMouseWaterWalk_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_MoveSpeedChange"))//速度变化消息
	{
		tagMouseMoveEvent event=m_event;
		event.start=m_pFSM->GetLocalPlayer()->GetPos();
		m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseWaterWalk,&event);
	}
	//--瞬移消息
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_SpecialMove"))
	{
		tagNS_SpecialMove* pCmd=(tagNS_SpecialMove*)pNetCmd;
		if( ESMT_Teleport == pCmd->eType )
		{
			tagMouseMoveEvent event=m_event;
			event.start=pCmd->vDestPos;
			event.bUserCtrl=false;
			OnGameEvent(&event);
		}
	}
}

void LowerStateMouseWaterWalk_LP::OnExit()
{
	if(m_finishedOnExit)
	{
		ShowDecal(false);
		SendFinishEvent(m_actMouseWaterWalk.GetResult()==ActionMouseWaterWalk_LP::ER_Arrive);
	}
}

bool LowerStateMouseWaterWalk_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	switch(state)
	{
	case FSM_LP::EUpperState_Idle:		//空闲
	case FSM_LP::EUpperState_Dead:		//死亡
	case FSM_LP::EUpperState_Dazzy:		//击晕
	case FSM_LP::EUpperState_Tie:		//定身
	case FSM_LP::EUpperState_Sopor:		//昏睡
	case FSM_LP::EUpperState_Follow:	//跟随
	case FSM_LP::EUpperState_PickupItem://拾取
	case FSM_LP::EUpperState_TalkNPC:	//与NPC对话
	case FSM_LP::EUpperState_UseItem:	//使用物品
		{
			return true;
		}
		break;
	}

	return false;
}

bool LowerStateMouseWaterWalk_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return true;
}
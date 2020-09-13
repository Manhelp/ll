//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: character_handler.cpp
// author: lh
// actor:
// data: 2008-08-31
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- �������
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/ScriptMsgInfo.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "quest.h"
#include "quest_mgr.h"
#include "script_mgr.h"
#include "creature.h"
#include "activity_mgr.h"
#include "quest_board.h"
#include "shielding_mgr.h"

//------------------------------------------------------------------------------------
// ��ȡ����
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleNPCAcceptQuest(tagNetCmd* pCmd)
{
	if (P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_NPCAcceptQuest* pRecv = (tagNC_NPCAcceptQuest*)pCmd;
	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	if( IsShieldingTask(pRecv->u16QuestID) )
	{
		tagNS_AcceptQuest send;
		send.u16QuestID = pRecv->u16QuestID;
		send.dwErrorCode = E_QUEST_NOT_EXIST;
		SendMessage(&send, send.dwSize);
		return 0;
	}

	INT nRet = pRole->AcceptQuestFromNPC(pRecv->u16QuestID, pRecv->dwNPCID);

	tagNS_AcceptQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------------------
// ͨ��������������
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleTriggerAcceptQuest(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_TriggerAcceptQuest* pRecv = (tagNC_TriggerAcceptQuest*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = pRole->AcceptQuestFromMapTrigger(pRecv->u16QuestID, pRecv->dwTriggerID);

	tagNS_AcceptQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------------
// �Ƿ������ɴ�����
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCompleteQuest(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_CompleteQuest* pRecv = (tagNC_CompleteQuest*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	UINT16 u16NexitQuestID = 0;

	INT nRet = E_Success;
	if(!IsBoardQuest(pRecv->u16QuestID) && !IsYellowListQuest(pRecv->u16QuestID))
	{
		nRet = pRole->CompleteQuest(pRecv->u16QuestID, pRecv->dwNPCID, pRecv->nRewChoicesItemIndex, pRecv->eType, u16NexitQuestID);
	}
	else
	{
		nRet = pRole->CompleteQuest(pRecv->u16QuestID, GT_INVALID, 0, EQCT_Normal, u16NexitQuestID);
	}
		
	// ���ͷ���
	tagNS_CompleteQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	if (nRet == E_Success && (IsBoardQuest(pRecv->u16QuestID) || IsYellowListQuest(pRecv->u16QuestID)))
	{
		EQuestBoardType eQuestBoardType = pRole->GetQuestBoardType(pRecv->u16QuestID);

		// �����������ɺ���ˢ�������
		pRole->GetBoardQuestMgr(eQuestBoardType)->Del(pRecv->u16QuestID);	// ɾ������
		pRole->GetBoardQuestMgr(eQuestBoardType)->IncreaseCompleteRefreshTime();
		pRole->GetBoardQuestMgr(eQuestBoardType)->RefreshBoard(pRole, EBRT_CompleteQuest);
	}

	// ����Ƿ��к����Զ���ȡ������
	if(E_Success == nRet && u16NexitQuestID != 0)
		pRole->AcceptQuestFromNPC(u16NexitQuestID, GT_INVALID);

	return 0;
}

DWORD	PlayerSession::HandleGetSingleQuestDoneInfo(tagNetCmd* pCmd)
{
	tagNC_GetSingleQuestDoneInfo* pRecv = (tagNC_GetSingleQuestDoneInfo*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	if ( 0 == pRecv->u16QuestID || !GT_VALID(pRecv->u16QuestID))
	{
		return GT_INVALID;
	}
    
	tagNS_GetSingleQuestDoneInfo msg;
    DWORD dwNum = pRole->GetQuestCompleteTimes(pRecv->u16QuestID);
	if ( !GT_VALID(dwNum) )
	{
		dwNum = 0;
	}
	msg.nTimes     =  dwNum;
	msg.u16QuestID =  pRecv->u16QuestID;
	SendMessage(&msg,msg.dwSize);
	return 0;
}

//------------------------------------------------------------------------------------
// �Ƿ����ɾ��������
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDeleteQuest(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_DeleteQuest* pRecv = (tagNC_DeleteQuest*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT nRet = pRole->DeleteQuest(pRecv->u16QuestID);

	// ���ͷ�����Ϣ
	tagNS_DeleteQuest send;
	send.u16QuestID = pRecv->u16QuestID;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

	if ( IsBoardQuest(pRecv->u16QuestID) || IsYellowListQuest(pRecv->u16QuestID))
	{
		EQuestBoardType eQuestBoardType = pRole->GetQuestBoardType(pRecv->u16QuestID);

		// �ı�����������״̬
		pRole->GetBoardQuestMgr(eQuestBoardType)->ChangeQuestState(pRecv->u16QuestID, EQS_NotAccept);
	}

	return 0;
}

//------------------------------------------------------------------------------------
// ����NPC�Ի�����״̬
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUpdateQuestNPCTalk(tagNetCmd* pCmd)
{
	if(P_VALID(m_pRole) && GetFatigueGuarder().GetEarnRate() < 10000)
	{
		GetRole()->SendFatigueGuardInfo(E_FatigueLimit_Quest);
		return 0;
	}

	tagNC_UpdateQuestNPCTalk* pRecv = (tagNC_UpdateQuestNPCTalk*)pCmd;

	// ���Role�Ƿ�Ϸ�
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	pRole->UpdateQuestNPCTalk(pRecv->u16QuestID, pRecv->dwNPCID);

	return 0;
}

//-----------------------------------------------------------------------------
// �ͻ��˷�������ĶԻ���ȱʡ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDlgDefaultMsg(tagNetCmd* pCmd)
{
	DWORD dwStart = timeGetTime();
	MGET_MSG(pRecv, pCmd, NC_DlgDefaultMsg);

	Role* pRole = GetRole();

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	switch (pRecv->eDlgTarget)
	{
	case EMUT_DlgTarget_Quest:
		break;
	case EMUT_DlgTarget_Item:
		break;
	case EMUT_DlgTarget_Creature:
		{
			DWORD			dwNpcID		= pRecv->dwTargetID;
			EDlgOption		eOption		= pRecv->eDlgOption;
			MTRANS_ELSE_RET(pMap,	pRole->GetMap(),				Map,			GT_INVALID);
			MTRANS_ELSE_RET(pNpc,	pMap->FindCreature(dwNpcID),	Creature,		GT_INVALID);
			MTRANS_ELSE_RET(pScript,pNpc->GetScript(),				CreatureScript,	GT_INVALID);
			pScript->OnTalk(pNpc, pRole, (INT)eOption);
		}
		break;
	default:
		break;
	}

	DWORD dwEnd = timeGetTime();
	if (dwEnd - dwStart > 200)
	{
		ILOG->Write(_T("NC_DlgDefaultMsg too long:%d eDlgTarget:%d dwTargetID:%d eDlgOption:%d\r\n"),
			(dwEnd - dwStart), pRecv->eDlgTarget, pRecv->dwTargetID, pRecv->eDlgOption);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// �ͻ��˴����������ű���ȱʡ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDefaultRequest(tagNetCmd* pCmd)
{
	DWORD dwStart = timeGetTime();
	MGET_MSG(pRecv, pCmd, NC_DefaultRequest);

	Role* pRole = GetRole();

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	switch (pRecv->eDlgTarget)
	{
		case EMUT_Request_Activity:
			{
				CActivity* pActivity = g_activityMgr.GetActivity(pRecv->dwTargetID);
				if( !P_VALID(pActivity) ) return GT_INVALID;

				const ActScript* pScript = pActivity->GetScript();
				if( !P_VALID(pActivity) ) return GT_INVALID;

				pScript->OnDefaultRequest(pRecv->dwTargetID, pRole, pRecv->dwEventType);
			}
			break;

		default:
			break;
	}

	DWORD dwEnd = timeGetTime();
	if (dwEnd - dwStart > 200)
	{
		ILOG->Write(_T("NC_DefaultRequest too long:%d eDlgTarget:%d dwTargetID:%d dwEventType:%d\r\n"),
					(dwEnd - dwStart), pRecv->eDlgTarget, pRecv->dwTargetID, pRecv->dwEventType);
	}
	return 0;
}
DWORD PlayerSession::HandlePullRoleMsgCallback(tagNetCmd*pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PullRoleMsgCallback);

	Role* pRole = GetRole();

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	switch( pRecv->dwAppID )
	{
	case 0:	// ����
		{
			const QuestScript * pScript = g_ScriptMgr.GetQuestScript(pRecv->dwActID);
			if( P_VALID(pScript) ) 
			{
				//const tagMsgDeliver * pDeliver = g_attRes.GetOneMsgDeliverProto();
				pScript->OnClientCallback(pRecv->dwIdMsgID,pRole,pRecv->param1,pRecv->param2,pRecv->param3,pRecv->param4);
			}
		}
		break;
	case 1:	// ����
		{
			const ActScript * pScript = g_ScriptMgr.GetActScript(pRecv->dwActID);
			if( P_VALID(pScript) ) 
			{
				pScript->OnClientCallback(pRecv->dwIdMsgID,pRole,pRecv->param1,pRecv->param2,pRecv->param3,pRecv->param4);
			}
		}
		break;
	}
	
	return E_Success;
}



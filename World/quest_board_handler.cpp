#include "stdafx.h"
#include "role.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/msg_quest_board.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "creature.h"

DWORD PlayerSession::HandleOpenQuestBoard(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_OpenQuestBoard);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	TMap<UINT16, tagBoardQuestProto*>& mapAllQuest = g_attRes.GetBoardQuestMap();
	EQuestBoardType eQuestBoardType = EQBT_Null;


	if(0 == pRecv->dwNPCID && pRecv->dwNPCTypeID > 0 )
	{//���ǵ��npc�����Ͱ�
		const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(pRecv->dwNPCTypeID);
		if(P_VALID(pCreatureProto))
		{
			if( EFNPCT_QuestPanelReward == pCreatureProto->eFunctionType )
				eQuestBoardType = EQBT_Reward;
			else
				return GT_INVALID;
		}
		else
		{
			return GT_INVALID;
		}
	}
	else
	{
		// �ж�npcְ��
		eQuestBoardType = GetQuestBoardType(pRecv->dwNPCID);
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_QuestPanelReward);
		if (dwErrorCode != E_Success)
		{
			dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_QuestPanelYellowList);
			if (dwErrorCode != E_Success)
			{
				tagNS_OpenQuestBoard msg;
				msg.eQuestBoardType = eQuestBoardType;
				msg.dwErrorCode = dwErrorCode;
				pRole->SendMessage(&msg, msg.dwSize);
				return dwErrorCode;
			}
		}
	}

	

	// �жϽ�ɫ�ȼ����ȼ�С��5���ܻ������
	if ( pRole->GetLevel()<5 )
	{
		dwErrorCode = E_QuestBoard_Level_Limit;
		tagNS_OpenQuestBoard msg;
		msg.dwErrorCode = dwErrorCode;
		msg.eQuestBoardType = eQuestBoardType;
		pRole->SendMessage(&msg, msg.dwSize);

		return dwErrorCode;
	}

	if (!P_VALID(pRole->GetBoardQuestMgr(eQuestBoardType)))
		return GT_INVALID;

	// ������δ����������񣬿�ʼ��
	if (pRole->GetBoardQuestMgr(eQuestBoardType)->GetRoleBoardQuest().Empty()) 
	{
		INT nMaxCount = pRole->GetBoardQuestMgr(eQuestBoardType)->GetQuestMaxCnt(pRole);

		UINT16 n16QuestPanelStartTaskID = 0;
		EBoardQuestQuality eBoardQuestQuality = EBQQ_White;
		if (eQuestBoardType == EQBT_Reward)
		{
			n16QuestPanelStartTaskID = QUEST_PANEL_START_TASK_ID;
			eBoardQuestQuality = EBQQ_White;
		}
		else if (eQuestBoardType == EQBT_YellowList)
		{
			n16QuestPanelStartTaskID = YELLOW_LIST_START_TASK_ID;
			eBoardQuestQuality = EBQQ_Blue;
		}

		pRole->GetBoardQuestMgr(eQuestBoardType)->RefreshBoardByQuestColor(pRole, pRole->GetLevel(), eBoardQuestQuality, nMaxCount, n16QuestPanelStartTaskID);

		// ��ˢ�º����������ݱ������ݿ�
		pRole->GetBoardQuestMgr(eQuestBoardType)->SaveBoardQuest2DB();
	}
	/* ��������ȼ��Ѿ�ԶС�ڽ�ɫ�ȼ�(����3������)����ˢ������� */
	else if (pRole->GetBoardQuestMgr(eQuestBoardType)->GetQuestBoardLevel() - pRole->GetLevel() > 3 || pRole->GetLevel() - pRole->GetBoardQuestMgr(eQuestBoardType)->GetQuestBoardLevel() > 3)
	{
		pRole->GetBoardQuestMgr(eQuestBoardType)->RefreshBoard(pRole, EBRT_Level);
	}
	
	// ������
	if(pRole->GetBoardQuestMgr(eQuestBoardType)->GetRoleBoardQuest().Size() > QUEST_BOARD_MAX_CAPACITY)
	{
		ASSERT(pRole->GetBoardQuestMgr(eQuestBoardType)->GetRoleBoardQuest().Size() <= QUEST_BOARD_MAX_CAPACITY);
		return GT_INVALID;
	}

	// ����������񷢸��ͻ���
	tagNS_OpenQuestBoard send;
	send.dwErrorCode = E_Success;
	send.eQuestBoardType = eQuestBoardType;

	RoleBoardQuestMap RoleQuest = pRole->GetBoardQuestMgr(eQuestBoardType)->GetRoleBoardQuest();
	RoleBoardQuestMap::TMapIterator it = RoleQuest.Begin();
	tagRoleBoardQuest* pQuest = NULL;
	INT nIndex = 0;
	while( RoleQuest.PeekNext(it, pQuest) )
	{
		if (nIndex>=QUEST_BOARD_MAX_CAPACITY)
		{
			ASSERT(nIndex<QUEST_BOARD_MAX_CAPACITY);
			break;
		}
		send.u16Quest[nIndex] = pQuest->u16BoardID;
		++nIndex;
	}

	pRole->SendMessage(&send, send.dwSize);
	return E_Success;
}

DWORD PlayerSession::HandleRefreshQuestBoard(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_RenewQuestBoard);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	EQuestBoardType eQuestBoardType = EQBT_Null;
	if( pRecv->dwNPCID > 0 )
	{
		eQuestBoardType = GetQuestBoardType(pRecv->dwNPCID);
	}
	else
	{
		eQuestBoardType = GetQuestBoardTypeByTypeID(pRecv->dwNPCTypeID);
	}

	// todo ���߼��
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemSerial);
	if (!P_VALID(pItem) || (pItem->dwTypeID != REFRESH_QUEST_PANEL_ITEM_ID1 && pItem->dwTypeID != REFRESH_QUEST_PANEL_ITEM_ID2))
	{
		tagNS_OpenQuestBoard send;
		send.dwErrorCode = E_RefreshBoard_IM_Limit;
		send.eQuestBoardType = eQuestBoardType;
		pRole->SendMessage(&send, send.dwSize);
		return E_RefreshBoard_IM_Limit;
	}

	BoardQuestMgr * pBQM = pRole->GetBoardQuestMgr(eQuestBoardType);
	if( !P_VALID(pBQM) )
		return GT_INVALID;
	pRole->GetBoardQuestMgr(eQuestBoardType)->RefreshBoard(pRole, EBRT_IM, pRecv->n64ItemSerial, pRecv->dwNPCID);

	// �۳�����
	pRole->GetItemMgr().ItemUsedFromBag(pRecv->n64ItemSerial, 1, ELCLD_Item_Use);
	return 0;
}

EQuestBoardType PlayerSession::GetQuestBoardType(DWORD dwNPCID)
{
	EQuestBoardType eQuestBoardType = EQBT_Null;
	Creature* pNPC = m_pRole->GetMap()->FindCreature(dwNPCID);
	if( P_VALID(pNPC) )
	{
		if (pNPC->IsFunctionNPC(EFNPCT_QuestPanelReward))
			eQuestBoardType = EQBT_Reward;
		else if (pNPC->IsFunctionNPC(EFNPCT_QuestPanelYellowList))
			eQuestBoardType = EQBT_YellowList;
	}
	return eQuestBoardType;
}

EQuestBoardType PlayerSession::GetQuestBoardTypeByTypeID(DWORD dwNPCTypeID)
{
	EQuestBoardType eQuestBoardType = EQBT_Null;

	const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(dwNPCTypeID);
	if( P_VALID(pCreatureProto) )
	{
		if( EFNPCT_QuestPanelReward == pCreatureProto->eFunctionType )
		{
			eQuestBoardType = EQBT_Reward;
		}
		else if( EFNPCT_QuestPanelYellowList == pCreatureProto->eFunctionType )
		{
			eQuestBoardType = EQBT_Reward;
		}
	}

	return eQuestBoardType;
}

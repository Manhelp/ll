#include "stdafx.h"
#include "att_res.h"
#include "role.h"
#include "role_mgr.h"
#include "../ServerDefine/family_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_family.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/msg_family.h"
#include "family.h"
#include "family_quest.h"
#include "family_sprite_mgr.h"
#include "family_sprite.h"

VOID FamilyQuest::Destroy()
{
	SAFE_DEL_ARRAY(m_pQuests);
}

VOID FamilyQuest::Init(Family* p, bool bNewFamily)
{
	if (!P_VALID(p))
	{
		return;
	}

	ZeroMemory(this, sizeof(*this));

	InitializeCriticalSection(&m_renewlock);

	DWORD dwNowTick = g_world.GetWorldTick();
	DWORD m_dwLatestSaveTick = dwNowTick + IUTIL->RandomInRange(1, 1000); 
	if(bNewFamily)
	{
		m_bInitOk = TRUE;
		m_bIssueState = false;
	}
	else
	{
		m_bInitOk = FALSE;
		m_bIssueState = true;
	}
	m_pFamily = p;
	m_pQuests = new tagFamilyQuest[MAX_FAMILY_QUESTS_NUM];
	if (P_VALID(m_pQuests))
	{
		ZeroMemory(m_pQuests, MAX_FAMILY_QUESTS_NUM*sizeof(tagFamilyQuest));
	}
}

VOID FamilyQuest::Init(tagFamilyQuestLoadData* pData)
{
	if (!P_VALID(pData))
	{
		return;
	}

	if (pData->dwFamilyID != m_pFamily->GetID())
	{
		return;
	}

	if(pData->nProcess>= 0 && pData->nProcess <= 10000)
		m_nProcess = pData->nProcess;

	m_bIssueState = pData->bIssueState;

	tagFamilyQuest* p = (tagFamilyQuest*)(pData->QuestData);
	INT nCount = 0;
	for (int i=0; i<MAX_FAMILY_QUESTS_NUM; ++i)
	{
		if (!P_VALID(p))
		{
			break;
		}

		if (!P_VALID(m_pQuests+i))
		{
			continue;
		}

		m_pQuests[i].dwQuestID = p->dwQuestID;
		m_pQuests[i].nTargetCount = p->nTargetCount;

		if (P_VALID(p->dwQuestID))
		{
			++nCount;
		}

		++p;
	}

	m_byQuestNum = nCount;
	m_bInitOk = TRUE;
	//SetInitOK();
}

INT FamilyQuest::CalCompletePercent()
{
	if (m_byQuestNum<=0)
	{
		return 0;
	}

	DWORD dwTmp = 0;
	for (int i=0; i<m_byQuestNum; ++i)
	{
		if (!P_VALID(m_pQuests))
		{
			return 0;
		}

		tagFamilyQuest* pQuest = (m_pQuests+i);
		if (!P_VALID(pQuest) || !P_VALID(pQuest->dwQuestID))
		{
			continue;
		}

		const tagFamilyQuestProto* pQuestProto = g_attRes.GetOneFamilyQuestProto(pQuest->dwQuestID);
		if (!P_VALID(pQuestProto))
		{
			continue;
		}

		dwTmp += (pQuest->nTargetCount*10000)/pQuestProto->nQuestNum;
	}

	INT nProcess = (INT)(dwTmp/m_byQuestNum);
	return ( nProcess > 10000 ) ? 10000 : nProcess;
}

BOOL FamilyQuest::CanGetItemRewardI()
{
	if (m_nProcess >= CAN_GET_ITEM_REWARD_PROCESS)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL FamilyQuest::CanGetItemRewardII()
{
	if (10000 == m_nProcess)
	{
		return TRUE;
	}

	return FALSE;
}

DWORD FamilyQuest::CanGetReward(DWORD dwRoleID)
{
	if (!P_VALID(m_pFamily))
	{
		return E_FamilyQuest_Rewarded_Error;
	}

	if(FALSE == m_pFamily->GetFamilyMember().IsMemberExist(dwRoleID))
	{
		return E_FamilyQuest_Rewarded_Error;
	}
	
	// �����������ɶ��Ƿ����30%
	if (m_nProcess<CAN_GET_REWARD_PROCESS)
	{
		return E_FamilyQuest_Rewarded_ProcessLimit;
	}

	// �Ƿ��Ѿ���ȡ��
	if (TRUE == m_pFamily->GetFamilyMember().IsMemberRewarded(dwRoleID))
	{
		return E_FamilyQuest_Rewarded_AlreadyDone;
	}

	// ���ռ������ĳ�Ա�޷���ȡ����
	tagDWORDTime dwJoinTime = (tagDWORDTime)m_pFamily->GetFamilyMember().GetMemberJoinTime(dwRoleID);
	tagDWORDTime now = GetCurrentDWORDTime();
	if (dwJoinTime.day == now.day && dwJoinTime.month == now.month && dwJoinTime.year == now.year)
	{
		return E_FamilyQuest_Rewarded_NewJoiner;
	}

	return E_Success;
}

VOID FamilyQuest::SetRenewFlag()
{
	m_bIssueState = false;

	if (!P_VALID(m_pFamily))
	{
		return;
	}

	tagNDBC_ChangFamilyQuestIssueState send;
	send.dwFamilyID = m_pFamily->GetID();
	send.bIssueFlag = false;
	g_dbSession.Send(&send, send.dwSize);

	//IncFamilySpiriteExp();
}

VOID FamilyQuest::IncFamilySpiriteExp()
{
	if (!P_VALID(m_pFamily))
	{
		return;
	}

	// ���ݼ���������ɶ�Ϊ�����������ӳɳ��� 
	m_nProcess = CalCompletePercent();	// ��ֵ��û�б������ݿ�
	INT nFamilySpiriteExp = (INT)(m_nProcess * MAX_FAMILY_SPIRITE_ADDED_EXP / 10000) + m_pFamily->GetFamilyAtt().nActive;
	if(P_VALID(m_pFamily))
	{
		FamilySprite* p = g_FamilySpriteMgr.FindFamilySprite(m_pFamily->GetID());
		if (!P_VALID(p)) return;
		p->AddExp(nFamilySpiriteExp);
	}
}

VOID FamilyQuest::RenewQuest()
{
	if (FALSE == m_bInitOk)
	{
		return;
	}

	if (m_bIssueState)
	{
		return;
	}
	
	if(!P_VALID(m_pFamily))
	{
		return;
	}

	EnterCriticalSection(&m_renewlock);

	// �������������ɶ�
	m_nProcess = CalCompletePercent();

	// ���������ȡ��¼
	m_pFamily->GetFamilyMember().ClearQuestRewardedFlag();

	// �����������
	tagFamilyQuestDetail OldQuests[MAX_FAMILY_QUESTS_NUM];
	ZeroMemory(OldQuests, MAX_FAMILY_QUESTS_NUM*sizeof(tagFamilyQuestDetail));
	//tagFamilyQuest* pQuest = m_pQuests;
	for(int i=0; i<m_byQuestNum; ++i)
	{
		if (i>=MAX_FAMILY_QUESTS_NUM)
		{
			ASSERT(0);
			break;
		}

// 		pQuest += i;
// 		if (!P_VALID(pQuest))
// 		{
// 			break;
// 		}

		OldQuests[i].dwQuestID = m_pQuests[i].dwQuestID;
		OldQuests[i].nTargetCount = m_pQuests[i].nTargetCount;
		OldQuests[i].m_pQuestProto = g_attRes.GetOneFamilyQuestProto(m_pQuests[i].dwQuestID);		
	}
	SAFE_DEL_ARRAY(m_pQuests);
	m_byQuestNum = 0;

	// ����������
	BYTE byQuestNum = 0;
	GetFamilyQuestNum(m_pFamily->GetFamilySpriteLevel(), byQuestNum)

	tagFamilyQuest* pTmpNewQuestPointer = new tagFamilyQuest[MAX_FAMILY_QUESTS_NUM];	// Destroy�������ͷ�
	if (!P_VALID(pTmpNewQuestPointer))
	{
		LeaveCriticalSection(&m_renewlock);
		return;
	}
	ZeroMemory(pTmpNewQuestPointer, sizeof(tagFamilyQuest)*MAX_FAMILY_QUESTS_NUM);

	//TMap<DWORD, tagFamilyQuestProto*>& TmpQuestCollection = g_attRes.GetFamilyQuestProto();
	TMap<DWORD, tagFamilyQuestProto*> TmpQuestCollection = g_attRes.GetFamilyQuestProto();
	if(TmpQuestCollection.Size() < byQuestNum)
	{
		LeaveCriticalSection(&m_renewlock);
		return;
	}

	// ��ʼ����������ɹ���
	EFamilyQuestType eNewestQuestType = EQT_Null;
	for (int i=0; i<byQuestNum; ++i)
	{
		tagFamilyQuestProto* pProto = NULL;
		DWORD dwQuestID = 0;
		while(!P_VALID(pProto))
		{
			TmpQuestCollection.RandPeek(dwQuestID, pProto);

		
			if (P_VALID(pProto))
			{
				if (!P_VALID(pProto->dwID))
				{
					pProto = NULL;
					continue;
				}

				// �������ȼ�����������ȼ��Ƿ�ƥ��
				if(pProto->nHighLevel < m_pFamily->GetFamilySpriteLevel() || pProto->nLowLevel > m_pFamily->GetFamilySpriteLevel())
				{
					TmpQuestCollection.Erase(pProto->dwID);
					pProto = NULL;
					if(TmpQuestCollection.Size() < byQuestNum-i)
						goto NotEnoughQuest;
					continue;
				}

				// ��������: �������������ظ������������������Ҳ�����ظ�
				for (int j=0; j<MAX_FAMILY_QUESTS_NUM; ++j)
				{

					if ((P_VALID(OldQuests[j].dwQuestID) && OldQuests[j].dwQuestID == pProto->dwID) ||
						(P_VALID(OldQuests[j].m_pQuestProto) && OldQuests[j].m_pQuestProto->dwConditionID == pProto->dwConditionID))

					{	// �������ظ���
						TmpQuestCollection.Erase(pProto->dwID);
						pProto = NULL;
						
						if(TmpQuestCollection.Size() < byQuestNum-i)
						{
// 							ASSERT(0);
// 							m_pQuests = NULL;
// 							m_byQuestNum = 0;
// 							SAFE_DEL_ARRAY(pTmpNewQuestPointer);
// 							return;
							goto NotEnoughQuest;
						}
						
						break;
					}
				}

				if (!P_VALID(pProto))
				{
					continue;
				}

				// �������Ͳ��ܼ���
				if (eNewestQuestType == pProto->eType)
				{
					TmpQuestCollection.Erase(pProto->dwID);
					pProto = NULL;
					if(TmpQuestCollection.Size() < byQuestNum-i)
						goto NotEnoughQuest;

					continue;
				}

				// �ҵ����ϵ���
				tagFamilyQuest* pNewOne = pTmpNewQuestPointer+i;
				if (!P_VALID(pNewOne))
				{
					m_pQuests = NULL;
					m_byQuestNum = 0;
					SAFE_DEL_ARRAY(pTmpNewQuestPointer);
					LeaveCriticalSection(&m_renewlock);
					return;
				}
				
				eNewestQuestType = pProto->eType;
				pNewOne->dwQuestID = pProto->dwID;
				pNewOne->nTargetCount = 0;
				++m_byQuestNum;

				TmpQuestCollection.Erase(pProto->dwID);
				if(TmpQuestCollection.Size() < byQuestNum-i-1) // �������������񲻹�
					goto NotEnoughQuest;
			}
		}
	}

NotEnoughQuest:
	if (byQuestNum > m_byQuestNum)
	{
		ILOG->Write(_T("\r\nRenew family quest failed,start to do this in second rule.\n"));

		INT nQuestNumWantted = byQuestNum - m_byQuestNum;
		TMap<DWORD, tagFamilyQuestProto*> &mapTmpQuestCollection = g_attRes.GetFamilyQuestProto();
		for (int i=0; i<nQuestNumWantted; ++i)
		{
			DWORD dwQuestID = 0;
			tagFamilyQuestProto* pQuestProto = NULL;
			tagFamilyQuest* pNewOne = pTmpNewQuestPointer+m_byQuestNum;
			while (!P_VALID(dwQuestID) || !P_VALID(pQuestProto))
			{
				mapTmpQuestCollection.RandPeek(dwQuestID, pQuestProto);
				if(!P_VALID(dwQuestID) || !P_VALID(pQuestProto)) continue;
				
				bool bExist = false;
				for(int j=0; j<m_byQuestNum; ++j)
				{
					if(dwQuestID == pTmpNewQuestPointer[j].dwQuestID)
					{
						bExist = true;
						break;
					}
				}
				
				if(bExist) 
				{
					dwQuestID = 0;
					pQuestProto = NULL;
					continue;
				}
				
				pNewOne->dwQuestID = dwQuestID;
				pNewOne->nTargetCount = 0;
				++m_byQuestNum;	
			}	
		}
	}
	
	m_pQuests = pTmpNewQuestPointer;
	m_bIssueState = true;
	if (m_byQuestNum <= 0)
	{
		ILOG->Write(_T("\r\nRenew family quest failed finally\n"));
		LeaveCriticalSection(&m_renewlock);
		return;
	}

	LeaveCriticalSection(&m_renewlock);

	// ͬ�����ݿ�
	tagNDBC_RenewFamilyQuest send;
	send.dwFamilyID = m_pFamily->GetID();
	send.nProcess = m_nProcess;
	ZeroMemory(&(send.Quests[0]), MAX_FAMILY_QUESTS_NUM * sizeof(tagFamilyQuest));
	for (int i=0; i<m_byQuestNum; ++i)
	{
		tagFamilyQuest* pQuest = m_pQuests+i;
		if (!P_VALID(pQuest))
		{
			break;
		}

		if (!P_VALID(pQuest->dwQuestID))
		{
			break;
		}

		send.Quests[i].dwQuestID = pQuest->dwQuestID;
		send.Quests[i].nTargetCount = 0;
		++(send.byNum);
	}

	g_dbSession.Send(&send, send.dwSize);
}

VOID FamilyQuest::SaveFamilyQuestDetail()
{
	if (m_byQuestNum<=0)
	{
		return;
	}

	if (m_dwLatestSaveTick >= g_world.GetWorldTick())
	{
		return;
	}

	if ( g_world.GetWorldTick() - m_dwLatestSaveTick < SAVE_FAMILY_QUEST_INTERNAL_MIN*60*TICK_PER_SECOND)
	{
		return;
	}

	if (!P_VALID(m_pFamily) || !P_VALID(m_pFamily->GetID()))
	{
		return;
	}

	m_dwLatestSaveTick = g_world.GetWorldTick();

	tagNDBC_SaveFamilyQuestDetail send;
	send.dwFamilyID = m_pFamily->GetID();
	for (int i=0; i<m_byQuestNum; ++i)
	{
		tagFamilyQuest* pQuest = m_pQuests+i;
		if (!P_VALID(pQuest) || !P_VALID(pQuest->dwQuestID))
		{
			// �쳣�������������
			m_byQuestNum = i;
			break;
		}
		send.Quests[i].dwQuestID = pQuest->dwQuestID;
		send.Quests[i].nTargetCount = pQuest->nTargetCount;
	}

	g_dbSession.Send(&send, send.dwSize);
}

VOID FamilyQuest::Update()
{
	// ������
	tagDWORDTime now = GetCurrentDWORDTime();
	if (0==now.hour && 0==now.min && now.day != m_dwLatestRenewTime.day)
	{
		m_dwLatestRenewTime = GetCurrentDWORDTime();

		SetRenewFlag(); 
	}

	if (FALSE == m_bInitOk || m_byQuestNum <= 0)
	{
		tagNDBC_RequestFamilyQuest send;
		send.dwFamilyID = m_pFamily->GetID();
		g_dbSession.Send(&send, send.dwSize);
		return;
	}
	
	// �����ݿ�
	if(m_bIssueState)
		SaveFamilyQuestDetail();
}

VOID FamilyQuest::OnAnyMemberKillMonster(DWORD dwMonsterID, INT nMonsterLevel, DWORD dwMapCrc)
{
	if (FALSE == m_bInitOk)
	{
		return;
	}

	if (!m_bIssueState)
	{
		return;
	}

	if (m_byQuestNum <= 0)
	{
		return;
	}

	const tagCreatureProto* pProto = g_attRes.GetCreatureProto(dwMonsterID);
	if(!P_VALID(pProto)) return;

	for (int i=0; i<m_byQuestNum; ++i)
	{
		tagFamilyQuest* pQuest = m_pQuests + i;
		if (!P_VALID(pQuest) || !P_VALID(pQuest->dwQuestID))
		{
			continue;
		}

		const tagFamilyQuestProto* pQuestProto = g_attRes.GetOneFamilyQuestProto(pQuest->dwQuestID);
		if (!P_VALID(pQuestProto) || pQuest->nTargetCount >= pQuestProto->nQuestNum)
		{
			continue;
		}

		const tagFamilyQuestAttProto* pQuestCondition = g_attRes.GetOneFamilyQuestCondition(pQuestProto->dwConditionID);
		if (!P_VALID(pQuestCondition) || pQuestCondition->eFinishType != EQFT_Monster)
		{
			continue;
		}
		
		if ( EJT_Condition == pQuestCondition->eJudgeType )
		{
			if (P_VALID(pQuestCondition->nLowLevel) && nMonsterLevel < pQuestCondition->nLowLevel)
			{
				continue;
			}

			if (P_VALID(pQuestCondition->nHighLevel) && nMonsterLevel > pQuestCondition->nHighLevel)
			{
				continue;
			}

			if (P_VALID(pQuestCondition->dwMapcrc) && dwMapCrc != pQuestCondition->dwMapcrc)
			{
				continue;
			}

			if (ER_Null!=(ERank)(pQuestCondition->nMonsterRank) && pProto->eLite != (ERank)(pQuestCondition->nMonsterRank))
			{
				continue;
			}

			++(pQuest->nTargetCount);
		}
		else if (EJT_ID == pQuestCondition->eJudgeType)
		{
			for (int j=0; j<MAX_ItemOrMonsterID; ++j)
			{
				if (dwMonsterID != pQuestCondition->dwItemOrMonsterID[j]) continue;
				
				++(pQuest->nTargetCount);
				break;
			}
		}
	}
}

DWORD FamilyQuest::OnAnyMemberGiveInItem(DWORD dwRoleID, DWORD dwQuestID, DWORD dwTypeID, INT64 n64Serial, INT nNum, INT& nSum)
{	
	if (FALSE == m_bInitOk)
	{
		return GT_INVALID;
	}

	if (!m_bIssueState)
	{
		return GT_INVALID;
	}

	if (m_byQuestNum <= 0)
	{
		return GT_INVALID;
	}

	if (nNum<=0)
	{
		return GT_INVALID;
	}

	const tagFamilyQuestProto* pProto = g_attRes.GetOneFamilyQuestProto(dwQuestID);
	if (!P_VALID(pProto))
	{
		return E_FamilyQuest_GiveInItem_QuestInvalid;
	}

	const tagFamilyQuestAttProto* pCondition = g_attRes.GetOneFamilyQuestCondition(pProto->dwConditionID);
	if (!P_VALID(pCondition) || pCondition->eFinishType != EQFT_Item)
	{
		return E_FamilyQuest_GiveInItem_QuestInvalid;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return E_FamilyQuest_GiveInItem_RoleInvalid;
	}

	tagFamilyQuest* pQuest = NULL;
	for (int i=0; i<m_byQuestNum; ++i)
	{
		pQuest = m_pQuests+i;
		if (!P_VALID(pQuest) || pQuest->dwQuestID != dwQuestID)
		{
			continue;
		}
		break;
	}

	if (!P_VALID(pQuest) || pQuest->dwQuestID != dwQuestID) 
		return E_FamilyQuest_GiveInItem_QuestNotMatch;

	// �������������Ʒ�Ƿ��Ѿ�����
	if (pQuest->nTargetCount >= pProto->nQuestNum)
		return E_FamilyQuest_GiveInItem_QuestItemEnough;

	if (P_VALID(dwTypeID))
	{
		if (MIsEquipment(dwTypeID))
		{
			return GT_INVALID;
		}

		// ����Ͻɵ���Ʒ�Ƿ�������������Ʒ
		bool bCheck = false;
		for (int i=0; i<MAX_ItemOrMonsterID; ++i)
		{
			if(pCondition->dwItemOrMonsterID[i] == dwTypeID)
			{
				bCheck = true;
				break;
			}
		}

		if (false == bCheck)
		{
			return E_FamilyQuest_GiveInItem_NotQuestItem;
		}

		TList<tagItem*> tmpList;
		INT nCount = pRole->GetItemMgr().GetBagSameItemList(tmpList, dwTypeID, nNum);
		if (nCount<nNum)
			return E_FamilyQuest_GiveInItem_ItemNotEnough;

		if (pQuest->nTargetCount + nNum > pProto->nQuestNum)
		{
			nNum = pProto->nQuestNum - pQuest->nTargetCount;
		}

		pQuest->nTargetCount += nNum;
		nSum = pQuest->nTargetCount;
		return pRole->GetItemMgr().DelBagSameItem(tmpList, nNum, ELCID_FamilyQuest_GiveInItem);
	}
	else if (P_VALID(n64Serial))
	{
		tagItem* pItem = pRole->GetItemMgr().GetBagItem(n64Serial);
		if (!P_VALID(pItem))
		{
			return E_FamilyQuest_GiveInItem_NotQuestItem;
		}

		if (!MIsEquipment(pItem->dwTypeID) || nNum != 1)
		{
			return GT_INVALID;
		}

		// ����Ͻɵ���Ʒ�Ƿ�������������Ʒ
		bool bCheck = false;
		for (int i=0; i<MAX_ItemOrMonsterID; ++i)
		{
			if(pCondition->dwItemOrMonsterID[i] == pItem->dwTypeID)
			{
				bCheck = true;
				break;
			}
		}

		if (false == bCheck)
		{
			return E_FamilyQuest_GiveInItem_NotQuestItem;
		}

		pQuest->nTargetCount++;
		nSum = pQuest->nTargetCount;
		return pRole->GetItemMgr().DelFromBag(n64Serial, ELCID_Family_Quest_GiveInItem, 1);
	}
	else
	{
		return E_FamilyQuest_GiveInItem_NotQuestItem;
	}
}
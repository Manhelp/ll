#include "stdafx.h"
#include "world.h"
#include "db_session.h"
#include "role_mgr.h"
#include "role.h"
#include "ClergyRank.h"

ClergyRankingMgr g_ClergyRankingMgr;

ClergyRankingMgr::ClergyRankingMgr()
{
	for (int i=0; i<ECGT_Num; ++i)
	{
		m_dwCandidateCount[i] = 0;
	}

	m_bCandidateCountChg = false;

	m_mapClergyMars.Clear();
	m_mapClergyApollo.Clear();
	m_mapClergyRabbi.Clear();
	m_mapClergyPeace.Clear();
}

DWORD GetClergyID(ECampaignGodType eType1, EClergyType eType2)
{
 	if (eType1 == ECGT_War)
 	{
 		if (eType2 == ECT_WomenChamp)
 			return ECC_WarWomen;
 		else if(eType2 == ECT_MenChamp)
			return ECC_WarMen;
 		else if(eType2 == ECT_Winner)
			return ECC_WarWinner;
 	}
 	else if (eType1 == ECGT_Sun)
 	{
 		if (eType2 == ECT_WomenChamp)
			return ECC_SunWomen;
 		else if(eType2 == ECT_MenChamp)
			return ECC_SunMen;
 		else if(eType2 == ECT_Winner)
			return ECC_SunWinner;
 	}
 	else if (eType1 == ECGT_Mana)
 	{
 		if (eType2 == ECT_WomenChamp)
			return ECC_ManaWomen;
 		else if(eType2 == ECT_MenChamp)
			return ECC_ManaMen;
 		else if(eType2 == ECT_Winner)
			return ECC_ManaWinner;
 	}
 	else if (eType1 == ECGT_Peace)
 	{
 		if (eType2 == ECT_WomenChamp)
			return ECC_PeaceWomen;
 		else if(eType2 == ECT_MenChamp)
			return ECC_PeaceMen;
 		else if(eType2 == ECT_Winner)
			return ECC_PeaceWinner;
 	}

	return NULL;
}

void ClergyRankingMgr::Init()
{
 	tagNDBC_RequestInitClergyRank send;
 	g_dbSession.Send(&send, send.dwSize);

	tagNDBC_Ask4CandidateCount msg;
	g_dbSession.Send(&msg, msg.dwSize);
}

// void ClergyRankingMgr::ReInit()
// {
// 	tagNDBC_RequestInitClergyRank send;
// 	g_dbSession.Send(&send, send.dwSize);
// }

VOID ClergyRankingMgr::DoInit(tagNDBS_InitClergyRank* pMsg)
{
	if (NULL == pMsg->nSum)
	{
		return;
	}

	if (pMsg->eType == ECGT_War)
	{
		DoInit(m_mapClergyMars, pMsg->eType, pMsg->Rankings);
	}
	else if (pMsg->eType == ECGT_Sun)
	{
		DoInit(m_mapClergyApollo, pMsg->eType, pMsg->Rankings);
	}
	else if (pMsg->eType == ECGT_Mana)
	{
		DoInit(m_mapClergyRabbi, pMsg->eType, pMsg->Rankings);
	}
	else if (pMsg->eType == ECGT_Peace)
	{
		DoInit(m_mapClergyPeace, pMsg->eType, pMsg->Rankings);
	}
}

VOID ClergyRankingMgr::ClearPreClergy(ECampaignGodType eType)
{
	TMap<DWORD, tagClergyNode*>& mapClergy = g_ClergyRankingMgr.GetClergyRank(eType);
	
	// ���ȴ������ߵ�
	DWORD dwOfflineRoles[MAX_CLERGY_WINNER] = {0};
	int i = 0;
	tagClergyNode* pTemp = NULL;
	TMap<DWORD, tagClergyNode*>::TMapIterator it = mapClergy.Begin();
	while (mapClergy.PeekNext(it, pTemp))
	{
		if ( pTemp->eClergy == ECT_Loser )
		{
			continue;
		}

		DWORD dwRoleID = pTemp->dwRoleID;
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{	
			dwOfflineRoles[i] = dwRoleID;
			i++;
			continue;
		}

		pRole->ClergyBack();
		SAFE_DEL(pTemp);
	}
	mapClergy.Clear();

	// ���ڲ����ߵģ��������ݿ�ȥ��
	if (i>0)
	{
		DWORD dwSize = sizeof(tagNDBC_CancelPreClergy) - sizeof(DWORD) + i*sizeof(DWORD);
		MCREATE_MSG(pSend, dwSize, NDBC_CancelPreClergy);
		pSend->nNum = i;
		memcpy(pSend->dwRoleID, dwOfflineRoles, sizeof(DWORD)*i);
		g_dbSession.Send(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}
}

VOID ClergyRankingMgr::DoInit(TMap<DWORD, tagClergyNode*>& mapClergy, ECampaignGodType eType, tagClergyNode* pData)
{
	if (!mapClergy.Empty())
	{
		return;
	}

	INT by = 0;
	DWORD dwRoleIDs[MAX_CLERGY_WINNER] = {0};
	for(INT i=0; i<MAX_CampaignInfoNumPerType; ++i)
	{
		if(NULL == pData[i].dwRoleID) continue;

		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pData[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;

		tagClergyNode* pTmpNode = pData+i;

		tagClergyNode* pRankingsNode = new tagClergyNode;
		pRankingsNode->nNum = pTmpNode->nNum;
		pRankingsNode->dwRoleID = pTmpNode->dwRoleID;
		pRankingsNode->eClergy = pTmpNode->eClergy;
		pRankingsNode->dwPoint = pTmpNode->dwPoint;
		pRankingsNode->bRecv = pTmpNode->bRecv;

		mapClergy.Add(pRankingsNode->dwRoleID, pRankingsNode);

		// ����ѡ�ɹ��Ľ�ɫ���ߣ���Ϊ���޸���ְ�����ڲ����ߵľ�ѡ�ɹ���ɫ�����ڽ�ɫ���ߵ�ʱ��Ϊ���޸���ְ
		
		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole) && !pRankingsNode->bRecv)
		{
			dwRoleIDs[by++] = pRole->GetID();

			DWORD dwClergyID = GetClergyID(eType, pRankingsNode->eClergy);
			if(dwClergyID!=NULL) 
			{
				// ��ְ��ְ
				if (E_Success == pRole->PromotionClergy(dwClergyID, false))
				{
					//�㲥����Χ�Ľ�ɫ
					tagNS_ChangeClergy send1;
					send1.dwClergy = dwClergyID;
					send1.dwRoleID = pRole->GetID();
					if( P_VALID(pRole->GetMap()))
						pRole->GetMap()->SendBigVisTileMsg(pRole, &send1, send1.dwSize);
				}
			}
		}
	}
	
	if (by>0)
	{
		// ����Ϣ���ڸı����ݿ��С�ְλ�Ƿ���ȡ��λ
		DWORD dwSize = sizeof(tagNDBC_CandidateRecvClergy) - sizeof(DWORD) + by*sizeof(DWORD);
		MCREATE_MSG(pSend, dwSize, NDBC_CandidateRecvClergy);
		pSend->eType = eType;
		pSend->nNum = by;
		memcpy(pSend->dwRoleID, dwRoleIDs, sizeof(DWORD)*by);
		g_dbSession.Send(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}
}

BOOL ClergyRankingMgr::IsRoleWinClergy(DWORD dwRoleID, ECampaignGodType& eType1, EClergyType& eType2)
{
	eType1 = ECGT_Null;
	eType2 = ECT_Null;
	tagClergyNode* pClergy = NULL;

	if (P_VALID(pClergy = m_mapClergyMars.Peek(dwRoleID)))
	{
		if (!pClergy->bRecv)
		{
			eType1 = ECGT_War;
			eType2 = pClergy->eClergy;
			return TRUE;
		}		
	}

	if (P_VALID(pClergy = m_mapClergyApollo.Peek(dwRoleID)))
	{
		if (!pClergy->bRecv)
		{
			eType1 = ECGT_Sun;
			eType2 = pClergy->eClergy;
			return TRUE;
		}
	}

	if (P_VALID(pClergy = m_mapClergyRabbi.Peek(dwRoleID)))
	{
		if (!pClergy->bRecv)
		{
			eType1 = ECGT_Mana;
			eType2 = pClergy->eClergy;
			return TRUE;
		}
	}

	if (P_VALID(pClergy = m_mapClergyPeace.Peek(dwRoleID)))
	{
		if (!pClergy->bRecv)
		{
			eType1 = ECGT_Peace;
			eType2 = pClergy->eClergy;
			return TRUE;
		}
	}

	return FALSE;
}
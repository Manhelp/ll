//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_struggle.cpp
// author: twan
// actor:
// data: 2009-9-10
// last:
// brief: ��ս
//-------------------------------------------------------------------------------

#include "StdAfx.h"
#include "city_struggle.h"
#include "Role.h"
#include "role_mgr.h"
#include "map.h"
#include "map_creator.h"
#include "city.h"
#include "city_mgr.h"
#include "guild_mgr.h"
#include "../WorldDefine/guild_define.h"

CityStruggle::CityStruggle()
{
}

CityStruggle::~CityStruggle()
{
	Destroy();
}

//--------------------------------------------------------------------
// ��ʼ��
//--------------------------------------------------------------------
VOID CityStruggle::Init(City* pCity)
{
	m_eState				=	ECSGS_UnStruggle;
	m_nUpdateTick			=	0;
	m_nAtkReviveTick		=	25 * TICK_PER_SECOND;
	m_nDefReviveTick		=	25 * TICK_PER_SECOND;
	m_nEudemonTick			=	0;
	m_pCity					=	pCity;
	m_bBeginSummonCountDown	=	FALSE;
	m_eResult				=	ECSRT_None;
}

//--------------------------------------------------------------------
// ���ó�ս״̬
//--------------------------------------------------------------------
VOID CityStruggle::SetCityStruggleState(ECityStruggleState eState)
{
	// �жϺϷ���
	if (eState < ECSGS_Start || eState > ECSGS_End)
	{
		return;
	}

	m_eState = eState;
}

//--------------------------------------------------------------------
// ����
//--------------------------------------------------------------------
VOID CityStruggle::Destroy()
{
	tagStruggleRevive* pRevive = NULL;
	m_mapRevive.ResetIterator();
	while (m_mapRevive.PeekNext(pRevive))
	{
		SAFE_DEL(pRevive);
	}
	m_mapRevive.Clear();
}

//--------------------------------------------------------------------
// ��ս����
//--------------------------------------------------------------------
VOID CityStruggle::Update()
{
	if (m_eState != ECSGS_Struggle)		return;

	++m_nUpdateTick;

	UpdateRoleRevive();
	UpdateBall();
	UpdateColumns();
	UpdateEudemon();
}

//--------------------------------------------------------------------
// ���³�ս��Ҹ���
//--------------------------------------------------------------------
VOID CityStruggle::UpdateRoleRevive()
{
	// �������������
	DWORD dwRoleID = 0;
	vector<DWORD> vecNeedDel;
	tagStruggleRevive* pRevive = NULL;
	TMap<DWORD, tagStruggleRevive*>::TMapIterator it = m_mapRevive.Begin();
	while(m_mapRevive.PeekNext(it,dwRoleID, pRevive))
	{
		if (pRevive->eForceType == ECFT_Attack)
		{
			// ��������
			if (m_nUpdateTick - pRevive->nDeadTick >= m_nAtkReviveTick)
			{
				// �������
				ReviveRole(dwRoleID);
				vecNeedDel.push_back(dwRoleID);
			}
		}
		else
		{
			// ��������
			if (m_nUpdateTick - pRevive->nDeadTick >= m_nDefReviveTick)
			{
				// �������
				ReviveRole(dwRoleID);
				vecNeedDel.push_back(dwRoleID);
			}
		}
	}  

	// �Ӵ����������ɾ���Ѹ������
	for (vector<DWORD>::iterator it = vecNeedDel.begin(); it != vecNeedDel.end(); ++it)
	{
		tagStruggleRevive* pDel = m_mapRevive.Peek(*it);
		SAFE_DEL(pDel);
		m_mapRevive.Erase(*it);
	}
}

//--------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------
VOID CityStruggle::UpdateBall()
{

}

//--------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------
VOID CityStruggle::UpdateColumns()
{

}

//--------------------------------------------------------------------
// �����ػ���
//--------------------------------------------------------------------
VOID CityStruggle::UpdateEudemon()
{
	// û�п�ʼ�ٻ�
	if (!m_bBeginSummonCountDown)
		return;

	if (--m_nEudemonTick < 0)
	{
		// �ɹ��ٻ������ýű�
		const ActScript* pScript = g_ScriptMgr.GetActScript(CITY_STRUGGLE_ACT_ID);
		if (P_VALID(pScript))
		{
			pScript->FinishEudemonSummon();
		}
		m_bBeginSummonCountDown = FALSE;
	}
}

//--------------------------------------------------------------------
// ������Ҽ��븴�����
//--------------------------------------------------------------------
VOID CityStruggle::AddToReviveMap(Role* pRole)
{
	if (m_eState != ECSGS_Struggle)		
		return;

	ECityForceType eForce;
	// �ӽ��������в������
	if (TRUE == IsRoleInForce(pRole,ECFT_Attack))
	{
		eForce = ECFT_Attack;
	}

	// �ӷ��������в������
	else if (TRUE == IsRoleInForce(pRole,ECFT_Defence))
	{
		eForce = ECFT_Defence;
	}

	else
	{
		// û�ҵ�
		return;
	}

	tagStruggleRevive* pRevive = new tagStruggleRevive;
	pRevive->eForceType = eForce;
	pRevive->nDeadTick = m_nUpdateTick;

	tagStruggleRevive* pRet = m_mapRevive.Peek(pRole->GetID());
	if (!P_VALID(pRet))
	{
		// ���ҵĽ�ɫû�ڸ�������в����
		m_mutexRevive.Acquire();
		m_mapRevive.Add(pRole->GetID(), pRevive);
		m_mutexRevive.Release();

		// ��ͻ��˷��͸�������ʱ��
		tagNS_CityStruggleRevive send;
		if (eForce == ECFT_Attack)
		{
			send.nSec = m_nAtkReviveTick / TICK_PER_SECOND;
		}
		else
		{
			send.nSec = m_nDefReviveTick / TICK_PER_SECOND;
		}
		pRole->SendMessage(&send, send.dwSize);
	}
	else
		delete pRevive;
}

//--------------------------------------------------------------------
// ����Ƿ�����Ӫ��
//--------------------------------------------------------------------
BOOL CityStruggle::IsRoleInForce(Role* pRole, ECityForceType eForce)
{
	ASSERT(P_VALID(m_pCity));

	// ��ȡ�������
	INT nGuildNum = MAX_CITY_ATTACK_FORCE;
	DWORD dwGuildID[MAX_CITY_ATTACK_FORCE];
	memset(dwGuildID, 0xFF, sizeof(dwGuildID));

	if (!m_pCity->GetConfirmGuild(eForce, nGuildNum, dwGuildID))
		return FALSE;

	if (nGuildNum <= 0)
		return FALSE;

	// �Ӱ�������
	for (INT i = 0; i != nGuildNum; ++i)
	{
		Guild* pGuild = g_guildMgr.GetGuild(dwGuildID[i]);
		if (!P_VALID(pGuild))
			return FALSE;

		tagGuildMember* pGuildMem = pGuild->GetMember(pRole->GetID());
		if (P_VALID(pGuildMem))		// �ҵ�
		{
			return TRUE;
		}

	}
	return FALSE;
}

//--------------------------------------------------------------------
// ��������Ӫ
//--------------------------------------------------------------------
BOOL CityStruggle::GetForceType(Role* pRole, ECityForceType& eForce)
{
	if (TRUE == IsRoleInForce(pRole, ECFT_Attack))
	{
		eForce = ECFT_Attack;
		return TRUE;
	}

	if (TRUE == IsRoleInForce(pRole, ECFT_Defence))
	{
		eForce = ECFT_Defence;
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------
// �������
//--------------------------------------------------------------------
VOID CityStruggle::ReviveRole(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))		
		return;

	tagNS_RoleRevive send;
	send.dwRoleID		= dwRoleID;
	//send.dwErrorCode	= pRole->Revive(ERRT_CityStruggle, GT_INVALID);
	if( !pRole->IsInState(ES_Dead) )
		send.dwErrorCode = E_Revive_NotDead;

	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(pRole->GetRebornMapID());
	if(!P_VALID(pMapInfo))
	{
		ASSERT(P_VALID(pMapInfo));
		send.dwErrorCode = E_Revive_MapNotFound;
	}

	
	// ���ø���㼰Ѫ����������
	pRole->GetRevive().nHP		=	pRole->GetAttValue(ERA_MaxHP);
	pRole->GetRevive().nMP		=	pRole->GetAttValue(ERA_MaxMP);
	pRole->GetRevive().fX		=	pMapInfo->vRebornPos.x;
	pRole->GetRevive().fY		=	pMapInfo->vRebornPos.y;
	pRole->GetRevive().fZ		=	pMapInfo->vRebornPos.z;
	pRole->GetRevive().dwMapID	=	pRole->GetRebornMapID();

	pRole->GetMap()->OnRevive(pRole, ERRT_CityStruggle, pRole->GetRevive().nHP, pRole->GetRevive().nMP, 
					pRole->GetRevive().fX, pRole->GetRevive().fY, pRole->GetRevive().fZ, pRole->GetRevive().dwMapID);

	pRole->SetAttValue(ERA_HP, pRole->GetRevive().nHP);
	pRole->SetAttValue(ERA_MP, pRole->GetRevive().nMP);
	pRole->UnsetState(ES_Dead);

	// ��ո�����ؼ�¼��Ϣ
	pRole->GetRevive().nHP = 0; 
	pRole->GetRevive().nMP = 0;
	send.dwErrorCode = E_Success;

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}

	if(E_Success == send.dwErrorCode)
	{
		// ���ָ���ص�
		pRole->GotoNewMap(pRole->GetRevive().dwMapID, 
			pRole->GetRevive().fX, pRole->GetRevive().fY, pRole->GetRevive().fZ);
	}

}

//--------------------------------------------------------------------
// �������˸���ʱ��
//--------------------------------------------------------------------
VOID CityStruggle::SetReviveTime(ECityForceType eForce, INT nSec)
{
	if (eForce < ECFT_Attack || eForce > ECFT_Defence)
		return;

	if (eForce == ECFT_Attack)
		m_nAtkReviveTick = nSec * TICK_PER_SECOND;
	else
		m_nDefReviveTick = nSec * TICK_PER_SECOND;
}

//--------------------------------------------------------------------
// �����ػ����ٻ�ʱ��
//--------------------------------------------------------------------
VOID CityStruggle::SetSummonTime(INT nSec)
{
	m_bBeginSummonCountDown = TRUE;
	m_nEudemonTick = nSec * TICK_PER_SECOND;
}

VOID CityStruggle::StopSummon()
{
	m_bBeginSummonCountDown = FALSE;
}

//--------------------------------------------------------------------
// ���ó�ս���
//--------------------------------------------------------------------
VOID CityStruggle::SetResult(ECityStruggleResult eResult)
{
	m_eResult = eResult;
}

//--------------------------------------------------------------------
// ��ʼ����ս�������
//--------------------------------------------------------------------
VOID CityStruggle::InitCSRoleData()
{
	m_mapRoleData.ResetIterator();
	tagCSRoleData* pData = NULL;
	while (m_mapRoleData.PeekNext(pData))
	{
		SAFE_DEL(pData);
	}
	m_mapRoleData.Clear();
}

//--------------------------------------------------------------------
// ����һ����ս�������
//--------------------------------------------------------------------
VOID CityStruggle::AddRoleDataInfo(DWORD dwRoleID)
{
	tagCSRoleData* pData = m_mapRoleData.Peek(dwRoleID);
	if (P_VALID(pData))
		return;

	pData = new tagCSRoleData;
	m_mapRoleData.Add(dwRoleID, pData);
}

//--------------------------------------------------------------------
// ��ս��ɱ������ӻ�ɱ����
//--------------------------------------------------------------------
VOID CityStruggle::AddKillRoleNum(DWORD dwKillerID, DWORD dwTargetID)
{
	tagCSRoleData* pData = m_mapRoleData.Peek(dwKillerID);
	if (!P_VALID(pData))
		return;

	if (pData->listKill.Size() < NOT_REPEAT_LIMIT_ROLE_NUM && !pData->listKill.IsExist(dwTargetID))
	{
		pData->listKill.PushBack(dwTargetID);
		AddRoleData(dwKillerID, ECSRD_Kills, 1);
	}
	else if (!pData->listKill.IsExist(dwTargetID))
	{
		pData->listKill.PopFront();
		pData->listKill.PushBack(dwTargetID);
		AddRoleData(dwKillerID, ECSRD_Kills, 1);
	}
	else
	{
		return;
	}

}

//--------------------------------------------------------------------
// �ı���ҳ�ս����
//--------------------------------------------------------------------
VOID CityStruggle::AddRoleData(DWORD dwRoleID, ECityStruggleRoleData eType, DWORD dwData)
{
	tagCSRoleData* pData = m_mapRoleData.Peek(dwRoleID);
	if (!P_VALID(pData))
		return;

	switch (eType)
	{
	case ECSRD_ActiveTime:
		if (pData->RoleData.byActiveTime < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byActiveTime += (BYTE)dwData;
		break;

	case ECSRD_Kills:
		if (pData->RoleData.byKills < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byKills += (BYTE)dwData;
		break;

	case ECSRD_DestroyDoor:
		if (pData->RoleData.byDestroyDoor < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byDestroyDoor += (BYTE)dwData;
		break;

	case ECSRD_DestroyMaterial:
		if (pData->RoleData.byDestroyMaterial < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byDestroyMaterial += (BYTE)dwData;
		break;

	case ECSRD_UseRepairStone:
		if (pData->RoleData.byUseRepairStone < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byUseRepairStone += (BYTE)dwData;
		break;

	case ECSRD_OpenYinYang:
		if (pData->RoleData.byOpenYinYang < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byOpenYinYang += (BYTE)dwData;
		break;

	case ECSRD_OpenLongZhu:
		if (pData->RoleData.byOpenLongZhu < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byOpenLongZhu += (BYTE)dwData;
		break;
	case ECSRD_GuildExploit:			// ���ɹ�ѫ
		pData->nExploit += (BYTE)dwData;
		break;
	case ECSRD_GuildContribute:			// ���ɹ���
		pData->nContribute += (BYTE)dwData;
		break;
	case ECSRD_GuildMoney:				// ��Ǯ����
		pData->nMoney += (BYTE)dwData;
		break;

	default:

		break;
	}

	pData->RoleData.nScore = pData->RoleData.byActiveTime * 2 + pData->RoleData.byKills + pData->RoleData.byDestroyDoor * 10
								+ pData->RoleData.byDestroyMaterial * 3 + pData->RoleData.byUseRepairStone + pData->RoleData.byOpenYinYang * 5
								+ pData->RoleData.byOpenLongZhu * 20;
}

//--------------------------------------------------------------------
// ��ս��һ�������
//--------------------------------------------------------------------
VOID CityStruggle::SortByScore()
{
	// ����ϴ�����
	m_vecScore.clear();

	// ��ʼ����
	m_mapRoleData.ResetIterator();
	DWORD dwRoleID = GT_INVALID;
	tagCSRoleData* pData = NULL;
	while (m_mapRoleData.PeekNext(dwRoleID, pData))
	{
		if (P_VALID(pData))
		{
			// �ŵ�����������
			m_vecScore.push_back(make_pair(dwRoleID, pData->RoleData.nScore));
		}
	}

	sort(m_vecScore.begin(), m_vecScore.end(), CSScoreRank());
}
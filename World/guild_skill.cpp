//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_guild.cpp
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: ���ɼ���
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "guild.h"
#include "guild_skill.h"
#include "att_res.h"
#include "role.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_guild.h"
#include "MsgInfoMgr.h"

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
GuildSkill::GuildSkill()
{
	m_bInit			= FALSE;
	m_pGuild		= NULL;
	m_dwCurSkill	= GT_INVALID;
	m_mapGuildSkill.Clear();
}

GuildSkill::~GuildSkill()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------------------
VOID GuildSkill::Init( Guild* pGuild, BOOL bRequest /*= FALSE*/ )
{
	ASSERT(P_VALID(pGuild));
	if (!P_VALID(pGuild))
	{
		return;
	}

	m_bInit			= FALSE;
	m_pGuild		= pGuild;
	m_dwCurSkill	= GT_INVALID;

	m_mapGuildSkill.Clear();

	// �������ݿ�����
	if (bRequest)
	{
		tagNDBC_LoadGuildSkillInfo send;
		send.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
		g_dbSession.Send(&send, send.dwSize);
	}
}

VOID GuildSkill::Destroy()
{
	m_bInit			= FALSE;
	m_pGuild		= NULL;
	m_dwCurSkill	= GT_INVALID;
	
	tagGuildSkill* pSkillInfo = NULL;
	MapGuildSkill::TMapIterator iter = m_mapGuildSkill.Begin();
	while (m_mapGuildSkill.PeekNext(iter, pSkillInfo))
	{
		SAFE_DEL(pSkillInfo);
	}
	m_mapGuildSkill.Clear();
}

//-----------------------------------------------------------------------------
// ������ɼ�����Ϣ
//-----------------------------------------------------------------------------
BOOL GuildSkill::LoadGuildSkillIInfo( tagGuildSkillInfo* pSkillInfo, INT nSkillNum )
{
	if (!P_VALID(pSkillInfo) || nSkillNum <= 0)
	{
		return FALSE;
	}

	if (!g_attRes.LoadGuildSkillInfo(m_mapGuildSkill))
		return FALSE;

	// �ϲ㱣֤pSkillInfo�Ĵ�С
	for (int n=0; n<nSkillNum; n++)
	{
		// ��ȡ���ɼ��ܾ�̬����
		tagGuildSkill* pGuildSkill	= m_mapGuildSkill.Peek(pSkillInfo[n].dwSkillID);
		if (!P_VALID(pGuildSkill))
			continue;

		if (!g_attRes.GetGuildSkillInfo(pSkillInfo[n].dwSkillID, pSkillInfo[n].nLevel, *pGuildSkill))
		{
			// �����ж�
			if (pSkillInfo[n].nLevel > g_attRes.GetGuildSkillMaxLvl(pSkillInfo[n].dwSkillID))
				pGuildSkill->bActive = pSkillInfo[n].bActive;
			continue;
		}
		pGuildSkill->n16Progress	= pSkillInfo[n].n16Progress;
		pGuildSkill->bActive		= pSkillInfo[n].bActive;

		// ���õ�ǰ�о�����
		if (pSkillInfo[n].bResearching)
		{
			if (!GT_VALID(m_dwCurSkill))
			{
				m_dwCurSkill = pSkillInfo[n].dwSkillID;
			}
			else if (pSkillInfo[n].dwSkillID != m_dwCurSkill)
			{
				// ���ݿ����д���,����֮
				ChangeResearchSkill(pSkillInfo[n].dwSkillID);
				m_dwCurSkill = pSkillInfo[n].dwSkillID;
			}
		}
	}

	// ��ʼ�����
	m_bInit = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ȡ���а��ɼ�����Ϣ
//-----------------------------------------------------------------------------
DWORD GuildSkill::GetGuildSkillInfo( DWORD& dwSkillID, INT16& n16Progress, INT& nSkillNum, tagGuildSkills* pLevelInfo )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// �ϲ㱣֤pLevelInfo��С
	if (!P_VALID(pLevelInfo))
	{
		return GT_INVALID;
	}
	tagGuildSkill* pGuildSkill	= NULL;
	nSkillNum					= 0;
	dwSkillID					= GT_INVALID;
	n16Progress					= GT_INVALID;

	// ȡ�õ�ǰ�о�����
	pGuildSkill = m_mapGuildSkill.Peek(m_dwCurSkill);
	if (P_VALID(pGuildSkill))
	{
		dwSkillID		= m_dwCurSkill * 100 + pGuildSkill->nLevel;
		n16Progress		= pGuildSkill->n16Progress;
	}
	
	MapGuildSkill::TMapIterator iter = m_mapGuildSkill.Begin();
	while (m_mapGuildSkill.PeekNext(iter, pGuildSkill))
	{
		if (!P_VALID(pGuildSkill))
		{
			continue;
		}

		pLevelInfo[nSkillNum].dwSkillID = pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
		pLevelInfo[nSkillNum].bActive = pGuildSkill->bActive;
		nSkillNum++;
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD GuildSkill::SkillUpgradeOnClock()
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// ȡ�õ����о�����
	if (!GT_VALID(m_dwCurSkill))
	{
		return E_GuildSkill_NotResearch;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(m_dwCurSkill);
	if (!P_VALID(pGuildSkill))
	{
		return E_GuildSkill_NotExist;
	}

	// ���ܴﵽ�ȼ�����
	if (pGuildSkill->nLevel > g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID))
	{
		return E_GuildSkill_Level_Limit;
	}

	// ����״̬�ж�
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Chaos))
	{
		return E_Guild_State_Limit;
	}

	FLOAT fModify = 1.0f;
	if (m_pGuild->IsInGuildState(EGDSS_Refulgence))
	{
		fModify = 1.5f;
	}
	else if (m_pGuild->IsInGuildState(EGDSS_Primacy))
	{
		fModify = 2.0f;
	}

	// �����ʽ��ʲ�����
	const tagGuild& sAtt = m_pGuild->GetGuildAtt();
	if (sAtt.nFund < pGuildSkill->nResearchFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (sAtt.nMaterial < pGuildSkill->nResearchMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// �۳�������Դ
	m_pGuild->DecGuildFund(GT_INVALID, pGuildSkill->nResearchFund, ELCLD_Guild_SkillResearch);
	m_pGuild->DecGuildMaterial(GT_INVALID, pGuildSkill->nResearchMaterial, ELCLD_Guild_SkillResearch);

	// ����������(�о�����������=���ɵȼ���10��[1+����ǰ������-5000��/10000])
	INT16 n16Upgrade = INT16(sAtt.byLevel * 10.0f * (1.0f + (sAtt.n16Peace - 5000.0f) / 10000.0f) * fModify);

	pGuildSkill->n16Progress += n16Upgrade;
	if (pGuildSkill->n16Progress >= pGuildSkill->n16Fulfill)
	{
		// ȡ�õȼ�����
		INT nMaxLevel = g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID);

		pGuildSkill->nLevel++;
		if (pGuildSkill->nLevel > nMaxLevel)
		{
			// �ﵽ��������
			m_dwCurSkill				= GT_INVALID;
			pGuildSkill->n16Progress	= 0;
			ChangeResearchSkill(GT_INVALID);

			// ֪ͨ�ͻ���
			tagNS_SetResearchSkill send;
			send.dwSkillID		= GT_INVALID;
			send.n16Progress	= 0;
			m_pGuild->SendGuildMsg(&send, send.dwSize);
		}
		else
		{
			INT16 n16Fulfill = pGuildSkill->n16Fulfill;

			// ��ȡ��һ�����ɼ��ܾ�̬����
			g_attRes.GetGuildSkillInfo(pGuildSkill->dwSkillID, pGuildSkill->nLevel, *pGuildSkill);

			pGuildSkill->n16Progress -= n16Fulfill;
		}

		// ���ɹ㲥
		tagNS_GuildSkillLevelUp send_broad;
		send_broad.dwSkillID = pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
		m_pGuild->SendGuildMsg(&send_broad, send_broad.dwSize);
	}

	// ���浽���ݿ�
	SaveSkillInfo2DB(pGuildSkill->dwSkillID);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �Ͻ���Ʒ����
//-----------------------------------------------------------------------------
DWORD GuildSkill::SkillUpgradeByHandIn( Role* pRole, INT64 n64Serial, DWORD& dwSkillID, INT16& n16Progress )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// ���Ȩ���ж�
	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}
// 	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bAdvSkill)
// 	{
// 		return E_Guild_Power_NotEnough;
// 	}

	// ȡ�õ����о�����
	if (!GT_VALID(m_dwCurSkill))
	{
		return E_GuildSkill_NotResearch;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(m_dwCurSkill);
	if (!P_VALID(pGuildSkill))
	{
		return E_GuildSkill_NotExist;
	}

	// ���ܴﵽ�ȼ�����(�������)
	if (pGuildSkill->nLevel > g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID))
	{
		return E_GuildSkill_Level_Limit;
	}

	// �Ͻ���Ʒ�ж�
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(n64Serial);
	if (!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_GuildSkillBook)
	{
		return E_GuildSkill_Wrong_Item;
	}

	// ����״̬�ж�
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Chaos))
	{
		return E_Guild_State_Limit;
	}

	FLOAT fModify = 1.0f;
	if (m_pGuild->IsInGuildState(EGDSS_Refulgence))
	{
		fModify = 1.5f;
	}
	else if (m_pGuild->IsInGuildState(EGDSS_Primacy))
	{
		fModify = 2.0f;
	}

	// �����ʽ��ʲ�����
	const tagGuild& sAtt = m_pGuild->GetGuildAtt();
	if (sAtt.nFund < pGuildSkill->nResearchFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (sAtt.nMaterial < pGuildSkill->nResearchMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// �۳�������Դ
	m_pGuild->DecGuildFund(pRole->GetID(), pGuildSkill->nResearchFund, ELCLD_Guild_SkillResearch);
	m_pGuild->DecGuildMaterial(pRole->GetID(), pGuildSkill->nResearchMaterial, ELCLD_Guild_SkillResearch);

	// ����������
	INT16 n16Upgrade = INT16(pItem->pProtoType->nSpecFuncVal1 * fModify);

	pGuildSkill->n16Progress += n16Upgrade;
	if (pGuildSkill->n16Progress >= pGuildSkill->n16Fulfill)
	{
		// ȡ�õȼ�����
		INT nMaxLevel = g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID);

		pGuildSkill->nLevel++;
		if (pGuildSkill->nLevel > nMaxLevel)
		{
			// �ﵽ��������
			m_dwCurSkill				= GT_INVALID;
			pGuildSkill->n16Progress	= 0;
			ChangeResearchSkill(GT_INVALID);

			// ֪ͨ�ͻ���
			tagNS_SetResearchSkill send;
			send.dwSkillID		= GT_INVALID;
			send.n16Progress	= 0;
			m_pGuild->SendGuildMsg(&send, send.dwSize);
		}
		else
		{
			INT16 n16Fulfill = pGuildSkill->n16Fulfill;

			// ��ȡ��һ�����ɼ��ܾ�̬����
			g_attRes.GetGuildSkillInfo(pGuildSkill->dwSkillID, pGuildSkill->nLevel, *pGuildSkill);

			pGuildSkill->n16Progress -= n16Fulfill;
		}

		// ���ɹ㲥
		tagNS_GuildSkillLevelUp send_broad;
		send_broad.dwSkillID = pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
		m_pGuild->SendGuildMsg(&send_broad, send_broad.dwSize);
	}

	// �㲥֪ͨ���й����Ա
	DWORD dwMsgInfoID	= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
	DWORD dwTmpNum		= 100053; // ��ҡ�<p1>���Ͻ��ˡ�<p2>����Ϊ�о��еĹ��Ἴ����ש���ߣ�
	DWORD dwRoleID		= pRole->GetID();
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ItemTypeID, &(pItem->dwTypeID));
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, m_pGuild);

	// ���浽���ݿ�
	SaveSkillInfo2DB(pGuildSkill->dwSkillID);

	// ������ҹ���ֵ
	m_pGuild->ChangeMemberContribution(pRole->GetID(), pItem->pProtoType->nSpecFuncVal2, TRUE);

	// �۳������Ʒ
	pRole->GetItemMgr().RemoveFromRole(pItem->dwTypeID, 1, ELCLD_Guild_SkillResearch);

	// ���ô�������
	dwSkillID	= pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
	n16Progress	= pGuildSkill->n16Progress;

	return E_Success;
}

//-----------------------------------------------------------------------------
// ѧϰ����
//-----------------------------------------------------------------------------
DWORD GuildSkill::LearnGuildSkill( Role* pRole, DWORD dwSkillID, INT& nLevel, BOOL& bLearnError )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// �ж����
	if (!P_VALID(pRole) || !GT_VALID(dwSkillID))
	{
		return GT_INVALID;
	}

	bLearnError	= FALSE;

	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}

	// �ж�ָ������
	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
	if (!P_VALID(pGuildSkill))
	{
		return E_GuildSkill_NotExist;
	}

	// �ж���ѧ����
	BOOL bLearn = TRUE;
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (P_VALID(pSkill))
	{
		bLearn = FALSE;		// ��������
		if (pGuildSkill->nLevel <= pSkill->GetLevel()+1)
		{
			return E_GuildSkill_Level_Limit;
		}
	}

	// �жϰ���״̬
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Warfare))
	{
		return E_Guild_State_Limit;
	}

	// ȡ�ð��ɼ�������
	const tagGuildSkill* pSkillProto = NULL;
	if (P_VALID(pSkill))
	{
		pSkillProto = g_attRes.GetGuildSkillProto(pSkill->GetTypeID() + 1);
	}
	else
	{
		pSkillProto = g_attRes.GetGuildSkillProto(Skill::CreateTypeID(dwSkillID, 1));
	}
	
	if (!P_VALID(pSkillProto))
	{
		return E_GuildSkill_Level_Limit;
	}

	// ���ѧϰ���Ἴ�ܵȼ�����
	if (pRole->GetLevel() < pSkillProto->n16LearnRestrictLvl)
	{
		return E_GuildSkill_Learn_Lvl_Limit;
	}

	// �����������
	if (pRole->GetCurMgr().GetBagSilver() < pSkillProto->nLearnSilver)
	{
		return E_BagSilver_NotEnough;
	}
	if (pMember->nContribution < pSkillProto->nLearnContribution)
	{
		return E_GuildMember_Contribution_Limit;
	}
	if(m_pGuild->GetGuildAtt().nFund < pSkillProto->nLearnFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (m_pGuild->GetGuildAtt().nMaterial < pSkillProto->nLearnMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// ѧϰ������������
	bLearnError			= TRUE;
	DWORD dwErrorCode	= GT_INVALID;
	if (bLearn)
	{
		dwErrorCode = pRole->LearnSkill(dwSkillID);
	}
	else
	{
		dwErrorCode = pRole->LevelUpSkill(dwSkillID);
	}

	if (E_Success == dwErrorCode)
	{
		// �۳�����
		pRole->GetCurMgr().DecBagSilver(pSkillProto->nLearnSilver, ELCLD_Guild_SkillLearn);
		m_pGuild->ChangeMemberContribution(pRole->GetID(), pSkillProto->nLearnContribution, FALSE);
// 		m_pGuild->DecGuildFund(pRole->GetID(), pSkillProto->nLearnFund, ELCLD_Guild_SkillLearn);
// 		m_pGuild->DecGuildMaterial(pRole->GetID(), pSkillProto->nLearnMaterial, ELCLD_Guild_SkillLearn);
	}

	// ���ô�������
	pSkill = pRole->GetSkill(dwSkillID);
	if (P_VALID(pSkill))
	{
		nLevel	= pSkill->GetLevel();
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �������ɼ���
//-----------------------------------------------------------------------------
VOID GuildSkill::CreateGuildSkill()
{
	// ��ȡ��ʼ���ɼ�����Ϣ
	g_attRes.LoadGuildSkillInfo(m_mapGuildSkill);

	// ���õ�ǰ���о�����
	m_dwCurSkill = GT_INVALID;

	tagGuildSkill* pSkillInfo = NULL;
	MapGuildSkill::TMapIterator iter = m_mapGuildSkill.Begin();
	while (m_mapGuildSkill.PeekNext(iter, pSkillInfo))
	{
		if (!P_VALID(pSkillInfo))
		{
			continue;
		}
		
		// �����ݿⷢ��1������
		tagNDBC_CreateGuildSkill send;
		send.dwGuildID					= m_pGuild->GetGuildAtt().dwID;
		send.sSkillInfo.nLevel			= pSkillInfo->nLevel;
		send.sSkillInfo.dwSkillID		= pSkillInfo->dwSkillID;
		send.sSkillInfo.n16Progress		= pSkillInfo->n16Progress;
		send.sSkillInfo.bResearching	= FALSE;
		send.sSkillInfo.bActive		= pSkillInfo->bActive;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ��ʼ�����
	m_bInit = TRUE;
}

//-----------------------------------------------------------------------------
// ���õ�ǰ��������
//-----------------------------------------------------------------------------
DWORD GuildSkill::SetCurUpgradeGuildSkill( DWORD dwRoleID, DWORD dwSkillID, INT& nLevel, INT16& n16Progress )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	if (!GT_VALID(dwRoleID))
	{
		return GT_INVALID;
	}

	// ͬ���ļ��ܲ�������
	if (dwSkillID == m_dwCurSkill)
	{
		return GT_INVALID;
	}

	// �ж����Ȩ��
	tagGuildMember* pMember = m_pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}
	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bSetSkill)
	{
		return E_Guild_Power_NotEnough;
	}

	// �жϼ���
	if (GT_VALID(dwSkillID))
	{
		tagGuildSkill* pGuildSkill = NULL;
		MapGuildSkill::TMapIterator iterator = m_mapGuildSkill.Begin();
		BOOL bFindSkill = FALSE;

		while(m_mapGuildSkill.PeekNext(iterator, pGuildSkill))
		{
			if (P_VALID(pGuildSkill) && pGuildSkill->dwSkillID == dwSkillID)
			{
				bFindSkill = TRUE; 
				break;
			}
		}
		if (!P_VALID(pGuildSkill) || !bFindSkill)
		{
			return E_GuildSkill_NotExist;
		}
		pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
		if (P_VALID(pGuildSkill) && pGuildSkill->n16ResearchRestrictLvl > m_pGuild->GetGuildFacilities().GetAcademyLevel())
		{
			return E_GuildSkill_Academy_Lvl_Limit;
		}
		if (pGuildSkill->bActive == FALSE)
		{
			return E_SystemError;
		}
		if (IsMaxSkillLevel(pGuildSkill->dwSkillID, pGuildSkill->nLevel))
		{
			return E_GuildSkill_Level_Limit;
		}
		n16Progress	= pGuildSkill->n16Progress;
		nLevel		= pGuildSkill->nLevel;

		// ��ȡ���ܾ�̬����
		g_attRes.GetGuildSkillInfo(pGuildSkill->dwSkillID, pGuildSkill->nLevel, *pGuildSkill);
	}

	// ���õ�ǰ��������
	ChangeResearchSkill(dwSkillID);
	m_dwCurSkill	= dwSkillID;

	return E_Success;
}

BOOL GuildSkill::IsMaxSkillLevel(DWORD dwSkillID, INT nLevel)
{
	INT nMaxLvl = g_attRes.GetGuildSkillMaxLvl(dwSkillID);
	if (nMaxLvl <= GT_INVALID)
	{
		return GT_INVALID;
	}
	if (nMaxLvl < nLevel)
	{
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// �����ݿ�ͨѶ
//-----------------------------------------------------------------------------
VOID GuildSkill::SaveSkillInfo2DB(DWORD dwSkillID)
{
	if (!GT_VALID(dwSkillID))
	{
		return;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
	if (!P_VALID(pGuildSkill))
	{
		return;
	}

	tagNDBC_SaveGuildSkill send;
	send.dwGuildID				= m_pGuild->GetGuildAtt().dwID;
	send.sSkillInfo.nLevel		= pGuildSkill->nLevel;
	send.sSkillInfo.dwSkillID	= pGuildSkill->dwSkillID;
	send.sSkillInfo.n16Progress	= pGuildSkill->n16Progress;
	send.sSkillInfo.bActive		= pGuildSkill->bActive;

	g_dbSession.Send(&send, send.dwSize);
}

VOID GuildSkill::ChangeResearchSkill( DWORD dwNewSkillID )
{
	tagNDBC_ChangeResearchSkill send;
	send.dwGuildID		= m_pGuild->GetGuildAtt().dwID;
	send.dwNewSkillID	= dwNewSkillID;

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// ������а��ɼ�����Ϣ
//-----------------------------------------------------------------------------
VOID GuildSkill::RemoveGuildSkillInfo()
{
	if (!m_bInit)
	{
		return;
	}

	tagNDBC_RemoveGuildSkill send;
	send.dwGuildID = m_pGuild->GetGuildAtt().dwID;

	g_dbSession.Send(&send, send.dwSize);
}

// ����Ἴ��
DWORD GuildSkill::ActiveGuildSkill(DWORD dwRoleID, INT64 n64ItemID, DWORD dwGuildID, DWORD dwSkillID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (!m_bInit)
	{
		return GT_INVALID;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
	if (!P_VALID(pGuildSkill))
	{
		return GT_INVALID;
	}

	// ���ж���û�й��Ἴ�ܼ������
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(n64ItemID);
	if (P_VALID(pItem) && P_VALID(pItem->pProtoType) && pItem->pProtoType->eSpecFunc == EISF_GuildSkillActive)
	{
		if (P_VALID(pItem->pProtoType) && pItem->pProtoType->nSpecFuncVal1 == dwSkillID)
			return GT_INVALID;

		if (pGuildSkill->bActive)
			return E_GuildSkill_Already_Active;

		// �۳���Ч�Ĺ��Ἴ���飬����Ӧ���ܡ��Ƿ��Ѽ���ֶα��Ϊ���о�״̬
		pGuildSkill->bActive = TRUE;
		SaveSkillInfo2DB(pGuildSkill->dwSkillID);
		pRole->GetItemMgr().DelFromBag(n64ItemID, ELCLD_Guild_Skill_Active, 1);
		return E_Success;
	}
	else
	{
		return E_GuildSkill_ActiveItem_Not_Exist;
	}
}

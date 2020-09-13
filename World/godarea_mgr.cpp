//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.cpp
// author: liutingting
// actor:
// data: 2011-01-19
// last:
// brief: ��ְ����������ݲ���
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "godarea_mgr.h"
#include "../WorldDefine/clergy_define.h"
#include "Role.h"
#define	AREA_CD	10*60 //����CDʱ�䣨�룩
//-------------------------------------------------
AreaMgr::AreaMgr(Role *pRole)
{
	m_pRole = pRole;
	m_dwLastTime = timeGetTime();
	m_dwCurTime = m_dwLastTime;
}
//-------------------------------------------------
AreaMgr::~AreaMgr()
{
	m_mapGodArea.Clear();
	//m_mapOptionalArea.Clear();
	//m_mapRareArea.Clear();

	MapAreaInfo::TMapIterator it = m_mapOptionalArea.Begin();
	tagAreaInfo* pAreaInfo = NULL;

	while( m_mapOptionalArea.PeekNext(it, pAreaInfo) )
	{
		SAFE_DEL(pAreaInfo);
	}
	m_mapOptionalArea.Clear();

	it = m_mapRareArea.Begin();
	pAreaInfo = NULL;

	while( m_mapRareArea.PeekNext(it, pAreaInfo) )
	{
		SAFE_DEL(pAreaInfo);
	}
	m_mapRareArea.Clear();
}

//--------------------------------------------------
//�Ƿ���Լ������������
DWORD AreaMgr::CanActivationAreaAbility( DWORD dwAreaID, BOOL bByCD/* = TRUE*/ )
{
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	switch(pGodAreaProto->eType)
	{
	case EGAT_GOD:	//��ְ����
		{
			return E_Success;
		}
		break;
	case EGAT_OPTIONAL:	//��ѡ����
	case EGAT_RARE:		//ϡ������
		{
			if(!P_VALID(m_pRole))
				return E_SystemError;

			//�Ƿ��и�����
			if(EGAT_OPTIONAL == pGodAreaProto->eType)
			{
				if( !m_mapOptionalArea.IsExist(dwAreaID) )
				{
					return E_God_Area_None;
				}
				else
				{//CD�ж�
					if(bByCD)
					{
						DWORD dwCD = GetAreaCD(dwAreaID);
						if( dwCD > 0 )
							return E_God_Area_CD;
					}
				}
			}

			if(EGAT_RARE == pGodAreaProto->eType)
			{
				if( !m_mapRareArea.IsExist(dwAreaID) )
				{
					return E_God_Area_None;
				}
				else
				{//CD�ж�
					if(bByCD)
					{
						DWORD dwCD = GetAreaCD(dwAreaID);
						if( dwCD > 0 )
							return E_God_Area_CD;
					}
				}
			}

			//�����Ƿ񵽴�����
			INT nGodHead = m_pRole->GetAttValue(ERA_God_Godhead);
			const tagGodHeadProto *pGodHeadProto = g_attRes.GetOneGodHeadProto(nGodHead);
			if( !P_VALID(pGodHeadProto) )
			{
				return E_SystemError;
			}

			if( GetAbilityAreaNum(EGAT_OPTIONAL,FALSE) + GetAbilityAreaNum(EGAT_RARE,FALSE) >= pGodHeadProto->nCanActNum )
			{
				return E_God_Area_Max;
			}

			//��ɫ�ȼ�Ҫ��
			INT nLevel = m_pRole->GetLevel();
			if( nLevel < pGodAreaProto->nLevelLimit )
			{
				return E_God_Area_Condition;
			}


			// ���ְҵ����
			const tagSkillProto *pSkillProto = g_attRes.GetSkillProto(pGodAreaProto->dwSkillID);

			if(P_VALID(pSkillProto))
			{
				if( E_Success != m_pRole->VocationCheck( pSkillProto->dwVocationLimit ))//dwLearnVocationLimit
					return E_God_Area_Career;
			}

			//��ɫ���Ҫ��
			if( nGodHead < pGodAreaProto->nGodheadLimit )
			{
				return E_God_Area_Condition;
			}

			//ǰ������ID
			//if( pGodAreaProto->dwPreID > 0 )
			//{
			//	if( !m_mapGodArea.IsExist(pGodAreaProto->dwPreID) 
			//		&& !m_mapOptionalArea.IsExist(pGodAreaProto->dwPreID)
			//		&& !m_mapRareArea.IsExist(pGodAreaProto->dwPreID) )
			//	return E_God_Area_Condition;
			//}

			//��������
			//INT nGodFaith = m_pRole->GetAttValue(ERA_God_Faith);
			//if( nGodFaith < pGodAreaProto->nFaithValue )
			//{
			//	return E_God_Area_Condition;
			//}

			//�Ͳ߻�ȷ�ϲ���Ҫ�ж��� liutingting 2011.2.23
			//�����ϡ��������Ҫ�жϷ�������
			//if( EGAT_RARE == pGodAreaProto->eType )
			//{
			//	if(pGodAreaProto->nMaxNum)//TODO liu
			//	{
			//		return E_God_Area_Condition;
			//	}
			//}

			return E_Success;
		}
		break;
	}

	return E_SystemError;
}

//--------------------------------------------------
//�ı���������
DWORD AreaMgr::ChangeAreaAbility( DWORD dwAreaID, BOOL bActive, BOOL bByCD/* = TRUE*/)
{
	if(bActive)
	{
		return ActivationAreaAbility(dwAreaID,bByCD);
	}
	else
	{
		return CancleAreaAbility(dwAreaID,bByCD);
	}
}
//--------------------------------------------------
//������������
DWORD AreaMgr::ActivationAreaAbility( DWORD dwAreaID, BOOL bByCD/* = TRUE*/)
{
	if(!P_VALID(m_pRole))
		return E_SystemError;

	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	//�жϿɷ񼤻���������
	DWORD dwRet = CanActivationAreaAbility(dwAreaID,bByCD);
	if( E_Success != dwRet )
	{
		return dwRet;
	}

	//�۳�����,�����ȿ۳�
	//INT nGodFaith = m_pRole->GetAttValue(ERA_God_Faith);
	//if( pGodAreaProto->nFaithValue > nGodFaith )
	//	return E_SystemError;

	//m_pRole->ModAttValue(ERA_God_Faith, -pGodAreaProto->nFaithValue);

	switch(pGodAreaProto->eNatureType)
	{
	case EGANT_SKILL:
		{//���Ӽ���
			const tagSkillProto *pSkillProto = g_attRes.GetSkillProto(pGodAreaProto->dwSkillID);
			if(!P_VALID(pSkillProto))
			{
				//�����۳�������
				//m_pRole->ModAttModValue(ERA_God_Faith, pGodAreaProto->nFaithValue);
				return E_SystemError;
			}

			m_pRole->AddSkillByArea(pGodAreaProto->dwSkillID);
			//if( E_Success != dwRet )
			//{
			//	//�����۳�������
			//	m_pRole->ModAttModValue(ERA_God_Faith, pGodAreaProto->nFaithValue);
			//	return dwRet;
			//}
		}
		break;
	case EGANT_BUFF:
		{//����BUFF
			m_pRole->TryAddBuffToMyself(pGodAreaProto->dwBuffID);
		}
		break;
	case EGANT_ALL:
		{
			m_pRole->AddSkillByArea(pGodAreaProto->dwSkillID);
			m_pRole->TryAddBuffToMyself(pGodAreaProto->dwBuffID);
		}
		break;
	default:
		return E_SystemError;
	}

	//����
	if(EGAT_OPTIONAL == pGodAreaProto->eType)
	{
		tagAreaInfo *pAreaInfo = m_mapOptionalArea.Peek(dwAreaID);
		if( !P_VALID(pAreaInfo) )
			return E_SystemError;

		pAreaInfo->bActive = TRUE;
		pAreaInfo->dwCD = AREA_CD;

		g_ScriptMgr.GetRoleScript()->OnRoleActivateArea(m_pRole,dwAreaID);
	}

	if(EGAT_RARE == pGodAreaProto->eType)
	{
		tagAreaInfo *pAreaInfo = m_mapRareArea.Peek(dwAreaID);
		if( !P_VALID(pAreaInfo) )
			return E_SystemError;

		pAreaInfo->bActive = TRUE;
		pAreaInfo->dwCD = AREA_CD;
	}

	//tagNS_CondensedChange msg;
	//msg.dwBelief		= m_pRole->GetAttValue(ERA_God_Faith);
	//msg.dwCondensedVal	= m_pRole->GetAttValue(ERA_God_Condenced);
	//m_pRole->SendMessage(&msg,msg.dwSize);


	//DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (2 - 1);
	//MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

	//pSend->dwSize = dwSize;
	//pSend->nNum = 2;

	//pSend->value[0].eType = ERA_God_Faith;
	//pSend->value[0].nValue = msg.dwBelief;

	//pSend->value[1].eType = ERA_God_Condenced;
	//pSend->value[1].nValue = msg.dwCondensedVal;

	//m_pRole->SendMessage(pSend, pSend->dwSize);

	//MDEL_MSG(pSend);

	return E_Success;
}
//--------------------------------------------------
DWORD AreaMgr::CanCancleAreaAbility(DWORD dwAreaID, BOOL bByCD/* = TRUE*/)
{
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	switch(pGodAreaProto->eType)
	{
	case EGAT_GOD:	//��ְ����
		{
			return E_Success;
		}
		break;
	case EGAT_OPTIONAL:	//��ѡ����
	case EGAT_RARE:		//ϡ������
		{
			if(!P_VALID(m_pRole))
				return E_SystemError;

			//�Ƿ��и�����
			if(EGAT_OPTIONAL == pGodAreaProto->eType)
			{
				//CD�ж�
				if(bByCD)
				{
					DWORD dwCD = GetAreaCD(dwAreaID);
					if( dwCD > 0 )
						return E_God_Area_CD;
				}
			}

			if(EGAT_RARE == pGodAreaProto->eType)
			{
				//CD�ж�
				if(bByCD)
				{
					DWORD dwCD = GetAreaCD(dwAreaID);
					if( dwCD > 0 )
						return E_God_Area_CD;
				}
			}
		}

	}

	return E_Success;
}
//--------------------------------------------------
//ȡ����������
DWORD AreaMgr::CancleAreaAbility( DWORD dwAreaID, BOOL bByCD/* = TRUE*/)
{
	if(!P_VALID(m_pRole))
		return E_SystemError;

	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	//�жϿɷ�ȡ����������
	DWORD dwRet = CanCancleAreaAbility(dwAreaID,bByCD);
	if( E_Success != dwRet )
	{
		return dwRet;
	}

	switch(pGodAreaProto->eNatureType)
	{
	case EGANT_SKILL:
		{//���ټ���
			m_pRole->RemoveSkill(Skill::GetIDFromTypeID(pGodAreaProto->dwSkillID));
		}
		break;
	case EGANT_BUFF:
		{//����BUFF
			m_pRole->RemoveBuff(Buff::GetIDFromTypeID(pGodAreaProto->dwBuffID), FALSE);
		}
		break;
	case EGANT_ALL:
		{
			m_pRole->RemoveSkill(Skill::GetIDFromTypeID(pGodAreaProto->dwSkillID));
			m_pRole->RemoveBuff(Buff::GetIDFromTypeID(pGodAreaProto->dwBuffID), FALSE);
		}
		break;
	}

	//ȡ��
	if(EGAT_OPTIONAL == pGodAreaProto->eType)
	{
		tagAreaInfo *pAreaInfo = m_mapOptionalArea.Peek(dwAreaID);
		if( !P_VALID(pAreaInfo) )
			return E_SystemError;

		pAreaInfo->bActive = FALSE;
		pAreaInfo->dwCD = AREA_CD;
	}

	if(EGAT_RARE == pGodAreaProto->eType)
	{
		tagAreaInfo *pAreaInfo = m_mapRareArea.Peek(dwAreaID);
		if( !P_VALID(pAreaInfo) )
			return E_SystemError;

		pAreaInfo->bActive = FALSE;
		pAreaInfo->dwCD = AREA_CD;
	}

	m_pRole->ReCalRoleAtt();

	return E_Success;
}
//-------------------------------------------------
//�ı�����
DWORD AreaMgr::ChangeArea( DWORD dwAreaID, BOOL bAdd, tagGlobalLimitedItem *pGlobalLimitedItem/*=NULL*/, BOOL bDelGodFaith/*=TRUE*/, BOOL bJudgePre/*=TRUE*/ )
{
	if(bAdd)
	{
		return AddArea(dwAreaID,pGlobalLimitedItem,bDelGodFaith,bJudgePre);
	}
	else
	{
		return DelArea(dwAreaID);
	}
}

//-------------------------------------------------
//�����ְ����
VOID AreaMgr::ClearClergyArea()
{
	if(!P_VALID(m_pRole))
		return;

	if (m_mapGodArea.Empty())
	{
		return;
	}

	DWORD dwAreaID = 0;
	BOOL bFlag = FALSE;
	m_mapGodArea.ResetIterator();
	while(m_mapGodArea.PeekNext(dwAreaID, bFlag))
	{
		if( m_mapGodArea.IsExist(dwAreaID) )
		{
			//��ְ����,�Զ�ȡ����������
			DelArea(dwAreaID);

			g_dbSession.SendDeleteArea2DB(m_pRole->GetID(),dwAreaID);
		}
	}
}

//-------------------------------------------------
//��������
DWORD AreaMgr::AddArea( DWORD dwAreaID, tagGlobalLimitedItem *pGlobalLimitedItem/*=NULL*/, BOOL bDelGodFaith/*=TRUE*/, BOOL bJudgePre/*=TRUE*/ )
{
	if(!P_VALID(m_pRole))
		return E_SystemError;

	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	//
	if(bJudgePre)
	{
		DWORD dwRet = CanAddArea(dwAreaID);
		if(E_Success != dwRet)
			return dwRet;
	}

	//�ж�����
	if( bDelGodFaith )
	{
		INT nGodFaith = m_pRole->GetAttValue(ERA_God_Faith);
		if( pGodAreaProto->nFaithValue > nGodFaith )
			return E_God_Area_FaithValue;
	}

	BOOL bRet = FALSE; 
	switch(pGodAreaProto->eType)
	{
	case EGAT_GOD:		//��ְ����Ĭ�ϼ�����������
		{
			if( !m_mapGodArea.IsExist(dwAreaID) )
			{
				m_mapGodArea.Add(dwAreaID,TRUE);
				//��ְ����,�Զ�������������
				ActivationAreaAbility(dwAreaID);

				bRet = TRUE;
			}
		}
		break;
	case EGAT_OPTIONAL:	//��ѡ����Ĭ�ϲ�������������
		{
			DelSameArea(dwAreaID);

			if( !m_mapOptionalArea.IsExist(dwAreaID) )
			{
				tagAreaInfo *pAreaInfo = new tagAreaInfo;
				if(P_VALID(!pAreaInfo))
					return E_SystemError;

				pAreaInfo->bActive = FALSE;
				pAreaInfo->dwAreaID = dwAreaID;
				pAreaInfo->dwCD = 0;

				m_mapOptionalArea.Add(dwAreaID,pAreaInfo);	

				bRet = TRUE;
			}
		}
		break;
	case EGAT_RARE:		//ϡ������Ĭ�ϲ�������������
		{
			DelSameArea(dwAreaID);

			if( !m_mapRareArea.IsExist(dwAreaID) )
			{
				tagAreaInfo *pAreaInfo = new tagAreaInfo;
				if(P_VALID(!pAreaInfo))
					return E_SystemError;

				pAreaInfo->bActive = FALSE;
				pAreaInfo->dwAreaID = dwAreaID;
				pAreaInfo->dwCD = 0;

				INT nNumPre = m_mapRareArea.Size();
				m_mapRareArea.Add(dwAreaID,pAreaInfo);


				if( P_VALID(pGlobalLimitedItem) )
				{
					if(EGLIET_Item == pGlobalLimitedItem->eType)
						pGlobalLimitedItem->nTime = pGodAreaProto->nTime;

					pGlobalLimitedItem->eType = EGLIET_Skill;
					pGlobalLimitedItem->n64ID = m_pRole->GetID();
				}

				g_ScriptMgr.GetRoleScript()->OnRoleGetRareArea(m_pRole,dwAreaID);

				SetAreaEffect(nNumPre,m_mapRareArea.Size());

				bRet = TRUE;
			}
		}
		break;
	}

	if(bRet)
	{
		if( bDelGodFaith )
		{
			//�۳�����
			m_pRole->ModAttValue(ERA_God_Faith, -pGodAreaProto->nFaithValue);

			tagNS_CondensedChange msg;
			msg.dwBelief		= m_pRole->GetAttValue(ERA_God_Faith);
			msg.dwCondensedVal	= m_pRole->GetAttValue(ERA_God_Condenced);
			m_pRole->SendMessage(&msg,msg.dwSize);


			DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (2 - 1);
			MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->nNum = 2;

			pSend->value[0].eType = ERA_God_Faith;
			pSend->value[0].nValue = msg.dwBelief;

			pSend->value[1].eType = ERA_God_Condenced;
			pSend->value[1].nValue = msg.dwCondensedVal;

			m_pRole->SendMessage(pSend, pSend->dwSize);

			MDEL_MSG(pSend);
		}

		return E_Success;
	}
	else
	{
		//��������
		//if( bDelGodFaith )
		//{
		//	m_pRole->ModAttValue(ERA_God_Faith, pGodAreaProto->nFaithValue);
		//}
		return E_SystemError;
	}

	return E_SystemError;
}

//-------------------------------------------------
//��������
DWORD AreaMgr::DelArea( DWORD dwAreaID )
{
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	switch(pGodAreaProto->eType)
	{
	case EGAT_GOD:		//��ְ����
		{
			if( m_mapGodArea.IsExist(dwAreaID) )
			{
				m_mapGodArea.Erase(dwAreaID);
				
				//��ְ����,�Զ�ȡ����������
				CancleAreaAbility(dwAreaID,FALSE);
			}

			return E_Success;
		}
		break;
	case EGAT_OPTIONAL:	//��ѡ����
		{
			if( m_mapOptionalArea.IsExist(dwAreaID) )
			{
				tagAreaInfo *pAreaInfo = m_mapOptionalArea.Peek(dwAreaID);
				SAFE_DEL(pAreaInfo);
				m_mapOptionalArea.Erase(dwAreaID);
			}

			return E_Success;
		}
		break;
	case EGAT_RARE:		//ϡ������
		{
			if( m_mapRareArea.IsExist(dwAreaID) )
			{
				INT nNum = m_mapRareArea.Size();
				tagAreaInfo *pAreaInfo = m_mapRareArea.Peek(dwAreaID);
				SAFE_DEL(pAreaInfo);
				m_mapRareArea.Erase(dwAreaID);

				SetAreaEffect(nNum,m_mapRareArea.Size());
			}

			return E_Success;
		}
		break;
	}

	return E_SystemError;
}
//-------------------------------------------------
//����Ѽ����ĳ����������
INT	AreaMgr::GetAbilityAreaNum(EGodAreaType eType,BOOL bAll/*=TRUE*/)
{
	switch(eType)
	{
	case EGAT_GOD:		//��ְ����
		{
			return m_mapGodArea.Size();
		}
		break;
	case EGAT_OPTIONAL:	//��ѡ����
		{
			if(!bAll)
			{
				INT nNum = 0;
				MapAreaInfo::TMapIterator it = m_mapOptionalArea.Begin();
				tagAreaInfo* pAreaInfo = NULL;

				while( m_mapOptionalArea.PeekNext(it, pAreaInfo) )
				{
					if( pAreaInfo->bActive )
						nNum++;
				}
			
				return nNum;
			}
			else
			{
				return m_mapOptionalArea.Size();
			}
		}
		break;
	case EGAT_RARE:		//ϡ������
		{
			if(!bAll)
			{
				INT nNum = 0;
				MapAreaInfo::TMapIterator it = m_mapRareArea.Begin();
				tagAreaInfo* pAreaInfo = NULL;

				while( m_mapRareArea.PeekNext(it, pAreaInfo) )
				{
					if( pAreaInfo->bActive )
						nNum++;
				}
			
				return nNum;
			}
			else
			{
				return m_mapRareArea.Size();
			}
		}
		break;
	}

	return 0;
}
//-------------------------------------------------
//ֱ�������������û��������������
DWORD AreaMgr::SetArea( DWORD dwAreaID, BOOL bActive, DWORD dwCD/*=0*/ )
{
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	switch(pGodAreaProto->eType)
	{
	case EGAT_GOD:		//��ְ����
		{
			if( !m_mapGodArea.IsExist(dwAreaID) )
				m_mapGodArea.Add(dwAreaID,bActive);
		}
		break;
	case EGAT_OPTIONAL:	//��ѡ����
		{
			if( !m_mapOptionalArea.IsExist(dwAreaID) )
			{
				tagAreaInfo *pAreaInfo = new tagAreaInfo();
				if( !P_VALID(pAreaInfo) )
					return E_SystemError;
				
				pAreaInfo->dwAreaID = dwAreaID;
				pAreaInfo->bActive = bActive;
				pAreaInfo->dwCD = dwCD;

				m_mapOptionalArea.Add(dwAreaID,pAreaInfo);
			}
		}
		break;
	case EGAT_RARE:		//ϡ������
		{
			if( !m_mapRareArea.IsExist(dwAreaID) )
			{
				tagAreaInfo *pAreaInfo = new tagAreaInfo();
				if( !P_VALID(pAreaInfo) )
					return E_SystemError;
				
				INT nNum = m_mapRareArea.Size();

				pAreaInfo->dwAreaID = dwAreaID;
				pAreaInfo->bActive = bActive;
				pAreaInfo->dwCD = dwCD;

				m_mapRareArea.Add(dwAreaID,pAreaInfo);

				SetAreaEffect(nNum,m_mapRareArea.Size());
			}
		}
		break;
	}

	return E_Success;
}
//-------------------------------------------------
//�����ݿ��ʼ��
DWORD AreaMgr::InitFromDB( const BYTE* &pData, INT nNum )
{
	if(!P_VALID(pData))
		return E_SystemError;

	//InitGodArea();

	if( nNum <= 0 ) 
		return E_Success;

	const tagAreaSave* pAreaSave = (const tagAreaSave*)pData;

	for(INT n = 0; n < nNum; ++n)
	{
		SetArea( pAreaSave->dwAreaID, pAreaSave->bActivation, pAreaSave->dwCD );
		pAreaSave++;
	}

	// ����ָ��
	pData = (const BYTE*)((tagAreaSave*)pData + nNum);

	return E_Success;
}
//-------------------------------------------------
//�洢�����ݿ�
DWORD AreaMgr::Save2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	if(!P_VALID(m_pRole))
		return E_SystemError;

	INT32 i	= 0;
	tagAreaSave *pAreaSave = (tagAreaSave*)pData;

	DWORD dwRet = FALSE;
	MapAreaInfo::TMapIterator iter = m_mapOptionalArea.Begin();
	while( iter!=m_mapOptionalArea.End() )
	{
		tagAreaInfo *pAreaInfo = iter->second;
		if(P_VALID(pAreaInfo))
		{
			pAreaSave[i].dwAreaID = pAreaInfo->dwAreaID;
			pAreaSave[i].dwRoleID = m_pRole->GetID();
			pAreaSave[i].bActivation = pAreaInfo->bActive;
			pAreaSave[i].dwCD = pAreaInfo->dwCD;
			i++;
		}
		iter++;
	}

	iter = m_mapRareArea.Begin();
	while(iter!=m_mapRareArea.End())
	{
		tagAreaInfo *pAreaInfo = iter->second;
		if(P_VALID(pAreaInfo))
		{
			pAreaSave[i].dwAreaID = pAreaInfo->dwAreaID;
			pAreaSave[i].dwRoleID = m_pRole->GetID();
			pAreaSave[i].bActivation = pAreaInfo->bActive;
			pAreaSave[i].dwCD = pAreaInfo->dwCD;
			i++;
		}
		iter++;
	}

	nNum = i;
	pOutPointer = &pAreaSave[nNum];

	return E_Success; 
}
//-------------------------------------------------
//��ʼ����ְ����
DWORD AreaMgr::InitGodArea()
{
	if(!P_VALID(m_pRole))
		return E_SystemError;

	const tagClergyProto *pClergyProto = g_attRes.GetClergyProto(m_pRole->GetClergy());
	if( !P_VALID(pClergyProto) )
	{
		return E_SystemError;
	}

	for( INT8 i = 0; i < MAX_FieldNum; ++i )
	{
		if( pClergyProto->dwFieldID[i] > 0 )
		{
			const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(pClergyProto->dwFieldID[i]);
			if( !P_VALID(pGodAreaProto) )
			{
				return E_SystemError;
			}

			if(EGAT_GOD != pGodAreaProto->eType)
			{
				return E_SystemError;
			}

			SetArea(pGodAreaProto->dwID,TRUE);
		}
	}

	return E_Success;
}
//---------------------------------------------------------------------------------
// ���������ʼ������ͻ���
VOID AreaMgr::SendInitStateArea()
{
	if(!P_VALID(m_pRole))
		return;

	INT nNum1 = m_mapOptionalArea.Size();
	INT nNum2 = m_mapRareArea.Size();
	
	if( nNum1 + nNum2 <= 0 )
	{
		tagNS_GetRoleInitState_Area send;
		send.nNum = 0;

		m_pRole->SendMessage(&send, send.dwSize);
	}
	else
	{
		DWORD dwSize = sizeof(tagNS_GetRoleInitState_Area) + sizeof(tagAreaMsgInfo) * (nNum1 + nNum2 - 1);
		MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_Area);

		pSend->dwSize = dwSize;
		pSend->nNum = nNum1 + nNum2;

		tagAreaMsgInfo *pAreaMsgInfo = pSend->Area;

		INT nIndex = 0;

		MapAreaInfo::TMapIterator iter = m_mapOptionalArea.Begin();
		while( iter!=m_mapOptionalArea.End() )
		{
			tagAreaInfo *pAreaInfo = iter->second;
			if(P_VALID(pAreaInfo))
			{
				pAreaMsgInfo[nIndex].dwAreaID = pAreaInfo->dwAreaID;
				pAreaMsgInfo[nIndex].bActivation = pAreaInfo->bActive;
				pAreaMsgInfo[nIndex].dwTime = 0;
				pAreaMsgInfo[nIndex].dwCD = pAreaInfo->dwCD;
				nIndex++;
			}
			iter++;
		}

		MapAreaInfo::TMapIterator iter1 = m_mapRareArea.Begin();
		while( iter1!=m_mapRareArea.End() )
		{
			tagAreaInfo *pAreaInfo = iter1->second;
			if(P_VALID(pAreaInfo))
			{
				pAreaMsgInfo[nIndex].dwAreaID = pAreaInfo->dwAreaID;
				pAreaMsgInfo[nIndex].bActivation = pAreaInfo->bActive;
				tagGlobalLimitedItem *pGlobalLimitedItem = g_GlobalLimitedItemMgr.GetLimitedItemFromMapByAreaID(iter1->first,m_pRole->GetID());
				if( P_VALID(pGlobalLimitedItem) )
					pAreaMsgInfo[nIndex].dwTime = pGlobalLimitedItem->nTime;
				else
					pAreaMsgInfo[nIndex].dwTime = 0;

				pAreaMsgInfo[nIndex].dwCD = pAreaInfo->dwCD;

				nIndex++;
			}
			iter1++;
		}

		m_pRole->SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}
}
//---------------------------------------------------------------------------------
//�������һ��ϡ����������,ʵ�ʾ��Ǽ��ٵ�һ���ҵ���
DWORD AreaMgr::RandCancleAreaAbility()
{
	MapAreaInfo::TMapIterator iter = m_mapRareArea.Begin();
	while( iter != m_mapRareArea.End() )
	{
		tagAreaInfo *pAreaInfo = iter->second;
		if(P_VALID(pAreaInfo))
		{
			if( pAreaInfo->bActive )
			{
				CancleAreaAbility(iter->first);
				return iter->first;
			}
		}
		iter++;
	}

	return 0;
}
//---------------------------------------------------------------------------------
//�������һ��ϡ������,ʵ�ʾ��Ǽ��ٵ�һ���ҵ���
DWORD AreaMgr::RandDelArea()
{
	MapAreaInfo::TMapIterator iter = m_mapRareArea.Begin();
	if( iter != m_mapRareArea.End() )
	{
		tagAreaInfo *pAreaInfo = iter->second;
		if(P_VALID(pAreaInfo))
		{
			DWORD dwAreaID = iter->first;
			if( pAreaInfo->bActive )
			{
				CancleAreaAbility(iter->first,FALSE);
			}

			if( E_Success != DelArea(iter->first) )
			{
				return E_SystemError;
			}

			return dwAreaID;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------------
BOOL AreaMgr::DelSameArea(DWORD dwAreaID)
{
	std::vector<DWORD> vecAreaID;
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return FALSE;
	}

	if( pGodAreaProto->dwPreID > 0)
	{//ɾ��ԭ�е�����
		vecAreaID.push_back(pGodAreaProto->dwPreID);
	}
	else
	{//�ж���û��һ��ϵ�е�����
		DWORD dwRet = dwAreaID / 100;
		DWORD dwLevel = dwAreaID % 100;
		if( EGAT_OPTIONAL == pGodAreaProto->eType )
		{
			MapAreaInfo::TMapIterator iter = m_mapOptionalArea.Begin();
			while( iter!=m_mapOptionalArea.End() )
			{
				if(dwRet == iter->first / 100)
				{
					if( iter->first % 100 < dwLevel )
						vecAreaID.push_back(iter->first);
				}
				iter++;
			}
		}
		else
		{
			MapAreaInfo::TMapIterator iter = m_mapRareArea.Begin();
			while( iter!=m_mapRareArea.End() )
			{
				if(dwRet == iter->first / 100)
				{
					if( iter->first % 100 < dwLevel )
						vecAreaID.push_back(iter->first);
				}
				iter++;
			}
		}
	}

	std::vector<DWORD>::iterator it;
	for( it = vecAreaID.begin(); it != vecAreaID.end(); ++it )
	{
		CancleAreaAbilityAndDelArea(*it);
	}

	return TRUE;
}
//---------------------------------------------------------------------------------
BOOL AreaMgr::CancleAreaAbilityAndDelArea(DWORD dwAreaID)
{
	if(!P_VALID(m_pRole))
		return FALSE;

	CancleAreaAbility( dwAreaID, FALSE );

	//ȡ������
	tagNS_AddArea send;
	send.dwAreaID = dwAreaID;
	send.dwErrorCode = DelArea( dwAreaID );
	send.bAdd = FALSE;
	if(E_Success == send.dwErrorCode)
	{
		m_pRole->SendMessage(&send,send.dwSize);
		g_dbSession.SendDeleteArea2DB(m_pRole->GetID(),dwAreaID);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
//---------------------------------------------------------------------------------
DWORD AreaMgr::GetAreaCD(DWORD dwAreaID)
{
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return 0;
	}

	if(EGAT_OPTIONAL == pGodAreaProto->eType)
	{
		tagAreaInfo* pAreaInfo = m_mapOptionalArea.Peek(dwAreaID);
		if( P_VALID(pAreaInfo) )
		{
			return pAreaInfo->dwCD;
		}
		else
		{
			return 0;
		}
	}

	if(EGAT_RARE == pGodAreaProto->eType)
	{
		tagAreaInfo* pAreaInfo = m_mapRareArea.Peek(dwAreaID);
		if( P_VALID(pAreaInfo) )
		{
			return pAreaInfo->dwCD;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------------
VOID AreaMgr::Update()
{
	m_dwCurTime = timeGetTime();
	if( m_dwCurTime - m_dwLastTime < 1000 )
		return;

	MapAreaInfo::TMapIterator it = m_mapOptionalArea.Begin();
	tagAreaInfo* pAreaInfo = NULL;

	while( m_mapOptionalArea.PeekNext(it, pAreaInfo) )
	{
		if( P_VALID(pAreaInfo) )
		{
			pAreaInfo->dwCD--;

			if( (INT)pAreaInfo->dwCD <= 0 )
				pAreaInfo->dwCD = 0;
		}
	}

	it = m_mapRareArea.Begin();
	pAreaInfo = NULL;

	while( m_mapRareArea.PeekNext(it, pAreaInfo) )
	{
		if( P_VALID(pAreaInfo) )
		{
			pAreaInfo->dwCD--;

			if( (INT)pAreaInfo->dwCD <= 0 )
				pAreaInfo->dwCD = 0;
		}
	}

	m_dwLastTime = m_dwCurTime;
}
//---------------------------------------------------------------------------------
DWORD AreaMgr::CanAddArea(DWORD dwAreaID)
{
	std::vector<DWORD> vecAreaID;
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_SystemError;
	}

	//ǰ������ID
	if( pGodAreaProto->dwPreID > 0 )
	{
		if( !m_mapGodArea.IsExist(pGodAreaProto->dwPreID) 
			&& !m_mapOptionalArea.IsExist(pGodAreaProto->dwPreID)
			&& !m_mapRareArea.IsExist(pGodAreaProto->dwPreID) )
		return E_God_Area_Condition;
	}
	else
	{//�ж���û��һ��ϵ�е�����
		DWORD dwRet = dwAreaID / 100;
		DWORD dwLevel = dwAreaID % 100;
		if( EGAT_OPTIONAL == pGodAreaProto->eType )
		{
			MapAreaInfo::TMapIterator iter = m_mapOptionalArea.Begin();
			while( iter!=m_mapOptionalArea.End() )
			{
				if(dwRet == iter->first / 100)
				{
					if( iter->first % 100 > dwLevel )
						return E_God_Area_Condition;
				}
				iter++;
			}
		}
		else
		{
			MapAreaInfo::TMapIterator iter = m_mapRareArea.Begin();
			while( iter!=m_mapRareArea.End() )
			{
				if(dwRet == iter->first / 100)
				{
					if( iter->first % 100 > dwLevel )
						return E_God_Area_Condition;
				}
				iter++;
			}
		}
	}

	return E_Success;
}
//---------------------------------------------------------------------------------
VOID AreaMgr::SetAreaEffect( INT nNumPre, INT nNumCur )
{
	if(!P_VALID(m_pRole))
		return;

	const tagAreaEffectProto *pAreaEffectProtoPre = g_attRes.GetAreaEffectProto(nNumPre);

	const tagAreaEffectProto *pAreaEffectProtoCur = g_attRes.GetAreaEffectProto(nNumCur);

	if(P_VALID(pAreaEffectProtoPre))
	{
		m_pRole->RemoveBuff(Buff::GetIDFromTypeID(pAreaEffectProtoPre->dwBuffID), FALSE);
	}

	if(P_VALID(pAreaEffectProtoCur))
	{
		m_pRole->TryAddBuffToMyself(pAreaEffectProtoCur->dwBuffID);
	}
}
//---------------------------------------------------------------------------------
//ȥ��ϡ��������Чbuff
VOID AreaMgr::RemoveAreaBuff()
{
	INT nNum = m_mapRareArea.Size();
	const tagAreaEffectProto *pAreaEffectProto = g_attRes.GetAreaEffectProto(nNum);

	if(P_VALID(pAreaEffectProto))
	{
		m_pRole->RemoveBuff(Buff::GetIDFromTypeID(pAreaEffectProto->dwBuffID), FALSE);
	}
}

//���ϡ��������Чbuff
VOID AreaMgr::AddAreaBuff()//add by xp
{
	INT nNum = m_mapRareArea.Size();
	const tagAreaEffectProto *pAreaEffectProto = g_attRes.GetAreaEffectProto(nNum);

	if(P_VALID(pAreaEffectProto))
	{
		m_pRole->TryAddBuffToMyself(pAreaEffectProto->dwBuffID);
	}
}//add end

//---------------------------------------------------------------------------------
DWORD AreaMgr::GetRandRareArea()
{
	DWORD dwAreaID = 0; 
	tagAreaInfo* pAreaInfo = NULL;

	if( m_mapRareArea.RandPeek(dwAreaID,pAreaInfo) )
	{
		return dwAreaID;
	}

	return 0;
}
//---------------------------------------------------------------------------------
BOOL AreaMgr::IsHaveArea(DWORD dwAreaID)
{
	if(P_VALID(m_mapGodArea.Peek(dwAreaID)))
		return TRUE;

	if(P_VALID(m_mapOptionalArea.Peek(dwAreaID)))
		return TRUE;

	if(P_VALID(m_mapRareArea.Peek(dwAreaID)))
		return TRUE;

	return FALSE;
}
//---------------------------------------------------------------------------------
BOOL AreaMgr::DirectDelOneRareArea(DWORD dwAreaID)
{
	tagAreaInfo *pAreaInfo = m_mapRareArea.Peek(dwAreaID);

	if(P_VALID(pAreaInfo))
	{
		if( pAreaInfo->bActive )
		{
			CancleAreaAbility(dwAreaID,FALSE);
		}

		if( E_Success != DelArea(dwAreaID) )
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}
//---------------------------------------------------------------------------------











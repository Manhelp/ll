/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:31
	filename: 	e:\code\World\war_role.cpp
	file path:	e:\code\World
	file base:	war_role
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	ս����ɫ
*********************************************************************/
#include "StdAfx.h"
#include "war_role.h"
#include "war_role_mgr.h"
#include "role_mgr.h"
#include "role.h"
#include "unit.h"
#include "creature.h"

WarRole::WarRole(DWORD dwAccountID, DWORD dwRoleID)
			:m_dwAccountID(dwAccountID)
			,m_dwRoleID(dwRoleID)
			,m_dwNeutralMonsterKillCount(0)
			,m_dwMonsterKillCount(0)
			,m_dwBossKillCount(0)
			,m_dwRoleKillCount(0)
			,m_dwRoleDeadCount(0)
			,m_nGolds(0)
{

}

WarRole::~WarRole()
{

}

WarRole* WarRole::Create(DWORD dwAccountID, DWORD dwRoleID)
{
	return new WarRole(dwAccountID, dwRoleID);
}

BOOL WarRole::Init()
{
	m_dwNeutralMonsterKillCount = 0;	// ��ɱ����������
	m_dwMonsterKillCount = 0;			// ��ɱ�������
	m_dwBossKillCount = 0;			// ��ɱboss����
	m_dwRoleKillCount = 0;			// ��ɱ��ɫ����
	m_dwRoleDeadCount = 0;			// ��ɫ��������
	m_nGolds = 0;
	return TRUE;
}

VOID WarRole::Update()
{

}

VOID WarRole::Destroy()
{

}

DWORD WarRole::GetAccountID()
{
	return m_dwAccountID;
}

DWORD WarRole::GetRoleID()
{
	return m_dwRoleID;
}

VOID WarRole::OnDead(Unit* pSelf, Unit* pSrc)
{
	if (!P_VALID(pSelf) || !P_VALID(pSrc))
	{
		return;
	}

	// ������
	if (pSelf->IsRole())
	{
		// �Լ������
		Role* pRole = (Role*)pSelf;
		++m_dwRoleDeadCount;
		pRole->SyncRoleAttr(ERoleAttrRoleDeadCount, EMR_Map);
	}
}

VOID WarRole::OnKill(Unit* pSelf, Unit* pSrc)
{
	if (!P_VALID(pSelf) || !P_VALID(pSrc))
	{
		return;
	}

	if (pSelf->IsRole())
	{
		// �Լ������
		Role* pRole = (Role*)pSelf;

		if (pSrc->IsCreature())
		{
			// ɱ���ǹ���
			Creature* pCreature = (Creature*)pSrc;
			if (pCreature->IsBoss())
			{
				// ɱboss��
				++m_dwBossKillCount;
				pRole->SyncRoleAttr(ERoleAttrBossKillCount, EMR_Map);

			}
			else
			{
				// ɱ����
				++m_dwMonsterKillCount;
				pRole->SyncRoleAttr(ERoleAttrMonsterKillCount, EMR_Map);
			}

			if (ECamp_Neutrality == pCreature->GetCampType())
			{
				// ��ɱ����������
				++m_dwNeutralMonsterKillCount;
				pRole->SyncRoleAttr(ERoleAttrNeutralMonsterKillCount, EMR_Map);
			}
		}
		else if (pSrc->IsRole())
		{
			// ɱ�������
			// ��ɱ��ɫ����
			++m_dwRoleKillCount;
			pRole->SyncRoleAttr(ERoleAttrRoleKillCount, EMR_Map);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
//  ��ҵ�
//-------------------------------------------------------------------------------------------------------
VOID WarRole::AddGlod(INT nGlod, BOOL bSendMsg/* = TRUE*/)
{
	if ( 0 == nGlod)
		return ;

	m_nGolds += nGlod;
	if ( m_nGolds < 0)	m_nGolds = 0;

	Role* pRole = g_roleMgr.GetRolePtrByID(GetRoleID());
	if (!P_VALID(pRole))
	{
		return;
	}

	pRole->SyncRoleAttr(ERoleAttrWarGold, EMR_Map);
}

DWORD WarRole::GetNeutralMonsterKillCount()
{
	return m_dwNeutralMonsterKillCount;
}

DWORD WarRole::GetMonsterKillCount()
{
	return m_dwMonsterKillCount;
}

DWORD WarRole::GetBossKillCount()
{
	return m_dwBossKillCount;
}

DWORD WarRole::GetRoleKillCount()
{
	return m_dwRoleKillCount;
}

DWORD WarRole::GetRoleDeadCount()
{
	return m_dwRoleDeadCount;
}

//-------------------------------------------------------------------------------------------------------
// ����ս��
//-------------------------------------------------------------------------------------------------------
VOID WarRole::OnEnterWar(Role* pRole)
{
	if ( !P_VALID(pRole))
		return;

}

//---------------------------------------------------------------------------------
// �������еĽ�ɫս����Ϣ
//---------------------------------------------------------------------------------
VOID WarRole::OnLeaveWar(Role* pRole, DWORD dwDestMapID)
{
	if ( !P_VALID(pRole))
		return;

}





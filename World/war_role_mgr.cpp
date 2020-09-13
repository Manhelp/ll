/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:25
	filename: 	e:\code\World\war_role_mgr.cpp
	file path:	e:\code\World
	file base:	war_role_mgr
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	ս����ɫ����
*********************************************************************/
#include "StdAfx.h"
#include "war_role_mgr.h"
#include "role.h"
#include "map.h"
#include "creature.h"
#include "map_creator.h"

//-------------------------------------------------------------------------------------------------------
//  ���졢����
//-------------------------------------------------------------------------------------------------------
WarRoleMgr::WarRoleMgr(Map* pMap)
				:m_pMap(pMap)
{
	m_mapWarRole.Clear();
	m_eWarSuccess = EWRR_NULL;
}

WarRoleMgr::~WarRoleMgr()
{
	WarRole* pWarRole = NULL;
	TMap<DWORD, WarRole*>::TMapIterator	itDel = m_mapWarRole.Begin();
	while ( m_mapWarRole.PeekNext(itDel, pWarRole))
	{
		SAFE_DEL(pWarRole);
	}

	m_mapWarRole.Clear();
}

//-------------------------------------------------------------------------------------------------------
//  ��ʼ��
//-------------------------------------------------------------------------------------------------------
BOOL WarRoleMgr::Init()
{
	m_mapWarRole.Clear();
	m_eWarSuccess = EWRR_NULL;
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
//  ����
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::Update()
{
	TMap<DWORD, WarRole*>::TMapIterator it = m_mapWarRole.Begin();
	WarRole* pWarRole = NULL;
	while (m_mapWarRole.PeekNext(it, pWarRole))
	{
		pWarRole->Update();
	}
}

//-------------------------------------------------------------------------------------------------------
//  ���
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::Clear()
{
	TMap<DWORD, WarRole*>::TMapIterator it = m_mapWarRole.Begin();
	WarRole* pWarRole = NULL;
	while ( m_mapWarRole.PeekNext(it, pWarRole))
	{
		SAFE_DEL(pWarRole);
	}
	m_mapWarRole.Clear();
}

//-------------------------------------------------------------------------------------------------------
// ����ս����ɫ
//-------------------------------------------------------------------------------------------------------
BOOL WarRoleMgr::AddWarRole(WarRole* pWarRole)
{
	if ( !P_VALID(pWarRole))
		return FALSE;

	if ( !P_VALID(pWarRole->GetRoleID()))
		return FALSE;

	m_mapWarRole.Add(pWarRole->GetRoleID(), pWarRole);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ս����ɫ
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::DelWarRole(DWORD dwRoleID)
{
	if ( !P_VALID(dwRoleID))
		return ;

	WarRole* pWarRole = GetWarRole(dwRoleID);
	if ( P_VALID(pWarRole))
	{
		SAFE_DEL(pWarRole);
		m_mapWarRole.Erase(dwRoleID);
	}
}

//-------------------------------------------------------------------------------------------------------
// �õ�ս����ɫ
//-------------------------------------------------------------------------------------------------------
WarRole* WarRoleMgr::GetWarRole(DWORD dwRoleID)
{
	if ( !P_VALID(dwRoleID))
		return NULL;

	return m_mapWarRole.Peek(dwRoleID);
}

//-------------------------------------------------------------------------------------------------------
// �õ���ս���е����н�ɫ
//-------------------------------------------------------------------------------------------------------
TMap<DWORD, WarRole*> WarRoleMgr::GetWarRoles()
{
	return m_mapWarRole;
}

EWarRoleResult WarRoleMgr::GetWarResult()
{
	return m_eWarSuccess;
}

VOID WarRoleMgr::SetWarResult(EWarRoleResult eResult)
{
	if (eResult < 0 || eResult >= EWRR_End)
	{
		eResult = EWRR_NULL;
	}
	m_eWarSuccess = eResult;
}

//-------------------------------------------------------------------------------------------------------
// ��ɫ����ս��
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::OnRoleEnterWar(Role* pRole)
{
	if ( !P_VALID(pRole) || !P_VALID(pRole->GetSession()) || !P_VALID(pRole->GetMap()))
		return;

	WarRole* pWarRole = NULL;
	pWarRole = GetWarRole(pRole->GetID());
	if ( !P_VALID(pWarRole))
	{
		pWarRole = WarRole::Create(pRole->GetSession()->GetSessionID(), pRole->GetID());
		if (!P_VALID(pWarRole))
		{
			return;
		}
		pWarRole->Init();

		AddWarRole(pWarRole);
	}

	pWarRole->OnEnterWar(pRole);
	pRole->SetWarRole(pWarRole);
}

//-------------------------------------------------------------------------------------------------------
// ��ɫ�뿪ս��
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::OnRoleLeaveWar(Role* pRole)
{
	if ( !P_VALID(pRole) )
		return;

	WarRole* pWarRole = GetWarRole(pRole->GetID());
	if ( P_VALID(pWarRole))
	{
		// ���ս����ɫ������
		pWarRole->OnLeaveWar(pRole);
		DelWarRole(pRole->GetID());
		pRole->SetWarRole(NULL);
	}

}

//-------------------------------------------------------------------------------------------------------
// ������Ϣ���
//-------------------------------------------------------------------------------------------------------
VOID	WarRoleMgr::SendMsgToAll(LPVOID pMsg, DWORD dwSize)
{
	m_mapWarRole.ResetIterator();
	DWORD dwRoleID = 0;
	WarRole* pWarRole = NULL;
	while( m_mapWarRole.PeekNext(dwRoleID, pWarRole) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if ( P_VALID(pRole))
		{
			pRole->SendMessage(pMsg, dwSize);
		}
	}
}



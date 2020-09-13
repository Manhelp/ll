/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:22
	filename: 	e:\code\World\war_role_mgr.h
	file path:	e:\code\World
	file base:	war_role_mgr
	file ext:	h
	author:		zhangzhihua
	
	purpose:	ս����ɫ����
	        1.����ս���Ľ�ɫ��������WarRole����,����ս������������ɾ��
				����ս������
*********************************************************************/

#pragma once
#include "war_role.h"

class Role;
class Unit;

//---------------------------------------------------------------------------------
// ս����ɫ����
//---------------------------------------------------------------------------------
class WarRoleMgr
{
public:
	WarRoleMgr(Map* pMap);
	~WarRoleMgr();

	//---------------------------------------------------------------------------------
	// ��ʼ�������º�����
	//---------------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Clear();

	//---------------------------------------------------------------------------------
	// Get/Set
	//---------------------------------------------------------------------------------
	WarRole* GetWarRole(DWORD dwRoleID);
	TMap<DWORD, WarRole*> GetWarRoles();

	EWarRoleResult	GetWarResult();
	VOID	SetWarResult(EWarRoleResult eResult);


	//---------------------------------------------------------------------------------
	// ��ɫ����ս��
	// 1.���ǽ�ɫ�Ļ���ֻ��Խ�ɫ�뿪ս���ͽ���ս������������
	//---------------------------------------------------------------------------------
	VOID	OnRoleEnterWar(Role* pRole);
	VOID	OnRoleLeaveWar(Role* pRole);

	//---------------------------------------------------------------------------------
	// ����������Ϣ��ս���ڵ�ȫ�����
	//---------------------------------------------------------------------------------
	template<typename UnitOperation>
	VOID ForEachRoleInWar( UnitOperation oper );

	//---------------------------------------------------------------------------------
	// ����������Ϣ��ս���ڵ�ȫ���ѷ�
	//---------------------------------------------------------------------------------
	template<typename UnitOperation>
	VOID ForEachRoleInSameCamp( UnitOperation oper, ECampType eCampType );

	//---------------------------------------------------------------------------------
	// ������Ϣ���
	//---------------------------------------------------------------------------------
	VOID	SendMsgToAll(LPVOID pMsg, DWORD dwSize);

private:
	//---------------------------------------------------------------------------------
	// ս����ɫ����
	//---------------------------------------------------------------------------------
	BOOL	AddWarRole(WarRole* pWarRole);
	VOID	DelWarRole(DWORD dwRoleID);

private:
	TMap<DWORD, WarRole*>	m_mapWarRole;				// key:RoleID  ���߹������ºۼ��ļ��
	Map*					m_pMap;

	EWarRoleResult			m_eWarSuccess;
};


//---------------------------------------------------------------------------------
// ����������Ϣ��ս���ڵ�ȫ�����
//---------------------------------------------------------------------------------
template<typename UnitOperation>
VOID WarRoleMgr::ForEachRoleInWar( UnitOperation oper )
{
	m_mapWarRole.ResetIterator();
	WarRole* pWarRole = NULL;
	while( m_mapWarRole.PeekNext(pWarRole) )
	{
		if( P_VALID(pWarRole))
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pWarRole->GetRoleID());
			if ( P_VALID(pRole))
			{
				oper(pRole);
			}
		}
	}
}

//---------------------------------------------------------------------------------
// ����������Ϣ��ս���ڵ�ȫ���ѷ�
//---------------------------------------------------------------------------------
template<typename UnitOperation>
VOID WarRoleMgr::ForEachRoleInSameCamp( UnitOperation oper, ECampType eCampType )
{
	m_mapWarRole.ResetIterator();
	WarRole* pWarRole = NULL;
	while( m_mapWarRole.PeekNext(pWarRole) )
	{
		if( P_VALID(pWarRole))
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pWarRole->GetRoleID());
			if ( P_VALID(pRole))
			{
				if(pRole->GetCampType() == eCampType)
				{
					oper(pRole);
				}
			}
		}
	}
}








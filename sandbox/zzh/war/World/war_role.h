/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:05
	filename: 	e:\code\World\war_role.h
	file path:	e:\code\World
	file base:	war_role
	file ext:	h
	author:		zhangzhihua
	
	purpose:	ս����ɫ
*********************************************************************/

#pragma once

class Unit;

class WarRole
{
public:
	WarRole(DWORD dwAccountID, DWORD dwRoleID);
	~WarRole();

	static WarRole* Create(DWORD dwAccountID, DWORD dwRoleID);

	//---------------------------------------------------------------------------------
	// ��ʼ�������º�����
	//---------------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	//---------------------------------------------------------------------------------
	// Set/Get
	//---------------------------------------------------------------------------------
	DWORD	GetAccountID();
	DWORD	GetRoleID();

	//---------------------------------------------------------------------------------
	// �����ͻ�ɱ
	//---------------------------------------------------------------------------------
	VOID	OnDead(Unit* pSelf, Unit* pSrc);
	VOID	OnKill(Unit* pSelf, Unit* pSrc);

	//---------------------------------------------------------------------------------
	// �޸Ľ�Ǯ ע�⣺������Ʒ����AddGlod��������ModGlod
	//---------------------------------------------------------------------------------
	VOID	AddGlod(INT nGold, BOOL bSendMsg = TRUE);
	INT		GetGold()						{ return m_nGolds; }

	//---------------------------------------------------------------------------------
	// ս������һ�ɱ��ҡ��֡���������
	//---------------------------------------------------------------------------------
	DWORD	GetNeutralMonsterKillCount();
	DWORD	GetMonsterKillCount();
	DWORD	GetBossKillCount();
	DWORD	GetRoleKillCount();
	DWORD	GetRoleDeadCount();

	//---------------------------------------------------------------------------------
	// ����ս��
	//---------------------------------------------------------------------------------
	VOID	OnEnterWar(Role* pRole);

	//---------------------------------------------------------------------------------
	// �뿪ս��
	//---------------------------------------------------------------------------------
	VOID	OnLeaveWar(Role* pRole, DWORD dwDestMapID);

private:
	DWORD				m_dwAccountID;			// �ʻ�ID
	DWORD				m_dwRoleID;				// ��ɫID
	INT					m_nGolds;				// ���	

	DWORD m_dwNeutralMonsterKillCount;	// ��ɱ����������
	DWORD m_dwMonsterKillCount;			// ��ɱ�������
	DWORD m_dwBossKillCount;			// ��ɱboss����
	DWORD m_dwRoleKillCount;			// ��ɱ��ɫ����
	DWORD m_dwRoleDeadCount;			// ��ɫ��������

};




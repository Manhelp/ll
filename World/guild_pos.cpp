//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_pos.cpp
// author: Sxg
// actor:
// data: 2009-4-20
// last:
// brief: ����ְλ����
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_pos.h"
#include "role_mgr.h"
#include "../ServerDefine/role_data_define.h"
//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
GuildPos::GuildPos()
{
	m_n8CurNumChairman	 = 0;
	m_n8CurNumViceChairman	 = 0;

	m_n16CurNumJingYingAndSimple	 = 0;
	m_n16MaxNumJingYingAndSimple	 = 0;

	m_n8CurBackBoneMember	 = 0;
	m_n8MaxBackBoneMember	 = 0;

	m_n16CurNumMember	 = 0;
	m_n16MaxNumMember	 = 0;
}

GuildPos::~GuildPos()
{
}

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
VOID GuildPos::Init(BYTE byGuildLevel)
{
#if 0  //��ʥħ֮Ѫ������û������
	for(INT i=0; i<X_GUILD_TANG_NUM; ++i)
	{
		m_Tang[i].Init(byGuildLevel);
	}
#endif
	m_n16MaxNumJingYingAndSimple	= MGuildMaxMember(byGuildLevel);
	m_n8MaxBackBoneMember = MGuildBackBoneMember(byGuildLevel);
	m_n16MaxNumMember	= MGuildMaxMember(byGuildLevel)+MGuildBackBoneMember(byGuildLevel)+MAX_GUILDPOS_BANGZHU+MAX_GUILDPOS_FUBANGZHU;
}

//-------------------------------------------------------------------------------
// ָ��ְλ����Ա
//-------------------------------------------------------------------------------
BOOL GuildPos::IsPosFull(EGuildMemberPos eGuildPos)
{
	ASSERT(!IsMemberFull());
	ASSERT(::IsGuildPosValid(eGuildPos));
	
	// ����ֱ��ְλ�ж�
	switch(eGuildPos)
	{
	case EGMP_HuiZhang:			//����᳤
		return IsChairmanFull();

	case EGMP_FuHuiZhang:		//���ḱ�᳤
		return IsViceChairmanFull();

	case EGMP_GuGan:			//�Ǹ�
		return IsBackBoneFull();

	case EGMP_JingYing:	//��Ӣ
	case EGMP_HuiYuan:		//��ͨ��Ա
		return IsJingYingAndSimpleFull();

	default:
		return TRUE;
	}

	// û�н����������̷��أ�˵��ְλ������
	return TRUE;
}

//-------------------------------------------------------------------------------
// ��ӳ�Ա
//-------------------------------------------------------------------------------
DWORD GuildPos::AddMember(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	ASSERT(!IsPosFull(eGuildPos));

	DWORD dwErrorCode = GT_INVALID;
	BOOL  bRunDefault = FALSE;

	// ����ֱ��ְλ�ж�
	switch(eGuildPos)
	{
	case EGMP_HuiZhang:			//����᳤
		dwErrorCode = AddChairman(dwRoleID);
		break;
	case EGMP_FuHuiZhang:		//���ḱ�᳤
		dwErrorCode = AddViceChairman(dwRoleID);
		break;
	case EGMP_GuGan:			//�Ǹ�
		dwErrorCode = AddBackStone(dwRoleID);
		break;
	case EGMP_JingYing:	//��Ӣ
	case EGMP_HuiYuan:		//��ͨ��Ա
		dwErrorCode = AddJingYingOrSimple(dwRoleID);
		break;

	default:
		bRunDefault = TRUE;
	}

	if(bRunDefault)
	{
		// ִ�е��˴������øú���ǰ���߼�������
		ASSERT(0);
	}

	if(E_Success == dwErrorCode)
	{
		++m_n16CurNumMember;
	}

	return dwErrorCode;
}

DWORD GuildPos::AddChairman(DWORD dwRoleID)
{
	ASSERT(!IsChairmanFull());
	++m_n8CurNumChairman;
	return E_Success;
}

DWORD GuildPos::AddViceChairman(DWORD dwRoleID)
{
	ASSERT(!IsViceChairmanFull());
	++m_n8CurNumViceChairman;
	return E_Success;
}

DWORD GuildPos::AddJingYingOrSimple(DWORD dwRoleID)
{
	ASSERT(!IsJingYingAndSimpleFull());
	++m_n16CurNumJingYingAndSimple;
	return E_Success;
}

DWORD GuildPos::AddBackStone(DWORD dwRoleID)
{
	ASSERT(!IsBackBoneFull());
	++m_n8CurBackBoneMember;
	return E_Success;
}

//-------------------------------------------------------------------------------
// ɾ��
//-------------------------------------------------------------------------------
VOID GuildPos::RemoveMember(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	ASSERT(m_n16CurNumMember >= 1);

	BOOL  bRunDefault = FALSE;
	
	// ����ֱ��ְλ�ж�
	switch(eGuildPos)
	{
	case EGMP_HuiZhang:			//����᳤
		RemoveChairman(dwRoleID);
		break;
	case EGMP_FuHuiZhang:		//���ḱ�᳤
		RemoveViceChairman(dwRoleID);
		break;
	case EGMP_GuGan:			//�Ǹ�
		RemoveBackStone(dwRoleID);
		break;
	case EGMP_JingYing:	//��Ӣ
	case EGMP_HuiYuan:		//��ͨ��Ա
		RemoveJingYingOrSimple(dwRoleID);
		break;

	default:
		bRunDefault = TRUE;
	}

	if(bRunDefault)
	{
		// ִ�е��˴������øú���ǰ���߼�������
		ASSERT(0);
		return;
	}

	--m_n16CurNumMember;
}

VOID GuildPos::RemoveChairman(DWORD dwRoleID)
{
	--m_n8CurNumChairman;
	ASSERT(m_n8CurNumChairman >= 0);
}

VOID GuildPos::RemoveViceChairman(DWORD dwRoleID)
{
	--m_n8CurNumViceChairman;
	ASSERT(m_n8CurNumViceChairman >= 0);
}

VOID GuildPos::RemoveJingYingOrSimple(DWORD dwRoleID)
{
	--m_n16CurNumJingYingAndSimple;
	ASSERT(m_n16CurNumJingYingAndSimple >= 0);
}

VOID GuildPos::RemoveBackStone(DWORD dwRoleID)
{
	--m_n8CurBackBoneMember;
	ASSERT(m_n8CurBackBoneMember >= 0);
}

//-------------------------------------------------------------------------------
// ְλ����Ҫ��
//-------------------------------------------------------------------------------
DWORD GuildPos::SpecGuildPosLimit(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	return E_Success;
}
INT16	GuildPos::GetPosNum(EGuildMemberPos eGuildPos) const
{
	switch( eGuildPos )
	{
	case EGMP_HuiZhang:
		return m_n8CurNumChairman;
	case EGMP_FuHuiZhang:
		return m_n8CurNumViceChairman;
	case EGMP_GuGan:
		return m_n16CurNumJingYingAndSimple;
	case EGMP_JingYing:
		return m_n8CurBackBoneMember;
	case EGMP_HuiYuan:
		return m_n16CurNumMember;
	}
	return 0;
}
VOID	GuildPos::IncPosNum(EGuildMemberPos eGuildPos,INT a )
{
	switch( eGuildPos )
	{
	case EGMP_HuiZhang:
		m_n8CurNumChairman += a;
	case EGMP_FuHuiZhang:
		m_n8CurNumViceChairman += a;
	case EGMP_GuGan:
		m_n16CurNumJingYingAndSimple += a;
	case EGMP_JingYing:
		m_n8CurBackBoneMember += a;
	case EGMP_HuiYuan:
		m_n16CurNumMember += a;
	}
}


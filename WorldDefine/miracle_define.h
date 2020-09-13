//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: miracle_define.h
// author: songg
// actor:
// data: 2010-01-05
// last:
// brief: �ɽ�����
//-----------------------------------------------------------------------------
#pragma once
#include "msg_common_errorcode.h"
#include "msg_talent.h"

#pragma pack(push, 1)


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT32 MIRACLE_PRC_RESET_NUM	= 100; //ÿ��0��ˢ

const INT32 MIRACLE_BASIC_ITEM_COST		= 3300102;	//��������������Ʒ	�ɽ�ɽ���
const INT32 MIRACLE_MIDDLE_ITEM_COST	= 3300103; //�м�����������Ʒ	�ɽ������
const INT32 MIRACLE_SENIER_ITEM_COST	= 3300104; //�߼�����������Ʒ	�ɽ������

const INT32 MIRACLE_ADD_PRC_NUM_ITEM	= 3300105; //�ɽ����з�

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
enum EMiracleSkillErrorCode
{
	E_Miracle_Success			= E_Skill_End,	//�ɹ�
	E_Miracle_LackItem ,						//ȱ�ٱر���Ʒ
	E_Miracle_LackPower	,						//ȱ��ʵ��ֵ
	E_Miracle_LackMoney	,						//������
	E_Miracle_LackPracticeNum ,					//������������

	E_Miracle_ItemNotExist ,					//��Ʒ������
	E_Miracle_SpecFunError ,					//����ʹ�����ʹ���

};


//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// �ṹ��
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//�ɽ缼�ܽṹ -- �ɽ缼��������Ϣ
//-----------------------------------------------------------------------------

struct tagMiracleSkill
{
	DWORD					dwSkillTypeID;		// ����typeid

	DWORD					dwSilverCost;		// ����
	INT16					n16PowerCost;		// ʵ��ֵ
	DWORD					dwItemTypeID;		// ������Ʒ��typeid
	INT16					n16ItemNumCost;		// ��Ʒ������


	tagMiracleSkill()
	{
		ZeroMemory(this, sizeof(*this));
	}
	tagMiracleSkill(const tagMiracleSkill &rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
	}
};


#pragma pack(pop)

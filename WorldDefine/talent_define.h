//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: talent_define.h
// author: Aslan
// actor:
// data: 2008-10-20
// last:
// brief: ��������̬����
//-----------------------------------------------------------------------------
#pragma once

#include "RoleDefine.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// CONSTS
//-----------------------------------------------------------------------------
const INT32 X_MAX_TALENT_PER_ROLE		=		2;		// һ����ҿ���࿪������������
const INT32 X_MAIN_TALENT_START			=		0;		// �����ʵ���ʼλ�ã�0
const INT32 X_FRESHHAND_TALENT			=		X_MAX_TALENT_PER_ROLE-1; //��������λ��


//-----------------------------------------------------------------------------
// ���ʽṹ
//-----------------------------------------------------------------------------
struct tagRoleTalent
{
	ETalentType		eType;		// ��������
	INT32			nPoint;		// �����ʵ���Ͷ����
};

#pragma pack(pop)
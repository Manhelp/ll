//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: stall_define.h
// author: 
// actor:
// data: 2008-12-17
// last:
// brief: ��̯
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
const INT64 LOWEST_PROFIT							= -50000;		// ����Ȿ����
const INT32 MAX_TIGER_NUM							= 7;				// �и��ʵĽ�������

const INT32 X1_ADD_BASE_NUM						= 200;
const INT32 X5_ADD_BASE_NUM						= 1000;
const INT32 X10_ADD_BASE_NUM					= 2000;
const INT32 X50_ADD_BASE_NUM					= 10000;

const INT32 X1_ADD_BASE_RATE						= 10;
const INT32 X5_ADD_BASE_RATE						= 10;
const INT32 X10_ADD_BASE_RATE						= 10;
const INT32 X50_ADD_BASE_RATE						= 5;


const INT32 TIGER_COIN_TYPEID						= 4820061;				// �ϻ�����ID



enum
{
	E_Tiger_Result_Null = 0,			// �޽���
	E_Tiger_Result_bounty1,			// ��߽���
	E_Tiger_Result_bounty2,			// 
	E_Tiger_Result_bounty3,			// 
	E_Tiger_Result_bounty4,	
	E_Tiger_Result_bounty5,
	E_Tiger_Result_bounty6,
	E_Tiger_Result_bounty7,			// ����
};

//-----------------------------------------------------------------------------
#pragma pack(pop)
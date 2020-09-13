//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_holySoul.h
// author: zxzhang
// actor:
// data: 2010-06-13
// last:
// brief:  Ԫ��ϵͳ���ݱ������ݿ�
//-----------------------------------------------------------------------------

#pragma once

#include "../WorldDefine/ItemDefine.h"
#pragma pack(push, 1)


//------------------------------------------------------------------------------
//Ԫ������
//------------------------------------------------------------------------------
struct tagHolySoulInfoEx
{
	INT64	         n64CurLevelExp;	   //Ԫ��ǰ����
	DWORD            dwSoulID;             //Ԫ��ID
	INT		         nCurSoulValue;		   //��ǰԪ��ֵ
	INT		         nCurCovalue;	       //��ǰĬ��ֵ	
	EHolySoulState   eHolySoulState;       //Ԫ��״̬
    tagSoulAtt       HolySoulAtt;          //Ԫ������
    tagHolySoulInfoEx()
	{
		memset(this,0,sizeof(*this));
	}
};

//----------------------------------------------------------------------------
// Ԫ����������
//----------------------------------------------------------------------------
enum ESoulAttTypeEx
{
	ESoulAttEx_Null	   = 0,		//������
	ESoulAttEx_SoulVal = 1,		//Ԫ��ֵ
	ESoulAttEx_Covalue = 2,		//Ԫ��Ĭ��ֵ
	ESoulAttEx_End,
};

//------------------------------------------------------------------------------
//����Ԫ����Ϣ
//------------------------------------------------------------------------------
CMD_START(NDBC_InsertHolySoulInfo)
DWORD				dwRoleID;
tagHolySoulInfoEx   SoulAtt;
CMD_END


//------------------------------------------------------------------------------
//����Ԫ����Ϣ
//------------------------------------------------------------------------------
CMD_START(NDBC_UpdateHolySoulInfo)
DWORD				dwRoleID;
tagHolySoulInfoEx   SoulAtt;
CMD_END

//------------------------------------------------------------------------------
//���¾�������
//------------------------------------------------------------------------------
CMD_START(NDBC_UpdateHolySoulExpAtt)
DWORD				dwRoleID;
INT64				n64Exp;				//����
CMD_END


//------------------------------------------------------------------------------
//����Ԫ������
//------------------------------------------------------------------------------
CMD_START(NDBC_UpdateHolySoulAttInfo)
ESoulAttTypeEx      eSoulAttTypeEx;			//Ԫ����������
DWORD				dwRoleID;
DWORD 				dwAttVal;				//Ԫ������ֵ
CMD_END

#pragma pack(pop)
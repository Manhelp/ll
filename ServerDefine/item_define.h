//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_define.h
// author: 
// actor:
// data: 2008-10-17
// last:
// brief: ����������Ʒ��ؽṹ
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
#include <vector>
using namespace std;

#include "../WorldDefine/itemdefine.h"
//-----------------------------------------------------------------------------
// ����updateʱ��(��λ��tick)
//-----------------------------------------------------------------------------
const INT ITEM_UPDATE_TIME		= 60 * TICK_PER_SECOND;		// 60s

//-----------------------------------------------------------------------------
// װ��B���������ɸ���[0��10000]
//-----------------------------------------------------------------------------
const INT g_nEquipSpecAttPct[X_EQUIP_SPECATT_NUM] = 
{
	800,	400,	200,	800,	400,
	200,	800,	400,	200,	100,
	800,	400,	200,	100,	500,
	500,	500,	500,	500,	500,
	100,	500,	500,	100
};

//-----------------------------------------------------------------------------
// ͬһ��������Ʒ�ƶ����ɷ��������Ĳ���
//-----------------------------------------------------------------------------
struct tagItemMove
{
	tagItem	*pItem2;		// Ŀ��λ���ϵ���Ʒ
	INT16	n16NumRes1;		// ���ƶ���Ʒ�����ո���
	INT16	n16NumRes2;		// Ŀ��λ������Ʒ�����ո���
	bool	bCreateItem;	// Ŀ��λ���ϵ���Ʒ�Ƿ�Ϊ�´�����(���ʱΪtrue)
	bool	bOverlap;		// �Ƿ�ѵ�
	bool	bChangePos;		// λ���Ƿ��ƶ�

	tagItemMove()
	{
		Init();
	}
	
	VOID Init()
	{
		pItem2		= NULL;
		n16NumRes1	= 0;
		n16NumRes2	= 0;
		bCreateItem = FALSE;
		bOverlap	= FALSE;
		bChangePos	= TRUE;
	}
};

//-----------------------------------------------------------------------------
// �����ɫ����ʱ����µ���Ʒ����
//-----------------------------------------------------------------------------
struct tagItemUpdate
{
	INT64		n64Serial;
	DWORD		dwOwnerID;
	DWORD		dwAccountID;
	INT32		nUseTimes;		// ��Ʒ�ϼ���ʹ�ô���
	INT16		n16Num;			// ��Ʒ����
	INT16		n16Index;		// ������λ��
	BYTE		byConType;		// ��������
	BYTE		byBindType;		// ������
	DWORD       dwUnlockTime;
	tagDWORDTime	dw1stGainTime;
	DWORD			dwSpecVal1;	// ��Ʒ���⹦���ֶ�ֵ1����̬���ԣ����ض���������
	DWORD			dwSpecVal2;	// ��Ʒ���⹦���ֶ�ֵ2
};

struct tagEquipSpecUpdate
{
	INT64			n64Serial;
	tagEquipSpec	equipSpec;
};

//-----------------------------------------------------------------------------
// IM ��ƷӰ������ö��
//-----------------------------------------------------------------------------
enum EIMEffect
{
	EIME_Null			= 0,

	EIME_Color			= 1,	// ��ɫ
	EIME_ComAdvance		= 2,	// ��߳ɹ�����
	EIME_ProtectSign	= 3,	// ���׷���ǿ��ʧ��ʱ������װ��Ǳ��ֵ��
};

//-----------------------------------------------------------------------------
// IM ��ƷӰ��
//-----------------------------------------------------------------------------
struct tagIMEffect
{
	EIMEffect	eEffect;	// Ӱ�����
	DWORD		dwParam1;	// Ӱ��ֵ
	DWORD		dwParam2;
	tagIMEffect()
	{
		eEffect = EIME_Null;
		dwParam1 = 0;
		dwParam2 = 0;
	}
};

//-----------------------------------------------------------------------------
// ��Ʒ��ȴ��Ϣ
//-----------------------------------------------------------------------------
struct tagCDInfo
{
	DWORD		dwElapseTime;
	DWORD		dwRemainTime;
};


// Jason 2009-12-8 ���ʯ���ýṹ
struct tagGoldStone
{
	DWORD	dwTypeID;
	INT		nPct;
};
struct tagPhilosophersStoneConfig
{
	union unionKey
	{
		struct tagPair
		{
			INT nStoneType;
			DWORD dwSrcItemTypeID;
		} m_Pair;
		INT64 m_n64Key;
	} uniKey;
	//DWORD dwDestItemTypeID;
	vector<tagGoldStone> vecGoldStone;
	DWORD GetStoneTypeIDByPct(INT nPct)
	{
		INT pct = 0;
		for( vector<tagGoldStone>::iterator i = vecGoldStone.begin(); i != vecGoldStone.end(); ++i )
		{
			pct += i->nPct;
			if( nPct <= pct )
				return i->dwTypeID;
		}
		return GT_INVALID;
	}
	tagPhilosophersStoneConfig()
	{
		uniKey.m_n64Key = 0;
	}
	tagPhilosophersStoneConfig(const tagPhilosophersStoneConfig & t)
	{
		_copy(t,*this);
	}
	tagPhilosophersStoneConfig & operator= (const tagPhilosophersStoneConfig &t)
	{
		_copy(t,*this);
		return *this;
	}
private:
	static void _copy(const tagPhilosophersStoneConfig & t, tagPhilosophersStoneConfig & t1)
	{
		t1.uniKey.m_n64Key = t.uniKey.m_n64Key;
		t1.vecGoldStone = t.vecGoldStone;
	}
};

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ���ݵĽṹ
//-----------------------------------------------------------------------------
struct tagItemData
{
	INT64	n64Serial;
	DWORD	dwTypeID;
};

// ϡ����Ʒ�Ĵ�����ʽ
enum EGlobalLimitedItemExistType
{
	EGLIET_Item		= 0,	// ��Ʒ
	EGLIET_Skill	= 1,	// ����
};

// ϡ����Ʒ�Ĵ��ڼ�¼
struct tagGlobalLimitedItem
{
	DWORD						dwID;
	EGlobalLimitedItemExistType eType;
	INT64						n64ID;			// ���������Ϊ׼,�������Ʒ��������Ʒ64λID������Ǽ��ܣ���Ϊ����������ID
	DWORD						dwAreaID;		//ϡ������ID
	INT							nTime;			//ʣ��ʱ��
	DWORD						dwRecordTime;	//��¼ʱ��

	tagGlobalLimitedItem& operator =(const tagGlobalLimitedItem &gli)
 	{
 		if( this == &gli )
 			return *this;
 
 		this->dwID			= gli.dwID;
 		this->eType			= gli.eType;
		this->n64ID			= gli.n64ID;
		this->dwAreaID		= gli.dwAreaID;
 		this->nTime			= gli.nTime;
		this->dwRecordTime	= gli.dwRecordTime;
 		return *this;
 	}
};

#pragma pack(pop)

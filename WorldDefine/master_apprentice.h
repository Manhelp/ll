//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: master_apprentice.h
// author: twan
// actor:
// data: 2010-01-18
// last: 
// brief: ʦͽ���
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "time.h"

#pragma pack(push, 1)

const	INT		MAX_APPRENTICE_NUM				=		5;		// ���ͽ������
const	INT		GOOD_STU_CAN_LEAVE_LEVEL		=		50;		// ͽ�ܳ�ʦ�ȼ�
const	INT		MASTER_QUALIFICATION_LEVEL		=		50;		// Ϊ��ʦ���ʸ�ȼ�
const	UINT16	MASTER_QUALIFICATION_QUEST_ID	=		8001;	// ʦ���ʸ�����
const	INT		MASTER_BREAKOUT_DAY_LIMIT		=		1;		// ���ʦͽ��ϵ��Ҫ��һ��δ����ʱ�䣨������
const	DWORD	APPRENTICE_LEVEL_UP_ITEM		=		1370001;	// ʦͽ����TypeID

//-----------------------------------------------------------------------------
// �����ݿ�ṹ
//-----------------------------------------------------------------------------
struct tagApprenticeData
{
	DWORD	dwData[MAX_APPRENTICE_NUM];
	tagApprenticeData()
	{
		ZeroMemory(dwData, sizeof(DWORD)*MAX_APPRENTICE_NUM);
	}

	tagApprenticeData& operator = (tagApprenticeData& other)
	{
		memcpy(this->dwData, other.dwData, sizeof(other.dwData));
		return *this;
	}
};

//-----------------------------------------------------------------------------
// ʦͽ��ҵ�������Ա�
// �����ݿ�ͬ�Žṹ
//-----------------------------------------------------------------------------
struct tagClassMateData : public tagApprenticeData
{
	INT32	nJingWuPoint;
	INT32	nQinWuPoint;
	INT32	nLevelUpBonus;
	tagClassMateData():tagApprenticeData(), nJingWuPoint(0), nQinWuPoint(0), nLevelUpBonus(0)
	{
		
	}

	tagClassMateData& operator = (tagClassMateData& other)
	{
		memcpy(this->dwData, other.dwData, sizeof(other.dwData));
		this->nJingWuPoint = other.nJingWuPoint;
		this->nQinWuPoint = other.nQinWuPoint;
		this->nLevelUpBonus = other.nLevelUpBonus;
		return *this;
	}
};

//-----------------------------------------------------------------------------
// ʦͽ����������Ա�
//-----------------------------------------------------------------------------
struct tagMasterApprenticeProto
{
	DWORD		dwLevelID;				// �ȼ�
	INT32		nLevelUpAddPoint;		// ����������ҵ����
	INT32		nLevelUpNeedPoint;		// ������Ҫ��ҵ����
	INT32		nLevelUpNeedYuanBao;	// ������Ҫ���ı�������
	INT32		nLevelUpSelfPoint;		// �������������ӵ��������
	INT32		nLevelUpBonus;			// ������ʦ�����ӵĻ���
};

//-----------------------------------------------------------------------------
// ͬ�ź�ͽ�ܵ�ͬ���ͻ��˽ṹ
//-----------------------------------------------------------------------------
struct tagMatesAndApprenticeInfo
{
	DWORD			dwRoleID;				// ���ID
	INT32			nLevel;					// ��ҵȼ�
	tagDWORDTime	lastQuestTime;			// �ϴ�ʦͽ����ʱ�䣨�죩������7��ͳһ��8��
	tagDWORDTime	lastOnLineTime;			// �ϴ����ߣ��죩������3��ͳһ��4��
	bool			bIsOnLine;				// �Ƿ�����
	bool			bReserved[3];			// ����
};

//-----------------------------------------------------------------------------
// ʦ���ͬ���ͻ��˽ṹ
//-----------------------------------------------------------------------------
struct tagMasterInfo
{
	DWORD		dwRoleID;				// ���ID
	INT32		nLevel;					// �ȼ�
	DWORD		dwJingWuPoint;			// ��ҵ����
	DWORD		dwQinWuPoint;			// ʦ�µ���
};


// ������
enum
{
	E_MA_Success							=	E_Success,		// �ɹ�
	E_MA_Already_Has_Master					=	1,				// ���Ѿ���ʦ��
	E_MA_Level_Limit						=	2,				// �ȼ�����50���������ʦ
	E_MA_DestRole_Have_No_Qualification		=	3,				// �Է�������ͽ�ʸ�
	E_MA_DestRole_Apprentice_Full			=	4,				// �Է�ͽ����������
	E_MA_Friend_Full						=	5,				// ���ĺ�����λ����
	E_MA_Dest_Friend_Full					=	6,				// �Է�������λ����
	E_MA_Dest_Already_Has_Master			=	7,				// �Է��Ѿ���ʦ��
	E_MA_Dest_Level_Limit					=	8,				// �Է��ȼ�����50�����޷���Ϊͽ��
	E_MA_Have_No_Qualification				=	9,				// ��������ͽ�ʸ�
	E_MA_Apprentice_Full					=	10,				// ��ͽ����������
	E_MA_Not_Enough_JingWuPoint				=	11,				// ��ҵ��������
	E_MA_Not_Enough_BaoJian					=	12,				// ��������
	E_MA_Not_OnLine							=	13,				// �Է�������
	E_MA_OnLine_Day_Limit					=	14,				// ��Ҫ�Է�����3��δ����
	E_MA_You_Refuse_BaiShi					=	15,				// ���ܾ���XXX�İ�ʦ����
	E_MA_Dest_Refuse_BaiShi					=	16,				// XXX�ܾ������İ�ʦ����
	E_MA_You_Refuse_ShouTu					=	17,				// ���ܾ���XXX����ͽ����
	E_MA_Dest_Refuse_ShouTu					=	18,				// XXX�ܾ���������ͽ����
	E_MA_Already_Force_BreakOut				=	19,				// ���Ѿ�ѡ���ǿ������ʦ�ţ�ʣ������ȴ�ʱ��ԼXСʱ�������ĵȴ���

    E_MA_ErrorCode_End
};

#pragma pack(pop)

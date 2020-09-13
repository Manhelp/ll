//-----------------------------------------------------------------------------
// filename: train_define.h
// author: lfmeng
// actor:
// data: 2010-3-10
// last:
// brief: ��֮����ϵͳ����
//-----------------------------------------------------------------------------

#pragma once

#pragma pack(push, 1)


//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
#define	REWARD_ITEM_MAX_CNT		2			// ������Ʒ������
#define TRAIN_MAX_CNT			100		// ����������

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
enum ETrainState
{
	ETS_Close	= 0,	// δ����
	ETS_Open	= 1,	// ������
	ETS_Finish	= 2,	// �����(û����ȡ����)
	ETS_Pass		= 3,	// ��ͨ��(�Ѿ���ȡ�꽱��)
};

//-----------------------------------------------------------------------------
// ��Ʒ����
//-----------------------------------------------------------------------------
struct tagItemReward
{
	DWORD			nItemTypeID;			// ��õ���Ʒ����
	INT				nItemCount;				// ��Ʒ�����ĸ���
	INT				nItemQuality;			// ��ƷƷ��
};

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
struct tagTrainReward
{
	INT				nStrengthReward;					// ʵ��ֵ����
	DWORD		dwMoneyReward;					// ��Ǯ����
	DWORD		dwExpReward;							// ���齱��
	tagItemReward	pItemReward[REWARD_ITEM_MAX_CNT];	// ��Ʒ����
};

//-----------------------------------------------------------------------------
// ����״̬��(�����ݿ�ṹ)
//-----------------------------------------------------------------------------
struct tagTrainState
{
	DWORD			dwTrainID;			// ����ID
	ETrainState		eTrainState;			// ����״̬
	INT					nCompleteness;	// ������ɶ�
};
struct tagTrainStateTable
{
	BOOL			bFirstInit;
	INT				nTrainCnt;
	tagTrainState	pTrainState[TRAIN_MAX_CNT];

	tagTrainStateTable()
	{
		bFirstInit = TRUE;
		nTrainCnt = 0;
		ZeroMemory(pTrainState, sizeof(tagTrainState)*TRAIN_MAX_CNT);
	}

	tagTrainStateTable& operator = (tagTrainStateTable& other)
	{
		bFirstInit	= other.bFirstInit;
		nTrainCnt	= other.nTrainCnt;
		memcpy(pTrainState, other.pTrainState, sizeof(tagTrainState)*TRAIN_MAX_CNT);
		return *this;
	}
};

//-----------------------------------------------------------------------------
// �������Ա�
//-----------------------------------------------------------------------------
struct tagTrainAttProto
{
	DWORD					dwTrainID;			// ����ID
	DWORD					dwType1;				// ��������1��������
	DWORD					dwType2;				// ��������2��С�����
	DWORD					dwMaxNum;		// �����Ҫ�����
	tagTrainReward		TrainReward;		// ����ͨ���Ľ���
};

#pragma pack(pop)
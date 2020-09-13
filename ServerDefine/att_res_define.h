//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res_define.h
// author: Sxg
// actor:
// data: 2009-02-19
// last:
// brief: ��Դ���������ݽṹ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/longhun_define.h"
#include "../WorldDefine/godbid_define.h"
#include "../WorldDefine/compose_define.h"
#include "..\WorldDefine\tiger_define.h"

//-----------------------------------------------------------------------------
// Ʒ��Ӱ���װ��������ز���
//-----------------------------------------------------------------------------
struct tagEquipQltyEffect
{
	// ��������Ӱ��ϵ��
	FLOAT fWeaponFactor;
	FLOAT fArmorFactor;

	// һ������
	INT32 nAttAFactor;
	FLOAT fAttAFactor;
	INT32 nAttANumEffect;

	// Ǳ��ֵ
	FLOAT fPotFactor;

	// ��Ƕ������ -- ��¼���ֵļ���
	INT32 nHoleNumPct[MAX_EQUIPHOLE_NUM + 1];

	// �������� -- ����
	INT32 nLonghunPct[X_LONGHUN_TYPE_NUM];

	// ��������
	INT32 nSpecAttPct;

	// ǿ���ȼ�����
	INT32	n8StrengLevelLimit;
};

//-----------------------------------------------------------------------------
// ��������֮���Ӱ��ṹ
//-----------------------------------------------------------------------------
struct tagSkillModify
{
	TList<DWORD>	listModify;		// ֱ��Ӱ��������ܵĴӼ���
};

//-----------------------------------------------------------------------------
// ʱװ���ɹ�����ض���
//-----------------------------------------------------------------------------
struct tagFashionGen
{
	FLOAT		fAppearanceFactor;	// ��������(AppearancePct)
	INT16		n16ReinPct;			// ͳ�����Լӳ�(ReinPct)
	INT16		n16SavvyPct;		// �������Լӳ�(SavvyPct)
	INT16		n16FortunePct;		// ��Ե���Լӳ�(FortunePct)
	INT8		n8ReinVal;			// ֵ=��Ʒ�ȼ���ReinVal[ȡ��](ReinVal)
	INT8		n8SavvyVal;			// ֵ=��Ʒ�ȼ���SavvyVal[ȡ��](SavvyVal)
	INT8		n8FortuneVal1;		// ֵ=FortuneVal1�����¸���20%��+װ��Ʒ��/FortuneVal2
	INT8		n8FortuneVal2;		// (FortuneVal1, FortuneVal2)
	INT8		n8Dummy[2];
};

struct tagFashionColorPct	// ʱװ����ʱ��ɫ����
{
	INT16	n16ColorPct[X_COLOR_NUM];	// ��ɫ����
};

//-----------------------------------------------------------------------------
// ������ɿ�ʹ��������߶���
//-----------------------------------------------------------------------------
struct tagQuestUseSpeciItem
{
	INT16	n16QuestID;		// ����ID
	DWORD	dwItemID;		// ����ID
	INT16	n16ItemNum;		// ��������
};

//-----------------------------------------------------------------------------
// ��ɱ��һ�ȡ�ľ�����
//-----------------------------------------------------------------------------
struct tagKillerExp
{
	INT16	n16Level;
	DWORD	dwExp;
	DWORD	dwBaseRwdExp; // ���齱������ֵ
	DWORD	dwBaseRwdSilver; // ��Ǯ��������ֵ
};
// Jason 2010-5-24 ��������ԭ��
struct tagLearnSoaringSkillProto
{
	//soar_skill_proto.xml
	DWORD	dwID;
	INT		nLearnSilverCosted;
	INT		nExpCosted;
	DWORD	dwItemNeeded;
	INT		nItemCount;
};
struct tagCastingProto
{
	DWORD	dwID;
	BYTE	byType;				// 0	������1	���ߣ�2	��Ʒ
	UINT16	u16Level;			// casting_proto.xml�еĵȼ�������Ҫ��װ��������������һ��
	UINT16	u16PotvalConsume;	// Ǳ��ֵ����
	INT		nProbability;		// �ɹ����ʣ�10000�ǰٷ�֮Ҽ��
	ERoleAttribute		eCastingRoleAtt[5];
	INT		nRoleAttValue[5];
	DWORD	dwStuffTypeID[5];
	INT		nStuffNum[5];
	tagCastingProto()
	{
		ZeroMemory(this,sizeof(*this));
	}
};

struct tagMsgDeliver
{
	DWORD	dwID;
	DWORD	dwIDMsgID;
	DWORD	dwMapID;
	INT	X;
	INT	Y;
	INT	Z;
};

// ����ʳ��׶ζ���
struct tagFabaoFoodParam
{
	INT	nMiniLevel;		// �ȼ����䣬��͵ȼ�
	INT nMaxLevel;			// �ȼ����䣬��ߵȼ�
	INT nRecoveryValue;	// ����ָ���
	tagFabaoFoodParam()
		: nMaxLevel(0),nMiniLevel(0),nRecoveryValue(0)	{}
};
// ����ʳ��ԭ�Ͷ���
struct tagFabaoFoodProto
{
	DWORD	dwItemTypeID;	// ����typeid
	INT		nLevel;			// Ҫ�������ȼ�
	INT		nExp;			// ����ʳ����⾭��
	DWORD	dwBuffID;		// ��Чbuffid
	DWORD	dwTriggerID;	// ��Чtriggerid
	tagFabaoFoodParam	param[3];
	tagFabaoFoodProto()
		: dwItemTypeID(GT_INVALID),nExp(0),nLevel(0),dwBuffID(GT_INVALID),dwTriggerID(GT_INVALID)
	{
	}
};
// ������ʵԭ��
struct tagFabaoFruitProto
{
	DWORD	dwTypeID;		// ��ʵtypeid
	INT		nLower;			// �������
	INT		nHight;			// �������
	INT		nExtType;		// �������ԣ�����������Ԫ���˺����߿���ֵ��ͬʱ�����и���������
	INT		nExtAdding;	// ��������
};

// �����ؼ�ԭ��
struct tagFabaoStuntProto
{
	DWORD	dwID;			// �����ؼ�id
	INT		nStuntType;	// �ؼ����ͣ���Ӧ��ĳ��Ԫ���˺�����
	DWORD	dwTriggerID;	// ������
	DWORD	dwBuffID;		// buff
	BOOL	bCanRealized;	// �Ƿ��������
	INT		nIntelligenceLimit;	// ��������
	INT		nStageLimit;		// �ȼ�����
	BOOL	bBinding;			// �Ƿ�󶨣��󶨵ģ����ᱻ������ѧϰ�������ʱ��
};

// ������������ԭ�� [3/13/2012 zhangzhihua]
const	INT FABAO_RESIST_UPGRADE_BASE = 10000;
struct tagFabaoAddPromoteTimesProto
{
	DWORD	dwID;			// Times�����������ֿ���10000��ʼ���� [3/13/2012 zhangzhihua]
	DWORD	dwMoney;		// ���Ľ�Ǯ [3/13/2012 zhangzhihua]
};

// ����ÿ�ս���ԭ��
struct tagGuildDailyRewardProto
{
	DWORD	dwID;
	INT		nGuildPos;		// ����ְλ
	INT		nWinNum;		// ռ��ս���Ĵ���
	DWORD	dwItemTypeID;	// �����typeid
	DWORD	dwBuffID;		// ״̬id
};

// �������Ա�ԭ��
struct tagGodBidProto
{
	DWORD 			dwProtoID;					// ����ԭ��ID
	DWORD 			dwItemID;					// ������ƷID
	DWORD 			dwItemCnt;					// ������Ʒ����
	EGodBidType	eGodBidType;				// ��������ö��
	DWORD			dwBasePrice;				// ��Ʒ
	DWORD			dwBidLowerLimit;		// ���۵���
};



// ������ֳ��ͷ�ֳ����ϸ��������ݽṹ
struct tagFabaoBirthItemType
{
	DWORD	dwType;	// �������ͣ���Ӧfabao_birth.xml���е�item_typex�ֶ�(xml����)�����߶�Ӧfabao_birth_type.xml�е�itemx
	INT		nPct;			// ����
};


template<typename T,INT num>
struct tagFabaoBirthFunc : public T 
{
	tagFabaoBirthItemType Items[num];
	DWORD GetItemByPct(INT nPct)
	{
		INT pct_total = 0;
		for( int i = 0; i < num; ++i )
		{
			pct_total += Items[i].nPct;
			if( nPct <= pct_total )
			{
				return Items[i].dwType;
			}
		}
		return GT_INVALID;
	}
	VOID PushBack(DWORD	dwType,INT nPct)
	{
		for( int i = 0; i < num; ++i )
		{
			if( !P_VALID(Items[i].dwType) )
			{
				Items[i].dwType = dwType;
				Items[i].nPct = nPct;
				break;
			}
		}
	}
private:
	void __copy(const tagFabaoBirthFunc<T,num> * p)
	{
		memcpy(Items,p->Items,sizeof(Items));
		// �������ʵ�ֵĺ���Copy
		Copy(p);
	}
public:
	tagFabaoBirthFunc(){}
	tagFabaoBirthFunc(const tagFabaoBirthFunc<T,num> & d)
	{
		__copy(&d);
	}
	tagFabaoBirthFunc<T,num> & operator= (const tagFabaoBirthFunc<T,num> & d)
	{
		__copy(&d);
		return *this;
	}
};


#define MFabaoBirthItemsCount 10
// ������ֳ��ԭ��
struct tagFabaoBirthProto
{
	DWORD	dwID;
	INT		nLowLimit;
	INT		nHigherLimit;
	VOID Copy(const tagFabaoBirthProto * p)
	{
		dwID = p->dwID;
		nLowLimit = p->nLowLimit;
		nHigherLimit = p->nHigherLimit;
	}
};

typedef tagFabaoBirthFunc<tagFabaoBirthProto,MFabaoBirthItemsCount>		TFabaoBirthProto;

#define MFabaoBirthItemTypes	6

struct tagFabaoBirthItem
{
	DWORD	dwMainFabaoTypeID;
	VOID Copy(const tagFabaoBirthItem * p)
	{
		dwMainFabaoTypeID = p->dwMainFabaoTypeID;
	}
};
typedef tagFabaoBirthFunc<tagFabaoBirthItem,MFabaoBirthItemTypes>		TFabaoBirthItem;
// ������ֳ����ϸ��
struct tagFabaoBirthItemTypeProto
{
	DWORD	dwID;
	INT		nItemType;		// ע���������Ϊmap������
	map<DWORD,TFabaoBirthItem> Items;
};

const INT32 GOLD_EGG_MAX_CNT = 20;
struct tagGoldEggPctProto
{
	DWORD dwLayer;			// ����
	INT n32EmptyPct;		// �յ�����
	INT n32ItemPct[GOLD_EGG_MAX_CNT];  // �������ҳ�����Ʒ�ļ�����Ϣ
};

struct tagGoldEggProto
{
	DWORD dwLayer;			// ����
	INT nGenConsum;			// ��ͨ����
	INT nSpecConsum;			// �ʵ�����
	DWORD dwItemID[GOLD_EGG_MAX_CNT];	// �������ҳ�����Ʒ
};

struct tagAdvCompMaterial
{
	DWORD dwTypeID;
	DWORD dwCount;
};
struct tagFashionComp
{
	DWORD dwTypeID;
	DWORD dwPct;
};

const INT32 MAX_ADVANCED_COMPOSE_MATERIAL = 2; // �߼��ϳ���Ҫ������
const INT32 MAX_FASHION_CAN_COMPOSE = 20; // �߼��ϳ���Ҫ������
struct tagAdvancedComposeProto
{
	DWORD dwComposeID;									// �ϳ�ID
	EAdvanceComposeType eAdvCompType;		// �߼��ϳ�����
	DWORD dwDesignID;										//  ���ͼID
	tagAdvCompMaterial Material[MAX_ADVANCED_COMPOSE_MATERIAL];
	INT64 n64SilverCost;										// ��Ǯ����
	INT nVitalityCost;									// ��������
	tagFashionComp dwFashionComp[MAX_FASHION_CAN_COMPOSE];
};

struct tagGodHeadProto
{
	DWORD	dwID;
	INT		nMinLevel;			// �ȼ�����
	INT		nMaxLevel;			// �ȼ�����
	INT		nCondenseValue;		// ��ǰ������Ҫ���ģ��������۶�
	INT		nCanActNum;			// �ܹ��������������
};


struct tagTigerProto
{
	DWORD dwId;										// ��4�� 1 ��5 ��10�� 50
	DWORD dwRate[MAX_TIGER_NUM];		// �����ĸ���
	DWORD dwNum[MAX_TIGER_NUM];		// ����������
};

struct tagExpOfflineProto
{
	DWORD dwID;										
	DWORD dwExpbase;		
	DWORD dwExpAdditional;	
	DWORD dwGold;
};

struct tagForceSkillProto
{
	DWORD	dwID;										
	BYTE	bySide;		
	DWORD	dwJob;
	DWORD	dwLevel;
	DWORD	dwNeedSkill;
	BOOL	bForerver;
	DWORD	dwOpenSoul;
	DWORD	dwOpenMoney;
	DWORD	dwContinueSoul;
	DWORD	dwContinueMoney;
	BOOL	bDelete;
};

struct tagForceLevelProto
{
	DWORD dwID;										
	DWORD dwPoint;
};


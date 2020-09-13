//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall_define.h
// author: 
// actor:
// data: 2009-1-5
// last:
// brief: �̳���ض���
//--------------------------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//--------------------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------------------
const INT	MALL_PACK_ITEM_NUM		= 5;	// ��Ʒ���������Ʒ������
const INT	Max_PackName_Length		= 10;	// ����������10����
const INT	Max_LeaveWord_Length		= 20;	//����20����
const INT	MAX_PromoteItem			= 5;	//�Ƽ���Ʒ����
const static INT   MAX_PromoteItemNum     = 10;   //��ʾ�Ƽ���Ʒͼ�������Ŀ
const static INT   MAX_PromoteItemPage	   = 2;    //��ʾ�Ƽ���Ʒͼ�����ҳ����
const INT64 MAX_YB_NUM				= 9999999;		// Ԫ�������˻����Ԫ����
const INT64 MAX_SILVER_NUM			= 9999999999;	// Ԫ�������˻����������
#define MALL_ITEM_PERSONAL_NUM_LIMIT_LINE	10000000 // ���ڽ�ɫ�ڹ����̳���Ʒ����������ص�
const INT	MAX_MALL_CART_NUM		= 6;	// �̳ǹ��ﳵ��Ʒ������ [6/5/2012 zhangzhihua]

enum EMallCfgAttr
{
	EMCA_BuyActivityOpen			= 0,	//�̳����ѻ����
	//EMCA_BuyActivityBeginTime,				//���ʼʱ��
	//EMCA_BuyActivityEndTime,				//�����ʱ��
	EMCA_BuyActivityRewardLine,				//������������ʥ��
	EMCA_BuyActivityRewardItem0,			//���������1
	EMCA_BuyActivityRewardItem1,
	EMCA_BuyActivityRewardItem2,
	EMCA_BuyActivityRewardItem3,
	//EMCA_BuyActivityRewardItem4,
	EMCA_BuyActivityRewardItemNum0,			//���������1����
	EMCA_BuyActivityRewardItemNum1,
	EMCA_BuyActivityRewardItemNum2,
	EMCA_BuyActivityRewardItemNum3,
	//EMCA_BuyActivityRewardItemNum4,

	EMCA_End,
};

//--------------------------------------------------------------------------------------------
// ��̬�ṹ
//--------------------------------------------------------------------------------------------
struct tagMallItemProtoBase
{
	DWORD			dwID;							// ID
	INT				nPrice;							// ����
	INT				nSalePrice;						// ������
	DWORD			dwTimeSaleEnd;					// ������ֹʱ��
	DWORD			dwPresentID;					// ��ƷID
	INT				nNum;							// ��������
	INT				nPresentNum;					// ��Ʒ����
	INT				nExAssign;						// ������������
	INT				nExNum;							// ��Ҫ��������
	BYTE			bySmallGroupHeadcount;			// С���Ź�����
	BYTE			bySmallGroupDiscount;			// С���Ź��ۿ�[0��100]
	BYTE			byMediumGroupHeadcount;			// �����Ź�����
	BYTE			byMediumGroupDiscount;			// �����Ź��ۿ�
	BYTE			byBigGroupHeadcount;			// �����Ź�����
	BYTE			byBigGroupDiscount;				// �����Ź��ۿ�
	bool			bNew	: 1;					// �Ƿ�Ϊ��Ʒ
	bool			bHot	: 1;					// �Ƿ�����
	INT				nIndexInServer;					// �ڷ������е�λ��
	DWORD			dwPersistTime;					// �Ź�����ʱ��
};


struct tagMallItemProto: public tagMallItemProtoBase
{
	DWORD			dwTypeID;						// ��ƷTypeID
	INT8			n8Kind;							// ��������

	INT8            n8Kind2;                        // �������� 
	BYTE			byRank;							// ��������
	BYTE			byGroupPurchaseAmount;			// �Ź�����
	BYTE			byDummy[1];
};

struct tagMallPromoteItemShow
{
	INT   iPage;
	INT	  iNum;
	DWORD  dwProItemID;
};

struct tagMallItemPrototeGood
{
	INT         id;                          //���
	INT			iMinLevel;						// ��С�ȼ�
	INT        iMaxLevel;						// ���ȼ�
	DWORD	   dwItemID[MAX_PromoteItemNum];    // ��ʾ����Ʒ
	tagMallItemPrototeGood()
	{
		for(INT i=0; i<MAX_PromoteItemNum; ++i)
		{
			dwItemID[i] = GT_INVALID;
		}
	}
};

struct tagMallPackProto: public tagMallItemProtoBase
{
	DWORD			dwTypeID[MALL_PACK_ITEM_NUM];	// ��ƷID
	INT				nItemPrice[MALL_PACK_ITEM_NUM];	// ������Ʒ�۸�(�ܼ�)
	INT			byItemNum[MALL_PACK_ITEM_NUM];	// ��Ʒ����
	INT8			n8ItemKind;						// ��Ʒ����(ID)����
	BYTE			byDummy[2];
	TCHAR			tszPackName[Max_PackName_Length];// �������
	tagMallPackProto()
	{
		ZeroMemory(this, sizeof(*this));
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			dwTypeID[i] = GT_INVALID;
		}
	}
};

struct tagMallFreeItem
{
	DWORD		dwTypeID;	// ��ƷID
	INT			nUnitPrice;	// ԭ����
	BYTE		byNum;		// ��ȡ����

	tagMallFreeItem()
	{
		Clear();
	}

	VOID Clear()
	{
		dwTypeID	= GT_INVALID;
		nUnitPrice	= 0;
		byNum		= 0;
	}
};

//--------------------------------------------------------------------------------------------
// �Ƽ���Ʒ���ݽṹ��
//--------------------------------------------------------------------------------------------
struct  tagRecommendingItemInfo
{
    DWORD    dwID ;
	INT16    n16MinLevel;
	INT16    n16MaxLevel;

	DWORD    dwItem1TypeID;
	DWORD    dwItem2TypeID;
	DWORD    dwItem3TypeID;
	DWORD    dwItem4TypeID;
	DWORD    dwItem5TypeID;

	DWORD    dwItem6TypeID;
	DWORD    dwItem7TypeID;
	DWORD    dwItem8TypeID;
	DWORD    dwItem9TypeID;
	DWORD    dwItem10TypeID;	

	tagRecommendingItemInfo()
	{
		memset(this,0,sizeof(*this));
	}
};
//--------------------------------------------------------------------------------------------
// ���������ݽṹ
//--------------------------------------------------------------------------------------------
//struct tagMallGoods
//{
//	DWORD	dwID;		// ��Ʒ����Ʒ��ID
//	BYTE	byCurNum;	// ��ǰ��������(GT_INVALID��ʾ������)
//	BYTE	byDummy[3];
//};

struct tagMallGoods
{
	union
	{
		const tagMallItemProto	*pMallItem;
		const tagMallPackProto	*pMallPack;
		const tagMallFreeItem	*pMallFreeItem;
	};

	INT	nCurNum;	// ��ǰ��������(GT_INVALID��ʾ������)
	//BYTE	byDummy[3];
};

//--------------------------------------------------------------------------------------------
// �ͻ������������Ʒ����ͬ���ṹ
//--------------------------------------------------------------------------------------------
struct tagMallUpdate
{
	DWORD		dwID;					//��ƷID
	INT 		byRemainNum;			//ʣ�����
};

//--------------------------------------------------------------------------------------------
// �����Ź�
//--------------------------------------------------------------------------------------------
enum EGPSCOPE
{
	EGPS_NULL		= -1,
	EGPS_SMALL,							//С���Ź�
	EGPS_MEDIUM,						//�����Ź�
	EGPS_BIG,							//�����Ź�
};

struct tagGroupPurchase
{
	DWORD			dwGuildID;			//����ID
	DWORD			dwRoleID;			//������ID
	DWORD			dwMallID;			//��ƷID
	INT32			nPrice;				//�Ź��۸�
	INT16			nParticipatorNum;	//��Ӧ����
	INT16			nRequireNum;		//�Ź���������
	DWORD			dwRemainTime;		//ʣ��ʱ��
	TList<DWORD>*	listParticipators;	//��Ӧ���б�

	tagGroupPurchase()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~tagGroupPurchase()
	{
		SAFE_DEL(listParticipators);
	}
};

enum EYBOTYPE							//Ԫ����������
{
	EYBOT_BUY,							//�չ�
	EYBOT_SELL,							//����
};

enum EYBOMODE							//Ԫ������״̬
{
	EYBOM_Submit,						//ί����
	EYBOM_Cancel,						//����
	EYBOM_Close,						//�ر�
};

//--------------------------------------------------------------------------------------------
// Ԫ�����׶���
//--------------------------------------------------------------------------------------------
struct tagYuanBaoOrder
{
	DWORD				dwID;					//����ID
	DWORD				dwRoleID;				//���ID
	EYBOTYPE			eYBOType;				//��������
	INT					nPrice;					//����
	INT					nNum;					//����
	EYBOMODE			eYBOMode;				//Ԫ������״̬
	tagDWORDTime		dwStartTime;			//��ʼʱ��
	tagDWORDTime		dwEndTime;				//����ʱ��
	INT					nAvgPrice;				//�ɽ�����
	INT					nDealNum;				//�ɽ�����
	tagYuanBaoOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwStartTime = GT_INVALID;
		dwEndTime = GT_INVALID;
	}

	INT		GetRemainNum() { return nNum - nDealNum; }	
};

//--------------------------------------------------------------------------------------------
// ����Ԫ�����׶���(�������ݿ�ʱʹ�ã�
//--------------------------------------------------------------------------------------------
struct tagYBOrder
{
	DWORD				dwID;
	DWORD				dwRoleID;
	INT16				nType;
	INT					nPrice;
	INT					nNum;
	INT16				nYBMode;
	DWORD				dwStartTime;
	tagYBOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwStartTime = GT_INVALID;
	}
};

// Ԫ�����׼۸��б�
struct tagYBPriceList
{
	INT					nPrice;
	INT					nNum;
};
 
// Ԫ�������˻�
struct tagYBAccount
{
	DWORD				dwRoleID;				//���ID
	INT					nYuanBao;				//Ԫ������
	INT					nGold;					//�������
	INT					nSilver;				//��������
	BOOL				bSellOrder;				//�Ƿ��г��۶���
	BOOL				bBuyOrder;				//�Ƿ����չ�����
};

// ������ر�Ԫ�����׶���
struct tagCompleteOrder
{
	DWORD				dwID;
	INT16				nYBOMode;
	DWORD				dwEndTime;
	tagCompleteOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwEndTime = GT_INVALID;
	}
};

// ����Ԫ�����׶���
struct tagRefresh
{	
	DWORD				dwID;
	INT					nAvgPrice;
	INT					nDealNum;
	tagRefresh()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
	}
};

// �����ڽ����ϵ���Ʒ��Ϣ
struct tagUIItem
{
	DWORD dwTypeID;
	DWORD dwUIType;
	tagUIItem()
	{
		ZeroMemory(this, sizeof(*this));
		dwTypeID = GT_INVALID;
		dwUIType = GT_INVALID;
	}
};

// ���ͻ��˷��͵�һԪ�ƻ���Ʒԭ��
struct tagMallOfferProto2Client
{
	DWORD dwTypeID;
	UINT16 nLevel;
	tagMallOfferProto2Client()
	{
		ZeroMemory(this, sizeof(*this));
		dwTypeID = GT_INVALID;
		nLevel = GT_INVALID;
	}
};

// 1Ԫ�ƻ�
const INT MAX_GIFT_PER_PAGE = 4; //ÿҳ��ʾ
const INT LEVEL_NUM_CHOOSE	= 4; //�ɹ�ѡ��ĵȼ�
const INT PLAN_GIFT_TYPE_NUM = 10;//1Ԫ�ƻ���Ӧһ���������
// һԪ�ƻ���Ʒԭ��
struct tagMallOfferProto
{
	DWORD dwID;
	DWORD dwPrice;
	tagMallOfferProto2Client MallOfferProto2Client;

	tagMallOfferProto()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		MallOfferProto2Client.dwTypeID = GT_INVALID;
		MallOfferProto2Client.nLevel = GT_INVALID;
		dwPrice = GT_INVALID;
	}
};

struct tagBuyMallItemCountSaveData
{
	//DWORD dwRoleID;
	DWORD dwTypeID;
	INT nNum;
};

//--------------------------------------------------------------------------------------------
#pragma pack(pop)
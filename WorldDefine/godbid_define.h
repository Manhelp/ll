#pragma once

const int MAX_GODBID_ITEM_NUM = 3;

// ��֮����ϵͳ����
enum EGodBidType
{
	EGBT_Start = -1,

	EGBT_Normal = 0,								// ��ͨģʽ
	EGBT_BlackMarket_Gold = 1,				// ���н��
	EGBT_BlackMarket_ShengBi = 2,		// ����Ԫ��

	EGBT_End
};

struct tagBidData
{
	DWORD	dwID;						// id
	DWORD  dwItemID;				// ��ƷID
	int			nNum;						//	����
	EGodBidType	eType;						// ����
	DWORD	dwStartPrice;			// ��ʼ��
	DWORD	dwMinBidPrice;		// ���۵���
	tagBidData():dwID(0),dwItemID(0),nNum(0),eType(EGBT_Start),dwStartPrice(0), dwMinBidPrice(0){}
	tagBidData& operator=(const tagBidData&rv)
	{
		if(this==&rv)
			return *this;

		dwID = rv.dwID;
		dwItemID = rv.dwItemID;
		nNum = rv.nNum;
		eType = rv.eType;
		dwStartPrice = rv.dwStartPrice;
		dwMinBidPrice = rv.dwMinBidPrice;
		return *this;
	}

	void clear()
	{
		dwID = 0;
		dwItemID = 0;
		eType = EGBT_Start;
		dwStartPrice = 0;
		dwMinBidPrice = 0;
	}
};

// ��ͨ������Ʒ��Ϣ
struct tagGodBidItemInfo
{
	tagBidData	sBidData;				
	INT64		n64CurPrice;				// ��ǰ�۸�
	bool			bHighestPrice;				// �Ƿ���߳�����
	INT64		n64PlayerOfferPrice;	// ����Ѿ����ļ�Ǯ

	tagGodBidItemInfo():n64CurPrice(0),n64PlayerOfferPrice(0),bHighestPrice(false){}
 	tagGodBidItemInfo& operator=(const tagGodBidItemInfo& rv)
 	{
		if(this==&rv)
			return *this;

		sBidData = rv.sBidData;
 		n64CurPrice = rv.n64CurPrice;
 		bHighestPrice = rv.bHighestPrice;
 		n64PlayerOfferPrice = rv.n64PlayerOfferPrice;
 		return *this;
 	}

	void clear()
	{
		n64CurPrice = 0;
		bHighestPrice = false;
		n64PlayerOfferPrice = 0;
		sBidData.clear();
	}
};

// ���о�����Ʒ��Ϣ
struct tagBlackMarketItemInfo
{
	tagBidData	sBidData;				
	bool				bIsBid;							// �Ƿ��Ѿ���

	tagBlackMarketItemInfo():bIsBid(false){}
	tagBlackMarketItemInfo& operator= (const tagBlackMarketItemInfo& rv)
	{
		if(this==&rv)
			return *this;

		sBidData = rv.sBidData;
		bIsBid = rv.bIsBid;
		return *this;
	}

	void clear()
	{
		bIsBid = false;
		sBidData.clear();
	}
};
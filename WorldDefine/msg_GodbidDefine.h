#pragma once
#include "godbid_define.h"
#pragma pack(push, 1)

enum EGodBidErrorCode
{
	EGBEC_Success = 0,
	EGBEC_Black_Only_One_Chance = 1,		// ���о���ֻ��һ�λ���
	EGBEC_Black_OutOfTime = 1,				// ���ھ���ʱ��
};

//-----------------------------------------------------------------------------
// ����֮������
//-----------------------------------------------------------------------------
CMD_START(NC_GodBiderShopOpen)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_GodBiderShopOpen)
	DWORD						dwErrorCode;
	tagGodBidItemInfo		bidItemInfo[MAX_GODBID_ITEM_NUM];	// ������Ʒ��Ϣ
	DWORD						dwLeftTime;	// ��ʣ�����ʱ��������������
CMD_END

//-----------------------------------------------------------------------------
// ȷ�Ͼ���(��ͨ����)
//-----------------------------------------------------------------------------
CMD_START(NC_ConformPrice)
	INT64			n64Price;				//	��Ǯ
	DWORD		dwFormulaID;		// ��ʽID
CMD_END

CMD_START(NS_ConformPrice)
	DWORD		dwErrorCode;
	INT64			n64Price;				//	��Ǯ
CMD_END

//-----------------------------------------------------------------------------
// �򿪺�������
//-----------------------------------------------------------------------------
CMD_START(NC_BlackMarketShopOpen)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_BlackMarketShopOpen)
	DWORD								dwErrorCode;
	tagBlackMarketItemInfo		GoldItemInfo;			// ��Ϸ����Ʒ��Ϣ
	tagBlackMarketItemInfo		ShengBiItemInfo;	// ʥ����Ʒ��Ϣ
	DWORD								dwLeftTime;			// ��ʣ�����ʱ��������������
CMD_END

//-----------------------------------------------------------------------------
// ȷ�Ͼ���(��������)
//-----------------------------------------------------------------------------
CMD_START(NC_BlackConPrice)
	INT64			n64Price;				//	��Ǯ
	DWORD		dwFormulaID;		// ��ʽID������������ʥ�һ��ǽ𣬿����ڹ�ʽ�в��ң�
CMD_END

CMD_START(NS_BlackConPrice)
	DWORD		dwErrorCode;
	bool				bIsGod;				// ture �ǽ�false��ʥ��
	INT64			n64Price;				//	��Ǯ
CMD_END

#pragma pack(pop)
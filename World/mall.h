//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall.h
// author: Sxg
// actor:
// data: 2009-1-22
// last:
// brief: �̳��� -- �ɶ�̬�����̳���Ʒ
//-----------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/msg_mall.h"
class Role;
class GuildPurchase;

struct tagItem;
//-----------------------------------------------------------------------------
// �����д�����������
//-----------------------------------------------------------------------------
struct tagMallItemSell
{
	tagItem		*pItem;
	tagItem		*pPresent;
	INT			nYuanBaoNeed;		//�һ�ʱ������������
	INT32		nExVolumeAssign;
	INT32       nVipPoint;
	INT32		byRemainNum;
	
	tagMallItemSell() { ZeroMemory(this, sizeof(*this)); }
};

struct tagMallPackSell
{
	tagItem		*pItem[MALL_PACK_ITEM_NUM];
	tagItem		*pPresent;
	INT			nYuanBaoNeed;		//�һ�ʱ������������
	INT32		nExVolumeAssign;
	INT32       nVipPoint;
	INT32		byRemainNum;

	
	tagMallPackSell() { ZeroMemory(this, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// �̳���Ʒ����
//----------------------------------------------------------------------------
enum EMallItemType
{
	EMIT_Item,			// ��Ʒ
	EMIT_Pack,			// �����Ʒ
	EMIT_FreeItem,		// �����Ʒ

	EMIT_End
};

//-----------------------------------------------------------------------------
// �̳���
//-----------------------------------------------------------------------------
class Mall
{
public:
	Mall();
	~Mall();

	// �򿪹ر��̳�
	BOOL Init();
	VOID Destroy();

	// ������Դ�������´��̳�
	DWORD ReInit();

	// �Ź���Ϣ����
	VOID Update();

public:
	BOOL IsInitOK()			const { return m_bInitOK; }
	INT  GetItemNum()		const { return m_nItemNum; }
	INT  GetPackNum()		const { return m_nPackNum; }
	INT	 GetFreeItemNum()	const { return (m_pMallFreeItem->pMallFreeItem->dwTypeID != GT_INVALID) ? 1 : 0; }

	DWORD GetMallTime()		const { return m_dwLoadTime; }

	const tagMallGoods* GetMallItem(BYTE byIndex, EMallItemType eType = EMIT_Item);

public:
	// ͬ��
	DWORD GetAllItems(OUT LPVOID pData);
	DWORD GetAllPacks(OUT LPVOID pData);
	DWORD GetFreeItem(OUT LPVOID pData);
	DWORD UpdateAllItems(OUT LPVOID pData, OUT INT &nRefreshNum);
	DWORD UpdateAllPacks(OUT LPVOID pData, OUT INT &nRefreshNum);
		
	// ���ﳵ [6/5/2012 zhangzhihua]
	DWORD CanSellCart(Role *pRole, BYTE byCartNum, DWORD* dwIDArray, INT* nIndexArray, INT16* n16BuyNumArray, DWORD &dwPrice);

	// ����                                      
	DWORD SellItem(Role *pRole, DWORD dwToRoleID, DWORD dwCmdID, DWORD dwID, INT nIndex,
				INT nUnitPrice, INT16 n16BuyNum, OUT tagMallItemSell &itemSell,EShoppingDistrict eShopingDistrict, INT16 n16OfferItemLvl);
	DWORD SellPack(Role *pRole, DWORD dwToRoleID, DWORD dwCmdID, DWORD dwID, BYTE byIndex, 
				INT nUnitPrice, OUT tagMallPackSell &packSell, BOOL bNeedCheckBagSpace,EShoppingDistrict eShopingDistrict);

	// �һ�
	DWORD ExchangeItem(Role *pRole, DWORD dwCmdID, DWORD dwID, INT nIndex, 
		INT nPrice, INT16 n16BuyNum, OUT tagMallItemSell &itemSell);
	DWORD ExchangePack(Role *pRole, DWORD dwCmdID, DWORD dwID, INT nIndex, 
		INT nPrice, OUT tagMallPackSell &packSell);

	// ��ѷ���
	DWORD GrantFreeItem(Role *pRole, DWORD dwID, OUT tagMallItemSell &itemSell);

	// �����Ź���Ϣ����
	DWORD LoadAllGPInfo(INT nGPInfoNum, LPVOID pData);
	DWORD LaunchGroupPurchase(Role *pRole, DWORD dwID, BYTE byScope,
		BYTE byIndex, INT nUnitPrice);
	DWORD RespondGroupPurchase(Role *pRole, DWORD dwGuildID, DWORD dwID, DWORD dwRoleID,
		INT nPrice);
	DWORD GetAllSimGPInfo(Role* pRole);
	DWORD GetParticipators(Role* pRole, DWORD dwGuildID, DWORD dwID, DWORD dwRoleID);
	VOID RemoveGuildPurchaseInfo(DWORD dwGuildID);

	//������Ʒ����
	DWORD InitLimitNumItemInfo(INT nItemInfoNum, LPVOID pData);
	VOID  SaveToDB(DWORD dwID,DWORD dwTypeID,INT nNum);
	BOOL IsInActivity();//�Ƿ��ڻ��
	//�Ƿ����Ԫ������֧��
	BOOL CanUseYuanBaoDaiBi(Role* pRole, INT nPrice);

private:
	VOID InitItem();
	VOID InitPack();
	BOOL CheckPack();

	BOOL IsActivityOpen();//��Ƿ��
	tagDWORDTime ConvertCfgTimeToDWORDTime(DWORD dwTime);
	VOID SetMallActivityOn(BOOL bVal);

public:
	// log
	VOID LogMallSell(DWORD dwBuyRoleID, DWORD dwToRoleID, 
					 const tagItem& item, INT64 n64Serial, INT16 n16Num, 
					 DWORD dwFstGainTime, INT nCostYuanBao, INT nCostExVolume,INT nCostVIPPoint,DWORD dwCmdID);
	VOID LogMallSellPack(DWORD dwPackID, DWORD dwBuyRoleID, DWORD dwToRoleID, INT nCostYuanBao);

private:
	BOOL			m_bInitOK;			// �̳ǿ���״̬
	DWORD			m_dwLoadTime;		// ��ȡ�̳���Ʒԭ��ʱ��(tagDwordTime)
	DWORD			m_dwTimeKeeper;		// ��ʱ��
	BYTE			m_byMinuteTime;		// ���Ӽ�ʱ

private:
	INT				m_nItemNum;			// ��Ʒ����
	INT				m_nPackNum;			// ��Ʒ������
	tagMallGoods	*m_pMallItem;		// ��Ʒ����ָ��
	tagMallGoods	*m_pMallPack;		// ��Ʒ������ָ��
	tagMallGoods	*m_pMallFreeItem;	// �����ȡ��Ʒ

private:
	// �����Ź���Ϣ����
	TMap<DWORD, GuildPurchase*>	                  m_mapGuildPurchase;
	TList<tagMallGoods*>                          m_listLimitItemInfo;

private:
	// ͳ��
	INT				m_nFreeGrantNum;
};

extern Mall g_mall;
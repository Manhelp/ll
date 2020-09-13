//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_mall.cpp
// author: Sxg
// actor:
// data: 2009-2-1
// last:
// brief: �̳Ǵ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_mall.h"
#include "../WorldDefine/mall_define.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "role.h"
#include "mall.h"
#include "role_mgr.h"
#include "TradeYuanBao.h"
#include "guild.h"
#include "guild_mgr.h"
#include "world.h"
#include "item_creator.h"

//-----------------------------------------------------------------------------
// ��ȡ�̳���������Ʒ
//-----------------------------------------------------------------------------
DWORD Role::GetMallAll(OUT DWORD &dwMallTime)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// ��ȡ�������̳Ǽ���ʱ��
	dwMallTime = g_mall.GetMallTime();

	INT nGoodsNum, nSzMsg;

	// ��ͨ��Ʒ
	nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallItem) - 1 + sizeof(tagMallItemProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallItem);
		pSend->nItemNum = nGoodsNum;
		g_mall.GetAllItems(pSend->byData);

		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	// ��Ʒ��
	nGoodsNum = g_mall.GetPackNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallPack) - 1 + sizeof(tagMallPackProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallPack);
		pSend->nPackNum = nGoodsNum;
		g_mall.GetAllPacks(pSend->byData);

		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}
	
	// �����ȡ��Ʒ(ֻ��1��)
	nGoodsNum = g_mall.GetFreeItemNum();
	if(nGoodsNum > 0)
	{
		tagNS_MallFreeItem send;
		g_mall.GetFreeItem(&send.freeItem);
		SendMessage(&send, send.dwSize);
	}

	//���Ƽ���Ʒ
    nGoodsNum = g_attRes.GetMallRecommendingItemNum();
	if ( nGoodsNum > 0)
	{
		 nSzMsg = sizeof(tagNS_MallRecommendingItem) - 1 + sizeof(tagRecommendingItemInfo)*nGoodsNum;
		 MCREATE_MSG(pSend,nSzMsg,NS_MallRecommendingItem);
		 pSend->nItemNum = nGoodsNum;

		 TMap<DWORD, tagRecommendingItemInfo*> mapRecommendingItemInfo = g_attRes.GetMallRecommendingItem();
         tagRecommendingItemInfo* pRecommendingItemInfo = NULL;
		 INT nNum = 0 ;
		 MTRANS_POINTER(p, pSend->byData, tagRecommendingItemInfo);
		 TMap<DWORD, tagRecommendingItemInfo*>::TMapIterator it = mapRecommendingItemInfo.Begin();
		 while (mapRecommendingItemInfo.PeekNext(it,pRecommendingItemInfo))
		 {

			 if( P_VALID(pRecommendingItemInfo) )
			 {
                 memcpy(&p[nNum],pRecommendingItemInfo,sizeof(tagRecommendingItemInfo));
				 nNum++;
			 }
             if ( nNum >= nGoodsNum)
             {
				 break;
             }
		 }
		 SendMessage(pSend,pSend->dwSize);
		 MDEL_MSG(pSend);
		

	}

	tagNS_MallUpdateCfg sendMallCfg;
	for (INT i = 0; i < EMCA_End; i++)
	{
		sendMallCfg.dwMallCfg[i] = g_attRes.GetMallCfg((EMallCfgAttr)i);
	}

	SendMessage(&sendMallCfg, sendMallCfg.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̳����г��۸������Ƶ�������Ʒ����
//-----------------------------------------------------------------------------
DWORD Role::UpdateMallAll(OUT DWORD &dwNewMallTime, IN DWORD dwOldMallTime)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// ��ȡ�������̳Ǽ���ʱ��
	dwNewMallTime = g_mall.GetMallTime();

	//-- ���ͻ��˵��̳Ǿ�̬�����Ƿ����������ͬ

	// 1.��ͬ�����·����̵�ԭ����Ϣ
	if(dwNewMallTime != dwOldMallTime)
	{
		return GetMallAll(dwNewMallTime);
	}

	// 2.��ͬ��ֻˢ���и������Ƶ���Ʒ������Ϣ
	INT nGoodsNum, nSzMsg;

	// ��ͨ��Ʒ
	nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);
		g_mall.UpdateAllItems(pSend->byData, pSend->nItemNum);

		// ���¼�����Ϣ��С
		if(pSend->nItemNum > 0)
		{
			pSend->dwSize = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate) * pSend->nItemNum;
			SendMessage(pSend, pSend->dwSize);
		}

		MDEL_MSG(pSend);
	}

	// ��Ʒ��
	nGoodsNum = g_mall.GetPackNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);
		g_mall.UpdateAllPacks(pSend->byData, pSend->nItemNum);

		// ���¼�����Ϣ��С
		if(pSend->nItemNum > 0)
		{
			pSend->dwSize = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate) * pSend->nItemNum;
			SendMessage(pSend, pSend->dwSize);
		}

		MDEL_MSG(pSend);
	}

	tagNS_MallUpdateCfg sendMallCfg;
	for (INT i = 0; i < EMCA_End; i++)
	{
		sendMallCfg.dwMallCfg[i] = g_attRes.GetMallCfg((EMallCfgAttr)i);
	}

	SendMessage(&sendMallCfg, sendMallCfg.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̳���Ʒ
//-----------------------------------------------------------------------------
DWORD Role::BuyMallItem(DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict, INT16 n16OfferItemLvl, ELogCmdID eCmdID)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellItem(this, GetID(), eCmdID, 
										dwID, nIndex, nUnitPrice, n16BuyNum, sItemSell,eShoppingDistrict, n16OfferItemLvl);

	// ������
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// Ԫ�������̳��п۳�
		
		// ����Ʒ�ŵ���ұ�����
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCID_Mall_BuyItem, TRUE);
		
		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sItemSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, 
							GetNameID(), GT_INVALID, EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCID_Mall_BuyItem_Add);

		}

		// �����������
		if (sItemSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sItemSell.nVipPoint, ELCID_Mall_BuyItem);
		}
	}

	// ��һ��һ�����ߵ��¼��ӿ�
	if (P_VALID(sItemSell.pItem))
	{
		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, sItemSell.pItem->dwTypeID, sItemSell.pItem->eCreateMode);
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);
		
		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̳���Ʒ��
//-----------------------------------------------------------------------------
DWORD Role::BuyMallPack(DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShoppingDistrict)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallPackSell sPackSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellPack(this, GetID(), ELCID_Mall_BuyPack, 
										dwID, byIndex, nUnitPrice, sPackSell, TRUE,eShoppingDistrict);

	// ������
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{
		// ����Ʒ�ŵ���ұ�����
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			GetItemMgr().Add2Bag(sPackSell.pItem[i], ELCID_Mall_BuyPack, TRUE);
		}

		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sPackSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
								GetNameID(), GT_INVALID, EBBRT_Mall, sPackSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCID_Mall_BuyPack_Add);

		}

		// �����������
		if (sPackSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sPackSell.nVipPoint, ELCID_Mall_BuyPack);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̳���Ʒ������
//-----------------------------------------------------------------------------
DWORD Role::BuyMallItem(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
						DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// ������ID����Ϸ�������Ƿ����
	if(!g_roleMgr.IsRoleBelongToWorld(dwTgtRoleID))
	{
		return E_Role_Not_ExistIn_World;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellItem(this, dwTgtRoleID, ELCID_Mall_PresentItem, 
										dwID, nIndex, nUnitPrice, n16BuyNum, sItemSell,eShoppingDistrict, 0);

	// ������
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// �ٱ�����ʷ��¼
		GetItemMgr().ProcBaiBaoRecord(sItemSell.pItem->dwTypeID, dwTgtRoleID, 
							GetNameID(), EBBRT_Friend, sItemSell.pItem->dw1stGainTime, szLeaveWord);

		// ����Ʒ�ŵ����Ѱٱ�����
		Role *pFriend = g_roleMgr.GetRolePtrByID(dwTgtRoleID);
		if(P_VALID(pFriend))
		{
			pFriend->GetItemMgr().Add2BaiBao(sItemSell.pItem, ELCID_Mall_PresentItem, FALSE, GetID());
		}
		else
		{
			// �洢��item_baibao����
			ItemMgr::InsertBaiBao2DB(sItemSell.pItem, dwTgtRoleID, ELCID_Mall_PresentItem);
			
			// ɾ����Ʒ
			::Destroy(sItemSell.pItem);
		}

		// �������Ʒ����ŵ����Ѱٱ�����
		if(P_VALID(sItemSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, dwTgtRoleID, 
										GetNameID(), EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCID_Mall_PresentItem_Add);
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sItemSell.pPresent, dwTgtRoleID, ELCID_Mall_PresentItem_Add);

				// ɾ����Ʒ
				::Destroy(sItemSell.pPresent);
			}
		}
		
		// ����һ�������
		if (sItemSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sItemSell.nVipPoint, ELCID_Mall_PresentItem);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������Ʒ��Ʒ��������
//-----------------------------------------------------------------------------
DWORD Role::BuyMallPack(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
						DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShoppingDistrict)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// ������ID����Ϸ�������Ƿ����
	if(!g_roleMgr.IsRoleBelongToWorld(dwTgtRoleID))
	{
		return E_Role_Not_ExistIn_World;
	}

	tagMallPackSell sPackSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.SellPack(this, dwTgtRoleID, ELCID_Mall_PresentPack, 
										dwID, byIndex, nUnitPrice, sPackSell, FALSE,eShoppingDistrict);

	// ������
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{		
		Role *pFriend = g_roleMgr.GetRolePtrByID(dwTgtRoleID);

		// ����Ʒ�ŵ����Ѱٱ����� -- item_baibao��
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			// ��¼����һ����Ʒ��
			if(0 == i)
			{
				// �ٱ�����ʷ��¼
				GetItemMgr().ProcBaiBaoRecord(sPackSell.pItem[i]->dwTypeID, 
					dwTgtRoleID, GetNameID(), EBBRT_Friend, sPackSell.pItem[i]->dw1stGainTime, szLeaveWord);
			}
			else
			{
				// �ٱ�����ʷ��¼(������)
				GetItemMgr().ProcBaiBaoRecord(sPackSell.pItem[i]->dwTypeID, 
					dwTgtRoleID, GetNameID(), EBBRT_Friend, sPackSell.pItem[i]->dw1stGainTime);
			}

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sPackSell.pItem[i], ELCID_Mall_PresentPack, FALSE, GetID());
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sPackSell.pItem[i], dwTgtRoleID, ELCID_Mall_PresentPack);

				// ɾ����Ʒ
				::Destroy(sPackSell.pItem[i]);
			}
		}

		// �������Ʒ����ŵ����Ѱٱ�����
		if(P_VALID(sPackSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
							dwTgtRoleID, GetNameID(), EBBRT_Mall, sPackSell.pPresent->dw1stGainTime);

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCID_Mall_PresentPack_Add, FALSE, GetID());
			}
			else
			{
				// �洢��item_baibao����
				ItemMgr::InsertBaiBao2DB(sPackSell.pPresent, dwTgtRoleID, ELCID_Mall_PresentPack_Add);

				// ɾ����Ʒ
				::Destroy(sPackSell.pPresent);
			}
		}

		// ����һ�������
		if (sPackSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sPackSell.nVipPoint, ELCID_Mall_PresentPack);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ�̳������Ʒ
//-----------------------------------------------------------------------------
DWORD Role::GetMallFreeItem(DWORD dwID)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.GrantFreeItem(this, dwID, sItemSell);

	// ������

	if(E_Success == dwErrorCode && P_VALID(sItemSell.pItem))
	{
		// ����Ʒ�ŵ�������
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCID_Mall_FreeItem, TRUE);
	}
	
	return dwErrorCode;
}

DWORD Role::BuyMallCart(DWORD* dwIDArray, INT* nIndexArray, INT16* n16BuyNumArray)
{
	if (!P_VALID(dwIDArray) || !P_VALID(nIndexArray) || !P_VALID(n16BuyNumArray))
	{
		return GT_INVALID;
	}

	DWORD dwPrice = 0;
	BYTE byCartNum = 0;	// ���ﳵ��Ʒ����

	for (INT i = 0; i < MAX_MALL_CART_NUM; i++)
	{
		if (!P_VALID(dwIDArray[i]))
		{
			break;
		}

		byCartNum++;
	}
	DWORD dwRet = g_mall.CanSellCart(this, byCartNum, dwIDArray, nIndexArray, n16BuyNumArray, dwPrice);

	if (E_Success != dwRet)
	{
		return dwRet;
	}

	for (INT i = 0; i < byCartNum; i++)
	{
		if (i >= MAX_MALL_CART_NUM)
		{
			return GT_INVALID;
		}

		dwRet = BuyMallItem(dwIDArray[i], 0, n16BuyNumArray[i], nIndexArray[i], Shopping_Item, GT_INVALID, ELCID_Mall_CartBuy);
		if (E_Success != dwRet)
		{
			return dwRet;
		}
	}

	if (TRUE == g_mall.IsInActivity() && E_Success == dwRet)
	{
		// �̳ǻ���� [6/14/2012 zhangzhihua]
		INT nTimes = dwPrice / g_attRes.GetMallCfg(EMCA_BuyActivityRewardLine);

		for (INT i = 0; i <= (EMCA_BuyActivityRewardItem3 - EMCA_BuyActivityRewardItem0); i++)
		{
			DWORD dwItemTypeID = g_attRes.GetMallCfg((EMallCfgAttr)(i + EMCA_BuyActivityRewardItem0));
			INT nItemNum = g_attRes.GetMallCfg((EMallCfgAttr)(i + EMCA_BuyActivityRewardItemNum0));
			if (P_VALID(dwItemTypeID) && nItemNum > 0)
			{
				// ������Ʒ
				INT nTotalNum = nTimes * nItemNum;
				while (nTotalNum > 0)
				{
					tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, dwItemTypeID, nTotalNum, EIQ_White);
					if(!P_VALID(pItemNew))
					{
						ASSERT(P_VALID(pItemNew));
						return E_Mall_CreateItem_Failed;
					}

					nTotalNum -= pItemNew->pProtoType->GetMaxLapNum();

					if (P_VALID(pItemNew->pProtoType) && pItemNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
						pItemNew->dw1stGainTime	 = g_world.GetWorldTime();
					// �ٱ�����ʷ��¼
					GetItemMgr().ProcBaiBaoRecord(pItemNew->dwTypeID, 
						GetNameID(), GT_INVALID, EBBRT_Mall, pItemNew->dw1stGainTime);

					GetItemMgr().Add2BaiBao(pItemNew, ELCID_MALL_Activity_Add);
				}
			}
		}
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// �һ��̳���Ʒ
//-----------------------------------------------------------------------------
DWORD Role::MallItemExchange(DWORD dwMallID, INT nPrice, INT16 n16BuyNum, INT nIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.ExchangeItem(this, ELCLD_Mall_ExchangeItem, dwMallID, nIndex, nPrice, n16BuyNum, sItemSell);

	// ������
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// ����Ʒ�ŵ���ұ�����
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCLD_Mall_ExchangeItem, TRUE);

		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sItemSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, 
				GetNameID(), GT_INVALID, EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCLD_Mall_ExchangeItem_Add);
		}

		// �����������
		if (sItemSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sItemSell.nExVolumeAssign, ELCLD_Mall_ExchangeItem);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwMallID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;

}

//-----------------------------------------------------------------------------
// �һ��̳Ǵ����Ʒ
//-----------------------------------------------------------------------------
DWORD Role::MallPackExchange(DWORD dwMallID, INT nPrice, INT nIndex)
{
	// �ж��̳��Ƿ񿪷�
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallPackSell sPackSell;

	// �̳�����Ʒ��ؼ��
	DWORD dwErrorCode = g_mall.ExchangePack(this, ELCLD_Mall_ExchangePack, dwMallID, nIndex, nPrice, sPackSell);

	// ������
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{
		INT64 n64Serial = sPackSell.pItem[0]->n64Serial;
		INT16 n16BuyNum = sPackSell.pItem[0]->GetNum();
		DWORD dwFstGainTime = g_world.GetWorldTime();

		// ����Ʒ�ŵ���ұ�����
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			GetItemMgr().Add2Bag(sPackSell.pItem[i], ELCLD_Mall_ExchangePack, TRUE);
		}

		// �������Ʒ����ŵ��ٱ�����
		if(P_VALID(sPackSell.pPresent))
		{
			// �ٱ�����ʷ��¼
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
				GetNameID(), GT_INVALID, EBBRT_Mall, dwFstGainTime);

			GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCLD_Mall_ExchangePack_Add);
		}

		// �����������
		if (sPackSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sPackSell.nExVolumeAssign, ELCLD_Mall_ExchangePack);
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwMallID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻���Ԫ��
//-----------------------------------------------------------------------------
DWORD Role::SaveYB2Account(DWORD dwID, INT nNum)
{
	if( g_world.IsUseIPGSystem() && (!GetGetCash() ))
		return GT_INVALID;

	if( g_world.IsUseIPGSystem() )
	{
		int nCash = GetIPGCash();
		if( nCash < nNum )
			return E_Trade_BagYB_NotEnough;
	}
	else
	{
		// �����ұ���Ԫ������
		if(GetCurMgr().GetBagYuanBao() < nNum)
			return E_Trade_BagYB_NotEnough;
	}

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
	{
		pYBAccount = g_tradeYB.CreateTradeAccount(dwID);
		if(!P_VALID(pYBAccount))
			return GT_INVALID;
	}

	// Jason 2010-3-25 ��ֹ�������
	if( pYBAccount->YuanBao.GetCur() + nNum > pYBAccount->YuanBao.GetMax() )
		return E_Trade_AccountYB_Full;

	pYBAccount->IncAccountYB(nNum, (DWORD)ELCID_Trade_SaveYuanBao, TRUE);
	

	if( g_world.IsUseIPGSystem())
	{
		if(GetGetCash())
		{
			TCHAR tszTemp[64] = {0};
			wsprintf(tszTemp,_T("DecIPGCash6. Dec cash:%d. \r\n"),nNum);
			g_world.WriteKroeaLog(tszTemp);

			if( !DecIPGCash(nNum))
				return GT_INVALID;
		}
	}
	else
	{
		GetCurMgr().DecBagYuanBao(nNum, (DWORD)ELCID_Trade_SaveYuanBao);
	}
		
	return E_Success;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻����Ǯ
//-----------------------------------------------------------------------------
DWORD Role::SaveSilver2Account(DWORD dwID, INT64 nNum)
{
	// �����ұ�����Ǯ����
	if(GetCurMgr().GetBagSilver() < nNum)
		return E_Trade_BagSilver_NotEnough;

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
	{
		pYBAccount = g_tradeYB.CreateTradeAccount(dwID);
		if(!P_VALID(pYBAccount))
			return GT_INVALID;
	}

	// Jason 2010-3-25 ��ֹ�������
	if( pYBAccount->Silver.GetCur() + nNum > pYBAccount->Silver.GetMax() )
		return E_Trade_AccountSilver_Full;

	pYBAccount->IncAccountSilver(nNum, (DWORD)ELCID_Trade_SaveSilver, TRUE);
	GetCurMgr().DecBagSilver(nNum, (DWORD)ELCID_Trade_SaveSilver);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻�ȡԪ��
//-----------------------------------------------------------------------------
DWORD Role::DepositYBAccout(DWORD dwID, INT nNum)
{
	if( g_world.IsUseIPGSystem() && (!GetGetCash() ))
		return GT_INVALID;

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	// ����˻�Ԫ������
	if(pYBAccount->GetAccountYB() < nNum)
		return E_Trade_AccountYB_NotEnough;

	// �������Ƿ��ύ�����۶���
	tagYuanBaoOrder *pYBOrder = g_tradeYB.GetYBSellOrder(dwID);
	if(P_VALID(pYBOrder))
		return GT_INVALID;

	// Jason 2010-2-2 ��ֹ�������
	if( GetCurMgr  ().GetBagYuanBao  () + nNum > GetCurMgr  ().GetMaxBagYuanBao  () )
		return E_Trade_BagYB_WillBeFull;

	pYBAccount->DecAccountYuanBao(nNum, (DWORD)ELCID_Trade_DepositYuanBao, TRUE);
	GetCurMgr().IncBagYuanBao(nNum, (DWORD)ELCID_Trade_DepositYuanBao);
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// �����Ԫ�������˻�ȡ��Ǯ
//-----------------------------------------------------------------------------
DWORD Role::DepositSilverAccount(DWORD dwID, INT64 nNum)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	// ����˻���Ǯ����
	if(pYBAccount->GetAccountSilver() < nNum)
		return E_Trade_AccountSilver_NotEnough;

	// �������Ƿ��ύ���չ�����
	tagYuanBaoOrder *pYBOrder = g_tradeYB.GetYBBuyOrder(dwID);
	if(P_VALID(pYBOrder))
		return GT_INVALID;

	// Jason 2010-2-2 ��ֹ�������
	if( GetCurMgr  ().GetBagSilver  () + nNum > GetCurMgr  ().GetMaxBagSilver  () )
		return E_Trade_BagSilver_WillBeFull;


	pYBAccount->DecAccountSilver(nNum, (DWORD)ELCID_Trade_DepositSilver, TRUE);
	GetCurMgr().IncBagSilver(nNum, (DWORD)ELCID_Trade_DepositSilver);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ͬ��Ԫ�����׳�ʼ����Ϣ
//-----------------------------------------------------------------------------
DWORD Role::GetYBTradeInfo()
{
	g_tradeYB.SynBuyPriceList(this);
	g_tradeYB.SynSellPriceList(this);
	g_tradeYB.SynYBAccount(this);
	return E_Success;
}

//-----------------------------------------------------------------------------
// ����ύԪ�����۶���
//-----------------------------------------------------------------------------
DWORD Role::SubmitSellOrder(DWORD dwRoleID, INT nNum, INT nPrice)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	if(nNum <= 0 || nPrice <= 0)
		return GT_INVALID;

	INT64 n64Total = (INT64)nNum * nPrice;
	INT64 n64Tem = ~0x7FFFFFFF;

	if( n64Total & n64Tem )
		return E_Trade_AmountOfTheTradeTooBig;
		
	// �Ƿ��Ѿ��ύ�����۶���
	tagYuanBaoOrder* pSellOrder = (tagYuanBaoOrder*)GT_INVALID;
	pSellOrder = g_tradeYB.GetYBSellOrder(dwRoleID);
	if(P_VALID(pSellOrder))
		return E_Trade_SellOrder_Exit;

	// �����˻�Ԫ���Ƿ��㹻
	if(pYBAccount->GetAccountYB() < nNum)
		return E_Trade_AccountYB_NotEnough;

	// Jason 2010-3-16 v1.5.0 ���佻�׽�Ǯ�����ж�
	//CurrencyMgr & refCurrency = GetCurMgr();
	//if( refCurrency.GetBagSilver() + nPrice * nNum > refCurrency.GetMaxBagSilver() )
	//	return E_Trade_AccountSilver_WillBeFull;

	// Jason 2010-3-16 v1.5.0�ύ����ʱ�����۳������ѣ������ڽ��׺�۳���
	// ����������Ϊ�ܼ۵�2%
	//INT nTax = nPrice * nNum * 2 / 100;
	//if(nTax < 1)    nTax = 1;

	// ����������Ƿ��㹻
	//if (GetCurMgr().GetBagSilver() < nTax)
	//	return E_Trade_Tax_NotEnough;
	if( pYBAccount->GetCanIncAccountSilver() < /*nNum*nPrice*/0 )
		return E_Trade_AccountSilver_Full;


	tagYuanBaoOrder * pYBOrder = g_tradeYB.CreateYBOrder(dwRoleID, EYBOT_SELL, nPrice, nNum);
	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	// �����˻��ж������ύ״̬
	pYBAccount->SetSellOrder(TRUE);

	// �۳������� 
	//GetCurMgr().DecBagSilver(nTax, ELCID_Trade_Tax);

	// ����Ԫ��
	g_tradeYB.DealYBSell(pYBOrder);
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// ����ύԪ���չ�����
//-----------------------------------------------------------------------------
DWORD Role::SubmitBuyOrder(DWORD dwRoleID, INT nNum, INT nPrice)
{
	tagYuanBaoAccount *pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	if(nNum <= 0 || nPrice <= 0)
		return GT_INVALID;

	INT64 n64Total = (INT64)nNum * nPrice;
	INT64 n64Tem = ~0x7FFFFFFF;

	if( n64Total & n64Tem )
		return E_Trade_AmountOfTheTradeTooBig;

	// �Ƿ��ѽ��ύ������
	tagYuanBaoOrder* pBuyOrder = (tagYuanBaoOrder*)GT_INVALID;
	pBuyOrder = g_tradeYB.GetYBBuyOrder(dwRoleID);
	if(P_VALID(pBuyOrder))
		return E_Trade_BuyOrder_Exit;

	// �����˻���Ǯ�Ƿ��㹻
	if(pYBAccount->GetAccountSilver() < n64Total)
		return E_Trade_AccountSilver_NotEnough;

	// Jason 2010-3-16 v1.5.0 �չ�Ԫ��ʱ�����۳������ѡ�
	// ����������Ϊ�ܼ۵�2%
	//INT nTax = (FLOAT)(nPrice * nNum) * 0.02f;
	//if(nTax < 1)    nTax = 1;

	// ����������Ƿ��㹻
	//if (GetCurMgr().GetBagSilver() < nTax)
	//	return E_Trade_Tax_NotEnough;
	if( pYBAccount->GetCanIncAccountYB() < 0 )
		return E_Trade_AccountYB_Full;

	tagYuanBaoOrder *pYBOrder = g_tradeYB.CreateYBOrder(dwRoleID, EYBOT_BUY, nPrice, nNum);
	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	// �����˻��ж������ύ״̬
	pYBAccount->SetBuyOrder(TRUE);

	// �۳������� ��
	//GetCurMgr().DecBagSilver(nTax, ELCID_Trade_Tax);

	// ����Ԫ��
	g_tradeYB.DealYBBuy(pYBOrder);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ɾ������
//-----------------------------------------------------------------------------
DWORD Role::DeleteOrder(DWORD dwRoleID, DWORD dwOrderID, EYBOTYPE eYBOType)
{
	if(eYBOType != EYBOT_BUY && eYBOType != EYBOT_SELL)
		return GT_INVALID;

	tagYuanBaoOrder *pYBOrder = (tagYuanBaoOrder*)GT_INVALID;
	if(eYBOType == EYBOT_BUY)
		pYBOrder = g_tradeYB.GetYBBuyOrder(dwRoleID);
	else
		pYBOrder = g_tradeYB.GetYBSellOrder(dwRoleID);

	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	if(pYBOrder->dwID != dwOrderID)
		return GT_INVALID;

	g_tradeYB.DeleteYBOrder(pYBOrder, EYBOM_Cancel);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ѯһ���ڸ���ҵ�Ԫ�����׶���
//-----------------------------------------------------------------------------
DWORD Role::GetYBOrder(DWORD dwRoleID)
{
	tagYuanBaoAccount *pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return  GT_INVALID;

	DWORD dwCurTick = g_world.GetWorldTick();
	if(dwCurTick - pYBAccount->GetQuestTick() > 25)
		pYBAccount->SetQuestTick(dwCurTick);
	else
		return GT_INVALID;

	// �����ݿⷢ�Ͳ�ѯ��Ϣ
	tagNDBC_GetRoleYBOrder	sendDB;
	sendDB.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return E_Success;
}
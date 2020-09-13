//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: ItemMgr.cpp
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ/װ��������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_func_npc.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_log.h"
#include "../ServerDefine/role_data.h"
#include "../WorldDefine/msg_spec_item.h"

// ����ͷ�ļ�
#include "../WorldDefine/msg_AutoFight.h"

#include "container_restrict.h"
#include "item_creator.h"
#include "title_mgr.h"
#include "role.h"
#include "role_mgr.h"
#include "db_session.h"
#include "att_res.h"
#include "map.h"
#include "item_mgr.h"
#include "pet_soul.h"
#include "pet_pocket.h"
#include "fairy_birth.h"
#include "global_limited_item.h"
#include "../WorldDefine/clergy_define.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "HolySoul_Container.h"


struct tagUpdateOMGZhanDouFuFunc
{
	DWORD operator()(tagItem* pItem, ItemContainer& con, ItemMgr& mgr, DWORD dwCmdID)
	{
		DWORD dwErrorCode = GT_INVALID;
		if(!P_VALID(pItem) || !P_VALID(pItem->pProtoType))
			return dwErrorCode;
		if (pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu &&
			pItem->pProtoType->nSpecFuncVal1 == EAFSFV2_OMG)
		{
			if (pItem->dwSpecVal1 > 1)
			{
				pItem->dwSpecVal1--;
				pItem->SetUpdate(EUDBS_Update);
				dwErrorCode = E_Success;
			}
			else if (pItem->dwSpecVal1 == 1)
			{
				mgr.OnZhanDouFuRemoved(pItem->pProtoType->eSpecFunc, pItem->n64Serial, TRUE);
				dwErrorCode = mgr.DelFromContainer(con, pItem->n64Serial, dwCmdID);
			}
			else if (pItem->dwSpecVal1 == 0)
			{
				dwErrorCode = E_Success;
			}
		}
		return dwErrorCode;
	}
};

template<typename Mgr,typename F>
VOID UpdateItems(Mgr & mgr,ItemContainer & con, F f, DWORD dwCmdID)
{
	INT size = con.GetCurSpaceSize();
	for( INT16 i = 0; i < size ; ++i )
	{
		tagItem * p = con.GetItem((INT16)i);
		if( P_VALID(p))
			f(p, con, mgr, dwCmdID);
	}
}

//-------------------------------------------------------------------------------------------------------
// ����/��������
//-------------------------------------------------------------------------------------------------------
ItemMgr::ItemMgr(Role* pRole, DWORD dwAcctID, DWORD dwRoleID, INT16 n16BagSize, INT16 n16WareSize)
: m_Bag(EICT_Bag, n16BagSize, SPACE_ALL_BAG, dwRoleID, dwAcctID), 
m_QuestItemBag(EICT_Quest, SPACE_QUEST_BAG, SPACE_QUEST_BAG, dwRoleID, dwAcctID, new QusetItemBarRestrict),
m_BaiBaoBag(EICT_Baibao, SPACE_BAIBAO_BAG, SPACE_BAIBAO_BAG, dwRoleID, dwAcctID, new BaiBaoBagRestrict),
m_RoleWare(EICT_RoleWare, n16WareSize, SPACE_ALL_WARE, dwRoleID, dwAcctID, new WareRestrict),
m_TempBag(EICT_TempBag, 1, 1, dwRoleID, dwAcctID),
m_EquipBar(EICT_Equip, X_EQUIP_BAR_SIZE, X_EQUIP_BAR_SIZE),m_dwLastFabaoMatingTick(0),
m_Wardrobe(EICT_Wardrobe,  g_attRes.GetWardrobeLayerCnt()*MAX_FASHION_PERLAYER, g_attRes.GetWardrobeLayerCnt()*MAX_FASHION_PERLAYER, dwRoleID, dwAcctID),
m_WarBag(EICT_WarBag, SPACE_WAR_BAG, SPACE_WAR_BAG, dwRoleID, dwAcctID),
m_HolySoulBar(EICT_HolyEquip,X_HolySoul_BAR_SIZE,X_HolySoul_BAR_SIZE),
m_HolyManBar(EICT_HolyManEquip, SHENGLING_EQUIP_BLANK, SHENGLING_EQUIP_BLANK, dwRoleID, dwAcctID)
{
	m_pRole = pRole;
	m_mapCDTime.Clear();
	m_mapMaxHold.Clear();
}

ItemMgr::~ItemMgr()
{
	tagCDInfo* pCDInfo = NULL;
	MapCDTime::TMapIterator iter = m_mapCDTime.Begin();
	while (m_mapCDTime.PeekNext(iter, pCDInfo))
	{
		if (P_VALID(pCDInfo))
		{
			g_MemPoolSafe.Free(pCDInfo);
			pCDInfo = NULL;
		}
			
	}
	m_mapCDTime.Clear();
}



//-------------------------------------------------------------------------------------------------------
// �������ָ�롣עֻ�ܵõ�ItemContainer����
//-------------------------------------------------------------------------------------------------------
ItemContainer* ItemMgr::GetContainer(EItemConType eConType)
{
	switch(eConType)
	{
	case EICT_Bag:
		return &m_Bag;
		break;
	case EICT_Quest:
		return &m_QuestItemBag;
		break;
	case EICT_Baibao:
		return &m_BaiBaoBag;
		break;
	case EICT_RoleWare:
		return &m_RoleWare;
		break;
	case EICT_TempBag:
		return &m_TempBag;
		break;
	case EICT_Wardrobe:
		return &m_Wardrobe;
		break;
	case EICT_WarBag:
		return &m_WarBag;
		break;
	case EICT_HolyManEquip:
		return &m_HolyManBar;
		break;
	case EICT_Equip:
	case EICT_Shop:
	case EICT_Ground:
	case EICT_HolyEquip:
		return NULL;
		break;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------------
// �����Ʒ�Ƿ���Ҫ��¼log������Ҫ������LoongDB������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::LogItem(const tagItem &item1, const tagItem *pItem2, 
					  INT16 n16OptNum, DWORD dwCmdID, DWORD dwRoleIDRel/* = GT_INVALID*/)
{
	if(!(IsItemNeedLog(item1) || (P_VALID(pItem2) && IsItemNeedLog(*pItem2))))
	{
		return;
	}

	tagNDBC_LogItem send;
	send.sLogItem.dwRoleID		= m_pRole->GetID();
	send.sLogItem.dwTypeID		= item1.pProtoType->dwTypeID;

	send.sLogItem.n64Serial1	= item1.n64Serial;
	send.sLogItem.n8ConType1	= item1.eConType;
	send.sLogItem.n16ResNum1	= item1.GetNum();

	send.sLogItem.n16OptNum		= n16OptNum;
	send.sLogItem.dwCmdID		= dwCmdID;

	send.sLogItem.dwRoleIDRel	= dwRoleIDRel;

	if(P_VALID(pItem2))
	{
		send.sLogItem.n64Serial2	= pItem2->n64Serial;
		send.sLogItem.n8ConType2	= pItem2->eConType;
		send.sLogItem.n16ResNum2	= pItem2->GetNum();
	}
	else
	{
		send.sLogItem.n64Serial2	= 0;
		send.sLogItem.n8ConType2	= EICT_Null;
		send.sLogItem.n16ResNum2	= 0;
	}

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// �����Ʒ�Ƿ���Ҫ��¼log������Ҫ������LoongDB������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::LogItemTimes(const tagItem &item, DWORD dwCmdID)
{
	if(!IsItemNeedLog(item))
	{
		return;
	}

	tagNDBC_LogItemTimes send;
	send.sLogItemTimes.dwRoleID		= m_pRole->GetID();
	send.sLogItemTimes.dwCmdID		= dwCmdID;
	send.sLogItemTimes.n64Serial	= item.n64Serial;
	send.sLogItemTimes.dwTypeID		= item.dwTypeID;
	send.sLogItemTimes.nUsedTimes	= item.nUseTimes;
	send.sLogItemTimes.nMaxUseTimes	= item.pProtoType->nMaxUseTimes;
	send.sLogItemTimes.n8ConType	= item.eConType;

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// װ������
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::IdentifyEquip(INT64 n64SerialReel, INT64 n64SerialEquip, DWORD dwCmdID)
{
	ItemContainer &Bag = GetBag();

	// ��������
	tagItem *pReel = Bag.GetItem(n64SerialReel);
	if(!P_VALID(pReel))
	{
		return E_Item_NotFound;
	}

	if(pReel->pProtoType->eSpecFunc != EISF_IdetifyEquip)
	{
		return GT_INVALID;
	}

	// ���װ��
	tagEquip *pEquip = (tagEquip*)Bag.GetItem(n64SerialEquip);
	if(!P_VALID(pEquip))
	{
		return E_Item_NotFound;
	}

	if(!MIsEquipment(pEquip->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	if(MIsIdentified(pEquip))
	{
		return E_Equip_Idendtifed;
	}

	// ��������ȼ��Ƿ�����Ҫ��(������ȼ���װ���ȼ�/10)
	if(pReel->pProtoType->byLevel < (pEquip->pProtoType->byLevel / 10))
	{
		return E_Item_LevelLimit;
	}

	// ���ɼ���������
	ItemCreator::IdentifyEquip(pEquip);

	if(pEquip->equipSpec.byQuality >= EIQ_Blue && pEquip->equipSpec.byQuality < EIQ_End)
	{
		// ��������ɫƷ�����ϵ�װ�� �ƺ��¼�����  zxzhang �޸�
		m_pRole->GetTitleMgr()->SigEvent(ETE_IDENTIFY_BLUE_EQUIP, GT_INVALID, GT_INVALID);
	}

	// ��DBServer�Ϳͻ��˷���Ϣ
	UpdateEquipSpec(*pEquip);
	// Jason 2010-4-20 v2.0.0
	if( MIsFaBao(pEquip) && g_world.IsFabaoOpen() )
		UpdateFabaoSpec(*((tagFabao*)pEquip));
	if (MIsSoulCrystal(pEquip))
		UpdateSoulCrystalSpec(*(tagSoulCrystal*)pEquip);
	if (MIsHoly(pEquip))
		UpdateHolySpec(*(tagHolyMan*)pEquip);
	if (MIsHolyEquip(pEquip))
		UpdateHolyEquipSpec(*(tagHolyEquip*)pEquip);

	// ʹ��һ��
	ItemUsedFromBag(n64SerialReel, 1, dwCmdID);

	return E_Success;
}

//----------------------------------------------------------------------------------------
// ��ʼ����Ʒװ�� -- �����ͻ���
//----------------------------------------------------------------------------------------
VOID ItemMgr::SendInitStateItem()
{
	ItemContainer	&Bag		= GetBag();
	ItemContainer	&QuestBag	= GetQuestItemBag();
	ItemContainer	&BaiBao		= GetBaiBaoBag();
	ItemContainer	&RoleWare	= GetRoleWare();
	EquipContainer	&EquipBar	= GetEquipBar();
	WardrobeContainer& Wardrobe = GetWardrobeContainer();
	ItemContainer	&WarBag		= GetWarBag();
	HolySoulContainer   &HolySoulBar = GetHolySoulBar();
	ItemContainer	&HolyManbar		= GetHolyManBar();

	INT16 n16NumBag			= Bag.GetCurSpaceSize() - Bag.GetFreeSpaceSize();
	INT16 n16NumQuestBag	= QuestBag.GetCurSpaceSize() - QuestBag.GetFreeSpaceSize();
	INT16 n16NumBaiBao		= BaiBao.GetCurSpaceSize() - BaiBao.GetFreeSpaceSize();
	INT16 n16NumRoleWare	= RoleWare.GetCurSpaceSize() - RoleWare.GetFreeSpaceSize();
	INT16 n16NumEquipBar	= EquipBar.GetCurSpaceSize() - EquipBar.GetFreeSpaceSize();
	INT16 n16NumWardrobe	= Wardrobe.GetCurSpaceSize() - Wardrobe.GetFreeSpaceSize();
	INT16 n16NumWarBag		= WarBag.GetCurSpaceSize()	  - WarBag.GetFreeSpaceSize();
	INT16 n16NumHolySoul    = HolySoulBar.GetCurSpaceSize() - HolySoulBar.GetFreeSpaceSize();
	INT16 n16NumHolyEquip    = HolyManbar.GetCurSpaceSize() - HolyManbar.GetFreeSpaceSize();

	INT32 nItemNum = n16NumBag + n16NumQuestBag + n16NumBaiBao + n16NumRoleWare + n16NumEquipBar + n16NumWardrobe + n16NumHolySoul + n16NumHolyEquip + n16NumWarBag;

	// ������ܵ����ռ�
	INT32 nMaxMsgSize = SIZE_FABAO * nItemNum + sizeof(tagNS_GetRoleInitState_Item);

	// ������Ϣ
	MCREATE_MSG(pSend, nMaxMsgSize, NS_GetRoleInitState_Item);

	pSend->n16SzBag			= Bag.GetCurSpaceSize();
	pSend->n16SzRoleWare	= RoleWare.GetCurSpaceSize();
	pSend->nNum				= nItemNum;

	INT32 nSizeTotal	= 0;
	INT32 nSizeOne		= 0;

	GetAllItem(Bag, n16NumBag, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(QuestBag, n16NumQuestBag, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(BaiBao, n16NumBaiBao, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(RoleWare, n16NumRoleWare, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(EquipBar, n16NumEquipBar, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(Wardrobe, n16NumWardrobe, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(WarBag, n16NumWarBag, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(HolySoulBar, n16NumHolySoul, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	GetAllItem(HolyManbar, n16NumHolyEquip, pSend->byData + nSizeTotal, nSizeOne);
	nSizeTotal += nSizeOne;

	pSend->dwSize = sizeof(tagNS_GetRoleInitState_Item) - 1 + nSizeTotal;

	SendMessage(pSend, pSend->dwSize);

	// �ͷ���Ϣ�ڴ�
	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------------------------------
// ��ɫ��ʼ��ʱ������Ʒ&װ������ָ������λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::Put2Container(tagItem *pItem)
{	
	if(!P_VALID(m_pRole))
	{
		ASSERT(P_VALID(m_pRole));
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;

	//������Ʒ���޹���
	if(!CanAddMaxHoldItem(*pItem)) return E_Item_MaxHold;

	switch(pItem->eConType)
	{
	case EICT_Bag:
		dwErrorCode = GetBag().Add(pItem, pItem->n16Index, FALSE, FALSE);
		break;
	case EICT_Quest:
		dwErrorCode = GetQuestItemBag().Add(pItem, pItem->n16Index, FALSE, FALSE);
		break;
	case EICT_Baibao:
		dwErrorCode = GetBaiBaoBag().Add(pItem, pItem->n16Index, FALSE, FALSE);
		break;
	case EICT_RoleWare:
		dwErrorCode = GetRoleWare().Add(pItem, pItem->n16Index, FALSE, FALSE);
		break;
	case EICT_Equip:
		dwErrorCode = GetEquipBar().Add((tagEquip*)pItem, (EEquipPos)pItem->n16Index);
		break;
	case EICT_Wardrobe:
		if (GetWardrobeContainer().CanAdd2Wardrobe(pItem, pItem->n16Index) == E_Success)
			dwErrorCode = GetWardrobeContainer().Add(pItem, pItem->n16Index, FALSE, FALSE);
		if (dwErrorCode == E_Success)
		{
			DWORD dwLayerID = GT_INVALID;
			EFashionState eFashionState = m_pRole->OnFashionChange(pItem->n16Index, dwLayerID);
			if (eFashionState != EFS_NULL)
				m_pRole->GetWardrobe().ChangeLayerState(dwLayerID, eFashionState);
		}
		break;
	case EICT_Family:
		{
			if( GT_INVALID != m_pRole->GetFamilyID() )
			{
				m_pRole->SetFamilyFabao(pItem);
				dwErrorCode = E_Success;
				return dwErrorCode;
			}
			else
			{//û�м��壬������������
				return E_Offline_Role_Sprite;
			}
		}
		break;
	case EICT_WarBag:
		dwErrorCode = GetWarBag().Add(pItem, pItem->n16Index, FALSE, FALSE);
		break;
	case EICT_HolyEquip:
		dwErrorCode = GetHolySoulBar().Add((tagEquip*)pItem,(EEquipPos)pItem->n16Index);
		break;
	case EICT_HolyManEquip:	//ʥ��װ������������
		dwErrorCode = GetHolyManBar().Add(pItem, pItem->n16Index, FALSE, FALSE);
		break;
		
	default:
		ASSERT(0);
		break;
	}

	// ��Ʒ���޹���
	if(dwErrorCode == E_Success)
		AddMaxHoldItem(*pItem);

	// �޸ı����־λ
	pItem->SetUpdate(EUDBS_Null);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �Ƿ���Խ���
//-----------------------------------------------------------------------------
BOOL ItemMgr::CanExchange(const tagItem& item) const
{
	const tagItem * pItem = &item;
	BOOL bOk = TRUE;
	if(MIsFaBao(pItem))
	{
		tagFabao * pFabao = (tagFabao *)pItem;
		bOk = pFabao->CanExchange();
	}
	return bOk && (!(item.bLock || item.IsBind() || item.IsAccountBind()) && m_pRole->GetRoleStateEx().IsInState(ERSE_BagPsdPass) && (item.pProtoType->eSpecFunc != EISF_TreasureSeizer));
}

//-----------------------------------------------------------------------------
// ��һ����Ʒ&װ�� -- ��ͨ��Ʒ���뱳����������Ʒ����������
//-----------------------------------------------------------------------------
BOOL ItemMgr::Add2Role(EItemCreateMode eCreateMode, DWORD dwCreateID, 
					   DWORD dwTypeID, INT32 nNum, EItemQuality eQlty, DWORD dwCmdID)
{
	INT16 n16BagSpace = 0;
	INT16 n16QuestSpace = 0;
	INT16 n16WarBagSpace = 0;
	INT16 n16MaxLap;

	if(!CalSpaceUsed(dwTypeID, nNum, n16BagSpace, n16QuestSpace, n16WarBagSpace, n16MaxLap))
	{
		return FALSE;
	}

	if(n16BagSpace > 0 && GetBagFreeSize() < n16BagSpace
		|| n16QuestSpace > 0 && GetQuestItemBagFreeSize() < n16QuestSpace
		|| n16WarBagSpace > 0 && GetWarBagFreeSize() < n16WarBagSpace)
	{
		return FALSE;
	}

	tagItem *pNew;
	ItemContainer &con = n16WarBagSpace > 0 ? GetWarBag() : (n16BagSpace > 0 ? GetBag() : GetQuestItemBag());
	INT16 n16Space = max(n16BagSpace, max(n16QuestSpace, n16WarBagSpace));

	INT16 n16NewLeft = nNum;
	for(INT16 i=0; i<n16Space-1; ++i)
	{
		pNew = ItemCreator::CreateEx(eCreateMode, dwCreateID, dwTypeID, n16MaxLap, eQlty);
		if(!P_VALID(pNew))
		{
			return FALSE;
		}

		if(AddItem(con, pNew, dwCmdID, TRUE, TRUE) != E_Success)
		{
			Destroy(pNew);
			return FALSE;
		}

		n16NewLeft -= n16MaxLap;
	}

	pNew = ItemCreator::CreateEx(eCreateMode, dwCreateID, dwTypeID, n16NewLeft, eQlty);
	if(!P_VALID(pNew))
	{
		return FALSE;
	}

	if(AddItem(con, pNew, dwCmdID, TRUE, TRUE) != E_Success)
	{
		Destroy(pNew);
		return FALSE;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// �ӱ������������������Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::RemoveFromRole(DWORD dwTypeID, INT32 nNum, DWORD dwCmdID)
{
	if(IsQuestItem(dwTypeID))
	{
		if(GT_INVALID == nNum)
		{
			return RemoveItems(GetQuestItemBag(), dwTypeID, dwCmdID);
		}
		else
		{
			return RemoveItems(GetQuestItemBag(), dwTypeID, nNum, dwCmdID);
		}
	}
	else
	{
		if(GT_INVALID == nNum)
		{
			return RemoveItems(GetBag(), dwTypeID, dwCmdID);
		}
		else
		{
			return RemoveItems(GetBag(), dwTypeID, nNum, dwCmdID);
		}
	}
}

// �Ӳֿ���ɾ����Ʒ
DWORD ItemMgr::RemoveFromWare(DWORD dwTypeID, INT32 nNum, DWORD dwCmdID) // nNum == -1 ʱ��ʾȫ��ɾ��
{
	if(GT_INVALID == nNum)
	{
		return RemoveItems(GetRoleWare(), dwTypeID, dwCmdID);
	}
	else
	{
		return RemoveItems(GetRoleWare(), dwTypeID, nNum, dwCmdID);
	}
}

//-------------------------------------------------------------------------------------------------------
// �������������Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::AddItem(ItemContainer& container, tagItem *&pItem, DWORD dwCmdID, 
					   BOOL bInsert2DB/* = FALSE*/, BOOL bCheckAdd/* = TRUE*/, DWORD dwRoleIDRel/* = GT_INVALID*/, BOOL bChangeOwner/* = TRUE*/, BOOL bFromFamily/*=FALSE*/)
{
	ASSERT(P_VALID(pItem));
	ASSERT(P_VALID(pItem->n64Serial));

	INT16		n16Index;
	DWORD		dwErrorCode;
	tagItemMove	itemMove;
	INT16		n16AddNum = pItem->GetNum();

	// ����ϡ����Ʒ�ĸ���
	if (EISF_GlobalLimitedItem == pItem->pProtoType->eSpecFunc)
	{
		if (FALSE == g_GlobalLimitedItemMgr.IsLimitedItemFull(pItem->pProtoType->dwTypeID))
		{
			tagGlobalLimitedItem* pTmp = new tagGlobalLimitedItem;
			if(P_VALID(pTmp))
			{
				pTmp->dwID = pItem->dwTypeID;
				pTmp->eType = EGLIET_Item;
				pTmp->n64ID = pItem->n64Serial;
				pTmp->dwAreaID = pItem->pProtoType->nSpecFuncVal2;
				pTmp->nTime = pItem->pProtoType->dwTimeLimit;
				g_GlobalLimitedItemMgr.AddNew2LimitedItemMap(pTmp);
			}
		}
		else
		{
			return E_Item_CountLimit;
		}
	}

	// �����������ʥ��������Ѿ��ﵽ�����������������ʥ��
	if (MIsHoly(pItem) && IsHolyExceed() && container.GetConType()!= EICT_Baibao)
	{
		return E_Holy_CountLimit;
	}

	// ����Ƿ��л�ȡʱ��
	if(0 == pItem->dw1stGainTime && P_VALID(pItem->pProtoType) && pItem->pProtoType->eSpecFunc != EISF_ZhanDouFu)
	{
		pItem->dw1stGainTime = g_world.GetWorldTime();
	}

	// ��Ʒ����
	if ( CanAddMaxHoldItem(*pItem) )
	{
		// ��������
		dwErrorCode = container.Add(pItem, n16Index, itemMove, bCheckAdd);
	}
	else
	{
		dwErrorCode = E_Item_MaxHold;
	}

	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
		IMSG(_T("\nAdd item to container failed! \nroleid: %u, item serial: %lld, container type: %d!!!!\n\n"),
			container.GetOwnerID(), pItem->n64Serial, container.GetConType());

		//?? ���ǽ�����Ʒ���浽log��

		return dwErrorCode;
	}

	// װ���ɳ���������
	if (MIsEquipment(pItem->dwTypeID))
	{
		tagEquip* pEquip = (tagEquip*)pItem;
		if (pEquip->pEquipProto->bCanGrow && !P_VALID(pEquip->equipSpec.dwGrowTypeID) && dwCmdID != ELCID_Exchange_Verify)	//ͨ�����׵Ĳ�������������������~
		{
			pEquip->equipSpec.dwGrowTypeID = m_pRole->OnRoleEquipGetGrowID(pEquip->pProtoType->dwTypeID, pEquip->equipSpec.byQuality);
			pEquip->equipSpec.dwGrowValue = 0;
			pEquip->equipSpec.byGrowLevel = 0;
			pEquip->equipSpec.nTotalGrowTimes = 0;
			pEquip->equipSpec.nTotalIMUseTimes = 0;
			ZeroMemory(&(pEquip->equipSpec.eGrowStar), sizeof(EEquipGrowStar)*(EQUIP_GROW_MAX_LEVEL-1)); 
			ZeroMemory(&(pEquip->equipSpec.nGrowAttValue), sizeof(INT)*EQUIP_GROW_ADD_ATT_MAX_NUM);
		}

		// ս����Ʒ������װ����ֱ�Ӽ�����
		if (MIsWarItem(pItem->pProtoType) && m_pRole->IsInWar())
		{
			m_pRole->ProcEquipEffectAtt((tagEquip*)pItem, true, n16Index, false, true);
		}
	}

	// ������Ʒ�����б�
	AddMaxHoldItem(*pItem);

	INT16 n16Num = itemMove.n16NumRes1 != 0 ? itemMove.n16NumRes1: itemMove.n16NumRes2;

	// ���������Ʒ,��û�к�������Ʒ�ѵ�,����LoongDB����Ϣ
	if(bInsert2DB && !itemMove.bOverlap)
	{
		// �󶨴���
		if((EBS_Unknown == pItem->byBind) && bChangeOwner)
		{
			pItem->Bind();
		}

		InsertItem2DB(*pItem);
		pItem->SetUpdate(EUDBS_Null);

		SendAddNew2Client(pItem);
	}
	else
	{
		if(!bFromFamily)
			SendAddItem2Client(container.GetConType(), n16Index, pItem->n64Serial, n16Num, itemMove.bOverlap);
		else
			SendAddNew2Client(pItem);
	}

	pItem->SetNum(itemMove.n16NumRes1);

	// ����Ƿ���Ҫ��¼log
	LogItem(*pItem, itemMove.pItem2, n16AddNum, dwCmdID, dwRoleIDRel);

	// ���񴥷�
	if(container.GetConType() == EICT_Bag || container.GetConType() == EICT_Quest)
	{
		m_pRole->OnQuestEvent(EQE_Item, pItem->dwTypeID, n16AddNum, TRUE);
	}

	// ��鹫����ȴʱ��
	CheckCDTimeMap(pItem->dwTypeID);

	// �����ƺ��¼�
	if (container.GetConType() == EICT_Bag)
	{
		m_pRole->GetTitleMgr()->SigEvent(ETE_BAG_ITEM, pItem->dwTypeID, GT_INVALID);
		if (P_VALID(g_ScriptMgr.GetRoleScript()))
			g_ScriptMgr.GetRoleScript()->OnAddItemToBag(m_pRole, pItem->dwTypeID);
	}

	// ����Ƿ����ͷŸ���Ʒ�ڴ�
	if(itemMove.bOverlap || pItem->eConType == EICT_Ground)
	{
		DeleteItemFromDB(pItem->n64Serial, pItem->dwTypeID);

		Destroy(pItem);
		pItem = itemMove.pItem2;
	}

	//ս����Ʒ
	if(MIsWarItem(pItem->pProtoType)&&MIsEquipment(pItem->pProtoType->dwTypeID))
	{
		m_pRole->GetWarItemRecalEffect(n16Index, TRUE);
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��������ָ��λ�������Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::Add2BagByIndex(tagItem *&pItem, DWORD dwCmdID, INT16 n16Index)
{
	ASSERT(P_VALID(pItem));
	ASSERT(P_VALID(pItem->n64Serial));

	DWORD		dwErrorCode;

	// ��������
	dwErrorCode =  GetBag().Add(pItem, n16Index, FALSE, FALSE);
	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
		IMSG(_T("\nAdd item to container failed! \nroleid: %u, item serial: %lld, container type: %d!!!!\n\n"),
			GetBag().GetOwnerID(), pItem->n64Serial, GetBag().GetConType());

		//?? ���ǽ�����Ʒ���浽log��

		return dwErrorCode;
	}

	// ����Ƿ���Ҫ��¼log
	LogItem(*pItem, NULL, pItem->GetNum(), dwCmdID);

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��������ָ��λ�������Ʒ���������ݿ�
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::Add2BagByIndexAndInsertDB( tagItem *&pItem, DWORD dwCmdID, INT16 n16Index )
{
	ASSERT(P_VALID(pItem));
	ASSERT(P_VALID(pItem->n64Serial));

	DWORD		dwErrorCode;

	// ��������
	dwErrorCode =  GetBag().Add(pItem, n16Index, TRUE, FALSE);
	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
		IMSG(_T("\nAdd item to container failed! \nroleid: %u, item serial: %lld, container type: %d!!!!\n\n"),
			GetBag().GetOwnerID(), pItem->n64Serial, GetBag().GetConType());

		//?? ���ǽ�����Ʒ���浽log��

		return dwErrorCode;
	}

	//��̬������Ʒ��ItemContainer::Add�������ò��������ߣ����Ե���������Ūһ��
	pItem->SetOwner(m_pRole->GetID(), m_pRole->GetSession()->GetSessionID());

	// ��¼���ݿ�
	InsertItem2DB(*pItem);
	pItem->SetUpdate(EUDBS_Null);

	// ֪ͨ�ͻ���
	SendAddNew2Client(pItem);
	//	SendAddItem2Client(EICT_Bag, pItem->n16Index, pItem->GetKey(), pItem->n16Num, pItem->pProtoType->n16MaxLapNum > 1);

	// ����Ƿ���Ҫ��¼log
	LogItem(*pItem, NULL, pItem->GetNum(), dwCmdID);

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::RemoveItem(ItemContainer& container, INT64 n64Serial,  
						  DWORD dwCmdID, BOOL bDelFromDB, BOOL bDelMem, BOOL bCheckRemove, DWORD dwRoleIDRel/* = GT_INVALID*/, BOOL bTimeOut/* = FALSE*/,BOOL bRemoveLimited/*=TRUE*/)
{
	tagItem *pItem = container.GetItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(P_VALID(pItem));
		IMSG(_T("Can not find item<serial: %lld> in container<Type: %d, RoleID: %u>!!!\r\n"),
			n64Serial, container.GetConType(), container.GetOwnerID());
		return GT_INVALID;
	}

	INT16 n16OldIndex	= pItem->n16Index;
	INT16 n16Num		= pItem->GetNum();


	//ս����Ʒ
	if(MIsWarItem(pItem->pProtoType)&&MIsEquipment(pItem->pProtoType->dwTypeID))
	{
		m_pRole->GetWarItemRecalEffect(n16OldIndex, FALSE);
	}

	DWORD dwErrorCode = container.Remove(n64Serial, TRUE, bCheckRemove);
	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
		IMSG(_T("\nRemove item<roleid:%u, container type: %d, serial:%lld, error:%u> from container failed!\n\n"),
			container.GetOwnerID(), container.GetConType(), n64Serial, dwErrorCode);

		return dwErrorCode;
	}

	// ϡ����Ʒ��������
	if (EISF_GlobalLimitedItem == pItem->pProtoType->eSpecFunc)
	{
		if(bRemoveLimited)
		{
			g_GlobalLimitedItemMgr.DelFromLimitedItemMap(pItem->dwTypeID, pItem->n64Serial);
		}
	}

	if (MIsWarItem(pItem->pProtoType) && m_pRole->IsInWar())
	{
		m_pRole->ProcEquipEffectAtt((tagEquip*)pItem, false, n16OldIndex, false, true);
	}

	// ��Ʒ���޹���
	RemoveMaxHoldItem(pItem->dwTypeID, pItem->GetNum());

	// ��ͻ��˷���Ϣ
	SendDelItem2Client(container.GetConType(), n16OldIndex, n64Serial, 0);

	// ����Ƿ���Ҫ��¼log -- ��ʹ��ʱ��ƷҪ����Ϸ������ɾ��,��ʱ�ڴ�Ҳû���ͷ�
	LogItem(*pItem, NULL, -n16Num, dwCmdID, dwRoleIDRel);

	// ��Ҷ�����ɾ����ս���������߻��ߡ�ս����������ʱ�޵���
	if (pItem->pProtoType)
	{
		OnZhanDouFuRemoved(pItem->pProtoType->eSpecFunc, n64Serial, bTimeOut);
	}

	// ���񴥷�
	if(container.GetConType() == EICT_Bag || container.GetConType() == EICT_Quest)
	{
		m_pRole->OnQuestEvent(EQE_Item, pItem->dwTypeID, n16Num, FALSE);
	}

	bool deldb = false;
	if(bDelFromDB)
	{
		deldb = true;
		DeleteItemFromDB(n64Serial, pItem->dwTypeID);
	}
	// �ͷ��ڴ�
	if(bDelMem)
	{
		if( !deldb )
			DeleteItemFromDB(n64Serial, pItem->dwTypeID);
		Destroy(pItem);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ָ����Ʒ�ĸ���(�����)
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::RemoveItem(ItemContainer& container, INT64 n64Serial, INT16 n16Num, DWORD dwCmdID,
						  BOOL bCheckRemove/* = TRUE*/, BOOL bDelete/* = FALSE*/,BOOL bRemoveLimited/*=TRUE*/)
{
	if(n16Num <= 0)
	{
		ASSERT(0);
		return GT_INVALID;
	}

	tagItem *pItem = container.GetItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(P_VALID(pItem));
		IMSG(_T("Can not find item<serial: %lld> in container<Type: %d, RoleID: %u>!!!\r\n"),
			n64Serial, container.GetConType(), container.GetOwnerID());
		return GT_INVALID;
	}

	/******************** ����ʹ�ô��� *******************/
	if(!bDelete && !pItem->CanOverlap() && pItem->pProtoType->nMaxUseTimes != 1)
	{
		ASSERT(!pItem->CanOverlap());

		// ���ɶѵ���Ʒ���ٵ���ʹ�ô���
		pItem->IncUseTimes(n16Num);
		ASSERT( pItem->pProtoType->nMaxUseTimes == GT_INVALID || pItem->nUseTimes <= pItem->pProtoType->nMaxUseTimes );

		// ����Ƿ���Ҫ��¼log
		LogItemTimes(*pItem, dwCmdID);

		//ASSERT(FALSE == bCheckRemove);
		if(pItem->pProtoType->nMaxUseTimes != GT_INVALID)
		{
			if(pItem->nUseTimes >= pItem->pProtoType->nMaxUseTimes)
			{
				return RemoveItem(container, n64Serial, dwCmdID, TRUE, TRUE, FALSE, -1, FALSE, bRemoveLimited);
			}
			else
			{
				SendDelItem2Client(container.GetConType(), pItem->n16Index, n64Serial, pItem->nUseTimes);
			}
		}

		return E_Success;
	}

	/********************* ������Ʒ ******************/
	ASSERT(n16Num <= pItem->GetNum());

	// ����Ƿ���ȫɾ��
	if(n16Num == pItem->GetNum())
	{
		return RemoveItem(container, n64Serial, dwCmdID, TRUE, TRUE, bCheckRemove, -1, FALSE, bRemoveLimited);
	}

	INT16 n16OldIndex = pItem->n16Index;

	DWORD dwErrorCode = container.Remove(n64Serial, n16Num, bCheckRemove);
	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
		IMSG(_T("\nRemove item<roleid:%u, container type: %d, serial:%lld, error:%u> from container failed!\n\n"),
			container.GetOwnerID(), container.GetConType(), n64Serial, dwErrorCode);

		return dwErrorCode;
	}

	// ��Ʒ���޹���
	RemoveMaxHoldItem(pItem->dwTypeID, n16Num);

	// ��ͻ��˷���Ϣ
	SendDelItem2Client(container.GetConType(), n16OldIndex, n64Serial, pItem->GetNum());

	// ����Ƿ���Ҫ��¼log
	LogItem(*pItem, NULL, -n16Num, dwCmdID);

	// ��Ҷ�����ɾ����ս���������߻��ߡ�ս����������ʱ�޵���
	if (pItem->pProtoType)
	{
		OnZhanDouFuRemoved(pItem->pProtoType->eSpecFunc, n64Serial);
	}

	// ���񴥷�
	if(container.GetConType() == EICT_Bag || container.GetConType() == EICT_Quest)
	{
		m_pRole->OnQuestEvent(EQE_Item, pItem->dwTypeID, n16Num, FALSE);
	}

	return E_Success;
}

// ��Ҷ�����ɾ����ս���������߻��ߡ�ս����������ʱ�޵���
void ItemMgr::OnZhanDouFuRemoved(EItemSpecFunc eItemSpecFunc, INT64 n64ItemID, BOOL bTimeOut/*=FALSE*/)
{
	// �ǡ�ս������������,���ҽ�ɫ�����Ѿ�û��ս��������
	if (eItemSpecFunc == EISF_ZhanDouFu)
	{
		if (P_VALID(m_pRole)&&m_pRole->GetCurZhanDouFu64ID() == n64ItemID)
		{
			// ���ͻ��˷���Ϣ��֪ͨ�ͻ��ˡ�ս���������ڣ����ص��һ�ϵͳ
			tagNS_SyncAutoFightState send;
			send.n64ItemID = n64ItemID;
			send.eAutoFightState = EAFS_Close;
			m_pRole->SendMessage(&send, send.dwSize);
			m_pRole->SetCurZhanDouFu64ID(0);
			m_pRole->SetAutoFightState(EAFS_Close);
		}

		if (bTimeOut)
		{
			tagNS_ZhanDouFuTimeOut zhanDouFuTimeOut;
			zhanDouFuTimeOut.n64ItemID = n64ItemID;
			m_pRole->SendMessage(&zhanDouFuTimeOut, zhanDouFuTimeOut.dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ͬһ�������ƶ���Ʒ(n16Num = 0 ��ʾȫ���ƶ�)
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::Move(EItemConType eConType, INT64 n64Serial,	INT16 n16Num, INT16 n16PosDst, DWORD dwCmdID)
{
	ASSERT(n16Num >= 0);

	ItemContainer *pItemCon = GetContainer(eConType);
	if(!P_VALID(pItemCon))
	{
		return GT_INVALID;
	}

	tagItem *pItem1 = pItemCon->GetItem(n64Serial);
	if(!P_VALID(pItem1))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode	= GT_INVALID;
	INT16 n16SrcPos		= pItem1->n16Index;	// n64Serialԭλ��

	tagItemMove	itemMove;

	if(0 == n16Num)
	{
		n16Num = pItem1->GetNum();
		dwErrorCode = pItemCon->MoveTo(n64Serial, n16PosDst, itemMove);
	}
	else if(n16Num > 0)
	{
		dwErrorCode = pItemCon->MoveTo(n64Serial, n16Num, n16PosDst, itemMove);
	}

	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
#ifdef _DEBUG
		IMSG(_T("\nMove item<roleid:%u, container type: %d, serial:%lld, error:%u> in container failed!\n\n"),
			pItemCon->GetOwnerID(), pItemCon->GetConType(), n64Serial, dwErrorCode);
#endif // _DEBUG

		return dwErrorCode;
	}

	// ��ͻ��˷���Ϣ
	tagNS_ItemPosChange send;
	send.eConType	= eConType;
	send.n64Serial1	= n64Serial;
	send.n64Serial2	= itemMove.pItem2 != NULL ? itemMove.pItem2->n64Serial : GT_INVALID;

	send.n16Num1	= itemMove.n16NumRes1;
	send.n16Num2	= itemMove.n16NumRes2;
	send.bCreateItem = itemMove.bCreateItem;

	// ����漰�³�
	if (P_VALID(m_pRole) && eConType == EICT_Wardrobe)
	{
		EFashionState eFashionState1, eFashionState2 = EFS_NULL;
		DWORD dwLayerID1, dwLayerID2 = GT_INVALID;
		eFashionState1 = m_pRole->OnFashionChange(n16SrcPos, dwLayerID1);
		eFashionState2 = m_pRole->OnFashionChange(n16PosDst, dwLayerID2);
		if (eFashionState1 != EFS_NULL)
			m_pRole->GetWardrobe().ChangeLayerState(dwLayerID1, eFashionState1);
		if (eFashionState2 != EFS_NULL)
			m_pRole->GetWardrobe().ChangeLayerState(dwLayerID2, eFashionState2);
	}

	if(itemMove.bChangePos)
	{
		send.n16PosDst1	= n16PosDst;
		send.n16PosDst2	= n16SrcPos;
	}
	else
	{
		send.n16PosDst1	= n16SrcPos;
		send.n16PosDst2	= n16PosDst;
	}

	if(itemMove.bOverlap)
	{
		send.n16PosDst1	= n16SrcPos;
		send.n16PosDst2	= n16PosDst;
	}

	SendMessage(&send, send.dwSize);

	// ����Ƿ���Ҫ��¼log
	if(itemMove.bCreateItem || itemMove.bOverlap)
	{
		LogItem(*pItem1, itemMove.pItem2, -n16Num, dwCmdID);
	}

	// ����ƶ�����Ʒ���Ƿ���ڣ��粻����,����LoongDBͬ���ͻ����ڴ�
	if(0 == itemMove.n16NumRes1 || pItem1->eConType == EICT_Ground)
	{
		DeleteItemFromDB(n64Serial, pItem1->dwTypeID);

		// ����Ƿ����ͷŸ���Ʒ�ڴ�
		Destroy(pItem1);
	}

	// ����Ƿ񴴽����µ���Ʒ��
	if(itemMove.bCreateItem)
	{
		tagNDBC_NewItem send1;
		IFASTCODE->MemCpy(&send1.item, itemMove.pItem2, SIZE_ITEM);

		g_dbSession.Send(&send1, send1.dwSize);

	}
	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��������Ʒ�ƶ�
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::Move2Other(EItemConType eConTypeSrc, INT64 n64Serial1, 
						  EItemConType eConTypeDst, INT16 n16PosDst, DWORD dwCmdID)
{
	ItemContainer *pConSrc = GetContainer(eConTypeSrc);
	if(!P_VALID(pConSrc))
	{
		return GT_INVALID;
	}

	ItemContainer *pConDst = GetContainer(eConTypeDst);
	if(!P_VALID(pConDst))
	{
		return GT_INVALID;
	}

	tagItem *pItem1 = pConSrc->GetItem(n64Serial1);

	if(!P_VALID(pItem1) || pItem1->eConType!= eConTypeSrc)
	{
		return GT_INVALID;
	}

	if (EICT_RoleWare == eConTypeDst)
	{
		if (pItem1->pProtoType->bCanMove2Ware || MIsHoly(pItem1))
		{
			return E_Ware_MoveLimit;
		}
	}

	if (EICT_GuildWare == eConTypeDst)
	{
		if (MIsHoly(pItem1))
		{
			return E_GuildWare_MoveLimit;
		}
	}

	// ���Ҫ�������з�ʥ�飬���ж��Ƿ�ﵽ�������
	if (EICT_Bag == eConTypeDst && MIsHoly(pItem1))
	{
		if (IsHolyExceed())
		{
			return E_Bag_NotEnoughSpace;
		}
	}


	tagItem  item1Old; //used for log
	memcpy(&item1Old, pItem1, sizeof(tagItem));

	if ( (eConTypeDst == EICT_RoleWare ) && ( eConTypeSrc == EICT_Bag ) && (pItem1->pProtoType->eSpecFunc == EISF_PortableWare))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode	= GT_INVALID;
	INT16 n16PosSrc1	= pItem1->n16Index;	// n64Serialԭλ��
	INT16 n16Num		= pItem1->GetNum();
	DWORD dwTypeID		= pItem1->dwTypeID;

	tagItemMove	itemMove;

	// ����Ƿ��ǴӰٱ�����ȡ��ʱ������Ʒ
	if (eConTypeSrc == EICT_Baibao)
	{
		if( pItem1->IsTimeLimit() && P_VALID(pItem1->pProtoType) && pItem1->pProtoType->eSpecFunc != EISF_ZhanDouFu)
		{
			pItem1->dw1stGainTime = g_world.GetWorldTime();
			//pConDst->Add2TimeLeftList( n64Serial1, pItem1->pProtoType->dwTimeLimit, pItem1->dw1stGainTime );
		}
	}
	
	if(GT_INVALID == n16PosDst)	// �һ���û��ָ��Ŀ��λ��
	{
		dwErrorCode = pConSrc->MoveTo(n64Serial1, *pConDst, itemMove, n16PosDst);
	}
	else
	{
		dwErrorCode = pConSrc->MoveTo(n64Serial1, *pConDst, n16PosDst, itemMove);
	}

	if(dwErrorCode != E_Success)
	{
		ASSERT(E_Success == dwErrorCode);
#ifdef _DEBUG
		IMSG(_T("\nMove item<roleid:%u, container type: %d, serial:%lld, error:%u> in container failed!\n\n"),
			pConSrc->GetOwnerID(), pConSrc->GetConType(), n64Serial1, dwErrorCode);
#endif // _DEBUG

		return dwErrorCode;
	}

	// ��ͻ��˷���Ϣ
	tagNS_ItemPosChangeEx send;

	send.eConTypeSrc1	= eConTypeSrc;
	send.eConTypeSrc2	= eConTypeDst;
	send.n64Serial1		= n64Serial1;
	send.n64Serial2		= itemMove.pItem2 != NULL ? itemMove.pItem2->n64Serial : GT_INVALID;
	send.n16Num1		= itemMove.n16NumRes1;
	send.n16Num2		= itemMove.n16NumRes2;

	if(itemMove.bOverlap)
	{
		send.eConTypeDst1	= eConTypeSrc;
		send.eConTypeDst2	= eConTypeDst;
		send.n16PosDst1		= n16PosSrc1;
		send.n16PosDst2		= n16PosDst;
	}
	else
	{
		send.eConTypeDst1	= eConTypeDst;
		send.eConTypeDst2	= eConTypeSrc;
		send.n16PosDst1		= n16PosDst;
		send.n16PosDst2		= n16PosSrc1;
	}

	SendMessage(&send, send.dwSize);

	// ����漰�³�
	if (P_VALID(m_pRole))
	{
		EFashionState eFashionState = EFS_NULL;
		DWORD dwLayerID = GT_INVALID;
		if (eConTypeSrc == EICT_Wardrobe)
			eFashionState = m_pRole->OnFashionChange(n16PosSrc1, dwLayerID);
		else if (eConTypeDst == EICT_Wardrobe)
			eFashionState = m_pRole->OnFashionChange(n16PosDst, dwLayerID);
		if (eFashionState != EFS_NULL)
			m_pRole->GetWardrobe().ChangeLayerState(dwLayerID, eFashionState);
	}

	// ��¼�ٱ�����ȡ��ʷ��¼
	if (eConTypeSrc == EICT_Baibao && eConTypeDst == EICT_Bag)
	{
		if ((itemMove.n16NumRes1 != 0) && (EBS_Unknown == pItem1->byBind))
		{
			pItem1->Bind();
			pItem1->SetUpdate(EUDBS_Update);
			// Jason 2010-3-26 fix(6603) 
			if( pItem1->IsBind() )
				SendItemBind(pItem1->GetKey(), pItem1->eConType);
		}
		ProcBaiBaoRecord(dwTypeID, m_pRole->GetID(), m_pRole->GetID(), EBBRT_Myself);
	}

	// ���񴥷�
	if( (eConTypeSrc == EICT_RoleWare || eConTypeSrc == EICT_GuildWare || eConTypeSrc == EICT_Baibao ) && eConTypeDst == EICT_Bag )
	{
		m_pRole->OnQuestEvent(EQE_Item, dwTypeID, n16Num, TRUE);
	}

	if(eConTypeSrc == EICT_Bag && (eConTypeDst == EICT_RoleWare || eConTypeDst == EICT_GuildWare ))
	{
		m_pRole->OnQuestEvent(EQE_Item, dwTypeID, n16Num, FALSE);
	}

	if(P_VALID(itemMove.pItem2) && itemMove.pItem2->eConType == item1Old.eConType)
	{
		if( (eConTypeSrc == EICT_RoleWare || eConTypeSrc == EICT_GuildWare || eConTypeSrc == EICT_Baibao) && eConTypeDst == EICT_Bag )
		{
			m_pRole->OnQuestEvent(EQE_Item, itemMove.pItem2->dwTypeID, itemMove.pItem2->GetNum(), FALSE); // item2�Ǵӱ����滻������
		}

		if(eConTypeSrc == EICT_Bag && (eConTypeDst == EICT_RoleWare || eConTypeDst == EICT_GuildWare))
		{
			m_pRole->OnQuestEvent(EQE_Item, itemMove.pItem2->dwTypeID, itemMove.pItem2->GetNum(), TRUE); // item2���滻�������е�
		}
	}

	// ����Ƿ���Ҫ��¼log
	if(itemMove.pItem2)
	{
		if(itemMove.pItem2->eConType == item1Old.eConType) // swap
		{
			item1Old.SetNum(0);
			LogItem(item1Old, pItem1, -pItem1->GetNum(), dwCmdID);
			memcpy(&item1Old, itemMove.pItem2, sizeof(tagItem));
			item1Old.SetNum(0);
			item1Old.SetConType(eConTypeDst);
			LogItem(item1Old, itemMove.pItem2, -itemMove.pItem2->GetNum(), dwCmdID);
		}
		else
		{
			item1Old.SetNum(pItem1->eConType==EICT_Ground? 0 : pItem1->GetNum());
			LogItem(item1Old, itemMove.pItem2, item1Old.GetNum() - n16Num, dwCmdID);
		}
	}
	else
	{
		item1Old.SetNum(0);
		LogItem(item1Old, pItem1, -n16Num, dwCmdID);
	}	

	// ����ƶ�����Ʒ���Ƿ���ڣ��粻����,����LoongDBͬ���ͻ����ڴ�
	if(0 == itemMove.n16NumRes1 || pItem1->eConType == EICT_Ground)
	{
		DeleteItemFromDB(n64Serial1, pItem1->dwTypeID);

		// �ͷŸ���Ʒ�ڴ�
		Destroy(pItem1);
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��ӱ����ж����Ʒ(��Ҽ佻��,�ʼ�) -- private
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::AddItems(ItemContainer& container, tagItem* pItem[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel)
{
	DWORD dwErrorCode;
	for(INT32 i=0; i<nSize; ++i)
	{
		if(!P_VALID(pItem[i]))
		{
			return;
		}

		dwErrorCode = AddItem(container, pItem[i], dwCmdID, TRUE, TRUE, dwRoleIDRel);
		ASSERT(E_Success == dwErrorCode);
	}
}

//-------------------------------------------------------------------------------------------------------
// ɾ�������ж����Ʒ(��Ҽ佻��,�ʼ�)
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::RemoveItems(ItemContainer& container, INT64 n64Serial[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel)
{
	for(INT32 i=0,j=0; i<nSize; ++i)
	{
		if(n64Serial[i] != 0)
		{
			DWORD dwErrorCode = RemoveItem(container, n64Serial[i], dwCmdID, TRUE, FALSE, FALSE, dwRoleIDRel);
			ASSERT(E_Success == dwErrorCode);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ĳ����Ʒ(������Ʒ������)�����سɹ�����
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::RemoveItems(ItemContainer& container, DWORD dwTypeID, DWORD dwCmdID)
{
	DWORD dwErrorCode;

	TList<tagItem*> itemList;
	container.GetSameItemList(itemList, dwTypeID);

	tagItem *pItem;
	itemList.ResetIterator();
	while(itemList.PeekNext(pItem))
	{		
		dwErrorCode = RemoveItem(container, pItem->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
		ASSERT(E_Success == dwErrorCode);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ָ��������ĳ����Ʒ(����Ʒ������)�����سɹ�����
//-------------------------------------------------------------------------------------------------------
DWORD ItemMgr::RemoveItems(ItemContainer& container, DWORD dwTypeID, INT32 nNum, DWORD dwCmdID)
{
	if(nNum <= 0)
	{
		ASSERT(nNum > 0);
		return GT_INVALID;
	}

	TList<tagItem*> itemList;
	INT32 nNumTotal = container.GetSameItemList(itemList, dwTypeID);

	// ����
	if(nNumTotal < nNum)
	{
		// ɾ��ʧ��,���ͼ����Ϣ //??

		ASSERT(nNumTotal >= nNum);
		return GT_INVALID;
	}

	// �պ�, ����
	DWORD dwErrorCode;
	INT16 n16NumDel = 0;
	INT32 nNumNeedDel = nNum;
	tagItem *pItem = NULL;
	itemList.ResetIterator();
	while(itemList.PeekNext(pItem) && nNumNeedDel != 0)
	{
		if(pItem->GetNum() <= nNumNeedDel)
		{
			n16NumDel = pItem->GetNum();
			dwErrorCode = RemoveItem(container, pItem->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
			if(dwErrorCode != E_Success)
			{
				ASSERT(E_Success == dwErrorCode);
				n16NumDel = 0;
			}

			nNumNeedDel -= n16NumDel;
		}
		else
		{
			n16NumDel = nNumNeedDel;
			dwErrorCode = RemoveItem(container, pItem->n64Serial, nNumNeedDel, dwCmdID, FALSE);
			if(dwErrorCode != E_Success)
			{
				ASSERT(E_Success == dwErrorCode);
				n16NumDel = 0;
			}

			nNumNeedDel -= n16NumDel;
		}
	}

	ASSERT(0 == nNumNeedDel);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ����Ϸ��ɾ����ָ��������ص���Ʒ
//-----------------------------------------------------------------------------
VOID ItemMgr::RemoveItems(ItemContainer& container, UINT16 u16QuestID, DWORD dwCmdID)
{
	const tagItem *pItem;
	for(INT16 i=0; i<container.GetCurSpaceSize(); ++i)
	{
		pItem = container.GetItem(i);
		if(!P_VALID(pItem))
		{
			continue;
		}

		// �����������ɾ������Ʒ
		if(u16QuestID == pItem->pProtoType->dwQuestID)
		{
			RemoveItem(container, pItem->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
		}
	}
}

//-----------------------------------------------------------------------------
// ��װ������ɾ����ָ��������ص���Ʒ
//-----------------------------------------------------------------------------
VOID ItemMgr::RemoveItems(EquipContainer& container, UINT16 u16QuestID, DWORD dwCmdID)
{
	const tagItem *pItem;
	for(INT16 i=0; i<container.GetCurSpaceSize(); ++i)
	{
		pItem = container.GetItem(i);
		if(!P_VALID(pItem))
		{
			continue;
		}

		// �����������ɾ������Ʒ
		if(u16QuestID == pItem->pProtoType->dwQuestID)
		{
			RemoveFromEquipBar(pItem->n64Serial, dwCmdID, TRUE);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ��鱳���ж����Ʒ�Ƿ����(��Ҽ佻��,�ʼ�)
//-------------------------------------------------------------------------------------------------------
BOOL ItemMgr::CheckItemsExist(OUT tagItem* pItem[], ItemContainer& container, 
							  INT64 n64Serial[], INT16 n16Num[], INT32 nSize)
{
	ASSERT(pItem != NULL);
	ASSERT(n64Serial != NULL);
	ASSERT(n16Num != NULL);

	for(INT32 i=0,j=0; i<nSize; ++i)
	{
		if(n64Serial[i] != 0)
		{
			pItem[j] = container.GetItem(n64Serial[i]);
			if(!P_VALID(pItem[j]) || pItem[j]->GetNum() != n16Num[i])
			{
				return FALSE;
			}

			++j;
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��ͻ��˷�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::SendMessage(LPVOID pMsg, DWORD dwSize)
{
	ASSERT(P_VALID(m_pRole));

	PlayerSession *pSession = m_pRole->GetSession();
	if(P_VALID(pSession))
	{
		pSession->SendMessage(pMsg, dwSize);
	}

	ASSERT(P_VALID(pSession));
}

//-------------------------------------------------------------------------------------------------------
// �������������Ʒ�����͵��ͻ�����Ϣ��װ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::SendAddNew2Client(const tagItem *pItem)
{
	if(MIsEquipment(pItem->dwTypeID))
	{
		// Jason 2010-4-15 v2.0.0
		if( MIsFaBao(pItem) )
		{
			tagNS_NewFabaoAdd Send;
			IFASTCODE->MemCpy(&Send.fabao, pItem, SIZE_FABAO);
			SendMessage(&Send, Send.dwSize);
			return;
		}

		// wcy 2012-04-01		
		if( MIsHoly(pItem) )
		{
			tagNS_NewHolyAdd Send;
			IFASTCODE->MemCpy(&Send.holy, pItem, SIZE_HOLY);
			SendMessage(&Send, Send.dwSize);
			return;
		}

		// wcy 2012-04-18		
		if( MIsHolyEquip(pItem) )
		{
			tagNS_NewHolyEquipAdd Send;
			IFASTCODE->MemCpy(&Send.holyEquip, pItem, SIZE_HOLYEQUIP);
			SendMessage(&Send, Send.dwSize);
			return;
		}

		else if (MIsSoulCrystal(pItem))
		{
			tagNS_NewSoulCrystalAdd Send;
			IFASTCODE->MemCpy(&Send.hunjing, pItem, SIZE_HUNJING);
			SendMessage(&Send, Send.dwSize);
			return;
		}
		tagNS_NewEquipAdd Send;
		IFASTCODE->MemCpy(&Send.Equip, pItem, SIZE_EQUIP);
		//Send.Equip.equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������

		SendMessage(&Send, Send.dwSize);
	}
	else
	{
		tagNS_NewItemAdd Send;
		IFASTCODE->MemCpy(&Send.Item, pItem, SIZE_ITEM);

		SendMessage(&Send, Send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// �������������Ʒ�����͵��ͻ�����Ϣ��װ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::SendAddItem2Client(EItemConType eConType, INT16 n16Index, INT64 n64Serial, INT16 n16Num, BOOL bOverlap)
{
	tagNS_ItemAdd	send;
	send.n64Serial	= n64Serial;
	send.n16Index	= n16Index;
	send.n16Num		= n16Num;
	send.eConType	= eConType;
	send.bOverlap	= bOverlap ? true : false;

	SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ����Ʒ�����͵��ͻ�����Ϣ��װ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::SendDelItem2Client(EItemConType eConType, INT16 n16Index, INT64 n64Serial, INT16 n16Num)
{
	tagNS_ItemRemove send;
	//send.dwErrorCode= dwErrorCode;
	send.n64Serial	= n64Serial;
	send.n16Index	= n16Index;
	send.n16Num		= n16Num;
	send.eConType	= eConType;

	SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// ��LoongDB����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::InsertItem2DB(tagItem &item)
{
	if(MIsEquipment(item.dwTypeID))
	{
		if (MIsWarItem(item.pProtoType))
		{
			return;
		}
		// Jason 2010-4-15 v2.0.0
		if( MIsFaBao((&item)) )
		{
			tagNDBC_NewFabao send;
			//tagFabao & fabao = (tagFabao&)item;
			IFASTCODE->MemCpy(&send.fabao,&item,SIZE_FABAO);
			g_dbSession.Send(&send, send.dwSize);
			return;
		}
		if (MIsSoulCrystal((&item)) )
		{
			tagNDBC_NewSoulCrystal send;
			IFASTCODE->MemCpy(&send.obj,&item,sizeof(tagSoulCrystal));
			g_dbSession.Send(&send, send.dwSize);
			return;
		}

		// �����ʥ��װ������Ҫ�洢 ʥ���
		if( MIsHoly((&item)) )
		{
			tagNDBC_NewHoly send;
			//tagFabao & fabao = (tagFabao&)item;
			IFASTCODE->MemCpy(&send.holy,&item,SIZE_HOLY);
			g_dbSession.Send(&send, send.dwSize);
			return;
		}	

		// �����ʥ�ƣ���Ҫ�洢 ʥ�Ʊ�
		if( MIsHolyEquip((&item)) )
		{
			tagNDBC_NewHolyEquip send;
			//tagFabao & fabao = (tagFabao&)item;
			IFASTCODE->MemCpy(&send.holyEquip,&item,SIZE_HOLYEQUIP);
			g_dbSession.Send(&send, send.dwSize);
			return;
		}

		

		tagNDBC_NewEquip send;
		IFASTCODE->MemCpy(&send.equip, &item, SIZE_EQUIP);
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		tagNDBC_NewItem send;
		IFASTCODE->MemCpy(&send.item, &item, SIZE_ITEM);
		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ��LoongDB����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::DeleteItemFromDB(INT64 n64Serial, INT32 dwTypeID)
{
	if(!MIsEquipment(dwTypeID))
	{
		tagNDBC_DelItem send;
		send.n64Serial = n64Serial;

		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		tagNDBC_DelEquip send;
		send.n64Serial = n64Serial;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ��Ϣ -- �����ɫ����ʱ����
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::SaveItem2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum, tagFabao* pFabao )
{
	INT32 nTmpNum;
	nNum = 0;

	Save2DB(GetBag(), pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetQuestItemBag(), pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetBaiBaoBag(), pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetRoleWare(), pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetEquipBar(), pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetWardrobeContainer(), pData, pData, nTmpNum);
	nNum += nTmpNum;

	SaveFamilySpriteToDB(pFabao, pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetHolyManBar(),pData, pData, nTmpNum);
	nNum += nTmpNum;

	Save2DB(GetHolySoulBar(),pData, pData, nTmpNum);
	nNum += nTmpNum;

	pOutPointer = pData;
}


//-----------------------------------------------------------------------------
// װ�����Ըı�󣬼�ʱ�������ݿ�
//-----------------------------------------------------------------------------
VOID ItemMgr::SendEquipSpec2DB(const tagEquip &equip)
{
	tagNDBC_UpdateEquipAtt send;
	send.equipSpecUpdate.n64Serial = equip.n64Serial;
	IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &equip.equipSpec, sizeof(tagEquipSpec));

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// װ�����Ըı����ͻ��˷���Ϣ
//-----------------------------------------------------------------------------
VOID ItemMgr::SendEquipSpec2Client(const tagEquip &equip)
{
	tagNS_EquipChange send;
	send.n64Serial = equip.n64Serial;
	IFASTCODE->MemCpy(&send.equipSpec, &equip.equipSpec, sizeof(tagEquipSpec));
	//send.equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������
	SendMessage(&send, send.dwSize);
}

VOID ItemMgr::SendEquipSingleSpecUpdate(const tagEquip &equip,EEquipSignleSpecType eEquipSingleSpecType)
{
	if ( EESSType_Null == eEquipSingleSpecType )
	{
		return;
	}
	tagNDBC_UpdateEquipSingleAtt DBmsg;
	tagNS_EquipSingleChange		 ClientMsg;
	DBmsg.equipSingleSpecUpdate.n64Serial			 =  equip.n64Serial; 
	DBmsg.equipSingleSpecUpdate.eEquipSingleSpecType = eEquipSingleSpecType ;
	ClientMsg.equipSingleSpec.n64Serial				 = equip.n64Serial;
	ClientMsg.equipSingleSpec.eEquipSingleSpecType	 = eEquipSingleSpecType;

	switch( eEquipSingleSpecType ) 
	{
	case EESSType_StarEnhance:
		DBmsg.equipSingleSpecUpdate.dwVal		=	equip.equipSpec.byStarEnhanceLv;
		ClientMsg.equipSingleSpec.dwVal			=   equip.equipSpec.byStarEnhanceLv;
		break;
	case EESSType_HolyEquipHole:
		DBmsg.equipSingleSpecUpdate.dwVal		=	equip.equipSpec.byHolyHoleNum;
		ClientMsg.equipSingleSpec.dwVal			=   equip.equipSpec.byHolyHoleNum;
		break;

	default:
		return;
	}

	g_dbSession.Send(&DBmsg,DBmsg.dwSize);
	SendMessage(&ClientMsg,ClientMsg.dwSize);
}
//-------------------------------------------------------------------------------------------------------
// ÿ��tick����һ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::Update()
{
	UpdateCDTime();
	UpdateUnlockCDTime();

	// ����update
	UpdateContainer(m_Bag);
	UpdateContainer(m_QuestItemBag);
	UpdateContainer(m_BaiBaoBag);
	UpdateContainer(m_RoleWare);
	UpdateContainer(m_TempBag);
	UpdateContainer(m_EquipBar);
	UpdateContainer(m_WarBag);
	//UpdateContainer(m_HolySoulBar);
	UpdateContainer(m_HolyManBar);
}

//-------------------------------------------------------------------------------------------------------
// ÿ��tick����һ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::UpdateContainer(ItemContainer& sItemCon)
{
	if( sItemCon.GetConType() != EICT_Baibao ) // �ٱ����е�ʱ����Ʒ����Ҫ��ʱ
	{
		sItemCon.Update();
	}
	
	// ̨��OMG�汾�������󣬽�ɫ����ʱս�����ż�ʱ
	UpdateItems(m_pRole->GetItemMgr(), sItemCon, tagUpdateOMGZhanDouFuFunc(), ELCLD_Item_AtTerm);

	TList<INT64>& lstNeedDel = sItemCon.GetNeedDelList();
	if(lstNeedDel.Size() <= 0)
	{
		return;
	}

	INT64 n64Serial;
	TList<INT64>::TListIterator iter = lstNeedDel.Begin();
	while(lstNeedDel.PeekNext(iter, n64Serial))
	{
		tagItem *pItem = sItemCon.GetItem(n64Serial);
		if(P_VALID(pItem))
		{
			const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
			if(P_VALID(pScript) && P_VALID(pItem->pProtoType) )
			{
				pScript->DelItem( 3, pItem->dwTypeID, pItem->pProtoType->byQuality );
			}
		}

		if(this->RemoveItem(sItemCon, n64Serial, (DWORD)ELCLD_Item_AtTerm, TRUE, TRUE, FALSE,GT_INVALID, TRUE, FALSE) != E_Success)
		{
			ASSERT(0);
			IMSG(_T("Item<%ld> time at term, but delete failed! Please check!\n"), n64Serial);
		}
	}

	sItemCon.ClearNeedDelList();
}

//-------------------------------------------------------------------------------------------------------
// ÿ��tick����һ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::UpdateContainer(EquipContainer& sEquipCon)
{
	sEquipCon.Update();

	TList<INT64>& lstNeedDel = sEquipCon.GetNeedDelList();
	if(lstNeedDel.Size() <= 0)
	{
		return;
	}

	INT64 n64Serial;
	TList<INT64>::TListIterator iter = lstNeedDel.Begin();
	while(lstNeedDel.PeekNext(iter, n64Serial))
	{
		this->RemoveFromEquipBar(n64Serial, (DWORD)ELCLD_Item_AtTerm, TRUE);
	}

	sEquipCon.ClearNeedDelList();
}

//-------------------------------------------------------------------------------------------------------
// ÿ��tick����һ����ȴʱ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::UpdateCDTime()
{
	if(m_mapCDTime.Size() == 0)
	{
		return;
	}

	DWORD		dwTypeID;
	tagCDInfo*	pCDInfo = NULL;
	MapCDTime::TMapIterator iter = m_mapCDTime.Begin();
	while(m_mapCDTime.PeekNext(iter, dwTypeID, pCDInfo))
	{
		if (!P_VALID(pCDInfo))
		{
			m_mapCDTime.Erase(dwTypeID);
			continue;
		}
		if(pCDInfo->dwRemainTime <= TICK_TIME)
		{
			g_MemPoolSafe.Free(pCDInfo);
			m_mapCDTime.Erase(dwTypeID);
			continue;
		}

		pCDInfo->dwElapseTime += TICK_TIME;
		pCDInfo->dwRemainTime -= TICK_TIME;
	}
}

//-------------------------------------------------------------------------------------------------------
// ÿ��tick����һ����Ʒ���������ȴʱ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::UpdateUnlockCDTime()
{
	if(m_mapUnlockCD.Size() == 0)
	{
		return;
	}

	INT64 n64Serial = 0;
	DWORD dwTime = 0;
	MapUnlockCD::TMapIterator iter = m_mapUnlockCD.Begin();
	while(m_mapUnlockCD.PeekNext(iter, n64Serial, dwTime))
	{
		if( GetCurrentDWORDTime() > dwTime )
		{
			tagItem* pItem = GetBagItem(n64Serial);
			if(!P_VALID(pItem)) continue;

			pItem->bLock = 0;
			pItem->dwUnlockTime = 0;

			m_mapUnlockCD.Erase(n64Serial);

			tagNDBC_LockItem send;
			send.n64Item = pItem->n64Serial;
			send.byType = pItem->bLock;
			send.dwUnlockTime = pItem->dwUnlockTime;
			g_dbSession.Send(&send, send.dwSize);

			tagNS_UnlockItemNotice _send;
			_send.byContainerType	= EICT_Bag;
			_send.u64ItemID				= pItem->n64Serial;
			m_pRole->SendMessage(&_send, _send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// �����������Ʒ����ȴʱ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::Add2CDTimeMap(DWORD dwTypeID, DWORD dwCDTime/* = GT_INVALID*/)
{
	if ( m_pRole->ObjectCoolOff() == TRUE)
	{
		return;
	}

	if(GT_INVALID == dwCDTime)
	{
		MapCDTime mapCDTime;
		GetSameCDItemList(mapCDTime, dwTypeID);

		DWORD dwTempTypeID	= GT_INVALID;
		tagCDInfo* pCDInfo	= NULL;
		MapCDTime::TMapIterator iter = mapCDTime.Begin();
		while (mapCDTime.PeekNext(iter, dwTempTypeID, pCDInfo))
		{
			if (!GT_VALID(dwTempTypeID) || !P_VALID(pCDInfo))
			{
				continue;
			}

			// �Ѿ�����ԭ�����Ƿ����
			m_mapCDTime.Add(dwTempTypeID, pCDInfo);

			tagNS_ItemCDUpdate	send;
			send.dwTypeID = dwTempTypeID;
			send.dwCDTime = pCDInfo->dwRemainTime;
			m_pRole->SendMessage(&send, send.dwSize);
		}
	}
	else
	{
		tagCDInfo* pCDInfo = (tagCDInfo*)g_MemPoolSafe.Alloc(sizeof(tagCDInfo));
		if (!P_VALID(pCDInfo))	return;

		tagItemProto *pProto = g_attRes.GetItemProto(dwTypeID);
		if(!P_VALID(pProto))	return;

		pCDInfo->dwElapseTime = max(0, pProto->dwCooldownTime - dwCDTime);
		pCDInfo->dwRemainTime = dwCDTime;
		m_mapCDTime.Add(dwTypeID, pCDInfo);

		//tagNS_ItemCDUpdate	send;
		//send.dwTypeID = dwTypeID;
		//send.dwCDTime = dwCDTime;
		//m_pRole->SendMessage(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ����µĽ����������ʱ��Ʒ
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::Add2UnlockCDTimeMap(INT64 n64Serial, DWORD dwTime)
{
	m_mapUnlockCD.Add(n64Serial, dwTime);
}

//-------------------------------------------------------------------------------------------------------
// ȡ�ñ�����ͬ��CD��Ʒ�����б�
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::GetSameCDItemList(TMap<DWORD, tagCDInfo*> &mapSameCD, DWORD dwTypeID)
{
	mapSameCD.Clear();

	// �ϲ��߼�����
	if (IsItemCDTime(dwTypeID))
	{
		return;
	}

	tagItemProto *pProto = g_attRes.GetItemProto(dwTypeID);
	if(!P_VALID(pProto))
	{
		ASSERT(P_VALID(pProto));
		return;
	}

	// û��ͬ����Ʒ
	if (pProto->eTypeReserved == EITR_Null)
	{
		// ���������ͼ���

		tagCDInfo* pCDInfo = (tagCDInfo*)g_MemPoolSafe.Alloc(sizeof(tagCDInfo));
		if (!P_VALID(pCDInfo))
		{
			return;
		}
		pCDInfo->dwElapseTime = 0;
		pCDInfo->dwRemainTime = pProto->dwCooldownTime;

		mapSameCD.Add(dwTypeID, pCDInfo);
		return;
	}

	// ��������
	tagItem* pItem = NULL;
	TMap<INT64, INT16>::TMapIterator iter = GetBag().Begin();

	while(GetBag().GetNextItem(iter, pItem))
	{
		if (!P_VALID(pItem))
		{
			continue;
		}
		if (mapSameCD.IsExist(pItem->dwTypeID) || m_mapCDTime.IsExist(pItem->dwTypeID))
		{
			continue;
		}
		if (pItem->pProtoType->eTypeReserved == pProto->eTypeReserved)
		{
			tagCDInfo* pCDInfo = (tagCDInfo*)g_MemPoolSafe.Alloc(sizeof(tagCDInfo));
			if (!P_VALID(pCDInfo))
			{
				continue;
			}
			pCDInfo->dwElapseTime = 0;

			if (pItem->pProtoType->dwCooldownTime != 0)
			{
				pCDInfo->dwRemainTime = pItem->pProtoType->dwCooldownTime;
			}
			else
			{
				pCDInfo->dwRemainTime = 1000;
			}
			mapSameCD.Add(pItem->dwTypeID, pCDInfo);
		}
	}

	iter = GetWarBag().Begin();
	while(GetWarBag().GetNextItem(iter, pItem))
	{
		if (!P_VALID(pItem))
		{
			continue;
		}
		if (mapSameCD.IsExist(pItem->dwTypeID) || m_mapCDTime.IsExist(pItem->dwTypeID))
		{
			continue;
		}
		if (pItem->pProtoType->eTypeReserved == pProto->eTypeReserved)
		{
			tagCDInfo* pCDInfo = (tagCDInfo*)g_MemPoolSafe.Alloc(sizeof(tagCDInfo));
			if (!P_VALID(pCDInfo))
			{
				continue;
			}
			pCDInfo->dwElapseTime = 0;

			if (pItem->pProtoType->dwCooldownTime != 0)
			{
				pCDInfo->dwRemainTime = pItem->pProtoType->dwCooldownTime;
			}
			else
			{
				pCDInfo->dwRemainTime = 1000;
			}
			mapSameCD.Add(pItem->dwTypeID, pCDInfo);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ��Ʒ�Ƿ�����ȴʱ��
//-------------------------------------------------------------------------------------------------------
BOOL ItemMgr::IsItemCDTime(DWORD dwTypeID)
{
	if(m_mapCDTime.IsExist(dwTypeID))
		return TRUE;
	else
		return FALSE;
}

//-------------------------------------------------------------------------------------------------------
// ���ͬ����Ʒ�Ĺ�����ȴʱ��
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::CheckCDTimeMap(DWORD dwTypeID)
{
	// ����Ʒ�Ѿ�����CD
	if (IsItemCDTime(dwTypeID))
	{
		return;
	}

	// ȡ����Ʒ����
	tagItemProto *pProto = g_attRes.GetItemProto(dwTypeID);
	if (!P_VALID(pProto))
	{
		return;
	}

	// û�й���CD����Ʒ����
	if (pProto->eTypeReserved == EITR_Null)
	{
		return;
	}

	// �����Ѵ���CD����Ʒ
	DWORD		dwTempTypeID;
	tagCDInfo*	pTempCDInfo = NULL;

	MapCDTime::TMapIterator iter = m_mapCDTime.Begin();
	while (m_mapCDTime.PeekNext(iter, dwTempTypeID, pTempCDInfo))
	{
		if (!GT_VALID(dwTempTypeID) || !P_VALID(pTempCDInfo))
		{
			continue;
		}

		// ȡ����Ʒ����
		tagItemProto *pTempProto = g_attRes.GetItemProto(dwTempTypeID);
		if(!P_VALID(pTempProto))
		{
			continue;
		}

		// �ж��Ƿ���ͬ����Ʒ
		if ((pTempProto->eTypeReserved == pProto->eTypeReserved)
			&& (pProto->dwCooldownTime > pTempCDInfo->dwElapseTime))
		{
			tagCDInfo* pCDInfo = (tagCDInfo*)g_MemPoolSafe.Alloc(sizeof(tagCDInfo));
			if (!P_VALID(pCDInfo))
			{
				continue;
			}

			pCDInfo->dwElapseTime	= pTempCDInfo->dwElapseTime;
			pCDInfo->dwRemainTime	= pProto->dwCooldownTime - pTempCDInfo->dwElapseTime;

			m_mapCDTime.Add(dwTypeID, pCDInfo);

			tagNS_ItemCDUpdate	send;
			send.dwTypeID = dwTypeID;
			send.dwCDTime = pCDInfo->dwRemainTime;
			m_pRole->SendMessage(&send, send.dwSize);

			// ����ѭ��
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// ����ȴʱ�䱣�浽���ݿ���
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::FormatCDTime(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	if(!P_VALID(m_pRole))
	{
		ASSERT(P_VALID(m_pRole));
		return;
	}

	nNum = m_mapCDTime.Size();
	pOutPointer = pData;

	if(nNum != 0)
	{
		DWORD		dwTypeID;
		tagCDInfo*	pCDInfo = NULL;
		INT32		nCounter = 0;

		MTRANS_POINTER(pCDTime, pData, tagCDTime);
		MapCDTime::TMapIterator iter = m_mapCDTime.Begin();
		while(m_mapCDTime.PeekNext(iter, dwTypeID, pCDInfo))
		{
			pCDTime[nCounter].dwTypeID	= dwTypeID;
			pCDTime[nCounter].dwTime	= pCDInfo->dwRemainTime;

			++nCounter;
		}

		pOutPointer = &pCDTime[nCounter];
	}
}

//-------------------------------------------------------------------------------------------------------
// ����ȴʱ�䷢�͵��ͻ���
//-------------------------------------------------------------------------------------------------------
VOID ItemMgr::SendInitStateItemCDTime()
{
	INT32 nNum = m_mapCDTime.Size();
	if(0 == nNum)
	{
		return;
	}

	INT32 nSize = sizeof(tagNS_GetRoleInitState_ItemCDTime) - 1 + sizeof(tagCDTime) * nNum;

	MCREATE_MSG(pSend, nSize, NS_GetRoleInitState_ItemCDTime);
	pSend->nNum = nNum;

	LPVOID pDummy;
	FormatCDTime(pSend->byData, pDummy, nNum);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-----------------------------------------------------------------------------
// �������Ƿ����ظ�ID
//-----------------------------------------------------------------------------
BOOL ItemMgr::IsRepeatID(INT64 n64ItemID[], INT32 nArraySz)
{
	//����һ�����������н���Ҫ���Ե�����
	set<INT64> idSet;

	for(INT n = 0; n < nArraySz; ++n)
	{
		idSet.insert(n64ItemID[n]);
	}

	if(idSet.size() != nArraySz)
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
// ����ռ�ø�����
//-----------------------------------------------------------------------------
BOOL ItemMgr::CalSpaceUsed(DWORD dwTypeID, INT32 nNum, OUT INT16 &n16UseBagSpace, 
						   OUT INT16 &n16UseQuestSpace, OUT INT16 &n16UseWarBagSpace, OUT INT16 &n16MaxLap)
{
	n16UseBagSpace = n16UseQuestSpace = n16UseWarBagSpace = 0;

	const tagItemProto *pProto;
	if(!MIsEquipment(dwTypeID))
	{
		pProto = g_attRes.GetItemProto(dwTypeID);
	}
	else
	{
		pProto = g_attRes.GetEquipProto(dwTypeID);
	}

	if(!P_VALID(pProto))
	{
		ASSERT(0);
		IMSG(_T("Cannot find item/equip proto<id: %ld>!"), dwTypeID);
		return FALSE;
	}

	// ����ռ�õĸ���
	INT16 nUseSpace = 0;
	if(nNum <= pProto->n16MaxLapNum)
	{
		nUseSpace = 1;
	}
	else
	{
		nUseSpace = (1 == pProto->n16MaxLapNum) ? nNum : ((nNum - 1) / pProto->n16MaxLapNum + 1);
	}

	// ���Ǳ���������������Ʒ
	if (MIsWarItem(pProto))
	{
		n16UseWarBagSpace = nUseSpace;
	}
	else
	{
		if(MIsQuestItem(pProto))
		{
			n16UseQuestSpace = nUseSpace;
		}
		else
		{
			n16UseBagSpace = nUseSpace;
		}
	}

	n16MaxLap = pProto->n16MaxLapNum;

	return TRUE;
}

//-----------------------------------------------------------------------------
// �Ƿ���������Ʒ
//-----------------------------------------------------------------------------
BOOL ItemMgr::IsQuestItem(DWORD dwTypeID)
{
	const tagItemProto *pProto;
	if(!MIsEquipment(dwTypeID))
	{
		pProto = g_attRes.GetItemProto(dwTypeID);
	}
	else
	{
		pProto = g_attRes.GetEquipProto(dwTypeID);
	}

	if(!P_VALID(pProto))
	{
		ASSERT(0);
		IMSG(_T("Cannot find item/equip proto<id: %ld>!"), dwTypeID);
		return FALSE;
	}

	return MIsQuestItem(pProto);
}

// �ж���ҵ�ʥ����Ŀ�Ƿ��Ѵﵽ����
BOOL ItemMgr::IsHolyExceed()
{
	INT32 nHolyNum = GetBagSameItemCount2(EIT_Holy);

	tagEquip * pEquip1 =GetEquipBarEquip((INT16)EEP_Holy);
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		nHolyNum++;
	}

	if (nHolyNum >= MAX_HOLY_NUM )
	{
		return true;

	}
	return false;
}

// �����ҵ�ʥ����Ŀ
int ItemMgr::GetHolyNum()
{
	INT32 nHolyNum = GetBagSameItemCount2(EIT_Holy);

	tagEquip * pEquip1 =GetEquipBarEquip((INT16)EEP_Holy);
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		nHolyNum++;
	}
	return nHolyNum;

}

//-----------------------------------------------------------------------------
// ��Ԫ��װ������ֱ��ɾ��һ��װ��
//-----------------------------------------------------------------------------
tagItem* ItemMgr::RemoveFromHolySoulBar(INT64 n64Serial, DWORD dwCmdID, BOOL bDelMem)
{

	tagItem *pItem = GetHolySoulBar().GetItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(P_VALID(pItem));
		IMSG(_T("Can not find item<serial: %lld> in container<Type: %d, RoleID: %u>!!!\r\n"),
			n64Serial, GetHolySoulBar().GetConType(), m_pRole->GetID());
		return NULL;
	}

	INT16 n16OldIndex = pItem->n16Index;

	// ����װ��
	tagEquip *pEquipSrc = GetHolySoulBar().Remove(n64Serial);
	if(NULL == pEquipSrc)
	{
		ASSERT(pEquipSrc != NULL);
		return NULL;
	}

	// ��ͻ��˷���Ϣ
	SendDelItem2Client(GetHolySoulBar().GetConType(), n16OldIndex, n64Serial, 0);

	// ����Ӱ��
	m_pRole->GetHolySoul().ProcEquipEffect(NULL, (tagEquip*)pItem, n16OldIndex, TRUE, TRUE);

	// �����ݿ���ɾ��
	DeleteItemFromDB(n64Serial, pItem->dwTypeID);

	// ����Ƿ���Ҫ��¼log -- ��ʹ��ʱ��ƷҪ����Ϸ������ɾ��,��ʱ�ڴ�Ҳû���ͷ�
	LogItem(*pItem, NULL, 1, dwCmdID);

	// ����Ƿ����ͷŸ���Ʒ�ڴ�
	if(bDelMem)
	{
		Destroy(pItem);
		return NULL;
	}

	return pItem;


}

//-----------------------------------------------------------------------------
// ��װ������ֱ��ɾ��һ��װ��
//-----------------------------------------------------------------------------
tagItem* ItemMgr::RemoveFromEquipBar(INT64 n64Serial, DWORD dwCmdID, BOOL bDelMem)
{
	tagItem *pItem = GetEquipBar().GetItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(P_VALID(pItem));
		IMSG(_T("Can not find item<serial: %lld> in container<Type: %d, RoleID: %u>!!!\r\n"),
			n64Serial, GetEquipBar().GetConType(), m_pRole->GetID());
		return NULL;
	}

	INT16 n16OldIndex = pItem->n16Index;

	// ����װ��
	tagEquip *pEquipSrc = GetEquipBar().Remove(n64Serial);
	if(NULL == pEquipSrc)
	{
		ASSERT(pEquipSrc != NULL);
		return NULL;
	}

	// ��ͻ��˷���Ϣ
	SendDelItem2Client(GetEquipBar().GetConType(), n16OldIndex, n64Serial, 0);

	// ����Ӱ��
	m_pRole->ProcEquipEffect(NULL, (tagEquip*)pItem, n16OldIndex, TRUE, TRUE);

	// �����ݿ���ɾ��
	DeleteItemFromDB(n64Serial, pItem->dwTypeID);

	// ����Ƿ���Ҫ��¼log -- ��ʹ��ʱ��ƷҪ����Ϸ������ɾ��,��ʱ�ڴ�Ҳû���ͷ�
	LogItem(*pItem, NULL, 1, dwCmdID);

	// ����Ƿ����ͷŸ���Ʒ�ڴ�
	if(bDelMem)
	{
		Destroy(pItem);
		return NULL;
	}

	return pItem;
}

//-----------------------------------------------------------------------------
// ����&��ɫ�ֿ�����
//-----------------------------------------------------------------------------
DWORD ItemMgr::ExtendBag( INT64 n64serial )
{
	ItemContainer &bag = GetBag();

	// �Ƿ����
	if(bag.GetMaxSpaceSize() <= bag.GetCurSpaceSize())
	{
		return E_Con_Cannot_Extend;
	}

	// �����Ʒ
	tagItem* pItem = GetBagItem(n64serial);
	if (!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_BagExtend)
	{
		return E_Item_NotExist;
	}

	// �۳���Ʒ
	if (E_Success != ItemUsedFromBag(n64serial, 1, ELCLD_Bag_Extend))
	{
		return E_Item_UseErr;
	}

	// ���䱳��
	bag.IncreaseSize(SPACE_BAG_PER_EXTEND);

	return E_Success;
}

DWORD ItemMgr::ExtendRoleWare( INT64 n64Serial )
{
	ItemContainer &ware = GetRoleWare();

	// �Ƿ����
	if(ware.GetMaxSpaceSize() <= ware.GetCurSpaceSize())
	{
		return E_Con_Cannot_Extend;
	}

	// �����Ʒ
	tagItem* pItem = GetBagItem(n64Serial);
	if (!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_RoleWareExtend)
	{
		return E_Item_NotExist;
	}

	// �۳���Ʒ
	if (E_Success != ItemUsedFromBag(n64Serial, 1, ELCLD_RoleWare_Extend))
	{
		return E_Item_UseErr;
	}

	// ���䱳��
	ware.IncreaseSize(SPACE_WARE_PER_EXTEND);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �ӱ�����ȡ���������� -- ��db�����������Ʒ���ͷ�
//-----------------------------------------------------------------------------
DWORD ItemMgr::DiscardFromBag(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut)
{
	pOut = NULL;

	// �����Ƿ����
	if(!m_pRole->GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	tagItem *pItem = GetBag().GetItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	if(pItem->bLock)
	{
		return E_Equip_Lock;
	}

	// �����ʥ�飬����ͬʱɾ��ʥ�����ϵ�ʥ��
	if (MIsHoly(pItem))
	{
		tagHolyMan* pHolyMan = (tagHolyMan*)pItem;
		for (int i=0; i< MaxHolyEquipNum; i++)
		{
			if (pHolyMan->n64EquipSerialIDs[i] != 0)
			{
				RemoveItem(GetHolyManBar(), pHolyMan->n64EquipSerialIDs[i], dwCmdID, TRUE, FALSE, TRUE);				
			}
		}

	}

	if(pItem->IsBind()||pItem->IsAccountBind())
	{
		return RemoveItem(GetBag(), n64Serial, dwCmdID, TRUE, TRUE, TRUE);
	}

	pOut = pItem;
	return RemoveItem(GetBag(), n64Serial, dwCmdID, TRUE, FALSE, TRUE);
}

//-----------------------------------------------------------------------------
// �ӱ����е��� -- ��db��������󶨻�������Ʒ�ڴ��ͷ�
//-----------------------------------------------------------------------------
DWORD ItemMgr::LootFromBag(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut)
{
	pOut = NULL;

	tagItem *pItem = GetBag().GetItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	// װ���������ػ���������
	if(MIsEquipment(pItem->dwTypeID) 
		&& EESA_Guard_DeadPunish == ((tagEquip*)pItem)->equipSpec.bySpecAtt)
	{
		return E_Equip_Guard_DeadPunish;
	}

	if(pItem->IsBind() || pItem->bLock || pItem->IsAccountBind())
	{
		return RemoveItem(GetBag(), n64Serial, dwCmdID, TRUE, TRUE, FALSE);
	}

	pOut = pItem;
	return RemoveItem(GetBag(), n64Serial, dwCmdID, TRUE, FALSE, FALSE);
}

DWORD ItemMgr::DiscardFromWarBag(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut)
{
	pOut = NULL;

	// �����Ƿ����
	// 	if( !m_pRole->GetRoleStateEx().IsInState(ERSE_BagPsdPass) )
	// 	{
	// 		return E_Con_PswNotPass;
	// 	}

	tagItem* pItem = GetWarBag().GetItem(n64Serial);
	if( !P_VALID(pItem) )
		return GT_INVALID;

	if( pItem->bLock )
		return E_Equip_Lock;

	if( pItem->IsBind() )
		return RemoveItem(GetWarBag(), n64Serial, dwCmdID, TRUE, TRUE, TRUE);

	pOut = pItem;
	return RemoveItem(GetWarBag(), n64Serial, dwCmdID, TRUE, FALSE, TRUE);
}

DWORD ItemMgr::DiscardFromWarBag(INT64 n64Serial, DWORD dwCmdID)
{
	tagItem* pItem = GetWarBag().GetItem(n64Serial);
	if( !P_VALID(pItem) )
		return GT_INVALID;

	if( pItem->bLock )
		return E_Equip_Lock;

	if( pItem->IsBind() )
		return RemoveItem(GetWarBag(), n64Serial, dwCmdID, TRUE, TRUE, TRUE);

	return RemoveItem(GetWarBag(), n64Serial, dwCmdID, TRUE, FALSE, TRUE);
}

//-----------------------------------------------------------------------------
// �ӱ����е��� -- ����ɵ�����Ʒ������������Ʒ��Ϣ
//-----------------------------------------------------------------------------
DWORD ItemMgr::LootFromBag(INT nLootNum, TList<tagItem*>& listItems, DWORD dwCmdID, TList<DWORD>& listBroadcast)
{
	listItems.Clear();
	if( nLootNum <= 0 )
	{
		return E_Success;
	}

	// ��ȡ���пɵ�����Ʒ
	TList<tagItem*> listCanLoot;

	// ��ѯ����
	tagItem* pItem = NULL;
	TMap<INT64, INT16>::TMapIterator iter = GetBag().Begin();

	while( GetBag().GetNextItem(iter, pItem) )
	{
		// ���ɵ������Ʒ
		if( !pItem->pProtoType->bDeadLoot )
		{
			continue;
		}

		// ����װ����˵������֮�󲻻����
		if (MIsEquipment(pItem->dwTypeID))
		{
			tagEquip* pTmpEquip = (tagEquip*)pItem;
			//if (pTmpEquip->equipSpec.bySigned == 2)
			if( pTmpEquip->bLock )
			{
				continue;
			}
		}

		// װ���������ػ���������
		if( MIsEquipment(pItem->dwTypeID) 
			&& EESA_Guard_DeadPunish == ((tagEquip*)pItem)->equipSpec.bySpecAtt)
		{
			continue;
		}

		// �����б�
		listCanLoot.PushBack(pItem);
	}

	// û�пɵ�����Ʒ
	if( listCanLoot.Empty() )
	{
		return E_Success;
	}

	// �ɵ�����Ʒ����
	if( listCanLoot.Size() <= nLootNum )
	{
		// һ��һ�����ó����
		tagItem* pItem1 = listCanLoot.PopFront();

		while( P_VALID(pItem1) )
		{
			// ��������Ʒ���뵽�㲥������
			listBroadcast.PushBack(pItem1->dwTypeID);

			if( pItem1->IsBind() || pItem1->IsAccountBind()/*|| pItem->bLock*/ )
			{
				RemoveItem(GetBag(), pItem1->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
			}
			else
			{
				RemoveItem(GetBag(), pItem1->n64Serial, dwCmdID, TRUE, FALSE, FALSE);
				listItems.PushBack(pItem1);
			}

			pItem1 = listCanLoot.PopFront();
		}
	}
	// ����Ҫ�������Ʒ�����������ȡ
	else
	{
		// ���ȡ������������
		for(INT n = 0; n < nLootNum; ++n)
		{
			if( !listCanLoot.RandPeek(pItem, TRUE) )
			{
				continue;
			}

			// ��������Ʒ���뵽�㲥������
			listBroadcast.PushBack(pItem->dwTypeID);

			if( pItem->IsBind() || pItem->IsAccountBind()/*|| pItem->bLock*/ )
			{
				RemoveItem(GetBag(), pItem->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
			}
			else
			{
				RemoveItem(GetBag(), pItem->n64Serial, dwCmdID, TRUE, FALSE, FALSE);
				listItems.PushBack(pItem);
			}
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��װ�����е��� -- ��db��������󶨻�������Ʒ�ڴ��ͷ�
//-----------------------------------------------------------------------------
DWORD ItemMgr::LootFromEquipBar(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut)
{
	pOut = RemoveFromEquipBar(n64Serial, dwCmdID, FALSE);
	if(!P_VALID(pOut))
	{
		return GT_INVALID;
	}

	// ����Ƿ���Ҫ����ڴ�������
	if(pOut->IsBind() || pOut->bLock|| pOut->IsAccountBind())
	{
		Destroy(pOut);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��װ�����е��� -- ����ɵ�����Ʒ������������Ʒ��Ϣ
//-----------------------------------------------------------------------------
DWORD ItemMgr::LootFromEquipBar(INT nLootNum, TList<tagEquip*>& listItems, DWORD dwCmdID, TList<DWORD>& listBroadcast)
{
	listItems.Clear();
	if( nLootNum <= 0 )
	{
		return E_Success;
	}

	// ��ȡ���пɵ�����Ʒ
	TList<tagEquip*> listCanLoot;

	// ��ѯװ����
	tagEquip* pEquip = NULL;
	TMap<INT64, INT16>::TMapIterator iter = GetEquipBar().Begin();

	while( GetEquipBar().GetNextItem(iter, pEquip) )
	{
		// ���ɵ������Ʒ
		if( !pEquip->pProtoType->bDeadLoot )
		{
			continue;
		}

		// ��������װ��������
		//if(pEquip->equipSpec.bySigned == 2)
		// ȥ����������Ϊ����
		if( pEquip->bLock )
		{
			continue;
		}

		// װ���������ػ���������
		if( MIsEquipment(pEquip->dwTypeID) 
			&& EESA_Guard_DeadPunish == ((tagEquip*)pEquip)->equipSpec.bySpecAtt)
		{
			continue;
		}

		// �����б�
		listCanLoot.PushBack(pEquip);
	}

	// û�пɵ�����Ʒ
	if( listCanLoot.Empty() )
	{
		return E_Success;
	}

	// �ɵ�����Ʒ����
	if( listCanLoot.Size() <= nLootNum )
	{
		// һ��һ�����ó����
		tagEquip* pEquip = listCanLoot.PopFront();

		while( P_VALID(pEquip) )
		{
			// ��������Ʒ���뵽�㲥������
			listBroadcast.PushBack(pEquip->dwTypeID);

			if( pEquip->IsBind() || pEquip->IsAccountBind()/*|| pEquip->bLock*/ )
			{
				//RemoveItem(GetEquipBar(), pEquip->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
				RemoveFromEquipBar(pEquip->n64Serial, dwCmdID, TRUE);
			}
			else
			{
				//RemoveItem(GetEquipBar(), pEquip->n64Serial, dwCmdID, TRUE, FALSE, FALSE);
				tagItem * pDelItem = RemoveFromEquipBar(pEquip->n64Serial, dwCmdID, FALSE);
				if( P_VALID(pDelItem) )
				listItems.PushBack(pEquip);
			}

			pEquip = listCanLoot.PopFront();
		}
	}
	// ����Ҫ�������Ʒ�����������ȡ
	else
	{
		// ���ȡ������������
		for(INT n = 0; n < nLootNum; ++n)
		{
			if( !listCanLoot.RandPeek(pEquip, TRUE) )
			{
				continue;
			}

			DWORD dwTypeID = pEquip->dwTypeID;

			if( pEquip->IsBind() || pEquip->IsAccountBind()/*|| pEquip->bLock*/ )
			{
				//RemoveItem(GetEquipBar(), pEquip->n64Serial, dwCmdID, TRUE, TRUE, FALSE);
				RemoveFromEquipBar(pEquip->n64Serial, dwCmdID, TRUE);
			}
			else
			{
				//RemoveItem(GetEquipBar(), pEquip->n64Serial, dwCmdID, TRUE, FALSE, FALSE);
				tagItem * pDelItem = RemoveFromEquipBar(pEquip->n64Serial, dwCmdID, FALSE);
				if( P_VALID(pDelItem) )
				listItems.PushBack(pEquip);
			}

			// ��������Ʒ���뵽�㲥������
			listBroadcast.PushBack(dwTypeID);
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ָ���б���ɾ��ָ����������Ʒ
//-----------------------------------------------------------------------------
DWORD ItemMgr::DelBagSameItem(TList<tagItem*> &list, INT32 nNum, DWORD dwCmdID)
{
	ASSERT(list.Size() > 0 && nNum > 0);

	INT32 nNumLeft	= nNum;
	INT32 n16NumDel	= 0;
	tagItem *pItem;
	TList<tagItem*>::TListIterator iter = list.Begin();
	while(list.PeekNext(iter, pItem))
	{
		if(nNumLeft <= 0)
		{
			break;
		}

		if(nNumLeft >= pItem->GetNum())
		{
			n16NumDel = pItem->GetNum();
		}
		else
		{
			n16NumDel = (INT16)nNumLeft;
		}

		DelFromBag(pItem->n64Serial, dwCmdID, n16NumDel);
		nNumLeft -= n16NumDel;
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// �ٱ������
//-----------------------------------------------------------------------------
VOID ItemMgr::Add2BaiBao(tagItem *&pItem, DWORD dwCmdID, 
						  BOOL bReadFromDB/* = FALSE*/, DWORD dwRoleIDRel/* = GT_INVALID*/, BOOL bFromFamily/*  = FALSE*/)
{
	// ����Ƿ��л�ȡʱ��
	if(0 == pItem->dw1stGainTime && P_VALID(pItem->pProtoType) && pItem->pProtoType->eSpecFunc != EISF_ZhanDouFu)
	{
		pItem->dw1stGainTime = g_world.GetWorldTime();
	}

	if(bReadFromDB || GetBaiBaoFreeSize() > 0)
	{
		// �洢��item����
		DWORD dwErrorCode = AddItem(GetBaiBaoBag(), pItem, dwCmdID, TRUE, FALSE, dwRoleIDRel, FALSE, bFromFamily);

		if(dwErrorCode != E_Success)
		{
			// ��¼ʧ�ܲ���log���Ա��֤ //??
			ItemMgr::InsertBaiBao2DB(pItem, m_pRole->GetID(), dwCmdID);

			::Destroy(pItem); 
			ILOG->Write(_T("Add2BaiBao failed1!\r\n"));
		}
		return;
	}
	else
	{
		// �洢��item_baibao����

		// �����У��Ͳ��Űٱ�����ֹˢ��Ʒ
		tagItem * pTemItem = GetBagItem(pItem->n64Serial);
		if( P_VALID(pTemItem) )
			return;
		ItemMgr::InsertBaiBao2DB(pItem, m_pRole->GetID(), dwCmdID);

		Destroy(pItem);
	}

	return;
}

DWORD ItemMgr::ClearBaibao()
{
	return E_Success;
}

VOID ItemMgr::InsertBaiBao2DB(tagItem *pItem, DWORD dwRoleID, DWORD dwCmdID)
{
	tagRoleInfo* RoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if(!P_VALID(RoleInfo))	return;

	ASSERT(pItem != NULL);

	pItem->dwAccountID = RoleInfo->dwAccountID;
	pItem->dwOwnerID = dwRoleID;
	// ��ʱ������Ʒ�����Ա��浽item_baibao��
	pItem->n16Index = 0;
	pItem->SetConType(EICT_Baibao);

	if(MIsEquipment(pItem->dwTypeID))
	{
		tagNDBC_NewBaiBaoEquip send;
		memcpy(&send.equip, pItem, SIZE_EQUIP);
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		tagNDBC_NewBaiBaoItem send;
		memcpy(&send.item, pItem, SIZE_ITEM);
		g_dbSession.Send(&send, send.dwSize);
	}

	// log����//??
}

DWORD ItemMgr::ProcBaiBaoRecord(DWORD dwTypeID, DWORD dwDstRoleID, DWORD dwSrcRoleID, INT16 n16Type, DWORD dwTime, LPCTSTR szLeaveWords)
{
	// �ϲ��豣֤��Ʒ����ɫ�İ�ȫ��,���������¼��ж�
	tagRoleInfo* RoleInfo = g_roleMgr.GetRoleInfo(dwDstRoleID);

	if(!P_VALID(RoleInfo))
		return GT_INVALID;

	INT nLen = _tcsnlen(szLeaveWords, Max_LeaveWord_Length);
	INT nSize = sizeof(tagBaiBaoRecord) + (nLen * sizeof(TCHAR));

	//��DB���ͱ�������
	MCREATE_MSG(pSendDB, sizeof(tagNDBC_AddBaiBaoLog)+nSize-sizeof(tagBaiBaoRecord), NDBC_AddBaiBaoLog);
	pSendDB->dwAccountID = RoleInfo->dwAccountID;

	tagBaiBaoRecord* pRecord = &(pSendDB->LogInfo);

	pRecord->n16Size = nSize;
	pRecord->dwRoleID = dwSrcRoleID;
	pRecord->dwTypeID = dwTypeID;
	pRecord->n16Type = n16Type;

	if (dwTime == GT_INVALID || dwTime == 0)
	{
		pRecord->dwTime = g_world.GetWorldTime();
	}
	else
	{
		pRecord->dwTime = dwTime;
	}

	_tcsncpy(pRecord->szWords, szLeaveWords, nLen);

	g_dbSession.Send(pSendDB, pSendDB->dwSize);


	// �ж�Ŀ������Ƿ�����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwDstRoleID);
	if (P_VALID(pRole))
	{
		// ��ͻ��˷��ͼ�¼ͬ����Ϣ
		MCREATE_MSG(pSend, sizeof(tagNS_SingleBaiBaoRecord)-sizeof(tagBaiBaoRecord)+nSize, NS_SingleBaiBaoRecord);
		IFASTCODE->MemCpy(&(pSend->sRecord), pRecord, nSize);
		pRole->SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	MDEL_MSG(pSendDB);	

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��Ʒ��ӵ���������
//-----------------------------------------------------------------------------
BOOL ItemMgr::IsMaxHoldLimitItem( DWORD dwTypeID )
{
	if (MIsEquipment(dwTypeID))
	{
		tagEquipProto* pTempEquipProto = g_attRes.GetEquipProto(dwTypeID);
		if (P_VALID(pTempEquipProto))
		{
			return pTempEquipProto->n16MaxHoldNum != GT_INVALID;
		}
	}
	else
	{
		tagItemProto* pTempItemProto = g_attRes.GetItemProto(dwTypeID);
		if (P_VALID(pTempItemProto))
		{
			return pTempItemProto->n16MaxHoldNum != GT_INVALID;
		}
	}

	return FALSE;
}

DWORD ItemMgr::AddMaxHoldItem( DWORD dwTypeID, INT nNum )
{
	//if (!IsMaxHoldLimitItem(dwTypeID)) return E_Success;
	//�ϲ㱣֤����Ʒ��ͨ����֤
	m_mapMaxHold.ModifyValue(dwTypeID, nNum);

	return E_Success;
}

DWORD ItemMgr::AddMaxHoldItem( const tagItem& item )
{
	if (item.IsBind() && item.dwOwnerID != m_pRole->GetID()) return E_Success;

	m_mapMaxHold.ModifyValue(item.dwTypeID, item.GetNum());

	return E_Success;
}

VOID ItemMgr::RemoveMaxHoldItem( DWORD dwTypeID, INT nNum )
{
	if (!IsMaxHoldLimitItem(dwTypeID)) return;

	INT HoldNum = m_mapMaxHold.Peek(dwTypeID);

	if (HoldNum == (INT)GT_INVALID || HoldNum < nNum)
	{
		IMSG(_T("\nDec Holdnum <item:%d, hold num: %d, dec num: %d> failed!\n\n"),
			dwTypeID, HoldNum, nNum);
		ASSERT(0);
		return;
	}

	m_mapMaxHold.ModifyValue(dwTypeID, -nNum);
}

BOOL ItemMgr::CanAddMaxHoldItem( DWORD dwTypeID, INT nNum )
{
	if (!IsMaxHoldLimitItem(dwTypeID)) return TRUE;

	INT HoldNum = m_mapMaxHold.Peek(dwTypeID);
	if ( HoldNum == (INT)GT_INVALID)
	{
		HoldNum = 0;
	}

	const tagItemProto* pProto = NULL;

	if (MIsEquipment(dwTypeID))
	{
		pProto = g_attRes.GetEquipProto(dwTypeID);
	}
	else
	{
		pProto = g_attRes.GetItemProto(dwTypeID);
	}
	
	return P_VALID(pProto) ? nNum + HoldNum <= pProto->n16MaxHoldNum : FALSE;
}

BOOL ItemMgr::CanAddMaxHoldItem( const tagItem& item )
{
	if (!IsMaxHoldLimitItem(item.dwTypeID)) return TRUE;

	if (item.IsBind() && item.dwOwnerID != m_pRole->GetID()) return TRUE;

	INT HoldNum = m_mapMaxHold.Peek(item.dwTypeID);
	if ( HoldNum == (INT)GT_INVALID)
	{
		HoldNum = 0;
	}

	return item.GetNum() + HoldNum <= item.pProtoType->n16MaxHoldNum;
}

//-----------------------------------------------------------------------------
// ����ո�¶ȴ���
//-----------------------------------------------------------------------------
VOID ItemMgr::ProcEquipNewness()
{
	tagEquip *pEquip = GetEquipBarEquip((INT16)EEP_RightHand);
	if(!P_VALID(pEquip))
	{
		return;
	}

	INT nAttackTimes = 0;
	pEquip->IncAttackTimes();
	if(pEquip->IsNewessChange(nAttackTimes))
	{
		tagNS_RWNewessChange send;
		send.nAttackTimes = nAttackTimes;
		SendMessage(&send, send.dwSize);


		// �����˺�Ӱ��
		m_pRole->ResetWeaponDmg(*pEquip);
	}
}

VOID ItemMgr::InsertBaiBao2DBEx( tagItem *pItem, DWORD dwAccountId, DWORD dwCmdID )
{
	ASSERT(pItem != NULL);

	pItem->dwAccountID = dwAccountId;
	pItem->dwOwnerID = GT_INVALID;
	// ��ʱ������Ʒ�����Ա��浽item_baibao��
	pItem->n16Index = 0;
	pItem->SetConType(EICT_Baibao);

	if(MIsEquipment(pItem->dwTypeID))
	{
		tagNDBC_NewBaiBaoEquip send;
		memcpy(&send.equip, pItem, SIZE_EQUIP);
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		tagNDBC_NewBaiBaoItem send;
		memcpy(&send.item, pItem, SIZE_ITEM);
		g_dbSession.Send(&send, send.dwSize);
	}
}

//Jason 2009-11-25
BOOL ItemMgr::AddEquipPotVal(INT64 n64Serial,INT val)
{
// 	tagEquip *pEquip = (tagEquip*)GetBagItem(n64Serial);
// 	if(!P_VALID(pEquip))
// 		return FALSE;
// 
// 	if(!MIsEquipment(pEquip->dwTypeID))
// 		return FALSE;
// 
// 	//����ɹ���
// 	//����������
// 
// 	pEquip->equipSpec.nPotVal += val;
// 	UpdateEquipSpec(*pEquip);
	return TRUE;
}

//-----------------------------------------------------------------------------
// ����Ʒ
//-----------------------------------------------------------------------------
VOID ItemMgr::SendItemBind( INT64 n64Seria, DWORD dwContTypel )
{
	tagNS_ItemBind send;
	send.n64Serial = n64Seria;
	send.eConType = (EItemConType)dwContTypel;
	if (P_VALID(m_pRole) && P_VALID(m_pRole->GetSession()))
	{
		m_pRole->GetSession()->SendMessage(&send, send.dwSize);
	}
}

//-----------------------------------------------------------------------------
// װ���İ�״̬�ı�ʱ֪ͨ�ͻ���
//-----------------------------------------------------------------------------
VOID ItemMgr::SendItemBindStatus(INT64 n64Seria, DWORD dwContTypel, BYTE byStatus)
{
	tagNS_ItemBindStatus send;
	send.n64Serial = n64Seria;
	send.eConType = (EItemConType)dwContTypel;
	send.byStatus = byStatus;
	if (P_VALID(m_pRole) && P_VALID(m_pRole->GetSession()))
	{
		m_pRole->GetSession()->SendMessage(&send, send.dwSize);
	}
}

//-----------------------------------------------------------------------------
// ����TypeID�õ������ҵ�����,����ΪNULL
//-----------------------------------------------------------------------------
tagItem* ItemMgr::GetItemByTypeID(DWORD dwTypeID)
{
	// �������ң����ݴ�(0--��ǰ���)
	for ( int i = 0; i < GetBagCurSize(); i++)
	{
		tagItem* pItem = (tagItem *) GetBag().GetItem((INT16)i);
		if (NULL != pItem && pItem->dwTypeID == dwTypeID)
			return pItem;
	}
	return NULL;
}
// Jason 2010-3-19 v1.5.0
INT	 ItemMgr::GetOneTypeItemFreeSpace( DWORD dwTypeID )
{
	INT re = 0;
	if( IsMaxHoldLimitItem(dwTypeID) )
	{
		INT HoldNum = m_mapMaxHold.Peek(dwTypeID);
		if ( HoldNum == (INT)GT_INVALID)
		{
			HoldNum = 0;
		}

		const tagItemProto* pProto = NULL;

		if (MIsEquipment(dwTypeID))
		{
			pProto = g_attRes.GetEquipProto(dwTypeID);
		}
		else
		{
			pProto = g_attRes.GetItemProto(dwTypeID);
		}

		if( P_VALID(pProto) && pProto->n16MaxHoldNum > 1 )
		{
			re = pProto->n16MaxHoldNum - HoldNum;
			if( re < 0 )
				re = 0;
		}
		else
			re = GetBagFreeSize();
	}
	else
		re = GetBagFreeSize();

	return re;
}

// Jason 2010-4-15 v2.0.0
VOID ItemMgr::SendFabaoSpec2Client(const tagFabao & fabao)
{
	tagNS_FabaoChange send;
	send.n64Serial = fabao.n64Serial;
	IFASTCODE->MemCpy(&send.fabaoSpec, (tagFabaoSpec*)&fabao, sizeof(tagFabaoSpec));
	SendMessage(&send, send.dwSize);
}
VOID ItemMgr::SendFabaoSpec2DB(const tagFabao & fabao)
{
	tagNDBC_UpdateFabaoAtt send;
	send.n64FabaoID = fabao.n64Serial;
	IFASTCODE->MemCpy(&send.fabaoSpec, (tagFabaoSpec*)&fabao, sizeof(tagFabaoSpec));
	g_dbSession.Send(&send, send.dwSize);
}

// wcy 2012-4-09
VOID ItemMgr::SendHolySpec2Client(const tagHolyMan & holy)
{
	tagNS_ShengLingChg send;
	send.n64Serial = holy.n64Serial;
	IFASTCODE->MemCpy(&send.info, (tagHolySpec*)&holy, sizeof(tagHolySpec));
	SendMessage(&send, send.dwSize);
}

VOID ItemMgr::SendHolyEquipSpec2Client(const tagHolyEquip & holyEquip)
{
	tagNS_HolyEquipChg send;
	send.n64Serial = holyEquip.n64Serial;
	IFASTCODE->MemCpy(&send.info, (tagHolyEquipSpec*)&holyEquip, sizeof(tagHolyEquipSpec));
	SendMessage(&send, send.dwSize);
}

VOID ItemMgr::SendHolySpec2DB(const tagHolyMan & holy)
{
	tagNDBC_UpdateHolyAtt send;
	send.n64HolyID = holy.n64Serial;
	IFASTCODE->MemCpy(&send.holySpec, (tagHolySpec*)&holy, sizeof(tagHolySpec));
	g_dbSession.Send(&send, send.dwSize);
}

VOID ItemMgr::SendHolyEquipSpec2DB(const tagHolyEquip & holyEquip)
{
	tagNDBC_UpdateHolyEquipAtt send;
	send.n64ID = holyEquip.n64Serial;
	IFASTCODE->MemCpy(&send.holyEquipSpec, (tagHolyEquipSpec*)&holyEquip, sizeof(tagHolyEquipSpec));
	g_dbSession.Send(&send, send.dwSize);
}

VOID ItemMgr::SendSoulCrystalSpec2DB(const tagSoulCrystal& obj)
{
	tagNDBC_UpdateSoulCrystal send;
	send.n64ID = obj.n64Serial;
	IFASTCODE->MemCpy(&send.objspec, &(obj.SoulSpec), sizeof(tagSoulCrystalSpec));
	g_dbSession.Send(&send, send.dwSize);
}

VOID	ItemMgr::UpdateFabaoSpec(const tagFabao & fabao,BOOL bSave2DB)
{
	SendFabaoSpec2Client(fabao);
	// ���ܻ��лص������ˣ�����û�а취����Ϊ�о���ֳɣ������ɱ��ʱ���϶���Ӱ�챣�浽���ݿ������
	if( bSave2DB )
		SendFabaoSpec2DB(fabao);
}

VOID ItemMgr::UpdateSoulCrystalSpec(const tagSoulCrystal & obj, BOOL bSave2DB)
{
	SendSoulCrystalSpec2Client(obj);

	if(bSave2DB)
		SendSoulCrystalSpec2DB(obj);
}

VOID ItemMgr::UpdateHolySpec(const tagHolyMan & obj,BOOL bSave2DB)
{
	SendHolySpec2Client(obj);

	if(bSave2DB)
		SendHolySpec2DB(obj);
}

VOID	ItemMgr::UpdateHolyEquipSpec(const tagHolyEquip & obj, BOOL bSave2DB)
{
	SendHolyEquipSpec2Client(obj);

	if(bSave2DB)
		SendHolyEquipSpec2DB(obj);
}

VOID ItemMgr::SendSoulCrystalSpec2Client(const tagSoulCrystal& obj)
{
	tagNS_SoulCrystalChg send;
	send.n64Serial = obj.n64Serial;
	IFASTCODE->MemCpy(&send.info, &obj.SoulSpec, sizeof(tagSoulCrystalSpec));
	SendMessage(&send, send.dwSize);
}

VOID	ItemMgr::SynchFabaoMood(const tagFabao & fabao,INT nOldMood,BOOL bSaveToDB)
{
	// ע��������û��֪ͨ�ͻ��˴�������buff��Ϣ ����Ҫʱ�ټӰ�
	if( bSaveToDB )
	{
		tagNDBC_FabaoMoodChanged dbmsg;
		dbmsg.n64SerialID	= fabao.n64Serial;
		dbmsg.nMood		= fabao.nMood;
		dbmsg.nMoodTicks	= fabao.nMoodTicks;
		g_dbSession.Send(&dbmsg, dbmsg.dwSize);
	}
	//tagNS_FabaoMoodChanged msg;
	//msg.n64Serial	= fabao.n64Serial;
	//msg.nOldMood	= nOldMood;
	//msg.nNewMood	= fabao.nMood;
	//SendMessage(&msg,msg.dwSize);
	SendFabaoSpec2Client(fabao);
}

VOID	ItemMgr::SynchHolyCoValue(const tagHolyMan & holy,BOOL bSaveToDB)
{
	if( bSaveToDB )
	{
		tagNDBC_HolyCoValue dbmsg;
		dbmsg.n64SerialID	= holy.n64Serial;
		dbmsg.nCoValue		= holy.nCoValue;		
		g_dbSession.Send(&dbmsg, dbmsg.dwSize);
	}
	
	//SendHolySpec2Client(holy);

	tagNS_ShengLingCoValueChg msg;
	msg.nCoValue = holy.nCoValue;
	SendMessage(&msg,msg.dwSize);
}

VOID	ItemMgr::UpdateFabaoEleInjuryType2DB(const tagFabao & fabao)
{
	tagNDBC_UpdateFabaoEleInjuryType msg;
	msg.n64FabaoID = fabao.n64Serial;
	msg.eInjury	= fabao.eEleInjury;
	g_dbSession.Send(&msg, msg.dwSize);
}
	
VOID	ItemMgr::UpdateFabaoNirVanaData2DB(const tagFabao & fabao)
{
	if( !P_VALID(m_pRole) )
		return ;
	PlayerSession *pSession = m_pRole->GetSession();
	if( !P_VALID(pSession) )
		return;
	if( fabao.n16Stage < 60 )
		return;
	tagNDBC_UpdateFabaoNirVanaData msg;
	msg._n64ID	= fabao.n64Serial;
	msg.dwAccountID	= pSession->GetSessionID();
	msg.dwRoleID		= m_pRole->GetID();
	msg._data.nStage	= (fabao.n16Stage / 10) * 10;	// ȡ��
	msg._data.nEleInjuryType	= fabao.eEleInjury;
	msg._data.nEleInjury		= fabao.nEleInjury;
	msg._data.nElePierce		= fabao.nElePierce;
	msg._data.wEleInjPromoteTimes= fabao.wEleInjPromoteTimes;
	msg._data.wEleResPromoteTimes= fabao.wEleResPromoteTimes;
	memcpy(msg._data.nEleResistance,fabao.nEleResistance,sizeof(msg._data.nEleResistance));
	msg._data.wDamageUpgradeTimes	= fabao.wDamageUpgradeTimes;
	msg._data.wResistanceUpgradeTimes = fabao.wResistanceUpgradeTimes;
	//memcpy(msg._data.dwFairySkill,fabao.dwFairySkill,sizeof(msg._data.dwFairySkill));
	//msg._data.dwUnsureSkill = fabao.dwUnsureSkill;
	g_dbSession.Send(&msg, msg.dwSize);
}

VOID	ItemMgr::GetFabaoNirVanaDataFromDB(const tagFabao & fabao,BOOL bFirstOrLast)
{
	if( !P_VALID(m_pRole) )
		return ;
	PlayerSession *pSession = m_pRole->GetSession();
	if( !P_VALID(pSession) )
		return;
	tagNDBC_GetFabaoNirVanaData msg;
	msg.n64ID			= fabao.n64Serial;
	msg.dwAccountID	= pSession->GetSessionID();
	msg.dwRoleID		= m_pRole->GetID();
	msg.nCurStage		= fabao.n16Stage;
	msg.bFirstOrLast	= bFirstOrLast;
	g_dbSession.Send(&msg, msg.dwSize);
}

tagItem * ItemMgr::GetItemFromAllContainer(INT64 n64Serial)
{
	tagItem * p = NULL;
	p = m_Bag.GetItem(n64Serial);
	if( !P_VALID(p) )
	{
		p = m_RoleWare.GetItem(n64Serial);
		if( P_VALID(p) && p->dwOwnerID != m_pRole->GetID() )
			p = NULL;
	}
	if( !P_VALID(p) )
	{
		p = m_TempBag.GetItem(n64Serial);
		if( P_VALID(p) && p->dwOwnerID != m_pRole->GetID() )
			p = NULL;
	}
	if( !P_VALID(p) )
		p = m_EquipBar.GetItem(n64Serial);
	if (!P_VALID(p))
	{
		p = m_WarBag.GetItem(n64Serial);
		if (P_VALID(p) && p->dwOwnerID != m_pRole->GetID())
		{
			p = NULL;
		}
	}
	return p;
}
tagItem * ItemMgr::GetItemFromContainer(INT64 n64Serial,EItemConType eCon)
{
	switch(eCon)
	{
	case EICT_Bag:
		return GetBagItem(n64Serial);
		break;
	case EICT_RoleWare:
		return m_RoleWare.GetItem(n64Serial);
		break;
	case EICT_TempBag:
		return m_TempBag.GetItem(n64Serial);
		break;
	case EICT_Equip:
		return m_EquipBar.GetItem(n64Serial);
		break;
	case EICT_WarBag:
		return m_WarBag.GetItem(n64Serial);
	case EICT_HolyManEquip:
		return m_HolyManBar.GetItem(n64Serial);
		break;
	}
	return NULL;
}

VOID	ItemMgr::InsertMatingFabao(tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return ;
	//tagFariyMatingData tem;
	//tem._fairy_id = pFabao->n64Serial;
	//tem._con_type = pFabao->eConType;
	if( !pFabao->CanMating() )
		return;
	m_FairyMating.Add(pFabao->n64Serial ,pFabao->eConType);
}
VOID	ItemMgr::RemoveMatingFabao(tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return ;
	m_FairyMating.Erase(pFabao->n64Serial);
}

static BOOL ChangeFabaoMatingTicks( tagFabao * pFabao, DWORD dwTicks );

VOID	ItemMgr::UpdateFabaoMating( )
{
	if( !g_world.IsFairyMatingOpen() )
		return ;

	if( !P_VALID(m_pRole) )
		return;

	if( m_dwLastFabaoMatingTick > 0 && m_dwLastFabaoMatingTick < TICK_PER_SECOND * 5 )
	{
		++m_dwLastFabaoMatingTick;
		return;
	}
	if( m_dwLastFabaoMatingTick == 0 )
		++m_dwLastFabaoMatingTick;
	else
		m_dwLastFabaoMatingTick = 1;

	DWORD dwTicks = 5;

	TMap<UINT64,EItemConType >::TMapIterator it = m_FairyMating.Begin();
	UINT64 id = 0;
	EItemConType contype;
	tagFabao * pFabao = NULL;
	tagItem * pItem = NULL;
	EFabaoStatus eFabaoStatus;

	TList<UINT64> lstToDel ;
	BOOL bCannotDec = FALSE;

	while(m_FairyMating.PeekNext(it,id,contype))
	{
		pFabao = NULL;
		pItem = NULL;
		bCannotDec = FALSE;

		pItem = GetItemFromAllContainer(id);
		
		if( !MIsFaBao(pItem) )
			continue;
		pFabao = (tagFabao*)pItem;

		switch(pFabao->eFabaoStatus)
		{
		case EEFS_Waiting:	// �ȴ���������Ӧ��
			eFabaoStatus = EEFS_Normal;
			//ILOG->Write(_T("UpdateFabaoMating1,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pFabao->nBirthTimes, pFabao->n64Serial );
			break;
		case EEFS_Pregnancy:	// ����
			eFabaoStatus = EEFS_GiveBirth;
			if( pFabao->nMood <= 799 )
				bCannotDec = TRUE;
			break;
		case EEFS_Rest:	// ��Ϣ
			eFabaoStatus = EEFS_Normal;
			//ILOG->Write(_T("UpdateFabaoMating2,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pFabao->nBirthTimes, pFabao->n64Serial );
			break;
		default:
			continue;
		}

		if( !bCannotDec && ChangeFabaoMatingTicks(pFabao,dwTicks) )
		{
			if(  pFabao->eFabaoStatus == EEFS_Waiting )
			{
				FairyMatingMgr* pMatingMgr = NULL;
				Role * pOther = NULL;
				tagFabao * pMyFabao = NULL, *pHisFabao = NULL;
				tagFairyInviteBirthData * pData = NULL;
				if( GetFabaoMatingOtherInfo(m_pRole,pOther,pMyFabao,pHisFabao,pData,pMatingMgr) )
				{
					tagNS_FairyMatingCancelAll back;
					m_pRole->SendMessage(&back,back.dwSize);
					pOther->SendMessage(&back,back.dwSize);
					pMatingMgr->Remove(m_pRole->GetID());
				}
			}
			BOOL bNormalChg = pFabao->LogFairyBirth(pFabao->eFabaoStatus, eFabaoStatus);
			if (!bNormalChg)
				ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
			pFabao->eFabaoStatus = eFabaoStatus;
			pFabao->dwStatusChangeTime = 0;
			UpdateFabaoSpec(*pFabao);
			lstToDel.PushBack(id);
		}
	}
	
	lstToDel.ResetIterator();
	while( lstToDel.PeekNext(id) )
	{
		m_FairyMating.Erase(id);
	}
}
static BOOL ChangeFabaoMatingTicks( tagFabao * pFabao, DWORD dwTicks )
{
	DWORD dwLastValue = pFabao->dwStatusChangeTime;
	if( pFabao->dwStatusChangeTime < dwTicks )
		pFabao->dwStatusChangeTime = 0;
	else
		pFabao->dwStatusChangeTime -= dwTicks;
	if( pFabao->dwStatusChangeTime < 1 && dwLastValue != pFabao->dwStatusChangeTime )
		return TRUE;
	return FALSE;
}

VOID	ItemMgr::SaveMatingFabao2DB()
{
	if( !g_world.IsFairyMatingOpen() )
		return ;

	TMap<UINT64,EItemConType >::TMapIterator it = m_FairyMating.Begin();
	UINT64 id = 0;
	EItemConType contype;
	tagFabao * pFabao = NULL;
	tagItem * pItem = NULL;
	//EFabaoStatus eFabaoStatus;

	TList<UINT64> lstToDel ;

	while(m_FairyMating.PeekNext(it,id,contype))
	{
		pFabao = NULL;
		pItem = NULL;
		
		pItem = GetItemFromAllContainer(id);

		if( !MIsFaBao(pItem) )
			continue;
		pFabao = (tagFabao*)pItem;

		SendFabaoSpec2DB(*pFabao);
	}
}

BOOL tagFabao::LogFairyBirth(INT nBeforeChangeBirthState, INT nAfterChangeBirthState)
{
	tagNDBC_LogFairyBirth send;
	send.sLogFairyBirth.dwAccountID = dwAccountID;
	send.sLogFairyBirth.dwRoleID = dwOwnerID;
	send.sLogFairyBirth.nFaBao64Serial = n64Serial;
	send.sLogFairyBirth.bNormalChange = IsBirthChangeValid(nBeforeChangeBirthState, nAfterChangeBirthState);
	send.sLogFairyBirth.nBeforeChangeBirthState = nBeforeChangeBirthState;
	send.sLogFairyBirth.nAfterChangeBirthState = nAfterChangeBirthState;
	g_dbSession.Send(&send, send.dwSize);

	return send.sLogFairyBirth.bNormalChange;
}
//-----------------------------------------------------------------------------
// ͨ����Ʒ�������� 
DWORD ItemMgr::AddAreaByItme( INT64 n64ItemID )
{
	if(!P_VALID(m_pRole))
		return E_UseItem_ScriptError;

	// �����Ʒ
	tagItem* pItem = GetBagItem(n64ItemID);
	if (!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_GlobalLimitedItem)
	{
		return E_UseItem_ItemNotExist;
	}

	//Ӧ�߻�Ҫ����ϣ�û����ְ�Ͳ���ʹ��
	if( m_pRole->GetClergy() <= 0 )
	{
		return E_UseItem_ItemCanNotUseByClergy;
	}

	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(pItem->pProtoType->nSpecFuncVal2);
	if( !P_VALID(pGodAreaProto) )
	{
		return E_UseItem_ScriptError;
	}

	tagGlobalLimitedItem *pGlobalLimitedItem = NULL;
	tagGlobalLimitedItem OldGlobalLimitedItem;
	INT64 n64ID = 0;
	if( EGAT_RARE == pGodAreaProto->eType )
	{
		pGlobalLimitedItem = g_GlobalLimitedItemMgr.GetLimitedItemFromMap(pItem->pProtoType->dwTypeID,n64ItemID);
		if(!P_VALID(pGlobalLimitedItem) )
		{
			return E_UseItem_ScriptError;
		}
		n64ID = pGlobalLimitedItem->n64ID;
		OldGlobalLimitedItem = *pGlobalLimitedItem;
	}

	// ��������
	tagNS_AddArea send;
	send.dwAreaID = pGodAreaProto->dwID;
	send.bAdd = TRUE;
	send.dwErrorCode = m_pRole->ChangeArea( pGodAreaProto->dwID, TRUE, pGlobalLimitedItem );
	send.dwTime = 0;

	// �۳���Ʒ
	if(E_Success == send.dwErrorCode)
	{
		if ( E_Success != ItemUsedFromBag(n64ItemID, 1, EISF_GlobalLimitedItem, FALSE))
		{
			m_pRole->ChangeArea( pGodAreaProto->dwID, FALSE );

			if( P_VALID(pGlobalLimitedItem) )
				*pGlobalLimitedItem = OldGlobalLimitedItem;

			return E_UseItem_ScriptError;
		}
	}
	else
	{
		switch(send.dwErrorCode)
		{
		case E_God_Area_FaithValue:
			return E_UseItem_ItemCanNotUseByFaithValue;
		case E_God_Area_Condition:
		case E_SystemError:
			return E_UseItem_ScriptError;
		}
	}

	if( P_VALID(pGlobalLimitedItem) )
		send.dwTime = pGlobalLimitedItem->nTime;
	
	m_pRole->SendMessage(&send,send.dwSize);

	if(E_Success == send.dwErrorCode)
	{
		g_dbSession.SendCreateArea2DB(m_pRole->GetID(),pGodAreaProto->dwID,FALSE);

		if( P_VALID(pGlobalLimitedItem) )
		{
			g_GlobalLimitedItemMgr.SendUpdateRareArea2DB(pGlobalLimitedItem,n64ID);
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
VOID ItemMgr::SaveFamilySpriteToDB( tagFabao *pFabao, OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	nNum = 0;

	MTRANS_POINTER(pCurPointer, pData, tagItemUpdate);

	if(P_VALID(pFabao) && pFabao->eStatus != EUDBS_Null)
	{
		pCurPointer[nNum].n64Serial		= pFabao->n64Serial;
		pCurPointer[nNum].dwOwnerID		= pFabao->dwOwnerID;
		pCurPointer[nNum].dwAccountID	= pFabao->dwAccountID;
		pCurPointer[nNum].nUseTimes		= pFabao->nUseTimes;
		pCurPointer[nNum].n16Num		= pFabao->GetNum();
		pCurPointer[nNum].n16Index		= pFabao->n16Index;
		pCurPointer[nNum].byConType		= pFabao->eConType;
		pCurPointer[nNum].byBindType	= pFabao->byBind;
		pCurPointer[nNum].dw1stGainTime = pFabao->dw1stGainTime;
		//pCurPointer[nNum].dwUnlockTime	= pTemp->dwUnlockTime;
		pCurPointer[nNum].dwSpecVal1		= pFabao->dwSpecVal1;
		pCurPointer[nNum].dwSpecVal2		= pFabao->dwSpecVal2;

		pFabao->SetUpdate(EUDBS_Null);

		++nNum;
	}


	pOutPointer = &pCurPointer[nNum];
}
//-------------------------------------------------------------------------------


VOID ItemMgr::CheckAllEquip(Role* pRole,DWORD dwMapID)
{
	// ������
	tagEquip* pEquip = NULL;
	TMap<INT64, INT16>::TMapIterator iter = m_EquipBar.Begin();

	while( m_EquipBar.GetNextItem(iter, pEquip) )
	{
		if (MIsEquipment(pEquip->dwTypeID))
		{
			const tagEquipProto * pEquipProto = pEquip->pEquipProto;

			if( !P_VALID(pEquipProto) || !pEquipProto->bOnlyScriptStrengthen )
				continue;

			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript))
			{
				if(P_VALID(pEquipProto))
				{
					const INT32 nMask	= 0xFFFFFFFF;
					INT32 n32High	= (INT32)((pEquip->GetKey() >> 32) & nMask);
					INT32 n32Low	= (INT32)(pEquip->GetKey() & nMask);
					DWORD dwSucceed = 0;
					if(pEquipProto->bMapLimit)
					{
						dwSucceed = pRoleScript->OnRoleMapChangeWithScriptEquip( pRole, pEquipProto->dwTypeID, n32High, n32Low ,dwMapID);
						if (!dwSucceed)
						{
							pRole->ResetOneEquipDisplay(NULL, pEquip->pEquipProto->eEquipPos);
							//ɾ����Ч
							tagNS_Unequip send;
							send.n64Serial = GT_INVALID;
							send.dwErrorCode = E_Success;
							send.eType = pEquip->pEquipProto->eEquipPos;
							this->RemoveFromEquipBar(pEquip->GetKey(), (DWORD)ELCLD_Item_AtTerm, TRUE);
							

							// �������ͻ��˻�װ���
							SendMessage(&send, send.dwSize);
						}
					}
				}
			}
		}
	}
}


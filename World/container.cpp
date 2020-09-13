//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Container.cpp
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ������
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Container.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"
#include "../WorldDefine/msg_item.h"
#include "att_res.h"
#include "container_restrict.h"
#include "item_creator.h"
#include "world_session.h"

//****************************************** ItemContainer ***********************************************

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
ItemContainer::ItemContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize, 
							 DWORD dwOwnerID, DWORD dwAccountID, ContainerRestrict *pRestrict/*=NULL*/)
							 : Container(eConType, n16CurSize, n16MaxSize), m_TimeLimitMgr(ITEM_UPDATE_TIME)
{
	m_n16MinFreeIndex = 0;
	m_nZhanDouFuItemCnt = 0;

	m_dwOwnerID = dwOwnerID;
	m_dwAccountID = dwAccountID;

	m_pRestrict = (pRestrict != NULL ? pRestrict : new ContainerRestrict);
	m_pRestrict->init(this);
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
ItemContainer::~ItemContainer()
{
	m_n16MinFreeIndex = GT_INVALID;

	if(P_VALID(m_pRestrict))
		SAFE_DEL(m_pRestrict);
}

//-------------------------------------------------------------------------------------------------------
// ���������������Ʒ,����ָ�����λ��,���ش�����
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Add(tagItem* pItem, OUT INT16 &n16Index, OUT tagItemMove &ItemMove, 
						 BOOL bCheckAdd/* = TRUE*/, BOOL bChangeOwner/* = TRUE*/)
{
	ASSERT(pItem != NULL);

	if(pItem->GetNum() <= 0)
	{
		return GT_INVALID;
	}

	if(bCheckAdd && !m_pRestrict->CanAdd(pItem))
	{
		return E_Item_CanNotAdd;
	}

	// ��ʼ����������
	n16Index = GT_INVALID;
	ItemMove.pItem2	= NULL;
	ItemMove.n16NumRes1	= 0;
	ItemMove.n16NumRes2	= 0;
	ItemMove.bCreateItem = FALSE;
	ItemMove.bOverlap = FALSE;
	ItemMove.bChangePos = TRUE;

	INT16 n16Add = 0;
	
	if(pItem->pProtoType->n16MaxLapNum > 1)	// �ɶѵ���Ʒ
	{
		TList<tagItem*> itemList;
		GetSameItemList(itemList, pItem->dwTypeID);

		tagItem *pItemInCon;
		INT16 n16CanLap;

		// ��ͬ����Ʒ����һ��
		itemList.ResetIterator();
		while(itemList.PeekNext(pItemInCon))
		{
			// ���е�ͬ����Ʒ�Ǳ������ģ��򲻿ɶѵ����
			if( (pItem->bLock && !pItemInCon->bLock)  ||  (pItemInCon->bLock && ( !pItem->bLock || pItem->dwOwnerID != pItemInCon->dwOwnerID ) ))
			{
				continue;
			}

			n16CanLap = pItemInCon->pProtoType->n16MaxLapNum - pItemInCon->GetNum();
			if(n16CanLap >= pItem->GetNum())
			{
				if(pItem->dwOwnerID != pItemInCon->dwOwnerID
					&& pItem->IsBind())
				{
					continue;
				}
				
				if (pItem->IsTimeLimit() || pItemInCon->IsTimeLimit())
				{
					continue;
				}

				n16Add = pItem->GetNum();
				pItemInCon->AddNum(n16Add);

				// ���ô�������
				ItemMove.pItem2	= pItemInCon;
				ItemMove.n16NumRes1	= 0;
				ItemMove.n16NumRes2	= pItemInCon->GetNum();
				ItemMove.bOverlap	= TRUE;

				n16Index = pItemInCon->n16Index;

				// ���ø���������Ϣλ
				pItemInCon->SetUpdate(EUDBS_Update);

				break;
			}
		}
	}
	
	// ȷ���ɹ�֮���ټ��
	if (n16Add > 0 &&
		P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	// ͬ����Ʒ�зŲ���,������λ
	if(0 == n16Add)
	{		
		// ���ô�������
		ItemMove.n16NumRes1	= pItem->GetNum();
		n16Index = GetOneFreeSpace();

		if(GT_INVALID == n16Index)
		{
			IMSG(_T("Container is full!<roleid:%u, eConType:%d>"), m_dwOwnerID, GetConType());
			return E_Con_NotEnoughSpace;
		}
		
		return this->Add(pItem, n16Index, bChangeOwner, FALSE);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ָ��λ���������Ʒ,������ӳɹ���Ʒ����(ָ��λ�ñ���Ϊ��)
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Add(tagItem* pItem, INT16 n16NewIndex, BOOL bChangeOwner/* = TRUE*/, BOOL bCheckAdd/* = TRUE*/)
{
	// ��λ�úϷ����ж�
	//if(n16NewIndex < 0 || n16NewIndex >= GetCurSpaceSize())
	//{
	//	ASSERT(n16NewIndex >=0 && n16NewIndex < GetCurSpaceSize());
	//	return GT_INVALID;
	//}
	
	// �Ƿ�ɷ�������
	if(bCheckAdd && !m_pRestrict->CanAdd(pItem))
	{
		return E_Item_CanNotAdd;
	}
	
	// ������λ���Ƿ�Ϸ�
	if(!IsOnePlaceFree(n16NewIndex))
	{
		return E_Item_Place_NotFree;
	}

	// �ж���Ʒ�����Ϸ���
	if(pItem->GetNum() <= 0)
	{
		return GT_INVALID;
	}

	INT16 n16Add = Container::Add(pItem, n16NewIndex);
	if(0 == n16Add)
	{
		return E_Item_AddFailed;
	}
	
	ASSERT(n16Add == pItem->GetNum());
	if(bChangeOwner && !pItem->IsBind())
	{
		pItem->SetOwner(m_dwOwnerID, m_dwAccountID);
	}

	// �Ƿ�Ϊʱ����Ʒ
	if( pItem->IsTimeLimit() &&	GetConType() != EICT_Baibao)
	{
		m_TimeLimitMgr.Add2TimeLeftList(pItem->n64Serial, pItem->pProtoType->dwTimeLimit, pItem->dw1stGainTime);
	}

	//// ���ø���������Ϣλ
	//pItem->SetUpdate(EUDBS_Update);

	// ȷ���ɹ�֮���ټ��
	if (P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ָ����Ʒ,���ش�����(�ô������Ķ��巵��ֵ��ͬ)
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Remove(INT64 n64Serial, BOOL bChangeOwner/* = FALSE*/, BOOL bCheckRemove/* = TRUE*/)
{
	tagItem *pItem = GetItem(n64Serial);
	if(NULL == pItem)
	{
		return E_Item_NotFound;
	}

	if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
	{
		return E_Item_CanNotRemove;
	}

	BOOL bZhanDouFu = FALSE;
	if (P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		bZhanDouFu = TRUE;
	}

	INT16 n16OldIndex = pItem->n16Index;
	Container::Remove(n64Serial);

	if(bChangeOwner)
	{
		pItem->SetOwner(GT_INVALID, GT_INVALID);
	}

	// ����m_n16MinFreeIndex
	if(n16OldIndex < m_n16MinFreeIndex)
	{
		m_n16MinFreeIndex = n16OldIndex;
	}

	// �Ƿ�Ϊʱ����Ʒ
	if(pItem->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pItem->n64Serial);
	}

	if (bZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��ָ��������ָ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Remove(INT64 n64Serial, INT16 n16Num, BOOL bCheckRemove/* = TRUE*/)
{
	if(n16Num <= 0)
	{
		ASSERT(n16Num > 0);
		return GT_INVALID;
	}

	tagItem *pItem = GetItem(n64Serial);
	if(NULL == pItem)
	{
		return E_Item_NotFound;
	}

	if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
	{
		return E_Item_CanNotRemove;
	}

	BOOL bZhanDouFu = FALSE;
	if (P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		bZhanDouFu = TRUE;
	}

	// ����
	if(pItem->GetNum() < n16Num)
	{
		// ɾ��ʧ��,���ͼ����Ϣ //??

		ASSERT(pItem->GetNum() >= n16Num);
		return GT_INVALID;
	}

	// �պ�
	if(pItem->GetNum() == n16Num)
	{
		// ִ�е��ô��������ڴ�й© -- ���Ӧ���ñ�Ľӿ�
		ASSERT(0);
		return this->Remove(n64Serial, TRUE, FALSE);
	}

	// �и���
	pItem->AddNum(-n16Num);

	// ����
	pItem->SetUpdate(EUDBS_Update);

	// ȷ���ɹ�֮���ټ��
	if (bZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	return E_Success;
}

////-------------------------------------------------------------------------------------------------------
//// ��������ɾ��ĳ����Ʒ�����سɹ�����
////-------------------------------------------------------------------------------------------------------
//INT32 ItemContainer::Remove(DWORD dwTypeID, BOOL bCheckRemove/* = TRUE*/)
//{
//	INT32 nNumDel = 0;
//
//	TList<tagItem*> itemList;
//	GetSameItemList(itemList, dwTypeID);
//
//	tagItem *pItem;
//	itemList.ResetIterator();
//	while(itemList.PeekNext(pItem))
//	{		
//		nNumDel += this->Remove(pItem->n64Serial, TRUE, bCheckRemove);
//	}
//
//	return nNumDel;
//}
//
////-------------------------------------------------------------------------------------------------------
//// ��������ɾ��ָ��������ĳ����Ʒ�����سɹ�����
////-------------------------------------------------------------------------------------------------------
//INT32 ItemContainer::Remove(DWORD dwTypeID, INT32 nNum, BOOL bCheckRemove/* = TRUE*/)
//{
//	if(nNum <= 0)
//	{
//		ASSERT(nNum > 0);
//		return 0;
//	}
//	
//	TList<tagItem*> itemList;
//	INT32 nNumTotal = GetSameItemList(itemList, dwTypeID);
//
//	// ����
//	if(nNumTotal < nNum)
//	{
//		// ɾ��ʧ��,���ͼ����Ϣ
//
//		ASSERT(nNumTotal >= nNum);
//		return 0;
//	}
//
//	// �պ�, ����
//	INT16 n16NumDel = 0;
//	INT32 nNumNeedDel = nNum;
//	tagItem *pItem = NULL;
//	itemList.ResetIterator();
//	while(itemList.PeekNext(pItem) && nNumNeedDel != 0)
//	{
//		if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
//		{
//			ASSERT(0 == n16NumDel);	
//			continue;
//		}
//
//		if(pItem->n16Num <= nNumNeedDel)
//		{
//			n16NumDel = this->Remove(pItem->n64Serial, TRUE, FALSE);
//			ASSERT(n16NumDel == pItem->n16Num);
//			nNumNeedDel -= n16NumDel;
//		}
//		else
//		{
//			pItem->n16Num -= nNumNeedDel;
//			nNumNeedDel = 0;
//
//			// ���ݿⱣ��״̬
//		}
//	}
//
//	return nNum - nNumNeedDel;
//}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ�ƶ���ָ��λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64Serial1, INT16 n16Index2, OUT tagItemMove &ItemMove)
{
	// 0a.Ŀ��λ�úϷ��Լ��
	if(n16Index2 < 0 || n16Index2 >= GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	tagItem *pItem1, *pItem2;

	// 0b.����ƶ�Ŀ��
	pItem1 = GetItem(n64Serial1);
	if(NULL == pItem1 || pItem1->n16Index == n16Index2)
	{
		ASSERT(pItem1 != NULL);
		ASSERT(pItem1->n16Index != n16Index2);
		return GT_INVALID;
	}

	// ��ʼ����������
	ItemMove.pItem2			= NULL;
	ItemMove.n16NumRes1		= pItem1->GetNum();
	ItemMove.n16NumRes2		= 0;
	ItemMove.bCreateItem	= FALSE;
	ItemMove.bOverlap		= FALSE;
	ItemMove.bChangePos		= TRUE;

	// 1��Ŀ��λ��Ϊ��
	if(IsOnePlaceFree(n16Index2))
	{
		DWORD dwErrorCode = this->Remove(n64Serial1, FALSE,FALSE);
		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}

		return this->Add(pItem1, n16Index2, FALSE,FALSE);
	}

	// 2��Ŀ��λ�ò�Ϊ��
	pItem2 = GetItem(n16Index2);
	if(!P_VALID(pItem2))
	{
		ASSERT(P_VALID(pItem2));
		return GT_INVALID;
	}
	
	// �õ�Ŀ��λ����Ʒ��Ϣ
	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->GetNum();

	// ���Ŀ��λ����Ʒ�İ�״̬��Ҫ���ڸ�λ���ϵ���Ʒ�İ�״̬��ͬ�����ܷ���
	if ((pItem2->bLock^pItem1->bLock) || 
		(pItem1->bLock && pItem2->bLock && (pItem1->dwUnlockTime!=pItem2->dwUnlockTime)) )
	{
		return GT_INVALID;
	}

	// 2a.��Ʒ����(TypeID || ����ƷOwnerID)��ͬ,��ĳһ����Ʒ�����ﵽ�˶ѵ�����
	if(pItem1->dwTypeID != pItem2->dwTypeID 
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem1->GetNum() >= pItem1->pProtoType->n16MaxLapNum
		|| pItem2->GetNum() >= pItem2->pProtoType->n16MaxLapNum)
	{
		Swap(pItem1->n16Index, pItem2->n16Index);

		return E_Success;
	}

	// 2b.ͬ������Ʒ�������Ѿ�û�дﵽ�ѵ�����
	INT32 nNum = pItem1->pProtoType->n16MaxLapNum - pItem2->GetNum();
	nNum = min(nNum, pItem1->GetNum());
	pItem1->AddNum(-nNum);
	pItem2->AddNum(nNum);

	// ����������Ʒ��Ϣ
	ItemMove.bOverlap	= TRUE;
	ItemMove.bChangePos	= FALSE;
	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.n16NumRes2 = pItem2->GetNum();

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	// ��Ʒ1ȫ���ƶ�����Ʒ2�� -- ���ⲿ��飬�������ڴ�
	if(0 == pItem1->GetNum())
	{
		this->Remove(pItem1->n64Serial, TRUE, FALSE);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ָ��������Ʒ�ƶ���ָ��λ��,�����ƶ��ɹ��ĸ���
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64Serial1, INT16 n16NumMove, INT16 n16Index2, OUT tagItemMove &ItemMove)
{	
	if(n16NumMove <= 0)
	{
		ASSERT(n16NumMove > 0);
		return GT_INVALID;
	}
	
	tagItem *pItem1 = GetItem(n64Serial1);
	if(NULL == pItem1)
	{
		ASSERT(pItem1 != NULL);
		return E_Item_NotFound;
	}

	if(pItem1->GetNum() <= n16NumMove)
	{
		// ȫ���ƶ�
		return this->MoveTo(n64Serial1, n16Index2, ItemMove);
	}

	// ��ʼ����������
	ItemMove.pItem2			= NULL;
	ItemMove.n16NumRes1		= pItem1->GetNum();
	ItemMove.n16NumRes2		= 0;
	ItemMove.bCreateItem	= FALSE;
	ItemMove.bOverlap		= FALSE;
	ItemMove.bChangePos		= FALSE;

	// 0.Ŀ��λ�úϷ��Լ��
	if(n16Index2 < 0 || n16Index2 >= GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	// 1��Ŀ��λ��Ϊ��
	if(IsOnePlaceFree(n16Index2))
	{
		if (!P_VALID(pItem1->pProtoType))
			return E_Item_NotFound;
		if(n16NumMove > pItem1->pProtoType->n16MaxLapNum)
			return E_Item_CanNotAdd;

		pItem1->AddNum(-n16NumMove);

		// �������ݿⱣ��״̬
		pItem1->SetUpdate(EUDBS_Update);

		// ����������Ʒ��Ϣ
		ItemMove.n16NumRes1 = pItem1->GetNum();

		// �����µĶѵ���Ʒ
		tagItem *pNewItem = ItemCreator::Create(*pItem1, n16NumMove);
		if (!P_VALID(pNewItem))
		{
			pItem1->AddNum(n16NumMove);
			pItem1->SetUpdate(EUDBS_Null);
			ItemMove.n16NumRes1	= pItem1->GetNum();
			return E_Item_CanNotAdd;
		}
		// �õ�Ŀ��λ����Ʒ��Ϣ
		ItemMove.pItem2 = pNewItem;
		ItemMove.n16NumRes2 = pNewItem->GetNum();
		ItemMove.bCreateItem = TRUE;

		return this->Add(pNewItem, n16Index2, FALSE, FALSE);
	}

	// 2��Ŀ��λ�ò�Ϊ��
	if(pItem1->n16Index == n16Index2)
	{
		ASSERT(pItem1->n16Index != n16Index2);
		return GT_INVALID;
	}

	// ����Ʒ�ش��ڣ����򣬲���ִ�е��˴�
	tagItem *pItem2 = GetItem(n16Index2);

	// �õ�Ŀ��λ����Ʒ��Ϣ
	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->GetNum();
	
	// ���Ŀ��λ����Ʒ�İ�״̬��Ҫ���ڸ�λ���ϵ���Ʒ�İ�״̬��ͬ�����ܷ���
	if ((pItem2->bLock^pItem1->bLock) || 
		(pItem1->bLock && pItem2->bLock && (pItem1->dwUnlockTime!=pItem2->dwUnlockTime)) )
	{
		return GT_INVALID;
	}

	// 2a.��Ʒ���Բ�ͬ,���ƶ��󳬹��ѵ�����
	if(pItem1->dwTypeID != pItem2->dwTypeID
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem2->GetNum() + n16NumMove > pItem2->pProtoType->n16MaxLapNum
		|| pItem1->IsTimeLimit() || pItem2->IsTimeLimit())
	{
		return GT_INVALID;
	}

	// 2b.ͬ������Ʒ�����ƶ���Ŀ����ƷҲ����ﵽ�ѵ�����
	pItem1->AddNum(-n16NumMove);
	pItem2->AddNum(n16NumMove);

	// ����������Ʒ��Ϣ
	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.n16NumRes2 = pItem2->GetNum();
	ItemMove.bOverlap	= TRUE;

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ�ƶ�������������(�ֿ�ͱ�����)����ָ��Ŀ�������о���λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, 
							OUT tagItemMove &ItemMove, OUT INT16 &n16IndexDst)
{
	// ���Ŀ���������Ƿ��пռ�
	if(conDst.GetFreeSpaceSize() < 1)
	{
		return GT_INVALID;
	}
	
	// ��ԭ������ȡ�����ƶ���Ʒ
	tagItem *pItem = GetItem(n64SerialSrc);
	if(NULL == pItem)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pItem != NULL);
		return GT_INVALID;
	}

	if(!m_pRestrict->CanMoveToOther(pItem, &conDst))
	{
		return E_Item_CanNotMove_Other;
	}

	this->Remove(n64SerialSrc, FALSE, FALSE);
	
	// ����Ŀ��������
	return conDst.Add(pItem, n16IndexDst, ItemMove, FALSE, TRUE);
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ�ƶ�������������(�ֿ�ͱ�����)��ָ��Ŀ�걳���о���λ��
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, INT16 n16IndexDst, OUT tagItemMove &ItemMove)
{
	// 0.Ŀ��λ�úϷ��Լ��
	if(n16IndexDst < 0 || n16IndexDst >= conDst.GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	tagItem *pItem1, *pItem2;

	// ��ô��ƶ���Ʒָ��
	pItem1 = GetItem(n64SerialSrc);
	if(NULL == pItem1)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���		
		ASSERT(pItem1 != NULL);
		return GT_INVALID;
	}

	// ��ʼ����������
	ItemMove.pItem2 = NULL;
	ItemMove.n16NumRes2 = 0;
	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.bCreateItem = FALSE;
	ItemMove.bOverlap = FALSE;
	ItemMove.bChangePos = TRUE;

	if(!m_pRestrict->CanMoveToOther(pItem1, &conDst))
	{
		return E_Item_CanNotMove_Other;
	}

	// 1��Ŀ��λ��Ϊ��
	if(conDst.IsOnePlaceFree(n16IndexDst))
	{		
		this->Remove(n64SerialSrc);
		return conDst.Add(pItem1, n16IndexDst, TRUE);
	}

	// 2��Ŀ��λ�ò�Ϊ��
	pItem2 = conDst.GetItem(n16IndexDst);

	if( !P_VALID(pItem2) )
		return GT_INVALID;

	if( (P_VALID(pItem2->dwOwnerID) && P_VALID(pItem1->dwOwnerID)) && (pItem2->dwOwnerID != pItem1->dwOwnerID) )
		return E_Item_CanNotMove_Other;
	if( !(pItem1->IsBind() && pItem2->IsBind() ) )
		return E_Item_CanNotMove_Other;
	//if( P_VALID(pItem2->dwOwnerID) && P_VALID(pItem1->dwOwnerID) && pItem2->dwOwnerID != pItem1->dwOwnerID && ( pItem1->IsBind() || pItem2->IsBind() ) )
	//	return E_Item_CanNotMove_Other;

	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->GetNum();

	// 2a.��Ʒ���Բ�ͬ,��ĳһ����Ʒ�����ﵽ�˶ѵ�����
	if(pItem1->dwTypeID != pItem2->dwTypeID
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem1->GetNum() >= pItem1->pProtoType->n16MaxLapNum
		|| pItem2->GetNum() >= pItem2->pProtoType->n16MaxLapNum)
	{
		if(!/*conDst.*/GetRestrict()->CanMoveToOther(pItem2, /*this*/&conDst))
		{
			return E_Item_CanNotMove_Other;
		}
		
		// ��¼ԭ��λ��
		INT16 n16IndexSrc = pItem1->n16Index;

		// �������������Ʒ
		Container::Remove(n16IndexSrc);
		((Container *)&conDst)->Remove(n16IndexDst);

		// �Ż�
		this->Add(pItem2, n16IndexSrc, TRUE);
		conDst.Add(pItem1, n16IndexDst, TRUE);

		return E_Success;
	}

	// 2b.ͬ������Ʒ�������Ѿ�û�дﵽ�ѵ�����
	INT32 nNum = pItem2->pProtoType->n16MaxLapNum - pItem2->GetNum();
	nNum = min(nNum, pItem1->GetNum());
	pItem1->AddNum(-nNum);
	pItem2->AddNum(nNum);

	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.n16NumRes2 = pItem2->GetNum();
	ItemMove.bOverlap	= TRUE;
	ItemMove.bChangePos = FALSE;

	// �������ݿⱣ��״̬
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	// ��Ʒ1ȫ���ƶ�����Ʒ2��
	if(0 == pItem1->GetNum())
	{
		this->Remove(pItem1->n64Serial, TRUE);
	}

	return E_Success;
}

//****************************************** EquipContainer ***********************************************

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
EquipContainer::EquipContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize)
								: Container(eConType, n16CurSize, n16MaxSize), m_TimeLimitMgr(ITEM_UPDATE_TIME)
{
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
EquipContainer::~EquipContainer()
{
}

//-------------------------------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Add(tagEquip *pEquip, EEquipPos ePos)
{
	// ����λ
	if(!IsOnePlaceFree(ePos))
	{
		return E_Item_Place_NotFree;
	}
	
	// Ϊ�˱�����ܳ��ֵĴ�λ����ʱװ��λ�ý��м��
	if (ePos >= EEP_Equip_End)
	{
		if(P_VALID(pEquip->pEquipProto) && pEquip->pEquipProto->eEquipPos != ePos)
		{
			return E_Item_EquipBar_Error;
		}
	}

	// ����������
	if(Container::Add(pEquip, ePos) < 1)
	{
		return E_Equip_OnFailed;
	}

	//// �������ݿⱣ��״̬
	//pEquip->SetUpdate(EUDBS_Update);

	// �Ƿ�Ϊʱ����Ʒ
	if(pEquip->IsTimeLimit())
	{
		m_TimeLimitMgr.Add2TimeLeftList(pEquip->n64Serial, pEquip->pProtoType->dwTimeLimit, pEquip->dw1stGainTime);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ƶ�(����������ָλ)
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::MoveTo(INT64 n64SerialSrc, EEquipPos ePosDst)
{
	// ���Ŀ��λ���Ƿ�Ϊ��ָλ
	if(ePosDst != EEP_Finger1 && ePosDst != EEP_Finger2)
	{
		return E_Equip_NotRingPos;
	}

	// ��ȡ���ƶ�װ��
	tagEquip *pEquip = GetItem(n64SerialSrc);
	if(NULL == pEquip)
	{
		ASSERT(pEquip != NULL);
		return GT_INVALID;
	}

	// ���ƶ�װ���Ƿ�Ϊring
	if(!MIsRing(pEquip))
	{
		return E_Equip_NotRing;
	}

	// �Ƿ�����Ŀ��λ��
	if(pEquip->n16Index == ePosDst)
	{
		return E_Equip_SamePos;
	}

	// �ƶ�
	if(IsOnePlaceFree(ePosDst))	// Ŀ��λ��Ϊ��
	{
		Container::Remove(n64SerialSrc);
		return this->Add(pEquip, ePosDst);
	}
	else // Ŀ��λ����װ��(ring)
	{
		Container::Swap(pEquip->n16Index, ePosDst);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����װ��
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Equip(ItemContainer &bagSrc, INT64 n64SerialSrc, EEquipPos ePosDst)
{
	// ȡ�ô���Ʒָ��
	tagItem *pItemNew = bagSrc.GetItem(n64SerialSrc);
	if(NULL == pItemNew)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pItemNew != NULL);
		return E_Item_NotFound;
	}

	// �ж��Ƿ�Ϊװ��
	if(!MIsEquipment(pItemNew->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	tagEquip *pEquipNew = (tagEquip *)pItemNew;

	// �õ���װ��λ��
	EEquipPos ePos = (pEquipNew->pEquipProto)->eEquipPos;

	// ����Ƿ����װ����Ŀ��λ��(ringҪ�������ж�)
	// if(!(ePos == ePosDst || (ePos + 1 == ePosDst && MIsRing(pEquipNew))))
	if(ePos != ePosDst && (ePos + 1 != ePosDst || (!MIsRing(pEquipNew) && !MIsWeapon(pEquipNew))))
	{
		return E_Equip_InvalidPos;
	}


	INT16 n16IndexBag = pEquipNew->n16Index;

	// Ŀ��λ��Ϊ��
	if(!IsOnePlaceFree(ePosDst))
	{
		// �ӱ�����ȡ��װ��
		((Container *)&bagSrc)->Remove(n16IndexBag);
		// ��װ������ȡ��װ��
		tagEquip *pEquipOld = Container::Remove((INT16)ePosDst);
		// ��ԭװ�����е�װ�����뱳����
		bagSrc.Add(pEquipOld, n16IndexBag, FALSE);
	}
	else
	{
		// �ӱ�����ȡ��װ��
		bagSrc.Remove(n64SerialSrc);
	}

	// ��ԭ��������Ʒװ����
	return this->Add(pEquipNew, ePosDst);
}

//-------------------------------------------------------------------------------------------------------
// ����װ��
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Unequip(INT64 n64SerialSrc, ItemContainer &bagDst)
{
	//// ��鱳�����Ƿ��пռ�
	//if(bagDst.GetFreeSpaceSize() < 1)
	//{
	//	return E_Bag_NotEnoughSpace;
	//}

	//// ����װ��
	//tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	//if(NULL == pEquipSrc)
	//{
	//	// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
	//	ASSERT(pEquipSrc != NULL);
	//	return E_Item_NotFound;
	//}

	//// ���뱳����
	//bagDst.Add(pEquipSrc, bagDst.GetOneFreeSpace(), FALSE);

	//return E_Success;

	return Unequip(n64SerialSrc, bagDst, bagDst.GetOneFreeSpace());
}

//-------------------------------------------------------------------------------------------------------
// ����װ��(ָ��������λ��)
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Unequip(INT64 n64SerialSrc, ItemContainer &bagDst, INT16 n16IndexDst)
{
	// ���Ŀ��λ���Ƿ�Ϸ�
	if(n16IndexDst < 0 || n16IndexDst > bagDst.GetCurSpaceSize() - 1)
	{
		return GT_INVALID;
	}
	
	// ��鱳����ָ��λ���Ƿ�����Ʒ�������ߴ���װ������
	if(!bagDst.IsOnePlaceFree(n16IndexDst))
	{
		tagEquip *pEquip = GetItem(n64SerialSrc);
		if(NULL == pEquip)
		{
			// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
			ASSERT(pEquip != NULL);
			return E_Item_NotFound;
		}
		
		return Equip(bagDst, bagDst.GetItem(n16IndexDst)->n64Serial, (EEquipPos)pEquip->n16Index);
	}

	// ����װ��
	tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	if(NULL == pEquipSrc)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pEquipSrc != NULL);
		return E_Item_NotFound;
	}

	// �Ƿ�Ϊʱ����Ʒ
	if(pEquipSrc->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pEquipSrc->n64Serial);
	}

	// ���뱳����
	bagDst.Add(pEquipSrc, n16IndexDst, FALSE);

	return E_Success;
}

tagEquip* EquipContainer::GetEquipBarEquip(DWORD dwTypeID)
{
	tagEquip* pEquip = NULL;
	TMap<INT64, INT16>::TMapIterator iterator = Begin();
	while (GetNextItem(iterator, pEquip))
	{
		if (P_VALID(pEquip) && (pEquip->dwTypeID == dwTypeID))
		{
			return pEquip;
		}
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------------
// �����ֽ���
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::SwapWeapon()
{
	tagEquip *pEquipRight	= GetItem((INT16)EEP_RightHand);
	tagEquip *pEquipLeft	= GetItem((INT16)EEP_LeftHand);

	if(P_VALID(pEquipRight))
	{
		Container::Remove((INT16)EEP_RightHand);
	}

	if(P_VALID(pEquipLeft))
	{
		Container::Remove((INT16)EEP_LeftHand);
	}

	if(P_VALID(pEquipRight))
	{
		Add(pEquipRight, EEP_LeftHand);
	}

	if(P_VALID(pEquipLeft))
	{
		Add(pEquipLeft, EEP_RightHand);
	}

	return E_Success;
}

VOID ItemContainer::Add2TimeLeftList(INT64 n64Serial, DWORD dwTimeLimit, tagDWORDTime dwSrcTime)
{
	m_TimeLimitMgr.Add2TimeLeftList(n64Serial, dwTimeLimit, dwSrcTime);
}

// �Ƿ�����ƶ�ʱװ(ͬһ������)
DWORD WardrobeContainer::CanMove(INT64 n64Serial, INT16 n16TargetIndex)
{
	if (!g_worldSession.IsWardrobeOn())
		return GT_INVALID;

	// ����ƶ�Ŀ��
	tagItem *pItem = GetItem(n64Serial);

	return CanAddNormalCheck(pItem, n16TargetIndex);
}

// �Ƿ��������������ӵ���
DWORD WardrobeContainer::CanAdd2Wardrobe(INT64 n64Serial, ItemContainer& RoleBag, INT16 &n16DstIndex)
{
 	if (!g_worldSession.IsWardrobeOn())
 		return GT_INVALID;

	// ��ԭ������ȡ�����ƶ���Ʒ
	tagItem *pItem = RoleBag.GetItem(n64Serial);

	return CanAdd2Wardrobe(pItem, n16DstIndex);
}
DWORD WardrobeContainer::CanAdd2Wardrobe(tagItem *pItem, INT16 &n16DstIndex)
{
 	if (!g_worldSession.IsWardrobeOn())
 		return GT_INVALID;

	// ��������������Ƿ��ʱװ���Ƿ��ʱ����
	if (!CanAddItem(pItem))
		return GT_INVALID;
	
	// ����Ƿ����Ҽ��Զ�����
 	if (n16DstIndex == GT_INVALID)
 		n16DstIndex = GetOneFreeSpace(pItem->dwTypeID);

	if (!IsOnePlaceFree(n16DstIndex))
		return GT_INVALID;

	return CanAddNormalCheck(pItem, n16DstIndex);
}

// ���һ����λ��ʧ�ܷ���GT_INVALID������ǰ��ȷ������Ʒ��ʱװ�����Ƿ�ʱ���Ƶ�
INT16 WardrobeContainer::GetOneFreeSpace(DWORD dwTypeID)
{
	TMap<DWORD, tagWardrobeProto*>& mapWardrobeProto = g_attRes.GetWardrobeProtoMap();
	TMap<DWORD, tagWardrobeProto*>::TMapIterator iterator = mapWardrobeProto.Begin();
	tagWardrobeProto* pWardrobeProto = NULL;
	while (mapWardrobeProto.PeekNext(iterator, pWardrobeProto))
	{
		if (!P_VALID(pWardrobeProto))
			continue;
		for (int nCnt = 0; nCnt < MAX_FASHION_PERLAYER; nCnt++)
		{
			if (pWardrobeProto->dwFashionID[nCnt] == dwTypeID)
				return (MAX_FASHION_PERLAYER*(pWardrobeProto->dwLayerID-1)+nCnt);
		}
	}
	return GT_INVALID;
}

BOOL WardrobeContainer::CanAddItem(tagItem* pItem)
{
	// ���¸�ֻ�ܷ�ʱװ
	if (P_VALID(pItem) && P_VALID(pItem->pProtoType)
		&& pItem->pProtoType->eType == EIT_Fashion
		&& pItem->IsTimeLimit() == FALSE)
		return TRUE;
	else
		return FALSE;
}

DWORD WardrobeContainer::CanAddNormalCheck( tagItem * pItem, INT16 n16TargetIndex )
{
	if(!P_VALID(pItem))
		return GT_INVALID;

	// Ŀ��λ�úϷ��Լ��
	if(	n16TargetIndex < 0 ||	n16TargetIndex >= GetCurSpaceSize())
		return GT_INVALID;

	// ����ǲ���װ��
	if (!MIsEquipment(pItem->dwTypeID))
		return GT_INVALID;

	// �������ʱװ��λ���Ƿ��Ӧ
	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
		return GT_INVALID;
	if (!P_VALID(pEquip->pEquipProto))
		return GT_INVALID;

	// ����Ƿ���װ����ͬһλ�õ�ʱװ
	DWORD dwLayer = CalLayerID(n16TargetIndex);
	DWORD dwPos = (DWORD)(n16TargetIndex%MAX_FASHION_PERLAYER);
	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(dwLayer);
	if (!P_VALID(pWardrobeProto) || dwPos > MAX_FASHION_PERLAYER)
		return GT_INVALID;
	if (pWardrobeProto->eLayerType == EWLT_Diy)
	{
		EWardrobeFashionPos eWardrobeFashionPos = EquipPos2WardrobeFashionPos(pEquip->pEquipProto->eEquipPos);
		if (eWardrobeFashionPos == EWFP_Null || dwPos != eWardrobeFashionPos)
			return GT_INVALID;
	}
	else if (pWardrobeProto->eLayerType == EWLT_Normal)
	{
		if (pWardrobeProto->dwFashionID[dwPos] != pItem->dwTypeID)
			return GT_INVALID;
	}
	else
	{
		return GT_INVALID;
	}
	return E_Success;
}

//---------------------------------------------------------------
//����װ�����������κ�������
DWORD EquipContainer::Unequip( INT64 n64SerialSrc )
{
	// ��鱳����ָ��λ���Ƿ�����Ʒ�������ߴ���װ������
	//if(!bagDst.IsOnePlaceFree(n16IndexDst))
	//{
	//	tagEquip *pEquip = GetItem(n64SerialSrc);
	//	if(NULL == pEquip)
	//	{
	//		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
	//		ASSERT(pEquip != NULL);
	//		return E_Item_NotFound;
	//	}
	//	
	//	return Equip(bagDst, bagDst.GetItem(n16IndexDst)->n64Serial, (EEquipPos)pEquip->n16Index);
	//}

	// ����װ��
	tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	if(NULL == pEquipSrc)
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pEquipSrc != NULL);
		return GT_INVALID;
	}

	// �Ƿ�Ϊʱ����Ʒ
	if(pEquipSrc->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pEquipSrc->n64Serial);
	}

	// ���뱳����
	//bagDst.Add(pEquipSrc, n16IndexDst, FALSE);

	return E_Success;
}

//---------------------------------------------------------------
//����װ���Ӽ���ע��ķ�����
DWORD EquipContainer::EquipByFamily(tagItem *pItemNew, EEquipPos ePosDst)
{
	// ȡ�ô���Ʒָ��
	
	if (!P_VALID(pItemNew))
	{
		// ���翨ʱ,�ͻ��˷��ظ�����Ϣ,����ִ�е���
		ASSERT(pItemNew != NULL);
		return E_Item_NotFound;
	}

	// �ж��Ƿ�Ϊװ��
	if(!MIsEquipment(pItemNew->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	tagEquip *pEquipNew = (tagEquip *)pItemNew;

	// �õ���װ��λ��
	EEquipPos ePos = (pEquipNew->pEquipProto)->eEquipPos;
	
	// ����Ƿ����װ����Ŀ��λ��(ringҪ�������ж�)
	// if(!(ePos == ePosDst || (ePos + 1 == ePosDst && MIsRing(pEquipNew))))
	if(ePos != ePosDst && (ePos + 1 != ePosDst || (!MIsRing(pEquipNew) && !MIsWeapon(pEquipNew))))
	{
		return E_Equip_InvalidPos;
	}

	//INT16 n16IndexBag = pEquipNew->n16Index;

	//// Ŀ��λ��Ϊ��
	//if(!IsOnePlaceFree(ePosDst))
	//{
	//	// �ӱ�����ȡ��װ��
	//	((Container *)&bagSrc)->Remove(n16IndexBag);
	//	// ��װ������ȡ��װ��
	//	tagEquip *pEquipOld = Container::Remove((INT16)ePosDst);
	//	// ��ԭװ�����е�װ�����뱳����
	//	bagSrc.Add(pEquipOld, n16IndexBag, FALSE);
	//}
	//else
	//{
	//	// �ӱ�����ȡ��װ��
	//	bagSrc.Remove(n64SerialSrc);
	//}

	// ��ԭ��������Ʒװ����
	return this->Add(pEquipNew, ePosDst);
}

BOOL ItemContainer::OverLappedItemHandler(tagItem* pItem)
{
	INT16 nSpace = GetOneFreeSpace();
	if (GT_INVALID != nSpace )
	{
		Add(pItem, nSpace, FALSE, FALSE);
		return TRUE;
	}

	return FALSE;
}





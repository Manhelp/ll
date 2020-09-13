//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_creator.cpp
// author: Sxg
// actor:
// data: 2008-07-29
// last:
// brief: ��Ʒ&װ�������� -- ��Ʒ&װ������,����������(��Ʒ����������)
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"


#include "att_res.h"
#include "item_creator.h"
#include "world.h"
#include "script_mgr.h"
#include "..\ServerDefine\consolidate_define.h"
#include "role.h"
#include "role_mgr.h"


INT64			ItemCreator::m_n64MaxSerial = 0;
INT64			ItemCreator::m_n64MinSerial = 0;
Mutex			ItemCreator::m_Mutex;

#pragma warning(disable:4244)

//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
ItemCreator::ItemCreator()
{}

ItemCreator::~ItemCreator()
{}

//-------------------------------------------------------------------------------------------------------
// ����������Ʒ�����µĶ���Ʒ
//-------------------------------------------------------------------------------------------------------
tagItem* ItemCreator::Create(const tagItem &item, INT16 n16Num)
{
	if(n16Num > item.pProtoType->n16MaxLapNum)
	{
		ASSERT(n16Num <= item.pProtoType->n16MaxLapNum);
		return NULL;
	}
	
	tagItem *pNewItem;
	if(MIsEquipment(item.dwTypeID))
	{
		// Jason 2010-4-13 v2.0.0
		if( MIsFaBaoType(item.pProtoType->eType) )
		{
			if( !g_world.IsFabaoOpen() )
				return NULL;
			pNewItem = new tagFabao;
			IFASTCODE->MemCpy(pNewItem, &item, SIZE_FABAO);
			// ��ʱ
			tagFabao * pFabao = (tagFabao*)pNewItem;
			const tagConsolidateItem* p = NULL;
			INT size_n = min(pFabao->nSlotNum,MaxWuxingSlotNum);
			pFabao->bySlotLingzhuMatch = 0;
			for(int i = 0; i < size_n; ++i)
			{
				pFabao->bySlotMacthNdx[i] = 0;
				if( P_VALID(pFabao->dwWuxingBeadID[i]) )
				{
					p = g_attRes.GetConsolidateProto(pFabao->dwWuxingBeadID[i]);
					if( P_VALID(p) )
					{
						if( p->eWuXing == pFabao->eWuxingHole[i] )
							pFabao->bySlotMacthNdx[i] = 1;
					}
				}
				if( pFabao->bySlotMacthNdx[i] )
					pFabao->bySlotLingzhuMatch++;
			}
		}
		else if (MIsSoulCrystalType(item.pProtoType->eType))
		{
			pNewItem = new tagSoulCrystal;
			IFASTCODE->MemCpy(pNewItem, &item, SIZE_HUNJING);
		}
		else if(MIsHolyType(item.pProtoType->eType))
		{
			pNewItem = new tagHolyMan;
			IFASTCODE->MemCpy(pNewItem, &item, SIZE_HOLY);
			//pNewItem->HolySpec
		}
		else if(MIsHolyEquipType(item.pProtoType->eType))
		{
			pNewItem = new tagHolyEquip;
			IFASTCODE->MemCpy(pNewItem, &item, SIZE_HOLYEQUIP);
			//pNewItem->HolySpec
		}
		else
		{
			pNewItem = new tagEquip;
			IFASTCODE->MemCpy(pNewItem, &item, SIZE_EQUIP);
		}
	}
	else
	{
		pNewItem = new tagItem;
		IFASTCODE->MemCpy(pNewItem, &item, SIZE_ITEM);
	}

	CreateItemSerial(pNewItem->n64Serial);
	pNewItem->SetNum(n16Num);

	return pNewItem;
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ&װ��
// Jason 2009-12-1 ������ɫƷ��Ӱ�죬װ��Ǳ��ֵ�ܲ���Ӱ��
//-------------------------------------------------------------------------------------------------------
tagItem* ItemCreator::Create(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID,
							 INT nNum/* = 1*/, DWORD dwCreator/* = GT_INVALID*/,
							 INT16 n16QltyModPct/* = 0*/, INT16 n16QltyModPctEx/* = 0*/, INT16 n16PotValPct/* = 10000*/, INT nPotAdding,INT qlty, BOOL bFixQlty /*= TRUE*/)
{
	if(nNum <= 0)
	{
		ASSERT(nNum > 0);
		return NULL;
	}

	if (dwTypeID == 0)
	{
		ILOG->Write(_T("Create Invalid tupeId, create mode : %d, createId : %d\n"), eCreateMode, dwCreateID);
	}
	
	tagItem *pRetItem;
	if(MIsEquipment(dwTypeID))
	{
		// Jason 2010-4-13 v2.0.0
		tagEquipProto * pEquipProto = g_attRes.GetEquipProto(dwTypeID);
		if( !P_VALID(pEquipProto) )
			return NULL;

		// ����װ����type��ͬ��������ͬ�Ľṹ��
		switch (pEquipProto->eType)
		{
		case EIT_FaBao:
			{
				if( !g_world.IsFabaoOpen() )
					return NULL;
				pRetItem = new tagFabao;
				break;
			}
		case EIT_SoulCrystal:
			{
				pRetItem = new tagSoulCrystal;
				break;			
			}
		case EIT_Holy:
			{
				pRetItem = new tagHolyMan;
				break;			
			}
		case EIT_HolyEquip:
			{
				pRetItem = new tagHolyEquip;
				break;			
			}
		default:
			pRetItem = new tagEquip;
			break;
		}

		//if( MIsFaBaoType(pEquipProto->eType) )
		//{
		//	if( !g_world.IsFabaoOpen() )
		//		return NULL;
		//	pRetItem = new tagFabao;
		//}
		//else if( MIsSoulCrystalType(pEquipProto->eType))
		//{
		//	pRetItem = new tagSoulCrystal;
		//}
		//else
		//	pRetItem = new tagEquip;

		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		InitEquipSpec(((tagEquip*)pRetItem)->equipSpec, n16QltyModPct, n16QltyModPctEx, n16PotValPct);

		// ����װ����type��ͬ�����ز�ͬ��proto
		switch (pEquipProto->eType)
		{
		case EIT_FaBao:
			{
				pRetItem->pProtoType = g_attRes.GetFabaoProto(dwTypeID);
				break;
			}
		case EIT_Holy:
			{
				pRetItem->pProtoType = g_attRes.GetHolyProto(dwTypeID);
				break;			
			}
		case EIT_HolyEquip:
			{
				pRetItem->pProtoType = g_attRes.GetHolyEquipProto(dwTypeID);
				break;			
			}			
		default:
			pRetItem->pProtoType = g_attRes.GetEquipProto(dwTypeID);
			break;
		}
		//// Jason 2010-4-13 v2.0.0
		//if( MIsFaBaoType(pEquipProto->eType) )
		//	pRetItem->pProtoType = g_attRes.GetFabaoProto(dwTypeID);
		//else
		//	pRetItem->pProtoType = g_attRes.GetEquipProto(dwTypeID);
	}
	else
	{		
		pRetItem = new tagItem;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		pRetItem->pProtoType = g_attRes.GetItemProto(dwTypeID);
	}

	if(!P_VALID(pRetItem->pProtoType)/* || pRetItem->pProtoType->n16MaxLapNum < nNum*/)
	{
		MAttResCaution(_T("item or equip proto"), _T("typeid"), dwTypeID);
		ASSERT(P_VALID(pRetItem->pProtoType));
		ASSERT(P_VALID(pRetItem->pProtoType) && pRetItem->pProtoType->n16MaxLapNum >= nNum);
		::Destroy(pRetItem);
		return NULL;
	}
	if( pRetItem->pProtoType->n16MaxLapNum < nNum )
		nNum = pRetItem->pProtoType->n16MaxLapNum;

	INT64 n64Serial;
	CreateItemSerial(n64Serial);

	InitItem(*pRetItem, eCreateMode, pRetItem->pProtoType, dwCreateID, n64Serial, nNum, dwCreator, g_world.GetWorldTime(), GT_INVALID, GT_INVALID);

	// װ��
	if(MIsEquipment(pRetItem->dwTypeID))
	{
		// Jason 2009-12-1 װ��Ǳ��ֵ��������
		((tagEquip*)pRetItem)->equipSpec.nPotIncTimes	= ((tagEquip*)pRetItem)->pEquipProto->nPotIncTimes + nPotAdding;	

		// ����Ƿ�Ϊ����������ɺ�ʱ����
		//if( (pRetItem->pEquipProto->bIdentifyLoot && EICM_Loot == eCreateMode)
		//	|| (pRetItem->pEquipProto->bIdentifyProduct && EICM_Product == eCreateMode)
		//	|| (EICM_LessingLoong == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) 
		//	|| (EICM_GoldStone == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) )
		if( qlty > EIQ_Null && qlty < EIQ_End )
		{
			ItemCreator::IdentifyEquip((tagEquip*)pRetItem,(EItemQuality)qlty, 0, bFixQlty);
		}
		else
			//if( (EICM_Loot == eCreateMode)
			//	|| ( EICM_Product == eCreateMode)
			//	|| (EICM_LessingLoong == eCreateMode  ) 
			//	|| (EICM_GoldStone == eCreateMode)
			//	/*|| (EICM_Quest == eCreateMode) */
			//	|| EICM_PetSkill == eCreateMode )
			ItemCreator::IdentifyEquip((tagEquip*)pRetItem, EIQ_Null, 0, bFixQlty);
	}

	const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
	Role* pRole = g_roleMgr.GetRolePtrByID( dwCreator );

	if(P_VALID(pScript) && P_VALID(pRole) && P_VALID(pRole->GetMap()) && P_VALID(pRetItem->pProtoType) )
	{
		if( P_VALID(pRole) && P_VALID(pRole->GetMap()) )
			pScript->CreateItem( pRole->GetMap()->GetMapID(), pRole->GetMap()->GetInstanceID(), pRetItem->dwTypeID, pRetItem->pProtoType->byQuality );
		else
			pScript->CreateItem( 0, 0, pRetItem->dwTypeID, pRetItem->pProtoType->byQuality );
	}

	if (MIsSoulCrystalType(pRetItem->pProtoType->eType) && P_VALID(pScript))
	{
		pScript->GetSoulCrystalQlty(pRetItem->dwTypeID, ((tagSoulCrystal*)pRetItem)->SoulSpec.bySoulCrystalQlty);
		((tagSoulCrystal*)pRetItem)->SoulSpec.dwStaticSkillID = pScript->GetSoulCrystalSkillID(pRetItem->dwTypeID, ((tagSoulCrystal*)pRetItem)->SoulSpec.bySoulCrystalQlty);
		pScript->GetSoulCrystalAtt(dwCreateID, pRetItem->dwTypeID, ((tagSoulCrystal*)pRetItem)->SoulSpec.bySoulCrystalQlty, ((tagSoulCrystal*)pRetItem)->SoulSpec.nSoulAttID);
	}

	return pRetItem;
}
// Jason 2009-12-1 ������ɫƷ���������
tagItem* ItemCreator::CreateByProduce(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, EItemQuality eQlty,INT nPotAdding,DWORD dwPurpleQltyIdfPct, 
							 INT16 n16Num/* = 1*/, DWORD dwCreator/* = GT_INVALID*/,
							 INT16 n16QltyModPct/* = 0*/, INT16 n16QltyModPctEx/* = 0*/, INT16 n16PotValPct/* = 10000*/,INT qlty)
{
	if(n16Num <= 0)
	{
		ASSERT(n16Num > 0);
		return NULL;
	}

	if (dwTypeID == 0)
	{
		ILOG->Write(_T("CreateByProduce Invalid tupeId, create mode : %d, createId : %d\n"), eCreateMode, dwCreateID);
	}

	tagItem *pRetItem;
	if(MIsEquipment(dwTypeID))
	{
		// Jason 2010-4-13 v2.0.0
		tagEquipProto * pEquipProto = g_attRes.GetEquipProto(dwTypeID);
		if( !P_VALID(pEquipProto) )
			return NULL;

		if( MIsFaBaoType(pEquipProto->eType) )
		{
			if( !g_world.IsFabaoOpen() )
				return NULL;
			pRetItem = new tagFabao;
		}
		else if (MIsSoulCrystalType(pEquipProto->eType))
		{
			pRetItem = new tagSoulCrystal;
		}
		else if (MIsHolyType(pEquipProto->eType))
		{
			pRetItem = new tagHolyMan;
		}
		else if (MIsHolyEquipType(pEquipProto->eType))
		{
			pRetItem = new tagHolyEquip;
		}
		else
			pRetItem = new tagEquip;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		InitEquipSpec(((tagEquip*)pRetItem)->equipSpec, n16QltyModPct, n16QltyModPctEx, n16PotValPct);
		// Jason 2010-4-13 v2.0.0
		if( MIsFaBaoType(pEquipProto->eType) )
			pRetItem->pProtoType = g_attRes.GetFabaoProto(dwTypeID);
		else if (MIsHolyType(pEquipProto->eType))
		{
			pRetItem->pProtoType = g_attRes.GetHolyProto(dwTypeID);
		}
		else if (MIsHolyEquipType(pEquipProto->eType))
		{
			pRetItem->pProtoType = g_attRes.GetHolyEquipProto(dwTypeID);
		}
		else
			pRetItem->pProtoType = g_attRes.GetEquipProto(dwTypeID);
	}
	else
	{		
		pRetItem = new tagItem;
		if(NULL == pRetItem)
		{
			ASSERT(pRetItem != NULL);
			return NULL;
		}

		pRetItem->pProtoType = g_attRes.GetItemProto(dwTypeID);
	}

	if(!P_VALID(pRetItem->pProtoType) || pRetItem->pProtoType->n16MaxLapNum < n16Num)
	{
		MAttResCaution(_T("item or equip proto"), _T("typeid"), dwTypeID);
		ASSERT(P_VALID(pRetItem->pProtoType));
		ASSERT(P_VALID(pRetItem->pProtoType) && pRetItem->pProtoType->n16MaxLapNum >= n16Num);
		::Destroy(pRetItem);
		return NULL;
	}

	INT64 n64Serial;
	CreateItemSerial(n64Serial);

	InitItem(*pRetItem, eCreateMode, pRetItem->pProtoType, dwCreateID, n64Serial, n16Num, dwCreator, g_world.GetWorldTime(), GT_INVALID, GT_INVALID);

	// װ��
	if(MIsEquipment(pRetItem->dwTypeID))
	{
		// Jason 2009-12-1 װ��Ǳ��ֵ��������
		((tagEquip*)pRetItem)->equipSpec.nPotIncTimes	= ((tagEquip*)pRetItem)->pEquipProto->nPotIncTimes + nPotAdding;
		((tagEquip*)pRetItem)->equipSpec.dwPurpleQltyIdfPct = dwPurpleQltyIdfPct;

		// ����Ƿ�Ϊ����������ɺ�ʱ����
		//if((pRetItem->pEquipProto->bIdentifyLoot && EICM_Loot == eCreateMode)
		//	|| (pRetItem->pEquipProto->bIdentifyProduct && EICM_Product == eCreateMode)
		//	|| (EICM_LessingLoong == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) 
		//	|| (EICM_GoldStone == eCreateMode && pRetItem->pEquipProto->bIdentifyProduct ) )
		if( qlty > EIQ_Null && qlty < EIQ_End )
		{
			ItemCreator::IdentifyEquip((tagEquip*)pRetItem,eQlty);
		}
		else
			ItemCreator::IdentifyEquip((tagEquip*)pRetItem);
	}

	const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
	Role* pRole = g_roleMgr.GetRolePtrByID( dwCreator );

	if(P_VALID(pScript) && P_VALID(pRole) && P_VALID(pRole->GetMap()) && P_VALID(pRetItem->pProtoType) )
	{
		if( P_VALID(pRole) && P_VALID(pRole->GetMap()) )
			pScript->CreateItem( pRole->GetMap()->GetMapID(), pRole->GetMap()->GetInstanceID(), pRetItem->dwTypeID, pRetItem->pProtoType->byQuality );
		else
			pScript->CreateItem( 0, 0, pRetItem->dwTypeID, pRetItem->pProtoType->byQuality );
	}

	return pRetItem;
}


//-------------------------------------------------------------------------------------------------------
// �������ݿ��ȡ�����ݴ�����Ʒ
//-------------------------------------------------------------------------------------------------------
tagItem* ItemCreator::CreateItemByData( PVOID pData )
{
	MTRANS_POINTER(pItem, pData, tagItem);
	tagItem* pNewItem = new tagItem;

	IFASTCODE->MemCpy(pNewItem, pItem, sizeof(tagItem));
	pNewItem->pProtoType = g_attRes.GetItemProto(pItem->dwTypeID);

	if (P_VALID(pNewItem->pProtoType) && IsGMItemNoInit(pNewItem))
	{
		InitItem(*pNewItem, pItem->eCreateMode, pNewItem->pProtoType, pNewItem->dwCreateID, pNewItem->n64Serial, pNewItem->GetNum(), pNewItem->dwCreatorID, pNewItem->dwCreateTime, pNewItem->dwAccountID, pNewItem->dwOwnerID);
	}

	return pNewItem;
}

//-------------------------------------------------------------------------------------------------------
// �������ݿ��ȡ�����ݴ���װ��
//-------------------------------------------------------------------------------------------------------
tagEquip* ItemCreator::CreateEquipByData( PVOID pData )
{
	MTRANS_POINTER(pEquip, pData, tagEquip);
	tagEquip* pNewEquip = new tagEquip;

	IFASTCODE->MemCpy(pNewEquip, pEquip, sizeof(tagEquip));
	pNewEquip->pProtoType = g_attRes.GetEquipProto(pEquip->dwTypeID);
	// Jason 2010-4-13 v2.0.0
	if( MIsFaBao(pNewEquip) )
	{
		delete pNewEquip;
		pNewEquip = new tagFabao;
		pEquip = (tagFabao*)pData;
		IFASTCODE->MemCpy(pNewEquip, pEquip, SIZE_FABAO);
		pNewEquip->pProtoType = g_attRes.GetFabaoProto(pNewEquip->dwTypeID);
		// ��ʱ
		tagFabao * pFabao = (tagFabao*)pNewEquip;
		const tagConsolidateItem* p = NULL;
		INT size_n = min(pFabao->nSlotNum,MaxWuxingSlotNum);
		pFabao->bySlotLingzhuMatch = 0;
		for(int i = 0; i < size_n; ++i)
		{
			pFabao->bySlotMacthNdx[i] = 0;
			if( P_VALID(pFabao->dwWuxingBeadID[i]) )
			{
				p = g_attRes.GetConsolidateProto(pFabao->dwWuxingBeadID[i]);
				if( P_VALID(p) )
				{
					if( p->eWuXing == pFabao->eWuxingHole[i] )
						pFabao->bySlotMacthNdx[i] = 1;
				}
			}
			if( pFabao->bySlotMacthNdx[i] )
				pFabao->bySlotLingzhuMatch++;
		}
	}
	else if (MIsSoulCrystal(pNewEquip))
	{
		delete pNewEquip;
		pNewEquip = new tagSoulCrystal;
		pEquip = (tagSoulCrystal*)pData;
		IFASTCODE->MemCpy(pNewEquip, pEquip, SIZE_HUNJING);
		pNewEquip->pProtoType = g_attRes.GetEquipProto(pEquip->dwTypeID);
	}
	else if (MIsHoly(pNewEquip))
	{
		delete pNewEquip;
		pNewEquip = new tagHolyMan;
		pEquip = (tagHolyMan*)pData;
		IFASTCODE->MemCpy(pNewEquip, pEquip, SIZE_HOLY);
		pNewEquip->pProtoType = g_attRes.GetHolyProto(pEquip->dwTypeID);
	}
	else if (MIsHolyEquip(pNewEquip))
	{
		delete pNewEquip;
		pNewEquip = new tagHolyEquip;
		pEquip = (tagHolyEquip*)pData;
		IFASTCODE->MemCpy(pNewEquip, pEquip, SIZE_HOLYEQUIP);
		pNewEquip->pProtoType = g_attRes.GetHolyEquipProto(pEquip->dwTypeID);
	}

	if(!P_VALID(pNewEquip->pProtoType )) 
	{
		delete pNewEquip;
		return NULL;
	}
	if (IsGMItemNoInit(pEquip))
	{
		InitItem(*pNewEquip, pNewEquip->eCreateMode, pNewEquip->pProtoType, pNewEquip->dwCreateID, pNewEquip->n64Serial, pNewEquip->GetNum(), pNewEquip->dwCreatorID, pNewEquip->dwCreateTime, pNewEquip->dwAccountID, pNewEquip->dwOwnerID);

		EItemQuality eQuality = (EItemQuality)pNewEquip->equipSpec.byQuality;
		InitEquipSpec(pNewEquip->equipSpec);
		IdentifyEquip(pNewEquip, eQuality);
	}

	return pNewEquip;
}

//-------------------------------------------------------------------------------------------------------
// ��������Ψһ��(ע��Ҫ����������)
//-------------------------------------------------------------------------------------------------------
VOID ItemCreator::CreateItemSerial(INT64 &n64NewSerial)
{
	m_Mutex.Acquire();
	n64NewSerial = ++m_n64MaxSerial;
	m_Mutex.Release();
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID ItemCreator::InitItem( tagItem &item, EItemCreateMode eCreateMode, const tagItemProto *pProto, DWORD dwCreateID, INT64 n64Serial, INT nNum, DWORD dwCreator, DWORD dwCreateTime, DWORD dwAccountID, DWORD dwOwnerID )
{
	ZeroMemory(&item, SIZE_ITEM);

	item.pProtoType		= pProto;
	
	item.n64Serial		= n64Serial;
	item.SetNum((INT16)nNum);
	item.dwTypeID		= pProto->dwTypeID;

	item.byBind			= EBS_Unknown;
	item.bLock			= FALSE;

	item.eCreateMode	= eCreateMode;
	item.dwCreateID		= dwCreateID;	// ���ɸ���Ʒ��ID,��: QuestID,GMID��
	
	item.dwCreatorID	= dwCreator;	/* ��ΪRoleID, ��ĳλ���������������Ʒ,��λ���ܱ�ʾΪ����Ҵ���;
											������Ʒͨ������ϵͳ����ʱ,���λͬdwCreateID*/
	item.dwNameID		= GT_INVALID;
	item.dwCreateTime	= dwCreateTime;

	item.dwOwnerID		= dwOwnerID;
	item.dwAccountID	= dwAccountID;
	item.SetConType(EICT_Null);
	item.n16Index		= GT_INVALID;

	item.eStatus		= EUDBS_Insert;
	item.dwSpecVal1	= 0;
	item.dwSpecVal2	= 0;

	item.pScript		= g_ScriptMgr.GetItemScript(pProto->dwTypeID);
	if(P_VALID(item.pProtoType) && item.pProtoType->eSpecFunc == EISF_FairyPowder)
	{
		if(item.pProtoType->nSpecFuncVal1 == GT_INVALID)
			item.dwSpecVal1	= IUTIL->RandomInRange(50,100);
		else
			item.dwSpecVal1	= item.pProtoType->nSpecFuncVal1;
	}
}

//-------------------------------------------------------------------------------------------------------
// ����δ����װ��(װ��ר�ò�������)
//-------------------------------------------------------------------------------------------------------
VOID ItemCreator::InitEquipSpec(tagEquipSpec &equipSpec, INT16 n16QltyModPct, INT16 n16QltyModPctEx, INT16 n16PotValPct)
{
	ZeroMemory(&equipSpec, SIZE_EQUIPSPEC);

	equipSpec.byQuality		= EIQ_Null;
	equipSpec.bCanCut		= TRUE;

	for(INT32 i=0; i<MAX_ROLEATT_POSY_EFFECT; ++i)
	{
		equipSpec.PosyEffect[i].eRoleAtt = ERA_Null;
	}
	//for(INT32 i = 0; i < MAX_ROLEATT_BASE_EFFECT; ++i)
	//{
	//	equipSpec.BaseEffect[i].eRoleAtt = ERA_Null;
	//}
	for( INT i = 0 ; i < MAX_FCONSOLIDATE_NUM; ++i )
		equipSpec.ConsolidateEffect[i].eRoleAtt = ERA_Null;

	equipSpec.dwLongHunInnerID	= GT_INVALID;
	equipSpec.dwLongHunOuterID	= GT_INVALID;

	equipSpec.bySpecAtt			= EESA_Null;
	equipSpec.n8ColorID			= EC_Null;

	equipSpec.n16QltyModPct		= n16QltyModPct;
	equipSpec.n16QltyModPctEx	= n16QltyModPctEx;

	equipSpec.n16PotValModPct	= n16PotValPct;
}

//-----------------------------------------------------------------------------
// ����װ��(û��Ʒ����Ʒ���Ƿ��������¼���Ʒ��)
//-----------------------------------------------------------------------------
VOID ItemCreator::IdentifyEquip(IN OUT tagEquip *pEquip, 
								EItemQuality eQlty/* = EIQ_Null*/, 
								const tagIMEffect *pIMEffect/* = NULL*/,
								BOOL bFixQlty/* = TRUE*/)
{
	ASSERT(P_VALID(pEquip));
	ASSERT(P_VALID(pEquip->pEquipProto));
	ASSERT(!MIsIdentified(pEquip));

	if( !P_VALID(pEquip) || !P_VALID(pEquip->pProtoType) )
		return;

	// ����Ƿ����Ѽ�����װ��
	//if(MIsIdentified(pEquip))
	//{
	//	return;
	//}

	if( MIsFaBao(pEquip) && !g_world.IsFabaoOpen() )
		return ;
	
	INT32 nQuality = eQlty;
	
	// û��Ʒ����Ʒ���Ƿ��������¼���Ʒ��
	//bool bPurpleQltyIdf = false;
	//if( pEquip->equipSpec.dwPurpleQltyIdfPct )
	//{
	//// �������Ӧ���Ǹ�bug����Ϊ������ʱ�Ѿ��������ɸѡ�ˣ���ˣ�ֻҪ���ֵ�к�������ݣ���һ������װ�����������޸ģ������������治�޸�
	//
	//	if( P_VALID(pEquip->equipSpec.dwPurpleQltyIdfPct) )
	// 		bPurpleQltyIdf = true;
	// 
	//	//FLOAT fRnd = ( (FLOAT) IUTIL->RandomInRange(0,100) ) / 100;
	//	//FLOAT fPct = (FLOAT)pEquip->equipSpec.dwPurpleQltyIdfPct / 100;
	//	//if( fRnd <= fPct )//�ɹ�
	//	//	bPurpleQltyIdf = true;
	//}
	
	if( eQlty <= EIQ_Start || eQlty >= EIQ_End ) 
	{
		//if( !bPurpleQltyIdf ) //��ɫ֮��ģ�����ԭƷ�����ɷ�ʽ
		//{
			// Jason 2010-4-14 v2.0.0����
			if( !MIsFaBao(pEquip) )
			{
				// ����װ��Ʒ�����ɼ�������Ʒ��
				nQuality = GenBaseEquipQlty(pEquip->dwTypeID);

				if (bFixQlty)
				{
					// ��������Ʒ��������������Ʒ��
					nQuality = ModEquipQltyByProduce(pEquip, nQuality);
				}
			}
		//}
		//else
		//	nQuality = EIQ_Purple;
	}
    
   
    
	// ��ʼ��Ʒ��
	if( pEquip->equipSpec.byQuality  != nQuality )
		pEquip->equipSpec.byQuality = (BYTE)nQuality;

	// ��ʼ���ȼ�����
	pEquip->equipSpec.byMinUseLevel = pEquip->pEquipProto->byMinUseLevel;	// �ȼ�����
	pEquip->equipSpec.byMaxUseLevel = pEquip->pEquipProto->byMaxUseLevel;	// �ȼ�����

	if( pEquip->pEquipProto->nInitConsolidateLevel > 0 )
		pEquip->equipSpec.byConsolidateLevel = (BYTE)pEquip->pEquipProto->nInitConsolidateLevel;

	if( pEquip->pEquipProto->nBornLevelOfGodStrengthen > 0 )
	{
		pEquip->equipSpec.byLastGodStrengthenLevel	= pEquip->pEquipProto->nBornLevelOfGodStrengthen;
		pEquip->equipSpec.byGodStrengthenLevel		= pEquip->pEquipProto->nBornLevelOfGodStrengthen;
	}

	// ����Ʒ�������������
	if(!MIsFashion(pEquip))
	{
		// Jason 2010-4-14 v2.0.0����
		if( MIsFaBao(pEquip) )
		{
			IdentifyFabao((tagFabao*)pEquip,eQlty);
			//nQuality = pEquip->equipSpec.byQuality;
			nQuality = pEquip->equipSpec.byQuality;
		}
		// wcy 2012-03-31 ʥ��
		if( MIsHoly(pEquip) )
		{
			IdentifyHoly((tagHolyMan*)pEquip,(EItemQuality)nQuality);
		}
		// wcy 2012-04-18 ʥ��
		if( MIsHolyEquip(pEquip) )
		{
			IdentifyHolyEquip((tagHolyEquip*)pEquip,(EItemQuality)nQuality);
		}		
		CreateEquipQltyRel(pEquip, pEquip->pEquipProto, (EItemQuality)nQuality);
	}
	else
	{
		CreateFashionQltyRel(pEquip, pEquip->pEquipProto, (EItemQuality)nQuality, pIMEffect);
	}
}

//-----------------------------------------------------------------------------
// ����װ��Ʒ�����ɼ�������Ʒ��
//-----------------------------------------------------------------------------
INT32 ItemCreator::GenBaseEquipQlty(DWORD dwTypeID)
{
	tagEquipQltyPct *pEquipQltyPct = g_attRes.GetEquipQltyPct(dwTypeID);
	if(!P_VALID(pEquipQltyPct))
	{
		ASSERT(P_VALID(pEquipQltyPct));
		return EIQ_White;
	}

	INT32 nEquipQltySumPct = 0;
	INT32 nRandPct = IUTIL->Rand() % 10000;
	for(INT32 i= EIQ_End - 1; i>EIQ_Start; --i)
	{
		if( !P_VALID(pEquipQltyPct->nEquipQltyPct[i]) )
			continue;
		nEquipQltySumPct += pEquipQltyPct->nEquipQltyPct[i]; 
		if(nRandPct <= nEquipQltySumPct)
		{
			return i;
		}
	}

	return EIQ_White;
}

//-----------------------------------------------------------------------------
// ��������Ʒ��������������Ʒ��
// ������������ɫƷ��������
// �����Զ�������ɫװ������ˣ���EIQ_END��ΪEIQ_Purple
//-----------------------------------------------------------------------------
INT32 ItemCreator::ModEquipQltyByProduce(const tagEquip *pEquip, INT32 nQuality)
{
	INT nQltyPct = IUTIL->Rand() % 10000;
	if(pEquip->equipSpec.n16QltyModPctEx > 0
		&& nQltyPct < pEquip->equipSpec.n16QltyModPctEx)	// ��������
	{
		return min(nQuality + 2, EIQ_End - 1);
	}
	else	// һ������
	{
		if(pEquip->equipSpec.n16QltyModPct > 0)
		{
			if(nQltyPct < pEquip->equipSpec.n16QltyModPct)
			{
				return min(nQuality + 1, EIQ_End - 1);
			}
		}
		// �����˼�
		//else if(pEquip->equipSpec.n16QltyModPct < 0)
		//{
		//	if(nQltyPct < -pEquip->equipSpec.n16QltyModPct)
		//	{
		//		if( nQuality >= EIQ_End )
		//			nQuality = EIQ_End - 1 ;
		//		return max(nQuality - 1, EIQ_White);
		//	}
		//}
	}

	return nQuality;
}

//-----------------------------------------------------------------------------
// ����ָ��Ʒ������װ���������
//-----------------------------------------------------------------------------
DWORD ItemCreator::GenLongHunRandID(INT nLongHunType, INT nEquipPos, INT nQlty)
{
	if(nEquipPos < EEP_Equip_Start || nEquipPos > EEP_Equip_End)
	{
		ASSERT(0);
		return GT_INVALID;
	}
	
	const std::vector<DWORD> vectLongHun 
		= g_attRes.GetLongHunSpecVect(nLongHunType, g_eEquipGenLongHunTypeEx[nEquipPos], nQlty);

	INT nSzVect = vectLongHun.size();
	if(nSzVect > 0)
	{
		INT nRandPct = IUTIL->Rand() % nSzVect;
		return vectLongHun[nRandPct];
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------------------
// ����ָ��Ʒ������װ���������
//-----------------------------------------------------------------------------
VOID ItemCreator::CreateEquipQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, EItemQuality eQlty)
{
	ASSERT(eQlty > EIQ_Start && eQlty < EIQ_End);
	
	// �õ�ָ��Ʒ��װ�����Բ���
	const tagEquipQltyEffect *pEquipQltyEffect = g_attRes.GetEquipQltyEffect(eQlty);
	if(!P_VALID(pEquipQltyEffect))
	{
		ASSERT(P_VALID(pEquipQltyEffect));
		return;
	}
	
	// ����ϵ��
	FLOAT fValueModFactor;

	// װ���������� -- "������ԭʼ�༭ֵ; ���ߣ�ԭʼ�༭ֵ"
	fValueModFactor = pEquipQltyEffect->fWeaponFactor;
	pEquip->equipSpec.n16WuHun	= (INT16)(pEquipProto->n16WuHun * fValueModFactor);		// �ڹ��˺�������
	pEquip->equipSpec.n16MinDmg	= (INT16)(pEquipProto->n16MinDmg * fValueModFactor);	// ������С�˺�
	pEquip->equipSpec.n16MaxDmg	= (INT16)(pEquipProto->n16MaxDmg * fValueModFactor);	// ��������˺�

	fValueModFactor = pEquipQltyEffect->fArmorFactor;
	pEquip->equipSpec.n16Armor	= (INT16)(pEquipProto->n16Armor * fValueModFactor);		// ���߻���

	INT32 neigong;
	INT32 wuxiao;
	INT32 wuda;
	INT32 fang;
	/*neigong = IUTIL->Rand() %100;
	wuxiao = IUTIL->Rand() %100;
	wuda = IUTIL->Rand() %100;
	fang = IUTIL->Rand() %50;

	pEquip->equipSpec.n16WuHun	= pEquip->equipSpec.n16WuHun + neigong;		// �ڹ��˺�������
	pEquip->equipSpec.n16MinDmg	= pEquip->equipSpec.n16MinDmg + wuxiao;	// ������С�˺�
	pEquip->equipSpec.n16MaxDmg	= pEquip->equipSpec.n16MaxDmg + wuda;	// ��������˺�
	pEquip->equipSpec.n16Armor	= pEquip->equipSpec.n16Armor + fang;		// ���߻���*/

	// ��ɫһ������	--	A4 ��ֵ4=0.5+�ȼ���0.05	����ֵ4�������������1��2����3��һ������֮��							
	ZeroMemory(pEquip->equipSpec.nRoleAttEffect, sizeof(pEquip->equipSpec.nRoleAttEffect));

	// ��������װ����һ���������
	if( !MIsFaBao(pEquip) && !MIsSoulCrystal(pEquip) && !MIsHoly(pEquip) && !MIsHolyEquip(pEquip))
	{
		//fValueModFactor = pEquipQltyEffect->fAttAFactor;
		//INT32 nValueModLeft = (INT32)(pEquipQltyEffect->nAttAFactor + pEquipProto->byLevel * fValueModFactor);
		//INT32 nValueMod, nAttAIndex, nAttANumMod = 0;

		//while(nValueModLeft != 0)
		//{
		//	++nAttANumMod;
		//	nValueMod = IUTIL->Rand() % nValueModLeft + 1;
		//	nAttAIndex = IUTIL->Rand() % X_ERA_ATTA_NUM;

		//	if(nAttANumMod >= pEquipQltyEffect->nAttANumEffect)
		//	{
		//		nValueMod = nValueModLeft;
		//	}

		//	pEquip->equipSpec.nRoleAttEffect[nAttAIndex] += nValueMod;

		//	nValueModLeft -= nValueMod;
		//}
		CreateEquipRoleL1Att(pEquip,pEquipProto,pEquipQltyEffect);
	}

	CreateEquipRoleL2Att(pEquip,pEquipProto);

	// װ��Ǳ��ֵ -- ԭʼ�༭ֵ(��10%����)��4
	fValueModFactor = (1.0f + FLOAT(IUTIL->Rand() % 21 - 10) / 100.0f) * pEquipQltyEffect->fPotFactor * (FLOAT)(pEquip->equipSpec.n16PotValModPct) / 10000.0f;
	if( fValueModFactor <= 0 )
		fValueModFactor = 0;//1;
	pEquip->equipSpec.nPotVal = (INT)(pEquipProto->nPotVal * fValueModFactor);
	// Jason 2009-12-22 ����bug����Ʒ����ʱ���ᳬ�����Ǳ��ֵ�������ɫƷ��
	if( pEquip->equipSpec.nPotVal > pEquip->pEquipProto->nMaxPotVal )
		pEquip->equipSpec.nPotVal = pEquip->pEquipProto->nMaxPotVal;
	//else if( pEquip->equipSpec.nPotVal < 0 )
	//	pEquip->equipSpec.nPotVal = 0;

	
	// �����[0,10000)
	INT32 nRandPct;
	
	// ��Ƕ������ -- �޿�(0%) 1��(40%) 2��(30%) 3��(20%) 4��(0%) 5��(0%)
	pEquip->equipSpec.byHoleNum = 0;

	INT32 nHolePctSum = 0;
	if(!MIsHoly(pEquip) && !MIsHolyEquip(pEquip))
	{
		nRandPct = IUTIL->Rand() % 10000;
		if( nRandPct == 0 )
			nRandPct = 1;
		for(INT32 i=MAX_EQUIPHOLE_NUM; i>0; --i)
		{
			nHolePctSum += pEquipQltyEffect->nHoleNumPct[i];
			if(nRandPct < nHolePctSum)
			{
				pEquip->equipSpec.byHoleNum = i;
				break;
			}
		}
	}
	
	// ��������	-- 	"50%���ʳ��ֱ���������, 25%���ʳ������������"
	pEquip->equipSpec.dwLongHunInnerID	= GT_INVALID;
	pEquip->equipSpec.dwLongHunOuterID	= GT_INVALID;

	if( MIsFaBao(pEquip) )
		return;

	// .1����ֻ�����һ�� -- ������
	nRandPct = IUTIL->Rand() % 10000;
	//if(nRandPct < pEquipQltyEffect->nLonghunPct[ELHT_Inner])
	if (nRandPct < 5000)
	{
		pEquip->equipSpec.dwLongHunInnerID = GenLongHunRandID(ELHT_Inner, pEquipProto->eEquipPos, eQlty);
	}
	else
	{
		//nRandPct = IUTIL->Rand() % 10000;
		//if(nRandPct < pEquipQltyEffect->nLonghunPct[ELHT_Outer])
		//{
			pEquip->equipSpec.dwLongHunOuterID = GenLongHunRandID(ELHT_Outer, pEquipProto->eEquipPos, eQlty);
		//}
	}

	// װ���������� -- 5%����1��B������
	//nRandPct = IUTIL->Rand() % 10000;
	//if(nRandPct < pEquipQltyEffect->nSpecAttPct)
	//{
	//	nRandPct = IUTIL->Rand() % 10000;
	//	INT32 nSpecAttPctSum = 0;
	//	for(INT32 i=0; i<X_EQUIP_SPECATT_NUM; ++i)
	//	{
	//		nSpecAttPctSum += g_nEquipSpecAttPct[i];
	//		if(nRandPct < nSpecAttPctSum)
	//		{
	//			pEquip->equipSpec.bySpecAtt = i;
	//			break;
	//		}
	//	}

	//	if(pEquip->equipSpec.bySpecAtt <= (BYTE)EESA_EquipSpec_RelEnd)
	//	{
	//		// �����������Զ�װ�����Ե�Ӱ��
	//		ProcEquipAttBySpecAtt(pEquip);
	//	}
	//	else
	//	{
	//		// ��װ��ǿ��Ӱ�����������Ӱ�� -- ���������ʱ����
	//	}
	//}
	pEquip->equipSpec.bySpecAtt = EESA_Null;
}

/**
 * CreateEquipRoleL1Att ����װ���Ľ�ɫһ������Ӱ��ֵ
 * \param pEquip װ���������Ƿ��жϣ����ϲ㸺���ж�
 * \param pEquipProto װ��ԭ�ͣ�ͬ��
 * \param pEquipQltyEffect װ��Ʒ��Ӱ�죬ͬ��
 */
VOID ItemCreator::CreateEquipRoleL1Att(tagEquip * pEquip, const tagEquipProto *pEquipProto,const tagEquipQltyEffect * pEquipQltyEffect)
{
/*
	A1	����ֵ1�������[���ʡ���������Ԫ]����1-3����������֮��									
	B1	����ֵ1�������[���ʡ���Ԫ������]����1-3����������֮��									
	C1	����ֵ1�������[���ʡ���������Ԫ]����1-3����������֮��									
	D1	����ֵ1�������[���ʡ���Ԫ������]����1-3����������֮��									
	E1	����ֵ1�������[���ʡ���������������Ԫ]����1-3����������֮��									
*/
	static ERoleAttribute eRA[] = {ERA_Physique,ERA_Strength,ERA_Pneuma};
	static ERoleAttribute eRB[] = {ERA_Physique,ERA_Pneuma,ERA_InnerForce};
	static ERoleAttribute eRC[] = {ERA_Physique,ERA_Strength,ERA_Pneuma,ERA_InnerForce};
	static ERoleAttribute eRD[] = {ERA_Physique,ERA_Pneuma,ERA_InnerForce,ERA_Strength};
	static ERoleAttribute eRE[] = {ERA_Physique,ERA_Strength,ERA_InnerForce,ERA_Pneuma};

	struct tagInnerFactor
	{
		static INT GetAttNum(const tagEquip * pEquip)
		{
			INT num =3;
			if( pEquip->equipSpec.byQuality >= EIQ_Purple )
				num = 2;
			//if(pSrcAtt == eRE)
			//	num = 3;
			switch( num )
			{
			case 2:
				if( IUTIL->RandomInRange(1,100) <= 10 )
					num = 1;
				else
					num = 2;
				break;
			case 3:
				{
					INT nPct = IUTIL->RandomInRange(1,100);
					if( nPct <= 10 )
						num = 1;
					else if( nPct <= 40 )
						num = 2;
					else
						num = 3;
				}
				break;
			}
			return num;
		}
		static const ERoleAttribute * GetAttType(const tagEquipProto * pEquipProto,INT &size)
		{
			if( pEquipProto->eType == EIT_Weapon && pEquipProto->eTypeEx >= EITE_Sword && pEquipProto->eTypeEx <= EITE_Bow )
			{
				size = sizeof(eRA)/sizeof(eRA[0]);
				return eRA;
			}
			else if( pEquipProto->eType == EIT_Weapon )
			{
				size = sizeof(eRB)/sizeof(eRB[0]);
				return eRB;
			}
			else if( pEquipProto->eType == EIT_ClothArmor )
			{
				size = sizeof(eRD)/sizeof(eRD[0]);
				return eRD;
			}
			else if( pEquipProto->eType == EIT_Armor || pEquipProto->eType == EIT_NoviceEquip )
			{
				size = sizeof(eRC)/sizeof(eRC[0]);
				return eRC;
			}
			else if( pEquipProto->eType == EIT_Decoration || pEquipProto->eType == EIT_FaBao )
			{
				size = sizeof(eRE)/sizeof(eRE[0]);
				return eRE;
			}
			return NULL;
		}
		static ERoleAttribute RandomAtt(const ERoleAttribute* pLastAtt,INT nLastAttNum,const ERoleAttribute * pSrcAtt,INT nSrcAttSize)
		{
			if( !P_VALID(pLastAtt) || nLastAttNum < 0 || !P_VALID(pSrcAtt) || nSrcAttSize <= 0)
				return ERA_Null;
			ERoleAttribute re;
			INT i = 0;
			do
			{
				re = pSrcAtt[IUTIL->RandomInRange(0,nSrcAttSize-1)];
				for( i = 0; i < nLastAttNum; ++i )
				{
					if(pLastAtt[i] == re)
						break;
				}
				if( i >= nLastAttNum )
					break;
			}
			while( 1 );
			return re;
		}
	};

	if( pEquip->equipSpec.byQuality <= EIQ_White || pEquip->equipSpec.byQuality >= EIQ_End )
		return;

	INT size_att = 0;
	const ERoleAttribute * pRAtt = tagInnerFactor::GetAttType(pEquipProto,size_att);
	if( !P_VALID(pRAtt) )
		return;

	FLOAT fValueModFactor = pEquipQltyEffect->fAttAFactor;
	// ����������Ե�150��
	INT nTotalFactor = pEquipQltyEffect->nAttANumEffect;
	if( nTotalFactor == 0 )
		nTotalFactor = 150;
	INT32 nValueModLeft = (INT32)(pEquipQltyEffect->nAttAFactor + pEquipProto->byLevel * fValueModFactor) / nTotalFactor; //���������
	INT32 nValueMod;

	INT attNum = tagInnerFactor::GetAttNum(pEquip) ;
	INT32 start = 1;

	ERoleAttribute lastAtt[X_ERA_ATTA_NUM] ;//= {ERA_Null,ERA_Null,ERA_Null,ERA_Null,ERA_Null};
	for( INT np = 0; np < sizeof(lastAtt) / sizeof(lastAtt[0]); ++np )
		lastAtt[np] = ERA_Null;
	
	for( int i = 0; i < attNum; ++i )
	{
		ERoleAttribute ndx = tagInnerFactor::RandomAtt(lastAtt,i+1,pRAtt,size_att);
		lastAtt[i] = ndx;

		if( i == attNum - 1 )
		{
			pEquip->equipSpec.nRoleAttEffect[ndx] = nValueModLeft;
			break;
		}
		FLOAT v = FLOAT(IUTIL->RandomInRange(1,100)) / 100;
		nValueMod = FLOAT(nValueModLeft) * v;
		if( nValueMod == nValueModLeft )
			nValueMod = nValueModLeft - 1;
		pEquip->equipSpec.nRoleAttEffect[ndx] = nValueMod;
		nValueModLeft -= nValueMod;
	}
}

/**
 * ����װ���Ľ������ԣ���ɫ�Ķ������ԣ�
 * \param pEquip 
 * \param *pEquipProto �Ϸ�����֤���ϲ㸺��
 */
VOID ItemCreator::CreateEquipRoleL2Att(tagEquip * pEquip, const tagEquipProto *pEquipProto)
{
	static INT pct_white[3] = {100};
	static INT att_w[3]={1};
	// 10%, 60%, 30%
	static INT pct_yellow[3] = {30,100};
	static INT att_y[3] = {1,2};
	// 10%, 60%, 30%
	static INT pct_green[] = {10,70,100};
	static INT att_g[] = {1,2,3};
	// 10%, 60%, 30%
	static INT pct_blue[] = {10,70,100};
	static INT att_b[] = {2,3,4};
	// 30%, 50%, 20%
	static INT pct_orange[] = {30,80,100};
	static INT att_o[] = {3,4,5};
	// 30%, 60%, 10%
	static INT pct_purple[] = {30,90,100};
	static INT att_pu[] = {4,5,6};
	// 20%, 50%, 30%
	static INT pct_pink[] = {20,70,100};
	static INT att_pi[] = {4,5,6};

	INT * pct;
	INT * num;
	// ��������
	// nRoleL2AttCount
	struct tagInnerFactor
	{
		static BOOL GetParam(const tagEquip * pEquip,INT *& pctArray,INT *& numArray)
		{
			switch(pEquip->equipSpec.byQuality)
			{
			case EIQ_White:
				pctArray = pct_white;
				numArray = att_w;
				break;
			case EIQ_Yellow:
				pctArray = pct_yellow;
				numArray = att_y;
				break;
			case EIQ_Green:
				pctArray = pct_green;
				numArray = att_g;
				break;
			case EIQ_Blue:
				pctArray = pct_blue;
				numArray = att_b;
				break;
			case EIQ_Orange:
				pctArray = pct_orange;
				numArray = att_o;
				break;
			case EIQ_Purple:
				pctArray = pct_purple;
				numArray = att_pu;
				break;
			case EIQ_Pink:
				pctArray = pct_pink;
				numArray = att_pi;
				break;
			default:
				return FALSE;
			}
			return TRUE;
		}
	};
	
	if( tagInnerFactor::GetParam(pEquip,pct,num) )
	{
		INT count = 3;
		INT rad = IUTIL->RandomInRange(1,100);
		INT i = 0;
		for( ; i < count; ++i )
		{
			if(rad <= pct[i])
				break;
		}
		if( i < count )
			pEquip->equipSpec.nRoleL2AttCount = num[i];
		else
			pEquip->equipSpec.nRoleL2AttCount = 0;
	}
}

//-----------------------------------------------------------------------------
// �����������Զ�װ�����Ե�Ӱ��
//-----------------------------------------------------------------------------
VOID ItemCreator::ProcEquipAttBySpecAtt(tagEquip *pEquip)
{
	tagEquipSpec &equipSpec = pEquip->equipSpec;

	switch(equipSpec.bySpecAtt)
	{
	case EESA_LevelLim_Simple:
		// ����:��װ���ȼ�����-5����Ϳɼ�����0
		equipSpec.byMinUseLevel = (equipSpec.byMinUseLevel > 5 ? equipSpec.byMinUseLevel - 5 : 0);
		break;
	case EESA_LevelLim_Fine:
		// ����		��װ���ȼ�����-10����Ϳɼ�����0
		equipSpec.byMinUseLevel = (equipSpec.byMinUseLevel > 10 ? equipSpec.byMinUseLevel - 10 : 0);
		break;
	case EESA_LevelLim_None:
		// �޼���	��װ���޵ȼ�����
		equipSpec.byMinUseLevel = 0;
		break;

	case EESA_AttALim_Simple:
		// ���		��װ���������Ƽ���10%��ȡ��
		equipSpec.n16AttALimModPct = -1000;
		break;
	case EESA_AttALim_Comfort:
		// ����		��װ���������Ƽ���25%��ȡ��
		equipSpec.n16AttALimModPct = -2500;
		break;
	case EESA_AttALim_Light:
		// ��ӯ		��װ���������Ƽ���50%��ȡ��
		equipSpec.n16AttALimModPct = -5000;
		break;

	case EESA_Potential_YinFeng:
		// ����		��װ���ĳ�ʼǱ��ֵ+200
		equipSpec.nPotVal = min(equipSpec.nPotVal + 200, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_YinHuang:
		// ����		��װ���ĳ�ʼǱ��ֵ+400
		equipSpec.nPotVal = min(equipSpec.nPotVal + 400, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_FeiFeng:
		// �ɷ�		��װ���ĳ�ʼǱ��ֵ+800
		equipSpec.nPotVal = min(equipSpec.nPotVal + 800, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_MingHuang:
		// ����		��װ���ĳ�ʼǱ��ֵ+1200
		equipSpec.nPotVal = min(equipSpec.nPotVal + 1200, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_WoLong:
		// ����		װ���ĳ�ʼǱ��ֵ���5%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.05, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_CangLong:
		// ����		װ���ĳ�ʼǱ��ֵ���10%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.1, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_FuLong:
		// ����		װ���ĳ�ʼǱ��ֵ���20%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.2, pEquip->pEquipProto->nMaxPotVal);
		break;
	case EESA_Potential_ShengLong:
		// ����		װ���ĳ�ʼǱ��ֵ���30%
		equipSpec.nPotVal = (INT32)min(equipSpec.nPotVal * 1.3, pEquip->pEquipProto->nMaxPotVal);
		break;
	}
}

//-----------------------------------------------------------------------------
// ����ָ��Ʒ������ʱװ�������
//-----------------------------------------------------------------------------
BOOL ItemCreator::CreateFashionQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, 
									EItemQuality eQlty, const tagIMEffect *pIMEffect/* = NULL*/)
{
	ASSERT(eQlty > EIQ_Start && eQlty < EIQ_End);

	// ��ȡʱװ���������Դ
	const tagFashionGen *pGen = g_attRes.GetFashionQltyEffect(eQlty);
	if(!P_VALID(pGen))
	{
		MAttResCaution(_T("fashion_qlty_effect"), _T("Quality"), (INT)eQlty);
		return FALSE;
	}

	const tagFashionColorPct *pColor = g_attRes.GetFashionColorPct(eQlty);
	if(!P_VALID(pColor))
	{
		MAttResCaution(_T("fashion_color_pct"), _T("Quality"), (INT)eQlty);
		return FALSE;
	}

	// ʱװ�����ɿ���
	pEquip->equipSpec.bCanCut = FALSE;

	// Jason 2010-9-10 begin ȥ���������Щ�������
	pEquip->equipSpec.n8ColorID = EC_Null;
	pEquip->equipSpec.n16Appearance = 0;
	pEquip->equipSpec.byRein = 0;
	pEquip->equipSpec.bySavvy = 0;
	pEquip->equipSpec.byFortune = 0;

	CreateEquipRoleL2Att(pEquip,pEquipProto);

	// �������� -- ԭʼ�༭ֵ��1.5
	for (INT32 nCnt = 0; nCnt < pEquip->equipSpec.nRoleL2AttCount; nCnt++)
	{
		if(ERA_Appearance == pEquipProto->BaseEffect[nCnt].eRoleAtt)
			pEquip->equipSpec.n16Appearance += (INT16)(pEquipProto->BaseEffect[nCnt].nValue);
	}

	return TRUE;
	// Jason 2010-9-10 end

	// �����������
	INT nRand;
	
	// ��ɫ����
	INT nColor		= EC_Null;
	INT nColorPct	= 0;
	if(pIMEffect != NULL && EIME_Color == pIMEffect->eEffect)
	{
		nColor		= (INT)pIMEffect->dwParam1;
		nColorPct	= (INT)pIMEffect->dwParam2;
	}

	// .0ԭ��������ɫ������ԭ������ɫ
	if(pEquipProto->dwColor <= EC_End)
	{
		pEquip->equipSpec.n8ColorID = (INT8)pEquipProto->dwColor;
	}
	// .1���������Ƿ�Ϊ100%
	else if(10000 == nColorPct && nColor >= EC_Start && nColor <= EC_End)
	{
		pEquip->equipSpec.n8ColorID = nColor;
	}
	// .2������ɫ��������������ɫ
	else
	{
		INT nPctSum = 0;
		nRand = IUTIL->Rand() % 10000;
		pEquip->equipSpec.n8ColorID = EC_Val0;
		for(INT i=X_COLOR_NUM-1; i>=0; --i)
		{
			nPctSum += pColor->n16ColorPct[i];
			if(nColor == i)
			{
				nPctSum += nColorPct;
			}

			if(nRand < nPctSum)
			{
				pEquip->equipSpec.n8ColorID = i;
				break;
			}
		}
	}

	// ��װû�����Լӳ�
	if(EIQ_White == eQlty)
	{
		return TRUE;
	}

	// �������� -- ԭʼ�༭ֵ��1.5
	if(ERA_Appearance == pEquipProto->BaseEffect[0].eRoleAtt)
	{
		pEquip->equipSpec.n16Appearance = (INT16)(pEquipProto->BaseEffect[0].nValue * (pGen->fAppearanceFactor - 1));
	}

	// ͳ�����Լӳ� -- 10%���ʳ���, ֵ=��Ʒ�ȼ���5[ȡ��]
	nRand = IUTIL->Rand() % 10000;
	if(nRand < pGen->n16ReinPct && pGen->n8ReinVal > 0)
	{
		pEquip->equipSpec.byRein = pEquip->pEquipProto->byLevel / pGen->n8ReinVal;
	}

	// �������Լӳ� -- 10%���ʳ���, ֵ=��Ʒ�ȼ���20[ȡ��]
	nRand = IUTIL->Rand() % 10000;
	if(nRand < pGen->n16SavvyPct && pGen->n8SavvyVal > 0)
	{
		pEquip->equipSpec.bySavvy = pEquip->pEquipProto->byLevel / pGen->n8SavvyVal;
	}

	// ��Ե���Լӳ� -- 1%���ʳ���, ֵ=5�����¸���20%��+װ���ȼ�/30
	nRand = IUTIL->Rand() % 10000;
	if(nRand < pGen->n16FortunePct && pGen->n8FortuneVal2 > 0)
	{
		FLOAT fFactor = 1.0f + (FLOAT(IUTIL->Rand() % 41 - 20) / 100.0f);
		pEquip->equipSpec.byFortune 
			= (BYTE)(pGen->n8FortuneVal1 * fFactor + (DOUBLE)pEquipProto->byLevel / pGen->n8FortuneVal2);
	}


	return TRUE;
}

tagItem* ItemCreator::CreateTreasure(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num /*= 1*/, DWORD dwCreator /*= GT_INVALID*/, INT16 n16QltyModPct /*= 0*/, INT16 n16QltyModPctEx /*= 0*/ )
{
	ASSERT(GT_VALID(dwNameID));
	tagItem* pNew = Create(eCreateMode, dwCreateID, dwTypeID, n16Num, dwCreator, n16QltyModPct, n16QltyModPctEx);
	if (P_VALID(pNew))
	{
		pNew->dwNameID = dwNameID;
	}	
	return pNew;
}

tagItem* ItemCreator::CreateTreasureEx(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
								 INT16 n16Num/* = 1*/, EItemQuality eQlty/* = EIQ_Null*/, DWORD dwCreator/* = GT_INVALID*/, 
								 const tagIMEffect *pIMEffect/* = NULL*/)
{
	ASSERT(GT_VALID(dwNameID));
	tagItem* pNew = CreateEx(eCreateMode, dwCreateID, dwTypeID, n16Num, eQlty, dwCreator, pIMEffect);
	if (P_VALID(pNew))
	{
		pNew->dwNameID = dwNameID;
	}
	return pNew;
}

BOOL ItemCreator::IsGMItemNoInit(tagItem* pTmpItem)
{
	ASSERT(P_VALID(pTmpItem));

	if (EICT_Baibao == pTmpItem->eConType 
		&& !GT_VALID(pTmpItem->dwCreateID)
		&& !GT_VALID(pTmpItem->dwCreatorID))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL ItemCreator::InitGMItem(tagItem* pTmpItem)
{
	// ��ʼ����Ʒ����

	// ��ʼ��װ������
	if (MIsEquipment(pTmpItem->dwTypeID))
	{
		MTRANS_POINTER(pEquip, pTmpItem, tagEquip);
		ItemCreator::IdentifyEquip(pEquip, EItemQuality(pEquip->equipSpec.byQuality));
	}

	return TRUE;
}

static FLOAT CalFabaoGrowAtt(INT nGrowAttVal,FLOAT fCoefficientOfGrowing,INT16 n16NativeIntelligence)
{
	//return (FLOAT)nGrowAttVal * fCoefficientOfGrowing * ((FLOAT)n16NativeIntelligence + 100.0f) / 100;
	return (FLOAT)nGrowAttVal * fCoefficientOfGrowing * pow((FLOAT)n16NativeIntelligence / 200.0f,2);

}

// Jason 2010-4-13 v2.0.0
static VOID ArrageHoleWuxingAttLst(EWuXing att,EWuXing lastAtt[EWX_End],EWuXing *atts,INT & count);

VOID ItemCreator::IdentifyFabao(tagFabao * pFabao,EItemQuality qlty)
{
	if( !g_world.IsFabaoOpen() )
		return ;
	if( P_VALID(pFabao) )
	{
		tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pProtoType;
		const tagFabaoLevelProto * pFabaoLevelProto = NULL;
		if( P_VALID(pFabaoProto) )
		{
			// �����ȼ���ʼ��
			pFabao->n16Stage = 1;

			pFabao->equipSpec.byQuality = qlty;

			// ����Ʒ�ʼ���
			if( pFabao->equipSpec.byQuality <= EIQ_Start || pFabao->equipSpec.byQuality >= EIQ_End )
			{
				INT32 nRandPct = IUTIL->Rand() % 10000;
				INT nEquipQltySumPct = 0;
				for(INT32 i=EIQ_End -1 ; i>EIQ_Start; --i)
				{
					if( !P_VALID(pFabaoProto->nIdfQltyPct[i]) )
						continue;
					nEquipQltySumPct += pFabaoProto->nIdfQltyPct[i]; 
					if(nRandPct <= nEquipQltySumPct)
					{
						pFabao->equipSpec.byQuality = i;
						break;
					}
				}
				if( pFabao->equipSpec.byQuality <= EIQ_Start || pFabao->equipSpec.byQuality >= EIQ_End )
					pFabao->equipSpec.byQuality = EIQ_White;
			}

			// ��������
			if(pFabaoProto->bHasDefaultValue && pFabaoProto->n16DefaultFabaoQulity != GT_INVALID)
				pFabao->n16NativeIntelligence = pFabaoProto->n16DefaultFabaoQulity;
			else
			{
				int a = 0,b = 0 ;
				MGetFabaoQltyIntelWhenCreate(pFabao->equipSpec.byQuality,a,b);
				pFabao->n16NativeIntelligence = IUTIL->RandomInRange(a,b);
			}

			// �������ͣ����Ǵ�ԭ�ͱ�������ġ�
			memcpy(pFabao->eAttType,pFabaoProto->eAttType,sizeof(pFabao->eAttType));
			for( int i = 0; i < MaxFabaoRoleAttNUM; ++i )
			{
				if( GT_INVALID == pFabaoProto->nAttGrowing[i] )
					pFabao->nAttGrowing0[i] = 0;
				else
					pFabao->nAttGrowing0[i] = pFabaoProto->nAttGrowing[i];//IUTIL->Fluctuate(pFabaoProto->nAttGrowing[i],20,20) * (100 + (FLOAT)pFabao->n16NativeIntelligence) / 100 ;
			}

			// ��ʼ�������Ƚף�����ɳ����ԡ�
			pFabao->n16Stage = 1;
			pFabaoLevelProto = g_attRes.GetFabaoLevelProto(pFabao->n16Stage);
			for( int i = 0; P_VALID(pFabaoLevelProto) && i < MaxFabaoRoleAttNUM; ++i )
			{
				if( GT_INVALID == pFabao->nAttGrowing0[i] )
					pFabao->nAttGrowing[i] = 0;
				else
					//pFabao->nAttGrowing[i] = (FLOAT)pFabao->nAttGrowing0[i] * ((FLOAT)pFabaoLevelProto->fCoefficientOfGrowing ) * ((FLOAT)pFabao->n16NativeIntelligence + 100.0f) / 100;
					pFabao->nAttGrowing[i] = CalFabaoGrowAtt(pFabao->nAttGrowing0[i],pFabaoLevelProto->fCoefficientOfGrowing,pFabao->n16NativeIntelligence);
			}

			// ��������������
			//if(pFabaoProto->bHasDefaultValue && EWX_Null != pFabaoProto->n16DefaultWuxing )
			//{
			//	pFabao->eWuxing = (EWuXing)pFabaoProto->n16DefaultWuxing;
			//}
			//else
			//	pFabao->eWuxing = (EWuXing)IUTIL->RandomInRange(EWX_Metal,EWX_Soil);
			pFabao->eWuxing = EWX_Null;

			// Ԫ���˺���ʼ��
			if( pFabaoProto->eEleInjury == EEI_Null )
				pFabao->eEleInjury = (EElementInjury)IUTIL->RandomInRange(EEI_Fire,EEI_Dark);
			else
				pFabao->eEleInjury = pFabaoProto->eEleInjury;
			// ���Ը���
			for( INT nER = EER_START; nER < EER_End; ++nER )
			{
				pFabao->nEleResistance[nER] = pFabaoProto->nEleResistance[nER];
			}

			// �����������
			// ������޸ģ��Ƿ�ֹxml�У�ĳЩ���������-1�����
			memset(pFabao->eWuxingHole,GT_INVALID,sizeof(pFabao->eWuxingHole));

			if(pFabaoProto->bHasDefaultValue )
			{
				for( int i = 0 ; i < EWX_End; ++i )
				{
					if( GT_INVALID != pFabaoProto->eDefaultSlotType[i] )
						pFabao->eWuxingHole[i] = pFabaoProto->eDefaultSlotType[i];
				}
			}
			{
				EWuXing ewLast[EWX_End] = {EWX_Null,EWX_Null,EWX_Null,EWX_Null,EWX_Null};
				EWuXing ewAtt[EWX_End] = {EWX_Metal,EWX_Wood,EWX_Water,EWX_Fire,EWX_Soil};
				INT aCount = EWX_End;
				for( int i = 0; i < EWX_End ; ++i )
				{
					if( GT_INVALID == pFabao->eWuxingHole[i] )
						pFabao->eWuxingHole[i] = ewAtt[IUTIL->RandomInRange(0,aCount-1)];
					ArrageHoleWuxingAttLst(pFabao->eWuxingHole[i],ewLast,ewAtt,aCount);
				}
			}

			// ���������ʼ��Ϊ0
			memset(pFabao->dwWuxingBeadID,GT_INVALID,sizeof(pFabao->dwWuxingBeadID));

			// ����������Ͻ�������ԭ�ͱ��ȡ
			memcpy(pFabao->eSlotAttType,pFabaoProto->eSlotAttType,sizeof(pFabao->eSlotAttType));
			memcpy(pFabao->nSlotAttValue,pFabaoProto->nSlotAttValue,sizeof(pFabao->nSlotAttValue));
			//for( INT i = 0; i < MaxWuxingSlotNum; ++i )
			//{
			//	if( pFabao->eWuxingHole[i] == pFabao->eWuxing ) //����ۼ���˫������
			//	{
			//		if( MIsValuePct(pFabao->nSlotAttValue[i]) )
			//		{
			//			INT percent = MValuePctTrans(pFabao->nSlotAttValue[i]);
			//			pFabao->nSlotAttValue[i] = pFabao->nSlotAttValue[i] > 100000 ? percent * 2 + 100000 : percent * 2 - 100000;
			//		}
			//		else
			//			pFabao->nSlotAttValue[i] += pFabao->nSlotAttValue[i];
			//	}
			//}
			
			// ��Сʹ�ü���
			pFabao->nUseLevel = pFabaoLevelProto->nMinUseLevel;

			// ������������������ȥ����������
			//memset(pFabao->nWuxingMatchReward,GT_INVALID,sizeof(pFabao->nWuxingMatchReward));
			//if(pFabaoProto->bHasDefaultValue)
			//{
			//	memcpy(pFabao->nWuxingMatchReward,pFabaoProto->nWuxingMatchReward,sizeof(pFabao->nWuxingMatchReward));
			//}
			//for( int i = 0; i < MaxWuxingRewardNUM; ++i )
			//{
			//	if( GT_INVALID == pFabao->nWuxingMatchReward[i] )
			//		pFabao->nWuxingMatchReward[i] = SpecRandFabaoWuxingRewardID(-1,pFabao->n16NativeIntelligence,pFabaoProto->eFarbaoType,i+1,pFabao->nWuxingMatchReward);
			//}

			// ��۳�ʼ��
			pFabao->nSlotNum = CalFabaoSlotNum(pFabao->n16Stage ) ;
			if( pFabao->nSlotNum > MaxWuxingSlotNum )
				pFabao->nSlotNum  = MaxWuxingSlotNum;
			for( INT i = 0; i < pFabao->nSlotNum; ++i )
				pFabao->dwWuxingBeadID[i] = 0;
			// ����ֵ
			pFabao->nPracticeValue = 0;

			pFabao->nMood	= 999;	// �����ڵ��ڵȼ�30����ʱ��һֱ��1000

			for( INT i = 0; i < MaxFairySkillNum; ++i )
				pFabao->dwFairySkill[i] = GT_INVALID;
		}
	}
}

// wcy 2012-03-31 ʥ��
VOID ItemCreator::IdentifyHoly(tagHolyMan * pHoly,EItemQuality qlty )
{
	if( P_VALID(pHoly) )
	{
		tagHolyProto * pHolyProto = (tagHolyProto*)pHoly->pProtoType;		
		if( P_VALID(pHolyProto) )
		{
			pHoly->equipSpec.byQuality = qlty;

			// ����proto��ʥ���������Գ�ʼ��
			pHoly->nDevourNum = 0;
			pHoly->n16EquipmentNumber = pHolyProto->n16EquipmentNumber;
			pHoly->nCoValue  = 0;				
			pHoly->nToDayEatNum = 0;
			pHoly->nCostHoly = 0;
			pHoly->dwLevelUpAtt[ESAT_Demage] = pHolyProto->nHolyDmg;			
			pHoly->dwLevelUpAtt[ESAT_HolyDef] = pHolyProto->nHolyDef;
			pHoly->dwLevelUpAtt[ESAT_Crit] = pHolyProto->nCrit;
			pHoly->dwLevelUpAtt[ESAT_CritRate] = pHolyProto->n16HolyCritRate;
			pHoly->dwLevelUpAtt[ESAT_ExDmg] = pHolyProto->nExDamage;
			pHoly->dwLevelUpAtt[ESAT_AttackTec] = pHolyProto->nAttackTec;
			pHoly->dwLevelUpAtt[ESAT_NeglectToughness] = pHolyProto->nNeglectToughness;
			pHoly->dwLevelUpAtt[ESAT_LingNeng] = pHolyProto->nHolyValue;	
			pHoly->nMaxDevourNum = pHolyProto->nDevourNum;
		}
	}
}

// wcy 2012-04-18 ʥ��
VOID ItemCreator::IdentifyHolyEquip(tagHolyEquip * pHolyEquip,EItemQuality qlty)
{
	if( P_VALID(pHolyEquip) )
	{
		tagHolyEquipProto * pHolyEquipProto = (tagHolyEquipProto*)pHolyEquip->pProtoType;		
		if( P_VALID(pHolyEquipProto) )
		{
			pHolyEquip->equipSpec.byQuality = qlty;

			// ����proto��ʥ���������Գ�ʼ��
			pHolyEquip->nCostHoly = pHolyEquipProto->nCostHolyValue;
			pHolyEquip->n16EnhanceCount = 0;
			EHolyAttType eAttType;
			for(INT32 i=0; i<MAX_EQUIPATT_BASE_EFFECT; ++i)
			{
				if (pHolyEquipProto->BaseEffect[i].eHolyAtt != ESAT_NULL)
				{
					eAttType = pHolyEquipProto->BaseEffect[i].eHolyAtt;
					if (eAttType >= X_ESAT_ATTA_NUM)
					{
						ILOG->Write(_T("tagHolyEquipProto BaseEffect[i].eHolyAtt cannot be ESAT_LingNeng\r\n"));
						continue;
					}
					pHolyEquip->nHolyAttEffect[eAttType] = pHolyEquipProto->BaseEffect[i].nValue;

				}	
				else
				{
					break;
				
				}
			}			
		}
	}


}

/**
 * ���߻����й��ɵõ�������۵�������������
 * \param att ������۵���������
 * \param lastAtt[EWX_End] ��۵���ʷ��¼
 * \param atts[EWX_End] ��ۿ���ʹ�õ�ʣ�������б�
 * \param count ʣ������������Ը���
 */
static VOID ArrageHoleWuxingAttLst(EWuXing att,EWuXing lastAtt[EWX_End],EWuXing* atts,INT & count)
{
	int i;
	for( i = 0; i < 5 ; ++i )
	{
		// �������������������2�Σ��ڴ�ʱ���������ʣ���������
		if( att == lastAtt[i] )
		{
			for( int j = 0; j < EWX_End; ++j )
			{
				if( atts[j] == lastAtt[i] )
				{
					if( j == EWX_End )
						atts[j] = EWX_Null;
					else
					{
						//memmove( &atts[j],&atts[j+1],(EWX_End-j-1) * sizeof(EWuXing) );
						for( int a = j ; a < EWX_End-1; ++a )
							atts[a] = atts[a+1];
					}
					//atts[count] = EWX_Null;

					--count;
					return;
				}
			}
			return ;
		}
		else if( lastAtt[i] == EWX_Null )
		{
			lastAtt[i] = att;
			return;
		}
	}
}
/**
 * �������������а�һ�����ɵõ�һ��������������������id
 *
 * \param nDifficulty ���Ѷ�
 * \param QltyIntl ����
 * \param type ��������
 * \param pos ����װ��λ�ã��沿
 * \return ���������е�һ��idֵ
 */
INT ItemCreator::RandFabaoWuxingRewardID(INT nDifficulty,INT QltyIntl,ENUM_FabaoType type,INT pos)
{
	TMap<INT,tagFabaoWuxingProto*> * map_wuxing = const_cast<TMap<INT,tagFabaoWuxingProto*> *>(&g_attRes.GetFabaoWuxingProtoMap());
	INT nRare = 0;

	INT nRand = IUTIL->RandomInRange(0,100);
	//if( nRand <= 80 )
	//	nRare = 3;
	//else if( nRand <= 95 )
	//	nRare = 2;
	//else
	//	nRare = 1;
	INT v = nDifficulty; // �ѶȾ��Ǹ����������ʣ������������������ã������Ҫ���Ӳ���
	v /= 10;
	if( nRand <= v )
		nRare = 1;
	else if( nRand <= v + 15 )
		nRare = 2;
	else
		nRare = 3;

	if( nDifficulty == GT_INVALID )
		nDifficulty = 0x7FFFFFFF;

	if( QltyIntl == GT_INVALID )
		QltyIntl = 0x7FFFFFFF;

	vector<int> vecTem;
	tagFabaoWuxingProto * pProto = NULL;
	for( TMap<INT,tagFabaoWuxingProto*>::TMapIterator i = map_wuxing->Begin(); map_wuxing->PeekNext(i,pProto); )
	{
		if( pProto->nRare == nRare && (type == EFB_NULL || pProto->eFabaoTypeLim==EFB_NULL || pProto->eFabaoTypeLim == type) && 
			pProto->n16NativeIntelligenceLim <= QltyIntl && 
			( EEP_Null == pos || pProto->nXSRewardTimes == EEP_Null || pos == pProto->nXSRewardTimes) && 
			nDifficulty >= pProto->nDifficulty )
		{
			vecTem.push_back(pProto->nID);
		}
	}

	size_t count = vecTem.size();
	if( count > 0 )
	{
		int ndx = IUTIL->RandomInRange(0,count - 1);
		return vecTem[ndx];
	}

	return GT_INVALID;
}

INT ItemCreator::SpecRandFabaoWuxingRewardID(INT nDifficulty,INT QltyIntl,ENUM_FabaoType type,INT pos,INT nWuxingMatchReward[MaxWuxingRewardNUM])
{
	//for(int i = 0; i < 3; ++i)
	//{
	//	INT a = 0;
	//	INT val = RandFabaoWuxingRewardID(nDifficulty,QltyIntl,type,pos);
	//	for( ; a < MaxWuxingRewardNUM; ++a )
	//	{
	//		if( val == nWuxingMatchReward[i] )
	//		{
	//			//val = RandFabaoWuxingRewardID(nDifficulty,QltyIntl,type,pos);
	//			break;
	//		}
	//	}
	//	if( a >= MaxWuxingRewardNUM )
	//		return val;
	//}
	return GT_INVALID;
}

BOOL ItemCreator::RecalFabaoAtt(tagFabao * pFabao)
{
	BOOL changed = FALSE;
	const INT nFabaoMaxStage = g_attRes.GetFabaoMaxStage();
	for( int i = pFabao->n16Stage + 1; i <= nFabaoMaxStage; ++i )
	{
		const tagFabaoLevelProto * p = g_attRes.GetFabaoLevelProto(i);
		if( P_VALID(p) )
		{
			if( pFabao->nPracticeValue >= p->nMinValue && pFabao->nPracticeValue <= p->nMinValue + p->nLevelUpWhated )
			{
				pFabao->n16Stage = i;
				changed = TRUE;
				pFabao->nUseLevel = p->nMinUseLevel;
				// ��۸���
				INT nSlotNum = pFabao->nSlotNum;
				INT nSlotnums = CalFabaoSlotNum(pFabao->n16Stage );
				if(nSlotnums > MaxWuxingSlotNum)
					nSlotnums = MaxWuxingSlotNum;
				pFabao->nSlotNum = nSlotnums;
				if( nSlotNum < pFabao->nSlotNum )
				{
					for( INT j = 0; j < pFabao->nSlotNum; ++j )
					{
						if( (DWORD)GT_INVALID == pFabao->dwWuxingBeadID[j] )
							pFabao->dwWuxingBeadID[j] = 0;
					}

				}
				// �ɳ�����
				const tagFabaoLevelProto * pFabaoLevelProto = g_attRes.GetFabaoLevelProto(pFabao->n16Stage);
				for( int j = 0; P_VALID(pFabaoLevelProto) && j < MaxFabaoRoleAttNUM; ++j )
				{
					//pFabao->nAttGrowing[j] = (FLOAT)pFabao->nAttGrowing0[j] * ((FLOAT)pFabaoLevelProto->fCoefficientOfGrowing ) * ((FLOAT)pFabao->n16NativeIntelligence + 100.0f) / 100;//(FLOAT)pFabao->nAttGrowing0[j] * (FLOAT)pFabaoLevelProto->fCoefficientOfGrowing;
					pFabao->nAttGrowing[j] = CalFabaoGrowAtt(pFabao->nAttGrowing0[j],pFabaoLevelProto->fCoefficientOfGrowing,pFabao->n16NativeIntelligence);
				}
				break;
			}
		}
	}
	RecalFaBaoCurStoredExp(pFabao);
	return changed;
}
BOOL ItemCreator::RecalFabaoAttWithoutCalStage(tagFabao * pFabao)
{
	BOOL re = FALSE;
	const tagFabaoLevelProto * pFabaoLevelProto = g_attRes.GetFabaoLevelProto(pFabao->n16Stage);
	for( int j = 0; P_VALID(pFabaoLevelProto) && j < MaxFabaoRoleAttNUM; ++j )
	{
		pFabao->nAttGrowing[j] = CalFabaoGrowAtt(pFabao->nAttGrowing0[j],pFabaoLevelProto->fCoefficientOfGrowing,pFabao->n16NativeIntelligence);
		if( !re )
			re = TRUE;
	}
	return re;
}
INT	ItemCreator::RandomFabaoWuxingHole(tagFabao * pFabao)
{
	if( P_VALID(pFabao) )
	{
		int a = -1;
		int count = min(MaxWuxingSlotNum,pFabao->nSlotNum);
		for( int i = 0 ; i < count ; ++i )
		{
			if( !P_VALID(pFabao->dwWuxingBeadID[i]) )
			{
				a = i;
				break;
			}
		}
		if( a == -1 )
		{
			if( 0 == count )
				return 0;
			return IUTIL->RandomInRange(0,count - 1);
		}
		else
		{
			INT nSlotnums = CalFabaoSlotNum(pFabao->n16Stage ) ;
			if(nSlotnums > MaxWuxingSlotNum)
				nSlotnums = MaxWuxingSlotNum;
			if( pFabao->nSlotNum > MaxWuxingSlotNum )
				pFabao->nSlotNum = nSlotnums;
			int per1 = 0;
			switch( pFabao->nSlotNum )
			{
			case 1:
				per1 = 100;
				break;
			case 2:
				per1 = 80;
				break;
			case 3:
				per1 = 60;
				break;
			case 4:
				per1 = 40;
				break;
			case 5:
				per1 = 20;
				break;
			}
			if( per1 > 0 && IUTIL->RandomInRange(0,100) > per1 )
			{
				if( a >= 1 ) // ��ʵ������������϶�����bug
					return IUTIL->RandomInRange(0,a-1);
			}
		}
		return a;
		//else if( 0 == a )
		//	return 0;
		//else if( a == 1 )
		//{
		//	return IUTIL->RandomInRange(0,100) <= 80 ? 0 : 1;
		//}
		//else if( a > 1 )
		//{
		//	int rand = IUTIL->RandomInRange(0,100);
		//	if( rand > 80 )
		//		return a;
		//	else
		//	{
		//		return IUTIL->RandomInRange(0,a-1);
		//	}
		//}
	}
	return GT_INVALID;
}
INT ItemCreator::MergeWuxingHoleAtt(INT nQvalue,tagFabao * pFabao,tagFabao* pSecFabao)
{
	if( !P_VALID(pFabao) )
		return GT_INVALID;
	INT nStart = -1,nEnd = -1;
	EWuXing eStart = EWX_Null, eVal = EWX_Null;
	if( nQvalue >= 90 )
	{
		if(IUTIL->RandomInRange(0,100) <= 50)
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Fire;
		}
		else
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Soil;
		}
	}
	else if( nQvalue >= 80 )
	{
		INT a = IUTIL->RandomInRange(0,100);
		if( a <= 20 )
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Water;
		}
		else if( a <= 80 )
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Fire;
		}
		else
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Soil;
		}
	}
	else if( nQvalue >= 70 )
	{
		INT a = IUTIL->RandomInRange(0,100);
		if( a <= 60 )
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Water;
		}
		else if( a <= 95 )
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Fire;
		}
		else
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Soil;
		}
	}
	else if( nQvalue >= 60 )
	{
		INT a = IUTIL->RandomInRange(0,100);
		if( a <= 90 )
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Water;
		}
		else
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Fire;
		}
	}
	else
	{
		INT a = IUTIL->RandomInRange(0,100);
		if( a <= 50 )
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Wood;
		}
		else
		{
			nStart	= EWX_Metal;
			nEnd	= EWX_Water;
		}
	}

	//pFabao->eWuxing = (EWuXing)IUTIL->RandomInRange(EWX_Metal,EWX_Soil);

	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{
		if( i == nStart )
		{
			eStart = (EWuXing)IUTIL->RandomInRange(EWX_Metal,EWX_Soil);
			pFabao->eWuxingHole[i] = eStart;
		}
		else if( i > nStart && i <= nEnd )
			pFabao->eWuxingHole[i] = eStart;
		else if( i > nEnd )
		{
			while(1)
			{
				eVal = (EWuXing)IUTIL->RandomInRange(EWX_Metal,EWX_Soil);
				if( eVal != eStart )
				{
					pFabao->eWuxingHole[i] = eVal;
					break;
				}
			}
		}
	}
	// ȥ������ĵ�������
#if 0
	// ������lese
	if( nQvalue < 70 )
	{
		EWuXing att[1+MaxWuxingSlotNum] = {EWX_Null,EWX_Null,EWX_Null,EWX_Null,EWX_Null,EWX_Null};
		BYTE pos[(1+MaxWuxingSlotNum) / 2] = {0};
		att[0] = pFabao->eWuxing;
		for( int i = 0; i < MaxWuxingSlotNum ; ++i )
		{
			att[1+i] = pFabao->eWuxingHole[i];
		}
		for( int i = 0; i < (1+MaxWuxingSlotNum) / 2; ++i )
		{
			for(int j = 0; j < MaxWuxingSlotNum+1; ++j)
				if( att[i] == att[j] )
					pos[i]++;
		}
		EWuXing ewAtt[EWX_End] = {EWX_Metal,EWX_Wood,EWX_Water,EWX_Fire,EWX_Soil};
		EWuXing ewTemAtt[EWX_End] = {EWX_Null,EWX_Null,EWX_Null,EWX_Null,EWX_Null};
		int a = 0;
		if( pos[0] == 6 )
		{
			for( int i = 0; i < EWX_End; ++i )
				if( ewAtt[i] != att[0] )
					ewTemAtt[a++] = ewAtt[i];
			int ndx = IUTIL->RandomInRange(0,5);
			int nF = 0;
			if( ndx == 0 )
			{
				pFabao->eWuxing = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
				att[ndx] = pFabao->eWuxing;
				nF = 1;
			}
			else
			{
				pFabao->eWuxingHole[ndx] = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
				att[ndx] = pFabao->eWuxingHole[ndx];
				nF = 0;
			}

			a = 0;
			for( int i = 0; i < EWX_End; ++i )
				if( ewAtt[i] != att[nF] && ewAtt[i] != att[ndx] )
					ewTemAtt[a++] = ewAtt[i];
			nF = 0;
			while(1)
			{
				nF = IUTIL->RandomInRange(0,5);
				if( nF != ndx )
					break;
			}
			ndx = nF;
			if( ndx == 0 )
				pFabao->eWuxing = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
			else
				pFabao->eWuxingHole[ndx - 1] = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
		}
		else if (pos[0] == 5 || pos[1] == 5 )
		{
			int i = 0;
			if( pos[0] != 5  )
			{
				int ndx = IUTIL->RandomInRange(0,4);
				for( int i = 0; i < EWX_End; ++i )
					if( ewAtt[i] != att[0] && ewAtt[i] != att[1] )
						ewTemAtt[a++] = ewAtt[i];
				pFabao->eWuxingHole[ndx] = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
			}
			else
			{
				for( ;i<MaxWuxingSlotNum;++i )
				{
					if(att[i] != 5)
						break;
				}
				int ndx = 0;
				while(1)
				{
					ndx = IUTIL->RandomInRange(0,5);
					if( ndx != i )
						break;
				}
				int nF = 0;
				if( i == 0 )
					nF = 1;
				else
					nF = 0;
				for( int j = 0; j < EWX_End; ++j )
					if( ewAtt[j] != att[nF] && ewAtt[j] != att[i] )
						ewTemAtt[a++] = ewAtt[j];
				if( ndx == 0 )
					pFabao->eWuxing = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
				else
					pFabao->eWuxingHole[ndx] = ewTemAtt[IUTIL->RandomInRange(0,a-1)];
			}
		}
	}
#endif	
	// ���������ģ��Ӽ�����������
	tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pProtoType;
	if( !P_VALID(pFabaoProto) )
		return GT_INVALID;
	// ����������Ͻ�������ԭ�ͱ��ȡ
	memcpy(pFabao->eSlotAttType,pFabaoProto->eSlotAttType,sizeof(pFabao->eSlotAttType));
	memcpy(pFabao->nSlotAttValue,pFabaoProto->nSlotAttValue,sizeof(pFabao->nSlotAttValue));

	// ȥ��˫������
#if 0
	for( INT i = 0; i < MaxWuxingSlotNum; ++i )
	{
		if( pFabao->eWuxingHole[i] == pFabao->eWuxing ) //����ۼ���˫������
		{
			if( MIsValuePct(pFabao->nSlotAttValue[i]) )
			{
				INT percent = MValuePctTrans(pFabao->nSlotAttValue[i]);
				pFabao->nSlotAttValue[i] = pFabao->nSlotAttValue[i] > 100000 ? percent * 2 + 100000 : percent * 2 - 100000;
			}
			else
				pFabao->nSlotAttValue[i] += pFabao->nSlotAttValue[i];
		}
	}
#endif
	return 0;
}

INT ItemCreator::ChangeFabaoNativeIntelligence(tagFabao * pFabao,INT addvalue,INT nEfficacy)
{
	if(!P_VALID(pFabao) || !P_VALID(pFabao->pProtoType))
		return GT_INVALID;
	INT nMinIntelligence,nMaxIntelligence;
	if( pFabao->equipSpec.byQuality <= EIQ_Start || pFabao->equipSpec.byQuality >= EIQ_End )
		return GT_INVALID;
	
	MGetFabaoQltyIntel(pFabao->equipSpec.byQuality,nMinIntelligence,nMaxIntelligence);
	nMaxIntelligence = nMaxIntelligence > pFabao->byMaxIntelligence ? nMaxIntelligence : pFabao->byMaxIntelligence;
	INT nFenmu = nMaxIntelligence - nMinIntelligence;
	if( nFenmu == 0 )
		return GT_INVALID;

	if( pFabao->n16NativeIntelligence >= nMaxIntelligence )
	{
		// ���ʷ����仯�����¼���
		RecalFaBaoCurStoredExp(pFabao);

		pFabao->n16NativeIntelligence = nMaxIntelligence;
		return EFABAO_RaiseIntelligence_AlreadyFull;
	}

	FLOAT fPct = 0.0f;
	if (-2 == nEfficacy) // �ȹ�������ҩ
	{
		fPct = 100.0f;
	}
	else
	{
		INT nTmpValue = (pFabao->n16NativeIntelligence < 100 && nMaxIntelligence > 100) ? 100 : nMaxIntelligence;
		fPct = (FLOAT(nTmpValue) - FLOAT(pFabao->n16NativeIntelligence)) / (FLOAT(nFenmu));
		fPct *= (1 + (FLOAT)nEfficacy / 90) + ((FLOAT)nEfficacy * (FLOAT)nEfficacy / 100000);
		fPct *= 100;
		if( fPct > 100 )
			fPct = 100;
		else if( fPct < 0 )
			fPct = 0;
	}
	
	bool bChanged = false;

	DWORD dwRe = EFABAO_RaiseIntelligence_Success;

	if( IUTIL->RandomInRange(0,100) <= fPct ) // �ɹ�
	{
		pFabao->n16NativeIntelligence += addvalue;
		if( pFabao->n16NativeIntelligence > nMaxIntelligence )
		{
			pFabao->n16NativeIntelligence = nMaxIntelligence;
			dwRe = EFABAO_RaiseIntelligence_NoChange;
		}
		else
		{
			bChanged = true;
		}
	}
	else // ʧ��
	{
		FLOAT fSubPct = (100.0f - fPct) * 0.2;
		dwRe = EFABAO_RaiseIntelligence_NoChange;
		if( fSubPct > 0 && IUTIL->RandomInRange(0,100.0f - fPct) <= fSubPct )
		{
			pFabao->n16NativeIntelligence -= addvalue;
			if( pFabao->n16NativeIntelligence < nMinIntelligence )
				pFabao->n16NativeIntelligence = nMinIntelligence;
			else
			{
				bChanged = true;
				dwRe = EFABAO_RaiseIntelligence_Failse;
			}
		}
	}
	// �ɳ�����
	if( bChanged )
	{
		const tagFabaoLevelProto * pFabaoLevelProto = g_attRes.GetFabaoLevelProto(pFabao->n16Stage);
		for( int j = 0; P_VALID(pFabaoLevelProto) && j < MaxFabaoRoleAttNUM; ++j )
		{
			//pFabao->nAttGrowing[j] = (FLOAT)pFabao->nAttGrowing0[j] * ((FLOAT)pFabaoLevelProto->fCoefficientOfGrowing ) * ((FLOAT)pFabao->n16NativeIntelligence + 100.0f) / 100;//(FLOAT)pFabao->nAttGrowing0[j] * (FLOAT)pFabaoLevelProto->fCoefficientOfGrowing;
			pFabao->nAttGrowing[j] = CalFabaoGrowAtt(pFabao->nAttGrowing0[j],pFabaoLevelProto->fCoefficientOfGrowing,pFabao->n16NativeIntelligence);
		}
	}
	
	// ���ʷ����仯�����¼���
	RecalFaBaoCurStoredExp(pFabao);

	return dwRe;
}

INT	ItemCreator::CalFabaoSlotNum(INT stage)
{
	if( stage <= 9  )
		return 0;
	else if( stage <= 19 )
		return 1;
	else if( stage <= 29 )
		return 2;
	else if( stage <= 39 )
		return 3;
	else if( stage <= 59 )
		return 4;
	return 5;
}

INT ItemCreator::RecalEquipStrengthAtt(tagEquip * pEquip,INT level)
{
	if( !P_VALID(pEquip) )
		return -1;

	for( INT i = 0 ; i < MAX_FCONSOLIDATE_NUM; ++i )
	{
		pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt = ERA_Null;
		pEquip->equipSpec.ConsolidateEffect[i].nValue = 0;
	}

	INT re = 0;

	TMap<DWORD,tagFConsolidateProto*> & conMap = g_attRes.GetFConsolidateProto();
	tagFConsolidateProto* p = NULL;
	for(TMap<DWORD,tagFConsolidateProto*>::TMapIterator it = conMap.Begin(); conMap.PeekNext(it,p); )
	{
		if( pEquip->pEquipProto->eTypeEx == p->eItemTypeEx && p->n16Times <= level )
		{
			const tagFConsolidateProto * pConFormula = p;
			for( INT i = 0 ; i < MAX_FCONSOLIDATE_NUM; ++i )
			{
#if 0
				if( ERA_AttA_Start <= pConFormula->RoleAttEffect[i].eRoleAtt && pConFormula->RoleAttEffect[i].eRoleAtt < ERA_End && P_VALID(pConFormula->RoleAttEffect[i].nValue) )
				{
					++re;
					
					for( int j = 0; j < MAX_FCONSOLIDATE_NUM; ++j )
					{
						if( pEquip->equipSpec.ConsolidateEffect[j].eRoleAtt == pConFormula->RoleAttEffect[i].eRoleAtt )
						{
							if( j < 3 )
								pEquip->equipSpec.ConsolidateEffect[j].nValue += pConFormula->RoleAttEffect[i].nValue * ( 1 + pConFormula->fcoefficientA * pEquip->equipSpec.byQuality * pEquip->pEquipProto->byLevel );
							else
								pEquip->equipSpec.ConsolidateEffect[j].nValue += pConFormula->RoleAttEffect[i].nValue;

							break;
						}
						else if( pEquip->equipSpec.ConsolidateEffect[j].eRoleAtt < ERA_AttA_Start || pEquip->equipSpec.ConsolidateEffect[j].eRoleAtt > ERA_End )
						{
							pEquip->equipSpec.ConsolidateEffect[j].eRoleAtt = pConFormula->RoleAttEffect[i].eRoleAtt;
							if( j < 3 )
								pEquip->equipSpec.ConsolidateEffect[j].nValue += pConFormula->RoleAttEffect[i].nValue * ( 1 + pConFormula->fcoefficientA * pEquip->equipSpec.byQuality * pEquip->pEquipProto->byLevel );
							else
								pEquip->equipSpec.ConsolidateEffect[j].nValue += pConFormula->RoleAttEffect[i].nValue;
							break;
						}
					}
				}
#else
				// ˳��ȡ����
				if( pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt != pConFormula->RoleAttEffect[i].eRoleAtt )
					pEquip->equipSpec.ConsolidateEffect[i].nValue = 0;

				if( pConFormula->RoleAttEffect[i].eRoleAtt <= ERA_Null || pConFormula->RoleAttEffect[i].eRoleAtt >= ERA_End )
					pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt = ERA_Null;
				if( pConFormula->RoleAttEffect[i].nValue < 0 )
					pEquip->equipSpec.ConsolidateEffect[i].nValue = 0;

				pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt = pConFormula->RoleAttEffect[i].eRoleAtt;
				if( pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt < ERA_AttA_Start || pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt > ERA_End )
					pEquip->equipSpec.ConsolidateEffect[i].nValue = 0;
				else
				{
					if( i < 3 )
						pEquip->equipSpec.ConsolidateEffect[i].nValue += pConFormula->RoleAttEffect[i].nValue * ( 1 + pConFormula->fcoefficientA * pEquip->equipSpec.byQuality * pEquip->pEquipProto->byLevel );
					else
						pEquip->equipSpec.ConsolidateEffect[i].nValue += pConFormula->RoleAttEffect[i].nValue;
				}
#endif
			}
		}
		else if( pEquip->pEquipProto->eTypeEx == p->eItemTypeEx && p->n16Times > level )
			break;
	}

	return re;
}

BOOL ItemCreator::CanLevelupFabao(tagFabao * pFabao,INT nPracticeValue)
{
	BOOL changed = FALSE;
	const INT nFabaoMaxStage = g_attRes.GetFabaoMaxStage();
	INT nRV = pFabao->nPracticeValue + nPracticeValue;
	for( int i = pFabao->n16Stage + 1; i <= nFabaoMaxStage; ++i )
	{
		const tagFabaoLevelProto * p = g_attRes.GetFabaoLevelProto(i);
		if( P_VALID(p) )
		{
			if( nRV >= p->nMinValue && nRV <= p->nMinValue + p->nLevelUpWhated )
			{
				changed = TRUE;
				break;
			}
			else if( nRV < p->nMinValue )
				break;
		}
	}
	return changed;
}

INT	ItemCreator::ModFabaoMood(tagFabao * pFabao,INT nMood)
{
	if( !P_VALID(pFabao) )
		return GT_INVALID;
	if( pFabao->n16Stage < 30 )
	{
		pFabao->nMood = 1000;
		return pFabao->nMood;
	}
	pFabao->nMood += nMood;
	if( pFabao->nMood < 0 )
		pFabao->nMood = 0;
	else if( pFabao->nMood > 2000 )
		pFabao->nMood = 2000;
	return pFabao->nMood;
}


static INT nFabaoMoodSection[][2] = 
{
	0	,	199,	// ����
	200	,	399,	// ����
	400	,	599,	// ��ɥ
	600	,	799,	// ʧ��
	800	,	999,	// ����
	1000,	2000	// ����
};
static FLOAT fFabaoMoodEffect[][3] = 
{
//  ս������ֳ� & �����ֳ� & ���齵���ٶ�
	0.5	,	0.6,	20,// ����
	0.6	,	0.8,	20,// ����
	0.7	,	0.85,	20,// ��ɥ
	0.8	,	0.9,	20,// ʧ��
	1.0	,	1.0,	20,// ����
	1.1,	1.1,	60 // ����
};

INT ItemCreator::GetFabaoMoodSection(ItemCreator::tagFabaoMoodSection * Mood,INT nSize,INT & num)
{
	//INT nCountOf = _countof(nFabaoMoodSection);
	num = sizeof(nFabaoMoodSection)/sizeof(nFabaoMoodSection[0]);
	if( num > nSize )
		return num;
	if( P_VALID(Mood) )
	{
		for( INT i = 0; i < num; ++i )
		{
			Mood[i].nMax = nFabaoMoodSection[i][1];
			Mood[i].nMin = nFabaoMoodSection[i][0];
		}
	}
	return num;
}

static INT GetIndexOfFabaoEffectByMoodValue(tagFabao* pFabao)
{
	if( pFabao->n16Stage < 30 )
		return GT_INVALID;

	INT i2size = sizeof(nFabaoMoodSection)/sizeof(nFabaoMoodSection[0]);

	for(int i = 0; i < i2size ; ++i)
	{
		if(pFabao->nMood >= nFabaoMoodSection[i][0] && pFabao->nMood <= nFabaoMoodSection[i][1])
			return i;
	}
	return GT_INVALID;
}

FLOAT ItemCreator::GetFabaoExpPercent(tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return 0;
	if( pFabao->n16Stage < 30 )
		return 1;
	int ndx = GetIndexOfFabaoEffectByMoodValue(pFabao);
	if( ndx != GT_INVALID )
	{
		return fFabaoMoodEffect[ndx][0];
	}
	return 0;
}
FLOAT ItemCreator::GetFabaoCapabilityPercent(tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return 0;
	if( pFabao->n16Stage < 30 )
		return 1;
	int ndx = GetIndexOfFabaoEffectByMoodValue(pFabao);
	if( ndx != GT_INVALID )
	{
		return fFabaoMoodEffect[ndx][1];
	}
	return 0;
}

INT ItemCreator::GetFabaoFeelingdown(tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return 0;
	if( pFabao->n16Stage < 30 )
		return 0;
	int ndx = GetIndexOfFabaoEffectByMoodValue(pFabao);
	if( ndx != GT_INVALID )
	{
		return fFabaoMoodEffect[ndx][2];
	}
	return 0;
}
DWORD ItemCreator::RealizeFabaoStunt(tagFabao * pFabao,INT nOldStage)
{
	if( !P_VALID(pFabao) )
		return 0;
	if( nOldStage == pFabao->n16Stage )
		return 0;
	if( P_VALID(pFabao->dwUnsureSkill) )
		return 0;
	INT nRnd = -1;
	FLOAT fPct = 0;
	INT nStunts = 0;
	set<DWORD> setAlready;
	for( INT i = 0; i < MaxFairySkillNum; ++i )
	{
		if( P_VALID(pFabao->dwFairySkill[i]) )
		{
			++nStunts;
			setAlready.insert(pFabao->dwFairySkill[i]);
		}
	}
	if( pFabao->n16Stage >= 60 && pFabao->n16Stage < 70 && nStunts <= 1 )
		fPct = 0.04 * ((FLOAT)pFabao->n16NativeIntelligence / 25.0f) * ((FLOAT)pFabao->n16NativeIntelligence / 25.0f);
	else if( pFabao->n16Stage >= 60 && pFabao->n16Stage < 70 && nStunts > 1 )
		fPct = 0.05 * ((FLOAT)pFabao->n16NativeIntelligence / 50.0f) * ((FLOAT)pFabao->n16NativeIntelligence / 50.0f);
	else if( pFabao->n16Stage >= 70 && pFabao->n16Stage < 80 && nStunts <= 2 )
		fPct = 0.03 * ((FLOAT)pFabao->n16NativeIntelligence / 25.0f) * ((FLOAT)pFabao->n16NativeIntelligence / 25.0f);
	else if( pFabao->n16Stage >= 70 && pFabao->n16Stage < 80 && nStunts > 2 )
		fPct = 0.08 * ((FLOAT)pFabao->n16NativeIntelligence / 75.0f) * ((FLOAT)pFabao->n16NativeIntelligence / 75.0f);
	else if( pFabao->n16Stage >= 80 && pFabao->n16Stage < 101 && nStunts <= 3 )
		fPct = 0.02 * ((FLOAT)pFabao->n16NativeIntelligence / 25.0f + 1) * ((FLOAT)pFabao->n16NativeIntelligence / 25.0f + 1);
	else if( pFabao->n16Stage >= 80 && pFabao->n16Stage < 101 && nStunts > 3 )
		fPct = 0.08 * ((FLOAT)pFabao->n16NativeIntelligence / 50.0f - 0.5) * ((FLOAT)pFabao->n16NativeIntelligence / 50.0f - 0.5);

	nRnd = fPct * 100;

	// ��������
	if( nRnd >= IUTIL->RandomInRange(1,100) )
	{
		TMap<DWORD,tagFabaoStuntProto *> & mapStunt = g_attRes.GetFabaoStuntProtos();
		TMap<DWORD,tagFabaoStuntProto *>::TMapIterator it = mapStunt.Begin();
		tagFabaoStuntProto * pStunt = NULL;
		vector<DWORD> vecStunts;
		for(; mapStunt.PeekNext(it,pStunt) ; )
		{
			if( pStunt->bCanRealized && pStunt->nIntelligenceLimit <= pFabao->n16NativeIntelligence && pStunt->nStageLimit <= pFabao->n16Stage && (setAlready.find(pStunt->dwID) == setAlready.end()) )
				vecStunts.push_back(pStunt->dwID);
		}
		if( vecStunts.size() > 0 )
		{
			pFabao->dwUnsureSkill = vecStunts[IUTIL->RandomInRange(0,vecStunts.size() - 1)];
		}

		return pFabao->dwUnsureSkill;
	}

	return 0;
}

VOID ItemCreator::RecalFaBaoCurStoredExp(tagFabao * pFabao)
{
	INT nMaxStoreExp = MFaBaoStoredMaxExp(pFabao->n16Stage, pFabao->n16NativeIntelligence);

	if (pFabao->nCurStoredExp > nMaxStoreExp)
		pFabao->nCurStoredExp = nMaxStoreExp;
}

DWORD ItemCreator::ResetEquipAtt(tagEquip * pEquip)
{
	if( !P_VALID(pEquip) || !P_VALID(pEquip->pProtoType) )
		return FALSE;
	const tagEquipQltyEffect *pEquipQltyEffect = g_attRes.GetEquipQltyEffect(pEquip->equipSpec.byQuality);
	if(!P_VALID(pEquipQltyEffect))
	{
		return FALSE;
	}
	if( !MIsFaBao(pEquip) )
	{
		memset(pEquip->equipSpec.nRoleAttEffect,0,sizeof(pEquip->equipSpec.nRoleAttEffect));
		CreateEquipRoleL1Att(pEquip,pEquip->pEquipProto,pEquipQltyEffect);
	}
	pEquip->equipSpec.nRoleL2AttCount = 0;
	CreateEquipRoleL2Att(pEquip,pEquip->pEquipProto);
	return TRUE;
}

#pragma warning(default:4244)

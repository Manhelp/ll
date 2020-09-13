//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_shop.h
// author: Sxg
// actor:
// data: 2008-10-28
// last:
// brief: �̵괦��
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_shop.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "role.h"
#include "creature.h"
#include "shop.h"
#include "city.h"
#include "city_mgr.h"

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ(��װ��)����ˢ����Ʒ�б�
//-----------------------------------------------------------------------------
DWORD Role::GetShopItems(DWORD dwNPCID, BYTE byShelf, DWORD dwNPCTypeID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = NULL;
	Shop *pShop = NULL;
	DWORD dwShopID = 0;
	
	if( dwNPCID > 0 )
	{
		// �ҵ�NPC�������Ϸ���
		pNPC = pMap->FindCreature(dwNPCID);
		if(!P_VALID(pNPC)
			|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
			|| !pNPC->CheckNPCTalkDistance(this))
		{
			return E_Shop_NPCNotValid;
		}

		// �ҵ��̵�
		pShop = pMap->GetShop(pNPC->GetTypeID());
		if(!P_VALID(pShop) || pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}

		dwShopID	= pNPC->GetShopID();
	}
	else
	{
		pShop = pMap->GetShop(dwNPCTypeID);
		if(!P_VALID(pShop) || pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}

		const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(dwNPCTypeID);
		if(P_VALID(pCreatureProto))
			dwShopID	= pCreatureProto->uFunctionID.dwShopID;
	}

	INT16 n16RareItemNum = pShop->GetRareGoodsNum(byShelf);
	
	// û��ˢ����Ʒ
	if(0 == n16RareItemNum)
	{
		tagNS_GetShopItems Send;
		Send.dwNPCID	= dwNPCID;
		Send.byShelf	= byShelf;
		Send.dwShopID	= dwShopID;
		Send.n16RareItemNum = 0;
		Send.dwNPCTypeID = dwNPCTypeID;

		SendMessage(&Send, Send.dwSize);
		return E_Success;
	}

	// ��ˢ����Ʒ
	INT32 nSzMsg = sizeof(tagNS_GetShopItems) - 1 + sizeof(tagMsgShopItem) * n16RareItemNum;

	MCREATE_MSG(pSend, nSzMsg, NS_GetShopItems);
	pSend->dwSize	= nSzMsg;
	pSend->dwNPCID	= dwNPCID;
	pSend->byShelf	= byShelf;
	pSend->dwShopID	= dwShopID;
	pSend->dwNPCTypeID = dwNPCTypeID;
	pSend->n16RareItemNum = n16RareItemNum;

	pShop->GetRareItems((tagMsgShopItem*)pSend->byData, n16RareItemNum, byShelf);
	
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ����װ��
//-----------------------------------------------------------------------------
DWORD Role::GetShopEquips(DWORD dwNPCID, BYTE byShelf, DWORD dwNPCTypeID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = NULL;
	Shop *pShop = NULL;
	DWORD dwShopID = 0;
	
	if( dwNPCID > 0 )
	{
		// �ҵ�NPC�������Ϸ���
		pNPC = pMap->FindCreature(dwNPCID);
		if(!P_VALID(pNPC)
			|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
			|| !pNPC->CheckNPCTalkDistance(this))
		{
			return E_Shop_NPCNotValid;
		}

		// �ҵ��̵�
		pShop = pMap->GetShop(pNPC->GetTypeID());
		if(!P_VALID(pShop) || !pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}
	}
	else
	{
		pShop = pMap->GetShop(dwNPCTypeID);
		if(!P_VALID(pShop) || pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}

		const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(dwNPCTypeID);
		if(P_VALID(pCreatureProto))
			dwShopID	= pCreatureProto->uFunctionID.dwShopID;
	}

	INT16 n16RareEquipNum = pShop->GetRareGoodsNum(byShelf);

	// û��ˢ����Ʒ
	if(0 == n16RareEquipNum)
	{
		tagNS_GetShopEquips Send;
		Send.dwNPCID		= dwNPCID;
		Send.byShelf		= byShelf;
		Send.dwShopID		= dwShopID;
		Send.dwNPCTypeID	= dwNPCTypeID;
		Send.n16EquipNum	= 0;

		SendMessage(&Send, Send.dwSize);
		return E_Success;
	}

	INT32 nSzMsg = sizeof(tagNS_GetShopEquips) - 1 + sizeof(tagMsgShopEquip) * n16RareEquipNum;

	MCREATE_MSG(pSend, nSzMsg, NS_GetShopEquips);
	pSend->dwSize	= nSzMsg;
	pSend->dwNPCID	= dwNPCID;
	pSend->byShelf	= byShelf;
	pSend->dwShopID	= dwShopID;
	pSend->dwNPCTypeID	= dwNPCTypeID;
	pSend->n16EquipNum = n16RareEquipNum;

	pShop->GetRareEquips((tagMsgShopEquip*)pSend->byData, byShelf, pSend->n16EquipNum);

	// ���¼����С
	if(pSend->n16EquipNum != n16RareEquipNum)
	{
		pSend->dwSize -= (n16RareEquipNum - pSend->n16EquipNum) * sizeof(tagMsgShopEquip);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ������Ʒ(��װ��)
//-----------------------------------------------------------------------------
DWORD Role::BuyShopItem(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT16 n16ItemNum,DWORD dwNPCTypeID)
{
	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = NULL;
	Shop *pShop = NULL;

	if( dwNPCID > 0 )
	{
		// �ҵ�NPC�������Ϸ���
		pNPC = pMap->FindCreature(dwNPCID);
		if(!P_VALID(pNPC)
			|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
			|| !pNPC->CheckNPCTalkDistance(this))
		{
			return E_Shop_NPCNotValid;
		}

		// �ҵ��̵�
		pShop = pMap->GetShop(pNPC->GetTypeID());
		if(!P_VALID(pShop) || pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}

		const CreatureScript * pScript = pNPC->GetScript  ();
		if( P_VALID(pScript) )
		{
			if( !pScript->IsShopOpened  ( pNPC,this,pMap ) )
			{
				return E_Shop_IsClosed;
			}
			const tagShopProto	* pShopProto = pShop->GetShopProto();
			if( !P_VALID(pShopProto) )
				return E_Shop_NotValid;

			if( !pScript->CanShopping(pNPC,this,pMap,pShopProto->dwID,dwTypeID) )
				return E_Shop_Cannot_Buy;
		}
	}
	else
	{
		// �ҵ��̵�
		pShop = pMap->GetShop(dwNPCTypeID);
		if(!P_VALID(pShop) || pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}
	}


	// �����ұ����Ƿ��еط�
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Shop_NotEnough_SpaceInBag;
	}

	// ͬ����Ʒ�����ﵽ����
	if (!GetItemMgr().CanAddMaxHoldItem(dwTypeID, n16ItemNum))
	{
		return E_Shop_ItemMaxHold;
	}

	tagSellItem SellItem;
	DWORD dwErrorCode = pShop->SellItem(this, byShelf, dwTypeID, n16ItemNum, SellItem);
	if(dwErrorCode != E_Success
		&& dwErrorCode != E_Shop_ItemNotEnough)
	{
		return dwErrorCode;
	}

	// ����ұ����п۳���Ǯ
	if(SellItem.nSilverCost > 0)
	{
		GetCurMgr().DecBagSilver(SellItem.nSilverCost, ELCID_Shop_BuyItem);

		if( P_VALID(m_pScript) )
			m_pScript->DecSilverByBuyItem( this, dwTypeID, SellItem.nSilverCost );

		//��˰
		if(SellItem.nTax > 0)
		{
			City* pCity = g_cityMgr.GetCity(pShop->GetCityID());
			if (P_VALID(pCity))
			{
				pCity->IncCityTaxation(SellItem.nTax);
			}
		}
	}

	// �۳���������
	if(SellItem.nSpecCost > 0)
	{
		if(ECCT_ItemExchange == SellItem.eCostType)
		{
			GetItemMgr().DelBagSameItem(SellItem.listItem, SellItem.nSpecCost, ELCID_Shop_BuyItem);
		}
		else if( ECCT_GuildContribe == SellItem.eCostType )
		{
			DWORD dwGuildID = GetGuildID();
			if( !P_VALID(dwGuildID) )
				return GT_INVALID;
			Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
			if( !P_VALID(pGuild) )
				return GT_INVALID;
			tagGuildMember* pMem = pGuild->GetMemberMgr().GetMember(GetID());
			if( !P_VALID(pMem) )
				return GT_INVALID;

			if( pMem->nContribution < SellItem.nSpecCost )
				return E_Shop_NotEnough_SpecCost;

			pGuild->ChangeMemberContribution(GetID(),SellItem.nSpecCost,FALSE);
		}
		else if(ECCT_GodSoul == SellItem.eCostType)
		{
			if( GetGod() < SellItem.nSpecCost)
			{
				return E_Shop_NotEnough_SpecCost;
			}
				
			ChangeSoul(-SellItem.nSpecCost,0);
		}
		else if(ECCT_MonsterSoul == SellItem.eCostType)
		{
			if( GetMonster() < SellItem.nSpecCost)
			{
				return E_Shop_NotEnough_SpecCost;
			}

			ChangeSoul( 0,-SellItem.nSpecCost );
		}
		else if(SellItem.eCostType != ECCT_Null)
		{
			GetCurMgr().DecCurrency(SellItem.eCostType, SellItem.nSpecCost, ELCID_Shop_BuyItem);
		}
		
	}

	// ��װ���ŵ���ұ�����
	if(P_VALID(SellItem.pItemSell))
	{
		GetItemMgr().Add2Bag(SellItem.pItemSell, ELCID_Shop_BuyItem, TRUE);
	}

	if(P_VALID(pNPC))
	{
		const CreatureScript * pScript1   = g_ScriptMgr.GetCreatureScript(pNPC->GetTypeID());
		if ( P_VALID(pScript1))
		{

			const tagShopProto	* pShopProto = pShop->GetShopProto();
			if( P_VALID(pShopProto) )
			{
				pScript1->OnShopping(pNPC,this,pMap,pShopProto->dwID,dwTypeID);
			}	
			
		}
	}

	// ���͸��º��̵���Ʒ -- ֻ��ˢ����ƷҪ������Ʒ����
	if(SellItem.n16RemainNum != GT_INVALID)
	{
		tagNS_BuyShopItem Send;
		Send.dwNPCID		= dwNPCID;
		Send.byShelf		= byShelf;
		Send.n16RemainNum	= SellItem.n16RemainNum;
		Send.dwTypeID		= dwTypeID;
		Send.byIndex		= SellItem.byIndex;
		Send.dwNPCTypeID	= dwNPCTypeID;

		SendMessage(&Send, Send.dwSize);
	}
	

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��װ��
//-----------------------------------------------------------------------------
DWORD Role::BuyShopEquip(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial, DWORD dwNPCTypeID)
{
	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = NULL;
	Shop *pShop = NULL;

	if( dwNPCID > 0 )
	{
		// �ҵ�NPC�������Ϸ���
		pNPC = pMap->FindCreature(dwNPCID);
		if(!P_VALID(pNPC)
			|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
			|| !pNPC->CheckNPCTalkDistance(this))
		{
			return E_Shop_NPCNotValid;
		}

		// �ҵ��̵�
		pShop = pMap->GetShop(pNPC->GetTypeID());
		if(!P_VALID(pShop) || !pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}

		const CreatureScript * pScript = pNPC->GetScript  ();

		if( P_VALID(pScript) )
		{
			if( !pScript->IsShopOpened  ( pNPC,this,pMap ) )
			{
				return E_Shop_IsClosed;
			}
			const tagShopProto	* pShopProto = pShop->GetShopProto();
			if( !P_VALID(pShopProto) )
				return E_Shop_NotValid;


			if( !pScript->CanShopping(pNPC,this,pMap,pShopProto->dwID,dwTypeID) )
				return E_Shop_Cannot_Buy;
		}
	}
	else
	{
		//pShop = pMap->GetShop(pNPC->GetTypeID());
		pShop = pMap->GetShop(dwNPCTypeID);
		if(!P_VALID(pShop) || !pShop->IsEquipShop())
		{
			return E_Shop_NotValid;
		}
	}


	// �����ұ����Ƿ��еط�
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Shop_NotEnough_SpaceInBag;
	}


	// ����Ƕһ�ʥ�飬�ж�ʥ�������Ƿ�ﵽ����
	tagEquipProto * pEquipProto = g_attRes.GetEquipProto(dwTypeID);

	// �����ս����Ʒ����鱳���Ƿ��еط�
	if( MIsWarItem(pEquipProto) && GetItemMgr().GetWarBagFreeSize() < 1)
	{
		return E_Shop_NotEnough_SpaceInBag;
	}

	if (P_VALID(pEquipProto) && MIsHolyType(pEquipProto->eType) && GetItemMgr().IsHolyExceed())
	{
		return E_Shop_Holy_CountLimit;
	}

	// ͬ����Ʒ�����ﵽ����
	if (!GetItemMgr().CanAddMaxHoldItem(dwTypeID, 1))
	{
		return E_Shop_ItemMaxHold;
	}

	tagSellEquip SellEquip;
	DWORD dwErrorCode = pShop->SellEquip(this, byShelf, dwTypeID, n64Serial, SellEquip);
	if(dwErrorCode != E_Success
		&& dwErrorCode != E_Shop_Equip_Sold
		&& dwErrorCode != E_Shop_CreateEquip_Failed)
	{
		return dwErrorCode;
	}

	// ����ұ����п۳���Ǯ
	if(SellEquip.nSilverCost > 0)
	{
		GetCurMgr().DecBagSilver(SellEquip.nSilverCost, ELCID_Shop_BuyEquip);
		if( P_VALID(m_pScript) )
			m_pScript->DecSilverByBuyItem( this, dwTypeID, SellEquip.nSilverCost );

		//��˰
		if(SellEquip.nTax > 0)
		{
			City* pCity = g_cityMgr.GetCity(pShop->GetCityID());
			if (P_VALID(pCity))
			{
				pCity->IncCityTaxation(SellEquip.nTax);
			}
		}
	}

	// �۳���������
	if(SellEquip.nSpecCost > 0)
	{
		if(ECCT_ItemExchange == SellEquip.eCostType)
		{
			GetItemMgr().DelBagSameItem(SellEquip.listItem, SellEquip.nSpecCost, ELCID_Shop_BuyEquip);
		}
		else if( ECCT_GuildContribe == SellEquip.eCostType )
		{
			DWORD dwGuildID = GetGuildID();
			if( !P_VALID(dwGuildID) )
				return GT_INVALID;
			Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
			if( !P_VALID(pGuild) )
				return GT_INVALID;
			tagGuildMember* pMem = pGuild->GetMemberMgr().GetMember(GetID());
			if( !P_VALID(pMem) )
				return GT_INVALID;

			if( pMem->nContribution < SellEquip.nSpecCost )
				return E_Shop_NotEnough_SpecCost;

			pGuild->ChangeMemberContribution(GetID(),SellEquip.nSpecCost,FALSE);
		}
		else if(ECCT_GodSoul == SellEquip.eCostType)
		{
			if( GetGod() < SellEquip.nSpecCost)
			{
				return E_Shop_NotEnough_SpecCost;
			}
				
			ChangeSoul(-SellEquip.nSpecCost,0);
		}
		else if(ECCT_MonsterSoul == SellEquip.eCostType)
		{
			if( GetMonster() < SellEquip.nSpecCost)
			{
				return E_Shop_NotEnough_SpecCost;
			}

			ChangeSoul( 0,-SellEquip.nSpecCost );
		}
		else if(ECCT_WarMoney == SellEquip.eCostType)
		{
			DWORD dwWarmoneyCost = GetCurMgr().GetWarMoney();
			if(dwWarmoneyCost < SellEquip.nSpecCost)
			{
				return E_Shop_NotEnough_WarMoney;
			}
			GetCurMgr().AddWarMoney(-SellEquip.nSpecCost);
		}
		else if(SellEquip.eCostType != ECCT_Null)
		{
			GetCurMgr().DecCurrency(SellEquip.eCostType, SellEquip.nSpecCost, ELCID_Shop_BuyEquip);
		}
	}

	// ��װ���ŵ���ұ�����
	if(P_VALID(SellEquip.pEquipSell))
	{
		tagItem *pItem = SellEquip.pEquipSell;
		GetItemMgr().Add2Bag(pItem, ELCID_Shop_BuyEquip, TRUE);

		// ��һ��һ�����ߵ��¼��ӿ�
		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
	}

	if(P_VALID(pNPC))
	{
		const CreatureScript * pScript1   = g_ScriptMgr.GetCreatureScript(pNPC->GetTypeID());
		if ( P_VALID(pScript1))
		{
			const tagShopProto	* pShopProto = pShop->GetShopProto();
			if( P_VALID(pShopProto) )
			{
				pScript1->OnShopping(pNPC,this,pMap,pShopProto->dwID,dwTypeID);
			}	
			
		}
	}

	// ������Ϊ��ͨ����
	if(GT_INVALID == SellEquip.n16RemainNum)
	{
		return dwErrorCode;
	}
	
	// ���͸��º��̵���Ʒ -- ֻ����ϡ������
	if(P_VALID(SellEquip.pEquipNew))
	{
		INT32 nMsgSz = sizeof(tagNS_BuyShopEquip) - 1 + SIZE_EQUIP;

		MCREATE_MSG(pSend, nMsgSz, NS_BuyShopEquip);
		pSend->dwNPCID		= dwNPCID;
		pSend->byShelf		= byShelf;
		pSend->n16RemainNum	= SellEquip.n16RemainNum;
		pSend->dwSize		= nMsgSz;
		pSend->byIndex		= SellEquip.byIndex;
		pSend->dwNPCTypeID  = dwNPCTypeID;
		
		IFASTCODE->MemCpy(pSend->byData, SellEquip.pEquipNew, SIZE_EQUIP);
		
		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}
	else
	{
		// ˢ��װ������û�д���װ�������¸���
		tagNS_BuyShopEquip Send;
		Send.dwNPCID		= dwNPCID;
		Send.byShelf		= byShelf;
		Send.n16RemainNum	= SellEquip.n16RemainNum;
		Send.byIndex		= SellEquip.byIndex;
		Send.dwNPCTypeID    = dwNPCTypeID;

		ASSERT(0 == SellEquip.n16RemainNum);

		SendMessage(&Send, Send.dwSize);

		GetShopEquips(dwNPCID,byShelf,dwNPCTypeID);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���̵������Ʒ&װ��
// Jason 2010-2-2 ������fix(5695)
//-----------------------------------------------------------------------------
DWORD Role::SellToShop(DWORD dwNPCID, INT64 n64Serial, DWORD dwNPCTypeID)
{
	// �����Ƿ����
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = NULL;
	Shop *pShop = NULL;

	if( dwNPCID > 0 )
	{
		// �ҵ�NPC�������Ϸ���
		pNPC = pMap->FindCreature(dwNPCID);
		if(!P_VALID(pNPC)
			|| !pNPC->IsFunctionNPC(EFNPCT_Shop) 
			|| !pNPC->CheckNPCTalkDistance(this))
		{
			return E_Shop_NPCNotValid;
		}

		// �ҵ��̵�
		pShop = pMap->GetShop(pNPC->GetTypeID());
		if(!P_VALID(pShop))
		{
			return E_Shop_NotValid;
		}

		const CreatureScript * pScript = pNPC->GetScript();
		if( P_VALID(pScript) && !pScript->IsShopOpened  ( pNPC,this,pMap ) )
		{
			return E_Shop_IsClosed;
		}
		//const tagShopProto	* pShopProto = pShop->GetShopProto();
		//if( !P_VALID(pShopProto) )
		//	return E_Shop_NotValid;

		//if( !pScript->CanShopping(pNPC,this,pMap,pShopProto->dwID,pItem->dwTypeID) )
		//	return E_Shop_Cannot_Buy;
	}
	else
	{
		// �ҵ��̵�
		pShop = pMap->GetShop(dwNPCTypeID);
		if(!P_VALID(pShop))
		{
			return E_Shop_NotValid;
		}
	}

	// �ҵ�������Ʒ
	tagItem *pItem = GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pItem))
	{
		pItem = GetItemMgr().GetWarBag().GetItem(n64Serial);
		if(!P_VALID(pItem))
		{
			return E_Shop_ItemNotFind;
		}
	}

	//if( dwNPCID > 0 )
	//{
	//	if( P_VALID(pScript) )
	//	{
	//		if( !pScript->IsShopOpened  ( pNPC,this,pMap ) )
	//		{
	//			return E_Shop_IsClosed;
	//		}
	//		//const tagShopProto	* pShopProto = pShop->GetShopProto();
	//		//if( !P_VALID(pShopProto) )
	//		//	return E_Shop_NotValid;

	//		//if( !pScript->CanShopping(pNPC,this,pMap,pShopProto->dwID,pItem->dwTypeID) )
	//		//	return E_Shop_Cannot_Buy;
	//	}
	//}

	// �Ƿ�ɳ���
	if(!GetItemMgr().CanSell(*pItem))
	{
		return E_Shop_ItemCannotSell;
	}

	// �����ۼ�//??û�м���˰��Ӱ��
	INT64 n64Price = pItem->pProtoType->nBasePrice * pItem->GetNum();
	if(MIsEquipment(pItem->dwTypeID))
	{
		FLOAT fFactor;
		MCalPriceFactor(fFactor, ((tagEquip*)pItem)->equipSpec.byQuality);
		n64Price = (INT64)(n64Price * fFactor);
		if(n64Price < 0)
		{
			ASSERT(0);
			n64Price = 0;
		}
	}

	// ������
	float fEarnRate = GetEarnRate() / 10000.0f;
	n64Price = (INT64)(n64Price * fEarnRate);

	if( GetCurMgr  ().GetBagSilver  () + n64Price > GetCurMgr  ().GetMaxBagSilver() )
		return E_Shop_WillBeFull_SilverInBag;

	// ���������ɾ����Ʒ
	tagItem *pItemTemp = GetItemMgr().GetBag().GetItem(n64Serial);
	if(P_VALID(pItemTemp))
	{
		const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
		if(P_VALID(pScript) && P_VALID(pItemTemp->pProtoType) )
		{
			pScript->DelItem( 4, pItemTemp->dwTypeID, pItemTemp->pProtoType->byQuality );
		}
	}

	DWORD dwErrorCode = GetItemMgr().DelFromBag(n64Serial, ELCID_Shop_Sell, GT_INVALID, TRUE);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}


	// ��һ�ý�Ǯ
	GetCurMgr().IncBagSilver(n64Price, ELCID_Shop_Sell);

	return E_Success;
}

//-----------------------------------------------------------------------------
// �̵���Ϣ����ṹ�������ͻ���
//-----------------------------------------------------------------------------
VOID Role::SendShopFeedbackMsg(DWORD dwErrorCode, DWORD dwNPCID)
{
	// �ɹ�Ҳ�᷵�أ��ͻ�����Ҫ����Ч����
	tagNS_FeedbackFromShop Send;
	Send.dwErrorCode	= dwErrorCode;
	Send.dwNPCID		= dwNPCID;

	SendMessage(&Send, Send.dwSize);
}
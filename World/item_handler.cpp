//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_handler.cpp
// author: Sxg
// actor:
// data: 2008-07-21
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- ��Ʒ/װ�����
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_loot.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_common.h"
#include "../ServerDefine/role_data.h"

#include "player_session.h"
#include "world_session.h"
#include "role.h"
#include "world.h"
#include "guild_mgr.h"
#include "role_mgr.h"
#include "item_creator.h"

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Equip);

	// ��Ϣ�Ϸ�����֤
	if(pRecv->ePosDst <= EEP_Start || pRecv->ePosDst >= EEP_End)
	{
		IMSG(_T("The equip position is invalid!"));
		return GT_INVALID;
	}

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��װ
	DWORD dwErrorCode = pRole->Equip(pRecv->n64Serial, pRecv->ePosDst);

	tagNS_Equip	send;
	send.n64Serial = pRecv->n64Serial;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUnequip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Unequip);
	
	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��װ
	DWORD dwErrorCode = pRole->Unequip(pRecv->n64Serial, pRecv->n16PosDst);
// 	if(E_Success == dwErrorCode)
// 	{
// 		return E_Success;
// 	}

	tagNS_Unequip send;
	send.n64Serial = pRecv->n64Serial;
	send.dwErrorCode = dwErrorCode;
	send.eType = pRecv->eType;

	// �������ͻ��˻�װ���
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����ֶԻ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSwapWeapon(tagNetCmd* pCmd)
{
	//MGET_MSG(pRecv, pCmd, NC_SwapWeapon);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��װ
	DWORD dwErrorCode = pRole->SwapWeapon();
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_SwapWeapon send;
	send.dwErrorCode = dwErrorCode;

	// �������ͻ��˻�װ���
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����װ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleIdentifyEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_IdentifyEquip);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->GetItemMgr().IdentifyEquip(pRecv->n64SerialReel, 
												pRecv->n64SerialEquip, ELCLD_Equip_Identify);
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_IdentifyEquip send;
	send.n64Serial = pRecv->n64SerialEquip;
	send.dwErrorCode = dwErrorCode;

	// �������ͻ��˻�װ���
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// ͬһ�������ƶ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeItemPos(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemPosChange);

	if(pRecv->n16Num < 0)
	{
		return GT_INVALID;
	}
	
	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode;
	switch(pRecv->eConType)
	{
	case EICT_Equip:
		return pRole->MoveRing(pRecv->n64Serial, pRecv->n16PosDst);
		break;
	case EICT_RoleWare:
		{
			if(pRecv->dwNPCID == GT_INVALID)
			{
				tagItem* pItem = NULL;
				TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

				while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
				{
					if (!P_VALID(pItem))
					{
						continue;
					}

					if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
					{
						dwErrorCode = E_Success;
						break;
					}

					dwErrorCode = E_UseItem_ItemNotExist;
				}				
			}
			else
			{
				// ���NPCIDΪ-1������ʹ������ֿ�,������֤npc
				dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
				
			}
			if(dwErrorCode != E_Success)
			{
				return dwErrorCode;
			}
		}
		break;

	case EICT_GuildWare:
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
		if(dwErrorCode == E_Success)
		{
			g_guildMgr.AddGuildWareEvent(pRole->GetID(), EVT_GuildWareMoveTo, pCmd->dwSize, pRecv);
		}
		return dwErrorCode;
		break;
	case EICT_Wardrobe:
		if (pRole->GetItemMgr().GetWardrobeContainer().CanMove(pRecv->n64Serial, pRecv->n16PosDst) != E_Success)
			return GT_INVALID;
		if (pRole->GetWardrobe().CanAddFashion(pRecv->n16PosDst, pRole->GetItemMgr().GetWardrobeContainer().GetItem(pRecv->n64Serial)))
			return GT_INVALID;
		break;
	}

	return pRole->GetItemMgr().Move(pRecv->eConType, pRecv->n64Serial, 
								pRecv->n16Num, pRecv->n16PosDst, ELCLD_Item_Move);
}

//-----------------------------------------------------------------------------
// �������ƶ���Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeItemPosEx(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemPosChangeEx);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ����ͽ�ɫ�ֿ��йأ������ж�NPC
	if(EICT_RoleWare == pRecv->eConTypeSrc || EICT_RoleWare == pRecv->eConTypeDst)
	{
		DWORD dwErrorCode = E_Success;
		
		if(!GT_VALID(pRecv->dwNPCID)) // ���NPCIDΪ-1������ʹ������ֿ�,������֤npc 
		{
			// �ж�����ֿ�������Ʒ�Ƿ����

			// ��������
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}
				
				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
		}	
		else
		{
			dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		}

		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}
		
// 		DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
// 		if(dwErrorCode != E_Success)
// 		{
// 			return dwErrorCode;
// 		}
	}

	// ����Ͱ��ɲֿ��йأ������ж�NPC
	if(EICT_GuildWare == pRecv->eConTypeSrc || EICT_GuildWare == pRecv->eConTypeDst)
	{
		DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
		if(dwErrorCode == E_Success)
		{
			if( pRecv->eConTypeDst == EICT_GuildWare )
			{
				tagItem * pItem = pRole->GetItemMgr().GetItemFromContainer(pRecv->n64Serial1,pRecv->eConTypeSrc);
				if( MIsFaBao(pItem) )
				{
					tagFabao * pFabao = (tagFabao*)pItem;
					if( pFabao->eFabaoStatus != EEFS_Normal )
						return E_UseItem_PosLimit;
				}

				if (MIsHoly(pItem))
				{
					return E_UseItem_PosLimit;
				}
			}
			g_guildMgr.AddGuildWareEvent(pRole->GetID(), EVT_GuildWareMove2Other, pCmd->dwSize, pRecv);
		}
		return dwErrorCode;
	}

	if (pRecv->eConTypeSrc == EICT_RoleWare && pRecv->eConTypeDst == EICT_Bag)
	{
		ItemContainer *pConSrc = pRole->GetItemMgr().GetContainer(pRecv->eConTypeSrc);
		if(P_VALID(pConSrc))
		{
			tagItem *pItem = pConSrc->GetItem(pRecv->n64Serial1);

			// ��һ��һ�����ߵ��¼��ӿ�
			if (P_VALID(pItem))
			{
				g_ScriptMgr.GetRoleScript()->OnObtainItem(pRole, pItem->dwTypeID, pItem->eCreateMode);
			}
		}
	}

	if (pRecv->eConTypeDst == EICT_Wardrobe)
	{
		DWORD dwErrorCode = pRole->GetItemMgr().GetWardrobeContainer().CanAdd2Wardrobe(pRecv->n64Serial1, pRole->GetItemMgr().GetBag(), pRecv->n16PosDst);
		if (dwErrorCode != E_Success)
		{
			if (pRecv->eConTypeSrc == EICT_Bag)
				return dwErrorCode;
			else
				return GT_INVALID;
		}
		ItemContainer *pConSrc = pRole->GetItemMgr().GetContainer(pRecv->eConTypeSrc);
		if(P_VALID(pConSrc))
		{
			tagItem *pItem = pConSrc->GetItem(pRecv->n64Serial1);
			if (!pRole->GetWardrobe().CanAddFashion(pRecv->n16PosDst, pItem))
				return GT_INVALID;
		}
	}
	else if (pRecv->eConTypeSrc == EICT_Wardrobe)
	{
		ItemContainer *pConSrc = pRole->GetItemMgr().GetContainer(pRecv->eConTypeSrc);
		if(P_VALID(pConSrc))
		{
			tagItem *pItem = pConSrc->GetItem(pRecv->n64Serial1);
			if (P_VALID(pItem))
			{
				if (CalLayerID(pItem->n16Index) == pRole->GetWardrobe().GetCurWareLayer())
					return GT_INVALID;
			}
		}
	}

	return pRole->GetItemMgr().Move2Other(pRecv->eConTypeSrc, pRecv->n64Serial1, 
									pRecv->eConTypeDst, pRecv->n16PosDst, ELCLD_Item_Move2Other);
}

//------------------------------------------------------------------------
// ��ָ��˳��������������Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleReorderItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemReorder);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ϣ�Ϸ��������
	if(pRecv->n16ItemNum <= 0 
		|| (pRecv->n16ItemNum > SPACE_ALL_BAG && pRecv->n16ItemNum > SPACE_ALL_WARE))
	{
		return GT_INVALID;
	}

	// ������Ϣ
	INT nSzMsg = sizeof(tagNS_ItemReorder) + (pRecv->n16ItemNum - 1) * sizeof(INT16);
	MCREATE_MSG(pSend, nSzMsg, NS_ItemReorder);

	// ����
	DWORD dwErrorCode = E_Success;
	if(GT_INVALID == pRecv->dwNPCID)	
	{
		if(pRecv->eContainerType == EICT_Bag)	// ����
		{
			pSend->eConType = EICT_Bag;
			dwErrorCode = pRole->GetItemMgr().ReorderBag(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
		else if (pRecv->eContainerType == EICT_RoleWare)
		{
			pSend->eConType = EICT_RoleWare;
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}

				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
			if(E_Success == dwErrorCode)
			{
				dwErrorCode = pRole->GetItemMgr().ReorderRoleWare(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
			}
		}
	}
	else	// �ֿ�
	{
		pSend->eConType = EICT_RoleWare;
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		if(E_Success == dwErrorCode)
		{
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWare(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
	}

	// ������Ϣ
	pSend->n16ItemNum = pRecv->n16ItemNum;
	pSend->dwErrorCode = dwErrorCode;

	if(dwErrorCode != E_Success)
	{
		pSend->dwSize = sizeof(tagNS_ItemReorder);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}
//------------------------------------------------------------------------
// ʹ�ü�¼ʽ���ͷ����ô�������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetTransportMap(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTransportMap);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	tagNS_SetTransportMap	sendclient;
	//���õ�
	sendclient.dwErrorCode = (E_SET_TRANSPORT_ERROR_CODE)pRole->SetItemTransportPoint(pRecv);
	//�ظ��ͻ���
	sendclient.dwTransportMapID = pRole->GetMapID();
	sendclient.vTransportPoint = pRole->GetCurPos();
	SendMessage(&sendclient, sendclient.dwSize);

	//���ݿ�
	if(sendclient.dwErrorCode == EST_SuccessSet)
	{
		tagNDBC_UpdateRoleItemTransport senddb;
		senddb.dwRoleID	= pRole->GetID();
		senddb.dwMapID	= pRole->GetMapID();
		senddb.fX		= sendclient.vTransportPoint.x;
		senddb.fZ		= sendclient.vTransportPoint.z;
		senddb.fY		= sendclient.vTransportPoint.y;
		g_dbSession.Send(&senddb, senddb.dwSize);
	}
	return E_Success;
}
//------------------------------------------------------------------------
// ʹ�ü�¼ʽ���ͷ�����
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUseTransportMap(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseTransportMap);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	tagNS_UseItem send;
	//ʹ�ü�¼ʽ���ͷ�
	send.dwErrorCode = pRole->UseNoteTakingItemTransport(pRecv);

	//����
	send.dwSrcRoleID	= pRole->GetID();
	send.dwTarRoleID	= pRole->GetID();
	send.n64ItemID		= pRecv->n64ItemID;
	send.dwTypeID		= GT_INVALID;
	send.dwSerial		= 0;
	send.bInmmediate	= true;

	SendMessage(&send, send.dwSize);

	return E_Success;
}


DWORD PlayerSession::HandleRoleReorderItemEx(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemReorderEx);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��Ϣ�Ϸ��������
	if(pRecv->n16ItemNum <= 0 
		|| (pRecv->n16ItemNum > SPACE_ALL_BAG && pRecv->n16ItemNum > SPACE_ALL_WARE))
	{
		return GT_INVALID;
	}

	// ������Ϣ
	INT nSzMsg = sizeof(tagNS_ItemReorderEx) + pRecv->n16ItemNum * sizeof(tagItemOrder) - sizeof(BYTE);
	MCREATE_MSG(pSend, nSzMsg, NS_ItemReorderEx);

	// ����
	DWORD dwErrorCode = E_Success;
	if( GT_INVALID == pRecv->dwNPCID)	// ����
	{
		if( pRecv->eContainerType == EICT_Bag )
		{
			pSend->eConType = EICT_Bag;
			dwErrorCode = pRole->GetItemMgr().ReorderBagEx(pRecv->byData, pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
		else if( pRecv->eContainerType == EICT_RoleWare )		//����ֿ�
		{
			pSend->eConType = EICT_RoleWare;
			pSend->eConType = EICT_RoleWare;
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}

				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
			if(E_Success == dwErrorCode)
			{
				dwErrorCode = pRole->GetItemMgr().ReorderRoleWareEx(pRecv->byData, pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
			}
		}
	}
	else	// �ֿ�
	{
		pSend->eConType = EICT_RoleWare;
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		if(E_Success == dwErrorCode)
		{
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWareEx(pRecv->byData, 
							pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
	}

	// ������Ϣ
	pSend->dwErrorCode = dwErrorCode;

	if(pSend->n16ItemNum != pRecv->n16ItemNum)
	{
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> maybe modified bag order msg!\n"), pRole->GetID());
		ASSERT(pSend->n16ItemNum == pRecv->n16ItemNum);

		pSend->dwSize = sizeof(tagNS_ItemReorderEx) + pSend->n16ItemNum * sizeof(tagItemOrder) - sizeof(BYTE);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ʰȡ��Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePickUpItem(tagNetCmd* pCmd)
{
	tagNC_RolePickUpItem* pRecv = (tagNC_RolePickUpItem*)pCmd;

	if( !P_VALID(GetRole()) )
		return GT_INVALID;

	DWORD dwTypeID = 0;
	DWORD dwRtv =GetRole()->CanPickUpItem(pRecv->n64Serial);
	if (E_Success == dwRtv)
	{
		dwRtv = GetRole()->PickUpItem(pRecv->n64Serial, dwTypeID);
	}
	tagNS_RolePickUpItem send;
	send.dwRoleID		= GetRole()->GetID();
	send.n64Serial		= pRecv->n64Serial;
	send.dwErrorCode	= dwRtv;
	SendMessage(&send, send.dwSize);

	if (E_Success == dwRtv)
	{
		const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(dwTypeID);
		if (P_VALID(pItemScript))
		{
			pItemScript->OnPickUpItem( dwTypeID, GetRole()->GetID());
		}
	}

	return 0;
}

//------------------------------------------------------------------------
// �ӵ���Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePutDownItem(tagNetCmd* pCmd)
{
	tagNC_RolePutDownItem* pRecv = (tagNC_RolePutDownItem*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
		return GT_INVALID;

	BOOL bWarBag = FALSE;
	tagItem* pItem = pRole->GetItemMgr().GetBag().GetItem(pRecv->n64Serial);
	if( !P_VALID(pItem) )
	{
		pItem = pRole->GetItemMgr().GetWarBag().GetItem(pRecv->n64Serial);
		if( !P_VALID(pItem) )
			return GT_INVALID;

		bWarBag = TRUE;
	}

	DWORD dwErrorCode = E_Success;

	// �ű��ж���Ʒ�Ƿ���Ա�ɾ��
	const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(pItem->dwTypeID);
	if (P_VALID(pItemScript))
	{
		if (pItemScript->ItemCanPutDown(pItem->dwTypeID, pItem->n64Serial) != E_Success)
			dwErrorCode = E_SystemError;
	}

	tagNS_RolePutDownItem send;
	if (dwErrorCode != E_Success || pItem->pProtoType->DiscardEvent == EDE_DiscardNot)
	{
		send.dwErrorCode = dwErrorCode;
	}
	else
	{
		INT64 n64GroupID = 0;
		BOOL bAdd2Ground = pItem->pProtoType->DiscardEvent == EDE_DiscardGround;
		if(pItem->IsBind()) // ���õ����Ѱ󶨣�DeadLoot = 0�����������̴ݻ٣�Ч��ͬDeadLoot = 1
			bAdd2Ground = FALSE;

		if( bWarBag )
		{
			send.dwErrorCode = pRole->PutDownWarItem(pRecv->n64Serial, n64GroupID, bAdd2Ground);
		}
		else
		{
			send.dwErrorCode = pRole->PutDownItem(pRecv->n64Serial, n64GroupID, bAdd2Ground);
		}
	}

	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------
// ���Ҽ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_SetBagPsd);
	
	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BagPsd send;
	
	if(IsHaveBagPsd())	// ������
	{
		send.dwErrorCode = E_BagPsd_Exist;
	}
	else	// δ���ã�����
	{
		SetBagPsd(p->dwBagPsdCrc);
		send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
		send.dwErrorCode	= E_Success;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ȡ�����Ҽ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUnsetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_UnsetBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(!IsHaveBagPsd())
	{
		send.dwErrorCode = E_BagPsd_NoExist;
	}
	else
	{
		if(GetSafeCode() != p->dwSafeCodeCrc)
		{
			send.dwErrorCode = E_BagPsd_SafeCode_Error;
		}
		else
		{
			SetBagPsd(GT_INVALID);
			send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
			send.dwErrorCode = E_Success;
		}
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// ��֤��������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCheckBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_OldBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;
	send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
	send.dwErrorCode	= E_BagPsd_OK;

	if(GetBagPsd() != p->dwOldBagPsdCrc)
	{
		send.dwErrorCode = E_BagPsd_Error;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// �޸���������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleResetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_ResetBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(!IsHaveBagPsd())
	{
		send.dwErrorCode = E_BagPsd_NoExist;
	}
	else
	{
		if(GetBagPsd() != p->dwOldBagPsdCrc)
		{
			send.dwErrorCode = E_BagPsd_Error;
		}
		else if(p->dwOldBagPsdCrc == p->dwNewBagPsdCrc)
		{
			send.dwErrorCode = GT_INVALID;
		}
		else
		{
			SetBagPsd(p->dwNewBagPsdCrc);
			send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
			send.dwErrorCode = E_Success;
		}
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// �򿪱�������Ҫ��������
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleOpenBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_OpenBagPsd);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(IsHaveBagPsd() && GetBagPsd() != p->dwBagPsdCrc)
	{
		send.dwErrorCode = E_BagPsd_Error;
	}
	else
	{
		pRole->SetRoleStateEx(ERSE_BagPsdPass, TRUE);
		send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
		send.dwErrorCode	= E_Success;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// �����������뼰���������Ϣ
//------------------------------------------------------------------------
VOID PlayerSession::SetBagPsd(DWORD dwNewPsdCrc)
{
	m_sAccountCommon.dwBagPsdCrc = dwNewPsdCrc;

	// ��dbserver����Ϣ
	tagNDBC_ChangeBagPsd send;
	send.dwAccountID = GetSessionID();
	send.dwNewBagPsd = dwNewPsdCrc;
	g_dbSession.Send(&send, send.dwSize);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return;
	}
	
	// �������������ý�ɫ״̬
	pRole->m_RoleStateEx.SetState(ERSE_BagPsdPass);
	pRole->m_RoleStateEx.SetState(ERSE_BagPsdExist);

	if(GT_INVALID == dwNewPsdCrc)
	{
		pRole->m_RoleStateEx.UnsetState(ERSE_BagPsdExist);
	}
}

//------------------------------------------------------------------------
// ���������ҵ�װ����Ϣ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSomeoneEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetRemoteRoleEquipInfo);

	// ��ȡ��ɫ
	Role *pMyRole = GetRole();
	if(!P_VALID(pMyRole))
		return GT_INVALID;

	DWORD dwRoleID = pRecv->dwRoleID;
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole)) 
	{
		tagNS_GetOfflineEquipInfo send;
		SendMessage(&send, send.dwSize);

		return GT_INVALID; 
	}

	DWORD dwErrorCode = E_Success;
//#if 0	// �߻��������󣬲鿴���װ�����������˽����Ӱ��
	//�����ѿɼ�

	if(pRole->GetSession()->GetPersonalSet().eInfoOpenType == EPIS_Friend)
	{
		tagFriend* pFriRole = pRole->GetFriendPtr(pMyRole->GetID());
		if( !P_VALID(pFriRole) )
		{
			dwErrorCode = ECRI_E_FriendOnly;
		}
	}
	//��˽
	else if(pRole->GetSession()->GetPersonalSet().eInfoOpenType == EPIS_Private)
	{
		dwErrorCode = ECRI_E_Private;
	}
//#endif

// 	if (dwErrorCode != E_Success)
// 	{
// 		tagNS_GetRemoteRoleEquipInfo send;
// 		send.dwErrorCode = dwErrorCode;
// 		SendMessage(&send, send.dwSize);
// 
// 		return GT_INVALID;
// 	}
	INT nIndex = 0;
	INT nMaxIndex = EEP_MaxEquip;
	BYTE* pTemp = NULL;

	INT nMaxSize = (sizeof(tagFabao)+1)*nMaxIndex + sizeof(tagNS_GetRemoteRoleEquipInfo);
	MCREATE_MSG(pSend, nMaxSize,NS_GetRemoteRoleEquipInfo);

	pSend->dwRoleID = pRecv->dwRoleID;
	pSend->nEquipNum = 0;
	pTemp = pSend->byEquip;

	pSend->dwErrorCode = dwErrorCode;

	for(; nIndex < nMaxIndex; ++nIndex)
	{	
		tagEquip* pEquipInfo = pRole->GetItemMgr().GetEquipBarEquip((INT16)nIndex);
		if (!P_VALID(pEquipInfo)) continue;

		(*pTemp++) = (BYTE) pEquipInfo->pEquipProto->eType;
		if(pEquipInfo->IsFaBao())
		{
			memcpy(pTemp, (tagFabao*)pEquipInfo, sizeof(tagFabao));
			pTemp += sizeof(tagFabao);
		}
		else if(MIsSoulCrystal(pEquipInfo))
		{
			memcpy(pTemp, (tagSoulCrystal*)pEquipInfo, sizeof(tagSoulCrystal));
			pTemp += sizeof(tagSoulCrystal);
		}
		else if(MIsHoly(pEquipInfo))
		{
			// ����ǽ���ʥ�飬��װ��ʥ�ƣ�������TypeId�ͳ��ܴ����滻 �����д�ŵ�ʥ��64λId
			tagHolyMan *pHoly = (tagHolyMan *)pEquipInfo;

			tagHolyMan tmpHoly;
			IFASTCODE->MemCpy(&tmpHoly, pHoly, sizeof(tagHolyMan));

			for (int i=0; i< MaxHolyEquipNum; i++)
			{
				if (tmpHoly.n64EquipSerialIDs[i] != 0)
				{
					tagHolyEquip * pHolyEquip = (tagHolyEquip*)pRole->GetItemMgr().GetHolyManBarItem(tmpHoly.n64EquipSerialIDs[i]);
					if (P_VALID(pHolyEquip))
					{
						INT64 nTmpType = (INT64)pHolyEquip->dwTypeID;
						tmpHoly.n64EquipSerialIDs[i] = nTmpType * 1000 + pHolyEquip->n16EnhanceCount;										
					}
				}
			}	
			memcpy(pTemp, &tmpHoly, sizeof(tagHolyMan));
			pTemp += sizeof(tagHolyMan);
		}
		else if(MIsHolyEquip(pEquipInfo))
		{
			memcpy(pTemp, (tagHolyEquip*)pEquipInfo, sizeof(tagHolyEquip));
			pTemp += sizeof(tagHolyEquip);
		}
		else
		{
			memcpy(pTemp, pEquipInfo, sizeof(tagEquip));
			pTemp += sizeof(tagEquip);			
		}

		(pSend->nEquipNum)++;
		pEquipInfo = NULL;
	}

	(*pTemp) = _T('\0');
	pSend->dwSize = sizeof(tagNS_GetRemoteRoleEquipInfo) + (pTemp-pSend->byEquip);

	SendMessage(pSend, pSend->dwSize);

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (P_VALID(pRoleInfo) && P_VALID(pRole))
	{
		tagRoleInfo* pThisRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		if (!P_VALID(pThisRoleInfo))
		{
			return GT_INVALID;
		}

		tagRoleInfo* pMyInfo = g_roleMgr.GetRoleInfo(pMyRole->GetID());
		if (!P_VALID(pMyInfo))
		{
			return GT_INVALID;
		}

		tagNS_SomeoneViewYourEquip send;
		memcpy_s(send.szRoleName, sizeof(send.szRoleName), pMyInfo->szRoleName, sizeof(pMyInfo->szRoleName));
		pRole->SendMessage(&send, send.dwSize);
	}

	MDEL_MSG(pSend);
	return E_Success;
}

//-----------------------------------------------------------------------------
// ������ֿ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSideWareOpen(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_WareOpen);

	DWORD dwRet = E_Success;

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_WareOpen	send;
	send.dwErrorCode = pRole->OpenSideWare(pRecv->n64ItemID);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

DWORD PlayerSession::HandleSyncFaBaoStoreExpInfo(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SyncFaBaoStoreExpInfo);
	
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	tagFabao* pFaBao = (tagFabao*)pRole->GetItemMgr().GetEquipBar().GetItem(static_cast<INT16>(EEP_Face));
	if (!P_VALID(pFaBao))
		return E_FabaoStoreExp_NoFabao;

	// �����������
	if (pFaBao->n16Stage < MIN_STORE_EXP_FABAO_LEVEL)
		return E_FabaoStoreExp_LvlLimit;

	pFaBao->bStoredExpOpen = pRecv->bStoreExpOpen ? TRUE:FALSE;
	
	tagNS_SyncFaBaoStoreExpInfo send;
	send.bStoreExpOpen = pFaBao->bStoredExpOpen;
	send.nCurStoreExp = pFaBao->nCurStoredExp;
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleExpBall2ExpPilular(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ExpBall2ExpPilular);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwRet = E_Success;

	tagFabao* pFaBao = (tagFabao*)pRole->GetItemMgr().GetEquipBar().GetItem(static_cast<INT16>(EEP_Face));
	if (!P_VALID(pFaBao))
	{
		dwRet = E_FabaoStoreExp_NoFabao;
		goto error;
	}

	// �����������
	if (pFaBao->n16Stage < MIN_STORE_EXP_FABAO_LEVEL)
	{
		dwRet = E_FabaoStoreExp_LvlLimit;
		goto error;
	}

	// ����Ƿ���ھ�����
 	tagItem* pExpBall = pRole->GetItemMgr().GetBag().GetItem(pRecv->n64ExpBallSerialID);
	if (!P_VALID(pExpBall))
	{
		dwRet = E_FabaoStoreExp_NoExpBall;
		goto error;
	}
	else if (!P_VALID(pExpBall->pProtoType))
	{
		IMSG(_T("Invalid ExpBall Proto!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	if (pExpBall->pProtoType->eSpecFunc != EISF_ExpBall)
	{
		IMSG(_T("Invalid ExpBall!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	BOOL bExpBallBinded = (pExpBall->pProtoType->nSpecFuncVal1 == 1)?TRUE:FALSE;
	DWORD dwBindedRoleID = pExpBall->dwSpecVal1;

	// ����ӡ�ľ����Ƿ��㹻����ˮ��֮��
	tagItemProto* pItemProto = g_attRes.GetItemProto(pRecv->dwExpPilularID);
	if (!P_VALID(pItemProto))
	{
		IMSG(_T("ExpPilular not exist!!typeid=%d\r\n"), pRecv->dwExpPilularID);
		return GT_INVALID;
	}
	if (pItemProto->eSpecFunc != EISF_ExpPilular)
	{
		IMSG(_T("Invalid ExpPilular!!typeid=%d\r\n"), pRecv->dwExpPilularID);
		return GT_INVALID;
	}
	if (pItemProto->nSpecFuncVal1 > pFaBao->nCurStoredExp)
	{
		dwRet = E_FabaoStoreExp_NoEnoughExp;
		goto error;
	}

	// ��鱳���ռ�
	if (pRole->GetItemMgr().GetBagFreeSize() <= 0)
	{
		dwRet = E_FabaoStoreExp_BagFull;
		goto error;
	}

	// �۳�ˮ����
	pRole->GetItemMgr().DelFromBag(pRecv->n64ExpBallSerialID, ELCID_Fabao_StoreExp_Costed, 1);

	// �۳���ӡ����
	pFaBao->nCurStoredExp -= pItemProto->nSpecFuncVal1;

	// ����ˮ��֮��
	tagItem* pExpPilular = ItemCreator::Create(EICM_ExpPilularBind, pRole->GetID(), pRecv->dwExpPilularID);
	if (!P_VALID(pExpPilular))
	{
		IMSG(_T("Create ExpPilularItem Failed!!!typeid=%d,roleid=%d\r\n"), pRecv->dwExpPilularID, pRole->GetID());
		return GT_INVALID;
	}
	
	// ���ˮ�����ǰ󶨵ģ����ɵ�ˮ��֮��Ҳ��Լ����
	if (bExpBallBinded == TRUE)
		pExpPilular->dwSpecVal1 = dwBindedRoleID;

	pRole->GetItemMgr().UpdateFabaoSpec(*pFaBao);

	pRole->GetItemMgr().Add2Bag(pExpPilular, ELCID_Fabao_StoreExp_Created, TRUE);
error:
	tagNS_ExpBall2ExpPilular send;
	send.dwErrorCode = dwRet;
	pRole->SendMessage(&send, send.dwSize);

	return dwRet;
}

DWORD PlayerSession::HandleExpBallBind2Role(tagNetCmd * pCmd)	
{
	MGET_MSG(pRecv, pCmd, NC_ExpBallBind2Role);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwRet = E_Success;

	tagFabao* pFaBao = (tagFabao*)pRole->GetItemMgr().GetEquipBar().GetItem(static_cast<INT16>(EEP_Face));
	if (!P_VALID(pFaBao))
	{
		dwRet = E_FabaoStoreExp_NoFabao;
		goto error;
	}

	// �ȼ���ǲ���δԼ����ˮ����
	tagItem* pExpBall = pRole->GetItemMgr().GetBag().GetItem(pRecv->n64ExpBallSerialID);
	if (!P_VALID(pExpBall))
	{
		dwRet = E_FabaoStoreExp_NoExpBall;
		goto error;
	}
	else if (!P_VALID(pExpBall->pProtoType))
	{
		IMSG(_T("Invalid ExpBall Proto!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	if (pExpBall->pProtoType->eSpecFunc != EISF_ExpBall)
	{
		IMSG(_T("Invalid ExpBall!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	if (pExpBall->dwSpecVal1 != 0 && pExpBall->dwSpecVal1 != GT_INVALID)
	{
		IMSG(_T("ExpBall already binded!!typeid=%d"), pExpBall->dwTypeID);
		dwRet = E_FabaoStoreExp_BallBinded;
		goto error;
	}

	// ���ָ����ˮ����[Լ��]�Ƿ����
	tagItemProto* pItemProto = g_attRes.GetItemProto(pExpBall->pProtoType->nSpecFuncVal2);
	if (!P_VALID(pItemProto))
	{
		IMSG(_T("binded ExpBall Proto not exist!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	
	// �жϱ����Ƿ��㹻
	if (pRole->GetItemMgr().GetBagFreeSize() <= 0)
	{
		dwRet = E_FabaoStoreExp_BagFull;
		goto error;
	}

	DWORD dwExpBallTypeID = pExpBall->pProtoType->nSpecFuncVal2;
	// ɾ��ԭ�е�ˮ����
	pRole->GetItemMgr().DelFromBag(pRecv->n64ExpBallSerialID, ELCID_Fabao_StoreExp_Costed, 1);

	// �����µ�ˮ����[Լ��]����ִ��Լ������
	tagItem* pNewExpBall = ItemCreator::Create(EICM_ExpPilularBind, pRole->GetID(), dwExpBallTypeID);
	if (!P_VALID(pNewExpBall))
	{
		IMSG(_T("Create ExpBallItem Failed!!!typeid=%d,roleid=%d\r\n"), pRecv->n64ExpBallSerialID, pRole->GetID());
		return GT_INVALID;
	}
	pNewExpBall->dwSpecVal1 = pRole->GetID();
	pRole->GetItemMgr().Add2Bag(pNewExpBall, ELCID_Fabao_StoreExp_Created, TRUE);

error:
	tagNS_ExpBallBind2Role send;
	send.dwErrorCode = dwRet;
	pRole->SendMessage(&send, send.dwSize);

	return dwRet;
}

DWORD	PlayerSession::HandleSetPickupMode(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PickItemStrategy);

	// ��ȡ����
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;
	pRole->PickupMode(pRecv->dwStrategy);
	return E_Success;
}


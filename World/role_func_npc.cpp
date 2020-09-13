//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_func_npc.cpp
// author: Sxg
// actor:
// data: 2008-11-10
// last:
// brief: ����ְ��NPC���ܴ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "role.h"
#include "creature.h"
#include "map_creator.h"
#include "../WorldDefine/msg_func_npc.h"
#include "../WorldDefine/func_npc_define.h"
#include "../WorldDefine/msg_map.h"
#include "../ServerDefine/log_cmdid_define.h"

//---------------------------------------------------------------------------------
// ����ְ��NPC����
//---------------------------------------------------------------------------------
DWORD Role::ProcDak(DWORD dwNPCID, INT32 nIndex, DWORD dwMapID)
{
	// �������Ƿ��ڲ����ƶ�״̬
	if(IsInStateCantMove() || IsInRoleState(ERS_Commerce))
	{
		return E_Role_CantMove;
	}
	
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC))
	{
		return E_Dak_NPCNotFound;
	}
	
	if(!pNPC->IsFunctionNPC(EFNPCT_Dak))
	{
		return E_Dak_NPCNotValid;
	}

	if(!pNPC->CheckNPCTalkDistance(this))
	{
		return E_Dak_TooFar;
	}

	// �ҵ���վ
	const tagDakProto *pDak = g_attRes.GetDakProto(pNPC->GetDakID());
	if(!P_VALID(pDak))
	{
		ASSERT(P_VALID(pDak));
		return E_Dak_NotExist;
	}

	// ���Ŀ�ĵ�
	if(pDak->dakSite[nIndex].dwMapID != dwMapID)
	{
		return E_Dak_TargetMap_Invalid;
	}

	// �������Ʒ 2010-07-21 ����һ���������ʹ�����ʯ  ECCT_TransferStone= 20 ,����������� spec_func= EISF_TransferStone 78
	if (pDak->dakSite[nIndex].eCostType==ECCT_TransferStone){

		tagItem* pItem = GetItemMgr().GetBag().GetOneSpecItemBySpecFunEnum(EISF_TransferStone);
		if( !P_VALID(pItem) || ( pItem->pProtoType->eSpecFunc != EISF_TransferStone ) ){
			return E_Dak_NoStone;
		}
			 
	} else{
		if(!GetCurMgr().IsEnough(pDak->dakSite[nIndex].eCostType, pDak->dakSite[nIndex].nCostVal))
		{
			return E_Dak_NotEnough_Money;
		}
	}


	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
	{
		if (!pRoleScript->CanUseDakNPC(this, dwMapID, pDak->dakSite[nIndex].fX, pDak->dakSite[nIndex].fY, pDak->dakSite[nIndex].fZ))
			return GT_INVALID;
	}
	
	// ����
	if(!GotoNewMap(dwMapID, pDak->dakSite[nIndex].fX, pDak->dakSite[nIndex].fY, pDak->dakSite[nIndex].fZ))
	{
		return E_Dak_ChangeMap_Failed;
	}

	// �۳�����Ʒ 2010-07-21 ����һ���������ʹ�����ʯ  ECCT_TransferStone= 20 ,����������� spec_func=78

	if (pDak->dakSite[nIndex].eCostType==ECCT_TransferStone){

		tagItem* pItem = GetItemMgr().GetBag().GetOneSpecItemBySpecFunEnum(EISF_TransferStone);
		if( P_VALID(pItem) && ( pItem->pProtoType->eSpecFunc == EISF_TransferStone ) ){
			GetItemMgr().ItemUsedFromBag(pItem->n64Serial, (INT16)1, (DWORD)ELCLD_Item_Use);
		} 


	} else{
		GetCurMgr().DecCurrency(pDak->dakSite[nIndex].eCostType, pDak->dakSite[nIndex].nCostVal, ELCLD_Dak);
	}
	return E_Success;   
}

//------------------------------------------------------------------------------
// ���ø������
//------------------------------------------------------------------------------
DWORD Role::SetRebornMap( DWORD dwNPCID, DWORD &dwBornMapID, Vector3 &vBornPos )
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	if(pMap->GetMapInfo()->eType != EMT_Normal)
	{
		ASSERT(pMap->GetMapInfo()->eType == EMT_Normal);
		return E_BindRebornMap_MapInvalid;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC))
	{
		return E_BindRebornMap_NPCNotFound;
	}

	if(!pNPC->IsFunctionNPC(EFNPCT_Revive))
	{
		return E_BindRebornMap_NPCInvalid;
	}

	if(!pNPC->CheckNPCTalkDistance(this))
	{
		return E_BindRebornMap_TooFar;
	}

	if(GetRebornMapID() == pMap->GetMapID())
	{
		return E_BindRebornMap_Already;
	}

	// ����
	m_dwRebornMapID = pMap->GetMapID();

	dwBornMapID = m_dwRebornMapID;
	vBornPos = g_mapCreator.GetReBornPoint(m_dwRebornMapID);

	return E_Success;
}

//---------------------------------------------------------------------------------
// ʹ��ĥʯ
//---------------------------------------------------------------------------------
DWORD Role::AbraseWeapon(INT64 n64AbraserSerial, INT64 n64WeaponSerial, OUT INT32 &nAttackTimes)
{
	nAttackTimes = 0;

	tagItem *pItem = GetItemMgr().GetBagItem(n64AbraserSerial);
	if(!P_VALID(pItem))
	{
		return GT_INVALID;
	}

	if(pItem->pProtoType->eSpecFunc != EISF_Grind)
	{
		return GT_INVALID;
	}

	BOOL bEquiped = FALSE;

	tagItem *pEquip = GetItemMgr().GetBagItem(n64WeaponSerial);
	if(P_VALID(pEquip))
	{
		bEquiped = FALSE;
	}
	else
	{
		pEquip = GetItemMgr().GetEquipBarEquip(n64WeaponSerial);
		if(P_VALID(pEquip))
		{
			bEquiped = TRUE;
		}
		else
		{
			return GT_INVALID;
		}
	}

	if(!MIsEquipment(pEquip->dwTypeID))
	{
		return GT_INVALID;
	}

	MTRANS_POINTER(p, pEquip, tagEquip);
	if(0 == p->nUseTimes)
	{
		return GT_INVALID;
	}

	INT nNewnessChange = pItem->pProtoType->nSpecFuncVal1;
	
	// ����ĥʯ
	DWORD dwErrorCode = GetItemMgr().ItemUsedFromBag(n64AbraserSerial, 1, ELCLD_Item_Use);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ����ո�¶�
	p->DecAttackTimes(nNewnessChange * ABRASION2USETIMES);
	GetItemMgr().UpdateEquipSpec(*p);
	nAttackTimes = p->nUseTimes;

	return E_Success;
}
//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: exchange_handler.cpp
// author: Wjiang
// actor:
// data: 2008-08-22
// last:
// brief: װ��ǿ��
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/creature_define.h"
#include "../WorldDefine/specfunc_define.h"
#include "map.h"
#include "role.h"
#include "creature.h"
#include "item_creator.h"
#include "../WorldDefine/msg_spec_item.h"
//#include "../WorldDefine/ItemDefine.h"
//#include "../WorldDefine/msg_compose.h"
#include "../serverdefine/msg_item.h"
#include "fairy_birth.h"
#include "const_define.h"
#include "MsgInfoMgr.h"
#include "../WorldDefine/msg_yuanshen.h"

#include "../ServerDefine/consolidate_define.h"
#include "../serverdefine/log_cmdid_define.h"
#include "../serverdefine/msg_log.h"

#include "player_session.h"


//------------------------------------------------------------------------
// ��Ʒǿ�������ƣ�
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleRolePosyEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidatePosy);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_ConsolidatePosy)) / sizeof(INT64);

	DWORD	dwErrorCode = pRole->PosyEquip(pRecv->dwNPCID, pRecv->dwFormulaID, pRecv->n64ItemID, \
												pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);

	// ������һ��װ��ǿ�����¼��ӿ�
	tagEquip *pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID));
	if(P_VALID(pEquip))
	{
		BOOL bSuccess = (E_Compose_Consolidate_Success == dwErrorCode || E_Compose_Consolidate_Perfect == dwErrorCode)?TRUE:FALSE;
		g_ScriptMgr.GetRoleScript()->OnItemConsolidate(pRole, pEquip->dwTypeID, EAllCliDateT_Posy, pEquip->GetWpnPosyTimes(), bSuccess);
	}
	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ConsolidatePosy	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ʒǿ�����Կ̣�
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEngraveEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidateEngrave);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_ConsolidateEngrave)) / sizeof(INT64);

	DWORD	dwErrorCode = pRole->EngraveEquip(pRecv->dwNPCID, pRecv->dwFormulaID, pRecv->n64ItemID, \
													pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);

	// ������һ��װ��ǿ�����¼��ӿ�
	tagEquip *pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID));
	if(P_VALID(pEquip))
	{
		BOOL bSuccess = (E_Compose_Consolidate_Success == dwErrorCode || E_Compose_Consolidate_Perfect == dwErrorCode)?TRUE:FALSE;
		g_ScriptMgr.GetRoleScript()->OnItemConsolidate(pRole, pEquip->dwTypeID, EAllCliDateT_Engrave, pEquip->GetWpnEngraveTimes(), bSuccess);
	}

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ConsolidateEngrave	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ʒǿ������Ƕ��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleInlayEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Inlay);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagEquip *pEquipTemp = (tagEquip*)pRole->GetItemMgr().GetBagItem(pRecv->n64DstItemID);
	if(P_VALID(pEquipTemp) && MIsEquipment(pEquipTemp->dwTypeID) && pEquipTemp->pEquipProto->bOnlyScriptStrengthen)
	{
		tagNS_Inlay	send;
		send.dwErrorCode = E_OnlyCanStrengthenByScript;
		SendMessage(&send, send.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	DWORD	dwErrorCode = pRole->InlayEquip(pRecv->n64SrcItemID, pRecv->n64DstItemID, pRecv->dwID);

	// ������һ��װ��ǿ�����¼��ӿ�
	tagEquip *pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(pRecv->n64DstItemID));
	if(P_VALID(pEquip))
	{
		BOOL bSuccess = (E_Compose_Consolidate_Success == dwErrorCode)?TRUE:FALSE;
		g_ScriptMgr.GetRoleScript()->OnItemConsolidate(pRole, pEquip->dwTypeID, EAllCliDateT_HoleGem, pEquip->GetLastHoleGemID(), bSuccess);
	}

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Inlay	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��Ʒǿ������ӡ��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBrandEquip(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Brand);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquipTemp = (tagEquip*)pRole->GetItemMgr().GetBagItem(pRecv->n64DstItemID);
	if(P_VALID(pEquipTemp)&& MIsEquipment(pEquipTemp->dwTypeID) && pEquipTemp->pEquipProto->bOnlyScriptStrengthen)
	{
		tagNS_Brand	send;
		send.dwErrorCode = E_OnlyCanStrengthenByScript;
		SendMessage(&send, send.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	DWORD	dwErrorCode = pRole->BrandEquip(pRecv->n64SrcItemID, pRecv->n64DstItemID, pRecv->n64IMID, pRecv->dwID);

	// ������һ��װ��ǿ�����¼��ӿ�
	tagEquip *pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(pRecv->n64DstItemID));
	if(P_VALID(pEquip))
	{
		BOOL bSuccess = (E_Compose_Consolidate_Success == dwErrorCode || E_Compose_Consolidate_Perfect == dwErrorCode)?TRUE:FALSE;
		g_ScriptMgr.GetRoleScript()->OnItemConsolidate(pRole, pEquip->dwTypeID, EAllCliDateT_Brand, pEquip->GetBrandLevel(), bSuccess);
	}

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Brand	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLoongSoul(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_LoongSoul);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = pRole->LoongSoulEquip(pRecv->n64SrcItemID, pRecv->n64DstItemID, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_LoongSoul	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ���
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleQuench(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidateQuench);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_ConsolidateEngrave)) / sizeof(INT64);

	DWORD	dwErrorCode = pRole->QuenchEquip(pRecv->dwNPCID, pRecv->dwFormulaID, pRecv->n64ItemID, 
											pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_ConsolidateQuench	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// �����ϳ���Ʒ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleProduceItem(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Produce);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ҵ��ϳ��䷽����
	const tagProduceProtoSer *pProduceProto = g_attRes.GetProduceProto(pRecv->dwFormulaID);
	if(!P_VALID(pProduceProto))
		return GT_INVALID;

	// ������ϸ���
	INT nStuffNum = 1 + ((INT)pRecv->dwSize - sizeof(tagNC_Produce)) / sizeof(INT64);
	
	DWORD	dwErrorCode = GT_INVALID;
	if(!MIsEquipment(pProduceProto->dwProItemTypeID) )
	{
		dwErrorCode = pRole->ProduceItem(pRecv->dwNPCID, Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->n64ItemID, pRecv->dwFormulaID,\
			pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);
	}
	else 
	{
		dwErrorCode = pRole->ProduceEquip(pRecv->dwNPCID, Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->n64ItemID, pRecv->dwFormulaID,\
			pRecv->n64IMID, pRecv->n64StuffID, nStuffNum, pRecv->dwID);
	}

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Produce	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

DWORD	PlayerSession::HandleRoleAdvancedCompose(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_AdvancedCompose);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwNewItemID = GT_INVALID;

	// ���NPC
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_AdvanceComposeNPC);
	if (dwErrorCode != E_Success)
		goto end;

	// �����ͼ
	tagItem* pDesign = pRole->GetItemMgr().GetBagItem(pRecv->n64DesignID);
	if (!P_VALID(pDesign))
		return GT_INVALID;
	if (!P_VALID(pDesign->pProtoType))
		return GT_INVALID;

	// �ҵ��ϳ��䷽����
	const tagAdvancedComposeProto* pProduceProto = g_attRes.GetAdvancedComposeProto(pDesign->dwTypeID);
	if(!P_VALID(pProduceProto))
		return GT_INVALID;

	// �Ҳ���,���жϲ��������Ƿ��㹻
	if (pRecv->n64Material1 != GT_INVALID)
	{
		tagItem* pMaterial1 = pRole->GetItemMgr().GetBagItem(pRecv->n64Material1);
		if (!P_VALID(pMaterial1))
			return GT_INVALID;
		if (!P_VALID(pMaterial1->pProtoType))
			return GT_INVALID;
		if ((pProduceProto->Material[0].dwTypeID != pMaterial1->dwTypeID) ||
			(pMaterial1->GetNum() < (INT16)pProduceProto->Material[0].dwCount))
		{
			return GT_INVALID;
		}
	}
	else if (pProduceProto->Material[0].dwTypeID != GT_INVALID)
	{
		return GT_INVALID;
	}
	if (pRecv->n64Material2 != GT_INVALID)
	{
		tagItem* pMaterial2 = pRole->GetItemMgr().GetBagItem(pRecv->n64Material2);
		if (!P_VALID(pMaterial2))
			return GT_INVALID;
		if (!P_VALID(pMaterial2->pProtoType))
			return GT_INVALID;
		if ((pProduceProto->Material[1].dwTypeID != pMaterial2->dwTypeID) ||
			(pMaterial2->GetNum() < (INT16)pProduceProto->Material[1].dwCount))
		{
			return GT_INVALID;
		}
	}
	else if (pProduceProto->Material[1].dwTypeID != GT_INVALID)
	{
		return GT_INVALID;
	}

	// �жϽ�Ǯ����
	if (pRole->GetCurMgr().GetBagSilver() < pProduceProto->n64SilverCost)
	{
		dwErrorCode = E_Compose_NotEnough_Money;
		goto end;
	}

	INT nVitality = pRole->GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pProduceProto->nVitalityCost);

	//if (pRole->GetAttValue(ERA_Vitality) < pProduceProto->nVitalityCost)
	//	return GT_INVALID;

	if (pRole->GetAttValue(ERA_Vitality) < nVitality)
	{
		dwErrorCode = E_Compose_Vitality_Inadequate;
		goto end;
	}

	// �жϱ����ռ�
	if (pRole->GetItemMgr().GetBagFreeSize() < 1)
	{
		dwErrorCode = E_Bag_NotEnoughSpace;
		goto end;
	}

	// ��Ǯ
	if ( pProduceProto->n64SilverCost > 0 && !pRole->GetCurMgr().DecBagSilver(pProduceProto->n64SilverCost, ELCID_Advanced_Composed_Cost))
	{
		IMSG(_T("HandleRoleAdvancedCompose::decrease silver failed!!roleid=%d,DesignID=%d\r\n"), pRole->GetID(), pDesign->dwTypeID);
		return GT_INVALID;
	}

	// �ۻ���
	//pRole->ChangeVitality(-pProduceProto->nVitalityCost);
	pRole->ChangeVitality(-nVitality);

	// �۳����ͼ�Ͳ���
	pRole->GetItemMgr().DelFromBag(pRecv->n64DesignID, ELCID_Advanced_Composed_Cost, 1);
	if (pRecv->n64Material1 != GT_INVALID)
	{
		pRole->GetItemMgr().DelFromBag(pRecv->n64Material1, ELCID_Advanced_Composed_Cost,
			(INT16)pProduceProto->Material[0].dwCount);
	}
	if (pRecv->n64Material2 != GT_INVALID)
	{
		pRole->GetItemMgr().DelFromBag(pRecv->n64Material2, ELCID_Advanced_Composed_Cost,
			(INT16)pProduceProto->Material[1].dwCount);
	}

	// ���ݸ�������ʱװ
	INT32 n32RandNum = IUTIL->Rand() % 10000;
	INT32 n32Pct = 0;
	for (INT nCnt = 0; nCnt < MAX_FASHION_CAN_COMPOSE; nCnt++)
	{
		n32Pct += pProduceProto->dwFashionComp[nCnt].dwPct;
		if (n32RandNum <= n32Pct)
		{
			dwNewItemID = pProduceProto->dwFashionComp[nCnt].dwTypeID;
			break;
		}
	}
	if (dwNewItemID == 0 || dwNewItemID == GT_INVALID)
	{
		IMSG(_T("HandleRoleAdvancedCompose::GenNewItemID failed!!roleid=%d,DesignID=%d\r\n"), pRole->GetID(), pDesign->dwTypeID);
		return GT_INVALID;
	}

	tagItem* pNewItem = ItemCreator::Create(EICM_AdvancedCompose, NULL, dwNewItemID);
	if (!P_VALID(pNewItem))
	{
		IMSG(_T("HandleRoleAdvancedCompose::CreateNewItem failed!!roleid=%d,DesignID=%d,newItemID=%d\r\n"), pRole->GetID(), pDesign->dwTypeID, dwNewItemID);
		return GT_INVALID;
	}
	pRole->GetItemMgr().Add2Bag(pNewItem, ELCID_Advanced_Composed_Add, TRUE);

end:
	tagNS_AdvancedCompose send;
	send.dwErrorCode = dwErrorCode;
	send.dwCompItemID = dwNewItemID;
	pRole->SendMessage(&send, send.dwSize);
	return dwErrorCode;
}

// Jason 2009-12-6 ����װ��Ǳ��ֵ����Э��
DWORD	PlayerSession::HandleRoleAddEquipPotVal(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_AddPot);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwPotValAdded = 0;
	DWORD	dwErrorCode = GT_INVALID;
	if( MIsEquipment( pRecv->n64DstItemID ) )
	{
		dwErrorCode = pRole->RaiseEquipPotVal( pRecv->n64SrcItemID,pRecv->n64DstItemID,pRecv->n64IMID,dwPotValAdded );
	}

	//������ 0���ɹ��� 1�������� 2��ʧ�� 
	//if( E_Compose_Consolidate_Perfect == dwErrorCode )
	//	dwErrorCode = 1;
	//else if( E_Compose_Consolidate_Success == dwErrorCode )
	//	dwErrorCode = 0;
	//else
	//	dwErrorCode = 2;

	tagNS_AddPot msg;
	msg.dwErrorCode	= dwErrorCode;
	msg.dwPotValue	= dwPotValAdded;
	SendMessage  ( &msg,msg.dwSize );

	return dwErrorCode;
}

//------------------------------------------------------------------------
// �㻯,װ���ϳ�
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDeCompose(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Decomposition)

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ҵ��ϳ��䷽����
	const tagDeComposeProtoSer *pComposeProto = g_attRes.GetDeComposeProto(pRecv->dwFormulaID);
	if(!P_VALID(pComposeProto))
		return GT_INVALID;

	DWORD	dwErrorCode = GT_INVALID;

	tagEquip *pEquipTemp = (tagEquip*)pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if(P_VALID(pEquipTemp) && MIsEquipment(pEquipTemp->dwTypeID) && pEquipTemp->pEquipProto->bOnlyScriptStrengthen)
	{

		tagNS_Decomposition	send;
		send.dwErrorCode = E_OnlyCanStrengthenByScript;
		SendMessage(&send, send.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	dwErrorCode = pRole->DeComposeItem(pRecv->dwNPCID, Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->n64ItemID, pRecv->dwFormulaID,\
			pRecv->n64IMID, pRecv->n64Item, pRecv->dwID);

	if(GT_INVALID == dwErrorCode)
	{
		return dwErrorCode;
	}

	tagNS_Decomposition	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ��ȡ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleExtract(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Extraction);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_Extraction send;

	DWORD dwErrorCode = E_Success;
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_ExtractNPC);
	if (E_Success != dwErrorCode)
	{
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return dwErrorCode;
	}

	send.dwErrorCode = pRole->Extraction(pRecv->dwNPCID, pRecv->n64Item);

	SendMessage(&send, send.dwSize);
	return E_Success;
}

//------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChisel(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_Chisel);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagEquip *pEquipTemp = (tagEquip*)pRole->GetItemMgr().GetBagItem(pRecv->n64SrcItemID);
	if (P_VALID(pEquipTemp)&& MIsEquipment(pEquipTemp->dwTypeID) && pEquipTemp->pEquipProto->bOnlyScriptStrengthen)
	{
		tagNS_Chisel	send;
		send.dwErrorCode = E_OnlyCanStrengthenByScript;
		SendMessage(&send, send.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->ChiselEquip(pRecv->n64SrcItemID, pRecv->n64StuffID, pRecv->dwID);

	// ������һ��װ��ǿ�����¼��ӿ�
	tagEquip *pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(pRecv->n64SrcItemID));
	if(P_VALID(pEquip))
	{
		BOOL bSuccess = (E_Compose_Consolidate_Success == dwErrorCode || E_Compose_Consolidate_Perfect == dwErrorCode)?TRUE:FALSE;
		g_ScriptMgr.GetRoleScript()->OnItemConsolidate(pRole, pEquip->dwTypeID, EAllCliDateT_Chisel, pEquip->GetBrandLevel(), bSuccess);
	}

	tagNS_Chisel	send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}
//------------------------------------------------------------------------
// ����ʱװ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSpinFashionDress(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_ConsolidateSpin);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	//npcְ�ܼ��
	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Spin);
	if (dwErrorCode != E_Success)
	{	
		tagNS_ConsolidateSpin send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	//����ʱװ
	dwErrorCode = pRole->SpinFashion(pRecv->eSpinType, pRecv->n64ItemID,pRecv->dwStuffTypeID, pRecv->dwStuffItemNum, pRecv->dwID);

	tagNS_ConsolidateSpin send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}
//------------------------------------------------------------------------
// ʱװȾɫ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleDyeFashion(tagNetCmd* pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_DyeFashion);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->DyeFashion(pRecv->n64DyeSN, pRecv->n64EquipSerial, pRecv->dwID);

	tagNS_DyeFashion send;
	send.dwErrCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}


DWORD PlayerSession::HandleGemRemoval(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_GemRemoval);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	dwErrorCode = pRole->RemoveGemFromEquip(pRecv->dwNPCID, pRecv->n64EquipID, (INT8)pRecv->n16GemNum,pRecv->n64GemRomovalSign);

	tagNS_GemRemoval send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}


DWORD PlayerSession::HandleGetGemRemovalInfo(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_GetGemRemovalInfo);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD	dwErrorCode = GT_INVALID;

	tagNS_GetGemRemovalInfo send;
	INT8 num  = 0;

	ItemMgr & itemMgr = pRole->GetItemMgr();

	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64EquipID);
	if( P_VALID(pItem) )
	{
		if( MIsEquipment(pItem->pProtoType->dwTypeID) )
		{
			tagEquip * pEquip = (tagEquip*)pItem;

			if (pEquip->pEquipProto->bOnlyScriptStrengthen)
			{
				send.dwErrCode = E_OnlyCanStrengthenByScript;
				SendMessage(&send, send.dwSize);
				return E_OnlyCanStrengthenByScript;
			}
		}
	}

	dwErrorCode = pRole->GetGemRemovalInfo(pRecv->dwNPCID, pRecv->n64EquipID, num,send.dwGemIDs);
	send.n16GemNum = num;

	send.dwErrCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

static DWORD GHandleBuildEuipment(tagNetCmd * pCmd,PlayerSession * pSession);

DWORD PlayerSession::HandleBuildEuipment(tagNetCmd * pCmd)
{
	tagNS_BuildEuipment msg;
	msg.dwErrorCode = GHandleBuildEuipment(pCmd,this);
	SendMessage(&msg,msg.dwSize);
	return msg.dwErrorCode;
}
// ��������
static DWORD GHandleBuildEuipment(tagNetCmd * pCmd,PlayerSession * pSession)
{
	if( !P_VALID(pSession) )
		return GT_INVALID;

	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_BuildEuipment);

	// ��ȡ��ɫ
	Role* pRole = pSession->GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	Map*	pMap =	pRole->GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}
	Creature* pNPC = pMap->FindCreature(pRecv->dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(pRole) )
		return E_Compose_NPC_Distance;
	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_SoarEquipment) )
		return E_SoaringEquipment_NPC_Invalid;

	tagItem * pDestEquip	= pRole->GetItemMgr().GetBagItem(pRecv->n64ToBuildEquipID);
	tagItem * pStone		= pRole->GetItemMgr().GetBagItem(pRecv->n64StoneID);
	tagItem * pSrcItem		= pRole->GetItemMgr().GetBagItem(pRecv->n64StuffID);

	tagEquip * pEquipment = NULL;
	tagEquip * pSrcEquip = NULL;
	const tagEquipProto * pEquipProto = NULL;

	if( !P_VALID(pDestEquip) )
		return E_Consolidate_Equip_Not_Exist;
	if( !P_VALID(pStone) )
		return E_SoaringStone_NotExist;

	if( !MIsEquipment(pRecv->n64ToBuildEquipID) )
		return E_SoaringEquipment_NotEquip;

	pEquipment = (tagEquip*)pDestEquip;
	pEquipProto = pEquipment->pEquipProto;

	if( MIsFashion(pEquipment) )
		return E_SoaringEquipment_FashionHasBeenProhibited;

	if( P_VALID(pEquipment->pEquipProto->dwTimeLimit) )
		return E_SoaringEquipment_TimelimittedEquipHasBeenProhibited;

	if( MIsFaBao(pEquipment) )
		return E_SoaringEquipment_FabaoHasBeenProhibited;

	if( pEquipment->equipSpec.byQuality < EIQ_White || pEquipment->equipSpec.byQuality >= EIQ_End )
		return E_SoaringEquipment_NotIdentify;

	if( pEquipment->pEquipProto->byLevel < 75 )
		return E_SoaringEquipment_LevelLower;

	if( pStone->pProtoType->eTypeEx != EITE_MixingEquipAccessories )
		return E_SoaringEquipment_NotAccessories;

	INT pct = 10;
	if( P_VALID(pSrcItem) )
	{
		if(!MIsEquipment(pRecv->n64StuffID))
			return E_SoaringEquipment_StuffMustBeEquip;
		pSrcEquip = (tagEquip*)pSrcItem;
		if( pSrcEquip->equipSpec.byQuality < EIQ_Blue )
			return E_SoaringEquipment_StuffQltyLower;
		pct += (FLOAT)pSrcEquip->pEquipProto->byLevel * pSrcEquip->pEquipProto->byLevel * ((FLOAT)pSrcEquip->equipSpec.byQuality * pSrcEquip->equipSpec.byQuality - 8) / 2000/*00*/;
	}

	DWORD re = E_Success;
	INT rand_value = IUTIL->RandomInRange(0,100);
	if( rand_value <= pct )
	{
		const tagEquipQltyEffect *pEquipQltyEffect = g_attRes.GetEquipQltyEffect(pEquipment->equipSpec.byQuality);
		if(!P_VALID(pEquipQltyEffect))
		{
			return GT_INVALID;
		}
		FLOAT fModPct = pEquipment->equipSpec.n16PotValModPct;
		if( fModPct < 10000 )
			fModPct = 10000.0f;
		FLOAT fValueModFactor = (1.0f + FLOAT(IUTIL->Rand() % 21 - 10) / 100.0f) * pEquipQltyEffect->fPotFactor * fModPct / 10000.0f;
		if( fValueModFactor <= 0 )
			fValueModFactor = 1;
		pEquipment->equipSpec.nPotVal = (INT)(pEquipProto->nPotVal * fValueModFactor);
		// Jason 2009-12-22 ����bug����Ʒ����ʱ���ᳬ�����Ǳ��ֵ�������ɫƷ��
		if( pEquipment->equipSpec.nPotVal > pEquipment->pEquipProto->nMaxPotVal )
			pEquipment->equipSpec.nPotVal = pEquipment->pEquipProto->nMaxPotVal;
		//else if( pEquipment->equipSpec.nPotVal < 0 )
		//	pEquipment->equipSpec.nPotVal = 0;
		pEquipment->equipSpec.nPotValUsed = 0;
		pEquipment->equipSpec.bSoaringFlag = TRUE;
		pEquipment->equipSpec.nPotIncTimes = 10;

		pRole->GetItemMgr().UpdateEquipSpec(*pEquipment);
	}
	else
		re = E_SoaringEquipment_Fail;
	pRole->GetItemMgr().DelFromBag(pRecv->n64StoneID,ELCID_Soaring_BuildEquip,1);
	pRole->GetItemMgr().DelFromBag(pRecv->n64StuffID,ELCID_Soaring_BuildEquip,1);

	pRole->OnCompletePracticeEquipment(pEquipment->pEquipProto->dwTypeID);

	return re;
}
static DWORD GHandleCastingEquipment(tagNetCmd*pCmd,PlayerSession * pSession);
DWORD PlayerSession::HandleCastingEquipment(tagNetCmd*pCmd)
{
	tagNS_SoarEquipCasting msg;
	msg.dwErrorCode = GHandleCastingEquipment(pCmd,this);
	SendMessage(&msg,msg.dwSize);
	return msg.dwErrorCode;
}
// ��������
static DWORD GHandleCastingEquipment(tagNetCmd*pCmd,PlayerSession * pSession)
{
	if( !P_VALID(pSession) )
		return GT_INVALID;

	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_SoarEquipCasting);

	const tagCastingProto * pCastingProto = g_attRes.GetOneCastingProto(pRecv->dwFormulaID);
	if( !P_VALID(pCastingProto) )
		return GT_INVALID;

	INT64			*	pCastingStuffs = pRecv->n64StuffID;
	if( !P_VALID(pCastingStuffs) )
		return GT_INVALID;

	// ��ȡ��ɫ
	Role* pRole = pSession->GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	Map*	pMap =	pRole->GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}
	Creature* pNPC = pMap->FindCreature(pRecv->dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(pRole) )
		return E_Compose_NPC_Distance;
	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_CastingEquipment) )
		return E_CastingEquipment_NPC_Invalid;

	tagItem * pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if( !P_VALID(pItem) )
		return E_Consolidate_Equip_Not_Exist;

	if( !MIsEquipment(pRecv->n64ItemID) )
		return E_CastingEquipment_NotEquip;

	tagEquip * pEquip = (tagEquip*)pItem;
	const tagEquipProto * pEquipProto = pEquip->pEquipProto;
	if( !P_VALID(pEquipProto) )
		return GT_INVALID;

	if( MIsFashion(pEquip) )
		return E_SoaringEquipment_FashionHasBeenProhibited;

	if( P_VALID(pEquip->pEquipProto->dwTimeLimit) )
		return E_SoaringEquipment_TimelimittedEquipHasBeenProhibited;

	if( MIsFaBao(pEquip) )
		return E_SoaringEquipment_FabaoHasBeenProhibited;

	if( pEquip->equipSpec.byQuality < EIQ_White || pEquip->equipSpec.byQuality >= EIQ_End )
		return E_SoaringEquipment_NotIdentify;

	if( !pEquip->equipSpec.bSoaringFlag )
		return E_CastingEquipment_MustBeSoaringEquip;

	if( pEquip->pEquipProto->eType == EIT_Weapon && pCastingProto->byType != 0 ||
		(pEquip->pEquipProto->eType == EIT_ClothArmor || pEquip->pEquipProto->eTypeEx == EIT_Armor) && pCastingProto->byType != 1 ||
		(pEquip->pEquipProto->eType == EIT_Decoration) && pCastingProto->byType != 2 )
		return E_CastingEquipment_StuffTypeDismatch;

	if( pEquip->equipSpec.nCastingTimes + 1 != pCastingProto->u16Level )
		return E_CastingEquipment_CastingTimesNotMatch;

	int count = 0,sended_count = 0;
	sended_count = (pRecv->dwSize - sizeof(tagNC_SoarEquipCasting) + sizeof( INT64 ) ) / sizeof( INT64 );
	int min_count = min(MAX_CASTING_PARAM_NUM,sended_count);
	for( int i = 0 ; i < min_count; ++i )
	{
		if( P_VALID(pCastingStuffs[i]) )
		{
			tagItem * pItem = pRole->GetItemMgr().GetBagItem(pCastingStuffs[i]);
			if( !P_VALID(pItem) )
				return E_CastingEquipment_StuffNotExist;
			if( pItem->pProtoType->dwTypeID != pCastingProto->dwStuffTypeID[i] )
				return E_CastingEquipment_StuffTypeDismatchProto;
			if( pItem->GetNum() < pCastingProto->nStuffNum[i] )
				return E_CastingEquipment_StuffNotEnough;
			++count;
		}
	}

	if( count != sended_count )
		return E_CastingEquipment_StuffNotEnough;

	if( pEquip->equipSpec.nPotVal < pCastingProto->u16PotvalConsume )
		return E_Compost_Equip_PotvalNotEnough;

	tagItem * pAccessorItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ProtectSignID);

	bool bCostPotval = true,bBackStage = true;
	if( P_VALID(pAccessorItem) )
	{
		if( pAccessorItem->pProtoType->eTypeEx == EITE_CastingAccessories )
			bCostPotval = false;
		else if( pAccessorItem->pProtoType->eTypeEx == EITE_AdvancedCastingAccessories )
		{
			bCostPotval = false;
			bBackStage = false;
		}
	}

	INT pct = 100 * pCastingProto->nProbability / 10000.0f ;//+ 0.1/*0.2 * 100*/ * (FLOAT)pRole->GetAttValue(ERA_Fortune) ;/// 200.0f;
	DWORD re = E_Success;

	if( pct >= IUTIL->RandomInRange(0,100) )
	{
		pEquip->equipSpec.nCastingTimes ++;
		pEquip->ChangePotVal(0-pCastingProto->u16PotvalConsume);
	}
	else
	{
		if( bCostPotval )
			pEquip->ChangePotVal(0-pCastingProto->u16PotvalConsume);
		if( bBackStage )
			pEquip->equipSpec.nCastingTimes--;
		if(pEquip->equipSpec.nCastingTimes < 0)
			pEquip->equipSpec.nCastingTimes = 0;
		re = E_CastingEquipment_Fail;
	}
	for( int i = 0 ; i < min_count; ++i )
	{
		if( P_VALID(pCastingStuffs[i]) )
		{
			tagItem * pItem = pRole->GetItemMgr().GetBagItem(pCastingStuffs[i]);
			if( !P_VALID(pItem) )
				continue;
			pRole->GetItemMgr().DelFromBag(pItem->n64Serial,ELCID_Casting_SoaringEquip,pCastingProto->nStuffNum[i]);
		}
	}
	if( P_VALID(pAccessorItem) )
		pRole->GetItemMgr().DelFromBag(pAccessorItem->n64Serial,ELCID_Casting_SoaringEquip,1);

	pRole->GetItemMgr().UpdateEquipSpec(*pEquip);

	pRole->OnCompleteCastingEquipment(pEquip->pEquipProto->dwTypeID,pEquip->equipSpec.nCastingTimes);

	return re;
}
// ��ȡǿ�����Ե���Ϣ
static const tagFConsolidateProto * GetFConsolidateOneFormula(tagEquip * pEquip,INT level)
{
	TMap<DWORD,tagFConsolidateProto*> & conMap = g_attRes.GetFConsolidateProto();
	tagFConsolidateProto* re = NULL, * p = NULL;
	for(TMap<DWORD,tagFConsolidateProto*>::TMapIterator it = conMap.Begin(); conMap.PeekNext(it,p); )
	{
		if( pEquip->pEquipProto->eTypeEx == p->eItemTypeEx && p->n16Times == level/*pEquip->equipSpec.byConsolidateLevel + 1*/ )
		{
			re = p;
			break;
		}
	}
	return re;
}
// װ��ǿ��
DWORD PlayerSession::HandleStrengthenEquip(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_StrengthenEquip);

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_StrengthenEquip backMsg;

	ItemMgr & itemMgr = pRole->GetItemMgr();

	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64ToBeStrengthenedEquip);
	if( !P_VALID(pItem) )
	{
		backMsg.dwErrorCode = E_Consolidate_Equip_Not_Exist;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_Equip_Not_Exist;
	}

	if( !MIsEquipment(pItem->pProtoType->dwTypeID) )
	{
		backMsg.dwErrorCode = E_Consolidate_NotEquipment;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_NotEquipment;
	}

	tagEquip * pEquip = (tagEquip*)pItem;

	if (pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		backMsg.dwErrorCode = E_OnlyCanStrengthenByScript;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	if( MIsFaBao(pEquip) )
	{
		backMsg.dwErrorCode = E_Consolidate_NotEquipment;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_NotEquipment;
	}

	tagItem * pStone = itemMgr.GetBagItem(pRecv->n64StrengthenStone);

	if( !P_VALID(pStone) )
	{
		backMsg.dwErrorCode = E_Compose_Stuff_Not_Exist;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Compose_Stuff_Not_Exist;
	}

	//if( pStone->pProtoType->eSpecFunc != EISF_QiangHuaStone )
	//{
	//	backMsg.dwErrorCode = E_Consolidate_Stone_Not_Exist;
	//	pRole->SendMessage(&backMsg,backMsg.dwSize);
	//	return E_Consolidate_Stone_Not_Exist;
	//}

	if( P_VALID(pStone->pProtoType->nSpecFuncVal1) && pStone->pProtoType->nSpecFuncVal1 < pEquip->pEquipProto->byLevel )
	{
		backMsg.dwErrorCode = E_Consolidate_Stone_LevelLow;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_Stone_LevelLow;
	}

	INT LevelAdding = 0;
	if( g_world.IsGodStrengthenOpen() )
	{
		if( pEquip->equipSpec.byQuality >= g_world.GetGodStrengthenEquipMinQlty() )
			LevelAdding = pEquip->equipSpec.byGodStrengthenLevel;
	}

	INT nMaxConLevel = pEquip->equipSpec.bSoaringFlag ? tagEquipSpec::ECL_AftConMaxLevel : (tagEquipSpec::ECL_BefConMaxLevel + LevelAdding);
	if( nMaxConLevel < tagEquipSpec::ECL_BefConMaxLevel + LevelAdding )
		nMaxConLevel = tagEquipSpec::ECL_BefConMaxLevel + LevelAdding;
	else if( nMaxConLevel > tagEquipSpec::ECL_AftConMaxLevel )
		nMaxConLevel = tagEquipSpec::ECL_AftConMaxLevel;
	nMaxConLevel = min(nMaxConLevel,pEquip->pEquipProto->nMaxConsolidateLevel + LevelAdding);

	const tagEquipQltyEffect* pEquipQltyEffect = g_attRes.GetEquipQltyEffect(pEquip->equipSpec.byQuality);
	if( P_VALID(pEquipQltyEffect) )
		nMaxConLevel = min(nMaxConLevel,pEquipQltyEffect->n8StrengLevelLimit + LevelAdding);

	if( pEquip->equipSpec.byConsolidateLevel >= nMaxConLevel )
	{
		backMsg.dwErrorCode = E_Consolidate_LevelLimitExceeded;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_LevelLimitExceeded;
	}

	const tagFConsolidateProto * pConFormula = GetFConsolidateOneFormula(pEquip,pEquip->equipSpec.byConsolidateLevel + 1);

	if( !P_VALID(pConFormula) )
	{
		backMsg.dwErrorCode = E_Consolidate_FormulaNotExist;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_FormulaNotExist;
	}

	if( pStone->pProtoType->eStuffType != pConFormula->ConsolidateStuff[0].eStuffType )
	{
		backMsg.dwErrorCode = E_Consolidate_Stone_NotMatchStuff;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_Stone_NotMatchStuff;
	}

	if( pRole->GetCurMgr().GetBagSilver() < pConFormula->dwMoneyConsume )
	{
		backMsg.dwErrorCode = E_Consolidate_NotEnough_Money;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_NotEnough_Money;
	}

	bool bDelEquip = true,bBackStage = true;

	tagItem * pBaodi = itemMgr.GetBagItem(pRecv->n64ProtectedSign);
	if( P_VALID(pBaodi) )
	{
		if( pBaodi->pProtoType->eSpecFunc == EISF_BaoDiFu )
		{
			if( P_VALID(pBaodi->pProtoType->nSpecFuncVal2) && pBaodi->pProtoType->nSpecFuncVal2 < pEquip->equipSpec.byConsolidateLevel )
			{
				backMsg.dwErrorCode = E_GodStrengthen_BaodiFuLevelLimit;
				pRole->SendMessage(&backMsg,backMsg.dwSize);
				return E_GodStrengthen_BaodiFuLevelLimit;
			}
			if( pBaodi->pProtoType->nSpecFuncVal1 == 0 )
			{
				bDelEquip = false;
			}
			else if( pBaodi->pProtoType->nSpecFuncVal1 == 1 )
			{
				bDelEquip = false;
				bBackStage = false;
			}
		}
	}

	FLOAT pctLucky = 0;

	tagItem * pLuckySign = itemMgr.GetBagItem(pRecv->n64LuckySign);
	if( P_VALID(pLuckySign) && pLuckySign->pProtoType->eSpecFunc == EISF_LuckyFu )
	{
		if (pLuckySign->pProtoType->nSpecFuncVal1 == 6) // �����
		{
			if(pLuckySign->pProtoType->nSpecFuncVal2 < pEquip->equipSpec.byConsolidateLevel)
			{
				backMsg.dwErrorCode = E_Consolidate_LuckySign_LevelLimit;
				pRole->SendMessage(&backMsg,backMsg.dwSize);
				return E_Consolidate_LuckySign_LevelLimit;
			}

			if (P_VALID(pBaodi) && P_VALID(pBaodi->pProtoType) && pBaodi->pProtoType->eSpecFunc == EISF_BaoDiFu)
			{
				backMsg.dwErrorCode = E_Consolidate_LuckySign_BaodiLimit;
				pRole->SendMessage(&backMsg,backMsg.dwSize);
				return E_Consolidate_LuckySign_BaodiLimit;
			}

			DWORD dwID = pLuckySign->pProtoType->nSpecFuncVal1 * 100 + pLuckySign->pProtoType->nSpecFuncVal2; 
			const tagFConsolidateLuckySignPct* pctSign = g_attRes.GetOneFConsolidateLuckySignPct(dwID);
			if( P_VALID(pctSign) )
				pctLucky = pctSign->nPct;
		}
		else
		{
			if( LevelAdding > 0 && pEquip->equipSpec.byConsolidateLevel >= tagEquipSpec::ECL_BefConMaxLevel && pLuckySign->pProtoType->nSpecFuncVal1 != 5 ) // �������˷�
			{
				backMsg.dwErrorCode = E_Consolidate_LuckySign_NeedGodStrengSign;
				pRole->SendMessage(&backMsg,backMsg.dwSize);
				return E_Consolidate_LuckySign_NeedGodStrengSign;
			}
			DWORD dwID = pLuckySign->pProtoType->nSpecFuncVal1 * 100 + pEquip->equipSpec.byConsolidateLevel + 1; 
			const tagFConsolidateLuckySignPct* pctSign = g_attRes.GetOneFConsolidateLuckySignPct(dwID);
			if( P_VALID(pctSign) )
				pctLucky = pctSign->nPct;
		}
	}

	// �ɹ���
	FLOAT sucPct = pConFormula->nSuccessRate;
	sucPct += pctLucky;

	if( pEquip->equipSpec.byConsolidateLevel < 15 )
	{
		INT nConTimes = pRole->GetConsolidateTimes();
		if( nConTimes > 0 && nConTimes % 10 == 0 )
		{
			//nConTimes /= 10;
			sucPct += /*nConTimes **/ pConFormula->nConsTimesPct;
		}
	}

	sucPct += FLOAT(pRole->GetAttValue(ERA_Fortune)) / 1000;

	INT nStrengththenSuc1 = pRole->GetSpecSkillValue(ESSF_Strengthen,pEquip->equipSpec.byConsolidateLevel,sucPct);
	INT nStrengththenSuc2 = pRole->GetSpecSkillValue(ESSF_Strengthen1,pEquip->equipSpec.byConsolidateLevel,sucPct);

	sucPct += (nStrengththenSuc1 + nStrengththenSuc2);
	sucPct /= 100;

	//ILOG->Write(_T("ǿ���ɹ��� %d%% \r\n"), (INT)sucPct);

	itemMgr.DelFromBag(pRecv->n64StrengthenStone,ELCID_FCompose_Consolidate,1);
	if( P_VALID(pRecv->n64ProtectedSign) )
		itemMgr.DelFromBag(pRecv->n64ProtectedSign,ELCID_FCompose_Consolidate,1);
	if( P_VALID(pRecv->n64LuckySign) )
		itemMgr.DelFromBag(pRecv->n64LuckySign,ELCID_FCompose_Consolidate,1);

	pRole->GetCurMgr().DecBagSilver(pConFormula->dwMoneyConsume,ELCID_FCompose_Consolidate);

	pRole->IncConsolidateTimes();

	DWORD dwTypeID = 0;
	BYTE byBeforeLvl = pEquip->equipSpec.byConsolidateLevel;
	BYTE byAfterLvl = 0;
	if (P_VALID(pEquip->pEquipProto))
		dwTypeID = pEquip->pEquipProto->dwTypeID;


	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	DWORD dwSucceed = 2;
	if (P_VALID(pRoleScript))
	{
		if(P_VALID(pEquip->pEquipProto))
		{
			const INT32 nMask	= 0xFFFFFFFF;
			INT32 n32High	= (INT32)((pEquip->GetKey() >> 32) & nMask);
			INT32 n32Low	= (INT32)(pEquip->GetKey() & nMask);
			dwSucceed = pRoleScript->CanStrengthenEquip( pRole, dwTypeID, n32High, n32Low, pEquip->pEquipProto->byQuality, byBeforeLvl );
		}
	}

	
	INT _pct = IUTIL->RandomInRange(0,100);
	if( 1 == dwSucceed || ( 2 == dwSucceed && _pct <= sucPct ) )
	{
		//if( pConFormula->fcoefficientC < 0.1 )
		//	memcpy(pEquip->equipSpec.ConsolidateEffect,pConFormula->RoleAttEffect,sizeof(pEquip->equipSpec.ConsolidateEffect));
		//else
		//{
			//for( INT i = 0 ; i < MAX_FCONSOLIDATE_NUM; ++i )
			//{
			//	if( P_VALID(pConFormula->RoleAttEffect[i].eRoleAtt) && P_VALID(pConFormula->RoleAttEffect[i].nValue) )
			//	{
			//		pEquip->equipSpec.ConsolidateEffect[i].eRoleAtt = pConFormula->RoleAttEffect[i].eRoleAtt;
			//		if( i < 3 )
			//			//pEquip->equipSpec.ConsolidateEffect[i].nValue = pConFormula->RoleAttEffect[i].nValue * (pConFormula->fcoefficientA + pConFormula->fcoefficientB * pEquip->equipSpec.byQuality + pEquip->pEquipProto->byLevel / pConFormula->fcoefficientC );
			//			pEquip->equipSpec.ConsolidateEffect[i].nValue = pConFormula->RoleAttEffect[i].nValue * ( 1 + pConFormula->fcoefficientA * pEquip->equipSpec.byQuality * pEquip->pEquipProto->byLevel );
			//		else
			//			pEquip->equipSpec.ConsolidateEffect[i].nValue = pConFormula->RoleAttEffect[i].nValue;
			//	}
			//}
		//}
		pEquip->equipSpec.byConsolidateLevel += 1;
		if( pEquip->equipSpec.byConsolidateLevel > nMaxConLevel )
			pEquip->equipSpec.byConsolidateLevel = nMaxConLevel;
		ItemCreator::RecalEquipStrengthAtt(pEquip,pEquip->equipSpec.byConsolidateLevel);
		// ����װ������
		pRole->CalEquipFlare(pEquip);
		itemMgr.UpdateEquipSpec(*pEquip);
		
		// ǿ�����ǿ���ȼ�
		byAfterLvl = pEquip->equipSpec.byConsolidateLevel;

		backMsg.dwErrorCode = E_Success;

		if (P_VALID(pRoleScript))
		{
			pRoleScript->OnEquipConsolidateLevelUp(pRole, dwTypeID, byBeforeLvl, byAfterLvl);

			if(P_VALID(pEquip->pEquipProto))
				pRoleScript->DecSilverByStrengthen(pRole, dwTypeID, pEquip->pEquipProto->byQuality, byBeforeLvl, pConFormula->dwMoneyConsume);
		}
	}
	else
	{
		if (P_VALID(pRoleScript))
		{
			pRoleScript->OnEquipConsolidateLevelUp(pRole, dwTypeID, byBeforeLvl, byAfterLvl);

			if(P_VALID(pEquip->pEquipProto))
				pRoleScript->DecSilverByStrengthen(pRole, dwTypeID, pEquip->pEquipProto->byQuality, byBeforeLvl, pConFormula->dwMoneyConsume);
		}

		if( bDelEquip )
			itemMgr.DelFromBag(pRecv->n64ToBeStrengthenedEquip,ELCID_FCompose_Consolidate);
		else if(bBackStage)
		{
			if( pEquip->equipSpec.byConsolidateLevel > 0 )
			{
				--pEquip->equipSpec.byConsolidateLevel;
				ItemCreator::RecalEquipStrengthAtt(pEquip,pEquip->equipSpec.byConsolidateLevel);
				// ����װ������
				pRole->CalEquipFlare(pEquip);
				itemMgr.UpdateEquipSpec(*pEquip);
			}

			// ǿ�����ǿ���ȼ�
			byAfterLvl = pEquip->equipSpec.byConsolidateLevel;
		}
		backMsg.dwErrorCode = E_Consolodate_F_Fail;
	}

	pRole->SendMessage(&backMsg,backMsg.dwSize);

	return E_Success;
}
// ����
static DWORD HandleDetachSoulFromEquip(tagNetCmd * pCmd,Role* pRole)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_DetachOrAttackSoul_Equip);

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr & itemMgr = pRole->GetItemMgr();
	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64Equip);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
		return GT_INVALID;

	tagNS_DetachOrAttackSoul_Equip backMsg;
	backMsg.bDetachOrAttack = TRUE;
	backMsg.n64ID = GT_INVALID;

	if( !MIsEquipment(pItem->pProtoType->dwTypeID) )
	{
		backMsg.dwErrorCode = E_Consolidate_NotEquipment;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_NotEquipment;
	}

	tagEquip * pEquip = (tagEquip*)pItem;

	if (pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		backMsg.dwErrorCode = E_OnlyCanStrengthenByScript;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	pItem = itemMgr.GetBagItem(pRecv->n64Stone);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_XihunStoneNotExist;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_XihunStoneNotExist;
	}
	if( pItem->pProtoType->eSpecFunc != EISF_XiHun )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_NotXihunStone;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_NotXihunStone;
	}

	if( MIsFashion(pEquip) || MIsFaBao(pEquip) || pEquip->pEquipProto->bSuckLimit )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_SpecEquipCannotBeDetachSoul;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_SpecEquipCannotBeDetachSoul;
	}

	if( pEquip->equipSpec.byConsolidateLevel == 0 )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_ConsolidateLevelZero;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_ConsolidateLevelZero;
	}

	if( pItem->pProtoType->nSpecFuncVal1 < pEquip->equipSpec.byConsolidateLevel )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_XihunLevelLow;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_XihunLevelLow;
	}

	const tagFAttachSoulProto* pSoulProto = g_attRes.GetOneFuhunStone(pEquip->equipSpec.byConsolidateLevel);
	if( !P_VALID(pSoulProto) || !P_VALID(pSoulProto->dwStoneTypeID) )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_CannotFindFuhunStone;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_CannotFindFuhunStone;
	}

	// ����װ���Ǳ��󶨸���ʯ���������������ǰ󶨸���ʯ������Ϊ���󶨸���ʯ
	tagItem * pSoulStone = NULL;
	if ( pEquip->IsAttachSoulBind() )
	{
		pSoulStone = ItemCreator::Create(EICM_Xihun,  GT_INVALID, pSoulProto->dwBindStoneTypeID, 1, GT_INVALID);
	}
	else
	{
		pSoulStone = ItemCreator::Create(EICM_Xihun,  GT_INVALID, pSoulProto->dwStoneTypeID, 1, GT_INVALID);	
	}
	
	if( !P_VALID(pSoulStone) )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_FuhunStoneCreatorFail;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_FuhunStoneCreatorFail;
	}

	if( itemMgr.GetBagFreeSize() < 1 )
		return E_Bag_NotEnoughSpace;

	if( pEquip->bLock )
	{
		pSoulStone->bLock = 1;
		pSoulStone->dwUnlockTime = 0;
		if( pSoulStone->eStatus == EUDBS_Null )
			pSoulStone->eStatus = EUDBS_Update;
	}

	// ����������װ���Ǳ�������ʯ����ģ���û����󶨱�ʯ���������ԭΪװ������Ĭ�ϵİ�״̬
	if ( pEquip->IsAttachSoulBind() )
	{
		// pSoulStone->byBind = EBS_Bind;
		// �ж�װ���Ƿ����а󶨵ı�ʯ
		bool bHasBindGemItem = false;
		for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
		{
			if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
			{
				continue;
			}
			BYTE byGemBindType = g_attRes.GetItemProto(pEquip->equipSpec.dwHoleGemID[i])->byBindType;
			if ( byGemBindType == EBM_Gain || byGemBindType == EBM_AccountBind )
			{
				bHasBindGemItem = true;
				break;
			}
		}

		// װ����û�а󶨵ı�ʯ����ԭ����װ������Ĭ�ϵİ�״̬,������Ϊ��ͨ��״̬
		if ( !bHasBindGemItem )
		{
			pEquip->Bind();
		}
		else
		{
			pEquip->byBind = EBS_Bind;				
		}
		// �ı�װ���İ�״̬��֪ͨDB�Ϳͻ���
		pEquip->SetUpdate(EUDBS_Update);
		itemMgr.SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);

	}

	itemMgr.DelFromBag(pRecv->n64Stone,ELCID_FCompose_Xinhun,1);
	itemMgr.Add2Bag(pSoulStone,ELCID_FCompose_Xinhun,TRUE);
	// ��һ��һ�����ߵ��¼��ӿ�
	g_ScriptMgr.GetRoleScript()->OnObtainItem(pRole, pSoulProto->dwStoneTypeID, pSoulStone->eCreateMode);

	pEquip->equipSpec.byConsolidateLevel = 0;
	
	// ����װ������
	pRole->CalEquipFlare(pEquip);

	ItemCreator::RecalEquipStrengthAtt(pEquip,NULL);
	itemMgr.UpdateEquipSpec(*pEquip);

	backMsg.dwErrorCode = E_Success;
	backMsg.n64ID = pSoulStone->n64Serial;
	pRole->SendMessage(&backMsg,backMsg.dwSize);

	return E_Success;
}

// ��ȡ����ʯ��Ϣ
static const tagFAttachSoulProto* GetFuhunStoneInfo(DWORD dwFuhunStoneTypeID)
{
	TMap<DWORD ,tagFAttachSoulProto*> & mapFuhun = g_attRes.GetFAttachSoulProto();
	tagFAttachSoulProto* p;
	for( TMap<DWORD ,tagFAttachSoulProto*>::TMapIterator it = mapFuhun.Begin(); mapFuhun.PeekNext(it,p); )
	{
		if( p->dwStoneTypeID == dwFuhunStoneTypeID || p->dwBindStoneTypeID == dwFuhunStoneTypeID )
		{
			return p;
		}
	}
	return NULL;
}
// ����
static DWORD HandleAttachSoulToEquip(tagNetCmd * pCmd,Role* pRole)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_DetachOrAttackSoul_Equip);

	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr & itemMgr = pRole->GetItemMgr();

	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64Stone);
	tagItem * pItemE = itemMgr.GetBagItem(pRecv->n64Equip);

	tagNS_DetachOrAttackSoul_Equip backMsg;
	backMsg.bDetachOrAttack = FALSE;
	backMsg.n64ID = GT_INVALID;

	if( !P_VALID(pItemE) || !P_VALID(pItemE->pProtoType) )
	{
		return GT_INVALID;
	}
	if( !MIsEquipment(pItemE->pProtoType->dwTypeID) )
	{
		backMsg.dwErrorCode = E_Consolidate_NotEquipment;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_NotEquipment;
	}
	if( MIsFashion(pItemE) || MIsFaBao(pItemE) )
	{
		backMsg.dwErrorCode = E_Consolidate_NotEquipment;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_Consolidate_NotEquipment;
	}

	tagEquip * pEquip = (tagEquip*)pItemE;

	if (pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		backMsg.dwErrorCode = E_OnlyCanStrengthenByScript;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_FuhunStoneNotExist;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_FuhunStoneNotExist;
	}
	if( pItem->pProtoType->eSpecFunc != EISF_FuHun )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_NotFuhunStone;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_NotFuhunStone;
	}
	
	const tagFAttachSoulProto* pSoulProto = GetFuhunStoneInfo(pItem->pProtoType->dwTypeID);
	if( !P_VALID(pSoulProto) )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_CannotFindFuhunInfo;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_CannotFindFuhunInfo;
	}

	INT LevelAdding = 0;
	if( g_world.IsGodStrengthenOpen() )
	{
		if( pEquip->equipSpec.byQuality >= g_world.GetGodStrengthenEquipMinQlty() )
			LevelAdding = pEquip->equipSpec.byGodStrengthenLevel;
	}

	INT nMaxConLevel = pEquip->equipSpec.bSoaringFlag ? tagEquipSpec::ECL_AftConMaxLevel : (tagEquipSpec::ECL_BefConMaxLevel + LevelAdding);
	if( nMaxConLevel < tagEquipSpec::ECL_BefConMaxLevel + LevelAdding )
		nMaxConLevel = tagEquipSpec::ECL_BefConMaxLevel + LevelAdding;
	else if( nMaxConLevel > tagEquipSpec::ECL_AftConMaxLevel )
		nMaxConLevel = tagEquipSpec::ECL_AftConMaxLevel;
	nMaxConLevel = min(nMaxConLevel,pEquip->pEquipProto->nMaxConsolidateLevel + LevelAdding);

	const tagEquipQltyEffect* pEquipQltyEffect = g_attRes.GetEquipQltyEffect(pEquip->equipSpec.byQuality);
	if( P_VALID(pEquipQltyEffect) )
		nMaxConLevel = min(nMaxConLevel,pEquipQltyEffect->n8StrengLevelLimit + LevelAdding);

	if( pSoulProto->dwConLevel > nMaxConLevel )
		return E_ADSoul_Equip_FuhunLevelToHight;

	if( pSoulProto->dwConLevel <= pEquip->equipSpec.byConsolidateLevel )
	{
		backMsg.dwErrorCode = E_ADSoul_Equip_FuhunLevelLow;
		pRole->SendMessage(&backMsg,backMsg.dwSize);
		return E_ADSoul_Equip_FuhunLevelLow;
	}

	if( g_world.IsGodStrengthenOpen() && pSoulProto->dwConLevel > tagEquipSpec::ECL_BefConMaxLevel && !pEquip->equipSpec.bSoaringFlag )
	{
		if( pEquip->equipSpec.byGodStrengthenLevel == 0 || pSoulProto->dwConLevel - tagEquipSpec::ECL_BefConMaxLevel > pEquip->equipSpec.byGodStrengthenLevel )
		{
			// �����Ը��꣬װ�������ȼ�����
			backMsg.dwErrorCode = E_ADSoul_Equip_GodStrength_NotEnough;
			pRole->SendMessage(&backMsg,backMsg.dwSize);
			return E_ADSoul_Equip_GodStrength_NotEnough;
		}
	}

	if( pItem->bLock )
	{
		if( !pEquip->bLock )
		{
			pEquip->bLock = 1;
			pEquip->dwUnlockTime = 0;
			if( pEquip->eStatus == EUDBS_Null )
				pEquip->eStatus = EUDBS_Update;
			tagNS_LockItem ret;
			ret.dwErrorCode = E_Success;
			ret.byType = 0;
			ret.dwUnlockTime = 0;
			ret.n64Serial = pEquip->n64Serial;
			pRole->SendMessage( &ret, ret.dwSize );
		}
	}

	// ���ݸ���ʯ��״̬�޸�װ���İ�״̬
	if ( pItem->IsBind() || pItem->IsAccountBind())
	{
		pEquip->byBind = EBS_AttachSoulBind;
		// �ı�װ���İ�״̬��֪ͨDB�Ϳͻ���
		pEquip->SetUpdate(EUDBS_Update);
		itemMgr.SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);
	}
	else
	{
		// �ж�װ���Ƿ����а󶨵ı�ʯ
		bool bHasBindGemItem = false;
		for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
		{
			if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
			{
				continue;
			}
			BYTE byGemBindType = g_attRes.GetItemProto(pEquip->equipSpec.dwHoleGemID[i])->byBindType;
			if ( byGemBindType == EBM_Gain || byGemBindType == EBM_AccountBind )
			{
				bHasBindGemItem = true;
				break;
			}
		}

		// �������ʯΪ�ǰ󶨣�װ��Ϊ�����
		if ( pEquip->IsAttachSoulBind() )
		{
			// ������а󶨱�ʯ����ԭΪ��ͨ��״̬������ԭΪװ������Ĭ�ϵİ�״̬
			if (bHasBindGemItem)
			{
				pEquip->byBind = EBS_Bind;				
			}
			else
			{
				pEquip->Bind();			
			}	
			// �ı�װ���İ�״̬��֪ͨDB			
			pEquip->SetUpdate(EUDBS_Update);
			itemMgr.SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);

		}

	}


	pEquip->equipSpec.byConsolidateLevel = pSoulProto->dwConLevel;
	if( pEquip->equipSpec.byConsolidateLevel > nMaxConLevel )
		pEquip->equipSpec.byConsolidateLevel = nMaxConLevel;

	// ����װ������
	pRole->CalEquipFlare(pEquip);

	//const tagFConsolidateProto * pConFormula = GetFConsolidateOneFormula(pEquip,pEquip->equipSpec.byConsolidateLevel);
	ItemCreator::RecalEquipStrengthAtt(pEquip,pEquip->equipSpec.byConsolidateLevel);
	itemMgr.DelFromBag(pRecv->n64Stone,ELCID_FCompose_Fuhun,1);
	itemMgr.UpdateEquipSpec(*pEquip);

	backMsg.n64ID = pRecv->n64Equip;

	backMsg.dwErrorCode = E_Success;
	pRole->SendMessage(&backMsg,backMsg.dwSize);

	return E_Success;
}
// ������߸���
DWORD PlayerSession::HandleDetachOrAttackSoulEquip(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DetachOrAttackSoul_Equip);
	if( P_VALID(pRecv) )
	{
		if( pRecv->bDetachOrAttack )
			return HandleDetachSoulFromEquip(pCmd,GetRole());
		return HandleAttachSoulToEquip(pCmd,GetRole());
	}
	return GT_INVALID;
}

//------------------------------------------------------------------------
// ��Ԫ
//------------------------------------------------------------------------
DWORD PlayerSession::HandleGuiYuan(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_GuiYuan);

	tagNS_GuiYuan send;
	send.dwErrorCode = E_Success;

	ItemMgr& sItemMgr = m_pRole->GetItemMgr();
	tagEquip* pEquipment = static_cast<tagEquip*>(sItemMgr.GetBagItem(pRecv->n64Equip));
	if (!P_VALID(pEquipment))
	{
		send.dwErrorCode = E_Consolidate_GuiYuanEquip_Not_Exist;
	}
	else
	{
		BYTE byBrandLvl = pEquipment->GetBrandLevel();
		if (pEquipment->pEquipProto->bOnlyScriptStrengthen)
		{
			send.dwErrorCode = E_OnlyCanStrengthenByScript;
		}
		else if (byBrandLvl > 0)
		{
			if (!P_VALID(sItemMgr.GetBagItem(pRecv->n64Stone)))
			{
				send.dwErrorCode = E_Consolidate_GuiYuanStone_Not_Exist;
			}
			else
			{
				pEquipment->ReSetBrand();
				
				// ���¼������ֵ
				m_pRole->CalEquipFlare(pEquipment);

				send.dwErrorCode = E_Success;
				sItemMgr.UpdateEquipSpec(*pEquipment);
			}
		}
		else
		{
			send.dwErrorCode = E_Consolidate_Brand_Not_Exit;
		}
	}

	if (send.dwErrorCode == E_Success)
	{
		sItemMgr.DelFromBag(pRecv->n64Stone, ELCID_Compose_Brand, 1);
	}

	m_pRole->SendMessage(&send, send.dwSize);
	return 0;
}
// ������ҩ����,ϴ����������
DWORD PlayerSession::HandleFairyPowder(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyPowder);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	ItemMgr & itemMgr = m_pRole->GetItemMgr();

	tagItem * pFabaoItem = itemMgr.GetBagItem(pRecv->n64Fairy);
	tagNS_FairyPowder backmsg;
	if( !P_VALID(pFabaoItem) )
	{
		backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_NotInBag;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}
	if( MIsEquipment(pFabaoItem->dwTypeID) && P_VALID(pFabaoItem->pProtoType) && MIsFaBao(pFabaoItem) )
	{
		tagFabao * pFabao = (tagFabao *) pFabaoItem;
		tagItem * pItem = itemMgr.GetBagItem(pRecv->n64FairyPowder);
		if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
		{
			return GT_INVALID;
		}
		//if( g_world.GetFabaoLevelLimit() <= pFabao->n16Stage )
		//	return E_Fabao_RaiseIntelligence_AlreadyFull;

		if( pItem->pProtoType->eSpecFunc != EISF_FairyPowder )
		{
			backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_FairyPowderNotExist;
			m_pRole->SendMessage(&backmsg,backmsg.dwSize);
			return E_Fabao_RaiseIntelligence_FairyPowderNotExist;
		}

		if ( (pFabao->n16NativeIntelligence >= 100 && pItem->pProtoType->nSpecFuncVal2 !=2) 
			|| (pItem->pProtoType->nSpecFuncVal2 == 2 && pFabao->n16NativeIntelligence < 100))
		{
			backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_FairyPowderNotFit;
			m_pRole->SendMessage(&backmsg,backmsg.dwSize);
			return E_Fabao_RaiseIntelligence_FairyPowderNotFit;
		}

		if( itemMgr.IsItemCDTime(pItem->pProtoType->dwTypeID ) )
		{
			backmsg.dwErrorCode = E_UseItem_CoolDowning;
			m_pRole->SendMessage(&backmsg,backmsg.dwSize);
			return E_UseItem_CoolDowning;
		}

		bool bDel = true;
		//INT nLastLevel = pFabao->n16Stage;
		INT nOldZhiZhi = pFabao->n16NativeIntelligence;
		
		if( !g_world.GetFabaoFoodQualifFlag() )
			backmsg.dwErrorCode = ItemCreator::ChangeFabaoNativeIntelligence(pFabao,1);
		else
			backmsg.dwErrorCode = ItemCreator::ChangeFabaoNativeIntelligence(pFabao,1,pItem->dwSpecVal1);

		// ��ʱ���Ӽ�¼log
		if (0 == backmsg.dwErrorCode && pFabao->n16NativeIntelligence>120)
		{
			ILOG->Write(_T("\r\n����(%d)���ʱ仯����Ϊ%d, ��ɫ(%d)ʹ����Ʒ:%d.\n"), 
				pFabao->dwTypeID, pFabao->n16NativeIntelligence, m_pRole->GetID(), pItem->pProtoType->dwTypeID);
		}

		// ���ʷ����仯�����¼���
		ItemCreator::RecalFaBaoCurStoredExp(pFabao);

		switch(backmsg.dwErrorCode )
		{
		case ItemCreator::EFABAO_RaiseIntelligence_AlreadyFull:
			backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_AlreadyFull;
			bDel = false;
			break;
		case ItemCreator::EFABAO_RaiseIntelligence_Failse:
			backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_Failse;
			break;
		case ItemCreator::EFABAO_RaiseIntelligence_NoChange:
			backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_NoChange;
			//bDel = false;
			break;
		case ItemCreator::EFABAO_RaiseIntelligence_Success:
			backmsg.dwErrorCode = E_Success;
			break;
		default:
			backmsg.dwErrorCode = GT_INVALID;
			bDel = false;
			break;
		}
		// ȥ��ϴ����ʱ�������ж�
#if 0
		if(g_world.GetFabaoElemStuntFlag() && pFabao->n16Stage > 59)
		{
			if( nOldZhiZhi != pFabao->n16NativeIntelligence )
			{
				if( pFabao->n16NativeIntelligence < 80 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
					//pFabao->dwFairySkill[2] = 0;
				}
				else if( pFabao->n16NativeIntelligence < 90 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
				}
				else if( pFabao->n16NativeIntelligence < 95 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
				}
				//if( /*nOldZhiZhi < 60 && */pFabao->n16Stage >= 60 && pFabao->n16Stage < 70) // �������bug����ô�����ú�����
				//{
				//	if( P_VALID( ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID ) )
				//	{
				//		//for( int nSkillN = 0; nSkillN < MaxFairySkillNum; ++nSkillN )
				//			if( pFabao->dwFairySkill[0] == 0 )
				//				pFabao->dwFairySkill[0] = ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID;
				//	}
				//}
			}
			//if( nLastLevel != pFabao->n16Stage )
			//{
			//	pFabao->byDamageUpgradeTimes		+= pFabao->n16Stage - nLastLevel;
			//	pFabao->byResistanceUpgradeTimes	+= pFabao->n16Stage - nLastLevel;
			//	if( nLastLevel < 60 )
			//	{
			//		pFabao->dwFairySkill[0] = 0;
			//		pFabao->dwFairySkill[1] = 0;
			//	}
			//	ItemCreator::RealizeFabaoStunt(pFabao,nLastLevel);
			//	if( pFabao->n16Stage % 10 == 0  || (nLastLevel / 10 != pFabao->n16Stage / 10) )
			//		itemMgr.UpdateFabaoNirVanaData2DB(*pFabao);
			//}
		}
#endif
		if( nOldZhiZhi != pFabao->n16NativeIntelligence )
		{
			if( pFabao->n16NativeIntelligence < 80 )
			{
				for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
					if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
						pFabao->dwFairySkill[nSkillNdx] = 0;
				//pFabao->dwFairySkill[2] = 0;
			}
			else if( pFabao->n16NativeIntelligence < 90 )
			{
				for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
					if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
						pFabao->dwFairySkill[nSkillNdx] = 0;
			}
			else if( pFabao->n16NativeIntelligence < 95 )
			{
				for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
					if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
						pFabao->dwFairySkill[nSkillNdx] = 0;
			}
			else if( pFabao->n16NativeIntelligence >= 95 )
			{
				for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
					if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
						pFabao->dwFairySkill[nSkillNdx] = 0;
			}
		}

		itemMgr.UpdateFabaoSpec(*pFabao);

		m_pRole->SendMessage(&backmsg,backmsg.dwSize);

		// ������Ʒ������ȴʱ��
		itemMgr.Add2CDTimeMap(pItem->pProtoType->dwTypeID);

		if( bDel )
			itemMgr.DelFromBag(pRecv->n64FairyPowder,ELCID_Fabao_FairyPowderCosted,1);
		return E_Success;
	}

	return GT_INVALID;
}

DWORD PlayerSession::HandleFairyRaiseIntelligenceMax(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyRaiseIntelligenceMax);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	ItemMgr & itemMgr = m_pRole->GetItemMgr();

	tagItem * pFabaoItem = itemMgr.GetBagItem(pRecv->n64Fairy);
	tagNS_FairyRaiseIntelligenceMax backmsg;
	if( !P_VALID(pFabaoItem) || !MIsEquipment(pFabaoItem->dwTypeID) || !P_VALID(pFabaoItem->pProtoType) || !MIsFaBao(pFabaoItem) )
	{
		backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_NotInBag;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}
	
	tagFabao * pFabao = (tagFabao *) pFabaoItem;
	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64FairyPowder);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
	{
		return GT_INVALID;
	}

	if( pItem->pProtoType->eSpecFunc != EISF_FairyRaiseIntelligenceMax )
	{
		backmsg.dwErrorCode = E_Fabao_Spec98ItemNotExist;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_Fabao_Spec98ItemNotExist;
	}

	DWORD dwErrorCode = E_Fabao_Spec98Item_Success;
	switch(pItem->pProtoType->nSpecFuncVal1)
	{
	case 0:
		if (pFabao->n16NativeIntelligence <= 90 || pFabao->n16NativeIntelligence > 100)
		{
			dwErrorCode = E_Fabao_ItemNotFit2FairyIntellgence;
			break;
		}

		if (pFabao->byMaxIntelligence == 130)
		{
			dwErrorCode = E_Fabao_Spec98Item_AlreadyUse;
			break;
		}

		if(IUTIL->Rand() % 10000 < pItem->pProtoType->nSpecFuncVal2)
			pFabao->byMaxIntelligence = 130;
		else
			dwErrorCode = E_Fabao_Spec98Item_UseFailed;
		break;
	default:
		return GT_INVALID;
	}

	backmsg.dwErrorCode = dwErrorCode;
	m_pRole->SendMessage(&backmsg,backmsg.dwSize);
	if (dwErrorCode == E_Fabao_ItemNotFit2FairyIntellgence || dwErrorCode == E_Fabao_Spec98Item_AlreadyUse)
	{
		return dwErrorCode;
	}

	if(dwErrorCode == E_Fabao_Spec98Item_Success)
		itemMgr.UpdateFabaoSpec(*pFabao);

	itemMgr.DelFromBag(pRecv->n64FairyPowder,ELCID_Fabao_FairyPowderCosted,1);
	return dwErrorCode;
}

DWORD PlayerSession::HandleFairyDoRaiseIntelligence(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyDoRaiseIntelligence);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	ItemMgr & itemMgr = m_pRole->GetItemMgr();

	tagItem * pFabaoItem = itemMgr.GetBagItem(pRecv->n64Fairy);
	tagNS_FairyDoRaiseIntelligence backmsg;
	if( !P_VALID(pFabaoItem) || !MIsEquipment(pFabaoItem->dwTypeID) || !P_VALID(pFabaoItem->pProtoType) || !MIsFaBao(pFabaoItem) )
	{
		backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_NotInBag;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	tagFabao * pFabao = (tagFabao *) pFabaoItem;
	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64FairyPowder);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
	{
		return GT_INVALID;
	}

	if( pItem->pProtoType->eSpecFunc != EISF_FairyDoRaiseIntelligence )
	{
		backmsg.dwErrorCode = E_Fabao_Spec99ItemNotExist;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_Fabao_Spec99ItemNotExist;
	}

	if(pFabao->n16NativeIntelligence < pItem->pProtoType->nSpecFuncVal1 ||
		pFabao->n16NativeIntelligence > pItem->pProtoType->nSpecFuncVal2)
	{
		backmsg.dwErrorCode = E_Fabao_ItemNotFit2FairyIntellgence;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_Fabao_ItemNotFit2FairyIntellgence;
	}

	ItemCreator::ChangeFabaoNativeIntelligence(pFabao,1, -2);

	// ��ʱ���Ӽ�¼log
	if(pFabao->n16NativeIntelligence > 120)
		ILOG->Write(_T("\r\n����(%d)���ʱ仯����Ϊ%d, ��ɫ(%d)ʹ����Ʒ:%d.\n"), 
			pFabao->dwTypeID, pFabao->n16NativeIntelligence, m_pRole->GetID(), pItem->pProtoType->dwTypeID);

	backmsg.dwErrorCode = E_Fabao_RaiseIntelligence_Success;
	m_pRole->SendMessage(&backmsg,backmsg.dwSize);

	itemMgr.UpdateFabaoSpec(*pFabao);

	itemMgr.DelFromBag(pRecv->n64FairyPowder,ELCID_Fabao_FairyPowderCosted,1);
	return E_Success;
}

// ���������ȼ��仯��
enum { GodStrengthenErrorCode_0 = 0,		// û�����������Ǿ�������
	GodStrengthenErrorCode_Success = 1, // ������
	GodStrengthenErrorCode_E1 = -1,		// �����ϴ�������û�г���һ��
	GodStrengthenErrorCode_E2 = -2,		// �ڴ���ûǮ
	GodStrengthenErrorCode_E3 = -3,		// ����û�иı�
	GodStrengthenErrorCode_EOther = -4	// ��������
};
static INT GetGodStrengthenLevelUpInfo( tagEquip * pDes, tagEquip * pAssit,CurrencyMgr & curMgr, INT64 & money )
{
	if( !P_VALID(pDes) || !P_VALID(pAssit) )
		return GodStrengthenErrorCode_EOther;
	if( pDes->equipSpec.byGodStrengthenLevel > 10 )
		return GodStrengthenErrorCode_EOther;
	const tagGodStrengthen * pProto = g_attRes.GetGodStrengthenProto(pDes->equipSpec.byGodStrengthenLevel);
	if( !P_VALID(pProto) )
		return GodStrengthenErrorCode_EOther;
	if( pProto->dwExp == 0 )
		return GodStrengthenErrorCode_EOther;
	INT adding = 0;
	money = pProto->dwCost;
	switch( pAssit->equipSpec.byQuality )
	{
	case EIQ_Green:
		adding = pProto->dwGreenExp;
		break;
	case EIQ_Blue:
		adding = pProto->dwBlueExp;
		break;
	case EIQ_Orange:
		adding = pProto->dwOrangeExp;
		break;
	case EIQ_Purple:
		adding = pProto->dwPurpleExp;
		break;
	case EIQ_Pink:
		adding = pProto->dwPinkExp;
		break;
	}
	DWORD dwOldV = pDes->equipSpec.dwCurGodStrengthenExp;
	pDes->equipSpec.dwCurGodStrengthenExp += adding;
	if( pDes->equipSpec.dwCurGodStrengthenExp >= pProto->dwExp )
	{
		// ȥ����ʱ�ж�
#if 0
		if( ((DWORD)pDes->equipSpec.dwGodStrengthenTime > 0) && CalcTimeDiff(g_world.GetWorldTime(),pDes->equipSpec.dwGodStrengthenTime) < g_world.GetGodStrengthenCoolDownTime() )
		{
			pDes->equipSpec.dwCurGodStrengthenExp -= adding;
			return GodStrengthenErrorCode_E1;
		}
		else
#else
		if( ((DWORD)pDes->equipSpec.dwGodStrengthenTime > 0) && CalcTimeDiff(g_world.GetWorldTime(),pDes->equipSpec.dwGodStrengthenTime) < g_world.GetGodStrengthenCoolDownTime() )
		{
			pDes->equipSpec.dwCurGodStrengthenExp = pProto->dwExp - 1;
			if( dwOldV == pDes->equipSpec.dwCurGodStrengthenExp )
				return GodStrengthenErrorCode_E3;
			return GodStrengthenErrorCode_0;
		}
#endif
		if( curMgr.GetBagSilver() < pProto->dwCost )
		{
			pDes->equipSpec.dwCurGodStrengthenExp -= adding;
			return GodStrengthenErrorCode_E2;
		}
		if( pDes->equipSpec.byLastGodStrengthenLevel < pDes->equipSpec.byGodStrengthenLevel )
			pDes->equipSpec.byLastGodStrengthenLevel = pDes->equipSpec.byGodStrengthenLevel;
		pDes->equipSpec.byGodStrengthenLevel++;
		pDes->equipSpec.dwCurGodStrengthenExp = pDes->equipSpec.dwCurGodStrengthenExp - pProto->dwExp;
		pProto = g_attRes.GetGodStrengthenProto(pDes->equipSpec.byGodStrengthenLevel);
		if( !P_VALID(pProto) )
			return GodStrengthenErrorCode_0;
		if( 0 == pProto->dwExp )
			pDes->equipSpec.dwCurGodStrengthenExp = 0;
		else if( pProto->dwExp <= pDes->equipSpec.dwCurGodStrengthenExp )
			pDes->equipSpec.dwCurGodStrengthenExp = pProto->dwExp - 1;
		return GodStrengthenErrorCode_Success;
	}
	if( dwOldV == pDes->equipSpec.dwCurGodStrengthenExp )
		return GodStrengthenErrorCode_E3;
	return GodStrengthenErrorCode_0;
}
// ����
DWORD	PlayerSession::HandleGodStrengthen(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	if(!g_world.IsGodStrengthenOpen())
		return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_GodStrengthen);

	ItemMgr & itemMgr = m_pRole->GetItemMgr();

	tagItem * pItem = NULL;
	tagEquip * pTobeGodEquip = NULL;
	tagEquip * pAssitEquip = NULL;

	tagNS_GodStrengthen backmsg;

	pItem = itemMgr.GetBagItem(pRecv->dwEquipID);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) || !MIsEquipment(pItem->pProtoType->dwTypeID) )
	{
		backmsg.dwErrorCode = E_Consolidate_Equip_Not_Exist;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_Consolidate_Equip_Not_Exist;
	}

	pTobeGodEquip = (tagEquip *)pItem;

	if (pTobeGodEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		backmsg.dwErrorCode = E_OnlyCanStrengthenByScript;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_OnlyCanStrengthenByScript;
	}

	pItem = itemMgr.GetBagItem(pRecv->dwAssitEquipID);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) || !MIsEquipment(pItem->pProtoType->dwTypeID) )
	{
		backmsg.dwErrorCode = E_Compose_Formula_Not_Exist;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_Compose_Formula_Not_Exist;
	}
	pAssitEquip = (tagEquip*)pItem;

	// A������ж�
	if( pTobeGodEquip->equipSpec.byQuality < g_world.GetGodStrengthenEquipMinQlty() )
	{
		backmsg.dwErrorCode = E_GodStrengthen_EquipQltyLower;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_EquipQltyLower;
	}
	if( pTobeGodEquip->equipSpec.byGodStrengthenLevel == 10 )
	{
		backmsg.dwErrorCode = E_GodStrengthen_EquipCannotStrengAgain;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_EquipCannotStrengAgain;
	}

	// B������ж�
	if( pAssitEquip->equipSpec.byQuality < g_world.GetGodStrengthenAssitEquipMinQlty() )
	{
		backmsg.dwErrorCode = E_GodStrengthen_AssitEquip_QltyLower;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_AssitEquip_QltyLower;
	}
	if( pAssitEquip->pEquipProto->byLevel < g_world.GetGodStengthenAssitEquipMinLevel() )
	{
		backmsg.dwErrorCode = E_GodStrengthen_AssitEquip_LevelLower;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_AssitEquip_LevelLower;
	}
	if( pAssitEquip->IsTimeLimit() )
	{
		backmsg.dwErrorCode = E_GodStrengthen_AssitEquip_TimelimForbided;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_AssitEquip_TimelimForbided;
	}
	if( pTobeGodEquip->pEquipProto->eEquipPos != pAssitEquip->pEquipProto->eEquipPos )
	{
		backmsg.dwErrorCode = E_GodStrengthen_EquipPosNotMatch;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_EquipPosNotMatch;
	}

	INT64 money = 0;

	switch( GetGodStrengthenLevelUpInfo(pTobeGodEquip,pAssitEquip,m_pRole->GetCurMgr(),money) )
	{
	case GodStrengthenErrorCode_E1:
		backmsg.dwErrorCode = E_GodStrengthen_EquipCannotLevelupInOneDay;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_GodStrengthen_EquipCannotLevelupInOneDay;
		break;
	case GodStrengthenErrorCode_E2:
		backmsg.dwErrorCode = E_Compose_NotEnough_Money;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return E_Compose_NotEnough_Money;
		break;
	case GodStrengthenErrorCode_Success:
		pTobeGodEquip->equipSpec.dwGodStrengthenTime = g_world.GetWorldTime();
		break;
	case GodStrengthenErrorCode_E3:
	case GodStrengthenErrorCode_EOther:
		backmsg.dwErrorCode = E_SystemError;
		m_pRole->SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	itemMgr.UpdateEquipSpec(*pTobeGodEquip);

	itemMgr.DelFromBag(pRecv->dwAssitEquipID,ELCID_GodStrengthen_Costed,1);
	if( money > 0 )
		m_pRole->GetCurMgr().DecBagSilver(money,ELCID_GodStrengthen_Costed);

	m_pRole->OnGodStrengthenSuccess(pTobeGodEquip);

	backmsg.dwErrorCode = E_Success;
	m_pRole->SendMessage(&backmsg,backmsg.dwSize);

	return E_Success;
}

struct tagFindFabaoFoodItemType
{
	bool operator () (const tagItem* pItem,DWORD dwType)
	{
		if(!P_VALID(pItem) || !P_VALID(pItem->pProtoType))
			return false;
		return pItem->pProtoType->dwTypeID == dwType;
	}
};

// ����ι�� ����
DWORD PlayerSession::HandleFabaoFeeding(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;
	if( !g_world.GetFabaoFoodQualifFlag() )
		return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_FabaoFeeding);

	ItemMgr & itemMgr = m_pRole->GetItemMgr();

	tagEquip * pEquip = itemMgr.GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	
	tagNS_FabaoFeeding msg;
	if( !P_VALID(pFabao) )
	{
		msg.dwBuffID = GT_INVALID;
		msg.dwErrorCode = E_FabaoFeeding_NoFabao;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoFeeding_NoFabao;
	}
	if( pFabao->n16Stage < 30 )
	{
		msg.dwErrorCode	= E_FabaoFeeding_FabaoLevelLimit;
		msg.dwBuffID = GT_INVALID;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoFeeding_FabaoLevelLimit;
	}

	TList<tagItem*> lstItem;
	int num = SearchPack(itemMgr.GetBag(),lstItem,pRecv->dwFoodTypeID,tagFindFabaoFoodItemType());
	if( num < 1 )
	{
		msg.dwBuffID = GT_INVALID;
		msg.dwErrorCode = E_FabaoFeeding_NoFood;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoFeeding_NoFood;
	}

	tagItem * pFoodItem = NULL;
	BOOL bFinded = FALSE;
	INT	nExp = 0;
	INT nMood = 0;
	INT nOldMood = pFabao->nMood;
	DWORD dwBuffID = GT_INVALID;
	DWORD dwTriggerID = GT_INVALID;
	lstItem.ResetIterator();
	while( lstItem.PeekNext(pFoodItem) )
	{
		if( P_VALID(pFoodItem->pProtoType) )
		{
			const tagFabaoFoodProto* pFoodProto = g_attRes.GetFabaoFoodProto(pFoodItem->pProtoType->dwTypeID);
			if( P_VALID(pFoodProto) )
			{
				if( pFoodProto->nLevel > pFabao->n16Stage )
					continue;
				for( int i = 0; i < 3 ; ++i )
				{
					if( pFoodProto->param[i].nMiniLevel <= pFabao->n16Stage && pFoodProto->param[i].nMaxLevel >= pFabao->n16Stage )
					{
						nExp = pFoodProto->nExp;
						dwTriggerID	= pFoodProto->dwTriggerID;
						dwBuffID		= pFoodProto->dwBuffID;
						nMood			= pFoodProto->param[i].nRecoveryValue;
						bFinded = TRUE;
						break;
					}
				}
			}
		}
		if( bFinded )
			break;
		pFoodItem = NULL;
	}

	if( !bFinded )
	{
		msg.dwBuffID = GT_INVALID;
		msg.dwErrorCode = E_FabaoFeeding_FoodLimit;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoFeeding_FoodLimit;
	}

	BOOL bBorthFood = FALSE;
	switch(pFoodItem->pProtoType->eSpecFunc)
	{
	case EISF_FairyDecPregnancy:
	case EISF_FairyIncBreedingTimes:
	case EISF_FairyDecRestTime:
		bBorthFood = TRUE;
		break;
	}

	if( !bBorthFood && pFabao->nMood > 1000 )
	{
		msg.dwErrorCode	= E_FabaoFeeding_Full;
		msg.dwBuffID = GT_INVALID;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoFeeding_Full;
	}

	if( pFoodItem->pProtoType->eSpecFunc == EISF_FairyDecPregnancy && P_VALID(pFoodItem->pProtoType->nSpecFuncVal1) ) // ��������
	{
		if( pFabao->eFabaoStatus == EEFS_Pregnancy )
		{
			if( pFabao->dwStatusChangeTime == 0 )
			{
				msg.dwBuffID = GT_INVALID;
				msg.dwErrorCode = E_FabaoFeeding_PregnancyTimeZero;
				SendMessage(&msg,msg.dwSize);
				pFabao->LogFairyBirth(pFabao->eFabaoStatus, EEFS_GiveBirth);
				pFabao->eFabaoStatus = EEFS_GiveBirth;
				itemMgr.UpdateFabaoSpec(*pFabao);
				return E_FabaoFeeding_PregnancyTimeZero;
			}
			if( FABAO_MATING_CD_STOP >= pFabao->dwStatusChangeTime )
			{
				msg.dwBuffID = GT_INVALID;
				msg.dwErrorCode = E_FabaoFeeding_MatingPregnancyCDTimeLT1h;
				SendMessage(&msg,msg.dwSize);
				return E_FabaoFeeding_MatingPregnancyCDTimeLT1h;
			}
			if( pFabao->dwStatusChangeTime <= pFoodItem->pProtoType->nSpecFuncVal1 )
			{
				pFabao->dwStatusChangeTime = 0;
				m_pRole->GetItemMgr().RemoveMatingFabao(pFabao);
				pFabao->LogFairyBirth(pFabao->eFabaoStatus, EEFS_GiveBirth);
				pFabao->eFabaoStatus = EEFS_GiveBirth;
			}
			else
				pFabao->dwStatusChangeTime -= pFoodItem->pProtoType->nSpecFuncVal1;
		}
		else
		{
			return GT_INVALID;
		}
	}
	if( pFoodItem->pProtoType->eSpecFunc == EISF_FairyIncBreedingTimes && P_VALID(pFoodItem->pProtoType->nSpecFuncVal1) ) // ������Դ���
	{
		//if(!((tagFabaoProto*)pFabao->pEquipProto)->bCanBirth)
		if(!pFabao->CanBirth())
		{
			msg.dwBuffID = GT_INVALID;
			msg.dwErrorCode = E_FabaoFeeding_NotBirthFabao;
			SendMessage(&msg,msg.dwSize);
			return E_FabaoFeeding_NotBirthFabao;
		}
		pFabao->nBirthTimes += (INT)pFoodItem->pProtoType->nSpecFuncVal1;
		pFabao->byIsGivenBirthTimes = 1;
	}
	if( pFoodItem->pProtoType->eSpecFunc == EISF_FairyDecRestTime && P_VALID(pFoodItem->pProtoType->nSpecFuncVal1) ) // �������ʱ��
	{
		if( pFabao->eFabaoStatus == EEFS_Rest )
		{
			if( pFabao->dwStatusChangeTime == 0 )
			{
				msg.dwBuffID = GT_INVALID;
				msg.dwErrorCode = E_FabaoFeeding_RestTimeZero;
				SendMessage(&msg,msg.dwSize);
				BOOL bNormalChg = pFabao->LogFairyBirth(pFabao->eFabaoStatus, EEFS_Normal);
				if (!bNormalChg)
					ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
				pFabao->eFabaoStatus = EEFS_Normal;
				itemMgr.UpdateFabaoSpec(*pFabao);
				//ILOG->Write(_T("FabaoFeeding1,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pFabao->nBirthTimes, pFabao->n64Serial);
				return E_FabaoFeeding_RestTimeZero;
			}
			if( FABAO_MATING_CD_STOP >= pFabao->dwStatusChangeTime )
			{
				msg.dwBuffID = GT_INVALID;
				msg.dwErrorCode = E_FabaoFeeding_MatingRestCDTimeLT1h;
				SendMessage(&msg,msg.dwSize);
				return E_FabaoFeeding_MatingRestCDTimeLT1h;
			}
			if( pFabao->dwStatusChangeTime <= pFoodItem->pProtoType->nSpecFuncVal1 )
			{
				pFabao->dwStatusChangeTime = 0;
				m_pRole->GetItemMgr().RemoveMatingFabao(pFabao);
				BOOL bNormalChg = pFabao->LogFairyBirth(pFabao->eFabaoStatus, EEFS_Normal);
				if (!bNormalChg)
					ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
				pFabao->eFabaoStatus = EEFS_Normal;
				//ILOG->Write(_T("FabaoFeeding2,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pFabao->nBirthTimes, pFabao->n64Serial);
			}
			else
				pFabao->dwStatusChangeTime -= pFoodItem->pProtoType->nSpecFuncVal1;
		}
		else
		{
			return GT_INVALID;
		}
	}

	if(!bBorthFood)
	{
		INT nHP = m_pRole->GetAttValue(ERA_HP);
		INT nMP = m_pRole->GetAttValue(ERA_MP);
		m_pRole->ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
		if( ItemCreator::ModFabaoMood(pFabao,nMood) >= 1000 )
		{
			const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffID);
			const tagTriggerProto * pTrigger = g_attRes.GetTriggerProto(dwTriggerID);
			m_pRole->TryAddBuff(m_pRole,pProto,pTrigger,NULL,NULL);
			pFabao->dwMoodBuffID	= dwBuffID;
		}
		else
		{
			pFabao->dwMoodBuffID	= GT_INVALID;
		}
		m_pRole->ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
		INT nMaxHP = m_pRole->GetAttValue(ERA_MaxHP);
		INT nMaxMP = m_pRole->GetAttValue(ERA_MaxMP);
		nHP = min(nHP, nMaxHP);
		nMP = min(nMP, nMaxMP);
		m_pRole->SetAttValue(ERA_HP,nHP/*,FALSE*/);
		m_pRole->SetAttValue(ERA_MP,nMP/*,FALSE*/);
	}

	itemMgr.DelFromBag(pFoodItem->n64Serial,ELCID_Fabao_Feeding,1);

	// ��Ϊ����������Ӱ�죬�����ø������������ṹ�ķ�������������Ч�ʺܵ�
	//itemMgr.SynchFabaoMood(*pFabao,nOldMood,TRUE);
	itemMgr.UpdateFabaoSpec(*pFabao);

	msg.dwErrorCode	= E_Success;
	msg.dwBuffID		= dwBuffID;
	SendMessage(&msg,msg.dwSize);

	return E_Success;
}


// �ı�Ԫ���˺�����
static DWORD GChangeFabaoEleInjuryType(tagNetCmd * pCmd,Role * ply,INT64 & n64SerialID,EElementInjury & eInjuryType)
{
	if( !P_VALID(ply) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyEleInjuryChange);

	n64SerialID = pRecv->n64FairyID;
	eInjuryType = EEI_Null;

	// ��õ�ͼ
	Map *pMap = ply->GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(pRecv->dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(ply) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_EleInjuryChangeNPC) )
		return E_Fabao_EleInjuryChange_NPCTypeErr;

	ItemMgr & itemMgr = ply->GetItemMgr();

	// ���������Ͳ���
	tagEquip * pEquip = (tagEquip*)itemMgr.GetBag().GetItem(pRecv->n64FairyID);
	tagItem * pStuffItem = itemMgr.GetBag().GetItem(pRecv->n64ItemID);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	else
		return E_Fabao_EleInjuryChange_FairyEquipErr;

	if( pFabao->n16Stage < 60 )
		return E_Fabao_EleInjuryChange_LevelLimit;

	if( !P_VALID(pStuffItem) || !P_VALID(pStuffItem->pProtoType) || pStuffItem->pProtoType->eSpecFunc != EISF_FairyCode )
		return E_Fabao_EleInjuryChange_StuffErr;

	switch( pRecv->eEleInjuryType )
	{
	case EEI_Fire:
		if( pStuffItem->pProtoType->nSpecFuncVal1 != 1 )
			return E_Fabao_EleInjuryChange_StuffErr;
		break;
	case EEI_Water:
		if( pStuffItem->pProtoType->nSpecFuncVal1 != 2 )
			return E_Fabao_EleInjuryChange_StuffErr;
		break;
	case EEI_Earth:
		if( pStuffItem->pProtoType->nSpecFuncVal1 != 3 )
			return E_Fabao_EleInjuryChange_StuffErr;
		break;
	case EEI_Wind:
		if( pStuffItem->pProtoType->nSpecFuncVal1 != 4 )
			return E_Fabao_EleInjuryChange_StuffErr;
		break;
	case EEI_Bright:
		if( pStuffItem->pProtoType->nSpecFuncVal1 != 5 )
			return E_Fabao_EleInjuryChange_StuffErr;
		break;
	case EEI_Dark:
		if( pStuffItem->pProtoType->nSpecFuncVal1 != 6 )
			return E_Fabao_EleInjuryChange_StuffErr;
		break;
	default:
		return GT_INVALID;
	}

	// ����Ǯ������
	INT nLevelDelta = pFabao->n16Stage - 60;
	INT64 nMoney = 400000 * nLevelDelta * nLevelDelta / 500;

	if( ply->GetCurMgr().GetBagSilver() < nMoney )
		return E_Fabao_EleInjuryChange_NotEnoughMoney;

	pFabao->eEleInjury = pRecv->eEleInjuryType;
	eInjuryType = pRecv->eEleInjuryType;
	ply->GetCurMgr().DecBagSilver(nMoney,ELCLD_Fabao_EleTypeChange_Costed);
	itemMgr.DelFromBag(pRecv->n64ItemID,ELCLD_Fabao_EleTypeChange_Costed,1);
	itemMgr.UpdateFabaoEleInjuryType2DB(*pFabao);
	itemMgr.UpdateFabaoSpec(*pFabao,FALSE);

	return E_Success;
}

// ����Ԫ���˺�����
DWORD PlayerSession::HandleFabaoEleInjuryType(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	if( !g_world.GetFabaoElemStuntFlag() )
		return GT_INVALID;

	tagNS_FairyEleInjuryChange msg;
	msg.dwErrorCode = GChangeFabaoEleInjuryType(pCmd,m_pRole,msg.n64FairyID,msg.eEleType);
	SendMessage(&msg,msg.dwSize);
	return msg.dwErrorCode;
}


// ��������������������
static void GAddFabaoFruitEffect(const tagItem * pItemFruit,const tagFabaoFruitProto * pFruitProto,INT& nMin,INT& nMax,INT nRaiseNdx)
{
	if( P_VALID(pItemFruit) && P_VALID(pItemFruit->pProtoType) )
	{
		if( EISF_FairyFruit == pItemFruit->pProtoType->eSpecFunc )
		{
			pFruitProto = g_attRes.GetFabaoFruitProto(pItemFruit->pProtoType->dwTypeID);
			if( P_VALID(pFruitProto) )
			{
				nMin += pFruitProto->nLower;
				nMax += pFruitProto->nHight;
				if(nRaiseNdx != GT_INVALID && nRaiseNdx == pFruitProto->nExtType )
				{
					nMax += pFruitProto->nExtAdding;
					nMin += pFruitProto->nExtAdding;
				}
			}
		}
	}
}

// ��������������Ԫ���˺��Լ����Ե�����
DWORD PlayerSession::HandleFabaoIncEleInjury(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyIncEleInjury);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	if( !g_world.GetFabaoElemStuntFlag() )
		return GT_INVALID;

	// ��������ʳ�������֮ʯ
	tagItem * pItemFruit1 = NULL, * pItemFruit2 = NULL, * pItemFruit3 = NULL;
	tagItem * pVerityOfStoneItem = NULL;
	tagNS_FairyIncEleInjury msg;
	msg.dwErrorCode = GT_INVALID;

	tagEquip * pEquip = m_pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	
	ItemMgr &itemMgr = m_pRole->GetItemMgr();

	if( P_VALID(pRecv->n64Fruit1) )
		pItemFruit1 = itemMgr.GetBagItem(pRecv->n64Fruit1);
	if( P_VALID(pRecv->n64Fruit2) )
		pItemFruit2 = itemMgr.GetBagItem(pRecv->n64Fruit2);
	if( P_VALID(pRecv->n64Fruit3) )
		pItemFruit3 = itemMgr.GetBagItem(pRecv->n64Fruit3);
	if( P_VALID(pRecv->n64VerityOfStone) )
		pVerityOfStoneItem = itemMgr.GetBagItem(pRecv->n64VerityOfStone);
	
	bool bIsEleInjury = false,bIsEleResis = false;
	INT nRaiseNdx = -1;
	bIsEleInjury = (pRecv->bAttType != 2);
	if( !bIsEleInjury )
		bIsEleResis = !(pRecv->bAttType != 2);
	// ����Ԫ���˺�
	nRaiseNdx = pRecv->byType;
	//if( (pRecv->u16AttNdx & 0xFF) >= EEI_START && (pRecv->u16AttNdx & 0xFF) < EEI_End )
	//{
	//	bIsEleInjury = true;
	//	nRaiseNdx = pRecv->u16AttNdx & 0xFF;
	//}
	//// ����Ԫ�ؿ���
	//else if( ((pRecv->u16AttNdx >> 8) & 0xFF) >= EER_START && ((pRecv->u16AttNdx >> 8) & 0xFF) < EER_End )
	//{
	//	bIsEleResis = true;
	//	nRaiseNdx = (pRecv->u16AttNdx >> 8) & 0xFF;
	//}
	if( nRaiseNdx == GT_INVALID )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	INT nMin = 0, nMax = 0;
	const tagFabaoFruitProto * pFruitProto = NULL;
	GAddFabaoFruitEffect(pItemFruit1,pFruitProto,nMin,nMax,nRaiseNdx);
	GAddFabaoFruitEffect(pItemFruit2,pFruitProto,nMin,nMax,nRaiseNdx);
	GAddFabaoFruitEffect(pItemFruit3,pFruitProto,nMin,nMax,nRaiseNdx);

	if( nMax == 0 || nMin == 0 )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if( P_VALID(pVerityOfStoneItem) && P_VALID(pVerityOfStoneItem->pProtoType) && EISF_VerityOfStone == pVerityOfStoneItem->pProtoType->eSpecFunc )
	{
		nMin += (nMax - nMin) / 2;
	}

	msg.dwErrorCode = E_Success;
	if( bIsEleInjury && pFabao->wDamageUpgradeTimes > 0 )
	{
		INT nHP = m_pRole->GetAttValue(ERA_HP);
		INT nMP = m_pRole->GetAttValue(ERA_MP);
		m_pRole->ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
		--pFabao->wDamageUpgradeTimes;
		msg.nAdded = IUTIL->RandomInRange(nMin,nMax);
		if (0 == pRecv->bAttType)
		{
			pFabao->nEleInjury += msg.nAdded;
		}
		else if (1 == pRecv->bAttType)
		{
			pFabao->nElePierce += msg.nAdded;
		}
		m_pRole->ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
		INT nMaxHP = m_pRole->GetAttValue(ERA_MaxHP);
		INT nMaxMP = m_pRole->GetAttValue(ERA_MaxMP);
		nHP = min(nHP, nMaxHP);
		nMP = min(nMP, nMaxMP);
		m_pRole->SetAttValue(ERA_HP,nHP/*,FALSE*/);
		m_pRole->SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 		m_pRole->RecalAtt();
	}
	else if( bIsEleResis && pFabao->wResistanceUpgradeTimes > 0 )
	{
		INT nHP = m_pRole->GetAttValue(ERA_HP);
		INT nMP = m_pRole->GetAttValue(ERA_MP);
		m_pRole->ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
		--pFabao->wResistanceUpgradeTimes;
		msg.nAdded = IUTIL->RandomInRange(nMin,nMax);
		pFabao->nEleResistance[nRaiseNdx] += msg.nAdded;
		m_pRole->ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
		INT nMaxHP = m_pRole->GetAttValue(ERA_MaxHP);
		INT nMaxMP = m_pRole->GetAttValue(ERA_MaxMP);
		nHP = min(nHP, nMaxHP);
		nMP = min(nMP, nMaxMP);
		m_pRole->SetAttValue(ERA_HP,nHP/*,FALSE*/);
		m_pRole->SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 		m_pRole->RecalAtt();
	}
	else
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	itemMgr.UpdateFabaoSpec(*pFabao);

	if( P_VALID(pRecv->n64Fruit1) )
		itemMgr.DelFromBag(pRecv->n64Fruit1,ELCLD_Fabao_IncEleAbility_Costed,1);
	if( P_VALID(pRecv->n64Fruit2) )
		itemMgr.DelFromBag(pRecv->n64Fruit2,ELCLD_Fabao_IncEleAbility_Costed,1);
	if( P_VALID(pRecv->n64Fruit3) )
		itemMgr.DelFromBag(pRecv->n64Fruit3,ELCLD_Fabao_IncEleAbility_Costed,1);
	if( P_VALID(pRecv->n64VerityOfStone) )
		itemMgr.DelFromBag(pRecv->n64VerityOfStone,ELCLD_Fabao_IncEleAbility_Costed,1);
	SendMessage(&msg,msg.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleFabaoAddPromoteTimes(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyAddPromoteTimes);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	if( !g_world.GetFabaoElemStuntFlag() )
		return GT_INVALID;

	tagEquip * pEquip = m_pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}

	tagNS_FairyAddPromoteTimes msg;
	msg.dwErrorCode = GT_INVALID;
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	ItemMgr &itemMgr = m_pRole->GetItemMgr();
	bool bIsInjury;
	if (1 == pRecv->byType)
	{
		bIsInjury = true;
	}
	else if (2 == pRecv->byType)
	{
		bIsInjury = false;
	}

	const tagFabaoAddPromoteTimesProto * pProto;
	if (bIsInjury)
	{
		pProto = g_attRes.GetFabaoAddPromoteTimesProto(pFabao->wEleInjPromoteTimes + 1);
	}
	else
	{
		pProto = g_attRes.GetFabaoAddPromoteTimesProto(pFabao->wEleResPromoteTimes + 1 + FABAO_RESIST_UPGRADE_BASE);
	}

	// �Ѵﵽ����������� [3/12/2012 zhangzhihua]
	if(!P_VALID(pProto))
	{
		msg.dwErrorCode = E_FairyAddPromoteTimes_ToMax;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// ��Ǯ���� [3/12/2012 zhangzhihua]
	const INT64 nMoney = pProto->dwMoney;
	if (nMoney > m_pRole->GetCurMgr().GetBagSilver())
	{
		msg.dwErrorCode = E_FairyAddPromoteTimes_LackMoney;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// ��Ǯ [3/12/2012 zhangzhihua]
	if (FALSE == m_pRole->GetCurMgr().DecBagSilver(nMoney, ELCLD_FairyAddPromoteTimes, m_pRole->GetID()))
	{
		msg.dwErrorCode = E_FairyAddPromoteTimes_LackMoney;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �������� [3/12/2012 zhangzhihua]
	if (bIsInjury)
	{
		pFabao->wEleInjPromoteTimes++;
		pFabao->wDamageUpgradeTimes++;
	}
	else
	{
		pFabao->wEleResPromoteTimes++;
		pFabao->wResistanceUpgradeTimes++;
	}

	itemMgr.UpdateFabaoSpec(*pFabao);

	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);

	return E_Success;
}


DWORD PlayerSession::HandleFabaoStuntConfirm(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyStuntConfirm);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	if( !g_world.GetFabaoElemStuntFlag() )
		return GT_INVALID;

	tagNS_FairyStuntConfirm msg;
	msg.bConfirmOrNot = pRecv->bConfirmOrNot;
	msg.dwErrorCode = GT_INVALID;

	tagEquip * pEquip = m_pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	if( !P_VALID(pFabao->dwUnsureSkill) )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if( pRecv->bConfirmOrNot == FALSE )
	{
		pFabao->dwUnsureSkill = GT_INVALID;
		m_pRole->GetItemMgr().UpdateFabaoSpec(*pFabao);
		msg.dwErrorCode = E_Success;
		SendMessage(&msg,msg.dwSize);
		return 0;
	}

	// ��ֹ��������ԭ���£���������-1���ǣ���֡�����
	if( pFabao->n16NativeIntelligence < 80 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}
	else if( pFabao->n16NativeIntelligence < 90 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}
	else if( pFabao->n16NativeIntelligence < 95 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}
	else if( pFabao->n16NativeIntelligence >= 95 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}

	vector<INT> vecUsed;
	vector<INT> vecUnused;
	bool bRepeat = false;
	for( int i = 0; i < MaxFairySkillNum; ++i )
	{
		if( P_VALID(pFabao->dwFairySkill[i]) )
			vecUsed.push_back(i);
		else if( pFabao->dwFairySkill[i] == 0 )
			vecUnused.push_back(i);

		if (pFabao->dwUnsureSkill == pFabao->dwFairySkill[i])
		{
			bRepeat = true;
			break;
		}
	}

	if (bRepeat)
	{
		msg.dwErrorCode = E_Fabao_StuntConfirm_SameID;
		SendMessage(&msg, msg.dwSize);
		return 0;
	}

	// ��δ���ؼ�
	m_pRole->ActiveFabaoStuntBuff(false,pFabao,EEP_Face);
	if( vecUnused.size() )
	{
		if( IUTIL->RandomInRange(1,100) < 51 ) // �����ϵ�
		{
			if( vecUsed.size() > 0 )
			{
				INT nd = vecUsed[IUTIL->RandomInRange( 0,vecUsed.size() - 1 )];
				const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[nd]);
				if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
					;
				else
					pFabao->dwFairySkill[nd] = pFabao->dwUnsureSkill;
			}
			else
			{
				const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[0]);
				if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
					;
				else
					pFabao->dwFairySkill[0] = pFabao->dwUnsureSkill;
			}
		}
		else // �µ�
		{
			pFabao->dwFairySkill[vecUsed.size()] = pFabao->dwUnsureSkill;
		}
	}
	// ֻ�����ؼ�
	else 
	{
		if( vecUsed.size() > 0 )
		{
			INT nd = vecUsed[IUTIL->RandomInRange( 0,vecUsed.size() - 1 )];
			const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[nd]);
			if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
				;
			else
				pFabao->dwFairySkill[nd] = pFabao->dwUnsureSkill;
		}
		else // ��ֹ���ⷢ����
			pFabao->dwFairySkill[0] =  pFabao->dwUnsureSkill;
	}
	m_pRole->ActiveFabaoStuntBuff(true,pFabao,EEP_Face);
	m_pRole->RecalAtt(TRUE);

	pFabao->dwUnsureSkill = GT_INVALID;
	m_pRole->GetItemMgr().UpdateFabaoSpec(*pFabao);
	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleFabaoStuntBook(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyStuntBook);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	if( !g_world.GetFabaoElemStuntFlag() )
		return GT_INVALID;

	tagNS_FairyStuntBook msg;
	msg.dwErrorCode = GT_INVALID;

	ItemMgr & itemMgr = m_pRole->GetItemMgr();
	tagItem * pItem = itemMgr.GetBagItem(pRecv->n64StuntBook);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) || EISF_FairyStuntBook != pItem->pProtoType->eSpecFunc )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	const tagFabaoStuntProto * pStuntProto = g_attRes.GetFabaoStuntProto(pItem->pProtoType->nSpecFuncVal1);
	if( !P_VALID(pStuntProto) )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	tagEquip * pEquip = m_pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if( pStuntProto->nIntelligenceLimit > pFabao->n16NativeIntelligence || pStuntProto->nStageLimit > pFabao->n16Stage )
	{
		msg.dwErrorCode = E_Fabao_StuntBook_LevelLimit;
		SendMessage(&msg,msg.dwSize);
		return E_Fabao_StuntBook_LevelLimit;
	}
	for( int i = 0; i < MaxFairySkillNum; ++i )
	{
		if( !P_VALID(pFabao->dwFairySkill[i]) )
			continue;
		if( pItem->pProtoType->nSpecFuncVal1 == pFabao->dwFairySkill[i] )
		{
			msg.dwErrorCode = E_Fabao_StuntBook_SameID;
			SendMessage(&msg,msg.dwSize);
			return E_Fabao_StuntBook_SameID;
		}
	}

	// ��ֹ��������ԭ���£���������-1���ǣ���֡�����
	if( pFabao->n16NativeIntelligence < 80 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}
	else if( pFabao->n16NativeIntelligence < 90 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}
	else if( pFabao->n16NativeIntelligence < 95 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}
	else if( pFabao->n16NativeIntelligence >= 95 )
	{
		for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
			if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
				pFabao->dwFairySkill[nSkillNdx] = 0;
	}

	vector<INT> vecUsed;
	vector<INT> vecUnused;
	for( int i = 0; i < MaxFairySkillNum; ++i )
	{
		if( P_VALID(pFabao->dwFairySkill[i]) )
		{
			const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[i]);
			if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
				continue;
			vecUsed.push_back(i);
		}
		else if( pFabao->dwFairySkill[i] == 0 )
			vecUnused.push_back(i);
	}

	// ��δ���ؼ�
	m_pRole->ActiveFabaoStuntBuff(false,pFabao,EEP_Face);
	if( vecUnused.size() )
	{
		if( IUTIL->RandomInRange(1,100) < 51 ) // �����ϵ�
		{
			if( vecUsed.size() > 0 )
			{
				INT nd = vecUsed[IUTIL->RandomInRange( 0,vecUsed.size() - 1 )];
				const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[nd]);
				if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
					;
				else
					pFabao->dwFairySkill[nd] = pItem->pProtoType->nSpecFuncVal1;
			}
			else
			{
				const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[0]);
				if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
					;
				else
					pFabao->dwFairySkill[0] = pItem->pProtoType->nSpecFuncVal1;
			}
		}
		else // �µ�
		{
			pFabao->dwFairySkill[vecUnused[0]] = pItem->pProtoType->nSpecFuncVal1;
		}
	}
	// ֻ�����ؼ�
	else 
	{
		if( vecUsed.size() > 0 )
		{
			INT nd = vecUsed[IUTIL->RandomInRange( 0,vecUsed.size() - 1 )];
			const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[nd]);
			if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
				;
			else
				pFabao->dwFairySkill[nd] = pItem->pProtoType->nSpecFuncVal1;
		}
		else // ��ֹ���ⷢ����
		{
			const tagFabaoStuntProto * pFabaoStunt_ = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[0]);
			if( P_VALID(pFabaoStunt_) && pFabaoStunt_->bBinding )
				;
			else
				pFabao->dwFairySkill[0] = pItem->pProtoType->nSpecFuncVal1;
		}
	}
	m_pRole->ActiveFabaoStuntBuff(true,pFabao,EEP_Face);
	m_pRole->RecalAtt(TRUE);

	//pFabao->dwUnsureSkill = GT_INVALID;
	m_pRole->GetItemMgr().UpdateFabaoSpec(*pFabao);
	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);
	itemMgr.DelFromBag(pRecv->n64StuntBook,ELCLD_Fabao_StuntBook,1);
	return E_Success;
}

DWORD PlayerSession::HandleFabaoNirvana(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyNirvana);

	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	if( !g_world.GetFabaoElemStuntFlag() )
		return GT_INVALID;

	if( !P_VALID(pRecv->n64NirvanID) )
		return GT_INVALID;

	tagItem * pItem = m_pRole->GetItemMgr().GetBagItem(pRecv->n64NirvanID);
	if( !P_VALID(pItem) || !P_VALID(pItem->pProtoType) )
		return GT_INVALID;
	if( pItem->pProtoType->eSpecFunc != EISF_FairyNirVana )
		return GT_INVALID;

	tagEquip * pEquip = m_pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		return GT_INVALID;
	}

	tagNDBC_GetFabaoNirVanaData msgDB;
	msgDB.bFirstOrLast = pItem->pProtoType->nSpecFuncVal1 == 0;
	msgDB.dwAccountID = GetSessionID();
	msgDB.dwRoleID		= m_pRole->GetID();
	msgDB.n64ID		= pFabao->n64Serial;
	msgDB.n64NirvanID	= pRecv->n64NirvanID;
	msgDB.nCurStage	= pFabao->n16Stage;
	g_dbSession.Send(&msgDB,msgDB.dwSize);
	return 0;
}

DWORD	PlayerSession::HandleRockStone(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_RockStone);

	ItemMgr & itemMgr = m_pRole->GetItemMgr();
	tagItem * pRockItem	= itemMgr.GetBagItem(pRecv->n64RockID);
	tagItem * pEquipItem	= itemMgr.GetBagItem(pRecv->n64EquipID);

	if( !P_VALID(pRockItem) || !P_VALID(pRockItem->pProtoType) )
		return GT_INVALID;
	if( !P_VALID(pEquipItem) || !P_VALID(pEquipItem->pProtoType) )
		return GT_INVALID;

	tagNS_RockStone msg;
	if( EISF_RockGod != pRockItem->pProtoType->eSpecFunc )
	{
		msg.dwErrorCode = E_RockStone_NotRockStone;
		SendMessage(&msg,msg.dwSize);
		return E_RockStone_NotRockStone;
	}

	if( !MIsEquipment(pEquipItem->pProtoType->dwTypeID) )
	{
		msg.dwErrorCode = E_RockStone_NotEquipment;
		SendMessage(&msg,msg.dwSize);
		return E_RockStone_NotEquipment;
	}

	tagEquip * pEquip = (tagEquip*)pEquipItem;
	if( pEquip->pProtoType->byMinUseLevel < pRockItem->pProtoType->nSpecFuncVal1 ||
		pEquip->pProtoType->byMinUseLevel > pRockItem->pProtoType->nSpecFuncVal2 )
	{
		msg.dwErrorCode = E_RockStone_LevelLimit;
		SendMessage(&msg,msg.dwSize);
		return E_RockStone_LevelLimit;
	}

	if (pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		msg.dwErrorCode = E_OnlyCanStrengthenByScript;
		SendMessage(&msg,msg.dwSize);
		return E_OnlyCanStrengthenByScript;
	}
	// ��ɫƷ����װ������ʹ����ʯ
	if (pEquip->equipSpec.byQuality <= EIQ_White)
	{
		msg.dwErrorCode = E_RockStone_WhiteQualityLimit;
		SendMessage(&msg,msg.dwSize);
		return E_RockStone_WhiteQualityLimit;
	}

	INT nEquipType [] = {EIT_Weapon,EIT_ClothArmor,EIT_Decoration,EIT_Armor,EIT_NoviceEquip};

	bool bFind = false;
	for( int i = 0; i < _countof(nEquipType); ++i )
	{
		if( pEquip->pProtoType->eType == nEquipType[i] )
		{
			bFind = TRUE;
			break;
		}
	}

	if( !bFind )
	{
		msg.dwErrorCode = E_RockStone_TypeMissMatch;
		SendMessage(&msg,msg.dwSize);
		return E_RockStone_TypeMissMatch;
	}

	itemMgr.DelFromBag(pRecv->n64RockID,ELCID_RockStone,1);

	ItemCreator::ResetEquipAtt(pEquip);
	itemMgr.UpdateEquipSpec(*pEquip);
	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);

	return E_Success;
}

// �鿴��������״̬
DWORD	PlayerSession::HandleFairyInquireSpirteBornState(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_InquireSpirteBornState);

	ItemMgr & itemMgr = m_pRole->GetItemMgr();
	tagEquip * pFabao = itemMgr.GetEquipBarEquip(pRecv->n64ExpBallSerialID);
	tagItem * pFabaoItem = NULL;
	if( !P_VALID(pFabao) )
	{
		pFabaoItem = itemMgr.GetBagItem(pRecv->n64ExpBallSerialID);
		if( !P_VALID(pFabaoItem) )
		{
			pFabaoItem = itemMgr.GetRoleWare().GetItem(pRecv->n64ExpBallSerialID);
		}
		if( !P_VALID(pFabaoItem) )
			return GT_INVALID;
		if( !P_VALID(pFabaoItem->pProtoType) )
			return GT_INVALID;
		if( !MIsEquipment(pFabaoItem->pProtoType->dwTypeID) )
			return GT_INVALID;
		if( !MIsFaBao(pFabaoItem) )
			return GT_INVALID;
		pFabao = (tagFabao*)pFabaoItem;
	}
	if( !P_VALID(pFabao) )
		return GT_INVALID;

	tagFabao * pFairy = (tagFabao*)pFabao;
	tagNS_InquireSpirteBornState msg;
	msg.eFabaoStatus		= pFairy->eFabaoStatus;
	msg.dwtagDwordTime	= pFairy->dwStatusChangeTime;
	SendMessage(&msg,msg.dwSize);
	return E_Success;
}
// ���� �������
DWORD	PlayerSession::HandleApplyFairyBirth(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_ApplyFairyBirth);

	// ��ȡ��ͼ
	Map *pMap = m_pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	tagNS_TransmitReplyFairyBirth msg;
	msg.dwInvitee = pRecv->dwRoleID;

	Role * pInvitee = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if( !P_VALID(pInvitee) )
	{
		msg.dwErrorCode = E_FabaoMating_NotOnline;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_NotOnline;
	}

	//��Ծ���
	if(!m_pRole->IsInDistance(*pInvitee, MAX_EXCHANGE_DISTANCE))
	{
		msg.dwErrorCode = E_FabaoMating_DistanceTooFar;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_DistanceTooFar;
	}
	tagNS_TransmitApplyFairyBirth msg_to;
	msg_to.dwApplicantID = m_pRole->GetID();
	pInvitee->SendMessage(&msg_to,msg_to.dwSize);

	FairyMatingMgr* pMatingMgr = FairyMatingMgr::Create();
	if( P_VALID(pMatingMgr) )
	{
		if(pMatingMgr->Find(pRecv->dwRoleID))
		{
			msg.dwErrorCode = FALSE;
			m_pRole->SendMessage(&msg,msg.dwSize);
			return E_Success;
		}
		pMatingMgr->Add(m_pRole->GetID());
	}
	return E_Success;
}
// Ӧ���������
DWORD	PlayerSession::HandleReplyFairyBirth(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_ReplyFairyBirth);

	Role * pInvitor = g_roleMgr.GetRolePtrByID(pRecv->dwApplicantID);
	if( !P_VALID(pInvitor) )
	{
		tagNS_FairyMatingCancelAll msg;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	tagNS_TransmitReplyFairyBirth msg;
	msg.dwInvitee = m_pRole->GetID();
	msg.dwErrorCode = pRecv->bAgreeOrNot;
	pInvitor->SendMessage(&msg,msg.dwSize);

	FairyMatingMgr* pMatingMgr = FairyMatingMgr::Create();
	if( P_VALID(pMatingMgr) )
	{
		if( pRecv->bAgreeOrNot == FALSE )
		{
			pMatingMgr->Remove(pRecv->dwApplicantID);
			return E_Success;
		}
		tagFairyInviteBirthData * pData = pMatingMgr->Find(pInvitor->GetID());
		if( P_VALID(pData) )
		{
			pData->dwInviteeID = m_pRole->GetID();
			pData->ResetTimeout() ;
			pMatingMgr->Mod(pRecv->dwApplicantID,pData);
		}
	}
	return E_Success;
}
// ��ȡ�������������Ϣ��ֻҪ�ú�������TRUE����Щָ������϶�����Ч�ģ�����᷵��FALSE�����������������
// ��������
DWORD	PlayerSession::HandlePutFairyInto(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_PutFairyInto);

	ItemMgr & itemMgr = m_pRole->GetItemMgr();
	tagEquip * pFabao = itemMgr.GetEquipBarEquip(pRecv->n64FairyID);
	tagItem * pFabaoItem = NULL;
	if( !P_VALID(pFabao) )
	{
		pFabaoItem = itemMgr.GetBagItem(pRecv->n64FairyID);
		if( !P_VALID(pFabaoItem) )
		{
			pFabaoItem = itemMgr.GetRoleWare().GetItem(pRecv->n64FairyID);
		}
		if( !P_VALID(pFabaoItem) )
			return GT_INVALID;
		if( !P_VALID(pFabaoItem->pProtoType) )
			return GT_INVALID;
		if( !MIsEquipment(pFabaoItem->pProtoType->dwTypeID) )
			return GT_INVALID;
		if( !MIsFaBao(pFabaoItem) )
			return GT_INVALID;
		pFabao = (tagFabao*)pFabaoItem;
	}
	if( !P_VALID(pFabao) )
		return GT_INVALID;

	tagNS_PutFairyInto msg;
	tagFabao * pFairy = (tagFabao*)pFabao;
	if( pFairy->equipSpec.byQuality < EIQ_Orange )
	{
		msg.dwErrorCode = E_FabaoMating_QltyLow;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_QltyLow;
	}
	if( pFairy->n16Stage < FABAO_MATING_MIN_STAGE )
	{
		msg.dwErrorCode = E_FabaoMating_LevelLow;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_LevelLow;
	}
	if( pFairy->n16NativeIntelligence < FABAO_MATING_MIN_INTELLIGENCE )
	{
		msg.dwErrorCode = E_FabaoMating_QualLow;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_QualLow;
	}
	//if( !((tagFabaoProto*)pFairy->pEquipProto)->bCanBirth )
	if(!pFairy->CanBirth())
	{
		msg.dwErrorCode = E_FabaoMating_CannotMating;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_CannotMating;
	}
	if( pFairy->nBirthTimes < 1 )
	{
		msg.dwErrorCode = E_FabaoMating_BirthTimesLow;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_BirthTimesLow;
	}
	if( pFairy->eFabaoStatus != EEFS_Normal )
	{
		msg.dwErrorCode = E_FabaoMating_CannotPutIn_ErrStatus;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_CannotPutIn_ErrStatus;
	}
	
	Role * pOther = NULL;
	tagFabao * pMyFabao, * pHisFabao;
	FairyMatingMgr* pMatingMgr = NULL;
	tagFairyInviteBirthData * pData = NULL;
	if( !GetFabaoMatingOtherInfo(m_pRole,pOther,pMyFabao,pHisFabao,pData,pMatingMgr) )
	{
		if( !P_VALID(pOther) || !P_VALID(pData) || !P_VALID(pMatingMgr) )
		{
			if( P_VALID(pMatingMgr) )
				pMatingMgr->Remove(m_pRole->GetID());
			tagNS_FairyMatingCancelAll back;
			SendMessage(&back,back.dwSize);
			return GT_INVALID;
		}
	}
	pData->ResetTimeout() ;
	if( pData->dwInviteeID == m_pRole->GetID() )
		pData->u64InviteeFairyID = pFairy->n64Serial;
	else
		pData->u64InvitorFairyID = pFairy->n64Serial;
	pMatingMgr->Mod(m_pRole->GetID(),pData);
	pMyFabao = pFairy;

	if( P_VALID(pOther) )
	{
		tagNS_TransmitPutFairyInto transmit_msg;
		transmit_msg.byQlty			= pFairy->equipSpec.byQuality;
		transmit_msg.dwFairyTypeID	= pFairy->pEquipProto->dwTypeID;
		transmit_msg.nBirthTimes		= pFairy->nBirthTimes;
		transmit_msg.nQualification	= pFairy->n16NativeIntelligence;
		pOther->SendMessage(&transmit_msg,transmit_msg.dwSize);
		//pMyFabao->eFabaoStatus		= EEFS_Waiting;
		//pMyFabao->dwStatusChangeTime = 30;
		//m_pRole->GetItemMgr().InsertMatingFabao(pMyFabao);
		msg.dwErrorCode = E_Success;
		SendMessage(&msg,msg.dwSize);
		return E_Success;
	}
	else
	{
		tagNS_FairyMatingCancelAll back;
		SendMessage(&back,back.dwSize);
	}
	msg.dwErrorCode = GT_INVALID;
	SendMessage(&msg,msg.dwSize);
	return GT_INVALID;
}
// �������
static INT CalFabaoMatingValue(const tagFabao * pMain,const tagFabao * pSec);
DWORD	PlayerSession::HandleFairyMating(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyMating);

	//ItemMgr & itemMgr = m_pRole->GetItemMgr();
	tagNS_TransmitFairyMating msg;
	msg.bAgreement = pRecv->bAgreement;
	msg.dwRoleID	= m_pRole->GetID();

	tagFabao * pMyFabao = NULL;
	tagFabao * pHisFabao = NULL;
	
	Role * pOther = NULL;
	tagFairyInviteBirthData * pData = NULL;
	FairyMatingMgr* pMatingMgr = NULL;
	if( !GetFabaoMatingOtherInfo(m_pRole,pOther,pMyFabao,pHisFabao,pData,pMatingMgr) )
	{
		if( P_VALID(pMatingMgr) )
			pMatingMgr->Remove(m_pRole->GetID());
		tagNS_FairyMatingCancelAll back;
		SendMessage(&back,back.dwSize);
		if( P_VALID(pOther) )
		{
			pOther->SendMessage(&back,back.dwSize);
		}
		return GT_INVALID;
	}

	pOther->SendMessage(&msg,msg.dwSize);
	if( pRecv->bAgreement ) // ͬ��
	{
		pMyFabao->LogFairyBirth(pMyFabao->eFabaoStatus, EEFS_Waiting);
		pMyFabao->eFabaoStatus = EEFS_Waiting;
		//ILOG->Write(_T("FairyMating,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pMyFabao->nBirthTimes, pMyFabao->n64Serial);

		// ���������ߣ����״̬��Ҫ�����ˣ�ע������û�б��棬Ҳ������Ҫ˫��ȷ�Ϻ󣬲ű������ݿ⣬��֪ͨ�ͻ���
		if( pHisFabao->eFabaoStatus == EEFS_Waiting )
		{
			pMyFabao->LogFairyBirth(pMyFabao->eFabaoStatus, EEFS_Pregnancy);
			pMyFabao->eFabaoStatus = EEFS_Pregnancy;
			pMyFabao->dwStatusChangeTime = 24*60*60;
			pMyFabao->nBirthValue = CalFabaoMatingValue(pMyFabao,pHisFabao);
			if( pMyFabao->nBirthTimes > 0 )
			{
				pMyFabao->nBirthTimes--;
				pMyFabao->byIsGivenBirthTimes = 1;
			}
			m_pRole->GetItemMgr().UpdateFabaoSpec(*pMyFabao);
			m_pRole->GetItemMgr().InsertMatingFabao(pMyFabao);

			//ILOG->Write(_T("Fabao1,RoleID:%d, Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),m_pRole->GetID(), pMyFabao->nBirthTimes, pMyFabao->n64Serial);

			pHisFabao->LogFairyBirth(pHisFabao->eFabaoStatus, EEFS_Pregnancy);
			pHisFabao->eFabaoStatus = EEFS_Pregnancy;
			pHisFabao->dwStatusChangeTime = 24*60*60;
			pHisFabao->nBirthValue = CalFabaoMatingValue(pHisFabao,pMyFabao);
			if( pHisFabao->nBirthTimes > 0 )
			{
				pHisFabao->nBirthTimes--;
				pHisFabao->byIsGivenBirthTimes = 1;
			}
			pOther->GetItemMgr().UpdateFabaoSpec(*pHisFabao);
			pOther->GetItemMgr().InsertMatingFabao(pHisFabao);

			//ILOG->Write(_T("Fabao2,RoleID:%d, Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pOther->GetID(), pHisFabao->nBirthTimes, pHisFabao->n64Serial );

			tagNS_FairyMating back;
			SendMessage(&back,back.dwSize);
			pOther->SendMessage(&back,back.dwSize);
		}
		else
		{
			pMyFabao->dwStatusChangeTime = 30;
			m_pRole->GetItemMgr().InsertMatingFabao(pMyFabao);
		}
	}
	else
	{
		BOOL bNormalChg = pMyFabao->LogFairyBirth(pMyFabao->eFabaoStatus, EEFS_Normal);
		if (!bNormalChg)
			ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
		pMyFabao->eFabaoStatus = EEFS_Normal;
	}

	return E_Success;
}

DWORD PlayerSession::GetFairyQualityValue(BYTE byQuality)
{
	//Ʒ�ʶ�Ӧ ��=15 ��=25 ��=50
	DWORD dwRet = 0;
	switch (byQuality)
	{
	case EIQ_Orange:
		{
			dwRet = 15;
		}
		break;
	case EIQ_Purple:
		{
			dwRet = 50;
		}
		break;
	case EIQ_Pink:
		{
			dwRet = 100;
		}
		break;
	}

	return dwRet;
}

INT PlayerSession::GetFairyQuality(INT nValue, INT nNum)
{
	TMap<DWORD, tagFairyFusionQualityProto *> mapFairyFQuality = g_attRes.GetFairyFusionQualityProtos();
	TMap<DWORD, tagFairyFusionQualityProto *>::TMapIterator it = mapFairyFQuality.Begin();
	tagFairyFusionQualityProto* pTemp = NULL;

	while (mapFairyFQuality.PeekNext(it, pTemp))
	{
		if (!P_VALID(pTemp) || nValue < pTemp->n16LowScore || nValue >pTemp->n16HighScore)
		{
			continue;
		}

		if (nNum > 0 && 10000 > pTemp->n16QualityRate[EFairyFusion_Pink])
		{
			return -2;
		}
		INT nRnd = IUTIL->Rand() % 10000;
		for (INT i = 0; i < EFairyFusion_End; i++)
		{
			INT16 n16Qual = pTemp->n16QualityRate[i];
			if (EFairyFusion_Pink != i)
			{
				n16Qual -= nNum * 100;
			}
			if (nRnd < n16Qual)
			{
				return i;
			}
		}
	}

	return GT_INVALID;
}

DWORD PlayerSession::HandleNC_FairyFusion(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_FairyFusion);

	Role* pRole = GetRole();
	if(!P_VALID(pRole) || pRole->m_bIsInFairyFusion)
	{
		return GT_INVALID;
	}

	if(!g_world.IsFabaoOpen() || pRecv->n64FirstFairy == pRecv->n64SecondFairy)
		return GT_INVALID;

	ItemMgr &itemMgr = pRole->GetItemMgr();
	tagEquip * pEquip1 = (tagEquip*)itemMgr.GetBagItem(pRecv->n64FirstFairy);
	tagFabao * pFabao1 = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsFaBao(pEquip1) )
	{
		pFabao1 = (tagFabao*)pEquip1;
	}
	tagEquip * pEquip2 = (tagEquip*)itemMgr.GetBagItem(pRecv->n64SecondFairy);
	tagFabao * pFabao2 = NULL;
	if( P_VALID(pEquip2) && P_VALID(pEquip2->pEquipProto) && MIsFaBao(pEquip2) )
	{
		pFabao2 = (tagFabao*)pEquip2;
	}

	tagNS_FairyFusion msg;
	msg.dwErrorCode = GT_INVALID;

	bool bHaveIMID = false;
	if (P_VALID(pRecv->n64IMID))
	{
		tagItem * pItem = itemMgr.GetBagItem(pRecv->n64IMID);
		const tagItemProto *pItemProto = g_attRes.GetItemProto(pItem->dwTypeID);
		if(!P_VALID(pItemProto) ||
			EISF_FairyFusion != pItemProto->eSpecFunc ||
			pItem->GetNum() < pRecv->byItemNum)
		{
			msg.dwErrorCode = E_FairyFusion_CannotUseItem;
			SendMessage(&msg,msg.dwSize);
			return GT_INVALID;
		}
		bHaveIMID = true;
	}

	if(!P_VALID(pFabao1) || !P_VALID(pFabao2) ||
		pRecv->byItemNum > 99)
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	const tagFabaoProto * pProto1 = g_attRes.GetFabaoProto(pFabao1->dwTypeID);
	const tagFabaoProto * pProto2 = g_attRes.GetFabaoProto(pFabao2->dwTypeID);

	if(!P_VALID(pProto1) || !P_VALID(pProto2))
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}


	// ����Ʒ�ʲ��㣨Ҫ���ɫ���ϣ� [3/22/2012 zhangzhihua]
	if(pFabao1->equipSpec.byQuality < EIQ_Orange || pFabao2->equipSpec.byQuality < EIQ_Orange)
	{
		msg.dwErrorCode = E_FairyFusion_LowQuality;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �������ʲ���60 [3/22/2012 zhangzhihua]
	if(pFabao1->n16NativeIntelligence < 60  || pFabao2->n16NativeIntelligence < 60)
	{
		msg.dwErrorCode = E_FairyFusion_LowNativeIntelligence;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �����ȼ�����40 [3/22/2012 zhangzhihua]
	if(pFabao1->n16Stage < 40  || pFabao2->n16Stage < 40)
	{
		msg.dwErrorCode = E_FairyFusion_LowLevel;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �������������������������׶� [3/22/2012 zhangzhihua]
	if (EEFS_Normal != pFabao1->eFabaoStatus || 
		EEFS_Normal != pFabao2->eFabaoStatus)
	{
		msg.dwErrorCode = E_FairyFusion_StatusError;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �����������޷��ں� [3/22/2012 zhangzhihua]
	if (!(*pProto1).CanFusion() || !pProto2->CanFusion())
	{
		msg.dwErrorCode = E_FairyFusion_FairyError;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// ����ֵ=[��1����������+1�������ȼ�/2+1������Ʒ�ʶ�Ӧֵ��+��2����������+2�������ȼ�/2+2������Ʒ�ʶ�Ӧֵ��]/2
	// Ʒ�ʶ�Ӧ ��=15 ��=25 ��=50
	//---------------------------------------------------------------------------------
	INT nValue = (pFabao1->n16NativeIntelligence + pFabao1->n16Stage / 2 + GetFairyQualityValue(pFabao1->equipSpec.byQuality) +
					pFabao2->n16NativeIntelligence + pFabao2->n16Stage / 2 + GetFairyQualityValue(pFabao2->equipSpec.byQuality)) / 2;

	INT nQuality = GetFairyQuality(nValue, pRecv->byItemNum);

	if (-2 == nQuality)
	{
		msg.dwErrorCode = E_FairyFusion_CannotUseItem;
	}
	else if (GT_INVALID == nQuality)
	{
		// �ں�ʧ�ܺ��漴�۳�һֱ���ںϵ����� [3/22/2012 zhangzhihua]
		itemMgr.DelFromBag(pFabao1->n64Serial, ELCLD_Fabao_Fusion, 1);
		itemMgr.DelFromBag(pFabao2->n64Serial, ELCLD_Fabao_Fusion, 1);
		msg.dwErrorCode = E_FairyFusion_Faild;
	}
	else
	{
		// �漴�������� [3/23/2012 zhangzhihua]
		// �ںϳɹ���ֻ��������ʧ������������ [3/22/2012 zhangzhihua]
		DWORD dwTypeID = GT_INVALID;
		const tagFairyFusionTypeProto* pFairyTypeProto = g_attRes.GetFairyFusionTypeProto(nQuality + 1);
		if (!P_VALID(pFairyTypeProto))
		{
			SendMessage(&msg,msg.dwSize);
			return GT_INVALID;
		}

		const INT nRnd = IUTIL->Rand() % 10000;
		for (INT i = 0; i < MAX_FAIRY_FUSION_TYPE; i++)
		{
			if (nRnd < pFairyTypeProto->n16Rate[i])
			{
				dwTypeID = pFairyTypeProto->dwTypeID[i];
				break;
			}
		}
		if (!P_VALID(dwTypeID))
		{
			SendMessage(&msg,msg.dwSize);
			return GT_INVALID;
		}
		pRole->m_n64FusionFairySerial = pFabao1->n64Serial;
		//itemMgr.DelFromBag(pFabao1->n64Serial, ELCLD_Fabao_Fusion, 1);
		itemMgr.DelFromBag(pFabao2->n64Serial, ELCLD_Fabao_Fusion, 1);
		if (bHaveIMID)
		{
			itemMgr.DelFromBag(pRecv->n64IMID, ELCLD_Fabao_Fusion, pRecv->byItemNum);
		}

		tagItem * pNewFabao = NULL;
		pNewFabao = ItemCreator::Create(EICM_FairyFusion, pRole->GetID(),dwTypeID,1);
		if( P_VALID(pNewFabao) )
		{
			if (P_VALID(itemMgr.GetTempBag().GetItem((INT16)0)))
			{
				itemMgr.GetTempBag().Remove(itemMgr.GetTempBag().GetItem((INT16)0)->GetKey());
			}
			itemMgr.Add2TempBag(pNewFabao,ELCLD_Fabao_Fusion,FALSE);
		}

		pRole->m_bIsInFairyFusion = true;

		msg.dwErrorCode = E_Success;
	}

	SendMessage(&msg,msg.dwSize);
	return E_Success;
}

DWORD	PlayerSession::HandleNC_GetFairyFusion(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_GetFusionFairy);

	Role* pRole = GetRole();
	if(!P_VALID(pRole) || !pRole->m_bIsInFairyFusion)
	{
		return GT_INVALID;
	}


	if(!g_world.IsFabaoOpen())
		return GT_INVALID;

	ItemMgr &itemMgr = pRole->GetItemMgr();
	tagEquip * pEquip1 = (tagEquip*)itemMgr.GetBagItem(pRole->m_n64FusionFairySerial);
	tagFabao * pFabao1 = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsFaBao(pEquip1) )
	{
		pFabao1 = (tagFabao*)pEquip1;
	}
	tagEquip * pEquip2 = (tagEquip*)itemMgr.GetTempBag().GetItem((INT16)0);
	tagFabao * pFabao2 = NULL;
	if( P_VALID(pEquip2) && P_VALID(pEquip2->pEquipProto) && MIsFaBao(pEquip2) )
	{
		pFabao2 = (tagFabao*)pEquip2;
	}

	pRole->m_n64FusionFairySerial = GT_INVALID;
	pRole->m_bIsInFairyFusion = false;

	tagNS_GetFusionFairy msg;
	msg.dwErrorCode = GT_INVALID;

	if(!P_VALID(pFabao1) || !P_VALID(pFabao2))
	{
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if (pRecv->bWhichOne)
	{
		// ѡ�������� [3/29/2012 zhangzhihua]
		itemMgr.DelFromTempBag(pFabao2->GetKey(), ELCLD_Fabao_Fusion, 1);
	}
	else
	{
		itemMgr.DelFromBag(pFabao1->GetKey(), ELCLD_Fabao_Fusion, 1);
		if (itemMgr.GetBag().GetFreeSpaceSize() > 0)
		{
			itemMgr.Move2Other(EICT_TempBag, pFabao2->GetKey(), EICT_Bag, itemMgr.GetBagOneFreeSpace(), ELCLD_Fabao_Fusion);
			tagNDBC_NewFabao send;
			IFASTCODE->MemCpy(&send.fabao, pFabao2, SIZE_FABAO);

			g_dbSession.Send(&send, send.dwSize);

		}
	}

	msg.dwErrorCode = E_Success;
	SendMessage(&msg, msg.dwSize);

	return E_Success;
}

// ����ʥ��װ�����ĵ�һ����λ��
int GetPosForHolyEquip(tagHolyMan * pHoly)
{
	int retPos = GT_INVALID;
	for(int i=0; i<pHoly->n16EquipmentNumber; i++)
	{
		if (i >= MaxHolyEquipNum)
		{
			break;
		}
		if (pHoly->n64EquipSerialIDs[i] == 0)
		{
			retPos = i;
			break;
		}
	
	}

	return retPos;
}

// ��ʥ�鴩װ��
DWORD	PlayerSession::HandleNC_EquipHoly(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_EquipHoly);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr &itemMgr = pRole->GetItemMgr();

	tagEquip * pEquip1 =pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		pHoly = (tagHolyMan*)pEquip1;
	}

	tagEquip * pEquip2 = (tagEquip*)itemMgr.GetBagItem(pRecv->n64Equip);
	tagHolyEquip * pHolyEquip = NULL;
	if( P_VALID(pEquip2) && P_VALID(pEquip2->pEquipProto) && MIsHolyEquip(pEquip2) )
	{
		pHolyEquip = (tagHolyEquip*)pEquip2;
	}

	tagNS_EquipHoly msg;
	msg.dwErrorCode = GT_INVALID;

	if (pHoly == NULL || pHolyEquip == NULL)
	{
		msg.dwErrorCode = E_EquipHoly_EquipInValid;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	tagHolyEquipProto * pHolyEquipProto = (tagHolyEquipProto*)pHolyEquip->pProtoType;

	// �ж��Ƿ񳬹��˸�����ʥ�ƿ���ʥ�����ظ�װ��������
	int nLapCount = 0;
	for(int i=0; i<pHoly->n16EquipmentNumber; i++)
	{
		if (i >= MaxHolyEquipNum)
		{
			ILOG->Write(_T("PlayerSession::HandleNC_EquipHoly error: holynum is invalid!\r\n"));
			return GT_INVALID;
		}
		if (pHoly->n64EquipSerialIDs[i] != 0)
		{
			tagHolyEquip * pHolyEquipTmp = (tagHolyEquip*)itemMgr.GetHolyManBarItem(pHoly->n64EquipSerialIDs[i]);
			if (P_VALID(pHolyEquipTmp) && pHolyEquipTmp->dwTypeID == pHolyEquip->dwTypeID)
			{
				nLapCount++;
			}			
		}
	}
	if ( nLapCount >= pHolyEquipProto->n16WearLapNum)
	{
		// �����˸�����ʥ�ƿ���ʥ�����ظ�װ��������
		msg.dwErrorCode = E_EquipHoly_ExceedLapCount;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}	
	
	// �ж�ʥ���Ƿ����ʥ�����ɵȼ�Ҫ��
	if (pHoly->nDevourNum < pHolyEquipProto->nHolyLevel )
	{
		// �����ʥ�����ɵȼ�Ҫ�����
		msg.dwErrorCode = E_EquipHoly_HolyLowLevel;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if (pHoly->dwLevelUpAtt[ESAT_LingNeng] - pHoly->nCostHoly < pHolyEquip->nCostHoly)
	{
		// ʥ��ʣ�����ܲ���
		msg.dwErrorCode = E_EquipHoly_NotEnough;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// ����ʥ��װ�����ĵ�һ����λ��
	int nPos = GetPosForHolyEquip(pHoly);
	if (nPos == GT_INVALID)
	{
		// ʥ����װ������
		msg.dwErrorCode = E_EquipHoly_NotEmpty;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	// ��¼ʥ��64λID
	pHoly->n64EquipSerialIDs[nPos] = pRecv->n64Equip;
	// ����ʥ�ƣ��޸�ʥ�������
	pHoly->nCostHoly += pHolyEquip->nCostHoly;
	pHoly->dwLevelUpAtt[ESAT_Demage] += pHolyEquip->nHolyAttEffect[ESAT_Demage];
	pHoly->dwLevelUpAtt[ESAT_HolyDef] += pHolyEquip->nHolyAttEffect[ESAT_HolyDef];
	pHoly->dwLevelUpAtt[ESAT_Crit] += pHolyEquip->nHolyAttEffect[ESAT_Crit];
	pHoly->dwLevelUpAtt[ESAT_CritRate] += pHolyEquip->nHolyAttEffect[ESAT_CritRate];
	pHoly->dwLevelUpAtt[ESAT_ExDmg] += pHolyEquip->nHolyAttEffect[ESAT_ExDmg];
	pHoly->dwLevelUpAtt[ESAT_AttackTec] += pHolyEquip->nHolyAttEffect[ESAT_AttackTec];
	pHoly->dwLevelUpAtt[ESAT_NeglectToughness] += pHolyEquip->nHolyAttEffect[ESAT_NeglectToughness];

	// ʥ�������޸�֪ͨ�ͻ���
	itemMgr.SendHolySpec2Client(*pHoly);

	// ʥ�������޸�֪ͨDB
	itemMgr.SendHolySpec2DB(*pHoly);

	//// �ӱ�����ȡ��ʥ��
	//itemMgr.GetBag().Remove(pRecv->n64Equip);
	//// ��ʥ�Ʒŵ�ʥ��װ����
	//itemMgr.GetHolyManBar().Add(pHolyEquip, nPos);

	// ��ʥ�ƴӱ����Ƶ�ʥ��װ����	
	itemMgr.Move2Other(EICT_Bag, pRecv->n64Equip, EICT_HolyManEquip, GT_INVALID, ELCLD_Item_EquipHoly);

	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);
	return E_Success;
}

// ��ʥ��жװ��
DWORD	PlayerSession::HandleNC_UnEquipHoly(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_UnEquipHoly);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr &itemMgr = pRole->GetItemMgr();
	tagNS_UnEquipHoly msg;
	msg.dwErrorCode = GT_INVALID;

	// �жϱ����ռ乻�����Ų�������ʥ��
	if( itemMgr.GetBagFreeSize() < 1 )
	{
		msg.dwErrorCode = E_EquipHoly_SpaceNotEnough;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// �ж���ҵĽ�ҹ�����
	if( pRole->GetCurMgr().GetBagSilver() < UNEQUIP_HOLY_COST_SILVER )
	{
		msg.dwErrorCode = E_EquipHoly_SilverNotEnough;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;		
	}

	tagEquip * pEquip1 =itemMgr.GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		pHoly = (tagHolyMan*)pEquip1;
	}
	if(!P_VALID(pHoly))
	{
		return GT_INVALID;
	}

	if (pRecv->byIndex < 0 || pRecv->byIndex >= MaxHolyEquipNum)
	{
		return GT_INVALID;
	}
	tagItemProto * pItemProto = g_attRes.GetItemProto( pRecv->dwTypeID );
	tagHolyEquip * pHolyEquip = (tagHolyEquip*)itemMgr.GetHolyManBarItem(pHoly->n64EquipSerialIDs[pRecv->byIndex]);
	tagItem *pChaichuItem = itemMgr.GetItemByTypeID(pRecv->dwTypeID);
	if(!P_VALID(pHolyEquip) || !P_VALID(pChaichuItem))
	{
		return GT_INVALID;
	}
	
	if (pItemProto->eSpecFunc != EISF_ShengWenChaiChu)
	{
		msg.dwErrorCode = E_EquipHoly_UnEquipInValid;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}
	// �ж�ϴӡ����ʥ�Ƶ�Ʒ���Ƿ����
	if (pItemProto->byQuality != pHolyEquip->equipSpec.byQuality )
	{
		msg.dwErrorCode = E_EquipHoly_QualityNotEqual;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// ϴӡ������ʥ���޸�ʥ�����Ӧ����
	pHoly->n64EquipSerialIDs[pRecv->byIndex] = 0;
	// ����ʥ�ƣ��޸�ʥ�������
	pHoly->nCostHoly -= pHolyEquip->nCostHoly;
	pHoly->dwLevelUpAtt[ESAT_Demage] -= pHolyEquip->nHolyAttEffect[ESAT_Demage];
	pHoly->dwLevelUpAtt[ESAT_HolyDef] -= pHolyEquip->nHolyAttEffect[ESAT_HolyDef];
	pHoly->dwLevelUpAtt[ESAT_Crit] -= pHolyEquip->nHolyAttEffect[ESAT_Crit];
	pHoly->dwLevelUpAtt[ESAT_CritRate] -= pHolyEquip->nHolyAttEffect[ESAT_CritRate];
	pHoly->dwLevelUpAtt[ESAT_ExDmg] -= pHolyEquip->nHolyAttEffect[ESAT_ExDmg];
	pHoly->dwLevelUpAtt[ESAT_AttackTec] -= pHolyEquip->nHolyAttEffect[ESAT_AttackTec];
	pHoly->dwLevelUpAtt[ESAT_NeglectToughness] -= pHolyEquip->nHolyAttEffect[ESAT_NeglectToughness];	

	// ��ʥ�ƴ�ʥ��װ�����Ƶ�����
	INT16 n16DstPos = itemMgr.GetBagOneFreeSpace();
	itemMgr.Move2Other(EICT_HolyManEquip, pHolyEquip->n64Serial, EICT_Bag, n16DstPos, ELCLD_Item_UnEquipHoly);

	// ɾ����Ӧϴӡ��
	itemMgr.DelFromBag(itemMgr.GetItemByTypeID(pRecv->dwTypeID)->n64Serial,ELCID_ShengWenChaiChu,1);

	// �۳���ҽ��
	pRole->GetCurMgr().DecBagSilver(UNEQUIP_HOLY_COST_SILVER, ELCLD_UnEquipHoly);

	// ʥ�������޸�֪ͨ�ͻ���
	itemMgr.SendHolySpec2Client(*pHoly);

	// ʥ�������޸�֪ͨDB
	itemMgr.SendHolySpec2DB(*pHoly);


	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);
	return E_Success;	
}

// ��ʥ�Ƴ�����
DWORD	PlayerSession::HandleNC_LingNeng(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_LingNeng);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr &itemMgr = pRole->GetItemMgr();
	tagNS_LingNeng msg;
	msg.dwErrorCode = GT_INVALID;

	// �ж���ҵĽ�ҹ�����
	if( pRole->GetCurMgr().GetBagSilver() < ENHANCE_HOLY_EQUIP_COST_SILVER )
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_SilverNotEnough;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;		
	}

	// ��ȡʥ��
	tagEquip * pEquip1 =itemMgr.GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		pHoly = (tagHolyMan*)pEquip1;
	}
	if(!P_VALID(pHoly))
	{
		return GT_INVALID;
	}

	if (pRecv->byIndex < 0 || pRecv->byIndex >= MaxHolyEquipNum)
	{
		return GT_INVALID;
	}

	// ��ȡҪ���ܵ�ʥ��
	tagHolyEquip * pHolyEquip = (tagHolyEquip*)itemMgr.GetHolyManBarItem(pHoly->n64EquipSerialIDs[pRecv->byIndex]);
	if(!P_VALID(pHolyEquip))
	{
		return GT_INVALID;
	}

	tagHolyEquipProto * pHolyEquipProto = (tagHolyEquipProto*)pHolyEquip->pProtoType;
	// �жϳ��ܴ����Ƿ�ﵽ���ֵ
	if (pHolyEquip->n16EnhanceCount >= pHolyEquipProto->n16MaxLapNum)
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_CountExceed;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// �ж����ĵ������Ƿ��Ѿ��ﵽ��С����ֵ
	if (pHolyEquip->nCostHoly == pHolyEquipProto->nCostMinHolyValue)
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_MinHolyExceed;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// ��ȡ����ǿ������ 
	tagItem * pEnhanceItem = itemMgr.GetBagItem(pRecv->n64ItemId);
	if (!P_VALID(pEnhanceItem) || pEnhanceItem->pProtoType->eStuffType != EST_LingNeng )
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_ItemNotFit;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// ��ȡ����ǿ�����߶�Ӧ�ļ�������ֵ
	const tagHolyEquipEnhance * pEnhanceProto = NULL;
	pEnhanceProto = g_attRes.GetEquipEnhanceProto(pEnhanceItem->dwTypeID);
	if (pEnhanceProto == NULL)
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_ProtoNotFound;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	m_dwLingNeng = pEnhanceProto->dwLingNengPow;

	// ���ܳɹ�
	if (pHolyEquip->nCostHoly - m_dwLingNeng < pHolyEquipProto->nCostMinHolyValue)
	{
		/*pHolyEquip->nCostHoly -= nMinusHoly;*/
		m_dwLingNeng = pHolyEquip->nCostHoly - pHolyEquipProto->nCostMinHolyValue;	

	}

	m_bNeedConfirm = true;
	m_byIndex = pRecv->byIndex; 

	// ɾ����Ӧǿ������
	itemMgr.DelFromBag(pEnhanceItem->n64Serial,ELCID_DelEnhanceItem,1);

	// �۳���ҽ��
	pRole->GetCurMgr().DecBagSilver(ENHANCE_HOLY_EQUIP_COST_SILVER, ELCLD_EnhanceHolyEquip);

	msg.dwErrorCode = E_Success;
	msg.dwValue = m_dwLingNeng;
	SendMessage(&msg,msg.dwSize);
	return E_Success;
}

// ʥ�ƻ������Գ���
DWORD	PlayerSession::HandleNC_ChongNeng(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_ChongNeng);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr &itemMgr = pRole->GetItemMgr();
	tagNS_ChongNeng msg;
	msg.dwErrorCode = GT_INVALID;

	// �ж���ҵĽ�ҹ�����
	if( pRole->GetCurMgr().GetBagSilver() < ENHANCE_HOLY_EQUIP_COST_SILVER )
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_SilverNotEnough;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;		
	}

	// ��ȡʥ��
	tagEquip * pEquip1 =itemMgr.GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		pHoly = (tagHolyMan*)pEquip1;
	}
	if(!P_VALID(pHoly))
	{
		return GT_INVALID;
	}

	if (pRecv->byIndex < 0 || pRecv->byIndex >= MaxHolyEquipNum )
	{
		return GT_INVALID;
	}

	// ��ȡҪ���ܵ�ʥ��
	tagHolyEquip * pHolyEquip = (tagHolyEquip*)itemMgr.GetHolyManBarItem(pHoly->n64EquipSerialIDs[pRecv->byIndex]);
	if(!P_VALID(pHolyEquip))
	{
		return GT_INVALID;
	}

	tagHolyEquipProto * pHolyEquipProto = (tagHolyEquipProto*)pHolyEquip->pProtoType;
	// �жϳ��ܴ����Ƿ�ﵽ���ֵ
	if (pHolyEquip->n16EnhanceCount >= pHolyEquipProto->n16MaxLapNum)
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_CountExceed;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// �ж�ʥ������������Ƿ��������10
	if (pHoly->nCostHoly + COST_HOLY_ADD > pHoly->dwLevelUpAtt[ESAT_LingNeng])
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_MaxHolyExceed;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// ��ȡ���ܵ��� 
	tagItem * pEnhanceItem = itemMgr.GetBagItem(pRecv->n64ChongNeng);
	if (!P_VALID(pEnhanceItem) || pEnhanceItem->pProtoType->eStuffType != EST_ChongNeng )
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_ItemNotFit;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;	
	}

	// ��ȡ����ǿ�����߶�Ӧ���Ա仯��Χ
	const tagHolyEquipEnhance * pEnhanceProto = NULL;
	pEnhanceProto = g_attRes.GetEquipEnhanceProto(pEnhanceItem->dwTypeID);
	if (pEnhanceProto == NULL)
	{
		msg.dwErrorCode = E_EnhanceHolyEquip_ProtoNotFound;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �����Ƿ����ƫ����ߣ��޸Ĳ�ͬ�Ļ�������
	if ( GT_VALID(pRecv->n64PianXiang) && pRecv->byPianXiangIndex >= 0 && pRecv->byPianXiangIndex < X_ESAT_ATTA_NUM)
	{
		if (pHolyEquip->nHolyAttEffect[pRecv->byPianXiangIndex] == 0)
		{
			msg.dwErrorCode = E_EnhanceHolyEquip_ProtoNotExist;
			SendMessage(&msg,msg.dwSize);
			return GT_INVALID;
		}
		int nChgAtt = IUTIL->RandomInRange(pEnhanceProto->dwPowMin[pRecv->byPianXiangIndex], pEnhanceProto->dwPowMax[pRecv->byPianXiangIndex]);
		m_bNeedConfirm = true;
		m_byAttType = pRecv->byPianXiangIndex;
		m_dwAttValue = nChgAtt;
		//pHolyEquip->nHolyAttEffect[pRecv->byPianXiangIndex] += nChgAtt;
		//pHoly->dwLevelUpAtt[pRecv->byPianXiangIndex] += nChgAtt;
	}
	else
	{
		// ��ƫ����ߣ�ƽ�����������������ճ�������
		vector<EHolyAttType> vIndex;
		vIndex.clear();
		for (int i=0; i<X_ESAT_ATTA_NUM; ++i)
		{
			if (pHolyEquip->nHolyAttEffect[i] != 0 && pEnhanceProto->dwPowMin[i] != 0)
			{
				vIndex.push_back((EHolyAttType)i);
			}
		}

		// ��ʥ�Ƶ�����Ӱ��ֵȫΪ0���򷵻�
		if (vIndex.size() == 0)
		{
			ILOG->Write(_T("PlayerSession::HandleNC_ChongNeng error, nHolyAttEffect is not correct\r\n"));
			return GT_INVALID;
		}


		INT nRand = IUTIL->RandomInRange(0,vIndex.size()-1);
		if (nRand < 0 || nRand >= X_ESAT_ATTA_NUM)
		{
			ILOG->Write(_T("PlayerSession::HandleNC_ChongNeng error\r\n"));
			return GT_INVALID;
		}
		EHolyAttType nEnhanceIndex =  vIndex[nRand];

		int nChgAtt = IUTIL->RandomInRange(pEnhanceProto->dwPowMin[nEnhanceIndex], pEnhanceProto->dwPowMax[nEnhanceIndex]);
		/*pHolyEquip->nHolyAttEffect[nEnhanceIndex] += nChgAtt;
		pHoly->dwLevelUpAtt[nEnhanceIndex] += nChgAtt;	*/
		m_bNeedConfirm = true;
		m_byAttType = nEnhanceIndex;
		m_dwAttValue = nChgAtt;
	}
	m_byIndex = pRecv->byIndex; 

	// ɾ����Ӧǿ������
	itemMgr.DelFromBag(pEnhanceItem->n64Serial,ELCID_DelEnhanceItem,1);

	// �۳�ƫ�����
	if ( GT_VALID(pRecv->n64PianXiang) )
	{
		itemMgr.DelFromBag(pRecv->n64PianXiang,ELCID_DelPianXiangItem,1);	
	}

	// �۳���ҽ��
	pRole->GetCurMgr().DecBagSilver(ENHANCE_HOLY_EQUIP_COST_SILVER, ELCLD_EnhanceHolyEquip);

	msg.dwErrorCode = E_Success;
	msg.byAtt = m_byAttType;
	msg.dwAttValue = m_dwAttValue;
	SendMessage(&msg,msg.dwSize);
	return E_Success;
}

// ʥ�Ƴ���ȷ��
DWORD	PlayerSession::HandleNC_ChongNengResult(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_ChongNengResult);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ж��Ƿ��Ѿ�ȷ�Ϲ�
	if (!m_bNeedConfirm)
	{
		return GT_INVALID;
	}

	if (!pRecv->isConfirm)
	{
		m_bNeedConfirm = false;
		return GT_INVALID;
	}

	ItemMgr &itemMgr = pRole->GetItemMgr();
	// ��ȡʥ��
	tagEquip * pEquip1 =itemMgr.GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		pHoly = (tagHolyMan*)pEquip1;
	}
	if(!P_VALID(pHoly))
	{
		return GT_INVALID;
	}

	if (m_byIndex < 0 || m_byIndex >= MaxHolyEquipNum ||
		m_byAttType < 0 || m_byAttType >= X_ESAT_ATTA_NUM)
	{
		return GT_INVALID;
	}

	// ��ȡҪ���ܵ�ʥ��
	tagHolyEquip * pHolyEquip = (tagHolyEquip*)itemMgr.GetHolyManBarItem(pHoly->n64EquipSerialIDs[m_byIndex]);
	if(!P_VALID(pHolyEquip))
	{
		return GT_INVALID;
	}

	if (pRecv->isLingNeng)
	{
		// ���ܳɹ����޸�ʥ������
		pHolyEquip->nCostHoly -= m_dwLingNeng;
		// ���ܳɹ����޸�ʥ������
		pHoly->nCostHoly -= m_dwLingNeng;
	}
	else
	{
		// ���ܳɹ����޸�ʥ������
		pHolyEquip->nHolyAttEffect[m_byAttType] += m_dwAttValue;
		pHoly->dwLevelUpAtt[m_byAttType] += m_dwAttValue;

		// �������ĵ�����ֵ
		pHolyEquip->nCostHoly += COST_HOLY_ADD;
		pHoly->nCostHoly += COST_HOLY_ADD;
	}

	// �޸�ʥ�Ƶĳ��ܴ���
	pHolyEquip->n16EnhanceCount ++;

	// ʥ�������޸�֪ͨ�ͻ���
	itemMgr.SendHolyEquipSpec2Client(*pHolyEquip);
	// ʥ�������޸�֪ͨDB
	itemMgr.SendHolyEquipSpec2DB(*pHolyEquip);

	// ʥ�������޸�֪ͨ�ͻ���
	itemMgr.SendHolySpec2Client(*pHoly);
	// ʥ�������޸�֪ͨDB
	itemMgr.SendHolySpec2DB(*pHoly);	

	m_bNeedConfirm = false;
	return E_Success;
}

// ʥ����������
DWORD	PlayerSession::HandleNC_SLEatFairy(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_SLEatFairy);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	ItemMgr &itemMgr = pRole->GetItemMgr();

	tagEquip * pEquip1 =itemMgr.GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip1) && P_VALID(pEquip1->pEquipProto) && MIsHoly(pEquip1) )
	{
		pHoly = (tagHolyMan*)pEquip1;
	}

	// ���������÷�
	INT nFairyScore = 0;

	// ���ظ��ͻ�����Ϣ
	tagNS_SLEatFairy msg;
	msg.dwErrorCode = GT_INVALID;


	// ��ȡι��ʥ��ĵ���
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Fairy);
	if (pHoly == NULL || pItem == NULL || (pItem->pProtoType->eSpecFunc != EISF_ReplaceFabao && EIT_FaBao  != pItem->pProtoType->eType) )
	{
		msg.dwErrorCode = E_SLEatFairy_EquipInValid;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	// �ж����������Ǵ���������	
	if(!P_VALID(pItem) || pItem->pProtoType->eSpecFunc == EISF_ReplaceFabao)
	{
		nFairyScore = pItem->pProtoType->nSpecFuncVal1;		
	}
	else
	{
		if (!P_VALID(pItem) || EIT_FaBao  == pItem->pProtoType->eType)
		{
			tagFabao * pFabao = (tagFabao*)pItem;

			// �����ȼ�С��40������������
			if (pFabao->n16Stage < MIN_FABAO_LEVEL)
			{
				msg.dwErrorCode = E_SLEatFairy_lowLevel;
				SendMessage(&msg,msg.dwSize);
				return GT_INVALID;
			}

			CallEatFairyScore(pFabao->equipSpec.byQuality, pFabao->n16Stage, nFairyScore);
		}	
	}

	
	// �ж�ʥ�����ɴ����Ƿ��Ѵﵽ�ܴ���
	tagHolyProto * pHolyProto = (tagHolyProto*)pHoly->pProtoType;
	if (!P_VALID(pHolyProto))
	{
		return GT_INVALID;
	}
	if (pHoly->nDevourNum == pHoly->nMaxDevourNum)
	{
		msg.dwErrorCode = E_SLEatFairy_CountExceed;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

// 	if (pHoly->nToDayEatNum >= MAX_HOLYEAT_COUNT)	//������Ʋ�Ҫ��~
// 	{
// 		msg.dwErrorCode = E_SLEatFairy_CountNotEnough;
// 		SendMessage(&msg,msg.dwSize);
// 		return GT_INVALID;
// 	}	

	// ���������÷��ҵ���Ӧ��������������
	const tagHolyLevelUpProto * pLevelUpProto = NULL;
	pLevelUpProto = g_attRes.GetHolyLevelUpProto(nFairyScore);
	if (pLevelUpProto == NULL)
	{
		msg.dwErrorCode = E_SLEatFairy_GetProtoFailed;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	pHoly->nDevourNum++;
	pHoly->nToDayEatNum++;
	pHoly->dwLevelUpAtt[ESAT_Demage] += IUTIL->RandomInRange(pLevelUpProto->dwDmgMin, pLevelUpProto->dwDmgMax);
	pHoly->dwLevelUpAtt[ESAT_HolyDef] += IUTIL->RandomInRange(pLevelUpProto->dwDefMin, pLevelUpProto->dwDefMax);

	// ����ʥ�����������ֵ
	if ( pHoly->dwLevelUpAtt[ESAT_LingNeng] + IUTIL->RandomInRange(pLevelUpProto->dwLingNengMin, pLevelUpProto->dwLingNengMax) <= pHolyProto->nHolyMaxvalue)
	{
		pHoly->dwLevelUpAtt[ESAT_LingNeng] += IUTIL->RandomInRange(pLevelUpProto->dwLingNengMin, pLevelUpProto->dwLingNengMax);
	}
	else
	{
		pHoly->dwLevelUpAtt[ESAT_LingNeng] = pHolyProto->nHolyMaxvalue;	
	}
	
	pHoly->dwLevelUpAtt[ESAT_Crit] += IUTIL->RandomInRange(pLevelUpProto->dwCritMin, pLevelUpProto->dwCritMax);
	pHoly->dwLevelUpAtt[ESAT_CritRate] += IUTIL->RandomInRange(pLevelUpProto->dwCritValueMin, pLevelUpProto->dwCritValueMax);
	pHoly->dwLevelUpAtt[ESAT_ExDmg] += IUTIL->RandomInRange(pLevelUpProto->dwDmgAddMin, pLevelUpProto->dwDmgAddMax);
	pHoly->dwLevelUpAtt[ESAT_AttackTec] += IUTIL->RandomInRange(pLevelUpProto->dwAttackTecMin, pLevelUpProto->dwAttackTecMax);
	pHoly->dwLevelUpAtt[ESAT_NeglectToughness] += IUTIL->RandomInRange(pLevelUpProto->dwNeglectMin, pLevelUpProto->dwNeglectMax);

	// ʥ������ɵȼ�����ʱ���ٿ���һ��ʥ��װ����
	if (pHoly->nDevourNum == pHolyProto->nDevourNum && pHoly->n16EquipmentNumber < MaxHolyEquipNum)
	{
		pHoly->n16EquipmentNumber++;		
	}

	// ʥ�������޸�֪ͨ�ͻ���
	itemMgr.SendHolySpec2Client(*pHoly);

	// ʥ�������޸�֪ͨDB
	itemMgr.SendHolySpec2DB(*pHoly);

	// ɾ��ι��ʥ��ĵ���
	itemMgr.DelFromBag(pRecv->n64Fairy, (DWORD)ELCLD_HolyEatFairy, 1);	

	msg.dwErrorCode = E_Success;
	SendMessage(&msg,msg.dwSize);

	// ���ýű��¼�
	if(P_VALID(pRole->GetScript()))
	{
		pRole->GetScript()->OnGetHolyDevourNum(pRole, pHoly->nDevourNum);
	}
	
	return E_Success;
}

// �ٻ�ʥ���ȡ���ٻ�ʥ��
DWORD	PlayerSession::HandleNC_CallShengLing(tagNetCmd* pCmd)
{
	if(!P_VALID(pCmd)) return GT_INVALID;
	MGET_MSG(pRecv, pCmd, NC_CallShengLing);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRole->IsInRoleState(ERS_CALLHOLY))
	{
		// ȡ���ٻ�ʥ��
		pRole->UnsetRoleState(ERS_CALLHOLY, FALSE);
		// ֪ͨ�ͻ��� �ٻ�ʥ����ʧ
		tagNS_CallShengLingDisappear msg;
		msg.dwPlayerID = pRole->GetID();
		Map* pMap = pRole->GetMap();
		if (P_VALID(pMap))
		{
			pMap->SendBigVisTileMsg(pRole, &msg, msg.dwSize);
		}				
	}
	else
	{
		// �ٻ�ʥ��
		tagNS_CallShengLing msg;
		msg.dwPlayID = pRole->GetID();

		if (pRole->IsInRoleState(ERS_Mount))
		{
			// ���״̬�²����ٻ�ʥ��
			msg.dwShengLingTypeID = GT_INVALID;
			msg.dwErrorCode = E_CallShengLing_ForMountState;
			SendMessage(&msg,msg.dwSize);
			return GT_INVALID;
		}

// 		if (pRole->IsUseSkill())
// 		{
// 			//���ܶ���ʱ�����ٻ�ʥ��
// 			msg.dwShengLingTypeID = GT_INVALID;
// 			msg.dwErrorCode = E_CallShengLing_UseSkillState;
// 			SendMessage(&msg,msg.dwSize);
// 			return GT_INVALID;
// 		}
		ItemMgr &itemMgr = pRole->GetItemMgr();

		tagEquip * pEquip =pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);
		tagHolyMan * pHoly = NULL;
		if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsHoly(pEquip) )
		{
			pHoly = (tagHolyMan*)pEquip;
		}		

		// ʥ�鲻�����ٻ�����
		if (pHoly == NULL)
		{
			msg.dwShengLingTypeID = GT_INVALID;
			msg.dwErrorCode = E_CallShengLing_EquipInValid;
			SendMessage(&msg,msg.dwSize);
			return GT_INVALID;
		}

		msg.dwShengLingTypeID = pHoly->dwTypeID;
		msg.dwEatFairyNum = pHoly->nDevourNum;
		msg.dwErrorCode = GT_INVALID;

		// �������ϵ�ʥ��ֵ�����ٻ�ʥ��	//�����ж�ʥ��ֵ��
// 		if (pRole->GetAttValue(ERA_Holy) < HOLY_MIN_VALUE)
// 		{
// 			msg.dwErrorCode = E_CallShengLing_HolyNotEnough;
// 			SendMessage(&msg,msg.dwSize);
// 			return GT_INVALID;
// 		}

		// ���ý�ɫΪ�ٻ�ʥ��״̬
		pRole->SetRoleState(ERS_CALLHOLY, FALSE);

		msg.dwErrorCode = E_Success;
		//SendMessage(&msg,msg.dwSize);

		// �ٻ�ʥ��ɹ���Ҫ����Χ���˷���Ϣ
		Map* pMap = pRole->GetMap();
		if (P_VALID(pMap))
		{
			//// ���Ź����ڵ���ҷ���Ҫ���������
			//send.dwErrorCode = EATE_SendBigVisTileMsg;
			pMap->SendBigVisTileMsg(pRole, &msg, msg.dwSize);
		}
	}	

	return E_Success;
}

static INT CalFabaoMatingValue(const tagFabao * pMain,const tagFabao * pSec)
{
	if( !P_VALID(pMain) || !P_VALID(pSec) )
		return 0;
	INT16 n16NativeIntelligenceM = (pMain->n16NativeIntelligence > 100) ? 100 : pMain->n16NativeIntelligence;
	INT16 n16NativeIntelligenceS = (pSec->n16NativeIntelligence > 100) ? 100 : pSec->n16NativeIntelligence;
	return ( n16NativeIntelligenceM + pMain->pProtoType->byLevel / 2 + GetFairyMatingValue(pMain) ) * 8 + 
		( n16NativeIntelligenceS + pSec->pProtoType->byLevel / 2 + GetFairyMatingValue(pSec) ) * 2 ;
}


DWORD	PlayerSession::HandleCancelInvitingFairyMating(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyMatingCancelAll);

	Role * pOther = NULL;
	FairyMatingMgr* pMatingMgr = NULL;
	tagFabao * pMyFabao = NULL;
	tagFabao * pHisFabao = NULL;
	tagFairyInviteBirthData * pData = NULL;

	tagNS_FairyMatingCancelAll back;
	if( !GetFabaoMatingOtherInfo(m_pRole,pOther,pMyFabao,pHisFabao,pData,pMatingMgr) )
	{
		if( P_VALID(pMatingMgr) )
			pMatingMgr->Remove(m_pRole->GetID());
		if( !P_VALID(pOther) )
		{
			if( P_VALID(pMyFabao) )
			{
				BOOL bNormalChg = pMyFabao->LogFairyBirth(pMyFabao->eFabaoStatus, EEFS_Normal);
				if (!bNormalChg)
					ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
				//pMyFabao->eFabaoStatus = EEFS_Normal;	//���ﲻ���������״̬
				//ILOG->Write(_T("CancelInvitingFairyMating,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pMyFabao->nBirthTimes, pMyFabao->n64Serial);
				//pMyFabao->dwStatusChangeTime = 0;
				//m_pRole->GetItemMgr().UpdateFabaoSpec(*pMyFabao);
			}
			return GT_INVALID;
		}
	}
	if( P_VALID(pMyFabao) )
	{
		BOOL bNormalChg = pMyFabao->LogFairyBirth(pMyFabao->eFabaoStatus, EEFS_Normal);
		if (!bNormalChg)
			ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
		//pMyFabao->eFabaoStatus = EEFS_Normal;
		//ILOG->Write(_T("CancelInvitingFairyMating1,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pMyFabao->nBirthTimes, pMyFabao->n64Serial );
		//pMyFabao->dwStatusChangeTime = 0;
		//m_pRole->GetItemMgr().UpdateFabaoSpec(*pMyFabao);
	}
	if( P_VALID(pHisFabao) )
	{
		BOOL bNormalChg = pHisFabao->LogFairyBirth(pHisFabao->eFabaoStatus, EEFS_Normal);
		if (!bNormalChg)
			ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
		//pHisFabao->eFabaoStatus = EEFS_Normal;
		//ILOG->Write(_T("CancelInvitingFairyMating2,Fabao BirthTimes:%d, FabaoID:%.2X. \r\n"),pHisFabao->nBirthTimes, pHisFabao->n64Serial);
		//pHisFabao->dwStatusChangeTime = 0;
		//pOther->GetItemMgr().UpdateFabaoSpec(*pHisFabao);
	}
	pOther->SendMessage(&back,back.dwSize);
	pMatingMgr->Remove(m_pRole->GetID());
	return E_Success;
}
static INT GetFabaoQltyCostValue(tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return 1;
	switch( pFabao->equipSpec.byQuality )
	{
	case EIQ_Orange:
		return 1;
	case EIQ_Purple:
		return 2;
	case EIQ_Pink:
		return 4;
	}
	return 1;
}
DWORD	PlayerSession::HandleFairyBorn(tagNetCmd * pCmd)
{
	if( !g_world.IsFairyMatingOpen() )
		return GT_INVALID;

	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;
	MTRANS_POINTER(pRecv, pCmd, tagNC_FairyBorn);

	// ��õ�ͼ
	Map *pMap = m_pRole->GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	tagNS_FairyBorn msg;

	Creature* pNPC = pMap->FindCreature(pRecv->dwNPCID);
	if( !P_VALID(pNPC) ) 
	{
		msg.dwErrCode = E_Compose_NPC_Not_Exist;
		SendMessage(&msg,msg.dwSize);
		return E_Compose_NPC_Not_Exist;
	}

	if( FALSE == pNPC->CheckNPCTalkDistance(m_pRole) )
	{
		msg.dwErrCode = E_Compose_NPC_Distance;
		SendMessage(&msg,msg.dwSize);
		return E_Compose_NPC_Distance;
	}

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_SpriteBornNPC) )
	{
		msg.dwErrCode = E_Consolidate_NPCCanNotPosy;
		SendMessage(&msg,msg.dwSize);
		return E_Consolidate_NPCCanNotPosy;
	}

	Role * pOther = NULL;
	FairyMatingMgr* pMatingMgr = NULL;
	tagFabao * pMyFabao = NULL;
	tagFabao * pHisFabao = NULL;
	tagFairyInviteBirthData * pData = NULL;

	if( !GetFabaoMatingOtherInfo(m_pRole,pOther,pMyFabao,pHisFabao,pData,pMatingMgr) )
	{
		;
	}

	tagItem * pFairyItem = m_pRole->GetItemMgr().GetBagItem(pRecv->n64FairyID);
	if( MIsFaBao(pFairyItem) )
		pMyFabao = (tagFabao*)pFairyItem;
	else
	{
		msg.dwErrCode = GT_INVALID;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	INT64 n64Silver = 10000 * pMyFabao->n16NativeIntelligence / 10 * pMyFabao->pProtoType->byLevel / 10 * GetFabaoQltyCostValue(pMyFabao);
	if( m_pRole->GetCurMgr().GetBagSilver() < n64Silver )
	{
		msg.dwErrCode = E_BagSilver_NotEnough;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if( m_pRole->GetItemMgr().GetBagFreeSize() < 1 )
	{
		msg.dwErrCode = E_Bag_NotEnoughSpace;
		SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	if( pMyFabao->eFabaoStatus != EEFS_GiveBirth )
	{
		msg.dwErrCode = E_FabaoMating_NotInGiveBirth;
		SendMessage(&msg,msg.dwSize);
		return E_FabaoMating_NotInGiveBirth;
	}

	DWORD dwMyChildFabaoTypeID = 0; 
	DWORD dwFairyBorthType = 0;
	dwMyChildFabaoTypeID = g_attRes.RandomPeekFabaoBirthItem(pMyFabao->nBirthValue,pMyFabao->pProtoType->dwTypeID,&dwFairyBorthType);

	tagItem * pMyChildFabao = NULL;
	if( P_VALID(dwMyChildFabaoTypeID) )
	{
		pMyChildFabao = ItemCreator::Create(EICM_FairyBirth,m_pRole->GetID(),dwMyChildFabaoTypeID,1);
		if( P_VALID(pMyChildFabao) )
		{
			m_pRole->GetItemMgr().Add2Bag(pMyChildFabao,ELCLD_Fabao_Birth,TRUE);
			if(MIsFaBao(pMyChildFabao))
			{
				tagFabao * pMFabao = (tagFabao*)pMyChildFabao;
				if( dwFairyBorthType == FariyVariationType )
				{
					DWORD dwRoleID = m_pRole->GetID();
					DWORD dwTypeID = pMFabao->dwTypeID;
					DWORD dwBroadcastID = BROADCAST_FairyVariation;
					DWORD dwMsgInfoID =	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
					g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwBroadcastID);
					g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
					g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ItemTypeID, &dwTypeID);
					g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
				}
			}
		}
	}

	// ȫ���������
	if( P_VALID(pMatingMgr) && P_VALID(pHisFabao) && pHisFabao->eFabaoStatus == EEFS_Rest )
		pMatingMgr->Remove(m_pRole->GetID());

	pMyFabao->LogFairyBirth(pMyFabao->eFabaoStatus, EEFS_Rest);
	pMyFabao->eFabaoStatus = EEFS_Rest;
	pMyFabao->dwStatusChangeTime = 48 * 60 * 60;

	m_pRole->GetCurMgr().DecBagSilver( n64Silver, EICM_FairyBirth, m_pRole->GetID() );
	m_pRole->GetItemMgr().UpdateFabaoSpec(*pMyFabao);

	m_pRole->GetItemMgr().InsertMatingFabao(pMyFabao);

	msg.dwErrCode = E_Success;
	SendMessage(&msg,msg.dwSize);
	return E_Success;
}

DWORD PlayerSession::HandleEquipGrow(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_EquipGrow);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	TList<tagItem*> IMs;
	tagItemProto* pIMProto = NULL;

	DWORD dwErrorCode = E_Success;
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Equip);
	if (!P_VALID(pItem) || !MIsEquipment(pItem->dwTypeID))
	{
		dwErrorCode = E_EquipGrow_EquipInvalid;
		goto end;
	}

	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip->equipSpec.dwGrowTypeID))
	{
		dwErrorCode = E_EquipGrow_EquipInvalid;
		goto end;
	}

	if(P_VALID(pEquip) && MIsEquipment(pEquip->dwTypeID) && pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		dwErrorCode = E_OnlyCanStrengthenByScript;
		goto end;
	}

	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pEquip->equipSpec.dwGrowTypeID);
	if (!P_VALID(pGrowProto))
	{
		return GT_INVALID;
	}
	
	if (pEquip->equipSpec.byGrowLevel >= EQUIP_GROW_MAX_LEVEL)
	{
		dwErrorCode = E_EquipGrow_ReachTop;
		goto end;
	}

	//�жϽ�Ǯ�Ƿ�
	if(pRole->GetCurMgr().GetBagSilver() < pRecv->dwGrowValue*1500)
	{
		dwErrorCode = E_EquipGrow_ClipsInvalid;
		goto end;
	}

	INT32 nIMNum = 0;
	if (P_VALID(pRecv->dwIM))
	{
		pIMProto = g_attRes.GetItemProto(pRecv->dwIM);
		if (P_VALID(pIMProto) && pIMProto->eSpecFunc == EISF_EquipGrowLuckSign)
		{
			nIMNum = pRole->GetItemMgr().GetBagSameItemList(IMs, pRecv->dwIM);
		}
	}
	
	// ͳ�Ƹ�����ɫ����
	int nGray=0, nGreen=0, nBlue=0, nPurple = 0;
	for (int i=0; i<EQUIP_GROW_MAX_LEVEL-1; ++i)
	{
		switch(pEquip->equipSpec.eGrowStar[i])
		{
		case EEGS_Gray:
			++nGray;
			break;
		case EEGS_Green:
			++nGreen;
			break;
		case EEGS_Blue:
			++nBlue;
			break;
		case EEGS_Purple:
			++nPurple;
			break;
		default:
			break;
		}
	}

	// ���ӳɳ��ȣ�������,һ��ֻ����һ��
	tagItem* pIM = IMs.PopFront();

	pEquip->equipSpec.dwGrowValue += pRecv->dwGrowValue;
	++pEquip->equipSpec.nTotalGrowTimes;
	
	// ����Ƿ������
	float fpct = 1.0f;
	DWORD dwValueLine = 0;
	for (int i=0; i<pEquip->equipSpec.byGrowLevel+1; ++i)
	{
		dwValueLine += pGrowProto->dwExp[i]*fpct;
		//dwValueNow = (pEquip->equipSpec.dwGrowValue > dwValueLine) ? 0 : (dwValueLine - pEquip->equipSpec.dwGrowValue);

		// ������
		if (pEquip->equipSpec.byGrowLevel <= i && pEquip->equipSpec.dwGrowValue >= dwValueLine)
		{
			//dwValueNow = dwValueNow - dwValueLine;
			DWORD dwIMInput = (P_VALID(pIM)&&pIM->GetNum()>0)?pIM->dwTypeID : 0;
			EEquipGrowStar eNewStar = (EEquipGrowStar)pRole->OnRoleEquipGrow(pEquip->pProtoType->dwTypeID, pEquip->equipSpec.byQuality, pEquip->equipSpec.nTotalGrowTimes,
				pEquip->equipSpec.nTotalIMUseTimes, pEquip->equipSpec.byGrowLevel+1, nGray, nGreen, nBlue, nPurple, dwIMInput);

			if (!IsGrowStarValid(eNewStar))
			{
				dwErrorCode = E_EquipGrow_Error;
				goto end;
			}

			++(pEquip->equipSpec.byGrowLevel);

			if (pEquip->equipSpec.byGrowLevel < EQUIP_GROW_MAX_LEVEL)
			{
				pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1] = eNewStar;
				switch(eNewStar)
				{
				case EEGS_Gray:
					++nGray;
					break;
				case EEGS_Green:
					++nGreen;
					break;
				case EEGS_Blue:
					++nBlue;
					break;
				case EEGS_Purple:
					{
						++nPurple;
						if(nPurple > 0 && nPurple <= 10)	//�������ǵĻ���Ҫ����һ�´�log��
							g_world.SetPuperStarsNum(BYTE(nPurple-1));
					}
					break;
				default:
					break;
				}
			}		
			else if(pEquip->equipSpec.byGrowLevel == EQUIP_GROW_MAX_LEVEL)
			{
				pEquip->equipSpec.dwGrowValue = dwValueLine;
			}

			// ����im����
			if (P_VALID(pIM) && pEquip->equipSpec.byGrowLevel != EQUIP_GROW_MAX_LEVEL)	//�����һ�β����ǲ������ĵ���
			{
				INT16 n16Num = pIM->GetNum();
				pRole->GetItemMgr().ItemUsedFromBag(pIM->GetKey(), 1, ELCID_EquipGrow);
				++pEquip->equipSpec.nTotalIMUseTimes;
				--nIMNum;

				if (1 == n16Num)
				{
					pIM = IMs.PopFront();
				}
			}
		}
	}
	
	// ���Ľ�Ǯ
	pRole->GetCurMgr().DecBagSilver(pRecv->dwGrowValue*1500, pRecv->dwID);	//ÿһ��ɳ��ȶ�Ӧ1500��

	// ���ӳɳ�����
	for (int i=0; i<EQUIP_GROW_ADD_ATT_MAX_NUM; ++i)
	{
		if (P_VALID(pGrowProto->byAttOpenLevel[i]) && pEquip->equipSpec.byGrowLevel + 1 >= pGrowProto->byAttOpenLevel[i])
		{
			DWORD dwAttValue = 0;
			// �ۻ�֮ǰ���������Ժ�
			for (int j=0; j<pEquip->equipSpec.byGrowLevel; ++j)
			{
				float fpct = 1.0f;
				if(j>0) // ���ǵ�ʱ��
					MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[j-1], fpct);
				
				dwAttValue += pGrowProto->dwAttValue[i][j]*fpct;
			}
			// �ۼӱ���������ֵ
			DWORD dwExpValue = 0;
			for (int k=0; k<pEquip->equipSpec.byGrowLevel; ++k)
			{
				dwExpValue += pGrowProto->dwExp[k];
			}
			float fpct1 = 1.0f;
			if(pEquip->equipSpec.byGrowLevel>0)
				MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1], fpct1);
			dwAttValue += (pEquip->equipSpec.dwGrowValue - dwExpValue)*fpct1*(pGrowProto->dwAttValue[i][pEquip->equipSpec.byGrowLevel])/(pGrowProto->dwExp[pEquip->equipSpec.byGrowLevel]);
			
			pEquip->equipSpec.nGrowAttValue[i] = dwAttValue;
		}
	}
	pRole->GetItemMgr().UpdateEquipSpec(*pEquip);

end:
	tagNS_EquipGrow send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD PlayerSession::HandleEquipPerfectGrow(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_EquipPerfectGrow);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	tagItem* pIM = pRole->GetItemMgr().GetBagItem(pRecv->n64IMSerialID);
	if(!P_VALID(pIM) || pIM->pProtoType->eSpecFunc != EISF_PerfectGrow)
	{
		dwErrorCode = E_EquipPerfectGrow_IMInvalid;	//�� ���߲����ڻ��ߵ������ʹ���
		goto end;
	}

	tagEquip* pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(pRecv->n64EquipID);
	if(!P_VALID(pEquip) || pEquip->equipSpec.dwGrowTypeID == GT_INVALID || pEquip->equipSpec.dwGrowTypeID == 0 )
	{
		dwErrorCode = E_EquipPerfectGrow_EquipInvalid;	//װ�������ڻ򲻴��ڳɳ�����
		goto end;
	}

	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pEquip->equipSpec.dwGrowTypeID);
	if(!P_VALID(pGrowProto))
	{
		dwErrorCode = E_EquipPerfectGrow_EquipInvalid;	//װ��û�гɳ�����
		goto end;
	}

	if(pEquip->pProtoType->dwTimeLimit != GT_INVALID)
	{
		dwErrorCode = E_EquipPerfectGrow_TimeLimit;	//ʱ����װ�����ܸ���
		goto end;
	}

	if(pEquip->pEquipProto->bOnlyScriptStrengthen == TRUE)
	{
		dwErrorCode = E_EquipPerfectGrow_TimeLimit;
		goto end;
	}

	//�Ȱѵ��߿۵�
	pRole->GetItemMgr().ItemUsedFromBag(pIM->GetKey(), 1, ELCID_EquipPerfectGrow);
	//��ʼ����װ����~
	pEquip->equipSpec.byGrowLevel = pIM->pProtoType->nSpecFuncVal1;
	//�Ȱ�֮ǰ�������
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		pEquip->equipSpec.eGrowStar[i] = EEGS_Null;
	}
	for(int i = 0; i < pIM->pProtoType->nSpecFuncVal1; i++)
	{
		pEquip->equipSpec.eGrowStar[i] = EEGS_Purple;
	}
	pEquip->equipSpec.dwGrowValue = 0;
	for(int i = 0; i < pIM->pProtoType->nSpecFuncVal1; i++)
	{
		pEquip->equipSpec.dwGrowValue += pGrowProto->dwExp[i];
	}
	
	for (int i=0; i<EQUIP_GROW_ADD_ATT_MAX_NUM; ++i)
	{
		if (P_VALID(pGrowProto->byAttOpenLevel[i]) && pEquip->equipSpec.byGrowLevel + 1 >= pGrowProto->byAttOpenLevel[i])
		{
			DWORD dwAttValue = 0;
			// �ۻ�֮ǰ���������Ժ�
			for (int j=0; j<pEquip->equipSpec.byGrowLevel; ++j)
			{
				float fpct = 1.0f;
				if(j>0) // ���ǵ�ʱ��
					MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[j-1], fpct);

				dwAttValue += pGrowProto->dwAttValue[i][j]*fpct;
			}
			// �ۼӱ���������ֵ
			DWORD dwExpValue = 0;
			for (int k=0; k<pEquip->equipSpec.byGrowLevel; ++k)
			{
				dwExpValue += pGrowProto->dwExp[k];
			}
			float fpct1 = 1.0f;
			if(pEquip->equipSpec.byGrowLevel>0)
				MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1], fpct1);
			dwAttValue += (pEquip->equipSpec.dwGrowValue - dwExpValue)*fpct1*(pGrowProto->dwAttValue[i][pEquip->equipSpec.byGrowLevel])/(pGrowProto->dwExp[pEquip->equipSpec.byGrowLevel]);

			pEquip->equipSpec.nGrowAttValue[i] = dwAttValue;
		}
	}
	pRole->GetItemMgr().UpdateEquipSpec(*pEquip);

	
end:
	tagNS_EquipPerfectGrow send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

}


DWORD PlayerSession::HandleXiPurpleStar(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_XiPurpleStar);
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	tagItem* pIM = pRole->GetItemMgr().GetBagItem(pRecv->n64IMID);
	if(!P_VALID(pIM) || pIM->pProtoType->eSpecFunc != EISF_XiStarIM)
	{
		dwErrorCode = E_EquipXiStar_IMInvalid;	//���߲����ڻ��ߵ������ʹ���
		goto end;
	}

	tagEquip* pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(pRecv->n64EquipID);
	if(!P_VALID(pEquip) || pEquip->equipSpec.dwGrowTypeID == GT_INVALID || pEquip->equipSpec.dwGrowTypeID == 0 )
	{
		dwErrorCode = E_EquipXiStar_EquipInvalid;	//װ�������ڻ򲻴��ڳɳ�����
		goto end;
	}

	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pEquip->equipSpec.dwGrowTypeID);
	if(!P_VALID(pGrowProto))
	{
		dwErrorCode = E_EquipXiStar_EquipInvalid;	//װ��û�гɳ�����
		goto end;
	}

	if(pEquip->pProtoType->dwTimeLimit != GT_INVALID)
	{
		dwErrorCode = E_EquipXStar_TimeLimitEquip;	//ʱ����װ����������
		goto end;
	}

	if(pEquip->pEquipProto->bOnlyScriptStrengthen == TRUE)
	{
		dwErrorCode = E_EquipXStar_TimeLimitEquip;
		goto end;
	}

	//�ж���Ҫ����װ���Ƿ����㹻������
	int nStarNum = 0;
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		if(pEquip->equipSpec.eGrowStar[i] == EEGS_Purple)
		{
			nStarNum++;
		}
	}

	if(nStarNum < pIM->pProtoType->nSpecFuncVal1)
	{
		dwErrorCode = E_EquipXiStar_NumNotEnough;	//ԭװ������������������
		goto end;
	}

	//�������װ������
	pEquip->equipSpec.byGrowLevel = 0;
	pEquip->equipSpec.dwGrowValue = 0;
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		pEquip->equipSpec.eGrowStar[i] = EEGS_Null;
	}
	for (int i=0; i<EQUIP_GROW_ADD_ATT_MAX_NUM; ++i)
	{
		pEquip->equipSpec.nGrowAttValue[i] = 0;
	}
	//�۳�����
	pRole->GetItemMgr().ItemUsedFromBag(pIM->GetKey(), 1, ELCID_EquipXiStar);
	//�����µ���
	if (pIM->pProtoType->nSpecFuncVal2 != GT_INVALID)
	{
		tagItem* pNewFuXingItem = ItemCreator::Create(EICM_EquipXiStar, pRole->GetID(), pIM->pProtoType->nSpecFuncVal2, 1);
		if(P_VALID(pNewFuXingItem))
			dwErrorCode = pRole->GetItemMgr().Add2Bag(pNewFuXingItem, ELCID_EquipXiStar, TRUE);
		if(E_Success != dwErrorCode)
		{
			IMSG(_T("New Item Create Failedm, dwErrCode:%u\r\n"), dwErrorCode);
			::Destroy(pNewFuXingItem);
			return GT_INVALID;
		}
	}
	dwErrorCode = E_Success;
	pRole->GetItemMgr().UpdateEquipSpec(*pEquip);
end:
	tagNS_XiPurpleStar send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD PlayerSession::HandleRoleHolyStrength(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_HolyStrength)
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	DWORD dwErrorCode = E_Success;

	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if (!P_VALID(pItem) || pItem->pEquipProto->eSpecFunc != EISF_HolyStrength )
	{
		dwErrorCode = E_NotHolyStrengthItem;
		goto end;
	}

	tagHolyMan* pHoly = (tagHolyMan*)pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);
	if(!P_VALID(pHoly) || !MIsHoly(pHoly))
	{
		dwErrorCode = E_NotHolyItem;
		goto end;
	}

	tagHolyProto * pHolyProto = (tagHolyProto*)pHoly->pProtoType;
	if(!P_VALID(pHolyProto))
	{
		dwErrorCode = E_NotHolyItem;
		goto end;
	}


	if(pHoly->nMaxDevourNum < (pHolyProto->nDevourNum*1.5f) )
	{
		//ɾ������
		pRole->GetItemMgr().ItemUsedFromBag(pItem->GetKey(), 1, ELCID_HolyStrength);
		pHoly->nMaxDevourNum++;
	}
	else
	{
		dwErrorCode = E_HolyEatNumMax;
		goto end;
	}

	pRole->GetItemMgr().UpdateHolySpec(*pHoly);

end:
	tagNS_HolyStrength send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD	PlayerSession::HandleRoleHolyEquipmentNumber(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_HolyEquipmentNumber)
		Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	DWORD dwErrorCode = E_Success;

	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if (!P_VALID(pItem) || pItem->pEquipProto->eSpecFunc != EISF_HolyEquipmentNumber )
	{
		dwErrorCode = E_NotHolyEquipmentNumber;
		goto end;
	}

	tagHolyMan* pHoly = (tagHolyMan*)pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);
	if(!P_VALID(pHoly) || !MIsHoly(pHoly))
	{
		dwErrorCode = E_NotHolyItem;
		goto end;
	}

	if(pHoly->n16EquipmentNumber < MaxHolyEquipNum )
	{
		//ɾ������
		pRole->GetItemMgr().ItemUsedFromBag(pItem->GetKey(), 1, ELCID_HolyEquipmentNumber);
		pHoly->n16EquipmentNumber++;
	}
	else
	{
		dwErrorCode = E_HolyEquipmentNumberMax;
		goto end;
	}

	pRole->GetItemMgr().UpdateHolySpec(*pHoly);

end:
	tagNS_HolyEquipmentNumber send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD PlayerSession::HandleEquipGrowTrans(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_EquipGrowTrans)

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	if (pRecv->n64OldEquip == pRecv->n64NewEquip)
	{
		return GT_INVALID;
	}

	tagItem* pOldItem = pRole->GetItemMgr().GetBagItem(pRecv->n64OldEquip);
	if (!P_VALID(pOldItem) || !MIsEquipment(pOldItem->dwTypeID) )
	{
		dwErrorCode = E_EquipGrowTrans_OldEquipInvalid;
		goto end;
	}

	tagEquip* pOldEquip = (tagEquip*)pOldItem;
	if(P_VALID(pOldEquip) && MIsEquipment(pOldEquip->dwTypeID) && pOldEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		dwErrorCode = E_OnlyCanStrengthenByScript;
		goto end;
	}
// 	if(!P_VALID(pOldEquip->pEquipProto) || !pOldEquip->pEquipProto->bCanGrow)
// 	{
// 		dwErrorCode = E_EquipGrowTrans_OldEquipInvalid;
// 		goto end;
// 	}

 	if(!P_VALID(pOldEquip->equipSpec.dwGrowTypeID))
 	{
 		dwErrorCode = E_EquipGrowTrans_OldEquipInvalid;
 		goto end;
 	}

	tagItem* pNewItem = pRole->GetItemMgr().GetBagItem(pRecv->n64NewEquip);
	if (!P_VALID(pNewItem) || !MIsEquipment(pNewItem->dwTypeID))
	{
		dwErrorCode = E_EquipGrowTrans_NewEquipInvalid;
		goto end;
	}

	tagEquip* pNewEquip = (tagEquip*) pNewItem;
	if (!P_VALID(pOldEquip->pEquipProto) || !P_VALID(pNewEquip->pEquipProto) || 
		pOldEquip->pEquipProto->eEquipPos != pNewEquip->pEquipProto->eEquipPos)
	{
		dwErrorCode = E_EquipGrowTrans_PositionNoMatch;
		goto end;
	}

	if (pNewEquip->pEquipProto->byLevel < 60)
	{
		dwErrorCode = E_EquipGrowTrans_NewEquipLevelLimit;
		goto end;
	}

	tagItem* pIM = pRole->GetItemMgr().GetBagItem(pRecv->n64IM);
	if (!P_VALID(pIM) || !P_VALID(pIM->pProtoType) || 
		(pIM->pProtoType->eSpecFunc != EISF_EquipGrowTransIM && pIM->pProtoType->eSpecFunc != EISF_EquipGrowTransPIM))
	{
		dwErrorCode = E_EquipGrowTrans_IMInvalid;
		goto end;
	}

	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pOldEquip->equipSpec.dwGrowTypeID);
	if (!P_VALID(pGrowProto))
	{
		dwErrorCode = E_EquipGrowTrans_Error;
		goto end;
	}

	//���ǲ���
	if (IUTIL->RandomInRange(1, 100) <= 50)
	{
		if(pOldEquip->equipSpec.byGrowLevel > 10)
			pOldEquip->equipSpec.byGrowLevel = (10>pIM->pProtoType->nSpecFuncVal1) ? 10 - pIM->pProtoType->nSpecFuncVal1:0;
		else
			pOldEquip->equipSpec.byGrowLevel = (pOldEquip->equipSpec.byGrowLevel > pIM->pProtoType->nSpecFuncVal1) ? 
																			pOldEquip->equipSpec.byGrowLevel - pIM->pProtoType->nSpecFuncVal1 : 0;
	}
	else
	{	
		if(pOldEquip->equipSpec.byGrowLevel > 10)
			pOldEquip->equipSpec.byGrowLevel = (10>pIM->pProtoType->nSpecFuncVal2) ? 10 - pIM->pProtoType->nSpecFuncVal2:0;
		else
			pOldEquip->equipSpec.byGrowLevel = (pOldEquip->equipSpec.byGrowLevel > pIM->pProtoType->nSpecFuncVal2) ? 
			pOldEquip->equipSpec.byGrowLevel - pIM->pProtoType->nSpecFuncVal2 : 0;
	}
	
	pOldEquip->equipSpec.byGrowLevel > 0 ? pOldEquip->equipSpec.byGrowLevel : 0;

	if (pOldEquip->equipSpec.byGrowLevel > 10)
	{
		dwErrorCode = E_EquipGrowTrans_Error;
		goto end;
	}

	// �̳в���
	pNewEquip->equipSpec.dwGrowTypeID = pOldEquip->equipSpec.dwGrowTypeID;
	pNewEquip->equipSpec.byGrowLevel = pOldEquip->equipSpec.byGrowLevel;

	IFASTCODE->MemCpy(pNewEquip->equipSpec.eGrowStar, pOldEquip->equipSpec.eGrowStar, sizeof(EEquipGrowStar)*(EQUIP_GROW_MAX_LEVEL-1));
	for (int i=0; i<EQUIP_GROW_MAX_LEVEL-1; ++i)
	{
		if(i>=pNewEquip->equipSpec.byGrowLevel)
			pNewEquip->equipSpec.eGrowStar[i] = EEGS_Null;
	}

	DWORD dwGrowValue = 0;
	float fpct = 1.0f;
	if (pNewEquip->equipSpec.byGrowLevel <= 0)
	{
		pNewEquip->equipSpec.dwGrowValue = 0;
		ZeroMemory(pNewEquip->equipSpec.nGrowAttValue, EQUIP_GROW_ADD_ATT_MAX_NUM*sizeof(INT));
	}
	else
	{
		for (int i=0; i<pNewEquip->equipSpec.byGrowLevel; ++i)
		{
// 			if(i>0)
// 			{
// 				if (!IsGrowStarValid(pNewEquip->equipSpec.eGrowStar[i-1]))
// 					break;
// 				MGetEquipGrowRatio(pNewEquip->equipSpec.eGrowStar[i-1], fpct);
// 			}

			dwGrowValue += pGrowProto->dwExp[i]*fpct;
		}

		pNewEquip->equipSpec.dwGrowValue = dwGrowValue;

		// �ɳ�����
		for (int j=0; j<EQUIP_GROW_ADD_ATT_MAX_NUM; ++j)
		{
			if (pGrowProto->eAtt[j] != ERA_Null && pNewEquip->equipSpec.byGrowLevel >= pGrowProto->byAttOpenLevel[j])
			{
				DWORD dwGrowAtt = 0;
				float fpct = 1.0f;
				for (int i=0; i<pNewEquip->equipSpec.byGrowLevel; ++i)
				{
					if(i>0)
					{
						if (!IsGrowStarValid(pNewEquip->equipSpec.eGrowStar[i-1]))
							break;
						MGetEquipGrowRatio(pNewEquip->equipSpec.eGrowStar[i-1], fpct);
					}
					fpct = (fpct == 0.0) ? 1.0f : fpct;
					dwGrowAtt += pGrowProto->dwAttValue[j][i]*fpct;
				}

				pNewEquip->equipSpec.nGrowAttValue[j] = dwGrowAtt;
			}
		}
	}
	
	// ������������
	pNewEquip->equipSpec.nTotalGrowTimes = 0;
	pNewEquip->equipSpec.nTotalIMUseTimes = 0;

	//ɾ��ԭװ���͵���
	if(pIM->pProtoType->eSpecFunc == EISF_EquipGrowTransIM)
	{
		pRole->GetItemMgr().ItemUsedFromBag(pRecv->n64OldEquip, 1, ELCID_EquipGrowTrans);
	}
	else
	{
		if(P_VALID(pOldEquip))
		{
			pOldEquip->equipSpec.dwGrowTypeID = 0;
			pOldEquip->equipSpec.dwGrowValue = 0;
			pOldEquip->equipSpec.byGrowLevel = 0;
			pOldEquip->equipSpec.nTotalGrowTimes = 0;
			pOldEquip->equipSpec.nTotalIMUseTimes = 0;
			//ZeroMemory(pOldEquip->equipSpec.eGrowStar, sizeof(pOldEquip->equipSpec.eGrowStar));
			//ZeroMemory(pOldEquip->equipSpec.nGrowAttValue, sizeof(pOldEquip->equipSpec.nGrowAttValue));
			memset( pOldEquip->equipSpec.eGrowStar, 0, sizeof(pOldEquip->equipSpec.eGrowStar));
			memset( pOldEquip->equipSpec.nGrowAttValue, 0, sizeof(pOldEquip->equipSpec.nGrowAttValue));
		
			// ͬ����װ������
			pRole->GetItemMgr().UpdateEquipSpec(*pOldEquip);
		}
	}
	
	pRole->GetItemMgr().ItemUsedFromBag(pRecv->n64IM, 1, ELCID_EquipGrowTrans);

	// ͬ��װ������
	pRole->GetItemMgr().UpdateEquipSpec(*pNewEquip);
end:
	tagNS_EquipGrowTrans send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD	PlayerSession::HandleEquipGrowDec(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_EquipGrowDec)

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	tagItem* pItem = NULL;
	tagItem* pIM = NULL;

	pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Equip);
	if (!P_VALID(pItem) || !MIsEquipment(pItem->dwTypeID))
	{
		dwErrorCode = E_EquipGrowDec_EquipInvalid;
		goto end;
	}
	
	tagEquip* pEquip = (tagEquip*)pItem;
// 	if (!pEquip->pEquipProto->bCanGrow)
// 	{
// 		dwErrorCode = E_EquipGrowDec_EquipInvalid;
// 		goto end;
// 	}

// 	if(P_VALID(pEquip) && MIsEquipment(pEquip->dwTypeID) && pEquip->pEquipProto->bOnlyScriptStrengthen)
// 	{
// 		dwErrorCode = E_OnlyCanStrengthenByScript;
// 		goto end;
// 	}

	if(pEquip->equipSpec.byGrowLevel <= 0) 
	{
		dwErrorCode = E_EquipGrowDec_NoStar;
		goto end;
	}

	pIM = pRole->GetItemMgr().GetBagItem(pRecv->n64IM);
	if (!P_VALID(pIM) || pIM->pProtoType->eSpecFunc != EISF_EquipReduceIM)
	{
		dwErrorCode = E_EquipGrowDec_IMInvalid;
		goto end;
	}

	// ����
	pEquip->equipSpec.byGrowLevel -= 1;
	if (pEquip->equipSpec.byGrowLevel == EQUIP_GROW_MAX_LEVEL-1)
	{
		--(pEquip->equipSpec.byGrowLevel);
	}
	pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel] = EEGS_Null;

	// �ɳ�������
	DWORD dwGrowValue = 0;
	float fpct = 1.0f;
	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pEquip->equipSpec.dwGrowTypeID);
	if (!P_VALID(pGrowProto))
	{
		return GT_INVALID;
	}

	if (pEquip->equipSpec.byGrowLevel <= 0)
	{
		pEquip->equipSpec.dwGrowValue = 0;
		ZeroMemory(pEquip->equipSpec.nGrowAttValue, EQUIP_GROW_ADD_ATT_MAX_NUM*sizeof(INT));
	}
	else
	{
		for (int i=0; i<pEquip->equipSpec.byGrowLevel; ++i)
		{
// 			if(i>0)
// 			{
// 				if (!IsGrowStarValid(pEquip->equipSpec.eGrowStar[i-1]))
// 					break;
// 				MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[i-1], fpct);
// 			}

			dwGrowValue += pGrowProto->dwExp[i]*fpct;
		}

		// ���óɳ���
		pEquip->equipSpec.dwGrowValue = dwGrowValue;

		// ���óɳ�����
		for (int j=0; j<EQUIP_GROW_ADD_ATT_MAX_NUM; ++j)
		{
			if (pGrowProto->eAtt[j] != ERA_Null && pEquip->equipSpec.byGrowLevel + 1 >= pGrowProto->byAttOpenLevel[j])
			{
				DWORD dwGrowAtt = 0;
				float fpct = 1.0f;
				for (int i=0; i<pEquip->equipSpec.byGrowLevel; ++i)
				{
					if(i>0)
					{
						if (!IsGrowStarValid(pEquip->equipSpec.eGrowStar[i-1]))
							break;
						MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[i-1], fpct);
					}
					fpct = (fpct == 0.0) ? 1.0f : fpct;
					dwGrowAtt += pGrowProto->dwAttValue[j][i]*fpct;
				}

				pEquip->equipSpec.nGrowAttValue[j] = dwGrowAtt;
			}
		}
	}

	// �۳�����
	pRole->GetItemMgr().ItemUsedFromBag(pIM->GetKey(), 1, ELCID_EquipGrow);
		
	// ͬ��װ������
	pRole->GetItemMgr().UpdateEquipSpec(*pEquip);

end:
	tagNS_EquipGrowDec send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD PlayerSession::HandleSoulCrystalGrow(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SoulCrystalGrow)

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	
	tagItem* pItem = NULL;
	tagItem* pIM = NULL;

	pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Serial);
	if (!P_VALID(pItem) || !MIsEquipment(pItem->dwTypeID) || !MIsSoulCrystal(pItem))
	{
		// �޻꾧
		dwErrorCode = E_SoulCrystal_Grow_ItemInvalid;
		goto end;
	}

	tagSoulCrystal*  pSoulCrystal = (tagSoulCrystal*)pItem;
	
	if(P_VALID(pRecv->n64IMSerial))
	{
		pIM = pRole->GetItemMgr().GetBagItem(pRecv->n64IMSerial);
		if (!P_VALID(pIM) || pIM->pProtoType->eSpecFunc != EISF_HunShiJingHua)
		{
			// ��ʯ��������
			dwErrorCode = E_SoulCrystal_Grow_IMError;
			goto end;
		}
	}
	
	if(pSoulCrystal->SoulSpec.bySoulCrystalLvl >= SOUL_CRYSTAL_MAX_LVL)
	{
		// �꾧�ȼ��Ѵ����
		dwErrorCode = E_SoulCrystal_Grow_MaxLvl;
		goto end;
	}

	DWORD dwRequireSoul = g_attRes.GetSoulCrystalLvlLine(pSoulCrystal->SoulSpec.bySoulCrystalLvl+1)->dwAttSoul;
	INT64 n64RequireMoney = g_attRes.GetSoulCrystalLvlLine(pSoulCrystal->SoulSpec.bySoulCrystalLvl+1)->n64Money;
	if (pSoulCrystal->pProtoType->eTypeEx == EITE_SoulCrystalGod && pRole->GetGod() < dwRequireSoul)
	{
		// ��겻��
		dwErrorCode = E_SoulCrystal_Grow_SoulLimit;
		goto end;
	}

	if (pSoulCrystal->pProtoType->eTypeEx == EITE_SoulCrystalEvil && pRole->GetMonster() < dwRequireSoul)
	{
		// ħ�겻��
		dwErrorCode = E_SoulCrystal_Grow_SoulLimit;
		goto end;
	}

	if (pRole->GetCurMgr().GetBagSilver() < n64RequireMoney)
	{
		// ��Ǯ����
		dwErrorCode = E_SoulCrystal_Grow_MoneyLimit;
		goto end;
	}

	INT nRetAtt[SOUL_CRYSTAL_ATT_MAX_NUM] = {0};
	INT nRateAdd1 = 0, nRateAdd2 = 0;
	if (P_VALID(pIM))
	{
		nRateAdd1 = pIM->pProtoType->nSpecFuncVal1;
		nRateAdd2 = pIM->pProtoType->nSpecFuncVal2;
	}
	const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
	if(!P_VALID(pScript))
	{
		return GT_INVALID;
	}

	pScript->GetSoulCrystalAtt(pRole->GetID(), pSoulCrystal->dwTypeID, pSoulCrystal->SoulSpec.bySoulCrystalQlty, nRetAtt, false,
							nRateAdd1, nRateAdd2, pSoulCrystal->SoulSpec.nSoulAttID);

	++(pSoulCrystal->SoulSpec.bySoulCrystalLvl);
	IFASTCODE->MemCpy(pSoulCrystal->SoulSpec.nSoulAttID, nRetAtt, sizeof(nRetAtt));

	// �۳���Ʒ��������Ժͽ�Ǯ
	if(P_VALID(pIM))
		pRole->GetItemMgr().DelFromBag(pIM->n64Serial, ELCID_SoulCrystal_Grow, 1);
	
	if(pSoulCrystal->pProtoType->eTypeEx == EITE_SoulCrystalGod)
		pRole->ChangeSoul(-dwRequireSoul, 0);
	else if(pSoulCrystal->pProtoType->eTypeEx == EITE_SoulCrystalEvil)
		pRole->ChangeSoul(0, -dwRequireSoul);

	pRole->GetCurMgr().DecBagSilver(n64RequireMoney, ELCID_SoulCrystal_Grow);

	pRole->GetItemMgr().UpdateSoulCrystalSpec(*pSoulCrystal);
end:
	tagNS_SoulCrystalGrow send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);
	return 0;
}

DWORD PlayerSession::HandleSoulCrystalRenew(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SoulCrystalRenew)

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	TList<tagItem*> listIM;
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Serial);
	if (!P_VALID(pItem) || !MIsEquipment(pItem->dwTypeID) || !MIsSoulCrystal(pItem))
	{
		// �޻꾧
		dwErrorCode = E_SoulCrystal_Renew_ItemInvalid;
		goto end;
	}

	if(pRole->GetItemMgr().GetBagSameItemList(listIM, SoulCrystal_Renew_Item_1) <= 0)
	{
		if(pRole->GetItemMgr().GetBagSameItemList(listIM, SoulCrystal_Renew_Item_2) <= 0)
		{
			dwErrorCode = E_SoulCrystal_Renew_IMLimit;
			goto end;
		}
	}

	tagSoulCrystal*  pSoulCrystal = (tagSoulCrystal*)pItem;

	INT nRetAtt[SOUL_CRYSTAL_ATT_MAX_NUM] = {0};
	pRole->RenewSoulCrystalAtt(pSoulCrystal->dwTypeID, pSoulCrystal->SoulSpec.bySoulCrystalQlty, 
		pSoulCrystal->SoulSpec.nSoulAttID, dwErrorCode, nRetAtt);

	if (dwErrorCode == E_Success)
	{
		IFASTCODE->MemCpy(pSoulCrystal->SoulSpec.nSoulAttID, nRetAtt, sizeof(nRetAtt));

		pRole->GetItemMgr().DelBagSameItem(listIM, 1, ELCID_SoulCrystal_Renew);

		pRole->GetItemMgr().UpdateSoulCrystalSpec(*pSoulCrystal);
	}
	else
	{
		return GT_INVALID;
	}

end:
	tagNS_SoulCrystalRenew send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);
	return 0;
}

//--------------------------------------------------------------------------------
// ʹ��ˢ�ɳ������Լ���ʯ�����ĵ���
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleUseDiamondEquipGrowIM(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DiamondEquipGrowIM);

	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return 0;
	}
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if(!P_VALID(pItem))
	{
		return 0;
	}
	DWORD dwErrorCode = pRole->OnRoleUseDiamondEquipGrowIM(pItem->dwTypeID, pRecv->n64EquipID);

	tagNS_DiamondEquipGrowIM send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

#ifdef ON_GAMEGUARD
// (����)�ӿͻ��˽���CS��֤��
DWORD   PlayerSession::HandleAnswerKoreaCSApprove(tagNetCmd* pCmd)
{
	// ��Log�����CS��֤��ʧ��
	//ILOG->Write(_T("�ͻ��˷���CS��֤�� Player: %u\r\n"), GetSessionID());

	MGET_MSG(pRecv, pCmd, NC_AnswerKoreaCSApprove);

	if ( !P_VALID(pRecv) )
	{		
		return GT_INVALID;
	}

	UINT32 uPackSize = pRecv->dwSize - sizeof(tagNC_AnswerKoreaCSApprove) + 1;
	memcpy(m_CS.packet, pRecv->byData, uPackSize);

	UINT32 uRet = m_CS.Check(uPackSize);

	if ( uRet >= 3000 )
	{
		// �Ͽ��������
		m_CS.Close();
		ILOG->Write(_T("�ͻ��˷��ص�CS��֤����֤ʧ��, �����룺%u, Kick Player: %u\r\n"), uRet, GetSessionID());
		g_worldSession.Kick(GetInternalIndex());
		SetKicked();
	}

	return uRet;

}

// (����)�ӿͻ��˽���GameGuard����
DWORD   PlayerSession::HandleGameGuardReport(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GameGuardReport);

	if ( !P_VALID(pRecv) )
	{		
		return GT_INVALID;
	}

	UINT32 uPackSize = pRecv->dwSize - sizeof(tagNC_GameGuardReport);

	UINT32 uRet = DecryptHackData("VanOnlineKR", pRecv->byData, uPackSize);
	if (uRet == -1)
	{
		ILOG->Write(_T("����Gameguard����ʧ��"));
		return uRet;
	}

	return uRet;
}
#endif

//------------------------------------------------------------------------
//Ԫ��װ������
//------------------------------------------------------------------------
// DWORD	PlayerSession::HandleStarEnhance(tagNetCmd* pCmd)
//{
// 	MGET_MSG(pRecv, pCmd, NC_StarEnhance);
// 	if ( !P_VALID(pRecv) )
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	//��ͼ�������жϴ���
// 	Role* pRole = GetRole();
// 	if ( !P_VALID(pRole) )
// 	{
// 		return GT_INVALID;
// 	}
// 	tagNS_StarEnhance msg;
// 	msg.dwErrCode = E_Success;
// 
// 	if ( !g_world.IsStarEnhanceOpent() )
// 	{
// 		msg.dwErrCode = E_StarEnhanceCondensate_NotOpen;
// 		pRole->SendMessage(&msg,msg.dwSize);
// 		return E_StarEnhanceCondensate_NotOpen;
// 	}
// 
// 	ItemMgr& itemMgr	=  pRole->GetItemMgr();
// 	tagItem* pItem		= itemMgr.GetBagItem(pRecv->n64EquipId);
// 	if ( !P_VALID(pItem) )
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	if ( !MIsEquipment(pItem->dwTypeID))
// 	{
// 		return GT_INVALID;
// 	}
// 	tagEquip* pEquip = (tagEquip*)pItem;
// 
// 	//�Ƿ�Ԫ��װ��
// 	if ( !MIsHolySoulEquipType(pEquip) )
// 	{
// 		msg.dwErrCode = E_StarEnhanceCondensate_EquipCanNotOpt;
// 		pRole->SendMessage(&msg,msg.dwSize);
// 		return E_StarEnhanceCondensate_EquipCanNotOpt;
// 	}
// 	BYTE byCurrLvl = pEquip->equipSpec.byStarEnhanceLv;
// 	BYTE byNextLvl = pEquip->equipSpec.byStarEnhanceLv + 1;
// 	//�Ƿ񳬹������ǵȼ�
// 	if ( byNextLvl > MAX_STARENHANCE_LEVEL)
// 	{
// 		msg.dwErrCode = E_StarEnhanceCondensate_MaxLevel;
// 		pRole->SendMessage(&msg,msg.dwSize);
// 		return E_StarEnhanceCondensate_MaxLevel;
// 	}
// 
// 	const tagStarEnhanceProto* pStarEnhanceProto = g_attRes.GetStarEnhanceProto(byNextLvl);
// 	if ( !P_VALID(pStarEnhanceProto) )
// 	{
// 		msg.dwErrCode = E_StarEnhanceCondensate_NoExist;
// 		pRole->SendMessage(&msg,msg.dwSize);
// 		return E_StarEnhanceCondensate_NoExist;
// 	}
// 
// 	//���ǵ����Ƿ���
// 	TList<tagItem*> list;
// 	INT dwItemNum	= itemMgr.GetBagSameItemList(list,pStarEnhanceProto->dwCostItemId);
// 	if ( (dwItemNum < pStarEnhanceProto->nCostItemNum) || list.Empty())
// 	{
// 		msg.dwErrCode = E_StarEnhanceCondensate_NoEnoughItem;
// 		pRole->SendMessage(&msg,msg.dwSize);
// 		return E_StarEnhanceCondensate_NoEnoughItem;
// 	}
// 
// 	//�Ƿ��㹻��Ǯ
// 	INT64 n64Cost = pRole->GetCurMgr().GetBagSilver();
// 	if ( n64Cost < pStarEnhanceProto->dwCost )
// 	{
// 		msg.dwErrCode = E_StarEnhanceCondensate_NoEnoughMoney;
// 		pRole->SendMessage(&msg,msg.dwSize);
// 		return E_StarEnhanceCondensate_NoEnoughMoney;
// 	}
// 
// 	//�жϳ����Ƿ�ɹ�
// 	bool bSuc		= false;
// 	bool bChange	= false;
// 	INT nHitNum		= IUTIL->RandomInRange(0,10000);
// 	if ( pStarEnhanceProto->nRate >= nHitNum )
// 	{
// 		bSuc		= true;
// 	}
// 
// 	//�۵���ҫʯ
// 	DWORD dwCostItemErrorcode = itemMgr.DelBagSameItem(list,pStarEnhanceProto->nCostItemNum,ELCLD_StarEnhanceConsolidate);
// 
// 	//�۵���Ǯ
// 	BOOL bCostMoney = pRole->GetCurMgr().DecBagSilver(pStarEnhanceProto->dwCost,ELCLD_StarEnhanceConsolidate);
// 
// 	//����ʧ��
// 	if ( !bSuc )
// 	{
// 		bChange	= pEquip->ChangeStarEnhanceLvl(pStarEnhanceProto->byFailLevel);
// 		msg.dwErrCode = E_StarEnhanceCondensate_Failed;
// 	}
// 	else
// 	{
// 		bChange = pEquip->ChangeStarEnhanceLvl(byNextLvl);
// 		msg.dwErrCode = E_Success;
// 	}
// 	//����Ԫ��װ������
// 
// 	if ( bChange )
// 	{
// 		itemMgr.UpdateEquipSpec(*pEquip,true,EESSType_StarEnhance);
// 	}
// 	pRole->SendMessage(&msg,msg.dwSize);
// 
// 	//��Ӧlog��¼
// 	tagNDBC_LogHolyEquipCon logHolyEquip;
// 	logHolyEquip.sLogHolyEquip.eType = EHECT_StarEnhance;
// 	logHolyEquip.sLogHolyEquip.n64HolyEquipID = pRecv->n64EquipId;
// 	logHolyEquip.sLogHolyEquip.bCostMoney = (bool)bCostMoney;
// 	logHolyEquip.sLogHolyEquip.nCostMoney = (INT)pStarEnhanceProto->dwCost;
// 	logHolyEquip.sLogHolyEquip.bCostItem = dwCostItemErrorcode == E_Success ? true : false ;
// 	logHolyEquip.sLogHolyEquip.nCostItem = (INT)(pStarEnhanceProto->nCostItemNum);
// 	logHolyEquip.sLogHolyEquip.bCostItemOther = false ;
// 	logHolyEquip.sLogHolyEquip.nCostItemOther = 0;
// 	logHolyEquip.sLogHolyEquip.bConsolidateRes = bSuc;
// 	logHolyEquip.sLogHolyEquip.byCurrentLevel = pEquip->equipSpec.byStarEnhanceLv;
// 	logHolyEquip.sLogHolyEquip.dwRoleID = pRole->GetID();
// 
// 	g_dbSession.Send(&logHolyEquip,logHolyEquip.dwSize);
//	return E_Success;
//}

//------------------------------------------------------------------------
//Ԫ��װ�����
//------------------------------------------------------------------------
// DWORD PlayerSession::HandleHolyEquipHole(tagNetCmd* pCmd)
// {
// 	MGET_MSG(pRecv, pCmd, NC_HolyEquipHole);
// 	if (!P_VALID(pRecv))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	Role* pRole = GetRole();
// 	if (!P_VALID(pRole))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	tagNS_HolyEquipHole msg;
// 	msg.dwErrorCode = GT_INVALID;
// 
// 	//�ж���Ʒ����Ч��
// 	ItemMgr& itemMgr =  pRole->GetItemMgr();
// 	tagItem* pEquipItem = itemMgr.GetBagItem(pRecv->n64SrcEquipID);
// 	tagItem* pItem = itemMgr.GetBagItem(pRecv->n64ItemID);
// 	if (!P_VALID(pEquipItem))
// 	{
// 		msg.dwErrorCode = E_HolyEquipHole_EquipNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipHole_EquipNotExist;
// 	}
// 
// 	if (!P_VALID(pItem))
// 	{
// 		msg.dwErrorCode = E_HolyEquipHole_ItemNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipHole_ItemNotExist;
// 	}
// 
// 	tagEquip* pEquip = (tagEquip*)pEquipItem;
// 	if (!P_VALID(pEquip))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	//�ж��Ƿ�Ԫ��װ��
// 	if (!MIsHolySoulEquipType(pEquip))
// 	{
// 		msg.dwErrorCode = E_HolyEquipHole_NotHolyEquip;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipHole_NotHolyEquip;
// 	}
// 
// 	//�ж��Ƿ��Ƽ�����
// // 	if (!P_VALID(pItem->pProtoType) || EISF_SoulChisel !=pItem->pProtoType->eSpecFunc)
// // 	{
// // 		msg.dwErrorCode = E_HolyEquipHole_NotHoleItem;
// // 		SendMessage(&msg,msg.dwSize);
// // 		return E_HolyEquipHole_NotHoleItem;
// // 	}
// 
// 	if (MAX_EQUIPHOLE_NUM <= pEquip->equipSpec.byHolyHoleNum)
// 	{
// 		msg.dwErrorCode = E_HolyEquipHole_HoleNumMax;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipHole_HoleNumMax;
// 	}
// 
// 	const tagHolySoulEquipHole* pSoulEquipHoleProto = g_attRes.GetSoulEquipHoleProto(pEquip->equipSpec.byHolyHoleNum + 1);
// 	if (!P_VALID(pSoulEquipHoleProto))
// 	{
// 		msg.dwErrorCode = E_HolyEquipHole_ProtoNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipHole_ProtoNotExist;
// 	}
// 
// 	//�жϱ����н�Ǯ�͵����Ƿ���
// 	INT64 n64Money = pRole->GetCurMgr().GetBagSilver();
// 	if (n64Money < pSoulEquipHoleProto->nCostMoney)
// 	{
// 		msg.dwErrorCode = E_HolyEquipHole_MoneyNotEnough;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipHole_MoneyNotEnough;
// 	}
// 
// 	//������������ɹ���ɾ�����ߺͽ�Ǯ
// 	// 	if (!P_VALID(pItem->pProtoType))
// 	// 	{
// 	// 		return GT_INVALID;
// 	// 	}
// 	//���ճɹ��� = <rate>* specfuncval1
// 	INT nFinalRate = (INT)(((FLOAT)pSoulEquipHoleProto->nRate / 10000) * pItem->pProtoType->nSpecFuncVal1);
// 	if (nFinalRate > 10000)
// 	{
// 		nFinalRate = 10000;
// 	}
// 	BOOL bRet = (IUTIL->RandomInRange(0,10000) < nFinalRate);
// 	DWORD dwErrCode = GT_INVALID;
// 	BOOL bCostMoney = FALSE;
// 	DWORD dwReturn = GT_INVALID;
// 	if (bRet)
// 	{
// 		//�۳���Ǯ�͵���
// 		dwErrCode = itemMgr.DelFromBag(pRecv->n64ItemID,ELCLD_HolySoulEquipHole,1,TRUE);
// 		//if (E_Success != dwErrCode)
// 		//{
// 		//	return GT_INVALID;
// 		//}
// 
// 		bCostMoney = pRole->GetCurMgr().DecBagSilver(pSoulEquipHoleProto->nCostMoney,ELCLD_HolySoulEquipHole);
// 
// 		//װ������+1
// 		(pEquip->equipSpec.byHolyHoleNum)++;
// 		//Ӧ�ò��ᷢ��
// 		if (pEquip->equipSpec.byHolyHoleNum > MAX_EQUIPHOLE_NUM)
// 		{
// 			return GT_INVALID;
// 		}
// 		itemMgr.UpdateEquipSpec(*pEquip,true,EESSType_HolyEquipHole);
// 
// 		msg.dwErrorCode = E_Success;
// 		SendMessage(&msg,msg.dwSize);
// 		dwReturn = E_Success;
// 
// 	}
// 	else
// 	{
// 		dwErrCode = itemMgr.DelFromBag(pRecv->n64ItemID,ELCLD_HolySoulEquipHole,1,TRUE);
// 		//if (E_Success != dwErrCode)
// 		//{
// 		//	return GT_INVALID;
// 		//}
// 
// 		bCostMoney = pRole->GetCurMgr().DecBagSilver(pSoulEquipHoleProto->nCostMoney,ELCLD_HolySoulEquipHole);
// 
// 		msg.dwErrorCode = E_HolyEquipHole_Fail;
// 		SendMessage(&msg,msg.dwSize);
// 		dwReturn = E_HolyEquipHole_Fail;
// 	}
// 
// 	//��Ӧlog��¼
// 	tagNDBC_LogHolyEquipCon logHolyEquip;
// 	logHolyEquip.sLogHolyEquip.eType = EHECT_EquipHole;
// 	logHolyEquip.sLogHolyEquip.n64HolyEquipID = pRecv->n64SrcEquipID;
// 	logHolyEquip.sLogHolyEquip.bCostMoney = (bool)bCostMoney;
// 	logHolyEquip.sLogHolyEquip.nCostMoney = pSoulEquipHoleProto->nCostMoney;
// 	logHolyEquip.sLogHolyEquip.bCostItem = (dwErrCode == E_Success ? true : false) ;
// 	logHolyEquip.sLogHolyEquip.nCostItem = 1;
// 	logHolyEquip.sLogHolyEquip.bCostItemOther = false ;
// 	logHolyEquip.sLogHolyEquip.nCostItemOther = 0;
// 	logHolyEquip.sLogHolyEquip.bConsolidateRes = bRet;
// 	logHolyEquip.sLogHolyEquip.byCurrentLevel = pEquip->equipSpec.byHolyHoleNum;//���ʱ�����ֶδ��װ����ǰ����
// 	logHolyEquip.sLogHolyEquip.dwRoleID = pRole->GetID();
// 
// 	g_dbSession.Send(&logHolyEquip,logHolyEquip.dwSize);

//	return 0;//dwReturn;
//}

//------------------------------------------------------------------------
//Ԫ��װ����Ƕ
//------------------------------------------------------------------------
//DWORD PlayerSession::HandleHolyEquipInlay(tagNetCmd* pCmd)
//{
// 	MGET_MSG(pRecv, pCmd,NC_HolyEquipInlay );
// 	if (!P_VALID(pRecv))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	Role* pRole = GetRole();
// 	if (!P_VALID(pRole))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	tagNS_HolyEquipInlay msg;
// 	msg.dwErrorCode = GT_INVALID;
// 
// 	//�ж���Ʒ����Ч��
// 	ItemMgr& itemMgr =  pRole->GetItemMgr();
// 	tagItem* pEquipItem = itemMgr.GetBagItem(pRecv->n64SrcEquipID);
// 	tagItem* pStoneItem = itemMgr.GetBagItem(pRecv->n64StoneID);
// 	tagItem* pItem = itemMgr.GetBagItem(pRecv->n64ItemID);
// 	tagEquip* pEquip = (tagEquip*)pEquipItem;
// 
// 	if(!P_VALID(pEquip) || !MIsHolySoulEquipType(pEquip))
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_EquipNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_EquipNotExist;
// 	}
// 
// 	//�ж��Ƿ���Ӱ��ʯ
// // 	if (!P_VALID(pStoneItem) || !P_VALID(pStoneItem->pEquipProto) || EISF_SoulInlay != pStoneItem->pProtoType->eSpecFunc)
// // 	{
// // 		msg.dwErrorCode = E_HolyEquipInlay_NotInlayItem;
// // 		SendMessage(&msg,msg.dwSize);
// // 		return E_HolyEquipInlay_NotInlayItem;
// // 	}
// 
// 	//�жϿ�λ
// 	if (0 >= pRecv->byHoleID  || MAX_EQUIPHOLE_NUM < pRecv->byHoleID)
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_HoleIDUnusual;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_HoleIDUnusual;
// 	}
// 	if (pEquip->equipSpec.byHolyHoleNum < pRecv->byHoleID)
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_HoleNotChisel;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_HoleNotChisel;
// 	}
// 	if(P_VALID(pEquip->equipSpec.dwHoleGemID[pRecv->byHoleID - 1]))
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_HoleAlreadyInlay;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_HoleAlreadyInlay;
// 	}
// 
// 	const tagHolySoulEquipInlay* pSoulEquipInlayProto = g_attRes.GetSoulEquipInlayProto(pRecv->byHoleID);
// 	if (!P_VALID(pSoulEquipInlayProto))
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_ProtoNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_ProtoNotExist;
// 	}
// 
// 	// �ҵ�ǿ������
// 	DWORD dwStoneTypeID = pStoneItem->dwTypeID;
// 	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(dwStoneTypeID);
// 	if(!P_VALID(pConsolidateProto))
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_FormulaNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_FormulaNotExist;
// 	}
// 
// 	//�жϽ�Ǯ�Ƿ���
// 	INT64 n64Money = pRole->GetCurMgr().GetBagSilver();
// 	if (n64Money < pSoulEquipInlayProto->nCostMoney)
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_MoneyNotEnough;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_MoneyNotEnough;
// 	}
// 
// 
// 	//�ж��ܷ���Ƕ,ֻ�ж�Ԫ��װ��λ��
// 	BOOL bConsolidate = FALSE;
// 	for (INT i = MAX_CONSOLIDATE_POS_QUANTITY; i < MAX_CONSOLIDATE_POS_QUANTITY + MAX_CONSOLIDATE_POS_HOLYEQUIP; i++)
// 	{
// 		if(pEquip->pEquipProto->eTypeEx != (EItemTypeEx)pConsolidateProto->ConsolidatePos[i].byConsolidate)
// 			continue;
// 		else
// 		{
// 			bConsolidate = TRUE;
// 			break;
// 		}
// 	}
// 	if (!bConsolidate)
// 	{
// 		msg.dwErrorCode = E_HolyEquipInlay_EquipCannotInlay;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipInlay_EquipCannotInlay;
// 	}
// 
// 	BOOL bHasLjxl = FALSE;//�Ƿ�������Ͼ���¶
// 	INT nFinalRate = pSoulEquipInlayProto->nRate;
// 	INT nBagItemNum = 0;
// 	//��������Ͼ���¶����ɹ���Ϊ���Ͼ���¶����*10%+[soul_equip_inlay]��<rate>
// 	if (P_VALID(pItem))
// 	{
// 		bHasLjxl = TRUE;
// 		//�����жϱ������Ͼ���¶�����Ƿ���
// 		nBagItemNum = itemMgr.GetBagSameItemCount(pItem->dwTypeID);
// 		if(pItem->dwTypeID == REBORN_STUFF_BINDING)
// 		{
// 			nBagItemNum += itemMgr.GetBagSameItemCount(REBORN_STUFF_UNBINDING); 
// 		}
// 		else if(pItem->dwTypeID == REBORN_STUFF_UNBINDING)
// 		{
// 			nBagItemNum += itemMgr.GetBagSameItemCount(REBORN_STUFF_BINDING); 
// 		}
// 		if(nBagItemNum < pRecv->nItemNum)
// 		{
// 			msg.dwErrorCode = E_HolyEquipInlay_ItemNotEnough;
// 			SendMessage(&msg,msg.dwSize);
// 			return E_HolyEquipInlay_ItemNotEnough;
// 		}
// 
// 		nFinalRate += (INT)(pRecv->nItemNum * (FLOAT)IMPROVE_SOULINLAY_RATE / 100 * 10000);
// 		if (nFinalRate > 10000)
// 		{
// 			nFinalRate = 10000;
// 		}		
// 	}
// 
// 	INT nItemCostNum = pRecv->nItemNum;
// 	//�жϰ󶨺ͷǰ󶨵��Ͼ���¶����
// 	TList<tagItem*> itemListBind;  //���İ��Ͼ���¶list
// 	TList<tagItem*> itemListUnBind;//���ķǰ��Ͼ���¶list
// 	itemListBind.Clear();
// 	itemListUnBind.Clear();
// 	INT nItemBindNum = 0;
// 	INT nItemUnBindNum = 0;
// 	if (P_VALID(pItem))
// 	{
// 		pRole->CalculateItemCostNum(pItem->dwTypeID,itemListBind,itemListUnBind,nItemCostNum,nItemBindNum,nItemUnBindNum);
// 	}
// 
// 	BOOL bRet = (IUTIL->RandomInRange(0,10000) < nFinalRate);
// 	DWORD dwErrCode = GT_INVALID;
// 	DWORD dwErrCodeOther = GT_INVALID;
// 	DWORD dwAdd2BagErrCode = GT_INVALID;
// 	BOOL bCostMoney = FALSE;
// 	DWORD dwReturn = GT_INVALID;
// 	if (bRet)
// 	{
// 		//ɾ�����ߺͽ�Ǯ
// 		if(bHasLjxl)
// 		{
// 			//dwErrCodeOther = itemMgr.DelFromBag(pRecv->n64ItemID,ELCLD_HolySoulEquipInlay,pRecv->nItemNum,TRUE);
// 			if(nItemBindNum)
// 				dwErrCodeOther = pRole->GetItemMgr().DelBagSameItem(itemListBind,nItemBindNum,ELCLD_HolySoulEquipInlay);
// 			if(nItemUnBindNum)
// 				dwErrCodeOther = pRole->GetItemMgr().DelBagSameItem(itemListUnBind,nItemUnBindNum,ELCLD_HolySoulEquipInlay);
// 		}
// 		// 		if (E_Success != dwErrCode)
// 		// 		{
// 		// 			return GT_INVALID;
// 		// 		}
// 		dwErrCode = itemMgr.DelFromBag(pRecv->n64StoneID,ELCLD_HolySoulEquipInlay, 1 ,TRUE);
// 		// 		if (E_Success != dwErrCode)
// 		// 		{
// 		// 			return GT_INVALID;
// 		// 		}
// 		bCostMoney = pRole->GetCurMgr().DecBagSilver(pSoulEquipInlayProto->nCostMoney,ELCLD_HolySoulEquipInlay);
// 
// 		//Ԫ��װ����Ƕ
// 		pEquip->equipSpec.dwHoleGemID[pRecv->byHoleID - 1] = dwStoneTypeID;		
// 		itemMgr.UpdateEquipSpec(*pEquip);
// 
// 		msg.dwErrorCode = E_Success;
// 		SendMessage(&msg,msg.dwSize);
// 		dwReturn = E_Success;
// 	}
// 	else
// 	{
// 
// 		//�����Ӱ��ʯ����1�����򽵵�1��
// 		if (pStoneItem->pProtoType->byLevel > 1)
// 		{
// 			//�жϱ����ռ��Ƿ���
// 			if(itemMgr.GetBagFreeSize() < 1)
// 			{
// 				msg.dwErrorCode = E_HolyEquipReomve_BagNotEnough;
// 				SendMessage(&msg,msg.dwSize);
// 				return E_HolyEquipReomve_BagNotEnough;
// 			}
// 			//����ɾ��ԭ����
// 			dwErrCode = itemMgr.DelFromBag(pRecv->n64StoneID,ELCLD_HolySoulEquipInlay,1,TRUE);
// 			//if (E_Success != dwErrCode)
// 			//{
// 			//	return GT_INVALID;
// 			//}
// 			//Ȼ�����ɵ�һ���ĵ���
// 			tagItem* pNewStoneItem = ItemCreator::Create(EICM_HolySoulEquipInlay, pRole->GetID(), dwStoneTypeID - 1, 1);
// 			if ( !P_VALID(pNewStoneItem) )
// 			{
// 				IMSG(_T("New Item Create Failed\r\n"));
// 				return GT_INVALID;
// 			}
// 			if (!P_VALID(pNewStoneItem->pProtoType))
// 			{
// 				IMSG(_T("New Item Create Failed\r\n"));
// 				Destroy(pNewStoneItem);
// 				return GT_INVALID;
// 			}
// 			//������ɵĲ�����Ӱ��ʯ
// // 			if ( EISF_SoulInlay != pNewStoneItem->pProtoType->eSpecFunc)
// // 			{
// // 				Destroy(pNewStoneItem);
// // 				return GT_INVALID;
// // 			}
// 			dwAdd2BagErrCode = itemMgr.Add2Bag(pNewStoneItem,ELCLD_HolySoulEquipInlay,TRUE);
// 			if (E_Success != dwAdd2BagErrCode)
// 			{
// 				IMSG(_T("New Item Create Failed! dwAdd2BagErrCode: %u \r\n"),dwAdd2BagErrCode);
// 				Destroy(pNewStoneItem);
// 				return GT_INVALID;
// 			}
// 		}
// 
// 		//ɾ���Ͼ���¶�ͽ�Ǯ
// 		if (bHasLjxl)
// 		{
// 			//dwErrCodeOther = itemMgr.DelFromBag(pRecv->n64ItemID,ELCLD_HolySoulEquipInlay,pRecv->nItemNum,TRUE);
// 			//�۳��Ͼ���¶
// 			if(nItemBindNum)
// 				dwErrCodeOther = pRole->GetItemMgr().DelBagSameItem(itemListBind,nItemBindNum,ELCID_Fabao_RemoveLingzhu);
// 			if(nItemUnBindNum)
// 				dwErrCodeOther = pRole->GetItemMgr().DelBagSameItem(itemListUnBind,nItemUnBindNum,ELCID_Fabao_RemoveLingzhu);
// 		}		
// 		//if (E_Success != dwErrCode)
// 		//{
// 		//	return GT_INVALID;
// 		//}
// 		bCostMoney = pRole->GetCurMgr().DecBagSilver(pSoulEquipInlayProto->nCostMoney,ELCLD_HolySoulEquipInlay);
// 
// 		msg.dwErrorCode = E_HolyEquipInlay_Fail;
// 		SendMessage(&msg,msg.dwSize);
// 		dwReturn = E_HolyEquipInlay_Fail;
// 	}
// 
// 	//��Ӧlog��¼
// 	tagNDBC_LogHolyEquipCon logHolyEquip;
// 	logHolyEquip.sLogHolyEquip.eType = EHECT_EquipInlay;
// 	logHolyEquip.sLogHolyEquip.n64HolyEquipID = pRecv->n64SrcEquipID;
// 	logHolyEquip.sLogHolyEquip.bCostMoney = (bool)bCostMoney;
// 	logHolyEquip.sLogHolyEquip.nCostMoney = pSoulEquipInlayProto->nCostMoney;
// 	logHolyEquip.sLogHolyEquip.bCostItem = (dwErrCode == E_Success ? true : false);
// 	logHolyEquip.sLogHolyEquip.nCostItem = 1;
// 	logHolyEquip.sLogHolyEquip.bCostItemOther = (dwErrCodeOther == E_Success ? true : false) ;
// 	logHolyEquip.sLogHolyEquip.nCostItemOther = pRecv->nItemNum;
// 	logHolyEquip.sLogHolyEquip.bConsolidateRes = bRet;
// 	logHolyEquip.sLogHolyEquip.byCurrentLevel = pEquip->equipSpec.byHolyHoleNum;//��Ƕʱ�����ֶδ��װ����ǰ����
// 	logHolyEquip.sLogHolyEquip.dwRoleID = pRole->GetID();
// 
// 	g_dbSession.Send(&logHolyEquip,logHolyEquip.dwSize);

//	return 0;//dwReturn;
//}

//------------------------------------------------------------------------
//Ԫ��װ��ժ��
//------------------------------------------------------------------------
//DWORD PlayerSession::HandleHolyEquipRemove(tagNetCmd* pCmd)
//{
// 	MGET_MSG(pRecv, pCmd, NC_HolyEquipRemove);
// 	if (!P_VALID(pRecv))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	Role* pRole = GetRole();
// 	if (!P_VALID(pRole))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	tagNS_HolyEquipRemove msg;
// 	msg.dwErrorCode = GT_INVALID;
// 
// 	ItemMgr& itemMgr = pRole->GetItemMgr();
// 	tagItem* pEquipItem = itemMgr.GetBagItem(pRecv->n64SrcEquipID);
// 	tagEquip* pEquip = (tagEquip*)pEquipItem;
// 	if (!P_VALID(pEquip) || !MIsHolySoulEquipType(pEquip))
// 	{
// 		msg.dwErrorCode = E_HolyEquipRemove_EquipNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipRemove_EquipNotExist;
// 	}
// 
// 	//�жϿ�λ
// 	if (0 >= pRecv->byHoleID || MAX_EQUIPHOLE_NUM < pRecv->byHoleID)
// 	{
// 		msg.dwErrorCode = E_HolyEquipReomve_HoleIDUnusual;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipReomve_HoleIDUnusual;
// 	}
// 	if (pEquip->equipSpec.byHolyHoleNum < pRecv->byHoleID)
// 	{
// 		msg.dwErrorCode = E_HolyEquipReomve_HoleNotChisel;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipReomve_HoleNotChisel;
// 	}
// 	//�жϿ�λ���Ƿ���Ƕ�˱�ʯ
// 	if(!P_VALID(pEquip->equipSpec.dwHoleGemID[pRecv->byHoleID - 1]))
// 	{
// 		msg.dwErrorCode = E_HolyEquipReomve_HoleNotInlay;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipReomve_HoleNotInlay;
// 	}
// 
// 	//�жϱ����ռ��Ƿ���
// 	if(itemMgr.GetBagFreeSize() < 1)
// 	{
// 		msg.dwErrorCode = E_HolyEquipReomve_BagNotEnough;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolyEquipReomve_BagNotEnough;
// 	}
// 
// 	//Ԫ��װ��ժ����ʯ
// 	DWORD dwStoneTypeID = pEquip->equipSpec.dwHoleGemID[pRecv->byHoleID - 1];
// 	pEquip->equipSpec.dwHoleGemID[pRecv->byHoleID - 1] = 0;
// 	itemMgr.UpdateEquipSpec(*pEquip);
// 
// 	DWORD dwErrCode;
// 	tagItem* pNewStoneItem = ItemCreator::Create(EICM_HolySoulEquipRemove, pRole->GetID(), dwStoneTypeID, 1);
// 	if ( !P_VALID(pNewStoneItem) )
// 	{
// 		return GT_INVALID;
// 	}
// // 	if (!P_VALID(pNewStoneItem->pEquipProto) || EISF_SoulInlay != pNewStoneItem->pProtoType->eSpecFunc)
// // 	{
// // 		IMSG(_T("New Item Create Failed\r\n"));
// // 		::Destroy(pNewStoneItem);
// // 		return GT_INVALID;
// // 	}
// 	dwErrCode = itemMgr.Add2Bag(pNewStoneItem,ELCLD_HolySoulEquipRemove,TRUE);
// 	if (E_Success != dwErrCode)
// 	{
// 		IMSG(_T("New Item Create Failed,dwErrCode:%u\r\n"),dwErrCode);
// 		::Destroy(pNewStoneItem);
// 		return GT_INVALID;
// 	}
// 	msg.dwErrorCode = E_Success;
// 	SendMessage(&msg,msg.dwSize);
//	return E_Success;
//}
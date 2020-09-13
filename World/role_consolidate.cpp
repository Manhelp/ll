//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_consolidate.cpp
// author: Wjiang
// actor:
// data: 
// last:
// brief: װ��ǿ��
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/specfunc_define.h"
#include "../WorldDefine/compose_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/item_define.h"

#include "role.h"
#include "map.h"
#include "creature.h"
#include "item_creator.h"
#include "title_mgr.h"
#include <math.h>
#include "shielding_mgr.h"

//-----------------------------------------------------------------------------
// ����
// Jason 2009-12-1 ������ɫƷ�ʲ��ϵļӳ�����
// Jason 2009-12-4 �޸ģ�ֻ��ָ�����͵Ĳ��ϣ��ż���Ӱ��ϵ��ͳ��
//-----------------------------------------------------------------------------
DWORD Role::PosyEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	return GT_INVALID;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Posy) )
		return E_Consolidate_NPCCanNotPosy;

	// �ҵ�ǿ������
	const tagPosyProtoSer *pPosyProto = g_attRes.GetPosyProto(dwFormulaID);
	if(!P_VALID(pPosyProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ������ƴ���
	if(pEquip->equipSpec.byPosyTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondPosyTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byPosyTimes + 1 ) != pPosyProto->byPosyTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsPosyPos(pPosyProto->ePosyAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotPosy;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pPosyProto->nPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pPosyProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// �����Ҳ����Ƿ��㹻(˳������ܲ���������
	INT nItemQualityNum[EIQ_End];
	ZeroMemory(nItemQualityNum, sizeof(nItemQualityNum));

	INT nTotleStuff = 0;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pPosyProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pPosyProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pPosyProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pPosyProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->GetNum() < (INT)pPosyProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;

		// Jason ������ָ��ĳ�����͵Ĳ��ϣ����򣬲�ͳ��
		if( EST_Null != pPosyProto->ConsolidateStuff[n].eStuffType )
		{
			nItemQualityNum[pItemStuff->pProtoType->byQuality] += pPosyProto->ConsolidateStuff[n].dwStuffNum;
			nTotleStuff += pPosyProto->ConsolidateStuff[n].dwStuffNum;
		}
	}

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagPosyProtoSer *pProto = const_cast<tagPosyProtoSer*>(pPosyProto);		
	CalIMEffect(ECTS_Posy, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		 GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Posy, (INT16)pPosyProto->ConsolidateStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pPosyProto->dwMoneyConsume, ELCID_Compose_Posy);

	// ����ɹ���
	// ���ճɹ���=�����ɹ���+��-10%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+5%����Ʒ��������
	// /�ܲ�������+10%����Ʒ��������/�ܲ�������+20%����Ʒ��������/�ܲ�����������[1+��װ���ȼ�-75��/150]
	// +����ɫ��Ե/1000��+IM���߼ӳ�                                      ���ճɹ��ʵ�ȡֵΪ0%-100%
	FLOAT fProp = 0;
	if( nTotleStuff )
		fProp = (FLOAT)pPosyProto->nSuccessRate + (((-0.1f * (FLOAT)nItemQualityNum[EIQ_White] 
				+ 0.05f * (FLOAT)nItemQualityNum[EIQ_Green] + 0.1f * (FLOAT)nItemQualityNum[EIQ_Blue] 
				+ 0.2f * (FLOAT)nItemQualityNum[EIQ_Orange] + 0.5 * (FLOAT)nItemQualityNum[EIQ_Purple] ) / (FLOAT)nTotleStuff) 
				* (1.0f + ((FLOAT)pEquip->pProtoType->byLevel - 75.0f) / 150.0f) + ((FLOAT) GetAttValue(ERA_Fortune) / 1000.0f)) * 10000;

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Posy);

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)			// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);
		
		//------------------------------------------------------------------------------
		// ���� װ��ǿ���¼�����
		// zxzhang  2009-12-29
		//------------------------------------------------------------------------------
		GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS,static_cast<DWORD>(ETEST_Posy),GT_INVALID);
		
		// װ�����ƴ�����һ
		pEquip->equipSpec.byPosyTimes++;

		// ����װ����Ӧǿ��ֵ
		// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�
		/* ����Ʒ�ʼӳ�=��-20%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+20%
		   ����Ʒ��������/�ܲ�������+50%����Ʒ��������/�ܲ�������+100%����Ʒ��������/�ܲ���������
		   ����Ʒ�ʼӳɵ�ȡֵΪ0%-100%  */

		// �����ӳ�
		FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pPosyProto->fcoefficientA 
							+ pPosyProto->fcoefficientB) / pPosyProto->fcoefficientC;

		// ����Ʒ�ʼӳ�
		FLOAT fStuffAttInc = 0;
		if( nTotleStuff )
			fStuffAttInc = (-0.3f * (FLOAT)nItemQualityNum[EIQ_White] + 0.5f * (FLOAT)nItemQualityNum[EIQ_Green] 
							  + (FLOAT)nItemQualityNum[EIQ_Blue] + 2.0f * (FLOAT)nItemQualityNum[EIQ_Orange] 
							  + 3.0 * (FLOAT)nItemQualityNum[EIQ_Purple] )
							  / (FLOAT)nTotleStuff; 

		//if(fStuffAttInc < 0.0f)
		//	fStuffAttInc = 0.0f;
		//if(fStuffAttInc > 1.0f)
		//	fStuffAttInc = 1.0f;

		// ����ת�� 
		ERoleAttribute eRoleAtt = ERA_Null;
		ConsolidatePosyAttConvert(eRoleAtt, pPosyProto->ePosyAtt);

		for(INT n = 0; n < MAX_ROLEATT_POSY_EFFECT; ++n)
		{
			if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == eRoleAtt)
			{
				pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + fStuffAttInc));
				break;
			}
			else if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == ERA_Null)
			{
				pEquip->equipSpec.PosyEffect[n].eRoleAtt = eRoleAtt;
				pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + fStuffAttInc));
				break;
			}
		}

		// Ǳ��ֵ����
		// ����������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000
		FLOAT fPefectProp = (fProp * 0.05f) / 10000.0f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ���Ƴɹ���75%
			pEquip->ChangePotVal(-pPosyProto->nPotValConsume * 75 / 100);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-pPosyProto->nPotValConsume);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else				// ʧ��
	{
		// װ��Ǳ��ֵ����5��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-20/*5*/); // Jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM����
//-----------------------------------------------------------------------------
DWORD Role::GMPosyEquip(DWORD dwFormulaID, INT16 n16ItemIndex)
{
	return GT_INVALID;
	// �ҵ�ǿ������
	const tagPosyProtoSer *pPosyProto = g_attRes.GetPosyProto(dwFormulaID);
	if(!P_VALID(pPosyProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16ItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ������ƴ���
	if(pEquip->equipSpec.byPosyTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondPosyTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byPosyTimes + 1 ) != pPosyProto->byPosyTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsPosyPos(pPosyProto->ePosyAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotPosy;

	// װ�����ƴ�����һ
	pEquip->equipSpec.byPosyTimes++;

	// ����װ����Ӧǿ��ֵ
	// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�

	// �����ӳ�
	FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pPosyProto->fcoefficientA 
		+ pPosyProto->fcoefficientB) / pPosyProto->fcoefficientC;

	// ����ת�� 
	ERoleAttribute eRoleAtt = ERA_Null;
	ConsolidatePosyAttConvert(eRoleAtt, pPosyProto->ePosyAtt);

	for(INT n = 0; n < MAX_ROLEATT_POSY_EFFECT; ++n)
	{
		if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == eRoleAtt)
		{
			pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + 0));
			break;
		}
		else if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == ERA_Null)
		{
			pEquip->equipSpec.PosyEffect[n].eRoleAtt = eRoleAtt;
			pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + 0));
			break;
		}
	}

	// ����װ������
	CalEquipFlare(pEquip);
		
	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//-----------------------------------------------------------------------------
// �Կ�
// Jason 2009-12-1 ������ɫƷ������Ӱ��
// Jason 2009-12-4 ֻͳ�Ʋ���������ȷ�Ĳ���
//-----------------------------------------------------------------------------
DWORD Role::EngraveEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	return GT_INVALID;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Engrave) )
		return E_Compose_NPCCanNotEngrave;

	// �ҵ�ǿ������
	const tagEngraveProtoSer *pEngraveProto = g_attRes.GetEngraveProto(dwFormulaID);
	if(!P_VALID(pEngraveProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotEngrave;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// ����Կ̴���
	if(pEquip->equipSpec.byEngraveTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondEngraveTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byEngraveTimes + 1 ) != pEngraveProto->byEngraveTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsEngravePos(pEngraveProto->eEngraveAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotEngrave;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pEngraveProto->nPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pEngraveProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// �����Ҳ����Ƿ��㹻(˳������ܲ���������
	INT nItemQualityNum[EIQ_End];
	ZeroMemory(nItemQualityNum, sizeof(nItemQualityNum));

	INT nTotleStuff = 0;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pEngraveProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pEngraveProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pEngraveProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pEngraveProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->GetNum() < (INT)pEngraveProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;

		if( EST_Null != pEngraveProto->ConsolidateStuff[n].eStuffType )
		{
			nItemQualityNum[pItemStuff->pProtoType->byQuality] += pEngraveProto->ConsolidateStuff[n].dwStuffNum;
			nTotleStuff += pEngraveProto->ConsolidateStuff[n].dwStuffNum;
		}
	}

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagEngraveProtoSer  *pProto = const_cast<tagEngraveProtoSer*>(pEngraveProto);
	CalIMEffect(ECTS_Engrave, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Engrave, (INT16)pEngraveProto->ConsolidateStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pEngraveProto->dwMoneyConsume, ELCID_Compose_Engrave);

	// ����ɹ���
	// ���ճɹ���=�����ɹ���+��-10%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+5%����Ʒ��������
	// /�ܲ�������+10%����Ʒ��������/�ܲ�������+20%����Ʒ��������/�ܲ�����������[1+��װ���ȼ�-75��/150]
	// +����ɫ��Ե/1000��+IM���߼ӳ�                                      ���ճɹ��ʵ�ȡֵΪ0%-100%
	FLOAT fProp = 0;
	if( nTotleStuff )
		fProp = (FLOAT)pEngraveProto->nSuccessRate + (((-0.1f * (FLOAT)nItemQualityNum[EIQ_White] 
				+ 0.05f * (FLOAT)nItemQualityNum[EIQ_Green] + 0.1f * (FLOAT)nItemQualityNum[EIQ_Blue] 
				+ 0.2f * (FLOAT)nItemQualityNum[EIQ_Orange] + 0.5 * (FLOAT)nItemQualityNum[EIQ_Purple] ) / (FLOAT)nTotleStuff) 
				* (1.0f + ((FLOAT)pEquip->pProtoType->byLevel - 75.0f) / 150.0f) 
				+ ((FLOAT) GetAttValue(ERA_Fortune) / 1000.0f)) * 10000;

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Engrave);

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)			// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

	

		//--------------------------------------------------------------------------------------------
        // �Կ� װ��ǿ���¼�����
		// zxzhang 2009-12-29
		//--------------------------------------------------------------------------------------------
        GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS,static_cast<WORD>(ETEST_Engrave), GT_INVALID); 

 		// װ�����ƴ�����һ
		pEquip->equipSpec.byEngraveTimes++;

		// ����װ����Ӧǿ��ֵ
		// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�
		/* ����Ʒ�ʼӳ�=��-20%����Ʒ��������/�ܲ�������+0%����Ʒ��������/�ܲ�������+20%
		����Ʒ��������/�ܲ�������+50%����Ʒ��������/�ܲ�������+100%����Ʒ��������/�ܲ���������
		����Ʒ�ʼӳɵ�ȡֵΪ0%-50%  */

		// �����ӳ�
		FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pEngraveProto->fcoefficientA 
			+ pEngraveProto->fcoefficientB) / pEngraveProto->fcoefficientC;

		// ����Ʒ�ʼӳ�
		FLOAT fStuffAttInc = 0;
		if( nTotleStuff )
			fStuffAttInc = (-0.3f * (FLOAT)nItemQualityNum[EIQ_White] + 0.5f * (FLOAT)nItemQualityNum[EIQ_Green] 
							  + (FLOAT)nItemQualityNum[EIQ_Blue] + 2.0f * (FLOAT)nItemQualityNum[EIQ_Orange] 
							  + 3.0f * (FLOAT)nItemQualityNum[EIQ_Purple] )
							  / (FLOAT)nTotleStuff; 

		//if(fStuffAttInc < 0.0f)
		//	fStuffAttInc = 0.0f;
		//if(fStuffAttInc > 1.0f)
		//	fStuffAttInc = 1.0f;

		// ���ռӳ�
		INT nEngraveAttInc = (INT)(fBaseAttInc * (1.0f + fStuffAttInc));

		switch(pEngraveProto->eEngraveAtt)
		{
		case EEngraveAtt_WeaponDmg:
			pEquip->equipSpec.nEngraveAtt[0] += nEngraveAttInc;
			pEquip->equipSpec.nEngraveAtt[1] += nEngraveAttInc;
			break;
		case EEngraveAtt_WeaponSoul:
			pEquip->equipSpec.nEngraveAtt[2] += nEngraveAttInc;
			break;
		case EEngraveAtt_Armor:
			pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
			break;
		case EEngraveAtt_Deration:
			pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
			break;
		default:
			break;
		}

		// Ǳ��ֵ����
		// ����������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000
		FLOAT fPefectProp = (fProp * 0.05f) / 10000.0f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ���Ƴɹ���75%
			pEquip->ChangePotVal(-pEngraveProto->nPotValConsume * 75 / 100);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-pEngraveProto->nPotValConsume);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else				// ʧ��
	{
		// װ��Ǳ��ֵ����5��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-20/*5*/); //Jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM �Կ�
//-----------------------------------------------------------------------------
DWORD Role::GMEngraveEquip(DWORD dwFormulaID, INT16 n16ItemIndex)
{
	return GT_INVALID;
	// �ҵ�ǿ������
	const tagEngraveProtoSer *pEngraveProto = g_attRes.GetEngraveProto(dwFormulaID);
	if(!P_VALID(pEngraveProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16ItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotEngrave;

	// ����Կ̴���
	if(pEquip->equipSpec.byEngraveTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondEngraveTime;

	// ���ǿ���䷽�Ƿ�Ϸ�
	if((pEquip->equipSpec.byEngraveTimes + 1 ) != pEngraveProto->byEngraveTimes)
		return E_Consolidate_FormulaNotMatch;

	// ����Ƿ���ǿ���䷽��Ӧ��װ������
	if(!g_attRes.IsEngravePos(pEngraveProto->eEngraveAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotEngrave;

	// װ�����ƴ�����һ
	pEquip->equipSpec.byEngraveTimes++;

	// ����װ����Ӧǿ��ֵ
	// ��ֵ=��װ���ȼ���fcoefficientA + fcoefficientB��/ fcoefficientC ����1+����Ʒ�ʼӳɣ�

	// �����ӳ�
	FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pEngraveProto->fcoefficientA 
		+ pEngraveProto->fcoefficientB) / pEngraveProto->fcoefficientC;

	// ���ռӳ�
	INT nEngraveAttInc = (INT)(fBaseAttInc * (1.0f + 0));

	switch(pEngraveProto->eEngraveAtt)
	{
	case EEngraveAtt_WeaponDmg:
		pEquip->equipSpec.nEngraveAtt[0] += nEngraveAttInc;
		pEquip->equipSpec.nEngraveAtt[1] += nEngraveAttInc;
		break;
	case EEngraveAtt_WeaponSoul:
		pEquip->equipSpec.nEngraveAtt[2] += nEngraveAttInc;
		break;
	case EEngraveAtt_Armor:
		pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
		break;
	case EEngraveAtt_Deration:
		pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
		break;
	default:
		break;
	}
	
	// ����װ������
	CalEquipFlare(pEquip);

	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//-----------------------------------------------------------------------------
// ��Ƕ
//-----------------------------------------------------------------------------
DWORD Role::InlayEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// ��鱦ʯ�Ƿ�����
	tagItem *pItemGem = GetItemMgr().GetBagItem(n64SrcItemID);
	if (!P_VALID(pItemGem))
	{
		return E_Consolidate_Gem_Not_Exit;
	}
	else
	{
		if (pItemGem->bLock)
		{
			return E_Consolidate_Gem_Locked;
		}
	}

	BOOL bEquiped = FALSE;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(P_VALID(pEquip))
	{
		// ����
		bEquiped = FALSE;
	}
	else
	{
		pEquip = GetItemMgr().GetEquipBarEquip(n64DstItemID);
		if(P_VALID(pEquip))
		{
			// װ����
			bEquiped = TRUE;
		}
		else
		{
			return E_Consolidate_Equip_Not_Exist;
		}
	}

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// Jason 2010-7-16 v1.0.0
	//if( MIsFaBao(pEquip) )
	//{
	//	if( pItemGem->pProtoType->nSpecFuncVal1 > ((tagFabao*)pEquip)->nUseLevel )
	//		return E_ConsolidateInlay_EquipLevelLower;
	//}
	//else
	if( pItemGem->pProtoType->nSpecFuncVal1 > pEquip->pEquipProto->byLevel )
		return E_ConsolidateInlay_EquipLevelLower;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemGem->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	if((INT)pEquip->equipSpec.byHoleNum == 0)
		return E_Consolidate_Gem_Not_Hole;

	// ����Ƿ�ʯ�Ѿ�����
	//if(pEquip->equipSpec.dwHoleGemID[pEquip->equipSpec.byHoleNum - 1] != GT_INVALID 
	//	&& pEquip->equipSpec.dwHoleGemID[pEquip->equipSpec.byHoleNum - 1] != 0 )
	bool bFull = true;
	for(int i = 0; i < pEquip->equipSpec.byHoleNum;++i)
	{
		if( !P_VALID(pEquip->equipSpec.dwHoleGemID[i]) )
		{
			bFull = false;
			break;
		}
	}
	if( bFull )
		return E_Consolidate_Gem_Full;

	// ��ⱦʯ�Ƿ�����Ƕ��װ����
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotInlay;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	//if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
	//	return E_Consolidate_ValConsume_Inadequate;

	if(bEquiped)
	{
		SaveTheValueBeforeStrengthen();
		ProcEquipEffect(NULL, pEquip, pEquip->n16Index, TRUE, FALSE);
	}

	// ��Ƕ��ʯ
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
		{
			pEquip->equipSpec.dwHoleGemID[i] = pItemGem->dwTypeID;
			break;
		}
	}

	// ���ݱ�ʯ�İ�״̬�޸ķǰ󶨵�װ��Ϊ��
	if ( pItemGem->IsBind() || pItemGem->IsAccountBind() )
	{
		if ( !pEquip->IsBind() )
		{
			pEquip->byBind = EBS_Bind;
			// �ı�װ���İ�״̬��֪ͨDB�Ϳͻ���
			pEquip->SetUpdate(EUDBS_Update);
			GetItemMgr().SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);
		}		
	}

	// ����װ������
	CalEquipFlare(pEquip);

	// ����װ��Ǳ��ֵ
	//pEquip->ChangePotVal(-(INT)pConsolidateProto->dwPotValConsume);
	GetItemMgr().UpdateEquipSpec(*pEquip);

	if(bEquiped)
	{
		ProcEquipEffect(pEquip, NULL, pEquip->n16Index, TRUE, TRUE);
		RestoreTheValueAfterStrengthen();
	}

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_Enchase, 1);

	// �ƺ��¼�����
	GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);


	//-----------------------------------------------------------------------------------------------------
	// ��Ƕ װ���ƺ��¼����� 
	// zxzhang 2009-12-29
	//-----------------------------------------------------------------------------------------------------
	GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS,static_cast<DWORD>(ETEST_Inlay),GT_INVALID);
	return E_Compose_Consolidate_Success;	
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
DWORD Role::ChiselEquip(INT64 n64SrcItemID, INT64 n64SuffID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	BOOL bEquipd = FALSE;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64SrcItemID);
	if (P_VALID(pEquip))
	{
		bEquipd = FALSE;
	}
	else
	{
		pEquip = (tagEquip*)GetItemMgr().GetEquipBarEquip(n64SrcItemID);
		if (P_VALID(pEquip))
		{
			bEquipd = TRUE;
		}
		else
		{
			return E_Consolidate_Equip_Not_Exist;
		}

	}	

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if(!MIsIdentified(pEquip))
		return E_Consolidate_NotIdentified;

	if(pEquip->equipSpec.bCanCut == false)
		return E_Consolidate_Equip_CanNot_Chisel;

	// �ҵ���ʯ
	tagItem *pItemChisel = GetItemMgr().GetBagItem(n64SuffID);
	if(!P_VALID(pItemChisel) )
	{
		return E_Consolidate_Chisel_Not_Exit;		
	}

	// ����5�����µĿױ����� ԭ����ʯ
	if(pEquip->equipSpec.byHoleNum < 5 && pItemChisel->pProtoType->eSpecFunc != EISF_Chisel )
	{
		return E_Consolidate_Chisel_Not_Exit;	
	}

	// ����5�����ϵĿױ����� ����ʯ
	if(pEquip->equipSpec.byHoleNum >= 5 && pEquip->equipSpec.byHoleNum < MAX_EQUIPHOLE_NUM && pItemChisel->pProtoType->eSpecFunc != EISF_NewChisel )
	{
		return E_Consolidate_Chisel_Not_Exit;	
	}

	// �ȼ�С���������װ���ȼ����򲻿ɽ��п���
	if(pEquip->pProtoType->byLevel > (pItemChisel->pProtoType->byLevel * 10))
		return E_Consolidate_Chisel_Not_Match;

	// ����ɹ���
	FLOAT fProp = 0.0;

	if (pItemChisel->pProtoType->eSpecFunc == EISF_Chisel)
	{
		// ���װ����Ƕ����
		if(pEquip->equipSpec.byHoleNum >= 5)
			return E_Consolidate_Equip_Hole_Full;


		// ����ɹ���(�������ճɹ���=��ʯ�����ɹ��ʡ���1-װ����ǰ��Ƕ������/5��+����ɫ��Ե/1000��)
		fProp = (((FLOAT)pItemChisel->pProtoType->nSpecFuncVal1 * (pow( 1.0f - (FLOAT)pEquip->equipSpec.byHoleNum / 5.0f,1.5f)) ) / 10000.0f 
			+ (FLOAT)GetAttValue(ERA_Fortune) / 5000.0f) *10000.0f;

		// ����B�����ԶԳɹ���Ӱ��
		fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Cut);
	}

	if (pItemChisel->pProtoType->eSpecFunc == EISF_NewChisel)
	{
		// ���װ����Ƕ����
		if(pEquip->equipSpec.byHoleNum >= MAX_EQUIPHOLE_NUM)
			return E_Consolidate_Equip_Hole_Full;


		// ����ʯ�����׵ĳɹ���Ϊ100%
		fProp = 10000.0f;	
	}


	// ��������
	GetItemMgr().DelFromBag(n64SuffID, (DWORD)ELCID_Compose_Chisel, 1);

	

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)		// �ɹ�
	{

		//----------------------------------------------------------------------------------------------
		// ǿ������װ�� �ƺ��¼�����
		// zxzhang 2009-12-29
		//----------------------------------------------------------------------------------------------
        GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID );
		 
		//----------------------------------------------------------------------------------------------
		//���� �ƺ��¼�����
		// zxzhang 2009-12-29
		//----------------------------------------------------------------------------------------------
		GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS, static_cast<DWORD>(ETEST_Chisel), GT_INVALID );

		// Jason 2010-6-12 ȥ��������
		// ����������
		// ������=1%+����ɫ��Ե/1000����ȡֵ��0-100%��
		//FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		//fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		//BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		//if(bPefect)
		//{
		//	pEquip->equipSpec.byHoleNum = ((pEquip->equipSpec.byHoleNum + 2 > MAX_EQUIPHOLE_NUM) ? MAX_EQUIPHOLE_NUM : (pEquip->equipSpec.byHoleNum + 2));
		//	GetItemMgr().UpdateEquipSpec(*pEquip);
		//	return  E_Compose_Consolidate_Perfect;
		//}
		//else
		{
			++(pEquip->equipSpec.byHoleNum);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			// �����ű�
			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript))
				pRoleScript->OnDigHoleOnEquip(this, pEquip->dwTypeID, bResult, pEquip->equipSpec.byHoleNum);
			return E_Compose_Consolidate_Success;
		}
	}

	// �����ű�
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
		pRoleScript->OnDigHoleOnEquip(this, pEquip->dwTypeID, bResult, pEquip->equipSpec.byHoleNum);

	return E_Compose_Consolidate_Lose;
}

//-----------------------------------------------------------------------------
//�õ����Ľ׶�
//-----------------------------------------------------------------------------
INT Role::GetSpinStep(BYTE byMinUseLevel)
{
	switch(byMinUseLevel)
	{
	case ESSEL_Null:
		return ESpinStep_Null;
	case ESSEL_Basic:
		return ESpinStep_Basic;
	case ESSEL_Middle:
		return ESpinStep_Middle;
	case ESSEL_Senior:
		return ESpinStep_Senior;
	default:
		return ESpinStep_Null;
	}

	return ESpinStep_Null;
}

//-----------------------------------------------------------------------------
//�õ�����װ����Ʒ��
//-----------------------------------------------------------------------------
EItemQuality Role::GetSpinItemQuality(ESpinStep eStep)
{
	switch(eStep)
	{
	case ESpinStep_Null:
		return EIQ_White;

	case ESpinStep_Basic:
		return EIQ_Green;

	case ESpinStep_Middle:
		return EIQ_Blue;

	case ESpinStep_Senior:
		return EIQ_Orange;

	default:
		return EIQ_White;
	}

	return EIQ_White;
}
//-----------------------------------------------------------------------------
// ����ʱװ
//-----------------------------------------------------------------------------
DWORD Role::SpinFashion(ESuitAttType eSpinType, INT64 n64EquipSerial, DWORD dwStuffTypeID,  DWORD dwStuffItemNum, DWORD dwCmdID)
{

	DWORD dwErrCode = E_Compose_Consolidate_Success;

	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	//�õ����ĵĳɹ���
	const tagSpinSuccessRateProto *pSuccessRate = g_attRes.GetSpinRateProto(dwStuffItemNum);
	if(!P_VALID(pSuccessRate))
		return GT_INVALID;

	if(eSpinType >= ESSAT_End || eSpinType < ESSAT_Null )
		return GT_INVALID;

	//////////////////////////////////////////////////////////////////////////


	//�õ�Ҫ���ĵ�ʱװ
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64EquipSerial);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_CanNotSpin;

	//�Ƿ�Ϊ�ɾ���ʱװ
	if(pEquip->pEquipProto->eTypeReserved != EITR_Spin)
		return E_Consolidate_CanNotSpin;

	//ȡ�����õ���Ʒԭ�� ��֤����ʹ�������Ƿ�ΪEISF_SpinFashionDressItem
	tagItemProto* pStuffItem = g_attRes.GetItemProto(dwStuffTypeID);
	if(!P_VALID(pStuffItem) || pStuffItem->eSpecFunc != EISF_SpinFashionDressItem)
		return GT_INVALID;

	//�жϸ���
	if(dwStuffItemNum > GetItemMgr().GetBagSameItemCount(dwStuffTypeID))
		return E_Compose_Stuff_Not_Enough;


	//�õ��÷�װ���ھ��ĵ��ĸ��׶�
	INT nStep = this->GetSpinStep(pEquip->pEquipProto->byMinUseLevel);

	//�����Ƿ�����پ���
	if(++nStep == ESpinStep_End)
		return E_Consolidate_Spin_TopLevel;

//////////////////////////////////////////////////////////////////////////

	//Ѱ�ҵ�ǰ��Ʒ�ľ���ԭ��
	const tagSpinProto *pSpinProto = NULL;
	for(INT nLoop = ESSAT_Brave; nLoop != ESSAT_End; nLoop++)
	{
		//�õ�����ԭ��
		pSpinProto = g_attRes.GetSpinProto(tagSpinKey(pEquip->dwTypeID,(ESuitAttType)nLoop));
		if(P_VALID(pSpinProto))
			break;
	}
	if(!P_VALID(pSpinProto))
		return E_Consolidate_Equip_Not_Exist;

	//����δ������Ʒ��typeid��Ŀ�꾫���������� ȡĿ�꾫��ԭ��
	const tagSpinProto *pSpinAfterProto = g_attRes.GetSpinProto(tagSpinKey(pSpinProto->dwSpinFashionTypeID[ESpinStep_Null],eSpinType));
	if(!P_VALID(pSpinAfterProto))
		return E_Consolidate_Equip_Not_Exist;

//////////////////////////////////////////////////////////////////////////
	if( nStep < ESpinStep_Null || nStep >= ESpinStep_End)
		return GT_INVALID;
	//�ж��Ƿ�ɹ�
	BOOL bPefect = IUTIL->Rand() % 10000 <= pSuccessRate->dwSuccessRate[nStep];

//////////////////////////////////////////////////////////////////////////
	//���������װ��ԭ��
	tagEquipProto *pSpinAfterEquipProto = g_attRes.GetEquipProto(pSpinAfterProto->dwSpinFashionTypeID[nStep]);
	if(!P_VALID(pSpinAfterEquipProto))
		return GT_INVALID;
/*
	//����߻���Ӹ�suitid��ȽϿ�
	//�Ҷ�Ӧ����װ���Ե���װid
	const tagSuitProto* pSuitProto = NULL;
	BOOL bFinded = FALSE;
	for(INT nSuitLoop = 0; nSuitLoop<MAX_PEREQUIP_SUIT_NUM; ++nSuitLoop)
	{
		const tagSuitProto* pSuitProto = g_attRes.GetSuitProto(pSpinAfterEquipProto->dwSuitID[nSuitLoop]);
		if(pSpinAfterEquipProto->dwSuitID[nSuitLoop] == GT_INVALID || !P_VALID(pSuitProto))
			continue;

		//��װ����������ͬ
		if(pSpinAfterProto->eAttType != ESSAT_Null && pSuitProto->eAttType == pSpinAfterProto->eAttType)
		{
			bFinded = TRUE;
			break;
		}
	}
	
//////////////////////////////////////////////////////////////////////////
	if(!P_VALID(pSuitProto) || bFinded == FALSE)
		return GT_INVALID;
*/
	//�ɹ��Ļ�
	if(bPefect == TRUE)
	{
		 //ɾ����ǿ����Ʒ
		GetItemMgr().DelFromBag(n64EquipSerial, (DWORD)ELCLD_Compose_Spin, 1);

		EItemQuality eQuality = this->GetSpinItemQuality((ESpinStep )nStep);
		//�����������µ�
		tagItem *pEquip = ItemCreator::CreateEx(EICM_Spin, GT_INVALID, pSpinAfterProto->dwSpinFashionTypeID[nStep], 1, eQuality);

		//�Ļ�������
		if(!P_VALID(pEquip))
			goto SPIN_END_LINE;

		if(GetItemMgr().Add2Bag(pEquip, ELCLD_Compose_Spin, TRUE, TRUE) != E_Success)
		{
			Destroy(pEquip);
			return E_Compose_Consolidate_Lose;
		}

	}
	//ʧ��
	else
	{
		dwErrCode = E_Compose_Consolidate_Lose;
	}

SPIN_END_LINE:

	//ɾ������
	GetItemMgr().RemoveFromRole(dwStuffTypeID, dwStuffItemNum, ELCLD_Compose_Spin );

	return dwErrCode;
}

//-----------------------------------------------------------------------------
// ʱװȾɫ
//-----------------------------------------------------------------------------
DWORD Role::DyeFashion(INT64 n64DyeSN, INT64 n64EquipSerial, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���Ⱦɫʱװ
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64EquipSerial);
	if(!P_VALID(pEquip))
		return E_Dye_NotExist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Dye_OtherItem;

	if(!MIsFashion(pEquip))
		return E_Dye_OtherItem;

	if( MIsFaBao(pEquip) )
		return E_Dye_OtherItem;

	if (pEquip->pEquipProto->bCanDye == false)
		return E_Dye_OtherItem;

	// �ҵ�Ⱦ��
	tagItem *pItemDye = GetItemMgr().GetBagItem(n64DyeSN);
	if(!P_VALID(pItemDye) || pItemDye->pProtoType->eSpecFunc != EISF_Dye )
	{
		return E_Dye_NotExist;
	}

	// �����ɫ�Ƿ���ͬ
	if(pEquip->equipSpec.n8ColorID == pItemDye->pProtoType->nSpecFuncVal1)
		return E_Dye_Exist;

	// �ȼ�С����Ⱦɫ��װ���ȼ����򲻿ɽ���Ⱦɫ
	if(pEquip->pProtoType->byLevel > (pItemDye->pProtoType->byLevel * 10))
		return E_Dye_LvLimit;

	// �ı�ʱװ��ɫ
	pEquip->equipSpec.n8ColorID = pItemDye->pProtoType->nSpecFuncVal1;
	GetItemMgr().UpdateEquipSpec(*pEquip);

	// ��������
	GetItemMgr().DelFromBag(n64DyeSN, (DWORD)ELCID_Compose_Dye, 1);

	//---------------------------------------------------------------------------
	// ʱװȾɫ�ƺ��¼�����
	// zxzhang 2009-12-29 
    //---------------------------------------------------------------------------
	GetTitleMgr()->SigEvent( ETE_CLOTHCOLOR,GT_INVALID ,GT_INVALID );

	return E_Success;
}

//-----------------------------------------------------------------------------
// ����B�����Զ�ǿ���ɹ��ʵ�Ӱ��
//-----------------------------------------------------------------------------
FLOAT Role::CalSpecAttEffectSuc(EEquipSpecAtt eSpecAtt, FLOAT fProp, EEquipSpecAtt eSpecType)
{
	switch (eSpecAtt)
	{
	case EESA_Guard_Posy:
	case EESA_Guard_Brand:
	case EESA_Guard_Engrave:
	case EESA_Guard_LongFu:
	case EESA_Guard_Quench:
	case EESA_Guard_Cut:
		{
			if(eSpecAtt != eSpecType)
				return fProp;
			else
				return 10000.0f;
		}
	case EESA_Guard_AllProduce:
		return 10000.0f;
		break;
	case EESA_Guard_Talisman:
		return fProp * 1.05f;
		break;
	default:
		return fProp;
		break;
	}
}

// ����B�����Զ�ǿ�������ʵ�Ӱ��
FLOAT Role::CalSpecAttEffectPef(EEquipSpecAtt eSpecAtt, FLOAT fPerfect)
{
	switch(eSpecAtt)
	{
	case EESA_Guard_Fortune:
		return fPerfect * 1.05f;
		break;
	default:
		return fPerfect;
		break;
	}
}

//-----------------------------------------------------------------------------
// ��ӡ
//-----------------------------------------------------------------------------
DWORD Role::BrandEquip(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD dwCmdID)
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotBrand;

	// �ҵ�ӡ��
	tagItem *pItemBrand = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemBrand) || pItemBrand->pProtoType->eSpecFunc != EISF_Brand )
		return E_Consolidate_Brand_Not_Exit;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemBrand->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byBrandLevel + pItemBrand->pProtoType->byLevel) > MAX_BRAND_LEVEL)
		return E_Consolidate_Brand_TopLevel;

	// ���װ���Ƿ��ܱ�ӡ��
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotBrand;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// ��¼ӡ�ǵȼ�
	BYTE byLevel = pItemBrand->pProtoType->byLevel;

	DWORD dwBrandItemTypeID = pItemBrand->dwTypeID;

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_Brand, 1);

	// �ɹ���=ʥӡ�����ɹ��ʡ���1-��ǰ׷�Ӵ���/10��^1.2+����/5000
	FLOAT fProp = pConsolidateProto->nSuccessRate * pow((1.0f - (FLOAT)(pEquip->equipSpec.byBrandLevel)/10.0f), 1.2f) + (FLOAT)GetAttValue(ERA_Fortune) / 5000.0f; 
	
	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Brand);

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagConsolidateItem *pProto = const_cast<tagConsolidateItem*>(pConsolidateProto);		
	CalIMEffect(ECTS_Brand, IMEffect, n64IMID, pProto);

	// ����ɹ��� 
	BOOL bResult = IUTIL->Rand() % 10000 <= fProp;
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if(bResult)		// �ɹ�
	{
		// �ƺ��¼�����
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

		//----------------------------------------------------------------------------------------
		// ǿ����ӡ �ƺ�  zxzhang 
		//----------------------------------------------------------------------------------------
   
        GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, static_cast<DWORD>(ETEST_Brand), GT_INVALID); 
  

		// װ����ӡ�ȼ�(������Ϊӡ�ǵȼ�)
		pEquip->equipSpec.byBrandLevel += byLevel;

		// װ�����Ӷ�Ӧǿ������
		for(INT n = 0; n < MAX_CONSOLIDATE_ROLEATT; ++n)
		{
			INT nIdx = 0;
			if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_Derate_Start &&
				pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_Derate_End)
			{
				nIdx = MTransERADerate2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
			}
			else if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_transform_Start &&
				pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_transform_End)
			{
				nIdx = MTransERATransform2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
			}
			pEquip->equipSpec.nBrandAtt[nIdx] += pConsolidateProto->tagRoleAtt[n].nAttVal;
		}

		// �����Ƿ�����������������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000)
		FLOAT fPefectProp = (pConsolidateProto->nSuccessRate * 0.05f) / 10000.0f+ (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

#if 0
		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ��ӡ�ɹ���75%
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume * 75 / 100));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
#endif
		{
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));
			GetItemMgr().UpdateEquipSpec(*pEquip);

			if (P_VALID(pRoleScript))
				pRoleScript->OnBrandEquip(this, pEquip->dwTypeID, dwBrandItemTypeID, TRUE, pEquip->equipSpec.byBrandLevel);

			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// װ��Ǳ��ֵ����10��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-40/*10*/);// jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);

		if (P_VALID(pRoleScript))
			pRoleScript->OnBrandEquip(this, pEquip->dwTypeID, dwBrandItemTypeID, FALSE, pEquip->equipSpec.byBrandLevel);
		
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
DWORD Role::LoongSoulEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID)
{
	return GT_INVALID;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotLoongSoul;

	// �ҵ�����ʯ
	tagItem *pItemLoongSoul = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemLoongSoul) || pItemLoongSoul->pProtoType->eSpecFunc != EISF_LongHunShi )
		return E_Consolidate_LoongSoul_Not_Exit;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemLoongSoul->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byLongfuLevel + pItemLoongSoul->pProtoType->byLevel) > MAX_LONGHUN_LEVEL)
		return E_Consolidate_LoongSoul_TopLevel;

	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotLoongSoul;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// ��¼����ʯ�ȼ�
	BYTE byLevel = pItemLoongSoul->pProtoType->byLevel;

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_LongFu, 1);

	// ����ɹ���(�ɹ���=�����ɹ��ʡ�[1+��װ���ȼ�-75��/300]+����ɫ��Ե/1000))
	FLOAT fProp = pConsolidateProto->nSuccessRate * (1.0f + (FLOAT)(pEquip->pProtoType->byLevel - 75.0f) / 300.0f)
					+ ((FLOAT)GetAttValue(ERA_Fortune) / 1000.0f) * 10000;

	// ����B�����ԶԳɹ���Ӱ��
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_LongFu);

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)		// �ɹ�
	{
		// �ƺ��¼�����������ǿ������
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, ETEST_LoongSoul, GT_INVALID);

		// װ������ȼ�(������Ϊ����ʯ�ȼ�)
		pEquip->equipSpec.byLongfuLevel += byLevel;

		// װ�����Ӷ�Ӧǿ������
		pEquip->equipSpec.byLongfuAtt[pConsolidateProto->eWuXing] += byLevel;
		
		// �����Ƿ�������������=1%+����ɫ��Ե/1000))
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// ����B�����Զ�������Ӱ��
		fPefectProp =  CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// ����װ������
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// ����װ��Ǳ��ֵΪ����ɹ���75%
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume * 75 / 100));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// װ����Ӧ�������ȼ���0���������꣬������װ��Ǳ��ֵ��װ���������������
		pEquip->equipSpec.byLongfuLevel = 0;
		ZeroMemory(pEquip->equipSpec.byLongfuAtt, sizeof(pEquip->equipSpec.byLongfuAtt));
		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM����
//-----------------------------------------------------------------------------
DWORD Role::GMLoongSoulEquip(INT64 n64SrcItemID, INT16 n16DstItemIndex)
{
	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16DstItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotLoongSoul;

	// �ҵ�����ʯ
	tagItem *pItemLoongSoul = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemLoongSoul) || pItemLoongSoul->pProtoType->eSpecFunc != EISF_LongHunShi )
		return E_Consolidate_LoongSoul_Not_Exit;

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemLoongSoul->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byLongfuLevel + pItemLoongSoul->pProtoType->byLevel) > MAX_LONGHUN_LEVEL)
		return E_Consolidate_LoongSoul_TopLevel;

	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotLoongSoul;

	// ��¼����ʯ�ȼ�
	BYTE byLevel = pItemLoongSoul->pProtoType->byLevel;

	// װ������ȼ�(������Ϊ����ʯ�ȼ�)
	pEquip->equipSpec.byLongfuLevel += byLevel;

	// װ�����Ӷ�Ӧǿ������
	pEquip->equipSpec.byLongfuAtt[pConsolidateProto->eWuXing] += byLevel;

	// ����װ������
	CalEquipFlare(pEquip);

	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//---------------------------------------------------------------------------------
// ���
//---------------------------------------------------------------------------------
DWORD Role::QuenchEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, 
											INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	return GT_INVALID;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Quench) )
		return E_Consolidate_NPCCanNotQuench;

	// �ҵ�ǿ������
	const tagQuenchProtoSer *pQuenchProto = g_attRes.GetQuenchProto(dwFormulaID);
	if(!P_VALID(pQuenchProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ���ǿ��װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_False;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pQuenchProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pQuenchProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// ԭ���Ե�����ֵ�Ƿ��㹻
	if(pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] < pQuenchProto->srcQuenchAtt.nWuXingValue)
		return E_Consolidate_WXValueInadequate;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pQuenchProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pQuenchProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pQuenchProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pQuenchProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->GetNum() < (INT)pQuenchProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;
	}

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagQuenchProtoSer *pProto = const_cast<tagQuenchProtoSer*>(pQuenchProto);
	CalIMEffect(ECTS_Quench, IMEffect, n64IMID, pProto);

	// �����������
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Quench, (INT16)pQuenchProto->ConsolidateStuff[n].dwStuffNum);
	}

	// װ��Ǳ��ֵ����
	pEquip->ChangePotVal(-(INT)(pQuenchProto->dwPotValConsume));

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pQuenchProto->dwMoneyConsume, ELCID_Compose_Quench);

	// ����B�����ԶԳɹ���Ӱ��
	FLOAT fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), (FLOAT)pQuenchProto->nSuccessRate, EESA_Guard_Quench);

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= fProp;
	if(bResult)
	{
		// ����Ŀ������
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] -= pQuenchProto->srcQuenchAtt.nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->dstQuenchAtt.eWuXing] += pQuenchProto->dstQuenchAtt.nWuXingValue;
		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Success;
	}
	else		// ʧ��
	{
		// ���ӵ����������ڳ���ԭ���Ժ�Ŀ��������������������������
		EWuXing		eType = EWX_Null; 
		INT			nWuXingValue = pQuenchProto->dstQuenchAtt.nWuXingValue;
		BYTE		byTotalValue = 0;
		while (TRUE)
		{
			eType =(EWuXing)(IUTIL->Rand() % 5);
			if(pQuenchProto->srcQuenchAtt.eWuXing != eType && pQuenchProto->dstQuenchAtt.eWuXing != eType)
				break;
		}

		// �䷽���������Ŀ����������ֵΪn����ʧ��ʱʵ������ֵ��[n-2��n+1]�������
		// ȡֵ��ΧΪ[1��10]���Ҵ����������������ܺϲ��ܳ���10
		nWuXingValue = IUTIL->Rand() % 3 + (nWuXingValue - 2);

		for(INT i = 0; i < EWX_End; ++i)
		{
			byTotalValue += pEquip->equipSpec.byLongfuAtt[i];
		}

		nWuXingValue = (nWuXingValue > (10 - byTotalValue)) ? (10 - byTotalValue) : nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[eType] += nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] -= pQuenchProto->srcQuenchAtt.nWuXingValue;

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Consolidate_False;

	}
}
//---------------------------------------------------------------------------------
// �����ϳ���Ʒ
//---------------------------------------------------------------------------------
DWORD Role::ProduceItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID,
								INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	if( IsShieldingFormula(dwFormulaID) )
		return E_Compose_Formula_Not_Exist;
	// �ҵ��ϳɱ�����
	const tagProduceProtoSer* pProduceProto = g_attRes.GetProduceProto(dwFormulaID);

	if(!P_VALID(pProduceProto))
		return E_Compose_Formula_Not_Exist;

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckProduceLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pProduceProto, n64StuffID, nArraySz);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	tagProduceProtoSer *pProto = const_cast<tagProduceProtoSer*>(pProduceProto);
	CalIMEffect(ECTS_Produce, IMEffect, n64IMID, pProto);

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);

	if (P_VALID(m_pScript))
	{
		m_pScript->DecSilverByCompose(this, pProduceProto->dwProItemTypeID, pProduceProto->dwMoneyConsume);
	}

	// ���Ӻϳɼ���������(ֻ�м��ܺϳɲŻ����Ӽ��������ȣ�
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pProduceProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// �۳���ҵĻ���ֵ
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_CONSOLODATE,pProduceProto->dwVitalityConsume);
	//ChangeVitality(-(INT)pProduceProto->dwVitalityConsume);
	ChangeVitality(-(INT)abs(nVitality));

	// ����ɹ���
	FLOAT	fProp = (FLOAT)pProduceProto->nSuccessRate;

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	INT nSuc = 0;
	if( EPCT_Gem == pProduceProto->eProType )
	{
		nSuc = GetSpecSkillValue(ESSF_Gem,pProduceProto->dwProItemTypeID);
	}

	if( EPCT_GodArtisan == pProduceProto->eProType )
	{
		nSuc = GetSpecSkillValue(ESSF_Brand,pProduceProto->dwProItemTypeID);
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= ((INT)fProp+nSuc);

	//ILOG->Write(_T("�ϳɳɹ��� %d \r\n"), (INT)fProp+nSuc);

	// �ƺ��¼�����
	if (bResult && GT_VALID(dwNPCID))
		GetTitleMgr()->SigEvent(ETE_COMPOSITE_SKILL_SUCCESS	, GT_INVALID, GT_INVALID);

	if(bResult)			// �ɹ�
	{
		// �����������
		for(INT n = 0; n < nArraySz; ++n)
		{
			if(!GetMap()->IsWar())
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
			else
				GetItemMgr().DelFromWarBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
		}
		// ��úϳɵ���
		if(pProduceProto->eComType == ECOMT_RefineDan)
		{
			// ����������
			//FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;
			//BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

			//// ���������������Ʒ��������
			//if(bPefect)
			//{
			//	tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
			//					pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity * 2), m_dwID);
			//	if( P_VALID(pItem) )
			//	{
			//		GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

			//		// ��һ��һ�����ߵ��¼��ӿ�
			//		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
			//	}

			//	return E_Compose_Consolidate_Perfect;
			//}
			//else
			{
				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)pProduceProto->dwProQuantity, m_dwID);
				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

					// ��һ��һ�����ߵ��¼��ӿ�
					g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
				}

				return E_Compose_Consolidate_Success;
			}
		}
		else
		{
			tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)pProduceProto->dwProQuantity, m_dwID);
			if( P_VALID(pItem) )
			{
				GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

				// ��һ��һ�����ߵ��¼��ӿ�
				g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
			}


			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// �����������
		for(INT n = 0; n < nArraySz; ++n)
		{
			if(!GetMap()->IsWar())
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwFailDeductionNum);
			else
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwFailDeductionNum);
		}
		return E_Compose_Consolidate_Lose;
	}

}

//---------------------------------------------------------------------------------
// �����ϳ�װ��
// Jason 2009-12-4 ���������ͼ���ӳ�
// Jason 2010-4-2 �������Ϳ���Ϊ�գ���ʱ���ϱ�����װ��
//--------------------------------------------------------------------------------
DWORD Role::ProduceEquip(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
										INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	if( IsShieldingFormula(dwFormulaID) )
		return E_Compose_Formula_Not_Exist;
	// �ҵ��ϳɱ�����
	const tagProduceProtoSer* pProduceProto = g_attRes.GetProduceProto(dwFormulaID);

	if(!P_VALID(pProduceProto))
		return E_Compose_Formula_Not_Exist;

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckProduceLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pProduceProto, n64StuffID, nArraySz);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	BOOL bFixQlty = TRUE;
	if (pProduceProto->dwQltyFixLimit == 0)
		bFixQlty = FALSE;

	// �����������(˳��������Ʒ�ʣ�
	INT nStuffNum[EIQ_End];
	ZeroMemory(nStuffNum, sizeof(nStuffNum));
	INT nTotalNum = 0;

	// ��ָ��typeid����������
	for(INT n = 0; n < nArraySz; ++n)
	{	
		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]);
	
		if(!P_VALID(pItemStuff))
			pItemStuff = GetItemMgr().GetWarBagItem(n64StuffID[n]);

		if( !P_VALID(pItemStuff) )
			continue;

		if( EST_Null == pProduceProto->ProduceStuff[n].eStuffType ) // ָ��id���Ҫ�ж�
		{
			if( MIsEquipment(pItemStuff->dwTypeID) )
			{
				tagEquip	*pEquip = (tagEquip*)pItemStuff;
				if( P_VALID(pEquip) )
				{
					if( EIQ_White > pEquip->equipSpec.byQuality || EIQ_End <= pEquip->equipSpec.byQuality )
						return E_Compose_EquipUnidentify;
				}
			}
		}
	}

	// Jason 2010-7-12 v1.0.0 ���ӳɹ����ж�
	INT nSucPct = pProduceProto->nSuccessRate / 100;
	if( IUTIL->RandomInRange(0,100) > nSucPct )
	{
		for(INT n = 0; n < nArraySz; ++n)
		{
			INT16 deductnum = pProduceProto->ProduceStuff[n].dwFailDeductionNum;
			if( deductnum <= 0 )
				continue;
			tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]);
			if( !P_VALID(pItemStuff) )
				continue;
			if(!GetMap()->IsWar())
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, deductnum);
			else
				GetItemMgr().DelFromWarBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, deductnum);
		}
		// ��Ǯ����
		GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);
		return E_Compose_Consolidate_Lose;
	}

	for(INT n = 0; n < nArraySz; ++n)
	{	
		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]);

		if(!P_VALID(pItemStuff))
			pItemStuff = GetItemMgr().GetWarBagItem(n64StuffID[n]);

		if( !P_VALID(pItemStuff) )
			continue;

		if( EST_Null != pProduceProto->ProduceStuff[n].eStuffType ) // Jason��ֻͳ��������ȷ�Ĳ�������
		{
			nStuffNum[pItemStuff->pProtoType->byQuality] += pProduceProto->ProduceStuff[n].dwStuffNum;
			nTotalNum += pProduceProto->ProduceStuff[n].dwStuffNum;
		}
		else
		{
			if( MIsEquipment(pItemStuff->dwTypeID) )
			{
				tagEquip	*pEquip = (tagEquip*)pItemStuff;
				if( P_VALID(pEquip) )
				{
					if( EIQ_Start < pEquip->equipSpec.byQuality && EIQ_End > pEquip->equipSpec.byQuality )
					{
						nStuffNum[pEquip->equipSpec.byQuality] += pProduceProto->ProduceStuff[n].dwStuffNum;
						nTotalNum += pProduceProto->ProduceStuff[n].dwStuffNum;
					}
				}
			}
		}

		if(!GetMap()->IsWar())
			GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
		else
			GetItemMgr().DelFromWarBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
	}

	// ��Ǯ����
	GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);

	// ���Ӻϳɼ���������(ֻ�м��ܺϳɲŻ����Ӽ��������ȣ�
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pProduceProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// �۳���ҵĻ���ֵ
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_CONSOLODATE,pProduceProto->dwVitalityConsume);
	//ChangeVitality(-(INT)pProduceProto->dwVitalityConsume);
	ChangeVitality(-(INT)abs(nVitality));

	// ��úϳɵ���
	/* 1��������=-50%����Ʒ��������/����������+10%����Ʒ��������/����������+15%����Ʒ��������
				 /����������+50%����Ʒ��������/����������+100%����Ʒ��������/����������

	   2��������=5%����Ʒ��������/����������+20%����Ʒ��������/����������+30%����Ʒ��������/����������						*/
	FLOAT fModify1 = 0;
	FLOAT fModify2 = 0;
	FLOAT fModify3 = 0;
	FLOAT fPurplePct = 0;
	INT nPotAdding = 0;

	if( nTotalNum )
	{
		fModify1 = ((-0.5f * (FLOAT)nStuffNum[EIQ_White] + 0.03f * (FLOAT)nStuffNum[EIQ_Yellow]
		+ 0.15f * (FLOAT)nStuffNum[EIQ_Green] + 0.6f * (FLOAT)nStuffNum[EIQ_Blue] 
		+ 1.0f * (FLOAT)nStuffNum[EIQ_Orange]
		// 0.0f���ţ��������ܻ��
		+ 0.0f * (FLOAT)nStuffNum[EIQ_Purple]  ) / (FLOAT)nTotalNum) * 10000.0f;

		fModify2 = ( ( 0.8f * (FLOAT)nStuffNum[EIQ_Orange] + nStuffNum[EIQ_Purple] ) / (FLOAT)nTotalNum ) * 10000.0f;

		// ȥ��Ǳ��ֵӰ��
		// Ǳ��ֵ������
		// Ǳ��ֵӰ��ϵ�� = 0.7����Ʒ��������/����������+0.8����Ʒ��������/����������+0.9����Ʒ��������/����������+1.0����Ʒ��������/����������+1.1����Ʒ��������/����������
		//fModify3 = ((0.7f * (FLOAT)nStuffNum[EIQ_White] + 0.8f * (FLOAT)nStuffNum[EIQ_Yellow]
		//+ 0.9f * (FLOAT)nStuffNum[EIQ_Green] + 1.0f * (FLOAT)nStuffNum[EIQ_Blue]
		//+ 1.1f * (FLOAT)nStuffNum[EIQ_Orange] 
		//+ 1.2  * nStuffNum[EIQ_Purple] ) / (FLOAT)nTotalNum) * 10000.0f;

		fPurplePct = 0;//( 0.05 * (FLOAT)nStuffNum[EIQ_Blue] + 0.22 * (FLOAT)nStuffNum[EIQ_Orange] + nStuffNum[EIQ_Purple] ) / nTotalNum;

		// Jason 2009-12-1
		//����װ��������Ǳ��ֵ���� = 
		//��̬����ֵ + ��N��/N��*0+N��/N��*0+N��/N��*0+N��/N��*3+N��/N��*6+N��/N��*10��
		//��ֵ����ȡ��
		nPotAdding = 0;//( nStuffNum[EIQ_Blue] * 3 + nStuffNum[EIQ_Orange] * 6 + nStuffNum[EIQ_Purple] * 10 ) / nTotalNum;
	}


	FLOAT nRnd = ( (FLOAT) IUTIL->RandomInRange(0,100) ) / 100;
	


	tagItem *pItem = NULL;
	//if( fPurplePct == 0 ) // ��nTotalNum=0ʱ����ǡ��nRnd=0��ԭ���İ汾���ͻ��������װ!
		pItem = ItemCreator::Create(EICM_Product,  m_dwID, 
		pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3,nPotAdding, -1, bFixQlty);
	//if( nRnd > fPurplePct  )//�����ɫƷ����������Ϊ0��Ҳ�����ϵļ���������Jason 2009-12-23 fix
	//{
	//	pItem = ItemCreator::Create(EICM_Product,  m_dwID, 
	//		pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3,nPotAdding);
	//	if( P_VALID(pItem) && MIsEquipment(pItem->dwTypeID) )
	//	{
	//		tagEquip * pEquip = static_cast<tagEquip*>(pItem);
	//		pEquip->equipSpec.dwPurpleQltyIdfPct = fPurplePct*100;
	//	}
	//}
	//else
	//	pItem = ItemCreator::CreateByProduce(EICM_Product,  m_dwID, 
	//	pProduceProto->dwProItemTypeID,EIQ_Purple, nPotAdding ,(DWORD)(fPurplePct * 100),(INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3);

	// �ж��Ƿ�Ϊʱװ���������
	if(P_VALID(pItem) && MIsFashion(pItem))
	{
		// ����IM���ߵ�Ӱ��
		tagIMEffect			IMEffect;
		tagProduceProtoSer *pProto = const_cast<tagProduceProtoSer*>(pProduceProto);
		CalIMEffect(ECTS_Produce, IMEffect, n64IMID, pProto);

		ItemCreator::IdentifyEquip((tagEquip*)pItem, EIQ_Null, &IMEffect, bFixQlty);
	}
	
	if( P_VALID(pItem) )
	{
		GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

		// ��һ��һ�����ߵ��¼��ӿ�
		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);

		tagEquip* pEquip = (tagEquip*)pItem;
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if (P_VALID(pRoleScript))
			pRoleScript->OnProduceEquip(this, pEquip->dwTypeID, pEquip->equipSpec.byQuality);
	}

	// �ƺ��¼�����
	if (GT_VALID(dwNPCID))
		GetTitleMgr()->SigEvent(ETE_COMPOSITE_SKILL_SUCCESS, GT_INVALID, GT_INVALID);


	return E_Compose_Consolidate_Success;
}

//---------------------------------------------------------------------------------
// �㻯,װ���ֽ�
//---------------------------------------------------------------------------------
DWORD Role::DeComposeItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
								INT64 n64IMID, INT64 n64Item, DWORD dwCmdID)
{
	// �ҵ��ϳɱ�����
	const tagDeComposeProtoSer* pDeComposeProto = g_attRes.GetDeComposeProto(dwFormulaID);

	if(!P_VALID(pDeComposeProto))
		return E_Compose_Formula_Not_Exist;

	// �ҵ��������
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64Item); 

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckDeComposeLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pDeComposeProto, n64Item, pEquip);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// �㻯ֻ���ڻ�ɫ������װ�����ֽ�����Ʒ��
	if(pDeComposeProto->eComType == ECOMT_PointUp && pEquip->equipSpec.byQuality == (BYTE)EIQ_White)
		return E_Compose_Quality_Not_Match;

	if( MIsFaBao(pEquip) )
		return E_Compose_Not_Fashion;

	// ��߼�ʱװ���ɵ㻯��ֽ�
	if(pEquip->pProtoType->eType == EIT_Fashion || pEquip->pEquipProto->eEquipPos == EEP_Face)
		return E_Compose_Not_Fashion;

	// ��������װ�����ɵ㻯��ֽ�
	if(pEquip->bLock == true)
		return E_Compose_Equip_Lock;

	// ��ʱ�����Ƶ�װ�����ɵ㻯��ֽ�
	if(pEquip->pProtoType->dwTimeLimit != GT_INVALID)
		return E_Compose_Equip_Time_Limit;

	// δ������װ�����ɵ㻯��ֽ�
	if(pEquip->equipSpec.byQuality == (BYTE)EIQ_Null)
		return E_Compose_Equip_Not_identify;

#if 0 // ��F�ƻ����㻯�������Ľ�Ǯ
	// ��Ǯ����
	GetCurMgr().DecBagSilver(pDeComposeProto->dwMoneyConsume, ELCID_Compose_Decompose);
#endif

	// ���Ӻϳɼ���������(ֻ�м��ܺϳɲŻ����Ӽ��������ȣ�
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pDeComposeProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// �۳���ҵĻ���ֵ
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pDeComposeProto->dwVitalityConsume);
	//ChangeVitality(-(INT)pDeComposeProto->dwVitalityConsume);
	ChangeVitality(-(INT)abs(nVitality));

	BOOL	bSuccess = FALSE;
#if 0 // ��F�ƻ����㻯���ٽ����������ж�
	// �����Ƿ�����
	// ������Ǳ��ֵ > Ǳ��ֵ����/5 
	// ����������������ʱ�����������ʵĹ�ʽΪ��������=1%+��ɫ��Ե/1000)
	BOOL	bPerfect = FALSE;
	if( pEquip->equipSpec.nPotValUsed > pEquip->pEquipProto->nMaxPotVal / 5)
		bPerfect = TRUE;
	else
	{
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;
		bPerfect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);
	}
#endif
	// �����������
	tagItem *pItem = GetItemMgr().GetBag().GetItem(n64Item);
	if(P_VALID(pItem))
	{
		const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
		if(P_VALID(pScript) && P_VALID(pItem->pProtoType) )
		{
			pScript->DelItem( 1, pItem->dwTypeID, pItem->pProtoType->byQuality );
		}
	}

	GetItemMgr().DelFromBag(n64Item, (DWORD)ELCID_Compose_Decompose, (INT16)pEquip->GetNum());


	for(INT i = 0; i < MAX_OUTPUT_QUANTITY; ++i)
	{
		if(pDeComposeProto->OutputStuff[i].dwStuffTypeID == GT_INVALID)
			break;

		// ����ɹ���
		BOOL bResult = IUTIL->Rand() % 10000 <= (pDeComposeProto->OutputStuff[i].nRate);
		
		if(bResult)
		{
			bSuccess = TRUE;

#if 0 // ��F�ƻ����㻯���ٽ����������ж�

			// ����
			if(bPerfect)
			{
				INT nStuffNum = IUTIL->Rand() % (pDeComposeProto->OutputStuff[i].nPerMaxVal + 1);

				if(nStuffNum < pDeComposeProto->OutputStuff[i].nPerMinVal)
					nStuffNum = pDeComposeProto->OutputStuff[i].nPerMinVal;

				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
					pDeComposeProto->OutputStuff[i].dwStuffTypeID, (INT16)nStuffNum, m_dwID);

				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}
			}
			else	// �ɹ�
#endif
			{
				INT nStuffNum = IUTIL->Rand() % (pDeComposeProto->OutputStuff[i].nSucMaxVal + 1);

				if(nStuffNum < pDeComposeProto->OutputStuff[i].nSucMinVal)
					nStuffNum = pDeComposeProto->OutputStuff[i].nSucMinVal;

				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
					pDeComposeProto->OutputStuff[i].dwStuffTypeID, (INT16)nStuffNum, m_dwID);

				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}
			}
		}		
	}

	if(bSuccess)
	{
#if 0 // ��F�ƻ����㻯���ٽ����������ж�
		if(bPerfect)
			return E_Compose_Consolidate_Perfect;
		else	
#endif
			return E_Compose_Consolidate_Success;
	}
	else
		return E_Compose_Consolidate_Lose;
}

//---------------------------------------------------------------------------------
// ��������ϳ�ʱ������
//---------------------------------------------------------------------------------
DWORD Role::CheckProduceLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
								const tagProduceProtoSer* &pProduceProto, INT64 n64StuffID[], INT32 nArraySz)
{
	// ���ܺϳɷ�ʽ
	if(( pProduceProto->eFormFrom == EFormulaFrom_Skill || pProduceProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pProduceProto->eFormFrom == EFormulaFrom_SkillorNPC) && GT_INVALID != dwSkillID)
	{
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill))
			return E_Compose_Skill_Not_Exist;

		// ��⼼�������Ƿ�ƥ��
		if(Skill2ProduceType(pSkill->GetTypeEx2()) != pProduceProto->eProType)
			return E_Compose_FormulaNotMatch;

		// ��⼼�ܵȼ����䷽�ȼ��Ƿ�ƥ��
		if(pSkill->GetLevel() < pProduceProto->nFormLvl)
			return E_Compose_FormulaNotMatch;		
	}
	// ��Ʒ�ϳɷ�ʽ
	else if(( pProduceProto->eFormFrom == EFormulaFrom_Item || pProduceProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pProduceProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != n64ItemID)
	{
		// ��Ʒ�Ƿ��ڱ�����

		// �Ƿ�Ϊ�ϳ�����Ʒ
	}
	// NPC�ϳɷ�ʽ
	else if(( pProduceProto->eFormFrom == EFormulaFrom_NPC || pProduceProto->eFormFrom == EFormulaFrom_SkillorNPC
		|| pProduceProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != dwNPCID)
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC)) 
			return E_Compose_NPC_Not_Exist;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_Compose_NPC_Distance;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Compose) )
			return E_Compose_NPCCanNotCompose;

		// ��Ҫ���NPC�Ƿ���кϳ��䷽
	}

	// ����ɫ��������				
	ItemMgr& itemMgr = GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_Compose_Bag_Full;

	// ��һ���ֵ�Ƿ��㹻
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pProduceProto->dwVitalityConsume);

	//if(GetAttValue(ERA_Vitality) < (INT)pProduceProto->dwVitalityConsume)
	//	return E_Compose_Vitality_Inadequate;
	
	if(GetAttValue(ERA_Vitality) < (nVitality))
		return E_Compose_Vitality_Inadequate;


	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pProduceProto->dwMoneyConsume)
		return E_Compose_NotEnough_Money;

	// �����Ҳ����Ƿ��㹻
	for(INT n = 0; n < MAX_PRODUCE_STUFF_QUANTITY; ++n)
	{
		if(pProduceProto->ProduceStuff[n].dwStuffID == GT_INVALID 
			&& pProduceProto->ProduceStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 
		if(!P_VALID(pItemStuff))
			pItemStuff = GetItemMgr().GetWarBagItem(n64StuffID[n]);

		if(!P_VALID(pItemStuff))
			return E_Compose_Stuff_Not_Enough;

		// ������64λID�Ƿ��ظ�
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Compose_Stuff_Not_Enough;
		}

		if(pProduceProto->ProduceStuff[n].dwStuffID == GT_INVALID )
		{
			if(pItemStuff->pProtoType->eStuffType != pProduceProto->ProduceStuff[n].eStuffType)
				return E_Compose_Stuff_Not_Enough;
		}
		else
		{
			if(pItemStuff->pProtoType->dwTypeID != pProduceProto->ProduceStuff[n].dwStuffID)
				return E_Compose_Stuff_Not_Enough;
		}

		if(pItemStuff->GetNum() < (INT)pProduceProto->ProduceStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;
	}

	return E_Success;
}

//---------------------------------------------------------------------------------
// ��������ϳ�ʱ������
//---------------------------------------------------------------------------------
DWORD Role::CheckDeComposeLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
						 const tagDeComposeProtoSer* &pDeComposeProto, INT64 n64Item, tagEquip *pEquip)
{
	// ���ܺϳɷ�ʽ
	if(( pDeComposeProto->eFormFrom == EFormulaFrom_Skill || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pDeComposeProto->eFormFrom == EFormulaFrom_SkillorNPC) && GT_INVALID != dwSkillID)
	{
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill))
			return E_Compose_Skill_Not_Exist;

		// ��⼼�������Ƿ�ƥ��
		if(pSkill->GetTypeEx2() != ESSTE2_PointUp)
			return E_Compose_FormulaNotMatch;

		// ��⼼�ܵȼ����䷽�ȼ��Ƿ�ƥ��
		if(pSkill->GetLevel() < pDeComposeProto->nFormLvl)
			return E_Compose_FormulaNotMatch;		
	}
	// ��Ʒ�ϳɷ�ʽ
	else if(( pDeComposeProto->eFormFrom == EFormulaFrom_NPC || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorNPC
			|| pDeComposeProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != n64ItemID )
	{
		// ��Ʒ�Ƿ��ڱ�����

		// �Ƿ�Ϊ�ϳ�����Ʒ
	}
	// NPC�ϳɷ�ʽ
	else if( ( pDeComposeProto->eFormFrom == EFormulaFrom_Item || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorItem
			|| pDeComposeProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != dwNPCID )
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC)) 
			return E_Compose_NPC_Not_Exist;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_Compose_NPC_Distance;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Compose) )
			return E_Compose_NPCCanNotCompose;

		// ��Ҫ���NPC�Ƿ���кϳ��䷽
	}

	// ����ɫ��������				
	ItemMgr& itemMgr = GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_Compose_Bag_Full;

	// ��һ���ֵ�Ƿ��㹻
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pDeComposeProto->dwVitalityConsume);

	//if(GetAttValue(ERA_Vitality) < (INT)pDeComposeProto->dwVitalityConsume)
	//	return E_Compose_Vitality_Inadequate;

	if(GetAttValue(ERA_Vitality) < (nVitality))
		return E_Compose_Vitality_Inadequate;

	if(!P_VALID(pEquip))
		return E_Compose_Stuff_Not_Enough;

	// �����ҽ�Ǯ�Ƿ��㹻
	if(GetCurMgr().GetBagSilver() < pDeComposeProto->dwMoneyConsume)
		return E_Compose_NotEnough_Money;

	// ���װ���ȼ��Ƿ�����
	if((pEquip->pProtoType->byLevel / 10) < pDeComposeProto->byLevel)
		return E_Compose_Equip_Level_Inadequate;

	// ���װ��Ʒ���Ƿ�����Ҫ��
	if(pEquip->equipSpec.byQuality < pDeComposeProto->byQuality)
		return E_Compose_Equip_Quality_Inadequate;

	// ������������Ƿ������䷽Ҫ��
	if(pEquip->pProtoType->eType == EIT_Weapon && pEquip->pProtoType->eTypeEx != pDeComposeProto->eTypeEx)
		return E_Compose_Equip_Type_Inadequate;

	// ���װ���Ƿ������䷽Ҫ��
	if((pEquip->pProtoType->eType == EIT_ClothArmor 
		|| pEquip->pProtoType->eType == EIT_Decoration 
		|| pEquip->pProtoType->eType == EIT_Armor) 
		&& pEquip->pEquipProto->eEquipPos != pDeComposeProto->ePos)

		return E_Compose_Equip_Type_Inadequate;

	return E_Success;
}

//---------------------------------------------------------------------------------
// ������������ת��Ϊ��������
//---------------------------------------------------------------------------------
EProduceType Role::Skill2ProduceType(ESkillTypeEx2 eSkillType)					 
{													 				 
	switch(eSkillType)								 
	{												 
	case ESSTE2_Smilt:					return EPCT_Smilt;							 
	case ESSTE2_Artisan:				return EPCT_Artisan;						 
	case ESSTE2_DanTraining:			return EPCT_DanTraining;					 
	case ESSTE2_Smith:					return EPCT_Smith;							 
	case ESSTE2_Casting:				return EPCT_Casting;						 
	case ESSTE2_Dressmaker:				return EPCT_Dressmaker;						 
	case ESSTE2_Aechnics:				return EPCT_Aechnics;
	default:							return EPCT_NULL;
	}												 
}

//---------------------------------------------------------------------------------
// ������Ʒ���Ƿ���Ҫ�������ߣ�
//---------------------------------------------------------------------------------
tagItem* Role::CreateItem(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num, DWORD dwCreator)
{
	tagItemProto *pProto = g_attRes.GetItemProto(dwTypeID);
	if(!P_VALID(pProto))
		return (tagItem*)GT_INVALID;

	if(pProto->n16MaxLapNum > 1)
		return ItemCreator::Create(EICM_Product,  m_dwID, dwTypeID, n16Num);
	else
		return ItemCreator::Create(EICM_Product,  m_dwID, dwTypeID, n16Num, m_dwID);		
}

//---------------------------------------------------------------------------------
// ����IM��Ʒ��Ӱ��
//---------------------------------------------------------------------------------
VOID Role::CalIMEffect(EConsolidateTypeSer eConType, tagIMEffect &IMEffect, INT64 n64IMID, const LPVOID pProto)
{

	// �������Ƿ�������߳ɹ��ʵ�IM
	if( GT_INVALID == n64IMID)
		return;

	tagItem *pItemIM = GetItemMgr().GetBagItem(n64IMID); 
	if(!P_VALID(pItemIM))
		return;

	switch (eConType)
	{
	case ECTS_Posy:
		{
			const tagPosyProtoSer*  pPosyProto = (tagPosyProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
			   if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Posy 
				  || pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
				  || pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
			   {
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Posy, 1);
			   }
			   break;
			}

			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Posy, 1);
			}
		}
		break;
	case ECTS_Engrave:
		{
			const tagEngraveProtoSer* pEngraveProto = (tagEngraveProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Engrave 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Engrave, 1);
				}
				break;
			}

			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Engrave, 1);
			}
		}
		break;
	case ECTS_Inlay:
		{

		}
		break;
	case ECTS_Brand:
		{
			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Brand, 1);
			}
		}
		break;
	case ECTS_LoongSoul:
		{

		}
		break;
	case ECTS_Quench:
		{
			const tagQuenchProtoSer *pQuenchProto  = (tagQuenchProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Quench 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Quench, 1);
				}
				break;
			}
		}
		break;
	case ECTS_Produce:
		{
			const tagProduceProtoSer*  pProduceProto = (tagProduceProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)pProduceProto->eComType 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)ESFCA_AllProduce
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// ɾ��IM����
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Produce, 1);
				}
				break;
			}
			else if (pItemIM->pProtoType->eSpecFunc == EISF_ColorProbability)
			{
				IMEffect.eEffect = EIME_Color;
				IMEffect.dwParam1 = pItemIM->pProtoType->nSpecFuncVal1;
				IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

				// ɾ��IM����
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Produce, 1);
			}
		}
		break;
	case ECTS_Decompose:
		{

		}
		break;
	case ECTS_RaisePotVal:
		{
			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign/*EISF_EquipAddPotValue*/)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// ɾ��IM����
			}
			GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Raise_PotVal, 1);
		}
		break;
	}
}


//---------------------------------------------------------------------------------
// ����װ������
//---------------------------------------------------------------------------------
VOID Role::CalEquipFlare(tagEquip* pEquip)
{
	BYTE byFlareVal = 0;
	BYTE byHoleBNum = 0;
	// ������Ƕ����
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
			continue;
		else
			byHoleBNum++;
	}

	// ����ֵ = 2��װ��ǿ���ȼ� + 4����Ƕ���� + 2��׷�ӵȼ�
	byFlareVal = pEquip->equipSpec.byConsolidateLevel*2 + byHoleBNum*4 + pEquip->equipSpec.byBrandLevel*2;

	pEquip->SetFlareVal(byFlareVal);
}



DWORD Role::RaiseEquipPotVal(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD & dwPotValAdded)
{
	dwPotValAdded = 0;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// �ҵ�������װ��
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;
	else if( !( EIT_Weapon		== pEquip->pEquipProto->eType ||
				EIT_ClothArmor	== pEquip->pEquipProto->eType ||
				EIT_Decoration	== pEquip->pEquipProto->eType ||
				EIT_Armor		== pEquip->pEquipProto->eType) )
		return E_Consolidate_NotEquipment;

	// �ҵ�Ǳ����������
	tagItem *pItemRaiser = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemRaiser) || pItemRaiser->pProtoType->eSpecFunc != EISF_EquipAddPotValue )
		return E_Compose_Stuff_Not_Exist/*E_Consolidate_Brand_Not_Exit*/;

	// �ҵ�ǿ�����ݣ��������Ҫ
	//const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemRaiser->dwTypeID);
	//if(!P_VALID(pConsolidateProto))
	//	return E_Compose_Formula_Not_Exist;

	// ����������������
	if( 0 == pEquip->equipSpec.nPotIncTimes )
		return E_Raise_PotVal_Times_Zero;

	// ���Ǳ���������ޣ���Ҫ��ԭ����������ֵ
	INT32 nMaxPotVal = pEquip->pEquipProto->nMaxPotVal/*pEquip->equipSpec.nPotVal + pEquip->equipSpec.nPotValUsed*/;
	if( pEquip->equipSpec.nPotVal >= nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ )
		return E_Raise_PotVal_Not_Needing;

	INT nPotDelta = pItemRaiser->pProtoType->nSpecFuncVal2;

	// ���տ�����װ������Ǳ��
	if((pEquip->equipSpec.nPotVal + nPotDelta ) > nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ )
		nPotDelta = nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ - pEquip->equipSpec.nPotVal;

	// ����ɹ���
	FLOAT fProp = /*(*/ pItemRaiser->pProtoType->nSpecFuncVal1 + (FLOAT)GetAttValue(ERA_Fortune) /*)*/ / 1000.0f;

	// ɾ������
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Raise_PotVal, 1);

	// ����IM���ߵ�Ӱ��
	tagIMEffect			IMEffect;
	IMEffect.eEffect	= EIME_Null;
	//tagConsolidateItem *pProto = const_cast<tagConsolidateItem*>(pConsolidateProto);		
	CalIMEffect(ECTS_RaisePotVal, IMEffect, n64IMID, NULL);

	// ����ɹ���
	INT rand_n = IUTIL->Rand();
	BOOL bResult = /*IUTIL->RandomInRange(0,100) <= fProp;*/rand_n  % 10000 <= fProp;

	if(bResult)		// �ɹ�
	{
		// �ݼ�����Ǳ��ֵ����
		--pEquip->equipSpec.nPotIncTimes;
		if( pEquip->equipSpec.nPotIncTimes < 0 )
			pEquip->equipSpec.nPotIncTimes = 0;

		// �����Ƿ�����������������=�����ɹ��ʡ�1/20+��ɫ��Ե/1000)
		FLOAT fPefectProp = 0.01 + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		rand_n = IUTIL->Rand();
		BOOL bPefect = rand_n % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			dwPotValAdded = nPotDelta*2;//pEquip->equipSpec.nPotVal;
			pEquip->ChangePotVal( dwPotValAdded );
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			dwPotValAdded = nPotDelta;
			pEquip->ChangePotVal( dwPotValAdded );
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// װ��Ǳ��ֵ����100��
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal( -100 );

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}

	return 0;
}
// Jason Gem removal info v1.3.1 2009-12-21
DWORD Role::GetGemRemovalInfo(DWORD dwNPCID,INT64 equipID,INT8 & num,DWORD dwGemIDs[MAX_EQUIPHOLE_NUM])
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GemRemoval) )
		return E_Gem_NPCCanNotRemoveGem;



	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(equipID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	num = pEquip->equipSpec.byHoleNum;
	if(num > MAX_EQUIPHOLE_NUM)
		num = MAX_EQUIPHOLE_NUM;
	else if( num < 0 )
		num = 0;

	memset(dwGemIDs,0,sizeof(dwGemIDs));

	for(int i = 0; i < num ; ++i)
		dwGemIDs[i] = pEquip->equipSpec.dwHoleGemID[i];

	if( 0 == num )
		return E_Consolidate_Gem_Not_Exit;

	return E_Success;
}

namespace jm_Gemtool
{
struct tagTem
{
	INT64 sid;
	int count;
	int index[MAX_EQUIPHOLE_NUM];
} ;
struct tagTems
{
	tagTem data[MAX_EQUIPHOLE_NUM];
	tagTems()
	{
		memset(data,0,sizeof(data));
	}
	void add(INT64 id, int index)
	{
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if(data[i].sid == 0)
			{
				data[i].sid = id;
				data[i].count = 1;
				data[i].index[data[i].count - 1] = index;
				break;
			}
			else if(data[i].sid == id)
			{
				data[i].count ++;
				data[i].index[data[i].count - 1] = index;
				break;
			}
		}
	}
	int countNoZero()
	{
		int count = 0;
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if(data[i].sid > 0)
				++count;
		}
		return count;
	}
	int countOneSid(INT64 id)
	{
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if( id == data[i].sid )
			{
				return data[i].count;
			}
		}
		return 0;
	}
	int decreaseOneSid(INT64 id)
	{
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if( id == data[i].sid )
			{
				if( data[i].count > 1 )
					data[i].count--;
				return data[i].count;
			}
		}
		return 0;
	}
	int decreaseOneSid(INT64 id,int num,INT64* pOuter)
	{
		if( !pOuter )
			return 0;
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if( id == data[i].sid )
			{
				for(int a = num ; a < data[i].count; ++a )
					pOuter[data[i].index[a]] = 0;
				return data[i].count;
			}
		}
		return 0;
	}
};
}


DWORD Role::RemoveGemFromEquip(DWORD dwNPCID,INT64 equipID,INT8 num,INT64 SignIDs[MAX_EQUIPHOLE_NUM])
{
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}


	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;
	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GemRemoval) )
		return E_Gem_NPCCanNotRemoveGem;

	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(equipID);
	if(!P_VALID(pEquip))
		return E_Compose_Stuff_Not_Exist;

	INT8 n8Num = pEquip->equipSpec.byHoleNum;


	tagItem* pItems[MAX_EQUIPHOLE_NUM] = {0};
	INT64 signs[MAX_EQUIPHOLE_NUM] = {0};
	DWORD gems[MAX_EQUIPHOLE_NUM] = {0};
	int count = 0;

	jm_Gemtool::tagTems temSigns;
	for(int i = 0; i < n8Num ; ++i)
		temSigns.add(SignIDs[i],i);

	for(int i = 0; i < n8Num; ++i)
	{
		if( SignIDs[i] == 0 || GT_INVALID == SignIDs[i] )
			continue;

		tagItem * pItem = GetItemMgr().GetBagItem(SignIDs[i]);
		if(!P_VALID(pItem))
		{
			SignIDs[i] = 0;
			continue;
		}

		INT count = temSigns.countOneSid  (SignIDs[i]);
		if( count > 0 )
		{
			if(pItem->GetNum() < count)
			{
				temSigns.decreaseOneSid(SignIDs[i],pItem->GetNum(),SignIDs);
			}
		}
	}


	for(int i = 0,j = 0; i < n8Num ; ++i)
	{
		if( SignIDs[i] == 0 || GT_INVALID == SignIDs[i] )
			continue;

		if( !P_VALID( GetItemMgr().GetBagItem(SignIDs[i]) ) )
			continue;

		tagItemProto * pItemProto = g_attRes.GetItemProto( pEquip->equipSpec.dwHoleGemID[i] );
		tagItem *pItemGemSign = GetItemMgr().GetBagItem(SignIDs[i]);
		if( P_VALID(pItemProto) && P_VALID(pItemGemSign) && pItemGemSign->pProtoType->eSpecFunc == EISF_GemRemovalSign )
		{
			//if( pItemProto->byQuality == pItemGemSign->pProtoType->byQuality )
			if( pItemProto->byLevel == pItemGemSign->pProtoType->byLevel )
			{
				signs[i] = SignIDs[i];
				gems[i] = pItemProto->dwTypeID;
				++count;
			}
		}
	}

	//for( int i = 0; i < count; ++i )
	//{
	//	if( ( MIsEquipment(gems[i]) && ( GetItemMgr().GetBagFreeSize() < count ) ) || 
	//		( !MIsEquipment(pCur->dwRewardItemTypeID) && ( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE || GetItemMgr().GetBagFreeSize() < 1 )
	//		( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE  || 
	//}
																																											  
	//if( GetItemMgr  ().GetBag().GetFreeSpaceSize() < count )
	//	return E_Compose_Bag_Full;

	if( count > 0 )
	{
		if( GetItemMgr().GetBagFreeSize() < count )
			return E_Compose_Bag_Full;

		INT64 totalSilver = count * 10 * 10000;
		if( GetCurMgr  ().GetBagSilver() >= totalSilver )
		{
			GetCurMgr().DecBagSilver(totalSilver, ELCID_GemRemoval);
			// ��ʯ�����ɾ��װ��
			//GetItemMgr  ().DelFromBag  (equipID,ELCID_GemRemoval,1);
			int a = 0;
			for (int i = 0 ; i < n8Num ; ++i)
			{
				if( signs[i] == 0 || GT_INVALID == signs[i] )
					continue;

				bool bFinded = false;
				if( P_VALID(pItems[a-1]) )
				{
					for( int j = 0 ; j < MAX_EQUIPHOLE_NUM; ++j )
					{
						if( !P_VALID(pEquip->equipSpec.dwHoleGemID[j]) )
							continue;
						if( pEquip->equipSpec.dwHoleGemID[j] == gems[i] && j == i )
						{
							pEquip->equipSpec.dwHoleGemID[j] = GT_INVALID;
							//if(pEquip->equipSpec.byHoleNum > MAX_EQUIPHOLE_NUM)
							//	pEquip->equipSpec.byHoleNum = MAX_EQUIPHOLE_NUM;
							//else if(pEquip->equipSpec.byHoleNum > 0)
							//	pEquip->equipSpec.byHoleNum--;
							bFinded = true;
							break;
						}
					}
				}
				//else
				if( bFinded )
				{
					pItems[a++] = ItemCreator::Create  (EICM_GemRemoval,GetID  (),gems[i]);
					GetItemMgr  ().DelFromBag  (signs[i],ELCID_GemRemoval,1);
				}
			}
			
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

			// װ����û�а󶨵ı�ʯ�Ͱ󶨸���ʯʱ����ԭ����װ������Ĭ�ϵİ�״̬
			if (!bHasBindGemItem && !pEquip->IsAttachSoulBind())
			{
				pEquip->Bind();
				// �ı�װ���İ�״̬��֪ͨDB�Ϳͻ���
				pEquip->SetUpdate(EUDBS_Update);
				GetItemMgr().SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);
			}

			// ���¼������ֵ
			CalEquipFlare(pEquip);

			GetItemMgr  ().UpdateEquipSpec(*pEquip);
			
			if( count != a )//Ӧ�ò��ᷢ���������
				count = a;

			GetItemMgr().Add2Bag  (pItems,count,ELCID_GemRemoval,TRUE);
		}
		else
			return E_Compose_NotEnough_Money;
	}
	else
		return E_Compose_Stuff_Not_Exist;

	return E_Compose_Consolidate_Success;
}

/**
 * ��������
 * \author Jason
 * \date 2010-4-15
 * \version v2.0.0
 * \param srcItemID 
 * \param num 
 * \param fabaoID 
 * \return 
 */
DWORD	Role::PracticeFabao(INT64 srcItemID,INT num,INT64 fabaoID,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagItem * pItem = GetItemMgr().GetBagItem(srcItemID);
	tagItem * pDstItem = GetItemMgr().GetEquipBarEquip(fabaoID);
	if( !P_VALID(pItem) )
		return E_Fabao_Practice_NoYuLingshi;
	if( EISF_YuLingZhu != pItem->pProtoType->eSpecFunc )
		return E_Fabao_Practice_NotYuLingshi;

	if( !P_VALID(pDstItem) )
		return E_Fabao_FabaoNotExist;
	if( !MIsFaBao(pDstItem) )
		return E_Fabao_Practice_NotFabao;

	INT nMaxFabaoStage = g_attRes.GetFabaoMaxStage();
	nMaxFabaoStage = min(nMaxFabaoStage,g_world.GetFabaoLevelLimit());

	tagFabao * pFabao = (tagFabao*)pDstItem;
	const tagFabaoLevelProto * pFabaoLevelProto = NULL,*pFabaoLevelProto0 = NULL;
	pFabaoLevelProto0 = g_attRes.GetFabaoLevelProto(pFabao->nSlotNum * 10 + 1);
	pFabaoLevelProto = g_attRes.GetFabaoLevelProto(nMaxFabaoStage);
	if( !P_VALID(pFabaoLevelProto) )
		return E_Fabao_Practice_NoResource;
	// �ܵ�ֵ
	INT nMaxValue = pFabaoLevelProto->nMinValue + pFabaoLevelProto->nLevelUpWhated;

	if( g_world.GetFabaoLevelLimit() <= pFabao->n16Stage )
		return E_Fabao_Practice_FabaoStageFull;
	if( pFabao->nPracticeValue >= nMaxValue )
		return E_Fabao_Practice_FabaoStageFull;

	if( num > pItem->GetNum() )
		num = pItem->GetNum();

	if( num > pItem->pProtoType->n16MaxLapNum )
		return E_Fabao_Practice_LingshiNumCrossBorder;
	// nMaxValue����˵�ǰ������Ҫ��ֵ
	//if( P_VALID(pFabaoLevelProto0) )
	//	nMaxValue = pFabaoLevelProto0->nMinValue ;//+ pFabaoLevelProto0->nLevelUpWhated;

	INT addValue = pItem->pProtoType->nSpecFuncVal1 * num;
	BOOL change = FALSE;
	INT nLastLevel = pFabao->n16Stage;

	if( pFabao->nPracticeValue + addValue >= nMaxValue )
	{
		tagFabaoLevelProto * p = NULL;
		//if( 40 >= pFabao->n16Stage )
		//{
		//	p = const_cast<tagFabaoLevelProto *>(g_attRes.GetFabaoLevelProto(pFabao->nSlotNum * 10 + 1));
		//}
		//else
		{
			p = const_cast<tagFabaoLevelProto *>(g_attRes.GetFabaoLevelProto(nMaxFabaoStage));
		}
		if( P_VALID(p) && pItem->pProtoType->nSpecFuncVal1 > 0 )
		{
			if( g_world.GetFabaoLevelLimit() < p->nID )
				return E_Compose_Consolidate_Lose;
		//INT temNum = ( p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue - 1 ) / pItem->pProtoType->nSpecFuncVal1;
			//if( temNum > num )
			//	temNum = num;
			//if( temNum > 0 )
			//{
			//	addValue = pItem->pProtoType->nSpecFuncVal1 * temNum;
			//	num = temNum;
			//	change = TRUE;
			//	//for( int j = 0; j < pFabao->nSlotNum; ++j )
			//	//	if( !P_VALID(pFabao->dwWuxingBeadID[j]) )
			//	//		return E_Fabao_LingzhuWanted;
			//}
			/*else*/ if( p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue >= 0 )
			{
				addValue = p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue;
				num = addValue % pItem->pProtoType->nSpecFuncVal1 > 0 ? addValue / pItem->pProtoType->nSpecFuncVal1 + 1 : addValue / pItem->pProtoType->nSpecFuncVal1 ;
			}
			else
				return E_Compose_Consolidate_Lose;
		}
		else
			return E_Compose_Consolidate_Lose;
	}

	// �������������жϣ�Ҫ������
	INT temVal = pFabao->nPracticeValue + addValue;
	for( int i = pFabao->n16Stage; i <= nMaxFabaoStage; ++i )
	{
		const tagFabaoLevelProto * p = g_attRes.GetFabaoLevelProto(i);
		if( P_VALID(p) )
		{
			if( i > 59 && (i%10 == 0) )
			{
				if( i != pFabao->n16Stage )
				{
					if( temVal >= p->nMinValue && temVal >= p->nMinValue + p->nLevelUpWhated )
					{
						if( p->nMinUseLevel > GetLevel() )
							return E_Fabao_Level_Limitted;
						addValue = p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue - 1;
						num = addValue % pItem->pProtoType->nSpecFuncVal1 > 0 ? addValue / pItem->pProtoType->nSpecFuncVal1 + 1 : addValue / pItem->pProtoType->nSpecFuncVal1 ;
						break;
					}
				}
			}
			if( temVal >= p->nMinValue && temVal < p->nMinValue + p->nLevelUpWhated )
			{
				if( p->nMinUseLevel > GetLevel() )
					return E_Fabao_Level_Limitted;
				break;
			}
		}
	}

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
	GetItemMgr().DelFromBag(srcItemID,ELCID_Fabao_RemoveYuLingzhu,num);

	INT nOldZhiZhi = pFabao->n16NativeIntelligence;

	pFabao->nPracticeValue += addValue;
	ItemCreator::RecalFabaoAtt(pFabao);

	if( nLastLevel != pFabao->n16Stage )
		change = TRUE;

	if( change )
	{
		//memcpy(&fabaoSpec,(tagFabaoSpec*)pFabao,sizeof(tagFabaoSpec));
		//tagNS_FairyLevelChange fabao_levelup_msg;
		//fabao_levelup_msg.dwRoleID		= GetID();
		//fabao_levelup_msg.nLevel		= pFabao->n16Stage;
		//SendMessage(&fabao_levelup_msg,fabao_levelup_msg.dwSize);
		if( nLastLevel / 10 != pFabao->n16Stage / 10 )
			SynRemoteFabaoInfo(TRUE);
		if(g_world.GetFabaoElemStuntFlag() && pFabao->n16Stage > 59)
		{
			INT nTemLast = nLastLevel;
			if( nTemLast < 60 )
				nTemLast = 59;
			pFabao->wDamageUpgradeTimes		+= pFabao->n16Stage - nTemLast;
			pFabao->wResistanceUpgradeTimes	+= pFabao->n16Stage - nTemLast;
			if( nLastLevel < 60 )
			{
				pFabao->dwFairySkill[0] = 0;
				pFabao->dwFairySkill[1] = 0;
			}
			//if( nOldZhiZhi != pFabao->n16NativeIntelligence )
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
				if( nLastLevel < 60 )
				{
					if( P_VALID( ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID ) )
					{
						//for( int nSkillN = 0; nSkillN < MaxFairySkillNum; ++nSkillN )
							if( pFabao->dwFairySkill[0] == 0 )
								pFabao->dwFairySkill[0] = ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID;
					}
				}
			}
			if( pFabao->n16Stage % 10 == 0  || (nLastLevel / 10 != pFabao->n16Stage / 10) )
				GetItemMgr().UpdateFabaoNirVanaData2DB(*pFabao);
			ItemCreator::RealizeFabaoStunt(pFabao,nLastLevel);
		}
		// ������Դ�������
		pFabao->IncBirthTimes(nLastLevel);
		if( P_VALID(m_pScript) )
			m_pScript->OnFabaoAttChange(this,pFabao,RFAE_LevelUp,pFabao->n16Stage);
	}
	ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();

	GetItemMgr().UpdateFabaoSpec(*pFabao);

	if( change )
	{
		tagNS_FairyLevelChange fabao_levelup_msg;
		fabao_levelup_msg.dwRoleID		= GetID();
		fabao_levelup_msg.nLevel		= pFabao->n16Stage;
		SendMessage(&fabao_levelup_msg,fabao_levelup_msg.dwSize);
	}

	return E_Success;
}
// ��Ƕ��������
DWORD	Role::InlayFabao(INT64 n64FabaoID,INT num,INT64 * pBeadIDs,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagItem * pDstItem = GetItemMgr().GetEquipBarEquip(n64FabaoID);
	if( !P_VALID(pDstItem) )
	{
		return E_Fabao_FabaoNotExist;
	}
	tagFabao * pFabao = (tagFabao*)pDstItem;
	INT nSlotnums = ItemCreator::CalFabaoSlotNum(pFabao->n16Stage );
	if(nSlotnums > MaxWuxingSlotNum)
		nSlotnums = MaxWuxingSlotNum;
	if( pFabao->nSlotNum != nSlotnums )
	{
		pFabao->nSlotNum = nSlotnums;
	}
	if( pFabao->nSlotNum < num )
		return E_Fabao_Inlay_SlotLTLingzhu;

	if( !P_VALID(pBeadIDs) )
		return E_Fabao_Inlay_NotLingzhu;

	const tagFabaoProto * pFabaoProto = g_attRes.GetFabaoProto(pFabao->dwTypeID);
	if( !P_VALID(pFabaoProto) )
		return E_Fabao_UnkownError;

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	pFabao->bySlotLingzhuMatch = 0;

	BOOL re = E_Success;

	for( int i = 0 ; i < num ; ++i )
	{
		tagItem * pItem = GetItemMgr().GetBagItem(pBeadIDs[0]);
		if( P_VALID(pItem) )
		{
			if( EISF_LingZhu == pItem->pProtoType->eSpecFunc )
			{
				if( pItem->pProtoType->nSpecFuncVal1 > pFabao->n16Stage || ( pItem->pProtoType->nSpecFuncVal2 != EFB_NULL && pItem->pProtoType->nSpecFuncVal2 != ((tagFabaoProto*)pFabao->pProtoType)->eFarbaoType) )
				{
					re = E_Fabao_Inlay_Mismatch;
					break;
				}
				const tagConsolidateItem* p = g_attRes.GetConsolidateProto(pItem->dwTypeID);
				if( P_VALID(p) )
				{
					int ndx = ItemCreator::RandomFabaoWuxingHole(pFabao);
					if( ndx == GT_INVALID )
					{
						re = E_Fabao_UnkownError;
						break;
					}

					pFabao->bySlotMacthNdx[ndx] = 0;
					if( p->eWuXing == pFabao->eWuxingHole[ndx] ) //���ƥ��
					{
						pFabao->bySlotMacthNdx[ndx] = 1;
					}
					//if( !pFabao->IsBind() )
					//	pFabao->Bind();
					pFabao->dwWuxingBeadID[ndx] = pItem->dwTypeID;
				}
				GetItemMgr().DelFromBag(pBeadIDs[i],ELCID_Fabao_RemoveLingzhu,1);
			}
		}
	}
	for(int i = 0; i < pFabao->nSlotNum; ++i)
		if( pFabao->bySlotMacthNdx[i] )
			pFabao->bySlotLingzhuMatch++;
	ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();
	GetItemMgr().UpdateFabaoSpec(*pFabao);
	return re;
}

// ����������ת
DWORD	Role::ReverseFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagEquip * pMainFabao0 = GetItemMgr().GetEquipBarEquip(dstFabaoID);
	tagItem * pSecFabao0	= GetItemMgr().GetBagItem(srcFabaoID);
	if( !P_VALID(pMainFabao0) || !P_VALID(pSecFabao0) )
		return E_Fabao_Not_Exist;

	if( !MIsFaBao(pMainFabao0) && !MIsFaBao(pSecFabao0) )
		return E_Fabao_TypeMismatch;

	tagFabao * pMainFabao = (tagFabao*)pMainFabao0;
	tagFabao * pSecFabao	= (tagFabao*)pSecFabao0;

	INT nMaxFabaoStage = g_attRes.GetFabaoMaxStage();

	if(pMainFabao->nSlotNum < MaxWuxingSlotNum )
		return E_Fabao_LevelLimmit;
	if( !((tagFabaoProto*)pMainFabao->pProtoType)->bCanReset )
		return E_Fabao_NotPermitted;
	if( !((tagFabaoProto*)pSecFabao->pProtoType)->bCanUse )
		return E_Fabao_CannotBeAider;

	if( pSecFabao->n16Stage < 20 )
		return E_Fabao_SecondeFabaoLevelLower;

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pMainFabao,pMainFabao->pEquipProto->eEquipPos,TRUE,FALSE);

	EWuXing tem0[] = {EWX_Metal,EWX_Wood,EWX_Water,EWX_Fire,EWX_Soil};

	INT adierFabaoStage = pSecFabao->n16Stage;
	if( adierFabaoStage > nMaxFabaoStage )
		adierFabaoStage = nMaxFabaoStage;
	//int nWuxingMatchCount = 0;
	//for( int i = 0 ; i < MaxWuxingSlotNum; ++i)
	//	if( pSecFabao->eWuxing == pSecFabao->eWuxingHole[i] )
	//		++nWuxingMatchCount;
	INT nSecFabaoStage = pSecFabao->n16Stage;
	if( nSecFabaoStage > 40 )
		nSecFabaoStage = 40;
	INT nQv = pMainFabao->n16NativeIntelligence / 2 + pSecFabao->n16NativeIntelligence / 4 + nSecFabaoStage / 4;//nWuxingMatchCount * 10 + adierFabaoStage;

	ZeroMemory(pMainFabao->bySlotMacthNdx,sizeof(pMainFabao->bySlotMacthNdx));
	pMainFabao->bySlotLingzhuMatch = 0;
	ItemCreator::MergeWuxingHoleAtt(nQv,pMainFabao,pSecFabao);

	DWORD dwSecondFabaoTypeID = pSecFabao->pProtoType->dwTypeID;

	GetItemMgr().DelFromBag(srcFabaoID,ELCID_Fabao_ReversalCosted,1);

	// ���������Լ���������
	pMainFabao->bySlotLingzhuMatch = 0;
	for( int i = 0 ; i < pMainFabao->nSlotNum ; ++i )
	{
		const tagItemProto * pBeadProto = g_attRes.GetItemProto(pMainFabao->dwWuxingBeadID[i]);
		if( !P_VALID(pBeadProto) )
			continue;

		if( EISF_LingZhu == pBeadProto->eSpecFunc )
		{
			const tagFabaoProto * pFabaoProto = (tagFabaoProto*)pMainFabao->pProtoType;
			if( pBeadProto->nSpecFuncVal1 > pMainFabao->n16Stage || ( pBeadProto->nSpecFuncVal2 != GT_INVALID && pBeadProto->nSpecFuncVal2 != pFabaoProto->eFarbaoType) )
			{
				// E_Fabao_Inlay_Mismatch;
				continue;
			}
			const tagConsolidateItem* p = g_attRes.GetConsolidateProto(pMainFabao->dwWuxingBeadID[i]);
			if( P_VALID(p) )
			{
				int ndx = i;

				pMainFabao->bySlotMacthNdx[ndx] = 0;
				if( p->eWuXing == pMainFabao->eWuxingHole[ndx] ) //���ƥ��
				{
					pMainFabao->bySlotMacthNdx[ndx] = 1;
				}
				if( !pMainFabao->IsBind() )
					pMainFabao->Bind();

			}
		}
	}
	for(int i = 0; i < pMainFabao->nSlotNum; ++i)
		if( pMainFabao->bySlotMacthNdx[i] )
			pMainFabao->bySlotLingzhuMatch++;

	ProcEquipEffect(pMainFabao,NULL,pMainFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();

	GetItemMgr().UpdateFabaoSpec(*pMainFabao);
	
	if( P_VALID(m_pScript) )
		m_pScript->OnFabaoAttChange(this,pMainFabao,RFAE_HoleAttReset,dwSecondFabaoTypeID);

	return E_Success;
}

// ��������ת��
DWORD	Role::MergeFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagEquip * pMainFabao0 = GetItemMgr().GetEquipBarEquip(dstFabaoID);
	tagItem * pSecFabao0	= GetItemMgr().GetBagItem(srcFabaoID);
	if( !P_VALID(pMainFabao0) || !P_VALID(pSecFabao0) )
		return E_Fabao_Not_Exist;

	if( !MIsFaBao(pMainFabao0) && !MIsFaBao(pSecFabao0) )
		return E_Fabao_TypeMismatch;

	tagFabao * pMainFabao = (tagFabao*)pMainFabao0;
	tagFabao * pSecFabao	= (tagFabao*)pSecFabao0;

	if(pMainFabao->n16Stage < (MaxWuxingSlotNum + 1) * 10 )
		return E_Fabao_LevelLimmit;
	if( !((tagFabaoProto*)pMainFabao->pProtoType)->bCanReset )
		return E_Fabao_NotPermitted;
	if( !((tagFabaoProto*)pSecFabao->pProtoType)->bCanUse )
		return E_Fabao_CannotBeAider;

	DWORD dwSecondFabaoTypeID = pSecFabao->pProtoType->dwTypeID;

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pMainFabao,pMainFabao->pEquipProto->eEquipPos,TRUE,FALSE);

	INT nDifficulty = pSecFabao->n16NativeIntelligence;//pSecFabao->n16NativeIntelligence / 2 + pSecFabao->n16Stage / 2 + pMainFabao->n16NativeIntelligence / 2 + pMainFabao->n16Stage / 2;
	const tagFabaoProto * pFabaoProto = (tagFabaoProto*)pMainFabao->pProtoType;

	//INT nRandV = IUTIL->RandomInRange(0,100);
	//if( /*nRandV > 20 && */nRandV <= 10 )
	//{
	//	for( int i = 0; i < MaxWuxingRewardNUM ; ++i )
	//	{
	//		pMainFabao->nWuxingMatchReward[i] = pSecFabao->nWuxingMatchReward[i];
	//	}
	//}
	//else //if( nRandV > 30 )
	//{
	//	INT v = 0;
	//	for( int i = 0; i < MaxWuxingRewardNUM; ++i )
	//	{
	//		v = ItemCreator::SpecRandFabaoWuxingRewardID(nDifficulty,pMainFabao->n16NativeIntelligence,pFabaoProto->eFarbaoType,i+1,pMainFabao->nWuxingMatchReward);
	//		if( v > 0 )
	//			pMainFabao->nWuxingMatchReward[i] = v;
	//	}
	//}

	GetItemMgr().DelFromBag(srcFabaoID,ELCID_Fabao_MergingCosted,1);

	ProcEquipEffect(pMainFabao,NULL,pMainFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();

	GetItemMgr().UpdateFabaoSpec(*pMainFabao);

	if( P_VALID(m_pScript) )
		m_pScript->OnFabaoAttChange(this,pMainFabao,RFAE_WuxingXSReset,dwSecondFabaoTypeID);

	return E_Success;
}

DWORD Role::Extraction(DWORD dwNPCID, INT64 n64Item)
{
	tagItem* pItem = this->GetItemMgr().GetBagItem(n64Item);
	if (!P_VALID(pItem))
	{
		return E_Extraction_EquipInvalid;
	}

	if (!MIsEquipment(pItem->dwTypeID))
	{
		return E_Extraction_NotEquip;
	}
	tagEquip* pEquip = (tagEquip*)pItem;
	const tagExtractProto* pProto = g_attRes.GetExtractProto(pItem->dwTypeID);
	if (!P_VALID(pProto))
	{
		return E_Extraction_NoFormula;
	}

	if (pEquip->equipSpec.byQuality < pProto->byQuality)
	{
		return E_Extraction_QualityLimit;
	}

	if (GetAttValue(ERA_Vitality) < pProto->dwVitalityConsume)
	{
		return E_Extraction_VitalityLimit;
	}

	if (GetCurMgr().GetBagSilver() < pProto->dwMoneyConsume)
	{
		return E_Extraction_MoneyLimit;
	}

	INT nMaxOutNum = 0;
	for(int j=0; j<MAX_EXTRACT_OUTPUT; ++j)
	{
		if (!P_VALID(pProto->dwItemID[j]))
		{
			continue;
		}

		nMaxOutNum++;
	}

	if (GetItemMgr().GetBagFreeSize() < nMaxOutNum)
	{
		return E_Extraction_BagFull; 
	}

	for(INT i = 0; i < MAX_EXTRACT_OUTPUT; ++i)
	{
		if(pProto->dwItemID[i] == GT_INVALID)
			break;

		// ����ɹ���
		BOOL bResult = IUTIL->Rand() % 10000 <= (pProto->dwRate[i]);

		if(bResult)
		{
			const INT nMin = pProto->nItemMinNum[i];
			const INT nMax = pProto->nItemMaxNum[i];
			INT nStuffNum = IUTIL->RandomInRange(nMin, nMax);

			tagItem *pItem = CreateItem(EICM_Product,  m_dwID, pProto->dwItemID[i], (INT16)nStuffNum, m_dwID);

			if( P_VALID(pItem) )
			{
				GetItemMgr().Add2Bag(pItem, ELCID_Compose_Extract, TRUE);
			}
			
		}		
	}

	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pProto->dwVitalityConsume);

	GetCurMgr().DecBagSilver((INT64)pProto->dwMoneyConsume, ELCID_Compose_Extract);
	//ChangeVitality(-((INT)pProto->dwVitalityConsume));
	ChangeVitality(-((INT)nVitality));
	GetItemMgr().ItemUsedFromBag(n64Item, 1, ELCID_Compose_Extract);

	return E_Success;
}

//����󶨺ͷǰ��Ͼ���¶��Ҫ���ĸ���
// VOID Role::CalculateItemCostNum(DWORD dwTypeID, TList<tagItem*>& itemListBind,TList<tagItem*>& itemListUnBind,INT& nLjxlNum ,INT& nItemBindNum, INT& nItemUnBindNum)
// {
// 	if(GetItemMgr().GetBagSameItemCount(dwTypeID) < nLjxlNum)
// 	{
// 		//nLjxlNum = GetItemMgr().GetBagSameItemCount(pItem->dwTypeID);
// 		if (REBORN_STUFF_BINDING == dwTypeID)
// 		{
// 			nItemBindNum = GetItemMgr().GetBagSameItemList(itemListBind,dwTypeID);
// 			nItemUnBindNum = GetItemMgr().GetBagSameItemList(itemListUnBind,REBORN_STUFF_UNBINDING,nLjxlNum - nItemBindNum);
// 
// 			//��������Ͼ���¶����֮�Ͳ���
// 			if(nItemBindNum + nItemUnBindNum < nLjxlNum)
// 			{
// 				nLjxlNum = nItemBindNum + nItemUnBindNum;
// 			}
// 			else
// 			{
// 				nItemUnBindNum = nLjxlNum - nItemBindNum;
// 			}
// 		}
// 		else if(REBORN_STUFF_UNBINDING == dwTypeID)
// 		{
// 			nItemUnBindNum = GetItemMgr().GetBagSameItemList(itemListUnBind,dwTypeID);
// 			nItemBindNum = GetItemMgr().GetBagSameItemList(itemListBind,REBORN_STUFF_BINDING,nLjxlNum - nItemUnBindNum);
// 			//��������Ͼ���¶����֮�Ͳ���
// 			if(nItemBindNum + nItemUnBindNum < nLjxlNum)
// 			{
// 				nLjxlNum = nItemBindNum + nItemUnBindNum;
// 			}
// 			else
// 			{
// 				nItemBindNum = nLjxlNum - nItemUnBindNum;
// 			}
// 		}
// 	}
// 	else 
// 	{
// 		if (REBORN_STUFF_BINDING == dwTypeID)
// 		{
// 			nItemBindNum = GetItemMgr().GetBagSameItemList(itemListBind,dwTypeID,nLjxlNum);
// 			nItemBindNum = nLjxlNum;
// 		}
// 		else if(REBORN_STUFF_UNBINDING == dwTypeID)
// 		{
// 			nItemUnBindNum = GetItemMgr().GetBagSameItemList(itemListUnBind,dwTypeID,nLjxlNum);	
// 			nItemUnBindNum = nLjxlNum;
// 		}
// 	}
// }


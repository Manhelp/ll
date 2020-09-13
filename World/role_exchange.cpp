//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_exchange.cpp
// author: Sxg
// actor:
// data: 2008-09-01
// last:
// brief: ��Ҽ佻�״���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "role.h"
#include "map.h"
#include "../WorldDefine/msg_exchange.h"
#include "../WorldDefine/msg_item.h"
#include "../ServerDefine/log_cmdid_define.h"

//------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------
DWORD Role::ProcExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID)
{
	// �Ƿ��Լ����Լ�����
	if(GetID() == dwTgtRoleID)
	{
		return GT_INVALID;
	}
	
	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �������Ƿ���Է�����
	if(!CanExchange())
	{
		return E_Exchange_SelfBusy;
	}

	// �������߼��
	pTarget = pMap->FindRole(dwTgtRoleID);
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Exchange_NotInSame_Map;
	}

	// ���������Ƿ�ɽ���
	if(!pTarget->CanExchange())
	{
		return E_Exchange_RoleBusy;
	}

	//���Է���ϵͳ����
	if(pTarget->GetSession()->GetPersonalSet().bAcceptTradeRequest == false)
	{
		return E_Exchange_SystemRefuse;
	}

	// ���׾���
	if(!IsInDistance(*pTarget, MAX_EXCHANGE_DISTANCE))
	{
		return E_Exchange_OutOfRange;
	}

	// ���������߽���״̬
	BeginExchange(dwTgtRoleID);  
	//GetExchMgr().CreateData();
	//GetExchMgr().SetTgtRoleID(dwTgtRoleID);

	// ���ñ�������״̬
	pTarget->GetExchMgr().SetTgtRoleID(GetID());

	return E_Success;
}

//------------------------------------------------------------------------
// ��Ҷ������׵ķ���
//------------------------------------------------------------------------
DWORD Role::ProcExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode)
{
	// �Ƿ��Լ����Լ�����
	if(GetID() == dwTgtRoleID)
	{
		return GT_INVALID;
	}

	// �жϽ��׶����Ƿ���ȷ
	if(GetExchMgr().GetTgtRoleID() != dwTgtRoleID)
	{
		return GT_INVALID;
	}

	//// �ٴ��ж��Ƿ�æ
	//if(!CanExchange())
	//{
	//	return E_Exchange_RoleBusy;
	//}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// ���������߼��
	pApplicant = pMap->FindRole(dwTgtRoleID);
	if(!P_VALID(pApplicant))
	{
		// ���������ߣ����߸�����ͼ
		return E_Exchange_NotInSame_Map;
	}

	// �����ߵĽ���Ŀ���Ƿ�仯
	ExchangeMgr &exchMgrApp = pApplicant->GetExchMgr();
	if(pApplicant->IsExchanging() && exchMgrApp.GetTgtRoleID() != GetID())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	// ��齻�׷�����
	if(dwErrorCode != E_Exchange_Accept)
	{
		// �������׷����ߵĽ���״̬
		pApplicant->EndExchange();
		return dwErrorCode;
	}

	// �����߽���״̬���
	if(!pApplicant->IsExchanging())
	{
		exchMgrApp.SetTgtRoleID(GT_INVALID);
		return GT_INVALID;
	}

	// ���׾���
	if(!IsInDistance(*pApplicant, MAX_EXCHANGE_DISTANCE))
	{
		return E_Exchange_OutOfRange;
	}

	// ���������߽���״̬
	BeginExchange(dwTgtRoleID);
	//pApplicant->SetRoleState(ERS_Exchange);

	return E_Exchange_Accept;
}

//------------------------------------------------------------------------
// ��ӽ�����Ʒ
//------------------------------------------------------------------------
DWORD Role::ProcExchangeAdd(OUT Role* &pTarget, OUT tagItem* &pItem, INT32 &nInsIndex, INT64 n64Serial)
{
	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ȡ�ý�����Ʒ
	pItem = GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pItem))
	{
		ASSERT(P_VALID(pItem));
		return GT_INVALID;
	}

	// ��Ʒ�Ƿ�ɱ���
	if(!GetItemMgr().CanExchange(*pItem))
	{	
		return E_Exchange_ItemCanNot_Exchange;
	}

	// ���뵽��������Ʒ��
	nInsIndex = GetExchMgr().AddItem(n64Serial, pItem->GetNum());
	if(GT_INVALID == nInsIndex)
	{
		// ���ڽ����б��У�������Ʒ�����Ѵﵽ����
		return GT_INVALID;
	}

	// ���Ŀ������״̬
	if( pTarget->GetExchMgr().IsValidate() )
	pTarget->GetExchMgr().Unlock();

	return E_Success;
}

//------------------------------------------------------------------------
// ȡ��������Ʒ
//------------------------------------------------------------------------
DWORD Role::ProcExchangeDec(OUT Role* &pTarget, INT64 n64Serial)
{
	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// ���Ŀ������״̬
	if( pTarget->GetExchMgr().IsValidate() )
	pTarget->GetExchMgr().Unlock();
	
	return GetExchMgr().DecItem(n64Serial);
}

//------------------------------------------------------------------------
// �޸Ľ��׽�Ǯ
//------------------------------------------------------------------------
DWORD Role::ProcExchangeMoney(OUT Role* &pTarget, INT64 n64Silver)
{
	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	if( !pTarget->GetExchMgr().IsValidate() )
	{
		EndExchange();
		return E_Exchange_NotAnswer;
	}

	// ���Ŀ������״̬
	pTarget->GetExchMgr().Unlock();

	// �������Ƿ����㹻�Ľ�Ǯ
	CurrencyMgr &CurMgr = GetCurMgr();
	if(n64Silver > CurMgr.GetBagSilver())
	{
		return E_Exchange_NotEnough_Money;
	}

	GetExchMgr().ResetMoney(n64Silver);

	return E_Success;
}

//------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------
DWORD Role::ProcExchangeLock(OUT Role* &pTarget)
{
	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	GetExchMgr().Lock();

	return E_Success;
}

//------------------------------------------------------------------------
// ���ȡ������
//------------------------------------------------------------------------
DWORD Role::ProcExchangeCancel(OUT Role* &pTarget)
{
	// �Ƿ��ڽ���״̬
	if(!IsExchanging())
	{
		return GT_INVALID;
	}

	// ��ý��׹������ͽ��׶���id
	ExchangeMgr &exchMgr = GetExchMgr();
	DWORD dwTargetRoleID = exchMgr.GetTgtRoleID();
	
	// ȡ������
	EndExchange();

	//// ���������Ƿ�������״̬
	//if(exchMgr.IsLock())
	//{
	//	return GT_INVALID;
	//}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pTarget = pMap->FindRole(dwTargetRoleID);
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Exchange_NotInSame_Map;
	}

	// Ŀ��û�д��ڽ���״̬�������ںͱ��˽���
	if(!pTarget->IsExchanging() || pTarget->GetExchMgr().GetTgtRoleID() != GetID())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	pTarget->EndExchange();

	return E_Success;
}

//------------------------------------------------------------------------
// ȷ�Ͻ���
//------------------------------------------------------------------------
DWORD Role::ProcExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID)
{
	DWORD dwErrorCode = E_Success;
	
	// �Ƿ��ڽ���״̬
	if(!IsExchanging())
	{
		return GT_INVALID;
	}

	// ��ý��׹�����
	ExchangeMgr &exchMgr = GetExchMgr();

	// ���������Ƿ�������״̬
	if(!exchMgr.IsLock())
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pTarget = pMap->FindRole(exchMgr.GetTgtRoleID());
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		dwFailedRoleID	= exchMgr.GetTgtRoleID();
		dwErrorCode		= E_Exchange_NotInSame_Map;
		goto Exit;
	}

	// Ŀ��û�д��ڽ���״̬�������ںͱ��˽���
	if(!pTarget->IsExchanging() || pTarget->GetExchMgr().GetTgtRoleID() != GetID())
	{
		//return E_Exchange_ApplicantTgt_Change;
		return GT_INVALID;
	}

	// Ŀ���Ƿ�������״̬
	if(!pTarget->GetExchMgr().IsLock())
	{
		return GT_INVALID;
	}

	exchMgr.Verify();
	if(!pTarget->GetExchMgr().IsVerify())
	{
		return GT_INVALID;
	}

	// ���׾���ȷ��
	if(!IsInDistance(*pTarget, MAX_EXCHANGE_DISTANCE))
	{
		dwErrorCode = E_Exchange_OutOfRange;
		goto Exit;
	}

	// �����ռ��� -- �����ռ䲻��ʱ���������������
	BYTE byExTypeNum	= exchMgr.GetItemTypeNum();
	BYTE byTgtExTypeNum	= pTarget->GetExchMgr().GetItemTypeNum();
	if(byExTypeNum > byTgtExTypeNum)
	{
		// ���Է�����
		if(byExTypeNum - byTgtExTypeNum > pTarget->GetItemMgr().GetBagFreeSize())
		{
			// ����
			exchMgr.Unlock();
			pTarget->GetExchMgr().Unlock();

			dwFailedRoleID = pTarget->GetID();
			return E_Exchange_NotEnough_BagSpace;
		}
	}
	else
	{
		// ����Լ�����
		if(byTgtExTypeNum - byExTypeNum > GetItemMgr().GetBagFreeSize())
		{
			// ����
			exchMgr.Unlock();
			pTarget->GetExchMgr().Unlock();

			dwFailedRoleID = GetID();
			return E_Exchange_NotEnough_BagSpace;
		}
	}

	/*************************************************
	* ������Ʒ����
	*************************************************/

	tagItem *pSrcItem[MAX_EXCHANGE_ITEM];
	tagItem *pDstItem[MAX_EXCHANGE_ITEM];
	ZeroMemory(pSrcItem, sizeof(tagItem*) * MAX_EXCHANGE_ITEM);
	ZeroMemory(pDstItem, sizeof(tagItem*) * MAX_EXCHANGE_ITEM);

	// ������Ʒ���
	dwErrorCode = VerifyExchangeData(pSrcItem);
	if(dwErrorCode != E_Success)
	{
		dwFailedRoleID = GetID();
		goto Exit;
	}

	dwErrorCode = pTarget->VerifyExchangeData(pDstItem);
	if(dwErrorCode != E_Success)
	{
		dwFailedRoleID = pTarget->GetID();
		goto Exit;
	}

	// �������ʥ�飬���ж϶Է���ʥ����Ŀ�Ƿ�ﵽ����
	int nSrcHolyNum = 0;
	int nDstHolyNum = 0;
	INT64	n64SrcHolyEquipSerial[SHENGLING_EQUIP_BLANK] = {0};
	INT64	n64DstHolyEquipSerial[SHENGLING_EQUIP_BLANK] = {0};
	tagItem *pSrcHolyEquip[SHENGLING_EQUIP_BLANK];
	ZeroMemory(pSrcHolyEquip, sizeof(tagItem*) * SHENGLING_EQUIP_BLANK);
	tagItem *pDstHolyEquip[SHENGLING_EQUIP_BLANK];
	ZeroMemory(pDstHolyEquip, sizeof(tagItem*) * SHENGLING_EQUIP_BLANK);
	int j=0,n=0;
	for (int i=0; i<MAX_EXCHANGE_ITEM; i++)
	{
		if (MIsHoly(pSrcItem[i]))
		{
			nSrcHolyNum++;
			tagHolyMan * pHoly = (tagHolyMan *)pSrcItem[i];
			for ( int k = 0; k<pHoly->n16EquipmentNumber; k++)
			{
				if (k >= MaxHolyEquipNum)
				{
					ILOG->Write(_T("ProcExchangeVerify error: holynum is invalid,pHoly->n16EquipmentNumber:%d!\r\n"), pHoly->n16EquipmentNumber);					
					break;
				}
				if (pHoly->n64EquipSerialIDs[k] != 0)
				{
					n64SrcHolyEquipSerial[j] = pHoly->n64EquipSerialIDs[k];
					pSrcHolyEquip[j] = GetItemMgr().GetHolyManBarItem(pHoly->n64EquipSerialIDs[k]);
					j++;
				}

			}
		}

	}

	for (int m=0; m<MAX_EXCHANGE_ITEM; m++)
	{
		if (MIsHoly(pDstItem[m]))
		{
			nDstHolyNum++;
			tagHolyMan * pHoly = (tagHolyMan *)pDstItem[m];
			for ( int k = 0; k<pHoly->n16EquipmentNumber; k++)
			{
				if (k >= MaxHolyEquipNum)
				{
					break;
				}
				if (pHoly->n64EquipSerialIDs[k] != 0)
				{
					n64DstHolyEquipSerial[n] = pHoly->n64EquipSerialIDs[k];
					pDstHolyEquip[n] = pTarget->GetItemMgr().GetHolyManBarItem(pHoly->n64EquipSerialIDs[k]);
					n++;
				}

			}
		}
	}
	if ((nSrcHolyNum + pTarget->GetItemMgr().GetHolyNum() > MAX_HOLY_NUM) || (nDstHolyNum + GetItemMgr().GetHolyNum()> MAX_HOLY_NUM))
	{
		dwErrorCode =  E_Exchange_HolyExceed;
		goto Exit;
	}

	// 4.�ƶ���Ʒ

	if( GetCurMgr  ().GetBagSilver() - exchMgr.GetMoney() + pTarget->GetExchMgr().GetMoney() > GetCurMgr  ().GetMaxBagSilver() ||
		pTarget->GetCurMgr().GetBagSilver  () - pTarget->GetExchMgr().GetMoney() + exchMgr.GetMoney() > pTarget->GetCurMgr  ().GetMaxBagSilver  () )
	{
		dwErrorCode =  E_Exchange_BagSilverWillBeFull;
		goto Exit;
	}

	// 4.1 ��Ǯ -- �ȼ���Ǯ,������ʧ
	BOOL bCanExg = TRUE;
	if(!GetCurMgr().DecBagSilver(exchMgr.GetMoney(), ELCID_Exchange_Verify, pTarget->GetID()))
	{
		//dwErrorCode =  E_Exchange_NotEnough_Money;
		//goto Exit;
	}
	if( !pTarget->GetCurMgr().DecBagSilver(pTarget->GetExchMgr().GetMoney(), ELCID_Exchange_Verify, GetID()) )
	{
		//dwErrorCode =  E_Exchange_NotEnough_Money;
		//goto Exit;
	}

	GetCurMgr().IncBagSilver(
		min(GetCurMgr().GetCanIncBagSilver(), pTarget->GetExchMgr().GetMoney()), 
		ELCID_Exchange_Verify, pTarget->GetID());

	pTarget->GetCurMgr().IncBagSilver(
		min(pTarget->GetCurMgr().GetCanIncBagSilver(), exchMgr.GetMoney()), 
		ELCID_Exchange_Verify, GetID());

	// 4.2 ��Ʒ

	// 4.2.1 �ȴ��������ɾ��������Ʒ -- ����ɶѵ���Ʒ�������
	GetItemMgr().RemoveFromBag(exchMgr.GetSerialArray(), MAX_EXCHANGE_ITEM, ELCID_Exchange_Verify, pTarget->GetID());
	pTarget->GetItemMgr().RemoveFromBag(pTarget->GetExchMgr().GetSerialArray(), 
										MAX_EXCHANGE_ITEM, ELCID_Exchange_Verify, GetID());

	// 4.2.2 ��������Ʒ�ŵ��������
	GetItemMgr().Add2Bag(pDstItem, MAX_EXCHANGE_ITEM, ELCID_Exchange_Verify, pTarget->GetID());
	pTarget->GetItemMgr().Add2Bag(pSrcItem, MAX_EXCHANGE_ITEM, ELCID_Exchange_Verify, GetID());

	int nSrcHolyEquip = j;
	int nDstHolyEquip = n;
	// ���������ɾ������ʥ���ϵ�ʥ��
	if (nSrcHolyNum > 0)
	{
		GetItemMgr().RemoveFromHolyManBar(n64SrcHolyEquipSerial, nSrcHolyEquip, ELCID_Exchange_Verify, pTarget->GetID());
	}
	if (nDstHolyNum > 0)
	{
		pTarget->GetItemMgr().RemoveFromHolyManBar(n64DstHolyEquipSerial, 
			nDstHolyEquip, ELCID_Exchange_Verify, GetID());
	}

	// ������ʥ���ϵ�ʥ�Ʒŵ��������
	if (nDstHolyNum > 0)
	{
		GetItemMgr().AddHolyManBar(pDstHolyEquip, nDstHolyEquip, ELCID_Exchange_Verify, pTarget->GetID());
	}
	if (nSrcHolyNum > 0)
	{
		pTarget->GetItemMgr().AddHolyManBar(pSrcHolyEquip, nSrcHolyEquip, ELCID_Exchange_Verify, GetID());
	}

	// ��һ��һ�����ߵ��¼��ӿ�
	if (P_VALID(pDstItem[0]))
	{
		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, (pDstItem[0])->dwTypeID, (pDstItem[0])->eCreateMode);
	}

Exit:
	// �ͷŽ�����Դ
	EndExchange();

	if(P_VALID(pTarget))
	{
		pTarget->EndExchange();
	}
	else
	{
		// �������Ŀ����������ͼ���ô�Ҳ����ֱ�Ӳ���(���̵߳���)
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// ���������ϵĽ�����Ʒ�Ƿ���ȫ
//------------------------------------------------------------------------
DWORD Role::VerifyExchangeData(OUT tagItem* pItem[])
{
	// ��ý��׹�����
	ExchangeMgr &exchMgr = GetExchMgr();
	
	// 1.��Ǯ�Ƿ񻹹�
	if(GetCurMgr().GetBagSilver() < exchMgr.GetMoney())
	{
		return E_Exchange_NotEnough_Money;
	}

	//// 2.�����Ƿ����㹻�Ŀռ�
	//if(GetItemMgr().GetBagFreeSize() < exchMgr.GetItemTypeNum())
	//{
	//	return E_Exchange_NotEnough_BagSpace;
	//}

	// 3.��������Ʒ�Ƿ�������, ���ڣ��õ���Ʒָ��
	if(!GetItemMgr().CheckExistInBag(pItem, exchMgr.GetSerialArray(), exchMgr.GetNumArray(), MAX_EXCHANGE_ITEM))
	{
		return E_Exchange_ItemCanNot_Find;
	}

	return E_Success;
}

//------------------------------------------------------------------------
// �޸Ľ�������ǰ�ļ�飬���õ���ض���
//------------------------------------------------------------------------
DWORD Role::ProcPrepareExchange(OUT Role* &pTarget)
{
	// �Ƿ��ڽ���״̬
	if(!IsExchanging())
	{
		return GT_INVALID;
	}

	// ��ý��׹�����
	ExchangeMgr &exchMgr = GetExchMgr();

	// ���������Ƿ�������״̬
	if(exchMgr.IsLock())
	{
		return GT_INVALID;
	}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pTarget = pMap->FindRole(exchMgr.GetTgtRoleID());
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Exchange_NotInSame_Map;
	}

	// Ŀ��û�д��ڽ���״̬�������ںͱ��˽���
	if(/*!pTarget->IsExchanging() || */pTarget->GetExchMgr().GetTgtRoleID() != GetID())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	return E_Success;
}

//-----------------------------------------------------
// ������ߵ�ʱ�����ڽ��׵�״̬ȡ��
//-------------------------------------------------------
VOID Role::ClenupExchState()
{
	
	DWORD dwTagetRoleid = GetExchMgr().GetTgtRoleID();
	if (P_VALID(dwTagetRoleid))
	{
		// ��ȡ��ͼ
		Map *pMap = GetMap();
		Role* pTarget = NULL;
		if(P_VALID(pMap))
		{
			pTarget = pMap->FindRole(dwTagetRoleid);
			if( P_VALID(pTarget) )
				pTarget->EndExchange();
		}
	}
	EndExchange();
}
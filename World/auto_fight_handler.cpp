//-----------------------------------------------------------------------------
// brief: ս���һ�ϵͳ��Ϣ����
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "player_session.h"
#include "../WorldDefine/msg_AutoFight.h"
#include "../WorldDefine/ItemDefine.h"
#include "role.h"

// ʹ��ս����
DWORD PlayerSession::HandleUseZhanDouFu(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseZhanDouFu);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//��ͼ�������жϴ���
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	tagNS_UseZhanDouFu send;

	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if (!P_VALID(pItem))
	{
		send.dwErrorCode = E_AutoFight_No_ZhanDouFu;
	}
	else if (P_VALID(pItem->pProtoType) && pItem->pProtoType->eSpecFunc != EISF_ZhanDouFu)
	{
		send.dwErrorCode = E_AutoFight_No_ZhanDouFu;
	}
	else
	{
		// �����̨��汾��ս��������dwSpecVal1��¼ʣ����������ʼֵΪdwTimeLimit
		if (pItem->pProtoType->nSpecFuncVal1 == EAFSFV2_OMG && pItem->dwSpecVal1 == 0)
		{
			pItem->dw1stGainTime = 0;
			pItem->dwSpecVal1 = pItem->pProtoType->dwTimeLimit*TICK_PER_SECOND;
			pItem->BindOMGAutoFightItem();
			pItem->SetUpdate(EUDBS_Update);
		}
		else if (pItem->pProtoType->nSpecFuncVal1 == EAFSFV2_Normal)
		{
			if (!pRole->GetItemMgr().GetBag().IsInTimeLimitMgr(pRecv->n64ItemID))
			{
					pItem->dw1stGainTime = GetCurrentDWORDTime();
					pItem->SetUpdate(EUDBS_Update);
					pRole->GetItemMgr().GetBag().Add2TimeLeftList(pRecv->n64ItemID, pItem->pProtoType->dwTimeLimit, pItem->dw1stGainTime);
			}
		}
		send.dw1stGainTime = pItem->dw1stGainTime;
		send.n64ItemID = pRecv->n64ItemID;
		send.dwLeftTicks = pItem->dwSpecVal1;
		send.dwErrorCode = E_AutoFight_UseZhanDouFu_Success;
	}

	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

// ͬ���ڹ�״̬
DWORD PlayerSession::HandleSyncAutoFightState(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SyncAutoFightState);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	// ��ͼ�������жϴ���
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	if (pRecv->eAutoFightState == EAFS_End ||
		pRecv->eAutoFightState == EAFS_Start)
	{
		tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
		if (!P_VALID(pItem))
		{
			// û��ս�������ߣ��ȹص�
			pRole->SetAutoFightState(EAFS_Close);
			pRole->SetCurZhanDouFu64ID(0);
		}
		else
		{
			// ��ս�������ߣ����ý�ɫս���һ�״̬
			INT64 n64CurZhanDouFu = (pRecv->eAutoFightState == EAFS_Start)?pRecv->n64ItemID:0;
			pRole->SetAutoFightState(pRecv->eAutoFightState);
			pRole->SetCurZhanDouFu64ID(n64CurZhanDouFu);
		}
	}
	else
	{
		pRole->SetAutoFightState(EAFS_Close);
		pRole->SetCurZhanDouFu64ID(0);
	}
	
	// Ȼ��֪ͨ�ͻ���
	tagNS_SyncAutoFightState send;
	send.n64ItemID = pRecv->n64ItemID;
	send.eAutoFightState = pRole->GetAutoFightState();
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

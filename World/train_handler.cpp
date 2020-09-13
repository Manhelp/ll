//-----------------------------------------------------------------------------
// Copyright (c) 2010 KONG.NET All rights reserved.
// filename: exit_game_handle.cpp
// author: lfmeng
// actor:
// data: 2010-3-10
// last:
// brief: ��֮����ϵͳ-��Ϣ����
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../WorldDefine/msg_train.h"
#include "../WorldDefine/train_define.h"

#include "player_session.h"
#include "role.h"

//-----------------------------------------------------------------------------
// ȡ�����б�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetTrainList(tagNetCmd* pCmd)
{
//	MGET_MSG(pRecv, pCmd, NC_GetTrainList);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))	return GT_INVALID;

	return pRole->SendTrainStateList2Client();
}

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleFinishTrain(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_FinishTrain);
	
	DWORD dwErrCode = E_Train_Not_Pass;
	tagNS_FinishTrain FinishTrain;
	Role* pRole		= GetRole();
	DWORD dwTrainID = pRecv->dwTrainID;

	if (!P_VALID(pRole))
	{
		return E_Train_Unknown;
	}

	// ����ɻ���δ����������ֱ�ӷ���
	if (pRole->GetTrainState(dwTrainID) != ETS_Finish)
	{
		return E_Train_Unknown;
	}

	const tagTrainAttProto* pTrainAttProto = g_attRes.GetOneTrainAttProto(dwTrainID);
	if (!P_VALID(pTrainAttProto))
	{
		return E_Train_Unknown;
	}
	INT nLapCnt = 0;
	nLapCnt += (pTrainAttProto->TrainReward.pItemReward[0].nItemCount>0)?1:0;
	nLapCnt += (pTrainAttProto->TrainReward.pItemReward[1].nItemCount>0)?1:0;

	// ��ⱳ��ʣ������
	ItemMgr& itemMgr = pRole->GetItemMgr();	
	INT nFreeSize = itemMgr.GetBagFreeSize();
	if (nFreeSize < nLapCnt)
	{
		//����Ϣ
		FinishTrain.dwTrainID		= dwTrainID;
		FinishTrain.eTrainErrCode	= E_Train_Bag_Full;
		FinishTrain.eTrainState		= pRole->GetTrainState(dwTrainID);
	}
	else
	{
		// ����ֵΪ1ͨ������
		if (g_ScriptMgr.GetRoleScript()->IsTrainFinished(pRole, dwTrainID) == 1)
		{
			// ������
			dwErrCode = pRole->GiveTrainReward(dwTrainID);
			switch (dwErrCode)
			{
			case E_Train_Pass:				// ͨ������
				FinishTrain.eTrainErrCode = E_Train_Pass;
				FinishTrain.eTrainState = ETS_Pass;
				break;
			case E_Train_List_Null:			// �����б�Ϊ��
			case E_Train_Bag_Full:			// ���ҿռ䲻�㣬��Ϊ����������㹻�ռ������µ�����������
			case E_Train_Not_Pass:			// ����ͨ������
				FinishTrain.eTrainErrCode = static_cast<ETrainErrCode>(dwErrCode);
				FinishTrain.eTrainState = pRole->GetTrainState(dwTrainID);
				break;
			case E_Train_Unknown:			// δ֪����
			default:
				FinishTrain.eTrainErrCode = E_Train_Unknown;
				FinishTrain.eTrainState = pRole->GetTrainState(dwTrainID);
				break;
			}
			FinishTrain.dwTrainID = dwTrainID;
		}
		else
		{
			//����Ϣ
			FinishTrain.dwTrainID		= dwTrainID;
			FinishTrain.eTrainErrCode	= E_Train_Not_Pass;
			FinishTrain.eTrainState		= pRole->GetTrainState(dwTrainID);
		}
	}


	// ��������״̬
	if (FinishTrain.eTrainState == ETS_Pass)
	{
		pRole->SaveTrainState(FinishTrain.dwTrainID, FinishTrain.eTrainState);
	}

	pRole->SendMessage((LPVOID)(&(FinishTrain)), FinishTrain.dwSize);

	return dwErrCode;
}

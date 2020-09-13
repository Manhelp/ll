//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: character_handler.cpp
// author: sunnee
// actor:
// data: 2009-11-09
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- ����ϵͳ
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../WorldDefine/msg_city.h"

#include "player_session.h"
#include "city.h"
#include "city_mgr.h"
#include "role.h"
#include "role_mgr.h"
#include "activity_mgr.h"

//-----------------------------------------------------------------------------
// ��ȡ����������Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityAttInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr)
					&& pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_SignUp);
	if(E_Success != dwErrorCode)
	{
		tagNS_GetCityInfo send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// ������Ϣ
	INT nMsgSize = sizeof(tagNS_GetCityInfo) + (MAX_CITY_ITEM_OUTPUT - 1) * sizeof(tagCityItemOutput);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCityInfo);

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		pSend->dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		pSend->dwErrorCode = pCity->GetCityAtt(pRole, &(pSend->sCityAtt));
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNS_GetCityInfo) + (pSend->sCityAtt.nItemOutputType - 1) * sizeof(tagCityItemOutput);

	// ������Ϣ
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
	return E_Success;
}

//-----------------------------------------------------------------------------
// ���ݳ���id��ȡ����ָ��������Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityAttInfoByCityID(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityInfoByCityID);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRecv->dwCityID);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// ������Ϣ
	INT nMsgSize = sizeof(tagNS_GetCityInfo) + (MAX_CITY_ITEM_OUTPUT - 1) * sizeof(tagCityItemOutput);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCityInfo);

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		pSend->dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		pSend->dwErrorCode = pCity->GetCityAtt(pRole, &(pSend->sCityAtt));
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNS_GetCityInfo) + (pSend->sCityAtt.nItemOutputType - 1) * sizeof(tagCityItemOutput);

	// ������Ϣ
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
	return E_Success;
}
//-----------------------------------------------------------------------------
// ��ȡ����ָ��������Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetAppointedCityAtt(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetAppointedCityAtt);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	if((E_Success != dwErrorCode) || !P_VALID(pCity))
	{
		tagNS_GetAppointedCityAtt send;
		send.sCityAtt.eType = pRecv->eCityAttType;
		send.sCityAtt.nDataNum = 0;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// ��Ϣ����
	INT nMsgSize = sizeof(tagNS_GetAppointedCityAtt) - 1 + pCity->GetCityAppointedAttSize(pRecv->eCityAttType);
	MCREATE_MSG(pSend, nMsgSize, NS_GetAppointedCityAtt);

	pSend->sCityAtt.eType = pRecv->eCityAttType;
	pCity->GetCityAppointedAtt(pRecv->eCityAttType, pSend->sCityAtt.nDataNum, pSend->sCityAtt.byData);

	// ������Ϣ
	SendMessage(pSend, pSend->dwSize);
	
	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������˰��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSetTaxRate(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTaxRate);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC���
	tagNS_SetTaxRate send;
	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);
	if (send.dwErrorCode != E_Success)
	{
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// ����˰��
	INT nTemp = 0;
	pCity->GetCityAppointedAtt(ECAT_CityID, nTemp, &(send.dwCityID));
	send.dwErrorCode = pCity->ModifyTaxRate(pRole, pRecv->nTaxRate);
	if (send.dwErrorCode != E_Success )
	{
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}
	pCity->GetCityAppointedAtt(ECAT_TaxRate, nTemp, &(send.nTaxRate));

	// ȫ����㲥
	g_roleMgr.SendWorldMsg(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ����˰��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleTransformTaxation(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_TransformTaxation);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC���
	tagNS_TransformTaxation send;
	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);
	if (send.dwErrorCode != E_Success)
	{
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// ��ȡ˰��
	INT nTemp = 0;
	send.dwErrorCode = pCity->TransformTaxation(pRole, pRecv->eTransType, pRecv->nTaxation);
	pCity->GetCityAppointedAtt(ECAT_Taxation, nTemp, &(send.nTaxation));

	// ���ͷ���
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ս����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSignupForCitywar(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SignupForCitywar);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	tagNS_SignupForCitywar send;

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		send.dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		send.dwErrorCode = pCity->SignupForCitywar(pRole,pRecv->eForce,pRecv->nFare);
	}

	// ���ͷ���
	SendMessage(&send, send.dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
// �õ������Ļ�����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSigupInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetSigupInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		return GT_INVALID;
	}
	
	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_SignUp);

	if(dwErrorCode != E_Success)
	{
		tagNS_GetSigupInfo send;
		send.dwErrorCode = dwErrorCode;
		// ���ͷ���
		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}
	//����������
	INT32 nAckNum = 0;
	INT32 nDefNum = 0;
	INT32 nConfirmNum = 0;
	INT nTemp = 0;
	pCity->GetCityAppointedAtt(ECAT_AttackSignUpNum,nTemp,&nAckNum);
	pCity->GetCityAppointedAtt(ECAT_DefenceSignUpNum,nTemp,&nDefNum);
	pCity->GetCityAppointedAtt(ECAT_DefenceConfirmNum,nTemp,&nConfirmNum);

	INT32 nTotalNum = (nAckNum + nDefNum + nConfirmNum) == 0 ? 1 : (nAckNum + nDefNum + nConfirmNum);
	DWORD dwSize = sizeof(tagNS_GetSigupInfo) + (nTotalNum - 1) * sizeof(DWORD);
	MCREATE_MSG(pSend, dwSize, NS_GetSigupInfo);

	//�õ�������Ϣ
	pCity->GetCitySigupInfo(pRole,pSend);
	//�������� ��ȷ������
	pSend->nAckNum		= nAckNum;
	pSend->nDefNum		= nDefNum;
	pSend->nConfirmNum	= nConfirmNum;

	// ���ͷ���
	SendMessage(pSend, dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
// ȷ�Ϸ��ط�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleConfirmDefenceForce(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ConfirmDefenceForce);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_SignUp);

	if(dwErrorCode != E_Success)
	{
		tagNS_ConfirmDefenceForce send;
		send.dwErrorCode = dwErrorCode;
		// ���ͷ���
		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}
	
	tagNS_ConfirmDefenceForce send;

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		send.dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		//���ط�ȷ��
		send.dwErrorCode = pCity->ConfirmDefenceForce(pRole,pRecv->dwGuildID);
	}

	// ���ͷ���
	SendMessage(&send, send.dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
// �õ���ս�еĳ���id
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityWarCity(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityWarCity);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		return GT_INVALID;
	}

	// ȡ�ó�ս�г�����Ŀ
	DWORD dwNum = g_cityMgr.GetCityNum();

	DWORD dwSize = sizeof(tagNS_GetCityWarCity) + (dwNum - 1) * sizeof(DWORD);

	//�����䳤��Ϣ
	MCREATE_MSG(pSend, dwSize, NS_GetCityWarCity);

	//�õ���ս�еĳ���id
	g_cityMgr.GetCityWarCity(pSend);

	// ���ͷ���
	SendMessage(pSend, pSend->dwSize);

	return E_Success;

}
//-----------------------------------------------------------------------------
// �õ���ս��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityWarInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityWarInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRecv->dwCityID);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// ��սδ����
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		return GT_INVALID;
	}

	tagNS_GetCityWarInfo send;
	//�õ�������Ϣ
	pCity->GetCityWarInfo(pRole,&send);

	// ���ͷ���
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpgradeProlificacy(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UpgradeProlificacy);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);
	if (dwErrorCode != E_Success)
	{	
		tagNS_UpgradeProlificacy send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// ȡ�ó���
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	tagNS_UpgradeProlificacy send;
	send.dwErrorCode = pCity->UpgradeProlificacy(pRole,pRecv->eType);

	// ���ͷ���
	SendMessage(&send, send.dwSize);

	return E_Success;

}
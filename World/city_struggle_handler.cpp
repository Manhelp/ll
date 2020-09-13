//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_struggle_handler.cpp
// author: twan
// actor:
// data: 2009-12-14
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ���� -- ��սϵͳ
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../WorldDefine/msg_city_struggle.h"
#include "../WorldDefine/creature_define.h"

#include "player_session.h"
#include "city.h"
#include "city_mgr.h"
#include "role.h"
#include "role_mgr.h"

//-----------------------------------------------------------------------------
// �����ػ�ʯ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleWaitSummon(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_WaitSummon);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	const tagCreatureProto* pProto = g_attRes.GetCreatureProto(pRecv->dwCreatureID);
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	BOOL bSuccess = FALSE;
	DWORD dwRet = g_ScriptMgr.GetRoleScript()->OnWaitSummon(pRole, pRecv->dwCreatureID, bSuccess);

	if (dwRet == E_Success)
	{
		tagNS_WaitSummon send;
		send.bSuccess = bSuccess;
		SendMessage(&send, send.dwSize);

		return E_Success;
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------------------
// ȡ���ٻ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCancelSummon(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CancelSummon);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	const tagCreatureProto* pProto = g_attRes.GetCreatureProto(pRecv->dwCreatureID);
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	DWORD dwRet = g_ScriptMgr.GetRoleScript()->OnCancelSummon(pRole, pRecv->dwCreatureID);

	return dwRet;
}

//-----------------------------------------------------------------------------
// ��ս����ս��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCSRoleRecord(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwRet = pRole->SendCityStruggleRecord();

	return dwRet;
}
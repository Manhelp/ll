//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loong_common.cpp
// author: Sxg
// actor:
// data: 2008-06-04
// last:
// brief: �������Ϸ���������յ�����Ϣ -- account_common�����
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "Loong.h"
#include "LoongDB.h"
#include "../ServerDefine/msg_common.h"
//-------------------------------------------------------------------------------------------------------
// ���ð�ȫ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SetSafeCode(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SafeCodeSet);

	if(!m_pDB->UpdateSafeCode(p->dwAccountID, p->dwSafeCodeCrc))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ð�ȫ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ResetSafeCode(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SafeCodeReset);

	if(!m_pDB->UpdateSafeCodeResetTime(p->dwAccountID, p->dwTimeReset))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ȡ����ȫ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CancelSafeCodeReset(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SafeCodeResetCancel);

	if(!m_pDB->UpdateSafeCodeResetTime(p->dwAccountID, GT_INVALID))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// �޸��ʺ���������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeBagPsd(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_ChangeBagPsd);

	if(!m_pDB->UpdateBagPsd(p->dwAccountID, p->dwNewBagPsd))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���²ֿ��С
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateWareSize(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_WareSizeUpdate);

	if(!m_pDB->UpdateWareSize(p->dwAccountID, &p->n16WareSize))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���²ֿ��Ǯ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateWareMoney(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_WareMoneyUpdate);

	if(!m_pDB->UpdateWareMoney(p->dwAccountID, p->n64WareSilver))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���°ٱ���Ԫ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateBaiBaoYuanBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_BaiBaoYBUpdate);

	if(!m_pDB->UpdateBaiBaoYuanBao(p->dwAccountID, &p->nBaiBaoYuanBao))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���±���Ԫ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateBagYuanBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_BagYBUpdate);
	if (!P_VALID(p))
	{
		return GT_INVALID;
	}

	if (p->bIncrement)
	{
		if(!m_pDB->IncBagYuanBao(p->dwRoleID, &p->nBagYuanBao))
		{
			return GT_INVALID;
		}
	}
	else
	{
		if(!m_pDB->UpdateBagYuanBao(p->dwRoleID, &p->nBagYuanBao))
		{
			return GT_INVALID;
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���±������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateBagGold(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_BagGoldUpdate);
	if (!P_VALID(p))
	{
		return GT_INVALID;
	}

	if (p->bIncrement)
	{
		if(!m_pDB->IncBagGold(p->dwRoleID, &p->nBagGod))
		{
			return GT_INVALID;
		}
	}
	else
	{
		if(!m_pDB->UpdateBagGold(p->dwRoleID, &p->nBagGod))
		{
			return GT_INVALID;
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ������Ҹ�������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdatePersonalSet(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_PersonalSet);

	if(!m_pDB->UpdatePersonalSet(p->dwAccountID, &p->stPersonalSet))
	{
		return GT_INVALID;
	}

	return 0;
}
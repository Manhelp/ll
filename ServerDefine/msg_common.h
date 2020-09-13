//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_common.h
// author: 
// actor:
// data: 2009-03-25
// last:
// brief: �˺��½�ɫͨ�����������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//-----------------------------------------------------------------------------

#include "../WorldDefine/currency_define.h"
#include "../WorldDefine/RoleDefine.h"
//-----------------------------------------------------------------------------
// ��Ϣ�ṹ
//-----------------------------------------------------------------------------
CMD_START(NDBC_ChangeBagPsd)
	DWORD		dwAccountID;
	DWORD		dwNewBagPsd;
CMD_END

CMD_START(NDBC_SafeCodeSet)
	DWORD		dwAccountID;
	DWORD		dwSafeCodeCrc;
CMD_END

//CMD_START(NDBS_SafeCodeSet)
//	DWORD		dwErrorCode;
//CMD_END

CMD_START(NDBC_SafeCodeReset)
	DWORD		dwAccountID;
	DWORD		dwTimeReset;
CMD_END

CMD_START(NDBC_SafeCodeResetCancel)
	DWORD		dwAccountID;
CMD_END


CMD_START(NDBC_WareSizeUpdate)
	DWORD		dwAccountID;
	INT16		n16WareSize;
CMD_END

CMD_START(NDBC_WareMoneyUpdate)
	DWORD		dwAccountID;
	INT64		n64WareSilver;
CMD_END

CMD_START(NDBC_BaiBaoYBUpdate)
	DWORD		dwAccountID;
	INT			nBaiBaoYuanBao;
CMD_END

CMD_START(NDBC_BagYBUpdate)
	DWORD		dwRoleID;
	INT			nBagYuanBao;
	BOOL		bIncrement;
CMD_END

CMD_START(NDBC_BagGoldUpdate)
	DWORD		dwRoleID;
	INT				nBagGod;
	BOOL		bIncrement;
CMD_END

CMD_START(NDBC_PersonalSet)
	DWORD					dwAccountID;
	tagDWPlayerPersonalSet	stPersonalSet;
CMD_END

// ���½�ɫAvatar�沿ģ��ID
CMD_START(NDBC_SaveAvatarFaceAndHair)
	DWORD	dwRoleID;
	WORD	wHairMdlID;			// ����
	WORD	wHairTexID;			// ��ɫ
	WORD	wFaceMdlID;			// �沿���
	WORD	wFaceDetailTexID;	// �沿ϸ��
CMD_END

// �������ý�����������ͳ������
struct tagMediaOnlineNum
{
	INT			nDistributionID;
	DWORD	dwOnlineNum;
};
CMD_START(NDBC_SaveMediaOnlineNum)
	DWORD	dwNum;
	tagMediaOnlineNum pData[1];
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)

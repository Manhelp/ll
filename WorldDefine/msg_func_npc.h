//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_func_npc.h
// author: 
// actor:
// data: 2008-11-10
// last:
// brief: ְ��NPC�����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{	
	E_Dak_NPCNotFound		= 1,	// û���ҵ�ָ��NPC
	E_Dak_NPCNotValid		= 2,	// ������վNPC
	E_Dak_TooFar			= 3,	// ������վNPC̫Զ
	E_Dak_NotExist			= 4,	// ��ָ��NPC����û���ҵ���վ
	E_Dak_TargetMap_Invalid	= 5,	// ָ��������map id��ָ��id����
	E_Dak_NotEnough_Money	= 6,	// ����Ʒ����
	E_Dak_ChangeMap_Failed	= 7,	// ����ʱʧ��
	E_Dak_NoStone			= 8,	// û�д���ʯͷ

	E_Ware_NPCNotValid		= 10,	// ���ǽ�ɫ�ֿ�NPC
	
	E_AccountReactive_Error = 20,
	E_AccountReactive_AlreadyDone	= 21, // ���˺��Ѿ���ȡ��
	E_AccountReactive_CodeInvalid = 22, // key����Ч
	E_AccountReactive_IdentityLimit = 23, // ���Ǳ��������/���ǻ�����
	E_AccountReactive_Reward_ScoreLimit = 24, // �����˷�������
	E_AccountReactive_Reward_Already = 25, // �����˵�ǰ�ȼ��Ľ����Ѿ���ȡ��

	E_Con_Cannot_Extend		= 50,	// ����������
	
	E_Silver_NotEnough		= 60,	// ��Ǯ����
	E_YuanBao_NotEnough		= 61,	// Ԫ������

	E_Item_NotExist			= 70,	// ��Ʒ������
	E_Item_UseErr			= 71,	// ��Ʒʹ��ʧ��
};

//----------------------------------------------------------------------------
// ��վ&Ǭ��ʯ
//----------------------------------------------------------------------------
CMD_START(NC_Dak)
	DWORD	dwNPCID;	// ְ��NPC ID
	INT32	nIndex;		// ��վĿ�ĵ��ڸ�NPC������
	DWORD	dwMapID;	// Ŀ�ĵ�Map ID
CMD_END

CMD_START(NS_Dak)
	DWORD	dwErrorCode;
CMD_END


//----------------------------------------------------------------------------
// ��ɫ�ֿ�
//----------------------------------------------------------------------------
CMD_START(NC_WareExtend)
	DWORD	dwNPCID;
	INT64	n64Serial;		// true: ʹ����Ϸ�ң�false:ʹ��Ԫ��
CMD_END

CMD_START(NS_WareExtend)
	DWORD	dwErrorCode;
	INT16	n16WareNum;
CMD_END

CMD_START(NC_BagExtend)
	DWORD	dwNPCID;
	INT64	n64Serial;		// ��Ʒid
CMD_END

CMD_START(NS_BagExtend)
	DWORD	dwErrorCode;
	INT16	n16BagNum;
CMD_END

//----------------------------------------------------------------------------
// ��Ǯ&Ԫ��
//----------------------------------------------------------------------------
CMD_START(NC_SaveSilver)
	DWORD	dwNPCID;
	INT64	n64SilverSave;
CMD_END

CMD_START(NS_SaveSilver)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_GetSilver)
	DWORD	dwNPCID;
	INT64	n64SilverGet;
CMD_END

CMD_START(NS_GetSilver)
	DWORD	dwErrorCode;
CMD_END

//CMD_START(NC_SaveYuanBao)
//	DWORD	dwNPCID;
//	INT		nYuanBaoSave;
//CMD_END

CMD_START(NS_SaveYuanBao)
	DWORD	dwErrorCode;
CMD_END

CMD_START(NC_GetYuanBao)	// �Ӱٱ����л�ȡԪ��
	INT		nYuanBaoGet;
CMD_END

//CMD_START(NS_GetYuanBao)
//	DWORD	dwErrorCode;
//CMD_END


//----------------------------------------------------------------------------
// ʹ��ĥʯ
//----------------------------------------------------------------------------
CMD_START(NC_Abrase)
	INT64	n64AbraserSerial;	// ĥʯ64λid
	INT64	n64WeaponSerial;	// ����64λid
CMD_END

CMD_START(NS_Abrase)
	DWORD	dwErrorCode;
	INT64	n64WeaponSerial;	// ����64λid
	INT		nAttackTimes;		// ���º�����ʹ�ô���
CMD_END


//----------------------------------------------------------------------------
// ʹ��KeyCode NPC
//----------------------------------------------------------------------------

CMD_START(NC_KeyCode)
	DWORD		dwNPCID;
	char szKeyCode[X_LONG_NAME];
CMD_END

CMD_START(NS_KeyCode)
	int	 nErrorCode;
CMD_END

CMD_START(NC_NewKeyCode)
	DWORD		dwNPCID;
	char		szKeyCode[X_LONG_NAME];
CMD_END

CMD_START(NS_NewKeyCode)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NC_ReactiveKeyCode)
	DWORD		dwKeyCode;
CMD_END

CMD_START(NS_ReactiveKeyCode)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NS_PlsInputReactiveKeyCode)
CMD_END

// ��������ȡ����
// CMD_START(NC_RewardReactive)
// 	//BYTE byRewardLevel; // Ҫ��ȡ�����ļ���
// CMD_END
// 
// CMD_START(NS_RewardReactive)
// 	DWORD dwErrorCode;
// CMD_END
//----------------------------------------------------------------------------
#pragma pack(pop)
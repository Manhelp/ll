//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2008-09-04
// last:
// brief: ��Ʒ���
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------


#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/ItemDefine.h"
#include "item_define.h"

//-----------------------------------------------------------------------------
// ö��
//-----------------------------------------------------------------------------
enum
{
	
};

//-----------------------------------------------------------------------------
// ��Ϣ�ṹ
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetItemInfo)	// �����Ϸ������ȫ����Ʒ�����Ϣ
CMD_END;

CMD_START(NDBS_GetItemInfo)
	DWORD	dwDummy;
	INT64	n64MaxSerial;
	INT64	n64MinSerial;
CMD_END;

CMD_START(NDBC_NewItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_NewEquip)
	tagEquip	equip;
CMD_END;

CMD_START(NDBC_DelItem)
	INT64		n64Serial;
CMD_END;

// ע��ֻ��ɾ��һ��װ��
CMD_START(NDBC_DelEquipFromRole)
	DWORD		dwRoleID;
	DWORD		dwTypeID;
CMD_END;

CMD_START(NDBC_DelEquip)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_NewBaiBaoItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_NewBaiBaoEquip)
	tagEquip	equip;
CMD_END;

CMD_START(NDBC_DelBaiBaoItem)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_DelBaiBaoEquip)
	INT64		n64Serial;
CMD_END;

//CMD_START(NDBC_UpdateItemPos)	// ������Ʒλ��
//	DWORD		dwAccountID;
//	INT64		n64Serial;
//	DWORD		dwRoleID;
//	INT32		nUseTimes;		// ��Ʒ�ϼ���ʹ�ô���
//	INT16		n16Num;			// ��Ʒ����
//	INT16		n16Index;		// ������λ��
//	BYTE		byConType;		// ��������
//CMD_END;

CMD_START(NDBC_UpdateEquipAtt)	// ����װ�����ԣ���װ�����Ա�
	tagEquipSpecUpdate	equipSpecUpdate;
CMD_END;

CMD_START(NDBC_UpdateEquipSingleAtt)	// ����װ�������������ԣ���װ�����Ա�
tagEquipSingleSpec	equipSingleSpecUpdate;
CMD_END;

//CMD_START(NDBC_ReplaceItemCDTime)	// �洢��Ʒ��ȴʱ��
//	DWORD			dwRoleID;
//	INT32			nNum;			// ����
//	BYTE			byCDTime[1];	// tagCDTime
//CMD_END;

CMD_START(NDBC_EquipSignature)		// ����װ��ǩ��
	INT64			n64Serial;		// ������װ����64λid
	DWORD			dwSignRoleID;	// �����ߵ�id
	TCHAR			szContent[10];	// ��������
CMD_END;

//-----------------------------------------------------------------------
// ��Ʒ��������
//-----------------------------------------------------------------------
CMD_START(NDBC_LockItem)
	INT64			n64Item;		// Ҫ������Ʒ��64λID
	BYTE			byType;			// 0��δ����״̬��1������״̬
	DWORD			dwUnlockTime;	// ����ʱ��
CMD_END

//-----------------------------------------------------------------------
// ��Ʒ�������ߵ��µİ�
//-----------------------------------------------------------------------
CMD_START(NDBC_BindItem)
	INT64			n64Item;		// Ҫ����Ʒ��64λID
	BYTE			byType;			// 0��δ��״̬��1����״̬
CMD_END

//-----------------------------------------------------------------------------
// ѹ��ר��
//-----------------------------------------------------------------------------
CMD_START(NDBC_TestSaveItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_TestSaveEquip)
	tagEquip	equip; 
CMD_END;

// Jason 2010-4-14 v2.0.0 ��������޸�
CMD_START(NDBC_NewFabao)
	tagFabao	fabao;
CMD_END;
CMD_START(NDBC_UpdateFabaoAtt)	// ���·�������
	INT64			n64FabaoID;		// Ҫ���·�����64λID
	tagFabaoSpec	fabaoSpec;
CMD_END;

CMD_START(NDBC_UpdateHolyAtt)	// ����ʥ������
INT64			n64HolyID;		// Ҫ����ʥ���64λID
tagHolySpec		holySpec;
CMD_END;

CMD_START(NDBC_UpdateHolyEquipAtt)	// ����ʥ������
INT64			n64ID;				// Ҫ����ʥ�Ƶ�64λID
tagHolyEquipSpec		holyEquipSpec;
CMD_END;

// ��������仯
CMD_START(NDBC_FabaoMoodChanged)
	INT64	n64SerialID;
	INT		nMood;
	INT		nMoodTicks;
CMD_END

// ʥ��Ĭ��ֵ�仯
CMD_START(NDBC_HolyCoValue)
INT64	n64SerialID;
INT		nCoValue;
CMD_END

CMD_START( NDBC_UpdateFabaoEleInjuryType )
	INT64	n64FabaoID;
	EElementInjury eInjury;
CMD_END

// [wcy 2010-03-31] ʥ������޸�
CMD_START(NDBC_NewHoly)
tagHolyMan	holy;
CMD_END;

// [wcy 2010-04-18] ʥ������޸�
CMD_START(NDBC_NewHolyEquip)
tagHolyEquip	holyEquip;
CMD_END;

// ��������������
struct tagFabaoNirVanaData
{
	INT	nStage;		// �ȼ�
	INT	nEleInjuryType;	// Ԫ���˺�����
	INT	nEleInjury;		// Ԫ���˺�
	INT	nElePierce;		// Ԫ�ش�͸
	WORD		wEleInjPromoteTimes;		// �˺��������� [3/12/2012 zhangzhihua]	
	WORD		wEleResPromoteTimes;		// ������������ [3/12/2012 zhangzhihua]
	INT			nEleResistance[EER_End];	// Ԫ�ؿ��ԣ���Ԫ�ؿ���ö������������
	WORD		wDamageUpgradeTimes;				// �˺�����������
	WORD		wResistanceUpgradeTimes;			// ���Կ���������
	//DWORD		dwFairySkill[MaxFairySkillNum];		// �����ؼ���Ĭ��GT_INVALID	
	//DWORD		dwUnsureSkill;						// ��ȷ���ؼ���Ĭ��GT_INVALID
};
// ��¼�����ȼ���60�����ϵ��ؼ����Եȣ�ÿ10����¼һ��
CMD_START( NDBC_UpdateFabaoNirVanaData )
	INT64					_n64ID;		// ����serialnum
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	tagFabaoNirVanaData _data;
CMD_END

// �õ�������������
CMD_START( NDBC_GetFabaoNirVanaData )
	INT64			n64ID;
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	INT				nCurStage;		// ��ǰ�ȼ�
	BOOL			bFirstOrLast;	// ��һ����60�������򣬾���ȡ���һ����������һ����
	INT64		n64NirvanID;	// ������Ʒ
CMD_END
CMD_START( NDBS_GetFabaoNirVanaData )
	DWORD			dwErrorCode;
	INT64			n64ID;
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	tagFabaoNirVanaData data;
	INT64		n64NirvanID;	// ������Ʒ
CMD_END

// �״β���������Դ���
CMD_START( NDBC_UpdateFabaoGivenBirthTimes )
	INT64					_n64ID;		// ����serialnum
	INT						nTimes;
CMD_END

// ��ȡϡ����Ʒ��Ϣ
CMD_START( NDBC_LoadGlobalLimitedItem )
CMD_END

CMD_START( NDBS_LoadGlobalLimitedItem )
	INT32 nNum;
	tagGlobalLimitedItem data[1]; 
CMD_END

// ����ϡ����Ʒ��Ϣ
CMD_START( NDBC_SaveGlobalLimitedItem )
	tagGlobalLimitedItem data;
CMD_END

CMD_START( NDBC_UnSaveGlobalLimitedItem )
	DWORD dwItemID;
	INT64 n64Serial;
CMD_END

//CMD_START( NDBC_CreateGlobalLimitedItem )
//	tagGlobalLimitedItem item;
//CMD_END
CMD_START( NDBC_UpdateGlobalLimitedItem )
	tagGlobalLimitedItem item;
	INT64	nOriginal64ID;
CMD_END

//CMD_START( NDBC_DeleteGlobalLimitedItem )
//	tagGlobalLimitedItem item;
//CMD_END

//CMD_START( NDBC_DelGlobalLimitedItem )
//	DWORD dwItemID;
//	INT64 n64Serial;
//CMD_END

CMD_START(NDBC_NewSoulCrystal)
	tagSoulCrystal	obj;
CMD_END;
CMD_START(NDBC_UpdateSoulCrystal)	// ���·�������
	INT64			n64ID;			// Ҫ���·�����64λID
	tagSoulCrystalSpec	objspec;
CMD_END;

CMD_START(NDBC_KeyCodeAddGift)
	DWORD dwRoleID;
CMD_END

CMD_START(NDBS_KeyCodeAddGift)
	DWORD dwRoleID;
	BOOL bIsSuccessful;
CMD_END

CMD_START(NDBC_ReceiveYuanBao)
	DWORD dwRoleID;
CMD_END

CMD_START(NDBS_ReceiveYuanBao)
	DWORD dwRoleID;
	int nNum;
CMD_END
//-----------------------------------------------------------------------------
#pragma pack(pop)

//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2008-07-21
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- ��Ʒ/װ�����
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "ItemDefine.h"
#include "AvatarAtt.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	// �ò��ַŵ�msg_common_errorcode.h�У��Ա��ģ��ͨ��
	//E_Item_LevelLimit				= 9,		// �ȼ�������Ҫ��
	//E_Item_SexLimit					= 10,		// �Ա�����Ҫ��
	//E_Item_ClassLimit				= 11,		// ְҵ������Ҫ��
	//E_Item_TalentLimit				= 12,		// ���ʲ�����Ҫ��
	//E_Item_ZoneRepLimit				= 13,		// ��������������Ҫ��
	//E_Item_OtherRepLimit			= 14,		// ��������������Ҫ��
	//E_Item_RoleStatusLimit			= 15,		// ���״̬(ս������ս��)������Ҫ��
	//E_Item_AttA_Limit				= 16,		// һ�����Բ�����

	E_Equip_OnFailed				= 100,		// װ��ʧ��
	E_Equip_NotRing					= 101,		// װ������ring
	E_Equip_InvalidPos				= 102,		// װ����λ��װ������Ҫ����λ����
	E_Equip_NotRingPos				= 103,		// װ����λ���ƶ���Ŀ��λ�ò���ring��λ
	E_Equip_SamePos					= 104,		// Ŀ��λ���뵱ǰλ��ͬ
	E_Equip_NotIdentify				= 105,		// װ��û�м���
	E_Equip_WeaponPos_Overlap		= 106,		// ����λ���ظ�
	E_Equip_Idendtifed				= 107,		// �����Ѽ�����
	E_Equip_Guard_DeadPunish		= 108,		// װ�����������ػ��������ԣ���ɫ����������
	E_Equip_Lock					= 109,		// ������Ʒ
	E_Equip_VocationLimit			= 110,		// ��ְҵ����װ����װ��
	E_Equip_CondenseLimit			= 111,		// ������ƣ�����װ��

	E_BagPsd_Exist					= 150,		// ��������
	E_BagPsd_NoExist				= 151,		// δ������
	E_BagPsd_SafeCode_Error			= 152,		// ��ȫ�����
	E_BagPsd_Error					= 153,		// �������
	E_BagPsd_OK						= 154,		// ������ȷ

    E_Dye_OtherItem                 = 170,      // ֻ�ܶ�ʱװ����Ⱦɫ
    E_Dye_Exist                     = 171,      // ��ʱװ��Ϊ����ɫ������Ⱦɫ
    E_Dye_LvLimit                   = 172,      // Ⱦ�ϵȼ�����
	E_Dye_NotExist					= 173,		// װ������Ⱦ�ϲ�����

	E_GoldStone_Success				= 180,		// ���ʯ�����ɹ�
	E_GoldStone_SrcItemNotExist		= 181,		// ���ʯԭ�ϲ�����
	E_GoldStone_NoConfigFile		= 182,		// ���ʯ�������������ļ�������
	E_GoldStone_CreateDestItemFail	= 183,		// ������Ʒʧ��
	E_GoldStone_MissingMatch		= 184,		// ���㻯��Ʒ��������ʯ��ƥ��
	E_GoldStone_OtherError			= 185,		// ����δ֪����
	E_GoldStone_BagIsFull			= 186,		// ��������

	E_FabaoFeeding_NoFood			= 187,		// û������ʳ��
	E_FabaoFeeding_NoFabao			= 188,		// û���������
	E_FabaoFeeding_FabaoLevelLimit	= 189,	// �����ȼ�����30��
	E_FabaoFeeding_FoodLimit		= 190,		// �����ȼ���ʳ�ﲻƥ��
	E_FabaoFeeding_Full				= 191,		// ����������������Ҫ��ʳ��

	E_FabaoStoreExp_LvlLimit			= 192,	// ����������40�������ܽ�������������ز���
	E_FabaoStoreExp_NoExpBall		= 193,  // �����򲻴���
	E_FabaoStoreExp_NoFabao			= 194,	// û���������
	E_FabaoStoreExp_NoEnoughExp		= 195,	// ������ӡ����ֵ����
	E_FabaoStoreExp_BagFull			= 196,	// ��������
	E_FabaoStoreExp_BallBinded		= 197,	// ˮ�����Ѿ�Լ��
	E_FabaoStoreExp_NoExpPililar		= 198,	// ���鵤������
	E_FabaoStoreExp_NotYourExpPililar		= 199,	// ���鵤���������
	E_FabaoStoreExp_ExpPililarUseTimesLimit		= 200,	// ���鵤ʹ�ô�������20��

	E_FabaoMating_RoleNotOnline					= 201,	// �Է�������
	E_FabaoMating_QltyLow							= 202,	// Ʒ�������ɫ
	E_FabaoMating_QualLow							= 203,	// ����̫��
	E_FabaoMating_CannotMating					= 204,	// �������
	E_FabaoMating_BirthTimesLow					= 205,	// ��Դ�������
	E_FabaoMating_LevelLow						= 206,	// �����ȼ�����
	E_FabaoMating_OtherBagFull					= 207,	// �Է�������
	E_FabaoMating_NotInGiveBirth					= 208,	// ��û�н���������
	E_FabaoMating_CannotPutIn_ErrStatus			= 209,	// ����״̬���ԣ����ܷ���
	E_FabaoFeeding_PregnancyTimeZero				= 210,	// ��������ʱ��Ϊ0����Ҫ�ټ���
	E_FabaoFeeding_RestTimeZero					= 211,	// ������Ϣʱ��Ϊ0����Ҫ�ټ���
	E_FabaoFeeding_MatingPregnancyCDTimeLT1h	= 212,	// ��������ʱ�����1Сʱ
	E_FabaoFeeding_MatingRestCDTimeLT1h			= 213,	// ������Ϣʱ�����1Сʱ
	E_FabaoMating_DistanceTooFar					= 214,	// ������ԣ�����̫Զ��hh
	E_FabaoMating_NotOnline						= 215,	// ������ԣ��Է�������

	E_FabaoFeeding_NotBirthFabao				= 216, // ���ǿ���������������������������������

	E_EquipGrow_ClipsInvalid	=	217, // ������Ƭ��Ч
	E_EquipGrow_TimesInvalid	=	218, // ����ĳɳ��������Ϸ�
	E_EquipGrow_ReachTop		= 219, // �޷������ɳ�
	E_EquipGrow_Error	= 220, // ����δ֪����
	E_EquipGrow_EquipInvalid = 221,

	E_EquipGrowTrans_OldEquipInvalid = 222,
	E_EquipGrowTrans_NewEquipInvalid = 223,
	E_EquipGrowTrans_PositionNoMatch = 224,
	E_EquipGrowTrans_NewEquipLevelLimit = 225,
	E_EquipGrowTrans_IMInvalid = 226,
	E_EquipGrowTrans_Error = 227,

	E_EquipGrowDec_EquipInvalid = 230,
	E_EquipGrowDec_IMInvalid = 231,
	E_EquipGrowDec_NoStar = 232,

	E_EquipPerfectGrow_IMInvalid = 240,		//���߲����ڻ������Ͳ���
	E_EquipPerfectGrow_EquipInvalid = 241,	//װ�������ڻ���װ��û�гɳ�����
	E_EquipXiStar_IMInvalid	= 242,			//���߲����ڻ������Ͳ���
	E_EquipXiStar_EquipInvalid = 243,		//װ�������ڻ���װ�����Ͳ���
	E_EquipXiStar_NumNotEnough = 244,		//������������
	E_EquipXStar_TimeLimitEquip = 245,		//ʱ��װ��
	E_EquipPerfectGrow_TimeLimit = 246,		//ʱ��װ��

	E_NotHolyStrengthItem = 250,			//����ʥ���������ɵȼ��ĵ���
	E_NotHolyItem		  = 251,			//����ʥ��
	E_HolyEatNumMax		  = 252,			//��������ˣ�����������

	E_NotHolyEquipmentNumber = 253,			//��������ʥ���װ��ʥ�������ĵ���
	E_HolyEquipmentNumberMax = 254,			//ʥ���װ��ʥ�������Ѵﵽ���
};

//----------------------------------------------------------------------------
// װ�����������Ϣ(ע��ṹ���ֶ�˳����ñ�֤�ڴ����)
//----------------------------------------------------------------------------
CMD_START(NC_Equip)	// װ��
	EEquipPos	ePosDst;		// װ����λ(ע���ָװ����λ���ͻ��������жϴ���)
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NS_Equip) // ����װ����������Ӱ����������Ϣ����
	DWORD		dwErrorCode;
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NC_Unequip)
	EEquipPos	eType;			// ����λ��
	INT16		n16PosDst;		// �����뱳����λ��
	INT16		n16Dummy;		// ������
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NS_Unequip)
	EEquipPos	eType;			// ����λ��
	DWORD		dwErrorCode;
	INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NC_SwapWeapon)
CMD_END

CMD_START(NS_SwapWeapon)
	DWORD		dwErrorCode;
CMD_END

CMD_START(NS_AvatarEquipChange)	// ������۱仯
	DWORD			dwRoleID;
	tagEquipDisplay	EquipDisplay;	// װ�����
CMD_END

CMD_START(NS_EquipEffectChange)	// ��������Ч���仯
	DWORD			dwRoleID;
	BYTE			byDisplayPos;	// ģ��λ��
	BYTE			byEquipEffect;	// ����Ч��
CMD_END

CMD_START(NC_IdentifyEquip)	// ��������
	INT64		n64SerialReel;	// ��������
	INT64		n64SerialEquip;	// װ��
CMD_END

CMD_START(NS_IdentifyEquip)	// ��������
	DWORD		dwErrorCode;
	INT64		n64Serial;
CMD_END

CMD_START(NS_EquipChange)
	INT64			n64Serial;
	tagEquipSpec	equipSpec;
CMD_END

CMD_START(NS_EquipSingleChange) //�������Ա仯
tagEquipSingleSpec			equipSingleSpec;
CMD_END


CMD_START(NS_SuitEffect)	// ��װ��Ч�ı�
	DWORD		dwRoleID;
	DWORD		dwSuitEffectID;
CMD_END

CMD_START(NS_SuitNum)		// ��װ�����ı�
	DWORD		dwSuitID;
	INT8		n8Num;
	INT8		n8Dummy[3];
CMD_END

//----------------------------------------------------------------------------
// ��Ʒλ�÷����仯
//----------------------------------------------------------------------------
CMD_START(NC_ItemPosChange) // ��Ʒλ���ƶ� -- ͬһ��������
	EItemConType	eConType;		// ��������
	INT64			n64Serial;		// �ƶ�λ�õ���Ʒ
	INT16			n16Num;			// �ƶ���Ʒ�ĸ���(0��ʾȫ���ƶ�)
	INT16			n16PosDst;		// ��Ʒ�ƶ���Ŀ��λ��
	DWORD			dwNPCID;		// ְ��NPC ID(����ֻ�вֿ�ʹ��)
CMD_END

CMD_START(NS_ItemPosChange) // ��Ʒλ���ƶ� -- ͬһ��������
	EItemConType	eConType;		// ��������
	INT64			n64Serial1;		// �ƶ�λ�õ���Ʒ1
	INT64			n64Serial2;		// ��Ӱ����Ʒ2, ��ΪGT_INVALIDʱ, ��ʾ����Ʒ1�ƶ���һ����λ
	INT16			n16PosDst1;		// ��Ʒ1�ƶ���Ŀ��λ��
	INT16			n16PosDst2;		// ��Ʒ2�ƶ���Ŀ��λ��
	INT16			n16Num1;		// ��Ʒ1���ո��� -- ����ֵΪ0ʱ,�轫����Ʒɾ��,������������Ϣ
	INT16			n16Num2;		// ��Ʒ2���ո���
	BOOL			bCreateItem;	// n64Serial2�Ƿ�Ϊ�´�������Ʒ�� -- Ϊ�½���ʱ����ͻ��˴���������������Ϣ
CMD_END

CMD_START(NC_ItemPosChangeEx) // ��Ʒλ���ƶ� -- ��ͬ������
	EItemConType	eConTypeSrc;	// Դ��������
	INT64			n64Serial1;		// �ƶ�λ�õ���Ʒ1
	EItemConType	eConTypeDst;	// Ŀ����������
	INT16			n16PosDst;		// ��Ʒ1�ƶ���Ŀ��λ��,ΪGT_INVALIDʱ��ʾû��ָ��Ŀ��λ��
	INT16			n16Dummy;
	DWORD			dwNPCID;		// ְ��NPC ID(����ֻ�вֿ�ʹ��)
CMD_END

CMD_START(NS_ItemPosChangeEx) // ��Ʒλ���ƶ� -- ��ͬ������
	EItemConType	eConTypeSrc1;	// �������� -- n64Serial1��ԭ����
	EItemConType	eConTypeSrc2;	// �������� -- n64Serial2��ԭ����
	EItemConType	eConTypeDst1;	// �������� -- n64Serial1��Ŀ������
	EItemConType	eConTypeDst2;	// �������� -- n64Serial2��Ŀ������
	INT16			n16PosDst1;		// ��Ʒ1�ƶ���Ŀ��λ��
	INT16			n16PosDst2;		// ��Ʒ2�ƶ���Ŀ��λ��
	INT64			n64Serial1;		// �ƶ�λ�õ���Ʒ1, ��ΪGT_INVALIDʱ, ��ʾ��Ʒ2�ƶ���һ����λ
	INT64			n64Serial2;		// �ƶ�λ�õ���Ʒ2, ��ΪGT_INVALIDʱ, ��ʾ��Ʒ1�ƶ���һ����λ
	INT16			n16Num1;		// ��Ʒ1���ո��� -- ����ֵΪ0ʱ,�轫����Ʒɾ��,������������Ϣ
	INT16			n16Num2;		// ��Ʒ2���ո���
CMD_END

CMD_START(NC_ItemReorder)	// ��������
	EItemConType	eContainerType;	// ��������
	DWORD			dwNPCID;		// GT_INVALIDʱ��Ϊ�ֿ���������Ϊ��������
	INT16			n16ItemNum;		// ��Ʒ����
	INT16			n16Index[1];	// ����ǰ��Ʒ�������е�λ�ã���ǰ�����±�Ϊ�����˳��
CMD_END

CMD_START(NS_ItemReorder)
	DWORD			dwErrorCode;
	EItemConType	eConType;		// ��������
	INT16			n16ItemNum;		// ��Ʒ����
	INT16			n16OldIndex[1];	// ��ǰ�����±�Ϊ�����˳��
CMD_END

CMD_START(NC_ItemReorderEx)	// ��������
	EItemConType	eContainerType;	// ��������
	DWORD			dwNPCID;		// GT_INVALIDʱ��Ϊ�ֿ���������Ϊ��������
	INT16			n16ItemNum;		// ��Ʒ����
	BYTE			byData[1];		// tagItemOrder
CMD_END

CMD_START(NS_ItemReorderEx)
	DWORD			dwErrorCode;
	EItemConType	eConType;		// ��������
	INT16			n16ItemNum;		// ��Ʒ����
	BYTE			byData[1];		// tagItemOrder
CMD_END


//----------------------------------------------------------------------------
// ��Ʒλ�÷����仯
//----------------------------------------------------------------------------
CMD_START(NC_GroundItem)
	INT64			n64Serial;
CMD_END

CMD_START(NS_ItemAdd)
//	DWORD			dwErrorCode;
	INT64			n64Serial;
	EItemConType	eConType;
	INT16			n16Index;		// ������λ���±�
	INT16			n16Num;			// ����ǿɶѵ���Ʒ,��Ϊ�ö�����
	bool			bOverlap;		// �ص���Ʒ�Ƿ�������жѣ����true���迼���ͷ��ڴ�
CMD_END

CMD_START(NS_NewItemAdd)
	tagItem			Item;
CMD_END

CMD_START(NS_NewEquipAdd)
	tagEquip		Equip;
CMD_END

CMD_START(NC_ItemRemove)
	EItemConType	eConType;
	INT64			n64Serial;
CMD_END

CMD_START(NS_ItemRemove)
//	DWORD			dwErrorCode;
	EItemConType	eConType;
	INT64			n64Serial;
	INT16			n16Index;
	INT16			n16Num;			// ����ǿɶѵ���Ʒ,��Ϊʣ�����,Ϊ0��ʾȫ��ɾ��
CMD_END

CMD_START(NS_ItemCDUpdate)
	DWORD			dwTypeID;
	DWORD			dwCDTime;
CMD_END;


//----------------------------------------------------------------------------
// ���Ҽ������
//----------------------------------------------------------------------------
CMD_START(NC_SetBagPsd)		// ��������
	DWORD	dwBagPsdCrc;	// �����õ���������crcֵ
CMD_END;

//CMD_START(NS_SetBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_UnsetBagPsd)	// ȡ������
	DWORD	dwSafeCodeCrc;	// ��ȫ���crcֵ
CMD_END;

//CMD_START(NS_UnsetBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_OldBagPsd)		// ������
	DWORD	dwOldBagPsdCrc;
CMD_END;

//CMD_START(NS_OldBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_ResetBagPsd)	// �޸�����
	DWORD	dwOldBagPsdCrc;
	DWORD	dwNewBagPsdCrc;
CMD_END;

//CMD_START(NS_ResetBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NC_OpenBagPsd)	// �����ܣ��򿪱���ʱ���ȷ��͸���Ϣ
	DWORD	dwBagPsdCrc;
CMD_END;

//CMD_START(NS_OpenBagPsd)
//	DWORD	dwErrorCode;
//CMD_END;

CMD_START(NS_BagPsd)		// �������������Ϣ����
	DWORD	dwErrorCode;
	DWORD	dwRoleStateEx;	// ֻ������Լ�֪����״̬
CMD_END;


//----------------------------------------------------------------------------
// ��������ո�¶ȸ���
//----------------------------------------------------------------------------
CMD_START(NS_RWNewessChange)
	INT32	nAttackTimes;		// ����������ǰʹ�ô���
CMD_END

//----------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------
CMD_START(NS_LongHunOn)		// ָ����λװ�������������Ѽ���
	INT16	n16EquipPos;
CMD_END

CMD_START(NS_LongHunOff)	// ָ����λװ������������δ����
	INT16	n16EquipPos;
CMD_END

//----------------------------------------------------------------------------
// ʹ��Ⱦ��Ϊʱװ��ɫ���
//----------------------------------------------------------------------------
CMD_START(NC_DyeFashion)
    INT64   n64DyeSN;       // Ⱦ��64λid
    INT64   n64EquipSerial; // ʱװװ��64λid
CMD_END

CMD_START(NS_DyeFashion)
    DWORD   dwErrCode;      // ������
CMD_END

//----------------------------------------------------------------------------
// �ٱ�����¼���
//----------------------------------------------------------------------------
CMD_START(NC_InitBaiBaoRecord)	// ����������ͳ�ʼ���ٱ�����¼����
CMD_END

CMD_START(NS_InitBaiBaoRecord)	// �������������е���Ϣ
	INT16	n16Num;				// ��¼����
	BYTE	byData[1];			// tagBaiBaoRecord�����ȼ���ȡn16Size����Ҫ��sizeof
CMD_END

CMD_START(NS_SingleBaiBaoRecord)// �������ʱ�����������͵����ٱ�����¼
	tagBaiBaoRecord	sRecord;	// ������¼
CMD_END

//----------------------------------------------------------------------------
// ������ֿ�
//----------------------------------------------------------------------------
CMD_START(NC_WareOpen)
	INT64	n64ItemID;
CMD_END

CMD_START(NS_WareOpen)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
//	���ʯ
//-----------------------------------------------------------------------------
CMD_START(NC_BeGoldStone)
	INT64		n64SrcItemID;	// ���ʯ
	INT64		n64DstItemID;	// ���ʯҪ��������Ʒ
CMD_END
CMD_START(NS_BeGoldStone)
	DWORD		dwErrCode;		// ���ʯ�������
CMD_END

//-----------------------------------------------------------------------------
//	��Ʒ��
//-----------------------------------------------------------------------------
CMD_START(NS_ItemBind)
	EItemConType	eConType;
	INT64			n64Serial;
CMD_END

//-----------------------------------------------------------------------------
//	������Ʒ��״̬
//-----------------------------------------------------------------------------
CMD_START(NS_ItemBindStatus)
	EItemConType	eConType;
	INT64			n64Serial;
	BYTE			byStatus;
CMD_END

// Jason 2010-4-15 v2.0.0
// ����������ز��������ߵ��ߺ�װ����Щ���̣����Э�鲿�֣�û�е������֣��������չ��
CMD_START(NS_NewFabaoAdd)
	tagFabao		fabao;
CMD_END
CMD_START(NS_FabaoChange)
	INT64			n64Serial;
	tagFabaoSpec	fabaoSpec;
CMD_END
// ��������仯֪ͨ��Ϣ
CMD_START(NS_FabaoMoodChanged)
	INT64			n64Serial;		// ��ţ���������װ�����ϵ�
	INT				nOldMood;
	INT				nNewMood;
CMD_END
CMD_START(NC_FabaoFeeding)
	DWORD			dwFoodTypeID;	// ����ʳ���typeid������ƷҪ����ұ�����
CMD_END
CMD_START(NS_FabaoFeeding)
	DWORD			dwErrorCode;	
	DWORD			dwBuffID;		// �ɹ�ιʳ��buffid����ЧʱΪGT_INVALID
CMD_END

// wcy 2012-04-01
CMD_START(NS_NewHolyAdd)
tagHolyMan		holy;
CMD_END

// wcy 2012-04-18
CMD_START(NS_NewHolyEquipAdd)
tagHolyEquip		holyEquip;
CMD_END

// ��Ʒ��̬����ֵ
CMD_START(NS_ItemSpecValChanged)
	INT64	n64Serial;	// ��Ʒ���
	DWORD	dwSpecVal1;// ��Ʒ��̬����ֵ1
	DWORD	dwSpecVal2;// ��Ʒ��̬����ֵ2
CMD_END

// ͬ�����������ӡ��Ϣ
CMD_START(NC_SyncFaBaoStoreExpInfo)
	BOOL bStoreExpOpen;			// ���������ӡ�Ƿ񼤻�
CMD_END

CMD_START(NS_SyncFaBaoStoreExpInfo)
	BOOL bStoreExpOpen;			// ���������ӡ�Ƿ񼤻�
	INT nCurStoreExp;					// ��ǰ��ӡ�ľ���
CMD_END

// ������ת��Ϊ���鵤
CMD_START(NC_ExpBall2ExpPilular)
	INT64 n64ExpBallSerialID;				// ���������ID
	DWORD dwExpPilularID;			// ���鵤����ID
CMD_END

CMD_START(NS_ExpBall2ExpPilular)
	DWORD dwErrorCode;
CMD_END

// ������Լ��
CMD_START(NC_ExpBallBind2Role)
	INT64 n64ExpBallSerialID;				// ���������ID
CMD_END

CMD_START(NS_ExpBallBind2Role)
	DWORD dwErrorCode;
CMD_END

//////////////////////////////////////////////////////////////////////////
//	������ֳ
//////////////////////////////////////////////////////////////////////////

// �鿴��������״̬
CMD_START(NC_InquireSpirteBornState)
	INT64 n64ExpBallSerialID;				// ����64ID
CMD_END

CMD_START(NS_InquireSpirteBornState)
	EFabaoStatus	eFabaoStatus;					// ����״̬
	DWORD			dwtagDwordTime;			// ״̬ת��ʣ��ʱ�䣨��û�д�0��
CMD_END

// �����������
CMD_START(NC_ApplyFairyBirth)
	DWORD	dwRoleID;				// ��ĳ�������������
CMD_END
// ������ת�͸��Է�
CMD_START(NS_TransmitApplyFairyBirth)
	DWORD	dwApplicantID;			// ������id
CMD_END
// �Է���Ӧ��
CMD_START(NC_ReplyFairyBirth)
	DWORD	dwApplicantID;			// ������
	BOOL	bAgreeOrNot;			// �Ƿ�ͬ��
CMD_END
// ������ת�͸�������
CMD_START(NS_TransmitReplyFairyBirth)
	DWORD	dwInvitee;				// ��������
	DWORD	dwErrorCode;			// �Ƿ�ͬ��
CMD_END


// ������
CMD_START(NC_PutFairyInto)
	INT64		n64FairyID;			// 64λid
CMD_END
// ������Ӧ��
CMD_START(NS_PutFairyInto)
	DWORD dwErrorCode;
CMD_END
// ת�ݶԷ���������Ҫ����
CMD_START(NS_TransmitPutFairyInto)
	BYTE		byQlty;					// Ʒ��
	INT			nBirthTimes;			// ��Դ�������������
	INT			nQualification;		// ����
	DWORD		dwFairyTypeID;			// ������typeid
CMD_END


// ȷ�����
CMD_START(NC_FairyMating)
	BOOL		bAgreement;			// �Ƿ�
CMD_END
CMD_START(NS_TransmitFairyMating)
	DWORD		dwRoleID;				// �Է�id
	BOOL		bAgreement;			// �Ƿ�
CMD_END
CMD_START(NS_FairyMating)				// ֪ͨ˫����ʼ����
CMD_END


// ȡ��������Ե����в���
CMD_START(NC_FairyMatingCancelAll)
CMD_END

CMD_START(NS_FairyMatingCancelAll)
CMD_END

// ��������
CMD_START(NC_FairyBorn)
	DWORD		dwNPCID;
	INT64			n64FairyID;			// 64λid
CMD_END
CMD_START(NS_FairyBorn)
	DWORD		dwErrCode;
CMD_END

// �˺Żع�
CMD_START(NS_LiLianChange)
	DWORD dwExp;	//�����ı���
CMD_END

CMD_START(NC_GetLiLianExp)
CMD_END

CMD_START(NS_GetLiLianExp)
	DWORD dwExp;		//�ܵ�����ֵ
	DWORD dwReceiveNum;	//��ȡ�������
CMD_END

CMD_START(NC_ReceiveGift)
CMD_END

CMD_START(NS_ReceiveGift)
	DWORD dwErrcode;
CMD_END

// �꾧
CMD_START(NS_NewSoulCrystalAdd)
	tagSoulCrystal		hunjing;
CMD_END

CMD_START(NS_SoulCrystalChg)
	INT64 n64Serial;
	tagSoulCrystalSpec info;
CMD_END

// ʥ��
CMD_START(NS_ShengLingChg)
	INT64 n64Serial;
	tagHolySpec info;
CMD_END

// ʥ��Ĭ��ֵ����
CMD_START(NS_ShengLingCoValueChg)
INT	nCoValue;
CMD_END

//����Key��
CMD_START(NC_GetKeyCodeGift)
	DWORD dwKeyCodeCrc;
CMD_END

CMD_START(NS_GetKeyCodeGift)
	DWORD dwErrorCode;
CMD_END

//�����ں�
CMD_START(NC_FairyFusion)
	INT64 n64FirstFairy;
	INT64 n64SecondFairy;
	INT64 n64IMID;
	BYTE byItemNum;
CMD_END

CMD_START(NS_FairyFusion)
	DWORD dwErrorCode;
CMD_END

CMD_START(NS_FairyFusionResult)
	tagFabao FirstFairy;	//���ɵ�����
CMD_END

CMD_START(NC_GetFusionFairy)
	bool bWhichOne;		//true��ʾѡ���һֻ��false��ʾѡ��ڶ�ֻ
CMD_END

CMD_START(NS_GetFusionFairy)
	DWORD dwErrorCode;
CMD_END

// ʥ����������
CMD_START(NC_SLEatFairy)
	INT64 n64Fairy;	//����64λid����������ߵ�64λId
CMD_END

CMD_START(NS_SLEatFairy)
	DWORD dwErrorCode;	//������
CMD_END

//ʥ���ٻ���Ϣ
CMD_START(NC_CallShengLing)
CMD_END

//���һ��ȡ���ٻ�ʥ����Ϣ
CMD_START(NC_CancelShengLing)
CMD_END

CMD_START(NS_CallShengLing)
	DWORD dwPlayID;
	DWORD dwShengLingTypeID;
	DWORD dwEatFairyNum;
	DWORD dwErrorCode;
CMD_END

CMD_START(NS_CallShengLingDisappear)
	DWORD dwPlayerID;
CMD_END

// ��ʥ�鴩װ��
CMD_START(NC_EquipHoly)
INT64 n64Equip;	//ʥ��64λid
CMD_END

CMD_START(NS_EquipHoly)
DWORD dwErrorCode;	//������
CMD_END

// ��ʥ��жװ��
CMD_START(NC_UnEquipHoly)
	BYTE byIndex;
	DWORD dwTypeID;
CMD_END

CMD_START(NS_UnEquipHoly)
DWORD dwErrorCode;	//������
CMD_END

CMD_START(NC_ChongNeng)
	BYTE byIndex;			//װ������
	INT64 n64ChongNeng;		//���ܵ���
	INT64 n64PianXiang;		//ƫ�����
	BYTE byPianXiangIndex;	//ƫ����������ֵ
CMD_END

CMD_START(NS_ChongNeng)
	DWORD dwErrorCode;
	BYTE byAtt;		//��������
	DWORD dwAttValue;	//����ֵ
CMD_END


CMD_START(NC_LingNeng)
	BYTE byIndex;		//װ������
	INT64 n64ItemId;	//����ǿ������
CMD_END

CMD_START(NS_LingNeng)
	DWORD dwErrorCode;
	DWORD dwValue;		//���ܼ���ֵ
CMD_END

CMD_START(NC_ChongNengResult)
	bool isConfirm;	//�Ƿ�ȷ��
	bool isLingNeng;	//�Ƿ������ܳ���
CMD_END
// ʥ��
CMD_START(NS_HolyEquipChg)
INT64 n64Serial;
tagHolyEquipSpec info;
CMD_END

// �洢����ֻ���
CMD_START(NC_SaveTelNum)
INT64 n64TelNum;	//����ֻ���
CMD_END


// ��Ҳ��洢�ֻ���
CMD_START(NC_NotSaveTelNum)
CMD_END

// ��ȡʥ�Ҿ�
CMD_START(NC_ReceiveYuanBao)
CMD_END

CMD_START(NS_ReceiveYuanBao)
	DWORD dwErrorCode;
CMD_END

#pragma pack(pop)

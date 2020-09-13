//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: container_define.h
// author: 
// actor:
// data: 2008-8-4
// last:
// brief: ����װ���������ҡ��ֿ����ض���
//--------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
const INT32 MAX_BAG_NUM				= 5;							// �������ҳ��
const INT32 SPACE_ONE_BAG			= 40;							// ������������
const INT32 SPACE_ALL_BAG			= SPACE_ONE_BAG * MAX_BAG_NUM;	// ��󱳰�����

const INT32 SPACE_QUEST_BAG			= 40;							// ������Ʒ��������	
const INT32 SPACE_BAIBAO_BAG		= 20;							// �ٱ���������
const INT32 SPACE_WAR_BAG			= 9;							// ר������������ [4/25/2012 zhangzhihua]

const INT32 MAX_BAIBAO_LOG_NUM		= 40;							// �ٱ������Log��

const INT32 MAX_WARE_NUM			= 6;							// �ֿ����ҳ��
const INT32 SPACE_ONE_WARE			= 40;							// �����ֿ����
const INT32 SPACE_ALL_WARE			= SPACE_ONE_WARE * MAX_WARE_NUM;// ���ֿ����

const INT32 MIN_PASSWORD_NUM		= 6;							// ������С����
const INT32 MAX_PASSWORD_NUM		= 14;							// ������󳤶�

const INT32 SPACE_WARE_PER_EXTEND	= 8;							// ��ɫ�ֿ�һ�����������
const INT32 SPACE_BAG_PER_EXTEND	= 8;							// ����һ�����������

const INT32 MAX_WARE_EX_TIMES_SILVER= 4;							// ʹ����Ϸ�����������Ĳֿ����
const INT32 MAX_BAG_EX_TIMES_SILVER	= 4;							// ʹ����Ϸ�����������ı�������

const INT32 MAX_GUILDWARE_NUM       =   5;                          // ���ɲֿ����ҳ��
const INT32 SPACE_ONE_GUILDWARE     =   35;                         // �������ɲֿ����
const INT32 SPACE_ALL_GUILDWARE     =   SPACE_ONE_GUILDWARE * MAX_GUILDWARE_NUM;

//Ԫ�����
//const INT32 HOLY_EQUIP_BLANK = 10;
//ʥ��װ��������������
const INT32 SHENGLING_EQUIP_BLANK = 60;

//-----------------------------------------------------------------------------
// ö�ٶ���
//-----------------------------------------------------------------------------
// ��������
enum EItemConType
{
	EICT_Start		= 0,
	EICT_Null		= 0,

	EICT_Bag		= 1, // ����
	EICT_Quest		= 2, // ������Ʒ��
	EICT_Baibao		= 3, // �ٱ���
	EICT_RoleWare	= 4, // ��ɫ�ֿ�
	EICT_Equip		= 5, // װ����

	EICT_Shop		= 6, // �̵�
	EICT_Ground		= 7, // ����

    EICT_PetEquip   = 8, // ����װ����

	EICT_GuildWare	= 9, // ���ɲֿ�

	EICT_Wardrobe = 10, // �³�

	EICT_Family = 11, // ��������

	EICT_TempBag	= 12,// ��ʱ��������������ںϲ����������� [3/29/2012 zhangzhihua]

	EICT_HolyEquip	= 13, //Ԫ��װ����

	EICT_HolyManEquip	= 14, //ʥ��װ����



	EICT_WarBag		= 15,// ר������ [4/25/2012 zhangzhihua]

	EICT_End		= 16,
};

// װ����
enum EEquipPos	// װ����λ
{
	//EEP_Null			= -1,
	//EEP_Start			= -1, 

	//EEP_Equip_Start		= 0,
	//EEP_Head			= 0, // ͷ��
	//EEP_Neck			= 1, // ����
	//EEP_Body			= 2, // ����
	//EEP_Legs			= 3, // ����
	//EEP_Back			= 4, // ����
	//EEP_Wrist			= 5, // ��
	//EEP_Feet			= 6, // �㲿

	//EEP_Finger1			= 7, // ��ָ
	//EEP_Finger2			= 8, // ��ָ
	//EEP_Waist			= 9, // ����

	//EEP_RightHand		= 10, // ��������
	//EEP_LeftHand		= 11, // ��������
	//EEP_Face			= 12, // ����
	//EEP_Equip_End		= 12,

	//EEP_MaxEquip		= 13, // װ������λ��

	//EEP_FashionHead		= 13, // ͷ��
	//EEP_FashionBody		= 14, // ����
	//EEP_FashionBack		= 15, // ����
	//EEP_FashionWrist	= 16, // �ֲ�
	//EEP_FashionFeet		= 17, // �㲿

	////EEP_MaxEquip		= 18, //F�ƻ��ı�
	//EEP_End				= 18,
	EEP_Null			= -1,
	EEP_Start			= -1, 

	EEP_Equip_Start		= 0,
	EEP_Head			= 0, // ͷ��
	EEP_Face			= 1, // ����
	EEP_Body			= 2, // ����
	EEP_Legs			= 3, // ����
	EEP_Back			= 4, // ����
	EEP_Wrist			= 5, // ��
	EEP_Feet			= 6, // �㲿

	EEP_Finger1			= 7, // ��ָ
	EEP_Finger2			= 8, // ��ָ
	EEP_Waist			= 9, // ����
	EEP_Neck			= 10, // ����

	EEP_RightHand		= 11, // ��������
	EEP_LeftHand		= 12, // ��������
	EEP_HunJing			= 13, // �꾧
	EEP_Holy			= 14,	//ʥ��
	EEP_Equip_End		= 14,

	EEP_MaxEquip		= 15, // װ������λ��

	EEP_FashionHead		= 15, // ͷ��
	EEP_FashionFace		= 16, // �沿��û���ˣ�
	EEP_FashionBody		= 17, // ����
	EEP_FashionLegs		= 18, // ����û���ˣ�
	EEP_FashionBack		= 19, // ����
	EEP_FashionWrist	= 20, // ��
	EEP_FashionFeet		= 21, // �㲿


	EEP_End				= 22,


};

enum EHolyEquipPos		//Ԫ��װ��λ������
{	
	EHEFI_Start   			= -1,			//���	typeEx	
	EHEFI_TianChong     	= 0,			//���	0			45
	EHEFI_LingHui			= 1,			//���	1			46
	EHEFI_QiPo				= 2,			//����	2			47
	EHEFI_LiPo				= 3,			//����	3			48	
	EHEFI_ZhongShu	        = 4,			//����	4			49
	EHEFI_JingPo			= 5,			//����	5			50
	EHEFI_YingPo			= 6,			//Ӣ��	6			51
	EHEFI_TianHun	    	= 7,			//���	7			42
	EHEFI_DiHun			    = 8,			//�ػ�	8			43
	EHEFI_MingHun	    	= 9,			//����	9			44

	EHEFI_End		    	= 10,		
};

enum EHolyEquipAttType	//Ԫ��װ���˺�����
{
	EHEAT_NULL				= 0,
	EHEAT_NULLEX			= 1,
	EHEAT_Demage			= 2,			//Ԫ������˺�
	EHEAT_Hit				= 3,			//Ԫ������
	EHEAT_Crit				= 4,			//Ԫ������
	EHEAT_CritAmount		= 5,			//Ԫ��������
	EHEAT_Exattack			= 6,			//Ԫ���⹦����
	EHEAT_InExattack		= 7,			//Ԫ���ڹ�����
	EHEAT_Accurate			= 8,			//Ԫ��׼
	EHEAT_Exdamage			= 9,			//Ԫ������˺�
	EHEAT_Toughness			= 10,			//Ԫ���������spp
	EHEAT_Morale			= 11,			//Ԫ��ʿ��
	EHEAT_Covalue			= 12,			//Ԫ��Ĭ��ֵ
	EHEAT_SoulValue			= 13,			//Ԫ��ֵ
	EHEAT_MaxSoulValue		= 14,			//Ԫ�����ֵ

	EHEAT_End				= 15,
};


const INT32 X_EQUIP_BAR_SIZE = EEP_End - EEP_Start - 1;
const INT32 X_HolySoul_BAR_SIZE = EHEFI_End;
#define MIsInEquipPos(n16Pos)	(n16Pos > EEP_Start && n16Pos < EEP_End)

// ����װ����λ
enum EWeaponPos
{
	EWP_NULL			= 0,	// ��
	EWP_Back1           = 1,    // ����  ��
	EWP_Back2           = 2,    // ����  ˫��
	EWP_Back3           = 3,    // ����  ǹ
	EWP_Back4           = 4,    // ����  �� ����
	EWP_Hand			= 5,	// �ֲ�
	EWP_dun			= 6,	// �ֲ�
};

const INT32 X_WEAPONPOS_NUM = EWP_dun + 1;

// �ٱ�����¼����
enum EBaiBaoRecordType
{
	EBBRT_System,			// ϵͳ����
	EBBRT_Mall,				// �̳���Ʒ
	EBBRT_Friend,			// ��������
	EBBRT_Myself,			// �Լ���ȡ
	EBBRT_GroupPurchase,	// �����Ź�
	EBBRT_VipNetBarGift,	// ����������Ʒ
};

// ���¸�ʱװλ��
enum EWardrobeFashionPos
{
	EWFP_Null = -1,

	EWFP_Head = 0,		// ͷ��
	EWFP_Body = 1,		// �·�
	EWFP_Wrist = 2,		// ��
	EWFP_Feet = 3,		// �㲿
	EWFP_Back = 4,		// ����

	EWFP_End
};

//-----------------------------------------------------------------------------
// �ṹ
//-----------------------------------------------------------------------------
// �Զ�����
struct tagItemOrder
{
	INT16	n16OldIndex;
	INT16	n16NewIndex;
};

// �ٱ�����¼
struct tagBaiBaoRecord
{
	INT16	n16Size;		// ������¼����
	INT16	n16Type;		// EBaiBaoRecordType
	DWORD	dwTypeID;		// ��Ʒ��TypeID
	DWORD	dwRoleID;		// ������ҵ�ID��GT_INVALIDΪϵͳ����
	DWORD	dwTime;			// ����ʱ�� tagDWORDTime
	TCHAR	szWords[1];		// ����
};

//-----------------------------------------------------------------------------
// ��
//-----------------------------------------------------------------------------

// �ֿ����ݴ���=����ǰ�ֿ��Կ�����λ��-24��/6
#define MCalWareExTimes(n16CurWareSpace)	((n16CurWareSpace - 24) / 6)

// ��ʹ����Ϸ�����䣬�շ�=���ֿ����ݴ���+1��^2��3000[��λ��Ϸ��]
#define MCalWareExSilver(nExTimes)	((nExTimes + 1) * (nExTimes + 1) * 3000)

// ��ʹ��Ԫ�����䣬���շ�=���ֿ����ݴ���+1����5[��λԪ��]
#define MCalWareExYuanBao(nExTimes)	((nExTimes + 1) * 5)


// �������ݴ���=����ǰ�����Կ�����λ��-20��/5
#define MCalBagExTimes(n16CurBagSpace)	((n16CurBagSpace - 20) / 5)

// ��ʹ����Ϸ�����䣬���շ�=���������ݴ���+1��^2��8000[��λ��Ϸ��]
#define MCalBagExSilver(nExTimes)	((nExTimes + 1) * (nExTimes + 1) * 8000)

// ��ʹ��Ԫ�����䣬���շ�=���������ݴ���+1����10[��λԪ��]
#define MCalBagExYuanBao(nExTimes)	((nExTimes + 1) * 10)


//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: log_cmdid_define.h
// author: 
// actor:
// data: 2008-11-11
// last:
// brief: ��������¼log�õ������������ö�� -- �������
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// log��������Ϣö�� -- һ����������ò�Ҫ�޸�(1.���ݿ�������Ӧ��¼��2.�ű�������д��)
//-----------------------------------------------------------------------------
enum ELogCmdID
{
	ELCID_Null					= 0,	// ��

	// �̵����
	ELCID_Shop_BuyItem			= 1,	// ����Ʒ(��װ��)
	ELCID_Shop_BuyEquip			= 2,	// ��װ��
	ELCID_Shop_Sell				= 3,	// ����Ʒ&װ��

	// PK�������
	ELCID_Open_SafeGuard		= 11,	// ����PK����

	// �����ͷ�
	ELCID_Dead_Penalty			= 12,	// PK�ͷ�

	// �ٱ������
	ELCID_BaiBao_LoadFromDB		= 20,	// ��item_baibao���ж�ȡ����Ʒ
	ELCID_BaiBao_Bill_YuanBao	= 21,	// Ԫ��ֱ��

	// �������
	ELCID_Bag_PosOverlap		= 30,	// ��ʼ����ɫ����ʱ��������Ʒλ���ص�

	// ϴ��
	ELCID_Clear_Att				= 40,	// ϴ���Ե�
	ELCID_Clear_Talent			= 41,	// ϴ���ʵ�
    ELCID_Buy_Talent            = 42,   // �������ʵ�

	// ��ɫ�ֿ����
	ELCID_RoleWare_PosOverlap	= 50,	// ��ʼ����ɫ�ֿ�ʱ��������Ʒλ���ص�
	ELCID_RoleWare_SaveSilver	= 51,	// ��Ǯ
	//ELCID_RoleWare_SaveYuanBao	= 52,	// ��Ԫ��
	ELCID_RoleWare_GetSilver	= 53,	// ȡǮ
	ELCID_RoleWare_GetYuanBao	= 54,	// ȡԪ��

	
	// �̳�
	ELCID_Mall_CartBuy			= 59,	// ʹ�ù��ﳵ������Ʒ
	ELCID_Mall_BuyItem			= 60,	// �����̳���Ʒ
	ELCID_Mall_BuyPack			= 61,	// �����̳���Ʒ��
	ELCID_Mall_FreeItem			= 62,	// ��ȡ�̳������Ʒ
	ELCID_Mall_BuyItem_Add		= 63,	// �����̳���Ʒʱ����
	ELCID_Mall_BuyPack_Add		= 64,	// �����̳���Ʒ��ʱ����
	ELCID_Mall_PresentItem		= 65,	// �����̳���Ʒ
	ELCID_Mall_PresentPack		= 66,	// �����̳���Ʒ��
	ELCID_Mall_PresentItem_Add	= 67,	// �����̳���Ʒʱ����
	ELCID_Mall_PresentPack_Add	= 68,	// �����̳���Ʒ��ʱ����
	ELCID_MALL_Activity_Add		= 69,	// �̳ǻ����

	// Ԫ������
	ELCID_Trade_SaveYuanBao		= 70,	// �����˻���Ԫ��
	ELCID_Trade_SaveSilver		= 71,	// �����˻����Ǯ
	ELCID_Trade_DepositYuanBao  = 72,	// �ӽ����˻�ȡ��Ԫ��
	ELCID_Trade_DepositSilver	= 73,	// �ӽ����˻�ȡ����Ǯ
	ELCID_Trade_Tax				= 74,	// �ύ����ʱ�۳�������

	// �̳� - ��
	ELCLD_Mall_ExchangeItem		= 80,	// �һ��̳���Ʒ
	ELCLD_Mall_ExchangePack		= 81,	// �һ��̳���Ʒ��
	ELCLD_Mall_ExchangeItem_Add	= 82,	// �һ��̳���Ʒʱ����
	ELCLD_Mall_ExchangePack_Add	= 83,	// �һ��̳���Ʒ��ʱ����
	// Jason 2010-3-16 v1.5.0 �ű���������ӿ�
	ELCLD_MALL_LUA_AddGiftPoint	= 84,	// �ű��ӿڣ�����ÿ����ȡ������
	ELCLD_MALL_GiftPointCosted	= 85,	// ��������

	// �Ź�
	ELCID_GroupPurchase_Faild		= 90,	// �Ź�ʧ�ܷ�������
	ELCLD_GroupPurchase_BuyItem		= 91,	// �Ź������̳���Ʒ
	ELCLD_GroupPurchase_BuyItem_Add	= 92,	// �Ź��̳���Ʒ����

	// �������
	ELCID_Quest_Loot			= 100,	// �������
	ELCID_Quest_Complete		= 101,	// �������
	ELCID_Quest_Accept			= 102,	// ��ȡ����
	ELCID_Quest_Discard			= 103,	// ��������
	ELCID_Quest_rewards			= 104,  // ������
	ELCID_FamilyQuest_rewards	= 105,	// ����������
	ELCID_FamilyQuest_GiveInItem = 106,// ���������Ͻ���Ʒ

	// װ��ǿ�����
	ELCLD_Compose_Spin			= 149,	// ����
	ELCID_Compose_Posy			= 150,	// ����
	ELCID_Compose_Engrave		= 151,	// �Կ�
	ELCID_Compose_Enchase		= 152,	// ��Ƕ
	ELCID_Compose_Brand			= 153,	// ��ӡ
	ELCID_Compose_LongFu		= 154,	// ����
	ELCID_Compose_Produce		= 155,	// �ϳ�
	ELCID_Compose_Quench		= 156,	// ���
	ELCID_Compose_Decompose		= 157,	// �㻯,װ���ֽ�
	ELCID_Compose_Chisel		= 158,	// ����
	ELCID_Compose_Dye			= 159,	// Ⱦɫ

	// ��Ҽ佻��
	ELCID_Exchange_Verify		= 160,	// ����ȷ��
	// Jason û��λ���ˣ���ʱ�����2009-12-6
	ELCID_Raise_PotVal			= 165,	// û�п�϶�ˣ�������ô����������
	ELCID_LessingLoong			= 166,	// �����͸�
	ELCID_PhilosophersStone		= 167,	// ���ʯ
	// ����Ǳ������������Ҫ����Ǳ����������
	ELCID_Spec_Raise_Potval		= 168,
	// ��ʯϴװ��
	ELCID_RockStone				= 169,

	// ս����ս����
	ELCID_CapPointRewards = 170, // ս��ռ���������
	ELCID_ActiveTimeRewards = 171, // ��Ծʱ�佱��

	// ԭ�ظ���,��������
	ELCLD_Revive_Locus			= 180,
	ELCLD_Revive_Perfect		= 181,

	// ��̯
	ELCLD_Stall_BeSold			= 190,	// ��̯����
	ELCLD_Stall_Buy				= 191,	// ��̯λ�Ϲ���

	// ������
	ELCLD_Dak					= 200,

	// ��Ʒ
	ELCLD_Item_Use				= 300,	// ��Ʒʹ��
	ELCLD_Item_Move				= 301,	// ͬһ�������ƶ�
	ELCLD_Item_Move2Other		= 302,	// ��ͬ�������ƶ�
	// Jason 2010-5-14 v2.1.0 ������������Ʒ
	ELCLD_Item_CostedByEarthTemple	= 303,	// ���������ĵ���
	ELCLD_Item_UseVipCard	= 304,	// ʹ��VIP��ݵ���

	ELCLD_Item_AtTerm			= 310,	// ��ʱ����Ʒ������ɾ��
	ELCLD_Item_EquipHoly		= 311,	// ʥ�鴩װ��
	ELCLD_Item_UnEquipHoly		= 312,	// ʥ����װ��

	// װ��
	ELCLD_Equip_Identify		= 350,	// װ������
	ELCLD_Equip_ReGet			= 351,	// װ���ָ�
	
	// ����
	ELCD_1stOnline_Item			= 380,	// ���ַ���

	// ��ҳԴӱ����ӵ���Ʒ
	ELCLD_Role_Discard_Item		= 401,

	// ���ʰȡ��Ʒ
	ELCLD_PickUp_Item			= 402,
	ELCLD_PickUp_Money			= 403,

	// ������������
	ELCLD_Send_Gift				= 410,

	// ��Ʒ��
	ELCLD_Gift_Bag				= 420,

	// ְ��NPC���
	ELCLD_Bag_Extend			= 450,	// ���䱳��
	ELCLD_RoleWare_Extend		= 451,	// ��ɫ�ֿ�����
	ELCLD_CureInjure			= 452,	// npc��������
	
	// ��ͨ����
	ELCID_Loot					= 500,	// ��ͨ����

	// �������
	ELCLD_Guild_Create			= 600,	// ��������
	ELCLD_Guild_Upgrade			= 601,	// ������ʩ����
	ELCLD_Guild_SpreadAffair	= 602,	// ��������
	ELCLD_Guild_DailyCost		= 603,	// �ճ�����
	ELCLD_Guild_SkillResearch	= 604,	// ���ɼ�������
	ELCLD_Guild_SkillLearn		= 605,	// ѧϰ���ɼ���
	ELCLD_Guild_Commerce		= 606,	// ����
	ELCLD_Guild_Reputation		= 607,	// �����������
	ELCID_Guild_ChangeName      = 608,  // ���ɸ���
	ELCID_Guild_DeclareWar      = 609,  // ��ս
	ELCID_Guild_BattleField_SignUp  = 610,  // ս����������
	ELCLD_Guild_Skill_Active	= 611,	// ���Ἴ�ܼ���
	ELCLD_Guild_BattleField_Hold = 612, //��ս������Ĺ����ʽ�ÿ��22:00�������

	// �������
	ELCLD_CHAT_WORLD_CHANNEL		= 700,	//������Ƶ������
	ELCLD_SHOW_EQUIP_WORLD_CHANNEL	= 701,	//������Ƶ��չʾװ��
	ELCLD_SHOW_ITEM_WORLD_CHANNEL	= 702,	//������Ƶ��չʾ��Ʒ
	ELCLD_CAST_TALK_IM				= 703,	//����ʹ��IM����(��)
    ELCLD_WORLD_TALK_IM             = 704,  //����ʹ��IM����
    ELCLD_WORLD_TALK_IM_UNBIND      = 705,  //����ʹ��IM����(�ǰ�)
	ELCLD_CAST_TALK_IM_UNBIND		= 706,	//����ʹ��IM����(�ǰ�)
	ELCLD_WORLD_TALK_IM_NETBAR		= 707,	//��������ʹ����������

	// �������
	ELCLD_PET_ITEM_CREATE_PET		= 800,	// ���ɳ���
	ELCLD_PET_ITEM_FEED_PET			= 801,	// ����ιʳ
	ELCLD_PET_ITEM_PET_LEARNSKILL	= 802,	// ����ѧϰ����
	ELCLD_PET_ITEM_PET_REVIVE		= 803,	// ���︴��
	ELCLD_PET_ITEM_PET_EQUIP		= 804,	// ����װ��
	ELCLD_RolePourExpPet			= 805,	// ��ע����
	ELCLD_PET_ITEM_PET_Enhance		= 806,	// ����װ��
	ELCLD_PET_ITEM_PET_Lock			= 807,	// ��������
	ELCLD_PET_ITEM_PET_UnLock		= 808,	// �������
	ELCLD_PET_ITEM_PET_Food			= 809,	// ����ʳ��
	ELCLD_PET_SKILL_SALE			= 810,	// ����С������
	
	ELCLD_PET_CREATE_PET			= 811,	// ��������
	ELCLD_PET_GAIN_PET				= 812,	// ��ó���
	ELCLD_PET_DEL_PET_CREATE		= 813,	// ɾ���������ʧ�ܣ�
	ELCLD_PET_DEL_PET_INIT			= 814,	// ɾ�������ʼ��ʧ�ܣ�
	ELCLD_PET_DEL_PET_CLIENT		= 815,	// ɾ������ͻ������
	ELCLD_PET_LOSE_PET				= 816,	// ʧȥ����
	// Jason v1.3.2 2010-1-31 ������ﱳ������
	ELCLD_PET_PocketExtend_Costed	= 817,	// 
	ELCLD_PET_RecallPet				= 818,	//���ճ���
	// ����Ԫ���˺�����ת��
	ELCLD_Fabao_EleTypeChange_Costed = 850,	// ����ת��Ԫ���˺���������
	ELCLD_Fabao_IncEleAbility_Costed		= 851,  // ������������
	ELCLD_Fabao_StuntBook					= 852,	// �����ؼ�������
	// ��������������Ʒ
	ELCLD_Fabao_Nirvana					= 853,	// ������������
	ELCLD_Fabao_Birth						= 854,	// ������ֳ
	ELCLD_Fabao_Fusion				= 855,	// �����ں����� [3/22/2012 zhangzhihua]
	ELCLD_FairyAddPromoteTimes		= 856,	//������������

	// �������
	ELCLD_ACT_TREASURE				= 900,	// ���������䱦���

	//�̶�����
	ELCLD_WU_SE_SHI					= 1000, // �̶��ɾ����ɫʯ
	ELCLD_PVP						= 1001, // pvp�̶��
	ELCLD_LEAVEWAR_DEL				= 1002, // �˳�ս���ı�������

	// VIP̯λ���
	ELCLD_VIP_Stall_Rent			= 1100,	// VIP̯λ����

	// ��������
	ELCLD_VIP_NetBar				= 1200,	// ��������

	// ����
	ELCLD_Skill_Use					= 1300, // ����

	// �����ɽ缼��
	ELCLD_PracticeM_Skill			= 1350, // ��ϰ�ɽ缼��
	ELCID_PracticeSoaringSkill		= 1351,	// ��ϵ������������

	// ְҵ���
	ELCLD_Class_Change				= 1400, // תְ
	ELCLD_Class_Get					= 1401, // ��ְ

	// ����ϵͳ
	ELCLD_City_Produce				= 1301,	// ���в���
	ELCLD_City_Prolificacy			= 1302,	// ��������������
	ELCLD_City_TransformTaxation	= 1303,	// ��ȡ˰��
	ELCLD_City_SignUp				= 1304,	// ��ս����
	ELCLD_City_Examine				= 1305,	// �鿴������Ϣ

	// ����ϵͳ
	ELCLD_Spirit_Reward				= 1400, // ����ϵͳ��������
	ELCLD_Strength_Reward			= 1401, // ����ϵͳ��������-ʵ��

	// ��������ڴ˴�֮�ϼ�
	ELCID_GemRemoval				= 1500,	// ��ʯ���

	// Jason v1.3.2 2010-1-20 ������߾�����ȡ
	ELCID_PlayerOfflineReward		= 1501,
	// ��������
	ELCID_Soaring_BuildEquip		= 1502,	// ����������־
	ELCID_Casting_SoaringEquip		= 1503, // �����Ѿ�����װ��

	ELCID_GuildDailyReward		= 1504,	// ����ս��ÿ�����߽���
	
	// Jason 2010-4-16 v2.0.0 �������
	ELCID_Fabao_RemoveYuLingzhu		= 1550,	// �����ɳ�����������
	ELCID_Fabao_RemoveLingzhu		= 1551,	// ����������ϣ���������
	ELCID_Fabao_ReversalCosted		= 1552,	// ����������ת���ĸ�������
	ELCID_Fabao_MergingCosted		= 1553, // ��������������������
	ELCID_Fabao_FairyPowderCosted	= 1554, // ����֮������
	ELCID_GodStrengthen_Costed		= 1555, // ��������
	ELCID_Fabao_Feeding				= 1556,	// ����ιʳ����
	ELCID_Fabao_StoreExp_Costed		= 1557,	// ���������ӡ����
	ELCID_Fabao_StoreExp_Created		= 1558,	// ���������ӡ����

	// ʦͽ
	ELCLD_Apprentice_UseBaoJian		= 1600,	// ͽ��ʹ�ñ�������
    
	// ѡ��
	ELCLD_VoteShow_UseNecklace      = 1700, // ѡ��������������

	// ����
	ELCLD_Appearance_Transform		= 1800, // �������ĵ���

	ELCID_FCompose_Consolidate		= 1850,	// F�ƻ���ǿ������
	ELCID_FCompose_Xinhun				= 1851,	// F�ƻ�������
	ELCID_FCompose_Fuhun				= 1852,	// F�ƻ�������

	// ������Լ������
	ELCID_Fairy_Costed						= 1900,	// ʥħ��������Լ����

	// ��֮����
	ELCID_Godbid_NormalBid_Cost = 1910,			// ��ͨ��������
	ELCID_Godbid_NormalBid_Added = 1911,		// ��ͨ����
	ELCID_Godbid_BlackBid_Cost = 1912,				// ���о�������
	ELCID_Godbid_BlackBid_Added = 1913,			// ���о���

	// �ҽ�
	ELCID_Egg_Broker_Cost = 1914,
	ELCID_Egg_Broker_Added = 1915,
	
	// �߼��ϳ�
	ELCID_Advanced_Composed_Cost = 1916,
	ELCID_Advanced_Composed_Add = 1917,

	// �³�
	ELCID_Wardrobe_Layer_LevelUp_Cost = 1918,

	// ��������
	ELCID_FamilyQuest_GetReward = 1919,

	ELCLD_Family_Sprite_Train		= 1920,		// ������������
	ELCID_Family_Sprite_Train_Item	= 1921,		// ����������Ʒ

	// Key���콱��
	ELCID_KeyCode_Reward = 1922,

	ELCID_Compose_Extract	= 1923,	// ��ȡ

	ELCLD_Family_Role_Sprite = 1924,		//���������ǼǷ���

	ELCID_Family_Quest_GiveInItem = 1925, // �Ͻɼ���������Ʒ

	ELCID_Marrige = 1926, // ���

	ELCID_Zhanghao_Gift		= 1927,	// �˺Żع����

	// ���
	ELCID_BloodBrother = 1928,

	ELCID_PulseSkill		= 1929, // ����

	// װ���ɳ�
	ELCID_EquipGrow = 1940,
	ELCID_EquipGrowTrans = 1941, // װ���ɳ��̳�

	// �ϻ���
	ELCID_Tiger = 1942,

	// ������ħ����
	ELCID_SOUL_SKILL = 1943,

	// ��ħ������Լ
	ELCID_SOUL_CONTINUE_SKILL = 1944,

	// ��ħ������Լ��Ʒ����
	ELCID_SOUL_CONTINUE_ITEM = 1945, 

	// �꾧
	ELCID_SoulCrystal_Grow = 1950, // �꾧����
	ELCID_SoulCrystal_Renew = 1951, // �꾧����

	// ˢ�ɳ����Ի��߱�ʯ����
	ELCID_EquipGrowIDOrDiamondAdd = 1960,	

	// ��key��
	ELCID_NewKeyCode	= 1970,

	// �����ֻ���
	ELCID_SaveTelNum	= 1971,

	//Ԫ��ǿ��
	ELCLD_StarEnhanceConsolidate		=   2000,//Ԫ��װ������ǿ��
	ELCLD_HolySoulReborn				=	2100,//Ԫ��ת��

	ELCLD_HolySoulEquipHole				=	2110,//Ԫ��װ�����
	ELCLD_HolySoulEquipInlay			=	2111,//Ԫ��װ����Ƕ
	ELCLD_HolySoulEquipRemove			=	2112,//Ԫ��װ��ժ��
	ELCLD_HolyEatFairy					=	2113,//ʥ����������

	ELCLD_UnEquipHoly					=	2120,//ʥ��ϴӡ���Ľ��
	ELCID_ShengWenChaiChu				=	2121,// ʥ��ϴӡ�۳�ϴӡ��

	ELCLD_EnhanceHolyEquip				=	2122,//����ʥ�����Ľ��
	ELCID_DelEnhanceItem				=	2123,//����ʥ�ƿ۳����ܵ���
	ELCID_DelPianXiangItem				=	2123,//����ʥ�ƿ۳�ƫ�����

	ELCID_EquipPerfectGrow				=	2130,//�����ɳ�����
	ELCID_EquipXiStar					=	2131,//���Ǵ�
	ELCID_WarReward						=	2132,//ս����Ʒ
	ELCID_WarCost						=	2133,//ս������

	ELCID_HolyStrength					=	2140,//����������������
	ELCID_HolyEquipmentNumber			=	2141,//����ʥ���װ��ʥ������

	ELCID_MallDaiBi						=	2150,//�̳Ǵ��ҹ���������
	ELCID_YuanBaoDaiBi					=   2151,//NPC��ȡ����

	// GM����
	ELCID_GM_ClearBag				= 1000000,	// ������
	ELCID_GM_CreateItem				= 1000001,	// ������Ʒ/װ��/ʱװ
	ELCID_GM_GetMoney				= 1000002,	// ��ý�Ǯ
	ELCID_GM_GetYuanBao				= 1000003,	// ���Ԫ��
	ELCID_GM_GetExVolume			= 1000004,	// �������
	ELCID_GM_GetFund				= 1000005,	// ��ð����ʽ�
	ELCID_GM_GetMaterial			= 1000006,	// ��ð����ʲ�
	ELCID_GM_GetContribute			= 1000007,	// ��ð��ɸ��˹���
    ELCID_GM_GetVIPPoint            = 1000008,  // ��û��� 
};

// Ԫ��װ��ǿ������
enum EHolyEquipConsolidateType
{
	EHECT_Null			= 0,	//
	EHECT_StarEnhance	= 1,	// ����ǿ��
	EHECT_EquipHole		= 2,	// ���ǿ��
	EHECT_EquipInlay	= 3,	// ��Ƕǿ��

};

//-----------------------------------------------------------------------------
// log�����ݽṹ
//-----------------------------------------------------------------------------
struct tagLogSilver				// ��Ǯ�仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	DWORD	dwRoleIDRel;
	INT64	n64Silver;			// +����ã�-��ʧȥ
	INT64	n64TotalSilver;		// ��Ӧ����������
	INT8	n8LogConType;		// �������ֿ�
};

struct tagLogYuanBao			// Ԫ���仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nYuanBao;			// +����ã�-��ʧȥ
	INT		nTotalYuanBao;		// ��Ӧ����������
	INT8	n8LogConType;		// �������ٱ���
};

struct tagLogExVolume			// ����仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nExVolume;			// +����ã�-��ʧȥ
	INT		nTotalExVolume;		// ��Ӧ����������
};

struct tagLogVIPPoint			// ���ֱ仯
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nVIPPoint;			// +����ã�-��ʧȥ
	INT		nTotalVIPPoint;		// ��Ӧ����������
};

struct tagLogGodBid			// ��֮����
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwItemID;
	BYTE		byMoneyType;
	DWORD	dwMoney;
};

struct tagLogBattlefield			// ս��ռ���¼
{
	DWORD	dwGuildID;															// '����ID'
	TCHAR		szGuildName[24];			// ��������
	BYTE		byBattlefield;														// 'ս��ö�٣�0--����ڣ 1--�߻�֮�� 2--�����ѹ�',
};

struct tagLogTimeStat			// ������Ҷ�ʱͳ��
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	INT64	n64BagSilver;
	INT64	n64WareSilver;
	INT		nBagYuanBao;
	INT		nBaiBaoYuanBao;
	INT		nExVolume;
	INT     nVIPPoint;
};

struct tagLogItem				// ��Ʒlog�ṹ
{
	DWORD		dwRoleID;
	DWORD		dwTypeID;

	INT64		n64Serial1;
	INT64		n64Serial2;
	INT8		n8ConType1;
	INT8		n8ConType2;
	INT16		n16ResNum1;
	INT16		n16ResNum2;

	INT16		n16OptNum;		// �仯����(+����ʾ��ã�-����ʾʧȥ)
	DWORD		dwCmdID;
	DWORD		dwRoleIDRel;
};

struct tagLogMallSell			// �̳ǳ�����Ʒlog�ṹ
{
	INT64		n64Serial;
	DWORD		dwTypeID;
	DWORD		dwAccountID;	// �������˺�ID
	DWORD		dwRoleID;		// ������
	DWORD		dwToRoleID;		// �����Ʒ��
	DWORD		dwFstGainTime;
	DWORD		dwExistTime;
	INT			nMaxUseTimes;
	INT			nCostYuanBao;
	INT			nCostExVolume;
	INT         nCostVIPPoint;
	DWORD		dwCmdID;
	INT16		n16NumSell;
};

struct tagLogMallSellPack		// �̳ǳ�����Ʒ��log�ṹ
{
	DWORD		dwPackID;
	DWORD		dwAccountID;	// �������˺�ID
	DWORD		dwBuyRoleID;
	DWORD		dwToRoleID;
	INT			nCostYuanBao;
};

struct tagLogItemTims			// ��Ʒʹ�ô���log�ṹ
{
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT64		n64Serial;
	DWORD		dwTypeID;
	INT			nUsedTimes;
	INT			nMaxUseTimes;
	INT8		n8ConType;
};

struct tagLogFund
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nFund;
	INT			nTotalFund;
};

struct tagLogMaterial
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nMaterial;
	INT			nTotalMaterial;
};

struct tagLogReputation
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nReputation;
	INT			nTotalReputation;
};

struct tagLogGMCmd
{
	DWORD		dwRoleID;
	TCHAR		szGMCmd[X_LONG_STRING];
	DWORD		dwErrorCode;
};

struct tagLogPet
{
	DWORD		dwMasterID;
	DWORD		dwPetID;
	DWORD		dwCmdID;
};

struct tagLogRoleOnline
{
	DWORD		dwRoleID;
	DWORD		dwOnlineLastTime;
	CHAR			szMac[X_SHORT_STRING];
};

struct tagLogFairyBirth
{
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	INT64			nFaBao64Serial;
	BOOL			bNormalChange;
	INT		nBeforeChangeBirthState;
	INT		nAfterChangeBirthState;
};
//Ԫ��װ��ǿ��log
struct tagLogHolyEquipConsolidate
{
	EHolyEquipConsolidateType	eType;	//Ԫ��װ��ǿ������
	INT64	n64HolyEquipID;				//Ԫ��װ��ID
	bool	bCostMoney;					//�۳���Ǯ�Ƿ�ɹ�
	INT		nCostMoney;					//�۳�Ǯ��
	bool	bCostItem;					//�۳���Ʒ�Ƿ�ɹ�
	INT		nCostItem;					//�۳���Ʒ��
	bool	bCostItemOther;				//�۳��Ͼ���¶�Ƿ�ɹ�
	INT		nCostItemOther;				//�۳��Ͼ���¶��
	bool	bConsolidateRes;			//ǿ���Ƿ�ɹ�
	BYTE	byCurrentLevel;				//Ԫ��װ����ǰ�ȼ�
	DWORD	dwRoleID;					//��ɫID	
};

//-----------------------------------------------------------------------------
#pragma pack(pop)
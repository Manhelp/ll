#pragma once


#pragma pack(push, 1)

//------------------------------------------------------------------------------
// ��Ʒ���⹦�����Ͷ���
//------------------------------------------------------------------------------

/** \��Ʒ���⹦������(�ô���<val1, val2>��ʾ��2������,<val>��ʾ1��,û��ע����ʾû��)
*/
enum EItemSpecFunc	
{
	EISF_Null										= 0,	// ��

	EISF_AcceptQuest								= 1,	// ��ȡ����
	// nSpecFuncVal1: ������
	// nSpecFuncVal2: ����
	EISF_IdetifyEquip								= 2,	// ����װ��
	// nSpecFuncVal1: ��������
	// nSpecFuncVal2: ����
	EISF_HoleGem									= 3,    // ��Ƕ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_Brand										= 4,	// ��ӡ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_LongHunShi									= 5,    // ����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RemoveAttPt								= 6,	// ����ϴ��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RemoveTalentPt								= 7,	// ����ϴ��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_LearnSkill									= 8,	// ѧϰ����
	// nSpecFuncVal1: ����ID
	// nSpecFuncVal2: ����
	EISF_Grind										= 9,	// ĥʯ
	// nSpecFuncVal1: �ָ����¶�ֵ
	// nSpecFuncVal2: ����
	EISF_ComposeAdvance								= 10,	// �ϳɼ���������
	// nSpecFuncVal1: ����������	
	// nSpecFuncVal2: �����ٷֱ�
	EISF_OpenSafeGuard								= 11,	// ǿ��PK����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RevieveItem								= 12,	// �������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_FriendGift									= 13,	// ��������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_Chisel										= 15,	// ����
	// nSpecFuncVal1: �����ɹ���
	// nSpecFuncVal2: ����ȼ�
	EISF_ProtectSign								= 16,	// ���׷�
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_Chest										= 17,	// ����
	// nSpecFuncVal1: ���俪������Ʒ(�����)
	// nSpecFuncVal2: Կ�׵�TypeID
	EISF_ChestKey									= 18,	// ����Կ��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

  //EISF_Chest*                                     = 19,   // ���䱣��
	// nSpecFuncVal1: ����
    // nSpecFuncVal2: ����
	
	EISF_PetEgg										= 20,	// ���ﵰ
	// nSpecFuncVal1: ԭ��ID
	// nSpecFuncVal2: ����
	EISF_PetFood									= 21,	// ����ʳ��
	// nSpecFuncVal1: 1,Я���ȼ� 2,��������
	// nSpecFuncVal2: 1������ֵ	 2��������
	EISF_PetSkillBook								= 22,	// ���＼����
	// nSpecFuncVal1: ����ԭ��ID
	// nSpecFuncVal2: ����
	EISF_PetLvlupStep								= 23,	// ��������
	// nSpecFuncVal1: ��׺�Ҫ��������ֵ
	// nSpecFuncVal2: ����
    EISF_PetEquipment                               = 24,	// ����װ��
    // nSpecFuncVal1: װ��ԭ��ID
    // nSpecFuncVal2: ����
    EISF_ColorProbability                           = 25,   // ָ����ɫ�׶μ�������
    // nSpecFuncVal1: ����1-4���ɵ͵��߷ֱ��ʾ�ĸ���ɫϡ�г̶Ƚ׶Σ��ͣ��У��ߣ�����
    // nSpecFuncVal2: 1-10000����ʾ��ָ���׶εļ��ʵ������̶ȣ�10000��ʾ����100%��
    EISF_Dye                                        = 26,   // Ⱦ��
    // nSpecFuncVal1: 1-12����Ӧ��ɫ��ͼ���
    // nSpecFuncVal2: ����
	EISF_GuildSkillBook								= 27,	// ���ɼ��ܵ伮
	// nSpecFuncVal1: �Ͻ�ʱ��������������
	// nSpecFuncVal2: ���ɹ���������
	EISF_PetExpPour									= 28,	// ���ﾭ���ע
	// nSpecFuncVal1: ��ע����ֵ
	// nSpecFuncVal2: ����
	EISF_PetEnhance									= 29,	// ������������
	// nSpecFuncVal1: ���޵�Ʒ��
	// nSpecFuncVal2: �������ֵ
	EISF_ItemLock									= 30,	// ��Ʒ����

	EISF_ItemUnLock									= 31,	// ��Ʒ����
	
	EISF_PetLock									= 32,	// ��������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_PetUnLock									= 33,	// �������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	
	EISF_PortableWare								= 34,   // ����ֿ�

	EISF_TransportTool                              = 35,   // �����ͷ�

	EISF_Fishing									= 36,	// ���㣬�һ�ϵͳ

	EISF_ChangeVocation								= 37,	// תְ

	EISF_EquipAddPotValue							= 38,	// ����װ��Ǳ������
	// nSpecFuncVal1: �ɹ���
	// nSpecFuncVal2: Ǳ��ֵ��������
	
	EISF_BagExtend									= 39,	// ������չ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	
	EISF_RoleWareExtend								= 40,	// �ֿ���չ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	
	EISF_BeGoldStone								= 41,	// ���ʯ
	// nSpecFuncVal1: 1�� ���ʯ	 2�� �μ����ʯ
	// nSpecFuncVal2: ����
	
	EISF_RenameItem									= 42,	// ������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	
	EISF_GemRemovalSign								= 43,	// ��ʯ�����

	EISF_PlayerTrack								= 44,	// ����׷��
	// nSpecFuncVal1: 0	��ʾ���ܽ��д���  1 ��ʾ�ܽ��д���
	// nSpecFuncVal2: ����

	EISF_EquipSignature								= 45,	// װ������
	// nSpecFuncVal1: 0	��ʾ�����󲻰�  1 ��ʾ�������

	EISF_PetPocketExtItem							= 46,	// ������������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	EISF_AddMiraclePrcNumItem						= 47,	// �����ɽ����е���ĵ��ߡ��ɽ����з�
	// nSpecFuncVal1: ���ӵĵ���
	// nSpecFuncVal2: ����

	EISF_SpinFashionDressItem						= 48,	// ��˿��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	EISF_LoadScript									= 49,	// ���ýű���������Ʒ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	// Jason 2010-4-12 v2.0.0
	EISF_LingZhu									= 50,	// ���飬��Ƕ�����ã������ں����飩
	// nSpecFuncVal1: ���󷨱��Ƚף�1~50��
	// nSpecFuncVal2: ���󷨱����ͣ���ľˮ������Ӧ1~5��
	EISF_YuLingZhu									= 51,	// �����飬���������Ƚ�֮��
	// nSpecFuncVal1: �����Ƚ׵���
	// nSpecFuncVal2: ����
	EISF_GuildChangeName							= 52,	// ���ɸ�������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_EarthTemple								= 53,	// ������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_UpgradeEquipMaxPotValue					= 54,	// ����װ��Ǳ�����޵���
	// nSpecFuncVal1: װ��λ�ã�Ĭ��ֵΪ-1������û�����ƣ�
	// nSpecFuncVal2: ʹ�øþߺ�װ����Ǳ��ֵ���޿�������������ֵ���ֶ�Ĭ��ֵΪ0
	EISF_ZhanDouFu									= 55,   // ս���һ�����ս����
	
	EISF_RefreshQuestBoard							= 70,	// ˢ����������

	EISF_LuckyFu									= 71,	// ���˷�
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: �����ٷֱ�
	EISF_BaoDiFu									= 72,	// ���׷�
	// nSpecFuncVal1: 0�����׷��� 1���ػ���
	// nSpecFuncVal2: ����
	EISF_XiHun										= 73,	// ����
	// nSpecFuncVal1: ����ȼ�
	// nSpecFuncVal2: ����
	EISF_FuHun										= 74,	// ����
	// nSpecFuncVal1: �����ǿ���ȼ�
	// nSpecFuncVal2: ����
	EISF_QiangHuaStone								= 75,   // ǿ��ʯ
	// nSpecFuncVal1: ǿ���ȼ�
	// nSpecFuncVal2: ����
	EISF_GuiYuan									= 76,   // ��ԭ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_FairyPowder								= 77,   // ����֮��
	// nSpecFuncVal1: ��������
	// nSpecFuncVal2: ��������

	EISF_TransferStone								= 78,   // ������ʯ

	EISF_GuildSkillActive							= 79,	// ���Ἴ�ܼ�����
	// specfuncval1: ���Լ���ļ���ID
	// nSpecFuncVal2: ����

	EISF_TreasureSeizer								= 80,	// �ᱦ���

	EISF_GodStrengthen								= 81,   // �������������Ʒ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	EISF_VipCard									= 82,   // VIP��
	// nSpecFuncVal1: VIP���ȼ���1����2��3�׽�
	// nSpecFuncVal2: ����

	EISF_FairyCode									= 83,	// ��������
	// nSpecFuncVal1: 1��2ˮ��3ɳ��4�磬5������6����
	
	EISF_VerityOfStone								= 84,	// ����֮ʯ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	EISF_FairyFruit									= 85,	// ������ʵ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	EISF_FairyStuntBook								= 86,	// �����ؼ���
	// nSpecFuncVal1: �ؼ�id 
	// nSpecFuncVal2: ����

	EISF_FairyNirVana								= 87,	// ��������
	// nSpecFuncVal1: 0: ���˵�60�� 1�����˵������10��
	// nSpecFuncVal2: ����

	EISF_ExpBall											= 88, // ������ˮ����
	// nSpecFuncVal1: 0:δԼ�� 1��Լ��
	// nSpecFuncVal2: ���nSpecFuncVal1=0��Ϊˮ����[Լ��]��typeID

	EISF_ExpPilular										= 89, // ���鵤��ˮ��֮�ģ�
	// nSpecFuncVal1: ���鵤����洢�ľ���
	// nSpecFuncVal2: ����

	EISF_RockGod											= 90,	// ��ʯ
	// nSpecFuncVal1: װ���ĵȼ�������С�ȼ�����װ���ĵȼ����ƣ�����ʹ�õȼ�
	// nSpecFuncVal2: װ���ĵȼ�������ߵȼ�

	EISF_FairyDecPregnancy								= 91,	// ��������
	EISF_FairyIncBreedingTimes							= 92,	// ������Դ���
	EISF_FairyDecRestTime								= 93,	// �������ʱ��

	EISF_EggHammer											= 94,	// �ҽ𵰵Ĵ���
	EISF_WardrobeLayerLvlUp							= 95,	// �����³���λ

	EISF_GlobalLimitedItem								= 96,	// ȫ����������Ʒ

	EISF_FamilySpriteItem									= 97,	// ��������������Ʒ

	EISF_FairyRaiseIntelligenceMax					= 98,	// �����������������ҩ��
	// nSpecFuncVal1: ��Ӧ�����������޵ı�ʶ 0-����100��120
	// nSpecFuncVal2: �ɹ��ĸ���

	EISF_FairyDoRaiseIntelligence						= 99,	//	�ȶ������������ҩ��
	// nSpecFuncVal1: ������������
	// nSpecFuncVal2: ������������

	EISF_BloodBrother										= 100,	//	���ʹ�õ���
	// nSpecFuncVal1: ����

	EISF_EquipGrowClips									= 101,	// ������Ƭ
	// nSpecFuncVal1: ������װ���ɳ���

	EISF_EquipGrowTransIM								= 102,	// װ���ɳ��̳з�
	// nSpecFuncVal1: ������С��
	// nSpecFuncVal2: ���������
	EISF_EquipGrowLuckSign								= 103,	//�ɳ����˷�
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_EquipReduceIM									= 104,	//���ǵ���
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

	EISF_TigerCoins								= 105,	//�ϻ�����

	EISF_HunShiJingHua								= 106,	//��ʯ����

	EISF_Soul_Skill								= 107,	//��ħ������Լ

	EISF_EquipGrowTransPIM				= 108,	// ����װ���ɳ��̳з�

	EISF_DiamondEquipGrowIM				= 109,	// װ���ɳ�ϴ�����Լ���ʯ��������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_FairyFusion					= 110,	//��������IM����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	//EISF_SoulInlay										= 111,	// ��Ӱ��ʯ--����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	//EISF_SoulChisel									= 112,	// ���Ƽ�
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_ShengWenChaiChu				= 111,	//ʥ�Ʋ����
	//nSpecFuncVal1: ����
	//nSpecFuncVal2: ����
	EISF_PerfectGrow					= 112,	//�����ɳ�����
	//nSpecFuncVal1: ��������
	//nSpecFuncVal2: ����
	EISF_XiStarIM						= 113,	//���Ǵ�
	//nSpecFuncVal1: ��������
	//nSpecFuncVal2: ��Ӧ���ɵĵ���id
	EISF_ReplaceFabao					= 115,	//����������
	//nSpecFuncVal1:�ֶ���д����ֵ��50-140������Ӧ�������ַ���
	//nSpecFuncVal2:����
	EISF_HolyStrength					= 116,	//����ʥ���������������ĵ���
	EISF_HolyEquipmentNumber			= 117,	//����ʥ���װ��ʥ������
	EISF_NewChisel						= 118,	// �¿���
	//nSpecFuncVal1: ����
	//nSpecFuncVal2: ����
};

/** \�ϳɼ��������� EISF_ComposeAdvance �������������
*/
enum ESpecFuncComposeAdvance			
{
	ESFCA_AllProduce		= 0,	// ����������������
	ESFCA_Artisan,					// �ɽ�
	ESFCA_DanTraining,				// ����
	ESFCA_Smith,					// ����
	ESFCA_Casting,					// ����
	ESFCA_Dressmaker,				// �÷�
	ESFCA_Aechnics,					// ����
	ESFCA_Smilt,					// ұ��

	EISFC_AllConsolidate	= 10,	// ����װ��ǿ����������
	EISFC_Posy,						// ����
	EISFC_Engrave,					// �Կ�
	EISFC_Quench,					// ���
	

	EISFC_All				= 20,	// ���кϳɶ������� 
};


#pragma pack(pop)
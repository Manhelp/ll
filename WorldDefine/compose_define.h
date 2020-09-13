#pragma once

#include "ItemDefine.h"
#include "suit_define.h"

#pragma pack(push, 1)

#define MAX_CONSOLIDATE_POS_QUANTITY 12
#define MAX_CONSOLIDATE_POS_HOLYEQUIP 3 //Ԫ��װ����ǿ��λ�ø���
#define MAX_CONSOLIDATE_STUFF_QUANTITY 6
#define MAX_PRODUCE_STUFF_QUANTITY 6
#define MAX_CONSOLIDATE_ROLEATT 3
#define MAX_OUTPUT_QUANTITY	6
#define MAX_CASTING_ROLEATT	5
#define MAX_CASTING_QUANTITY 5
#define X_EXTRACT_MAX_PRODUCTION 6
#define MAX_EXTRACT_OUTPUT 6

//��������
enum EProduceType
{
	EPCT_NULL	= 0,
	EPCT_Artisan,			//����1
	EPCT_DanTraining,		//����2
	EPCT_Smith,				//����3
	EPCT_Casting,			//����4
	EPCT_Dressmaker,		//�÷�5
	EPCT_Aechnics,			//����6
	EPCT_Smilt,				//ұ��7
	EPCT_PointUp,			//�㻯8
	EPCT_GodArtisan,		//�콳9	add xtian 2009-4-13
	EPCT_Pet,				//����10 add xtian 2009-8-21
	EPCT_EquipMaster,		// װ����ʦ
	EPCT_EquipRefine,		// װ����ȡ(F:ħ��)
	EPCT_StuffRefine,		// ����������F:��ʯ������
	EPCT_TimeLimitEquip,	// ʱ��װ��14(F:��)
	EPCT_Gem,				// ��ʯ����15
	EPCT_TreasureCom = 900,	// ����ϳ� 
	EPCT_FoodGod1 = 20,	// ʳ��1��	�ҶԲ߻����óϷ�					(ʳ�ļӹ�)
	EPCT_FoodGod2 = 21,	// ʳ��2��	�ҶԲ߻�����Ͷ��					(ʳ������)
	EPCT_FoodGod3 = 22,	// ʳ��3		�ҶԲ߻���Ϊ����					(ʥ������)
	EPCT_Perfume =  23,	// ��ˮ����  �ҶԲ߻��޻���˵������������ (����ں�)
	EPCT_Crystal = 24,	// ˮ���ϳ�
	EPCT_Stole_52 = 25, // 52�ȼ�ʥ��
	EPCT_Stole_67 = 26, // 67�ȼ�ʥ��
	EPCT_Stole_82 = 27, // 82�ȼ�ʥ��
	EPCT_Stole_Blade = 28, // ���ʥ��
	EPCT_Stole_Bow = 29, // ����ʥ��
	EPCT_Stole_Valor = 30, // Ǭ��ʥ��
	EPCT_Stole_Swing = 31, // ����ʥ��
	EPCT_SoulLinker = 32, // �����
	EPCT_MoonCake = 33, // �±�
	EPCT_HallowmasGift = 34, // ��ʥ�����
	EPCT_HallowmasName = 35, // ��ʥ�ڳƺ�
	EPCT_MoShenEquip = 38,	//̨��ħ��װ��
	EPCT_FairySoulStren=39,// �������ǿ�� add by xp
	EPCT_FairySkillbookStren=40,//����������ǿ�� add by xp
	EPCT_BattleFieldCompose=41,//ս����Ʒ�ϳ�  add by xp
	EPCT_ReplaceFabao=42,//������� add by xp
};
//�ϳ�����(�������͵�����)
enum EComposeType
{
	ECOMT_NULL = 0,
	ECOMT_StuffRefine = 10,			//(ʳ��,��)����	10
	ECOMT_StuffProcess,				//(ʳ��,��)�ӹ�	11
	ECOMT_StuffDecompose1,			//����			13(add xtian 2009-10-13)
	ECOMT_RefineDan = 20,			//����			20
	ECOMT_Cooking,					//���			21
	ECOMT_WeaponMake = 30,			//��������		30
	ECOMT_MillstoneMake,			//����Ʒ			31
	ECIMT_WeaponAlbum,				// ����ͼ��		32
	ECOMT_LoricaeMake = 40,			//��������		40
	ECOMT_RollMake,					//����������		41
	ECOMT_ArmorAlbum,				// ����ͼ��		42
	ECOMT_ClothMake = 50,			//��������		50
	ECOMT_FashionMake,				//ʱװ����		51
	ECOMT_ShaZhiMake,				//ɰֽ����		52
	ECOMT_DecorateMake = 60,		//��Ʒ����		60
	ECOMT_ChiselMake,				//�������		61(add xtian 12-3)
	ECOMT_GemProcess,				//��ʯ�ӹ�		62(add xtian 2009-2-12)
	ECOMT_DecorationAlbum,			// ��Ʒͼ��		63
	ECOMT_Refine = 70,				//(��ʯ,��ҩ)���� 70
	//ECOMT_Process = 71,					//(��ʯ,��ҩ)�ӹ� 71
	//ECOMT_StuffDecompose2,			//����2			72(add xtian 2009-10-13)
	ECOMT_EquipdDecompose = 80,		//װ���ֽ�		80
	ECOMT_PointUp,					//�㻯			81
	ECOMT_Rebuild = 90,				//ӡ�Ǹ���		90	add xtian 2009-4-13
	ECOMT_LoongSoulCom,				//����ϳ�		91
	ECOMT_LoongSoulReb,				//�������		92
	ECOMT_GemCompose,				//��ʯ�ϳ�		93
	ECOMT_GemReBuild,				//��ʯ����		94
	ECOMT_GemCarve,					//��ʯ����		95
	ECOMT_PetMake = 100,			//���Ǻϳ�		100 add xtian 2009-8-21
	ECOMT_WeaponProduce = 110,		// ��������		110
	ECOMT_ArmorProdeuce = 111,		// ��������		111
	ECOMT_ClothArmorProduce = 112,	// ��������		112
	ECOMT_DecorationProduce = 113,	// ��Ʒ����		113
	ECOMT_WeaponRefine = 120,		// ������ȡ		120
	ECOMT_ArmorRefine = 121,		// ������ȡ		121
	ECOMT_DecorationRefine = 122,	// ��Ʒ��ȡ		122
	ECOMT_OriStoneRefine = 130,		// ԭʯ����		130
	//ECOMT_JingHuaRefine = 131,		// ��������		131
	ECOMT_TimeLimitWeapon = 140,	// ʱ������		140
	ECOMT_TimeLimitArmor = 141,		// ʱ�޷���		141
	ECOMT_legendDecoration = 142,		// ������Ʒ		142

	ECOMT_FoodProduce = 200,				// ʳ�ļӹ�		200
	ECOMT_SpriteFoodProduce = 210,	// ����ʳ��		210

	ECOMT_ChristmasWeapon = 220,		// ʥ������		220
	ECOMT_SpriteSoul = 230,					// �������		230
	ECOMT_HighSoul = 231,					// �߼����		231
	ECOMT_WashSoul = 232,					// ���ϴ��		232

	ECOMT_CryStal = 240,					// ˮ���ϳ�

	ECOMT_Stole_52 = 250, // 52�ȼ�ʥ��
	ECOMT_Stole_67 = 260, // 67�ȼ�ʥ��
	ECOMT_Stole_82 = 270, // 82�ȼ�ʥ��
	ECOMT_Stole_Blade = 280, // ���ʥ��
	ECOMT_Stole_Bow = 290, // ����ʥ��
	ECOMT_Stole_Valor = 300, // Ǭ��ʥ��
	ECOMT_Stole_Swing = 310, // ����ʥ��

	ECOMT_SoulLinker = 320, // �����

	ECOMT_MoonCake = 330, //�±��ϳ�

	ECOMT_HallowmasGift = 340, // ��ʥ�����
	ECOMT_HallowmasName = 350, // ��ʥ�ڳƺ�

	ECOMT_MoshenEquip = 380,	//ħ��װ��

	ECOMT_FairySoulStren=390,//�������ǿ�� add by xp
	ECOMT_FairySkillbookStren=400,//����������ǿ�� add by xp
	ECOMT_BattleFieldCompose=410,//ս���ϳ� add by xp
	ECOMT_ReplaceFabao=420,//������� add by xp

	//ECOMT_TimeLimitDecoration = 142,// ʱ����Ʒ		142
	ECPOT_TreasureCompose = 900,	// ����ϳ�		900 
	ECOMT_SpritFood1 = 901,			// �´���ʳ	�߻�ţ��
	ECOMT_SpritFood2 = 902,			// �´���ʳ	�߻�����
	ECOMT_SpritFood3 = 903,			// �´���ʳ	�߻��޵�
	ECOMT_GoodPerfume = 904,		// ������ˮ	�߻����
};	

#define IsAdvanceComposeTypeValid(type) (((type) > EADVCOMT_NULL) && ((type) < EADVCOMT_MAX))

//ÿ�����������ĺϳ���������
#define ArtisanSubTypeQuan		ECOMT_StuffDecompose1 - ECOMT_StuffRefine + 1
#define DanTrainingSubTypeQuan	ECOMT_Cooking -  ECOMT_RefineDan + 1
#define SmithSubTypeQuan		ECIMT_WeaponAlbum - ECOMT_WeaponMake + 1
#define CastingSubTypeQuan		ECOMT_ArmorAlbum - ECOMT_LoricaeMake + 1
#define DressmakerSubTypeQuan	ECOMT_ShaZhiMake - ECOMT_ClothMake + 1
#define AechnicsSubTypeQuan		ECOMT_DecorationAlbum - ECOMT_DecorateMake + 1
#define SmiltSubTypeQuan		ECOMT_Refine - ECOMT_Refine + 1
#define PointUpSubTypeQuan		ECOMT_PointUp - ECOMT_EquipdDecompose + 1
#define GodArtisanSubTypeQuan	ECOMT_GemCarve - ECOMT_Rebuild + 1
#define PetSubTypeQuan			ECOMT_PetMake - ECOMT_PetMake + 1
#define EquipSubTypeQuan		ECOMT_DecorationProduce - ECOMT_WeaponProduce + 1
#define EquipRefineTypeQuan		ECOMT_DecorationRefine - ECOMT_WeaponRefine + 1
#define StuffRefineTypeQuan		ECOMT_OriStoneRefine - ECOMT_OriStoneRefine + 1
#define TimeLimitEquipTypeQuan		ECOMT_legendDecoration - ECOMT_TimeLimitWeapon + 1
#define FoodProduceTypeQuan		ECOMT_FoodProduce - ECOMT_FoodProduce + 1
#define SpriteFoodProduceTypeQuan		ECOMT_SpriteFoodProduce - ECOMT_SpriteFoodProduce + 1
#define ChristmasWeaponTypeQuan		ECOMT_ChristmasWeapon - ECOMT_ChristmasWeapon + 1
#define SpriteSoulTypeQuan		ECOMT_WashSoul - ECOMT_SpriteSoul + 1
#define CryStalTypeQuan		ECOMT_CryStal - ECOMT_CryStal + 1
#define Stole_52TypeQuan		ECOMT_Stole_52 - ECOMT_Stole_52 + 1
#define Stole_67TypeQuan		ECOMT_Stole_67 - ECOMT_Stole_67 + 1
#define Stole_82TypeQuan		ECOMT_Stole_82 - ECOMT_Stole_82 + 1
#define Stole_BladeTypeQuan		ECOMT_Stole_Blade - ECOMT_Stole_Blade + 1
#define Stole_BowTypeQuan		ECOMT_Stole_Bow - ECOMT_Stole_Bow + 1
#define Stole_ValorTypeQuan		ECOMT_Stole_Valor - ECOMT_Stole_Valor + 1
#define Stole_SwingTypeQuan		ECOMT_Stole_Swing - ECOMT_Stole_Swing + 1
#define Stole_SoulLinker		ECOMT_SoulLinker - ECOMT_SoulLinker +1
#define Stole_MoonCake		ECOMT_MoonCake - ECOMT_MoonCake +1
#define HallowmasGift		ECOMT_HallowmasGift - ECOMT_HallowmasGift +1
#define HallowmasName	ECOMT_HallowmasName - ECOMT_HallowmasName +1
#define TaiWamMoshenEquip ECOMT_MoshenEquip - ECOMT_MoshenEquip + 1
#define  FairySoulStren  ECOMT_FairySoulStren-ECOMT_FairySoulStren+1//add by xp
#define  FairySkillbookStren ECOMT_FairySkillbookStren-ECOMT_FairySkillbookStren+1//add by xp
#define  BattleFieldCompose ECOMT_BattleFieldCompose-ECOMT_BattleFieldCompose+1//add by xp
#define  ReplaceFabao ECOMT_ReplaceFabao-ECOMT_ReplaceFabao+1//add by xp

//�ϳ��䷽����
enum EFormulaType
{
	EFORMT_NULL	= 0,
	EFORMT_OreRefine,		//��ʯ�ᴿ	1
	EFORMT_HerbalRefine,	//��ҩ�ᴿ	2
	EFORMT_ClothRefine,		//���Ͼ���	3
	EFORMT_FoodRefine,		//ʳ�ľ���	4
	EFORMT_OreProcess,		//��ʯ�ӹ�	5
	EFORMT_HerbalProcess,	//��ҩ�ӹ�	6
	EFORMT_ClothProcess,	//���ϼӹ�	7
	EFORMT_FoodProcess,		//ʳ�ļӹ�	8
	EFORMT_ThewDrug,		//����ҩ		9
	EFORMT_EnergyDrug,		//����ҩ		10
	EFORMT_RestoreDrug,		//ȫ��ҩ		11
	EFORMT_GodDrug,			//��ҩ		12
	EFORMT_SecondReDrug,	//���ҩ		13
	EFORMT_InterinjuryDrug,	//����ҩ		14
	EFORMT_NormalDrug,		//��ͨҩ		15
	EFORMT_Wine,			//��			16
	EFORMT_Cate,			//��ʳ		17
	EFORMT_Glave,			//��			18
	EFORMT_Sword,			//��			19
	EFORMT_Halberd,			//����		20
	EFORMT_Bow,				//��			21
	EFORMT_SmallOrgan,		//С�ͷ���	22
	EFORMT_BigOrgan,		//���ͷ���	23
	EFORMT_Instrument,		//��			24
	EFORMT_Millstone,		//ħʯ		25
	EFORMT_Armet,			//ͷ��		26
	EFORMT_UpperLoricae,	//�������	27
	EFORMT_LowerLoricae,	//�������	28
	EFORMT_Armguard,		//����		29
	EFORMT_BattleShoe,		//սѥ		30
	EFORMT_Roll,			//������		31
	EFORMT_Cap,				//ñ��		32
	EFORMT_Jupe,			//����		33	
	EFORMT_Pants,			//����		34
	EFORMT_Glove,			//����		35
	EFORMT_Shoe,			//Ь��		36
	EFORMT_Mantle,			//����		37
	EFORMT_FashionCap,		//ʱװñ��	38
	EFORMT_FashionJupe,		//ʱװ����	39
	EFORMT_FashionPants,	//ʱװ����	40
	EFORMT_FashionGloave,	//ʱװ����	41
	EFORMT_FashionShoe,		//ʱװЬ��	42
	EFORMT_FashionMantle,	//ʱװ����	43
	EFORMT_FingerRing,		//��ָ		44
	EFORMT_Caestus,			//����		45
	EFORMT_Necklace,		//����		46
	EFORMT_Chisel,			//���		47
	EFORMT_YuanRockIncise,	//ԭʯ�и�	48
	EFORMT_GeBlade,			//�		49
	EFORMT_GangRebuild,		//�ֶ�ӡ���� 50 add xtian 2009-4-13
	EFORMT_YuRebuild,		//��ĥӡ����
	EFORMT_YinRebuild,		//����ӡ����
	EFORMT_JinRebuild,		//����ӡ����
	EFORMT_JinLoongSoul,	//������
	EFORMT_MuLoongSoul,		//ľ����
	EFORMT_ShuLoongSoul,	//ˮ����
	EFORMT_HuoLoongSoul,	//������
	EFORMT_TuLoongSoul,		//������
	EFORMT_JinLoongSoulReb,	//���������
	EFORMT_MuLoongSoulReb,	//ľ�������
	EFORMT_ShuLoongSoulReb,	//ˮ�������
	EFORMT_HuoLoongSoulReb,	//���������
	EFORMT_TuLoongSoulReb,	//���������
	EFORMT_2Gem,			//������ʯ
	EFORMT_3Gem,			//������ʯ
	EFORMT_4Gem,			//�ļ���ʯ
	EFORMT_2GemRebuild,		//������ʯ����
	EFORMT_3GemRebuild,		//������ʯ����
	EFORMT_4GemRebuild,		//�ļ���ʯ����
	EFORMT_RefineDanOther,	//��������
	EFORMT_WeaponMakeOther, //������������
	EFORMT_LoricaeMakeOther,//������������
	EFORMT_ClothMakeOther,	//������������
	EFORMT_DecorateMakeOther,//��Ʒ��������
	EFORMT_ShangSoul,			//��Ʒ����
	EFORMT_LingSoul,			//��Ʒ����
	EFORMT_XianSoul,			//��Ʒ����
	EFORMT_ShengSoul,			//ʥƷ����
	EFORMT_MakeJade,			//����
	EFORMT_PetFood,				//���޿���
	EFORMT_GemBlade,			//��ʯ��
	EFORMT_GemCarve,			//��ʯ����
	EFORMT_ClothDecompose,		//���Ͻ���
	EFORMT_FoodDecompose,		//ʳ�Ľ���
	EFORMT_OreDecompose,		//��ʯ����
	EFORMT_HerbalDecompose,		//��ҩ����
	EFORMT_Album,				// ͼ��
	EFORMT_5Gem,			// �弶��ʯ
	EFORMT_6Gem,			// ������ʯ
	EFORMT_7Gem,			// �߼���ʯ
	EFORMT_5GemRebuild,		// �弶��ʯ����
	EFORMT_6GemRebuild,		// ������ʯ����
	EFORMT_7GemRebuild,		// �߼���ʯ����

	EFORMT_WeaponRefine = 120,		// ������ȡ		120
	EFORMT_ArmorRefine = 121,		// ������ȡ		121
	EFORMT_DecorationRefine = 122,	// ��Ʒ��ȡ		122
	EFORMT_OriStoneRefine = 130,	// ԭʯ����		130
	EFORMT_JingHuaRefine = 131,		// ��������		131

	EFORMT_TimeLimitWeapon_40 = 140,		// 40��ʱ������		141
	EFORMT_TimeLimitWeapon_60 = 141,		// 60��ʱ������		141
	EFORMT_TimeLimitWeapon_80 = 142,		// 80��ʱ������		142
	EFORMT_TimeLimitWeapon_100 = 143,		// 100��ʱ������		143
	EFORMT_TimeLimitArmor_40 = 144,			// 40��ʱ�޷���		144
	EFORMT_TimeLimitArmor_60 = 145,			// 60��ʱ�޷���		145
	EFORMT_TimeLimitArmor_80 = 146,			// 80��ʱ�޷���		146
	EFORMT_TimeLimitArmor_100 = 147,		// 100��ʱ�޷���		147
	EFORMT_TimeLimitDecoration_40 = 148,	// 40��ʱ����Ʒ		148
	EFORMT_TimeLimitDecoration_60 = 149,	// 60��ʱ����Ʒ		149
	EFORMT_TimeLimitDecoration_80 = 150,	// 80��ʱ����Ʒ		150
	EFORMT_TimeLimitDecoration_100 = 151,	// 100��ʱ����Ʒ		151

	EFORMT_2LevelGemUpGrade = 161,	// ������ʯ����		161
	EFORMT_3LevelGemUpGrade = 162,	// 3����ʯ����		162
	EFORMT_4LevelGemUpGrade = 163,	// 4����ʯ����		163
	EFORMT_5LevelGemUpGrade = 164,	// 5����ʯ����		164
	EFORMT_6LevelGemUpGrade = 165,	// 6����ʯ����		165
	EFORMT_7LevelGemUpGrade = 166,	// 7����ʯ����		166
	EFORMT_8LevelGemUpGrade = 167,	// 8����ʯ����		167
	EFORMT_9LevelGemUpGrade = 168,	// 9����ʯ����		168

	EFORMT_GodWeapon_60 = 171,		// 60��αħ������	171
	EFORMT_GodWeapon_80 = 172,		// 80��αħ������	172
	EFORMT_GodArmor_60 = 173,		// 60��αħ�����	173
	EFORMT_GodArmor_80 = 174,		// 80��αħ�����	174
	EFORMT_GodDecoration_60 = 175,	// 60��αħ����Ʒ	175
	EFORMT_GodDecoration_80 = 176,	// 80��αħ����Ʒ	176

	EFORMT_WaterProduct = 200,	// ˮ���ӹ�	200
	EFORMT_EarthProduct = 201,		// ����ӹ�	201

	EFORMT_NormalFood= 210,		// ��ͨʳ��	210
	EFORMT_SpecFood = 211,			// ����ʳ��	211

	EFORMT_OrangeChristmasWeapon = 220,			// ��ɫʥ������	220
	EFORMT_PurpleChristmasWeapon = 221,				// ��ɫʥ������	221

	EFORMT_SpriteSoulLevel_2 = 230,			// 2���������	230
	EFORMT_SpriteSoulLevel_3 = 231,			// 3���������	231
	EFORMT_SpriteSoulLevel_4 = 232,			// 4���������	232
	EFORMT_SpriteSoulLevel_5 = 233,			// 5���������	233
	EFORMT_SpriteSoulLevel_6 = 234,			// 6���������	234
	EFORMT_SpriteSoulLevel_7 = 235,			// 7���������	235
	EFORMT_SpriteSoulLevel_8 = 236,			// 8���������	236
	EFORMT_SpriteSoulLevel_9 = 237,			// 9���������	237
	EFORMT_SpriteSoulLevel_10= 238,			// 10���������	238

	EFORMT_CryStal_Sky= 240,						// ��֮ˮ���ϳ�
	EFORMT_CryStal_Earth = 241,						// ��֮ˮ���ϳ�
	
	EFORMT_Stole_Body_2 = 250,						// ����2
	EFORMT_Stole_Legs_2 = 251,						// ʥ��ͷ2
	EFORMT_Stole_Wrist_2 = 252,						// ʥ������2
	EFORMT_Stole_Feet_2 = 253,						// ʥ����һ2
	EFORMT_Stole_Back_2 = 254,						// ʥ������2
	EFORMT_Stole_Finger_2 = 255,						// Ь��2
	EFORMT_Stole_Waist_2 = 256,						// ����2
	EFORMT_Stole_Neck_2 = 257,						// ��Ʒ2

	EFORMT_Stole_Body_3 = 260,						// ����3
	EFORMT_Stole_Legs_3 = 261,						// ʥ��ͷ3
	EFORMT_Stole_Wrist_3 = 262,						// ʥ������3
	EFORMT_Stole_Feet_3 = 263,						// ʥ������3
	EFORMT_Stole_Back_3 = 264,						// ʥ������3
	EFORMT_Stole_Finger_3 = 265,						// Ь��3
	EFORMT_Stole_Waist_3 = 266,						// ����3
	EFORMT_Stole_Neck_3 = 267,						// ��Ʒ3

	EFORMT_Stole_Body_4 = 270,						// ����4
	EFORMT_Stole_Legs_4 = 271,						// ʥ��ͷ4
	EFORMT_Stole_Wrist_4 = 272,						// ʥ������4
	EFORMT_Stole_Feet_4 = 273,						// ʥ������4
	EFORMT_Stole_Back_4 = 274,						// ʥ������4
	EFORMT_Stole_Finger_4 = 275,						// Ь��4
	EFORMT_Stole_Waist_4 = 276,						// ����4
	EFORMT_Stole_Neck_4 = 277,						// ��Ʒ4

	EFORMT_Stole_Blade_85 = 280,						// 85�������ʥ��
	EFORMT_Stole_Blade_95 = 281,						// 95�������ʥ��

	EFORMT_Stole_Bow_85 = 290,						// 85��������ʥ��
	EFORMT_Stole_Bow_95 = 291,						// 95��������ʥ��

	EFORMT_Stole_Valor_85 = 300,						// 85��Ǭ����ʥ��
	EFORMT_Stole_Valor_95 = 301,						// 95��Ǭ����ʥ��

	EFORMT_Stole_Swing_85 = 310,						// 85��������ʥ��
	EFORMT_Stole_Swing_95 = 311,						// 95��������ʥ��

	EFORMT_SoulLinker = 320,						// ������ϳ�

	EFORMT_MoonCake = 330,						// �±��ϳ�

	EFORMT_HallowmasGift = 340,						// ��ʥ�����
	EFORMT_HallowmasName = 350,						// ��ʥ�ڳƺ�

	EFORMT_CryStalMake = 360,						// ˮ���ϳ�
	EFORMT_EquipGrow = 370,						// װ���ɳ�

	EFORMT_Level8StrenSoul=391,               //�˼�ǿ����� add by xp
	EFORMT_Level9StrenSoul=392,               //�ż�ǿ����� add by xp
	EFORMT_Level10StrenSoul=393,            //ʮ��ǿ����� add by xp
	EFORMT_FairyStrenSkillbook=401,          //����ǿ�������� add by xp
	EFORMT_BattleFieldCompose=411,       //ս���ϳ� add by xp
	EFORMT_ReplaceFabao80=421,			  //�������[80]  add by xp
	EFORMT_ReplaceFabao85=422,			  //�������[85]  add by xp
	EFORMT_ReplaceFabao90=423,			  //�������[90]  add by xp
	EFORMT_ReplaceFabao95=424,			  //�������[95]  add by xp
	EFORMT_ReplaceFabao99=425,			  //�������[99]  add by xp
	EFORMT_ReplaceFabao100=426,			  //�������[100]  add by xp

	EFORMT_SpriteFood1 = 901,		// �´���ʳ �߻�ţ��
	EFORMT_SpriteFood2 = 902,		// �´���ʳ �߻�����
	EFORMT_SpriteFood3 = 903,		// �´���ʳ �߻��޵�
	EFORMT_GoodFerfume = 904,		// ������ˮ �߻����

	EFORMT_SpriteSoul_2 = 2301,		// 2���������
	EFORMT_SpriteSoul_3 = 2302,		// 3���������
	EFORMT_SpriteSoul_4 = 2303,		// 4���������
	EFORMT_SpriteSoul_5 = 2304,		// 5���������
	EFORMT_SpriteSoul_6 = 2305,		// 6���������
	EFORMT_SpriteSoul_7 = 2306,		// 7���������
	EFORMT_SpriteSoul_8 = 2307,		// 8���������
	EFORMT_SpriteSoul_9 = 2308,		// 9���������
	EFORMT_SpriteSoul_10 = 2309,		// 10���������

	EFORMT_HighSoul_5 = 2311,		// 5���߼����
	EFORMT_HighSoul_6 = 2312,		// 6���߼����
	EFORMT_HighSoul_7 = 2313,		// 7���߼����
	EFORMT_HighSoul_8 = 2314,		// 8���߼����
	EFORMT_HighSoul_9 = 2315,		// 9���߼����
	EFORMT_HighSoul_10 = 2316,		// 10���߼����

	EFORMT_WashSoul_1 = 2321,		// 1�����ϴ��
	EFORMT_WashSoul_2 = 2322,		// 2�����ϴ��
	EFORMT_WashSoul_3 = 2323,		// 3�����ϴ��
	EFORMT_WashSoul_4 = 2324,		// 4�����ϴ��
	EFORMT_WashSoul_5 = 2325,		// 5�����ϴ��
	EFORMT_WashSoul_6 = 2326,		// 6�����ϴ��
	EFORMT_WashSoul_7 = 2327,		// 7�����ϴ��
	EFORMT_WashSoul_8 = 2328,		// 8�����ϴ��
	EFORMT_WashSoul_9 = 2329,		// 9�����ϴ��
	EFORMT_WashSoul_10 = 2330,		// 10�����ϴ��

};

//�䷽��Դ
enum EFormulaFrom
{
	EFormulaFrom_NULL = 0,
	EFormulaFrom_Skill,					//����
	EFormulaFrom_NPC,					//NPC
	EFormulaFrom_Item,					//��Ʒ
	EFormulaFrom_SkillorNPC,			//���ܼ�NPC
	EFormulaFrom_SkillorItem,			//���ܼ���Ʒ
	EFormulaFrom_NPCorItem,				//NPC����Ʒ
};

//�߼��ϳ�����(�������͵����֧࣬�ֶ�����Ϻϳɶ�������������һ��)
enum EAdvanceComposeType
{
	EADVCOMT_NULL = 0,
	EADVCOMT_Fashion = 1,				// ʱװ�ϳ�
	EADVCOMT_Gem = 2,					// ��ʯ�ϳ�
	EADVCOMT_GemBluePrint = 3,			// ��ʯͼֽ
	EADVCOMT_YuanGu = 4,				// Զ��װ��ͼֽ	
	EADVCOMT_ShengWen = 5,				// ʥ�ƺϳ�
	EADVCOMT_MAX = 6,
};


//װ��ǿ������
enum EConsolidateType
{
	ECliDateT_NULL 		= 0,		//��
	ECliDateT_Posy		= 1,		//����
	ECliDateT_Engrave	= 2,		//�Կ�
	ECliDateT_Quench	= 3,		//���
	ECliDateT_Casting	= 4,		//����
};

//����ǿ������
enum EAllConsolidateType
{
	EAllCliDateT_NULL 		= 0,		//��
	EAllCliDateT_Posy		= 1,		//����
	EAllCliDateT_Engrave	= 2,		//�Կ�
	EAllCliDateT_Quench		= 3,		//���
	EAllCliDateT_Chisel		= 4,		//����
	EAllCliDateT_HoleGem	= 5,		//��Ƕ
	EAllCliDateT_Brand		= 6,		//��ӡ
	EAllCliDateT_LongFu		= 7,		//����
};

//ǿ����������
enum EPosyAtt
{
	EPosyAtt_NULL		= 0,	//��	
	EPosyAtt_MaxHP		= 1,    //�������
	EPosyAtt_MaxMP		= 2,	//�������
	EPosyAtt_ExAttack	= 3,	//�⹦����
	EPosyAtt_InAttack	= 4,	//�ڹ�����
	EPosyAtt_ExDefense	= 5,	//�⹦����
	EPosyAtt_InDefense	= 6,	//�ڹ�����

	EPosyAtt_End,
};
//ǿ���Կ�����
enum EEngraveAtt
{
	EEngraveAtt_NULL		= 0,	//��
	EEngraveAtt_WeaponDmg	= 1,	//�����˺�
	EEngraveAtt_WeaponSoul  = 2,	//���
	EEngraveAtt_Armor		= 3,	//���߻���(��)
	EEngraveAtt_Deration	= 4,	//���߻���(��)
	EEngraveAtt_End,
};

//�������
enum EQuenchType
{
	EEQuenchType_Metal		= 0,	//���
	EEQuenchType_Wood,				//ľ��
	EEQuenchType_Water,				//ˮ��
	EEQuenchType_Fire,				//���
	EEQuenchType_Soil,				//����
};

//���Ľ׶�
enum ESpinStep
{
	ESpinStep_Null		= 0,	//δ����
	ESpinStep_Basic,			//����
	ESpinStep_Middle,			//�м�
	ESpinStep_Senior,			//�߼�
	ESpinStep_End,
};

//���Ľ׶ζ�Ӧ�ȼ�
enum ESpinStepEquipLevel
{
	ESSEL_Null		= 1,			//δ����
	ESSEL_Basic		= 30,			//����
	ESSEL_Middle	= 60,			//�м�
	ESSEL_Senior	= 90,			//�߼�
	ESSEL_End,
};

enum ESoarCastingType
{
	ESCT_Weapon			= 0,		//����
	ESCT_Defence		= 1,		//����
	ESCT_Decoration		= 2			//��Ʒ
};

//���ĳɹ���ԭ��
struct tagSpinSuccessRateProto
{
	DWORD				dwSpinItemNum;	//��˿�ߵ���

	DWORD				dwSuccessRate[ESpinStep_End]; //�ɹ���

	tagSpinSuccessRateProto()
	{
		memset(this,0,sizeof(*this));
	}

};

//map key
struct tagSpinKey
{
	DWORD				dwSpinFashionTypeID;
	ESuitAttType		eAttType; //ǿ����װ��������

	tagSpinKey()
	{
		dwSpinFashionTypeID = GT_INVALID;
		eAttType = ESSAT_Null;
	}

	tagSpinKey(DWORD dwSpinFashionTypeID,ESuitAttType eAttType)
	{
		this->dwSpinFashionTypeID = dwSpinFashionTypeID;
		this->eAttType = eAttType;
	}
	bool operator<(const tagSpinKey& ver) const
	{
		if(dwSpinFashionTypeID<ver.dwSpinFashionTypeID)
			return true;
		else if(dwSpinFashionTypeID>ver.dwSpinFashionTypeID)
			return false;

		if( eAttType<ver.eAttType)
			return true;
		else if(eAttType>ver.eAttType)
			return false;

		return false;
	}

};


//����ԭ��
struct tagSpinProto
{
	DWORD				dwSpinFashionTypeID[ESpinStep_End];		//����װ����typeid

//	TCHAR				szName[ESpinType_End][X_SHORT_NAME];	//װ������

//	DWORD				dwSuitID[ESpinType_End];				//��װid

	ESuitAttType		eAttType; //ǿ����װ��������

	tagSpinProto()
	{
		memset(this,0,sizeof(*this));
	}
};

//��ǿ��װ����λ
struct tagConsolidatePos
{
	EEquipPos ePos;
	BYTE	  byConsolidate;
};

//ǿ������
struct tagConsolidateStuff
{
	DWORD				dwStuffID;			//����ID
	EStuffType			eStuffType;			//��������
	DWORD				dwStuffNum;			//��������
};

//�����ϳɲ���
struct tagProduceSutff
{
	DWORD				dwStuffID;			//����ID
	EStuffType			eStuffType;			//��������
	DWORD				dwStuffNum;			//��������	
	DWORD				dwFailDeductionNum;		// ʧ��ʱ�۳�ԭ������
};

//���ƽṹ��
struct tagPosyProto
{

	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EPosyAtt			ePosyAtt;				//��������
	BYTE				byPosyTimes;			//���ƴ���
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	FLOAT 				fcoefficientA; 			//���Լ�ֵ��ʽ����A
	FLOAT 				fcoefficientB; 			//���Լ�ֵ��ʽ����B
	FLOAT 				fcoefficientC; 			//���Լ�ֵ��ʽ����C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//��ǿ��װ����λ
};

//�Կ̽ṹ��
struct tagEngraveProto
{

	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EEngraveAtt			eEngraveAtt;			//�Կ�����
	BYTE				byEngraveTimes;			//�Կ̴���
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	FLOAT 				fcoefficientA; 			//���Լ�ֵ��ʽ����A
	FLOAT 				fcoefficientB; 			//���Լ�ֵ��ʽ����B
	FLOAT 				fcoefficientC; 			//���Լ�ֵ��ʽ����C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//��ǿ��װ����λ
};

//�������
struct tagQuenchAtt
{
	EWuXing				eWuXing;
	INT					nWuXingValue;
};

//���ṹ��
struct tagQuenchProto
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EQuenchType			eQuenchType;			//�������
	tagQuenchAtt		srcQuenchAtt;			//ԭ���Լ�����ֵ
	tagQuenchAtt		dstQuenchAtt;			//Ŀ�����Լ�����ֵ
	DWORD				dwPotValConsume;		//Ǳ��ֵ����
	DWORD				dwMoneyConsume;			//��Ǯ����
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //ǿ������
};

//�����ϳ�
struct tagProduceProto
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EProduceType		eProType;				//��������
	EComposeType		eComType;				//�ϳ�����
	EFormulaType		eFormType;				//�䷽����
	EFormulaFrom		eFormFrom;				//�䷽��Դ
	DWORD				dwFormLvl;				//�䷽�ȼ�
	DWORD				dwProItemTypeID;		//�ϳɵ���Ʒ
	DWORD				dwProQuantity;			//�ϳɵ�����
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwVitalityConsume;		//��������
	DWORD				dwMasterIncre;			//����������
	DWORD				dwMoneyConsume;			//��Ǯ����
	tagProduceSutff		ProduceStuff[MAX_PRODUCE_STUFF_QUANTITY];//��������
};

#define MAX_RANDOMPRODUCE_STUFF 2			// ��Ҫ�Ĳ���
#define MAX_RANDOMPRODUCE_NUM 20			// ����������������Ʒ

//ʱװ�ϳ�
struct tagRandomProduceProto
{
	DWORD				dwID;									// �䷽ID
	EAdvanceComposeType		eRProType;								// ��������
	DWORD				dwDesigner;								// ����ͼ
	DWORD				dwStuff[MAX_RANDOMPRODUCE_STUFF];		// �������
	INT16				n16StuffNum[MAX_RANDOMPRODUCE_STUFF];	// ���������Ŀ
	DWORD				dwVitalityConsume;						// ��������
	DWORD				dwMoneyConsume;							// ��Ǯ����
	DWORD				dwItemTypeID[MAX_RANDOMPRODUCE_NUM];	// �����ɵ���ƷtypeID
};

// ����
struct tagGodStrengthen
{
	DWORD dwID;					// �����ȼ�
	DWORD dwExp;				// ��ǰ�ȼ����辭��
	DWORD dwGreenExp;
	DWORD dwBlueExp;
	DWORD dwOrangeExp;
	DWORD dwPurpleExp;
	DWORD dwPinkExp;
	DWORD dwCost;				// ��Ǯ����
};

//ǿ������ǿ��������
struct tagConsolidateRoleAtt
{
	ERoleAttribute		eRoleAtt;
	INT					nAttVal;
};

//��Ƕ,��ӡ,����(ǿ������)
struct tagConsolidateItem
{
	DWORD					dwID;						//���ߵ�ID
	DWORD					dwPotValConsume;			//Ǳ��ֵ����
	INT						nSuccessRate;				//�ɹ���
	EWuXing					eWuXing;					//��������
	tagConsolidateRoleAtt	tagRoleAtt[MAX_CONSOLIDATE_ROLEATT];		//ǿ������	
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY + MAX_CONSOLIDATE_POS_HOLYEQUIP];	//��ǿ��װ����λ
};

//  �ֽ�㻯�����Ʒ
struct tagOutputStuff
{
	DWORD				dwStuffTypeID;			//�������TypeID
	INT					nPerMinVal;				//������С����
	INT					nPerMaxVal;				//�����������
	INT					nSucMinVal;				//�ɹ���С����
	INT					nSucMaxVal;				//�ɹ��������
	INT					nRate;					//����
};

// �ֽ�㻯
struct tagDeComposeProto
{
	DWORD				dwID;					//�䷽ID
	TCHAR				szName[X_SHORT_NAME];	//�䷽����
	EComposeType		eComType;				//�ϳ�����
	BYTE				byLevel;				//װ���ȼ�
	BYTE				byQuality;				//װ��Ʒ��
	EItemType			eType;					//װ������
	EItemTypeEx			eTypeEx;				//��������
	EEquipPos			ePos;					//װ��λ��
	DWORD				dwFormLvl;				//�䷽�ȼ�
	EFormulaFrom		eFormFrom;				//�䷽��Դ
	INT					nSuccessRate;			//�ɹ���
	DWORD				dwProduceTime;			//������ʱ
	DWORD				dwVitalityConsume;		//��������
	DWORD				dwMasterIncre;			//����������
	DWORD				dwMoneyConsume;			//��Ǯ����
	tagOutputStuff		OutputStuff[MAX_OUTPUT_QUANTITY];			//�������		
};

// ��ȡ
struct tagExtractProto
{
	DWORD				dwEquipID;
	DWORD				dwProduceTime;							//������ʱ
	DWORD				dwVitalityConsume;						//��������
	DWORD				dwMoneyConsume;							//��Ǯ����
	BYTE				byQuality;								//װ��Ʒ��
	DWORD				dwItemID[X_EXTRACT_MAX_PRODUCTION];		//�ֽ�����ID
	INT					nItemMaxNum[X_EXTRACT_MAX_PRODUCTION];	//�ֽ�����������
	INT					nItemMinNum[X_EXTRACT_MAX_PRODUCTION];	//�ֽ������С����
	DWORD				dwRate[X_EXTRACT_MAX_PRODUCTION];		//�������ɼ���
};

//	��������
struct tagCastingStuff
{
	DWORD					dwID;				//����ID
	DWORD					dwStuffNum;			//��������	

};
// ����װ������
struct tagSoarCastingProto
{
	DWORD					dwID;					//�䷽ID
	TCHAR					szName[X_SHORT_NAME];	//�䷽����
	BYTE					byLevel;				//װ���ȼ�
	ESoarCastingType		eType;					//��������
	DWORD					dwPotvalConsume;		//Ǳ��ֵ����
	INT						nSuccessRate;			//�ɹ���
	tagConsolidateRoleAtt	CastingRoleAtt[MAX_CASTING_ROLEATT];		//ǿ������	
	tagCastingStuff			CastingStuff[MAX_OUTPUT_QUANTITY];		//����
};

//Ԫ��װ��ǿ�����Խṹ

//--------------------------------------------------------------------------------------------
//Ԫ��װ������
//--------------------------------------------------------------------------------------------
struct tagStarEnhanceProto
{
	BYTE	byID;			//Ԫ��װ�����Ǽ�
	DWORD	dwCostItemId;	//ǿ���Ǽ����ĵĵ��ߵ�ID
	UINT16	nCostItemNum;	//ǿ���Ǽ����ĵĵ��ߵ�����
	UINT16	nRate;			//ǿ����Ӧ�Ǽ��ĳɹ�����
	BYTE	byFailLevel;	//ǿ���Ǽ�ʧ�ܺ��˻صĵȼ�
	DWORD	dwCost;			//ǿ���Ǽ����ĵĽ�Ǯ��
	UINT	nStunt;			//�����˺��ӳ�
	UINT	nHitRate;		//���мӳ�
	UINT	nCritRate;		//�����ӳ�
	UINT	nCritaMount;	//�������ӳ�
	UINT	nExAttack;		//�⹦�����ӳ�
	UINT	nInAttack;		//�ڹ������ӳ�
	UINT	nAccuRate;		//��׼�ӳ�
	UINT	nExDamage;		//�����˺��ӳ�
	UINT	nNegToughness;	//�������Լӳ�
	UINT	nMorale;		//ʿ���ӳ�
	tagStarEnhanceProto()
	{
		memset(this,0,sizeof(*this));
	}
};

#pragma pack(pop)

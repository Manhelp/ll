#pragma once

#include "RoleDefine.h"
#include "container_define.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/reputation.h"
#include "specfunc_define.h"
#include "../WorldDefine/container_define.h"


#pragma pack(push, 1)

class ItemScript;

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
const INT MAX_ITEM_TYPE_NUM				= 100000;	// ��̬���У���Ʒװ�����������

const INT MAX_ROLEATT_BASE_EFFECT		= 6;	// װ��ԭ���п���д��Ӱ�����Ը���

const INT MAX_EQUIPATT_BASE_EFFECT		= 7;	// ʥ��ԭ���п���д��Ӱ��ʥ�����Ը���

const INT MAX_POSY_TIMES				= 20;	// һ��װ�������������Ĵ���
const INT MAX_ROLEATT_POSY_EFFECT		= 4;	// һ��װ�����������Ӱ������Ը���

const INT MAX_ENGRAVE_TIMES				= 20;	// һ��װ�����������Կ̴���
const INT MAX_ROLEATT_ENGRAVE_EFFECT	= 4;	// һ��װ�����Կ����Ӱ������Ը���

const INT MAX_ROLEATT_ENHANCE_EFFECT	= 3;	// ǿ��װ���Ĳ���Ӱ������Ը���
const INT MAX_EQUIPHOLE_NUM				= 7;	// һ��װ������������Ƕ������

const INT MAX_BRAND_LEVEL				= 10;	// װ������ӡ�ȼ�����

const INT MAX_LONGHUN_LEVEL				= 10;	// ����ȼ�����


const INT ABRASION2USETIMES				= 25;	// ����ʹ�ô�����ո�¶Ƚ���.1��ո�¶� = ʹ��50��

const INT MAX_ITEM_DCTIME				= 7 * 24 * 60 * 60;	// ��Ʒ���ȴʱ��(��λ����)

const INT MAX_EQUIP_ATTACH_NUM          = 20;    // װ�����ر���ģ���������

const INT MAX_PEREQUIP_SUIT_NUM			= 3;	// һ��װ����������3����װ

const INT MAX_EQUIP_BUFF_NUM			= 5;	// װ���ϴ���buff������

const INT MAX_ROLEATT_SPIN_EFFECT		= 3;	// ���Ķ��������Ե�Ӱ��

const INT MAX_CASTING_PARAM_NUM			= 5;	// �������������������Ϻͽ�ɫ���Ը���

const INT MAX_FCONSOLIDATE_NUM		= 5;	// F�ƻ�װ�����ǿ������Ӱ��

const INT MIN_STORE_EXP_FABAO_LEVEL = 40;  // ���Լ������������ӡ����С����

const INT MIN_Sprite_Born_LEVEL = 40;  // ���Լ���������������С����

//const INT EQUIP_GROW_STAR_MAX_NUM = 7; // װ���ɳ����׶ζ�Ӧ���ǵ�Ʒ��

const INT EQUIP_GROW_ADD_ATT_MAX_NUM = 5; // װ���ɳ�Ӱ������Ե�������

const INT EQUIP_GROW_MAX_LEVEL = 11; // װ���ɳ�����ߵȼ�

#define MFaBaoStoredMaxExp(Level, Intelligence) (200000.0f*((Level/20.0f)*(Level/20.0f))*((Intelligence/50.0f)*(Intelligence/50.0f)))

#define MAX_LIMITED_ITEM_PROTO	100	// ȫ��ϡ����Ʒ���������

const INT SOUL_CRYSTAL_ATT_MAX_NUM = 8; // �꾧�ɳ����������� 

const INT SOUL_CRYSTAL_MAX_LVL = 10; // �꾧�����ȼ�
//Ԫ����ǵȼ�����
const INT8 MAX_STARENHANCE_LEVEL	= 15;	//�����ǵȼ�
//Ԫ����Ƕ�����Ͼ���¶��������ֵ
const INT8 IMPROVE_SOULINLAY_RATE	= 10;

const INT		MAX_ROLE_HOLY_VALUE					= 600;	//��ɫʥ��ֵ����
const INT		HOLY_MIN_VALUE						= 100;	// ��ɫ�����ٻ�ʥ���������Сʥ��ֵ

const INT		MAX_HOLY_NUM   = 10;		// ��ɫ���ϵ����ʥ������

const INT		MAX_HOLYEAT_COUNT   = 5;		// ʥ��ÿ������������������


const INT		UNEQUIP_HOLY_COST_SILVER   = 20*10000;			// ʥ��ϴӡ���Ľ����
const INT		ENHANCE_HOLY_EQUIP_COST_SILVER   = 20*10000;		// ʥ�Ƴ������Ľ����
const INT		COST_HOLY_ADD   = 10;						// ʥ�ƻ������Գ��ܣ����ӵ���������ֵ

const INT		MIN_FABAO_LEVEL   = 40;		// ������������С�ȼ�
//-----------------------------------------------------------------------------
// ö�ٶ���
//-----------------------------------------------------------------------------
enum EItemType
{
	EIT_Null		= 0,
	EIT_Expend		= 1,	// ����Ʒ
	EIT_Unexpend	= 2,	// ������Ʒ
	EIT_Spec		= 3,	// ������Ʒ
	EIT_Pet			= 4,	// ����
	EIT_Bag			= 5,	// ��Ʒ��
	EIT_Mall		= 6,	// �̳ǵ���

	EIT_Weapon		= 7,	// ����
	EIT_ClothArmor	= 8,	// ����
	EIT_Decoration	= 9,	// ��Ʒ
	EIT_Fashion		= 10,	// ʱװ
	EIT_Armor		= 11,	// ����
	// Jason 2010-4-12 v2.0.0
	EIT_FaBao		= 12,	// ����
	EIT_NoviceEquip = 13,	// ����װ
	EIT_SoulCrystal = 14,	// �꾧
	EIT_Holy        = 15,	//ʥ�飬ʥħ���Ԫ����һ��װ��������װ�������ٷ�װ��~
	EIT_HolyEquip	= 16, //Ԫ��װ��
	//EIT_TransItem  = 16, //�������
};

enum EItemTypeEx
{
	EITE_Null		= 0,
	EITE_Resume		= 1,	// �ָ���
	EITE_Advance	= 2,	// ǿ����
	EITE_Other		= 3,	// ����
	EITE_Material	= 4,	// ԭ����
	EITE_Capture	= 5,	// ս��Ʒ
	EITE_Quest		= 6,	// ������Ʒ
	EITE_Book		= 7,	// �ؼ�
	EITE_Pet		= 8,	// ����
	EITE_Ride		= 9,	// ����
	EITE_Expend		= 10,	// ����
	EITE_Time		= 11,	// ʱ��

	EITE_Sword		= 12,	// ��
	EITE_Blade		= 13,	// ��
	EITE_Wand		= 14,	// ����
	EITE_Bow		= 15,	// �� 
	EITE_Faqi		= 16,	// ����
	EITE_Qin		= 17,	// ��
	// Jason 2010-6-18 ��������
	EITE_Neck		= 25,	// ����
	EITE_Finger	= 26,	// ��ָ
	EITE_Waist		= 27,	// ����
	EITE_Head		= 28,	// ͷ��
	EITE_Body		= 29,	// ����
	EITE_Legs		= 30,	// ����
	EITE_Hand		= 31,	// �ֲ�
	EITE_Feet		= 32,	// �㲿
	EITE_Back		= 33,	// ����
	EITE_Face		= 34,	// �沿
	EITE_Weapon	= 35,	// ����
	//
	EITE_CastingAccessories	= 36,		// ��������
	EITE_MixingEquipAccessories = 37, // ��������
	EITE_AdvancedCastingAccessories	= 38,		// ��������
	EITE_Cloak		= 39,	// ����
	EITE_Armor		= 40,	// ����
	EITE_SoulCrystalGod = 41, // ��꾧
	EITE_SoulCrystalEvil = 42, // ħ�꾧
	EITE_War			= 43,	// ר������ [4/26/2012 zhangzhihua]

// 	EITE_TianHun			= 43,			//���		//��Ԫ��װ��ɾ����Ϣ
// 	EITE_DiHun				= 44,			//�ػ�
// 	EITE_MingHun			= 45,			//����
// 	EITE_TianChong			= 46,			//���
// 	EITE_LingHui			= 47,			//���
// 	EITE_QiPo				= 48,			//����
// 	EITE_LiPo				= 49,			//����
// 	EITE_ZhongShu			= 50,			//����
// 	EITE_JingPo				= 51,			//����
// 	EITE_YingPo				= 52,			//Ӣ��

};

enum EItemTypeReserved
{
	EITR_Null		= 0,
	EITR_Yaopin		= 1,	// ҩƷ
	EITR_Shipin		= 2,	// ʳƷ
	EITR_Xiandan	= 3,	// �ɵ�
	EITR_Longhun	= 4,	// ����
	EITR_Yinji		= 5,	// ӡ��
	EITR_Baoshi		= 6,	// ��ʯ
	EITR_Qizhen		= 7,	// ����
	EITR_Ranliao	= 8,	// Ⱦ��
	EITR_Siliao		= 9,	// ����
	EITR_Fuzhou		= 10,	// ����
	EITR_Kuangshi	= 11,	// ��ʯ
	EITR_Caoyao		= 12,	// ��ҩ
	EITR_ShiCai		= 13,	// ʳ��
	EITR_Buliao		= 14,	// ����
	EITR_Mucai		= 15,	// ľ��
	EITR_Longyuan	= 16,	// ����ԭ��
	EITR_Yinyuan	= 17,	// ӡ��ԭ��
	EITR_BigFaqi	= 18,	// ���ͷ���
	EITR_SmallFaqi	= 19,	// С�ͷ���
	EITR_VocationSuit = 20, // ְҵ��װ
	EITR_Spin		= 21,	// �ɾ���ʱװ
};

enum EItemQuality
{
	EIQ_Start		= -1,

	EIQ_Null		= -1,	// δ����
	EIQ_White		= 0,
	EIQ_Yellow		= 1,
	EIQ_Green		= 2,
	EIQ_Blue		= 3,
	EIQ_Orange		= 4,
	// Jason 2009-11-30 ������ɫƷ��
	EIQ_Purple		= 5,
	EIQ_Pink		= 6,

	EIQ_End			= 7,
};

const INT32 X_EQUIP_QUALITY_NUM = EIQ_End - EIQ_Start - 1;

enum EBindMode	// ������
{
	EBM_Null		= 0,	// ��

	EBM_Gain		= 1,	// ��ð�
	EBM_BindAfterUse	= 2,	// ʹ�ú��
	EBM_AccountBind	= 3,		// �˺Ű�
};

enum EBindStatus	// ��״̬
{
	EBS_Unknown		= 0,		// δ֪�ģ����̵��е���Ʒ
	EBS_Unbind		= 1,		// δ��
	EBS_Bind		= 2,		// ��
	EBS_BindAfterUse = 3,		// ʹ�ú��
	EBS_AccountBind	= 4,		// �˺Ű�
	EBS_AttachSoulBind	= 5,	// ����� ��װ�����󶨵ĸ���ʯ���ú󣬲���ԭ���ǰ󶨻��Ƿǰ󶨶���ɸ���󶨣�
};

//enum ETimeMode		// ��ʱ��ʽ
//{
//	ETM_Null		= 0,
//	TEM_Create		= 1, // ���ɼ�ʱ
//	TEM_Online		= 2, // ���ϼ�ʱ
//	TEM_Battle		= 3, // ս����ʱ
//	TEM_Quest		= 4, // ���񴥷���ʱ
//	TEM_Used		= 5, // ʹ�ú��ʱ
//	TEM_Get			= 6, // ��ü�ʱ
//};

enum ESexLimit		// ��Ʒ��ʹ�õĽ�ɫ�Ա�,Ҫ��AvatarAtt���Ա��Ӧ
{
	ESL_Null		= -1,	// �����ƣ�����Ů������
	ESL_Woman		= 0,	// Ů���ƣ���Ů����
	ESL_Man			= 1,	// �����ƣ����п���
};

enum EClassLimit
{
	ECL_Sword		= 0x1,	// ����
	ECL_JianXian	= 0x2,	// ����
	ECL_XieWu		= 0x4,	// а��
					
	ECL_Blade		= 0x1,	// ����
	ECL_BaDao		= 0x2,	// �Ե�
	ECL_GangRen		= 0x4,	// ���
					
	ECL_Wand		= 0x1,	// �ȿ�
	ECL_ZongZhang	= 0x2,	// ����
	ECL_DiMo		= 0x4,	// ��ī
					
	ECL_Bow			= 0x1,	// ����
	ECL_TianJian	= 0x2,	// ���
	ECL_MoLing		= 0x4,	// ħ��
					
	ECL_Valor		= 0x1,	// ��ʦ
	ECL_XuXing		= 0x2,	// ����
	ECL_GuiQi		= 0x4,	// ����
					
	ECL_Supple		= 0x1,	// ��ʦ
	ECL_XuangWang	= 0x2,	// ����
	ECL_MiZong		= 0x4,	// ����

	ECL_Apothecary	= 0x1,	// ҩʦ
	ECL_XianYi		= 0x2,	// ��ҽ
	ECL_DuYin		= 0x4,	// ����

	ECL_Musician	= 0x1,	// ��ʦ
	ECL_YueSheng	= 0x2,	// ��ʥ
	ECL_HuanYin		= 0x4,	// ����

	ECL_Stabber		= 0x1,	// �̿�
	ECL_YingWu		= 0x2,	// Ӱ��
	ECL_GuiWu		= 0x4,	// ����
};

enum EWuXing	// ��������
{
	EWX_Start		= -1,
	EWX_Null		= -1,

	EWX_Metal		= 0,
	EWX_Wood		= 1,
	EWX_Water		= 2,
	EWX_Fire		= 3,
	EWX_Soil		= 4,

	EWX_End			= 5,
};

// Jason 2010-10-24 Ԫ���˺�����
enum EElementInjury
{
	EEI_Null	= -1,
	EEI_START	= 0,
	EEI_Fire	= 0,	// ��Ԫ��
	EEI_Water	= 1,	// ˮԪ��
	EEI_Earth	= 2,	// ��Ԫ��
	EEI_Wind	= 3,	// ��Ԫ��
	EEI_Bright	= 4,	// ����Ԫ��
	EEI_Dark	= 5,	// �ڰ�Ԫ��
	EEI_End		= 6
};
// Ԫ�ؿ���
enum EElementResistance
{
	EER_Null	= -1,
	EER_START	= 0,
	EER_Fire	= 0,	// ��Ԫ�ؿ���
	EER_Water	= 1,	// ˮԪ�ؿ���
	EER_Earth	= 2,	// ��Ԫ�ؿ���
	EER_Wind	= 3,	// ��Ԫ�ؿ���
	EER_Bright	= 4,	// ����Ԫ�ؿ���
	EER_Dark	= 5,	// �ڰ�Ԫ�ؿ���
	EER_End		= 6
};

const INT32 X_WUXING_TYPE_NUM = EWX_End - EWX_Start - 1;


enum EStuffType		// ��������
{
	EST_Null		= 0,	//��
	EST_Stone		= 1,	//ʯ
	EST_Jade		= 2,	//��
	EST_Iron		= 3,	//��
	EST_Silver		= 4,	//��
	EST_Gold		= 5,	//��
	EST_ChunXinCao	= 6,	//������
	EST_XiaKuCao	= 7,	//�Ŀݲ�
	EST_QiuBaiCao	= 8,	//��ײ�
	EST_DongKuiCao	= 9,	//������
	EST_DongYangCao = 10,	//������
	EST_Flax		= 11,	//�鲼
	EST_Leather		= 12,	//Ƥ��
	EST_Cotton		= 13,	//�޲�
	EST_SilkOrgandy	= 14,	//��ɴ
	EST_Silk		= 15,	//˿��
	EST_Chop		= 16,	//���
	EST_Dew			= 17,	//¶ˮ
	EST_Grain		= 18,	//����
	EST_Bone		= 19,	//��ͷ
	EST_Potherb		= 20,	//Ұ��
	EST_IcePith		= 21,	// ����
	EST_FirePith	= 22,	// ��
	EST_WoodPith	= 23,	// ľ��
	EST_Consolidate = 24,	// ǿ��
	EST_Consolidate2 = 25,	// ǿ��2����ˮ��
	EST_Consolidate3 = 26,	// ǿ��3����ǿ��ˮ��
	EST_Consolidate4 = 27,	// ǿ��4ʥħ֮Ѫ
	EST_ChongNeng	= 28,	// ��ͨ���ܵ���
	EST_PianXiang	= 29,	// ʥ��ƫ�����
	EST_LingNeng	= 30,	// ����ǿ������
};

enum EItemCreateMode	// ��Ʒ&װ��������ʽ
{
	EICM_Null			= 0,	// û�м�¼
	EICM_GM				= 1,	// GM��������
	EICM_Loot			= 2,	// �������
	EICM_Quest			= 3,	// ������
	EICM_Product		= 4,	// �������
	EICM_ShopNPC		= 5,	// �̵�NPC
	EICM_AccQuest		= 6,	// ������
	EICM_Mall			= 7,	// �̳�
	EICM_Gift			= 8,	// ��Ʒ��
	EICM_ActTreasure	= 9,	// ���������䱦
	EICM_GMTool			= 10,	// GM��������
	EICM_PetSkill		= 11,	// ���＼��
	EICM_VipNetBar		= 12,	// ��������
	EICM_ReGet			= 13,	// ���߻ָ�
	EICM_Activity		= 14,	// �����
	EICM_LessingLoong	= 15,	// �����͸�
	EICM_GoldStone		= 16,	// ���ʯ
	EICM_GemRemoval		= 17,	// ��ʯ���
	EICM_ServerMerge	= 18,	// �Ϸ�����
	EICM_CityProduce	= 19,	// ���в���
	EICM_SpiritReward	= 20,	// ����ϵͳ����
	EICM_StrengthReward	= 21,	// ����ϵͳ����-ʵ��
	EICM_Spin			= 22,	// ����
	EICM_Xihun			= 23,	// ���꣬��������ʯ
	EICM_GuildDailyReward	= 24,	// ����ÿ�����߽�����ȡ
	EICM_ExpPilularBind	= 25,	// ���������ӡ-ˮ����Լ��
	EICM_GodBid	= 26,	// ��֮����
	EICM_FairyBirth	= 27,	// ������ֳ
	EICM_EggBroker	= 28,	// �ҽ�
	EICM_PlayerBack = 29,	// ��һع齱��
	EICM_AdvancedCompose = 30,	// �߼��ϳ�
	EICM_FamilyQuest	= 31,	//��������
	EICM_KeyCodeReward	= 32, // ����Key����ȡ����
	EICM_Zhanghao	= 33, // �˺Żع�
	EICM_Tiger	= 34, // �ϻ���
	EICM_NewGiftBox = 35, //�±���
	EICM_FairyFusion = 36,	// �����ں� [3/22/2012 zhangzhihua]
	EICM_NewKeyCode = 37,	//��Key��
	EICM_HolySoulEquipInlay=38,//Ԫ��װ����Ƕ��ʧ�ܺ�ɾ�������ɵ�һ������Ӱ��ʯ
	EICM_HolySoulEquipRemove=39,//Ԫ��װ��ժ��
	EICM_EquipXiStar = 40,		//���Ǵ�
	EICM_SaveTelNum = 41,		//��ұ����ֻ���
	EICM_WarReward		= 42,	//�ػ���ͼ����
	EICM_YuanBaoDaiBi	= 43,	//Ԫ�����ҹ���
	EICM_RecallPet		=44,
	EICM_ReceiveYuanBaoDaiBi = 45,	//ͨ��NPC��ȡ��

	EICM_End,
};

enum EEnhanceEquipPos	// ��ǿ����װ������
{
	EEEP_Hand		= 0x00000001,	// ����
	EEEP_Head		= 0x00000002,	// ͷ��
	EEEP_Face			= 0x00000004,	// �沿
	EEEP_Body		= 0x00000008,	// ����
	EEEP_Legs			= 0x00000010,	// ����
	EEEP_Back			= 0x00000020,	// ����
	EEEP_Wrist		= 0x00000040,	// ��
	EEEP_Feet			= 0x00000080,	// �㲿
	EEEP_Finger		= 0x00000100,	// ��ָ
	EEEP_Waist		= 0x00000200,	// ����
	EEEP_Neck		= 0x00000400,	// ����
	EEEP_HunJing	= 0x00000800,	// �꾧
};

enum EEquipMdlType     // װ����ģ������
{
	EEMT_NULL	= 0,
	EEMT_FSKIN  = 1,   // ��Ƥ
	EEMT_FAK    = 2,   // �ؼ�֡
	EEMT_FSM    = 3,   // ��̬ģ��
};

enum EEquipAttachPos    // װ���ɱ��β��ֵĹ���λ��
{
	EEAP_NULL   = 0,
    EEAP_haa	= 1,	// ͷ
	EEAP_chm	= 2,	// ǰ��
	EEAP_baa	= 3,	// ��
	EEAP_bab	= 4,	// ��
	EEAP_shb	= 5,	// ���
	EEAP_sha	= 6,	// �Ҽ�
	EEAP_shl	= 7,	// ���ϱ�1
	EEAP_shx	= 8,	// ���ϱ�2
	EEAP_shr	= 9,	// ���ϱ�1
	EEAP_shu	= 10,	// ���ϱ�2
	EEAP_chg	= 11,	// ���1
	EEAP_chh	= 12,	// ���2
	EEAP_chi	= 13,	// ���3
	EEAP_chd	= 14,	// �Ұ�1
	EEAP_che	= 15,	// �Ұ�2
	EEAP_chf	= 16,	// �Ұ�3
	EEAP_chj	= 17,	// ���1
	EEAP_chk	= 18,	// ���2
	EEAP_chl	= 19,	// ���3
	EEAP_chp	= 20,	// ��
	EEAP_lla	= 21,	// ��С��
	EEAP_lra	= 22,	// ��С��
	EEAP_fla	= 23,	// ���
	EEAP_fra	= 24,	// �ҽ�
	EEAP_gla	= 25,	// ��С��
	EEAP_gra	= 26,	// ��С��
	EEAP_bal	= 27,	// ��
	/// hdf [20100919] add two ���
	EEAP_bax	= 28,
	EEAP_bay	= 29,
	EEAP_Num	= 30	///28
};

enum EEquipSpecAtt		// װ��B������
{
	EESA_Null				= -1,
	EESA_Start				= 0,

	EESA_LevelLim_Simple	= 0,	// ����		��װ���ȼ�����-5����Ϳɼ�����0
	EESA_LevelLim_Fine		= 1,	// ����		��װ���ȼ�����-10����Ϳɼ�����0
	EESA_LevelLim_None		= 2,	// �޼���	��װ���޵ȼ�����

	EESA_AttALim_Simple		= 3,	// ���		��װ���������Ƽ���10%��ȡ��
	EESA_AttALim_Comfort	= 4,	// ����		��װ���������Ƽ���25%��ȡ��
	EESA_AttALim_Light		= 5,	// ��ӯ		��װ���������Ƽ���50%��ȡ��

	EESA_Potential_YinFeng	= 6,	// ����		��װ���ĳ�ʼǱ��ֵ+200
	EESA_Potential_YinHuang	= 7,	// ����		��װ���ĳ�ʼǱ��ֵ+400
	EESA_Potential_FeiFeng	= 8,	// �ɷ�		��װ���ĳ�ʼǱ��ֵ+800
	EESA_Potential_MingHuang= 9,	// ����		��װ���ĳ�ʼǱ��ֵ+1200
	EESA_Potential_WoLong	= 10,	// ����		װ���ĳ�ʼǱ��ֵ���5%
	EESA_Potential_CangLong	= 11,	// ����		װ���ĳ�ʼǱ��ֵ���10%
	EESA_Potential_FuLong	= 12,	// ����		װ���ĳ�ʼǱ��ֵ���20%
	EESA_Potential_ShengLong= 13,	// ����		װ���ĳ�ʼǱ��ֵ���30%

	EESA_EquipSpec_RelEnd	= 13,	// 

	EESA_Guard_Posy			= 14,	// �����ػ�	װ���ڽ�������ǿ��ʱ����ʧ��
	EESA_Guard_Brand		= 15,	// ��ӡ�ػ�	װ���ڽ�����ӡǿ��ʱ����ʧ��
	EESA_Guard_Engrave		= 16,	// �Կ��ػ�	װ���ڽ����Կ�ǿ��ʱ����ʧ��
	EESA_Guard_LongFu		= 17,	// �����ػ�	װ���ڽ�������ǿ��ʱ����ʧ��
	EESA_Guard_Quench		= 18,	// ����ػ�	װ���ڽ��д��ǿ��ʱ����ʧ��
	EESA_Guard_Cut			= 19,	// �����ػ�	װ���ڽ��п���ǿ��ʱ����ʧ��
	EESA_Guard_AllProduce	= 20,	// ����		װ���ڽ����κ�ǿ��ʱ����ʧ��

	EESA_Guard_Talisman		= 21,	// ��а		����װ��ǿ��ʱ�ɹ�������[����5%]
	EESA_Guard_Fortune		= 22,	// ����		����װ��ǿ��ʱ����������[����5%]

	EESA_Guard_DeadPunish	= 23,	// �����ػ�	��װ������������ʱ����

	EESA_End				= 23,
};

const INT X_EQUIP_SPECATT_NUM	= EESA_End - EESA_Start + 1;

enum ECorlor		// ��ɫ��ͼö��
{
	EC_Null		= -1,

	EC_Start	= 0,
	EC_Val0		= 0,
	EC_Val1		= 1,
	EC_Val2		= 2,
	EC_Val3		= 3,
	EC_Val4		= 4,
	EC_Val5		= 5,
	EC_Val6		= 6,
	EC_Val7		= 7,
	EC_Val8		= 8,
	EC_Val9		= 9,
	EC_Val10	= 10,
	EC_Val11	= 11,
	EC_End		= 11,
};

enum EPickupType
{
	EPT_Money = 0,			// Ǯ
	EPT_Equip = 1,			// װ��
	EPT_Medicine = 2,		// ҩˮ
	EPT_StrengthenStuff = 3,	// ǿ������
	EPT_Gem = 4,				// ��ʯʥӡ
	EPT_Quest = 5,			// ����
	EPT_Sprite = 6,			// �������
	EPT_Others = 7,			// ������Ʒ
};

// ��������
enum EDiscardEvent
{
	EDE_Null = -1,
	EDE_DiscardGround	= 0, // ������
	EDE_DiscardDestory	= 1, // ��������
	EDE_DiscardNot		= 2, // ���ܶ���
};

const INT X_COLOR_NUM	= EC_End - EC_Start + 1;

// Jason 2010-4-13 v2.0.0
// ��������
enum ENUM_FabaoType 
{
	EFB_NULL	= -1,
	EFB_Attack	= 0, // �����ͷ���
	EFB_Defense	= 1, // �����ͷ���
	EFB_Assit	= 2, // �����ͷ���
	EFB_END
};

// ս����
enum EAutoFightSpecFuncVal2
{
	EAFSFV2_Normal = 0,	// ��ͨ�汾
	EAFSFV2_OMG = 1,	// ̨��OMG�汾
};

// װ���ɳ���
enum EEquipGrowStar
{
	EEGS_Null		= 0,
	EEGS_Gray	= 1,
	EEGS_Green	= 2,
	EEGS_Blue	= 3,
	EEGS_Purple	= 4,

	EEGS_End = 5,
};
#define IsGrowStarValid(star) (star > EEGS_Null && star < EEGS_End)

struct tagFabao;

//-----------------------------------------------------------------------------
// �ṹ����
//-----------------------------------------------------------------------------
struct tagRoleAttEffect
{
	ERoleAttribute	eRoleAtt;
	INT32			nValue;

	tagRoleAttEffect() { ZeroMemory(this, sizeof(*this)); eRoleAtt = ERA_Null; }
};

struct tagHolyAttEffect
{
	EHolyAttType	eHolyAtt;
	INT32			nValue;

	tagHolyAttEffect() { ZeroMemory(this, sizeof(*this)); eHolyAtt = ESAT_NULL; }
};

struct tagEquipQltyPct
{
	DWORD				dwTypeID;
	INT32				nEquipQltyPct[X_EQUIP_QUALITY_NUM];	// Ʒ�����ɼ���
};

struct tagCDTime
{
	DWORD		dwTypeID;
	DWORD		dwTime;		// ��ȴʱ��(��λ������)
};

/** struct	tagItemDisplayInfo
brief	��Ʒ��ʾ�ṹ
remarks	��Ҫ���ڴ洢��Ʒ�����ơ����͡�������ģ�͵ȵ�	
*/
struct tagItemDisplayInfo
{
	TCHAR				szType[X_SHORT_NAME];				// ��ʾ����
	TCHAR				szName[X_SHORT_NAME];				// ��Ʒ����
	TCHAR				szDesc[X_HUGE_NAME];				// ��Ʒ����
	TCHAR				szDescEx[X_HUGE_NAME];				// ��Ʒ��������
	TCHAR				szMaleIcon[X_SHORT_NAME];			// ��Ʒͼ���а�
	TCHAR				szFemaleIcon[X_SHORT_NAME];			// ��Ʒͼ��Ů��
	TCHAR				szActionMod[X_SHORT_NAME];			// ��Ʒʹ�ö������
	TCHAR				szEffectMod[X_SHORT_NAME];			// ��Ʒʹ����Ч���
	TCHAR				szSourceEffect[X_SHORT_NAME];		// ��Ʒʹ�÷�������Ч���
	TCHAR				szDestEffect[X_SHORT_NAME];			// ��Ʒʹ��Ŀ����Ч���
	TCHAR				szPrepareMod[X_SHORT_NAME];			// ��Ʒʹ�����ֶ������
	TCHAR				szMaleRightMod[X_SHORT_NAME];		// ��ģ�ͱ��1
	TCHAR				szFemaleRightMod[X_SHORT_NAME];		// Ůģ�ͱ��1
	TCHAR				szMaleLeftMod[X_SHORT_NAME];		// ��ģ�ͱ��2
	TCHAR				szFemaleLeftMod[X_SHORT_NAME];		// Ůģ�ͱ��2
	TCHAR				szDropMod[X_SHORT_NAME];			// ����ģ�ͱ��
	TCHAR				szPertainMod[X_SHORT_NAME];			// ����ģ�ͱ��
	TCHAR				szBladeFlareSFX[X_SHORT_NAME];		// ������Ч���
	TCHAR				szBladeGlowSFX[X_SHORT_NAME];		// ������Ч���
	TCHAR				szAttachSFX[X_SHORT_NAME];			// ���������Ч���
	TCHAR				szBladeBornSFX[X_SHORT_NAME];		// �����Դ���Ч���
	EWeaponPos			ePertainModPos;						// ����ģ�͹���λ��
	EEquipMdlType       eModelType;							// ģ������
	EEquipAttachPos     eAttachModelPos[MAX_EQUIP_ATTACH_NUM];  // ���ر���ģ�͵�λ��
	BOOL				bActionBlend;						// �Ƿ�������ֿ�
	
	tagItemDisplayInfo(){ZeroMemory(this, sizeof(*this));}
};

struct tagItemProto
{
	DWORD				dwTypeID;		// ������Ʒ������(ͬ����Ʒ��ͬ)

	EItemType			eType;			// ��������
	EItemTypeEx			eTypeEx;		// ��չ����
	EItemTypeReserved	eTypeReserved;	// ��������

	EStuffType			eStuffType;		// �������ͣ��ϳ���

 	DWORD				dwQuestID;		// ������
	INT32				nBasePrice;		// �����۸�

	INT32				nMaxUseTimes;	// ���ʹ�ô���
	INT16				n16MaxLapNum;	// ����������
	INT16				n16MaxHoldNum;	// ���ӵ������

	DWORD				dwTimeLimit;	// ��ʱ����Ʒ(��)

	INT16				n16Enmity;		// ���

	BYTE				byMinUseLevel;	// �ȼ�����
	BYTE				byMaxUseLevel;	// �ȼ�����
	ESexLimit			eSexLimit;		// �Ա�����
	DWORD				dwVocationLimit;// ְҵ����
	// Jason 2010-5-18 v2.1.2
	BOOL				bSoarLimit;		// ��������

	EReputationType		eClanRepute;	// ������������ֵ
	INT32				nClanReputeVal;	// ��������ֵ
	EReputationType		eOtherClanRepute;	// ������������ֵ
	INT32				nOtherClanReputeVal;// ��������ֵ

	EItemSpecFunc		eSpecFunc;		// ��Ʒ���⹦������
	INT32				nSpecFuncVal1;
	INT32				nSpecFuncVal2;

	ESkillOPType		eOPType;							// ��������
	FLOAT				fOPDist;							// ���þ��루���絥λ��
	FLOAT				fOPRadius;							// ���÷�Χ�����絥λ��
	INT32				nPrepareTime;						// ����ʱ�䣨���룩
	DWORD				dwCooldownTime;						// ������ȴʱ��(��λ��ms)
	INT32				nInterruptSkillOrdRate;				// ��ͨ���ܴ�ϼ���[0,10000]
	DWORD				dwSpecBuffLimitID;					// ����BUFF����ID
	DWORD				dwStateLimit;						// ����״̬��־λ����
	DWORD				dwTargetLimit;						// ��������
	BOOL				bMoveable;							// �Ƿ�����ƶ�ʹ��			
	bool				bInterruptMove;						// �Ƿ��ƶ����

	BYTE				byLevel;			// ��Ʒ�ȼ�
	BYTE				byQuality;			// ��ƷƷ��

	BYTE				byBindType;			// ������

	DWORD				dwTriggerID0;		// ��������
	DWORD				dwBuffID0;			// Buff
	
	DWORD				dwTriggerID1;		// ������ID1
	DWORD				dwBuffID1;			// ״̬ID1

	DWORD				dwTransTriggerID;	// ���δ�������ID
	DWORD				dwTransID;			// ���κ�ID

	bool				bNeedBroadcast;		// �Ƿ�㲥(0�����㲥��1���㲥)
	bool				bCanSell;			// �Ƿ񲻿ɳ���(0���ɳ��ۣ�1�����ɳ���)

	bool				bNeedLog;			// �Ƿ���Ҫ��log(����������Ӫͨ�����ݱ�item_needlog�༭)
	BYTE				byLogMinQlty;		// ��¼log��Ʒ�����Ʒ��

	bool				bDeadLoot;			// ��������
	EDiscardEvent		DiscardEvent;		// "DiscardEvent"	0\1\2
	EPickupType			ePickType;			// ʰȡ���ͣ��Զ�ʰȡɸѡʹ�ã�

	bool				bCanMove2Ware;		// �Ƿ�ɴ���ֿ� 0-���Դ棬1-�����Դ�

	tagItemDisplayInfo*	pDisplayInfo;		// ��Ʒ��ʾ�ṹ���ͻ�����
	
	tagItemProto(){ZeroMemory(this,sizeof(*this));}
	tagItemProto(DWORD dwID){ZeroMemory(this,sizeof(*this)); dwTypeID = dwID;}
	INT16			GetMaxLapNum() const
	{
		if (n16MaxLapNum <= 0)
		{
			return 1;
		}

		return n16MaxLapNum;
	}
};

struct tagEquipProto: public tagItemProto
{
	DWORD				dwSuitID[MAX_PEREQUIP_SUIT_NUM];		// ��װ���
	BYTE				bySuitMinQlty[MAX_PEREQUIP_SUIT_NUM];	// ���Ʒ��Ҫ��
	bool				bCanDye;								// ʱװ�Ƿ��Ⱦɫ
	bool				bCanGrow;	// װ���Ƿ���гɳ�����

	INT16				n16AttALimit[X_ERA_ATTA_NUM];			// ��������
	
	DWORD				dwVocationLimitWear;					// ����ְҵ����
	DWORD				dwCondenseLimit;						// �������

	EEquipPos			eEquipPos;		// װ��λ��
	EWeaponPos			eWeaponPos;		// ����λ��

	INT16				n16WuHun;		// �ڹ��˺�������
	INT16				n16MinDmg;		// ������С�˺�
	INT16				n16MaxDmg;		// ��������˺�
	INT16				n16Armor;		// ���߻���

	INT32				nMaxPotVal;		// װ�����Ǳ��ֵ
	INT32				nPotVal;		// װ��Ǳ��ֵ
	// Jason 2009-12-1 װ��Ǳ��ֵ��������
	INT32				nPotIncTimes;	// Ǳ��ֵ����ʣ�������ע�����ֵ�ǵݼ��ġ�

	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_BASE_EFFECT];// �ӹ�ǰ��Ӱ�������(��Ʒ���޹�)
	BYTE				byLongfuAtt[X_WUXING_TYPE_NUM];		// ��������,������

	//BYTE				byClassLimit[X_ClASS_TYPE_NUM];		// ְҵ����(ָ��ְҵ����)//??

	DWORD				dwTriggerID2;		// ������ID1
	DWORD				dwBuffID2;			// ״̬ID1
	
	DWORD				dwColor;			// ��ɫ

	INT16				n16Newness;			// ո�¶� -- ������

	bool				bIdentifyProduct;	// ��������״̬
	bool				bIdentifyLoot;		// �������״̬

	INT				nInitConsolidateLevel;		// ��ʼװ��ǿ���ȼ�
	INT				nMaxConsolidateLevel;	// װ�����ǿ���ȼ�����

	// ������ʼ�ȼ���Ĭ��-1
	INT				nBornLevelOfGodStrengthen;
	bool			bSuckLimit;				// װ���Ƿ���Ա����꣬1��ʾ�������꣬0��ʾ��������
	bool			bOnlyScriptStrengthen;		// ֻ�нű�����ǿ��
	bool			bMapLimit;				//�Ƿ��ͼ����

	tagEquipProto(){ZeroMemory(this,sizeof(*this));}
	tagEquipProto(DWORD dwID){ZeroMemory(this, sizeof(*this)); dwTypeID = dwID;}
};

struct tagSoulCrystalLvlLine
{
	BYTE byLvl;
	DWORD dwAttSoul;
	INT64 n64Money;
};

struct tagSoulCrystalAttData
{
	INT nID;
	BYTE byLevel;
	ERoleAttribute eAttType;
	DWORD dwAttValue;
};

struct tagEquipGrowProto
{
	DWORD dwID;
	DWORD dwExp[EQUIP_GROW_MAX_LEVEL];

	ERoleAttribute eAtt[EQUIP_GROW_ADD_ATT_MAX_NUM]; // װ���ĳɳ�����
	BYTE byAttOpenLevel[EQUIP_GROW_ADD_ATT_MAX_NUM];	// �ɳ����Կ���������(װ���ɳ�������ʱ����)
	DWORD dwAttValue[EQUIP_GROW_ADD_ATT_MAX_NUM][EQUIP_GROW_MAX_LEVEL]; // �ɳ������ڸ���ʱ�����ֵ
};

struct tagEquipGrowName : tagEquipGrowProto
{
	BYTE byStrength;	//����ǿ��
};

#define MEquipMaxUseTimes(pEquipProto)	((pEquipProto)->n16Newness * ABRASION2USETIMES)

struct tagGemProto		// ǿ����Ʒԭ��
{
	DWORD				dwTypeID;		// ��Ʒԭ�ͱ��

	INT16				n16PotValNeed;	// ��Ҫ���ĵ�װ��Ǳ��ֵ
	INT16				n16SuccessPct;	// �ɹ���

	EWuXing				eWuXing;		// ���꣬����������(-1��ʾ����Ʒ��������)
	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_ENHANCE_EFFECT];	// װ��ǿ��Ӱ��

	INT32				nEnhanceFlg;	// ��λ��¼ -- EEnhanceEquipPos

	tagGemProto() { ZeroMemory(this, sizeof(*this)); }
};

struct tagItem
{
	INT64			n64Serial;		// ��ƷΨһ���к�(��ͬ�ѵ���Ʒ��ͬ)
	DWORD			dwTypeID;		// ������Ʒ������(ͬ����Ʒ��ͬ)

	INT16			n16Num;			// ��Ʒ����
	bool			bLock	: 1;	// ������־
	bool			bUsing	: 1;	// �Ƿ��ڶԸ���Ʒ������������
	BYTE			byBind;			// ��״̬
	INT32			nUseTimes;		// ��Ʒ�ϼ���ʹ�ô���

	DWORD			dwAccountID;	// ����˺�ID, �ٱ����ͽ�ɫ�ֿ���(ͬһ�ʺ��½�ɫ����)
	DWORD			dwOwnerID;		// ӵ����, GT_INVALID��ʾû��ӵ����

	EItemConType	eConType;		// ��Ʒ������������
	INT16			n16Index;		// ��Ʒ�������е�λ��
	INT16			n16Dummy;		// �ڴ������

	EItemCreateMode	eCreateMode;	// ��Ʒ&װ�����ɷ�ʽ
	DWORD			dwCreateID;		// �ⲿ���ɸ���Ʒ��ID���磺GMID��QuestID��, �����ϲ���ʾ
	
	DWORD			dwCreatorID;	// ��Ʒ�����ߣ�GT_INVALID��ʾϵͳ����, ��������ʾ
	tagDWORDTime	dwCreateTime;	// ������Ʒ��ʱ��

	DWORD			dwNameID;		// �䱦����ID

	mutable tagDWORDTime	dw1stGainTime;	// ��һ����һ��ʱ��(��Ϊս���������������Ϊmutable)

	EUpdateDBStatus	eStatus;		// ��¼��Ʒ�Ƿ���Ҫ�������ݿ����
	
	DWORD			dwUnlockTime;	// �����Ľ���ʱ��

	DWORD			dwSpecVal1;	// ��Ʒ���⹦���ֶ�ֵ1����̬���ԣ����ض���������
	DWORD			dwSpecVal2;	// ��Ʒ���⹦���ֶ�ֵ2

	union
	{
		const tagItemProto	*pProtoType;	// ָ����Ʒ��������
		const tagEquipProto	*pEquipProto;	// ִ��װ����������
	};

	const ItemScript		*pScript;		// �ű�ָ��
	
	tagItem() { ZeroMemory(this, sizeof(*this)); }
	VOID SetOwner(DWORD dwNewOwnerID, DWORD dwNewAccountID) { dwOwnerID = dwNewOwnerID; dwAccountID = dwNewAccountID; }
	VOID SetPos(EItemConType eNewCon, INT16 n16NewIndex) { if(TRUE == SetConType(eNewCon)) n16Index = n16NewIndex; }
	VOID SetIndex(INT16 n16NewIndex) { n16Index = n16NewIndex; }
	VOID SetUsing(bool bUsingState) { bUsing = bUsingState; }
	VOID SetUpdate(EUpdateDBStatus eNewUpdateStatus) { eStatus = eNewUpdateStatus; }
	INT64 GetKey() const { return n64Serial; }
	VOID SetNum(INT16 n16Val)
	{
		if (!P_VALID(pProtoType))
		{
			return;
		}
		if (n16Val > pProtoType->n16MaxLapNum)
		{
			n16Val = pProtoType->n16MaxLapNum;
		}
		if (n16Val < 0)
		{
			n16Val = 0;
		}
		n16Num = n16Val;
	}
	VOID AddNum(INT16 n16Val)
	{
		n16Num += n16Val;
		if (!P_VALID(pProtoType))
		{
			return;
		}
		if (n16Num > pProtoType->n16MaxLapNum)
		{
			n16Num = pProtoType->n16MaxLapNum;
		}
		if (n16Num < 0)
		{
			n16Num = 0;
		}
	}
	INT16 GetNum() const
	{
		if (n16Num < 0 || !P_VALID(pProtoType))
		{
			return 0;
		}
		return (n16Num > pProtoType->n16MaxLapNum) ? pProtoType->n16MaxLapNum : n16Num;
	}
	BOOL CanOverlap() const { return (P_VALID(pProtoType) ? pProtoType->n16MaxLapNum > 1 : FALSE); }
	VOID IncUseTimes(INT32 nUse) { nUseTimes += nUse; SetUpdate(EUDBS_Update); }
	BOOL SetConType(EItemConType eNewCon)
	{
		// ���ܽ���װ����Ʒװ�������� [4/13/2012 zhangzhihua]
		if (EICT_Equip == eNewCon && !MIsEquipment(dwTypeID))
		{
			return FALSE;
		}

		eConType = eNewCon;
		return TRUE;
	}

	// �õ�ĳ��������Ӧ��BuffID
	DWORD GetBuffID(INT nIndex)
	{
		switch(nIndex)
		{
		case 0:
			return pProtoType->dwBuffID0;
		case 1:
			return pProtoType->dwBuffID1;
		case 2:
			return pProtoType->dwTransID;
		default:
			return GT_INVALID;
		}
	}

	// �õ�ĳ��������Ӧ��TriggerID
	DWORD GetTriggerID(INT nIndex)
	{
		switch(nIndex)
		{
		case 0:
			return pProtoType->dwTriggerID0;
		case 1:
			return pProtoType->dwTriggerID1;
		case 2:
			return pProtoType->dwTransTriggerID;
		default:
			return GT_INVALID;
		}
	}

	VOID Bind()
	{
		ASSERT(P_VALID(pProtoType));
		switch(pProtoType->byBindType)
		{
		case EBM_Null:
			byBind = EBS_Unbind;
			break;
		case EBM_Gain:
			byBind = EBS_Bind;
			break;
		case EBM_AccountBind:
			byBind = EBS_AccountBind;
			break;
		//case EBM_BindAfterUse:
		//	byBind = EBS_BindAfterUse;
		//	break;
		}
	}
	// ע��OMGս����ר��
	VOID BindOMGAutoFightItem()
	{
		byBind = EBS_Bind;
	}
	// ʹ�ú��
	VOID BindAfterUse()
	{
		if(EBM_BindAfterUse == pProtoType->byBindType)
		{
			byBind = EBS_Bind;
			eStatus = EUDBS_Update;
		}
	}

	BOOL IsBind() const { return EBS_Bind == byBind || EBS_AttachSoulBind == byBind; }

	BOOL IsAccountBind() const { return EBS_AccountBind == byBind; }

	BOOL IsAttachSoulBind() const { return EBS_AttachSoulBind == byBind; }

	BOOL IsLevelLimit(BYTE byRoleLevel) const
	{
		return pProtoType->byMinUseLevel > byRoleLevel || pProtoType->byMaxUseLevel < byRoleLevel;
	}

	BOOL IsTimeLimit() const { return pProtoType->dwTimeLimit != GT_INVALID; }

	EBindMode GetBindMode() const 
	{
		if(P_VALID(pProtoType))
		{
			return (EBindMode)pProtoType->byBindType;
		}
		return EBM_Null;
	}

};

struct tagEquipSpec
{
	// ����ǰ��ǿ���ȼ�����
	enum{ ECL_BefConMaxLevel = 20,ECL_AftConMaxLevel = 30};
	/******װ������ʱȷ��******/
	INT16				n16WuHun;		// �ڹ��˺�������
	INT16				n16MinDmg;		// ������С�˺�
	INT16				n16MaxDmg;		// ��������˺�
	INT16				n16Armor;		// ���߻���

	INT32				nPotVal;		// װ��Ǳ��ֵ(ԭǱ��ֵ+��������ֵ)
	INT32				nPotValUsed;	// �Ѿ����ĵ�Ǳ��ֵ
	// Jason 2009-12-1 װ��Ǳ��ֵ��������
	INT32				nPotIncTimes;	// Ǳ��ֵ����ʣ�������ע�����ֵ�ǵݼ��ġ�
	// Jason 2009-12-2 ������ɫװ����������
	DWORD				dwPurpleQltyIdfPct;

	// һ������
	INT32				nRoleAttEffect[X_ERA_ATTA_NUM];
	// �������ԣ��������ԣ���ԭ�ͱ�����������������
	//tagRoleAttEffect	BaseEffect[MAX_ROLEATT_BASE_EFFECT];//
	// �������ԣ����߻�Э�̺󣬰�˳��Ӱ���ɫ���ԣ�����ֻ��Ҫһ���������Եĸ����Ϳ����ˣ������������������Լ�ֵ����ԭ�ͱ������ȡ��
	INT32				nRoleL2AttCount;
	// ǿ���ȼ������˼�ԭ��
	BYTE				byConsolidateLevel;

	// ��,����������(GT_INVALID��ʾû��)
	DWORD				dwLongHunInnerID;
	DWORD				dwLongHunOuterID;

	// ��������
	BYTE				bySpecAtt;

	// ʹ�õȼ�����(�����ⲿ��������)
	BYTE				byMinUseLevel;
	BYTE				byMaxUseLevel;

	// Ʒ��
	BYTE				byQuality;

	/******ʱװ����ʱȷ��******/
	INT16				n16Appearance;	// ����
	BYTE				byRein;			// ͳ��
	BYTE				bySavvy;		// ����
	BYTE				byFortune;		// ��Ե
	INT8				n8ColorID;		// ��ɫ
	INT8				n8Dummy[2];


	/******Ԫ��װ�����ǵȼ�******/
	BYTE						byStarEnhanceLv;
	//Ԫ��װ����ǰ����
	BYTE				byHolyHoleNum;

	/******װ��ǿ��ʱ�޸�******/
	bool				bCanCut;		// �Ƿ�ɿ���
	BYTE				byFlareVal;		// ����

	// ԭʼһ������(�����Զ��ӵ���ֶ��ӵ㲿��)����
	INT16				n16AttALimMod;
	INT16				n16AttALimModPct;	/*10000Ϊ����1*/

	// ���������ģ��Կ̣���Ƕ����ӡ(�˺������������)
	BYTE				byLongfuLevel;
	BYTE				byPosyTimes;
	BYTE				byEngraveTimes;
	BYTE				byHoleNum;
	BYTE				byBrandLevel;

	BYTE				byLongfuAtt[X_WUXING_TYPE_NUM];
	tagRoleAttEffect	PosyEffect[MAX_ROLEATT_POSY_EFFECT];
	INT32				nEngraveAtt[MAX_ROLEATT_ENGRAVE_EFFECT];
	DWORD				dwHoleGemID[MAX_EQUIPHOLE_NUM];
	INT32				nBrandAtt[X_ERA_BRAND_NUM];

	/******����װ�� -- Ʒ������������******/
	INT16				n16QltyModPct;		// һ��������(-10000,10000)
	INT16				n16QltyModPctEx;	// ����������[0,10000)

	/******����װ�� -- Ǳ��ֵӰ��ϵ��******/
	INT16				n16PotValModPct;

	BYTE				bySigned;				// �Ƿ�����,1Ϊ������ 2Ϊ������
	DWORD				dwSignatruePlayerID;	// �����ߵ�ID
	TCHAR				szSignature[11];		// ��������
	// Jason 2010-5-18 v2.1.2
	BOOL				bSoaringFlag;
	// ��������������
	INT					nCastingTimes;							// ��������
	//ERoleAttribute		eCastingRoleAtt[MAX_CASTING_PARAM_NUM]; // �����ɹ��󣬴�װӰ������
	//INT					nCastingAttValue[MAX_CASTING_PARAM_NUM];// ����ֵ

	// Jason 2010-6-18 v1.0.0ǿ������Ӱ�죬ǿ����id
	//INT					nConsolidateEffectID;

	BYTE				 byLastGodStrengthenLevel;		// �ϴ������ȼ�
	BYTE				 byGodStrengthenLevel;			// ������ǰ�ȼ�
	DWORD				 dwCurGodStrengthenExp;			// ��ǰ�ȼ��ѻ�õ���������
	tagDWORDTime		 dwGodStrengthenTime;			// �ϴ�����ʱ��	 
	// �����ֵ��
	tagRoleAttEffect	ConsolidateEffect[MAX_FCONSOLIDATE_NUM];

	// װ���ɳ��������
	DWORD dwGrowTypeID;	// �ɳ�����ID
	DWORD dwGrowValue; // ��ǰ�ɳ�ֵ
	BYTE byGrowLevel; // ��ǰ�ɳ��ȼ���0-11
	INT nTotalGrowTimes; // �ܹ��ɳ����Ĵ���
	INT nTotalIMUseTimes; // �ܹ�ʹ�ù�IM���ߵĴ���
	EEquipGrowStar	eGrowStar[EQUIP_GROW_MAX_LEVEL-1]; // �ɳ����ǵ���ɫ
	INT nGrowAttValue[EQUIP_GROW_ADD_ATT_MAX_NUM]; // �ɳ����Ե�ֵ

	// ��ʯ����
	BYTE byDiamondLevel;	//��ʯ����0-10
	
	/******����******/
	tagEquipSpec() { ZeroMemory(this, sizeof(*this)); }
};

#define MGetEquipGrowRatio(star, Output)	do{ \
	switch(star) \
	{ \
		case EEGS_Gray: \
			Output = 0.5; \
			break; \
		case EEGS_Green: \
			Output = 0.8; \
			break; \
		case EEGS_Blue: \
			Output = 1.0; \
			break; \
		case EEGS_Purple: \
			Output = 1.5; \
			break; \
		default: \
			Output = 0; \
	} \
}while(0)

enum EEquipSignleSpecType
{
	EESSType_Null			= 0, //��
	EESSType_StarEnhance	= 1, //��������
	EESSType_HolyEquipHole	= 2, //�������
	EESSType_HolyEquipInlay = 3, //��Ƕ����
};

struct tagEquipSingleSpec
{
	INT64		  		 n64Serial;
	EEquipSignleSpecType eEquipSingleSpecType;
	DWORD				 dwVal;
};

struct tagEquip: public tagItem
{
	tagEquipSpec	equipSpec;

	VOID SetFlareVal(BYTE byFlareVal)
	{
		equipSpec.byFlareVal = byFlareVal;
	}

	VOID ChangeFlareVal(BYTE byFlareVal)
	{
		equipSpec.byFlareVal += byFlareVal;
		equipSpec.byFlareVal = (equipSpec.byFlareVal > 100) ? 100 : equipSpec.byFlareVal;
		equipSpec.byFlareVal = (equipSpec.byFlareVal < 0 ) ? 0 : equipSpec.byFlareVal;
	}

	bool ChangeStarEnhanceLvl(BYTE byLvl)
	{
		BYTE byOldVal = equipSpec.byStarEnhanceLv;
		equipSpec.byStarEnhanceLv = byLvl;
		if ( equipSpec.byStarEnhanceLv < 0){ equipSpec.byStarEnhanceLv = 0; }
		if ( equipSpec.byStarEnhanceLv > MAX_STARENHANCE_LEVEL	){ equipSpec.byStarEnhanceLv = MAX_STARENHANCE_LEVEL;}
		if ( byOldVal == equipSpec.byStarEnhanceLv )
		{
			return false;
		}
		return true;
	}

	VOID ChangePotVal(INT nValue)
	{
		INT nOrgPotVal = equipSpec.nPotVal;
		equipSpec.nPotVal += nValue;

		if(equipSpec.nPotVal < 0)		equipSpec.nPotVal = 0;

		if(equipSpec.nPotVal > pEquipProto->nMaxPotVal)
			equipSpec.nPotVal = pEquipProto->nMaxPotVal;

		if(nValue < 0)
			equipSpec.nPotValUsed += (nOrgPotVal - equipSpec.nPotVal);
	}

	VOID IncAttackTimes()
	{		
		++nUseTimes;

		if(eStatus != EUDBS_Update)
		{
			SetUpdate(EUDBS_Update);
		}
	}

	VOID DecAttackTimes(INT nTimes)
	{
		nUseTimes = min(MEquipMaxUseTimes(pEquipProto), nUseTimes);
		nUseTimes = max(0, nUseTimes - nTimes);

		if(eStatus != EUDBS_Update)
		{
			SetUpdate(EUDBS_Update);
		}
	}

	BOOL IsNewessChange(INT &nCurAttackTimes)
	{
		nCurAttackTimes = MEquipMaxUseTimes(pEquipProto);
		if(nUseTimes > nCurAttackTimes)
		{
			nUseTimes = nCurAttackTimes;
			return FALSE;
		}

		nCurAttackTimes = nUseTimes;
		if(nUseTimes % ABRASION2USETIMES == 0)
		{
			return TRUE;
		}

		return FALSE;
	}

	VOID GetTriggerIDBuffID(OUT DWORD &dwTriggerID, OUT DWORD &dwBuffID, IN INT nIndex)
	{
		dwTriggerID = dwBuffID = GT_INVALID;

		if(!P_VALID(pEquipProto))
		{
			ASSERT(0);
			return;
		}

		switch(nIndex)
		{
		case 0:
			dwTriggerID = pEquipProto->dwTriggerID0;
			dwBuffID	= pEquipProto->dwBuffID0;
			break;
		case 1:
			dwTriggerID = pEquipProto->dwTriggerID1;
			dwBuffID	= pEquipProto->dwBuffID1;
			break;
		case 2:
			dwTriggerID = pEquipProto->dwTriggerID2;
			dwBuffID	= pEquipProto->dwBuffID2;
			break;
		default:
			break;
		}
	}

	VOID GetLongHunID(DWORD &dwLongHunID, BOOL &bOuterLongHun) const
	{
		if(equipSpec.dwLongHunOuterID != GT_INVALID)
		{
			bOuterLongHun	= TRUE;
			dwLongHunID		= equipSpec.dwLongHunOuterID;
			return;
		}

		if(equipSpec.dwLongHunInnerID != GT_INVALID)
		{
			bOuterLongHun	= FALSE;
			dwLongHunID		= equipSpec.dwLongHunInnerID;
			return;
		}
	}

	BYTE GetLongfuAtt(INT nWuXingType) const
	{
		return equipSpec.byLongfuAtt[nWuXingType] + pEquipProto->byLongfuAtt[nWuXingType];
	}

	VOID Signature(TCHAR* szContent)
	{
		_tcscpy_s(equipSpec.szSignature, 11, szContent);
	}

	BYTE GetWpnPosyTimes()
	{
		return equipSpec.byPosyTimes;
	}
	BYTE GetWpnEngraveTimes()
	{
		return equipSpec.byEngraveTimes;
	}
	BYTE GetWpnEquipHoleCnt()
	{
		return equipSpec.byHoleNum;
	}

	DWORD GetLastHoleGemID()
	{
		if (equipSpec.byHoleNum >= 1 && equipSpec.byHoleNum <= MAX_EQUIPHOLE_NUM) // ��Ϊ�������byHoleNum-1�����Բ��õȺ�
			return equipSpec.dwHoleGemID[equipSpec.byHoleNum-1];
		else
			return 0;
	}

	BYTE GetBrandLevel()
	{
		return equipSpec.byBrandLevel;
	}
	VOID ReSetBrand()
	{
		equipSpec.byBrandLevel = 0;
		memset(equipSpec.nBrandAtt, 0, sizeof(equipSpec.nBrandAtt));
	}

	// Jason 2010-4-12 v2.0.0
	BOOL IsFaBao(VOID)
	{
		return pEquipProto->eType == EIT_FaBao;
	}
};


//-----------------------------------------------------------------------------
// �������� -- �ͽṹ���
//-----------------------------------------------------------------------------
const INT SIZE_ITEM						= sizeof(tagItem);
const INT SIZE_EQUIP					= sizeof(tagEquip);
const INT SIZE_EQUIPSPEC				= sizeof(tagEquipSpec);

// Jason 2010-4-13 v2.0.0
// ����ϵͳ
const INT MaxWuxingRewardNUM = 3;	// �������Ͻ�������
const INT MaxFabaoRoleAttNUM = 3;	// �������Ͻ�������
const INT MaxWuxingSlotNum	= 5;	// ���а��۸���
const INT MaxFairySkillNum	= 5;	// ��������ؼ�����
const INT MaxFairyBirthLevel		= 8; // ����40��60��80��100��120������Դ���

struct tagFabaoProto : public tagEquipProto
{
	ENUM_FabaoType	eFarbaoType;								// ��������
	ERoleAttribute	eAttType[MaxFabaoRoleAttNUM];			// Ӱ���ɫ��������
	INT16			nAttGrowing[MaxFabaoRoleAttNUM];		// ����Ӱ��ֵ,Ҳ���ɳ�����
	ERoleAttribute	eSlotAttType[MaxWuxingSlotNum];				// �������Ͻ�������
	INT				nSlotAttValue[MaxWuxingSlotNum];			// �������Ͻ���ֵ������100000��ֵΪ�ٷֱȡ�����110000=100%��101000=10%��
	BOOL			bCanReset;									// �Ƿ���������ת��������������
	BOOL			bCanUse;									// �Ƿ�������Ϊ��������
	BOOL			bHasDefaultValue;							// �Ƿ���Ĭ������
	INT16			n16DefaultFabaoQulity;						// �������ʣ�������Ĭ��������£���ȡԭ�ͱ��е�ֵ
	INT16			n16DefaultWuxing;							// ȱʡ��������
	//INT				nWuxingMatchReward[MaxWuxingRewardNUM];	// ȱʡ�������ϴ�����id
	EWuXing			eDefaultSlotType[MaxWuxingSlotNum];			// Ĭ�ϰ���
	INT				nIdfQltyPct[EIQ_End];						// Ʒ�ʼ�������

	// Jason 2010-10-24 ����Ԫ���˺��Լ����Ե�
	EElementInjury eEleInjury;		// Ԫ���˺�����
	INT			nEleInjury;		// Ԫ���˺�ֵ
	INT			nElePierce;		// Ԫ�ش�͸ [2/16/2012 zhangzhihua]

	INT			nEleResistance[EER_End];	// Ԫ�ؿ��ԣ���Ԫ�ؿ���ö������������
	DWORD		dwDef60StuntID;				// ��������60����ĳ��ѧϰ�����ؼ������Բ���
	BOOL		bCanBirth;						// �Ƿ�������
	INT16		n16CanFusion;				// �Ƿ�����ں� [3/22/2012 zhangzhihua]

	bool		CanFusion() const {return GT_INVALID != n16CanFusion;}
};

const INT MaxHolyEquipNum	= 6;		// ʥ���Ͽɷ�ʥ�Ƶĸ���
// ʥ�鸽������
struct tagHolyProto : public tagEquipProto
{
	INT nDevourNum;						// ʥ��������������ܴ���
	INT16 n16EquipmentNumber;			// ʥ���ʼ��װ����������
	INT	  nHolyMaxvalue;				// ���ܵ�����ֵ
	INT16 n16CallConsumption;			// ��ʥ���ٻ�״̬��ÿ�����ĵ�����ʥ��ֵ
	INT   nHolyDmg;						// ʥ���˺�ֵ
	INT	  nHolyDef;						// ʥ�����
	INT   nHolyValue;					// ��ʼ����ֵ
	INT	  nCrit;						// ����,Ӱ�������ʵ�����
	INT16 n16HolyCritRate;				// ������
	INT   nExDamage;					// �˺�����
	INT   nAttackTec;					// ��׼
	INT   nNeglectToughness;			// ����	
};

struct tagHolyEquipProto : public tagEquipProto
{
	INT   nCostHolyValue;									// װ����ʥ��Ҫ���ĵ�����ֵ
	INT   nCostMinHolyValue;								// װ����ʥ��Ҫ���ĵ���С����ֵ
	INT16 n16MaxLapNum;										// �����ܴ���
	INT16 n16WearLapNum;									// ����ʥ�����ظ�װ��������
	INT   nHolyLevel;										// ʥ�����ɵȼ�����

	tagHolyAttEffect BaseEffect[MAX_EQUIPATT_BASE_EFFECT];	// ����ǰ��װ��ʥ�� Ӱ�������(��Ʒ���޹�)
};



const INT MAX_FAIRY_SHAPE = 11;
struct tagFairyEffect
{
	DWORD dwTypeID;										// ����typeID
	TCHAR szFairyShape[MAX_FAIRY_SHAPE][X_SHORT_NAME];	// ������̬
	TCHAR szFirstPic[X_LONG_NAME];		// ��һ��̬
	TCHAR szSecondPic[X_LONG_NAME];		// �ڶ���̬
	TCHAR szThirdPic[X_LONG_NAME];		// ������̬
	tagFairyEffect()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// �����Ƚ�ԭ��
struct tagFabaoLevelProto
{
	INT		nID;
	INT		nMinUseLevel;			// ʹ�÷�����С�ȼ�Ҫ�󣬲���Ҫ������Ƶ�Ӵ
	INT		nLevelUpWhated;			// ������һ����Ҫ��������ֵ
	INT		nMinValue;				// �ﵽ�ü�����Ҫ���ٶ�������ֵ
	FLOAT	fCoefficientOfGrowing;	// �ɳ�ϵ��
};
// ����������������ԭ��
struct tagFabaoWuxingProto
{
	INT		nID;
	DWORD	dwTriggerID;				// ��������������
	DWORD	dwBuffID;					// ������buffid����ID
	INT		nDifficulty;				// ���������Ѷ�20 - 200
	INT16	n16NativeIntelligenceLim;	// ������������
	ENUM_FabaoType	eFabaoTypeLim;		// ������������
	INT		nXSRewardTimes;		// �������������������ͣ����ۣ��Ĳۣ���ۣ������ֲ�����
	INT		nRare;						// ���Ӷ�
};

struct tagTrumpWuXingAward
{
	DWORD dwTypeID;
	TCHAR szName[X_LONG_NAME];
	TCHAR szDesc[X_HUGE_NAME];
	tagTrumpWuXingAward(){ZeroMemory(this, sizeof(*this));}
};

// Jason 2010-12-6 v1.3.0������ֳ���
enum EFabaoStatus
{
	EEFS_Normal	=	0,	// ����״̬
	EEFS_Waiting	=	1,	// �ȴ�����ȷ��
	EEFS_Pregnancy	=	2,	// ����
	EEFS_GiveBirth	=	3,	// ����
	EEFS_Rest		=	4,	// ��Ϣ
};
struct tagFabaoSpec
{
	INT16		n16NativeIntelligence;	// ����:��ɫ�ķ�������Ϊ1~10����ɫ�ķ�������Ϊ11~20����ɫƷ�ʵķ�������Ϊ21~40����ɫƷ�ʵķ���Ʒ��Ϊ41~60����ɫ�ķ�������Ϊ61~80����ɫ�ķ�������Ϊ81~100
	INT16		n16Stage;				// �Ƚ�
	INT			nPracticeValue;			// ����ֵ
	BOOL		bStoredExpOpen;		// �����ӡ�Ƿ���
	INT			nCurStoredExp;			// ��ӡ�ľ���ֵ
	ERoleAttribute	eAttType[MaxFabaoRoleAttNUM];			// Ӱ���ɫ��������
	INT16			nAttGrowing0[MaxFabaoRoleAttNUM];		// ����Ӱ��ֵ,Ҳ���ɳ�����
	INT			nAttGrowing[MaxFabaoRoleAttNUM];		// ����Ӱ��ֵ,Ҳ���ɳ�����
	EWuXing		eWuxing;				// �������� 
	EWuXing		eWuxingHole[MaxWuxingSlotNum];					// �������
	DWORD		dwWuxingBeadID[MaxWuxingSlotNum];				// ��������id���õ���typeid�����ǡ�����
	ERoleAttribute	eSlotAttType[MaxWuxingSlotNum];				// �������Ͻ�������
	INT			nSlotAttValue[MaxWuxingSlotNum];		// �������϶Ƚ���
	//INT			nWuxingMatchReward[MaxWuxingRewardNUM];		// ������������
	INT			nUseLevel;										// �ȼ�����
	INT			nSlotNum;										// ��ǰ�����
	// 2010-4-16����
	BYTE			bySlotLingzhuMatch;							// ��ǰ����������
	BYTE		bySlotMacthNdx[MaxWuxingSlotNum];				// ��ǰ����Ƿ�����
	//ERoleAttribute	eAttLingzhu[MaxWuxingSlotNum][MaxWuxingRewardNUM];			// ��Ƕ���������Ӱ������
	//INT16			nAttLingzhuValue[MaxWuxingSlotNum][MaxWuxingRewardNUM];		// ����Ӱ��ֵ,Ҳ���ɳ�����

	INT			nMood;									// ���飬����
	INT			nMoodTicks;							// �����ʱ����������
	DWORD		dwMoodBuffID;		// ����״̬buff

	EElementInjury eEleInjury;		// Ԫ���˺�����
	INT			nEleInjury;		// Ԫ���˺�ֵ
	INT			nElePierce;			// Ԫ�ش�͸
	INT			nEleResistance[EER_End];	// Ԫ�ؿ��ԣ���Ԫ�ؿ���ö������������
	WORD		wEleInjPromoteTimes;		// �˺��������� [3/12/2012 zhangzhihua]	
	WORD		wEleResPromoteTimes;		// ������������ [3/12/2012 zhangzhihua]

	WORD		wDamageUpgradeTimes;				// �˺�����������
	WORD		wResistanceUpgradeTimes;			// ���Կ���������
	DWORD		dwFairySkill[MaxFairySkillNum];		// �����ؼ���Ĭ��GT_INVALID	
	DWORD		dwUnsureSkill;						// ��ȷ���ؼ���Ĭ��GT_INVALID

	EFabaoStatus	eFabaoStatus;					// ����״̬
	DWORD			dwStatusChangeTime;		// ״̬����ʱ�䣬��λ��
	INT				nBirthValue;				// ����ֵ�������ʱ���㣬����ʱ���߸�ֵ���������Ʒ
	INT				nBirthTimes;				// ��Դ���

	BYTE			byBirthLevel[MaxFairyBirthLevel]; // ������������ÿ���Դ���ʱ����¼�����ȼ�
	BYTE			byIsGivenBirthTimes;					// ���ֺܹ֣�hh���Ƿ�����������Ҫ��Ϊ�˸��Ѿ�������40���ϵ���Ҳ�����Դ����ж���

	BYTE			byMaxIntelligence;	// �������ޣ�δ�����������Ļ���ֵΪ0
	// Ԫ���˺����ͣ��˺�ֵ
	// ����ֵ����ö��ֵ���������

	tagFabaoSpec()
		:n16NativeIntelligence(0),	n16Stage(0),				nPracticeValue(0),				bStoredExpOpen(FALSE),		nCurStoredExp(0),
		eWuxing(EWX_Null),			nUseLevel(0),				nSlotNum(0),					bySlotLingzhuMatch(0),		nMood(0),
		nMoodTicks(0),				dwMoodBuffID(GT_INVALID),	wDamageUpgradeTimes(0),		wResistanceUpgradeTimes(0),	dwUnsureSkill(-1),
		nEleInjury(0),				eEleInjury(EEI_Null),		eFabaoStatus(EEFS_Normal),	dwStatusChangeTime(0),		nBirthValue(0),
		nBirthTimes(0),				byIsGivenBirthTimes(0),		byMaxIntelligence(0),			nElePierce(0),				wEleInjPromoteTimes(0),
		wEleResPromoteTimes(0)
	{
		memset(eWuxingHole,EWX_Null,sizeof(eWuxingHole));
		memset(dwWuxingBeadID,0,sizeof(dwWuxingBeadID));
		memset(eSlotAttType,ERA_Null,sizeof(eSlotAttType));
		memset(nSlotAttValue,0,sizeof(nSlotAttValue));
		//memset(nWuxingMatchReward,0,sizeof(nWuxingMatchReward));
		memset(bySlotMacthNdx,0,sizeof(bySlotMacthNdx));
		//memset(eAttLingzhu,ERA_Null,sizeof(eAttLingzhu));
		//memset(nAttLingzhuValue,0,sizeof(nAttLingzhuValue));
		memset(nEleResistance,0,sizeof(nEleResistance));
		memset(dwFairySkill,GT_INVALID,sizeof(dwFairySkill));
		memset(byBirthLevel,0,sizeof(byBirthLevel));
	}
};

struct tagHolySpec
{
	INT		nDevourNum;							// ���������ĵ�ǰ����
	INT16	n16EquipmentNumber;					// ʥ���װ����������
	INT		nCoValue;							// Ĭ��ֵ
	INT		nToDayEatNum;						// ������˶���ֻ��~
	INT64	n64EquipSerialIDs[MaxHolyEquipNum];	//  ʥ����ʥ��64ID
	INT		nCostHoly;							// װ��ʥ�������ĵ�����
	INT		nMaxDevourNum;						// ���ɳ�����������ͨ������������δ����ʱ��proto���е�nDevourNum�ֶ�

	DWORD dwLevelUpAtt[8];			//ʥ��ǿ������������飬��һ��ֵ��ʾ�ȼ��ڶ�������ʾ��������  �ڶ�����Ӧ�ö�ӦESAT_End


	//INT   nHolyDmg;						// ʥ���˺�ֵ
	//INT	  nHolyDef;						// ʥ�����
	//INT	  nCrit;						// ����,Ӱ�������ʵ�����
	//INT16 n16HolyCritRate;				// ������
	//INT   nExDamage;					// �˺�����
	//INT   nAttackTec;					// ��׼
	//INT   nNeglectToughness;			// ����
	//INT   nCoValue;						// Ĭ��ֵ

	tagHolySpec()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct tagHolyEquipSpec
{
	INT   nCostHoly;							// װ����ʥ��Ҫ���ĵ�����
	UINT16 n16EnhanceCount;						// ʥ�Ƶĳ��ܴ���
	INT	  nHolyAttEffect[X_ESAT_ATTA_NUM];		// ��ʥ�������Ӱ��ֵ	

	tagHolyEquipSpec()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// ������ֳ��С�ȼ�
#define FABAO_MATING_MIN_STAGE	40
// ������ֳ��С����
#define FABAO_MATING_MIN_INTELLIGENCE	70
// ��������cdֹͣ����
#define FABAO_MATING_CD_STOP	(60*60)

struct tagFabao : public tagEquip,public tagFabaoSpec
{
	//const tagFabaoProto * pFabaoProto;	// ����ԭ��
	tagFabao(){}
	//ԭ��ָ��

	BOOL CanMating() // �Ƿ�������
	{
		if( equipSpec.byQuality < EIQ_Orange )
			return FALSE;
		if( n16Stage < FABAO_MATING_MIN_STAGE )
			return FALSE;
		if( n16NativeIntelligence < FABAO_MATING_MIN_INTELLIGENCE )
			return FALSE;
		if( P_VALID(pEquipProto) && (((tagFabaoProto*)pEquipProto)->bCanBirth == GT_INVALID))
			return FALSE;
		return TRUE;
	}
	INT GetIncTimes(INT nLastStage)
	{
		static INT nTimes[] = {0,1,2,3,4};
		INT last = 0;
		INT cur = 0;
		if(nLastStage >= FABAO_MATING_MIN_STAGE)
			last = (nLastStage - FABAO_MATING_MIN_STAGE/2) / 20;
		if(n16Stage >= FABAO_MATING_MIN_STAGE)
			cur = (n16Stage - FABAO_MATING_MIN_STAGE/2) / 20;
		return nTimes[cur] - nTimes[last];
	}
	BOOL CanIncBirthTimes(int stage)
	{
		BOOL bFull = TRUE;
		if( P_VALID(pProtoType) )
		{
			tagFabaoProto * pFabaoProto = (tagFabaoProto*)pProtoType;
			if( pFabaoProto->bCanBirth == GT_INVALID )
				return FALSE;
		}
		for( int i = 0; i < MaxFairyBirthLevel; ++i )
		{
			if( !P_VALID(byBirthLevel[i]) )
				bFull = FALSE;
			if(byBirthLevel[i] == stage)
				return FALSE;
		}
		return !bFull;
	}
	BOOL IncBirthTimes(int nLastLevel)
	{
		if( P_VALID(pProtoType) )
		{
			tagFabaoProto * pFabaoProto = (tagFabaoProto*)pProtoType;
			if( pFabaoProto->bCanBirth == GT_INVALID )
				return FALSE;
		}
		if( n16Stage >= FABAO_MATING_MIN_STAGE )
		{
			BOOL bFinded = FALSE;
			if( nLastLevel < FABAO_MATING_MIN_STAGE )
			{
				bFinded = TRUE;
				// ����ʼֵ
				tagFabaoProto * pFabaoProto = (tagFabaoProto*)pProtoType;
				if(P_VALID(pFabaoProto) && pFabaoProto->bCanBirth != GT_INVALID)
					nBirthTimes = pFabaoProto->bCanBirth;
			}
			else if( (nLastLevel - FABAO_MATING_MIN_STAGE) / 20 != (n16Stage  - FABAO_MATING_MIN_STAGE) / 20 )
				bFinded = TRUE;
			if( !bFinded )
				return FALSE;
			
			bFinded = FALSE;
			int nCurStage = (n16Stage / 10) * 10;
			int nNdx = -1;
			for( int i = MaxFairyBirthLevel - 1; i >= 0 ; --i )
			{
				if( byBirthLevel[i] == nCurStage)
				{
					bFinded = TRUE;
					break;
				}
				if( byBirthLevel[i] == 0 )
				{
					nNdx = i;
				}
			}
			if( !bFinded && nNdx >= 0 && nNdx < MaxFairyBirthLevel )
			{
				nBirthTimes += GetIncTimes(nLastLevel);
				byBirthLevel[nNdx] = nCurStage;
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL CanExchange() const
	{
		switch( eFabaoStatus )
		{
		case EEFS_Pregnancy:
		case EEFS_Rest:
			return FALSE;
		}
		return TRUE;
	}
	BOOL CanBirth() const
	{
		if( P_VALID(pEquipProto) )
		{
			const tagFabaoProto * pPro = (const tagFabaoProto*)pEquipProto;
			if( pPro->bCanBirth != GT_INVALID)
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL LogFairyBirth(INT nBeforeChangeBirthState, INT nAfterChangeBirthState);
	BOOL IsBirthChangeValid(INT nBeforeChangeBirthState, INT nAfterChangeBirthState)
	{
		if (nBeforeChangeBirthState == EEFS_Rest && nAfterChangeBirthState == EEFS_Normal)
			return TRUE;
		if (nBeforeChangeBirthState > nAfterChangeBirthState)
			return FALSE;
		if (nBeforeChangeBirthState == nAfterChangeBirthState)
			return TRUE;
		if (nAfterChangeBirthState - nBeforeChangeBirthState == 1)
			return TRUE;
	}
};/*�����ṹ����*/

// ʥ��ṹ����
struct tagHolyMan :public tagEquip, public tagHolySpec
{
	tagHolyMan(){};		
};

// ʥ�ƽṹ����
struct tagHolyEquip :public tagEquip, public tagHolyEquipSpec
{
	tagHolyEquip(){};		
};



static CONST INT FabaoQltyNativeIntelligence[][3] = 
{
	{ 1,50,50 },	/*��ɫƷ��*/
	{ 1,50,50 },	/*��ɫƷ��*/
	{ 1,50,50 },	/*��ɫƷ��*/
	{ 50,80,70 },	/*��ɫƷ��*/
	{ 50,90,80 },	/*��ɫƷ��*/
	{ 60,100,80 },	/*��ɫƷ��*/
	{ 60,100,90 }	/*��ɫƷ��*/
};
#define MGetFabaoQltyIntel(qlty,minIntel,maxIntel)	do{ \
	if( qlty > EIQ_Start && qlty < EIQ_End ) \
	{\
		minIntel = FabaoQltyNativeIntelligence[qlty][0];\
		maxIntel = FabaoQltyNativeIntelligence[qlty][1];\
	}\
}while(0)

#define MGetFabaoQltyIntelWhenCreate(qlty,minIntel,maxIntel)	do{ \
	if( qlty > EIQ_Start && qlty < EIQ_End ) \
{\
	minIntel = FabaoQltyNativeIntelligence[qlty][0];\
	maxIntel = FabaoQltyNativeIntelligence[qlty][2];\
}\
}while(0)

CONST INT SIZE_FABAO	= sizeof(tagFabao);
CONST INT SIZE_HOLY	= sizeof(tagHolyMan);
CONST INT SIZE_HOLYEQUIP	= sizeof(tagHolyEquip);

//-----------------------------------------------------------------------------
// �꾧�ṹ����
//-----------------------------------------------------------------------------
struct tagSoulCrystalSpec
{
	BYTE bySoulCrystalQlty;		// ����
	BYTE bySoulCrystalLvl;			// �ȼ� 0-10
	DWORD dwStaticSkillID;		// �Դ�����
	INT nSoulAttID[SOUL_CRYSTAL_ATT_MAX_NUM];	// �ɳ�����			
	DWORD dwSoulStrength;	// ����ǿ��

	tagSoulCrystalSpec()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct tagSoulCrystal : public tagEquip/*, public tagSoulCrystalSpec*/
{
	tagSoulCrystalSpec SoulSpec;
};

CONST INT SIZE_HUNJING	= sizeof(tagSoulCrystal);

// ������Ʒ&װ��(������tagItem��ʹ������������)
static VOID Destroy(tagItem *&pItem)
{
	if(!P_VALID(pItem))
	{
		return;
	}

	if( P_VALID(pItem->pProtoType) && EIT_FaBao  == pItem->pProtoType->eType )
		delete ((tagFabao*)pItem);
	else if( P_VALID(pItem->pProtoType) && EIT_SoulCrystal == pItem->pProtoType->eType )
		delete ((tagSoulCrystal*)pItem);
	else if( P_VALID(pItem->pProtoType) && EIT_Holy == pItem->pProtoType->eType )
		delete ((tagHolyMan*)pItem);
	else if( P_VALID(pItem->pProtoType) && EIT_HolyEquip == pItem->pProtoType->eType )
		delete ((tagHolyEquip*)pItem);
	else if(MIsEquipment(pItem->dwTypeID))
	{
		delete ((tagEquip*)pItem);
	}
	else
	{
		delete pItem;
	}

	pItem = NULL;
}

// ������Ʒ&װ��(������tagItem��ʹ������������)
static VOID Destroy(tagEquip *&pEquip)
{
	if( P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_FaBao  == pEquip->pProtoType->eType )
	{
		delete ((tagFabao*)pEquip);
		pEquip = NULL;
		return;
	}
	else if (P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_SoulCrystal  == pEquip->pProtoType->eType)
	{
		delete ((tagSoulCrystal*)pEquip);
		pEquip = NULL;
		return;
	}
	else if (P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_Holy  == pEquip->pProtoType->eType)
	{
		delete ((tagHolyMan*)pEquip);
		pEquip = NULL;
		return;
	}
	else if (P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_HolyEquip  == pEquip->pProtoType->eType)
	{
		delete ((tagHolyEquip*)pEquip);
		pEquip = NULL;
		return;
	}
	if(!P_VALID(pEquip))
	{
		return;
	}

	SAFE_DEL(pEquip);
}

// Jason 2010-8-26 v1.0.0 ������Լ���ĵ����itemtypeids
static const DWORD ItemsOfFairyContract[]=
{
	4080080,	// �˰�֮�����������Ƶı˰���
	4080081,	// ����֮ʯ
	4080082,	// ����֮ˮ
	4080083,	// �û�֮��
	4080084	// ��Լ֮��
};

/** struct	tagItemDisplayInfo
brief �����ؼ�
*/
struct tagFairySkill
{
	DWORD dwTypeID;							// ��ƷID
	INT nType;
	TCHAR szName[X_SHORT_NAME];				// ��Ʒ����
	TCHAR szDesc[X_HUGE_NAME];				// ��Ʒ����
	tagFairySkill(){ZeroMemory(this, sizeof(*this));}
};

static INT GetFairyMatingValue(const tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return 0;
	switch(pFabao->equipSpec.byQuality)
	{
	case EIQ_Orange:
		return 15;
	case EIQ_Purple:
		return 30;
	case EIQ_Pink:
		return 50;
	}
	return 0;
}

#define MAX_GIFT_ITEM_NUM 5
#define MAX_GIFT_ITEM_SHOW_NUM 6	//��ʾ��Ʒ�����...
struct tagIDBuyBack
{
	DWORD dwID;
	DWORD dwExp;
	DWORD dwGiftID;
	DWORD dwItemID[MAX_GIFT_ITEM_NUM];
};
struct tagIDBuyBackEx
{
	DWORD dwID;
	DWORD dwItemID[MAX_GIFT_ITEM_SHOW_NUM];
	tstring strDesc[MAX_GIFT_ITEM_SHOW_NUM];
};

#define NewKeyCodeString _T("JDSHJKSDHF")
#define NewKeyCodeGiftid 4700294

#define SaveTelNumGiftid 4830067
#define YuanBaoDaiBi 4810054

//�����ں�
struct tagFairyFusionRetProto
{
	DWORD dwID;			//id
	INT byLowScore;	//��ͷ���
	INT byHighScore;	//��߷���
	DWORD dwPinkRet;	//��ɫ����
	DWORD dwPurpleRet;	//��ɫ����
	DWORD dwOrangeRet;	//��ɫ����
	DWORD dwFailRet;	//ʧ�ܼ���
};

//�����ں� ������
enum
{
	EFairyFusion_Orange = 0,
	EFairyFusion_Purple,
	EFairyFusion_Pink,

	EFairyFusion_End,
};
struct tagFairyFusionQualityProto
{
	DWORD dwID;			//id
	INT16 n16LowScore;	//��ͷ���
	INT16 n16HighScore;	//��߷���
	INT16 n16QualityRate[EFairyFusion_End];	//��ɫ����
};

const INT MAX_FAIRY_FUSION_TYPE = 29;
struct tagFairyFusionTypeProto
{
	DWORD dwID;			//id
	DWORD dwTypeID[MAX_FAIRY_FUSION_TYPE];	//�漴������ID
	INT16 n16Rate[MAX_FAIRY_FUSION_TYPE];		//���ֵĸ���
};

struct tagWarRewardPointProto
{
	DWORD dwID;
	DWORD dwExp;
	DWORD dwMoney;
};

//-----------------------Ԫ��-------------------------------------------------
// Ԫ�����
const INT		MAX_HOLY_VALUE						= 99999;
const INT		MAX_PRIVITE_VALUE					= 999;
const INT64     MAX_HOLY_EXP						= 999999999;
const INT       MIN_HOLY_SOUL_LEVEL					= 15;			 // Ԫ����͵ȼ�
//const DWORD		HOLYSOUL_BUFFID						= 99905;		 // Ԫ��buff����ID
const INT		DEC_SOULVAL_PER_SECOND				= 1;			 // ÿ������Ԫ��ֵ
const INT		ADD_SOULVAL_PER_MINUTE				= 1;			 //	ÿ��ָ�Ԫ��ֵ
const INT		CALLSOUL_MIN_SOULVAL				= 25;			 // �ٻ�Ԫ���Ԫ��ֵ����
const INT		HOLYSOUL_MAX_LEVEL					= 100;			 // ÿһתԪ�����ȼ�
const INT		HOLYSOUL_INITID						= 10001;		 // Ԫ���ʼ��ID
const INT		COVALUE_OFFTIME_PER_HOUR			= 5;			 // ����ÿСʱ�۳�Ĭ��ֵ
const INT		COVALUE_ONTIME_PER_SECOND			= 1;			 // ����Ԫ�����״̬�ָ�Ĭ��ֵ
//const INT		HOLYSOUL_SKILLID					= 6020104;		 // �ٻ�Ԫ��ļ���ID
const INT		ADD_SOULVAL_TIME					= 5;			 // ÿ5������һ��Ԫ��ֵ

const INT		REBORN_MAX_LEVEL					= 3;		 // Ԫ��ת����ߵȼ�
const DWORD REBORN_ITEM_TYPEID_LV1 = 6020101;		//��Ԫ���� һת
const DWORD REBORN_ITEM_TYPEID_LV2 = 6020102;		//��Ԫ���� ��ת
const DWORD REBORN_ITEM_TYPEID_LV3 = 6020103;		//��Ԫ���� ��ת

const DWORD	REBORN_STUFF_BINDING		= 2005301;	// �Ͼ���¶
const DWORD	REBORN_STUFF_UNBINDING	= 2005303;	// �Ͼ���¶(�ǰ�)

//-----------------------------------------------------------------------------
// Ԫ��״̬����
//-----------------------------------------------------------------------------
enum EHolySoulState
{
	EHSS_Asleep         = 0,       //Ԫ������
	EHSS_Awake          = 1,       //Ԫ�����
};

//-----------------------------------------------------------------------------
// Ԫ���������
//-----------------------------------------------------------------------------
struct tagSoulAttProto
{
// 	DWORD	    dwID;                //Ԫ��ID
// 	INT64		n64UpLvExp;		     //Ԫ���������辭��
// 	INT16		n8RebornLv;		     //Ԫ��ת���ȼ� 
// 	INT16		n8SoulLevel;		 //Ԫ��ȼ� 150
// 	INT			nDamage;			 //Ԫ��������˺�
// 	INT			nHit;				 //��������
// 	INT			nCrit;				 //��������
// 	INT			nCritAmount;	     //����������
// 	INT			nExattack;			 //�⹦����
// 	INT			nInattack;			 //�ڹ�����		
// 	INT			nAccrate;			 //��׼
// 	INT			nExdamage;		     //�����˺�
// 	INT			nToughness;		     //��������
// 	INT16        n8Morale;	       	 //ʿ��
// 	INT16		n8Covalue;		     //Ĭ��ֵ��ǰ���ֵ	
// 	DWORD       dwSoulValue;         //Ԫ��ǰ���ֵ

	DWORD		dwID;	//id
	BYTE		byEquipNum;				//��װ������
	DWORD		dwHolyDmg;				//ʥ���˺�
	DWORD		dwHolyValue;			//����ֵ(��ʼֵ)
	DWORD		dwHolyValueMax;			//��������(���������������)
	DWORD		dwCallConsumption;		//�ٻ�ʱ��������ֵ
	DWORD		dwCritValue;			//����
	DWORD		dwCritRate;				//������
	DWORD		dwExDmg;				//�˺�����
	DWORD		dwAttackTec;			//��׼
	DWORD		dwCoValue;				//Ĭ��ֵ
	DWORD		dwEatNum;				//������������
	tagSoulAttProto()
	{
		memset(this,0,sizeof(*this));
	}
};

struct tagSoulAttName : tagHolyProto
{
	TCHAR szMode_Low[128];		//�ͽ�ģ�ͱ��
	TCHAR szMode_High[128];		//�߽�ģ�ͱ��
	TCHAR szIcon_Low[128];		//�ͽ�ͼ��
	TCHAR szIcon_High[128];		//�߽�ͼ��
	TCHAR szEffect_Low[128];	//�ͽ���Ч���
	TCHAR szEffect_High[128];	//�߽���Ч���
	DWORD	dwHighEatNum;		//�Զ��ٸ������Ժ��ɸ߽�~
	DWORD	dwScal;				//����
};


//-----------------------------------------------------------------------------
// Ԫ�������
//-----------------------------------------------------------------------------
struct tagSoulAtt
{
	INT			nSoulAtt[ESAT_End];
};

//-----------------------------------------------------------------------------
// Ԫ��ǰ����
//-----------------------------------------------------------------------------
struct tagSoulAttEx
{
	DWORD	    dwSoulID;
	INT64		n64LevelExp;	//Ԫ��ǰ����
	INT64		n64MaxExp;		//Ԫ���������辭��
	INT			nCurSoulValue;
	INT			nMaxSoulValue;
	INT			nCurCovalue;	//��ǰĬ��ֵ	
	INT			nMaxCovalue;	//��ǰĬ��ֵ	
	tagSoulAttEx()
	{
		memset(this,0,sizeof(*this));
	}
};

//-----------------------------------------------------------------------------
// Ԫ���������
//-----------------------------------------------------------------------------
struct tagHolySoulInfo
{
	tagSoulAtt    SoulAtt;
	tagSoulAttEx  SoulAttEx;
	tagHolySoulInfo()
	{
		memset(this,0,sizeof(*this));
	}
};


//Ԫ��ת������
struct tagHolySoulReborn
{
	BYTE	byCostNum;	//�����Ͼ���¶������
	INT		nRate1;		//һת�ɹ���
	INT		nRate2;		//��ת�ɹ���
	tagHolySoulReborn()
	{
		memset(this,0,sizeof(*this));
	}
};


//Ԫ���������
struct tagHolySoulEquipHole
{
	BYTE	byID;			//Ԫ��װ���Ŀ�λ
	INT		nRate;			//�����λ��Ӧ�ĳɹ�����
	INT		nCostMoney;		//װ��������ĵĽ�Ǯ��
	tagHolySoulEquipHole()
	{
		memset(this,0,sizeof(*this));
	}
};

//Ԫ����Ƕ����
struct tagHolySoulEquipInlay
{
	BYTE	byID;			//Ԫ��װ���Ŀ�λ
	INT		nRate;			//��Ƕ��Ӧ�ĳɹ�����
	INT		nCostMoney;		//��Ƕ���ĵĽ�Ǯ��
	tagHolySoulEquipInlay()
	{
		memset(this,0,sizeof(*this));
	}
};
//--------------------------------------------------------------------------------
//ʥ��������������proto
struct tagHolyLevelUpProto
{
	DWORD dwID;				//id��ʾ����
	DWORD dwDmgMin;			//�˺���Сֵ
	DWORD dwDmgMax;			//�˺����ֵ
	DWORD dwDefMin;			//������Сֵ
	DWORD dwDefMax;			//�������ֵ
	DWORD dwLingNengMin;	//������Сֵ
	DWORD dwLingNengMax;	//�������ֵ
	DWORD dwCritMin;		//������Сֵ
	DWORD dwCritMax;		//�������ֵ
	DWORD dwCritValueMin;	//��������Сֵ
	DWORD dwCritValueMax;	//���������ֵ
	DWORD dwDmgAddMin;		//�˺�������Сֵ
	DWORD dwDmgAddMax;		//�˺��������ֵ
	DWORD dwAttackTecMin;	//��׼��Сֵ
	DWORD dwAttackTecMax;	//��׼���ֵ
	DWORD dwNeglectMin;		//������Сֵ
	DWORD dwNeglectMax;		//�������ֵ
};

//--------------------------------------------------------------------------------
//ʥ��ǿ�����Ա�
struct tagHolyEquipEnhance
{
	DWORD dwID;							//id
	DWORD dwPowMin[X_ESAT_ATTA_NUM];	//����������Сֵ
	DWORD dwPowMax[X_ESAT_ATTA_NUM];	//�����������ֵ

	DWORD dwLingNengPow;		//���ٵ���������

	tagHolyEquipEnhance()
	{
		memset(this,0,sizeof(*this));
	}
};

//ս�������ṹ
struct tagWarReward
{
	DWORD dwID;
	DWORD dwExpReward;	//ÿ�ָ����پ���
	DWORD dwMoneyReward;//ÿ�ָ����ٽ�Ǯ
};

// static CONST FLOAT FairyQltyScore[4] = 
// {
// 		1.25,	//��ɫƷ��
// 		1.75,	//��ɫƷ��
// 		2.75,	//��ɫƷ��
// 		4		//��ɫƷ��
// };
// //��������Ʒ�� �ȼ�  ���ʼ����ֵ,�������ɫ����
// //���ȼ�����*��Ʒ��Ӱ��*1.5��+���ʷ���*��Ʒ��Ӱ��*1.8����*10
// //�ȼ�������1.25*(1+(�����ȼ�/100)^3)
// //���ʷ�����1.75*(1+(��������/100)^5)
// #define CallEatFairyScore(nQuility, nLevel, nIntelligence, nOutPut)	do{ \
// 	if( nQuility >= EIQ_Blue && nQuility <= EIQ_Pink ) \
// {\
// 	nOutPut = (INT)((1.25f * (1 + pow((nLevel/100.0f), 3)) * FairyQltyScore[nQuility-3]*1.5f + 1.75f * (1+pow((nIntelligence/100.0f), 5)) * FairyQltyScore[nQuility-3]*1.8f)*10.0f);\
// }\
// }while(0)

static CONST FLOAT FairyQltyScore[3] = 
{
		0,	//��ɫƷ��
		20,	//��ɫƷ��
		40,	//��ɫƷ��
};
#define CallEatFairyScore(nQuility, nLevel, nOutPut)	do{ \
	if( nQuility >= EIQ_Blue && nQuility <= EIQ_Purple ) \
{\
	nOutPut = FairyQltyScore[nQuility - EIQ_Blue] + nLevel;\
}\
}while(0)


static CONST DWORD GetColorValue[] =
{
	0xFFFFF7E0,	//��ɫ
	0xfffff717, //��ɫ
	0xff05ff00, //��ɫ
	0xff00a2ff, //��ɫ
	0xffff7800, //��ɫ
	0xff9932cd, //��ɫ
	0xffff80c0, //��ɫ
};
#pragma pack(pop)

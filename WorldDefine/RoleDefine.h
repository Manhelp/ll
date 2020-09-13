#pragma once
/********************************************************************
	created:	2010/06/09
	created:	9:6:2010   16:23
	filename: 	e:\F-Project\outofcontrol\trunk\WorldDefine\RoleDefine.h
	file path:	e:\F-Project\outofcontrol\trunk\WorldDefine
	file base:	RoleDefine
	file ext:	h
	author:		
	
	purpose:	F-Project
	history:
			2010-6-9 �޸���ؽ�ɫ�������ƣ������������
*********************************************************************/
#include "base_define.h"
#include "AvatarAtt.h"
#include "QuestDef.h"

#pragma pack(push, 1)

const INT	MAX_ROLENUM_ONEACCOUNT		= 5;				// �ʺŵ�����ɫ����
const INT	MAX_ROLE_LEVEL				= 150;				// ��ҿ���������߼���
const DWORD MAX_SAFECODE_RESET_TIME		= (72 * 60 * 60);	// ��ȫ���������ʱ��(72Сʱ)

const INT	ROLE_LEVEL_LIMIT			= 100;				// �ȼ�����

const DWORD MAX_KILL_MONSTER			= 99999;			// ��ɱ�����������
const DWORD MAX_KILL_ROLE				= 9999;				// ��ɱ��ɫ��������

//---------------------------------------------------------------------------
// ���ݱ仯״̬,���ݿⱣ�����
//---------------------------------------------------------------------------
enum EUpdateDBStatus
{
	EUDBS_Null			= 0, // ������(Ĭ��״̬)
	EUDBS_Insert		= 1, // ������¼
	EUDBS_Remove		= 2, // ɾ��
	EUDBS_Update		= 3, // ���ݸ���
};

//---------------------------------------------------------------------------
// �Ա�
//---------------------------------------------------------------------------
enum ESex
{
	ES_Null			= -1,
	ES_Woman		= 0,	// Ů���ƣ���Ů����
	ES_Man			= 1,	// �����ƣ����п���
};

// ��Ϣ���ͷ�Χ [4/28/2012 zhangzhihua]
enum EMessageRange
{
	EMR_Self		= 0,	//
	EMR_Around,				//
	EMR_Map,				//
	EMR_World,				//
};

//----------------------------------------------------------------------------
// ְҵ����
//----------------------------------------------------------------------------
//enum EClassType
//{
//	ECT_Start		= -1,
//
//	ECT_Null		= -1,
//	ECT_Hobo		= 0,	// ����
//	ECT_Sword		= 1,	// ����
//	ECT_Blade		= 2,	// ����
//	ECT_Wand		= 3,	// �ȿ�
//	ECT_Bow			= 4,	// ����
//	ECT_Valor		= 5,	// ��ʦ
//	ECT_Supple		= 6,	// ��ʦ
//	ECT_Apothecary	= 7,	// ҩʦ
//	ECT_Musician	= 8,	// ��ʦ
//	ECT_Stabber		= 9,	// �̿�
//
//	ECT_End			= 10,
//};
//
//const INT32 X_ClASS_TYPE_NUM = ECT_End - ECT_Start - 1;

//---------------------------------------------------------------------------
// ְҵ������չ
//---------------------------------------------------------------------------
//enum EClassTypeEx
//{
//	ECTE_Base		= 0x1,	// ����ְҵ
//
//	ECTE_JianXian	= 0x2,	// ����
//	ECTE_XieWu		= 0x4,	// а��
//
//	ECTE_BaDao		= 0x2,	// �Ե�
//	ECTE_GangRen	= 0x4,	// ���
//
//	ECTE_ZongZhang	= 0x2,	// ����
//	ECTE_DiMo		= 0x4,	// ��ī
//
//	ECTE_TianJian	= 0x2,	// ���
//	ECTE_MoLing		= 0x4,	// ħ��
//
//	ECTE_XuXing		= 0x2,	// ����
//	ECTE_GuiQi		= 0x4,	// ����
//
//	ECTE_XuangWang	= 0x2,	// ����
//	ECTE_MiZong		= 0x4,	// ����
//
//	ECTE_XianYi		= 0x2,	// ��ҽ
//	ECTE_DuYin		= 0x4,	// ����
//n 
//	ECTE_YuSheng	= 0x2,	// ��ʥ
//	ECTE_HuanYin	= 0x4,	// ����
//
//	ECTE_YingWu		= 0x2,	// Ӱ��
//	ECTE_GuiWu		= 0x4,	// ����
//};

//---------------------------------------------------------------
// ר��ְҵ
//---------------------------------------------------------------
enum EClassType				 
{
	EV_Begin = -1,
    EV_Null  = 0,
	
	EV_Base	= 1,	// ����ְҵ������

	EV_Blade,		// ���
	EV_Bow,			// ����
	EV_Strength,	// Ǭ��
	EV_Musician,	// ����
	EV_Fuhu,		// ����
	EV_XuanJia,		// ���
	EV_HuanShi,		// ��ʸ
	EV_ZhanXin,		// ռ��
	EV_LeiMang,		// ����
	EV_GuaXiang,	// ��ħ
	EV_ShenShi,		// ����
	EV_TianYi,		// ����

	EV_End,
};

const INT32 X_ClASS_TYPE_NUM = EV_End - EV_Begin - 2;
//--------------------------------------------------------------
// Ӣ��ְҵ
//--------------------------------------------------------------
enum EClassTypeEx
{
	EHV_Begin = -1,

	EHV_Base = 0,		// ����ְҵ��δѡ��Ӣ��ְҵ

	EHV_JianXian,		// �ɽ�
	EHV_XieWu,			// а��

	EHV_BaDao,			// �Ե�
	EHV_YingWu,			// Ӱ��
	
	EHV_JinGang,		// ���
	EHV_KuangZhan,		// ��ս
	
	EHV_TianJian,		// ���
	EHV_MoLing,			// ħ��
	
	EHV_XuanHuang,		// ����
	EHV_MiaoFa,			// �
	
	EHV_MingWang,		// ڤ��
	EHV_XuKong,			// ���
	
	EHV_XianYi,			// ��ҽ
	EHV_DuYin,			// ����
	
	EHV_YueSheng,		// ��ʥ
	EHV_HuanYin,		// ����

	EHV_End,
};

//--------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------
enum ETalentType
{
	ETT_Start		= -1,
	ETT_Null		= -1,

	ETT_Blade		= 0, // ���
	ETT_Bow			= 1, // ����
	ETT_Valor		= 2, // Ǭ��
	ETT_Swing		= 3, // ����
	ETT_FreshHand	= 4, // ���ּ���

	ETT_End			= 5,
};

const INT32 X_TALENT_TYPE_NUM = ETT_End - ETT_Start - 1;

//--------------------------------------------------------------------------
// �������ͣ����ܻ����������͵�������
//--------------------------------------------------------------------------
// enum EReputationType
// {
// 	ERT_Start		= -1,
// 	ERT_Null		= -1,
// 
// 	ERT_End			= 0,
// };

//--------------------------------------------------------------------------
// ��ɫ�����﹫��״̬��ʾ
//--------------------------------------------------------------------------
enum EState
{
	ES_Dead			=	0,	// ����
	ES_Dizzy		=	1,	// ѣ��
	ES_Tie			=	2,	// ����
	ES_Spor			=	3,	// ��˯
	ES_Invincible	=	4,	// �޵�
	ES_Lurk			=	5,	// ����
	ES_DisArm		=	6,	// ��е
	ES_NoSkill		=	7,	// �⼼

	ES_CSAttack		=	8,	// ��ս������Ӫ
	ES_CSDefence	=	9,	// ��ս������Ӫ

	ES_PvPAttack	=	10,	// ��������Ӫ
	ES_PvPDefence	=	11,	// ��������Ӫ
	ES_NoMovement	=	12, // ��ֹ�ƶ�
	ES_NoJump		=	13, // �޷���Ծ

	ES_End			=	14,
};

//--------------------------------------------------------------------------
// ��ɫ״̬��ʶ -- ��״̬�ı䣬��Ҫ����Χ��ҹ㲥
//--------------------------------------------------------------------------
enum ERoleState
{
	ERS_Safeguard	= 0x0001,	// ������
	ERS_Combat		= 0x0002,	// ս����̬
	ERS_PK			= 0x0004,	// ɱ¾ģʽ
	ERS_Arena		= 0x0008,	// ��̨(����PVP���ս������״̬�ɽű����ã�

	ERS_SafeArea	= 0x0010,	// ��ȫ��
	ERS_PVPArea		= 0x0020,	// PVP��
	ERS_StallArea	= 0x0040,	// ��̯��
	ERS_PVP			= 0x0080,	// PVP״̬

	ERS_Shop		= 0x0100,	// �̵꽻��
	ERS_Exchange	= 0x0200,	// ��Ҽ�ֱ�ӽ���
	ERS_Stall		= 0x0400,	// ��̯����
	ERS_StallSet	= 0x0800,	// ���ð�̯��Ʒ

	ERS_PrisonArea	= 0x1000,	// ����״̬

	ERS_Mount		= 0x2000,	// ���
	ERS_WaterWalk   = 0x4000,   // ˮ������
	ERS_Mount2		= 0x8000,	// ���2

	ERS_Commerce	= 0x10000,	// ������	
	ERS_PetExchange	= 0x20000,	// ���ｻ��

	ERS_Hostility	= 0x40000,	// ����״̬

	ERS_Wedding		= 0x80000,	// ����״̬

	ERS_PKEX		= 0x100000,	// �Կ�ģʽ

	ERS_TaxArea		= 0x200000,	// ��˰��
	ERS_ScriptArea	= 0x400000,	// �ű���
	ERS_PeaceModel	= 0x800000,	// ����ģʽ

	ERS_HoldChest	= 0x1000000,// �ᱦ����Я��ģʽ
	ERS_WardrobeWare= 0x2000000,// �³�ʱװ����ģʽ
	ERS_Transform	= 0x4000000,// ����״̬

	ERS_CALLHOLY    = 0x8000000,// �ٻ�ʥ��״̬

	ERS_End			= 0x40000000,
};

//--------------------------------------------------------------------------
// ��ɫ״̬��ʶ -- ��״̬�ı䣬ֻ��Ҫ֪ͨ��ұ���
//--------------------------------------------------------------------------
enum ERoleStateEx
{
	ERSE_BagPsdPass		= 0x1,	// ����������֤��ͨ��
	ERSE_BagPsdExist	= 0x2,	// �����������
};

//--------------------------------------------------------------------------
// ��ɫPK״̬
//--------------------------------------------------------------------------
enum ERolePKState
{
	ERPKS_Peace = 0,			// ��ƽ
	ERPKS_Warnning,				// ����
	ERPKS_Wicked,				// а��
	ERPKS_Wanted,				// ͨ��
	ERPKS_End,
};

//-------------------------------------------------------------------------------
// ��Ӫ�趨
//-------------------------------------------------------------------------------
enum ECampType
{
	ECamp_Null = 0,			// ����Ӫ [4/5/2012 zhangzhihua]
	ECamp_Justice,				// ���� [4/5/2012 zhangzhihua]
	ECamp_Evil,					// а�� [4/5/2012 zhangzhihua]
	ECamp_Neutrality,			// ���� [4/5/2012 zhangzhihua]

	ECamp_End,
};

//-------------------------------------------------------------------------------
// �����������ö��(�ô�ö��˳�򡢸�������,���ܻ�Ӱ�쵽���ݿ�װ����)
//-------------------------------------------------------------------------------
enum ERoleAttribute
{
	ERA_Null					=	-1,		//
	
	// һ������
	ERA_AttA_Start				=	0,
	ERA_Physique				=	0,		// ��ǰ����
	ERA_Strength				=	1,		// ��ǰ����	
	ERA_Pneuma					=	2,		// ��ǰ��Ԫ
	ERA_InnerForce				=	3,		// ��ǰ����	
	ERA_Technique				=	4,		// ��ǰ����������ʹ�á�	
	ERA_Agility					=	5,		// ��ǰ��������ʹ�á�
	ERA_AttA_End				=	5,

	// ��ҵĵ���
	ERA_AttPoint				=	6,		// ��ǰ���Ե���
	ERA_TalentPoint				=	7,		// ��ǰ���ʵ���

	// ��������
	ERA_AttB_Start				=	8,
	ERA_MaxHP					=	8,		// �������
	ERA_HP						=	9,		// ��ǰ����
	ERA_MaxMP					=	10,		// �������
	ERA_MP						=	11,		// ��ǰ����
	ERA_ExAttack				=	12,		// ������
	ERA_ExDefense				=	13,		// �������
	ERA_InAttack				=	14,		// ��������
	ERA_InDefense				=	15,		// ��������
	ERA_AttackTec				=	16,		// ��׼
	ERA_DefenseTec				=	17,		// ����
	ERA_HitRate					=	18,		// �����ʡ�����ʹ�á�
	ERA_Dodge					=	19,		// �����ʡ�����ʹ�á�
	ERA_Endurance				=	20,		// ��ǰ����������
	ERA_MaxEndurance			=	21,		// �����������������
	ERA_AttB_End				=	22,

	// ��������
	ERA_HPRegainRate			=	22,		// �����ָ��ٶ�
	ERA_MPRegainRate			=	23,		// �����ָ��ٶ�
	ERA_Rage					=	24,		// ŭ��
	ERA_Vitality				=	25,		// ����
	ERA_MaxVitality				=	26,		// ������
	ERA_VitalityRegainRate		=	27,		// �����ָ��ٶ�
	ERA_Speed_XZ				=	28,		// XZƽ���ٶ��ٶ�
	ERA_Speed_Y					=	29,		// Y�����ٶ�
	ERA_Speed_Swim				=	30,		// ��Ӿ��XZ�ٶ�
	ERA_Speed_Mount				=	31,		// ��˵�XZ�ٶ�

	ERA_EngraveAtt_Start		=	32,		// �Կ�Ӱ������
	ERA_WeaponDmgMin			=	32,		// ��С�����˺�
	ERA_WeaponDmgMax			=	33,		// ��������˺�
	ERA_WeaponSoul				=	34,		// �����˺�
	ERA_Armor					=	35,		// ���߻���
	ERA_EngraveAtt_End			=	35,		// �Կ�Ӱ������

	ERA_Derate_Start			=	36,
	ERA_Derate_Ordinary			=	36,		// ��ͨ��ͽ�֣��˺����⡾����ʹ�á�
	ERA_Derate_Bleeding			=	37,		// ��Ѫ�˺����⡾����ʹ�á�
	ERA_Derate_Brunt			=	38,		// �����˺�����
	ERA_Derate_Bang				=	39,		// ����˺�����
	ERA_Derate_Poison			=	40,		// �����˺����⡾����ʹ�á�
	ERA_Derate_Thinker			=	41,		// �����˺�����
	ERA_Derate_Injury			=	42,		// Ǭ���˺�����
	ERA_Derate_Stunt			=	43,		// �����˺����⡾����ʹ�á�
	ERA_Derate_ExAttack			=	44,		// �����˺�����
	ERA_Derate_InAttack			=	45,		// �����˺�����
	ERA_Derate_ALL				=	46,		// �����˺�����
	ERA_Derate_End				=	46,

	ERA_ExDamage				=	47,		// �����˺�
	ERA_ExDamage_Absorb			=	48,		// �����˺�����

	ERA_Resist_Bleeding			=	49,		// ���˿��ԡ������м���
	ERA_Resist_Weak				=	50,		// ˥�Ͽ��ԡ������м���
	ERA_Resist_Choas			=	51,		// ���ҿ��ԡ������м���
	ERA_Resist_Special			=	52,		// ���ٿ��ԡ������м���
	ERA_Regain_Addtion			=	53,		// ����

	ERA_Attack_MissRate			=	54,		// ����ʧ����
	ERA_CloseAttack_DodgeRate	=	55,		// ��ս����ʼӳ�
	ERA_RemoteAttack_DodgeRate	=	56,		// Զ�̶���ʼӳ�

	ERA_Crit_Rate				=	57,		// �����ӳ�
	ERA_Crit_Amount				=	58,		// �������ӳ�
	ERA_Block_Rate				=	59,		// ���ʼӳ�

	ERA_Inspiration				=	60,		// ���
	ERA_Lurk					=	61,		// ����
	ERA_Savvy					=	62,		// ����
	ERA_Morale					=	63,		// ʿ��������ʹ�á�
	ERA_Injury					=	64,		// ����
	ERA_Enmity_Degree			=	65,		// ��޶�
	ERA_Shape					=	66,		// ����
	ERA_Exp_Add_Rate			=	67,		// �����üӳ�		ԭ����/��Ǯ/���� * (1+�ӳ�/10000)
	ERA_Money_Add_Rate			=	68,		// ��Ǯ��üӳ�
	ERA_Loot_Add_Rate			=	69,		// ��Ʒ���伸�ʼӳ�

	// �������
	ERA_Fortune					=	70,		// ��Ե
	ERA_Appearance				=	71,		// ���ݡ�����ʹ�á�
	ERA_Rein					=	72,		// ͳ��������ʹ�á�
	ERA_Knowledge				=	73,		// ����
	ERA_Morality				=	74,		// ����
	ERA_Culture					=	75,		// ��Ϊ������ʹ�á�
	ERA_Hostility				=	76,		// ����	������ʹ�á�

	ERA_Spirit					=	77,		// ����������ʹ�á�
	ERA_Shili					=	78,		// ʵ��

	ERA_WuXun					=	79,		// ��ѫ
	ERA_WuJi					=	80,		// �似

	ERA_Weary					=	81,     // ƣ��ֵ
	
	ERA_Soar					=   82,		// ����״̬

	ERA_Toughness				=	83,		// ���ԡ�����ʹ�á�

	ERA_transform_Start			=	84,
	ERA_transform_Bang			=	84,		// ����˺�����
	ERA_transform_Brunt			=	85,		// �����˺�����
	ERA_transform_Injury		=	86,		// Ǭ���˺�����
	ERA_transform_Thinker		=	87,		// �����˺�����
	ERA_transform_ExAttack		=	88,		// �����˺�����
	ERA_transform_InAttack		=	89,		// �����˺�����
	ERA_transform_ALL			=	90,		// ȫ���˺�����
	ERA_transform_End			=	90,

	ERA_VipLevel				=   91,		// ��ɫ��Vip�ȼ�

	// ������������ž��е��˺����ԣ��ֳ��⣬�ֿ���������˺�
	ERA_EEI_START	= 92,
	ERA_EEI_Fire	= 92,	// ��Ԫ��
	ERA_EEI_Water	= 93,	// ˮԪ��
	ERA_EEI_Earth	= 94,	// ��Ԫ��
	ERA_EEI_Wind	= 95,	// ��Ԫ��
	ERA_EEI_Bright	= 96,	// ����Ԫ��
	ERA_EEI_Dark	= 97,	// �ڰ�Ԫ��
	ERA_EEI_End		= 97,
	// ����������еĿ��ԣ���Ȼ��Щ�ֳ��⣬�ֿ����п���
	ERA_EER_START	= 98,
	ERA_EER_Fire	= 98,	// ��Ԫ�ؿ���
	ERA_EER_Water	= 99,	// ˮԪ�ؿ���
	ERA_EER_Earth	= 100,	// ��Ԫ�ؿ���
	ERA_EER_Wind	= 101,	// ��Ԫ�ؿ���
	ERA_EER_Bright	= 102,	// ����Ԫ�ؿ���
	ERA_EER_Dark	= 103,	// �ڰ�Ԫ�ؿ���
	ERA_EER_End		= 103,

	ERA_InstanceWearyEffect		=	104,	// ���븱��ƣ��Ӱ��

	ERA_Target					= 105,			// Ŀ�꣬���ڡ�Ŀ���Ŀ�ꡱ

	// ��ϵͳ�������start
	ERA_God_Godhead   = 106, // ���
	ERA_God_Faith     = 107, // ����
	ERA_God_Condenced = 108, // ���۶�
	ERA_Mana_combustion_resistance                =			109,//����ȼ�յֿ�
	ERA_Toughness_god                             =			110,//����
	ERA_Toughness_strength                        =			111,//����ǿ��
	ERA_Control_effect_resistance                 =			112,//����Ч���ֿ�//����������ʹ�ã�����2011.9.16
	ERA_Contrel_effect_deepen                     =			113,//����Ч������
	ERA_Fixed_damage_aggrandizement               =			114,//�̶��˺�ǿ��
	ERA_Aging_effect_strengthening                =			115,//˥��Ч��ǿ��
	ERA_Slowing_effect_strengthening              =			116,//����Ч��ǿ��
	ERA_Physical_damage_rebound_num               =			117,//��������ֵ�˺�
	ERA_Physical_damage_rebound_ratio             =			118,//�����������˺� 
	ERA_Magic_damage_rebound_num                  =			119,//����������ֵ�˺�
	ERA_Magic_damage_rebound_ratio                =			120,//�������������˺�
	ERA_Physical_damage_rebound_immune            =			121,//�������˺�����
	ERA_Magic_damage_rebound_immune               =			122,//���������˺�����
	ERA_Mana_combustion                           =			123,//����ȼ��
	
	ERA_God_Day_Faith = 124,	//ÿ������

	//û�����ڼӵ�
	ERA_Nosee_attack = 125,		//���ӹ���
	ERA_Nosee_defend = 126,		//���ӷ���

	// ��ϵͳ�������end
	ERA_Soul		= 127, //���
	//�ʻ������������۵�����
	ERA_FlowerNum	= 128,
	ERA_EggsNum		= 129,

	// ������͸ [3/12/2012 zhangzhihua]
	ERA_EE_ATTR = 130,			// �����˺���͸Ԫ������ [3/12/2012 zhangzhihua]
	ERA_EEP_VALUE = 131,			// ������͸ֵ [3/12/2012 zhangzhihua]
	ERA_EER_ALL = 132,			// ����Ԫ�ؿ���ͨ��ֵ [3/20/2012 zhangzhihua]

	//ʥ��ֵ
	ERA_Holy = 133,
	//ʥħ ����
	ERA_HitRateAdd = 134,
	//Ԫ���˺�
	ERA_EEI_ALL = 135,

	ERA_End,
	// ��ҵ�������������Ͷ�����Ե㣬�Ѿ�Ͷ�ŵ����ʵ�û�зŵ�����������
};

//-----------------------------------------------------------------------------
// Ԫ���������ö������
//-----------------------------------------------------------------------------

enum EHolyAttType	//ʥ��������ǿ�������ԣ����ֵ�ļ�����Proto�������������
{
	ESAT_NULL				= -1,	// ��

	ESAT_Demage				= 0,	//ʥ���˺�
	ESAT_HolyDef			= 1,	//ʥ�����
	ESAT_Crit				= 2,	//����
	ESAT_CritRate			= 3,	//������
	ESAT_ExDmg				= 4,	//�˺�����
	ESAT_AttackTec			= 5,	//��׼0
	ESAT_NeglectToughness	= 6,	//����
	ESAT_LingNeng			= 7,	//����

	ESAT_End				= 8,	//---type ��Ŀ
};

const INT32	X_ERA_ATTA_NUM		= ERA_AttA_End - ERA_AttA_Start + 1;
const INT32	X_ESAT_ATTA_NUM		= ESAT_LingNeng - ESAT_Demage;

#define MTransERAAttA2Index(ERAAttA)		((ERAAttA) - ERA_AttA_Start)
#define MTransIndex2ERAATTA(Index)			((Index) + ERA_AttA_Start)

const INT32 X_ERA_AttB_NUM		= ERA_AttB_End - ERA_AttB_Start;
#define MTransERAAttB2Index(ERAAttB)		((ERAAttB) - ERA_AttB_Start)
#define MTransIndex2ERAATTB(Index)			((Index) + ERA_AttB_Start)

const INT32	X_ERA_DERATE_NUM	= ERA_Derate_End - ERA_Derate_Start + 1;
#define MTransERADerate2Index(ERADerate)	((ERADerate) - ERA_Derate_Start)
#define MTransIndex2ERADerate(Index)		((Index) + ERA_Derate_Start)

const INT32 X_ERA_TRANSFORM_NUM	= ERA_transform_End - ERA_transform_Start + 1;
#define MTransERATransform2Index(ERATransform)	(X_ERA_DERATE_NUM + (ERATransform) - ERA_transform_Start)
#define MTransIndex2ERATransform(Index)			((Index) - X_ERA_DERATE_NUM + ERA_transform_Start)

const INT32 X_ERA_BRAND_NUM = X_ERA_TRANSFORM_NUM + X_ERA_DERATE_NUM;

//--------------------------------------------------------------------------------------
// Զ����ҵ�����ö��
//--------------------------------------------------------------------------------------
enum ERemoteRoleAtt
{
	ERRA_Null		=	-1,		//

	ERRA_MaxHP		=	0,		// �������
	ERRA_HP			=	1,		// ��ǰ����
	ERRA_MaxMP		=	2,		// �������
	ERRA_MP			=	3,		// ��ǰ����
	ERRA_Rage		=	4,		// ŭ��
	ERRA_Speed_XZ	=	5,		// XZƽ���ٶ�
	ERRA_Speed_Y	=	6,		// Yƽ���ٶ�
	ERRA_Speed_Swim	=	7,		// ��Ӿ�ٶ�
	ERRA_Speed_Mount=	8,		// ����ٶ�
	ERRA_Shape		=	9,		// ����
	ERRA_Hostility	=	10,		// ����״̬
	// Jason 2010-5-27 v2.2.0
	ERRA_Soaring	=	11,		// ������־
	ERRA_VipLevel	=	12,		// VIP�ȼ�

	ERRA_End		=	13,
};

//-----------------------------------------------------------------------------------------
// Ŀ�����ͱ�ʾλ
//-----------------------------------------------------------------------------------------
enum ETargetFlag
{
	ETF_Self				=	0x00000001,	// ����
	ETF_Teammate			=	0x00000002,	// ����
	ETF_Guildmate			=	0x00000004,	// ͬ��
	ETF_Lover				=	0x00000008,	// ����
	ETF_Couple				=	0x00000010,	// ��ż
	ETF_Brother				=	0x00000020,	// ���
	ETF_Teacher				=	0x00000040,	// ʦͽ
	ETF_Player				=	0x00000080,	// ���

	ETF_Pet					=	0x00000100,	// ����
	ETF_NPC					=	0x00000200,	// NPC
	ETF_NormalMonster		=	0x00000400,	// ��ͨ����
	ETF_Boss				=	0x00000800,	// Boss
	ETF_Nest				=	0x00001000,	// ��Ѩ
	ETF_NatuRes				=	0x00002000,	// ��Ȼ��Դ
	ETF_ManRes				=	0x00004000,	// ������Դ
	ETF_Door				=	0x00008000,	// ����
	ETF_Building			=	0x00010000,	// ����
	ETF_NormalGameObject	=	0x00020000,	// ��ͨ�ɽ�������
	ETF_InvesGameObject		=	0x00040000,	// �ɵ������
	ETF_QuestTalkGameObject	=	0x00080000,	// ����Ի�����
	ETF_Remnant				=   0x00100000,	// ��
	ETF_FairySoul			=   0x00200000,	// ��������
	ETF_FairyHeritage		=	0x00400000,	// �����Ų�������
};

//-----------------------------------------------------------------------------------------
// ״̬��ʾλ
//-----------------------------------------------------------------------------------------
enum EStateFlag
{
	ESF_Dead			=	0x00000001,	// ����
	ESF_NoDead			=	0x00000002,	// ������
	ESF_Dizzy			=	0x00000004,	// ѣ��
	ESF_NoDizzy			=	0x00000008,	// ��ѣ��
	ESF_Spor			=	0x00000010,	// ��˯
	ESF_NoSpor			=	0x00000020,	// �ǻ�˯
	ESF_Tie				=	0x00000040,	// ����
	ESF_NoTie			=	0x00000080,	// �Ƕ���
	ESF_Invincible		=	0x00000100,	// �޵�
	ESF_NoInvincible	=	0x00000200,	// ���޵�
	ESF_NoSkill			=	0x00000400,	// �⼼
	ESF_NoNoSkill		=	0x00000800, // �Ƿ⼼
	ESF_DisArm			=	0x00001000,	// ��е
	ESF_NoDisArm		=	0x00002000,	// �ǽ�е
	ESF_Lurk			=	0x00004000,	// ����
	ESF_NoLurk			=	0x00008000,	// ������
	ESF_Mount			=	0x00010000,	// ���
	ESF_NoMount			=	0x00020000, // �����
	ESF_Prison			=	0x00040000,	// ����
	ESF_NoPrison		=	0x00080000,	// ������
	ESF_Mount2			=	0x00100000,	// ���2
	ESF_NoMount2		=	0x00200000, // �����2
	ESF_Commerce		=	0x00400000,	// ������
	ESF_NoCommerce		=	0x00800000,	// ��������

	ESF_Transform		=	0x01000000, // ����
	ESF_NoTransform	=	0x02000000,	// �Ǳ���
};

//-----------------------------------------------------------------------------------------
// ��������������Ե�Ĭ��ֵ�������Сֵ
//-----------------------------------------------------------------------------------------
struct tagAttDefMinMax
{
	INT		nDefRole;			// �����ʼ��Ĭ��ֵ
	INT		nDefCreature;		// �����ʼ��Ĭ��ֵ
	INT		nMin;				// ������Сֵ
	INT		nMax;				// �������ֵ
};

////---------------------------------------------------------------------------------------
//// �ͻ��˱������ͬ��������Ϣ�ṹ��δ��ɣ�
////---------------------------------------------------------------------------------------
//struct tagRoleData
//{
//	DWORD			dwID;						// ���ID
//	DWORD			dwRoleNameCrc;				// ����CRC
//	TCHAR			szRoleName[X_SHORT_NAME];	// ����
//
//	DWORD			dwMapID;					// ���ڵĵ�ͼ��ID
//	FLOAT			fPos[3];					// λ��
//	FLOAT			fFaceTo[3];					// ����
//
//	tagAvatarAtt	Avatar;						// ���
//	tagAvatarEquip	AvatarEquip;				// װ�����
//	tagDisplaySet	DisplaySet;					// �����ʾ����
//	INT				nAtt[ERA_End];				// ��ǰ��������
//
//	INT				nAttPointAdd[X_ERA_ATTA_NUM];// ���Ͷ�ŵ�����һ�������е�ֵ
//
//	EClassType		eClass;						// ְҵ
//	EClassTypeEx	eClassEx;					// ��չְҵ
//
//	INT16			n16MaxBagSize;				// ������С
//	INT16			n16MaxWareSize;				// �˺Ųֿ��С
//
//	INT				nSkillNum;					// ���ܸ���
//	INT				nBuffNum;					// ״̬����
//	INT				nTitleNum;					// �ƺŸ���
//	INT				nItemNum;					// ��Ʒ����
//	INT				nEquipNum;					// װ������
//	INT				nFriendNum;					// ���Ѹ���
//	INT				nEnemyNum;					// ��и���
//	INT				nQuestNum;					// �����б����
//	INT				nQuestDoneNum;				// ����ɵ��������
//
//	/*********************************************************************************************************
//	 *�����б�״̬�б��ƺ��б���Ʒ�б�װ���б������б�����б������б�����������б�
//	 *���밴˳�򿽱�
//	 *********************************************************************************************************/
//	//�����б�
//	DWORD			dwIncompleteQuestID[QUEST_MAX_COUNT];			// �ڵ�ǰ�������δ��ɵ����� 30����
//	DWORD			dwIncompleteQuestTakeTime[QUEST_MAX_COUNT];		// �ڵ�ǰ������ϵ�ǰ����Ľ�ȡʱ�� 30����
//	//�����б�
//	
//
//	BYTE			byData[1];
//};

//------------------------------------------------------------------------------------------
// ��ѡ�˽���Ҫ������������Ϣ
//------------------------------------------------------------------------------------------
struct tagSimRole								// size = 64
{
	tagAvatarAtt	Avatar;						// size = 16
	tagAvatarEquip	AvatarEquip;				// size = (2 * 9 + 1) * 4
	TCHAR			szRoleName[X_SHORT_NAME];

	DWORD			dwRoleID;
	DWORD			dwMapID;
	BYTE			byLevel;
	tagDisplaySet	sDisplaySet;
	BOOL			bOldPlayerBack;
	DWORD			timeLogout; // ���һ�εǳ�ʱ��
	UINT16			dwBackDays;
	BYTE			byReserved[2];
};

//------------------------------------------------------------------------------------------
// �Ƿ��Զ����ҹ��������Ϣ -- ע�����ݿ���¼��32λ
//------------------------------------------------------------------------------------------
struct tagRemoteOpenSet
{
	DWORD	bTitle		: 1,	// �ƺ�
			bVCard		: 1,	// ����
			bRoleAtt	: 1,	// ��ɫ����
			bDummy		: 29;	// ռλ��

	tagRemoteOpenSet() { ZeroMemory(this, sizeof(*this)); }
	
	tagRemoteOpenSet& operator = (DWORD dw)
	{
		return *this = *(tagRemoteOpenSet*)&dw;
	}

	operator DWORD()
	{
		return *(DWORD*)this;
	}
};


//------------------------------------------------------------------------------------------
// �����Ϣ�Ĺ�������
//------------------------------------------------------------------------------------------
enum EPlayerPersonlInfoOpenType
{
	EPIS_Private,  // ��ȫ����
	EPIS_Friend,   // ���Ի�Ϊ���ѵ���ҹ���
	EPIS_Public,   // ��ȫ����
	EPIS_End,
};

//------------------------------------------------------------------------------------------
// ��ҵĸ������� 
// ע�� 
// ÿ�������ֶ�Ҫ��1���ֽڶ��� �������1�ֽ�������λ
//------------------------------------------------------------------------------------------
struct tagDWPlayerPersonalSet
{
	BYTE	bAcceptTeamInvite		: 1,	// �Ƿ�����������
			bAcceptFactionInvite	: 1,	// �Ƿ���ܰ�������
			bAllowFriendAdd			: 1,	// �Ƿ���������������Լ�Ϊ����
			bAcceptTradeRequest		: 1,	// �Ƿ���ܽ�������
			eInfoOpenType           : 4;    // ��Ϣ�Ĺ�������

	tagDWPlayerPersonalSet() 
	{ 
		bAcceptTeamInvite		=	true;
		bAcceptFactionInvite	=	true;
		bAllowFriendAdd			=	true;
		bAcceptTradeRequest		=	true;
		eInfoOpenType			=	EPIS_Public;
	}	

	tagDWPlayerPersonalSet& operator = (const tagDWPlayerPersonalSet &set)
	{
		bAcceptTeamInvite		= set.bAcceptTeamInvite;
		bAcceptFactionInvite	= set.bAcceptFactionInvite;
		bAllowFriendAdd			= set.bAllowFriendAdd;
		bAcceptTradeRequest		= set.bAcceptTradeRequest;
		eInfoOpenType			= set.eInfoOpenType;

		return *this;
	}

	tagDWPlayerPersonalSet(const tagDWPlayerPersonalSet& set)
	{
		*this = set;
	}

};

//-------------------------------------------------------------------------------------
// ʵ��ֵ
//-------------------------------------------------------------------------------------
struct tagStrength
{
	INT nStrength;					// ��ʵ��ֵ������ʵ������

	// ��ʵ��ֵ�������
	INT nBaseStrength;				// ����ʵ��ֵ
	INT	nAdvanceStrength;			// ����ʵ��ֵ
	INT nConsumptiveStrength;		// ������ʵ��ֵ

	// ����ʵ��ֵ��ظ���
	INT	nLevel;						// �ȼ�
	INT nEquipValue;				// װ��ʵ��
	INT	nPet;						// ����ʵ��
	INT nFriendship;				// ���ѣ���Һ����б����Ѻöȳ���300�ĺ�������*5
	INT	nQuest;						// ������������ɵķ��ظ�����������������/5
	INT nProduce;					// �������ܣ������ѧ�����������ܵȼ�֮��*50
	INT nFame;						// ����
	INT nTitle;						// �ƺ�
	INT nFairy;                     // ����
	// �ȼ�

	// ����ʵ��ֵ��ظ���
	INT nActivity;					// ����
	INT nRanQuest;					// ÿ�ա��������
	INT nInstance;					// ����
	INT nKill;						// ��ɱ�ǰ������

	tagStrength()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//------------------------------------------------------------------------------------------
// ö����Ҿ�������
//------------------------------------------------------------------------------------------
enum EAthleticsProperty
{
	EAP_ValiantMerit = 0,  //��ѫ
	EAP_ValiantSkill = 1,  //�似
	EAP_End,
};

//------------------------------------------------------------------------------------------
// ö�����ÿһ��ѫ�ȼ���Ӧ����ѫֵ
//------------------------------------------------------------------------------------------
enum EMAXVALIANT_PERLEVEL
{
	EMVPL_Level1_Max_Valiant =      199,
	EMVPL_Level2_Max_Valiant =      399,
	EMVPL_Level3_Max_Valiant =      599,
	EMVPL_Level4_Max_Valiant =      799,
	EMVPL_Level5_Max_Valiant =      999,
	EMVPL_Level6_Max_Valiant =      1199,
	EMVPL_Level7_Max_Valiant =      1399,
    EMVPL_Level8_Max_Valiant =      1599,
    EMVPL_Level9_Max_Valiant =      1799,
	EMVPL_Level10_Max_Valiant=      9999,
	EMVPL_End,
};

//-------------------------------------------------------------------------------------
// ���������ؽ�ɫ��Ϥ
//-------------------------------------------------------------------------------------

const INT16 MAX_ATHLETICS_PROPERTY = 9999;

//-------------------------------------------------------------------------------------
// ��ɫ��VIP״̬����ö��
//-------------------------------------------------------------------------------------
enum EVipLevel
{
	EVL_None		= 0, // ����vip
	EVL_Silver		= 1, // ���꼶vip
	EVL_Gold			= 2, // ���꼶vip
	EVL_Platinum	= 3, // �׽�vip

	EVL_Max,
};

#define DEFAULT_MAX_VIP_DAYS 30
#define MAX_DAYS_SECOND(MaxDays) ((MaxDays)*86400) // MaxDays���ж�����24*60*60
#define SILVER_VIP_BUFF			9003401 // ���꼶VIP
#define GOLDEN_VIP_BUFF		9003402 // ���꼶VIP
#define PLATINUM_VIP_BUFF	9003403 // �׽�VIP

#define MaxExpPillularBaseTimes 20
#define MaxExpPillularUseTimes(VipLvl) ((VipLvl+1)*MaxExpPillularBaseTimes)

#define GetGodHeadLimit( level,maxvalue )	do{	\
		if(level < 20)	\
		{	\
			maxvalue = 0;	\
			break;			\
		}	\
		else if(level < 30)	\
		{	\
			maxvalue = 1;	\
			break;			\
		}	\
		else if(level < 40)	\
		{	\
			maxvalue = 2;	\
			break;			\
		}	\
		else if(level < 50)	\
		{	\
			maxvalue = 3;	\
			break;			\
		}	\
		else if(level < 60)	\
		{	\
			maxvalue = 5;	\
			break;			\
		}	\
		else if(level < 70)	\
		{	\
			maxvalue = 7;	\
			break;			\
		}	\
		else if(level < 80)	\
		{	\
			maxvalue = 9;	\
			break;			\
		}	\
		else if(level < 90)	\
		{	\
			maxvalue = 15;	\
			break;			\
		}	\
		else if(level <= 100)	\
		{	\
			maxvalue = 20;	\
			break;			\
		}	\
	}while(0)

enum EAccountSpec
{
	EAS_Null = -1,
	EAS_Normal = 0,	// ��ͨ�˺�
	EAS_FromEM = 1, // ���Զ�ħ����
	EAS_FromEMRewared = 2, // ���Զ�ħ������ȡ������
};

enum ERoleAttrSync
{
	ERoleAttrWarGold = 0,					// ս�����
	ERoleAttrMonsterKillCount,				// ��ɱ�������
	ERoleAttrBossKillCount,					// ��ɱboss����(����)
	ERoleAttrRoleKillCount,					// ��������
	ERoleAttrRareBossKillCount,				// ��ɱboss����(ϡ��)
	//ERoleAttrRoleDeadCount,				// ��ɱ�з���������
	//ERoleAttrNeutralMonsterKillCount,		// ���������ɱ����

	ERoleAttrEnd,
};

enum EWarRoleResult
{
	EWRR_NULL		= 0,
	EWRR_Lose,
	EWRR_Win,

	EWRR_End,
};

const DWORD REFRESH_BAOXIANG_ITEMID = 3110037;

#pragma pack(pop)

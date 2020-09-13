//-----------------------------------------------------------------------------
//!\file msg_compose.h
//!
//!\date 2008-09-25
//!
//!\brief �ͻ����������֮����ںϳɣ��ֽ⣬ǿ������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "compose_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum 
{
	//���д�����
	E_Compose_NPC_Not_Exist = 1, 				//NPC������
	E_Compose_Formula_Not_Exist = 2, 			//�䷽������
	E_Compose_Stuff_Not_Enough = 3, 			//������������
	E_Compose_Stuff_Formula_Not_Match = 4, 		//���Ϻ��䷽��ƥ��
	E_Compose_NPC_Distance = 5,					//NPC����̫Զ
	
	//�ϳ�ǿ�����ص����ֽ��,�Ǹ��ݺϳɹ�ʽ���
	E_Compose_Consolidate_Success = 11, 		//�ɹ�
	E_Compose_Consolidate_Lose = 12, 			//ʧ��
	E_Compose_Consolidate_Perfect = 13, 		//����

	
	//�ϳɴ�����
	E_Compose_Stuff_Not_Exist = 21, 			//�ϳɲ��ϲ�����
	E_Compose_Type_Not_Exist = 22,				//�ϳ����Ͳ�����
	E_Compose_Skill_Not_Exist = 23,				//���û�кϳɼ���
	E_Compose_FormulaNotMatch = 24,				//�ϳ��䷽��ƥ��
	E_Compose_NPCCanNotCompose = 25,			//���Ǻϳ�NPC
	E_Compose_Vitality_Inadequate = 26,			//��һ���ֵ����
	E_Compose_NotEnough_Money = 27,				//��ҽ�Ǯ����
	E_Compose_Bag_Full = 28,					//��������
	E_Compose_EquipUnidentify = 29,				// װ��δ����
	
	//�ֽ������
	E_Decomposition_Item_Not_Exist = 31, 		//�ֽ���Ʒ������

	//ǿ��������
	E_Consolidate_Equip_Not_Exist = 41, 		//ǿ��װ��������
	E_Consolidate_Type_Not_Exist = 42,			//ǿ�����Ͳ�����
	E_Consolidate_NPCCanNotPosy	= 43,			//��������NPC
	E_Consolidate_NotEquipment = 44,			//ǿ������Ʒ����װ��
	E_Consolidate_BeyondPosyTime,
	E_Consolidate_FormulaNotMatch,				//ǿ���䷽��ƥ��
	E_Consolidate_NotEnough_Money,				//��ҽ�Ǯ����
	E_Consolidate_NotEnough_Stuff,				//��Ҳ��ϲ���
	E_Consolidate_EquipCanNotPosy,				//�����Բ��ܱ�����
	E_Consolidate_ValConsume_Inadequate,		//װ��Ǳ��ֵ����

	E_Compose_NPCCanNotEngrave,					//�����Կ�NPC
	E_Consolidate_EquipCanNotEngrave,			//�����Բ��ܱ��Կ�
	E_Consolidate_BeyondEngraveTime,			//�����Կ̴���

	E_Consolidate_Gem_Not_Exit,					//��ʯ������
	E_Consolidate_Gem_Locked,					//��ʯ������
	E_Consolidate_Gem_Full,						//װ����������ʯ
	E_Consolidate_EquipCanNotInlay,				//�ñ�ʯ���ܱ���Ƕ
	E_Consolidate_Gem_Not_Hole,					//װ���޿�

	E_Consolidate_Brand_Not_Exit,				//ӡ�ǲ�����
	E_Consolidate_Brand_TopLevel,				//ӡ�ǵȼ��ﵽ����
	E_Consolidate_EquipCanNotBrand,				//��װ�����ܱ�ӡ��
	E_Consolidate_GuiYuanStone_Not_Exist,		//��Ԫʯ������
	E_Consolidate_GuiYuanEquip_Not_Exist,		//Ҫִ�й�Ԫ������װ��������

	E_Consolidate_LoongSoul_Not_Exit,			//����ʯ������
	E_Consolidate_LoongSoul_TopLevel,			//����ȼ��ﵽ����
	E_Consolidate_EquipCanNotLoongSoul,			//��װ�����ܱ�����

	E_Consolidate_NPCCanNotQuench,				//���Ǵ��NPC
	E_Consolidate_WXValueInadequate,			//ԭ����ֵ����
	E_Consolidate_False,						//���ʧ��

	E_Compose_Quality_Not_Match,				//�㻯ֻ���ڻ�ɫ������װ��
	E_Compose_Not_Fashion,						//��߼�ʱװ���ɵ㻯��ֽ�
	E_Compose_Equip_Lock,						//��������װ�����ɵ㻯��ֽ�
	E_Compose_Equip_Time_Limit,					//��ʱ�����Ƶ�װ�����ɵ㻯��ֽ�
	E_Compose_Equip_Not_identify,				//δ������װ�����ɵ㻯��ֽ�
	E_Compose_Equip_Level_Inadequate,			//װ���ȼ�����
	E_Compose_Equip_Quality_Inadequate,			//װ��Ʒ�ʲ���
	E_Compose_Equip_Type_Inadequate,			//װ�����Ͳ������䷽Ҫ��

	E_Consolidate_Chisel_Not_Exit,				//��ʯ������
	E_Consolidate_Equip_CanNot_Chisel,			//װ�����ܱ�����
	E_Consolidate_Equip_Hole_Full,				//��Ƕ���������������ɽ��п���
	E_Consolidate_Chisel_Not_Match,				//��ʯ�ȼ�С���������װ���ȼ������ɽ��п���
	E_Consolidate_NotIdentified,				//δ����װ�����ɿ���

	E_Raise_PotVal_Not_Needing,					// �Ѿ����ˣ�����Ҫ������Ǳ��ֵ
	E_Raise_PotVal_Times_Zero,					// ��������Ϊ��

	// Jason v1.3.1 2009-12-21 ��ʯ���errorid
	E_Gem_NPCCanNotRemoveGem,					//���Ǳ�ʯ���NPC

	E_Consolidate_CanNotSpin,					//�Ǿ�����Ʒ

	E_Consolidate_Spin_TopLevel,				//���Ĵﵽ��ߵȼ�

	E_Extraction_EquipInvalid,					//��ȡ��װ��������
	E_Extraction_NotEquip,						//Ҫ��ȡ�Ĳ���װ��
	E_Extraction_NoFormula,						//û�ж�Ӧ����ȡ�䷽
	E_Extraction_QualityLimit,					//Ʒ������
	E_Extraction_VitalityLimit,					//��������
	E_Extraction_MoneyLimit,					//��Ǯ����
	E_Extraction_BagFull,							//�����ռ䲻��

	// Jason 2010-4-15 v2.0.0 �������
	E_Fabao_Practice_NoYuLingshi,				// ����ʯ������
	E_Fabao_Practice_NotYuLingshi,				// ԭ�ϲ�������ʯ
	E_Fabao_FabaoNotExist,				// ����������
	E_Fabao_Practice_NotFabao,					// Ŀ��װ�����Ƿ���
	E_Fabao_Practice_LingshiNumCrossBorder,		// ����ʯ�����������ɶѵ���������
	E_Fabao_Practice_FabaoStageFull,			// �����Ѿ���������
	E_Fabao_Practice_NoResource,				// û���ҵ���ص���Դ������Ƚױ�xml
	E_Fabao_Level_Limitted,						// �����������ܵ����˵ȼ�����
	E_Fabao_LingzhuWanted,						// ��Ҫ�ں�����󣬲��ܽ�һ����������

	// 2010-4-16 v2.0.0
	E_Fabao_Inlay_NotLingzhu,					// ��������
	E_Fabao_Inlay_SlotLTLingzhu,				// ���С���������
	E_Fabao_Inlay_Mismatch,						// �����뷨����ƥ�䣨�������ͣ������ȣ�
	E_Fabao_UnkownError,						// ����δ֪����

	E_Fabao_Not_Exist,							// ����������
	E_Fabao_TypeMismatch,						// �Ƿ���
	E_Fabao_LevelLimmit,						// �����ȼ�����
	E_Fabao_NotPermitted,						// ����������
	E_Fabao_CannotBeAider,						// ������Ϊ��������

	E_UpgradeMaxPot_NoNeed,						// ��װ������Ҫʹ�ô˵���
	E_UpgradeMaxPot_EquipPotTooLow,				// ��װ�������Ǳ��ֵ̫�ͣ��޷�ʹ�ô˵���

	E_SoaringEquipment_NPC_Invalid,				// npc�Ƿ�

	E_EquipPotval_ItemTypeInvalid,				// ����Ǳ��ֵ���޵ĵ������Ͳ���
	E_EquipPotval_EquipPosInvalid,				// ��Ʒ������������װ����Ǳ��ֵ
	E_SoaringStone_NotExist,					// ��ʯ������
	E_SoaringEquipment_NotEquip,			// ��װ����������
	E_SoaringEquipment_StuffMustBeEquip,	// �������ϱ�����װ��
	E_SoaringEquipment_StuffQltyLower,		// ����װ��Ʒ������
	E_SoaringEquipment_NotAccessories,		// ����������
	E_SoaringEquipment_LevelLower,			// ������װ�����ȼ�����

	E_CastingEquipment_NPC_Invalid,			// ������npc
	E_CastingEquipment_NotEquip,			// ��װ�������ɲ�������
	E_CastingEquipment_MustBeSoaringEquip,	// �����Ƿ���װ��
	E_CastingEquipment_CastingTimesNotMatch,	// ��������idû��ƥ��װ������������
	E_CastingEquipment_StuffTypeDismatch,	// ������װ�����Ͳ�һ��
	E_CastingEquipment_StuffTypeDismatchProto,	// ����������ԭ�����Ͳ�һ��
	E_CastingEquipment_StuffNotEnough,		// ������������������
	E_CastingEquipment_StuffNotExist,		// ���ϲ�����


	E_Compost_Equip_PotvalNotEnough,		//װ��Ǳ��ֵ����
	E_SoaringEquipment_NotIdentify,			// δ����װ����������������
	E_SoaringEquipment_FabaoHasBeenProhibited,	// ������������������
	E_SoaringEquipment_Fail,					// ����ʧ�ܣ���������ʧ��
	E_CastingEquipment_Fail,					// ����ʧ�ܣ���������ʧ��
	E_SoaringEquipment_FashionHasBeenProhibited,	// ʱװ���ܱ���������������
	E_SoaringEquipment_TimelimittedEquipHasBeenProhibited,	// ʱ��װ����������������

	// Jason 2010-6-19 v1.0.0 ǿ�����������
	E_Consolidate_Stone_Not_Exist ,				// û��ǿ��ʯ
	E_Consolidate_Stone_LevelLow,				// ǿ��ʯ�ȼ���
	E_Consolidate_FormulaNotExist,				// ǿ���䷽������
	E_Consolidate_LevelLimitExceeded,			// ����ǿ����ߵȼ�
	E_Consolodate_F_Fail,							// ǿ��ʧ��
	E_ADSoul_Equip_ConsolidateLevelZero,		// װ��ǿ���ȼ�Ϊ0����������
	E_ADSoul_Equip_XihunStoneNotExist,			// ����ʯ������
	E_ADSoul_Equip_NotXihunStone,				// ������ʯ
	E_ADSoul_Equip_XihunLevelLow,				// ����ʯ�ȼ���
	E_ADSoul_Equip_SpecEquipCannotBeDetachSoul,	// ����װ������ʱװ�������ȣ������Ա�����
	E_ADSoul_Equip_CannotFindFuhunStone	,			// �޷��ҵ�����ʯ
	E_ADSoul_Equip_FuhunStoneCreatorFail,			// ����ʯ����ʧ��

	E_ADSoul_Equip_FuhunStoneNotExist,				// ����ʯ������
	E_ADSoul_Equip_NotFuhunStone,					// �Ǹ���ʯ
	E_ADSoul_Equip_CannotFindFuhunInfo,				// �Ҳ�������ʯ��ص�ǿ���ȼ���Ϣ
	E_ADSoul_Equip_FuhunLevelLow,					// ����ʯ�ȼ�û��װ��ǿ���ȼ���

	E_Fabao_RaiseIntelligence_Success,				// ������ҩ���������ʳɹ�
	E_Fabao_RaiseIntelligence_Failse,				// ��ʾʧ�ܣ��������ʼ���
	E_Fabao_RaiseIntelligence_NoChange,				// ����û�иı�
	E_Fabao_RaiseIntelligence_AlreadyFull,			// ���������������������ˡ�
	E_Fabao_RaiseIntelligence_FairyPowderNotExist,			// ����֮�۲�����
	E_Fabao_RaiseIntelligence_FairyPowderNotFit,
	E_Fabao_Spec98ItemNotExist,	// ����Ǳ����ҩ������
	E_Fabao_Spec98Item_Success, // �ɹ���������������������
	E_Fabao_Spec98Item_AlreadyUse,	// ���������Ѿ�������Ǳ���Ҳ����ĵ���
	E_Fabao_Spec98Item_UseFailed, // ����������������ʧ��
	E_Fabao_Spec99ItemNotExist, // �ȹ���������ҩ��������
	E_Fabao_ItemNotFit2FairyIntellgence,	// ������ǰ�������޷�ʹ�ø�ҩ��
	E_Fabao_ItemUseFailed,	// ��Ʒʹ��ʧ��
	E_Fabao_RaiseIntelligence_NotInBag,				// ����û���ڱ����������Ҫ�ڱ�����������������װ����
	E_Fabao_SecondeFabaoLevelLower,					// ����������ԭ�������Ƚײ���20��
	E_ConsolidateInlay_EquipLevelLower,				// װ���ȼ�����

	E_ADSoul_Equip_FuhunLevelToHight,				// ����ʯ�ȼ�̫�ߣ�װ��δ������20����������30�����ܻᱨ�������
	E_Consolidate_Stone_NotMatchStuff,				// װ��ǿ��ʯ���Ͳ�ƥ����һ�ȼ�ǿ����������
	E_ADSoul_Equip_GodStrength_NotEnough,			// Ҫ�����װ�������ȼ����㣬�����Ը���

	// ����
	E_GodStrengthen_EquipQltyLower,					// ��װ��Ʒ������
	E_GodStrengthen_EquipCannotStrengAgain,			// ��װ������Ҫ�������ˣ���10����
	E_GodStrengthen_AssitEquip_QltyLower,			// B��װ��Ʒ������
	E_GodStrengthen_AssitEquip_LevelLower,			// B��װ���ȼ�����
	E_GodStrengthen_AssitEquip_TimelimForbided,	// ʱ����װ��������Ϊ����װ��
	E_GodStrengthen_EquipPosNotMatch,				// װ��λ�ò�ƥ��
	E_GodStrengthen_EquipCannotLevelupInOneDay,	// װ��������һ���ڣ�������������

	// ǿ������errorcode
	E_Consolidate_LuckySign_NeedGodStrengSign,				// װ��ǿ����Ҫ�������˷�
	E_Consolidate_LuckySign_LevelLimit,		// ��������ȼ�����
	E_Consolidate_LuckySign_BaodiLimit,	// �����������Ҫ���׷�

	E_Fabao_EleInjuryChange_NPCTypeErr		,		// ����Ԫ���˺�����ת��npc���Ͳ���
	E_Fabao_EleInjuryChange_FairyEquipErr	,		// �������������������������
	E_Fabao_EleInjuryChange_StuffErr,				// �������Ͳ���
	E_Fabao_EleInjuryChange_LevelLimit,				// �����ȼ�̫�ͣ�����60��
	E_Fabao_EleInjuryChange_NotEnoughMoney,			// Ǯ����

	E_Fabao_StuntBook_SameID,						// ����ѧϰ��ͬ�������ؼ�
	E_Fabao_StuntBook_LevelLimit,						// �������ʻ�ȼ����㣬����ѧϰ

	E_Fabao_StuntConfirm_SameID,					// ����ѧϰ��ͬ�������ؼ�

	E_RockStone_NotRockStone,						// ������ʯ
	E_RockStone_NotEquipment,						// ����ϴ��װ������
	E_RockStone_LevelLimit,							// װ���ȼ�����
	E_RockStone_TypeMissMatch,						// ����ϴ����װ��
	E_RockStone_WhiteQualityLimit,				// ����ϴ��ɫƷ��װ��

	E_GodStrengthen_BaodiFuLevelLimit,				// ���׷������ػ������ȼ�̫��

	E_OnlyCanStrengthenByScript,				// ֻ�ܿ��ű��Ը�װ������ǿ��	

	E_SoulCrystal_Grow_ItemInvalid,	// �꾧�Ƿ�
	E_SoulCrystal_Grow_IMError, // �����Ƿ�
	E_SoulCrystal_Grow_MaxLvl, // �꾧�Ѵ����ȼ�
	E_SoulCrystal_Grow_SoulLimit, // ��겻��
	E_SoulCrystal_Grow_MoneyLimit, // ��Ǯ����

	E_SoulCrystal_Renew_ItemInvalid, // ����Ļ꾧�Ƿ�
	E_SoulCrystal_Renew_IMLimit, // ȱ�ٻ꾧�����
	E_SoulCrystal_Renew_Limit, // ����������������
	E_FairyAddPromoteTimes_ToMax,// �Ѵﵽ�����������
	E_FairyAddPromoteTimes_LackMoney,//��Ǯ����

	E_FairyFusion_LowQuality,// ����Ʒ�ʲ��㣨Ҫ���ɫ���ϣ� [3/22/2012 zhangzhihua]
	E_FairyFusion_LowNativeIntelligence,// �������ʲ���60 [3/22/2012 zhangzhihua]
	E_FairyFusion_LowLevel,// �����ȼ�����40 [3/22/2012 zhangzhihua]
	E_FairyFusion_StatusError,// �������������������������׶� [3/22/2012 zhangzhihua]
	E_FairyFusion_FairyError,// �����������޷��ں� [3/22/2012 zhangzhihua]
	E_FairyFusion_CannotUseItem,// ����ʹ�õ��� [3/23/2012 zhangzhihua]
	E_FairyFusion_Faild,

	E_SLEatFairy_EquipInValid,		// ʥ�������������ͻ����ṩ��װ�����Ͳ�����
	E_SLEatFairy_GetProtoFailed,    // ʥ������������ͨ�������÷�û���ҵ���Ӧ��������������
	E_SLEatFairy_CountNotEnough,	// ʥ����������������ʣ���������
	E_SLEatFairy_CountExceed,		// ʥ�������������Ѵﵽ�ܴ�������
	E_SLEatFairy_lowLevel,			// ʥ��������������������С��40��������
	
	E_CallShengLing_HolyNotEnough,	// �������ϵ�ʥ��ֵ�����ٻ�ʥ��
	E_CallShengLing_EquipInValid,	// ʥ�鲻�����ٻ�����
	E_CallShengLing_ForMountState,	// ���״̬�²����ٻ�ʥ��
	E_CallShengLing_UseSkillState,	// ���ܶ���ʱ�����ٻ�ʥ��

	E_EquipHoly_EquipInValid,		// ��ʥ�鴩װ�����ͻ����ṩ��װ�����Ͳ�����
	E_EquipHoly_NotEmpty,			// ��ʥ�鴩װ����ʥ����װ������
	E_EquipHoly_HolyLowLevel,		// ��ʥ�鴩װ���������ʥ�����ɵȼ�Ҫ�����
	E_EquipHoly_NotEnough,			// ��ʥ�鴩װ����ʥ��ʣ�����ܲ���
	E_EquipHoly_ExceedLapCount,		// ��ʥ�鴩װ���������˸�����ʥ�ƿ���ʥ�����ظ�װ��������

	E_EquipHoly_UnEquipInValid,		// ��ʥ��жװ�����ͻ����ṩ��װ�����Ͳ�����
	E_EquipHoly_QualityNotEqual,	// ��ʥ��жװ����ϴӡ����ʥ�� Ʒ�������
	E_EquipHoly_SilverNotEnough,	// ��ʥ��жװ���������Ҳ���
	E_EquipHoly_SpaceNotEnough,		// ��ʥ��жװ���������ռ䲻��

	E_EnhanceHolyEquip_SilverNotEnough,	// ��ʥ�Ƴ��ܣ������Ҳ���
	E_EnhanceHolyEquip_CountExceed,		// ��ʥ�Ƴ��ܣ����ܴ����Ѿ��ﵽ���
	E_EnhanceHolyEquip_ProtoNotFound,	// ��ʥ�Ƴ��ܣ��Ҳ���ǿ�����߶�Ӧ������
	E_EnhanceHolyEquip_MinHolyExceed,	// ��ʥ�Ƴ��ܣ����ĵ������Ѿ��ﵽ��С����ֵ
	E_EnhanceHolyEquip_ItemNotFit,		// ��ʥ�Ƴ��ܣ����ܵ��߲�����

	E_EnhanceHolyEquip_MaxHolyExceed,		// ��ʥ�ƻ������Գ��ܣ�ʥ����������Ĳ���������10
	E_EnhanceHolyEquip_ProtoNotExist,		// ��ʥ�ƻ������Գ��ܣ�ʥ�Ʊ����߱�Ҫƫ�������

	E_StarEnhanceCondensate_MaxLevel,				//Ԫ��װ�������Ѵﵽ���ȼ�
	E_StarEnhanceCondensate_NoExist,				//����ǿ��������
	E_StarEnhanceCondensate_EquipCanNotOpt,			//��Ԫ��װ�����ܽ��г���ǿ��
	E_StarEnhanceCondensate_NoEnoughItem,			//û���㹻���߽�����һ������ǿ��
	E_StarEnhanceCondensate_NoEnoughMoney,			//û���㹻��Ǯ������һ�����ǿ��
	E_StarEnhanceCondensate_Failed,					//����ʧ��
	E_StarEnhanceCondensate_NotOpen,				//����ǿ����ʱ�ر�

	E_HolySoulReborn_LevelNotEnough,				//Ԫ�����ﵽ100�����ſ��Խ���ת������
	E_HolySoulReborn_ItemNotExist,					//Ԫ��ת��������Ʒ����
	E_HolySoulReborn_LevelMax,						//Ԫ��ת���ȼ��Ѵﵽ���
	E_HolySoulReborn_ProtoNotExist,					//Ԫ��ת��������	
	E_HolySoulReborn_AwakeCannot,					//Ԫ�����״̬�²���ת��
	E_HolySoulReborn_ItemNotEnough,					//�Ͼ���¶��������
	E_HolySoulReborn_Fail,							//Ԫ��ת��ʧ��

	E_TransEquipSoul_HasNotFuHuned,					//δ����װ�����ܽ��л��ת��
	E_TransEquipSoul_NoEquipSoulTransfer,			//û����Ӧ�Ļ�˿�ת��
	E_TransEquipSoul_NoExsitItem,					//������Ӧ�ĵ�������
	E_TransEquipSoul_Failed,						//���ת�Ʋ���ʧ��
	E_TransEquipSoul_PosNotExist,					//��ת�ƻ�˵�װ����λ��ƥ��
	E_TransEquipSoul_UnIdentified,					//δ����װ�����ɽ��л��ת�Ʋ���
	E_TransEquipSoul_EquipTimeLimited,				//ʱ����װ�����ɽ��л��ת�Ʋ���
	E_TransEquipSoul_NotEnoughNumToCostItem,		//û���㹻�����ĵ���

	E_HolyEquipHole_EquipNotExist,					//���������װ��
	E_HolyEquipHole_ItemNotExist,					//�������׵���
	E_HolyEquipHole_NotHolyEquip,					//����Ĳ���Ԫ��װ��
	E_HolyEquipHole_NotHoleItem,					//����Ĳ��Ǵ��Ƽ�
	E_HolyEquipHole_HoleNumMax,						//��װ����������Ѵﵽ����޷���������
	E_HolyEquipHole_ProtoNotExist,					//������Բ�����	
	E_HolyEquipHole_MoneyNotEnough,					//�����н�Ǯ����
	E_HolyEquipHole_Fail,							//���ʧ��

	E_HolyEquipInlay_EquipNotExist,					//�����Ԫ��װ��
	E_HolyEquipInlay_NotInlayItem,					//�������Ӱ��ʯ
	E_HolyEquipInlay_HoleIDUnusual,					//ѡ����Ƕ�Ŀ�λ�쳣
	E_HolyEquipInlay_HoleNotChisel,					//ѡ����Ƕ�Ŀ�λ��δ����
	E_HolyEquipInlay_HoleAlreadyInlay,				//ѡ����Ƕ�Ŀ�λ�Ѿ���Ƕ��ʯ
	E_HolyEquipInlay_ProtoNotExist,					//��Ƕ���Բ�����	
	E_HolyEquipInlay_FormulaNotExist,				//�䷽������
	E_HolyEquipInlay_MoneyNotEnough,				//�����н�Ǯ����
	E_HolyEquipInlay_EquipCannotInlay,				//��Ӱ��ʯ��װ����Ƕ��λ��ƥ��
	E_HolyEquipInlay_ItemNotEnough,					//�������Ͼ���¶��������
	E_HolyEquipInlay_Fail,							//��Ƕʧ��

	E_HolyEquipRemove_EquipNotExist,				//�����Ԫ��װ��
	E_HolyEquipReomve_HoleIDUnusual,				//ѡ��Ŀ�λ�쳣
	E_HolyEquipReomve_HoleNotChisel,				//ѡ����Ƕ�Ŀ�λ��δ����
	E_HolyEquipReomve_HoleNotInlay,					//װ���ÿ�λ��δ��Ƕ
	E_HolyEquipReomve_BagNotEnough,					//�����ռ䲻��
};

//-----------------------------------------------------------------------------
//	�ϳ���Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_Produce)
	DWORD 				dwNPCID; 		//NPCID
	DWORD				dwSkillID;		//����ID
	INT64				n64ItemID;		//��ƷID
	DWORD 				dwFormulaID;  	//�䷽ID
	INT64				n64IMID;		//IMid
	INT64				n64StuffID[1]; 	//���� �����䳤
CMD_END

CMD_START(NS_Produce)
	DWORD				dwErrorCode;	//������
CMD_END

// �߼�����
CMD_START(NC_AdvancedCompose)
	DWORD dwNPCID; 		//NPCID
	INT64 n64DesignID;
	INT64 n64Material1;
	INT64 n64Material2;
CMD_END

CMD_START(NS_AdvancedCompose)
	DWORD dwErrorCode;
	DWORD dwCompItemID;
CMD_END

//-----------------------------------------------------------------------------
//	�ֽ���Ϣ ���ڵ㻯�����е�һ����֧
//-----------------------------------------------------------------------------
CMD_START(NC_Decomposition)
	DWORD				dwNPCID; 		//NPCID	
	DWORD				dwSkillID;		//����ID
	INT64				n64ItemID;		//��ƷID
	DWORD				dwFormulaID;	//�䷽ID
	INT64				n64IMID;		//IMid
	INT64				n64Item; 		//���ֽ���Ʒ64λID
CMD_END

CMD_START(NS_Decomposition)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	��ȡ��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_Extraction)
	DWORD				dwNPCID; 		//NPCID	
	INT64				n64Item; 		//����ȡ��Ʒ64λID
CMD_END

CMD_START(NS_Extraction)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	ǿ����Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_ConsolidatePosy)
	DWORD 				dwNPCID; 		//NPCID
	DWORD 				dwFormulaID;  	//�䷽ID
	INT64 				n64ItemID; 		//Ҫ��ǿ����Ʒ��64λID
	INT64				n64IMID;		//IMid
	INT64				n64StuffID[1]; 	//���� �����䳤	
CMD_END

CMD_START(NS_ConsolidatePosy)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_ConsolidateEngrave)
	DWORD				dwNPCID;		//NPCID
	DWORD				dwFormulaID;	//�䷽ID
	INT64				n64ItemID;		//Ҫ��ǿ����Ʒ��64λID
	INT64				n64IMID;		//IMID
	INT64				n64StuffID[1];	//���� 
CMD_END

CMD_START(NS_ConsolidateEngrave)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_ConsolidateQuench)
	DWORD				dwNPCID;		//NPCID
	DWORD				dwFormulaID;	//�䷽ID
	INT64				n64ItemID;		//Ҫ��ǿ����Ʒ��64λID
	INT64				n64IMID;		//IMID
	INT64				n64StuffID[1];	//���� 
CMD_END

CMD_START(NS_ConsolidateQuench)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	ʱװ����
//-----------------------------------------------------------------------------
CMD_START(NC_ConsolidateSpin)
	DWORD 				dwNPCID; 		//NPCID
	ESuitAttType 		eSpinType;  	//������������
	INT64 				n64ItemID; 		//Ҫ��ǿ����Ʒ��64λID
	DWORD				dwStuffTypeID;	//���Ĳ���typeid
	DWORD				dwStuffItemNum;	//���Ĳ���
CMD_END

CMD_START(NS_ConsolidateSpin)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	��Ƕ����ӡ������
//-----------------------------------------------------------------------------
CMD_START(NC_Inlay)
	INT64				n64SrcItemID;	//��ʯ��64ID
	INT64				n64DstItemID;	//װ����64ID
CMD_END

CMD_START(NS_Inlay)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_Brand)
	INT64				n64SrcItemID;	//ӡ�ǵ�64ID
	INT64				n64DstItemID;	//װ����64ID
	INT64				n64IMID;		//���׷�64ID ��ʹ��:GT_INVALID
CMD_END

CMD_START(NS_Brand)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_LoongSoul)
	INT64				n64SrcItemID;	//����ʯ64ID
	INT64				n64DstItemID;	//װ����64ID
CMD_END

CMD_START(NS_LoongSoul)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_Chisel)	
	INT64				n64SrcItemID;	//װ����64ID
	INT64				n64StuffID;		//��ʯ��64ID
CMD_END

CMD_START(NS_Chisel)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	����װ��Ǳ��ֵ
//-----------------------------------------------------------------------------

CMD_START(NC_AddPot)
	INT64				n64SrcItemID;	//���ߵ�64ID
	INT64				n64DstItemID;	//װ����64ID
	INT64				n64IMID;		// ���׷�64ID ��ʹ��:GT_INVALID
CMD_END

CMD_START(NS_AddPot)
	DWORD				dwErrorCode;	//������ 0���ɹ��� 1�������� 2��ʧ�� 
	DWORD				dwPotValue;		// ���ӵ�Ǳ��ֵ
CMD_END

// Jason v1.3.1 2009-12-21 ��ʯ������Э��
CMD_START( NC_GetGemRemovalInfo )
	DWORD 				dwNPCID; 		// NPCID
	INT64				n64EquipID;		// װ������id
CMD_END

CMD_START( NS_GetGemRemovalInfo )
	DWORD				dwErrCode;		// 
	INT16				n16GemNum;		// ��ʯ���������5�ű�ʯ
	DWORD				dwGemIDs[MAX_EQUIPHOLE_NUM];	// ��ʯtypeids��0/GT_INVALID��ʾû�б�ʯ
CMD_END

CMD_START( NC_GemRemoval )
	DWORD 				dwNPCID; 		// NPCID
	INT64				n64EquipID;		// װ������id
	INT16				n16GemNum;
	INT64				n64GemRomovalSign[MAX_EQUIPHOLE_NUM];	// ��ʯ�����id
CMD_END

CMD_START( NS_GemRemoval )
	DWORD				dwErrorCode;
CMD_END

// Jason 2010-4-15 v2.0.0 ��������
CMD_START( NC_FabaoPractice )
	INT64				n64SrcItemID;	// ����ʯid
	INT					nSrcItemNum;	// ������ʯ����
	INT64				n64FabaoID;		// ����id
CMD_END
CMD_START( NS_FabaoPractice )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END
// ������Ƕ
CMD_START( NC_FabaoInlay )
	INT64				n64FabaoID;
	INT64				n64Bead;	//����serialid
CMD_END
CMD_START( NS_FabaoInlay )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END

// ������ת
CMD_START( NC_FabaoWuxingReversal )
	INT64				n64MainFabaoID;
	INT64				n64SecondaryFabaoID;
CMD_END
CMD_START( NS_FabaoWuxingReversal )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END
// ����ת��
CMD_START( NC_FabaoWuxingMerging )
	INT64				n64MainFabaoID;
	INT64				n64SecondaryFabaoID;
CMD_END
CMD_START( NS_FabaoWuxingMerging )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END

// ����Ǳ��ֵ������Ҫ��Ǳ��ֵ������������
CMD_START( NC_RaiseEquipPotval )
	INT64		n64EquipID;
	INT64		n64ItemID;
CMD_END
CMD_START( NS_RaiseEquipPotval )
	DWORD dwErrorCode;
CMD_END

// Jason 2010-5-25 v2.1.2 װ�������Լ�����
CMD_START( NC_BuildEuipment )
	DWORD		dwNPCID;
	INT64		n64ToBuildEquipID;		// ������װ��64λid
	INT64		n64StoneID		;		// ��ʯID
	INT64		n64StuffID		;		// ����id��Ҳ��װ��
CMD_END
CMD_START( NS_BuildEuipment )
	DWORD	dwErrorCode;
CMD_END
//-----------------------------------------------------------------------------
//	װ������
//-----------------------------------------------------------------------------
CMD_START(NC_SoarEquipCasting)
	DWORD 				dwNPCID; 		//NPCID
	DWORD 				dwFormulaID;  	//�䷽ID
	INT64 				n64ItemID; 		//Ҫ��ǿ����Ʒ��64λID
	INT64 				n64ProtectSignID;	//���׷�64λID
	INT64				n64StuffID[1]; 		//���� �����䳤	
CMD_END

CMD_START(NS_SoarEquipCasting)
	DWORD				dwErrorCode;	//������
CMD_END


//struct tagCastingStuff
//{
//	INT64		n64ItemID;
//	UINT16		n16ItemNum;
//};
//
//CMD_START( NC_CastingEquipment )
//	DWORD	dwNPCID;
//	INT64	n64EquipID;
//	DWORD	dwCastringProtoID;
//	INT64	n64ItemID;
//	BYTE	byItemNum;
//	BYTE	byData[1];	// tagCastingStuff
//CMD_END

//CMD_START( NS_CastingEquipment )
//	DWORD dwErrorCode;
//CMD_END
// װ��ǿ��
CMD_START( NC_StrengthenEquip )
	INT64	n64ToBeStrengthenedEquip;	// ��ǿ��װ��
	INT64	n64StrengthenStone;			// ǿ��ʯ
	INT64	n64LuckySign;					// ���˷���Ʒid���Ǳ���Ʒ����0����-1����ʾû��
	INT64	n64ProtectedSign;				// ���׷����Ǳ���Ʒ����0����-1����ʾû��
CMD_END
CMD_START( NS_StrengthenEquip )
	DWORD dwErrorCode;
CMD_END
// ���긽��
CMD_START( NC_DetachOrAttackSoul_Equip )
	BOOL	bDetachOrAttack;				// TRUE�����ꣻFALSE������
	INT64	n64Equip;						// װ��
	INT64	n64Stone;						// ����ʯ���߸���ʯ
CMD_END
CMD_START( NS_DetachOrAttackSoul_Equip )
	DWORD dwErrorCode;
	BOOL bDetachOrAttack;	// TRUE�����ꣻFALSE������
	INT64	n64ID;				// 64λID
CMD_END

// ��Ԫ
CMD_START( NC_GuiYuan )
	INT64	n64Equip;						// װ��
	INT64	n64Stone;						// ��Ԫʯ
CMD_END
CMD_START( NS_GuiYuan )
	DWORD dwErrorCode;
CMD_END

// ����֮��
CMD_START( NC_FairyPowder )
	INT64	n64Fairy;						// ����
	INT64	n64FairyPowder;					// ����֮��
CMD_END
CMD_START( NS_FairyPowder )
	DWORD dwErrorCode;
CMD_END

CMD_START( NC_FairyRaiseIntelligenceMax )
	INT64	n64Fairy;						// ����
	INT64	n64FairyPowder;					// ����֮��
CMD_END
CMD_START( NS_FairyRaiseIntelligenceMax )
	DWORD dwErrorCode;
CMD_END

CMD_START( NC_FairyDoRaiseIntelligence )
	INT64	n64Fairy;						// ����
	INT64	n64FairyPowder;					// ����֮��
CMD_END
CMD_START( NS_FairyDoRaiseIntelligence )
	DWORD dwErrorCode;
CMD_END

CMD_START( NC_GodStrengthen )
	INT64		dwEquipID;			// ����װ��ID
	INT64		dwAssitEquipID;		// ����װ��ID
CMD_END

CMD_START( NS_GodStrengthen )
	DWORD	dwErrorCode;
CMD_END

// ����Ԫ���˺�ת��
CMD_START( NC_FairyEleInjuryChange )
	DWORD 				dwNPCID; 		//NPCID
	INT64				n64FairyID;	// ������ţ������е�����
	EElementInjury		eEleInjuryType;		// Ҫת��Ϊʲô���͵�Ԫ���˺�
	INT64				n64ItemID;				// �������ߵ�ID
CMD_END
CMD_START( NS_FairyEleInjuryChange )
	DWORD	dwErrorCode;			// �ɹ�0���������ʧ�ܣ��������
	INT64				n64FairyID;	// ������ţ������е�����
	EElementInjury		eEleType;	// �ɹ���Ԫ���˺����ͣ������˺�ֵ����
CMD_END

CMD_START( NC_FairyIncEleInjury )
	BYTE			byType;			// ������������
	BYTE			bAttType;		// Ԫ������ 0�˺� 1��͸ 2�ֿ�
	INT64			n64Fruit1;		// ������ʵ1
	INT64			n64Fruit2;		// ������ʵ2
	INT64			n64Fruit3;		// ������ʵ3
	INT64			n64VerityOfStone;	// ����֮ʯ
CMD_END
CMD_START( NS_FairyIncEleInjury )
	DWORD			dwErrorCode;
	INT				nAdded;
CMD_END
// ������������������ [3/12/2012 zhangzhihua]
CMD_START(NC_FairyAddPromoteTimes)
	BYTE			byType;			// 1:�˺� 2:�ֿ� [3/12/2012 zhangzhihua]
CMD_END
CMD_START(NS_FairyAddPromoteTimes)
	DWORD			dwErrorCode;
CMD_END
// �����ؼ�ȷ��
CMD_START( NC_FairyStuntConfirm )
	BOOL		bConfirmOrNot;		// �Ƿ�ȷ��
CMD_END
CMD_START( NS_FairyStuntConfirm )
	BOOL		bConfirmOrNot;		// �Ƿ�ȷ�ϣ������������
	DWORD		dwErrorCode;
CMD_END

// �����ؼ���ѧϰ
CMD_START( NC_FairyStuntBook )
	INT64		n64StuntBook;	// ������
CMD_END
CMD_START( NS_FairyStuntBook )
	DWORD		dwErrorCode;
CMD_END

// ��������
CMD_START( NC_FairyNirvana )
	INT64		n64NirvanID;	// ������Ʒ
CMD_END
CMD_START( NS_FairyNirvana )
	DWORD		dwErrorCode;
CMD_END

// ��ʯ
CMD_START( NC_RockStone )
	INT64		n64RockID;
	INT64		n64EquipID;
CMD_END
CMD_START( NS_RockStone )
	DWORD dwErrorCode;
CMD_END

// װ���ɳ�
CMD_START( NC_EquipGrow )
	INT64		n64Equip;
	DWORD		dwGrowValue;	//�ɳ���
	DWORD		dwIM; // ���˷�
CMD_END
CMD_START( NS_EquipGrow )
	DWORD dwErrorCode;
CMD_END

// װ���ɳ�����ת��
CMD_START(NC_EquipGrowTrans)
	INT64 n64OldEquip;
	INT64 n64NewEquip;
	INT64 n64IM;
CMD_END
CMD_START(NS_EquipGrowTrans)
	DWORD dwErrorCode;
CMD_END

// װ���ɳ�����
CMD_START(NC_EquipGrowDec)
	INT64 n64Equip;
	INT64 n64IM;
CMD_END
CMD_START(NS_EquipGrowDec)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_SoulCrystalGrow)
	INT64	n64Serial;
	INT64	n64IMSerial;
CMD_END

CMD_START(NS_SoulCrystalGrow)
	DWORD dwErrorCode;
CMD_END

//װ�������ɳ� һ�³���10������~
CMD_START(NC_EquipPerfectGrow)
	INT64 n64IMSerialID;
	INT64 n64EquipID;
CMD_END

CMD_START(NS_EquipPerfectGrow)
	DWORD dwErrorCode;
CMD_END

//�������ɳ��� 
CMD_START(NC_XiPurpleStar)
	INT64 n64IMID;
	INT64 n64EquipID;
CMD_END

CMD_START(NS_XiPurpleStar)
	DWORD dwErrorCode;
CMD_END

// �꾧����
#define SoulCrystal_Renew_Item_1 4820083
#define SoulCrystal_Renew_Item_2 4820084
CMD_START(NC_SoulCrystalRenew)
	INT64	n64Serial;
CMD_END

CMD_START(NS_SoulCrystalRenew)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_DiamondEquipGrowIM)
	INT64 n64ItemID;	//����id
	INT64 n64EquipID;	//װ��id
CMD_END

CMD_START(NS_DiamondEquipGrowIM)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_HolyStrength)
	INT64 n64ItemID;
CMD_END

CMD_START(NS_HolyStrength)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_HolyEquipmentNumber)
INT64 n64ItemID;
CMD_END

CMD_START(NS_HolyEquipmentNumber)
DWORD dwErrorCode;
CMD_END

#pragma pack(pop)
		





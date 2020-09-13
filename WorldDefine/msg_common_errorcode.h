#pragma once

//------------------------------------------------------------------------
// ����������Ϣ��ͬ�Ĵ�����
// 
// ����E_Success�����д������Ϊ������������Ϣ���еĴ�����ɴ�1��ʼ
//------------------------------------------------------------------------
enum
{
	E_Success						= 0,
	E_SystemError					= -1,

	E_Item_NotFound					= -2,		// û���ҵ���Ʒ
	E_Item_NotEquipment				= -3,		// ����װ��

	E_Item_CanNotAdd				= -4,		// ������ӵ�ָ����������
	E_Item_CanNotRemove				= -5,		// ���ܴ�����ɾ����Ʒ
	E_Item_CanNotMove_Other			= -6,		// �����ƶ�������������
	E_Item_AddFailed				= -7,		// �������в���ʱʧ��

	E_Item_Place_NotFree			= -8,		// Ŀ��λ�ò�Ϊ��

	E_Item_LevelLimit				= -9,		// �ȼ�������Ҫ��
	E_Item_SexLimit					= -10,		// �Ա�����Ҫ��
	E_Item_ClassLimit				= -11,		// ְҵ������Ҫ��
	E_Item_TalentLimit				= -12,		// ���ʲ�����Ҫ��
	E_Item_ClanRepLimit				= -13,		// ��������������Ҫ��
	E_Item_OtherClanRepLimit		= -14,		// ��������������Ҫ��
	E_Item_RoleStatusLimit			= -15,		// ���״̬(ս������ս��)������Ҫ��
	E_Item_AttA_Limit				= -16,		// һ�����Բ�����

	E_Item_NotEnough				= -20,		// ��Ʒ��������

	E_Item_MaxHold					= -25,		// ��Ʒ�ﵽ��ӵ������

	E_Item_CountLimit				= -26,		// ϡ����Ʒ����ȫ�����������ƶ�����ʰȡ

	E_Holy_CountLimit				= -27,		// ��������ʥ���������ƣ������ٻ��

	E_Item_Used_NotValid			= -30,		// ʹ�õ���Ʒ�Ƿ�

	E_Cannot_FindMap				= -31,		// �޷��ҵ���ͼ
	E_Area_Limitted					= -32,		// �������򣬲��ɽ���֮��
	E_Role_Status_Limitted			= -33,		// ��ɫ״̬����
	E_Role_Buff_Limit					= -34,		// ��ɫ����buff����

	E_Pet_RecallWithoutItem			= -35,		// û���ҵ����ճ�����Ҫ�ĵ���
	E_Pet_RecallDeleteErr			= -36,		// ɾ������ʧ��

	E_Bag_NotEnoughSpace			= -100,		// ����û���㹻�Ŀռ�
	E_Con_NotEnoughSpace			= -101,		// û���㹻�Ŀռ�
	E_Con_PswNotPass				= -102,		// δ�������������
	E_Ware_MoveLimit				= -103,		// ����Ʒ���ܴ���ֿ�
	E_GuildWare_MoveLimit			= -104,		// ����Ʒ���ܷ�����ɲֿ�

	E_BagYuanBao_NotEnough			= -200,		// ������Ԫ������
	E_BagSilver_NotEnough			= -201,		// �����н�Ǯ����
	E_ExVolume_NotEnough			= -202,		// ��ɫ���㲻��
	E_VipPoint_NotEnough            = -203,     // ��ɫ���ֲ�����

	E_Role_Not_ExistIn_World		= -300,		// ��Ϸ�����в�����ָ����ɫ
	E_Role_Not_Online				= -301,		// ��ɫ������
	E_Role_CantMove					= -302,		// ��ɫ���ڲ������ƶ�״̬

	E_NPC_NotFound					= -310,		// û���ҵ�ָ��NPC
	E_NPC_NotValid					= -311,		// ְ�����Ͳ���
	E_NPC_TooFar					= -312,		// ����NPC̫Զ

	E_Filter_Name_InFilterFile		= -351,		// �ڹ��˴ʱ���
	E_Filter_Name_TooLong			= -352,		// ����̫��
	E_Filter_Name_Null				= -353,		// ����Ϊ��
	E_Filter_Name_Illegal			= -354,		// ���ְ��������ַ�
	E_Filter_Chat_NoContent			= -355,		// ��������Ϊ��
	E_Filter_Text_TooLong			= -356,		// ����̫��
	E_Filter_Name_TooShort			= -357,		// ����̫��

	E_Opt_Timeout					=	-400,	// ������ʱ
	E_Opt_Fail						= -401,		// ����ʧ��

	E_Promotion_Clergy_Type			= -410,		// ��ְ������ʽ����
	E_Promotion_Clergy				= -411,		// ��ְ�����ʸ�δ����
	E_Enroll_Clergy_Limited			= -412,		// ��ְ��ѡ�������Ѿ�����
	E_God_Area_None					= -413,		// ����δӵ�и������������޷�����	
	E_God_Area_Max					= -414,		// �����������ѡ������ϡ������֮���Ѿ������������ޣ��޷������µ���������
	E_God_Area_Condition			= -415,		// ��δ���������ļ����������޷�����
	E_Enroll_Clergy_LevelLimited	= -416,		// ��ְ��ѡ�������ȼ�����
	E_Enroll_Clergy_SexLimited		= -417,		// ��ְ��ѡ�������Ա𲻷�
	E_Enroll_Clergy_ClassLimited	= -418,		// ��ְ��ѡ������ְҵ����
	E_Enroll_Clergy_GodheadLimited	= -419,		// ��ְ��ѡ��������񲻹�
	E_Enroll_Clergy_AttValueLimited	= -420,		// ��ְ��ѡ�������������ֵ����
	E_God_Area_CD					= -421,		// CDʱ��δ��
	E_God_Area_FaithValue			= -422,		// ��������
	E_KeyCode_CodeInvalid		= -423,		// ��Ч��key��
	E_KeyCode_NPCInvalid		= -424,		// NPC��Ч
	E_KeyCode_OnlyOnce			= -425,		// ֻ����ȡһ��
	E_God_Area_Career		= -426,			//ְҵ���������ɼ���
	E_Offline_Role_Sprite		= -427,			//���ߺ�Ľ�ɫ��������

	E_NOT_GET_GIFT				= -428,			//��������ȡ�������

	E_TimeEquipLimit			= -429,		//������ʱװ�����ɽ���

	E_Item_EquipBar_Error = -500, // װ������Ʒ��λ

	E_Call_Holy_NotTakeoff = -501, // �ٻ�ʥ�鲻�ܱ�����

	E_Return_NotNeedFeed			= -10000,	// ������ͻ��˷��� -- ��ֵΪ�����������ͻ�����Ϣ��
};
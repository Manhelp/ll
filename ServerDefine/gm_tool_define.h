//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_c.h
// author: 
// actor:
// data: 2009-04-27
// last:
// brief: GM�������GM����֮������ݶ���
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

#define Notice_Title 30
#define Notice_Content 256
#define Notice_Link 256
#define System_Cmd_Length 1024


// �о��붨ʱ�йص���Ϣ
enum ETimeLimitedMsg
{
	ETLM_Start,

	ETLM_Double = 1,
	ETLM_RightNotice,
	ETLM_AutoNotice,
	ETLM_AutoChatNotice,

	ETLM_End
};

enum 
{
	E_GMTool_Err							= -1,
	E_GMTool_Success						= 0,

	E_GMTool_GM_AlreadyExist				= 1,		// GM�Ѿ�����
	E_GMTool_GM_NotExist					= 2,		// GM������
	E_GMTool_GM_CreateFail					= 3,		// GM����ʧ��

	E_GMTool_ResumeRole_NameAlreadyExist	= 10,		// ��ɫ���Ѿ����ڣ�����������
	E_GMTool_ResumeRole_RoleNotExist		= 11,		// �ָ��Ľ�ɫ������
	E_GMTool_ResumeRole_RoleNotReMove		= 12,		// �ָ��Ľ�ɫδ��ɾ��

	E_GMTool_ModifyRole_RoleNotExist		= 20,		// ��ɫ������

	E_GMTool_NullContent					= 30,		// ���ݲ���Ϊ��

	E_GMTool_RoleNotExist					= 40,		// ��ɫ������
	E_GMTool_AccountNotExist				= 41,		// �˺Ų�����
	E_GMTool_RoleNotOnline					= 42,		// ��ɫ������	

	E_GMTool_DeleteItem_ItemNotExist		= 50,		// ��Ʒ������
	E_GMTool_CreateItem_EquipNumErr			= 51,		// װ����������
	E_GMTool_CreateItem_ItemQualityErr		= 52,		// ��ƷƷ������
	E_GMTool_CreateItem_ItemTypeIDErr		= 53,		// �������ƷID����ȷ

	E_GMTool_GuildNotExist					= 60,		// ���ɲ�����

	E_GMTool_Time_Err						= 100,		// ʱ�䲻��	

	E_RTClient_IpBind_Err					= 110,		// ip�󶨴���
	E_RTClient_Ip_Err						= 111,		// ip�󶨴���

	E_GMTool_TimeSetError					= 120,		// ʱ�����ô������õĿ�ʼʱ���ѹ�

	R_RTClient_NotEnoughPri					= 130,		// Ȩ�޲��������ܲ�ѯ

	E_GMTool_ResumeItemNotExit				= 140,		// Ҫ�ָ���װ��������
	E_GMTool_ResumeItemNotEquip				= 141,		// Ҫ�ָ�����Ʒ����װ��

	E_GMTool_MoveRole_MapNameErr			= 150,		// ��ͼ������
	E_GMTool_MoveRole_CoordinateErr			= 151,		// ���겻�Ϸ�
    E_GMTool_MoveRole_RoleOffLine           = 152,      // ������� 

	E_GMTool_AddPKLog_RoleOffline			= 155,		// ��ɫ������
	E_GMTool_AddPKLog_RoleInvalid			= 156,		// ��ɫָ���Ĳ���

	E_GMTool_FilterWords_Existed			= 160,		// ���˴��Ѿ�����
	E_GMTool_FilterWords_NotExisted			= 161,		// ���˴ʲ�����

	E_GMTool_LoginDBLost	= 162,	// login���ݿ�����ʧ��
};

enum EDoubleType
{
	EDoubleType_NULL		= -1,
	EDoubleType_Exp			= 0,				//˫������
	EDoubleType_Item		= 1,				//˫����Ʒ
	EDoubleType_End			= 2,
};

const INT NUM_EDT			= EDoubleType_End - EDoubleType_Exp;
#define EDT_VALID(edt)	((edt) > EDoubleType_NULL && (edt) < EDoubleType_End)	


//���ܵ�/�Զ�ѭ��������ɫ����
enum EColorType
{
	EColorType_NULL			= -1,
	EColorType_Red			= 0,
	EColorType_Yellow		= 1,
	EColorType_Green		= 2,	
	EColorType_Blue			= 3,
	EColorType_Orange		= 4,
	EColorType_End			= 5,
};

//Log�ṹ
enum EActionType
{
	EAT_Null,

	EAT_CreateGM = 1,			// ����GM����
	EAT_ModifyGM,				// ����GM�޸�
	EAT_DeleteGM,				// ����GMɾ��

	EAT_ResumeRole,				// ��ɫ�ָ�
	EAT_ProModify,				// �����޸�

	EAT_ItemCreate,				// ��������
	EAT_ItemDelete,				// ����ɾ��
	EAT_ItemResume,				// ���߻ָ�

	EAT_DoubleStart,			// ����˫��
	EAT_DoubleEnd,				// ˫��ɾ��

	EAT_AutoNoticeStart,		// �Զ�ѭ������
	EAT_AutoNoticeEnd,			// �Զ�ѭ������ɾ��

	EAT_NoticeStart,			// ���½ǹ���
	EAT_NoticeEnd,				// ���½ǹ���ɾ��

	EAT_AutoChatStart,			// ����Ƶ������
	EAT_AutoChatEnd,			// ����Ƶ������ɾ��

	EAT_StartServer,			// ����������
	EAT_CloseServer,			// �رշ�����
	EAT_KillServer,				// ǿ�ƹرշ�����

	EAT_IpBind,					// ��IP

	EAT_SendSystemCmd,			// ����ϵͳ��Ϣ

	EAT_AccountBanned,			// �˺ŷ�ͣ
	EAT_UndoAccountBanned,		// �˺Ž��

	EAT_End
};
#pragma pack(pop)
//////////////////////////////////////////////////////////////////////////
// ����ϵͳ��Ϣ����
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push, 1)
#include "..\WorldDefine\family_define.h"

//////////////////////////////////////////////////////////////////////////
// common
//////////////////////////////////////////////////////////////////////////
enum
{
	E_FAMILY_SUCCESS =	0,

	E_CreateFamily_NoTeam =1,									// ����4�˻�4��������Ӳſɴ�������
	E_CreateFamily_NotLeader=2,								// ֻ�жӳ��ſɴ�������
	E_CreateFamily_TeamMemNotEnough=3,				// ���Ķ������������4�ˣ������޷���������
	E_CreateFamily_MemberHaveFamily=4,					// ���Ķ����Ա�����Ѿ��������������壬�޷�����
	E_CreateFamily_MemberJustLeaveFamily=5,			// ���Ķ����Ա�����˸�������壬�޷�����
	E_CreateFamily_NameExist=6,								// ������

	E_DismissFamily_NotLeader=7,								// �������峤���޷���ɢ����
	E_DismissFamily_FamilyNotEmpty=8,					// �����ﻹ��������Ա����ɢ��������ڼ�����û�������κγ�Ա������½���

	E_InviteRole_NoRole=9,											// ��ɫ�����ڣ��޷���������
	E_InviteRole_RoleHadFamily=10,								// �Է�ӵ���������壬�޷���������
	E_InviteRole_FamilyFull=11,										// ����������Ѿ����ͣ��޷��������µ����
	E_InviteRole_JustLeaveFamily=12,							// �Է��ո������������壬��ʱ�޷������µļ���

	E_ApplyJoin_JustLeaveFamily=13,							// ����������壬��ʱ���޷������µļ���
	E_ApplyJoin_DestFamilyFull=14,								// �Է��ļ��������������޷�����
	E_ApplyJoin_DestRoleOffLine=15,							// Ŀ���Ѳ����ߣ��޷�����
	E_ApplyJoin_FamilyNotExist=16,								// Ŀ����岻����
	E_ApplyJoin_RoleHadFamily=17,								// ���Ѿ������������壬�޷���������
	
	E_LeaveFamily_BeLeader=18,									// �����峤���޷��������
	E_LeaveFamily_BeMarried=19,									// ���Ѿ���飬����������壬�����Ƚ�����ķ��޹�ϵ

	E_KickMember_CanNotKickSelf=20,							// ���޷������Լ�
	E_KickMember_NotLeader=21,									// ���޷������Լ�

	E_HandOverLeader_NotLeader=22,							// ֻ���峤�����ƽ�Ȩ��
	E_HandOverLeader_TargetNotInFamily=23,				// Ŀ�겻�Ǽ����Ա�������ƽ��峤����

	// ����������ش�����
	E_FamilyQuest_GiveInItem_RoleInvalid = 30,	// �Ͻ���Ʒ�Ľ�ɫ��Ч
	E_FamilyQuest_GiveInItem_QuestInvalid=31,			// ��������ID��Ч
	E_FamilyQuest_GiveInItem_QuestNotMatch=32,	// ��ɫ�ļ����޴�����
	E_FamilyQuest_GiveInItem_ItemNotEnough=33,		// ��������Ʒ����
	
	E_FamilyQuest_Rewarded_Error=34,
	E_FamilyQuest_Rewarded_ProcessLimit=35,
	E_FamilyQuest_Rewarded_NewJoiner=36,
	E_FamilyQuest_Rewarded_AlreadyDone=37,
	E_FamilyQuest_Rewarded_BagFull=38,
	
	E_FamilyQuest_GiveInItem_NotQuestItem = 39,	// �Ͻɵ���Ʒ���Ϸ�
	E_FamilyQuest_GiveInItem_QuestItemEnough = 40,
	// 

	E_FamilyJoin_Refuse			= 51,					// �ܾ�����
	E_FamilyApply_Refuse		= 52,					// �ܾ�����
	E_FamilyJoin_TimeOut		= 53,					// ���볬ʱ
	E_FamilyApply_TimeOut     = 54,					// ���볬ʱ
};

// ����ʧ�ܷ���
CMD_START(NS_FamilyProcFailed)
	DWORD			dwErrorCode;
CMD_END

// ��������
CMD_START(NC_CreateFamily)
	DWORD		dwNPCID;
	TCHAR		szFamilyName[1];
CMD_END

CMD_START(NS_CreateFamilyBroad)
	DWORD		dwRoleID;		// ������id
	DWORD		dwFamilyID;
	TCHAR			szName[1];		// ��ɫ��+������
CMD_END

CMD_START(NS_CreateFamilyAllBroad)
	TCHAR		szRoleName[X_SHORT_NAME];		// ����������
	TCHAR		szFamilyName[X_SHORT_NAME];		// ������
CMD_END

// ��ɢ����
CMD_START(NC_FamilyDismiss)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_FamilyDismiss)
CMD_END

// �������
CMD_START(NC_FamilyJoinReq)		// �������Ŀ��������
	DWORD		dwDstRoleID;
	TCHAR		szDstRoleName[1];
CMD_END

CMD_START(NS_FamilyJoinReq)		// �������������͸�Ŀ�����
	DWORD		dwSrcRoleID;
	DWORD		dwFamilyID;
	TCHAR		szSrcRoleName[1];
CMD_END

CMD_START(NC_FamilyJoinReqRes)	// Ŀ����ҷ���������
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
	DWORD		dwDstFamilyID;
CMD_END

CMD_START(NS_FamilyJoinReqRes)	// ������ת����Ŀ����ҷ���
	DWORD		dwErrorCode;
	DWORD		dwInviterID;
	DWORD		dwInviteeID;
CMD_END

CMD_START(NS_FamilyJoinBroad)	// ������
	DWORD		dwRoleID;
	DWORD		dwFamilyID;
	TCHAR		szRoleName[1];
CMD_END

// �������
CMD_START(NC_FamilyApply)		// �������������
	DWORD		dwApplyRoleID;
	TCHAR		szDstRoleName[1];
CMD_END

CMD_START(NS_FamilyApply)		// �������������͸��峤
	DWORD		dwApplyRoleID;
	TCHAR		szApplyRoleName[1];
CMD_END

CMD_START(NC_FamilyApplyRes)	// �峤����������
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
	DWORD		dwDstFamilyID;
CMD_END

CMD_START(NS_FamilyApplyReqRes)	// ������ת����Ŀ����ҷ���
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
CMD_END

//�������������Ĺ���
//CMD_START(NS_FamilyJoinBroad)	// ������

// ����뿪����
CMD_START(NC_FamilyLeave)
CMD_END

CMD_START(NS_FamilyLeaveBroad)
	DWORD		dwSrcRoleID;
CMD_END

// ������ҳ�����
CMD_START(NC_FamilyKick)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_FamilyKickBroad)
	DWORD		dwDstRoleID;
CMD_END

// �ƽ��峤
CMD_START(NC_FamilyHandOver)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_FamilyHandOverBroad)
	DWORD		dwSrcRoleID;
	DWORD		dwDstRoleID;
CMD_END

// ��ȡ��������
CMD_START(NC_GetFamilyName)
DWORD		dwFamilyID;
CMD_END

CMD_START(NS_GetFamilyName)
DWORD		dwFamilyID;
TCHAR		szFamilyName[1];
CMD_END

// ��ȡ������Ϣ���ͻ��ˣ�
CMD_START(NC_GetFamilyInfo)
DWORD		dwFamilyID;
CMD_END

CMD_START(NS_GetFamilyInfo)
INT8					n8MemberNum;
tagRoleFamilyInfo		sFamilyInfo[1]; 
CMD_END

// ͬ�������Ծ��
CMD_START(NS_SyncFamilyActive)
	INT nActive;
CMD_END

//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////

//��������
CMD_START(NC_FamilySpriteTrain) 
	DWORD dwTrainID;		// ����ID
	INT64	n64ID;				// ������Ʒ��64ID
	BOOL bAgain;			// �Ƿ��ٴ��ж�
CMD_END

CMD_START(NS_FamilySpriteTrain)
	DWORD	dwTrainID;		//����ID
	INT		nTrainRet;		//��ߵ�����
	DWORD	dwErrorCode;
CMD_END


//�����Ǽ�
CMD_START(NC_FamilySpriteRegister) 
	INT64 n64id;
	//BOOL  bAgain;			// �Ƿ��ٴ��ж�
CMD_END

CMD_START(NS_FamilySpriteRegister)
	DWORD dwErrorCode;
CMD_END

//ȡ������
CMD_START(NC_FamilySpriteGetBack) 
CMD_END

CMD_START(NS_FamilySpriteGetBack)
	DWORD dwErrorCode;
CMD_END

//��ȡ������������
CMD_START(NC_GetFamilySpriteTrain) 
CMD_END

CMD_START(NS_GetFamilySpriteTrain)
	DWORD	dwFamilyID;		// ����ID
	DWORD	dwErrorCode;
	INT		nNum;
	tagFamilySpriteRequirementSave	ftsi[1];
CMD_END

//��ȡ�����Ǽ���Ϣ
CMD_START(NC_GetFamilyRoleSprite) 
CMD_END

CMD_START(NS_GetFamilyRoleSprite)
	DWORD	dwFamilyID;		// ����ID
	DWORD	dwErrorCode;
	INT		nNum;
	tagRoleSpriteDataSave frsi[1];
CMD_END

//��ȡ�����Ǽ���Ϣ
CMD_START(NC_GetFamilySpriteAtt) 
CMD_END

CMD_START(NS_GetFamilySpriteAtt)
	DWORD dwSpriteAtt[EFSA_Num];		// -1����δ����
CMD_END

//###����ػ��������������ػ�������###���������ͣ�����###����������
CMD_START(NS_FamilySpritTrainBroad) 
	TCHAR	tszRoleName[X_SHORT_NAME];	
	DWORD	dwType;
	INT		nRet;
CMD_END

//��ȡ����������Ϣ
CMD_START(NC_GetFamilySpriteInfo) 
CMD_END

CMD_START(NS_GetFamilySpriteInfo)
	INT		   nLevel;		// �ȼ�
	DWORD dwExp;		// ��ǰ�ľ���
	DWORD dwSpriteAtt[EFSA_Num];		// ��ǰ��attֵ 
CMD_END
//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////

// �򿪼����������
CMD_START(NC_OpenFamilyQuestPage) 
CMD_END

CMD_START(NS_OpenFamilyQuestPage)
	INT				nProcess;			// ������ɶ�
	bool			bIsReceive;			// �Ƿ���ȡ����
	BYTE			byQuestNum;			// �����������
	tagFamilyQuest	Quests[1];			// ��������
CMD_END

// ���ռ�������������
CMD_START(NC_YesterdayQuestInfo)
CMD_END

CMD_START(NS_YesterdayQuestInfo)
	INT				nYesterdayPercent;	// ������ɶ�
	bool			bIsGetReward;		// �Ƿ���ȡ����
CMD_END

// �Ͻ�������Ʒ
CMD_START(NC_GiveInFamilyQuestItem)
	DWORD			dwQuestID;			// ����ID
	DWORD			dwTypeID;			// �Ͻ���Ʒ��TypeID(�Ͻ���Ʒ��ʱ����Ч)
	INT64				n64EquipSerial;	// �Ͻ�װ����64λID(�Ͻ�װ����ʱ����Ч)
	INT				nNum;				// ��Ʒ����
CMD_END

CMD_START(NS_GiveInFamilyQuestItem)
	DWORD			dwErrorCode;
	DWORD			dwQuestID;			// ����ID
	INT				nCount;				// �������
CMD_END

// ��ȡ����
CMD_START(NC_RewardFamilyMember) 
CMD_END

CMD_START(NS_RewardFamilyMember)
	DWORD			dwErrorCode;
CMD_END

#pragma pack(pop)
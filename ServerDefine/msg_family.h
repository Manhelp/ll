//////////////////////////////////////////////////////////////////////////
// ����ϵͳ��Ϣ����
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push, 1)

#include "family_define.h"

struct tagFamilyAtt;
struct tagFamilyMember;

//////////////////////////////////////////////////////////////////////////
// common
//////////////////////////////////////////////////////////////////////////
CMD_START(NDBC_CreateFamily)
	tagFamilyLoad		sFamilyLoad;
CMD_END

CMD_START(NDBC_DismissFamily)
	DWORD		dwFamilyID;
CMD_END

CMD_START(NDBC_LoadAllFamily)
CMD_END

CMD_START(NDBS_LoadAllFamily)	// ÿ�η���һ��������Ϣ
	tagFamilyLoad		sFamilyLoad;
CMD_END

CMD_START(NDBC_LoadFamilyMember)
CMD_END

CMD_START(NDBS_LoadFamilyMember)
	INT				nMemberNum;
	tagFamilyMember	sFamilyMember[1];
CMD_END

CMD_START(NDBS_FamilyMemberInitOK)
CMD_END

CMD_START(NDBC_FamilyInitOK)
CMD_END

CMD_START(NDBS_FamilyInitOK)
CMD_END

//-----------------------------------------------------------------------------
// �ر���Ϸ������ʱ�������������
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveAllFamily)
	INT					   nFamilyNum;
	tagFamilyLoad	   pFamilyLoad[1];
CMD_END

//-----------------------------------------------------------------------------
// ���Ա��ʱ�����������
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveFamilyAtt)
	tagFamilyAtt   sFamilyAtt;
CMD_END

//-----------------------------------------------------------------------------
// �����Ծ�ȱ��ʱ�����������
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveFamilyActive)
	DWORD   dwFamilyID;
	INT           nActive;             // �����Ծ��
CMD_END

//-----------------------------------------------------------------------------
// �����Ա
//-----------------------------------------------------------------------------
CMD_START(NDBC_JoinFamily)			// �������
	tagFamilyMember	sFamilyMemInfo;
CMD_END

CMD_START(NDBC_LeaveFamily)			// �뿪�������
	DWORD		dwRoleID;
CMD_END

CMD_START(NDBC_SetLeader)		// �峤�������ƽ�
	DWORD		dwRoleID;
	BOOL			bLeader;
CMD_END

//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////

// ˢ�¼�������
// ���������ϢҪ�������飺1��������״̬��Ϊ�ѷ�����2����ճ�Ա��ȡ������ʶ
CMD_START(NDBC_RenewFamilyQuest)
	DWORD dwFamilyID;
	INT nProcess;
	BYTE byNum;
	tagFamilyQuest Quests[MAX_FAMILY_QUESTS_NUM];
CMD_END

// ͬ������������Ϣ
CMD_START(NDBC_SaveFamilyQuestDetail)
	DWORD dwFamilyID;
	tagFamilyQuest Quests[MAX_FAMILY_QUESTS_NUM];
CMD_END

// ���񷢲�״̬����
CMD_START(NDBC_ChangFamilyQuestIssueState)
	DWORD dwFamilyID;
	bool bIssueFlag;
CMD_END

// ��ʼ����������
CMD_START(NDBC_LoadFamilyQuest)
CMD_END

CMD_START(NDBS_LoadFamilyQuest)
	DWORD dwErrorCode;
	bool bLastOne; 
	INT nNum;
	tagFamilyQuestLoadData  QuestData[1];
CMD_END

// ��ʼ����������
CMD_START(NDBC_SaveRoleFamilyQuestRewarded)
	DWORD dwFamilyID;
	DWORD dwRoleID;
CMD_END

// �������������Ϣ
CMD_START(NDBC_RequestFamilyQuest)
	DWORD dwFamilyID;
CMD_END

CMD_START(NDBC_DeleteFamilyQuest)
	DWORD dwFamilyID;
CMD_END

#pragma pack(pop)
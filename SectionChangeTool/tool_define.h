#pragma once

#include "stdafx.h"
#include "../ServerDefine/role_data_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/vcard_define.h"

#define SERVICE_CONFIG_INI		"config"		// �����ļ�������
#define MIBAO_LEN				240				// �ܱ����̶�

//extern CSectionChangeToolDlg* g_pDlg;

//��ɫ����
struct tagTWrap_Loong_RoleData
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	BOOL  bRemoveFlag;
	INT64 n64WorkedLevel;						//�ӳɹ��ĵȼ�ֵ�����ڵȼ�����
	CHAR  szRemoveTime[X_DATATIME_LEN + 1];		// del time
	tagRoleDataLoad	stOthers;

	tagTWrap_Loong_RoleData()
	{
		bRemoveFlag = FALSE;
		dwRoleID = GT_INVALID;
		dwAccountID = GT_INVALID;
		memset(szRemoveTime ,0 ,X_DATATIME_LEN + 1);
	}
};

// ��Ʒ
struct tagTWrap_Loong_Item
{	
	tagItem stOthers;
};

// װ��
struct tagTWrap_Loong_Equip
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_Equip()
	{
		n64SerialNum = GT_INVALID;
	}
};

// ����
struct tagTWrap_Loong_VisitingCard
{
	TCHAR tcHeadUrl[LEN_MAX_URL];
	TCHAR tcSignature[LEN_MAX_SIGNATURE];

	TCHAR tcNameHistory[500];

	tagVCardData stOthers;

	tagTWrap_Loong_VisitingCard()
	{
		memset(tcHeadUrl,0,LEN_MAX_URL);
		memset(tcSignature,0,LEN_MAX_SIGNATURE);
		memset(tcNameHistory,0,500);
	}
};

//����
struct tagTWrap_Loong_Skill
{

	DWORD dwRoleID;
	tagSkillSave stOthers;

	tagTWrap_Loong_Skill()
	{
		dwRoleID = GT_INVALID;
	}
};
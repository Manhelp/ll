//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: master_apprentice.h
// author: twan
// actor:
// data: 2010-01-20
// last:
// brief: ʦͽ�����ݿ⽻���ṹ����
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ʦͽ����ö��
//-----------------------------------------------------------------------------
enum EOneMasterApprenticeData
{
	EOMAD_JingWu,					// �������
	EOMAD_QinWu,					// �������
	EOMAD_LevelUpBonus,				// ͽ����������
	EOMAD_RewardNum,				// ��ʦ��������
	EOMAD_Apprentice,				// ͽ������
};

//-----------------------------------------------------------------------------
// �������ݿⷽʽ
//-----------------------------------------------------------------------------
enum EMasterApprenticeModifyMode
{
	EMAMM_Set,						// �滻
	EMAMM_Mod,					    // ����
};

//-----------------------------------------------------------------------------
// ��һ��ʦͽ����
//-----------------------------------------------------------------------------
struct tagOneMAData2DB
{
	EOneMasterApprenticeData		eType;
	EMasterApprenticeModifyMode		eMode;
	BYTE							byData[4];

	tagOneMAData2DB& operator = (tagOneMAData2DB& other)
	{
		eType = other.eType;
		eMode = other.eMode;
		memcpy(byData, other.byData, sizeof(BYTE) * 4);
		return *this;
	}
};

//-----------------------------------------------------------------------------
// �����ݿ�ṹ
//-----------------------------------------------------------------------------
struct tagMasterApprentice2DB
{
	DWORD				dwID;				// ����������ʦ��RoleID��
	INT					nJingWuPoint;		// �������
	INT					nQinWuPoint;		// �������
	INT					nLevelUpBonus;		// ͽ����������
	INT					nRewardNum;			// ��ʦ��������
	tagApprenticeData	sApprentice;		// ͽ��ID
};

struct tagForceBreakOutApprentice
{
	DWORD			dwApprenticeID;			// ͽ��ID
	DWORD			dwMasterID;				// ʦ��ID
	tagDWORDTime	BreakoutInsertTime;		// ͽ������ǿ������ʦ��ʱ���ʱ��
};


CMD_START(NDBC_LoadMasterApprenticeData)
	DWORD						dwMasterID;
CMD_END

CMD_START(NDBS_LoadMasterApprenticeData)
	tagMasterApprentice2DB		sMasterApprentice;
CMD_END

CMD_START(NDBC_SaveAllMasterApprenticeData)				// �������е�ʦͽ����
	tagMasterApprentice2DB		sMasterApprentice;
CMD_END

CMD_START(NDBC_SaveOneMasterApprenticeData)
	DWORD						dwMasterID;
	tagOneMAData2DB				sOneMAData;
CMD_END

CMD_START(NDBC_InsertMasterApprenticeData)
	tagMasterApprentice2DB		sMasterApprentice;
CMD_END

CMD_START(NDBC_DeleteAnApprentice)
	DWORD						dwMasterID;				// ������ID
	DWORD						dwApprenticeID;			// ͽ��ID
CMD_END

CMD_START(NDBC_LoadMasterApprenticeCount)
	DWORD						dwRoleID;
CMD_END

CMD_START(NDBS_LoadMasterApprenticeCount)
	DWORD						dwRoleID;
	DWORD						dwMasterID;
	DWORD						dwApprenticeCount;
CMD_END

CMD_START(NDBC_LoadForceBreakOutApprenticeList)
CMD_END

CMD_START(NDBS_LoadForceBreakOutApprenticeList)
	DWORD						dwForceBreakOutApprenceCnt;
	tagForceBreakOutApprentice	sForceBreakOutApprentice[1];
CMD_END

CMD_START(NDBC_InsertForceBreakOutApprentice)
	tagForceBreakOutApprentice sForceBreakOutApprentice;
CMD_END

CMD_START(NDBC_RemoveForceBreakOutApprentice)
	tagForceBreakOutApprentice sForceBreakOutApprentice;
CMD_END

CMD_START(NDBC_ModifyApprenticeJingWuPoint)
	DWORD	dwApprenticeID;
	DWORD	dwJingWuPoint;
CMD_END

//-----------------------------------------------------------------------------
// ��һ��ʦͽ���ݣ���tagMasterApprentice2DB���������ݿ�updateͨ��ģ��ʱ�ã�
//-----------------------------------------------------------------------------
struct tagOneMASaveData
{
	tagMasterApprentice2DB		loadData;
	tagOneMAData2DB				oneMAData;
};

#pragma pack(pop)
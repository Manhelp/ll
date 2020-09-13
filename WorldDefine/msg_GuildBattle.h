//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_GuildBattle.h
// author: chdeng
// actor:
// data: 2009--01--25
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- ��ս���
//-----------------------------------------------------------------------------
#pragma once
#include "guild_define.h"

#pragma pack(push, 1)

enum EGuildErrorCode
{
	EGEC_Success = 0,
	EGEC_NPC_NotExist	= 1,	// ս�����npc������
	EGEC_NPC_DisFar	= 2,	// ��NPC����̫Զ
	EGEC_NPC_TypeMismatch = 3,	// npc���Ͳ�ƥ��
	EGEC_DailyReward_LevelLimit	= 4,	// ��ȡÿ�ս����ȼ�����
	EGEC_DailyReward_GuildPosLimit	= 5,	// ��ȡÿ�ս�����ְλ��������
	EGEC_DailyReward_AlreadyTaken		= 6,	// �Ѿ���ȡ��ÿ�ս���
	EGEC_DailyReward_CannotFindProto = 7,	// û���ҵ�����ԭ��
	EGEC_DailyReward_BagFull = 8,	// ��������
};

// ͬ��������������
CMD_START(NS_SyncFlagAttack)
	EGodMiracle eGodID;					// ������
	EFlagType eFlagType;				// ָ������
	EGuildCapType eGuildCapType;		// ��������
	INT nFlagPosX;
	INT nFlagPosY;
	INT nFlagPosZ;
CMD_END

CMD_START(NC_ApplyCapGodMiracle)		// ����������
	BOOL				bAddBid;		// �Ƿ���׷�Ӿ���
	EGodMiracle			dwGodID;		// Ҫ�������ID
	DWORD				dwMoney;		// ��ҳ��ݵĽ�Ǯ
CMD_END

CMD_START(NS_ApplyCapGodMiracle)		// ���������񼣷���
	DWORD				dwErrorCode;
	EGodMiracle			dwGodID;		// Ҫ�������ID
	DWORD				dwMoney;		// ��ҳ��ݵĽ�Ǯ
CMD_END

CMD_START(NC_GetAllGodInfo)				// ��ȡ�񼣵�������Ϣ 
CMD_END


struct tagGuildInfo
{
	DWORD dwGuildID;	// ����ID
	int   nLevel;		// ���ɵȼ�
	INT   nNum;			// ��Ա���� 
	tagGuildInfo() : dwGuildID(GT_INVALID), nLevel(0), nNum(0){}
	tagGuildInfo& operator = (tagGuildInfo& sGuildInfo)
	{
		dwGuildID = sGuildInfo.dwGuildID;
		nLevel = sGuildInfo.nLevel;
		nNum = sGuildInfo.nNum;
		return *this;
	}
};

struct tagGodInfo
{
	EGodMiracle			eGodType;				// ������
	EGodMiracleState	eGodState;				// �񼣵�״̬
	//DWORD				dwDefendGuild;			// �����İ���ID(�޷������ɼ�Ϊ0���޷���������ô�Ͳ������������)
	tagGuildInfo        sDefenceGuild;			// �����İ���(�޷�������dwGuildIDΪ0���޷���������ô�Ͳ������������)
	DWORD				dwStarttime;			// �������ɶ�ȡ��ʱ��ʱ��
	tagGuildInfo        sCapGuild;				// ��������
	DWORD				dwApplyMoney;			// ��������ľ���
	DWORD				dwBidFund;				// �ͻ��˹��ᵱǰ���ۣ�����ǰ��λ����0��
	DWORD				dwAttAlliance[MAX_ATTACK_ALLIANCE_GUILD];
	DWORD				dwDefAlliance[MAX_DEFANCE_ALLIANCE_GUILD];

	tagGodInfo():dwStarttime(0),dwApplyMoney(0)
	{}
};


CMD_START(NS_GetAllGodInfo)				// ���������񼣵�������Ϣ 
	BYTE		byGodNum;				// �񼣵�����
	tagGodInfo	sTagInfo[1];			// �񼣵���Ϣ
CMD_END


// ȡͬ���б�
CMD_START(NC_GetAllianceList)
	EGodMiracle			eGodID;		// ս��ID
CMD_END

CMD_START(NS_GetAllianceList)
	EGodMiracle			eGodID;		// ս��ID
	EGuildCapType		eGuildCapType;
	DWORD				dwGuildCnt;			// ���Ϊ0���������Ժ���
	tagGuildInfo		sGuildInfo[1];
CMD_END

// ս��ͬ�˹��ᱨ��
CMD_START(NC_ApplyForAlliance)
	EGodMiracle			dwGodID;		// ս��ID
	EGuildCapType		eGuildCapType;
CMD_END

CMD_START(NS_ApplyForAlliance)
	DWORD				dwErrorCode;
	EGodMiracle			dwGodID;		// ս��ID
CMD_END

// ȷ��ͬ��
CMD_START(NC_ConfirmAllianceGuild)
	EGodMiracle			dwGodID;		// ս��ID
	DWORD				dwGuildID;
CMD_END

CMD_START(NS_ConfirmAllianceGuild)
	DWORD				dwErrorCode;
	DWORD				dwGuildID;
	EGodMiracle			dwGodID;		// ս��ID
	EGuildCapType		eGuildCapType;
CMD_END

struct tagWinOrLoseRoleInfo
{
	DWORD dwRoleID;
	int   nKillTime;
	tagWinOrLoseRoleInfo():dwRoleID(0),nKillTime(0)
	{}
};
// �������������, ���������������ͻ���
CMD_START(NS_GodReward)
	bool bWin;							// �Ƿ��ҷ���ʤ

	int  nKillTime;						// ��ɱ����
	DWORD dwLiveTime;					// ��Ծʱ��
	int  nExploit;						// ���ɹ�ѫ
	int  nContribute;					// ���ɹ���
	
	tagWinOrLoseRoleInfo	sRoleIndo[5];// ����ǰ��λ�����
CMD_END

// ����������
CMD_START(NS_EnterGodArea)
	DWORD dwAttackGuildID;		// ��������ID
	DWORD dwDefenceGuildID;		// ��������ID
	INT   nAttackGuildMember;	// �������ɳ�Ա��Ŀ
	INT   nDefenceGuildMember;	// �������ɳ�Ա��Ŀ
CMD_END

// �뿪������
CMD_START(NS_LeaveGodArea)
CMD_END

// ���������ڵ�״��ˢ��
CMD_START(NS_GodPersonalState)
	INT   nKillTime;			// ���˻�ɱ����
	DWORD dwLiveTime;			// ���˻�Ծʱ��
	INT   nExp;					// ���þ���
	INT   nMoney;				// ���ý�Ǯ
	INT   nGuildExploit;		// ���ɹ�ѫ
	INT   nGuildContribute;		// ���ɹ���
CMD_END

// �򱻶����İ��ɰ������ͱ�������Ϣ
CMD_START(NS_InsteadInfo)
	EReturnMoneyReason	eReturnMoneyReason;	// �Ƿ��ǽ���Ȩȷ�ϵ��¾���ʧ��
	EGodMiracle			dwGodID;			// Ҫ�������ID
CMD_END

//////////////////////////////////////////////////////////////////////////
// ����ս������
//////////////////////////////////////////////////////////////////////////
// ����ǰռ���ս������
CMD_START(NC_GuildOccupyedNum)
CMD_END

CMD_START(NS_GuildOccupyedNum)
	INT		nNum;				// ��ǰ����ռ��ս��������
	bool		bAlreadyTake;	// ����Ľ����Ƿ��Ѿ���ȡ��
CMD_END

// ��ȡ����
CMD_START(NC_TakeGuildWarReword)
	DWORD dwNPCID;	// NPCid
CMD_END

CMD_START(NS_TakeGuildWarReword)
	DWORD dwErrorCode;
CMD_END

// ս��ռ������ͻ�Ծʱ�佱������ʾ
CMD_START(NS_GetBattleReward)
	EBattleFieldRewardType eRewardType;
	DWORD dwPointOrTime;
	DWORD dwExp;
	DWORD dwSilver;
CMD_END

#pragma pack(pop)
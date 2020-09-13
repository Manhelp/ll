//--------------------------------------------------------------------------------
//!\file quest_board.h
//!\author zhangrong
//!
//!\date 2010-06
//! last 2010-06
//!
//!\brief �����ͷ�ļ�
//!
//--------------------------------------------------------------------------------
#pragma once

#define QUEST_BOARD_MIN_CAPACITY	2				// ��������ʾ����С������
#define QUEST_BOARD_MAX_CAPACITY	6				// ��������ʾ�����������
#define BOARD_QUEST_LIMIT_LEVEL		10				// ��ȡ������������͵ȼ�
#define GUILD_QUEST_LIMIT_LEVEL		40				// ��ȡ����������������͵ȼ�
#define YELLOW_QUEST_LIMIT_LEVEL	50				// ��ȡ��֮�л�������������͵ȼ�
const UINT16 QUEST_PANEL_START_TASK_ID	= 10001;	// �������ʼ����ID
const UINT16 QUEST_PANEL_END_TASK_ID	= 10012;	// ������ֹ����ID
const UINT16 YELLOW_LIST_START_TASK_ID = 10007;	// ��֮�л��������ʼ����ID
const DWORD REFRESH_QUEST_PANEL_ITEM_ID1 = 4500002;	// ������
const DWORD REFRESH_QUEST_PANEL_ITEM_ID2 = 3070056;	// ��������
const DWORD REFRESH_QUEST_PANEL_FIRST_ID = 3070056;

#define BOARD_QUEST_INVEST_NUM		4
#define BOARD_QUEST_MONSTER_NUM		4
#define BOARD_QUEST_NPC_NUM			2
#define BOARD_QUEST_GATHER_NUM		3
#define BOARD_QUEST_REWARD_NUM		5

#define YELLOWLIST_ACCEPT_TIMES_LIMIT 10		// ��֮�л������ÿ��ֻ�ܽ�ȡ10��
#define RewardBoardQuestLvlIdx					30   //--��¼����������Ͱ�ĵȼ�
#define GuildBoardQuestLvlIdx						31   //--��¼������Ϲ��������ĵȼ�                             
#define YellowListBoardQuestLvlIdx				32   //--��¼���������֮�л������ĵȼ�                      
#define TodayYellowListQuestTimesIdx		33   //--��¼��ҵ����ȡ��֮�л����������Ĵ���   

// ���������Ʒ��ö��
enum EBoardQuestQuality
{
	EBQQ_White		= 0,	// ��
	EBQQ_Yellow		= 1,	// ��
	EBQQ_Green		= 2,	// ��
	EBQQ_Blue		= 3,	// ��
	EBQQ_Orange		= 4,	// ��
	EBQQ_Purple		= 5,	// ��
};

#pragma pack(push)
#pragma pack(1)
// ����״̬
enum EBoardQuestState
{
	EQS_Null		= -1,

	EQS_NotAccept	= 0,	// δ��ȡ
	EQS_HaveAccept	= 1,	// �ѽ�ȡ
	
	EQS_End			= 2,
};

// ���ݿ�洢�ṹ
struct tagBoardQuestSaveData
{
	DWORD		dwRoleID;
	UINT16		u16QuestID;
	UINT16		u16BoardID;
	BYTE			byType;
	EBoardQuestState	eState;
};
#pragma pack(pop)

struct tagBoardQuestTargetMonster
{
	DWORD		dwTargetMonsterID_A;
	DWORD		dwTargetMonsterID_B;
	INT			byTargetMonsterNum;
};

struct tagBoardQuestObject
{
	DWORD		dwID;
	INT			byNum;
};

struct tagBoardQuestReward
{
	DWORD		dwID;
	BYTE		byNum;
	BYTE		byQuality;
};

// ���������������Խṹ
struct tagBoardQuestProto
{
	UINT16						u16ID;			// ����ID
	BYTE						byType;			// ��������
	EQuestTargetType			eTarget;		// ����Ŀ������
	BYTE						byLevel;		// ����ȼ�
	EBoardQuestQuality			eQuality;		// ����Ʒ��

	// ������������
	DWORD						dwInvestItem[BOARD_QUEST_INVEST_NUM];

	// �������ɱ����
	tagBoardQuestTargetMonster	TargetMonster[BOARD_QUEST_MONSTER_NUM];

	// ������Ի�NPC
	DWORD						dwTalkToNPC[BOARD_QUEST_NPC_NUM];

	// ������ɼ���Դ
	tagBoardQuestObject			GatherItem[BOARD_QUEST_GATHER_NUM];
	
	// ��������Ʒ
	tagBoardQuestReward			RewardItem[BOARD_QUEST_REWARD_NUM];

	// ����������
	INT							nRewardExp;

	// ��������Ǯ
	INT							nRewardMoney;
};
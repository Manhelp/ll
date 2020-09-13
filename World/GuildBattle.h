//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: GuildBattle.h
// author: chdeng
// actor:
// data: 2009-01-26
// last:
// brief: ��ս
//-------------------------------------------------------------------------------
#pragma once
#include "activity_mgr.h"
#include "role_mgr.h"
#include "guild_mgr.h"
#include "guild.h"
#include <hash_map>
#include <list>
#include "world.h"

#include "../WorldDefine/time.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/guild_define.h"

#define GUILBATTLE_ACTIVITY_ID 25

enum EBroadcastType
{
	EBT_Start = 0,

	EBT_Attack = 1,							// ������
	EBT_AttackAlliance = 2,				// ����ͬ��
	EBT_DefendAlliance = 3,			// ����ͬ��

	EBT_End,
};

//  �������������һ���
struct tagGuildMaxScoresPlayer
{
	DWORD dwGuildID;
	DWORD dwRoleID;
	DWORD dwScores;
	tagGuildMaxScoresPlayer():dwGuildID(0),dwRoleID(0),dwScores(0)
	{}
};

// �����ɫ��Ϣ
struct tagRevieRoleInfo
{
	DWORD dwRoleID;
	DWORD dwCurTick;
	tagRevieRoleInfo():dwRoleID(0),dwCurTick(0)
	{}
};


//����д��������Ч�����ظ�����
// ��ɱ���
struct tagBeKillerInfo
{
	DWORD dwBeKilledRoleID;
	DWORD dwTime;
	tagBeKillerInfo():dwBeKilledRoleID(0),dwTime(0) { Init(); }

	BOOL Init()
	{
		dwBeKilledRoleID = 0;
		dwTime			 = 0;
		return TRUE; 
	}
public:
	void  SetRoleID(DWORD inRoleID) { dwBeKilledRoleID = inRoleID;}
	DWORD GetRoleID()				{ return dwBeKilledRoleID;}

	void  SetTime(DWORD inTime)		{dwTime = inTime;}
	DWORD GetTime()					{ return dwTime;}
};

// ս��ռ���������
const INT32 CAP_POINT_REWARDS[] = {
// ����     ���齱������ ��Ǯ��������
	30,      3,                 1,
	60,   	 6,                 2,
	120,     12,                4,
	180,     20,                6,
	240,     30,                8,
	300,     40,                10
};

// ��Ծʱ�佱��
const INT32 ACTIVE_TIME_REWARDS[] = {
// ��Ծʱ��    ���齱������   ��Ǯ��������
	15,              10,       2,
	30,              10,       2,
	45,              10,       2
};

// ���ջ�������
const INT MAX_MIRACLES_POINTS = 5000;

#define CAL_FINIALPOINTS(active_time, kill_times) ((active_time)*20+(kill_times)*20)

// �����Ϣ
struct tagGodRoleInfo
{
public:
	DWORD	dwRoleID;				// ��ɫID
	DWORD	dwKillTimes;			// ��ɱ����
	DWORD	dwActiveTime;			// ��Ծʱ��
	BOOL	bIsInGodMiracleArea;	// �Ƿ���������
	
	BOOL    bIsBeCheck;

	DWORD	dwGodID;				// ���ڵ�������

	BOOL    bChange;				// ��ɱ�ͻ�Ծʱ���Ƿ�ı�

	BOOL	nKillStopFalg;		// 60նֹͣ��־

	INT   nExp;					// ���þ���
	INT   nMoney;				// ���ý�Ǯ

	tagBeKillerInfo	sBeKilledRole[3];

	tagGodRoleInfo():dwRoleID(0),dwKillTimes(0),dwActiveTime(0),bIsInGodMiracleArea(FALSE),
					 bIsBeCheck(0),dwGodID(-1),bChange(FALSE),nKillStopFalg(FALSE),nExp(0),nMoney(0)
	{}
	BOOL Init()
	{
		dwRoleID	= 0;
		dwKillTimes = 0;
		dwActiveTime= 0;
		bIsInGodMiracleArea =FALSE;
		bIsBeCheck  = 0;
		dwGodID		= -1;
		bChange     = 0;
		nKillStopFalg	= FALSE;
		nExp = 0;
		nMoney = 0;
	}

public:
	void   SetGodID(DWORD inGodID)	{ dwGodID = inGodID;}
	DWORD  GetGodID()					{ return dwGodID;}	

	void  SetRoleID(DWORD inRoleID)			{ dwRoleID = inRoleID; }
	DWORD GetRoleID()						{ return dwRoleID; }

	void  ModKillTimes(DWORD inKillTimes)	{ dwKillTimes += inKillTimes;}
	DWORD GetKillTimes()					{ return dwKillTimes;}

	void  SetActiveTime(DWORD inActiveTime)	{ dwActiveTime = inActiveTime;}
	DWORD GetActiveTime()					{ return dwActiveTime;}
	BOOL  GetAreaFlag()						{ return bIsInGodMiracleArea ;}

	void  SetAreaFlag(BOOL inFlag)			{ bIsInGodMiracleArea = inFlag;}
	BOOL  SetBeKilledRole(DWORD inBeKilledRoleID, DWORD dwGuildID, BOOL bIsTeamFlag = FALSE);
	
	void  SetBeCheck(BOOL bType)			{ bIsBeCheck = bType;}
	BOOL  GetBeCheck()						{ return bIsBeCheck; }

	VOID IncExp(INT nAddExp)	{ nExp += nAddExp; }
	VOID IncMoney(INT nAddMoney) 	{ nMoney += nAddMoney; }

public:
	// ���ͻ�Ծʱ��ͻ�ɱ�����ıߵ���ʱ������Ϣ
	void SendChangeActiveTimeAndKillTime();

	VOID  UpdateMin();
	VOID Update();
	VOID GiveActiveTimeReward(DWORD dwActiveTime, DWORD dwLastTime);
	static VOID GiveReward(DWORD	dwGodID, EBattleFieldRewardType eRewardType, DWORD dwRoleID, DWORD dwExpRewardParam, DWORD dwSilverRewardParam, DWORD dwTimeOrPoint );
	VOID UnsetState(EState eState);
};

class CGodMiracleMgr;

// ��
class CGodMiracle
{
public:
	CGodMiracle();
	~CGodMiracle();

	BOOL Init();
	void Uninit();
	void Refresh();

public:
	void  SetCurState(EGodMiracleState eCurState);
	EGodMiracleState GetCurState()						{ return m_eCurState;}
	void  SetGodID(DWORD dwGodID)
	{
		m_dwGodID = dwGodID;
		switch (m_dwGodID)
		{
		case EGM_NH:// ����ڣս��
			m_bGodMiracleOpen = g_world.IsTianShenZhongOpen();
			break;

		case EGM_XY:// �߻�֮��ս��
			m_bGodMiracleOpen = g_world.IsQiHuaZhiHaiOpen();
			break;

		case EGM_SM:// �����ѹ�ս��
			m_bGodMiracleOpen = g_world.IsBenLeiLieGuOpen();
			break;
		}
	}
	DWORD GetGodID()						{ return m_dwGodID;}
	DWORD GetCapGodID()						{ return m_dwCapGodMiracle;}
	DWORD GetCapStartTime()					{ return m_dwCapStartTime ;}
	void  SetApplyID(DWORD dwApplyID)		{ m_dwAttackGuild = dwApplyID;}
	DWORD GetApplyID()						{ return m_dwAttackGuild;}  
	void  SetApplyMoney(DWORD dwApplyMoney) { m_dwAttackBidFund = dwApplyMoney;}
	DWORD GetApplyMoney()					{ return m_dwAttackBidFund;}
	void  ModPoint(DWORD dwDAndCFlag, DWORD dwPoint);

	DWORD GetPoint(DWORD dwDAndCFlag)
	{
		// �ط�
		if ( 1 == dwDAndCFlag) return m_dwDefendPoint;
		// ����
	    if ( 2 == dwDAndCFlag)  return m_dwCapturePoint;

		return 0;
	
	}

	void  SetCapGodID(DWORD dwCapID)		{ m_dwCapGodMiracle = dwCapID;}
	void  SetCapStartTime(DWORD dwStartToime){ m_dwCapStartTime = dwStartToime;}


	tagGodRoleInfo* GetGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID);
	tagGodRoleInfo* GetGodRoleInfo(DWORD dwRoleID);

	BOOL	PushGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID);

	BOOL	RemoveGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID);

	// �õ�һ����ɱ�����������
	tagGodRoleInfo*	GetNextMaxKillTimesPlayer(DWORD dwDefendOrCaptureFlag);

	// �������仯ʱ����������ҷ��͵�ǰ�Ķ�ս���
	VOID SendCurDAndCInfoToAllGodplayer();

public:
	// �������������
	DWORD HandleApplyCapGodMiracle(DWORD dwRoleID, DWORD& dwMoney, BOOL bAddBid);
	
	// �õ��񼣵�״̬(����ʱ��+����ռ�����)
	EGodMiracleState GetGodMiracleState();
	
	// ÿ���Ӹ���	
	VOID UpdateMin();
	
	// ��ʱ����
	VOID Update();

	// �������������ҵĹ��׺͹�ѫ(1 = �ط�Ӯ 2--�ط���)
	VOID CalAllRoleContributionAndExploit(DWORD dwWinOrLoseType); 

	// ������ռ����Ϣ���浽���ݿ�
	VOID SaveCapTureInfoToActive();

	// �õ����Ļ������
	tagGodRoleInfo* GetMaxScoresPlayer(EGuildCapType eGuidType);

	// �õ����ֳ���ĳһ��ֵ�����
	BOOL GetSomeplayerOverFixedScores(std::vector<tagGuildMaxScoresPlayer> &lstMaxScoresPlayer);
	
	// ��ȫ�ֻ������,�õ���Ҫ��������
	BOOL GetAllCachePlayer();

	// �ж����еİ��ɳ�Ա�Ƿ��������򲢴���һ���¼�
	VOID CheckAllGuildMemberIsInGodArea();

	// ������������г�Ա���͸�����״̬��Ϣ
	VOID BroadcastAllGoodPlayer();

	static BOOL IsNeedReward(EBattleFieldRewardType eBattleFieldRewardType, DWORD dwPointOrTime, DWORD dwLastPointOrTime, DWORD& dwExpReward, DWORD& dwSilverReward);
	VOID GivePointReward(DWORD dwDAndCFlag, DWORD dwPoint, DWORD dwLastPoint);
	VOID GiveGuildMemberReward(DWORD dwGuildID, DWORD dwExpRewardParam, DWORD dwSilverRewardParam, DWORD dwTimeOrPoint);

	typedef stdext::hash_map<DWORD,tagGodRoleInfo>	MAPROLEINFO;
	typedef TMap<DWORD, tagAllianceGuildInfo*>	AllicanceGuildMap;
protected:
	// ��ʤ�����㲥ÿλ��ս����Ϣ
	VOID BroadcastGodCaptureResultToAllPlayer(DWORD dwWinOrLoseType);
	void AddWinnerBuff( Role* pRole );
	VOID LogBattleField();
#if 0
	BOOL CalDefendContributionAndExploit(DWORD dwWinOrLoseType);
	BOOL CalCaptureContributionAndExploit(DWORD dwWinOrLoseType);
#endif

private:
	BOOL IsBidEnough(DWORD dwMoney);
	VOID ResetSignUpList(DWORD dwGuildID, DWORD dwMoney);
	VOID GiveMoneyBack2Guild(DWORD dwGuildID, DWORD dwMoney, EReturnMoneyReason eReturnMoneyReason);
	VOID ConfirmSignUp();

public:
	VOID GiveMoneyBack2FailedGuild();
	INT32 GetFirstMiracleIdxInSignUpList(DWORD dwGuildID, DWORD& dwBidFund);
	DWORD CanGuildDismiss(DWORD dwGuildID);
	DWORD Add2AllianceGuildMap(	DWORD			dwGuildID,				// ����ID
								EGuildCapType	eAllianceForceType,		// ���ػ����
								BOOL			bConfirmed,				// �Ƿ�ȷ����
							    BOOL			bSave2DB = TRUE);
	DWORD ConfirmAllianceGuild(DWORD dwGuildID);

	VOID ClearAllianceGuildMap();

	VOID SendAllianceList2Client(EGodMiracle eGodID, Role* pRole);

	DWORD Add2SignUpGuildInfo(DWORD dwGuildID, DWORD dwBidFund);

	AllicanceGuildMap& GetAllianceGuildMap()
	{
		return m_mapAllianceGuild;
	}

	EGuildCapType GetAllianceCapType(DWORD dwGuildID);
	EGuildCapType GetCapType(DWORD dwGuildID, BOOL &bAlliance );

	VOID SendAllianceGuildMsg(DWORD	dwMsgInfoID, DWORD	dwAllianceType);

	VOID UnsetStateOfAllRoleInBattleField(EGuildCapType eGuildCapType, EState eState);
	BOOL IsInAllianceGuild(DWORD dwGuildID);
	VOID SendJoinBattleMsg(Guild* pGuild, EBroadcastType eBroadcastType = EBT_Attack);

	BOOL GetAllianceConfirmed(DWORD dwGuildID);
private:
	DWORD m_dwGodID;				// ��ID
	DWORD m_dwCapGodMiracle;		// ռ���񼣵İ���ID
	DWORD m_dwCapStartTime;			// ռ����ɵĿ�ʼʱ�� 
	DWORD m_dwAttackGuild;			// ��ù���Ȩ�İ���ID
	DWORD m_dwAttackBidFund;		// ��ù���Ȩ�İ��ɳ���Ǯ
	
	DWORD m_dwDefendPoint;			// ���ط�����
	DWORD m_dwCapturePoint;			// ����������
	BOOL  m_PointChangeFlag;		// ���ָı��־  

	EGodMiracleState m_eCurState;				// ��ǰ״̬

	tagSignUpGuildInfo		m_SignUpGuildInfo[MAX_COMPETE_BID];	// ���Ὰ�۱�����Ϣ
	AllicanceGuildMap		m_mapAllianceGuild;					// ͬ�˹��ᱨ����Ϣ

	// �������ط��Ľ�ɫ�б�
	MAPROLEINFO						m_mapDefendGodRoleInfo;
	MAPROLEINFO						m_mapCaptureGodRoleInfo;

	BOOL m_bGodMiracleOpen;
};


// �񼣹�����
class CGodMiracleMgr
{
friend class CGodMiracle;

public:
	CGodMiracleMgr();
	~CGodMiracleMgr();
	
	BOOL Init();
	void Uninit() {};
	
	static CGodMiracleMgr* GetInst();

	// ��ָ���������������Ϣ
	BOOL LoadGodMiracleInfoFormActivity(CActivity* pActivity);

	// �������
	VOID UpdateRevie();

	// ÿ���Ӹ���
	VOID UpdateMin();

	// ��ʱ����
	VOID Update();

public:
	// �鿴ĳ����ռ�м�����
	DWORD GetGuildDefendGodMiracle(DWORD dwGuildID);

	// �鿴ĳ����ȡ���˼���ս���Ľ���Ȩ
	DWORD GetGuildAttackGodMiracle(DWORD dwGuildID);

	// �õ���
	CGodMiracle* GetGodMiracle(EGodMiracle eGodType);

	// �õ���ҵĻ�ɱ����
	 DWORD GetOnePleyerKillTimes(DWORD dwRoleID);

	// �õ���ҵĻ�Ծʱ
	 DWORD GetonePlyerActiveTime(DWORD dwRoleID);
	
	 // �õ�������񼣵İ��ɵ����������(������ߵ�)
	 tagGodRoleInfo* GetCaptureGuildMaxScoresPlayer(DWORD dwGodID, EGuildCapType eGuidType);

	 BOOL GetGuildMaxScoresPlayer(std::vector<tagGuildMaxScoresPlayer> &lstMaxScoresPlayer);

	// ���Ӹ����ɫ
	BOOL AddRevieRole(DWORD dwRoleID);

	// ����
	VOID ReviveRole(DWORD dwRoleID);
	
	// �ж�������Ƿ��ڲ�ͬ�İ��ɲ�����ͬһ��
	BOOL IsCaptureGodAndDifferentGuild(Role* pRole, Role* pAnotherRole);
	
	// �ж�����Ƿ��������񼣰�����
	BOOL IsInCaptureGodMiracleGuild(Role* pRole);

	// �鿴��ǰ��ɫ�İ����Ƿ��������񼣵İ��ɵķ�Χ��
	CGodMiracle* IsInCaptureGodRange(DWORD dwGuildID);

	// �㲥������������������
	VOID BroadcastAllGoodPlayer();

	/// ������״̬
	VOID SetGodDefendAndCaptureState(int nGodIndex);

	/// ��ɫ��ɱ������ӵĴ���
	VOID ModteamInfoForkillRole(DWORD dwGodID, DWORD dwSrcID, DWORD dwTargetID);

// Dealwith Msg
	// ���������񼣵���Ϣ
	DWORD HandleSendAllGodInfoToPlayer(DWORD dwRoleID);

	// ������������г�Ա���͸�����״̬��Ϣ
	DWORD UpdateAllGodInfoToAllPlayer();

	BOOL IsGodMiracleTime();
	BOOL IsAllianceSignUpTime();
	DWORD CanGuildDismiss(DWORD dwGuildID);

	EGodMiracle GetEGodMiracle(DWORD dwMapID);

	DWORD Add2AllianceGuildMap(	EGodMiracle		eGodID,					// ս��ID
								DWORD			dwGuildID,				// ����ID
								EGuildCapType	eAllianceForceType,		// ���ػ����
								BOOL			bConfirmed);			// �Ƿ�ȷ����

	DWORD ConfirmAllianceGuild(	EGodMiracle		eGodID,					// ս��ID
								DWORD			dwGuildID);				// ����ID

	VOID ClearAllianceGuildMap(EGodMiracle eGodID);

	VOID SendAllianceList2Client(EGodMiracle eGodID, Role* pRole);

	// ���뾺�۱�����ͬ�˹��ᱨ��
	VOID LoadSignUpInfo();

	// ����ȷ��
	VOID ConfirmBidFundSignUp();

	// �ж����������ǲ��ǵжԹ�ϵ
	BOOL IsGuildInSameAlliance(DWORD dwMapID, DWORD dwGuildID1, DWORD dwGuildID2);

	// ռ��ս������Ĺ����ʽ�ÿ��22:00�������
	VOID AddCapGuildFundEveryday();

	VOID ResetMergedData()
	{
		m_bHaveMergedData = FALSE;
		m_listMergedCapGuildID.Clear();
	}

public:
	typedef TMap<DWORD, tagRevieRoleInfo*>			MAPREVIEROLEINFO;
private:
	CGodMiracle	m_VecGodmiracle[EGM_MAX];
	DWORD m_dwBattleFieldMapID[EGM_MAX];
	BOOL m_bHaveMergedData;		// �Ƿ��кϷ����ߺϲ��Ļ����
	TList<DWORD> m_listMergedCapGuildID;
	Mutex					m_mutexRevive;	
	MAPREVIEROLEINFO		m_mapRevieRole;

	BOOL	m_bBidFundSignUpConfirm;	// ���۱���ȷ�Ͻ��������ǲ����Ѿ�ȷ�Ϲ���
	BOOL	m_bAddCapGuildFund;	// ռ��ս������Ĺ����ʽ�ÿ��22:00�������
};
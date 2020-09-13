#pragma once

#include "ActivityProtoData.h"
#include "..\WorldDefine\role_att.h"
#include "../WorldDefine/quest.h"

struct tagNS_RoleInitOk;
// ǰ������
struct tagNS_ActivityTaskState;
struct tagNS_GetRoleInitState_DailyCompleteQuest;
struct tagNS_XunLianChangRemainTime;
class FestivalActivityPromptFrame;

class ActivityMgr
{

public:
	enum EActivityState
	{
		EAS_Start,		// ��ʼ��
		EAS_End, 		// ������
		EAS_StartSoon,	// ������ʼ
		EAS_EndSoon		// ��������
	};
	
	enum ESortType
	{
		EST_None,
		EST_SortByTime,
		EST_SortByName,
		EST_SortByLevel,
		EST_SortByNpc,
		EST_SortByAcceptTime,
		EST_SortByMonthAndDay
	};
	
	typedef std::vector<tagActivityProtoData> ActivityVector;
	typedef std::vector<tagDailyQuestProtoData> DailyVector;

public:
	~ActivityMgr();
	bool Init();
	VOID Update();
	void Destroy();
	
	static ActivityMgr* Inst();
	const ActivityProtoData& GetData() const { return m_data; }
	const ActivityVector& GetActivityVector();
	const DailyVector& GetDailyVector();
	void SortActivities(ESortType type);
	void SortDaily();
	UINT16 GetActivityID(DWORD index) const;

private:
	ActivityMgr();

	bool LoadActivityProto();
	
	// ��ʼ����� 
	void InitActivityTable();

	// ��Ӧ�������״̬������Ϣ 
	DWORD OnNS_ActivityTaskState(tagNS_ActivityTaskState *pCmd, DWORD);

	DWORD OnNS_RoleInitOk( tagNS_RoleInitOk* pNetCmd, DWORD dwParam );
	DWORD NS_GetCompleteQuest(tagNS_GetRoleInitState_DailyCompleteQuest* pMsg, DWORD pParam);
	DWORD NS_GetSingleQuestDoneInfo(tagNS_GetSingleQuestDoneInfo* pMsg, DWORD pParam);
	DWORD NS_XunLianChangRemainTime(tagNS_XunLianChangRemainTime* pMsg, DWORD pParam);

	DWORD ClearDailyQuestDoneTimes(tagGameEvent* pGameEvent);
	
	
private:
	TSFPTrunk<ActivityMgr> m_trunk;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<NetSession> m_pSession;
	
	FestivalActivityPromptFrame*	m_pFsectivalActivityFrame;
	ActivityProtoData m_data;								// ��������Ա�
	ActivityVector m_activities;
	DailyVector    m_dailys;
};

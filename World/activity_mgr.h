//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: activity.h
// author: wjiang
// actor:
// data: 2009-03-27
// last:
// brief: �������
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/activity_define.h"
#include "db_session.h"
#include "unit.h"

//----------------------------------------------------------------------------------------------------
// �̶����
//----------------------------------------------------------------------------------------------------

class ActScript;

struct tagMergedScriptData
{
	BOOL bHaveData;
	ScriptData<ESD_Activity> m_MergedScriptData;		// �Ϸ����ߺϲ�������scriptdata

	tagMergedScriptData():bHaveData(FALSE){}
	tagMergedScriptData& operator=(tagMergedScriptData& MergedScriptData)
	{
		bHaveData = MergedScriptData.bHaveData;
		m_MergedScriptData = MergedScriptData.m_MergedScriptData;
		return *this;
	}
};

class CActivity : public ScriptData<ESD_Activity>
{
public:
	CActivity();
	~CActivity();

	VOID	Init(const tagActInfo *pActInfo);
	VOID	InitScriptData(tagActivityData* pActivityData);
	BOOL	CanStart(tagDWORDTime cur_time);
	BOOL	IsStartBroad(tagDWORDTime cur_time);
	BOOL	IsEndBroad(tagDWORDTime cur_time);
	BOOL	IsEnd(tagDWORDTime cur_time);
	BOOL	IsStart() { return m_bIsStart; }
	VOID	Start();
	VOID	End();

	DWORD		GetActID() { return m_pActInfo->dwID; }
	EActMode	GetMode()	{return m_pActInfo->eActMode; }

	tagDWORDTime	GetStartTime() { return m_pActInfo->ActTime.startTime; }
	tagDWORDTime	GetEndTime() { return m_pActInfo->ActTime.endTime; }
	DWORD			GetWeekDay() { return m_pActInfo->ActTime.eWeekday; }

	const ActScript* GetScript() { return m_pScript; }
	
	VOID	Update();
	VOID	OnMinute(tagDWORDTime cur_time);
	
	/// �����񼣵�������Ϣ�����
	DWORD	SendAllGodMiracleToPlayer(Unit* pPlayer);
	/// �õ�ĳ�����Ƿ�ռ����
	DWORD	GetGuildCpatureGodMiracleNum(DWORD dwGuild);

	VOID OpenActivity(DWORD dwActivityID);
	VOID CloseActivity(DWORD dwActivityID);

public:
	VOID	AddEventTime(DWORD dwTime);
	VOID	SaveActivityData2DB();

	BOOL IsHaveMergedData() { return m_bHaveMergedData; }
	VOID ResetIsHaveMergedData()
	{
		m_bHaveMergedData = FALSE;
		for (INT nCnt = 0; nCnt < MERGED_SCRIPTDATA_MAX; nCnt++)
		{
			m_MergedData[nCnt].bHaveData = FALSE;
			for (INT nDataIdx = 0; nDataIdx < ESD_Activity; nDataIdx++)
				m_MergedData[nCnt].m_MergedScriptData.SetScriptData(nDataIdx, 0);
		}
	}
	BOOL GetMergedScriptData(INT nIdx, tagMergedScriptData& MergedScriptData);
private:
	VOID    BroadActivityState(EActBroadMode eState);

private:
	const tagActInfo*				m_pActInfo;
	BOOL							m_bIsStart;				// ��Ƿ�ʼ
	std::vector<tagDWORDTime>		m_vecStartBroadTime;	// ���ʼǰ�Ĺ㲥ʱ��
	std::vector<tagDWORDTime>		m_vecEndBroadTime;		// �����ǰ�Ĺ㲥ʱ��
	std::set<DWORD>					m_setOnTimeTick;		// ���ʱ�䴥���¼��Ĵ���ʱ���б�
	DWORD							m_dwTick;				// ����е�ʱ��

	const ActScript*				m_pScript;

	BOOL m_bHaveMergedData;		// �Ƿ��кϷ����ߺϲ��Ļ����
	tagMergedScriptData m_MergedData[MERGED_SCRIPTDATA_MAX];
};


//----------------------------------------------------------------------------------------------------
// �̶��������
//----------------------------------------------------------------------------------------------------
class CActivityMgr
{
public:
	CActivityMgr():m_dwCityStruggleState(1)
	{

	}
	~CActivityMgr();

	BOOL   Init();
	VOID   Update();
	VOID   OnMinute(DWORD cur_time);
	VOID   Destroy();

	CActivity*	GetActivity(DWORD dwID)	{ return m_mapActivity.Peek(dwID); }
	BOOL		HaveActivitStart();
	VOID		SaveAllActivityData2DB();

	DWORD		GetCityStruggleState(){ return m_dwCityStruggleState;}

private:
	BOOL   LoadActivityData();

	VOID OpenActivity(LPCTSTR szActivityID);
	VOID CloseActivity(LPCTSTR szActivityID);

private:
	TSFPTrunk<CActivityMgr> m_Trunk;

	TObjRef<VarContainer>		m_pVar;

	TMap<DWORD,	tagActInfo*>	m_mapActInfo;
	TMap<DWORD, CActivity*>		m_mapActivity;
	DWORD						m_dwCityStruggleState;//1Ϊ������ս��0Ϊ�رճ�ս
};

extern CActivityMgr	g_activityMgr;


















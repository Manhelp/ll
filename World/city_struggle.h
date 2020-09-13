//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_struggle.h
// author: twan
// actor:
// data: 2009-9-10
// last:
// brief: ��ս
//-------------------------------------------------------------------------------
#pragma once

#include "mutex.h"
#include "guild.h"
#include "guild_mgr.h"
#include "../WorldDefine/city_struggle_define.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/base_define.h"

class City;

class CSScoreRank
{
public:
	bool operator() (const pair<DWORD, INT>& lhs, const pair<DWORD, INT>& rhs)
	{
		return lhs.second > rhs.second;
	}
};

class CityStruggle
{
public:
	typedef TMap<DWORD, tagStruggleRevive*>			ReviveMap;
public:
	CityStruggle();
	~CityStruggle();

	//--------------------------------------------------------------------
	// ��ʼ��
	//--------------------------------------------------------------------
	VOID	Init(City* pCity);

	//--------------------------------------------------------------------
	// ���ó�ս״̬
	//--------------------------------------------------------------------
	VOID	SetCityStruggleState(ECityStruggleState eState);
	
	//--------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------
	VOID	Destroy();

	//--------------------------------------------------------------------
	// ����
	//--------------------------------------------------------------------
	VOID	Update();

	//--------------------------------------------------------------------
	// �����ػ���
	//--------------------------------------------------------------------
	VOID	UpdateEudemon();

	//--------------------------------------------------------------------
	// ���³�ս��Ҹ���
	//--------------------------------------------------------------------
	VOID	UpdateRoleRevive();

	//--------------------------------------------------------------------
	// ��������
	//--------------------------------------------------------------------
	VOID	UpdateBall();

	//--------------------------------------------------------------------
	// ����������
	//--------------------------------------------------------------------
	VOID	UpdateColumns();

	//--------------------------------------------------------------------
	// ������Ҽ��븴�����
	//--------------------------------------------------------------------
	VOID	AddToReviveMap(Role* pRole);

	//--------------------------------------------------------------------
	// ����Ƿ�����Ӫ��
	//--------------------------------------------------------------------
	BOOL	IsRoleInForce(Role* pRole, ECityForceType eForce);

	//--------------------------------------------------------------------
	// ��������Ӫ
	//--------------------------------------------------------------------
	BOOL	GetForceType(Role* pRole, ECityForceType& eForce);

	//--------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------
	VOID	ReviveRole(DWORD dwRoleID);

	//--------------------------------------------------------------------
	// �������˸���ʱ��
	//--------------------------------------------------------------------
	VOID	SetReviveTime(ECityForceType eForce, INT nSec);

	//--------------------------------------------------------------------
	// �����ػ����ٻ�ʱ��
	//--------------------------------------------------------------------
	VOID	SetSummonTime(INT nSec);

	//--------------------------------------------------------------------
	// ��ֹ�ٻ��ػ���
	//--------------------------------------------------------------------
	VOID	StopSummon();

	//--------------------------------------------------------------------
	// ���ó�ս���
	//--------------------------------------------------------------------
	VOID	SetResult(ECityStruggleResult eResult);

	//--------------------------------------------------------------------
	// ����get
	//--------------------------------------------------------------------
	ECityStruggleState		GetCityStruggleState()	{ return m_eState; }
	ECityStruggleResult		GetResult()	{ return m_eResult; }

	//--------------------------------------------------------------------
	// ��ս������ݸ��ֲ���
	//--------------------------------------------------------------------
	VOID	InitCSRoleData();
	VOID	AddRoleDataInfo(DWORD dwRoleID);
	VOID	AddKillRoleNum(DWORD dwKillerID, DWORD dwTargetID);
	VOID	AddRoleData(DWORD dwRoleID, ECityStruggleRoleData eType, DWORD dwData);
	tagCSRoleData*	GetRoleDataPtr(DWORD dwRoleID)	{ return m_mapRoleData.Peek(dwRoleID); }
	VOID	SortByScore();
	vector< pair<DWORD, INT> >& GetRankData()		{ return m_vecScore; }

private:
	ECityStruggleState		m_eState;						// ��ǰ��ս״̬

	ReviveMap				m_mapRevive;					// ������Ľ�ɫ��ID��Ӧ�ڸý�ɫ����ʱ�ĳ�ս��ʱ
	Mutex					m_mutexRevive;					
	volatile INT			m_nUpdateTick;					// ��ս���¼�ʱ��ÿtick��1
	volatile INT			m_nAtkReviveTick;				// �������˳�Ա��������Ҫ�ȴ���tick��
	volatile INT			m_nDefReviveTick;				// �������˳�Ա��������Ҫ�ȴ���tick��
	volatile INT			m_nEudemonTick;					// �ٻ��ػ�������ȴ���tick��
	City*					m_pCity;						// ����ָ��
	BOOL					m_bBeginSummonCountDown;		// ��ʼ�ٻ�����ʱ

	TMap<DWORD, tagCSRoleData*>		m_mapRoleData;			// ��ս�������
	vector< pair<DWORD, INT> >		m_vecScore;				// ��һ�������
	ECityStruggleResult		m_eResult;
};
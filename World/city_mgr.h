//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_mgr.h
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: ����ϵͳ����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/city_define.h"

class City;
class Unit;

class CityMgr
{
public:
	typedef TMap<DWORD, City*>	MapCity;

public:
	CityMgr();
	~CityMgr();

	BOOL	Init();
	VOID	Update();
	VOID	Destory();

	City*	GetCity(DWORD dwCityID)			{ return m_mapCity.Peek(dwCityID); }
	City*	GetCity(ECLanType eType);
	City*	GetCity(Unit* pUnit);
	City*	GetCity(Map* pMap);

	// ������ݿ��г�����Ϣ
	VOID	RequestCityInfoFromDB();

	// �����ɱ�����Ϣ
	BOOL	IsAlreadySignup(DWORD dwGuildID, ECityForceType eForce);

	// ���ɽ�ɢ����
	VOID	OnGuildDismissed(DWORD dwGuildID);

	// �õ�������
	DWORD	GetCityNum();

	// �õ�����˰��
	VOID	GetAllCityTaxRate(LPVOID pData);

	// �õ���ս�еĳ���id
	VOID GetCityWarCity(tagNetCmd* pCmd);

private:

	// ��ս����������̨�����ã�
	VOID SignupForCityWar(LPCTSTR szCityID, LPCTSTR szGuildID, LPCTSTR szForceType, LPCTSTR szFare);

private:
	TSFPTrunk<CityMgr> m_Trunk;

	MapCity		m_mapCity;
};

extern CityMgr g_cityMgr;

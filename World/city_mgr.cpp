//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_mgr.cpp
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: ����ϵͳ����
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../ServerDefine/msg_city.h"
#include "../WorldDefine/msg_city.h"

#include "city_mgr.h"
#include "city.h"
#include "att_res.h"
#include "db_session.h"
#include "unit.h"

CityMgr g_cityMgr;

//-------------------------------------------------------------------------
// ���������
//-------------------------------------------------------------------------
CityMgr::CityMgr()
{
	m_mapCity.Clear();
}

CityMgr::~CityMgr()
{
	Destory();
}

//-------------------------------------------------------------------------
// ��ʼ�������¡�����
//-------------------------------------------------------------------------
BOOL CityMgr::Init()
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("signupforcitywar"), m_Trunk.sfp4(&CityMgr::SignupForCityWar), _T("Signup For City War"), 4);

	m_mapCity.Clear();

	// ȡ�ó���ID��Χ
	DWORD dwCityStart	= GT_INVALID;
	DWORD dwCityEnd		= GT_INVALID;
	g_attRes.GetCityIDRange(dwCityStart, dwCityEnd);
	if (!GT_VALID(dwCityStart) || !GT_VALID(dwCityEnd))
	{
		return TRUE;	// û�г�����Ϣ
	}

	// ��ʼ������
	for (DWORD dwID = dwCityStart; dwID <= dwCityEnd; dwID++)
	{
		City* pCity = new City;
		if (!pCity->Init(dwID))
		{
			IMSG(_T("city init failed cityid:%d\r\n"), dwID);
			SAFE_DEL(pCity);
			continue;
		}
		m_mapCity.Add(dwID, pCity);
	}

	return TRUE;
}

VOID CityMgr::Update()
{
	CHECK_TIME()
	tagDWORDTime tm = g_world.GetWorldTime();

	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();
	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (P_VALID(pCity))
		{
			pCity->Update(tm);
		}
	}
}

VOID CityMgr::Destory()
{
	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();
	while (m_mapCity.PeekNext(iter, pCity))
	{
		SAFE_DEL(pCity);
	}
	m_mapCity.Clear();
}

//-------------------------------------------------------------------------
// ������ݿ��г�����Ϣ
//-------------------------------------------------------------------------
VOID CityMgr::RequestCityInfoFromDB()
{
	if (m_mapCity.Empty())
	{
		return;
	}

	// ������Ϣ
	INT nMsgSize = sizeof(tagNDBC_CreateCityInfo) + sizeof(DWORD) * (m_mapCity.Size() - 1);
	MCREATE_MSG(pSend, nMsgSize, NDBC_CreateCityInfo);

	City* pCity		= NULL;
	DWORD dwCityID	= GT_INVALID;
	MapCity::TMapIterator iter = m_mapCity.Begin();
	while (m_mapCity.PeekNext(iter, dwCityID, pCity))
	{
		if (P_VALID(pCity) && GT_VALID(dwCityID))
		{
			pSend->dwCityID[pSend->nCityNum++] = dwCityID;
		}
	}
	if (pSend->nCityNum > 0)
	{
		// ���¼�����Ϣ��С
		pSend->dwSize = sizeof(tagNDBC_CreateCityInfo) + sizeof(DWORD) * (pSend->nCityNum - 1);

		g_dbSession.Send(pSend, pSend->dwSize);

		// ����ÿһ��������Ϣ
		for (int n=0; n<pSend->nCityNum; n++)
		{
			tagNDBC_LoadCityInfo send;
			send.dwCityID = pSend->dwCityID[n];
			g_dbSession.Send(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------
// �����ɱ�����Ϣ
//-------------------------------------------------------------------------
BOOL CityMgr::IsAlreadySignup( DWORD dwGuildID, ECityForceType eForce )
{
	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (P_VALID(pCity) && pCity->IsAlreadySignup(dwGuildID, eForce))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------
// ����ָ���������ڳ���
//-------------------------------------------------------------------------
City* CityMgr::GetCity( ECLanType eType )
{
	City* pCity = NULL;
	INT nNum = 0;
	ECLanType eClanType;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!P_VALID(pCity))
			continue;

		pCity->GetCityAppointedAtt(ECAT_ClanType, nNum, &eClanType);
		if ((nNum > 0) && (eClanType == eType))
		{
			return pCity;
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------
// ���ݵ�ͼָ��õ�����
//-------------------------------------------------------------------------
City* CityMgr::GetCity( Map* pMap )
{
	if (!P_VALID(pMap))
	{
		return NULL;
	}
	City* pCity = NULL;
	INT nNum = 0;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!(P_VALID(pCity) && pCity->IsInitOK()))
			continue;

		if ((pMap->GetMapID() == pCity->GetShellCity()->GetMapID())
			|| pMap->GetMapID() == pCity->GetKernelCity()->GetMapID())
		{
			return pCity;
		}
	}

	return NULL;

}
//-------------------------------------------------------------------------
// ����ָ����λ���ڳ���
//-------------------------------------------------------------------------
City* CityMgr::GetCity( Unit* pUnit )
{
	if (!P_VALID(pUnit))
	{
		return NULL;
	}

	Map* pMap = pUnit->GetMap();
	if (!P_VALID(pMap))
	{
		return NULL;
	}

	City* pCity = NULL;
	INT nNum = 0;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!(P_VALID(pCity) && pCity->IsInitOK()))
			continue;

		if ((pMap->GetMapID() == pCity->GetShellCity()->GetMapID())
			|| (pMap->GetMapID() == pCity->GetKernelCity()->GetMapID()))
		{
			return pCity;
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------
// �õ�������Ŀ
//-------------------------------------------------------------------------
DWORD CityMgr::GetCityNum()
{
	return m_mapCity.Size();
}

//-------------------------------------------------------------------------
// �õ����еĳ���˰��
//-------------------------------------------------------------------------
VOID CityMgr::GetAllCityTaxRate(LPVOID pData)
{
	INT nDataNum = 0;

	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!P_VALID(pCity))
			continue;

		//�õ�cityid��˰��
		INT nTemp = 0;
		pCity->GetCityAppointedAtt(ECAT_CityID,nTemp,&((tagCityTaxRate*)pData)[nDataNum].byCityID);
		pCity->GetCityAppointedAtt(ECAT_TaxRate,nTemp,&((tagCityTaxRate*)pData)[nDataNum].byTaxRate);
		++nDataNum;
	}
}
//-------------------------------------------------------------------------
// �õ���ս���е�ID
//-------------------------------------------------------------------------
VOID CityMgr::GetCityWarCity(tagNetCmd* pCmd)
{
	MGET_MSG(pSend, pCmd, NS_GetCityWarCity);

	DWORD dwNum = 0;
	tagDWORDTime tm = GetCurrentDWORDTime();

	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!P_VALID(pCity))
			continue;

		INT nDataNum = 0;
		pCity->GetCityAppointedAtt(ECAT_CityID,nDataNum,&pSend->dwCityID[dwNum++]);
	}
	
	//��ǰ��ս�г��е���Ŀ
	pSend->nNum = dwNum;
}

// ��ս����������̨�����ã�
VOID CityMgr::SignupForCityWar(LPCTSTR szCityID, LPCTSTR szGuildID, LPCTSTR szForceType, LPCTSTR szFare)
{
	DWORD				dwCityID = _tcstoul(szCityID, NULL, 10);
	DWORD				dwGuildID = _tcstoul(szGuildID, NULL, 10);			// ��������ID
	ECityForceType		eCityForceType = static_cast<ECityForceType>(_ttoi(szForceType));	// ������������
	INT32				nFare = _tcstoul(szFare, NULL, 10);					// ������(������Ϊ����ʱ�����Ƿ񾭹�����)

	City* pCity = m_mapCity.Peek(dwCityID);
	if (!P_VALID(pCity))
	{
		IMSG(_T("city id invalid or city is not exist!! \r\n"));
		return;
	}

 	if (GT_INVALID != pCity->SignupForCitywar(dwGuildID, eCityForceType, nFare))
	{
		IMSG(_T("Signup For City War Success!!!\r\n"));
	}

}

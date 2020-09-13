//----------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_creator.h
// author: Aslan
// actor:
// data: 2008-6-27
// last:
// brief: ��ͼ��������ͳ��������е�MapMgr��ά������MapMgr�������ڣ����ṩ��ͼ��̬���ԣ�ȫ�ֶ���
//----------------------------------------------------------------------------------------------
#pragma once

#include "map.h"
#include "mutex.h"

struct tagMapInfo;
class Map;
class Team;
class MapInstance;
class MapMgr;
class MapRestrict;

//---------------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------------
enum EWearyCheckResult
{
	EWCR_NoWearyCheck = 0,	// ����Ҫ������ƣ��ֵ���
	EWCR_Can_Enter,			// ���Խ��븱��
	EWCR_Can_Not_Enter,		// ���ܽ��븱��
};

//---------------------------------------------------------------------------------
// �л���ͼ�Ľṹ
//---------------------------------------------------------------------------------
struct tagChangeMapInfo
{
	DWORD		dwRoleID;			// ��ɫID
	DWORD		dwDestMapID;		// Ŀ���ͼID
	Vector3		vPos;				// Ŀ���ͼ����
	DWORD		dwMisc;				// �������������������������Ѷ�

	tagChangeMapInfo(DWORD)
	{
		dwRoleID = GT_INVALID;
	}
	tagChangeMapInfo()
	{
		dwRoleID = GT_INVALID;
	}
};

//---------------------------------------------------------------------------------
// �л���ͼ�Ľṹ,����һ��ȷ�еĸ���
//---------------------------------------------------------------------------------
struct tagChangeMapInstInfo
{
	DWORD		dwRoleID;			// ��ɫID
	DWORD		dwDestMapID;		// Ŀ���ͼID
	DWORD		dwDestMapInstID;	// Ŀ���ͼInstance ID
	Vector3		vPos;				// Ŀ���ͼ����
	DWORD		dwMisc;				// �������������������������Ѷ�

	tagChangeMapInstInfo(DWORD)
	{
		dwRoleID = GT_INVALID;
	}
	tagChangeMapInstInfo()
	{
		dwRoleID = GT_INVALID;
	}
};

//-----------------------------------------------------------------------------------
// ����ɾ���ṹ
//-----------------------------------------------------------------------------------
struct tagInstanceDestroyInfo
{
	DWORD		dwMapID;			// ��ͼID
	DWORD		dwInstanceID;		// ����ID

	tagInstanceDestroyInfo(DWORD)
	{
		dwMapID = GT_INVALID;
	}
	tagInstanceDestroyInfo(DWORD _dwMapID, DWORD _dwInstanceID)
	{
		dwMapID = _dwMapID;
		dwInstanceID = _dwInstanceID;
	}
	tagInstanceDestroyInfo()
	{
		dwMapID = GT_INVALID;
	}
};

//-------------------------------------------------------------------------------------------------
// ��ͼ������
//-------------------------------------------------------------------------------------------------
class MapCreator
{
public:
	typedef		TMap<DWORD, MapMgr*>		MapMgrMap;
	typedef		TMap<DWORD, NavMap*>		InstanceTemplateMap;
	typedef		TMap<DWORD, tagMapInfo*>	MapInfoMap;
	typedef		TList<DWORD>				ListRoleID;
	typedef		TList<DWORD>				ListNormalMapID;

public:
	MapCreator();
	~MapCreator();

	//---------------------------------------------------------------------------------------------
	// ��ʼ�������¼�����
	//---------------------------------------------------------------------------------------------
	BOOL				Init();
	NavMap*				LoadInstanceTemplate(tagMapInfo* pInfo);
	VOID				Update();
	VOID				Destroy();

	//--------------------------------------------------------------------------------------------
	// �������
	//--------------------------------------------------------------------------------------------
	Map*				CreateAFMap(const tagMapInfo* pInfo);
	MapInstance*		CreateAFMapInstance(const tagMapInfo* pInfo);
	MapRestrict*		CreateAFMapRestrict(const tagMapInfo* pInfo);

	VOID				DestroyAFMap(Map* pMap);
	VOID				DestroyAFMapRestrict(MapRestrict* pMap);

	//--------------------------------------------------------------------------------------------
	// �̹߳���
	//--------------------------------------------------------------------------------------------
	VOID				StartAllMapMgr();								// ��Tick�������е�ͼ�߳�����
	VOID				WaitAllMapMgrEnd();								// �ȴ���Tick���е�ͼ�߳̽���
	VOID				UpdateAllDelayedStuff();						// ÿһTick�������е�ͼ�߳���ͣ��Ĳ���
	VOID				StopAllMapMgr();								// ����ֹͣ���е�ͼ�������߳�
	Event&				GetAllMapStartEvent(INT n);						// �õ���Tick�������е�ͼ�߳����е��¼�
	VOID				OneMapMgrEnd();									// һ����ͼ�̱߳�Tickִ�����

	VOID				WaitIdleCpu()	{ m_pSemIdleCpu->Wait();}	// �ȴ�����
	VOID				ReleaseCpu()	{ m_pSemIdleCpu->Release();}// ���½���

	//--------------------------------------------------------------------------------------------
	// ����Get
	//--------------------------------------------------------------------------------------------
	MapMgr*				GetMapMgr(DWORD dwMapID)			{ return m_mapMapMgr.Peek(dwMapID); }
	const tagMapInfo*	GetMapInfo(DWORD dwMapID)			{ return m_mapMapInfo.Peek(dwMapID); }
	DWORD				GetBornMapID()						{ return m_dwBornMapID; }
	NavMap*				GetInstanceTemplate(DWORD dwMapID)	{ return m_mapInstanceTemplate.Peek(dwMapID); }
	BOOL				IsMapExist(DWORD dwMapID)			{ return m_mapMapInfo.IsExist(dwMapID); }

	Vector3				RandGetOneBornPos();
	Map*				GetMap(DWORD dwMapID, DWORD dwInstance);
	DWORD				GetPrisonMapID()			{ return m_dwPrisonMapID; }
	const Vector3		GetPutInPrisonPoint()		{ return m_PutInPrisonPoint; }
	const Vector3		GetPutOutPrisonPoint()		{ return m_PutOutPrisonPoint; }
	const Vector3		GetReBornPoint(DWORD dwRebornMapID);
	
	INT					GetStableInstanceNum()		{ return m_nMaxStableInstanceNum; }

	//---------------------------------------------------------------------------------------------
	// ��ɫ������
	//---------------------------------------------------------------------------------------------
	VOID				RoleLogOut(DWORD dwRoleID)			{ m_listLogOutRole.PushBack(dwRoleID); }
	VOID				RoleReturnCharacter(DWORD dwRoleID)	{ m_listReturnCharacterRole.PushBack(dwRoleID); }
	VOID				RoleChangeMap(DWORD dwRoleID, DWORD dwDestMapID, Vector3& vPos, DWORD dwMisc);
	VOID				RoleChangeMapInst(DWORD dwRoleID, DWORD dwDestMapID, DWORD dwDestMapInstID, Vector3& vPos, DWORD dwMisc);

	BOOL				TakeOverRoleWhenOnline(Role* pRole);

	//---------------------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------------------
	DWORD				CreateNewInstanceID()
	{
		InterlockedExchangeAdd((LPLONG)&m_dwInstanceIDGen, 1);
		
		// ����һ���������������Ľ�ɫID���б�
		AddRoleIDList2Map(m_dwInstanceIDGen);

		return m_dwInstanceIDGen;
	}
	VOID				InstanceDestroyed(DWORD dwMapID, DWORD dwInstanceID);

	VOID				SetInstanceDelete(DWORD dwMapID, DWORD dwInstanceID);

	BOOL				CanCreateInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr);
	VOID				AddInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr);
	VOID				RemoveInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr);
	INT					GetInstanceNum(INT nIndex)		{ return m_nInstanceNumCur[nIndex]; }
	INT					GetInstanceCoefNum()			{ return m_nInstanceCoefNumCur; }
	INT					GetInstanceCoefNumLimit()		{ return m_nInstanceCoefNumLimit; }

	// ����½����������Ϣ֪ͨ�����Կ�ʼ��ֹͣ��ʱ��Խ�ϰ汾�����ԣ�
	VOID				SendFatigueTimer(Role* pRole, BYTE byEnterMap);

	DWORD				GetRandNormalMapID();

	// Jason ��¼��������ĳһʱ�̣���ͼ�����������������������Լ����� 2010-8-18
	VOID				ReportMapDynInfo( BOOL bSaveToDB = FALSE );
private:
	//---------------------------------------------------------------------------------------------
	// ��ʼ��
	//---------------------------------------------------------------------------------------------
	BOOL				LoadMapInfo(LPCTSTR szFileMapName, TObjRef<VarContainer>& pLogic);				// �����ͼ�Ļ�����Ϣ������ɹ�ʱ������뵽m_mapMapInfo
	BOOL				LoadMapWayPoint(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);		// �����ͼ�е����е�����
	BOOL				LoadMapTrigger(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);			// �����ͼ�е����д�����
	BOOL				LoadMapArea(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);			// �����ͼ�е���������
	BOOL				LoadMapCreature(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);		// �����ͼ�е����г�������
	BOOL				LoadMapSpawnPoint(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);		// �����ͼ�е�����ˢ�ֵ�
	BOOL				LoadMapTriggerEffect(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);	// �����ͼ�е�������Ч
	BOOL				LoadMapDoor(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo);			// �����ͼ�е�������

	// ���ع���ˢ�±� [4/18/2012 zhangzhihua]
	BOOL				LoadMapMonsterGen(tagMapInfo* pInfo);

	//----------------------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------------------
	VOID				RegisterAFMap();
	VOID				RegisterAFMapRestrict();
	VOID				UnRegisterAll();

	//----------------------------------------------------------------------------------------------
	// �̹߳���
	//----------------------------------------------------------------------------------------------
	MapMgr*				CreateMapMgr(tagMapInfo* pInfo);

	//----------------------------------------------------------------------------------------------
	// ����������
	//----------------------------------------------------------------------------------------------
	VOID				LogoutAllRemovedRolePerTick();
	VOID				DealAllChangeMapRole();
	VOID				DealAllChangeMapInstRole();
	VOID				DealAllReturnCharacterRole();
	VOID				DealAllDestroiedInstance();

	// ���ٽ������������н�ɫ��ID�б�
	VOID				DestroyRoleIDList();

	// ����һ���������������Ľ�ɫID���б�
	VOID				AddRoleIDList2Map(DWORD dwInstanceID);

	// ���븱��ǰ���Խ�ɫ����ƣ�Ͷȼ�飻 ����0,���Խ��븱��������1������-1�����ܽ��븱��
	EWearyCheckResult	CheckRoleWearyValue(DWORD dwRoleID, DWORD dwInstanceMapID, DWORD dwInstanceID, INT* pnAddWearyValue);

private:
	TObjRef<Util>						m_pUtil;
	TObjRef<Thread>						m_pThread;

	TAutoFactory<MapRestrict>			m_AFRestrict;					// ��ͼ�����๤������ͼ���͡������������ࣩ
	TAutoFactory<Map>					m_AFMap;						// ��ͼ�๤������ͼ���͡�����ͼ�����ࣩ

	MapMgrMap							m_mapMapMgr;					// ���е�MapMgr�б�����Ƿ�Ҫ��������
	InstanceTemplateMap					m_mapInstanceTemplate;			// Ԥ���������и�����NavMap�����ɸ���ʱֱ�ӿ���
	MapInfoMap							m_mapMapInfo;					// ���е�ͼ���߼�������Ϣ
	ListNormalMapID						m_listNormalMapID;				// ������ͨ��ͼID���ǰ�ȫ����
	volatile DWORD						m_dwInstanceIDGen;				// ����ID������

	INT									m_nInstanceNumCur[EMIR_End];	// ��ͬ��񸱱��ĵ�ǰ��������
	INT									m_nInstanceCoefNumCur;			// ��ǰ���и����ļ�Ȩ�ܺ�
	INT									m_nInstanceCoefNumLimit;		// ���и����ļ�Ȩ�ܺ�����

	DWORD								m_dwBornMapID;					// ��ҳ�����ͼ��ID
	INT									m_nBornPosNum;					// ���������
	Vector3*							m_pvBornPos;					// ��������������

	Event								m_MapStartEvent[2];				// �¼������������е�ͼ�߳�ִ�У������¼������е�ͼ�̵߳�Update�߳̽�������������¼���
	INT									m_nWhichEvent;					// ��ǰʹ�õ����ĸ��¼�
	Event								m_MapEndEvent;					// �¼��������е�ͼUpdate���
	volatile INT						m_nNumMapMgr;					// MapMgr��������ÿ��Tick����ΪMapMgr����������ÿ��MapMgr������һ�κ���ٸ�ֵ

	TSafeList<DWORD>					m_listLogOutRole;				// ÿ��Tick�ǳ������
	TSafeList<tagChangeMapInfo>			m_listChangeMapRole;			// ÿ��TickҪ�л���ͼ�����
	TSafeList<tagChangeMapInstInfo>		m_listChangeMapInstRole;		// ÿ��TickҪ�л���ȷ����ͼ�����
	TSafeList<DWORD>					m_listReturnCharacterRole;		// ÿ��TickҪ�ص�ѡ�˽�������
	
	TSafeList<tagInstanceDestroyInfo>	m_listDestroyInstance;			// ÿ��TickҪɾ���ĸ���

	// ������Ϣ
	DWORD								m_dwPrisonMapID;				// ������ͼid
	Vector3								m_PutInPrisonPoint;				// ����������point
	Vector3								m_PutOutPrisonPoint;			// ����������point

	// ���ִ�
	INT									m_nMaxStableInstanceNum;		// ���ִ帱������

	Semaphore*							m_pSemIdleCpu;					// ����cpu����

	TMap<DWORD, ListRoleID*>			m_mapRoleIDList;				// �������������н�ɫ��ID�б�

	// Jason ͳ����������״��
public:
	struct tagMapOnlineInfo
	{
		DWORD	dwMapID;
		INT			nOnlineNum;
		BOOL		bIsInst;
		INT			nOpenNum;
		tstring		strMapName;
		BOOL		bChanged;
		tagMapOnlineInfo()
			:dwMapID(0),nOnlineNum(0),bIsInst(FALSE),nOpenNum(0),bChanged(FALSE)
		{
		}
		void copy(const tagMapOnlineInfo * pMapOnInfo)
		{
			if( P_VALID(pMapOnInfo) )
			{
				dwMapID			= pMapOnInfo->dwMapID;
				nOnlineNum	= pMapOnInfo->nOnlineNum;
				bIsInst				= pMapOnInfo->bIsInst;
				nOpenNum		= pMapOnInfo->nOpenNum;
				bChanged			= pMapOnInfo->bChanged;
				strMapName	= pMapOnInfo->strMapName;
			}
		}
	};
private:
	TMap<DWORD,tagMapOnlineInfo*>	m_mapOnlineInfo;

protected:
	VOID DealAllAttackDefenseInfo(VOID);
};

//--------------------------------------------------------------------------------------------
// �������е�ͼ�߳�
//--------------------------------------------------------------------------------------------
inline VOID MapCreator::StartAllMapMgr()
{
	m_nWhichEvent = ((m_nWhichEvent == 0) ? 1 : 0);
	InterlockedExchange((LPLONG)&m_nNumMapMgr, m_mapMapMgr.Size());
	m_MapStartEvent[m_nWhichEvent].Set();
}

//--------------------------------------------------------------------------------------------
// �ȴ����е�ͼ�̱߳�tick���
//--------------------------------------------------------------------------------------------
inline VOID	MapCreator::WaitAllMapMgrEnd()
{
	m_MapEndEvent.Wait();
	m_MapEndEvent.ReSet();
	m_MapStartEvent[m_nWhichEvent].ReSet();
}

//---------------------------------------------------------------------------------------------
// ����õ�һ�����������
//---------------------------------------------------------------------------------------------
inline Vector3 MapCreator::RandGetOneBornPos()
{
	INT nRand = m_pUtil->Rand() % m_nBornPosNum;
	return m_pvBornPos[nRand];
}

//---------------------------------------------------------------------------------------------
// �õ�������ͼ�߳����е��¼�
//---------------------------------------------------------------------------------------------
inline Event& MapCreator::GetAllMapStartEvent(INT n)
{
	return m_MapStartEvent[n];
}

//---------------------------------------------------------------------------------------------
// ĳ����ͼ�̱߳�Tickִ�����
//---------------------------------------------------------------------------------------------
inline VOID MapCreator::OneMapMgrEnd()
{
	// �����е�ͼ�߳�ִ����Ϻ������¼�֪ͨ
	if( 0 == InterlockedDecrement((LPLONG)&m_nNumMapMgr ) ) 
	{
		m_MapEndEvent.Set();
	}
}

//----------------------------------------------------------------------------------------------
// ����л���ͼ
//----------------------------------------------------------------------------------------------
inline VOID MapCreator::RoleChangeMap(DWORD dwRoleID, DWORD dwDestMapID, Vector3& vPos, DWORD dwMisc)
{
	MapMgr* pMapMgr = GetMapMgr(dwDestMapID);
	if( !P_VALID(pMapMgr) ) return;

	tagChangeMapInfo info;
	info.dwRoleID		=	dwRoleID;
	info.dwDestMapID	=	dwDestMapID;
	info.vPos			=	vPos;
	info.dwMisc			=	dwMisc;

	m_listChangeMapRole.PushBack(info);
}

inline VOID MapCreator::RoleChangeMapInst(DWORD dwRoleID, DWORD dwDestMapID, DWORD dwDestMapInstID, Vector3& vPos, DWORD dwMisc)
{
	MapMgr* pMapMgr = GetMapMgr(dwDestMapID);
	if( !P_VALID(pMapMgr) ) return;

	tagChangeMapInstInfo info;
	info.dwRoleID		=	dwRoleID;
	info.dwDestMapID	=	dwDestMapID;
	info.vPos			=	vPos;
	info.dwMisc			=	dwMisc;
	info.dwDestMapInstID=	dwDestMapInstID;

	m_listChangeMapInstRole.PushBack(info);
}

//------------------------------------------------------------------------------------------------
// ��������ɾ��
//------------------------------------------------------------------------------------------------
inline VOID MapCreator::InstanceDestroyed(DWORD dwMapID, DWORD dwInstanceID)
{
	MapMgr* pMapMgr = GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) ) return;

	tagInstanceDestroyInfo info(dwMapID, dwInstanceID);

	m_listDestroyInstance.PushBack(info);
}

extern MapCreator g_mapCreator;


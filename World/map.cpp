//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map.h
// author: aslan
// actor:
// data: 2008-6-27
// last:
// brief: ��ͼ�࣬����һ��ȷ���ĵ�ͼʵ��������Ϊһ�������ĵ�ͼʵ��
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/action.h"
#include "../WorldDefine/remote_role.h"
#include "../WorldDefine/msg_map.h"
#include "../WorldDefine/msg_loot.h"
#include "../WorldDefine/script_data.h"

#include "../ServerDefine/sspawnpoint_define.h"
#include "../ServerDefine/NPCTeam_define.h"



#include "player_session.h"
#include "world_session.h"
#include "login_session.h"
#include "map_creator.h"
#include "unit.h"
#include "role.h"
#include "creature.h"
#include "pet.h"
#include "role_mgr.h"
#include "map.h"
#include "att_res.h"
#include "shop.h"
#include "team.h"
#include "group_mgr.h"
#include "move_data.h"
#include "script_mgr.h"
#include "gm_policy.h"
#include "NPCTeam_mgr.h"
#include "NPCTeam.h"
#include "guild_CofC.h"
#include "TimeChecker.h"
#include "item_creator.h"
#include "loot_mgr.h"
#include "fairy_contract.h"
#include "vip_netbar.h"
#include "shielding_mgr.h"
#include "war_role_mgr.h"


//------------------------------------------------------------------------------
// ������Ʒ�Ĵ�С
//------------------------------------------------------------------------------
static Vector3 vGroundItemSize(50.0f, 50.0f, 50.0f);


//------------------------------------------------------------------------------
// construct
//------------------------------------------------------------------------------
Map::Map() : m_dwID(GT_INVALID), m_dwInstanceID(GT_INVALID), m_pInfo(NULL),
m_pVisTile(NULL), m_pNav(NULL), m_dwCurScriptArea(0),ScriptData<ESD_Map>()
,m_bCreatureChanged(TRUE),m_pFairyContractMgr(NULL),m_pWarRoleMgr(NULL)
{
}

//------------------------------------------------------------------------------
// destruct
//------------------------------------------------------------------------------
Map::~Map()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// ��ʼ����ͼ
//-------------------------------------------------------------------------------
BOOL Map::Init(const tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pInfo) );
	ASSERT( EMT_Normal == pInfo->eType );

	m_pUtil = "Util";

	m_pInfo = pInfo;
	m_dwID = m_pInfo->dwID;

	m_mapSession.Clear();
	m_mapRole.Clear();
	m_mapShop.Clear();
	m_mapCofC.Clear();
	m_mapGroundItem.Clear();

	m_listSceneEffect.Clear();

	m_dwLastMinute = timeGetTime();

	// ���뵼��ͼ����
	IFS* pVfs = Kernel::Inst()->DefaultFS();
	m_pNav = new NavMap;
	if( !P_VALID(m_pNav) )
	{
		return FALSE;
	}

	m_MonsterGen.Init(this);

	m_pTeamMgr = new NPCTeamMgr(this);
	if(!P_VALID(m_pTeamMgr))
		return FALSE;

	if (IsWar())
	{
		m_pWarRoleMgr = new WarRoleMgr(this);
		if(!P_VALID(m_pWarRoleMgr))
			return FALSE;
	}

	m_pNav->LoadFromFile(pInfo->szMapFileName, pVfs, pVfs, NavMap::ELO_Collider | NavMap::ELO_NPCNavMap | NavMap::ELO_NPCNavGraph | NavMap::ELO_NPCNavMapDoors);
	//NavResMgr::Inst()->Report();
	Vector3 zoneSize(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
	m_pNav->SetViewZoneSize(zoneSize);
	Vector3 zoneCenter(0, 0, 0);
	m_pNav->InvalidViewZone(zoneCenter);

	// ����mapinfo����ʼ����ͼ�Ĺ�������б����Ϣ
	if( FALSE == InitLogicalInfo() )
	{
		SAFE_DEL(m_pNav);
		return FALSE;
	}
	// ��ֹ�������죬�����������죬���init���ᱻ���õģ��������������͵ĵ�ͼ
	m_bCreatureChanged = TRUE;
	return TRUE;
}

//---------------------------------------------------------------------------------
// ����ʼ�����ʱ
//---------------------------------------------------------------------------------
VOID Map::OnInit()
{
	if( P_VALID(m_pScript) )
		m_pScript->OnInit(this);
}

//------------------------------------------------------------------------------------------
// ȡ������ﲢ�����Ա߷ŵ���һ����Ʒ
//------------------------------------------------------------------------------------------
DWORD Map::PutItemNearRandCreature(DWORD dwItemID, Vector3& vPos, INT64& n64Serial, INT64& n64GroupID)
{
	// ȡ�������
	Creature* pCreature = NULL;
	DWORD dwCreatureID = 0;
	m_mapCreature.RandPeek(dwCreatureID, pCreature);
	if (!P_VALID(pCreature))
	{
		IMSG(_T("[PutItemNearRandCreature] random creature not exist!!"));
		return E_SystemError;
	}

	// �õ���Ʒ����
	tagItemProto* pProto = NULL;
	if( MIsEquipment(dwItemID) )
	{
		pProto = g_attRes.GetEquipProto(dwItemID);
	}
	else
	{
		pProto = g_attRes.GetItemProto(dwItemID);
	}
	if( !P_VALID(pProto) )
	{
		IMSG(_T("[PutItemNearRandCreature] item proto %d not exist!!"), dwItemID);
		return E_SystemError;
	}

	// ������Ʒ
	tagItem* pItem = ItemCreator::Create(EICM_Loot, pCreature->GetID(), pProto->dwTypeID, 1);
	if( !P_VALID(pItem) )
	{
		IMSG(_T("[PutItemNearRandCreature] Can not create item %d!!"), dwItemID);
		return E_SystemError;
	}

	// ����Ʒ���䵽Creature�Աߵĸ���
	BOOL bGetPutDownPos = FALSE;
	
	MTRANS_POINTER(pMap, pCreature->GetMap(), Map);
	if(!P_VALID(pMap))	return E_SystemError;

	for(INT nIndex = 0; nIndex < MAX_LOOT_POS; ++nIndex)
	{
		if (CanPutDown(pCreature, nIndex, vPos))
		{
			bGetPutDownPos = TRUE;
			break;
		}
	}

	if( !bGetPutDownPos )
	{
		::Destroy(pItem);
		IMSG(_T("[PutItemNearRandCreature] Can not find put down position!!"));
		return E_SystemError;
	}

	n64GroupID = PutDownItem(pCreature, pItem, GT_INVALID, GT_INVALID, vPos);
	n64Serial = pItem->n64Serial;
	
	return E_Success;
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID Map::Destroy()
{
	// ɾ����ͼ��Ĺ���
	Creature* pCreature = NULL;

	TMap<DWORD, Creature*>::TMapIterator itCreature = m_mapCreature.Begin();
	while( m_mapCreature.PeekNext(itCreature, pCreature) )
	{
		Creature::Delete(pCreature);
	}
	m_mapCreature.Clear();

	TMap<DWORD, Creature*>::TMapIterator itResCreature = m_mapRespawnCreature.Begin();
	while( m_mapRespawnCreature.PeekNext(itResCreature, pCreature) )
	{
		Creature::Delete(pCreature);
	}
	m_mapRespawnCreature.Clear();
	m_mapDeadDoor.Clear();

	//��ջ����
	m_mapActiveCreature.Clear();
	m_mapDoor.Clear();

	// ɾ��������Ʒ����
	m_mapGroundItem.ResetIterator();
	tagGroundItem* pGroundItem = NULL;
	while( m_mapGroundItem.PeekNext(pGroundItem) )
	{
		pGroundItem->DestroyItem();
		SAFE_DEL(pGroundItem);
	}

	// ɾ���̵�
	Shop* pShop = NULL;
	TMap<DWORD, Shop*>::TMapIterator itShop = m_mapShop.Begin();
	while( m_mapShop.PeekNext(itShop, pShop) )
	{
		Shop::Delete(pShop);
	}
	m_mapShop.Clear();

	// ɾ���̻�
	GuildCofC* pGuildCofC = NULL;
	CofCMap::TMapIterator itCofC = m_mapCofC.Begin();
	while (m_mapCofC.PeekNext(itCofC, pGuildCofC))
	{
		GuildCofC::Delete(pGuildCofC);
	}
	m_mapCofC.Clear();

	// �رվŹ���͵���ͼ
	SAFE_DEL_ARRAY(m_pVisTile);
	SAFE_DEL(m_pNav);

	m_MonsterGen.Destroy();
	// ɾ������С�ӹ�����
	SAFE_DEL(m_pTeamMgr);

	if (IsWar())
	{
		SAFE_DEL(m_pWarRoleMgr);
	}

	// ��սű�
	m_pScript = NULL;

	if( P_VALID(m_pFairyContractMgr) )
		delete m_pFairyContractMgr;
}

//---------------------------------------------------------------------------------
// ���ɵ�ͼ���߼����ԣ���������������㣬��������NPC����ȵ�
//---------------------------------------------------------------------------------
BOOL Map::InitLogicalInfo()
{
	// ����VISTILE
	m_nVisTileArrayWidth = (m_pInfo->nWidth + m_pInfo->nVisDist - 1) / m_pInfo->nVisDist + 2;
	m_nVisTileArrayHeight = (m_pInfo->nHeight + m_pInfo->nVisDist - 1) / m_pInfo->nVisDist + 2;

	m_pVisTile = new tagVisTile[m_nVisTileArrayWidth * m_nVisTileArrayHeight];
	if( !P_VALID(m_pVisTile) )
	{
		return FALSE;
	}

	// ���ɵ�ͼ�ű�
	m_pScript = g_ScriptMgr.GetMapScript(m_dwID);

	// ���ص�ͼ����
	if( FALSE == InitAllMapCreature() )
	{
		SAFE_DEL_ARRAY(m_pVisTile);
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ��ʼ�����Ե�ͼ�еĹ������ʱΪ���߳�
//-----------------------------------------------------------------------------------
BOOL Map::InitAllMapCreature()
{
	// ��ʼ������ID������
	m_CreatureIDGen.Init(m_pInfo);

	return InitAllFixedCreature() && InitAllSpawnPointCreature() && InitAllFixedDoor();
}

//-----------------------------------------------------------------------------------
// ��ʼ�����еĵ�ͼ�ڰڷŵĹ���
//-----------------------------------------------------------------------------------
BOOL Map::InitAllFixedCreature()
{
	// һ��һ���Ĵ�������
	tagMapCreatureInfo* pCreatureInfo = NULL;
	const tagCreatureProto* pProto = NULL;
	m_pInfo->mapCreatureInfo.ResetIterator();
	while( m_pInfo->mapCreatureInfo.PeekNext(pCreatureInfo) )
	{
		pProto = g_attRes.GetCreatureProto(pCreatureInfo->dwTypeID);
		if( !P_VALID(pProto) )
		{
			IMSG(_T("Detect a unknown creature in map, dwObjID=%u\r\n"), pCreatureInfo->dwTypeID);
			continue;
		}

		// ȡ��һ��ID
		DWORD dwID = m_CreatureIDGen.GetNewCreatureID();
		ASSERT( IS_CREATURE(dwID) );

		// ���ɳ�������ͳ�������
		Vector3 vFaceTo;
		vFaceTo.x = cosf(pCreatureInfo->fYaw * 3.1415927f / 180.0f);
		vFaceTo.z = sinf(pCreatureInfo->fYaw * 3.1415927f / 180.0f);
		vFaceTo.y = 0.0f;

		// ���ɹ���
		Creature* pCreature = Creature::Create(dwID, m_dwID, pProto, pCreatureInfo->vPos, vFaceTo, 
			ECST_Load, GT_INVALID, pCreatureInfo->bCollide, pCreatureInfo->patrolList, GT_INVALID, GT_INVALID, this);

		// ���뵽��ͼ��
		AddCreatureOnLoad(pCreature);

		// ����ǳ�Ѩ������س�Ѩ����
		if( pCreature->IsNest() )
		{
			InitNestCreature(pCreature);
		}

		// ����ǹ���С�ӣ������С�ӹ���
		if( pCreature->IsTeam() )
		{
			InitTeamCreature(pCreature);
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ��ʼ����ͨ��ͼˢ�ֵ�Ĺ������ʱΪ���߳�
//-----------------------------------------------------------------------------------
BOOL Map::InitAllSpawnPointCreature()
{
	// ��ÿһ��ˢ�ֵ�	
	tagMapSpawnPointInfo* pMapSpawnInfo = NULL;
	m_pInfo->mapSpawnPoint.ResetIterator();
	while(m_pInfo->mapSpawnPoint.PeekNext(pMapSpawnInfo))
	{
		ASSERT_P_VALID(pMapSpawnInfo);
		if (!P_VALID(pMapSpawnInfo)) continue;

		// ���ѡ��һ����
		const tagSSpawnPointProto* pSSpawnProto = g_attRes.GetSSpawnPointProto(pMapSpawnInfo->dwSpawnPointID);
		ASSERT_P_VALID(pSSpawnProto);
		if (!P_VALID(pSSpawnProto)) continue;

		INT nCandiNum	= 0;
		while (GT_VALID(pSSpawnProto->dwTypeIDs[nCandiNum]))
			nCandiNum++;
		if (0 == nCandiNum) continue;

		INT nIndex = IUTIL->Rand() % nCandiNum;
		const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(pSSpawnProto->dwTypeIDs[nIndex]);
		ASSERT_P_VALID(pCreatureProto);
		if (!P_VALID(pCreatureProto)) continue;

		// ��ȡid
		DWORD dwCreatureID = m_CreatureIDGen.GetNewCreatureID();

		// ���ɳ�������ͳ�������
		FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
		Vector3 vFaceTo;
		vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
		vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
		vFaceTo.y = 0.0f;

		// ������
		Creature* pCreature = Creature::Create(	dwCreatureID, GetMapID(), pCreatureProto, pMapSpawnInfo->vPos, vFaceTo, 
			ECST_SpawnPoint, pMapSpawnInfo->dwSpawnPointID, pMapSpawnInfo->bCollide, NULL);
		ASSERT_P_VALID(pCreature);
		if (!P_VALID(pCreature)) continue;

		// �����ͼ
		AddCreatureOnLoad(pCreature);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ��ʼ����ͼ�������ţ�����ʱΪ���߳�
//-----------------------------------------------------------------------------------
BOOL Map::InitAllFixedDoor()
{
	tagMapDoor* pMapDoor = NULL;
	const tagCreatureProto* pProto = NULL;
	m_pInfo->mapDoor.ResetIterator();
	while( m_pInfo->mapDoor.PeekNext(pMapDoor) )
	{
		pProto = g_attRes.GetCreatureProto(pMapDoor->dwTypeID);
		if( !P_VALID(pProto) )
		{
			IMSG(_T("Detect a unknown creature in map, dwObjID=%u\r\n"), pMapDoor->dwObjID);
			continue;
		}

		// ȡ��һ��ID
		DWORD dwID = m_DoorIDGen.GetValidDoorID();
		ASSERT( IS_DOOR(dwID) );

		// ���ɳ�������ͳ�������
		Vector3 vFaceTo;
		vFaceTo.x = cosf(pMapDoor->fYaw * 3.1415927f / 180.0f);
		vFaceTo.z = sinf(pMapDoor->fYaw * 3.1415927f / 180.0f);
		vFaceTo.y = 0.0f;

		// ���ɹ���
		Creature* pCreature = Creature::Create(dwID, m_dwID, pProto, pMapDoor->vPos, vFaceTo, 
			ECST_Load, GT_INVALID, TRUE, NULL, GT_INVALID, pMapDoor->dwObjID);

		if (!P_VALID(pCreature))
			continue;

		if (!pCreature->IsDoor())
		{
			Creature::Delete(pCreature);
			continue;
		}

		pCreature->SetDoorStatus(pMapDoor->bInitState, FALSE);

		// ���뵽��ͼ��
		AddCreatureOnLoad(pCreature);

		// ����ӳ���
		m_mapDoorObj2Gen.Add(pMapDoor->dwObjID, pCreature->GetID());
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ��ʼ����Ѩ����
//-----------------------------------------------------------------------------------
VOID Map::InitNestCreature(Creature* pCreature)
{
	ASSERT( P_VALID(pCreature) && pCreature->IsNest() );

	const tagCreatureProto* pProto = pCreature->GetProto();
	ASSERT( P_VALID(pProto) && P_VALID(pProto->pNest) );

	const tagNestProto* pNest = pProto->pNest;
	const Vector3& vSpawnCenter = pCreature->GetCurPos();

	Vector3 vPos;
	Vector3 vFaceTo;

	for(INT n = 0; n < pNest->nCreatureNum; n++)
	{
		DWORD dwCreatureTypeID = pNest->dwSpawnID[n];
		INT nNum = pNest->nSpawnMax[n];

		const tagCreatureProto* pSpawnedCreatureProto = g_attRes.GetCreatureProto(dwCreatureTypeID);

		ASSERT( P_VALID(pSpawnedCreatureProto) && nNum > 0 );

		for(INT m = 0; m < nNum; m++)
		{
			INT nNum = 0;
			// �ҵ�һ�������ߵ������
			while(TRUE)
			{
				vPos.x = FLOAT(IUTIL->Rand() % (pNest->nSpawnRadius * 2) - pNest->nSpawnRadius) + vSpawnCenter.x;
				vPos.z = FLOAT(IUTIL->Rand() % (pNest->nSpawnRadius * 2) - pNest->nSpawnRadius) + vSpawnCenter.z;

				if(pCreature->NeedCollide())
				{
					vPos.y = pCreature->GetCurPosTop().y;
					break;
				}
				else
				{
					if( IfCanGo(vPos.x, vPos.z) )
					{
						vPos.y = GetGroundHeight(vPos.x, vPos.z);
						break;
					}
				}

				++nNum;
				if(nNum > 100)
				{
					break;
				}
			}
			if(nNum > 20)
				continue;

			// ���һ������
			FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
			vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
			vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
			vFaceTo.y = 0.0f;

			// ȡ��һ��ID
			DWORD dwID = m_CreatureIDGen.GetNewCreatureID();
			ASSERT( IS_CREATURE(dwID) );

			// ���ɹ���
			Creature* pSpawnedCreature = Creature::Create(dwID, m_dwID, pSpawnedCreatureProto, 
				vPos, vFaceTo, ECST_Nest, pCreature->GetID(), pCreature->NeedCollide(), NULL);

			// ���뵽��ͼ��
			AddCreatureOnLoad(pSpawnedCreature);
		}
	}
}


//-----------------------------------------------------------------------------------
// ��ʼ��С�ӹ���
//-----------------------------------------------------------------------------------
VOID Map::InitTeamCreature(Creature* pCreature)
{
	ASSERT( P_VALID(pCreature) && pCreature->IsTeam() );

	const tagCreatureProto* pProto = pCreature->GetProto();
	ASSERT( P_VALID(pProto) && P_VALID(pProto->pNest) );

	const tagNestProto* pNest = pProto->pNest;
	const Vector3& vSpawnCenter = pCreature->GetCurPos();

	Vector3 vPos;
	Vector3 vFaceTo = pCreature->GetFaceTo();
	INT		nIndex = 0;
	INT		nNum = 0;
	INT		nNumIndex = 0;

	// �õ�����С�ӵĶ���
	const tagNPCTeamOrder* pTeamOrder = m_pTeamMgr->GetNPCTeamOrder(pNest->eOrderType);
	ASSERT(P_VALID(pTeamOrder));

	// ��������С��
	NPCTeam* pTeam = m_pTeamMgr->CreateTeam(pCreature);
	if(!P_VALID(pTeam))		return;

	// ���öӳ�С�ӣɣ�
	pCreature->SetTeamID(pTeam->GetID());
	std::vector<POINT>::const_iterator it = pTeamOrder->NPCOrder.begin();
	while (it != pTeamOrder->NPCOrder.end())
	{

		POINT point = *it;
		CVector2D vector((FLOAT)point.x, (FLOAT)point.y);
		DWORD dwCreatureTypeID = pNest->dwSpawnID[nIndex];
		nNum = pNest->nSpawnMax[nIndex];

		const tagCreatureProto* pTeamMemProto = g_attRes.GetCreatureProto(dwCreatureTypeID);
		if(!P_VALID(pTeamMemProto))
			return;

		// ȡ��һ��ID
		DWORD dwID = m_CreatureIDGen.GetNewCreatureID();
		ASSERT( IS_CREATURE(dwID) );

		// �������λ��
		vPos = m_pTeamMgr->CalTeamMemPos(pCreature, point, vFaceTo, pNest);

		// ���ɹ���
		Creature* pSpawnedCreature = Creature::Create(dwID, m_dwID, pTeamMemProto , 
			vPos, vFaceTo, ECST_Team, pCreature->GetID(), pCreature->NeedCollide(), NULL, pTeam->GetID());

		// ���뵽��ͼ��
		AddCreatureOnLoad(pSpawnedCreature);

		// ���뵽����С��
		pTeam->AddNPCTeamMem(pSpawnedCreature);

		++nNumIndex;
		if(nNumIndex >= nNum)
		{
			++nIndex;
			nNumIndex = 0;
		}

		++it;
	}
}

//-----------------------------------------------------------------------------------
// ���£�������õ�ͼ��MapMgr���̺߳�������
//-----------------------------------------------------------------------------------
VOID Map::Update()
{
	if( g_world.IsShutingdown() )
		return;
	CHECK_TIME_MAP(m_pInfo->szMapFileName);

	if( (EMT_Normal == m_pInfo->eType) && m_mapRole.Size() < 1 && m_mapSession.Size() < 1 )
	{
		// û���˵�ʱ��Ҳ����Ҫ���ĸ��²�����
		IdleUpdata();
		return;
	}

	UpdateSession();
	UpdateAllObjects();
	UpdateAllShops();
	UpdateAllCofC();
	// ��ͼˢ�� [4/20/2012 zhangzhihua]
	GetMonsterGen().Update();

	if( timeGetTime() - m_dwLastMinute >= 60*1000 )
	{ 
		m_dwLastMinute = timeGetTime();
		OnMinute();
	}
}

VOID Map::SyncDoorToUser(Role* pRole)
{
	if( g_world.IsShutingdown() )
		return;
	if(!P_VALID(pRole)) return;
	if(pRole->GetMapID() != GetMapID()) return;
	INT nVisIndex = WorldPosToVisIndex(pRole->GetCurPos());
	std::list<INT> visList;
	visList.push_back(nVisIndex);
	visList.push_back(nVisIndex - 1);
	visList.push_back(nVisIndex + 1);
	visList.push_back(nVisIndex - m_nVisTileArrayWidth);
	visList.push_back(nVisIndex - m_nVisTileArrayWidth+1);
	visList.push_back(nVisIndex - m_nVisTileArrayWidth-1);
	visList.push_back(nVisIndex + m_nVisTileArrayWidth);
	visList.push_back(nVisIndex + m_nVisTileArrayWidth+1);
	visList.push_back(nVisIndex + m_nVisTileArrayWidth-1);
	visList.sort();
	for(CreatureMap::TMapIterator itor = m_mapDeadDoor.Begin(); itor != m_mapDeadDoor.End(); itor++)
	{
		if(std::binary_search(visList.begin(), visList.end(), WorldPosToVisIndex(itor->second->GetCurPos())))
		{
			BYTE byMsg[1024] = {0};
			DWORD dwSize = CalMovementMsg(itor->second, byMsg, 1024);
			if(dwSize == 0)
				return;
			pRole->SendMessage(byMsg, dwSize);
		}
	}
	for(CreatureMap::TMapIterator itor = m_mapDoor.Begin(); itor != m_mapDoor.End(); itor++)
	{
		if(std::binary_search(visList.begin(), visList.end(), WorldPosToVisIndex(itor->second->GetCurPos())))
		{
			BYTE byMsg[1024] = {0};
			DWORD dwSize = CalMovementMsg(itor->second, byMsg, 1024);
			if(dwSize == 0)
				return;
			pRole->SendMessage(byMsg, dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------
// �����������㣬ÿ����ִ��һ��
//-------------------------------------------------------------------------------------
VOID Map::OnMinute()
{
	UpdateAllActiveCreature();
}

//-------------------------------------------------------------------------------------
// ���µ�ͼ�����������ҵ���Ϣ
//-------------------------------------------------------------------------------------
VOID Map::UpdateSession()
{
	if( g_world.IsShutingdown() )
		return;
	CHECK_TIME_WITH_ALLPARAM(NULL,100);
	PlayerSession* pSession = NULL;
	m_mapSession.ResetIterator();

	while( m_mapSession.PeekNext(pSession) )
	{
		pSession->Update();
	}
}

//--------------------------------------------------------------------------------------
// ���µ�ͼ�����������Ϸ�����״̬
//--------------------------------------------------------------------------------------
VOID Map::UpdateAllObjects()
{
	if( g_world.IsShutingdown() )
		return;
	// �������е�ͼ������
	RoleMap::TMapIterator itRole = m_mapRole.Begin();
	Role* pRole = NULL;

	CHECK_SPEC_BEGIN("RoleUpdate",100)
	while( m_mapRole.PeekNext(itRole, pRole) )
	{
		pRole->Update();
	}
	CHECK_SPEC_END

	// ���µ�ͼ������й���
	CHECK_SPEC_BEGIN("CretureUpdate",100)
	//UINT64 frequency = 1000,last,cur;
	//QueryPerformanceCounter((LARGE_INTEGER*)&last);
	//if( m_bCreatureChanged )
	//{
		ClearIdeCreature();	//�Ȳ鿴map���Ƿ��п�ָ��
		m_mapCurCreature = m_mapCreature;
		m_bCreatureChanged = FALSE;
	//}
	CreatureMap & tempMap = m_mapCurCreature;
	//QueryPerformanceCounter((LARGE_INTEGER*)&cur);
	//QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	//static int tick_time = 0;
	//if( tick_time++ % 5 == 0 )
	//{
	//	TCHAR bufT[100] = {0};
	//	if( frequency > 0 )
	//	{
	//		_sntprintf(bufT,10,_T("%u:%f\r\n"),GetCurrentThreadId(),((double)cur - last) / frequency);
	//		OutputDebugString(bufT);
	//	}
	//}
	CreatureMap::TMapIterator itCreature = tempMap.Begin();
	Creature* pCreature = NULL;

	while( tempMap.PeekNext(itCreature, pCreature) )
	{
		if( !P_VALID(pCreature) )
			continue;
		if(!pCreature->HasRemoveFlag()) pCreature->Update();
	}
	CHECK_SPEC_END

	// ������ԼС��Ϸ
	if( g_world.IsFairyContractOpen() && P_VALID(m_pFairyContractMgr) )
	{
		m_pFairyContractMgr->Update();
	}

	// ���µ�ͼ�������д�ˢ�µĹ���
	CHECK_SPEC_BEGIN("RespawnUpdate",100)
	CreatureMap::TMapIterator itDeadCreature = m_mapRespawnCreature.Begin();
	Creature* pDeadCreature = NULL;

	while( m_mapRespawnCreature.PeekNext(itDeadCreature, pDeadCreature) )
	{
		ECreatureReviveResult eRet = pDeadCreature->TryRevive();

		if( ECRR_Success == eRet )
		{
			// �����ɹ�
			m_mapRespawnCreature.Erase(pDeadCreature->GetID());
			m_mapDeadDoor.Erase(pDeadCreature->GetID());
			AddCreature(pDeadCreature);
		}
		else if( ECRR_Failed == eRet )
		{
			// ����ʧ��
		}
		else if( ECRR_NeedDestroy == eRet )
		{
			// ��̬���ɵģ���Ҫɾ����
			m_mapRespawnCreature.Erase(pDeadCreature->GetID());
			m_mapDeadDoor.Erase(pDeadCreature->GetID());
			// ��©����ֹû�дӸ���ɾ���ֵ�����
			RemoveCreatureFromMapEx(pDeadCreature);
			Creature::Delete(pDeadCreature);
		}
		else if( ECRR_NeedReplace == eRet )
		{
			// ˢ�ֵ����ɵģ���Ҫ�滻
			SSpawnPtReplaceCreature(pDeadCreature);
		}
		else
		{
			ASSERT(0);
		}
	}
	CHECK_SPEC_END

	// ���µ�����Ʒ
	CHECK_SPEC_BEGIN("GroundUpdate",100)
	TMap<INT64, tagGroundItem*>::TMapIterator it = m_mapGroundItem.Begin();
	tagGroundItem* pGroundItem = NULL;

	while( m_mapGroundItem.PeekNext(it, pGroundItem) )
	{
		switch(pGroundItem->Update())
		{
			// �Ƴ�����
		case EGIU_Remove:
			{
				// �Ƴ�����
				RemoveGroundItem(pGroundItem);

				// ������Ʒ
				pGroundItem->DestroyItem();

				// �ͷ��ڴ�
				SAFE_DEL(pGroundItem);
			}
			break;
			// ͬ������״̬
		case EGIU_Sync:
			{
				// ������Ʒ���ڵĿ��ӵ�ש����
				INT nVisIndex = WorldPosToVisIndex(pGroundItem->vPos);

				// �õ��Ź���
				tagVisTile* pVisTile[ED_End] = {0};
				GetVisTile(nVisIndex, pVisTile);

				// ͬ�������뵽�ͻ��˵���Һ�����
				SyncGroundItemState(pGroundItem, pVisTile);
			}
			break;
		case EGIU_Null:
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	CHECK_SPEC_END
}

//---------------------------------------------------------------------------------------
// ���¸õ�ͼ�������̵�
//---------------------------------------------------------------------------------------
VOID Map::UpdateAllShops()
{
	CHECK_TIME();
	Shop *pShop = NULL;
	m_mapShop.ResetIterator();

	while( m_mapShop.PeekNext(pShop) )
	{
		pShop->Update();
	}
}

//---------------------------------------------------------------------------------------
// ���¸õ�ͼ�������̻�
//---------------------------------------------------------------------------------------
VOID Map::UpdateAllCofC()
{
	CHECK_TIME();
	GuildCofC *pCofC = NULL;
	m_mapCofC.ResetIterator();

	while( m_mapCofC.PeekNext(pCofC) )
	{
		pCofC->Update();
	}
}

//---------------------------------------------------------------------------------------
// ��ʽ����һ����ң���ֻ���ɹ���õ�ͼ��MapMgr����
//---------------------------------------------------------------------------------------
VOID Map::AddRole(Role* pRole)
{
	ASSERT( P_VALID(pRole) );

	// ���뵽����б���
	m_mapRole.Add(pRole->GetID(), pRole);

	// ������ҵ�session���뵽session�б���
	PlayerSession* pSession = pRole->GetSession();
	if( P_VALID(pSession) )
	{
		m_mapSession.Add(pSession->GetSessionID(), pSession);
	}

	// ������ҵĵ�ͼ
	pRole->SetMap(this);
    

	// ��������
	FixPos(pRole->GetCurPos());

	// �����߶�
	pRole->GetMoveData().DropDownStandPoint();

	// ���ͽ����ͼ��Ϣ���ͻ���
	SendGotoNewMapToPlayer(pRole);

	// ���͸õ�ͼ�г�����Ч
	SendSceneEffect(pRole);

	// ����������ڵĿ��ӵ�ש����
	INT nVisIndex = WorldPosToVisIndex(pRole->GetCurPos());

	// �õ��Ź���
	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	// ͬ�������뵽�ͻ��˵���Һ�����
	SyncAddedUnits(pRole, pVisTile);

	// ������䵽9����֮��
	AddToVisTile(pRole, nVisIndex);

	// �����ͼ����
	pRole->CheckMapArea();

	// �����ͼ���������
	pRole->OnEnterMap();

	if(this->GetMapInfo()->bNoJump)
		pRole->SetState(ES_NoJump);
	else
		pRole->UnsetState(ES_NoJump);

	// ���»NPC
	UpdateAllActiveCreature(TRUE, pRole);

	//���½�ɫ�ļ���
	UpdateRoleSkill(pRole);

	// ���ýű�
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnPlayerEnter(pRole, this);
	}
}

//----------------------------------------------------------------------------------------
// �������Ϣ ʵʱˢ�� ����û�л���� ҲҪ����Ϣ
//----------------------------------------------------------------------------------------
VOID Map::UpdateAllActiveCreature(BOOL bSendMsg2SingleRole/* = FALSE*/, Role* pRole/* = NULL*/)
{
	//ƴ���䳤��Ϣ
	DWORD	dwSize  = sizeof(tagNS_SyncActiveCreaturesPos) + (m_mapActiveCreature.Size() - 1) * sizeof(tagACreaturePosInfo);
	MCREATE_MSG(pSend, dwSize, NS_SyncActiveCreaturesPos);

	Creature* pCreature = NULL;

	TMap<DWORD, Creature*>::TMapIterator iter = m_mapActiveCreature.Begin();
	while(m_mapActiveCreature.PeekNext(iter,pCreature))
	{
		if(!P_VALID(pCreature))
			continue;

		pSend->stData[pSend->nCreaturesNum].dwTypeID = pCreature->GetTypeID();
		pSend->stData[pSend->nCreaturesNum].fX = pCreature->GetCurPos().x;
		pSend->stData[pSend->nCreaturesNum++].fZ = pCreature->GetCurPos().z;
	}

	ASSERT(pSend->nCreaturesNum == m_mapActiveCreature.Size());

	if (bSendMsg2SingleRole && P_VALID(pRole))
		pRole->SendMessage(pSend, pSend->dwSize);
	else
		SendMapMessage(pSend,pSend->dwSize);
}

//----------------------------------------------------------------------------------------
// �õ���ײ�����
//----------------------------------------------------------------------------------------
NavCollider* Map::GetNavCollider()
{
	return GetNavMap()->GetCollider();
}

//----------------------------------------------------------------------------------------
// ��/�ر�ָ������
//----------------------------------------------------------------------------------------
VOID Map::OpenCloseDoor(DWORD dwObjID, BOOL bOpen)
{
	DWORD dwDoorID = m_mapDoorObj2Gen.Peek(dwObjID);
	if (!IS_DOOR(dwDoorID))	return;
	
	Creature* pDoor = m_mapCreature.Peek(dwDoorID);
	if (!P_VALID(pDoor))	return;
	
	pDoor->SetDoorStatus(bOpen);

	bool boolOpen = bOpen ? true : false;
	GetNavMap()->GetCollider()->EnableDoorCollide(dwObjID, !boolOpen);
	GetNavMap()->GetNPCNavMap()->OpenCloseDoor(dwObjID, boolOpen);
}

//----------------------------------------------------------------------------------------
// ��/�ر����е���
//----------------------------------------------------------------------------------------
VOID Map::OpenCloseAllDoors(BOOL bOpen)
{
	DWORD dwDoorID = GT_INVALID;
	TMap<DWORD, DWORD>::TMapIterator iter = m_mapDoorObj2Gen.Begin();
	while (m_mapDoorObj2Gen.PeekNext(iter, dwDoorID))
	{
		if (!IS_DOOR(dwDoorID))
			continue;

		Creature* pDoor = m_mapCreature.Peek(dwDoorID);
		if (!P_VALID(pDoor))
			continue;

		pDoor->SetDoorStatus(bOpen);
	}

	bool boolOpen = bOpen ? true : false;
	GetNavMap()->GetCollider()->EnableAllDoorsCollide(!boolOpen);
	GetNavMap()->GetNPCNavMap()->OpenCloseAllDoors(boolOpen);
}

//-----------------------------------------------------------------------------------------
// ��ͼ�ڼ���һ���������������ʱ��ӵģ����Ի�����Ҫͬ��
//-----------------------------------------------------------------------------------------
VOID Map::AddCreatureOnLoad(Creature* pCreature)
{
	ASSERT( P_VALID(pCreature) );
	m_bCreatureChanged = TRUE;
	// ���뵽�����б���
	m_mapCreature.Add(pCreature->GetID(), pCreature);

	// �����
	if(pCreature->IsActive())
		m_mapActiveCreature.Add(pCreature->GetID(), pCreature);

	if(pCreature->IsDoor())
		m_mapDoor.Add(pCreature->GetID(), pCreature);

	// ��������ĵ�ͼ
	pCreature->SetMap(this);

	// �����������ڵĿ��ӵ�ש����
	INT nVisIndex = WorldPosToVisIndex(pCreature->GetCurPos());

	// �������䵽�Ź���֮��
	AddToVisTile(pCreature, nVisIndex);

	// ������̵�ְ��NPC������ض�Ӧ���̵�
	if (ECT_NPC == pCreature->GetProto()->eType)
	{
		switch (pCreature->GetProto()->eFunctionType)
		{
		case EFNPCT_Shop:
			AddShop(pCreature->GetTypeID(), pCreature->GetShopID());
			break;

		case EFNPCT_CofC:
			AddCofC(pCreature->GetTypeID(), pCreature->GetShopID());
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------
// ��ͼ�м���һ�������������Ϸ����ʱ��ӣ�������Ҫͬ��
//-----------------------------------------------------------------------------------------
VOID Map::AddCreature(Creature* pCreature)
{
	ASSERT( P_VALID(pCreature) );
	m_bCreatureChanged = TRUE;
	// ���뵽�����б���
	m_mapCreature.Add(pCreature->GetID(), pCreature);

	// �����
	if(pCreature->IsActive())
		m_mapActiveCreature.Add(pCreature->GetID(), pCreature);

	if(pCreature->IsDoor())
		m_mapDoor.Add(pCreature->GetID(), pCreature);

	// ��������ĵ�ͼ
	pCreature->SetMap(this);

	// �����������ڵĿ��ӵ�ש����
	INT nVisIndex = WorldPosToVisIndex(pCreature->GetCurPos());

	// �õ��Ź���
	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	// ͬ�������뵽�ͻ��˵���Һ�����
	SyncAddedUnits(pCreature, pVisTile);

	// �������䵽9����֮��
	AddToVisTile(pCreature, nVisIndex);
}

//-----------------------------------------------------------------------------------------
// �ӵ�ͼ������һ��������������б�
//-----------------------------------------------------------------------------------------
VOID Map::RemoveCreature(Creature* pCreature)
{
	ASSERT( P_VALID(pCreature) );
	m_bCreatureChanged = TRUE;
	// �������б��������������
	m_mapCreature.Erase(pCreature->GetID());

	if(pCreature->IsActive())
		m_mapActiveCreature.Erase(pCreature->GetID());

	if(pCreature->IsDoor())
		m_mapDoor.Erase(pCreature->GetID());

	//// ����������ڵ�vistile
	//INT nVisIndex = pCreature->GetVisTileIndex();

	//// �õ��Ź���
	//tagVisTile* pVisTile[ED_End] = {0};
	//GetVisTile(nVisIndex, pVisTile);

	// �ӿ��ӵ�ש�����ߣ�����ͬ���ͻ���
	RemoveFromVisTile(pCreature);

	// ���뵽�����б��еȴ�����
	m_mapRespawnCreature.Add(pCreature->GetID(), pCreature);

	if(pCreature->IsDoor())
	{
		m_mapDeadDoor.Add(pCreature->GetID(), pCreature);
	}
}
// Jason 2010-2-7 
VOID				Map::RemoveCreatureFromMapEx(Creature* pCreature)
{
	if( P_VALID(pCreature) )
	{
		m_bCreatureChanged = TRUE;
		// �������б��������������
		m_mapCreature.Erase(pCreature->GetID());

		if(pCreature->IsActive())
			m_mapActiveCreature.Erase(pCreature->GetID());

		if(pCreature->IsDoor())
			m_mapDoor.Erase(pCreature->GetID());

		// ����������ڵ�vistile
		INT nVisIndex = pCreature->GetVisTileIndex();
		if( nVisIndex < 0 || nVisIndex > m_nVisTileArrayWidth * m_nVisTileArrayHeight )
			return;

		// �õ��Ź���
		tagVisTile* pVisTile[ED_End] = {0};
		GetVisTile(nVisIndex, pVisTile);

		// �ӿ��ӵ�ש�����ߣ�����ͬ���ͻ���
		RemoveFromVisTile(pCreature);
	}
}

//-----------------------------------------------------------------------------------------
// �ӵ�ͼ���Ƴ�����
//-----------------------------------------------------------------------------------------
VOID Map::RemovePet(Pet* pPet)
{
	if (!P_VALID(pPet))
	{
		ASSERT(0);
		return;
	}
	m_bCreatureChanged = TRUE;
	// �������б��������������
	m_mapCreature.Erase(pPet->GetID());

	pPet->SetMap(NULL);

	// �ӿ��ӵ�ש�����ߣ�����ͬ���ͻ���
	RemoveFromVisTile(pPet);

	// ͬ�����ͻ���
	SyncRemoveUnitToBigVisTile(pPet);
}


//-----------------------------------------------------------------------------------------
// ���뵽��ͼ�е�ĳ��VisTile
//-----------------------------------------------------------------------------------------
VOID Map::AddToVisTile(Unit* pUnit, INT nVisIndex)
{
	ASSERT( P_VALID(pUnit) );
	ASSERT( nVisIndex != GT_INVALID );

	pUnit->SetVisTileIndex(nVisIndex);
	if( pUnit->IsRole() )
		m_pVisTile[nVisIndex].mapRole.Add(pUnit->GetID(), static_cast<Role*>(pUnit));
	else
		m_pVisTile[nVisIndex].mapCreature.Add(pUnit->GetID(), static_cast<Creature*>(pUnit));

	if( pUnit->IsInStateInvisible() )
		m_pVisTile[nVisIndex].mapInvisibleUnit.Add(pUnit->GetID(), pUnit);
}

//-----------------------------------------------------------------------------------------
// ��ĳ�����ӵ�ש��ɾ��һ�����
//-----------------------------------------------------------------------------------------
VOID Map::RemoveFromVisTile(Unit* pUnit)
{
	ASSERT( P_VALID(pUnit) );

	INT nVisIndex = pUnit->GetVisTileIndex();
	ASSERT( GT_VALID(nVisIndex) );
	if( nVisIndex < 0 || nVisIndex > m_nVisTileArrayWidth * m_nVisTileArrayHeight )
		return;

	if( pUnit->IsRole() )
		m_pVisTile[nVisIndex].mapRole.Erase(pUnit->GetID());
	else
		m_pVisTile[nVisIndex].mapCreature.Erase(pUnit->GetID());

	if( pUnit->IsInStateInvisible() )
		m_pVisTile[nVisIndex].mapInvisibleUnit.Erase(pUnit->GetID());
	
	pUnit->SetVisTileIndex(GT_INVALID);
}

//-----------------------------------------------------------------------------------------
// ���絥λתΪ���ӵ�ש������
//-----------------------------------------------------------------------------------------
INT Map::WorldPosToVisIndex(const Vector3& pos)
{
	// ����Ƿ�
	if( !IsPosValid(pos) ) return GT_INVALID;

	INT nIndexX = INT(pos.x / TILE_SCALE / m_pInfo->nVisDist) + 1;
	INT nIndexZ = INT(pos.z / TILE_SCALE / m_pInfo->nVisDist) + 1;

	ASSERT( nIndexX > 0 && nIndexX < m_nVisTileArrayWidth - 1 );
	ASSERT( nIndexZ > 0 && nIndexZ < m_nVisTileArrayHeight - 1 );

	return nIndexZ * m_nVisTileArrayWidth + nIndexX;
}

//------------------------------------------------------------------------------------------
// �õ���Χ��ש
//------------------------------------------------------------------------------------------
VOID Map::GetVisTile(INT nVisIndex, tagVisTile* visTile[ED_End])
{
	// todo: ����ط���Ҫ��һ�£��жϲ��Ǻ��Ͻ�
	if( nVisIndex == GT_INVALID )
		return;

	// ��Ұ�ĵ�ש����
	INT m[ED_End] = {0};
	m[ED_Center]		= nVisIndex;
	m[ED_Left]			= nVisIndex - 1;
	m[ED_Right]			= nVisIndex + 1;
	m[ED_Top]			= nVisIndex - m_nVisTileArrayWidth;
	m[ED_Bottom]		= nVisIndex + m_nVisTileArrayWidth;
	m[ED_LeftTop]		= m[ED_Top] - 1;
	m[ED_RightTop]		= m[ED_Top] + 1;
	m[ED_LeftBottom]	= m[ED_Bottom] - 1;
	m[ED_RightBottom]	= m[ED_Bottom] + 1;

	for(INT n = 0 ; n < ED_End; n++)
	{
		if(m[n]>=0 && m[n] < m_nVisTileArrayWidth * m_nVisTileArrayHeight)
			visTile[n] = &m_pVisTile[m[n]];
	}
}

//------------------------------------------------------------------------------------------
// ����ĳ��������Լ���Χȷ���м���visTile�ڸþ��η�Χ��
//------------------------------------------------------------------------------------------
VOID Map::GetVisTile(Vector3& pos, FLOAT fRange, tagVisTile* visTile[ED_End])
{
	BOOL b[ED_End] = {0};

	INT m[ED_End] = {0};
	m[ED_Center]		= GT_INVALID;
	m[ED_Left]			= GT_INVALID;
	m[ED_Right]			= GT_INVALID;
	m[ED_Top]			= GT_INVALID;
	m[ED_Bottom]		= GT_INVALID;
	m[ED_LeftTop]		= GT_INVALID;
	m[ED_RightTop]		= GT_INVALID;
	m[ED_LeftBottom]	= GT_INVALID;
	m[ED_RightBottom]	= GT_INVALID;

	// ȡ�����ĵ��visIndex
	INT nIndexX = INT(pos.x / TILE_SCALE / m_pInfo->nVisDist) + 1;
	INT nIndexZ = INT(pos.z / TILE_SCALE / m_pInfo->nVisDist) + 1;

	ASSERT( nIndexX > 0 && nIndexX < m_nVisTileArrayWidth - 1 );
	ASSERT( nIndexZ > 0 && nIndexZ < m_nVisTileArrayHeight - 1 );

	// ȡ���߽������
	FLOAT fTempX = pos.x - fRange;
	INT nSrcIndexX = INT(fTempX / TILE_SCALE / m_pInfo->nVisDist) + 1;

	fTempX = pos.x + fRange;
	INT nDestIndexX = INT(fTempX / TILE_SCALE / m_pInfo->nVisDist) + 1;

	FLOAT fTempZ = pos.z - fRange;
	INT nSrcIndexZ = INT(fTempZ / TILE_SCALE / m_pInfo->nVisDist) + 1;

	fTempZ = pos.z + fRange;
	INT nDestIndexZ = INT(fTempZ / TILE_SCALE / m_pInfo->nVisDist) + 1;


	// �����Ƿ����
	b[ED_Center] = TRUE;
	if( nSrcIndexX < nIndexX )
	{
		b[ED_Left] = TRUE;	
	}
	if( nDestIndexX > nIndexX )
	{
		b[ED_Right] = TRUE;
	}
	if( nSrcIndexZ < nIndexZ )
	{
		b[ED_Top] = TRUE;
	}
	if( nDestIndexZ > nIndexZ )
	{
		b[ED_Bottom] = TRUE;
	}

	b[ED_LeftTop]		=	b[ED_Left] && b[ED_Top];
	b[ED_LeftBottom]	=	b[ED_Left] && b[ED_Bottom];
	b[ED_RightTop]		=	b[ED_Right] && b[ED_Top];
	b[ED_RightBottom]	=	b[ED_Right] && b[ED_Bottom];


	m[ED_Center] = nIndexZ * m_nVisTileArrayWidth + nIndexX;

	if( b[ED_Left] )		m[ED_Left] = m[ED_Center] - 1;
	if( b[ED_Right] )		m[ED_Right] = m[ED_Center] + 1;
	if( b[ED_Top] )			m[ED_Top] = m[ED_Center] - m_nVisTileArrayWidth;
	if( b[ED_Bottom] )		m[ED_Bottom] = m[ED_Center] + m_nVisTileArrayWidth;
	if( b[ED_LeftTop] )		m[ED_LeftTop] = m[ED_Top] - 1;
	if( b[ED_RightTop] )	m[ED_RightTop] = m[ED_Top] + 1;
	if( b[ED_LeftBottom] )	m[ED_LeftBottom] = m[ED_Bottom] - 1;
	if( b[ED_RightBottom] )	m[ED_RightBottom] = m[ED_Bottom] + 1;

	for(INT n = 0 ; n < ED_End; n++)
	{
		if( m[n] != GT_INVALID )
			visTile[n] = &m_pVisTile[m[n]];
	}

}

INT	Map::GetVisTileEx(Vector3& pos, FLOAT fRange, vector<tagVisTile*>& visTileVec)
{
	if( fRange <= 0 )
		return 0;

	// ȡ�����ĵ��visIndex
	INT nIndexX = INT(pos.x / TILE_SCALE / m_pInfo->nVisDist) + 1;
	INT nIndexZ = INT(pos.z / TILE_SCALE / m_pInfo->nVisDist) + 1;

	if(!( nIndexX > 0 && nIndexX < m_nVisTileArrayWidth - 1 ))
		return 0;
	if(!( nIndexZ > 0 && nIndexZ < m_nVisTileArrayHeight - 1 ))
		return 0;

	// ȡ���߽������
	FLOAT fTempX = pos.x - fRange;
	INT nSrcIndexX = max(INT(fTempX / TILE_SCALE / m_pInfo->nVisDist) + 1, 0);


	fTempX = pos.x + fRange;
	INT nDestIndexX = min(INT(fTempX / TILE_SCALE / m_pInfo->nVisDist) + 1, m_nVisTileArrayWidth-1);

	FLOAT fTempZ = pos.z - fRange;
	INT nSrcIndexZ = max(INT(fTempZ / TILE_SCALE / m_pInfo->nVisDist) + 1, 0);

	fTempZ = pos.z + fRange;
	INT nDestIndexZ = min(INT(fTempZ / TILE_SCALE / m_pInfo->nVisDist) + 1, m_nVisTileArrayHeight-1);

	nSrcIndexX = nSrcIndexZ * m_nVisTileArrayWidth + nSrcIndexX;
	nDestIndexX = nDestIndexZ * m_nVisTileArrayWidth + nDestIndexX;

	INT width = nDestIndexX % m_nVisTileArrayWidth - nSrcIndexX % m_nVisTileArrayWidth;

	for( int x = nSrcIndexX ; width > 0 && x < m_nVisTileArrayWidth * m_nVisTileArrayHeight;  )
	{
		for( int i = x; i < x + width; ++i )
		{
			visTileVec.push_back(&m_pVisTile[i]);
		}
		x += m_nVisTileArrayWidth;
		if( x > nDestIndexX )
			break;
	}

	return visTileVec.size();
}

//------------------------------------------------------------------------------------------
// �õ���Ҹı�λ�ú��뿪��Ұ�ĵ�ש
//-------------------------------------------------------------------------------------------
VOID Map::GetVisTileDec(INT nOldVisIndex, INT nNewVisIndex, tagVisTile* visTile[ED_End])
{
	if( nOldVisIndex == GT_INVALID || nNewVisIndex == GT_INVALID )
		return;

	if( nNewVisIndex == nOldVisIndex )
		return;

	INT nOldVisIndexX = nOldVisIndex % m_nVisTileArrayWidth;
	INT nOldVisIndexZ = nOldVisIndex / m_nVisTileArrayWidth;
	INT nNewVisIndexX = nNewVisIndex % m_nVisTileArrayWidth;
	INT nNewVisIndexZ = nNewVisIndex / m_nVisTileArrayWidth;

	INT m[ED_End] = {0};
	m[ED_Center]		= GT_INVALID;
	m[ED_Left]			= GT_INVALID;
	m[ED_Right]			= GT_INVALID;
	m[ED_Top]			= GT_INVALID;
	m[ED_Bottom]		= GT_INVALID;
	m[ED_LeftTop]		= GT_INVALID;
	m[ED_RightTop]		= GT_INVALID;
	m[ED_LeftBottom]	= GT_INVALID;
	m[ED_RightBottom]	= GT_INVALID;

	if( abs(nOldVisIndexX - nNewVisIndexX) >= 3
		|| abs(nOldVisIndexZ - nNewVisIndexZ) >= 3 )
	{
		m[ED_Center]		= nOldVisIndex;
		m[ED_Left]			= nOldVisIndex - 1;
		m[ED_Right]			= nOldVisIndex + 1;
		m[ED_Top]			= nOldVisIndex - m_nVisTileArrayWidth;
		m[ED_Bottom]		= nOldVisIndex + m_nVisTileArrayWidth;
		m[ED_LeftTop]		= m[ED_Top] - 1;
		m[ED_RightTop]		= m[ED_Top] + 1;
		m[ED_LeftBottom]	= m[ED_Bottom] - 1;
		m[ED_RightBottom]	= m[ED_Bottom] + 1;

		for(INT n = 0; n < ED_End; n++)
		{
			visTile[n] = &m_pVisTile[m[n]];
		}
		return;
	}

	// X����ƶ�
	if( nNewVisIndexX < nOldVisIndexX )
	{
		m[ED_Right]			= nOldVisIndex + 1;
		m[ED_RightTop]		= m[ED_Right] - m_nVisTileArrayWidth;
		m[ED_RightBottom]	= m[ED_Right] + m_nVisTileArrayWidth;

		if( nNewVisIndexX - nOldVisIndexX == -2 )
		{
			m[ED_Center]	= nOldVisIndex;
			m[ED_Top]		= m[ED_Center] - m_nVisTileArrayWidth;
			m[ED_Bottom]	= m[ED_Center] + m_nVisTileArrayWidth;
		}
	}
	// X�ұ��ƶ�
	else if( nNewVisIndexX > nOldVisIndexX )
	{
		m[ED_Left]			= nOldVisIndex - 1;
		m[ED_LeftTop]		= m[ED_Left] - m_nVisTileArrayWidth;
		m[ED_LeftBottom]	= m[ED_Left] + m_nVisTileArrayWidth;

		if( nNewVisIndexX - nOldVisIndexX == 2 )
		{
			m[ED_Center]	= nOldVisIndex;
			m[ED_Top]		= m[ED_Center] - m_nVisTileArrayWidth;
			m[ED_Bottom]	= m[ED_Center] + m_nVisTileArrayWidth;
		}
	}
	// Xû���ƶ�
	else
	{

	}

	// Z�ϲ��ƶ�
	if( nNewVisIndexZ < nOldVisIndexZ )
	{
		m[ED_Bottom]		= nOldVisIndex + m_nVisTileArrayWidth;
		m[ED_LeftBottom]	= m[ED_Bottom] - 1;
		m[ED_RightBottom]	= m[ED_Bottom] + 1;

		if( nNewVisIndexZ - nOldVisIndexZ == -2 )
		{
			m[ED_Center]	= nOldVisIndex;
			m[ED_Left]		= m[ED_Center] - 1;
			m[ED_Bottom]	= m[ED_Center] + 1;
		}
	}
	// Z�²��ƶ�
	else if( nNewVisIndexZ > nOldVisIndexZ )
	{
		m[ED_Top]			= nOldVisIndex - m_nVisTileArrayWidth;
		m[ED_LeftTop]		= m[ED_Top] - 1;
		m[ED_RightTop]		= m[ED_Top] + 1;

		if( nNewVisIndexZ - nOldVisIndexZ == 2 )
		{
			m[ED_Center]	= nOldVisIndex;
			m[ED_Left]		= m[ED_Center] - 1;
			m[ED_Bottom]	= m[ED_Center] + 1;
		}
	}
	// Zû���ƶ�
	else
	{

	}

	// ͳ�������
	for(INT n = 0; n < ED_End; n++)
	{
		if( m[n] != GT_INVALID )
			visTile[n] = &m_pVisTile[m[n]];
	}
}

//------------------------------------------------------------------------------------------
// �õ���Ҹı�λ�ú������Ұ�ĵ�ש
//-------------------------------------------------------------------------------------------
VOID Map::GetVisTileAdd(INT nOldVisIndex, INT nNewVisIndex, tagVisTile* visTile[ED_End])
{
	if( nOldVisIndex == GT_INVALID || nNewVisIndex == GT_INVALID )
		return;

	if( nNewVisIndex == nOldVisIndex )
		return;

	INT nOldVisIndexX = nOldVisIndex % m_nVisTileArrayWidth;
	INT nOldVisIndexZ = nOldVisIndex / m_nVisTileArrayWidth;
	INT nNewVisIndexX = nNewVisIndex % m_nVisTileArrayWidth;
	INT nNewVisIndexZ = nNewVisIndex / m_nVisTileArrayWidth;

	INT m[ED_End] = {0};
	m[ED_Center]		= GT_INVALID;
	m[ED_Left]			= GT_INVALID;
	m[ED_Right]			= GT_INVALID;
	m[ED_Top]			= GT_INVALID;
	m[ED_Bottom]		= GT_INVALID;
	m[ED_LeftTop]		= GT_INVALID;
	m[ED_RightTop]		= GT_INVALID;
	m[ED_LeftBottom]	= GT_INVALID;
	m[ED_RightBottom]	= GT_INVALID;

	if( abs(nOldVisIndexX - nNewVisIndexX) >= 3
		|| abs(nOldVisIndexZ - nNewVisIndexZ) >= 3 )
	{
		m[ED_Center]		= nNewVisIndex;
		m[ED_Left]			= nNewVisIndex - 1;
		m[ED_Right]			= nNewVisIndex + 1;
		m[ED_Top]			= nNewVisIndex - m_nVisTileArrayWidth;
		m[ED_Bottom]		= nNewVisIndex + m_nVisTileArrayWidth;
		m[ED_LeftTop]		= m[ED_Top] - 1;
		m[ED_RightTop]		= m[ED_Top] + 1;
		m[ED_LeftBottom]	= m[ED_Bottom] - 1;
		m[ED_RightBottom]	= m[ED_Bottom] + 1;

		for(INT n = 0; n < ED_End; n++)
		{
			visTile[n] = &m_pVisTile[m[n]];
		}
		return;
	}

	// X����ƶ�
	if( nNewVisIndexX < nOldVisIndexX )
	{
		m[ED_Left]			= nNewVisIndex - 1;
		m[ED_LeftTop]		= m[ED_Left] - m_nVisTileArrayWidth;
		m[ED_LeftBottom]	= m[ED_Left] + m_nVisTileArrayWidth;

		if( nNewVisIndexX - nOldVisIndexX == -2 )
		{
			m[ED_Center]	= nNewVisIndex;
			m[ED_Top]		= m[ED_Center] - m_nVisTileArrayWidth;
			m[ED_Bottom]	= m[ED_Center] + m_nVisTileArrayWidth;
		}
	}
	// X�ұ��ƶ�
	else if( nNewVisIndexX > nOldVisIndexX )
	{
		m[ED_Right]			= nNewVisIndex + 1;
		m[ED_RightTop]		= m[ED_Right] - m_nVisTileArrayWidth;
		m[ED_RightBottom]	= m[ED_Right] + m_nVisTileArrayWidth;

		if( nNewVisIndexX - nOldVisIndexX == 2 )
		{
			m[ED_Center]	= nNewVisIndex;
			m[ED_Top]		= m[ED_Center] - m_nVisTileArrayWidth;
			m[ED_Bottom]	= m[ED_Center] + m_nVisTileArrayWidth;
		}
	}
	// Xû���ƶ�
	else
	{

	}

	// Z�ϲ��ƶ�
	if( nNewVisIndexZ < nOldVisIndexZ )
	{
		m[ED_Top]		= nNewVisIndex - m_nVisTileArrayWidth;
		m[ED_LeftTop]	= m[ED_Top] - 1;
		m[ED_RightTop]	= m[ED_Top] + 1;

		if( nNewVisIndexZ - nOldVisIndexZ == -2 )
		{
			m[ED_Center]	= nNewVisIndex;
			m[ED_Left]		= m[ED_Center] - 1;
			m[ED_Bottom]	= m[ED_Center] + 1;
		}
	}
	// Z�²��ƶ�
	else if( nNewVisIndexZ > nOldVisIndexZ )
	{
		m[ED_Bottom]		= nNewVisIndex + m_nVisTileArrayWidth;
		m[ED_LeftBottom]	= m[ED_Bottom] - 1;
		m[ED_RightBottom]	= m[ED_Bottom] + 1;

		if( nNewVisIndexZ - nOldVisIndexZ == 2 )
		{
			m[ED_Center]	= nNewVisIndex;
			m[ED_Left]		= m[ED_Center] - 1;
			m[ED_Bottom]	= m[ED_Center] + 1;
		}
	}
	// Zû���ƶ�
	else
	{

	}

	// ͳ�������
	for(INT n = 0; n < ED_End; n++)
	{
		if( m[n] != GT_INVALID )
			visTile[n] = &m_pVisTile[m[n]];
	}
}

//---------------------------------------------------------------------------------------------
// �õ���Ҹı�λ�ú������Ұ�ĵ�ש���뿪��Ұ�ĵ�ש
//---------------------------------------------------------------------------------------------
VOID Map::GetVisTileAddAndDec(INT nOldVisIndex, INT nNewVisIndex, tagVisTile* visTileAdd[ED_End], tagVisTile* visTileDec[ED_End])
{
	if( nOldVisIndex == GT_INVALID || nNewVisIndex == GT_INVALID )
		return;

	if( nNewVisIndex == nOldVisIndex )
		return;

	INT nOldVisIndexX = nOldVisIndex % m_nVisTileArrayWidth;
	INT nOldVisIndexZ = nOldVisIndex / m_nVisTileArrayWidth;
	INT nNewVisIndexX = nNewVisIndex % m_nVisTileArrayWidth;
	INT nNewVisIndexZ = nNewVisIndex / m_nVisTileArrayWidth;

	// ������Ұ�ĵ�ש����
	INT m[ED_End] = {0};
	m[ED_Center]		= GT_INVALID;
	m[ED_Left]			= GT_INVALID;
	m[ED_Right]			= GT_INVALID;
	m[ED_Top]			= GT_INVALID;
	m[ED_Bottom]		= GT_INVALID;
	m[ED_LeftTop]		= GT_INVALID;
	m[ED_RightTop]		= GT_INVALID;
	m[ED_LeftBottom]	= GT_INVALID;
	m[ED_RightBottom]	= GT_INVALID;

	// �뿪��Ұ�ĵ�ש����
	INT r[ED_End] = {0};
	r[ED_Center]		= GT_INVALID;
	r[ED_Left]			= GT_INVALID;
	r[ED_Right]			= GT_INVALID;
	r[ED_Top]			= GT_INVALID;
	r[ED_Bottom]		= GT_INVALID;
	r[ED_LeftTop]		= GT_INVALID;
	r[ED_RightTop]		= GT_INVALID;
	r[ED_LeftBottom]	= GT_INVALID;
	r[ED_RightBottom]	= GT_INVALID;


	if( abs(nOldVisIndexX - nNewVisIndexX) >= 3
		|| abs(nOldVisIndexZ - nNewVisIndexZ) >= 3 )
	{
		// ������Ұ�Ŀ��ӵ�ש
		m[ED_Center]		= nNewVisIndex;
		m[ED_Left]			= nNewVisIndex - 1;
		m[ED_Right]			= nNewVisIndex + 1;
		m[ED_Top]			= nNewVisIndex - m_nVisTileArrayWidth;
		m[ED_Bottom]		= nNewVisIndex + m_nVisTileArrayWidth;
		m[ED_LeftTop]		= m[ED_Top] - 1;
		m[ED_RightTop]		= m[ED_Top] + 1;
		m[ED_LeftBottom]	= m[ED_Bottom] - 1;
		m[ED_RightBottom]	= m[ED_Bottom] + 1;

		// �뿪��Ұ�Ŀ��ӵ�ש
		r[ED_Center]		= nOldVisIndex;
		r[ED_Left]			= nOldVisIndex - 1;
		r[ED_Right]			= nOldVisIndex + 1;
		r[ED_Top]			= nOldVisIndex - m_nVisTileArrayWidth;
		r[ED_Bottom]		= nOldVisIndex + m_nVisTileArrayWidth;
		r[ED_LeftTop]		= r[ED_Top] - 1;
		r[ED_RightTop]		= r[ED_Top] + 1;
		r[ED_LeftBottom]	= r[ED_Bottom] - 1;
		r[ED_RightBottom]	= r[ED_Bottom] + 1;

		for(INT n = 0; n < ED_End; n++)
		{
			visTileAdd[n] = &m_pVisTile[m[n]];
			visTileDec[n] = &m_pVisTile[r[n]];
		}
		return;
	}

	// X����ƶ�
	if( nNewVisIndexX < nOldVisIndexX )
	{
		// ������Ұ�Ŀ��ӵ�ש
		m[ED_Left]			= nNewVisIndex - 1;
		m[ED_LeftTop]		= m[ED_Left] - m_nVisTileArrayWidth;
		m[ED_LeftBottom]	= m[ED_Left] + m_nVisTileArrayWidth;

		// �뿪��Ұ�Ŀ��ӵ�ש
		r[ED_Right]			= nOldVisIndex + 1;
		r[ED_RightTop]		= r[ED_Right] - m_nVisTileArrayWidth;
		r[ED_RightBottom]	= r[ED_Right] + m_nVisTileArrayWidth;

		if( nNewVisIndexX - nOldVisIndexX == -2 )
		{
			// ������Ұ�Ŀ��ӵ�ש
			m[ED_Center]	= nNewVisIndex;
			m[ED_Top]		= m[ED_Center] - m_nVisTileArrayWidth;
			m[ED_Bottom]	= m[ED_Center] + m_nVisTileArrayWidth;

			// �뿪��Ұ�Ŀ��ӵ�ש
			r[ED_Center]	= nOldVisIndex;
			r[ED_Top]		= r[ED_Center] - m_nVisTileArrayWidth;
			r[ED_Bottom]	= r[ED_Center] + m_nVisTileArrayWidth;
		}
	}
	// X�ұ��ƶ�
	else if( nNewVisIndexX > nOldVisIndexX )
	{
		// ������Ұ�Ŀ��ӵ�ש
		m[ED_Right]			= nNewVisIndex + 1;
		m[ED_RightTop]		= m[ED_Right] - m_nVisTileArrayWidth;
		m[ED_RightBottom]	= m[ED_Right] + m_nVisTileArrayWidth;

		// �뿪��Ұ�Ŀ��ӵ�ש
		r[ED_Left]			= nOldVisIndex - 1;
		r[ED_LeftTop]		= r[ED_Left] - m_nVisTileArrayWidth;
		r[ED_LeftBottom]	= r[ED_Left] + m_nVisTileArrayWidth;

		if( nNewVisIndexX - nOldVisIndexX == 2 )
		{
			// ������Ұ�Ŀ��ӵ�ש
			m[ED_Center]	= nNewVisIndex;
			m[ED_Top]		= m[ED_Center] - m_nVisTileArrayWidth;
			m[ED_Bottom]	= m[ED_Center] + m_nVisTileArrayWidth;

			// �뿪��Ұ�Ŀ��ӵ�ש
			r[ED_Center]	= nOldVisIndex;
			r[ED_Top]		= r[ED_Center] - m_nVisTileArrayWidth;
			r[ED_Bottom]	= r[ED_Center] + m_nVisTileArrayWidth;
		}
	}
	// Xû���ƶ�
	else
	{

	}

	// Z�ϲ��ƶ�
	if( nNewVisIndexZ < nOldVisIndexZ )
	{
		// ������Ұ�Ŀ��ӵ�ש
		m[ED_Top]			= nNewVisIndex - m_nVisTileArrayWidth;
		m[ED_LeftTop]		= m[ED_Top] - 1;
		m[ED_RightTop]		= m[ED_Top] + 1;

		// �뿪��Ұ�Ŀ��ӵ�ש
		r[ED_Bottom]		= nOldVisIndex + m_nVisTileArrayWidth;
		r[ED_LeftBottom]	= r[ED_Bottom] - 1;
		r[ED_RightBottom]	= r[ED_Bottom] + 1;


		if( nNewVisIndexZ - nOldVisIndexZ == -2 )
		{
			// ������Ұ�Ŀ��ӵ�ש
			m[ED_Center]	= nNewVisIndex;
			m[ED_Left]		= m[ED_Center] - 1;
			m[ED_Bottom]	= m[ED_Center] + 1;

			// �뿪��Ұ�Ŀ��ӵ�ש
			r[ED_Center]	= nOldVisIndex;
			r[ED_Left]		= r[ED_Center] - 1;
			r[ED_Bottom]	= r[ED_Center] + 1;

		}
	}
	// Z�²��ƶ�
	else if( nNewVisIndexZ > nOldVisIndexZ )
	{
		// ������Ұ�Ŀ��ӵ�ש
		m[ED_Bottom]		= nNewVisIndex + m_nVisTileArrayWidth;
		m[ED_LeftBottom]	= m[ED_Bottom] - 1;
		m[ED_RightBottom]	= m[ED_Bottom] + 1;

		// �뿪��Ұ�Ŀ��ӵ�ש
		r[ED_Top]			= nOldVisIndex - m_nVisTileArrayWidth;
		r[ED_LeftTop]		= r[ED_Top] - 1;
		r[ED_RightTop]		= r[ED_Top] + 1;


		if( nNewVisIndexZ - nOldVisIndexZ == 2 )
		{
			// ������Ұ�Ŀ��ӵ�ש
			m[ED_Center]	= nNewVisIndex;
			m[ED_Left]		= m[ED_Center] - 1;
			m[ED_Bottom]	= m[ED_Center] + 1;

			// �뿪��Ұ�Ŀ��ӵ�ש
			r[ED_Center]	= nOldVisIndex;
			r[ED_Left]		= r[ED_Center] - 1;
			r[ED_Bottom]	= r[ED_Center] + 1;
		}
	}
	// Zû���ƶ�
	else
	{

	}

	// ͳ�������
	for(INT n = 0; n < ED_End; n++)
	{
		if( m[n] != GT_INVALID )
			visTileAdd[n] = &m_pVisTile[m[n]];

		if( r[n] != GT_INVALID )
			visTileDec[n] = &m_pVisTile[r[n]];
	}
}

//------------------------------------------------------------------------------------------
// ���;Ź���Χ�ڵ���Ϣ
//------------------------------------------------------------------------------------------
VOID Map::SendBigVisTileMsg(Unit *pSelf, const LPVOID pMsg, const DWORD dwSize)
{
	if( g_world.IsShutingdown() )
		return;
	if( !P_VALID(pSelf) ) return;
	if( !P_VALID(pMsg) ) return;

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(pSelf->GetVisTileIndex(), pVisTile);

	SendBigVisTileMsg(pVisTile, pMsg, dwSize);
}

//------------------------------------------------------------------------------------------
// ���;Ź���Χ�ڵ���Ϣ
//------------------------------------------------------------------------------------------
VOID Map::SendBigVisTileMsg(tagVisTile *pVisTile[ED_End], const LPVOID pMsg, const DWORD dwSize)
{
	if( g_world.IsShutingdown() )
		return;
	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;
		
		// �ҵ�ÿ����ש����
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD,Role*>::TMapIterator it = mapRole.Begin();
		//mapRole.ResetIterator();
		Role* pRole = NULL;

		while( mapRole.PeekNext(it,pRole) )
		{
			if( P_VALID(pRole) )
			{
				pRole->SendMessage(pMsg, dwSize);

				//if (ECamp_Null != pRole->GetCampType())
				//{
				//	tagNS_SyncCamp syncCamp;
				//	syncCamp.dwRoleID = pRole->GetID();
				//	syncCamp.eCamp = pRole->GetCampType();
				//	pRole->SendMessage(&syncCamp, syncCamp.dwSize);
				//}

			}
		}
	}
}

//------------------------------------------------------------------------------------------
// ������Ϣ������ͼ�ڵ����
//------------------------------------------------------------------------------------------
VOID Map::SendMapMessage(const LPVOID pMsg, const DWORD dwSize)
{
	if(!P_VALID(pMsg))	return;
	if( g_world.IsShutingdown() )
		return;
	Role *pRole = NULL;

	TMap<DWORD, Role*>::TMapIterator it = m_mapRole.Begin();
	while (m_mapRole.PeekNext(it, pRole))
	{
		if( P_VALID(pRole) && P_VALID(pRole->GetSession()) )
		{
			pRole->GetSession()->SendMessage(pMsg, dwSize);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ͬ�����ӵ�ש�ڵ�����λ�����
//--------------------------------------------------------------------------------------------
VOID Map::SyncBigVisTileInvisUnit2Role(Role* pRole, tagVisTile *pVisTile[ED_End])
{
	if( g_world.IsShutingdown() )
		return;
	// 1.�Ƿ���̽������
	if( !pRole->HasDectiveSkill() )
		return;
	
	BYTE			byMsg[1024] = {0};
	TList<DWORD>	listRemove;
	
	// ������
	//if ( !pRole->IsInRoleState(ERS_PK))

		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;

			// �ҵ�ÿ����ש�ĵ�λ
			TMap<DWORD, Unit*>& mapUnit = pVisTile[n]->mapInvisibleUnit;
			mapUnit.ResetIterator();
			Unit* pUnit = NULL;

			while( mapUnit.PeekNext(pUnit) )
			{
				if( !P_VALID(pUnit) )
					continue;

				// 2.�Ƿ���ȫ�ɼ�
				if( (pRole->CanSeeTargetEntirely(pUnit)) )
					continue;

					// 3.�ڿ����б���
					if( pRole->IsInVisList(pUnit->GetID()) )
					{
						// �ڿ��ӷ�Χ��
						if( pRole->IsInVisDist(pUnit) )
							continue;

						// ���ڿ��ӷ�Χ��
						listRemove.PushBack(pUnit->GetID());

						pRole->RemoveFromVisList(pUnit->GetID());

						continue;
					}

					// 4.����A�Ŀ����б���
					// 4.1����A���ӷ�Χ��
					if( !pRole->IsInVisDist(pUnit) )
						continue;

					// 4.2 ��A���ӷ�Χ��
					DWORD dwSize = CalMovementMsg(pUnit, byMsg, 1024);
					if(dwSize == 0)
						continue;

					pRole->SendMessage(byMsg, dwSize);

					pRole->Add2VisList(pUnit->GetID());
			}
		}


	// ���list��Ϊ�գ���self����ҵĻ������͸������Ҫɾ����Զ�����
	INT nListSize = listRemove.Size();
	if( nListSize <= 0 )
		return;

	DWORD dwSize = sizeof(tagNS_RemoveRemote) + (nListSize-1)*sizeof(DWORD);
	MCREATE_MSG(pSend, dwSize, NS_RemoveRemote);

	for(INT n = 0; n < nListSize; n++)
	{
		pSend->dwRoleID[n] = listRemove.PopFront();
	}

	pRole->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pMsg);
}

//--------------------------------------------------------------------------------------------
// �����ӵ�ש�����п��ӵ�λ���������Ʒͬ�������
//--------------------------------------------------------------------------------------------
VOID Map::SyncBigVisTileVisUnitAndGroundItem2Role(Role* pRole, tagVisTile *pVisTile[ED_End])
{
	ASSERT( P_VALID(pRole) );
	if( g_world.IsShutingdown() )
		return;

	BYTE byMsg[1024] = {0};
	DWORD dwSize = 0;	

	tagNS_SyncGroundItem send;

	// �����Ź����б�
	for(INT n = 0; n < ED_End; n++)
	{
		if( !P_VALID(pVisTile[n]) )
			continue;

		// ͬ��������״̬�¹�
		TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;
		mapCreature.ResetIterator();
		Creature* pCreature = NULL;

		while( mapCreature.PeekNext(pCreature) )
		{
			if( pCreature->IsInStateInvisible() )
				continue;

			if( IsShieldingNPC(pCreature) )
				continue;

			dwSize = CalMovementMsg(pCreature, byMsg, 1024);
			if( dwSize == 0 )
				continue;
			pRole->SendMessage(byMsg, dwSize);

			//if (ECamp_Null != pCreature->GetCampType())
			//{
			//	tagNS_SyncCamp syncCamp;
			//	syncCamp.dwRoleID = pCreature->GetID();
			//	syncCamp.eCamp = pCreature->GetCampType();
			//	pRole->SendMessage(&syncCamp, syncCamp.dwSize);
			//}
		}

		// ͬ��������״̬��Զ�����
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator itRole = mapRole.Begin();
		//mapRole.ResetIterator();
		Role* pRemoteRole = NULL;

		while( mapRole.PeekNext(itRole,pRemoteRole) )
		{
			if( pRemoteRole->IsInStateInvisible() )
				continue;

			dwSize = CalMovementMsg(pRemoteRole, byMsg, 1024);
			if( dwSize == 0 )
				continue;
			pRole->SendMessage(byMsg, dwSize);
			//if (ECamp_Null != pRemoteRole->GetCampType())
			//{
			//	tagNS_SyncCamp syncCamp;
			//	syncCamp.dwRoleID = pRemoteRole->GetID();
			//	syncCamp.eCamp = pRemoteRole->GetCampType();
			//	pRole->SendMessage(&syncCamp, syncCamp.dwSize);
			//}
		}



		//ͬ��������Ʒ���ͻ���
		TMap<INT64, tagGroundItem*>& mapGroundItem = pVisTile[n]->mapGroundItem;
		mapGroundItem.ResetIterator();
		tagGroundItem* pGroundItem = NULL;

		while( mapGroundItem.PeekNext(pGroundItem) )
		{
			if(IsShieldingItem(pGroundItem->dwTypeID))
				continue;
			send.n64Serial	= pGroundItem->n64ID;
			send.dwTypeID	= pGroundItem->dwTypeID;
			send.nNum		= pGroundItem->nNum;

			send.dwPutDownUnitID = pGroundItem->dwPutDownUnitID;
			send.dwOwnerID = pGroundItem->dwOwnerID;
			send.dwTeamID  = pGroundItem->dwTeamID;
			send.dwGroupID = pGroundItem->dwGroupID;
			send.vPos = pGroundItem->vPos;
			send.bKilled = FALSE;
			send.nEquipQlty	= EIQ_Null;
			if(MIsEquipment(send.dwTypeID))
			{
				tagEquip * pEquip = (tagEquip*)pGroundItem->pItem;
				if( P_VALID(pEquip) && pEquip->equipSpec.byQuality > EIQ_Start && pEquip->equipSpec.byQuality < EIQ_End )
				{
					send.nEquipQlty	= pEquip->equipSpec.byQuality;
				}
			}
			pRole->SendMessage(&send, send.dwSize);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ������λͬ�������ӵ�ש�ڵĿ��Կ����������
//--------------------------------------------------------------------------------------------
VOID Map::SyncInvisUnit2BigVisTileRole(Unit* pUnit, tagVisTile *pVisTile[ED_End], 
										const LPVOID pMsg, const DWORD dwSize)
{
	ASSERT(pUnit->IsInStateInvisible());
	if( g_world.IsShutingdown() )
		return;
	
	tagNS_RemoveRemote sendRemove;
	sendRemove.dwRoleID[0] = pUnit->GetID();
	
	// ����PK״̬
	if (!pUnit->IsRole())
	{
		return;
	}

	Role *pRoleSrc  =static_cast<Role *>(pUnit);
	if( !pRoleSrc->IsInRoleState(ERS_PK) && !pRoleSrc->IsInRoleState(ERS_PKEX) )
	{
		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;

			// �ҵ�ÿ����ש����
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			TMap<DWORD, Role*>::TMapIterator itRole = mapRole.Begin();
			//mapRole.ResetIterator();
			Role* pRole = NULL;

			while( mapRole.PeekNext(itRole,pRole) )
			{
				if( !P_VALID(pRole) )
					continue;

				// 1.�Ƿ���ȫ�ɼ�
				if( pRole->CanSeeTargetEntirely(pUnit) )
				{
					pRole->SendMessage(pMsg, dwSize);
					continue;
				}

				// 2.B�Ƿ���̽������
				if( !pRole->HasDectiveSkill() )
					continue;

				// 3.��B�Ŀ����б���
				if( pRole->IsInVisList(pUnit->GetID()) )
				{
					// ����B���ӷ�Χ��
					if( !pRole->IsInVisDist(pUnit) )
					{
						pRole->SendMessage(&sendRemove, sendRemove.dwSize);
						pRole->RemoveFromVisList(pUnit->GetID());
					}
					else
					{
						pRole->SendMessage(pMsg, dwSize);
					}

					continue;
				}
				
				// 4.����B�Ŀ����б���
				// 4.1����B���ӷ�Χ��
				if( !pRole->IsInVisDist(pUnit) )
					continue;

				// 4.2 ��B���ӷ�Χ��
				pRole->SendMessage(pMsg, dwSize);

				pRole->Add2VisList(pUnit->GetID());
			}
		}
	}
	else
	{
		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;

			// �ҵ�ÿ����ש����
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			TMap<DWORD, Role*>::TMapIterator itRole = mapRole.Begin();
			//mapRole.ResetIterator();
			Role* pRole = NULL;

			while( mapRole.PeekNext(itRole,pRole) )
			{
				if( !P_VALID(pRole) )
					continue;

				// 1.�Ƿ���ȫ�ɼ�
				if( pRole->CanSeeTargetEntirely(pUnit) )
				{
					pRole->SendMessage(pMsg, dwSize);
					continue;
				}

				// 2.B�Ƿ���̽������
				if( !pRole->HasDectiveSkill() )
					continue;

				// 3.��B�Ŀ����б���
				if( pRole->IsInVisList(pUnit->GetID()) )
				{
					// ����B���ӷ�Χ��
					if( !pRole->IsInVisDistForOpenMacre(pUnit) )
					{
						pRole->SendMessage(&sendRemove, sendRemove.dwSize);
						pRole->RemoveFromVisList(pUnit->GetID());
					}
					else
					{
						pRole->SendMessage(pMsg, dwSize);
					}

					continue;
				}

				// 4.����B�Ŀ����б���
				// 4.1����B���ӷ�Χ��
				if( !pRole->IsInVisDistForOpenMacre(pUnit) )
					continue;

				// 4.2 ��B���ӷ�Χ��
				pRole->SendMessage(pMsg, dwSize);

				pRole->Add2VisList(pUnit->GetID());
			}
		}
	}
		
	
}

//--------------------------------------------------------------------------------------------
// ͬ�����ӵ�ש�ڵ��ƶ�
//--------------------------------------------------------------------------------------------
VOID Map::SyncMovementToBigVisTile(Unit* pSelf)
{
	if( !P_VALID(pSelf) ) return;
	if( g_world.IsShutingdown() )
		return;
	if( IsShieldingNPC(pSelf) )
		return;
	BYTE	byMsg[1024] = {0};
	DWORD	dwSize = CalMovementMsg(pSelf, byMsg, 1024);

	if( dwSize == 0 ) return;

	tagVisTile* pVisTile[ED_End] = {0};

	// �����Χ�Ź���Ŀ��ӵ�ש
	GetVisTile(pSelf->GetVisTileIndex(), pVisTile);
	
	DWORD dwCurState = pSelf->GetState();

	// ����ӵ�ש���˷���ͬ������
	if( !pSelf->IsInStateInvisible() )	// A�ɼ�
	{
		SyncVisUnit2BigVisTileRole(pSelf, pVisTile, byMsg, dwSize);
	}
	else	// A���ɼ�
	{
		SyncInvisUnit2BigVisTileRole(pSelf, pVisTile, byMsg, dwSize);
	}

	// �����ӵ�ש������λͬ������ǰ���
	if( pSelf->IsRole() )
		SyncBigVisTileInvisUnit2Role(static_cast<Role*>(pSelf), pVisTile);
}

//-------------------------------------------------------------------------------------------------------------
// �ı��ͼ���ӵ�שʱͬ��
//-------------------------------------------------------------------------------------------------------------
VOID Map::SyncChangeVisTile(Unit* pUnit, INT nOldVisIndex, INT nNewVisIndex)
{
	if( g_world.IsShutingdown() )
		return;
	// ���˰������ȷת������������
	// ����Ҵӵ�ǰ�����Ƴ�
	RemoveFromVisTile(pUnit);

	// ȡ����Ϊ�л���ש��ɾ������ӵĵ�ש()
	tagVisTile* pVisTileAdd[ED_End] = {0};
	tagVisTile* pVisTileDec[ED_End] = {0};
	GetVisTileAddAndDec(nOldVisIndex, nNewVisIndex, pVisTileAdd, pVisTileDec);

	// �������ɾ������Һ���������Ƴ�(���Ҫɾ������ҽ��в���a)
	SyncRemovedUnits(pUnit, pVisTileDec);

	// ��ͬ�����뵽��Ұ�е���Һ�����(���Ҫ���ӵ���ҽ��в���a)
	SyncAddedUnits(pUnit, pVisTileAdd);

	// ��������ӵ���ǰ����
	AddToVisTile(pUnit, nNewVisIndex);
}

//-------------------------------------------------------------------------------------------------------------
// ֪ͨ����Զ�����ɾ���ö��󣬲������Self����ҵĻ�������ЩԶ����Ҵ��Լ����ؿͻ���ɾ��
//-------------------------------------------------------------------------------------------------------------
VOID Map::SyncRemovedUnits(Unit* pSelf, tagVisTile* pVisTileDec[ED_End])
{
	ASSERT( P_VALID(pSelf) );
	if( g_world.IsShutingdown() )
		return;

	if( IsShieldingNPC(pSelf) )
		return ;
	
	BOOL bSelfInvisible = pSelf->IsInStateInvisible();

	tagNS_RemoveRemote send;
	send.dwRoleID[0] = pSelf->GetID();

	// ͬ�����Ź������������
	if( pSelf->IsInStateInvisible() )
	{
		SyncInvisUnit2BigVisTileRole(pSelf, pVisTileDec, &send, send.dwSize);
	}
	else
	{
		SyncVisUnit2BigVisTileRole(pSelf, pVisTileDec, &send, send.dwSize);
	}

	// ���Ź�������Ϣͬ���������
	if( pSelf->IsRole() )
	{
		Role *pRole = static_cast<Role*>(pSelf);

		SyncRemoveBigVisTileUnitAndGroundItem2Role(pRole, pVisTileDec);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ��ɫ�쳣�Ź���󣬽�������Ϣͬ������ɫ
//-------------------------------------------------------------------------------------------------------------
VOID Map::SyncRemoveBigVisTileUnitAndGroundItem2Role(Role* pRole, tagVisTile *pVisTileDec[ED_End])
{
	if( g_world.IsShutingdown() )
		return;
	TList<DWORD> listRemovedUnit;
	TList<INT64> listGroundItem;
	
	// �����Ź����б�
	for(INT n = 0;  n < ED_End; n++)
	{
		if( !P_VALID(pVisTileDec[n]) )
			continue;

		// �ÿ��ӵ�שҪ��ɾ����������ÿ��ӵ�ש�е������б�
		TMap<DWORD, Role*>& mapRole = pVisTileDec[n]->mapRole;
		//mapRole.ResetIterator();
		TMap<DWORD, Role*>::TMapIterator itRole = mapRole.Begin();
		Role* pRemoteRole = NULL;

		while( mapRole.PeekNext(itRole,pRemoteRole) )
		{
			// ��Ҫɾ������Ҽ��뵽�б���
			if( !pRemoteRole->IsInStateInvisible() )
				listRemovedUnit.PushBack(pRemoteRole->GetID());
			else if( pRole->IsInVisList(pRemoteRole->GetID()) )
			{
				listRemovedUnit.PushBack(pRemoteRole->GetID());
				pRole->RemoveFromVisList(pRemoteRole->GetID());
			}
		}

		// �����ÿ��ӵ�ש�еķ�����б�
		TMap<DWORD, Creature*>& mapCreature = pVisTileDec[n]->mapCreature;
		//mapCreature.ResetIterator();
		TMap<DWORD, Creature*>::TMapIterator itCreature = mapCreature.Begin();
		Creature* pCreature = NULL;

		while( mapCreature.PeekNext(itCreature,pCreature) )
		{
			// ��Ҫɾ���ķ���Ҽ��뵽�б���
			if( !pCreature->IsInStateInvisible() )
				listRemovedUnit.PushBack(pCreature->GetID());
			else if( pRole->IsInVisList(pCreature->GetID()) )
			{
				listRemovedUnit.PushBack(pCreature->GetID());
				pRole->RemoveFromVisList(pCreature->GetID());
			}
		}

		//�����ÿ��ӵ�ש�е�GroundItem�б�
		TMap<INT64, tagGroundItem*>& mapGroundItem = pVisTileDec[n]->mapGroundItem;
		//mapGroundItem.ResetIterator();
		TMap<INT64, tagGroundItem*> ::TMapIterator itGround = mapGroundItem.Begin();
		tagGroundItem* pGroundItem = NULL;
		while (mapGroundItem.PeekNext(itGround,pGroundItem))
		{
			//��Ҫͬ����GroundItem�����б�
			if( pGroundItem->IsValid())
			{
				listGroundItem.PushBack(pGroundItem->n64ID);
			}
		}
	}

	// ���list��Ϊ�գ���self����ҵĻ������͸������Ҫɾ����Զ�����
	INT nListSize = listRemovedUnit.Size();

	if( nListSize > 0 )
	{
		DWORD dwSize = sizeof(tagNS_RemoveRemote) + (nListSize-1)*sizeof(DWORD);
		MCREATE_MSG(pSend, dwSize, NS_RemoveRemote);

		for(INT n = 0; n < nListSize; n++)
		{
			pSend->dwRoleID[n] = listRemovedUnit.PopFront();
		}

		pRole->GetSession()->SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	//���list��Ϊ��,��self����ҵĻ�����һ�ν�GroundItem���͸������Ҫɾ����GroundItem
	INT nSize = listGroundItem.Size();
	if (nSize > 0)
	{
		DWORD dwSize = sizeof(tagNS_RoleGroundItemDisappear) + (nSize - 1)*sizeof(INT64);
		MCREATE_MSG(pSend, dwSize, NS_RoleGroundItemDisappear);

		for (INT n = 0; n < nSize; n++)
		{
			pSend->n64Serial[n] = listGroundItem.PopFront();
		}

		pRole->GetSession()->SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ֪ͨ����Զ�����ɾ���ö��󣬲������Self����ҵĻ�������ЩԶ����Ҵ��Լ����ؿͻ���ɾ��
//-------------------------------------------------------------------------------------------------------------
VOID Map::SyncRemoveUnitToBigVisTile(Unit* pSelf)
{
	if( g_world.IsShutingdown() )
		return;

	if( IsShieldingNPC(pSelf) )
		return;

	INT nVisIndex = WorldPosToVisIndex(pSelf->GetMoveData().m_vPos);

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	SyncRemovedUnits(pSelf, pVisTile);
}

//-------------------------------------------------------------------------------------------------------
// ͬ�����뵽��ש�е���ң�������
//-------------------------------------------------------------------------------------------------------
VOID Map::SyncAddedUnits(Unit* pSelf, tagVisTile* pVisTileAdd[ED_End])
{
	ASSERT( P_VALID(pSelf) );
	if( g_world.IsShutingdown() )
		return;

	BOOL bSelfInvisible = pSelf->IsInStateInvisible();

	BYTE byMsg[1024] = {0};
	DWORD dwSize = CalMovementMsg(pSelf, byMsg, 1024);
	if(dwSize == 0)
		return;

	// ��Aͬ�������������
	if( IsShieldingNPC(pSelf) )
		return ;
		
	if( !pSelf->IsInStateInvisible() )	// A�ɼ�
	{
		SyncVisUnit2BigVisTileRole(pSelf, pVisTileAdd, byMsg, dwSize);
	}
	else	// A���ɼ�
	{
		if(pSelf->IsRole())
			SyncInvisUnit2BigVisTileRole(pSelf, pVisTileAdd, byMsg, dwSize);
		else
			ILOG->Write(_T("\nThis is monster in lurk state,it's error. monster state: %d.\n"), pSelf->m_StateMgr.GetStateFlags());			
	}

	// ��AΪ��ң��򽫸�������Ϣͬ������
	if( pSelf->IsRole() )
	{
		Role *pRole = static_cast<Role*>(pSelf);

		// �����ڷ�����λ������
		SyncBigVisTileVisUnitAndGroundItem2Role(pRole, pVisTileAdd);

		// ����������λ
		SyncBigVisTileInvisUnit2Role(pRole, pVisTileAdd);
	}
}

//----------------------------------------------------------------------------
// ������������Ƿ���ͬһ��vistile��
//----------------------------------------------------------------------------
BOOL Map::InSameBigVisTile(Unit* pSelf, Unit* pRemote)
{
	if( !P_VALID(pSelf) || !P_VALID(pRemote) )
	{
		return FALSE;
	}

	INT nSelfVisIndex = pSelf->GetVisTileIndex();
	INT nRemoteVisIndex = pRemote->GetVisTileIndex();

	if( GT_INVALID == nSelfVisIndex || GT_INVALID == nRemoteVisIndex )
	{
		return FALSE;
	}

	INT nMinus = abs(nSelfVisIndex - nRemoteVisIndex);

	if( ( nMinus >= m_nVisTileArrayWidth - 1 && nMinus <= m_nVisTileArrayWidth + 1 )
		|| 0 == nMinus
		|| 1 == nMinus
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
// ͨ������ĵ�ǰ�������㷢�͸��ͻ��˵�ͬ����Ϣ
//----------------------------------------------------------------------------
DWORD Map::CalMovementMsg(Unit* pSelf, LPBYTE pMsg, DWORD dwSize)
{
	ASSERT( P_VALID(pSelf) && P_VALID(pMsg) );

	// �������ﵱǰ�ƶ�״̬����ͬ������
	EMoveState eCurMove = pSelf->GetMoveState();
	DWORD dwRealSize = 0;

	switch (eCurMove)
	{
		// վ��
	case EMS_Stand:
		{
			tagNS_SyncStand* pSend = (tagNS_SyncStand*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncStand");
			pSend->dwSize = sizeof(tagNS_SyncStand);
			pSend->dwRoleID = pSelf->GetID();
			pSend->curPos = pSelf->GetCurPos();
			pSend->faceTo = pSelf->GetFaceTo();
			pSend->bSwim = false;

			dwRealSize = pSend->dwSize;
		}
		break;

		// ����
	case EMS_Walk:
		{
			ASSERT( pSelf->IsRole() );

			tagNS_SyncWalk* pSend = (tagNS_SyncWalk*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncWalk");
			pSend->dwSize = sizeof(tagNS_SyncWalk);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dstPos = pSelf->GetDestPos();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetXZSpeed();
			pSend->bCollide = true;
			pSend->bSwim = false;

			dwRealSize = pSend->dwSize;
		}
		break;

		// ��Ծ
	case EMS_Jump:
		{
			ASSERT( pSelf->IsRole() );

			tagNS_SyncJump* pSend = (tagNS_SyncJump*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncJump");
			pSend->dwSize = sizeof(tagNS_SyncJump);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dir = pSelf->GetMoveDir();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetXZSpeed();
			pSend->fYSpeed = pSelf->GetYSpeed();

			dwRealSize = pSend->dwSize;
		}
		break;

		// ����
	case EMS_Drop:
		{
			ASSERT( pSelf->IsRole() );

			tagNS_SyncDrop* pSend = (tagNS_SyncDrop*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncDrop");
			pSend->dwSize = sizeof(tagNS_SyncDrop);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dir = pSelf->GetMoveDir();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetDropXZSpeed();

			dwRealSize = pSend->dwSize;
		}
		break;

		// ��ֱ����
	case EMS_VDrop:
		{
			ASSERT( pSelf->IsRole() );

			tagNS_SyncVDrop* pSend = (tagNS_SyncVDrop*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncVDrop");
			pSend->dwSize = sizeof(tagNS_SyncVDrop);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->curTime = pSelf->GetMovePassTime();

			dwRealSize = pSend->dwSize;
		}
		break;

		// ����
	case EMS_Slide:
		{
			ASSERT( pSelf->IsRole() );

			tagNS_SyncSlide* pSend = (tagNS_SyncSlide*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncSlide");
			pSend->dwSize = sizeof(tagNS_SyncSlide);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetSlideSpeed();

			dwRealSize = pSend->dwSize;
		}
		break;

		// ��Ӿ
	case EMS_Swim:
		{
			ASSERT( pSelf->IsRole() );
			Role* pRole = static_cast<Role*>(pSelf);

			tagNS_SyncWalk* pSend = (tagNS_SyncWalk*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncWalk");
			pSend->dwSize = sizeof(tagNS_SyncWalk);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dstPos = pSelf->GetDestPos();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = ( pRole->IsInRoleState(ERS_WaterWalk) ? pSelf->GetXZSpeed() : pSelf->GetSwimXZSpeed() );
			pSend->bCollide = true;
			pSend->bSwim = true;

			dwRealSize = pSend->dwSize;
		}
		break;

		// Ư��
	case EMS_SwimStand:
		{
			ASSERT( pSelf->IsRole() );

			tagNS_SyncStand* pSend = (tagNS_SyncStand*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncStand");
			pSend->dwSize = sizeof(tagNS_SyncStand);
			pSend->dwRoleID = pSelf->GetID();
			pSend->curPos = pSelf->GetCurPos();
			pSend->faceTo = pSelf->GetFaceTo();
			pSend->bSwim = true;

			dwRealSize = pSend->dwSize;
		}
		break;

		// ����Ѳ��
	case EMS_CreaturePatrol:
		{
			ASSERT( pSelf->IsCreature() );

			tagNS_SyncWalk* pSend = (tagNS_SyncWalk*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncWalk");
			pSend->dwSize = sizeof(tagNS_SyncWalk);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dstPos = pSelf->GetDestPos();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetXZSpeed() * 0.4f;	// Ѳ���ٶ�
			pSend->bCollide = static_cast<Creature*>(pSelf)->NeedCollide();

			dwRealSize = pSend->dwSize;
		}
		break;

		// ��������
	case EMS_CreatureWalk:
		{
			ASSERT( pSelf->IsCreature() );

			tagNS_SyncWalk* pSend = (tagNS_SyncWalk*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncWalk");
			pSend->dwSize = sizeof(tagNS_SyncWalk);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dstPos = pSelf->GetDestPos();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetXZSpeed();
			pSend->bCollide = static_cast<Creature*>(pSelf)->NeedCollide();

			dwRealSize = pSend->dwSize;
		}
		break;

		// ��������
	case EMS_CreatureFlee:
		{
			ASSERT( pSelf->IsCreature() );

			tagNS_SyncWalk* pSend = (tagNS_SyncWalk*)pMsg;
			pSend->dwID = m_pUtil->Crc32("NS_SyncWalk");
			pSend->dwSize = sizeof(tagNS_SyncWalk);
			pSend->dwRoleID = pSelf->GetID();
			pSend->srcPos = pSelf->GetStartPos();
			pSend->dstPos = pSelf->GetDestPos();
			pSend->curTime = pSelf->GetMovePassTime();
			pSend->fXZSpeed = pSelf->GetXZSpeed() * 0.7;
			pSend->bCollide = static_cast<Creature*>(pSelf)->NeedCollide();

			dwRealSize = pSend->dwSize;
		}

	default:
		break;
	}

	if( dwRealSize > dwSize )
		dwRealSize = 0;

	return dwRealSize;
}

//------------------------------------------------------------------------------
// ����������λ�ö�Ӧ������
//------------------------------------------------------------------------------
DWORD Map::CheckArea(Role* pRole)
{
	if( !P_VALID(pRole) ) return 0;
	if( !P_VALID(m_pInfo) ) return 0;

	DWORD dwRet = 0;
#if 0
	// �õ���ҵİ����к͸�������
	AABBox roleBox = pRole->GetAABBox();
	INT nTileX = INT(pRole->GetCurPos().x / (FLOAT)TILE_SCALE);
	INT nTileZ = INT(pRole->GetCurPos().z / (FLOAT)TILE_SCALE);

	tagMapAreaInfo* pArea = NULL;

	// ���ȼ�ⰲȫ��
	pArea = IsInArea(m_pInfo->mapSafeArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) dwRet |= ERS_SafeArea;

	// �ټ��PVP��
	pArea = IsInArea(m_pInfo->mapPVPArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) dwRet |= ERS_PVPArea;

	// �ټ���̯��
	pArea = IsInArea(m_pInfo->mapStallArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) dwRet |= ERS_StallArea;

	// �ټ�������s
	pArea = IsInArea(m_pInfo->mapPrisonArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) dwRet |= ERS_PrisonArea;

	// ���ű���
	pArea = IsInArea(m_pInfo->mapScriptArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) )
	{
			pRole->SetRoleState(ERS_ScriptArea);
			OnEnterArea(pRole, pArea);	

			// ��¼һ�µ�ǰ�Ľű�����
			//m_dwCurScriptArea = pArea->dwObjID;
			pRole->SetCurScriptArea(pArea->dwObjID);

	}
	else
	{
		if (pRole->IsInRoleState(ERS_ScriptArea))
		{
			pRole->UnsetRoleState(ERS_ScriptArea);
			OnLeaveArea(pRole, pRole->GetCurScriptArea());	
			//m_dwCurScriptArea = 0;
			pRole->SetCurScriptArea(0);
		}
	}
	
	/*pArea = IsInArea(m_pInfo->mapScriptArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) OnEnterArea(pRole, pArea);*/

	// �����˰��
	pArea = IsInArea(m_pInfo->mapTaxArea, roleBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) dwRet |= ERS_TaxArea;
#else
	tagMapAreaInfo * pArea = NULL;
	dwRet = CheckAreaEx(pRole->GetAABBox(),pRole->GetCurPos(),ERS_ScriptArea,&pArea);
	if( dwRet & ERS_ScriptArea )
	{
		pRole->SetRoleState(ERS_ScriptArea);
		OnEnterArea(pRole, pArea);	

		// ��¼һ�µ�ǰ�Ľű�����
		//m_dwCurScriptArea = pArea->dwObjID;
		pRole->SetCurScriptArea(pArea->dwObjID);
	}
	else
	{
		INT nPKDefault = SAFE_GUARD_FORCE_LEVEL;
		if( g_world.IsPkConstraintsOpen() )
		{
			if ( pRole->IsUsingPKConstraintsSafeGuradMinLevel())
			{
				nPKDefault = g_world.GetPKMiniLevel();
				ResetRoleSafeGuardState(nPKDefault, pRole, dwRet);

			}
		}
		else
		{
			ResetRoleSafeGuardState(nPKDefault, pRole, dwRet);
		}
		if (pRole->IsInRoleState(ERS_ScriptArea))
		{
			pRole->UnsetRoleState(ERS_ScriptArea);
			OnLeaveArea(pRole, pRole->GetCurScriptArea());	
			//m_dwCurScriptArea = 0;
			pRole->SetCurScriptArea(0);
		}
	}
#endif
	// �ټ��
	return dwRet;
}

DWORD	Map::CheckAreaEx(const AABBox & box,const Vector3 & pos, int areaReturn,tagMapAreaInfo ** ppArea)
{
	if( !P_VALID(m_pInfo) ) return 0;

	DWORD dwRet = 0;

	// �õ���ҵİ����к͸�������
	AABBox temBox = box;
	INT nTileX = INT(pos.x / (FLOAT)TILE_SCALE);
	INT nTileZ = INT(pos.z / (FLOAT)TILE_SCALE);

	tagMapAreaInfo* pArea = NULL;

	// ���ȼ�ⰲȫ��
	pArea = IsInArea(m_pInfo->mapSafeArea, temBox, nTileX, nTileZ);
	if( P_VALID(pArea) )
	{
		if( areaReturn == ERS_SafeArea && P_VALID(ppArea) )
			*ppArea = pArea;
		dwRet |= ERS_SafeArea;
	}

	// �ټ��PVP��
	pArea = IsInArea(m_pInfo->mapPVPArea, temBox, nTileX, nTileZ);
	if( P_VALID(pArea) )
	{
		if( areaReturn == ERS_PVPArea && P_VALID(ppArea) )
			*ppArea = pArea;
		dwRet |= ERS_PVPArea;
	}

	// �ټ���̯��
	pArea = IsInArea(m_pInfo->mapStallArea, temBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) 
	{
		if( areaReturn == ERS_StallArea && P_VALID(ppArea) )
			*ppArea = pArea;
		dwRet |= ERS_StallArea;
	}

	// �ټ�������s
	pArea = IsInArea(m_pInfo->mapPrisonArea, temBox, nTileX, nTileZ);
	if( P_VALID(pArea) )
	{
		if( areaReturn == ERS_PrisonArea && P_VALID(ppArea) )
			*ppArea = pArea;
		dwRet |= ERS_PrisonArea;
	}

	// ���ű���
	pArea = IsInArea(m_pInfo->mapScriptArea, temBox, nTileX, nTileZ);
	if( P_VALID(pArea) )
	{
		if( areaReturn == ERS_ScriptArea && P_VALID(ppArea) )
			*ppArea = pArea;
		dwRet |= ERS_ScriptArea;
	}

	// �����˰��
	pArea = IsInArea(m_pInfo->mapTaxArea, temBox, nTileX, nTileZ);
	if( P_VALID(pArea) ) 
	{
		if( areaReturn == ERS_TaxArea && P_VALID(ppArea) )
			*ppArea = pArea;
		dwRet |= ERS_TaxArea;
	}

	return dwRet;
}

//------------------------------------------------------------------------------
// �������ǲ�����ĳ����������Χ��
//------------------------------------------------------------------------------
BOOL Map::IsInTrigger(Role* pRole, DWORD dwMapTriggerID)
{
	if( !P_VALID(pRole) ) return FALSE;
	if( !P_VALID(m_pInfo) ) return FALSE;

	tagMapTriggerInfo* pTrigger = m_pInfo->mapTrigger.Peek(dwMapTriggerID);
	if( !P_VALID(pTrigger) ) return FALSE;

	// �õ���ҵİ����к͸�������
	AABBox roleBox = pRole->GetAABBox();
	INT nTileX = INT(pRole->GetCurPos().x / (FLOAT)TILE_SCALE);
	INT nTileZ = INT(pRole->GetCurPos().z / (FLOAT)TILE_SCALE);

	if( !pTrigger->box.Intersects(roleBox) ) return FALSE;

	if( !PtInRegion(pTrigger->hRgn, nTileX, nTileZ) ) return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
// �õ�ĳ��trigger��Ӧ���������к�
//------------------------------------------------------------------------------
DWORD Map::GetTriggerQuestSerial(DWORD dwMapTriggerID)
{
	if( !P_VALID(m_pInfo) ) return GT_INVALID;

	tagMapTriggerInfo* pTrigger = m_pInfo->mapTrigger.Peek(dwMapTriggerID);
	if( !P_VALID(pTrigger) ) return GT_INVALID;

	if( pTrigger->eType != EMT_Script ) return GT_INVALID;

	return pTrigger->dwQuestSerial;
}

//------------------------------------------------------------------------------
// ���ĳ�������к͸��������Ƿ���ĳ�������б��е�һ��������
//------------------------------------------------------------------------------
tagMapAreaInfo* Map::IsInArea(tagMapInfo::MapAreaMap& mapArea, AABBox& roleBox, INT nTileX, INT nTileZ)
{
	tagMapInfo::MapAreaMap::TMapIterator it = mapArea.Begin();
	tagMapAreaInfo* pArea = NULL;

	while( mapArea.PeekNext(it, pArea) )
	{
		// �������ж����ཻ
		if( !pArea->box.Intersects(roleBox) ) continue;

		// �������ཻ��������������ڲ�����������
		if( PtInRegion(pArea->hRgn, nTileX, nTileZ) )
			return pArea;
	}

	return NULL;
}

//------------------------------------------------------------------------------
// �Ƿ���ս��
//------------------------------------------------------------------------------
BOOL Map::IsWar()
{
	return (ENMT_Battle == m_pInfo->eNoarmalMapType);
}

//------------------------------------------------------------------------------
// �������ӵ��ǳ��б��У����ϲ���������ҵ�ʵ�ʵǳ��ͱ���
//------------------------------------------------------------------------------
VOID Map::RoleLogOut(DWORD dwID)
{
	Role* pRole = m_mapRole.Peek(dwID);
	if( P_VALID(pRole) )
	{
		
		RoleLeaveMap(pRole->GetID(), FALSE, TRUE);
	}
}

//-------------------------------------------------------------------------------
// ����뿪��ͼ��ֻ���������߳��������
//-------------------------------------------------------------------------------
VOID Map::RoleLeaveMap( DWORD dwID, BOOL bChgMap, BOOL bLogout)
{
	Role* pRole = m_mapRole.Peek(dwID);
	if( P_VALID(pRole) )
	{
		//�����뿪��ͼǰ���������
		pRole->OnLeaveMap(bChgMap);

		// ���ýű�
		if( P_VALID(m_pScript))
		{
			m_pScript->OnPlayerLeave(pRole, this, bLogout);
		}

		// ͬ����Χ���ɾ���Լ�
		SyncRoleLeaveMap(pRole);

		// ����Ҵ��Լ����б���ɾ��
		m_mapRole.Erase(dwID);
		pRole->SetMap(NULL);

		// ������session��Ч�������ھʹ��б���ɾ��
		if( pRole->GetSession() )
		{
			m_mapSession.Erase(pRole->GetSession()->GetSessionID());
		}

		if( GetMapType() != EMT_Normal )
		{
			// �������ɣ����⸱��buff
			if( g_VipNetBarMgr.IsOpen() )
				g_VipNetBarMgr.RemoveInstBuff(pRole);
		}
	}
}

//-------------------------------------------------------------------------------
// �ڵ�ͼ�и���ĳ��ID������һ���������ã�һ��Ҫ�ڸõ�ͼ�߳�֮��ʹ�ã�
//-------------------------------------------------------------------------------
Unit* Map::FindUnit(DWORD dwID)
{
	if( IS_PLAYER(dwID) )
		return m_mapRole.Peek(dwID);
	else if( IS_CREATURE(dwID) )
		return m_mapCreature.Peek(dwID);
	else if( IS_PET(dwID) )
		return m_mapCreature.Peek(dwID);
	else
		return NULL;
}

//-------------------------------------------------------------------------------
// ��������ʼ��NPC�������̵�
//-------------------------------------------------------------------------------
BOOL Map::AddShop(DWORD dwNPCID, DWORD dwShopID)
{
	if(m_mapShop.IsExist(dwNPCID))
	{
		return TRUE;
	}

	Shop* pShop = Shop::Create(dwNPCID, dwShopID);
	if(!P_VALID(pShop))
	{
		ASSERT(P_VALID(pShop));
		IMSG(_T("Create shop<npc_typid:%u, shop_id:%u> failed!\n"), dwNPCID, dwShopID);
		return FALSE;
	}

	m_mapShop.Add(dwNPCID, pShop);

	return TRUE;
}

//-------------------------------------------------------------------------------
// ��������ʼ��NPC�������̻�
//-------------------------------------------------------------------------------
BOOL Map::AddCofC(DWORD dwNPCID, DWORD dwCofCID)
{
	if(m_mapCofC.IsExist(dwNPCID))
	{
		return TRUE;
	}

	GuildCofC* pCofC = GuildCofC::Create(dwNPCID, dwCofCID);
	if(!P_VALID(pCofC))
	{
		ASSERT(P_VALID(pCofC));
		IMSG(_T("Create CofC<npc_typid:%u, CofC_id:%u> failed!\n"), dwNPCID, dwCofCID);
		return FALSE;
	}

	m_mapCofC.Add(dwNPCID, pCofC);

	return TRUE;
}

//-------------------------------------------------------------------------------
// ����뿪��ͼͬ��
//-------------------------------------------------------------------------------
VOID Map::SyncRoleLeaveMap(Role* pSelf)
{
	if( g_world.IsShutingdown() )
		return;
	if( !P_VALID(pSelf) ) return;

	INT nVisTileIndex = pSelf->GetVisTileIndex();

	// ���ȴ�VisTile���Ƴ�
	RemoveFromVisTile(pSelf);

	// �õ��Ź���������
	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisTileIndex, pVisTile);

	// �����Ƴ�ͬ������
	SyncRemovedUnits(pSelf, pVisTile);
}

//----------------------------------------------------------------------------------
// ���ͽ����µ�ͼ���ͻ���
//----------------------------------------------------------------------------------
VOID Map::SendGotoNewMapToPlayer(Role* pSelf)
{
	if( g_world.IsShutingdown() )
		return;
	if( !P_VALID(pSelf) || !P_VALID(pSelf->GetSession()) )
		return;

	tagNS_GotoNewMap send;
	send.dwMapID = pSelf->GetMapID();
	send.pos = pSelf->GetCurPos();
	send.faceTo = pSelf->GetFaceTo();

	pSelf->GetSession()->SendMessage(&send, send.dwSize);
}

//----------------------------------------------------------------------------------
// ���������Ʒ�Ƿ񵽵��� --- ������Ʒϵͳ 
//----------------------------------------------------------------------------------
BOOL Map::CanPutDown(Creature* pCreature, INT nIndex, Vector3& vPos)
{
	if( !P_VALID(pCreature) )
		return FALSE;

	// ���ݹ�������Ʒ����
	const Vector3& vCreaturePos = pCreature->GetCurPos();

	// �õ��������
	INT nIndexX = INT(vCreaturePos.x / TILE_SCALE);
	INT nIndexZ = INT(vCreaturePos.z / TILE_SCALE);

	// �ҳ�һ������������
	if( FALSE == GetCanGoPosFromIndex(nIndex, nIndexX, nIndexZ, 2 * (nIndex / 8 + 1), vPos) )
	{
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------
// ���������Ʒ������ --- ������Ʒϵͳ 
//----------------------------------------------------------------------------------
INT64 Map::PutDownItem(Creature* pCreature, tagItem* pLootItem, DWORD dwOwnerID, DWORD dwTeamID, Vector3& vPos)
{
	if( !P_VALID(pCreature) || !P_VALID(pLootItem) )
		return 0;

	// ȡ����ͷ���߶�
	vPos.y = pCreature->GetCurPosTop().y;

	// ���ɵ�����Ʒ
	tagGroundItem* pGroundItem = new tagGroundItem(GenGroundItemID(), pLootItem->dwTypeID, 
														pLootItem->GetNum(), pLootItem, vPos,
														dwOwnerID, dwTeamID, 0, pCreature->GetID());
	ASSERT(pGroundItem->IsValid());

	// �ڵ����������Ʒ
	AddGroundItem(pGroundItem);

	return pGroundItem->n64ID;
}

//----------------------------------------------------------------------------------
// ��������Ǯ������ --- ������Ʒϵͳ 
//----------------------------------------------------------------------------------
VOID Map::PutDownMoney(Creature* pCreature, INT nMoney, Role* pRole, Vector3 vPos)
{
	if( !P_VALID(pCreature) || !P_VALID(pRole) )
		return;

	DWORD dwTeamID = pRole->GetTeamID();
	DWORD dwOwnerID = GT_VALID(dwTeamID) ? GT_INVALID : pRole->GetID();

	// ȡ����ͷ���߶�
	vPos.y = pCreature->GetCurPosTop().y;

	// ���ɵ�����Ʒ
	tagGroundItem* pGroundItem = new tagGroundItem(GenGroundItemID(), TYPE_ID_MONEY, nMoney, NULL,
													vPos, dwOwnerID, dwTeamID, 0, pCreature->GetID());
	ASSERT(pGroundItem->IsValid());

	// ��������������Ʒ
	AddGroundItem(pGroundItem);

	const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
	if(P_VALID(pScript))
	{
		pScript->DropSilver( pCreature, pRole->GetID(), pCreature->GetTypeID(), nMoney );
	}
}

BOOL Map::GetCanGoPosFromIndex(INT32 nIndex, INT32 nIndexX, INT32 nIndexZ, INT32 n, Vector3& vPos)
{
	INT index = nIndex % 8;
	switch (index)
	{
	case 0:
		nIndexX -= n;
		nIndexZ -= n;
		break;
	case 1:
		nIndexZ -= n;
		break;
	case 2:
		nIndexX += n;
		nIndexZ -= n;
		break;
	case 3:
		nIndexX += n;
		break;
	case 4:
		nIndexX += n;
		nIndexZ += n;
		break;
	case 5:
		nIndexZ += n;
		break;
	case 6:
		nIndexX -= n;
		nIndexZ += n;
		break;
	case 7:
		nIndexX -= n;
		break;
	}

	vPos.x = (FLOAT)(nIndexX * TILE_SCALE);
	vPos.z = (FLOAT)(nIndexZ * TILE_SCALE);

	if( nIndexX < 0 || nIndexX > m_pInfo->nWidth - 1 ||
		nIndexZ < 0 || nIndexZ > m_pInfo->nHeight - 1 )
	{
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��������������Ʒ --- ������Ʒϵͳ
//-----------------------------------------------------------------------------
VOID Map::AddGroundItem(tagGroundItem* pGroundItem)
{
	if( !P_VALID(pGroundItem) ) return;
	if( !pGroundItem->IsValid() ) return;

	// ����һ������
	AABBox box;
	FLOAT fHalfX = vGroundItemSize.x / 2.0f;
	FLOAT fHalfZ = vGroundItemSize.z / 2.0f;
	box.min.x = pGroundItem->vPos.x - fHalfX;
	box.min.z = pGroundItem->vPos.z - fHalfZ;
	box.min.y = pGroundItem->vPos.y;
	box.max.x = pGroundItem->vPos.x + fHalfX;
	box.max.z = pGroundItem->vPos.z + fHalfZ;
	box.max.y = pGroundItem->vPos.y + vGroundItemSize.y;

	pGroundItem->vPos.y = GetGroundAndModelHeight(box);

	// �����ˮ��Ļ�������䵽ˮ��
	FLOAT fY = 0.0f;
	if( GetNavCollider()->IfWillOnWater(pGroundItem->vPos, fY) )
	{
		pGroundItem->vPos.y = fY;
	}

	// ���뵽m_mapGroundItem��
	m_mapGroundItem.Add(pGroundItem->n64ID, pGroundItem);

	// ������Ʒ���ڵĿ��ӵ�ש����
	INT nVisIndex = WorldPosToVisIndex(pGroundItem->vPos);

	// �õ��Ź���
	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	// ͬ�������뵽�ͻ��˵���Һ�����
	SyncAddGroundItem(pGroundItem, pVisTile);

	// �������䵽9����֮��
	AddGroundItemToVisTile(pGroundItem, nVisIndex);

}
//-----------------------------------------------------------------------------
// �ӵ������Ƴ���Ʒ --- ������Ʒϵͳ
//-----------------------------------------------------------------------------
VOID Map::RemoveGroundItem(tagGroundItem *pGroundItem)
{
	if(!P_VALID(pGroundItem))
		return;

	if (!pGroundItem->IsValid())
		return;

	//��m_mapGroundItem ���Ƴ�
	m_mapGroundItem.Erase(pGroundItem->n64ID);
	//������Ʒ���ڵĿ��ӵ�ש����
	INT nVisIndex = WorldPosToVisIndex(pGroundItem->vPos);

	// �õ��Ź���
	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	// ͬ�������뵽�ͻ��˵���Һ�����
	SyncRemoveGroundItem(pGroundItem, pVisTile);

	// �������䵽9����֮��
	RemoveGroundItemFromVisTile(pGroundItem, nVisIndex);

}

VOID Map::RemoveGroundItem(INT64 n64Serial)
{
	RemoveGroundItem(m_mapGroundItem.Peek(n64Serial));
}

//-----------------------------------------------------------------------------
// ͬ�����뵽��ש�е���Ʒ --- ������Ʒϵͳ
//-----------------------------------------------------------------------------
VOID Map::SyncAddGroundItem(tagGroundItem *pGroundItem, tagVisTile *pVisTileAdd[ED_End])
{
	if( g_world.IsShutingdown() )
		return;
	if( IsShieldingItem(pGroundItem->dwTypeID) )
		return;
	tagNS_SyncGroundItem send;

	send.n64Serial	= pGroundItem->n64ID;
	send.dwTypeID	= pGroundItem->dwTypeID;
	send.nNum		= pGroundItem->nNum;

	send.dwPutDownUnitID = pGroundItem->dwPutDownUnitID;
	send.dwOwnerID = pGroundItem->dwOwnerID;
	send.dwTeamID = pGroundItem->dwTeamID;
	send.dwGroupID = pGroundItem->dwGroupID;
	send.vPos = pGroundItem->vPos;
	send.bKilled = TRUE;
	send.nEquipQlty	= EIQ_Null;
	if(MIsEquipment(send.dwTypeID))
	{
		tagEquip * pEquip = (tagEquip*)pGroundItem->pItem;
		if( P_VALID(pEquip) && pEquip->equipSpec.byQuality > EIQ_Start && pEquip->equipSpec.byQuality < EIQ_End )
		{
			send.nEquipQlty	= pEquip->equipSpec.byQuality;
		}
	}
	
	// ����ӵ�ש���˷���ͬ������
	SendBigVisTileMsg(pVisTileAdd, &send, send.dwSize);
}

//-----------------------------------------------------------------------------
// ͬ�����뵽��ש�е���Ʒ --- ������Ʒϵͳ
//-----------------------------------------------------------------------------
VOID Map::SyncGroundItemState(tagGroundItem *pGroundItem, tagVisTile *pVisTileAdd[ED_End])
{
	if( g_world.IsShutingdown() )
		return;

	if( IsShieldingItem(pGroundItem->dwTypeID) )
		return ;
		
	tagNS_SyncGroundItem send;

	send.n64Serial	= pGroundItem->n64ID;
	send.dwTypeID	= pGroundItem->dwTypeID;
	send.nNum		= pGroundItem->nNum;

	send.dwPutDownUnitID = pGroundItem->dwPutDownUnitID;
	send.dwOwnerID = pGroundItem->dwOwnerID;
	send.dwTeamID = pGroundItem->dwTeamID;
	send.dwGroupID = pGroundItem->dwGroupID;
	send.vPos = pGroundItem->vPos;
	send.bKilled = FALSE;
	send.nEquipQlty	= EIQ_Null;
	if(MIsEquipment(send.dwTypeID))
	{
		tagEquip * pEquip = (tagEquip*)pGroundItem->pItem;
		if( P_VALID(pEquip) && pEquip->equipSpec.byQuality > EIQ_Start && pEquip->equipSpec.byQuality < EIQ_End )
		{
			send.nEquipQlty	= pEquip->equipSpec.byQuality;
		}
	}

	// ����ӵ�ש���˷���ͬ������
	SendBigVisTileMsg(pVisTileAdd, &send, send.dwSize);
}


//-----------------------------------------------------------------------------
// ͬ���Ƴ���ש�е���Ʒ --- ������Ʒϵͳ
//-----------------------------------------------------------------------------
VOID Map::SyncRemoveGroundItem(tagGroundItem *pGroundItem, tagVisTile *pVisTileAdd[ED_End])
{
	if( g_world.IsShutingdown() )
		return;
	if(!pGroundItem->IsValid())
		return;

	if( IsShieldingItem(pGroundItem->dwTypeID) )
		return;

	//������Ʒ��ʧ��Ϣ
	tagNS_RoleGroundItemDisappear disappear;
	disappear.n64Serial[0] = pGroundItem->n64ID;

	// ����ӵ�ש���˷���ͬ������
	SendBigVisTileMsg(pVisTileAdd, &disappear, disappear.dwSize);
}

//-----------------------------------------------------------------------------
// ���͵�ͼ������Ч��Ϣ	---	��ҽ��볡������
//-----------------------------------------------------------------------------
VOID Map::SendSceneEffect(Role* pSelf)
{
	if( g_world.IsShutingdown() )
		return;
	if (!P_VALID(pSelf))	return;

	// ��ÿͻ��˷��������Ѿ�����ĳ�����Ч
	m_Mutex.Acquire();
	DWORD dwObjID = GT_INVALID;
	TList<DWORD>::TListIterator iter = m_listSceneEffect.Begin();
	while (m_listSceneEffect.PeekNext(iter, dwObjID))
	{
		if (!GT_VALID(dwObjID))
			continue;

		tagNS_PlaySceneEffect send;
		send.eType		= ESET_ByObjID;
		send.dwObjID	= dwObjID;
		pSelf->SendMessage(&send, send.dwSize);
	}
	m_Mutex.Release();
}

//-----------------------------------------------------------------------------
// ����������Ч	--- �ű�����
//-----------------------------------------------------------------------------
VOID Map::PlaySceneEffect(ESceneEffectType eType, DWORD dwObjID, Vector3 vPos, DWORD dwEffectID)
{
	if( g_world.IsShutingdown() )
		return;
	// ��鳡����Ч����
	if (!P_VALID(m_pInfo))	return;
	if (!P_VALID(m_pInfo->mapTriggerEffect.Peek(dwObjID)))
		return;

	// ������Ч����
	m_Mutex.Acquire();
	if (!m_listSceneEffect.IsExist(dwObjID))
		m_listSceneEffect.PushBack(dwObjID);
	m_Mutex.Release();

	// ֪ͨ��ͼ����ҿ���������Ч(����ͻ��˼�����)
	tagNS_PlaySceneEffect send;
	send.eType		= eType;
	send.dwObjID	= dwObjID;
	send.vPos		= vPos;
	send.dwEffectID	= dwEffectID;
	SendMapMessage(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// �رճ�����Ч	--- �ű�����
//-----------------------------------------------------------------------------
VOID Map::StopSceneEffect(DWORD dwObjID)
{
	if( g_world.IsShutingdown() )
		return;
	// ����Ƿ��Ѿ�����
	m_Mutex.Acquire();
	if (!m_listSceneEffect.IsExist(dwObjID))
	{
		m_Mutex.Release();
		return;
	}
	else
	{
		m_listSceneEffect.Erase(dwObjID);
	}
	m_Mutex.Release();

	// ֪ͨ��ͼ���������
	tagNS_StopSceneEffect send;
	send.dwObjID	= dwObjID;
	SendMapMessage(&send, send.dwSize);
}

//-----------------------------------------------------------------------------------------
// ������Ʒ����ͼ�е�ĳ��VisTile --- ������Ʒϵͳ
//-----------------------------------------------------------------------------------------
VOID Map::AddGroundItemToVisTile(tagGroundItem* pGroundItem, INT nVisIndex)
{
	m_pVisTile[nVisIndex].mapGroundItem.Add(pGroundItem->n64ID, pGroundItem);
}

//-----------------------------------------------------------------------------------------
// �Ƴ���ͼ��ĳ��VisTile����Ʒ --- ������Ʒϵͳ
//-----------------------------------------------------------------------------------------
VOID Map::RemoveGroundItemFromVisTile(tagGroundItem* pGroundItem, INT nVisIndex)
{
	m_pVisTile[nVisIndex].mapGroundItem.Erase(pGroundItem->n64ID);
}

//-----------------------------------------------------------------------------------------
// ˢ�µ�ˢ��
//-----------------------------------------------------------------------------------------
VOID Map::SSpawnPtReplaceCreature( Creature* pDeadCreature )
{
	ASSERT_P_VALID(pDeadCreature);

	// ��Ҫ�õ�������
	DWORD	dwReuseID		= pDeadCreature->GetID();
	DWORD	dwSSpawnPtID	= pDeadCreature->GetSpawnPtID();
	Vector3	vPos			= pDeadCreature->GetBornPos();
	Vector3 vFase			= pDeadCreature->GetBornFace();
	BOOL	bCollide		= pDeadCreature->NeedCollide();

	// �Ҹ��µ�
	const tagSSpawnPointProto* pSSpawnProto = g_attRes.GetSSpawnPointProto(dwSSpawnPtID);
	ASSERT_P_VALID( pSSpawnProto );
	if (!P_VALID(pSSpawnProto))
		return ;

	INT nCandiNum	= 0;
	while (GT_VALID(pSSpawnProto->dwTypeIDs[nCandiNum]))
		nCandiNum++;
	INT nIndex = IUTIL->Rand() % nCandiNum;
	DWORD dwNewTypeID = pSSpawnProto->dwTypeIDs[nIndex];
	ASSERT( GT_VALID(dwNewTypeID) );
	if (!P_VALID(dwNewTypeID))
		return ;
	const tagCreatureProto* pCreatureProto = g_attRes.GetCreatureProto(dwNewTypeID);


	// ɾ���ɵ�
	m_mapRespawnCreature.Erase(pDeadCreature->GetID());
	m_mapDeadDoor.Erase(pDeadCreature->GetID());
	// ��©����ֹû�дӸ���ɾ���ֵ�����
	RemoveCreatureFromMapEx(pDeadCreature);
	Creature::Delete(pDeadCreature);

	// �����µ�
	pDeadCreature = Creature::Create(dwReuseID, GetMapID(), pCreatureProto, vPos, vFase, ECST_SpawnPoint, dwSSpawnPtID, bCollide, NULL);

	// ���
	AddCreature(pDeadCreature);
}

//-----------------------------------------------------------------------------------------
// ��̬��������
//-----------------------------------------------------------------------------------------
Creature* Map::CreateCreature(DWORD dwTypeID, Vector3& vPos, Vector3& vFaceTo, DWORD dwSpawnerID, BOOL bCollide, CHAR* pPatrolListName)
{
	Creature *pCreature = (Creature*)GT_INVALID;

	const tagCreatureProto* pProto = g_attRes.GetCreatureProto(dwTypeID);
	if( !P_VALID(pProto) )
	{
		IMSG(_T("Detect a unknown creature in map, dwTypeID=%u\r\n"), dwTypeID);
		return pCreature;
	}

	if (MoveData::IsPosInvalid(vPos) || MoveData::IsFaceInvalid(vFaceTo))
	{
		return pCreature;
	}

	if( !IsPosValid(vPos) )
	{
		IMSG(_T("creature is out of map, dwTypeID=%u\r\n"), dwTypeID);
		return pCreature;
	}

	tagMapWayPointInfoList* patrolList = NULL;
	if(P_VALID(pPatrolListName))
		patrolList = m_pInfo->mapWayPointList.Peek(m_pUtil->Crc32(pPatrolListName));

	DWORD dwID = m_CreatureIDGen.GetNewCreatureID();

	pCreature = Creature::Create(dwID, m_dwID, pProto, vPos, vFaceTo, ECST_Dynamic, dwSpawnerID, bCollide, patrolList,GT_INVALID,GT_INVALID,this);

	if(P_VALID(pCreature))
		AddCreature(pCreature);

	return pCreature;
}

//-----------------------------------------------------------------------------------------
// ɾ������
//-----------------------------------------------------------------------------------------
VOID Map::DeleteCreature(DWORD dwID)
{
	Creature* pCreature = FindCreature(dwID);
	if (!P_VALID(pCreature))	return;
	m_bCreatureChanged = TRUE;
	// �������б��������������
	m_mapCreature.Erase(dwID);

	if(pCreature->IsActive())
		m_mapActiveCreature.Erase(pCreature->GetID());

	if(pCreature->IsDoor())
		m_mapDoor.Erase(pCreature->GetID());

	// �ӿ��ӵ�ש�����ߣ�����ͬ���ͻ���
	RemoveFromVisTile(pCreature);

	// ͬ�����ͻ���
	SyncRemoveUnitToBigVisTile(pCreature);

	//m_mapDeadDoor.Erase(dwID);
	//Creature::Delete(pCreature);
	// ���뵽�����б��еȴ�����
	pCreature->SetRemoveFlag();
	m_mapRespawnCreature.Add(pCreature->GetID(), pCreature);
}

VOID Map::SetCreaturePatrolList(Creature* pCreature, DWORD dwPatrolID)
{
	if (!P_VALID(pCreature))
	{
		return;
	}

	tagMapWayPointInfoList* patrolList = NULL;
	patrolList = m_pInfo->mapWayPointList.Peek(dwPatrolID);

	if (!P_VALID(patrolList))
	{
		return;
	}

	pCreature->SetPatrol(patrolList);
}

//------------------------------------------------------------------------------------------
// Ǳ��
//------------------------------------------------------------------------------------------
VOID Map::Lurk(Unit *pUnit)
{
	ASSERT(P_VALID(pUnit));
	if( g_world.IsShutingdown() )
		return;

	INT nVisIndex = pUnit->GetVisTileIndex();
	ASSERT(GT_VALID(nVisIndex));

	// �����������λ�б�
	m_pVisTile[nVisIndex].mapInvisibleUnit.Add(pUnit->GetID(), pUnit);

	// ͬ���������б������
	tagNS_RemoveRemote sendRemove;
	sendRemove.dwRoleID[0] = pUnit->GetID();

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);
	
	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;

		// �ҵ�ÿ����ש����
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		//mapRole.ResetIterator();
		TMap<DWORD, Role*>::TMapIterator itRole = mapRole.Begin();
		Role* pRole = NULL;

		while( mapRole.PeekNext(itRole,pRole) )
		{
			if( !P_VALID(pRole) )
				continue;

			// 1.�Ƿ���ȫ�ɼ�
			if( pRole->CanSeeTargetEntirely(pUnit) )
			{
				continue;
			}

			ASSERT(!pRole->IsInVisList(pUnit->GetID()));

			// 2.B�Ƿ���̽�����������Ƿ��ڿ��ӷ�Χ��
			if( !pRole->HasDectiveSkill() || !pRole->IsInVisDist(pUnit) )
			{
				pRole->SendMessage(&sendRemove, sendRemove.dwSize);
				continue;
			}

			// 3 ��B���ӷ�Χ��
			pRole->Add2VisList(pUnit->GetID());
		}
	}
}

//------------------------------------------------------------------------------------------
// Ǳ��״̬��ʧ -- ����
//------------------------------------------------------------------------------------------
VOID Map::UnLurk(Unit *pUnit)
{
	ASSERT(P_VALID(pUnit));
	if( g_world.IsShutingdown() )
		return;

	INT nVisIndex = pUnit->GetVisTileIndex();
	ASSERT(GT_VALID(nVisIndex));

	// �Ӹ�������λ�б���ȥ��
	m_pVisTile[nVisIndex].mapInvisibleUnit.Erase(pUnit->GetID());

	// ͬ���������б������
	BYTE	byMsg[1024] = {0};
	DWORD	dwSize = CalMovementMsg(pUnit, byMsg, 1024);

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;

		// �ҵ�ÿ����ש����
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator itRole = mapRole.Begin();
		//mapRole.ResetIterator();
		Role* pRole = NULL;

		while( mapRole.PeekNext(itRole,pRole) )
		{
			if( !P_VALID(pRole) )
				continue;

			// 1.�Ƿ���ȫ�ɼ�
			if( pRole->CanSeeTargetEntirely(pUnit) )
			{
				continue;
			}

			// 2.B�Ƿ���̽�����������Ƿ��ڿ��ӷ�Χ��
			if( !pRole->HasDectiveSkill() || !pRole->IsInVisDist(pUnit) )
			{
				pRole->SendMessage(byMsg, dwSize);
				continue;
			}

			// 3 ��B���ӷ�Χ��
			pRole->RemoveFromVisList(pUnit->GetID());
		}
	}
}

//------------------------------------------------------------------------------------------
// Ǳ�е�λ�ƶ�������ͬ�������ӷ�Χ�����
//------------------------------------------------------------------------------------------
VOID Map::UpdateLurk2BigVisTileRole(Unit *pUnit)
{
	ASSERT(pUnit->IsInStateInvisible());
	if( g_world.IsShutingdown() )
		return;

	INT nVisIndex = pUnit->GetVisTileIndex();
	ASSERT(GT_VALID(nVisIndex));

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	tagNS_RemoveRemote sendRemove;
	sendRemove.dwRoleID[0] = pUnit->GetID();

	BYTE	byMsg[1024] = {0};
	DWORD	dwSize = CalMovementMsg(pUnit, byMsg, 1024);

	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;

		// �ҵ�ÿ����ש����
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		//mapRole.ResetIterator();
		TMap<DWORD, Role*>::TMapIterator irRole = mapRole.Begin();
		Role* pRole = NULL;

		while( mapRole.PeekNext(irRole,pRole) )
		{
			if( !P_VALID(pRole) )
				continue;

			// 1.�Ƿ���ȫ�ɼ�
			if( pRole->CanSeeTargetEntirely(pUnit) )
			{
				continue;
			}

			// 2.B�Ƿ���̽������
			if( !pRole->HasDectiveSkill() )
				continue;

			// 3.��B�Ŀ����б���
			if( pRole->IsInVisList(pUnit->GetID()) )
			{
				// ����B���ӷ�Χ��
				if( !pRole->IsInVisDist(pUnit) )
				{
					pRole->SendMessage(&sendRemove, sendRemove.dwSize);
					pRole->RemoveFromVisList(pUnit->GetID());
				}

				continue;
			}

			// 4.����B�Ŀ����б���
			// 4.1����B���ӷ�Χ��
			if( !pRole->IsInVisDist(pUnit) )
				continue;

			// 4.2 ��B���ӷ�Χ��
			pRole->SendMessage(byMsg, dwSize);
			pRole->Add2VisList(pUnit->GetID());
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
// ����ΧǱ�е�λͬ���������
//---------------------------------------------------------------------------------------------------------------------
VOID Map::UpdateBigVisTileLurkUnit2Role(Role* pRole)
{
	if( g_world.IsShutingdown() )
		return;
	INT nVisIndex = pRole->GetVisTileIndex();
	ASSERT(GT_VALID(nVisIndex));

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nVisIndex, pVisTile);

	SyncBigVisTileInvisUnit2Role(pRole, pVisTile);
}

FLOAT Map::GetExpRate()
{
	return g_GMPolicy.GetExpRate();
}

FLOAT Map::GetLootRate()
{
	return g_GMPolicy.GetLootRate(GetMapType() == EMT_Normal);
}

Pet* Map::FindPet( DWORD dwID ) /* ?���ڵ�ͼ�߳�֮��ʹ�� */
{
	Creature* pCreature = m_mapCreature.Peek(dwID); 
	if (P_VALID(pCreature))
	{
		return dynamic_cast<Pet*>(pCreature);
	}
	else
	{
		return NULL;
	}
}


//---------------------------------------------------------------------------------------------------------------------
// ����뿪ĳ����
//---------------------------------------------------------------------------------------------------------------------
VOID Map::OnLeaveArea(Role* pRole, DWORD dwCurScriptArea)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnLeaveArea(pRole, dwCurScriptArea, this);
	}
}

//---------------------------------------------------------------------------------------------------------------------
// ������-����һ���µľ�������ͼ
//---------------------------------------------------------------------------------------------------------------------
VOID Map::OnCreatePvPMap(RoomBase * pRoomInfo)
{
	if( g_world.IsShutingdown() )
		return;
	if (P_VALID(m_pScript) && P_VALID(pRoomInfo))
	{
		if (pRoomInfo->GetRoomType() == E_PvP_Room3v3)
		{
			Athletics3V3Room* p3v3Room = (Athletics3V3Room*)pRoomInfo;
			DWORD nPlayerRoleID[P3v3ROOM_TOTAL_PERSON_NUM] = {0};

			for (int nCnt = 0; nCnt < P3v3ROOM_TOTAL_PERSON_NUM; nCnt++)
			{
				//GetPosRoleInfo�Ĳ�����1��ʼ������nCntҪ��1
				tagPvPRoleInfo& pPvPRoleInfo = p3v3Room->GetPosRoleInfo(nCnt+1);
				nPlayerRoleID[nCnt] = pPvPRoleInfo.dwRoleID;

				if ( P_VALID(pPvPRoleInfo.dwRoleID) )
				{
					Role* pRole = g_roleMgr.GetRolePtrByID(pPvPRoleInfo.dwRoleID);
					tagNS_PvPEnterPvP msg;
					if (P_VALID( pRole))
					{
						pRole->SendMessage(&msg,msg.dwSize);
					}
				}
			}

			m_pScript->OnCreatePvPMap3v3(GetMapID(),
				GetInstanceID(),
				p3v3Room->GetRoomSerialID(),
				nPlayerRoleID,
				P3v3ROOM_TOTAL_PERSON_NUM);
		}
		else
		{
			 ASSERT(0);
			 return ;
		}
	}
}
INT	Map::CanEnter(Role *pRole)
{
	if( P_VALID(pRole) )
	{
		const tagMapInfo* pMapInfo = GetMapInfo();
		if( P_VALID(pMapInfo) )
		{
			if( pMapInfo->bSoaringLimit )
			{
				if( ESV_SoaringUp == pRole->GetAttValue(ERA_Soar) )
				{
					return E_Success;
				}
				else
					return E_SoaringLimitted;
			}
		}
		else
			return E_SystemError;
	}
	return E_Success;
}

VOID Map::ClearIdeCreature()
{
	Creature* pCreature = NULL;
	TMap<DWORD, Creature*>::TMapIterator itCreature = m_mapCreature.Begin();
	while( m_mapCreature.PeekNext(itCreature, pCreature) )
	{
		if(pCreature == NULL)
			m_mapCreature.Erase(itCreature);
	}
}
VOID Map::UpdateIdleCreature()
{
	if( g_world.IsShutingdown() )
		return;
	//if( m_bCreatureChanged )
	{
		ClearIdeCreature();	//�Ȳ鿴map���Ƿ��п�ָ��
		m_mapCurCreature = m_mapCreature;
		m_bCreatureChanged = FALSE;
	}

	CreatureMap::TMapIterator it_active_creature = m_mapActiveCreature.Begin();
	Creature* pCreature = NULL;

	while( m_mapActiveCreature.PeekNext(it_active_creature, pCreature) )
	{
		if(!pCreature->HasRemoveFlag()) pCreature->Update();
	}

	// ���µ�ͼ�������д�ˢ�µĹ���
	CreatureMap::TMapIterator itDeadCreature = m_mapRespawnCreature.Begin();
	Creature* pDeadCreature = NULL;

	while( m_mapRespawnCreature.PeekNext(itDeadCreature, pDeadCreature) )
	{
		ECreatureReviveResult eRet = pDeadCreature->TryRevive();

		if( ECRR_Success == eRet )
		{
			// �����ɹ�
			m_mapRespawnCreature.Erase(pDeadCreature->GetID());
			m_mapDeadDoor.Erase(pDeadCreature->GetID());
			AddCreature(pDeadCreature);
		}
		else if( ECRR_Failed == eRet )
		{
			// ����ʧ��
		}
		else if( ECRR_NeedDestroy == eRet )
		{
			// ��̬���ɵģ���Ҫɾ����
			m_mapRespawnCreature.Erase(pDeadCreature->GetID());
			m_mapDeadDoor.Erase(pDeadCreature->GetID());
			// ��©����ֹû�дӸ���ɾ���ֵ�����
			RemoveCreatureFromMapEx(pDeadCreature);
			Creature::Delete(pDeadCreature);
		}
		else if( ECRR_NeedReplace == eRet )
		{
			// ˢ�ֵ����ɵģ���Ҫ�滻
			SSpawnPtReplaceCreature(pDeadCreature);
		}
		else
		{
			ASSERT(0);
		}
	}

	// ���µ�����Ʒ
	TMap<INT64, tagGroundItem*>::TMapIterator it = m_mapGroundItem.Begin();
	tagGroundItem* pGroundItem = NULL;

	while( m_mapGroundItem.PeekNext(it, pGroundItem) )
	{
		switch(pGroundItem->Update())
		{
			// �Ƴ�����
		case EGIU_Remove:
			{
				// �Ƴ�����
				RemoveGroundItem(pGroundItem);

				// ������Ʒ
				pGroundItem->DestroyItem();

				// �ͷ��ڴ�
				SAFE_DEL(pGroundItem);
			}
			break;
			// ͬ������״̬
		case EGIU_Sync:
			break;
		case EGIU_Null:
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}

// û����ҵ�ʱ�򣬱���Ҫ���ĸ��²���
VOID Map::IdleUpdata()
{
	if( g_world.IsShutingdown() )
		return;
	// 1���������������Ĺ���͵���
	UpdateIdleCreature();
	// 2�������̵�
	UpdateAllShops();
	// 3�������̻�
	UpdateAllCofC();
	// 4����Ҫ����ˢ�»��
	if( timeGetTime() - m_dwLastMinute >= 60*1000 )
	{ 
		m_dwLastMinute = timeGetTime();
		OnMinute();
	}
	// ��ͼˢ�� [4/20/2012 zhangzhihua]
	GetMonsterGen().Update();
}

MapFairyContract * Map::MakeFairyContract(DWORD dwFairyID)
{
	if( !g_world.IsFairyContractOpen() )
		return NULL;

	if( !P_VALID(m_pFairyContractMgr) )
	{
		m_pFairyContractMgr = new MapFairyContract(GetMapID());
		m_pFairyContractMgr->MapInstanceID( GetInstanceID() );
	}
	if( P_VALID(m_pFairyContractMgr) )
	{
		m_pFairyContractMgr->AddContract(dwFairyID);
	}
	return m_pFairyContractMgr;
}
MapFairyContract * Map::GetFairyContract() const
{
	if( !g_world.IsFairyContractOpen() )
		return NULL;
	return m_pFairyContractMgr;
}


VOID	Map::SynOneVisTileMsgToAllRole(Unit * pUnit,CONST LPVOID pMsg,DWORD dwSize)
{
	if( g_world.IsShutingdown() )
		return;
	if( !P_VALID(pUnit) )
		return;

	if( IsShieldingNPC(pUnit) )
		return;

	INT nNdx = pUnit->GetVisTileIndex();
	if( nNdx < 0 || nNdx >= m_nVisTileArrayWidth * m_nVisTileArrayHeight )
		return;
	TMap<DWORD, Role*>& mapRole = m_pVisTile[nNdx].mapRole;
	TMap<DWORD, Role*>::TMapIterator it;
	it = mapRole.Begin();
	Role* pRole = NULL;

	while( mapRole.PeekNext(it,pRole) )
	{
		if( !P_VALID(pRole) )
			continue;

		pRole->SendMessage(pMsg, dwSize);
	}
}

VOID	Map::GetVisTileTeams(INT nVisTileNdx,std::set<DWORD>& teams )
{
	INT nNdx = nVisTileNdx;
	if( nNdx < 0 || nNdx >= m_nVisTileArrayWidth * m_nVisTileArrayHeight )
		return;
	DWORD dwTeamID = GT_INVALID;

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nNdx, pVisTile);

	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;

		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator it;
		it = mapRole.Begin();
		Role* pRole = NULL;

		while( mapRole.PeekNext(it,pRole) )
		{
			if( !P_VALID(pRole) )
				continue;

			dwTeamID = pRole->GetTeamID();
			if( dwTeamID != (DWORD)GT_INVALID )
			{
				teams.insert(dwTeamID);
			}
		}
	}
}

VOID	Map::GetNoTeamPlayers( INT nVisTileNdx,std::set<DWORD>& plys )
{
	INT nNdx = nVisTileNdx;
	if( nNdx < 0 || nNdx >= m_nVisTileArrayWidth * m_nVisTileArrayHeight )
		return;
	//DWORD dwPlyID = GT_INVALID;

	tagVisTile* pVisTile[ED_End] = {0};
	GetVisTile(nNdx, pVisTile);

	for(INT n = 0; n < ED_End; n++)
	{
		if( NULL == pVisTile[n] )
			continue;

		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator it;
		it = mapRole.Begin();
		Role* pRole = NULL;

		while( mapRole.PeekNext(it,pRole) )
		{
			if( !P_VALID(pRole) )
				continue;

			if( pRole->GetTeamID() == (DWORD)GT_INVALID )
			{
				plys.insert(pRole->GetID());
			}
		}
	}
}

VOID	Map::RemoveOneUnitFromVisTile(Unit * pUnit)
{
	RemoveFromVisTile(pUnit);
}
BOOL Map::GetCanGoPos(const AABBox & boxTarget,FLOAT fYSpeed,Vector3& vPos,INT nTryTimes )
{
	if( IfCanGo(vPos.x,vPos.z) )
	{
		FLOAT fHeight = GetGroundHeight(vPos.x,vPos.z);
		if( fYSpeed + boxTarget.min.y >= fHeight)
			return TRUE;
	}

	if(nTryTimes <= 0)
		return FALSE;
		
	//���ѡ��һ���Ƕ�
	FLOAT fAngle	= (IUTIL->Rand() % 360) / 360.0f * 3.14f * 2;
	//���ѡ��һ������ķ�Χ
	FLOAT fDist		= (IUTIL->Rand() % INT(TILE_SCALE * 2)) ;
	//����Ŀ���
	vPos.x +=	sin(fAngle) * fDist;
	vPos.z +=	cos(fAngle) * fDist;
	
	return GetCanGoPos(boxTarget,fYSpeed,vPos,nTryTimes - 1);
}

void Map::ResetRoleSafeGuardState( INT nPKDefault, Role* pRole, DWORD dwRet )
{
	if( nPKDefault < pRole->GetLevel() )
	{
		if( dwRet & ERS_Safeguard )
			pRole->SetRoleState(ERS_Safeguard);
		else
			pRole->UnsetRoleState(ERS_Safeguard);
	}
}
//----------------------------------------------------------------------------
//���½�ɫ�ļ���
VOID Map::UpdateRoleSkill(Role *pRole)
{
	if( !P_VALID(pRole) )
		return;

	pRole->UpdateSkillByMap( GetMapType(), GetMapID() );
}
//----------------------------------------------------------------------------





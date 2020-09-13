//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_mgr.cpp
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݹ����������ڸ���������������ɼ�ɾ�������洢�������ڵ���������
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "role_mgr.h"
#include "role.h"
#include "player_session.h"
#include "group_mgr.h"
#include "social_mgr.h"
#include "famehall.h"
#include "TradeYuanBao.h"
#include "vip_stall.h"
#include "mall.h"
#include "..\\ServerDefine\\role_data_define.h"
#include "..\\ServerDefine\\role_data.h"
#include "../ServerDefine/msg_log.h"

#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/msg_mall.h"
#include "../ServerDefine/msg_buff.h"

#include "player_regression.h"

// Jason 2010-2-21 v1.3.2 �������������
#include "guild_apply_to_join.h"
#include "gm_policy.h"
#include "convenient_team.h"
#include "../ServerDefine/family_define.h"
#include "family_join.h"
#include "family_mgr.h"
#include "../ServerDefine/msg_mall.h"

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID tagRoleMgr::Reset()
{
	nMaxUseIndex	= -1;
	nLastSaveIndex	= 0;
	nFreeHeadIndex	= 0;
	nFreeNodeNum	= MAX_PLAYER_NUM;
	
	for(INT i=0; i<MAX_PLAYER_NUM - 1; ++i)
	{
		sAllRole[i].nNext = i + 1;
		sAllRole[i].pRole = NULL;
	}

	// ���һ��
	sAllRole[MAX_PLAYER_NUM - 1].nNext = GT_INVALID;
	sAllRole[MAX_PLAYER_NUM - 1].pRole = NULL;
}

//------------------------------------------------------------------------------
// ��ȡ�豣��Ľ�ɫ
//------------------------------------------------------------------------------
Role* tagRoleMgr::GetRoleNeedSave()
{
	if(nMaxUseIndex < 0 || nMaxUseIndex >= MAX_PLAYER_NUM)
	{
		return NULL;
	}
	
	INT nIndexSave = nLastSaveIndex;
	while(true)
	{
		if(nIndexSave >= nMaxUseIndex)
		{
			nIndexSave = 0;
		}
		else
		{
			++nIndexSave;
		}

		if(sAllRole[nIndexSave].pRole != NULL)
		{
			nLastSaveIndex = nIndexSave;
			break;
		}

		if(nIndexSave == nLastSaveIndex)
		{
			// û���ҵ���Ҫ�洢�Ľ�ɫ
			nLastSaveIndex	= 0;
			nMaxUseIndex	= -1;
			
			// �������Ƿ�����©
			if(nFreeNodeNum != MAX_PLAYER_NUM)
			{
				IMSG(_T("\n\n\n\n\nCritical Error in tagRoleMgr!!!!!\n\n"));
			}

			return NULL;
		}
	}

	if(!sAllRole[nLastSaveIndex].pRole->IsNeedSave2DB())
	{
		return NULL;
	}

	return sAllRole[nLastSaveIndex].pRole;
}

//------------------------------------------------------------------------------
// constructor
//------------------------------------------------------------------------------
RoleMgr::RoleMgr():m_nRoleLevelLimit(ROLE_LEVEL_LIMIT)
{
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
RoleMgr::~RoleMgr()
{
	Destroy();
}

//--------------------------------------------------------------------------------
// ��ʼ��
//--------------------------------------------------------------------------------
BOOL RoleMgr::Init()
{
	m_pFastCode = "FastCode";
	m_vctRole.Reset();

	//��world.ini����������ߵȼ�
	TObjRef<VarContainer> pVar = "VarContainer";
	if(!P_VALID(pVar))
	{
		ERR(_T("Create World(VarContainer) obj failed!"));
		return FALSE;
	}
	m_nRoleLevelLimit = (INT32)pVar->GetInt(_T("role_level_limit"), _T("world"),ROLE_LEVEL_LIMIT);

	return TRUE;
}

//---------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------
VOID RoleMgr::Destroy()
{
	// ɾ�����е����߽�ɫ
	m_RoleMapMutex.Acquire();

	INT nIndex;
	Role* pRole = NULL;

	RoleMap::TMapIterator it = m_mapRole.Begin();
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.Remove(nIndex);
		if( P_VALID(pRole) )
		{
			Map * pMap = pRole->GetMap();
			if( P_VALID(pMap) )
				pMap->RemoveOneUnitFromVisTile(pRole);
		}
		else
			continue;

		Role::Delete(pRole);
	}
	m_mapRole.Clear();
	tagDWORDTime* pTempTime = NULL;
	RoleCreateTime::TMapIterator TempIt = m_mapRoleCreateTime.Begin();
	while ( m_mapRoleCreateTime.PeekNext(TempIt,pTempTime))
	{
		if ( pTempTime )
		{
			SAFE_DEL(pTempTime);
		}
	}
	m_mapRoleCreateTime.Clear();
   
	m_vctRole.Reset();

	m_RoleMapMutex.Release();

	// ɾ�����е����߽�ɫ
	EraseAllRoleInfo();
}

//-----------------------------------------------------------------------------------
// ����һ���µ����߽�ɫ
//-----------------------------------------------------------------------------------
BOOL RoleMgr::CreateRoleInfo(const tagRoleInfo* pInfo)
{
	if( pInfo->dwRoleID == 0 )
		return FALSE;
	m_RoleInfoMapMutex.Acquire();
	if (!m_mapRoleInfo.IsExist(pInfo->dwRoleID))
	{
		tagRoleInfo* pNew = new tagRoleInfo;
		if( !P_VALID(pNew) ) return FALSE;

		m_pFastCode->MemCpy(pNew, pInfo, sizeof(tagRoleInfo));
		Role * pTemRole = GetRolePtrByID(pInfo->dwRoleID);
		if(P_VALID(pTemRole))
			pNew->bOnline = true;
		else
			pNew->bOnline = false;
		m_mapRoleInfo.Add(pNew->dwRoleID, pNew);
		m_mapRoleNameCrcID.Add(pNew->dwRoleNameCrc, pNew->dwRoleID);
	}
	m_RoleInfoMapMutex.Release();

	return TRUE;
}

//------------------------------------------------------------------------------------
// ɾ��һ�����߽�ɫ����һ����ɾ�����ʱ����
//------------------------------------------------------------------------------------
BOOL RoleMgr::DeleteRoleInfo(const DWORD dwRoleID)
{
	m_RoleInfoMapMutex.Acquire();

	tagRoleInfo* pInfo = m_mapRoleInfo.Peek(dwRoleID);
	if( P_VALID(pInfo) )
	{
		DeletAllFriend(dwRoleID);
		g_VIPStall.RemoveRoleVIPStall(dwRoleID);	// VIP̯λ����
		DeleteYBOrder(dwRoleID);
		m_mapRoleInfo.Erase(dwRoleID);
		m_mapRoleNameCrcID.Erase(pInfo->dwRoleNameCrc);
		SAFE_DEL(pInfo);
	}

	m_RoleInfoMapMutex.Release();

	return TRUE;
}

//------------------------------------------------------------------------------------
// ĳ����ɫ��������Ҫ��m_mapRoleNameCrcID�н�����crcֵ�ı�
//------------------------------------------------------------------------------------
VOID RoleMgr::ChangeRoleName(DWORD dwRoleID, LPCTSTR szRoleName)
{
	RoleIDMap::TMapIterator it = m_mapRoleNameCrcID.Begin();
	DWORD dwTmpRoleID = 0;
	while ( it->second != dwRoleID )
	{
		it++;
	}

	m_mapRoleNameCrcID.Erase(it->first);

	TCHAR buf[X_SHORT_NAME] = {0};
	_tcsncpy(buf, szRoleName, X_SHORT_NAME);
	buf[X_SHORT_NAME-1] = _T('\0');
	_tcslwr(buf);

	DWORD dwNameCrc = IUTIL->Crc32(buf);
	m_mapRoleNameCrcID.Add(dwNameCrc, dwRoleID);

	tagRoleInfo* pTmpInfo = m_mapRoleInfo.Peek(dwRoleID);
	if (P_VALID(pTmpInfo))
	{
		pTmpInfo->dwRoleNameCrc = dwNameCrc;
	}
}
//------------------------------------------------------------------------------------
// ��� ���Ķ�Ӧ��loverid
//------------------------------------------------------------------------------------
VOID RoleMgr::BreakMarriage(DWORD dwRoleID1,DWORD dwRoleID2)
{
	tagRoleInfo* pTmpInfo = m_mapRoleInfo.Peek(dwRoleID1);
	if (P_VALID(pTmpInfo))
	{
		pTmpInfo->dwLoverID = GT_INVALID;
	}


	pTmpInfo = m_mapRoleInfo.Peek(dwRoleID2);
	if (P_VALID(pTmpInfo))
	{
		pTmpInfo->dwLoverID = GT_INVALID;
	}
}
//------------------------------------------------------------------------------------
// ��� ���Ķ�Ӧ��loverid
//------------------------------------------------------------------------------------
VOID RoleMgr::GetMarriage(DWORD dwRoleID1,DWORD dwRoleID2)
{
	tagRoleInfo* pTmpInfo = m_mapRoleInfo.Peek(dwRoleID1);
	if (P_VALID(pTmpInfo))
	{
		pTmpInfo->dwLoverID = dwRoleID2;
	}

	pTmpInfo = m_mapRoleInfo.Peek(dwRoleID2);
	if (P_VALID(pTmpInfo))
	{
		pTmpInfo->dwLoverID = dwRoleID1;
	}
}

//------------------------------------------------------------------------------------
// ����ʦͽ��ϵ
//------------------------------------------------------------------------------------
VOID RoleMgr::ModifyMasterApprenticeCount(DWORD dwRoleID, INT nCount)
{
	tagRoleInfo* pInfo = m_mapRoleInfo.Peek(dwRoleID);
	if (P_VALID(pInfo))
	{
		pInfo->nMARelation += nCount;
		if(pInfo->nMARelation < 0) pInfo->nMARelation = 0;
	}
}

//-------------------------------------------------------------------------------------
// ����һ��������ң�����LoongDB�����ɫ����ʱ����
//-------------------------------------------------------------------------------------
Role* RoleMgr::CreateRole(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession, BOOL& bFirst)
{
	if( 0 == dwRoleID )
		return NULL;
	Role* pOld = GetRolePtrByID(dwRoleID);
	if (P_VALID(pOld))
	{
		IMSG(_T("Role already loaded! roleid:%u, accountid:%u!\n"), dwRoleID, pSession->GetSessionID());
		ILOG->Write(_T("Role already loaded! roleid:%u, accountid:%u!\n"), dwRoleID, pSession->GetSessionID());
		return NULL;
	}

    tagDWORDTime now = GetCurrentDWORDTime();
	Role* pNew = Role::Create(dwRoleID, pData, pSession);
	if( !P_VALID(pNew) ) return NULL;

	tagRoleInfo * pTemInfo = GetRoleInfo(dwRoleID);
	if(!P_VALID(pTemInfo) && P_VALID(pSession))
	{
		tagNDBC_LoadOneRoleInfo temMsg;
		temMsg.dwAccountID	= pSession->GetSessionID();
		temMsg.dwRoleID		= dwRoleID;
		g_dbSession.Send(&temMsg,temMsg.dwSize);
	}

	m_RoleMapMutex.Acquire();

	INT nIndex = m_vctRole.Add(pNew);
	if(GT_INVALID == nIndex)
	{
		IMSG(_T("\n\n\n\n\nCritical Error in tagRoleMgr!!!!!\n\n"));
		ASSERT(0);
		Role::Delete(pNew);
		m_RoleMapMutex.Release();
		return NULL;
	}

	m_mapRole.Add(pNew->GetID(), nIndex);

	pNew->UpdateRoleFamilySprite();
	
    tagDWORDTime* pTeampTime = GetRoleCreateTime(pNew->GetID());
	if ( !P_VALID(pTeampTime))
	{
		tagDWORDTime* pNow = new tagDWORDTime;
		if( pSession->GetPreLoginTime().day != now.day )
		{
			if ( P_VALID(pNow))
			{
				(*pNow) = pSession->GetPreLoginTime();
				SetRoleCreateTime(pNow,pNew->GetID());
			}

		}
		else
		{
			if ( P_VALID(pNow))
			{
				(*pNow) = now;
				SetRoleCreateTime(pNow,pNew->GetID());
			}
		}

	}

	

	m_RoleMapMutex.Release();

	// �鿴�ǲ��ǵ�һ������
	if( GT_INVALID == pNew->GetOnlineTime() )
	{
		bFirst = TRUE;
	}

	PlayerOnline(pNew, bFirst);

	// Jason 2010-1-19 v1.3.2 ��һع�
	PlayerRegression * pPlayerRegression = TPlayerRegression::Instance  ();
	if( P_VALID(pPlayerRegression) )
		pPlayerRegression->IniRole  (pNew);

	g_GMPolicy.NotifyToPlayer(pNew);

	return pNew;
}

//--------------------------------------------------------------------------------------
// ɾ��һ��������ң���һ������������ʱɾ������һ���߾�ɾ�������ǵ����ݿⷵ���˲�ɾ����
//--------------------------------------------------------------------------------------
BOOL RoleMgr::DeleteRole(const DWORD dwRoleID)
{
	m_RoleMapMutex.Acquire();

	INT nIndex = m_mapRole.Peek(dwRoleID);
	Role* pRole = m_vctRole.GetRole(nIndex);
	if( P_VALID(pRole) )
	{
		ConvenientTeam& easy_team = ConvenientTeam::GetInstance();
		easy_team.DeleteCTInfoOfPlayer(pRole);
		PlayerOutLine(pRole);
		m_vctRole.Remove(nIndex);
		m_mapRole.Erase(dwRoleID);
		Role::Delete(pRole);
	}
	m_RoleMapMutex.Release();
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����һ����ҵ�DB
//--------------------------------------------------------------------------------------
VOID RoleMgr::SaveOneRoleToDB()
{
	if(m_mapRole.Empty())
	{
		return;
	}
	
	Role *pRole = m_vctRole.GetRoleNeedSave();
	if(pRole != NULL)
	{
		pRole->SaveToDB();
	}
}

//--------------------------------------------------------------------------------------
// ����������ҵ�DB��һ����ϵͳͣ��ʱ��
//--------------------------------------------------------------------------------------
VOID RoleMgr::SaveAllRolesToDB()
{
	INT nIndex = GT_INVALID;
	Role* pRole = NULL;
	RoleMap::TMapIterator it = m_mapRole.Begin();

	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			pRole->SaveToDB(TRUE);
		}
	}
}

//---------------------------------------------------------------------------------------
// �����������CRCֵ�õ����ID
//---------------------------------------------------------------------------------------
DWORD RoleMgr::GetRoleIDByNameCrc(const DWORD dwNameCrc)
{
	return m_mapRoleNameCrcID.Peek(dwNameCrc);
}

//---------------------------------------------------------------------------------------
// �������ID�õ��������
//---------------------------------------------------------------------------------------
VOID RoleMgr::GetRoleNameByID(const DWORD dwRoleID, LPTSTR szName)
{
	tagRoleInfo* pRoleInfo = m_mapRoleInfo.Peek(dwRoleID);
	if( P_VALID(pRoleInfo) )
	{
		_tcsncpy(szName, pRoleInfo->szRoleName, X_SHORT_NAME);
	}
	else
	{
		ZeroMemory(szName, X_SHORT_NAME * sizeof(TCHAR));
	}
}

//---------------------------------------------------------------------------------------
// �������ID�õ����ָ��
//---------------------------------------------------------------------------------------
Role* RoleMgr::GetRolePtrByID(const DWORD dwRoleID)
{
	if( 0 == dwRoleID )
		return NULL;
	return m_vctRole.GetRole(m_mapRole.Peek(dwRoleID));
}

//---------------------------------------------------------------------------------------
// �����ȡ�������bMustGetΪ�������ȡ��һ��
//---------------------------------------------------------------------------------------
Role* RoleMgr::GetRandRolePtr( BOOL bGetFirst /*= FALSE*/ )
{
	if (bGetFirst)
	{
		if (m_mapRole.Size() <= 0)
		{
			return NULL;
		}

		return m_vctRole.GetRole(m_mapRole.Begin()->second);
	}
	INT	nMaxUsedIndex = m_vctRole.GetMaxUsedIndex();

	if(nMaxUsedIndex == 0)
		return (Role*)GT_INVALID;

	INT nRandIndex = rand() % nMaxUsedIndex;

	return m_vctRole.GetRole(nRandIndex);
}

struct SendMsg
{
	SendMsg(PVOID pMsg, DWORD dwSize)
		:m_pMsg(pMsg), m_dwSize(dwSize){}
	VOID operator()(Unit* pUnit)
	{
		if (P_VALID(pUnit) && pUnit->IsRole())
		{
			Role* pRole = dynamic_cast<Role*>(pUnit);
			if (P_VALID(pRole))
			{
				pRole->SendMessage(m_pMsg, m_dwSize);
			}			
		}
	}
	PVOID m_pMsg;
	DWORD m_dwSize;
};

// ���ý�ɫ��ƣ��ֵ
struct ResetWeary
{
	ResetWeary() {}

	VOID operator()(Unit* pUnit)
	{
		if (P_VALID(pUnit) && pUnit->IsRole())
		{
			Role* pRole = dynamic_cast<Role*>(pUnit);
			if (P_VALID(pRole))
			{
				pRole->ResetWearyValue();
			}
		}
	}
};

//--------------------------------------------------------------------------------------------
// �����������е�ͼ�ڵ���ҷ���Ϣ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::SendWorldMsg(LPVOID pMsg, DWORD dwSize)
{
	ForEachRoleInWorld(SendMsg(pMsg, dwSize));
// 	RoleMap::TMapIterator it = m_mapRole.Begin();
// 	INT nIndex = -1;
// 	Role* pRole = NULL;
// 	while( m_mapRole.PeekNext(it, nIndex) )
// 	{
// 		pRole = m_vctRole.GetRole(nIndex);
// 		if( P_VALID(pRole) )
// 		{
// 			pRole->SendMessage(pMsg, dwSize);
// 		}
// 	}
}

// �����������߽�ɫ��ƣ��ֵ
VOID RoleMgr::ResetWearyOfEachRoleInWorld()
{
	ForEachRoleInWorld(ResetWeary());
}

//--------------------------------------------------------------------------------------------
// �õ���ҵļ�����Ϣ��������Ұ��������ߣ�
//--------------------------------------------------------------------------------------------
tagRoleInfo* RoleMgr::GetRoleInfo(const DWORD dwRoleID)
{
	if( 0 == dwRoleID )
		return NULL;
	tagRoleInfo* pRoleInfo = m_mapRoleInfo.Peek(dwRoleID);
	return pRoleInfo;	
}
void RoleMgr::SetRoleInfo(const tagRoleInfo * p)
{
	tagRoleInfo * pOld = m_mapRoleInfo.Peek(p->dwRoleID);
	if(P_VALID(pOld))
	{
		memcpy(pOld,p,sizeof(*pOld));
		Role * pTemRole = GetRolePtrByID(p->dwRoleID);
		if(P_VALID(pTemRole))
			pOld->bOnline = true;
		else
			pOld->bOnline = false;
		m_mapRoleInfo.ChangeValue(p->dwRoleID,pOld);
	}
}

VOID RoleMgr::SetRoleClass(DWORD dwRoleID, BYTE byClass)
{
	if (byClass >= EV_End || byClass <= EV_Null)
		return;

	tagRoleInfo * pOld = m_mapRoleInfo.Peek(dwRoleID);
	if(P_VALID(pOld))
	{
		pOld->byClass = byClass;
		m_mapRoleInfo.ChangeValue(dwRoleID, pOld);
	}
}

tagDWORDTime*  RoleMgr::GetRoleCreateTime(DWORD dwRoleID)
{
      return m_mapRoleCreateTime.Peek(dwRoleID);
}

VOID RoleMgr::SetRoleCreateTime(const tagDWORDTime* pRoleCreateTime,DWORD dwRoleID)
{
         m_mapRoleCreateTime.Add(dwRoleID,(tagDWORDTime*)pRoleCreateTime);
}

//--------------------------------------------------------------------------------------------
// �����������ش���
//--------------------------------------------------------------------------------------------
VOID RoleMgr::PlayerOnline(Role *pRole, BOOL bFirst)
{
	// �����̳�����
	INT nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		INT nSzMsg = sizeof(tagNS_MallItem) - 1 + sizeof(tagMallItemProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallItem);
		pSend->nItemNum = nGoodsNum;
		g_mall.GetAllItems(pSend->byData);

		pRole->SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	INT nPackNum = g_mall.GetPackNum();
	if (nPackNum > 0)
	{
		INT nSzMsg = sizeof(tagNS_MallPack) - 1 + sizeof(tagMallPackProto) * nPackNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallPack);
		pSend->nPackNum = nPackNum;
		g_mall.GetAllPacks(pSend->byData);

		pRole->SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	// ����ǵ�һ�����ߣ�����ó������ߺ���
	pRole->Online(bFirst);

	// ����������ߵ��¼�
	g_socialMgr.SendLoginToFriend(pRole);

	// ������ߣ�֪ͨ���
	g_socialMgr.SendEnemyLoginToRole(pRole);

	// �������������
	SendBoardQuest2Role(pRole, EQBT_Reward);
	SendBoardQuest2Role(pRole, EQBT_YellowList);
	//-----------------------------------------


	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());

	if(P_VALID(pRoleInfo))
		pRoleInfo->bOnline = true;

	g_FamilyMgr.OnMemberLogin(pRole);
}

VOID RoleMgr::SendBoardQuest2Role(Role *pRole, EQuestBoardType eQuestBoardType)
{
	if (!P_VALID(pRole))
		return;

	tagNS_OpenQuestBoard send;
	send.dwErrorCode = E_Success;
	send.eQuestBoardType = eQuestBoardType;

	RoleBoardQuestMap RoleQuest = pRole->GetBoardQuestMgr(eQuestBoardType)->GetRoleBoardQuest();
	RoleBoardQuestMap::TMapIterator it = RoleQuest.Begin();
	tagRoleBoardQuest* pQuest = NULL;
	INT nIndex = 0;
	while(RoleQuest.PeekNext(it, pQuest))
	{
		if (nIndex>=QUEST_BOARD_MAX_CAPACITY)
		{
			ASSERT(nIndex<QUEST_BOARD_MAX_CAPACITY);
			break;
		}
		send.u16Quest[nIndex] = pQuest->u16BoardID;
		++nIndex;
	}
	pRole->SendMessage(&send, send.dwSize);
}


//--------------------------------------------------------------------------------------------
// �����������ش���
//--------------------------------------------------------------------------------------------
VOID RoleMgr::PlayerOutLine(Role* pRole)
{
	// �����ʱ������Ϣ������
	g_socialMgr.SendLogoutToFriend(pRole);

	// ����˳�С��
	DWORD dwTeamID = pRole->GetTeamID();
	if( GT_INVALID != dwTeamID )
	{
		g_groupMgr.RoleOutline(pRole);
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());

	if(P_VALID(pRoleInfo))
	{
		pRoleInfo->bOnline = false;
		pRoleInfo->dwTimeLastLogout = pRole->GetLogoutTime();
	}
}

//--------------------------------------------------------------------------------------------
// ���ɾ����ɫʱ��ɾ�����ݿ�����صĺ�����Ϣ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::DeletAllFriend(DWORD dwRoleID)
{
	tagNDBC_DelAllFriRel	sendFri;
	sendFri.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendFri, sendFri.dwSize);

	tagNDBC_DelAllFriValRel	sendVal;
	sendVal.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendVal, sendVal.dwSize);
}

//--------------------------------------------------------------------------------------------
// ���ɾ����ɫʱ��ɾ������ύ����Ԫ�����׶���
//--------------------------------------------------------------------------------------------
VOID RoleMgr::DeleteYBOrder(DWORD dwRoleID)
{
	tagYuanBaoOrder* pYBSellOrder= g_tradeYB.GetYBSellOrder(dwRoleID);
	if(P_VALID(pYBSellOrder))
		g_tradeYB.DeleteYBOrder(pYBSellOrder, EYBOM_Close);

	tagYuanBaoOrder* pYBBuyOrder= g_tradeYB.GetYBBuyOrder(dwRoleID);
	if(P_VALID(pYBBuyOrder))
		g_tradeYB.DeleteYBOrder(pYBBuyOrder, EYBOM_Close);
}

//--------------------------------------------------------------------------------------------
// ɾ�����е����߽�ɫ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::EraseAllRoleInfo()
{
	m_RoleInfoMapMutex.Acquire();

	tagRoleInfo* pInfo = NULL;
	RoleInfoMap::TMapIterator it2 = m_mapRoleInfo.Begin();
	while( m_mapRoleInfo.PeekNext(it2, pInfo) )
	{
		SAFE_DEL(pInfo);
	}
	m_mapRoleInfo.Clear();

	m_RoleInfoMapMutex.Release();
}

//--------------------------------------------------------------------------------------------
// �����������ֵ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::ResetRoleRep( ECLanType eClanType, EReputationLevel eRepLvl, tagDWORDTime dwtResetTime )
{
	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = GT_INVALID;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			g_fameHall.RoleRepUpdate(pRole, eClanType);
		}
	}
}

//--------------------------------------------------------------------------------------------
// ������������Ҽ�һ��buff��ֻ�������̵߳��ã�
//--------------------------------------------------------------------------------------------
VOID RoleMgr::AddAllRoleBuff(DWORD dwBuffTypeID)
{
	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return;

	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = -1;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
		}
	}
}
//tbd:ͨ������id��ȡ����
VOID RoleMgr::GetRoleNameByNameID( const DWORD dwNameID, LPTSTR szName )
{
	// ��ʱ��RoleID��NameID
	GetRoleNameByID(dwNameID, szName);
}

//--------------------------------------------------------------------------------------------
// ���¿�����ʵ��ֵ�ͽ���ʵ��ֵ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::RenewAdvanceStrength()
{
	m_RoleMapMutex.Acquire();

	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = GT_INVALID;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			//INT nTmp = pRole->GetStrength().nConsumptiveStrength;
			//INT nTemp = pRole->GetStrength().nAdvanceStrength;

			// ������ʵ��ֵ�ۼӵ�������ʵ��ֵ��
			//pRole->SetConsumptiveStrength(nTemp+nTmp);
			
			// ����ʵ��ֵ����
			pRole->SetScriptData(99, 0);
			pRole->SetScriptData(100, 0);
			pRole->SetScriptData(101, 0);
			pRole->SetScriptData(95, 0);
			pRole->SetAdvanceStrength(0);
		}
	}

	m_RoleMapMutex.Release();
}

//--------------------------------------------------------------------------------------------
// ���¿�����ʵ��ֵ�ͽ���ʵ��ֵ
//--------------------------------------------------------------------------------------------
VOID RoleMgr::RenewClergyCandidateType()
{
	m_RoleMapMutex.Acquire();

	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = GT_INVALID;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole) )
		{
			pRole->SetClergyCandidateType(ECGT_Null);
		}
	}

	m_RoleMapMutex.Release();
}

VOID RoleMgr::SaveRoleInfoToDB(DWORD dwRoleID)
{
	tagRoleInfo* pRoleInfo = m_mapRoleInfo.Peek(dwRoleID);
	if(P_VALID(pRoleInfo))
	{
		tagNDBC_SaveOneRoleInfo send;
		send.RoleInfo = *pRoleInfo;
		g_dbSession.Send(&send, send.dwSize);
	}
}

INT RoleMgr::GetOnlineRoleNum(VOID)
{
	return m_mapRole.Size();
}

VOID RoleMgr::OfflineRoleAddBuff(DWORD dwRoleID, DWORD dwBuffID,
								 INT8 n8ModifierNum /*= 0*/,
								 DWORD dwItemTypeID /*= GT_INVALID*/,
								 DWORD dwSrcSkillID/* = GT_INVALID*/,
								 INT64 n64Serial/* = 0*/,
								 INT8 n8CurLapTimes/* = 1*/,
								 INT32 nPersistTick/* = 1*/)
{
	Role* pRole = GetRolePtrByID(dwRoleID);
	if ( NULL == pRole)
	{
		tagNDBC_InsertBuff2DB msgInsertBuff;
		msgInsertBuff.dwRoleID = dwRoleID;
		ZeroMemory(&msgInsertBuff.BuffSave,sizeof(msgInsertBuff.BuffSave));
		msgInsertBuff.BuffSave.dwBuffID	= Buff::GetIDFromTypeID(dwBuffID);
		msgInsertBuff.BuffSave.n8Level = Buff::GetLevelFromTypeID(dwBuffID);
		msgInsertBuff.BuffSave.n8ModifierNum	= n8ModifierNum;
		msgInsertBuff.BuffSave.dwItemTypeID	= dwItemTypeID;
		msgInsertBuff.BuffSave.dwSrcSkillID	= dwSrcSkillID;
		msgInsertBuff.BuffSave.dwSrcUnitID	= dwRoleID;
		msgInsertBuff.BuffSave.n64Serial	= n64Serial;
		msgInsertBuff.BuffSave.n8CurLapTimes	= n8CurLapTimes;
		msgInsertBuff.BuffSave.nPersistTick	= nPersistTick;

		g_dbSession.Send(&msgInsertBuff,msgInsertBuff.dwSize);
		return ;
	}
}

VOID RoleMgr::ClearBuyMallItemCount()
{
	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = -1;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole))
		{
			pRole->ClearBuyMallItemCount();
		}
	}

	tagNDBC_ClearAllMallBuyItemCount send;
	g_dbSession.Send(&send, send.dwSize);
}

RoleMgr g_roleMgr;

//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_mgr.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݹ����������ڸ���������������ɼ�ɾ�������洢�������ڵ���������
//------------------------------------------------------------------------------
#pragma once

#include "mutex.h"
#include "world_session.h"
#include "group_mgr.h"

struct tagRoleInfo;
struct tagRoleDataLoad;
class Role;
class PlayerSession;
enum EQuestBoardType;

//------------------------------------------------------------------------------
// ���߽�ɫά���ṹ
//------------------------------------------------------------------------------
struct tagRoleNode
{
	Role	*pRole;
	INT		nNext;
};

struct tagRoleMgr
{
	tagRoleNode	sAllRole[MAX_PLAYER_NUM];
	INT			nFreeHeadIndex;
	INT			nMaxUseIndex;
	INT			nLastSaveIndex;
	INT			nFreeNodeNum;		// �����

	// ������������ݣ�������洢ָ�������ͷ�
	VOID	Reset();
	INT		GetMaxUsedIndex() const;
	Role*	GetRole(INT nIndex);
	Role*	GetRoleNeedSave();

	INT		Add(Role *pRole);
	Role*	Remove(INT nIndex);
};


inline INT tagRoleMgr::GetMaxUsedIndex() const { return nMaxUseIndex; }

inline Role* tagRoleMgr::GetRole(INT nIndex)
{
	if(nIndex < 0 || nIndex >= MAX_PLAYER_NUM)
	{
		return NULL;
	}

	return sAllRole[nIndex].pRole;
}

inline INT tagRoleMgr::Add(Role *pRole)
{
	if(nFreeHeadIndex < 0 || nFreeHeadIndex >= MAX_PLAYER_NUM)
	{
		return GT_INVALID;
	}

	INT nRetIndex = nFreeHeadIndex;
	tagRoleNode *pNode = &sAllRole[nRetIndex];

	// �������ÿ���ͷ
	nFreeHeadIndex = pNode->nNext;
	if(nMaxUseIndex < nRetIndex)
	{
		nMaxUseIndex = nRetIndex;
	}

	// ��������
	pNode->pRole = pRole;
	pNode->nNext = GT_INVALID;

	--nFreeNodeNum;

	return nRetIndex;
}

inline Role* tagRoleMgr::Remove(INT nIndex)
{
	if(nIndex < 0 || nIndex >= MAX_PLAYER_NUM)
	{
		return NULL;
	}

	Role *pRole = sAllRole[nIndex].pRole;

	// ���սڵ�
	sAllRole[nIndex].nNext = nFreeHeadIndex;
	sAllRole[nIndex].pRole = NULL;
	nFreeHeadIndex = nIndex;

	++nFreeNodeNum;

	return pRole;
}

//------------------------------------------------------------------------------
// ��ɫά����
//------------------------------------------------------------------------------

class RoleMgr
{
	friend VOID	GroupMgr::OnAddAllRoleBuff(DWORD dwSenderID, LPVOID pEventMessage);
public:
	typedef TMap<DWORD, tagRoleInfo*> RoleInfoMap;
	typedef TMap<DWORD, INT> RoleMap;
	typedef TMap<DWORD, DWORD> RoleIDMap;
	typedef TMap<DWORD, tagDWORDTime*> RoleCreateTime;  //��ɫ��������Ĵ���ʱ�� 

public:
	RoleMgr();
	~RoleMgr();
	BOOL Init();
	VOID Destroy();

public:
	BOOL CreateRoleInfo(const tagRoleInfo* pInfo);
	BOOL DeleteRoleInfo(const DWORD dwRoleID);

	VOID ChangeRoleName(DWORD dwRoleID, LPCTSTR szRoleName);
	VOID BreakMarriage(DWORD dwRoleID1,DWORD dwRoleID2);
	VOID GetMarriage(DWORD dwRoleID1,DWORD dwRoleID2);

	VOID ModifyMasterApprenticeCount(DWORD dwRoleID, INT nCount);

	Role* CreateRole(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession, BOOL& bFirst);
	BOOL DeleteRole(const DWORD dwRoleID);

	VOID SaveOneRoleToDB();
	VOID SaveAllRolesToDB();

	VOID SaveRoleInfoToDB(DWORD dwRoleID);
	DWORD GetRoleIDByNameCrc(const DWORD dwNameCrc);
	VOID  GetRoleNameByID(const DWORD dwRoleID, LPTSTR szName);
	VOID  GetRoleNameByNameID(const DWORD dwNameID, LPTSTR szName);
	Role* GetRolePtrByID(const DWORD dwRoleID);
	Role* GetRandRolePtr(BOOL bGetFirst = FALSE);
	tagRoleInfo* GetRoleInfo(const DWORD dwRoleID);
	void SetRoleInfo(const tagRoleInfo * p);
	VOID SetRoleClass(DWORD dwRoleID, BYTE byClass);

	BOOL IsRoleBelongToWorld(const DWORD dwRoleID)
	{
		return P_VALID(GetRoleInfo(dwRoleID));
	}

	VOID SendWorldMsg(LPVOID pMsg, DWORD dwSize);
	VOID ResetRoleRep(ECLanType eClanType, EReputationLevel eRepLvl, tagDWORDTime dwtResetTime);
	VOID EraseAllRoleInfo();

	template<typename UnitOperation>
	VOID ForEachRoleInWorld(UnitOperation oper);

	template<typename Condition,typename p,typename UnitOperation>
	VOID ForEachSpec1(Condition func,p conv,UnitOperation oper);

	template<typename Condition,typename x,typename y,typename UnitOperation>
	VOID ForEachSpec2(Condition func,x conx,y cony,UnitOperation oper);


	INT32 GetRoleLevelLimit() { return m_nRoleLevelLimit; }

	VOID RenewAdvanceStrength();
	VOID RenewClergyCandidateType();

	// �����������߽�ɫ��ƣ��ֵ
	VOID ResetWearyOfEachRoleInWorld();
	tagDWORDTime*  GetRoleCreateTime(DWORD dwRoleID);
    VOID SetRoleCreateTime(const tagDWORDTime* pRoleCreateTime,DWORD dwRoleID);

	// ������update֮ǰ��֮������ȫ����Ϊ��û���õ�m_RoleMapMutex�������߱���Ե��øú����İ�ȫ����ʮ������ſ���ʹ�á�
	INT GetOnlineRoleNum(VOID);

	VOID OfflineRoleAddBuff(DWORD dwRoleID, DWORD dwBuffID,
		INT8 n8ModifierNum = 0,
		DWORD dwItemTypeID = GT_INVALID,
		DWORD dwSrcSkillID = GT_INVALID,
		INT64 n64Serial = 0,
		INT8 n8CurLapTimes = 1,
		INT32 nPersistTick = 1);

	VOID ClearBuyMallItemCount();

private:
	VOID PlayerOnline(Role *pRole, BOOL bFirst);
	VOID PlayerOutLine(Role* pRole);
	VOID DeletAllFriend(DWORD dwRoleID);
	VOID DeleteYBOrder(DWORD dwRoleID);
	VOID AddAllRoleBuff(DWORD dwBuffTypeID);

	VOID SendBoardQuest2Role(Role *pRole, EQuestBoardType eQuestBoardType);
private:
	TObjRef<FastCode>	m_pFastCode;

private:
	RoleInfoMap			m_mapRoleInfo;
	RoleIDMap			m_mapRoleNameCrcID;
	RoleCreateTime      m_mapRoleCreateTime;
	Mutex				m_RoleInfoMapMutex;

	RoleMap				m_mapRole;
	tagRoleMgr			m_vctRole;
	Mutex				m_RoleMapMutex;

	INT32				m_nRoleLevelLimit;
};

template<typename UnitOperation>
VOID RoleMgr::ForEachRoleInWorld( UnitOperation oper )
{
	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = -1;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole))
		{
			oper(pRole);
		}
	}
}
template<typename Condition,typename p,typename UnitOperation>
VOID RoleMgr::ForEachSpec1(Condition func,p conv,UnitOperation oper)
{
	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = -1;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole))
		{
			if( func(pRole,conv) )
				oper(pRole);
		}
	}
}
template<typename Condition,typename x,typename y,typename UnitOperation>
VOID RoleMgr::ForEachSpec2(Condition func,x conx,y cony,UnitOperation oper)
{
	RoleMap::TMapIterator it = m_mapRole.Begin();
	INT nIndex = -1;
	Role* pRole = NULL;
	while( m_mapRole.PeekNext(it, nIndex) )
	{
		pRole = m_vctRole.GetRole(nIndex);
		if( P_VALID(pRole))
		{
			if( func(pRole,conx,cony) )
				oper(pRole);
		}
	}
}

extern RoleMgr g_roleMgr;

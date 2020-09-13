// ���������
#include "stdafx.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/msg_family.h"
#include "../WorldDefine/filter.h"
#include "../ServerDefine/msg_family.h"
#include "role.h"
#include "role_mgr.h"
#include "family_member.h"
#include "family.h"
#include "family_join.h"
#include "family_mgr.h"
#include "db_session.h"
#include "family_sprite.h"
#include "family_sprite_mgr.h"


FamilyMgr g_FamilyMgr;

struct SafeDelFamily
{
	VOID operator() (Family* pFamily)
	{
		SAFE_DEL(pFamily);
	}
};
struct UpdateFamily
{
	UpdateFamily(DWORD dwUpdateTime)
		:m_dwUpdateTime(dwUpdateTime)
	{
	}
	VOID operator() (Family* pFamily)
	{
		if (P_VALID(pFamily))
		{
			pFamily->Update();
			tagDWORDTime dwUpdateTime = static_cast<tagDWORDTime>(m_dwUpdateTime);
			if (dwUpdateTime.hour == 0 &&	dwUpdateTime.min == 0 && !pFamily->IsResetFamilyActive())
			{
				// ���Ӽ��������ɳ���
				pFamily->GetFamilyQuest().IncFamilySpiriteExp();

				pFamily->SetResetFamilyActive(TRUE);
				pFamily->ResetFamilyActive();
			}
			if (dwUpdateTime.hour == 0 &&	dwUpdateTime.min == 1 && pFamily->IsResetFamilyActive())
				pFamily->SetResetFamilyActive(FALSE);
		}
	}
private:
	DWORD m_dwUpdateTime;
};

struct SetFamilyQuestInitOK
{
	VOID operator() (Family* pFamily)
	{
		if (P_VALID(pFamily))
			pFamily->SetQuestInitOK();
	}
};

struct  AddTeamMember2Family
{
	AddTeamMember2Family(Family* pFamily, Team* pTeam)
		:m_pTeam(pTeam), m_pFamily(pFamily)
	{
	}
	VOID operator() (Role* pMember)
	{
		if (P_VALID(m_pFamily) && P_VALID(pMember) && P_VALID(m_pTeam))
		{
			m_pFamily->AddMember(pMember->GetID(), GetCurrentDWORDTime());

			// ÿ�������Ա�����Զ��ӶԷ�Ϊ���ѣ����Զ����Ӻ��ѹ��ܣ�
			m_pFamily->GetFamilyMember().MakeFriendWithOthers(pMember->GetID());

			pMember->SetFamilyID(m_pFamily->GetFamilyAtt().dwFamilyID);

			// �ӳ�Ĭ�ϳ�Ϊ�峤,��Ҫ�ж��Ƿ�Ϊ�ӳ�
			m_pFamily->SetMemberLeader(m_pTeam->GetMemID(0));

			m_pFamily->SendRoleJoinFamily2DB(pMember->GetID());
		}
	}
private:
	Family* m_pFamily;
	Team* m_pTeam;
};

struct GetFamilyMemberInfo
{
	GetFamilyMemberInfo(Family* pFamily, tagNS_GetFamilyInfo* pGetFamilyInfoMsg)
		:m_pFamily(pFamily), m_nIdx(0), m_pFamilyInfoMsg(pGetFamilyInfoMsg)
	{
	}
	VOID operator()(tagFamilyMember* pFamilyMember)
	{
		if (P_VALID(m_pFamily) &&
			P_VALID(m_pFamilyInfoMsg) &&
			m_nIdx >= 0 && m_nIdx < MAX_FAMILY_MEMBER)
		{
			m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].dwFamilyID = m_pFamily->GetFamilyAtt().dwFamilyID;
			m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].bLeader = pFamilyMember->bLeader;
			tagRoleInfo* pMemberInfo = g_roleMgr.GetRoleInfo(pFamilyMember->dwRoleID);
			if (P_VALID(pMemberInfo))
			{
				Role* pMember = g_roleMgr.GetRolePtrByID(pFamilyMember->dwRoleID);
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].bySex = pMemberInfo->bySex;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].dwLoverID = pMemberInfo->dwLoverID;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].dwMapID = P_VALID(pMember)?pMember->GetMapID():GT_INVALID;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].dwRoleID = pMemberInfo->dwRoleID;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].eClassType = static_cast<EClassType>(pMemberInfo->byClass);
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].nHp = P_VALID(pMember)?pMember->GetAttValue(ERA_HP):GT_INVALID;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].nHpMax = P_VALID(pMember)?pMember->GetAttValue(ERA_MaxHP):GT_INVALID;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].nLevel = pMemberInfo->byLevel;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].nMp = P_VALID(pMember)?pMember->GetAttValue(ERA_MP):GT_INVALID;
				m_pFamilyInfoMsg->sFamilyInfo[m_nIdx].nMpMax = P_VALID(pMember)?pMember->GetAttValue(ERA_MaxMP):GT_INVALID;
				m_pFamilyInfoMsg->n8MemberNum = ++m_nIdx;
			}
		}
	}
private:
	Family* m_pFamily;
	INT8 m_nIdx;
	tagNS_GetFamilyInfo* m_pFamilyInfoMsg;
};

FamilyMgr::FamilyMgr()
{
	Init();
}
FamilyMgr::~FamilyMgr()
{
	Destroy();
}

BOOL	FamilyMgr::Init()
{
	return TRUE;
}
VOID	FamilyMgr::Update()
{
	m_FamilyJoin.Update();
	ForEachFamily(UpdateFamily(GetCurrentDWORDTime()));
}
VOID	FamilyMgr::Destroy()
{
	if (m_mapFamily.Size() == 0)
		return;

	ForEachFamily(SafeDelFamily());

	m_mapFamily.Clear();
}

VOID	FamilyMgr::SetQuestInitOK()
{	
	ForEachFamily(SetFamilyQuestInitOK());
}

DWORD	FamilyMgr::CreateFamily(Role* pCreator, LPCTSTR szFamilyName)
{
	if (!P_VALID(pCreator) || !P_VALID(szFamilyName))
		return GT_INVALID;

	const Team* pTeam = g_groupMgr.GetTeamPtr(pCreator->GetTeamID());
	if ( !P_VALID(pTeam))
		return E_CreateFamily_NoTeam;

	Family* pFamily = new Family(true);
	if (!P_VALID(pFamily))
		return GT_INVALID;

	DWORD dwTeamLeaderID = pTeam->GetMemID(0);
	DWORD dwFamilyID = TObjRef<Util>()->Crc32(szFamilyName);
	tstring strFamilyName(szFamilyName);
	pFamily->SetFamilyName(strFamilyName);
	pFamily->GetFamilyAtt().Init(dwFamilyID, dwTeamLeaderID, dwTeamLeaderID, 0, GetCurrentDWORDTime());

	// ��Ա��Ϊ�����Ա
	pTeam->ForEachRoleInTeam(AddTeamMember2Family(pFamily, const_cast<Team*>(pTeam)));

	m_mapFamily.Add(dwFamilyID, pFamily);

	// �������ݿ�
	SendCreateFamily2DB(pFamily->GetFamilyAtt(), strFamilyName.c_str());

	// ������ȫ���㲥����###�����������ƣ��������µļ���####���������������ƣ���
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pCreator->GetID());
	if (P_VALID(pRoleInfo))
	{
		INT32 nRoleNameLen = _tcsclen(pRoleInfo->szRoleName);
		INT32 nFamilyNameLen = _tcsclen(szFamilyName);
		INT32 nMsgSize = sizeof(tagNS_CreateFamilyBroad)+(nRoleNameLen + nFamilyNameLen)*sizeof(TCHAR);
		MCREATE_MSG(pSend, nMsgSize, NS_CreateFamilyBroad);
		if (P_VALID(pSend))
		{
			pSend->dwFamilyID = pFamily->GetFamilyAtt().dwFamilyID;
			pSend->dwRoleID = pCreator->GetID();
			_tcscpy_s(pSend->szName, nRoleNameLen+1, pRoleInfo->szRoleName);
			_tcscpy_s((pSend->szName+nRoleNameLen+1), nFamilyNameLen+1, szFamilyName);
			//g_roleMgr.SendWorldMsg(pSend, pSend->dwSize);
			pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member(pSend));

			tagNS_CreateFamilyAllBroad send;
			_tcscpy_s(send.szRoleName, nRoleNameLen+1, pRoleInfo->szRoleName);
			_tcscpy_s(send.szFamilyName, nFamilyNameLen+1, szFamilyName);
			g_roleMgr.SendWorldMsg(&send, send.dwSize);
		}
	}

	// ������������
	g_FamilySpriteMgr.CreateFamilySprite(dwFamilyID,szFamilyName);

	// ͬ�������Ծ��
	INT nTeamMemNum = pTeam->GetMemNum();
	INT nActive = FAMILY_ACTIVE_INCREASE_ONCE*nTeamMemNum;
	pFamily->ChangeFamilyActive(nActive);
	//pFamily->SetResetFamilyActive(TRUE);

	return E_FAMILY_SUCCESS;
}

VOID FamilyMgr::SendCreateFamily2DB(tagFamilyAtt& sFamilyAtt, LPCTSTR pFamilyName)
{
	if (!P_VALID(pFamilyName))
		return;
	INT32 nNameLen = _tcsclen(pFamilyName);
	if (nNameLen == 0)
		return;
	
	tagNDBC_CreateFamily send;
	send.sFamilyLoad.sFamilyAtt = sFamilyAtt;
	memcpy_s(send.sFamilyLoad.strFamilyName,
		MAX_FAMILY_NAME*sizeof(TCHAR),
		pFamilyName,
		nNameLen*sizeof(TCHAR));

	g_dbSession.Send(&send, send.dwSize);
}

DWORD FamilyMgr::CanCreateFamily(Role* pCreator, LPCTSTR szFamilyName)
{
	if (!P_VALID(pCreator))
		return GT_INVALID;

	// �Ƿ����ѽ���������
	DWORD dwNewFamilyID = g_world.LowerCrc32(szFamilyName);
	if(m_mapFamily.IsExist(dwNewFamilyID))
		return E_CreateFamily_NameExist;

	// �������Ϸ����ж�
	DWORD dwErrorCode = Filter::CheckName(szFamilyName, MAX_FAMILY_NAME_LEN, 
		g_attRes.GetVariableLen().nGuildNameMin, g_attRes.GetNameFilterWords());
	if(dwErrorCode != E_Success)
		return dwErrorCode;

	// 	�жϽ�ɫ�Ƿ�ӵ�ж��飬��δӵ�У�����ʾ��������4�˻�4��������Ӳſɴ������塱����ӵ�У�������һ��
	const Team* pTeam = g_groupMgr.GetTeamPtr(pCreator->GetTeamID());
	if (!P_VALID(pTeam))
		return E_CreateFamily_NoTeam;

	// 	�жϽ�ɫ�Ƿ�Ϊ�ӳ�������Ϊ�ӳ�������ʾ����ֻ�жӳ��ſɴ������塱����Ϊ�ӳ���������һ��
	if (!pTeam->IsLeader(pCreator->GetID()))
		return E_CreateFamily_NotLeader;

	// 	�ж϶����Ա�����Ƿ��4�ˣ��������ϣ�����ʾ�������Ķ������������4�ˣ������޷��������塱�������ϣ��������һ��
	dwErrorCode = (const_cast<Team*>(pTeam))->CanCreateFamily();
	if (dwErrorCode != E_FAMILY_SUCCESS)
		return dwErrorCode;

	return E_FAMILY_SUCCESS;
}

DWORD	FamilyMgr::DismissFamily(Role* pLeader)
{
	if (!P_VALID(pLeader))
		return GT_INVALID;

	Family* pFamily = g_FamilyMgr.GetFamily(pLeader->GetFamilyID());
	if (!P_VALID(pFamily))
	{
		ILOG->Write(_T("FamilyMgr::DismissFamily,pFamily Invalid!!!"));
		return GT_INVALID;
	}

	if (pFamily->GetFamilyAtt().dwLeaderID != pLeader->GetID())
		return E_DismissFamily_NotLeader;

	if (pFamily->GetFamilyMember().GetFamilyMemberCnt() > 1)
		return E_DismissFamily_FamilyNotEmpty;

	// Todo �����Ѿ��Ǽǵ�����
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(pFamily->GetID());
	if(P_VALID(pFamilySprite)) 
		pFamilySprite->RemoveAllRoleSprite();

	g_FamilySpriteMgr.DelFamilySprite(pFamily->GetID());

	// ֪ͨ���ݿ�ɾ���������
	SendDismissFamily2DB(pLeader->GetFamilyID());
	pFamily->SendLeaveFamily2DB(pLeader->GetID());

	pLeader->TryAddBuffToMyself(LEAVE_FAMILY_BUFF_ID);

	// ֪ͨ�峤�������ɢ��
	tagNS_FamilyDismiss send;
	pLeader->SendMessage(&send, send.dwSize);
	
	m_mapFamily.Erase(pLeader->GetFamilyID());

	pLeader->SetFamilyID(GT_INVALID);

	SAFE_DEL(pFamily);

	return E_FAMILY_SUCCESS;
}

VOID FamilyMgr::SendDismissFamily2DB(DWORD dwFamilyID)
{
	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return;
	tagNDBC_DismissFamily send;
	send.dwFamilyID = dwFamilyID;
	g_dbSession.Send(&send, send.dwSize);

	tagNDBC_DeleteFamilyQuest msg;
	msg.dwFamilyID = dwFamilyID;
	g_dbSession.Send(&msg, msg.dwSize);
}

VOID FamilyMgr::SendFamilyProcFailed2Client(Role* pRole, DWORD dwErrorCode)
{
	if (P_VALID(pRole))
	{
		tagNS_FamilyProcFailed send;
		send.dwErrorCode = dwErrorCode;
		pRole->SendMessage(&send, send.dwSize);
	}
}

DWORD FamilyMgr::InviteRoleJoin(Role* pInviter, DWORD dwInviteeID)
{
	if (!P_VALID(pInviter))
		return GT_INVALID;

	tagRoleInfo* pInviterInfo = g_roleMgr.GetRoleInfo(pInviter->GetID());
	if (!P_VALID(pInviterInfo))
		return GT_INVALID;
	INT nNameLen = _tcsclen(pInviterInfo->szRoleName);
	if (nNameLen == 0)
		return GT_INVALID;

	Family* pFamily = g_FamilyMgr.GetFamily(pInviter->GetFamilyID());
	if (!P_VALID(pFamily))
		return GT_INVALID;

	Role* pInvitee = g_roleMgr.GetRolePtrByID(dwInviteeID);
	if (!P_VALID(pInvitee))
		return E_InviteRole_NoRole;

	if (pInvitee->GetFamilyID() != GT_INVALID)
		return E_InviteRole_RoleHadFamily;

	if (pFamily->GetFamilyMember().GetFamilyMemberCnt() >= MAX_FAMILY_MEMBER)
		return E_InviteRole_FamilyFull;

	if (pInvitee->IsHaveBuff(Buff::GetIDFromTypeID(LEAVE_FAMILY_BUFF_ID)))
		return E_InviteRole_JustLeaveFamily;

	INT32 nMsgSize = sizeof(tagNS_FamilyJoinReq)+(nNameLen)*sizeof(TCHAR);
	MCREATE_MSG(pSend, nMsgSize, NS_FamilyJoinReq);
	if (!P_VALID(pSend))
		return GT_INVALID;

	pSend->dwFamilyID = pFamily->GetID();
	pSend->dwSrcRoleID = pInviter->GetID();
	memcpy_s(pSend->szSrcRoleName, nNameLen*sizeof(TCHAR),
		pInviterInfo->szRoleName, nNameLen*sizeof(TCHAR));
	pInvitee->SendMessage(pSend, pSend->dwSize);

	tagJoinFamilyReq JoinFamilyReq;
	JoinFamilyReq.bInviteJoin = TRUE;
	JoinFamilyReq.dwAddReqTime = GetCurrentDWORDTime();
	JoinFamilyReq.dwJoinerID = dwInviteeID;
	JoinFamilyReq.dwMemberID = pInviter->GetID();
	m_FamilyJoin.AddJoinReq(JoinFamilyReq);

	return E_FAMILY_SUCCESS;
}

DWORD FamilyMgr::ApplyRoleJoin(Role* pApply, DWORD dwMemberID)
{
	if (!P_VALID(pApply))
		return GT_INVALID;

	Role* pMember = g_roleMgr.GetRolePtrByID(dwMemberID);
	if (!P_VALID(pMember))
		return E_ApplyJoin_DestRoleOffLine;

	Family* pFamily = g_FamilyMgr.GetFamily(pMember->GetFamilyID());
	if (!P_VALID(pFamily))
		return E_ApplyJoin_FamilyNotExist;

	if (pApply->GetFamilyID() != GT_INVALID)
		return E_ApplyJoin_RoleHadFamily;

	if (pApply->IsHaveBuff(Buff::GetIDFromTypeID(LEAVE_FAMILY_BUFF_ID)))
		return E_ApplyJoin_JustLeaveFamily;

	if (pFamily->GetFamilyMember().GetFamilyMemberCnt() >= MAX_FAMILY_MEMBER)
		return E_ApplyJoin_DestFamilyFull;

	tagNS_FamilyApply send;
	send.dwApplyRoleID = pApply->GetID();
	pMember->SendMessage(&send, send.dwSize);

	tagJoinFamilyReq JoinFamilyReq;
	JoinFamilyReq.bInviteJoin = FALSE;
	JoinFamilyReq.dwAddReqTime = GetCurrentDWORDTime();
	JoinFamilyReq.dwJoinerID = pApply->GetID();
	JoinFamilyReq.dwMemberID = dwMemberID;
	m_FamilyJoin.AddJoinReq(JoinFamilyReq);

	return E_FAMILY_SUCCESS;
}

VOID FamilyMgr::AddRole2Family(Family* pFamily, Role* pRole)
{
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
	if (P_VALID(pRoleInfo) && P_VALID(pFamily) && P_VALID(pRole))
	{
		INT32 nNameLen = _tcsclen(pRoleInfo->szRoleName);
		if (nNameLen == 0)
			return;

		pFamily->AddMember(pRoleInfo->dwRoleID, GetCurrentDWORDTime());
		pRole->SetFamilyID(pFamily->GetFamilyAtt().dwFamilyID);
		pRole->SyncFamilyActive(pFamily->GetFamilyAtt().nActive);
		// ÿ�������Ա�����Զ��ӶԷ�Ϊ���ѣ����Զ����Ӻ��ѹ��ܣ�
		pFamily->GetFamilyMember().MakeFriendWithOthers(pRoleInfo->dwRoleID);

		INT32 nMsgSize = sizeof(tagNS_FamilyJoinBroad)+(nNameLen)*sizeof(TCHAR);
		MCREATE_MSG(pSend, nMsgSize, NS_FamilyJoinBroad);
		if (!P_VALID(pSend))
			return;

		pSend->dwRoleID = pRole->GetID();
		pSend->dwFamilyID = pFamily->GetFamilyAtt().dwFamilyID;
		memcpy_s(pSend->szRoleName, nNameLen*sizeof(TCHAR),
			pRoleInfo->szRoleName, nNameLen*sizeof(TCHAR));

		// �����ڹ㲥�����������³�Ա����ӭ���������뱾���壡��
		pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member((tagNetCmd*)pSend));

		pFamily->SendRoleJoinFamily2DB(pRole->GetID());
	}
}

DWORD FamilyMgr::LeaveFamily(Role* pRole)
{
	if (!P_VALID(pRole))
		return GT_INVALID;

	Family* pFamily = GetFamily(pRole->GetFamilyID());
	if (!P_VALID(pFamily))
		return GT_INVALID;

	if (pFamily->GetFamilyAtt().dwLeaderID == pRole->GetID())
		return E_LeaveFamily_BeLeader;

	// �������ˣ�Ҳ�����뿪����
	if (pRole->IsRoleMarried())
		return E_LeaveFamily_BeMarried;

	//�뿪���巵���Ǽǵ�����
	pRole->FamilyRoleSpriteReturn();

	// ����Ƶ�����棺��###�뿪�˱����塱
	tagNS_FamilyLeaveBroad send;
	send.dwSrcRoleID = pRole->GetID();
	pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member((tagNetCmd*)&send));
	//pFamily->GetFamilyMember().KickMember(pRole->GetID());
	pFamily->KickMember(pRole->GetID());
	pRole->TryAddBuffToMyself(LEAVE_FAMILY_BUFF_ID);
	pRole->SetFamilyID(GT_INVALID);

	pFamily->SendLeaveFamily2DB(pRole->GetID());

	return E_FAMILY_SUCCESS;
}

DWORD FamilyMgr::KickFromFamily(Role* pKicker, DWORD dwRoleID)
{
	if (!P_VALID(pKicker))
		return GT_INVALID;

	Family* pFamily = GetFamily(pKicker->GetFamilyID());
	if (!P_VALID(pFamily))
		return GT_INVALID;

	if (pFamily->GetFamilyAtt().dwLeaderID != pKicker->GetID())
		return E_KickMember_NotLeader;

	if (dwRoleID == pKicker->GetID())
		return E_KickMember_CanNotKickSelf;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (pRole == NULL)
	{
		g_roleMgr.OfflineRoleAddBuff(dwRoleID, LEAVE_FAMILY_BUFF_ID);
		FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(pFamily->GetID());
		if(P_VALID(pFamilySprite))
			pFamilySprite->RemoveRoleSpriteOffline(dwRoleID);
	}
	else if (P_VALID(pRole))
	{
		//�������巵���Ǽǵ�����
		pRole->FamilyRoleSpriteReturn();
		pRole->TryAddBuffToMyself(LEAVE_FAMILY_BUFF_ID);
		pRole->SetFamilyID(GT_INVALID);
	}

	// ����Ƶ�����棺��###�����������塱
	tagNS_FamilyKickBroad send;
	send.dwDstRoleID = dwRoleID;
	pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member((tagNetCmd*)&send));
	//pFamily->GetFamilyMember().KickMember(dwRoleID);
	pFamily->KickMember(dwRoleID);
	pFamily->SendLeaveFamily2DB(dwRoleID);

	return E_FAMILY_SUCCESS;
}

DWORD FamilyMgr::LeaderHandOver(Role* pLeader, DWORD dwHeritorID)
{
	if (!P_VALID(pLeader))
		return GT_INVALID;

	Family* pFamily = GetFamily(pLeader->GetFamilyID());
	if (!P_VALID(pFamily))
		return GT_INVALID;

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwHeritorID);
	if (!P_VALID(pRoleInfo))
		return GT_INVALID;

	if (pFamily->GetFamilyAtt().dwLeaderID != pLeader->GetID())
		return E_HandOverLeader_NotLeader;

	if (!P_VALID(pFamily->GetFamilyMember().GetFamilyMember(dwHeritorID)))
		return E_HandOverLeader_TargetNotInFamily;

	tagNS_FamilyHandOverBroad send;
	send.dwDstRoleID = dwHeritorID;
	send.dwSrcRoleID = pLeader->GetID();
	pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member((tagNetCmd*)&send));
	pFamily->SetMemberLeader(dwHeritorID);

	pFamily->SendSetLeader2DB(pLeader->GetID(), FALSE);
	pFamily->SendSetLeader2DB(dwHeritorID, TRUE);

	return E_FAMILY_SUCCESS;
}

DWORD FamilyMgr::GetFamilyName(Role* pRole, DWORD dwFamilyID)
{
	if (!P_VALID(pRole))
		return GT_INVALID;

	Family* pFamily = GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return GT_INVALID;

	LPCTSTR pFamilyName = pFamily->GetFamilyName();
	if (!P_VALID(pFamily))
		return GT_INVALID;

	DWORD dwFamilyNameLen = _tcslen(pFamilyName);
	if (dwFamilyNameLen == 0)
		return GT_INVALID;

	DWORD dwSize = sizeof(tagNS_GetFamilyName) + (dwFamilyNameLen+1)*sizeof(TCHAR);
	MCREATE_MSG(pSend, dwSize, NS_GetFamilyName);
	if (!P_VALID(pSend))
		return GT_INVALID;

	memcpy_s(pSend->szFamilyName,
		(dwFamilyNameLen+1)*sizeof(TCHAR),
		pFamilyName,
		dwFamilyNameLen*sizeof(TCHAR));
	pSend->dwFamilyID = dwFamilyID;
	pRole->SendMessage(pSend, pSend->dwSize);

	return E_FAMILY_SUCCESS;
}

DWORD FamilyMgr::GetFamilyInfo(Role* pRole, DWORD dwFamilyID)
{
	if (!P_VALID(pRole))
		return GT_INVALID;

	// �����Ա�������������Ϣ
	if (pRole->GetFamilyID() != dwFamilyID)
		return GT_INVALID;

	Family* pFamily = GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return GT_INVALID;

	DWORD dwFamilyMemCnt = pFamily->GetFamilyMember().GetFamilyMemberCnt();
	if (dwFamilyMemCnt == 0)
		return GT_INVALID;

	tagFamilyMember* pFamilyMember = pFamily->GetFamilyMember().GetFamilyMember(pRole->GetID());
	if (!P_VALID(pFamilyMember))
		return GT_INVALID;

	DWORD dwSize = sizeof(tagNS_GetFamilyInfo) + (dwFamilyMemCnt-1)*sizeof(tagRoleFamilyInfo);
	MCREATE_MSG(pSend, dwSize, NS_GetFamilyInfo);
	if ( !P_VALID(pSend))
		return GT_INVALID;
	pFamily->GetFamilyMember().ForEachFamilyMember(GetFamilyMemberInfo(pFamily, pSend));
	pRole->SendMessage(pSend, pSend->dwSize);

	return E_FAMILY_SUCCESS;
}

VOID FamilyMgr::LoadAllFamily(tagFamilyLoad& sFamilyLoad)
{
	Family* pFamily = new Family(sFamilyLoad);
	if (P_VALID(pFamily))
		m_mapFamily.Add(pFamily->GetFamilyAtt().dwFamilyID, pFamily);
}

VOID FamilyMgr::LoadFamilyMember(INT nMemNum, tagFamilyMember* pFamilyMember)
{
	if (nMemNum <= 0 || !P_VALID(pFamilyMember))
		return;

	for (INT nCnt = 0; nCnt < nMemNum; nCnt++)
	{
		Family* pFamily = GetFamily(pFamilyMember[nCnt].dwFamilyID);
		if (!P_VALID(pFamily))
			return;

		pFamily->LoadFamilyMember(&pFamilyMember[nCnt]);
	}
}

BOOL FamilyMgr::IsFamilyInitOK()
{
	BOOL bRet	= TRUE;

	Family* pFamily	= NULL;
	FamilyMap::TMapIterator iter = m_mapFamily.Begin();
	while (m_mapFamily.PeekNext(iter, pFamily))
	{
		if (!P_VALID(pFamily))
			continue;

		if (!pFamily->IsFamilyInitOK())
		{
			ILOG->Write(_T("Family<id: %u> load failed! Please check db! \n"), pFamily->GetFamilyAtt().dwFamilyID);
			bRet = FALSE;
		}
	}

	return bRet;
}

VOID FamilyMgr::SendAllFamilyInfo2DB()
{
	INT32 nFamilyNum = GetFamilyNum();
	if(nFamilyNum <= 0)
	{
		return;
	}

	INT32 nMsgSz = sizeof(tagNDBC_SaveAllFamily) + (nFamilyNum - 1) * sizeof(tagFamilyLoad);
	MCREATE_MSG(pSend, nMsgSz, NDBC_SaveAllFamily);
	pSend->nFamilyNum	= nFamilyNum;

	INT32 nIndex	= 0;
	Family *pFamily	= NULL;
	FamilyMap::TMapIterator iter = m_mapFamily.Begin();
	while(m_mapFamily.PeekNext(iter, pFamily))
	{
		if (!P_VALID(pFamily))
			continue;
		LPCTSTR pFamilyName = pFamily->GetFamilyName();
		if (!P_VALID(pFamilyName))
			continue;

		pSend->pFamilyLoad[nIndex].sFamilyAtt = pFamily->GetFamilyAtt();
		INT nNameLen = _tcsclen(pFamilyName);
		memcpy_s(pSend->pFamilyLoad[nIndex].strFamilyName,
			sizeof(pSend->pFamilyLoad[nIndex].strFamilyName),
			pFamilyName, nNameLen*sizeof(TCHAR));
		nIndex++;
	}

	g_dbSession.Send(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
}

VOID FamilyMgr::OnMemberLogin(Role* pRole)
{
	if (!P_VALID(pRole))
		return;

	Family* pFamily = GetFamily(pRole->GetFamilyID());
	if (!P_VALID(pFamily))
		return;

	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	if (!pRole->IsTodayFirstLogin())
		goto SyncFamilyActive;

	// ��ֹ��23��59�֣������ڵļ����Ծ�ȼ����ֹ
	if (dwCurTime.hour == 23 && dwCurTime.min == 59)
		goto SyncFamilyActive;

	INT nActive = pFamily->GetFamilyAtt().nActive;
	nActive += FAMILY_ACTIVE_INCREASE_ONCE;

	pFamily->ChangeFamilyActive(nActive);

	return;

// ����˴ε�½��Ӱ������Ծ�ȣ���Ҫ����½�����ͬ����ǰ��Ծ��
SyncFamilyActive:
	pRole->SyncFamilyActive(pFamily->GetFamilyAtt().nActive);
}

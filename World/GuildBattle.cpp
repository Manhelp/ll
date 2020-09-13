//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: GuildBattle.cpp
// author: chdeng
// actor:
// data: 2009-01-26
// last:
// brief: ��ս
//-------------------------------------------------------------------------------
#include "stdafx.h"
#include "map.h"
#include "role.h"
#include "team.h"
#include "group_mgr.h"
#include "att_res.h"
#include "role_mgr.h"
#include "MsgInfoMgr.h"
#include "map_creator.h"
#include "GuildBattle.h"
#include "activity_mgr.h"
#include "../WorldDefine/msg_GuildBattle.h"
#include "../WorldDefine/msg_combat.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/msg_log.h"

// ȫ�ֻ���MAP��
stdext::hash_map<DWORD,tagGodRoleInfo> g_mapGodCacheRoleInfo;
Mutex					g_mutexGodCacheRoleInfo;	
const INT32	MAX_KILL_TIMES = 1000;

// ����ɫɱ���Ƿ���ˢ��
BOOL tagGodRoleInfo::SetBeKilledRole(DWORD inBeKilledRoleID, DWORD dwGuildID, BOOL bIsTeamFlag/* = FALSE*/)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(GetRoleID());
	if (!P_VALID(pRole)) return FALSE;


	// ��ֹˢ
	for ( int i = 0; i < 3; i++)
	{
		if (inBeKilledRoleID == sBeKilledRole[i].GetRoleID())
			return FALSE;
	}
	
	// ��������
	if ( GetKillTimes() < MAX_KILL_TIMES)
	{
		ModKillTimes(1);
		bChange = TRUE;
	}
	// �õ����类ɱ�����
	DWORD tmpMinTime(0);
	int   nTmpFlag(0);
	tmpMinTime = sBeKilledRole[0].GetTime();
	if ( tmpMinTime > sBeKilledRole[1].GetTime())
	{
		tmpMinTime = sBeKilledRole[1].GetTime();
		nTmpFlag = 1;
	}
	if ( tmpMinTime > sBeKilledRole[2].GetTime())
		nTmpFlag = 2;
	
	sBeKilledRole[nTmpFlag].Init();
	sBeKilledRole[nTmpFlag].SetRoleID(inBeKilledRoleID);
	sBeKilledRole[nTmpFlag].SetTime((DWORD)g_world.GetWorldTime());
	
	
	/// ������ӻ�ɱ
	if ( FALSE == bIsTeamFlag)
	{
		// �����������������ɵġ��������������ڱ�������ս�еĻ�ɱ�����Ѿ��ﵽXXն�����ǿֲ���
		BOOL IsSendFlag(FALSE);
		// �㲥
		if ( GetKillTimes() == 10)		 IsSendFlag = TRUE;
		else if ( GetKillTimes() == 20 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 40 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 60 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 80 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 150 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 200 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 250 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 300 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 450 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 550 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 600 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 650 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 700 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 750 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 800 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 850 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 900 ) IsSendFlag = TRUE;
		else if ( GetKillTimes() == 950 && FALSE == nKillStopFalg) { IsSendFlag = TRUE; nKillStopFalg = TRUE; }

		// ���߸ĳ����ط��򹥷��������
		if ( TRUE == IsSendFlag )
		{

			DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
			DWORD dwTmpNum = 100062;
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &dwGuildID);
			DWORD dwRoleID = GetRoleID();
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);

			DWORD dwKillTimes= GetKillTimes();
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, &dwKillTimes);
			g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pRole->GetMap());

			IsSendFlag = FALSE;
		}

		DWORD dwMsgInfoID = 0;
		if (GetKillTimes() == 100)
		{
			dwMsgInfoID = 100063;
		}
		else if (GetKillTimes() == 500)
		{
			dwMsgInfoID = 100064;
		}
		else if (GetKillTimes() == 1000)
		{
			dwMsgInfoID = 100065;
		}
		if (dwMsgInfoID != 0)
		{
			DWORD dwMsgInfoID = (DWORD)g_MsgInfoMgr.BeginMsgEvent();
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwMsgInfoID);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &dwGuildID);
			DWORD dwRoleID = GetRoleID();
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
			g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pRole->GetMap());
		}

		// ��ɱ����40������ң������Ӧ�ľ���
		Role* pBeKORole = g_roleMgr.GetRolePtrByID(inBeKilledRoleID);
		if ( NULL == pBeKORole )
			return FALSE;
		if ( pBeKORole->GetLevel() >= 40)
		{
			// ������ɱ����Ӧ�ľ���
			const tagKillerExp* pKillExp = g_attRes.GetKillObjExpProto(pBeKORole->GetLevel());
			if ( P_VALID(pKillExp))
			{
				if ( pKillExp->dwExp > 0 )
				{
					pRole->ExpChange(INT64(pKillExp->dwExp));
					IncExp(pKillExp->dwExp);
				}
			}
		}
	}
	else if ( TRUE == bIsTeamFlag) //  ��ӻ�ɱ
	{
		/// �����Ķ����ɱ�˵з���XXX�������������ɱ����+1����þ�������㡣��
		//  g_attRes.GetKillObjExpProto(pBeKORole->GetLevel());
		// ��ɱ����40������ң������Ӧ�ľ���
		Role* pBeKORole = g_roleMgr.GetRolePtrByID(inBeKilledRoleID);
		ASSERT(P_VALID(pBeKORole));
		if ( P_VALID(pBeKORole) && pBeKORole->GetLevel() >= 40)
		{
			DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
			DWORD   dwTmpNum = 1438;
			DWORD	Exp = 0;
				
			if ( P_VALID(pBeKORole) )
			{
				// ������ɱ����Ӧ�ľ���
				const tagKillerExp* pKillExp = g_attRes.GetKillObjExpProto(pBeKORole->GetLevel());
				if ( P_VALID(pKillExp))
				{
					Exp = pKillExp->dwExp;	
					pRole->ExpChange(INT64(pKillExp->dwExp));
				}
			}

			Team* pTemp = (Team*)g_groupMgr.GetTeamPtr(pRole->GetTeamID());
			if ( !P_VALID(pTemp))
				return FALSE;

			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_BigBroadcast, &dwTmpNum);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &inBeKilledRoleID);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, &Exp);
			g_MsgInfoMgr.DispatchTeamMapMsgEvent(dwMsgInfoID, pRole->GetMap(),pTemp);
		}
	}	

	return TRUE;
}


VOID tagGodRoleInfo::Update()
{
	tagDWORDTime dwTime = GetCurrentDWORDTime();

	if ( TRUE == bChange)
	{
		SendChangeActiveTimeAndKillTime();
	}

	if ( dwTime.hour == 20)
	{
		// ��ʱ������Һ���
		Role* pRole = g_roleMgr.GetRolePtrByID(GetRoleID());
		if (P_VALID(pRole))
		{
			INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
			if( g_world.IsPkConstraintsOpen() && pRole->IsUsingPKConstraintsSafeGuradMinLevel() )
			{
				Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
			}
			if ( pRole->GetLevel() <= Max_PK_SafeGuard_Level)
			{
				if ( pRole->IsInRoleState(ERS_PK) )
					pRole->UnsetRoleState(ERS_PK);
// 				if ( pRole->IsInRoleState(ERS_PKEX))
// 					pRole->UnsetRoleState(ERS_PKEX);
			}
			else	
			{
				if (!pRole->IsInRoleState(ERS_PK))
					pRole->SetRoleState(ERS_PK);
// 				if (!pRole->IsInRoleState(ERS_PKEX))
// 					pRole->SetRoleState(ERS_PKEX);
			}
		}
	}

}

VOID tagGodRoleInfo::UnsetState(EState eState)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(GetRoleID());
	if (P_VALID(pRole))
	{
		pRole->UnsetState(eState);
	}
}

void tagGodRoleInfo::SendChangeActiveTimeAndKillTime()
{
	Role* pRole = g_roleMgr.GetRolePtrByID(GetRoleID());
	ASSERT(P_VALID(pRole));
	if (!P_VALID(pRole))
		return;

	DWORD dwFinialPoints = CAL_FINIALPOINTS(GetActiveTime(), GetKillTimes());
	if (dwFinialPoints > MAX_MIRACLES_POINTS)
		dwFinialPoints = MAX_MIRACLES_POINTS;

	/// �����ɫ����
	tagNS_GodPersonalState pSend;
	pSend.dwLiveTime = GetActiveTime();
	pSend.nKillTime  = (int)GetKillTimes();
	pSend.nGuildContribute = dwFinialPoints/20;
	pSend.nGuildExploit = dwFinialPoints/2;
	pSend.nMoney = nMoney;
	pSend.nExp = nExp;

	pRole->SendMessage(&pSend, pSend.dwSize);
}

VOID  tagGodRoleInfo::UpdateMin()
{
	if ( TRUE == bIsInGodMiracleArea &&
		P_VALID(CGodMiracleMgr::GetInst()) &&
		CGodMiracleMgr::GetInst()->IsGodMiracleTime())
	{
		DWORD dwLastActiveTime = dwActiveTime;
		dwActiveTime ++;
		GiveActiveTimeReward(dwActiveTime, dwLastActiveTime);
		bChange = TRUE;	
	}
}
VOID tagGodRoleInfo::GiveActiveTimeReward(DWORD dwActiveTime, DWORD dwLastTime)
{
	DWORD dwExpRewardParam = 0;
	DWORD dwSilverRewardParam = 0;
	if (!CGodMiracle::IsNeedReward(EBFRT_ActiveTimeRewards, dwActiveTime, dwLastTime, dwExpRewardParam, dwSilverRewardParam))
		return;

	// ������
	GiveReward(dwGodID, EBFRT_ActiveTimeRewards, GetRoleID(), dwExpRewardParam, dwSilverRewardParam, dwActiveTime);
}

VOID tagGodRoleInfo::GiveReward(DWORD	dwGodID, EBattleFieldRewardType eRewardType, DWORD dwRoleID, DWORD dwExpRewardParam, DWORD dwSilverRewardParam, DWORD dwTimeOrPoint )
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT(P_VALID(pRole));
	if (!P_VALID(pRole))
		return;

	if (!P_VALID(CGodMiracleMgr::GetInst()))
		return;

	const tagKillerExp* pBaseRewards = g_attRes.GetKillObjExpProto(pRole->GetLevel());
	if (!P_VALID(pBaseRewards))
		return;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodID);
	if (!P_VALID(pGodMiracle))
		return;

	tagGodRoleInfo* pGodRoleInfo = pGodMiracle->GetGodRoleInfo(dwRoleID);
	if (!P_VALID(pGodRoleInfo))
		return;

	DWORD dwExpReward = pBaseRewards->dwBaseRwdExp*dwExpRewardParam;
	DWORD dwSilverReward = pBaseRewards->dwBaseRwdSilver*dwSilverRewardParam;

	pGodRoleInfo->IncExp((INT)dwExpReward);
	pGodRoleInfo->IncMoney((INT)dwSilverReward);

	pRole->GetCurMgr().IncBagSilver(dwSilverReward, ELCID_ActiveTimeRewards);
	pRole->ExpChange((INT64)dwExpReward);

	tagNS_GetBattleReward send;
	send.dwExp = dwExpReward;
	send.dwPointOrTime = dwTimeOrPoint;
	send.dwSilver = dwSilverReward;
	send.eRewardType = eRewardType;
	pRole->SendMessage(&send, send.dwSize);
}

CGodMiracle::CGodMiracle():
m_dwCapGodMiracle(0),
m_dwAttackGuild(0),
m_dwAttackBidFund(0),
m_dwCapStartTime(0),
m_dwGodID(0),
m_eCurState(EGMS_Null),
m_dwDefendPoint(0),
m_dwCapturePoint(0),
m_PointChangeFlag(FALSE),
m_bGodMiracleOpen(FALSE)
{
	Init();
}

CGodMiracle::~CGodMiracle()
{
	Uninit();
}

BOOL CGodMiracle::Init()
{
	//m_dwGodID =0;
	m_dwCapGodMiracle	= 0;
	m_dwAttackGuild		= 0;
	m_dwAttackBidFund = 0;
	m_dwCapStartTime	= 0;
	m_dwDefendPoint		= 0;
	m_dwCapturePoint	= 0;
	m_PointChangeFlag   = 0;
	m_mapDefendGodRoleInfo.clear();
	m_mapCaptureGodRoleInfo.clear();
	return TRUE;
}

void CGodMiracle::Uninit()
{
	m_dwCapGodMiracle	= 0;
	m_dwAttackGuild		= 0;
	m_dwAttackBidFund = 0;
	m_dwCapStartTime	= 0;
	m_dwDefendPoint		= 0;
	m_dwCapturePoint	= 0;
	m_PointChangeFlag   = 0;
	m_eCurState			= EGMS_Null;
	m_mapDefendGodRoleInfo.clear();
	m_mapCaptureGodRoleInfo.clear();
	AllicanceGuildMap::TMapIterator iterator = m_mapAllianceGuild.Begin();
	tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
	while (m_mapAllianceGuild.PeekNext(iterator, pAllianceGuildInfo))
	{
		if (P_VALID(pAllianceGuildInfo))
			SAFE_DEL(pAllianceGuildInfo);
	}
	m_mapAllianceGuild.Clear();
}

void CGodMiracle::Refresh()
{
	// ��18:00 ɾ������Ϊ������ܹ���ѯ
	m_mapDefendGodRoleInfo.clear();
	m_mapCaptureGodRoleInfo.clear();
	g_mapGodCacheRoleInfo.clear();
	m_dwDefendPoint		= 0;
	m_dwCapturePoint	= 0;
	m_PointChangeFlag   = 0;
}

VOID CGodMiracle::UpdateMin()
{	
	MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.begin();
	for ( ; it != m_mapDefendGodRoleInfo.end(); ++it)
	{	
		tagGodRoleInfo* pGodRole = &(*it).second;
			pGodRole->UpdateMin();
	}

	MAPROLEINFO::iterator it1 = m_mapCaptureGodRoleInfo.begin();
	for ( ; it1 != m_mapCaptureGodRoleInfo.end(); ++it1)
	{	
		tagGodRoleInfo* pGodRole = &(*it1).second;
		pGodRole->UpdateMin();
	}
}

BOOL CGodMiracle::IsNeedReward(EBattleFieldRewardType eBattleFieldRewardType, DWORD dwPointOrTime, DWORD dwLastPointOrTime, DWORD& dwExpReward, DWORD& dwSilverReward)
{
	if (eBattleFieldRewardType >= EBFRT_Max)
		return FALSE;
	INT nRewardCnt = (eBattleFieldRewardType == EBFRT_CapPointRewards) ?
		(sizeof(CAP_POINT_REWARDS)/sizeof(DWORD)):(sizeof(ACTIVE_TIME_REWARDS)/sizeof(DWORD));
	BOOL bNeedReward = FALSE;

	// ����Ƿ�Ҫ���뽱��
	INT nCnt = 0;
	while (nCnt < nRewardCnt)
	{
		if (eBattleFieldRewardType == EBFRT_CapPointRewards && CAP_POINT_REWARDS[nCnt] <= (INT32)dwPointOrTime && CAP_POINT_REWARDS[nCnt] > (INT32)dwLastPointOrTime)
		{
			bNeedReward = TRUE;
			dwExpReward = CAP_POINT_REWARDS[nCnt+1];
			dwSilverReward = CAP_POINT_REWARDS[nCnt+2];
			break;
		}
		else if (eBattleFieldRewardType == EBFRT_ActiveTimeRewards && ACTIVE_TIME_REWARDS[nCnt] <= (INT32)dwPointOrTime && ACTIVE_TIME_REWARDS[nCnt] > (INT32)dwLastPointOrTime)
		{
			bNeedReward = TRUE;
			dwExpReward = ACTIVE_TIME_REWARDS[nCnt+1];
			dwSilverReward = ACTIVE_TIME_REWARDS[nCnt+2];
			break;
		}
		nCnt += 3;
	}
	return bNeedReward;
}

VOID CGodMiracle::GivePointReward(DWORD dwDAndCFlag, DWORD dwPoint, DWORD dwLastPoint)
{
	DWORD dwExpRewardParam = 0;
	DWORD dwSilverRewardParam = 0;
	if (!IsNeedReward(EBFRT_CapPointRewards, dwPoint, dwLastPoint, dwExpRewardParam, dwSilverRewardParam))
		return;

	// �����й����Ա�ӽ���
	if ( 1 == dwDAndCFlag)
	{
		GiveGuildMemberReward(m_dwCapGodMiracle, dwExpRewardParam, dwSilverRewardParam, dwPoint);
	}
	else if ( 2 == dwDAndCFlag)
	{
		GiveGuildMemberReward(m_dwAttackGuild, dwExpRewardParam, dwSilverRewardParam, dwPoint);
	}

	// ͬ�˹���
	if (m_mapAllianceGuild.Size() <= 0)
		return;

	AllicanceGuildMap::TMapIterator iterator = m_mapAllianceGuild.Begin();
	tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
	while(m_mapAllianceGuild.PeekNext(iterator, pAllianceGuildInfo))
	{
		if (!P_VALID(pAllianceGuildInfo))
			return;
		if (pAllianceGuildInfo->eAllianceForceType == dwDAndCFlag)
			GiveGuildMemberReward(pAllianceGuildInfo->dwGuildID, dwExpRewardParam, dwSilverRewardParam, dwPoint);
	}
}

VOID CGodMiracle::GiveGuildMemberReward(DWORD dwGuildID, DWORD dwExpRewardParam, DWORD dwSilverRewardParam, DWORD dwTimeOrPoint)
{
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
		return;

	MapGuildMember& mapGuildMember = pGuild->GetGuildMemberMap();
	if (mapGuildMember.Size() <= 0)
		return;

	MapGuildMember::TMapIterator iterator = mapGuildMember.Begin();
	tagGuildMember* pGuildMember = NULL;
	while(mapGuildMember.PeekNext(iterator, pGuildMember))
	{
		if (!P_VALID(pGuildMember))
			continue;
		
		tagGodRoleInfo::GiveReward(GetGodID(), EBFRT_CapPointRewards, pGuildMember->dwRoleID, dwExpRewardParam, dwSilverRewardParam, dwTimeOrPoint);
	}
}

void  CGodMiracle::ModPoint(DWORD dwDAndCFlag, DWORD dwPoint)	
{
	// �ط�
	if ( 1 == dwDAndCFlag)
	{
		DWORD dwLastDefendPoint = m_dwDefendPoint;
		m_dwDefendPoint += dwPoint;
		GivePointReward(1, m_dwDefendPoint, dwLastDefendPoint);
	}
	// ����
	if ( 2 == dwDAndCFlag)
	{
		DWORD dwLastCapturePoint = m_dwCapturePoint;
		m_dwCapturePoint += dwPoint;
		// ����Ƿ�Ҫ���뽱��
		GivePointReward(2, m_dwCapturePoint, dwLastCapturePoint);
	}

	SendCurDAndCInfoToAllGodplayer();
}

VOID CGodMiracle::GiveMoneyBack2Guild(DWORD dwGuildID, DWORD dwMoney, EReturnMoneyReason eReturnMoneyReason)
{
	// ���������ʽ�֪ͨ������ʧ�ܵĹ���᳤
	Guild* pReturnMenoyGuild = g_guildMgr.GetGuild(dwGuildID);
	if ( P_VALID(pReturnMenoyGuild))
	{
		pReturnMenoyGuild->IncGuildFund(pReturnMenoyGuild->GetMemberMgr().GetMaster()->dwRoleID, dwMoney, ELCID_Guild_BattleField_SignUp);
		// ���б�Ĺ��ᳬ��������XXս������Ȩ�ľ��ۣ��������˳���ǰ3�����ᣬͬʱ�������������ѱ��˻ء����Ƿ�������뾺�ꣿ
		Role* pReturnRole = g_roleMgr.GetRolePtrByID(pReturnMenoyGuild->GetMemberMgr().GetMaster()->dwRoleID);
		if ( P_VALID(pReturnRole))
		{
			tagNS_InsteadInfo pSend;
			pSend.eReturnMoneyReason = eReturnMoneyReason;
			pSend.dwGodID   = EGodMiracle(GetGodID());

			pReturnRole->SendMessage(&pSend, pSend.dwSize);
		}
	}

}

DWORD CGodMiracle::CanGuildDismiss(DWORD dwGuildID)
{
	if ( GetCapGodID() == dwGuildID)
		return E_Guild_BattleField_HOLD_Limit;

	if (GetApplyID() == dwGuildID)
		return E_Guild_BattleField_ATTACK_Limit;

	DWORD dwBidFund = 0;
	if (GetFirstMiracleIdxInSignUpList(dwGuildID, dwBidFund) != GT_INVALID)
		return E_Guild_BattleField_ATTACK_SIGNUP_Limit;

	tagAllianceGuildInfo* pAllianceGuildInfo = m_mapAllianceGuild.Peek(dwGuildID);
	if (P_VALID(pAllianceGuildInfo))
	{
		if (pAllianceGuildInfo->bConfirmed)
			return E_Guild_BattleField_ALLIANCE_Limit;
		else
			return E_Guild_BattleField_ALLIANCE_SIGNUP_Limit;
	}

	return E_Success;
}

VOID CGodMiracle::SendJoinBattleMsg(Guild* pGuild, EBroadcastType eBroadcastType/* = EBT_Attack*/)
{
	if (!P_VALID(pGuild))
		return;
	
	if (eBroadcastType > EBT_End || eBroadcastType < EBT_Start)
		return;

	DWORD dwTmpNum = 0;
	if (eBroadcastType == EBT_AttackAlliance)
		dwTmpNum = 300001;
	else if (eBroadcastType == EBT_DefendAlliance)
		dwTmpNum = 300002;
	else if (eBroadcastType == EBT_Attack)
		dwTmpNum = 100056;

	DWORD dwMsgInfoID =	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	DWORD dwGodMirNPCID[EGM_MAX] = {
		5510630,   // ����ڣս��
		5510631,   // �߻�֮��ս��
		5510632,   // ����ս��
	};
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NPCTypeID, &dwGodMirNPCID[m_dwGodID]);
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pGuild);
}

VOID CGodMiracle::GiveMoneyBack2FailedGuild()
{
	DWORD dwRemainBidden = 0;

	// �ҵ�����
	for (DWORD dwCnt = 0; dwCnt < MAX_COMPETE_BID; dwCnt++)
	{
		if (m_SignUpGuildInfo[dwCnt].dwGuildID != 0 && m_SignUpGuildInfo[dwCnt].dwGuildID != GT_INVALID)
		{
			if (dwRemainBidden < m_SignUpGuildInfo[dwCnt].dwBidFund)
				dwRemainBidden = m_SignUpGuildInfo[dwCnt].dwBidFund;
		}
	}

	// ��ʼ���������Ѻ�ȷ�Ͻ�������Ĳ���
	for (DWORD dwCnt = 0; dwCnt < MAX_COMPETE_BID; dwCnt++)
	{
		if (dwRemainBidden > m_SignUpGuildInfo[dwCnt].dwBidFund)
		{
			// ����������
			GiveMoneyBack2Guild(m_SignUpGuildInfo[dwCnt].dwGuildID, m_SignUpGuildInfo[dwCnt].dwBidFund, ERMR_Confirm);
		}
		else
		{
			Guild* pAttGuild = g_guildMgr.GetGuild(m_SignUpGuildInfo[dwCnt].dwGuildID);
			if (P_VALID(pAttGuild))
			{
				// ȷ�Ͻ�������
				SetApplyID(m_SignUpGuildInfo[dwCnt].dwGuildID);
				SetApplyMoney(m_SignUpGuildInfo[dwCnt].dwBidFund);

				// ��ȡ�ý���Ȩ�Ĺ��ᷢ�㲥
				SendJoinBattleMsg(pAttGuild);
			}
		}
		m_SignUpGuildInfo[dwCnt].dwBidFund = 0;
		m_SignUpGuildInfo[dwCnt].dwGuildID = 0;
	}

	//�����񼣵ĵ�ǰ��Ϣ�����ݿ�
	SaveCapTureInfoToActive();

	// ֪ͨ���ݿ⣬��ձ�����Ϣ
	tagNDBC_ClearAttackSignUpList send;
	send.eGodID = static_cast<EGodMiracle>(m_dwGodID);
	g_dbSession.Send(&send, send.dwSize);
}

DWORD CGodMiracle::Add2AllianceGuildMap(DWORD			dwGuildID,				// ����ID
									   EGuildCapType	eAllianceForceType,		// ���ػ����
									   BOOL				bConfirmed,				// �Ƿ�ȷ����
									   BOOL				bSave2DB/* = FALSE*/)
{
	if (GetCurState() == EGMS_Closed)
	{
		return E_SystemError;
	}

	if (dwGuildID == 0)
		return E_SystemError;

	if (eAllianceForceType != EGCT_Defend && eAllianceForceType != EGCT_Capture)
		return E_SystemError;

	// ��Ҫ�ж��ǲ����Ѿ���������(һ��ս��ʱ���ڣ�һ������ͬʱֻ�ܳ�Ϊ1��ս����ĳһ����ͬ�˹���)
	if (m_mapAllianceGuild.IsExist(dwGuildID))
		return EEGB_AllianceExist;

	tagAllianceGuildInfo *pAllianceGuildInfo = new tagAllianceGuildInfo(dwGuildID, eAllianceForceType, bConfirmed);
	if (P_VALID(pAllianceGuildInfo))
	{
		m_mapAllianceGuild.Add(dwGuildID, pAllianceGuildInfo);
	}

	if (bSave2DB)
	{
		// ֪ͨ���ݿ⣬�����µı�����Ϣ
		tagNDBC_InsertIntoAllianceList send;
		send.bConfirmed = bConfirmed;
		send.dwGuildID = dwGuildID;
		send.eAllianceType = eAllianceForceType;
		send.eGodID = static_cast<EGodMiracle>(m_dwGodID);
		g_dbSession.Send(&send, send.dwSize);
	}
	
	return E_Success;
}

DWORD CGodMiracle::ConfirmAllianceGuild(DWORD dwGuildID)
{
	if (GetCurState() == EGMS_Closed)
	{
		return E_SystemError;
	}

	if (dwGuildID == 0)
		return E_SystemError;

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
		return E_SystemError;

	tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
	tagAllianceGuildInfo* pConfirmGuildInfo = m_mapAllianceGuild.Peek(dwGuildID);
	if (!P_VALID(pConfirmGuildInfo))
		return E_SystemError;

	if (pConfirmGuildInfo->bConfirmed)
		return E_SystemError;

	if (pConfirmGuildInfo->eAllianceForceType != EGCT_Defend &&	pConfirmGuildInfo->eAllianceForceType != EGCT_Capture)
		return E_SystemError;

	// �ж϶�Ӧս��ĳһ������ȷ�ϵ�ͬ�������Ƿ�����
	DWORD dwAllianceCnt = 0;
	AllicanceGuildMap::TMapIterator iterator = m_mapAllianceGuild.Begin();
	while (m_mapAllianceGuild.PeekNext(iterator, pAllianceGuildInfo))
	{
		if (P_VALID(pAllianceGuildInfo) &&
			pAllianceGuildInfo->bConfirmed == TRUE &&
			pAllianceGuildInfo->eAllianceForceType == pConfirmGuildInfo->eAllianceForceType)
		{
			dwAllianceCnt++;
		}
	}

	DWORD dwMax = 0;
	EBroadcastType eBroadcastType = EBT_Start;
	if (pConfirmGuildInfo->eAllianceForceType == EGCT_Defend)
	{
		eBroadcastType = EBT_DefendAlliance;
		dwMax = MAX_DEFANCE_ALLIANCE_GUILD;
	}
	else if (pConfirmGuildInfo->eAllianceForceType == EGCT_Capture)
	{
		eBroadcastType = EBT_AttackAlliance;
		dwMax = MAX_ATTACK_ALLIANCE_GUILD;
	}

	if (dwAllianceCnt < dwMax)
	{
		pConfirmGuildInfo->bConfirmed = TRUE;

		SendJoinBattleMsg(pGuild, eBroadcastType);

		// ֪ͨ���ݿ⣬�����µı�����Ϣ
		tagNDBC_ConfirmAlliance send;
		send.dwGuildID = dwGuildID;
		send.eGodID = static_cast<EGodMiracle>(m_dwGodID);
		g_dbSession.Send(&send, send.dwSize);
		return E_Success;
	}
	else
	{
		return EEGB_AllianceMax;
	}
}

EGuildCapType CGodMiracle::GetAllianceCapType(DWORD dwGuildID)
{
	if (dwGuildID == 0)
		return EGCT_NULL;

	tagAllianceGuildInfo* pAllianceGuildInfo = m_mapAllianceGuild.Peek(dwGuildID);
	if (P_VALID(pAllianceGuildInfo))
		return pAllianceGuildInfo->eAllianceForceType;

	return EGCT_NULL;
}

EGuildCapType CGodMiracle::GetCapType(DWORD dwGuildID, BOOL &bAlliance )
{
	if (dwGuildID == 0)
		return EGCT_NULL;

	if (dwGuildID == m_dwAttackGuild)
	{
		bAlliance = FALSE;
		return EGCT_Capture;
	}

	if (dwGuildID == m_dwCapGodMiracle)
	{
		bAlliance = FALSE;
		return EGCT_Defend;
	}

	bAlliance = TRUE;
	return GetAllianceCapType(dwGuildID);
}

VOID CGodMiracle::SendAllianceGuildMsg(DWORD dwMsgInfoID, DWORD	dwAllianceType)
{
	AllicanceGuildMap::TMapIterator iterator = m_mapAllianceGuild.Begin();
	tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
	while (m_mapAllianceGuild.PeekNext(iterator, pAllianceGuildInfo))
	{
		if (P_VALID(pAllianceGuildInfo))
		{
			if (pAllianceGuildInfo->bConfirmed &&
				pAllianceGuildInfo->eAllianceForceType == static_cast<EGuildCapType>(dwAllianceType))
			{
				Guild* pGuild = g_guildMgr.GetGuild(pAllianceGuildInfo->dwGuildID);
				if (P_VALID(pGuild))
				{
					g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pGuild);
				}
			}
		}
	}
}

VOID CGodMiracle::UnsetStateOfAllRoleInBattleField(EGuildCapType eGuildCapType, EState eState)
{
	if (eGuildCapType == EGCT_Defend)
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.begin();
		for ( ; it != m_mapDefendGodRoleInfo.end(); ++it)
		{	
			tagGodRoleInfo* pGodRole = &(*it).second;
			pGodRole->UnsetState(eState);
		}
	}
	else if (eGuildCapType == EGCT_Capture)
	{
		MAPROLEINFO::iterator it1 = m_mapCaptureGodRoleInfo.begin();
		for ( ; it1 != m_mapCaptureGodRoleInfo.end(); ++it1)
		{	
			tagGodRoleInfo* pGodRole = &(*it1).second;
			pGodRole->UnsetState(eState);
		}
	}
}

BOOL CGodMiracle::IsInAllianceGuild(DWORD dwGuildID)
{
	if (P_VALID(m_mapAllianceGuild.Peek(dwGuildID)))
	{
		return TRUE;
	}
	return FALSE;
}

VOID CGodMiracle::ClearAllianceGuildMap()
{
	AllicanceGuildMap::TMapIterator iterator = m_mapAllianceGuild.Begin();
	tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
	while (m_mapAllianceGuild.PeekNext(iterator, pAllianceGuildInfo))
	{
		if (P_VALID(pAllianceGuildInfo))
			SAFE_DEL(pAllianceGuildInfo);
	}
	m_mapAllianceGuild.Clear();

	// ֪ͨ���ݿ⣬�����µı�����Ϣ
	tagNDBC_ClearAllianceList send;
	send.eGodID = static_cast<EGodMiracle>(m_dwGodID);
	g_dbSession.Send(&send, send.dwSize);
}

VOID CGodMiracle::SendAllianceList2Client(EGodMiracle eGodID, Role* pRole)
{
	if (!P_VALID(pRole))
		return;

	DWORD dwGuildID = pRole->GetGuildID();
	if (dwGuildID != GetApplyID() && dwGuildID != GetCapGodID())
		return;

	EGuildCapType eAllianceType = (dwGuildID == GetApplyID())?EGCT_Capture:EGCT_Defend;
	TList<tagGuildInfo> GuildInfoList;
	tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
	AllicanceGuildMap::TMapIterator iterator = m_mapAllianceGuild.Begin();
	while (m_mapAllianceGuild.PeekNext(iterator,pAllianceGuildInfo))
	{
		if (P_VALID(pAllianceGuildInfo) &&
			pAllianceGuildInfo->bConfirmed == FALSE &&
			pAllianceGuildInfo->eAllianceForceType == eAllianceType)
		{
			tagGuildInfo sGuildInfo;
			Guild* pGuild = g_guildMgr.GetGuild(pAllianceGuildInfo->dwGuildID);
			if (P_VALID(pGuild))
			{
				tagGuild& sGuild = const_cast<tagGuild&>(pGuild->GetGuildAtt());
				sGuildInfo.dwGuildID = pAllianceGuildInfo->dwGuildID;
				sGuildInfo.nLevel = sGuild.byLevel;
				sGuildInfo.nNum = sGuild.n16MemberNum;
				GuildInfoList.PushBack(sGuildInfo);
			}
		}
	}

	DWORD dwGuildCnt = GuildInfoList.Size();
	if (dwGuildCnt > 0)
	{
		DWORD dwSize = sizeof(tagNS_GetAllianceList)+(dwGuildCnt - 1)*sizeof(tagGuildInfo);

		MCREATE_MSG(pSend, dwSize, NS_GetAllianceList);
		if (P_VALID(pSend))
		{
			pSend->eGodID = eGodID;
			pSend->dwGuildCnt = dwGuildCnt;
			pSend->eGuildCapType = eAllianceType;

			GuildInfoList.ResetIterator();
			for (DWORD dwCnt = 0; dwCnt < dwGuildCnt; dwCnt++)
			{
				if (GuildInfoList.PeekNext(pSend->sGuildInfo[dwCnt]) == FALSE)
					break;
			}
			
			pRole->SendMessage(pSend, pSend->dwSize);
		}
	}
	else
	{
		tagNS_GetAllianceList send;
		send.dwGuildCnt = 0;
		pRole->SendMessage(&send, send.dwSize);
	}
}

DWORD CGodMiracle::Add2SignUpGuildInfo(DWORD dwGuildID, DWORD dwBidFund)
{
	for (int nCnt = 0; nCnt < MAX_COMPETE_BID; nCnt++)
	{
		if (m_SignUpGuildInfo[nCnt].dwGuildID == 0)
		{
			m_SignUpGuildInfo[nCnt].dwGuildID = dwGuildID;
			m_SignUpGuildInfo[nCnt].dwBidFund = dwBidFund;
			break;
		}
	}
	return E_Success;
}

// ��ʱ����
VOID CGodMiracle::Update()
{	
	tagDWORDTime dwTime = GetCurrentDWORDTime();

	MAPROLEINFO::iterator itDefend = m_mapDefendGodRoleInfo.begin();
	for ( ; itDefend  != m_mapDefendGodRoleInfo.end(); ++itDefend)
	{
		tagGodRoleInfo* pRoleinfo = &(*itDefend).second;
		pRoleinfo->Update();
	}

	MAPROLEINFO::iterator itCapture = m_mapCaptureGodRoleInfo.begin();
	for ( ; itCapture != m_mapCaptureGodRoleInfo.end(); ++itCapture)
	{
		tagGodRoleInfo* pRoleinfo = &(*itCapture).second;
		pRoleinfo->Update();
	}
}

#define  MAX_APPLY_MONEY 9999999
DWORD CGodMiracle::HandleApplyCapGodMiracle(DWORD dwRoleID, DWORD& dwMoney, BOOL bAddBid)
{
	if (GetCurState() == EGMS_Closed)
	{
		return E_SystemError;
	}
	if (GetApplyID() != 0 && GetApplyID() != GT_INVALID)
	{
		return E_SystemError;
	}
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT(P_VALID(pRole));
	if ( !P_VALID(pRole))
		return E_SystemError;
	
	// ���۷Ƿ�
	if ( dwMoney > MAX_APPLY_MONEY)
		return E_SystemError;

	DWORD dwBidFund = 0;
	INT32 nIdx = GetFirstMiracleIdxInSignUpList(pRole->GetGuildID(), dwBidFund);
	
	// ����Ѿ���ǰ�������ҵ��˾��۱������������ˣ����ʱ��ͻ��ˡ����۱������İ�ťӦ���ǻҵ���
	if (bAddBid == FALSE && nIdx >= 0)
		return E_SystemError;
	
	// �������ǡ�׷�Ӿ��ۡ��������ֲ���ǰ�������ش�����
	if (bAddBid && nIdx == GT_INVALID)
		return E_SystemError;

	tagDWORDTime dwTime = GetCurrentDWORDTime();

	///	ÿ��һ0��00������22��00Ϊս�����۱���ʱ��
	DWORD dwWeekDay(8);
	dwWeekDay = WhichWeekday(dwTime);
	if (8 == dwWeekDay)
		return E_SystemError;
	else
	{
		if ( dwWeekDay == 6 || dwWeekDay == 0 || (dwWeekDay == 5 && dwTime.hour >= 22))
			return EEGB_NoApplyTime;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	ASSERT(P_VALID(pGuild));
	if (!P_VALID(pGuild))
		return E_SystemError;

	// ��û�а���ռ�죬�������룬��ֱ��ռ��
	if (0 == GetCapGodID())
		return EEGB_NoCapture;

	// ռ�����������
	if ( GetCapGodID() ==  pRole->GetGuildID() )
		return EEGB_CapGuildNoApply;

	// �����߲��ǰ���
	if ( EGMP_HuiZhang != pGuild->GetMemberMgr().GetMember(dwRoleID)->eGuildPos)
		return 	EEGB_ApplyNoGuildMaster;

	// �������㹻��Ǯ
	DWORD n32Money(0);
	n32Money = pGuild->GetGuildAtt().nFund;
	if ( n32Money < dwMoney)
		return	EEGB_NoEnoughMoney;
	
	// �������ǡ�׷�Ӿ��ۡ���������ǰ��
	if (bAddBid && nIdx != GT_INVALID && nIdx >= 0 && nIdx < MAX_COMPETE_BID)
	{
		m_SignUpGuildInfo[nIdx].dwBidFund += dwMoney;

		// ֪ͨ���ݿ⣬�����µı�����Ϣ
		tagNDBC_ModifyBidFund send;
		send.dwBidFund = m_SignUpGuildInfo[nIdx].dwBidFund;
		send.dwGuildID = m_SignUpGuildInfo[nIdx].dwGuildID;
		send.eGodID = static_cast<EGodMiracle>(m_dwGodID);
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		// �����߳��۵���ǰ����������
		if (!IsBidEnough(dwMoney))
			return EEGB_MoneyLow;

		// �黹���۵���������Ľ�Ǯ
		ResetSignUpList(pRole->GetGuildID(), dwMoney);
	}

	// ��ȥ���ɽ�Ǯ 
	pGuild->DecGuildFund(dwRoleID, dwMoney, ELCID_Guild_BattleField_SignUp);
	
	//�����񼣵ĵ�ǰ��Ϣ�����ݿ�
	SaveCapTureInfoToActive();

	// �����һᷢһ�θ��񼣵Ļ�����Ϣ
	CGodMiracleMgr::GetInst()->HandleSendAllGodInfoToPlayer(dwRoleID);	
	
	return E_Success;
}

BOOL CGodMiracle::IsBidEnough(DWORD dwMoney)
{
	for (INT nCnt = 0; nCnt < MAX_COMPETE_BID; nCnt++)
	{
		if (m_SignUpGuildInfo[nCnt].dwGuildID != 0)
		{
			// ������۸�ǰ�������۵���һ��������û��ǰ������
			if (dwMoney == m_SignUpGuildInfo[nCnt].dwBidFund)
				return FALSE;
			else 	if (dwMoney > m_SignUpGuildInfo[nCnt].dwBidFund)
				return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

INT32 CGodMiracle::GetFirstMiracleIdxInSignUpList(DWORD dwGuildID, DWORD& dwBidFund)
{
	for (INT nCnt = 0; nCnt < MAX_COMPETE_BID; nCnt++)
	{
		if (m_SignUpGuildInfo[nCnt].dwGuildID != 0)
		{
			if (dwGuildID == m_SignUpGuildInfo[nCnt].dwGuildID)
			{
				dwBidFund = m_SignUpGuildInfo[nCnt].dwBidFund;
				return nCnt;
			}
		}
	}
	return GT_INVALID;
}

VOID CGodMiracle::ResetSignUpList(DWORD dwGuildID, DWORD dwMoney)
{
	DWORD dwKickIdx = 0;
	tagSignUpGuildInfo sKickedGuild;
	tagSignUpGuildInfo sNewGuildInfo = tagSignUpGuildInfo(dwGuildID, dwMoney);
	BOOL bSignUpListNotFull = FALSE;

	for (DWORD dwCnt = 0; dwCnt < MAX_COMPETE_BID; dwCnt++)
	{
		if (m_SignUpGuildInfo[dwCnt].dwGuildID != 0)
		{
			if (dwCnt < MAX_COMPETE_BID-1)
			{
				dwKickIdx = (m_SignUpGuildInfo[dwCnt].dwBidFund < m_SignUpGuildInfo[dwCnt+1].dwBidFund)?dwCnt:(dwCnt+1);
			}
		}
		else
		{
			bSignUpListNotFull = TRUE;
			dwKickIdx = dwCnt;
			break;
		}
	}

	if (bSignUpListNotFull)
	{
		m_SignUpGuildInfo[dwKickIdx] = sNewGuildInfo;
	}
	else
	{
		sKickedGuild = m_SignUpGuildInfo[dwKickIdx];
		m_SignUpGuildInfo[dwKickIdx] = sNewGuildInfo;

		// ֪ͨ���ݿ⣬ɾ�����ߵ��ı�����Ϣ
		tagNDBC_DeleteFromAttackSignUpList send;
		send.dwGuildID = sKickedGuild.dwGuildID;
		send.eGodID = static_cast<EGodMiracle>(m_dwGodID);
		g_dbSession.Send(&send, send.dwSize);

		// ���������ʽ�֪ͨ������ʧ�ܵĹ���᳤
		GiveMoneyBack2Guild(sKickedGuild.dwGuildID, sKickedGuild.dwBidFund, ERMR_MoneyLow);
	}

	// ֪ͨ���ݿ⣬�����µı�����Ϣ
	tagNDBC_InsertIntoAttackSignUpList sendInsert;
	sendInsert.dwBidFund = sNewGuildInfo.dwBidFund;
	sendInsert.dwGuildID = sNewGuildInfo.dwGuildID;
	sendInsert.eGodID = static_cast<EGodMiracle>(m_dwGodID);
	g_dbSession.Send(&sendInsert, sendInsert.dwSize);
}

// �ϸ���ʱ�仮��
EGodMiracleState CGodMiracle::GetGodMiracleState()
{
	// ��û�а���ռ�켴Ϊδ��ռ��״̬ 
/*
		if ( 0 == GetCapGodID() )
			return EGMS_Null;
		else
		{
			tagDWORDTime dwTime = GetCurrentDWORDTime();
			// ����ʱ��������ս�����������״̬
			if ( dwTime.hour >= 20 && (dwTime.hour <= 20 && dwTime.min <= 30) && 0 != GetApplyID())
				return EGMS_Capture;
			else ( dwTime.hour >= 20 && (dwTime.hour <= 20 && dwTime.min <= 30) && 0 == GetApplyID())
				return EGMS_CapNoApply;
			else
				return EGMS_Rest;
		}*/
	

/*
	tagDWORDTime dwTime = GetCurrentDWORDTime();
	if ( dwTime.hour >= 20 && (dwTime.hour <= 20 && dwTime.min <= 30) )
	{
		if ( 0 == GetCapGodID() )
			return EGMS_Null;
		else
		{
			if ( 0 != GetApplyID())
				return EGMS_Capture;
			else
				return EGMS_CapNoApply;
		}
	}
	else
		return EGMS_Rest;

	return EGMS_Rest;*/

	return GetCurState();

}

tagGodRoleInfo* CGodMiracle::GetNextMaxKillTimesPlayer(DWORD dwDefendOrCaptureFlag)
{
	ILOG->Write(_T("GetNextMaxKillTimesPlayer() begin!\n"));

	DWORD dwRoleID(0);
	DWORD dwKillTimes(0);
	if ( 1 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.begin();
		for ( ; it != m_mapDefendGodRoleInfo.end(); ++it)
		{
			tagGodRoleInfo* pRoleInfo =&(*it).second;

			if ( dwKillTimes < pRoleInfo->GetKillTimes() && pRoleInfo->GetBeCheck() == FALSE)
			{
				dwKillTimes = pRoleInfo->GetKillTimes();
				dwRoleID    = pRoleInfo->GetRoleID();
			}
		}
		if ( 0 == dwRoleID)
			return NULL;
		else
		{
			it = m_mapDefendGodRoleInfo.find(dwRoleID);
			(*it).second.SetBeCheck(TRUE);
			return &(*it).second;
		}
	}
	else if ( 2 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapCaptureGodRoleInfo.begin();
		for ( ; it != m_mapCaptureGodRoleInfo.end(); ++it)
		{
			tagGodRoleInfo* pRoleInfo =&(*it).second;
			if ( dwKillTimes < pRoleInfo->GetKillTimes() && pRoleInfo->GetBeCheck() == FALSE)
			{
				dwKillTimes = pRoleInfo->GetKillTimes();
				dwRoleID    = pRoleInfo->GetRoleID();
			}
		}
		if ( 0 == dwRoleID)
			return NULL;
		else
		{
			it = m_mapCaptureGodRoleInfo.find(dwRoleID);
			(*it).second.SetBeCheck(TRUE);
			return &(*it).second;
		}
	}

	ILOG->Write(_T("GetNextMaxKillTimesPlayer() end!\n"));
	return NULL;
}


VOID CGodMiracle::SendCurDAndCInfoToAllGodplayer()
{
	tagNS_EnterGodArea pSend;
	pSend.dwAttackGuildID = GetApplyID();
	pSend.dwDefenceGuildID= GetCapGodID();
	pSend.nDefenceGuildMember = GetPoint(1);
	pSend.nAttackGuildMember  = GetPoint(2);

	MAPROLEINFO::iterator itDefend = m_mapDefendGodRoleInfo.begin();
	for ( ; itDefend != m_mapDefendGodRoleInfo.end(); ++itDefend)
	{
		tagGodRoleInfo* pRoleInfo =  &(*itDefend).second;
		Role* pRole = g_roleMgr.GetRolePtrByID(pRoleInfo->GetRoleID());
		if ( !P_VALID(pRole))
			continue;
		else
		{
			pRole->SendMessage(&pSend, pSend.dwSize);
		}
	}
	

	MAPROLEINFO::iterator itCapture = m_mapCaptureGodRoleInfo.begin();
	for ( ; itCapture != m_mapCaptureGodRoleInfo.end(); ++itCapture)
	{
		tagGodRoleInfo* pRoleInfo =  &(*itCapture).second;
		Role* pRole = g_roleMgr.GetRolePtrByID(pRoleInfo->GetRoleID());
		if ( !P_VALID(pRole))
			continue;
		else
		{
			pRole->SendMessage(&pSend, pSend.dwSize);
		}
	}
}

tagGodRoleInfo* CGodMiracle::GetGodRoleInfo(DWORD dwRoleID)
{
	tagGodRoleInfo* pGodRoleInfo1 = GetGodRoleInfo(1, dwRoleID);
	if (P_VALID(pGodRoleInfo1))
		return pGodRoleInfo1;
	tagGodRoleInfo* pGodRoleInfo2 = GetGodRoleInfo(2, dwRoleID);
	if (P_VALID(pGodRoleInfo2))
		return pGodRoleInfo2;
	return NULL;
}

tagGodRoleInfo* CGodMiracle::GetGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID)
{
	if (1 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator itRole;
		itRole = m_mapDefendGodRoleInfo.find(dwRoleID);
		if ( itRole != m_mapDefendGodRoleInfo.end())
			return &(*itRole).second;
		else
			return NULL;
	}
	else if ( 2 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator itRole1;
		itRole1 = m_mapCaptureGodRoleInfo.find(dwRoleID);
		if ( itRole1 != m_mapCaptureGodRoleInfo.end())
			return &(*itRole1).second;
		else
			return NULL;
	}
	return NULL;
}

BOOL CGodMiracle::PushGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT(P_VALID(pRole));
	if ( !P_VALID(pRole))
		return FALSE;

	// ֪ͨ��ҽ���,��ҽ���������
	tagNS_EnterGodArea pSend;
	pSend.dwAttackGuildID = GetApplyID();
	pSend.dwDefenceGuildID= GetCapGodID();

	tagGodRoleInfo sFixInfo;
	// �ط�
	if (1 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.find(dwRoleID);
		if ( it != m_mapDefendGodRoleInfo.end())
		{
			if ( (*it).second.bIsInGodMiracleArea == 0)
			{
				pSend.nDefenceGuildMember= GetPoint(1);
				pSend.nAttackGuildMember = GetPoint(2);
				pRole->SendMessage(&pSend, pSend.dwSize);
			}
			
			return TRUE;
		}

		stdext::hash_map<DWORD,tagGodRoleInfo>::iterator itLive = g_mapGodCacheRoleInfo.find(dwRoleID);
		if ( itLive != g_mapGodCacheRoleInfo.end() )
		{
			m_mapDefendGodRoleInfo[dwRoleID] = (*itLive).second;
			m_mapDefendGodRoleInfo[dwRoleID].SetGodID(m_dwGodID);
			g_mutexGodCacheRoleInfo.Acquire();
			g_mapGodCacheRoleInfo.erase(itLive);
			g_mutexGodCacheRoleInfo.Release();

			sFixInfo = (*itLive).second;
		}
		else
		{
			tagGodRoleInfo	tmpRoleInfo;
			tmpRoleInfo.SetRoleID(dwRoleID);
			tmpRoleInfo.SetGodID(m_dwGodID);
			m_mapDefendGodRoleInfo[dwRoleID] = tmpRoleInfo;

			sFixInfo = tmpRoleInfo;
		}

		pSend.nDefenceGuildMember= GetPoint(1);
		pSend.nAttackGuildMember = GetPoint(2);
		pRole->SendMessage(&pSend, pSend.dwSize);
		SendCurDAndCInfoToAllGodplayer();
		
		sFixInfo.SendChangeActiveTimeAndKillTime();
		return TRUE;
	}
	// ����
	else if ( 2 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapCaptureGodRoleInfo.find(dwRoleID);
		if ( it != m_mapCaptureGodRoleInfo.end())
		{
			if ( (*it).second.bIsInGodMiracleArea == 0)
			{
				pSend.nDefenceGuildMember= GetPoint(1);
				pSend.nAttackGuildMember = GetPoint(2);
				pRole->SendMessage(&pSend, pSend.dwSize);
			}
			return TRUE;
		}
		stdext::hash_map<DWORD,tagGodRoleInfo>::iterator itLive = g_mapGodCacheRoleInfo.find(dwRoleID);
		if ( itLive != g_mapGodCacheRoleInfo.end())
		{
			m_mapCaptureGodRoleInfo[dwRoleID] = (*itLive).second;
			m_mapCaptureGodRoleInfo[dwRoleID].SetGodID(m_dwGodID);
			g_mutexGodCacheRoleInfo.Acquire();
			g_mapGodCacheRoleInfo.erase(itLive);
			g_mutexGodCacheRoleInfo.Release();
			sFixInfo = (*itLive).second;
		}
		else
		{
			tagGodRoleInfo	tmpRoleInfo;
			tmpRoleInfo.SetRoleID(dwRoleID);
			tmpRoleInfo.SetGodID(m_dwGodID);
			m_mapCaptureGodRoleInfo[dwRoleID] = tmpRoleInfo;

			sFixInfo = tmpRoleInfo;
		}
		
		// ֪ͨ��ҽ���,��ҽ���������
		pSend.nDefenceGuildMember= GetPoint(1);
		pSend.nAttackGuildMember = GetPoint(2);
		pRole->SendMessage(&pSend, pSend.dwSize);
		SendCurDAndCInfoToAllGodplayer();

		sFixInfo.SendChangeActiveTimeAndKillTime();
		return TRUE;
	}
	
	return FALSE;
}

BOOL CGodMiracle::RemoveGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID)
{
	// ��ע�⣬������������ս����ʱ�����������ڲſɻ�����յİ��ɹ�ѫ�Ͱ��ɹ��׽��������������İ��ɻ��ʤ�������������3���Ľ�����

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT(P_VALID(pRole));
	if ( !P_VALID(pRole))
		return FALSE;



	if ( 1 == dwDefendOrCaptureFlag )
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.find(dwRoleID);
		if ( it == m_mapDefendGodRoleInfo.end())
			return FALSE;
		g_mutexGodCacheRoleInfo.Acquire();
		g_mapGodCacheRoleInfo[dwRoleID]= (*it).second;
		g_mutexGodCacheRoleInfo.Release();
		m_mapDefendGodRoleInfo.erase(dwRoleID);

	}
	else if ( 2 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapCaptureGodRoleInfo.find(dwRoleID);
		if ( it == m_mapCaptureGodRoleInfo.end())
			return FALSE;
		
		g_mutexGodCacheRoleInfo.Acquire();
		g_mapGodCacheRoleInfo[dwRoleID]= (*it).second;
		g_mutexGodCacheRoleInfo.Release();
		m_mapCaptureGodRoleInfo.erase(it);
	}
	
	if (pRole->IsInRoleState(ERS_PK))
		pRole->UnsetRoleState(ERS_PK);
	if (pRole->IsInRoleState(ERS_PKEX))
		pRole->UnsetRoleState(ERS_PKEX);

		tagNS_LeaveGodArea pSend;
	// ֪ͨ�ͻ���,����뿪������
	pRole->SendMessage(&pSend, pSend.dwSize);



/*
	// �ط�
	if (1 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.find(dwRoleID);
		if ( it == m_mapDefendGodRoleInfo.end())
			return TRUE;
		
		// ֪ͨ�ͻ���,����뿪������
		pRole->SendMessage(&pSend, pSend.dwSize);

		m_mapDefendGodRoleInfo.erase(it);
		return TRUE;
	}
	// ����
	else if ( 2 == dwDefendOrCaptureFlag)
	{
		MAPROLEINFO::iterator it = m_mapCaptureGodRoleInfo.find(dwRoleID);
		if ( it == m_mapCaptureGodRoleInfo.end())
			return TRUE;

		// ֪ͨ�ͻ���,����뿪������
		pRole->SendMessage(&pSend, pSend.dwSize);

		m_mapDefendGodRoleInfo.erase(it);
		return TRUE;
	}*/


	return FALSE;
}
#if 0
BOOL CGodMiracle::CalDefendContributionAndExploit(DWORD dwWinOrLoseType)
{
	ASSERT( dwWinOrLoseType == 1 || dwWinOrLoseType == 2);

	Guild* pGuild = g_guildMgr.GetGuild(m_dwCapGodMiracle);
	ASSERT(P_VALID(pGuild));
	if ( !P_VALID(pGuild))
		return FALSE;

	// Ӯ
	if ( 1 == dwWinOrLoseType)
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.begin();
		for ( ; it != m_mapDefendGodRoleInfo.end(); ++it)
		{
			tagGodRoleInfo* pGodRole = &((*it).second);
			Role* pRole = g_roleMgr.GetRolePtrByID(pGodRole->GetRoleID());
			if ( !P_VALID(pRole))
				continue;

			BOOL bRet(FALSE);
			bRet = pGuild->GetMemberMgr().IsExist(pGodRole->GetRoleID());
			if ( FALSE == bRet)
				continue;

			pGuild->GetMemberMgr().IncMemberContribution(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes())/2);
			pGuild->GetMemberMgr().SetMemberExploit(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes())*3);
		}
	}
	else // ��
	{
		MAPROLEINFO::iterator it = m_mapDefendGodRoleInfo.begin();
		for ( ; it != m_mapDefendGodRoleInfo.end(); ++it)
		{
			tagGodRoleInfo* pGodRole = &((*it).second);
			Role* pRole = g_roleMgr.GetRolePtrByID(pGodRole->GetRoleID());
			if ( !P_VALID(pRole))
				continue;

			BOOL bRet(FALSE);
			bRet = pGuild->GetMemberMgr().IsExist(pGodRole->GetRoleID());
			if ( FALSE == bRet)
				continue;

			pGuild->GetMemberMgr().IncMemberContribution(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes()/3));
			pGuild->GetMemberMgr().SetMemberExploit(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes()));
		}
	}
	return TRUE;
}

BOOL CGodMiracle::CalCaptureContributionAndExploit(DWORD dwWinOrLoseType)
{
	ASSERT( dwWinOrLoseType == 1 || dwWinOrLoseType == 2);

	Guild* pGuild = g_guildMgr.GetGuild(m_dwAttackGuild);
	ASSERT(P_VALID(pGuild));
	if ( !P_VALID(pGuild))
		return FALSE;

	// Ӯ
	if ( 1 == dwWinOrLoseType)
	{
		MAPROLEINFO::iterator it = m_mapCaptureGodRoleInfo.begin();
		for ( ; it != m_mapCaptureGodRoleInfo.end(); ++it)
		{
			tagGodRoleInfo* pGodRole = &((*it).second);
			Role* pRole = g_roleMgr.GetRolePtrByID(pGodRole->GetRoleID());
			if ( !P_VALID(pRole))
				continue;

			BOOL bRet(FALSE);
			bRet = pGuild->GetMemberMgr().IsExist(pGodRole->GetRoleID());
			if ( FALSE == bRet)
				continue;

			pGuild->GetMemberMgr().IncMemberContribution(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes())/2);
			pGuild->GetMemberMgr().SetMemberExploit(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes())*3);
		}
	}
	else // ��
	{
		MAPROLEINFO::iterator it = m_mapCaptureGodRoleInfo.begin();
		for ( ; it != m_mapCaptureGodRoleInfo.end(); ++it)
		{
			tagGodRoleInfo* pGodRole = &((*it).second);
			Role* pRole = g_roleMgr.GetRolePtrByID(pGodRole->GetRoleID());
			if ( !P_VALID(pRole))
				continue;

			BOOL bRet(FALSE);
			bRet = pGuild->GetMemberMgr().IsExist(pGodRole->GetRoleID());
			if ( FALSE == bRet)
				continue;

			pGuild->GetMemberMgr().IncMemberContribution(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes()/3));
			pGuild->GetMemberMgr().SetMemberExploit(pGodRole->GetRoleID(), (pGodRole->GetActiveTime()*2 + pGodRole->GetKillTimes()));
		}
	}
	return TRUE;
}
#endif

// ��ȫ�ֻ������,�õ���Ҫ��������
BOOL CGodMiracle::GetAllCachePlayer()
{
	ILOG->Write(_T("God: %d, GetAllCachePlayer begin!\r\n"), GetGodID());

	int  nIndex(0);
	stdext::hash_map<DWORD,tagGodRoleInfo>::iterator it = g_mapGodCacheRoleInfo.begin();
	for ( ; it != g_mapGodCacheRoleInfo.end(); ++it)
	{
		tagGodRoleInfo* pRoleInfo = &(*it).second;
		Role* pRole = g_roleMgr.GetRolePtrByID(pRoleInfo->GetRoleID());
		if ( !P_VALID(pRole))
			continue;

		if ( pRoleInfo->GetGodID() == GetGodID() )
		{
			if ( GetCapGodID() == pRole->GetGuildID()) // �ط�
			{
				g_mutexGodCacheRoleInfo.Acquire();
				m_mapDefendGodRoleInfo[pRoleInfo->GetRoleID()] = (*it).second;	
				g_mutexGodCacheRoleInfo.Release();
			}
			else if ( GetApplyID() == pRole->GetGuildID()) // ����
			{
				g_mutexGodCacheRoleInfo.Acquire();
				m_mapCaptureGodRoleInfo[pRoleInfo->GetRoleID()] = (*it).second;
				g_mutexGodCacheRoleInfo.Release();
			}
		}
	}
	ILOG->Write(_T("God: %d, GetAllCachePlayer end!\r\n"), GetGodID());
	return TRUE;
}

// �������������ҵĹ��׺͹�ѫ(1 = ʤ�� 2--��)
VOID CGodMiracle::CalAllRoleContributionAndExploit(DWORD dwSuccesOrFailedType)
{
	ASSERT( dwSuccesOrFailedType == 1 || dwSuccesOrFailedType == 2);
	
	GetAllCachePlayer();

	// �ط�Ӯ
	if ( 1 == dwSuccesOrFailedType)
	{
		// �㲥 
		BroadcastGodCaptureResultToAllPlayer(dwSuccesOrFailedType);

		SetApplyID(0);
		SetApplyMoney(0);
		ILOG->Write(_T("SetApplyID(0)|SetApplyMoney(0)\r\n"));

		SaveCapTureInfoToActive();
		ILOG->Write(_T("Save ScriptData To DB!\r\n"));

	}
	else if (2 == dwSuccesOrFailedType )// �ط���
	{
		// �㲥 
		BroadcastGodCaptureResultToAllPlayer(dwSuccesOrFailedType);

		// ��������Ȩ��
		SetCapGodID(m_dwAttackGuild);
		SetCapStartTime(GetCurrentDWORDTime());
		ILOG->Write(_T("SetCapGodID(m_dwApplyGuild)|SetCapStartTime(GetCurrentDWORDTime()):	%d\r\n"), m_dwAttackGuild);

		SetApplyID(0);
		SetApplyMoney(0);
		ILOG->Write(_T("SetApplyID(0)|SetApplyMoney(0))\r\n"));

		// ���������Ϣ�����ScriptData
		SaveCapTureInfoToActive();
		ILOG->Write(_T("Save ScriptData To DB!\r\n"));

	}

	// ��¼ռ�칫����Ϣ
	LogBattleField();

	// ������ںϷ������ݣ���ʱ������ˣ��������������
	CActivity* pActivity = g_activityMgr.GetActivity(GUILBATTLE_ACTIVITY_ID);
	if (P_VALID(pActivity) && pActivity->IsHaveMergedData())
	{
		pActivity->ResetIsHaveMergedData();
		tagNDBC_ClearMergedScriptData send;
		g_dbSession.Send(&send, send.dwSize);

		if (P_VALID(CGodMiracleMgr::GetInst()))
			CGodMiracleMgr::GetInst()->ResetMergedData();
	}

	ClearAllianceGuildMap();
// 	Refresh();
	// �������
	m_dwDefendPoint	= 0;
	m_dwCapturePoint	= 0;
}

VOID CGodMiracle::LogBattleField()
{
	DWORD dwCapGuildID = GetCapGodID();
	Guild* pGuild = g_guildMgr.GetGuild(dwCapGuildID);
	if (!P_VALID(pGuild))
		return;
	if (GetGodID() >= EGM_MAX)
		return;
	tagNDBC_LogBattlefield send;
	send.sLogBattlefield.dwGuildID = dwCapGuildID;
	memcpy_s(send.sLogBattlefield.szGuildName, sizeof(send.sLogBattlefield.szGuildName), pGuild->GetGuildAtt().strName.c_str(), sizeof(send.sLogBattlefield.szGuildName));
	send.sLogBattlefield.byBattlefield = (BYTE)GetGodID();
	g_dbSession.Send(&send, send.dwSize);
}

VOID CGodMiracle::SaveCapTureInfoToActive()
{
	CActivity* pActive = g_activityMgr.GetActivity(GUILBATTLE_ACTIVITY_ID);
	ASSERT(P_VALID(pActive));
	if ( !P_VALID(pActive))
	{
		IMSG(_T("SaveCapTureInfoToActive....Failed!\r\n"));
		return ;
	}

	if ( 0  == GetGodID())
	{
		pActive->SetScriptData(0, GetCapGodID());
		pActive->SetScriptData(1, GetCapStartTime());
		pActive->SetScriptData(10, GetApplyID());
		pActive->SetScriptData(11, GetApplyMoney());
	}
	else if ( 1  == GetGodID())
	{
		pActive->SetScriptData(2, GetCapGodID());
		pActive->SetScriptData(3, GetCapStartTime());
		pActive->SetScriptData(12, GetApplyID());
		pActive->SetScriptData(13, GetApplyMoney());
	}
	else if ( 2  == GetGodID())
	{
		pActive->SetScriptData(4, GetCapGodID());
		pActive->SetScriptData(5, GetCapStartTime());
		pActive->SetScriptData(14, GetApplyID());
		pActive->SetScriptData(15, GetApplyMoney());
	}

	
	// Ҫ���������ݿ�
	pActive->SaveActivityData2DB();
}

// ��ʤ�����㲥ÿλ��ս����Ϣ
VOID CGodMiracle::BroadcastGodCaptureResultToAllPlayer(DWORD dwWinOrLoseType)
{
	ILOG->Write(_T("BroadcastGodCaptureResultToAllPlayer begin!\r\n"));

	tagGodRoleInfo* pDefendRoleInfo;
	tagGodRoleInfo* pCaptureRoleInfo;
	
	tagNS_GodReward pSendDefend;
	tagNS_GodReward pSendCapture;

	// ��ÿ����ҷ��͹ر���սʵ��
	tagNS_LeaveGodArea pSend;

	BOOL bStopFlagD(FALSE);
	BOOL bStopFlagC(FALSE);

	for ( int i = 0; i < 5; i++)
	{
		if(bStopFlagD == FALSE)
		{
			pDefendRoleInfo = GetNextMaxKillTimesPlayer(1); 
			if ( P_VALID(pDefendRoleInfo))
			{
				pSendDefend.sRoleIndo[i].dwRoleID = pDefendRoleInfo->GetRoleID();
				pSendDefend.sRoleIndo[i].nKillTime= pDefendRoleInfo->GetKillTimes();
			}
			else
				bStopFlagD = TRUE;
		}
		
		if(bStopFlagC == FALSE)
		{
			pCaptureRoleInfo = GetNextMaxKillTimesPlayer(2);
			if ( P_VALID(pCaptureRoleInfo))
			{
				pSendCapture.sRoleIndo[i].dwRoleID = pCaptureRoleInfo->GetRoleID();
				pSendCapture.sRoleIndo[i].nKillTime= pCaptureRoleInfo->GetKillTimes();
			}
			else
				bStopFlagC = TRUE;
		}
	}

		// �ط���Ӯȡ����
		MAPROLEINFO::iterator itDefend = m_mapDefendGodRoleInfo.begin();
		for ( ; itDefend != m_mapDefendGodRoleInfo.end(); ++itDefend)
		{		
			tagGodRoleInfo* pRoleInfo = &(*itDefend).second;

			Role* pRole = g_roleMgr.GetRolePtrByID(pRoleInfo->GetRoleID());
			if ( !P_VALID(pRole))
				continue;
			
			//  �رպ���
			pRole->UnsetRoleState(ERS_PK);
			pRole->UnsetRoleState(ERS_PKEX);

			DWORD dwFinialPoints = CAL_FINIALPOINTS(pRoleInfo->GetActiveTime(), pRoleInfo->GetKillTimes());
			if (dwFinialPoints > MAX_MIRACLES_POINTS)
				dwFinialPoints = MAX_MIRACLES_POINTS;
			if (1 == dwWinOrLoseType)
			{
				pSendDefend.bWin = true;
				pSendDefend.nContribute = dwFinialPoints/10;
				pSendDefend.nExploit    = dwFinialPoints;
			}
			else if (2 == dwWinOrLoseType)
			{
				pSendDefend.bWin = false;
				pSendDefend.nContribute = dwFinialPoints/20;
				pSendDefend.nExploit    = dwFinialPoints/2;
			}
			pSendDefend.nKillTime = pRoleInfo->GetKillTimes();
			pSendDefend.dwLiveTime= pRoleInfo->GetActiveTime();
			
			///  ������һ���
			pRole->GodMiraclePoints(dwFinialPoints);

			Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
			if ( P_VALID(pGuild))
			{
				BOOL bRet(FALSE);
				bRet = pGuild->GetMemberMgr().IsExist(pRoleInfo->GetRoleID());
				if ( TRUE == bRet)
				{
					pGuild->GetMemberMgr().IncMemberContribution(pRoleInfo->GetRoleID(), pSendDefend.nContribute);
					pGuild->GetMemberMgr().SetMemberExploit(pRoleInfo->GetRoleID(), pSendDefend.nExploit);
				}
			}
			
			pRole->SendMessage(&pSendDefend, pSendDefend.dwSize);
			pRole->SendMessage(&pSend, pSend.dwSize);

			CGodMiracleMgr::GetInst()->HandleSendAllGodInfoToPlayer(pRole->GetID());

			// ս������ʱ�����е�ս��������һ���2Сʱ��˫������״̬
			const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(BATTLE_FIELD_END_BUFFID);
			if ( P_VALID(pBuffProto))
			{
				pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
			}
			// �ط�Ӯ���ж�ְλ����buff
			if (1 == dwWinOrLoseType)
			{
				AddWinnerBuff(pRole);
			}
		}
		// ���������˱���
		MAPROLEINFO::iterator itCapture = m_mapCaptureGodRoleInfo.begin();
		for ( ; itCapture != m_mapCaptureGodRoleInfo.end(); ++itCapture)
		{		
			tagGodRoleInfo* pRoleInfo = &(*itCapture).second;

			Role* pRole = g_roleMgr.GetRolePtrByID(pRoleInfo->GetRoleID());
			if ( !P_VALID(pRole))
				continue;

			pRole->UnsetRoleState(ERS_PK);
			pRole->UnsetRoleState(ERS_PKEX);

			DWORD dwFinialPoints = CAL_FINIALPOINTS(pRoleInfo->GetActiveTime(), pRoleInfo->GetKillTimes());
			if (dwFinialPoints > MAX_MIRACLES_POINTS)
				dwFinialPoints = MAX_MIRACLES_POINTS;

			if (1 == dwWinOrLoseType)
			{
				pSendCapture.bWin = false;
				pSendCapture.nContribute = dwFinialPoints/20;
				pSendCapture.nExploit    = dwFinialPoints/2;
			}
			else if (2 == dwWinOrLoseType)
			{
				pSendCapture.bWin = true;
				pSendCapture.nContribute = dwFinialPoints/10;
				pSendCapture.nExploit = dwFinialPoints;
			}

			pSendCapture.nKillTime = pRoleInfo->GetKillTimes();
			pSendCapture.dwLiveTime= pRoleInfo->GetActiveTime();
			
			///  ������һ���
			pRole->GodMiraclePoints(dwFinialPoints);

			Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
			if ( P_VALID(pGuild))
			{
				BOOL bRet(FALSE);
				bRet = pGuild->GetMemberMgr().IsExist(pRoleInfo->GetRoleID());
				if ( TRUE == bRet)
				{
					pGuild->GetMemberMgr().IncMemberContribution(pRoleInfo->GetRoleID(), pSendCapture.nContribute);
					pGuild->GetMemberMgr().SetMemberExploit(pRoleInfo->GetRoleID(), pSendCapture.nExploit);
				}
			}
			
			pRole->SendMessage(&pSendCapture, pSendCapture.dwSize);
			pRole->SendMessage(&pSend, pSend.dwSize);
			CGodMiracleMgr::GetInst()->HandleSendAllGodInfoToPlayer(pRole->GetID());

			// ս������ʱ�����е�ս��������һ���2Сʱ��˫������״̬
			const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(BATTLE_FIELD_END_BUFFID);
			if ( P_VALID(pBuffProto))
			{
				pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
			}
			// �ط�Ӯ���ж�ְλ����buff
			if (2 == dwWinOrLoseType)
			{
				AddWinnerBuff(pRole);
			}
		}
		ILOG->Write(_T("BroadcastGodCaptureResultToAllPlayer end!\r\n"));
}

void  CGodMiracle::SetCurState(EGodMiracleState CGodMiracle)
{	
	m_eCurState = m_bGodMiracleOpen?CGodMiracle:EGMS_Closed;
}

// �õ����Ļ������
tagGodRoleInfo* CGodMiracle::GetMaxScoresPlayer(EGuildCapType eGuidType)
{
	DWORD dwScores(0);
	DWORD dwRoleID(0);
	tagGodRoleInfo* pRetRoleInfo = NULL;

	if ( EGCT_Defend == eGuidType)
	{
		MAPROLEINFO::iterator itDefend = m_mapDefendGodRoleInfo.begin();
		for ( ; itDefend != m_mapDefendGodRoleInfo.end(); ++itDefend)
		{
			tagGodRoleInfo* pRoleInfo = &(*itDefend).second;
			DWORD tmpScores(0);
			tmpScores = pRoleInfo->GetActiveTime()*2 + pRoleInfo->GetKillTimes();
			if ( tmpScores > dwScores)
				pRetRoleInfo = pRoleInfo;
		}
	}
	
	if ( EGCT_Capture == eGuidType )
	{
		MAPROLEINFO::iterator itCapture = m_mapCaptureGodRoleInfo.begin();
		for ( ; itCapture != m_mapCaptureGodRoleInfo.end(); ++itCapture)
		{
			tagGodRoleInfo* pRoleInfo = &(*itCapture).second;
			DWORD tmpScores(0);

			tmpScores =  pRoleInfo->GetActiveTime()*2 + pRoleInfo->GetKillTimes();
			if ( tmpScores > dwScores)
				pRetRoleInfo = pRoleInfo;
		}
	}
	
 	return pRetRoleInfo;
}

BOOL CGodMiracle::GetSomeplayerOverFixedScores(std::vector<tagGuildMaxScoresPlayer> &lstMaxScoresPlayer)
{
	
	if ( 0 == GetCapGodID() || 0 == GetApplyID() )
		return FALSE;

// 	// �ط� 
// 	MAPROLEINFO::iterator  itDefend = m_mapDefendGodRoleInfo.begin();
// 	for ( ; itDefend != m_mapDefendGodRoleInfo.end(); ++itDefend)
// 	{
// 		tagGodRoleInfo* pRoleInfo = &(*itDefend).second;
// 		if ( (pRoleInfo ->GetActiveTime()*2 + pRoleInfo->GetKillTimes()) >= 120 )
// 		{
// 			tagGuildMaxScoresPlayer tmpScoresPlayer;
// 			tmpScoresPlayer.dwGuildID = GetCapGodID();
// 			tmpScoresPlayer.dwRoleID  = pRoleInfo->GetRoleID();
// 			tmpScoresPlayer.dwScores  = pRoleInfo ->GetActiveTime()*2 + pRoleInfo->GetKillTimes();
// 			
// 			lstMaxScoresPlayer.push_back(tmpScoresPlayer);
// 		}
// 	}
// 
// 	// ����
// 	MAPROLEINFO::iterator itCapture = m_mapCaptureGodRoleInfo.begin();
// 	for ( ; itCapture != m_mapCaptureGodRoleInfo.end(); ++itCapture)
// 	{
// 		tagGodRoleInfo* pRoleInfo = &(*itCapture).second;
// 		if ( (pRoleInfo ->GetActiveTime()*2 + pRoleInfo->GetKillTimes()) >= 120 )
// 		{
// 			tagGuildMaxScoresPlayer tmpScoresPlayer;
// 			tmpScoresPlayer.dwGuildID = GetApplyID();
// 			tmpScoresPlayer.dwRoleID  = pRoleInfo->GetRoleID();
// 			tmpScoresPlayer.dwScores  = pRoleInfo ->GetActiveTime()*2 + pRoleInfo->GetKillTimes();
// 			lstMaxScoresPlayer.push_back(tmpScoresPlayer);
// 		}
// 	}

	// ���ս����Ϣ
	Refresh();
	return TRUE;
}

// VOID CGodMiracle::CheckAllGuildMemberIsInGodArea()
// {
// 	if ( 0 == GetApplyID())
// 		return;
// 
// 	if ( 0 != GetCapGodID())
// 	{
// 		Guild *pGuild = g_guildMgr.GetGuild(GetCapGodID());
// 		if ( P_VALID(pGuild))
// 		{
// 			pGuild->GetMemberMgr().CheckAllGuildMemberIsInGodArea();
// 		}
// 	}
// 
// 	if ( 0 != GetApplyID())
// 	{
// 		Guild *pGuild = g_guildMgr.GetGuild(GetApplyID());
// 		if ( P_VALID(pGuild))
// 		{
// 			pGuild->GetMemberMgr().CheckAllGuildMemberIsInGodArea();
// 		}
// 	}
// }

BOOL bEndTimeFlag(FALSE);

// ������������г�Ա���͸�����״̬��Ϣ
VOID CGodMiracle::BroadcastAllGoodPlayer()
{
	if ( 0 == GetCapGodID())
		return;
	
		DWORD dwTmpNum = 1436;
		int   nChanal(1);
		DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ChatInfo, &nChanal);
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_MsgID, &dwTmpNum);
		g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);

	// �ط� 
	MAPROLEINFO::iterator  itDefend = m_mapDefendGodRoleInfo.begin();
	for ( ; itDefend != m_mapDefendGodRoleInfo.end(); ++itDefend)
	{
		tagGodRoleInfo* pRoleInfo = &(*itDefend).second;
		if ( (pRoleInfo ->GetActiveTime()*2 + pRoleInfo->GetKillTimes()) >= 120 )
		{
			DWORD dwTmpNum1 = 1437;
			int   nChanal(1);
			DWORD dwRoleID(0);
			dwRoleID = pRoleInfo->GetRoleID();
			Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
			if ( !P_VALID(pRole))
				continue;

			DWORD dwGuild(0);
			dwGuild = pRole->GetGuildID();
			if (!P_VALID(dwGuild))
				continue;

			DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ChatInfo, &nChanal);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_MsgID, &dwTmpNum1);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &dwGuild);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
			g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
		}
	}

	// ����
	MAPROLEINFO::iterator itCapture = m_mapCaptureGodRoleInfo.begin();
	for ( ; itCapture != m_mapCaptureGodRoleInfo.end(); ++itCapture)
	{
		tagGodRoleInfo* pRoleInfo = &(*itCapture).second;
		if ( (pRoleInfo ->GetActiveTime()*2 + pRoleInfo->GetKillTimes()) >= 120 )
		{
			tagGodRoleInfo* pRoleInfo = &(*itCapture).second;
			DWORD dwTmpNum2 = 1437;
			int nChanal(1);
			DWORD dwRoleID(0);
			dwRoleID = pRoleInfo->GetRoleID();

			Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
			if ( !P_VALID(pRole))
				continue;
			DWORD dwGuild(0);
			dwGuild = pRole->GetGuildID();
			if (!P_VALID(dwGuild))
				continue;

			DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ChatInfo, &nChanal);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_MsgID, &dwTmpNum2);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &dwGuild);
			g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
			g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
		}
	}

	// ���ս����Ϣ
	Refresh();
}	

void CGodMiracle::AddWinnerBuff( Role* pRole )
{
	if (!P_VALID(pRole))
		return;

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (P_VALID(pGuild))
	{
		tagGuildMember * pGuildMember= pGuild->GetGuildMemberMap().Peek(pRole->GetID());
		const tagBuffProto* pBuffProto = NULL;
		if (pGuildMember->eGuildPos == EGMP_HuiZhang || pGuildMember->eGuildPos == EGMP_FuHuiZhang)
			pBuffProto = g_attRes.GetBuffProto(BATTLE_FIELD_WIN_LEADER_BUFFID);
		else
			pBuffProto = g_attRes.GetBuffProto(BATTLE_FIELD_WIN_MEMBER_BUFFID);
		if ( P_VALID(pBuffProto))
			pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
	}
}
//////////////////////////////////////////////////////////////////////////
// 

CGodMiracleMgr::CGodMiracleMgr()
:m_bBidFundSignUpConfirm(FALSE),
m_bAddCapGuildFund(FALSE),
m_bHaveMergedData(FALSE)
{
	for (int i = 0; i < EGM_MAX; i++)
	{
		m_VecGodmiracle[i].Init();
		m_VecGodmiracle[i].SetGodID(i);
	}
}

CGodMiracleMgr::~CGodMiracleMgr()
{}

BOOL CGodMiracleMgr::Init()
{
	for (int i = 0; i < EGM_MAX; i++)
	{
		m_VecGodmiracle[i].Init();
		m_VecGodmiracle[i].SetGodID(i);
	}

	m_dwBattleFieldMapID[EGM_NH] = TObjRef<Util>()->Crc32(_T(TIANSHENZHONG_MAP_NAME));
	m_dwBattleFieldMapID[EGM_XY] = TObjRef<Util>()->Crc32(_T(QIHUAZHIHAI_MAP_NAME));
	m_dwBattleFieldMapID[EGM_SM] = TObjRef<Util>()->Crc32(_T(BENLEILIEGU_MAP_NAME));

	return TRUE;
}

VOID CGodMiracleMgr::LoadSignUpInfo()
{
	// �����ݿ����󾺼۱�������
	tagNDBC_LoadBattleFieldAttackSignUpInfo send1;
	g_dbSession.Send(&send1, send1.dwSize);

	// �����������ͬ�˱�������
	tagNDBC_LoadAllicanceGuildInfo send2;
	g_dbSession.Send(&send2, send2.dwSize);
}

BOOL CGodMiracleMgr::LoadGodMiracleInfoFormActivity(CActivity* pActivity)
{
	ASSERT(P_VALID(pActivity));
	if ( !P_VALID(pActivity))
	{
		IMSG(_T("LoadGodMiracleInfoFormActivity()....Failed!\r\n"));
		return FALSE;
	}

	m_VecGodmiracle[0].SetCapGodID(pActivity->GetScriptData(0));
	ILOG->Write(_T("SetCapGodID(pActivity->GetScriptData(0)):	%d\r\n"), pActivity->GetScriptData(0));
	m_VecGodmiracle[0].SetCapStartTime(pActivity->GetScriptData(1));
	m_VecGodmiracle[0].SetApplyID(pActivity->GetScriptData(10));
	m_VecGodmiracle[0].SetApplyMoney(pActivity->GetScriptData(11));
	if ( 0 == m_VecGodmiracle[0].GetCapGodID() )
		m_VecGodmiracle[0].SetCurState(EGMS_Null);
	else
		m_VecGodmiracle[0].SetCurState(EGMS_Rest);
	
	SetGodDefendAndCaptureState(0);
	
	m_VecGodmiracle[1].SetCapGodID(pActivity->GetScriptData(2));
	ILOG->Write(_T("SetCapGodID(pActivity->GetScriptData(2)):	%d\r\n"), pActivity->GetScriptData(2));
	m_VecGodmiracle[1].SetCapStartTime(pActivity->GetScriptData(3));
	m_VecGodmiracle[1].SetApplyID(pActivity->GetScriptData(12));
	m_VecGodmiracle[1].SetApplyMoney(pActivity->GetScriptData(13));
	if ( 0 == m_VecGodmiracle[1].GetCapGodID() )
		m_VecGodmiracle[1].SetCurState(EGMS_Null);
	else
		m_VecGodmiracle[1].SetCurState(EGMS_Rest);

	SetGodDefendAndCaptureState(1);

	m_VecGodmiracle[2].SetCapGodID(pActivity->GetScriptData(4));
	ILOG->Write(_T("SetCapGodID(pActivity->GetScriptData(4)):	%d\r\n"), pActivity->GetScriptData(4));
	m_VecGodmiracle[2].SetCapStartTime(pActivity->GetScriptData(5));
	m_VecGodmiracle[2].SetApplyID(pActivity->GetScriptData(14));
	m_VecGodmiracle[2].SetApplyMoney(pActivity->GetScriptData(15));
	if ( 0 == m_VecGodmiracle[2].GetCapGodID() )
		m_VecGodmiracle[2].SetCurState(EGMS_Null);
	else
		m_VecGodmiracle[2].SetCurState(EGMS_Rest);

	SetGodDefendAndCaptureState(2);

	if (pActivity->IsHaveMergedData())
	{
		for (INT nIdx = 0; nIdx < MERGED_SCRIPTDATA_MAX; nIdx++)
		{
			tagMergedScriptData MergedScriptData;
			if (pActivity->GetMergedScriptData(nIdx, MergedScriptData))
			{
				if (MergedScriptData.bHaveData)
				{
					DWORD dwCapGuildID1 = MergedScriptData.m_MergedScriptData.GetScriptData(0);
					if (dwCapGuildID1 != 0 && dwCapGuildID1 != GT_INVALID)
						m_listMergedCapGuildID.PushBack(dwCapGuildID1);
					DWORD dwCapGuildID2 = MergedScriptData.m_MergedScriptData.GetScriptData(2);
					if (dwCapGuildID2 != 0 && dwCapGuildID2 != GT_INVALID)
						m_listMergedCapGuildID.PushBack(dwCapGuildID2);
					DWORD dwCapGuildID3 = MergedScriptData.m_MergedScriptData.GetScriptData(4);
					if (dwCapGuildID3 != 0 && dwCapGuildID3 != GT_INVALID)
						m_listMergedCapGuildID.PushBack(dwCapGuildID3);
				}
			}
		}
		m_bHaveMergedData = TRUE;
	}

	return TRUE;
}

CGodMiracleMgr	g_GodMiracleMgr;
CGodMiracleMgr* CGodMiracleMgr::GetInst()
{
	// static CGodMiracleMgr I; 
	 return &g_GodMiracleMgr;
}

CGodMiracle* CGodMiracleMgr::GetGodMiracle(EGodMiracle eGodType)
{
	ASSERT(eGodType >= EGM_NH && eGodType < EGM_MAX);
	if ( eGodType < EGM_NH || eGodType >= EGM_MAX)
		return NULL;

	return &m_VecGodmiracle[eGodType];
}

DWORD CGodMiracleMgr::HandleSendAllGodInfoToPlayer(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT(P_VALID(pRole));
	if ( !P_VALID(pRole))
		return E_SystemError;
	
	INT nMsgSize = sizeof(tagNS_GetAllGodInfo) + (EGM_MAX - 1) * sizeof(tagGodInfo);
	MCREATE_MSG(pSend, nMsgSize, NS_GetAllGodInfo);

	for (int i = 0; i< EGM_MAX; i++)
	{	
		pSend->sTagInfo[i].eGodType = (EGodMiracle)i;
		pSend->sTagInfo[i].eGodState = m_VecGodmiracle[i].GetGodMiracleState();
		
		Guild* pGuildDefend = g_guildMgr.GetGuild(m_VecGodmiracle[i].GetCapGodID());
		if (P_VALID( pGuildDefend))
		{
			pSend->sTagInfo[i].sDefenceGuild.dwGuildID = m_VecGodmiracle[i].GetCapGodID();
			pSend->sTagInfo[i].sDefenceGuild.nLevel    = pGuildDefend->GetGuildAtt().byLevel;
			pSend->sTagInfo[i].sDefenceGuild.nNum      = pGuildDefend->GetMemberMgr().GetNumber(); 
			pSend->sTagInfo[i].dwStarttime			   = m_VecGodmiracle[i].GetCapStartTime();
		}
			
		
		Guild* pGuildCapture = g_guildMgr.GetGuild(m_VecGodmiracle[i].GetApplyID());
		if (P_VALID(pGuildCapture))
		{
			pSend->sTagInfo[i].sCapGuild.dwGuildID = m_VecGodmiracle[i].GetApplyID();
			pSend->sTagInfo[i].sCapGuild.nLevel    = pGuildCapture->GetGuildAtt().byLevel;
			pSend->sTagInfo[i].sCapGuild.nNum      = pGuildCapture->GetMemberMgr().GetNumber();
			pSend->sTagInfo[i].dwApplyMoney = m_VecGodmiracle[i].GetApplyMoney();
		}

		DWORD dwBidFund = 0;
		DWORD dwGuildID = pRole->GetGuildID();
		INT32 nIdx = m_VecGodmiracle[i].GetFirstMiracleIdxInSignUpList(dwGuildID, dwBidFund);
		if (nIdx != GT_INVALID)
			pSend->sTagInfo[i].dwBidFund = dwBidFund;
		else
			pSend->sTagInfo[i].dwBidFund = 0;

		CGodMiracle::AllicanceGuildMap& AllianceGuildmap = m_VecGodmiracle[i].GetAllianceGuildMap();
		CGodMiracle::AllicanceGuildMap::TMapIterator iterator = AllianceGuildmap.Begin();
		tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
		INT nAttIdx = 0, nDefIdx = 0;
		while (AllianceGuildmap.PeekNext(iterator, pAllianceGuildInfo))
		{
			if (P_VALID(pAllianceGuildInfo))
			{
				// ����
				if (pAllianceGuildInfo->eAllianceForceType == EGCT_Defend)
				{
					if (pAllianceGuildInfo->bConfirmed == TRUE && nDefIdx < MAX_DEFANCE_ALLIANCE_GUILD)
					{
						pSend->sTagInfo[i].dwDefAlliance[nDefIdx] = pAllianceGuildInfo->dwGuildID;
						nDefIdx++;
					}
				}
				// ����
				else if (pAllianceGuildInfo->eAllianceForceType == EGCT_Capture)
				{
					if (pAllianceGuildInfo->bConfirmed == TRUE && nAttIdx < MAX_ATTACK_ALLIANCE_GUILD)
					{
						pSend->sTagInfo[i].dwAttAlliance[nAttIdx] = pAllianceGuildInfo->dwGuildID;
						nAttIdx++;
					}
				}
			}
		}
	}

	pSend->byGodNum = EGM_MAX;
	pSend->dwSize = sizeof(tagNS_GetAllGodInfo) + (sizeof(tagGodInfo))*(pSend->byGodNum-1);
	
	pRole->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);

	return E_Success;
}

// ������������г�Ա���͸�����״̬��Ϣ
DWORD CGodMiracleMgr::UpdateAllGodInfoToAllPlayer()
{
	INT nMsgSize = sizeof(tagNS_GetAllGodInfo) + (EGM_MAX - 1) * sizeof(tagGodInfo);
	MCREATE_MSG(pSend, nMsgSize, NS_GetAllGodInfo);
	if (!P_VALID(pSend))
		return GT_INVALID;

	// ͬ�����ͻ��˷�����ʱ��
// 	tagNS_SynchronismTime sendClient;
// 	sendClient.dwTime = GetCurrentDWORDTime();
	//SendMessage(&sendClient, sendClient.dwSize);

	for (int i = 0; i< EGM_MAX; i++)
	{	
		pSend->sTagInfo[i].eGodType = (EGodMiracle)i;
		pSend->sTagInfo[i].eGodState = m_VecGodmiracle[i].GetGodMiracleState();

		Guild* pGuildDefend = g_guildMgr.GetGuild(m_VecGodmiracle[i].GetCapGodID());
		if (P_VALID( pGuildDefend))
		{
			pSend->sTagInfo[i].sDefenceGuild.dwGuildID = m_VecGodmiracle[i].GetCapGodID();
			pSend->sTagInfo[i].sDefenceGuild.nLevel    = pGuildDefend->GetGuildAtt().byLevel;
			pSend->sTagInfo[i].sDefenceGuild.nNum      = pGuildDefend->GetMemberMgr().GetNumber(); 
			pSend->sTagInfo[i].dwStarttime			   = m_VecGodmiracle[i].GetCapStartTime();
		}


		Guild* pGuildCapture = g_guildMgr.GetGuild(m_VecGodmiracle[i].GetApplyID());
		if (P_VALID(pGuildCapture))
		{
			pSend->sTagInfo[i].sCapGuild.dwGuildID = m_VecGodmiracle[i].GetApplyID();
			pSend->sTagInfo[i].sCapGuild.nLevel    = pGuildCapture->GetGuildAtt().byLevel;
			pSend->sTagInfo[i].sCapGuild.nNum      = pGuildCapture->GetMemberMgr().GetNumber();
			pSend->sTagInfo[i].dwApplyMoney = m_VecGodmiracle[i].GetApplyMoney();
		}
		CGodMiracle::AllicanceGuildMap& AllianceGuildmap = m_VecGodmiracle[i].GetAllianceGuildMap();
		CGodMiracle::AllicanceGuildMap::TMapIterator iterator = AllianceGuildmap.Begin();
		tagAllianceGuildInfo* pAllianceGuildInfo = NULL;
		INT nAttIdx = 0, nDefIdx = 0;
		while (AllianceGuildmap.PeekNext(iterator, pAllianceGuildInfo))
		{
			if (P_VALID(pAllianceGuildInfo))
			{
				// ����
				if (pAllianceGuildInfo->eAllianceForceType == EGCT_Defend)
				{
					if (pAllianceGuildInfo->bConfirmed == TRUE && nDefIdx < MAX_DEFANCE_ALLIANCE_GUILD)
					{
						pSend->sTagInfo[i].dwDefAlliance[nDefIdx] = pAllianceGuildInfo->dwGuildID;
						nDefIdx++;
					}
				}
				// ����
				else if (pAllianceGuildInfo->eAllianceForceType == EGCT_Capture)
				{
					if (pAllianceGuildInfo->bConfirmed == TRUE && nAttIdx < MAX_ATTACK_ALLIANCE_GUILD)
					{
						pSend->sTagInfo[i].dwAttAlliance[nAttIdx] = pAllianceGuildInfo->dwGuildID;
						nAttIdx++;
					}
				}
			}
		}

	}

	pSend->byGodNum = EGM_MAX;
	pSend->dwSize = sizeof(tagNS_GetAllGodInfo) + (sizeof(tagGodInfo))*(pSend->byGodNum-1);

	TMap<DWORD, Guild*>& mapGuild = g_guildMgr.GetMapGuild();
	Guild *pGuild = NULL;
	TMap<DWORD, Guild*>::TMapIterator iter = mapGuild.Begin();
	while(mapGuild.PeekNext(iter, pGuild))
	{
		if (!P_VALID(pGuild))
			continue;
		DWORD dwGuildID = pGuild->GetGuildAtt().dwID;
		for (INT nCnt = 0; nCnt < EGM_MAX; nCnt++)
		{
			DWORD dwBidFund = 0;
			INT32 nIdx = m_VecGodmiracle[nCnt].GetFirstMiracleIdxInSignUpList(dwGuildID, dwBidFund);
			if (nIdx != GT_INVALID)
				pSend->sTagInfo[nCnt].dwBidFund = dwBidFund;
			else
				pSend->sTagInfo[nCnt].dwBidFund = 0;
		}
		pGuild->GetMemberMgr().SendGuildMsg(pSend, pSend->dwSize);
	}

// 	g_guildMgr.SendMsgToAllGuild(pSend, pSend->dwSize);
	return E_Success;
}

struct tagReviePoit 
{
	float fX;
	float fY;
	float fZ;
};

tagReviePoit g_GodMiracleRevieDefend[EGM_MAX] = 
{
	{9021, 5940, 45269},			// ����ڣ
	{45288, 2929, 62315},			// �߻�֮��
	{18334, 13601, 62031},		// �����ѹ�
};



tagReviePoit g_GodMiracleRevieCapture[EGM_MAX] = 
{
	{43058, 5170, 10603},			// ����ڣ
	{20664, 3149, 23297},			// �߻�֮��
	{24361, 14451, 23517},		// �����ѹ�
};

// ���Ӹ����ɫ
BOOL CGodMiracleMgr::AddRevieRole(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	ASSERT(P_VALID(pRole));
	if ( !P_VALID(pRole))
		return FALSE;
	
	if ( m_mapRevieRole.IsExist(dwRoleID))
		return FALSE;

	tagRevieRoleInfo* pRevieRole = new tagRevieRoleInfo;
	pRevieRole->dwRoleID = dwRoleID;
	tagRevieRoleInfo* tmpRevieRole = m_mapRevieRole.Peek(dwRoleID);
	if(!P_VALID(tmpRevieRole))
	{
		
		m_mutexRevive.Acquire();
		m_mapRevieRole.Add(dwRoleID, pRevieRole);
		m_mutexRevive.Release();
	}
	else
		delete pRevieRole;
	return TRUE;
}

// ����
VOID CGodMiracleMgr::ReviveRole(DWORD dwRoleID)
{
	// ͨ���õ���ɫ�ĵ�ͼ�õ��񼣵ĵ�ͼ�Ӷ��õ������
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))		
		return;
	
	DWORD dwGuildID(0);
	dwGuildID = pRole->GetGuildID();
	if ( 0 == dwGuildID)
		return ;

	Map* pMap = pRole->GetMap();
	ASSERT(P_VALID(pMap));
	if (!P_VALID(pMap))
		return;
	

	DWORD dwMapID = pRole->GetMapID();
	EGodMiracle eGod = GetEGodMiracle(dwMapID);
	if ( eGod >= EGM_MAX || eGod < EGM_NH)
		return;
	
	tagNS_RoleRevive send;
	send.dwRoleID		= dwRoleID;
	if( !pRole->IsInState(ES_Dead) )
		send.dwErrorCode = E_Revive_NotDead;

	BOOL bAlliance = FALSE;
	// �ط�
	if ( m_VecGodmiracle[eGod].GetCapType(pRole->GetGuildID(),bAlliance) == EGCT_Defend || m_VecGodmiracle[eGod].GetCapType(pRole->GetGuildID(),bAlliance) == EGCT_NULL)
	{
		pRole->GetRevive().fX		=	g_GodMiracleRevieDefend[eGod].fX;
		pRole->GetRevive().fY		=	g_GodMiracleRevieDefend[eGod].fY;
		pRole->GetRevive().fZ		=	g_GodMiracleRevieDefend[eGod].fZ;
	}
	// ����
	else if ( m_VecGodmiracle[eGod].GetCapType(pRole->GetGuildID(),bAlliance) ==  EGCT_Capture)
	{
		pRole->GetRevive().fX		=	g_GodMiracleRevieCapture[eGod].fX;
		pRole->GetRevive().fY		=	g_GodMiracleRevieCapture[eGod].fY;
		pRole->GetRevive().fZ		=	g_GodMiracleRevieCapture[eGod].fZ;
	}


	// ���ø���㼰Ѫ����������
 	pRole->GetRevive().nHP		=	pRole->GetAttValue(ERA_MaxHP);
 	pRole->GetRevive().nMP		=	pRole->GetAttValue(ERA_MaxMP);
 	pRole->GetRevive().dwMapID	=	pRole->GetMapID();
	
	pRole->GetMap()->OnRevive(pRole, ERRT_Locus, pRole->GetRevive().nHP, pRole->GetRevive().nMP, 
	pRole->GetRevive().fX, pRole->GetRevive().fY, pRole->GetRevive().fZ, pRole->GetRevive().dwMapID);


	pRole->SetAttValue(ERA_HP, pRole->GetRevive().nHP);
	pRole->SetAttValue(ERA_MP, pRole->GetRevive().nMP);
	pRole->UnsetState(ES_Dead);

	// ��ո�����ؼ�¼��Ϣ
	pRole->GetRevive().nHP = 0; 
	pRole->GetRevive().nMP = 0;
	send.dwErrorCode = E_Success;

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}

	if(E_Success == send.dwErrorCode)
	{
		// ���ָ���ص�
		pRole->GotoNewMap(pRole->GetRevive().dwMapID, 
			pRole->GetRevive().fX, pRole->GetRevive().fY, pRole->GetRevive().fZ);

		const tagBuffProto* pProto = g_attRes.GetBuffProto(9999001);
		if ( P_VALID(pProto))
		{
			// ��ɫ�����޵�BUFF
			pRole->TryAddBuff((Unit*)pRole, pProto, NULL, NULL, NULL);
		}
	}

}

BOOL CGodMiracleMgr::IsGodMiracleTime()
{
	tagDWORDTime dwTime = GetCurrentDWORDTime();
	DWORD dwWeekDay(8);
	dwWeekDay = WhichWeekday(dwTime);

	if (dwWeekDay == 6 && dwTime.hour == 20)
		return TRUE;

	return FALSE;
}

BOOL CGodMiracleMgr::IsAllianceSignUpTime()
{
	tagDWORDTime dwTime = GetCurrentDWORDTime();
	DWORD dwWeekDay(8);
	dwWeekDay = WhichWeekday(dwTime);

	if ((dwWeekDay == 5 && dwTime.hour >= 22) || (dwWeekDay == 6 && dwTime.hour < 18))
		return TRUE;

	return FALSE;
}

DWORD CGodMiracleMgr::CanGuildDismiss(DWORD dwGuildID)
{
	DWORD dwErrorCode = E_Success;
	for (int nCnt=0; nCnt < EGM_MAX; nCnt++)
	{
		dwErrorCode = m_VecGodmiracle[nCnt].CanGuildDismiss(dwGuildID);
		if (dwErrorCode != E_Success)
			break;
	}
	return dwErrorCode;
}

DWORD CGodMiracleMgr::Add2AllianceGuildMap(EGodMiracle	eGodID,					// ս��ID
						  DWORD			dwGuildID,				// ����ID
						  EGuildCapType	eAllianceForceType,		// ���ػ����
						  BOOL			bConfirmed)				// �Ƿ�ȷ����
{
	if (eGodID >= EGM_MAX || eGodID < EGM_NH)
		return E_SystemError;

	if (!IsAllianceSignUpTime())
		return EEGB_NoApplyTime;

	// �жϸù����Ƿ�û��ռ���κ�ս������û�л���κ�ս���Ĺ���Ȩ
	if (GetGuildDefendGodMiracle(dwGuildID) > 0 || GetGuildAttackGodMiracle(dwGuildID) > 0)
		return E_SystemError;	

	return m_VecGodmiracle[eGodID].Add2AllianceGuildMap(dwGuildID, eAllianceForceType, bConfirmed);
}

DWORD CGodMiracleMgr::ConfirmAllianceGuild(EGodMiracle	eGodID,					// ս��ID
										   DWORD		dwGuildID)				// ����ID
{
	if (eGodID >= EGM_MAX || eGodID < EGM_NH)
		return E_SystemError;

	if (!IsAllianceSignUpTime())
		return EEGB_NoApplyTime;

	return m_VecGodmiracle[eGodID].ConfirmAllianceGuild(dwGuildID);
}

VOID CGodMiracleMgr::ClearAllianceGuildMap(EGodMiracle eGodID)
{
	if (eGodID >= EGM_MAX || eGodID < EGM_NH)
		return;

	if (IsGodMiracleTime())
		return;

	m_VecGodmiracle[eGodID].ClearAllianceGuildMap();
}

VOID CGodMiracleMgr::SendAllianceList2Client(EGodMiracle eGodID, Role* pRole)
{
	if (eGodID >= EGM_MAX || eGodID < EGM_NH)
		return;

	if (!P_VALID(pRole))
		return;

	if (!IsAllianceSignUpTime())
		return;

	m_VecGodmiracle[eGodID].SendAllianceList2Client(eGodID, pRole);
}

// �ж�������Ƿ��ڲ�ͬ�İ��ɲ�����ͬһ��
BOOL CGodMiracleMgr::IsCaptureGodAndDifferentGuild(Role* pRole, Role* pAnotherRole)
{
	// ��������ʱ���ڲ����ж�
	if (IsGodMiracleTime() == FALSE)
		return FALSE;
	
	if ( pRole->GetGuildID() == pAnotherRole->GetGuildID())
		return FALSE;

	if ( pRole->GetMapID() != pAnotherRole->GetMapID())
		return FALSE;
	
	DWORD dwMapID = pRole->GetMapID();
	EGodMiracle eGod = GetEGodMiracle(dwMapID);
	if ( eGod >= EGM_MAX || eGod < EGM_NH)
		return FALSE;

	if ( m_VecGodmiracle[eGod].GetCapGodID() != pRole->GetGuildID() &&
		m_VecGodmiracle[eGod].GetApplyID() != pRole->GetGuildID() )
		return FALSE;
	
	if ( m_VecGodmiracle[eGod].GetCapGodID() != pAnotherRole->GetGuildID() &&
		m_VecGodmiracle[eGod].GetApplyID() != pAnotherRole->GetGuildID() )
		return FALSE;
	
	if (m_VecGodmiracle[eGod].IsInAllianceGuild(pRole->GetGuildID()) &&
		m_VecGodmiracle[eGod].IsInAllianceGuild(pAnotherRole->GetGuildID()))
		return FALSE;

	return TRUE;
}

BOOL CGodMiracleMgr::IsInCaptureGodMiracleGuild(Role* pRole)
{
	// ��������ʱ���ڲ����ж�
	if (IsGodMiracleTime() == FALSE)
		return FALSE;

	DWORD dwMapID = pRole->GetMapID();
	EGodMiracle eGod = GetEGodMiracle(dwMapID);
	if ( eGod >= EGM_MAX || eGod < EGM_NH)
		return FALSE;

	if ( m_VecGodmiracle[eGod].GetCapGodID() != pRole->GetGuildID() &&
		m_VecGodmiracle[eGod].GetApplyID() != pRole->GetGuildID() &&
		m_VecGodmiracle[eGod].IsInAllianceGuild(pRole->GetGuildID()) == FALSE)
		return FALSE;

	return TRUE;
}

EGodMiracle CGodMiracleMgr::GetEGodMiracle(DWORD dwMapID)
{
	EGodMiracle eGod = EGM_MAX;
	//FX
	for (INT nCnt = 0; nCnt < EGM_MAX; nCnt++)
	{
		if (dwMapID == m_dwBattleFieldMapID[nCnt])
		{
			eGod = static_cast<EGodMiracle>(nCnt);
			break;
		}
	}
	return eGod;
}

// ÿ���Ӹ���
VOID CGodMiracleMgr::UpdateMin()
{
	for ( int i = 0; i < EGM_MAX; i++)
	{
		m_VecGodmiracle[i].UpdateMin();
	}
}

//BOOL bEndTimeFlag(FALSE);
BOOL bSaveCapTureInfoToActive = FALSE;
// ��ʱ����
VOID CGodMiracleMgr::Update()
{
	tagDWORDTime dwTime = GetCurrentDWORDTime();

	// �����񼣸���
	for ( int i = 0; i < EGM_MAX; i++)
	{
		m_VecGodmiracle[i].Update();
	}
	
	if ( (dwTime.hour == 15 || dwTime.hour == 18 || dwTime.hour == 20) && (dwTime.min == 0 && dwTime.sec == 0))
		UpdateAllGodInfoToAllPlayer();

	if (dwTime.hour == 0 && dwTime.min == 0 && bSaveCapTureInfoToActive == FALSE)
	{
		ILOG->Write(_T("SaveCapTureInfoToActive year=%d,month=%d,day=%d,hour=%d,min=%d"), dwTime.year, dwTime.month, dwTime.day, dwTime.hour, dwTime.min);
		bSaveCapTureInfoToActive = TRUE;
		for ( int i = 0; i < EGM_MAX; i++)
			m_VecGodmiracle[i].SaveCapTureInfoToActive();
	}
	if (dwTime.hour == 0 && dwTime.min == 1)
		bSaveCapTureInfoToActive = FALSE;

	// ����ȷ��
	ConfirmBidFundSignUp();

	// ռ��ս������Ĺ����ʽ�ÿ��22:00�������
	if (dwTime.hour == 22)
	{
		if (dwTime.min == 0 && m_bAddCapGuildFund == FALSE)
		{
			AddCapGuildFundEveryday();
			m_bAddCapGuildFund = TRUE;
		}
		else if (dwTime.min == 1 && m_bAddCapGuildFund == TRUE)
		{
			m_bAddCapGuildFund = FALSE;
		}
	}
	// ����
	UpdateRevie();
	
}

// ����ȷ��
VOID CGodMiracleMgr::ConfirmBidFundSignUp()
{
	tagDWORDTime dwTime = GetCurrentDWORDTime();
	DWORD dwWeekDay(8);
	dwWeekDay = WhichWeekday(dwTime);

	// ����22�㿪ʼȷ�Ͻ�������
	if (dwWeekDay == 5 && dwTime.hour == 22)
	{
		// ǰһ������ȷ�ϣ�����һ�������аɣ����ڶ����ӵ�ʱ���״̬�û���
		if (dwTime.min == 0 && m_bBidFundSignUpConfirm == FALSE)
		{
			for ( int i = 0; i < EGM_MAX; i++)
			{
				m_VecGodmiracle[i].GiveMoneyBack2FailedGuild();
				ClearAllianceGuildMap(static_cast<EGodMiracle>(i));
			}
			m_bBidFundSignUpConfirm = TRUE;
			// ������������г�Ա���͸�����״̬��Ϣ
			UpdateAllGodInfoToAllPlayer();
		}
		else if (dwTime.min == 1 && m_bBidFundSignUpConfirm == TRUE)
		{
			m_bBidFundSignUpConfirm = FALSE;
		}
	}
}

// ռ��ս������Ĺ����ʽ�ÿ��22:00�������
VOID CGodMiracleMgr::AddCapGuildFundEveryday()
{
	// ������=��ռ��ս������������6000+��4500������ȼ�������20
	for ( int i = 0; i < EGM_MAX; i++)
	{
		DWORD dwCapGodID = m_VecGodmiracle[i].GetCapGodID();
		Guild* pGuild = g_guildMgr.GetGuild(dwCapGodID);
		if (P_VALID(pGuild))
		{
			tagGuild sGuild = pGuild->GetGuildAtt();
			INT32 nFund = pGuild->GetCaptureGodMiracleNum()*(6000+4500*sGuild.byLevel)*20;
			pGuild->IncGuildFund(GT_INVALID, nFund, ELCLD_Guild_BattleField_Hold);
		}
	}
}

// �ж����������ǲ��ǵжԹ�ϵ
BOOL CGodMiracleMgr::IsGuildInSameAlliance(DWORD dwMapID, DWORD dwGuildID1, DWORD dwGuildID2)
{
	DWORD dwGodMirMapID[EGM_MAX] = {
		2216111541,   // ����ڣս��
		1781543424,   // �߻�֮��ս��
		1781543680,   // ����ս��
	};
		
	for (INT nGodMirIdx = 0; nGodMirIdx < EGM_MAX; nGodMirIdx++)
	{
		if (dwMapID == dwGodMirMapID[nGodMirIdx])
		{
			BOOL bAlliance = FALSE;
			EGuildCapType eGuildCapType1 = m_VecGodmiracle[nGodMirIdx].GetCapType(dwGuildID1,bAlliance);
			EGuildCapType eGuildCapType2 = m_VecGodmiracle[nGodMirIdx].GetCapType(dwGuildID2,bAlliance);
			if ((eGuildCapType1 == EGCT_Defend || eGuildCapType1 == EGCT_Capture) && eGuildCapType1 == eGuildCapType2)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

VOID CGodMiracleMgr::UpdateRevie()
{

	if ( 0 == m_mapRevieRole.Size() )
		return;

	DWORD dwRoleID = 0;
	vector<DWORD> vecNeedDel;
	tagRevieRoleInfo* pRevive = NULL;
	TMap<DWORD, tagRevieRoleInfo*>::TMapIterator it = m_mapRevieRole.Begin();
	while(m_mapRevieRole.PeekNext(it, dwRoleID, pRevive))
	{
		 pRevive->dwCurTick++;
		if ( pRevive->dwCurTick == 75)
		{

			ReviveRole(dwRoleID);
			vecNeedDel.push_back(dwRoleID);
		}
	}

	// �Ӵ����������ɾ���Ѹ������
	for (vector<DWORD>::iterator it = vecNeedDel.begin(); it != vecNeedDel.end(); ++it)
	{
		tagRevieRoleInfo* pDel = m_mapRevieRole.Peek(*it);
		SAFE_DEL(pDel);
		m_mapRevieRole.Erase(*it);
	}
}


// �õ���ҵĻ�ɱ����
DWORD CGodMiracleMgr::GetOnePleyerKillTimes(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))		
		return NULL;

	DWORD dwGuildID(0);
	dwGuildID = pRole->GetGuildID();
	if (0 == dwGuildID)
		return NULL;

	DWORD dwRetKillTimes(0);
	
	for ( int i=0; i < EGM_MAX; i++)
	{
		//if (dwGuildID ==  m_VecGodmiracle[i].GetApplyID())
		//{
			tagGodRoleInfo* pRoleInfo = m_VecGodmiracle[i].GetGodRoleInfo(EGCT_Capture, dwRoleID);
			if (P_VALID(pRoleInfo ))
				dwRetKillTimes += pRoleInfo->GetKillTimes();
		//}
		//else if (dwGuildID ==  m_VecGodmiracle[i].GetCapGodID())
		//{
			tagGodRoleInfo* pRoleInfo2 = m_VecGodmiracle[i].GetGodRoleInfo(EGCT_Defend, dwRoleID);
			if (P_VALID(pRoleInfo ))
				dwRetKillTimes += pRoleInfo->GetKillTimes();
		//}
	}

	return dwRetKillTimes;
}

// �õ���ҵĻ�Ծʱ
DWORD CGodMiracleMgr::GetonePlyerActiveTime(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))		
		return NULL;

	DWORD dwGuildID(0);
	dwGuildID = pRole->GetGuildID();
	if (0 == dwGuildID)
		return NULL;

	DWORD dwRetActiveTime(0);

	for ( int i=0; i < EGM_MAX; i++)
	{
	//	if (dwGuildID ==  m_VecGodmiracle[i].GetApplyID())
	//	{
			tagGodRoleInfo* pRoleInfo = m_VecGodmiracle[i].GetGodRoleInfo(EGCT_Capture, dwRoleID);
			if (P_VALID(pRoleInfo ))
				dwRetActiveTime += pRoleInfo->GetActiveTime();
	//	}
	//	else if (dwGuildID ==  m_VecGodmiracle[i].GetCapGodID())
	//	{
			tagGodRoleInfo* pRoleInfo2 = m_VecGodmiracle[i].GetGodRoleInfo(EGCT_Defend, dwRoleID);
			if (P_VALID(pRoleInfo ))
				dwRetActiveTime += pRoleInfo->GetActiveTime();
	//	}
	}

	return dwRetActiveTime;
}


// �õ�������񼣵İ��ɵ����������(������ߵ�)
tagGodRoleInfo* CGodMiracleMgr::GetCaptureGuildMaxScoresPlayer(DWORD dwGodID, EGuildCapType eGuidType)
{
	ASSERT( dwGodID >= EGM_NH && dwGodID < EGM_MAX && eGuidType != EGCT_NULL);
	if ( dwGodID <  EGM_NH || dwGodID >= EGM_MAX || eGuidType != EGCT_NULL)
		return NULL;

	// �����񼣲���
	tagGodRoleInfo* pRoleInfo=  m_VecGodmiracle[dwGodID].GetMaxScoresPlayer(eGuidType);
	if ( NULL != pRoleInfo)
		return pRoleInfo;

	return NULL;
}

BOOL CGodMiracleMgr::GetGuildMaxScoresPlayer(std::vector<tagGuildMaxScoresPlayer> &lstMaxScoresPlayer)
{
	for ( int i = 0; i < EGM_MAX; i++)
	{
		// �ط�
		if ( 0 == m_VecGodmiracle[i].GetCapGodID() || 0 == m_VecGodmiracle[i].GetApplyID())
			m_VecGodmiracle[i].Refresh();
		else
		 m_VecGodmiracle[i].GetSomeplayerOverFixedScores(lstMaxScoresPlayer);
	}
	return  FALSE;
}

// �鿴ĳ����ռ�м�����
DWORD CGodMiracleMgr::GetGuildDefendGodMiracle(DWORD dwGuildID)
{
	DWORD dwRet(0);
	for ( int i = 0; i < EGM_MAX; i++)
	{
		if ( m_VecGodmiracle[i].GetCapGodID() == dwGuildID)
			dwRet++;
	}
	if (dwRet == 0 && m_bHaveMergedData == TRUE)
	{
		DWORD dwMergedCapGuildID = 0;
		m_listMergedCapGuildID.ResetIterator();
		while (m_listMergedCapGuildID.PeekNext(dwMergedCapGuildID))
		{
			if (dwMergedCapGuildID == dwGuildID)
			{
				dwRet++;
				if (dwRet == EGM_MAX-1)
					break;
			}
		}
	}
	return dwRet;
}

// �鿴ĳ����ȡ���˼���ս���Ľ���Ȩ
DWORD CGodMiracleMgr::GetGuildAttackGodMiracle(DWORD dwGuildID)
{
	DWORD dwRet(0);
	for ( int i = 0; i < EGM_MAX; i++)
	{
		if ( m_VecGodmiracle[i].GetApplyID() == dwGuildID)
			dwRet++;
	}
	return dwRet;
}


CGodMiracle* CGodMiracleMgr::IsInCaptureGodRange(DWORD dwGuildID)
{
	if ( !P_VALID(dwGuildID))
		return NULL;

	for ( int i = 0; i < EGM_MAX; i++)
	{
		if ( m_VecGodmiracle[i].GetCapGodID() == dwGuildID)
			return &m_VecGodmiracle[i];
		if ( m_VecGodmiracle[i].GetApplyID() == dwGuildID)
			return &m_VecGodmiracle[i];
		if (m_VecGodmiracle[i].GetAllianceCapType(dwGuildID) == EGCT_Defend || m_VecGodmiracle[i].GetAllianceCapType(dwGuildID) == EGCT_Capture) // ����
			return &m_VecGodmiracle[i];;
	}
	return NULL;
}

// �㲥������������������
VOID CGodMiracleMgr::BroadcastAllGoodPlayer()
{
	for ( int i = 0; i < EGM_MAX; i++)
	{
		if ( 0 == m_VecGodmiracle[i].GetCapGodID())
			m_VecGodmiracle[i].Refresh();
		else
			m_VecGodmiracle[i].BroadcastAllGoodPlayer();
	}
}

/// ������״̬
VOID CGodMiracleMgr::SetGodDefendAndCaptureState(int nGodIndex)
{
	if ( nGodIndex < 0 || nGodIndex > 4)
		return ;

	if ( 0 != m_VecGodmiracle[nGodIndex].GetCapGodID() && 0 != m_VecGodmiracle[nGodIndex].GetApplyID())
	{
		Guild* pDefendGuild = g_guildMgr.GetGuild(m_VecGodmiracle[nGodIndex].GetCapGodID());
		if ( P_VALID(pDefendGuild))
			pDefendGuild->UnsetGuildState(EGDSS_Warfare);

		Guild* pCaptureGuild = g_guildMgr.GetGuild(m_VecGodmiracle[nGodIndex].GetApplyID());
		if ( P_VALID(pCaptureGuild))
			pCaptureGuild->UnsetGuildState(EGDSS_Warfare);

	}
}

/// ��ɫ��ɱ������ӵĴ���
VOID CGodMiracleMgr::ModteamInfoForkillRole(DWORD dwGodID, DWORD dwSrcID, DWORD dwTargetID)
{
	if ( dwGodID < NULL || dwGodID > EGM_MAX)
		return ;

	if ( dwSrcID <= NULL || dwTargetID <= NULL)
		return ;

	Role* pSrc = g_roleMgr.GetRolePtrByID(dwSrcID);
	if ( !P_VALID(pSrc))
		return ;

	DWORD dwSrcGuildID(NULL);
	dwSrcGuildID = pSrc->GetGuildID();
	if ( !P_VALID(dwSrcGuildID))
		return ;
		

// 	Role* pTarget = g_roleMgr.GetRolePtrByID(dwTargetID);
// 	if ( !P_VALID(pTarget))
// 		return;
	
	/// Src�Ƿ����
	DWORD dwTeamID(NULL);
	dwTeamID = pSrc->GetTeamID();
	if (NULL == dwTeamID)
		return;
	
	///  �õ����е�С�ӳ�Ա
	DWORD aryTeamRoleID[MAX_TEAM_NUM] = { NULL};
	Team* pTeam = (Team*)g_groupMgr.GetTeamPtr(dwTeamID);
	if ( !P_VALID(pTeam))
		return;
	
	/// �õ���Ա�ĸ���
	DWORD dwCount(NULL);
	dwCount = pTeam->GetTemAllMemberID(aryTeamRoleID);
	if ( NULL == dwCount)
		return;
	
	/// ѭ������������ӵĳ�Ա
	for ( DWORD i = NULL; i < dwCount; i++)
	{
		if ( !P_VALID(aryTeamRoleID[i]))
			continue;
		
		Role* pRole = g_roleMgr.GetRolePtrByID(aryTeamRoleID[i]);
		if ( !P_VALID(pRole))
			continue;

		/// ��ӳ�Ա����ͬһ��ͼֱ�ӷ���
		if ( pSrc->GetMapID() != pRole->GetMapID())
			continue;

		/// ȥ���б����ҵ������
		/// �ط���
		tagGodRoleInfo* pRoleInfo =  m_VecGodmiracle[dwGodID].GetGodRoleInfo(1, aryTeamRoleID[i]);
		if ( P_VALID(pRoleInfo))
			pRoleInfo->SetBeKilledRole(dwTargetID, dwSrcGuildID, TRUE);
		
		pRoleInfo = NULL;
		pRoleInfo = m_VecGodmiracle[dwGodID].GetGodRoleInfo(2, aryTeamRoleID[i]);
		if(P_VALID(pRoleInfo))
			pRoleInfo->SetBeKilledRole(dwTargetID, dwSrcGuildID, TRUE);

		/// �뿪���������κδ���
	}

}

BOOL CGodMiracle::GetAllianceConfirmed(DWORD dwGuildID)
{
	if (dwGuildID == 0)
		return FALSE;

	tagAllianceGuildInfo* pAllianceGuildInfo = m_mapAllianceGuild.Peek(dwGuildID);
	if (P_VALID(pAllianceGuildInfo))
		return pAllianceGuildInfo->bConfirmed;

	return FALSE;
}
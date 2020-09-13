//-----------------------------------------------------------------------------
//!\file talent_handler.cpp
//!\author wjiang

//!\date 2008-12-04
//! last 
//!
//!\brief �����ύ����Ϣ������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/group_define.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_group.h"
#include "../WorldDefine/msg_social.h"

#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "group_mgr.h"
#include "social_mgr.h"
#include "../WorldDefine/msg_easy_team.h"
#include "convenient_team.h"

//-----------------------------------------------------------------------------
// ��Ӻ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMakeFriend(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_RoleMakeFriend* pRecv = (tagNC_RoleMakeFriend*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_MakeFriend, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ɾ������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCancelFriend(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_RoleCancelFriend* pRecv = (tagNC_RoleCancelFriend*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_CancelFriend, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ɾ������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpdateFriendGroup(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_UpdateFriGroup* pRecv = (tagNC_UpdateFriGroup*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_FriendGrp, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ��������������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleMoveBlackList(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_MoveToBlackList* pRecv = (tagNC_MoveToBlackList*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_InsertBlkList, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ������Ƴ�������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDeleteBlackList(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_DeleteBlackList* pRecv = (tagNC_DeleteBlackList*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_DeleteBlkList, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSendGift(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_SendGift* pRecv = (tagNC_SendGift*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_SendGift, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// �յ������ظ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSendGiftReply(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_SendGiftReply* pRecv = (tagNC_SendGiftReply*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_SendGiftRpy, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ������Ҽ���С��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleJoinTeam(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_InviteJoinTeam* pRecv = (tagNC_InviteJoinTeam*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_JoinTeam, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ��������Ƿ�ͬ�����С��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleJoinTeamReply(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_InviteReply* pRecv = (tagNC_InviteReply*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_JoinTeamRepley, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ����ҳ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleKickMember(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_KickMember* pRecv = (tagNC_KickMember*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_KickMember, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ����뿪С��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLeaveTeam(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_LeaveTeam* pRecv = (tagNC_LeaveTeam*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_LeaveTeam, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// ����С��ʰȡģʽ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetPickMol(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole))	return GT_INVALID;

	tagNC_SetPickMode* pRecv = (tagNC_SetPickMode*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_SetPickMol, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// �ı�С�Ӷӳ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeLeader(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole))	return GT_INVALID;

	tagNC_ChangeLeader* pRecv = (tagNC_ChangeLeader*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_ChangeLeader, pCmd->dwSize, pRecv);

	return 0;
}


//-----------------------------------------------------------------------------
// ͬ����������Ƿ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpdateFriOnline(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole))	return GT_INVALID;

	tagNC_UpdateFriEnemyState* pRecv = (tagNC_UpdateFriEnemyState*)pCmd;

	m_pRole->UpdateFriOnline();
	return 0;
}


//Jason ����������Э��

DWORD	PlayerSession::HandleApplyJoinTeam(tagNetCmd *pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNC_ApplyToJoinTeam* pRecv = (tagNC_ApplyToJoinTeam*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_ApplyToJoinTeam, pCmd->dwSize, pRecv);

	return 0;
}
DWORD	PlayerSession::HandleReplyForApplingToJoinTeam(tagNetCmd *pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNC_ApplyReply* pRecv = (tagNC_ApplyReply*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_ApplyReply, pCmd->dwSize, pRecv);

	return 0;
}

// �������
DWORD PlayerSession::HandleTeamProfit(tagNetCmd* pCmd)
{
	if (!P_VALID(pCmd))	return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_TeamBufChange);

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	// �������Ĵ���������Ϊ1
	if (pRecv->bTeamBuffer) 
	{
		DWORD dwTeamID = pRole->GetTeamID();
		if (dwTeamID == GT_INVALID)
		{
			// �������С�ӣ���ô��Ϣ��֤��ͨ����������
			return GT_INVALID;
		}
		else
		{
			if (pRole->CanGetTeamProfit())
			{
				pRole->GetTeamProfit();
				return E_Success;
			}
		}
	}
	// �������Ĵ���������Ϊ0
	else
	{
		DWORD dwTeamID = pRole->GetTeamID();
		if (dwTeamID == GT_INVALID)
		{
			// �������С�ӣ���ô��Ϣ��ʵ��ȡ���������
			pRole->CancelTeamProfit();
			return E_Success;
		}
		else
		{
			// ��֤��100�������Ƿ��ж���
			if (!pRole->CanGetTeamProfit())
			{
				pRole->CancelTeamProfit();
				return E_Success;
			}
		}
	}

	return GT_INVALID;
}

//---------------------------------------------------------------
// ����б������Ϣ
//---------------------------------------------------------------
// ɾ�����
DWORD PlayerSession::HandleDeleteEnemy(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNC_DeleteEnemy* pRecv = (tagNC_DeleteEnemy*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_DeleteEnemy, pCmd->dwSize, pRecv);

	return 0;
}

// ��������Ϣ�������Ҫ��g_groupMgr���洦����Ϊ��Ϣ���ܺܶ࣬�Ž���ͼ�̰߳ɣ����ΰ�������
DWORD PlayerSession::HandleGetEasyTeamInfo(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_GetEasyTeamInfo * pMsg = NULL;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	if( EasyTeam.GetEasyTeamInfo(pMsg,m_pRole,&g_MemPoolSafe) )
	{
		SendMessage(pMsg,pMsg->dwSize);
		g_MemPoolSafe.Free(pMsg);
	}

	return 0;
}

DWORD PlayerSession::HandleGetFreePlayers(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_GetSinglePlayers * pMsg = NULL;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	if( EasyTeam.GetFreePlayers(pMsg,m_pRole,&g_MemPoolSafe) )
	{
		SendMessage(pMsg,pMsg->dwSize);
		g_MemPoolSafe.Free(pMsg);
	}

	return 0;
}

DWORD PlayerSession::HandleGetPersonalInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_GetPersonalInfo * pMsg = NULL;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	if( EasyTeam.GetPersonalInfo(pMsg,m_pRole,&g_MemPoolSafe) )
	{
		SendMessage(pMsg,pMsg->dwSize);
		g_MemPoolSafe.Free(pMsg);
	}

	return 0;
}
DWORD PlayerSession::HandleNCPutOutInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_PutOutInfo msg;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	msg.dwErrorCode = EasyTeam.IssueCTInfo((tagNC_PutOutInfo*)pCmd,m_pRole);
	SendMessage(&msg,msg.dwSize);

	return 0;
}
DWORD PlayerSession::HandleNCDeleteAppTeamInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.DeleteCTInfoOfPlayer(m_pRole);

	return 0;
}
DWORD PlayerSession::HandleNCDeleteMyTeamInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.DeleteCTInfoOfPlayer(m_pRole);

	return 0;
}





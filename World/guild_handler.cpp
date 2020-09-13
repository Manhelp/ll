//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_handler.h
// author: 
// actor:
// data: 2009-04-17
// last:
// brief: ������Ϣ����
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "guild_CofC.h"
#include "../WorldDefine/msg_guild.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/login_issue.h"
#include "login_session.h"
// Jason 2010-02-20 v1.3.2 ������������
#include "guild_apply_to_join.h"
#include "city_mgr.h"
#include "att_res.h"
#include "GuildBattle.h"
#include "../WorldDefine/filter.h"
#include "guild_war_declare_mgr.h"
#include "../WorldDefine/msg_guild_war_declare.h"
#include "MsgInfoMgr.h"
#include "../WorldDefine/msg_GuildBattle.h"
#include "creature.h"
#include "item_creator.h"

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCreateGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildCreate);
    
	
	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��������ǰ�����жϽ�ɫ�����Ƿ��С����ɾ��ѡ���buff(bufferID��LEAVE_GUILD_BUFF_ID)
	if (pRole->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
	{
		tagNS_RoleInJustLeaveGuildState msg;
		msg.dwErrorCode = E_Guild_Can_Not_Join_Or_Create;
		pRole->SendMessage(&msg, msg.dwSize);
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;

	INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildCreate, szGuildName)) / sizeof(TCHAR);
	// ��ȡ���� -- ȷ�����ַ���������
	tstring szTmpGuildName(pRecv->szGuildName, nNameTCHAR);

	// �������Ϸ����ж�
	dwErrorCode = Filter::CheckName(szTmpGuildName.c_str(), g_attRes.GetVariableLen().nGuildNameMax, 
		g_attRes.GetVariableLen().nGuildNameMin, g_attRes.GetNameFilterWords());
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		return dwErrorCode;
	}

	// �Ƿ����ѽ���������
	DWORD dwNewGuildID = g_world.LowerCrc32(szTmpGuildName.c_str());
	if(g_guildMgr.GetMapGuild().IsExist(dwNewGuildID))
	{
		dwErrorCode = E_Guild_Create_NameExist;
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		return dwErrorCode;
	}

	// �ȼ��Ƿ�ﵽ20��
	if(pRole->GetLevel() < g_guildMgr.GetGuildCfg().nCreateRoleMinLevel)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, E_Guild_Create_LevelLimit);
		return dwErrorCode;
	}

	// �ֽ��Ƿ��㹻(10��)
	INT64 n64Cost = MGold2Silver(g_guildMgr.GetGuildCfg().nGoldCreateNeeded);
	if(pRole->GetCurMgr().GetBagSilver() < n64Cost)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, E_BagSilver_NotEnough);
		return E_BagSilver_NotEnough;
	}
	// NPC���
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Guild);

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		return dwErrorCode;
	}

	tagNWL_CreateGuild msg;
	memset(msg.szGuildName,0,sizeof(msg.szGuildName));
	msg.dwAccountID = m_dwAccountID;
	msg.dwNPCID  = pRecv->dwNPCID;
	msg.dwRoleID = pRole->GetID();
	_tcscpy_s(msg.szGuildName,X_SHORT_NAME,szTmpGuildName.c_str());
	g_loginSession.Send(&msg,msg.dwSize);
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ɢ����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDismissGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildDismiss);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;
	DWORD dwGuildID = pRole->GetGuildID();

	// NPC���
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Guild);
	if(E_Success == dwErrorCode)
	{
		// ����״̬�ж�
		Guild *pGuild = g_guildMgr.GetGuild(dwGuildID);
		if( !P_VALID(pGuild) )
		{
			return GT_INVALID;
		}
		else if( pGuild->IsInGuildState(EGDSS_Warfare) )
		{
			dwErrorCode = E_Guild_State_Limit;
		}

		if (P_VALID(CGodMiracleMgr::GetInst()))
		{
			dwErrorCode = CGodMiracleMgr::GetInst()->CanGuildDismiss(dwGuildID);
		}

		if (E_Success == dwErrorCode)
		{
			dwErrorCode = g_guildMgr.DismissGuild(pRole, dwGuildID);
			if( E_Success == dwErrorCode )
			{
				ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
				if( P_VALID(pToolkit) && P_VALID(pRole) )
				{
					pToolkit->DelGuild(dwGuildID);
				}
			}
		}
	}

	// ��ͻ��˷���
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGuildReq(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildJoinReq);
	
	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ����ǰ�����жϽ�ɫ�����Ƿ��С����ɾ��ѡ���buff(bufferID��LEAVE_GUILD_BUFF_ID)
	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}
	if (pDestRole->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
	{
		tagNS_RoleInJustLeaveGuildState msg;
		msg.dwErrorCode = E_Guild_Can_Not_Be_Invited;
		pRole->SendMessage(&msg, msg.dwSize);
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pRole->GetID() == pRecv->dwDstRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����״̬�ж�
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// ����
	Role *pInvitee = NULL;
	DWORD dwErrorCode = pGuild->CanInviteJoin(pRole->GetID(), pRecv->dwDstRoleID, &pInvitee);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		ASSERT(P_VALID(pInvitee));

		tagNS_GuildJoinReq send;
		send.dwSrcRoleID	= pRole->GetID();
		send.dwGuildID		= pRole->GetGuildID();
		pInvitee->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��������ҷ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGuildReqRes(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildJoinReqRes);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwDstGuildID);
	if(!P_VALID(pGuild))
	{ 
		return GT_INVALID;
	}

	if(pRecv->dwErrorCode != E_Success)
	{
		// �������������Ϣ
		pGuild->EraseInviteJoin(pRole->GetID());	

		// ����������Ƿ�������
		Role *pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
		if(P_VALID(pInviter))
		{
			// �������߷���Ϣ
			tagNS_GuildJoinReqRes	send;
			send.dwErrorCode = E_Guild_Join_BeRefused;
			send.dwInviterID = pRecv->dwDstRoleID;
			send.dwInviteeID = pRole->GetID();
			pInviter->SendMessage(&send, send.dwSize);
		}

		return E_Success;
	}

	// ����
	DWORD dwErrorCode = pGuild->InviteJoin(pRecv->dwDstRoleID, pRole->GetID());
	if(dwErrorCode != E_Success)
	{		
		// ��˫������Ϣ
		tagNS_GuildJoinReqRes	send;
		send.dwErrorCode = dwErrorCode;
		send.dwInviterID = pRecv->dwDstRoleID;
		send.dwInviteeID = pRole->GetID();
		SendMessage(&send, send.dwSize);

		// ����������Ƿ�������
		Role *pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
		if(P_VALID(pInviter))
		{
			pInviter->SendMessage(&send, send.dwSize);
		}
	}
	else
	{
		// ��ɫ������ɣ�����Ƿ��й�����ս��ϵ
		g_guildWarDeclareMgr.OnRoleJoinGuild(pRole);

		// ���ɹ㲥
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		if (P_VALID(pRoleInfo))
		{
			INT32 nRoleNameCnt = _tcsclen(pRoleInfo->szRoleName);
			INT32 nMsgSz = sizeof(tagNS_GuildJoinBroad) + nRoleNameCnt * sizeof(TCHAR);

			MCREATE_MSG(pSend, nMsgSz, NS_GuildJoinBroad);
			pSend->dwRoleID		= pRole->GetID();
			pSend->dwRoleNameID	= pRole->GetNameID();

			memcpy(pSend->szRoleName, pRoleInfo->szRoleName, (nRoleNameCnt + 1) * sizeof(TCHAR));

			pGuild->SendGuildMsg(pSend, pSend->dwSize);
			MDEL_MSG(pSend);
		}

		// ͬ������Χ���
		tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
		Map*			pMap	= pRole->GetMap();
		if (P_VALID(pMember) && P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send;
			send.dwGuildID		= pRecv->dwDstGuildID;
			send.dwRoleID		= pRole->GetID();
			send.n8GuildPos		= pMember->eGuildPos;

			pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}

		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			pToolkit->ChangeGuildMember(pRecv->dwDstGuildID,pRole->GetID());
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �뿪����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleLeaveGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildLeave);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ������״̬����
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// ����
	DWORD dwErrorCode = pGuild->LeaveGuild(pRole);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ���ɹ㲥
		tagNS_GuildLeaveBroad send;
		send.dwSrcRoleID = pRole->GetID();
		pGuild->SendGuildMsg(&send, send.dwSize);

		// �뿪��
		SendMessage(&send, send.dwSize);

		// ��ɫ�뿪���ɣ�����Ƿ��й�����ս��ϵ
		g_guildWarDeclareMgr.OnRoleLeaveGuild(pRole);

		// ͬ������Χ���
		Map* pMap	= pRole->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= GT_INVALID;
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= EGMP_Null;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}

		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			pToolkit->ChangeGuildMember(pGuild->GetGuildAtt().dwID,pRole->GetID());
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �Ӱ������߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleKickFromGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildKick);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pRole->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ������״̬����
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// ����
	DWORD dwErrorCode = pGuild->KickMember(pRole->GetID(), pRecv->dwRoleID);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildKickBroad send;
		send.dwDstRoleID = pRecv->dwRoleID;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ��鱻�����Ƿ�����
		Role *pKicked = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if(P_VALID(pKicked))
		{
			pKicked->SendMessage(&send, send.dwSize);

			// ͬ������Χ���
			Map* pMap	= pKicked->GetMap();
			if (P_VALID(pMap))
			{
				tagNS_RemoteRoleGuildInfoChange send_broad;
				send_broad.dwGuildID	= GT_INVALID;
				send_broad.dwRoleID		= pKicked->GetID();
				send_broad.n8GuildPos	= EGMP_Null;

				pMap->SendBigVisTileMsg(pKicked, &send_broad, send_broad.dwSize);
			}

			// ��ɫ�뿪���ɣ�����Ƿ��й�����ս��ϵ
			g_guildWarDeclareMgr.OnRoleLeaveGuild(pKicked);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �ƽ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleTurnoverGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildTurnover);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pRole->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ������״̬����
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// ����
	DWORD dwErrorCode = pGuild->TurnoverLeader(pRole->GetID(), pRecv->dwRoleID);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildTurnoverBroad send;
		send.dwSrcRoleID	= pRole->GetID();
		send.dwDstRoleID	= pRecv->dwRoleID;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ͬ������Χ���(ԭ�������Ȱ���)
		tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
		Map*			pMap	= pRole->GetMap();
		if (P_VALID(pMember) && P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pRole->GetGuildID();
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= pMember->eGuildPos;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}

		// �ж��Ȱ����Ƿ�����
		Role* pNewLeader = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (P_VALID(pNewLeader))
		{
			pMember = pGuild->GetMember(pNewLeader->GetID());
			pMap	= pNewLeader->GetMap();
			if (P_VALID(pMember) && P_VALID(pMap))
			{
				tagNS_RemoteRoleGuildInfoChange send_broad;
				send_broad.dwGuildID	= pNewLeader->GetGuildID();
				send_broad.dwRoleID		= pNewLeader->GetID();
				send_broad.n8GuildPos	= pMember->eGuildPos;

				pMap->SendBigVisTileMsg(pNewLeader, &send_broad, send_broad.dwSize);
			}
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ְ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDemissFromGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildDemiss);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ������״̬����
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// ����
	INT8 n8OldGuildPos = EGMP_HuiYuan;
	DWORD dwErrorCode = pGuild->DemissPostion(pRole->GetID(), n8OldGuildPos);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildDemissBroad send;
		send.dwRoleID		= pRole->GetID();
		send.n8OldGuildPos	= n8OldGuildPos;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ͬ������Χ���
		Map* pMap	= pRole->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pRole->GetGuildID();
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= EGMP_HuiYuan;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����ְλ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAppointForGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildAppoint);

	// ��ȡ��ɫ
	Role *pAppointor = GetRole();
	if(!P_VALID(pAppointor))
	{
		return GT_INVALID;
	}

	// �Ƿ���Լ�����
	if(pAppointor->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pAppointor->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}
	
	// ������״̬����
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// ����
	DWORD dwErrorCode = pGuild->Appoint(pAppointor->GetID(), pRecv->dwRoleID, pRecv->ePos);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildAppointBroad send;
		send.dwSrcRoleID	= pAppointor->GetID();
		send.dwDstRoleID	= pRecv->dwRoleID;
		send.ePos			= pRecv->ePos;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// ͬ������Χ���
		Role* pDstRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if( P_VALID(pDstRole) && P_VALID(pDstRole->GetMap()) )
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pDstRole->GetGuildID();
			send_broad.dwRoleID		= pRecv->dwRoleID;
			send_broad.n8GuildPos	= pRecv->ePos;

			pDstRole->GetMap()->SendBigVisTileMsg(pDstRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �޸İ�����ּ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleChangeGuildTenet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildTenetChange);

	// ��ȡ��ɫ
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ����
	INT32 nTenetCnt = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildTenetChange, szTenet)) / sizeof(TCHAR);
	DWORD dwErrorCode = pGuild->ModifyTenet(pRole->GetID(), pRecv->szTenet, nTenetCnt);
	if(dwErrorCode != E_Success)
	{
		// �ж��Ƿ���Ҫ����
		if (dwErrorCode != E_Return_NotNeedFeed)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
	}
	else
	{
		INT32 nMsgSz = sizeof(tagNS_GuildTenetChangeBroad) + nTenetCnt * sizeof(TCHAR);
		MCREATE_MSG(pSend, nMsgSz, NS_GuildTenetChangeBroad);
		pSend->dwRoleID	= pRole->GetID();
		memcpy(pSend->szTenet, pRecv->szTenet, (nTenetCnt + 1) * sizeof(TCHAR));
		pSend->szTenet[nTenetCnt] = _T('\0');
		pGuild->SendGuildMsg(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}


//********************** ��ȡ���������Ϣ��Ϣ���� **********************//

//-----------------------------------------------------------------------------
// �����޸�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGuildChangeName(tagNetCmd* pCmd)
{ 
	MGET_MSG(pRecv, pCmd, NC_GuildChangeName);
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
    DWORD dwError = E_Success ;
	INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildChangeName, szGuildName)) / sizeof(TCHAR);
	tstring szTmpGuildName(pRecv->szGuildName, nNameTCHAR);

	// �������Ϸ����ж�
	dwError = Filter::CheckName(szTmpGuildName.c_str(), g_attRes.GetVariableLen().nGuildNameMax, 
		g_attRes.GetVariableLen().nGuildNameMin, g_attRes.GetNameFilterWords());
	if(dwError != E_Success)
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_NameInvalid ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;

	}


	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if ( !P_VALID(pGuild))
	{
        return GT_INVALID;
	}
    

	tagGuildMember* pGuildMember = pGuild->GetMember(pRole->GetID());
	if ( P_VALID(pGuildMember) )
	{
		if ( pGuildMember->eGuildPos !=  EGMP_HuiZhang)
		{
             tagNS_GuildChangeName send;
			 send.dwErrorCode = E_GuildChangeName_NotBangZhu ;
			 pRole->SendMessage(&send,send.dwSize);
			 return 0 ;
		}
	}

	if ( pGuild->GetGuildState() ==  EGDSS_Warfare)
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_Warfare ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}

	//�жϱ������Ƿ��ڳ�ս����״̬
	DWORD dwGuildId = pGuild->GetGuildAtt().dwID  ;
	if(g_cityMgr.IsAlreadySignup(dwGuildId,ECFT_Attack)
		||g_cityMgr.IsAlreadySignup(dwGuildId,ECFT_Defence)
		||g_cityMgr.IsAlreadySignup(dwGuildId,ECFT_Request))
	{
	
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_AlreadySigup ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}

	//�жϱ������Ƿ��������ᱨ��״̬
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->IsInCaptureGodRange(dwGuildId);
	if ( P_VALID(pGodMiracle) )
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_IsInGodRange ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}
    
	// �����Ƿ�����
	DWORD dwNewGuildID = g_world.LowerCrc32(szTmpGuildName.c_str());
    if( P_VALID(g_guildMgr.GetGuild(dwNewGuildID)))
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_Alreadyexist ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}
	
	
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Serial);
    if ( !P_VALID(pItem))
    {
		return GT_INVALID;
    }
	else
	{
		dwError = pRole->GetItemMgr().DelFromBag(pRecv->n64Serial,ELCID_Guild_ChangeName,1);
		if(dwError == E_Success)
		{
             pGuild->ChangeName(szTmpGuildName.c_str()); 
			 INT nMsgSz = sizeof(tagNS_GuildChangeName) + ((pGuild->GetGuildAtt().strName.size()) * sizeof(TCHAR));
			 MCREATE_MSG(pSend, nMsgSz, NS_GuildChangeName);
			 pSend->dwErrorCode = E_Success ;
			 memcpy(pSend->szGuildName,pGuild->GetGuildAtt().strName.c_str(),pGuild->GetGuildAtt().strName.size()*sizeof(TCHAR)) ;
			 pGuild->SendGuildMsg(pSend,pSend->dwSize);
             
			 //��ÿ�����ɳ�Ա��Χ9�ո�㲥���ֱ仯
			 pGuild->SendChangeGuildNameMsg(NULL,0);
             
			

			  MDEL_MSG(pSend);
			 return 0 ; 
		}
		
		
	}
   return E_Success;





}


//-----------------------------------------------------------------------------
// ��ȡ�������г�Ա
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildMembers(tagNetCmd* pCmd)
{
	//MGET_MSG(p, pCmd, NC_GuildGetAllMembers);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// ִ�е��˴���˵�����ɹ��������� -- ������û�г�Աʱ�ſ���ɾ������
		ASSERT(0);
		return GT_INVALID;
	}

	pGuild->SendAllMembers2Client(pRole);
	//// Jason 2010-3-9 v1.4.0 ���ɹ�ѫ֪ͨ��Ϣ
	//tagNS_GuildExploit msg_guild_exploit;
	//msg_guild_exploit.dwRoleID = pRole->GetID();
	//msg_guild_exploit.nExploit = 0;

	//tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
	//if( P_VALID(pMember) )
	//{
	//	msg_guild_exploit.nExploit = pMember->nExploit;
	//}

	//pRole->SendMessage(&msg_guild_exploit,msg_guild_exploit.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ����ָ����Ա��չ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildMemberEx(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_GuildGetMemberEx);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// ִ�е��˴���˵�����ɹ��������� -- ������û�г�Աʱ�ſ���ɾ������
		ASSERT(0);
		return GT_INVALID;
	}

	return pGuild->SendSpecMemberEx2Client(pRole, p->dwRoleID);
}

//-----------------------------------------------------------------------------
// ˢ�°���ָ����Ա��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRefreshGuildMember(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_GuildRefMember);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// ִ�е��˴���˵�����ɹ��������� -- ������û�г�Աʱ�ſ���ɾ������
		ASSERT(0);
		return GT_INVALID;
	}

	return pGuild->SendSpecMember2Client(pRole, p->dwRoleID);
}

//-----------------------------------------------------------------------------
// ��ȡ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildName);

	Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	return pGuild->SendGuildName2Client(this);
}
//-----------------------------------------------------------------------------
// ������ȡ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSomeGuildName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetSomeGuildName);

	INT		nNum = pRecv->nNum;

	if(nNum <= 0)
		return 0;

	if(nNum > 50)
		return 0;

	DWORD	dwSize = sizeof(tagNS_GetSomeGuildName) + (nNum - 1) * sizeof(tagGuildIDName);
	MCREATE_MSG(pSend, dwSize, NS_GetSomeGuildName);

	pSend->nNum = nNum; 
	pSend->nUserData = pRecv->nUserData;

	for(INT n = 0; n < nNum; ++n)
	{
		pSend->IdName[n].dwID = pRecv->dwAllID[n];
		Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwAllID[n]);
		if(P_VALID(pGuild))
		{
			pRecv->dwAllID[n] = pGuild->GetGuildAtt().dwID;
			_tcscpy(pSend->IdName[n].szName, pGuild->GetGuildAtt().strName.c_str());	
		}
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}
//-----------------------------------------------------------------------------
// ��ȡ������ּ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildTenet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildTenet);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	return pGuild->SendGuildTenet2Client(this);
}

//-----------------------------------------------------------------------------
// ��ȡ���ɲֿ�����Ʒ��Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildWareItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildWare);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success != dwErrorCode)
	{
		return GT_INVALID;
	}

	// �����㹻��Ļ�����
	MCREATE_MSG(pSend, TEMP_GUILD_BUFF_SIZE, NS_GetGuildWare);
	pSend->dwLastUpdateTime = pRecv->dwLastUpdateTime;

	INT32 nItemSize = 0;
	pSend->dwErrorCode = pGuild->GetGuildWarehouse().GetGuildWareInfo(pSend->byData, pSend->nItemNum, pSend->dwLastUpdateTime, pSend->nSzGuildWare, nItemSize);

	// ����������Ϣ��С
	pSend->dwSize = sizeof(tagNS_GetGuildWare) + nItemSize;

	// ���͸��ͻ���
	SendMessage(pSend, pSend->dwSize);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ���ɲֿ����Ȩ���б�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildWarePriList(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildWarePriList);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->GetGuildWarehouse().SendGuildWarePriList2Client(pRole);
	}

	if (GT_VALID(dwErrorCode) && (E_Success != dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------------------
// ���ָ����Ա�İ��ɲֿ����Ȩ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGuildWarePrivilege(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildWarePri);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->SetGuildWarePrivilege(pRole, pRecv->dwRoleID, pRecv->bCanUse);
	}
	
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ֪ͨ�����߸����б�
		tagNS_GuildWarePri send;
		send.dwRoleID	= pRecv->dwRoleID;
		send.bCanUse	= pRecv->bCanUse;
		SendMessage(&send, send.dwSize);

		// ֪ͨ��������Ȩ�ޱ��
		Role* pMember = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (P_VALID(pMember))
		{
			pMember->SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ������ʩ������Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildFacilitiesInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildUpgradeInfo);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	// NPC���
	if (pRecv->dwNPCID != GT_INVALID)
	{
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildMgr);
	}

	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->SendFacilitiesInfo2Client(pRole);
	}

	if (GT_VALID(dwErrorCode) && (E_Success != dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����Ͻɰ�����ʩ����������Ʒ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleHandInItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_HandInItems);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// ��������ʩ�Ƿ��Ѿ�����
	if (pGuild->CheckLevelFull(pRecv->eType) == TRUE)
	{
		return GT_INVALID;
	}

	// NPC���
	DWORD dwErrorCode = E_Success;
	if (pRecv->dwNPCID != GT_INVALID)
	{
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildMgr);
	}
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->GetGuildFacilities().HandInItems(pRole, pRecv->eType);
	}

	if (GT_VALID(dwErrorCode))
	{
		if (E_Success != dwErrorCode)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
		else
		{
			// ���ر仯�����ʩ��Ϣ
			tagNS_UpdateFacilitiesInfo send;
			pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&send.sFacilitiesInfo, pRecv->eType);
			pRole->SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ������������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSpreadGuildAffair(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SpreadGuildAffair);
	if (!GT_VALID(pRecv->dwBuffID))
	{
		return GT_INVALID;
	}

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildAffair().SpreadGuildAffair(pRole, pRecv->dwBuffID);

	if (GT_VALID(dwErrorCode) && (dwErrorCode != E_Success))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ȡ�ð��ɼ�����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildSkillInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	MCREATE_MSG(pSend, TEMP_GUILD_BUFF_SIZE, NS_GetGuildSkillInfo);
	DWORD dwErrorCode = pGuild->GetGuildSkill().GetGuildSkillInfo(pSend->dwCurSkillID, pSend->n16Progress,
		pSend->nSkillNum, pSend->dwSkillInfo);

	// ���¼�����Ϣ��С
	if (dwErrorCode == E_Success)
	{
		pSend->dwSize = sizeof(tagNS_GetGuildSkillInfo) + (pSend->nSkillNum - 1) * sizeof(tagGuildSkills);
		SendMessage(pSend, pSend->dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �Ͻɰ��ɼ��ܵ伮
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpgradeGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildSkillUpgrade);
	if (!GT_VALID(pRecv->n64ItemSerial))
	{
		return GT_INVALID;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_GuildSkillUpgrade send;
	DWORD dwErrorCode = pGuild->GetGuildSkill().SkillUpgradeByHandIn(pRole, pRecv->n64ItemSerial, send.dwSkillID, send.n16Progress);
	if (E_Success == dwErrorCode)
	{
		SendMessage(&send, send.dwSize);
	}
	else if (GT_VALID(dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ѧϰ���ɼ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleLearnGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LearnGuildSkill);
	if (!GT_VALID(pRecv->dwSkillID))
	{
		return GT_INVALID;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_LearnGuildSkill send;
	BOOL bLearnError = FALSE;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().LearnGuildSkill(pRole, pRecv->dwSkillID, nLevel, bLearnError);

	if (GT_VALID(dwErrorCode))
	{
		if (bLearnError)
		{
			send.dwErrorCode	= dwErrorCode;
			send.dwSkillID		= pRecv->dwSkillID * 100 + nLevel;
			SendMessage(&send, send.dwSize);
		}
		else if (dwErrorCode != E_Success)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
	}
	
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ���õ�ǰ�о����ɼ���
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSetResearchSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetResearchSkill);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_SetResearchSkill send;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().SetCurUpgradeGuildSkill(pRole->GetID(), pRecv->dwSkillID, nLevel, send.n16Progress);
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		if (GT_VALID(pRecv->dwSkillID))
		{
			send.dwSkillID	= pRecv->dwSkillID * 100 + nLevel;
		}
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ����Ἴ��
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleActiveGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ActiveGuildSkill);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagGuildMember* pMaster = pGuild->GetMemberMgr().GetMaster();
	if (!P_VALID(pMaster))
	{
		return GT_INVALID;
	}

	// ����ǲ��ǻ᳤
	tagGuildMember* pGuildMember = pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pGuildMember))
	{
		return GT_INVALID;
	}
	if (pGuildMember->eGuildPos != EGMP_HuiZhang && pGuildMember->eGuildPos != EGMP_FuHuiZhang)
	{
		return GT_INVALID;
	}

	tagNS_ActiveGuildSkill send;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().ActiveGuildSkill(pRole->GetID(), pRecv->n64ItemID, pRole->GetGuildID(), pRecv->dwGuildSkillID);
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		if (GT_VALID(pRecv->dwGuildSkillID))
		{
			send.dwGuildSkillID	= pRecv->dwGuildSkillID;
		}
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ͬ�����ɻ�����Ϣ
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSyncGuildInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_SyncGuildInfo send;
	send.sGuildInfo = (tagGuildBase)pGuild->GetGuildAtt();

	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ�����̻���Ϣ	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCofCInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCofCInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(pRecv->dwNPCID);
	if (!P_VALID(pCofC))
	{
		// ����Ҫ֪ͨ�ͻ���
		return GT_INVALID;
	}

	return pCofC->SendCommerceGoodsInfo2Role(pRole);
}

//-----------------------------------------------------------------------------
// �ر��̻����		-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCloseCofC(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CloseCofC);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(pRecv->dwNPCID);
	if (!P_VALID(pCofC))
	{
		return GT_INVALID;
	}

	// �Ƴ��۲����б�
	pCofC->RemoveObserver(pRole->GetID());

	return E_Success;
}

//-----------------------------------------------------------------------------
// �����̻��̻�		-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleBuyCofCGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BuyCofCGoods);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().BuyGoods(pRole, pRecv->dwNPCID, pRecv->dwGoodID, pRecv->byBuyNum);
	if ((E_Success != dwErrorCode) && GT_VALID(dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// �����̻����̻�	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSellCofCGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SellCofCGoods);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().SellGoods(pRole, pRecv->dwNPCID, pRecv->dwGoodID, pRecv->bySellNum);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ����������Ϣ	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCommodityInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCommodityInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT nMsgSize = sizeof(tagNS_GetCommodityInfo) + sizeof(tagCommerceGoodInfo) * (MAX_COMMODITY_CAPACITY - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCommodityInfo);
	// ��ȡ�̻���Ϣ
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommodityGoodInfo(pRole, pSend->sGoodInfo, pSend->nGoodNum, pSend->nCurTael, pSend->nLevel);

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ������Ϣ��С
		pSend->dwSize = sizeof(tagNS_GetCommodityInfo) + sizeof(tagCommerceGoodInfo) * (pSend->nGoodNum - 1);
		SendMessage(pSend, pSend->dwSize);
	}

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��ȡ���̳�ʼ��Ϣ	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetTaelInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_GetTaelInfo send;
	tagTaelInfo sTaelInfo;
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommerceBeginningInfo(pRole->GetID(), send.nLevel, sTaelInfo);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// ��ȡ������������	-- ��ͼ�߳�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCommerceRank(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT nMsgSize = sizeof(tagNS_GetCommerceRank) + sizeof(tagCommerceRank) * (MAX_COMMERCE_RANK_INFO - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCommerceRank);
	// ��ȡ������Ϣ
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommerceRankInfo(pSend->sRankInfo, pSend->nRankNum, pSend->bCommend);

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// ������Ϣ��С
		pSend->dwSize = sizeof(tagNS_GetCommerceRank) + sizeof(tagCommerceRank) * (pSend->nRankNum - 1);
		SendMessage(pSend, pSend->dwSize);
	}

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAcceptCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().AcceptCommerce(pRole);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		INT nLevel;
		tagTaelInfo sTaelInfo;
		tagNS_AcceptCommerce send;
		dwErrorCode = pGuild->GetGuildCommerce().GetCommerceBeginningInfo(pRole->GetID(), nLevel, sTaelInfo);
		if (dwErrorCode == E_Success)
		{
			send.nBeginningTael = sTaelInfo.nBeginningTael;
			SendMessage(&send, send.dwSize);
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// �Ͻ�����
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCompleteCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT32 nFund = 0;
	DWORD dwErrorCode = pGuild->GetGuildCommerce().CompleteCommerce(pRole, nFund);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_CompleteCommerce send;
		send.dwRoleID	= pRole->GetID();
		send.nFund		= nFund;
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAbandonCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().AbandonCommerce(pRole->GetID());
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// �������̼ν�״̬
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSwitchCommendation(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SwitchCommendation);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ȡ�ð�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().SwitchCommendation(pRole->GetID(), pRecv->bSwitchON);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_SwitchCommendation send;
		send.dwRoleID	= pRole->GetID();
		send.bSwitchON	= pRecv->bSwitchON;
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return E_Success;
}

DWORD PlayerSession::HandleGetGuildPurpose(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetGuildPurpose);
	if( P_VALID(pRecv) && P_VALID(pRecv->dwGuildID) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->GetGuildTenet(GetRole(),pRecv->dwGuildID);
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleGetGuildSmallInfo(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetAllGuildSmallInfo);
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->GetGuildRanking(GetRole());
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleApplyToJoinGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_WantToJoinGuild);
	if( P_VALID(pRecv) )
	{
		// �������ǰ�����жϽ�ɫ�����Ƿ��С����ɾ��ѡ���buff(bufferID��LEAVE_GUILD_BUFF_ID)
		Role* pRole = GetRole();
		if(P_VALID(pRole))
		{
			if (pRole->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
			{
				tagNS_WantToJoinGuild msg;
				msg.dwErrorCode = E_Guild_Can_Not_Join_Or_Create;
				pRole->SendMessage(&msg, msg.dwSize);
				return GT_INVALID;
			}
		}
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->ApplyToJoinGuild(GetRole(),pRecv->dwGuildID);
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleContactGuildLeader(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ContactGuildLeader);
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->ConstractGuildMaster(GetRole(),pRecv->dwGuildID);
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleGetGuildApplierBill(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetGuildApplierBill);
	Role * pRole = GetRole();
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			return pToolkit->GetApplyBill(pRole,pRole->GetGuildID());
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleApproveToJoinGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ApproveToJoinGuild);
	if( P_VALID(pRecv) )
	{
		Role * pRole = GetRole();
		Role * pApplier = g_roleMgr.GetRolePtrByID(pRecv->dwApplierID);
// 		tagRoleInfo * pRoleInfo;
		if( P_VALID(pRole) /*&& P_VALID(pApplier)*/ )
		{
		//	pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		//	tagNS_ApproveToJoinGuildTrans msg;
		//	DWORD dwGuildID = pRole->GetGuildID();
		//	msg.dwErrorCode = pRecv->dwErrorCode;
		//	if( P_VALID(dwGuildID) && P_VALID(pRoleInfo) )
		//	{
		//		Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
		//		if( P_VALID(pGuild) )
		//		{
		//			const tagGuild & guildInfo = pGuild->GetGuildAtt();
		//			if( guildInfo.dwLeaderRoleID == pRole->GetID() )
		//			{
		//				msg.dwGuildMasterID = guildInfo.dwLeaderRoleID;
		//				INT len = _tcslen(pRoleInfo->szRoleName);
		//				if( len > X_SHORT_NAME - 1 )
		//					len = X_SHORT_NAME - 1;
		//				_tcsncpy(msg.szMasterName,pRoleInfo->szRoleName,len);
		//				msg.szMasterName[len] = 0;

		//				pApplier->SendMessage(&msg,msg.dwSize);
		//				return E_Success;
		//			}
		//		}
		//	}

			// �������ǰ�����жϽ�ɫ�����Ƿ��С����ɾ��ѡ���buff(bufferID��LEAVE_GUILD_BUFF_ID)
			if(P_VALID(pApplier) && pRecv->dwErrorCode == 0)
			{
				if (pApplier->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
				{
					tagNS_RoleInJustLeaveGuildState msg;
					msg.dwErrorCode = E_Guild_Can_Not_Be_Joined;
					pRole->SendMessage(&msg, msg.dwSize);
					return GT_INVALID;
				}
			}

			ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
			if( P_VALID(pToolkit) && P_VALID(pRole) )
			{
				return pToolkit->ApproveToJoinGuild(pRole,pRecv->dwApplierID,pRecv->dwErrorCode == 0);
			}
		}
	}
	return GT_INVALID;
}


DWORD PlayerSession::HandleSearchGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_SearchGuild);
	Role * pRole = GetRole();
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			pRecv->szName[X_SHORT_NAME-1] = 0;
			tstring what(pRecv->szName);
			return pToolkit->SearchGuild(pRole,pRecv->dwSearchMode,what);
		}
	}
	return GT_INVALID;
}

//------------------------------------------------------------------------
// ����ս��
//------------------------------------------------------------------------
DWORD PlayerSession::HandleDeclareGuildWar(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_DeclareGuildWar);
	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �ͻ�����������ˣ��Է���һ����һ�¼�飬������Լ���ս���������
	if (pRecv->dwAttackGuildID == pRecv->dwDefenceGuildID)
	{
		return GT_INVALID;
	}

	Guild* pAttackGuild = g_guildMgr.GetGuild(pRecv->dwAttackGuildID);
	Guild* pDefenceGuild = g_guildMgr.GetGuild(pRecv->dwDefenceGuildID);
	if (!P_VALID(pAttackGuild))
	{
		return GT_INVALID;
	}

	tagNS_DeclareGuildWar send;

	// ����ս���ɲ�������
	if (!P_VALID(pDefenceGuild))
	{
		send.dwErrorCode = E_GuildDeclareWar_Guild_Not_Exist;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	tagGuildMember* pMaster = pAttackGuild->GetMemberMgr().GetMaster();
	if (!P_VALID(pMaster))
	{
		return GT_INVALID;
	}

	// ����ǲ��ǻ᳤
	if (pMaster->dwRoleID != pRole->GetID())
	{
		send.dwErrorCode = E_GuildDeclareWar_Not_Guild_Leader;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// ��鱳���Ƿ������µ��ߣ����ݶ�1w��ң���
	if (pAttackGuild->GetGuildAtt().nFund < DECLEAR_WAR_SILVER_COST)
	{
		send.dwErrorCode = E_GuildDeclareWar_Gold_Not_Enough;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// ��鱳���Ƿ������µ��ߣ����ݶ�����ս���ơ����ߣ�
	tagItem* pItem = pRole->GetItemMgr().GetItemByTypeID(DECLEAR_WAR_ITEM_TYPE_ID);
	if (!P_VALID(pItem))
	{
		send.dwErrorCode = E_GuildDeclareWar_No_Spec_Item;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	//����������ɵ���ս��ϵ�ǲ����Ѿ�������
	if (g_guildWarDeclareMgr.IsInGuildWarState(pRecv->dwAttackGuildID, pRecv->dwDefenceGuildID))
	{
		send.dwErrorCode = E_GuildDeclareWar_Declare_Exist;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// ��ս�������
	send.byTodayDeclareWarTimes = g_guildWarDeclareMgr.GetTodayDeclareWarTimes(pRecv->dwAttackGuildID);
	if (send.byTodayDeclareWarTimes >= MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY)
	{
		send.dwErrorCode = E_GuildDeclareWar_Declare_Times_Full;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	
	// ��ͬһ������ÿ����ս�������
	if (g_guildWarDeclareMgr.GetTodaySameGuildBeWarTimes(pRecv->dwAttackGuildID, pRecv->dwDefenceGuildID) >= MAX_SAME_GUILD_BE_DECLARE_WAR_TIMES)
	{
		send.dwErrorCode = E_GuildDeclareWar_Same_Guild_Full;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// ��ս�ɹ�, ���㲥"���Ĺ����Ѿ���XXX�������л�ս��"
	DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	DWORD   dwTmpNum = 100036;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwDefenceGuildID);
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pAttackGuild);
	
	// ��ս�ɹ�, ���㲥"���Ĺ����Ѿ���XXX�������л�ս��"
	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	dwTmpNum = 100036;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwAttackGuildID);
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pDefenceGuild);

	// ��ս�ɹ�, ���緢�㲥"XXX�����Ѿ���XXX����������л�ս��"
	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	dwTmpNum = 100035;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwAttackGuildID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwDefenceGuildID);
	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);

	// ��ս�ɹ����۳�����ս��ս���ߺ���ս�ʽ�
	pAttackGuild->DecGuildFund(pRole->GetID(), DECLEAR_WAR_SILVER_COST, ELCID_Guild_DeclareWar);
	pRole->GetItemMgr().DelFromBag(pItem->n64Serial, ELCID_Guild_DeclareWar, 1);

	// ���������ɷ�����ս�ɹ���Ϣ
	g_guildWarDeclareMgr.AddGuildWarDeclare2Map(pRecv->dwAttackGuildID, pRecv->dwDefenceGuildID);
	send.dwErrorCode = E_GuildDeclareWar_Success;
	send.byTodayDeclareWarTimes = g_guildWarDeclareMgr.GetTodayDeclareWarTimes(pRecv->dwAttackGuildID);
	send.dwDefenceGuildID = pRecv->dwDefenceGuildID;
	send.dwAttackGuildID = pRecv->dwAttackGuildID;
	pAttackGuild->SendGuildMsg(&send, send.dwSize);
	pDefenceGuild->SendGuildMsg(&send, send.dwSize);

	return E_Success;
}

// ȡͬ���б�
DWORD PlayerSession::HandleGetAllianceList(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetAllianceList);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (P_VALID(CGodMiracleMgr::GetInst()))
	{
		CGodMiracleMgr::GetInst()->SendAllianceList2Client(pRecv->eGodID, pRole);
		return E_Success;
	}
	return GT_INVALID;
}

// ս��ͬ�˹��ᱨ��
DWORD PlayerSession::HandleApplyForAlliance(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ApplyForAlliance);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (P_VALID(CGodMiracleMgr::GetInst()))
	{
		tagNS_ApplyForAlliance send;
		send.dwGodID = pRecv->dwGodID;

		// �жϽ�ɫ�Ƿ��ǹ���᳤
		Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
		ASSERT(P_VALID(pGuild));
		if (!P_VALID(pGuild))
			return E_SystemError;

		if ( EGMP_HuiZhang != pGuild->GetMemberMgr().GetMember(pRole->GetID())->eGuildPos)
			send.dwErrorCode = EEGB_ApplyNoGuildMaster;
		else
			send.dwErrorCode = CGodMiracleMgr::GetInst()->Add2AllianceGuildMap(pRecv->dwGodID, pRole->GetGuildID(), pRecv->eGuildCapType, FALSE);
		pRole->SendMessage(&send, send.dwSize);
	}
	return E_Success;
}

// ȷ��ͬ��
DWORD PlayerSession::HandleConfirmAllianceGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ConfirmAllianceGuild);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (P_VALID(CGodMiracleMgr::GetInst()) && P_VALID(CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->dwGodID)))
	{
		tagNS_ConfirmAllianceGuild send;
		send.dwGodID = pRecv->dwGodID;
		send.dwGuildID = pRecv->dwGuildID;
		send.dwErrorCode = CGodMiracleMgr::GetInst()->ConfirmAllianceGuild(pRecv->dwGodID, pRecv->dwGuildID);
		send.eGuildCapType = CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->dwGodID)->GetAllianceCapType(pRecv->dwGuildID);
		pRole->SendMessage(&send, send.dwSize);
	}

	return E_Success;
}

DWORD PlayerSession::HandleGuildWarOccupyedNum(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GuildOccupyedNum);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_GuildOccupyedNum backmsg;

	DWORD dwGuildID = pRole->GetGuildID() ;
	if( !P_VALID(dwGuildID) )
	{
		backmsg.bAlreadyTake = FALSE;
		backmsg.nNum			= 0;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
	{
		backmsg.bAlreadyTake = FALSE;
		backmsg.nNum			= 0;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	CGodMiracleMgr* pGod = CGodMiracleMgr::GetInst();
	if( !P_VALID(pGod) )
	{
		backmsg.bAlreadyTake = FALSE;
		backmsg.nNum			= 0;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	backmsg.nNum = pGod->GetGuildDefendGodMiracle(dwGuildID);
	backmsg.bAlreadyTake = pRole->HasTakenGuildDailyReward() ? true : false;
	SendMessage(&backmsg,backmsg.dwSize);
	return E_Success;
}

DWORD PlayerSession::HandleGuildWarTakeWarReword(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_TakeGuildWarReword);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_TakeGuildWarReword backmsg;

	if( pRole->HasTakenGuildDailyReward() )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_AlreadyTaken;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	// ��õ�ͼ
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(pRecv->dwNPCID);
	if( !P_VALID(pNPC) ) 
	{
		backmsg.dwErrorCode = EGEC_NPC_NotExist;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_NPC_NotExist;
	}

	if( FALSE == pNPC->CheckNPCTalkDistance(pRole) )
	{
		backmsg.dwErrorCode = EGEC_NPC_DisFar;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_NPC_DisFar;
	}

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GuildWarRewordNPC) )
	{
		backmsg.dwErrorCode = EGEC_NPC_TypeMismatch;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_NPC_TypeMismatch;
	}


	DWORD dwGuildID = pRole->GetGuildID() ;
	if( !P_VALID(dwGuildID) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	CGodMiracleMgr* pGod = CGodMiracleMgr::GetInst();
	if( !P_VALID(pGod) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	if( pRole->GetLevel() < 40 )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_LevelLimit;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_DailyReward_LevelLimit;
	}

	const tagGuild & guildAtt = pGuild->GetGuildAtt();
	GuildMember & guildMem = pGuild->GetMemberMgr();
	tagGuildMember * pMem = NULL;
	pMem = guildMem.GetMember(pRole->GetID());

	if( !P_VALID(pMem) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}
	if( EGMP_Start > pMem->eGuildPos || pMem->eGuildPos > EGMP_End )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	const tagGuildDailyRewardProto * pReward = g_attRes.GetGuildDailyRewardProto(pMem->eGuildPos,pGod->GetGuildDefendGodMiracle(dwGuildID));
	if( !P_VALID(pReward) )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_CannotFindProto;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_DailyReward_CannotFindProto;
	}

	GuildPos & guildPos = pGuild->GetGuildPos();

	if( guildAtt.wGuildPosTakeRewardTimes[pMem->eGuildPos] >= guildPos.GetPosNum(pMem->eGuildPos) )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_GuildPosLimit;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	if( pRole->GetItemMgr().GetBagFreeSize() < 1 )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_BagFull;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_DailyReward_BagFull;
	}

	if( P_VALID(pReward->dwBuffID) )
	{
		pRole->TryAddBuffToMyself(pReward->dwBuffID);
	}

	tagItem * pItem = ItemCreator::Create(EICM_GuildDailyReward,  GT_INVALID, pReward->dwItemTypeID );
	if( !P_VALID(pItem) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}
	pRole->GetItemMgr().Add2Bag(pItem, ELCID_GuildDailyReward, TRUE);
	// ���浽���ݿ�
	const_cast<tagGuild&>(guildAtt).wGuildPosTakeRewardTimes[pMem->eGuildPos] ++;
	pGuild->SynGuildAtt2DB();
	pRole->ResetGuildDailyRewardTime();

	backmsg.dwErrorCode = E_Success;
	SendMessage(&backmsg,backmsg.dwSize);

	return E_Success;
}




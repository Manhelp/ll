//-----------------------------------------------------------------------------
//!\file reputation_handler.cpp
//!\author xlguo
//!
//!\date 2009-02-19
//! last 
//!
//!\brief ������Ϣ�������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "player_session.h"
#include "guild_mgr.h"
#include "role_mgr.h"
#include "role.h"
#include "guild.h"
#include "../WorldDefine/msg_reputation.h"
#include "../WorldDefine/msg_guild.h"

DWORD PlayerSession::HandleGetRoleClanData(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetReputation);
	
	//Role* pRole = GetOtherInMap(pRecv->dwRoleID);
	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pRole))
	{
		tagNS_GetOfflineReputation send;
		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
    INT  nGuildExploit = 0,nGuildContribute = 0;
	Guild* pGuild  = g_guildMgr.GetGuild(pRole->GetGuildID());
	if ( P_VALID(pGuild) )
	{
		tagGuildMember* pGuildMember = pGuild->GetMember(pRole->GetID());
		if( P_VALID(pGuildMember) )
		{
			nGuildExploit     = pGuildMember->nExploit ;
			nGuildContribute  = pGuildMember->nContribution;
		}
		else
		{
			nGuildContribute  = 0;
		    nGuildExploit     = 0;
		}
		
	}
	else
	{
		nGuildContribute  = 0;
		nGuildExploit     = 0;
	}
	//INT nTotal    =    pRole->GetScriptData(110);   //����������
	//INT nWinCount =    pRole->GetScriptData(111);   //Ӯ�ô���

 //   INT nRate     =    0 ;
	//if ( 0 == nTotal )
	//{
	//	nRate = 0;
	//}
	//else
	//{
 //       nRate = (nWinCount*100/nTotal);
	//}
	tagNS_GetReputation send;
	send.dwRoleID = pRecv->dwRoleID;
	send.ReputeData.nEquip    =    pRole->GetStrength().nEquipValue;
    send.ReputeData.nFairy    =    pRole->GetStrength().nFairy;
// 	send.ReputeData.nFriend   =    pRole->GetStrength().nFriendship;
	send.ReputeData.nGuildContribute  =  nGuildContribute;
	send.ReputeData.nGuildExploit     =  nGuildExploit;
	send.ReputeData.nLevel            =  pRole->GetStrength().nLevel;
	//send.ReputeData.nPVPTimes         =  nTotal;
	send.ReputeData.nPVPTimes         =  pRole->GetScriptData(24);		//����������	
	send.ReputeData.nRate             =  0;								//����������ʱ����
// 	send.ReputeData.nTitle            =  pRole->GetStrength().nTitle;
	send.ReputeData.nTotalStrength    =  pRole->GetTotalStrength();
	send.ReputeData.nWuJi             =  pRole->GetAttValue(ERA_WuJi);
	send.ReputeData.nWuXun            =  pRole->GetAttValue(ERA_WuXun);
	send.ReputeData.nWinTimes		  =  pRole->GetScriptData(25);		//��ʤ�ĳ�����	
	send.ReputeData.nFlowerNum		  =  pRole->GetAttValue(ERA_FlowerNum);	//�ʻ���
	send.ReputeData.nEggNum			  =  pRole->GetAttValue(ERA_EggsNum);	//������
	SendMessage(&send, send.dwSize);

	return 0;
}

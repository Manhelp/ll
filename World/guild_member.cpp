//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_member.cpp
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ���ɳ�Ա
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_member.h"
#include "role.h"
#include "guild.h"
#include "guild_mgr.h"
#include "role_mgr.h"
#include "role_mgr.h"
#include "db_session.h"
#include "att_res.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/buff_define.h"
#include "../ServerDefine/msg_guild.h"
#include "title_mgr.h"
//-------------------------------------------------------------------------------
// ����&����
//-------------------------------------------------------------------------------
GuildMember::GuildMember():m_MemberChangeFlag(FALSE)
{
}

GuildMember::~GuildMember()
{
	tagGuildMember *pMember;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		SAFE_DEL(pMember);
	}

	m_mapMember.Clear();
}

//-------------------------------------------------------------------------------
// ��ӳ�Ա
//-------------------------------------------------------------------------------
VOID GuildMember::AddMember(DWORD dwInviteeID, EGuildMemberPos ePos, BOOL bSave2DB/* = TRUE*/)
{
	// ����Ƿ��Ѽ��� -- LoongDB˲��
	ASSERT(!m_mapMember.IsExist(dwInviteeID));
	
	tagGuildMember *pNewMember = new tagGuildMember(dwInviteeID, ePos);
	if(!P_VALID(pNewMember))
	{
		ASSERT(0);
		return;
	}

	m_mapMember.Add(dwInviteeID, pNewMember);

	if(bSave2DB)
	{
		tagNDBC_JoinGuild send;
		send.sGuildMemInfo.dwRoleID		= dwInviteeID;
		send.sGuildMemInfo.dwGuildID	= m_dwGuildID;
		send.sGuildMemInfo.n8GuildPos	= ePos;

		g_dbSession.Send(&send, send.dwSize);
	}

	// �����а��ɳ�Ա�������������仯��Ϣ
	SyncOnlineNum2AllGuildMumber();

	m_MemberChangeFlag = TRUE;
}

//-------------------------------------------------------------------------------
// �����Ա
//-------------------------------------------------------------------------------
VOID GuildMember::LoadMember(const tagGuildMember& sGuildMember)
{
	if (m_mapMember.IsExist(sGuildMember.dwRoleID))
	{
		return;
	}

	tagGuildMember *pNewMember = new tagGuildMember(sGuildMember);
	if(!P_VALID(pNewMember))
	{
		ASSERT(0);
		return;
	}

	m_mapMember.Add(sGuildMember.dwRoleID, pNewMember);
}

//-------------------------------------------------------------------------------
// ɾ����Ա
//-------------------------------------------------------------------------------
VOID GuildMember::RemoveMember(DWORD dwRoleID, BOOL bSave2DB/* = TRUE*/)
{
	tagGuildMember *pMember = m_mapMember.Peek(dwRoleID);
	if(!P_VALID(pMember))
	{
		return;
	}

	m_mapMember.Erase(dwRoleID);

	if(bSave2DB)
	{
		tagNDBC_LeaveGuild send;
		send.dwRoleID	= dwRoleID;

		g_dbSession.Send(&send, send.dwSize);
	}

	SAFE_DEL(pMember);

	// ����ɾ���İ��ɳ�Աͬ�����߰��������仯��Ϣ
	Role* pRemovedRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRemovedRole))
	{
		pRemovedRole->SyncOnlineNum2Role(ESOR_Online_GuildMember_Num, 0);
	}

	// �����а��ɳ�Ա�������������仯��Ϣ
	SyncOnlineNum2AllGuildMumber();

	m_MemberChangeFlag = TRUE;
}

//-------------------------------------------------------------------------------
// ���ó�Ա�İ���ְλ
//-------------------------------------------------------------------------------
VOID GuildMember::SetGuildPos(tagGuildMember *pMember, EGuildMemberPos ePos, BOOL bSave2DB/* = TRUE*/)
{
	ASSERT(P_VALID(pMember));
	ASSERT(::IsGuildPosValid(ePos));

	pMember->eGuildPos = ePos;

	if(bSave2DB)
	{
		tagNDBC_ChangeGuildPos send;
		send.dwRoleID	= pMember->dwRoleID;
		send.n8NewPos	= ePos;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------
// �����ڹ㲥
//-------------------------------------------------------------------------------
VOID GuildMember::SendGuildMsg(LPVOID pMsg, DWORD dwSize)
{
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if(!P_VALID(pRole))
		{
			continue;
		}

		pRole->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------
// �����ڼ���9�ո�㲥
//-------------------------------------------------------------------------------
VOID   GuildMember::SendChangeGuildNameMsg(LPVOID pMsg, DWORD dwSize)
{
	
    Guild* pGuild = g_guildMgr.GetGuild(m_dwGuildID);
	if( !P_VALID(pGuild) )
	{
		return ;
	}
	INT32 nNumTCHAR = pGuild->GetGuildAtt().strName.size();
	ASSERT(nNumTCHAR > 0);

	INT32 nMsgSz = sizeof(tagNS_GetGuildName) + nNumTCHAR * sizeof(TCHAR);

	MCREATE_MSG(pSend, nMsgSz, NS_GetGuildName);
	pSend->dwGuildID	= pGuild->GetGuildAtt().dwID;
	_tcscpy(pSend->szGuildName,pGuild->GetGuildAtt().strName.c_str());
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if(!P_VALID(pRole))
		{
			continue;
		}
		else
		{
			Map* pMap = pRole->GetMap();
			if ( P_VALID(pMap) )
			{
				g_roleMgr.SendWorldMsg(pSend,pSend->dwSize);
			}
		}
	}
	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------
// �������а��ɳ�Ա��Ϣ
//-------------------------------------------------------------------------------
VOID GuildMember::SendAllMembers2Client(Role *p2Role)
{
	INT16 n16Num = m_mapMember.Size() - 1;	// ��ȥ�Լ�
	ASSERT(n16Num > 0);

	// �����ڴ�
	INT32 nMsgSz = sizeof(tagNS_GuildGetAllMembers) - sizeof(BYTE) + n16Num * sizeof(tagGuildMemInfo);
	MCREATE_MSG(pSend, nMsgSz, NS_GuildGetAllMembers);

	pSend->n16Num = 0;
	MTRANS_POINTER(pMemberInfo, pSend->byData, tagGuildMemInfo);
	
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		if(pMember->dwRoleID == p2Role->GetID())
		{
			continue;
		}

		pMemberInfo[pSend->n16Num].dwRoleID		= pMember->dwRoleID;
		pMemberInfo[pSend->n16Num].n8GuildPos	= pMember->eGuildPos;
		pMemberInfo[pSend->n16Num].nCurContrib	= pMember->nContribution;
		
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pMember->dwRoleID);
		if(P_VALID(pRoleInfo))
		{
			pMemberInfo[pSend->n16Num].byLevel			= pRoleInfo->byLevel;
			pMemberInfo[pSend->n16Num].bySex			= pRoleInfo->bySex;
			pMemberInfo[pSend->n16Num].byClass			= pRoleInfo->byClass;		//
			pMemberInfo[pSend->n16Num].dwTimeLastLogout	= pRoleInfo->dwTimeLastLogout;
			pMemberInfo[pSend->n16Num].bOnline			= pRoleInfo->bOnline;
		}
		
		// �����߼���Ա�����߼�������
		ASSERT(P_VALID(pRoleInfo));

		++pSend->n16Num;
	}

	if(n16Num != pSend->n16Num)
	{
		ASSERT(n16Num == pSend->n16Num);
		IMSG(_T("\n\n\n\tCaution:\n\tGuild<id:%u> members maybe have problem, please check db!\n\n"), p2Role->GetGuildID());
	}

	p2Role->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------------------------------
// ���ð��ɲֿ����Ȩ��
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::SetGuildWarePrivilege( DWORD dwRoleID, BOOL bCanUse, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	if (bSave2DB && (pMember->bUseGuildWare != bCanUse))
	{
		pMember->bUseGuildWare = bCanUse;

		// ֪ͨ���ݿ�
		tagNDBC_GuildWarePri send;
		send.dwRoleID	= dwRoleID;
		send.bEnable	= bCanUse;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// ���ɹ���
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::IncMemberContribution( DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	if (nContribution <= 0)
		return;

	// ȡ�ó�Ա����
	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	// ���Ա仯
	pMember->nContribution += nContribution;
	if (pMember->nContribution > MAX_GUILDMEM_CURCONTRIB)
	{
		pMember->nContribution = MAX_GUILDMEM_CURCONTRIB;
	}
	pMember->nTotalContribution += nContribution;
	if (pMember->nTotalContribution > MAX_GUILDMEM_TOTALCONTRIB)
	{
		pMember->nTotalContribution = MAX_GUILDMEM_TOTALCONTRIB;
	}
	
	// ���浽���ݿ�
	if (bSave2DB)
	{
		tagNDBC_ChangeContrib send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ͬ�����ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildContribution send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;
		pRole->SendMessage(&send, send.dwSize);
	}
}

VOID GuildMember::DecMemberContribution( DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	if (nContribution <= 0)
		return;

	// ȡ�ó�Ա����
	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	// ���Ա仯
	pMember->nContribution -= nContribution;
	if (pMember->nContribution < 0)
	{
		pMember->nContribution = 0;
	}

	// ���浽���ݿ�
	if (bSave2DB)
	{
		tagNDBC_ChangeContrib send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ͬ�����ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildContribution send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;
		pRole->SendMessage(&send, send.dwSize);
	}
}

VOID GuildMember::SetMemberExploit(DWORD dwRoleID, INT32 nExploit, BOOL bSave2DB /* = TRUE */)
{
	if (!GT_VALID(dwRoleID))
		return;

	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;
	
	pMember->nExploit += nExploit;
		
	if (pMember->nExploit > MAX_GUILDMEM_EXPLOIT)
		pMember->nExploit = MAX_GUILDMEM_EXPLOIT;
	if (pMember->nExploit < 0)
		pMember->nExploit = 0;

	if (bSave2DB)
	{
		// ֪ͨ���ݿ�
		tagNDBC_ChangeExploit send;
		send.dwRoleID	= dwRoleID;
		send.nExploit	= pMember->nExploit;

		g_dbSession.Send(&send, send.dwSize);
	}

	// ͬ�����ͻ���
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildExploit send;
		send.dwRoleID			= dwRoleID;
		send.nExploit		= pMember->nExploit;
		pRole->SendMessage(&send, send.dwSize);
		// Jason 2010-3-4 v1.4.0���ɹ�ѫ�ƺ�
		pRole->GetTitleMgr()->SigEvent( ETE_GUILD_EXPLOIT, GT_INVALID ,pMember->nExploit );
	}

}

//-------------------------------------------------------------------------------------------------------
// �����а��ɳ�Ա���BUFF
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::AddBuffToAllOnLineMember(DWORD dwBuffID)
{
	const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffID);
	if ( !P_VALID(pBuffProto)) return ;

	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role* pRole = NULL;
		pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if ( NULL != pRole)
			pRole->TryAddBuff((Unit*)pRole, pBuffProto, NULL, NULL, NULL);
	}
}

//-------------------------------------------------------------------------------------------------------
// �Ƴ����а��ɳ�Ա��BUFFF
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::RemoveBuffFromAllOnLineMember(DWORD dwBuffID)
{
	const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffID);
	if ( !P_VALID(pBuffProto)) return ;

	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role* pRole = NULL;
		pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if ( NULL != pRole)
			pRole->RemoveBuff(Buff::GetIDFromTypeID(dwBuffID), TRUE);
	}
}

/// �������й����Ա��State
VOID GuildMember::UnsetStateOfAllGuildMember(EState eState)
{
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role* pRole = NULL;
		pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if ( NULL != pRole)
			pRole->UnsetState(eState);
	}
}

//-------------------------------------------------------------------------------------------------------
// �õ�����
//-------------------------------------------------------------------------------------------------------
tagGuildMember* GuildMember::GetMaster()
{
	tagGuildMember* pGuildMember = NULL;
	TMap<DWORD, tagGuildMember*>::TMapIterator itMember = m_mapMember.Begin();
	while(m_mapMember.PeekNext(itMember, pGuildMember))
	{
		if ( EGMP_HuiZhang == pGuildMember->eGuildPos )
			return pGuildMember;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------------
// ǿ�ư������г�Ա���һ�νű�����
//-------------------------------------------------------------------------------------------------------
VOID   GuildMember::CheckAllGuildMemberIsInGodArea()
{
	tagGuildMember* pGuildMember = NULL;
	TMap<DWORD, tagGuildMember*>::TMapIterator itMember = m_mapMember.Begin();
	while(m_mapMember.PeekNext(itMember, pGuildMember))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(pGuildMember->dwRoleID);
		if ( P_VALID(pRole))
		{
			pRole->GetMap()->CheckArea(pRole);
		}
	}
}

INT32 GuildMember::GetOnlineGuildMemberCnt()
{
	tagGuildMember* pGuildMember = NULL;
	UINT16 nOnlineNum = 0;

	MapGuildMember::TMapIterator& iterator = m_mapMember.Begin();
	while (m_mapMember.PeekNext(iterator, pGuildMember))
	{
		if (!P_VALID(pGuildMember))
		{
			continue;
		}
		Role* pRole = g_roleMgr.GetRolePtrByID(pGuildMember->dwRoleID);
		if (P_VALID(pRole))
		{
			nOnlineNum++;
		}
	}
	return nOnlineNum;
}

// �����а��ɳ�Ա�������������仯��Ϣ
VOID GuildMember::SyncOnlineNum2AllGuildMumber(BOOL bLogout)
{
	tagGuildMember* pGuildMember = NULL;
	UINT16 nOnlineNum = GetOnlineGuildMemberCnt();
	MapGuildMember::TMapIterator& iterator = m_mapMember.Begin();

	while (m_mapMember.PeekNext(iterator, pGuildMember))
	{
		if (!P_VALID(pGuildMember))
		{
			continue;
		}
		Role* pRole = g_roleMgr.GetRolePtrByID(pGuildMember->dwRoleID);
		if (P_VALID(pRole))
		{
			// ͬ�����������������仯��֪ͨ�ͻ���
			pRole->SyncOnlineNum2Role(ESOR_Online_GuildMember_Num, (bLogout?(nOnlineNum-1):nOnlineNum));
		}
	}
}

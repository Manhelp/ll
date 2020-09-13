/* Copyright (c) 2010  ����������޹�˾
*  All rights reserve
*
*  �ļ�����:  group_chat.cpp
*  ժ    Ҫ:  ��ҿ�����Ⱥ�ĵ�����ϵͳ
*  ��ǰ�汾:  1.0
*  �������:  2010-6-4
*  ��    ��:  lamoair
*  
*/


#include "stdafx.h"
#include "../WorldDefine/msg_GroupChat.h"

#include "role.h"
#include "role_mgr.h"
#include "group_chat.h"

group_chat::group_chat():m_dwGroupChatMasterId(0),m_dwRoleNum(0),m_dwGroupChatId(0)
{
	memset(&m_szGroupChatName,0,MAX_GROUPCHAT_LEN*sizeof(TCHAR));
	m_mapGroupMember.Clear();
}

group_chat::~group_chat()
{
	 m_mapGroupMember.Clear();
}

//---------------------------------------------------------------------
//����һ��Ⱥ���쵥Ԫ
//---------------------------------------------------------------------
VOID group_chat::Init(const TCHAR* pGroupChatName,DWORD dwGroupChatMasterId,DWORD dwGroupChatId)
{
	if (!P_VALID(pGroupChatName) )
	{
		return ;
	}
	m_dwGroupChatId        = dwGroupChatId;
    m_dwGroupChatMasterId  = dwGroupChatMasterId;
	_tcscpy_s(m_szGroupChatName,MAX_GROUPCHAT_LEN,pGroupChatName);
    
}

//---------------------------------------------------------------------
//��ӽ�ɫ������Ⱥ��
//---------------------------------------------------------------------
DWORD   group_chat::AddRoleToGroup(Role* pRole)
{

    if (!P_VALID(pRole))
    {
		return GRC_AddRoleNotExist;
    }
	DWORD dwRoleId = pRole->GetID();
	if ( m_mapGroupMember.IsExist(dwRoleId))
	{
          return GRC_AlreadyInGroupChat;
	}
	else
	{
		m_mapGroupMember.Add(dwRoleId,pRole);
        m_listGroupMemberId.PushBack(dwRoleId);
		pRole->AddGroupChat(m_dwGroupChatId,this);
		m_dwRoleNum++;
	}
	
    return 0;
}

//---------------------------------------------------------------------
//T����ɫ������Ⱥ
//---------------------------------------------------------------------
DWORD   group_chat::KickRoleFrouGroup(DWORD dwRoleId)
{
	if ( !m_mapGroupMember.IsExist(dwRoleId))
	{
		return  GRC_LoginOutGroupRoleNotExist;
	}
	m_mapGroupMember.Erase(dwRoleId);
	m_listGroupMemberId.Erase(dwRoleId);
	m_dwRoleNum--;

	return E_Success;
}

VOID    group_chat::RoleDeleteGroup()
{
	std::list<Role*> listGroupChatRole;
	m_mapGroupMember.ExportAllValue(listGroupChatRole);
	std::list<Role*>::iterator it = listGroupChatRole.begin();
	for (; it != listGroupChatRole.end();it++)
	{

		if ( P_VALID(*it) )
		{
			(*it)->DeleteGroupChat(m_dwGroupChatId);
		}
	}
}

//---------------------------------------------------------------------
//�˳�һ��Ⱥ
//---------------------------------------------------------------------
DWORD   group_chat::LoginOutGroup(Role* pRole)
{
    if (!P_VALID(pRole))
    {
		return GRC_LoginOutGroupRoleNotExist ;
    }
	DWORD dwRoleId  = pRole->GetID();
	if ( !m_mapGroupMember.IsExist(dwRoleId))
	{
		return  GRC_LoginOutGroupRoleNotExist;
	}
	pRole->DeleteGroupChat(m_dwGroupChatId);
	m_mapGroupMember.Erase(dwRoleId);
	m_listGroupMemberId.Erase(dwRoleId);
	m_dwRoleNum--;
	return E_Success;
}

BOOL   group_chat::GetGroupChatRole(DWORD dwRoleId)
{
	 if (m_mapGroupMember.IsExist(dwRoleId))
	 {
		 return TRUE;
	 }
	 else
	 {
		 return FALSE;
	 }
}

VOID  group_chat::SendGroupMsg(VOID* pMsg,DWORD dwSize)
{
	if ( P_VALID(pMsg) )
	{
		std::list<Role*> listGroupChatRole;
		m_mapGroupMember.ExportAllValue(listGroupChatRole);
		std::list<Role*>::iterator it = listGroupChatRole.begin();
		for (; it != listGroupChatRole.end();it++)
		{
			
			if ( P_VALID(*it) )
			{
				(*it)->SendMessage(pMsg,dwSize);
			}
		}
	}
}



////////////////////////////////////////////////////
GroupChatMgr g_GroupChatMgr;



//---------------------------------------------------------------------
// ��ɫһ����ɫ����Ⱥ
//---------------------------------------------------------------------
GroupChatMgr::GroupChatMgr():m_dwDefGroupChatNum(0), m_dwGroupChatId(1)
{ 
	 
}

GroupChatMgr::~GroupChatMgr()
{

}

group_chat*  GroupChatMgr::CreateGroupChat(DWORD dwGroupChatID)
{
    group_chat* pGroupChat = new group_chat;
	if ( P_VALID(dwGroupChatID) )
	{
		m_mapGroupChat.Add(dwGroupChatID,(group_chat*)pGroupChat);
		m_dwDefGroupChatNum++;
		return pGroupChat;
	}
	else
	{
		return NULL;
	}
}

VOID  GroupChatMgr::DeleteGroupChat(DWORD dwGroupChatID)
{
      group_chat* pGroupChat = m_mapGroupChat.Peek(dwGroupChatID);
	  if( P_VALID(pGroupChat) )
	  {
		  SAFE_DEL(pGroupChat);
		  m_mapGroupChat.Erase(dwGroupChatID);
		  m_dwDefGroupChatNum--;
	  }

}

//---------------------------------------------------------------------
// ��ȡһ����ɫ����Ⱥ
//---------------------------------------------------------------------
group_chat* GroupChatMgr::GetGroupChat(DWORD dwGroupChatID)
{
    group_chat* pGroupChat = m_mapGroupChat.Peek(dwGroupChatID);
	if ( P_VALID(pGroupChat) )
	{
		return pGroupChat ;
	}
	else
		return NULL;
}

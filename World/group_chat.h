/* Copyright (c) 2010  ����������޹�˾
*  All rights reserve
*
*  �ļ�����:  group_chat.h
*  ժ    Ҫ:  ��ҿ�����Ⱥ�ĵ�����ϵͳ
*  ��ǰ�汾:  1.0
*  �������:  2010-6-4
*  ��    ��:  lamoair
*  
*/

#pragma once




class group_chat
{
public:
	//---------------------------------------------------------------------
	// ���캯�� �� ��������
	//---------------------------------------------------------------------
	group_chat();
	~group_chat();
	
	VOID Init(const TCHAR* pGroupChatName,DWORD dwGroupChatMasterId,DWORD dwGroupChatId);

	//---------------------------------------------------------------------
	//��ӽ�ɫ������Ⱥ��
	//---------------------------------------------------------------------
	DWORD   AddRoleToGroup(Role* pRole);

	//---------------------------------------------------------------------
	//T����ɫ������Ⱥ
	//---------------------------------------------------------------------
    DWORD   KickRoleFrouGroup(DWORD dwRoleId);
	//---------------------------------------------------------------------
	//�˳�һ��Ⱥ
	//---------------------------------------------------------------------
    DWORD   LoginOutGroup(Role* pRole);
  
	VOID    RoleDeleteGroup();
	//---------------------------------------------------------------------
	// ������Ϣ���ݸ�����Ⱥ��Ա
	//---------------------------------------------------------------------
	VOID   SendGroupMsg(VOID* pMsg,DWORD dwSize);

    TSafeList<DWORD>&  GetListGroupMemberId()
	{
		 return m_listGroupMemberId;
	}

	DWORD  GetGroupMemberNum()
	{
		return m_mapGroupMember.Size();
	}

    BOOL   GetGroupChatRole(DWORD dwRoleId);

	DWORD  GetGroupChatId()const {  return m_dwGroupChatId; }

	DWORD  GetGroupChatMasterId()const  { return m_dwGroupChatMasterId ;}
	 
	const TCHAR*  GetGroupChatName()const {  return m_szGroupChatName; }

	template<typename UnitOperation>
	VOID ForEachRoleInGroupChat( UnitOperation oper );
	
private:

	TSafeMap<DWORD,Role*>                m_mapGroupMember;
	TSafeList<DWORD>                     m_listGroupMemberId;
	DWORD                                m_dwGroupChatMasterId;
	DWORD                                m_dwRoleNum;
	TObjRef<Util>                        m_Util;
	TCHAR                                m_szGroupChatName[MAX_GROUPCHAT_LEN];
	DWORD                                m_dwGroupChatId; 
};

template<typename UnitOperation>
VOID group_chat::ForEachRoleInGroupChat( UnitOperation oper )
{
	std::list<Role*> listGroupChatRole;
	m_mapGroupMember.ExportAllValue(listGroupChatRole);
	std::list<Role*>::iterator it = listGroupChatRole.begin();
	for (; it != listGroupChatRole.end();it++)
	{
		if ( P_VALID(*it) )
		{
			oper(*it);
		}
	}
}

class GroupChatMgr
{
public:
	//---------------------------------------------------------------------
	// ���캯�� �� ��������
	//---------------------------------------------------------------------
	GroupChatMgr();
	~GroupChatMgr();
    
	//---------------------------------------------------------------------
	// ��ȡһ����ɫ����Ⱥ
	//---------------------------------------------------------------------
    group_chat* GetGroupChat(DWORD dwGroupChatID) ;

	//---------------------------------------------------------------------
	// ��ɫһ����ɫ����Ⱥ
	//---------------------------------------------------------------------
	group_chat* CreateGroupChat(DWORD dwGroupChatID);
	VOID  DeleteGroupChat(DWORD dwGroupChatID);
	DWORD GetDefGroupChatNum()const  { return  m_dwDefGroupChatNum ;}

	DWORD CreateGroupChatId(){  return m_dwGroupChatId++; }

private:
	TSafeMap<DWORD,group_chat*>               m_mapGroupChat;     //����Ⱥ����
	volatile DWORD                            m_dwDefGroupChatNum; //����Ⱥ���ܸ���
    volatile DWORD                            m_dwGroupChatId;

};


extern  GroupChatMgr g_GroupChatMgr;
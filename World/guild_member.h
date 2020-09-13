//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_member.h
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: ���ɳ�Ա
//-------------------------------------------------------------------------------
#pragma once

struct	tagGuildMember;
enum	EGuildMemberPos;
enum	EState;
//-------------------------------------------------------------------------------
typedef TMap<DWORD, tagGuildMember*>		MapGuildMember;

class GuildMember
{
public:
	GuildMember();
	~GuildMember();

	VOID	Init(DWORD dwGuildID);

	VOID	AddMember(DWORD dwInviteeID, EGuildMemberPos ePos, BOOL bSave2DB = TRUE);
	VOID	LoadMember(const tagGuildMember& sGuildMember);
	VOID	RemoveMember(DWORD dwRoleID, BOOL bSave2DB = TRUE);

	INT32	GetNumber();
	MapGuildMember& GetMemberMap();
	
	VOID	SendGuildMsg(LPVOID pMsg, DWORD dwSize);
	VOID    SendChangeGuildNameMsg(LPVOID pMsg, DWORD dwSize);
	VOID	SendAllMembers2Client(Role *p2Role);

	tagGuildMember* GetMember(DWORD dwRoleID);

	BOOL	IsExist(DWORD dwRoleID);
	
	/// �����а��ɳ�Ա���BUFF
	VOID	AddBuffToAllOnLineMember(DWORD dwBuffID);

	/// �Ƴ����а��ɳ�Ա��BUFFF
	VOID    RemoveBuffFromAllOnLineMember(DWORD dwBuffID);
	
	/// �������й����Ա��State
	VOID    UnsetStateOfAllGuildMember(EState eState);

	/// �õ����ɳ�Ա����
	MapGuildMember& GetMapGuildMember() { return m_mapMember; }

	// �õ����ɳ�Ա�Ƿ�仯�ı�־
	BOOL	GetGuildMemberChangeFlag() { return m_MemberChangeFlag; }
	VOID	SetGuildMemberChangeFlag(BOOL dwFalg) {m_MemberChangeFlag  = dwFalg; }

	// �õ�����
	tagGuildMember* GetMaster();

	// ������еİ��ɳ�Ա������һ�μ������
	VOID   CheckAllGuildMemberIsInGodArea();
public:
	// ��Ա�������
	VOID	SetGuildPos(tagGuildMember *pMember, EGuildMemberPos ePos, BOOL bSave2DB = TRUE);
	VOID	SetGuildWarePrivilege(DWORD dwRoleID, BOOL bCanUse, BOOL bSave2DB = TRUE);
	VOID	IncMemberContribution(DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB = TRUE);
	VOID	DecMemberContribution(DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB = TRUE);
	VOID	SetMemberExploit(DWORD dwRoleID, INT32 nExploit, BOOL bSave2DB = TRUE);
	INT32	GetOnlineGuildMemberCnt();

	// �����а��ɳ�Ա�������������仯��Ϣ
	VOID	SyncOnlineNum2AllGuildMumber(BOOL bLogout = FALSE);

private:
	MapGuildMember			m_mapMember;

	// ��������
	DWORD					m_dwGuildID;

	// ���ɳ�Ա�仯��־
	BOOL					m_MemberChangeFlag;
};

/**************************** ����ʵ�� ********************************/

inline VOID GuildMember::Init(DWORD dwGuildID)
{
	ASSERT(m_mapMember.Empty());

	m_dwGuildID = dwGuildID;
}

//-------------------------------------------------------------------------------
// ��ȡ��Ա
//-------------------------------------------------------------------------------
inline tagGuildMember* GuildMember::GetMember(DWORD dwRoleID)
{
	return m_mapMember.Peek(dwRoleID);
}

//-------------------------------------------------------------------------------
// ��Ա����
//-------------------------------------------------------------------------------
inline INT32 GuildMember::GetNumber()
{
	return m_mapMember.Size();
}

//-------------------------------------------------------------------------------
// �Ƿ��Ѽ���
//-------------------------------------------------------------------------------
inline BOOL GuildMember::IsExist(DWORD dwRoleID)
{
	return m_mapMember.IsExist(dwRoleID);
}

//-------------------------------------------------------------------------------
// �Ƿ��Ѽ���
//-------------------------------------------------------------------------------
inline MapGuildMember& GuildMember::GetMemberMap()
{
	return m_mapMember;
}

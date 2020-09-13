// ����
#pragma once
#include "family_quest.h"
#include "family_member.h"

struct tagFamilyAtt;
class FamilyMember;
class FamilySprite;
struct tagFamilyLoad;

class Family
{
public:
	Family(bool bCreateNew = false)
	{
		m_bResetFamilyActive = FALSE;
		//m_pFamilySprite = NULL;
		m_FamilyQuest.Init(this, bCreateNew);
	}
	Family(tagFamilyLoad& sFamilyLoad);
	~Family()
	{
		Destory();
	}

	VOID	Update();
	VOID	Destory();

	VOID AddMember(DWORD dwRoleID, DWORD dwJoinTime);
	VOID KickMember(DWORD dwRoleID);

	VOID SetMemberLeader(DWORD dwRoleID);
	VOID SetQuestInitOK() { m_FamilyQuest.SetInitOK(); }

	BOOL MemberIsLeader(DWORD dwRoleID)
	{
		tagFamilyMember* pFamilyMember = GetFamilyMember().GetFamilyMember(dwRoleID);
		if (P_VALID(pFamilyMember))
			return pFamilyMember->bLeader;
		return FALSE;
	}

	VOID	OnMemberKillMonster(DWORD dwRoleID, DWORD dwMonsterID, INT nLevel, DWORD dwMapCrc);

	FamilyMember& GetFamilyMember()
	{
		return m_FamilyMem;
	}

	tagFamilyAtt& GetFamilyAtt()
	{
		return m_FamilyAtt;
	}

	VOID SetFamilyName(tstring strName)
	{
		m_strName = strName;
	}
	LPCTSTR GetFamilyName()
	{
		return m_strName.c_str();
	}

	DWORD			GetID()				{ return m_FamilyAtt.dwFamilyID; }

	FamilyQuest&	GetFamilyQuest()	{ return m_FamilyQuest; }


	//���һ��������û������һ��
	//FamilySprite* CreateFamilySprite( DWORD dwFamilyID, TCHAR *pszName );

	//���ü�������
	//VOID SetFamilySprite( FamilySprite* pFamilySprite );

	//��ü��������ȼ�
	UINT16	GetFamilySpriteLevel();

	//��ü�������
	//FamilySprite* GetFamilySprite(){return m_pFamilySprite;}

	BOOL IsFamilyInitOK();
	VOID LoadFamilyMember(tagFamilyMember* pFamilyMember);

	VOID SendRoleJoinFamily2DB(DWORD dwMemberID);
	VOID SendSetLeader2DB(DWORD dwRoleID, BOOL bLeader);
	VOID SendLeaveFamily2DB(DWORD dwRoleID);
	VOID SendFamilyAtt2DB(tagFamilyAtt& sFamilyAtt);
	VOID ChangeFamilyActive(INT nActive);

	BOOL IsResetFamilyActive()
	{
		return m_bResetFamilyActive;
	}
	VOID SetResetFamilyActive(BOOL bResetFamilyActive)
	{
		m_bResetFamilyActive = bResetFamilyActive;
	}
	VOID ResetFamilyActive()
	{
		ChangeFamilyActive(0);
	}

private:
	// ��������
	tstring					m_strName;	
	
	// ��������
	tagFamilyAtt			m_FamilyAtt;

	// �����Ա
	FamilyMember			m_FamilyMem;

	// ��������
	FamilyQuest				m_FamilyQuest;

	//��������
	//FamilySprite			*m_pFamilySprite;

	BOOL m_bResetFamilyActive;
};


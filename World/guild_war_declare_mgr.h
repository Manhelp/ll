//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: ����ս��ս������
//-----------------------------------------------------------------------------

#pragma once

#include "../ServerDefine/guild_war_declare_define.h"
#include "guild_mgr.h"
#include "guild.h"

// ����ս��ս��
class CGuildWarDeclare
{
public:
	CGuildWarDeclare( DWORD 		dwAttackGuildID,			// ��ս����ID
					  DWORD 		dwDefenceGuildID,			// ����ս����ID
					  BOOL			bWarEnded,					// ��ս�ǲ����Ѿ���������
					  tagDWORDTime	dwWarDeclareTime);			// ��սʱ��
					
	~CGuildWarDeclare() {}

	// �����������������߽�ɫ���ӹ���սbuff
	VOID AddBuffToAllOnlineRole();

	// ɾ�����������������߽�ɫ���ӹ���սbuff
	VOID DeleteBuffOnAllOnlineRole( INT nDelType = 0 );

	// ����ս��ϵ���浽���ݿ�
	VOID SaveGuildWarDeclare2DB();

	DWORD GetAttackGuildID() { return m_dwAttackGuildID; }
	DWORD GetDefenceGuildID() { return m_dwDefenceGuildID; }
	tagDWORDTime GetWarDeclareTime() { return m_dwWarDeclareTime; }

	// ������ս��ʱ���Ƿ���
	BOOL IsGuildWarEnd();

	// ����ս��������
	VOID GuildWarEnd();

	// ��鹫��ս�ǲ����Ѿ���������
	BOOL IsGuildWarEnded()	{ return m_bWarEnded; }

private:

	DWORD			m_dwAttackGuildID;			// ��ս����ID
	DWORD			m_dwDefenceGuildID;			// ����ս����ID
	tagDWORDTime	m_dwWarDeclareTime;			// ��սʱ��

	BOOL			m_bWarEnded;				// �ǲ����Ѿ���������
};

// ����ս��ս��������
class CGuildWarDeclareMgr
{
private:
	typedef multimap<DWORD, CGuildWarDeclare*>::iterator MultiMapIterator;

public:
	CGuildWarDeclareMgr();
	~CGuildWarDeclareMgr();

	VOID Init();

	VOID Update();

	// ����ս��ϵ��ӵ�map����
	VOID AddGuildWarDeclare2Map(DWORD dwAttackGuildID,			// ��ս����ID
		DWORD dwDefenceGuildID);		// ����ս����ID

	// ��ʼ��������ս��ϵmap
	VOID InitGuildWarDeclareMap(DWORD dwGuildWarDeclareCnt, tagGuildWarDeclare* pGuildWarDeclare);

	// ������ݿ��й��ڵĹ�����ս��ϵ
	VOID ClearExpireGuildDeclareInDB();

	// ͬ�����ɫ�������ᴦ��ս��״̬�İ���
	VOID SyncWarStateGuild(Role* pRole);

	// ȡָ�����ᵱ�������ս����
	BYTE GetTodayDeclareWarTimes(DWORD dwGuildID);

	// ȡָ�������ͬһ��ָ�����ᵱ�����ս����
	BYTE GetTodaySameGuildBeWarTimes( DWORD dwAttackGuildID,			// ��ս����ID
									  DWORD dwDefenceGuildID);			// ����ս����ID

	// ���й����ɢʱ����
	VOID OnGuildDismiss(DWORD dwGuildID);

	// ���н�ɫ�˳�����ʱ�򴥷�
	VOID OnRoleLeaveGuild(Role* pRole);

	// ���н�ɫ���빫��ʱ�򴥷�
	VOID OnRoleJoinGuild(Role* pRole);

	// ���������Ƿ�����ս��ϵ
	BOOL IsInGuildWarState(Role* pRole1, Role* pRole2);
	BOOL IsInGuildWarState(DWORD dwGuildID1, DWORD dwGuildID2);

	// ����ɫ����ʱ����
	VOID OnRoleLogout(Role* pRole);

	//��ȡ�ù����ڹ���ս�е������������ǹ�����Ҳ���Ƿ�����
	INT GetGuildWarNum( DWORD dwGuildID );

private:

	// �������ݿ��еĹ�����ս��ϵ
	VOID LoadAllGuildWarDeclareFromDB();

	// �ͷŹ�����ս��ϵmap
	VOID DestoryGuildWarDeclareMap();

	// ����ս��ϵ��ӵ�map����
	VOID AddGuildWarDeclare2Map(DWORD dwAttackGuildID,			// ��ս����ID
		DWORD			dwDefenceGuildID,						// ����ս����ID
		tagDWORDTime	dwWarDeclareTime,						// ��սʱ��
		BOOL			bWarEnded = FALSE,						// ��ս�ǲ����Ѿ���������
		BOOL			bNeedSave2DB = TRUE);					// �Ƿ�Ҫ�����ݿ�

	template<typename DeclareOperation>
	VOID ForEachDeclareInMap(DeclareOperation oper);

private:

	// ����������ս
	std::multimap<DWORD, CGuildWarDeclare*> m_mapGuildWarDeclareList;

};

template<typename DeclareOperation>
VOID CGuildWarDeclareMgr::ForEachDeclareInMap(DeclareOperation oper)
{
	TMap<DWORD, CGuildWarDeclare*>::TMapIterator iterator = m_mapGuildWarDeclareList.Begin();
	CGuildWarDeclare* pGuildWarDeclare = NULL;
	while (m_mapGuildWarDeclareList.PeekNext(iterator,pGuildWarDeclare))
	{
		if (P_VALID(pGuildWarDeclare))
		{
			oper(pGuildWarDeclare);
		}
	}
}

extern CGuildWarDeclareMgr g_guildWarDeclareMgr;
//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: ����ս��ս������
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "guild_war_declare_mgr.h"
#include "../WorldDefine/msg_guild_war_declare.h"
#include "../ServerDefine/msg_guild_war_declare.h"
#include "db_session.h"
#include "role.h"

#define ONE_HOUR 3600 // 60*60,һ��Сʱ�ж�����


CGuildWarDeclare::CGuildWarDeclare( DWORD 			dwAttackGuildID,			// ��ս����ID
									DWORD 			dwDefenceGuildID,			// ����ս����ID
									BOOL			bWarEnded,					// ��ս�ǲ����Ѿ���������
									tagDWORDTime	dwWarDeclareTime)			// ��սʱ��
:m_dwAttackGuildID(dwAttackGuildID),
m_dwDefenceGuildID(dwDefenceGuildID),
m_bWarEnded(bWarEnded),
m_dwWarDeclareTime(dwWarDeclareTime)
{
}

// �����������������߽�ɫ���ӹ���սbuff
VOID CGuildWarDeclare::AddBuffToAllOnlineRole()
{
	Guild* pAttackGuild = g_guildMgr.GetGuild(m_dwAttackGuildID);
	if (P_VALID(pAttackGuild))
	{
		pAttackGuild->GetMemberMgr().AddBuffToAllOnLineMember(GUILD_WAR_BUFF_ID);
	}
	Guild* pDefenceGuild = g_guildMgr.GetGuild(m_dwDefenceGuildID);
	if (P_VALID(pDefenceGuild))
	{
		pDefenceGuild->GetMemberMgr().AddBuffToAllOnLineMember(GUILD_WAR_BUFF_ID);
	}
}

// ɾ�����������������߽�ɫ���ӹ���սbuff
VOID CGuildWarDeclare::DeleteBuffOnAllOnlineRole(INT nDelType/*=0*/)
{
	if( 0 == nDelType || 1 == nDelType)
	{
		Guild* pAttackGuild = g_guildMgr.GetGuild(m_dwAttackGuildID);
		if (P_VALID(pAttackGuild))
		{
			pAttackGuild->GetMemberMgr().RemoveBuffFromAllOnLineMember(GUILD_WAR_BUFF_ID);
		}
	}
	if( 0 == nDelType || 2 == nDelType)
	{
		Guild* pDefenceGuild = g_guildMgr.GetGuild(m_dwDefenceGuildID);
		if (P_VALID(pDefenceGuild))
		{
			pDefenceGuild->GetMemberMgr().RemoveBuffFromAllOnLineMember(GUILD_WAR_BUFF_ID);
		}
	}
}

// ����ս��ϵ���浽���ݿ�
VOID CGuildWarDeclare::SaveGuildWarDeclare2DB()
{
	tagNDBC_InsertGuildWarDeclare send;
	send.dwAttackGuildID = m_dwAttackGuildID;
	send.dwDefenceGuildID = m_dwDefenceGuildID;
	send.dwWarDeclareTime = m_dwWarDeclareTime;
	send.bWarEnded = m_bWarEnded;

	g_dbSession.Send(&send, send.dwSize);
}

// ���ս������ʱ���Ƿ���
BOOL CGuildWarDeclare::IsGuildWarEnd()
{
	if (CalcTimeDiff(GetCurrentDWORDTime(), m_dwWarDeclareTime) >= ONE_HOUR)
	{
		return TRUE;
	}
	return FALSE;
}

// ����ս��������
VOID CGuildWarDeclare::GuildWarEnd()
{
	tagNS_GuildWarEnd send;
	send.dwAttackGuildID = m_dwAttackGuildID;
	send.dwDefenceGuildID = m_dwDefenceGuildID;

	// ����ս���ɷ���ս��������Ϣ
	Guild* pAttackGuild = g_guildMgr.GetGuild(m_dwAttackGuildID);
	if (P_VALID(pAttackGuild))
	{
		pAttackGuild->SendGuildMsg(&send, send.dwSize);
	}

	// ������ս���ɷ���ս��������Ϣ
	Guild* pDefenceGuild = g_guildMgr.GetGuild(m_dwDefenceGuildID);
	if (P_VALID(pDefenceGuild))
	{
		pDefenceGuild->SendGuildMsg(&send, send.dwSize);
	}

	// ��ʶ�Ѿ���������
	m_bWarEnded = TRUE;

	// �������ݿ�
	tagNDBC_SetGuildWarDeclareEnded send2DB;
	send2DB.dwAttackGuildID = m_dwAttackGuildID;
	send2DB.dwDefenceGuildID = m_dwDefenceGuildID;
	send2DB.dwWarDeclareTime = m_dwWarDeclareTime;
	g_dbSession.Send(&send2DB, send2DB.dwSize);
}

// ����ս��ս��������
CGuildWarDeclareMgr::CGuildWarDeclareMgr()
{

}

CGuildWarDeclareMgr::~CGuildWarDeclareMgr()
{
	// �ͷ�ǿ������ʦ�������б�
	DestoryGuildWarDeclareMap();
}

VOID CGuildWarDeclareMgr::Init()
{
	// �������ݿ��еĹ�����ս��ϵ
	LoadAllGuildWarDeclareFromDB();
}

VOID CGuildWarDeclareMgr::Update()
{
	CHECK_TIME()
	MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
	CGuildWarDeclare* pGuildWarDeclare = NULL;
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if (!pGuildWarDeclare->IsGuildWarEnded() && pGuildWarDeclare->IsGuildWarEnd())
				{
					INT nType = 0;
					if( GetGuildWarNum(pGuildWarDeclare->GetAttackGuildID()) > 1)
					{
						nType = 2;
					}

					if( GetGuildWarNum(pGuildWarDeclare->GetDefenceGuildID()) > 1)
					{
						if( 2 == nType)
						{
							nType = 3;
						}
						else
						{
							nType = 1;
						}
					}

					// �����ˣ�ɾ��buff
					pGuildWarDeclare->DeleteBuffOnAllOnlineRole(nType);

					// ����ս��������
					pGuildWarDeclare->GuildWarEnd();
				}
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}
}

// ����ս��ϵ��ӵ�map����
VOID CGuildWarDeclareMgr::AddGuildWarDeclare2Map(DWORD dwAttackGuildID,			// ��ս����ID
												 DWORD dwDefenceGuildID)		// ����ս����ID
{
	// ����ս��ϵ��ӵ�map����
	AddGuildWarDeclare2Map(dwAttackGuildID, dwDefenceGuildID, GetCurrentDWORDTime());
}

// ��ʼ����ս��ϵmap
VOID CGuildWarDeclareMgr::InitGuildWarDeclareMap(DWORD dwGuildWarDeclareCnt, tagGuildWarDeclare* pGuildWarDeclare)
{
	if (dwGuildWarDeclareCnt == 0 || !P_VALID(pGuildWarDeclare))
	{
		return;
	}

	for (DWORD dwCnt = 0; dwCnt < dwGuildWarDeclareCnt; dwCnt++)
	{
		// ����ս��ϵ��ӵ�map����
		AddGuildWarDeclare2Map(pGuildWarDeclare[dwCnt].dwAttackGuildID, pGuildWarDeclare[dwCnt].dwDefenceGuildID, pGuildWarDeclare[dwCnt].dwWarDeclareTime, pGuildWarDeclare[dwCnt].bWarEnded, FALSE);
	}
}

// ������ݿ��й��ڵ���ս��ϵ
VOID CGuildWarDeclareMgr::ClearExpireGuildDeclareInDB()
{
	tagNDBC_ClearExpireGuildWarDeclare send;
	g_dbSession.Send(&send, send.dwSize);

	// ����ڴ��й��ڵ���ս��ϵ
	tagDWORDTime dwCurTime = DecreaseTime(GetCurrentDWORDTime(), ONE_HOUR);
	
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator eraseIterator;
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if (pGuildWarDeclare->GetWarDeclareTime() < dwCurTime)
				{
					eraseIterator = iterator;
					iterator++;
					// �����ˣ�ɾ��֮
					m_mapGuildWarDeclareList.erase(eraseIterator);
					SAFE_DEL(pGuildWarDeclare);
				}
				else
				{
					iterator++;
				}
				
			}
			if (dwCnt < 0)
				break;
		}
	}

	// ֪ͨ���й���ָ�ʣ����ս����
	tagNS_ResetRemainDeclareTimes send2Client;
	g_guildMgr.SendMsgToAllGuild(&send2Client, send2Client.dwSize);
}

// ͬ�����ɫ�������ɴ���ս��״̬�İ���
VOID CGuildWarDeclareMgr::SyncWarStateGuild(Role* pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}

	DWORD dwGuildID = pRole->GetGuildID();
	INT nGuildCnt = 0;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	// ȡ���ɸ����Ͱ����б�
	TList<DWORD> listGuildID;
	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if (pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
					pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
					(pGuildWarDeclare->GetAttackGuildID() == dwGuildID ||
					pGuildWarDeclare->GetDefenceGuildID() == dwGuildID))
				{
					nGuildCnt++;
					DWORD dwEnemyGuildID = (pGuildWarDeclare->GetAttackGuildID() == dwGuildID)?
						pGuildWarDeclare->GetDefenceGuildID():pGuildWarDeclare->GetAttackGuildID();
					listGuildID.PushBack(dwEnemyGuildID);
				}
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}

	// ���û�а��ɣ��򷵻�
	if (nGuildCnt <= 0)
	{
		tagNS_SyncWarStateGuild syncWarStateGuild;
		syncWarStateGuild.nGuildCount = 0;
		syncWarStateGuild.nRemainWarDeclareTimes = (MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY <= GetTodayDeclareWarTimes(dwGuildID))?
			0:MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY - GetTodayDeclareWarTimes(dwGuildID);
		pRole->SendMessage(&syncWarStateGuild, syncWarStateGuild.dwSize);
		return;
	}

	// ������Ϣ
	DWORD dwSize = sizeof(tagNS_SyncWarStateGuild) + (nGuildCnt-1)*sizeof(DWORD);
	MCREATE_MSG(pSend, dwSize, NS_SyncWarStateGuild);
	if (!P_VALID(pSend))
	{
		return;
	}

	listGuildID.ResetIterator();
	pSend->nGuildCount = nGuildCnt;
	for (INT nCnt = 0; nCnt < nGuildCnt; nCnt++)
	{
		if (listGuildID.PeekNext(pSend->dwGuildID[nCnt]) == FALSE)
		{
			break;
		}
	}

	// ����ɫ��ӹ���սbuff
	const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(GUILD_WAR_BUFF_ID);
	if ( P_VALID(pBuffProto))
	{
		pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
	}

	pSend->nRemainWarDeclareTimes = (MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY <= GetTodayDeclareWarTimes(dwGuildID))?
		0:MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY - GetTodayDeclareWarTimes(dwGuildID);

	// ������Ϣ
	pRole->SendMessage(pSend, pSend->dwSize);
}

// ȡָ�����ɵ��������ս����
BYTE CGuildWarDeclareMgr::GetTodayDeclareWarTimes(DWORD dwGuildID)
{
	if (dwGuildID == 0)
		return 0;

	INT nGuildCnt = 0;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.find(dwGuildID);
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;

			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				GetCurrentDWORDTime().day == pGuildWarDeclare->GetWarDeclareTime().day &&
				pGuildWarDeclare->GetAttackGuildID() == dwGuildID)
			{
				nGuildCnt++;		
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}
	return nGuildCnt;
}

// ȡָ�����ɶ�ͬһ��ָ�����ɵ������ս����
BYTE CGuildWarDeclareMgr::GetTodaySameGuildBeWarTimes( DWORD dwAttackGuildID,			// ��ս����ID
													   DWORD dwDefenceGuildID)			// ����ս����ID
{
	if (dwAttackGuildID == 0 || dwDefenceGuildID == 0)
		return 0;

	INT nGuildCnt = 0;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.find(dwAttackGuildID);
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				GetCurrentDWORDTime().day == pGuildWarDeclare->GetWarDeclareTime().day &&
				pGuildWarDeclare->GetAttackGuildID() == dwAttackGuildID &&
				pGuildWarDeclare->GetDefenceGuildID() == dwDefenceGuildID)
			{
				nGuildCnt++;		
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}

	return nGuildCnt;
}

// �������ݿ��еĹ�����ս��ϵ
VOID CGuildWarDeclareMgr::LoadAllGuildWarDeclareFromDB()
{
	tagNDBC_LoadGuildWarDeclare send;

	g_dbSession.Send(&send, send.dwSize);
}

// �ͷŹ�����ս��ϵmap
VOID CGuildWarDeclareMgr::DestoryGuildWarDeclareMap()
{
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			SAFE_DEL(pGuildWarDeclare);
			iterator++;
			if (dwCnt < 0)
				break;
		}
		m_mapGuildWarDeclareList.clear();
	}

}

// ����ս��ϵ��ӵ�map����
VOID CGuildWarDeclareMgr::AddGuildWarDeclare2Map(DWORD 			dwAttackGuildID,			// ��ս����ID
												 DWORD 			dwDefenceGuildID,			// ����ս����ID
												 tagDWORDTime	dwWarDeclareTime,			// ��սʱ��
												 BOOL			bWarEnded/* = FALSE*/,		// ��ս�ǲ����Ѿ���������
												 BOOL			bNeedSave2DB /*= TRUE*/) 	// �Ƿ�Ҫ�����ݿ�
{
	if (dwAttackGuildID == 0 || dwDefenceGuildID == 0)
		return;

	CGuildWarDeclare* pGuildWarDeclare = new CGuildWarDeclare(dwAttackGuildID, dwDefenceGuildID, bWarEnded, dwWarDeclareTime);
	if (P_VALID(pGuildWarDeclare))
	{
		m_mapGuildWarDeclareList.insert(std::make_pair(dwAttackGuildID, pGuildWarDeclare));
		if (bNeedSave2DB)
		{
			// �����ݴ������ݿ�
			pGuildWarDeclare->SaveGuildWarDeclare2DB();
		}

		// ��ս�����buff
		pGuildWarDeclare->AddBuffToAllOnlineRole();
	}
}

// ���й����ɢʱ����
VOID CGuildWarDeclareMgr::OnGuildDismiss(DWORD dwGuildID)
{
	if (dwGuildID == 0)
		return;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
				pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
				(pGuildWarDeclare->GetAttackGuildID() == dwGuildID ||
				pGuildWarDeclare->GetDefenceGuildID() == dwGuildID))
			{
				// �����ˣ�ɾ��buff
				pGuildWarDeclare->DeleteBuffOnAllOnlineRole();

				// ����ս��������
				pGuildWarDeclare->GuildWarEnd();
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}
}

// ���н�ɫ�˳�����ʱ�򴥷�
VOID CGuildWarDeclareMgr::OnRoleLeaveGuild(Role* pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}

	// ��������й���ս����buff��ɾ����
	if (pRole->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID))
	{
		pRole->RemoveBuff(GUILD_WAR_BUFF_TYPE_ID, TRUE);
	}

	// ͬ������ս��״̬
	tagNS_SyncWarStateGuild send;
	send.nGuildCount = 0;
	send.nRemainWarDeclareTimes = 0;
	pRole->SendMessage(&send, send.dwSize);
}

// ���н�ɫ���빫��ʱ�򴥷�
VOID CGuildWarDeclareMgr::OnRoleJoinGuild(Role* pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}

	BOOL bGuildInWar = FALSE;
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
				pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
				(pGuildWarDeclare->GetAttackGuildID() == pRole->GetGuildID() ||
				pGuildWarDeclare->GetDefenceGuildID() == pRole->GetGuildID()))
			{
				// �������û���й���ս����buff�����һ��
				if (pRole->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID) == FALSE)
				{
					const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(GUILD_WAR_BUFF_ID);
					if (P_VALID(pBuffProto))
					{
						pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
					}
				}
				bGuildInWar = TRUE;
				break;
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}
	if (bGuildInWar)
	{
		// ͬ������ս��״̬
		SyncWarStateGuild(pRole);
	}
}

// ���������Ƿ��ежԹ�ϵ
BOOL CGuildWarDeclareMgr::IsInGuildWarState(Role* pRole1, Role* pRole2)
{
	if (!P_VALID(pRole1) || !P_VALID(pRole2))
	{
		return FALSE;
	}
	if (!pRole1->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID) || !pRole2->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID))
	{
		return FALSE;
	}
	DWORD dwGuildID1 = pRole1->GetGuildID();
	DWORD dwGuildID2 = pRole2->GetGuildID();

	return IsInGuildWarState(dwGuildID1, dwGuildID2);
}

BOOL CGuildWarDeclareMgr::IsInGuildWarState(DWORD dwGuildID1, DWORD dwGuildID2)
{
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
				pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
				((pGuildWarDeclare->GetAttackGuildID() == dwGuildID1 &&
				pGuildWarDeclare->GetDefenceGuildID() == dwGuildID2) ||
				(pGuildWarDeclare->GetAttackGuildID() == dwGuildID2 &&
				pGuildWarDeclare->GetDefenceGuildID() == dwGuildID1)))
			{
				return TRUE;
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}
	return FALSE;
}

// ����ɫ����ʱ����
VOID CGuildWarDeclareMgr::OnRoleLogout(Role* pRole)
{
	// �����ɫ�����й���սbuff��Ҫɾ��
	if (pRole->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID))
	{
		pRole->RemoveBuff(GUILD_WAR_BUFF_TYPE_ID, TRUE);
	}
}


INT CGuildWarDeclareMgr::GetGuildWarNum( DWORD dwGuildID )
{
	MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
	CGuildWarDeclare* pGuildWarDeclare = NULL;
	INT dwCnt = m_mapGuildWarDeclareList.size();
	INT nNum = 0;

	if (dwCnt > 0)
	{
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if( dwGuildID == pGuildWarDeclare->GetAttackGuildID() )
				{
					nNum++;
				}

				if( dwGuildID ==  pGuildWarDeclare->GetDefenceGuildID() )
				{
					nNum++;
				}
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}

	return nNum;
}


CGuildWarDeclareMgr g_guildWarDeclareMgr;
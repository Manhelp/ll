/********************************************************************
	created:	2008/11/04
	created:	4:11:2008   13:59
	filename: 	d:\loong_client\Client\QuestTrackFrame.cpp
	file path:	d:\loong_client\Client
	file base:	QuestTrackFrame
	file ext:	cpp
	author:		leili
	
	purpose:	����׷�ٽ���ʵ��
*********************************************************************/
#include "StdAfx.h"
#include "QuestTrackFrame.h"
#include "QuestMgr.h"
#include "CreatureData.h"
#include "ItemProtoData.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CombatEvent.h"
#include "SocialMgr.h"
#include "TeamSys.h"
#include "CombatSysUtil.h"
#include "ItemMgr.h"
#include "Container.h"
#include "..\WorldDefine\quest_board.h"
#include "..\WorldDefine\msg_quest_board.h"

namespace
{
	struct tagTrack 
	{
		UINT16	questID;
		float	lastUpdateTime;
	};

	class TrackSort
	{
	public:
		bool operator()(const tagTrack& quest1,const tagTrack& quest2)
		{
			return quest1.lastUpdateTime > quest2.lastUpdateTime;
		}
	};
}

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
QuestTrackFrame::QuestTrackFrame( void )
: m_Trunk(this)
, m_pWnd(0)
, m_pQuest(0)
, m_pBtnCloseWnd(0)
, m_trackCount(0)
{

}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
QuestTrackFrame::~QuestTrackFrame( void )
{
	
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL QuestTrackFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
BOOL QuestTrackFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���¶��봰�ڽ���
//-----------------------------------------------------------------------------
BOOL QuestTrackFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\tasktrack.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuestTrackFrame::EventHandler));

	m_pBtnCloseWnd	= (GUIButton*)	m_pWnd->GetChild(_T("tt_pic\\closebutt"));
	m_pQuest		= (GUIStaticEx*)m_pWnd->GetChild(_T("tt_pic\\tt_back\\tt_list"));
	m_pQuest->SetText(_T(""));

	//m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������Ϣ�¼�����
//-----------------------------------------------------------------------------
BOOL QuestTrackFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnCloseWnd )// �ر�����׷�ٽ���
			{
				tagGameEvent e(_T("QuestMainFrameEvent_CloseTrack"), this);
				m_pMgr->SendEvent(&e);

				QuestMgr::Inst()->CloseTrackFrame();
			}
		}
		break;

	case EGUIE_StaticExLinkClick:
		{
			Vector3 pos;
			QuestData *pData = QuestMgr::Inst()->GetData();
			ASSERT( P_VALID(pData) );
			QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
			ASSERT( P_VALID(pQuery) );
			bool isMapEx = false;//�Ƿ�糡��
			tstring mapName;

			if( !pQuery->GetNPCPosition( pEvent->dwParam1, pos ) )//���ڱ���ͼ�����п糡��Ѱ·
			{
				if( !pData->GetPathPointMap( pEvent->dwParam1, mapName ) )//��õ�ͼ����
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("quest_cannot_find_npc")]);
					break;
				}

				isMapEx = true;
				QuestMgr::Inst()->MoveToMap(mapName.c_str(),pos);
			}

			LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();

			tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
			event.start = pLP->GetPos();
			event.end = pos;
			event.validDist = 100.0f;
			event.dwNpcTypeID = pEvent->dwParam1;
			event.bNotThisMap = isMapEx;
			event.strMapName = mapName;

			m_pMgr->SendEvent( &event );
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ��ʾ����
//-----------------------------------------------------------------------------
VOID QuestTrackFrame::Show( void )
{
	if( P_VALID(m_pWnd) ) 
	{
		m_pWnd->SetInvisible(FALSE);
		m_pWnd->FlipToTop();
	}
}

//-----------------------------------------------------------------------------
// ���ؽ���
//-----------------------------------------------------------------------------
VOID QuestTrackFrame::Hide( void )
{
	if( P_VALID(m_pWnd) ) 
		m_pWnd->SetInvisible(TRUE);
}

//-----------------------------------------------------------------------------
// �����Ƿ�ɼ�
//-----------------------------------------------------------------------------
BOOL QuestTrackFrame::IsVisible( void ) const
{
	if( P_VALID(m_pWnd) )
	{
		return !m_pWnd->IsInvisible();
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// ����ĳ������
//-----------------------------------------------------------------------------
BOOL QuestTrackFrame::AddQuestTrack( UINT16 questID )
{
	QuestData *pData = QuestMgr::Inst()->GetData();
	ASSERT( P_VALID(pData) && _T("QuestTrackFrame���QuestData����") );

	m_trackCount++;

	tstringstream stream;
	tstring questName;
	if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
	{
		UINT16 nIndex = 0;
		BYTE byType = 0;
		if(questID>=YELLOW_LIST_START_TASK_ID)
		{
			nIndex = questID - YELLOW_LIST_START_TASK_ID;
			byType = EQBT_YellowList;
		}
		else
		{
			nIndex = questID - QUEST_PANEL_START_TASK_ID;
			byType = EQBT_Reward;
		}
		const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
		const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[nIndex]);
		pData->GetQuestPanelTaskName( vecQuest[nIndex], questName );

	}
	else
		pData->GetQuestName( questID, questName );
	TCHAR szQuestTrack[512];
	DWORD color = 0xFFFFF7E0;
	pData->GetQuestTypeColor(questID, color);

	stream << m_pQuest->GetText();

	// ��������
	tstring questTypeName(_T(""));
	INT16 type = pData->GetQuestCategoryByQuestID(questID);
	if (type > 0 && type < 12 )
	{
		questTypeName = pData->GetTaskTypeName(type);
	}

	_stprintf( szQuestTrack, 
		_T("<color=0x%x>[%s]<color=0xFFFFF7E0>"),
		color, questTypeName.c_str());
	stream << szQuestTrack;

	// ��������

	_stprintf( szQuestTrack, 
		_T("%d.<color=0xFFFFF7E0>%s<color=0xFFFFF7E0>"),
		m_trackCount,
		questName.c_str());
	stream << szQuestTrack;

	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	ASSERT( P_VALID(pQuery) && _T("QuestTrackFrame���QuestQuery����") );

	// �������״��
	if( pQuery->QuestCanComplete(questID) )
	{
		_stprintf( szQuestTrack,
			_T("(%s)\\n"),
			g_StrTable[_T("QuestTrack_Done")]);
		stream << szQuestTrack;
	}
	else
	{
		_stprintf( szQuestTrack,
			_T("(%s)\\n"),
			g_StrTable[_T("QuestTrack_Doing")]);
		stream << szQuestTrack;
	}

	const tagQuestProto* pQuest = pData->GetQuestProto(questID);

	const tagQuestDynamicTarget *pDynamicTarget = pData->GetQuestDynamicTarget(questID);
	if( P_VALID(pDynamicTarget) )//��̬
	{
		UINT16 itemCount,creatureCount,itemSum,creatureSum;
		DWORD itemID,creatureID;

		switch( pDynamicTarget->eTargetType )
		{
		case EQTT_Kill:
			{
				// �������
				for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
				{
					if( pDynamicTarget->dwTargetID[i] == 0 )
						continue;
					if( pData->GetQuestRequiredCreatureCount( questID, i, creatureID, creatureSum ) )
					{
						const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
						ASSERT( creatureProto != NULL );

						pQuery->GetQuestCreatureCount( questID, i, creatureCount );
						if( creatureCount > creatureSum )
						{
							creatureCount = creatureSum;
						}

						_stprintf( szQuestTrack, 
							_T("  <color=0xFFFFF7E0><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"),
							creatureProto->dwTypeID, 
							creatureProto->szName, 
							creatureCount, 
							creatureSum );
						stream << szQuestTrack;
					}
				}
			}
			break;
		case EQTT_Collect:
			{
				// ��Ʒ����
				for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
				{
					if( pData->GetQuestRequiredItemCount( questID, i, itemID, itemSum ) )
					{
						const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
						ASSERT( itemProto != NULL );

						pQuery->GetQuestItemCount( questID, i, itemCount );
						if( itemCount > itemSum )
						{
							itemCount = itemSum;
						}

						DWORD dwMonsterID = pData->GetMonsterIDByItemID(itemID);
						if( dwMonsterID != GT_INVALID )
						{
							_stprintf( szQuestTrack, 
								_T("  <color=0xFFFFF7E0><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"),
								dwMonsterID, 
								itemProto->szName, 
								itemCount, 
								itemSum );
						}
						else
						{
							_stprintf( szQuestTrack, 
								_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
								itemProto->szName, 
								itemCount, 
								itemSum );
						}
						stream << szQuestTrack;
					}
				}
			}
			break;
		case EQTT_NPCTalk:
			{
				// �Ի�NPC����
				for( int i=0; i<3; i++ )
				{
					if( pDynamicTarget->dwTargetID[i] == 0 )
						break;

					tstring strTalked;
					if( pData->NPCBeTalked( questID, i ) )
						strTalked = g_StrTable[_T("QuestTrack_NPCTalked")];
					else
						strTalked = g_StrTable[_T("QuestTrack_NPCNoTalk")];

					const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pDynamicTarget->dwTargetID[i] );
					ASSERT( P_VALID(creatureProto) );
					_stprintf( szQuestTrack,
						_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
						creatureProto->dwTypeID,
						creatureProto->szName,
						strTalked.c_str() );
					stream << szQuestTrack;
				}
			}
			break;
		case EQTT_Invest:
			{
				// �������׷��
				for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
				{
					if( pDynamicTarget->dwTargetID[i] == 0 )
						break;

					tstring strInves;
					if( pQuery->ObjBeInves( questID, i ) )
						strInves = g_StrTable[_T("QuestTrack_TriggerUpdated")];
					else
						strInves = g_StrTable[_T("QuestTrack_TriggerNoUpdate")];

					const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pDynamicTarget->dwTargetID[i] );
					ASSERT( P_VALID(creatureProto) );
					_stprintf( szQuestTrack,
						_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
						creatureProto->dwTypeID,
						creatureProto->szName,
						strInves.c_str() );
					stream << szQuestTrack;
				}
			}
			break;
		}
	}
	else
	{
		UINT16 itemCount,creatureCount,itemSum,creatureSum;
		DWORD itemID,creatureID;
		
		if (pQuest->only_one_item == TRUE)
		{
			_stprintf( szQuestTrack,
				_T("(%s)\\n"),
				g_StrTable[_T("QuestTrack_Only_One_Item")]);
			stream << szQuestTrack;
		}
		else if (pQuest->only_one_npc == TRUE)
		{
			_stprintf( szQuestTrack,
				_T("(%s)\\n"),
				g_StrTable[_T("QuestTrack_Only_One_NPC")]);
			stream << szQuestTrack;
		}
		else if (pQuest->only_one_creature == TRUE)
		{
			_stprintf( szQuestTrack,
				_T("(%s)\\n"),
				g_StrTable[_T("QuestTrack_Only_One_Creature")]);
			stream << szQuestTrack;
		}

		// ����
		if( pQuest->complete_req_map != 0 )
		{
			int nNum = 0;
			SocialMgr* pSocialMgr = (SocialMgr*)TObjRef<GameFrameMgr>()->GetFrame(_T("SocialMgr"));
			if( P_VALID(pSocialMgr) )
			{
				nNum = pSocialMgr->GetData()->GetFriendNum();
				if( nNum >= pQuest->complete_req_map )
					nNum = pQuest->complete_req_map;
			}

			_stprintf( szQuestTrack, 
				_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
				g_StrTable[_T("FriendNum")], 
				nNum, 
				pQuest->complete_req_map );
			stream << szQuestTrack;
		}

		// ����
		if( pQuest->complete_req_map_x  != 0 )
		{
			int nNum = TeamSys::Inst()->GetTeammateNum();
			if( nNum >= pQuest->complete_req_map_x )
				nNum = pQuest->complete_req_map_x;

			_stprintf( szQuestTrack, 
				_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
				g_StrTable[_T("TeammateNum")], 
				nNum, 
				pQuest->complete_req_map_x );
			stream << szQuestTrack;
		}

		// ��ҵȼ�
		if( pQuest->complete_req_level != 0 )
		{
			_stprintf( szQuestTrack, 
				_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
				g_StrTable[_T("SelectRole_Lvl")], 
				RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel(), 
				pQuest->complete_req_level );
			stream << szQuestTrack;
		}

		// ����
		if( pQuest->chuandao_count > 0 )
		{
			UINT16 nCount = 0;
			if(pQuery->GetQuestKillTimes(questID, nCount))
			{
				if(nCount > pQuest->chuandao_count)
					nCount = pQuest->chuandao_count;
				_stprintf( szQuestTrack, 
					_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
					g_StrTable[_T("QuestKillChuanDaoMonster")], nCount, pQuest->chuandao_count );
			}
			
			stream << szQuestTrack;
		}

		// ��Ʒ����
		for( int i=0; i<QUEST_ITEMS_COUNT; i++ )
		{
			if( pData->GetQuestRequiredItemCount( questID, i, itemID, itemSum ) )
			{
				const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
				ASSERT( itemProto != NULL );

				pQuery->GetQuestItemCount( questID, i, itemCount );
				if( itemCount > itemSum )
				{
					itemCount = itemSum;
				}

				DWORD dwMonsterID = pData->GetMonsterIDByItemID(itemID);
				if( dwMonsterID != GT_INVALID )
				{
					_stprintf( szQuestTrack, 
						_T("  <color=0xFFFFF7E0><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"),
						dwMonsterID, 
						itemProto->szName, 
						itemCount, 
						itemSum );
				}
				else
				{
					_stprintf( szQuestTrack, 
						_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
						itemProto->szName, 
						itemCount, 
						itemSum );
				}
				stream << szQuestTrack;
			}
		}

		// �������
		for( int i=0; i<QUEST_CREATURES_COUNT; i++ )
		{
			if( pData->GetQuestRequiredCreatureCount( questID, i, creatureID, creatureSum ) )
			{
				const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
				ASSERT( creatureProto != NULL );

				pQuery->GetQuestCreatureCount( questID, i, creatureCount );
				if( creatureCount > creatureSum )
				{
					creatureCount = creatureSum;
				}

				_stprintf( szQuestTrack, 
					_T("  <color=0xFFFFF7E0><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"),
					creatureProto->dwTypeID, 
					creatureProto->szName, 
					creatureCount, 
					creatureSum );
				stream << szQuestTrack;
			}
		}

		// �Ի�NPC����
		for( int i=0; i<QUEST_NPC_COUNT; i++ )
		{
			if( pQuest->complete_req_npc[i] == 0 )
				break;

			tstring strTalked;
			if( pData->NPCBeTalked( questID, i ) )
				strTalked = g_StrTable[_T("QuestTrack_NPCTalked")];
			else
				strTalked = g_StrTable[_T("QuestTrack_NPCNoTalk")];

			const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pQuest->complete_req_npc[i] );
			ASSERT( creatureProto != NULL );
			_stprintf( szQuestTrack,
				_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
				creatureProto->dwTypeID,
				creatureProto->szName,
				strTalked.c_str() );
			stream << szQuestTrack;
		}

		// ������׷��
		for( int i=0; i<QUEST_TRIGGERS_COUNT; i++ )
		{
			if( pQuest->complete_req_trigger[i] == 0 )
				break;

			tstring strTrigger;
			if( pData->BeTriggered( questID, i ) )
				strTrigger = g_StrTable[_T("QuestTrack_TriggerUpdated")];
			else
				strTrigger = g_StrTable[_T("QuestTrack_TriggerNoUpdate")];

			QuestData *pData = QuestMgr::Inst()->GetData();
			tstring strName;
			pData->GetTriggerName( pQuest->complete_req_trigger[i], strName );
			_stprintf( szQuestTrack,
				_T("  <color=0xFFABABFF>%s <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
				strName.c_str(),
				strTrigger.c_str() );
			stream << szQuestTrack;
		}

		// ����Ŀ������
		if( pQuest->spec_condition )
			stream << _T("  ") << pQuest->conditions << _T("\\n");

		// ����������
		const QuestData::tagQuestCompleteConditionBySpecItem *pItem = pData->GetQuestItemConditionSpecItem(questID);
		if (P_VALID(pItem))
		{
			ItemContainer* pItemContainer = ItemMgr::Inst()->GetPocket();
			if (P_VALID(pItemContainer))
			{
				int nNum = pItemContainer->GetItemQuantity(pItem->itemid);
				if (nNum >= pItem->itemnum)
					nNum = pItem->itemnum;

				_stprintf( szQuestTrack, 
					_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
					g_StrTable[_T("QuickDoneQuest")], 
					nNum, 
					pItem->itemnum );
				stream << szQuestTrack;
			}
		}
	}

	// ����ظ�
	TCHAR szCompleteNPC[128]={0};
	for( int i=0; i<QUEST_COMPLETE_NPC; i++ )
	{
		if( pQuest->complete_quest_npc[i] == 0 )
			break;

		const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pQuest->complete_quest_npc[i] );
		ASSERT( creatureProto != NULL );
		if( P_VALID(creatureProto ) )
		{
			if( _tcslen(szCompleteNPC) > 0 )
			{
				_stprintf( szCompleteNPC,
					_T("%s,<color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFFFF7E0>"),
					szCompleteNPC,
					creatureProto->dwTypeID,
					creatureProto->szName );
			}
			else
			{
				_stprintf( szCompleteNPC,
					_T("<color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFFFF7E0>"),
					creatureProto->dwTypeID,
					creatureProto->szName );
			}
		}
	}
	if( _tcslen(szCompleteNPC) > 0 )
	{
		_stprintf( szQuestTrack,
			_T("\\n%s%s"),
			g_StrTable[_T("QuestTrack_Reply")],
			szCompleteNPC);
		stream << szQuestTrack;
	}

	// ����֮��Ŀ���
	stream << _T("\\n\\n");
	m_pQuest->SetText( stream.str().c_str() );

	// �������϶�����
	tagPoint size = m_pQuest->GetSize();
	if( m_pQuest->GetTextTotalHeight() > size.y )
	{
		size.y = m_pQuest->GetTextTotalHeight();
		m_pQuest->SetResizable(TRUE);
		m_pQuest->Resize( size );
		m_pQuest->SetResizable(FALSE);
	}

	m_pQuest->SetRefresh(TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------
VOID QuestTrackFrame::UpdateQuestTrack( void )
{
	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	ASSERT( P_VALID(pQuery) && _T("QuestTrackFrame���QuestQuery����") );

	m_trackCount = 0;
	m_pQuest->SetText(_T(""));

	// ����Ѿ����ٵ������б�
	vector<tagTrack> tracks;
	const QuestQuery::QuestsMap& quests = pQuery->GetCurrentQuests();
	for( QuestQuery::QuestsMap::const_iterator iter = quests.begin(); iter!=quests.end(); ++iter )
	{
		const QuestQuery::tagQuestTrack& track = iter->second;
		if( track.isTracked )
		{
			tagTrack trackInfo;
			trackInfo.questID = iter->first;
			trackInfo.lastUpdateTime = track.lastUpdateTime;

			tracks.push_back( trackInfo );
		}
	}

	// ���������ʱ�����������ʾ
	TrackSort sortObj;
	sort(tracks.begin(),tracks.end(),sortObj);
	for( size_t i=0; i<tracks.size(); i++ )
	{
		tagTrack& track = tracks[i];
		if( CanTrackQuest(track.questID) )
			AddQuestTrack( track.questID );
	}

	m_pQuest->SetRefresh(TRUE);
}

BOOL QuestTrackFrame::CanTrackQuest( UINT16 questID )
{
	if( m_trackCount >= 50 )
		return FALSE;

	return TRUE;
}
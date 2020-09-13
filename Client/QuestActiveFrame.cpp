/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:44
	filename: 	d:\loong_client\Client\QuestActiveFrame.cpp
	file path:	d:\loong_client\Client
	file base:	QuestActiveFrame
	file ext:	cpp
	author:		leili
	
	purpose:	�̶���������ʵ�֣���������߼�
*********************************************************************/
#include "StdAfx.h"
#include "QuestActiveFrame.h"
#include "QuestMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "ServerTime.h"
#include "CreatureData.h"

//-----------------------------------------------------------------------------
// ���캯��
//-----------------------------------------------------------------------------
QuestActiveFrame::QuestActiveFrame( void )
: m_Trunk(this)
, m_pWnd(0)
, m_pLbActivityList(0)		// �����б�
, m_pPbToday(0)           // ���ջ��ť
, m_pPbDaily(0)			// �ճ�����ť
,m_bIfPbTodayFlash(FALSE)
, m_pStTodayDescOne(0)          // ��һ������
, m_pStTodayDescTwo(0)          // �ڶ�������
, m_pStTodayDescThree(0)        // ����������
, m_pStDailyDescOne(0)          // ��һ������
, m_pStDailyDescTwo(0)          // �ڶ�������
, m_pStDailyDescThree(0)        // ����������
, m_pLbTodayList(0)		// ���ջ�б�
, m_pLbDailyList(0)		// �ճ���б�
, m_pStDesc(0)             // ������������
, m_pPshSwitchCan(0)		// ���˿����
, m_pBtnCloseWnd(0)		// �رմ���X��
, m_pBtnCloseWndButton(0)	// �رմ��ڰ�ť
, m_curSortType(ActivityMgr::EST_SortByTime)
, m_curRow(0)
, m_currentRowforActivity(0)
, m_showCan(true)
{

}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
QuestActiveFrame::~QuestActiveFrame( void )
{
	
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	return TRUE;
}

//-----------------------------------------------------------------------------
// ���¶��봰�ڽ���
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\activity_list.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	/*m_pGUI->ChangeXml( &element, "activity_win\\activity_pic\\listback3\\start", "ClassName", "GUIStatic");
	m_pGUI->ChangeXml( &element, "activity_win\\activity_pic\\listback3\\name", "ClassName", "GUIStatic");
	m_pGUI->ChangeXml( &element, "activity_win\\activity_pic\\listback3\\complet", "ClassName", "GUIStatic");
	m_pGUI->ChangeXml( &element, "activity_win\\activity_pic\\listback4\\start", "ClassName", "GUIStatic");
	m_pGUI->ChangeXml( &element, "activity_win\\activity_pic\\listback4\\name", "ClassName", "GUIStatic");
	m_pGUI->ChangeXml( &element, "activity_win\\activity_pic\\listback4\\complet", "ClassName", "GUIStatic");*/
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pLbActivityList	=	(GUIListBox*)	m_pWnd->GetChild(_T("activity_pic\\listback0\\holiday_list"));		// �����б�
	m_pPbToday			=	(GUIPushButton*)m_pWnd->GetChild(_T("activity_pic\\fraback\\opbutt1"));           // ���ջ��ť
	m_pPbDaily			=	(GUIPushButton*)m_pWnd->GetChild(_T("activity_pic\\fraback\\opbutt2"));			// �ճ�����ť
	m_pStTodayDescOne	=   (GUIPatch*)	m_pWnd->GetChild(_T("activity_pic\\listback3\\grade"));       // ��һ������
	m_pStTodayDescTwo	=   (GUIPatch*)	m_pWnd->GetChild(_T("activity_pic\\listback3\\name"));      // �ڶ�������
	m_pStTodayDescThree =   (GUIPatch*)	m_pWnd->GetChild(_T("activity_pic\\listback3\\complete"));     // ����������
	m_pStDailyDescOne	=   (GUIPatch*)	m_pWnd->GetChild(_T("activity_pic\\listback4\\start"));       // ��һ������
	m_pStDailyDescTwo	=   (GUIPatch*)	m_pWnd->GetChild(_T("activity_pic\\listback4\\name"));       // �ڶ�������
	m_pStDailyDescThree =   (GUIPatch*)	m_pWnd->GetChild(_T("activity_pic\\listback4\\stay"));     // ����������
	m_pLbTodayList		=	(GUIListBox*)	m_pWnd->GetChild(_T("activity_pic\\listback3\\activity_list"));		// ���ջ�б�
	m_pLbDailyList		=	(GUIListBox*)	m_pWnd->GetChild(_T("activity_pic\\listback4\\activity_list"));		// �ճ���б�
	m_pStDesc			=   (GUIStaticEx*)	m_pWnd->GetChild(_T("activity_pic\\listback2\\win\\word"));         // ������������
	m_pPshSwitchCan		=	(GUIPushButton*)m_pWnd->GetChild(_T("activity_pic\\optbutt"));		// ���˿����
	m_pBtnCloseWnd		=	(GUIButton*)	m_pWnd->GetChild(_T("activity_pic\\closebutt"));		// �رմ���X��
	m_pBtnCloseWndButton=	(GUIButton*)	m_pWnd->GetChild(_T("activity_pic\\cancelbutt"));		// �رմ��ڰ�ť

	// ��б��п�����
	m_pLbActivityList->SetColNum(4,20);
	m_pLbActivityList->SetColWidth(0,50);
	m_pLbActivityList->SetColWidth(1,170);
	m_pLbActivityList->SetColWidth(2,170);
	m_pLbActivityList->SetInvisible( FALSE );
	ShowFestival();

	// ���ջ�б��п�����
	m_pLbTodayList->SetColNum(7,20);
	m_pLbTodayList->SetColWidth(0,60);
	m_pLbTodayList->SetColWidth(1,365);
	m_pLbTodayList->SetColWidth(2,5);
	m_pLbTodayList->SetColWidth(3,5);
	m_pLbTodayList->SetColWidth(4,5);
	m_pLbTodayList->SetColWidth(5,60);

	// �ճ���б��п�����
	m_pLbDailyList->SetColNum(4,20);
	m_pLbDailyList->SetColWidth(0,190);
	m_pLbDailyList->SetColWidth(1,260);
	m_pLbDailyList->SetColWidth(2,100);

	m_pLbTodayList->SetInvisible( FALSE );
	m_pLbDailyList->SetInvisible( TRUE );

	m_pPbToday->SetState(EGUIBS_PushDown);


	if( m_showCan )
		m_pPshSwitchCan->SetState( EGUIBS_PushDown, FALSE );
	else
		m_pPshSwitchCan->SetState( EGUIBS_PopUp, FALSE );

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuestActiveFrame::EventHandler));

	m_pWnd->SetInvisible( TRUE );

	ShowQuest();

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������Ϣ�¼�����
//-----------------------------------------------------------------------------
BOOL QuestActiveFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnCloseWnd )//�رմ���
			{	
				QuestMgr::Inst()->CloseActiveFrame();
				m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_QueryBtn_Flash"),NULL));
			}
			if( pWnd == m_pBtnCloseWndButton )//�رմ���
			{	
				QuestMgr::Inst()->CloseActiveFrame();
				m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_QueryBtn_Flash"),NULL));
			}
		}
		break;

	case EGUIE_ItemClick:
		{
			//���ջ
			if (m_pPbToday->GetState() == EGUIBS_PushDown)
			{
				if( pWnd == m_pLbTodayList )
				{
					if( m_curRow >= pEvent->dwParam1
						&& pEvent->msg.dwType == WM_LBUTTONUP
						&& pEvent->dwParam2 >0
						&& pEvent->dwParam2 <5
						&& m_pLbTodayList->GetText(pEvent->dwParam1,pEvent->dwParam2) != _T(""))
					{
						// ��������
						std::map<int,int>::iterator ite = m_curShowQuestPosIDMap.find(pEvent->dwParam1*10+pEvent->dwParam2);
						if (ite != m_curShowQuestPosIDMap.end())
						{
							UINT16 u16ID = ite->second;
							ShowDesc(u16ID);
						}
					}
				}
			}
			//�ճ��
			if (m_pPbDaily->GetState() == EGUIBS_PushDown)
			{
				if( pWnd == m_pLbDailyList )
				{
					if( m_curRow > pEvent->dwParam1
						&& pEvent->msg.dwType == WM_LBUTTONUP)
					{
						UINT16 u16ID = m_curShowQuestIDs[pEvent->dwParam1];
						ShowDailyDesc(u16ID);
					}

				}
			}

			//���ջ
			if ( pWnd == m_pLbActivityList )
			{
				if (m_currentRowforActivity > pEvent->dwParam1
					&& pEvent->msg.dwType == WM_LBUTTONUP)
				{
					std::map<int,int>::iterator ite = m_curShowActivityIDMap.find(pEvent->dwParam1);
					if (ite != m_curShowActivityIDMap.end())
					{
						UINT16 u16ID = ite->second;
						ShowDesc(u16ID);
					}
				}
			}
			
		}
		break;

	case EGUIE_ItemDblClick:
		{
			//if( pWnd == m_pListQuests )
			//{
			//	if( m_curRow > pEvent->dwParam1
			//		&& pEvent->msg.dwType == WM_LBUTTONUP )
			//	{
			//		tagPoint ptLT = m_pWnd->GetClient();
			//		ptLT = ptLT + m_pListQuests->GetFather()->GetFather()->GetClient();
			//		ptLT = ptLT + m_pListQuests->GetFather()->GetClient();
			//		ptLT = ptLT + m_pListQuests->GetClient();
			//		if( pEvent->msg.pt.x > ptLT.x+310
			//			&& pEvent->msg.pt.x < ptLT.x+420 )
			//		{
			//			QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
			//			DWORD npcID = pQuery->GetActiveQuestNPCID( pEvent->dwParam1 );
			//			Vector3 pos;
			//			if( pQuery->GetNPCPosition( npcID, pos ) )//�Զ�Ѱ·
			//			{
			//				LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();

			//				tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
			//				event.start = pLP->GetPos();
			//				event.end = pos;
			//				event.validDist = 100.0f;

			//				m_pMgr->SendEvent( &event );
			//			}
			//		}
			//	}
			//}
		}
		break;

	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pPshSwitchCan )//��ʾ�ʺ�������Ļ
			{
				if( pEvent->dwParam1 == EGUIBS_PushDown )
				{
					m_showCan = true;
				}
				else
				{
					m_showCan = false;
				}

				ShowQuest();
				break;
			}
			if ( pWnd == m_pPbToday ) // ���ջ
			{
				if (m_pPbToday->GetState() == EGUIBS_PushDown)
				{
					m_pLbTodayList->SetInvisible(FALSE);
					m_pLbTodayList->FlipToTop();
					m_pLbDailyList->SetInvisible(TRUE);
					m_curSortType = ActivityMgr::EST_SortByTime;
					ShowQuest();
				}
				break;
			}
			if ( pWnd == m_pPbDaily ) // �ճ��
			{
				if (m_pPbDaily->GetState() == EGUIBS_PushDown)
				{
					m_pLbTodayList->SetInvisible(TRUE);
					m_pLbDailyList->SetInvisible(FALSE);
					m_pLbDailyList->FlipToTop();
					ShowQuest();
				}
				break;
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

VOID QuestActiveFrame::Show( void )
{
	if( P_VALID(m_pWnd) ) 
	{
		m_pWnd->SetInvisible(FALSE); 
		m_pWnd->FlipToTop();
	}
}

VOID QuestActiveFrame::ShowDailyDesc(UINT16 questID)
{
	const tagDailyQuestProtoData *data = ActivityMgr::Inst()->GetData().GetDailyProto(questID);
	if ( P_VALID(data))
	{
		std::vector<tagIDLink> idLinks;
		std::vector<tagPosLink> posLinks;
		tstring strDesc = QuestMgr::Inst()->ParseQuestView(const_cast<tstring&>(data->desc), idLinks, posLinks);
		m_pStDesc->SetText(strDesc.c_str());
		tagPoint size = m_pStDesc->GetSize();
		if( m_pStDesc->GetTextTotalHeight() > size.y )
		{
			size.y = m_pStDesc->GetTextTotalHeight();
			m_pStDesc->SetResizable(TRUE);
			m_pStDesc->Resize( size );
			m_pStDesc->SetResizable(FALSE);
		}
		m_pStDesc->SetRefresh(TRUE);
	}
}
//-----------------------------------------------------------------------------
// ��ʾ�����б���ָ������ʽ
//-----------------------------------------------------------------------------
VOID QuestActiveFrame::ShowQuest( void )
{
	ShowFestival();
	ActivityMgr::Inst()->SortActivities(m_curSortType);		
	m_pLbTodayList->Clear();
	m_pLbDailyList->Clear();

	const ActivityMgr::ActivityVector &activities = ActivityMgr::Inst()->GetActivityVector();
	m_curRow = 0;
	m_curShowQuestIDs.clear();
	m_curShowQuestPosIDMap.clear();


	// ���ջ
	if (m_pPbToday->GetState() == EGUIBS_PushDown)
	{
		INT CurrColCount = 1;
		//bool changeCol = FALSE;
		bool oneTime = TRUE;
		struct tempTime 
		{
			int hour;
			int minute;
			tempTime():hour(0),minute(0)
			{}
		};
		tempTime timeMarkForCol;

		for ( UINT i= 0; i < activities.size(); ++i)
		{
			const tagActivityProtoData &activitydata = activities[i];

			if( !activitydata.visible)
				continue;

			if (activitydata.activityType == ENewActivityType_Festival)//���˽��ջ
				continue;

			DWORD color = GetActivityColour(activitydata.activityType);
			tstring strWeek;
			EWeek curWeek = ServerTime::Inst()->GetCurrentWeekDay();
			INT   curDay  = ServerTime::Inst()->GetCurrentDay();
			INT   curMonth= ServerTime::Inst()->GetCurrentMonth();

			// ����ͬһ��	
			if (activitydata.week != curWeek%7 )
			{
				if(activitydata.mode == 3&&(curMonth>=activitydata.startmonth&&curMonth<=activitydata.endmonth))   //�����ڲ���������
				{
					if(curMonth==activitydata.endmonth&&curDay>activitydata.endday)                                //�����������ж��ڲ���������
						continue;
				}
				else 
					continue;
			}

			// �����Ѿ�����������
			if (CompareTime(activitydata.endHour, activitydata.endMinute, ServerTime::Inst()->GetCurrentHour(), 
				ServerTime::Inst()->GetCurrentMinute()) == false)
			{
				if( m_showCan ) 
					continue;
			}

			// �ж��Ƿ���Ҫ����
			if (m_curRow == 0 && CurrColCount == 1 && oneTime) // ����ǵ�һ�������жϣ�ֱ��д��
			{
				m_pLbTodayList->SetText(m_curRow, 1, activitydata.name.c_str(), color);
				//CurrColCount++;
				timeMarkForCol.hour = activitydata.startHour;
				timeMarkForCol.minute = activitydata.startMinute;
				oneTime = FALSE;
			} 
			else
			{
				// �����ʼʱ����ͬ
				if (SameStartTime(activitydata.startHour, activitydata.startMinute,timeMarkForCol.hour,timeMarkForCol.minute) == true)
				{
					CurrColCount++;
					if (5 == CurrColCount)
					{
						++m_curRow;
						CurrColCount = 1;
						//changeCol = TRUE;
					}
				}
				else
				{
					++m_curRow;
					CurrColCount = 1;// ���б�־λ��0
				}
				m_pLbTodayList->SetText(m_curRow, CurrColCount, activitydata.name.c_str(), color);

			}

			// �Ѿ�����
			if (CompareTime(activitydata.endHour, activitydata.endMinute, ServerTime::Inst()->GetCurrentHour(), 
				ServerTime::Inst()->GetCurrentMinute()) == false)
			{
				DWORD dwcolor = 0xFF808080;//��ɫ
				m_pLbTodayList->SetText(m_curRow, 5, g_StrTable[_T("Activity_Finish")], dwcolor);
				m_bIfPbTodayFlash = FALSE; //
			}
			// δ��ʱ
			else if (CompareTime(activitydata.startHour, activitydata.startMinute, 
				ServerTime::Inst()->GetCurrentHour(), ServerTime::Inst()->GetCurrentMinute()) == true)
			{
				DWORD dwcolor = 0xFFFFF7E0;	//��ɫ
				m_pLbTodayList->SetText(m_curRow, 5, g_StrTable[_T("Activity_UnStart")], dwcolor);
			}
			// ������
			else
			{
				DWORD dwcolor = 0xFF05FF00;//��ɫ
				m_pLbTodayList->SetText(m_curRow, 5, g_StrTable[_T("Activity_Start")], dwcolor);
				//if ( m_bIfClickFlashPb == FALSE )
				//{
				//	m_bIfPbTodayFlash = TRUE;
				//	/*if ( m_bIfPbTodayFlash == FALSE && m_bIfClickFlashPb == FALSE )
				//	{
				//		m_bIfPbTodayFlash = TRUE;
				//		m_bIfClickFlashPb = TRUE;
				//	}*/
				//}
			}

			//���ʱ��λ
			TCHAR szBuffer1[4];
			TCHAR szBuffer2[4];

			_sntprintf(szBuffer1, 4, _T("%02d"), activitydata.startHour);
			_sntprintf(szBuffer2, 4, _T("%02d"), activitydata.startMinute);

			strWeek = strWeek + _T(" ") + szBuffer1 + _T(":") + szBuffer2;
			DWORD dwcolorfortime = 0xFFFFF7E0;	//��ɫ
			m_pLbTodayList->SetText(m_curRow, 0, strWeek.c_str(), dwcolorfortime);
			
			m_curShowQuestPosIDMap.insert(make_pair((m_curRow*10+CurrColCount),activities[i].id));

			timeMarkForCol.hour = activitydata.startHour;
			timeMarkForCol.minute = activitydata.startMinute;

		}	
	}

	// �ճ��
	if (m_pPbDaily->GetState() == EGUIBS_PushDown)
	{
		ActivityMgr::Inst()->SortDaily();	
		const ActivityMgr::DailyVector& dailys = ActivityMgr::Inst()->GetDailyVector();

		for ( UINT i= 0; i < dailys.size(); ++i)
		{
			const tagDailyQuestProtoData &dailydata = dailys[i];
			DWORD color = 0xFFFFF7E0;

			//����ȼ�������
			int RoleLv = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
			if (RoleLv < dailydata.minLevel || RoleLv > dailydata.maxLevel)
			{
				if( m_showCan )
					continue;

				color = 0xFFD04937;
			}

			//���ǰ�ûû���������ʾ
			if( 0 != dailydata.preID && !QuestMgr::Inst()->GetQuery()->QuestIsCompleted( dailydata.preID))
				continue;

			TCHAR szLevel[10];
			_stprintf( szLevel, _T("%d-%d%s"), dailydata.minLevel, dailydata.maxLevel,g_StrTable[_T("Activity_Levels")] );

			m_pLbDailyList->SetText(m_curRow, 0, szLevel, color);
			m_pLbDailyList->SetText(m_curRow, 1, dailydata.name.c_str(), color);
			DWORD color2 = 0xFFFFF7E0;

			INT QusetDoneTime = 0;
			const tagDailyQuestProtoData *data = ActivityMgr::Inst()->GetData().GetDailyProto(dailydata.id);
			if ( P_VALID(data))
			{
				QusetDoneTime = data->doneTimes;
			}

			if(QusetDoneTime > dailydata.acceptTimes)
				QusetDoneTime = dailydata.acceptTimes;

			tstringstream ss;
			ss << QusetDoneTime <<_T("/") << dailydata.acceptTimes;
			DWORD colorGreen = 0xFF05FF00;
			if ( QusetDoneTime == dailydata.acceptTimes)
			{
				m_pLbDailyList->SetText(m_curRow, 2, ss.str().c_str(), colorGreen);
			}
			else
			{
				m_pLbDailyList->SetText(m_curRow, 2, ss.str().c_str(), color2);
			}

			m_curShowQuestIDs.push_back(dailys[i].id);

			++m_curRow;
		}	
	}
}

//-----------------------------------------------------------------------------
// ��ʾ��������
//-----------------------------------------------------------------------------
VOID QuestActiveFrame::ShowDesc( UINT16 questID )
{
	
	const tagActivityProtoData *data = ActivityMgr::Inst()->GetData().GetQuestProto(questID);
	if( P_VALID(data) )
	{
		std::vector<tagIDLink> idLinks;
		std::vector<tagPosLink> posLinks;
		tstring strDesc = QuestMgr::Inst()->ParseQuestView(const_cast<tstring&>(data->desc), idLinks, posLinks);
		m_pStDesc->SetText(strDesc.c_str());
		tagPoint size = m_pStDesc->GetSize();
		if( m_pStDesc->GetTextTotalHeight() > size.y )
		{
			size.y = m_pStDesc->GetTextTotalHeight();
			m_pStDesc->SetResizable(TRUE);
			m_pStDesc->Resize( size );
			m_pStDesc->SetResizable(FALSE);
		}
		m_pStDesc->SetRefresh(TRUE);
	}
}

BOOL QuestActiveFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
	{
		QuestMgr::Inst()->CloseActiveFrame();
		return TRUE;
	}

	return GameFrame::EscCancel();
}

DWORD QuestActiveFrame::GetActivityColour(ENewActivityType eType)
{
	DWORD color = 0;
	switch (eType)
	{
	case ENewActivityType_Festival:
		color = 0xFFFFF7E0;//��ɫ
		break;
	case ENewActivityType_Scene:
		color = 0xFFFFF7E0;//��ɫ
		break;
	case ENewActivityType_Fallow:
		color = 0xFF05FF00;//��ɫ
		break;
	case ENewActivityType_PVP:
		color = 0xFFFF402D;//��ɫ
		break;
	case ENewActivityType_Instance:
		color = 0xFFFFAE00;//��ɫ
		break;
	case ENewActivityType_Shuaguai:
		color = 0xFF00A2FF;//��ɫ
		break;
	case ENewActivityType_Daily:
		color = 0xFFFFF7E0;//��ɫ
		break;
	default:
		color = 0xFFFFF7E0;//��ɫ
		break;
	}
	return color;
}

VOID QuestActiveFrame::ShowFestival()
{
	m_currentRowforActivity = 0;
	ActivityMgr::Inst()->SortActivities(ActivityMgr::EST_SortByMonthAndDay);		
	m_pLbActivityList->Clear();
	m_curShowActivityIDMap.clear();
	const ActivityMgr::ActivityVector &activities = ActivityMgr::Inst()->GetActivityVector();
	for ( UINT i= 0; i < activities.size(); ++i)
	{
		tstring strWeek;
		const tagActivityProtoData &activitydata = activities[i];

		if (activitydata.activityType != ENewActivityType_Festival)
			continue;

		// skip the actictiy which's type is every week
		if (   0 == activitydata.startmonth 
			&& 0 == activitydata.endmonth
			&& 0 == activitydata.startday
			&& 0 == activitydata.endday)
		{
				continue;
		}
		//��������޳��Ѿ����ڵĽ��ջ
		if((activitydata.startyear+2000)<ServerTime::Inst()->GetCurrentYear()&&(activitydata.endyear+2000)<ServerTime::Inst()->GetCurrentYear())
		{
			continue;
		}
		// skip the actictiy which's end time is bigger than today
		if (CompareDay(activitydata.endmonth, activitydata.endday, ServerTime::Inst()->GetCurrentMonth(), 
			ServerTime::Inst()->GetCurrentDay()) == false)
		{
				continue;
		}

		DWORD color = 0xFFFFF7E0;

		//���ʱ��λ
		TCHAR szBuffer1[4];
		TCHAR szBuffer2[4];
		TCHAR szBuffer3[4];
		TCHAR szBuffer4[4];

		_sntprintf(szBuffer1, 4, _T("%02d"), activitydata.startmonth);
		_sntprintf(szBuffer2, 4, _T("%02d"), activitydata.startday);
		_sntprintf(szBuffer3, 4, _T("%02d"), activitydata.endmonth);
		_sntprintf(szBuffer4, 4, _T("%02d"), activitydata.endday);

		strWeek = strWeek + _T(" ") + szBuffer1 + g_StrTable[_T("Activity_Month")] + szBuffer2 +g_StrTable[_T("Activity_Day")]
						+ _T(" - ") + szBuffer3 + g_StrTable[_T("Activity_Month")] + szBuffer4 +g_StrTable[_T("Activity_Day")];


		m_pLbActivityList->SetText(m_currentRowforActivity, 1, strWeek.c_str(), color);
		m_pLbActivityList->SetText(m_currentRowforActivity, 2, activitydata.name.c_str(), color);

		m_curShowActivityIDMap.insert(make_pair(m_currentRowforActivity,activities[i].id));

		++m_currentRowforActivity;
	}	
}
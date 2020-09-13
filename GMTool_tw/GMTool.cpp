/////////////////////////////////////////////////////////////////////////////
// Program:     GMTool
// Name:        GMTool.cpp
// Created:     2009-02-04
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wx/wx.h"
#include "wx/busyinfo.h"
#include "wx/panel.h"
#include "wx/bookctrl.h"
#include "GMTool.h"
#include "common.h"

#include "DoublePage.h"
#include "PmdPage.h"
#include "YxjPage.h"
#include "ZdxhPage.h"
#include "GmMgrPage.h"
#include "RolePage.h"
#include "AutoChat.h"
#include "AccountPage.h"
#include "resource.h"
#include "NetSessionMgr.h"
#include "UserFrameMgr.h"
#include "DataMgr.h"
#include "ItemProtoData.h"
#include "GuildPage.h"
#include "..\ServerDefine\gm_tool_define.h"
#include "..\ServerDefine\msg_rt_c.h"
#include "../ServerDefine/msg_gm_tool_c.h"

#define TIMER_ID 2000

extern bool g_bGMserverCallback;

//�������WinMain
IMPLEMENT_APP(MyApp)

// ����log�ļ�
CLog g_AFLog(_T("GMTool_log(�˺ŷ�ͣ).txt"));
CLog g_PMDLog(_T("GMTool_log(�����).txt"));
CLog g_RNLog(_T("GMTool_log(���½�).txt"));
CLog g_CNLog(_T("GMTool_log(����Ƶ������).txt"));
CLog g_MRLog(_T("GMTool_log(�౶����).txt"));

tstring g_Tester;

//----------------------------------------------------------------------------
// MyApp 
//----------------------------------------------------------------------------
bool MyApp::OnInit()
{
	if(!wxApp::OnInit())
		return false;

	// ��ʼ�������ļ�
	if( !g_StrTable.Init(NULL, _T("gmtool_tw.xml")) )
	{
		IMSG(_T("Init gmtool_tw.xml Fail"));
		return false;
	}

	// ��ʼ������
	new NetSessionMgr;
	if( !sNetSessionMgrPtr->Init() )
	{
		IMSG(_T("Init NetSession Fail"));
		return false;;
	}

	new UserFrameMgr;
	new DataMgr;
	ItemProtoData::Inst()->LoadFromFile();

	// ��������������Ϣ�߳�
	sNetSessionMgrPtr->CreateThread();
	//��¼
	MyDialog *dialog = new MyDialog(wxT("SM_GMTool"));
	dialog->Show(true);

	return true;
}

int MyApp::OnExit()
{
	if( P_VALID(sNetSessionMgrPtr) )
	{
		delete sNetSessionMgrPtr;
	}

	if( P_VALID(sUserFrameMgrPgr) )
	{
		delete sUserFrameMgrPgr;
	}
	if( P_VALID(sDataMgrPtr) )
	{
		delete sDataMgrPtr;
	}
	return 0;
}

//----------------------------------------------------------------------------
// MyDialog
//----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyDialog, wxDialog)
	EVT_BUTTON(wxID_OK, MyDialog::OnOk)
	EVT_BUTTON(wxID_CANCEL, MyDialog::OnCancel)
	//EVT_TIMER(TIMER_ID, MyDialog::OnTimer)
	EVT_CLOSE(MyDialog::OnClose)
END_EVENT_TABLE()

MyDialog::MyDialog(const wxString& title) :wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300,150))
//,m_timer(this, TIMER_ID)
{
	wxSizer *sizeTop = new wxBoxSizer(wxVERTICAL);

	wxSizer *sizeRow = NULL;
	sizeRow = CreateSizerWithTextAndLabel(this, g_StrTable[_T("Account")], wxID_ANY, &m_textName);
	sizeTop->Add(sizeRow, 0, wxALL | wxGROW, CONTROL_BORDER);
	sizeRow = CreateSizerWithTextAndLabel(this, g_StrTable[_T("PassWord")], wxID_ANY, &m_textPassword, true);
	sizeTop->Add(sizeRow, 0, wxALL | wxGROW, CONTROL_BORDER);

	m_btnOk		= new wxButton(this, wxID_OK, g_StrTable[_T("OK")]);
	m_btnCancel = new wxButton(this, wxID_CANCEL, g_StrTable[_T("Cancel")]);
	wxSizer *sizeBtn = new wxBoxSizer(wxHORIZONTAL);
	sizeBtn->Add(m_btnOk, 0, wxALL, CONTROL_BORDER);
	sizeBtn->Add(m_btnCancel, 0, wxALL, CONTROL_BORDER);

	sizeTop->Add(sizeBtn, 0, wxALIGN_CENTER);

	//m_timer.Start(200);
	m_bConnect = false;

	SetSizer(sizeTop);
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagLogin"), (FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnLoginEvent));
}

DWORD MyDialog::OnLoginEvent(tagUserEvent* pGameEvent)
{
	tagLoginEvent* pEvt = (tagLoginEvent*) pGameEvent;
	if( pEvt->strEventName==_T("tagLogin") && pEvt->dwErrorCode==2 )
	{
		MyFrame* frame = (MyFrame*)FindWindow(MyFrame_ID);
		if( !P_VALID(frame) )
		{
			new MyFrame(this, wxT("SM_GMTool"), MyFrame_ID);

			tagNGMTC_LoadLog send1;
			send1.eType1 = EAT_DoubleStart;
			send1.eType2 = EAT_DoubleEnd;
			sNetSessionMgrPtr->SendMsg(&send1, send1.dwSize);

			tagNGMTC_LoadLog send2;
			send2.eType1 = EAT_AutoChatStart;
			send2.eType2 = EAT_AutoChatEnd;
			sNetSessionMgrPtr->SendMsg(&send2, send2.dwSize);

			tagNGMTC_LoadLog send3;
			send3.eType1 = EAT_AutoNoticeStart;
			send3.eType2 = EAT_AutoNoticeEnd;
			sNetSessionMgrPtr->SendMsg(&send3, send3.dwSize);

			tagNGMTC_LoadLog send4;
			send4.eType1 = EAT_NoticeStart;
			send4.eType2 = EAT_NoticeEnd;
			sNetSessionMgrPtr->SendMsg(&send4, send4.dwSize);

			tagNGMTC_LoadLog send5;
			send5.eType1 = EAT_AccountBanned;
			send5.eType2 = EAT_UndoAccountBanned;
			sNetSessionMgrPtr->SendMsg(&send5, send5.dwSize);
		}	
	}
	else if( pEvt->dwErrorCode == 0 )
	{
		MyFrame* frame = (MyFrame*)FindWindow(MyFrame_ID);
		if( P_VALID(frame) )
		{
			frame->CenterOnScreen();
			frame->Show();
			this->Hide();
		}
	}
	else if( pEvt->dwErrorCode!=0 )
	{
		sNetSessionMgrPtr->StopTryingConnect();
		wxMessageBox(g_StrTable[_T("UserOrPswError")], wxT(""), wxOK|wxCENTRE, this);
	}
	return 0;
}

//void MyDialog::OnTimer(wxTimerEvent& event)
//{
//
//}

void MyDialog::OnOk(wxCommandEvent &event)
{	
	g_Tester = m_textName->GetValue();

	while(true)
	{
		// �����Ϸ����û���������
		if( sNetSessionMgrPtr->IsConnected() )
		{
			sNetSessionMgrPtr->SetLoginState(ELS_Connect_Success);	
			tstring strName = m_textName->GetValue();
			tstring strPsw = m_textPassword->GetValue();
			tagNRTC_Challenge msg;
			_sntprintf( msg.szAccountName, X_SHORT_NAME, strName.c_str() );
			msg.dwPsdCrc = Crc32( strPsw.c_str() );
			sNetSessionMgrPtr->SendMsg( &msg, msg.dwSize );

			while (!g_bGMserverCallback && sNetSessionMgrPtr->IsConnected())
			{
				Sleep(200);
			}
			
			tagLoginEvent evt(_T("tagLogin"));
			evt.dwErrorCode = 2;
			sUserFrameMgrPgr->SendEvent(&evt);

			g_bGMserverCallback = false;
			break;
		}

		// ��Ȼ�ڳ�������
		if( sNetSessionMgrPtr->IsTryingToConnect() )	
			continue;	

		// ����ʧ��
		if( sNetSessionMgrPtr->GetLoginState() == ELS_Connect )	
		{
			m_dwConnectTime = timeGetTime();
			wxBusyInfo info(g_StrTable[_T("ConnectFail")], this);
			sNetSessionMgrPtr->SetLoginState(ELS_Connect_Fail);
			this->Suspend();
			sNetSessionMgrPtr->SetLoginState(ELS_NULL);
			break;
		}

		tstring strIP = g_StrTable[_T("IP")];
		INT nPort = _tstoi(g_StrTable[_T("Port")]);

		// ��������
		sNetSessionMgrPtr->TryToConnect( g_Util.UnicodeToUnicode8(strIP.c_str()), nPort );
		wxBusyInfo info(g_StrTable[_T("Connecting")], this);
		sNetSessionMgrPtr->SetLoginState(ELS_Connect);
	}
}

void MyDialog::Suspend()
{
	for( int i = 0; i < 18; i++ )
	{
		wxTheApp->Yield();
	}
	wxSleep(2);
}

void MyDialog::OnCancel(wxCommandEvent& event)
{
	Destroy();
}

void MyDialog::OnClose(wxCloseEvent& event)
{
	Destroy();
}

//----------------------------------------------------------------------------
// MyFrame
//----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_CLOSE(MyFrame::OnClose)
	EVT_CHECKBOX(LoongGMTool_SelectAll, MyFrame::OnSelectAll)
	EVT_CHECKLISTBOX(LoongGMTool_ServerList, MyFrame::OnSelectCheckListBox)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent, const wxString& title, INT nID) : wxFrame(parent, nID, title, wxPoint(0, 0), wxSize(980, 600), 
												  wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE|wxCLIP_CHILDREN|wxTAB_TRAVERSAL)
{
	//m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN);
	m_panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL, "");
	m_panel->SetScrollbars(20,20,50,50);
	m_panel->EnableScrolling(true, true);

	wxSizer *sizeAll = new wxBoxSizer(wxHORIZONTAL);

	//����-��
	wxSizer* sizerLeft = new wxBoxSizer(wxVERTICAL);

	//�������б�
	wxStaticBox *box = new wxStaticBox(m_panel, wxID_ANY, g_StrTable[_T("Server")]);
	wxSizer* sizerLeftBox = new wxStaticBoxSizer(box, wxVERTICAL);
	int flags = 0;
	flags = wxLB_MULTIPLE | wxLB_HSCROLL;
	m_lbox = new wxCheckListBox(m_panel, LoongGMTool_ServerList, wxDefaultPosition, wxDefaultSize, 0, NULL, flags);
	sizerLeftBox->Add(m_lbox, 1, wxGROW | wxALL, CONTROL_BORDER);

	sUserFrameMgrPgr->RegisterEventHandle(_T("tagServerStatInfoEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnServerStatInfoEvent));
	sUserFrameMgrPgr->RegisterEventHandle(_T("tagTimeLimited"), (FRAMEEVENTPROC)m_Trunk.sfp1(&UserFrame::OnTimeLimited));
	
	m_chkSelectAll = CreateCheckBoxAndAddToSizer(m_panel, sizerLeftBox, g_StrTable[_T("SelectAll")], LoongGMTool_SelectAll);
	sizerLeft->Add(sizerLeftBox, 1, wxGROW|wxALL, CONTROL_BORDER);
	sizerLeft->SetMinSize(170, 540);
	//����-��
	wxSizer *sizeRight = new wxBoxSizer(wxHORIZONTAL);

	int style = wxNO_FULL_REPAINT_ON_RESIZE|wxCLIP_CHILDREN;
	m_book = new wxBookCtrl(m_panel,wxID_ANY, wxDefaultPosition, wxDefaultSize, style);

	GmMgrPage* gmmgr_page = new GmMgrPage(m_book, g_StrTable[_T("GMMgrPage")]);
	if( P_VALID(gmmgr_page) ) gmmgr_page->SetMyFrame(this);

	AccountPage* account_page = new AccountPage(m_book, g_StrTable[_T("AccountPage")]);
	if( P_VALID(account_page) ) account_page->SetMyFrame(this);
	
	RolePage* role_page = new RolePage(m_book, g_StrTable[_T("RolePage")]);
	if( P_VALID(role_page) ) role_page->SetMyFrame(this);
	
	DoublePage* double_page = new DoublePage(m_book, g_StrTable[_T("DoublePage")]);
	if( P_VALID(double_page) )	double_page->SetMyFrame(this);
	
	ZdxhPage* zdxh_page = new ZdxhPage(m_book, g_StrTable[_T("ZdxhPage")]);
	if( P_VALID(zdxh_page) ) zdxh_page->SetMyFrame(this);
	
	YxjPage* yxj_page = new YxjPage(m_book, g_StrTable[_T("YxjPage")]);
	if( P_VALID(yxj_page) ) yxj_page->SetMyFrame(this);
	
	AutoChatPage* autochat_page = new AutoChatPage(m_book, g_StrTable[_T("AutoChatPage")]);
	if ( P_VALID(autochat_page) ) autochat_page->SetMyFrame(this);

	GuildPage* guild_page = new GuildPage(m_book, g_StrTable[_T("GuildPage")]);
	if ( P_VALID(guild_page) ) guild_page->SetMyFrame(this);

	m_book->AddPage(gmmgr_page, gmmgr_page->GetLabel(), false);
	m_book->AddPage(account_page, account_page->GetLabel(), false);
	m_book->AddPage(role_page, role_page->GetLabel(), false);
	m_book->AddPage(guild_page, guild_page->GetLabel(), false);
	m_book->AddPage(double_page, double_page->GetLabel(), false);
	//m_book->AddPage(pmd_page, pmd_page->GetLabel(), false);
	m_book->AddPage(zdxh_page, zdxh_page->GetLabel(), false);
	m_book->AddPage(yxj_page, yxj_page->GetLabel(), false);
	m_book->AddPage(autochat_page, autochat_page->GetLabel(), false);
	sizeRight->Add(m_book, 1, wxALL| wxGROW, CONTROL_BORDER);
	
	sizeAll->Add(sizerLeft, 0, wxGROW|wxALL, CONTROL_BORDER);
	sizeAll->Add(sizeRight, 1, wxGROW|wxALL, CONTROL_BORDER);
	m_panel->SetSizer(sizeAll);
	m_bSelectAll = false;
}

MyFrame::~MyFrame()
{
	
}

void MyFrame::OnClose(wxCloseEvent &event)
{
	Destroy();
	GetParent()->Destroy();
}

void MyFrame::OnSelectAll(wxCommandEvent& event)
{
	int count = m_lbox->GetCount();
	if (count <= 0)
		return;
	if (m_bSelectAll == true)
	{
		for (int i = 0; i < count; i++)
		{
			m_lbox->Check(i, false);
		}
		m_bSelectAll = false;
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			m_lbox->Check(i, true);
			tstring strSerName = m_lbox->GetString( (unsigned int)i );
			tagSWId id;
			sDataMgrPtr->GetSectionAndWorldID(strSerName, &id);
			if( sDataMgrPtr->JudgeWorldListState(id.section_id, id.world_id) != EWS_Well )  
			{
				TCHAR szBuff[128] = {0};
				_sntprintf( szBuff, 128, g_StrTable[_T("ServerNoWell")], strSerName.c_str() );
				wxMessageBox(szBuff, wxT(""), wxOK|wxCENTRE, this);
				m_lbox->Check( (unsigned int)i, false );
			}

		}
		m_bSelectAll = true;
	}
	
}

void MyFrame::GetCheckListBoxChecked( std::vector<int>& vec )
{
	int count = m_lbox->GetCount();
	if (count <= 0)
		return;
	std::vector<int> vecTemp;
	for (int i = 0; i < count; i++)
	{
		if(m_lbox->IsChecked(i))
		{
			vecTemp.push_back(i);
		}
	}
	vec = vecTemp;
}

void MyFrame::OnSelectCheckListBox( wxCommandEvent& evet )
{
	int nSelected = evet.GetSelection();
	if( m_lbox->IsChecked( (unsigned int)nSelected ) )
	{
		wxString cmd_String = evet.GetString();
		tstring strSerName = cmd_String.wx_str();
		tagSWId id;
		sDataMgrPtr->GetSectionAndWorldID(strSerName, &id);
		if( sDataMgrPtr->JudgeWorldListState(id.section_id, id.world_id) != EWS_Well )  
		{
			TCHAR szBuff[128] = {0};
			_sntprintf( szBuff, 128, g_StrTable[_T("ServerNoWell")], strSerName.c_str() );
			wxMessageBox(szBuff, wxT(""), wxOK|wxCENTRE, this);
			m_lbox->Check( (unsigned int)nSelected, false );
		}
	}
}

DWORD MyFrame::OnServerStatInfoEvent( tagUserEvent* pGameEvent )
{
	//--������Ϸ����
	if( P_VALID(m_lbox) )
	{
		std::list<tagWorld*>* world = sDataMgrPtr->GetWorldDataList();
		std::list<tagWorld*>::iterator it = world->begin();
		for( ; it!=world->end(); it++ )
		{
			m_lbox->Append((*it)->world_name);
		}
	}
	return 0;
}

DWORD MyFrame::OnTimeLimited( tagUserEvent* pGameEvent )
{
	if( pGameEvent->strEventName == _T("tagTimeLimited") )
	{
		tagTimeLimited* pEvt = (tagTimeLimited*)pGameEvent;
		switch( pEvt->dwErrorCode )
		{
		case E_GMTool_Success:
			{
				TCHAR szBuff[X_SHORT_NAME] = {0};
				tstring strWorldName = sDataMgrPtr->GetWorldNameByWorldID(pEvt->dwWorldID);
				if( pEvt->dwType == ETLM_Double )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("OpenDouble0")], strWorldName.c_str() );
					//g_MRLog.Note(_T("!!!!! Success !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_RightNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("YxjNotice0")], strWorldName.c_str() );	
					//g_RNLog.Note(_T("!!!!! Success !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_AutoNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("AutoCast0")], strWorldName.c_str() );
					//g_PMDLog.Note(_T("!!!!! Success !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_AutoChatNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("AutoChatNotice0")], strWorldName.c_str() );
					//g_CNLog.Note(_T("!!!!! Success !!!!!\n"));
				}
				wxMessageBox(szBuff, wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_TimeSetError:
			{
				TCHAR szBuff[X_SHORT_NAME] = {0};
				tstring strWorldName = sDataMgrPtr->GetWorldNameByWorldID(pEvt->dwWorldID);
				if( pEvt->dwType == ETLM_Double )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("OpenDouble2")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					DoublePage* pPage = (DoublePage*)pbook->GetPage(EGMP_Double);
					pPage->DoubleOperationStatus(FALSE);
					//g_MRLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_RightNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("YxjNotice2")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					YxjPage* pPage = (YxjPage*)pbook->GetPage(EGMP_RightNotice);
					pPage->YxjOperationStatus(FALSE);
					//g_RNLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_AutoNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("AutoCast2")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					ZdxhPage* pPage = (ZdxhPage*)pbook->GetPage(EGMP_AutoNotice);
					pPage->ZdxhOperationStatus(FALSE);
					//g_PMDLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_AutoChatNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("AutoChatNotice1")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					AutoChatPage* pPage = (AutoChatPage*)pbook->GetPage(EGMP_AutoChat);
					pPage->AutoChatOperationStatus(FALSE);
					//g_CNLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				wxMessageBox(szBuff, wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		case E_GMTool_Err:
			{
				TCHAR szBuff[X_SHORT_NAME] = {0};
				tstring strWorldName = sDataMgrPtr->GetWorldNameByWorldID(pEvt->dwWorldID);
				if( pEvt->dwType == ETLM_Double )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("OpenDouble1")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					DoublePage* pPage = (DoublePage*)pbook->GetPage(EGMP_Double);
					pPage->DoubleOperationStatus(FALSE);
					//g_MRLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_RightNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("YxjNotice1")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					YxjPage* pPage = (YxjPage*)pbook->GetPage(EGMP_RightNotice);
					pPage->YxjOperationStatus(FALSE);
					//g_RNLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_AutoNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("AutoCast1")], strWorldName.c_str() );	
					wxBookCtrlBase* pbook = GetBook();
					ZdxhPage* pPage = (ZdxhPage*)pbook->GetPage(EGMP_AutoNotice);
					pPage->ZdxhOperationStatus(FALSE);
					//g_PMDLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				else if( pEvt->dwType == ETLM_AutoChatNotice )
				{
					_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("AutoChatNotice2")], strWorldName.c_str() );
					wxBookCtrlBase* pbook = GetBook();
					AutoChatPage* pPage = (AutoChatPage*)pbook->GetPage(EGMP_AutoChat);
					pPage->AutoChatOperationStatus(FALSE);
					//g_CNLog.Note(_T("!!!!! Error Occur !!!!!\n"));
				}
				wxMessageBox(szBuff, wxT(""), wxOK|wxCENTRE, this);
			}
			break;
		}
	}

	return 0;
}

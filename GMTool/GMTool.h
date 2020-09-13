/////////////////////////////////////////////////////////////////////////////
// Program:     LoongGMTool
// Name:        LoongGMTool.h
// Created:     2009-02-04
/////////////////////////////////////////////////////////////////////////////

#ifndef LOONGGMTOOL_H_
#define LOONGGMTOOL_H_
#include "UserFrame.h"
#include "log.h"
class MyPageInfo;

extern tstring g_Tester;
extern CLog g_PMDLog;
extern CLog g_RNLog;
extern CLog g_CNLog;
extern CLog g_AFLog;
extern CLog g_MRLog;

//GMTool��ҳ��ö��ֵ
enum EGMToolPages
{
	EGMP_Start,
	EGMP_GMManger					= 0,//GM����
	EGMP_AccountManager				= 1,//�ʺŹ���
	EGMP_RoleInfo					= 2,//��ɫ��Ϣ
	EGMP_Guild						= 3,//������Ϣ
	EGMP_Double						= 4,//����˫��
	EGMP_AutoNotice					= 5,//�Զ�ѭ�������
	EGMP_RightNotice				= 6,//���½ǹ���
	EGMP_AutoChat					= 7,//����Ƶ������


	EGMP_End						= 8,
};
//�������
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
};

//��ģʽ�Ի���
class MyDialog : public wxDialog, public UserFrame
{
public:
	MyDialog(const wxString& title);

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	//void OnTimer(wxTimerEvent& event);

	//--����2��
	void Suspend();

	DWORD OnLoginEvent(tagUserEvent* pGameEvent);

private:
	//TextCtrl
	wxTextCtrl *m_textName;
	wxTextCtrl *m_textPassword;

	//Button
	wxButton   *m_btnOk;
	wxButton   *m_btnCancel;

	//wxTimer		m_timer;
	DWORD		m_dwConnectTime;
	BOOL		m_bConnect;

private:
	DECLARE_EVENT_TABLE()
};

#define MyFrame_ID 9527
//Frame
class MyFrame : public wxFrame, public UserFrame
{
public:
	MyFrame(wxWindow *parent, const wxString& title, INT nID);
	~MyFrame();
	
	void OnClose(wxCloseEvent& event);
	void OnSelectAll(wxCommandEvent& event);
	void OnSelectCheckListBox(wxCommandEvent& evet);

	//�ȵ����÷������б�
	wxCheckListBox* GetCheckListBox() { return m_lbox; }
	wxBookCtrlBase* GetBook() {return m_book;}

	void GetCheckListBoxChecked(std::vector<int>& vec);

	DWORD OnServerStatInfoEvent(tagUserEvent* pGameEvent);
	DWORD OnTimeLimited(tagUserEvent* pGameEvent);

private:
	//wxPanel		   *m_panel;
	wxScrolledWindow   *m_panel;
	wxBookCtrlBase *m_book;
	wxCheckListBox *m_lbox;
	wxCheckBox     *m_chkSelectAll;
	bool            m_bSelectAll;
	DECLARE_EVENT_TABLE()
};
#endif /* LOONGGMTOOL_H_ */
#pragma once

#include "SemiTranDlg/TranslucentDialog.h"



// LauncherStdDlg dialog

class CLauncherStdDlg : public CTranslucentDialog
{
public:
	CLauncherStdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherStdDlg();

// Dialog Data
	enum { IDD = IDD_STANDARDDIALOG };


	virtual CStandardDialog* CreateStandardDialog();
	virtual void OnInitChildrenWnds();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	//��Ϣ����
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

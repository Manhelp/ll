// LauncherStdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "launcher.h"
#include "LauncherDlg.h"
#include "LauncherStdDlg.h"


// LauncherStdDlg dialog

CLauncherStdDlg::CLauncherStdDlg(CWnd* pParent /*=NULL*/)
	: CTranslucentDialog(CLauncherStdDlg::IDD, _T("launcher\\tempback.png"), pParent)
{

}

CLauncherStdDlg::~CLauncherStdDlg()
{
}


CStandardDialog* CLauncherStdDlg::CreateStandardDialog()
{
	return ::new CLauncherDlg(this);
}

void CLauncherStdDlg::OnInitChildrenWnds()
{
}

void CLauncherStdDlg::DoDataExchange(CDataExchange* pDX)
{
	CTranslucentDialog::DoDataExchange(pDX);
}


//-----------------------------------------------------------------------------
// CLauncherDlg ��Ϣ�������
//-----------------------------------------------------------------------------
BOOL CLauncherStdDlg::OnInitDialog()
{
	CTranslucentDialog::OnInitDialog();
	return TRUE;
}

//-----------------------------------------------------------------------------------------------
//��Ϣ����
//-----------------------------------------------------------------------------------------------
BOOL CLauncherStdDlg::PreTranslateMessage(MSG* pMsg)
{
	return CTranslucentDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------------------------
// ��Ϣ����
//-----------------------------------------------------------------------------------------------
LRESULT CLauncherStdDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CTranslucentDialog::WindowProc(message, wParam, lParam);
}


BEGIN_MESSAGE_MAP(CLauncherStdDlg, CTranslucentDialog)
END_MESSAGE_MAP()


// LauncherStdDlg message handlers

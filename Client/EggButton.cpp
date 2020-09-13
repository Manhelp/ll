#include "StdAfx.h"
#include "EggButton.h"

const TCHAR* szEggPic = _T("data\\ui\\Main\\egg.bmp");

EggButton::EggButton(void)
{
}

EggButton::~EggButton(void)
{
}

BOOL EggButton::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRet = ButtonEx::Init(pSystem, pFather, pXmlElement);

	return bRet;
}

VOID EggButton::Destroy()
{
	ButtonEx::Destroy();
}

BOOL EggButton::OnInputMessage(tagGUIInputMsg* pMsg)
{
	if( m_bDisable )
		return ButtonEx::OnInputMessage(pMsg);

	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		if( m_pSystem->GetMousePoint() == this ) // if( PointInWnd(pMsg->pt) )
		{
			m_eState = EGUIBS_PushDown;
			m_bHaveMouseKeyDown = true;
			SetRefresh();
			//m_pRender->PlaySound(_T("button_down"), 0.7f);
		}
		break;

	case WM_MOUSEMOVE:
		if( m_pSystem->GetMousePoint() == this )
		{ 
			if( (MK_LBUTTON & pMsg->dwParam1) && m_bHaveMouseKeyDown )
				m_eState = EGUIBS_PushDown;
			else
				m_eState = EGUIBS_HighLight;
		}
		else
		{
			m_eState = EGUIBS_PopUp;	// ��갴�º��뿪����ť��Ҫ�ɿ�
		}
		SetRefresh();
		break;

	case WM_LBUTTONUP:
		if( m_bHaveMouseKeyDown && PointInWnd(pMsg->pt) )
		{
			tagGUIEvent event(this, (EGUIEvent)EGUIUD_EggClick, pMsg);
			event.dwParam1 =m_nLayer;				// �ڼ����
			event.dwParam2 =m_nPos;					// �ڼ�����
			event.dwParam3 =m_bIsColorEgg;		// �ǲ��ǲʵ�
			SendEvent(&event);
			m_eState = EGUIBS_HighLight;
		}
		else
			m_eState = EGUIBS_PopUp;
		m_bHaveMouseKeyDown = false;
		SetRefresh();
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}

VOID EggButton::OnEvent(tagGUIEvent* pEvent)
{
	
}

VOID EggButton::InitEggData(int layer,int pos,BOOL IsColour /*= FALSE*/)
{
	m_nLayer = layer;					// ����
	m_nPos = pos;						// λ��
	m_bIsColorEgg = IsColour;			//	�ǲ��ǲʵ�

}

VOID EggButton::Change2NormalEgg()
{
	tagRect rect = tagRect(666,0,724,62) ;
	SetWndPicRect(EWPICTYPE_CURPIC,rect);
	rect = tagRect(788,0,846,62) ;
	SetWndPicRect(EWPICTYPE_PUSHPIC,rect);
	rect = tagRect(727,0,785,62) ;
	SetWndPicRect(EWPICTYPE_MOVEPIC,rect);

	SetWndPic(EWPICTYPE_CURPIC,szEggPic);
	SetWndPic(EWPICTYPE_PUSHPIC,szEggPic);
	SetWndPic(EWPICTYPE_MOVEPIC,szEggPic);
	SetRefresh(TRUE);
}

VOID EggButton::Change2ColuorEgg()
{
	tagRect rect = tagRect(666,65,724,127) ;
	SetWndPicRect(EWPICTYPE_CURPIC,rect);
	rect = tagRect(788,65,846,127) ;
	SetWndPicRect(EWPICTYPE_PUSHPIC,rect);
	rect = tagRect(727,65,785,127) ;
	SetWndPicRect(EWPICTYPE_MOVEPIC,rect);

	SetWndPic(EWPICTYPE_CURPIC,szEggPic);
	SetWndPic(EWPICTYPE_PUSHPIC,szEggPic);
	SetWndPic(EWPICTYPE_MOVEPIC,szEggPic);
	SetRefresh(TRUE);
}

VOID EggButton::Render()
{
	GUIButton::Render();
}
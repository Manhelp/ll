#include "StdAfx.h"
#include "GlintButton.h"

GlintButton::GlintButton(void)
{
	m_bShowGlint = false;
	m_bShowInterval = false;
	m_strGlintText = _T("");
	m_dwGlintInterval = 400;	//����
	m_pGlintPic = NULL;
}

GlintButton::~GlintButton(void)
{
}

BOOL GlintButton::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRet = GUIButton::Init( pSystem, pFather, pXmlElement );

	m_dwGlintTextColor = m_dwPushTextColor;
	m_strGlintText = m_strText;
	m_dwGlintPicColor = m_dwPushPicColor;
	return bRet;
}

VOID GlintButton::Destroy()
{
	GUIButton::Destroy();

	if( P_VALID(m_pGlintPic) )
	{
		m_pRender->DestroyImage( m_pGlintPic );
		m_pGlintPic = NULL;
	}
}

VOID GlintButton::Update()
{
	GUIButton::Update();
	SetRefresh( TRUE );

	if( !m_bShowGlint )
		return;

	DWORD dwCurrentTime = timeGetTime();
	if( (dwCurrentTime - m_dwLastGlint) < m_dwGlintInterval )
	{
		m_bShowInterval = true;
	}
	else if( (dwCurrentTime - m_dwLastGlint) <= (m_dwGlintInterval*2) )
	{
		m_bShowInterval = false;
	} 
	else
	{
		m_dwLastGlint = dwCurrentTime;
		m_bShowInterval = true;
	}
}

VOID GlintButton::Render()
{
	GUIButton::Render();

	if(m_bShowGlint)
		DrawGlint();
}

void GlintButton::DrawGlint()
{
	if( m_bShowInterval )
	{
		//�����������˸ͼƬ�������õ�ͼƬ
		if( P_VALID(m_pGlintPic) )
		{
			m_pRender->Draw(m_pCanvas, m_pGlintPic, m_dwGlintPicColor, m_eAlphaWrite);
		}
		//û������˸ͼƬ�ã���ť���µ�ͼƬ
		else
		{
			m_pRender->Draw(m_pCanvas, m_pPushPic, m_dwPushPicColor, m_eAlphaWrite);
		}

		tstring str = m_strText;
		//��������˸ʱ�����֣������õ�
		if( !m_strGlintText.empty() )
			str = m_strGlintText;

		tstring sub(_T("\\n"));
		while(tstring::npos != str.find(sub))
			str.replace(str.find(sub), sub.size(), _T("\n"));

		m_pRender->Text(m_pCanvas, str, m_pFont, m_dwGlintTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
	}
}

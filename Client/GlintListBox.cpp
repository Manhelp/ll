#include "StdAfx.h"
#include "GlintListBox.h"

const INT nRowHeight = 16;
GlintListBox::GlintListBox()
{
	m_bShowGlint = false;
	m_bShowInterval = false;
	m_strGlintText = _T("");
	m_dwGlintInterval = 400;	//����
	m_pGlintPic = NULL;
}

GlintListBox::~GlintListBox()
{

}

BOOL GlintListBox::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRet = GUIListBox::Init( pSystem, pFather, pXmlElement );

	m_dwGlintTextColor = m_dwSelectTextColor;
	m_strGlintText = m_strText;
	m_dwGlintPicColor = m_dwSelectStickActiveColor;
	return bRet;
}

VOID GlintListBox::Destroy()
{
	GUIListBox::Destroy();

	if( P_VALID(m_pGlintPic) )
	{
		m_pRender->DestroyImage( m_pGlintPic );
		m_pGlintPic = NULL;
	}
}

VOID GlintListBox::Update()
{
	GUIListBox::Update();
	SetRefresh( TRUE );

	if (m_listGlintRow.empty())
	{
		m_bShowGlint = FALSE;
	}
	else
	{
		m_bShowGlint = TRUE;
	}
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

VOID GlintListBox::Render()
{
	GUIListBox::Render();

	if(m_bShowGlint)
		DrawGlint();
}
VOID  GlintListBox::AddClintRow( INT i )
{ 
	if (m_listGlintRow.empty())
	{
		m_listGlintRow.push_back(i);
	}
	else
	{
		std::list<INT>::iterator iter = m_listGlintRow.begin();
		for ( ; iter!=m_listGlintRow.end(); iter++ )
		{
			if ( (*iter) == i )
			{
				m_bShowGlint = TRUE;
				return;
			}
		}
		m_listGlintRow.push_back(i);
	}
	m_bShowGlint = TRUE;
}

VOID  GlintListBox::StopClintRow( INT i )
{
	std::list<INT>::iterator iter = m_listGlintRow.begin();
	while( iter != m_listGlintRow.end() )
	{
		if ( (*iter) == i )
		{
			m_listGlintRow.erase(iter);
			break;
		}
		iter++;
	}
}

void GlintListBox::DrawGlint()
{
	if( m_bShowInterval )
	{
		// ����ѡ���
		for(INT nY=0, nRow = m_nCurrentTextRow; nY<m_ptSize.y; nY += m_nRowHeight, nRow++)
		{
			std::list<INT>::iterator iter = m_listGlintRow.begin();
			for ( ; iter != m_listGlintRow.end(); iter++ )
			{
				if ( (*iter) == nRow )
				{
					m_pCanvas->rc.Set(m_ptView.x, m_ptView.y + nY, m_ptView.x + m_ptSize.x, 
						m_ptView.y + nY + m_nRowHeight);
					if( m_pCanvas->rc.bottom > m_ptView.y + m_ptSize.y )
						m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;

					//�����������˸ͼƬ�������õ�ͼƬ
					if( P_VALID(m_pGlintPic) )
					{
						m_pRender->Draw(m_pCanvas, m_pGlintPic, m_dwSelectStickColor, m_eAlphaWrite);
					}
					//û������˸ͼƬ��ѡ��������ͼƬ
					else
					{
						m_pRender->Draw(m_pCanvas, m_pSelectStickPic, m_dwSelectStickActiveColor, m_eAlphaWrite );
					}

					tstring str = m_strText;
					//��������˸ʱ�����֣������õ�
					if( !m_strGlintText.empty() )
						tstring str = m_strGlintText;

					tstring sub(_T("\\n"));
					while(tstring::npos != str.find(sub))
						str.replace(str.find(sub), sub.size(), _T("\n"));

					m_pRender->Text(m_pCanvas, str, m_pFont, m_dwGlintTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

				}
			}
		}		
	}
}

//-----------------------------------------------------------------------------
// ɾ����������
//-----------------------------------------------------------------------------
VOID GlintListBox::ClearListBox()
{
	if ( !m_mapItem.Empty())
	{
		m_nCurrentTextRow = 0;
		m_nTotalTextRow = 0;
		m_nTotalRow = m_ptSize.y / nRowHeight;
		m_nCurSelectedSegment = GT_INVALID;	// ��ǰ���ѡ�е����ָ���

		if( m_nCurSelectedRow >= m_nTotalRow )
			m_nCurSelectedRow = 0;	// ��ǰѡ�е���
		m_mapItem.ResetIterator();
		tagGUIListItem* pItem = 0;
		while( m_mapItem.PeekNext(pItem) )
		{
			m_pRender->DestroyImage(pItem->pPic);
			delete(pItem);
		}
		m_mapItem.Clear();
		// ����������Ϣ
		tagGUIEvent event(this, EGUIE_Scroll, NULL);
		event.dwParam1 = (DWORD)m_nCurSelectedRow;
		event.dwParam2 = (DWORD)m_nTotalRow;
		event.dwParam3 = (DWORD)m_nTotalRow;
		OnEvent(&event);
		SetRefresh();
	}
}
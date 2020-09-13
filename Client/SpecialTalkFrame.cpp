#include "StdAfx.h"
#include "SpecialTalkFrame.h"
#include "QuestMgr.h"

SpecialTalkFrame::SpecialTalkFrame(void): m_Trunk(this), 
m_pWnd(0), 
m_pStcCondition(0),
m_pListOptions(0),
m_pBtnCloseWnd(0),
m_curRow(0),
m_pCaption(0)
{

}

SpecialTalkFrame::~SpecialTalkFrame(void)
{
}


//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID( m_pWnd ) )
		m_pGUI->AddToDestroyList( m_pWnd );
	
	m_pWnd = NULL;
	return TRUE;
}

//-----------------------------------------------------------------------------
// ���¶��봰�ڽ���
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\npctalk.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&SpecialTalkFrame::EventHandler) );

	m_pCaption		= (GUIPatch*)		m_pWnd->GetChild( _T("nt_pic") );
	m_pStcCondition	= (GUIStaticEx*)	m_pWnd->GetChild( _T("nt_pic\\nt_pic2\\npctalkpic\\contentWnd\\npctalk") );
	m_pListOptions	= (GUIListBox*)		m_pWnd->GetChild( _T("nt_pic\\nt_pic2\\optpic\\optlist") );
	m_pBtnCloseWnd	= (GUIButton*)		m_pWnd->GetChild( _T("nt_pic\\new0") );

	m_pListOptions->SetColNum( 1, 39 );
	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

//-----------------------------------------------------------------------------
// ������Ϣ�¼�����
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			//�رմ���
			if( pWnd == m_pBtnCloseWnd )
			{
				m_pMgr->AddToDestroyList(this);
			}
		}
		break;

	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListOptions )
			{
				if( m_curRow > pEvent->dwParam1 )
				{
					QuestScript *pQS = QuestMgr::Inst()->GetScript();
					if( !P_VALID(pQS) )
						return TRUE;

					const tagOptAttr& optAttr = m_optAttrs[pEvent->dwParam1];
					switch( optAttr.type )
					{
					case ESTOT_WalkWare:			//����ֿ⣬ѡ���¼�(������չ���ֿ���չ)
						{
							pQS->OnScenarioTalk4WalkWare( optAttr.id, optAttr.step );
						}
						break;
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// ����NPC��������
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::SetCaption( LPCTSTR szCaption )
{
	m_pCaption->SetText( szCaption );
}

//-----------------------------------------------------------------------------
// ���öԻ�����
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::SetContent( LPCTSTR szContent )
{
	tstring strContent = szContent;
	vector<tagIDLink> idLinks;
	vector<tagPosLink> posLinks;
	QuestMgr::Inst()->ParseQuestView( strContent,idLinks,posLinks );
	m_pStcCondition->SetText( strContent.c_str() );

	tagPoint size = m_pStcCondition->GetSize();
	if( m_pStcCondition->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcCondition->GetTextTotalHeight();
		m_pStcCondition->SetResizable(TRUE);
		m_pStcCondition->Resize( size );
		m_pStcCondition->SetResizable( FALSE );
	}
}

//-----------------------------------------------------------------------------
// ������жԻ�ѡ��
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::ClearOptions( void )
{
	if( P_VALID( m_pWnd ) )
	{
		m_pListOptions->Clear();
		m_pListOptions->SetCurSelectedRow( 0 );
		m_curRow = 0;
		m_optAttrs.clear();
	}
}

//-----------------------------------------------------------------------------
// ���ӶԻ�ѡ��
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::AddTalkOption( LPCTSTR szOption, DWORD param1, DWORD param2, ESpecialTalkOptionType optionType )
{
	m_pListOptions->SetText( m_curRow++, 0, szOption );

	tagOptAttr optAttr;
	optAttr.id = param1;
	optAttr.step = param2;
	optAttr.type = optionType;
	m_optAttrs.push_back( optAttr );
}

//-----------------------------------------------------------------------------
// ��ʾ�Ի�����
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::Show()
{
	if( P_VALID( m_pWnd ) )
	{
		m_pWnd->SetInvisible(FALSE);
		m_pWnd->FlipToTop();
	}
}

BOOL SpecialTalkFrame::EscCancel()
{
	if( P_VALID( m_pWnd ) && !m_pWnd->IsInvisible() )
	{
		m_pMgr->AddToDestroyList( this );
		return TRUE;
	}

	return GameFrame::EscCancel();
}


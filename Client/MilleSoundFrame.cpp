#include "StdAfx.h"
#include "MilleSoundFrame.h"
#include "PlayerNameTab.h"
#include "HttpPicEx.h"
#include "RichEditBoxForChatShow.h"
#include "ChatShowPop.h"

MilleSoundFrame::MilleSoundFrame(void):
m_Trunk(this),
m_pStcImg( NULL ),
m_pStcName( NULL ),
m_pStcExContent( NULL ),
m_bDisplaying( FALSE ),
m_dwMsgPersistTime( 4000 ),
m_dwWndCloseTime( 3000 ),
m_pEmotionFrame(NULL)
{
}

MilleSoundFrame::~MilleSoundFrame(void)
{
}

BOOL MilleSoundFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init( pMgr, pFather, dwParam );

	//-------------------------------------------
	tstring szFile = g_strLocalPath + _T("\\chat\\emotion.xml");

	//���������
	m_pEmotionFrame = (EmotionFrame*)m_pMgr->GetFrame( _T("Emotion") );
	if( !P_VALID(m_pEmotionFrame) )
	{
		m_pEmotionFrame = (EmotionFrame*)m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("Emotion"), _T("EmotionFrame"), 0);
	}
	if( P_VALID(m_pEmotionFrame) )
		m_pEmotionFrame->LoadEmotionFile(szFile);

	TCHAR szEmnName[32] = {0};
	for(int i=0; i<MAX_EMOTION; i++)
	{
		_tcscpy(szEmnName, m_pEmotionFrame->GetEmtionTran(i));
		ASSERT(_tcslen(szEmnName)>1);
		this->RegisterPicForRichEdit(szEmnName, i);
	}

	//-------------------------------------------

	m_pFrameMgr->RegisterEventHandle( _T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&MilleSoundFrame::OnRecvNetRoleGetName) );
	m_pFrameMgr->RegisterEventHandle( _T("tagGetHeadpicByID"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MilleSoundFrame::OnRecvHeadpicByID) );

	return bRet;
}


BOOL MilleSoundFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pFrameMgr->UnRegisterEventHandler( _T("tagRoleGetNameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MilleSoundFrame::OnRecvNetRoleGetName) );
	m_pFrameMgr->UnRegisterEventHandler( _T("tagGetHeadpicByID"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MilleSoundFrame::OnRecvHeadpicByID) );
	
	m_pEmotionFrame = NULL;

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd( m_pWnd );
	}

	return bRet;
}


BOOL MilleSoundFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\talk_notice.xml");
	
	m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() );
	m_pGUI->ChangeXml( &ele, "talknotice_win\\back\\imgback\\img", "ClassName", "HttpPicEx" );
	m_pGUI->ChangeXml(&ele,"talknotice_win\\back\\word","ClassName","RichEditBoxForChatShow"); //����Ϊ֧�ֱ�������
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &ele );

	// ע���¼�������
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1( &MilleSoundFrame::EventHandler ) );
	
	m_pStcImg = (HttpPicEx*)m_pWnd->GetChild( _T("back\\imgback\\img") );
	m_pStcName = (GUIStatic*)m_pWnd->GetChild( _T("back\\name") );
	m_pStcExContent = (RichEditBoxForChatShow*)m_pWnd->GetChild( _T("back\\word") );
	
	m_pWnd->SetInvisible( TRUE );
	return TRUE;
}

VOID MilleSoundFrame::Update()
{
	//������û��Ҫ��ʾ�Ĵ���
	if( m_lstCast.size() <= 0 )
	{	
		m_pWnd->SetInvisible( TRUE );
		m_pWnd->SetRefresh( TRUE );
		m_bDisplaying = FALSE;
		return;
	}
	
	//ϵͳ������ʾһ������
	if( m_bDisplaying == FALSE )
	{
		ShowCastInfo();
		m_dwStartTime = timeGetTime();
		m_bDisplaying = TRUE;
		return;
	}

	//��ǰ������ʱ
	DWORD dwCurTime = timeGetTime();

	if( dwCurTime - m_dwStartTime > m_dwMsgPersistTime )
	{
		//�����������������ȴ�����һ���ǵ�ǰ����
		if( m_lstCast.size() > 1 )
		{
			m_lstCast.pop_front();
			m_bDisplaying = FALSE;
			return;
		}
		
		//������û�����������ȴ��ˣ��Ͷ���ʾһ��
		if( (dwCurTime - m_dwStartTime) > (m_dwMsgPersistTime + m_dwWndCloseTime) )
		{	
			m_lstCast.pop_front();
			m_pWnd->SetInvisible( TRUE );
			m_pWnd->SetRefresh( TRUE );
			m_bDisplaying = FALSE;
		}
	}

	GameFrame::Update();
}

DWORD MilleSoundFrame::EventHandler(tagGUIEvent* pEvent)
{
	return 0;
}

VOID MilleSoundFrame::ShowCastInfo()
{
	tagScroText text = m_lstCast.front();
	if( !text.url.empty() )
	{
		m_pStcImg->SetPicURL( text.url );
	}

	if ( text.dwRoleID == GT_INVALID && text.eSex == ES_Null )//f�ƻ���������ʾ����
	{
		m_pStcImg->SetInvisible(TRUE);
		m_pStcName->SetInvisible(TRUE);
	}
	else  //����
	{
		if( text.eSex == ES_Woman )
			m_pStcImg->SetDefaultPic( _T("data\\ui\\state\\l_status-image-female.dds") );
		else if( text.eSex == ES_Man )
			m_pStcImg->SetDefaultPic( _T("data\\ui\\state\\l_status-image-male.dds") );
		else
			m_pStcImg->SetDefaultPic( _T("data\\ui\\state\\l_status-image-none.dds") );


		m_pStcName->SetText( text.szName.c_str() );
		//m_pStcExContent->SetText( text.szInfo.c_str() );
	}
	m_pStcExContent->ClsText();
	m_pStcExContent->AddText(text.szInfo.c_str(),FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);
	m_pWnd->SetInvisible( FALSE );
	m_pWnd->SetRefresh( TRUE );
}

VOID MilleSoundFrame::PushCastText( LPCTSTR szText, DWORD dwRoleID, ESex eSex, float fTime )
{

	if ( dwRoleID == GT_INVALID && eSex == ES_Null ) //f�ƻ���������ʾ����
	{
		tagScroText text;
		text.szInfo = szText ;
		text.szName = _T("");
		text.fTime = fTime;
		text.url = _T("");
		text.eSex = ES_Null;
		m_lstCast.push_back( text );
	}
	else  //����
	{
		tstring strName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
		tstring strUrl = PlayerNameTab::Inst()->FindHeadPicURLbyID(dwRoleID);
		if(!strName.empty() && !strUrl.empty())
		{
			tagScroText text;
			text.szInfo = szText ;
			text.szName = strName;
			text.fTime = fTime;
			text.url = strUrl;
			text.eSex = eSex;
			m_lstCast.push_back( text );
		}
		else
		{
			tagScroText text;
			text.szInfo = szText;
			text.fTime = fTime;
			text.dwRoleID = dwRoleID;
			text.szName = strName;
			text.url = strUrl;
			text.eSex = eSex;
			m_mapCastNoName.insert( make_pair(dwRoleID,text ) );
		}
	}
}

DWORD MilleSoundFrame::OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg )
{
	if( !P_VALID(pMsg) || !GT_VALID(pMsg->dwRoleID) || pMsg->bResult == FALSE )
		return 0;

	std::map<DWORD,tagScroText>::iterator itFind = m_mapCastNoName.find( pMsg->dwRoleID );
	if( itFind == m_mapCastNoName.end() )
		return 0;

	if( itFind->second.url.empty() )
	{
		itFind->second.szName =  pMsg->strRoleName;
	}
	else
	{
		tagScroText textNew;
		textNew.szName = pMsg->strRoleName;
		textNew.fTime = itFind->second.fTime;
		textNew.url = itFind->second.url;
		textNew.szInfo = itFind->second.szInfo;
		textNew.eSex = itFind->second.eSex;
		m_lstCast.push_back( textNew );
		m_mapCastNoName.erase( itFind );
	}

	return 0;
}

DWORD MilleSoundFrame::OnRecvHeadpicByID( tagGetHeadpicByID* pGameEvent )
{
	if( !P_VALID(pGameEvent) || !GT_VALID(pGameEvent->dwRoleID) )
		return 0;

	std::map<DWORD,tagScroText>::iterator itFind = m_mapCastNoName.find( pGameEvent->dwRoleID );
	if( itFind == m_mapCastNoName.end() )
		return 0;

	if( itFind->second.szName.empty() )
	{
		itFind->second.url =  pGameEvent->strImageURL;
	}
	else
	{
		tagScroText textNew;
		textNew.szName = itFind->second.szName;
		textNew.fTime = itFind->second.fTime;
		textNew.url = pGameEvent->strImageURL;
		textNew.szInfo = itFind->second.szInfo;
		textNew.eSex = itFind->second.eSex;
		m_lstCast.push_back( textNew );
		m_mapCastNoName.erase( itFind );
	}

	return 0;
}

VOID MilleSoundFrame::Move( tagPoint& pos )
{
	if( P_VALID( m_pWnd ) )
	{
		m_pWnd->SetMovable( TRUE );
		m_pWnd->Move( pos );
		m_pWnd->SetMovable( FALSE );
		m_pWnd->SetRefresh( TRUE );
	}
}

void MilleSoundFrame::RegisterPicForRichEdit(const TCHAR *szName,int index)
{
	if(index<0 || index>=MAX_EMOTION)return;

	TCHAR szPicPath[_MAX_PATH] = {0};

	_stprintf(szPicPath, _T("data\\ui\\chat\\emotion\\emo%02d.bmp"), index+1);

	//��ʼע��ת���ַ�
	TCHAR szTranName[32] = {0};
	_tcscpy(szTranName, szName);			//עRegisterPic��������ʹ��const char[]����

	m_pStcExContent->RegisterPic(szTranName,szPicPath);

}
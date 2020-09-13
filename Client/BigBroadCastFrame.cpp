#include "stdafx.h"
#include "BigBroadCastFrame.h"




BigBroadCastFrame::BigBroadCastFrame()
: m_pWnd(NULL)
{
	m_bBroadcasting[0] = false;
	m_bBroadcasting[1] = false;
	m_bBroadcasting[2] = false;
	m_dwTime[0]		   = 0;
	m_dwTime[1]        = 0;
	m_dwTime[2]        = 0;
}

BigBroadCastFrame::~BigBroadCastFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL BigBroadCastFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	
	m_bBroadcasting[0] = false;

	return TRUE;
}

BOOL BigBroadCastFrame::Destroy()
{	
	if (!GameFrame::Destroy())
		return FALSE;
	return TRUE;
}

BOOL BigBroadCastFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\big_broadcast.xml");
	m_pWnd = (GUIWnd*)m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	TCHAR tbuff[128];
	for (int i = 0; i != MAX_BROADCAST_NUM; ++i)
	{
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("broadword%d"), i);
		m_pStcBroadcast[i]  =   static_cast<GUIStaticEx*>(m_pWnd->GetChild(tbuff));  
	}
	m_pWnd->SetUnClickable(TRUE);

	m_pStcBroadcast[0]->SetText(_T(""));
	m_pStcBroadcast[1]->SetText(_T(""));
	m_pStcBroadcast[2]->SetText(_T(""));

	m_pStcBroadcast[0]->SetInvisible(FALSE);
	m_pStcBroadcast[1]->SetInvisible(FALSE);
	m_pStcBroadcast[2]->SetInvisible(TRUE);

	return TRUE;
}

VOID BigBroadCastFrame::Update()
{	
	m_dwTime[0] += Kernel::Inst()->GetDeltaTimeDW();
	m_dwTime[1] += Kernel::Inst()->GetDeltaTimeDW();

	// �жϵ�һ���Ƿ��ٲ��ţ��������û�в�����Ϣ
	if (!m_bBroadcasting[1])
	{
		if (!m_msgCache.empty())
		{
			MsgCache::iterator iter = m_msgCache.begin();
			m_pStcBroadcast[1]->SetText(iter->c_str());

            m_pStcBroadcast[0]->SetText(_T(""));
            m_pStcBroadcast[0]->SetTextColor( 0xCCFFF7E0 );

			m_pWnd->SetRefresh(TRUE);
			m_msgCache.erase(iter);
			m_dwTime[1] = 0;
            m_dwTime[0] = 0;
			m_bBroadcasting[1] = true;
		}
	}
	// ����в�����Ϣ���ڲ���
	else
	{
		if (m_dwTime[1] >= 3 * 1000)  // 3�����Ƿ����µ���Ϣ����
		{
			if (!m_msgCache.empty())
			{

				tstring tempText2 = m_pStcBroadcast[1]->GetText(); // ��1�еĸ���2��
				m_pStcBroadcast[0]->SetText( tempText2.c_str() );
				m_pStcBroadcast[0]->SetTextColor( 0xCCFFF7E0 );
				m_pStcBroadcast[0]->SetInvisible(FALSE);
				m_bBroadcasting[0] = true;
				m_dwTime[0]	= 0;

				MsgCache::iterator iter = m_msgCache.begin();
				m_pStcBroadcast[1]->SetText(iter->c_str());
				m_pStcBroadcast[1]->SetInvisible(FALSE);
				m_bBroadcasting[1] = true;
				m_dwTime[1] = 0;

				m_pWnd->SetRefresh(TRUE);
				m_msgCache.erase(iter);
				
			}
			
		}

		if (m_dwTime[1] >= 8 * 1000)  // ����ʱ�����8�����Զ���ʧ
		{
			m_dwTime[1] = 0;
			m_bBroadcasting[1] = false;
		}
	}

	// �жϵ�2���Ƿ��ٲ���,2������
	if (m_bBroadcasting[0])
	{
		if (m_dwTime[0] >= 2 * 1000)  
		{
			m_bBroadcasting[0] = false;
			m_pStcBroadcast[0]->SetText(_T(""));
            m_pStcBroadcast[0]->SetTextColor( 0xCCFFF7E0 );
		}
	}


	// �����һ��û����Ϣ�����û���κ���Ϣ������������������
	if (!m_bBroadcasting[1])
		m_pWnd->SetInvisible(TRUE);
	else
		m_pWnd->SetInvisible(FALSE);

	if (!m_bBroadcasting[0])
		m_pStcBroadcast[0]->SetInvisible(TRUE);
	else
		m_pStcBroadcast[0]->SetInvisible(FALSE);
}


VOID BigBroadCastFrame::PushInfo(LPCTSTR szContent)
{
	m_msgCache.push_back(szContent);
}
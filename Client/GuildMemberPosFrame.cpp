#include "StdAfx.h"
#include "GuildMemberPosFrame.h"
#include "GuildEvent.h"
#include "GuildMgr.h"
#include "..\WorldDefine\guild_define.h"
#include "GuildApplyManageFrame.h"

GuildMemberPosFrame::GuildMemberPosFrame(void):m_Trunk(this)
{
	m_pWnd		= NULL;
	m_pBtnClose	= NULL;

	m_dwDstRoleID = GT_INVALID;
}	

GuildMemberPosFrame::~GuildMemberPosFrame(void)
{
}

BOOL GuildMemberPosFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	
	ReflashPosButton((INT)dwParam);

	m_pWnd->SetTopMost(TRUE);
	return bRet;
}

BOOL GuildMemberPosFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
	}

	return bRet;
}

BOOL GuildMemberPosFrame::ReloadUI()
{
	BOOL bRet = GameFrame::ReloadUI();
	//����
	tstring strPath = g_strLocalPath + _T("\\ui\\party_jobappointed.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)m_Trunk.sfp1(&GuildMemberPosFrame::EventHandler));

	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\closebutt"));

	m_pBtnPosAppoint.clear();
	// �����б�
	m_pBtnPosAppoint.push_back((GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back\\butt4")));
	m_pBtnPosAppoint.push_back((GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back\\butt3")));
	m_pBtnPosAppoint.push_back((GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back\\butt0")));
	m_pBtnPosAppoint.push_back((GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back\\butt1")));
	m_pBtnPosAppoint.push_back((GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back\\butt2")));

	m_pBtnApply	= (GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back0\\butt0"));
	m_pBtnKickMember	= (GUIButton*)m_pWnd->GetChild(_T("jobapp_backpic\\basicmessage_back0\\butt1"));
	return bRet;
}

DWORD GuildMemberPosFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnClose)
			{
				m_pMgr->AddToDestroyList(this);
			}
			if(pWnd == m_pBtnApply)
			{
				OnClickManageGuild();
				m_pMgr->AddToDestroyList(this);
			}
			if(pWnd == m_pBtnKickMember)
			{
				TObjRef<GameFrameMgr>()->SendEvent( &tagGameEvent( _T("tagKickGuildMemberF"), NULL ) );
			}
			else
			{
				if(GuildMgr::Inst()->IsInSpecState( EGDSS_Warfare ))
				{
					GuildMgr::Inst()->ShowGuildErrorMsg(E_Guild_Pos_Limit);
					return 0;
				}
				for(INT i=0; i<=EGMP_End; ++i)
				{
					if(pWnd == m_pBtnPosAppoint[i])
					{
						// ������ƽ��᳤
						if(EGMP_HuiZhang == i)
						{
							tagGameEvent event(_T("GuildEvent_PosAppoint_HuiZhang"), this);
							m_pMgr->SendEvent(&event);
						}
						else
						{
							tagGuildPosAppointEvent event(_T("GuildEvent_PosAppoint"), this);
							event.dwRoleID	= m_dwDstRoleID; 
							event.ePos		= (EGuildMemberPos)i;
							m_pMgr->SendEvent(&event);
						}
						m_pMgr->AddToDestroyList(this);

						break;
					}
				}
			}
		}
		break;
	}
	return 0;
}

VOID GuildMemberPosFrame::ReflashPosButton( INT nIndex )
{
	BOOL bRet = TRUE;// �Ƿ��п�������ְλ
	// ��ȡ���������Ϣ
	const tagGuildMemberClient* pMember = GuildMgr::Inst()->GetMember(nIndex);
	if(!P_VALID(pMember))
		bRet = FALSE;
	// ��ȡ���������Ϣ
	const tagGuildMember* pLpInfo = GuildMgr::Inst()->GetLpInfo();
	if(!P_VALID(pLpInfo))
		bRet = FALSE;
	
	if(bRet)// ����ȡ�������
	{
		// ���õ�ǰĿ�����
		m_dwDstRoleID = pMember->sInfo.dwRoleID;
		// ��ȡ��ǰ�������Ȩ�ޱ�
		const tagGuildAppoint* pAppoint = GuildMgr::Inst()->GetAppointList((INT)pLpInfo->eGuildPos);
		if(P_VALID(pAppoint))
		{
			// ���ѡ�е��ǻ᳤
			if (pMember->sInfo.n8GuildPos == EGMP_HuiZhang)	//����ǻ᳤
			{
				for(INT i=0; i<X_GUILD_POS_NUM; ++i)
				{
					m_pBtnPosAppoint[i]->SetEnable(FALSE);
				}
			}
			// ���������ҿ�����Ŀ����ҵ�ǰְλ����ˢ��������ť
			else if(pAppoint->BitSetGuildAppoint[pMember->sInfo.n8GuildPos])
			{
				// ˢ�����а�ť�ɲ�����
				for(INT i=0; i<X_GUILD_POS_NUM; ++i)
				{
					m_pBtnPosAppoint[i]->SetEnable(pAppoint->BitSetGuildAppoint[i]);
				}
				// ��Ŀ����ҵ�ǰְλ����Ϊ���ɲ���
				m_pBtnPosAppoint[pMember->sInfo.n8GuildPos]->SetEnable(false);

				if (pLpInfo->eGuildPos == EGMP_HuiZhang)
					m_pBtnPosAppoint[EGMP_HuiZhang]->SetEnable(TRUE);
				
			}
			else
			{
				bRet = FALSE;
				// ���ɹ���˵������
			}
		}
		else// δ��ȡ����Ϊ���ɹ�
			bRet = FALSE;
	}

	if(!bRet)// û�гɹ���ɾ���ô���
	{
		m_pWnd->SetInvisible(true);
		m_pMgr->AddToDestroyList(this);
	}
}

BOOL GuildMemberPosFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;

	m_pMgr->AddToDestroyList(this);
	return TRUE;
}

void GuildMemberPosFrame::OnClickManageGuild()
{
	GuildApplyManageFrame* pFrame = (GuildApplyManageFrame*)m_pMgr->GetFrame( _T("GuildApplyManageFrame") );
	if( !P_VALID(pFrame) )
	{
		pFrame = (GuildApplyManageFrame*)m_pMgr->CreateFrame( _T("World"), _T("GuildApplyManageFrame"), _T("GuildApplyManageFrame"), 0 );
	}

	if( !P_VALID(pFrame) )
		return;

	pFrame->ShowWnd( TRUE );
}
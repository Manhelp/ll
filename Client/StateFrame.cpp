#include "stdafx.h"
#include "StateFrame.h"
#include "CharacterFrame.h"
#include "ButtonEx.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "RoleEvent.h"
#include "PlayerNameTab.h"
#include "LevelUpData.h"
#include "NetSession.h"
#include "..\WorldDefine\role_att.h"
#include "ChatFrame.h"
#include "CombatSysUtil.h"
#include "ToolTipFrame.h"
#include "Container.h"
#include "ItemMgr.h"
#include "ActiveProgress.h"
#include "../WorldDefine/RoleDefine.h"
#include "ServerTime.h"
#include "../WorldDefine/time.h"
static INT iToughnes = 0;
StateFrame::StateFrame(void):m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndFather = NULL;
	INT i;
	for (i = 0; i < EDP_F_End; i++)
	{
		m_pStcAtt[i] = NULL;
		m_pBtnAtt[i] = NULL;
	}
	for (i = 0; i < EDPE_End; i++)
	{
		m_pStcAttEx[i] = NULL;
	}
	for (i = 0; i < EDR_End; i++)
	{
// 		m_pStcResPic[i] = NULL;
// 		m_pStcResValue[i] = NULL;
	}
	m_pStcPlayerName = NULL;
	m_pStcPlayerLevel = NULL;
	m_pStcPlayerClass = NULL;
	m_pStcPlayerGuild = NULL;
	m_pStcOverplus = NULL;
	m_pPbnDisplayAtt = NULL;
	m_pPbnDisplayEquip = NULL;
	m_pPbnDisplayFashion = NULL;
    m_pWndState = NULL;

	m_isSwappingSuitPattern = false;
	m_bIsClick        = false;

	m_pWndAtt = NULL;
	m_pWndAttEx = NULL;
	m_pWndAttIn = NULL;
	m_pWndRes = NULL;
	m_pBtnSure = NULL;
	m_pBtnCancel = NULL;


	m_pStcAttWeary= NULL;     //ƣ�����Դ��ڿؼ�
	m_pStcAttWearyFont= NULL; //ƣ��ֵ������ʾ�ؼ�
	m_pProPlayerWeary= NULL;  //ƣ����ʾ�� 

	for (i=0; i<EDT_Origin_End; ++i)
	{
		m_Temporary[i] = 0;
	}
	for (i=0; i<ERA_End; ++i)
	{
		m_TempAttEquipAdd[i] = 0;
	}
	for (i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		m_nOgnAtt[i] = 0;
		m_nPreBid[i] = 0;
		m_nBidden[i] = 0;
	}
	m_nInternalInjury = GT_INVALID;
	m_bDontHaveToSentEvent = false;
}

StateFrame::~StateFrame(void)
{
}

BOOL StateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	// ע����Ϸ�¼�������
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPBidAttEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPLevel"),		  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
    m_pMgr->RegisterEventHandle(_T("tagInitLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("CleanPointConfirm_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnMsgClearPoint));
	TObjRef<NetCmdMgr>()->Register("NS_SynTotalStrength", (NETMSGPROC)m_Trunk.sfp2(&StateFrame::OnNetSynTotalStrength), _T("NS_SynTotalStrength"));
	TObjRef<NetCmdMgr>()->Register("NS_RoleClearAttPoint",	        (NETMSGPROC)m_Trunk.sfp2(&StateFrame::NetRoleClearAttPoint),		_T("NS_RoleClearAttPoint"));
	TObjRef<NetCmdMgr>()->Register("NS_SyncStartVipTime",	        (NETMSGPROC)m_Trunk.sfp2(&StateFrame::OnNS_SyncStartVipTime),		_T("NS_SyncStartVipTime"));

	m_pMgr->RegisterEventHandle(_T("tagChangeFashionEvent"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("tagChangeFashionEventEx"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("LocalPlayerTransform"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("LocalPlayerUnTransform"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->RegisterEventHandle(_T("Close_Role_UI"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	return bRet;
}

BOOL StateFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID( m_pWndAttExShow ) )
	{
		m_pGUI->DestroyWnd(m_pWndAttExShow);
		m_pWndAttExShow = NULL;
	}
	
	// ע����Ϸ�¼�������
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPAttEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPBidAttEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPLevel"),		 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
    m_pMgr->UnRegisterEventHandler(_T("tagInitLPAttEvent"),	     (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("CleanPointConfirm_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnMsgClearPoint));
	TObjRef<NetCmdMgr>()->UnRegister("NS_SynTotalStrength",  (NETMSGPROC)m_Trunk.sfp2(&StateFrame::OnNetSynTotalStrength));
	TObjRef<NetCmdMgr>()->UnRegister("NS_RoleClearAttPoint",		(NETMSGPROC)m_Trunk.sfp2(&StateFrame::NetRoleClearAttPoint));
	TObjRef<NetCmdMgr>()->UnRegister("NS_SyncStartVipTime",		(NETMSGPROC)m_Trunk.sfp2(&StateFrame::OnNS_SyncStartVipTime));
	m_pMgr->UnRegisterEventHandler(_T("tagChangeFashionEventEx"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagChangeFashionEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("LocalPlayerTransform"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("LocalPlayerUnTransform"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	m_pMgr->UnRegisterEventHandler(_T("Close_Role_UI"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&StateFrame::OnGameEvent));
	return bRet;
}

BOOL StateFrame::ReloadUI()
{
	// ����
    m_pWndState = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetStateWndFullName().c_str());
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("rstatepic"));

	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\rs_expand.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWndAttExShow = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWndAttExShow ) )
	{
		m_pGUI->DestroyWnd(m_pWndAttExShow);
		m_pWndAttExShow = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWndAttExShow->GetFullName().c_str(),   m_Trunk.sfp1(&StateFrame::EventHandler));
	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&StateFrame::EventHandler));

	TCHAR szTmp[X_SHORT_NAME] = {0};
	//��������
	{
		m_pStcPlayerName = (GUIStatic*)m_pWnd->GetChild(_T("rs_pic2\\rname"));
		m_pStcPlayerLevel = (GUIStatic*)m_pWnd->GetChild(_T("rs_pic2\\rgrade"));
		m_pStcPlayerClass = (GUIStatic*)m_pWnd->GetChild(_T("rs_pic2\\rcareer"));
		//m_pStcPlayerGuild = (GUIStatic*)m_pWnd->GetChild(_T("rpost"));
		m_pStcPlayerLevel->SetText(_T(""));
	}

	{
		m_pStcSilverVIP = (GUIStatic*)m_pWnd->GetChild(_T("equipback\\role\\silvervip"));
		m_pStcSilverVIPLeftTime = (GUIStatic*)m_pWnd->GetChild(_T("equipback\\role\\silvervip\\day"));
		m_pStcGoldVIP = (GUIStatic*)m_pWnd->GetChild(_T("equipback\\role\\goldvip"));
		m_pStcGoldVIPLeftTime = (GUIStatic*)m_pWnd->GetChild(_T("equipback\\role\\goldvip\\day"));
		m_pStcPlatinumVIP = (GUIStatic*)m_pWnd->GetChild(_T("equipback\\role\\supervip"));
		m_pStcPlatinumVIPLeftTime = (GUIStatic*)m_pWnd->GetChild(_T("equipback\\role\\supervip\\day"));
	}
	//һ�����Ժ�Ͷ��
	{
		m_pWndAtt = m_pWnd->GetChild(_T("rs_add"));
		
		for (INT i = 0; i < EDP_F_End; i++)
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("rnum%d"), i+1);
			m_pStcAtt[i] = (GUIStatic*)m_pWndAtt->GetChild(szTmp);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("ra_butt%d"), i+1);
			m_pBtnAtt[i] = (GUIButton*)m_pWndAtt->GetChild(szTmp);
			m_pStcAtt[i]->SetText(_T(""));
		}

		m_pStcOverplus = (GUIStatic*)m_pWndAtt->GetChild(_T("num0"));
		m_pStcOverplusText = (GUIStatic*)m_pWndAtt->GetChild(_T("word0"));
		m_pBtnCancel = (GUIButton*)m_pWndAtt->GetChild(_T("cancle"));
		m_pBtnSure = (GUIButton*)m_pWndAtt->GetChild(_T("sure"));
		m_pBtnCleanPoints = (GUIButton*)m_pWndAtt->GetChild(_T("refresh"));
		m_pBtnSure->SetEnable(FALSE);
		m_pBtnCancel->SetEnable(FALSE);
	}
	//�������ԺͿ���
	{
		m_pWndAttIn = m_pWnd->GetChild(_T("rs_pic2"));
		//m_pBtnDetail = (GUIButton*)m_pWndAttIn->GetChild(_T("infobutt"));
		for (INT i = 0; i < EDPE_End; i++)
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("num%d"), i);
			m_pStcAttEx[i] = (GUIStatic*)m_pWndAttIn->GetChild(szTmp);
			//m_pStcAttEx[i]->SetText(_T(""));
		}
		{
			// ����������ԵĿؼ�·�������޸�
			m_pStcAttEx[EDPE_JiaShen] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back0\\word1"));//�˺�����
			m_pStcAttEx[EDPE_JianMian] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back0\\word2"));//�˺�����
			m_pStcAttEx[EDPE_DiKang] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back0\\word3"));//�˺��ֿ�
			m_pStcAttEx[EDPE_QiangHua] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back0\\word4"));//����Ч��ǿ��

			m_pStcAttEx[EDPE_Ele_Injery] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back1\\word0"));//Ԫ���˺�
			m_pStcAttEx[EDPE_Ele_Pierce] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back1\\word1"));//Ԫ�ش�͸
			m_pStcAttEx[EDPE_Ele_Res] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back1\\word2"));//Ԫ�ؿ���
			m_pStcAttEx[EDPE_Ignore_Attack] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back1\\word3"));//���ӹ���
			m_pStcAttEx[EDPE_Ignore_Defend] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back1\\word4"));//���ӷ���

			m_pStcAttEx[EDPE_Mana_combustion] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back2\\word0"));//����ȼ��
			m_pStcAttEx[EDPE_Mana_combustion_resistance] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back2\\word1"));//����ȼ�յֿ�
			m_pStcAttEx[EDPE_ExDamage] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back2\\word2"));//�����˺�
			m_pStcAttEx[EDPE_ExJiaCheng] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back2\\word3"));//����ӳ�
			
			m_pStcAttEx[EDPE_Ren] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back4\\word0"));//����
			m_pStcAttEx[EDPE_Crit_Resist] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back4\\word1"));//��������

			m_pStcAttEx[EDPE_rebound] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back3\\word0"));//�̶�����
			m_pStcAttEx[EDPE_rebound_ratio] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back3\\word1"));//��������
			m_pStcAttEx[EDPE_rebound_immune] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back3\\word2"));//��������
			m_pStcAttEx[EDPE_slow_up] = (GUIStatic*)m_pWndAttExShow->GetChild(_T("expand_back\\back3\\num3"));

		}
		// ��չ����չʾ����
		m_pBtnAttExShow = (GUIButton*)m_pWndAttIn->GetChild(_T("detailbutt")); 
		m_pBtnAttExShowClose = (GUIButton*)m_pWndAttExShow->GetChild(_T("expand_back\\close")); 
		

		m_pStcAttWeary = (GUIStatic*)m_pWnd->GetChild(_T("rs_pic2\\progressback")); //ƣ�ͽ�����������
		m_pStcAttWearyFont = (GUIStatic*)m_pStcAttWeary->GetChild(_T("number"));  //ƣ��ֵ������ʾ
		m_pProPlayerWeary = (GUIProgress*)m_pStcAttWeary->GetChild(_T("progressbar"));
		m_pProPlayerWeary->SetMaxValue(1.0);
		//m_pStcAttEx[i] = m_pStcAttWearyFont;

		m_pStcAttHP = (GUIStatic*)m_pWnd->GetChild(_T("rs_pic2\\hpback")); 
		m_pStcAttHPFont = (GUIStatic*)m_pStcAttHP->GetChild(_T("number"));  
		m_pProPlayerHP = (ActiveProgress*)m_pStcAttHP->GetChild(_T("hpbar"));
		m_pProPlayerHP->SetMaxValue(1.0);

		m_pStcAttMP = (GUIStatic*)m_pWnd->GetChild(_T("rs_pic2\\mpback")); 
		m_pStcAttMPFont = (GUIStatic*)m_pStcAttMP->GetChild(_T("number"));  
		m_pProPlayerMP = (ActiveProgress*)m_pStcAttMP->GetChild(_T("mpbar"));
		m_pProPlayerMP->SetMaxValue(1.0);



		/*for (INT i = 0; i < EDPE_End; i++)
		{
			m_pStcAttEx[i]->SetText(_T("0"));
			m_pStcAttEx[i]->SetTextColor(ORDINARY_COLOR);
		}*/

	}
	//��������˺�ͼ�����ֵ & ����ֵ
	{
		/*m_pWndRes = m_pWnd->GetChild(_T("rs_pic3"));
		for (INT i = 0; i < EDR_End; i++)
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("spic%d"), i+1);
			m_pStcResPic[i] = (GUIStatic*)m_pWndRes->GetChild(szTmp);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("dnum%d"), i+1);
			m_pStcResValue[i] = (GUIStatic*)m_pWndRes->GetChild(szTmp);
			m_pStcResValue[i]->SetText(_T(""));
			m_pStcResPic[i]->SetInvisible(TRUE);
		}
		m_pStcToughnes = (GUIStatic*)m_pWndRes->GetChild(_T("spic9"));  
		m_pStcToughnesValue = (GUIStatic*)m_pWndRes->GetChild(_T("dnum9"));  */
	}
	//ѡ�����ý�����ʾ
	{
		//m_pPbnDisplayAtt = (GUIPushButton*)m_pWnd->GetChild(_T("word1\\optnutt1"));
		//m_pPbnDisplayAtt->SetState(EGUIBS_PushDown, FALSE);

		m_pPbnShowHead = (GUIPushButton*)m_pWnd->GetChild(_T("word\\optbutt1"));
		m_pPbnShowPiFeng = (GUIPushButton*)m_pWnd->GetChild(_T("word\\optbutt2"));

		// ʱװ��װ��ת��
		m_pFmSuitOpt = (GUIFrameMutex*)m_pWnd->GetChild(_T("word\\ra_opt"));
		//m_pPbnDisplayEquip = (GUIPushButton*)m_pWnd->GetChild(_T("word\\ra_opt\\optbutt1"));
		m_pPbnDisplayFashion = (GUIPushButton*)m_pWnd->GetChild(_T("word\\optbutt20"));
	}

	// ��ʱ����
	//m_pBtnDetail->SetEnable(false);
	m_pStcAttHPFont->SetInvisible(TRUE);
	m_pStcAttMPFont->SetInvisible(TRUE);
	
	m_pWndAttExShow->SetInvisible(TRUE);

	return TRUE;
}

VOID StateFrame::Update()
{
	/*! �����ۻ�ʱ�䣬����ҵ����װ��/ʱװģʽʱ�����ӳ� */
	static DWORD dwAccumTime = 0;
	if (m_isSwappingSuitPattern)
		{
		dwAccumTime += Kernel::Inst()->GetDeltaTimeDW();
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if (dwAccumTime >= BUTTON_DELAY_MS && P_VALID(pLp) && 0 == static_cast<AvatarEquipNode*>(pLp->GetSceneNode())->GetAtt().byTransSkinIndex)
		{
			dwAccumTime = 0;
			m_isSwappingSuitPattern = false;
			//m_pPbnDisplayEquip->SetEnable(true);
			//m_pPbnDisplayEquip->SetRefresh();
			m_pPbnDisplayFashion->SetEnable(true);
			m_pPbnDisplayFashion->SetRefresh();
		}
	}

    //ˢ������ٶ�
    static DWORD dwTimeFirst = 0;
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (P_VALID(m_pWndState)
        && !m_pWndState->IsInvisible()
        && P_VALID(pLp)
        && pLp->IsAttributeInited()
        && Kernel::Inst()->GetAccumTimeDW() - dwTimeFirst > 3000)
    {
        if (Kernel::Inst()->GetAccumTimeDW() - dwTimeFirst > 3000)
        {
            UpdateAttribute(ERA_Speed_XZ);
            UpdateAttribute(ERA_Speed_Mount);

            dwTimeFirst = Kernel::Inst()->GetAccumTimeDW();
        }
    }
}
DWORD StateFrame::OnGameEvent(tagGameEvent* pGameEvent)
{
	if (pGameEvent->strEventName == _T("tagUpdateLPAttEvent"))
	{
		tagUpdateLPAttEvent* pEvent = (tagUpdateLPAttEvent*)pGameEvent;
		UpdateAttribute(pEvent->eType);
	}
	else if (pGameEvent->strEventName == _T("tagUpdateLPLevel"))
	{
		
		tagUpdateLPLevel* pEvent = (tagUpdateLPLevel*)pGameEvent;
		SetPlayerLevel(pEvent->nCurLevel);
		//�Ƴ�ԤͶ��
		RemoveBidPoint();
		//ˢ���������
		UpdateBidAttribute();
	}
	else if (pGameEvent->strEventName == _T("tagRoleGetNameEvent"))
	{
		tagRoleGetNameEvent* pEvent = (tagRoleGetNameEvent*)pGameEvent;
		if(pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
		{
			SetPlayerName(PlayerNameTab::Inst()->FindNameByID(pEvent->dwRoleID));
		}
	}
	else if (pGameEvent->strEventName == _T("tagUpdateLPBidAttEvent"))
	{
		tagUpdateLPAttEvent* pEvent = (tagUpdateLPAttEvent*)pGameEvent;
		
	}
    else if (pGameEvent->strEventName == _T("tagInitLPAttEvent"))
    {
        // ��ɫ��ʼװ��ģʽ
        LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
        if( P_VALID( pLP ) && pLP->IsAttributeInited() )
        {
            bool bFashion = RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet().bFashionDisplay;
            if (bFashion)
                m_pPbnDisplayFashion->SetState(EGUIBS_PushDown, FALSE);
            else
                m_pPbnDisplayFashion->SetState(EGUIBS_PopUp,FALSE);

			bool bHead = RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet().bHideHead;
			if (!bHead)
				m_pPbnShowHead->SetState(EGUIBS_PushDown, FALSE);

			bool bPiFeng = RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet().bHideBack;
			if (!bPiFeng)
				m_pPbnShowPiFeng->SetState(EGUIBS_PushDown, FALSE);

        }
    }
    else if (pGameEvent->strEventName == _T("tagChangeFashionEvent"))
	{
		tagChangeFashionEvent* pEvent = (tagChangeFashionEvent*)pGameEvent;
		if (!m_bIsClick)
		{
			m_isSwappingSuitPattern = true;
			m_pPbnDisplayFashion->SetState(EGUIBS_PopUp,TRUE);    
		}
		else
		{
			if (pEvent->bDisFashion)
				m_pPbnDisplayFashion->SetState(EGUIBS_PushDown, TRUE);
			else
				m_pPbnDisplayFashion->SetState(EGUIBS_PopUp,TRUE);
             m_bIsClick = false ;
			 m_bDontHaveToSentEvent = true;
		}
		

	}
	else if(pGameEvent->strEventName == _T("tagChangeFashionEventEx"))
	{
		tagChangeFashionEvent* pEvent = (tagChangeFashionEvent*)pGameEvent;
	
		if(m_pPbnDisplayFashion->GetState() == EGUIBS_PushDown)
			return 0;
		if (pEvent->bDisFashion)
			m_pPbnDisplayFashion->SetState(EGUIBS_PushDown, TRUE);
		else
			m_pPbnDisplayFashion->SetState(EGUIBS_PopUp,TRUE);
		m_bIsClick = false ;
		m_bDontHaveToSentEvent = false;
	}
	else if (pGameEvent->strEventName == _T("LocalPlayerTransform"))
	{
		m_pPbnDisplayFashion->SetEnable(FALSE);
		//m_pPbnDisplayEquip->SetEnable(FALSE);
	}
	else if (pGameEvent->strEventName == _T("LocalPlayerUnTransform"))
	{
		m_pPbnDisplayFashion->SetEnable(TRUE);
		//m_pPbnDisplayEquip->SetEnable(TRUE);
	}
	else if(pGameEvent->strEventName == _T("Close_Role_UI"))
	{
		m_pWndAttExShow->SetInvisible(TRUE);
	}
	return 0;
}

DWORD StateFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnSure)//ȷ��
			{
				
				//����Ͷ������ 
				SendNetBidAtt();
				//�Ƴ�ԤͶ��
				RemoveBidPoint();
			}
			else if (pWnd == m_pBtnAttExShow)	
			{
				AlignAttEXShowWnd();

				if (m_pWndAttExShow->IsInvisible())
					m_pWndAttExShow->SetInvisible(false);
				else
					m_pWndAttExShow->SetInvisible(true);
			}
			else if (pWnd == m_pBtnAttExShowClose)
			{	
				m_pWndAttExShow->SetInvisible(true);
			}
			else if (pWnd == m_pBtnCancel)	//����
			{
				//�Ƴ�ԤͶ��
				RemoveBidPoint();
				//ˢ���������
				UpdateBidAttribute();
			}
			else if (pWnd == m_pBtnCleanPoints)//ϴ��
			{

				// ϴ��
				if(P_VALID(GetObj("CleanPointMsgBox")))
					KillObj("CleanPointMsgBox");

				CreateObj("CleanPointMsgBox", "MsgBox");
				TObjRef<MsgBox>("CleanPointMsgBox")->Init(_T(""), g_StrTable[_T("CleanATTConfirm")], _T("CleanPointConfirm_Event"));

				//ˢ���������
				//UpdateBidAttribute();
			}
			//else if (pWnd == m_pBtnDetail)	// ��ϸ
			//{
			//	GameFrame *pFrame = m_pFrameMgr->GetFrame(_T("StrengthShow"));
			//	if (!P_VALID(pFrame))
			//	{
			//		m_pFrameMgr->CreateFrame(_T("World"), _T("StrengthShow"), _T("StrengthShowFrame"), 0);
			//	}
			//	else
			//	{
			//		m_pFrameMgr->DestroyFrame(pFrame);
			//	}
			//}
			else
			{
				for (INT i = 0; i < EDP_F_End; i++)
				{
					if (pWnd == m_pBtnAtt[i])
					{
						//ԤͶ��
						if(m_nOverplus > 0)
						{
							AddPreBidPoint(TransBtn2Att((EDisplayProperty)i), 1);
							SetPlayerOverplus(m_nOverplus - 1);
						}
						break;
					}
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPbnDisplayAtt)
			{
				//Ͷ�����
				m_pWndAtt->SetInvisible(pEvent->dwParam1 == 2);

			}
 			else if(pWnd == m_pPbnDisplayEquip)
 			{
 				m_bIsClick     = true ;
 				m_isSwappingSuitPattern = true;
 				m_pPbnDisplayEquip->SetEnable(false);
 				m_pPbnDisplayEquip->SetRefresh();
 				m_pPbnDisplayFashion->SetEnable(false);
 				m_pPbnDisplayFashion->SetRefresh();
 				/*! ��������װ��ģʽ��Ϸ�¼� */
 				if (!m_bDontHaveToSentEvent)
 				{
 					tagFashionEvent e(_T("FashionEvent"), this);
 					e.bDisFashion = false;
 					m_pFrameMgr->SendEvent(&e);
 				}
 				m_bDontHaveToSentEvent = false;
 				
 			}
			else if(pWnd == m_pPbnDisplayFashion)
			{
				m_isSwappingSuitPattern = true;
				//m_pPbnDisplayEquip->SetEnable(false);
				//m_pPbnDisplayEquip->SetRefresh();
				m_pPbnDisplayFashion->SetEnable(false);
				m_pPbnDisplayFashion->SetRefresh();
				/*! ��������ʱװģʽ��Ϸ�¼� */
				tagFashionEvent e(_T("FashionEvent"), this);
				e.bDisFashion = m_pPbnDisplayFashion->GetState() == EGUIBS_PushDown ? true : false;
				m_pFrameMgr->SendEvent(&e);
			}
			else if(pWnd == m_pPbnShowHead)
			{
				if(m_pPbnShowHead->GetState() == EGUIBS_PushDown) 
					m_curDisplaySet.bHideHead = false;
				else
					m_curDisplaySet.bHideHead = true;

				if( m_pPbnShowPiFeng->GetState() == EGUIBS_PushDown)
					m_curDisplaySet.bHideBack = false;
				else
					m_curDisplaySet.bHideBack = true;
// 				tagUpdataAvatarEvent event(_T("tagUpdataAvatarEvent"), this);
// 				event.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
// 				m_pFrameMgr->SendEvent(&event);
				tagNC_RoleSetDisplay cmd;
				cmd.sDisplaySet = m_curDisplaySet;
				m_pSession->Send( &cmd );
					
			}
			else if(pWnd == m_pPbnShowPiFeng)
			{
				if( m_pPbnShowPiFeng->GetState() == EGUIBS_PushDown)
					m_curDisplaySet.bHideBack = false;
				else
					m_curDisplaySet.bHideBack = true;

				if(m_pPbnShowHead->GetState() == EGUIBS_PushDown) 
					m_curDisplaySet.bHideHead = false;
				else
					m_curDisplaySet.bHideHead = true;

// 				tagUpdataAvatarEvent event(_T("tagUpdataAvatarEvent"), this);
// 				event.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
// 				m_pFrameMgr->SendEvent(&event);
				tagNC_RoleSetDisplay cmd;
				cmd.sDisplaySet = m_curDisplaySet;
				m_pSession->Send( &cmd );
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if ( pWnd == (GUIWnd*)m_pStcToughnes )
			{
				float fTempToughnesValue = (float)iToughnes/100;
				INT   iTempToughnesValue = (INT)fTempToughnesValue;
				TCHAR szTmp[X_SHORT_NAME] = {0};
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR),g_StrTable[_T("Toughnes_Tips")], iTempToughnesValue);
				//ShowTip( m_pStcToughnes, szTmp );
			} 
			else if(pWnd == m_pStcAttEx[EDPE_JiaShen])//�˺�����
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_transform_Bang)  + plp->GetAttribute(ERA_transform_ExAttack) + plp->GetAttribute(ERA_transform_ALL);//����˺�����
				INT nValue2 = plp->GetAttribute(ERA_transform_Brunt)+ plp->GetAttribute(ERA_transform_ExAttack) + plp->GetAttribute(ERA_transform_ALL) ;//�����˺�����
				INT nValue3 = plp->GetAttribute(ERA_transform_Injury) + plp->GetAttribute(ERA_transform_InAttack) + plp->GetAttribute(ERA_transform_ALL);//Ǭ���˺�����
				INT nValue4 = plp->GetAttribute(ERA_transform_Thinker)+ plp->GetAttribute(ERA_transform_InAttack) + plp->GetAttribute(ERA_transform_ALL) ;//�����˺�����
				
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d\\n%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_84")], nValue1,g_StrTable[_T("ERoleAttribute_85")], nValue2,g_StrTable[_T("ERoleAttribute_86")], nValue3,g_StrTable[_T("ERoleAttribute_87")], nValue4);
				ShowTip( pWnd, szTmp );
			}
			else if(pWnd == m_pStcAttEx[EDPE_JianMian])//�˺�����
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_Derate_Bang)+ plp->GetAttribute(ERA_Derate_ExAttack) + plp->GetAttribute(ERA_Derate_ALL);//����˺�����
				INT nValue2 = plp->GetAttribute(ERA_Derate_Brunt)+ plp->GetAttribute(ERA_Derate_ExAttack) + plp->GetAttribute(ERA_Derate_ALL) ;//�����˺�����
				INT nValue3 = plp->GetAttribute(ERA_Derate_Injury) + plp->GetAttribute(ERA_Derate_InAttack) + plp->GetAttribute(ERA_Derate_ALL);//Ǭ���˺�����
				INT nValue4 = plp->GetAttribute(ERA_Derate_Thinker)+ plp->GetAttribute(ERA_Derate_InAttack) + plp->GetAttribute(ERA_Derate_ALL) ;//�����˺�����

				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d\\n%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_39")], nValue1,g_StrTable[_T("ERoleAttribute_38")], nValue2,g_StrTable[_T("ERoleAttribute_42")], nValue3,g_StrTable[_T("ERoleAttribute_41")], nValue4);
				ShowTip( pWnd, szTmp );
			}
			else if(pWnd == m_pStcAttEx[EDPE_DiKang])//�˺��ֿ�
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_Resist_Bleeding)>200?200:plp->GetAttribute(ERA_Resist_Bleeding);//�̶��˺�����
				INT nValue2 = plp->GetAttribute(ERA_Resist_Weak)>200?200:plp->GetAttribute(ERA_Resist_Weak);//˥��Ч������
				INT nValue3 = plp->GetAttribute(ERA_Resist_Choas)>200?200:plp->GetAttribute(ERA_Resist_Choas);//����Ч������
				INT nValue4 = plp->GetAttribute(ERA_Resist_Special)>200?200:plp->GetAttribute(ERA_Resist_Special);//����Ч������

				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d\\n%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_49")], nValue1,g_StrTable[_T("ERoleAttribute_50")], nValue2,g_StrTable[_T("ERoleAttribute_51")], nValue3,g_StrTable[_T("ERoleAttribute_52")], nValue4);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_QiangHua])//����Ч��ǿ��
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_Fixed_damage_aggrandizement);//�̶��˺�ǿ��
				INT nValue2 = plp->GetAttribute(ERA_Aging_effect_strengthening);//˥��Ч��ǿ��
				INT nValue3 = plp->GetAttribute(ERA_Contrel_effect_deepen);//����Ч��ǿ��
				INT nValue4 = plp->GetAttribute(ERA_Slowing_effect_strengthening);//����Ч��ǿ��

				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d\\n%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_115")], nValue1,g_StrTable[_T("ERoleAttribute_114")], nValue2,g_StrTable[_T("ERoleAttribute_113")], nValue3,g_StrTable[_T("ERoleAttribute_116")], nValue4);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Ele_Injery])//Ԫ���˺�
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_EEI_Fire);//��Ԫ���˺�
				INT nValue2 = plp->GetAttribute(ERA_EEI_Water);//ˮԪ���˺�
				INT nValue3 = plp->GetAttribute(ERA_EEI_Earth);//��Ԫ���˺�
				INT nValue4 = plp->GetAttribute(ERA_EEI_Wind);//��Ԫ���˺�
				INT nValue5 = plp->GetAttribute(ERA_EEI_Bright);//����Ԫ���˺�
				INT nValue6 = plp->GetAttribute(ERA_EEI_Dark);//�ڰ�Ԫ���˺�

				Equipment* pFabao = ItemMgr::Inst()->GetCurEquip(EEP_Face);
				if(P_VALID(pFabao))
				{
					const tagFabaoSpec* pSpec = pFabao->GetFaBaoSpe();
					if(P_VALID(pSpec))
					{
						switch(pSpec->eWuxing)
						{
						case EWX_Metal:
							nValue1 += plp->GetAttribute(ERA_EEI_ALL);
							break;
						case EWX_Wood:
							nValue2 += plp->GetAttribute(ERA_EEI_ALL);
							break;
						case EWX_Water:
							nValue3 += plp->GetAttribute(ERA_EEI_ALL);
							break;
						case EWX_Fire:
							nValue4 += plp->GetAttribute(ERA_EEI_ALL);
							break;
						case EWX_Soil:
							nValue5 += plp->GetAttribute(ERA_EEI_ALL);
							break;
						default:
							nValue1 += plp->GetAttribute(ERA_EEI_ALL);
							break;
						}
					}
				}
				

				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d\\n%s:%d\\n%s:%d\\n%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_92")], nValue1,g_StrTable[_T("ERoleAttribute_93")], nValue2,g_StrTable[_T("ERoleAttribute_94")], nValue3,g_StrTable[_T("ERoleAttribute_95")], nValue4,g_StrTable[_T("ERoleAttribute_96")], nValue5,g_StrTable[_T("ERoleAttribute_97")], nValue6);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Ele_Pierce])//Ԫ�ش�͸
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_EEP_VALUE);//Ԫ�ش�͸
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_131")], nValue1);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Ele_Res])//Ԫ�ؿ���
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();//add by xp
				INT nValue1 = plp->GetAttribute(ERA_EER_Fire)+plp->GetAttribute(ERA_EER_ALL);//��Ԫ�ؿ���
				INT nValue2 = plp->GetAttribute(ERA_EER_Water)+plp->GetAttribute(ERA_EER_ALL);//ˮԪ�ؿ���
				INT nValue3 = plp->GetAttribute(ERA_EER_Earth)+plp->GetAttribute(ERA_EER_ALL);//��Ԫ�ؿ���
				INT nValue4 = plp->GetAttribute(ERA_EER_Wind)+plp->GetAttribute(ERA_EER_ALL);//��Ԫ�ؿ���
				INT nValue5 = plp->GetAttribute(ERA_EER_Bright)+plp->GetAttribute(ERA_EER_ALL);//����Ԫ�ؿ���
				INT nValue6 = plp->GetAttribute(ERA_EER_Dark)+plp->GetAttribute(ERA_EER_ALL);//�ڰ�Ԫ�ؿ���
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d\\n%s:%d\\n%s:%d\\n%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_98")], nValue1,g_StrTable[_T("ERoleAttribute_99")], nValue2,g_StrTable[_T("ERoleAttribute_100")], nValue3,g_StrTable[_T("ERoleAttribute_101")], nValue4,g_StrTable[_T("ERoleAttribute_102")], nValue5,g_StrTable[_T("ERoleAttribute_103")], nValue6);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Ignore_Attack])//���ӹ���
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Nosee_attack);//���ӹ���
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_125")], nValue1);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Ignore_Defend])//���ӷ���
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Nosee_defend);//���ӷ���
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_126")], nValue1);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Mana_combustion])//����ȼ��
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Mana_combustion);//����ȼ��
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_123")], nValue1);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Mana_combustion_resistance])//����ȼ�յֿ�
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Mana_combustion_resistance);//����ȼ�յֿ�
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_109")], nValue1);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_ExDamage])//�����˺�
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_ExDamage);//�����˺�
				INT nValue2 = plp->GetAttribute(ERA_ExDamage_Absorb);//�����˺�����
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_47")], nValue1,g_StrTable[_T("ERoleAttribute_48")], nValue2);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_ExJiaCheng])//����ӳ�
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Exp_Add_Rate);//�����üӳ�
				INT nValue2 = plp->GetAttribute(ERA_Money_Add_Rate);//��Ǯ��üӳ�
				INT nValue3 = plp->GetAttribute(ERA_Loot_Add_Rate);//�������ʼӳ�
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d%%\\n%s:%d%%\\n%s:%d%%"), g_StrTable[_T("ERoleAttribute_67")], nValue1/100,g_StrTable[_T("ERoleAttribute_68")], nValue2/100,g_StrTable[_T("ERoleAttribute_69")], nValue3/100);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_rebound])//�̶�����
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Physical_damage_rebound_num);//������
				INT nValue2 = plp->GetAttribute(ERA_Magic_damage_rebound_num);//��������
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d\\n%s:%d"), g_StrTable[_T("ERoleAttribute_117")], nValue1,g_StrTable[_T("ERoleAttribute_119")], nValue2);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_rebound_ratio])//��������
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Physical_damage_rebound_ratio);//����������
				INT nValue2 = plp->GetAttribute(ERA_Magic_damage_rebound_ratio);//������������
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%.1f%%\\n%s:%.1f%%"), g_StrTable[_T("ERoleAttribute_118")], nValue1/100.f,g_StrTable[_T("ERoleAttribute_120")], nValue2/100.f);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_rebound_immune])//��������
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Physical_damage_rebound_immune);//����������
				INT nValue2 = plp->GetAttribute(ERA_Magic_damage_rebound_immune);//������������
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%.1f%%\\n%s:%.1f%%"), g_StrTable[_T("ERoleAttribute_121")], nValue1/100.f,g_StrTable[_T("ERoleAttribute_122")], nValue2/100.f);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Ren])//����
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Toughness_god);//����
				
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_110")], nValue1);
				ShowTip( pWnd, szTmp);
			}
			else if(pWnd == m_pStcAttEx[EDPE_Crit_Resist])//��������
			{
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				INT nValue1 = plp->GetAttribute(ERA_Toughness_strength);//��������

				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf(szTmp, _T("%s:%d"), g_StrTable[_T("ERoleAttribute_111")], nValue1);
				ShowTip( pWnd, szTmp);
			}
		}
       break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	}
	return 0;
}

VOID StateFrame::UpdatePalyerAtt()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
/*		UpdateSettingsOfLocalPlayer(pLp);*/
		SetPlayerName(PlayerNameTab::Inst()->FindNameByID(pLp->GetID()));
		SetPlayerLevel(pLp->GetRoleLevel());
		//�Ƴ�ԤͶ��
		RemoveBidPoint();
		//ˢ���������
		UpdateBidAttribute();
		if(pLp->GetRoleLevel() < 20)
			m_pBtnCleanPoints->SetEnable(false);
		else
			m_pBtnCleanPoints->SetEnable(true);
	}

	// ������Ϣ����ʵ��
	tagNC_SynTotalStrength cmd;
	m_pSession->Send(&cmd);
}

VOID StateFrame::SetPlayerName(LPCTSTR szName)
{
	m_pStcPlayerName->SetText(szName);
}

VOID StateFrame::SetPlayerLevel(INT nLevel)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], nLevel);
	m_pStcPlayerLevel->SetText(szTmp);
	m_pStcPlayerLevel->SetRefresh(TRUE);

	// �ȼ��ı䣬��Ӧ����ɳ�ֵҲҪ�����仯
	const tagExperience* pExp = LevelUpData::Inst()->FindExpData(nLevel);
	if(P_VALID(pExp))
	{
		for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
		{
			m_nOgnAtt[i] = pExp->nAtt[i];
		}
	}
}

VOID StateFrame::SetPlayerClass(LPCTSTR szClass)
{
	m_pStcPlayerLevel->SetText(szClass);
}

VOID StateFrame::SetPlayerGuild(LPCTSTR szGuild)
{
	//m_pStcPlayerGuild->SetText(szGuild);
}

VOID StateFrame::SetPlayerOverplus(INT nValue)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
	m_pStcOverplus->SetText(szTmp);
	m_pStcOverplus->SetRefresh(TRUE);

	if( nValue == 0)
	{
		m_pStcOverplusText->SetInvisible(TRUE);
		m_pStcOverplus->SetInvisible(TRUE);
	}else{
		m_pStcOverplusText->SetInvisible(FALSE);
		m_pStcOverplus->SetInvisible(FALSE);
	}
	
	if(0 != m_nOverplus && 0 == nValue)
	{
		for(INT i=0; i<EDP_F_End; ++i)
		{
			m_pBtnAtt[i]->SetEnable(FALSE);
		}
	}
	else if(0 == m_nOverplus && 0 != nValue)
	{
		for(INT i=0; i<EDP_F_End; ++i)
		{
			m_pBtnAtt[i]->SetEnable(TRUE);
		}
	}
	m_nOverplus = nValue;
}

VOID StateFrame::SetPlayerAtt(EDisplayProperty eType, INT nValue, DWORD dwColor /* = ORDINARY_COLOR */)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
	m_pStcAtt[eType]->SetText(szTmp);
	m_pStcAtt[eType]->SetTextColor(dwColor);
	m_pStcAtt[eType]->SetRefresh(TRUE);
}

VOID StateFrame::SetPlayerAttEx(EDisplayPropertyEx eType, INT nValue, DWORD dwColor /* = ORDINARY_COLOR */)
{
	LocalPlayer* localPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(localPlayer))
		return;

	TCHAR szTmp[X_SHORT_NAME] = {0};
	switch(eType)
	{
	case EDPE_ArmDefine0:		// ���׷�������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_ArmDefine]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_ArmDefine]->SetText(szTmp);
			m_pStcAttEx[EDPE_ArmDefine]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_ArmDefine]->SetRefresh(TRUE);
		}
		break;
	case EDPE_Luck0:			// ��Ե����ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_Luck]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_Luck]->SetText(szTmp);
			m_pStcAttEx[EDPE_Luck]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Luck]->SetRefresh(TRUE);
		}
		break;
	//case EDPE_Feature0:			// ��������ֵ
	//case EDPE_Command0:			// ͳ������ֵ
	case EDPE_Morality0:		// ��������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_Morality]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_Morality]->SetText(szTmp);
			m_pStcAttEx[EDPE_Morality]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Morality]->SetRefresh(TRUE);
		}
		break;
	//case EDPE_Cultivate0:		// ��Ϊ����ֵ
	case EDPE_MeleeDamage0:		// �⹦��������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_MeleeDamage]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_MeleeDamage]->SetText(szTmp);
			m_pStcAttEx[EDPE_MeleeDamage]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MeleeDamage]->SetRefresh(TRUE);
		}
		break;
	case EDPE_MeleeDefine0:		// �⹦��������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_MeleeDefine]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_MeleeDefine]->SetText(szTmp);
			m_pStcAttEx[EDPE_MeleeDefine]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MeleeDefine]->SetRefresh(TRUE);
		}
		break;
	case EDPE_MagicDamage0:		// �ڹ���������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_MagicDamage]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_MagicDamage]->SetText(szTmp);
			m_pStcAttEx[EDPE_MagicDamage]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MagicDamage]->SetRefresh(TRUE);
		}
		break;
	case EDPE_MagicDefine0:		// �ڹ���������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_MagicDefine]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_MagicDefine]->SetText(szTmp);
			m_pStcAttEx[EDPE_MagicDefine]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MagicDefine]->SetRefresh(TRUE);
		}
		break;
	case EDPE_DamageSkill0:		// ������������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_DamageSkill]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_DamageSkill]->SetText(szTmp);
			m_pStcAttEx[EDPE_DamageSkill]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_DamageSkill]->SetRefresh(TRUE);
		}
		break;
	case EDPE_DefineSkill0:		// ������������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_DefineSkill]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue+ temp);
			m_pStcAttEx[EDPE_DefineSkill]->SetText(szTmp);
			m_pStcAttEx[EDPE_DefineSkill]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_DefineSkill]->SetRefresh(TRUE);
		}
		break;
	//case EDPE_Hit0:				// ��������ֵ
	//case EDPE_Dodge0:			// ��������ֵ
		//{
		//	if(MINUS_RED_COLOR == dwColor)
		//		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
		//	else if(PLUS_GREEN_COLOR == dwColor)
		//		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("+%d"), nValue);
		//		
		//}
		break;
	case EDPE_Energy0:			// �������ֵ
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_Energy]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), temp, nValue);
			m_pStcAttEx[EDPE_Energy]->SetText(szTmp);
			m_pStcAttEx[EDPE_Energy]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Energy]->SetRefresh(TRUE);
		}
		break;
	case EDPE_Health0:			// �������ֵ
		{
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				float fProValue =0;
				int Hp = plp->GetAttribute(ERA_HP);
				TCHAR szTmp2[X_SHORT_NAME] = {0};
				_sntprintf(szTmp2, sizeof(szTmp2)/sizeof(TCHAR), _T("%d/%d"), Hp,nValue);
// 				m_pStcAttHPFont->SetText();
// 				m_pStcAttHPFont->SetRefresh(TRUE);

				fProValue = (float)Hp/(float)nValue;
				m_pProPlayerHP->SetValue(Hp);
				m_pProPlayerHP->SetMaxValue(nValue);
				m_pProPlayerHP->SetSpeed(10000000.0f);
				m_pProPlayerHP->SetPicColor(0x00000000); 
				m_pProPlayerHP->SetForePicColor(0xFFFF402D);
				m_pProPlayerHP->SetText(szTmp2);

			}
		}
		break;
	case EDPE_Power0:			// �������ֵ
		{
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				float fProValue =0;
				int Hp = plp->GetAttribute(ERA_MP);
				TCHAR szTmp2[X_SHORT_NAME] = {0};
				_sntprintf(szTmp2, sizeof(szTmp2)/sizeof(TCHAR), _T("%d/%d"), Hp,nValue);
// 				m_pStcAttMPFont->SetText(szTmp2);
// 				m_pStcAttMPFont->SetRefresh(TRUE);

				fProValue = (float)Hp/(float)nValue;
				m_pProPlayerMP->SetValue(Hp);
				m_pProPlayerMP->SetMaxValue(nValue);
				m_pProPlayerMP->SetSpeed(10000000.0f);
				m_pProPlayerMP->SetPicColor(0x00000000); 
				m_pProPlayerMP->SetForePicColor(0xFF00A2FF);
				m_pProPlayerMP->SetText(szTmp2);

			}
		}
		break;
	case EDPE_Toughness:	//�־���
		{
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				int Toughness = plp->GetAttribute(ERA_Endurance);
				int ToughnessMax = plp->GetAttribute(ERA_MaxEndurance);
				//int temp = _tcstol(m_pStcAttEx[EDPE_Toughness]->GetText().c_str(),    NULL, 10);
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"),  nValue,ToughnessMax);
				m_pStcAttEx[EDPE_Toughness]->SetText(szTmp);
				m_pStcAttEx[EDPE_Toughness]->SetTextColor(dwColor);
				m_pStcAttEx[EDPE_Toughness]->SetRefresh(TRUE);
			}
		}
		break;
	case EDPE_Toughness0:		// �־������ֵ
		{
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				int Toughness = plp->GetAttribute(ERA_Endurance);
				int ToughnessMax = plp->GetAttribute(ERA_MaxEndurance);
				//int temp = _tcstol(m_pStcAttEx[EDPE_Toughness]->GetText().c_str(),    NULL, 10);
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"),  Toughness,nValue);
				m_pStcAttEx[EDPE_Toughness]->SetText(szTmp);
				m_pStcAttEx[EDPE_Toughness]->SetTextColor(dwColor);
				m_pStcAttEx[EDPE_Toughness]->SetRefresh(TRUE);
			}
			
		}
		break;
	case EDPE_WeaponDamage0:
		{
			int temp = _tcstol(m_pStcAttEx[EDPE_WeaponDamage]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d-%d"), temp,nValue);
			m_pStcAttEx[EDPE_WeaponDamage]->SetText(szTmp);
			m_pStcAttEx[EDPE_WeaponDamage]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_WeaponDamage]->SetRefresh(TRUE);
		}
		break;
	case EDPE_MoveSpeed:
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d%%"), nValue/100);
		}
		break;
	//case EDPE_Weary:               // ƣ��ֵ
	//	{
	//		
	//		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/100"), nValue);
	//		 m_pStcAttWearyFont->FlipToTop();
	//	}
	//	break;		
	default:
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
		}
		break;
	}

	if(eType > EDPE_End)
		return;
	m_pStcAttEx[eType]->SetText(szTmp);
	m_pStcAttEx[eType]->SetTextColor(dwColor);
	m_pStcAttEx[eType]->SetRefresh(TRUE);
}

VOID StateFrame::SetPlayerResist(EDisplayResist eType, INT nValue)
{
	/*TCHAR szTmp[X_SHORT_NAME] = {0};
	if(nValue >= 0)
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("+%d"), abs(nValue));
	else
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("-%d"), abs(nValue));

	m_pStcResValue[eType]->SetText(szTmp);
	m_pStcResPic[eType]->SetInvisible(FALSE);
	m_pStcResPic[eType]->SetRefresh(TRUE);*/
}

VOID StateFrame::UpdateAttribute(ERoleAttribute eType)
{
	INT nValue1,nValue2,nValue3;
	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	nValue1 = pLP->GetAttribute(eType);
	tagDWORDTime time2;
	tagDWORDTime time3;
	DWORD timeDiff = 0;;
	INT LastDays=0;
	DWORD dwColor = ORDINARY_COLOR;
	TCHAR szTmp[X_SHORT_NAME] = {0};

	switch(eType)
	{
	case ERA_Physique:			// ��ǰ���
		{
			// ��ǰ��ǵ���ʾ��Ҫ���ϵ�ǰ��ԤͶ��ֵ�����ڽ�����ʾЧ��
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Physique, nValue1);

			// ԭʼ����=��ǰ��ǡ�10
			m_Temporary[EDT_Origin_Health] = (INT)(nValue1 * 6);
			if(m_nPreBid[eType] > 0)
			{
				nValue3 = pLP->GetAttribute(ERA_MaxHP) + (INT)(m_nPreBid[eType] * 6);
				SetPlayerAttEx(EDPE_Health0, nValue3 /*+ m_TempAttEquipAdd[ERA_MaxHP]*/);
			}
			
			// ��ǰ��������ʾ��Ҫ���ϵ�ǰ��ԤͶ��ֵ�����ڽ�����ʾЧ�� 
			nValue2 = pLP->GetAttribute(ERA_Pneuma) + m_nPreBid[ERA_Pneuma];
			// ԭʼ�⹥����=��ǰ��ǡ�5+��ǰ������1
			m_Temporary[EDT_Origin_ExDefine] = (INT)(nValue1 * 6 + nValue2 * 4); 
			SetPlayerAttEx(EDPE_MeleeDefine, m_Temporary[EDT_Origin_ExDefine] + m_TempAttEquipAdd[ERA_ExDefense]);

			m_Temporary[EDT_Origin_InDefine] = (INT)(nValue1 * 6 + nValue2 * 4);
			SetPlayerAttEx(EDPE_MagicDefine, m_Temporary[EDT_Origin_InDefine] + m_TempAttEquipAdd[ERA_InDefense]);
		}
		break;
	case ERA_Strength:			// ��ǰ����
		{
			INT nRealValue = 0;
			INT nDiffValue = 0;
			// ��ǰ��������ʾ��Ҫ���ϵ�ǰ��ԤͶ��ֵ�����ڽ�����ʾЧ��
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Strength, nValue1);

			// ԭʼ�⹥����=��ǰ������5
			m_Temporary[EDT_Origin_ExDamge] = (INT)(nValue1 * 10); 
			SetPlayerAttEx(EDPE_MeleeDamage, m_Temporary[EDT_Origin_ExDamge] + m_TempAttEquipAdd[ERA_ExAttack]);// �����������������ӵ��⹦�������Լӳ�
			
			
			nValue2 = pLP->GetAttribute(ERA_Physique) + m_nPreBid[ERA_Physique];
			nValue3 = pLP->GetAttribute(ERA_Pneuma) + m_nPreBid[ERA_Pneuma];
			// ԭʼ�⹥����=��ǰ��ǡ�5+��ǰ������1
			m_Temporary[EDT_Origin_ExDefine] = (INT)(nValue2 * 6 + nValue3 * 4); 
			SetPlayerAttEx(EDPE_MeleeDefine, m_Temporary[EDT_Origin_ExDefine] + m_TempAttEquipAdd[ERA_ExDefense]);
		}
		break;
	case ERA_Pneuma:			// ��ǰԪ��
		{
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_Pneuma, nValue1);

			// ԭʼ����=��ǰԪ����10
			m_Temporary[EDT_Origin_Power] = (INT)(nValue1 * 10);
			if(m_nPreBid[eType] > 0)
			{
				nValue3 = pLP->GetAttribute(ERA_MaxMP) + (INT)(m_nPreBid[eType] * 10);
				SetPlayerAttEx(EDPE_Power0, nValue3 /*+ m_TempAttEquipAdd[ERA_MaxMP]*/);
			}
			
			// ԭʼ�ڹ�����=��ǰԪ����5+��ǰ������1
			nValue2 = pLP->GetAttribute(ERA_Physique) + m_nPreBid[ERA_Physique];
			m_Temporary[EDT_Origin_InDefine] = (INT)(nValue1 * 4 + nValue2 * 6);
			SetPlayerAttEx(EDPE_MagicDefine, m_Temporary[EDT_Origin_InDefine] + m_TempAttEquipAdd[ERA_InDefense]);

			m_Temporary[EDT_Origin_ExDefine] = (INT)(nValue1 * 4 + nValue2 * 6); 
			SetPlayerAttEx(EDPE_MeleeDefine, m_Temporary[EDT_Origin_ExDefine] + m_TempAttEquipAdd[ERA_ExDefense]);
		}
		break;
	case ERA_InnerForce:		// ��ǰ����
		{
			nValue1 = nValue1 + m_nPreBid[eType];
			SetPlayerAtt(EDP_InnerForce, nValue1);

			// ԭʼ�ڹ�����=��ǰ������5
			m_Temporary[EDT_Origin_InDamage] = (INT)(nValue1 * 10);
			SetPlayerAttEx(EDPE_MagicDamage, m_Temporary[EDT_Origin_InDamage] + m_TempAttEquipAdd[ERA_InAttack]);
			
			// ԭʼ�ڹ�����=��ǰԪ����5+��ǰ������1
			nValue2 = pLP->GetAttribute(ERA_Physique) + m_nPreBid[ERA_Physique];
			nValue3 = pLP->GetAttribute(ERA_Pneuma) + m_nPreBid[ERA_Pneuma];
			m_Temporary[EDT_Origin_InDefine] = (INT)(nValue2 * 6 + nValue3 * 4);
			SetPlayerAttEx(EDPE_MagicDefine, m_Temporary[EDT_Origin_InDefine] + m_TempAttEquipAdd[ERA_InDefense]);
		}
		break;
	case ERA_Technique:			// ��ǰ����
		{/*
			nValue1 = nValue1 + m_nPreBid[eType];
			//SetPlayerAtt(EDP_Technique, nValue1);

			// ԭʼ��������=��ǰ������6
			m_Temporary[EDT_Origin_DmgSkill] = 0;//(INT)(nValue1 * 6); 
			SetPlayerAttEx(EDPE_DamageSkill, m_Temporary[EDT_Origin_DmgSkill]);//////////////////////////////////////////////////////////////////////////

			// ԭʼ����=��ǰ������10
			m_Temporary[EDT_Origin_Hit] = 0;//(INT)(nValue1 * 10);
			//SetPlayerAttEx(EDPE_Hit, m_Temporary[EDT_Origin_Hit]);//////////////////////////////////////////////////////////////////////////

			// ԭʼ�־���=[98]+������ǰֵ��0.1+����ǰֵ��0.1
			*/
		}
		break;
	case ERA_Agility:			// ��ǰ��
		{/*
			nValue1 = nValue1 + m_nPreBid[eType];
			//SetPlayerAtt(EDP_Agility, nValue1);

			// ԭʼ��������=��ǰ����4
			m_Temporary[EDT_Origin_DefSkill] = 0;//(INT)(nValue1 * 4);
			SetPlayerAttEx(EDPE_DefineSkill, m_Temporary[EDT_Origin_DefSkill]);//////////////////////////////////////////////////////////////////////////
			
			// ԭʼ����=��ǰ����10
			m_Temporary[EDT_Origin_Dodge] = 0;//(INT)(nValue1 * 10);
			//SetPlayerAttEx(EDPE_Dodge, m_Temporary[EDT_Origin_Dodge]);

			// ԭʼ�־���=[98]+������ǰֵ��0.1+����ǰֵ��0.1
			LevelUpData* pLevelupAtt = LevelUpData::Inst();
			if( P_VALID(pLevelupAtt) )
			{
				SetPlayerAttEx( EDPE_Toughness,100 + pLevelupAtt->GetRoleAttInc( ERA_MaxEndurance ,pLP->GetRoleLevel() ) );
			}
			*/
		}
		break;
	case ERA_AttPoint:			// ��ǰ���Ե���
		{
			SetPlayerOverplus(nValue1);
		}
		break;
	case ERA_MaxHP:				// �������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Health];
			m_TempAttEquipAdd[ERA_MaxHP] = nValue2;
			//SetPlayerAttEx(EDPE_Health0, nValue1, GETTRUECOLOR(nValue2));

			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				float fProValue =0;
				int Hp = plp->GetAttribute(ERA_HP);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), Hp,nValue1 + (INT)(m_nPreBid[ERA_Physique] * 6));
// 				m_pStcAttHPFont->SetText(szTmp);
// 				m_pStcAttHPFont->SetRefresh(TRUE);

				fProValue = (float)Hp/(float)nValue1;
				m_pProPlayerHP->SetValue(Hp);
				m_pProPlayerHP->SetMaxValue(nValue1);
				m_pProPlayerHP->SetSpeed(10000000.0f);
				m_pProPlayerHP->SetPicColor(0x00000000); 
				m_pProPlayerHP->SetForePicColor(0xFFFF402D);
				m_pProPlayerHP->SetText(szTmp);

			}
		}
		break;
	case ERA_HP:				// ��ǰ����
		{

			//SetPlayerAttEx(EDPE_Health, nValue1);
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				if (pLP->IsDead())
					break;

				float fProValue =0;
				int maxHp = plp->GetAttribute(ERA_MaxHP);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), nValue1,maxHp);
// 				m_pStcAttHPFont->SetText(szTmp);
// 				m_pStcAttHPFont->SetRefresh(TRUE);

				fProValue = (float)nValue1/(float)maxHp;
				m_pProPlayerHP->SetValue(nValue1);
				m_pProPlayerHP->SetMaxValue(maxHp);
				m_pProPlayerHP->SetSpeed(10000000.0f);
				m_pProPlayerHP->SetPicColor(0x00000000); 
				m_pProPlayerHP->SetForePicColor(0xFFFF402D);
				m_pProPlayerHP->SetText(szTmp);

			}
		}
		break;
	case ERA_MaxMP:				// �������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Power];
			m_TempAttEquipAdd[ERA_MaxMP] = nValue2;
			//SetPlayerAttEx(EDPE_Power0, nValue1, GETTRUECOLOR(nValue2));
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				float fProValue =0;
				int Mp = plp->GetAttribute(ERA_MP);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), Mp,nValue1 + (INT)(m_nPreBid[ERA_Pneuma] * 10));
// 				m_pStcAttMPFont->SetText(szTmp);
// 				m_pStcAttMPFont->SetRefresh(TRUE);

				fProValue = (float)Mp/(float)nValue1;
				m_pProPlayerMP->SetValue(Mp);
				m_pProPlayerMP->SetMaxValue(nValue1);
				m_pProPlayerMP->SetSpeed(10000000.0f);
				m_pProPlayerMP->SetPicColor(0x00000000); 
				m_pProPlayerMP->SetForePicColor(0xFF00A2FF);
				m_pProPlayerMP->SetText(szTmp);

			}
		}
		break;
	case ERA_MP:				// ��ǰ����
		{
			//SetPlayerAttEx(EDPE_Power, nValue1);
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				if (pLP->IsDead())
					break;

				float fProValue =0;
				int maxMp = plp->GetAttribute(ERA_MaxMP);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), nValue1,maxMp);
// 				m_pStcAttMPFont->SetText(szTmp);
// 				m_pStcAttMPFont->SetRefresh(TRUE);

				fProValue = (float)nValue1/(float)maxMp;
				m_pProPlayerMP->SetValue(nValue1);
				m_pProPlayerMP->SetMaxValue(maxMp);
				m_pProPlayerMP->SetSpeed(10000000.0f);
				m_pProPlayerMP->SetPicColor(0x00000000); 
				m_pProPlayerMP->SetForePicColor(0xFF00A2FF);
				m_pProPlayerMP->SetText(szTmp);

			}
		}
		break;
	case ERA_ExAttack:			// �⹦����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_ExDamge];
			//SetPlayerAttEx(EDPE_MeleeDamage0, nValue2, GETTRUECOLOR(nValue2));
			m_TempAttEquipAdd[ERA_ExAttack] = nValue2;

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + m_nPreBid[ERA_Strength]*10);
			m_pStcAttEx[EDPE_MeleeDamage]->SetText(szTmp);
			m_pStcAttEx[EDPE_MeleeDamage]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MeleeDamage]->SetRefresh(TRUE);

		}
		break;
	case ERA_ExDefense:			// �⹦����
		{
			
			nValue2 = nValue1 - m_Temporary[EDT_Origin_ExDefine];
			//SetPlayerAttEx(EDPE_MeleeDefine0, nValue2, GETTRUECOLOR(nValue2));
			m_TempAttEquipAdd[ERA_ExDefense] = nValue2;

			//int temp = _tcstol(m_pStcAttEx[EDPE_MeleeDefine]->GetText().c_str(),    NULL, 10);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + m_nPreBid[ERA_Physique]*6 + m_nPreBid[ERA_Pneuma]*4);
			m_pStcAttEx[EDPE_MeleeDefine]->SetText(szTmp);
			m_pStcAttEx[EDPE_MeleeDefine]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MeleeDefine]->SetRefresh(TRUE);
		}
		break;
	case ERA_InAttack:			// �ڹ�����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_InDamage];
			//SetPlayerAttEx(EDPE_MagicDamage0, nValue2, GETTRUECOLOR(nValue2));
			m_TempAttEquipAdd[ERA_InAttack] = nValue2;

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + m_nPreBid[ERA_InnerForce]*10);
			m_pStcAttEx[EDPE_MagicDamage]->SetText(szTmp);
			m_pStcAttEx[EDPE_MagicDamage]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MagicDamage]->SetRefresh(TRUE);
		}
		break;
	case ERA_InDefense:			// �ڹ�����
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_InDefine];
			//SetPlayerAttEx(EDPE_MagicDefine0, nValue2, GETTRUECOLOR(nValue2));
			m_TempAttEquipAdd[ERA_InDefense] = nValue2;

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + m_nPreBid[ERA_Physique]*6 + m_nPreBid[ERA_Pneuma]*4);
			m_pStcAttEx[EDPE_MagicDefine]->SetText(szTmp);
			m_pStcAttEx[EDPE_MagicDefine]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_MagicDefine]->SetRefresh(TRUE);
		}
		break;
	case ERA_AttackTec:			// ��������
		{
 			nValue2 = nValue1 - m_Temporary[EDT_Origin_DmgSkill];
			m_TempAttEquipAdd[ERA_AttackTec] = nValue2;
// 			SetPlayerAttEx(EDPE_DamageSkill0, nValue2, GETTRUECOLOR(nValue2));
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_DamageSkill]->SetText(szTmp);
			m_pStcAttEx[EDPE_DamageSkill]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_DamageSkill]->SetRefresh(TRUE);
		}
		break;
	case ERA_DefenseTec:		// ��������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_DefSkill];
			m_TempAttEquipAdd[ERA_DefenseTec] = nValue2;
			//SetPlayerAttEx(EDPE_DefineSkill0, nValue2, GETTRUECOLOR(nValue2));
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_DefineSkill]->SetText(szTmp);
			m_pStcAttEx[EDPE_DefineSkill]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_DefineSkill]->SetRefresh(TRUE);
		}
		break;
	case ERA_HitRate:			// ������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Hit];
			//SetPlayerAttEx(EDPE_Hit0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_Dodge:				// ������
		{
			nValue2 = nValue1 - m_Temporary[EDT_Origin_Dodge];
			//SetPlayerAttEx(EDPE_Dodge0, nValue2, GETTRUECOLOR(nValue2));
		}
		break;
	case ERA_Endurance:			// �־���
		{
			SetPlayerAttEx(EDPE_Toughness, nValue1);
// 			nValue2 = nValue1 - m_Temporary[EDT_Origin_Toughness];
// 			SetPlayerAttEx(EDPE_Toughness0, nValue2, GETTRUECOLOR(nValue2));
			
		}
		break;
	case ERA_MaxEndurance:
		{
			SetPlayerAttEx(EDPE_Toughness0, nValue1);
		}
		break;
	case ERA_Vitality:			// ����
		{
			//SetPlayerAttEx(EDPE_Energy, nValue1);
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				int maxWd = plp->GetAttribute(ERA_MaxVitality);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				//int temp = _tcstol(m_pStcAttEx[EDPE_WeaponDamage]->GetText().c_str(),    NULL, 10);
				//int temp = _tcstol(m_pStcAttEx[EDPE_Energy]->GetText().c_str(),    NULL, 10);
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), nValue1, maxWd);
				m_pStcAttEx[EDPE_Energy]->SetText(szTmp);
				m_pStcAttEx[EDPE_Energy]->SetTextColor(dwColor);
				m_pStcAttEx[EDPE_Energy]->SetRefresh(TRUE);
			}
			
		}
		break;
	case ERA_MaxVitality:		// ������
		{
			//SetPlayerAttEx(EDPE_Energy0, nValue1);
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				int maxWd = plp->GetAttribute(ERA_Vitality);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				//int temp = _tcstol(m_pStcAttEx[EDPE_WeaponDamage]->GetText().c_str(),    NULL, 10);
				//int temp = _tcstol(m_pStcAttEx[EDPE_Energy]->GetText().c_str(),    NULL, 10);
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), maxWd, nValue1);
				m_pStcAttEx[EDPE_Energy]->SetText(szTmp);
				m_pStcAttEx[EDPE_Energy]->SetTextColor(dwColor);
				m_pStcAttEx[EDPE_Energy]->SetRefresh(TRUE);
			}
		}
		break;
	case ERA_Speed_XZ:			// XZƽ���ٶ��ٶ�
		{
            // �Ƿ������״̬
            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pLp) && !P_VALID(pLp->GetMount()))
                SetPlayerAttEx(EDPE_MoveSpeed, nValue1);
		}
		break;
    case ERA_Speed_Mount:
        {
            // �Ƿ������״̬
            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pLp) && P_VALID(pLp->GetMount()))
                SetPlayerAttEx(EDPE_MoveSpeed, nValue1);
        }
        break;
	case ERA_WeaponDmgMin:		// ��С��������
		{
			//SetPlayerAttEx(EDPE_WeaponDamage, nValue1);
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				int maxWd = plp->GetAttribute(ERA_WeaponDmgMax);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				//int temp = _tcstol(m_pStcAttEx[EDPE_WeaponDamage]->GetText().c_str(),    NULL, 10);
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d-%d"), nValue1,maxWd);
				m_pStcAttEx[EDPE_WeaponDamage]->SetText(szTmp);
				m_pStcAttEx[EDPE_WeaponDamage]->SetTextColor(dwColor);
				m_pStcAttEx[EDPE_WeaponDamage]->SetRefresh(TRUE);
			}
		}
		break;
	case ERA_WeaponDmgMax:		// �����������
		{
			/*SetPlayerAttEx(EDPE_WeaponDamage0, nValue1);*/
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if(P_VALID(plp))
			{
				int maxWd = plp->GetAttribute(ERA_WeaponDmgMin);
				TCHAR szTmp[X_SHORT_NAME] = {0};
				//int temp = _tcstol(m_pStcAttEx[EDPE_WeaponDamage]->GetText().c_str(),    NULL, 10);
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d-%d"), maxWd,nValue1);
				m_pStcAttEx[EDPE_WeaponDamage]->SetText(szTmp);
				m_pStcAttEx[EDPE_WeaponDamage]->SetTextColor(dwColor);
				m_pStcAttEx[EDPE_WeaponDamage]->SetRefresh(TRUE);
			}
		}
		break;
	case ERA_WeaponSoul:		// ���
		{
			SetPlayerAttEx(EDPE_WuHun, nValue1);
		}
		break;
	case ERA_Armor:				// ���߻���
		{
			SetPlayerAttEx(EDPE_ArmDefine, nValue1);
		}
		break;
	case ERA_Derate_Ordinary:	// ��ͨ�˺�����
		{
            //SetPlayerResist(EDR_OrdinaryResist, CalExAttackResist(nValue1));

			//TCHAR szTmp[X_SHORT_NAME] = {0};
			////nValue1 += pLP->GetAttribute(ERA_Derate_ExAttack) + pLP->GetAttribute(ERA_Derate_ALL);
			//_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), CalExAttackResist(nValue1));
			//m_pStcAttEx[EDPE_Derate_Ordinary]->SetText(szTmp);
			//m_pStcAttEx[EDPE_Derate_Ordinary]->SetTextColor(dwColor);
			//m_pStcAttEx[EDPE_Derate_Ordinary]->SetRefresh(TRUE);
		}
		break;
	case ERA_Derate_Bleeding:	// ��Ѫ�˺�����
		{
			SetPlayerResist(EDR_BleedResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Brunt:		// ����˺�����
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			nValue1 += pLP->GetAttribute(ERA_Derate_ExAttack) + pLP->GetAttribute(ERA_Derate_ALL);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Derate_Brunt]->SetText(szTmp);
			m_pStcAttEx[EDPE_Derate_Brunt]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Derate_Brunt]->SetRefresh(TRUE);*/
			//SetPlayerResist(EDR_BruntResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Bang:		// �ػ��˺�����
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			nValue1 += pLP->GetAttribute(ERA_Derate_ExAttack) + pLP->GetAttribute(ERA_Derate_ALL);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Derate_Bang]->SetText(szTmp);
			m_pStcAttEx[EDPE_Derate_Bang]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Derate_Bang]->SetRefresh(TRUE);*/
			//SetPlayerResist(EDR_BangResist, CalExAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Poison:		// �����˺�����
		{
			SetPlayerResist(EDR_PoisonResist, CalInAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Thinker:	// �����˺�����
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			nValue1 += pLP->GetAttribute(ERA_Derate_InAttack) + pLP->GetAttribute(ERA_Derate_ALL);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Derate_Thinker]->SetText(szTmp);
			m_pStcAttEx[EDPE_Derate_Thinker]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Derate_Thinker]->SetRefresh(TRUE);*/
			//SetPlayerResist(EDR_ThinkerResist, CalInAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Injury:		// �����˺�����
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			nValue1 += pLP->GetAttribute(ERA_Derate_InAttack) + pLP->GetAttribute(ERA_Derate_ALL);
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Derate_Injury]->SetText(szTmp);
			m_pStcAttEx[EDPE_Derate_Injury]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Derate_Injury]->SetRefresh(TRUE);*/
			//SetPlayerResist(EDR_InjuryResist, CalInAttackResist(nValue1));
		}
		break;
	case ERA_Derate_Stunt:		// �����˺�����
		{
		}
		break;
	case ERA_Derate_ExAttack:	// �⹦�˺�����
		{
            UpdateAttribute(ERA_Derate_Ordinary);
            UpdateAttribute(ERA_Derate_Bleeding);
            UpdateAttribute(ERA_Derate_Brunt);
            UpdateAttribute(ERA_Derate_Bang);
		}
		break;
	case ERA_Derate_InAttack:	// �ڹ��˺�����
		{
            UpdateAttribute(ERA_Derate_Poison);
            UpdateAttribute(ERA_Derate_Thinker);
            UpdateAttribute(ERA_Derate_Injury);
		}
		break;
	case ERA_Derate_ALL:		// �����˺�����
		{
            UpdateAttribute(ERA_Derate_Ordinary);
            UpdateAttribute(ERA_Derate_Bleeding);
            UpdateAttribute(ERA_Derate_Brunt);
            UpdateAttribute(ERA_Derate_Bang);

            UpdateAttribute(ERA_Derate_Poison);
            UpdateAttribute(ERA_Derate_Thinker);
            UpdateAttribute(ERA_Derate_Injury);
		}
		break;
	case ERA_Morale:			// ʿ��
		{
			//SetPlayerAttEx(EDPE_Morale, nValue1);
		}
		break;
	case ERA_Injury:			// ����
		{
			SetPlayerAttEx(EDPE_InternalInjury, nValue1, MINUS_RED_COLOR);

			//��ʼ��ʱ�������Ϊ0����ʾ
			if( nValue1!=m_nInternalInjury && 0==nValue1 )
			{
				m_nInternalInjury = nValue1;
				return;
			}

			if( nValue1 == 0 && nValue1!=m_nInternalInjury)
			{
				ChatFrame* pFrame = (ChatFrame*)m_pMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					pFrame->PushInfo( g_StrTable[_T("RoleChat_Injury1")], ESCC_System );
				}
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("RoleChat_Injury3")]);
			}
			else if( nValue1 > 0  )
			{
				ChatFrame* pFrame = (ChatFrame*)m_pMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					pFrame->PushInfo( g_StrTable[_T("RoleChat_Injury0")], ESCC_System );
				}
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("RoleChat_Injury2")]);

				LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
				if( P_VALID(pLp) )
				{
					int nvalue = pLp->GetAttribute( ERA_HP );
					if( nvalue == 0 || pLp->IsDead() )
					{
						TCHAR szInjury[X_LONG_NAME] = {0};
						_sntprintf( szInjury, X_LONG_NAME, g_StrTable[_T("Dead_Injury")], nValue1 );
						CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szInjury);
					}
				}
			}
		}
		break;
	case ERA_Fortune:			// ��Ե
		{
			SetPlayerAttEx(EDPE_Luck, nValue1);
		}
		break;
	case ERA_Appearance:		// ����
		{
			//SetPlayerAttEx(EDPE_Feature, nValue1);
		}
		break;
	case ERA_Rein:				// ͳ��
		{
			//SetPlayerAttEx(EDPE_Command, nValue1);
		}
		break;
	case ERA_Knowledge:			// ����
		{

		}
		break;
	case ERA_Morality:			// ����
		{
			SetPlayerAttEx(EDPE_Morality, nValue1);
		}
		break;
	case ERA_Culture:			// ��Ϊ,����ֶ�ò��û�ã���ERA_Shili�غ���
		{
			//SetPlayerAttEx(EDPE_Cultivate, nValue1);
		}
		break;
	case ERA_Shili:
		{
			//��������趨��ͨ����Ϣ����ȡ
			//SetPlayerAttEx(EDPE_Cultivate, nValue1);
		}
		break;
	case ERA_Weary:           //ƣ��
		{
			float fProValue = nValue1;
			if (fProValue<0)
			{
				fProValue = 0;
			}
			const float MAX_WEARY_VALUE = GetWearyUpperLimit();
			if (fProValue>MAX_WEARY_VALUE)
			{
				fProValue = MAX_WEARY_VALUE;
			}
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), (int)fProValue, (int)MAX_WEARY_VALUE);
			m_pStcAttWearyFont->SetText(szTmp);
			m_pStcAttWearyFont->SetRefresh(TRUE);
			//SetPlayerAttEx(EDPE_Weary, fProValue);
			
			fProValue = fProValue/MAX_WEARY_VALUE;
			m_pProPlayerWeary->SetValue(fProValue);
			m_pProPlayerWeary->SetSpeed(10000000.0f);
			m_pProPlayerWeary ->SetPicColor(0x00000000); 
				
			if (nValue1<0)
			{
				 nValue1=0;
			} 
			else if (nValue1<=0.3f*MAX_WEARY_VALUE)
			{ 
				//weary value <= 30% ,show green   
				m_pProPlayerWeary->SetForePicColor(0xFF80FF00);
			} 
			else if (nValue1<0.8f*MAX_WEARY_VALUE)
			{
				//yellow
				m_pProPlayerWeary->SetForePicColor(0xFFF8FD13);
			} 
			else if (nValue1<MAX_WEARY_VALUE)
			{
				//red
				m_pProPlayerWeary->SetForePicColor(0xFFFF6088);
			}
			else
			{
				nValue1 = MAX_WEARY_VALUE;
				m_pProPlayerWeary->SetForePicColor(0xFFFF6088);
			}
		 
		}
		break;
	case ERA_Toughness:        //����
		{
			/*iToughnes = nValue1;
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR),g_StrTable[_T("LocalPlayerAtt_83")], nValue1);
			m_pStcToughnes->SetText(szTmp);
			m_pStcToughnes->SetTextColor(0xFF8B65B1);*/
		}
		break;
	case ERA_transform_ExAttack:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + pLP->GetAttribute(ERA_transform_Bang) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_ShenBing]->SetText(szTmp);
			m_pStcAttEx[EDPE_ShenBing]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_ShenBing]->SetRefresh(TRUE);

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_Brunt) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_XuanLing]->SetText(szTmp);
			m_pStcAttEx[EDPE_XuanLing]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_XuanLing]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_transform_InAttack:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + pLP->GetAttribute(ERA_transform_Injury) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_QianKun]->SetText(szTmp);
			m_pStcAttEx[EDPE_QianKun]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_QianKun]->SetRefresh(TRUE);

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_Thinker) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_XianYin]->SetText(szTmp);
			m_pStcAttEx[EDPE_XianYin]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_XianYin]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_transform_Bang:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + pLP->GetAttribute(ERA_transform_ExAttack) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_ShenBing]->SetText(szTmp);
			m_pStcAttEx[EDPE_ShenBing]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_ShenBing]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_transform_Brunt:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_ExAttack) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_XuanLing]->SetText(szTmp);
			m_pStcAttEx[EDPE_XuanLing]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_XuanLing]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_transform_Injury:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_InAttack) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_QianKun]->SetText(szTmp);
			m_pStcAttEx[EDPE_QianKun]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_QianKun]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_transform_Thinker:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_InAttack) + pLP->GetAttribute(ERA_transform_ALL));
			m_pStcAttEx[EDPE_XianYin]->SetText(szTmp);
			m_pStcAttEx[EDPE_XianYin]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_XianYin]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_transform_ALL:
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + pLP->GetAttribute(ERA_transform_Bang) + pLP->GetAttribute(ERA_transform_ExAttack));
			m_pStcAttEx[EDPE_ShenBing]->SetText(szTmp);
			m_pStcAttEx[EDPE_ShenBing]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_ShenBing]->SetRefresh(TRUE);

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1 + pLP->GetAttribute(ERA_transform_Brunt) + pLP->GetAttribute(ERA_transform_ExAttack));
			m_pStcAttEx[EDPE_XuanLing]->SetText(szTmp);
			m_pStcAttEx[EDPE_XuanLing]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_XuanLing]->SetRefresh(TRUE);

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_Injury) + pLP->GetAttribute(ERA_transform_InAttack));
			m_pStcAttEx[EDPE_QianKun]->SetText(szTmp);
			m_pStcAttEx[EDPE_QianKun]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_QianKun]->SetRefresh(TRUE);

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1+ pLP->GetAttribute(ERA_transform_Thinker) + pLP->GetAttribute(ERA_transform_InAttack));
			m_pStcAttEx[EDPE_XianYin]->SetText(szTmp);
			m_pStcAttEx[EDPE_XianYin]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_XianYin]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_VipLevel:
		{
			LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
			if (!P_VALID(pLp))
				break;

			time2 = ServerTime::Inst()->CalCurrentServerDwordTime();
			time3 = pLP->GetVipDays();
			timeDiff = CalcTimeDiff(time2,time3);
			LastDays = pLP->GetVipDaysCanLast() - timeDiff/3600/24;
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR),_T("%d"), LastDays);

			m_pStcSilverVIP->SetInvisible(TRUE);
			m_pStcGoldVIP->SetInvisible(TRUE);
			m_pStcPlatinumVIP->SetInvisible(TRUE);
			m_pStcSilverVIPLeftTime->SetText(_T(""));
			m_pStcGoldVIPLeftTime->SetText(_T(""));
			m_pStcPlatinumVIPLeftTime->SetText(_T(""));
			if (nValue1 == 0)
			{
				break;
			}
			else if (nValue1 == 1)
			{
				m_pStcSilverVIP->SetInvisible(FALSE);
				m_pStcSilverVIPLeftTime->SetText(szTmp);
			}
			else if (nValue1 == 2)
			{
				m_pStcGoldVIP->SetInvisible(FALSE);
				m_pStcGoldVIPLeftTime->SetText(szTmp);
			}	
			else if (nValue1 == 3)
			{
				m_pStcPlatinumVIP->SetInvisible(FALSE);
				m_pStcPlatinumVIPLeftTime->SetText(szTmp);
			}
			//ͬʱ����ƣ��ֵ
			UpdateAttribute(ERA_Weary);
		}
		break;
	case ERA_Regain_Addtion:		
		{
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Regain_Addtion]->SetText(szTmp);
			m_pStcAttEx[EDPE_Regain_Addtion]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Regain_Addtion]->SetRefresh(TRUE);
		}
		break;
	case ERA_Crit_Rate:			
		{
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d%%"), nValue1/10);
			m_pStcAttEx[EDPE_Crit_Rate]->SetText(szTmp);
			m_pStcAttEx[EDPE_Crit_Rate]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Crit_Rate]->SetRefresh(TRUE);
		}
		break;
	case ERA_Crit_Amount:			
		{
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d%%"), nValue1/100);
			m_pStcAttEx[EDPE_Crit_Amount]->SetText(szTmp);
			m_pStcAttEx[EDPE_Crit_Amount]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Crit_Amount]->SetRefresh(TRUE);
		}
		break;
	case ERA_Resist_Bleeding:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1>200 ? 200:nValue1);
			m_pStcAttEx[EDPE_Resist_Bleeding]->SetText(szTmp);
			m_pStcAttEx[EDPE_Resist_Bleeding]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Resist_Bleeding]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Resist_Weak:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1>200 ? 200:nValue1);
			m_pStcAttEx[EDPE_Resist_Weak]->SetText(szTmp);
			m_pStcAttEx[EDPE_Resist_Weak]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Resist_Weak]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Resist_Choas:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1>200 ? 200:nValue1);
			m_pStcAttEx[EDPE_Resist_Choas]->SetText(szTmp);
			m_pStcAttEx[EDPE_Resist_Choas]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Resist_Choas]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Resist_Special:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1>200 ? 200:nValue1);
			m_pStcAttEx[EDPE_Resist_Special]->SetText(szTmp);
			m_pStcAttEx[EDPE_Resist_Special]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Resist_Special]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Toughness_god:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Ren]->SetText(szTmp);
			m_pStcAttEx[EDPE_Ren]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Ren]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Toughness_strength:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_Crit_Resist]->SetText(szTmp);
			m_pStcAttEx[EDPE_Crit_Resist]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_Crit_Resist]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Fixed_damage_aggrandizement:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_fix_up]->SetText(szTmp);
			m_pStcAttEx[EDPE_fix_up]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_fix_up]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Aging_effect_strengthening:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_weak_up]->SetText(szTmp);
			m_pStcAttEx[EDPE_weak_up]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_weak_up]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Contrel_effect_deepen:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_control_up]->SetText(szTmp);
			m_pStcAttEx[EDPE_control_up]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_control_up]->SetRefresh(TRUE);*/
		}
		break;
	case ERA_Slowing_effect_strengthening:			
		{
			/*TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue1);
			m_pStcAttEx[EDPE_slow_up]->SetText(szTmp);
			m_pStcAttEx[EDPE_slow_up]->SetTextColor(dwColor);
			m_pStcAttEx[EDPE_slow_up]->SetRefresh(TRUE);*/
		}
		break;
	default:
		break;
	}
	
}

VOID StateFrame::AddPreBidPoint( ERoleAttribute eType, INT nInc )
{
	ASSERT(eType >= ERA_AttA_Start && eType <= ERA_AttA_End);

	m_nPreBid[eType] += nInc;
	UpdateAttribute(eType);
	
	m_pBtnSure->SetEnable(TRUE);
	m_pBtnCancel->SetEnable(TRUE);
}

VOID StateFrame::RemoveBidPoint()
{
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		m_nPreBid[i] = 0;
	}

	m_pBtnSure->SetEnable(FALSE);
	m_pBtnCancel->SetEnable(FALSE);
}

VOID StateFrame::SendNetBidAtt()
{
	tagNC_RoleBidAttPoint cmd;
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		cmd.nAttPointAdd[i] = m_nPreBid[i];
	}
	m_pSession->Send(&cmd);
}

ERoleAttribute StateFrame::TransBtn2Att( EDisplayProperty eType )
{
	switch (eType)
	{
	case EDP_Physique:
		return ERA_Physique;
	case EDP_Strength:
		return ERA_Strength;
	case EDP_InnerForce:
		return ERA_InnerForce;
	case EDP_Pneuma:
		return ERA_Pneuma;
	case EDP_Agility:
		return ERA_Agility;
	case EDP_Technique:
		return ERA_Technique;
	}
	return ERA_Null;
}

VOID StateFrame::SetBiddenPoint( ERoleAttribute eType, INT nValue )
{
	ASSERT(eType >= ERA_AttA_Start && eType <= ERA_AttA_End);

	m_nPreBid[eType] = nValue;

	// ��������
}

VOID StateFrame::UpdateBidAttribute()
{
	for(INT i=0; i<X_ERA_ATTA_NUM; ++i)
	{
		UpdateAttribute((ERoleAttribute)i);
	}
    for (int i = 0; i < X_ERA_DERATE_NUM; ++i)
    {
        UpdateAttribute((ERoleAttribute)(ERA_Derate_Start + i));
    }
	UpdateAttribute(ERA_MaxHP);
	UpdateAttribute(ERA_MaxMP);
	UpdateAttribute(ERA_AttPoint);
	UpdateAttribute(ERA_Shili);

	UpdateAttribute(ERA_ExAttack);
	UpdateAttribute(ERA_ExDefense);
	UpdateAttribute(ERA_InAttack);
	UpdateAttribute(ERA_InDefense);
	UpdateAttribute(ERA_MaxEndurance);
	UpdateAttribute(ERA_AttackTec);
	UpdateAttribute(ERA_DefenseTec);
}

int StateFrame::CalExAttackResist( int src )
{
    LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLP))
        return 0;

    int nRetValue = src
        + pLP->GetAttribute(ERA_Derate_ALL)
        + pLP->GetAttribute(ERA_Derate_ExAttack);

    if (nRetValue > MAX_PLAYER_RESIST)
        nRetValue = MAX_PLAYER_RESIST;
    else if (nRetValue < MIN_PLAYER_RESIST)
        nRetValue = MIN_PLAYER_RESIST;

    return nRetValue;
}

int StateFrame::CalInAttackResist( int src )
{
    LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
    if (!P_VALID(pLP))
        return 0;

    int nRetValue = src
        + pLP->GetAttribute(ERA_Derate_ALL)
        + pLP->GetAttribute(ERA_Derate_InAttack);

    if (nRetValue > MAX_PLAYER_RESIST)
        nRetValue = MAX_PLAYER_RESIST;
    else if (nRetValue < MIN_PLAYER_RESIST)
        nRetValue = MIN_PLAYER_RESIST;

    return nRetValue;
}

DWORD StateFrame::OnNetSynTotalStrength(tagNS_SynTotalStrength *pNetCmd, DWORD)
{
	//SetPlayerAttEx(EDPE_Cultivate, pNetCmd->nTotalStrength);
	return 0;
}

DWORD StateFrame::NetRoleClearAttPoint(tagNS_RoleClearAttPoint* pNetCmd, DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("NS_RoleClearAttPoint_Success")]);
		UpdateBidAttribute();
	}
	else if (pNetCmd->dwErrorCode == E_ClearAttPoint_ItemNotValid)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NS_RoleClearAttPoint_NoSuitItem")]);
	}
	else if (pNetCmd->dwErrorCode == E_ClearAttPoint_NoNeedClear)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NS_RoleClearAttPoint_NoNeedTo")]);
	}

	return 0;
}

DWORD StateFrame::OnNS_SyncStartVipTime(tagNS_SyncStartVipTime* pNetCmd, DWORD)
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		plp->SetVipDays(pNetCmd->dwStartVipTime);
		plp->SetVipDaysCanLast(pNetCmd->dwVipMaxDays);
		UpdateAttribute(ERA_VipLevel);
	}
	return 0;
}

VOID StateFrame::UpdateSettingsOfLocalPlayer(LocalPlayer* plp)
{
	m_curDisplaySet = plp->GetDisplaySet();
	m_pPbnShowHead->SetState( false == m_curDisplaySet.bHideHead ? EGUIBS_PushDown : EGUIBS_PopUp );
	m_pPbnShowPiFeng->SetState( false == m_curDisplaySet.bHideBack ? EGUIBS_PushDown : EGUIBS_PopUp );
}

DWORD StateFrame::OnMsgClearPoint(tagMsgBoxEvent* pGameEvent)
{
	if(pGameEvent->eResult == MBF_OK)
	{
		ItemContainer* pItemCon = ItemMgr::Inst()->GetPocket();
		if(P_VALID(pItemCon))
		{
			Item* pItem = pItemCon->GetSpecFuncItem( EICT_Bag, EISF_RemoveAttPt );
			if (P_VALID(pItem))
			{
				tagNC_RoleClearAttPoint cmd;
				cmd.n64ItemID = pItem->GetItemId();
				m_pSession->Send( &cmd );
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotHaveRemoveAttPtItem")]);
			}
		}
	}
	return 0;
}

float StateFrame::GetWearyUpperLimit()
{
	float nWearyUpperLimit = 100.0f;
	EVipLevel eVipLvl = (EVipLevel)RoleMgr::Inst()->GetLocalPlayer()->GetAttribute(ERA_VipLevel);
	switch (eVipLvl)
	{
	case EVL_Silver: // ���꼶vip
		nWearyUpperLimit = 120.0f;
		break;
	case EVL_Gold: // ���꼶vip
		nWearyUpperLimit = 150.0f;
		break;
	case EVL_Platinum: // �׽�vip
		nWearyUpperLimit = 200.0f;
		break;
	case EVL_None: // ����vip
	default:
		nWearyUpperLimit = 100.0f;
		break;
	}
	return nWearyUpperLimit;
}

BOOL StateFrame::EscCancel()
{
	if (!m_pWndAttExShow->IsInvisible())
	{
		m_pWndAttExShow->SetInvisible(TRUE);
		return TRUE;
	}
	return FALSE;
}

VOID StateFrame::AlignAttEXShowWnd()
{
	if (P_VALID(m_pWndAttExShow))
		m_pWndAttExShow->Align();
}
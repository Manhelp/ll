#pragma once
#include "gameframe.h"

class FamilyMemberFrame;
class FamilySpriteFrame;
class FamilyQuestFrame;
struct tagNC_OpenFamilyQuestPage;
struct tagFamilyNameEvent;

class FamilyFrame :
	public GameFrame
{
public:
	FamilyFrame(void);
	~FamilyFrame(void);
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowWnd(BOOL bShow=TRUE);
	VOID RefreshUI();
	BOOL IsShowing(); 
	// common----------------------------------------------------------------------------------------------------
	tstring GetGUIFullName()
	{
		if(P_VALID(m_pWndCaption))
			return m_pWndCaption->GetFullName();
		else 
			return _T("");
	};
	// �����Ա----------------------------------------------------------------------------------------------------

	// ��������----------------------------------------------------------------------------------------------------

	// ��������----------------------------------------------------------------------------------------------------
private:
	// �����Ա----------------------------------------------------------------------------------------------------

	// ��������----------------------------------------------------------------------------------------------------

	// ��������----------------------------------------------------------------------------------------------------
	
	// common----------------------------------------------------------------------------------------------------
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventFamilyOpen( tagGameEvent* pGameEvent );
	DWORD OnEventFamilyClose( tagGameEvent* pGameEvent );
	DWORD OnEventGetName( tagFamilyNameEvent* pGameEvent );

private:
	TSFPTrunk<FamilyFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;
	// common----------------------------------------------------------------------------------------------------
	GUIWnd*										m_pWnd;
	GUIWnd*										m_pWndCaption;
	GUIPushButton*							m_pPbnState;		// �����Ա״̬
	GUIPushButton*							m_pPbnSprite;		// ��������
	GUIPushButton*							m_pPbnQuest;		// ��������
	
	GUIButton*								m_pClose;
	GUIStatic*								m_pStcFamilyName;
	GUIStatic*								m_pStcLeaderName;
	GUIStatic*								m_pStcMenbersNum;
	GUIStatic*								m_pStcTodayActiv;
	
	// �����Ա----------------------------------------------------------------------------------------------------
	FamilyMemberFrame*						m_pWndMember;		//�������
	// ��������----------------------------------------------------------------------------------------------------
	FamilyQuestFrame*						m_pFamilyQuestFrame;
	// ��������----------------------------------------------------------------------------------------------------
	FamilySpriteFrame*						m_pSpriteFrame;
};

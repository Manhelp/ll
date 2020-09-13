#pragma once

#include "..\WorldDefine\easybuy.h"

class IconStatic;
class MallButton;
class EasyBuyFrame : public GameFrame
{
public:
	EasyBuyFrame(void);
	~EasyBuyFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 Show( DWORD dwUiType );
	VOID		 Hide(void);

	BOOL		 IsVisible(void) const;
	BOOL		 ReloadUI(LPCSTR alignCtrl, LPCSTR alignType, LPCSTR szNewName = NULL );
	VOID		 Align(void);

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	void		 RefreshItemFlag();
	DWORD		 OnClosePackageEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseSocialEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseStorageEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseQuestEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseRoleSkillEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseRoleInfoEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnClosePetEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseComposeEasyBuyFrame( tagGameEvent* pGameEvent );


private:
	TSFPTrunk<EasyBuyFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd						*m_pWnd;							// ������

	IconStatic*					m_pItemSel[UI_MAX_ITEM];			// ��Ʒѡ���
	MallButton*					m_pItem[UI_MAX_ITEM];				// ��Ʒ
	GUIStatic					*m_pStcYuanBao[UI_MAX_ITEM];		// ��Ʒ�۸�
	GUIStatic					*m_pStcBao[UI_MAX_ITEM];			// ��

	GUIButton					*m_pBtnMall;						// �̳ǽ���
	GUIButton					*m_pBtnClose;						// �رմ��ڰ�ť
};
/********************************************************************

** �ļ���: FairyAutoFeedFrame.h

** Copyright (c) �Ϻ�������缼�����޹�˾�����ֹ�˾ All Right Reserved

** ������: yfw

** ��  ��: 10/18/2010

** �޸���: yfw

** ��  ��: 10/18/2010

** ��  ��: �����Զ�ι��

*********************************************************************/
#pragma once

class IconStatic;
struct tagFairyMood;
struct tagUseFairyFoodEvent;
struct tagItemPosChangeExEvent;
struct tagNS_RolePutDownItem;
struct tagNS_FabaoFeeding;
struct tagRoleGetNameEvent;

class FairyAutoFeedFrame : public GameFrame
{
public:
	FairyAutoFeedFrame(void);
	~FairyAutoFeedFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	VOID ShowUI(bool bShow);

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent);
	VOID PutItem( INT16 nPos, bool bMainFood = true );
	VOID UpdateUI( DWORD &dwFoodID );
	VOID ReadSettingFromFile();
	VOID SaveSettingToFile();
	DWORD OnUpdateFairyMood(tagFairyMood* pEvent);
	DWORD OnEventChangeFairy(tagGameEvent* pGameEvent);
	DWORD OnNoFairyFood( tagMsgBoxEvent *pGameEvent );
	DWORD OnUseFairyFood( tagUseFairyFoodEvent *pGameEvent );
	VOID LoadFromFile();
	bool IfCanEat(DWORD dwTypeID, bool bKnownFairy = false, bool bShowTips = true );
	DWORD NetFairyFeed( tagNS_FabaoFeeding *pNetCmd, DWORD );
	DWORD NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD );

private:
	TSFPTrunk<FairyAutoFeedFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;					// ������
	GUIEditBox *m_pStcMoodRate;		// �������
	GUIEditBox *m_pStcIntervalTime;	// ���ʱ��
	IconStatic *m_pIconMainFood;	// ����ʳ��
	IconStatic *m_pIconSecFood;		// ��ʳ��
	GUIStatic *m_pStcMainFoodName;
	GUIStatic *m_pStcMainFoodNum;
	GUIStatic *m_pStcSecFoodName;
	GUIStatic *m_pStcSecFoodNum;
	GUIButton *m_pBtnAuto;
	GUIButton *m_pBtnClose;
	GUIButton *m_pBtnCancle;
	INT m_nMoodRate;
	INT m_nTime;
	DWORD m_dwEscapeTime;
	DWORD m_dwMainFood;
	DWORD m_dwSecFood;
	INT m_nFairyMood;
	bool m_bAutoFeed;
	bool m_bWarning;
	map<DWORD, INT> m_mapFairyFood;
};
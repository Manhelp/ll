#pragma once


struct tagCityWarAssistConfirmRefreshEvent;

class CityWarAssistConfirmFrame : public GameFrame
{
public:
	CityWarAssistConfirmFrame();
	~CityWarAssistConfirmFrame();
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pGUIEvent);
	DWORD OnEventRefreshUI(tagCityWarAssistConfirmRefreshEvent *pEvent);
	DWORD OnMsgBoxEventAssistConfirm(tagMsgBoxEvent *pEvent);
	VOID UpdateUI();
private:
	TSFPTrunk<CityWarAssistConfirmFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<GUISystem> m_pGUI;

	GUIWnd *m_pWnd;
	GUIListBox *m_pLstGuilds;	// ������������
	GUIListBox *m_pLstAssist;	// ȷ��Э������
	GUIButton *m_pBtnConfirm;	// ͬ��
	GUIButton *m_pBtnNotConfirm;// ����
	GUIButton *m_pBtnOK;		// ȷ��
	GUIButton *m_pBtnCancel;	// ȡ��
	GUIButton *m_pBtnClose;		// �ر�

	typedef std::vector<DWORD> GuildArray;
	GuildArray m_vecGuilds;				// ���������İ���
	GuildArray m_vecAssistGuilds;		// ȷ�ϵİ���
	int m_nGuildRow;
	int m_nAssistRow;
	DWORD m_dwSelectedGuildID;
	int m_nCurSelectGuildRow;
	int m_nCurSelectAssistRow;
};
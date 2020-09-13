#pragma once

class IconStatic;
struct tagCityWarCityRefreshEvent;
struct tagNS_GetCityWarInfo;
struct tagNS_GetCityWarCity;
struct tagGetSomeGuildNameEvent;


// ��С��ͼ��ť�����鿴��ս��Ϣ���������ɵȣ�
class CityWarInfoFrame : public GameFrame
{
public:
	CityWarInfoFrame();
	~CityWarInfoFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnEventCityWarCityRefresh(tagCityWarCityRefreshEvent *pEvent);
	DWORD OnEventGetSomeGuildName(tagGetSomeGuildNameEvent *pEvent);
	DWORD OnEventViewCityInfoMsgBox(tagMsgBoxEvent *pEvent);
	DWORD OnNetGetCityWarInfo(tagNS_GetCityWarInfo *pNetCmd, DWORD);
	DWORD OnNetGetCityWarCity(tagNS_GetCityWarCity *pNetCmd, DWORD);
	void UpdateUI();
private:
	TSFPTrunk<CityWarInfoFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<GUISystem> m_pGUI;

	GUIWnd *m_pWnd;
	GUIStatic *m_pStcCityName;
	IconStatic *m_pIstCityIcon;
	GUIButton *m_pBtnCityInfo;
	GUIListBox *m_pLstWarCity;
	GUIListBox *m_pLstAttacks;
	GUIListBox *m_pLstDefences;
	GUIButton *m_pBtnClose;

	int m_nCityRow;
	int m_nCurCityRow;

	std::vector<DWORD> m_vecCities;
	std::vector<DWORD> m_vecAttacks;
	std::vector<DWORD> m_vecDefenses;

	int m_nGuildNameGetFlag;
};
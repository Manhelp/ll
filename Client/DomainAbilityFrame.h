#pragma once
#include "gameframe.h"

class IconStatic;
struct tagUpdateLPAttEvent;
struct tagNS_GetRoleInitState_Area;
struct tagNS_AddArea;
struct tagNS_ActivationArea;
struct tagNS_RareAreaInfo;
struct tagNS_PromotionClergy;
struct tagNS_ChangeClergy;
#define DOMAIN_ROW_NUM 7
#define DOMAIN_COLUNM_NUM 3
const int MAX_GOD_LEVEL = 20;

struct tagAbilityButton
{
	GUIStatic*	pStMainBack;	// �����ؼ�
	GUIPushButton* pPbSelected;	// �Ƿ�Ҫѡ�� �Թ�
	IconStatic*	pIsPic;	// ͼƬ
	GUIStatic*	pStName;	// ����
	GUIStatic*	pStLevel;	// �ȼ�
	GUIStatic*	pStDesc;	// ˵��

	// ˢ��ͼ�� ����IDΪ5λID
	void RefreshAbilityButton(DWORD dwID);
	void SetActive(bool b)
	{
		bActive = b;
		pPbSelected->SetState(bActive?EGUIBS_PushDown:EGUIBS_PopUp,TRUE,FALSE);
	}
	void HideSelectBtn(bool b)
	{
		pPbSelected->SetInvisible(b);
	}
	void Clear()
	{
		bActive = false;
		dwSkillID = 0;
		bUnKnow = false;
	}

	bool bActive;	//	�Ƿ񱻼���
	DWORD dwSkillID;
	bool bUnKnow;	// �Ƿ���δ֪����

	tagAbilityButton():bActive(false),dwSkillID(0),bUnKnow(false){}

};

struct DomianData
{
	bool bActive;
	DWORD dwSkillID;
	DWORD dwTime;

	DomianData():bActive(false),dwSkillID(0),dwTime(0){}
};

struct GodLevelPic
{
	GUIStatic* pBack;
	GUIStatic* pText;
};
enum EDomain
{
	ED_God = 0,			// ��ְ����
	ED_FreePick,	// ��ѡ����
	ED_Rare,			// ϡ������
	ED_Enabled,		// ����Ч����
};

enum
{
	E_LeftWord = 0,
	E_RightWord,
	E_MidWord,
	E_WordNum,
};

struct WordDesc
{
	GUIStatic* pWord;
	GUIStatic* pNum;
};

class DomainAbilityFrame :
	public GameFrame
{
public:
	DomainAbilityFrame(void);
	~DomainAbilityFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID OpenFrame();
	VOID CloseFrame(){m_pWnd->SetInvisible(TRUE);}
	bool IsWindowVisible(){ return !m_pWnd->IsInvisible();}
private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	void GetShowFreePickDomainInfo(int &level,int &pos);

	void RefreshUI();
	void RefreshFreePickDomain();
	void RefreshRareDomain();
	void RefreshGodDomain();
	void RefreshAllDomains();

	void RefreshPage();
	void RefreshPlayerInfo();
	void EnableAllPushButtons(BOOL bEnable);

	void HideAllAbilityButton();
	void HideAllGodLevelPic(bool bHide);

	void SetGodLevel(int row,int num);
	int GetCurrTotalPage(){return m_nCurrTotalPage;}

	void BuildGodDomains();
	DWORD OnNS_GetRoleInitState_Area(tagNS_GetRoleInitState_Area* pNetCmd,DWORD);
	DWORD OnNS_AddArea(tagNS_AddArea* pNetCmd,DWORD);
	DWORD OnNS_ActivationArea(tagNS_ActivationArea* pNetCmd,DWORD);
	DWORD OnNS_LoseArea(tagNS_RareAreaInfo* pNetCmd,DWORD);
	DWORD OnNS_PromotionClergy(tagNS_PromotionClergy* pCmd, DWORD);
	DWORD OnNS_ChangeClergy(tagNS_ChangeClergy* pCmd, DWORD);


	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent);
	DWORD OnOpenEvent(tagGameEvent* pEvent);
	DWORD OnCloseEvent(tagGameEvent* pEvent);
	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	int GetCurAcitveAbilityNum(int& nFreeNum, int& nRateNum);
	void RefReshDesc(EDomain eDomain);
private:
	TSFPTrunk<DomainAbilityFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	vector<DWORD> m_vecGodDomain;	// ��ְ����
	multimap<int,DomianData>	m_mmapFreePickDomain;	// ��ѡ���� <level,data>
	vector<DomianData>	m_vecRareDomain;	// ϡ������
	GUIWnd*			m_pWnd;
	
	GUIPushButton*	m_pPbnGod;
	GUIPushButton*	m_pPbnFreePick;
	GUIPushButton*	m_pPbnRare;
	GUIPushButton*	m_pPbnActive;

	GUIButton*	m_pBnPer;
	GUIButton*	m_pBnNext;
	GUIStatic*	m_pStPage;

	EDomain			m_eCurrDomain;
	int					m_nCurrPage;
	int					m_nCurrTotalPage;
	GodLevelPic		m_pStGodLevel[DOMAIN_ROW_NUM];
	GUIStatic*			m_pStPicSide;	// ����
	tagAbilityButton m_abilityButton[DOMAIN_ROW_NUM][DOMAIN_COLUNM_NUM];

	WordDesc	m_sDesc[E_WordNum];
	GUIStatic*	m_pStXinYang;	// ����
	GUIProgress*	m_pStAgglomerate;	//	���۶�
	GUIStatic*	m_pStAgglomerateText;	// ���۶�
	GUIStatic*	m_pGodLevelPic[MAX_GOD_LEVEL];

	IconStatic*  m_pCurrGodJobPic;	// ��ǰ��ְ
	GUIButton*	m_pBnClose;
	DWORD		m_dwPushingTime;
	bool				m_bStartCount;
	GUIStaticEx*	m_pStexDesc;

	bool m_bOpened;
	DWORD m_dwUpdateTime;
};

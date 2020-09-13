#pragma once
#include "gameframe.h"
#include "..\WorldDefine\clergy_define.h"

#define Campaign_Num 4

enum
{
	EGT_Wars =0,
	EGT_Suns,
	EGT_Manas,
	EGT_Peaces,
	EGT_Num
};

struct tagNS_Campaign;
struct tagNS_CampaignInfo;

struct CampaignPatch 
{
	GUIStatic*		pPic;		// ͼƬ
	GUIStaticEx*	pStMale;		// ���Ե�һ(�Ƿ�����Ҫ��)
	GUIStaticEx*	pStFemale;	// Ů�Ե�һ(�Ƿ�����Ҫ��)
	GUIStaticEx*	pStCommon;	// ��ͨ(�Ƿ�����Ҫ��)

	GUIStatic*		pStCampaignNum;	// ��������
	GUIStatic*		pStStrength;	// ʵ��

	GUIPatch*		pStSelected;	// ѡ�п�

	// for dame select�� i hate this
	GUIPatch*		pPBack;
	GUIStatic*		pStTextName;
	GUIStatic*		pStTextMale1;
	GUIStatic*		pStTextMale2;
	GUIStatic*		pStTextFemale1;
	GUIStatic*		pStTextFemale2;
	GUIStatic*		pStTextCommon1;
	GUIStatic*		pStTextCommon2;
};

class CampaignFrame :
	public GameFrame
{
public:
	CampaignFrame(void);
	~CampaignFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	VOID Update();
	void ShowFrameMsg();
private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD CalculateStrangth(int nType);
	DWORD OnNS_Campaign(tagNS_Campaign* pNetCmd,DWORD);
	DWORD OnNS_CampaignInfo(tagNS_CampaignInfo* pNetCmd,DWORD);
	DWORD OnOpenCampaingResultFrame(tagGameEvent* pEvent);
	
	void RefreshSatisfyUI();
private:
	TSFPTrunk<CampaignFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*			m_pWnd;
	GUIButton*		m_pBnClose;
	GUIButton*		m_pBnCampaign;
	GUIButton*		m_pBnShowLastResult;
	GUIStatic*		m_pBnTitle;
	
	ECampaignGodType m_eCurrType;
	CampaignPatch m_CampaignGods[EGT_Num];

	DWORD												m_dwNPCID;				// NPC��ID
	DWORD												m_escapeTime;
};

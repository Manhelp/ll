#pragma once
#include "gameframe.h"
#include "EggLayer.h"
#include "EggData.h"

extern const TCHAR* szBackPic;

#define MAX_LAYER 7
struct tagNS_EggBegin;
struct tagNS_BreakEgg;
struct tagNS_TakeEggTreasure;

class IconStatic;
class ItemButton;
struct tagNS_ItemAdd;
struct tagNS_NewItemAdd;

// ע���һ����û�вʵ����ӵ�
struct tagHammer
{
	GUIStatic*	m_pStHammer;				// ��ͨ����
	GUIStatic*	m_pStHammer_colour;	// �ʵ�����
};

struct tagGoods
{
	DWORD	dwItemID;
	ItemButton*	m_pIsGoods;
	IconStatic*	m_pChoosePic;

	tagGoods():dwItemID(0){}

	void clear();
	
	// ѡ����
	void	Choose(bool b);
	// ˢ��UI
	void RefreshGoods(DWORD dwID,BYTE bQuality);
};

class EggBrokerFrame :
	public GameFrame
{
public:
	EggBrokerFrame(void);
	~EggBrokerFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	BOOL EventHandler(tagGUIEvent* pEvent);

	// �ܽ������
	VOID OpenFrame();
	VOID CloseFrame();
	BOOL IsInVisable(){return m_pWnd->IsInvisible();}

	// ȡ��
	VOID TakeTreasure();
	VOID ClearAllGoods();

	// ����
	VOID EnableLayer(EEggLayerNum layer);
	tstring GetLayerTips(EEggLayerNum layer);

	// ����UI
	VOID RefreshCurrHammers();
	VOID GenerateHammerNums();

	// easybuy
	VOID FillEasyBuyInfo();
private:
	// netcmd
	DWORD OnNS_EggBegin(tagNS_EggBegin* pNetCmd,DWORD);
	DWORD OnNS_BreakEgg(tagNS_BreakEgg* pNetCmd,DWORD);
	DWORD OnNS_TakeEggTreasure(tagNS_TakeEggTreasure* pNetCmd,DWORD);
	// ����һ����֪��Ʒ
	DWORD NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD);
	// ����һ����Ʒ
	DWORD NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD);

	// event
	DWORD OnMsgBox_EggBroker_Check(tagMsgBoxEvent* pGameEvent);
	DWORD OnMsgBox_EggBroker_EmptyEgg(tagMsgBoxEvent* pGameEvent);
private:
	TSFPTrunk<EggBrokerFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*		m_pWnd;
	GUIButton*		m_pBnClose;
	GUIButton*		m_pBnTakeTreasure;
	GUIStatic*		m_pStCurrHammers;

	EggLayer			m_Layers[MAX_LAYER];
	tagHammer		m_Hammers[MAX_LAYER];
	tagGoods		m_Goods[MAX_LAYER];
	
	int					m_nCurrEnableLayer;
	int					m_nCurrTreasureLayer;	// ��ǰ�����ѡ�����
	EggData			m_LayerData;
	bool					m_bCanSendMsg;

	GUIWnd*			m_pEasyBuyWnd;		// ��ݹ���
	ItemButton*		m_pItHammer;
	GUIStatic*		m_pStPrice;
	GUIStatic*		m_pStLeft;
	GUIButton*		m_pBnClose_2;
	GUIButton*		m_pBnBuy;

	
};

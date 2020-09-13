#pragma once
#include "gameframe.h"
#include "..\WorldDefine\godbid_define.h"

struct tagNS_BlackMarketShopOpen;
struct tagNS_BlackConPrice;
class IconStatic;

enum ECurrentBidMode
{
	ECBM_Gold,			// ��Ҿ���
	ECBM_Shengbi		// ʥ�Ҿ���
};

class BlackMarketFrame :
	public GameFrame
{
public:
	
	BlackMarketFrame(void);
	~BlackMarketFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();

	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	VOID Update();
	VOID ComformPrice();
	VOID RefreshMoney();
	VOID RefreshUI();
	VOID RsfreshPicBack(BYTE byQuality);

	DWORD GetCurStartPrice();

	VOID ClearMoney();
private:
	DWORD OnNS_BlackMarketShopOpen(tagNS_BlackMarketShopOpen* pNetCmd,DWORD);
	DWORD OnNS_BlackConPrice(tagNS_BlackConPrice* pNetCmd,DWORD);
	BOOL EventHandler(tagGUIEvent* pEvent);
private:
	TSFPTrunk<BlackMarketFrame>		m_Trunk;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<GUISystem>						m_pGUI;

	DWORD												m_dwNPCID;				// NPC��ID
	DWORD												m_escapeTime;
	DWORD												m_dwLeftTime;			// ʣ��ʱ��

	DWORD												m_nFinalPrice;				// ���ճ���
	DWORD												m_nAddPrice;				// �Ӽ�
	DWORD												m_dwCurMinOffer;		// ��ǰ����ͳ���
	int														m_nRefreshLimitTime;// ˢ�°�ť���ʱ���ʱ��

	tagBlackMarketItemInfo						m_tagGoldItem;			// ���������Ʒ
	tagBlackMarketItemInfo						m_tagShengbiItem;		// ʥ��������Ʒ
	
	ECurrentBidMode								m_eMode;					// ����ģʽ
	DWORD												m_nPreparePrice;		// ׼������

	GUIWnd*												m_pWnd;
	GUIPushButton*									m_pPbGold;				// ��Ϸ��
	GUIPushButton*									m_pPbShengbi;			// ʥ��

	IconStatic*	m_pIcnItemPic;						// ��Ʒͼ
	IconStatic*	m_pIcnItemPicBack;						// ��Ʒͼ��ͼ
	IconStatic*	m_pIcnGoldPic1;
	IconStatic*	m_pIcnGoldPic2;
	IconStatic*	m_pIcnGoldPic3;
	GUIStaticEx*	m_pStItemName;					// ��Ʒ����
	GUIStatic*	m_pStCurPrice;						// ԭʼ�׼�
	GUIStatic*	m_pStMinOfferPrice;				// ��ͳ���

	GUIStaticEx*	m_pStFinalPrice;						// ���ճ���

	GUIStatic*	m_pStMoneyNotEnoughPic;	// �ʽ������

	GUIStatic*	m_pStTime_Minite;					// ʣ��ʱ�� ��
	GUIStatic*	m_pStTime_Second;				// ʣ��ʱ�� ��

	GUIButton*	m_pBnPriceAddx1;					// 1������
	GUIButton*	m_pBnPriceAddx10;				// 10������
	GUIButton*	m_pBnPriceAddx100;				// 100������
	GUIButton*	m_pBnPriceAdd_All;				// ����

	GUIButton*	m_pBnClearPrice;					// ����Ӽ�
	GUIButton*	m_pBnConformPrice;				// ȷ�ϳ���
	GUIButton*	m_pBnRefresh;						// ˢ��

	GUIButton*	m_pBnClose;
};

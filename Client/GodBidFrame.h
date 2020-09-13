#pragma once
#include "gameframe.h"
#include "..\WorldDefine\godbid_define.h"
struct tagNS_GodBiderShopOpen;
struct tagNS_ConformPrice;

class ItemButton;
class IconStatic;
struct tagBidItem
{
	GUIStatic*	pIcnBack;					// ����
	IconStatic*	pIcnItemPic;			// ��Ʒͼ
	IconStatic*	pIcnItemPicBack;			// ��Ʒͼ��ͼ
	GUIStatic*	pIcnHighestPic;		// ��߳���
	GUIStatic*	pStUnOpen;				// δ��ʱ
	GUIStatic*	pStOpen;					// ��ʼ��
	GUIStatic*	pStItemName;			// ��Ʒ����
	GUIStatic*	pStCurPrice;				// ��ǰ�۸�
	GUIStatic*	pStMinOfferPrice;	// ��ͳ���
	GUIStatic*	pStChooseing;			// ѡ�п�
};

class GodBidFrame :
	public GameFrame
{
public:
	GodBidFrame(void);
	~GodBidFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	VOID Update();
private:
	DWORD OnNS_GodBiderShopOpen(tagNS_GodBiderShopOpen* pNetCmd,DWORD);
	DWORD OnNS_ConformPrice(tagNS_ConformPrice* pNetCmd,DWORD);
	VOID ComformPrice();
	BOOL EventHandler(tagGUIEvent* pEvent);

	VOID RefreshUI(INT nRow);
	VOID RefreshMoney();
	VOID RefreshPicBack(IconStatic* pWnd,BYTE byQuality);

	VOID ClearMoney();
	DWORD GetCurrStartPrice();
	bool IsCurrItemPriceOffered();
private:
	TSFPTrunk<GodBidFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	DWORD										m_dwNPCID;				// NPC��ID
	DWORD										m_escapeTime;

	map<int,tagGodBidItemInfo>	m_mapBidItemInfo;
	DWORD										m_dwLeftTime;			// ʣ��ʱ��
	int												m_nCurrRow;				// �ڼ��е���Ʒ
	DWORD										m_nFinalPrice;				// ���ճ��ۣ��� ��λ��
	DWORD										m_nAddPrice;				// �Ӽۣ��� ��λ��
	DWORD										m_dwCurPrice;				// ��ǰ����Ѿ����ļ۸񣨽� ��λ��
	DWORD										m_dwCurMinOffer;		// ���ڵ���ͳ��ۣ��� ��λ��
	int												m_nRefreshLimitTime;// ˢ�°�ť���ʱ���ʱ��

	tagBidItem									m_BidItems[MAX_GODBID_ITEM_NUM];
	GUIWnd*		m_pWnd;
	GUIStatic*	m_pStAlreadyOfferPrice;			//	�ѳ���
	GUIStaticEx*	m_pStAddMoney;					// �������Ӽ۸�
	GUIStaticEx*	m_pStFinalPrice;						// ���ճ���

	GUIStatic*	m_pStMoneyNotEnoughPic;	// �ʽ������
	GUIStatic*	m_pStHighestPricePic;			// ��߳��۽���

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

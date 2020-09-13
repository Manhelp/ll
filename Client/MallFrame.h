#pragma once
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"

/** class MallFrame
	brief �̳�
*/
class NetSession;
class IconStatic;
class PlayerTryDressWnd;
class ItemButton;
class MallButton;
class GlintPushButton;

struct tagMsgMallGiftEvent;
struct tagRoleYuanbaoChange;
struct tagRoleSilverChange;
struct tagExchangeVolumeChange;
struct tagMarkChange;
class HttpPicEx;
#define MALL_MAXITEM_PERPAGE 10 //18
#define MALL_ITEM_STATE 4//5		//��Ʒ״̬
#define MALL_PROMOTE_ITEM_STATE 2		//�Ƽ���Ʒ״̬
#define MALL_MAX_PROMOTE_ITEM 5
#define MALL_MAX_PROMOTE_ITEM_PerPage 5
#define MALL_ITEM_SUBTYPE	8
#define MALL_ITEM_SECOND_SUBTYPE 5
#define SPECIAL_TYPE 2
#define MALL_MAXPACK_PERPAGE 5 //7
#define MALL_PACKPRO_NUM 5			//��������Ʒ��
#define MALL_PACKPRO_STATE 4		//���״̬
#define MALL_DAY_PART 6
#define Mall_ShopingCart_Num 6		//���ﳵ����Ʒ����
#define Mall_ShopingCart_RewardNum 4 //������Ʒ����

struct tagUpdateMallDataEvent : public tagGameEvent
{

	tagUpdateMallDataEvent():tagGameEvent(_T("tagUpdateMallDataEvent"),NULL){}
	tagUpdateMallDataEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

class MallFrame : public GameFrame
{
public:
	enum EMallItemType 
	{
		EMIT_NULL		= -3,	//��
		EMIT_All		= -2,
		EMIT_Package	= -1,
		EMIT_0			=  0, //��һ��һ���˵�
		EMIT_1			=  1,
		EMIT_2			=  2,
		EMIT_3			=  3,
		EMIT_4			=  4,
		EMIT_5			=  5,
		EMIT_Volume		=  6,
		EMIT_Mark		=  7,
		EMIT_END		=  8,  //����

		EMIT_GiftPlan		=  10, //1ԭ�ƻ�
	};

	enum EMallItemStateType			//��Ʒ״̬ͼ��
	{
		EMIST_New		= 0,		//��Ʒ
		EMIST_Hot		= 1,		//����
		EMIST_Promote	= 2,		//����
		EMIST_Num		= 3,		//����
	  //EMIST_Present	= 5,		//����
	  //EMIST_Exchange	= 6,		//�һ�
	  //EMIST_GroupBuy	= 7,		//�Ź�
	};

	enum EMallIPromoteItemStateType	 //�Ƽ���Ʒ״̬ͼ��
	{
		EMPIST_New		= 0,		//��Ʒ
		EMPIST_Num		= 1,		//����
	};

	enum EMamllItemTipsType
	{
		EMITT_All		= 0,
		EMITT_Package,
		EMITT_Promote,
	};

	enum EMallItemOperateState
	{
		EMIOS_NULL				= 0,
		EMIOS_NormalItem,
		EMIOS_PackItem,
		EMIOS_Promote,
		EMIOS_Free,
	};

public:
	MallFrame(void);
	~MallFrame(void);

    tstring GetGUIFullName()
    {
        if(P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else 
            return _T("");
    };

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	virtual BOOL EscCancel();

	// �Ǵ����Ʒ����
	void ClassifiedMallItem();

	void ShowWnd(BOOL bVal);

	//����avatar
	void UpdateAvatar();

	//���������Ʒ
	DWORD GetMallItemHopeTypeID() { return m_dwSelectItemID; }
	EMallItemOperateState GetOpereateState() { return m_eOperateState; }

public:
	//����Ҫ�������ƷID
	void SetHopeBuyItemID(DWORD dwID) 
	{ 
		m_dwSelectItemID = dwID; 
	}

	//���õ�ǰѡ������,���ⲿ���ã����������棬��ݹ���
	void SetCurMenu( EMallItemType eMallItemType );

	//������Ʒ
	void BuyMallItem();

	//1Ԫ�ƻ� ��˸����
	VOID SetGiftPlan();
private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);

	//ˢ����Ʒѡ���
	void RefreshMallItemFlag();

	//ˢ���Ƽ���Ʒ
	void RefreshPromoteItemState();

	//��ʾ�Ƽ���Ʒ
	VOID ShowPromoteMallItem();

	//ˢ�������ȡ��Ʒ
	void RefreshFreeItem();

	//��ʾ��ǰҳ����Ʒ(�����)
	void ShowCurPageMallItemByType(INT nPage);

	//��ʾ��ǰҳ����Ʒ(ȫ��)
	void ShowCurPageMallItemByAll(INT nPage);

	//��ʾ��ǰҳ�����
	void ShowCurPageMallPack(INT nPage);

	//�ж���ʾ��Ʒ
	void JudgeShowMallItem(INT nMenu, INT nSubMenu);

	//���ö������ఴť��ʾ
	void SetSubMenuText(INT nMenu);

	//��õ�ǰ���ID���������������
	DWORD GetCurPackID(INT index);
	
	//--��ҳ
	void PageUp();
	void PageDown();

	//��ʾ��ǰҳ��
	void SetCurrentPage();

	// ���ý�Ǯ
	void SetMoney( INT64 n64Value );
	//����Ԫ��
	void SetYuanBao();
	void SetExchangeVolume();
	void SetMark();

	//�һ���Ʒ
	void ExchangeMallItem();

	void BuyMallItemPromote();

	//��ȡ�����Ʒ
	void FreeGetMallItem();

	//����û����Ʒ����ͨ��Ʒ�ؼ�
	void HideCtrlMallItem();
	void HideCtrlPromoteItem();
	void HideCtrlMallPack();
	int TipsPos2State( const tagMallItemProtoBase* pItem,  int nStatePos );
	//--������Ϸ�¼�
	DWORD OnBuyMallItemInputEvent(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnBuyMallPromoteItemInputEvet(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnAffirmBuyMallItemEvent(tagMsgBoxEvent* pGameEvent);
	DWORD OnMsgMallGiftEvent(tagMsgMallGiftEvent* pGameEvent);
	DWORD OnBagYuanbaoChange(tagRoleYuanbaoChange* pGameEvent);
	DWORD OnBagMoneyChange(tagRoleSilverChange* pGameEvent);
	DWORD OnExchangeVolumeChange(tagExchangeVolumeChange* pGameEvent);
	DWORD OnMarkChange(tagMarkChange* pGameEvent);
	DWORD OnExchangePackItem(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnExchangeMallItem(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnAffirmExchangePackItem(tagMsgBoxEvent* pGameEvent);
	DWORD OnAffirmExchangeMallItem(tagMsgBoxEvent* pGameEvent);
	DWORD OnNS_MallUpdate(tagUpdateMallDataEvent* pGameEvent);
	DWORD OnAddToShopingCart(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnConfirmShopingCart_Event(tagMsgBoxEvent* pGameEvent);
	//--����������Ϣ

	//--����������Ϣ
	VOID OnSendBuyMallItem(const tagMallItemProto* pItem, INT16 n16Num); 
	VOID OnSendBuyMallPack(const tagMallPackProto* pPack, INT16 n16Num);

	//--����������Ϣ
	DWORD OnNS_MallBuyItem(tagNS_MallBuyItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallFreeGetItem(tagNS_MallFreeGetItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallPresentItem(tagNS_MallPresentItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallBuyPack(tagNS_MallBuyPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallPresentPack(tagNS_MallPresentPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallItemExchange(tagNS_MallItemExchange* pMsg, DWORD dwParam);
	DWORD OnNS_MallPackExchange(tagNS_MallPackExchange* pMsg, DWORD dwParam);

	DWORD OnNS_MallUpdateCfg(tagNS_MallUpdateCfg* pMsg, DWORD dwParam);
	DWORD OnNS_MallShoppingCart(tagNS_MallShoppingCart* pmsg, DWORD dwParam);
	
	//--��ʾ��ƷTips
	void ShowMallGoodsTips(int nGoodsPos, int nStatePos);
	void ShowMallPromoteGoodsTips(int nGoodsPos, int nStatePos);
	void ShowMallPackTips(int nGoodsPos, int nStatePos);

	//--������Tips
	void ShowPromoteProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowGroupBuyProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowPresentProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowNumProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowExchangeProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);

	//--�������ж�
	bool JudgePromotePro(const tagMallItemProtoBase* pItem);
	bool JudgeGroupBuyPro(const tagMallItemProtoBase* pItem);
	bool JudgePresentPro(const tagMallItemProtoBase* pItem);
	bool JudgeNumPro(const tagMallItemProtoBase* pItem);
	bool JudgeExchangePro(const tagMallItemProtoBase* pItem);

	// �Դ�
	void JudgeTryDress(DWORD dwTypeID);

	// ��������������
	void SetPresentMaxNum();

	void SetOperateState(EMallItemOperateState eType=EMIOS_NULL);	
	
	//ֱ��
	void YuanBaoAbundance();

	//������
	void ShowMallNotice(BOOL bVal);

	// �Դ�ʱװ��ɫ
	int HandleFation( EAvatarElement eAvat, bool bCanDye, DWORD dwTypeID );

	// һ����ҳ�İ�ť״̬
	VOID PopUpAll();
	//-------------------------------------------------------------------------------------------
	//Ԫ������
	//-------------------------------------------------------------------------------------------

	//ˢ�¹��ﳵ��Ʒ
	VOID RefreshShopingCart();
	//ˢ�½�����Ʒ
	VOID RefreshShopingReward();
	//��ʾ���ع��ﳵ��Ʒ
	VOID HideShopingCart(bool bHide = TRUE);
	//��ʾ���ؽ�����Ʒ
	VOID HideShopingReward(bool bHide = TRUE);
	//��ʾ���ؼ��빺�ﳵ��ť
	VOID HideAddToShopingCart(bool bHide = TRUE);
	//��ӵ����ﳵ
	VOID AddToShopingCart(int nNum);
	//��չ��ﳵ
	VOID ClearShopingCart();
private:

	TSFPTrunk<MallFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;		
	PlayerTryDressWnd*			m_pWndTryDress;							// �Դ�����
	GUIStatic*					m_pStcGold;								// ���
	GUIStatic*					m_pStcSilver;							// ����
	GUIStatic*					m_pStcYuanBao;							// Ԫ��
	GUIStatic*					m_pExchangeVolume;						// �һ��� 
	GUIStatic*					m_pStcMark;								// ����
	GUIButton*					m_pBtnClose;							// �ر�
	GUIPushButton*				m_pPBtnYuanBaoTrade;					// Ԫ������
	//GUIPushButton*				m_pPBtnGroupBuy;						// �����Ź�
	//GUIPushButton*				m_pPBtnNotice;							// �̳ǹ���
	GUIButton*					m_pBtnYuanBaoAbundance;					// Ԫ����ֵ
	GUIButton*					m_pBtnReset;							// �Դ�����

	IconStatic*					m_pStcFetchBack;						// �����ȡ����
	IconStatic*					m_pStcFetchSel;							// �����ȡѡ���
	ItemButton*					m_pFetchUnitItem;						// �����ȡ��Ʒ
	GUIButton*					m_pBtnFetch;							// �����ȡ��ť	
	GUIStatic*					m_pBtnFetchTitle;						// �����ȡ��Ʒ��

	GUIStatic*					m_pStcGoodsBack;						// ����������
	GUIStatic*					m_pStcGoodsBackEx;						// ��ͨ��Ʒ
	IconStatic*					m_pUnitItemBack[MALL_MAXITEM_PERPAGE];	// ��Ʒ��
	IconStatic*					m_pUnitItemSel[MALL_MAXITEM_PERPAGE];	// ��Ʒѡ���
	MallButton*					m_pUnitItem[MALL_MAXITEM_PERPAGE];		// ��Ʒ
	GUIStatic*					m_pStcItemName[MALL_MAXITEM_PERPAGE];	// ��Ʒ����
	GUIStatic*					m_pStcInfo[MALL_MAXITEM_PERPAGE];		// ��Ʒ��Ϣ

	GUIStatic*					m_pStcGoodsRest[MALL_MAXITEM_PERPAGE];   // ��Ʒʣ������
	GUIStatic*					m_pStcGoodsInfo[MALL_MAXITEM_PERPAGE];	 // ��Ʒԭ������
	IconStatic*					m_pStcGoodsPrice[MALL_MAXITEM_PERPAGE];  // ��Ʒԭ��ֵ��ʾ
	IconStatic*					m_pStcGoodsPriceDel[MALL_MAXITEM_PERPAGE];	// ��Ʒԭ��ֵ��ʾɾ����
	GUIStatic*					m_pStcGoodsInfoNow[MALL_MAXITEM_PERPAGE];	// ��Ʒ�ּ�����
	GUIStatic*					m_pStcGoodsPriceNow[MALL_MAXITEM_PERPAGE];  // ��Ʒ�ּ���ʾ
	GUIStatic*					m_pStcGoodsGift[MALL_MAXITEM_PERPAGE];	   // ��Ʒ��Ʒ����
	GUIStatic*					m_pStcGoodsGiftNum[MALL_MAXITEM_PERPAGE];  // ��Ʒ��Ʒ������ʾ

	IconStatic*					m_pStcState[MALL_MAXITEM_PERPAGE][MALL_ITEM_STATE];// ��Ʒ״̬
	GUIStatic*					m_pStcStateInv[MALL_MAXITEM_PERPAGE][MALL_ITEM_STATE];// ��Ʒ״̬��ͼ
	GUIButton*					m_pBtnPDItem;							// ��һҳ(��ͨ��Ʒ)
	GUIButton*					m_pBtnPUItem;							// ��һҳ(��ͨ��Ʒ)
	GUIStatic*					m_pStcCPItem;							// ��ǰҳ��ʾ(��ͨ��Ʒ)
	GUIButton*					m_pBtnPDPack;							// ��һҳ(�����Ʒ)
	GUIButton*					m_pBtnPUPack;							// ��һҳ(�����Ʒ)
	GUIStatic*					m_pBtnCPPack;							// ��ǰҳ��ʾ(�����Ʒ)

	GUIButton*					m_pBtnBuy;								// ����
	GUIButton*					m_pBtnPresent;							// ����
	//GUIButton*					m_pBtnGroupBuy;							// �Ź�
	//GUIButton*					m_pBtnExchange;							// �һ�

	GUIStatic*					m_pStcProBack;							// �Ƽ���Ʒ����

	GUIStatic*					m_pStcProRest[MALL_MAX_PROMOTE_ITEM_PerPage];   // �Ƽ���Ʒʣ������
	GUIStatic*					m_pStcProInfo[MALL_MAX_PROMOTE_ITEM_PerPage];	 // �Ƽ���Ʒԭ������
	IconStatic*					m_pStcProPrice[MALL_MAX_PROMOTE_ITEM_PerPage];  // �Ƽ���Ʒԭ��ֵ��ʾ
	IconStatic*					m_pStcProPriceDel[MALL_MAX_PROMOTE_ITEM_PerPage];		// �Ƽ���Ʒԭ��ֵ��ʾɾ����
	GUIStatic*					m_pStcProInfoNow[MALL_MAX_PROMOTE_ITEM_PerPage];	// �Ƽ���Ʒ�ּ�����
	GUIStatic*					m_pStcProPriceNow[MALL_MAX_PROMOTE_ITEM_PerPage];  // �Ƽ���Ʒ�ּ���ʾ
	GUIStatic*					m_pStcProGift[MALL_MAX_PROMOTE_ITEM_PerPage];	   // �Ƽ���Ʒ��Ʒ����
	GUIStatic*					m_pStcProGiftNum[MALL_MAX_PROMOTE_ITEM_PerPage];  // �Ƽ���Ʒ��Ʒ������ʾ

	GUIStatic*					m_pStcProPageText;						// �Ƽ���Ʒҳ����ʾ	
	GUIButton*					m_pBtnFormerPage;						// �Ƽ���Ʒ��ǰ��ҳ
	GUIButton*					m_pBtnLatterPage;						// �Ƽ���Ʒ���ҳ
	IconStatic*					m_pProItemBack[MALL_MAX_PROMOTE_ITEM_PerPage];
	IconStatic*					m_pUnitProItemSel[MALL_MAX_PROMOTE_ITEM_PerPage];
	MallButton*					m_pUnitProItem[MALL_MAX_PROMOTE_ITEM_PerPage];
	IconStatic*					m_pProItemState[MALL_MAX_PROMOTE_ITEM_PerPage][MALL_PROMOTE_ITEM_STATE];// �Ƽ���Ʒ״̬
	GUIStatic*					m_pProItemStateInv[MALL_MAX_PROMOTE_ITEM_PerPage][MALL_PROMOTE_ITEM_STATE];// �Ƽ���Ʒ״̬ǰ��
	GUIStatic*					m_pProItemName[MALL_MAX_PROMOTE_ITEM_PerPage]; 
	//GUIStatic*					m_pProItmeInfo[MALL_MAX_PROMOTE_ITEM];

	GUIStatic*					m_pStcPackBack;							//�������

	IconStatic*					m_pUnitPackBack[MALL_MAXPACK_PERPAGE];	//ÿ�����
	GUIButton*					m_pBtnPackBack[MALL_MAXPACK_PERPAGE];	//�������button
	IconStatic*					m_pUnitPackSel[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_NUM];	//���ѡ���
	MallButton*					m_pUnitPack[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_NUM];	//��������Ʒ
	IconStatic*					m_pStcPackName[MALL_MAXPACK_PERPAGE];	// �������

	GUIStatic*					m_pStcPackRest[MALL_MAXPACK_PERPAGE];  // ���ʣ������
	GUIStatic*					m_pStcPackInfo[MALL_MAXPACK_PERPAGE];	// ���ԭ������
	IconStatic*					m_pStcPackPrice[MALL_MAXPACK_PERPAGE];  // ���ԭ��ֵ��ʾ
	IconStatic*					m_pStcPackPriceDel[MALL_MAXPACK_PERPAGE];	// ���ԭ��ֵ��ʾɾ����
	GUIStatic*					m_pStcPackInfoNow[MALL_MAXPACK_PERPAGE];	// ����ּ�����
	GUIStatic*					m_pStcPackPriceNow[MALL_MAXPACK_PERPAGE];  // ����ּ���ʾ
	GUIStatic*					m_pStcPackGift[MALL_MAXPACK_PERPAGE];	  // �����Ʒ����
	GUIStatic*					m_pStcPackGiftNum[MALL_MAXPACK_PERPAGE];  // �����Ʒ������ʾ

	IconStatic*					m_pStcPackState[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_STATE];// ���״̬
	GUIStatic*					m_pStcPackStateInv[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_STATE];// ���״̬ǰ��
	GUIStatic*					m_pStcPackNew[MALL_MAXPACK_PERPAGE];	// ���������ʾ
	GUIStatic*					m_pStcPackHot[MALL_MAXPACK_PERPAGE];	// ���������ʾ

	// ʱװԤ��Ⱦɫ
	GUIComboBox*				m_pCBDayFation[MALL_DAY_PART];
	GUIStatic*					m_pStcDyeTips[MALL_DAY_PART];

	GUIPushButton*				m_pPBtnAll;								// ȫ����Ʒ��ť
	GUIPushButton*				m_pPBtnPackage;							// �����Ʒ��ť
	GUIPushButton*				m_pPBtnSubMall[MALL_ITEM_SUBTYPE-2];		// ��Ʒ���ఴť һ������
	GUIPushButton*				m_pPBtnPresent;							// ������ఴť
	GUIPushButton*				m_pPBtnMark;							// ���ַ��ఴť
	GUIPushButton*				m_pPBtnMallType2[MALL_ITEM_SECOND_SUBTYPE];	// ��Ʒ��������
	GUIPushButton*				m_pPBtnMall;							// ˢ����Ʒ����ť

	//GUIStatic*					m_pWndNotice;							// ����
	//HttpPicEx*					m_pHttp_MallNotice;				

	GUIWnd*						m_pWndShowBox;




	TMap<DWORD, tagMallItemProto*>	m_mapMallItem[MALL_ITEM_SUBTYPE][MALL_ITEM_SECOND_SUBTYPE];	// ��Ʒ�����б�
	map<pair<DWORD, DWORD>, int>	m_map1and2toBtIndex;	// ͨ��һ������Ͷ�������ȡ��ť���
	map<pair<DWORD, int>, DWORD>	m_map1andBtIndexTo2;	// ͨ��һ������Ͱ�ť���ȡ��������

	INT							m_nCurMenu;								// ��ǰ��Ʒ�������:0-7 ȫ��:-2 ���:-1
	INT							m_nCurSubMenu;							// ��ǰ��Ʒ�������ܷ���:0-4
	INT							m_nCurPage[MALL_ITEM_SUBTYPE][MALL_ITEM_SECOND_SUBTYPE];			// ��ǰҳ
	INT							m_nCurMaxPage[MALL_ITEM_SUBTYPE][MALL_ITEM_SECOND_SUBTYPE];		// ��ǰ�������ҳ

	INT							m_nSpecialPage[SPECIAL_TYPE];			// ȫ��:0 ���:1
	INT							m_nSpecialMaxPage[SPECIAL_TYPE];

	DWORD						m_dwSelectItemID;						// Ҫ�������ƷID
	INT16						m_n16BuyNum;							// ���������
	vector<tagMsgMallGiftEvent>	m_vecGiftEvent;							// ��������
	bool						m_bSelectPromote;
	EMallItemOperateState		m_eOperateState;
	tstring						m_strMallNotice;						// ������̳ǹ���

	//-------------------------------------------------------------------------------------------
	//Ԫ������
	//-------------------------------------------------------------------------------------------
    GUIStatic*					m_pStcYuanbaoBack;						// Ԫ�����ױ���

	//-------------------------------------------------------------------------------------------
	//�Ź�
	//-------------------------------------------------------------------------------------------
	//GUIStatic*					m_pStcGroupBuyBack;						// �Ź�����

	DWORD						m_dwTryDressID[MALL_DAY_PART];			// �Դ�ʱװID

	INT							m_nPolicy;//��½ģʽ

	INT							m_iPromoteItemShowPage;					//�Ƽ���Ʒ�ڵڼ�ҳ

	EShoppingDistrict			m_eShopingDistrict;        // ��������

	GUIPushButton				*m_pPBLevelLimit;			//�����ȼ�����
	BOOL						m_bLevelLimit;
	EMallItemType				m_eClickType;				//����˵�����

	INT8						m_n8SecondMenu;				//��¼������ҳҳ��
	
	//�Ƿ�Ϊ�ⲿ���ã����������棬��ݹ���
	BOOL						m_bIsExterior;  

	//1Ԫ�ƻ�
	GlintPushButton*			m_pGPBLevel[LEVEL_NUM_CHOOSE];


	//���ﳵ
	GUIButton*					m_pBtnShopingCart[MALL_MAXITEM_PERPAGE];					//���ﳵ��ť
	IconStatic*					m_pIStcShopingCartItem[Mall_ShopingCart_Num];				//���ﳵ����Ʒ
	IconStatic*					m_pIStcShopingCartReward[Mall_ShopingCart_RewardNum];		//������Ʒ
	GUIButton*					m_pBtnCheckout;												//���˰�ť
	GUIButton*					m_pBtnClearSHopingCart;										//��չ��ﳵ��ť
	GUIStatic*					m_pStcYuanBaoCast;											//Ԫ��������
	GUIStatic*					m_pStcRewardNum;											//����������ʤ���ܼ�ֵ/��ȡ���������ʤ������
	GUIButton*					m_pBtnItemAdd[Mall_ShopingCart_Num];
	GUIButton*					m_pBtnItemRed[Mall_ShopingCart_Num];
	GUIStaticEx*				m_pStcShopingTips;
	GUIWnd*						m_pWndShopingText;											//���ﳵ�Ǽ����֣��������û��ҩ���ص�
	GUIWnd*						m_pWndShopingCatrWnd;										//�������û��Ҫ���ص�

	const tagMallItemProto*		m_pShopingItem[Mall_ShopingCart_Num];						//���ﳵ�ڵ��̳���Ʒ
	INT							m_nShopingIetmNum[Mall_ShopingCart_Num];					//��Ʒ����
	DWORD						m_dwAddItemId;
	DWORD						m_dwMallCfg[EMCA_End];
};
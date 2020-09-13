#pragma once
#include "gameframe.h"
#include "MallFrame.h"
#include "CurrencyMgr.h"
#include "YuanbaoTradeMgr.h"

const INT YB_SELL_LIST_COL      =   5;  //!< Ԫ�������б�����
const INT YB_BUY_LIST_COL       =   5;  //!< Ԫ���չ��б�����
const INT YB_ORDER_LIST_COL     =   11; //!< Ԫ�������б�����

struct tagRoleSilverChange;
struct tagRoleYuanbaoChange;

//! \class  YuanbaoFrame
//! \brief  Ԫ������
//! \author hyu
//! \date   2009-3-11
//! \base   GameFrame
//! \sa     MallFrame, MallFrame
class YuanbaoFrame : public GameFrame
{
public:
    YuanbaoFrame(void);
    ~YuanbaoFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
	virtual VOID Update();

    virtual BOOL EscCancel();

private:
    //! ��ʼ��ListBox�ؼ�
    VOID InitListBox();
    //! ����ListBox�ؼ�
    VOID ResetSellList();
    VOID ResetBuyList();
    VOID ResetOrderList();

    //! ����Ԫ���������ݵ�����
    VOID UpdateYuanbaoTrade();

    //! ˢ�³���list
    VOID UpdateSellList();
    //! ˢ���չ�list
    VOID UpdateBuyList();
    //! ˢ����Ҫ����/�չ�
    VOID ResetInput();
    //! ˢ�½����˻�
    VOID UpdateYBAccount();
    //! ˢ�¶����б�
    VOID BeginUpdateOrder();
    VOID UpdateOrder();

    //! ɾ������
    VOID CanelOrder();
	// ���㽻�׽�Ǯ
	VOID CalcTradeMoney();

    //! �õ��ı�
    LPCTSTR GetTradeType(EYBOTYPE e) { return EYBOT_BUY == e ? g_StrTable[_T("YuanBaoTrade13")] : g_StrTable[_T("YuanBaoTrade14")]; }
    LPCTSTR GetOrderStus(EYBOMODE e)
    {
        switch (e)
        {
        case EYBOM_Submit:
            return g_StrTable[_T("YuanBaoTrade15")];
        	break;
        case EYBOM_Cancel:
            return g_StrTable[_T("YuanBaoTrade16")];
            break;
        case EYBOM_Close:
            return g_StrTable[_T("YuanBaoTrade17")];
            break;
        default:
            return _T("");
            break;
        }
    }

    // ��Ϸ�¼�

    //! ����Ԫ�����׷���˷�����Ϣ
    DWORD OnYuanBaoTradeRet(tagYBReturnEvnt* pEvent);

    //! ������Ϣ�򷵻��¼�
    DWORD OnSaveMoney(tagMsgMoneyBoxEvent* pEvent);
    DWORD OnDepositMoney(tagMsgMoneyBoxEvent* pEvent);
    DWORD OnSellYuanBao(tagMsgBoxEvent* pEvent);
    DWORD OnBuyYuanBao(tagMsgBoxEvent* pEvent);

	DWORD OnTooMuchMoney(tagMsgBoxEvent* pEvent);
	DWORD OnTooMuchYuanBao(tagMsgBoxEvent* pEvent);

    // �����¼�

    //! ����Ԫ������GUI�¼�
    DWORD YuanbaoEventHandler(tagGUIEvent* pEvent);
    //! ��������ѯ����GUI�¼�
    DWORD OrderQEventHandler(tagGUIEvent* pEvent);

    //! �����ܼ�
    INT64 CalSum(INT64 nGold, INT64 nSilver, INT nNum);
    //! ����������
    INT64 CalCharge(INT64 nGold, INT64 nSilver, INT nNum);

    //! �򿪶�����ѯ
    VOID ShowOrderQuery(BOOL bVal);
	//  �õ���ɫ
	DWORD GetOrderColor(EYBOMODE e);

	VOID DestroyObjects();

    TSFPTrunk<YuanbaoFrame> m_Trunk;
    TObjRef<GUISystem>      m_pGUI;
    TObjRef<Util>           m_pUtil;
    TObjRef<GameFrameMgr>   m_pFrameMgr;
    TObjRef<NetSession>     m_pSession;
    CurrencyMgr*            m_pCurrencyMgr;

    INT64                   m_nInputGold;           //!< ������ݴ�����
    INT64                   m_nInputSilver;
    INT                     m_nInputNum;

    map<INT, DWORD>         m_mapRow2OrderID;       //!< ����id���б��еĶ�Ӧ

    // Ԫ������
    GUIWnd*                 m_pWnd;                 //!< Ԫ�����״���
    GUIWnd*                 m_pWndFather;           //!< ������-����������mall.xml

    GUIButton*              m_pBtSalePrice;         //!< Ԫ���۳�����
    GUIButton*              m_pBtSaleNum;           //!< Ԫ���۳�����
    GUIButton*              m_pBtBuyPrice;          //!< Ԫ�����򵥼�
    GUIButton*              m_pBtBuyNum;            //!< Ԫ����������
    GUIListBox*             m_pLBSale;              //!< Ԫ�������б�
    GUIListBox*             m_pLBBuy;               //!< Ԫ�������б�
	BOOL					m_bBuyFlag;				// �Լ������
	BOOL					m_bSellFlag;				// �Լ������

    GUIPushButton*          m_pBtWannaSell;         //!< ��Ҫ����
    GUIPushButton*          m_pBtWannaBuy;          //!< ��Ҫ�չ�
    GUIEditBox*             m_pEdtWannaPriceGold;   //!< ����(��)
    GUIEditBox*             m_pEdtWannaPriceSilver; //!< ����(��)
    GUIEditBox*             m_pEdtWannaNum;         //!< ����
    GUIButton*          m_pBtWannaConfirm;      //!< ��ʼ����/�չ�
	GUIStatic*				m_pStcTotalGold;		// ���׽��
	GUIStatic*				m_pStcTotalSilver;		// ��������
	GUIStatic*				m_pStcTaxGold;			// �����ѽ��
	GUIStatic*				m_pStcTaxSilver;		// ����������

    GUIStatic*              m_pStcAcctGold;         //!< �����˻����(��)
    GUIStatic*              m_pStcAcctSilver;       //!< �����˻����(��)
    GUIButton*          m_pBtMoneySave;         //!< Ǯ����
    GUIButton*          m_pBtMoneyLoad;         //!< Ǯȡ��
    GUIStatic*              m_pStcYuanbao;          //!< �����˻���Ԫ��
    GUIButton*          m_pBtRefresh;           //!< ˢ��
    GUIButton*          m_pBtOrderQuery;        //!< ��ѯ����

    // ������ѯ
    GUIWnd*                 m_pOrderWnd;            //!< ������ѯ����

    GUIButton*              m_pOrderClose;          //!< �����رհ�ť

    GUIListBox*             m_pOrderTable;          //!< �����б�

    GUIButton*              m_pOrderID;             //!< �������
    GUIButton*              m_pOrderType;           //!< ��������
    GUIButton*              m_pOrderPrice;          //!< ����
    GUIButton*              m_pOrderNum;            //!< ����
    GUIButton*              m_pOrderStus;           //!< ״̬
    GUIButton*              m_pOrderTimeFrom;       //!< ��ʼʱ��
    GUIButton*              m_pOrderTimeTo;         //!< ����ʱ��
    GUIButton*              m_pOrderAveragePrice;   //!< �ɽ�����
    GUIButton*              m_pOrderFinalNum;       //!< �ɽ�����

    GUIButton*              m_pOrderCannel;         //!< ��������
    GUIButton*              m_pOrderRefresh;        //!< ˢ�¶���

    bool                    m_bisSell;              //!< �Ƿ��Ǽ���/�չ�״̬
	GUIStaticEx*			m_pStcRules;			// Ԫ�����׹���

	DWORD						m_dwTime;			//ˢ����Ϣ����ʱ��
	BOOL						m_bCanSend;			
};

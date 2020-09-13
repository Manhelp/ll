#pragma once
#include "ComposeStatic.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "..\WorldDefine\ItemDefine.h"
#include "ActiveProgress.h"

struct tagNC_ReceiveGift;
struct tagNS_ReceiveGift;
struct tagNC_GetLiLianExp;
struct tagNS_GetLiLianExp;
struct tagNS_LiLianChange;

class IDButBackFrame : public GameFrame
{
private:
	enum EDeComposeMsg
	{
		EDeCompose_NULL = 0,
		EDeCompose_SendMsg,		
		EDeCompose_Waiting,
		EDeCompose_RecvMsg,
	};

	enum EWhichFrame	//���ĸ�Frame...
	{
		EFrameNULL = 0,
		EFrameHaveReceive,		//�Ѿ���ȡ
		EFrameIDbuyBack,		//�˺Żع�
		EFrameReceiveGift,		//��ȡ����
		EFrameLiLianDu,			//������
	};
public:
	IDButBackFrame();
	~IDButBackFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnViewLiLian(tagGameEvent* pEvent);
	DWORD OnReceiveGift(tagGameEvent* pEvent);
	DWORD OnIDBuyBack(tagGameEvent* pEvent);
	DWORD OnHaveReceive(tagGameEvent* pEvent);

	DWORD OnNS_GetLiLianExp(tagNS_GetLiLianExp* pCmd, DWORD);
	DWORD OnNS_ReceiveGift(tagNS_ReceiveGift* pCmd, DWORD);
	DWORD OnNS_LiLianChange(tagNS_LiLianChange* pCmd, DWORD);

	VOID CloseAllWnd();
	VOID SetInfo();

private:
	TSFPTrunk<IDButBackFrame>				m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*									m_pWndLiLianDu;								//����ɶȵ��Ǹ�
	IconStatic*								m_pIStcItemIcon[MAX_GIFT_ITEM_NUM];			//��Ʒͼ��
	GUIStatic*								m_pStcItemName[MAX_GIFT_ITEM_NUM];			//��Ʒ����
	ActiveProgress*							m_pProProcess;								//�ܽ�����	
	ActiveProgress*							m_pProCurProcess;							//��ǰ������
	IconStatic*								m_pIStcItemShowIcon[MAX_GIFT_ITEM_SHOW_NUM];//��ʾ��ɶȸߵĽ���
	GUIStatic*								m_pStcItemShowName[MAX_GIFT_ITEM_SHOW_NUM];	//
	GUIStatic*								m_pStcDesc[MAX_GIFT_ITEM_SHOW_NUM];			//��Ʒ����
	GUIButton*								m_pBtndLiLianDuClose;								//�رհ�ť
	GUIStatic*								m_pStcCompleteDesc;							//����ɶ�
	GUIStatic*								m_pStcCurCompleteDesc;						//��ǰ��ɶ�

	GUIWnd*									m_pWndReceiveGift;							//��ȡ�������Ǹ�~
	IconStatic*								m_pIStcCurItem[MAX_GIFT_ITEM_NUM];			//��ǰ������Ʒͼ��
	GUIStatic*								m_pStcCurItemName[MAX_GIFT_ITEM_NUM];		//��ǰ������Ʒ����
	IconStatic*								m_pIStcNextItem[MAX_GIFT_ITEM_NUM];			//�¸�������Ʒͼ��
	GUIStatic*								m_pStcNextItemName[MAX_GIFT_ITEM_NUM];		//�¸�������Ʒ����
	GUIButton*								m_pBtnOk;									//ȷ����ȡ
	GUIButton*								m_pBtnReceiveClose;							//�ر�

	GUIWnd*									m_pWndIDbuyBack;							//�˺Ż��ս���
	IconStatic*								m_pIStcBuyBackItemIcon[MAX_GIFT_ITEM_NUM];	//��Ʒͼ��
	GUIStatic*								m_pStcBuyBackItemName[MAX_GIFT_ITEM_NUM];	//��Ʒ����
	GUIButton*								m_pBtnBuyBackOk;							//ȷ���ջ��˺�
	GUIButton*								m_pBtnBuyBackReceiveGigt;					//��ȡ��Ʒ
	GUIButton*								m_pBtnBuyThink;								//������~
	GUIButton*								m_pBtnBuyClose;								//�رհ�ť
	IconStatic*								m_pIStcBuyBackShowItem[MAX_GIFT_ITEM_SHOW_NUM];		
	GUIStatic*								m_pIStcBuyBackShowItemName[MAX_GIFT_ITEM_SHOW_NUM];
	GUIStatic*								m_pStcButBackDesc[MAX_GIFT_ITEM_SHOW_NUM];

	GUIWnd*									m_pWndHaveReceive;							//�ظ���ȡ
	IconStatic*								m_pIStcHaveReceiveItemIcon[MAX_GIFT_ITEM_NUM];	//��һ�׶ν���ͼ��..
	GUIStatic*								m_pStcReceiveItemName[MAX_GIFT_ITEM_NUM];	//��Ʒ����
	GUIButton*								m_pBtnHaveReceiveClose;						//�رհ�ť
	GUIButton*								m_pBtnHaveReceiveOK;						//ȷ����ť
	GUIStatic*								m_pStcHaveReceiveTips;						//�Ǹ���ʾ

	DWORD									m_dwCurLiLianExp;							//��ǰ����ֵ
	INT										m_nGetGiftNum;								//��ȡ�������
	tagIDBuyBackEx*							m_IDBuyBackEx;
	DWORD									m_dwMaxExp;

	EWhichFrame								m_eOpenWhictFrame;
};
#pragma once
#include "ComposeStatic.h"
#include "ActiveProgress.h"
#include "Item.h"
#include "IconStatic.h"

struct tagStartNPCTalkEvent;
struct tagItemPosChangeExEvent;
struct tagNS_FairyFusion;
struct tagNS_FairyFusionResult;
struct tagNS_GetFusionFairy;
struct tagUpdateLPItemEvent;

class FairyFusionFrame :
	public GameFrame
{
private:

public:
	FairyFusionFrame();
	~FairyFusionFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	VOID RefreshRetUI();
	VOID ClearData();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnStartNPCTalkEvent(tagStartNPCTalkEvent* pGameEvent);
	DWORD OnEventProduceAddItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnOpenFairyFusion(tagGameEvent* pEvent);
	DWORD OnBeginFusionEvent(tagMsgBoxEvent* pEvent);
	DWORD OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pEvent);

	DWORD OnNS_FairyFusion(tagNS_FairyFusion* pMsg, LPVOID pPrama);
	DWORD OnNS_GetFusionFairy(tagNS_GetFusionFairy* pMsg, LPVOID pPrama);

	VOID SendGetFairyMsg(bool isAutoSend);

	BYTE GetFushionScoreByQuality(BYTE byQuality)
	{
		if(byQuality == 6) 			//��ɫ
			return 100; 
		else if(byQuality == 5) 	//��ɫ
			return 50; 
		else if(byQuality == 4)		//��ɫ
			return 15;

		return 0;
	}

	//DWORD OnNS_GetKeyCodeGift(tagNS_GetKeyCodeGift* pMsg, DWORD pPrama);
private:
	TSFPTrunk<FairyFusionFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	ComposeStatic*				m_pFirstFairyIcon;
	ComposeStatic*				m_pSecondFairyIcon;
	ComposeStatic*				m_pIMIcon;
	GUIButton*					m_pBtnClose;
	GUIButton*					m_pBtnFusion;
	GUIStatic*					m_pStcTip;			//����ǰ����ʾ��Ϣ
	GUIStatic*					m_pStcRetTip;		//����ʱ����tip
	GUIStatic*					m_pStcPinkRet;		//��ɫ�ɹ���
	GUIStatic*					m_pStcPurpleRet;	//��ɫ�ɹ���
	GUIStatic*					m_pStcOrangeRet;	//��ɫ�ɹ���
	GUIStatic*					m_pStcIMNum;		//��������
	GUIButton*					m_pBtnUp;			//����
	GUIButton*					m_pBtnDown;			//����
	ActiveProgress*				m_pProgress;		//������

	DWORD						m_escapeTime;
	INT64						m_n64FirstID;
	INT64						m_n64SecondID;
	INT64						m_n64IMID;
	BYTE						m_byIMNum;
	BOOL						m_bSendMsg;

	Item*						m_pLockFirstItem;	//��������ֻ������ָ��
	Item*						m_pLockSecondItem;
	Item*						m_pLockIMItem;		//����IM����


	GUIWnd*						m_pWndGetFairy;		//��ȡ�������
	IconStatic*					m_pIStcFirstFairy;	//��һֻ����
	IconStatic*					m_pIStcSecondFairy;	//�ڶ�ֻ����
	IconStatic*					m_pIStcFirstFairyPic;//��ͼ
	IconStatic*					m_pIStcSecondFairyPic;//��ͼ
	GUIButton*					m_pBtnRecive;		//��ȡ����
	GUIButton*					m_pBtnCloseGetWnd;	//�رհ�ť

	BOOL						m_bChoseFirst;		//�Ƿ�ѡ������
	Equipment*					m_pResultFairy;		//���ɵ�����
	INT64						m_n64TmepFirstFairy;//������id����

};

#define NULLIconPic _T("data\\ui\\Common\\L_diban-b.bmp")
//Ĭ�ϵ�ͼ
#define DefaultIconPic _T("data\\ui\\common\\l_icon.bmp")
//ѡ�е�ͼ
#define SelectIconPic _T("data\\ui\\common\\l_icon-l")



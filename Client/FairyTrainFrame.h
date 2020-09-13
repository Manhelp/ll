/********************************************************************

** �ļ���: FairyTrainFrame.h

** Copyright (c) �Ϻ�������缼�����޹�˾�����ֹ�˾ All Right Reserved

** ������: yfw

** ��  ��: 08/25/2010

** �޸���: yfw

** ��  ��: 08/25/2010

** ��  ��: ������Լ

*********************************************************************/
#pragma once

class IconStatic;
struct tagNS_OpenFairyTrain;
struct tagNS_ArrestingFairy;
struct tagShowDeadMenuEvent;

class FairyTrainFrame : public GameFrame
{
public:
	FairyTrainFrame(void);
	~FairyTrainFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

public:
	VOID SetNPCID( DWORD ID ) { m_dwNPCID = ID; }

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	DWORD OnNS_OpenFairyTrain(tagNS_OpenFairyTrain* pMsg, DWORD dwParam);
	DWORD OnNS_ArrestingFairy(tagNS_ArrestingFairy* pMsg, DWORD dwParam);
	DWORD OnLPDeadEvent(tagShowDeadMenuEvent* pEvent);
	VOID UpdateUI();
	VOID CloseAllUiEffect();
	VOID PlayEffect();

private:
	TSFPTrunk<FairyTrainFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;										// ������
	GUIStatic *m_pStcFairyName;							// �����������
	GUIStatic *m_pStcCurTameVale;						// ��ǰ��ѱ��ֵ
	GUIProgress *m_pProTameRate;						// 
	GUIStatic *m_pStcMaxTameValue;						// ��Լֵ
	IconStatic *m_pTameItem;							// ��Լ��Ʒ
	GUIProgress *m_pProTime;							// ��Լʱ�䣻
	IconStatic *m_pItem[5];								// ��Լ������Ʒ
	GUIButton *m_pBtnItemNum[5];						// �����Լ��Ʒ����
	DWORD m_dwItemTypeID[5];							// ��Լ��ƷID
	DWORD m_dwCurItemID;								// ��ǰ��Լ��ƷID
	INT m_nCurTameValue;								// ��ǰ��Լֵ
	INT m_nMaxTameValue;								// �����Լֵ
	GUIButton *m_pBtnClose;
	GUIButton *m_pBtnQuit;
	GUIPatch*m_pPcBack;

	DWORD m_dwNPCID;									// ��������ID
	DWORD m_dwEscapeTime;
	vector<DWORD> m_vecItemID;							// ��Լ��ƷID
	bool m_bChoosing;									// �Ƿ�ѡ����
};
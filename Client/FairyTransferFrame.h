/********************************************************************

** �ļ���: FairyTransferFrame.h

** Copyright (c) �Ϻ�������缼�����޹�˾�����ֹ�˾ All Right Reserved

** ������: yfw

** ��  ��: 10/25/2010

** �޸���: yfw

** ��  ��: 10/25/2010

** ��  ��: �����˺�Ԫ��ת��

*********************************************************************/
#pragma once
class IconStatic;
struct tagItemPosChangeExEvent;
struct tagNS_FairyEleInjuryChange;

class FairyTransferFrame : public GameFrame
{
public:
	FairyTransferFrame(void);
	~FairyTransferFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

public:
	VOID SetNPCID( DWORD ID ) { m_dwNPCID = ID; }

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	VOID PutFairy(INT16 nPos);
	VOID SetBook();
	VOID LockItem(INT64 nID, bool bOperable);
	VOID ClearInfo();
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD NetFairyTranfer( tagNS_FairyEleInjuryChange *pNetCmd, DWORD );

private:
	TSFPTrunk<FairyTransferFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;										// ������
	IconStatic *m_pIconFairy;
	GUIStatic *m_pStcFairyName;
	GUIStatic *m_pStcFairyLevel;
	GUIStatic *m_pStcFairyDamageType;
	IconStatic *m_pIconFairyDamageType[6];
	GUIButton *m_pBtnFairyDamageType[6];
	GUIStatic *m_pStcBookName;
	GUIStaticEx *m_pStcBookNeedNum;
	GUIStatic *m_pStcBookTotalNum;
	GUIStaticEx *m_pStcGold;
	GUIStaticEx *m_pStcSilver;
	GUIProgress *m_pProTime;

	GUIButton *m_pBtnTranfer;
	GUIButton *m_pBtnQuit;
	GUIButton *m_pBtnExit;

	DWORD m_dwNPCID;									// ת��ʦ
	bool m_bProcessing;									// ����
	DWORD m_dwEscapeTime;

	DWORD m_dwItem[6];
	INT64 m_n64FairyID;
	INT64 m_n64ItemID;
	BYTE m_byDamageType;
	BOOL m_bHaveMoney;
	BOOL m_bHaveBook;
};
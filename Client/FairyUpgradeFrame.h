/********************************************************************

** �ļ���: FairyUpgradeFrame.h

** Copyright (c) �Ϻ�������缼�����޹�˾�����ֹ�˾ All Right Reserved

** ������: yfw

** ��  ��: 10/26/2010

** �޸���: yfw

** ��  ��: 10/26/2010

** ��  ��: ������������

*********************************************************************/
#pragma once
class IconStatic;
struct tagItemPosChangeExEvent;
struct tagNS_FairyIncEleInjury;

class FairyUpgradeFrame : public GameFrame
{
public:
	FairyUpgradeFrame(void);
	~FairyUpgradeFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	//VOID SetInfo( BYTE byType, BYTE byTimes, INT nValue, bool bDamage = false );
	VOID SetInfo( BYTE byType, WORD wTimes, INT nValue, BYTE bAttType );//modify by xp

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	VOID LockItem(INT64 nID, bool bOperable);
	DWORD NetFairyUpgrade( tagNS_FairyIncEleInjury *pNetCmd, DWORD );
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	VOID PutItem(INT16 nPos, DWORD dwIndex = GT_INVALID);
	VOID UpdateUI();

private:
	TSFPTrunk<FairyUpgradeFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;										// ������
	GUIStatic *m_pStcFairyDamage;
	GUIStatic *m_pStcDamageBaseValue;
	GUIStatic *m_pStcAddMin;
	GUIStatic *m_pStcAddMax;

	IconStatic *m_pIconFruit[3];
	GUIStatic *m_pStcFruitName[3];
	GUIStatic *m_pStcFruitAddMin[3];
	GUIStatic *m_pStcFruitAddMax[3];

	IconStatic *m_pIconStone;
	GUIStatic *m_pStcRate;
	GUIStatic *m_pStcUpgradeTime;

	GUIProgress *m_pProTime;

	GUIButton *m_pBtnUpgrade;
	GUIButton *m_pBtnQuit;
	GUIButton *m_pBtnExit;

	bool m_bProcessing;

	INT m_nValue;
	WORD m_wTime;
	BYTE m_byType;
// 	BOOL m_bDamage;
	BYTE m_bDamageType;//Ԫ���˺����ͣ�0�����˺���1����͸
	BYTE  m_bAttType;//add by xp ���������˺�����͸�͵ֿ�0�˺���1��͸��2�ֿ�
	INT m_nMin;
	INT m_nMax;

	INT64 m_n64FruitID[3];
	INT64 m_n64StoneID;
};
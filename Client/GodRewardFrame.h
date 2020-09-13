#pragma once

struct tagRefreshGodRewardEvent;
struct tagRoleGetNameEvent;

class GodRewardFrame : public GameFrame
{
public:
	GodRewardFrame();
	~GodRewardFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	
private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodRewardFrame> m_Trunk;

	GUIWnd *m_pWnd;
	
	GUIStatic *m_pStcWin;
	GUIStatic *m_pStcNum1;	// ��1��
	GUIStatic *m_pStcNum2;	// ��2��
	GUIStatic *m_pStcNum3;	// ��3��
	GUIStatic *m_pStcNum4;	// ��4��
	GUIStatic *m_pStcNum5;	// ��5��

	GUIStatic *m_pStcTime1;
	GUIStatic *m_pStcTime2;
	GUIStatic *m_pStcTime3;
	GUIStatic *m_pStcTime4;
	GUIStatic *m_pStcTime5;


	GUIStatic *m_pStcKillNum;	// ��ɱ����
	GUIStatic *m_pStcLiveTime;	// ��Ծʱ��

	GUIStatic *m_pStcExploit;		// ���ɹ�ѫ
	GUIStatic *m_pStcContribution;	// ���ɹ���
	GUIStatic *m_pStcIntegral;		// ���ɻ���
	GUIStaticEx *m_pStcReward;		// ������ȡ��

	GUIButton *m_pBtnOK;	// ȷ��
	GUIButton *m_pBtnClose;	// X

	DWORD m_dwRoleID1;
	DWORD m_dwRoleID2;
	DWORD m_dwRoleID3;
	DWORD m_dwRoleID4;
	DWORD m_dwRoleID5;


	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnEventRefresh(tagRefreshGodRewardEvent *pEvent);
	DWORD OnEventRoleGetName(tagRoleGetNameEvent *pEvent);

	DWORD m_dwElapseTime;
};
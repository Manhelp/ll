#pragma once

struct tagNS_GetStrengthDetail;

class StrengthShowFrame : public GameFrame
{	
public:
	StrengthShowFrame();
	~StrengthShowFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
//	DWORD OnNetGetStrengthDetail(tagNS_GetStrengthDetail *pNetCmd, DWORD);
private:
	TSFPTrunk<StrengthShowFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGFMgr;

	GUIWnd *m_pWnd;
	GUIStatic *m_pStcTotalStrength;	// ʵ����ֵ
	GUIStatic *m_pStcBaseStrength;	// ����ʵ��ֵ
	GUIStatic *m_pStcLevel;			// �ȼ�
	GUIStatic *m_pStcEquip;			// װ��
	GUIStatic *m_pStcPet;			// ����
	GUIStatic *m_pStcFame;			// ����
	GUIStatic *m_pStcTitle;			// �ƺ�
	GUIStatic *m_pStcFriend;		// ����
	GUIStatic *m_pStcQuest;			// ����
	GUIStatic *m_pStcProduction;	// ����
	GUIStatic *m_pStcAdvanceStrength;// ����ʵ��ֵ 
	GUIStatic *m_pStcActivity;		// �
	GUIStatic *m_pStcInstance;		// ����
	GUIStatic *m_pStcRanQuest;			// ����
	GUIStatic *m_pStckillPlayer;		// սʤ�ǰ������
	GUIStatic *m_pStcConsumptiveStrength;// �����ĵ���
	GUIButton *m_pBtnClose;			// �ر�
};	
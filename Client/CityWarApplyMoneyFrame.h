#pragma once

class CityWarApplyMoneyFrame : public GameFrame
{
public:
	CityWarApplyMoneyFrame();
	~CityWarApplyMoneyFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID SetLowerText(int nFund);
	VOID SetAverageText(int nFund);
private:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnMsgBoxEventConfirmAttack(tagMsgBoxEvent *pEvent);
	DWORD OnEventClose(tagGameEvent *pEvent);

private:
	TSFPTrunk<CityWarApplyMoneyFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<GUISystem> m_pGUI;
	
	GUIWnd *m_pWnd;
	GUIEditBox *m_pInputGold;		// �����
	GUIStatic *m_pStcGold;			// �������޷��ý�
	GUIStatic *m_pStcAverageGold;	// ����ƽ�����ý�
	
	GUIButton *m_pBtnOK;			// ȷ��
	GUIButton *m_pBtnCancel;		// ȡ��

	INT32 m_nInputGold;			// ������
	DWORD m_dwCityID;			// ����ID
};


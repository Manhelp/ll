#pragma once
#include "gameframe.h"

struct tagNS_RolePracticeTalentPoint;

class BuyTalentFrame : public GameFrame
{
public:
	BuyTalentFrame(void);
	~BuyTalentFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID ShowWnd( BOOL bShow );

protected:
	
	DWORD EventHandler( tagGUIEvent* pEvent );

	DWORD OnBuyTalent();

	void RefreshDisplay();

	INT64 CalculateCost( DWORD dwTalentPointNum );

	DWORD OnNS_RolePracticeTalentPoint( tagNS_RolePracticeTalentPoint* pNetCmd, DWORD dwParam );

private:
	TSFPTrunk<BuyTalentFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*					m_pWnd;
	GUIButton*				m_pBtnAdd;				//�������ʰ�ť
	GUIButton*				m_pBtnOk;				//ȷ����ť
	GUIButton*				m_pBtnCancel;			//ȡ����ť
	GUIButton*				m_pBtnClose;			//�رհ�ť

	GUIStaticEx*			m_pStcMaxPoint;			//��ǰ����ɵõ���������ʵ���
	GUIStaticEx*			m_pStcCanBuyPoint;		//���ɹ�������ʵ���
	GUIStaticEx*			m_pStcGold;				//��Ҫ���ѵĽ�
	GUIStaticEx*			m_pStcSilver;			//��Ҫ���ѵ���
	GUIStaticEx*			m_pStcHaveGold;			//��ɫ��ǰ���еĽ�
	GUIStaticEx*			m_pStcHaveSilver;		//��ɫ��ǰ���е���

	INT64					m_nWillCostGold;		//��Ҫ���ѵĽ�
	INT64					m_nWillCostSilver;		//��Ҫ���ѵ���
};

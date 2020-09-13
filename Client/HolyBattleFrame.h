#pragma once
#include "gameframe.h"

/** class	HolyBattleFrame
brief	   �ɽ���Ҷ������
*/ 

struct tagNS_SetArenaInterface;
struct tagNS_SetArenaName;
struct tagNS_SetArenaPercent;
struct tagNS_SetArenaMemNum;


class HolyBattleFrame :
	public GameFrame
{
public:
	HolyBattleFrame(void);
	~HolyBattleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID	ShowWnd(BOOL bOpen=TRUE);
	VOID	ShowMinWnd(BOOL bOpen=TRUE);
private:
	DWORD OnNS_SetArenaInterface(tagNS_SetArenaInterface *pNetCmd, DWORD);
	DWORD OnNS_SetArenaName(tagNS_SetArenaName *pNetCmd, DWORD);
	DWORD OnNS_SetArenaPercent(tagNS_SetArenaPercent *pNetCmd, DWORD);
	DWORD OnNS_SetArenaMemNum(tagNS_SetArenaMemNum *pNetCmd, DWORD);

	VOID ShowTips(DWORD dwIndex);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD MinEventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<HolyBattleFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*				m_pWnd;
	GUIButton*			m_pBtnUnfold;							//�رհ�ť
	GUIStatic*			m_pStcBattleName;				//ս����
	GUIProgress*		m_pProRedArmy;					//���������
	GUIStatic*			m_pStcRedPercent;					//����ٷֱ�
	GUIStatic*			m_pStcRedNum;					//����ٷֱ�
	GUIProgress*		m_pProBlueArmy;					//����������
	GUIStatic*			m_pStcBluePercent;				//�����ٷֱ�
	GUIStatic*			m_pStcBlueNum;				//�����ٷֱ�

	GUIWnd*				m_pWndMin;
	GUIButton*			m_pBtnWndFold;							//չ����ť

};

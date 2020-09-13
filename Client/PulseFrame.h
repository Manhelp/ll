#pragma once
#include "gameframe.h"
#include "PulseData.h"

struct tagNS_ActivePulse;
struct tagNS_PulseCanLearnNum;
class IconCutStatic;
class ItemButton;

struct tagPulseStatic
{
	IconCutStatic*	pICSpic;
	GUIStatic*			pStSelected;

	bool IsContain(GUIWnd* pWnd);
	VOID Selected();
	VOID DeSelected();
	bool IsSeclected();
	VOID SetScaleValue(DWORD dwScale,bool bSetCur = true);
};

#define MAX_PULSE_PERPAGE 10

enum
{
	EP_P_FIRE = 0,
	EP_P_WATER,
	EP_P_THUNDER,
	EP_P_WIND,
	EP_P_ELEMENT,
};

class PulseFrame :
	public GameFrame
{
public:
	PulseFrame(void);
	~PulseFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	DWORD EventHandler(tagGUIEvent* pEvent);

	VOID UpdateUI(bool bfirst = true);
	VOID UpdateDesc();
	VOID ActivePulse();
	
	bool CanClick(int pos);

	VOID ClearSelectedMark();
	VOID ShowFrame(BOOL b){m_pWnd->SetInvisible(!b); if(b) UpdateUI(true); }

private:
	DWORD OnNS_ActivePulse( tagNS_ActivePulse* pMsg, DWORD dwParam );
	DWORD OnNS_PulseCanLearnNum( tagNS_PulseCanLearnNum* pMsg, DWORD dwParam );
	DWORD OnOpenPulse(tagGameEvent* pEvent);
	DWORD OnClosePulse(tagGameEvent* pEvent);
	DWORD OnNotCheckDistance(tagGameEvent* pEvent);//�������NPC�ľ���
private:
	TSFPTrunk<PulseFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	PulseData									m_data;

	DWORD									m_dwNPCID;
	DWORD									m_escapeTime;

	GUIWnd*									m_pWnd;

	GUIButton*								m_pBnActive;
	GUIButton*								m_pBnClose;

	GUIPushButton*						m_pPbFire;
	GUIPushButton*						m_pPbWater;
	GUIPushButton*						m_pPbThunder;
	GUIPushButton*						m_pPbWind;
	GUIPushButton*						m_pPbElement;


	GUIStatic*									m_pPcFire;
	GUIStatic*									m_pPcWater;
	GUIStatic*									m_pPcThunder;
	GUIStatic*									m_pPcWind;
	GUIStatic*									m_pPcElement;

	GUIStatic*									m_pStName;
	GUIStaticEx*								m_pStLevels;
	GUIStaticEx*								m_pStNextDesc;
	GUIStatic*									m_pStGold;
	GUIStatic*									m_pStSilver;
	ItemButton*									m_pStItem;
	ItemButton*									m_pStItem2;	//�¼ӵ���

	GUIStatic*									m_pStItemBack;
	GUIStatic*									m_pStItemBack2;
	GUIStatic*									m_pStSuccessRate;

	GUIStatic*									m_pStCurrGold;
	GUIStatic*									m_pStCurrSilver;
	GUIStatic*									m_pSTodayAcitve;

	tagPulseStatic							m_stPulseFire[MAX_PULSE_PERPAGE];
	tagPulseStatic							m_stPulseWater[MAX_PULSE_PERPAGE];
	tagPulseStatic							m_stPulseThunder[MAX_PULSE_PERPAGE];
	tagPulseStatic							m_stPulseWind[MAX_PULSE_PERPAGE];
	tagPulseStatic							m_stPulseElement[MAX_PULSE_PERPAGE];

	INT											m_ePage;			// ��ǰ��ҳ
	INT											m_nCurPos;		// ��ǰλ��

	INT											m_nCurrActiveTime;	// �Ѿ������˶��ٴ�

	bool											m_bCanSend;
	BOOL										IsCheckDistance;//�Ƿ�����NPC�ľ���
};

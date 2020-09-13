#pragma once

#include "ServerTime.h"
struct tagRefreshGodStateEvent;
struct tagRefreshPersonalGodStateEvent;
struct tagRefreshFlagStateEvent;
class IconStatic;

class GodStateFrame : public GameFrame
{
public:
	GodStateFrame();
	~GodStateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	VOID Show(bool bShow);
private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodStateFrame> m_Trunk;
	
	GUIWnd *m_pWndExpand;
	GUIPatch *m_pAttackNum;
	GUIPatch *m_pAttackGuildName;
	GUIPatch *m_pDefenceNum;
	GUIPatch *m_pDefenceGuildName;
	GUIButton *m_pBtnFold;	// ����
	GUIButton *m_pBtnDetail;// ����
	GUIStatic *m_pStcKillTime;	// ��ɱ����
	GUIStatic *m_pStcLiveTime;	// ��Ծʱ��
	GUIStatic *m_pStcTime;		// ����ʱ
	IconStatic *m_pIconFlagSky;		// ����
	IconStatic *m_pIconFlagDevil;	// ħ��
	IconStatic *m_pIconFlagHuman;	// ����
	GUIStatic *m_pStcExp;			// ���ý�Ǯ
	GUIStatic *m_pStcGold;			// ���ý�
	GUIStatic *m_pStcSilver;		// ������
	GUIStatic *m_pStcGuildExploit;		// ���Ṧѫ
	GUIStatic *m_pStcGuildContribute;	// ���ṱ��

	DWORD EventHandlerExpand(tagGUIEvent *pGUIEvent);

	DWORD OnEventRefreshGodState(tagRefreshGodStateEvent *pEvent);
	DWORD OnEventRefreshPersonalGodState(tagRefreshPersonalGodStateEvent *pEvent);
	DWORD OnEventRefreshFlagState(tagRefreshFlagStateEvent *pEvent);

	DWORD m_dwAttackGuildID;
	DWORD m_dwDefenceGuildID;

	tagDWORDTime m_ActivityTime;	// ���ʼʱ��

	GUIWnd *m_pWndFold;
	GUIPatch *m_pAttackNumFold;
	GUIPatch *m_pAttackGuildNameFold;
	GUIPatch *m_pDefenceNumFold;
	GUIPatch *m_pDefenceGuildNameFold;
	IconStatic *m_pIconFlagSkyFold;		// ����
	IconStatic *m_pIconFlagDevilFold;	// ħ��
	IconStatic *m_pIconFlagHumanFold;	// ����
	GUIStatic *m_pStcTimeFold;		// ����ʱ
	GUIButton *m_pBtnExpend;	// ����
};


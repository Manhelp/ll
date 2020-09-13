#pragma once
#include "gameframe.h"

//#define _UICAMERA_TOOL // ����������Ч����С����

class UICameraTestFrame :
	public GameFrame
{
public:
	UICameraTestFrame(void);
	~UICameraTestFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID GO();
	DWORD OnOpenUICameraTool(tagGameEvent* pGameEvent);//add by xp

private:
	TSFPTrunk<UICameraTestFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;


	GUIWnd*							m_pMainWnd;					//������
	GUIEditBox*						m_pEd_PosX;	
	GUIEditBox*						m_pEd_PosY;
	GUIEditBox*						m_pEd_PosZ;	

	GUIEditBox*						m_pEd_LookAtX;
	GUIEditBox*						m_pEd_LookAtY;	
	GUIEditBox*						m_pEd_LookAtZ;

	GUIEditBox*						m_pEd_EffectPosX;
	GUIEditBox*						m_pEd_EffectPosY;	
	GUIEditBox*						m_pEd_EffectPosZ;

	GUIEditBox*						m_pEd_EffectName;	
	GUIEditBox*						m_pEd_WndPath;

	GUIButton*						m_pBnGO;
	GUIButton*						m_pBnClose;
	GUIButton*						m_pBnClose1;//���Ͻǵġ��Źرհ�ť

	Vector3							m_v3Pos;
	Vector3							m_v3LookAt;
	Vector3							m_v3EffectPos;

	tstring							m_strEffectName;
	tstring							m_strWndPath;
};

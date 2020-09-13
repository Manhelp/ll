#pragma once
#include "gameframe.h"
struct tagNS_KeyCode;
struct tagNS_NewKeyCode;

class KeyCodeFrame :
	public GameFrame
{
public:
	KeyCodeFrame(void);
	~KeyCodeFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	BOOL IsCheckDistance;//add by xp�Ƿ�����NPC�ľ���

	virtual BOOL EscCancel();

	VOID Show(BOOL b) { if(P_VALID(m_pWnd)) m_pWnd->SetInvisible(!b); }
	VOID SetNPC(DWORD dwNPCID) { m_dwNPCID = dwNPCID; }
	VOID SetType(BOOL IsNew){ m_bIsNew = IsNew;}
private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnRecvServerMsg( tagNS_KeyCode* msg, DWORD );
	DWORD OnNS_NewKeyCode( tagNS_NewKeyCode* msg, DWORD );
	VOID  DestroyObjects();
	BOOL  RevMsgError(DWORD dwErrorID);
	BOOL  CanSendKeyCode();
private:
	TSFPTrunk<KeyCodeFrame> m_Trunk;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;

	GUIWnd*					m_pWnd;     
	GUIStaticEx*			m_pStETips;
	GUIEditBox*				m_pEBxKeyCode;
	GUIButton*				m_BtnConfirm;
	GUIButton*				m_BtnCancel;

	DWORD					m_escapeTime;
	DWORD					m_dwNPCID;
	BOOL					m_bIsNew;	//�Ƿ�Ϊ�µĶ���key��

	bool						m_bOldPlayerBack;

};

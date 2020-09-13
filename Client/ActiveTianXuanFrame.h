#pragma once
#include "GameFrameMgr.h"
#include "GameFrame.h"
#include "ItemEvent.h"
#include "ItemFrame.h"
#include "..\WorldDefine\msg_activity.h"
#include "ActiveProgress.h"
#include "..\WorldDefine\msg_map.h"

class ActiveTianXuanFrame : public GameFrame
{
public:
	ActiveTianXuanFrame(void);
	~ActiveTianXuanFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);

	VOID RefreshUI();	//ˢ�·�������
	tstring GetNextValue(LPCTSTR &pMsg, TCHAR token);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_DotaMsg(tagNS_DotaMsg* pCmd, DWORD);
	DWORD OnNS_GotoNewMap(tagNS_GotoNewMap* pCmd, DWORD);

private:
	TSFPTrunk<ActiveTianXuanFrame>			m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*									m_pWndMain;
	GUIStatic*								m_pStcBluePoint;	//��������
	GUIStatic*								m_pStcRedPoint;		//�췽����
	GUIStatic*								m_pStcBlueNum;		//��������
	GUIStatic*								m_pStcRedNum;		//�췽����
	GUIStatic*								m_pStcIcon;			//�м��Ǹ�����
	ActiveProgress*							m_pProPersent;		//������

	DWORD									m_dwRedPoint;		//�����
	DWORD									m_dwBluePoint;		//������
	INT										m_nRedNum;			//������
	INT										m_nBlueNum;			//������

};
#pragma once
#include "..\WorldDefine\RoleDefine.h"
#include "..\WorldDefine\clergy_define.h"
#include "ActiveProgress.h"

/** \brief ����Frame
*/

struct tagNS_CondensedChange;
struct tagNS_StopCondensed;
struct tagNS_BeginCondensed;
struct tagNS_Skill;

class CondensedFrame : GameFrame
{
public:
	CondensedFrame();
	~CondensedFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID Show(bool bShow);
	VOID Clear();

private:
	DWORD OnNS_CondensedChange(tagNS_CondensedChange* pCmd, DWORD);
	DWORD OnNS_StopCondensed(tagNS_StopCondensed* pCmd, DWORD);
	DWORD OnNS_BeginCondensed(tagNS_BeginCondensed* pCmd, DWORD);

	DWORD OnOpenCondensedFrame(tagGameEvent* pEvent);
	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);

private:
	TSFPTrunk<CondensedFrame> m_Trunk;
	TObjRef<GUISystem>		   m_pGUI;
	TObjRef<GameFrameMgr>	   m_pFrameMgr;
	TObjRef<NetSession>        m_pNetSession;
	TObjRef<NetCmdMgr>         m_pNetCmdMgr;

	GUIWnd                  *m_pWnd;
	GUIWnd                  *m_pWndCaption;
	GUIStatic				*m_pStcGodHead;					//���
	GUIStatic               *m_pStcBelief;					//����
	ActiveProgress          *m_pProCondensed;				//���۶�
	GUIStatic				*m_pStcCondensed;				//���۶���ֵ
	ActiveProgress          *m_pProBar;						//������
	GUIButton               *m_pBtnBegin;					//��ʼ����
	GUIButton               *m_pBtnClose;					//�ر�
	GUIButton               *m_pBtnCloseWnd;				//�ر�*
	GUIStatic				*m_pStcStaricon[MAX_GodHead];	//С����ͼƬ

	BOOL					m_bIsConsdensed;				//�Ƿ���������

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
};
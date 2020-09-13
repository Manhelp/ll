#pragma once
#include "..\WorldDefine\clergy_define.h"

struct tagNS_GetGodRelation;

const DWORD dwNameColor = 0xfffffea9;		//������ɫ
const DWORD dwNameShade = 0xff321500;		//�������
const DWORD dwDefaultColor = 0xff8ea28e;	//��ȱʱ����ɫ
const DWORD dwDefaultShade = 0xff112a11;	//��ȱʱ���

class GodRelationFrame : GameFrame
{
public:
	GodRelationFrame();
	~GodRelationFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID Hide();
	VOID Show();

	VOID ClearName();		//���пؼ����óɿ�ȱ��
	VOID ShowClergyTips(GUIWnd* pWnd, int nClergyID);	//��ʾ��ְtips

private:
	TSFPTrunk<GodRelationFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pNetSession;
	TObjRef<NetCmdMgr>				m_pNetCmdMgr;

	GUIWnd							*m_pWnd;
	GUIWnd							*m_pWndCaption;
	GUIStaticEx						*m_pStcMap[ECGT_Num][MAX_GOD_NUM];
	GUIButton						*m_pBtnClose;
	GUIStatic						*m_pStcGodTips[MAX_GOD_Relation_NUM];	//������ʾtips����26���ؼ�..

	DWORD							m_escapeTime;


private:
	BOOL EventHandler(tagGUIEvent* pEvent);

	DWORD OnNS_GetGodRelation(tagNS_GetGodRelation* pCmd, DWORD);

	DWORD OnOpenGodRelation(tagGameEvent* pEvent);
};
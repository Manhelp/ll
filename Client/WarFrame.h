#pragma once

/*!
\class WarFrame
\brief �ػ���ͼ���Frame
\author lib
\base GameFrame
\remark
*/
#define MAX_WAR_ROL_NUM	6
struct tagRefreshWarInfo;
class IconStatic;
struct tagWarroleInfo
{
	DWORD dwRoleID;
	ERoleAttrSync ARoleInfo[ERoleAttrEnd];
};
class WarFrame : public GameFrame
{
public:
	WarFrame(void);
	~WarFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	DWORD On_RefreshWarInfo(tagRefreshWarInfo* evt);
	DWORD OnEnterWar(tagGameEvent* evt);
	DWORD OnEnterNormalMap(tagGameEvent* evt);
	DWORD OnShow_WarResult(tagGameEvent* evt);
	DWORD OnClose_WarFrame(tagGameEvent* evt);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	TSFPTrunk<WarFrame>			m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI�ؼ� �����Ϣ
	GUIWnd*						m_pWndRoleInfo;
	GUIListBox*					m_pLstRoleInfo;
	GUIButton*					m_pBtnHideRoleInfo;

	//���������Ϣ
	GUIWnd*						m_pWndShowRoleInfo;
	GUIButton*					m_pBtnShowRoleInfo;

	//�������
	GUIWnd*						m_pWndJieSuan;
	GUIListBox*					m_pLstJieSuan;	//�б�
	GUIButton*					m_pBtnQuitInfo;	//�˳�����
	GUIButton*					m_pBtnCloseWnd;	//�رմ���
	IconStatic*					m_pIStcItem1;
	IconStatic*					m_pIStcItem2;
	IconStatic*					m_pIStcItem3;
	GUIStatic*					m_pStcItemMeng1;	//����Ч���ؼ�
	GUIStatic*					m_pStcItemMeng2;
	GUIStatic*					m_pStcItemMeng3;

	GUIStatic*					m_pStcExpReward;
	GUIStatic*					m_pStcGold;
	GUIStatic*					m_pStcSilver;

	GUIButton*					m_pBtnRefreshReward;
	GUIButton*					m_pBtnReciveReward;
	GUIButton*					m_pBtnCloseScore;


	tagWarroleInfo				m_saRoleWarInfo[MAX_WAR_ROL_NUM];	//�����Ϣ
};

#pragma once

class ItemButton;
class Equipment;
class IconStatic;
struct tagItemPosChangeExEvent;
struct tagNS_StrengthenEquip;
struct tagNS_RolePutDownItem;

class EquipStrengthenFrame :public GameFrame
{

public:
	EquipStrengthenFrame(void);
	~EquipStrengthenFrame(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnNoGuardFu( tagMsgBoxEvent *pGameEvent );
	DWORD NetEquipStrengthen( tagNS_StrengthenEquip *pNetCmd, DWORD );
	DWORD NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD );
	VOID PutItem( INT16 nPos );
	void LockItem( bool bOperable );
	BOOL CanStartStrengthen();
	VOID ClearUI();
	VOID UpdateUI();
	VOID CloseAllUiEffect();
	DWORD OnItemEvent_UpdateItem(tagGameEvent* pEvent);

private:
	TSFPTrunk<EquipStrengthenFrame>	m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndCaption;
	ItemButton*					m_pEquip;				// ��Ҫǿ����װ��
	ItemButton*					m_pStone;				// ǿ��ʯ
	ItemButton*					m_pLucky;				// ���˷�
	ItemButton*					m_pGuard;				// �ػ���
	GUIProgress*				m_pProTime;				// ������
	GUIStaticEx*				m_pStcEquipName;
	GUIStatic*					m_pStcEquipState;		// װ��������״̬
	GUIStatic*					m_pStcStoneName;
	GUIStatic*					m_pStcLuckyName;
	GUIStatic*					m_pStcGuardName;
	GUIStaticEx*				m_pStcSucRate;			// ���ӵĳɹ���
	GUIStatic*					m_pStcGolden;			// ��
	GUIStatic*					m_pStcSiller;			// ��
	GUIButton*					m_pBtnOk;				// ��ʼǿ��
	GUIButton*					m_pBtnCancel;			// �ر�
	GUIButton*					m_pBtnExit;				// �˳�
	IconStatic*					m_pStcResult;			// ǿ�����
	DWORD						m_dwRecivMegTime;

	DWORD						m_dwNPCID;				// NPC��ID
	DWORD						m_escapeTime;

	Equipment*					m_pItemEquip;	// װ��
	Item*						m_pItemStone;	// ǿ��ʯ
	Item*						m_pItemLucky;	// ���˷�
	Item*						m_pItemGuard;	// ���׷�
	BOOL						m_bProcessing;	// �Ƿ�ǿ����
};
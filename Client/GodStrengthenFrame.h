#pragma once

class ItemButton;
struct tagNS_GodStrengthen;
struct tagItemPosChangeExEvent;

class GodStrengthenFrame :public GameFrame
{

public:
	GodStrengthenFrame(void);
	~GodStrengthenFrame(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD NetEquipStrengthen( tagNS_GodStrengthen *pNetCmd, DWORD );
	VOID PutItem( INT16 nPos, bool bMainEquip = true );
	void LockItem( INT64 nID, bool bOperable );
	VOID UpdateUI();
	DWORD OnGodStrengthenExpValue( tagMsgBoxEvent *pGameEvent );

private:
	TSFPTrunk<GodStrengthenFrame>	m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndCaption;
	ItemButton*					m_pEquip;				// ��Ҫǿ����װ��
	ItemButton*					m_pAssisEquip;			// ����ǿ��װ��
	GUIStatic*					m_pStcEquipLevel;		// �����ȼ�
	GUIStatic*					m_pStcTime;				// �´�����ʱ��
	GUIProgress*				m_pProExp;				// ���������
	GUIStatic*					m_pProValue;			// ���������
	GUIStatic*					m_pStcExp;				// ���β������ӵľ���ֵ
	GUIStatic*					m_pStcGolden;			// ��
	GUIStatic*					m_pStcSiller;			// ��
	GUIButton*					m_pBtnOk;				// ��ʼ����
	GUIButton*					m_pBtnCancel;			// ȡ��
	GUIButton*					m_pBtnExit;				// x

	INT64						m_n64EquipID;
	INT64						m_n64AssisEquipID;
	BOOL						m_bTime;				// �Ƿ��ѹ�24Сʱ
};
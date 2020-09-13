#pragma once

const INT FREE_PK_LEVEL = 20; // ��ҵȼ��ﵽ30��ʱ��PKģʽ��Ϊ����ģʽ 

/** \����״̬����ʱ��ʼ�¼�
*/
struct tagPKStateStartCountDownEvent : public tagGameEvent
{
	DWORD dwTimeCount;           // ����ʱ��ʱ�䣨��λ�����룩
	tagPKStateStartCountDownEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

class ActiveProgress;
class FlashPushButton;
class BuffUIList;
class RoleHeadPicIcon;
struct tagRoleRenameEvent;
struct tagUpdataAvatarEvent;
struct tagRoleBuffChangeEvent;
struct tagUpdateLPAttEvent;
struct tagLPRoleStateChangeEvent;
struct tagUpdateLPLevel;
struct tagTeamLeaderChangeEvent;
struct tagUpdataAvatarDisplaySetEvent;
struct tagRoleGetNameEvent;
struct tagNS_RolePKStateChange;
struct tagLPPKState;
struct tagNS_RoleRevive;
struct tagShowDeadMenuEvent;
struct tagNS_RolePKMorality;

/** \class PlayerStateFrame
    \brief ���Ͻǽ�ɫ���Խ���
*/
class PlayerStateFrame : public GameFrame
{
public:
	enum { BUFFUI_NUM = 20 };

	/** \������Ҳ˵�ѡ��
	*/
	enum EMenuItem
	{
		EMI_PKMode,             // PKģʽ
		EMI_RemoveTeamMate,     // �����Ա
		EMI_CaptainHandOut,     // �ӳ��ƽ�
		EMI_PickMode,           // ʰȡģʽ
		EMI_ExitTeam,           // �˳�����
		EMI_RefuseApply,		// �ܾ�����
		EMI_Num
	};

	/** \PKģʽ�˵�ѡ��
	*/
	enum EMenuPKModeItem
	{
		EMPKI_Protected,         // ����ģʽ
		EMPKI_Freedom,           // ����PK
		EMPKI_Num
	};

	/** \ʰȡģʽ�˵�ѡ��
	*/
	enum EMenuPickModeItem
	{
		EMPickI_Freedom,         // ����ʰȡ
		EMPickI_InTurn,          // ����ʰȡ
		EMPickI_Num
	};

	/** \�ܾ�����˵�ѡ��
	*/
	enum EMenuRefuseModeItem
	{
		EMRefuse_No,          // ��������
		EMRefuse_Yes,         // �ܾ�����
		EMRefuse_Num
	};

	PlayerStateFrame();
	~PlayerStateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID Render3D();

private:
	BOOL GUIEventHandler(tagGUIEvent* pEvent);
	VOID SafeDestroyUI();
	BOOL CreateMenus();
	VOID UpdataMenuItem();
	VOID ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow );
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex );
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize );
	VOID UpdataSecondMenuDisplay();
	VOID ResetSecondMenuPos( const INT nCurMainMenuSel );
	VOID ReBuildTeammateList( GUIListBox* pListBox );
	VOID OnSecondMenuItemClick( const INT nSelItem );
    VOID RefreshClassText();
	/** \��Ϸ�¼���Ӧ����
	*/
	DWORD OnRoleRenameEvent(tagRoleRenameEvent* pEvent);
	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent);
	DWORD OnUpdataAvatarEvent(tagUpdataAvatarEvent* pEvent);
	DWORD OnPKStateStartCountDownEvent(tagPKStateStartCountDownEvent* pEvent);
	DWORD OnRoleBuffChangeEvent(tagRoleBuffChangeEvent* pEvent);
	DWORD OnUpdateLPLevel(tagUpdateLPLevel* pEvent);
	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent);
	DWORD OnLPRoleStateChangeEvent(tagLPRoleStateChangeEvent* pEvent);
	DWORD OnTeamLeaderChangeEvent(tagTeamLeaderChangeEvent* pEvent);
	DWORD OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent);
    DWORD OnLocalPlayerSetClass(tagGameEvent* pEvent);
	DWORD OnLPPKState(tagLPPKState* pEvent);
	DWORD OnLPDeadEvent(tagShowDeadMenuEvent* pEvent);
	/** \�ȼ���Ӧ����
	*/
	DWORD OnOpen_ViolenceOnlyHotKey(DWORD dwID, BOOL bDown);		// L_CTL + X
	DWORD OnOpen_ViolenceAllHotKey(DWORD dwID, BOOL bDown);			// L_CTL + C
	DWORD OnOpen_ViolenceKillHotKey(DWORD dwID, BOOL bDown);		// L_CTL + V

	//DWORD OnNetPKStateChange(tagNS_RolePKStateChange* pMsg, DWORD pParam);
	DWORD NetRecvNS_RoleRevive(tagNS_RoleRevive* pMsg, DWORD dwParam);
	DWORD NetNS_RolePKMorality(tagNS_RolePKMorality* pMsg, DWORD dwParam);

	DWORD OnEventAutoOpenPK(tagGameEvent *pEvent);

private:
	TSFPTrunk<PlayerStateFrame>	    m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameInputMap>			m_pKeyMap;

	GUIWnd*                         m_pWnd;                    // ������
	RoleHeadPicIcon*				m_pHead;				   // ͷ��ͼ��
	GUIStatic*                      m_pSt_PVP;                 // PVP״̬��ǩ
	GUIStatic*                      m_pSt_Name;                // ���Ʊ�ǩ
	GUIStatic*                      m_pSt_Level;               // �ȼ���ǩ
	ActiveProgress*                 m_pPro_Hp;                 // HP������
	ActiveProgress*                 m_pPro_Mp;                 // MP������
	GUIProgress*                    m_pPro_Sta;                // �־���������
	GUIProgress*					m_pPro_LoongYin;			   // ����ֵ
	GUIStatic*                      m_pSt_Vocation;            // ְҵ��ǩ
	GUIPushButton*                  m_pBn_Menu;                // �˵���ť
	GUIButton*						m_pBtnExitTeam;			   // �˶�
	    
	GUIStatic*                      m_pSt_Captain;             // �ӳ�ͼ��
	GUIWnd*							m_pWnd_Menu;               // ����������˵�����
	GUIWnd*							m_pWnd_SecondMenu;         // �����˵�����
	GUIListBox*                     m_pLb_Menu;                // ����������˵��б��
	GUIListBox*                     m_pLb_SecondMenu;          // �����˵��˵��б��

	GUIWnd*                         m_pWnd_Violence;           // PKģʽ����
	GUIFrameMutex*					m_pMutex;				   // �����
	FlashPushButton*				m_pBtnPeace;				// ����ģʽ
	GUIPushButton*					m_pBtnRivalry;				// �Կ�ģʽ
	GUIPushButton*					m_pBtnKill;					// ɱ¾ģʽ
	GUIStatic*						m_pSt_ViolenceTime;			// ����ʱ

	BuffUIList*                     m_pBuffUIList;                  // buff�����б�
	EMenuPKModeItem					m_ePKMode;                      // ��ǰ��PKģʽ
	EMenuPickModeItem				m_ePickMode;                    // ��ǰ��ʰȡģʽ
	EMenuRefuseModeItem				m_eRefuseApplyMode;				// ��ǰ�ľܾ�����ģʽ
	DWORD							m_dwViolenceTimeCount;			// ����״̬��ʼ��ʱ��ʱ��
	DWORD                           m_dwViolenceStartCountDownTime; // ����״̬��ʼ��ʱʱ��
	INT                             m_nCurMenuIndex[EMI_Num];       // ��ǰ��Ҳ˵�ѡ���ӦListBox������
	INT								m_nLastSelMainMenu;             // һ���˵���һ��ѡ�е�����
	INT                             m_nMenuPanelHeightExt;          // �˵��װ�߶���listBox�Ĳ�(���Ų˵�ʱʹ��)

	bool m_bFlag;
};


#pragma  once 

#include "../WorldDefine/msg_master_apprentice.h"

class MasterFrame : public GameFrame
{
	// ��ݲ˵�
	enum EMenuItem
	{
		EMI_PlayerInfo,			// �������
		EMI_Chat,				// ��Ϊ˽��
		EMI_AddFriend,			// ��Ϊ����
		EMI_InviteGroup,		// �������
		EMI_ApplyGroup,			// �������
		EMI_Num,
	};
public:
	MasterFrame();
	~MasterFrame();

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();
	void ShowFrame();

	// ʹ����ҵ��������
	void ShowUseWnd();
private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	void ShowMaster();
	void ShowStudent();

	// �ƶ���ݲ˵�
	VOID MoveMenu( GUIListBox* pListBox, const tagPoint& ptDest );
	// ���±�ݲ˵�
	VOID UpdateMenu();
	// �ı��ݲ˵��Ĵ�С
	VOID ResizeMenu( GUIListBox* pListBox, int row, int width );

	// ��ȡ����ʱ����ʾ
	tstring GetOfflineTime( bool bIsOnline, bool bMasterMask, DWORD &dwColor, DWORD dwOfflineTime = GT_INVALID );


	// �鿴�ҵ�ʦ�Ž���
	DWORD OnNetGetMasterInfo(tagNS_MatesInfo *pNetCmd, DWORD);
	// �鿴�ҵ�ͽ�ܽ���
	DWORD OnNetGetStudentInfo(tagNS_ApprenticeInfo *pNetCmd, DWORD);
	// ʹ����ҵ����
	DWORD OnNetUseJingWuPoint(tagNS_UseJingWuPoint *pNetCmd, DWORD);
	// ������ҵ������Ϣ
	DWORD OnNetAssignJingWuPoint( tagNS_GiveJingWuPoint *pNetCmd, DWORD );

	// ��ʾ������ҵ��������
	void ShowAssignWnd();
	// �Ƿ���Է�����ҵ����
	bool CanAssignJingWuPoint();
	// �Ƿ����ʹ����ҵ����
	bool CanUseJingWuPoint();

	// ���ʦ��
	DWORD OnNS_BreakOutApprentice( tagNS_BreakOutApprentice *pNetCmd, DWORD );
	// ���ʦ��
	DWORD OnNS_BreakOutApprenticeConfirm( tagNS_BreakOutApprenticeConfirm *pNetCmd, DWORD );
	// ���ʦ��ȷ��
	DWORD OnLeaveApprentice( tagMsgBoxEvent *pGameEvent );

	// ����ʦ��
	DWORD OnNS_BreakOutMaster( tagNS_BreakOutMaster *pNetCmd, DWORD );
	// ����ʦ��
	DWORD OnNS_ForceBreakOutMaster( tagNS_ForceBreakOutMaster *pNetCmd, DWORD );
	// ����ʦ��
	DWORD OnNS_BreakOutMasterConfirm( tagNS_BreakOutMasterConfirm *pNetCmd, DWORD );
	// ����ʦ��ȷ��
	DWORD OnLeaveMaster( tagMsgBoxEvent *pGameEvent );
	// ǿ������ʦ��
	DWORD OnForceLeaveMaster( tagMsgBoxEvent *pGameEvent );

private:
	TSFPTrunk<MasterFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;
	TObjRef<NetSession> m_pSession;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;

	GUIWnd *m_pWnd;									// ʦͽ������
	GUIPatch *m_pWndBack;
	GUIPatch *m_pMain;

	GUIFrameMutex *m_pTeaAndStu;					
	GUIPushButton *m_pBtnMaster;					// �ҵ�ʦ��
	GUIPushButton *m_pBtnStu;						// �ҵ�ͽ��

	GUIStatic *m_pStcName;							// ʦ������
	GUIStatic *m_pStcLevel;							// ʦ��ȼ�
	GUIStatic *m_pStcNumOfQingWu;					// ʦ����ҵ����
	GUIStatic *m_pStcNumOfQinWu;					// ʦ��ʦ�µ���

	GUIButton *m_pBtnName;							// ����
	GUIButton *m_pBtnLevel;							// �ȼ�
	GUIButton *m_pBtnTask;							// ʦͽ����
	GUIButton *m_pBtnLastLogin;						// �ϴ�����ʱ��
	GUIListBox *m_pList;							// ʦ���б��

	GUIStatic *m_pStcInfo;							// ʦ��˵��
	GUIButton *m_pBtnAssign;						// �������ʹ����ҵ����
	GUIButton *m_pBtnLeave;							// ����ʦ��
	GUIButton *m_pBtnClose;							// �ر�ʦ��������
	GUIButton *m_pBtnQuit;							// �ر�ʦ��������
	GUIButton *m_pBtnRule;							// ����˵��

	GUIWnd *m_pAssignWnd;							// ������ҵ��������
	GUIStatic *m_pStcStuName;						// ���ӵ�����
	GUIStatic *m_pStcJingWu;						// ������Ҫ����ҵ����
	GUIEditBox *m_pEdtAssignJingWu;					// ��������ӵ���ҵ����
	GUIStatic *m_pStcTotalJingWu;					// �ɷ������ҵ����
	GUIButton *m_pBtnOK;							// ȷ������
	GUIButton *m_pBtnCancle;						// ȡ������
	GUIButton *m_pBtnQuitA;							// ȡ������

	GUIWnd *m_pUseWnd;								// ʹ����ҵ��������
	GUIStaticEx *m_pStcUseInfo;						// ʹ��˵��
	GUIButton *m_pBtnUseOK;							// ��ҵ����ʹ��ȷ��
	GUIButton *m_pBtnUseCancle;						// ��ҵ����ʹ��ȡ��

	GUIWnd *m_pMenuWnd;								// ��ݲ˵�����
	GUIListBox *m_pMenuList;						// ��ݲ˵��б��

	DWORD m_dwTotalRow;								
	DWORD m_dwClickRow;								// ��ǰѡ�е���
	DWORD m_dwRoleID;								// ��ǰѡ�е��ӵ�ID

	GUIWnd *m_pRuleWnd;								// ����˵��
	GUIButton *m_pBtnRuleMaster;					// ��ʦ��ʦ
	GUIButton *m_pBtnRuleMasterTask;				// ʦͽ����
	GUIButton *m_pBtnRuleJingWu;					// ��ҵ����
	GUIButton *m_pBtnRuleQuit;						// �ر�

	vector<tagMatesAndApprenticeInfo> m_vecMyGuild;						// �ҵ�ʦ��
	vector<tagMatesAndApprenticeInfo> m_vecMyStudent;					// �ҵ�ͽ��
};
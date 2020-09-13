#pragma once

struct tagPlayerPopupMenu;
struct tagRoleRenameEvent;
struct tagPvPRoomMenuEvent;

class PopUpMenuFrame : public GameFrame
{
	//�Ҽ��˵�����ʵ���ͣ��Է��㲻ͬ�Ҽ��˵�����չ
	enum EShowPopUpMenuType
	{
		ESPPT_Null,
		ESPPT_Player,
		ESPPT_PvPRoomPos,
	};

	//����Ҳ˵���Ч�Ĳ˵���ö��
	enum EMenuItemPlayer
	{
		EMIP_PrivateChat,       // ˽    ��
		EMIP_Friend,            // ��    ��
		EMIP_Trade,             // ��    ��
		EMIP_Follow,            // ��    ��
		EMIP_TeamInvite,        // �������
		EMIP_FactionInvite,     // ��������
		EMIP_PlayerInfo,        // �����Ϣ
		EMIP_AddToBlackList,    // �Ӻ�����
		EMIP_PetTrade,          // ���޽���
		EMIP_MountInvite,		// �������
		EMIP_TeamApply,         // �������
		EMIP_PlayerInfo2,		// �������
		EMIP_MiniChat,          // ����
        EMIP_ApprenticeToMaster,// �����ʦ
        EMIP_ToHaveApprentice,  // ��Ϊͽ��
		EMIP_ApplySpritePart,  // �����������
		EMI_BeautyFriends,		// װ�����
		EMI_InviteFamily,		//��������
		EMI_ApplyFamily,		//��������
		EMIP_Num,
	};
public:
	PopUpMenuFrame( void );
	~PopUpMenuFrame( void );

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();

private:
	
	/** �¼�������
	*/
	BOOL EventHandler(tagGUIEvent* pEvent);

	/**  �����Ҽ��˵�����ʾ���ͣ�����ʵ����ַ����ʵ��ķ���
	*/
	void UpdateMenuItem();

	/** ���ý�ɫ���Ҽ��˵�����ʵ����
	*/
	void UpateMenuPlayer();

	/** ����PvP���Ҽ��˵�����ʵ����
	*/
	void UpdateMenuPvP();
	
	/**	�����ɫ�Ҽ��˵���ѡ���¼�
	*/
	void ProcessPlayerEvent( DWORD dwMenuItemIndex );

	/**	����PvP�Ҽ��˵���ѡ���¼�
	*/
	void ProcessPvPEvent( DWORD dwMenuItemIndex );
	
	/** ������ʾ����Ŀ���ı�˵���۵Ĵ�С
	*/
	VOID ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow );
	
	/** ���ò˵������ʾ����
	*/
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex );
	
	/** ���ò˵������ʾ����
	*/
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize );

	/** ���ز˵���������ʾģʽΪ null
	*/
	void ResetMenu();

	/** ���˵��ƶ���ָ��λ��
	*/
	void ForceMoveTo(tagPoint& pt );

	DWORD OnPlayerPopUpMenu( tagPlayerPopupMenu* pEvent );

	DWORD OnPvPPopUpMenu( tagPvPRoomMenuEvent* pEvent );

	/** 
		\���ݹ�����ڵ㵽�˵��ľ����ж�Ҫ��Ҫ�رղ˵�
		\param pt ����
		\param ptView �˵����Ͻǵ�
		\param ptSize �˵���С
		\param dist ������������Ҫ�ر�
	*/
	BOOL NeedClose( tagPoint pt, tagPoint ptView,tagPoint ptSize, LONG dist );

	VOID UpdateFashionData();

private:
	
	TSFPTrunk<PopUpMenuFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pMenu;						//�˵�������
	GUIListBox*					m_pListBox;						//�˵����б�
	GUIWnd*						m_pWnd_SuitMenu;				// ʱװ�˵�����
	GUIListBox*                 m_pLb_SuitMenu;					// ʱװ�˵��б��
	EShowPopUpMenuType			m_eShowType;					//��ǰ�Ҽ��˵�����ʵ����
	INT							m_nMenuPanelHeightExt;			//�˵��߿�

	DWORD						m_dwCurShowRoleID;				//����Ҳ˵�����ʾʱ����ʾ��ҵ�ID
	tstring						m_strRoleName;					//��ɫ������
	INT							m_MenuItemPlayer[EMIP_Num];		//����Ҽ��˵���

};

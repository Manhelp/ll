#pragma once
#include ".\WorldFrame.h"
#include "..\WorldDefine\msg_quickbar.h"
class ButtonEx;
class ShortcutButton;
class DragBtn;


struct tagUpdateLPItemEvent;
struct tagUpdateLPSkillEvent;
struct tagRoleGetNameEvent;
struct tagShortcutCacheEvent;
struct tagRolePickEvent;
struct tagNS_VirginOnline;
struct tagNS_MsgQuickBarData;
struct tagQuickBarState;
struct tagNS_MsgIsSavedQuickBarData;
struct tagUpdateLPAttEvent;
struct tagUpdateLPSkillEvent;
//-----------------------------------------------------------------------------
// !class ShortcutBarFrame.h
// !brief ��������
//-----------------------------------------------------------------------------
class ShortcutBarFrame :
	public GameFrame
{
public:
	ShortcutBarFrame(void);
	~ShortcutBarFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual VOID Update();

	//�ؼ��¼�������
	DWORD EventHandler(tagGUIEvent* pEvent);

	//���ؿ�ݰ�ť
	BOOL LinkShortcutBtn(TCHAR* szName, int nBar);

	//������չ��ݰ�ť
	void CreateShortcutBtnEx(char* szName);

	//������ˢ��
	void RefreshRoleExp();

	//���ָ���Ŀ�ݰ�ť
	ShortcutButton* GetShortcutButtion(int group, int num);

	void ReflashCDTime();
	void SetBtnEnable();

	// ������������	
	void SaveBtnData();
    
	void SetIsChange(BOOL bIschange);
	//������������
	void SaveQuickBarToDB();

    // �Զ�����¼��ܵ����������λ
    void AddNewSkill(DWORD dwTypeID);

private:
    // ��ɫ���ε�¼���ÿ�ݰ�ť
    DWORD OnNS_VirginOnline(tagNS_VirginOnline* pMsg, DWORD);
    // ��ɫ��½ʱ��ȡ���ݿ���������
	DWORD OnNS_MsgQuickBarData(tagNS_MsgQuickBarData* pMsg, DWORD);
    // �������������ݿ��Ƿ���������Ϣ
	DWORD OnNS_MsgIsSavedQuickBarData(tagNS_MsgIsSavedQuickBarData* pMsg, DWORD);
	

	// �������ʱ��ɾ����Ӧ�Ŀ�ݷ�ʽ
	DWORD OnDeletePet( tagGameEvent* pEvent );

	//ˢ����ͨ����ͼƬ
	DWORD OnWeaponSwap(tagGameEvent* pEvent);

	DWORD OnActiveHolySkillEvent(tagUpdateLPSkillEvent* pEvent);

	//��ʾ/���ؿ�ݰ�ť
	void ShowBar(int bar);
	void HideBar(int bar);

	DWORD OnKey(DWORD dwID, BOOL bDown);
	DWORD OnKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKeyEx(DWORD dwID, BOOL bDown);
	DWORD OnKeyExChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKey2Move(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKey2Move1(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveChange1(DWORD dwID, DWORD key1, DWORD key2);

	DWORD OnKeyTransSkill(DWORD dwID, BOOL bDown);
	DWORD OnKeyTransSkillChangel(DWORD dwID, DWORD key1, DWORD key2);

	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
	DWORD OnInitLPAttEvent(tagGameEvent* pEvent);
	DWORD OnResetSkillIdleTime(tagGameEvent* pEvent);
	DWORD OnKeyUpDown(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveOpenOrClose(DWORD dwID, BOOL bDown);

	// ��DIK�õ������ַ�����
	LPCTSTR GetKeyName(DIK key);

	//ע�����
	void RegisterKeyBoard();

	//���°�ť�Ŀ�������
	void UpdateBtnEnable();		

	//���ּ��ܿ��������
	BOOL AutoAdaptFreshHandSkill(ShortcutButton* pBtn, DWORD dwSkillID);

	// �뱳��ͬ������
	DWORD OnUpdateLPItemEvent(tagUpdateLPItemEvent* pEvent);
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pEvent);
	DWORD OnInitedSkillEvent(tagGameEvent* pEvent);

	DWORD OnEventClearTalent(tagGameEvent* pGameEvent);

	// 
	DWORD OnShortcutCacheEvent(tagShortcutCacheEvent* pGameEvent);

	// ��ȡ���������	
	void LoadBtnData(DWORD dwLocalID);
	void ReflashShortcutBtn( bool bOnlySkill = false );

	DWORD OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg);

	void ShowMoveShortcutBar(BOOL bShow);
	void ShowMove1ShortcutBar(BOOL bShow);

	DWORD ShowAddAtt(tagGameEvent*);    //������ʾ���������Ժ�����Ͷ���С��ť
	DWORD HideAddAtt(tagGameEvent*);
	DWORD ShowAddTelent(tagGameEvent*);
	DWORD HideAddTelent(tagGameEvent*);

	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent);

	// ��ʾ�����ܱ�
	DWORD ShowTransSkill(tagGameEvent*);
	// ���ر����ܱ�
	DWORD HideTransSkill(tagGameEvent*); 
private:

	TSFPTrunk<ShortcutBarFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameInputMap>		m_pKeyMap;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	
	static const int			BAR_COUNT = 11;					//�����������
	static const int			BAR_BUTTON_COUNT = 8;			//ÿ���ݼ�����
	static const int			TRANS_BUTTON_COUNT = 6;			//�����ݼ�����
	static const int			BAR_TITLE_COUNT = 12;
	static const int			BAR_COUNT_TIPS = 4;
	static TCHAR*				m_szAccArray[BAR_TITLE_COUNT+BAR_COUNT_TIPS];
	static int					m_nAccArray[ BAR_TITLE_COUNT+BAR_COUNT_TIPS ];
	static TCHAR*				m_szMoveArray[BAR_BUTTON_COUNT];
	static byte					m_byMoveArray[BAR_BUTTON_COUNT];
	static TCHAR*				m_szMoveArray1[BAR_BUTTON_COUNT];
	static int					m_nMoveArray1[BAR_BUTTON_COUNT];
	static TCHAR*				m_szTransSkill[TRANS_BUTTON_COUNT];
	static int					m_nTransSkill[TRANS_BUTTON_COUNT];
	static TCHAR*				m_szBarGroup[BAR_COUNT_TIPS];
	static byte					m_byBarGroup[BAR_COUNT_TIPS];
	static TCHAR*				m_szKeyUpDown[2];
	static int					m_nKeyUpDown[2];
	static TCHAR*				m_szKeyMoveOC[2];
	static int					m_nKeyMoveOC[2];
	int							m_nBarGroupCount;				//��¼��ǰ��������
	FLOAT						m_fCurrentExp;					//��ǰ����
	DWORD						m_LastSuccSkill;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pPthBack;						//���������
	GUIStatic*					m_pPthShortcutBack;				//�������ť����
	GUIStatic*					m_pPthExpBack;					//����������
	GUIProgress*				m_pProExp;						//������
	GUIStatic*					m_pStcChoose;					//���������ʾF1-F4
	GUIButton*					m_pBtnMoveDn;					//�ƶ���������л������л���ť
	GUIButton*					m_pBtnMoveUp;					//�ƶ���������л������л���ť
	GUIButton*					m_pBtnDn;						//��������л������л���ť
	GUIButton*					m_pBtnUp;						//��������л������л���ť
  	GUIButton*                  m_pAddatt;
  	GUIButton*                  m_pAddTelent;

	tagQuickBarState            m_QuickBarFlags[BAR_COUNT][BAR_BUTTON_COUNT];      //�����״̬�仯
	ShortcutButton*				m_pShortcutBtn[BAR_COUNT][BAR_BUTTON_COUNT];		//4���ݰ�ť
	BOOL						m_bInitShortcut;				//�Ƿ��ȡ������
	BOOL                        m_bIsLoadFromDB;                  //�Ƿ񱣴浽����
	INT                         m_nQuickBar_Col_Count;           //���������ڼ���
	BOOL                        m_bHaveNotSavedQuickBarData;     //���ݿ��Ƿ����������ݵı�ʾ
	BOOL                        m_bIsChange;                     //֪ͨ�����������ݵĿ���
	BOOL                        m_bIsQuit;                       //�Ƿ��˳���
	BOOL                        m_bIsReQuest;                    //�Ƿ��������ȥ�Ƿ�������

	GUIWnd*						m_pWnd_Move1;
	GUIWnd*						m_pWnd_Move2;

	GUIWnd*						m_pWnd_TransSkill;

	INT64							m_nCurExp;						//��ǰ����
	INT							m_nCurLev;						//��ǰ�ȼ�
	INT64							m_nCurTotalExp;					//��һ����Ҫ�ľ���
	INT							m_nPctNExp;						//N%������
	
	bool						m_bLPAttIsOK;
	ShortcutButton				*m_pWndEvent;
	float						m_fTime;
	bool						m_bSend;
	DWORD				m_dwATTcheckTimer;
	//Ԫ��
	GUIButton*				m_pBtnHolyMen;

public:
	ShortcutButton*				m_pLastPress;					//�ϴΰ��Ŀ�ݼ�

private:
	VOID WG_CheckingInPut();
private:
	DWORD						m_dwIdleTime;
	bool						m_bCheckOn;
	bool						m_bNetSessionStartCheck;

};

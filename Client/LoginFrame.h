//-----------------------------------------------------------------------------
//!\file LoginFrame.h
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief login frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "..\Cool3D\Animation\AnimationCtrl.h"
#include "..\WorldDefine\selectrole.h"

#define  OPEN_PASSPORT_LOG		0
#define  OPEN_PASSPORT		1
const int  g_nUrlNum = 5;

//-----------------------------------------------------------------------------
//!\brief login frame
//!
//��		��¼ģʽ��0			�ڲ�ʹ��
//��		��¼ģʽ��1			��½
//��		��¼ģʽ��2			Ѹ��
//��		��¼ģʽ��3			̨�壨���ӣ�
//��		��¼ģʽ��4			ʢ��
//��		��¼ģʽ��5			����
//��		��¼ģʽ��6			Խ��
//��		��¼ģʽ��7			̨��	OMG
//!			��¼ģʽ��8			�¼���  OMG	�ڵ�¼ģʽ0�Ļ����������˱�������Ĺ���
//!			��¼ģʽ��9			���͵�¼ģʽ
//��		��¼ģʽ��10		����˹
//!			��½ģʽ��11		����SM
//-----------------------------------------------------------------------------
class NetSession;
class Client;
struct tagNLS_GetTokenID;
class HttpPicEx;
struct tagWeb_URLInfo;
struct tagNLC_SafeProof;
struct tagURLInfo
{
	DWORD	dwID;
	string		strUrl;
	tstring		szDesc;
	HttpDownload*  pHttp;
	tagURLInfo(): dwID(GT_INVALID),strUrl(""),pHttp(NULL){}
};

class LoginFrame : public GameFrame
{
public:
	LoginFrame();
	~LoginFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	
	//VOID SetPlayState(int nState) { m_nPlayState = nState; }

	VOID Connect(bool bUnrealLogin = TRUE);

	void SetEncrypt( const char* szEncry , const char* szUserName ); 
	void SetUserNameUI( const char* szUserName );
	void SetYY_GetTokenIDorNot(bool b){m_bAskTokenID =b; }

	DWORD OnMsgbox_KoreaLoginErr3Times(tagMsgBoxEvent* pGameEvent);

private:
	TSFPTrunk<LoginFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<XClient>			m_pStream;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<Client>				m_pClient;
	
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndKey;
	//GUIStatic*					m_pWndCaption;
	//GUIStatic*					m_pWndPswHint;
	GUIStatic*					m_pStcAccountText;
	GUIStatic*					m_pStcPsswordText;
	GUIStatic*					m_pStcAccountBack;
	GUIStatic*					m_pStcPsswordBack;
	GUIEditBox*					m_pEditAccount;
	GUIEditBox*					m_pEditPassword;
	GUIButton*					m_pButtonLogin;
	GUIButton*					m_pButtonQuit;
	GUIButton*					m_pBtnReturn;
	GUIStatic*					m_pStcHint;				//������ʾ
	GUIPushButton*				m_pPBtn;
	GUIPushButton*				m_pPBtnSaveAccount;
	GUIPushButton*				m_pPBtnSavePassword;
	GUIPushButton*				m_pPBtnCamera;
	GUIPushButton*				m_pPBtn_Tab;			//��Сд�л�
	GUIStatic*					m_pStcSavePass;
	//GUIStatic*					m_pStcUser;
	GUIEditBox*					m_pLastActive;
	GUIStatic*					m_pStcSavePasswordBack;

	GUIWnd*						m_pWndWorldList;		//������ѡ��
	GUIStatic*					m_pStcWLHint;
	GUIButton*					m_pBtnOKWorld;
	GUIButton*					m_pBtnCancelWorld;
	GUIListBox*					m_pWorldList;

	GUIWnd*						m_pWndQueue;
	GUIStatic*					m_pStcTime;				//�Ŷ�ʣ��ʱ��
	GUIStatic*					m_pStcPosition;			//�Ŷӵ�λ��

	GUIStatic*					m_pWndCancelQueue;		//ȡ���Ŷӽ���
	GUIButton*					m_pBtnCancelQueue;		//ȡ���Ŷ�
	GUIStatic*					m_pStcTips;				//������ʾ

	GUIWnd*						m_pWndSecurity;			//�ܱ�
	GUIStatic*					m_pStcSecuHint;			//�ܱ���ʾ
	GUIEditBox*					m_pStcSecuNum;			//��ʾ������

	GUIWnd*						m_pWndSeInputBack;		//�ܱ����뱳��
	GUIButton*					m_pBtnSecuInput[10];	//
	GUIStatic*					m_pStcInputHint;		//
	GUIButton*					m_pBtnSecuOK;			//
	GUIButton*					m_pBtnSecuCancel;	
	GUIStatic*					m_pStcWebLoginTips;		//������½��ʾ

	//������������ȷ�Ͽؼ�
	GUIWnd*						m_pWndKoreaPassWord;	//����������������
	GUIButton*					m_pBtnKoreaPassWordClose;//�ر�
	GUIButton*					m_pBtnKoreaChangeWorld;	//�����޸Ķ�������
	GUIButton*					m_pBtnKoreaLoginOK;		//��������������ȷ����ť�������ǵ�½��ȷ�ϵ�
	GUIButton*					m_pBtnKoreaLoginCancel;	//������������ȡ����ť
	GUIButton*					m_pBtnKoreaWordKey[10];	//����0��9
	GUIButton*					m_pBtnKoreaClear;		//���㰴ť
	GUIButton*					m_pBtnKoreaBack;		//���˰�ť
	//�������������ؿؼ�
	GUIWnd*						m_pWndFirstSetPassWord;	//��һ�������������
	GUIStatic*					m_pStcKoreaFTip;		//�Ǹ���ʾ��Ϣ��
	GUIStatic*					m_pStcFirstPassWord;	//��һ����������
	GUIStatic*					m_pStcSecondPassWord;	//�ڶ�������ȷ��
	GUIStatic*					m_pStcFirstPassWordPic;	//ѡ���һ�κ���ʾ���Ǹ���
	GUIStatic*					m_pStcSecondPassWordPic;//ѡ��ڶ��κ���ʾ���Ǹ���
	//��½���
	GUIWnd*						m_pWndKoreaLogin;		//�������������½���
	GUIStatic*					m_pStcKoreaLoginTip;	//��½��ʾ
	GUIStatic*					m_pStcLoginPassWord;	//��½����
	//�޸��������
	GUIWnd*						m_pWndKoreaChangePassWord;	//�����޸Ķ�������
	GUIStatic*					m_pStcChangeWordTip;		//�޸�������ʾ
	GUIStatic*					m_pStcOldPassWord;			//������
	GUIStatic*					m_pStcNewPassWord;			//������
	GUIStatic*					m_pStcNewPassWordConfirm;	//������ȷ��
	GUIStatic*					m_pStcOldPassWordPic;		//�������ͼ
	GUIStatic*					m_pStcNewPassWordPic;		//�������ͼ
	GUIStatic*					m_pStcNewPassWordConfirmPic;//������ȷ�ϵ�ͼ

	bool						m_bLoginType;			// �Ƿ��������е�½��ʽ

	string						m_strKoreaLoginNumber;	//������½����
	string						m_strKoreaSetPassWordOne;//�������ö��������һ������
	string						m_strKoreaSetPassWordTwo;//�������ö�������ڶ�������
	string						m_strKoreaChangePassWordOne;	//�����޸Ķ��������һ������
	string						m_strKoreaChangePassWordTwo;	//�����޸Ķ�������ڶ�������
	string						m_strKoreaChangePassWordOld;	//�����޸Ķ������������
	INT8						m_n8KoreaTryTimes;			//������½���Դ���

	VOID SetKoreaTimaOutError();		//�������ӳ�ʱ��ʾ
	
	enum EMBPhase
	{
		EMBP_NULL	= -1,					// ��ʼ
		EMBP_First	= 0,					// �ܱ���һ�׶�
		EMBP_Second	= 2,					// �ܱ��ڶ��׶�
		EMBP_Third	= 4,					// �ܱ������׶�
		EMBP_End	= 6,					// ����
	};

	enum ELoginState
	{
		ELS_WaitInput,
		ELS_Connect,
		ELS_WaitProofResult,
		ELS_MiBao,
		ELS_Queue,
		ELS_ConnectZoneServer,		// �������������
		ELS_SentJoinGame,			// ����JoinGame��Ϣ
		ELS_JoinGame,				// ���յ�JoinGame��Ϣ��
		ELS_Launcher,				// �汾����,�������³���
	};

	enum
	{
		Loading	= 0,				//0������״̬��
		Preface,					//1�����ſ���������
		Loop,						//2�������м䶯����
		Over,						//3�����Ž���������
		Enter,						//4����������ѡ�����
	};

	enum EXLHint
	{
		EXLHint_User,				//�û���������ʾ
		EXLHint_UserError,			//�û������������ʾ
		EXLHint_SendMsg,			//������Ϣ��ʾ
		EXLHint_ConnectFail,		//��ʱ
		EXLHint_Account_No_Match,	
		EXLHint_Account_In_Use,
		EXLHint_Account_Freezed,
		EXLHint_Wrong_Build_Number,
		EXLHint_Disabled,
		EXLHint_NoFindLauncher,
		EXLHint_Wrong_Type,
		EXLHint_Server_Full,
		EXLHint_Server_Suspended,
		EXLHint_Server_Maintenance,
		EXLHint_Failed,
		EXLHint_Short_Time,
		EXLHint_No_Select,
		EXLHint_GameWorldName_Wrong,
		EXLHint_SystemError,
		EXLHint_ProofResult_Forbid_MemberCenter,
		EXLHint_ProofResult_Forbid_GMTool,
		EXLHint_ProofResult_Forbid_CellPhone,
		EXLHint_ProofResult_Forbid_MiBao,
	};

	enum KoreaInputType
	{
		EKIT_Null,
		EKIT_SetPassWordOne,		//���������һ��
		EKIT_SetPassWordTwo,		//��������ڶ���
		EKIT_ChangePassWordOld,		//�޸����룬������
		EKIT_ChangePassWordOne,		//�޸����룬��һ������
		EKIT_ChangePassWordTwo,		//�޸����룬�ڶ�������
	};
	KoreaInputType m_eKoreaInputType;

	enum EKoreaChangeUI
	{
		EKCU_SetPassWord,
		EKCU_Login,
		EKCU_ChangePassWord,
	};

	ELoginState					m_eState;
	DWORD						m_dwStateTime;
	DWORD						m_dwZoneIP;			//ѡ���Zone������IP
	DWORD						m_dwZonePort;		//ѡ���Zone������port
	FLOAT						m_fPingTime;		//pingʱ��
	//NullSceneGraph*				m_pSG;				//����
	//CameraEuler					m_Camera;			//�����
	//EntityNode*					m_EntityNode;		//����
	//VertexAnimationCtrl*		m_pAniCtrl;			//�ؼ�֡����
	//float						m_pTime;			//ÿ�ζ����Ŀ�ʼʱ��
	//ResRenderable*				m_pResRenderable;	//��Դ
	//int							m_nPlayState;		//�������ŵ�״̬
	BOOL						m_bEnter;			//�Ƿ��������ѡ�����
	DWORD						m_dwAccountID;		//joingame�õ�
	DWORD						m_dwVerifyCode;		//joingame�õ�
	DWORD						m_dwSentJoinGame;
	BOOL						m_bSelectedWorld;
	BOOL						m_bGuard;			//�Ƿ�������˺�
	BOOL						m_bSave;
	BOOL						m_bSavePassword;

	INT							m_nPolicy;			//��֤����
	EMBPhase					m_eMB;				//�����ܱ��׶�
	BOOL						m_bMiBao;			//�Ƿ�����ܱ�����
	tstring						m_strRand;			//��¼����ַ���
	string						m_strMiBao;			//��¼�ܱ���ʾ
	tstring						m_strInputMiBao;	//��¼������ܱ�
	BOOL						m_bTab;				//��Сд

	char						m_szEncrypt[100];		//Ѹ�׽ӿڷ��صļ���
	char						m_szUserName[20];		//Ѹ�׽ӿڷ��ص��ʺ�
	EXLHint						m_eHint;			//Ѹ�׽ӿ���ʾ
	HINSTANCE					m_hDll;

	char						m_szPass[64];	// ̨������

   tstring						m_szEncryptCode;    // �����û���
   tstring						m_szPssword;    // ��������

	tstring						m_szOMGUserName;	//OMGģʽ�û��������͵�½ģʽҲ������
	tstring						m_szOMGPassWord;	//OMGģʽ���루���͵�½ģʽҲ������

	tstring						m_szKoreaWebUID;	//����Web UID

	BOOL						m_bSendSDOAInfo;

	INT							m_nBase;
	Vector3						m_LookAt;
	Vector3						m_LookFrom;
	bool						m_bIsShake;
	bool						m_bLeft;
	bool						m_bRight;
	float						m_fMAXTime;

	VOID SetState(ELoginState eState);

	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD EventHandlerForSecurity(tagGUIEvent* pEvent);
	DWORD EventKoreaLoginEvent(tagGUIEvent* pEvent);
	
	VOID WaitProofResult();	
	VOID WaitQueue();
	VOID TryIsConnectZoneSer();

	void JudegPolicy();
	void SendLoginMsg();
	
	// ���ض���
	VOID LoadMap();		

	// ��ʾ�û���\���������Ϣ
	VOID ShowUserPwdInfo(DWORD nInfo);

	// ����UI
	VOID SetUIEnable(BOOL bVale);

	// ����������̰�ťλ��
	VOID SetKeyButtonPos();	

	// ����JoinGame��Ϣ
	DWORD NetRecvJoinGame(tagNS_JoinGame* pMsg, DWORD pPrama); 

	// �Ͽ���zone������������
	VOID CutOffZone();

	// �Ͽ���login������������
	VOID CutOffLogin();

	//��ע����ȡ��¼��Ϣ
	VOID LoadRegistLoginInfo();

	//�������ж�ȡ��¼��Ϣ
	BOOL LoadCMDLoginInfo();

	//��¼��ϢOMGģʽ
	BOOL LoadOMGLoginTWInfo();

	//��¼��Ϣ����ģʽ dk mode
	BOOL LoadDKLoginInfo();

	//��½��Ϣ����Webģʽ
	BOOL LoadKoreaWebInfo();

	// ��ע����ȡ��ǰ�û���������
	VOID LoadLastLoginInfo();

	// ��¼��ǰ�û��������뵽ע���
	VOID SaveLastLoginInfo();

	VOID SaveLastPassword();
	// �����û����������еĻس����е��������ַ�
	VOID FilterString(tstring& str);

	// ����û��ܱ���ť����
	void RandSetKeyMB();

	// �ж���������׶�
	void MBPhaseHint();
	void ClearMBPhase(){ m_eMB = EMBP_NULL; } 
	void SetMBWndShow(BOOL bVal);

	LPCTSTR GetXunLeiHint(EXLHint eType);

	INT GetWindowMode(DWORD dwIndex);

	void ShellExecuteXunLei();

	void LetterTab();

	bool GetCmdLineUserAndPassword();

	void HideLoginFrame( bool bHide );

	void LoginGame();
	
	// password ����
	tstring& InPassWord(tstring& ss);
	// password	����
	tstring& OutPassWord(tstring& ss);

	DWORD OnExitLoadingFrame(tagGameEvent* pGameEvent);

	//������½�������������
	VOID GetRandomNumber();
	//������½��ˢ����ʾ�ĸ������
	VOID KoreaRefrashInput(KoreaInputType eNextState);
	//������½�����������Ϣ
	VOID KoreaClearPassWordInfo();
	//������½���л����á���½���޸��������
	VOID KoreaChangeUI(EKoreaChangeUI eType);
	tagNLC_SafeProof* m_sendSave;	//��������һ�ε�¼��Ϣ
	BOOL			  m_bIsNeedSendAgain;	//������֤ʧ���ٴη���һ��
	
	ResTexture			*m_pTex;
	RECT				m_rectSrc;
	RECT				m_rectDest;
	INT					m_nAlpha;
	DWORD				m_dwColor;
	bool				m_bRender;
	float				m_fParam;

	BYTE m_pMsgBuff[1024];
	DWORD m_dwCurPos;
	BOOL			m_bAutoLogin;
	bool				m_b8sendLogin;
	bool            m_bAskTokenID;		// �Ƿ�Ҫ��tokenid
	bool            m_bShowID;				// �Ƿ���Ҫ��ʾYY
	DWORD		m_dwConnID;

	bool				m_bCloseMapOnce;	//�Ƿ��Ѿ��رյ�ͼ
	//------ open passport ���
	std::vector<tagURLInfo> m_OpenPassportInfo;
	std::vector<tagURLInfo>::iterator m_itOpenPassPortInfo;
	BYTE				m_byOpenPassPotIndex;	//��ǰ����һ�������̵�����
	std::vector<HttpPicEx*> m_vecHttpPic;
	//HttpPicEx*			m_pHttpPicCurr;			//������½����½��ť���Ǹ�ͼ��
	GUIPatch*			m_pOpenPassportPath;	//�װ�
	HttpPicEx*			m_pStcUrlPic[g_nUrlNum];		//url pic
	GUIPushButton*	m_pPBUrl[g_nUrlNum];		//url pic
	GUIButton*			m_pBtnPrev;
	GUIButton*			m_pBtnNext;
	GUIButton*			m_pBtnNormal;		//����������½
	INT						m_nCurUrlPage;  //��0��ʼ
	INT						m_nCurChoose;	//��0��ʼ
	DWORD				m_UpdateUrlTime;
	BOOL					m_bOpenPassport;		//��open passport��½���� �����½
	DWORD				m_dwDownLoadTime;
	BOOL					m_bSendUrlLoginMsg;	//������url��½��Ϣ���
	BOOL					m_bOpenWaitFlag;	// �ȴ�ʱ����
	GUIStatic*			m_pStcOpenPassName[g_nUrlNum];		// ����������
	DWORD				m_dwCropID;		//������id  Ϊ0�����½����Ϊ������ģʽ��½
	GUIStatic*			m_pStcLoginPic;	//login��ͼ

	VOID	RefreshUrlPic();
	VOID   OpenPassportUpdate();
	VOID	SendUnrealLoginInfo();
	VOID	SendChooseUrlMsg(INT index);
	VOID   UrlModeHideWedget( BOOL bHide );
	VOID   OpenPassportLog(string sLogInfo);
	VOID	DownloadFunction();	// ����
	VOID	RedownloadFunction();	// ������
	BOOL	NewHttpDownload( tstring tsPath, INT nIndexID );
	BOOL	NewHttpDownloadPic( tstring tsPath, INT nIndexID );
	BOOL	IsFinishAllDownload();
	DWORD OnFinishCloseLoading(tagGameEvent* pGameEvent);
	VOID	HideNormalLoginUI(BOOL isHide);	//���س����½��ذ�ť��
	//------open passport end

	//������������½����http����
	void SendYunLianHttpPost();
	//�رո��ֵ�������½
	void CloseOtherLogin();
};


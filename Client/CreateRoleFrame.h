#pragma once
#include "..\WorldDefine\selectrole.h"
#include "..\WorldDefine\ItemDefine.h"
#include "CreateRolePreviewData.h"


enum EGUI_Mouse_Event
{
	EMouse_LeftDown = EGUIE_User + 110,		//������һֱ������ת��ť(ԭ�¼���û�����һֱ���µ��¼�)
};

class Player;
class NetSession;
/** \class CreateRoleFrame
\brief ������ɫ���
*/
class CreateRoleFrame :
	public GameFrame
{
public:
	enum ECRState
	{
		ECRState_Cartoon = 0,				//Ƭͷ����
		ECRState_Inputing,					//�ȴ��������������ѡ����������
		ECRState_WaitRepose,				//�ȴ���������Ӧ
		ECRState_PlayAction,				//����������ݺ󲥷�һ�ζ���
		ECRState_Close,						//�ر�
		ECRState_End,						
	};

	enum EStep
	{
		EStep_EnterCombat,	  //����ս��
		EStep_HandUpWeapon,   //������
		EStep_OpenEquip,      //չ��װ�������Σ�
		EStep_Fighting,		  //ս��״̬
		EStep_CloseEquip,     //�ر�װ�������Σ�
		EStep_HandDownWeapon, //������
		EStep_End,			  //����,����վ������
		EStep_Over			  //һ�鶯������
	};

	enum ECreateRolePreview	  //װ��Ԥ���õ�����־��������������͵�װ��
	{
		ECRP_Null = -1,
		ECRP_Sword = 0,		 //��
		ECRP_Blade,			 //��
		ECRP_Wand,			 //����
		ECRP_Bow,			 //��
		ECRP_Organ,			 //����
		ECRP_LongShou,		 //£��
		ECRP_Qin,			 //��
		ECRP_Amor,			 //����
		ECRP_ClothAmor,		 //����
		ECRP_FashionOne,	 //ʱװҼ
		ECRP_FashionTwo		 //ʱװ��
	};
 
public:
	CreateRoleFrame(void);
	~CreateRoleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID Render3D();

	EPreChapStatus GetChapterStatus() { return m_eStatus; }

	//��ʼ������ɫ
	void BeginCreateRole();

	//����������ɫ
	void EndCreateRole();

	//��ý�ɫ���������
	ECRState GetCurState()const { return m_curState; }

	//���շ�������Ϣ(��������)
	DWORD NetRecvCreateRole(tagNS_CreateRole* pMsg, DWORD pPrama);

private:

	//������Ĵ���ת��������������
//	ECreateRolePreview ParseEquipSelect( GUIWnd* pWnd );

	//�����л��¼� ��������
	void ProcessEquipPreview( ECreateRolePreview eCreateRole );

	//����������������ʵ�����
	void PreviewSkeletonHelp( ECreateRolePreview eCreateRole );

	//Ԥ��ʱ ��������Ч�ĸ�������
	void PrepareEffectSkeleton( const tagCreateRolePreviewItem* pCfgItem, ECreateRolePreview ePreview,ECreateRolePreview eLeftPreview, BOOL bPlayCombat = TRUE );

	//ˢ�µ�ʱ����ô˷�����������Ч�Ͷ���
	void PlayEquipSkeleton();

	/** �õ���Ч������
		\ param  index ��Ϊֻ������������չ��Ч������ ������չ��Ч index ȡֵ��Χ�� [0-2]
		\ param  pBtye ��������Ч�����ֱ�ţ�pByte ��һ����Чָ�룬�͸�ֵΪ������Ч�ı��
	*/
	tstring GetEffectName( ECreateRolePreview eEquipType, BYTE index, LPBYTE pByte = NULL );

	//״̬ת������
	void SwitchToState(ECRState nextState);

	DWORD EventHandler(tagGUIEvent* pEvent);

	//�������������������û�з���������Ϣ��ȷ����ť�ſ���
	BOOL CheckInputedName(); 

	//������ý�ɫ����
	void RandAvatarAtt(tagAvatarAtt& att, bool bRandAll=false);

	//������������
	void UpdateAttUI(bool bSave);

	void Clear();

	//������������ת��ť
	void CheckMouseLeftDown(float deltaTime);

	//��ɫ��ת
	void RoleRotate(float deltaTime);

	//�Ƿ�ʰȡ�˽�ɫ
	bool IsPickRole(int mouseX, int mouseY);

	void CanRolerAnimation(float time);

	void RefreshCamera(bool bFar);

	VOID FilterString(tstring& str);

	VOID RefreshCareerIntro();

	VOID ShowCareerEquip( INT nCareerType );
	void CreateBackGroud( INT nCareer );

    //void WeaponIntro(ECreateRolePreview eWeaponType);	 //��ʾ����˵��ҳ��

    //INT  PageMaxNum(ECreateRolePreview eWeaponType);

    //bool WeaponIntroEnvent(tagGUIEvent* pEvent );   //����˵��ҳ��ʱ����Ӧ


private:

	TSFPTrunk<CreateRoleFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<VarContainer>			m_pVarContainer;
	TObjRef<InputDX8>				m_pInput;

	CreateRolePreviewData			m_previewData;		//Ԥ�������ļ�
	EStep							m_step;				//����Ԥ���Ķ����׶�
	DWORD							m_dwStartTime;		//��ʱ
	tstring							m_szEnterTrackName; //����ս��
	tstring							m_szLeaveTrackName;
	bool							m_bPlayPreview;		//�Ƿ񲥷�Ԥ������
	ECreateRolePreview				m_eEquipType;		//��ǰ��������������
	ECreateRolePreview				m_eLeftEquipType;		//��ǰ��������������

	GUIWnd*							m_pWnd;
	GUIWnd*							m_pWndCaption;
	GUIEditBox*						m_pName;			//��������ؼ�
	GUIStatic*						m_pStcErrHint;		//������ʾstatic
	GUIStatic*						m_pStcSex;			//
	GUIFrameMutex*					m_pSexMutex;		//�Ա𻥳�ؼ�
	GUIPushButton*					m_pMalePBtn;		//����pushbutton
	GUIPushButton*					m_pFmalePBtn;		//Ů��pushbutton
	GUIPushButton*					m_pStatusPBtn[EPCS_End];	//�������pushbutton

	GUIButton*						m_pLBtnHType;		//������ť
	GUIButton*						m_pRBtnHType;		//�����Ұ�ť
	GUIStatic*						m_pStcHType;		//���ͺ���ʾstatic

	GUIButton*						m_pLBtnHColor;		//��ɫ��ť
	GUIButton*						m_pRBtnHColor;		//��ɫ�Ұ�ť
	GUIStatic*						m_pStcHColor;		//��ɫ����ʾstatic

	GUIButton*						m_pLBtnFace;		//�沿��ť
	GUIButton*						m_pRBtnFace;		//�沿�Ұ�ť
	GUIStatic*						m_pStcFace;			//�沿����ʾstatic

	GUIButton*						m_pLBtnFDetail;		//�沿ϸ����ť
	GUIButton*						m_pRBtnFDetail;		//�沿ϸ���Ұ�ť
	GUIStatic*						m_pStcFDetail;		//�沿ϸ�ں���ʾstatic

	GUIButton*						m_pLBtnDress;		//��ʽ��ť
	GUIButton*						m_pRBtnDress;		//��ʽ�Ұ�ť
	GUIStatic*						m_pStcFDress;		//��ʽ����ʾstatic

	GUIButton*						m_pBtnOK;			//ȷ��
	GUIButton*						m_pBtnRandom;		//���
	GUIButton*						m_pBtnCancel;		//����

	GUIWnd*							m_pWnd_Control;		//
	GUIButton*						m_pLBtnRotate;		//����ת��ť
	GUIButton*						m_pRBtnRotate;		//����ת��ť
	GUIPushButton*					m_pPBtn_Push;
	GUIPushButton*					m_pPBtn_Pull;


	//GUIButton*						m_pBtnSword;		//��
	//GUIButton*						m_pBtnBlade;		//��
	//GUIButton*						m_pBtnWand;			//����
	//GUIButton*						m_pBtnBow;			//��
	//GUIButton*						m_pBtnOrgan;		//����
	//GUIButton*						m_pBtnLongShou;		//£��
	//GUIButton*						m_pBtnQin;			//��
	//GUIButton*						m_pBtnAmorSuit;		//������װ
	//GUIButton*						m_pBtnClothAmorSuit;//������װ
	//GUIButton*						m_pBtnFashion1;		//ʱװ1
	//GUIButton*						m_pBtnFashion2;		//ʱװ2

	ECreateRolePreview              m_ECurWeapon;       //��ǰѡ������� 
	//INT                             m_iMaxTextNum;		//����˵���������ҳ�� ��ֵΪ1
	//INT                             m_iCurTextNum;		//����˵�����ڵ�ǰҳ�� ��ֵΪ1
	//GUIWnd*							m_pWndWeaponIntro;  //����˵������
	//GUIStatic*						m_pStcIntroNum;     //����˵��ҳ����ʾ
	//GUIStaticEx*					m_pStcIntroText;    //����˵���ı���ʾ
	//GUIButton*						m_pBtnPreIntro;     //����˵��ҳǰһҳ��ť
	//GUIButton*						m_pBtnNextIntro;    //����˵��ҳ��һҳ��ť
    //TMap<INT CurTextMun, tstring *ptstr>   m_WeaponIntroMap; 
	   //              // ����˵��map��first��ʾ��ǰҳ��secondΪ��ǰҳ���text
	
	GUIWnd		*m_pWndJob;
	GUIButton	*m_pBtnShenBing;	// ���
	GUIButton	*m_pBtnXuanLing;	// ����
	GUIButton	*m_pBtnQianKun;		// Ǭ��
	GUIButton	*m_pBtnXianYin;		// ����
	GUIStaticEx *m_pStcIntor;		// ְҵ����
	GUIScrollBar *m_pScrollBar;
	ECRState						m_curState;			//��ǰ״̬
	float							m_stateTime;		//��ǰ״̬������ʱ��
	bool							m_bRotate;			//�����Ƿ���ת
	bool							m_bLRotate;			//��������ת
	bool							m_bRRotate;			//��������ת
	EPreChapStatus					m_eStatus;			//�������
	bool							m_bSentCreate;		//�Ƿ��Ѿ����͹�������ɫ��Ϣ,��ֹ�ظ�����
	tagAvatarAtt					m_avatarAtt;		//��ɫ����
	tagAvatarEquip					m_avatarEquip;      //��ɫװ��������ʱװ��
	float							m_fRoleYaw;			//��ɫ��ת�ĺ�ƫֵ(��Y��)
	Player*							m_pPlayer;			//��ɫ	
	INT								m_nRoleNameLength;	//������������󳤶�
	bool							m_bPickRole;		//�Ƿ�ʰȡ�˽�ɫ
	BOOL							m_bAnimation;
	float							m_fRolerTimer;
	float							m_fAnimDelay;

	ResTexture						*m_pTex;			// ְҵ����ͼ
	RECT							m_rectSrc;
	RECT							m_rectDest;

	Vector3 						PREROLE_POS;
	Vector3 						LookPos;		
	Vector3 						Lookat;		
	Vector3 						LookPosNear;	
	Vector3 						LookatNear;	

};

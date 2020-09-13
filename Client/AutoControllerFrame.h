#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "NPC.h"
#include "Role.h"
#include "RoleMgr.h"

struct tagNS_UseZhanDouFu;
struct tagNS_ZhanDouFuTimeOut;
struct tagSpecItemEvent;
struct tagNS_SyncAutoFightState;
struct tagUpdateLPSkillEvent;
struct tagRolePickEvent;
class SkillButton;
struct tagHitTargetEvent;
struct tagShowDeadMenuEvent;
struct tagNS_RoleExpChange;

const INT MAX_SKILL         =   6;
const INT MAX_ITEM			=   2;


enum EAutoState
{
	EAS_NONE	  = 0,	//���Զ�ս��״̬
	EAS_COMBAT	  = 1,	//ս��
	EAS_SEARCHING = 2,	//����
	EAS_NOT_ALL_AUTO = 3,	//�ر����Զ�ѡ�ֹ���
	EAS_PATHFING	= 4,	//Ŀ�걻�ڵ�ס·����������
};


struct tagAutoSkillData
{
	DWORD	dwSkillID;	//����ID
	INT		nTimeDelay;	//ʱ���ӳ�(Ĭ��1����)
	DWORD     nCurrentTime;	//��ǰʱ��
	bool	bIsActive;	//�Ƿ��Ѿ�����(Ĭ�ϷǼ���)
	tagAutoSkillData():dwSkillID(0),nTimeDelay(1000),nCurrentTime(0),bIsActive(false){}
	void ClearSkillData()
	{
		dwSkillID = 0;
		nTimeDelay = 1000;
		nCurrentTime = 0;
		bIsActive = false;
	}
};

struct tagAutoItemData
{
	DWORD			dwItemID;		//��ƷID
	EItemConType	eType;	//��Ʒλ��
	INT16			n16Index;//�����е�λ��
	INT				nTimeDelay;		//�趨ʱ���ӳ�
	DWORD			nCurrentTime;	//��ǰʱ��
	INT				nUsePercent;	//�ٷֱ�
	bool			bIsActive;		//�Ƿ񼤻�
	INT64			n64ID;
	tagAutoItemData():dwItemID(0),n16Index(0),nTimeDelay(1000),nCurrentTime(0),nUsePercent(50),bIsActive(false),n64ID(0){
		eType = EICT_Bag;
	}
	void ClearItemData()
	{
		dwItemID = 0;
		nTimeDelay = 1000;
		nUsePercent = 50;
		nCurrentTime = 0;
		n16Index = 0;
		eType = EICT_Bag;
		bIsActive = false;
		n64ID = 0;
	}
};

class AutoControllerFrame :
	public GameFrame
{
public:
	AutoControllerFrame(void);
	~AutoControllerFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	bool IsAutoCombating()	{  return !( EAS_NONE == eState);	}

private:
	BOOL OnWnd_AutoCombat(tagGUIEvent* pEvent);
	BOOL OnWnd_Setting(tagGUIEvent* pEvent);
	BOOL OnWnd_Min(tagGUIEvent* pEvent);

	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnShowDeadMenuEvent(tagShowDeadMenuEvent* pEvent);
	/**
	��ʼ�Զ�ս��
	*/
	VOID StartAutoCombating();	
	/**
	�����Զ�ս��
	*/
	VOID EndAutoCombating();	
	/**
	ˢ��ս������UI
	*/
	VOID RefreashCombatUI();
	/**
	����ս������UI
	*/
	VOID SetDefaultCombatUI();
	/**
	������һ������
	*/
	bool SearchNextMonster();
	/**
	ʹ�ü���
	*/
	bool UseAutoSkill();
	/**
	ʹ����Ʒ
	*/
	VOID UseAutoItem();
	/**
	��ȥ����
	*/
	VOID GoBack();
	/**
	��ȥ�ϴο����ֵĵط�
	*/
	VOID GoBackToLastPositionSeeMonster();
	/**
	ȥ���ﵱǰ��λ��
	*/
	VOID GoToMonsterCurrentPosition();
	/**
	�����Զ�ս���߼�
	*/
	VOID UpdateCombat();
	/**
	Ĭ������UI
	*/
	VOID SetDefaultSettingUI();
	/**
	��������
	*/
	VOID ReadSettingFromFile();
	/**
	��������
	*/
	VOID SaveSetting();
	VOID SaveSettingToLocalFile();

	VOID RefreshSettingToUI();

	/**
	������ȴʱ��
	*/
	VOID UpdateCDtimes();

	VOID AutoBaoQi();

	DWORD OnOpenAutoHook(tagSpecItemEvent* pEvent);
	
	DWORD OnNS_UseZhanDouFu(tagNS_UseZhanDouFu* pMsg,DWORD pParam);
	DWORD OnNS_ZhanDouFuTimeOut(tagNS_ZhanDouFuTimeOut* pMsg,DWORD pParam);
	DWORD OnNS_SyncAutoFightState(tagNS_SyncAutoFightState* pMsg,DWORD pParam);

	DWORD OnNS_RoleExpChange(tagNS_RoleExpChange* pNetCmd, DWORD dwParam);

	VOID BlockCheck();
	VOID IdleCheck();

	bool MonsterIsValid(DWORD dwID);

	DWORD OnRolePickEvent( tagRolePickEvent* pEvent );
	DWORD OnHitTargetEvent(tagHitTargetEvent* pEvent);

	VOID UpdateUsetimeInfo();
private:

	TSFPTrunk<AutoControllerFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	SkillButton*					m_pBtSkills[MAX_SKILL]; //����ͼ��
	ItemButton*						m_pBtItems[MAX_ITEM];	//��Ʒͼ��

	GUIButton*						m_pBnStartCombat;		//��ʼս��
	GUIButton*						m_pBnAutoSetting;		//����

	GUIButton*						m_pBnMinSize;			//��С��
	GUIButton*						m_pBnClose;				//�ر�


	GUIWnd*							m_pWnd;					//������
	GUIWnd*							m_pWndSetting;			//set����
	GUIWnd*							m_pWndMin;				//��С������
	GUIButton*						m_pBnMin;				//��С����ť

	EAutoState						eState;					//ս��״̬

	tagAutoSkillData				m_skillData[MAX_SKILL];	//������Ϣ�б�
	tagAutoItemData					m_itemData[MAX_ITEM];	//��Ʒ��Ϣ�б�

	Vector3							m_staticPoint;			//�����ֵĵ�

	bool							m_bAutoSelectMonster;	//�Զ�ѡ��
	bool							m_bAutoBaoQi;			//�Զ�����
	bool							m_bPointStand;			//������

	DWORD							m_LockTargetID;			//�����Ĺ���Ŀ��

	DWORD							m_dwCurrentSkillTime;
	DWORD							m_dwTimeMark;

	//Setting ���

	GUIPushButton*					m_pPbItem0Active;	//�����Զ�ʹ������ҩ
	GUIPushButton*					m_pPbItem1Active;	//�����Զ�ʹ������ҩ

	GUIEditBox*						m_pEd_Item0Percent;	
	GUIEditBox*						m_pEd_Item1Percent;

	GUIPushButton*					m_pPbSkill0Active;	//�����Զ�ʹ�ü���0
	GUIPushButton*					m_pPbSkill1Active;	//�����Զ�ʹ�ü���1
	GUIPushButton*					m_pPbSkill2Active;	//�����Զ�ʹ�ü���2
	GUIPushButton*					m_pPbSkill3Active;	//�����Զ�ʹ�ü���3	
	GUIPushButton*					m_pPbSkill4Active;	//�����Զ�ʹ�ü���4
	GUIPushButton*					m_pPbSkill5Active;	//�����Զ�ʹ�ü���5
	
	GUIPushButton*					m_pPbAutoSelectActive;	//�����Զ�ѡ��
	GUIPushButton*					m_pPbAutoBaoQiActive;	//�����Զ�����
	GUIPushButton*					m_pPbStaticPointActive;	//���ö�����

	GUIEditBox*						m_pEd_Item0Time;	
	GUIEditBox*						m_pEd_Item1Time;

	GUIEditBox*						m_pEd_Skill0Time;	
	GUIEditBox*						m_pEd_Skill1Time;
	GUIEditBox*						m_pEd_Skill2Time;	
	GUIEditBox*						m_pEd_Skill3Time;
	GUIEditBox*						m_pEd_Skill4Time;	
	GUIEditBox*						m_pEd_Skill5Time;

	GUIButton*						m_pBnSaveSettings;
	GUIButton*						m_pBnCloseSetting;

	GUIStatic*						m_pStLeftTime;
	GUIStatic*						m_pStStartTime;
	GUIStatic*						m_pStExpHaveGot;

	bool							m_bFristOpen;

	INT64							m_n64ID;

	DWORD							m_dwTimeSpace;				//����ʱ���ʱ��
	Vector3							m_LastFightPoint;			//�ϴδ�ֵĵص�
	DWORD							m_dwTargetHP;				//����HP
	list<DWORD>						m_lsBlockIDList;				//��������ID

	DWORD							m_dwIdleTime;				//��ɫ����״̬��ʱ��

	DWORD							m_dwTimeEclipse;				//�һ����ʱ��

	DWORD							m_dwPathfingTimeEclipse;		//Ѱ·ʱ��

	DWORD							m_dwUseTime;		// ��ʼʹ���˵�ʱ�䣬��Ϊ��λ
	INT64								m_dwUseExp;		// ��ʼʹ��ʱ��õľ���
	tagDWORDTime				m_sGetTime;		// ս������õ�ʱ��
	DWORD							m_dwLeftTime;
};

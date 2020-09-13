#pragma once
#include "..\WorldDefine\RoleDefine.h"
struct tagFashionEvent;
struct tagNS_SynTotalStrength;

struct tagChangeFashionEvent;
struct tagNS_RoleClearAttPoint;
struct tagNS_SyncStartVipTime;
class LocalPlayer;
const DWORD BUTTON_DELAY_MS = 3000;	/*< ��ť�����ӳ�,��λ:���� */
const int MAX_PLAYER_RESIST = 1000;
const int MIN_PLAYER_RESIST = -1000;

enum EDisplayProperty
{
	EDP_Physique = 0,	// ���
	EDP_Strength = 1,	// ����
	EDP_Pneuma   = 2,		// Ԫ��
	EDP_InnerForce = 3,	// ����

	EDP_F_End = 4,		//F�ƻ�������

	EDP_Agility = 4,	// ��
	EDP_Technique = 5,  // ����
	
	EDP_End = 5,
};

enum EDisplayPropertyEx
{
	EDPE_WeaponDamage = 0,		// ��С�����˺���todo ��������˺�
	EDPE_WuHun = 1,		// �����˺�
	EDPE_ArmDefine = 2,		// ���׷���
	EDPE_MeleeDamage = 3,			// ������
	EDPE_MagicDamage = 4,			// ��������
	EDPE_MeleeDefine = 5,			// �������
	EDPE_MagicDefine = 6,			// ��������
	EDPE_DamageSkill = 7,			// ��׼
	EDPE_DefineSkill = 8,		// ����
	EDPE_MoveSpeed = 9,			// �ƶ��ٶ�
	EDPE_Toughness = 10,		// ����
	EDPE_Energy = 11,			// ����
	EDPE_Luck = 12,				// ����
	EDPE_InternalInjury = 13,	// ����
	EDPE_Morality = 14,			// ����

	EDPE_JiaShen = 15,			// �˺�����
	EDPE_JianMian = 16,			// �˺�����
	EDPE_DiKang = 17,			// �˺��ֿ�
	EDPE_QiangHua = 18,			// ����Ч��ǿ��

	EDPE_Regain_Addtion = 19,			// ����
	EDPE_Crit_Rate = 20,			// ���Ⱪ����
	EDPE_Crit_Amount = 21,			// ���Ⱪ����

	EDPE_Ele_Injery =	22,	// Ԫ���˺�
	EDPE_Ele_Pierce = 23,		// Ԫ�ش�͸
	EDPE_Ele_Res = 24,	// Ԫ�ؿ���
	EDPE_Ignore_Attack = 25,	// ���ӹ���
	EDPE_Ignore_Defend = 26,	// ���ӷ���

	EDPE_Mana_combustion		=	27,		// ����ȼ��
	EDPE_Mana_combustion_resistance			=	28,		// ����ȼ�յֿ�
	EDPE_ExDamage			=	29,		// �����˺�
	EDPE_ExJiaCheng		=	30,		// ����ӳ�
	EDPE_Ren						=	31,						//����
	EDPE_Crit_Resist				=	32,		// ��������
	EDPE_rebound						=	33,		// �̶�����
	EDPE_rebound_ratio					=	34,		// ��������
	EDPE_rebound_immune				=	35,		// ��������
	EDPE_slow_up					=	36,		// �����˺�ǿ��

	EDPE_End,
	EDPE_WeaponDamage0,		// �����˺����ֵ
	EDPE_WuHun0,			// �������ֵ
	EDPE_ArmDefine0,		// ���׷�������ֵ
	EDPE_MoveSpeed0,		// �ƶ��ٶ�����ֵ
	EDPE_Morale0,			// ʿ������ֵ
	EDPE_InternalInjury0,	// ��������ֵ
	EDPE_Luck0,				// ��Ե����ֵ
	EDPE_Energy0,			// �������ֵ
	EDPE_Toughness0,		// �־�������ֵ
	EDPE_MeleeDamage0,		// �⹦��������ֵ
	EDPE_MeleeDefine0,		// �⹦��������ֵ
	EDPE_MagicDamage0,		// �ڹ���������ֵ
	EDPE_MagicDefine0,		// �ڹ���������ֵ
	EDPE_DamageSkill0,		// ������������ֵ
	EDPE_DefineSkill0,		// ������������ֵ
	EDPE_Morality0,			// ��������ֵ
	EDPE_Health0,			// �������ֵ
	EDPE_Power0,			// �������ֵ
	//EDPE_WeaponDamage,		// �����˺���Сֵ
	//EDPE_WuHun,				// ���
	//EDPE_ArmDefine,			// ���׷���
	//EDPE_MoveSpeed,			// �ƶ��ٶ�
	//EDPE_Morale,			// ʿ��
	//EDPE_InternalInjury,	// ����
// 	EDPE_WeaponDamage0,		// �����˺����ֵ
// 	EDPE_WuHun0,			// �������ֵ
// 	EDPE_ArmDefine0,		// ���׷�������ֵ
// 	EDPE_MoveSpeed0,		// �ƶ��ٶ�����ֵ
// 	EDPE_Morale0,			// ʿ������ֵ
// 	EDPE_InternalInjury0,	// ��������ֵ
	//EDPE_Luck,				// ��Ե
	//EDPE_Feature,			// ����
	//EDPE_Command,			// ͳ��
	//EDPE_Energy,			// ����
	//EDPE_Morality,			// ����
	//EDPE_Cultivate,			// ��Ϊ
	//EDPE_Luck0,				// ��Ե����ֵ
	//EDPE_Feature0,			// ��������ֵ
	//EDPE_Command0,			// ͳ������ֵ
	//EDPE_Energy0,			// �������ֵ
	//EDPE_Morality0,			// ��������ֵ
	//EDPE_Cultivate0,		// ��Ϊ����ֵ
	//EDPE_Health,			// ����
	//EDPE_Power,				// ����
	//EDPE_Toughness,			// �־���
	//EDPE_MeleeDamage,		// �⹦����
	//EDPE_MeleeDefine,		// �⹦����
	//EDPE_MagicDamage,		// �ڹ�����
	//EDPE_MagicDefine,		// �ڹ�����
	//EDPE_DamageSkill,		// ��������
	//EDPE_DefineSkill,		// ��������
	//EDPE_Hit,				// ����
	//EDPE_Dodge,				// ����
	//EDPE_Health0,			// �������ֵ
	//EDPE_Power0,			// �������ֵ
// 	EDPE_Toughness0,		// �־�������ֵ
// 	EDPE_MeleeDamage0,		// �⹦��������ֵ
// 	EDPE_MeleeDefine0,		// �⹦��������ֵ
// 	EDPE_MagicDamage0,		// �ڹ���������ֵ
// 	EDPE_MagicDefine0,		// �ڹ���������ֵ
// 	EDPE_DamageSkill0,		// ������������ֵ
// 	EDPE_DefineSkill0,		// ������������ֵ
	//EDPE_Hit0,				// ��������ֵ
	//EDPE_Dodge0,			// ��������ֵ
	//EDPE_Weary,             // ƣ��ֵ
	//EDPE_End,
	//
};

enum EDisplayResist
{
	EDR_BleedResist,		// ��Ѫ�˺�����
	EDR_BruntResist,		// ����˺�����
	EDR_BangResist,			// �ػ��˺�����
	EDR_PoisonResist,		// �����˺�����
	EDR_ThinkerResist,		// �����˺�����
	EDR_InjuryResist,		// �����˺�����
	EDR_OrdinaryResist,		// ��ͨ�˺�����

	EDR_End,
};

enum EDisplayTemporary
{
	EDT_Origin_Health,		// ԭʼ����
	EDT_Origin_Power,		// ԭʼ����
	EDT_Origin_ExDamge,		// ԭʼ�⹦����
	EDT_Origin_ExDefine,	// ԭʼ�⹦����
	EDT_Origin_InDamage,	// ԭʼ�ڹ�����
	EDT_Origin_InDefine,	// ԭʼ�ڹ�����
	EDT_Origin_DmgSkill,	// ԭʼ��������
	EDT_Origin_DefSkill,	// ԭʼ��������
	EDT_Origin_Hit,			// ԭʼ����
	EDT_Origin_Dodge,		// ԭʼ����
	EDT_Origin_Toughness,	// ԭʼ�־���

	EDT_Origin_End,
};

const DWORD ORDINARY_COLOR = Color4ub(255, 243, 236, 164);// ԭʼ��ɫ
const DWORD MINUS_RED_COLOR = Color4ub(255, 247, 3, 15);// ����ʱ�ĺ�ɫ
const DWORD PLUS_GREEN_COLOR = Color4ub(255, 194, 246, 58);// ����ʱ����ɫ
#define GETTRUECOLOR(n) (0 > n ? MINUS_RED_COLOR : \
	( 0 < n ? PLUS_GREEN_COLOR : ORDINARY_COLOR))

class NetSession; 
class ActiveProgress;
/** class	StateFrame
	brief	״̬����ģ��
	remarks ��ʾ�͹�����ҵ�ǰ����������Ϣ
*/ 
class StateFrame :
	public GameFrame
{
public:
	StateFrame(void);
	~StateFrame(void);
	
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID UpdatePalyerAtt();

	VOID AlignAttEXShowWnd();
	
private:

	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	DWORD OnMsgClearPoint(tagMsgBoxEvent* pGameEvent);
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnNetSynTotalStrength(tagNS_SynTotalStrength *pNetCmd, DWORD);
	

	VOID UpdateAttribute(ERoleAttribute eType);
	
	VOID SetPlayerName(LPCTSTR szName);
	VOID SetPlayerLevel(INT nLevel);
	VOID SetPlayerClass(LPCTSTR szClass);
	VOID SetPlayerGuild(LPCTSTR szGuild);

	VOID SetPlayerOverplus(INT nValue);
	VOID SetBiddenPoint(ERoleAttribute eType, INT nValue);
	VOID AddPreBidPoint(ERoleAttribute eType, INT nInc);
	VOID RemoveBidPoint();
	VOID UpdateBidAttribute();

	VOID SetPlayerAtt(EDisplayProperty eType, INT nValue, DWORD dwColor = ORDINARY_COLOR);
	VOID SetPlayerAttEx(EDisplayPropertyEx eType, INT nValue, DWORD dwColor = ORDINARY_COLOR);
	VOID SetPlayerResist(EDisplayResist eType, INT nValue);

	VOID SendNetBidAtt();

	ERoleAttribute TransBtn2Att(EDisplayProperty eType);

    // �⹦�˺�����ӳ�
    int CalExAttackResist(int src);
    // �ڹ��˺�����ӳ�
    int CalInAttackResist(int src);

	DWORD NetRoleClearAttPoint(tagNS_RoleClearAttPoint* pNetCmd, DWORD);
	DWORD OnNS_SyncStartVipTime(tagNS_SyncStartVipTime* pNetCmd, DWORD);
	

	VOID UpdateSettingsOfLocalPlayer(LocalPlayer* plp);

	float GetWearyUpperLimit();
private:
	TSFPTrunk<StateFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		    m_pSession;

	// GUI�ؼ�
    GUIWnd*                     m_pWndState;
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	GUIStatic*					m_pStcAtt[EDP_F_End];	//1�����ԣ��ӵ�
	GUIStatic*					m_pStcAttEx[EDPE_End];	//2�����Ի��п���
	//GUIStatic*					m_pStcResPic[EDR_End];
	//GUIStatic*					m_pStcResValue[EDR_End];
	GUIStatic*					m_pStcOverplus;
	GUIStatic*					m_pStcOverplusText;//"ʣ�����"����
	GUIButton*					m_pBtnAtt[EDP_F_End];
	GUIFrameMutex*				m_pFmSuitOpt;
	GUIPushButton*				m_pPbnDisplayAtt;
	GUIPushButton*				m_pPbnDisplayEquip;
	GUIPushButton*				m_pPbnDisplayFashion;
	GUIStatic*					m_pStcPlayerName;
	GUIStatic*					m_pStcPlayerGuild;
	GUIStatic*					m_pStcPlayerLevel;
	GUIStatic*					m_pStcPlayerClass;
	GUIStatic*				    m_pStcAttWeary;     //ƣ�����Դ��ڿؼ�
	GUIStatic*				    m_pStcAttWearyFont; //ƣ��ֵ������ʾ�ؼ�
	GUIProgress*		        m_pProPlayerWeary;  //ƣ����ʾ�� 
	GUIButton*					m_pBtnSure;
	GUIButton*					m_pBtnCancel;
	GUIButton*					m_pBtnCleanPoints;	//ϴ��
	GUIWnd*						m_pWndAtt;
	GUIWnd*						m_pWndAttEx;
	GUIWnd*						m_pWndAttIn;

	GUIWnd*						m_pWndRes;
	GUIButton*					m_pBtnDetail;

	GUIWnd*						m_pWndAttExShow;	// ������չ����
	GUIButton*					m_pBtnAttExShow;	// ������չ��ť
	GUIButton*					m_pBtnAttExShowClose;	// ������չ�رհ�ť

	bool						m_isSwappingSuitPattern;
	bool                        m_bIsClick;

	// ��ʱ����
	INT							m_Temporary[EDT_Origin_End];
	INT							m_nPreBid[X_ERA_ATTA_NUM];// ԤͶ��
	INT							m_nBidden[X_ERA_ATTA_NUM];// ��Ͷ��
	INT							m_nOgnAtt[X_ERA_ATTA_NUM];// ����ɳ���
	INT							m_nOverplus;			  // ��ǰ��Ͷ����
	INT							m_nInternalInjury;		//����ֵ
	INT							m_TempAttEquipAdd[ERA_End];

	GUIStatic*					m_pStcToughnes;
	GUIStatic*					m_pStcToughnesValue;

	GUIStatic*				    m_pStcAttHP;     //Ѫ���Դ��ڿؼ�
	GUIStatic*				    m_pStcAttHPFont; //Ѫֵ������ʾ�ؼ�
	ActiveProgress*		        m_pProPlayerHP;  //Ѫ��ʾ�� 

	GUIStatic*				    m_pStcAttMP;     //�����Դ��ڿؼ�
	GUIStatic*				    m_pStcAttMPFont; //��ֵ������ʾ�ؼ�
	ActiveProgress*		        m_pProPlayerMP;  //����ʾ�� 

	GUIPushButton*				m_pPbnShowHead;		//��ʾͷ��
	GUIPushButton*				m_pPbnShowPiFeng;	//��ʾ����

	tagDisplaySet				m_curDisplaySet;              // ��ǰ��װ����ʾ����

	bool						m_bDontHaveToSentEvent;		// �Ƿ���Ҫ�����л�װ��ģʽ�¼�

	// vip
	GUIStatic*					m_pStcSilverVIP;
	GUIStatic*					m_pStcSilverVIPLeftTime;
	GUIStatic*					m_pStcGoldVIP;
	GUIStatic*					m_pStcGoldVIPLeftTime;
	GUIStatic*					m_pStcPlatinumVIP;
	GUIStatic*					m_pStcPlatinumVIPLeftTime;

};

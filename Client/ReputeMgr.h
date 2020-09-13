#pragma once
#include "ProgressEx.h"
#include "..\WorldDefine\reputation.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_soul.h"

struct tagNS_GetRoleInitState_Reputation;
struct tagNS_RoleReputeChange;
struct tagNS_SoulCrystalGrow;
struct tagNS_SoulCrystalRenew;

//! �����׶���ɫ
enum EPrgrssColor
{
	EPC_RED			=	0,
	EPC_YELLOW		=	1,
	EPC_GREEN		=	2,
	EPC_BLUE		=	3,
	EPC_ORANGE		=	4,

	EPC_END			=	5
};

enum EReputeType
{
	ERPUT_God			=	0,
	ERPUT_Evil		=	1,
	ERPUT_End,
};
#define MIsValidReputeType(eType) (eType >= ERPUT_God && eType < ERPUT_End) 
//! \struct tagReputeLevel
//! \brief �����ȼ�����
struct tagReputeLevel
{
	EReputationLevel		eLevel;			//!< �����׶�
	EPrgrssColor			eColor;			//!< �����׶���ɫ
	LPCTSTR					szLevel;		//!< �����׶��ı�
	INT						nFrom;			//!< ����ֵ [nFrom, nTo)
	INT						nTo;
	INT						nDiffValue;		//!< �����׶�ֵ = nTo - nFrom
	INT						nMaxContribute;	//!< ����ֵ����
};

// ��ħ��������
struct tagForceSkill
{
	DWORD					dwID;
	EReputeType				eType;
	DWORD					dwClassTypeLimited;
	DWORD					dwReputeLevel;
	DWORD					dwNeedSkill;
	BOOL					bForever;
	DWORD					dwActvNeedSoul;
	DWORD					dwActvNeedMoney;
	DWORD					dwRenewNeedSoul;
	DWORD					dwRenewNeedMoney;
	BOOL					bNeedDel;
};

const INT GODEVIL_SKILL_LEVEL = 6;

//! ����ֵ�б仯
struct tagReputeChange : public tagGameEvent
{
	ECLanType				eClanType;		//!< ��������
	INT						nValue;			//!< ����ֵ
	INT						nChange;		//!< �仯ֵ

	tagReputeChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

//! ��Ϊ����
struct tagBecomeEvent : public tagGameEvent
{
    ECLanType               eClanType;      //!< ��������

    tagBecomeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! �����䱦��������ı�
struct tagTreasureActCountChange : public tagGameEvent
{
    ECLanType               eClanType;      //!< ��������
    INT                     nCount;         //!< ʣ�༤�����
    tagTreasureActCountChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! ���������ȼ��仯 by add xtian 2009-6-15
struct tagReputeLvlChange : public tagGameEvent
{
	 ECLanType					eClanType;      //!< ��������
	 EReputationLevel			eLevel;			//!< �����׶�
	 tagReputeLvlChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		 : tagGameEvent(szEventName, pSenderFrame) {}
};

struct tagHunJingProto
{
	DWORD	dwID;
	INT			nLevel;
	DWORD	dwAttType;
	DWORD	dwAttValue;
};

struct tagHunJingName : public tagHunJingProto
{
	TCHAR     szName[X_LONG_NAME];
	TCHAR     szDesc[X_LONG_NAME];
};

struct tagHunJingLevel
{
	INT			nLevel;	// ��Ҫ�����ĵȼ�
	DWORD	dwHunNum;	// ��Ҫ�Ļ������
	DWORD	dwMoney;	// ��Ҫ�Ľ�Ǯ������
};

//! \class ReputeMgr
//! \brief ����ֵ������
//! \author hyu
class ReputeMgr
{
public:
	ReputeMgr(void);
	~ReputeMgr(void);

	VOID Init();
	VOID Destroy();

	static ReputeMgr* Inst();

	VOID LoadFormFile();
	//! ��ȡ��ǰ����ֵ
	INT GetReputeValue(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? m_nReputeValue[eCtype] : 0;}
    //! ��GetReputeValue����һ�������������ͬ����߻���������ص����������Ǵ�20��ʼ����Ӧʹ�ô˺���
    INT GetReputeValueEx(EReputationType eRtype) {return JDG_VALID(ERT, eRtype) ? m_nReputeValue[eRtype - ERT_BEGIN] : 0;}

	//! ��ȡ��ǰ�����ȼ�
	const tagReputeLevel* GetReputeLevel(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? &m_ReputeLevel[eCtype] : NULL;}

    //! ��õ�ǰ�Ƿ�������
    BOOL IsFame(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? m_bisFame[eCtype] : FALSE;}

    //! ��õ�ǰ�����䱦�������
    INT GetActCount(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? m_nActCount[eCtype] : 0;}

	//! ��������ֵ���������ȼ�
	BOOL UlityCalReputeLevel(tagReputeLevel& ReputeSrc, INT nReputeValue);

	//! ������������ɫ
	VOID UlitySetPrgrssColor( ProgressEx* pProgress, EPrgrssColor eColor );

	//! ��ħ�������
	VOID SendNC_ActiveSoulSkill(DWORD _dwSkillID);
	VOID SendNC_ContinueSoulSkill(DWORD _dwSkillID,INT64 _n64ItemID);
	VOID SetGodRepute(DWORD dwValue){ m_dwGERepute[ERPUT_God] = dwValue;}
	VOID SetEvilRepute(DWORD dwValue){ m_dwGERepute[ERPUT_Evil] = dwValue;}
	VOID SetGodSoul(DWORD dwValue){ m_dwGESoul[ERPUT_God] = dwValue;}
	VOID SetEvilSoul(DWORD dwValue){ m_dwGESoul[ERPUT_Evil] = dwValue;}
	VOID ShowGEMsgCode(DWORD dwErrorCode);

	const tagForceSkill* GetForceSkillInfo(DWORD dwSkillID);
	vector<DWORD> GetLocalPlayerForceSkillList(EReputeType eType,DWORD dwLevel);
	DWORD		  GetForceLevelPoint(INT nLevel);
	DWORD		  GetGEReputeLevel(EReputeType eType);

	DWORD		  GetGERepute(EReputeType eType) const {return MIsValidReputeType(eType) ? m_dwGERepute[eType] : 0;}
	DWORD		  GetGESoul(EReputeType eType) const   {return MIsValidReputeType(eType) ? m_dwGESoul[eType] : 0;}

	tagHunJingName* FindHunJingName(DWORD dwID);
	tagHunJingLevel* FindHunJingLevelInfo(INT nLevel );
private:

	VOID LoadDataFromFile();
	VOID ClearAllData();

	//! ��������ֵ
	VOID SetReputeValue(ECLanType eCtype, INT nValue, INT nChange);

	DWORD OnOpenGodReputeFrame(tagGameEvent* pGameEvent);
	DWORD OnOpenEvilReputeFrame(tagGameEvent* pGameEvent);

	//------------------------------------------------------------------------
	//! ������Ϣ
	//------------------------------------------------------------------------
	
	//! ��ʼ��
	DWORD OnInitReputation(tagNS_GetRoleInitState_Reputation* pNetCmd, DWORD);

	//! ��ʼ�����������������ȼ� by add xtian 2009-6-15
	VOID OnInitRepuLvl(ECLanType eType, INT nReputeValue);

	//! ��ǰ��ɫ���������ȼ������仯ʱ������Ϸ�¼� by add xtian 2009-6-15
	void UlityLocalPlayerRepuLvl( ECLanType eType, INT nReputeValue, INT nChange );

	//! ����ֵ�ı� 
	DWORD OnReputeChange(tagNS_RoleReputeChange* pNetCmd, DWORD);

    //! ����״̬�ı�
    DWORD OnBecomeFame(tagNS_BecomeFame* pNetCmd, DWORD);

    //! �����䱦��������ı�
    DWORD OnActCountChange(tagNS_TreasureActCountChange* pNetCmd, DWORD);

    //! ���幱�״�����ʱ����ʾ
    DWORD OnNS_ClanConUpperLimit(tagNS_ClanConUpperLimit* pNetCmd, DWORD);

	//! ��ħ�������
	DWORD OnNS_ChangeSoul(tagNS_ChangeSoul* pCmd, DWORD);
	DWORD OnNS_ActiveSoulSkill(tagNS_ActiveSoulSkill* pCmd, DWORD);
	DWORD OnNS_ContinueSoulSkill(tagNS_ContinueSoulSkill* pCmd, DWORD);

	DWORD OnNS_SoulCrystalGrow(tagNS_SoulCrystalGrow* pCmd, DWORD);
	DWORD OnNS_SoulCrystalRenew(tagNS_SoulCrystalRenew* pCmd, DWORD);

	TSFPTrunk<ReputeMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	INT							m_nReputeValue[ECLT_NUM];		//!< ��ŵ�ǰ��ɫ����ֵ
	tagReputeLevel				m_ReputeLevel[ECLT_NUM];		//!< ��ŵ�ǰ��ɫ�����׶�
	EReputationLevel			m_eRepuLvl[ECLT_NUM];			//!< ��ŵ�ǰ��ɫ������������ȼ� by add xtian 2009-6-15
    BOOL                        m_bisFame[ECLT_NUM];            //!< �Ƿ�������
    INT                         m_nActCount[ECLT_NUM];          //!< �䱦�������

	map<DWORD,tagForceSkill>	m_mapForceSkill;
	DWORD						m_lstReputeLevel[GODEVIL_SKILL_LEVEL];
	DWORD						m_dwGERepute[ERPUT_End];					// ��ħ����
	DWORD						m_dwGESoul[ERPUT_End];						// ��ħ��
	//! ������ǰ����ɫ
	tstring						m_strPrgssForePic_Red;
	tstring						m_strPrgssForePic_Yellow;
	tstring						m_strPrgssForePic_Green;
	tstring						m_strPrgssForePic_Blue;
	tstring						m_strPrgssForePic_Orange;

	map<DWORD, tagHunJingName>	m_mapHunJingName;
	map<INT, tagHunJingLevel>		m_mapHunJingLevel;
};

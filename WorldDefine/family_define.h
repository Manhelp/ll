//////////////////////////////////////////////////////////////////////////
// ����ϵͳ�ṹ�嶨��
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "RoleDefine.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
// common
//////////////////////////////////////////////////////////////////////////
#define MAX_ItemOrMonsterID				80			// �����������Ʒ��������
#define MAX_FQ_RewardNum				5			// ������Ʒ����
#define MAX_FAMILY_MEMBER				12			// �����ĳ�Ա����
#define MAX_FAMILY_NAME_LEN				7			// ����������󳤶�
#define MIN_CREATE_FAMILY_TEAM_MEMBER	2			// �������������������
#define MAX_FAMILY_QUESTS_NUM			4			// ��������������
#define CAN_GET_REWARD_PROCESS			3000		// ������ɶȴﵽ30%�ļ������ȡ����
#define CAN_GET_ITEM_REWARD_PROCESS		8000		// ������ɶȴﵽ80%�ļ������ȡ��Ʒ
#define SAVE_FAMILY_QUEST_INTERNAL_MIN	5			// ����һ�����壬ÿ��5���ӱ���һ�������������
#define JOIN_FAMILY_REQ_TIMEOUT			60		// �������������ʱ��
#define MAX_FAMILY_REWARD_ITEM_NUM		5			// ������Ʒ����
#define FAMILY_ACTIVE_INCREASE_ONCE		40			// �����Ա��½��Ϸ���Ӽ����Ծ��
#define MAX_FAMILY_ACTIVE						(FAMILY_ACTIVE_INCREASE_ONCE*MAX_FAMILY_MEMBER)
#define MAX_FAMILY_SPIRITE_ADDED_EXP	1000 // ���������ĳɳ������ӵ����ֵ
// ��������
struct tagFamilyAtt 
{
	DWORD 	dwFamilyID;				// ����ID���������Ƶ�CRC��
	DWORD 	dwLeaderID;				// �����峤ID
	DWORD 	dwFounderID;			// ��ʼ��ID
	INT		nActive;				// �����Ծ��
	DWORD 	dwCreateTime;			// ���崴��ʱ��

	tagFamilyAtt()
	{
		ZeroMemory(this, sizeof(*this));
		dwFamilyID = GT_INVALID;
		dwLeaderID = GT_INVALID;
		dwFounderID = GT_INVALID;
	}
	VOID Init(DWORD 	dwFamilyID,			// ����ID���������Ƶ�CRC��
					DWORD 	dwLeaderID,		// �����峤ID
					DWORD 	dwFounderID,		// ��ʼ��ID
					INT			nActive,				// �����Ծ��
					DWORD 	dwCreateTime)	// ���崴��ʱ��
					{
						this->dwFamilyID = dwFamilyID;
						this->dwLeaderID = dwLeaderID;
						this->dwFounderID = dwFounderID;
						this->nActive = nActive;
						this->dwCreateTime = dwCreateTime;
					}
	tagFamilyAtt& operator=(tagFamilyAtt& other)
	{
		dwFamilyID = other.dwFamilyID;
		dwLeaderID = other.dwLeaderID;
		dwFounderID = other.dwFounderID;
		nActive = other.nActive;
		dwCreateTime = other.dwCreateTime;
		return *this;
	}
	VOID SetFamilyActive(INT nFamilyActive)
	{
		if (nFamilyActive > MAX_FAMILY_ACTIVE)
			nActive = MAX_FAMILY_ACTIVE;
		else if (nFamilyActive < 0)
			nActive = 0;
		else
			nActive = nFamilyActive;
	}
};

// �����Ա
struct tagFamilyMember
{
	DWORD	dwRoleID;
	DWORD	dwFamilyID;		// ��������ID���������Ƶ�CRC��
	DWORD	dwJoinTime;		// ����ʱ��
	BOOL	bLeader;		// �Ƿ����峤
	BOOL	bRewarded;	// �Ƿ���ȡ���ϴμ�������Ľ���
	// Ȩ�ޣ�����

	tagFamilyMember()   { ZeroMemory(this, sizeof(*this)); }
	tagFamilyMember(DWORD dwRoleID, DWORD dwFamilyID, DWORD dwJoinTime, BOOL bLeader, BOOL bRewarded)
	{
		this->dwRoleID = dwRoleID;
		this->dwFamilyID = dwFamilyID;
		this->dwJoinTime = dwJoinTime;
		this->bLeader = bLeader;
		this->bRewarded = bRewarded;
	}
	tagFamilyMember& operator=(tagFamilyMember& other)
	{
		this->dwRoleID = other.dwRoleID;
		this->dwFamilyID = other.dwFamilyID;
		this->dwJoinTime = other.dwJoinTime;
		this->bLeader = other.bLeader;
		this->bRewarded = other.bRewarded;
		return *this;
	}
};

// ��Ҽ�����Ϣ
struct tagRoleFamilyInfoBase
{
	DWORD	dwFamilyID;		// ��������ID���������Ƶ�CRC��
	BOOL	bLeader;				// �Ƿ����峤
	tagRoleFamilyInfoBase()
		:dwFamilyID(GT_INVALID), bLeader(FALSE)
	{}
};

// ��Ҽ�����Ϣ
struct tagRoleFamilyInfo: public tagRoleFamilyInfoBase
{
	DWORD				dwRoleID;				// ���ID
	INT						nLevel;						// �ȼ�
	BYTE					bySex;						// 0:girl��1:boy(�����Ժ���չ)
	EClassType          eClassType;             // ר��ְҵ
	INT						nHpMax;				//���Ѫ��
	INT						nHp;				//��ǰѪ��
	INT						nMpMax;
	INT						nMp;
	DWORD				dwMapID;				// ���ڳ���ID
	DWORD				dwLoverID;				// ��żID
	tagRoleFamilyInfo()
		:tagRoleFamilyInfoBase(), dwRoleID(GT_INVALID)
		,nLevel(0),bySex(GT_INVALID),eClassType(EV_Null)
		,nHp(0),nHpMax(0),nMp(0),nMpMax(0),dwMapID(GT_INVALID)
		,dwLoverID(GT_INVALID)
	{
	}
};

// �����������
struct tagJoinFamilyReq
{
	DWORD dwJoinerID;
	DWORD dwMemberID;
	DWORD dwAddReqTime;
	BOOL bInviteJoin;
	tagJoinFamilyReq()
		:dwJoinerID(GT_INVALID), dwMemberID(GT_INVALID), dwAddReqTime(0), bInviteJoin(TRUE)
	{}
	tagJoinFamilyReq& operator=(tagJoinFamilyReq& other)
	{
		this->dwJoinerID = other.dwJoinerID;
		this->dwMemberID = other.dwMemberID;
		this->dwAddReqTime = other.dwAddReqTime;
		this->bInviteJoin = other.bInviteJoin;
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////
enum EFamilySpriteError
{
	EFSE_LACK_ITEM = 1,				//��ɫ�����Ƿ�ӵ���㹻�����Ķ�Ӧ��Ʒ
	EFSE_NOT_FAMILY_MEMBER = 2,		//���Ǽ����Ա
	EFSE_HAS_COMPLETE = 3,			//�����������ѱ������Ա���
	EFSE_BAG_FULL = 4,				//���ı������������޷������Ʒ�������������������ύ
	EFSE_SPRITE_LEVEL_FULL = 5,		//�ػ������ĸ������Ѿ��ﵽ��ǰ���ޣ����Կɽ�����������ֻ���þ������Ʒ���������Ƿ�ȷ�ϣ�����ȷ��/ȡ��
	EFSE_FABAO_WHITE = 6,			//���Ǽǵ�����Ʒ�ʱ�����ڰ�ɫƷ��
	EFSE_FABAO_LEVEL = 7,			//���޷�������������ȼ����������еǼ�
	EFSE_FABAO_STATU = 8,			//���������������������������׶�ʱ�����޷����еǼǵ�
	EFSE_FABAO_HAVE = 9,			//�Ѿ��еǼǵ�������
	EFSE_FABAO_NOT_HAVE = 10,		//��û�еǼ�����
	EFSE_FABAO_BAG = 11,			//��������
	EFSE_FABAO_LEVEL_DOWN = 12,		//��ɫ�ȼ�̫��
	EFSE_USE_ITEM_MAX = 13,			//ʹ����Ʒ�����������
};

//	 �ػ���������
enum EFamilySpriteAtt
{
	EFSA_Begin = 0,
	EFSA_HP = 0,
	EFSA_EXAttack,
	EFSA_InAttack,
	EFSA_EXDefense,
	EFSA_InDefense,
	EFSA_EXAttackDeeper,
	EFSA_InAttackDeeper,
	EFSA_EXAttackResistance,
	EFSA_InAttackResistance,
	EFSA_Toughness,
	EFSA_CritDes,
	EFSA_ControleffectDeepen,	
	EFSA_ControleffectResistance,
	EFSA_SlowingeffectDeepen,
	EFSA_SlowingeffectResistance,
	EFSA_FixedeffectDeepen,		
	EFSA_FixedeffectResistance,
	EFSA_AgingeffectDeepen,		
	EFSA_AgingeffectResistance,

	EFSA_Num,
};


struct tagFamilySpriteProto
{
	INT							nID;									// �ȼ���ΪID
	DWORD						dwExp;									// �ɳ���
	DWORD						dwAbilityUpperLimit[EFSA_Num]; 
	DWORD						dwPickTypeNum[2];						// ��ȡ��������
	DWORD						dwShedingzhi;

	//INT						nLevel;									// �ȼ�
	//DWORD						dwTrainNum;								// �������󷢲�����
	
	//DWORD						dwHP;									// ��������
	//DWORD						dwExAttack;								// ��������������
	//DWORD						dwInAttack;								// ����������������
	//DWORD						dwExDefense;							// ���������������
	//DWORD						dwInDefense;							// ����������������
	//DWORD						dwExAttackAdd;							// �����˺���������
	//DWORD						dwInAttackAdd;							// �����˺���������
	//DWORD						dwExDefenseDes;							// ��������������
	//DWORD						dwInDefenseDes;							// ���������������
	//DWORD						dwToughness;							// ��������
	//DWORD						dwCritDes;								// ������������
	//DWORD						dwControleffectDeepen;					// ����Ч����������
	//DWORD						dwControleffectResistance;				// ���Ƶֿ���������
	//DWORD						dwSlowingeffectDeepen;					// ����Ч����������
	//DWORD						dwSlowingeffectResistance;				// ���ٵֿ���������
	//DWORD						dwFixedeffectDeepen;					// �̶�Ч����������
	//DWORD						dwFixedeffectResistance;				// �̶��ֿ���������
	//DWORD						dwAgingeffectDeepen;					// ˥��Ч����������
	//DWORD						dwAgingeffectResistance;				// ˥���ֿ���������
	
};



struct tagSpriteTrainProto
{
	INT	nID;										
	INT	nPageType;								// ��ҳ����
	INT	nLevelLimit;								// �ȼ�����
	DWORD	dwAbilityType;			// ��������
	DWORD	dwPowerUpLimit;			// ������������
	DWORD	dwPowerLowLimit;			// ������������
	DWORD	dwItemID;						// ������Ʒ
	DWORD	dwItemNum;					// ������Ʒ����
	DWORD	dwItemQuality;				// ������ƷƷ��(��װ����ʱ��Ŷ�)
	DWORD	dwShowUpLimit;				// ���ֵȼ�����
	DWORD	dwShowLowLimit;			// ���ֵȼ�����
	DWORD	dwPickType;					// ��ȡ����
	DWORD	dwSpriteExpReword;		// �����ɳ��Ƚ���
	DWORD	dwExpReword;					// ���齱��
	DWORD	dwItemReword;				// ��Ʒ����
	DWORD	dwItemRewordNum;		// ��Ʒ��������
	DWORD	dwItemRewordQuality;	// ��Ʒ����Ʒ��(��װ����ʱ��Ŷ�)
};

struct tagSpriteTrainData
{
	INT		nLevel;
	std::vector<DWORD>	m_vecTrainID1;
	std::vector<DWORD>	m_vecTrainID2;
};

struct tagSpriteTrainClientProto : tagSpriteTrainProto
{
	DWORD	dwRealResult;							// ʵ�ʽ��
	TCHAR		szName[X_LONG_NAME];		// �����
	tagSpriteTrainClientProto():dwRealResult(GT_INVALID)
	{
		ZeroMemory(szName,X_LONG_NAME*sizeof(TCHAR));
	}
};

enum EFamilySpritePickType
{
	EFSPT_1 = 1,
	EFSPT_2 = 2,
};


//���������洢
struct tagFamilySpriteSave
{
	DWORD		dwFamilyID;								// ����ID
	UINT16		un2Level;								// �ȼ�
	INT64		n8Exp;									// �ɳ���
	TCHAR		tszName[X_SHORT_NAME];					// ����
	INT			nAbility[EFSA_Num];						// �ػ�����

	tagFamilySpriteSave()
	{
		Clear();
	}

	~tagFamilySpriteSave()
	{
		Clear();
	}

	tagFamilySpriteSave& operator=(tagFamilySpriteSave& other)
	{
		if( this == &other)
			return *this;

		this->dwFamilyID = other.dwFamilyID;
		this->un2Level = other.un2Level;
		this->n8Exp = other.n8Exp;
		for(int i = 0; i < EFSA_Num; ++i)
		{
			this->nAbility[i] = other.nAbility[i];
		}
		_stprintf(this->tszName,other.tszName);

		return *this;
	}

	VOID Clear()
	{
		for(int i = 0; i < EFSA_Num; ++i)				//�ػ�����
		{
			nAbility[i] = -1;
		}
		memset( tszName, 0, sizeof(tszName) );			//�ػ���������	
		un2Level = 0;									//�ػ������ȼ�
		n8Exp = 0;										//�ػ������ɳ���	
		dwFamilyID = 0;
	}

};

//����������������洢
struct tagFamilySpriteRequirementSave
{
	DWORD		dwRequirementID;						//����ID
	BOOL		bComplete;								//���״̬
	TCHAR		tszRoleName[X_SHORT_NAME];				//�����
	INT			nRet;									//���ʵ�ʽ��

	tagFamilySpriteRequirementSave()
	{
		Clear();
	}

	~tagFamilySpriteRequirementSave()
	{
		Clear();
	}

	tagFamilySpriteRequirementSave& operator=(const tagFamilySpriteRequirementSave& other)
	{
		if( this == &other)
			return *this;

		this->dwRequirementID = other.dwRequirementID;
		this->bComplete = other.bComplete;
		this->nRet = other.nRet;
		_tcscpy_s( this->tszRoleName, X_SHORT_NAME, other.tszRoleName );

		return *this;
	}

	VOID Clear()
	{
		dwRequirementID = 0;
		bComplete = FALSE;
		nRet = 0;
		memset( tszRoleName, 0, sizeof(tszRoleName) );
	}
};

//��ҵǼ�������Ϣ
struct tagRoleSpriteDataSave
{
	TCHAR	tszRoleName[X_SHORT_NAME];				//�Ǽ���
	FLOAT	fShili;									//ʵ��ֵ
	FLOAT	fGongming;								//����ֵ
	UINT16	un2Level;								//��ҵǼ������ȼ�
	DWORD	dwSpriteType;							//��ҵǼ�����TypeID
	DWORD	dwRoleID;								//��ɫID
	INT	bQuality;

	tagRoleSpriteDataSave()
	{
		Clear();
	}

	~tagRoleSpriteDataSave()
	{
		Clear();
	}

	tagRoleSpriteDataSave& operator=(tagRoleSpriteDataSave& other)
	{
		if( this == &other)
			return *this;

		this->fShili = other.fShili;
		this->fGongming = other.fGongming;
		this->un2Level = other.un2Level;
		this->dwSpriteType = other.dwSpriteType;
		this->dwRoleID = other.dwRoleID;
		this->bQuality = other.bQuality;
		_tcscpy_s( this->tszRoleName, X_SHORT_NAME, other.tszRoleName );

		return *this;
	}

	VOID Clear()
	{
		fShili = 0;
		fGongming = 0;
		un2Level = 0;
		dwSpriteType = 0;
		dwRoleID = 0;
		bQuality = 0;
		memset( tszRoleName, 0, sizeof(tszRoleName) );
	}
};


//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////
enum EFamilyQuestType
{
	EQT_Null = 0,
	EQT_Collect = 1,
	EQT_Kill	= 2
};

//��������ṹ��
struct tagFamilyQuestProto
{
	DWORD					dwID;			// ����ID	
	EFamilyQuestType		eType;
	DWORD					dwConditionID;	// ��������ID
	int						nLowLevel;
	int						nHighLevel;
	INT						nQuestNum;		
};

struct tagFamilyQuestName : tagFamilyQuestProto
{
	TCHAR szDesc[X_LONG_NAME];
};

enum EFamilyQuestFinishType
{
	EQFT_NULL = 0,
	EQFT_Item = 1,		//��Ʒ
	EQFT_Monster = 2,	//����
};

enum EJudgeType
{	
	EJT_Null = 0,
	EJT_ID = 1,				//�ж�ID
	EJT_Condition = 2,	//�ж�����
};

//���ϱ�
struct tagFamilyQuestAttProto
{
	DWORD dwID;
	EFamilyQuestFinishType eFinishType;
	EJudgeType eJudgeType;
	int nLowLevel;
	int nHighLevel;
	DWORD dwMapcrc;
	INT nMonsterRank;
	DWORD dwItemOrMonsterID[MAX_ItemOrMonsterID];
};

//������������
struct tagFamilyQuestRewardProto
{
	DWORD dwID;		//��ֵ��ʾ��ɫ�ȼ����������ֵȡ������
	DWORD dwExp;
	DWORD dwMoney;
	DWORD dwItemID[MAX_FQ_RewardNum];
	DWORD dwItemNum[MAX_FQ_RewardNum];
	DWORD dwItemIDEx[MAX_FQ_RewardNum];
	DWORD dwItemNumEx[MAX_FQ_RewardNum];
};

//  ��¼��������״̬�Ľṹ
struct tagFamilyQuest
{
	DWORD	dwQuestID;			// ����ID
	INT		nTargetCount;		// ����
	//INT		nProcess;			// ��ɶ�
};

//�����������ö��
enum EFamilyEventType
{
	EFET_NoEvent		= 0,

	EFET_ChangeLeader	= 1,	//�ƽ��峤
	EFET_MemberDismis	= 2,	//������Ա
	EFET_LeaveFamily	= 3,	//�뿪����

	EFET_EventNum,
};

#pragma pack(pop)
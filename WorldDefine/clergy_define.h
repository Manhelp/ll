//-----------------------------------------------------------------------------
// brief: ��ϵͳ����
//-----------------------------------------------------------------------------
#pragma once

#include "RoleDefine.h"

#pragma pack(push, 1)

const INT	MAX_FieldNum = 8;					// ��ְ�����������
const INT	MAX_GodHead  = 20;					// ����������
const INT	MAX_Belief   = 99999;				// �������ֵ 
const INT	MAX_CampaignInfoNumPerType = 10;	// ��ѡ�����ʾ������������
const INT   MAX_GOD_NUM = 7;					// ÿһϵ�߼������
const INT	MAX_GOD_Relation_NUM = 26;			// ��֮ϵ���ϵ���ְ����

enum ECompetitiveClergy
{
	ECC_WarWinner	= 15,	//ս��
	ECC_SunWinner	= 16,	//̫����
	ECC_ManaWinner	= 17,	//��ʦ֮��
	ECC_PeaceWinner	= 18,	//��ƽ֮��

	ECC_WarMen		= 19,	//����
	ECC_WarWomen	= 20,	//����Ů��
	ECC_SunMen		= 21,	//��ҹħ��
	ECC_SunWomen	= 22, 	//��Ӱ֮Ů
	ECC_ManaMen		= 23, 	//Ԫ�ط���
	ECC_ManaWomen	= 24,	//ħ��Ů��
	ECC_PeaceMen    = 25,	//ʹ��֮��
	ECC_PeaceWomen  = 26,	//����
};

enum ECandidateFlag
{
	ECF_Null		= 0,
	ECF_4BaseClergy	= 1,
	ECF_4BossClergy	= 2,
};

enum EPromotionType
{
	EPTT_NULL     = 0, //��
	EPTT_Directly = 1, //ֱ�ӽ���
	EPTT_Campaign = 2, //��������
};

enum ECampaignGodType
{
	ECGT_Null =-1,

	ECGT_War =0,	// ս��
	ECGT_Sun,		// ̫����
	ECGT_Mana,		// ��ʦ֮��
	ECGT_Peace,		// ��ƽ֮��

	ECGT_Num,
};

enum EGodType
{
	EGT_Null = 0,

	EGT_War,
	EGT_WarBoss,
	EGT_Sun,
	EGT_SunBoss,
	EGT_Mana,
	EGT_ManaBoss,
	EGT_Peace,
	EGT_PeaceBoss,
};

#define ECGT_INVALID(n) (n<=ECGT_Null || n>=ECGT_Num)

// ��ϵ�ڵ�ְλ
enum EClergyType
{
	ECT_Null = -1,

	ECT_WomenChamp	= 0,
	ECT_MenChamp	= 1,
	ECT_Winner		= 2,
	ECT_Loser		= 3,
};

// ��ְ����
// enum EClergyType
// {
// 	ECT_Start	= -1,
// 	ECT_Mars	= 0,	// ս��
// 	ECT_Apollo	= 1,	// ̫����
// 	ECT_Rabbi	= 2,	// ��ʦ֮��
// 	ECT_Peace	= 3,	// ��ƽ֮��
// 	ECT_End		= 4,
// };

struct tagClergyProto
{
	DWORD			dwID;						//��ְID
	INT8			n8ClergyLevel;				//��ְ��ֵ�ȼ�
	INT				nNum;						//��������
	EGodType		eType;						//��ְ����
	EPromotionType	ePromotionType;				//����ģʽ
	DWORD			dwClassLimit;				//ְҵ����
	INT				nGodheadLimit;				//�������
	INT				nLevelLimit;				//�ȼ�����
	ESex			eSexLimit;					//�Ա�����
	ERoleAttribute	eAttType;					//��������
	DWORD			dwAttValue;					//������ֵ
	INT8			n8FieldNum;					//��������
	DWORD			dwFieldID[MAX_FieldNum];	//����ID
};

struct tagClergyName : tagClergyProto
{
	DWORD			dwID;										//��ְID
	TCHAR			szName[X_LONG_NAME];						//��ְ����
	TCHAR			szIcon[X_LONG_NAME];						//��ְͼ��
	TCHAR			szEffect[X_LONG_NAME];						//��Ч
	TCHAR			szText[X_LONG_NAME];						//�ı�����
	TCHAR			szClergyText[X_LONG_NAME];					//��ְ�ı�(��λ���)

};

struct tagCondensedProto
{
	DWORD			dwID;						//ID
	INT8			n8LowerLevel;				//��͵ȼ�
	DWORD			dwAgglomerate_val;			//���۶�����
	INT				nCanActNum;					// �ܹ��������������
};

struct tagPersonInfo
{
	TCHAR			szName[X_SHORT_NAME];	// ����
	DWORD			dwStrength;				// ʵ��ֵ
	EClergyType		eClergy;				// ְλ

	tagPersonInfo& operator =(const tagPersonInfo&rv)
	{
		if(this==&rv)
			return *this;

		memcpy(this->szName, rv.szName, X_SHORT_NAME);
		this->dwStrength = rv.dwStrength;
		this->eClergy = rv.eClergy;
		return *this;
	}
};

struct tagGodCampaignInfo
{
	tagPersonInfo	sPlayerInfo[MAX_CampaignInfoNumPerType];

	tagGodCampaignInfo& operator =(const tagGodCampaignInfo& rv)
	{
		if(this==&rv)
			return *this;

		for (int i =0; i < MAX_CampaignInfoNumPerType ;i++)
		{
			this->sPlayerInfo[i] = rv.sPlayerInfo[i];
		}

		return *this;
	}
};

struct tagCampaignResultInfo
{
	tagGodCampaignInfo sCampaignResult[ECGT_Num];

	tagCampaignResultInfo& operator = (const tagCampaignResultInfo& rv)
	{
		if(this==&rv)
			return *this;

		for (int i=0; i <ECGT_Num ;i++)
		{
			this->sCampaignResult[i] = rv.sCampaignResult[i];
		}

		return *this;
	}
};

#define GetClassEnumBinary(eClassType, val) do{\
		val = 1<<(eClassType-1);	\
		break;	\
	}while(0)


//��ְ�������
//��������
enum EGodAreaType
{
	EGAT_NULL     = 0,	//��
	EGAT_GOD,			//��ְ
	EGAT_OPTIONAL, 		//��ѡ
	EGAT_RARE,	  		//ϡ��

	ECGT_NUM,
};

//������ʽ
enum EGodAreaNatureType
{
	EGANT_NULL     = 0,	//��
	EGANT_SKILL,		//����
	EGANT_BUFF, 		//״̬
	EGANT_ALL, 			//����+״̬

	EGANT_NUM,
};

//������
enum EGodAreaUseType
{
	EGANUT_NULL     = 0,	//��
	EGANUT_INITIATIVE,		//����
	EGANUT_PASSIVE, 		//����

	EGANUT_NUM,
};

//���⹦��
enum EGodAreaFuncType
{
	EGAFT_NULL     = -1,	//��
};

struct tagGodAreaProto
{
	DWORD				dwID;						//����ID
	EGodAreaType		eType;						//��������
	EGodAreaNatureType	eNatureType;				//������ʽ
	EGodAreaUseType		eUseType;					//������
	INT					nLevel;						//�����ȼ�
	INT					nLevelLimit;				//����ȼ�Ҫ��
	INT					nGodheadLimit;				//�������Ҫ��
	INT					nMaxNum;					//��������
	DWORD				dwPreID;					//ѧϰʱ����ǰ��
	INT					nFaithValue;				//ѧϰʱ��������
	DWORD				dwSkillID;					//��������
	DWORD				dwBuffID;					//����״̬
	EGodAreaFuncType	eFunctionType;				//���⹦��	
	INT					nFunctionValue;				//���⹦�ܲ���
	INT					nTime;						//ϡ������ʱ��
	BOOL				bContest;					//�Ƿ��������
};

struct tagGodAreaName : tagGodAreaProto
{
	DWORD				dwID;						//����ID
	TCHAR				szName[X_LONG_NAME];		//����
	TCHAR				szIcon[X_LONG_NAME];		//ͼ��
	TCHAR				szDesc[X_LONG_NAME];		//˵��
	TCHAR				szDescType[X_LONG_NAME];	//������������
	TCHAR				szGetDesc[X_LONG_NAME];	//���;��
};

//--------------------------------------------------------------------------------
// �����Ϳͻ��˵���Ϣ�ṹ
struct tagAreaMsgInfo
{
	DWORD	dwAreaID;								//����ID
	BOOL	bActivation;							//�Ƿ񼤻�
	DWORD	dwTime;									//ϡ������ʣ��ʱ��
	DWORD	dwCD;									//����CD

 	tagAreaMsgInfo& operator =(const tagAreaMsgInfo&rv)
 	{
 		if(this==&rv)
 			return *this;
 
 		this->dwAreaID = rv.dwAreaID;
 		this->bActivation = rv.bActivation;
		this->dwTime = rv.dwTime;
 		return *this;
 	}
};

struct tagAreaEffectProto
{
	DWORD dwID;
	DWORD dwBuffID;
};

#pragma pack(pop)
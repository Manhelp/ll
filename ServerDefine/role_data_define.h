#pragma once

#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/currency_define.h"
#include "../WorldDefine/master_apprentice.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/rankings_define.h"
#include "../WorldDefine/train_define.h"
#include "../WorldDefine/clergy_define.h"
#pragma pack(push, 1)

struct tagDWORDTime;

//-------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------
const INT MIN_ROLE_SAVE2DB_INTERVAL			= 60 * TICK_PER_SECOND;
const INT ROLE_TIME_STAT_INTERVAL			= 30 * 60 * TICK_PER_SECOND;

// Jason 2009-12-7 �����͸���󼶱�
// �����޶�,�����Ա�ȥ�������bug:ȥ�� static�оֲ����ԣ�
extern INT ROLE_MAX_LESSLEVEL_OF_LOONG		 ;

//-------------------------------------------------------------------------------------
// ���ﴴ���ṹ
//-------------------------------------------------------------------------------------
struct tagCreateRoleInfo
{
	tagAvatarAtt	Avatar;						// size = 16
	TCHAR			szRoleName[X_SHORT_NAME];

	tagDWORDTime	CreateTime;					// ����ʱ��
	DWORD			dwMapID;
	FLOAT			fX;
	FLOAT			fY;
	FLOAT			fZ;
	FLOAT			fFaceX;
	FLOAT			fFaceY;
	FLOAT			fFaceZ;
	BYTE			byLevel;
	BYTE			byReserved[3];

};

//---------------------------------------------------------------------------------------
// ʵʱ�������ݿ�ö��
//---------------------------------------------------------------------------------------
enum ERealTimeSaveAtt
{
	ERTSA_Null			= -1,
	ERTSA_Exp			= 0,
	ERTSA_Level			= 1,
};

//---------------------------------------------------------------------------------------
// ����������ݽṹ
//---------------------------------------------------------------------------------------
struct tagRoleInfo
{
	DWORD			dwRoleID;					// ID
	DWORD			dwRoleNameCrc;				// ����CRC
	DWORD			dwAccountID;				// �˻�ID
	TCHAR			szRoleName[X_SHORT_NAME];	// ����
	DWORD			dwMapID;					// ���ڵ�ͼ
	tagDWORDTime	dwTimeLastLogout;			// �������ʱ��
	BYTE			byLevel;					// �ȼ�
	BYTE			bySex;						// �Ա�
	bool			bOnline;					// ����Ƿ�����
	BYTE			byClass;					// ְҵ
	DWORD			dwLoverID;					// ����
	// Jason 2010-3-2 v1.3.2 ���Ӱ���id
	DWORD			dwGuildID;					// �����������
	INT				nMARelation;				// ʦͽ����
	INT		nVIPLevel;								// vip�ȼ���0���޵ȼ���1����2��3�׽�
	DWORD dwFamilyID;					// ����ID
};

//---------------------------------------------------------------------------------------
// ��ȫ��
//---------------------------------------------------------------------------------------
struct tagSafeCode
{
	DWORD	dwSafeCodeCrc;		// ��ȫ��Crc32��GT_INVALID��ʾû�����ù�
	DWORD	dwTimeReset;		// ����ʱ�䣬GT_INVALID��ʾû������
};


// Jason �����͸������ļ����ݽṹ
struct tagConfigLessingOfLoong
{
	INT nID;			// ����0-7��Ҳ��8��
	INT nExpireTime;	// ����ʱ��
	INT64 nExperience;	// ����ֵ
	DWORD	dwRewardItemTypeID;	// ������Ʒtypeid
	INT	nQlty;
	INT	nItemCount;		// ������Ʒ����
	tagConfigLessingOfLoong()
		:nID(0),nExpireTime(0),nExperience(0),dwRewardItemTypeID(0),nItemCount(0),nQlty(0)
	{
	}
	tagConfigLessingOfLoong(const tagConfigLessingOfLoong & t)
		:nID(t.nID),nExpireTime(t.nExpireTime),nExperience(t.nExperience),dwRewardItemTypeID(t.dwRewardItemTypeID),nItemCount(t.nItemCount),
		nQlty(t.nQlty)
	{
	}
	tagConfigLessingOfLoong & operator = (const tagConfigLessingOfLoong & t)
	{
		memcpy(this,&t,sizeof(t));
	}
};


//---------------------------------------------------------------------------------------
// accountrel�����������
//---------------------------------------------------------------------------------------
struct tagAccountCommon
{
	tagSafeCode				sSafeCode;
	DWORD					dwBagPsdCrc;
	INT						nBaiBaoYuanBao;
	INT64					n64WareSilver;
	INT16					n16WareSize;
	tagDWPlayerPersonalSet	stPersonalSet;
	DWORD					dwLastUseRoleID;
	EAccountSpec		eSpec; // ����Ȩ�ޣ������ض��

	VOID Init()
	{
		sSafeCode.dwSafeCodeCrc = GT_INVALID;
		sSafeCode.dwTimeReset	= GT_INVALID;

		dwBagPsdCrc				= GT_INVALID;
		n64WareSilver			= 0;
		nBaiBaoYuanBao			= 0;
		n16WareSize				= 0;

		stPersonalSet.bAcceptFactionInvite	=	true;
		stPersonalSet.bAcceptTeamInvite		=	true;
		stPersonalSet.bAcceptTradeRequest	=	true;
		stPersonalSet.bAllowFriendAdd		=	true;
		stPersonalSet.eInfoOpenType			=	EPIS_Public;

		dwLastUseRoleID			=	GT_INVALID;
	}
};

//--------------------------------------------------------------------------------------------
//����˺�����Ϣ
//--------------------------------------------------------------------------------------------
struct tagNewAccountCommon
{
	DWORD  _dwChannelID;                    // �������Ӫ��ID
	CHAR   szNewAccountName[X_SHORT_NAME];	// ����
	tagNewAccountCommon()
	{
		memset(this, 0,sizeof(*this));
	}
};

//---------------------------------------------------------------------------------------
// ��Ϸ�����������ݿ��������ȡ��������ݽṹ
//---------------------------------------------------------------------------------------
struct tagRoleDataConst
{
	DWORD			dwRoleNameCrc;				// ����CRC
	TCHAR			szRoleName[X_SHORT_NAME];	// ����
	tagAvatarAtt	Avatar;						// ���
	tagDWORDTime	CreateTime;					// ����ʱ��
};

struct tagRoleDataSave
{
	tagDWORDTime	LoginTime;					// ����ʱ��
	tagDWORDTime	LogoutTime;					// �ǳ�ʱ��
	INT				nOnlineTime;				// ������ʱ��
	INT				nCurOnlineTime;				// ��������ʱ��

	// Jason 2009-12-7 �����͸�
	DWORD			timeLastLessing;			// �ϴ������͸�ʱ�䣬ע�⣬��ֵ��������ʱ�䵥λһ��
	INT				nLessingLevel;				// �͸��������ROLE_MAX_LESSLEVEL_OF_LOONG��
	INT				nLessingLevelEx;			// Jason��2010-3-12 v1.5.0

	// Jason 2010-1-8 ÿ�����߽�������ֶ�
	DWORD			dwCurrentDayOnlineTime;		// �����ۼ�����ʱ��
	tagDWORDTime	timeOneDayFirstLogined;		// �����״ε�½ʱ��
	tagDWORDTime	dateLastLessingLoong;		// ���һ�������͸���ʱ��,�����͸���Чʱ��û�ж����ж�
	tagDWORDTime	timeOnlineRewardPerDayRec;	// ������ȡ���߽�����ʱ��
	// Jason 2010-1-15 ���߽���
	BOOL			bIsOpenOfflineReward;		// �Ƿ������߽���
	// Jason 2010-1-30 v1.3.2��������
	UINT16			u16FollowPetPocketValve;  // �����������
    UINT16          u16RidingPetPocketValve;  // ��������
	// Jason 2010-3-29 v1.5.1 �񼣻���
	INT				nGodMiraclePoints;

	BYTE			byTrainStatesTransfered;

	INT16			n16MiraclePracticeNum;		// �񼣼�����������
	tagDWORDTime	dwMiracleResetTime;			// ����ʱ��

	tagAvatarEquip	AvatarEquip;				// װ�����
	tagDisplaySet	DisplaySet;					// �����ʾ����

	DWORD			dwMapID;					// ���ڵĵ�ͼ��ID
	DWORD			dwRebornMapID;				// �����ͼID
	FLOAT			fCoordinate[3];				// λ�ã�x,y,z��
	FLOAT			fFace[3];					// ����Vectro3��

	EClassType		eClass;						// ְҵ
	EClassTypeEx	eClassEx;					// ��չְҵ

	INT				nLevel;						// ��ҵȼ�
	DWORD			dwLastUpgrade;				// ���һ�ε�����ʱ��
	INT				nLevelPm;					// �ȼ�����
	INT64			nCurExp;					// ��ҵ�ǰ�������������

	INT				nHP;						// ����
	INT				nMP;						// ����
	INT				nRage;						// ŭ��
	INT				nEndurance;					// �־���
	INT				nVitality;					// ����
	INT				nInjury;					// ����
	INT				nKnowledge;					// ����
	INT				nMorale;					// ʿ��
	INT				nMorality;					// ����
	INT				nCulture;					// ��ѧ��Ϊ
	INT				nHostility;					// ����
	INT				nCredit;					// ���ö�
	INT				nIdentity;					// ���
	INT				nSpirit;					// ����
	INT				nWunXun;					// ��ѫ
	INT				nWunJi;						// �似
	INT				nVIPPoint;					// ��Ա����

	INT				nWeary;						// ƣ��ֵ
	INT				nVipLevel;					// VIP�ȼ�
	tagDWORDTime dwVipStartTime;					// Vip����ʹ��ʱ�䣬û��ʹ�ù������Ѿ�����Ϊ0'
	UINT16				n16FreeWorldTalkTimes;	// Vip��ɫ�Ѿ�ʹ�õ�������纰���Ĵ���
	DWORD		dwVipMaxDays;	// Vip����ʱ��(��λΪ��)
	INT				nAttPoint;					// ��ҿ��õ����Ե�
	INT				nTalentPoint;				// ��ҿ��õ����ʵ�

	// ʵ�����
	INT				nStrength;					// ��ʵ��ֵ
	INT				nAdvanceStrength;			// ����ʵ��ֵ
	INT				nConsumptiveStrength;		// ������ʵ��ֵ
	INT				nStrengthNum;				// ʵ������
	INT				nEquipValue;				// װ��ʵ��ֵ
	INT				nYaoJingValue;				// ����ʵ��ֵ
	INT				nEquipPm;					// װ��ʵ������
	INT				nKillScore;					// ��¼�����һ��֮�ڻ�ɱ�ǰ�����һ�õĽ���ʵ��ֵ

	INT				nAttPointAdd[X_ERA_ATTA_NUM];// ���Ͷ�ŵ�����һ�������е�ֵ
	tagRoleTalent	talent[X_MAX_TALENT_PER_ROLE];// ��ҵ�����

	BOOL			bSafeGuard;					// �Ƿ�PK����
	tagDWORDTime	CloseSafeGuardTime;			// �ϴιر�PK������ʱ��
	INT				nDeadUnSetSafeGuardCountDown;	//�ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick
	DWORD           dwItemTransportMapID;           //��ǰ��¼���ͷ��󶨵�ͼID
	FLOAT			fItemTransportX;				//��ǰ��¼���ͷ��󶨵�x����
	FLOAT			fItemTransportZ;				//��ǰ��¼���ͷ��󶨵�z����
	FLOAT			fItemTransportY;				//��ǰ��¼���ͷ��󶨵�y����

	INT16			n16BagSize;					// ������С
	INT				nBagGold;					// �����н�����
	INT				nBagSilver;					// ������������
	INT				nBagYuanBao;				// ������Ԫ����
	INT				nExVolume;					// ��ɫ������

	//INT16			n16WareSize;				// �˺Ųֿ��С
	//INT				nWareGold;					// �˺Ųֿ��н�����
	//INT				nWareSilver;				// �˺Ųֿ���Ӭ����
	//INT				nWareYuanBao;				// �˺Ųֿ���Ԫ����

	DWORD			dwGuildID;					// ����id
	DWORD			dwFamilyID;					// ����id
	
	INT				nTotalTax;					// ��˰�ܶ�

	tagRemoteOpenSet sRemoteOpenSet;			// ��Զ����ҹ�����Ϣ����
	UINT16			u16ActiveTitleID;			// ��ǰ�ƺ�

	DWORD			dwTimeGetMallFree;			// �ϴδ��̳���ȡ�����Ʒʱ��

	INT	 			nSkillNum;					// ���ܸ���			-- tagSkillSave
	INT				nBuffNum;					// ״̬����			-- tagBuffSave

	INT				nTitleNum;					// �ƺŸ���			-- tagTitleSave

	INT				nItemNum;					// ��Ʒװ������		-- tagItem,tagEquip
	INT				nFriendNum;					// ���Ѹ���			-- ��: tagFriendSave, ��: ʵʱ,����������Ϣ
	INT				nEnemyNum;					// ��и���			-- ��: DWORD dwEnemyID, ��: ʵʱ,����������Ϣ
	INT				nQuestNum;					// �����б����		-- tagQuestSave
	INT				nQuestDoneNum;				// ����ɵ�������� -- ����tagQuestDoneSave���棺ʵʱ
	INT             nDailyQuestDoneNum;         // ����ɵ�ÿ��������� �� tagQuestDoneSave���棺ʵʱ
	INT				nItemCDTimeNum;				// ��Ʒ��ȴʱ��		-- tagCDTime
	INT				nFriendshipNum;				// �Ѻö�			-- tagFriendshipSave
	INT				nBlackNum;					// ������			-- DWORD
	BOOL			bClanDataChg;				// ���������Ƿ�ı�
	INT				nPetsNum;					// ������Ŀ
	tagScriptData<ESD_Role>	Data;				// ��ҽű�����
	INT				nTreasureSum;				// ��ҿ���������

	DWORD			dwLastStallTime;			// ̯λ�������ʱ��
	INT32			nStallDailyExp;				// ̯λ���վ���
	INT32			nStallCurExp;				// ̯λ��ǰ�ȼ�����
	BYTE			byStallLevel;				// ̯λ�ȼ�
	DWORD			dwLoverID;					// ���˵�roleid û������4294967295
	BOOL			bHaveWedding;				// �Ƿ���й�����
	BOOL			bHasLeftMsg;				// ������
	
	BOOL			bSpeakOff;					// ����״̬
	BOOL			bNeedPrisonRevive;			// ��Ҫ����������
	DWORD			dwFixSpiritCD;				// ������ȴʱ��
	DWORD			dwMasterID;					// ʦ��ID
	tagApprenticeData		ApprenticeData;		// ͽ��ID
	INT				nJingWuPoint;				// �������
	INT				nQinWuPoint;				// �������
	INT				nMARelation;				// ʦͽ����
	INT				nOpenTrainCnt;				// �ѿ�����������
	// Jason 2010-5-18 v2.1.2
	UINT8			nSoaringValue;				// ������־
	INT				nSoaringSkillLearnTimes;	// ��������ѧϰ��������
	//tagDWORDTime	timeLearnSoaringSkill;		// ������������ʱ��
	// Jason 2010-5-27 v2.2.0
	INT				nToughness;					// ��������
	// Jason 2010-6-13 v1.0.0
	INT				nConsolidateTimes;				// ǿ������

	// ��������
	INT				nCompleteRefreshTime;
	INT				nIMRefreshTime;
	INT				nBoardQuestNum;
	INT				nYellowListCompleteRefreshTime;
	INT				nYellowListIMRefreshTime;
	// Jason 2010-7-14 1.0.0
	DWORD		dwRoleState;

	// ��ɫ�Ƿ��Ѿ�������һԪ�ƻ���е�װ��
	BYTE 			byBuy50LvlItemFlag;
	BYTE 			byBuy60LvlItemFlag;
	BYTE 			byBuy70LvlItemFlag;
	BYTE 			byBuy80LvlItemFlag;

	BYTE			byCanRankFlag;// ��ɫ�Ƿ��ܽ�������,0���ܣ�1��

	DWORD		dwExpPilularUseTimes;

	INT32			nMonthOnlineTimeCnt;
	INT32			nEggInfoCnt;
	INT32			nWardrobeLayerCnt;
	// ��������װ��
	DWORD		dwBePlayActLayerID;
	BYTE			byBePlayActLevel;

	// ��ȡ����ÿ�ս�����ʱ�䣬ע��ÿ��ֻ����ȡһ�Σ�����û�м�¼����
	tagDWORDTime	timeTakeGuildReward;	// ��ȡս��ÿ�ս�����ʱ��

	DWORD			dwPickupModeSetting;	// ʰȡģʽ����
	BYTE			byPlayerBack;			// �Ƿ���ȡ����һع齱��
	UINT16			byPlayerBackDays;		// �ۼƻع�����
	BYTE			byUsingConstraintsPKSafeGuardLevel; // �Ƿ�ʹ��Ĭ�ϵ�pk�����ȼ�

	INT				nGodhead;		// ���
	INT				nGodFaith;		// ����
	INT				nGodCondense;	// ���۶�

	// ��ѡ��ְ���
	DWORD			dwClergyMarsPoint;			// ս��ϵ��ֵ
	DWORD			dwClergyApolloPoint;		// ̫����ϵ��ֵ
	DWORD			dwClergyRabbiPoint;			// ��ϵ��ϵ��ֵ
	DWORD			dwClergyPeacePoint;			// ��ƽ֮��ϵ��ֵ
	ECampaignGodType		eClergyCandidate;	// ��ְ��ѡ����״̬
	ECandidateFlag	eSeniorOrJunior;			// �Ƿ���Ե�ѡ����
	DWORD			dwClergy;					// ��ǰ��ְ
	DWORD			dwPreClergy;				// �ϴε���ְ
	INT					nAreaNum;					// ��ְ��������������������ѡ�����ϡ������ģ���������ְ�����
	
	// �̳�����������Ʒ�����¼
	INT					nBuyMallItemLogCount;	

	// key����ȡ�������
	DWORD			dwKeyCodeRewarded;

	DWORD 			dwTrainDate;				// ������������������
	INT				nTrainNum;					// ����������������������Ʒʹ�ô���
	INT				nAwardPoint;				// ��ɫ������ 
	INT				nAwardFlag;					// �����ȡ��־λ 
	DWORD		dwBrotherTeatherID;		// ����ֵ��д���ID
	tagDWORDTime	timeBloodBrother;	// ����ֵܵĽ���ʱ��
	INT				nPulseLearnRemainTime;	// ʣ��ľ������Դ�ͨ�Ĵ���
	INT				nTigerTime;	// �ϻ�����Ĵ���

	INT 			nGod;				//���
	INT 			nMonster;			//ħ��
	INT 			nGodPoint;			//�������
	INT 			nMonsterPoint;		//ħ������
	INT				nFlower;			//�ʻ�
	INT				nEgg;				//����
	INT				nHolyValue;			//ʥ��ֵ
	INT				nHidAdd;			//����
	INT				nEEI_All;			//Ԫ��ȫ��

	//bool			bIsHolySoulActivated;      //Ԫ���Ƿ񱻼���
	/*********************************************************************************************************
	*�����б�״̬�б��ƺ��б��ƺ������б���Ʒ�б�װ���б������б�����б������б�����������б�
	*��Ʒ��ȴʱ���, �Ѻöȱ���ɫ����(�Զ���)��ʦͽ�б��ѿ�������״̬����ϵͳ�����
	*���밴˳�򿽱�
	*********************************************************************************************************/

	BYTE			byData[1];
};

struct tagRoleDataLoad
{
	DWORD				dwSize;					// �����Ϣ��С
	tagRoleDataConst	RoleDataConst;
	tagRoleDataSave		RoleDataSave;
};

struct tagRecordDel // ɾ���������Ϣ(���α����)
{
	INT	 	nSkillNum;		// ���ܸ���(dwSkillID):  ����ԭ������ļ�����ʧ,�������������ѧ,�˴�Ҳ���¼
	INT		nBuffNum;		// ״̬����(dwBuffID):    �����������ʧ��������״̬
//	INT		nTalentNum;		// �����б�(ETalentType): ϴ�������
//	INT		nItemNum;		// ��Ʒ����(n64Serial):   ��ݻٵģ���������ݿ���ɾ���� -- ʵʱ
//	INT		nEquipNum;		// װ������(n64Serial):   ��ݻٵģ���������ݿ���ɾ���� -- ʵʱ
	INT		nQuestNum;		// �������(dwQuestID):   ����ɾ���ĺ���ɵ�

	/*********************************************************************************************************
	*�����б�״̬�б���Ʒ�б�װ���б������б�
	*���밴˳�򿽱�
	*********************************************************************************************************/
	BYTE	byData[1];		// ��¼����
};

//---------------------------------------------------------------------------------------------
// ���＼��
//---------------------------------------------------------------------------------------------
struct tagSkillSave
{
	DWORD	dwID;				// ����ID
	INT		nSelfLevel;			// ����ȼ�
	INT		nLearnLevel;		// Ͷ��ȼ�
	INT		nCoolDown;		// ���ܻ���Ҫ����ȴʱ��
	INT     nProficiency;		// ����������
	DWORD	dwActiveTime;		//����ʱ��
};

//---------------------------------------------------------------------------------------------
// ����buff�Ļ�����Ϣ
//---------------------------------------------------------------------------------------------
struct tagBuffSave
{
	DWORD		dwBuffID;					// ID
	INT32		nPersistTick;				// ��ǰ����ʱ��

	INT8		n8Level;					// �ȼ�
	INT8		n8CurLapTimes;				// ��ǰ���Ӵ���
	INT8		n8ModifierNum;				// Ӱ�����ļ��ܸ���
	INT8		n8Dummy[1];

	DWORD		dwSrcUnitID;				// ԴUnit��ID
	DWORD		dwSrcSkillID;				// ���ĸ����ܲ�������ID��
	INT64		n64Serial;					// ���ĸ���Ʒ��װ������
	DWORD		dwItemTypeID;				// ��Ʒ��װ��������ID

	BYTE		byData[1];
};

//---------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------
struct tagFriendSave
{
	DWORD		dwFriendID;		// ����id
	INT32		nGroupID;		// ����
};

struct tagFriendshipSave
{
	DWORD		dwFriendID;		// ����id
	INT32		nFriVal;		// �Ѻö�
};

//-----------------------------------------------------------------------------
// ���浽���ݿ����������
//-----------------------------------------------------------------------------
struct tagTitleSave
{
	UINT16			u16TitleID;			// �ƺ�ID
	DWORD			dwStateMark;		// ��Ϊ0�����ʾ�ѻ�ȡ�������ʾpara2
	DWORD			dwTimeRemain;		// ��Ϊʱ���Ƴƺţ����ʾʣ��ʱ��
};

//---------------------------------------------------------------------------------------
// ��Ҹ�����������Ӧ�Ļ������Լ������ü��õ����ʺ�������һ������ľ������� -- ��xml�ж���
//---------------------------------------------------------------------------------------
struct tagLevelUpEffect
{
	INT64	nExpLevelUp;		// ������һ������ľ�������
	INT16	n16RoleAtt[6];		// ��ǡ�������Ԫ������������������
	INT16	n16RoleAttAvail;	// �õ��Ŀ�Ͷ���Ե�
	INT16	n16TalentAvail;		// �õ��Ŀ�Ͷ���ʵ�
    INT16   n16TalentAvail_limit;// �õ���ǰ������������
	INT16	n16HP;				// ��ǰ����Ѫ��
	INT16	n16MP;				// ��ǰ����������
	INT16	n16Vitality;		// ��ǰ�������
	INT16	n16Durance;		// ����

	tagLevelUpEffect() { ZeroMemory(this, sizeof(*this)); nExpLevelUp = GT_INVALID; }
};

//---------------------------------------------------------------------------------------
// ��Ҹ�����ѫ�ȼ�����Ӧ����ѫ���似 -- ��xml�ж���
//---------------------------------------------------------------------------------------
struct tagWuXunProto
{
	INT16     n16WuXunLevel ;    //��ѫ�ȼ�
	UINT      unWuXunVal;        //��ǰ��ѫ�ȼ�����ѫֵ
	UINT      unMaxWuJiVal;      //�似ֵ   
	tagWuXunProto() { ZeroMemory(this,sizeof(*this));}
};

//---------------------------------------------------------------------------------------
// ��ҹ�������������ʵ���Ӧ���� -- ��xml�ж���
//---------------------------------------------------------------------------------------
struct tagTalentAvailCost
{
	
    INT     nTalentSequenceNum ; // ���������������
	DWORD   dwCostSilver ;       // ���ѵ���Ϸ��
	tagTalentAvailCost() { ZeroMemory(this, sizeof(*this)); }
};

//---------------------------------------------------------------------------------------
// ��ҽű��������Ͷ���
//---------------------------------------------------------------------------------------
enum ERoleScriptData
{
	ERSD_Injury			=	0,				// 0������������״̬
	ERSD_END			=	100,
};

// �·�ö��
enum EMonth
{
	EM_January = 0,
	EM_February,
	EM_March,
	EM_April,
	EM_May,
	EM_June,
	EM_July,
	EM_August,
	EM_September,
	EM_October,
	EM_November,
	EM_December,

	EM_End
};
//-------------------------------------------------------------------------------------
// ��ɫÿ������ʱ��ͳ��(����ʱ����λΪ����)
//-------------------------------------------------------------------------------------
struct tagMonthOnlineTime 
{
	tagDWORDTime	dwCurTime;								// ��ǰʱ��
	UINT16				n16OnlineTime[EM_End];		// ��ɫ����ÿ������ʱ��
	tagMonthOnlineTime() { ZeroMemory(this, sizeof(*this));  }
};

//-------------------------------------------------------------------------------------------
struct tagAreaSave
{
	DWORD	dwAreaID;				//����ID
	DWORD	dwRoleID;				//��ɫID
	BOOL	bActivation;			//����״̬ 
	DWORD	dwCD;					//CDʱ��
};
//-------------------------------------------------------------------------------------------

#pragma pack(pop)
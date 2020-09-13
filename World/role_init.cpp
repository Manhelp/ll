//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_init.cpp
// author: 
// actor:
// data: 2008-8-11
// last:
// brief: ��ʼ���������ݽṹ
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/master_apprentice.h"
#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_virgin_online.h"
#include "../WorldDefine/msg_activity.h"
#include "../WorldDefine/RoleDefine.h"
#include "../ServerDefine/msg_item.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/family_define.h"
#include "..\ServerDefine\family_define.h"
#include "../ServerDefine/msg_holySoul.h"

#include "role.h"
#include "att_res.h"
#include "skill.h"
#include "quest.h"
#include "quest_mgr.h"
#include "map_creator.h"
#include "script_mgr.h"
#include "title_mgr.h"
#include "pet_pocket.h"
#include "vip_netbar.h"
#include "guild.h"
#include "guild_mgr.h"
#include "quest_board.h"
#include "item_creator.h"
#include "family.h"
#include "family_join.h"
#include "Family_mgr.h"
#include "family_sprite.h"
#include "family_sprite_mgr.h"
#include "HolySoul.h"


//-------------------------------------------------------------------------------
// ��ʼ�����
//-------------------------------------------------------------------------------
VOID Role::Init(const tagRoleDataLoad* pData)
{
	ASSERT( P_VALID(pData) );

	const tagRoleDataConst* pDataConst = &pData->RoleDataConst;
	const tagRoleDataSave* pDataSave = &pData->RoleDataSave;

	// ��ʼ����ҽű�
	m_pScript = g_ScriptMgr.GetRoleScript();

	// ��ҽű�����
	IFASTCODE->MemCpy(m_ScriptData.dwData, pDataSave->Data.dwData, sizeof(DWORD)*ESD_Role);

	// �ƺŹ�����
	m_pTitleMgr		= new TitleMgr;
	// Jason 2010-3-30
	if( P_VALID(m_pTitleMgr) )
		m_pTitleMgr->Resize(MAX_TITLE_NUM_EX);

	// �½������
	m_pPetPocket	= new PetPocket;
	
	// ����������
	m_pBoardQuestMgr = new BoardQuestMgr(this);
	m_pBoardQuestMgr->Init(this->GetScriptData(RewardBoardQuestLvlIdx), EQBT_Reward);

	m_pYellowListMgr = new BoardQuestMgr(this);
	m_pYellowListMgr->Init(this->GetScriptData(YellowListBoardQuestLvlIdx), EQBT_YellowList);
	
	m_n16YellowListAcceptTimes = this->GetScriptData(TodayYellowListQuestTimesIdx);

	// ��������ж��Ƿ��ǽ���ĵ�һ�ε�½
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	if (pDataSave->LoginTime.year == dwCurTime.year &&
		pDataSave->LoginTime.month == dwCurTime.month &&
		pDataSave->LoginTime.day == dwCurTime.day)
	{
		m_bTodayFirstLogin = FALSE;
	}
	else
	{
		m_bTodayFirstLogin = TRUE;
	}

	// ��ʼ����������
	IFASTCODE->MemCpy(&m_Avatar, &pDataConst->Avatar, sizeof(m_Avatar));
	m_DisplaySet = pDataSave->DisplaySet;
	m_dwRebornMapID = pDataSave->dwRebornMapID;
	m_CreatTime = pDataConst->CreateTime;
	m_LoginTime = g_world.GetWorldTime();		// ��������ʱ��
	
	//--------------------------------------------------------------------------------------------------

	// Jason 2010-1-8 ÿ�����߽������
	m_dateLastLessingLoong	= pDataSave->dateLastLessingLoong;
	if( (DWORD)m_dateLastLessingLoong == 0 )
		m_dateLastLessingLoong = m_LoginTime;

	m_LogoutTime = pDataSave->LogoutTime;
	m_timeRealLogout = m_LogoutTime;


	m_timeOnlineRewardPerDayRec = pDataSave->timeOnlineRewardPerDayRec;
	m_timeOneDayFirstLogined = pDataSave->timeOneDayFirstLogined;
	m_dwCurrentDayOnlineTime = pDataSave->dwCurrentDayOnlineTime;
	if( m_timeOneDayFirstLogined.day != m_LoginTime.day || m_timeOneDayFirstLogined.month != m_LoginTime.month || m_timeOneDayFirstLogined.year != m_LoginTime.year  ) // ������
	{
		// �����˳������
		if( m_LogoutTime.day == m_LoginTime.day && 
			m_LogoutTime.month == m_LoginTime.month &&
			m_LogoutTime.year == m_LoginTime.year ) 
		{
			// �����˳��ˣ����ǵڶ���û����ȡ����ʱ��ֻ����m_nLessingLevelEx�������ۼ�����ʱ��m_timeLastLessing�����㣬������¼�ʱ
			if( m_timeOnlineRewardPerDayRec.day !=	m_LoginTime.day || m_timeOnlineRewardPerDayRec.month != m_LoginTime.month || m_timeOnlineRewardPerDayRec.year != m_LoginTime.year   )
				m_nLessingLevelEx = 0;//�������п���ʹ�û����콱��Ӵ
		}
		// ���ǿ����˳�������Ͱ���Щʱ�����������
		else  
		{
			m_dwCurrentDayOnlineTime = 0;
			m_nLessingLevelEx = 0;
			m_timeLastLessing = 0;
		}
		m_timeOneDayFirstLogined = m_LoginTime;
	}
	
	//m_timeOnlineRewardPerDayRec = pDataSave->timeOnlineRewardPerDayRec;

	// 2010-1-15 ���߹һ����
	m_bIsOpenOfflineReward = pDataSave->bIsOpenOfflineReward;

	//--------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------
	// Jason 2010-1-30 v1.3.2��������
	m_u16PetPocketValve = pDataSave->u16FollowPetPocketValve+pDataSave->u16RidingPetPocketValve	;
	m_u16FollowPetPocketValve = pDataSave->u16FollowPetPocketValve;
	m_u16RidingPetPocketValve = pDataSave->u16RidingPetPocketValve;
	//--------------------------------------------------------------------------------------------------

	//m_LogoutTime = pDataSave->LogoutTime;
	m_nOnlineTime = pDataSave->nOnlineTime;
	m_nCurOnlineTime = pDataSave->nCurOnlineTime;
	m_dwLastUpgrade = pDataSave->dwLastUpgrade;
	m_eClass = pDataSave->eClass;
	m_eClassEx = pDataSave->eClassEx;
	m_nKillScore = pDataSave->nKillScore;
	m_bHasLeftMsg = pDataSave->bHasLeftMsg;
	// Jason 2010-4-12 
	m_nGodMiraclePoints = pDataSave->nGodMiraclePoints;

	// ��ʼ���ϴλ�ȡ�̳������Ʒʱ��
	SetLastGetMallFreeTime(pDataSave->dwTimeGetMallFree);

	// ���������Ƿ����������������
	m_RoleStateEx.SetState(ERSE_BagPsdExist);
	if(GetSession() && !GetSession()->IsHaveBagPsd())
	{
		m_RoleStateEx.UnsetState(ERSE_BagPsdExist);
		m_RoleStateEx.SetState(ERSE_BagPsdPass);
	}

	// ��ʼ���ƺ�ѡ��
	m_pTitleMgr->InitOpts(this, pDataSave->u16ActiveTitleID, pDataSave->sRemoteOpenSet.bTitle);

	// ��������CDʱ��
	m_dwFixSpiriteCDTime = pDataSave->dwFixSpiritCD;

	// PK�������
	m_CloseSafeGuardTime = pDataSave->CloseSafeGuardTime;
	m_nDeadUnSetSafeGuardCountDown = pDataSave->nDeadUnSetSafeGuardCountDown;

	m_bNeedPrisonRevive = pDataSave->bNeedPrisonRevive;		//��Ҫ��������

	m_bUsingPkMinLevel = pDataSave->byUsingConstraintsPKSafeGuardLevel;

	// ʦͽ���
	m_dwMasterID = pDataSave->dwMasterID;
	m_nMARelation = pDataSave->nMARelation;
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(m_dwID);
	if(P_VALID(pInfo)) m_nMARelation = pInfo->nMARelation; // ʹ�����µ� 
	//IFASTCODE->MemCpy(&m_ApprenticeData.dwData, pDataSave->ApprenticeData.dwData, sizeof(m_ApprenticeData.dwData));
	//for (int i=0; i!=MAX_APPRENTICE_NUM; ++i)
	//{
	//	if (m_ApprenticeData.dwData[i] != 0)
	//	{
	//		m_mapApprentice.Add(m_ApprenticeData.dwData[i], m_ApprenticeData.dwData[i]);
	//	}
	//}
	if(m_dwMasterID != GT_INVALID)
		m_nJingWuPoint = pDataSave->nJingWuPoint;				// �������
	//m_nQinWuPoint  = pDataSave->nQinWuPoint;				// �������

	IFASTCODE->MemCpy(m_Talent, pDataSave->talent, sizeof(m_Talent));

	// Jason 2010-7-14 v1.0.0
	m_RoleState.InitState(0);
	if( pDataSave->dwRoleState & ERS_PK )
	{
		SetRoleState(ERS_PK,FALSE);
	}
	// ����ģʽ
	else if( pDataSave->dwRoleState & ERS_PeaceModel )
	{
		SetRoleState(ERS_PeaceModel,FALSE);
	}
	else if( pDataSave->dwRoleState & ERS_PKEX )
	{
		SetRoleState(ERS_PKEX,FALSE);
	}

	INT nMax_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
	if(g_world.IsPkConstraintsOpen())
	{
		nMax_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
	}

	if(pDataSave->bSafeGuard && pDataSave->nLevel <= nMax_PK_SafeGuard_Level)
	{
		SetRoleState(ERS_Safeguard, FALSE);
	}

	m_byBuy50LvlItemFlag = pDataSave->byBuy50LvlItemFlag;
	m_byBuy60LvlItemFlag = pDataSave->byBuy60LvlItemFlag;
	m_byBuy70LvlItemFlag = pDataSave->byBuy70LvlItemFlag;
	m_byBuy80LvlItemFlag = pDataSave->byBuy80LvlItemFlag;
	GetWardrobe().SetBePlayActLayerID(pDataSave->dwBePlayActLayerID);
	GetWardrobe().SetBePlayActLevel(pDataSave->byBePlayActLevel);

	// ����
	m_dwGuildID = pDataSave->dwGuildID;
	m_dwFamilyID = pDataSave->dwFamilyID;
	if (GT_VALID(m_dwGuildID))
	{
		// �������Ƿ��ڰ�����
		Guild* pGuild	= g_guildMgr.GetGuild(m_dwGuildID);
		if (!P_VALID(pGuild) || !P_VALID(pGuild->GetMember(GetID())))
		{
			SetGuildID(GT_INVALID);
		}
		else
		{
			// ����״̬
			GuildCommodity* pCommodity = pGuild->GetGuildCommerce().GetCommodity(m_dwID);
			if (P_VALID(pCommodity))
			{
				SetRoleState(ERS_Commerce, FALSE);
			}
		}
	}
	
	m_nPulseLearnTimes = pData->RoleDataSave.nPulseLearnRemainTime;
	m_nTigerTimes = pData->RoleDataSave.nTigerTime;

	if( pDataSave->LogoutTime.year != m_LoginTime.year || pDataSave->LogoutTime.month != m_LoginTime.month || pDataSave->LogoutTime.day != m_LoginTime.day )
	{
		// ������죬����ÿ�ս�ȡ��֮�л�������������
		ResetYellowListAcceptTimes();

		// ������죬���ý�ɫʹ�þ��鵤����
		ResetExpPilularUseTimes();

		// ���þ���ѧϰ����
		ResetPulseCanLearnNum();
	}

	// ��ȡ�������������ݿ�����
	InitAtt(pDataSave);

	//if( pDataSave->timeLearnSoaringSkill.year != m_LoginTime.year || pDataSave->timeLearnSoaringSkill.month != m_LoginTime.month || pDataSave->timeLearnSoaringSkill.day != m_LoginTime.day  )
	if( pDataSave->LogoutTime.year != m_LoginTime.year || pDataSave->LogoutTime.month != m_LoginTime.month || pDataSave->LogoutTime.day != m_LoginTime.day )
	{
		m_nSoaringSkillLearnTimes	= m_nCLearnSoaringSkillTotalTimes;
		//m_timeLearnSoaringSkill		= m_LoginTime;
	}
	else
	{
		m_nSoaringSkillLearnTimes	= pDataSave->nSoaringSkillLearnTimes;
		//m_timeLearnSoaringSkill		= pDataSave->timeLearnSoaringSkill;
	}

	// ��������
	m_pBoardQuestMgr->SetCompleteRefreshTime(pDataSave->nCompleteRefreshTime);
	m_pBoardQuestMgr->SetIMRefreshTime(pDataSave->nIMRefreshTime);
	m_pYellowListMgr->SetCompleteRefreshTime(pDataSave->nYellowListCompleteRefreshTime);
	m_pYellowListMgr->SetIMRefreshTime(pDataSave->nYellowListIMRefreshTime);

	// ��ʼ����ɫʵ��ֵ��صĻ���ֵ����Ҫ������ں������
	//m_Strength.nAdvanceStrength = pData->RoleDataSave.nAdvanceStrength;
	m_Strength.nConsumptiveStrength = pData->RoleDataSave.nConsumptiveStrength;
	m_nStrengthNum = pData->RoleDataSave.nStrengthNum;
	m_nEquipPm = pDataSave->nEquipPm;

	// Jason 2010-6-12 v1.0.0
	m_nConsolidateTimes = pDataSave->nConsolidateTimes;

	m_timeTakeGuildReward = pDataSave->timeTakeGuildReward	;

	// ����Ƿ���Ҫ���½���ʵ��ֵ
	CheckRoleAdvanceStrength();

	// ��ȡʰȡģʽ����
	m_dwPickupModeSetting = pDataSave->dwPickupModeSetting;

	// ��ȡ��һع��־
	m_bPlayerBack = pDataSave->byPlayerBack;
	if(!g_world.IsPlayerBack())
	{
		m_bPlayerBack = FALSE;
		m_byPlayerBackDay = 0;
	}
	m_byPlayerBackDay = pDataSave->byPlayerBackDays;

	// ��ְ��ѡ���
	m_dwClergyMarsPoint = pDataSave->dwClergyMarsPoint;
	m_dwClergyApolloPoint = pDataSave->dwClergyApolloPoint;
	m_dwClergyRabbiPoint = pDataSave->dwClergyRabbiPoint;
	m_dwClergyPeacePoint = pDataSave->dwClergyPeacePoint;
	m_eClergyCandidate = pDataSave->eClergyCandidate;
	m_eClergy4SeniorOrJunior = pDataSave->eSeniorOrJunior;
	m_dwClergy = pDataSave->dwClergy;
	m_dwPrevClergy = pDataSave->dwPreClergy;

	m_dwKeyCodeRewarded = pDataSave->dwKeyCodeRewarded;

	m_dwTrainDate = pDataSave->dwTrainDate;
	m_nTrainNum = pDataSave->nTrainNum;
	m_nAwardPoint = pDataSave->nAwardPoint;
	m_nAwardFlag = pDataSave->nAwardFlag;

	m_nGod = pDataSave->nGod;
	m_nMonster = pDataSave->nMonster;
	m_nGodPoint = pDataSave->nGodPoint;
	m_nMonsterPoint = pDataSave->nMonsterPoint;

	UpdateFamilyTrainNum();

	// todo����ȡ�����б�װ���б�buff�б���������Զ��������Ե�Ӱ��
	/*********************************************************************************************************
	*�����б�״̬�б��ƺ��б��ƺ������б���Ʒ�б�װ���б������б�����б������б�����������б���ɫ����
	*ʦͽ��ͬ�ţ� �ѿ�������״̬��
	*���밴˳���ȡ(��˳���tagRoleDataSave�ж�Ӧ)
	*********************************************************************************************************/

	const BYTE *pDBData = &pDataSave->byData[0];	// �б�����ָ��,��ָ�����ڶ�ȡ�������ƶ�,��pDBData��ֵ�Ǳ仯��

	// ��ʼ�������б�
	InitSkill(pDBData, pDataSave->nSkillNum);

	// ��ʼ��״̬�б�
	InitBuff(pDBData, pDataSave->nBuffNum);

	// ��ʼ���ƺ��б�
	m_pTitleMgr->InitTitles(pDBData, pDataSave->nTitleNum);

	// ������ҵ����߷��������Ա�۳�ʥ��Ĭ��ֵ
	/*DWORD dwOffSecond = CalcTimeDiff(m_LoginTime,pDataSave->LogoutTime);
	INT nOffMinute =(INT)(dwOffSecond / (DWORD)60);*/
	// ��ʼ����Ʒװ���б�
	InitItem(pDBData, pDataSave->nItemNum);

	// ��ʼ�����ѳ���б�
	InitFriend(pDBData, pDataSave->nFriendNum);
	
	// ����б������Ϣ
	InitEnemy(pDBData, pDataSave->nEnemyNum);

	// ��ʼ����ҵ�ǰ�����б�
	InitCurrentQuest(pDBData, pDataSave->nQuestNum);

	// ��ʼ�������������б�
	InitCompleteQuest(pDBData, pDataSave->nQuestDoneNum);

	// ��ʼ��������ÿ�������б�
	InitDailyCompleteQuest(pDBData, pDataSave->nDailyQuestDoneNum);

	// ��ʼ����Ʒ��ȴʱ��
	InitItemCDTime(pDBData, pDataSave->nItemCDTimeNum);

	// ��ʼ�����Ѷ�
	InitFriendValue(pDBData, pDataSave->nFriendshipNum);

	// ��ʼ��������
	InitBlackList(pDBData, pDataSave->nBlackNum);

	// ��ʼ����ɫ����
	m_VCard.Init(pDBData, this);

	// ��ʼ����������
	m_ClanData.Init(pDBData, this);

	// ��ʼ�������
	m_pPetPocket->Init(pDBData, pDataSave->nPetsNum, this);

	// ��ʼ��ʦͽ
	InitMaster(pDBData);

	// ��ʼ��ͬ��
	InitClassMates(pDBData);

	// ��ʼ���ѿ�������״̬��
	InitTrainState(pDBData, pDataSave->nOpenTrainCnt);

	// ���������
	m_pBoardQuestMgr->Init(pDBData, pDataSave->nBoardQuestNum, this, FALSE);
	m_pYellowListMgr->Init(pDBData, pDataSave->nBoardQuestNum, this, TRUE);

	// ��ɫ��ǰ������ʱ��
	//InitCurMonthOnlineTime(pDBData, pDataSave->nMonthOnlineTimeCnt);

	// �ҽ�
	m_EggBroker.InitEggInfoFromDB(pDBData, pDataSave->nEggInfoCnt);

	// �³�
	m_Wardrobe.InitWardrobe(pDBData, pDataSave->nWardrobeLayerCnt, this);

	// ��ʼ����ϵͳ�����б�
	InitArea(pDBData, pDataSave->nAreaNum);

	// ��ʼ�������̳���Ʒ����
	InitBuyMallItemCount(pDBData, pDataSave->nBuyMallItemLogCount);

	//��ʼ�������������Լӳ�
	InitAbilityFromFamilySprite();

	// ��ְ��ѡ
	LoginClergyVoteResultCheck();

	if( GetBaseAttValue(ERA_Soar) == ESV_NotCompletedSoaring && !IsHaveQuest(40001) && !IsHaveDoneQuest(40001) )
	{
		SetBaseAttValue(ERA_Soar,ESV_NotSoaring);
	}
	//��ʼ��Ԫ����Ϣ
	//if ( pDataSave->bIsHolySoulActivated )
	//{
	//	DWORD dwOffSecond = CalcTimeDiff(m_LoginTime,pDataSave->LogoutTime);
	//	INT nOffHour =(INT)(dwOffSecond / (DWORD)3600);
	//	InitHolySoulInfo(pDBData,nOffHour);
	//}

	// ���������Ѿ�������ϣ����¼��������ʼ��ǰ����(�����������ʵ��ϵͳ��صĸ���ֵ)
	CalInitAtt();

	// ��������״̬
	CalInitState();

	g_VipNetBarMgr.PlayerNotify(GetSession()->GetSessionID());

	// ��ʼ����ɫ���������״̬
	InitChestState();

	m_BrotherMgr.InitData(pDataSave->dwBrotherTeatherID,pDataSave->timeBloodBrother);

	// �������30��������ʱ����Ҫ���ر�pk����
// 	INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
// 	if( g_world.IsPkConstraintsOpen()  && IsUsingPKConstraintsSafeGuradMinLevel() )
// 	{
// 		Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
// 	}
// 	if ( m_nLevel > Max_PK_SafeGuard_Level )
// 	{
// 		m_bLevelUpClosedSafeGuard = TRUE;
// 	}

	tagNS_RoleInitOk send;
	GetSession()->SendMessage(&send, send.dwSize);

	//UpdateBlessingOfLoong();// Jason
	// Jason 12-8-2007������ģʽ
	//NotifyClientIniLessingOfLoong();

	// Jason v1.3.1�ⲿ����
	g_dbSession.SendMsgToPlayerForExtLinks  (this);


	if( g_VipNetBarMgr.IsOpen() )
	{
		g_VipNetBarMgr.RecReward(this);
	}

	SynPickupModeSetting2Client();
}

//----------------------------------------------------------------------------------------------
// ��ʼ�����ݿ����е���������
//----------------------------------------------------------------------------------------------
VOID Role::InitAtt(const tagRoleDataSave* pDataSave)
{
	// ����һ�����Ե�Ͷ��
	IFASTCODE->MemCpy(m_nAttPointAdd, pDataSave->nAttPointAdd, sizeof(m_nAttPointAdd));

	// �������Ը���Ĭ��ֵ
	for(INT n = 0; n < ERA_End; n++)
	{
		m_nBaseAtt[n] = g_attRes.GetAttDefRole(n);
	}

	// ����һЩ���������ݿ��е����ԣ�һЩ�������ֵ�ı�����ԣ���ȱ��һ��ʿ����
	m_nAtt[ERA_HP]			=	pDataSave->nHP;
	m_nAtt[ERA_MP]			=	pDataSave->nMP;
	m_nAtt[ERA_Rage]		=	pDataSave->nRage;
	m_nAtt[ERA_Vitality]	=	pDataSave->nVitality;
	m_nAtt[ERA_Endurance]	=	pDataSave->nEndurance;
	m_nAtt[ERA_Knowledge]	=	pDataSave->nKnowledge;
	m_nAtt[ERA_Injury]		=	pDataSave->nInjury;
	m_nAtt[ERA_Morale]		=	pDataSave->nMorale;
	m_nAtt[ERA_Morality]	=	pDataSave->nMorality;
	m_nAtt[ERA_Culture]		=	pDataSave->nCulture;
	m_nAtt[ERA_AttPoint]	=	pDataSave->nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	pDataSave->nTalentPoint;
	m_nAtt[ERA_Hostility]	=	pDataSave->nHostility;
	//m_nAtt[ERA_Shili]		=	pDataSave->nStrength;		// ʵ��ֵ
	m_nAtt[ERA_Spirit]		=	pDataSave->nSpirit;			// ����ֵ
	m_nAtt[ERA_WuXun]		=	pDataSave->nWunXun;			// ��ѫֵ
	m_nAtt[ERA_WuJi]		=	pDataSave->nWunJi;			// �似ֵ
	//��ϵͳ����
	m_nAtt[ERA_God_Condenced] = pDataSave->nGodCondense;
	m_nAtt[ERA_God_Faith]		= pDataSave->nGodFaith;
	m_nAtt[ERA_God_Godhead]	= pDataSave->nGodhead;
	//�ʻ�����
	m_nAtt[ERA_FlowerNum]		=	pDataSave->nFlower;			// �ʻ�
	m_nAtt[ERA_EggsNum]			=	pDataSave->nEgg;			// ����
	m_nAtt[ERA_Holy]			=	pDataSave->nHolyValue;		// ʥ��ֵ

	if( pDataSave->LogoutTime.year != m_LoginTime.year || pDataSave->LogoutTime.month != m_LoginTime.month || pDataSave->LogoutTime.day != m_LoginTime.day )
	{
		m_nAtt[ERA_Weary]	=	0;							// ƣ��ֵ��0
	}
	else
	{
		m_nAtt[ERA_Weary]	=	pDataSave->nWeary;			// ƣ��ֵ
	}
	m_nAtt[ERA_VipLevel]	= pDataSave->nVipLevel;	// ��ɫvip�ȼ�

	m_nLevel		= pDataSave->nLevel;					// �ȼ�
	m_nLevelPm      = pDataSave->nLevelPm;					// �ȼ�����
	m_dwLastUpgrade = pDataSave->dwLastUpgrade;				// ���һ������ʱ��
	m_nCurLevelExp	= pDataSave->nCurExp;					// ��ǰ��������

	m_eClass	= pDataSave->eClass;						// ְҵ
	m_eClassEx	= pDataSave->eClassEx;						// ְҵ��չ
	m_nCredit	= pDataSave->nCredit;						// ���ö�
	m_nIdentity = pDataSave->nIdentity;						// ���
	m_nVIPPoint = pDataSave->nVIPPoint;						// ��Ա����
	m_nTreasureSum = pDataSave->nTreasureSum;				//�����������

	// ͨ������ȼ������Ӧ��һ������
	const tagLevelUpEffect* pLevelUp = g_attRes.GetLevelUpEffect(pDataSave->nLevel);

	SetBaseAttValue(ERA_Physique, pLevelUp->n16RoleAtt[ERA_Physique]);
	SetBaseAttValue(ERA_Strength, pLevelUp->n16RoleAtt[ERA_Strength]);
	SetBaseAttValue(ERA_Pneuma, pLevelUp->n16RoleAtt[ERA_Pneuma]);
	SetBaseAttValue(ERA_InnerForce, pLevelUp->n16RoleAtt[ERA_InnerForce]);
	SetBaseAttValue(ERA_Technique, pLevelUp->n16RoleAtt[ERA_Technique]);
	SetBaseAttValue(ERA_Agility, pLevelUp->n16RoleAtt[ERA_Agility]);

	// ����ĳЩ��������
	SetBaseAttValue(ERA_MaxHP,			pLevelUp->n16HP);
	SetBaseAttValue(ERA_MaxMP,			pLevelUp->n16MP);
	SetBaseAttValue(ERA_MaxVitality,	pLevelUp->n16Vitality);

	// ����������Ե�Ͷ��ֵ������һ������
	ModBaseAttValue(ERA_Physique, m_nAttPointAdd[ERA_Physique]);
	ModBaseAttValue(ERA_Strength, m_nAttPointAdd[ERA_Strength]);
	ModBaseAttValue(ERA_Pneuma, m_nAttPointAdd[ERA_Pneuma]);
	ModBaseAttValue(ERA_InnerForce, m_nAttPointAdd[ERA_InnerForce]);
	ModBaseAttValue(ERA_Technique, m_nAttPointAdd[ERA_Technique]);
	ModBaseAttValue(ERA_Agility, m_nAttPointAdd[ERA_Agility]);
    

	// Jason 2010-5-24 v2.2.0
	//SetAttValue(ERA_Soar,pDataSave->nSoaringValue);
	//m_nAtt[ERA_Soar] = pDataSave->nSoaringValue;
	//m_nAtt[ERA_Toughness] = pDataSave->nToughness;
	int v = pDataSave->nSoaringValue;
	switch( v )
	{
	case ESV_NotSoaring:
	case ESV_SoaringUp:
	case ESV_NotCompletedSoaring:
		break;
	default:
		v = 0;
		break;
	}
	SetBaseAttValue(ERA_Soar,v);
	SetBaseAttValue(ERA_Toughness,pDataSave->nToughness);
}

//---------------------------------------------------------------------------------
// ��ʼ������
//---------------------------------------------------------------------------------
VOID Role::InitSkill(const BYTE* &pData, INT32 nNum)
{
#if 0 //��ͨ����Ϊ�������ܣ�����Ҫ��ʱ�����
	// ���ȼ�����ͨ��������
	for(INT n = 0; n < X_NORMAL_ATTACK_SKILL_NUM; n++)
	{
		DWORD dwSkillID = NORMAL_ATTACK_SKILL_ID[n];
		if( !P_VALID( g_attRes.GetSkillProto(Skill::CreateTypeID(dwSkillID, 1)) ) )
		{
			continue;
		}
		Skill* pSkill = new Skill(dwSkillID, 1, 0, 0, 0);
		AddSkill(pSkill, FALSE);
	}
#endif
	// ���ر���ļ���
	if( nNum <= 0 ) return;

	const tagSkillSave* pSkillSave = (const tagSkillSave*)pData;

	for(INT n = 0; n < nNum; n++)
	{
		if( !P_VALID( g_attRes.GetSkillProto(Skill::CreateTypeID(pSkillSave->dwID, pSkillSave->nSelfLevel + pSkillSave->nLearnLevel)) ) )
		{
			pSkillSave++;
			continue;
		}

		const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(Skill::CreateTypeID(pSkillSave->dwID, pSkillSave->nSelfLevel + pSkillSave->nLearnLevel));
		if( P_VALID(pForceSkillProto))
		{
			if( !pForceSkillProto->bForerver )
			{
				//�����Ѿ����˼�����
				if( CalcTimeDiff(GetCurrentDWORDTime(), pSkillSave->dwActiveTime) > 0 )
				{
					g_dbSession.SendRemoveSkill( GetID(), pSkillSave->dwID );
					pSkillSave++;
					continue;
				}
			}
		}

		Skill* pSkill = new Skill(pSkillSave->dwID, pSkillSave->nSelfLevel, pSkillSave->nLearnLevel, pSkillSave->nCoolDown, pSkillSave->nProficiency, pSkillSave->dwActiveTime);

		// �����ܼ��뵽�б���
		AddSkill(pSkill, FALSE);

		if( P_VALID(pForceSkillProto) && (!pForceSkillProto->bForerver) )
		{
			m_listSkillActiveTime.PushBack(pSkillSave->dwID);
		}

		pSkillSave++;
	}

	// ����ָ��
	pData = (const BYTE*)((tagSkillSave*)pData + nNum);
	//pData = (const BYTE*)pSkillSave;
}

//-------------------------------------------------------------------------------
// ��ʼ��״̬
//-------------------------------------------------------------------------------
VOID Role::InitBuff(const BYTE* &pData, INT32 nNum)
{
	if( nNum <= 0 ) return;

	INT nBeBuffIndex = 0;
	INT nDeBuffIndex = MAX_BENEFIT_BUFF_NUM;

/*	const tagBuffSave* pEnd = (tagBuffSave*)pData + nNum;*/
	DWORD dwVipNetbarBuff = GT_INVALID,dwVipNetbarInstBuffID = GT_INVALID;

	for(INT n = 0; n < nNum; n++)
	{
		tagBuffSave* pBuffSave = (tagBuffSave*)pData;

		const tagBuffProto* pProto = g_attRes.GetBuffProto(Buff::GetTypeIDFromIDAndLevel(pBuffSave->dwBuffID, pBuffSave->n8Level));
		if( !P_VALID(pProto) ) goto next_buff;

		// �������߼�ʱBuff
		if (pProto->bOfflineConsume)
		{
			// ��ȡ��ǰʱ��͸�����ϴ����ߵ�ʱ���(��λ����)
			DWORD dwOfflineTick = CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime) * TICK_PER_SECOND;
			
			// ����Buffʣ�����ʱ��(��ʹDWORD=>INT32Ҳû����)
			if (dwOfflineTick > MAX_BUFF_PERSIST_TICK)
			{
				goto next_buff;
			}
			else if ( pProto->nPersistTick > (INT32)dwOfflineTick + pBuffSave->nPersistTick )
			{
				pBuffSave->nPersistTick += (INT32)dwOfflineTick;
			}
			else
			{
				goto next_buff;
			}
		}

		// ֮ǰ������buff������û�������ɵ�¼������Ч����
		dwVipNetbarBuff = GT_INVALID;
		dwVipNetbarInstBuffID = GT_INVALID;
		// û���ڽ������ɣ����ɼӳɵ�buff�������
		if( !g_VipNetBarMgr.IsInVipNetbar(this,&dwVipNetbarBuff,&dwVipNetbarInstBuffID) )
		{
			if( g_VipNetBarMgr.IsVipNetbarBuff(pProto->dwID) )
				goto next_buff;
		}
		// ���ɹ��ܹرգ�Ҳ��������������buff
		if( g_VipNetBarMgr.IsOpen() == FALSE )
		{
			if( g_VipNetBarMgr.IsVipNetbarBuff(pProto->dwID) )
				goto next_buff;
		}
		// ��������ɵĸ�������buff
		if( g_VipNetBarMgr.IsVipNetbarInstBuff( pProto->dwID ) )
			goto next_buff;

		// ���������Buff
		INT nIndex = GT_INVALID;
		if( pProto->bBenifit )
		{
			nIndex = nBeBuffIndex;
			// ����Buff����������
			if( nIndex >= MAX_BENEFIT_BUFF_NUM ) continue;
			++nBeBuffIndex;
		}
		// �������к�Buff
		else
		{
			nIndex = nDeBuffIndex;
			// �к�Buff����������
			if( nIndex >= MAX_BUFF_NUM ) continue;
			++nDeBuffIndex;
		}

		m_Buff[nIndex].Init(this, pBuffSave, nIndex);
		m_mapBuff.Add(m_Buff[nIndex].GetID(), &m_Buff[nIndex]);

next_buff:
		// �ۼ�ָ��
		pData += sizeof(tagBuffSave) - 1 + pBuffSave->n8ModifierNum * sizeof(DWORD);
	}

/*	pData = (const BYTE*)pEnd;*/
}

//-------------------------------------------------------------------------------
// ��ʼ�������Ʒ(װ��)
//-------------------------------------------------------------------------------
VOID Role::InitItem(const BYTE* &pData, INT32 nNum)
{
	INT32 nItemSize		= sizeof(tagItem);
	INT32 nEquipSize	= sizeof(tagEquip);
	
	// ��������Ϣ
	ZeroMemory(&m_AvatarEquipEquip, SIZE_AVATAR_EQUIP);
	ZeroMemory(&m_AvatarEquipFashion, SIZE_AVATAR_EQUIP);
	ZeroMemory(&m_AvatarEquipWardrobe, SIZE_AVATAR_EQUIP);
	
	DWORD dwErrorCode;
	TList<tagItem *> listItem;
	const tagItem	*pTmpItem	= NULL;
	tagItem			*pNewItem	= NULL;
	BYTE * p = (BYTE*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		nItemSize		= sizeof(tagItem);
		nEquipSize	= sizeof(tagEquip);
		INT32 * pLen = (INT32*)p;
		p += sizeof(INT32);

		pTmpItem = (const tagItem *)p;
		// �����Ʒ�Ƿ��ڽ����������ʱ��
		if(pTmpItem->dwUnlockTime != 0)
		{
			GetItemMgr().Add2UnlockCDTimeMap(pTmpItem->n64Serial, pTmpItem->dwUnlockTime);
		}

		if(!MIsEquipment(pTmpItem->dwTypeID))
		{
			nItemSize = *pLen;
			pNewItem = new tagItem;
			if(*pLen >= sizeof(tagItem))
			{
				IFASTCODE->MemCpy(pNewItem, pTmpItem, sizeof(tagItem));
				pNewItem->pProtoType = g_attRes.GetItemProto(pTmpItem->dwTypeID);
			}
			else
			{
				delete pNewItem;
				p += *pLen;
				continue;
			}
			if( !P_VALID(pNewItem->pProtoType) )
			{
				// ���û�а취
				p += *pLen;
				delete pNewItem;
				continue;
			}

		}
		else
		{
			pNewItem = new tagEquip;
			((tagEquip*)pNewItem)->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
			if( *pLen >= sizeof(tagEquip) )
			{
				IFASTCODE->MemCpy(pNewItem, pTmpItem, nEquipSize);
				pNewItem->pProtoType = g_attRes.GetEquipProto(pTmpItem->dwTypeID);
			}
			else // ���󣬻���ʱ��Ӧ�û������ݣ��������١�
			{
				p += *pLen;
				delete (tagEquip*)pNewItem;
				continue;
			}
			if( !P_VALID(pNewItem->pProtoType) )
			{
				nEquipSize = *pLen;
				// ���û�а취�жϷ�����
				p += *pLen;
				delete (tagEquip*)pNewItem;
				continue;
			}
			if (MIsSoulCrystalType(pNewItem->pProtoType->eType))
			{
				delete (tagEquip*)pNewItem ;
				tagSoulCrystal * pSoulCrystal = new tagSoulCrystal;
				pSoulCrystal->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
				pNewItem = pSoulCrystal;
				if( *pLen >= sizeof(tagSoulCrystal) )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, sizeof(tagSoulCrystal));
					pNewItem->pProtoType = g_attRes.GetEquipProto(pNewItem->dwTypeID);
				}
				else
				{
					p += *pLen;
					delete pSoulCrystal;
					continue;
				}
			}
			if( MIsFaBaoType(pNewItem->pProtoType->eType) )
			{
				delete (tagEquip*)pNewItem ;
				if( !g_world.IsFabaoOpen() )
				{
					p += *pLen;
					continue;
				}
				tagFabao * pFabao = new tagFabao;
				pFabao->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
				pNewItem = pFabao;
				if( *pLen >= SIZE_FABAO )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, SIZE_FABAO);
				}
				else // �����ǻ��������ģ�����Ҳ�п����Ǵ���Ŀǰ�����ְ취����
				{
					if( *pLen >= SIZE_EQUIP )
					{
						IFASTCODE->MemCpy((tagEquip*)pNewItem, pTmpItem, nEquipSize);
						pFabao->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
						tagNDBC_NewFabao msg;
						memcpy(&msg .fabao,pFabao,SIZE_FABAO);
						g_dbSession.Send(&msg, msg.dwSize);
					}
					else
					{
						delete pFabao;
						p += *pLen;
						continue;
					}
				}
				pNewItem->pProtoType = g_attRes.GetFabaoProto(pTmpItem->dwTypeID);
				if( pFabao->equipSpec.byQuality >= EIQ_End /*|| pFabao->n16NativeIntelligence > 100 */|| pFabao->n16Stage > 150 || pFabao->nSlotNum > 5 )
				{
					ItemCreator::IdentifyEquip(pFabao);
					tagNDBC_UpdateEquipAtt send;
					send.equipSpecUpdate.n64Serial = pFabao->n64Serial;
					IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &pFabao->equipSpec, sizeof(tagEquipSpec));

					g_dbSession.Send(&send, send.dwSize);
					GetItemMgr().SendFabaoSpec2DB(*pFabao);
				}
				// ��ʱ
				const tagConsolidateItem* p = NULL;
				INT size_n = min(pFabao->nSlotNum,MaxWuxingSlotNum);
				pFabao->bySlotLingzhuMatch = 0;
				for(int i = 0; i < size_n; ++i)
				{
					pFabao->bySlotMacthNdx[i] = 0;
					if( P_VALID(pFabao->dwWuxingBeadID[i]) )
					{
						p = g_attRes.GetConsolidateProto(pFabao->dwWuxingBeadID[i]);
						if( P_VALID(p) )
						{
							if( p->eWuXing == pFabao->eWuxingHole[i] )
								pFabao->bySlotMacthNdx[i] = 1;
						}
					}
					if( pFabao->bySlotMacthNdx[i] )
						pFabao->bySlotLingzhuMatch++;
				}

				// ��ֹ��������ԭ���£���������-1���ǣ���֡�����
				if(pFabao->n16Stage >= 60)
				{
					if( pFabao->n16NativeIntelligence < 80 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
					else if( pFabao->n16NativeIntelligence < 90 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
					else if( pFabao->n16NativeIntelligence < 95 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
					else if( pFabao->n16NativeIntelligence >= 95 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
				}

				switch( pFabao->eFabaoStatus )
				{
				case EEFS_Waiting:
					pFabao->LogFairyBirth(EEFS_Waiting, EEFS_Normal);
					pFabao->eFabaoStatus = EEFS_Normal;
					pFabao->dwStatusChangeTime = 0;
					break;
				case EEFS_Pregnancy:
				case EEFS_Rest:
					if( pFabao->dwStatusChangeTime > 0 )
						GetItemMgr().InsertMatingFabao(pFabao);
					else
					{
						if(pFabao->eFabaoStatus == EEFS_Pregnancy)
						{
							pFabao->LogFairyBirth(EEFS_Pregnancy, EEFS_GiveBirth);
							pFabao->eFabaoStatus = EEFS_GiveBirth;
						}
						else
						{
							BOOL bNormalChg = pFabao->LogFairyBirth(pFabao->eFabaoStatus, EEFS_Normal);
							if (!bNormalChg)
								ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
							pFabao->eFabaoStatus = EEFS_Normal;
						}
						GetItemMgr().SendFabaoSpec2DB(*pFabao);
					}
					break;
				default :
					pFabao->dwStatusChangeTime = 0;
				}

				if( g_world.IsFairyMatingOpen() )
				{
					BOOL bCanBirth = TRUE;
					if( P_VALID(pFabao->pProtoType) )
					{
						tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pProtoType;
						if( pFabaoProto->bCanBirth == GT_INVALID )
							bCanBirth = FALSE;
					}
					if( bCanBirth && pFabao->byIsGivenBirthTimes == 0 && pFabao->n16Stage >= 40 && pFabao->nBirthTimes == 0 )
					{
						tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pProtoType;
						INT nStart = 0;
						if(P_VALID(pFabaoProto) && pFabaoProto->bCanBirth != GT_INVALID)
							nStart = pFabaoProto->bCanBirth;
						pFabao->nBirthTimes = (pFabao->n16Stage - 40) / 20 + 1 + nStart;
						pFabao->byIsGivenBirthTimes = 1;
						tagNDBC_UpdateFabaoGivenBirthTimes __msg;
						__msg._n64ID = pFabao->n64Serial;
						__msg.nTimes = pFabao->nBirthTimes;
						g_dbSession.Send(&__msg,__msg.dwSize);
					}
					if( !bCanBirth && pFabao->nBirthTimes > 0 )
					{
						pFabao->nBirthTimes = 0;
						tagNDBC_UpdateFabaoGivenBirthTimes __msg;
						__msg._n64ID = pFabao->n64Serial;
						__msg.nTimes = pFabao->nBirthTimes;
						g_dbSession.Send(&__msg,__msg.dwSize);
					}
				}
			}

			if( MIsHolyType(pNewItem->pProtoType->eType) )
			{
				delete (tagEquip*)pNewItem ;
				//if( !g_world.IsFabaoOpen() )
				//{
				//	p += *pLen;
				//	continue;
				//}
				tagHolyMan * pHoly = new tagHolyMan;
				pHoly->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
				pNewItem = pHoly;
				if( *pLen >= SIZE_HOLY )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, SIZE_HOLY);

					//// ���ÿ����һ���ӣ�ʥ���Ĭ��ֵ��1������СֵΪ0
					//if ( pHoly->nCoValue >= nOffMinute * 1)
					//{
					//	pHoly->nCoValue -=  nOffMinute;
					//}
					//else
					//{
					//	pHoly->nCoValue = 0;					
					//}	

					// ��������¼��������� ʥ���ÿ��������������
					if( m_LogoutTime.year != m_LoginTime.year || m_LogoutTime.month != m_LoginTime.month || m_LogoutTime.day != m_LoginTime.day )
					{
						pHoly->nToDayEatNum = 0;
					}

					// ʥ�������޸�֪ͨDB
					GetItemMgr().SendHolySpec2DB(*pHoly);
				}
				else // �����ǻ��������ģ�����Ҳ�п����Ǵ���Ŀǰ�����ְ취����
				{
					if( *pLen >= SIZE_EQUIP )
					{
						ILOG->Write(_T("ShengLingErrSize1 -%d\n"), *pLen);
						IFASTCODE->MemCpy((tagEquip*)pNewItem, pTmpItem, nEquipSize);
						pHoly->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
						tagNDBC_NewHoly msg;
						memcpy(&msg.holy,pHoly,SIZE_HOLY);
						g_dbSession.Send(&msg, msg.dwSize);
					}
					else
					{
						ILOG->Write(_T("ShengLingErrSize2 -%d\n"), *pLen);
						delete pHoly;
						p += *pLen;
						continue;
					}
				}
				pNewItem->pProtoType = g_attRes.GetHolyProto(pTmpItem->dwTypeID);
				//if( pHoly->equipSpec.byQuality >= EIQ_End /*|| pFabao->n16NativeIntelligence > 100 */|| pFabao->n16Stage > 150 || pFabao->nSlotNum > 5 )
				//{
				//	ItemCreator::IdentifyEquip(pFabao);
				//	tagNDBC_UpdateEquipAtt send;
				//	send.equipSpecUpdate.n64Serial = pFabao->n64Serial;
				//	IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &pFabao->equipSpec, sizeof(tagEquipSpec));

				//	g_dbSession.Send(&send, send.dwSize);
				//	GetItemMgr().SendFabaoSpec2DB(*pFabao);
				//}
				// ��ʱ				
			}

			if( MIsHolyEquipType(pNewItem->pProtoType->eType) )
			{
				delete (tagEquip*)pNewItem ;
				//if( !g_world.IsFabaoOpen() )
				//{
				//	p += *pLen;
				//	continue;
				//}
				tagHolyEquip * pHolyEquip = new tagHolyEquip;
				pHolyEquip->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
				pNewItem = pHolyEquip;
				if( *pLen >= SIZE_HOLYEQUIP )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, SIZE_HOLYEQUIP);					
				}
				else // �����ǻ��������ģ�����Ҳ�п����Ǵ���Ŀǰ�����ְ취����
				{
					if( *pLen >= SIZE_EQUIP )
					{
						ILOG->Write(_T("ShengLingEquipErrSize1 -%d"), *pLen);
						IFASTCODE->MemCpy((tagEquip*)pNewItem, pTmpItem, nEquipSize);
						pHolyEquip->equipSpec.byQuality = GT_INVALID; // ��byte��ֵ������255
						tagNDBC_NewHolyEquip msg;
						memcpy(&msg.holyEquip,pHolyEquip,SIZE_HOLYEQUIP);
						g_dbSession.Send(&msg, msg.dwSize);
					}
					else
					{
						ILOG->Write(_T("ShengLingEquipErrSize2 -%d"), *pLen);
						delete pHolyEquip;
						p += *pLen;
						continue;
					}
				}
				pNewItem->pProtoType = g_attRes.GetHolyEquipProto(pTmpItem->dwTypeID);
				//if( pHoly->equipSpec.byQuality >= EIQ_End /*|| pFabao->n16NativeIntelligence > 100 */|| pFabao->n16Stage > 150 || pFabao->nSlotNum > 5 )
				//{
				//	ItemCreator::IdentifyEquip(pFabao);
				//	tagNDBC_UpdateEquipAtt send;
				//	send.equipSpecUpdate.n64Serial = pFabao->n64Serial;
				//	IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &pFabao->equipSpec, sizeof(tagEquipSpec));

				//	g_dbSession.Send(&send, send.dwSize);
				//	GetItemMgr().SendFabaoSpec2DB(*pFabao);
				//}
				// ��ʱ				
			}


			//if( MIsFaBaoType(pNewItem->pProtoType->eType) )
			//	pTmpItem = (const tagItem*)((BYTE*)pTmpItem + SIZE_FABAO);
			//else
			//	pTmpItem = (const tagItem*)((BYTE*)pTmpItem + nEquipSize);
		}
		p += *pLen;

		if(!P_VALID(pNewItem->pProtoType))
		{
			ASSERT(P_VALID(pNewItem->pProtoType));
			MAttResCaution(_T("item/equip"), _T("typeid"), pNewItem->dwTypeID);
			IMSG(_T("InitItem failed The item(SerialNum: %lld) hasn't found proto type!\n"), pNewItem->n64Serial);
			Destroy(pNewItem);
			continue;
		}

		pNewItem->eStatus = EUDBS_Null;
		pNewItem->pScript = g_ScriptMgr.GetItemScript( pNewItem->dwTypeID);
		
		dwErrorCode = Put2Container(pNewItem);
		if(dwErrorCode != E_Success)
		{
			if(dwErrorCode != E_Item_Place_NotFree && dwErrorCode != E_Offline_Role_Sprite && dwErrorCode != E_Item_EquipBar_Error )
			{
				ASSERT(0);
				Destroy(pNewItem);
				continue;
			}

			listItem.PushBack(pNewItem);
		}
	}


	// ����Ƿ�����λ���ظ����µĲ�����ӵ�������
	while(listItem.Size() != 0)
	{
		pNewItem = listItem.PopFront();

		// ����Ǳ�����ֿ��е���Ʒ�������λ
		switch(pNewItem->eConType)
		{
		case EICT_Bag:
			IMSG(_T("Ignor bag overlap for role<roleid:%u> item<serial: %lld> Container<id:%u> Position<index:%u>\n"), m_dwID, pNewItem->n64Serial, pNewItem->eConType, pNewItem->n16Index);
			ILOG->Write(_T("Ignor bag overlap for role<roleid:%u> item<serial: %lld> Container<id:%u> Position<index:%u>\n"), m_dwID, pNewItem->n64Serial, pNewItem->eConType, pNewItem->n16Index);
			//Destroy(pNewItem);
			if (FALSE == GetItemMgr().GetBag().OverLappedItemHandler(pNewItem))
				GetItemMgr().GetBaiBaoBag().OverLappedItemHandler(pNewItem);
			break;
		case EICT_Equip:
// 			dwErrorCode = GetItemMgr().Add2Bag(pNewItem, (DWORD)ELCID_Bag_PosOverlap, FALSE, FALSE);
// 			if(E_Con_NotEnoughSpace == dwErrorCode)
// 			{
// 				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld\n> load failed!\n"), 
// 					EICT_Bag, pNewItem->n64Serial);
// 				IMSG(_T("Make some places and login again can solve this problem!\n"));				
// 			}
// 			if(dwErrorCode != E_Success)
// 				Destroy(pNewItem);
			if (FALSE == GetItemMgr().GetBag().OverLappedItemHandler(pNewItem))
				GetItemMgr().GetBaiBaoBag().OverLappedItemHandler(pNewItem);
			break;
		case EICT_RoleWare:
			dwErrorCode = GetItemMgr().Add2RoleWare(pNewItem, (DWORD)ELCID_RoleWare_PosOverlap, FALSE, FALSE);
			if(E_Con_NotEnoughSpace == dwErrorCode)
			{
				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld> load failed!\n"), 
					EICT_RoleWare, pNewItem->n64Serial);
				IMSG(_T("Make some places and login again can solve this problem!\n"));
			}
			if(dwErrorCode != E_Success)
				if (FALSE == GetItemMgr().GetRoleWare().OverLappedItemHandler(pNewItem))
					GetItemMgr().GetBaiBaoBag().OverLappedItemHandler(pNewItem);
				//Destroy(pNewItem);
			break;
		case EICT_Family:
			{
				//if( GetFamilyID() > 0 )
				//{
				//	SetFamilyFabao(pNewItem);
				//}
				//else
				{//û�м��壬������������
					AddRoleSpriteFromFamily((tagFabao*)pNewItem);
					tagNDBC_DeleteRoleSpriteData send;
					send.dwRoleID = GetID();
					g_dbSession.Send( &send, send.dwSize );
				}
			}
			break;
		default:
			ASSERT(0);
			Destroy(pNewItem);
			continue;
		}
		
		ASSERT(E_Success == dwErrorCode);
	}

	// ����ָ��
	//pData = (const BYTE*)pTmpItem;
	pData = p;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж������Ʒ(װ��),���뵽��Ӧ��������
//-------------------------------------------------------------------------------
DWORD Role::Put2Container(tagItem *pItem)
{
	const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(pItem->dwTypeID);
	if (P_VALID(pItemScript))
	{
		if(pItemScript->CanDeleteItem(pItem->dwTypeID, pItem->n64Serial))
		{
			tagNDBC_DelItem send;
			send.n64Serial = pItem->n64Serial;
			g_dbSession.Send(&send, send.dwSize);
			return GT_INVALID;
		}
	}
	DWORD dwErrorCode = GetItemMgr().Put2Container(pItem);

	if(EICT_Equip == pItem->eConType && E_Success == dwErrorCode)
	{
		MTRANS_POINTER(p, pItem, tagEquip);
		ProcEquipEffectAtt(p, TRUE, p->n16Index);
		ResetOneEquipDisplay(p, p->n16Index);
		m_Suit.Add(p, p->n16Index, FALSE);
		m_LongHun.Add(p, p->n16Index, FALSE);
	}
	/*else if ( EICT_HolyEquip == pItem->eConType && E_Success == dwErrorCode)
	{
		MTRANS_POINTER(p, pItem, tagEquip);
		m_HolySoul.ProcEquipEffectAtt(p, TRUE, p->n16Index);
	}*/

	return dwErrorCode;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж��������Ʒ��ȴʱ��
//-------------------------------------------------------------------------------
VOID Role::InitItemCDTime(const BYTE* &pData, INT32 nNum)
{
	if(nNum <= 0)
	{
		return;
	}

	// ��ȡ��ǰʱ��͸�����ϴ����ߵ�ʱ���(��λ����)
	DWORD dwInterval = CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	if(dwInterval > (DWORD)MAX_ITEM_DCTIME)
	{
		// ����ָ��
		pData = pData + nNum * sizeof(tagCDTime);
		return;
	}

	// ת���ɺ���
	dwInterval *= 1000;
	
	ItemMgr &itemMgr = GetItemMgr();
	const tagCDTime *p = (const tagCDTime*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		if(p->dwTime > dwInterval && p->dwTime <= (DWORD)MAX_ITEM_DCTIME * 1000 )
		{
			itemMgr.Add2CDTimeMap(p->dwTypeID, p->dwTime - dwInterval);
		}

		++p;
	}

	// ����ָ��
	pData = (const BYTE*)((tagCDTime *)pData + nNum);
	//pData = (const BYTE*)p;
}

//-------------------------------------------------------------------------------
// ��ʼ�������б�
//-------------------------------------------------------------------------------
VOID Role::InitFriend(const BYTE* &pData, INT32 nNum)
{
	for(INT i = 0; i < MAX_FRIENDNUM; ++i)
	{
		m_Friend[i].dwFriendID = GT_INVALID;
		m_Friend[i].dwFriVal = 0;
		m_Friend[i].byGroup = 1;
	}

	// ��������ʱ��
	memset(m_dwBlackList, 0XFF, sizeof(m_dwBlackList));
	if(nNum <= 0)
		return;

	const tagFriendSave	*pFriend = NULL;
	DWORD dwSizeFriend = sizeof(tagFriendSave);
	pFriend = (const tagFriendSave*)pData;

	for(INT m = 0; m < nNum; ++m)
	{
		SetFriend(m, pFriend->dwFriendID, 0, pFriend->nGroupID);
		pFriend = (const tagFriendSave*)((BYTE*)pFriend + dwSizeFriend);
	}

	// ����ָ��
	pData = (const BYTE*)((tagFriendSave*)pData + nNum);
	//pData = (const BYTE*)pFriend;
}

//-------------------------------------------------------------------------------
// ����б������Ϣ
//-------------------------------------------------------------------------------
VOID Role::InitEnemy(const BYTE* &pData, INT32 nNum)
{
	const DWORD *pEnemyList = NULL;
	DWORD dwSize = sizeof(DWORD);
	pEnemyList = (const DWORD*)pData;

	// ����б��ʱ��
	memset(m_dwEnemyList, 0XFF, sizeof(m_dwEnemyList));

	for(INT i = 0; i < nNum; ++i)
	{
		SetEnemyList(i, *pEnemyList);
		pEnemyList = (const DWORD*)((BYTE*)pEnemyList + dwSize);
	}

	// ����ָ��
	pData = (const BYTE*)((DWORD*)pData + nNum);
	//pData = (const BYTE*)pEnemyList;
}

//-------------------------------------------------------------------------------
// ��ʼ�����Ѷ�
//-------------------------------------------------------------------------------
VOID Role::InitFriendValue(const BYTE* &pData, INT32 nNum)
{
	const tagFriendshipSave *pFriendSave = NULL;
	tagFriend *pFriend = NULL;
	DWORD dwSizeFriend = sizeof(tagFriendshipSave);
	pFriendSave = (const tagFriendshipSave*)pData;

	for(INT i = 0; i < nNum; ++i)
	{
		pFriend = m_mapFriend.Peek(pFriendSave->dwFriendID);

		if(P_VALID(pFriend))
			pFriend->dwFriVal = pFriendSave->nFriVal;

		pFriendSave = (const tagFriendshipSave*)((BYTE*)pFriendSave + dwSizeFriend);
	}

	// ����ָ��
	pData = (const BYTE*)((tagFriendshipSave*)pData + nNum);
	//pData = (const BYTE*)pFriendSave;
}

//-------------------------------------------------------------------------------
// ��ʼ��������
//-------------------------------------------------------------------------------
VOID Role::InitBlackList(const BYTE* &pData, INT32 nNum)
{
	const DWORD *pBlackList = NULL;
	DWORD dwSize = sizeof(DWORD);
	pBlackList = (const DWORD*)pData;

	for(INT i = 0; i < nNum; ++i)
	{
		SetBlackList(i, *pBlackList);
		pBlackList = (const DWORD*)((BYTE*)pBlackList + dwSize);
	}

	// ����ָ��
	pData = (const BYTE*)((DWORD*)pData + nNum);
	//pData = (const BYTE*)pBlackList;
}

//-------------------------------------------------------------------------------
// ��ʼ��ʦͽ
//-------------------------------------------------------------------------------
VOID Role::InitMaster(const BYTE* &pData)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	BOOL bIsMaster = IsHaveDoneQuest(MASTER_QUALIFICATION_QUEST_ID);
	if (bIsMaster)
	{
		m_nJingWuPoint	= p->nJingWuPoint;
	}

	m_nQinWuPoint	= p->nQinWuPoint;
	m_nLevelUpBonus = p->nLevelUpBonus;
	m_nRewardNum	= p->nRewardNum;
	m_ApprenticeData = p->sApprentice;

	for (int i=0; i!=MAX_APPRENTICE_NUM; ++i)
	{
		if (m_ApprenticeData.dwData[i] != 0)
		{
			m_mapApprentice.Add(m_ApprenticeData.dwData[i], m_ApprenticeData.dwData[i]);
		}
	}

	// ����ָ��
	pData = (const BYTE*)((BYTE*)pData + sizeof(tagMasterApprentice2DB));

	// ��ȡʦ��ID
	MTRANS_POINTER(pTmp, pData, tagMasterApprentice2DB);
	m_dwMasterID = pTmp->dwID;
	if (!bIsMaster && m_dwMasterID == GT_INVALID)
	{
		// ���û��ʦ����������ʦ������վ������
		m_nJingWuPoint = 0;
	}

	// ����ָ��
	pData = (const BYTE*)((BYTE*)pData + sizeof(tagMasterApprentice2DB));
}

//-------------------------------------------------------------------------------
// ��ʼ��ͬ��
//-------------------------------------------------------------------------------
VOID Role::InitClassMates(const BYTE* &pData)
{
	MTRANS_POINTER(p, pData, tagClassMateData);
	m_ClassMateData = *p;

	// ����ָ��
	pData = (const BYTE*)((BYTE*)pData + sizeof(tagClassMateData));
}

//-------------------------------------------------------------------------------
// ��ʼ��ʵ��ֵ----�����ڽ�ɫ��ʼ����ʱ���õ�����ÿ�α����ɫ��Ϣ��ʱ��Ҳ�õ�
//-------------------------------------------------------------------------------
VOID Role::InitStrength()
{
	UpdateEquipValue();
	UpdateYaoJingValue();				// ����������ʵ��ֵ
	UpdateBaseStrength();
	UpdateAdvanceStrength();
	//UpdateConsumptiveStrength();
	UpdateStrength();
}

//-------------------------------------------------------------------------------
// �������ݿ��ж�����ҵ�ǰ�����񼯺�
//-------------------------------------------------------------------------------
VOID Role::InitCurrentQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestSave* pQuestSave = (const tagQuestSave*)pData;
	const tagQuestSave* pEndQuestSave = pQuestSave + nNum;
	const tagQuestProto* pProto = NULL;
	INT nIndex = 0;

	for ( ; pQuestSave < pEndQuestSave; ++pQuestSave)
	{
		pProto = g_questMgr.GetQuestProto(pQuestSave->u16QuestID);
		if( !P_VALID(pProto) ) 
		{
			MAttResCaution(_T("QuestFile"), _T("questID"), pQuestSave->u16QuestID);
			continue;
		}

		m_Quests[nIndex].Init(pQuestSave, this, nIndex);
		m_mapCurrentQuests.Add(m_Quests[nIndex].GetID(), &m_Quests[nIndex]);
		++nIndex;
	}
	
	pData = (const BYTE*)((tagQuestSave*)pData + nNum);
	//pData = (const BYTE *)pEndQuestSave;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж�������Ѿ���ɵ����񼯺�
//-------------------------------------------------------------------------------
VOID Role::InitCompleteQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestDoneSave* pQuestDoneSave = (const tagQuestDoneSave*)pData;

	for(INT32 i = 0; i < nNum; ++i)
	{
		tagQuestDoneSave* pDoneQuest = new tagQuestDoneSave;
		IFASTCODE->MemCpy(pDoneQuest, pQuestDoneSave, sizeof(tagQuestDoneSave));
		m_mapCompleteQuests.Add(pDoneQuest->u16QuestID, pDoneQuest);
		
		++pQuestDoneSave;
	}

	// ����ָ��
	pData = (const BYTE*)((tagQuestDoneSave*)pData + nNum);
	//pData = (const BYTE*)pQuestDoneSave;
}

//-------------------------------------------------------------------------------
// �������ݿ��ж������ÿ���Ѿ���ɵ������������
//-------------------------------------------------------------------------------
VOID Role::InitDailyCompleteQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestDoneSave* pQuestDoneSave = (const tagQuestDoneSave*)pData;

	for(INT32 i = 0; i < nNum; ++i)
	{
		tagQuestDoneSave* pDoneQuest = new tagQuestDoneSave;
		IFASTCODE->MemCpy(pDoneQuest, pQuestDoneSave, sizeof(tagQuestDoneSave));
		m_mapDailyCompleteQuests.Add(pDoneQuest->u16QuestID, pDoneQuest);

		++pQuestDoneSave;
	}

	
	pData = (const BYTE*)((tagQuestDoneSave*)pData + nNum);
}



//-------------------------------------------------------------------------------
// ���������ʼ��ǰ����
//-------------------------------------------------------------------------------
VOID Role::CalInitAtt()
{
	// �ȱ���һЩ�������ֵ�ı������
	INT nHP				=	m_nAtt[ERA_HP];
	INT nMP				=	m_nAtt[ERA_MP];
	INT nVitality		=	m_nAtt[ERA_Vitality];
	INT nEndurance		=	m_nAtt[ERA_Endurance];
	INT nKnowledge		=	m_nAtt[ERA_Knowledge];
	INT nInjury			=	m_nAtt[ERA_Injury];
	INT nMorale			=	m_nAtt[ERA_Morale];
	INT nMorality		=	m_nAtt[ERA_Morality];
	INT nCulture		=	m_nAtt[ERA_Culture];
	INT nAttPoint		=	m_nAtt[ERA_AttPoint];
	INT nTalentPoint	=	m_nAtt[ERA_TalentPoint];
	INT nHostility		=	m_nAtt[ERA_Hostility];
	INT nRage			=	m_nAtt[ERA_Rage];
	INT nSpirit			=	m_nAtt[ERA_Spirit];
	INT nWuxun			=	m_nAtt[ERA_WuXun];
	INT nWuJi			=	m_nAtt[ERA_WuJi];
	INT nWeary			=	m_nAtt[ERA_Weary];
	INT nVipLevel		= m_nAtt[ERA_VipLevel];
	
	INT	nGodhead		= m_nAtt[ERA_God_Godhead];
	INT	nFaith			= m_nAtt[ERA_God_Faith];
	INT nCondenced		= m_nAtt[ERA_God_Condenced];

	INT nFlowerNum		= m_nAtt[ERA_FlowerNum];// �ʻ�
	INT nEggNum			= m_nAtt[ERA_EggsNum];// ����
	INT nHoly			= m_nAtt[ERA_Holy];	// ʥ��ֵ
	// ����Ԫ���˺����Լ�����
	//INT nEEI_Fire		=	m_nAtt[ERA_EEI_Fire];
	//INT nEEI_Water		=	m_nAtt[ERA_EEI_Water];
	//INT nEEI_Earth		=	m_nAtt[ERA_EEI_Earth];
	//INT nEEI_Wind		=	m_nAtt[ERA_EEI_Wind];
	//INT nEEI_Bright		=	m_nAtt[ERA_EEI_Bright];
	//INT nEEI_Dark		=	m_nAtt[ERA_EEI_Dark];
	//INT nEER_Fire		=	m_nAtt[ERA_EER_Fire];
	//INT nEER_Water		=	m_nAtt[ERA_EER_Water];
	//INT nEER_Earth		=	m_nAtt[ERA_EER_Earth];
	//INT nEER_Wind		=	m_nAtt[ERA_EER_Wind];
	//INT nEER_Bright		=	m_nAtt[ERA_EER_Bright];
	//INT nEER_Dark		=	m_nAtt[ERA_EER_Dark];

	for(INT n = 0; n < ERA_End; n++)
	{
		m_nAtt[n] = CalAttMod(m_nBaseAtt[n], n);
	}

	// ͨ��һ�����Եĵ�ǰֵ�����������
	m_nAtt[ERA_MaxHP]		=	m_nAtt[ERA_Physique] * 6 + m_nBaseAtt[ERA_MaxHP];
	m_nAtt[ERA_MaxMP]		=	m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
	m_nAtt[ERA_ExAttack]	=	m_nAtt[ERA_Strength] * 10;
	m_nAtt[ERA_ExDefense]	=	m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
	m_nAtt[ERA_InAttack]	=	m_nAtt[ERA_InnerForce] * 10;
	m_nAtt[ERA_InDefense]	=	m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
	m_nAtt[ERA_AttackTec]	=	0;//m_nAtt[ERA_Technique] * 6;
	m_nAtt[ERA_DefenseTec]	=	0;//m_nAtt[ERA_Agility] * 4;
	m_nAtt[ERA_HitRate]		=	0;//m_nAtt[ERA_Technique] * 10;
	m_nAtt[ERA_Dodge]		=	0;//m_nAtt[ERA_Agility] * 10;
	const tagLevelUpEffect* pLevelupEff = g_attRes.GetLevelUpEffect(GetLevel());
	if( P_VALID(pLevelupEff) )
		m_nAtt[ERA_MaxEndurance]=	100 + pLevelupEff->n16Durance;
	else
		m_nAtt[ERA_MaxEndurance]=	0;//(m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
	m_nAtt[ERA_WuXun]		=	nWuxun;
	m_nAtt[ERA_WuJi]		=	nWuJi;
	m_nAtt[ERA_Weary]		=	nWeary;
	m_nAtt[ERA_VipLevel] = nVipLevel;

	m_nAtt[ERA_God_Godhead] = nGodhead;
	m_nAtt[ERA_God_Faith] = nFaith;
	m_nAtt[ERA_God_Condenced] = nCondenced;

	m_nAtt[ERA_FlowerNum] = nFlowerNum;
	m_nAtt[ERA_EggsNum] = nEggNum;
	m_nAtt[ERA_Holy] = nHoly;

	// ����������Լӳ�
	for(INT n = ERA_AttB_Start; n < ERA_AttB_End; n++)
	{
		m_nAtt[n] = CalAttMod(m_nAtt[n], n);
	}

	//���ϼ���������Ӱ��
	for(int i = 0; i < EFSA_Num; ++i)
	{
		if( m_nAbility[i] >= 0 )
		{
			ERoleAttribute eType = GetRoleIndexFromFamilySprite((EFamilySpriteAtt)i);
			if(eType > ERA_Null && eType < ERA_End)
			{
				m_nAtt[eType] += m_nAbility[i];
				SetRecalAbility((EFamilySpriteAtt)i,FALSE);
			}
		}
	}

	// ����ĳЩ����ǰ���ԡ������ֵ֮�����Сֵ
	if( nHP < 0 )
	{
		m_nAtt[ERA_HP] = m_nAtt[ERA_MaxHP];
	}
	else
	{
		m_nAtt[ERA_HP] = min(nHP, m_nAtt[ERA_MaxHP]);
	}

	if( nMP < 0 )
	{
		m_nAtt[ERA_MP] = m_nAtt[ERA_MaxMP];
	}
	else
	{
		m_nAtt[ERA_MP] = min(nMP, m_nAtt[ERA_MaxMP]);
	}

	if( nVitality < 0 )
	{
		m_nAtt[ERA_Vitality] = m_nAtt[ERA_MaxVitality];
	}
	else
	{
		m_nAtt[ERA_Vitality] = min(nVitality, m_nAtt[ERA_MaxVitality]);
	}

	if( nEndurance < 0 )
	{
		m_nAtt[ERA_Endurance] = m_nAtt[ERA_MaxEndurance];
	}
	else
	{
		m_nAtt[ERA_Endurance] = min(nEndurance, m_nAtt[ERA_MaxEndurance]);
	}

	if(nSpirit < 0)
	{
		m_nAtt[ERA_Spirit] = g_attRes.GetAttMin(ERA_Spirit);
	}
	else
	{
		m_nAtt[ERA_Spirit] = min(nSpirit, g_attRes.GetAttMax(ERA_Spirit));
	}
	// �����ϲ���������Ըı�ĵ�ǰ����
	m_nAtt[ERA_Knowledge]	=	nKnowledge;
	m_nAtt[ERA_Injury]		=	nInjury;
	m_nAtt[ERA_Morale]		=	nMorale;
	m_nAtt[ERA_Morality]	=	nMorality;
	m_nAtt[ERA_Culture]		=	nCulture;
	m_nAtt[ERA_AttPoint]	=	nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	nTalentPoint;
	m_nAtt[ERA_Hostility]	=	nHostility;
	m_nAtt[ERA_Rage]		=	nRage;
	// ����Ԫ���˺��Լ�����
	//m_nAtt[ERA_EEI_Fire]		=	nEEI_Fire		;
	//m_nAtt[ERA_EEI_Water]		=	nEEI_Water	;
	//m_nAtt[ERA_EEI_Earth]		=	nEEI_Earth	;
	//m_nAtt[ERA_EEI_Wind]		=	nEEI_Wind		;
	//m_nAtt[ERA_EEI_Bright]	=	nEEI_Bright	;
	//m_nAtt[ERA_EEI_Dark]		=	nEEI_Dark		;
	//m_nAtt[ERA_EER_Fire]		=	nEER_Fire		;
	//m_nAtt[ERA_EER_Water]		=	nEER_Water	;
	//m_nAtt[ERA_EER_Earth]		=	nEER_Earth	;
	//m_nAtt[ERA_EER_Wind]		=	nEER_Wind		;
	//m_nAtt[ERA_EER_Bright]	=	nEER_Bright	;
	//m_nAtt[ERA_EER_Dark]		=	nEER_Dark		;

	// ����ĳЩ��������Ա仯������
	m_fXZSpeed			= X_DEF_XZ_SPEED;
	m_fYSpeed			= X_DEF_Y_SPEED; 
	m_fSwimXZSpeed		= X_DEF_XZ_SPEED; 
	m_fDropXZSpeed		= X_DEF_DROP_XZ_SPEED; 
	m_fSlideSpeed		= X_DEF_SLIDE_SPEED;

	m_fMountXZSpeed *= FLOAT(m_nAtt[ERA_Speed_Mount]) / 10000.0f;
	m_fXZSpeed *= FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f;
	m_fYSpeed *= FLOAT(m_nAtt[ERA_Speed_Y]) / 10000.0f;
	m_fSwimXZSpeed *= FLOAT(m_nAtt[ERA_Speed_Swim]) / 10000.0f;
	m_Size *= FLOAT(m_nAtt[ERA_Shape]) / 10000.0f;
	// todo: pk״̬

	// ʵ��ϵͳ
	InitStrength();
	m_nAtt[ERA_Shili] = m_Strength.nStrength;

	//����ϵͳ
	if(g_attRes.IsSpiritSysOn() && GT_INVALID != GetOnlineTime() && GetLevel() >= g_attRes.SpiritStartLevel()) //��������� �������40��
	{
		if(m_nAtt[ERA_Spirit] < g_attRes.GetAttDefRole(ERA_Spirit))	// ���߻ظ�������Ĭ�ϵ���
		{
			DWORD offlineTime = CalcTimeDiff(m_LoginTime, m_LogoutTime) / 60; // �����ϴ�����ʱ��
			if(offlineTime > 0)
			{
				m_nAtt[ERA_Spirit] += offlineTime * g_attRes.GetSpiritRSpeed(); // �����ٶ� �ָ���ָ������
				m_nAtt[ERA_Spirit] = min(m_nAtt[ERA_Spirit], g_attRes.GetAttDefRole(ERA_Spirit)); // ϵͳ�жϲ�����Ĭ�ϵ���
			}			
		}
		m_fSpiRate = CalSpiritRate(m_nAtt[ERA_Spirit]);	// ��Ҫ����
	}
	else
	{
		m_nAtt[ERA_Spirit] = g_attRes.GetAttDefRole(ERA_Spirit);
	}

	//����Ԫ�����Լӳ�
	/*if ( m_HolySoul.IsActivated() )
	{
		m_HolySoul.HolyChangeRoleAtt();
	}*/


	// ��������ֶ�
	ClearAttRecalFlag();
}

//-----------------------------------------------------------------------
// �����ʼ״̬
//-----------------------------------------------------------------------
VOID Role::CalInitState()
{
	// ����PK״̬
	CalPKState(FALSE);

	// ��������״̬
	if( GetAttValue(ERA_HP) <= 0 )
	{
		SetState(ES_Dead, FALSE);
	}
}

//------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------
VOID Role::Online(BOOL bFirst/* =FALSE */)
{
	if( bFirst )
		VirginOnline();

	// �������ߵĽű�����
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleOnline(this);
	}
}

//------------------------------------------------------------------------
// ��һ������
//------------------------------------------------------------------------
VOID Role::VirginOnline()
{
	// ��������ʱ��
	m_nCurOnlineTime = 0;
	m_nOnlineTime = 0;

	// ���ó�����ͼ�͸����ͼ
	m_dwRebornMapID = m_dwMapID = g_mapCreator.GetBornMapID();

	// �õ�һ������ĳ�����
	Vector3 vBornPos = g_mapCreator.RandGetOneBornPos();

	// ���һ������
	FLOAT fYaw = 180;//FLOAT(IUTIL->Rand() % 360);

	// ������
	m_MoveData.Reset(vBornPos.x, vBornPos.y, vBornPos.z, cosf(fYaw * 3.1415927f / 180.0f), 0.0f, sinf(fYaw * 3.1415927f / 180.0f));

	// ���ó������޵Ľű�����
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleFirstOnline(this);
	}
}

//------------------------------------------------------------------------
// ��ʼ�������������
//------------------------------------------------------------------------
VOID Role::InitChestState()
{
	m_TreasureState.nChestSerial = 0;
	m_TreasureState.nKeySerial = 0;
	m_TreasureState.dwChestTypeID = 0;
	m_TreasureState.dwKeyTypeID = 0;
	m_TreasureState.ChestItem.dwTypeID = 0;
	m_TreasureState.ChestItem.nNum = 0;
	m_TreasureState.ChestItem.nTableID = 0;
	m_TreasureState.ChestItem.fBeginRate = 0.0;
	m_TreasureState.ChestItem.fNormalRate = 0.0;
	m_TreasureState.ChestItem.fRoleRate = 0.0;
	m_TreasureState.ChestItem.fServerRate = 0.0;
	
}

VOID Role::InitTrainState(const BYTE* &pData, INT32 nNum)
{
	const tagTrainState* pTrainStateSave = (const tagTrainState*)pData;

	// ���ȴ�����ԭ�ͱ��ж�ȡ���е�����
	tagTrainAttProto* pTrainAtt = NULL;
	TMap<DWORD, tagTrainAttProto*>& TrainAttProto = g_attRes.GetTrainAttProto();
	TrainAttProto.ResetIterator();
	while (TrainAttProto.PeekNext(pTrainAtt))
	{
		if (!P_VALID(pTrainAtt)) return;

		tagTrainState* pTrainState = new tagTrainState;
		if (!P_VALID(pTrainState))	return;
		
		pTrainState->dwTrainID = pTrainAtt->dwTrainID;
		pTrainState->eTrainState = ETS_Close;
		pTrainState->nCompleteness = 0;

		m_mapTrainStates.Add(pTrainState->dwTrainID, pTrainState);
	}

	// �����ݿ��ж�ȡ�����ݺϲ���m_mapTrainStates����ȥ
	for(INT32 i = 0; i < nNum; ++i)
	{
		if (P_VALID(pTrainStateSave))
		{
			tagTrainState * pTrainState = m_mapTrainStates.Peek(pTrainStateSave->dwTrainID);
			if (P_VALID(pTrainState))
			{
				pTrainState->eTrainState = pTrainStateSave->eTrainState;
				pTrainState->nCompleteness = pTrainStateSave->nCompleteness;
			}
			++pTrainStateSave;
		}
	}

	// ����ָ��
	pData = (const BYTE*)((tagTrainState*)pData + nNum);

	// ���ͻ��˷�������״̬��
	SendTrainStateList2Client();
}


VOID Role::InitCurMonthOnlineTime(const BYTE* &pData, INT nNum)
{
	tagMonthOnlineTime* pMonthOnlineTime = (tagMonthOnlineTime*)pData;
	if (P_VALID(pMonthOnlineTime) && nNum == 1)
	{
		for (int nMonthIdx = EM_January; nMonthIdx < EM_End; nMonthIdx++)
		{	
			m_CurMonthOnlineTime.n16OnlineTime[nMonthIdx] = (nNum > 0)?pMonthOnlineTime->n16OnlineTime[nMonthIdx]:0;
		}
		m_CurMonthOnlineTime.dwCurTime = GetCurrentDWORDTime();
	}
	// ����ָ��
	pData = (const BYTE*)((tagMonthOnlineTime*)pData + nNum);
}
//---------------------------------------------------------------------------------
// ��ʼ������
VOID Role::InitArea(const BYTE* &pData, INT32 nNum)
{
	//���ó���ְ�������������
	m_GodAreaMgr.InitFromDB(pData,nNum);
	//���ϡ��������Чbuff
	m_GodAreaMgr.AddAreaBuff();//add by xp
}
//---------------------------------------------------------------------------------
VOID Role::SetFamilyFabao(tagItem *pItem)
{
	if (!P_VALID(pItem)) 
		return;

	if(!MIsFaBao(pItem))
		return;

	//Ŀǰ��û�еǼǵ�����
	// ������
	//Family *pFamily = g_FamilyMgr.GetFamily(GetFamilyID());
	//if( !P_VALID(pFamily) )
	//	return;

	//// �����������
	//FamilySprite* pFamilySprite = pFamily->GetFamilySprite();
	//if( !P_VALID(pFamilySprite) )
	//	return;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return;

	pFamilySprite->SetRoleSprite( GetID(), (tagFabao*)pItem );

	//pFamilySprite->UpdateRoleAtt( GetID() );
}
//---------------------------------------------------------------------------------
//��ʼ����ҵļ����������ӵ���������
VOID Role::InitAbilityFromFamilySprite()
{
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return;

	pFamilySprite->CaculateRoleAtt(GetID(),this);
}
//---------------------------------------------------------------------------------
VOID Role::InitBuyMallItemCount(const BYTE* &pData, INT32 nNum)
{
	if (nNum <= 0)
	{
		return;
	}

	tagBuyMallItemCountSaveData* p = (tagBuyMallItemCountSaveData*)pData;
	for (int i=0; i<nNum; ++i)
	{
		tagBuyMallItemCountSaveData* pTmp = p+i;
		if (P_VALID(pTmp->dwTypeID) && P_VALID(pTmp->nNum))
		{
			m_mapBuyMallItemCount.Add(pTmp->dwTypeID, pTmp->nNum);
		}
	}

	pData = (const BYTE*)((tagBuyMallItemCountSaveData*)pData + nNum);
}

VOID Role::ClearBuyMallItemCount()
{
	m_mapBuyMallItemCount.Clear();
}


//-------------------------------------------------------------------------------
// ��ʼ��ǩԪ����Ϣ
//-------------------------------------------------------------------------------
VOID Role::InitHolySoulInfo(const BYTE* &pData,INT nOffHour/*����Сʱ*/)
{
	const tagHolySoulInfoEx* pHolySoulInfo = (const tagHolySoulInfoEx*)pData;

	DWORD dwRet = m_HolySoul.Init(pHolySoulInfo,nOffHour);
	if ( E_Success != dwRet )
	{
		IMSG(_T(" HolySoul Init Failed!"));   
		ILOG->Write(_T(" HolySoul Init Failed!")); 
	}

	//����ָ��
	pData = (const BYTE*)((tagHolySoulInfoEx*)pData + nOffHour);
}

//-------------------------------------------------------------------------------
// ����Ԫ��
//-------------------------------------------------------------------------------
VOID	Role::ActiveHolySoul()
{
	bool bIsSoulActivated = m_HolySoul.IsActivated();
	if ( !bIsSoulActivated )
	{
		//����Ԫ��
		DWORD dwRet = m_HolySoul.ActivateSoul();
		if ( E_Success == dwRet )
		{
			tagNS_ActivateHolySoul msg_HolySoul;
			SendMessage(&msg_HolySoul,msg_HolySoul.dwSize);
		}
	}

}
//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݽṹ
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "db_session.h"

#include "../WorldDefine/chat.h"
#include "../WorldDefine/chat_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/pk_define.h"
#include "../WorldDefine/msg_talent.h"
#include "../WorldDefine/msg_pk.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/msg_stall.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_social.h"
#include "../WorldDefine/msg_prison.h"
#include "../WorldDefine/msg_map.h"
#include "../WorldDefine/exchange_define.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/msg_loot.h"
#include "../WorldDefine/msg_motion.h"
#include "../WorldDefine/miracle_define.h"
#include "../WorldDefine/msg_miracle.h"
#include "../WorldDefine/msg_city_struggle.h"
#include "../WorldDefine/msg_GroupChat.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/clergy_define.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/msg_func_npc.h"
#include "..\ServerDefine\family_define.h"

#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_quest.h"
#include "../ServerDefine/msg_skill.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/login_issue.h"
#include "../ServerDefine/msg_miracle.h"
#include "../ServerDefine/msg_log.h"
#include "../WorldDefine/motion_define.h"
#include "../ServerDefine/msg_train.h"
#include "pet_define.h"
#include "db_session.h"
#include "login_session.h"
#include "map.h"
#include "map_instance.h"
#include "world.h"
#include "map_creator.h"
#include "map_mgr.h"
#include "att_res.h"
#include "skill.h"
#include "buff.h"
#include "quest.h"
#include "quest_mgr.h"
#include "group_chat.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "item_creator.h"
#include "role_mgr.h"
#include "stall.h"
#include "group_mgr.h"
#include "social_mgr.h"
#include "script_mgr.h"
#include "title_mgr.h"
#include "pet_pocket.h"
#include "guild_mgr.h"
#include "pet_soul.h"
#include "guild_commodity.h"
#include "guild.h"
#include "MsgInfoMgr.h"
#include "city_mgr.h"
#include "WorldRankings.h"
#include "GuildBattle.h"
#include "../WorldDefine/athletics_define.h"
#include "athletics_room.h"
#include "athletics_room_mgr.h"
#include "athletics_system.h"
#include "pet_exchange.h"
#include "ClergyRank.h"
#include "world.h"
#include "war_role_mgr.h"

//#include "container_restrict.h"

#include "../WorldDefine/msg_player_preventlost.h"

#include "online_reward.h"

#include "pet_define.h"

#include "offline_reward.h"
// Jason 2010-03-02 v1.3.2 ������������
#include "guild_apply_to_join.h"

#include "city.h"

#include "guild_war_declare_mgr.h"
#include "shutdown_server.h"
#include "const_define.h"
#include "MsgInfoMgr.h"

#include "../ServerDefine/msg_god.h"
#include "family.h"
#include "family_join.h"
#include "family_mgr.h"
#include "family_sprite.h"
#include "family_sprite_mgr.h"
#include "../WorldDefine/msg_family.h"
#include "..\WorldDefine\msg_bloodbrother.h"
#include "..\WorldDefine\brother_define.h"
#include "..\WorldDefine\pulse_define.h"
#include "..\WorldDefine\msg_pulse.h"
//#include "ipg_session.h"
#include "IPGDragonfly.h"
#include "..\WorldDefine\msg_soul.h"
#ifdef ON_INTERNET_CAFE
#include "internet_cafes_session.h"
#endif

static INT64 CalOnlineRewardExp(INT level,INT64 Exp);


static DWORD OFFLINE_TIMEOUT = 4 * 60 * 60;
static BOOL		bLoongLessingOpen = TRUE;

static  DWORD dwOfflineItemType = 0;
static INT dwOfflineItemCount = 0;

static INT nOnlineRewardCount = 0;

//��������ÿ�����ĵ����������ӵ����۶�
#define		CONDENSE_NUM		30

//ÿ�����ʹ�÷�������������Ʒ�Ĵ���
#define		USE_FAMILY_SPEC_ITEM_MAX		5
//#define Hostility_OEN	
//#define ExpPenalty_OPEN
// Jason 2010-9-20 ÿ����ñ���һ�ν�ɫ����
INT	Role::m_nNeedSaveExpCounter = 10;


Role* Role::Create( DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession )
{
	return new Role(dwRoleID, pData, pSession);
}

VOID Role::Delete( Role* &pRole )
{
	SAFE_DEL(pRole);
}

Role::SaveRole::SaveRole()
{
	BYTE *byData = new BYTE[X_ROLE_BUFF_SIZE];
	ZeroMemory(byData, X_ROLE_BUFF_SIZE);
	MMSGINIT(byData, tagNDBC_SaveRole);
	m_pSaveRole = (tagNDBC_SaveRole*)(byData) ;
	m_pSaveRole->dwSize = X_ROLE_BUFF_SIZE;
}

Role::SaveRole::~SaveRole()
{
	delete [](BYTE*)m_pSaveRole;
}

VOID Role::SaveRole::Init()
{
	m_pSaveRole->dwRoleID = GT_INVALID;
	ZeroMemory(&m_pSaveRole->RoleData, X_ROLE_BUFF_SIZE - FIELD_OFFSET(tagNDBC_SaveRole, RoleData));
}

Role::SaveRole	Role::m_SaveRole;
Mutex			Role::m_SaveRoleLock;

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
Role::Role(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession)
		: Unit(dwRoleID, pData->RoleDataSave.dwMapID, 
				pData->RoleDataSave.fCoordinate[0], pData->RoleDataSave.fCoordinate[1], pData->RoleDataSave.fCoordinate[2],
				pData->RoleDataSave.fFace[0], pData->RoleDataSave.fFace[1], pData->RoleDataSave.fFace[2]),
		  m_ePKState(ERPKS_Peace), m_pStall(new Stall(this, &(pData->RoleDataSave))),
		  m_ItemMgr(this, pSession->GetSessionID(), dwRoleID, pData->RoleDataSave.n16BagSize, pSession->GetWareSize()),
		  m_CurMgr(this, pData->RoleDataSave.nBagGold, pData->RoleDataSave.nBagSilver, pData->RoleDataSave.nBagYuanBao,
					pSession->GetWareSilver(), pSession->GetBaiBaoYB(), pData->RoleDataSave.nExVolume,pData->RoleDataSave.nVIPPoint),
		  m_pSession(pSession), m_LongHun(this), m_nNeedSave2DBCountDown(MIN_ROLE_SAVE2DB_INTERVAL),
		  m_Suit(this),m_dwTeamID(GT_INVALID),m_dwGroupID(GT_INVALID),m_dwTeamInvite(GT_INVALID), m_dwOwnInstanceID(GT_INVALID), m_dwOwnInstanceMapID(GT_INVALID),
		  m_dwExportMapID(GT_INVALID), m_pScript(NULL), m_pPetPocket(NULL),	m_nMotionCounter(GT_INVALID), m_dwPartnerID(GT_INVALID),
		  ScriptData<ESD_Role>(),m_nWorldTalkCounter(GT_INVALID),m_bObjectCoolOff(FALSE), m_bSpeakOff(FALSE), /*m_bLevelUpClosedSafeGuard(FALSE),*/
		  m_dwItemTransportMapID(pData->RoleDataSave.dwItemTransportMapID),m_fItemTransportX(pData->RoleDataSave.fItemTransportX),m_fItemTransportZ(pData->RoleDataSave.fItemTransportZ),
		  m_fItemTransportY(pData->RoleDataSave.fItemTransportY),m_dwLoverID(pData->RoleDataSave.dwLoverID),m_bHaveWedding(pData->RoleDataSave.bHaveWedding),
		  m_timeLastLessing(pData->RoleDataSave.timeLastLessing),m_nLessingLevel(pData->RoleDataSave.nLessingLevel),
		  m_dwCurrentDayOnlineTime(0),m_timeOneDayFirstLogined(0),m_dateLastLessingLoong(0),m_n32UpdateOnlineReward(0),m_bOnlineRewardPromptFlag(FALSE),m_timeLastCalOnlineTime(0),
		  m_bIsOpenOfflineReward(FALSE)
		  ,m_dwFixSpiriteCDTime(0), m_dwSpiUpdateTickCount(0), m_fSpiRate(1.), m_nMountShape(0), m_nSelfShape(0), m_dwLatestOne(0), m_dwLastClearAdvenceStrength(0)
		  ,m_u16FollowPetPocketValve(5),m_u16RidingPetPocketValve(5),m_n16MiraclePracticeNum(pData->RoleDataSave.n16MiraclePracticeNum),m_dwMiracleResetTime(pData->RoleDataSave.dwMiracleResetTime),m_nValiantMeritLevel(1),
		  m_nLessingLevelEx(pData->RoleDataSave.nLessingLevelEx)
		  ,m_byTeammatesNum(0), m_byTeamFriendNum(0), m_nJingWuPoint(0), m_byTrainStateTransfered(1)
		  ,m_nGodMiraclePoints(0),bIsDailyQuestCleared(FALSE),m_dwCurScriptArea(0),m_dwMapBuffID(0),m_nIsSended(0)
		  // ��ɫս���һ�ϵͳ״̬��ʼ��
		  ,m_eAutoFightState(EAFS_Close)
		  ,m_nConsolidateTimes(0)
		  ,m_nUnSetPKTickCountDown(0)
		  ,m_dwMonsterKillCount(0), m_dwRoleKillCount(0),m_bClientReqShortCutBarData(FALSE),m_bLoadFromDBFlag(FALSE),m_bIsSended(FALSE)
		  ,m_dwVipStartTime(pData->RoleDataSave.dwVipStartTime),m_n16FreeWorldTalkTimes(pData->RoleDataSave.n16FreeWorldTalkTimes)
		  ,m_dwVipMaxDays(pData->RoleDataSave.dwVipMaxDays)
		  ,m_bSaveExpDirect(FALSE),m_nSaveExpCounter(0)
		  ,m_n64CurZhanDouFu(0)
		  ,m_nFabaoMoodUpdateTicks(0)
		  ,m_nHolyTicks(COST_HOLY_INTER_TICK)
		  ,m_nCoValueTicks(HOLY_COVALUE_INTER_TICK)
		  ,m_byBuy50LvlItemFlag(0)
		  ,m_byBuy60LvlItemFlag(0)
		  ,m_byBuy70LvlItemFlag(0)
		  ,m_byBuy80LvlItemFlag(0)
		  ,m_n16YellowListAcceptTimes(0)
		  ,m_dwExpPilularUseTimes(pData->RoleDataSave.dwExpPilularUseTimes)
		  ,m_dwVipNetBarInstBuffID(GT_INVALID)
		  ,m_dwPickupModeSetting(GT_INVALID)
		  ,m_bPlayerBack(FALSE)
		  ,m_byPlayerBackDay(0)
		  ,m_bUsingPkMinLevel(FALSE)
		  ,m_Wardrobe(GetID())
		  ,m_nSceneTalkCounter(0)
		  ,m_nCondenseCoolDown(GT_INVALID)
		  ,m_nTransformSkinNdx(0)
		  ,m_GodAreaMgr(this)
		  ,m_dwFamilyID(GT_INVALID)
		  //,m_dwKeyCodeRewarded(0)
		  ,m_bTodayFirstLogin(FALSE)
		  ,m_bSafeguardFlag(FALSE)
		  ,m_dwTrainDate(0)
		  ,m_nTrainNum(0)
		  ,m_nAwardPoint(0)
		  ,m_nAwardFlag(0)
		  ,m_bInitPkState(FALSE)
		  ,m_bGetCash(FALSE)
		  ,m_bIsInFairyFusion(false)
		  ,m_pWarRole(NULL)
		  ,m_HolySoul(this)
#ifdef ON_INTERNET_CAFE
		  ,m_dwInternetCafePingTime(0)
		  ,m_byInternetCafePingLostTime(0)
		  ,m_bTimeout(FALSE)
#endif
{		   
	if(g_world.IsUseIPGSystem())
	{
		m_CurMgr.SetCurBagYuanBao(-1);
		m_CurMgr.SetCurBaiBaoYuanBao(0);
	}

	m_timeTakeGuildReward = 0;
	m_timeRealLogout = 0;
	ZeroMemory(m_dwRoleLogData, sizeof(DWORD)*DataCollection_Max_RoleData_Num);
	Init(pData);
	m_iniLogoutTime = m_LogoutTime;
	m_timeCurrentDayTime = 0;
	if( IsValidOfLessingLoong() && m_timeLastLessing == 0 )
		m_timeLastLessing = GetCurrentOnlineTime();
	//if( !IsValidOfLessingLoong() && m_nLessingLevelEx == 0 )
	//	m_timeLastLessing = 0;//CalTodayOnlineTime();
	NotifyClientIniLessingOfLoong();

	g_FriendlyShutdownServer.NotifyNewJoinGamePly(this);


	tagDWORDTime now = GetCurrentDWORDTime();
	if (now.hour<8)
	{
		m_dwLastClearAdvenceStrength = now;
		m_dwLastClearAdvenceStrength.day -= 1;
	}
	else
	{
		m_dwLastClearAdvenceStrength = now;
	}
	memset( m_ShortCutBarData, 0x00, sizeof( m_ShortCutBarData ) );
	memset( m_QuickBarStateFlags, 0x00, sizeof( m_QuickBarStateFlags ) );
	LoadQuickBarData2DB();
	
	//��ʼ�����ӽ������ɹ���
	m_clsOrangeVNBMgr.Init(pData->RoleDataConst.szRoleName,pSession->GetAccount(),pSession->GetGetIP());

	m_nLessingExCounter = 0;
	ZeroMemory(&m_ownerSpeeds,sizeof(m_ownerSpeeds));

	CheckVipStateWhenLogin();

	RemoveBuff(Buff::GetIDFromTypeID(AUTO_FIGHT_BUFF_ID), TRUE);

	if( P_VALID(pSession) )
	{
		tagNC_QuerySoaringSwitch msg;
		msg.bQueryOrOpen = FALSE;
		pSession->SetRole(this);
		pSession->HandleQuerySoaringSwitch(&msg);

		tagNS_SyncStartVipTime send;
		send.dwStartVipTime = m_dwVipStartTime;
		send.dwVipMaxDays = m_dwVipMaxDays;
		SendMessage(&send, send.dwSize);

		BOOL bSyncCanRankFlag = FALSE;
		BYTE bySyncCanRankFlag = 1;// ��ɫ�Ƿ��ܽ�������,0���ܣ�1��
		if (pSession->GetPriviliege() == 0 && pData->RoleDataSave.byCanRankFlag != 1)// ����GM
		{
			bSyncCanRankFlag = TRUE;
			bySyncCanRankFlag = 1;
		}
		else if (pSession->GetPriviliege() > 0 && pData->RoleDataSave.byCanRankFlag != 0)
		{
			bSyncCanRankFlag = TRUE;
			bySyncCanRankFlag = 0;
		}
		if (bSyncCanRankFlag)
		{
			tagNDBC_SyncCanRankFlag syncCanRankFlag;
			syncCanRankFlag.dwRoleID = GetID();
			syncCanRankFlag.byCanRankFlag = bySyncCanRankFlag;
			g_dbSession.Send(&syncCanRankFlag, syncCanRankFlag.dwSize);
		}
		SyncMallOfferProto();

		NoticeClient();
	}

#ifdef ON_INTERNET_CAFE
	if (!IsEnd() && TRUE == g_InternetCafeSession.IsWell())
	{
		g_InternetCafeSession.SendPlayerLogin((CHAR*)GetSession()->GetAccount(), GetID(), GetSession()->GetGetIP());
	}
#endif
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Role::~Role()
{
#ifdef ON_INTERNET_CAFE
	if (!IsEnd() && TRUE == g_InternetCafeSession.IsWell())
	{
		g_InternetCafeSession.SendPlayerLogout((CHAR*)GetSession()->GetAccount(), GetID(), GetSession()->GetGetIP());
	}
#endif

	if(P_VALID(m_pScript))
	{
		m_pScript->OnRoleLogout(this);
	}

	tagQuestDoneSave* pDoneQuest = NULL;
	QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();
	while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
	{
		SAFE_DEL(pDoneQuest);
	}
	m_mapCompleteQuests.Clear();

    pDoneQuest = NULL;
	it = m_mapDailyCompleteQuests.Begin();
	while( m_mapDailyCompleteQuests.PeekNext(it, pDoneQuest) )
	{
		SAFE_DEL(pDoneQuest);
	}
	m_mapDailyCompleteQuests.Clear();
 

	SAFE_DEL(m_pStall);
	m_pScript = NULL;
	m_bGetCash = FALSE;
	m_bIsInFairyFusion = false;

	SAFE_DEL(m_pPetPocket);

	SAFE_DEL(m_pTitleMgr);

	SAFE_DEL(m_pBoardQuestMgr);
	SAFE_DEL(m_pYellowListMgr);

	DelTrainStateMap();

	ClearGroupChat();

	g_AthleticsSystem.GetRoomMgr().OnOffLine(GetID());
    
	// ��¼log
	PlayerSession* pSession = GetSession();
	tagNDBC_LogRoleOnlineInfo log;
	log.sLogRoleOnline.dwRoleID = this->GetID();
	log.sLogRoleOnline.dwOnlineLastTime = CalcTimeDiff(GetCurrentDWORDTime(),this->GetLoginTime());
	if(P_VALID(pSession)) pSession->GetMac(log.sLogRoleOnline.szMac);
	g_dbSession.Send(&log, log.dwSize);
};

//--------------------------------------------------------------------------------
VOID Role::Update()
{
	UpdateSpirit(); //�ȸ��¾���ֵ
	GetMoveData().Update();
	GetCombatHandler().Update();
	UpdateSkill();
	UpdateBuff();
	UpdatePK();
	UpdatePVP();
	UpdateHostilitySafeGuard();
	UpdateTimedIssue();
	GetItemMgr().Update();
	GetTitleMgr()->Update();		//ʱ���ƳƺŸ���
	m_pStall->Update();
	m_CurMgr.Update();
	GetPetPocket()->Update();
	UpdateTeamProfit();
	
	// ��������һ���ϰ˵������д˲���
	ClearAdvanceStrength(); 
	// ÿ���˻��������ճ���б�
	ClearDailyCompleteQuest();

	--m_nNeedSave2DBCountDown;
	UpdateTalkToWorld();
	UpDateTalkToScene();
	UpdateMotionInviteState();

	UpdateBlessingOfLoong();

	UpdateMiracleSkill();
     
	UpdateSendMapBuffLeftTimeToClient();
	m_clsOrangeVNBMgr.Update();

	//UpdateCurMonthOnlineTime();
	UpdateVipLevel();
	UpdateChaseHolderOnWater();
	UpdateFabaoMood();

	// ʥ��������ظ��£����ٻ�ʥ��ʱ��ɫʥ��ֵ����
	//UpdateHolyData();

 	GetItemMgr().UpdateFabaoMating();

	GetWardrobe().Update();
	UpdateCondense();
	UpdateArea();
	UpdateFamilyTrainNum();

	if( g_world.IsUseIPGSystem() )
	{
		//ILOG->Write(_T("g_world.IsUseIPGSystem. \r\n"));
		UpdateGetCash();
	}

#ifdef ON_INTERNET_CAFE
	UpdateInternetCafe();
#endif

	//m_HolySoul.Update();

}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
BOOL Role::AddToWorld(BOOL bFirst)
{
	BOOL bRet = g_mapCreator.TakeOverRoleWhenOnline(this);

	if( bRet && P_VALID(m_pScript) )
	{
		if( bFirst )		// ��һ�ν�����Ϸ����
		{
			m_pScript->OnRoleFirstIntoWorld(this);
		}
		m_pScript->OnRoleIntoWorld(this);	// ������Ϸ����
	}

	return bRet;
}

//-----------------------------------------------------------------------
// ����Ƿ��ڶ�Ӧְ��NPC����
//-----------------------------------------------------------------------
DWORD Role::CheckFuncNPC(DWORD dwNPCID, EFunctionNPCType eNPCType,
						 OUT Creature **ppNPC/* = NULL*/, OUT Map **ppMap/* = NULL*/)
{
	// ���map
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// �ҵ�NPC�������Ϸ���
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC))
	{
		return E_NPC_NotFound;
	}

	if(!pNPC->IsFunctionNPC(eNPCType))
	{
		return E_NPC_NotValid;
	}

	if(!pNPC->CheckNPCTalkDistance(this))
	{
		return E_NPC_TooFar;
	}

	if(ppNPC != NULL)
	{
		*ppNPC = pNPC;
	}

	if(ppMap != NULL)
	{
		*ppMap = pMap;
	}

	return E_Success;
}

//----------------------------------------------------------------------------------------
// ��ʼ����Ʒװ�� -- �����ͻ���
//----------------------------------------------------------------------------------------
VOID Role::SendInitStateItem()
{
	GetItemMgr().SendInitStateItem();
	GetItemMgr().SendInitStateItemCDTime();
	GetWardrobe().SyncWardrobeInfo(this);
	GetWardrobe().PutOnCurFashion(this, TRUE);
	GetWardrobe().PutOnBePlayActFashion(this, TRUE);
}

//----------------------------------------------------------------------------------------
// ��ʼ��װ
//----------------------------------------------------------------------------------------
VOID Role::SendInitStateSuit()
{
	INT nSuitNum = m_Suit.GetSuitNum();
	if(0 == nSuitNum)
	{
		return;
	}

	INT nSzMsg = sizeof(tagNS_GetRoleInitState_Suit) - 1 + sizeof(tagSuitInit) * nSuitNum;

	MCREATE_MSG(pSend, nSzMsg, NS_GetRoleInitState_Suit);
	if(!P_VALID(pSend))
	{
		ASSERT(P_VALID(pSend));
		return;
	}

	m_Suit.InitSendInitState(pSend->byData);
	pSend->nSuitNum = nSuitNum;
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//---------------------------------------------------------------------------
// ��ʼ��������
//---------------------------------------------------------------------------
VOID Role::SendInitStateLongHun()
{
	INT8 n8Num = m_LongHun.GetActiveNum() + m_LongHun.GetPassiveNum();
	if(n8Num <=0)
	{
		return;
	}

	INT nSzMsg = sizeof(tagNS_GetRoleInitState_Longhun) + (n8Num - 1) * sizeof(INT16);
	
	MCREATE_MSG(pSend, nSzMsg, NS_GetRoleInitState_Longhun);
	if(!P_VALID(pSend))
	{
		ASSERT(P_VALID(pSend));
		return;
	}

	m_LongHun.InitSendInitState(pSend->n16EquipPos, n8Num);
	pSend->n8Num = n8Num;
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}
//---------------------------------------------------------------------------
// �����˺Ż������ø��ͻ���
//---------------------------------------------------------------------------
VOID Role::SendInitPersonalSet()
{
	tagNS_SetPersonalSet send;

	PlayerSession* pPlayer =  GetSession();
	if(P_VALID(pPlayer))
	{
		send.PersonalSet = pPlayer->GetPersonalSet();
	}

	SendMessage(&send, send.dwSize);
}
//---------------------------------------------------------------------------
// ���������ʼ�������Ը��ͻ���
//---------------------------------------------------------------------------
VOID Role::SendInitStateAtt()
{
	INT nBuffNum = GetBuffNum();
	DWORD dwSize = sizeof(tagNS_GetRoleInitState_Att) + ((nBuffNum > 0) ? (nBuffNum - 1)*sizeof(tagBuffMsgInfo) : 0);

	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_Att);

	pSend->Avatar = m_Avatar;
	pSend->AvatarEquip = GetAvatarEquip();
	pSend->DisplaySet = GetDisplaySet();
	pSend->nLevel = m_nLevel;
	pSend->nCurLevelExp = m_nCurLevelExp;
	pSend->nCredit = m_nCredit;
	pSend->nIdentity = m_nIdentity;
	pSend->nVIPPoint = GetCurMgr().GetVIPPoint();
	pSend->dwState = GetState();
	pSend->dwRoleState = m_RoleState.GetState();
	pSend->dwRoleStateEx = m_RoleStateEx.GetState();
	pSend->ePKState = m_ePKState;
	pSend->dwRebornMapID = m_dwRebornMapID;
	pSend->vRebornPoint = g_mapCreator.GetReBornPoint(m_dwRebornMapID);
	pSend->dwTransportID = m_dwItemTransportMapID;
	pSend->vTransportPoint.x = m_fItemTransportX;
	pSend->vTransportPoint.z = m_fItemTransportZ;
	pSend->dwGuildID = GetGuildID();
	pSend->u16ActTitleID = GetTitleMgr()->GetActiviteTitle();
	pSend->dwLoverID = m_dwLoverID;
	pSend->bHaveWedding = m_bHaveWedding;
	pSend->bHasLeftMsg = m_bHasLeftMsg;
	pSend->eClassType = m_eClass;
	pSend->eClassTypeEx = m_eClassEx;
	pSend->byStallLevel = this->GetStallModeLevel();
	pSend->nConsumptiveStrength = m_Strength.nConsumptiveStrength;
	pSend->n16MiraclePrcNum = m_n16MiraclePracticeNum;
	pSend->dwClergyID = m_dwClergy;
	Family* pFamily = g_FamilyMgr.GetFamily(GetFamilyID());
	bool bFamilyLeader = false;
	if (P_VALID(pFamily))
		bFamilyLeader = pFamily->MemberIsLeader(GetID())?true:false;
	pSend->bFamilyLeader = P_VALID(pFamily)?bFamilyLeader:false;
	pSend->dwFamilyID = P_VALID(pFamily)?GetFamilyID():GT_INVALID;

	IFASTCODE->MemCpy(pSend->nAtt, m_nAtt, sizeof(m_nAtt));
	IFASTCODE->MemCpy(pSend->nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));

	pSend->nBuffNum = nBuffNum;

	pSend->nGod = m_nGod;
	pSend->nMonster = m_nMonster;
	pSend->nGodPoint = m_nGodPoint;
	pSend->nMonsterPoint = m_nMonsterPoint;

	if( nBuffNum > 0 )
	{
		GetAllBuffMsgInfo(pSend->Buff, nBuffNum);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
	if( GetAttValue(ERA_Soar) == ESV_SoaringUp )
	{
		tagNS_ResetSoarSkillLearnTimes send_msg;
		send_msg.nCurrLeftNum = GetPracticeSoaringSkillTimes();
		SendMessage(&send_msg,send_msg.dwSize);
	}
	DWORD dwState = m_RoleState.GetState();
	INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
	if( g_world.IsPkConstraintsOpen() && IsUsingPKConstraintsSafeGuradMinLevel() )
	{
		Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
	}
	if( (m_nLevel > Max_PK_SafeGuard_Level) && !( dwState & ERS_PK ) && !( dwState & ERS_PKEX ) && !( dwState & ERS_PeaceModel )  )
		SetPK(-1);
}

//----------------------------------------------------------------------------
// ���������ʼ���ܸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateSkill()
{
	INT nNum = m_mapSkill.Size();

	TMap<DWORD, Skill*>::TMapIterator it = m_mapSkill.Begin();
	Skill* pSkill = NULL;

	if( nNum <= 0 )
	{
		tagNS_GetRoleInitState_Skill send;
		IFASTCODE->MemCpy(send.Talent, m_Talent, sizeof(m_Talent));
		send.nNum = 0;

		SendMessage(&send, send.dwSize);
	}

	else
	{
		DWORD dwSize = sizeof(tagNS_GetRoleInitState_Skill) + sizeof(tagSkillMsgInfo) * (nNum-1);
		MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_Skill);

		pSend->dwSize = dwSize;
		IFASTCODE->MemCpy(pSend->Talent, m_Talent, sizeof(m_Talent));
		pSend->nNum = nNum;

		INT nIndex = 0;
		while( m_mapSkill.PeekNext(it, pSkill) )
		{
			pSkill->GenerateMsgInfo(&pSend->Skill[nIndex]);
			nIndex++;
		}

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}


}

//----------------------------------------------------------------------------
// ����������������б���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateCompleteQuest()
{
	// �õ��������ĸ���
	INT nCompleteQuestNum = GetCompleteQuestCount();
	
	DWORD dwSize = sizeof(tagNS_GetRoleInitState_CompleteQuest);
	if( nCompleteQuestNum > 0 )
	{
		dwSize += (nCompleteQuestNum-1) * sizeof(tagCompleteQuestMsgInfo);
	}
	
	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_CompleteQuest);

	pSend->nNum = nCompleteQuestNum;

	INT nIndex = 0;
	tagQuestDoneSave* pDoneQuest = NULL;
	QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();

	while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
	{
		pSend->completeQuest[nIndex].u16QuestID = pDoneQuest->u16QuestID;
		pSend->completeQuest[nIndex].dwStartTime = pDoneQuest->dwStartTime;
		pSend->completeQuest[nIndex].nTimes = pDoneQuest->nTimes;
		++nIndex;

		if( nIndex >= nCompleteQuestNum ) break;
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

}


//----------------------------------------------------------------------------
// �����������ÿ�������б���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateDailyCompleteQuest()
{
	bIsDailyQuestCleared = TRUE;
	// �õ��������ĸ���
	INT nCompleteQuestNum = m_mapDailyCompleteQuests.Size();

	DWORD dwSize = sizeof(tagNS_GetRoleInitState_DailyCompleteQuest);
	if( nCompleteQuestNum > 0 )
	{
		dwSize += (nCompleteQuestNum-1) * sizeof(tagCompleteQuestMsgInfo);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_DailyCompleteQuest);

	pSend->nNum = nCompleteQuestNum;

	INT nIndex = 0;
	tagQuestDoneSave* pDoneQuest = NULL;
	QuestDoneMap::TMapIterator it = m_mapDailyCompleteQuests.Begin();

	while( m_mapDailyCompleteQuests.PeekNext(it, pDoneQuest) )
	{
		pSend->completeQuest[nIndex].u16QuestID = pDoneQuest->u16QuestID;
		pSend->completeQuest[nIndex].dwStartTime = pDoneQuest->dwStartTime;
		pSend->completeQuest[nIndex].nTimes = pDoneQuest->nTimes;
		++nIndex;

		if( nIndex >= nCompleteQuestNum ) break;
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

}

//----------------------------------------------------------------------------
// �������ﵱǰ�����б���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateIncompleteQuest()
{
	//�õ���ǰ����ĸ���
	INT nCurrentQuestNum = GetCurrentQuestCount();

	DWORD dwSize = sizeof(tagNS_GetRoleInitState_IncompleteQuest);
	if( nCurrentQuestNum > 0 )
	{
		dwSize += (nCurrentQuestNum-1) * sizeof(tagIncompleteQuestMsgInfo);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetRoleInitState_IncompleteQuest);

	pSend->nNum = nCurrentQuestNum;

	INT		nIndex = 0;
	BYTE*	pTemp = (BYTE*)pSend->incompleteQuest;	

	for(INT n = 0; n < QUEST_MAX_COUNT; ++n)
	{
		if( FALSE == m_Quests[n].IsValid() ) continue;

		DWORD dwQuestSize = 0;

		m_Quests[n].GenerateMsgInfo((tagIncompleteQuestMsgInfo*)pTemp, dwSize, dwQuestSize, this);
		++nIndex;
		pTemp += dwQuestSize;
		

		if( nIndex >= nCurrentQuestNum ) break;
	}
	pSend->dwSize = dwSize;
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//----------------------------------------------------------------------------
//  ���������Ǯ���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateMoney()
{
	tagNS_GetRoleInitState_Money send;

	CurrencyMgr &CurMgr		= GetCurMgr();
	send.nBagYuanBao		= CurMgr.GetBagYuanBao();
	send.n64BagSilver		= CurMgr.GetBagSilver();
	send.n64WareSilver		= CurMgr.GetWareSilver();
	send.nBaiBaoYuanBao		= CurMgr.GetBaiBaoYuanBao();
	send.nExchangeVolume	= CurMgr.GetExchangeVolume();
    send.nVipPoint          = CurMgr.GetVIPPoint();
	for (INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		send.nCurClanCon[nClanType] = CurMgr.GetClanCon(static_cast<ECLanType>(nClanType));
	}

	SendMessage(&send, send.dwSize);
}

//----------------------------------------------------------------------------
//  �������ﵱǰ�������ݸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateReputation()
{
	tagNS_GetRoleInitState_Reputation send;

	ClanData& clanData = GetClanData();

	for (INT32 clantype = ECLT_BEGIN; clantype < ECLT_END; ++clantype)
	{
		send.nReputation[clantype]	= clanData.RepGetVal((ECLanType)clantype);
		send.nActiveCount[clantype]	= clanData.ActCountGetVal((ECLanType)clantype);
		send.bisFame[clantype]		= clanData.IsFame((ECLanType)clantype);
	}

	SendMessage(&send, send.dwSize);
}

//----------------------------------------------------------------------------
//  ����Ԫ����Ϣ���ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitHolySoulInfo()
{
	bool bIsSoulActivated = m_HolySoul.IsActivated();
	if ( bIsSoulActivated )
	{
		m_HolySoul.SendHolySoulAttToClient();
	}
}

//----------------------------------------------------------------------------
//  �������г��е�˰��
//----------------------------------------------------------------------------
VOID Role::SendInitStateCity()
{
	// ��Ϣ����
	INT nMsgSize = sizeof(tagNS_GetRoleInitState_City) + sizeof(tagCityTaxRate)*(g_cityMgr.GetCityNum() - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetRoleInitState_City);

	g_cityMgr.GetAllCityTaxRate(pSend->stData);
	pSend->byCityNum = g_cityMgr.GetCityNum();

	SendMessage(pSend, pSend->dwSize);

}
//----------------------------------------------------------------------------
//  �����������ڰ����������ݸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendInitStateGuild()
{
	tagNS_GetRoleInitState_Guild send;

	g_guildMgr.InitGuildInitState(this, send.sGuildBase, send.sGuildMember);

	SendMessage(&send, send.dwSize);
	// Jason ��ʱ��Ӱ������������֪ͨ���������Ϣ
	ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
	if( P_VALID(pToolkit) )
	{
		pToolkit->OnGuildMasterNotice(this);
	}

    //�Ѹ���ͬ�����ͻ���
    Guild* pGuild =  g_guildMgr.GetGuild(GetGuildID());
	if (P_VALID(pGuild))
	{
		pGuild->SendGuildName2Client(GetSession());
	}

}

//----------------------------------------------------------------------------
//  ���ͷ��������ݸ��ͻ���
//----------------------------------------------------------------------------
VOID Role::SendFatigueGuardInfo( BYTE byCode )
{
	tagNS_FatigueLimit send;
	send.byLimitCode = byCode;
	SendMessage(&send, send.dwSize);
}


//----------------------------------------------------------------------------
//  ����������Ѻͺ�����
//----------------------------------------------------------------------------
VOID Role::SendFriendAndEnemy()
{
	//�õ����Ѹ���
	INT nFriendNum = GetFriendCount();
	INT nIndex = 0;
	tagRoleInfo* pRoleInfo = (tagRoleInfo*)GT_INVALID;

	DWORD dwSize = sizeof(tagNS_SendFriendBlackList);
	if( nFriendNum > 0 )
	{
		dwSize += (nFriendNum-1) * sizeof(tagFriendInfo);
	}

	MCREATE_MSG(pSend, dwSize, NS_SendFriendBlackList);
	IFASTCODE->MemCpy(pSend->dwRolesID, m_dwBlackList, sizeof(DWORD) * MAX_BLACKLIST);

	// ���ӳ������
// 	IFASTCODE->MemCpy(pSend->dwEnemysID, m_dwEnemyList, sizeof(tagEnemyInfo) * MAX_ENEMYNUM);
	pSend->nNum = nFriendNum;
	INT nEnemyIdx = 0;
	for (INT nCnt = 0; nCnt < MAX_ENEMYNUM; nCnt++)
	{
		if (GT_INVALID == m_dwEnemyList[nCnt]) continue;

		pRoleInfo = g_roleMgr.GetRoleInfo(m_dwEnemyList[nCnt]);
		if (P_VALID(pRoleInfo))
		{
			pSend->dwEnemysID[nEnemyIdx].dwEnemyID = m_dwEnemyList[nCnt];
			pSend->dwEnemysID[nEnemyIdx].nLevel = pRoleInfo->byLevel;
		}

		++nEnemyIdx;
	}

	for(INT n = 0; n < MAX_FRIENDNUM; ++n)
	{
		if( GT_INVALID == m_Friend[n].dwFriendID ) continue;

		pSend->FriendInfo[nIndex].dwFriendID = m_Friend[n].dwFriendID;
		pSend->FriendInfo[nIndex].dwFriVal = m_Friend[n].dwFriVal;
		pSend->FriendInfo[nIndex].byGroup = m_Friend[n].byGroup;
		
		pRoleInfo = g_roleMgr.GetRoleInfo(m_Friend[n].dwFriendID);
		if(P_VALID(pRoleInfo))
		{
			pSend->FriendInfo[nIndex].bOnline = pRoleInfo->bOnline;
			pSend->FriendInfo[nIndex].nLevel = pRoleInfo->byLevel;
			Role* pRole = g_roleMgr.GetRolePtrByID(m_Friend[n].dwFriendID);
			if (P_VALID(pRole))
				pSend->FriendInfo[nIndex].nVIPLevel = pRole->GetAttValue(ERA_VipLevel);
			else
				pSend->FriendInfo[nIndex].nVIPLevel = pRoleInfo->nVIPLevel;
		}

		++nIndex;
	}
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);	
}

//----------------------------------------------------------------------------------------
// ��NPC����ȡ����
//----------------------------------------------------------------------------------------
INT Role::AcceptQuestFromNPC(UINT16 u16QuestID, DWORD dwNPCID)
{
	Creature* pNPC = NULL;

	// ������Ǵ�NPC��ȡ
	if( !P_VALID(dwNPCID) )
	{
		if( g_questMgr.IsNeedNPCAcceptQuest(u16QuestID) )
			return E_QUEST_NPC_NOT_EXIST;
	}
	// �Ǵ�NPC��ȡ
	else if( !IsBoardQuest(u16QuestID) && !IsYellowListQuest(u16QuestID))
	{
		pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) )
		{
			return E_QUEST_NPC_NOT_EXIST;
		}
		else if( FALSE == pNPC->IsInDistance(*this, 500.0f) )
		{
			return E_QUEST_NPC_TOO_FAR;
		}
		else if( FALSE == g_questMgr.IsNPCAcceptQuest(pNPC, u16QuestID) )
		{
			return E_QUEST_NOT_EXIST;
		}
	}

	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if(!P_VALID(pProto)) return E_QUEST_NOT_EXIST;

	INT nRet = E_Success;
	INT nIndex = GT_INVALID;
	if( !IsBoardQuest(u16QuestID) && !IsYellowListQuest(u16QuestID))
	{
		nRet = CanAcceptQuest(u16QuestID, nIndex);
	}
	else
	{
		EQuestBoardType eQuestBoardType = GetQuestBoardType(u16QuestID);
		if (eQuestBoardType != EQBT_Null)
			nRet = CanAcceptBoardQuest(u16QuestID, nIndex, eQuestBoardType);
	}
	
	if( E_Success == nRet )
	{
		// ��log ��ʱ���
		if(30825==u16QuestID)
			ILOG->Write(_T("\r\nRole(%d) accept quest(%d)...................\n"), this->GetID(), u16QuestID);

		// �����������������
		AddQuest(pProto, nIndex);
		
		if (!IsBoardQuest(u16QuestID))
		{
			m_Quests[nIndex].OnAccept(pNPC);
		}
		else
		{
			EQuestBoardType eType = GetQuestBoardType(u16QuestID);
			if (eType!=EQBT_Null)
			{
				UINT16 u16BoardID = GetBoardQuestMgr(eType)->GetBoardIDFromID(u16QuestID);

				const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
				if(P_VALID(pScript)) 
					pScript->OnAccept(u16BoardID, this, pNPC);
			}
		}
	}

	return nRet;
}

//----------------------------------------------------------------------------------------
// �Ӵ����������һ������
//----------------------------------------------------------------------------------------
INT Role::AcceptQuestFromMapTrigger(UINT16 u16QuestID, DWORD dwMapTriggerID)
{
	// ��������Ƿ����
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) )	return E_QUEST_NOT_EXIST;

	// �ҵ��Ƿ��ڴ���������
	if( FALSE == GetMap()->IsInTrigger(this, dwMapTriggerID) )
		return E_CanTakeQuest_FAILED_WRONG_TRIGGER;

	// ��ô������������к�
	DWORD dwQuestSerial = GetMap()->GetTriggerQuestSerial(dwMapTriggerID);
	if( !P_VALID(dwQuestSerial) )
		return E_CanTakeQuest_FAILED_WRONG_TRIGGER;

	// �õ������Ƿ���Ҫ���������
	INT n = 0;
	for(; n < QUEST_TRIGGERS_COUNT; ++n)
	{
		if( 0 == pProto->accept_req_trriger[n] ) continue;

		if( dwQuestSerial == pProto->accept_req_trriger[n] )
			break;
	}
	if( n >= QUEST_TRIGGERS_COUNT )
	{
		return E_CanTakeQuest_FAILED_WRONG_TRIGGER;
	}


	// �Ƿ���������ȡ����
	INT nIndex = GT_INVALID;
	INT nRet = CanAcceptQuest(u16QuestID, nIndex);

	if( E_Success == nRet )
	{
		// �����������������
		AddQuest(pProto, nIndex);
		m_Quests[n].OnAccept(NULL);
	}

	return nRet;
}

//----------------------------------------------------------------------------------------
// ĳ�������Ƿ���Խ�ȡ
//----------------------------------------------------------------------------------------
INT Role::CanAcceptQuest(UINT16 u16QuestID, INT& nIndex, Creature* pNPC)
{
// 	// ������
// 	if (GetEarnRate() < 10000)
// 	{
// 		return E_QUEST_FATIGUEGUARD;
// 	}

//	g_questMgr.IsNeedNPCAcceptQuest(u16QuestID);

	// ��������Ƿ����
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) )	return E_QUEST_NOT_EXIST;
	
	// ���ְҵ�Ƿ����
	if ( E_Success != VocationCheck( pProto->class_type) )
		return E_CanTakeQuest_Class;

	// �ж��Ա��Ƿ����
// 	if ( GetSex() != pProto->sex )
// 	{
// 		return E_CanTakeQuest_SexLimit;
// 	}

	//INT nTmpClass =  1 << ( m_eClass - 1 );
	////INT nTmpClassEx = 1 << ( m_eClassEx + 8 );
	//INT nTmpClassEx = 0;

	//if ( (INT)m_eClassEx != (INT)EHV_Base )
	//{
	//	nTmpClassEx = 1 << ( m_eClassEx + 8 );
	//}

	//DWORD dwTmpLimitStr = nTmpClass + nTmpClassEx;
	//if( !( pProto->class_type & dwTmpLimitStr ) )
	//	return E_CanTakeQuest_Class;

	// ����Ƿ��Ѿ��ӹ�������
	if( IsHaveQuest(u16QuestID) ) return E_CanTakeQuest_ALREADY_TAKE;

	// ����Ƿ�����������
	if( IsHaveDoneQuest(u16QuestID) )
	{
		// ��������ǲ����ظ��ģ��򷵻��Ѿ�����
		if( !pProto->repeatable ) return E_CanTakeQuest_ALREADY_DONE;

		// ����ǿ��ظ��ģ������ǲ����д������Ƶ�
		if( 0 != pProto->accept_times )
		{
			INT nTimes = GetQuestCompleteTimes(u16QuestID);
			if( nTimes >= pProto->accept_times )
				return E_CanTakeQuest_FAILED_WRONG_TIMES;
		}
	}

	// ���ǰ������
	BYTE byPreQuestOK = 0x01;
	if( pProto->prev_quest_id != 0 && FALSE == IsHaveDoneQuest(pProto->prev_quest_id) )
	{
		byPreQuestOK = 0x00;
	}
	BYTE byPreQuest2OK = 0x01;
	if( pProto->prev_quest2_id != 0 && FALSE == IsHaveDoneQuest(pProto->prev_quest2_id) )
	{
		byPreQuest2OK = 0x00;
	}
	if( pProto->prev_quest_relation == 0 && (byPreQuestOK&byPreQuest2OK) == 0 )//ǰ��������붼���
	{
		return E_CanTakeQuest_FAILED_PREV;
	}
	else if( pProto->prev_quest_relation == 1 && (byPreQuestOK|byPreQuest2OK) == 0 )//���һ��ǰ�����񼴿�
	{
		return E_CanTakeQuest_FAILED_PREV;
	}

	// ��������������ʱ��
	if(pProto->period)
	{
		DWORD dwStartTime = GetQuestCompleteAcceptTime(u16QuestID);
		DWORD dwStartWeekDay = WhichWeekday(dwStartTime);
		DWORD dwCurrentTime = GetCurrentDWORDTime();
		DWORD dwCurrentWeekDay = WhichWeekday(dwCurrentTime);

		// ÿ������
		if(GT_INVALID != dwStartTime && EQuestPeriodic_DAY == pProto->period_type)
		{
			tagDWORDTime StartTime = (tagDWORDTime)dwStartTime;
			tagDWORDTime CurrentTime = (tagDWORDTime)dwCurrentTime;

			//if(dwStartWeekDay == dwCurrentWeekDay)
			if ( StartTime.year == CurrentTime.year && StartTime.month == CurrentTime.month && StartTime.day == CurrentTime.day)
				return E_CanTakeQuest_Tomorrow;
		}

		// ÿ������
		if (GT_INVALID != dwStartTime && EQuestPeriodic_WEEK == pProto->period_type)
		{
			if(dwCurrentWeekDay != (EWeek)pProto->week)
				return E_CanTakeQuest_Week;
		}
	}

	// ���ȼ�����
	if( 0 != pProto->accept_req_min_level )
	{
		if( GetLevel() < pProto->accept_req_min_level )
			return E_CanTakeQuest_FAILED_LOW_LEVEL;
	}
	if( 0 != pProto->accept_req_max_level )
	{
		if( GetLevel() > pProto->accept_req_max_level )
			return E_CanTakeQuest_FAILED_LOW_LEVEL;
	}

	// ����Ա�����
	if( 0 != pProto->sex )
	{
		BYTE bySex = GetSex();
		if( 0 == bySex && 1 == pProto->sex )
		{
			return E_CanTakeQuest_FAILED_WRONG_SEX;
		}
		if( 1== bySex && 2 == pProto->sex )
		{
			return E_CanTakeQuest_FAILED_WRONG_SEX;
		}
	}

	//// ���ְҵ����
	//if( ECT_Null != pProto->class_type )
	//{
	//	if( GetClass() != pProto->class_type )
	//	{
	//		return E_CanTakeQuest_FAILED_WRONG_CLASS;
	//	}
	//}

	// todo������
	

	// ��⼼������
	for(INT n = 0; n < QUEST_SKILLS_COUNT; n++)
	{
		if( !P_VALID(pProto->accept_req_skill[n]) ) continue;

		Skill* pSkill = GetSkill(pProto->accept_req_skill[n]);
		if( !P_VALID(pSkill) || pSkill->GetLevel() < pProto->accept_req_skill_val[n] )
		{
			return E_CanTakeQuest_FAILED_MISSING_SKILLS;
		}
	}

	// �����������
	for(INT n = 0; n < QUEST_ATTS_COUNT; n++)
	{
		if( ERA_Null >= pProto->accept_req_att[n] ||
			ERA_End <= pProto->accept_req_att[n] )
			continue;
		
		if( GetAttValue(pProto->accept_req_att[n]) < pProto->accept_req_att_val[n] )
			return E_CanTakeQuest_FAILED_MISSING_Att;
	}

	// �����Ʒ����
	for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
	{
		if( !P_VALID(pProto->accept_req_item[n]) ) continue;

		INT nTotal = GetItemMgr().GetBagSameItemCount(pProto->accept_req_item[n]) + GetItemMgr().GetQuestBagSameItemCount(pProto->accept_req_item[n]);
		if( nTotal < pProto->accept_req_item_num[n] )
		{
			return E_CanTakeQuest_FAILED_MISSING_ITEMS;
		}
	}

	// todo������Ƿ��Ѿ���飬�Ƿ�������

	// ��ⱳ���ܷ���½�������������Ʒ
	if( (0 != pProto->src_item) && pProto->src_item_num > 0)
	{
		if( GetItemMgr().GetBagFreeSize() < pProto->src_item_num ) 
			return E_CanTakeQuest_FAILED_BAG_FULL;
	}

	// �ű��ж�
	const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16QuestID);
	if( P_VALID(pScript) && FALSE == pScript->CheckAccept(u16QuestID, this, pNPC) )
	{
		return GT_INVALID;
	}

	// ��������Ƿ�����
	for(nIndex = 0; nIndex < QUEST_MAX_COUNT; ++nIndex)
	{
		if( FALSE == m_Quests[nIndex].IsValid() )
		{
			break;
		}
	}
	if( nIndex >= QUEST_MAX_COUNT )
	{
		return E_CanTakeQuest_FAILED_QUEST_NUM_FULL;
	}

	// ���ͨ��
	return E_Success;
}

INT	Role::CanAcceptBoardQuest(UINT16 u16QuestID, INT& nIndex, EQuestBoardType eQuestBoardType)
{
	// ��ø�����������ID
	UINT16 u16BoardID = GetBoardQuestMgr(eQuestBoardType)->GetBoardIDFromID(u16QuestID);

	// ��������Ƿ����
	tagBoardQuestProto* pProto = g_attRes.GetBoardQuestProto(u16BoardID);
	if( !P_VALID(pProto) )	return E_QUEST_NOT_EXIST;

	// ����Ƿ��Ѿ��ӹ�������
	if( IsHaveQuest(u16QuestID) ) return E_CanTakeQuest_ALREADY_TAKE;

	// ��������Ƿ�����
	for(nIndex = 0; nIndex < QUEST_MAX_COUNT; ++nIndex)
	{
		if( FALSE == m_Quests[nIndex].IsValid() )
		{
			break;
		}
	}

	if( nIndex >= QUEST_MAX_COUNT )
	{
		return E_CanTakeQuest_FAILED_QUEST_NUM_FULL;
	}

	// �ű��ж�
	const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
	if( P_VALID(pScript) && FALSE == pScript->CheckAccept(u16BoardID, this, NULL) )
	{
		return E_CanTakeQuest_FAILED_SCRIPT;
	}

	// ���ͨ��
	return E_Success;
}

//----------------------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------------------
BOOL Role::AddQuest(const tagQuestProto* pProto, INT nIndex)
{
	if( !P_VALID(pProto) ) return FALSE;
	if( nIndex < 0 || nIndex >= QUEST_MAX_COUNT ) return FALSE;
	if( m_Quests[nIndex].IsValid() ) return FALSE;

	// �������
	m_mapCurrentQuests.Add(pProto->id, &m_Quests[nIndex]);
	m_Quests[nIndex].Init(pProto, this, nIndex);

	// ���͸��ͻ�������һ������
	tagNS_AddQuest send1;
	send1.u16QuestID = m_Quests[nIndex].GetID();
	m_Quests[nIndex].GetInitQuestItem(send1.n16ItemCount);
	m_Quests[nIndex].GetInitDynamicTarget(&send1.DynamicTarget, send1.dwSize);
	SendMessage(&send1, send1.dwSize);

	// ���͸����ݿⱣ��
	tagNDBC_AcceptQuest send;
	send.dwRoleID = GetID();
	send.acceptQuest.u16QuestID = m_Quests[nIndex].GetID();
	send.acceptQuest.dwAcceptTime = m_Quests[nIndex].GetAcceptTime();
	g_dbSession.Send(&send, send.dwSize);

	// ���������ʱ��Ҫɾ��ĳЩ������Ʒ����������ɾ��
	if( pProto->del_req_item )
	{
		for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
		{
			if( !P_VALID(pProto->accept_req_item[n]) ) break;

			GetItemMgr().RemoveFromRole(pProto->accept_req_item[n], (INT32)pProto->accept_req_item_num[n], (DWORD)ELCID_Quest_Accept);
		}
	}

	// ���������ʱ���һ��������Ʒ�������������
	if( P_VALID(pProto->src_item) )
	{
		GetItemMgr().Add2Role(EICM_AccQuest, pProto->id, pProto->src_item, pProto->src_item_num, EIQ_Null, ELCID_Quest_Accept);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------------------
INT Role::CompleteQuest(UINT16 u16QuestID, DWORD dwNPCID, INT nChoiceItemIndex, EQuestComplteType eComplteType, UINT16& u16NexitQuestID)
{
	const tagQuestProto* pProto = g_questMgr.GetQuestProto(u16QuestID);
	if( !P_VALID(pProto) ) return E_QUEST_NOT_EXIST;
	
	// ��������Ƿ����
	Quest* pQuest = m_mapCurrentQuests.Peek(u16QuestID);
	if( !P_VALID(pQuest) ) return E_QUEST_NOT_EXIST;
	
	//tagItem* pItem = NULL;
	INT nRet = -1;
	
	// ������߲���
	INT nCostSpecItemNum = 0;
	
	TList<tagItem*> lstItem;
	if ( EQCT_SpecItem == eComplteType)
	{
		// �õ�������������⴦��ľ�̬��
		const tagQuestUseSpeciItem* pUseSpecitem = g_attRes.GetQuestUseSpeciItem(u16QuestID);
		if ( P_VALID(pUseSpecitem))
		{
			nCostSpecItemNum = pUseSpecitem->n16ItemNum;
			//// ��ұ����в�������ID(���ĵ��߾�Ϊ1)
			//pItem = GetItemMgr().GetItemByTypeID(pUseSpecitem->dwItemID);
			//// ������������������
			//if ( pItem != NULL && pQuest->IsCanCompleteRewardEx(nChoiceItemIndex))
			//	nRet = E_Success;
			//else
			//	return E_CanTakeQuest_FAILED_MISSING_ITEMS;
			if( GetItemMgr().GetBag().GetSameItemCount(pUseSpecitem->dwItemID) >= pUseSpecitem->n16ItemNum )
			{
				GetItemMgr().GetBagSameItemList(lstItem,pUseSpecitem->dwItemID,pUseSpecitem->n16ItemNum);
				nRet = E_Success;
			}
		}
	}
	// ��ͨ����
	else if (EQCT_Normal == eComplteType)
		nRet = CanCompleteQuest(pQuest, dwNPCID, nChoiceItemIndex);


	// ������Խ������������������ɾ������
	//if( E_Success != nRet )
	//{
	//	const tagQuestUnimpededToken * pQuestUnimpededToken = g_attRes.GetOneUnimpededToken(pQuest->GetID());
	//	if( P_VALID(pQuestUnimpededToken) )
	//	{
	//		if( GetItemMgr().GetBag().GetSameItemCount(pQuestUnimpededToken->dwUnimpededTokenTypeID) >= pQuestUnimpededToken->nTokenNum )
	//		{
	//			TList<tagItem*> lstItem;
	//			GetItemMgr().GetBagSameItemList(lstItem,pQuestUnimpededToken->dwUnimpededTokenTypeID,pQuestUnimpededToken->nTokenNum);
	//			GetItemMgr().DelBagSameItem(lstItem,pQuestUnimpededToken->nTokenNum,ELCID_Quest_Complete);
	//			nRet = E_Success;
	//		}
	//	}
	//}

	// ÿ�ս�ȡ��֮�л����������������ܴ���10
	if (IsYellowListQuest(u16QuestID))
	{
		if (GetYellowListAcceptTimes() >= YELLOWLIST_ACCEPT_TIMES_LIMIT)
			nRet = E_CanAcceptQuest_TenTimesEachDay;
// 		else
// 			IncrYellowListAcceptTimes();			// ÿ�ս�ȡ��֮�л�����������1
	}

	if( E_Success == nRet )
	{
		if (IsYellowListQuest(u16QuestID))
			IncrYellowListAcceptTimes();

		pQuest->SetCompleteMark(TRUE);
	
		// ��������
		if (!IsBoardQuest(u16QuestID) && !IsYellowListQuest(u16QuestID))
		{
			RewardQuest(pQuest, nChoiceItemIndex);
		}
		else
		{
			EQuestBoardType eQuestBoardType = GetQuestBoardType(u16QuestID);
			// ���������Ľ���
			RewardBoardQuest(u16QuestID, eQuestBoardType);
		}

		// ���ýű������ʱ����
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if (!IsBoardQuest(u16QuestID))
		{
			pQuest->OnComplete(pNPC);
		}
		else
		{
			EQuestBoardType eType = GetQuestBoardType(u16QuestID);
			if (eType!=EQBT_Null)
			{
				UINT16 u16BoardID = GetBoardQuestMgr(eType)->GetBoardIDFromID(u16QuestID);

				const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
				if(P_VALID(pScript)) 
					pScript->OnBoardQuestComplete(u16BoardID, this, pNPC);
			}
		}
		
		// ��������
		//if (!IsBoardQuest(u16QuestID) && !IsYellowListQuest(u16QuestID))
		//{
		//	RewardQuest(pQuest, nChoiceItemIndex);
		//}
		//else
		//{
		//	EQuestBoardType eQuestBoardType = GetQuestBoardType(u16QuestID);
		//	// ���������Ľ���
		//	RewardBoardQuest(u16QuestID, eQuestBoardType);
		//}
		
		// tbc:���ͳƺ���Ϣ
		GetTitleMgr()->SigEvent(ETE_QUEST_COMPLETE, pQuest->GetID(), GT_INVALID);

		// �۳���Ǯ
		if(pProto->complete_del_money)
		{
			INT32 n32Money = pProto->complete_req_money;
			GetCurMgr().DecBagSilver(n32Money, ELCID_Quest_Complete);
		}
		
		if ( EQCT_SpecItem == eComplteType /*&& pItem != NULL*/)
		{
			// ɾ��ָ������
			//GetItemMgr().DelFromBag(pItem->n64Serial, ELCID_Quest_Complete, 1);
			GetItemMgr().DelBagSameItem(lstItem,nCostSpecItemNum,ELCID_Quest_Complete);
		}
		
		// ɾ������
		RemoveQuest(pQuest->GetID(), TRUE);

		//// �����������ɺ���ˢ�������
		//if(IsBoardQuest(u16QuestID))
		//{
		//	// ɾ������
		//	GetBoardQuestMgr()->Del(u16QuestID);

		//	GetBoardQuestMgr()->IncreaseCompleteRefreshTime();
		//	GetBoardQuestMgr()->RefreshBoard(this, EBRT_CompleteQuest);
		//}

		// ����Ƿ��к����Զ���ȡ����
		if(pProto->auto_add_quest)
			u16NexitQuestID = pProto->next_quest_id;
	}

	return nRet;
}

//----------------------------------------------------------------------------------------
// �Ƿ���Խ�����
//----------------------------------------------------------------------------------------
INT Role::CanCompleteQuest(Quest* pQuest, DWORD dwNPCID, INT nChoiceItemIndex)
{
// 	// ������
// 	if (GetEarnRate() < 10000)
// 	{
// 		return E_QUEST_FATIGUEGUARD;
// 	}

	if( !P_VALID(pQuest) ) return GT_INVALID;

	Creature* pNPC = NULL;

	// ���dwNPCID��ΪGT_INVALID�������NPC�Ƿ�Ϸ�
	if( P_VALID(dwNPCID) )
	{
		pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) )
		{
			return E_QUEST_NPC_NOT_EXIST;
		}
		else if( FALSE == pNPC->IsInDistance(*this, 500.0f) )
		{
			return E_QUEST_NPC_TOO_FAR;
		}
		else if( FALSE == g_questMgr.IsNPCCompleteQuest(pNPC, pQuest->GetID()) )
		{
			return E_QUEST_NOT_EXIST;
		}
	}
	// ���dwNPCIDΪGT_INVALID������������Ƿ�ͨ��NPC�Ϳ����ύ
	else
	{
		if( !IsBoardQuest(pQuest->GetID()) && !IsYellowListQuest(pQuest->GetID()) && g_questMgr.IsNeedNPCCompleteQuest(pQuest->GetID()) )
		{
			return E_QUEST_NPC_NOT_EXIST;
		}
	}

	return pQuest->IsCanComplete(nChoiceItemIndex, pNPC);
}


//----------------------------------------------------------------------------------------
// �������ɾ������
//----------------------------------------------------------------------------------------
INT Role::DeleteQuest(UINT16 u16QuestID)
{
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return E_QUEST_NOT_EXIST;

	// ���ýű�������ɾ���ӿ�
	pQuest->OnCancel();

	// �ҵ���������ɾ��������
	RemoveQuest(u16QuestID, FALSE);

	return E_Success;
}

//----------------------------------------------------------------------------------------
// ɾ������
//----------------------------------------------------------------------------------------
VOID Role::RemoveQuest(UINT16 u16QuestID, BOOL bComplete)
{
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return;

	const tagQuestProto* pProto = pQuest->GetProto();
	DWORD dwStartTime = pQuest->GetAcceptTime();
	tagQuestDynamicTarget*	pQuestDynamicTarget = pQuest->GetDynamicTarget();
	EQuestTargetType	eTargetType = EQTT_NULL;
	DWORD				dwTargetID[DYNAMIC_TARGET_COUNT];		// Ŀ��TypeID
	INT					nTargetNum[DYNAMIC_TARGET_COUNT];		// Ŀ������
	if(P_VALID(pQuestDynamicTarget))
	{
		eTargetType = pQuestDynamicTarget->eTargetType;
		memcpy(dwTargetID, pQuestDynamicTarget->dwTargetID, sizeof(DWORD) * DYNAMIC_TARGET_COUNT);
		memcpy(nTargetNum, pQuestDynamicTarget->nTargetNum, sizeof(DWORD) * DYNAMIC_TARGET_COUNT);
	}
	// ���Ƚ���������
	m_mapCurrentQuests.Erase(u16QuestID);
	pQuest->Destroy();

	// ������������
	if( bComplete )
	{
		// ���������Ҫɾ���ռ�����Ʒ����ɾ����Ʒ
		if( P_VALID(pProto) && pProto->delete_item )
		{
			for(INT i = 0; i < QUEST_ITEMS_COUNT; i++)
			{
				if( !P_VALID(pProto->complete_req_item[i]) )
					break;

				GetItemMgr().RemoveFromRole(pProto->complete_req_item[i], (INT32)pProto->complete_req_item_num[i], (DWORD)ELCID_Quest_Complete);
			}
		}

		// ɾ����������ռ�����Ʒ
		if( EQTT_Collect == eTargetType)
		{
			for(INT n = 0; n < DYNAMIC_TARGET_COUNT; n++)
			{
				if( !P_VALID(dwTargetID[n]) )
					break;

				GetItemMgr().RemoveFromRole(dwTargetID[n], (INT32)nTargetNum[n], (DWORD)ELCID_Quest_Complete);
			}
		}

		// ɾ������������Ӧ������������Ʒ
		GetItemMgr().RemoveFromRole(u16QuestID, (DWORD)ELCID_Quest_Complete);

		// ����������뵽��������б���
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) )
		{
			pDoneQuest = new tagQuestDoneSave;
			pDoneQuest->u16QuestID = u16QuestID;
			pDoneQuest->dwStartTime = dwStartTime;
			pDoneQuest->nTimes = 0;
			m_mapCompleteQuests.Add(u16QuestID, pDoneQuest);
		}
		pDoneQuest->nTimes += 1;
		pDoneQuest->dwStartTime = dwStartTime;

		// ����������뵽��������б���
		tagQuestDoneSave* pDoneDailyQuest = m_mapDailyCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneDailyQuest) )
		{
			pDoneDailyQuest = new tagQuestDoneSave;
			pDoneDailyQuest->u16QuestID = u16QuestID;
			pDoneDailyQuest->dwStartTime = dwStartTime;
			pDoneDailyQuest->nTimes = 0;
			m_mapDailyCompleteQuests.Add(u16QuestID, pDoneDailyQuest);
		}
		pDoneDailyQuest->nTimes += 1;
		pDoneDailyQuest->dwStartTime = dwStartTime;

		tagNS_GetSingleQuestDoneInfo msgSend;
		msgSend.u16QuestID = u16QuestID;
		msgSend.nTimes = pDoneDailyQuest->nTimes;
		SendMessage(&msgSend,msgSend.dwSize);


		// ���͸����ݿⱣ��
		tagNDBC_CompleteQuest send;
		send.dwRoleID = GetID();
		send.QuestDone.u16QuestID = pDoneQuest->u16QuestID;
		send.QuestDone.dwStartTime = pDoneQuest->dwStartTime;
		send.QuestDone.nTimes = pDoneQuest->nTimes;
		g_dbSession.Send(&send, send.dwSize);

        // �����ճ��������ݸ����ݿⱣ��
		tagNDBC_DailyCompleteQuest Msgsend;
		Msgsend.dwRoleID = GetID();
		Msgsend.QuestDone.u16QuestID = pDoneDailyQuest->u16QuestID;
		Msgsend.QuestDone.dwStartTime = pDoneDailyQuest->dwStartTime;
		Msgsend.QuestDone.nTimes = pDoneDailyQuest->nTimes;
		g_dbSession.Send(&Msgsend, Msgsend.dwSize);
	}
	// ������ɾ������
	else
	{
		// ɾ������������Ӧ������������Ʒ
		GetItemMgr().RemoveFromRole(u16QuestID, (DWORD)ELCID_Quest_Discard);

		// ���͸����ݿ�ɾ��
		tagNDBC_DiscardQuest send;
		send.dwRoleID = GetID();
		send.u16QuestID = u16QuestID;
		g_dbSession.Send(&send, send.dwSize);
	}

	// ɾ�������ȡʱ������Ʒ
	if( P_VALID(pProto->src_item) && pProto->src_item_num > 0 )
	{
		GetItemMgr().RemoveFromRole(pProto->src_item, pProto->src_item_num, (DWORD)ELCID_Quest_Complete);
	}
}


//----------------------------------------------------------------------------------------
// ��������NPC״̬
//----------------------------------------------------------------------------------------
VOID Role::UpdateQuestNPCTalk(UINT16 u16QuestID, DWORD dwNPCID)
{
	// �Ƿ����������
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return;

	// �õ������Ƿ����
	Creature* pCreature = GetMap()->FindCreature(dwNPCID);
	if( !P_VALID(pCreature) ) return;

	// ���Ը�������
	pQuest->OnEvent(EQE_Talk, pCreature->GetID(), pCreature->GetTypeID());
}

//----------------------------------------------------------------------------------------
// ��������Trigger״̬
//----------------------------------------------------------------------------------------
VOID Role::UpdateQuestTrigger(UINT16 u16QuestID, DWORD dwTriggerID)
{
	// �Ƿ����������
	Quest* pQuest = GetQuest(u16QuestID);
	if( !P_VALID(pQuest) ) return;

	// ���Ȳ鿴�����ǲ����ڸô�������
	if( FALSE == GetMap()->IsInTrigger(this, dwTriggerID) )
		return;

	// �õ��ô��������������к�
	DWORD dwTriggerSerial = GetMap()->GetTriggerQuestSerial(dwTriggerID);
	if( !P_VALID(dwTriggerSerial) ) return;

	// ���Ը�������
	pQuest->OnEvent(EQE_Trigger, dwTriggerSerial);
}

//----------------------------------------------------------------------------
// ���������¼�
//----------------------------------------------------------------------------
VOID Role::OnQuestEvent(EQuestEvent eQuestType, DWORD dwEventMisc1/* =0 */, DWORD dwEventMisc2/* =0 */,DWORD dwEventMisc3/*=0*/)
{
	for(INT n = 0; n < QUEST_MAX_COUNT; ++n)
	{
		if( FALSE == m_Quests[n].IsValid() ) continue;

		if(m_Quests[n].OnEvent(eQuestType, dwEventMisc1, dwEventMisc2, dwEventMisc3))
			break;
	}
}

//-------------------------------------------------------------------------------------
// ��������ɱ�������¼�
//-------------------------------------------------------------------------------------
VOID Role::OnFamilyKillMonsterQuestEvent(DWORD dwMonsterID, INT nMonsterLevel, DWORD dwMapCrc)
{
	if (0==m_dwFamilyID)
	{
		return;
	}

	Family* pFamily = g_FamilyMgr.GetFamily(m_dwFamilyID);
	if (!P_VALID(pFamily))
	{
		return;
	}

	pFamily->OnMemberKillMonster(GetID(), dwMonsterID, nMonsterLevel, dwMapCrc);
}

VOID Role::ClearDailyCompleteQuest()
{
	if ( !bIsDailyQuestCleared )
	{
		return ;
	}
	tagDWORDTime now = GetCurrentDWORDTime();
	
	tagDWORDTime* pre = g_roleMgr.GetRoleCreateTime(GetID());
	if( P_VALID(pre))
	{
		if( pre->day != now.day )
		{

			DWORD dwCount =  m_mapDailyCompleteQuests.Size();
			DWORD dwSize  = sizeof(tagNDBC_ClearDailyCompleteQuest) + (dwCount - 1)*sizeof(tagQuestDoneSave);
			MCREATE_MSG(pSend,dwSize, NDBC_ClearDailyCompleteQuest);
			DWORD dwI = 0;
			pSend->dwCount = dwCount;
			pSend->dwRoleID = GetID();
			tagQuestDoneSave* pDoneQuest = NULL;
			QuestDoneMap::TMapIterator it = m_mapDailyCompleteQuests.Begin();
			while( m_mapDailyCompleteQuests.PeekNext(it, pDoneQuest) )
			{
				if (dwI >= dwCount)
				{
					break;
				}
				pSend->pData[dwI].dwStartTime = pDoneQuest->dwStartTime;
				pSend->pData[dwI].nTimes      = 0 ; 
				pSend->pData[dwI].u16QuestID  = pDoneQuest->u16QuestID;
				SAFE_DEL(pDoneQuest);
				dwI++;

			}
			g_dbSession.Send(pSend,pSend->dwSize);
			MDEL_MSG(pSend);
			m_mapDailyCompleteQuests.Clear();
			SendInitStateDailyCompleteQuest();

			(*pre) = now;

		}
	}




}

//-------------------------------------------------------------------------------------
// �������ɿضԻ���ȱʡ�¼�
//-------------------------------------------------------------------------------------
VOID Role::OnDlgDefaultEvent(EMsgUnitType eDlgTarget, DWORD dwTargetID, EDlgOption eDlgOption)
{
	switch (eDlgTarget)
	{
	case EMUT_DlgTarget_Quest:
		{
			Quest* pQuest = GetQuest((UINT16)dwTargetID); 
			if(!P_VALID(pQuest))	return;

			pQuest->OnEvent(EQE_DlgDefault, eDlgOption);
		}
		break;
	case EMUT_DlgTarget_Item:
		{

		}
		break;
	case EMUT_DlgTarget_Creature:
		{

		}
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------------------
// ������
//----------------------------------------------------------------------------------------
VOID Role::RewardQuest(Quest* pQuest, INT32 nChoiceItemIndex)
{
	// ������
	if (GetEarnRate() < 10000)
	{
		return ;
	}

	// �ȿ��Ƿ�Ϊ��̬����
	if(pQuest->DynamicRewardQuest(this, nChoiceItemIndex))
		return;
	

	const tagQuestProto* pProto = pQuest->GetProto();
	if( !P_VALID(pProto) ) return;

	// ������Ʒ
	for(INT n = 0; n < QUEST_REW_ITEM; ++n)
	{
		if( !P_VALID(pProto->rew_item[n]) ) break;

		if( pProto->rew_item_num[n] <= 0 ) continue;
        
		if( MIsEquipment(pProto->rew_item[n]))
		{
			RewardEquip(EICM_Quest, pQuest->GetID(), pProto->rew_item[n], (INT16)pProto->rew_item_num[n], (EItemQuality)pProto->rew_item_quality[n], ELCID_Quest_Complete);
		}
		else
		{
			GetItemMgr().Add2Role(EICM_Quest, pQuest->GetID(), pProto->rew_item[n], (INT16)pProto->rew_item_num[n], (EItemQuality)pProto->rew_item_quality[n], ELCID_Quest_Complete);
		}
		
	}

	// ��ѡ������Ʒ
	if( nChoiceItemIndex >= 0 && nChoiceItemIndex < QUEST_REW_ITEM )
	{
		if( P_VALID(pProto->rew_choice_item[nChoiceItemIndex]) &&
			pProto->rew_choice_item_num[nChoiceItemIndex] > 0 )
		{
			if( MIsEquipment(pProto->rew_choice_item[nChoiceItemIndex]))
			{
				RewardEquip(EICM_Quest, pQuest->GetID(), pProto->rew_choice_item[nChoiceItemIndex], 
					(INT16)pProto->rew_choice_item_num[nChoiceItemIndex], 
					(EItemQuality)pProto->rew_choice_quality[nChoiceItemIndex], ELCID_Quest_Complete);
			}
			else
			{


				GetItemMgr().Add2Role(EICM_Quest, pQuest->GetID(), pProto->rew_choice_item[nChoiceItemIndex], 
					(INT16)pProto->rew_choice_item_num[nChoiceItemIndex], 
					(EItemQuality)pProto->rew_choice_quality[nChoiceItemIndex], ELCID_Quest_Complete);
			}
		}
	}

	// ְҵ����
	EClassType eClass = this->GetClass();
	INT nTmp = 1 << (eClass-1); 
	for(INT i=0; i<QUEST_CLASS_REW_COUNT; ++i)
	{
		if (nTmp & pProto->rew_class_item[i].nClass)
		{
			for(INT j=0; j<CLASS_REWARD_ITEM_NUM; ++j)
			{
				if (!P_VALID(pProto->rew_class_item[i].Item[j].dwItemID))
				{
					continue;
				}

				if( MIsEquipment(pProto->rew_class_item[i].Item[j].dwItemID))
				{
					RewardEquip(EICM_Quest, pQuest->GetID(), pProto->rew_class_item[i].Item[j].dwItemID, (INT16)pProto->rew_class_item[i].Item[j].byNum, (EItemQuality)pProto->rew_class_item[i].Item[j].byQuality, ELCID_Quest_Complete);
				}
				else
				{
					GetItemMgr().Add2Role(EICM_Quest, pQuest->GetID(), pProto->rew_class_item[i].Item[j].dwItemID, (INT16)pProto->rew_class_item[i].Item[j].byNum, (EItemQuality)pProto->rew_class_item[i].Item[j].byQuality, ELCID_Quest_Complete);
				}
			}

			break;
		}
	}

	// ����
	if( pProto->rew_xp != 0 )
	{
		ExpChange(INT64(pProto->rew_xp));
	}

	// ��Ǯ
	if( pProto->rew_money > 0 )
	{
		GetCurMgr().IncBagSilver(pProto->rew_money, (DWORD)ELCID_Quest_Complete);
		if(P_VALID(m_pScript))
			m_pScript->IncSilverByQuest( this, pQuest->GetID(), pProto->rew_money );
	}

	// ����
	for (INT i = 0; JDG_VALID(ERT, pProto->rew_rep[i]) && i < QUEST_REPUTATIONS_COUNT; ++i)
	{
		GetClanData().RepModVal(MTRANS_ERT2ECLT(pProto->rew_rep[i]), pProto->rew_rep_val[i]);
	}

	// ����
	for (INT i = 0; JDG_VALID(ERCT, pProto->rew_contribution[i]) && i < QUEST_CONTRIBUTION_COUNT; ++i)
	{
		GetCurMgr().IncClanCon(pProto->rew_contribution_val[i], GT_INVALID, MTRANS_ECCT2ECLT(pProto->rew_contribution[i]));
	}

	// ����
	for(INT n = 0; n < QUEST_ATTS_COUNT; ++n)
	{
		if( ERA_Null >= pProto->rew_att[n] ||
			ERA_End <= pProto->rew_att[n] )
			break;

		//��ǰ��������
		if ( pProto->rew_att[n] != ERA_TalentPoint )
		{
                ModAttValue(pProto->rew_att[n], pProto->rew_att_val[n]);
		}
		else
		{
			    tagNS_RoleGetTalentPoint sendMsg ;
			    //�������������ʽ�������
			    INT nLevel = GetLevel();
				INT nCurrentTalent = GetAttValue(ERA_TalentPoint); //��ȡ��ǰ�Ŀ�Ͷ����
				INT nCurrentTotalTalent = nCurrentTalent + GetTotalTalentPoint(); //��ȡ��ǰ������(������Ͷ����)
				INT nCurrentTalentAvail_Limit = GetTalentAvail_limit(nLevel);  //��ȡ��ǰ����ɻ���������
				INT nRew_att_val = 0;
				if ( nCurrentTotalTalent >= nCurrentTalentAvail_Limit) // ��ǰ��������������ǰ�������ɻ������ �Ͳ�������
				{
                    nRew_att_val = 0;
					sendMsg.dwErrorCode = ERTEC_OverMaxTalentAvail_Limit ;
                       

				}
				else
				{
					nRew_att_val = pProto->rew_att_val[n];
					INT nTotalTalent = nRew_att_val + nCurrentTotalTalent ;  // ��ǰ�������ʼ������ͻ������ 
                    if ( nTotalTalent > nCurrentTalentAvail_Limit)
					{
						nRew_att_val = nCurrentTalentAvail_Limit - nCurrentTotalTalent;
						sendMsg.dwErrorCode = ERTEC_OverMaxTalentAvail_Limit ;
					}
					else
					{
                        sendMsg.dwErrorCode = ERTEC_Success ;
					}

				}
				SendMessage(&sendMsg,sendMsg.dwSize);
				ModAttValue(ERA_TalentPoint, nRew_att_val);
		}

		
	}

	// todo��Buff
}

VOID Role::RewardBoardQuest(UINT16 u16QuestID, EQuestBoardType eQuestBoardType)
{
	// ��������ID��ö�Ӧ�������ID
	UINT16 uBoardID = GetBoardQuestMgr(eQuestBoardType)->GetBoardIDFromID(u16QuestID);

	tagBoardQuestProto* pProto = g_attRes.GetBoardQuestProto(uBoardID);
	if (!P_VALID(pProto))
	{
		return;
	}

	// ������Ʒ
	for(INT i=0; i<BOARD_QUEST_REWARD_NUM; ++i)
	{
		tagBoardQuestReward reward = pProto->RewardItem[i];
		
		if( reward.dwID == 0) continue;

		GetItemMgr().Add2Role(EICM_Quest, u16QuestID, reward.dwID, reward.byNum, (EItemQuality)reward.byQuality, ELCID_Quest_Complete);
	}

	// ����
	if( pProto->nRewardExp != 0 )
	{
		ExpChange((INT64)pProto->nRewardExp);
	}

	// ��Ǯ
	if( pProto->nRewardMoney > 0 )
	{
		GetCurMgr().IncBagSilver(pProto->nRewardMoney, (DWORD)ELCID_Quest_Complete);
		if(P_VALID(m_pScript))
			m_pScript->IncSilverByQuest( this, u16QuestID, pProto->nRewardMoney );
	}

	if(P_VALID(m_pScript))
	{
		m_pScript->OnGetBoardQuestReward(this, u16QuestID, uBoardID, pProto->nRewardMoney, pProto->nRewardExp);
	}
}

//----------------------------------------------------------------------------------------
// ��ͼ������
//----------------------------------------------------------------------------------------
BOOL Role::MapTrigger(DWORD dwTriggerID, DWORD dwMisc)
{
	Map* pMap = GetMap();

	if( !P_VALID(pMap) ) return FALSE;

	const tagMapInfo* pInfo = pMap->GetMapInfo();

	// ���Ƿ���ڸ�Trigger
	tagMapTriggerInfo* pTrigger = pInfo->mapTrigger.Peek(dwTriggerID);
	if( !P_VALID(pTrigger) ) return FALSE;

	// �����ҵ�ǰλ���ǲ�����trigger��Χ֮��
	if( !pMap->IsInTrigger(this, dwTriggerID) ) return FALSE;

	// �鿴pTrigger������

	switch(pTrigger->eType)
	{
		// �л���ͼ
	case EMT_GotoNewMap:
		{
			// �õ�Ŀ���ͼ�ĵ�����
			const tagMapInfo* pMapInfo = g_mapCreator.GetMapInfo(pTrigger->dwMapID);
			if( !P_VALID(pMapInfo) ) return FALSE;

			const tagMapWayPointInfoList* pList = pMapInfo->mapWayPointList.Peek(pTrigger->dwWayPointID);
			if( !P_VALID(pList) ) return FALSE;

			// ��Ŀ�굼�����б�����ȡһ��������
			tagWayPointInfo wayPoint;
			pList->list.RandPeek(wayPoint);

			// todo: �������һ���㣬Ŀǰѡȡ���ĵ�

			// �ƶ����µ�ͼ
			if( FALSE == GotoNewMap(pTrigger->dwMapID, wayPoint.vPos.x, wayPoint.vPos.y, wayPoint.vPos.z, dwMisc))
				return FALSE;
		}
		break;

		// ���񴥷���
	case EMT_Script:
		{
			// �õ���trigger��Ӧ���������к�
			if( !P_VALID(pTrigger->dwQuestSerial) ) return FALSE;
			OnQuestEvent(EQE_Trigger, pTrigger->dwQuestSerial);
		}
		break;

		// ʲô��û�У�����ýű�
	case EMT_Null:
		{
			pMap->OnEnterTrigger(this, pTrigger);
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------
// ֪ͨ���ѽ��븱��
//--------------------------------------------------------------------------------------
DWORD Role::InstanceNotify(BOOL bNotify)
{
	if(bNotify == FALSE)	return GT_INVALID;

	// �ҵ����С��
	const Team* pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
	if( !P_VALID(pTeam) ) return GT_INVALID;

	// �������Ƿ񴴽��˸���
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return GT_INVALID;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return GT_INVALID;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return GT_INVALID;	

	// �����Ƿ�����֪ͨ����
	if( !pInstanceProto->bNoticeTeamate ) return GT_INVALID;

	// ֪ͨ����
	tagNS_InstanceAgree	 send;
	send.dwInviterID	=	m_dwID;
	send.dwMapID		=	pInstance->GetMapID();
	g_groupMgr.SendTeamInstanceNotify(this, GetTeamID(), &send, send.dwSize);

	return E_Success;
}

//--------------------------------------------------------------------------------------
// ����Ƿ�ͬ��������ҽ��븱��������
//--------------------------------------------------------------------------------------
DWORD Role::InstanceAgree(BOOL bAgree)
{
	if(bAgree == FALSE)		return GT_INVALID;

	if (IsInRoleStateAny(ERS_Commerce | ERS_PrisonArea))	return GT_INVALID;
			
	// �ҵ����С��
	const Team* pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
	if( !P_VALID(pTeam) ) return GT_INVALID;

	// �������Ƿ񴴽��˸���
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return GT_INVALID;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return GT_INVALID;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return GT_INVALID;

	GotoNewMap(pInstance->GetMapID(), pInstanceProto->vEnterPos.x, pInstanceProto->vEnterPos.y, pInstanceProto->vEnterPos.z, GT_INVALID, FALSE);

	return E_Success;
}

//---------------------------------------------------------------------------------
// ��������뿪����
//---------------------------------------------------------------------------------
DWORD Role::LeaveInstance()
{
	const tagMapInfo* pInfo = GetMap()->GetMapInfo();
	if( !P_VALID(pInfo) ) return GT_INVALID;

	if( EMT_Normal == pInfo->eType )
		return GT_INVALID;

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(pInfo->dwID);
	if( !P_VALID(pMapMgr) ) return GT_INVALID;

	pMapMgr->RoleInstanceOut(this);

	return E_Success;
}

//--------------------------------------------------------------------------------------
// �õ������ĸ���ID������ж����ȡ����ģ����û�ж����ȡ�Լ��ģ�
//--------------------------------------------------------------------------------------
DWORD Role::GetOwnInstanceID() const
{
	DWORD dwTeamID = GetTeamID();
	
	// �����С��
	if( dwTeamID != GT_INVALID )
	{
		const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if( P_VALID(pTeam) )
		{
			return pTeam->GetOwnInstanceID();
		}
		else
		{
			return GT_INVALID;
		}
	}
	else // ���û��С��
	{
		return m_dwOwnInstanceID;
	}
}

//--------------------------------------------------------------------------------------
// �õ������ĸ�����ͼID������ж����ȡ����ģ�û�ж����ȡ�Լ��ģ�
//--------------------------------------------------------------------------------------
DWORD Role::GetOwnInstanceMapID() const
{
	DWORD dwTeamID = GetTeamID();

	// �����С��
	if( dwTeamID != GT_INVALID )
	{
		const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if( P_VALID(pTeam) )
		{
			return pTeam->GetOwnInstanceMapID();
		}
		else
		{
			return GT_INVALID;
		}
	}
	else // ���û��С��
	{
		return m_dwOwnInstanceMapID;
	}
}

//--------------------------------------------------------------------------------------
// ȥһ���µ�ͼ
//--------------------------------------------------------------------------------------
BOOL Role::GotoNewMap(DWORD dwDestMapID, FLOAT fX, FLOAT fY, FLOAT fZ, DWORD dwMisc, BOOL bSameInstace, DWORD dwDestMapInstID)
{
	if( fX < 0.0f || fZ < 0.0f || fY < 0.0f )
	{
		return FALSE;
	}

	// ���һ�µ�ͼ�Ĵ�С
	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwDestMapID);
	if( !P_VALID(pInfo) ) return FALSE;

	// ���һ������
	if( fX < 0.0f || fX >= FLOAT(pInfo->nWidth * TILE_SCALE) ||
		fZ < 0.0f || fZ >= FLOAT(pInfo->nHeight * TILE_SCALE) )
		return FALSE;

	// ���������
	PetSoul* pMountSoul = GetPetPocket()->GetMountPetSoul();
	if (P_VALID(pMountSoul))
	{
		Role* pPassenger = pMountSoul->GetPassenger();
		if (P_VALID(pPassenger))
		{
			pPassenger->StopMount();
		}
	}

	// ���ȼ���ǲ��Ǳ���ͼ
	if( GetMapID() == dwDestMapID && bSameInstace )
	{

		// ͬ��ͼ�ڴ���
		Vector3 vNewPos(fX, fY, fZ);
		m_MoveData.ForceTeleport(vNewPos);

		// �õ��µľŹ���
		GetMap()->SendGotoNewMapToPlayer(this);

		return TRUE;
	}
	else
	{
		// ���µ�ͼ�� �����ҵ�MapMgr
		MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwDestMapID);
		if( !P_VALID(pMapMgr) ) return FALSE;

		// û���ҵ�MapMgr����ͼID�Ƿ�
		Map* pMap = GetMap();
		if( !P_VALID(pMap) ) return FALSE;

		// �����л���ͼ
		Vector3 vDestPos;
		vDestPos.x = fX;
		vDestPos.y = fY;
		vDestPos.z = fZ;

		if(dwDestMapInstID == 0)
			g_mapCreator.RoleChangeMap(m_dwID, dwDestMapID, vDestPos, dwMisc);
		else
			g_mapCreator.RoleChangeMapInst(m_dwID, dwDestMapID, dwDestMapInstID, vDestPos, dwMisc);

		return TRUE;
	}
}

//----------------------------------------------------------------------------------------------------
// �س�
//----------------------------------------------------------------------------------------------------
VOID Role::ReturnCity()
{
	if( !P_VALID(m_dwRebornMapID) ) return;

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(m_dwRebornMapID);
	if( !P_VALID(pInfo) ) return;

	GotoNewMap(m_dwRebornMapID, pInfo->vRebornPos.x, pInfo->vRebornPos.y, pInfo->vRebornPos.z);
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID Role::ReName(LPCTSTR szNewName)
{
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(this->GetID());
	if (!P_VALID(pRoleInfo))
	{
		return;
	}

	ZeroMemory( pRoleInfo->szRoleName, X_SHORT_NAME );
	_tcscpy_s( pRoleInfo->szRoleName, X_SHORT_NAME, szNewName);

	g_roleMgr.ChangeRoleName(GetID(), szNewName);

	SaveNewName2DB();
}

VOID Role::SaveNewName2DB()
{
	tagNDBC_SaveRenameInfo send;

	send.dwAccountID = (g_roleMgr.GetRoleInfo(GetID()))->dwAccountID;
	send.dwRoleID = (g_roleMgr.GetRoleInfo(GetID()))->dwRoleID;
	_tcscpy_s(send.szName, g_roleMgr.GetRoleInfo(this->GetID())->szRoleName);

	g_dbSession.Send(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------
// �����ɽ����е���
//-------------------------------------------------------------------------------------
DWORD Role::UseAddMiraclePrcNumItem(INT64 n64ItemID)
{
	DWORD dwErrorCode = E_Miracle_Success;

	// �����Ʒ�Ƿ��ڱ�����
	tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID); 
	if( !P_VALID(pItem) ) 
	{
		dwErrorCode = E_Miracle_ItemNotExist;
	}
	else
	{
		//�����Ʒ����ʹ������
		if(pItem->pProtoType->eSpecFunc != EISF_AddMiraclePrcNumItem)
		{
			dwErrorCode = E_Miracle_SpecFunError;
		}
		else
		{
			// ������Ʒ��ʧ
			INT16 addvalue = pItem->pProtoType->nSpecFuncVal1;
			dwErrorCode = GetItemMgr().ItemUsedFromBag(n64ItemID, 1, (DWORD)ELCLD_PracticeM_Skill);
			if(dwErrorCode == E_Success)
			{
				//
				m_n16MiraclePracticeNum += addvalue;
			}
		}
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
// ��ϰ�ɽ缼�� �߼���������
//-------------------------------------------------------------------------------------
DWORD Role::PracticeMiracleSkill(DWORD dwSkillID)
{
	// ���ü����Ƿ��Ѿ�����
	Skill* pSkill = GetSkill(dwSkillID);
	if( P_VALID(pSkill) )
	{	
		//����Ƿ�������������
		DWORD dwErrorCode = CheckPracticeMiracleSkillLimit(Skill::CreateTypeID(pSkill->GetID(), pSkill->GetLevel() + 1));
		if(dwErrorCode != E_Miracle_Success)
			return dwErrorCode;

		//����
		dwErrorCode = DoPracticeMiracleSkillCost(Skill::CreateTypeID(pSkill->GetID(), pSkill->GetLevel() + 1));
		if(dwErrorCode != E_Miracle_Success)
			return dwErrorCode;

		//����������
		ChangeSkillExp(pSkill,1);
	}
	//�����ǰ�������������
	else
	{
		//����Ƿ�������������
		DWORD dwErrorCode = CheckPracticeMiracleSkillLimit(Skill::CreateTypeID(dwSkillID, 1));
		if(dwErrorCode != E_Miracle_Success)
			return dwErrorCode;

		//����
		dwErrorCode = DoPracticeMiracleSkillCost(Skill::CreateTypeID(dwSkillID, 1));
		if(dwErrorCode != E_Miracle_Success)
			return dwErrorCode;

		//ѧϰ����
		dwErrorCode = LearnSkill(dwSkillID);
		if(dwErrorCode != E_Success)
			return dwErrorCode;

	}

	return E_Miracle_Success;
}

//-------------------------------------------------------------------------------------
// �ɽ缼������
//-------------------------------------------------------------------------------------
DWORD Role::DoPracticeMiracleSkillCost(DWORD dwSkillTypeID)
{

	//�Ҳ���ԭ��
	const tagSkillProto* pSkillProto  =  g_attRes.GetSkillProto(dwSkillTypeID);
	if(!P_VALID(pSkillProto))
		return GT_INVALID;


	//�õ��ɽ缼��ԭ��
	const tagMiracleSkill *pMSkill = g_attRes.GetMSkillProto(dwSkillTypeID);
	if(!P_VALID(pMSkill))
		return GT_INVALID;

	//���������
	--m_n16MiraclePracticeNum;
	if(m_n16MiraclePracticeNum < 0)
	{
		m_n16MiraclePracticeNum = 0;
		return E_Miracle_LackPracticeNum;
	}

	//��Ǯ
	BOOL bRet = GetCurMgr().DecBagSilver(pMSkill->dwSilverCost, ELCLD_PracticeM_Skill);
	if(bRet == FALSE)
	{
		return E_Miracle_LackMoney;
	}

	//��ʵ��ֵ
	m_Strength.nConsumptiveStrength -= pMSkill->n16PowerCost;
	if(m_Strength.nConsumptiveStrength < 0)
	{
		m_Strength.nConsumptiveStrength = 0;
		return E_Miracle_LackPower;
	}

	//������Ʒ
	//��Ʒ
	GetItemMgr().RemoveFromRole(pMSkill->dwItemTypeID, pMSkill->n16ItemNumCost, ELCLD_PracticeM_Skill);
	

	return E_Miracle_Success;
}

//-------------------------------------------------------------------------------------
// ����ɽ缼����������
//-------------------------------------------------------------------------------------
DWORD Role::CheckPracticeMiracleSkillLimit(DWORD dwSkillTypeID)
{
	Skill* pSkill = GetSkill(Skill::GetIDFromTypeID(dwSkillTypeID));
	if( P_VALID(pSkill) )
	{
		if( pSkill->GetLevel() >= pSkill->GetMaxLearnLevel() )
			return E_LevelUpSkill_ExceedMaxLearnLevel;
	}
	//�Ҳ���ԭ��
	const tagSkillProto* pSkillProto  =  g_attRes.GetSkillProto(dwSkillTypeID);
	if(!P_VALID(pSkillProto))
		return GT_INVALID;


	//�õ��ɽ缼��ԭ��
	const tagMiracleSkill *pMSkill = g_attRes.GetMSkillProto(dwSkillTypeID);
	if(!P_VALID(pMSkill))
		return GT_INVALID;

	//����Ƿ񳬹���ѧ�ȼ�����
	if(  Skill::GetLevelFromTypeID(dwSkillTypeID) > pSkillProto->nMaxLearnLevel )
		return E_LevelUpSkill_ExceedMaxLearnLevel;

	//��ǰ�������
	if(m_n16MiraclePracticeNum < 1)
	{
		return E_Miracle_LackPracticeNum;
	}

	//��Ǯ
	if(GetCurMgr().GetBagSilver() < pMSkill->dwSilverCost)
	{
		return E_Miracle_LackMoney;
	}

	//ʵ��ֵ
	if((m_Strength.nConsumptiveStrength - pMSkill->n16PowerCost) < 0)
	{
		return E_Miracle_LackPower;
	}

	//��Ʒ
	if(GetItemMgr().GetBagSameItemCount(pMSkill->dwItemTypeID) < pMSkill->n16ItemNumCost)
		return E_Miracle_LackItem;

	//����
	return E_Miracle_Success;
}

//---------------------------------------------------------------------------------
// ���ǰ�ü���
//---------------------------------------------------------------------------------
INT Role::CheckPreSkill(DWORD dwPreLevelSkillID)
{
	// ����IDС��10000��Ϊ���ܺ��ӣ���Ҫ
	if (dwPreLevelSkillID > 0 && dwPreLevelSkillID < 10000)
	{
		const tagSkillProto* pProto = NULL;
		const tagSkillBoxProto * pSkillBoxProto = g_attRes.GetOneSkillProto(dwPreLevelSkillID);
		if( !P_VALID(pSkillBoxProto) )
			return E_LearnSkill_ProtoNotFound;

		// ��ѧϰ������ʣ�����������
		for (int nCnt = 1; nCnt < tagSkillBoxProto::E_SBP_Size; nCnt++)
		{
			if(!P_VALID(pSkillBoxProto->dwSkillIDs[nCnt])) continue;
			DWORD dwPreSkillID = Skill::GetIDFromTypeID(pSkillBoxProto->dwSkillIDs[nCnt]);
			INT nPreSkillLevel = Skill::GetLevelFromTypeID(pSkillBoxProto->dwSkillIDs[nCnt]);
			Skill* pPreSkill = GetSkill(dwPreSkillID);
			if( !P_VALID(pPreSkill) || pPreSkill->GetLevel() < nPreSkillLevel )
				return E_LearnSkill_NoPreSkill;
		}
	}
	else
	{
		DWORD dwPreSkillID = Skill::GetIDFromTypeID(dwPreLevelSkillID);
		INT nPreSkillLevel = Skill::GetLevelFromTypeID(dwPreLevelSkillID);
		Skill* pPreSkill = GetSkill(dwPreSkillID);
		if( !P_VALID(pPreSkill) || pPreSkill->GetLevel() < nPreSkillLevel )
			return E_LearnSkill_NoPreSkill;
	}

	return E_Success;
}

//---------------------------------------------------------------------------------
// F�ƻ�--�Ƿ����ѧ����
//---------------------------------------------------------------------------------
INT	Role::CanLearnSkill(DWORD dwSkillID, const tagSkillProto* pProto, DWORD dwNPCID/*=GT_INVALID*/, INT64 n64ItemID/*=GT_INVALID*/)
{
	INT nTalentIndex = GT_INVALID;			// ��������ʼ����Ҹ�ϵ������Ͷ�����¼��ϵ���ʵ�λ��
	INT nBlankTalentIndex = GT_INVALID;		// ��������ʼ��ܵ���ϵ����δͶ�����¼��ϵ���ʿɲ����λ��

	// ���ü����Ƿ��Ѿ�����
	Skill* pSkill = GetSkill(dwSkillID);
	if( P_VALID(pSkill) ) return E_LearnSkill_Existed;

	if( !P_VALID(pProto) ) return E_LearnSkill_ProtoNotFound;

	// �������ȼ�
	if( m_nLevel < pProto->nNeedRoleLevel )
		return E_LearnSkill_NeedMoreLevel;

	// ���ְҵ����
	if( E_Success != VocationCheck( pProto->dwVocationLimit ))
		return E_LearnSkill_ClassLimit;

	// ���ǰ�ü���
	if( GT_INVALID != pProto->dwPreLevelSkillID )
	{
		INT nRet = CheckPreSkill(pProto->dwPreLevelSkillID);
		if (E_Success != nRet)
		{
			return nRet;
		}
	}

	// ��������ʼ��ܣ���������������������Ƿ��Ѿ�����
	if( ETT_Null != pProto->eTalentType )
	{
		nTalentIndex = FindTalentIndex(pProto->eTalentType);

		if( GT_INVALID != nTalentIndex )
		{
			// ��������ϵ������Ͷ�����ʵ��ǲ������㼼�ܿ�ѧ����
			if( GetTalentPoint(nTalentIndex) < pProto->nNeedTalentPoint )
				return E_LearnSkill_NeedMoreTalentPoint;
		}
		else
		{
			// ��������û�и�ϵ���ʣ����ж��Ƿ񻹿�����ѧϰ�µ�����
			nBlankTalentIndex = FindBlankTalentIndex(pProto->eTalentType);
			if( GT_INVALID == nBlankTalentIndex )
				return E_LearnSkill_ExceedTalentMaxNum;
		}
	}
	
	// NPC��ʽѧϰ
	if( GT_INVALID != dwNPCID )
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) ) return E_LearnSkill_NPCNotEixst;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_LearnSkill_NPCTooFar;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Teacher) )
			return E_LevelUpSkill_NPCCanNotTeach;

		// todo: ��Ҫ���NPC�ɲ����Խ��ڸ��ּ���
	}
	// ������ѧϰ
	else if( GT_INVALID != n64ItemID )
	{
		tagItem *pItem = GetItemMgr().GetBagItem(n64ItemID);
		if( !P_VALID(pItem) )
			return E_Item_NotFound;

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
			return E_Item_LevelLimit;
		
		// �������Ч������
		if( pItem->pProtoType->eSpecFunc != EISF_LearnSkill 
			|| pItem->pProtoType->nSpecFuncVal1 != dwSkillID)
			return E_Item_Used_NotValid;
	}
	// ���ɼ���
	else if (pProto->nType2 == ESSTE_Guild)
	{
		// �ϲ��Ѿ�ͨ����֤
	}
	// �ɽ缼��
	else if (pProto->nType2 == ESSTE_LowerImmortal || pProto->nType2 == ESSTE_NormalImmortal || pProto->nType2 == ESSTE_HighImmortal)
	{
		// �ϲ��Ѿ�ͨ����֤
	}
	else if( pProto->bSoarLimitLearn )
	{

	}
	// Ͷ��ѧϰ��Jason���Ժ����������͵ļ��ܣ���������������жϣ�������ӣ�����Ĭ������ͨ���жϺ󣬶�����ѧϰ��
	else if( ETT_Null != pProto->eTalentType )
	{
		// ���ü����Ƿ�����������
		//if( ETT_Null == pProto->eTalentType )
		//	return E_LearnSkill_NoTalentType;

		// �������Ŀǰ�����Ƿ������ʵ�
		if( GetAttValue(ERA_TalentPoint) <= 0 )
			return E_LearnSkill_NoTalentPoint;

		// ��⼼�ܱ����ǹ̶�������ʽ
		if( ESLUT_Fixed != pProto->eLevelUpType )
			return GT_INVALID;
	}

	// ������������ܣ����ж�Ŀǰ���ϵ��������������Ƿ񳬹����������
	if( ESSTE_Produce == pProto->nType2 )
	{
		if( GetProduceSkillNum() >= MAX_ROLE_PRODUCE_NUM )
			return E_LearnSkill_ProduceSkillFull;
	}
	return E_Success;
}

//---------------------------------------------------------------------------------
// F�ƻ�--ѧ����
//---------------------------------------------------------------------------------
VOID Role::LearnSkill(DWORD dwSkillID, const tagSkillProto* pProto, DWORD dwNPCID/*=GT_INVALID*/, INT64 n64ItemID/*=GT_INVALID*/, BOOL bDelTalentPoint/* = TRUE*/)
{
	if( !P_VALID(pProto) )
		return;

	INT nTalentIndex = FindTalentIndex(pProto->eTalentType);			// ��������ʼ����Ҹ�ϵ������Ͷ�����¼��ϵ���ʵ�λ��
	INT nBlankTalentIndex = FindBlankTalentIndex(pProto->eTalentType);		// ��������ʼ��ܵ���ϵ����δͶ�����¼��ϵ���ʿɲ����λ��

	// һ���ж�ͨ�������������Ӽ���
	INT nSelfLevel = 0;
	INT nLearnLevel = 0;

	// ������ܲ������ʼ��ܣ�������nSelfLevel
	if( ETT_Null == pProto->eTalentType )
	{
		nSelfLevel = 1;
	}
	// ��������ʼ���
	else
	{
		// �����ͨ��NPC���߼�����ѧ���ģ�������nSelfLevel
		if( GT_INVALID != dwNPCID || GT_INVALID != n64ItemID )
		{
			nSelfLevel = 1;

			// ����ĳϵ����
			if( nTalentIndex != GT_INVALID )
			{
				;
			}
			else if( nBlankTalentIndex != GT_INVALID )
			{
				AddTalent(nBlankTalentIndex, pProto->eTalentType, 0);
				SaveTalentPoint2DB(nBlankTalentIndex);
			}
			else
			{
				ASSERT(0);
			}
		}
		// ����������nLearnLevel����������Ӧ�����ʵ�
		else
		{
			nLearnLevel = 1;

			// ��ҿ۳�һ�����ʵ�
			if (bDelTalentPoint)
			{
				ModAttValue(ERA_TalentPoint, -1);

				// �ۼ�ĳЩ���ʵ����ʵ�
				if( nTalentIndex != GT_INVALID )
				{
					AddTalentPoint(nTalentIndex, 1);
					SaveTalentPoint2DB(nTalentIndex);
				}
				else if( nBlankTalentIndex != GT_INVALID )
				{
					AddTalent(nBlankTalentIndex, pProto->eTalentType, 1);
					SaveTalentPoint2DB(nBlankTalentIndex);
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}

	// ��Ӽ���
	Skill* pSkill = new Skill(dwSkillID, nSelfLevel, nLearnLevel, 0, 0);
	AddSkill(pSkill);

	const SkillScript * pSkillScript = pSkill->GetSkillScript();
	if( P_VALID(pSkillScript) )
	{
		pSkillScript->OnSkillChange(GetMap(),GetID(),pSkill->GetID(),0,1);
	}

	const RoleScript * pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript) && P_VALID(pSkill))
	{
		DWORD dwSkillIDWithLevel = pSkill->GetID()*100+pSkill->GetLevel();
		pRoleScript->OnLearnSkill(this, dwSkillIDWithLevel);
	}

	// ���¼�������
	RecalAtt();
}

//-------------------------------------------------------------------------------------
// ѧϰһ������
//-------------------------------------------------------------------------------------
INT Role::LearnSkill(DWORD dwSkillID, DWORD dwNPCID, INT64 n64ItemID)
{
	const tagSkillProto* pProto = NULL;
	INT nRet = E_Success;

	// ����IDС��10000��Ϊ���ܺ��ӣ���Ҫ
	if (dwSkillID > 0 && dwSkillID < 10000)
	{
		const tagSkillBoxProto * pSkillBoxProto = g_attRes.GetOneSkillProto(dwSkillID);
		if( !P_VALID(pSkillBoxProto) || !pSkillBoxProto->bCanBeLearned)
			return E_LearnSkill_ProtoNotFound;

		// �����ҵ��ü��ܶ�Ӧ�ľ�̬���ԣ�Ĭ��һ����
		pProto = g_attRes.GetSkillProto(Skill::CreateTypeID(pSkillBoxProto->dwSkillIDs[0], 1));
		if( !P_VALID(pProto) )
			return E_LearnSkill_ProtoNotFound;

		nRet = CanLearnSkill(pSkillBoxProto->dwSkillIDs[0], pProto, dwNPCID, n64ItemID);
		if (nRet == E_Success)
		{
			// ��ѧϰ�����е�һ������
			LearnSkill(pSkillBoxProto->dwSkillIDs[0], pProto, dwNPCID, n64ItemID);

			// ��ѧϰ������ʣ�����������
			for (int nCnt = 1; nCnt < tagSkillBoxProto::E_SBP_Size; nCnt++)
			{
				// �����ҵ��ü��ܶ�Ӧ�ľ�̬���ԣ�Ĭ��һ����
				pProto = g_attRes.GetSkillProto(Skill::CreateTypeID(pSkillBoxProto->dwSkillIDs[nCnt], 1));
				if( !P_VALID(pProto) )
					return E_LearnSkill_ProtoNotFound;

				// ѧϰ����
				LearnSkill(pSkillBoxProto->dwSkillIDs[nCnt], pProto, dwNPCID, n64ItemID, FALSE);
			}
		}
	}
	else
	{
		// �����ҵ��ü��ܶ�Ӧ�ľ�̬���ԣ�Ĭ��һ����
		pProto = g_attRes.GetSkillProto(Skill::CreateTypeID(dwSkillID, 1));
		if( !P_VALID(pProto) )
			return E_LearnSkill_ProtoNotFound;

		nRet = CanLearnSkill(dwSkillID, pProto, dwNPCID, n64ItemID);
		if (nRet == E_Success)
		{
			// ѧϰ����
			LearnSkill(dwSkillID, pProto, dwNPCID, n64ItemID);
		}
	}

	return nRet;
}

//---------------------------------------------------------------------------------
// F�ƻ�--�Ƿ������������
//---------------------------------------------------------------------------------
INT Role::CanLevelUpSkill(Skill* pSkill, const tagSkillProto* pProto, DWORD dwNPCID/*=GT_INVALID*/, INT64 n64ItemID/*=GT_INVALID*/)
{
	// ��������ʼ����Ҹ�ϵ������Ͷ�����¼��ϵ���ʵ�λ��
	INT nTalentIndex = GT_INVALID;

	// ���ü����Ƿ��Ѿ�����
	if( !P_VALID(pSkill) ) return E_LevelUpSkill_NotExist;

	// ��⼼���Ƿ��Ѿ�������ߵȼ�
	if( pSkill->GetLevel() >= pSkill->GetMaxLevel() )
		return E_LevelUpSkill_ExceedMaxLevel;

	// �����ҵ��ü��ܶ�Ӧ�ľ�̬����
	if( !P_VALID(pProto) ) return E_LevelUpSkill_ProtoNotFound;

	// ����������ȷ�ʽ�����ģ�����ֱ����
	if( ESLUT_Exp == pProto->eLevelUpType )
		return E_LevelUpSkill_NotFixed;

	// �������ȼ�
	if( m_nLevel < pProto->nNeedRoleLevel )
		return E_LearnSkill_NeedMoreLevel;

	// todo: ���ְҵ����
	//if( E_Success != VocationCheck( pProto-> ))

	// ���ǰ�ü���
	if(pProto->dwPreLevelSkillID != GT_INVALID)
	{
		INT nRet = CheckPreSkill(pProto->dwPreLevelSkillID);
		if (E_Success != nRet)
		{
			return nRet;
		}
	}

	// ��������ʼ��ܣ���������������������Ƿ��Ѿ�����
	if( ETT_Null != pProto->eTalentType )
	{
		nTalentIndex = FindTalentIndex(pProto->eTalentType);

		if( GT_INVALID != nTalentIndex )
		{
			// ��������ϵ������Ͷ�����ʵ��ǲ������㼼�ܿ�ѧ����
			if( GetTalentPoint(nTalentIndex) < pProto->nNeedTalentPoint )
				return E_LearnSkill_NeedMoreTalentPoint;
		}
		else
		{
			ASSERT(0);
			return GT_INVALID;
		}
	}

	// NPC��ʽ����
	if( GT_INVALID != dwNPCID )
	{
		// NPC����������������
		if( ETT_Null != pProto->eTalentType )
			return GT_INVALID;

		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC) ) return E_LevelUpSkill_NPCNotEixst;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_LevelUpSkill_NPCTooFar;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Teacher) )
			return E_LevelUpSkill_NPCCanNotTeach;

		// todo: ��Ҫ���NPC�ɲ����Խ��ڸ��ּ���
	}
	// ������ѧϰ
	else if( GT_INVALID != n64ItemID )
	{
		// �����鲻��������������
		if( ETT_Null != pProto->eTalentType )
			return GT_INVALID;

		// todo: ����Ʒ�����������Ʒ�����������Ч������

		// todo: ���ü��������ϴ��ļ���ID�ǲ��ǲ����еļ���ID
	}
	// ���ɼ���
	else if (pProto->nType2 == ESSTE_Guild)
	{
		// �ϲ��Ѿ�ͨ����֤
	}
	else if( pProto->nType2 == ESSTE_FairySoul )
	{
	}
	// Ͷ��ѧϰ��shit�������ƣ��Ժ��¼�type2�ļ��ܣ���Ҫ��Ͷ���ж����ɲ���
	else if( ETT_Null != pProto->eTalentType )
	{
		// ���ü����Ƿ�����������
		//if( ETT_Null == pProto->eTalentType )
		//	return E_LevelUpSkill_NoTalentType;

		// �������Ŀǰ�����Ƿ������ʵ�
		if( GetAttValue(ERA_TalentPoint) <= 0 )
			return E_LevelUpSkill_NoTalentPoint;

		// ��⼼�ܱ����ǹ̶�������ʽ
		if( ESLUT_Fixed != pProto->eLevelUpType )
			return GT_INVALID;
	}

	return E_Success;
}

//---------------------------------------------------------------------------------
// F�ƻ�--��������
//---------------------------------------------------------------------------------
VOID Role::LevelUpSkill(Skill* pSkill, const tagSkillProto* pProto, DWORD dwNPCID/*=GT_INVALID*/, INT64 n64ItemID/*=GT_INVALID*/, BOOL bDelTalentPoint/* = TRUE*/)
{
	if( !P_VALID(pProto) )
		return;

	// ��������ʼ����Ҹ�ϵ������Ͷ�����¼��ϵ���ʵ�λ��
	INT nTalentIndex = FindTalentIndex(pProto->eTalentType);

	// һ���ж�ͨ����������＼������
	BOOL bLearn = FALSE;

	// �����ͨ��NPC���߼����������ģ���������ȼ�����
	if( GT_INVALID != dwNPCID || GT_INVALID != n64ItemID )
	{
		bLearn = 1;
	}
	// ���ɼ�������
	else if (pProto->nType2 == ESSTE_Guild)
	{
		bLearn = FALSE;
	}
	// �����Ͷ��������
	else if( ETT_Null != pProto->eTalentType )
	{
		bLearn = TRUE;

		// ��ҿ۳�һ�����ʵ�
		if (bDelTalentPoint)
		{
			ModAttValue(ERA_TalentPoint, -1);

			// �ۼ�ĳϵ���ʵ����ʵ�
			if( nTalentIndex != GT_INVALID )
			{
				AddTalentPoint(nTalentIndex, 1);
				SaveTalentPoint2DB(nTalentIndex);
			}
			else
			{
				ASSERT(0);
			}
		}
	}

	// ����
	if( bLearn )
		ChangeSkillLevel(pSkill, ESLC_Learn);
	else
		ChangeSkillLevel(pSkill, ESLC_Self);

	const RoleScript * pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript) && P_VALID(pSkill))
	{
		DWORD dwSkillIDWithLevel = pSkill->GetID()*100+pSkill->GetLevel();
		pRoleScript->OnLearnSkill(this, dwSkillIDWithLevel);
	}

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();
}

//---------------------------------------------------------------------------------------
// ����һ������
//---------------------------------------------------------------------------------------
INT Role::LevelUpSkill(DWORD dwSkillID, DWORD dwNPCID, INT64 n64ItemID)
{
	INT nRet = E_Success;
	const tagSkillProto* pProto = NULL;

	// ����IDС��10000��Ϊ���ܺ��ӣ���Ҫ
	if (dwSkillID > 0 && dwSkillID < 10000)
	{
		const tagSkillBoxProto * pSkillBoxProto = g_attRes.GetOneSkillProto(dwSkillID);
		if( !P_VALID(pSkillBoxProto) || !pSkillBoxProto->bCanBeLearned)
			return E_LearnSkill_ProtoNotFound;

		// ��ȡ���ܶ���
		Skill* pSkill = GetSkill(pSkillBoxProto->dwSkillIDs[0]);

		if(!P_VALID(pSkill)) return E_LearnSkill_ProtoNotFound;

		// �ҵ��ü��ܶ�Ӧ�ľ�̬����
		INT nLevel = pSkill->GetLevel() + 1;
		pProto = g_attRes.GetSkillProto(Skill::CreateTypeID(pSkillBoxProto->dwSkillIDs[0], nLevel));
		if( !P_VALID(pProto) ) return E_LevelUpSkill_ProtoNotFound;

		nRet = CanLevelUpSkill(pSkill, pProto, dwNPCID, n64ItemID);
		if (nRet == E_Success)
		{
			// ��������
			LevelUpSkill(pSkill, pProto, dwNPCID, n64ItemID);
			
			for (int nCnt = 1; nCnt < tagSkillBoxProto::E_SBP_Size; nCnt++)
			{
				// ��ȡ���ܶ���
				Skill* pSkill = GetSkill(pSkillBoxProto->dwSkillIDs[nCnt]);
				if(!P_VALID(pSkill)) continue;
				// �ҵ��ü��ܶ�Ӧ�ľ�̬����
				INT nLevel = pSkill->GetLevel() + 1;
				pProto = g_attRes.GetSkillProto(Skill::CreateTypeID(pSkillBoxProto->dwSkillIDs[nCnt], nLevel));
				if( !P_VALID(pProto) ) return E_LevelUpSkill_ProtoNotFound;

				// ��������,ֻ�е�һ�����ܿ۳����ʵ���
				LevelUpSkill(pSkill, pProto, dwNPCID, n64ItemID, FALSE);
			}
		}
	}
	else
	{
		// ��ȡ���ܶ���
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill)) return E_LearnSkill_ProtoNotFound;

		// �õ�������ľ�̬���ԣ�Ȼ���ҵ��ü��ܶ�Ӧ�ľ�̬����
		INT nLevel = pSkill->GetLevel() + 1;
		pProto = g_attRes.GetSkillProto(Skill::CreateTypeID(dwSkillID, nLevel));
		if( !P_VALID(pProto) ) return E_LevelUpSkill_ProtoNotFound;

		// �жϼ����Ƿ�������
		nRet = CanLevelUpSkill(pSkill, pProto, dwNPCID, n64ItemID);
		if (nRet == E_Success)
		{
			// ��������
			LevelUpSkill(pSkill, pProto, dwNPCID, n64ItemID);
		}
	}

	return nRet;
}

//---------------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------------
INT Role::ForgetSkill(DWORD dwSkillID, DWORD dwNPCID)
{
	// �����������Ƿ��иü���
	Skill* pSkill = GetSkill(dwSkillID);
	if( !P_VALID(pSkill) ) return E_ForgetSkill_NotExist;

	// ��⼼�ܺϷ���
	if( ETT_Null != pSkill->GetTalentType() )
		return E_ForgetSkill_CanNotBeForgotten;

	// ���NPC�Ϸ���
	Creature* pNPC = GetMap()->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ||
		FALSE == pNPC->IsFunctionNPC(EFNPCT_Teacher) ||
		FALSE == pNPC->CheckNPCTalkDistance(this))
		return E_ForgetSkill_NPCNotValid;

	// todo�����NPC�Ƿ�ӵ�иü���

	// ���ͨ����ɾ������
	RemoveSkill(dwSkillID);

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;
}

//-----------------------------------------------------------------------------------
// ϴ��
//-----------------------------------------------------------------------------------
INT Role::ClearTalent(INT64 n64ItemID, BOOL bCheckItem)
{
	if (bCheckItem)
	{
		// �ж���Ʒ�����ǲ���ϴ�����
		tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID);
		if( !P_VALID(pItem) || (pItem->pProtoType->eSpecFunc != EISF_RemoveTalentPt && pItem->pProtoType->eSpecFunc != EISF_ChangeVocation))
			return E_ClearAttPoint_ItemNotValid;

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
			return E_Item_LevelLimit;

		// �ж�ͨ����ɾ����Ʒ
		GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCID_Clear_Talent);
	}
	
	// ����Ƿ�������
	for(INT n = 0; n < X_MAX_TALENT_PER_ROLE; ++n)
	{
		if( ETT_Null == m_Talent[n].eType  || m_Talent[n].eType == ETT_FreshHand)
			continue;

		// ɾ�����и�ϵ�����ʼ��ܣ���map�������������ܻ������⣩
		TMap<DWORD, Skill*>::TMapIterator it = m_mapSkill.Begin();
		Skill* pSkill = NULL;
		while( m_mapSkill.PeekNext(it, pSkill) )
		{
			if( pSkill->GetTalentType() == m_Talent[n].eType )
				RemoveSkill(pSkill->GetID());
		}

		// ���������ظ����
		ModAttValue(ERA_TalentPoint, GetTalentPoint(n));
		// ��ո�ϵ����
		RemoveTalent(n);
		// �������ʵ����ݿ�
		SaveTalentPoint2DB(n);
	}
    
	//------------------------------------------------------------------------------
	// ϴ���ʳƺ��¼�����
	// zxzhang   2009-12-29
	//------------------------------------------------------------------------------
	GetTitleMgr()->SigEvent( ETE_CLEARTALENT,GT_INVALID ,GT_INVALID );

	// ���¼�����������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;

}

//----------------------------------------------------------------------------------
// ����PK����
//----------------------------------------------------------------------------------
INT Role::OpenSafeGuard(INT64 n64ItemID)
{
	// �鿴�Ƿ��Ѿ����ù���
	if( IsInRoleState(ERS_Safeguard) ) return E_SafeGuard_Already;

	if( P_VALID(GetMap()))
	{
		if( !GetMap()->CanSetSafeguard() )	return E_SafeGuard_PKStateInvalid;
	}

	// ���һ��PK״̬������״̬
	if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) || m_ePKState != ERPKS_Peace || IsInRoleState(ERS_Arena))
		return E_SafeGuard_PKStateInvalid;

	// ���һ��ʱ��
	//tagDWORDTime curTime = g_world.GetWorldTime();
	//if( CalcTimeDiff(curTime, m_CloseSafeGuardTime) < OPEN_SAFE_GUARD_TIME )
	//	return E_SafeGuard_TimeLimit;

	// ���һ�µȼ�
	BOOL bUseItem = FALSE;
	if( GetLevel() >= SAFE_GUARD_FREE_LEVEL )
	{
		// �����Ʒ�Ƿ�Ϸ�
		const tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID);
		if( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_OpenSafeGuard )
		{
			return E_SafeGuard_ItemLimit;
		}
		else
		{
			bUseItem = TRUE;
		}

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
		{
			return E_Item_LevelLimit;
		}
	}

	// �ж�ͨ�����������ΪPK����״̬
	//SetRoleState(ERS_Safeguard);

	// �����ʹ������Ʒ�򿪣���Ҫ�ӱ�����ɾ����Ʒ
	if( bUseItem )
	{
		GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCID_Open_SafeGuard);
		SetRoleState(ERS_Safeguard);
		return E_Success;
	}

	return E_SafeGuard_OpenLimit;
}

//----------------------------------------------------------------------------------
// �ر�PK����
//----------------------------------------------------------------------------------
INT Role::CloseSafeGuard()
{
	// ����ǲ����Ѿ��Ǹ�״̬
	if( !IsInRoleState(ERS_Safeguard) ) return E_SafeGuard_Already;

	// ���ȼ��ȼ�����
	INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
	if( g_world.IsPkConstraintsOpen() && IsUsingPKConstraintsSafeGuradMinLevel() )
	{
		Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
	}
	if( GetLevel() < Max_PK_SafeGuard_Level )
		return E_SafeGuard_LevelLimit;

	// �ж�ͨ����������ҵ�PK����״̬
	UnsetRoleState(ERS_Safeguard);

	// ������ҹر�PK������ʱ��
	m_CloseSafeGuardTime = g_world.GetWorldTime();

	return E_Success;
}

// ��Buffʱ����PK����
INT Role::OpenSafeGuard()
{
	// �鿴�Ƿ��Ѿ����ù���
	if( IsInRoleState(ERS_Safeguard) ) return E_SafeGuard_Already;

	if( P_VALID(GetMap()))
	{
		if( !GetMap()->CanSetSafeguard() )	return E_SafeGuard_PKStateInvalid;
	}

	// ���һ��PK״̬������״̬
	if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) || m_ePKState != ERPKS_Peace || IsInRoleState(ERS_Arena))
		return E_SafeGuard_PKStateInvalid;

	// �ж�ͨ�����������ΪPK����״̬
	SetRoleState(ERS_Safeguard);

	return E_Success;

}

//----------------------------------------------------------------------------------
// ��������״̬
// Jason 2010-6-22 �޸�pk
// bOnlyPK : TRUE:�Կ�ģʽ; FALSE:ɱ¾ģʽ; -1:����ģʽ
//----------------------------------------------------------------------------------
INT Role::SetPK(BOOL bOnlyPK)
{
	if (bOnlyPK == TRUE)
	{
		if (IsInRoleState(ERS_PKEX))
			return E_PKEX_Already;
	}
	// ����ģʽ
	else if( GT_INVALID == bOnlyPK )
	{
		if (IsInRoleState(ERS_PeaceModel))
			return E_PK_PeaceModelAlready;
	}
	else
	{
		if (IsInRoleState(ERS_PK))
			return E_PK_Already;
	}

	// �������ǲ��ǿ���PK����
	if( IsInRoleState(ERS_Safeguard) ) return E_PK_SafeGuard;

	if( m_nUnSetPKTickCountDown > 0 )
	{
		return E_SafeGuard_TimeLimit ;
	}

	// �ж�ͨ�����������Ϊ����״̬����������״̬���⣩
	INT nTick = UNSET_PK_TICK;
	if( GT_INVALID == bOnlyPK )
		nTick = 0;
	if ( TRUE == bOnlyPK )
	{
		SetRoleState(ERS_PKEX);
		UnsetRoleState(ERS_PeaceModel);
		UnsetRoleState(ERS_PK);
	}
	else if( GT_INVALID == bOnlyPK )
	{
		SetRoleState(ERS_PeaceModel);
		UnsetRoleState(ERS_PKEX);
		UnsetRoleState(ERS_PK);
	}
	else
	{
		SetRoleState(ERS_PK);
		UnsetRoleState(ERS_PeaceModel);
		UnsetRoleState(ERS_PKEX);
	}

	// ���ó�������״̬����ʱ
	m_nUnSetPKTickCountDown = nTick;
	
	OpenMacroToDealWithLuckState();

	return E_Success;
}

//----------------------------------------------------------------------------------
// ��������״̬
//----------------------------------------------------------------------------------
VOID Role::ResetPK()
{
	if( !IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) ) return;
	m_nUnSetPKTickCountDown = UNSET_PK_TICK;

	tagNS_PK send;
	send.dwErrorCode = E_Success;
	send.bReset = TRUE;
	SendMessage(&send, send.dwSize);
	
	CloseMacroToDealWithLuckState();
}

//----------------------------------------------------------------------------------
// ��������״̬
// Jason 2010-6-22 v1.0.0 pk����
//----------------------------------------------------------------------------------
VOID Role::UpdatePK()
{
	if( !IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) && !IsInRoleState(ERS_PeaceModel) ) return;

	if(--m_nUnSetPKTickCountDown <= 0)
	{
		m_nUnSetPKTickCountDown = -1;
		//UnsetRoleState(ERS_PK);
		//UnsetRoleState(ERS_PKEX);
		//UnsetRoleState(ERS_PeaceModel);
	}
}

//----------------------------------------------------------------------------------
// ���¾���ֵ
//----------------------------------------------------------------------------------
VOID Role::UpdateSpirit()
{
	if(g_attRes.IsSpiritSysOn() && GetLevel() >= (INT)g_attRes.SpiritStartLevel() && !IsInState(ES_Dead))
	{
		m_dwSpiUpdateTickCount = (m_dwSpiUpdateTickCount+1) % (5 * 60); //ÿ300��tick����һ��
		if(m_dwSpiUpdateTickCount == 0)
		{
			if(IsInRoleState(ERS_SafeArea)) // ��Ҫ���Ӿ���ֵ
			{
				if(m_nAtt[ERA_Spirit] < g_attRes.GetAttDefRole(ERA_Spirit))
				{
					SetAttValue(ERA_Spirit, m_nAtt[ERA_Spirit]+1);
				}
			}
			else if(m_dwFixSpiriteCDTime <= 0 && m_nAtt[ERA_Spirit]>0) // cdʱ��������
			{
				SetAttValue(ERA_Spirit, m_nAtt[ERA_Spirit]-1);
			}			
		}
	}
	if(m_dwFixSpiriteCDTime > 0) m_dwFixSpiriteCDTime--; // ÿ�����ڱ�Ȼ����һ��
}

float Role::CalSpiritRate(INT nSpirit)
{
	float rate = 1.;
	DWORD nUpper = g_attRes.GetAttMax(ERA_Spirit);
	const std::map<DWORD, float>& rateMap = g_attRes.GetSpiriteRate();
	for(std::map<DWORD, float>::const_iterator itor=rateMap.begin(); rateMap.end() != itor; itor++)
	{
		if(itor->first<=nUpper && nSpirit <= itor->first)
		{
			rate = itor->second;
			nUpper = itor->first;
		}
	}
	return rate;
}

//----------------------------------------------------------------------------------
// ����PVP״̬
//----------------------------------------------------------------------------------
VOID Role::SetPVP()
{
	if( !IsInRoleState(ERS_PVP) )
	{
		SetRoleState(ERS_PVP);
	}
	
	m_nUnSetPVPTickCountDown = UNSET_PVP_TICK;
}

//----------------------------------------------------------------------------------
// ����PVP״̬
//-----------------------------------------------------------------------------------
VOID Role::UpdatePVP()
{
	if( !IsInRoleState(ERS_PVP) ) return;

	if(--m_nUnSetPVPTickCountDown <= 0)
	{
		UnsetRoleState(ERS_PVP);
	}
}

//----------------------------------------------------------------------------------
// ����PK״̬
// Jason 2010-6-22 v1.0.0 �޸�pk
//----------------------------------------------------------------------------------
VOID Role::CalPKState(BOOL bSendMsg)
{
	ERolePKState ePKState = m_ePKState;
	INT n = 0;
	for(; n < ERPKS_End; n++)
	{
		if( m_nAtt[ERA_Morality] >= PK_STATE_MORALITY[n] )
		{
			ePKState = (ERolePKState)n;
			break;
		}
	}
	ASSERT( n != ERPKS_End );
	if( n >= ERPKS_End )
		return;

	// PK״̬�ı�
	if( ePKState != m_ePKState || m_bInitPkState )
	{
		ERolePKState ePKStateOld = m_ePKState;
		m_ePKState = ePKState;

		if( bSendMsg )
		{
			tagNS_RolePKStateChange send;
			send.dwRoleID = GetID();
			send.eState = ePKState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
				m_bInitPkState = FALSE;
			}
			else
			{
				m_bInitPkState = TRUE;
			}

			if( ERPKS_Warnning == ePKStateOld &&  ERPKS_Wicked == m_ePKState )
			{
				tagNS_RolePKMorality send;
				send.nTime = g_world.GetMoralityTime();
				send.nValue = g_world.GetReturnMorality();
				SendMessage(&send,send.dwSize);
			}
		}
		else
		{
			m_bInitPkState = TRUE;
		}
	}
}

//----------------------------------------------------------------------------------
// ��������Ϊ0ʱ�سǸ����pk����
//----------------------------------------------------------------------------------
VOID Role::SetHostilitySafeGuard()
{
	// ����Ϊ0�سǸ���Ļ�����10���ӵ�pk����
	//if ( GetAttValue(ERA_Hostility) == 0 )
	return;
	if(m_nLevel > SAFE_GUARD_FORCE_LEVEL)
	{
		m_nDeadUnSetSafeGuardCountDown = REVIVE_UNSETSAFEGUARD_TICK;
		SetRoleState(ERS_Safeguard);
	}
}

//----------------------------------------------------------------------------------
// ��������Ϊ0ʱ�سǸ����pk����
//----------------------------------------------------------------------------------
VOID Role::UpdateHostilitySafeGuard()
{
	return;
	// ��ҵȼ�����30��ʱ������
	if ( m_nLevel <= SAFE_GUARD_FORCE_LEVEL )	return;

	if( !IsInRoleState(ERS_Safeguard) ) return;

	if (m_nDeadUnSetSafeGuardCountDown < 0)
	{
		return;
	}
	else if (m_nDeadUnSetSafeGuardCountDown > 0)
	{
		--m_nDeadUnSetSafeGuardCountDown;
	}
	else
	{
		UnsetRoleState(ERS_Safeguard);
		m_nDeadUnSetSafeGuardCountDown = -1;
	}
}

//----------------------------------------------------------------------------------
// ����ı�
//----------------------------------------------------------------------------------
VOID Role::ExpChange(INT64 nValue, BOOL bKill, BOOL bForce,BOOL bShare /*= FALSE*/)
{
	if( 0 == nValue ) return;

	// ���پ���
	if( nValue < 0 )
	{
		INT64 nTemp = m_nCurLevelExp;
		m_nCurLevelExp += nValue;
		if( m_nCurLevelExp < 0 ) m_nCurLevelExp = 0;

		tagNS_RoleExpChange send;
		send.nExp = m_nCurLevelExp;
		send.nChange = m_nCurLevelExp - nTemp;
		send.bKill = bKill;
		send.bShare = bShare;
		SendMessage(&send, send.dwSize);

		if (!bKill)
		{
			m_bSaveExpDirect = TRUE;
			SaveExp2DB();
		}

		return;
	}

	// ��������Ӿ���
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
	INT64 nLevelUpExpRemain = pEffect->nExpLevelUp - m_nCurLevelExp;
	ASSERT( nLevelUpExpRemain >= 0 );

	// ��������
	if( nValue < nLevelUpExpRemain )
	{
		// ����Ƿ���10%�ı�����������ˣ������һ���Ѫ������
		//INT nPhaseExp = pEffect->nExpLevelUp / 10;
		//INT nOldPhase = m_nCurLevelExp / nPhaseExp;
		//INT nNewPhase = (m_nCurLevelExp + nValue) / nPhaseExp;
		//if( nOldPhase != nNewPhase )
		//{
		//	SetAttValue(ERA_HP, GetAttValue(ERA_MaxHP));
		//	SetAttValue(ERA_MP, GetAttValue(ERA_MaxMP));
		//}

		// �ۼ����µľ���
		m_nCurLevelExp += nValue;

		tagNS_RoleExpChange send;
		send.nExp = m_nCurLevelExp;
		send.nChange = nValue;
		send.bKill = bKill;
		send.bShare = bShare;
		SendMessage(&send, send.dwSize);
	}

	// ��Ҫ����
	else
	{
		// �����Ƿ�ǿ���������������ܵĵȼ�����
		INT nMaxLevel = g_roleMgr.GetRoleLevelLimit();
		// Jason ������������
		//if( GetAttValue(ERA_Soar) == ESV_SoaringUp )
		//{
		//	nMaxLevel = m_nLevel;
		//}
		/*else */if( bForce )
		{
			nMaxLevel = MAX_ROLE_LEVEL;
		}

		m_nCurLevelExp += nLevelUpExpRemain;

		// �Ѿ���������
		if( m_nLevel == nMaxLevel )
		{
			if( nLevelUpExpRemain != 0 )
			{
				tagNS_RoleExpChange send;
				send.nExp = m_nCurLevelExp;
				send.nChange = nLevelUpExpRemain;
				send.bKill = bKill;
				send.bShare = bShare;
				SendMessage(&send, send.dwSize);
			}
		}

		// ��������
		else
		{
			m_bSaveExpDirect = TRUE;
			INT64 nTemExpValue = nValue;
			INT64 nExpRealAdd = nLevelUpExpRemain;
			nValue -= nLevelUpExpRemain;
			INT nNextLevel = m_nLevel + 1;
			INT nAttPointAdd = 0;
			INT nTalentPointAdd = 0;

			// ��������������
			for(; nNextLevel <= nMaxLevel; nNextLevel++)
			{
				pEffect = g_attRes.GetLevelUpEffect(nNextLevel);

				// �������ټ�������
				if( pEffect->nExpLevelUp <= nValue )
				{
					nValue -= pEffect->nExpLevelUp;
					nExpRealAdd = pEffect->nExpLevelUp;
					nAttPointAdd += pEffect->n16RoleAttAvail;
					nTalentPointAdd += pEffect->n16TalentAvail;
				}
				// ��������������
				else
				{
					nAttPointAdd += pEffect->n16RoleAttAvail;
					nTalentPointAdd += pEffect->n16TalentAvail;
					break;
				}
			}

			// ������ı�ȼ�
			if( nNextLevel > nMaxLevel )
			{
				nNextLevel = nMaxLevel;
				// ��������һ��
				if( nValue > pEffect->nExpLevelUp )
				{
					nValue = pEffect->nExpLevelUp;
				}
			}

			LevelChange(nNextLevel, bKill, bForce);

			m_nCurLevelExp = nValue;
			nExpRealAdd += nValue;

			// ������������Ե�����ʵ�
			ModAttValue(ERA_AttPoint, nAttPointAdd);

			//���ʻ������
			//�������������ʽ�������
			tagNS_RoleGetTalentPoint sendMsg ;
			INT nLevel = nNextLevel;
			INT nCurrentTalent = GetAttValue(ERA_TalentPoint); //��ȡ��ǰ�Ŀ�Ͷ����
			INT nCurrentTotalTalent = nCurrentTalent + GetTotalTalentPoint(); //��ȡ��ǰ������(������Ͷ����)
			INT nCurrentTalentAvail_Limit = GetTalentAvail_limit(nLevel);  //��ȡ��ǰ����ɻ���������
			INT nRew_att_val = 0;
			if ( nCurrentTotalTalent >= nCurrentTalentAvail_Limit) // ��ǰ��������������ǰ�������ɻ������ �Ͳ�������
			{
				nRew_att_val = 0;
				sendMsg.dwErrorCode = ERTEC_OverMaxTalentAvail_Limit;
			}
			else
			{
				nRew_att_val = nTalentPointAdd;
				INT nTotalTalent = nRew_att_val + nCurrentTotalTalent ;  // ��ǰ�������ʼ������ͻ������ 
				if ( nTotalTalent > nCurrentTalentAvail_Limit)
				{
					nRew_att_val = nCurrentTalentAvail_Limit - nCurrentTotalTalent;
					sendMsg.dwErrorCode = ERTEC_OverMaxTalentAvail_Limit;
				}
				else
				{
                    sendMsg.dwErrorCode = ERTEC_Success; 
				}

			}
			SendMessage(&sendMsg,sendMsg.dwSize);
			ModAttValue(ERA_TalentPoint, nRew_att_val);
		

			// ���;���ı�
			tagNS_RoleExpChange send;
			send.nExp = m_nCurLevelExp;
			send.nChange = nTemExpValue;//nExpRealAdd;
			send.bKill = bKill;
			send.bShare = bShare;
			SendMessage(&send, send.dwSize);

			SaveAttPoint2DB();
			SaveTalentPoint2DB(0);
			//m_SaveRoleLock.Acquire();
			//SaveToDB();
			//m_SaveRoleLock.Release();

			FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
			if( P_VALID(pFamilySprite) )
			{
				pFamilySprite->UpdateRoleAtt(GetID());
			}
		}
	}

	if (!bKill)
	{
		SaveExp2DB();
	}
}

//----------------------------------------------------------------------------
// �ı�ȼ�
//----------------------------------------------------------------------------
VOID Role::LevelChange(INT nValue, BOOL bKill, BOOL bForce)
{
	// �����Ƿ�ǿ��������������ߵȼ�
	INT nMaxLevel = g_roleMgr.GetRoleLevelLimit();
	if( bForce ) nMaxLevel = MAX_ROLE_LEVEL;

	if( nValue < 0 || nValue > nMaxLevel )
		return;

	if( m_nLevel == nValue ) return;

	INT nOldLevel = m_nLevel;

	m_nLevel = nValue;
	m_dwLastUpgrade = g_world.GetWorldTime();//GetCurrentDWORDTime();
	m_nCurLevelExp = 0;

	// ���͵ȼ��ı�
	tagNS_RoleLevelChange send;
	send.dwRoleID = GetID();
	send.nLevel = m_nLevel;
	send.bKill = send.bKill;

	SaveLevel2DB();

	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}

	// �����С����,���͵ȼ��ı��¼�
	if(GetTeamID() != GT_INVALID)
		g_groupMgr.AddEvent(GetID(), EVT_ChangeLevel, send.dwSize, &send);

	// ͬ��tagRoleInfo
	g_socialMgr.AddEvent(GetID(), EVT_SynRoleLevel, send.dwSize, &send);

	// �ƺ��¼�����
	GetTitleMgr()->SigEvent(ETE_ROLE_LEVEL, GT_INVALID, m_nLevel);

	// ����������Լ�����
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
	SetBaseAttValue(ERA_Physique,	pEffect->n16RoleAtt[0] + m_nAttPointAdd[0]);
	SetBaseAttValue(ERA_Strength,	pEffect->n16RoleAtt[1] + m_nAttPointAdd[1]);
	SetBaseAttValue(ERA_Pneuma,		pEffect->n16RoleAtt[2] + m_nAttPointAdd[2]);
	SetBaseAttValue(ERA_InnerForce,	pEffect->n16RoleAtt[3] + m_nAttPointAdd[3]);
	//��������������Ȼ�����ˣ�������Ҫ������¼����־���������Լ���һ������
	SetBaseAttValue(ERA_Technique,	0/*pEffect->n16RoleAtt[4] + m_nAttPointAdd[4]*/);
	SetBaseAttValue(ERA_Agility,	0/*pEffect->n16RoleAtt[5] + m_nAttPointAdd[5]*/);

	// ����ĳЩ������������
	SetBaseAttValue(ERA_MaxHP,		pEffect->n16HP);
	SetBaseAttValue(ERA_MaxMP,		pEffect->n16MP);
	SetBaseAttValue(ERA_MaxVitality,pEffect->n16Vitality);
	// ����ֵ�������������ĺ���������㣬������������һ��ֵ
	SetBaseAttValue(ERA_MaxEndurance,100);

	RecalAtt();

	// ����Ѫ��������
	SetAttValue(ERA_HP, GetAttValue(ERA_MaxHP));
	SetAttValue(ERA_MP, GetAttValue(ERA_MaxMP));

	// �������20�����ر�pk����
	INT nRet = E_Success;
	INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
	if( g_world.IsPkConstraintsOpen() )
	{
		Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
		if(nOldLevel <= SAFE_GUARD_FORCE_LEVEL && m_nLevel > SAFE_GUARD_FORCE_LEVEL)
		{
			// ���Ϳͻ�����Ϣ���Ƿ�Ҫͬ��ر�pk����
			tagNS_AskToClosePKSafeGuard msg_;
			msg_.nPKSafeGuardMaxLevel = Max_PK_SafeGuard_Level;
			SendMessage(&msg_,msg_.dwSize);
		}
	}
	if (m_nLevel > Max_PK_SafeGuard_Level)
	{
		//if( !m_bLevelUpClosedSafeGuard )
		if( nOldLevel <= Max_PK_SafeGuard_Level )
		{
			nRet = CloseSafeGuard();

			// ������Ϣ���ͻ���
			tagNS_SafeGuard send;
			send.bOpen = FALSE;
			send.dwErrorCode = nRet;
			SendMessage(&send, send.dwSize);

// 			m_bLevelUpClosedSafeGuard = TRUE;

			//tagNS_SetRoleState srs_msg;
			//srs_msg.dwRoleID	= GetID();
			//srs_msg.eState = ERS_PeaceModel;
			//SendMessage(&srs_msg,srs_msg.dwSize);
			SetPK(-1);
		}
		
	}
	else
	{
		if (g_world.IsPkConstraintsOpen())
		{
			if (IsUsingPKConstraintsSafeGuradMinLevel())
			{
				if( !IsInRoleState(ERS_Safeguard) )
					SetRoleState(ERS_Safeguard);
			}
		}
		else
		{
			if( !IsInRoleState(ERS_Safeguard) )
				SetRoleState(ERS_Safeguard);
		}
	}

	if (bForce)
	{
		PlayerSession* pSession = GetSession();
		if (P_VALID(pSession) && !pSession->IsPrivilegeEnough(1))
		{
			tagNWL_ForbidAccount send;
			send.dwAccountID = pSession->GetSessionID();
			send.dwForbidCode = EPLM_Level;
			g_loginSession.Send(&send, send.dwSize);
		}
	}

	// ���������¼�
	OnQuestEvent(EQE_Level);

	// ���������Ľű�����
	if( P_VALID(m_pScript))
	{
		m_pScript->OnRoleLevelChange(this);
	}
	// ����
	if( m_nLevel >= 80 )
	{ 
		if( g_world.IsSoaringOpen() && GetAttValue(ERA_Soar) != ESV_SoaringUp )
		{
			tagNS_QuerySoaringSwitch msg;
			msg.dwErrorCode = ESEC_CanOpen;
			SendMessage(&msg,msg.dwSize);
		}
	}

	if (m_nLevel >= BloodBrother_DistinguishLevel)
	{
		if (!m_BrotherMgr.CanMakeBrother())
		{
			
			tagNS_BrotherEnd msg2;
			msg2.bReason = 1;
			SendMessage(&msg2,msg2.dwSize);

			DWORD dwTeamID = GetTeamID();
			if (dwTeamID == GT_INVALID)
				return;

			const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
			if (!P_VALID(pTeam))
				return;

			pTeam->DeleteTeamBloodBrotherBuff(this);

			m_BrotherMgr.ClearTeatherData();
			CancelBrotherProfit();
				
		}
	}
}

//-------------------------------------------------------------------------------
// Ͷ��
//-------------------------------------------------------------------------------
INT Role::BidAttPoint(const INT nAttPointAdd[X_ERA_ATTA_NUM])
{
	// �������Ƿ�Ϸ�
	INT nSum = 0;
	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( nAttPointAdd[n] < 0 )
			return E_BidAttPoint_PointInvalid;

		nSum += nAttPointAdd[n];
	}

	// ����ܵ���Ϊ0����ֱ�ӷ���
	if( nSum <= 0 ) return E_BidAttPoint_PointInvalid;

	// �����ҵ�ǰ���ϵĿ�Ͷ�����Ե㹻����
	if( GetAttValue(ERA_AttPoint) < nSum )
		return E_BidAttPoint_NoEnoughAttPoint;

	// �ȿ۳����Ե�
	ModAttValue(ERA_AttPoint, -nSum);

	// �����ζ�Ӧÿ�����Խ��мӳ�
	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( 0 == nAttPointAdd[n] ) continue;

		m_nAttPointAdd[n] += nAttPointAdd[n];
		ModBaseAttValue(MTransIndex2ERAATTA(n), nAttPointAdd[n]);
	}

	// ������������
	RecalAtt();

	// ����ÿ������Ͷ������ǰֵ���ͻ���
	tagNS_RoleAttPointAddChange send;
	IFASTCODE->MemCpy(send.nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));
	SendMessage(&send, send.dwSize);

	SaveAttPoint2DB();

	return E_Success;
}

//----------------------------------------------------------------------------
// ϴ���Ե�
//----------------------------------------------------------------------------
INT Role::ClearAttPoint(INT64 n64ItemID, BOOL bItemCheck)
{
	BOOL bNeedClear = FALSE;
	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( m_nAttPointAdd[n] > 0 )
		{
			bNeedClear = TRUE;
			break;
		}
	}

	if (bNeedClear == FALSE)
	{
		return E_ClearAttPoint_NoNeedClear;
	}

	// F�ƻ�--�жϽ�ɫ�������Ƿ��н�ɫϴ�����͵���
	tagItem* pItem = GetItemMgr().GetBag().GetOneSpecItemBySpecFunEnum(EISF_RemoveAttPt);

	if (bItemCheck)
	{
		if( !P_VALID(pItem) || ( pItem->pProtoType->eSpecFunc != EISF_RemoveAttPt ) )
			return E_ClearAttPoint_ItemNotValid;

		// ʹ�õĵȼ��ж�
		if( pItem->IsLevelLimit(GetLevel()) )
			return E_Item_LevelLimit;

		// �ж�ͨ����ɾ����Ʒ
		GetItemMgr().ItemUsedFromBag(pItem->n64Serial, (INT16)1, (DWORD)ELCID_Clear_Att);
	}
	
	// ����������һ������Ͷ��
	INT nSum = 0;

	for(INT n = 0; n < X_ERA_ATTA_NUM; n++)
	{
		if( 0 == m_nAttPointAdd[n] ) continue;

		ModBaseAttValue(MTransIndex2ERAATTA(n), -m_nAttPointAdd[n]);
		nSum += m_nAttPointAdd[n];
		m_nAttPointAdd[n] = 0;
	}
    
	//--------------------------------------------------------------------
	// ϴ���Ե� �ƺ��¼�����    
	// zxzhang 2009-12-29  
	//--------------------------------------------------------------------
	if ( P_VALID(GetTitleMgr()) )
	{
		GetTitleMgr()->SigEvent( ETE_CLEARPOINT ,GT_INVALID,GT_INVALID);
	}
    

	// ������������
	RecalAtt();

	// ���µ�ǰ��Ͷ�����Ե�
	ModAttValue(ERA_AttPoint, nSum);

	// ����ÿ������Ͷ������ǰֵ���ͻ���
	tagNS_RoleAttPointAddChange send;
	IFASTCODE->MemCpy(send.nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));
	SendMessage(&send, send.dwSize);

	// ���Ե�����ݿ�
	SaveAttPoint2DB();

	return E_Success;
}

//-------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------
VOID Role::OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited)
{
	ASSERT( P_VALID(pSkill) && P_VALID(pSrc) );

	// �жϼ��ܵĵ���Ŀ������
	if( !pSkill->IsHostile() && !pSkill->IsFriendly() ) return;

	DWORD dwFriendEnemyFlag = pSrc->FriendEnemy(this);

	// ����ü��ܿ��ԶԵз�ʹ�ã�����˫�������з���ϵ
	if( pSkill->IsHostile() && this != pSrc && (ETFE_Hostile & dwFriendEnemyFlag) )
	{
		// �������
		InterruptCondense();

		// ���������Ҳ����ң�������˫����PVP״̬������״̬
		if( pSrc->IsRole() )
		{
			Role* pSrcRole = static_cast<Role*>(pSrc);

			// ����˫����PVP״̬
			SetPVP();
			pSrcRole->SetPVP();

			// ����������������״̬
			ResetPK();

			// ���������������״̬������������ʱ��
			if( pSrcRole->IsInRoleState(ERS_PK) || pSrcRole->IsInRoleState(ERS_PKEX) )
			{
				pSrcRole->ResetPK();	
			}

			// �����µ�Ԥ�����ƣ�Jason 2010-7-17 v1.0.0
			//// �����Ҵ��ڷ�����״̬������ͻ��˷��;���
			//if ( !IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) )
			//{
			//	tagNS_RoleBeAttacked send;
			//	SendMessage(&send, send.dwSize);
			//}
			bool bSendWaring = false;
			
			DWORD dwAtDeRelationship = TestAttackDefenseRelationship(pSrcRole->GetID());
			if(  (dwAtDeRelationship & EAD_Defense) && !(dwAtDeRelationship & EAD_Attack)  ) // �ҷ����� �������Ҳ�����
			{
				//pSrcRole->AddAttackRelationship(GetID());
				bSendWaring = true;
			}
			else if( (dwAtDeRelationship & EAD_Attack) && !(dwAtDeRelationship & EAD_Defense) ) //  �ҹ�����������ʼ����
			{
				AddDefenseRelationship(pSrcRole->GetID());
				pSrcRole->AddAttackRelationship(GetID());
			}
			else if( dwAtDeRelationship == EAD_NULL  )	// û�й������ع�ϵ��
			{
				bSendWaring = true;
				AddDefenseRelationship(pSrcRole->GetID());
				pSrcRole->AddAttackRelationship(GetID());
			}
			if( bSendWaring )
			{
				tagNS_RoleBeAttacked send;
				send.dwEvilRoleID = pSrcRole->GetID();
				send.bWarning = TRUE;
				SendMessage(&send, send.dwSize);
			}
		}
		else if( pSrc->IsCreature() )
		{
			// ���﹥����ң�����һ����
			Creature* pSrcCreature = static_cast<Creature*>(pSrc);
			if( P_VALID(pSrcCreature->GetAI()) )
				pSrcCreature->GetAI()->AddEnmity(this, 1);
		}

		// ������ϵı������ܺ�װ����Buff����
		if( bHited )
		{
			OnPassiveSkillBuffTrigger(pSrc, ETEE_Hited, 0, 0);
			OnEquipmentBuffTrigger(NULL, ETEE_Hited);
			OnBuffTrigger(pSrc, ETEE_Hited, 0, 0);
			if( bBlock )
			{
				OnPassiveSkillBuffTrigger(pSrc, ETEE_Block, 0, 0);
				OnEquipmentBuffTrigger(NULL, ETEE_Block);
				OnBuffTrigger(pSrc, ETEE_Block, 0, 0);
			}
			if( bCrited )
			{
				OnPassiveSkillBuffTrigger(pSrc, ETEE_Crited, 0, 0);
				OnEquipmentBuffTrigger(NULL, ETEE_Crited);
				OnBuffTrigger(pSrc, ETEE_Crited, 0, 0);
			}

			// ��⵱ǰ���ܵĴ��
			GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Skill, ESSTE_Default == pSkill->GetTypeEx());

			// ���ĳЩ��������ϵ�buff
			OnInterruptBuffEvent(EBIF_BeAttacked);
		}
		else
		{
			OnPassiveSkillBuffTrigger(pSrc, ETEE_Dodge, 0, 0);
			OnEquipmentBuffTrigger(NULL, ETEE_Dodge);
			OnBuffTrigger(pSrc, ETEE_Dodge, 0, 0);
		}
		OnPassiveSkillBuffTrigger(pSrc, ETEE_BeAttacked, 0, 0);
		OnEquipmentBuffTrigger(NULL, ETEE_BeAttacked);
		OnBuffTrigger(pSrc, ETEE_BeAttacked, 0, 0);
	}

	// ����ü��ܿ��Զ��ѷ�ʹ�ã�����˫�������ѷ���ϵ
	if( pSkill->IsFriendly() && (ETFE_Friendly & dwFriendEnemyFlag) )
	{
		// ���ֵ
		INT	nValue = pSkill->GetEnmity() + pSrc->GetAttValue(ERA_Enmity_Degree);

		// ������ϱ�����������Ĺ���ȫ�����ӳ��
		DWORD dwCreatureID = GT_INVALID;
		Creature* pCreature = (Creature*)GT_INVALID;
		m_mapEnmityCreature.ResetIterator();
		while( m_mapEnmityCreature.PeekNext(dwCreatureID) )
		{
			pCreature = GetMap()->FindCreature(dwCreatureID);
			if( !P_VALID(pCreature) ) continue;

			if( P_VALID(pCreature->GetAI()) )
				pCreature->GetAI()->AddEnmity(pSrc, nValue);
		}
	}
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID Role::OnDead(Unit* pSrc, Skill* pSkill/*=NULL*/, const tagBuffProto* pBuff/*=NULL*/, DWORD dwSerial, DWORD dwMisc)
{
	// ��������Ѿ���������ֱ�ӷ���
	if( IsInState(ES_Dead) ) return;

	BOOL bKillEnemyGuildMember = FALSE;
	if (P_VALID(pSrc) && pSrc->IsRole())
	{
		Role* pRoleKilledMe = static_cast<Role*>(pSrc);
		if (g_guildWarDeclareMgr.IsInGuildWarState(this, pRoleKilledMe) == TRUE)
		{
			bKillEnemyGuildMember = TRUE;
		}
	}

	// ����ʱ����
	OnPassiveSkillBuffTrigger(pSrc, ETEE_Die);
	OnEquipmentBuffTrigger(pSrc, ETEE_Die);
	OnBuffTrigger(pSrc, ETEE_Die);

	// ����ʱȡ���ٻ�ʥ��״̬
	if (IsInRoleState(ERS_CALLHOLY))
	{
		UnsetRoleState(ERS_CALLHOLY, FALSE);	
		// ֪ͨ�ͻ��� �ٻ�ʥ����ʧ
		tagNS_CallShengLingDisappear msg;
		msg.dwPlayerID = GetID();
		Map* pMap = GetMap();
		if (P_VALID(pMap))
		{
			pMap->SendBigVisTileMsg(this, &msg, msg.dwSize);
		}
	}		

	// ����Ϊ����״̬
	SetState(ES_Dead);
	// ����pkģʽ����ʱΪ0
	m_nUnSetPKTickCountDown = 0;

	// ֹͣ�ƶ�
	m_MoveData.StopMoveForce();

	// ֹͣ��ǰ�����ͷŵļ���
	m_CombatHandler.End();

	// �Ƴ�����������������ʧ��buff
	OnInterruptBuffEvent(EBIF_Die);

	// ��ճ���б�
	ClearEnmityCreature();

	ClearTargetBy();

	// �������
	InterruptCondense();
    
	// ����ڷ���,T��
	g_AthleticsSystem.GetRoomMgr().OnOffLine(GetID());
	// �ű��ж��Ƿ����װ������ͷ�
	BOOL bEquipLootPenalty = IsDeadPenalty(EDPM_EquipLoot);

	// �Ƿ��о���ͷ�
	BOOL bExpPenalty = IsDeadPenalty(EDPM_Exp);

	// �Ƿ������˳ͷ�
	BOOL bInjuryPenalty = IsDeadPenalty(EDPM_Injury);

	// �Ƿ��������ͷ�
#if defined(Hostility_OEN)
	BOOL bHostilityPenalty = IsDeadPenalty(EDPM_Hostility);
#else
	BOOL bHostilityPenalty = FALSE;
#endif	

	// �Ƿ������������ͷ�
	BOOL bCommercePenalty = IsDeadPenalty(EDPM_Commerce);

	BOOL bEquip = FALSE;
	BOOL bExp = FALSE;
	BOOL bInjury = FALSE;
	BOOL bHostility = FALSE;
	BOOL bCommerce = FALSE;

	// �Ƿ�����������ͷ�
	CanCancelPenalty(pSrc, bEquip, bExp, bInjury, bHostility, bCommerce);
	
	if (bEquip)
		bEquipLootPenalty = FALSE;		// ���װ������ͷ�

	if (bExp)
		bExpPenalty = FALSE;			// �������ͷ�

	if (bInjury)
		bInjuryPenalty = FALSE;			// ������˳ͷ�

	if (bHostility)
		bHostilityPenalty = FALSE;		// ��������ͷ�

	if (bCommerce)
		bCommercePenalty = FALSE;		// ������������ͷ�


	// �����ͷ�
	if (bEquipLootPenalty && !bKillEnemyGuildMember)
		DeadPenalty(pSrc, EDPM_EquipLoot);

	if (bExpPenalty && !bKillEnemyGuildMember)
		DeadPenalty(pSrc, EDPM_Exp);

	if (bInjuryPenalty && !bKillEnemyGuildMember)
		DeadPenalty(pSrc, EDPM_Injury);

	if (bHostilityPenalty && !bKillEnemyGuildMember)
		DeadPenalty(pSrc, EDPM_Hostility);

	if (bCommercePenalty && !bKillEnemyGuildMember)
		DeadPenalty(pSrc, EDPM_Commerce);

	// ��ɱ��
	if( P_VALID(pSrc) )
	{
		pSrc->OnKill(this);
	}

	// �ű��¼�
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
	{
		DWORD dwSrcRoleID = 0;
		if (P_VALID(pSrc))
			dwSrcRoleID = pSrc->GetID();
		pRoleScript->OnChaseHolderDie(this, dwSrcRoleID);
	}

	// ���õ�ͼ�¼�
	if( P_VALID(GetMap()) )
	{
		GetMap()->OnRoleDie(this, pSrc);
	}

	//// ȡ������
	//UnsetRoleState(ERS_PK);
	//UnsetRoleState(ERS_PKEX);

	// ���ͽ�ɫ������Ϣ
	tagNS_RoleDead send;
	send.dwRoleID = GetID();
	send.dwSrcRoleID = (P_VALID(pSrc) ? pSrc->GetID() : GT_INVALID);
	send.eSuggestRevive = ERRT_None;
	if( P_VALID(pSkill) )
	{
		send.eCause = ERDC_Skill;
		send.dwMisc = pSkill->GetTypeID();
	}
	else if( P_VALID(pBuff) )
	{
		send.eCause = ERDC_Buff;
		send.dwMisc = pBuff->dwID;
	}
	else
	{
		send.eCause = ERDC_Other;
		send.dwMisc = GT_INVALID;
	}

	if (P_VALID(pSrc) && pSrc->IsRole() && ERPKS_Wanted == GetPKState())
	{
		//// ��Ҫ��������
		//m_bNeedPrisonRevive = TRUE;
		//// ��������
		//send.eSuggestRevive = ERRT_Prison;

		//tagNS_SendToPrisonEx sMsg;
		//sMsg.dwCaptorRoleID = send.dwSrcRoleID;
		//sMsg.dwCatchedRoleID = send.dwRoleID;
		//g_roleMgr.GetRoleNameByID( sMsg.dwCaptorRoleID,sMsg.strCaptorRoleName );
		//g_roleMgr.GetRoleNameByID( sMsg.dwCatchedRoleID,sMsg.strCatcherRoleName );
		//g_roleMgr.SendWorldMsg(&sMsg, sMsg.dwSize);
	}
	send.dwMisc2 = dwMisc;
	send.dwSerial = dwSerial;

	if( P_VALID(GetMap()) )
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}

	ModFabaoMood(-5);

	if (P_VALID(GetMap()) && P_VALID(GetWarRole()) && GetMap()->IsWar())
	{
		GetWarRole()->OnDead((Unit*)this, pSrc);
	}

	// ���ݿ����ж��Ƿ� ��Log��
	if (g_worldSession.IsKillRoleLogOn())
	{
		if (P_VALID(pSrc) && pSrc->IsRole())
		{
			tagNDBC_LogKillrole logKillRole;
			logKillRole.dwSrcRoleID = pSrc->GetID();
			logKillRole.dwTargetRoleID = GetID();
			g_dbSession.Send(&logKillRole,logKillRole.dwSize);
		}
	}
}

//---------------------------------------------------------------------------------
// ��ɱĿ��
// Jason 2010-6-22 ɱ¾״̬��ɱ������ң��Ż�������³ͷ������ǣ�֮ǰ��pvp��״̬����֪�����ʷ񣬴���ͨ
//---------------------------------------------------------------------------------
VOID Role::OnKill(Unit* pTarget)
{
	ASSERT( P_VALID(pTarget) );

	// ��ɱ�������
	if( pTarget->IsRole() )
	{
		// �����ɱ������ң���һ�ɱ����+1, ��ɱ������
		if (GetID() != pTarget->GetID())
		{
			KillCountInc(ERKT_Role);
		}

		// �����������ܺ�װ��
		OnPassiveSkillBuffTrigger(pTarget, ETEE_Kill);
		OnEquipmentBuffTrigger(pTarget, ETEE_Kill);
		OnBuffTrigger(pTarget, ETEE_Kill);

		Role* pRole = static_cast<Role*>(pTarget);
		ERolePKState ePKState = pRole->GetPKState();

		// ���ӳ��
		BOOL bAddKillerToEnemy = TRUE;

		BOOL bKillEnemyGuildMember = FALSE;
		if (g_guildWarDeclareMgr.IsInGuildWarState(this, pRole) == TRUE)
		{
			bKillEnemyGuildMember = TRUE;
		}
		
		// ����ɱ���Ƿǰ�����ң����ұ��κ���һ�λ�ɱ�Ĳ���ͬһ���ˣ��������صĽ���ʵ��ֵ(�����ɱ���˺�ɱ�˵�������������ս���ᣬ������ʵ��ֵ����)
		if( bKillEnemyGuildMember == FALSE &&
			ePKState != ERPKS_Peace &&
			m_dwLatestOne != pRole->GetID() &&
			m_nKillScore < 100 )
		{
			INT nSaveAdvanceStrength = GetScriptData(101);
			INT nAdvanceStrength = GetScriptData(101);

			if (pTarget->GetLevel() - m_nLevel > 3)
			{
				m_nKillScore += 4;
				if (m_nKillScore>=100)
				{
					nAdvanceStrength = nAdvanceStrength + 4 - (m_nKillScore-100);
					m_nKillScore = 100;
				}
				else
				{
					nAdvanceStrength += 4;
				}
			}
			else if(pTarget->GetLevel() - m_nLevel <= 3 && pTarget->GetLevel() - m_nLevel >= -3)
			{
				m_nKillScore += 3;
				if (m_nKillScore>=100)
				{
					nAdvanceStrength = nAdvanceStrength + 3 - (m_nKillScore-100);
					m_nKillScore = 100;
				}
				else
				{
					nAdvanceStrength += 3;
				}
			}
			else if(pTarget->GetLevel() - m_nLevel < -3 && pTarget->GetLevel() - m_nLevel >= -10)
			{
				m_nKillScore += 2;
				if (m_nKillScore>=100)
				{
					nAdvanceStrength = nAdvanceStrength + 2 - (m_nKillScore-100);
					m_nKillScore = 100;
				}
				else
				{
					nAdvanceStrength += 2;
				}
			}
			else
			{
				m_nKillScore += 1;
				if (m_nKillScore>=100)
				{
					nAdvanceStrength = nAdvanceStrength + 1 - (m_nKillScore-100);
					m_nKillScore = 100;
				}
				else
				{
					nAdvanceStrength += 1;
				}
			}

			SetScriptData(101, nAdvanceStrength);
			// �ı������ʵ��ֵ
			INT nChange = nAdvanceStrength - nSaveAdvanceStrength;
			INT nTmp = m_Strength.nConsumptiveStrength;
			SetConsumptiveStrength(nTmp+nChange);
			// ��¼�����ɱ�Ķ���
			m_dwLatestOne = pTarget->GetID();
		}

		// �Ƿ��е���ֵ�ͷ�(�����ɱ���˺�ɱ�˵�������������ս���ᣬû�е���ֵ�ͷ�)
		if (bKillEnemyGuildMember == FALSE && IsDeadPenalty(EDPM_Morality))
		{
			INT nMoralityDec = 0;

			// ��ɱ�κ���pvp��̨�����ս������������״̬����ң����ı����ֵ
#if defined(Hostility_OEN)			
			if(!pRole->IsInRoleState(ERS_Arena) && !pRole->IsInRoleState(ERS_Hostility) )
#else
			if(!pRole->IsInRoleState(ERS_Arena))
#endif
			{
				//// ��ɱ������״̬����ң��ı����ֵ-10, -5, -1
				//if (!pRole->IsInRoleState(ERS_PK) && !pRole->IsInRoleState(ERS_PKEX))
				//{
				//	if( ERPKS_Peace == ePKState )
				//		nMoralityDec =  -10;
				//	else if( ERPKS_Warnning == ePKState )
				//		nMoralityDec = -5;
				//	else if( ERPKS_Wicked == ePKState )
				//		nMoralityDec = -1;
				//}
				//
				//// ��ɱ����״̬����ң��ı����ֵ-3, -1, 0
				//else
				//{
				//	if( ERPKS_Peace == ePKState )
				//		nMoralityDec =  -3;
				//	else if( ERPKS_Warnning == ePKState )
				//		nMoralityDec = -1;
				//	else if( ERPKS_Wicked == ePKState )
				//		nMoralityDec = 0;
				//}
				if( (pRole->IsInRoleState(ERS_PeaceModel) && pRole->GetAttValue(ERA_Morality) >= PK_STATE_MORALITY[ERPKS_Peace]) && IsInRoleState(ERS_PK) )
				{
					//nMoralityDec = KILL_MORALITY_DEC[ERPKS_Peace];
					nMoralityDec = -g_world.GetDeductMorality();
				}
			}

			if( nMoralityDec != 0 )
			{
				ModAttValue(ERA_Morality, nMoralityDec);
				
// 				// ���ӳ��
// 				bAddKillerToEnemy = TRUE;
			}
		}
		// �����ͷ�ȥ��	
#if defined(Hostility_OEN)
		// �Ƿ�������ֵ�ͷ�(�����ɱ���˺�ɱ�˵�������������ս���ᣬû������ֵ�ͷ�)
		if (bKillEnemyGuildMember == FALSE && IsDeadPenalty(EDPM_Hostility))
		{
			// ��ɱ�κβ���pvp��̨�����ս�����ȷ�����״̬Ҳ������״̬����ң��ı�����
			if( !pRole->IsInRoleState(ERS_PK) && !pRole->IsInRoleState(ERS_PKEX) && !pRole->IsInRoleState(ERS_Arena) && 
				pRole->GetAttValue(ERA_Hostility)==0 )
			{
				INT nAdd = 0;

// 				// ���ӳ��
// 				bAddKillerToEnemy = TRUE;
// 
				if (pRole->GetPKState() == ERPKS_Peace)
				{
					nAdd = 3;		// �����ɱ������ң���������3
				}
				else
				{
					nAdd = 1;		// �����ɱ������������������ң���������1
				}

				// �޸�����ֵǰ�Ƚ�������ϵ�����ֵ��Ӧbuffȥ���������һ���µ�buff
				INT nSrcHostility = GetAttValue(ERA_Hostility);
				if (nSrcHostility > 0)
				{
					RemoveBuff(Buff::GetIDFromTypeID( HOSTILITY_DEADPENALTY_BUFF_ID[nSrcHostility - 1]), TRUE );
				}

				// �ı��ɱ������ֵ
				SetAttValue( ERA_Hostility, min( GetAttValue(ERA_Hostility) + nAdd, MAX_HOSTILITY ) );

				// ��ӳͷ�buff
				nSrcHostility = GetAttValue(ERA_Hostility);
				if (nSrcHostility > 0)
				{
					const tagBuffProto* pProto = g_attRes.GetBuffProto( HOSTILITY_DEADPENALTY_BUFF_ID[nSrcHostility - 1] );
					if( P_VALID(pProto) )
					{
						TryAddBuff(this, pProto, NULL, NULL, NULL);
					}
				}
				
				
				if ( !this->IsInRoleState(ERS_Hostility) )
				{
					// ��ҳ�Ϊ����״̬
					SetRoleState(ERS_Hostility);
				}
			}

			// ����ɱ�����ȥ����Ӧ����ֵ��buff
			INT nTargetHostility = pRole->GetAttValue(ERA_Hostility);
			if (nTargetHostility > 0)
			{
				pRole->RemoveBuff(Buff::GetIDFromTypeID( HOSTILITY_DEADPENALTY_BUFF_ID[nTargetHostility - 1]), TRUE );
			}

			// ����ɱ��ң�����ֵ��1
			pRole->SetAttValue( ERA_Hostility, max( 0, pRole->GetAttValue(ERA_Hostility) - 1 ) );

			// ��ӳͷ�buff
			nTargetHostility = pRole->GetAttValue(ERA_Hostility);
			if (nTargetHostility > 0)
			{
				const tagBuffProto* pProto = g_attRes.GetBuffProto( HOSTILITY_DEADPENALTY_BUFF_ID[nTargetHostility - 1] );
				if( P_VALID(pProto) )
				{
					pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
				}
			}
			

			// �������ɱ�������ֵΪ0���Ҵ�������״̬����ȡ������ɱ�������״̬
			if ( pRole->GetAttValue(ERA_Hostility) == 0 && pRole->IsInRoleState(ERS_Hostility) )
			{
				pRole->UnsetRoleState(ERS_Hostility);
			}
		}
#endif

		// ���ӳ��
		if (bKillEnemyGuildMember == TRUE || // ����ս��ɱ
			(pRole->IsInState(ES_CSAttack) && IsInState(ES_CSDefence)) ||	// ս����ɱ
			(pRole->IsInState(ES_CSDefence) && IsInState(ES_CSAttack)) ||   // ս����ɱ
			(pRole->IsInRoleState(ERS_PKEX) && IsInRoleState(ERS_PKEX))||   // ���˶��ǶԿ�ģʽ
			(pRole->GetMap()->GetMapID() == TObjRef<Util>()->Crc32(_T("s59"))) )		//��ѡ��֮ս
		{
			bAddKillerToEnemy = FALSE;
		}
		if ((bAddKillerToEnemy == TRUE) && P_VALID(g_ScriptMgr.GetRoleScript()))
		{
			DWORD dwResult = g_ScriptMgr.GetRoleScript()->CanAddEnemy(pRole, GetID());
			if (dwResult != GT_INVALID)
			{
				bAddKillerToEnemy = (dwResult == 0)?FALSE:TRUE;
			}
		}
		if (bAddKillerToEnemy == TRUE)
		{
			g_socialMgr.InsertEnemy(pRole, GetID());
		}

		// �㲥��ɱͨ�������Ϣ(�����ɱ���˺�ɱ�˵�������������ս���ᣬ����Ҫ�㲥)
		if (bKillEnemyGuildMember == FALSE && (ERPKS_Wanted == ePKState))
		{
			tagNS_SendToPrison send;
			send.dwCaptorRoleID = GetID();
			send.dwCatchedRoleID = pRole->GetID();
			
			g_roleMgr.SendWorldMsg(&send, send.dwSize);
		}

		// �ƺ��¼�����(�����ɱ���˺�ɱ�˵�������������ս���ᣬ����Ҫ�����ƺ��¼�)
		if (bKillEnemyGuildMember == FALSE)
		{
			GetTitleMgr()->SigEvent(ETE_KILL_ROLE, GT_INVALID, GT_INVALID);
		}

		// Jason 2010-7-17 v1.0.0 pkԤ����Ϣ����
		//DWORD dwAtDeRelationship = TestAttackDefenseRelationship(pRole->GetID());
		pRole->ClearAttackDefenseRelationship();
		//RemoveAttackDefenseRelationship(pRole->GetID());
		//if( dwAtDeRelationship & EAD_Attack ) // ���Ҵ��
		//{
		//}
		//else if(  dwAtDeRelationship & EAD_Defense  ) // ���ҵ�
		//{
		//}

		//ϡ��������صĸı�
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if( P_VALID(pRoleScript) && pRoleScript->IsCanRobRareArea(pRole) && m_dwClergy > 0)
		{
			//��ɱ�ߵ�ǰ����/500+����ɱ�ߵ�ǰ����/300��1.5������ɱ�ߵ�ǰ����/300��1.2��+5%�����ȡ95%�����ȡ25%
			int nRet = ( (float)( m_nAtt[ERA_Morality] / 500 ) + (float)( m_nAtt[ERA_Morality] / 300 * 1.5 * pRole->m_nAtt[ERA_Morality]/300 * 1.2 + 0.05 ) ) * 100;
			
			if(nRet > 95)
				nRet = 95;

			if(nRet < 25)
				nRet = 25;

			if( pRole->GetAbilityAreaNum(EGAT_RARE) > 0 && IUTIL->RandomInRange(1,100) <= nRet)
			{
				DWORD dwAreaID = pRole->GetRandRareArea();
				const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
				if(!IsHaveArea(dwAreaID) && P_VALID(pGodAreaProto) && pGodAreaProto->bContest )
				{
					//���ֹͣһ��ϡ����������
					if(pRole->DirectDelOneRareArea(dwAreaID) && dwAreaID > 0)
					{
						tagNS_AddArea send;
						send.dwAreaID = dwAreaID;
						send.dwErrorCode = E_Success;
						send.bAdd = FALSE;
						pRole->SendMessage(&send, send.dwSize);

						if(E_Success == send.dwErrorCode)
							g_dbSession.SendDeleteArea2DB(pRole->GetID(),dwAreaID);

						//����ɱ���ߵ�ϡ����������
						tagGlobalLimitedItem *pGlobalLimitedItem = g_GlobalLimitedItemMgr.GetLimitedItemFromMapByAreaID( dwAreaID, pRole->GetID() );
						if(P_VALID(pGlobalLimitedItem))
						{
							tagNS_AddArea send1;
							send1.dwAreaID = dwAreaID;
							//tagGlobalLimitedItem *pGlobalLimitedItem = g_GlobalLimitedItemMgr.GetLimitedItemFromMapByAreaID( dwAreaID, pRole->GetID() );
							send1.dwErrorCode = ChangeArea(dwAreaID,TRUE,pGlobalLimitedItem,FALSE);
							send1.bAdd = TRUE;
							send1.dwTime = pGlobalLimitedItem->nTime;
							if(E_Success == send1.dwErrorCode)
							{
								SendMessage(&send1, send1.dwSize);
								g_dbSession.SendCreateArea2DB(GetID(),dwAreaID,FALSE);

								g_GlobalLimitedItemMgr.SendUpdateRareArea2DB(pGlobalLimitedItem,pRole->GetID());
							}

							tagNS_RareAreaInfo send2;
							send2.bSuccess = TRUE;
							send2.dwAreaID = dwAreaID;
							send2.dwRoleID = pRole->GetID();
							send2.dwRobRoleID = GetID();
							ZeroMemory( send2.szRoalName, X_SHORT_NAME );

							tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(this->GetID());
							if (P_VALID(pRoleInfo))
							{
								_tcscpy_s( send2.szRoalName, X_SHORT_NAME, pRoleInfo->szRoleName);
							}
							pRole->SendMessage(&send2,send2.dwSize);
							SendMessage(&send2,send2.dwSize);

							if( P_VALID(m_pScript) )
								m_pScript->OnRoleWinRareArea(pRole,GetID(),dwAreaID);
						}
					}
					else
					{//���������������ʧ��
						tagNS_RareAreaInfo send;
						send.bSuccess = FALSE;
						SendMessage(&send,send.dwSize);
					}
				}
				else
				{//�������Ѿ������������
					tagNS_RareAreaInfo send;
					send.bSuccess = FALSE;
					SendMessage(&send,send.dwSize);
				}
			}
			else
			{//��������û��ϡ��������߼���ʧ��
				tagNS_RareAreaInfo send;
				send.bSuccess = FALSE;
				SendMessage(&send,send.dwSize);
			}
		}
		else
		{
			//DWORD dwMsgInfoID =	g_MsgInfoMgr.BeginMsgEvent();
			////��ϡ����������ʧ�ܣ���
			//DWORD dwMsgID = 0; //TODO liu
			//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_BigBroadcast, &dwMsgID);
			//g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, this);

			//dwMsgInfoID =	g_MsgInfoMgr.BeginMsgEvent();
			//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_BigBroadcast, &dwMsgID);
			//g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, pRole);
		}
	}

	// �����ɱ���ǹ���
	else if( pTarget->IsCreature() )
	{
		Creature* pCreature = dynamic_cast<Creature*>(pTarget);
		if (!P_VALID(pCreature))
			return;

		// �����������ܺ�װ��
		if( !pCreature->IsGameObject() )
		{
			OnPassiveSkillBuffTrigger(pTarget, ETEE_Kill);
			OnEquipmentBuffTrigger(pTarget, ETEE_Kill);
			OnBuffTrigger(pTarget, ETEE_Kill);
		}



		// �ƺ����
		if (pCreature->IsNPC())
		{
			GetTitleMgr()->SigEvent(ETE_KILL_NPC, pCreature->GetTypeID(), GT_INVALID);
		}
		else if (pCreature->IsBoss())
		{
			// ����ǹ�������ɱ����+1
			KillCountInc(ERKT_Monster);

			GetTitleMgr()->SigEvent(ETE_KILL_BOSS, pCreature->GetTypeID(), GT_INVALID);

			if(P_VALID(m_pScript))
				m_pScript->OnKillMonster(this,pCreature);
		}
		else if (pCreature->IsMonster())
		{
			// ����ǹ�������ɱ����+1
			KillCountInc(ERKT_Monster);

			if(GetTeamID() ==GT_INVALID)
			{
				GetTitleMgr()->SigEvent(ETE_KILL_MONSTER, pCreature->GetTypeID(), GT_INVALID);
			}
			else
			{
				const Team *pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
				std::vector<Role*>	vecRole;
				INT   nShareRole = pCreature->CalExpShareRole(pTeam, vecRole);

				std::vector<Role*>::iterator it = vecRole.begin();
				while (it != vecRole.end())
				{
					Role* pMember = *it;
					++it;

					if( !P_VALID(pMember) )
						continue;
                     pMember->GetTitleMgr()->SigEvent(ETE_KILL_MONSTER, pCreature->GetTypeID(), GT_INVALID);
				}
			} 

			if(P_VALID(m_pScript))
				m_pScript->OnKillMonster(this,pCreature);
		}
	}

	if (P_VALID(GetMap()) && P_VALID(GetWarRole()) && GetMap()->IsWar())
	{
		GetWarRole()->OnKill((Unit*)this, pTarget);
	}
}

//---------------------------------------------------------------------------------
// �Ƿ���������ͷ�
//---------------------------------------------------------------------------------
BOOL Role::IsDeadPenalty(EDeadPenaltyMode eMode)
{
	if(P_VALID(m_pScript))
		return m_pScript->IsDeadPenalty(this, eMode);

	return TRUE;
}

//---------------------------------------------------------------------------------
// �����ͷ�
// Jason 2010-6-22 ���³ͷ�֮װ�����䣬��������С
//---------------------------------------------------------------------------------
VOID Role::DeadPenalty(Unit* pSrc, EDeadPenaltyMode eMode)
{
	// �������Ǽ���С�������ȼ��ͷ��ĵȼ����ޣ������κ������ͷ�
	if( GetLevel() < DEAD_PENALTY_LEVEL ) return;

	// ���������PVP���������򲻻��ܵ��κ������ͷ�
	if( IsInRoleState(ERS_PVPArea) ) return;

	switch(eMode)
	{
	case EDPM_EquipLoot:
		{
			// װ������ͷ���PK������Ҳ��ܳͷ���������һ�ɱҲ����װ������ĳͷ�
			if( !IsInRoleState(ERS_Safeguard) && P_VALID(pSrc) && pSrc->IsRole() && GetAttValue(ERA_Morality) < PK_STATE_MORALITY[ERPKS_Warnning] )
			{
				INT nLootProp = LOOT_RATE_PER_PK_STATE[m_ePKState];
				if( nLootProp > 0 &&  IUTIL->Rand() % 10000 < nLootProp )
				{
					// 80%�ļ��ʵ���1��װ�������
					// 15%�ļ��ʵ���2��װ�������
					// 4%�ļ��ʵ���3��װ�������
					// 0.8%�ļ��ʵ���4��װ�������
					// 0.2%�ļ��ʵ���5��װ�������
					INT nNum = 1;
					//INT nProp = IUTIL->Rand() % 10000;

					//if( nProp < 8000 )			nNum = 1;
					//else if( nProp < 9500 )		nNum = 2;
					//else if( nProp < 9900 )		nNum = 3;
					//else if( nProp < 9980 )		nNum = 4;
					//else						nNum = 5;

					// ���ݸ����ӱ�������װ�����е�����Ʒ
					TList<tagItem*> listLootItems;
					TList<tagEquip*> listLootEquips;

					// �㲥������Ʒ
					TList<DWORD> listItemsBroadcast;

					// �漴ȷ��ɾ����װ������Ʒ����
					INT nEquip = nNum;//IUTIL->Rand() % (nNum + 1);
					INT nItem = 0;//;nNum - nEquip;
					INT nlooted = 0;

					// ����װ��
					GetItemMgr().LootFromEquipBar(nEquip, listLootEquips, ELCID_Dead_Penalty, listItemsBroadcast);

					// ���ɵ���װ��
					tagEquip* pEquip = NULL;
					TList<tagEquip*>::TListIterator iterEquip = listLootEquips.Begin();
					while (listLootEquips.PeekNext(iterEquip, pEquip))
					{
						if (!P_VALID(pEquip))
						{
							continue;
						}

						if( GetMap() )
						{
							tagGroundItem* pGroundItem = new tagGroundItem(GetMap()->GenGroundItemID(), pEquip->dwTypeID, 
								pEquip->GetNum(), pEquip, GetCurPos(), GT_INVALID, 
								GT_INVALID, 0, GetID());

							ASSERT(pGroundItem->IsValid());

							GetMap()->AddGroundItem(pGroundItem);
						}
						else
						{
							Destroy(pEquip);
						}
						++nlooted;
					}

					// ������Ʒ
					nItem = nNum - nlooted;
					if( nItem > 0 )
						GetItemMgr().LootFromBag(nItem, listLootItems, ELCID_Dead_Penalty, listItemsBroadcast);

					// ���ɵ�����Ʒ
					tagItem* pItem = NULL;
					TList<tagItem*>::TListIterator iterItem = listLootItems.Begin();
					while (listLootItems.PeekNext(iterItem, pItem))
					{
						if (!P_VALID(pItem))
						{
							continue;
						}

						if( GetMap() )
						{
							tagGroundItem* pGroundItem = new tagGroundItem(GetMap()->GenGroundItemID(), pItem->dwTypeID, 
								pItem->GetNum(), pItem, GetCurPos(), GT_INVALID, 
								GT_INVALID, 0, GetID());

							ASSERT(pGroundItem->IsValid());

							GetMap()->AddGroundItem(pGroundItem);
						}
						else
						{
							Destroy(pItem);
						}
					}
					
					// �㲥������Ʒ
					listItemsBroadcast.ResetIterator();
					DWORD dwTypeID = GT_INVALID;
					while (listItemsBroadcast.PeekNext(dwTypeID))
					{
						// ������Ƶ������ʾ
						DWORD dwMsgInfoID =	g_MsgInfoMgr.BeginMsgEvent();
						DWORD dwData = 1;
						g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ChatInfo, &dwData);
						dwData = DEAD_LOOT_MSG_ID;
						g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_MsgID, &dwData);
						g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ItemTypeID, &dwTypeID);
						g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, this);
					}

				}
			}
		}
		break;

	case EDPM_Exp:
		{
#if defined(ExpPenalty_OPEN)
			// ����ͷ���ֻ������״̬����ҲŻ���ʧ���飩
			if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) )
			{
				// ֻ�б���һ�ɱ������²Ż���ʧ����
				if( P_VALID(pSrc) && pSrc->IsRole() )
				{
					// ��ʧ�ľ��飺[150/(��ɫ�ȼ�^2)] %����������
					const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(GetLevel());
					ASSERT( P_VALID(pEffect) );
					INT level = GetLevel();
					INT nExp = FLOAT(pEffect->nExpLevelUp / level) / level * 150 ;
					// Jason 2010-2-22 v1.3.2 ����ͷ����Ե���ϵ��
					INT nMorality = 1;
					INT MoralityValue = GetAttValue(ERA_Morality);
					if( -29 <= MoralityValue )
						nMorality = 0;
					else if( -99 <= MoralityValue )
						nMorality = 1;
					else if( -998 <= MoralityValue )
						nMorality = 2;
					else
						nMorality = 3;
					nExp *= nMorality;
					ExpChange(-nExp);
				}
			}
#endif
		}

		break;

	case EDPM_Injury:
		{
			// ���˳ͷ�
			// 29%���ʲ�������
			// 30%��������+1
			// 20%��������+2
			// 15%��������+3
			// 5%��������+5
			// 1%��������+10
			INT nInjury = 0;
			INT nProp = IUTIL->Rand() % 100;

			// �����ɫvip�ȼ������˸��ʵ�Ӱ��
			nProp = ((100 - GetInjuryRateDerate())*nProp)/100;

			if( nProp < 29 )			nInjury = 0;
			else if( nProp < 59 )		nInjury = 1;
			else if( nProp < 79 )		nInjury = 2;
			else if( nProp < 94 )		nInjury = 3;
			else if( nProp < 99 )		nInjury = 5;
			else						nInjury = 10;

			if( nInjury != 0 )
				ModAttValue(ERA_Injury, nInjury);

		}
		break;

	case EDPM_Hostility:
		{
			// ����״̬���������õ����Գͷ�
			//if (IsInRoleState(ERS_Hostility))
			//{
			//	INT ndx = GetAttValue(ERA_Hostility);
			//	if( ndx < 0  )
			//		ndx = 0;
			//	else if( ndx > 9 )
			//		ndx = 9;
			//	if ( 0 == ndx )
			//		break;

			//	const tagBuffProto* pProto = g_attRes.GetBuffProto(HOSTILITY_DEADPENALTY_BUFF_ID[ndx - 1]);
			//	if( P_VALID(pProto) )
			//	{
			//		TryAddBuff(this, pProto, NULL, NULL, NULL);
			//	}
			//}
		}
		break;

	case EDPM_Commerce:
		{
			// ���������ͷ�
			Guild* pGuild = g_guildMgr.GetGuild(GetGuildID());
			if (!P_VALID(pGuild))				return;
			if (!IsInRoleState(ERS_Commerce))	return;
			GuildCommodity* pCommodity = pGuild->GetGuildCommerce().GetCommodity(GetID());
			if (!P_VALID(pCommodity))			return;
			pCommodity->DeadPenalty();
		}
		break;

	default:
		break;
	}

	
	
}

//---------------------------------------------------------------------------------
// �Ƿ�����������ͷ�
//---------------------------------------------------------------------------------
VOID Role::CanCancelPenalty(Unit* pSrc, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommercePenalty)
{
	if( !P_VALID(pSrc) ) return;
	if (pSrc->IsRole())
	{
		if(P_VALID(m_pScript))
		{
			m_pScript->CanCancelPenalty(this, pSrc, bEquip, bExp, bInjury, bHostility, bCommercePenalty);
		}
	}
}

//---------------------------------------------------------------------------------
// Ŀ������ͱ�־
//---------------------------------------------------------------------------------
DWORD Role::TargetTypeFlag(Unit* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	if( pTarget->IsRole() )
		return TargetTypeFlag(static_cast<Role*>(pTarget));
	else if( pTarget->IsCreature() )
		return TargetTypeFlag(static_cast<Creature*>(pTarget));
	else
		return 0;
}

DWORD Role::TargetTypeFlag(Role* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	// ���Ŀ������Լ����򷵻�����
	if( this == pTarget ) return ETF_Self;

	DWORD dwFlag = 0;

	// todo�������Ҹ��Լ��ǲ���ĳЩ����ϵ������ǣ�����������ص�ĳЩ�ֶ�

	// С�Ӷ���
	if( IsTeamMate(pTarget) )
	{	
		// �鿴�Ƿ�Ϊ�����񼣵Ĺ��������
		//if (! CGodMiracleMgr::GetInst()->IsCaptureGodAndDifferentGuild(this, (Role*)pTarget));
			dwFlag |= ETF_Teammate;
	}

	// ��Ȼ����ĳЩ����ϵ����Ҳ�������
	dwFlag |= ETF_Player;

	return dwFlag;
}

DWORD Role::TargetTypeFlag(Creature* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;

	DWORD dwFlag = 0;

	// ����
	if( pTarget->IsMonster() )
	{
		// ��ͨ����
		if( pTarget->IsNormalMonster() )
		{
			if( !pTarget->IsBoss() )
				dwFlag |= ETF_NormalMonster;
			else
				dwFlag |= ETF_Boss;
		}

		// ����	��͸�����ӹ���  �ɱ�������
		if (pTarget->IsDoorMonster())
		{
			dwFlag |= ETF_NormalMonster;
		}

		// ��Ѩ
		else if( pTarget->IsNest())
		{
			dwFlag |= ETF_Nest;
		}
	}

	// NPC
	else if( pTarget->IsNPC() )
	{
		dwFlag |= ETF_NPC;
	}

	// ����
	else if( pTarget->IsPet() )
	{
		dwFlag |= ETF_Pet;
	}

	// ��Դ
	else if( pTarget->IsRes() )
	{
		if ( pTarget->IsManRes() )
			dwFlag |= ETF_ManRes;
		else if ( pTarget->IsNatuRes() )
			dwFlag |= ETF_NatuRes;
		else
			ASSERT(0);
	}

	// �ɵ������
	else if( pTarget->IsInves() )
	{
		dwFlag |= ETF_InvesGameObject;
	}
	// ��
	else if (pTarget->IsDoor())
	{
		dwFlag |= ETF_Door;
	}
	// ��
	else if ( pTarget->IsRemnant())
	{
		dwFlag |= ETF_Remnant;
	}
	else if( pTarget->IsFairySoul() )
		dwFlag |= ETF_FairySoul;
	else if( pTarget->IsFairyHeritage() )
		dwFlag |= ETF_FairyHeritage;
	
	return dwFlag;
}

//---------------------------------------------------------------------------------
// ��Ŀ��ĵ��������ж�
//---------------------------------------------------------------------------------
DWORD Role::FriendEnemy(Unit* pTarget)
{
	if( !P_VALID(pTarget) ) return 0;
	if( !P_VALID(GetMap()) || GetMap() != pTarget->GetMap() ) return 0;

	// ����Ļ����������
	if( this == pTarget )
	{
		return ETFE_Friendly | ETFE_Hostile | ETFE_Independent;
	}

	// ȡ���ڵ�ͼ�ĵ��ҹ�ϵ�ж�
	BOOL bIgnore = FALSE;
	DWORD dwMapFlag = GetMap()->GetFriendEnemyFlag(this, pTarget, bIgnore);

	// �������Ҫ�жϵ�������ģ����ж�����
	if( !bIgnore )
	{
		DWORD dwSelfFlag = 0;
		if (IsInCamp() && pTarget->IsInCamp())
		{
			dwSelfFlag = FriendEnemyOfCamp(pTarget);
		}
		else
		{
			if( pTarget->IsRole() )
				dwSelfFlag = FriendEnemy(static_cast<Role*>(pTarget));
			else if( pTarget->IsCreature() )
				dwSelfFlag = FriendEnemy(static_cast<Creature*>(pTarget));
			else
				dwSelfFlag = 0;
		}

		return (dwMapFlag | dwSelfFlag);
	}
	else
	{
		return dwMapFlag;
	}
}

// �����ж��ж���

// ��ȫ
#define STA_NDX_Safe	0
// ����
#define STA_NDX_PK_Lianji	1
// �Կ�
#define STA_NDX_PK_Duikang	2
// ɱ¾
#define STA_NDX_PK_Shalu	3

static bool sta_fri_ene[4][4] =
{
	// ��ȫ		����	�Կ�	ɱ¾
	// ��ȫģʽ�£������˶����ܹ���������
/*��ȫ*/0,		0,		0,		0,
	// 
/*����*/0,		0,		0,		1,
// 
/*�Կ�*/0,		0,		1,		1,
// 
/*ɱ¾*/0,		1,		1,		1
};

static int Jm_GetStaNdx(DWORD rs)
{
	if( rs & ERS_Safeguard )
		return STA_NDX_Safe;
	else if( rs & ERS_PeaceModel )
		return STA_NDX_PK_Lianji;
	else if( rs & ERS_PKEX )
		return STA_NDX_PK_Duikang;
	else if( rs & ERS_PK )
		return STA_NDX_PK_Shalu;
	//else if( rs & ERS_Safeguard )
	//	return STA_NDX_Safe;
	return -1;
}

static DWORD Jm_GetFriEnemy(Role * src,Role * des)
{
	int ndx_src = Jm_GetStaNdx(src->GetRoleState());
	int ndx_des = Jm_GetStaNdx(des->GetRoleState());
	if( ndx_src < 0 || ndx_des < 0 )
		return ETFE_Independent;
	if( sta_fri_ene[ndx_src][ndx_des] )
		return ETFE_Hostile;
	return ETFE_Independent;
}

#undef STA_NDX_Safe
#undef STA_NDX_PK_Lianji
#undef STA_NDX_PK_Duikang
#undef STA_NDX_PK_Shalu

BOOL Role::IsInBattleField(Role* pSrcRole, Role* pTargetRole)
{
	if (P_VALID(pSrcRole) && P_VALID(pTargetRole))
	{
		DWORD dwSrcRoleMapID = pSrcRole->GetMapID();
		DWORD dwTargetRoleMapID = pTargetRole->GetMapID();
		if (dwSrcRoleMapID != dwTargetRoleMapID)
		{
			return FALSE;
		}
		if (IsGuildBattleMap(dwSrcRoleMapID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

DWORD Role::FriendEnemy(Role* pTarget)
{
	DWORD dwFlag = 0;

	// �ж�������ɫ�ǲ��Ƕ���ս��
	if (P_VALID(CGodMiracleMgr::GetInst()) &&
		CGodMiracleMgr::GetInst()->IsGodMiracleTime()&& 
		IsInBattleField(this, pTarget))
	{
		// �ж����������ǲ��ǵжԹ�ϵ
		if (CGodMiracleMgr::GetInst()->IsGuildInSameAlliance(GetMapID(), GetGuildID(), pTarget->GetGuildID()))
			dwFlag |= ETFE_Friendly;
		else
			dwFlag |= ETFE_Hostile;
	}
	else
	{
		// ����ս��״̬��������ģʽ�ĵ��ҹ�ϵ�ж�
		if (g_guildWarDeclareMgr.IsInGuildWarState(this, pTarget))
		{
			dwFlag |= ETFE_Hostile;
		}
		else
		{
			if (IsInRoleState(ERS_HoldChest) || pTarget->IsInRoleState(ERS_HoldChest))
			{
				dwFlag |= ETFE_Hostile;
			}
			else
			{
				// ��ͨ��Դ����Ŀ�귽֮��Ĺ�ϵȷ�����ҹ�ϵ
				if( IsFriendlySocial(pTarget) )
				{
					// Jason 2010-6-22 ֻҪ������ϵ���������ѣ���ʵ�����Ҳ��(��ע�Ͳ���)������Ч�ʾ͵ͺܶ�
					//if( !pTarget->IsInRoleState(ERS_PK) && !this->IsInRoleState(ERS_PK) )
					//{
					//	// ˫������ĳЩ����ϵ������˫��������������״̬�����ѷ�
					//	dwFlag |= ETFE_Friendly;
					//}
					//else if( pTarget->IsInRoleState(ERS_PK) && this->IsInRoleState(ERS_PK) )
					//{
					//	// ˫������ĳЩ����ϵ������˫������������״̬�����ѷ�
					//	dwFlag |= ETFE_Friendly;
					//}
					//else
					//{
					//	// ˫������ĳЩ����ϵ������ֻ����һ����������״̬�����ѷ�
					//	dwFlag |= ETFE_Friendly;
					//}
					if( InSamePKState(pTarget) || InSamePKStateEx(pTarget))
						dwFlag |= ETFE_Friendly;
				}
				else
				{
					// ����Լ����ó�Ϊ����״̬����Ϊ�з�
					//if( IsInRoleState(ERS_PK) )
					//{
					//	dwFlag |= ETFE_Hostile;
					//}

					//// ����Լ�Ϊ����״̬��ֻ��������ң������ҶԷ�ҲΪ����״̬����Ϊ�з�
					//else if (IsInRoleState(ERS_PKEX) && (pTarget->IsInRoleState(ERS_PK) || pTarget->IsInRoleState(ERS_PKEX)) )
					//{
					//	dwFlag |= ETFE_Hostile;
					//}

					//else
					//{
					//	// ����Ϊ����
					//	dwFlag |= ETFE_Independent;
					//}

					//// ����Լ����ڷ�����״̬����Ϊ�з�
					//if (!IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX))
					//{
					//	if ( dwFlag & ETFE_Hostile )
					//	{
					//		dwFlag ^= ETFE_Hostile;
					//	}
					//}

					dwFlag |= Jm_GetFriEnemy(this,pTarget);

				}
			}
		}
	}

	// �ٿ���һ��PK������ص�����
	if( pTarget->IsInRoleState(ERS_Safeguard) )
	{
		// �Կ���PK��������Ҳ��ܽ���PK
		if( dwFlag & ETFE_Hostile )
		{
			dwFlag ^= ETFE_Hostile;
		}
	}
	if( IsInRoleState(ERS_Safeguard) )
	{
		// �Լ�����PK���������ܶ����˽���PK
		if( dwFlag & ETFE_Hostile )
		{
			dwFlag ^= ETFE_Hostile;
		}

		// ���ܶԵ�ǰ����PVP״̬�µ��ѷ���ҽ���������ʽ
		if( (dwFlag & ETFE_Friendly) && pTarget->IsInRoleState(ERS_PVP) )
		{
			dwFlag ^= ETFE_Friendly;
		}
	}

	// ��ȫ��
	if( IsInRoleState(ERS_SafeArea) || pTarget->IsInRoleState(ERS_SafeArea) )
	{
		if( dwFlag & ETFE_Hostile )
		{
			dwFlag ^= ETFE_Hostile;
		}
	}

	// PVP����
	if( IsInRoleState(ERS_PVPArea) && pTarget->IsInRoleState(ERS_PVPArea) )
	{
		dwFlag |= ETFE_Hostile;
	}

	return dwFlag;
}

DWORD Role::FriendEnemy(Creature* pCreature)
{
	DWORD dwFlag = 0;

	// ����
	if( pCreature->IsMonster() )
	{
		dwFlag |= ETFE_Hostile;
	}

	// NPC
	else if( pCreature->IsNPC() )
	{
		//if( IsInRoleState(ERS_PK) || IsInRoleState(ERS_PKEX) )
		//{
		//	dwFlag |= ETFE_Hostile;
		//}
		//else
		//{
			dwFlag |= ETFE_Friendly;
		//}
	}

	// ����
	else if( pCreature->IsGameObject() )
	{
		if( pCreature->IsRes() 
			|| pCreature->IsInves())
		{
			dwFlag |= ETFE_Independent;
		}
		else if (pCreature->IsDoor())
		{
			dwFlag |= ETFE_Hostile;
		}
		else if ( pCreature->IsRemnant())
		{
			dwFlag |= ETFE_Hostile;
		}
		else if( pCreature->IsFairySoul() )
		{
			dwFlag |= ETFE_Friendly;
		}
		else if( pCreature->IsFairyHeritage() )
			dwFlag |= ETFE_Independent;
		else
		{
			// todo:ʣ�µ�һЩ����Ŀǰ�Ȳ����ǣ������޷������͵�
		}
	}

	// ����
	else if( pCreature->IsPet() )
	{

	}

	return dwFlag;
}

//------------------------------------------------------------------------------
// ע�ᴥ�������ܹ�����
//------------------------------------------------------------------------------
VOID Role::RegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID)
{
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return;

	std::set<DWORD>& skillSet = m_setPassiveSkillTrigger[eTriggerType];

	skillSet.insert(dwSkillID);
}

//----------------------------------------------------------------------------
// ��ע�ᴥ�������ܹ�����
//----------------------------------------------------------------------------
VOID Role::UnRegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID)
{
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return;

	std::set<DWORD>& skillSet = m_setPassiveSkillTrigger[eTriggerType];

	skillSet.erase(dwSkillID);
}

//----------------------------------------------------------------------------------
// װ�����buffԤ��������ȡtigger����
//----------------------------------------------------------------------------------
EPassiveSkillAndEquipTrigger Role::PreRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, BOOL bEquip)
{
	const tagTriggerProto* pProto = g_attRes.GetTriggerProto(dwTriggerID);
	if( !P_VALID(pProto) )
	{
		MAttResCaution(_T("trigger proto"), _T("TriggerID"), dwTriggerID);
		return EPSAET_Null;
	}

	// �Ƿ���װ����trigger
	if( ETEE_Equip == pProto->eEventType )
	{
		ProcEquipBuffTrigger(dwBuffID, bEquip);
		return EPSAET_Null;
	}

	return TriggerTypeToPassiveSkillAndEquipTriggerType(pProto->eEventType);
}

//----------------------------------------------------------------------------
// �������ܴ���������
//----------------------------------------------------------------------------
BOOL Role::OnPassiveSkillBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, 
		DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	// ���ȸ����¼������жϳ��������ܶ�Ӧ�Ĵ���������
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return FALSE;


	// �ҵ��Ƿ��б������ܶԸ�trigger���ͽ�����ע��
	if( m_setPassiveSkillTrigger[eTriggerType].empty() ) return FALSE;

	// Jason 
	INT nTargetCheckRet = 0,nSkillCheckRet = 0;

	// һ������һ�����ܵ���ѯ
	std::set<DWORD>& skillSet = m_setPassiveSkillTrigger[eTriggerType];
	for(std::set<DWORD>::iterator it = skillSet.begin(); it != skillSet.end(); it++)
	{
		// �ҵ�����
		DWORD dwSkillID = *it;
		Skill* pSkill = GetSkill(dwSkillID);
		if( !P_VALID(pSkill) ) continue;
		if( pSkill->GetCoolDownCountDown() > 0 )	continue;

		// todo: ����ط���Ҫ�жϱ������ܵ����ƣ�combathandler�����ټ�һ���жϱ��������ܷ�ʹ�õ��ж�

		BOOL bSelfUseble = FALSE;			// �����ܷ�����
		BOOL bTargetUseble = FALSE;			// Ŀ���ܷ�����

		// Jason
		nTargetCheckRet = m_CombatHandler.CheckTargetLogicLimit(pSkill, pTarget);
		nSkillCheckRet	= m_CombatHandler.CheckSkillLimit(pSkill);

		if( E_Success == nTargetCheckRet )
		{
			bTargetUseble = TRUE;
		}
		if( E_Success == nSkillCheckRet )
		{
			bSelfUseble = TRUE;
		}

		// �����Ŀ�궼�������ã���ֱ�ӷ��أ����������������Ҫ��������
		if( FALSE == bSelfUseble && bTargetUseble == FALSE )
			continue;

		// ��鼼������skillbuff���������Ա��е�buffid
		for(INT n = 0; n < MAX_BUFF_PER_SKILL; n++)
		{
			DWORD dwBuffTypeID = pSkill->GetBuffTypeID(n);
			DWORD dwTriggerID = pSkill->GetTriggerID(n);
			if( !P_VALID(dwBuffTypeID) || !P_VALID(dwTriggerID) ) continue;

			const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
			if( !P_VALID(pBuffProto) || !P_VALID(pTriggerProto) ) continue;

			// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
			if( ETEE_Null != pTriggerProto->eEventType && eEvent != pTriggerProto->eEventType )
				continue;

			BOOL bSelfTriggered = FALSE,bTargetTriggered = FALSE;

			// ���Ŀ��������ã���������Ŀ��
			if( bTargetUseble /*&& nSkillCheckRet != E_UseSkill_WeaponLimit*/ )
				bTargetTriggered = pTarget->TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);

			// �������������ã�������������
			if( bSelfUseble )
				bSelfTriggered = TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);

			if( bTargetTriggered || bSelfTriggered )
				StartSkillCoolDown(pSkill);
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------------
// ʹ����Ʒ״̬����
//------------------------------------------------------------------------------
BOOL Role::OnActiveItemBuffTrigger(tagItem* pItem, ETriggerEventType eEvent, 
					DWORD dwEventMisc1/*=GT_INVALID*/, DWORD dwEventMisc2/*=GT_INVALID*/)
{
	// ��Ʒ�Ƿ����
	if( !P_VALID(pItem)) return FALSE;

	for(INT n = 0; n < MAX_BUFF_PER_ITEM; ++n)
	{
		DWORD dwBuffTypeID = pItem->GetBuffID(n);
		DWORD dwTriggerID = pItem->GetTriggerID(n);
		if( !P_VALID(dwBuffTypeID) || !P_VALID(dwTriggerID) ) continue;

		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
		const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
		if( !P_VALID(pBuffProto) || !P_VALID(pTriggerProto) ) continue;

		// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
		if( ETEE_Null != pTriggerProto->eEventType && eEvent != pTriggerProto->eEventType )
			continue;

		// ���Լ��Ƿ�����
		TryAddBuff(this, pBuffProto, pTriggerProto, NULL, pItem);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------
// װ����ر���trigger��������
//----------------------------------------------------------------------------------
VOID Role::OnEquipmentBuffTriggerCommon(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType)
{
	// װ������ͨbuff���� -- ��ѭװ����
	BitSetEquipPos& equipSet = m_bitsetEquipTrigger[eTriggerType];
	tagFabao * pFabao = NULL;
	for(INT16 i=0; i<equipSet.size(); ++i)
	{
		if(!equipSet.test(i))
		{
			continue;
		}

		// �ҵ�װ��
		tagEquip *pEquip = GetItemMgr().GetEquipBarEquip(i);
		if(!P_VALID(pEquip))
		{
			// ִ�е��ô���˵��װ��buff��trigger����������
			ASSERT(0);
			IMSG(_T("\n\n\nCaution:\n"));
			IMSG(_T("\tTrigger&Buff of equip process maybe have problem! Please tell programmer to check!"));
			continue;
		}

		// �ҵ�װ����buff���������
		for(INT32 i=0; i<MAX_EQUIP_BUFF_NUM; ++i)
		{
			// ���Լ��Ƿ����㣬�ȼ����ؼ�buff
			INT nPctAdding = -1;
			if( MIsFaBao(pEquip) )	// �����ؼ�buff�Ĵ������ر���ˣ�����Ҫ�ӱ�С�ģ���Ӱ���������ؼ���buff����������е�Ӱ��Ч��
			{
				pFabao = (tagFabao*)pEquip;
				if( pFabao->n16Stage > 59 )
				{
					for( INT nStunt = 0; nStunt < MaxFairySkillNum; ++nStunt )
					{
						if( P_VALID(pFabao->dwFairySkill[nStunt]) )
						{
							const tagFabaoStuntProto * pProto = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[i]);
							if( !P_VALID(pProto) )
								continue;
							if( (pProto->nStuntType == -1 || pProto->nStuntType == (INT)pFabao->eEleInjury) )
							{
								nPctAdding = 100 * ((FLOAT)pFabao->nMood / 2000) + 50;
								const tagTriggerProto* pStuntTriggerProto = g_attRes.GetTriggerProto(pProto->dwTriggerID);
								const tagBuffProto*	pStuntBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID);
								if(ETEE_Null == pStuntTriggerProto->eEventType || eEvent != pStuntTriggerProto->eEventType)
									continue;
								TryAddBuff(this, pStuntBuffProto, pStuntTriggerProto, NULL, pEquip,TRUE,nPctAdding);
							}
						}
					}
				}
			}

			DWORD	dwBuffTypeID	=	GT_INVALID;
			DWORD	dwTriggerID		=	GT_INVALID;
			pEquip->GetTriggerIDBuffID(dwTriggerID, dwBuffTypeID, i);
			if(GT_INVALID == dwTriggerID || GT_INVALID == dwBuffTypeID)
			{
				continue;
			}

			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
			if(!P_VALID(pTriggerProto))
			{
				MAttResCaution(_T("equip trigger or buff"), _T("equip typeid"), pEquip->dwTypeID);
				continue;
			}

			// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
			if(ETEE_Null == pTriggerProto->eEventType || eEvent != pTriggerProto->eEventType)
			{
				continue;
			}

			const tagBuffProto*	pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
			if(!P_VALID(pBuffProto))
			{
				MAttResCaution(_T("equip buff"), _T("equip typeid"), pEquip->dwTypeID);
				continue;
			}

			TryAddBuff(this, pBuffProto, pTriggerProto, NULL, pEquip,TRUE);
		}

		if( MIsFaBao(pEquip) )
			TriggerFabaoMatchBuff((const tagFabao*)pEquip);
	}
}

//----------------------------------------------------------------------------------
// װ����ر���trigger��������
//----------------------------------------------------------------------------------
VOID Role::OnEquipmentBuffTriggerLongHun(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType)
{
	BitSetEquipPos& bitsetLongHun = m_bitsetLongHunTrigger[eTriggerType];
	for(INT16 i=0; i<bitsetLongHun.size(); ++i)
	{
		if(!bitsetLongHun.test(i))
		{
			continue;
		}

		// �ҵ���������ID
		DWORD dwLongHunID = m_LongHun.GetLongHunID(i);
		if(GT_INVALID == dwLongHunID)
		{
			ASSERT(0);
			IMSG(_T("\n\n\tLongHun reg/unreg maybe have problems! Please check!!!\n\n"));
		}

#ifdef	_DEBUG

		// �ҵ�װ��
		tagEquip *pEquip = GetItemMgr().GetEquipBarEquip(i);
		if(!P_VALID(pEquip))
		{
			// ִ�е��ô���˵��װ��buff��trigger����������
			ASSERT(0);
			IMSG(_T("\n\n\nCaution:\n"));
			IMSG(_T("\tTrigger&Buff of Longhun process maybe have problem! Please tell programmer to check!"));
		}
		else
		{
			DWORD	dwLongHunIDCheck	= GT_INVALID;
			BOOL	bOuter				= TRUE;

			pEquip->GetLongHunID(dwLongHunIDCheck, bOuter);
			if(dwLongHunIDCheck != dwLongHunID)
			{
				// ���������������м�¼��ʵ��װ���������
				ASSERT(0);
			}
		}

#endif

		// ��ȡ��������ԭ��
		const tagLongHunProto *pLongHunProto = g_attRes.GetLongHunProto(dwLongHunID);
		if(!P_VALID(pLongHunProto))
		{
			ASSERT(0);
			MAttResCaution(_T("LongHun"), _T("dwLongHunID"), dwLongHunID);
			continue;
		}

		if(GT_INVALID == pLongHunProto->dwTriggerID
			|| (GT_INVALID == pLongHunProto->dwBuffID1 && GT_INVALID == pLongHunProto->dwBuffID2))
		{
			ASSERT(0);
			MAttResCaution(_T("LongHun"), _T("dwLongHunID"), dwLongHunID);
			continue;
		}

		// trigger proto
		const tagTriggerProto* pTriggerProto	= g_attRes.GetTriggerProto(pLongHunProto->dwTriggerID);
		if(!P_VALID(pTriggerProto))
		{
			MAttResCaution(_T("longhun trigger"), _T("LongHunID"), dwLongHunID);
			continue;
		}

		// �鿴trigger�����Ƿ���ͬ�������
		if(ETEE_Null == pTriggerProto->eEventType || eEvent != pTriggerProto->eEventType)
		{
			ASSERT(0);
			IMSG(_T("\n\n\tLongHun reg/unreg maybe have problems! Please check!!!\n\n"));
			continue;
		}

		// buff proto
		if(pLongHunProto->dwBuffID1 != GT_INVALID)
		{
			const tagBuffProto*	pBuffProto	= g_attRes.GetBuffProto(pLongHunProto->dwBuffID1);
			if(!P_VALID(pBuffProto))
			{
				ASSERT(0);
				MAttResCaution(_T("LongHun Buff"), _T("LongHunID"), dwLongHunID);
			}
			else
			{
				// ���Լ��Ƿ�����
				TryAddBuff(this, pBuffProto, pTriggerProto, NULL, NULL);
			}
		}

		if(pLongHunProto->dwBuffID2 != GT_INVALID)
		{
			const tagBuffProto*	pBuffProto	= g_attRes.GetBuffProto(pLongHunProto->dwBuffID2);
			if(!P_VALID(pBuffProto))
			{
				ASSERT(0);
				MAttResCaution(_T("LongHun Buff"), _T("LongHunID"), dwLongHunID);
			}
			else
			{
				// ���Լ��Ƿ�����
				TryAddBuff(this, pBuffProto, pTriggerProto, NULL, NULL);
			}
		}
	}
}

//----------------------------------------------------------------------------------
// װ����ر���trigger��������
//----------------------------------------------------------------------------------
VOID Role::OnEquipmentBuffTriggerSuit(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType)
{
	SetSuitTrigger& suitSet = m_setSuitTrigger[eTriggerType];

	for(SetSuitTrigger::iterator it = suitSet.begin(); it != suitSet.end(); ++it)
	{
		// ��ȡ��װ�����������Ѽ���buff
		INT32 nSuitEquipNum = m_Suit.GetSuitEquipNum(*it);
		ASSERT(nSuitEquipNum >= MIN_SUIT_EQUIP_NUM);

		// ��װԭ��
		const tagSuitProto *pSuitProto = g_attRes.GetSuitProto(*it);
		if(!P_VALID(pSuitProto))
		{
			ASSERT(0);
			MAttResCaution(_T("suit"), _T("SuitID"), *it);
			continue;
		}

		if(nSuitEquipNum < pSuitProto->n8ActiveNum[0])
		{
			ASSERT(0);
			IMSG(_T("\n\n\tSuit trigger reg/unreg maybe have problems! Please check!!!\n\n"));
			continue;
		}

		for(INT32 i=0; i<MAX_SUIT_ATT_NUM; ++i)
		{
			if(nSuitEquipNum < pSuitProto->n8ActiveNum[i])
			{
				break;
			}

			if(GT_INVALID == pSuitProto->dwTriggerID[i] || GT_INVALID == pSuitProto->dwBuffID[i])
			{
				break;
			}

			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(pSuitProto->dwTriggerID[i]);
			if(!P_VALID(pTriggerProto))
			{
				MAttResCaution(_T("suit trigger"), _T("suitid"), *it);
				continue;
			}

			// �鿴trigger�����Ƿ���ͬ����Ҫ��Ϊ���Ż�
			if(ETEE_Null == pTriggerProto->eEventType || eEvent != pTriggerProto->eEventType)
			{
				continue;
			}

			const tagBuffProto*	pBuffProto = g_attRes.GetBuffProto(pSuitProto->dwBuffID[i]);
			if(!P_VALID(pBuffProto))
			{
				MAttResCaution(_T("suit buff"), _T("suitid"), *it);
				continue;
			}

			// ���Լ��Ƿ�����
			TryAddBuff(this, pBuffProto, pTriggerProto, NULL, NULL);
		}
	}
}

//----------------------------------------------------------------------------
// װ��״̬����
//----------------------------------------------------------------------------
BOOL Role::OnEquipmentBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, 
								  DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	// ���ȸ����¼������жϳ��������ܶ�Ӧ�Ĵ���������
	EPassiveSkillAndEquipTrigger eTriggerType = TriggerTypeToPassiveSkillAndEquipTriggerType(eEvent);
	if( EPSAET_Null == eTriggerType ) return FALSE;

	// װ������ͨbuff����
	if( m_bitsetEquipTrigger[eTriggerType].any() )
	{
		OnEquipmentBuffTriggerCommon(eEvent, eTriggerType);
	}

	// ��������
	if(m_bitsetLongHunTrigger[eTriggerType].any())
	{
		OnEquipmentBuffTriggerLongHun(eEvent, eTriggerType);
	}

	// ��װ
	if(m_setSuitTrigger[eTriggerType].size() > 0)
	{
		OnEquipmentBuffTriggerSuit(eEvent, eTriggerType);
	}

	return TRUE;
}

#ifdef ON_INTERNET_CAFE

VOID Role::EnableInternetCafe()
{
	m_byInternetCafePingLostTime = 0;
	m_dwInternetCafePingTime = time(NULL) + INTERNET_CAFE_PING_INTERVAL;
	AddBuff(INTERNET_CAFE_BUFF_LIST[0], (Unit*)this, GT_INVALID, NULL, TRUE);
}

VOID Role::CancelInternetCafe()
{
	for (INT i = 0; i < sizeof(INTERNET_CAFE_BUFF_LIST) / sizeof(DWORD); i++)
	{
		RemoveBuff(Buff::GetIDFromTypeID(INTERNET_CAFE_BUFF_LIST[i]), TRUE);
	}
}

VOID Role::UpdateInternetCafe()
{
	if (!IsEnd() && TRUE == g_InternetCafeSession.IsWell() && time(NULL) > m_dwInternetCafePingTime)
	{
		m_dwInternetCafePingTime = time(NULL) + INTERNET_CAFE_PING_INTERVAL;
		g_InternetCafeSession.SendPing((CHAR*)GetSession()->GetAccount(), GetID(), GetSession()->GetGetIP());
	}
}

VOID Role::SetInternetCafePingLostTime(BYTE byTime)
{
	m_byInternetCafePingLostTime = byTime;
}

BYTE Role::GetInternetCafePingLostTime()
{
	return m_byInternetCafePingLostTime;
}

#endif//ON_INTERNET_CAFE

//-----------------------------------------------------------------------------
// ����ͼ����
//-----------------------------------------------------------------------------
VOID Role::CheckMapArea()
{
	DWORD dwRet = 0;
	if( GetMap() )
		dwRet = GetMap()->CheckArea(this);

	// ��ȫ��
	if( dwRet & ERS_SafeArea )
	{
		// ���밲ȫ����
		if( !IsInRoleState(ERS_SafeArea) ) 
		{
			m_dwSpiUpdateTickCount = 0;
			// ������״̬��������ֵΪ0����ҽ��밲ȫ����Ϊ����ģʽ
			// Jason 2010-6-22 ���ӶԿ�ģʽ��Ӱ��
			if (!IsInRoleState(ERS_PK) && !IsInRoleState(ERS_PKEX) && !IsInRoleState(ERS_PeaceModel) /*&& 0 == GetAttValue(ERA_Hostility)*/ && !IsInRoleState(ERS_Safeguard))
			{
				SetRoleState(ERS_Safeguard);
				SetRoleSafeguardFlag(TRUE);
			}

			SetRoleState(ERS_SafeArea);
			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript))
				pRoleScript->ForceLootChase(this);
		}
	}
	else
	{
		// �뿪��ȫ����
		if( IsInRoleState(ERS_SafeArea) ) 
		{
			m_dwSpiUpdateTickCount = 0;
			// 30�����ϲ���δ����ʮ����pk���������ڵ�����뿪��ȫ����ʱ��Ϊ����ģʽ
			INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
			if (g_world.IsPkConstraintsOpen())
			{
				if( IsUsingPKConstraintsSafeGuradMinLevel() )
				{
					Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
					if (m_nLevel > Max_PK_SafeGuard_Level && m_nDeadUnSetSafeGuardCountDown <= 0 && IsInRoleState(ERS_Safeguard))
					{
						UnsetRoleState(ERS_Safeguard);
					}
				}
			}
			else
			{
				if (m_nLevel > Max_PK_SafeGuard_Level && m_nDeadUnSetSafeGuardCountDown <= 0 && IsInRoleState(ERS_Safeguard))
				{
					UnsetRoleState(ERS_Safeguard);
				}
			}

			if( GetRoleSafeguardFlag() && IsInRoleState(ERS_Safeguard) )
			{
				SetRoleSafeguardFlag(FALSE);
				UnsetRoleState(ERS_Safeguard);
			}
			UnsetRoleState(ERS_SafeArea);
		}
	}

	// PVP��
	if( dwRet & ERS_PVPArea )
	{
		if( !IsInRoleState(ERS_PVPArea) ) SetRoleState(ERS_PVPArea);
	}
	else
	{
		if( IsInRoleState(ERS_PVPArea) ) UnsetRoleState(ERS_PVPArea);
	}

	// ��̯��
	if( dwRet & ERS_StallArea )
	{
		if( !IsInRoleState(ERS_StallArea) ) SetRoleState(ERS_StallArea);
	}
	else
	{
		if( IsInRoleState(ERS_StallArea) ) UnsetRoleState(ERS_StallArea);
	}

	// ������
	if( dwRet & ERS_PrisonArea )
	{
		if( !IsInRoleState(ERS_PrisonArea) ) SetRoleState(ERS_PrisonArea);
	}
	else
	{
		if( IsInRoleState(ERS_PrisonArea) ) UnsetRoleState(ERS_PrisonArea);
	}

	// ��˰��
	if( dwRet & ERS_TaxArea )
	{
		if( !IsInRoleState(ERS_TaxArea) ) SetRoleState(ERS_TaxArea);
	}
	else
	{
		if( IsInRoleState(ERS_TaxArea) ) UnsetRoleState(ERS_TaxArea);
	}

}

//------------------------------------------------------------------------------
// �������ø����
//------------------------------------------------------------------------------
DWORD Role::QuestSetRebornMap(UINT16 u16QuestID, DWORD &dwBornMapID, Vector3 &vBornPos)
{
	return GT_INVALID;
	// ��õ�ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	if(pMap->GetMapInfo()->eType != EMT_Normal)
	{
		ASSERT(pMap->GetMapInfo()->eType == EMT_Normal);
		return E_BindRebornMap_MapInvalid;
	}

	if(GetRebornMapID() == pMap->GetMapID())
	{
		return E_BindRebornMap_Already;
	}

	// �ж��Ƿ�������
	if (IsHaveQuest(u16QuestID) || IsHaveDoneQuest(u16QuestID))
	{
		// ����
		m_dwRebornMapID = pMap->GetMapID();
		dwBornMapID = m_dwRebornMapID;
		vBornPos = g_mapCreator.GetReBornPoint(m_dwRebornMapID);
		return E_Success;
	}

	return E_BindRebornMap_MapInvalid;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
DWORD Role::Revive(ERoleReviveType eType, INT64 n64ItemID, BOOL bNeedItem)
{
	if( !IsInState(ES_Dead) )
		return E_Revive_NotDead;	// �Ѿ���������״̬��

	if (m_bNeedPrisonRevive)		// �Ƿ���Ҫ��������
	{
		eType = ERRT_Prison;
		m_bNeedPrisonRevive = FALSE;
	}

	DWORD dwErrorCode = E_Success;
	
	// �жϸ�������
	switch(eType)
	{
	case ERRT_ReturnCity:
		dwErrorCode = CityRevive();
		break;
	case ERRT_Locus:
		dwErrorCode = LocusRevive(n64ItemID, bNeedItem);
		break;
	case ERRT_Accept:
		dwErrorCode = AcceptRevive();
		break;
	case ERRT_Prison:
		dwErrorCode = PrisonRevive();
		break;
	case ERRT_Perfect:
		dwErrorCode = PerfectRevive();
		break;
	default:
		// ����ִ�е�����(�ϲ����������ж�)
		ASSERT(0);
		dwErrorCode = E_Revive_Unknown;
	}

	GetMap()->OnRevive(this, eType, m_Revive.nHP, m_Revive.nMP, m_Revive.fX, m_Revive.fY, m_Revive.fZ, m_Revive.dwMapID);

	// ����
	if(E_Success == dwErrorCode)
	{
		SetAttValue(ERA_HP, m_Revive.nHP);
		SetAttValue(ERA_MP, m_Revive.nMP);
		UnsetState(ES_Dead);
	}

	// ��ո�����ؼ�¼��Ϣ
	m_Revive.nHP = 0;
	m_Revive.nMP = 0;

	return dwErrorCode;
}

DWORD Role::CityRevive()
{
	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(m_dwRebornMapID);
	if(!P_VALID(pMapInfo))
	{
		ASSERT(P_VALID(pMapInfo));
		return E_Revive_MapNotFound;
	}

	// �ж��Ƿ��ڳ�սʱ
	if (IsInState(ES_CSAttack) || IsInState(ES_CSDefence))
	{
		return E_Revive_CanNotTransmit;			// �޷��ڳ�սʱ�سǸ���
	}

	// ���ø���㼰Ѫ����������
	m_Revive.nHP = GetAttValue(ERA_MaxHP);
	m_Revive.nMP = GetAttValue(ERA_MaxMP);
	m_Revive.fX = pMapInfo->vRebornPos.x;
	m_Revive.fY = pMapInfo->vRebornPos.y;
	m_Revive.fZ = pMapInfo->vRebornPos.z;
	m_Revive.dwMapID = m_dwRebornMapID;

	IsWarRevive();

	// �ж��Ƿ���Ӣ�������Ⱥ����¹��ͼ����
	// 2983744463 m21
	// 2983743695 m22
	if (pMapInfo->dwID != TObjRef<Util>()->Crc32("m21") && pMapInfo->dwID != TObjRef<Util>()->Crc32("m22") && pMapInfo->dwID != TObjRef<Util>()->Crc32("s59"))
	{
		SetHostilitySafeGuard();
	}

	return E_Success;
}

DWORD Role::LocusRevive(INT64 n64ItemID, BOOL bNeedItem)
{
	// �ж��Ƿ��ڳ�սʱ
	if (IsInState(ES_CSAttack) || IsInState(ES_CSDefence))
	{
		return E_Revive_CanNotReviveOnCityStruggle;			// �޷��ڳ�սʱԭ�ظ���
	}

	// ���������޷�����
	if( IsInState(ES_PvPAttack) || IsInState(ES_PvPDefence))
	{
		return E_Revive_CanNotReviveInPvP;
	}

	// �ж��Ƿ������䳡
	if (IsContestMap(GetMapID()))
	{
		return E_Revive_CanNotReviveOnContestMap;			// �޷������䳡ԭ�ظ���
	}

	// �����Ʒ
	if (bNeedItem)
	{
		tagItem *pItem = GetItemMgr().GetBagItem(n64ItemID);
		if(!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_RevieveItem)
		{
			return E_Revive_ItemLimit;
		}
	
		// ɾ����Ʒ
		DWORD dwErrorCode = GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCLD_Revive_Locus);
		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}

		// ���ø���㼰Ѫ����������
		m_Revive.nHP = GetAttValue(ERA_MaxHP) / 4;
		m_Revive.nMP = GetAttValue(ERA_MaxMP) / 4;
	}
	else
	{
		// ���ø���㼰Ѫ����������
		m_Revive.nHP = GetAttValue(ERA_MaxHP);
		m_Revive.nMP = GetAttValue(ERA_MaxMP);
	}

	m_Revive.fX = m_MoveData.m_vPos.x;
	m_Revive.fY = m_MoveData.m_vPos.y;
	m_Revive.fZ = m_MoveData.m_vPos.z;
	m_Revive.dwMapID = GetMapID();

	IsWarRevive();

	return E_Success;
}

DWORD Role::AcceptRevive()
{
	// ���������޷�����
	if( IsInState(ES_PvPAttack) || IsInState(ES_PvPDefence))
	{
		return E_Revive_CanNotReviveInPvP;
	}

	m_Revive.dwMapID = GetMapID();
	m_Revive.fX = GetCurPos().x;
	m_Revive.fY = GetCurPos().y;
	m_Revive.fZ = GetCurPos().z;

	IsWarRevive();

	// ���HP
	if(m_Revive.nHP <= 0)
	{
		return GT_INVALID;
	}
	
	return E_Success;
}

DWORD Role::PrisonRevive()
{
	DWORD	dwPrisonMapID	= g_mapCreator.GetPrisonMapID();
	Vector3	PutInPoint		= g_mapCreator.GetPutInPrisonPoint();

	// ���������޷�����
	if( IsInState(ES_PvPAttack) || IsInState(ES_PvPDefence))
	{
		return E_Revive_CanNotReviveInPvP;
	}

	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(dwPrisonMapID);
	if(!P_VALID(pMapInfo))
	{
		ASSERT(P_VALID(pMapInfo));
		return E_Revive_MapNotFound;
	}

	// ���ø���㼰Ѫ����������
	m_Revive.nHP = GetAttValue(ERA_MaxHP);
	m_Revive.nMP = GetAttValue(ERA_MaxMP);
	m_Revive.fX = PutInPoint.x;
	m_Revive.fY = PutInPoint.y;
	m_Revive.fZ = PutInPoint.z;
	m_Revive.dwMapID = dwPrisonMapID;

	return E_Success;
}

DWORD Role::PerfectRevive()
{
	// ���������޷�����
	if( IsInState(ES_PvPAttack) || IsInState(ES_PvPDefence))
	{
		return E_Revive_CanNotReviveInPvP;
	}

	// ��������3070018,9��,��Ҫ5������
	INT32 num = GetItemMgr().GetBag().GetSameItemCount(3070018);
	if( num<5 )
	{
		INT32 numCount = GetItemMgr().GetBag().GetSameItemCount(3070019);
		if( numCount + num < 5)
		{	
		     return E_Revive_ItemLimit;
		}
	}

	// ɾ����Ʒ
	tagItem* pItem = NULL;
	TMap<INT64, INT16>::TMapIterator iter = GetItemMgr().GetBag().Begin();
	INT count = 0; // �Ѿ��ռ����������������
	while( GetItemMgr().GetBag().GetNextItem(iter, pItem) )
	{
		if (!P_VALID(pItem))
		{
			continue;
		}
		if(pItem->dwTypeID == 3070018)
		{
			if (pItem->GetNum()>=5-count)
			{
				GetItemMgr().DelFromBag( pItem->n64Serial, ELCLD_Revive_Perfect, 5-count);
				break;
			}
			else
			{
				count += pItem->GetNum();
				GetItemMgr().DelFromBag( pItem->n64Serial, ELCLD_Revive_Perfect );
			}
			continue;
		}

		if(pItem->dwTypeID == 3070019)
		{
			if (pItem->GetNum()>=5-count)
			{
				GetItemMgr().DelFromBag( pItem->n64Serial, ELCLD_Revive_Perfect, 5-count);
				break;
			}
			else
			{
				count += pItem->GetNum();
				GetItemMgr().DelFromBag( pItem->n64Serial, ELCLD_Revive_Perfect );
			}
			continue;
		}

	}

	

	// ���ø���㼰Ѫ����������
	m_Revive.nHP = GetAttValue(ERA_MaxHP);
	m_Revive.nMP = GetAttValue(ERA_MaxMP);
	
	m_Revive.fX = m_MoveData.m_vPos.x;
	m_Revive.fY = m_MoveData.m_vPos.y;
	m_Revive.fZ = m_MoveData.m_vPos.z;
	m_Revive.dwMapID = GetMapID();

	IsWarRevive();

	return E_Success;
}

//-----------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------
VOID Role::BeRevived(INT nHP, INT nMP, Unit* pSrc)
{
	if( nHP <= 0 ) return;

	m_Revive.nHP = nHP;
	m_Revive.nMP = nMP;

	//if( P_VALID(pSrc) && pSrc->GetMapID() == GetMapID() )
	//{
	//	m_Revive.dwMapID = GetMapID();
	//	m_Revive.fX = pSrc->GetCurPos().x;
	//	m_Revive.fY = pSrc->GetCurPos().y;
	//	m_Revive.fZ = pSrc->GetCurPos().z;
	//}
	//else
	//{
	//	m_Revive.dwMapID = GetMapID();
	//	m_Revive.fX = GetCurPos().x;
	//	m_Revive.fY = GetCurPos().y;
	//	m_Revive.fZ = GetCurPos().z;
	//}


	// ���͸��ͻ���
	tagNS_RoleReviveNotify send;
	SendMessage(&send, send.dwSize);
}

DWORD Role::SendCloseStall()
{
	if(!IsInRoleStateAny(ERS_Stall | ERS_StallSet))
	{
		return E_Success;
	}
	
	tagNS_StallClose send;
	send.dwErrorCode	= CloseStall();
	send.dwStallRoleID	= GetID();
	if(P_VALID(GetMap()))
	{
		GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
	}

	return send.dwErrorCode;
}

DWORD Role::UpdateFriOnline()
{
	INT				nFriendNum = GetFriendCount();
	DWORD			dwFriendID = GT_INVALID;
	tagRoleInfo*	pRoleInfo = (tagRoleInfo*)GT_INVALID;
	tagFriend*		pFriend = (tagFriend*)GT_INVALID;
	INT				nIndex = 0;
	DWORD			dwSize = sizeof(tagNS_UpdateFriEnemyState);
		
	if( nFriendNum > 0 )
	{
		dwSize += (nFriendNum-1) * sizeof(tagFriUpdate);
	}
	
	MCREATE_MSG(pSend, dwSize, NS_UpdateFriEnemyState);
		
	INT nEnemyIdx = 0;
	for (INT nCnt = 0; nCnt < MAX_ENEMYNUM; nCnt++)
	{
		if (GT_INVALID == m_dwEnemyList[nCnt]) continue;

		pSend->dwEnemysID[nEnemyIdx].dwEnemyID = m_dwEnemyList[nCnt];

		pRoleInfo = g_roleMgr.GetRoleInfo(m_dwEnemyList[nCnt]);
		if (P_VALID(pRoleInfo))
		{
			pSend->dwEnemysID[nEnemyIdx].nLevel = pRoleInfo->byLevel;
		}

		++nEnemyIdx;
	}

	m_mapFriend.ResetIterator();
	while(m_mapFriend.PeekNext(dwFriendID, pFriend))
	{
		pRoleInfo = g_roleMgr.GetRoleInfo(dwFriendID);
		
		if(P_VALID(pRoleInfo))
		{
			pSend->FriUpdate[nIndex].dwRoleID = dwFriendID;
			pSend->FriUpdate[nIndex].bOnline = pRoleInfo->bOnline;
			pSend->FriUpdate[nIndex].nLevel = pRoleInfo->byLevel;
			Role* pRole = g_roleMgr.GetRolePtrByID(dwFriendID);
			if (P_VALID(pRole))
				pSend->FriUpdate[nIndex].nVIPLevel = pRole->GetAttValue(ERA_VipLevel);
			else
				pSend->FriUpdate[nIndex].nVIPLevel = pRoleInfo->nVIPLevel;
			++nIndex;
		}
		else
		{	
			tagNS_RoleCancelFriend send;
			send.dwDestRoleID = dwFriendID;
			send.byGroup = pFriend->byGroup;
			send.dwErrorCode = E_Success;
			SendMessage(&send, send.dwSize);

			pFriend->dwFriendID = GT_INVALID;
			pFriend->dwFriVal = 0;
			pFriend->byGroup = 1;
			m_mapFriend.Erase(dwFriendID);
		}
	}

	pSend->nNum = nIndex;
	if( nIndex > 0) 
	{
		pSend->dwSize = (nIndex - 1) * sizeof(tagFriUpdate) + sizeof(tagNS_UpdateFriEnemyState);
	}
	else
		pSend->dwSize = sizeof(tagNS_UpdateFriEnemyState);

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}

//--------------------------------------------------------------------------------
// ���Ըı�������������ݸı�
//--------------------------------------------------------------------------------
VOID Role::OnAttChange(INT nIndex)
{
	switch(nIndex)
	{
		// Ѫ��
	case ERA_HP:
		m_nAtt[ERA_HP] = min(m_nAtt[ERA_HP], m_nAtt[ERA_MaxHP]);
		break;

		// ����
	case ERA_MP:
		m_nAtt[ERA_MP] = min(m_nAtt[ERA_MP], m_nAtt[ERA_MaxMP]);
		break;

		// ����
	case ERA_Vitality:
		m_nAtt[ERA_Vitality] = min(m_nAtt[ERA_Vitality], m_nAtt[ERA_MaxVitality]);
		break;

	case ERA_Endurance:
		m_nAtt[ERA_Endurance] = min(m_nAtt[ERA_Endurance], m_nAtt[ERA_MaxEndurance]);
		break;

		// ����
	case ERA_Morality:
		{
			CalPKState();
		}
		break;

	case ERA_Spirit:
		{
			m_fSpiRate = CalSpiritRate(m_nAtt[ERA_Spirit]);
		}
		break;
	case ERA_Shape:
		{
			if(m_nMountShape >0 && m_nSelfShape>0 && m_nAtt[ERA_Shape] != m_nMountShape)
			{
				m_nSelfShape = GetAttValue(ERA_Shape);
				SetAttValue(ERA_Shape, m_nMountShape);
			}
		}
		break;
	default:
		break;
	}

	// �ƺ���������
	if( nIndex >= 0 && nIndex < ERA_End )
	GetTitleMgr()->SigEvent(ETE_ROLE_ATT, nIndex, m_nAtt[nIndex]);
}

VOID Role::OnAttChange(bool bRecalFlag[ERA_End])
{
	// XZ�����ٶ�
	if( bRecalFlag[ERA_Speed_XZ] )
	{
		m_fXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f);
		if( E_Success == m_MoveData.StopMove() )
		{
			// ���͸��Լ��ٶȸı�
			tagNS_MoveSpeedChange send;
			send.dwRoleID = m_dwID;
			send.curPos = m_MoveData.m_vPos;
			send.faceTo = m_MoveData.m_vFace;
			SendMessage(&send, send.dwSize);
		}
	}

	// Y�����ٶ�
	if( bRecalFlag[ERA_Speed_Y] )
	{
		m_fYSpeed = X_DEF_Y_SPEED * (FLOAT(m_nAtt[ERA_Speed_Y]) / 10000.0f);
	}

	// ��Ӿ�ٶ�
	if( bRecalFlag[ERA_Speed_Swim] )
	{
		m_fSwimXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_Swim]) / 10000.0f);
		if( E_Success == m_MoveData.StopMove() )
		{
			// ���͸��Լ��ٶȸı�
			tagNS_MoveSpeedChange send;
			send.dwRoleID = m_dwID;
			send.curPos = m_MoveData.m_vPos;
			send.faceTo = m_MoveData.m_vFace;
			SendMessage(&send, send.dwSize);
		}
	}

	// ����ٶ�
	if( bRecalFlag[ERA_Speed_Mount] )
	{
		m_fMountXZSpeed = X_DEF_XZ_SPEED * (FLOAT(m_nAtt[ERA_Speed_Mount]) / 10000.0f);
		if( E_Success == m_MoveData.StopMove() )
		{
			// ���͸��Լ��ٶȸı�
			tagNS_MoveSpeedChange send;
			send.dwRoleID = m_dwID;
			send.curPos = m_MoveData.m_vPos;
			send.faceTo = m_MoveData.m_vFace;
			SendMessage(&send, send.dwSize);
		}
	}

	// ����
	if( bRecalFlag[ERA_Shape] )
	{
		FLOAT fCosf = FLOAT(m_nAtt[ERA_Shape]) / 10000.0f;
		m_Size.x = X_DEF_ROLE_SIZE_X * fCosf;
		m_Size.y = X_DEF_ROLE_SIZE_Y * fCosf;
		m_Size.z = X_DEF_ROLE_SIZE_Z * fCosf;
	}

	// �ӳ�ͳ�����ı�
	if(GetTeamID() != GT_INVALID && bRecalFlag[ERA_Rein])
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(GetTeamID());
		if( P_VALID(pTeam) && pTeam->IsLeader(GetID()) )
		{
			tagLeaderRein	LeaderRein;
			LeaderRein.nLeaderRein = GetAttValue(ERA_Rein);
			g_groupMgr.AddEvent(GetID(), EVT_ChangeRein, sizeof(tagLeaderRein), &LeaderRein);
		}
	}

	// ����ֵ����
	if(bRecalFlag[ERA_Spirit])
	{
		m_fSpiRate = CalSpiritRate(m_nAtt[ERA_Spirit]);
	}

	for(int i=0; i<ERA_End; ++i)
		if (bRecalFlag[i])
			GetTitleMgr()->SigEvent(ETE_ROLE_ATT, i, m_nAtt[i]);
}

VOID Role::OnLeaveMap( BOOL bChgMap /*= TRUE*/ )
{
	// �л���ͼ��ʧ��buff
	if (bChgMap)
	{
		// buff ���
		OnInterruptBuffEvent(EBIF_ChangeMap);

		// ���ִ��
		GetCombatHandler().InterruptPrepare(CombatHandler::EIT_Move, FALSE);

		// �ͷŴ��
		GetCombatHandler().InterruptOperate(CombatHandler::EIT_Move, EMS_Walk);
	}

	//�������
	InterruptCondense();

	// ��ճ���б�
	ClearEnmityCreature();

	ClearTargetBy();

	// �����̯
	SendCloseStall();

	// ������
	BreakMount();
	// �����ٻ�����
	GetPetPocket()->CalledPetEnterPocket();
	m_nIsSended = 0 ;
    m_dwMapBuffID = 0 ;
}

VOID Role::OnEnterMap()
{
	m_pScript->OnRoleEnterMap(this);

	// �ͷ��ٻ�����
	GetPetPocket()->CalledPetLeavePocket();
	ContinueMount();

	static DWORD dwMapS02ID = 0 ;
	if( dwMapS02ID == 0 )
	{
		dwMapS02ID = IUTIL->Crc32(_T("s02"));
	}
	if( m_dwRebornMapID != dwMapS02ID )
	{
		Map * pMap = GetMap();
		if( P_VALID(pMap) )
		{
			const tagMapInfo* pMapInfo = pMap->GetMapInfo();
			if( P_VALID(pMapInfo) && pMapInfo->dwID == dwMapS02ID && GetLevel() >= 10 )
			{
				m_dwRebornMapID = dwMapS02ID;
			}
		}
	}
}

//--------------------------------------------------------------------------------
// ���ð�������
//--------------------------------------------------------------------------------
VOID Role::SetGuildID(DWORD dwGuildID)
{
	m_dwGuildID = dwGuildID;

	tagNDBC_ChangeRoleGuild send;
	send.dwRoleID	= GetID();
	send.dwGuildID	= dwGuildID;
	g_dbSession.Send(&send, send.dwSize);
}

//--------------------------------------------------------------------------------
//���ͽ���ͼbuff��ʣ��ʱ����ͻ���  
//--------------------------------------------------------------------------------

VOID Role::UpdateSendMapBuffLeftTimeToClient()
{
	//�ж�����Ƿ������������ͼ �ǵĻ��͸�����buffid
	if( 0 == m_dwMapBuffID )
	{
		Map* pMap = GetMap();
		if ( P_VALID(pMap) )
		{
			tagMapBuffInfo* pMapBuffInfo = g_attRes.GetMapBuffInfo(pMap->GetMapID());
			if ( P_VALID(pMapBuffInfo)  )
			{
				for( INT i = 0 ; i < MAPBUFF_MAXCOUNT ; i++)
				{

					DWORD dwBuffID = pMapBuffInfo->dwBuffID[i];
					DWORD dwID = Buff::GetIDFromTypeID(dwBuffID);
					Buff* pBuff = GetBuffPtr(dwID);
					if( P_VALID(pBuff) )
					{
						SetRoleMapBuffID(dwID);
						break;

					}

				}
			}
		}
	}

	if ( 0 != m_dwMapBuffID )
	{
		  Buff* pBuff = GetBuffPtr(m_dwMapBuffID);
          if( P_VALID(pBuff) )
		  {
			  DWORD dwPersistTickLeft = pBuff->GetPersistTimeLeft();
			  if ( dwPersistTickLeft <= 300*TICK_TIME )  //ʣ��1����(tick)
			  {

				  if( 0 == m_nIsSended ) 
				  {
					  tagNS_XunLianChangRemainTime msg;
					  msg.dwBuffID = m_dwMapBuffID;
					  msg.dwTime   = dwPersistTickLeft ;
					  SendMessage(&msg,msg.dwSize);
					  m_nIsSended = 1;
				  }

				  if( 3 == m_nIsSended ) 
				  {
					  tagNS_XunLianChangRemainTime msg;
					  msg.dwBuffID = m_dwMapBuffID;
					  msg.dwTime   = dwPersistTickLeft ;
					  SendMessage(&msg,msg.dwSize);
					  m_nIsSended = 1;
				  }


			  }
			  else if ( dwPersistTickLeft <= 1500*TICK_TIME - 5000)    //shengyu
			  {

				  if( 0 == m_nIsSended )
				  {
					  tagNS_XunLianChangRemainTime msg;
					  msg.dwBuffID = m_dwMapBuffID;
					  msg.dwTime   = dwPersistTickLeft ;
					  SendMessage(&msg,msg.dwSize);
					  m_nIsSended = 3;
				  }

				  if( 2 == m_nIsSended )
				  {
					  tagNS_XunLianChangRemainTime msg;
					  msg.dwBuffID = m_dwMapBuffID;
					  msg.dwTime   = dwPersistTickLeft ;
					  SendMessage(&msg,msg.dwSize);
					  m_nIsSended = 3; 
				  }
			  }
			  else if ( dwPersistTickLeft <= (3000*TICK_TIME- 5000) ) //ʣ��10����
			  {
				  if( 0 == m_nIsSended )
				  {
					  tagNS_XunLianChangRemainTime msg;
					  msg.dwBuffID = m_dwMapBuffID;
					  msg.dwTime   = dwPersistTickLeft ;
					  SendMessage(&msg,msg.dwSize);
					  m_nIsSended = 2;

				  }
				
			  }
                 
		  }

	}
	
	
}

VOID Role::SendMessage(LPVOID pMsg, DWORD dwSize, EMessageRange eRange/* = EMR_Self*/)
{
	switch (eRange)
	{
	case EMR_Self:
		if (P_VALID(GetSession()))
		{
			GetSession()->SendMessage(pMsg, dwSize);
		}
		break;
	case EMR_Around:
		if (P_VALID(GetMap()))
		{
			GetMap()->SendBigVisTileMsg(this, pMsg, dwSize);
		}
		break;
	case EMR_Map:
		if (P_VALID(GetMap()))
		{
			GetMap()->SendMapMessage(pMsg, dwSize);
		}
		break;
	case EMR_World:
		// ��ʱ����Ҫ [4/28/2012 zhangzhihua]
		break;
	}
}

//--------------------------------------------------------------------------------
// ���纰��
//--------------------------------------------------------------------------------
BOOL Role::TalkToWorld()
{
	if (m_nWorldTalkCounter < 0)
	{
		m_nWorldTalkCounter = WORLD_CHANNEL_INTERVAL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//--------------------------------------------------------------------------------
// �������纰����ʱ
//--------------------------------------------------------------------------------
VOID Role::UpdateTalkToWorld()
{
	if (m_nWorldTalkCounter >= 0)
	{
		--m_nWorldTalkCounter;
	}
}

//--------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------
BOOL Role::TalkToScene()
{
	if (m_nSceneTalkCounter <= 0)
	{
		m_nSceneTalkCounter = SCENE_CHANNEL_INTERVAL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

//--------------------------------------------------------------------------------
// ������������
//--------------------------------------------------------------------------------
VOID Role::UpDateTalkToScene()
{
	if (m_nSceneTalkCounter > 0)
	{
		--m_nSceneTalkCounter;
	}
}

//--------------------------------------------------------------------------------
// Ⱥ�����������Ⱥ
//--------------------------------------------------------------------------------
VOID   Role::ClearGroupChat()
{
	std::list<group_chat*> listGroupChat;
	m_mapGroupChat.ExportAllValue(listGroupChat);
	std::list<group_chat*>::iterator it = listGroupChat.begin();
	for ( ; it != listGroupChat.end(); it++ )
	{
		(*it)->LoginOutGroup(this);
		tagNS_LoginOutGroupChat msg;
		msg.dwError  =  E_Success;
		msg.dwRoleId =  m_dwID;
		msg.dwGroupChatId = (*it)->GetGroupChatId();
		msg.dwGroupChatMasterId  = (*it)->GetGroupChatMasterId();
		(*it)->SendGroupMsg(&msg,msg.dwSize);
		if ( (*it)->GetGroupMemberNum() <= 0)
		{
			g_GroupChatMgr.DeleteGroupChat((*it)->GetGroupChatId());
		}

	}
	m_mapGroupChat.Clear();
}


//--------------------------------------------------------------------------------
// ʰȡ��Ʒ
//--------------------------------------------------------------------------------
DWORD Role::PickUpItem( INT64 n64GroundID, DWORD& dwTypeID, BOOL bRole )
{
	if (bRole)
		StopMount();

	MTRANS_ELSE_RET(pMap, GetMap(), Map, E_Loot_Map_Not_Found);
	MTRANS_ELSE_RET(pGroundItem, pMap->GetGroundItem(n64GroundID), tagGroundItem, E_Loot_Item_Not_Found);

	DWORD dwRtv = E_Success;

	dwTypeID = pGroundItem->dwTypeID;
	Role* pToRole = this;
	tagItem * pItemGetted = NULL;
	INT64 n64Itemn = 0;
	if( P_VALID(pGroundItem->pItem) )
		n64Itemn = pGroundItem->pItem->n64Serial;

	// �����ʰȡʥ�飬�ж�ʥ�������Ƿ�ﵽ����
	tagEquipProto * pEquipProto = g_attRes.GetEquipProto(pGroundItem->dwTypeID);
	if (P_VALID(pEquipProto) && MIsHolyType(pEquipProto->eType) && GetItemMgr().IsHolyExceed())
	{
		return E_Loot_Holy_CountLimit;
	}

	//ע��ѵ�����Ʒʰȡ��������,����tagGroundItem�е�pItemָ��ᱻ����,���ԷŽ�����Ӧ�������
	if (pGroundItem->dwTypeID == TYPE_ID_MONEY)
	{
		if(FALSE == GetCurMgr().IncBagSilver(pGroundItem->nNum, ELCLD_PickUp_Money))
			return E_Loot_Add_Bag_Not_Success;
	}
	//ע��ѵ�����Ʒʰȡ��������,����tagGroundItem�е�pItemָ��ᱻ����,���ԷŽ�����Ӧ�������
	else
	{			
		if(pGroundItem->dwTeamID != GT_INVALID && pGroundItem->dwOwnerID!=GT_INVALID && pGroundItem->dwOwnerID != GetID()) // ����CanPick�жϣ� TeamID����ɫ��TeamID��Ȼ��ͬ
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pGroundItem->dwOwnerID);
			if(P_VALID(pRole))
			{
				if(pRole->IsInTeamPickupRank(GetMapID(), pGroundItem->vPos) && pRole->GetItemMgr().GetBagFreeSize() > 0
					&& pRole->GetItemMgr().CanAddMaxHoldItem(pGroundItem->dwTypeID, pGroundItem->nNum)
					&&E_Success == (dwRtv = pRole->GetItemMgr().Add2Bag(pGroundItem->pItem, ELCLD_PickUp_Item, TRUE)))
				{
					pToRole = pRole; 
					pItemGetted = pRole->GetItemMgr().GetBagItem(n64Itemn);
				}
				else{
					dwRtv = GetItemMgr().Add2Bag(pGroundItem->pItem, ELCLD_PickUp_Item, TRUE);
				}
				if(E_Success != dwRtv) return dwRtv;
			}
			else return E_Loot_Belong_To_You;
		}
		else if(E_Success != (dwRtv = GetItemMgr().Add2Bag(pGroundItem->pItem, ELCLD_PickUp_Item, TRUE)))
			return dwRtv;
	}
	// ���µ�ǰ������Ʒ

	if( !P_VALID(pItemGetted) )
		pItemGetted = GetItemMgr().GetBagItem(n64Itemn);

	// Jason 2009-12-4������䷽ʽ��©���ġ�
	Team::BroadcastLootItemMsgInTeamExceptPicker(pToRole,/*pGroundItem->pItem*/pItemGetted);
	//������Ʒ��ʧ��Ϣ
	tagNS_RoleGroundItemDisappear disappear;
	disappear.n64Serial[0] = n64GroundID;
	pMap->SendBigVisTileMsg(this, &disappear, disappear.dwSize);

	//��Map��ɾ��������Ʒ
	//��������Ʒ�ӵ���ɾ��
	pMap->RemoveGroundItem(pGroundItem);

	//��ָ��ָ�ΪNULL
	pGroundItem->pItem=NULL;
	SAFE_DEL(pGroundItem);

	// ��һ��һ�����ߵ��¼��ӿ�
	g_ScriptMgr.GetRoleScript()->OnObtainItem(pToRole, dwTypeID, EICM_Loot);


	return E_Success;
}

//--------------------------------------------------------------------------------
// ������Ʒ
//--------------------------------------------------------------------------------
DWORD Role::PutDownItem( INT64 n64Serial, INT64 &n64GroupID, BOOL bAdd2Ground)
{

	MTRANS_ELSE_RET(pMap, GetMap(), Map, GT_INVALID);

	DWORD dwRet = E_Success;
	tagItem *pItem = NULL;

	;

	tagItem *pItemTemp = GetItemMgr().GetBag().GetItem(n64Serial);
	if(P_VALID(pItemTemp))
	{
		const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
		if(P_VALID(pScript) && P_VALID(pItemTemp->pProtoType) )
		{
			pScript->DelItem( 2, pItemTemp->dwTypeID, pItemTemp->pProtoType->byQuality );
		}
	}


	if( E_Success != (dwRet = GetItemMgr().DiscardFromBag(n64Serial, ELCLD_Role_Discard_Item, pItem)))
	{
		return dwRet;
	}
	
	// ����������Ʒ���ٵ��䵽����
	if (P_VALID(pItem) && bAdd2Ground)
	{
		tagGroundItem* pGroundItem = new tagGroundItem(pMap->GenGroundItemID(), 
													pItem->dwTypeID, pItem->GetNum(), pItem, 
													GetCurPos(), GT_INVALID, GT_INVALID, 0, GetID());

		ASSERT(pGroundItem->IsValid());

		pMap->AddGroundItem(pGroundItem);

		n64GroupID = pGroundItem->n64ID;
	}
	else if( P_VALID(pItem) )
	{
		Destroy(pItem);
	}

	return E_Success;
}

//--------------------------------------------------------------------------------
// ������Ʒ
//--------------------------------------------------------------------------------
DWORD Role::PutDownWarItem( INT64 n64Serial, INT64 &n64GroupID, BOOL bAdd2Ground)
{
	MTRANS_ELSE_RET(pMap, GetMap(), Map, GT_INVALID);

	DWORD dwRet = E_Success;
	tagItem *pItem = NULL;
	dwRet = GetItemMgr().DiscardFromWarBag( n64Serial, ELCLD_Role_Discard_Item, pItem);
	if( E_Success != dwRet )
		return dwRet;

	// ����������Ʒ���ٵ��䵽����
	if( P_VALID(pItem) && bAdd2Ground )
	{
		tagGroundItem* pGroundItem = new tagGroundItem(pMap->GenGroundItemID(), 
			pItem->dwTypeID, pItem->GetNum(), pItem, 
			GetCurPos(), GT_INVALID, GT_INVALID, 0, GetID());

		ASSERT(pGroundItem->IsValid());

		pMap->AddGroundItem(pGroundItem);

		n64GroupID = pGroundItem->n64ID;
	}
	else if( P_VALID(pItem) )
	{
		Destroy(pItem);
	}

	return E_Success;
}

DWORD Role::LootItem(INT64 n64Serial, INT64 &n64GroupID)
{
	MTRANS_ELSE_RET(pMap, GetMap(), Map, GT_INVALID);

	DWORD dwRet = E_Success;
	tagItem *pItem = NULL;

	if( E_Success != (dwRet = GetItemMgr().LootFromBag(n64Serial, ELCLD_Role_Discard_Item, pItem)))
		return dwRet;

	if(P_VALID(pItem))
	{
		tagGroundItem* pGroundItem = new tagGroundItem(pMap->GenGroundItemID(), 
			pItem->dwTypeID, pItem->GetNum(), pItem, 
			GetCurPos(), GT_INVALID, GT_INVALID, 0, GetID());

		ASSERT(pGroundItem->IsValid());

		pMap->AddGroundItem(pGroundItem);

		n64GroupID = pGroundItem->n64ID;
	}
	return E_Success;
}

VOID Role::SetMount( BOOL bSet, INT nMountSpeed, const tagPetProto* pPetProto )
{
	if (bSet)
	{
		// ��������״̬
		SetRoleState(ERS_Mount);

		// ������ײ��
		SetSize(pPetProto->vSize * pPetProto->fScale);

		// ���������ٶ�
		ModAttModValue(ERA_Speed_Mount, nMountSpeed);

		//���buff
		if(pPetProto->dwAddBuffID != GT_INVALID)
		{
			TryAddBuffToMyself(pPetProto->dwAddBuffID);
		}
	}
	else
	{
		// ��������״̬
		UnsetRoleState(ERS_Mount);

		// ������ײ��
		SetAttRecalFlag(ERA_Shape);

		// ���������ٶ�
		ModAttModValue(ERA_Speed_Mount, -nMountSpeed);

		//ɾ��buff
		if(pPetProto->dwAddBuffID != GT_INVALID)
		{
			RemoveBuff(pPetProto->dwAddBuffID/100, TRUE);
		}
	}

	RecalAtt();	
}

FLOAT Role::GetVNBExpRate()
{
	return GetSession()->GetVNBExpRate() / 10000.0f;
}

FLOAT Role::GetVNBLootRate()
{
	return GetSession()->GetVNBLootRate() / 10000.0f;
}

//--------------------------------------------------------------------------------
// ���ӽ�ɫ����������
//--------------------------------------------------------------------------------
VOID Role::IncTreasureSum()
{
	++m_nTreasureSum;
	if (m_nTreasureSum > ROLE_CHSET_RATE)
		m_nTreasureSum = 1;

	tagNDBC_UpdateTreasureSum send;
	send.dwRoleID	= GetID();
	send.nSum	= m_nTreasureSum;
	g_dbSession.Send(&send, send.dwSize);
	
}

//--------------------------------------------------------------------------------
// ���ý�ɫ����������
//--------------------------------------------------------------------------------
VOID Role::SetTreasureSum(INT nSum)
{
	if (nSum > ROLE_CHSET_RATE)
		nSum = 1;

	m_nTreasureSum = nSum;
	tagNDBC_UpdateTreasureSum send;
	send.dwRoleID	= GetID();
	send.nSum	= m_nTreasureSum;
	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::StopMount()
{
	RemoveBuff(Buff::GetIDFromTypeID(MOUNT2_BUFF_ID), TRUE);
	RemoveBuff(Buff::GetIDFromTypeID(MOUNT_BUFF_ID), TRUE);
}

VOID Role::SetPassenger( Role* pHost )
{
	if (P_VALID(pHost))
	{
		SetBaseAttValue(ERA_Speed_Mount, pHost->GetAttValue(ERA_Speed_Mount));
		m_nSelfShape = GetAttValue(ERA_Shape);
		m_nMountShape = pHost->GetAttValue(ERA_Shape);
		SetAttValue(ERA_Shape, m_nMountShape);
	}
	else
	{
		INT nSelfShape =  m_nSelfShape;
		m_nSelfShape = m_nMountShape = 0;
		SetBaseAttValue(ERA_Speed_Mount, 10000);
		SetAttValue(ERA_Shape, nSelfShape);		
	}
	
	RecalAtt(TRUE);

	return ;
}


VOID Role::ContinueMount()
{
	PetSoul* pSoul = GetPetPocket()->GetPreparingPetSoul();
	if (P_VALID(pSoul) && GetPetPocket()->GetKeepMount() && pSoul->CanSetMount(TRUE))
	{
		TryAddBuff(this, g_attRes.GetBuffProto(MOUNT_BUFF_ID), NULL, NULL, NULL);
		GetPetPocket()->SetKeepMount(FALSE);
	}
	
}

void Role::BreakMount()
{
	if (IsInRoleStateAny(ERS_Mount | ERS_Mount2))
	{
		GetPetPocket()->SetKeepMount(TRUE);
		StopMount();
	}
}

//----------------------------------------------------------------------------------------
// ���ָ����װ������װ��������(GM�������)
//----------------------------------------------------------------------------------------
DWORD Role::AddSuit(DWORD dwSuitID, INT nQlty)
{
	TMap<DWORD, tagEquipProto*> mapEquip = g_attRes.GetEquipMap();
	mapEquip.ResetIterator();
	
	DWORD dwEquipID = 0;
	tagEquipProto* pEquipProto = NULL;

	while ( mapEquip.PeekNext(dwEquipID, pEquipProto) )
	{
		for(INT i=0; i<MAX_PEREQUIP_SUIT_NUM; ++i)
		{
			if(dwSuitID == pEquipProto->dwSuitID[i])
			{
				tagItem *pItem = ItemCreator::Create(EICM_GM, NULL, dwEquipID, 1,-1,0,0,10000,0,nQlty);
				if(!P_VALID(pItem)) return GT_INVALID;
				ASSERT( MIsEquipment(pItem->dwTypeID) );

				//ItemCreator::IdentifyEquip((tagEquip*)pItem, (EItemQuality)nQlty);
				GetItemMgr().Add2Bag(pItem, ELCID_GM_CreateItem, TRUE);

				//m_Suit.Add((tagEquip*)pItem, pItem->n16Index);
			}
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------
// ���ָ�����������������(GM�������) nType 0:���� 1:����
//----------------------------------------------------------------------------------------
DWORD Role::AddEquip(INT nType, INT nLevel, INT nQlty)
{
	TMap<DWORD, tagEquipProto*> mapEquip = g_attRes.GetEquipMap();
	mapEquip.ResetIterator();

	DWORD dwEquipID = 0;
	tagEquipProto* pEquipProto = NULL;

	while ( mapEquip.PeekNext(dwEquipID, pEquipProto) )
	{
		if(0 == nType)
		{
			if (pEquipProto->eType != 7) continue;
		}
		else
		{
			if (!(pEquipProto->eType == 8 || pEquipProto->eType == 9 || pEquipProto->eType == 10 || pEquipProto->eType == 11)) continue;
		}

		if(pEquipProto->byMinUseLevel == nLevel || pEquipProto->byMinUseLevel+5 == nLevel)
		{
			tagItem *pItem = ItemCreator::Create(EICM_GM, NULL, dwEquipID, 1,-1,0,0,10000,0,nQlty);
			if(!P_VALID(pItem)) return GT_INVALID;

			ASSERT( MIsEquipment(pItem->dwTypeID) );

			//ItemCreator::IdentifyEquip((tagEquip*)pItem, (EItemQuality)nQlty);
			GetItemMgr().Add2Bag(pItem, ELCID_GM_CreateItem, TRUE);
		}
	}

	return TRUE;
}

MoveData::EMoveRet Role::MoveAction( PFMoveAction2P pAction, Vector3& v1, Vector3& v2 )
{
	if(GetPetPocket()->IsRideAfter())
	{
		return MoveData::EMR_Invalid;
	}

	MoveData::EMoveRet emr = (GetMoveData().*pAction)(v1, v2);
	if (MoveData::EMR_Success == emr)
	{
		PetSoul* pSoul = GetPetPocket()->GetMountPetSoul();
		if (P_VALID(pSoul))
		{
			Role* pPassenger = pSoul->GetPassenger();
			if (P_VALID(pPassenger))
			{
				pPassenger->SynetRiderSpeed(this);
				MoveData::EMoveRet emrp = ((pPassenger->GetMoveData()).*pAction)(v1, v2);
				ASSERT(MoveData::EMR_Success == emrp);
			}
		}
	}

	return emr;
}

MoveData::EMoveRet Role::MoveAction( PFMoveAction1P pAction, Vector3& v )
{
	if(GetPetPocket()->IsRideAfter())
	{
		return MoveData::EMR_Invalid;
	}

	MoveData::EMoveRet emr = (GetMoveData().*pAction)(v);
	if (MoveData::EMR_Success == emr)
	{
		PetSoul* pSoul = GetPetPocket()->GetMountPetSoul();
		if (P_VALID(pSoul))
		{
			Role* pPassenger = pSoul->GetPassenger();
			if (P_VALID(pPassenger))
			{
				pPassenger->SynetRiderSpeed(this);
				MoveData::EMoveRet emrp = ((pPassenger->GetMoveData()).*pAction)(v);
				ASSERT(MoveData::EMR_Success == emrp);
			}
		}
	}

	return emr;
}

DWORD Role::CanCastMotion(DWORD dwActionID)
{
	const tagMotionProto* pMotion = g_attRes.GetMotionProto(dwActionID);
	//ϵͳ�жϴ��ڴ˵��˶���
	if (!P_VALID(pMotion) || pMotion->eMotionType != EMT1_Self)
	{
		return E_Motion_NotValid;
	}

	// ϵͳ�ж�Ŀ���Ƿ��ڷǳ�е״̬���������ϣ�����ʾ�����޷����н��������������������һ����
	if (IsInRoleStateAny(ERS_Combat /*| ERS_PK | ERS_PKEX*/ | ERS_Arena) )
	{
		return E_Motion_CanotActive;
	}
	return E_Motion_Success;
}

DWORD Role::CanCastMotion( Unit* pDest, DWORD dwActionID )
{
	if (!P_VALID(pDest))
	{
		return E_Motion_CanotActive;
	}

	if(!GetMotionInviteStatus(pDest->GetID()))
		return E_Motion_CanotActive;

	const tagMotionProto* pMotion = g_attRes.GetMotionProto(dwActionID);
	if (!P_VALID(pMotion))
	{
		return E_Motion_NotValid;
	}

	// ϵͳ�ж�Ŀ���Ƿ�Ϊ��ɫ�����ǹ��NPC���κηǽ�ɫ�Ŀ�ѡ��Ŀ�꣩���������ϣ�����ʾ�����޷����н��������������������һ����
	if (!pDest->IsRole())
	{
		return E_Motion_CanotActive;
	}
	Role* pDestRole = static_cast<Role*>(pDest);
	// ϵͳ�ж�Ŀ���Ƿ�Ϊ���ԣ��������ϣ�����ʾ�����޷����н��������������������һ����
	if(pDestRole->GetSex() && GetSex() || !pDestRole->GetSex() && !GetSex())
	{
		return E_Motion_SameSex;
	}
	// ϵͳ�ж�Ŀ���Ƿ��ڷǳ�е״̬���������ϣ�����ʾ�����޷����н��������������������һ����
	if (pDestRole->IsInRoleStateAny(ERS_Combat /*| ERS_PK | ERS_PKEX */| ERS_Arena | ERS_Stall) || IsInRoleStateAny(ERS_Combat | ERS_PK | ERS_PKEX | ERS_Arena | ERS_Stall))
	{
		return E_Motion_CanotActive;
	}
	// ϵͳ�ж�˫�����Ѻö��Ƿ�����
	//tagFriend* pFriend = GetFriendPtr(pDestRole->GetID());
	//if (!P_VALID(pFriend) || pFriend->dwFriVal < pMotion->dwFriendVal)
	//{
	//	return E_Motion_FriendValNotEnough;
	//}

	// ϵͳ�ж�˫����Ϊ����
	if(pDestRole->GetFriendPtr(GetID()) == NULL)
	{
		return E_Motion_FriendValNotEnough;
	}

	// ������ֱ�߾������X�����ӣ�������Զ����������ʧ�ܣ���ȡ����
	if (!IsInCombatDistance(*pDest, MAX_MOTION_DIST))
	{
		return E_Motion_DistanceNotEnough;
	}

	return E_Motion_Success;
	// Ŀ���ɫ����Ļ�ڵ���ȷ����ʾ�򣺡�####�������ɫ��������������###�����Զ������ƣ�����������/�ܾ���
	// ������ܾ�������������ʾ�����Է��ܾ����������󡣡�
	// ��30����δ����Ӧ�����Զ��ж�Ϊ�ܾ���
	// ��������ܡ�������뽻�������������̣�
	// ������������߿�ʼ�Զ��ƶ���
	// �����Զ��ƶ������н������ƶ����򽻻�����ʧ�ܣ���ȡ����
	// �����Զ��ƶ������з����������ƶ����򽻻�����ʧ�ܣ���ȡ����
	// ������ֱ�߾������X�����ӣ�������Զ����������ʧ�ܣ���ȡ����
	// ������������������Զ��ƶ������У����赲����߲�����������򣩣��򽻻�����ʧ�ܣ���ȡ����
	// ������һ���жϡ����̶�ͨ���󣬷������ƶ���������ͬһ����ͬһ�߶�λ�ã�Ȼ��ϵͳ�Զ���������Ϊͬһ����ͬʱ��ʼ���Ž������Զ�����
	// ���ڽ��������������̹����У�����������ɫ��������ɫ��������һ���ѷ���Ľ����������룬��֮ǰ�Ķ����������̱���ϡ�

}

DWORD Role::CastMotion( Unit* pDest, DWORD dwActionID )
{
	Role* pDestRole = static_cast<Role*>(pDest);
	
	tagNS_RoleStyleAction send;
	send.dwActionID = dwActionID;

	send.dwRoleID = GetID();
	send.dwDstRoleID = pDestRole->GetID();
	GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);

	return E_Motion_Success;
}

BOOL Role::GetMotionInviteStatus(DWORD dwRoleID)
{
	// �ǻ���������ͬ��
	if (dwRoleID == m_dwPartnerID)
		return TRUE;
	// û�л�飬���ҳ�ʱ
	else if (!GT_VALID(m_dwPartnerID) && !GT_VALID(m_nMotionCounter))
		return TRUE;
	// ���ǻ�飬����ʱ
	else
		return FALSE;
}

VOID Role::SetMotionInviteStatus(BOOL bSet, DWORD dwRoleID)
{
	if (GT_VALID(m_dwPartnerID) && dwRoleID != m_dwPartnerID)
		return ;

	if (bSet)
	{
		m_nMotionCounter = TICK_PER_SECOND * 60;
		m_dwPartnerID = dwRoleID;
	}
	else
	{
		m_nMotionCounter = GT_INVALID;
		m_dwPartnerID = GT_INVALID;
	}
}

VOID Role::UpdateMotionInviteState()
{
	if (m_nMotionCounter >= 0)
	{
		--m_nMotionCounter;
	}
	// time out, reset the partner
	else if (!GT_VALID(m_nMotionCounter))
	{
		m_dwPartnerID = GT_INVALID;
	}
}

INT Role::CanGather(Creature* pRes)
{
	// �����Դ�ͽ�ɫ
	Creature* pCreature	= pRes;	
	Role* pRole	= this;	

	if ( !P_VALID(pCreature) || !P_VALID(pRole) )
		return GT_INVALID;

	// �ж���Դ����ü���
	Skill* pGatherSkill = NULL;
	if ( pCreature->IsNatuRes() )
		pGatherSkill = pRole->GetSkill(GATHER_SKILL_MINING);
	else if ( pCreature->IsManRes() )
		pGatherSkill = pRole->GetSkill(GATHER_SKILL_HARVEST);
	if ( !P_VALID(pGatherSkill) )
		return GT_INVALID;

	// ����ɫ��������				
	ItemMgr& itemMgr = pRole->GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_UseItem_NotEnoughSpace;

	// ����Դ���빻�� 
	if (!pRole->IsInCombatDistance(*pCreature, pGatherSkill->GetOPDist()))
		return E_UseSkill_DistLimit;	

	// ��Դ�ѱ�ռ��
	if( pCreature->IsTagged() )
		return E_UseSkill_TargetLimit;

	

	return E_Success;
}

VOID Role::ModMountSpeed( INT nMod )
{
	ModAttModValue(ERA_Speed_Mount, nMod);
	RecalAtt();
}

VOID Role::SaveExp2DB()
{
	if( !m_bSaveExpDirect && m_nNeedSaveExpCounter > 0 )
	{
		++m_nSaveExpCounter;
		if( m_nSaveExpCounter % m_nNeedSaveExpCounter > 0 )
			return;
	}
	m_nSaveExpCounter = 0;

	tagNDBC_UpdateRoleAtt send;

	send.dwRoleID = GetID();
	send.nType = ERTSA_Exp;
	send.nValue = m_nCurLevelExp;

	g_dbSession.Send(&send, send.dwSize);
	m_bSaveExpDirect = FALSE;
}

VOID Role::SaveAttPoint2DB()
{
	tagNDBC_UpdateRoleAttPoint send;

	send.dwRoleID = GetID();
	send.nAttPointLeft = GetAttValue(ERA_AttPoint);
	IFASTCODE->MemCpy(send.nAttPointAdd, m_nAttPointAdd, sizeof(send.nAttPointAdd));

	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::SaveTalentPoint2DB(INT nIndex)
{
	tagNDBC_UpdateRoleTalentPoint send;

	send.dwRoleID = GetID();
	send.nTalentPoint = GetAttValue(ERA_TalentPoint);
	send.Talent = m_Talent[nIndex];
	send.nIndex = nIndex;
	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::SaveLevel2DB()
{
	tagNDBC_UpdateRoleAtt send;

	send.dwRoleID = GetID();
	send.nType = ERTSA_Level;
	send.nValue = m_nLevel;
	g_dbSession.Send(&send, send.dwSize);
}
// ʹ�ü�¼ʽ���ͷ�
DWORD Role::UseNoteTakingItemTransport(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseTransportMap);

	DWORD	dwErrorCode	=	E_Success;

	// �����Ʒ�Ƿ��ڱ�����
	tagItem* pItem = GetItemMgr().GetBagItem(pRecv->n64ItemID); 
	if( !P_VALID(pItem) ) 
	{
		dwErrorCode = E_UseItem_ItemNotExist;
	}
	else
	{
		/// ������
		if (P_VALID(GetGuildID()))
		{
			if(P_VALID(CGodMiracleMgr::GetInst()->IsInCaptureGodRange(GetGuildID())) &&
				IsGuildBattleMap(GetMapID()))
			{
				// ����а���,���Ҹð��������������У����ܴ�
				return E_UseItem_PosLimit;
			}
		}

		//���״̬����
		DWORD dwSelfStateFlag = GetStateFlag();
		if( (dwSelfStateFlag & pItem->pProtoType->dwStateLimit) != dwSelfStateFlag )
		{
			dwErrorCode = E_UseItem_SelfStateLimit;
		}
		else if(this->IsInRoleState(ERS_Stall))
		{
			dwErrorCode = E_UseItem_SelfStateLimit;
		}
		else if(pItem->pProtoType->eSpecFunc != EISF_TransportTool)
		{
			dwErrorCode = E_UseItem_SpecFunError;
		}
		else
		{
			// ������Ʒ��ʧ
			dwErrorCode = GetItemMgr().ItemUsedFromBag(pRecv->n64ItemID, 1, (DWORD)ELCLD_Item_Use);
			if(dwErrorCode == E_Success)
			{
				GotoNewMap(m_dwItemTransportMapID, m_fItemTransportX, m_fItemTransportY, m_fItemTransportZ);
			}
		}
	}

	return dwErrorCode;
	
}
// ʹ�ü�¼ʽ���ͷ���¼��ǰ����
DWORD Role::SetItemTransportPoint(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTransportMap);

	DWORD dwErrorCode = EST_SuccessSet;
	
	Map *pMap = GetMap();
	//����Ǹ��� �� ������
	if(pMap->GetMapInfo()->eType != EMT_Normal || 
		(pMap->GetMapInfo()->eType == EMT_Normal && pMap->GetMapInfo()->eNoarmalMapType == ENMT_TrainingField))
	{
		dwErrorCode = EST_InInstance;
	}

	// �����Ʒ�Ƿ��ڱ�����
	tagItem* pItem = this->GetItemMgr().GetBagItem(pRecv->n64ItemID); 
	if( !P_VALID(pItem) ) 
	{
		dwErrorCode = EST_LackNeedItem;
	}
	else
	{	
		if( this->IsInRoleState(ERS_PrisonArea) )
		{
			dwErrorCode = EST_SelfStateLimit;
		}
		else if(pItem->pProtoType->eSpecFunc != EISF_TransportTool)
		{
			dwErrorCode = EST_SpecFunError;
		}
	}

	// ����ڳ�ս������,��ʾ���ܼ�¼����
	if (IsCityStruggleMap(pMap->GetMapID()))
	{
		dwErrorCode = EST_InCityStruggleInstance;
	}

	//����ɹ�
	if(dwErrorCode == EST_SuccessSet)
	{
		//��¼����
		m_dwItemTransportMapID = pMap->GetMapID();
		m_fItemTransportX = GetCurPos().x;
		m_fItemTransportZ = GetCurPos().z;
		m_fItemTransportY = GetCurPos().y;
	}

	return dwErrorCode;
}


// Jason
BOOL Role::IsValidOfLessingLoong(VOID)
{
	return ROLE_MAX_LESSLEVEL_OF_LOONG > m_nLessingLevel;
}

// Jason 2009-12-7 �����͸�
VOID	Role::UpdateBlessingOfLoong(VOID)
{
	if( !IsValidOfLessingLoong  () ) //�����͸��Ѿ�������
	{
		if( ++m_n32UpdateOnlineReward >= TICK_PER_SECOND * 60 /** 10 */ )//��ս��ϵͳ���������gworld�еķ������������
		{
			m_n32UpdateOnlineReward = 0;
			CalTodayOnlineTime(TRUE);
			// ȡ������������������Ϣ����
			//if( m_nLessingExCounter )
			//	NotifyClientIniLessingOfLoong();
			//m_nLessingExCounter = 0;
		}
	}
	//----------------------------------------------------------------
	// ��ǰ����ʱ���¼�����
	// zxzhang 2009-12-29
	//----------------------------------------------------------------
	GetTitleMgr()->SigEvent( ETE_ONLINETIME, GT_INVALID ,GetCurrentOnlineTime()/3600 );
}

// Jason
VOID Role::NotifyClientToReceiveLessingOfLoong(VOID)
{
	if( !bLoongLessingOpen )
		return ;

	tagNS_SetNextBenediction msg;
	tagConfigLessingOfLoong * pCur/*, * pNext*/;
	msg.isEnd			= !IsValidOfLessingLoong();

	// ������������
	FLOAT fEarnRate		= GetEarnRate() / 10000.0f;
	if( fEarnRate < 0.5 )
		return;


	if( !msg.isEnd )
	{
		pCur = g_attRes.GetConfigInfoOfLessingLoong(m_nLessingLevel + 1);//Ĭ�����ݿ�������0

		if( !P_VALID( pCur ) )
			return;

		DWORD _now = GetCurrentOnlineTime();
		DWORD	lastLessingTime = m_timeLastLessing;
		if( 0 == lastLessingTime )
			lastLessingTime = _now;

		DWORD val = pCur->nExpireTime + (DWORD)m_timeLastLessing ;
		if( val <= _now ) //�����ˣ����߳���
			msg.nextServerTime = 0;
		else
			msg.nextServerTime	= val - _now;/*CalcTimeDiff( tagDWORDTime(val)  , _now )*/;

		if( fEarnRate < 1.0 )
		{
			msg.nItemTypeID		= GT_INVALID;
			msg.nItemCount		= 0;
		}
		else
		{
			msg.nItemTypeID		= pCur->dwRewardItemTypeID;
			msg.nItemCount		= pCur->nItemCount;
		}
		msg.n32Exprence		= pCur->nExperience * fEarnRate;
	}
	else //�����͸��Ѿ����ˣ�Ŀǰ�ǣ�20�Ρ�
	{
		// ����������������Ϣ���ͻ��ˣ��ͻ��ˣ����ڹ����ʱ��������ȡ�͸�����ʱ�������ڰѾ�����Ϣ����ȥ��
#if 1
		// �������ֻ���ڳ�ʼ��ʱ������
		tagDWORDTime now = g_world.GetWorldTime();

			const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
			if( P_VALID( pEffect ) )
			{
				//if( 0 == (DWORD)m_timeOnlineRewardPerDayRec )
					//msg.isEnd = m_dateLastLessingLoong.day == now.day && m_dateLastLessingLoong.month == now.month && m_dateLastLessingLoong.year == now.year;
				//else
				//	msg.isEnd = m_timeOnlineRewardPerDayRec.day == now.day && m_timeOnlineRewardPerDayRec.month == now.month && m_timeOnlineRewardPerDayRec.year == now.year;
				msg.isEnd = nOnlineRewardCount <= m_nLessingLevelEx;
				if(msg.isEnd)
				{
					msg.nextServerTime	= 0;
					msg.nItemTypeID		= GT_INVALID;
					msg.nItemCount		= 0;
					msg.n32Exprence		= 0;
				}
				else
				{
					DWORD tem = CalTodayCurrentTime();
					OnlieRewardParam::tagLessingConfig tagTem;

					if( g_OnlineRewardParam.GetParam(m_nLessingLevelEx,tagTem) )
					{
						// �����ж�
						DWORD val = tagTem.dwExpireTime + (DWORD)m_timeLastLessing;
						msg.nextServerTime	= val > tem ? val - tem : 0;
						//if( msg.nextServerTime == 0 )
						//	m_bOnlineRewardPromptFlag = TRUE;
						if( fEarnRate < 1.0 )
						{
							msg.nItemTypeID		= GT_INVALID;
							msg.nItemCount		= 0;
						}
						else
						{
							msg.nItemTypeID		= tagTem.dwRewardItemID;
							msg.nItemCount		= tagTem.nItemCount;
						}
						msg.n32Exprence		= CalOnlineRewardExp(GetLevel(),pEffect->nExpLevelUp * tagTem.fAddExpPct) * fEarnRate;
					}
				}
			}
#endif
	}

	SendMessage  (&msg,msg.dwSize);
}

// Jason
VOID Role::NotifyClientIniLessingOfLoong(VOID)
{
	if( 0 == dwOfflineItemType )
	{
		nOnlineRewardCount = g_OnlineRewardParam.GetCount();
		//g_OnlineRewardParam.GetParam  (dwOfflineItemType,dwOfflineItemCount,OFFLINE_TIMEOUT/*,bLoongLessingOpen*/);
		dwOfflineItemType = 1;
		bLoongLessingOpen = OnlieRewardParam::LoongLessingSwitch();
	}
	// ������������
	FLOAT fEarnRate		= GetEarnRate() / 10000.0f;
	if( fEarnRate < 0.5 )
		return;

	tagDWORDTime now = g_world.GetWorldTime();
	if( !IsValidOfLessingLoong() && (m_dateLastLessingLoong.day != now.day || m_dateLastLessingLoong.month != now.month || m_dateLastLessingLoong.year != now.year ) && nOnlineRewardCount > m_nLessingLevelEx )
		m_bOnlineRewardPromptFlag = TRUE;
	else if( !IsValidOfLessingLoong() ) //��ֹ������ȡ�������ˣ������ͻ��˷���ʼ����Ϣ��û�з�ʱ�䣬�ͻ��˻���ʾ���ֵ��
	{
		return;
	}

	if( !bLoongLessingOpen )
		return ;

	tagNS_LoongBenedictionInit msg;
	if( IsValidOfLessingLoong() )
	{
		msg.nType = 0;
	}
	else
	{
		//if( m_nLessingLevelEx == 0 || ( m_timeOnlineRewardPerDayRec.day != m_LoginTime.day || m_timeOnlineRewardPerDayRec.month != m_LoginTime.month || m_timeOnlineRewardPerDayRec.year != m_LoginTime.year ) )
		//{
		//	m_nLessingLevelEx = 0;
		//	m_timeLastLessing = 0;
		//	//if( m_timeOnlineRewardPerDayRec.day != m_LoginTime.day || m_timeOnlineRewardPerDayRec.month != m_LoginTime.month || m_timeOnlineRewardPerDayRec.year != m_LoginTime.year )
		//	//	m_nCurrentDayOnlineTime = 0;
		//}
		//m_timeLastLessing = CalTodayCurrentTime();
		msg.nType	= 1;
	}
	SendMessage( &msg, msg.dwSize );
	NotifyClientToReceiveLessingOfLoong();
}
// Jason
DWORD Role::ReceiveLessingOfLoong(VOID)
{
	if( !bLoongLessingOpen )
	{
		tagNS_GetLoongBenediction msg;
		msg.dwErrorCode = ELBD_No_Benediction;
		msg.nExprence	= 0;
		msg.nItemCount	= 0;
		msg.nItemTypeID	= GT_INVALID;

		SendMessage  (&msg,msg.dwSize);

		//tagNS_SetNextBenediction msg1;
		//msg1.isEnd = TRUE;
		//msg1.nextServerTime	= 0;
		//msg1.nItemTypeID		= GT_INVALID;
		//msg1.nItemCount		= 0;
		//msg1.n32Exprence		= 0;
		//SendMessage  (&msg1,msg1.dwSize);
		return 0xFF;
	}


	tagNS_GetLoongBenediction msg;
	DWORD re = 0;

	// ������������
	FLOAT fEarnRate		= GetEarnRate() / 10000.0f;
	if( fEarnRate < 0.001 )
	{
		msg.dwErrorCode = ELBD_No_Benediction;
		msg.nExprence	= 0;
		msg.nItemCount	= 0;
		msg.nItemTypeID	= GT_INVALID;

		SendMessage  (&msg,msg.dwSize);
		return ELBD_No_Benediction;
	}

	// �ﵽ��ߴΣ�ֹͣ�û
	if( IsValidOfLessingLoong() )
	{
		do 
		{
			tagConfigLessingOfLoong *pCur = g_attRes.GetConfigInfoOfLessingLoong(m_nLessingLevel + 1);
			if( !P_VALID( pCur ) )
			{
				re = ELBD_No_LessingProto;
				IMSG(_T("Caution: Lessing of loong , no lessing prototype.\n"));
				return re;
				//break;
			}

			DWORD _now = GetCurrentOnlineTime();
			DWORD	lastLessingTime = m_timeLastLessing;

			// �����ж�
			DWORD val = pCur->nExpireTime + (DWORD)m_timeLastLessing ;
			if( val > _now )
			{
				re = ELBD_Time_NotArrive;
				break;
			}// �����ڻ��߳��ڣ�Ӧ�ô���

			if( fEarnRate >= 1.0 && pCur->nItemCount > 0 && P_VALID(pCur->dwRewardItemTypeID) )
			{
				//do{
				if( ( MIsEquipment(pCur->dwRewardItemTypeID) && ( GetItemMgr().GetBagFreeSize() < pCur->nItemCount ) ) || 
					( !MIsEquipment(pCur->dwRewardItemTypeID) && ( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE || GetItemMgr().GetBagFreeSize() < 1 )/*( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE  || 
					)*/
				)
				)
				{
					re = ELBD_Bag_NotEnoughSpace;

					msg.dwErrorCode = re;
					msg.nExprence	= 0;
					msg.nItemCount	= 0;
					msg.nItemTypeID	= GT_INVALID;

					SendMessage  (&msg,msg.dwSize);

					return re;
				}

					tagItem *pItem = NULL;
					pItem = ItemCreator::CreateByProduce(EICM_LessingLoong,  GetID(), 
						pCur->dwRewardItemTypeID,(EItemQuality)pCur->nQlty,0,0,pCur->nItemCount/*,GetID()*/ );

					if( !P_VALID(pItem) )
					{
						re = ELBD_Item_Create_Failed;
						IMSG(_T("Caution: Lessing of loong , create item failed.\n"));
						return re;
					}


					// �ж��Ƿ�Ϊʱװ���������
					if(P_VALID(pItem) && MIsFashion(pItem))
					{
						ItemCreator::IdentifyEquip((tagEquip*)pItem);
					}


					if( P_VALID(pItem) )
					{
						re = GetItemMgr().Add2Bag(pItem, ELCID_LessingLoong, TRUE);
					}
					else
					{
						re = ELBD_Item_Create_Failed;
						IMSG(_T("Caution: Lessing of loong , bag not enough space.\n"));
						return re;
						//break;
					}
				//}while(0);
			}

			m_timeLastLessing = /*g_world.GetWorldTime()*/GetCurrentOnlineTime();
			++m_nLessingLevel;
			//m_bFiredOnceOfLessingLoong = FALSE;

			if( E_Item_MaxHold == re )
			{
				re = ELBD_Bag_NotEnoughSpace;
				//break;
				IMSG(_T("Caution: Lessing of loong , bag not enough space.\n"));
			}

			// ���Ӿ��飬������پ���ġ�
			if( pCur->nExperience > 0 )
				ExpChange( INT64(pCur->nExperience * fEarnRate) );

			msg.dwErrorCode = re;
			msg.nExprence	= pCur->nExperience * fEarnRate;
			if( fEarnRate < 1.0 )
			{
				msg.nItemCount	= 0;
				msg.nItemTypeID	= GT_INVALID;
			}
			else
			{
				msg.nItemCount	= pCur->nItemCount;
				msg.nItemTypeID	= pCur->dwRewardItemTypeID;
			}

			SendMessage  (&msg,msg.dwSize);

			m_dateLastLessingLoong = g_world.GetWorldTime  ();

			if( IsValidOfLessingLoong() )
				NotifyClientToReceiveLessingOfLoong();
			else
			{
				tagNS_SetNextBenediction msg1;
				msg1.isEnd = TRUE;
				msg1.nextServerTime	= 0;
				msg1.nItemTypeID		= GT_INVALID;
				msg1.nItemCount		= 0;
				msg1.n32Exprence		= 0;
				SendMessage  (&msg1,msg1.dwSize);
			}

			return re;
		} while (0);
	}
	else //���������͸������ƣ�����ÿ�����߽����������͸���ȡ���һ�εĵڶ��쿪ʼ��
	{
		//re = ELBD_No_Benediction;
		// ÿ�����߳���4Сʱ����
		CalTodayOnlineTime();
		const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
		if( !P_VALID( pEffect ) )
		{
			return ELBD_No_LessingProto;
		}
		OnlieRewardParam::tagLessingConfig tagTem;

		if( !g_OnlineRewardParam.GetParam(m_nLessingLevelEx,tagTem) )
			return ELBD_No_LessingProto;

		INT64 levelExp = pEffect->nExpLevelUp;
		INT nItemTypeID		= tagTem.dwRewardItemID;
		INT64 nExprence		= CalOnlineRewardExp(GetLevel  (),levelExp * tagTem.fAddExpPct);//( (double)150 / (( GetLevel  () + 10 )^2) ) * levelExp;
		if( nExprence < 0 )
			return ELBD_No_LessingProto;

		tagDWORDTime now = g_world.GetWorldTime();
		if( (m_dateLastLessingLoong.day != now.day || m_dateLastLessingLoong.month != now.month || m_dateLastLessingLoong.year != now.year ) && 
			//( (DWORD)m_timeOnlineRewardPerDayRec == 0 || m_timeOnlineRewardPerDayRec.day != now.day || m_timeOnlineRewardPerDayRec.month != now.month || m_timeOnlineRewardPerDayRec.year != now.year ) && 
			CalTodayCurrentTime() >= tagTem.dwExpireTime + m_timeLastLessing )
		{
				//if( levelExp > 0 )
				{


					if( fEarnRate >= 1.0 && ( MIsEquipment(nItemTypeID) && ( GetItemMgr().GetBagFreeSize() < 1 ) ) || 
						( !MIsEquipment(nItemTypeID) && ( GetItemMgr  ().CanAddMaxHoldItem(nItemTypeID,1) == FALSE || GetItemMgr().GetBagFreeSize() < 1 ))
					  )
					{
						re = ELBD_Bag_NotEnoughSpace;

						msg.dwErrorCode = re;
						msg.nExprence	= 0;
						msg.nItemCount	= 0;
						msg.nItemTypeID	= GT_INVALID;

						SendMessage  (&msg,msg.dwSize);

						return re;
					}

					INT itemcount = 1,itemtotal = 0;
					for( INT i = 0; fEarnRate >= 1.0 && i < tagTem.nItemCount ; )
					{
						itemcount = 1;
						if( !MIsEquipment(nItemTypeID) )
						{
							const tagItemProto * pProto = g_attRes.GetItemProto(nItemTypeID);
							if( P_VALID(pProto) )
							{
								if( pProto->n16MaxLapNum > 1 )
								{
									itemcount = tagTem.nItemCount - i;
									if( itemcount > pProto->n16MaxLapNum )
										itemcount = pProto->n16MaxLapNum;
								}
							}
						}
						tagItem *pItem = NULL;
						pItem = ItemCreator::Create( EICM_LessingLoong,  GetID(),nItemTypeID,itemcount );

						if( !P_VALID(pItem) )
						{
							re = ELBD_Item_Create_Failed;
							IMSG(_T("Caution: Lessing of loong , create item failed.\n"));
							//return re;
							break;
						}

						if( P_VALID(pItem) )
						{
							re = GetItemMgr().Add2Bag(pItem, ELCID_LessingLoong, TRUE);
						}
						else
						{
							re = ELBD_Item_Create_Failed;
							IMSG(_T("Caution: Lessing of loong , bag not enough space.\n"));
							//return re;
							break;
						}
						i += itemcount;
						itemtotal = i;
					}

					// ���Ӿ��飬������پ���ġ�
					if( nExprence > 0 )
						ExpChange(INT64(nExprence * fEarnRate));

					m_timeLastLessing = CalTodayCurrentTime();
					++m_nLessingLevelEx;

					if( m_nLessingLevelEx < nOnlineRewardCount )
						m_nLessingExCounter = 1;
					else
						m_nLessingExCounter = 0;

					tagNS_GetLoongBenediction msg2;

					msg2.dwErrorCode = re;
					msg2.nExprence	= nExprence*fEarnRate;
					if( fEarnRate < 1.0 )
					{
						msg2.nItemCount	= 0;//tagTem.nItemCount;
						msg2.nItemTypeID	= GT_INVALID;
					}
					else
					{
						msg2.nItemCount	= itemtotal;//tagTem.nItemCount;
						msg2.nItemTypeID	= nItemTypeID;
					}

					SendMessage  (&msg2,msg2.dwSize);

					m_timeOnlineRewardPerDayRec = now;

					if( nOnlineRewardCount > m_nLessingLevelEx )
						m_bOnlineRewardPromptFlag = TRUE;
					//tagNS_SetNextBenediction msg1;
					//msg1.isEnd			= TRUE;
					//msg1.nItemTypeID		= GT_INVALID;
					//msg1.nItemCount		= 0;
					//msg1.n32Exprence		= 0;
					//SendMessage  (&msg1,msg1.dwSize);
					return re;
				}
		}
		else if( (m_dateLastLessingLoong.day != now.day || m_dateLastLessingLoong.month != now.month || m_dateLastLessingLoong.year != now.year ) )// ʱ�䲻��
		{
			DWORD t = CalTodayCurrentTime() ;
			if( t < tagTem.dwExpireTime + m_timeLastLessing )
				t = tagTem.dwExpireTime + m_timeLastLessing - t;
			else
				t = 0;
			tagNS_SetNextBenediction msg1;
			msg1.nextServerTime = t;
			msg1.isEnd			= FALSE;
			if( fEarnRate < 1.0 )
			{
				msg1.nItemTypeID		= GT_INVALID;
				msg1.nItemCount		= 0;
			}
			else
			{
				msg1.nItemTypeID		= nItemTypeID;
				msg1.nItemCount		= tagTem.nItemCount;
			}
			msg1.n32Exprence		= nExprence * fEarnRate;
			SendMessage  (&msg1,msg1.dwSize);
			return re;
		}
	}

	msg.dwErrorCode = re;
	msg.nExprence	= 0;
	msg.nItemCount	= 0;
	msg.nItemTypeID	= GT_INVALID;

	SendMessage  (&msg,msg.dwSize);
	return re;
}


DWORD Role::GetCurrentOnlineTime(VOID)
{
	DWORD re = 0;
//������ role_save.cpp line : 35	
	if(m_LogoutTime < m_LoginTime)
	{
		re += CalcTimeDiff(g_world.GetWorldTime(), m_LoginTime);
	}
	else
	{
		re += CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	}
	if( m_nOnlineTime == GT_INVALID )
		return re;
	re += m_nOnlineTime;

	

	return re;
}
DWORD Role::CalTodayCurrentTime()
{
	//DWORD re = 0;
	////������ role_save.cpp line : 35
	//if( 0 == m_timeCurrentDayTime )
	//	m_timeCurrentDayTime = m_LogoutTime > m_LoginTime ? m_LogoutTime : m_LoginTime;
	//if( m_timeCurrentDayTime.day != m_LoginTime.day )
	//{
	//	tagDWORDTime now = g_world.GetWorldTime();
	//	m_timeCurrentDayTime = now;
	//	m_timeCurrentDayTime.hour = 0;
	//	m_timeCurrentDayTime.min	= 0;
	//	m_timeCurrentDayTime.sec	= 0;
	//	
	//}
	//re += CalcTimeDiff(g_world.GetWorldTime(), m_timeCurrentDayTime);
	//re += m_dwCurrentDayOnlineTime;//m_nCurrentDayOnlineTime;

	return m_dwCurrentDayOnlineTime;

//	return re;
}
/**
 * ���㵱���ۼ�����ʱ��
 * \author	: Jason
 * \date	: 2010-1-8
 *			: 2010-3-11
 * \return  �����ۼ�����ʱ������������ˣ����޸��ۼ�ֵ�����й����������
 */
DWORD Role::CalTodayOnlineTime(BOOL notify)
{
	tagDWORDTime now = g_world.GetWorldTime  ();
	DWORD delta = 0;

	if( 0 == (DWORD)m_timeLastCalOnlineTime )
		m_timeLastCalOnlineTime = m_LoginTime;

	if( now.day == m_timeOneDayFirstLogined.day && m_timeOneDayFirstLogined.month == now.month && m_timeOneDayFirstLogined.year == now.year ) //����
	{
		delta = CalcTimeDiff( now , m_timeLastCalOnlineTime );
	}
	else // ��������1��
	{
		// ��һ�ο�������
		if(m_timeLastCalOnlineTime.day != now.day || m_timeLastCalOnlineTime.month != now.month || m_timeLastCalOnlineTime.year != now.year )
		{
			//tagDWORDTime t = DecreaseTime(now,/*deltaDay*/1 * 24 * 60 * 60);
			tagDWORDTime tem(0,0,0,now.day ,now.month,now.year);
			delta = CalcTimeDiff( now , tem );
			m_dwCurrentDayOnlineTime = 0;	//�����ۼ�ʱ������
			//m_nCurrentDayOnlineTime = 0;
			m_bOnlineRewardPromptFlag = TRUE;
			// ���ӹ���㷢�ͳ�ʼ����Ϣ
			if( !IsValidOfLessingLoong() )
			{
				m_timeLastLessing = 0; // ÿ�ս���ʱ����������ֶΣ���������㡣
				m_nLessingLevelEx = 0;
				if( notify )
				{
					FLOAT fEarnRate		= GetEarnRate() / 10000.0f;
					// ��������жϣ��Ƿ�ֹ��½�󣬳������ߵ���ң���������tagNS_LoongBenedictionInit��Ϣ���ͻ��ˣ���Ϊ����ҵ�½��ʼ��ʱ���Ѿ����͹�һ��
					if( (fEarnRate > 0.001) && (m_LoginTime.day != now.day || m_LoginTime.month != now.month || m_LoginTime.year != now.year) )
					{
						tagNS_LoongBenedictionInit msg1;
						msg1.nType	= 1;
						SendMessage( &msg1, msg1.dwSize );
						NotifyClientToReceiveLessingOfLoong();
					}
				}
			}

			if( GetAttValue(ERA_Soar) == ESV_SoaringUp )
			{
				m_nSoaringSkillLearnTimes = m_nCLearnSoaringSkillTotalTimes;
				//m_timeLearnSoaringSkill = now;
				tagNS_ResetSoarSkillLearnTimes send_msg;
				send_msg.nCurrLeftNum = GetPracticeSoaringSkillTimes();
				SendMessage(&send_msg,send_msg.dwSize);
			}

			// �����ۼ�ʱ������ʱҲ��ƣ��ֵ��0
			if ( GetAttValue(ERA_Weary) > 0)
			{
				SetAttValue(ERA_Weary, 0);
			}
			ResetPulseCanLearnNum();
			// ������죬����ÿ�ս�ȡ��֮�л�������������
			ResetYellowListAcceptTimes();

			// ������죬���ý�ɫʹ�þ��鵤����
			ResetExpPilularUseTimes();

			// �������, ������ҵ� ʥ��ÿ��������������
			ResetHolyToDayEatNum();
		}
		else // ������ս������������һֱû�����ߣ��ǵ�һ�ο�������
		{
			delta = CalcTimeDiff( now , m_timeLastCalOnlineTime );
		}
	}
	m_dwCurrentDayOnlineTime += delta;
	m_timeLastCalOnlineTime = now;
	
	if( notify && !IsValidOfLessingLoong() && (m_dateLastLessingLoong.day != now.day || m_dateLastLessingLoong.month != now.month || m_dateLastLessingLoong.year != now.year ) )
	{
		//�Ƿ�֪ͨ��ȡ���������͸�
		DWORD _now = CalTodayCurrentTime();
		DWORD	lastLessingTime = m_timeLastLessing;
		OnlieRewardParam::tagLessingConfig tagTem;

		if( nOnlineRewardCount <= m_nLessingLevelEx )
			m_bOnlineRewardPromptFlag = FALSE;
		else 
		{
			if( g_OnlineRewardParam.GetParam(m_nLessingLevelEx,tagTem) )
			{
				// �����ж�
				FLOAT fEarnRate		= GetEarnRate() / 10000.0f;
				DWORD val = tagTem.dwExpireTime + (DWORD)lastLessingTime ;
				if( fEarnRate > 0.001 && val <= _now ) // ���ڻ��߳��ڣ�Ӧ�ô���
				{
					m_bOnlineRewardPromptFlag = FALSE;
					tagNS_LoongBenedictionInit msg1;
					msg1.nType	= 1;
					SendMessage( &msg1, msg1.dwSize );
					NotifyClientToReceiveLessingOfLoong();
				}
				else
				{
					if( fEarnRate > 0.001 && m_bOnlineRewardPromptFlag )
					{
						m_bOnlineRewardPromptFlag = FALSE;
						tagNS_LoongBenedictionInit msg1;
						msg1.nType	= 1;
						SendMessage( &msg1, msg1.dwSize );
						NotifyClientToReceiveLessingOfLoong();
					}
				}
			}
		}
	}

	return m_dwCurrentDayOnlineTime;
}

DWORD Role::ProcessGoldStoneMsg( INT64 GoldStoneItemID/*���ʯ*/, INT64 destItemID/*������Ʒ*/ )
{
	tagItem *pItem = GetItemMgr().GetBagItem( GoldStoneItemID ); 
	tagItem *pDestItem = GetItemMgr().GetBagItem( destItemID ); 
	if( !P_VALID( pItem ) || !P_VALID( pDestItem ) )
	{
		//IMSG(_T("Caution: Gold Stone , no such item."));
		return E_GoldStone_SrcItemNotExist;
	}

	if( EISF_BeGoldStone == pItem->pProtoType->eSpecFunc )
	{
		DWORD dwTypeID = pDestItem->pProtoType->dwTypeID;
		BOOL bLockState = pDestItem->bLock;
		DWORD dwNewStoneTypeID = 0;
		tagPhilosophersStoneConfig * pConfig = g_attRes.GetConfigInfoOfPhilosophersStone(pItem->pProtoType->nSpecFuncVal1,dwTypeID);
		if( P_VALID( pConfig )  )//û�������ļ���Ҫ��һ��errlog
		{
			DWORD dwDestStoneTypeID = pConfig->GetStoneTypeIDByPct(IUTIL->Rand() % 10000);
			if( dwDestStoneTypeID == GT_INVALID )
				return E_GoldStone_OtherError;

			if( GetItemMgr().GetBagFreeSize() < 1 )
				return E_GoldStone_BagIsFull;

			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript) && P_VALID(pItem->pProtoType))
			{
				DWORD dwID = pRoleScript->OnCutStone(this, pItem->pProtoType->dwTypeID/*���ʯ*/, dwTypeID/*������Ʒ*/, dwDestStoneTypeID/*������Ʒ*/);
				dwDestStoneTypeID = dwID;
			}

			DWORD re = 0;
			tagItem *pNewItem = NULL;
			//0��ʾ�ֽ�ɹ���1��ʾ�������㣬��Ҫɾ������
			if(0 != dwDestStoneTypeID && 1 != dwDestStoneTypeID)
			{
				GetItemMgr  ().DelFromBag(GoldStoneItemID,ELCID_PhilosophersStone,1);
				pNewItem = ItemCreator::Create(EICM_GoldStone,  GT_INVALID/*GetID()*/, dwDestStoneTypeID/*,1,GetID()*/ );
			}
			else if(0 == dwDestStoneTypeID)
			{
				GetItemMgr  ().DelFromBag(GoldStoneItemID,ELCID_PhilosophersStone,1);
				GetItemMgr().DelFromBag  (destItemID,ELCID_PhilosophersStone,1);
				return E_Success;
			}
			else if(1 == dwDestStoneTypeID)
			{
				return E_GoldStone_BagIsFull;
			}

			if( P_VALID(pNewItem) )
			{
				// �����״̬
				if (MIsEquipment(pNewItem->pProtoType->dwTypeID) && TRUE == bLockState)
				{
					pNewItem->bLock = TRUE;
				}
				
				if( P_VALID(pDestItem) && P_VALID(pDestItem->pProtoType) && MIsFaBao(pDestItem) && pNewItem->pProtoType->eSpecFunc == EISF_FairyPowder )
				{
					if( pNewItem->pProtoType->nSpecFuncVal1 == GT_INVALID )
					{
						//tagFabao * pMyFabao = (tagFabao*)pDestItem;
						//FLOAT fDelta = 0.2 * (100 - pMyFabao->n16NativeIntelligence) / 50;
						//INT nMin = pMyFabao->n16NativeIntelligence * (1 - fDelta);
						//INT nMax = pMyFabao->n16NativeIntelligence * (1 + fDelta);
						//pNewItem->dwSpecVal1	= IUTIL->RandomInRange(nMin,nMax);
						pNewItem->dwSpecVal1	= IUTIL->RandomInRange(50,100);
					}
					else
						pNewItem->dwSpecVal1	= pNewItem->pProtoType->nSpecFuncVal1;
				}
				
				re = GetItemMgr().Add2Bag(pNewItem, ELCID_PhilosophersStone, TRUE);
				GetItemMgr().DelFromBag  (destItemID,ELCID_PhilosophersStone,1);
			
				return E_GoldStone_Success;
			}
			else
			{
				re = E_GoldStone_CreateDestItemFail;
				//IMSG(_T("Caution: Gold Stone , create new item false.\n"));
				return re;
				//break;
			}		
		}
		else
		{
			//IMSG(_T("Caution: Gold Stone , philosopher's stone missing match.\n"));
			return E_GoldStone_MissingMatch;
		}
	}
	// else ���ǵ��ʯ

	//IMSG(_T("Caution: Gold Stone , it's not philosopher's stone.\n"));

	return E_GoldStone_OtherError;
}

DWORD Role::VocationCheck( DWORD dwVocationLimit )
{
	// Ĭ��ֵ��0����ʾ��ְҵ����
	if (0 == dwVocationLimit)
	{
		return E_Success;
	}

	INT nTmpClass =  1 << ( m_eClass - 1 );
	//INT nTmpClassEx = 0;
	//INT nTmp = 0;

	//if ( (INT)m_eClassEx != (INT)EHV_Base )
	//{
	//	nTmpClassEx = 1 << ( m_eClassEx + 8 );
	//}

	//nTmp = nTmpClass + nTmpClassEx;

	if ( !( nTmpClass & dwVocationLimit ) )
		return E_SystemError;

	return E_Success;
}

DWORD Role::CanPickUpItem( INT64 n64GroundID, BOOL bRole /*= TRUE*/ )
{
	MTRANS_ELSE_RET(pMap, GetMap(), Map, E_Loot_Map_Not_Found);
	MTRANS_ELSE_RET(pGroundItem, pMap->GetGroundItem(n64GroundID), tagGroundItem, E_Loot_Item_Not_Found);

	if(pGroundItem->dwTeamID != GT_INVALID && !P_VALID(g_groupMgr.GetTeamPtr(pGroundItem->dwTeamID))) // �����ɢ��
		pGroundItem->dwTeamID = GT_INVALID;

	// ��ɫid��ͬ �Ҳ��ڶ�����
	if(pGroundItem->dwTeamID == GT_INVALID && pGroundItem->dwOwnerID != GT_INVALID && GetID() != pGroundItem->dwOwnerID)
		return E_Loot_Belong_To_You;

	if(bRole)
	{
		//�����Һ���Ʒ�ľ��� ʰȡ��Χ���Զ�ʰȡ��Χ�ǲ�һ����
		if ( abs(pGroundItem->vPos.x - GetCurPos().x) > X_DEF_AUTO_PICKUP_DIST 
			|| abs(pGroundItem->vPos.z - GetCurPos().z) > X_DEF_AUTO_PICKUP_DIST )
			return E_Loot_Pick_Up_Too_Far;
	}

	if(TYPE_ID_MONEY == pGroundItem->dwTypeID)
	{
		// ���鲻��
		if(pGroundItem->dwTeamID != GT_INVALID && GetTeamID() != pGroundItem->dwTeamID)
		{
			return E_Loot_Belong_To_You;
		}
		// ��ɫ���ж��Ѿ�������
	}
	else if(pGroundItem->dwTeamID != GT_INVALID) // ��ӵ��ߵ�ʰȡ����
	{
		if(GetTeamID() != pGroundItem->dwTeamID)
		{
			return E_Loot_Belong_To_You;
		}
		else if(pGroundItem->dwOwnerID != GetID() && pGroundItem->dwOwnerID != GT_INVALID)
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pGroundItem->dwOwnerID);
			if(P_VALID(pRole) && pRole->GetTeamID() == pGroundItem->dwTeamID && pRole->IsInTeamPickupRank(GetMapID(), pGroundItem->vPos))
			{
				if(pRole->CanPickUpItem(n64GroundID, FALSE) == E_Success) return E_Success;
			}			
			// ��Ҳ��ڶ������� ��ʰȡ������ ����������ж�
		}
	}

	//�����Ƿ��пռ�
	if ( TYPE_ID_MONEY != pGroundItem->dwTypeID && GetItemMgr().GetBagFreeSize() <= 0)
		return E_Loot_BAG_NOT_ENOUGH_SPACE;

	if (P_VALID(pGroundItem->pItem) &&
		P_VALID(pGroundItem->pItem->pProtoType) &&
		pGroundItem->pItem->pProtoType->eSpecFunc == EISF_TreasureSeizer &&
		GetMoveData().IsWillSwim())
	{
		return E_Loot_Swim_Can_Not_Pickup_BaoXiang;
	}

	const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(pGroundItem->dwTypeID);
	if (P_VALID(pItemScript))
	{
		DWORD dwErrorCode = pItemScript->CanPickUpItem(pGroundItem->dwTypeID, GetID());
		if (dwErrorCode != E_Success)
			return dwErrorCode;
	}

	// ���ѵ���Ŀ
	BOOL bCan = GetItemMgr().CanAddMaxHoldItem(pGroundItem->dwTypeID, pGroundItem->nNum);

	return bCan ? E_Success : E_Item_MaxHold;
}

VOID Role::UpdateEquipValue()
{
	// ���ģ�    װ��ʵ��ֵ = [ʹ�õȼ�*3+��ƷƷ��*20+Ǳ��ֵ���ֵ/10+��ʯ�ȼ�*��Ӧ��Ȩ*3+����Ƕ����*5+��(����ǿ���ȼ�^2)/30 + 2*��Ƕ���� + 2*��ǰ��ӡ�ȼ� + 3*װ����ǰ�����ȼ���*5]*2
	// ʥħ�ģ� װ��ʵ��ֵ = ʹ�õȼ�*2+��ʯ�ȼ�*��Ӧ��Ȩ*6+ǿ���ȼ�^2*2 + 6*װ������^2 +��ǰ׷�ӵȼ�^2*5+��ƷƷ��*50
	// ʥħ��װ��ʵ��ֵ���� װ���ɳ� ʥ��:      +	ʥ�������ܺ�/100	+װ���ɳ��ȼ�^2*4+��Ʒ��

	BYTE byIndex = 0;
	BYTE byMaxIndex = EEP_MaxEquip;
	DWORD dwEquipValue = 0;
	INT nWeightedlValue[] = {1,1,
		2,1,
		3,1, 
		4,2,
		5,3,
		6,3,
		7,4,
		8,6,
		9,8,
		10,10};

	for(; byIndex < byMaxIndex; ++byIndex)
	{	
		tagEquip* pEquipInfo = this->GetItemMgr().GetEquipBarEquip((INT16)byIndex);
		if (!P_VALID(pEquipInfo)) continue;

		INT nTmpValue = 0;

		// ������������������Ƿ����Ļ�Ҫ���⴦��
		if (P_VALID(pEquipInfo->pEquipProto) && pEquipInfo->pEquipProto->eType == EIT_FaBao)
			continue;
		else
			nTmpValue += pEquipInfo->equipSpec.byMinUseLevel * 2;

		INT nTmpGem = 0;
		for (INT i=0; i<MAX_EQUIPHOLE_NUM; ++i) // ���λ�ø�����Ƕ��װ���ϵı�ʯ
		{
			DWORD dwID = pEquipInfo->equipSpec.dwHoleGemID[i];
			tagItemProto* pTmp = g_attRes.GetItemProto(dwID);
			if (!P_VALID(pTmp)) continue;
			
			int nStoneLevel = pTmp->byLevel;
			if (nStoneLevel < 1)
				nStoneLevel = 1;

			if (nStoneLevel > 10)
				nStoneLevel = 10;

			nTmpGem += (nStoneLevel*nWeightedlValue[(nStoneLevel-1)*2+1]*6);
		}

		nTmpValue += nTmpGem;

		nTmpValue += (pEquipInfo->equipSpec.byConsolidateLevel) * (pEquipInfo->equipSpec.byConsolidateLevel) * 2;

		nTmpValue += (pEquipInfo->equipSpec.byHoleNum) * (pEquipInfo->equipSpec.byHoleNum) * 6;

		nTmpValue += (pEquipInfo->equipSpec.byBrandLevel*pEquipInfo->equipSpec.byBrandLevel) * 5;

		nTmpValue += pEquipInfo->equipSpec.byQuality * 50;

		//// ����ǻ꾧�� +�꾧�ȼ�^2*2+�꾧����ǿ��*20
		//if (P_VALID(pEquipInfo->pEquipProto) && pEquipInfo->pEquipProto->eType == EIT_SoulCrystal)
		//{
		//	tagSoulCrystal* pSoul = (tagSoulCrystal*)pEquipInfo;
		//	nTmpValue += (pSoul->SoulSpec.bySoulCrystalLvl) * (pSoul->SoulSpec.bySoulCrystalLvl) * 2;
		//	nTmpValue += pSoul->SoulSpec.dwSoulStrength * 20;		
		//}

		// �����ʥ�飬 +ʥ�������ܺ�/100
		if (P_VALID(pEquipInfo->pEquipProto) && pEquipInfo->pEquipProto->eType == EIT_Holy)
		{
			DWORD dwTempSum = 0; 
			tagHolyMan* pHoly = (tagHolyMan*)pEquipInfo;
			for (int i=0; i< X_ESAT_ATTA_NUM; i++)
			{
				dwTempSum += pHoly->dwLevelUpAtt[i];
			}

			nTmpValue += (INT)dwTempSum/100;		
		}

		// +װ���ɳ��ȼ�^2*4+��Ʒ��
		nTmpValue += (pEquipInfo->equipSpec.byGrowLevel) * (pEquipInfo->equipSpec.byGrowLevel) * 4;
		for (int j=0; j<EQUIP_GROW_MAX_LEVEL-1; ++j)
		{
			switch(pEquipInfo->equipSpec.eGrowStar[j])
			{
			case EEGS_Gray:
				nTmpValue += 5;
				break;
			case EEGS_Green:
				nTmpValue += 15;
				break;
			case EEGS_Blue:
				nTmpValue += 30;
				break;
			case EEGS_Purple:
				nTmpValue += 50;
				break;
			default:
				break;
			}
		}

		dwEquipValue += nTmpValue;
	}

	m_Strength.nEquipValue = (INT)dwEquipValue;
}

VOID Role::UpdateYaoJingValue()				// ����������ʵ��ֵ
{
	// ����*Ʒ��*3/2 + �������ȼ�^2 + �������ȼ�*5 + ������ǰ�ȼ�*3
	tagFabao * pFabao = (tagFabao*)GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	if (!P_VALID(pFabao))
	{
		m_Strength.nFairy = 0;
		return;
	}

	DWORD dwFairyValue = 0;

	dwFairyValue += (pFabao->n16NativeIntelligence * pFabao->equipSpec.byQuality * 3)/2;

	for (INT nIdx = 0; nIdx < MaxWuxingSlotNum; nIdx++)
	{
		tagItemProto* pItemProto = g_attRes.GetItemProto(pFabao->dwWuxingBeadID[nIdx]);
		if (!P_VALID(pItemProto))
			continue;

		BYTE byLevel = pItemProto->byLevel/10;
		dwFairyValue += byLevel*byLevel;
		dwFairyValue += byLevel*5;
	}

	dwFairyValue += pFabao->n16Stage*3;

	m_Strength.nFairy = dwFairyValue;
}

VOID Role::UpdateBaseStrength()
{
	// ����ʵ��(���и�ֵ���ܺ�)��
	DWORD dwStrength = 0;

	// �ȼ�
	m_Strength.nLevel = m_nLevel*10;
	dwStrength += m_Strength.nLevel;

	// װ��ʵ��
	dwStrength += m_Strength.nEquipValue;

#if 0
	//?������ʵ��	
	DWORD dwPetStrength = 0;
	PetPocket* p = GetPetPocket();
	if (P_VALID(p))
	{	
		int num = 0;
		DWORD PetIDs[CONST_MAX_PETSOUL_NUM] = {0};
		p->GetAllPetID(PetIDs, num);
		if (P_VALID(PetIDs))
		{
			int i = 0;
			while ( num > 0 )
			{
				PetSoul* pPet = p->GetPetSoul(PetIDs[i]);
				if (P_VALID(pPet))
				{
					PetAtt& Att = pPet->GetPetAtt();
					dwPetStrength += Att.GetPetValue();
				}

				i++;
				num--;
			}
		}
	}

	m_Strength.nPet = (INT)dwPetStrength;
	dwStrength += m_Strength.nPet;
#endif
	//?����ʵ�� - ����ѻ�õ���������������ֵ֮��/1000
	INT64 nTotalFame = GetClanData().RepGetTotal();
	if ( nTotalFame > ClanData::ArrNRepLvlMin[ERL_Worshipped]*ECLT_NUM ) nTotalFame = ClanData::ArrNRepLvlMin[ERL_Worshipped]*ECLT_NUM;
	m_Strength.nFame = nTotalFame/1000;
	dwStrength += m_Strength.nFame;

	// ��óƺ����� - ����Ѿ���õĳƺ�����*2
	m_Strength.nTitle = this->GetTitleMgr()->GetObtainedTitlesNum()*2;
	dwStrength += m_Strength.nTitle;

	//?�Ѻöȳ���300�ĺ������� - ��Һ����б����Ѻöȳ���300�ĺ�������*5
	int count = 0;
	tagFriend* pFriend = NULL;
	TMap<DWORD, tagFriend*>::TMapIterator it = m_mapFriend.Begin();
	while (m_mapFriend.PeekNext(it, pFriend))
	{
		if (P_VALID(pFriend))
		{
			if ( pFriend->dwFriVal >= 300 )
			{
				count++;
			}
		}
	}

	m_Strength.nFriendship = count*5;
	dwStrength += m_Strength.nFriendship;

	//?����������� - ���з��ظ�����������������/5
	INT num = m_mapCompleteQuests.Size();
	m_Strength.nQuest = num/5;
	dwStrength += m_Strength.nQuest;

	// �������������������ܺ�
	INT nProduceLevel=0;
	TMap<DWORD, Skill*>::TMapIterator iter = m_mapSkill.Begin();
	Skill* pSkill = NULL;
	while(m_mapSkill.PeekNext(iter, pSkill))
	{
		if (!P_VALID(pSkill))
		{
			continue;
		}

		if (pSkill->GetTypeEx() == ESSTE_Produce) // �ü�������������
		{
			nProduceLevel += pSkill->GetLevel();
		}
	}

	m_Strength.nProduce = nProduceLevel*50;
	dwStrength += m_Strength.nProduce;

	SetBaseStrength((INT)dwStrength);
}

VOID Role::UpdateAdvanceStrength()
{ 
	m_Strength.nRanQuest = GetQuestStrength();
	m_Strength.nActivity = GetActivityStrength();
	m_Strength.nInstance = GetInstanceStrength();
	m_Strength.nKill = GetKillStrength();

	// ����Ƿ���Ҫ��ÿ��KillScore����
	tagDWORDTime dwNow = GetCurrentDWORDTime();
	if (dwNow.day != m_dwLastClearKillScore.day)
	{
		ClearKillScore();
	}

	m_Strength.nAdvanceStrength = m_Strength.nActivity + m_Strength.nInstance + m_Strength.nRanQuest + m_Strength.nKill;	
}

//VOID Role::UpdateConsumptiveStrength()
//{
//	m_Strength.nConsumptiveStrength += m_Strength.nAdvanceStrength;
//}

VOID Role::UpdateStrength()
{
	m_Strength.nStrength = m_Strength.nBaseStrength + m_Strength.nAdvanceStrength;
}

//--------------------------------------------------------------------------------
// �ɽ缼�� �������ˢ��
//--------------------------------------------------------------------------------
VOID Role::UpdateMiracleSkill()
{
	const tagDWORDTime &now = g_world.GetWorldTime();

	//����״ε�½ �� ������ ��Ҫ����
	if(m_dwMiracleResetTime == 0 || now.day != m_dwMiracleResetTime.day)
	{
		//��������
		m_n16MiraclePracticeNum = MIRACLE_PRC_RESET_NUM;

		//����ʱ��
		m_dwMiracleResetTime	= now;

		tagNS_ResetMiraclePrcNum send2c;
		SendMessage(&send2c,send2c.dwSize);

		tagNDBC_SaveMiraclePrcNum send2db;
		send2db.dwRoleID = GetID();
		send2db.n16MiraclePrcNum = m_n16MiraclePracticeNum;
		g_dbSession.Send(&send2db,send2db.dwSize);
	}
};

VOID Role::CheckRoleAdvanceStrength()
{
	tagDWORDTime dwNow = GetCurrentDWORDTime();
	tagDWORDTime dwLogout = m_LogoutTime;

	// ������ε�¼��ʱ�����ϴεǳ���ʱ�䲻��ͬһ���ˣ������ջ�ɱ������һ�õĽ���ʵ��ֵ����
	if (m_LogoutTime.day != dwNow.day)
	{
		ClearKillScore();
	}
	else
	{
		m_dwLastClearKillScore = GetCurrentDWORDTime();
	}

	// ������ε�¼��ʱ�����ϴεǳ���ʱ�����ڼ��Խ����һ��˵㣬��ô��Ҫ������ʵ��ֵ����
	tagDWORDTime dwIntend = 0; // ��������ϴεǳ���Ϸ�����һ�θ���ʱ��
	if (WhichWeekday(dwLogout)==1)
	{
		if (dwLogout.hour<8)
		{
			dwLogout.hour = 8;
			dwLogout.min = 0;
			dwLogout.sec = 0;
			dwIntend = dwLogout;
		}
		else
		{
			dwLogout.hour = 8;
			dwLogout.min = 0;
			dwLogout.sec = 0;
			dwIntend = IncreaseTime(dwLogout, 7*24*60*60);
		}

		if (dwNow >= dwIntend)
		{
			// ���£�����ʵ������
			SetScriptData(95, 0);
			SetScriptData(99, 0);
			SetScriptData(100, 0);
			SetScriptData(101, 0);

			return;
		}
		else
		{
			return;
		}
	}

	while (WhichWeekday(dwLogout)!=1)
	{
		//dwLogout.day++;
		dwLogout = IncreaseTime(dwLogout, 24*60*60);
	}

	dwLogout.hour = 8;
	dwLogout.min = 0;
	dwLogout.sec = 0;
	dwIntend = dwLogout;

	if (dwNow >= dwIntend)
	{
		// ���£�����ʵ������
		SetScriptData(95, 0);
		SetScriptData(99, 0);
		SetScriptData(100, 0);
		SetScriptData(101, 0);
	}
	else
	{
		return;
	}
}

static INT64 CalOnlineRewardExp(INT level,INT64 Exp)
{
	INT re = 0;
	if( level > 0 )
	{
		re =  (double)150 / pow( double(level + 10) ,(double)2) * Exp;
	}
	return re;
}


//==========================================================================================================
// �������
//==========================================================================================================

//-------------------------------------------------------------------------------------
//��ȡ��ҵ���ѫ�ȼ�
//-------------------------------------------------------------------------------------
INT16 Role::GetValiantMeritLevel(INT16 nValiantMeritVal)
{
    EMAXVALIANT_PERLEVEL eVal = static_cast<EMAXVALIANT_PERLEVEL>(nValiantMeritVal);
	
	if (  eVal >= 0 && eVal < g_attRes.GetWuXunProto(2).unWuXunVal )
	{
         m_nValiantMeritLevel = 1;
	}
	else if( eVal >= g_attRes.GetWuXunProto(2).unWuXunVal && eVal < g_attRes.GetWuXunProto(3).unWuXunVal  )
	{
        m_nValiantMeritLevel = 2 ;
	}
	else if( eVal >= g_attRes.GetWuXunProto(3).unWuXunVal && eVal < g_attRes.GetWuXunProto(4).unWuXunVal)
	{
         m_nValiantMeritLevel = 3 ;
	}
	else if( eVal >= g_attRes.GetWuXunProto(4).unWuXunVal && eVal < g_attRes.GetWuXunProto(5).unWuXunVal)
	{
		m_nValiantMeritLevel = 4 ;
	}
	else if ( eVal >= g_attRes.GetWuXunProto(5).unWuXunVal && eVal < g_attRes.GetWuXunProto(6).unWuXunVal )
	{
        m_nValiantMeritLevel = 5 ;
	}
	else if ( eVal >= g_attRes.GetWuXunProto(6).unWuXunVal && eVal < g_attRes.GetWuXunProto(7).unWuXunVal )
	{
		m_nValiantMeritLevel = 6 ;
	}
	else if ( eVal >= g_attRes.GetWuXunProto(7).unWuXunVal && eVal < g_attRes.GetWuXunProto(8).unWuXunVal )
	{
		m_nValiantMeritLevel = 7 ;
	}
	else if ( eVal >= g_attRes.GetWuXunProto(8).unWuXunVal && eVal < g_attRes.GetWuXunProto(9).unWuXunVal )
	{
		m_nValiantMeritLevel = 8 ;
	}
	else if ( eVal >= g_attRes.GetWuXunProto(9).unWuXunVal && eVal < g_attRes.GetWuXunProto(10).unWuXunVal )
	{
		m_nValiantMeritLevel = 9 ;
	}
	else if ( eVal >= g_attRes.GetWuXunProto(10).unWuXunVal  )
	{
		m_nValiantMeritLevel = 10 ;
	}

   
    return  m_nValiantMeritLevel;
}

//-------------------------------------------------------------------------------------
// ������������ݿ�
//-------------------------------------------------------------------------------------
VOID Role::SetShortCutBarData(INT nLnCount,INT nCol,const tagQuickData& QuickBarData)
{

	if ( (nLnCount < 0 || nLnCount >= BAR_COUNT) || (nCol < 0 || nCol >= BAR_BUTTON_COUNT) )
	{
		ASSERT(0);
		return ;
	}
	m_ShortCutBarData[nLnCount][nCol].m_eType     = QuickBarData.m_eType;
	m_ShortCutBarData[nLnCount][nCol].m_dwParam1  = QuickBarData.m_dwParam1;
	m_ShortCutBarData[nLnCount][nCol].m_dwParam2  = QuickBarData.m_dwParam2;
	m_ShortCutBarData[nLnCount][nCol].m_n64Param3 = QuickBarData.m_n64Param3;
	if ( m_ShortCutBarData[nLnCount][nCol].m_eType != m_QuickBarStateFlags[nLnCount][nCol].m_eType )
	{
        m_QuickBarStateFlags[nLnCount][nCol].m_bIsChange = TRUE;
	}
	else
	{
		 if ( 1 == m_ShortCutBarData[nLnCount][nCol].m_eType  )
		 {
			 if ( m_QuickBarStateFlags[nLnCount][nCol].m_dwParam1 != m_ShortCutBarData[nLnCount][nCol].m_dwParam1 )
			 {
                  m_QuickBarStateFlags[nLnCount][nCol].m_bIsChange = TRUE;
				  return;
			 }
		 }

		 if ( 3 == m_ShortCutBarData[nLnCount][nCol].m_eType )
		 {
			 if ( m_QuickBarStateFlags[nLnCount][nCol].m_dwParam2 != m_ShortCutBarData[nLnCount][nCol].m_dwParam2 )
			 {
				 m_QuickBarStateFlags[nLnCount][nCol].m_bIsChange = TRUE;
				 return;
			 }
		 }
		 if ( 2 == m_ShortCutBarData[nLnCount][nCol].m_eType )
		 {
			 if ( m_QuickBarStateFlags[nLnCount][nCol].m_n64Param3 != m_ShortCutBarData[nLnCount][nCol].m_n64Param3 )
			 {
				 m_QuickBarStateFlags[nLnCount][nCol].m_bIsChange = TRUE;
				 return;
			 }
		 }
		 if ( 4 == m_ShortCutBarData[nLnCount][nCol].m_eType )
		 {
			 if ( m_QuickBarStateFlags[nLnCount][nCol].m_dwParam1 != m_ShortCutBarData[nLnCount][nCol].m_dwParam1 )
			 {
				 m_QuickBarStateFlags[nLnCount][nCol].m_bIsChange = TRUE;
				 return;
			 }

		 }
	}
}

VOID Role::SetShortCutBarDataFlag(INT nLnCount,INT nCol,const tagQuickData& QuickBarData)
{
	if ( (nLnCount < 0 || nLnCount >= BAR_COUNT) || (nCol < 0 || nCol >= BAR_BUTTON_COUNT) )
	{
		ASSERT(0);
		return ;
	}

	m_QuickBarStateFlags[nLnCount][nCol].m_bIsChange = FALSE;
	m_QuickBarStateFlags[nLnCount][nCol].m_dwParam1  = QuickBarData.m_dwParam1;
	m_QuickBarStateFlags[nLnCount][nCol].m_dwParam2  = QuickBarData.m_dwParam2;
	m_QuickBarStateFlags[nLnCount][nCol].m_n64Param3 = QuickBarData.m_n64Param3;
    m_QuickBarStateFlags[nLnCount][nCol].m_eType     = QuickBarData.m_eType;
}

//-------------------------------------------------------------------------------------
// �Ƿ���Ҫ����
//-------------------------------------------------------------------------------------
BOOL Role::IsNeedSave()
{

	for(int i = 0; i < BAR_COUNT ; i++ )
	{
		for(int j = 0; j < BAR_BUTTON_COUNT; j++)
		{
			if( m_QuickBarStateFlags[i][j].m_bIsChange )
			{
				return TRUE;
			}
			
		}
	}
	return FALSE;
    
}

tagQuickData& Role::GetShortCutBarData(INT nLnCount,INT nCol)
{
    if ( (nLnCount < 0 || nLnCount >= BAR_COUNT) || (nCol < 0 || nCol >= BAR_BUTTON_COUNT) )
    {
		ASSERT(0);
		static tagQuickData re;
		return re;
		
    }
	return m_ShortCutBarData[nLnCount][nCol];
}

VOID Role::SendShortCutBarDataToClient()
{
	if ( !m_bIsSended )
	{
		tagNS_MsgQuickBarData  send;
		send.dwRoleID = GetID();
		INT nLn = 0;

		for (INT nSendCount = 0 ; nSendCount < QUICK_BAR_SEND_COUNT; nSendCount++)
		{
			for ( INT i = 0 ; i < QUICK_BAR_COUNT ; i++ )
			{
				for( INT j = 0; j < QUICK_BAR_BUTTON_COUNT; j++ )
				{
					send.btnData[i][j] = GetShortCutBarData(nLn,j);
				}
				nLn++;
			}

			switch( nSendCount )
			{

			case 0:
				send.eQuickSendCount = QUICK_BAR_ONE; 
				break;
			case 1:
				send.eQuickSendCount = QUICK_BAR_TWO;
				break;
			case 2:
				send.eQuickSendCount = QUICK_BAR_THREE;
				break;
			case 3:
				send.eQuickSendCount = QUICK_BAR_FOUR;
				break;
			case 4:
				send.eQuickSendCount = QUICK_BAR_FIVE;
				break;
			default:

				return ;
				break;
			}
			SendMessage(&send,send.dwSize);

		}
		m_bIsSended = TRUE;
	}



}


VOID  Role::SaveQuickBarData2DB()
{
	 BOOL bIsNeedSave = IsNeedSave();
	 if ( !bIsNeedSave )
	 {
          return ;
	 }
     tagNDBC_SaveShortCut_QuickBar  send;
	 send.dwRoleID = GetID();
     for(int i = 0; i < BAR_COUNT ; i++ )
	 {
		 for(int j = 0; j < BAR_BUTTON_COUNT; j++)
		 {
			 send.QuickBarData[i*BAR_BUTTON_COUNT+j] = GetShortCutBarData(i,j);
		 }
	 }
	 g_dbSession.Send(&send,send.dwSize);

}

VOID   Role::SetIsSavedQuickBarData(INT16 nIsSaved)
{
       nIsSavedQuickBarData = nIsSaved ;
}

INT16   Role::GetIsSavedQuickBarData()const
{
       return  nIsSavedQuickBarData;
}

VOID   Role::SendHaveNotSavedQuickBarMsg()
{
	 if( nIsSavedQuickBarData ==(INT16)Enum_NotSaved )
	 {

		 tagNS_MsgIsSavedQuickBarData msgSend;
     	 msgSend.dwRoleID = GetID() ;
		 msgSend.nIsSaved = nIsSavedQuickBarData ;
		 SendMessage(&msgSend,msgSend.dwSize);
		 SetIsSavedQuickBarData((INT16)Enum_Saved);
	 }
	
}

 VOID  Role::LoadQuickBarData2DB()
 {
      tagNDBC_LoadShortCut_QuickBar  send;
	  send.dwRoleID = GetID();
	  g_dbSession.Send(&send,send.dwSize);
 }

//=====================================================================================
// Jason 2010-1-15 v1.3.2 �ж��ѻ����Ƿ���

BOOL Role::IsOfflineHangOpen(VOID)
{
	return FALSE;
}


BOOL Role::GetOfflineRewardParam(DWORD & hour,DWORD & minute)
{
	hour = 0;
	minute = 0;
	if(!bLoongLessingOpen)
		return FALSE;

	if( OfflineReward::OfflineRewardSwitch() != 1 )
		return FALSE;

	if(IsValidOfLessingLoong())
	{
		return FALSE;
	}
	tagDWORDTime t = g_world.GetWorldTime  ();
	if(m_dateLastLessingLoong.day == t.day && m_dateLastLessingLoong.month == t.month && m_dateLastLessingLoong.year == t.year )
	{
		return FALSE;
	}
	if( t.day == m_timeOnlineRewardPerDayRec.day && t.month == m_timeOnlineRewardPerDayRec.month && t.year == m_timeOnlineRewardPerDayRec.year ) 
	{
		return FALSE;
	}
	DWORD tem = CalTodayOnlineTime();
	DWORD val = OFFLINE_TIMEOUT > tem ? OFFLINE_TIMEOUT - tem : 0;
	if( val )
	{
		hour = (double)val / (60 * 60);
		minute = ( (double)val - (double)hour * 60 * 60 ) / 60; 
	}
	return TRUE;
}

#ifdef OFFLINE_TIMEOUT
#undef OFFLINE_TIMEOUT
#endif

DWORD Role::GetOfflineTime()
{
	DWORD re = 0;
	//if(m_LogoutTime < m_LoginTime)
	//{
	//	re += CalcTimeDiff(m_LoginTime, m_LogoutTime);
	//}
	//else
	//{
	//	re += CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	//}
	if( m_iniLogoutTime < m_LoginTime )
		return CalcTimeDiff(m_LoginTime, m_iniLogoutTime);

	return re;
}

BOOL Role::IncFollowPetPocketValve()
{
	IncPetPocketValve();
	if( m_u16FollowPetPocketValve + m_u16RidingPetPocketValve >= m_u16PetPocketValve )
		return FALSE;
	if( m_u16FollowPetPocketValve < MAX_FOLLOWPETSOUL_NUM )
	{
		++m_u16FollowPetPocketValve;
		return TRUE;
	}
	return FALSE;
}

BOOL Role::IncRidingPetPocketValve()
{
	IncPetPocketValve();
	if( m_u16FollowPetPocketValve + m_u16RidingPetPocketValve >= m_u16PetPocketValve )
		return FALSE;
	if( m_u16RidingPetPocketValve < MAX_RIDINGPETSOUL_NUM )
	{
		++m_u16RidingPetPocketValve;
		return TRUE;
	}
	return FALSE;
}

BOOL Role::IncPetPocketValve()
{
	if( m_u16PetPocketValve < MAX_PETSOUL_NUM )
	{
		++m_u16PetPocketValve;
		return TRUE;
	}
	return FALSE;
}

// ��ý�ɫ������
INT Role::GetPm(ERankingType eType)
{
	TMap<INT, tagRankingsNode*>::TMapIterator it;
	TMap<INT, tagRankingsNode*> TmpMap;

	switch(eType)
	{
	case ERT_Strength:
		{
			TmpMap = g_WorldRankingMgr.GetStrengthRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}
	case ERT_Fame:
		{
			TmpMap = g_WorldRankingMgr.GetFameRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}
	case ERT_Equip:
		{
			TmpMap = g_WorldRankingMgr.GetEquipRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}
	case ERT_Level:
		{
			TmpMap = g_WorldRankingMgr.GetLevelRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}
	case ERT_Pet:
		{
			DWORD pAllPetID[CONST_MAX_PETSOUL_NUM] ={0};
			INT nPetNum = 0;
			GetPetPocket()->GetAllPetID( pAllPetID, nPetNum );

			if ( P_VALID(pAllPetID) )
			{
				INT nBestPm = 501;
				DWORD dwBestPetID = 0;
				for( INT i = 0; i < nPetNum; i++ )
				{
					DWORD dwPetID = pAllPetID[i];
					PetSoul* pPet = GetPetPocket()->GetPetSoul(dwPetID);
					if (!P_VALID(pPet))
					{
						continue;
					}

					INT nPm = pPet->GetPetAtt().GetPetPm();
					if ( nPm < nBestPm && nPm!=0 )
					{
						nBestPm = nPm;
						dwBestPetID = dwPetID;
					}
				}

				if (nBestPm>500)
				{
					return 0;
				}
				else
				{
					return nBestPm;
				}
			}
		}


	case ERT_WuXun:
		{
			TmpMap = g_WorldRankingMgr.GetWuXunRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}
	case ERT_Flower:
		{
			TmpMap = g_WorldRankingMgr.GetFlowerRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}

	case ERT_Egg:
		{
			TmpMap = g_WorldRankingMgr.GetEggRanking();
			it = TmpMap.Begin();

			tagRankingsNode* pTmp = NULL;
			while (TmpMap.PeekNext(it, pTmp))
			{
				if (!P_VALID(pTmp))
				{
					continue;
				}

				if (pTmp->dwRoleID == GetID())
				{
					return pTmp->nNum;
				}
			}

			return 0;
		}

	default:
		return 0;
	}
}

// ÿ��һ��˵�������н�ɫ�Ľ���ʵ��ֵ
VOID Role::ClearAdvanceStrength()
{
	tagDWORDTime now = GetCurrentDWORDTime();

	if(
		(1==WhichWeekday(now)) 
		&& (now.hour == 8 && now.min == 0) 
		&& (m_dwLastClearAdvenceStrength.year != now.year || m_dwLastClearAdvenceStrength.month != now.month || m_dwLastClearAdvenceStrength.day != now.day)
		)
	{
		SetScriptData(99,	0);
		SetScriptData(100,	0);
		SetScriptData(101,	0);
		SetScriptData(95,	0);
		SetAdvanceStrength(0);

		m_dwLastClearAdvenceStrength = now;
	}
}

//-----------------------------------------------------------------------------
// �Ƿ�ɻ���������
//-----------------------------------------------------------------------------
BOOL Role::CanGetTeamProfit()
{
	DWORD dwTeamID = GetTeamID();
	const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
	if(!P_VALID(pTeam)) return FALSE;

	// С���������������򲻿��ܲ����������
	INT nNum = pTeam->GetMemNum();
	if(nNum < 2) return FALSE;

	for (INT i = 0; i<nNum; ++i)
	{
		DWORD dwMemID = pTeam->GetMemID(i);
		if (dwMemID != GetID())
		{
			Role* pTeammate = g_roleMgr.GetRolePtrByID(dwMemID);
			if (!P_VALID(pTeammate))
			{
				continue;
			}

			FLOAT fXd = pTeammate->GetCurPos().x;
			FLOAT fZd = pTeammate->GetCurPos().z;
			FLOAT fXs = GetCurPos().x;
			FLOAT fZs = GetCurPos().z;

			// �������
			if( Vec3DistSq(this->GetCurPos(), pTeammate->GetCurPos()) < 50*100*50*100 )
			{
				return TRUE;
			}

			//FLOAT fDistance = (fXd-fXs)*(fXd-fXs) + (fZd-fZs)*(fZd-fZs);

			//if (fDistance < 50*100*50*100) // ����С��100����
			//{
			//	return TRUE;
			//}
		}
	}

	return FALSE;
}

//--------------------------------------------------------------------------------
// �����������
//--------------------------------------------------------------------------------
VOID Role::GetTeamProfit()
{
	if (m_byTeammatesNum < 2) return;
	
	// to do ���ݶ���������Ϊ�ý�ɫ������Ӧ��buff
	const tagBuffProto* pProto = NULL;
	switch(m_byTeammatesNum)
	{
	case 2:
		{
			pProto = g_attRes.GetBuffProto(BuffTeamProfit2);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}

	case 3:
		{
			pProto = g_attRes.GetBuffProto(BuffTeamProfit3);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 4:
		{
			pProto = g_attRes.GetBuffProto(BuffTeamProfit4);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 5:
		{
			pProto = g_attRes.GetBuffProto(BuffTeamProfit5);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 6:
		{
			pProto = g_attRes.GetBuffProto(BuffTeamProfit6);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}

	}

	// ���ݺ�������Ϊ�ý�ɫ�����Ӧ��buff
	pProto = NULL;
	switch(m_byTeamFriendNum)
	{
	case 1:
		{
			pProto = g_attRes.GetBuffProto(BuffFriExpProfit2);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}

	case 2:
		{
			pProto = g_attRes.GetBuffProto(BuffFriExpProfit3);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 3:
		{
			pProto = g_attRes.GetBuffProto(BuffFriExpProfit4);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 4:
		{
			pProto = g_attRes.GetBuffProto(BuffFriExpProfit5);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 5:
		{
			pProto = g_attRes.GetBuffProto(BuffFriExpProfit6);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	}

	pProto = NULL;
	switch(m_byTeamFriendNum)
	{
	case 1:
		{
			pProto = g_attRes.GetBuffProto(BuffFriBaseProfit2);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}

	case 2:
		{
			pProto = g_attRes.GetBuffProto(BuffFriBaseProfit3);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 3:
		{
			pProto = g_attRes.GetBuffProto(BuffFriBaseProfit4);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 4:
		{
			pProto = g_attRes.GetBuffProto(BuffFriBaseProfit5);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	case 5:
		{
			pProto = g_attRes.GetBuffProto(BuffFriBaseProfit6);
			if(!P_VALID(pProto)) break;

			TryAddBuff(this, pProto, NULL, NULL, NULL);
			break;
		}
	}
}

//-----------------------------------------------------------------------------------
// ȥ���������
//-----------------------------------------------------------------------------------
VOID Role::CancelTeamProfit(bool bLauncheredByDeleteMember /*= false*/)
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		if (m_Buff[n].GetID() == BuffTeamProfit || m_Buff[n].GetID() == BuffFriExpProfit || m_Buff[n].GetID() == BuffFriBaseProfit )
		{
			RemoveBuff(m_Buff[n].GetID(), FALSE);
		}

		if (bLauncheredByDeleteMember)
		{
			if (m_Buff[n].GetID() == BloodBrotherBuffTeather/100 || m_Buff[n].GetID() == BloodBrotherBuffBrother/100)
			{
				RemoveBuff(m_Buff[n].GetID(), FALSE);
			}
		}
		
	}

	if( NeedRecalAtt() )
		RecalAtt();
}

//-----------------------------------------------------------------------------------
// ȥ�����BUFF
//-----------------------------------------------------------------------------------
VOID Role::CancelBrotherProfit()
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		if (m_Buff[n].GetID() == BloodBrotherBuffTeather/100 || m_Buff[n].GetID() == BloodBrotherBuffBrother/100)
		{
			RemoveBuff(m_Buff[n].GetID(), FALSE);
		}
	}
}
//-----------------------------------------------------------------------------------
// �����������״̬
//-----------------------------------------------------------------------------------
VOID Role::UpdateTeamProfit()
{
	DWORD dwTeamID = GetTeamID();
	if (dwTeamID == GT_INVALID)
	{
		return;
	}

	const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
	if (!P_VALID(pTeam))
	{
		return;
	}

	// �ߵ����˵����ɫ��С��
	// �����ж��Ƿ�ɻ���������,�����������İ�buffȥ�������������Ĳ�����
	if ( !CanGetTeamProfit() )
	{
		CancelTeamProfit();
	}
	else
	{
		// ��ö������Ͷ����еĺ�����
		BYTE byTeammatesNum = pTeam->GetMemNum();
		BYTE byTeamFriendsNum = 0;
		for (INT i = 0; i<byTeammatesNum; ++i)
		{
			DWORD dwMemID = pTeam->GetMemID(i);
			if (dwMemID != GetID())
			{
				Role* pTeammate = g_roleMgr.GetRolePtrByID(dwMemID);
				if (!P_VALID(pTeammate))
				{
					continue;
				}

				if( P_VALID(this->GetFriendPtr(pTeammate->GetID())) )
				{
					byTeamFriendsNum++;
				}
			}
		}

		// �ж϶������Ͷ����еĺ������Ƿ��б仯���еĻ���Ҫ����buff
		if ( byTeamFriendsNum != m_byTeamFriendNum || byTeammatesNum != m_byTeammatesNum )
		{
			m_byTeammatesNum = byTeammatesNum;
			m_byTeamFriendNum = byTeamFriendsNum;

			CancelTeamProfit();
			GetTeamProfit();
		}
	}
}

// ��֮����ϵͳ-��������״̬
VOID Role::SaveTrainState(DWORD dwTrainID, ETrainState eTrainState)
{
	INT nState = 0;

	// û������������ID���Ϸ���������״̬������
	if (eTrainState < ETS_Close || eTrainState > ETS_Pass)
		return;

	tagTrainState* pTrainState = m_mapTrainStates.Peek(dwTrainID);
	if (P_VALID(pTrainState))
	{
		// ���ȸı��ڴ��е�����״̬
		pTrainState->eTrainState = eTrainState;

		// Ȼ��֪ͨ���ݿ�ı�����״̬
		tagNDBC_SaveOpenTrainState send;
		send.dwRoleID = GetID();
		send.dwTrainID = dwTrainID;
		send.eTrainState = eTrainState;
		send.dwCompleteness = pTrainState->nCompleteness;

		g_dbSession.Send(&send, send.dwSize);
	}
}

// ȡ����״̬
ETrainState Role::GetTrainState(DWORD dwTrainID)
{
	ETrainState	eCurTrainState = ETS_Close;			// ����״̬

	if (m_mapTrainStates.IsExist(dwTrainID))
	{
		eCurTrainState = (m_mapTrainStates.Peek(dwTrainID))->eTrainState;
	}
	else
	{
		eCurTrainState = ETS_Close;
	}

	return eCurTrainState;
}

// ��ȡһ����������ɶ�
DWORD Role::GetTrialCompleteNum(DWORD dwTrainID)
{
	tagTrainState* pTrainState = m_mapTrainStates.Peek(dwTrainID);
	if (P_VALID(pTrainState))
		return pTrainState->nCompleteness;
	else
		return 0;
}

// ����һ����������ɶ�
VOID Role::SetTrainCompleteness(DWORD dwTrainID, DWORD dwCompleteness)
{
	tagTrainState* pTrainState = m_mapTrainStates.Peek(dwTrainID);
	if (!P_VALID(pTrainState))
	{
		pTrainState = new tagTrainState;
		if (!P_VALID(pTrainState))
			return;
		pTrainState->dwTrainID = dwTrainID;
		pTrainState->eTrainState = ETS_Close;
		pTrainState->nCompleteness = dwCompleteness;
		m_mapTrainStates.Add(dwTrainID, pTrainState);
	}
	else
	{
		pTrainState->nCompleteness = dwCompleteness;
	}
	
	tagNS_SyncTrainCompleteness send;
	send.dwTrainID = pTrainState->dwTrainID;
	send.dwCompleteness = pTrainState->nCompleteness;
	SendMessage(&send, send.dwSize);

	SaveTrainState(dwTrainID, pTrainState->eTrainState);
}

// ��һ���ɾ�����Ϊ�����״̬
VOID Role::CompleteTrain(DWORD dwTrainID)
{
	tagTrainState* pTrainState = m_mapTrainStates.Peek(dwTrainID);
	if (P_VALID(pTrainState) && pTrainState->eTrainState > ETS_Close && pTrainState->eTrainState <= ETS_Finish)
	{
		SaveTrainState(dwTrainID, ETS_Finish);

		tagNS_FinishTrain FinishTrain;
		FinishTrain.dwTrainID = dwTrainID;
		FinishTrain.eTrainErrCode = E_Train_Success;
		FinishTrain.eTrainState = ETS_Finish;
		SendMessage(&FinishTrain, FinishTrain.dwSize);
	}
}

// ȡ�����������
INT Role::GetPassTrainCnt()
{
	INT				nPassTrainCnt = 0;
	tagTrainState*	pTrainState = NULL;

	m_mapTrainStates.ResetIterator();
	while (m_mapTrainStates.PeekNext(pTrainState))
	{
		if (!P_VALID(pTrainState))
		{
			return 0;
		}
		if (pTrainState->eTrainState == ETS_Pass)
		{
			nPassTrainCnt++;
		}
	}

	return nPassTrainCnt;
}

// ���ͻ��˷�������״̬��
ETrainErrCode Role::SendTrainStateList2Client()
{
	INT	  nTrainCnt = m_mapTrainStates.Size();
	DWORD dwSize	= sizeof(tagNS_GetTrainList) + nTrainCnt*sizeof(tagTrainState);

	MCREATE_MSG(pSend, dwSize, NS_GetTrainList);

	if (nTrainCnt <= 0)
	{
		pSend->eTrainErrCode	= E_Train_List_Null;
		pSend->nTrainCnt		= 0;
	}
	else
	{
		INT nCnt = 0;
		tagTrainState* pTrainState = NULL;
		tagTrainState* pTmpTrainState = pSend->pTrainAttTable;

		pSend->eTrainErrCode	= E_Train_Success;
		pSend->nTrainCnt		= nTrainCnt;

		m_mapTrainStates.ResetIterator();
		while (m_mapTrainStates.PeekNext(pTrainState))
		{
			if (!P_VALID(pTrainState))
			{
				return E_Train_Unknown;
			}
			IFASTCODE->MemCpy(pTmpTrainState, pTrainState, sizeof(tagTrainState));
			pTmpTrainState++;
		}
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return E_Train_Success;
}

// ��֮����ϵͳ-������������
DWORD Role::GiveTrainReward(DWORD dwTrainID)
{
	DWORD dwErrCode = E_Train_Not_Pass;
	const tagTrainAttProto* pTrainAttProto = g_attRes.GetOneTrainAttProto(dwTrainID);

	if (!P_VALID(pTrainAttProto))
	{
		dwErrCode = E_Train_Unknown;
	}
	else
	{
		TList<tagItem *> ListItem1;
		TList<tagItem *> ListItem2;
		tagItemReward* pItemReward1 = (tagItemReward*)(&(pTrainAttProto->TrainReward.pItemReward[0]));
		tagItemReward* pItemReward2 = (tagItemReward*)(&(pTrainAttProto->TrainReward.pItemReward[1]));
		INT16 nLapNum1 = 0;
		INT16 nLapNum2 = 0;

		// ����������Ʒ1
		if (CreateRewardItem(ListItem1, pItemReward1, &nLapNum1) != E_Train_Success)
		{
			return E_Train_Unknown;
		}

		// ����������Ʒ2
		if (CreateRewardItem(ListItem2, pItemReward2, &nLapNum2) != E_Train_Success)
		{
			return E_Train_Unknown;
		}

		// ��ⱳ��ʣ������
		ItemMgr& itemMgr = GetItemMgr();	
		INT nFreeSize = itemMgr.GetBagFreeSize();
		if (nFreeSize < (nLapNum1 + nLapNum2))
		{
			return E_Train_Bag_Full;
		}

		// ���轱��
		tagItem * pItem = NULL;
		INT nCnt = 0;
		ListItem1.ResetIterator();
		while (ListItem1.PeekNext(pItem))
		{
			itemMgr.Add2Bag(pItem, ELCID_Quest_rewards, TRUE);
			nCnt++;
			if (nCnt >= pItemReward1->nItemCount)
			{
				break;
			}
		}
		nCnt = 0;
		ListItem2.ResetIterator();
		while (ListItem2.PeekNext(pItem))
		{
			itemMgr.Add2Bag(pItem, ELCID_Quest_rewards, TRUE);
			nCnt++;
			if (nCnt >= pItemReward2->nItemCount)
			{
				break;
			}
		}

		// ��Ǯ
		m_CurMgr.IncBagSilver(pTrainAttProto->TrainReward.dwMoneyReward, ELCID_Quest_rewards);

		// ������
		ExpChange(INT64(pTrainAttProto->TrainReward.dwExpReward));

		dwErrCode = E_Train_Pass;
	}
	return dwErrCode;
}

//��֮����ϵͳ-����������Ʒ  nLapNum--ռ��������
DWORD Role::CreateRewardItem(TList<tagItem *>& pListRewardItem, tagItemReward* pItemReward, INT16* nLapNum)
{
	DWORD dwErrCode = E_Train_Success;

	if (!P_VALID(nLapNum))
	{
		return E_Train_Unknown;
	}
	if (pItemReward->nItemCount <= 0)
	{
		*nLapNum = 0;
		return dwErrCode;
	}

	for (int nCnt = 0; nCnt < pItemReward->nItemCount; nCnt++)
	{
		tagItem* pNewItem = ItemCreator::Create(EICM_Quest, GetID(), pItemReward->nItemTypeID, 1,-1,0,0,10000,0,pItemReward->nItemQuality);
		if (!P_VALID(pNewItem))
		{
			*nLapNum = 0;
			return E_Train_Unknown;
		}

		//if(MIsEquipment(pNewItem->dwTypeID))
		//{
		//	//if(pItemReward->nItemQuality == GT_INVALID)
		//	//{
		//	//	// ������	
		//	//}
		//	//else
		//	{
		//		ItemCreator::IdentifyEquip((tagEquip*)pNewItem, (EItemQuality)pItemReward->nItemQuality);
		//	}
		//}

		pListRewardItem.PushBack(pNewItem);
	}

	*nLapNum += ((pItemReward->nItemCount/((pListRewardItem.Front())->pProtoType->n16MaxLapNum)) + 1);
	
	return dwErrCode;
}
VOID	Role::GodMiraclePoints(INT points)
{
	m_nGodMiraclePoints += points;
	if(  m_nGodMiraclePoints < 0 )
		m_nGodMiraclePoints = 0;
}
INT		Role::GetGodMiraclePoints(VOID)
{
	return m_nGodMiraclePoints;
}

// ���ͻ��˷��ͳ�ս����ս����Ϣ
DWORD Role::SendCityStruggleRecord()
{
	Map* pMap = GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	City* pCity = g_cityMgr.GetCity(pMap);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	tagCSRoleData* pRoleData = pCity->GetCityStruggle().GetRoleDataPtr(GetID());
	if (!P_VALID(pRoleData))
	{
		return GT_INVALID;
	}

	tagNS_CSRoleRecord send;
	send.eResult = pCity->GetCityStruggle().GetResult();
	send.RoleData = pRoleData->RoleData;
	send.nExploit = pRoleData->nExploit;
	send.nContribute = pRoleData->nContribute;
	send.nMoney = pRoleData->nMoney;

	INT nSize = pCity->GetCityStruggle().GetRankData().size();

	// ս������
	for (INT i=0; i!=MAX_CS_RANK_NUM && i!=nSize; ++i)
	{
		send.RoleRank[i].dwRoleID = (pCity->GetCityStruggle().GetRankData())[i].first;

		Role* pRankRole = g_roleMgr.GetRolePtrByID(send.RoleRank[i].dwRoleID);
		if (!P_VALID(pRankRole))
		{
			continue;
		}

		tagCSRoleData* pRankData = pCity->GetCityStruggle().GetRoleDataPtr(pRankRole->GetID());
		if (!P_VALID(pRankData))
		{
			continue;
		}

		pCity->GetForceType(pRankRole, send.RoleRank[i].eForce);
		send.RoleRank[i].byKills = pRankData->RoleData.byKills;
		send.RoleRank[i].nScore = pRankData->RoleData.nScore;
	}

	SendMessage(&send, send.dwSize);
	return E_Success;
}

// ���ͻ���ͬ����������������ͬ��������������ͬ����������
VOID Role::SyncOnlineNum2Role(ESyncOnlineNumType eSyncOnlineNumType, UINT16 nOnlineNum)
{
	tagNS_SyncOnlineNum send;

	if (eSyncOnlineNumType != ESOR_Online_Friend_Num &&				// ������������
		eSyncOnlineNumType != ESOR_Online_GuildMember_Num &&		// ͬ������������
		eSyncOnlineNumType != ESOR_Online_ClassMate_Num)		// ͬ����������
	{
		return;
	}
	send.eSyncOnlineNumType = eSyncOnlineNumType;
	send.nNum = nOnlineNum;

	SendMessage(&send, send.dwSize);
}

VOID Role::SyncRoleAttr(ERoleAttrSync eAttr, EMessageRange eRange/* = EMR_Self*/)
{
	if (eAttr >= ERoleAttrEnd)
	{
		return;
	}

	INT nVal = 0;
	switch (eAttr)
	{
	//case ERoleAttrNeutralMonsterKillCount:
	//	if (P_VALID(GetWarRole()))
	//	{
	//		nVal = GetWarRole()->GetNeutralMonsterKillCount();
	//	}
	//	break;
	case ERoleAttrMonsterKillCount:
		if (P_VALID(GetWarRole()))
		{
			nVal = GetWarRole()->GetMonsterKillCount();
		}
		break;
	case ERoleAttrBossKillCount:
		if (P_VALID(GetWarRole()))
		{
			nVal = GetWarRole()->GetBossKillCount();
		}
		break;
	case ERoleAttrRoleKillCount:	//��ɫ��������~
		if (P_VALID(GetWarRole()))
		{
			nVal = GetWarRole()->GetRoleDeadCount();
		}
		break;
	case ERoleAttrRareBossKillCount:
		if (P_VALID(GetWarRole()))
		{
			nVal = GetWarRole()->GetRareBossKillCount();
		}
		break;
	case ERoleAttrWarGold:
		if (P_VALID(GetWarRole()))
		{
			nVal = GetWarRole()->GetGold();
		}
		break;
	}

	if (EMR_Self == eRange)
	{
		tagNS_SyncRoleAttr msg;
		msg.eAttr	= eAttr;
		msg.nVal	= nVal;
		SendMessage(&msg, msg.dwSize, eRange);
	}
	else
	{
		tagNS_SyncRoleAttrToOther msg;
		msg.dwRoleID	= GetID();
		msg.eAttr		= eAttr;
		msg.nVal		= nVal;
		SendMessage(&msg, msg.dwSize, eRange);
	}
}

// ȡ���ߺ�������
UINT16 Role::GetOnlineFriendCnt()
{
	tagFriend* pFriend = NULL;
	UINT16 nOnlineNum = 0;
	TMap<DWORD, tagFriend*>::TMapIterator iterator = m_mapFriend.Begin();

	while (m_mapFriend.PeekNext(iterator, pFriend))
	{
		if (!P_VALID(pFriend))
		{
			continue;
		}
		Role* pRole = g_roleMgr.GetRolePtrByID(pFriend->dwFriendID);
		if (P_VALID(pRole))
		{
			nOnlineNum++;
		}
	}
	return nOnlineNum;
}

// ȡͬ����������
UINT16 Role::GetOnlineClassmatesCnt()
{
	UINT16 nOnlineNum = 0;
	BOOL bHasApprentice = FALSE;

	// �ж�ʦ��ʦ������
	Role* pMaster = g_roleMgr.GetRolePtrByID(GetMasterID());
	if (P_VALID(pMaster))
	{
		nOnlineNum++;
	}

	// ���ͬ���Ƿ����ߣ��õ�����ͬ������
	for (INT i=0; i != MAX_APPRENTICE_NUM; ++i)
	{
		DWORD dwMateID = GetClassMate().dwData[i];
		if (dwMateID == 0)
		{
			continue;
		}
		Role* pRole = g_roleMgr.GetRolePtrByID(dwMateID);
		if (P_VALID(pRole))
		{
			nOnlineNum++;
		}
	}

	// ���ͽ���Ƿ�����
	DWORD dwApprenticeID = 0;
	TMap<DWORD, DWORD>::TMapIterator iterator = m_mapApprentice.Begin();
	while (m_mapApprentice.PeekNext(iterator, dwApprenticeID))
	{
		if (dwApprenticeID > 0)
		{
			bHasApprentice = TRUE;
		}
		Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
		if (P_VALID(pApprentice))
		{
			nOnlineNum++;
		}
	}

	// �Լ���ʦ����Ҫ��1
	if (bHasApprentice)
	{
		nOnlineNum += 1;
	}
	return nOnlineNum;
}

// ��ͬ��ͬ��ͬ�������仯
VOID Role::SyncOnlineNum2Classmates(BOOL bLogout)
{
	// ���ͬ���Ƿ����ߣ��õ�����ͬ������
	for (INT i=0; i != MAX_APPRENTICE_NUM; ++i)
	{
		DWORD dwMateID = GetClassMate().dwData[i];
		if (dwMateID == 0)
		{
			continue;
		}

		Role* pRole = g_roleMgr.GetRolePtrByID(dwMateID);
		if (P_VALID(pRole))
		{
			pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, (bLogout?(pRole->GetOnlineClassmatesCnt()-1):pRole->GetOnlineClassmatesCnt()));
		}
	}
}

// ��ͽ��ͬ��ͬ�������仯
VOID Role::SyncOnlineNum2Apprentices(BOOL bLogout)
{
	DWORD dwApprenticeID = 0;
	TMap<DWORD, DWORD>::TMapIterator iterator = m_mapApprentice.Begin();

	while (m_mapApprentice.PeekNext(iterator, dwApprenticeID))
	{
		Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
		if (P_VALID(pApprentice))
		{
			pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, (bLogout?(pApprentice->GetOnlineClassmatesCnt()-1):pApprentice->GetOnlineClassmatesCnt()));
		}
	}
}

// ��ʦ��ͬ��ͬ��ͬ�������仯
VOID Role::SyncOnlineNum2MasterApprenticesClassmates(BOOL bLogout)
{
	// ���Լ�����ͬ����������
	if (bLogout == FALSE)
	{
		SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, GetOnlineClassmatesCnt());
	}

	// ��ʦ��ͬ��ͬ����������
	Role* pMaster = g_roleMgr.GetRolePtrByID(GetMasterID());
	if (P_VALID(pMaster))
	{
		// ��ʦ��ͬ��ͬ�������仯
		pMaster->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, (bLogout?(pMaster->GetOnlineClassmatesCnt()-1):pMaster->GetOnlineClassmatesCnt()));
	}

	// ��ͬ��ͬ��ͬ�������仯
	SyncOnlineNum2Classmates(bLogout);

	// ��ͽ��ͬ��ͬ�������仯
	SyncOnlineNum2Apprentices(bLogout);
}

DWORD g_dwGodAreaID[5] = { 6862, 10502, 6047, 6197, 9456};

BOOL Role::IsInFixArea(DWORD dwAreaID)
{
	for (int i = 0; i < 5; i++)
	{
		if ( g_dwGodAreaID[i] == dwAreaID)
			return TRUE;
	}
	return FALSE;
}
VOID	Role::SynetRiderSpeed(Role*pRider)
{
	if( P_VALID(pRider) )
	{
		if( !m_ownerSpeeds.bSetted )
		{
			m_ownerSpeeds.bSetted = TRUE;
			m_ownerSpeeds.m_fXZSpeed	= m_fXZSpeed;
			m_ownerSpeeds.m_fYSpeed		= m_fYSpeed;
			m_ownerSpeeds.m_fSwimXZSpeed	= m_fSwimXZSpeed;
			m_ownerSpeeds.m_fJumpXZSpeed	= m_fJumpXZSpeed;
			m_ownerSpeeds.m_fJumpYSpeed		= m_fJumpYSpeed;
			m_ownerSpeeds.m_fDropXZSpeed	= m_fDropXZSpeed;
			m_ownerSpeeds.m_fSlideSpeed		= m_fSlideSpeed;
			m_ownerSpeeds.m_fMountXZSpeed	= m_fMountXZSpeed;
		}
		m_fXZSpeed		= pRider->m_fXZSpeed;     
		m_fYSpeed		= pRider->m_fYSpeed;      
		m_fSwimXZSpeed	= pRider->m_fSwimXZSpeed; 
		m_fJumpXZSpeed	= pRider->m_fJumpXZSpeed; 
		m_fJumpYSpeed	= pRider->m_fJumpYSpeed;  
		m_fDropXZSpeed	= pRider->m_fDropXZSpeed; 
		m_fSlideSpeed	= pRider->m_fSlideSpeed;  
		m_fMountXZSpeed	= pRider->m_fMountXZSpeed;
	}
}
VOID	Role::RestorOwnerSpeed(VOID)
{
	if( !m_ownerSpeeds.bSetted )
		return ;
	m_ownerSpeeds.bSetted = FALSE;
	m_fXZSpeed			   =  m_ownerSpeeds.m_fXZSpeed			;
	m_fYSpeed			   =  m_ownerSpeeds.m_fYSpeed				;
	m_fSwimXZSpeed	       =  m_ownerSpeeds.m_fSwimXZSpeed	;
	m_fJumpXZSpeed	       =  m_ownerSpeeds.m_fJumpXZSpeed	;                         
	m_fJumpYSpeed		   =  m_ownerSpeeds.m_fJumpYSpeed		;                         
	m_fDropXZSpeed	       =  m_ownerSpeeds.m_fDropXZSpeed	;                         
	m_fSlideSpeed		   =  m_ownerSpeeds.m_fSlideSpeed		;                         
	m_fMountXZSpeed	       =  m_ownerSpeeds.m_fMountXZSpeed	;                         
}


VOID Role::SaveTheValueBeforeStrengthen(VOID)
{
	m_stuValueBeforeStrengthen.nHP		= GetAttValue(ERA_HP);
	m_stuValueBeforeStrengthen.nMP		= GetAttValue(ERA_MP);
	m_stuValueBeforeStrengthen.nVitality= GetAttValue(ERA_Vitality);
	m_stuValueBeforeStrengthen.nEndurance= GetAttValue(ERA_Endurance);
}
VOID Role::RestoreTheValueAfterStrengthen(VOID)
{
	INT val;
	if( m_stuValueBeforeStrengthen.nHP != GetAttValue(ERA_HP) )
	{
		val = min(GetAttValue(ERA_MaxHP),m_stuValueBeforeStrengthen.nHP);
		SetAttValue(ERA_HP,val,TRUE);
	}
	if( m_stuValueBeforeStrengthen.nMP != GetAttValue(ERA_MP) )
	{
		val = min(GetAttValue(ERA_MaxMP),m_stuValueBeforeStrengthen.nMP);
		SetAttValue(ERA_MP,val,TRUE);
	}
	if( m_stuValueBeforeStrengthen.nVitality != GetAttValue(ERA_Vitality) )
	{
		val = min(GetAttValue(ERA_MaxVitality),m_stuValueBeforeStrengthen.nVitality);
		SetAttValue(ERA_Vitality,val,TRUE);
	}
	if( m_stuValueBeforeStrengthen.nEndurance != GetAttValue(ERA_Endurance) )
	{
		val = min(GetAttValue(ERA_MaxEndurance),m_stuValueBeforeStrengthen.nEndurance);
		SetAttValue(ERA_Endurance,val,TRUE);
	}
}


VOID Role::CalFabaoMatchTriggerBuff(const tagFabao * pFabao,bool bEquip,INT nPos)
{
	//if( P_VALID(pFabao) )
	//{
	//	pFun_RegTriggerEquipSet pRegTriggerEquipSet = &Role::RegisterTriggerEquipSet;
	//	if(!bEquip)
	//	{
	//		pRegTriggerEquipSet = &Role::UnRegisterTriggerEquipSet;
	//	}
	//	if( pFabao->bySlotLingzhuMatch >= 3 && pFabao->bySlotLingzhuMatch <= MaxWuxingSlotNum )
	//	{
	//		for(INT i = 0; i <= pFabao->bySlotLingzhuMatch - 3; ++i)
	//		{
	//			INT xsNdx = pFabao->nWuxingMatchReward[i];
	//			const tagFabaoWuxingProto * pFabaoWuxingProto = g_attRes.GetFabaoWuxingProto(xsNdx);
	//			if( P_VALID(pFabaoWuxingProto) )
	//			{
	//				const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(pFabaoWuxingProto->dwTriggerID);
	//				if( P_VALID(pTriggerProto) )
	//				{
	//					//if( ETEE_Equip == pTriggerProto->eEventType )
	//						(this->*pRegTriggerEquipSet)(pFabaoWuxingProto->dwTriggerID, pFabaoWuxingProto->dwBuffID, EEP_Face);
	//				}
	//			}
	//		}
	//	}
	//}
}

VOID Role::TriggerFabaoMatchBuff(const tagFabao * pFabao)
{
	//if( P_VALID(pFabao) )
	//{
	//	//INT len = m_vecFabaoMatchTriggerBuff.size();
	//	for(INT i = 0; i <= pFabao->bySlotLingzhuMatch - 3; ++i)
	//	{
	//		INT xsNdx = pFabao->nWuxingMatchReward[i];
	//		const tagFabaoWuxingProto * pFabaoWuxingProto = g_attRes.GetFabaoWuxingProto(xsNdx);
	//		if( P_VALID(pFabaoWuxingProto) )
	//		{
	//			const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(pFabaoWuxingProto->dwTriggerID);

	//			const tagBuffProto*	pBuffProto = g_attRes.GetBuffProto(pFabaoWuxingProto->dwBuffID);

	//			if( !P_VALID(pTriggerProto) || !P_VALID(pBuffProto) )
	//				continue;

	//			if(  ETEE_Equip == pTriggerProto->eEventType  )
	//				continue;

	//			TryAddBuff(this, pBuffProto, pTriggerProto, NULL, const_cast<tagItem*>((tagItem*)pFabao) );
	//		}
	//	}
	//}
}

DWORD Role::LearnSpecSkillLimit(PSpecSkillUsedParam param)
{
	DWORD re = E_Success;
	if( !param )
		return GT_INVALID;
	TSpecSkillUsedFunc func[3] = 
	{
		param->checkfunc,
		param->calcostfunc,
		param->learnfunc
	};
	for( INT i = 0; i < 3; ++i )
	{
		if( func[i] )
			re = (this->*func[i])(param->dwSkillID,param->dwNPCID,param->n64ItemID);
		if( re != E_Success )
			return re;
	}
	return re;
}

DWORD Role::LearnSoaringSkill(DWORD dwSkillID)
{
	Skill* pSkill = GetSkill(dwSkillID);
	//if( !P_VALID(pSkill) )
	//	return GT_INVALID;
	
	tagSpecSkillUsedParam param;
	param.dwSkillID = P_VALID(pSkill) ? Skill::CreateTypeID(pSkill->GetID(), pSkill->GetLevel() + 1) : dwSkillID * 100 + 1;
	param.dwNPCID = 0;
	param.n64ItemID = 0;
	param.checkfunc = &Role::CheckSoaringSkill;
	param.calcostfunc = &Role::CalSoaringSkillCost;
	param.learnfunc = &Role::LearnSoaringSkillSpec;
	return LearnSpecSkillLimit(&param);
}

DWORD Role::CheckSoaringSkill(DWORD dwSkillTypeID,DWORD dwNPCID,INT64 n64ItemID)
{
	Skill* pSkill = GetSkill(Skill::GetIDFromTypeID(dwSkillTypeID));
	if( P_VALID(pSkill) )
	{
		if( pSkill->GetLevel() >= pSkill->GetMaxLearnLevel() )
			return E_LevelUpSkill_ExceedMaxLearnLevel;
	}

	//�Ҳ���ԭ��
	const tagSkillProto* pSkillProto  =  g_attRes.GetSkillProto(dwSkillTypeID);
	if(!P_VALID(pSkillProto))
		return GT_INVALID;

	if( !pSkillProto->bSoarLimitLearn || GetAttValue(ERA_Soar) != ESV_SoaringUp )
		return E_LearnSkill_NotSoaring;

	//�õ���������ԭ��
	const tagLearnSoaringSkillProto *pSoaringSkillProto = g_attRes.GetOneLearnSoaringSkillProto(dwSkillTypeID);
	if(!P_VALID(pSoaringSkillProto))
		return GT_INVALID;

	//����Ƿ񳬹���ѧ�ȼ�����
	if(  Skill::GetLevelFromTypeID(dwSkillTypeID) > pSkillProto->nMaxLearnLevel )
		return E_LevelUpSkill_ExceedMaxLearnLevel;

	//��ǰ�������
	if(m_nSoaringSkillLearnTimes < 1)
	{
		return ERSSEC_TimesLimitted;
	}

	//��Ǯ
	if(GetCurMgr().GetBagSilver() < pSoaringSkillProto->nLearnSilverCosted)
	{
		return ERSSEC_SilverLimitted;
	}

	//��ǰ����
	if(( GetCurLevelExp() - pSoaringSkillProto->nExpCosted ) < 0)
	{
		return ERSSEC_ExpLimitted;
	}

	//��Ʒ
	if(GetItemMgr().GetBagSameItemCount(pSoaringSkillProto->dwItemNeeded) < pSoaringSkillProto->nItemCount)
		return ERSSEC_ItemLimitted;

	const tagSkillProto * pProto = pSkillProto;

	// �������ȼ�
	if( m_nLevel < pProto->nNeedRoleLevel )
		return E_LearnSkill_NeedMoreLevel;

	// ���ְҵ����
	if( E_Success != VocationCheck( pProto->dwVocationLimit ))
		return E_LearnSkill_ClassLimit;

	// ���ǰ�ü���
	if( GT_INVALID != pProto->dwPreLevelSkillID )
	{
		INT nRet = CheckPreSkill(pProto->dwPreLevelSkillID);
		if (E_Success != nRet)
		{
			return nRet;
		}
	}

	return ERSSEC_Success;
}
DWORD Role::CalSoaringSkillCost(DWORD dwSkillTypeID,DWORD dwNPCID,INT64 n64ItemID)
{	
	//�Ҳ���ԭ��
	const tagSkillProto* pSkillProto  =  g_attRes.GetSkillProto(dwSkillTypeID);
	if(!P_VALID(pSkillProto))
		return GT_INVALID;

	//�õ���������ԭ��
	const tagLearnSoaringSkillProto *pSoaringSkillProto = g_attRes.GetOneLearnSoaringSkillProto(dwSkillTypeID);
	if(!P_VALID(pSoaringSkillProto))
		return GT_INVALID;

	//���������
	--m_nSoaringSkillLearnTimes;
	if(m_nSoaringSkillLearnTimes < 0)
	{
		m_nSoaringSkillLearnTimes = 0;
		return ERSSEC_SkillPracticeTimesLimitted;
	}

	//��Ǯ
	GetCurMgr().DecBagSilver(pSoaringSkillProto->nLearnSilverCosted, ELCID_PracticeSoaringSkill);

	//��ʵ��ֵ
	ExpChange(INT64(0 - pSoaringSkillProto->nExpCosted));

	//������Ʒ
	GetItemMgr().RemoveFromRole(pSoaringSkillProto->dwItemNeeded, pSoaringSkillProto->nItemCount, ELCID_PracticeSoaringSkill);


	return ERSSEC_Success;
}
DWORD Role::LearnSoaringSkillSpec(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID)
{
	dwSkillID /= 100;
	// ���ü����Ƿ��Ѿ�����
	Skill* pSkill = GetSkill(dwSkillID);
	if( P_VALID(pSkill) )
	{	
		//����������
		ChangeSkillExp(pSkill,1);
	}
	//�����ǰ�������������
	else
	{
		//ѧϰ����
		DWORD dwErrorCode = LearnSkill(dwSkillID);
		if(dwErrorCode != E_Success)
			return dwErrorCode;
	}
	//m_timeLearnSoaringSkill = g_world.GetWorldTime();
	return ERSSEC_Success;
}


VOID Role::OnReadyToSoaring(VOID)
{
	if( P_VALID(m_pScript) )
		m_pScript->OnReadyToSoaring(this);
}

VOID Role::OnCompletePracticeEquipment(DWORD dwEquipTypeID)
{
	if( P_VALID(m_pScript) )
		m_pScript->OnCompletePracticeEquipment(this,dwEquipTypeID);
}
VOID Role::OnCompleteCastingEquipment(DWORD dwEquipTypeID,INT nCastingLevel)
{
	if( P_VALID(m_pScript) )
		m_pScript->OnCompleteCastingEquipment(this,dwEquipTypeID,nCastingLevel);
}

VOID Role::SynRemoteBodyFabaoInfo(tagRemoteRoleFabaoInfo & info)
{
	EquipContainer & container = GetItemMgr().GetEquipBar();
	int size = container.GetCurSpaceSize();
	memset(&info,0,sizeof(tagRemoteRoleFabaoInfo));
	tagEquip * pEquip = NULL;
	for( int i = 0; i < size; ++i )
	{
		pEquip = container.GetItem((INT16)i);
		if( P_VALID(pEquip) )
		{
			if( MIsFaBao(pEquip) && g_world.IsFabaoOpen() )
			{
				tagFabao * pFabao = (tagFabao*)pEquip;
				tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pEquipProto;
				if( P_VALID(pFabaoProto) )
				{
					info.byEquipFabaoFlag = true;
					info.byStage		= pFabao->n16Stage;
					//info.n16FabaoType	= pFabaoProto->eFarbaoType;
					//info.n16WuxingType	= pFabao->eWuxing;
					info.dwTypeID		= pFabao->pEquipProto->dwTypeID;
					break;
				}
			}
		}
	}
}

VOID Role::SetSoaringAtt(INT v)
{
	INT last = GetAttValue(ERA_Soar);
	SetBaseAttValue(ERA_Soar,v);
	RecalAtt();
	if( v == (INT)ESV_SoaringUp )
	{
		if( last != v )
		{
			m_nSoaringSkillLearnTimes = m_nCLearnSoaringSkillTotalTimes;
			tagNS_ResetSoarSkillLearnTimes send_msg;
			send_msg.nCurrLeftNum = GetPracticeSoaringSkillTimes();
			SendMessage(&send_msg,send_msg.dwSize);
		}
	}
}

INT		Role::GetConsolidateTimes() const
{
	return m_nConsolidateTimes;
}
VOID	Role::IncConsolidateTimes(INT add)
{
	m_nConsolidateTimes += add;
	if( m_nConsolidateTimes < 0 )
		m_nConsolidateTimes = 0;
}


VOID Role::StartSkillCoolDown(Skill* pSkill)
{
	ESkillTypeEx skillType = pSkill->GetTypeEx();
	if(skillType == ESSTE_FreshHandNomal || skillType == ESSTE_FreshHandRanged || skillType == ESSTE_FreshHandEnhanced)
	{
		char pBuf[sizeof(tagNS_UpdateSkillCoolDownEx) + sizeof(DWORD)*tagSkillBoxProto::E_SBP_Size];
		tagNS_UpdateSkillCoolDownEx* pSend = (tagNS_UpdateSkillCoolDownEx*)pBuf;	
		tagNS_UpdateSkillCoolDownEx send;
		memcpy(pSend, &send, sizeof(send));
		pSend->nCount = 0;
		Skill* pTemp;
		TMap<DWORD, Skill*>::TMapIterator itor = m_mapSkill.Begin();
		while(m_mapSkill.PeekNext(itor, pTemp))
		{
			if(pTemp->GetTypeEx() == skillType)
			{
				Unit::StartSkillCoolDown(pTemp);
				pSend->dwSkillID[pSend->nCount++] = pTemp->GetID();
				if(pSend->nCount>= tagSkillBoxProto::E_SBP_Size) break;
			}
		}		
		pSend->nCoolDown = pSkill->GetCoolDownCountDown();
		pSend->dwSize = sizeof(tagNS_UpdateSkillCoolDownEx) + sizeof(DWORD)*(pSend->nCount-1);
		SendMessage(pSend, pSend->dwSize);
	}
	else
	{
		Unit::StartSkillCoolDown(pSkill);
		// ���͸��Լ����¼���CD
		tagNS_UpdateSkillCoolDown send;
		send.dwSkillID = pSkill->GetID();
		send.nCoolDown = pSkill->GetCoolDownCountDown();
		SendMessage(&send, send.dwSize);
	}

}

VOID Role::ClearSkillCoolDown(DWORD dwSkillID)
{
	Unit::ClearSkillCoolDown(dwSkillID);

	// ���͸��Լ����¼���CD
	tagNS_UpdateSkillCoolDown send;
	send.dwSkillID = dwSkillID;
	send.nCoolDown = 0;
	SendMessage(&send, send.dwSize);
}

VOID Role::OnScriptNoticeOfFabaoAttChange(Role*pRole,tagFabao * pFabao,INT nEventType)
{
	if( !P_VALID(pRole) || !P_VALID(pFabao) || nEventType < RFAE_LevelUp || nEventType > RFAE_WuxingXSReset )
		return;
	if( P_VALID(pRole->m_pScript) )
		pRole->m_pScript->OnFabaoAttChange(pRole,pFabao,(ERoleFabaoAttEvent)nEventType,pFabao->n16Stage);
}

INT Role::ChangeFabaoPracticeValue( Role * pRole,tagFabao * pFabao,INT addValue,INT & nChanged )
{
	nChanged = 0;
	if( !P_VALID(pFabao) || !P_VALID(pRole) )
		return EFABAO_ChangePractice_Failse;

	if( g_world.GetFabaoLevelLimit() <= pFabao->n16Stage )
		return EFABAO_ChangePractice_AlreadyFull;

	INT nMaxFabaoStage = g_attRes.GetFabaoMaxStage();
	nMaxFabaoStage = min(nMaxFabaoStage,g_world.GetFabaoLevelLimit());
	const tagFabaoLevelProto * pFabaoLevelProto = NULL,*pFabaoLevelProto0 = NULL;

	pFabaoLevelProto0 = g_attRes.GetFabaoLevelProto(pFabao->nSlotNum * 10 + 1);
	pFabaoLevelProto = g_attRes.GetFabaoLevelProto(nMaxFabaoStage);
	if( !P_VALID(pFabaoLevelProto) )
		return EFABAO_ChangePractice_Failse;

	// �ܵ�ֵ
	INT nMaxValue = pFabaoLevelProto->nMinValue + pFabaoLevelProto->nLevelUpWhated;
	if( pFabao->nPracticeValue >= nMaxValue )
		return EFABAO_ChangePractice_Failse;

	BOOL change = FALSE;
	INT nLastLevel = pFabao->n16Stage;

	if( pFabao->nPracticeValue + addValue >= nMaxValue )
	{
		tagFabaoLevelProto * p = NULL;
		p = const_cast<tagFabaoLevelProto *>(g_attRes.GetFabaoLevelProto(nMaxFabaoStage));
		if( P_VALID(p) )
		{
			if( p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue >= 0 )
			{
				addValue = p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue;
			}
			else
				return EFABAO_ChangePractice_AlreadyFull;
		}
		else
			return EFABAO_ChangePractice_Failse;
	}

	// �������������жϣ�Ҫ������
	INT temVal = pFabao->nPracticeValue + addValue;
	for( int i = pFabao->n16Stage; i <= nMaxFabaoStage; ++i )
	{
		const tagFabaoLevelProto * p = g_attRes.GetFabaoLevelProto(i);
		if( P_VALID(p) )
		{
			if( p->nID > g_world.GetFabaoLevelLimit() )
				return EFABAO_ChangePractice_Failse;
			if( i > 59 && (i%10 == 0) )
			{
				if( i != pFabao->n16Stage )
				{
					if( temVal >= p->nMinValue && temVal >= p->nMinValue + p->nLevelUpWhated )
					{
						if( p->nMinUseLevel > pRole->GetLevel() )
							return EFABAO_ChangePractice_RoleLevelLower;
						addValue = p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue - 1;
						break;
					}
				}
			}
			if( temVal >= p->nMinValue && temVal < p->nMinValue + p->nLevelUpWhated )
			{
				if( p->nMinUseLevel > pRole->GetLevel() )
					return EFABAO_ChangePractice_RoleLevelLower;
				break;
			}
		}
	}

	INT nHP = pRole->m_nAtt[ERA_HP];
	INT nMP = pRole->m_nAtt[ERA_MP];
	change = ItemCreator::CanLevelupFabao(pFabao,addValue);
	if( change )
	{
		pRole->ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
	}
	INT nOldZhiZhi = pFabao->n16NativeIntelligence;
	pFabao->nPracticeValue += addValue;

	ItemCreator::RecalFabaoAtt(pFabao);

	if( nLastLevel != pFabao->n16Stage )
		nChanged = pFabao->n16Stage - nLastLevel;

	if( change )
	{
		if( nLastLevel / 10 != pFabao->n16Stage / 10 )
			pRole->SynRemoteFabaoInfo(TRUE);
		if(g_world.GetFabaoElemStuntFlag() && pFabao->n16Stage > 59)
		{
			INT nTemLast = nLastLevel;
			if( nTemLast < 60 )
				nTemLast = 59;
			pFabao->wDamageUpgradeTimes		+= pFabao->n16Stage - nTemLast;
			pFabao->wResistanceUpgradeTimes	+= pFabao->n16Stage - nTemLast;
			if( nLastLevel < 60 )
			{
				pFabao->dwFairySkill[0] = 0;
				pFabao->dwFairySkill[1] = 0;
			}
			//if( nOldZhiZhi != pFabao->n16NativeIntelligence )
			{
				if( pFabao->n16NativeIntelligence < 80 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
					//pFabao->dwFairySkill[2] = 0;
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
				if( nLastLevel < 60 )
				{
					if( P_VALID( ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID ) )
					{
						//for( int nSkillN = 0; nSkillN < MaxFairySkillNum; ++nSkillN )
							if( pFabao->dwFairySkill[0] == 0 )
								pFabao->dwFairySkill[0] = ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID;
					}
				}
			}
			ItemCreator::RealizeFabaoStunt(pFabao,nLastLevel);
			if( pFabao->n16Stage % 10 == 0 || (nLastLevel / 10 != pFabao->n16Stage / 10) )
				pRole->GetItemMgr().UpdateFabaoNirVanaData2DB(*pFabao);
		}
		// ������Դ�������
		pFabao->IncBirthTimes(nLastLevel);
		
		pRole->ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
		INT nMaxHP = pRole->GetAttValue(ERA_MaxHP);
		INT nMaxMP = pRole->GetAttValue(ERA_MaxMP);
		nHP = min(nHP, nMaxHP);
		nMP = min(nMP, nMaxMP);
		pRole->SetAttValue(ERA_HP,nHP/*,FALSE*/);
		pRole->SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 		pRole->RecalAtt();
		//memcpy(&fabaoSpec,(tagFabaoSpec*)pFabao,sizeof(tagFabaoSpec));
		tagNS_FairyLevelChange fabao_levelup_msg;
		fabao_levelup_msg.dwRoleID		= pRole->GetID();
		fabao_levelup_msg.nLevel		= pFabao->n16Stage;
		pRole->SendMessage(&fabao_levelup_msg,fabao_levelup_msg.dwSize);
		Role::OnScriptNoticeOfFabaoAttChange(pRole,pFabao,RFAE_LevelUp);
	}

	pRole->GetItemMgr().UpdateFabaoSpec(*pFabao,change);

	return EFABAO_ChangePractice_Success;
}

BOOL Role::ModWoreFabao(INT nAddValue,INT & nChanged)
{
	nChanged = 0;
	if( nAddValue <= 0 )
		return FALSE;
	tagFabao * pFabao = NULL;
	EquipContainer & con = GetItemMgr().GetEquipBar();

	pFabao = (tagFabao*)con.GetItem(static_cast<INT16>(EEP_Face));
	if( P_VALID(pFabao) &&  pFabao->nUseLevel < GetLevel() )
	{
		nAddValue *= ItemCreator::GetFabaoExpPercent(pFabao);
		if (pFabao->bStoredExpOpen)
		{
			INT nMaxStoreExp = MFaBaoStoredMaxExp(pFabao->n16Stage, pFabao->n16NativeIntelligence);
			INT nStoreExpFreeSpace = nMaxStoreExp - pFabao->nCurStoredExp;
			INT nStoreExp = 0;

			if (nStoreExpFreeSpace >= nAddValue)
			{
				nStoreExp = nAddValue;
				nAddValue = 0;
			}
			else
			{
				nStoreExp = nStoreExpFreeSpace;
				nAddValue -= nStoreExpFreeSpace;
			}
			pFabao->nCurStoredExp += nStoreExp;
		}
		return ChangeFabaoPracticeValue(this,pFabao,nAddValue,nChanged) == EFABAO_ChangePractice_Success;
	}
	return FALSE;
}

BOOL Role::InSamePKState(Role * pRole)
{
	if( !P_VALID(pRole) )
		return FALSE;
	DWORD myStatus = GetRoleState();
	DWORD otherStatus = pRole->GetRoleState();
	if( (myStatus & ERS_PKEX) && (otherStatus & ERS_PKEX) )
		return TRUE;
	else if(  (myStatus & ERS_PeaceModel) && (otherStatus & ERS_PeaceModel)  )
		return TRUE;
	else if(  (myStatus & ERS_PK) && (otherStatus & ERS_PK)  )
		return TRUE;
	if( !(myStatus & (ERS_PK | ERS_PeaceModel | ERS_PKEX) ) && !(otherStatus & (ERS_PK | ERS_PeaceModel | ERS_PKEX)) )
		return TRUE;
	return FALSE;
}

DWORD Role::TestAttackDefenseRelationship(DWORD dwRoleID)
{
	DWORD re = EAD_NULL;	
	set<DWORD>::iterator i;
	i = m_AttackSet.find(dwRoleID);
	if( i != m_AttackSet.end() )
		re |= EAD_Attack;
	i = m_DefenseSet.find(dwRoleID);
	if( i != m_DefenseSet.end() )
		re |= EAD_Defense;
	return re;
}

static VOID Role_RemoveAttackDefenseRelationship(set<DWORD>& sets,Role * pSrc)
{
	set<DWORD>::iterator i;
	for( i = sets.begin(); i != sets.end(); ++i )
	{
		Role * ply = g_roleMgr.GetRolePtrByID(*i);
		if( P_VALID(ply) )
		{
			if( pSrc->GetMapID() != ply->GetMapID() )
			{
				MapMgr * pMgr = g_mapCreator.GetMapMgr(pSrc->GetMapID());
				if( P_VALID(pMgr) )
				{
					pMgr->DelayedRemoveAttackDefenseRelationship(pSrc->GetID(),ply->GetID());
				}
			}
			else
				ply->RemoveAttackDefenseRelationship(pSrc->GetID());
		}
	}
	sets.clear();
}

VOID Role::ClearAttackDefenseRelationship(VOID)
{
	Role_RemoveAttackDefenseRelationship(m_AttackSet,this);
	Role_RemoveAttackDefenseRelationship(m_DefenseSet,this);
}
VOID Role::RemoveAttackDefenseRelationship(DWORD dwRoleID)
{
	m_AttackSet.erase(dwRoleID);
	m_DefenseSet.erase(dwRoleID);
}
VOID Role::AddAttackRelationship(DWORD dwRoleID)
{
	m_AttackSet.insert(dwRoleID);
}
VOID Role::AddDefenseRelationship(DWORD dwRoleID)
{
	m_DefenseSet.insert(dwRoleID);
}

// �ж��Ƿ���Teamʰȡ��Χ��
BOOL Role::IsInTeamPickupRank(DWORD dwMapID, const Vector3& vPos)
{
	if(GetMapID() == dwMapID && Vec3DistSq(GetCurPos(), vPos) <= X_DEF_PICKUP_DIST_SQ)
		return TRUE;
	else
		return FALSE;
}

//  ������������ֳɵ�ExpChange 
VOID Role::ExpChangeWithFabaoExp(INT64 nValue, BOOL bKill, BOOL bForce, INT nFabaoChanged)
{
	if( 0 == nValue ) return;

	// ���پ��飬����������
	if( nValue < 0 )
	{
		INT64 nTemp = m_nCurLevelExp;
		m_nCurLevelExp += nValue;
		if( m_nCurLevelExp < 0 ) m_nCurLevelExp = 0;

		tagNS_RoleExpChange send;
		send.nExp = m_nCurLevelExp;
		send.nChange = m_nCurLevelExp - nTemp;
		send.bKill = bKill;
		send.bShare = false;
		SendMessage(&send, send.dwSize);

		if (!bKill)
		{
			m_bSaveExpDirect = TRUE;
			SaveExp2DB();
		}

		return;
	}

	// ��������Ӿ���
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(m_nLevel);
	INT64 nLevelUpExpRemain = pEffect->nExpLevelUp - m_nCurLevelExp;
	ASSERT( nLevelUpExpRemain >= 0 );

	// ��������
	if( nValue < nLevelUpExpRemain )
	{
		// ����Ƿ���10%�ı�����������ˣ������һ���Ѫ������
		//INT nPhaseExp = pEffect->nExpLevelUp / 10;
		//INT nOldPhase = m_nCurLevelExp / nPhaseExp;
		//INT nNewPhase = (m_nCurLevelExp + nValue) / nPhaseExp;
		//if( nOldPhase != nNewPhase )
		//{
		//	SetAttValue(ERA_HP, GetAttValue(ERA_MaxHP));
		//	SetAttValue(ERA_MP, GetAttValue(ERA_MaxMP));
		//}

		// �ۼ����µľ���
		m_nCurLevelExp += nValue;

		//INT nFaBaoExp = GetSpecSkillValue(ESSF_FabaoExp,nValue * 0.2f);
		INT nFaBaoExp = GetSpecSkillValue(ESSF_FabaoExp,nValue);

		tagNS_RoleExpChange send;
		send.nExp = m_nCurLevelExp;
		send.nChange = nValue;
		send.bKill = bKill;
		send.nFairyExpChange = nFaBaoExp;
		send.nFairyLvInc	= nFabaoChanged;
		send.bShare = false;
		SendMessage(&send, send.dwSize);
	}

	// ��Ҫ����
	else
	{
		// �����Ƿ�ǿ���������������ܵĵȼ�����
		INT nMaxLevel = g_roleMgr.GetRoleLevelLimit();
		// Jason ������������
		//if( GetAttValue(ERA_Soar) == ESV_SoaringUp )
		//{
		//	nMaxLevel = m_nLevel;
		//}
		/*else */if( bForce )
		{
			nMaxLevel = MAX_ROLE_LEVEL;
		}

		m_nCurLevelExp += nLevelUpExpRemain;

		// �Ѿ���������
		if( m_nLevel == nMaxLevel )
		{
			if( nLevelUpExpRemain != 0 )
			{
				tagNS_RoleExpChange send;
				send.nExp = m_nCurLevelExp;
				send.nChange = nLevelUpExpRemain;
				send.bKill = bKill;
				send.bShare = false;
				//INT nFaBaoExp = GetSpecSkillValue(ESSF_FabaoExp,nValue * 0.2f);
				INT nFaBaoExp = GetSpecSkillValue(ESSF_FabaoExp,nValue);
				send.nFairyExpChange = nFaBaoExp;
				send.nFairyLvInc	= nFabaoChanged;
				SendMessage(&send, send.dwSize);
			}
		}

		// ��������
		else
		{
			m_bSaveExpDirect = TRUE;
			INT64 nTemExpValue = nValue;
			INT64 nExpRealAdd = nLevelUpExpRemain;
			nValue -= nLevelUpExpRemain;
			INT nNextLevel = m_nLevel + 1;
			INT nAttPointAdd = 0;
			INT nTalentPointAdd = 0;

			// ��������������
			for(; nNextLevel <= nMaxLevel; nNextLevel++)
			{
				pEffect = g_attRes.GetLevelUpEffect(nNextLevel);

				// �������ټ�������
				if( pEffect->nExpLevelUp <= nValue )
				{
					nValue -= pEffect->nExpLevelUp;
					nExpRealAdd = pEffect->nExpLevelUp;
					nAttPointAdd += pEffect->n16RoleAttAvail;
					nTalentPointAdd += pEffect->n16TalentAvail;
				}
				// ��������������
				else
				{
					nAttPointAdd += pEffect->n16RoleAttAvail;
					nTalentPointAdd += pEffect->n16TalentAvail;
					break;
				}
			}

			// ������ı�ȼ�
			if( nNextLevel > nMaxLevel )
			{
				nNextLevel = nMaxLevel;
				// ��������һ��
				if( nValue > pEffect->nExpLevelUp )
				{
					nValue = pEffect->nExpLevelUp;
				}
			}

			LevelChange(nNextLevel, bKill, bForce);

			m_nCurLevelExp = nValue;
			nExpRealAdd += nValue;

			// ������������Ե�����ʵ�
			ModAttValue(ERA_AttPoint, nAttPointAdd);

			//���ʻ������
			//�������������ʽ�������
			tagNS_RoleGetTalentPoint sendMsg ;
			INT nLevel = nNextLevel;
			INT nCurrentTalent = GetAttValue(ERA_TalentPoint); //��ȡ��ǰ�Ŀ�Ͷ����
			INT nCurrentTotalTalent = nCurrentTalent + GetTotalTalentPoint(); //��ȡ��ǰ������(������Ͷ����)
			INT nCurrentTalentAvail_Limit = GetTalentAvail_limit(nLevel);  //��ȡ��ǰ����ɻ���������
			INT nRew_att_val = 0;
			if ( nCurrentTotalTalent >= nCurrentTalentAvail_Limit) // ��ǰ��������������ǰ�������ɻ������ �Ͳ�������
			{
				nRew_att_val = 0;
				sendMsg.dwErrorCode = ERTEC_OverMaxTalentAvail_Limit;
			}
			else
			{
				nRew_att_val = nTalentPointAdd;
				INT nTotalTalent = nRew_att_val + nCurrentTotalTalent ;  // ��ǰ�������ʼ������ͻ������ 
				if ( nTotalTalent > nCurrentTalentAvail_Limit)
				{
					nRew_att_val = nCurrentTalentAvail_Limit - nCurrentTotalTalent;
					sendMsg.dwErrorCode = ERTEC_OverMaxTalentAvail_Limit;
				}
				else
				{
					sendMsg.dwErrorCode = ERTEC_Success; 
				}

			}
			SendMessage(&sendMsg,sendMsg.dwSize);
			ModAttValue(ERA_TalentPoint, nRew_att_val);


			// ���;���ı�
			tagNS_RoleExpChange send;
			send.nExp = m_nCurLevelExp;
			send.nChange = nTemExpValue;//nExpRealAdd;
			send.bKill = bKill;
			send.bShare = false;
			//INT nFaBaoExp = GetSpecSkillValue(ESSF_FabaoExp,nValue * 0.2f);
			INT nFaBaoExp = GetSpecSkillValue(ESSF_FabaoExp,nValue);
			send.nFairyExpChange = nFaBaoExp;
			send.nFairyLvInc	= nFabaoChanged;
			SendMessage(&send, send.dwSize);

			SaveAttPoint2DB();
			SaveTalentPoint2DB(0);
			//m_SaveRoleLock.Acquire();
			//SaveToDB();
			//m_SaveRoleLock.Release();
		}
	}
	if (!bKill)
	{
		SaveExp2DB();
	}
}

VOID Role::KillCountInc(ERoleKillType eRoleKillType)
{
	if (!g_world.IsKillCounterOpen())
	{
		return;
	}

	tagNS_SendRoleKillCount send;
	send.eRoleKillType = eRoleKillType;
	if (eRoleKillType == ERKT_Monster)
	{
		send.dwKillCount = ++m_dwMonsterKillCount;
	}
	else if (eRoleKillType == ERKT_Role)
	{
		send.dwKillCount = ++m_dwRoleKillCount;
	}

	if (send.dwKillCount > 0)
	{
		SendMessage(&send, send.dwSize);
	}

	if (eRoleKillType == ERKT_Monster)
	{
		BOOL bAddCrazyBuff = FALSE;
		BOOL bAddZhaoMoBuff = FALSE;

		// ����ɫ��ӻ�ɱ�����ۼ�Ч��
		if (m_dwMonsterKillCount % 100 == 0)
		{
			bAddZhaoMoBuff = TRUE;
			bAddCrazyBuff = TRUE;
		}
		if (m_nLevel <= 10 && (m_dwMonsterKillCount == 5 || m_dwMonsterKillCount == 20 || m_dwMonsterKillCount== 40 || m_dwMonsterKillCount == 60 || m_dwMonsterKillCount == 80))
		{
			bAddCrazyBuff = TRUE;
		}
		else if (m_nLevel <= 20 && (m_dwMonsterKillCount == 20 || m_dwMonsterKillCount == 50))
		{
			bAddCrazyBuff = TRUE;
		}
		if (bAddZhaoMoBuff)
		{
			// ���Լ������ħbuff
			TryAddBuffToMyself(9001201);
		}
		if (bAddCrazyBuff)
		{
			// ���Լ���ӷ��buff
			TryAddBuffToMyself(9001301);
		}
	}
}

VOID Role::TryAddBuffToMyself(DWORD dwBuffID)
{
	// ȡ��buff����
	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffID);
	if ( P_VALID(pProto) )
	{
		TryAddBuff((Unit*)this, pProto, NULL, NULL, NULL);
	}
}

VOID Role::UpdateCurMonthOnlineTime(BOOL bForceSave)
{
	PlayerSession* pSession = GetSession();
	if (!P_VALID(pSession))
		return;

	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	DWORD dwTimeDiff = CalcTimeDiff(dwCurTime, m_CurMonthOnlineTime.dwCurTime);
	if (dwTimeDiff < 600 && !bForceSave)
		return;

	UINT16 nMinDiff = ((dwTimeDiff%60) > 30)?(dwTimeDiff/60+1):dwTimeDiff/60;
	tagDWORDTime dwLastTime = static_cast<tagDWORDTime>(m_CurMonthOnlineTime.dwCurTime);
	DWORD dwCurMonth = (dwCurTime.month > 1)? dwCurTime.month-1 : 0;
	if (dwLastTime.month != dwCurTime.month)
		m_CurMonthOnlineTime.n16OnlineTime[dwCurMonth] = 0;
	m_CurMonthOnlineTime.dwCurTime = dwCurTime;
	m_CurMonthOnlineTime.n16OnlineTime[dwCurMonth] += nMinDiff;

	tagNDBC_SaveOnlineTimeEachMonth send;
	send.dwAccountID = pSession->GetSessionID();
	send.dwRoleID = GetID();
	send.byYear = dwCurTime.year;
	for (int nMonthIdx = EM_January; nMonthIdx < EM_End; nMonthIdx++)
	{
		send.n16OnlineTime[nMonthIdx] = m_CurMonthOnlineTime.n16OnlineTime[nMonthIdx];
	}
	g_dbSession.Send(&send, send.dwSize);
}

VOID Role::UpdateVipLevel()
{
	EVipLevel eVipLevel = static_cast<EVipLevel>(GetAttValue(ERA_VipLevel));
	if (eVipLevel == EVL_None || m_dwVipStartTime == 0 || m_dwVipMaxDays == 0)
		return;

	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	DWORD dwTimePassed = CalcTimeDiff(dwCurTime, m_dwVipStartTime);
	if (dwTimePassed >= MAX_DAYS_SECOND(m_dwVipMaxDays))
	{
		ResetVipState();
		SetAttValue(ERA_VipLevel, EVL_None);
	}
}

VOID Role::ResetVipState()
{
	// VIP��buff��������������ֻҪɾ��һ�������buff�Ϳ�����
	RemoveBuff(Buff::GetIDFromTypeID(SILVER_VIP_BUFF), TRUE);
	SetFreeWorldTalkTimes(0);		// ������Ѻ�������
	m_dwVipStartTime = 0;	// ����
	m_dwVipMaxDays = 0;
	SetAttValue(ERA_VipLevel, EVL_None);
}

VOID Role::AddVipBuff(EVipLevel eVipLevel)
{
	// ��ɾ��VIP��buff(��ΪVIP�������ȼ���buff��ͬһ��buff�������ȼ�������ֻҪɾ��һ���Ϳ�����)
	RemoveBuff(Buff::GetIDFromTypeID(SILVER_VIP_BUFF), TRUE);
	if (eVipLevel == EVL_Silver)
	{
		const tagBuffProto* pProto = g_attRes.GetBuffProto( SILVER_VIP_BUFF );
		if( P_VALID(pProto) )
			TryAddBuff(this, pProto, NULL, NULL, NULL);
	}
	else if (eVipLevel == EVL_Gold)
	{
		const tagBuffProto* pProto = g_attRes.GetBuffProto( GOLDEN_VIP_BUFF );
		if( P_VALID(pProto) )
			TryAddBuff(this, pProto, NULL, NULL, NULL);
	}
	else if (eVipLevel == EVL_Platinum)
	{
		const tagBuffProto* pProto = g_attRes.GetBuffProto( PLATINUM_VIP_BUFF );
		if( P_VALID(pProto) )
			TryAddBuff(this, pProto, NULL, NULL, NULL);
	}
}

VOID Role::CheckVipStateWhenLogin()
{
	EVipLevel eVipLevel = static_cast<EVipLevel>(GetAttValue(ERA_VipLevel));
	if (eVipLevel == EVL_Silver || eVipLevel == EVL_Gold || eVipLevel == EVL_Platinum)
	{
		if (m_dwVipStartTime == 0)
			m_dwVipStartTime = GetCurrentDWORDTime();
		if (m_dwVipMaxDays == 0)
			m_dwVipMaxDays = DEFAULT_MAX_VIP_DAYS;

		AddVipBuff(eVipLevel);
	}
	else
	{
		ResetVipState();
	}
}

VOID Role::UseVipCard(INT64 n64ItemID)
{
	tagItem* pItem = GetItemMgr().GetBagItem(n64ItemID);
	if (!P_VALID(pItem))
		return;
	
	if (!P_VALID(pItem->pProtoType))
		return;

	if (pItem->pProtoType->eSpecFunc != EISF_VipCard ||
		pItem->pProtoType->nSpecFuncVal1 <= EVL_None ||
		pItem->pProtoType->nSpecFuncVal1 >= EVL_Max)
		return;
	
	if (pItem->pProtoType->nSpecFuncVal2 <= 0)
		return;

	if (GetAttValue(ERA_VipLevel) > pItem->pProtoType->nSpecFuncVal1)
		return;

	EVipLevel eVipLevel = static_cast<EVipLevel>(pItem->pProtoType->nSpecFuncVal1);
	SetAttValue(ERA_VipLevel, eVipLevel);
	
	// ������Ѻ�������
	SetFreeWorldTalkTimes(0);

	// ����VIP��ݵ���ʼʱ��
	m_dwVipStartTime = GetCurrentDWORDTime();
	m_dwVipMaxDays = pItem->pProtoType->nSpecFuncVal2;

	// ͬ��VIP��Ƭ���߿�ʼʹ��ʱ��
	tagNS_SyncStartVipTime send;
	send.dwStartVipTime = m_dwVipStartTime;
	send.dwVipMaxDays = m_dwVipMaxDays;
	SendMessage(&send, send.dwSize);

	//�۳�����
	GetItemMgr().DelFromBag(n64ItemID, ELCLD_Item_UseVipCard);

	AddVipBuff(eVipLevel);
}

// �������ϵ�����
VOID Role::SaveRoleFabaoChange(VOID)
{
	tagFabao * pFabao = NULL;
	EquipContainer & con = GetItemMgr().GetEquipBar();
	INT16 nEquipNum = con.GetCurSpaceSize();
	tagEquip * pEquip = NULL;

	for(INT16 i = 0 ; i < nEquipNum; ++i)
	{
		pEquip = con.GetItem(i);
		if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
		{
			pFabao = (tagFabao*)pEquip;
			GetItemMgr().SendFabaoSpec2DB(*pFabao);
			break;
		}
	}
}

void Role::UpdateChaseHolderOnWater()
{
	if (IsInRoleState(ERS_HoldChest) && GetMoveData().IsWillSwim())
	{
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if (P_VALID(pRoleScript))
			pRoleScript->ForceLootChase(this);
	}
}

// ������������
VOID Role::UpdateFabaoMood(VOID)
{
	// ÿ6���Ӹ���һ��
	static const INT nSexMins = TICK_PER_SECOND*60*6;
	if( ++m_nFabaoMoodUpdateTicks % (TICK_PER_SECOND*60) == 0 )
	{
		tagEquip * pEquip = GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
		if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
		{
			tagFabao * pFabao = (tagFabao*)pEquip;
			pFabao->nMoodTicks += TICK_PER_SECOND*60;
			if( pFabao->nMoodTicks < nSexMins )
				return;
			if( pFabao->n16Stage < 30 )
				return;
			pFabao->nMoodTicks = pFabao->nMoodTicks - nSexMins;

			INT nOldMood = pFabao->nMood;
			INT nHP = m_nAtt[ERA_HP];
			INT nMP = m_nAtt[ERA_MP];
			ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
			INT nSpeed = ItemCreator::GetFabaoFeelingdown(pFabao);
			ItemCreator::ModFabaoMood(pFabao,-nSpeed);
			ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
			INT nMaxHP = GetAttValue(ERA_MaxHP);
			INT nMaxMP = GetAttValue(ERA_MaxMP);
			nHP = min(nHP, nMaxHP);
			nMP = min(nMP, nMaxMP);
			SetAttValue(ERA_HP,nHP/*,FALSE*/);
			SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 			RecalAtt();
			if( pFabao->nMood != nOldMood )
			{
				GetItemMgr().SynchFabaoMood(*pFabao,nOldMood,TRUE);
				if( pFabao->nMood < 1000 )
				{
					if( P_VALID(pFabao->dwMoodBuffID) )
					{
						if( IsHaveBuff(Buff::GetIDFromTypeID(pFabao->dwMoodBuffID)) )
						{
							RemoveBuff(Buff::GetIDFromTypeID(pFabao->dwMoodBuffID),TRUE);
							pFabao->dwMoodBuffID = GT_INVALID;
						}
					}
				}
			}
		}
	}
}

// ʥ��������ظ��£����ٻ�ʥ��ʱ��ɫʥ��ֵ����
VOID Role::UpdateHolyData(VOID)
{
	//tagHolyMan * pHoly = NULL;	


	//// ÿ����20�룬��������ʥ���Ĭ��ֵ��1,Ĭ�������ֵ��100
	//if ( --m_nCoValueTicks <= 0 )
	//{
	//	m_nCoValueTicks = HOLY_COVALUE_INTER_TICK;
	//	tagEquip * pEquip =GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);	
	//	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsHoly(pEquip) )
	//	{
	//		pHoly = (tagHolyMan*)pEquip;
	//	}

	//	if (P_VALID(pHoly))
	//	{
	//		if (pHoly->nCoValue + 20 < 100)
	//		{
	//			pHoly->nCoValue += 20;									
	//		}	
	//		else
	//		{
	//			pHoly->nCoValue = 100;			
	//		}

	//		ItemMgr &itemMgr = GetItemMgr();
	//		// ʥ�������޸�֪ͨ�ͻ���
	//		itemMgr.SynchHolyCoValue(*pHoly, TRUE);
	//	}
	//}

// 	if ( --m_nHolyTicks <= 0)	//������ʥ��ֵ��
// 	{
// 		m_nHolyTicks = COST_HOLY_INTER_TICK;
// 
// 		if ( IsInRoleState(ERS_CALLHOLY))
// 		{	
// 			if (!P_VALID(pHoly))
// 			{
// 				tagEquip * pEquip =GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);	
// 				if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsHoly(pEquip) )
// 				{
// 					pHoly = (tagHolyMan*)pEquip;
// 				}
// 			}
// 			if (P_VALID(pHoly))
// 			{
// 				tagHolyProto * pHolyProto = (tagHolyProto*)pHoly->pProtoType;
// 				// ��������ʥ��ֵС���ٻ�ʥ��Ҫ���ĵ�ʥ��ֵ����ȡ���ٻ�
// 				if (GetAttValue(ERA_Holy) < pHolyProto->n16CallConsumption * 5)
// 				{
// 					UnsetRoleState(ERS_CALLHOLY, FALSE);
// 					// ֪ͨ�ͻ��� �ٻ�ʥ����ʧ
// 					tagNS_CallShengLingDisappear msg;
// 					msg.dwPlayerID = GetID();
// 					Map* pMap = GetMap();
// 					if (P_VALID(pMap))
// 					{
// 						pMap->SendBigVisTileMsg(this, &msg, msg.dwSize);
// 					}				
// 				}
// 				else
// 				{
// 					// ������ٻ�ʥ�飬��ÿ������һ��ʥ��ֵ
// 					SetAttValue(ERA_Holy, (GetAttValue(ERA_Holy) - pHolyProto->n16CallConsumption * 5));							
// 				}
// 			}					
// 
// 
// 		}
// 
// 		
// 	}		

}
INT	 Role::ModFabaoMood(INT v)
{
	tagEquip * pEquip = GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		tagFabao * pFabao = (tagFabao*)pEquip;
		INT nOldMood = pFabao->nMood;
		int re = ItemCreator::ModFabaoMood((tagFabao*)pEquip,v);
		if( pFabao->nMood != nOldMood )
		{
			GetItemMgr().SynchFabaoMood(*pFabao,nOldMood,TRUE);
		}
	}
	return GT_INVALID;
}

// ���������������Ԫ���˺�������
FLOAT Role::CalAdditionalDmg(Skill * pSkill)
{
	return 0;
}
// ���������Ŀ��Ե�Ӱ��
FLOAT Role::CalAdditionalResistance(Skill * pSkill)
{
	return 0;
}

// ����һԪ�ƻ�ԭ�ͱ���ͻ���
VOID Role::SyncMallOfferProto()
{
	TMap<DWORD, tagMallOfferProto*>& mapMallOfferProto = g_attRes.GetMallOfferProtoMap();
	DWORD dwSize = mapMallOfferProto.Size();
	if (dwSize == 0)
		return;

	DWORD dwIndex = 0;
	tagMallOfferProto* pMallOfferProto = NULL;
	TMap<DWORD, tagMallOfferProto*>::TMapIterator iterator = mapMallOfferProto.Begin();
	
	DWORD dwMsgSize = sizeof(tagNS_SyncMallOfferProto) + (dwSize-1)*sizeof(tagNS_SyncMallOfferProto);

	MCREATE_MSG(pSend, dwMsgSize, NS_SyncMallOfferProto);
	if (!P_VALID(pSend))
		return;

	pSend->dwMallOfferCnt = dwSize;
	pSend->byBuy50LvlItemFlag = GetBuy50LvlItemFlag();
	pSend->byBuy60LvlItemFlag = GetBuy60LvlItemFlag();
	pSend->byBuy70LvlItemFlag = GetBuy70LvlItemFlag();
	pSend->byBuy80LvlItemFlag = GetBuy80LvlItemFlag();
	while (mapMallOfferProto.PeekNext(iterator, pMallOfferProto))
	{
		if (P_VALID(pMallOfferProto))
		{
			pSend->MallOfferProto[dwIndex].dwTypeID = pMallOfferProto->MallOfferProto2Client.dwTypeID;
			pSend->MallOfferProto[dwIndex].nLevel = pMallOfferProto->MallOfferProto2Client.nLevel;
			dwIndex++;
		}
	}
	SendMessage(pSend, pSend->dwSize);
}

BOOL Role::ActiveFabaoStuntBuff(bool bEquip,tagFabao * pFabao,int nEquipPos)
{
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		return FALSE;
	}

	pFun_RegTriggerEquipSet pRegTriggerEquipSet = &Role::RegisterTriggerEquipSet;
	if(!bEquip)
	{
		pRegTriggerEquipSet = &Role::UnRegisterTriggerEquipSet;
	}

	for( int i = 0; i < MaxFairySkillNum; ++i )
	{
		if( P_VALID(pFabao->dwFairySkill[i]) )
		{
			const tagFabaoStuntProto * pProto = g_attRes.GetFabaoStuntProto(pFabao->dwFairySkill[i]);
			if( !P_VALID(pProto) )
				continue;
			//if( IsHaveBuff(Buff::GetIDFromTypeID(pProto->dwBuffID)) )
			//	RemoveBuff(Buff::GetIDFromTypeID(pProto->dwBuffID),TRUE);
			if( bEquip )
			{
				if( pProto->nStuntType == -1 || pProto->nStuntType == (INT)pFabao->eEleInjury )
				{
					(this->*pRegTriggerEquipSet)(pProto->dwTriggerID, pProto->dwBuffID, nEquipPos);
				}
			}
			else
			{
				if( pProto->nStuntType == -1 || pProto->nStuntType == (INT)pFabao->eEleInjury )
				(this->*pRegTriggerEquipSet)(pProto->dwTriggerID, pProto->dwBuffID, nEquipPos);
				//if( IsHaveBuff(Buff::GetIDFromTypeID(pProto->dwBuffID)) )
				//	RemoveBuff(Buff::GetIDFromTypeID(pProto->dwBuffID),TRUE);
			}
		}
	}
	return TRUE;
}

VOID  Role::NoticeClient()
{
	 tagNS_WGCheck_On Msg;	 
	 Msg.bOpen = g_world.IsWGCheck_OnOpen();
	 SendMessage(&Msg,Msg.dwSize);
}

VOID Role::OnGodStrengthenSuccess(tagEquip*pEquip)
{
	if( P_VALID(m_pScript) )
		m_pScript->OnGodStrengthenSuccess(this,pEquip);
}

VOID Role::OnChuanDaoQuest(EChuanDaoType eType)
{
	DWORD dwTeamID = this->GetTeamID();
	if (!P_VALID(dwTeamID))
	{
		return;
	}

	const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
	if (!P_VALID(pTeam))
	{
		return;
	}

	for (int i=0; i<MAX_TEAM_NUM; ++i)
	{
		Role* pMember = pTeam->GetMemPtr(i);
		if (!P_VALID(pMember) || pMember->GetID() == GetID())
			continue;

		if (pMember->GetMapID() != GetMapID())
			continue;

		UINT16 u16Temp = pMember->IsHaveChuanDaoQuest(GetLevel());
		if( !P_VALID(u16Temp))
			continue;

		Quest* pQuest = pMember->GetQuest(u16Temp);
		if(!P_VALID(pQuest))
			continue;

		pQuest->IncChuanDaoCount();
	}
}

BOOL Role::HasTakenGuildDailyReward() const
{
	tagDWORDTime tem = m_timeTakeGuildReward;
	if( (DWORD)tem == 0 )
		return FALSE;
	tagDWORDTime now = g_world.GetWorldTime();
	if( tem.year == now.year && tem.month == now.month && tem.day == now.day )
		return TRUE;
	return FALSE;
}
VOID Role::ResetGuildDailyRewardTime()
{
	m_timeTakeGuildReward = g_world.GetWorldTime();
}
INT		Role::GetBuffWrapTimes(DWORD dwBuffTypeID,DWORD & dwSrcRoleID)
{
	Buff * pBuff = GetBuffPtr(Buff::GetIDFromTypeID(dwBuffTypeID));
	dwSrcRoleID = GT_INVALID;
	if( P_VALID(pBuff) )
	{
		dwSrcRoleID = pBuff->GetSrcUnitID();
		return pBuff->GetWrapTimes();
	}
	return 0;
}

void Role::OnCreateGuild(DWORD dwGuildID)
{
	if( P_VALID(m_pScript) )
		m_pScript->OnCreateGuild(this,dwGuildID);
}
static BOOL Func_TargetOfMe(Unit * pSrc,Unit * pOther)
{
	if( P_VALID(pSrc) && P_VALID(pOther) )
	{
		DWORD dwSrcID = pSrc->GetID();
		DWORD dwDesID = pOther->GetID();
		if( dwSrcID != dwDesID )
		{
			if( pOther->GetAttValue(ERA_Target) == dwSrcID )
				return TRUE;
		}
	}
	return FALSE;
}
VOID	Role::SynTargetChangeToRolesOfVisTile()
{
	Map * pMap = GetMap();
	if( P_VALID(pMap) )
	{
		tagNS_TargetOfTarget msg;
		msg .dwPlayerID = GetID();
		msg .dwTargetID = GetAttValue(ERA_Target);
		pMap->SynVisTileMsgWithCon(this,&msg,msg.dwSize,Func_TargetOfMe);
	}
}

VOID	Role::SynPickupModeSetting2Client()
{
	tagNS_PickItemStrategy msg;
	msg.dwStrategy = m_dwPickupModeSetting;
	SendMessage(&msg,msg.dwSize);
}

VOID	Role::SynRemoteFabaoInfo(BOOL bEquip)
{
	Map * pMap = GetMap();
	if( !P_VALID(pMap) )
		return;

	tagEquip * pEquip = GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	if( !P_VALID(pEquip) || !P_VALID(pEquip->pEquipProto) || !MIsFaBao(pEquip) )
		return;
	tagFabao * pFabao = (tagFabao*)pEquip;

	tagNS_RemoteRoleChangeFabao send;
	tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pEquipProto;
	tagRemoteRoleFabaoInfo & info = send.tagFaBaoIno;
	send.dwRoleID = GetID();
	info.byEquipFabaoFlag = bEquip ? 1 : 0;
	info.byStage		= pFabao->n16Stage;
	//info.n16FabaoType	= pFabaoProto->eFarbaoType;
	//info.n16WuxingType	= pFabao->eWuxing;
	info.dwTypeID		= pFabao->pEquipProto->dwTypeID;
	pMap->SendBigVisTileMsg(this,&send,send.dwSize);
}

bool Role::IsOldSerNewPlayer()
{
	tagDWORDTime dwOldSerNewPlayer = g_world.GetOldSerNewPlayerDate();
	if (!dwOldSerNewPlayer.IsValid())
		return false;

	tagDWORDTime dwCurrentTime = GetCurrentDWORDTime();
	if (CompareDwordTime(dwCurrentTime, dwOldSerNewPlayer) < 0)
		return false;
	return true;
}

VOID Role::ClosePKSafeGuard(BOOL bCloseOrNot)
{
	m_bUsingPkMinLevel = !bCloseOrNot;
	if(!bCloseOrNot)
		return ;

	INT nRet = CloseSafeGuard();

	// ������Ϣ���ͻ���
	tagNS_SafeGuard send;
	send.bOpen = FALSE;
	send.dwErrorCode = nRet;
	SendMessage(&send, send.dwSize);

// 	m_bLevelUpClosedSafeGuard = TRUE;

	//tagNS_SetRoleState srs_msg;
	//srs_msg.dwRoleID	= GetID();
	//srs_msg.eState = ERS_PeaceModel;
	//SendMessage(&srs_msg,srs_msg.dwSize);
	SetPK(-1);
}


EFashionState Role::OnFashionChange(DWORD dwIdx, DWORD& dwLayerID)
{
	dwLayerID = CalLayerID(dwIdx);
	tagWardrobeLayer* pWardrobeLayer = GetWardrobe().GetWardrobeLayer(dwLayerID);
	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(dwLayerID);
	if (!P_VALID(pWardrobeProto) || !P_VALID(pWardrobeLayer))
		return EFS_None;
	DWORD dwFashionID[MAX_FASHION_PERLAYER] = {GT_INVALID};
	DWORD dwStartIdx = (dwLayerID>=1)?((dwLayerID-1)*MAX_FASHION_PERLAYER):0;
	INT16 byFashionTemperament = 0;		// ʱװ����ֵ
	BOOL bNone = TRUE;
	for (INT nCnt = 0; nCnt < MAX_FASHION_PERLAYER; nCnt++)
	{
		tagItem* pItem = GetItemMgr().GetWardrobeContainer().GetItem((INT16)(dwStartIdx+nCnt));
		if (P_VALID(pItem))
		{
			bNone = FALSE;
			dwFashionID[nCnt] = pItem->dwTypeID;
			tagEquip* pEquip = (tagEquip*)pItem;
			if (P_VALID(pEquip))
				byFashionTemperament += pEquip->equipSpec.n16Appearance;
		}
	}
	
	// ��������
	GetWardrobe().SetFashionTemperament(dwLayerID, byFashionTemperament);

	// װ����ѵĲ���״̬Ӱ��
	if (pWardrobeLayer->eState == EFS_UseForFriends)
		return EFS_UseForFriends;

	if (bNone)
		return EFS_None;

	if (pWardrobeProto->eLayerType == EWLT_Normal)
	{
		for (INT nRealCnt = 0; nRealCnt < MAX_FASHION_PERLAYER; nRealCnt++)
		{
			if (pWardrobeProto->dwFashionID[nRealCnt] != 0 &&
				pWardrobeProto->dwFashionID[nRealCnt] != GT_INVALID)
			{
				if (pWardrobeProto->dwFashionID[nRealCnt] != dwFashionID[nRealCnt])
					return EFS_Incomplete;
			}
		}
		return EFS_Complete;
	}
	else if (pWardrobeProto->eLayerType == EWLT_Diy)
	{
		for (INT nDiyCnt = 0; nDiyCnt < MAX_FASHION_PERLAYER-1; nDiyCnt++)
		{
			if (dwFashionID[nDiyCnt] == GT_INVALID || dwFashionID[nDiyCnt] == 0)
				return EFS_Incomplete;
		}
		return EFS_Complete;
	}
	return EFS_Incomplete;
}
//---------------------------------------------------------------------------------
// ���һ����ʱ���ܣ�����
//---------------------------------------------------------------------------------
INT	Role::LearnExtraSkill(DWORD dwSkillID, INT nLevel)
{
	Skill* pSkill = GetSkill(dwSkillID);
	if( P_VALID(pSkill) ) return E_LearnSkill_Existed;

	// �����ҵ��ü��ܶ�Ӧ�ľ�̬���ԣ�Ĭ��һ����
	DWORD dwSkillTypeID = Skill::CreateTypeID(dwSkillID, nLevel);
	const tagSkillProto* pProto = g_attRes.GetSkillProto(dwSkillTypeID);
	if( !P_VALID(pProto) ) return E_LearnSkill_ProtoNotFound;

	pSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);
	pSkill->SetExtra();
	AddSkill(pSkill);

	const SkillScript * pSkillScript = pSkill->GetSkillScript();
	if( P_VALID(pSkillScript) )
	{
		pSkillScript->OnSkillChange(GetMap(),GetID(),pSkill->GetID(),0,nLevel);
	}

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;
}

//---------------------------------------------------------------------------------
// ���һ����ʱ���ܣ�����
//---------------------------------------------------------------------------------
INT	Role::RemoveExtraSkill(DWORD dwSkillID)
{
	Skill* pSkill = GetSkill(dwSkillID);
	if( !P_VALID(pSkill) || !pSkill->IsExtraSkill()) return E_LearnSkill_Existed;

	const SkillScript * pSkillScript = pSkill->GetSkillScript();
	if( P_VALID(pSkillScript) )
	{
		pSkillScript->OnSkillChange(GetMap(),GetID(),pSkill->GetID(),0,0);
	}

	RemoveSkill(dwSkillID);

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;
}

BOOL Role::StartCondense()
{
	if( GetAttValue(ERA_God_Faith) <= (CONDENSE_NUM-1) )
		return FALSE;

	if(IsDead())
		return FALSE;

	INT nMaxGodHead = 0;
	GetGodHeadLimit(GetLevel(),nMaxGodHead);
	if( GetAttValue(ERA_God_Godhead) >= nMaxGodHead )
	{
		const tagGodHeadProto * pHeadProto = g_attRes.GetOneGodHeadProto(nMaxGodHead+1);
		if(!P_VALID(pHeadProto))
			return FALSE;

		if(GetAttValue(ERA_God_Condenced) >= pHeadProto->nCondenseValue)
			return FALSE;
	}

	m_nCondenseCoolDown = 0;
	m_bCondensing = TRUE;
	return TRUE;
}

VOID Role::UpdateCondense()
{
	if( m_nCondenseCoolDown < 0 )
		return ;
	if( ++m_nCondenseCoolDown >= TICK_PER_SECOND * 5 )
	{
		m_nCondenseCoolDown = 0;
		DWORD dwRet = CondenseGodHead();
		if( E_Success != dwRet ) // �����Ƿ����
		{
			if(E_UseSkill_CannotCondenseFaithZero == dwRet)
			{//�����û�������ˣ�Ӧ�ͻ���Ҫ����Ҫ��E_Success���ܹرսڽ���
				StopCondense(2,E_Success,TRUE);
			}
			else
			{
				StopCondense(2,dwRet,TRUE);
			}

			if( IsHaveBuff (Buff::GetIDFromTypeID(GODCONDENSE_BUFFID)) )
			{
				RemoveBuff(Buff::GetIDFromTypeID(GODCONDENSE_BUFFID), TRUE);
			}

		}
	}
}

DWORD Role::CondenseGodHead()
{
	// ��������̫��������Ϣ��
	if( GetAttValue(ERA_God_Faith) <= (CONDENSE_NUM-1) )
		return E_UseSkill_CannotCondenseFaithZero;
	
	INT nGodHead = GetAttValue(ERA_God_Godhead);
	const tagGodHeadProto * pHeadProto = g_attRes.GetOneGodHeadProto(nGodHead+1);
	if(!P_VALID(pHeadProto))
		return E_SystemError;

	ModAttValue(ERA_God_Faith,-CONDENSE_NUM,FALSE);
	ModAttValue(ERA_God_Condenced,CONDENSE_NUM,FALSE);
	
	if( GetAttValue(ERA_God_Condenced) >= pHeadProto->nCondenseValue - CONDENSE_NUM ) // g_attRes��ȡ���۶���1
	{
		INT nMaxGodHead = 0;
		GetGodHeadLimit(GetLevel(),nMaxGodHead);
		if( nMaxGodHead == 0 )
		{
			tagNS_CondensedChange msg;
			msg.dwBelief		= GetAttValue(ERA_God_Faith);
			msg.dwCondensedVal	= GetAttValue(ERA_God_Condenced);
			SendMessage(&msg,msg.dwSize);

			DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (2 - 1);
			MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->nNum = 2;

			pSend->value[0].eType = ERA_God_Faith;
			pSend->value[0].nValue = msg.dwBelief;

			pSend->value[1].eType = ERA_God_Condenced;
			pSend->value[1].nValue = msg.dwCondensedVal;

			SendMessage(pSend, pSend->dwSize);

			MDEL_MSG(pSend);

			return E_UseSkill_CannotCondenseGodHeadLimitZero;
		}

		if( GetAttValue(ERA_God_Godhead) >= nMaxGodHead )
		{
			tagNS_CondensedChange msg;
			msg.dwBelief		= GetAttValue(ERA_God_Faith);
			msg.dwCondensedVal	= GetAttValue(ERA_God_Condenced);
			SendMessage(&msg,msg.dwSize);

			DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (2 - 1);
			MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->nNum = 2;

			pSend->value[0].eType = ERA_God_Faith;
			pSend->value[0].nValue = msg.dwBelief;

			pSend->value[1].eType = ERA_God_Condenced;
			pSend->value[1].nValue = msg.dwCondensedVal;

			SendMessage(pSend, pSend->dwSize);

			MDEL_MSG(pSend);

			return E_UseSkill_CannotCondenseGodHeadFull;
		}

		
	}

	if( GetAttValue(ERA_God_Condenced) >= pHeadProto->nCondenseValue ) // g_attRes��ȡ���۶�
	{
		ModAttValue(ERA_God_Godhead,1);
		SetAttValue(ERA_God_Condenced,0,FALSE);
		if( P_VALID(m_pScript) )
			m_pScript->OnRoleCondenseUpgrade(this);
	}

	tagNS_CondensedChange msg;
	msg.dwBelief		= GetAttValue(ERA_God_Faith);
	msg.dwCondensedVal	= GetAttValue(ERA_God_Condenced);
	SendMessage(&msg,msg.dwSize);

	DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (2 - 1);
	MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

	pSend->dwSize = dwSize;
	pSend->nNum = 2;

	pSend->value[0].eType = ERA_God_Faith;
	pSend->value[0].nValue = msg.dwBelief;

	pSend->value[1].eType = ERA_God_Condenced;
	pSend->value[1].nValue = msg.dwCondensedVal;

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);


	return E_Success;
}

VOID Role::StopCondense(INT nType, DWORD dwErr, BOOL bSendMsg)
{
	m_nCondenseCoolDown = GT_INVALID;
	if( bSendMsg )
	{
		tagNS_StopCondensed msg;
		msg.dwErrorCode = dwErr;
		msg.nType = nType;
		SendMessage(&msg,msg.dwSize);
	}
	m_bCondensing = FALSE;
}

//---------------------------------------------------------------------------------
//������ְ���Ǿ�ѡ��
DWORD Role::DirectPromotionClergy(DWORD dwClergyID)
{
	//�жϿɷ����
	DWORD dwRet = CanPromotionClergy(dwClergyID);
	if(E_Success != dwRet)
		return dwRet;

	//��ʼ��������������������
	return PromotionClergy(dwClergyID);
}
//---------------------------------------------------------------------------------
// �ж��Ƿ���Խ�����ְ
DWORD Role::CanPromotionClergy(DWORD dwClergyID)
{
	const tagClergyProto *pClergyProto = g_attRes.GetClergyProto(dwClergyID);
	if( !P_VALID(pClergyProto) )
	{
		return E_SystemError;
	}

	//��ְ���������ж�
	if(EPTT_Directly != pClergyProto->ePromotionType)
	{
		return E_Promotion_Clergy_Type;
	}

	//��ɫ��ǰ��ְ�ж�
	if(m_dwClergy == pClergyProto->dwID)
	{
		return E_Promotion_Clergy;
	}

	//���������ж�
	//���
	INT nGodHead = GetAttValue(ERA_God_Godhead);
	if(nGodHead < pClergyProto->nGodheadLimit)
	{
		return E_Promotion_Clergy;
	}

	//�ȼ�
	INT nLevel = GetLevel();
	if(nLevel < pClergyProto->nLevelLimit)
	{
		return E_Promotion_Clergy;
	}

	//�Ա�
	BYTE bySex = GetSex();
	if(pClergyProto->eSexLimit != ES_Null && bySex != (BYTE)pClergyProto->eSexLimit )
	{
		return E_Promotion_Clergy;
	}

	//ְҵ
	if( E_Success != VocationCheck(pClergyProto->dwClassLimit) )
	{
		return E_Promotion_Clergy;
	}

	//����
	if(ERA_Null != pClergyProto->eAttType)
	{
		INT nValue = GetAttValue(pClergyProto->eAttType);
		if( nValue < pClergyProto->dwAttValue )
		{
			return E_Promotion_Clergy;
		}
	}

	if(!IsEquipTimeEquipment())
	{
		return E_TimeEquipLimit;
	}
	

	return E_Success;
}
//---------------------------------------------------------------------------------
//������ְ����������ֱ�ӽ������Ǿ�ѡ����������������ӿ�
DWORD Role::PromotionClergy(DWORD dwClergyID, bool bNeedSavePreClergy)
{
	//��õ�ǰ��ְ
	const tagClergyProto *pClergyProtoCur = g_attRes.GetClergyProto(m_dwClergy);
	if( !P_VALID(pClergyProtoCur) && m_dwClergy > 0)
	{
		return E_SystemError;
	}

	//�����ְ������
	m_GodAreaMgr.ClearClergyArea();

// 	if( m_dwClergy > 0 )
// 	{
// 		for( INT8 i = 0; i < MAX_FieldNum; ++i )
// 		{
// 			if( pClergyProtoCur->dwFieldID[i] > 0 )
// 				m_GodAreaMgr.ChangeArea(pClergyProtoCur->dwFieldID[i],FALSE);
// 		}
// 	}
	
	const tagClergyProto *pClergyProto = g_attRes.GetClergyProto(dwClergyID);
	if( !P_VALID(pClergyProto))
	{
		return E_SystemError;
	}

	//���ӽ�������ְ�������ܵȽ���
	for( INT8 i = 0; i < MAX_FieldNum; ++i )
	{
		if( pClergyProto->dwFieldID[i] > 0 )
		{
			m_GodAreaMgr.ChangeArea(pClergyProto->dwFieldID[i],TRUE,NULL,TRUE,FALSE);

			g_dbSession.SendCreateArea2DB(GetID(),pClergyProto->dwFieldID[i],TRUE);
		}
	}

	//������һ�ε���ְ
	if(bNeedSavePreClergy)
	{
		if( P_VALID(pClergyProtoCur) && 
			pClergyProtoCur->eType != EGT_WarBoss && pClergyProtoCur->eType != EGT_SunBoss &&
			pClergyProtoCur->eType != EGT_ManaBoss && pClergyProtoCur->eType != EGT_PeaceBoss && 
			pClergyProtoCur->eType != EGT_War && pClergyProtoCur->eType != EGT_Sun &&
			pClergyProtoCur->eType != EGT_Mana && pClergyProtoCur->eType != EGT_Peace)
			m_dwPrevClergy = m_dwClergy;
	}

	//������ְ
	m_dwClergy = dwClergyID;

	//���ýű�
	if( P_VALID(m_pScript) )
		m_pScript->OnRoleGetClergy(this);

	return E_Success;
}

//---------------------------------------------------------------------------------
VOID Role::InterruptCondense()
{
	if( m_nCondenseCoolDown > GT_INVALID )
		StopCondense(2,E_UseSkill_BeInterrupted,TRUE);
	else
		StopCondense(2,E_Success,TRUE);

	if( IsHaveBuff (Buff::GetIDFromTypeID(GODCONDENSE_BUFFID)) )
	{
		RemoveBuff(Buff::GetIDFromTypeID(GODCONDENSE_BUFFID), TRUE);
	}
}


//---------------------------------------------------------------------------------
//
VOID Role::LoginClergyVoteResultCheck()
{
	ECampaignGodType eType1 = ECGT_Null;
	EClergyType eType2 = ECT_Null;

	if (g_ClergyRankingMgr.IsRoleWinClergy(this->GetID(), eType1, eType2))
	{
		DWORD dwClergyID = GetClergyID(eType1, eType2);
		if(dwClergyID != NULL) PromotionClergy(dwClergyID);

		tagNDBC_CandidateRecvClergy send;
		send.eType = eType1;
		send.nNum = 1;
		send.dwRoleID[0] = this->GetID();
		g_dbSession.Send(&send, send.dwSize);
	}
	else
	{
		PromotionClergy(m_dwClergy);
	}

	// �����ְ��ص�buff�������Ƿ��ȫ
 	const tagClergyProto* pProto = g_attRes.GetClergyProto(m_dwClergy);
 	if(P_VALID(pProto))
 	{
 		for( INT8 i = 0; i < MAX_FieldNum; ++i )
 		{
 			if( pProto->dwFieldID[i] > 0 && !GetGodAreaMgr().IsHaveGodArea(pProto->dwFieldID[i]))
 				m_GodAreaMgr.ChangeArea(pProto->dwFieldID[i],TRUE,NULL,TRUE,FALSE);
 		}
 	}
}
//---------------------------------------------------------------------------------
//������ְ��������
DWORD Role::ChangeAreaAbility( DWORD dwAreaID, BOOL bActive, BOOL bByCD /*=TRUE*/)
{
	return GetGodAreaMgr().ChangeAreaAbility(dwAreaID,bActive,bByCD);
}

//---------------------------------------------------------------------------------
//������ְ����
DWORD Role::ChangeArea( DWORD dwAreaID, BOOL bAdd, tagGlobalLimitedItem *pGlobalLimitedItem/*=NULL*/,BOOL bDelGodFaith/*=TRUE*/ )
{
	return GetGodAreaMgr().ChangeArea(dwAreaID,bAdd, pGlobalLimitedItem,bDelGodFaith);
}
//---------------------------------------------------------------------------------
// ���������ʼ������ͻ���
VOID Role::SendInitStateArea()
{
	GetGodAreaMgr().SendInitStateArea();
}
//---------------------------------------------------------------------------------

static BOOL SendTransSkinMsgCon(Unit * pSelf ,Unit *  pVisTileUnit)
{
	if( P_VALID(pVisTileUnit) && P_VALID(pSelf) && pSelf->GetID() != pVisTileUnit->GetID() )
		return TRUE;
	return FALSE;
}
VOID Role::SetTransSkinNdx(INT ndx,BOOL bSendBigVisMsg, BOOL bSendAvatarEquip)
{
	INT nOld = m_nTransformSkinNdx;
	m_nTransformSkinNdx = ndx; 
	if( m_nTransformSkinNdx != nOld )
	{
		if (bSendAvatarEquip)
		{
			SendAvatarEquip2Client();
		}
		if( bSendBigVisMsg )
		{
			SyncRoleSkin2RemoteRole();
		}
	}
}
//---------------------------------------------------------------------------------
//�������һ��ϡ����������
DWORD Role::RandCancleAreaAbility()
{
	return m_GodAreaMgr.RandCancleAreaAbility();
}
//---------------------------------------------------------------------------------
//����Ѽ����ĳ����������
INT	Role::GetAbilityAreaNum(EGodAreaType eType, BOOL bAll/* = TRUE*/)
{
	return m_GodAreaMgr.GetAbilityAreaNum(eType,bAll);
}
//---------------------------------------------------------------------------------
//���ɾ��һ��ϡ������
DWORD Role::RandDelArea()
{
	return m_GodAreaMgr.RandDelArea();
}
//-----------------------------------------------------------------
// �ܷ������ְ��ѡ����
DWORD Role::CanGetClergy(ECampaignGodType eType, bool bSeniorOrJunior/* true ���� false С��*/)
{
	const tagClergyProto* pProto = NULL;
	if (bSeniorOrJunior)
	{
		pProto = g_attRes.GetClergyBoss(eType,(ESex)GetSex());
	}
	else
	{
		pProto = g_attRes.GetClergyBase(eType);
	}
	
	if (!P_VALID(pProto))
	{
		return E_SystemError;
	}

	if(E_Success != VocationCheck(pProto->dwClassLimit))
	{
		return E_Enroll_Clergy_ClassLimited;
	}

	if (GetLevel() < pProto->nLevelLimit)
	{
		return E_Enroll_Clergy_LevelLimited;
	}

	if (pProto->eSexLimit != ES_Null && GetSex() != pProto->eSexLimit)
	{
		return E_Enroll_Clergy_SexLimited;
	}

	if (GetAttValue((INT)pProto->eAttType) < pProto->dwAttValue)
	{
		return E_Enroll_Clergy_AttValueLimited;
	}

// 	INT nGodHead = 0;
// 	GetGodHeadLimit(GetLevel(), nGodHead);
	if (m_nAtt[ERA_God_Godhead] < pProto->nGodheadLimit)
	{
		return E_Enroll_Clergy_GodheadLimited;
	}

	return E_Success;
}
void Role::SendAvatarEquip2Client()
{
	tagNS_AvatarEquip msgSelf;
	msgSelf.dwRoleID = GetID();
	msgSelf.bFashion = m_DisplaySet.bFashionDisplay;
	//if( m_nTransformSkinNdx <= 0 )	// �ָ�����
	memcpy(&msgSelf.sAvatarEquip,&GetAvatarEquip(),sizeof(tagAvatarEquip));
	//else
	//	memset(&msgSelf.sAvatarEquip,GT_INVALID,sizeof(tagAvatarEquip));
	msgSelf.byTransSkinIndex = m_nTransformSkinNdx;
	SendMessage(&msgSelf,msgSelf.dwSize);
}





void Role::SyncRoleSkin2RemoteRole()
{
	tagNS_RemoteRoleSkin msgVistile;
	msgVistile.dwRoleID = GetID();
	msgVistile.sDisplaySet				= GetDisplaySet();
	IFASTCODE->MemCpy(&msgVistile.Avatar, GetAvatar(), sizeof(tagAvatarAtt));
	msgVistile.Avatar.byTransSkinIndex = m_nTransformSkinNdx;
	IFASTCODE->MemCpy(&msgVistile.AvatarEquip, &GetAvatarEquip(), sizeof(tagAvatarEquip));
	Map * pMap = GetMap();
	if( P_VALID(pMap) )
		pMap->SynVisTileMsgWithCon(this,&msgVistile,msgVistile.dwSize,SendTransSkinMsgCon);
}
//---------------------------------------------------------------------------------
//���ݵ�ͼ���ͺ�ID���½�ɫ�ļ���
VOID Role::UpdateSkillByMap( EMapType eType, DWORD dwMapID )
{
	TMap<DWORD, Skill*>::TMapIterator it = m_mapSkill.Begin();
	Skill* pSkill = NULL;
	BOOL bCanUse = FALSE;
	while( m_mapSkill.PeekNext(it, pSkill) )
	{
		if( !P_VALID(pSkill) )
			continue;

		if( ESSF_SkillMap == pSkill->GetSpecFunc() )
		{
			BOOL bCanUse = CanUseSkillByMap(pSkill->GetID(),eType,dwMapID);

			if(bCanUse)
			{
				ResumeUseSkillByMap(pSkill);
			}
			else
			{
				ForbidUseSkillByMap(pSkill);
			}
		}

		//if( ESSF_SkillMap == pSkill->GetSpecFunc() )
		//{
		//	INT nVal1 = pSkill->GetSpecFuncVal1();
		//	switch(nVal1)
		//	{
		//	case 1:
		//		{//��ͨ��ͼ
		//			if(EMT_Normal == eType)
		//			{							
		//				bCanUse = TRUE;
		//			}
		//			else
		//			{
		//				bCanUse = FALSE;
		//			}
		//		}
		//		break;
		//	case 2:
		//		{//������ͼ
		//			if(	EMT_Instance == eType ||
		//				EMT_System == eType ||
		//				EMT_PVP == eType ||
		//				EMT_Build == eType ||
		//				EMT_ScriptCreate == eType )
		//			{
		//				bCanUse = TRUE;
		//			}
		//			else
		//			{
		//				bCanUse = FALSE;
		//			}
		//		}
		//		break;
		//	case 3:
		//		{//��ͼ����
		//			INT nVal2 = pSkill->GetSpecFuncVal2();
		//			const tagMapBoxProto *pMapBoxProto = g_attRes.GetMapBoxProto(nVal2);
		//			if( !P_VALID(pMapBoxProto) )
		//			{
		//				continue;
		//			}

		//			bCanUse = FALSE;
		//			for( int i = 0; i < 12; ++i )
		//			{
		//				if( pMapBoxProto->dwMapID[i] <= 0 )
		//					break;

		//				if( dwMapID == pMapBoxProto->dwMapID[i] )
		//				{
		//					bCanUse = TRUE;
		//					break;
		//				}
		//			}
		//		}
		//		break;
		//	default:
		//		continue;
		//	}

		//	if(bCanUse)
		//	{
		//		ResumeUseSkillByMap(pSkill);
		//	}
		//	else
		//	{
		//		ForbidUseSkillByMap(pSkill);
		//	}
		//}
	}
}
//---------------------------------------------------------------------------------
//���¼����
VOID Role::ResumeUseSkillByMap( Skill *pSkill )
{
	if( !P_VALID(pSkill) )
		return;

	if(!pSkill->GetCanUseByMap())
	{
		// ���¼�������Ӱ��
		AddSkillMod(pSkill);
		pSkill->SetOwnerMod();
		pSkill->SetCanUseByMap(TRUE);
	}
}

//---------------------------------------------------------------------------------
//���ܽ�ֹʹ��
VOID Role::ForbidUseSkillByMap( Skill *pSkill )
{
	if( !P_VALID(pSkill) )
		return;

	if(pSkill->GetCanUseByMap())
	{
		// ��������ɸü��ܲ���������buff
		RemoveAllBuffBelongSkill(pSkill->GetID(), FALSE);

		// ȥ����������Ӱ�죬��ȥ������������Ӱ��
		pSkill->UnsetOwnerMod();
		RemoveSkillMod(pSkill);

		pSkill->SetCanUseByMap(FALSE);
	}

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();
}
//---------------------------------------------------------------------------------
//ͨ������ֱ�ӼӼ���
VOID Role::AddSkillByArea(DWORD dwSkillID)
{
	Skill* pSkill = m_mapSkill.Peek(Skill::GetIDFromTypeID(dwSkillID));
	if(P_VALID(pSkill))
	{
		RemoveSkill(Skill::GetIDFromTypeID(dwSkillID));
	}
	// ��Ӽ���
	pSkill = new Skill(Skill::GetIDFromTypeID(dwSkillID), 0, Skill::GetLevelFromTypeID(dwSkillID), 0, 0);
	AddSkill(pSkill);

	const SkillScript *pSkillScript = pSkill->GetSkillScript();
	if( P_VALID(pSkillScript) )
	{
		pSkillScript->OnSkillChange(GetMap(),GetID(),pSkill->GetID(),0,1);
	}

	const RoleScript *pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript) && P_VALID(pSkill))
	{
		DWORD dwSkillIDWithLevel = pSkill->GetID()*100+pSkill->GetLevel();
		pRoleScript->OnLearnSkill(this, dwSkillIDWithLevel);
	}

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();
}
//---------------------------------------------------------------------------------
//�ڸõ�ͼ�����Ƿ����
BOOL Role::CanUseSkillByMap(DWORD dwSkillID, EMapType eType, DWORD dwMapID)
{
	Skill* pSkill = m_mapSkill.Peek(dwSkillID);

	if( !P_VALID(pSkill) )
		return TRUE;

	BOOL bCanUse = FALSE;
	if( ESSF_SkillMap == pSkill->GetSpecFunc() )
	{
		INT nVal1 = pSkill->GetSpecFuncVal1();
		switch(nVal1)
		{
		case 1:
			{//��ͨ��ͼ
				if(EMT_Normal == eType)
				{							
					bCanUse = TRUE;
				}
				else
				{
					bCanUse = FALSE;
				}
			}
			break;
		case 2:
			{//������ͼ
				if(	EMT_Instance == eType ||
					EMT_System == eType ||
					EMT_PVP == eType ||
					EMT_Build == eType ||
					EMT_ScriptCreate == eType )
				{
					bCanUse = TRUE;
				}
				else
				{
					bCanUse = FALSE;
				}
			}
			break;
		case 3:
			{//��ͼ����
				INT nVal2 = pSkill->GetSpecFuncVal2();
				const tagMapBoxProto *pMapBoxProto = g_attRes.GetMapBoxProto(nVal2);
				if( !P_VALID(pMapBoxProto) )
				{
					return TRUE;
				}

				bCanUse = FALSE;
				for( int i = 0; i < 12; ++i )
				{
					if( pMapBoxProto->dwMapID[i] <= 0 )
						break;

					if( dwMapID == pMapBoxProto->dwMapID[i] )
					{
						bCanUse = TRUE;
						break;
					}
				}
			}
			break;
		default:
			return TRUE;
		}
	}
	else
	{
		bCanUse = TRUE;
	}

	return bCanUse;
}
//---------------------------------------------------------------------------------
//���ݵ�ͼ���ͺ�ID���½�ɫ�ļ���
VOID Role::UpdateSkillByMap( DWORD dwSkillID, EMapType eType, DWORD dwMapID )
{
	Skill* pSkill = m_mapSkill.Peek(dwSkillID);

	if( !P_VALID(pSkill) )
		return;

	if( ESSF_SkillMap == pSkill->GetSpecFunc() )
	{
		BOOL bCanUse = CanUseSkillByMap(dwSkillID,eType,dwMapID);

		if(bCanUse)
		{
			ResumeUseSkillByMap(pSkill);
		}
		else
		{
			ForbidUseSkillByMap(pSkill);
		}
	}

	//BOOL bCanUse = FALSE;
	//if( ESSF_SkillMap == pSkill->GetSpecFunc() )
	//{
	//	INT nVal1 = pSkill->GetSpecFuncVal1();
	//	switch(nVal1)
	//	{
	//	case 1:
	//		{//��ͨ��ͼ
	//			if(EMT_Normal == eType)
	//			{							
	//				bCanUse = TRUE;
	//			}
	//			else
	//			{
	//				bCanUse = FALSE;
	//			}
	//		}
	//		break;
	//	case 2:
	//		{//������ͼ
	//			if(	EMT_Instance == eType ||
	//				EMT_System == eType ||
	//				EMT_PVP == eType ||
	//				EMT_Build == eType ||
	//				EMT_ScriptCreate == eType )
	//			{
	//				bCanUse = TRUE;
	//			}
	//			else
	//			{
	//				bCanUse = FALSE;
	//			}
	//		}
	//		break;
	//	case 3:
	//		{//��ͼ����
	//			INT nVal2 = pSkill->GetSpecFuncVal2();
	//			const tagMapBoxProto *pMapBoxProto = g_attRes.GetMapBoxProto(nVal2);
	//			if( !P_VALID(pMapBoxProto) )
	//			{
	//				return;
	//			}

	//			bCanUse = FALSE;
	//			for( int i = 0; i < 12; ++i )
	//			{
	//				if( pMapBoxProto->dwMapID[i] <= 0 )
	//					break;

	//				if( dwMapID == pMapBoxProto->dwMapID[i] )
	//				{
	//					bCanUse = TRUE;
	//					break;
	//				}
	//			}
	//		}
	//		break;
	//	default:
	//		return;
	//	}

		//if(bCanUse)
		//{
		//	ResumeUseSkillByMap(pSkill);
		//}
		//else
		//{
		//	ForbidUseSkillByMap(pSkill);
		//}
	//}
}
//---------------------------------------------------------------------------------
VOID Role::ReCalRoleAtt()
{
// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();
}
//---------------------------------------------------------------------------------
VOID Role::UpdateArea()
{
	m_GodAreaMgr.Update();
}
//---------------------------------------------------------------------------------
//�������CDʱ��
DWORD Role::GetAreaCD(DWORD dwAreaID)
{
	return m_GodAreaMgr.GetAreaCD(dwAreaID);
}
//---------------------------------------------------------------------------------
//ȥ��ϡ��������Чbuff
VOID Role::RemoveAreaBuff()
{
	m_GodAreaMgr.RemoveAreaBuff();
}
//---------------------------------------------------------------------------------
DWORD Role::GetRandRareArea()
{
	return m_GodAreaMgr.GetRandRareArea();
}
//---------------------------------------------------------------------------------
BOOL Role::IsHaveArea(DWORD dwAreaID)
{
	return m_GodAreaMgr.IsHaveArea(dwAreaID);
}
//---------------------------------------------------------------------------------
BOOL Role::DirectDelOneRareArea(DWORD dwAreaID)
{
	return m_GodAreaMgr.DirectDelOneRareArea(dwAreaID);
}
//---------------------------------------------------------------------------------
DWORD Role::TrainFamilySprite( DWORD dwTrainID, INT &nTrainRet, INT64 n8ItemID, BOOL bAgain/*=FALSE*/ )
{
//ϵͳ�߽���������ɺ���������¼�����
//�۳���ɫ���϶�Ӧ��������Ʒ
//���ػ���������δ�ﵽ����
//�������������������������ȡ������ֵ�������ػ���������ֵ
//����ȡ�õ�ֵ+ԭ��ֵ���������ޣ������޼���
//���ػ����������Ѵﵽ���ޣ����Ӧ����ֵ�������κα仯
//�ڽ�������ʾ�����������Ľ��
//�ػ������ɳ��Ȼ�����ӣ���ɫ��ö�Ӧ�ľ������Ʒ����
//�������������״̬��Ϊ������ɡ����ڽ�������ʾ��������ֵ����ɽ�ɫ����
//����Ƶ���ڹ��棺��###����ػ��������������ػ�������###���������ͣ�����###����������
	DWORD dwRet = CanTrainFamilySprite(dwTrainID,n8ItemID,bAgain);
	if( E_Success != dwRet )
		return dwRet;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return GT_INVALID;

	const tagSpriteTrainProto *pSpriteTrainProto = g_attRes.GetSpriteTrainProto(dwTrainID);
	if( !P_VALID(pSpriteTrainProto) ) 
		return GT_INVALID;

	const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFamilySprite->GetLevel());
	if( !P_VALID(pFamilySpriteProto) ) 
		return GT_INVALID;


	// �۳���Ʒ
	tagItem *pItem = GetItemMgr().GetBag().GetItem(n8ItemID);
	if(P_VALID(pItem))
	{
		if(P_VALID(pItem->pProtoType) && EISF_FamilySpriteItem != pItem->pProtoType->eSpecFunc)
		{
			if(pItem->dwTypeID == pSpriteTrainProto->dwItemID)
			{
				if( E_Success != GetItemMgr().RemoveFromRole(pSpriteTrainProto->dwItemID, pSpriteTrainProto->dwItemNum, ELCLD_Family_Sprite_Train) )
				{
					return GT_INVALID;
				}
			}
			else
			{
				return GT_INVALID; 
			}
		}
		else
		{
			if( E_Success != GetItemMgr().DelFromBag(n8ItemID,ELCLD_Family_Sprite_Train,1) )
			{
				return GT_INVALID;
			}
			else
			{
				m_nTrainNum++;
			}
		}
	}
	else
	{
		return GT_INVALID; 
	}


	// �۳���Ʒ
	//GetItemMgr().RemoveFromRole(pSpriteTrainProto->dwItemID, pSpriteTrainProto->dwItemNum, ELCLD_Family_Sprite_Train);

	//���ȡ��Ҫ���ӵ�����ֵ
	INT nTrain = 0;
	INT nTrainCur = 0;

	if( -1 != pFamilySpriteProto->dwAbilityUpperLimit[pSpriteTrainProto->dwAbilityType] && pFamilySprite->GetAbility((EFamilySpriteAtt)pSpriteTrainProto->dwAbilityType) < pFamilySpriteProto->dwAbilityUpperLimit[pSpriteTrainProto->dwAbilityType])
	{
		nTrain = IUTIL->RandomInRange(pSpriteTrainProto->dwPowerLowLimit,pSpriteTrainProto->dwPowerUpLimit);
		nTrainCur = pFamilySprite->GetAbility((EFamilySpriteAtt)pSpriteTrainProto->dwAbilityType);

		//������������ֵ
		if( nTrain + nTrainCur > pFamilySpriteProto->dwAbilityUpperLimit[pSpriteTrainProto->dwAbilityType])
			pFamilySprite->SetAbility( (EFamilySpriteAtt)pSpriteTrainProto->dwAbilityType, pFamilySpriteProto->dwAbilityUpperLimit[pSpriteTrainProto->dwAbilityType] );
		else
			pFamilySprite->SetAbility( (EFamilySpriteAtt)pSpriteTrainProto->dwAbilityType, nTrain + nTrainCur );
	}

	//������������
	pFamilySprite->AddExp(pSpriteTrainProto->dwSpriteExpReword);
	//INT nExp = pFamilySprite->GetExp();
	//INT nRet = nExp + pSpriteTrainProto->dwSpriteExpReword - pFamilySpriteProto->dwExp;
	//if(nRet >= 0)
	//{
	//	const tagFamilySpriteProto *pFamilySpriteProtoTemp = g_attRes.GetFamilySpriteProto(pFamilySprite->GetLevel()+1);
	//	if( P_VALID(pFamilySpriteProtoTemp) ) 
	//	{
	//		pFamilySprite->SetLevel( pFamilySprite->GetLevel() + 1 );
	//		pFamilySprite->SetExp(nRet);

	//		//������ʱ����Ҫ���һ����û�б����������
	//		pFamilySprite->ActivationFamilySpriteAbility();
	//	}
	//	else
	//	{
	//		pFamilySprite->SetExp(nExp + pSpriteTrainProto->dwSpriteExpReword);
	//	}
	//}
	//else
	//{
	//	pFamilySprite->SetExp( nExp + pSpriteTrainProto->dwSpriteExpReword );
	//}

	//���ý�ɫ����
	ExpChange(INT64(pSpriteTrainProto->dwExpReword));

	//���ӽ�����Ʒ
	tagItem *pItemNew = ItemCreator::CreateEx( EICM_Mall, GT_INVALID, pSpriteTrainProto->dwItemReword, pSpriteTrainProto->dwItemNum );
	if(P_VALID(pItemNew))
	{
		GetItemMgr().Add2Bag(pItemNew, ELCID_Family_Sprite_Train_Item, TRUE);
	}

	//������������״̬
	pFamilySprite->UpdateFamilySpriteRequirementData( dwTrainID, TRUE, nTrain, GetID() );

	//���¼����������ݵ�DB
	pFamilySprite->UpdateFamilySpriteData2DB();

	nTrainRet = nTrain;

	pFamilySprite->UpdateAllRoleAtt();

	Family *pFamily = g_FamilyMgr.GetFamily(GetFamilyID());
	if( P_VALID(pFamily) )
	{
		tagNS_FamilySpritTrainBroad send;
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(GetID());
		if (P_VALID(pRoleInfo))
			_tcscpy_s( send.tszRoleName, X_SHORT_NAME, pRoleInfo->szRoleName);
		send.dwType = pSpriteTrainProto->dwAbilityType;
		send.nRet = nTrain;
		pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member((tagNetCmd*)&send));
	}

	return E_Success;
}
//---------------------------------------------------------------------------------
DWORD Role::CanTrainFamilySprite(DWORD dwTrainID, INT64 n8ItemID, BOOL bAgain/*=FALSE*/)
{
//�����������������ť��ϵͳ���������жϣ�
//�жϽ�ɫ�����Ƿ�ӵ���㹻�����Ķ�Ӧ��Ʒ����δӵ�У�����ʾ��������ӵ�е�������Ʒ�������㣬�޷��ύ��������ӵ�У��������һ��
//�жϽ�ɫ�Ƿ�����������壬�������ڣ�����ʾ�������Ѳ����ڸü��壬�޷��ύ���������ڣ��������һ��
//�жϸ������Ƿ��ѱ���ɣ�������ɣ�����ʾ���������������ѱ������Ա��ɡ�����δ��ɣ��������һ��
//�жϽ�ɫ�����Ƿ�������������������ʾ�������ı������������޷������Ʒ�������������������ύ��
//ϵͳ�ж϶�Ӧ����������ػ����������Ƿ��Ѿ��ﵽ���ޣ����Ѵﵽ���ޣ��򵯳�ȷ����ʾ�򣺡��ػ������ĸ������Ѿ��ﵽ��ǰ���ޣ����Կɽ�����������ֻ���þ������Ʒ���������Ƿ�ȷ�ϣ�����ȷ��/ȡ���������ȷ�ϣ�������δ�ﵽ���ޣ��������һ��
	const tagSpriteTrainProto *pSpriteTrainProto = g_attRes.GetSpriteTrainProto(dwTrainID);
	if( !P_VALID(pSpriteTrainProto) ) 
		return GT_INVALID;

	if( GetLevel() < pSpriteTrainProto->nLevelLimit )
		return EFSE_FABAO_LEVEL_DOWN;

	tagItem *pItem = GetItemMgr().GetBag().GetItem(n8ItemID);
	if( P_VALID(pItem) )
	{
		if( P_VALID(pItem->pProtoType) && EISF_FamilySpriteItem != pItem->pProtoType->eSpecFunc)
		{
			if(pItem->dwTypeID == pSpriteTrainProto->dwItemID)
			{
				// �����Ʒ
				INT nTotal = GetItemMgr().GetBagSameItemCount(pSpriteTrainProto->dwItemID);
				if( nTotal < pSpriteTrainProto->dwItemNum )
					return EFSE_LACK_ITEM;
			}
			else
			{
				return EFSE_LACK_ITEM;
			}
		}
		else
		{
			if( m_nTrainNum >= USE_FAMILY_SPEC_ITEM_MAX )
			{
				return EFSE_USE_ITEM_MAX; 
			}
		}
	}
	else
	{
		return EFSE_LACK_ITEM;
	}

	//// ������
	//Family *pFamily = g_FamilyMgr.GetFamily(GetFamilyID());
	//if( !P_VALID(pFamily) )
	//	return EFSE_NOT_FAMILY_MEMBER;

	//// �����������
	//FamilySprite* pFamilySprite = pFamily->GetFamilySprite();
	//if( !P_VALID(pFamilySprite) )
	//	return GT_INVALID;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return GT_INVALID;

	//�����������ֵ
	const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFamilySprite->GetLevel());
	if( !P_VALID(pFamilySpriteProto) ) 
		return GT_INVALID;

	if(pFamilySprite->IsFamilySpriteTrainComplete(dwTrainID))
		return EFSE_HAS_COMPLETE;

	// ��鱳��
	if( GetItemMgr().GetBagFreeSize() < pSpriteTrainProto->dwItemRewordNum ) 
		return EFSE_BAG_FULL;

	//�����������ֵ
	if(!bAgain)
	{
		const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFamilySprite->GetLevel());
		if( !P_VALID(pFamilySpriteProto) ) 
			return GT_INVALID;

		//if( -1 != pFamilySpriteProto->dwAbilityUpperLimit[pSpriteTrainProto->dwAbilityType])
		//{
		//	if( pFamilySprite->GetAbility((EFamilySpriteAtt)pSpriteTrainProto->dwAbilityType) >=  pFamilySpriteProto->dwAbilityUpperLimit[pSpriteTrainProto->dwAbilityType])
		//		return EFSE_SPRITE_LEVEL_FULL;
		//}
	}

	return E_Success;
}
//---------------------------------------------------------------------------------
DWORD Role::FamilySpriteRegister( INT64 n8ID )
{
	DWORD dwRet = CanFamilySpriteRegister( n8ID );

	if( E_Success != dwRet )
		return dwRet;

	//Ŀǰ��û�еǼǵ�����
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite( GetFamilyID() );
	if( !P_VALID(pFamilySprite) )
		return GT_INVALID;

	//if( pFamilySprite->HaveRoleSprite( GetID() ) )
	//	return GT_INVALID;

	tagItem *pItem = NULL;
	pItem = GetItemMgr().GetItemFromAllContainer( n8ID );
	if( P_VALID(pItem) ) 
	{
		if( !MIsFaBao(pItem) )
			return GT_INVALID;

		if(EICT_Bag != pItem->eConType && EICT_Equip != pItem->eConType )
			return GT_INVALID; 
	
		tagFabao *pFabao = (tagFabao*)pItem;
		if( EICT_Equip == pItem->eConType )
		{
			DWORD dwErrorCode = GetItemMgr().UnequipOnly(n8ID);
			if(dwErrorCode != E_Success)
			{
				return GT_INVALID;
			}

			ProcEquipEffect(NULL, pFabao, pFabao->n16Index, FALSE, TRUE, TRUE);

			// ���񴥷�
			OnQuestEvent(EQE_Item, pFabao->dwTypeID, 1, TRUE);
			
			GetItemMgr().SendFabaoSpec2DB(*pFabao);
			Map *pMap = GetMap();
			if( P_VALID(pMap) )
			{
				tagNS_RemoteRoleChangeFabao send;
				tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pEquipProto;
				tagRemoteRoleFabaoInfo & info = send.tagFaBaoIno;
				send.dwRoleID = GetID();
				info.byEquipFabaoFlag = 0;
				info.byStage		= pFabao->n16Stage;
				info.dwTypeID		= pFabao->pEquipProto->dwTypeID;
				pMap->SendBigVisTileMsg(this,&send,send.dwSize);
			}

			//�Ǽ�����
			tagFabao *pFabaoOld = pFamilySprite->RegisterRoleSprite( GetID(), pFabao );

			pFabao->SetPos(EICT_Family,0);
			pFabao->SetUpdate(EUDBS_Update);

			if( P_VALID(pFabaoOld) )
			{//װ��������װ����

				// ����Ƿ����װ������
				DWORD dwErrorCode = CanEquip(pFabaoOld, EEP_Face);
				if(dwErrorCode != E_Success)
				{
					return GT_INVALID;
				}

				// ��װ
				dwErrorCode = GetItemMgr().Equip( n8ID, EEP_Face );
				if(E_Success != dwErrorCode)
					return GT_INVALID;

				ProcEquipEffect( pFabaoOld, NULL, (INT16)EEP_Face, FALSE, TRUE, TRUE );

				const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
				if (P_VALID(pRoleScript))
				{
					const INT32 nMask	= 0xFFFFFFFF;
					INT32 n32High	= (INT32)((pFabaoOld->n64Serial >> 32) & nMask);
					INT32 n32Low	= (INT32)(pFabaoOld->n64Serial & nMask);
					pRoleScript->OnDressEquip(this, true, pFabaoOld->dwTypeID, static_cast<DWORD>(EEP_Face), n32High, n32Low);
				}
				// ���񴥷�
				OnQuestEvent(EQE_Item, pFabaoOld->dwTypeID, 1, FALSE);

				if( g_world.IsFabaoOpen() )
				{
					Map * pMap = GetMap();
					if( P_VALID(pMap) )
					{
						tagNS_RemoteRoleChangeFabao send;
						tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabaoOld->pEquipProto;
						tagRemoteRoleFabaoInfo & info = send.tagFaBaoIno;
						send.dwRoleID = GetID();
						info.byEquipFabaoFlag = 1;
						info.byStage		= pFabaoOld->n16Stage;
						info.dwTypeID		= pFabaoOld->pEquipProto->dwTypeID;
						pMap->SendBigVisTileMsg(this,&send,send.dwSize);
					}
				}

				pFabaoOld->BindAfterUse();
			}
		}
		else
		{
			INT16 n16IndexOld = pFabao->n16Index;
			// �ӱ�����ȡ��װ��
			GetItemMgr().GetBag().Remove(n8ID);
			// ��ͻ��˷���Ϣ
			//tagNS_ItemPosChangeEx sendPos;
			//sendPos.eConTypeSrc1 = EICT_Bag;
			//sendPos.eConTypeSrc2 = EICT_Family;
			//sendPos.eConTypeDst1 = EICT_Family;
			//sendPos.eConTypeDst2 = EICT_Bag;
			//sendPos.n64Serial1 = n8ID;
			//sendPos.n64Serial2 = GT_INVALID;
			//sendPos.n16Num1 = 1;
			//sendPos.n16Num2 = GT_INVALID;
			//sendPos.n16PosDst1 = GT_INVALID;
			//sendPos.n16PosDst2 = GT_INVALID;

			//SendMessage(&sendPos, sendPos.dwSize);
			tagNS_ItemRemove send;
			send.n64Serial	= n8ID;
			send.n16Index	= n16IndexOld;
			send.n16Num		= 0;
			send.eConType	= EICT_Bag;

			SendMessage(&send, send.dwSize);

			//�Ǽ�����
			tagFabao *pFabaoOld = pFamilySprite->RegisterRoleSprite( GetID(), pFabao );

			pFabao->SetPos(EICT_Family,0);
			pFabao->SetUpdate(EUDBS_Update);

			if( P_VALID(pFabaoOld) )
			{
				tagItem *pItem = pFabaoOld;
				DWORD dwRet = GetItemMgr().Add2Bag( pItem, ELCLD_Family_Role_Sprite, FALSE, TRUE, TRUE );
				if(dwRet != 0)
					ILOG->Write(_T("RoleID is : %d, ELCLD_Family_Role_Sprite1 Result:%d!\r\n"),GetID(),dwRet);
			}
		}
	}

	return E_Success;
}
//---------------------------------------------------------------------------------
DWORD Role::CanFamilySpriteRegister( INT64 n8ID )
{
//�жϸ�������Ʒ���Ƿ�Ϊ��ɫ����Ϊ��ɫ������ʾ�������Ǽǵ�����Ʒ�ʱ�����ڰ�ɫƷ�ʡ�������Ϊ��ɫ���������һ��
//�жϸ������ĵȼ��Ƿ���ڽ�ɫ�ȼ��������ڣ�����ʾ�������޷�������������ȼ����������еǼǡ����������ڣ��������һ��
//�жϸ������Ƿ������������������׶Σ������ڣ�����ʾ�������������������������������׶�ʱ�����޷����еǼǵġ����������ڣ��������һ��
//ϵͳ�ж�����Ƿ�Ϊ�������Ա���������ڣ�����ʾ�������Ѳ����ڱ����壬�޷��ȼ����������ڣ��������һ��
//����������������¼���
//����ɫ֮ǰδ�ڼ���Ǽ�����
//ϵͳ��ʾ�����Ǽ��������������������ػ������Ĺ����̶ȣ��Ӷ���ü����ػ�������������ͬʱ������������������ʱ���ܣ�������ʱȡ�ػ��滻�����Ǽǵ���������ȷ�Ϻ󣬲��������¼���
//�Ǽ������ɹ��������ڸý�ɫӵ���˹����̶ȣ��Ӷ�������ػ�����
//������Ͽ۳�������������ʱ���浽������
//����ɫ֮ǰ���ڼ���Ǽ�������
//ϵͳ��ʾ���������Ǽǵ���������###��ԭ�Ǽ��������滻Ϊ###�������Ǽǵ������������Ƿ�ȷ�ϣ�������ȷ��/ȡ�������ȷ���󣬲��������¼���
//������ȼ������������滻�����½���1�ι����̶����㣬���¼����������õ��ػ�����
//�۳����ϣ���Ǽǵ��������������֮ǰ�Ǽǵ�����

	tagItem *pItem = GetItemMgr().GetItemFromAllContainer(n8ID);
	if( !P_VALID(pItem) ) 
		return GT_INVALID;

	if( !MIsFaBao(pItem) )
		return GT_INVALID;
	
	tagFabao *pFabao = (tagFabao*)pItem;

	//�жϸ�������Ʒ���Ƿ�Ϊ��ɫ
	if(pFabao->equipSpec.byQuality <= EIQ_White)
		return EFSE_FABAO_WHITE;

	//�жϸ������ĵȼ��Ƿ���ڽ�ɫ�ȼ�
	if( pFabao->n16Stage > GetLevel() )
		return EFSE_FABAO_LEVEL; 

	//�жϸ������Ƿ������������������׶�
	if( EEFS_Waiting == pFabao->eFabaoStatus || EEFS_Pregnancy == pFabao->eFabaoStatus || EEFS_GiveBirth == pFabao->eFabaoStatus || EEFS_Rest == pFabao->eFabaoStatus )
		return EFSE_FABAO_STATU; 

	return E_Success;
}
//---------------------------------------------------------------------------------
DWORD Role::FamilySpriteGetBack()
{
	DWORD dwRet = CanFamilySpriteGetBack(GetID());
	if( E_Success != dwRet )
		return dwRet;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return GT_INVALID;

	INT n16IndexDst = GetItemMgr().GetBagOneFreeSpace();
	if(GT_INVALID == n16IndexDst)
		return EFSE_FABAO_BAG;

	tagFabao *pFabao = pFamilySprite->RemoveRoleSprite(GetID());
	if( !P_VALID(pFabao) )
		return GT_INVALID;

	//GetItemMgr().GetBag().Add(pFabao,n16IndexDst);
	tagItem *pItem = pFabao;
	//GetItemMgr().Add2Bag( pItem, ELCLD_Family_Role_Sprite );
	DWORD dwRet1 = GetItemMgr().Add2Bag( pItem, ELCLD_Family_Role_Sprite, FALSE, TRUE, TRUE );
	if(dwRet1 != 0)
		ILOG->Write(_T("RoleID is %d, ELCLD_Family_Role_Sprite2 Result:%d!\r\n"),GetID(),dwRet1);
	//pFabao->SetPos( EICT_Bag, n16IndexDst );
	//pFabao->SetUpdate(EUDBS_Update);

	// ��ͻ��˷���Ϣ
	//tagNS_ItemPosChangeEx sendPos;
	//sendPos.eConTypeSrc1 = EICT_Family;
	//sendPos.eConTypeSrc2 = EICT_Bag;
	//sendPos.eConTypeDst1 = EICT_Bag;
	//sendPos.eConTypeDst2 = EICT_Family;
	//sendPos.n64Serial1 = pFabao->n64Serial;
	//sendPos.n64Serial2 = GT_INVALID;
	//sendPos.n16Num1 = 1;
	//sendPos.n16Num2 = GT_INVALID;
	//sendPos.n16PosDst1 = n16IndexDst;
	//sendPos.n16PosDst2 = GT_INVALID;


	//tagNS_NewFabaoAdd send;
	//send.fabao = *pFabao;

	//SendMessage(&send, send.dwSize);

	return E_Success;
}
//---------------------------------------------------------------------------------
//�Ƿ����ȡ���������
DWORD Role::CanFamilySpriteGetBack( INT64 n8ID )
{
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return GT_INVALID;

	if( !pFamilySprite->HaveRoleSprite( GetID() ) )
		return EFSE_FABAO_NOT_HAVE;

	INT16 n2Free = GetItemMgr().GetBag().GetFreeSpaceSize();
	if( n2Free <= 0 )
		return EFSE_FABAO_BAG;

	return E_Success;
}
//---------------------------------------------------------------------------------
//ֱ�ӷ��ظ���ɫ�Ǽǵ�����
VOID Role::FamilyRoleSpriteReturn()
{
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return;

	tagFabao *pFabao = pFamilySprite->RemoveRoleSprite(GetID());
	if( !P_VALID(pFabao) )
		return;

	// ��ͻ��˷���Ϣ
	AddRoleSpriteFromFamily( pFabao );
}
//---------------------------------------------------------------------------------
VOID Role::AddRoleSpriteFromFamily( tagFabao *pFabao )
{
	if( !P_VALID(pFabao) )
		return;

	DWORD dwRet = GT_INVALID;
	
	tagItem *pItem = pFabao;
	//dwRet = GetItemMgr().Add2Bag( pItem, ELCLD_Family_Role_Sprite );
	dwRet = GetItemMgr().Add2Bag( pItem, ELCLD_Family_Role_Sprite, FALSE, TRUE, TRUE );
	if( E_Success != dwRet )
	{
		dwRet = GetItemMgr().Add2RoleWare( pItem, ELCLD_Family_Role_Sprite, FALSE, TRUE, TRUE );
		if( E_Success != dwRet )
		{
			GetItemMgr().Add2BaiBao( pItem, ELCLD_Family_Role_Sprite, FALSE, TRUE, TRUE );
			ILOG->Write(_T("ELCLD_Family_Role_Sprite3!\r\n"));
		}
	}
}
//---------------------------------------------------------------------------------

VOID Role::SyncFamilyActive(INT nActive)
{
	tagNS_SyncFamilyActive send;
	send.nActive = nActive;
	SendMessage(&send, send.dwSize);
}

//---------------------------------------------------------------------------------
//��ɫ��½ʱ���µǼ��������������Լӳ�
VOID Role::UpdateRoleFamilySprite()
{
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return;

	pFamilySprite->UpdateRoleAtt(GetID());
}
//---------------------------------------------------------------------------------
BOOL Role::InSamePKStateEx(Role * pRole)
{
	if( !P_VALID(pRole) )
		return FALSE;

	DWORD myStatus = GetRoleState();
	DWORD otherStatus = pRole->GetRoleState();

	if( ((myStatus & ERS_Safeguard) && (otherStatus & ERS_PeaceModel)) || ((myStatus & ERS_PeaceModel) && (otherStatus & ERS_Safeguard)) )
		return TRUE;

	return FALSE;
}
//---------------------------------------------------------------------------------
VOID Role::SendInitStateClientConfig()
{
	tagNS_GetRoleInitState_ClientConfig send;
	send.nDistance = g_world.GetDistance();
	SendMessage(&send,send.dwSize);
}
//---------------------------------------------------------------------------------
// �̳���Ʒ�����������
INT Role::BuyThisMallItemCount(DWORD dwTypeID)
{
	if (!m_mapBuyMallItemCount.IsExist(dwTypeID))
	{
		return 0;
	}

	return m_mapBuyMallItemCount.Peek(dwTypeID);
}

VOID Role::IncreaseRoleBuyMallItemCount(DWORD dwTypeID, INT nNum)
{
	if (!m_mapBuyMallItemCount.IsExist(dwTypeID))
	{
		m_mapBuyMallItemCount.Add(dwTypeID, nNum);
	}

	INT nTmp = m_mapBuyMallItemCount.Peek(dwTypeID);
	m_mapBuyMallItemCount.ChangeValue(dwTypeID, nTmp+nNum);
}
//---------------------------------------------------------------------
VOID Role::UpdateFamilyTrainNum()
{
	tagDWORDTime cur_time = GetCurrentDWORDTime();
	
	tagDWORDTime old_time(m_dwTrainDate);

	if(cur_time.day != old_time.day || cur_time.month != old_time.month || cur_time.year != old_time.year)
	{
		m_nTrainNum = 0;
		m_dwTrainDate = DWORD(cur_time);
	}
}
//---------------------------------------------------------------------
VOID Role::AddRoleAwardPoint(INT nData)
{
	m_nAwardPoint += nData;

	const tagIDBuyBack *pIDBuyBack = g_attRes.GetIdbuybackProto(100);
	if( P_VALID(pIDBuyBack))
	{
		if(m_nAwardPoint > pIDBuyBack->dwExp)	
		{
			m_nAwardPoint = pIDBuyBack->dwExp;
		}
	}

	tagNS_LiLianChange send;
	send.dwExp = nData;
	SendMessage(&send,send.dwSize);
}
//---------------------------------------------------------------------
BOOL Role::CanAddGiftByAwardPoint(DWORD &dwID, DWORD &dwGiftID)
{
	dwID = 0;

	for( int i = 1; i <= 100; ++i )
	{
		const tagIDBuyBack *pIDBuyBack = g_attRes.GetIdbuybackProto(i);
		if( P_VALID(pIDBuyBack) && m_nAwardPoint < pIDBuyBack->dwExp)
		{
			dwID = pIDBuyBack->dwID - 1;
			break;
		}
	}

	const tagIDBuyBack *pIDBuyBack = g_attRes.GetIdbuybackProto(100);
	if( P_VALID(pIDBuyBack) )
	{
		if( m_nAwardPoint >= pIDBuyBack->dwExp )
		{
			dwID = pIDBuyBack->dwID;
		}
	}

	if( 0 == dwID )
		return FALSE;

	INT nAwardFlag = dwID / 10;
	if(m_nAwardFlag >= nAwardFlag)
	{
		return FALSE;
	}

	if( m_nAwardFlag + 1 < nAwardFlag )
	{
		dwID = (m_nAwardFlag + 1)*10;
		const tagIDBuyBack *pIDBuyBack = g_attRes.GetIdbuybackProto( dwID );
		if( !P_VALID(pIDBuyBack) )
		{
			return FALSE;
		}

		dwGiftID = pIDBuyBack->dwGiftID;
		return TRUE;
	}

	pIDBuyBack = g_attRes.GetIdbuybackProto(dwID);
	if( !P_VALID(pIDBuyBack) )
	{
		return FALSE;
	}

	dwGiftID = pIDBuyBack->dwGiftID;

	return TRUE;
}
//---------------------------------------------------------------------
DWORD Role::AddGiftByAwardPoint()
{
	DWORD dwID = 0;
	DWORD dwGiftID = 0;

	if( !CanAddGiftByAwardPoint(dwID,dwGiftID) )
	{
		return E_NOT_GET_GIFT;
	}

	if( !GetItemMgr().Add2Role(EICM_Zhanghao,dwID,dwGiftID,1,EIQ_Null,ELCID_Zhanghao_Gift))
	{
		return GT_INVALID;
	}

	m_nAwardFlag++;

	return E_Success;
}
//---------------------------------------------------------------------


VOID Role::MakeBorther(DWORD dwID,bool IsTeacher /*= false*/)
{
	if (!IsTeacher)
		m_BrotherMgr.SetTeatherID(dwID);
}

VOID Role::MakeBrotherSuccess(DWORD dwRoleID)
{
	tagNS_TryMakeBrother cmd;
	cmd.dwErrorCode = E_Success;
	cmd.dwRoleID = dwRoleID;
	SendMessage(&cmd,cmd.dwSize);
}

VOID	Role::SyncBrotherEndTime()
{
	tagNS_BrotherLeftSecond cmd;
	cmd.dwTeacherID = m_BrotherMgr.GetTeatherID();
	cmd.dwEndTime = m_BrotherMgr.GetEndTime();
	SendMessage(&cmd,cmd.dwSize);
}

bool Role::IsBrotherOf(DWORD dwID)
{
	if (dwID == m_BrotherMgr.GetTeatherID())
	{
		if (1 == CompareDwordTime(m_BrotherMgr.GetEndTime(),GetCurrentDWORDTime()))
			return true;
		else
		{
			DWORD dwTeamID = GetTeamID();
			if (P_VALID(dwTeamID))
			{
				const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
				if (P_VALID(pTeam))
				{
					pTeam->DeleteTeamBloodBrotherBuff(this);
				}
			}

			m_BrotherMgr.GetData().clearTeatherInfo();
			// ֪ͨ�ͻ���
			SyncClient2EndBloodBrother();
			// ȡ��BUFF
			CancelBrotherProfit();
			return false;
		}
	}
	return false;
}

VOID	Role::SyncClient2EndBloodBrother()
{
	// ֪ͨ�ͻ���
	tagNS_BrotherEnd msg2;
	msg2.bReason = 0;
	SendMessage(&msg2,msg2.dwSize);
}

DWORD Role::ActivePulse(DWORD dwID)
{
	tagPulseProto* pProto = g_attRes.GetPulseProto(dwID);
	if(!P_VALID(pProto))	return E_Pulse_NoProto;

	// �ȼ����
	if(	GetLevel() < MIN_PULSE_LEVEL	 ||  GetLevel() < pProto->dwLevelLock )
		return E_Pulse_LessLevel;

	// �����
	if( GetAttValue(ERA_God_Godhead)  < pProto->dwGodLock )
		return E_Pulse_LessGodLevel;

	// ѧϰ�������
	if(  m_nPulseLearnTimes >= MAX_PULSE_TIMEONEDAY	)
		return E_Pulse_MaxTimePerDay;

	tagSpecSkillUsedParam param;
	param.dwSkillID = dwID;
	param.dwNPCID = 0;
	param.n64ItemID = 0;
	param.checkfunc = &Role::CheckPulseSkill;
	param.calcostfunc = &Role::CalPulseSkillCost;
	param.learnfunc = &Role::LearnPulseSkillSpec;
	return LearnSpecSkillLimit(&param);

}

VOID Role::ResetPulseCanLearnNum()
{
	m_nPulseLearnTimes = 0;
 	tagNS_PulseCanLearnNum __msg;
 	__msg.dwNum = 0;
 	SendMessage(&__msg,__msg.dwSize);
}

DWORD Role::CheckPulseSkill(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID)
{
	tagPulseProto* pProto = g_attRes.GetPulseProto(dwID);
	if(!P_VALID(pProto))	return E_Pulse_NoProto;

	Skill* pSkill = GetSkill(pProto->dwSkillID);
	DWORD dwSkillTypeID = P_VALID(pSkill) ? Skill::CreateTypeID(pSkill->GetID(), pSkill->GetLevel() + 1) : pProto->dwSkillID * 100 + 1;

	if( P_VALID(pSkill) )
	{
		if( pSkill->GetLevel() >= pSkill->GetMaxLearnLevel() )
			return E_Pulse_MaxLevel;
	}
	
	const tagSkillProto* pSkillProto  =  g_attRes.GetSkillProto(dwSkillTypeID);
	if(!P_VALID(pSkillProto))
		return E_Pulse_NoProto;

	//����Ƿ񳬹���ѧ�ȼ�����
	if(  Skill::GetLevelFromTypeID(dwSkillTypeID) > pProto->dwMaxLevel )
		return E_Pulse_MaxLevel;

	//��Ǯ
	if(GetCurMgr().GetBagSilver() < pProto->dwMoney)
		return E_Pulse_NoMoney;

	//��Ʒ
	if(pProto->dwItemID != GT_INVALID)
	{
		if(GetItemMgr().GetBagSameItemCount(pProto->dwItemID) < pProto->dwItemNum)
			return E_Pulse_NoItem;
	}

	if(pProto->dwItemID2 != GT_INVALID)
	{
		if(GetItemMgr().GetBagSameItemCount(pProto->dwItemID2) < pProto->dwItemNum2)
			return E_Pulse_NoItem;
	}

	// �������ȼ�
	if( m_nLevel < pSkillProto->nNeedRoleLevel )
		return E_LearnSkill_NeedMoreLevel;

	// ���ְҵ����
	if( E_Success != VocationCheck( pSkillProto->dwVocationLimit ))
		return E_Pulse_NoProto;
	
	// ���ǰ�ü���
	if( GT_INVALID != pProto->dwPerSkill1)
	{
		Skill* pPreSkill = GetSkill(pProto->dwPerSkill1/100);
		if( !P_VALID(pPreSkill))
			return E_Pulse_NoPerSkill;

		if(pPreSkill->GetLevel() < pProto->dwPerSkill1%100)
			return E_Pulse_NoPerSkill;
	}

	if( GT_INVALID != pProto->dwPerSkill2)
	{
		Skill* pPreSkill = GetSkill(pProto->dwPerSkill2/100);
		if( !P_VALID(pPreSkill))
			return E_Pulse_NoPerSkill;

		if(pPreSkill->GetLevel() < pProto->dwPerSkill2%100)
			return E_Pulse_NoPerSkill;
	}

	if( GT_INVALID != pProto->dwPerSkill3)
	{
		Skill* pPreSkill = GetSkill(pProto->dwPerSkill3/100);
		if( !P_VALID(pPreSkill))
			return E_Pulse_NoPerSkill;

		if(pPreSkill->GetLevel() < pProto->dwPerSkill3%100)
			return E_Pulse_NoPerSkill;
	}

	return E_Success;
}
DWORD Role::CalPulseSkillCost(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID)
{
	tagPulseProto* pProto = g_attRes.GetPulseProto(dwID);
	if(!P_VALID(pProto))	return E_Pulse_NoProto;

	Skill* pSkill = GetSkill(pProto->dwSkillID);
	DWORD dwSkillTypeID = P_VALID(pSkill) ? Skill::CreateTypeID(pSkill->GetID(), pSkill->GetLevel() + 1) : pProto->dwSkillID * 100 + 1;

	//�Ҳ���ԭ��
	const tagSkillProto* pSkillProto  =  g_attRes.GetSkillProto(dwSkillTypeID);
	if(!P_VALID(pSkillProto))
		return E_Pulse_NoProto;


	//���������
	if(m_nPulseLearnTimes >= MAX_PULSE_TIMEONEDAY)
	{
		m_nPulseLearnTimes = MAX_PULSE_TIMEONEDAY;
		return E_Pulse_MaxTimePerDay;
	}
	++m_nPulseLearnTimes;

	bool bSuccess = true;
	INT nSuccessNum = IUTIL->RandomInRange(0,10000);
	if(  nSuccessNum > pProto->dwSuccessRate ) // ʧ��
	{
		bSuccess = false;
	}

	//��Ǯ
	GetCurMgr().DecBagSilver(pProto->dwMoney, ELCID_PulseSkill);

	//������Ʒ
	if (pProto->dwItemID != GT_INVALID)
		GetItemMgr().RemoveFromRole(pProto->dwItemID, pProto->dwItemNum, ELCID_PulseSkill);

	if (pProto->dwItemID2 != GT_INVALID)
		GetItemMgr().RemoveFromRole(pProto->dwItemID2, pProto->dwItemNum2, ELCID_PulseSkill);

	if( !bSuccess )
		return E_Pulse_Fail;


	return E_Success;
}
DWORD Role::LearnPulseSkillSpec(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID)
{
	tagPulseProto* pProto = g_attRes.GetPulseProto(dwID);
	if(!P_VALID(pProto))	return E_Pulse_NoProto;

	// ���ü����Ƿ��Ѿ�����
	Skill* pSkill = GetSkill(pProto->dwSkillID);
	if( P_VALID(pSkill) )
	{	
		//����������
		DWORD dwErrorCode  =LevelUpSkill(pProto->dwSkillID);
		if(dwErrorCode != E_Success)
			return dwErrorCode;
	}
	//�����ǰ�������������
	else
	{
		//ѧϰ����
		DWORD dwErrorCode = LearnSkill(pProto->dwSkillID);
		if(dwErrorCode != E_Success)
			return dwErrorCode;
	}

	return E_Success;
}

DWORD Role::OnRoleEquipGetGrowID(DWORD dwTypeID, BYTE byQlty)
{
	if (P_VALID(m_pScript))
	{
		return m_pScript->OnRoleEquipGetGrowID(this, dwTypeID, (INT)byQlty);
	}

	return 0;
}

BYTE Role::OnRoleEquipGrow(DWORD dwTypeID, BYTE byQlty, INT nGrowTimes, INT nIMUseTimes, BYTE byGrowLevel, 
						   BYTE byGrayNum, BYTE byGreenNum, BYTE byBlueNum, BYTE byPurpleNum, DWORD dwIM)
{
	if (P_VALID(m_pScript))
	{
		return m_pScript->OnRoleEquipGrow(this, dwTypeID, byQlty, nGrowTimes, nIMUseTimes, byGrowLevel, 
			byGrayNum, byGreenNum, byBlueNum, byPurpleNum, dwIM);
	}

	return (BYTE)EEGS_End;
}

DWORD Role::OnRoleUseDiamondEquipGrowIM(DWORD dwTypeID, INT64 n64EquipID)
{
	DWORD dwErrorCode;
	if(P_VALID(m_pScript))
	{
		dwErrorCode = m_pScript->OnUseDiamondAddAndEquipGrowIM(this, dwTypeID, n64EquipID);
	}
	//�жϳɹ���ɾ����Ʒ
	if(dwErrorCode == 1)
	{
		ItemMgr& itemMgr = GetItemMgr();
		itemMgr.RemoveFromRole(dwTypeID, 1, (DWORD)ELCID_EquipGrowIDOrDiamondAdd);	//ɾ������

		//���سɹ�
		return 1;
	}
	else
	{
		//����ʧ��
		return 0;	
	}
	return 0;
}

DWORD Role::OnRefreshBaoXiang(BOOL bItemFree)
{
	WarRoleMgr* pWarRoleMgr = GetMap()->GetWarRoleMgr();
	if (!P_VALID(pWarRoleMgr))
	{
		return GT_INVALID;
	}

	if (EWRR_NULL == pWarRoleMgr->GetWarResult())
	{
		return GT_INVALID;
	}

	// ˢ�µ��� 3110037ս��ƾ֤
	ItemContainer &bag = GetItemMgr().GetBag();
	if (FALSE == bItemFree && bag.GetSameItemCount(REFRESH_BAOXIANG_ITEMID) <= 0)
	{
		return GT_INVALID;
	}

	// //���ܷ�=��ɱ������*1+��ɱ������*10+��ɱϡ��boss*30-��������*50
	WarRole *pWarRole = GetWarRole();
	if (!P_VALID(pWarRole) || !IsInWar())
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode;
	DWORD dwItemID1 = GT_INVALID;
	DWORD dwItemID2 = GT_INVALID;
	DWORD dwItemID3 = GT_INVALID;
	DWORD dwScole = pWarRole->GetMonsterKillCount()
					+ pWarRole->GetBossKillCount() * 10
					+ pWarRole->GetRareBossKillCount() * 30
					- pWarRole->GetRoleDeadCount() * 50;

	if (EWRR_Lose == pWarRoleMgr->GetWarResult())
	{
		// ʧ�ܼ���
		dwScole /= 2;
	}

	if(P_VALID(m_pScript))
	{
		dwErrorCode = m_pScript->OnRefreshBaoXiang(dwScole, dwItemID1, dwItemID2, dwItemID3);
		WarRole* pWarRole = GetWarRole();
		if (!P_VALID(pWarRole))
		{
			return GT_INVALID;
		}

		// �۳����ĵ���
		if (FALSE == bItemFree)
		{
			dwErrorCode = GetItemMgr().RemoveFromRole(REFRESH_BAOXIANG_ITEMID, 1, ELCID_WarCost);
			ASSERT(E_Success == dwErrorCode);
		}


		pWarRole->SetRewardItemID(dwItemID1, dwItemID2, dwItemID3);

		tagNS_RefreshBaoXiang msg;
		msg.byErrorCode = pWarRoleMgr->GetWarResult();
		msg.dwItemID1	= dwItemID1;
		msg.dwItemID2	= dwItemID2;
		msg.dwItemID3	= dwItemID3;
		SendMessage(&msg, msg.dwSize);
	}
	return 0;
}

DWORD Role::OnGetBaoXiang(BYTE byIndex, DWORD dwItemID)
{
	WarRoleMgr* pWarRoleMgr = GetMap()->GetWarRoleMgr();
	if (!P_VALID(pWarRoleMgr))
	{
		return GT_INVALID;
	}
	const tagWarRewardPointProto* pProto = g_attRes.GetWarRewardPointProto(GetLevel());
	WarRole* pWarRole = GetWarRole();
	if (!P_VALID(pProto) || !P_VALID(pWarRole) || !IsInWar())
	{
		return GT_INVALID;
	}

	if (EWRR_NULL == pWarRoleMgr->GetWarResult())
	{
		return GT_INVALID;
	}
	// �Ƿ�����콱

	// ��֤��Ʒ
	if (pWarRole->GetRewardItemID(byIndex) != dwItemID)
	{
		return GT_INVALID;
	}

	// Ԥ��鱳�����Ƿ��п�λ
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagItem *pItemNew = ItemCreator::CreateEx(EICM_WarReward, GT_INVALID, dwItemID, 1, EIQ_White);
	if(!P_VALID(pItemNew))
	{
		ASSERT(P_VALID(pItemNew));
		return E_Item_AddFailed;
	}
	if (P_VALID(pItemNew->pProtoType) && pItemNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
		pItemNew->dw1stGainTime	 = g_world.GetWorldTime();

	GetItemMgr().Add2Bag(pItemNew, ELCID_WarReward, TRUE);

	GetCurMgr().IncBagSilver(pProto->dwMoney, ELCID_WarReward);
	ExpChange(pProto->dwExp);

	return E_Success;
}

VOID Role::RenewSoulCrystalAtt(DWORD dwTypeID, BYTE byQlty, INT* nAtt, OUT DWORD& dwErrorCode, OUT INT* nRetAtt)
{
	Role* pRole = this;

	if(!P_VALID(m_pScript))
		return;

	if(!P_VALID(nAtt) || !P_VALID(nRetAtt))
		return;

	m_pScript->RenewSoulCrystalAtt(pRole, dwTypeID, byQlty, nAtt, dwErrorCode, nRetAtt);
}

VOID Role::AccountSuccessReactive()
{
	Role* pRole = this;
	if (P_VALID(m_pScript))
	{
		m_pScript->AccountSuccessReactive(pRole);
	}
}

// DWORD Role::RewardReactive(BYTE byRewardLevel)
// {
// 	Role* pRole = this;
// 	if (P_VALID(m_pScript))
// 	{
// 		return m_pScript->RewardReactive(pRole, byRewardLevel);
// 	}
// 	else
// 		return E_AccountReactive_Error;
// }

//-------------------------------------------------------------------------------
DWORD Role::ActiveSoulSkill( DWORD dwSkillID, DWORD &dwTime )
{
	DWORD dwRet = CanActiveSoulSkill(dwSkillID);
	if( dwRet != E_Success )
		return dwRet;

	const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(dwSkillID);
	if(!P_VALID(pForceSkillProto))
	{
		return GT_INVALID;
	}

	DWORD dwTempID = pForceSkillProto->bySide * 10 + pForceSkillProto->dwLevel;
	const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(dwTempID);
	if(!P_VALID(pForceLevelProto))
	{
		return GT_INVALID;
	}

	const tagSkillProto *pSkillProto = g_attRes.GetSkillProto(pForceSkillProto->dwID);
	if(!P_VALID(pSkillProto))
	{
		return GT_INVALID;
	}

	//�����ħ��
	if( 0 == pForceSkillProto->bySide )
	{
		ChangeSoul(-pForceSkillProto->dwOpenSoul,0);
	}
	else
	{
		ChangeSoul(0,-pForceSkillProto->dwOpenSoul);
	}

	//��Ǯ
	CurrencyMgr &CurMgr = GetCurMgr();
	if( pForceSkillProto->dwOpenMoney > 0 )
	{
		if( !CurMgr.DecBagSilver( pForceSkillProto->dwOpenMoney, ELCID_SOUL_SKILL ) )
		{
			return E_SOUL_MONEY;
		}
	}

	//ѧϰ���߼����
	dwRet = AddSkillBySoul(pForceSkillProto->dwID,pForceSkillProto->bForerver,dwTime);

	return dwRet;
}
//-------------------------------------------------------------------------------
DWORD Role::ContinueSoulSkill( DWORD dwSkillID, INT64 n64ItemID, DWORD &dwTime )
{
	DWORD dwRet = CanContinueSoulSkill(dwSkillID,n64ItemID);
	if( dwRet != E_Success )
		return dwRet;

	const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(dwSkillID);
	if(!P_VALID(pForceSkillProto))
	{
		return GT_INVALID;
	}

	DWORD dwTempID = pForceSkillProto->bySide * 10 + pForceSkillProto->dwLevel;
	const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(dwTempID);
	if(!P_VALID(pForceLevelProto))
	{
		return GT_INVALID;
	}

	//const tagSkillProto *pSkillProto = g_attRes.GetSkillProto(pForceSkillProto->dwID);
	//if(!P_VALID(pSkillProto))
	//{
	//	return GT_INVALID;
	//}

	//��ȡ����
	Skill* pSkill = m_mapSkill.Peek(Skill::GetIDFromTypeID(dwSkillID));
	if(!P_VALID(pSkill))
	{
		return E_SOUL_SKILL;
	}

	//��ȡ��Լ����
	DWORD dwItemTypeID = 0;
	INT nDay = 0;
	tagItem *pItem = GetItemMgr().GetItemFromContainer(n64ItemID,EICT_Bag);
	if(P_VALID(pItem))
	{
		//dwItemTypeID = pItem->dwTypeID;
		const tagItemProto *pItemProto = g_attRes.GetItemProto(pItem->dwTypeID);
		if( !P_VALID(pItemProto) || EISF_Soul_Skill != pItemProto->eSpecFunc )
		{
			return E_SOUL_ITEM;
		}
		nDay = pItemProto->nSpecFuncVal1;
		if(-1 == nDay)
			nDay = 9999;
	}
	else
	{
		return E_SOUL_ITEM;
	}


	//�����ħ��
	if( 0 == pForceSkillProto->bySide )
	{
		ChangeSoul(-pForceSkillProto->dwContinueSoul,0);
	}
	else
	{
		ChangeSoul(0,-pForceSkillProto->dwContinueSoul);
	}

	//��Ǯ
	CurrencyMgr &CurMgr = GetCurMgr();
	if( pForceSkillProto->dwOpenMoney > 0 )
	{
		if( !CurMgr.DecBagSilver( pForceSkillProto->dwContinueMoney, ELCID_SOUL_CONTINUE_SKILL ) )
		{
			return E_SOUL_MONEY;
		}
	}

	//����Ʒ
	dwRet = GetItemMgr().DelFromBag(n64ItemID, ELCID_SOUL_CONTINUE_ITEM, 1);
	if(dwRet != E_Success)
	{
		CurMgr.IncBagSilver( pForceSkillProto->dwContinueMoney, ELCID_SOUL_CONTINUE_SKILL );
	}

	//��ʼ��Լ
	
	tagDWORDTime time = IncreaseTime( pSkill->GetActiveTime(),nDay * 24 *3600 );
	tagDWORDTime curtime = GetCurrentDWORDTime();
	dwTime = CalcTimeDiff( time, curtime );
	if( dwTime > SKILL_ACTIVE_TIME_MAX )
	{
		time = IncreaseTime( curtime, SKILL_ACTIVE_TIME_MAX - (dwTime - SKILL_ACTIVE_TIME_MAX) );
	}

	pSkill->SetActiveTime(time);

	return E_Success;
}
//-------------------------------------------------------------------------------
DWORD Role::CanActiveSoulSkill( DWORD dwSkillID )
{
	const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(dwSkillID);
	if(!P_VALID(pForceSkillProto))
	{
		return GT_INVALID;
	}

	// ���ְҵ�Ƿ����
	if ( E_Success != VocationCheck( pForceSkillProto->dwJob ) )
		return E_SOUL_JOB;

	// ��������Ƿ����
	DWORD dwTempID = pForceSkillProto->bySide * 10 + pForceSkillProto->dwLevel;
	const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(dwTempID);
	if(!P_VALID(pForceLevelProto))
	{
		return GT_INVALID;
	}

	if( 0 == pForceSkillProto->bySide )
	{
		if ( pForceLevelProto->dwPoint > m_nGodPoint )
			return E_SOUL_POINT;
	}
	else
	{
		if ( pForceLevelProto->dwPoint > m_nMonsterPoint )
			return E_SOUL_POINT;
	}

	//���ǰ�ü���
	if(0 != pForceSkillProto->dwNeedSkill)
	{
		//DWORD dwPreSkillID = Skill::GetIDFromTypeID(pForceSkillProto->dwNeedSkill);
		//INT nPreSkillLevel = Skill::GetLevelFromTypeID(pForceSkillProto->dwNeedSkill);
		Skill* pPreSkill = GetSkill(pForceSkillProto->dwNeedSkill);
		if( !P_VALID(pPreSkill) /*|| pPreSkill->GetLevel() < nPreSkillLevel */)
			return E_SOUL_PRESKILL;
	}

	//������ħ��
	if( 0 == pForceSkillProto->bySide )
	{
		if ( pForceSkillProto->dwOpenSoul > m_nGod )
			return E_SOUL_SOUL;
	}
	else
	{
		if ( pForceSkillProto->dwOpenSoul > m_nMonster )
			return E_SOUL_SOUL;
	}

	//���Ǯ
	CurrencyMgr &CurMgr = GetCurMgr();
	if( pForceSkillProto->dwOpenMoney > CurMgr.GetBagSilver() )
	{
		return E_SOUL_MONEY;
	}
	
	return E_Success;
}
//-------------------------------------------------------------------------------
DWORD Role::CanContinueSoulSkill( DWORD dwSkillID, INT64 n64ItemID )
{
	const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(dwSkillID);
	if(!P_VALID(pForceSkillProto))
	{
		return GT_INVALID;
	}

	// ���ְҵ�Ƿ����
	//if ( E_Success != VocationCheck( pForceSkillProto->dwJob ) )
	//	return E_SOUL_JOB;

	// ��������Ƿ����
	DWORD dwTempID = pForceSkillProto->bySide * 10 + pForceSkillProto->dwLevel;
	const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(dwTempID);
	if(!P_VALID(pForceLevelProto))
	{
		return GT_INVALID;
	}

	if( 0 == pForceSkillProto->bySide )
	{
		if ( pForceLevelProto->dwPoint > m_nGodPoint )
			return E_SOUL_POINT;
	}
	else
	{
		if ( pForceLevelProto->dwPoint > m_nMonsterPoint )
			return E_SOUL_POINT;
	}

	//���ǰ�ü���
	//if(0 != pForceSkillProto->dwNeedSkill)
	//{
	//	DWORD dwPreSkillID = Skill::GetIDFromTypeID(pForceSkillProto->dwNeedSkill);
	//	INT nPreSkillLevel = Skill::GetLevelFromTypeID(pForceSkillProto->dwNeedSkill);
	//	Skill* pPreSkill = GetSkill(dwPreSkillID);
	//	if( !P_VALID(pPreSkill) || pPreSkill->GetLevel() < nPreSkillLevel )
	//		return E_SOUL_PRESKILL;
	//}

	//������ħ��
	if( 0 == pForceSkillProto->bySide )
	{
		if ( pForceSkillProto->dwContinueSoul > m_nGod )
			return E_SOUL_SOUL;
	}
	else
	{
		if ( pForceSkillProto->dwContinueSoul > m_nMonster )
			return E_SOUL_SOUL;
	}

	//���Ǯ
	CurrencyMgr &CurMgr = GetCurMgr();
	if( pForceSkillProto->dwContinueMoney > CurMgr.GetBagSilver() )
	{
		return E_SOUL_MONEY;
	}

	//���ü����Ƿ����
	Skill* pSkill = m_mapSkill.Peek(Skill::GetIDFromTypeID(dwSkillID));
	//if(P_VALID(pSkill))
	//{
	//	DWORD dwLevel = Skill::GetLevelFromTypeID(dwSkillID);

	//	//����ͬһ����
	//	if( dwLevel != pSkill->GetLevel())
	//	{
	//		return E_SOUL_SKILL;
	//	}
	//}
	//else
	//{
	//	return E_SOUL_SKILL;
	//}
	if(!P_VALID(pSkill))
	{
		return E_SOUL_SKILL;
	}
	
	//�����Ʒ����
	tagItem *pItem = GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pItem))
	{
		return E_SOUL_ITEM;
	}


	return E_Success;
}
//-------------------------------------------------------------------------------
//ͨ����ħֱ�ӼӼ���
DWORD Role::AddSkillBySoul(DWORD dwSkillID,BOOL bForerver, DWORD &dwTime)
{
	Skill* pSkill = m_mapSkill.Peek(Skill::GetIDFromTypeID(dwSkillID));
	if(P_VALID(pSkill))
	{
		DWORD dwLevel = Skill::GetLevelFromTypeID(dwSkillID);

		//ͬһ���ܲ���������
		if( dwLevel == pSkill->GetLevel())
		{
			return GT_INVALID;
		}
		else
		{
			RemoveSkill(Skill::GetIDFromTypeID(dwSkillID));
		}
	}
	// ��Ӽ���
	pSkill = new Skill(Skill::GetIDFromTypeID(dwSkillID), 0, Skill::GetLevelFromTypeID(dwSkillID), 0, 0);
	AddSkill(pSkill);

	const SkillScript *pSkillScript = pSkill->GetSkillScript();
	if( P_VALID(pSkillScript) )
	{
		pSkillScript->OnSkillChange(GetMap(),GetID(),pSkill->GetID(),0,1);
	}

	const RoleScript *pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript) && P_VALID(pSkill))
	{
		DWORD dwSkillIDWithLevel = pSkill->GetID()*100+pSkill->GetLevel();
		pRoleScript->OnLearnSkill(this, dwSkillIDWithLevel);
	}

	if( !bForerver )
	{
		dwTime = SKILL_ACTIVE_TIME;
		tagDWORDTime time = IncreaseTime(GetCurrentDWORDTime(),SKILL_ACTIVE_TIME);
		pSkill->SetActiveTime(time);
	
		m_listSkillActiveTime.PushBack(pSkill->GetID());
	}

	// ���¼�������
	if( NeedRecalAtt() )
		RecalAtt();

	return E_Success;
}
//---------------------------------------------------------------------------------
//�ı���ħ����
VOID Role::ChangeSoulPoint( INT nGodPoint, INT nMonsterPoint )
{
	m_nGodPoint += nGodPoint;
	m_nMonsterPoint += nMonsterPoint;

	//������ħ�꼼��
	//if( nGodPoint < 0 || nMonsterPoint < 0 )
	//{
	//	FiltrateSoulSkill(); 
	//}

	if(m_nGodPoint < 0)
		m_nGodPoint = 0;

	if(m_nMonsterPoint < 0)
		m_nMonsterPoint = 0;

	tagNS_ChangeSoul send;
	send.nGod = m_nGod;
	send.nMonster = m_nMonster;
	send.nGodPoint = m_nGodPoint;
	send.nMonsterPoint = m_nMonsterPoint;
	SendMessage(&send,send.dwSize);
}
//---------------------------------------------------------------------------------
//�ı���ħ��
VOID Role::ChangeSoul( INT nGod, INT nMonster )
{
	m_nGod += nGod;
	m_nMonster += nMonster;

	if(m_nGod < 0)
		m_nGod = 0;

	if(m_nMonster < 0)
		m_nMonster = 0;

	tagNS_ChangeSoul send;
	send.nGod = m_nGod;
	send.nMonster = m_nMonster;
	send.nGodPoint = m_nGodPoint;
	send.nMonsterPoint = m_nMonsterPoint;
	SendMessage(&send,send.dwSize);
}
//---------------------------------------------------------------------------------
//������ħ�꼼��
VOID Role::FiltrateSoulSkill()
{
	TMap<DWORD, Skill*>::TMapIterator iter = m_mapSkill.Begin();
	Skill* pSkill = NULL;
	while(m_mapSkill.PeekNext(iter, pSkill))
	{
		if (!P_VALID(pSkill))
		{
			continue;
		}

		const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(pSkill->GetTypeID());
		if(!P_VALID(pForceSkillProto))
		{
			continue;
		}

		if( !pForceSkillProto->bDelete )
		{
			continue;
		}

		// ���ְҵ�Ƿ����
		if ( E_Success != VocationCheck( pForceSkillProto->dwJob ) )
		{
			RemoveSkill(pSkill->GetID());
		}

		// ��������Ƿ����
		//DWORD dwTempID = pForceSkillProto->bySide * 10 + pForceSkillProto->dwLevel;
		//const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(dwTempID);
		//if(!P_VALID(pForceLevelProto))
		//{
		//	continue;
		//}

		//if( 0 == pForceSkillProto->bySide )
		//{
		//	if ( pForceLevelProto->dwPoint > m_nGodPoint )
		//	{
		//		//tagNS_ActiveSoulSkill send;
		//		//send.dwSkillID = pSkill->GetTypeID();	//����ID
		//		//send.dwResult = E_Success;				//���
		//		//send.byOpt = 1;
		//		//SendMsg(send,send.dwSize);

		//		RemoveSkill(pSkill->GetID());
		//	}
		//}
		//else
		//{
		//	if ( pForceLevelProto->dwPoint > m_nMonsterPoint )
		//	{
		//		//tagNS_ActiveSoulSkill send;
		//		//send.dwSkillID = pSkill->GetTypeID();	//����ID
		//		//send.dwResult = E_Success;				//���
		//		//send.byOpt = 1;
		//		//SendMsg(send,send.dwSize);

		//		RemoveSkill(pSkill->GetID());
		//	}
		//}
	}
}
//---------------------------------------------------------------------------------
BYTE Role::GetGodSoulLevel()
{
	INT nGodSoulPoint = GetGodPoint();

	for( int i = 0; i < 5; ++i )
	{
		const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(i);
		if(!P_VALID(pForceLevelProto))
		{
			continue;
		}

		if( nGodSoulPoint < pForceLevelProto->dwPoint )
		{
			return i-1;
		}
	}

	return 5;
}
//---------------------------------------------------------------------------------
BYTE Role::GetMonsterSoulLevel()
{
	INT nMonsterSoulPoint = GetMonsterPoint();

	for( int i = 10; i < 15; ++i )
	{
		const tagForceLevelProto* pForceLevelProto = g_attRes.GetForceLevelProto(i);
		if(!P_VALID(pForceLevelProto))
		{
			continue;
		}

		if( nMonsterSoulPoint < pForceLevelProto->dwPoint )
		{
			return i-1;
		}
	}

	return 15;
}
//---------------------------------------------------------------------------------
VOID Role::UpdateGetCash()
{
	if( !g_world.IsUseIPGSystem() )
		return;
	
	if( !m_bGetCash || m_CurMgr.GetBagYuanBao() < 0 )
	{
		//g_world.WriteKroeaLog(_T("Role UpdateGetCash. \r\n"));

		//PlayerSession * pSession = GetSession();
		//if( P_VALID(pSession))
		//{
		//	g_world.WriteKroeaLog(_T("UpdateGetCash GetSession ok. \r\n"));

		//	TCHAR szName[32] ={0};
		//	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(GetID());
		//	if (P_VALID(pRoleInfo))
		//	{
		//		_tcscpy( szName, pRoleInfo->szRoleName );
		//	}

		//	IPGDragonfly* pIPGDragonfly = IPGDragonfly::GetInstance();
		//	if( NULL != pIPGDragonfly && pIPGDragonfly->IsInitialize() )
		//	{
		//		int nCash = 0;

		//		g_world.WriteKroeaLog(_T("UpdateGetCash GetDFCashReq begin. \r\n"));

		//		if( pIPGDragonfly->GetDFCashReq( pSession->GetAccount(), pSession->GetKrUserID(), IUTIL->UnicodeToAnsi(szName), nCash))
		//		{
		//			TCHAR tszTemp[64] = {0};
		//			wsprintf(tszTemp,_T("UpdateGetCash GetDFCashReq cash:%d. \r\n"),nCash);
		//			g_world.WriteKroeaLog(tszTemp);

		//			SetRoleBagYuanBao(nCash);

		//			SetGetCash(TRUE);
		//			g_world.WriteKroeaLog(_T("SetGetCash TRUE. \r\n"));

		//			//���͵��ͻ���
		//			SendBagYuanBao();
		//		}

		//		g_world.WriteKroeaLog(_T("UpdateGetCash GetDFCashReq end. \r\n"));
		//	}
		//}
		//else
		//{
		//	g_world.WriteKroeaLog(_T("role getsession err. \r\n"));
		//}

		int nCash = GetIPGCash();
		if(nCash >= 0)
		{
			SetRoleBagYuanBao(nCash);

			SetGetCash(TRUE);
			g_world.WriteKroeaLog(_T("SetGetCash TRUE. \r\n"));

			//���͵��ͻ���
			SendBagYuanBao();
		}
	}
	else
	{
		g_world.WriteKroeaLog(_T("get cash3. \r\n"));
	}
}
//---------------------------------------------------------------------------------
VOID Role::ContinueSoulSkillByBook( DWORD dwSkillID, tagDWORDTime time )
{
	//��ȡ����
	Skill* pSkill = m_mapSkill.Peek(dwSkillID);
	if(!P_VALID(pSkill))
	{
		return;
	}

	//��ʼ��Լ
	pSkill->SetActiveTime(time);

	//��ʱ�䷢���ͻ���
	DWORD dwTime = 0;
	tagDWORDTime curtime = GetCurrentDWORDTime();
	dwTime = CalcTimeDiff( time, curtime );
	if( dwTime > SKILL_ACTIVE_TIME_MAX )
	{
		time = IncreaseTime( curtime, SKILL_ACTIVE_TIME_MAX - (dwTime - SKILL_ACTIVE_TIME_MAX) );
	}

	tagNS_ContinueSoulSkill send;
	send.dwSkillID = pSkill->GetTypeID();
	send.dwResult = E_Success;
	send.dwTime = dwTime;

	SendMessage( &send, send.dwSize);
}

VOID Role::CheckAllEquip(DWORD dwMapID)
{
	m_ItemMgr.CheckAllEquip(this,dwMapID);
	SendAvatarEquip2Client();
}

//---------------------------------------------------------------------------------
BOOL Role::DecIPGCash( int nYuanbao, int nNum/*=0*/, DWORD dwItemTypeID/*=0*/ )
{
	//INT32 nBagYuanBao = GetCurMgr().GetBagYuanBao();

	if(g_world.IsUseIPGSystem())
	{
		TCHAR tszTemp[64] = {0};
		wsprintf( tszTemp, _T("Role Dec cash begin. BagYuanBao:%d,itemID:%u. \r\n"),nYuanbao,dwItemTypeID);
		g_world.WriteKroeaLog(tszTemp);

		PlayerSession *pPlayerSession = GetSession();
		if (!P_VALID(pPlayerSession))
		{
			g_world.WriteKroeaLog(_T("Role Dec cash Session err. \r\n"));
			return FALSE;
		}
		//���͵�IPG
		IPGDragonfly* pIPGDragonfly = IPGDragonfly::GetInstance();
		if( NULL != pIPGDragonfly && pIPGDragonfly->IsInitialize() )
		{
			char szUserIp[16] = {0};

			DWORD dwIP = pPlayerSession->GetGetIP();
			strncpy(szUserIp, inet_ntoa((*(in_addr*)&dwIP)), 16);


			int nCash = 0;
			INOUT_RET ret;

			char szName[32] ={0};
			tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(GetID());
			if (P_VALID(pRoleInfo))
			{
				strncpy(szName, IUTIL->UnicodeToAnsi(pRoleInfo->szRoleName), _countof(szName) - 1);
				szName[32-1] = 0;
			}

			bool bRet = pIPGDragonfly->OutDFCashReq( ret, pPlayerSession->GetAccount(), pPlayerSession->GetKrUserID(), szName, szUserIp, dwItemTypeID, nYuanbao, GetSex(), "", nNum);

			g_world.WriteKroeaLog(_T("Role Dec cash end. \r\n"));

			int nCurCash = GetIPGCash();
			if( nCurCash >=0 )
			{
				TCHAR tszTemp[64] = {0};
				wsprintf( tszTemp, _T("Role Dec cash and GetIPGCash. cur cash:%d. \r\n"),nCurCash);
				g_world.WriteKroeaLog(tszTemp);

				SetRoleBagYuanBao(nCurCash);

				//���͵��ͻ���
				SendBagYuanBao();
			}

			return bRet;
		}
		else
		{
			g_world.WriteKroeaLog(_T("IPGDragonfly err1. \r\n"));
			return FALSE;
		}
	}

	g_world.WriteKroeaLog(_T("IPGDragonfly err2. \r\n"));
	return FALSE;
}
//---------------------------------------------------------------------------------
int Role::GetIPGCash()
{
	g_world.WriteKroeaLog(_T("Role GetIPGCash. \r\n"));

	PlayerSession * pSession = GetSession();
	if( P_VALID(pSession))
	{
		g_world.WriteKroeaLog(_T("GetIPGCash GetSession ok. \r\n"));

		TCHAR szName[32] ={0};
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(GetID());
		if (P_VALID(pRoleInfo))
		{
			_tcscpy( szName, pRoleInfo->szRoleName );
		}

		IPGDragonfly* pIPGDragonfly = IPGDragonfly::GetInstance();
		if( NULL != pIPGDragonfly && pIPGDragonfly->IsInitialize() )
		{
			int nCash = 0;

			g_world.WriteKroeaLog(_T("GetIPGCash GetDFCashReq begin. \r\n"));

			if( pIPGDragonfly->GetDFCashReq( pSession->GetAccount(), pSession->GetKrUserID(), IUTIL->UnicodeToAnsi(szName), nCash))
			{
				TCHAR tszTemp[64] = {0};
				wsprintf(tszTemp,_T("GetIPGCash GetDFCashReq cash:%d. \r\n"),nCash);
				g_world.WriteKroeaLog(tszTemp);

				return nCash;
				//SetRoleBagYuanBao(nCash);

				////���͵��ͻ���
				//SendBagYuanBao();
			}

			g_world.WriteKroeaLog(_T("UpdateGetCash GetDFCashReq end. \r\n"));
		}
	}
	else
	{
		g_world.WriteKroeaLog(_T("role getsession err. \r\n"));
	}

	return 0;
}
//---------------------------------------------------------------------------------

BOOL Role::IsEquipTimeEquipment()
{
	//��ʱװ���ж�
	for(int i = EEP_Equip_Start; i < EEP_Equip_End; i++)
	{
		tagEquip* pEquip = GetItemMgr().GetEquipBarEquip((INT16)i);
		if(!P_VALID(pEquip))
		{
			continue;
		}
		if(pEquip->pEquipProto->dwTimeLimit != GT_INVALID)
		{
			return FALSE;
		}
	}

	return TRUE;
}

DWORD Role::AddNewKeyCodeGift(DWORD dwKeyCodeCrc)
{
	if(dwKeyCodeCrc != TObjRef<Util>()->Crc32(NewKeyCodeString))
	{
		tagNS_GetKeyCodeGift send;
		send.dwErrorCode = 1;
		SendMessage(&send, send.dwSize);

		return 0;	//Key�벻��
	}

	tagNDBC_KeyCodeAddGift send;
	send.dwRoleID = GetID();
	g_dbSession.Send(&send, send.dwSize);

	return 0;
}

BOOL Role::IsInWar()
{
	Map* pMap = GetMap();
	if (!P_VALID(pMap))
	{
		return false;
	}

	return pMap->IsWar();
}

BOOL Role::IsUseSkill()
{
	return GetCombatHandler().IsUseSkill();
}

void Role::SetWarRole(WarRole* pWarRole)
{
	m_pWarRole = pWarRole;
}

WarRole* Role::GetWarRole()
{
	if (FALSE == IsInWar())
	{
		return NULL;
	}

	return m_pWarRole;
}

// ������ҵ� ʥ��ÿ��������������
VOID Role::ResetHolyToDayEatNum()
{
	ItemMgr &itemMgr = GetItemMgr();
	// �����������ʥ��� ÿ��������������
	tagEquip * pEquip =itemMgr.GetEquipBarEquip((INT16)EEP_Holy);
	tagHolyMan * pHoly = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsHoly(pEquip) )
	{
		pHoly = (tagHolyMan*)pEquip;
	}
	if (P_VALID(pHoly) && pHoly->nToDayEatNum != 0)
	{
		pHoly->nToDayEatNum = 0;
		// ʥ�������޸�֪ͨDB�Ϳͻ���
		itemMgr.SendHolySpec2DB(*pHoly);
		itemMgr.SendHolySpec2Client(*pHoly);
	}

	// �������ң������ñ����е�ʥ��ÿ��������������
	tagHolyMan * pTempHoly = NULL;
	int nHolyNum = 0;
	int nMaxHolyInBag = MAX_HOLY_NUM; //������ʥ����������
	// tagEquip * pEquip1 =GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);	
	if( P_VALID(pHoly) )
	{
		nMaxHolyInBag--;		
	}	
	for ( int i = 0; i < itemMgr.GetBagCurSize(); i++)
	{
		tagItem* pItem = (tagItem *) itemMgr.GetBag().GetItem((INT16)i);
		if (NULL != pItem && MIsHoly(pItem))
		{
			pTempHoly = (tagHolyMan*)pItem;
			if (pTempHoly->nToDayEatNum != 0)
			{
				pTempHoly->nToDayEatNum = 0;
				// ʥ�������޸�֪ͨDB�Ϳͻ���
				itemMgr.SendHolySpec2DB(*pTempHoly);
				itemMgr.SendHolySpec2Client(*pTempHoly);

				nHolyNum++;
				if (nHolyNum == nMaxHolyInBag)
				{
					break;
				}
			}	

		}

	}
}

DWORD Role::IsWarRevive()
{
	//�ж��Ƿ�Ϊ�ػ���ͼ
	if(GetMapID() == 2962677420)
	{
		const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(GetMapID());
		if(!P_VALID(pMapInfo))
		{
			ASSERT(P_VALID(pMapInfo));
			return E_Revive_MapNotFound;
		}
		m_Revive.fX = pMapInfo->vRebornPos.x;
		m_Revive.fY = pMapInfo->vRebornPos.y;
		m_Revive.fZ = pMapInfo->vRebornPos.z;
		m_Revive.dwMapID = GetMapID();
	}
}



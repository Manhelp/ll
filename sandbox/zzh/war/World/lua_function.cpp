//--------------------------------------------------------------------------------
//!\file lua_functions.cpp
//!\author Aslan
//!
//!\date 2009-03-13
//! last 2009-03-13
//!
//!\brief Lua�ű���������
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#include "StdAfx.h"
#include "script_mgr.h"

#include "map.h"
#include "map_mgr.h"
#include "map_creator.h"
#include "map_instance.h"
#include "unit.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "skill.h"
#include "gm_policy.h"
#include "buff.h"
#include "activity_mgr.h"
#include "role_mgr.h"
#include "group_mgr.h"
#include "team.h"
#include "item_creator.h"
#include "MsgInfoMgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "pet_pocket.h"
#include "title_mgr.h"
#include "city.h"
#include "city_mgr.h"
#include "city_struggle.h"
#include "guild_member.h"
#include "quest_mgr.h"
#include "../WorldDefine/city_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/quest.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_digbao.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/msg_virgin_online.h"
#include "../WorldDefine/msg_fishing.h"
#include "../WorldDefine/msg_social.h"
#include "../WorldDefine/msg_city_struggle.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/msg_frame.h"
#include "../WorldDefine/msg_master_apprentice.h"
#include "../WorldDefine/group_define.h"
#include "../WorldDefine/msg_group.h"
#include "../WorldDefine/remote_role.h"
#include "../WorldDefine/msg_script.h"
#include "../WorldDefine/msg_func_npc.h"

#include "athletics_PvP_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "../WorldDefine/msg_train.h"
#include "../WorldDefine/quest.h"
#include "att_res.h"
#include "TreasureChest_mgr.h"
#include "world_session.h"
#include "../ServerDefine/role_data.h"
#include "GuildBattle.h"
#include "../ServerDefine/master_apprentice.h"

#include "athletics_room_mgr.h"
#include "athletics_room.h"
#include "athletics_system.h"
#include "map_instance_stable.h"
#include "athletics_pvp.h"
#include "athletics_pvp_mgr.h"

#include "../WorldDefine/msg_VoteShow_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../WorldDefine/msg_GuildBattle.h"

#include "draft_show.h"
#include "player_regression.h"
// Jason 2010-1-27 v1.3.2 ��һع�/���߽������
#include "player_account.h"

#include "pet_att.h"
#include "pet_soul.h"

#include "../WorldDefine/msg_activity.h"
#include "fbbs_session.h"
#include "family.h"
#include "family_mgr.h"
#include "../ServerDefine/data_collect.h"
#include "data_collect.h"

extern AthleticsSystem g_AthleticsSystem;

//--------------------------------------------------------------------------------
// �ű�����ע��
//--------------------------------------------------------------------------------
static int SC_RegisterCreatureEvent(lua_State* L);
static int SC_RegisterQuestEvent(lua_State* L);
static int SC_RegisterRoleEvent(lua_State* L);
static int SC_RegisterMapEvent(lua_State* L);
static int SC_RegisterActEvent(lua_State* L);
static int SC_RegisterWorldEvent(lua_State* L);
static int SC_RegisterItemEvent(lua_State* L);
static int SC_RegisterSkillEvent(lua_State* L);
static int SC_RegisterMotionEvent(lua_State* L);
static int SC_RegisterBuffEvent(lua_State* L);

//---------------------------------------------------------------------------------
// �ű�������
//---------------------------------------------------------------------------------
//static int SC_CreateScriptMutex(lua_State* L);
//static int SC_LockScriptMutex(lua_State* L);
//static int SC_UnLockScriptMutex(lua_State* L);

//---------------------------------------------------------------------------------
// ��Һ����ﹲ�еĽӿں���
//---------------------------------------------------------------------------------
static int SC_ChangeHP(lua_State* L);
static int SC_ChangeMP(lua_State* L);
static int SC_AddBuff(lua_State* L);
static int SC_CancelBuff(lua_State* L); 
static int SC_GetPosition(lua_State* L);
static int SC_IsDead(lua_State* L);
static int SC_IsPlayer(lua_State* L);
static int SC_IsCreature(lua_State* L);
static int SC_StopAttack(lua_State* L);
static int SC_StopMove(lua_State* L);
static int SC_IsAttack(lua_State* L);
static int SC_FriendEnemy(lua_State* L);
static int SC_GetUnitAttValue(lua_State* L);
static int SC_GetAttValue(lua_State* L);
static int SC_ModAttValue(lua_State* L);
static int SC_SetAttValue(lua_State* L);
static int SC_GetUnitTypeID(lua_State* L);
static int SC_IsInState(lua_State* L);
static int SC_SetState(lua_State* L);
static int SC_UnsetState(lua_State* L);
static int SC_GetCamp(lua_State* L);
static int SC_SetCamp(lua_State* L);
static int SC_SetFaceTo(lua_State* L);
static int SC_GetRoleCreateTime(lua_State* L);

//---------------------------------------------------------------------------------
// ��Ҷ��к���
//---------------------------------------------------------------------------------
static int SC_RoleAddQuest(lua_State* L);
static int SC_RoleAddQuestFromNPC(lua_State* L);
static int SC_DeleteQuest(lua_State* L);
static int SC_RoleCompleteQuest(lua_State* L);
static int SC_QuestInit(lua_State* L);
static int SC_IsRoleHaveQuest(lua_State* L);
static int SC_IsRoleHaveBoardQuest(lua_State* L);
static int SC_IsRoleCompleteQuest(lua_State* L);
static int SC_RoleGotoNewMap(lua_State* L);
static int SC_GetRoleLevel(lua_State *L);
static int SC_GetRoleAttValue(lua_State *L);
static int SC_ModRoleAttValue(lua_State *L);
static int SC_QuestAddRoleItem(lua_State* L);
static int SC_BaibaoAddRoleItem(lua_State* L);
static int SC_RoleAddSkill(lua_State* L);
static int SC_IsRoleLearnedSkill(lua_State* L);
static int SC_GetRoleName(lua_State* L);
static int SC_IsRoleHaveTeam(lua_State* L);
static int SC_GetRoleTeamMemID(lua_State* L);
static int SC_SetQuestScriptData(lua_State* L);
static int SC_ModQuestScriptData(lua_State* L);
static int SC_GetQuestScriptData(lua_State* L);
static int SC_AddRoleItem(lua_State* L);
static int SC_RoleEquip(lua_State* L);
static int SC_RemoveFromRole(lua_State* L);
static int SC_RemoveFromEquipBar(lua_State* L);
static int SC_RemoveFromWare(lua_State* L);
static int SC_GetBagFreeSize(lua_State* L);
static int SC_GetRoleItemNum(lua_State* L);
static int SC_AddRoleExp(lua_State* L);
static int SC_AddRoleSilver(lua_State* L);
static int SC_DecRoleSilver(lua_State* L);
static int SC_GetRoleSilver(lua_State* L);
static int SC_GetRoleSex(lua_State* L);
static int SC_GetRoldDressid(lua_State* L);
static int SC_SetRoleScriptData(lua_State* L);
static int SC_ModRoleScriptData(lua_State* L);
static int SC_GetRoleScriptData(lua_State* L);
static int SC_AddRoleExVolume(lua_State* L);
static int SC_IsRoleInStatus(lua_State* L);
static int SC_CanMount(lua_State* L);
static int SC_CanGather(lua_State* L);
static int SC_GetRoleLevelUpExp(lua_State* L);
static int SC_IsRoleOnLine(lua_State* L);
static int SC_GetActiveLongHunNum(lua_State* L);
static int SC_CastActiveLongHun(lua_State* L);
static int SC_ProlifeLongHun(lua_State* L);
static int SC_IsRoleOnStall(lua_State* L);
static int SC_SetRoleState(lua_State* L);
static int SC_UnSetRoleState(lua_State* L);
static int SC_CanOpenChest(lua_State* L);
static int SC_SendChestMsg(lua_State* L);
static int SC_IncTreasureSum(lua_State* L);
static int SC_GetTreasureSum(lua_State* L);
static int SC_GetChestItem(lua_State* L);
static int SC_ItemNeedBroadcast(lua_State* L);
static int SC_StopMount(lua_State* L);
static int SC_InstanceNotify(lua_State* L);
static int SC_IsRoleMarried(lua_State* L);
static int SC_IsRoleWedded(lua_State* L);
static int SC_GetFriendShipValue(lua_State* L);
static int SC_SigTitleEvent(lua_State* L);
static int SC_SigRemoveTitleEvent(lua_State* L);
static int SC_ChangeSkillExp(lua_State* L);
static int SC_IsRoleHaveBuff(lua_State* L);
static int SC_QuestSetRebornMap(lua_State* L);
static int SC_VirginOnlineSendClient(lua_State* L);
static int SC_FishingSendClient(lua_State* L);
static int SC_RoleCancelSkill(lua_State* L);
static int SC_RoleFixSpirit(lua_State* L);
static int SC_AddFriendShipValue(lua_State* L);
static int SC_IsCouple(lua_State* L);
static int SC_GetRoleYuanbao(lua_State* L);
static int SC_RoleUseSkill(lua_State* L);
static int SC_GetMasterID(lua_State* L);
static int SC_RoleBecomeMaster(lua_State* L);
static int SC_LevelUpAddJingWuPoint(lua_State* L);
static int SC_LevelUpAddBonus(lua_State* L);
static int SC_GraduateNotice(lua_State* L);
static int SC_GetGraduateRewardNum(lua_State* L);
static int SC_SetGraduateRewardNum(lua_State* L);
static int SC_GetQinWuPoint(lua_State* L);
static int SC_SetQinWuPoint(lua_State* L);
static int SC_GetLevelUpBonus(lua_State* L);
static int SC_SetLevelUpBonus(lua_State* L);
static int SC_GetKillObjExp(lua_State* L);
static int SC_EudemonSummonCountDown(lua_State* L);
static int SC_ModRankData(lua_State* L);
static int SC_GetRoleRankData(lua_State* L);
static int SC_GetDisplayRank(lua_State* L);
static int SC_CancelRoleStallState(lua_State* L);
static int SC_ModSpecialTargetValue(lua_State* L);
static int SC_SetRoleClass(lua_State* L);
static int SC_GetRoleClass(lua_State* L);
static int SC_GetRoleCompleteBoardQuestNum(lua_State* L);
//static int SC_ModSpecialTargetValue(lua_State* L);
static int SC_NotifyFBBS(lua_State* L);
static int SC_IsFairyItem(lua_State* L);
static int SC_GetRoleQuestNum(lua_State* L);
static int SC_GetWardrobeDetailInfo(lua_State* L);
                
// Jason 2010-1-19 v1.3.2 �������ʱ��
static int SC_GetPlayerOfflineTime(lua_State* L);

// Jason 2010-1-27 v1.3.2 �˺�����ʱ��
int SC_GetPlayerAccountOfflineTime(lua_State* L);
int SC_SetRecAccountRegressionRewardFlag(lua_State* L);
int SC_CanRecAccountRegressionReward(lua_State* L);
// Jason 2010-1-29 v1.3.2 �ű�����ɾ��һ������
int SC_RemoveSkill(lua_State* L);
static int SC_GetConsumptiveStrength(lua_State* L);
static int SC_SetConsumptiveStrength(lua_State* L);
static int SC_GetRoleMapID(lua_State* L);
// Jason 2010-3-16 v1.5.0
static int SC_AddRoleGiftPoints(lua_State * L);
// Jason 2010-7-28 v1.0.0
static INT SC_GetRoleYaojingLevel(lua_State * L);
static INT SC_PutDownItem(lua_State * L);
static int SC_GetBuffWrapTimes(lua_State* L);

// ������-�������ṩLUA���ýӿ�
static int SC_SetForceInArena(lua_State* L);
static int SC_LetRoleLeaveTeam(lua_State* L);
static int SC_CreatANewTeam(lua_State* L);
static int SC_LetRoleJoinTeam(lua_State* L);
static int SC_ShowAlivePeopleInArena(lua_State* L);
static int SC_ShowKillerPictureInArena(lua_State* L);
static int SC_RoleGotoArena(lua_State* L);
static int SC_ShowArenaEndFrame(lua_State* L);
static int SC_ShowArenaRoomFrame(lua_State* L);
static int SC_ForcedToRevive(lua_State* L);
static int SC_DeletRoleFromArenaTable(lua_State* L);
static int SC_GetArenaType(lua_State* L);
static int SC_GetArenaRoomLimit(lua_State* L);
static int SC_JoinArena(lua_State* L);
static int SC_ShowAlivePeopleNameInArena(lua_State* L);
static int SC_CloseArenaWaitingRoom(lua_State* L);
static int SC_GetRoleSoulCrystalLvl(lua_State* L);

// �ɾ�ϵͳ
static int SC_OpenTrain(lua_State* L);
static int SC_GetClanRepValue(lua_State* L);
static int SC_GetPetStepGrade(lua_State* L);
static int SC_GetFstBoxWpnPosyTimes(lua_State* L);
static int SC_GetFstBoxWpnEngraveTimes(lua_State* L);
static int SC_GetFstBoxWpnEquipHoleCnt(lua_State* L);
static int SC_GetGuildExploit(lua_State* L);
static int SC_GetPlayerTrainCnt(lua_State* L);
static int SC_IsRoleHaveTitle(lua_State* L);
static int SC_GodMiraclePoints(lua_State* L);
static int SC_ModifyMiraclePoints(lua_State* L);
static int SC_GetTrialCompleteNum(lua_State* L);
static int SC_SetTrialCompleteNum(lua_State* L);
static int SC_TrialComplete(lua_State* L);
static int SC_GetTrialState(lua_State* L);
static int SC_GetFriendNum(lua_State* L);
static int SC_GetEquipIDOnPos(lua_State* L);
static int SC_GetEquipLvLimit(lua_State* L);
static int SC_GetEquipQlty(lua_State* L);

// �ű�װ��ǿ��
static int SC_GetEquipStrengthLevel(lua_State* L);
static int SC_SetEquipStrengthLevel(lua_State* L);
static int SC_DigHoleOnEquip(lua_State* L);
static int SC_PutStoneOnEquip(lua_State* L);
static int SC_SetEquipAdditionalAbility(lua_State* L);
static int SC_SetGrowEquipLevel(lua_State* L);

// ���ͻ��˷��ͳ�ս����ս����Ϣ
static int SC_SendCityStruggleRecord2Role(lua_State* L);

static int SC_GetRoleGuildAtt(lua_State* L);
static int SC_GetSoarValue(lua_State* L);
static int SC_SetSoarValue(lua_State* L);
static int SC_GetGuildAtt(lua_State* L);
static int SC_SyncFlagAttack(lua_State* L);

// �ᱦ����ű��ӿ�
static int SC_SyncOpenChaseRemainTime(lua_State* L);
static int SC_ForceLootChase(lua_State* L);
static int SC_GetCurMoveState(lua_State* L);

static int SC_UnsetStateBefAdd2Map(lua_State* L);

// �������
static int SC_GetRoleFamilyID(lua_State* L);
static int SC_GetRoleLoverID(lua_State* L);
static int SC_GetFamilyMemberID(lua_State* L);
static int SC_GetFamilyLeaderID(lua_State* L);


static int SC_AddRoleAwardData(lua_State* L);
static int SC_GetRoleAwardPoint(lua_State* L);

static int SC_GetRoleSpecFlag(lua_State* L);
static int SC_SetRoleSpecFlag(lua_State* L);

// ��һع�
static int SC_GetAccountReactiveStatus(lua_State* L);
static int SC_GetAccountReactiveKey(lua_State* L);
static int SC_GetAccountReactiveScore(lua_State* L);
static int SC_PopInputReactiveKey(lua_State* L);
static int SC_GetAccountReactiveLevel(lua_State* L);
static int SC_SetAccountReactiveLevel(lua_State* L);

static int SC_GetRoleSoul(lua_State* L);
static int SC_SetRoleSoul(lua_State* L);
static int SC_GetRoleSoulPoint(lua_State* L);
static int SC_SetRoleSoulPoint(lua_State* L);

static int SC_GetSkillActiveTime(lua_State* L);
static int SC_SetSkillActiveTime(lua_State* L);

static int SC_DotaSendMsg(lua_State* L);


//---------------------------------------------------------------------------------
// ������ؽӿ�
//---------------------------------------------------------------------------------
static int SC_GetRoleGuildID(lua_State* L);						// �õ���Ұ���ID�����û�а��ɷ���GT_INVALID)
static int SC_GetGuildPos(lua_State* L);
static int SC_GetGuildFund(lua_State* L);
static int SC_GetGuildMaterial(lua_State* L);
static int SC_GetGuildPeace(lua_State* L);
static int SC_GetGuildReputation(lua_State* L);

static int SC_IsGuildInStatus(lua_State* L);
static int SC_SetGuildStatus(lua_State* L);
static int SC_UnsetGuildStatus(lua_State* L);

static int SC_ModifyGuildFund(lua_State* L);
static int SC_ModifyGuildMaterial(lua_State* L);
static int SC_ModifyGuildPeace(lua_State* L);
static int SC_ModifyGuildReputation(lua_State* L);
static int SC_ModifyContribution(lua_State* L);
static int SC_ModifyMemberExploit(lua_State* L);
static int SC_IsHaveGuild(lua_State* L);
static int SC_AddBuffToAllGuildMember(lua_State* L);
static int SC_RemoveBuffFromAllGuildMember(lua_State* L);
static int SC_UnsetStateOfAllGuildMember(lua_State* L);
static int SC_UnsetStateOfAllRoleInBattleField(lua_State* L);
static int SC_GetAllGuildMember(lua_State* L);
static int SC_InChangeForGuildMember(lua_State* L);
static int SC_GuildRoleInterGodArea(lua_State* L);
static int SC_SetRoleStartActiveTimes(lua_State* L);
static int SC_GetGodMiracleCurState(lua_State* L);
static int SC_UpdateGodMiracleMin(lua_State* L);
static int SC_UpdateGodMiracle(lua_State* L);
static int SC_SetCaptureGodGuildID(lua_State* L);
static int SC_GetCaptureGodGuildID(lua_State* L);
static int SC_SetApplyGuildID(lua_State* L);
static int SC_GetApplyGuildID(lua_State* L);
static int SC_SetGuildRoleDie(lua_State* L);
static int SC_CalCaptureContributionAndExploit(lua_State* L);
static int SC_InitGodMiracle(lua_State* L);
static int SC_SetGodCurState(lua_State* L);
static int SC_RoleLeaveArea(lua_State* L);
static int SC_SendAllCoptureGodGuildMember(lua_State* L);
static int SC_SetGodGuildPoint(lua_State* L);
static int SC_GetDefendGuildID(lua_State* L);
static int SC_GetGuildAllianceType(lua_State* L);

static int SC_SetGuildValue(lua_State* L);
static int SC_GetGuildValue(lua_State* L);

//---------------------------------------------------------------------------------
// ������к���
//---------------------------------------------------------------------------------
static int SC_GetAroundCreature(lua_State* L);
static int SC_GetAroundRole(lua_State* L);
static int SC_SetCreatureScriptData(lua_State* L);
static int SC_ModCreatureScriptData(lua_State* L);
static int SC_GetCreatureScriptData(lua_State* L);
static int SC_SetCreatureUpdateAITimer(lua_State* L);
static int SC_CreatureUseSkill(lua_State* L);
static int SC_MonsterSay(lua_State* L);
static int SC_MonsterPlayAction(lua_State* L);
static int SC_CreatureChangeAIState(lua_State* L);
static int SC_GetEnterCombatTick(lua_State* L);
static int SC_GetCreatureTypeID(lua_State* L);
static int SC_AddEnmity(lua_State* L);
static int SC_GetCreatureCurTargetID(lua_State* L);
// Jason 2010-4-13 v2.0.0
static int SC_PullCreatureToPos(lua_State* L);
static int SC_IsHaveBuff(lua_State* L);

static int SC_GetEnemyList(lua_State* L);
static int SC_SetEnemyValue(lua_State* L);
static int SC_GetCampByCreatureTid(lua_State* L);
static int SC_SetCampByCreatureTid(lua_State* L);

//---------------------------------------------------------------------------------
// �̶���ӿ�
//---------------------------------------------------------------------------------
static int SC_InitEventTime(lua_State* L);
static int SC_GetActIsStart(lua_State* L);
static int SC_AddAllRoleBuff(lua_State* L);
static int SC_SetActScriptData(lua_State* L);
static int SC_GetActScriptData(lua_State* L);
static int SC_SaveActScriptData(lua_State* L);
static int SC_GetGuildCaptureGodMiracleNum(lua_State* L);
static int SC_GetDuoBaoQiBingOpenState(lua_State* L);

//---------------------------------------------------------------------------------
// ��ͼ�ӿ�
//---------------------------------------------------------------------------------
static int SC_SetMapScriptData(lua_State* L);
static int SC_GetMapScriptData(lua_State* L);
static int SC_MapCreateCreature(lua_State* L);
static int SC_MapCreateColCreature(lua_State* L);
static int SC_MapDeleteCreature(lua_State* L);
static int SC_MapPlaySceneEffectByObjID(lua_State* L);
static int SC_MapPlaySceneEffectByID(lua_State* L);
static int SC_MapPlaySceneMusic(lua_State* L);
static int SC_AddMapRoleBuff(lua_State* L);
static int SC_CreateInstance(lua_State* L);
static int SC_DeleteInstance(lua_State* L);
static int SC_IsInArea(lua_State* L);
static int SC_PlaySceneEffect(lua_State* L);
static int SC_StopSceneEffect(lua_State* L);
static int SC_OpenCloseDoor(lua_State* L);
static int SC_OpenCloseAllDoors(lua_State* L);
static int SC_GetDoorPos(lua_State* L);
static int SC_OpenCloseDoorByPos(lua_State* L);
static int SC_GetMapAllRoleID(lua_State* L);
static int SC_IsCreateMap(lua_State* L);
static int SC_PutItemNearRandCreature(lua_State* L);
static int SC_RemoveGroundItem(lua_State* L);
static int SC_MoveRole2AnotherMap(lua_State* L);
static int SC_SetMonsterGenState(lua_State* L);

//---------------------------------------------------------------------------------
// ����ϵͳ�ӿ�
//---------------------------------------------------------------------------------
static int SC_GetConfirmGuild(lua_State* L);
static int SC_GetCityAppointedAtt(lua_State* L);
static int SC_ChangeCityHolder(lua_State* L);
static int SC_SetCityStruggleState(lua_State* L);
static int SC_SetReviveTime(lua_State* L);
static int SC_GetRoleForce(lua_State* L);
static int SC_EnterLeaveCityStruggle(lua_State* L);
static int SC_ChangeCityDoor(lua_State* L);
static int SC_GetCityAppointedAttByMapID(lua_State* L);
static int SC_ChangeCityDefence(lua_State* L);
static int SC_SendCityStruggleInfo(lua_State* L);
static int SC_SendOneCityStruggleInfo(lua_State* L);
static int SC_InitCSRoleData(lua_State* L);
static int SC_AddRoleDataInfo(lua_State* L);
static int SC_AddKillRoleNum(lua_State* L);
static int SC_AddRoleData(lua_State* L);
static int SC_GetRoleCSScore(lua_State* L);
static int SC_SortByScore(lua_State* L);
static int SC_GetRoleKillNum(lua_State* L);
static int SC_GetCSRankedRoleID(lua_State* L);
static int SC_SetCityStruggleResult(lua_State* L);
static int SC_SummonPersonChange(lua_State* L);
static int SC_SetSummonTime(lua_State* L);
static int SC_StopSummon(lua_State* L);

//---------------------------------------------------------------------------------
// �ű���Ϣ�ӿ�
//---------------------------------------------------------------------------------
static int SC_BeginMsgEvent(lua_State* L);
static int SC_AddMsgEvent(lua_State* L);
static int SC_DispatchRoleMsgEvent(lua_State* L);
static int SC_DispatchWorldMsgEvent(lua_State* L);
static int SC_DispatchMapMsgEvent(lua_State* L); 
static int SC_DispatchGuildMsgEvent(lua_State* L); 
static int SC_DispatchAllianceGuildMsgEvent(lua_State* L); 
static int SC_DispatchBroadcast(lua_State* L);// ����ָ��ID�ķ������㲥
static int SC_SendWorldSwitchMsg(lua_State* L);
static int SC_SendRoleSwitchMsg(lua_State* L);

//---------------------------------------------------------------------------------
// ְҵϵͳ��ؽӿ�
//---------------------------------------------------------------------------------
static int SC_IsRoleInaugural(lua_State* L);
static int SC_ChangeClass(lua_State* L); 

//---------------------------------------------------------------------------------
// ϵͳ��ؽӿ�
//---------------------------------------------------------------------------------
static int SC_TimeGetTime(lua_State* L);
static int SC_DataTime2DWORDTime(lua_State* L);
static int SC_GetCurrentDWORDTime(lua_State* L);
static int SC_CalcTimeDiff(lua_State* L);
static int SC_IncreaseTime(lua_State* L);
static int SC_DecreaseTime(lua_State* L);
static int SC_GetGMExpRate(lua_State* L);
static int SC_GetWorldTick(lua_State* L);
static int SC_CalUnitDist(lua_State* L);
static int SC_IsOldSerNewPlayer(lua_State* L);
static int SC_MultiRateReward(lua_State* L);
static int SC_GetWorldDataTable(lua_State* L);
static int SC_SetWorldDataTable(lua_State* L);
static int SC_GetRoleDataTable(lua_State* L);
static int SC_SetRoleDataTable(lua_State* L);

//---------------------------------------------------------------------------------
// �ű�log��¼
//---------------------------------------------------------------------------------
static int SC_WriteLog(lua_State* L);
static int SC_Trace(lua_State * L);

//--------------------------------------------------------------------------------
// 64λ���ݴ���
//--------------------------------------------------------------------------------
static void		push_64bit_data(lua_State* L, INT64 n64Data);
static INT64	pop_64bit_data(lua_State* L, int nHigh, int nLow);

//---------------------------------------------------------------------------------
// ���С�ӽӿ�
//---------------------------------------------------------------------------------
static int SC_GetTeamLeaderID(lua_State* L);
static int SC_CreateTeam(lua_State* L);
static int SC_AddTeamMember(lua_State* L);

//---------------------------------------------------------------------------------
// �������ڽ���
//---------------------------------------------------------------------------------
static int SC_OpenFramePage(lua_State* L);

//Ԫ�����
static int SC_GetSoulMaxLevelExp(lua_State* L);
static int SC_AddHolySoulExp(lua_State* L);
static int SC_GetSoulCurLevel(lua_State* L);

static int SC_GetHolySoulRebornLevel(lua_State* L);

static int SC_GetCurCovalue(lua_State* L);
static int SC_AddCurCovalue(lua_State* L);
static int SC_GetCurSoulValue(lua_State* L);
static int SC_AddCurSoulValue(lua_State* L);
static int SC_GetMaxSoulValue(lua_State* L);
static int SC_GetSoulCurExp(lua_State* L);
static int SC_ReloadHolySoulAtt(lua_State* L);
static int SC_SetHunJingStrength(lua_State* L);
static int SC_SetWarGold(lua_State* L);
static int SC_GetWarGold(lua_State* L);


//��ְ
static int SC_GetRoleClergy(lua_State* L);

//��ʯǿ����ˢ�ɳ�id
static int SC_GetEquipGrowAttID(lua_State* L);	
static int SC_SetEquipGrowAttID(lua_State* L);	
static int SC_GetEquipStoneAddLevel(lua_State* L);
static int SC_SetEquipStoneAddLevel(lua_State* L);
//�̳ǻ���
static int SC_AddMallMark(lua_State* L);


//---------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------
static const luaL_Reg auxLib[] =
{
	{"RegisterCreatureEvent",			SC_RegisterCreatureEvent},
	{"RegisterQuestEvent",				SC_RegisterQuestEvent},
	{"RegisterRoleEvent",				SC_RegisterRoleEvent},
	{"RegisterMapEvent",				SC_RegisterMapEvent},
	{"RegisterActEvent",				SC_RegisterActEvent},
	{"RegisterWorldEvent",				SC_RegisterWorldEvent},
	{"RegisterItemEvent",				SC_RegisterItemEvent},
	{"RegisterSkillEvent",				SC_RegisterSkillEvent},
	{"RegisterMotionEvent",             SC_RegisterMotionEvent},
	{"RegisterBuffEvent",				SC_RegisterBuffEvent},
	//{"CreateScriptMutex",				SC_CreateScriptMutex},
	//{"LockScriptMutex",					SC_LockScriptMutex},
	//{"UnLockScriptMutex",				SC_UnLockScriptMutex},
	{"TimeGetTime",						SC_TimeGetTime},
	{"GetWorldTick",					SC_GetWorldTick},
	{"WriteLog",						SC_WriteLog},
	{"DataTime2DWORDTime",				SC_DataTime2DWORDTime},
	{"GetCurrentDWORDTime",				SC_GetCurrentDWORDTime},
	{"CalcTimeDiff",					SC_CalcTimeDiff},
	{"IncreaseTime",					SC_IncreaseTime},
	{"DecreaseTime",					SC_DecreaseTime},
	{"GetGMExpRate",					SC_GetGMExpRate},
	{"Trace",							SC_Trace},
	{"CalUnitDist",						SC_CalUnitDist},
	{"IsOldSerNewPlayer",		SC_IsOldSerNewPlayer},
	{"MultiRateReward",		SC_MultiRateReward},
	{"GetWorldDataTable",	SC_GetWorldDataTable},
	{"SetWorldDataTable",		SC_SetWorldDataTable},
	{"GetRoleDataTable",		SC_GetRoleDataTable},
	{"SetRoleDataTable",		SC_SetRoleDataTable},

	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// ��Һ����ﹲ�нӿڰ�
//----------------------------------------------------------------------------------
static const luaL_Reg unitLib[] = 
{
	{"ChangeHP",						SC_ChangeHP},
	{"ChangeMP",						SC_ChangeMP},
	{"AddBuff",							SC_AddBuff},
	{"CancelBuff",						SC_CancelBuff},
	{"GetPosition",						SC_GetPosition},
	{"IsDead",							SC_IsDead},
	{"IsPlayer",						SC_IsPlayer},
	{"IsCreature",						SC_IsCreature},
	{"StopMove",						SC_StopMove},
	{"StopAttack",						SC_StopAttack},
	{"IsAttack",						SC_IsAttack},
	{"FriendEnemy",						SC_FriendEnemy},
	{"GetUnitAttValue",					SC_GetUnitAttValue},
	{"GetAttValue",						SC_GetAttValue},
	{"ModAttValue",						SC_ModAttValue},
	{"SetAttValue",						SC_SetAttValue},
	{"GetUnitTypeID",					SC_GetUnitTypeID},
	{"IsInState",						SC_IsInState},
	{"SetState",						SC_SetState},
	{"UnsetState",						SC_UnsetState},
	{"GetCamp",							SC_GetCamp},
	{"SetCamp",							SC_SetCamp},
	{"SetFaceTo",						SC_SetFaceTo},
	
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// �̶���ӿ�
//----------------------------------------------------------------------------------
static const luaL_Reg actLib[] =
{
	{"InitEventTime",					SC_InitEventTime},
	{"GetActIsStart",					SC_GetActIsStart},
	{"AddAllRoleBuff",					SC_AddAllRoleBuff},
	{"SetActScriptData",				SC_SetActScriptData},
	{"GetActScriptData",				SC_GetActScriptData},
	{"SaveActScriptData",				SC_SaveActScriptData},
	{"GetGuildCaptureGodMiracleNum",	SC_GetGuildCaptureGodMiracleNum},
	{"GetDuoBaoQiBingOpenState",	SC_GetDuoBaoQiBingOpenState},
	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// ��ͼ�¼��ӿ�
//----------------------------------------------------------------------------------
static const luaL_Reg mapLib[] = 
{
	{"SetMapScriptData",				SC_SetMapScriptData},
	{"GetMapScriptData",				SC_GetMapScriptData},
	{"MapCreateCreature",				SC_MapCreateCreature},
	{"MapCreateColCreature",			SC_MapCreateColCreature},
	{"MapDeleteCreature",				SC_MapDeleteCreature},
	{"CreateInstance",					SC_CreateInstance},
	{"DeleteInstance",					SC_DeleteInstance},
	{"AddMapRoleBuff",					SC_AddMapRoleBuff},
	{"IsInArea",						SC_IsInArea},
//	{"MapPlaySceneEffectByObjID",		SC_MapPlaySceneEffectByObjID},
//	{"MapPlaySceneEffectByID",			SC_MapPlaySceneEffectByID},
//	{"MapPlaySceneMusic",				SC_MapPlaySceneMusic},
	{"PlaySceneEffect",					SC_PlaySceneEffect},
	{"StopSceneEffect",					SC_StopSceneEffect},
	{"OpenCloseDoor",					SC_OpenCloseDoor},
	{"OpenCloseAllDoors",				SC_OpenCloseAllDoors},
	{"GetDoorPos",						SC_GetDoorPos},
	{"OpenCloseDoorByPos",				SC_OpenCloseDoorByPos},
	{"GetMapAllRoleID",					SC_GetMapAllRoleID},
	{"IsCreateMap",						SC_IsCreateMap},
	{"PutItemNearRandCreature",			SC_PutItemNearRandCreature},
	{"RemoveGroundItem",				SC_RemoveGroundItem},
	{"MoveRole2AnotherMap",				SC_MoveRole2AnotherMap},
	{"SetMonsterGenState",				SC_SetMonsterGenState},

	{NULL, NULL},
};

//----------------------------------------------------------------------------------
// ����¼��ӿ�
//----------------------------------------------------------------------------------
static const luaL_Reg roleLib[] = 
{
	{"RoleGotoNewMap",					SC_RoleGotoNewMap},
	{"GetRoleLevel",					SC_GetRoleLevel},
	{"GetRoleAttValue",					SC_GetRoleAttValue},
	{"ModRoleAttValue",					SC_ModRoleAttValue},
	{"AddQuest",						SC_RoleAddQuest},
	{"AddQuestFromNPC",						SC_RoleAddQuestFromNPC},
	{"DeleteQuest",						SC_DeleteQuest},
	{"RoleCompleteQuest",				SC_RoleCompleteQuest},
	{"QuestInit",						SC_QuestInit},
	{"QuestAddRoleItem",				SC_QuestAddRoleItem},
	{"BaibaoAddRoleItem",				SC_BaibaoAddRoleItem},
	{"AddSkill",						SC_RoleAddSkill},
	{"IsLearnedSkill",					SC_IsRoleLearnedSkill},
	{"GetRoleName",						SC_GetRoleName},
	{"IsRoleHaveTeam",					SC_IsRoleHaveTeam},
	{"GetRoleTeamMemID",				SC_GetRoleTeamMemID},
	{"SetQuestScriptData",				SC_SetQuestScriptData},
	{"GetQuestScriptData",				SC_GetQuestScriptData},
	{"ModQuestScriptData",				SC_ModQuestScriptData},
	{"IsRoleHaveQuest",					SC_IsRoleHaveQuest},
	{"IsRoleHaveBoardQuest",			SC_IsRoleHaveBoardQuest},
	{"IsRoleCompleteQuest",				SC_IsRoleCompleteQuest},
	{"AddRoleItem",						SC_AddRoleItem},
	{"RoleEquip",						SC_RoleEquip},
	{"RemoveFromRole",					SC_RemoveFromRole},
	{"RemoveFromEquipBar",				SC_RemoveFromEquipBar},
	{"RemoveFromWare",					SC_RemoveFromWare},
	{"GetBagFreeSize",					SC_GetBagFreeSize},
	{"GetRoleItemNum",					SC_GetRoleItemNum},
	{"AddRoleExp",						SC_AddRoleExp},
	{"AddRoleSilver",					SC_AddRoleSilver},
	{"DecRoleSilver",					SC_DecRoleSilver},
	{"GetRoleSilver",					SC_GetRoleSilver},
	{"GetRoleSex",						SC_GetRoleSex},
	{"GetRoleDressid",					SC_GetRoldDressid},
	{"SetRoleScriptData",				SC_SetRoleScriptData},
	{"ModRoleScriptData",				SC_ModRoleScriptData},
	{"GetRoleScriptData",				SC_GetRoleScriptData},
	{"AddRoleExVolume",					SC_AddRoleExVolume},
	{"IsRoleInStatus",					SC_IsRoleInStatus},
	{"CanMount",						SC_CanMount},
	{"CanGather",						SC_CanGather},
	{"GetRoleLevelUpExp",				SC_GetRoleLevelUpExp},
	{"IsRoleOnLine",					SC_IsRoleOnLine},
	{"GetLongHunID",					SC_GetActiveLongHunNum},
	{"CastLongHun",						SC_CastActiveLongHun},
	{"ProlifeLongHun",					SC_ProlifeLongHun},
	{"IsRoleOnStall",					SC_IsRoleOnStall},	
	{"CanOpenChest",					SC_CanOpenChest},		
	{"SendChestMsg",					SC_SendChestMsg},		
	{"IncTreasureSum",					SC_IncTreasureSum},		
	{"GetTreasureSum",					SC_GetTreasureSum},		
	{"GetChestItem",					SC_GetChestItem},
	{"ItemNeedBroadcast",				SC_ItemNeedBroadcast},
	{"SetRoleState",					SC_SetRoleState},
	{"UnSetRoleState",					SC_UnSetRoleState},
	{"StopMount",						SC_StopMount},
	{"InstanceNotify",					SC_InstanceNotify},
	{"IsRoleMarried",					SC_IsRoleMarried},
	{"IsRoleWedded",					SC_IsRoleWedded},
	{"GetFriendShipValue",				SC_GetFriendShipValue},
	{"SigTitleEvent",					SC_SigTitleEvent},
	{"SigRemoveTitleEvent",				SC_SigRemoveTitleEvent},
	{"ChangeSkillExp",					SC_ChangeSkillExp},
	{"IsRoleHaveBuff",					SC_IsRoleHaveBuff},
	{"QuestSetRebornMap",				SC_QuestSetRebornMap},
	{"VirginOnlineSendClient",			SC_VirginOnlineSendClient},
	{"FishingSendClient",				SC_FishingSendClient},
	{"IsCouple",						SC_IsCouple},
	{"GetRoleYuanbao",					SC_GetRoleYuanbao},
	{"RoleCancelSkill",					SC_RoleCancelSkill},
	{"RoleUseSkill",					SC_RoleUseSkill},
	{"GetMasterID",						SC_GetMasterID},
	{"RoleBecomeMaster",				SC_RoleBecomeMaster},
	{"LevelUpAddJingWuPoint",			SC_LevelUpAddJingWuPoint},
	{"LevelUpAddBonus",					SC_LevelUpAddBonus},
	{"GraduateNotice",					SC_GraduateNotice},
	{"GetGraduateRewardNum",			SC_GetGraduateRewardNum},
	{"SetGraduateRewardNum",			SC_SetGraduateRewardNum},
	{"GetQinWuPoint",					SC_GetQinWuPoint},
	{"SetQinWuPoint",					SC_SetQinWuPoint},
	{"GetLevelUpBonus",					SC_GetLevelUpBonus},
	{"SetLevelUpBonus",					SC_SetLevelUpBonus},
	{"GetKillObjExp",					SC_GetKillObjExp},
	{"EudemonSummonCountDown",			SC_EudemonSummonCountDown},
	{"ModRankData",			            SC_ModRankData},
	{"GetRoleRankData",			        SC_GetRoleRankData},
	{"GetDisplayRank",			        SC_GetDisplayRank},
	{"CancelRoleStallState",			SC_CancelRoleStallState},
	{"GetWardrobeDetailInfo",			SC_GetWardrobeDetailInfo},
	         
	// Jason 2010-1-19 v1.3.2
	{"GetPlayerOfflineTime",			SC_GetPlayerOfflineTime},
	{"RoleFixSpirit",					SC_RoleFixSpirit},	//  ���þ���ֵcdʱ��
	// Jason 2010-1-27 v.1.3.2 ���߽���/�ع�
	{"GetPlayerAccountOfflineTime" ,	SC_GetPlayerAccountOfflineTime},
	{"CanRecAccountRegressionReward" ,	SC_CanRecAccountRegressionReward},
	{"SetRecAccountRegressionRewardFlag" ,	SC_SetRecAccountRegressionRewardFlag},
	// Jason 2010-1-29 v1.3.2 ɾ�����һ������
	{"RemoveSkill"					,	SC_RemoveSkill},

	{"OpenFramePage",					SC_OpenFramePage},
	{"GetConsumptiveStrength",			SC_GetConsumptiveStrength},								
	{"SetConsumptiveStrength",			SC_SetConsumptiveStrength},
	{"GetRoleMapID",					SC_GetRoleMapID},
	// Jason 2010-3-16 v1.5.0
	{"AddRoleGiftPoints",				SC_AddRoleGiftPoints},

	// ������-�������ṩLUA���ýӿ�
	{"SetForceInArena",					SC_SetForceInArena},
	{"LetRoleLeaveTeam",				SC_LetRoleLeaveTeam},
	{"CreatANewTeam",					SC_CreatANewTeam},
	{"LetRoleJoinTeam",					SC_LetRoleJoinTeam},
	{"ShowAlivePeopleInArena",			SC_ShowAlivePeopleInArena},
	{"ShowKillerPictureInArena",		SC_ShowKillerPictureInArena},
	{"ShowArenaEndFrame",				SC_ShowArenaEndFrame},
	{"ShowArenaRoomFrame",				SC_ShowArenaRoomFrame},
	{"RoleGotoArena",					SC_RoleGotoArena},
	{"DeletRoleFromArenaTable",			SC_DeletRoleFromArenaTable},
	{"ForcedToRevive",					SC_ForcedToRevive},
	{"GetArenaType",					SC_GetArenaType},
	{"GetArenaRoomLimit",               SC_GetArenaRoomLimit},
	{"JoinArena",                       SC_JoinArena},
	{"ShowAlivePeopleNameInArena",      SC_ShowAlivePeopleNameInArena},
	{"CloseArenaWaitingRoom",           SC_CloseArenaWaitingRoom},
	{"GetRoleSoulCrystalLvl",           SC_GetRoleSoulCrystalLvl},
	 
	// ��֮����ϵͳ-�������ṩLUA���ýӿ�
	{"OpenTrain",								SC_OpenTrain},
	{"GetClanRepValue",					SC_GetClanRepValue},
	{"GetPetStepGrade",					SC_GetPetStepGrade},
	{"GetFstBoxWpnPosyTimes",			SC_GetFstBoxWpnPosyTimes},
	{"GetFstBoxWpnEngraveTimes",		SC_GetFstBoxWpnEngraveTimes},
	{"GetFstBoxWpnEquipHoleCnt",		SC_GetFstBoxWpnEquipHoleCnt},
	{"GetGuildExploit",					SC_GetGuildExploit},
	{"GetPlayerTrainCnt",				SC_GetPlayerTrainCnt},
	{"IsRoleHaveTitle",					SC_IsRoleHaveTitle},
	{"GodMiraclePoints",				SC_GodMiraclePoints},
	{"ModifyMiraclePoints",				SC_ModifyMiraclePoints},
	{"GetTrialCompleteNum",			SC_GetTrialCompleteNum},
	{"SetTrialCompleteNum",			SC_SetTrialCompleteNum},
	{"TrialComplete",						SC_TrialComplete},
	{"GetTrialState",						SC_GetTrialState},
	{"GetFriendNum",					SC_GetFriendNum},
	{"GetEquipIDOnPos",				SC_GetEquipIDOnPos},
	{"GetEquipLvLimit",					SC_GetEquipLvLimit},
	{"GetEquipQlty",						SC_GetEquipQlty},

	// ���ͻ��˷��ͳ�ս����ս����Ϣ
	{"SendCityStruggleRecord2Role",		SC_SendCityStruggleRecord2Role},
	{"GetSoarValue",					SC_GetSoarValue},
	{"SetSoarValue",					SC_SetSoarValue},
	{"GetGuildAtt",						SC_GetRoleGuildAtt},

	{"ModSpecialTargetValue",			SC_ModSpecialTargetValue},
	{"SetRoleClass",					SC_SetRoleClass},
	{"GetRoleClass",					SC_GetRoleClass},
	{"GetRoleCompleteBoardQuestNum",	SC_GetRoleCompleteBoardQuestNum},
	{"GetYaojingLevel",					SC_GetRoleYaojingLevel},
	{"PutDownItem",						SC_PutDownItem},

	{"SyncOpenChaseRemainTime",			SC_SyncOpenChaseRemainTime},
	{"ForceLootChase",					SC_ForceLootChase},
	{"GetCurMoveState",				SC_GetCurMoveState},

	{"UnsetStateBefAdd2Map",		SC_UnsetStateBefAdd2Map},
	{"GetBuffLayer",				SC_GetBuffWrapTimes},
	{"NotifyFBBS",					SC_NotifyFBBS},
	{"IsFairyItem",					SC_IsFairyItem},
	{"GetRoleQuestNum",				SC_GetRoleQuestNum},
	{"GetRoleClergy",				SC_GetRoleClergy},

	
	{"GetRoleFamilyID", SC_GetRoleFamilyID},
	{"GetRoleLoverID",	SC_GetRoleLoverID},
	{"AddRoleAwardData", SC_AddRoleAwardData},
	{"GetRoleAwardPoint", SC_GetRoleAwardPoint},
	
	{"GetRoleSpecFlag",		SC_GetRoleSpecFlag},
	{"SetRoleSpecFlag",		SC_SetRoleSpecFlag},

	{"GetAccountReactiveStatus",		SC_GetAccountReactiveStatus},
	{"GetAccountReactiveKey",			SC_GetAccountReactiveKey},
	{"GetAccountReactiveScore",		SC_GetAccountReactiveScore},
	{"PopInputReactiveKey",				SC_PopInputReactiveKey},
	{"GetAccountReactiveLevel",		SC_GetAccountReactiveLevel},
	{"SetAccountReactiveLevel",		SC_SetAccountReactiveLevel},

	{"GetRoleSoul",			SC_GetRoleSoul},
	{"SetRoleSoul",			SC_SetRoleSoul},
	{"GetRoleSoulPoint",	SC_GetRoleSoulPoint},
	{"SetRoleSoulPoint",	SC_SetRoleSoulPoint},

	{"GetSkillActiveTime",	SC_GetSkillActiveTime},
	{"SetSkillActiveTime",	SC_SetSkillActiveTime},

	//��ѡ��֮ս��ӽ��
	{"DotaSendMsg",			SC_DotaSendMsg},
	{"GetEquipStrengthLevel",	SC_GetEquipStrengthLevel},
	{"SetEquipStrengthLevel",	SC_SetEquipStrengthLevel},
	{"DigHoleOnEquip",	SC_DigHoleOnEquip},
	{"PutStoneOnEquip",	SC_PutStoneOnEquip},
	{"SetEquipAdditionalAbility",	SC_SetEquipAdditionalAbility},
	{"SetGrowEquipLevel",	SC_SetGrowEquipLevel},
	//ˢ�ɳ����ԣ���ʯǿ����ؽӿ�
	{"GetEquipGrowAttID",		SC_GetEquipGrowAttID},		//��ȡװ���ɳ�id
	{"SetEquipGrowAttID",		SC_SetEquipGrowAttID},		//����װ���ɳ�id
	{"GetEquipStoneAddLevel",	SC_GetEquipStoneAddLevel},	//��ȡ��ʯǿ���ȼ�
	{"SetEquipStoneAddLevel",	SC_SetEquipStoneAddLevel},	//���ñ�ʯǿ���ȼ�
	//�̳ǻ���
	{"AddMallMark",				SC_AddMallMark},			//�����̳ǻ���
	//��ɫ����ʱ��
	{"GetRoleCreateTime",		SC_GetRoleCreateTime},
	//Ԫ�����
	{"GetSoulMaxLevelExp",				SC_GetSoulMaxLevelExp},
	{"AddHolySoulExp",					SC_AddHolySoulExp},
	{"GetSoulCurLevel",					SC_GetSoulCurLevel},	
	{"GetHolySoulRebornLevel",			SC_GetHolySoulRebornLevel},
	{"GetSoulCurExp",					SC_GetSoulCurExp},
	{"GetCurCovalue",					SC_GetCurCovalue},
	{"AddCurCovalue",					SC_AddCurCovalue},
	{"GetCurSoulValue",					SC_GetCurSoulValue},
	{"AddCurSoulValue",					SC_AddCurSoulValue},
	{"GetMaxSoulValue",					SC_GetMaxSoulValue},
	{"ReloadHolySoulAtt",				SC_ReloadHolySoulAtt},

	{"SetHunJingStrength",				SC_SetHunJingStrength},
	{"SetWarGold",						SC_SetWarGold},
	{"GetWarGold",						SC_GetWarGold},

	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// ���庯���ӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg familyLib[] = 
{
	{"GetFamilyAllMemberID",	SC_GetFamilyMemberID},
	{"GetFamilyLeaderID",	SC_GetFamilyLeaderID},
};

//--------------------------------------------------------------------------------------
// ��Ϣ�¼��ӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg msgLib[] = 
{
	{"BeginMsgEvent",					SC_BeginMsgEvent},
	{"AddMsgEvent",						SC_AddMsgEvent},
	{"DispatchRoleMsgEvent",			SC_DispatchRoleMsgEvent},
	{"DispatchWorldMsgEvent",			SC_DispatchWorldMsgEvent},
	{"DispatchMapMsgEvent",				SC_DispatchMapMsgEvent},
	{"DispatchGuildMsgEvent",			SC_DispatchGuildMsgEvent},
	{"DispatchAllianceGuildMsgEvent",	SC_DispatchAllianceGuildMsgEvent},
	{"DispatchBroadcast",				SC_DispatchBroadcast},// ����ָ��ID�ķ������㲥
	{"SendWorldSwitchMsg",				SC_SendWorldSwitchMsg},
	{"SendRoleSwitchMsg",				SC_SendRoleSwitchMsg},
	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// �����¼��ӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg creLib[] = 
{
	{"GetAroundCreature",				SC_GetAroundCreature},
	{"GetAroundRole",					SC_GetAroundRole},
	{"GetCreatureScriptData",			SC_GetCreatureScriptData},
	{"SetCreatureScriptData",			SC_SetCreatureScriptData},
	{"ModCreatureScriptData",			SC_ModCreatureScriptData},
	{"SetCreatureAITimer",				SC_SetCreatureUpdateAITimer},
	{"CreatureUseSkill",				SC_CreatureUseSkill},
	{"MonsterSay",						SC_MonsterSay},
	{"MonsterPlayerAction",				SC_MonsterPlayAction},
	{"CreatureChangeAIState",			SC_CreatureChangeAIState},
	{"GetEnterCombatTick",				SC_GetEnterCombatTick},
	{"GetCreatureTypeID",				SC_GetCreatureTypeID},
	{"AddEnmity",						SC_AddEnmity},
	{"GetCreatureCurTargetID",			SC_GetCreatureCurTargetID},
	// Jason 2010-4-13 v2.0.0
	{"PullCreatureToPos",				SC_PullCreatureToPos},
	{"IsHaveBuff",						SC_IsHaveBuff},
	// lib 2012��1��10�� ��ӹ������б���ؽӿ�
	{"GetEnemyList",					SC_GetEnemyList},//��ȡ����б�
	{"SetEnemyValue",					SC_SetEnemyValue},	 //���ó���б��еڼ����ĳ��Ϊ���ٶ���
	{"GetCampByCreatureTid",			SC_GetCampByCreatureTid},
	{"SetCampByCreatureTid",			SC_SetCampByCreatureTid},

	{NULL,	NULL},
};

static const luaL_Reg teamLib[] = 
{
	{"CreateTeam",						SC_CreateTeam},
	{"AddTeamMember",					SC_AddTeamMember},
	{"GetTeamLeaderID",					SC_GetTeamLeaderID},
	{NULL, NULL},
};

//--------------------------------------------------------------------------------------
// ���ɽӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg guildLib[] = 
{
	{"GetRoleGuildID",					SC_GetRoleGuildID},
	{"GetRoleGuildPos",					SC_GetGuildPos},
	{"GetGuildFund",					SC_GetGuildFund},
	{"GetGuildMaterial",				SC_GetGuildMaterial},
	{"GetGuildPeace",					SC_GetGuildPeace},
	{"GetGuildReputation",				SC_GetGuildReputation},
	{"ModifyRoleContribute",			SC_ModifyContribution},
	{"ModifyGuildFund",					SC_ModifyGuildFund},
	{"ModifyGuildMaterial",				SC_ModifyGuildMaterial},
	{"ModifyGuildPeace",				SC_ModifyGuildPeace},
	{"ModifyGuildReputation",			SC_ModifyGuildReputation},
	{"IsGuildInStatus",					SC_IsGuildInStatus},
	{"SetGuildStatus",					SC_SetGuildStatus},
	{"UnsetGuildStatus",				SC_UnsetGuildStatus},
	{"ModifyMemberExploit",				SC_ModifyMemberExploit},
	{"IsHaveGuild",						SC_IsHaveGuild},
	{"AddBuffToAllGuildMember",			SC_AddBuffToAllGuildMember},
	{"RemoveBuffFromAllGuildMember",	SC_RemoveBuffFromAllGuildMember},
	{"UnsetStateOfAllGuildMember",		SC_UnsetStateOfAllGuildMember},
	{"UnsetStateOfAllRoleInBattleField",SC_UnsetStateOfAllRoleInBattleField},
	{"GetAllGuildMember",				SC_GetAllGuildMember},
	{"IsChangeForGuildMember",			SC_InChangeForGuildMember},
	{"GuildRoleEnterGodArea",			SC_GuildRoleInterGodArea},
	{"SetRoleStartActiveTimes",			SC_SetRoleStartActiveTimes},
	{"GetGodMiracleCurState",			SC_GetGodMiracleCurState},
	{"UpdateGodMiracleMin",				SC_UpdateGodMiracleMin},
	{"UpdateGodMiracle",				SC_UpdateGodMiracle},
	{"SetCaptureGodGuildID",			SC_SetCaptureGodGuildID},
	{"GetCaptureGodGuildID",			SC_GetCaptureGodGuildID},
	{"SetApplyGuildID",					SC_SetApplyGuildID},
	{"GetApplyGuildID",					SC_GetApplyGuildID},
	{"SetGuildRoleDie",					SC_SetGuildRoleDie},
	{"CalCaptureContributionAndExploit",SC_CalCaptureContributionAndExploit},
	{"InitGodMiracle",					SC_InitGodMiracle},
	{"SetGodCurState",					SC_SetGodCurState},
	{"RoleLeaveArea",					SC_RoleLeaveArea},
	{"SendAllCoptureGodGuildMember",	SC_SendAllCoptureGodGuildMember},
	{"SetGodGuildPoint",				SC_SetGodGuildPoint},
	{"GetDefendGuildID",				SC_GetDefendGuildID},
	{"GetGuildAllianceType",			SC_GetGuildAllianceType},

	{"GetGuildAtt",						SC_GetGuildAtt},

	{"SyncFlagAttack",					SC_SyncFlagAttack},
	//����Ҫ��������ֵ�Ķ�д
	{"SetGuildValue",					SC_SetGuildValue},
	{"GetGuildValue",					SC_GetGuildValue},

	{NULL,	NULL},
};

//--------------------------------------------------------------------------------------
// ����ϵͳ�ӿ�
//--------------------------------------------------------------------------------------
static const luaL_Reg cityLib[] = 
{
	{"GetConfirmGuild",					SC_GetConfirmGuild},
	{"GetCityAppointedAtt",				SC_GetCityAppointedAtt},
	{"ChangeCityHolder",				SC_ChangeCityHolder},
	{"SetCityStruggleState",			SC_SetCityStruggleState},
	{"SetReviveTime",					SC_SetReviveTime},
	{"GetRoleForce",					SC_GetRoleForce},
	{"EnterLeaveCityStruggle",			SC_EnterLeaveCityStruggle},
	{"ChangeCityDoor",					SC_ChangeCityDoor},
	{"GetCityAppointedAttByMapID",		SC_GetCityAppointedAttByMapID},
	{"ChangeCityDefence",				SC_ChangeCityDefence},
	{"SendCityStruggleInfo",			SC_SendCityStruggleInfo},
	{"SendOneCityStruggleInfo",			SC_SendOneCityStruggleInfo},
	{"InitCSRoleData",					SC_InitCSRoleData},
	{"AddRoleDataInfo",					SC_AddRoleDataInfo},
	{"AddKillRoleNum",					SC_AddKillRoleNum},
	{"AddRoleData",						SC_AddRoleData},
	{"GetRoleCSScore",					SC_GetRoleCSScore},
	{"SortByScore",						SC_SortByScore},
	{"GetRoleKillNum",					SC_GetRoleKillNum},
	{"GetCSRankedRoleID",				SC_GetCSRankedRoleID},
	{"SetCityStruggleResult",			SC_SetCityStruggleResult},
	{"SummonPersonChange",				SC_SummonPersonChange},
	{"SetSummonTime",					SC_SetSummonTime},
	{"StopSummon",						SC_StopSummon},
	{NULL,	NULL},
};

//----------------------------------------------------------------------------------
// ע��C�����⺯��
//----------------------------------------------------------------------------------
VOID LuaOpenCommLibs(lua_State*  L)
{
	luaL_openlib(L, "aux",  auxLib,  0);
	luaL_openlib(L, "unit", unitLib, 0);
	luaL_openlib(L, "role", roleLib, 0);
	luaL_openlib(L, "act",  actLib,  0);
	luaL_openlib(L,	"map",  mapLib,  0);
	luaL_openlib(L, "msg",  msgLib,  0);
	luaL_openlib(L, "cre",  creLib,  0);
	luaL_openlib(L, "team", teamLib, 0);
	luaL_openlib(L, "guild", guildLib, 0);
	luaL_openlib(L, "city",	cityLib, 0);
	luaL_openlib(L, "family",	familyLib, 0);
}

//--------------------------------------------------------------------------------
// 64λ���ݴ���
//--------------------------------------------------------------------------------
void push_64bit_data(lua_State* L, INT64 n64Data)
{
	const INT32 nMask	= 0xFFFFFFFF;

	DWORD n32High	= (DWORD)((n64Data >> 32) & nMask);
	DWORD n32Low	= (DWORD)(n64Data & nMask);

	lua_pushnumber(L, n32High);
	lua_pushnumber(L, n32Low);
}

INT64 pop_64bit_data(lua_State* L, int nHigh, int nLow)
{
	const INT64 n64Mask	= 0x00000000FFFFFFFF;

	DWORD n32High	= (DWORD)lua_tonumber(L, nHigh);
	DWORD n32Low	= (DWORD)lua_tonumber(L, nLow);

	INT64 n64Data	= n32High;
	n64Data			= (n64Data << 32) | (n32Low & n64Mask);

	return n64Data;
}

//-----------------------------------------------------------------------------------
// �ⲿ��ע�ắ��
//-----------------------------------------------------------------------------------
int SC_RegisterCreatureEvent(lua_State* L)
{
	DWORD dwTypeID = (DWORD)luaL_checknumber(L, 1);			// �����ID
	INT nEventType = luaL_checkint(L, 2);				// ������¼�����
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !GT_VALID(dwTypeID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterCreatureEvent(dwTypeID, (EScriptCreatureEvent)nEventType, szFunction);
	return 0;
}

int SC_RegisterQuestEvent(lua_State* L)
{
	UINT16 u16QuestID = (UINT16)luaL_checknumber(L, 1);	// ����ID
	INT nEventType = luaL_checkint(L, 2);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !P_VALID(u16QuestID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterQuestEvent(u16QuestID, (EScriptQuestEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterRoleEvent(lua_State* L)
{
	INT nEventType = luaL_checkint(L, 1);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 2);	// �ű�����

	if( !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterRoleEvent((EScriptRoleEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterMapEvent(lua_State* L)
{
	const CHAR* szMapName = luaL_checkstring(L, 1);		// ��ͼ����
	INT nEventType = luaL_checkint(L, 2);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !P_VALID(szMapName) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterMapEvent(szMapName, (EScriptMapEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterActEvent(lua_State* L)
{
	DWORD dwActID = luaL_checkint(L, 1);				// �ID
	INT nEventType = luaL_checkint(L, 2);				// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if( !GT_VALID(dwActID) || !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterActEvent(dwActID, (EScriptActEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterWorldEvent(lua_State* L)
{
	INT		nEventType = luaL_checkint(L, 1);			// �¼�
	const CHAR* szFunction = luaL_checkstring(L,2);	// �ű�����

	if( !GT_VALID(nEventType) || !P_VALID(szFunction) )
		return 0;

	g_ScriptMgr.RegisterWorldEvent((EScriptWorldEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterItemEvent(lua_State* L)
{
	DWORD dwTypeID	=	(DWORD)luaL_checknumber(L, 1);			// ��ƷID
	INT	nEventType	=	luaL_checkint(L, 2);			// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if(!GT_VALID(dwTypeID) || !GT_VALID(nEventType) || !P_VALID(szFunction))
		return 0;

	g_ScriptMgr.RegisterItemEvent(dwTypeID, (EScriptItemEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterSkillEvent(lua_State* L)
{
	DWORD dwSkillID	=	(DWORD)luaL_checknumber(L, 1);			// ����ID
	INT	nEventType	=	luaL_checkint(L, 2);			// �¼�
	const CHAR* szFunction = luaL_checkstring(L, 3);	// �ű�����

	if(!GT_VALID(dwSkillID) || !GT_VALID(nEventType) || !P_VALID(szFunction))
		return 0;

	g_ScriptMgr.RegisterSkillEvent(dwSkillID, (EScriptSkillEvent)nEventType, szFunction);

	return 0;
}

int SC_RegisterMotionEvent(lua_State* L)
{
	INT nEventType =  luaL_checkint(L,1);           //�¼�ID
	const CHAR* szFunction = luaL_checkstring(L,2); //�ű�����
    if(!GT_VALID( nEventType ) || !P_VALID(szFunction))
		return 0;
	g_ScriptMgr.RegisterMotionEvent((EScriptMotionEvent)nEventType, szFunction);

	return 0;

}

int SC_RegisterBuffEvent(lua_State* L)
{
	INT nEventType =  luaL_checkint(L,1);           //�¼�ID
	const CHAR* szFunction = luaL_checkstring(L,2); //�ű�����
	if(!GT_VALID( nEventType ) || !P_VALID(szFunction))
		return 0;
	g_ScriptMgr.RegisterBuffEvent((EScriptBuffEvent)nEventType, szFunction);

	return 0;

}

////-------------------------------------------------------------------------------------
//// ����
////-------------------------------------------------------------------------------------
//int SC_CreateScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = g_ScriptMgr.CreateScriptMutex();
//
//	lua_pushnumber(L, 0);
//
//	return 1;
//}
//
//int SC_LockScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = lua_tonumber(L, 1);
//
//	g_ScriptMgr.LockScriptMutex(dwMutexID);
//
//	return 0;
//}
//
//int SC_UnLockScriptMutex(lua_State* L)
//{
//	DWORD dwMutexID = lua_tonumber(L, 1);
//
//	g_ScriptMgr.UnLockScriptMutex(dwMutexID);
//
//	return 0;
//}

//-------------------------------------------------------------------------------------
// ���ϵͳʱ��
//-------------------------------------------------------------------------------------
int SC_TimeGetTime(lua_State* L)
{
	DWORD dwTime = timeGetTime();

	lua_pushnumber(L, dwTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���������ǰ��������
//-------------------------------------------------------------------------------------
int SC_GetWorldTick(lua_State* L)
{
	DWORD dwTick = g_world.GetWorldTick();

	lua_pushnumber(L, dwTick);

	return 1;
}

//-------------------------------------------------------------------------------------
//��¼�ű�log
//-------------------------------------------------------------------------------------
int SC_WriteLog(lua_State* L)
{
	ILOG->Write(IUTIL->AnsiToUnicode(lua_tostring(L, 1)));
	return 0;
}

//-------------------------------------------------------------------------------------
// ת��ʱ��ΪDWORD
//-------------------------------------------------------------------------------------
int SC_DataTime2DWORDTime(lua_State* L)
{
	BYTE	byYear		=	(BYTE)lua_tonumber(L, 1);
	BYTE	byMonth		=	(BYTE)lua_tonumber(L, 2);
	BYTE	byDay		=	(BYTE)lua_tonumber(L, 3);
	BYTE	byHour		=	(BYTE)lua_tonumber(L, 4);
	BYTE	byMin		=	(BYTE)lua_tonumber(L, 5);
	BYTE	bySec		=	(BYTE)lua_tonumber(L, 6);

	if( bySec < 0 || bySec > 59 || byMin < 0 || byMin > 59 || byHour < 0 || byHour > 23 ||
		byDay < 0 || byDay > 31 || byMonth <  0 || byMonth > 12 || byYear < 0 || byYear > 63 )
	{
		ASSERT(0);
		return 0;
	}

	DWORD	dwDWORDTime = tagDWORDTime(bySec, byMin, byHour, byDay, byMonth, byYear);

	lua_pushnumber(L, dwDWORDTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���ǰDWORDʱ��
//-------------------------------------------------------------------------------------
int SC_GetCurrentDWORDTime(lua_State* L)
{
	DWORD	dwCurTime = GetCurrentDWORDTime();
	lua_pushnumber(L, dwCurTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// �����������ڵ�ʱ��������
//-------------------------------------------------------------------------------------
int SC_CalcTimeDiff(lua_State* L)
{
	DWORD	dwDestTime		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSrcTime		=	(DWORD)lua_tonumber(L, 2);
	
	DWORD dwSecond = CalcTimeDiff(dwDestTime, dwSrcTime);
	lua_pushnumber(L, dwSecond);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��������Unit�������(ע������Unit���������ͬһ��ͼ)
//-------------------------------------------------------------------------------------
static int SC_CalUnitDist(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID1		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwUnitID2		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit1 = pMap->FindUnit(dwUnitID1);
	if( !P_VALID(pUnit1) ) return 0;

	Unit* pUnit2 = pMap->FindUnit(dwUnitID2);
	if( !P_VALID(pUnit2) ) return 0;

	// ��������Unit�������
	LONG fDistSQ = Vec3DistSq(pUnit1->GetCurPos(), pUnit2->GetCurPos());

	lua_pushnumber(L, (fDistSQ > 750*750)?0:1);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ĳ��ʱ�����ĳ��ֵ
//-------------------------------------------------------------------------------------
int SC_IncreaseTime(lua_State* L)
{
	DWORD	dwSrcTime		=	(DWORD)lua_tonumber(L, 1);
	INT		nIncSecond		=	(INT)lua_tonumber(L, 2);

	DWORD	dwDestTime = IncreaseTime(dwSrcTime, nIncSecond);	
	lua_tonumber(L, dwDestTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// ʱ���ϵݼ�ָ������
//-------------------------------------------------------------------------------------
int SC_DecreaseTime(lua_State* L)
{
	DWORD	dwSrcTime		=	(DWORD)lua_tonumber(L, 1);
	INT		nDecSecond		=	(INT)lua_tonumber(L, 2);

	DWORD	dwDestTime = DecreaseTime(dwSrcTime, nDecSecond);
	lua_tonumber(L, dwDestTime);

	return 1;
}

//-------------------------------------------------------------------------------------
// GM������
//-------------------------------------------------------------------------------------
int SC_GetGMExpRate(lua_State* L)
{
	FLOAT	fGMExpRate = g_GMPolicy.GetExpRate();

	lua_pushnumber(L, fGMExpRate);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��Һ͹��ﹲ�еĽӿں���
//-------------------------------------------------------------------------------------
int SC_ChangeHP(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nHP				=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->ChangeHP(nHP);

	return 0;
}

int SC_ChangeMP(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nMP				=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->ChangeMP(nMP);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�Unit���ڵ�ͼMapCrc������
//-------------------------------------------------------------------------------------
int SC_GetPosition(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	INT nX = INT(pUnit->GetCurPos().x / TILE_SCALE);
	INT nY = INT(pUnit->GetCurPos().y);
	INT nZ = INT(pUnit->GetCurPos().z / TILE_SCALE);

	lua_pushinteger(L, nX);
	lua_pushinteger(L, nY);
	lua_pushinteger(L, nZ);

	return 3;
}

int SC_IsDead(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	BOOL bDead = pUnit->IsDead();
	lua_pushboolean(L, bDead);

	return 1;
}

int SC_IsPlayer(lua_State* L)
{
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 1);
	BOOL	bPlayer			=	FALSE;

	if(IS_PLAYER(dwUnitID))
		bPlayer = TRUE;

	lua_pushboolean(L, bPlayer);

	return 1;
}

int SC_IsCreature(lua_State* L)
{
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 1);
	BOOL	bCreature		=	FALSE;

	if(IS_CREATURE(dwUnitID) || IS_PET(dwUnitID))
		bCreature = TRUE;

	lua_pushboolean(L, bCreature);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��Unitֹͣ�ƶ�
//-------------------------------------------------------------------------------------
int SC_StopMove(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	BOOL	bSendMsg		=	(BOOL)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->GetMoveData().StopMove(bSendMsg);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Unitֹͣ����
//-------------------------------------------------------------------------------------
int SC_StopAttack(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->GetCombatHandler().End();

	return 0;
}

//-------------------------------------------------------------------------------------
// ��λ�Ƿ���ս���У�ֻ������creature
//-------------------------------------------------------------------------------------
int SC_IsAttack(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	// �õ�creature��ǰ״̬
	INT nFight = 0;
	if (pCreature->GetAI()->GetCurrentStateType() == AIST_Idle)
	{
		nFight = 0;
	}
	else
	{
		nFight = 1;
	}

	lua_pushinteger(L, nFight);
	return 1;

}

//-------------------------------------------------------------------------------------
// �����ж�
//-------------------------------------------------------------------------------------
int SC_FriendEnemy(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwSrcID			=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwDstID			=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pSrc = pMap->FindUnit(dwSrcID);
	if( !P_VALID(pSrc) ) return 0;

	Unit* pDest = pMap->FindUnit(dwDstID);
	if( !P_VALID(pDest) ) return 0;

	DWORD dwFriendEnemy = pSrc->FriendEnemy(pDest);

	lua_pushnumber(L, dwFriendEnemy);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡUnit Att
//-------------------------------------------------------------------------------------
static int SC_GetUnitAttValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	lua_pushinteger(L, pUnit->GetAttValue(nIndex));

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡ��λ��ǰ����
//-------------------------------------------------------------------------------------
int SC_GetAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	lua_pushinteger(L, pUnit->GetAttValue(nIndex));
	return 1;
}

//-------------------------------------------------------------------------------------
// �޸ĵ�λ��ǰ����
//-------------------------------------------------------------------------------------
int SC_ModAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pUnit->ModAttValue(nIndex, nValue);

	return 0;
};

//-------------------------------------------------------------------------------------
// ���õ�λ��ǰ����
//-------------------------------------------------------------------------------------
int SC_SetAttValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pUnit->SetAttValue(nIndex, nValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ�����TYPEID
//-------------------------------------------------------------------------------------
int SC_GetUnitTypeID(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwUnitID);
	if( !P_VALID(pCreature) ) return 0;

	DWORD dwTypeID = pCreature->GetTypeID();

	lua_pushnumber(L, dwTypeID);

	return 1;
}

//-------------------------------------------------------------------------------------
// �жϵ�λ�Ƿ���ĳ��״̬
//-------------------------------------------------------------------------------------
int SC_IsInState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	EState		eState			=	(EState)lua_tointeger(L, 4);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	lua_pushboolean(L, pUnit->IsInState(eState));
	return 1;
}

//-------------------------------------------------------------------------------------
// ���õ�λ״̬
//-------------------------------------------------------------------------------------
int SC_SetState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	EState		eState			=	(EState)lua_tointeger(L, 4);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->SetState(eState);
	return 0;
}

//-------------------------------------------------------------------------------------
// �����õ�λ״̬
//-------------------------------------------------------------------------------------
int SC_UnsetState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	EState		eState			=	(EState)lua_tointeger(L, 4);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	pUnit->UnsetState(eState);
	return 0;
}

// �����Ӫ [4/12/2012 zhangzhihua]
int SC_GetCamp(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		lua_pushnumber(L, ECamp_Null);
		return 0;
	}

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))
	{
		lua_pushnumber(L, ECamp_Null);
		return 0;
	}

	ECampType eType = pUnit->GetCampType();
	lua_pushnumber(L, eType);

	return 1;
}

// ������Ӫ [4/12/2012 zhangzhihua]
int SC_SetCamp(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	ECampType	eType			=	(ECampType)lua_tointeger(L, 4);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		return 0;
	}

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))
	{
		return 0;
	}

	pUnit->SetCampType(eType);

	return 1;
}

// ���ó��� [4/12/2012 zhangzhihua]
int SC_SetFaceTo(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	FLOAT		fx				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT		fz				=	(FLOAT)lua_tonumber(L, 5);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		return 0;
	}

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if(!P_VALID(pUnit))
	{
		return 0;
	}

	Vector3 vFaceTo;
	vFaceTo.x = fx;
	vFaceTo.z = fz;
	vFaceTo.y = 0.0f;

	pUnit->GetMoveData().SetFaceTo(vFaceTo, true);

	return 1;
}

//��ȡ��ɫ����ʱ��
int SC_GetRoleCreateTime(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwUnitID		=	(DWORD)lua_tonumber(L, 3);

	// �ҵ���ͼ�͵�λ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Role* pRole = pMap->FindRole(dwUnitID);
	if( !P_VALID(pRole) ) return 0;

	tagDWORDTime pre = pRole->GetRoleCreateTime();

	lua_pushinteger(L, pre.year);
	lua_pushinteger(L, pre.month);
	lua_pushinteger(L, pre.day);
	lua_pushinteger(L, pre.hour);
	return 4;
}

static int SC_UnsetStateBefAdd2Map(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	EState		eState			=	(EState)lua_tointeger(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	pRole->UnsetState(eState);
	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ӽ���
//-------------------------------------------------------------------------------------
static int SC_RoleAddSkill(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSkillTypeID	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	const tagSkillProto* pProto = g_attRes.GetSkillProto(dwSkillTypeID);
	if( !P_VALID(pProto) ) return 0;

	DWORD	dwSkillID	= Skill::GetIDFromTypeID(dwSkillTypeID);
	INT		nLevel		= Skill::GetLevelFromTypeID(dwSkillTypeID);

	if(P_VALID(pRole->GetSkill(dwSkillID))) return 0;
	Skill*  pSkill = new Skill(dwSkillID, nLevel, 0, 0, 0);
	pRole->AddSkill(pSkill);

	const RoleScript * pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript) && P_VALID(pSkill))
		pRoleScript->OnLearnSkill(pRole, dwSkillTypeID);

	// ���¼�������
	pRole->RoleRecalAtt();

	return 0;
}

//-------------------------------------------------------------------------------------
// �Ƿ��Ѿ�ѧ��ָ������
//-------------------------------------------------------------------------------------
static int SC_IsRoleLearnedSkill(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSkillID	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	Skill* pSkill = pRole->GetSkill(dwSkillID);	
	if (!P_VALID(pSkill))	return 0;

	lua_pushinteger(L, pSkill->GetLevel());

	return 1;
}

int SC_AddBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffTypeID	=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwSrcUnitID		=	(DWORD)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	Unit* pSrcUnit = pMap->FindUnit(dwSrcUnitID);

	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return 0;

	pUnit->TryAddBuff(pSrcUnit, pProto, NULL, NULL, NULL);

	return 0;
}

int SC_CancelBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwUnitID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffTypeID	=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Unit* pUnit = pMap->FindUnit(dwUnitID);
	if( !P_VALID(pUnit) ) return 0;

	//pUnit->CancelBuff(dwBuffTypeID);
	pUnit->RemoveBuff(Buff::GetIDFromTypeID(dwBuffTypeID), TRUE);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ҷ��к���
//-------------------------------------------------------------------------------------
int SC_RoleAddQuest(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// �������
	pRole->AcceptQuestFromNPC(u16QuestID, GT_INVALID);

	return 0;
}
static int SC_RoleAddQuestFromNPC(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	DWORD	dwNPCID	=	(UINT16)lua_tointeger(L, 3);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// �������
	pRole->AcceptQuestFromNPC(u16QuestID, dwNPCID);
	return 0;
}
int SC_DeleteQuest(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// ɾ������
	pRole->RemoveQuest(u16QuestID, FALSE);

	// ���ͷ�����Ϣ���ͻ���
	tagNS_DeleteQuest send;
	send.u16QuestID = u16QuestID;
	send.dwErrorCode = E_Success;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

int SC_RoleCompleteQuest(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	DWORD	dwNPCID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16NextQuestID = 0;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	INT nRet = pRole->CompleteQuest(u16QuestID, dwNPCID, -1, EQCT_Normal, u16NextQuestID);

	// ���ͷ���
	tagNS_CompleteQuest send;
	send.u16QuestID = u16QuestID;
	send.dwErrorCode = nRet;
	pRole->SendMessage(&send, send.dwSize);

	// ����Ƿ��к����Զ���ȡ������
	if(E_Success == nRet && u16NextQuestID != 0)
		pRole->AcceptQuestFromNPC(u16NextQuestID, GT_INVALID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ҷ��к���
//-------------------------------------------------------------------------------------
int SC_QuestInit(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID	=	(UINT16)lua_tointeger(L, 2);
	
	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	tagQuestDynamicTarget* pTarget = pQuest->GetDynamicTarget();
	if(!P_VALID(pTarget)) return 0;

	pTarget->eTargetType = (EQuestTargetType)lua_tointeger(L, 3);	// ����̬Ŀ������
	pTarget->dwQuestTipsID = (DWORD)lua_tonumber(L, 4);

	if(EQTT_NPCTalk	== pTarget->eTargetType || EQTT_Invest == pTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			pTarget->dwTargetID[i] = (DWORD)lua_tonumber(L ,i + 5);
		}
	}
	else
	{
		for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
		{
			pTarget->dwTargetID[n] = (DWORD)lua_tonumber(L, (2*n)+5);
			pTarget->nTargetNum[n] = (INT)lua_tonumber(L, (2*n)+6);
		}
	}

	pTarget->nRewMoney = (INT32)lua_tonumber(L, 13);
	pTarget->nRewExp = (INT32)lua_tonumber(L, 14);

	for(int i = 0; i<DYNAMIC_TARGET_COUNT; ++i)
	{
		pTarget->dwRewItemID[i] = (DWORD)lua_tonumber(L, 2*i+15);
		pTarget->dwRewItemNum[i] = (DWORD)lua_tonumber(L, 2*i+16);
	}

	for(int i = 0; i<DYNAMIC_TARGET_COUNT; ++i)
	{
		pTarget->dwOptionRewItemID[i] = (DWORD)lua_tonumber(L, 2*i+23);
		pTarget->dwOptionRewItemNum[i] = (DWORD)lua_tonumber(L, 2*i+24);
	}

	return 0;
}

int SC_RoleGotoNewMap(lua_State* L)		// ȥһ���µ�ͼ
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwDestMapID		=	(DWORD)lua_tonumber(L, 4);
	FLOAT	fx				=	(FLOAT)lua_tonumber(L, 5);
	FLOAT	fy				=	(FLOAT)lua_tonumber(L, 6);
	FLOAT	fz				=	(FLOAT)lua_tonumber(L, 7);

	fx = fx * TILE_SCALE;
	fy = fy * TILE_SCALE;
	fz = fz * TILE_SCALE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->GotoNewMap(dwDestMapID, fx, fy, fz);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ���ҵȼ�
//-------------------------------------------------------------------------------------
int SC_GetRoleLevel(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushinteger(L, pRole->GetLevel());
	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡ��ҵ�ǰ����
//-------------------------------------------------------------------------------------
int SC_GetRoleAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	lua_pushinteger(L, pRole->GetAttValue(nIndex));
	return 1;
}

//-------------------------------------------------------------------------------------
// �޸���ҵ�ǰ����
//-------------------------------------------------------------------------------------
int SC_ModRoleAttValue(lua_State *L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nIndex			=	lua_tointeger(L, 4);
	INT		nValue			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nIndex < 0 || nIndex >= ERA_End ) return 0;

	pRole->ModAttValue(nIndex, nValue);

	return 0;
};

//-------------------------------------------------------------------------------------
// �����Ʒװ�����ٱ���
//-------------------------------------------------------------------------------------
int SC_BaibaoAddRoleItem(lua_State* L)
{
	DWORD			dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD			dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD			dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD			dwTypeID		=	(DWORD)lua_tonumber(L, 4);					// ��ƷTypeID
	INT				nNum			=	lua_tointeger(L, 5);				// ��Ʒ����
	EItemQuality	eQuality		=	(EItemQuality)lua_tointeger(L, 6);	// ��ƷƷ��
	ELogCmdID		eLogCmdID		=	(ELogCmdID)lua_tointeger(L, 7);		// ��¼log����
	INT				nCreateMode		=	lua_tointeger(L, 8);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagItem* pNew = ItemCreator::CreateEx((EItemCreateMode)nCreateMode, dwRoleID, dwTypeID, nNum, eQuality);
	if(!P_VALID(pNew))
	{
		return FALSE;
	}

	pRole->GetItemMgr().ProcBaiBaoRecord(dwTypeID, dwRoleID, 
		pRole->GetNameID(), EBBRT_System, pNew->dw1stGainTime);
	pRole->GetItemMgr().Add2BaiBao(pNew, eLogCmdID, FALSE, dwRoleID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��һ��������Ʒװ��
//-------------------------------------------------------------------------------------
int SC_QuestAddRoleItem(lua_State* L)
{
	DWORD			dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD			dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD			dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16			u16QuestID		=   (UINT16)lua_tointeger(L, 4);		// ����ID
	DWORD			dwTypeID		=	(DWORD)lua_tonumber(L, 5);					// ��ƷTypeID
	INT				nNum			=	lua_tointeger(L, 6);				// ��Ʒ����
	EItemQuality	eQuality		=	(EItemQuality)lua_tointeger(L, 7);	// ��ƷƷ��
	ELogCmdID		eLogCmdID		=	(ELogCmdID)lua_tointeger(L, 8);		// ��¼log����

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;
	
	pRole->GetItemMgr().Add2Role(EICM_Quest, u16QuestID, dwTypeID, (INT16)nNum, eQuality, eLogCmdID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��һ����Ʒװ��
//-------------------------------------------------------------------------------------
static int SC_AddRoleItem(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT32 nQlty			=	lua_tointeger(L, 6);
	INT   nCreateMode	=	lua_tointeger(L, 7);
	INT	  nLogCmdID		=	lua_tointeger(L, 8);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	//�жϱ����Ƿ��п�λ
	ItemMgr& itemMgr = pRole->GetItemMgr();
	if (itemMgr.GetBagFreeSize() < 1)
	{
		return -1;
	}

	if(n16Num <= 0)
	{
		n16Num = 1;
	}

	tagItem *pNewItem = ItemCreator::Create((EItemCreateMode)nCreateMode, dwRoleID, dwTypeID, n16Num,-1,0,0,10000,0,nQlty);
	if(!P_VALID(pNewItem))
	{
		return 0;
	}

	//if(MIsEquipment(pNewItem->dwTypeID))
	//{
	//	if(nQlty == GT_INVALID)
	//	{
	//		// ������	
	//	}
	//	else
	//	{
	//		ItemCreator::IdentifyEquip((tagEquip*)pNewItem, (EItemQuality)nQlty);
	//	}
	//}

	//pRole->GetItemMgr().Add2Bag(pNewItem, (ELogCmdID)nLogCmdID, TRUE);

	DWORD dwErrorCode = pRole->GetItemMgr().Add2Bag(pNewItem, (ELogCmdID)nLogCmdID, TRUE);
	//���뱳��ʧ�ܣ���ɾ������Ʒ
	if (E_Success != dwErrorCode)
	{
		Destroy(pNewItem);
		return 0;
	}

	//lua_pushnumber(L, pNewItem->n64Serial);
	push_64bit_data(L, pNewItem->n64Serial);
	
	if(!MIsEquipment(dwTypeID))
	{
		return 2;
	}

	lua_pushinteger(L, pNewItem->pEquipProto->eEquipPos);

	return 3;
}

//-------------------------------------------------------------------------------------
// ��Ҵ���װ��
//-------------------------------------------------------------------------------------
static int SC_RoleEquip(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	//INT64 n64Serial		=	lua_tonumber(L, 4);
	INT64 n64Serial		=	pop_64bit_data(L, 4, 5);
	INT16 n16EquipPos	=	(INT16)lua_tonumber(L, 6);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->Equip(n64Serial, (EEquipPos)n16EquipPos);

	return 0;
}

//-------------------------------------------------------------------------------------
// �ӱ�����������ɾ����Ʒ
//-------------------------------------------------------------------------------------
int SC_RemoveFromRole(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->GetItemMgr().RemoveFromRole(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

int SC_RemoveFromWare(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->GetItemMgr().RemoveFromWare(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

int SC_RemoveFromEquipBar(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID		=	(DWORD)lua_tonumber(L, 4);
	INT16 n16Num		=	(INT16)lua_tointeger(L, 5);
	INT	  nLogCmdID		=	lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT nRet = pRole->DeleteEquipFromEquipBar(dwTypeID, n16Num, nLogCmdID);

	lua_pushinteger(L, nRet);

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ұ������пռ�����
//-------------------------------------------------------------------------------------
int SC_GetBagFreeSize(lua_State* L)
{
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	ItemMgr& itemMgr = pRole->GetItemMgr();	

	INT nFreeSize = itemMgr.GetBagFreeSize();

	lua_pushinteger(L, nFreeSize);

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ұ��������񱳰�����Ʒ������
//-------------------------------------------------------------------------------------
int SC_GetRoleItemNum(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwTypeID		=	(DWORD)lua_tonumber(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) 
	{
		lua_pushinteger(L, 0);
		return 1;
	}

	ItemMgr& itemMgr = pRole->GetItemMgr();

	INT nTotal = itemMgr.GetBagSameItemCount(dwTypeID) + itemMgr.GetQuestBagSameItemCount(dwTypeID);

	lua_pushinteger(L, nTotal);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------------
int SC_GetRoleName(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	TCHAR szName[X_SHORT_NAME];
	g_roleMgr.GetRoleNameByID(dwRoleID, szName);

	lua_pushstring(L, IUTIL->UnicodeToUnicode8(szName));

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ƿ���С��
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveTeam(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetTeamID());

	return 1;
}

//-------------------------------------------------------------------------------------
// ������С�ӳ�ԱID
//-------------------------------------------------------------------------------------
int SC_GetRoleTeamMemID(lua_State* L)
{
	DWORD	dwTeamID		=	(DWORD)lua_tonumber(L, 1);
	
	const Team *pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
	if(!P_VALID(pTeam))	return 0;

	DWORD dwMemID[MAX_TEAM_NUM];
	memset(dwMemID, 0xFF, sizeof(dwMemID));

	pTeam->ExportAllMemID(dwMemID);

	for (INT i = 0; i < MAX_TEAM_NUM; ++i)
	{
		lua_pushnumber(L, dwMemID[i]);
	}

	return MAX_TEAM_NUM;
}

//-------------------------------------------------------------------------------------
// ��������ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// ����ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+5);

		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		pQuest->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �޸�����ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_ModQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// ����ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+5);

		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		pQuest->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��������ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetQuestScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 2);		// ����ID
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if(!P_VALID(pQuest)) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+4);		
		
		if( nIndex < 0 || nIndex >= ESD_Quest ) return 0;

		dwValue = pQuest->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ����Ƿ��ȡ�˸�����
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveQuest(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// ����ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	BOOL bHaveQuest = pRole->IsHaveQuest(u16QuestID);
	lua_pushboolean(L, bHaveQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ƿ��ȡ�˸����������
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveBoardQuest(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// ����ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	UINT16 u16ID = 0;
	for (int i=QUEST_PANEL_START_TASK_ID; i<=QUEST_PANEL_END_TASK_ID; ++i)
	{
		if (EQBT_Null == pRole->GetQuestBoardType(i))
		{
			continue;
		}

		if (IsRewardBoardQuest(i) && u16QuestID == pRole->GetBoardQuestMgr(EQBT_Reward)->GetBoardIDFromID(i))
		{
			u16ID = i;
			break;
		}

		if (IsYellowListQuest(i) && u16QuestID == pRole->GetBoardQuestMgr(EQBT_YellowList)->GetBoardIDFromID(i))
		{
			u16ID = i;
			break;
		}
	}

	BOOL bHaveQuest = pRole->IsHaveQuest(u16ID);
	lua_pushboolean(L, bHaveQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ƿ�����ĳ����
//-------------------------------------------------------------------------------------
int SC_IsRoleCompleteQuest(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=   (UINT16)lua_tonumber(L, 4);		// ����ID

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	BOOL bCompleteQuest = pRole->IsHaveDoneQuest(u16QuestID);
	lua_pushboolean(L, bCompleteQuest);

	return 1;
}

//-------------------------------------------------------------------------------------
// ������Ҿ���
//-------------------------------------------------------------------------------------
int SC_AddRoleExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	//INT64	nValue			= pop_64bit_data(L,4,5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->ExpChange((INT64)nValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ҽ�Ǯ
//-------------------------------------------------------------------------------------
int SC_AddRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( nValue <= 0 ) return 0;

	pRole->GetCurMgr().IncBagSilver(nValue, (ELogCmdID)nCmdID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ҽ�Ǯ
//-------------------------------------------------------------------------------------
int SC_DecRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT64	n64Value		=	(INT64)lua_tonumber(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if( n64Value <= 0 ) return 0;

	pRole->GetCurMgr().DecBagSilver(n64Value, (ELogCmdID)nCmdID);

	return 0;
}


//-------------------------------------------------------------------------------------
// �����ҽ�Ǯ
//-------------------------------------------------------------------------------------
int SC_GetRoleSilver(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT64 n64Silver = pRole->GetCurMgr().GetBagSilver();
	lua_pushnumber(L, n64Silver);
	//lua_pushinteger(L, nSilver);
	return 1;
}

//-------------------------------------------------------------------------------------
// ������Ԫ����
//-------------------------------------------------------------------------------------
int SC_GetRoleYuanbao(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT32  iYuanbao = pRole->GetCurMgr().GetBagYuanBao();
	lua_pushnumber(L, iYuanbao);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ɫʹ�ü���
//-------------------------------------------------------------------------------------
static int SC_RoleUseSkill(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=   (DWORD)lua_tonumber(L, 4);
	DWORD	dwSkillID		=   (DWORD)lua_tonumber(L, 5);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCombatHandler().UseSkill(dwSkillID,dwTargetID, 0);
	
	return 0;
}

//------------------------------------------------------------------------------------
// ȡ��ʹ�ü���
//-------------------------------------------------------------------------------------
static int SC_RoleCancelSkill(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwSkillID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCombatHandler().CancelSkillUse(dwSkillID);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�ʦ��ID
//-------------------------------------------------------------------------------------
static int SC_GetMasterID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetMasterID());
	return 1;
}

//-------------------------------------------------------------------------------------
// ������ʦ���ʸ����񣬳�Ϊʦ��
//-------------------------------------------------------------------------------------
static int SC_RoleBecomeMaster(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ��DB��master���в���һ��
	tagNDBC_InsertMasterApprenticeData sendDB;
	sendDB.sMasterApprentice.dwID = dwRoleID;
	sendDB.sMasterApprentice.nJingWuPoint = 0;
	sendDB.sMasterApprentice.nQinWuPoint = 0;
	sendDB.sMasterApprentice.sApprentice = tagApprenticeData();
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// �������Ϊʦ�����Ӿ��������ͬʱΪ�Լ�����һ���ľ������
//-------------------------------------------------------------------------------------
static int SC_LevelUpAddJingWuPoint(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return 0;
	}

	// ���ʦ������
	Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
	if (P_VALID(pMaster))
	{
		pMaster->SetJingWuPoint(pProto->nLevelUpAddPoint + pMaster->GetJingWuPoint());
	}

	// ���������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetMasterID();
	sendDB.sOneMAData.eType = EOMAD_JingWu;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// ��������
	*(INT*)(sendDB.sOneMAData.byData) = pProto->nLevelUpAddPoint;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// Ϊ�Լ����Ӿ������
	pRole->SetJingWuPoint(pProto->nLevelUpSelfPoint + pRole->GetJingWuPoint());

	// ��������ͬ����ʦ����õľ������
	for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
	{
		DWORD dwMateID = pRole->GetClassMate().dwData[i];
		Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
		if (P_VALID(pMate))
		{
			pMate->GetClassMate().nJingWuPoint += pProto->nLevelUpAddPoint;
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �������Ϊʦ������ͽ����������
//-------------------------------------------------------------------------------------
static int SC_LevelUpAddBonus(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return 0;
	}

	// ���ʦ������
	Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
	if (P_VALID(pMaster))
	{
		pMaster->SetLevelUpBonus(pProto->nLevelUpBonus + pMaster->GetLevelUpBonus());
	}

	// ���������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetMasterID();
	sendDB.sOneMAData.eType = EOMAD_LevelUpBonus;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// ��������
	*(INT*)(sendDB.sOneMAData.byData) = pProto->nLevelUpBonus;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ʦ֪ͨ
//-------------------------------------------------------------------------------------
static int SC_GraduateNotice(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ��ͽ�ܷ�
	tagNS_GraduateNotice sendApprentice;
	sendApprentice.dwRoleID = dwRoleID;
	pRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);

	// ���ʦ��������֪ͨʦ��������һ��ʦ������ȡ�ĳ�ʦ��������
	Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
	if (P_VALID(pMaster))
	{
		pMaster->SetRewardNum(pMaster->GetRewardNum() + 1);

		tagNS_GraduateNotice sendMaster;
		sendMaster.dwRoleID = dwRoleID;
		pMaster->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
	}

	// ���������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetMasterID();
	sendDB.sOneMAData.eType = EOMAD_RewardNum;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// ��������
	*(INT*)(sendDB.sOneMAData.byData) = 1;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ÿ��ó�ʦ��������
//-------------------------------------------------------------------------------------
static int SC_GetGraduateRewardNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetRewardNum());
	return 1;
}

//-------------------------------------------------------------------------------------
// �ı���ó�ʦ��������
//-------------------------------------------------------------------------------------
static int SC_SetGraduateRewardNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nNum			=	(INT)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetRewardNum(nNum);

	// ���������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_RewardNum;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// ��������
	*(INT*)(sendDB.sOneMAData.byData) = -1;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡʦ�µ���
//-------------------------------------------------------------------------------------
static int SC_GetQinWuPoint(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetQinWuPoint());
	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡͽ����������
//-------------------------------------------------------------------------------------
static int SC_GetLevelUpBonus(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetLevelUpBonus());
	return 1;
}

//-------------------------------------------------------------------------------------
// �ı�ͽ����������
//-------------------------------------------------------------------------------------
static int SC_SetLevelUpBonus(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nPoint			=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetLevelUpBonus(nPoint);

	// ���������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_LevelUpBonus;
	sendDB.sOneMAData.eMode = EMAMM_Set;		// �滻����
	*(INT*)(sendDB.sOneMAData.byData) = nPoint;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}


//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ���ú���״̬
//-------------------------------------------------------------------------------------
static int SC_SetForceInArena(lua_State* L)
{
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID		= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			= (DWORD)lua_tonumber(L, 3);
	BOOL  bRedOrBlue		= (BOOL)(DWORD)lua_tonumber(L, 4);
	
	//ȡ���������
	AthleticsRoomMgr* pAthleticsRoomMgr = g_AthleticsSystem.GetRoomMgrPtr();
	if (!P_VALID(pAthleticsRoomMgr))
	{
		return 0;
	}

	tagPvPRoleInRoom* pRoleInRoom = pAthleticsRoomMgr->GetInRoomRole(dwRoleID);
	if (!P_VALID(pRoleInRoom))
	{
		return 0;
	}

	RoomBase* pRoom = pAthleticsRoomMgr->GetPvPRoom(pRoleInRoom->dwSerialID);
	if (!P_VALID(pRoom))
	{
		return 0;
	}

	if (pRoom->GetRoomType() == E_PvP_Room3v3)
	{
		Athletics3V3Room* p3v3Room		= (Athletics3V3Room*)pRoom;
		BYTE ucPos						= p3v3Room->GetRolePos(dwRoleID);
		tagPvPRoleInfo stPvPRoleInfo	= {0};
		tagPvPRoleInfo& PvPRoleInfo		= p3v3Room->GetPosRoleInfo(ucPos);

		stPvPRoleInfo.dwRoleID		= PvPRoleInfo.dwRoleID;
		stPvPRoleInfo.eRoleInPvP	= PvPRoleInfo.eRoleInPvP;
		stPvPRoleInfo.eRoleInSide	= PvPRoleInfo.eRoleInSide;
		stPvPRoleInfo.eState		= PvPRoleInfo.eState;
		stPvPRoleInfo.ucPos			= PvPRoleInfo.ucPos;

		memcpy(&stPvPRoleInfo, &PvPRoleInfo, sizeof(tagPvPRoleInfo));
		
		stPvPRoleInfo.eRoleInSide = bRedOrBlue?ERBS_DefenseSide:ERBS_Attack;

		p3v3Room->SetPosRoleInfo(ucPos, (tagPvPRoleInfo&)stPvPRoleInfo);

		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(P_VALID(pRole))
		{
			pRole->SetState(bRedOrBlue?ES_PvPDefence:ES_PvPAttack);
			pRole->UnsetState(bRedOrBlue?ES_PvPAttack:ES_PvPDefence);
		}

		//ͬ���з�Ѫ��
	    p3v3Room->SendRemoteAttToPvPRole(dwRoleID,TRUE);
	
	}
	else
	{
		ASSERT(0);
		return 0;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ָ���������ԭ����С��
//-------------------------------------------------------------------------------------
static int SC_LetRoleLeaveTeam(lua_State* L)
{
	DWORD dwTeamID 		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID 		= (DWORD)lua_tonumber(L, 2);



     
	DWORD dwRet  = E_Success;
	//dwRet = ((Team*)pTeam)->LeaveTeam(pRole);
     g_groupMgr.OnMemberLeaveTeam(dwRoleID,NULL);




	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ָ�����1��ָ�����2���һ���µ�С�Ӳ��ҷ���С��ID
//-------------------------------------------------------------------------------------
static int SC_CreatANewTeam(lua_State* L)
{
	DWORD dwRoleID1 	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID2 	= (DWORD)lua_tonumber(L, 2);

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if(!P_VALID(pRole1) || pRole1->GetTeamID() != GT_INVALID)
	{
		// pRole1�����߻����Ѿ��ж���
		return 0;
	}

	Role* pRole2 = g_roleMgr.GetRolePtrByID(dwRoleID2);
	if(!P_VALID(pRole2) || pRole2->GetTeamID() != GT_INVALID)
	{
		// pRole2�����߻����Ѿ��ж���
		return 0;
	}


	//�������С��
	Team * pTeam = g_groupMgr.CreateTeamWithoutNotify(pRole1, pRole2);
	if(!P_VALID(pTeam))	return 0;	
	
	lua_pushnumber(L, pTeam->GetTeamID());

	return 1;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ָ����ҷ���һ��ָ����С����
//-------------------------------------------------------------------------------------
static int SC_LetRoleJoinTeam(lua_State* L)
{
	DWORD dwTeamID = (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 2);

	// �ҵ����С��
	Team* pTeam = const_cast<Team*>(g_groupMgr.GetTeamPtr(dwTeamID));
	if (!P_VALID(pTeam))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	if (pTeam->AddMemberWithoutNotify(dwRoleID) != E_Success)
	{
		lua_pushboolean(L, 0);
		return 1;
	}	

	lua_pushboolean(L, 1);

	return 1;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ����Ҵ��뾺����
//-------------------------------------------------------------------------------------
static int SC_RoleGotoArena(lua_State* L)
{
	DWORD	dwMapID				=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID		=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwDestMapID			=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwDestInstanceID	=	(DWORD)lua_tonumber(L, 5);
	FLOAT	fx					=	(FLOAT)lua_tonumber(L, 6);
	FLOAT	fy					=	(FLOAT)lua_tonumber(L, 7);
	FLOAT	fz					=	(FLOAT)lua_tonumber(L, 8);

	//ȡ��ͼ����ɫ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ��鸱���Ƿ񱻴�����
	Map* pDestMap = g_mapCreator.GetMap(dwDestMapID, dwDestInstanceID);
	if( !P_VALID(pDestMap) || EMT_System != pDestMap->GetMapType() ) return 0;

	//����Ҵ��ͽ�������
// 	pInstance->AddRole(pRole);
	pRole->GotoNewMap(dwDestMapID, fx, fy, fz, GT_INVALID, FALSE, dwDestInstanceID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// �Ӿ����������ͷ����б���ɾ��һ�����
//-------------------------------------------------------------------------------------
static int SC_DeletRoleFromArenaTable(lua_State* L)
{
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
    

	//Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
    g_AthleticsSystem.GetRoomMgr().OnOffLine(dwRoleID,FALSE);

	//EPvPError ePvPError = E_PvP_Success;
	
	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���θò��ֹ���
	//--------------------------------------------------------------------------
	// 	tagNS_PvPLeaveHall send;
	// 	send.dwErrorCode = ePvPError;
	// 	pRole->SendMessage(&send,send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ǿ�Ƹ���Ľű��ӿ�
//-------------------------------------------------------------------------------------
static int SC_ForcedToRevive(lua_State* L)
{
	DWORD	dwMapID				=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID		=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 3);
	INT		nReviveMP			=	1;
	INT		nReviveHP			=	1;
	DWORD	dwRefMapID			=	dwMapID;
	float	x, y, z;

	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetAttValue(ERA_HP, nReviveHP);
	pRole->SetAttValue(ERA_MP, nReviveMP);
	pRole->UnsetState(ES_Dead);
	x = pRole->GetCurPos().x;
	y = pRole->GetCurPos().y;
	z = pRole->GetCurPos().z;

	//�����ɫ
	pMap->OnRevive(pRole, ERRT_Locus, nReviveHP, nReviveMP, x, y, z, dwRefMapID);
	pRole->SetAttValue(ERA_HP, nReviveHP);
	pRole->SetAttValue(ERA_MP, nReviveMP);

	tagNS_RoleRevive send;
	send.dwRoleID		= dwRoleID;
	send.dwErrorCode = E_Success;
	pRole->SendMessage(&send, send.dwSize);

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}
    


	pRole->GotoNewMap(dwRefMapID, x, y,z);

	return 0;
}
//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ���ָ������ָ��������ڵķ������͡���������ַ�����0���������ͨ����1��
// �����û���ҵ��򷵻�-1
//-------------------------------------------------------------------------------------
static int SC_GetArenaType(lua_State* L)
{
	DWORD	dwRoomID				=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 2);

	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���θò��ֹ���
	//--------------------------------------------------------------------------
	lua_pushinteger(L, GT_INVALID);
	return 1;
	//--------------------------------------------------------------------------

	//ȡ���������
	AthleticsRoomMgr* pAthleticsRoomMgr = g_AthleticsSystem.GetRoomMgrPtr();
	if (!P_VALID(pAthleticsRoomMgr))
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}

	tagPvPRoleInRoom* pRoleInRoom = pAthleticsRoomMgr->GetInRoomRole(dwRoleID);
	if (!P_VALID(pRoleInRoom))
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}

	RoomBase* pRoom = pAthleticsRoomMgr->GetPvPRoom(pRoleInRoom->dwSerialID);
	if (!P_VALID(pRoom))
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}

	if (pRoom->GetRoomLevel() == E_PvP_Junior)	//����(���־���) 
	{
		lua_pushinteger(L, 0);
		return 1;
	}
	else if (pRoom->GetRoomLevel() == E_PvP_Middle)	//�м�(��ͨ���� )
	{
		lua_pushinteger(L, 1);
		return 1;
	}
	else
	{
		lua_pushinteger(L, GT_INVALID);
		return 1;
	}
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��þ��������������
// Ҫ�󷵻�����ֵ���������ѫҪ�󣬷������͵ȼ�Ҫ�󣬷������ߵȼ�Ҫ��
// ���û���ҵ��÷��䣬�򷵻�-1��

//-------------------------------------------------------------------------------------
static int SC_GetArenaRoomLimit(lua_State* L)
{
	INT	nRoomType	= lua_tointeger(L, 1);
	INT	nRoomID 	= lua_tointeger(L, 2);

	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���θò��ֹ���
	//--------------------------------------------------------------------------
	lua_pushinteger(L,-1);
	return 1;
	//--------------------------------------------------------------------------

	tagPvPRoom*  pRoomInfo = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo(nRoomID,(E_PvP_RoomLevel)nRoomType);
    if ( P_VALID( pRoomInfo) )
    {
		INT nWuXunValLimit       = pRoomInfo->dwWuXunLimit ;
        INT nRoomLevelLowerLimit = pRoomInfo->iLevelLowerLimit;
		INT nRoomLevelUpperLimit = pRoomInfo->iLevelUpperLimit;
		lua_pushinteger(L, nWuXunValLimit);
		lua_pushinteger(L, nRoomLevelLowerLimit);
		lua_pushinteger(L, nRoomLevelUpperLimit);
		return 3 ;
    }
	else
	{
         lua_pushinteger(L,-1);
		 return 1;
	}
}

//--------------------------------------------------------------------------
//SM ������������޸�
//���Ӹò��ֹ��� 
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// �����״̬�����ͻ���
// �����û���ҵ��򷵻�-1
//-------------------------------------------------------------------------------------
static int SC_ShowAlivePeopleNameInArena(lua_State* L)
{
	DWORD  dwRoleID        	= lua_tonumber(L, 1);
	DWORD  dwEnermyRoleID 	= lua_tonumber(L, 2); //�з���ɫID
	DWORD  dwStateVal       = lua_tonumber(L, 3);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
	{
		return 0;
	}
	
	tagNS_PvPSinglePlayerInfo Msg;
	Msg.dwPlayerID = dwEnermyRoleID;
	Msg.eState     = (EPvPPlayerState)dwStateVal;
	pRole->SendMessage(&Msg,Msg.dwSize);
    return 0;

}

//--------------------------------------------------------------------------
//SM ������������޸�
//���Ӹò��ֹ��� 
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// �����״̬�����ͻ���.��������ַ�����0���������ͨ����1��
// �����û���ҵ��򷵻�-1
//-------------------------------------------------------------------------------------
static int SC_CloseArenaWaitingRoom(lua_State* L)
{
    g_AthleticsSystem.GetRoomMgr().AthleticsActiveOver();  
	return 0;
}



//--------------------------------------------------------------------------
//SM ������������޸�
//���Ӹò��ֹ��� 
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ����Ҽ��뾺��������������ַ�����0���������ͨ����1��
// �����û���ҵ��򷵻�-1
//-------------------------------------------------------------------------------------
static int SC_JoinArena(lua_State* L)
{
	DWORD dwMapID	    = (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID 	= (DWORD)lua_tonumber(L, 2);
    DWORD dwRoleID      = (DWORD)lua_tonumber(L, 3);



	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
	{
		return 0;
	}

	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		tagNS_PvPJoinRoom msg;
		msg.dwErrorCode = E_PvP_UnOpened;
		pRole->SendMessage(&msg,msg.dwSize);
		return E_PvP_UnOpened;

	}

	if ( g_AthleticsSystem.GetRoomMgr().IsRoleExist(dwRoleID))
	{
		tagNS_PvPJoinRoom msg;
		msg.dwErrorCode = E_PvP_AlreadyInRoom;
		pRole->SendMessage(&msg,msg.dwSize);
		return E_PvP_AlreadyInRoom;         
	}
    
	DWORD dwRoleLevel = pRole->GetLevel();
    DWORD dwErrorCode = E_Success;

	if( MAX_LEVEL_LIMIT > dwRoleLevel )
	{
		tagNS_PvPJoinRoom msg;
		msg.dwErrorCode = E_PvP_InvalidLevel;
		pRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	tagNS_PvPJoinRoom Msg;

	//DWORD dwValiantMerit = pRole->GetAttValue(ERA_WuXun); 
	//DWORD dwValiantMeritLevel = pRole->GetValiantMeritLevel(dwValiantMerit);

	// 	if ( dwRoleLevel < 30)
	// 	{
	// 		Msg.dwErrorCode = E_PvP_InvalidLevel;
	// 		Msg.eRoomLevel  = E_PvP_Junior;
	// 		Msg.nRoomID     = 0;
	// 		pRole->SendMessage(&Msg,Msg.dwSize);
	// 		return 0;
	// 	}

	TMap<INT16, tagPvPRoom*>& pvpMapRoomInfo = g_AthleticsSystem.GetRoomMgr().GetMapPvPRoomInfo(E_PvP_Junior);
	TMap<INT16, tagPvPRoom*>::TMapIterator it = pvpMapRoomInfo.Begin();
	tagPvPRoom* pRoomInfo = NULL;
	BOOL  bFindOut   = FALSE ;
	INT  nRoomID    =  0;
	std::vector<INT16> vecRoomID;
	vecRoomID.clear();
	while ( pvpMapRoomInfo.PeekNext(it,pRoomInfo))
	{
		if ( P_VALID(pRoomInfo) )
		{
			if ( (pRoomInfo->eRoomState != E_PvP_Waiting) || (pRoomInfo->ucCurrentPersonNum == 6) || pRoomInfo->ucCurrentPersonNum == 0 )
			{
				continue;
			}
			if ( (dwRoleLevel  >= pRoomInfo->iLevelLowerLimit) && (dwRoleLevel <= pRoomInfo->iLevelUpperLimit))
			{
				vecRoomID.push_back(pRoomInfo->nRoomID);
			}
		}
	}
	if (vecRoomID.size() > 0)
	{
         bFindOut  = TRUE ;
		 if ( 1 == vecRoomID.size())
		 {
              nRoomID = vecRoomID[0];
		 }
		 else
		 {
			  INT nMaxRoomID = (INT)(vecRoomID.size() - 1);
			  INT nRandomIndex  = IUTIL->RandomInRange(0,nMaxRoomID);
              nRoomID  =  vecRoomID[nRandomIndex];
		 }
	}

	if ( !bFindOut)   // û���ҵ����Զ�����һ���ʺ���ҵķ���
	{

		TCHAR szRoleName[X_SHORT_NAME];
		DWORD dwRoleId = pRole->GetID();
		g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);

		INT  nLevelUpperLimit =  dwRoleLevel + 8;//dwRoleLevel + (10 - ((dwRoleLevel%10)== 0 ? 10 : (dwRoleLevel%10) ));
		INT  nLevelLowerLimit =  dwRoleLevel - 8;//dwRoleLevel - ((dwRoleLevel%10)== 0 ? 10  : (dwRoleLevel%10) );
		if ( MAX_LEVEL_LIMIT == dwRoleLevel)
		{
			nLevelLowerLimit = MAX_LEVEL_LIMIT ;
			nLevelUpperLimit = MAX_LEVEL_LIMIT + 8 ;
		}

		if ( nLevelLowerLimit < MAX_LEVEL_LIMIT )
		{
			nLevelLowerLimit = MAX_LEVEL_LIMIT;
		}

		tagPvPRoom PvPRoom;
		PvPRoom.nRoomID            = (INT16)g_AthleticsSystem.GetRoomMgr().GetAvailRoomId(E_PvP_Junior);
		PvPRoom.dwManagerID        = dwRoleId;
		PvPRoom.dwWuXunLimit       = 1;
		PvPRoom.eLevel             = E_PvP_Junior;
		PvPRoom.eRoomState         = E_PvP_Waiting;
		PvPRoom.eRoomType          = E_PvP_Room3v3;
		PvPRoom.iLevelLowerLimit   = nLevelLowerLimit;
		PvPRoom.iLevelUpperLimit   = nLevelUpperLimit;
		_tcscpy_s(PvPRoom.szManagerName,X_SHORT_NAME, szRoleName);
		PvPRoom.ucCurrentPersonNum = 1;
		PvPRoom.ucTotalPersonNum   = 6;
	

		if ( pRole->IsDead())
		{
			dwErrorCode = E_PvP_CreateRoomFail;
			Msg.dwErrorCode = dwErrorCode;
			Msg.eRoomLevel  = E_PvP_RoomLevelNull;
			Msg.nRoomID     = 0;
			pRole->SendMessage(&Msg,Msg.dwSize);
			return 0;	
		}

		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if ( P_VALID( pRoleScript ) )
		{
			BOOL bCanCreate = FALSE;
			pRoleScript->OnRoleCreateArenaRoom(pRole,E_PvP_Junior,PvPRoom.nRoomID,bCanCreate);
			if ( !bCanCreate)
			{    
				dwErrorCode = E_PvP_CreateRoomFail;
				Msg.dwErrorCode = dwErrorCode;
				Msg.eRoomLevel  = E_PvP_RoomLevelNull;
				Msg.nRoomID     = 0;
				pRole->SendMessage(&Msg,Msg.dwSize);
				return 0;		

			}
		}
		else
		{
			return 0;
		}

		//�ȼ��뾺�������� ��֮ǰ�޸��������������޸�

		dwErrorCode =  g_AthleticsSystem.GetRoomMgr().AddToHall(E_PvP_Room3v3,dwRoleID);
		if ( dwErrorCode != E_Success)
		{
			return 0;
		}
		dwErrorCode = g_AthleticsSystem.GetRoomMgr().CreateAthleticsRoom(PvPRoom);

		// 		if (  dwErrorCode == E_PvP_Success)
		// 		{
		// 			g_AthleticsSystem.ModHallRoomNumTemp(1);
		// 	
	    //       }

        if ( E_PvP_Success != dwErrorCode)
        {
			 g_AthleticsSystem.GetRoomMgr().LeaveHall(dwRoleID);
			 return dwErrorCode;
        }
         
        
		DWORD dwTotal    = pRole->GetScriptData(24);
		DWORD dwWinCount = pRole->GetScriptData(25);	
		Msg.dwLocalPlayerRankLevel   = pRole->GetPm(ERT_WuXun) ;
		if ( dwTotal > 0)
		{
			Msg.fLocalPlayerSuccessRate  = (float)(((float)dwWinCount) / ((float)dwTotal));
		}
		else
		{
			Msg.fLocalPlayerSuccessRate  = 0.0f;
		}

		Msg.dwErrorCode = dwErrorCode;
		Msg.eRoomLevel  = PvPRoom.eLevel;
		Msg.nRoomID     = PvPRoom.nRoomID;
		pRole->SendMessage(&Msg,Msg.dwSize);
		return 0;   
	}
	else
	{

		if ( pRole->IsDead())
		{
			Msg.dwErrorCode = E_PvP_CanNotJoinRoom;
			Msg.eRoomLevel  = E_PvP_Junior;
			Msg.nRoomID     = nRoomID;
			pRole->SendMessage(&Msg,Msg.dwSize); 
			return 0;

		}

		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if ( P_VALID( pRoleScript ) )
		{
			BOOL bCanJoin = FALSE;
			pRoleScript->OnRoleJoinToArenaRoom(pRole,E_PvP_Junior,nRoomID,bCanJoin);
			if ( !bCanJoin)
			{    
				return 0;
			}
		}
		else
		{
			return 0;
		}
		//�ȼ��뾺�������� ��֮ǰ�޸��������������޸�
		dwErrorCode =  g_AthleticsSystem.GetRoomMgr().AddToHall(E_PvP_Room3v3,dwRoleID);

		if ( dwErrorCode != E_Success)
		{
             return dwErrorCode;
		}
		dwErrorCode = g_AthleticsSystem.GetRoomMgr().RoleJoinInRoom(pRole->GetID(),E_PvP_Junior,nRoomID); 

		if ( dwErrorCode != E_Success )
		{
           g_AthleticsSystem.GetRoomMgr().LeaveHall(dwRoleID);
		   return dwErrorCode;
		}


		DWORD dwTotal    = pRole->GetScriptData(24);
		DWORD dwWinCount = pRole->GetScriptData(25);
		Msg.dwLocalPlayerRankLevel   = pRole->GetPm(ERT_WuXun) ;
		if ( dwTotal > 0)
		{
			Msg.fLocalPlayerSuccessRate  = (float)(((float)dwWinCount) / ((float)dwTotal));
		}
		else
		{
			Msg.fLocalPlayerSuccessRate  = 0.0f;
		}
		Msg.dwErrorCode = dwErrorCode;
		Msg.eRoomLevel  = E_PvP_Junior;
		Msg.nRoomID     = nRoomID;
		pRole->SendMessage(&Msg,Msg.dwSize); 
	}

   return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ָ�������ʾ�����������
//-------------------------------------------------------------------------------------
static int SC_ShowAlivePeopleInArena(lua_State* L)
{
	DWORD	dwRoleID	= (DWORD)lua_tonumber(L, 1);
	INT		nAliveRed	= lua_tointeger(L, 2);
	INT		nAliveBlue	= lua_tointeger(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;


 	tagNS_PvPGetPvPState send;
 	send.dwRedCount = nAliveRed;
 	send.dwGreenCount = nAliveBlue;

 	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ʾɱ����ͼƬ
//-------------------------------------------------------------------------------------
static int SC_ShowKillerPictureInArena(lua_State* L)
{
	DWORD	dwRoleID		= (DWORD)lua_tonumber(L, 1);
	INT		nPicture_Num	= lua_tointeger(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	return 0;

 	tagNS_PvPNotifyKill send;
 	send.dwKillCount = nPicture_Num;

 	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ָ�������ʾ��������
//-------------------------------------------------------------------------------------
static int SC_ShowArenaEndFrame(lua_State* L)
{
	DWORD dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	CHAR* pMsgName	=	(CHAR*)lua_tostring(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	return 0;

 	tagNS_PvPResultInfo send;
	TObjRef<Util> pUtil = "Util"; 	
	MCREATE_MSG(pSend, sizeof(tagNS_PvPResultInfo) + strlen(pMsgName)* sizeof(TCHAR), NS_PvPResultInfo);
	_tcscpy(pSend->pData, pUtil->AnsiToUnicode(pMsgName));
 	pRole->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	return 0;
}

//-------------------------------------------------------------------------------------
// ������-�������ṩLUA���ýӿ�
// ��ָ���ļ�����ҷŻص�֮ǰ�ķ����У���ȷ������
//-------------------------------------------------------------------------------------
static int SC_ShowArenaRoomFrame(lua_State* L)
{
	

	DWORD dwSerialID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID1 	= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID2 	= (DWORD)lua_tonumber(L, 3);
	DWORD dwRoleID3 	= (DWORD)lua_tonumber(L, 4);
	DWORD dwRoleID4 	= (DWORD)lua_tonumber(L, 5);
	DWORD dwRoleID5 	= (DWORD)lua_tonumber(L, 6);
	DWORD dwRoleID6 	= (DWORD)lua_tonumber(L, 7);
	PvPMgr* pPvPMgr = g_AthleticsSystem.GetPvPMgrPtr();
	if (!P_VALID(pPvPMgr)) return 0;


	dwRoleID1 = (dwRoleID1 < 0)? 0 : dwRoleID1;
	dwRoleID2 = (dwRoleID2 < 0)? 0 : dwRoleID2;
	dwRoleID3 = (dwRoleID3 < 0)? 0 : dwRoleID3;
	dwRoleID4 = (dwRoleID4 < 0)? 0 : dwRoleID4;
	dwRoleID5 = (dwRoleID5 < 0)? 0 : dwRoleID5;
	dwRoleID6 = (dwRoleID6 < 0)? 0 : dwRoleID6;

	
 
 	AthleticsPvP* pAthleticsPvP = pPvPMgr->GetPvP(dwSerialID);
 	if (!P_VALID(pAthleticsPvP)) 
	{
		
		return 0;
	}    


 	RoomBase* pRoom = g_AthleticsSystem.GetRoomMgr().GetPvPRoom(dwSerialID);
	

 	if (!P_VALID(pRoom))
	{
		pAthleticsPvP->ClosePvP(TRUE);
		return 0;
	}   
	Athletics3V3Room* p3v3Room  = NULL;
	if ( pRoom->GetRoomType() == E_PvP_Room3v3)
	{
		p3v3Room = (Athletics3V3Room*)pRoom;
	}
	else
	{
		pAthleticsPvP->ClosePvP(TRUE);
		
		return 0; 
	}


	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		pAthleticsPvP->ClosePvP(TRUE);
		p3v3Room->SetRoomState(E_PvP_RoomStateNull);
		p3v3Room->SetIsPvPRoleEnterPvP(FALSE);
		return 0;

	}
 	p3v3Room->DeleteRoleFromPvP(dwRoleID1, dwRoleID2, dwRoleID3, dwRoleID4, dwRoleID5, dwRoleID6);
    
	//-------------------------------------------------------------------------------------------------------
	// �����б����Ϣ�޸�
	//-------------------------------------------------------------------------------------------------------
	tagPvPRoom*  pRoomlist = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo((INT16)p3v3Room->GetRoomID(),p3v3Room->GetRoomLevel());
	if ( P_VALID( pRoomlist ) )
	{
		pRoomlist->dwManagerID        = p3v3Room->GetMasterRoleID();
		pRoomlist->eRoomState         = p3v3Room->GetRoomState();
		pRoomlist->dwSerialID         = p3v3Room->GetRoomSerialID();
		_tcscpy(pRoomlist->szManagerName, p3v3Room->GetMasterName());
		pRoomlist->ucCurrentPersonNum = p3v3Room->GetRoomPersonNumber();
	}
	pAthleticsPvP->ClosePvP(TRUE);

	return 0;
}

//-------------------------------------------------------------------------------------
// ���ͻ��˷���Ϣ��֪ͨ��������
//-------------------------------------------------------------------------------------
static int SC_OpenTrain(lua_State* L)
{
	DWORD dwOwnerID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwTrainID		= (DWORD)lua_tonumber(L, 2);
	
	Role * pOwner = g_roleMgr.GetRolePtrByID(dwOwnerID);
	if (!P_VALID(pOwner))
	{
		return 0;
	}

	ETrainState eTrainState = pOwner->GetTrainState(dwTrainID);
	if (eTrainState == ETS_Close)
	{
		// ���ͻ��˷���Ϣ��֪ͨ��������
		tagNS_OpenTrain send;
		send.dwTrainID = dwTrainID;
		
		pOwner->SendMessage((LPVOID)(&send), send.dwSize);

		// ��������״̬
		pOwner->SaveTrainState(dwTrainID, ETS_Open);
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ���ָ�����������ֵ
// Ret����Ӧ�����������͵�����ֵ
//-------------------------------------------------------------------------------------
static int SC_GetClanRepValue(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	ECLanType   eClanType	=	(ECLanType)lua_tointeger(L, 2);

	if (eClanType < ECLT_XuanYuan || eClanType > ECLT_GongGong)
	{
		return 0;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	ClanData& refClanData = pRole->GetClanData();
	lua_pushinteger(L, refClanData.RepGetVal(eClanType));

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ�����ӵ�е������������ߵ����޵����ֵ��
// Ret��������ֵ�����û������ʱ����nil,�������ӵ����׷ֱ�Ϊ3��5��4����ֻ���ޣ��򷵻�ֵΪ5
//-------------------------------------------------------------------------------------
static int SC_GetPetStepGrade(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	PetPocket* pPetPocket = pRole->GetPetPocket();
	if (!P_VALID(pPetPocket))
	{
		return 0;
	}

	DWORD	dwPetID[CONST_MAX_PETSOUL_NUM] = {0};
	INT		nPetNum = 0;
	INT		nPetStep = 0;

	// ȡ�����������ID
	pPetPocket->GetAllPetID(dwPetID, (INT&)nPetNum);
	
	// ���û������ʱ����nil
	if (nPetNum <= 0)
	{
		return 0;
	}

	// ȡ�����������������ֵ
	for (INT nCnt = 0; nCnt < nPetNum; nCnt++)
	{
		PetSoul* pPetSoul = pPetPocket->GetPetSoul(dwPetID[nCnt]);
		if (!P_VALID(pPetSoul))
		{
			return 0;
		}
		PetAtt&	refPetAtt = pPetSoul->GetPetAtt();
		INT nCurPetStep = refPetAtt.GetStep();
		nPetStep = max(nCurPetStep, nPetStep);
	}
	
	lua_pushinteger(L, nPetStep);

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ��������е�һ�������е�װ�������ĵȼ���
// Ret�����ĵȼ�������һ������������Ʒ������Ʒ������װ�����򷵻�nil
//-------------------------------------------------------------------------------------
static int SC_GetFstBoxWpnPosyTimes(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// ��Ʒ/װ��������
	ItemMgr& refItemMgr = pRole->GetItemMgr();

	tagItem * pItem = refItemMgr.GetBagItem((INT16)0);
	if (!P_VALID(pItem))
	{
		return 0;
	}

	tagEquipProto *pProto = g_attRes.GetEquipProto(pItem->dwTypeID);
	// ����Ʒ������װ��
	if(!P_VALID(pProto))
	{
		return 0;
	}
	
	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->GetWpnPosyTimes());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ��������е�һ�������е�װ�����Կ̵ȼ���
// Ret���Կ̵ȼ�������һ������������Ʒ������Ʒ������װ�����򷵻�nil
//-------------------------------------------------------------------------------------
static int SC_GetFstBoxWpnEngraveTimes(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// ��Ʒ/װ��������
	ItemMgr& refItemMgr = pRole->GetItemMgr();

	tagItem * pItem = refItemMgr.GetBagItem((INT16)0);
	if (!P_VALID(pItem))
	{
		return 0;
	}

	tagEquipProto *pProto = g_attRes.GetEquipProto(pItem->dwTypeID);
	// ����Ʒ������װ��
	if(!P_VALID(pProto))
	{
		return 0;
	}

	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->GetWpnEngraveTimes());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ��������е�һ�������е�װ������Ƕ������
// Ret��װ����Ƕ����������һ������������Ʒ������Ʒ������װ�����򷵻�nil
//-------------------------------------------------------------------------------------
static int SC_GetFstBoxWpnEquipHoleCnt(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// ��Ʒ/װ��������
	ItemMgr& refItemMgr = pRole->GetItemMgr();

	tagItem * pItem = refItemMgr.GetBagItem((INT16)0);
	if (!P_VALID(pItem))
	{
		return 0;
	}

	tagEquipProto *pProto = g_attRes.GetEquipProto(pItem->dwTypeID);
	// ����Ʒ������װ��
	if(!P_VALID(pProto))
	{
		return 0;
	}

	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->GetWpnEquipHoleCnt());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ��ҵİ��ɹ�ѫֵ
// Ret����ҵİ��ɹ�ѫֵ
//-------------------------------------------------------------------------------------
static int SC_GetGuildExploit(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// �õ�����
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return 0;
	}

	// �õ����ɳ�Ա
	tagGuildMember* pMember = pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		return 0;
	}

	lua_pushnumber(L, pMember->nExploit);

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ����ȡ�������ɵ���������
// Ret���������������
//-------------------------------------------------------------------------------------
static int SC_GetPlayerTrainCnt(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetPassTrainCnt());

	return 1;
}

//-------------------------------------------------------------------------------------
// fn ���ж�����Ƿ���ĳ���ƺ�
// Ret������Ƿ���ĳ���ƺ�
//-------------------------------------------------------------------------------------
static int SC_IsRoleHaveTitle(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	UINT16		u16TitleID	=	(UINT16)lua_tointeger(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	TitleMgr* pTitleMgr = pRole->GetTitleMgr();
	if (!P_VALID(pTitleMgr))
	{
		return 0;
	}

	int nb = pTitleMgr->IsTitleObtained(u16TitleID) ? 1 : 0; 
	lua_pushboolean(L, nb);

	return 1;
}


//-------------------------------------------------------------------------------------
// �ı�ʦ�µ���
//-------------------------------------------------------------------------------------
static int SC_SetQinWuPoint(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nNum			=	(INT)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->SetQinWuPoint(nNum);

	// ͽ�ܸ���ʦ����Ϣ
	pRole->GetApprenticeData().ResetIterator();
	DWORD dwApprenticeID = GT_INVALID;
	while (pRole->GetApprenticeData().PeekNext(dwApprenticeID))
	{
		Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
		if (P_VALID(pApprentice))
		{
			pApprentice->GetClassMate().nQinWuPoint = nNum;
		}
	}

	// ���������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_QinWu;
	sendDB.sOneMAData.eMode = EMAMM_Set;		// �滻����
	*(INT*)(sendDB.sOneMAData.byData) = nNum;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ���ɱ��Ҿ���
//-------------------------------------------------------------------------------------
static int SC_GetKillObjExp(lua_State* L)
{
	INT		nLevel		=	(INT)lua_tointeger(L, 1);

	const tagKillerExp*	pProto = g_attRes.GetKillObjExpProto((INT16)nLevel);
	if (!P_VALID(pProto))
		return 0;

	lua_pushnumber(L, pProto->dwExp);
	return 1;
}

//-------------------------------------------------------------------------------------
// �ػ��񵹼�ʱ֪ͨ�ͻ���
//-------------------------------------------------------------------------------------
static int SC_EudemonSummonCountDown(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT32	nCountDown		=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_BeginSummon send;
	send.nCountDown = nCountDown;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// �޸Ļ��������
//-------------------------------------------------------------------------------------
static int SC_ModRankData(lua_State* L)
{ 
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
	INT16	n16DataType	        =	(INT16)lua_tonumber(L, 2);
	INT	    nModValue		    =	(INT)lua_tonumber(L, 3);
	if ( 0 == dwRoleID || !GT_VALID(dwRoleID) || 0 == nModValue )
	{
        return 0;
	}
	g_WorldExpo.ModRankingsData(dwRoleID,(E_ActiveDataType)(n16DataType-1),nModValue);
    
    return 0;
}

//-------------------------------------------------------------------------------------
// ��û��������
//-------------------------------------------------------------------------------------
static int SC_GetRoleRankData(lua_State* L)
{
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
	INT16	n16DataType      	=	(INT16)lua_tonumber(L, 2);
	if ( 0 == dwRoleID || !GT_VALID(dwRoleID) )
	{
		return 0;
	}
	INT32   nValue              =  g_WorldExpo.GetRoleRankingsData(dwRoleID,(E_ActiveDataType)(n16DataType-1));
    lua_pushnumber(L, nValue);
    return 1;
}

//-------------------------------------------------------------------------------------
// �����ʾ��������
//-------------------------------------------------------------------------------------
static int SC_GetDisplayRank(lua_State* L)
{
	DWORD	dwRoleID			=	(DWORD)lua_tonumber(L, 1);
	INT16	n16DataType     	=	(INT16)lua_tonumber(L, 2);
	INT  	nLowRankPos  		=	(INT)lua_tonumber(L, 3);
	INT 	nHighRankPos		=	(INT)lua_tonumber(L, 4);
	if ( 0 == dwRoleID || !GT_VALID(dwRoleID) )
	{
		return 0;
	}
    g_WorldExpo.GetRoleDisplayRankings(dwRoleID,(E_ActiveDataType)(n16DataType-1),nLowRankPos,nHighRankPos);


    return 0;
}

static int SC_CancelRoleStallState(lua_State* L)
{
     DWORD   dwMapId           = (DWORD)lua_tonumber(L,1);
	 DWORD   dwMapInstanceId   = (DWORD)lua_tonumber(L,2);
	 DWORD   dwRoleId          = (DWORD)lua_tonumber(L,3);
	 DWORD   dwError   =  0 ;
     Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleId);
	 if ( !P_VALID(pRole) )
	 {
		 dwError = 1;
	 }
	 else
	 {
		 if ( !pRole->GetMap())
		 {
             dwError  = 1;
		 }
		 else
		 {
	    	  dwError =  pRole->SendCloseStall();
		 }
	 }
	 lua_pushnumber(L,dwError);
	 return 1;
}


//-------------------------------------------------------------------------------------
// ���þ���ֵcdʱ��
//-------------------------------------------------------------------------------------
static int SC_RoleFixSpirit(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwCDTime		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->ModFixSpiriteCDTime(dwCDTime);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�����Ա�
//-------------------------------------------------------------------------------------
static int SC_GetRoleSex(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetSex());
	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���ҷ�װ���
//-------------------------------------------------------------------------------------
int SC_GetRoldDressid(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushinteger(L, pRole->GetAvatar()->wDressMdlID);
	return 1;
}

//-------------------------------------------------------------------------------------
// �õ���Ұ���ID
//-------------------------------------------------------------------------------------
int SC_GetRoleGuildID(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	lua_pushnumber(L, pRole->GetGuildID());

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ҿɼ�����������
//-------------------------------------------------------------------------------------
int SC_GetActiveLongHunNum(lua_State* L)
{
	lua_pushinteger(L, 0);
	return 1;

	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )
	{
		return 0;
	}

	// ȡ�ÿɼ�����������
	INT nActiveNum = pRole->GetLongHun().GetActiveNum();

	lua_pushinteger(L, nActiveNum);

	return 1;
}

//-------------------------------------------------------------------------------------
// ������������
//-------------------------------------------------------------------------------------
int SC_CastActiveLongHun(lua_State* L)
{
	return 0;
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwSkillID	= (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	// ȡ����Ҽ���
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (!P_VALID(pSkill))
	{
		return 0;
	}

	// ���ȡ���ɱ�������ID
	DWORD dwLongHunID = pRole->GetLongHun().GetRandomLongHunID();
	if (!GT_VALID(dwLongHunID))
	{
		return 0;
	}

	// ȡ����������
	const tagLongHunProto* pProto = g_attRes.GetLongHunProto(dwLongHunID);
	if (!P_VALID(pProto))
	{
		return 0;
	}

	if (pProto->eTypeEx == ELHTE_PowerGather)
	{
		return 0;
	}

	if (GT_VALID(pProto->dwBuffID1))
	{
		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID1);
		if (P_VALID(pBuffProto))
		{
			pRole->TryAddBuff(pRole, pBuffProto, NULL, pSkill, NULL);
		}
	}
	if (GT_VALID(pProto->dwBuffID2))
	{
		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(pProto->dwBuffID2);
		if (P_VALID(pBuffProto))
		{
			pRole->TryAddBuff(pRole, pBuffProto, NULL, pSkill, NULL);
		}	
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ���꼤��
//-------------------------------------------------------------------------------------
int SC_ProlifeLongHun(lua_State* L)
{
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwSkillID	= (DWORD)lua_tonumber(L, 2);
	BOOL bProlife	= (BOOL)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	// ȡ����Ҽ���
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (!P_VALID(pSkill))
	{
		return 0;
	}

	// ȡ�õ�ǰ���������Ѿ�����������buff
	Buff* pBuff = pRole->GetRelativeSkillBuff(dwSkillID);
	if (!P_VALID(pBuff))
	{
		return 0;
	}

	// ȡ����һ��Buff����
	const tagBuffProto* pProto = g_attRes.GetBuffProto(pBuff->GetTypeID()+1);
	if (!P_VALID(pProto))
	{
		return 0;
	}
	
	if (bProlife)
	{
		pRole->TryAddBuff(pRole, pProto, NULL, pSkill, NULL);
	}
	lua_pushboolean(L, true);

	return 1;
}

//-------------------------------------------------------------------------------------
// �ж�����Ƿ��ڰ�̯
//-------------------------------------------------------------------------------------
int SC_IsRoleOnStall(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	int nOnStall = FALSE;

	// �ж��Ƿ��Ѿ����ڰ�̯���̯����״̬
	if(pRole->IsInRoleStateAny(ERS_Stall | ERS_StallSet))
		nOnStall = TRUE;
	else
		nOnStall = FALSE;

	lua_pushboolean(L, nOnStall);

	return 1;
}

//-------------------------------------------------------------------------------------
// �������״̬
//-------------------------------------------------------------------------------------
int SC_SetRoleState(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	INT			nState		=	lua_tointeger(L, 2);
	BOOL		bSendMsg	=	(BOOL)lua_toboolean(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	if( nState <= 0 || nState >= ERS_End)	return 0;

	pRole->SetRoleState((ERoleState)nState, bSendMsg);

	if(ERS_Safeguard == nState)
	{
		// ���ظ��ͻ���
		tagNS_SafeGuard send;
		send.bOpen = TRUE;
		send.dwErrorCode = E_Success;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}

int SC_UnSetRoleState(lua_State* L)
{
	DWORD		dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	INT			nState		=	lua_tointeger(L, 2);
	BOOL		bSendMsg	=	(BOOL)lua_toboolean(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	if( nState <= 0 || nState >= ERS_End)	return 0;

	pRole->UnsetRoleState((ERoleState)nState, bSendMsg);

	if(ERS_Safeguard == nState)
	{
		// ���ظ��ͻ���
		tagNS_SafeGuard send;
		send.bOpen = FALSE;
		send.dwErrorCode = E_Success;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �ܷ�������
//-------------------------------------------------------------------------------------
int SC_CanOpenChest(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwChestID		=	(DWORD)lua_tonumber(L, 4);
	DWORD dwKeyID		=	(DWORD)lua_tonumber(L, 5);

// 	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
// 	if(!P_VALID(pMap))	return 0;
// 
// 	Role* pRole = pMap->FindRole(dwRoleID);
// 	if(!P_VALID(pRole))	return 0;
// 
// 	tagItemProto* pItemProto = g_attRes.GetItemProto(dwChestID);
// 	if (!P_VALID(pItemProto))
// 	{
// 		return 0;
// 	}
	// ���俪��ȥ������
	lua_pushboolean(L, true);

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ͻ��˷�����Ϣ
//-------------------------------------------------------------------------------------
int SC_SendChestMsg(lua_State* L)
{
	DWORD dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID		=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			=	(DWORD)lua_tonumber(L, 3);
	DWORD dwChestID			=	(DWORD)lua_tonumber(L, 4);
	const string strMsgName =	lua_tostring(L, 5);
	BOOL bOpened			=	(BOOL)lua_toboolean(L, 6);
	BOOL bDestroy			=	(BOOL)lua_toboolean(L, 7);
	DWORD dwItemID			=	(DWORD)lua_tonumber(L, 8);
	INT nItemNum			=	lua_tointeger(L, 9);
	DWORD dwErrorCode		=	(DWORD)lua_tonumber(L, 10);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ������Ϣ���ͻ���
	g_TreasureChestMgr.SendMsg2Client(pRole, dwChestID, strMsgName, bOpened, bDestroy, dwItemID, nItemNum, dwErrorCode);

	return 0;
}

//-------------------------------------------------------------------------------------
// ʹ���籦�俪�������ͽ�ɫ���俪��������1
//-------------------------------------------------------------------------------------
int SC_IncTreasureSum(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	g_worldSession.IncTreasureSum();
	pRole->IncTreasureSum();
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ������籦�俪�������ͽ�ɫ���俪������
//-------------------------------------------------------------------------------------
int SC_GetTreasureSum(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushinteger(L, pRole->GetTreasureSum());
	lua_pushinteger(L, g_worldSession.GetTreasureSum());

	return 2;
}

//-------------------------------------------------------------------------------------
// �õ���������Ʒ
//-------------------------------------------------------------------------------------
int SC_GetChestItem(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwChestID		=	(DWORD)lua_tonumber(L, 4);
	ERateType eRate		=	(ERateType)lua_tointeger(L, 5);
	FLOAT fRand			=	(FLOAT)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagChestItem* pItem = g_TreasureChestMgr.GetRandomItem(dwChestID, eRate, fRand);
	if (!P_VALID(pItem))	return 0;

	DWORD dwItemTypeID = pItem->dwTypeID;	// ������Ʒtypeid
	INT nNum = pItem->nNum;					// ������Ʒ����
	lua_pushnumber(L, dwItemTypeID);
	lua_pushinteger(L, nNum);

	return 2;
}

//-------------------------------------------------------------------------------------
// ��Ʒ�Ƿ���Ҫ�㲥
//-------------------------------------------------------------------------------------
int SC_ItemNeedBroadcast(lua_State* L)
{
	DWORD dwTypeID	=	(DWORD)lua_tonumber(L, 1);

	// �����װ��
	if (MIsEquipment(dwTypeID))
	{
		// �ҵ�װ��
		tagEquipProto* pProto = g_attRes.GetEquipProto(dwTypeID);
		if (!P_VALID(pProto))	return 0;

		lua_pushnumber(L, pProto->bNeedBroadcast);
		return 1;
	}
	
	// ����Ʒ
	else
	{
		// �ҵ���Ʒ
		tagItemProto* pProto = g_attRes.GetItemProto(dwTypeID);
		if (!P_VALID(pProto))	return 0;

		lua_pushnumber(L, pProto->bNeedBroadcast);
		return 1;
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// �õ���������Ʒ
//-------------------------------------------------------------------------------------
int SC_StopMount(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->StopMount();

	return 0;
}

//-------------------------------------------------------------------------------------
// ֪ͨ���ѽ��븱��
//-------------------------------------------------------------------------------------
int SC_InstanceNotify(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	BOOL	bNotify		=	(BOOL)lua_toboolean(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	if(bNotify == FALSE)	return 0;

	// �ҵ����С��
	const Team* pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
	if( !P_VALID(pTeam) ) return 0;

	// �������Ƿ񴴽��˸���
	Map* pMap = g_mapCreator.GetMap(pTeam->GetOwnInstanceMapID(), pTeam->GetOwnInstanceID());
	if( !P_VALID(pMap) || EMT_Instance != pMap->GetMapType() ) return 0;

	MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
	if( !P_VALID(pInstance) ) return 0;

	const tagInstance* pInstanceProto = pInstance->GetInstanceProto();
	if( !P_VALID(pInstanceProto) ) return 0;	

	// �����Ƿ�����֪ͨ����
	if( !pInstanceProto->bNoticeTeamate ) return 0;

	// ֪ͨ����
	tagNS_InstanceAgree	 send;
	send.dwInviterID	=	pRole->GetID();
	send.dwMapID		=	pInstance->GetMapID();
	g_groupMgr.SendTeamInstanceNotify(pRole, pRole->GetTeamID(), &send, send.dwSize);

	return 0;
}
//-------------------------------------------------------------------------------------
// �Ƿ��Ѿ����
//-------------------------------------------------------------------------------------
static int SC_IsRoleMarried(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);


	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	lua_pushboolean(L, pRole->IsRoleMarried());

	return 1;
}
//-------------------------------------------------------------------------------------
// �Ƿ�Ϊ����
//-------------------------------------------------------------------------------------
static int SC_IsCouple(lua_State* L)
{
	DWORD	dwRoleID1	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole1) )	return 0;

	if(pRole1->GetLoverID() == dwRoleID2)
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}
//-------------------------------------------------------------------------------------
// �Ƿ��Ѿ����й�����
//-------------------------------------------------------------------------------------
static int SC_IsRoleWedded(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) )	return 0;

	lua_pushboolean(L, pRole->IsRoleWedded());

	return 1;
}
//-------------------------------------------------------------------------------------
// �õ��Ѻö�(���ܶ�)
//-------------------------------------------------------------------------------------
static int SC_GetFriendShipValue(lua_State* L)
{
	DWORD	dwRoleID1	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole) )	return 0;

	tagFriend* pFriRole = pRole->GetFriendPtr(dwRoleID2);
	if( !P_VALID(pFriRole) )
	{
		lua_pushinteger(L, GT_INVALID);
	}
	else
	{
		lua_pushinteger(L, pFriRole->dwFriVal);
	}
	return 1;
}
//-------------------------------------------------------------------------------------
// �����Ѻö�
//-------------------------------------------------------------------------------------
static int SC_AddFriendShipValue(lua_State* L)
{
	DWORD	dwRoleID1	=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID2	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwAddFriVal	=	(DWORD)lua_tonumber(L, 3);

	if(dwAddFriVal <= 0)
		return 0;

	Role* pRole1 = g_roleMgr.GetRolePtrByID(dwRoleID1);
	if( !P_VALID(pRole1) )	return 0;

	Role* pRole2 = g_roleMgr.GetRolePtrByID(dwRoleID2);
	if( !P_VALID(pRole2) )	return 0;

	//�õ�role1������
	tagFriend* pRole1Fri = pRole1->GetFriendPtr(dwRoleID2);

	//�õ�role2������
	tagFriend* pRole2Fri = pRole2->GetFriendPtr(dwRoleID1);


	BOOL bInsert = TRUE;
	DWORD dwFriVal = 0;
	if(P_VALID(pRole1Fri))
	{
		if(pRole1Fri->dwFriVal > 0)
			bInsert = FALSE;

		dwFriVal = pRole1Fri->dwFriVal += dwAddFriVal;

		tagNS_UpdateFriValue send;
		send.dwRoleID = pRole1Fri->dwFriendID;
		send.nFriVal = dwFriVal;
		pRole1->SendMessage(&send, send.dwSize);

		//���³ƺ�	
		if(pRole1->GetLoverID() == pRole1Fri->dwFriendID)
			pRole1->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);

	}

	if(P_VALID(pRole2Fri))
	{
		if(pRole2Fri->dwFriVal > 0)
			bInsert = FALSE;

		dwFriVal = pRole2Fri->dwFriVal += dwAddFriVal;

		tagNS_UpdateFriValue send;
		send.dwRoleID = pRole2Fri->dwFriendID;
		send.nFriVal = dwFriVal;
		pRole2->SendMessage(&send, send.dwSize);

		
		//���³ƺ�	
		if(pRole2->GetLoverID() == pRole2Fri->dwFriendID)
			pRole2->GetTitleMgr()->SigEvent(ETE_MARRY_JOIN,  GT_INVALID, dwFriVal);
	}

	if(bInsert)
	{
		tagNDBC_InsertFriVal sendDB;
		sendDB.dwRoleID = (dwRoleID1 < dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.dwFriendID = (dwRoleID1 > dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.nFriVal = dwFriVal;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}
	else{
		tagNDBC_UpdateFriVal sendDB;
		sendDB.dwRoleID = (dwRoleID1 < dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.dwFriendID = (dwRoleID1 > dwRoleID2) ? dwRoleID1 : dwRoleID2;
		sendDB.FriendshipSave.nFriVal = dwFriVal;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}

	return 0;
}
//-------------------------------------------------------------------------------------
// ���ӳƺ�
//-------------------------------------------------------------------------------------
int SC_SigTitleEvent(lua_State* L)
{
	DWORD			dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD			dwArg1		= (DWORD)lua_tonumber(L, 2);


	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	BOOL bHas = pRole->GetTitleMgr()->SigEvent(ETE_SCRIPT, dwArg1, GT_INVALID);

	lua_pushboolean(L, bHas);

	return 1;
}
//-------------------------------------------------------------------------------------
// ɾ���ƺ�
//-------------------------------------------------------------------------------------
int SC_SigRemoveTitleEvent(lua_State* L)
{
	DWORD			dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD			titleEvent	= (DWORD)lua_tonumber(L, 2);
	DWORD			dwArg1		= (DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	pRole->GetTitleMgr()->SigRemoveEvent((EnumTitleEvent)titleEvent, dwArg1);

	return 1;
}
//-------------------------------------------------------------------------------------
// �ı似��������
//-------------------------------------------------------------------------------------
int SC_ChangeSkillExp(lua_State* L)
{
	DWORD			dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	DWORD			dwSkillID	=	(DWORD)lua_tonumber(L, 2);
	DWORD			dwSkillExp	=	(DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(dwRoleID))
	{
		return 0;
	}

	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if( !P_VALID(pSkill) )
	{
		return 0;
	}

	pRole->ChangeSkillExp(pSkill, dwSkillExp);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����Ƿ���ĳbuff
//-------------------------------------------------------------------------------------
int SC_IsRoleHaveBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffID		=	(DWORD)lua_tonumber(L, 4);
	BOOL	bRet			=	FALSE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	bRet = pRole->IsHaveBuff(dwBuffID);
	lua_pushboolean(L, bRet);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����������Ҹ����
//-------------------------------------------------------------------------------------
int SC_QuestSetRebornMap(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=	(UINT16)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_BindRebornMap send;
	send.dwErrorCode = pRole->QuestSetRebornMap(u16QuestID, send.dwBornMapID, send.vBornPos);
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// �״����߽���Ʒͬ�����ͻ���
//-------------------------------------------------------------------------------------
int SC_VirginOnlineSendClient(lua_State* L)
{
	DWORD	dwRoleID	=	(DWORD)lua_tonumber(L, 1);
	INT64	n64Serial1	=	pop_64bit_data(L, 2, 3);
	INT64	n64Serial2	=	pop_64bit_data(L, 4, 5);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	// ���ͻ��˷��ͳ���������Ϣ
	tagNS_VirginOnline send;
	send.n64Serial1 = n64Serial1;
	send.n64Serial2 = n64Serial2;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����ɹ���Ϣ���͸��ͻ���
//-------------------------------------------------------------------------------------
int SC_FishingSendClient(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_Fishing send;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+1+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		pRole->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �޸���ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_ModRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		pRole->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �����ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetRoleScriptData(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+3);

		if( nIndex < 0 || nIndex >= ESD_Role ) return 0;

		dwValue = pRole->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// �����ҽű�ͨ������
//-------------------------------------------------------------------------------------
int SC_CanMount(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;
	
	INT nRt = pRole->GetPetPocket()->CanRidePet();

	lua_pushinteger(L, nRt);

	return 1;
}

//-------------------------------------------------------------------------------------
// ���������������
//-------------------------------------------------------------------------------------
int SC_AddRoleExVolume(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT		nCmdID			=	lua_tointeger(L, 5);



	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	pRole->GetCurMgr().IncExchangeVolume(nValue, (ELogCmdID)nCmdID);

	return 0;
}


//-------------------------------------------------------------------------------------
// �ж�����״̬
//-------------------------------------------------------------------------------------
int SC_IsRoleInStatus(lua_State* L)
{
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwStatus	= (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	
	lua_pushboolean(L, pRole->IsInRoleStateAll(dwStatus));

	return 1;
}

//-------------------------------------------------------------------------------------
// �����ű�ͨ����Ϣ
//-------------------------------------------------------------------------------------
int SC_BeginMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	lua_pushnumber(L, dwMsgInfoID);

	return 1;
}

//-------------------------------------------------------------------------------------
// ����Ϣ���������¼�����
//-------------------------------------------------------------------------------------
int SC_AddMsgEvent(lua_State* L)
{
	DWORD			dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	EMsgUnitType	eMsgUnitType	=	(EMsgUnitType)lua_tointeger(L, 2);
	LPVOID			pData			=	NULL;

	if(EMUT_RoleName == eMsgUnitType)
	{
		LPCTSTR pData = IUTIL->AnsiToUnicode(lua_tostring(L, 3));
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, eMsgUnitType, (LPVOID)pData);
	}
	else
	{
		DWORD dwData = (DWORD)lua_tonumber(L, 3);
		g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, eMsgUnitType, &dwData);
	}

	return 0;
}


//-------------------------------------------------------------------------------------
// ���ͽű�ͨ����Ϣ�����
//-------------------------------------------------------------------------------------
int SC_DispatchRoleMsgEvent(lua_State* L)
{	
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, pRole);
	return 0;
}

//-------------------------------------------------------------------------------------
// �����������е�ͼ�ڵ���ҷ��ͽű�ͨ����Ϣ
//-------------------------------------------------------------------------------------
int SC_DispatchWorldMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);

	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);

	return 0;
}

int SC_DispatchMapMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pMap);

	return 0;
}


//-------------------------------------------------------------------------------------
// ���ð��ɵĳ�Ա���ű���Ϣ
//-------------------------------------------------------------------------------------
int SC_DispatchGuildMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwGuildID		=	(DWORD)lua_tonumber(L, 2);

	Guild *pGuild = g_guildMgr.GetGuild(dwGuildID);

	if(!P_VALID(pGuild))	return 0;

	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pGuild);

	return 0;

}

//-------------------------------------------------------------------------------------
// ����ս����սͬ�˹���ĳ�Ա���ű���Ϣ
//-------------------------------------------------------------------------------------
int SC_DispatchAllianceGuildMsgEvent(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwAllianceType	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwGodID			=	(DWORD)lua_tonumber(L, 3);

	if (dwGodID >= EGM_MAX || dwGodID < EGM_NH)
		return 0;

	if (dwAllianceType == 1 || dwAllianceType == 2)
	{
		CGodMiracleMgr::GetInst()->GetGodMiracle(static_cast<EGodMiracle>(dwGodID))->SendAllianceGuildMsg(dwMsgInfoID, dwAllianceType);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ����ָ��ID�ķ������㲥
//-------------------------------------------------------------------------------------
int SC_DispatchBroadcast(lua_State* L)
{
	DWORD	dwMsgInfoID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwGuildID		=	(DWORD)lua_tonumber(L, 4);

	// ���ò���Ϊ-1��������㲥Ϊȫ���㲥
	if (dwMapID == -1)
	{
		g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
	}
	else if (dwMapID == -2)
	{
		// dwMapID == -2���ɹ㲥,���ɲ����ڣ��㲥�����š�
		Guild *pGuild = g_guildMgr.GetGuild(dwGuildID);
		if(!P_VALID(pGuild))	return 0;

		g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pGuild);
	}
	else
	{
		// ���ò�����Ϊ-1ʱ���򲥷�CRCֵ��ò�����ȵĳ����ĳ����㲥;�����ƹ㲥��Χ�Ĳ�����Ϊ-1�����Ҳ������ֵ��Ӧ�ĵ�ͼ��CRCʱ��������㲥���Ქ��
		Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
		if(!P_VALID(pMap))	return 0;

		g_MsgInfoMgr.DispatchMapMsgEvent(dwMsgInfoID, pMap);
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// ְҵϵͳ�ӿ�
//-------------------------------------------------------------------------------------
int SC_IsRoleInaugural(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwVocation		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	INT nVocation = (INT)(pRole->GetClass());
	DWORD dwTmpStr = 1 << (nVocation-1);
	if( dwTmpStr & dwVocation )
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, 0);

	return 1;
}

int SC_ChangeClass(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD dwNPCID = (DWORD)lua_tonumber(L, 2);
	INT nVocation = (INT)lua_tonumber(L, 3);
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	pRole->ChangeVocation(dwNPCID, (EClassType)nVocation);
	
	return 0;
}

//-------------------------------------------------------------------------------------
// �̶���ӿ�
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// ��ʼ���ʱ���¼��Ĵ���ʱ��
//-------------------------------------------------------------------------------------
int SC_InitEventTime(lua_State* L)
{
	const CHAR* szTableName		=   lua_tostring(L, 1);			// ���ʱ���table������
	INT			nNum			=   lua_tointeger(L, 2);		// ʱ���ĸ���
	DWORD		dwID			=   (DWORD)lua_tonumber(L, 3);			// �ID

	if(nNum <= 0)	return 0;

	CActivity *pActivity = g_activityMgr.GetActivity(dwID);
	if(!P_VALID(pActivity))	return 0;

	for(INT i = 1; i <= nNum; ++i)
	{
		lua_getglobal(L, szTableName);
		lua_pushnumber(L, i);
		lua_gettable(L, -2);
		DWORD	dwTime = lua_tointeger(L, -1);

		pActivity->AddEventTime(dwTime);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��Ƿ�ʼ
//-------------------------------------------------------------------------------------
int SC_GetActIsStart(lua_State* L)
{
	BOOL		bStart		=	FALSE;
	DWORD		dwID		=	(DWORD)lua_tonumber(L, 1);			//�ID

	CActivity* pActivity = g_activityMgr.GetActivity(dwID);
	if( !P_VALID(pActivity) )	return 0;

	if( pActivity->IsStart() ) bStart = TRUE;

	lua_pushboolean(L, bStart);
	return 1;
}

//-------------------------------------------------------------------------------------
// ��Ƿ�ʼ
//-------------------------------------------------------------------------------------
int SC_AddAllRoleBuff(lua_State* L)
{
	DWORD	dwBuffTypeID		=	(DWORD)lua_tonumber(L, 1);
	tagAllRoleBuff 	AllRoleBuff;
	AllRoleBuff.dwBuffTypeID = dwBuffTypeID;
	g_groupMgr.AddEvent(GT_INVALID, EVT_AddAllRoleBuff, sizeof(tagAllRoleBuff), &AllRoleBuff);

	return 0;
}

//-------------------------------------------------------------------------------------
// ���ù̶���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetActScriptData(lua_State* L)
{
	DWORD	dwActID			=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ��
	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+3);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+1+3);

		pActivity->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ù̶���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetActScriptData(lua_State* L)
{
	DWORD	dwActID			=	(DWORD)lua_tonumber(L, 1);
	INT		nParamNum		=	lua_tointeger(L, 2);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ��
	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) ) return 0;

	if( nParamNum <= 0 ) return 0;

	std::vector<INT> vec;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+3);
		vec.push_back(nIndex);
	}

	std::vector<INT>::iterator it = vec.begin();
	while (it != vec.end())
	{
		dwValue = pActivity->GetScriptData(*it);
		lua_pushnumber(L, dwValue);
		++it;
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ����̶���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SaveActScriptData(lua_State* L)
{
	DWORD	dwActID			=	(DWORD)lua_tonumber(L, 1);

	CActivity* pActivity = g_activityMgr.GetActivity(dwActID);
	if( !P_VALID(pActivity) )	return 0;
		
	pActivity->SaveActivityData2DB();

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�����ռ���񼣵ĸ��� 0:ûռ�� 
//-------------------------------------------------------------------------------------
static int SC_GetGuildCaptureGodMiracleNum(lua_State* L)
{
	DWORD	dwGuild			=	(DWORD)lua_tonumber(L, 1);

	// �񼣻д��
	CActivity* pActive = NULL;
	pActive	= g_activityMgr.GetActivity(GUILBATTLE_ACTIVITY_ID);
	if ( NULL != pActive)
	{
		lua_pushnumber(L, pActive->GetGuildCpatureGodMiracleNum(dwGuild));
	}
	else
		lua_pushnumber(L, 0);
	
	return 1;
}

//-------------------------------------------------------------------------------------
// �õ��ᱦ���һ����ÿһ���Ƿ񿪷ŵ�״̬
//-------------------------------------------------------------------------------------
static int SC_GetDuoBaoQiBingOpenState(lua_State* L)
{
	for (DWORD dwWeekDay = 0; dwWeekDay < DAYS_ONE_WEEK; dwWeekDay++)
	{
		lua_pushnumber(L, g_worldSession.GetDBQBOpenState(dwWeekDay));
	}

	return DAYS_ONE_WEEK;
}

//-------------------------------------------------------------------------------------
// �������С��
//-------------------------------------------------------------------------------------
int SC_CreateTeam(lua_State* L)
{
	DWORD	dwSrcRoleID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwDesRoleID			=	(DWORD)lua_tonumber(L, 2);

	tagCreateTeam	CreateTeam;
	CreateTeam.dwSrcRoleID = dwSrcRoleID;
	CreateTeam.dwDesRoleID = dwDesRoleID;
	g_groupMgr.AddEvent(GT_INVALID, EVT_CreateTeam, sizeof(tagCreateTeam), &CreateTeam);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����һ����ҵ����С��
//-------------------------------------------------------------------------------------
int SC_AddTeamMember(lua_State* L)
{
	DWORD	dwTeamID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwDesRoleID		=	(DWORD)lua_tonumber(L, 2);

	tagAddTeamMember	AddTeamMem;
	AddTeamMem.dwTeamID = dwTeamID;
	AddTeamMem.dwDesRoleID = dwDesRoleID;

	g_groupMgr.AddEvent(GT_INVALID, EVT_AddMember, sizeof(tagAddTeamMember), &AddTeamMem);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ�С�Ӷӳ���ID
//-------------------------------------------------------------------------------------
int SC_GetTeamLeaderID(lua_State* L)
{
	DWORD	dwTeamID		=	(DWORD)lua_tonumber(L, 1);

	const Team*	pTeam		=	g_groupMgr.GetTeamPtr(dwTeamID);
	if ( !P_VALID(pTeam) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	lua_pushnumber(L, pTeam->GetMemID(0));

	return 1;
}


//-------------------------------------------------------------------------------------
// ���õ�ͼ�ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetMapScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	INT		dwInstanceID	=	lua_tointeger(L, 2);
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+4);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+1+4);

		pMap->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��õ�ͼ�ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_GetMapScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	INT		nParamNum		=	lua_tointeger(L, 3);
	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	if( nParamNum <= 0 ) return 0;

	std::vector<INT> vec;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+4);
		vec.push_back(nIndex);
	}

	std::vector<INT>::iterator it = vec.begin();
	while (it != vec.end())
	{
		dwValue = pMap->GetScriptData(*it);
		lua_pushnumber(L, dwValue);
		++it;
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ��ͼ��̬ˢ������ײ����
//-------------------------------------------------------------------------------------
int SC_MapCreateCreature(lua_State* L)
{
 	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwTypeID		=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fx				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT	fy				=	(FLOAT)lua_tonumber(L, 5);
	FLOAT	fz				=	(FLOAT)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Vector3 vPos;
	vPos.x = fx * TILE_SCALE;
	vPos.y = fy;
	vPos.z = fz * TILE_SCALE;

	// ������ﳯ��
	Vector3 vFaceTo;
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
	vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.y = 0.0f;

	Creature *pCreature = pMap->CreateCreature(dwTypeID, vPos, vFaceTo);

	if (P_VALID(pCreature))
	{
		lua_pushnumber(L, pCreature->GetID());
		return 1;
	}

	lua_pushnumber(L, GT_INVALID);
	return 1;

}

//-------------------------------------------------------------------------------------
// ��̬ˢ����ײ����
//-------------------------------------------------------------------------------------
int SC_MapCreateColCreature(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwTypeID		=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fx				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT	fy				=	(FLOAT)lua_tonumber(L, 5);
	FLOAT	fz				=	(FLOAT)lua_tonumber(L, 6);
	BOOL	bCollide		=	lua_toboolean(L, 7);
	CHAR*	pPatrolListName	=   const_cast<CHAR*>(lua_tostring(L, 8));


	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Vector3 vPos;
	vPos.x = fx * TILE_SCALE;
	vPos.y = fy;
	vPos.z = fz * TILE_SCALE;

	// ������ﳯ��
	Vector3 vFaceTo;
	FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
	vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
	vFaceTo.y = 0.0f;

	Creature *pCreature = pMap->CreateCreature(dwTypeID, vPos, vFaceTo, GT_INVALID, bCollide, pPatrolListName);

	if (P_VALID(pCreature))
	{
		lua_pushnumber(L, pCreature->GetID());
		return 1;
	}

	lua_pushnumber(L, GT_INVALID);
	return 1;
}

//-------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------
int SC_MapDeleteCreature(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwID			=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	pMap->DeleteCreature(dwID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------
int SC_CreateInstance(lua_State* L)
{
	DWORD	dwMapID 		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr))		return 0;

	BOOL	bRet = pMapMgr->CreateScriptInstance(dwInstanceID);

	lua_pushboolean(L, bRet);

	return 1;	
}

//-------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------
int SC_DeleteInstance(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr))		return 0;

	MapInstance* pInstance = pMapMgr->GetInstance(dwInstanceID);
	if( !P_VALID(pInstance))			return 0;

	pInstance->SetDelete();
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ���ų�����Ч
//-------------------------------------------------------------------------------------
int SC_PlaySceneEffect(lua_State* L)
{
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID		= (DWORD)lua_tonumber(L, 2);
	ESceneEffectType eType	= (ESceneEffectType)lua_tointeger(L, 3);
	DWORD dwObjID			= (DWORD)lua_tonumber(L, 4);
	DWORD dwEffectID		= (DWORD)lua_tonumber(L, 5);
	FLOAT fPosX				= (FLOAT)lua_tonumber(L, 6);
	FLOAT fPosY				= (FLOAT)lua_tonumber(L, 7);
	FLOAT fPosZ				= (FLOAT)lua_tonumber(L, 8);

	Vector3 vPos(fPosX, fPosY, fPosZ);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->PlaySceneEffect(eType, dwObjID, vPos, dwEffectID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ֹͣ������Ч
//-------------------------------------------------------------------------------------
int SC_StopSceneEffect(lua_State* L)
{
	DWORD dwMapID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwObjID		= (DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->StopSceneEffect(dwObjID);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��/�رյ�ͼ��ָ������
//-------------------------------------------------------------------------------------
int SC_OpenCloseDoor(lua_State* L)
{
	DWORD dwMapID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwObjID		= (DWORD)lua_tonumber(L, 3);
	BOOL bOpen			= lua_toboolean(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->OpenCloseDoor(dwObjID, bOpen);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��/�رյ�ͼ�����е���
//-------------------------------------------------------------------------------------
int SC_OpenCloseAllDoors(lua_State* L)
{
	DWORD dwMapID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	= (DWORD)lua_tonumber(L, 2);
	BOOL bOpen			= lua_toboolean(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	pMap->OpenCloseAllDoors(bOpen);

	return 0;
}

//-------------------------------------------------------------------------------------
// �õ���ͼ�������ŵ�λ��
//-------------------------------------------------------------------------------------
int SC_GetDoorPos(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwMapID);
	if (!P_VALID(pInfo))
		return 0;

	tagMapDoor* pDoor = NULL;
	pInfo->mapDoor.ResetIterator();
	INT nTableNum = 0;
	while (pInfo->mapDoor.PeekNext(pDoor))
	{
		lua_newtable(L);
		lua_pushnumber(L, pDoor->vPos.x / TILE_SCALE);
		lua_rawseti(L, -2, nTableNum*3 + 1);

		lua_pushnumber(L, pDoor->vPos.y);
		lua_rawseti(L, -2, nTableNum*3 + 2);

		lua_pushnumber(L, pDoor->vPos.z / TILE_SCALE);
		lua_rawseti(L, -2, nTableNum*3 + 3);

		++nTableNum;
	}

	return pInfo->mapDoor.Size();
}

//-------------------------------------------------------------------------------------
// �򿪻�رյ�ͼ��ָ��λ�õ���
//-------------------------------------------------------------------------------------
int SC_OpenCloseDoorByPos(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	FLOAT	x				=	(FLOAT)lua_tonumber(L, 3);
	FLOAT	y				=	(FLOAT)lua_tonumber(L, 4);
	FLOAT	z				=	(FLOAT)lua_tonumber(L, 5);
	BOOL	bOpen			=	lua_toboolean(L, 6);

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwMapID);
	if (!P_VALID(pInfo))
		return 0;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	tagMapDoor* pDoor = NULL;
	pInfo->mapDoor.ResetIterator();
	while (pInfo->mapDoor.PeekNext(pDoor))
	{
		FLOAT fDist = Vec3Dist( Vector3( pDoor->vPos.x / TILE_SCALE, pDoor->vPos.y, pDoor->vPos.z / TILE_SCALE ), Vector3(x, y, z));
		if (fDist < 10)
		{
			pMap->OpenCloseDoor(pDoor->dwObjID, bOpen);
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ��ͼ�������ID
//-------------------------------------------------------------------------------------
int SC_GetMapAllRoleID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
	{
		return 0;
	}

	lua_newtable(L);
	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	INT n = 0;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		lua_pushnumber(L, pRole->GetID());
		lua_rawseti(L, -2, ++n);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ȡ��ͼ�������ID
//-------------------------------------------------------------------------------------
int SC_IsCreateMap(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if (!P_VALID(pMap))
		lua_pushboolean(L, 0);
	else
		lua_pushboolean(L,1);

	return 0;
}

//-------------------------------------------------------------------------------------
// ȡһ�������Ұ�ⳡ�����ǰ�ȫ����,ȡ������ﲢ�����Ա߷ŵ���һ����Ʒ
//-------------------------------------------------------------------------------------
static int SC_PutItemNearRandCreature(lua_State* L)
{
	DWORD dwItemID  = (DWORD)lua_tonumber(L, 1);

	DWORD dwRandMapID;
	if(rand()%2 == 0)
		dwRandMapID = TObjRef<Util>()->Crc32(_T("s03"));
	else
		dwRandMapID = TObjRef<Util>()->Crc32(_T("s04"));

	Map* pMap = g_mapCreator.GetMap(dwRandMapID, 0);
	if (!P_VALID(pMap))
		return 0;

	Vector3 vPos;
	INT64 n64Serial = 0;
	INT64 n64GroupID = 0;
	if (pMap->PutItemNearRandCreature(dwItemID, vPos, n64Serial, n64GroupID) != E_Success)
	{
		return 0;
	}
	else
	{
		lua_pushnumber(L, dwRandMapID);
		lua_pushnumber(L, vPos.x/TILE_SCALE);
		lua_pushnumber(L, vPos.z/TILE_SCALE);
		push_64bit_data(L, n64Serial);
		push_64bit_data(L, n64GroupID);
		return 7;
	}
}

static int SC_RemoveGroundItem(lua_State* L)
{
	DWORD dwMapID	  = (DWORD)lua_tonumber(L, 1);
	INT64 n64Serial   = pop_64bit_data(L, 2, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, 0);
	if (!P_VALID(pMap))
		return 0;	
	pMap->RemoveGroundItem(n64Serial);
	return 0;
}

//-------------------------------------------------------------------------------------
// ����ͼ���е��������һ��buff
//-------------------------------------------------------------------------------------
int SC_AddMapRoleBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwBuffTypeID	=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffTypeID);
	if( !P_VALID(pProto) ) return 0;

	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		pRole->TryAddBuff(pRole, pProto, NULL, NULL, NULL);
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ����Ƿ���ĳһ�ű����͵�����
//-------------------------------------------------------------------------------------
int SC_IsInArea(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwObjID			=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	const tagMapInfo* pMapInfo = pMap->GetMapInfo();
	if( !P_VALID(pMapInfo) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if( !P_VALID(pRole) )	
	{
		lua_pushboolean(L, false);
		return 1;
	}

	// �õ���ҵİ����к͸�������
	AABBox roleBox = pRole->GetAABBox();
	INT nTileX = INT(pRole->GetCurPos().x / (FLOAT)TILE_SCALE);
	INT nTileZ = INT(pRole->GetCurPos().z / (FLOAT)TILE_SCALE);

	tagMapAreaInfo* pArea = NULL;

	// ���ű���
	pArea = pMap->IsInArea(pMapInfo->mapScriptArea, roleBox, nTileX, nTileZ);
	if( !P_VALID(pArea) )
	{
		lua_pushboolean(L, false);
		return 1;
	}

	if (pArea->dwObjID == dwObjID)
	{
		lua_pushboolean(L, true);
	}
	else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// ��ù�����Χ�Ĺ���
//-------------------------------------------------------------------------------------
int SC_GetAroundCreature(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fOPRadius		=   (FLOAT)lua_tonumber(L, 4);
	FLOAT	fHigh			=	(FLOAT)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	std::vector<DWORD> vecCreature;
	INT nCreatureNum = 0;
	INT Index = 1;
	fOPRadius = fOPRadius * TILE_SCALE;
	nCreatureNum = pCreature->GetAroundCreature(vecCreature, fOPRadius, fHigh);

	lua_createtable(L,nCreatureNum,0);

	std::vector<DWORD>::iterator it = vecCreature.begin();
	while(it != vecCreature.end())
	{
		lua_pushnumber(L, Index);
		lua_pushnumber(L, *it);
		lua_settable(L,-3);  
		++it;
		++Index;
	}

	return 1;
};

//-------------------------------------------------------------------------------------
// ��ù�����Χ�����
//-------------------------------------------------------------------------------------
int SC_GetAroundRole(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	FLOAT	fOPRadius		=   (FLOAT)lua_tonumber(L, 4);
	FLOAT	fHigh			=	(FLOAT)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		return 0;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) )
	{
		return 0;
	}

	std::vector<DWORD> vecRole;
	INT nRoleNum = 0;
	INT Index = 1;
	fOPRadius = fOPRadius * TILE_SCALE;
	nRoleNum = pCreature->GetAroundRole(vecRole, fOPRadius, fHigh);

	lua_createtable(L,nRoleNum,0);

	std::vector<DWORD>::iterator it = vecRole.begin();
	while(it != vecRole.end())
	{
		lua_pushnumber(L, Index);
		lua_pushnumber(L, *it);
		lua_settable(L,-3);  
		++it;
		++Index;
	}

	return 1;
}

//-------------------------------------------------------------------------------------------
// ��ù���Ľű�����
//-------------------------------------------------------------------------------------------
int SC_GetCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// �õ���������
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex = lua_tointeger(L, i+5);
		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		dwValue = pCreature->GetScriptData(nIndex);
		lua_pushnumber(L, dwValue);
	}

	return nParamNum;
}

//-------------------------------------------------------------------------------------
// ���ù���ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_SetCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ��������
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+5);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+6);

		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		pCreature->SetScriptData(nIndex, dwValue);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �ı����ű�ͨ������
//-------------------------------------------------------------------------------------
int SC_ModCreatureScriptData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nParamNum		=	lua_tointeger(L, 4);

	INT		nIndex			=	0;
	DWORD	dwValue			=	GT_INVALID;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ��������
	if( nParamNum <= 0 ) return 0;

	for(INT i = 0; i < nParamNum; ++i)
	{
		nIndex		=	lua_tointeger(L, i*2+5);
		dwValue		=	(DWORD)lua_tonumber(L, i*2+6);

		if( nIndex < 0 || nIndex >= ESD_Creature ) return 0;

		pCreature->ModScriptData(nIndex, dwValue);
	}

	return 0;
}

//------------------------------------------------------------------------------------------
// ���ù�����½ű�AI��ʱ��������tickΪ��λ
//------------------------------------------------------------------------------------------
int SC_SetCreatureUpdateAITimer(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT		nTimer			=	lua_tointeger(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	if( nTimer <= 0 && nTimer != GT_INVALID ) return 0;

	pCreature->GetAI()->SetScriptUpdateTimer(nTimer);

	return 0;
}

//------------------------------------------------------------------------------------------
// ����ʹ�ü���
//------------------------------------------------------------------------------------------
int SC_CreatureUseSkill(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwSkillID		=	(DWORD)lua_tonumber(L, 4);
	INT		nTargetType		=	lua_tointeger(L, 5);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// �õ�Ŀ��ID
	DWORD dwTargetID = pCreature->GetAI()->GetTargetIDByType((ECreatureTargetFriendEnemy)nTargetType);

	// ʹ�ü���
	pCreature->GetAI()->AIUseSkill(dwSkillID, dwTargetID);

	return 0;
}

//---------------------------------------------------------------------------------------------
// ���ﺰ��
//----------------------------------------------------------------------------------------------
int SC_MonsterSay(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwSayID			=	(DWORD)lua_tonumber(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ����
	pCreature->Say(dwSayID);

	return 0;
}

//----------------------------------------------------------------------------------------------
// ���ﲥ�Ŷ���
//----------------------------------------------------------------------------------------------
int SC_MonsterPlayAction(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	const CHAR*	szFourCC		=	lua_tostring(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) ) return 0;

	// ����Ƿ�Ϸ���fourCCֻ�����ĸ���С���ַ�
	if( !P_VALID(szFourCC) || strlen(szFourCC) != 4 ) return 0;

	// ת����ID
	DWORD dwActionID = Cool3D::_MakeFourCCA(szFourCC);

	pCreature->PlayerAction(dwActionID);

	return 0;
}

//--------------------------------------------------------------------------------------------------
// �����л�AI״̬
//--------------------------------------------------------------------------------------------------
int SC_CreatureChangeAIState(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	INT			nState			=	lua_tointeger(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// �л�״̬
	pCreature->GetAI()->ChangeState((AIStateType)nState);

	return 0;
}

//--------------------------------------------------------------------------------------------------
// �������ս�����ʱ��
//--------------------------------------------------------------------------------------------------
int SC_GetEnterCombatTick(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	// �õ�����ս��ʱ��
	DWORD dwTick = pCreature->GetAI()->GetEnterCombatTick();

	lua_pushnumber(L, dwTick);

	return 1;
}

//--------------------------------------------------------------------------------------------------
// ���ݹ���ID�õ�TypeID
//--------------------------------------------------------------------------------------------------
int SC_GetCreatureTypeID(lua_State* L)
{
	DWORD		dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD		dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	DWORD dwTypeID = pCreature->GetTypeID();

	lua_pushnumber(L, dwTypeID);

	return 1;
}

//-------------------------------------------------------------------------------------
// ���ӹ�����
//-------------------------------------------------------------------------------------
int SC_AddEnmity(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwValue			=	(DWORD)lua_tonumber(L, 5);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;

	pCreature->GetAI()->AddEnmity(pTarget, dwValue);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ���ﵱǰ�Ĺ���Ŀ��
//-------------------------------------------------------------------------------------
int SC_GetCreatureCurTargetID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) )
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) 
	{
		lua_pushnumber(L, GT_INVALID);
		return 1;
	}

	DWORD	dwTargetID = pCreature->GetAI()->GetTargetUnitID();

	lua_pushnumber(L, dwTargetID);

	return 1;
}

//-------------------------------------------------------------------------------------
// �������������辭��
//-------------------------------------------------------------------------------------
int SC_GetRoleLevelUpExp(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ��õ�ǰ�������辭��
	const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(pRole->GetLevel());
	if (!P_VALID(pEffect))	return 0;

	INT32 nHigh = pEffect->nExpLevelUp/1000000000;
	INT32 nLow = pEffect->nExpLevelUp % 1000000000;

	lua_pushinteger(L, nHigh);
	lua_pushinteger(L, nLow);
	return 2;
}

//-------------------------------------------------------------------------------------
// ����Ƿ�����
//-------------------------------------------------------------------------------------
int SC_IsRoleOnLine(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
	{
		lua_pushinteger(L, 0);
	}
	else
	{
		lua_pushinteger(L, 1);
	}

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ���ɳ�Աְλ
//---------------------------------------------------------------------------------
static int SC_GetGuildPos(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// ������
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// ���ڰ���
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return 0;
	}

	tagGuildMember* pMember = pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		// ���ڰ�����
		return 0;
	}

	lua_pushinteger(L, pMember->eGuildPos);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ�����ʽ�
//---------------------------------------------------------------------------------
static int SC_GetGuildFund(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nFund);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ�����ʲ�
//---------------------------------------------------------------------------------
static int SC_GetGuildMaterial(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nMaterial);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ���ɰ�����
//---------------------------------------------------------------------------------
static int SC_GetGuildPeace(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().n16Peace);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ��������
//---------------------------------------------------------------------------------
static int SC_GetGuildReputation(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushnumber(L, pGuild->GetGuildAtt().nReputation);

	return 1;
}
//---------------------------------------------------------------------------------
// ������״̬
//---------------------------------------------------------------------------------
static int SC_IsGuildInStatus(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwStatus	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	lua_pushboolean(L, pGuild->IsInGuildStateAll(dwStatus));

	return 1;
}

//---------------------------------------------------------------------------------
// ���ð���״̬
//---------------------------------------------------------------------------------
static int SC_SetGuildStatus(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	INT	nStatus		= (INT)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	pGuild->SetGuildState((EGuildSpecState)nStatus);
	return 0;
}

//---------------------------------------------------------------------------------
// ȡ������״̬
//---------------------------------------------------------------------------------
static int SC_UnsetGuildStatus(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	INT nStatus		= (INT)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	pGuild->UnsetGuildState((EGuildSpecState)nStatus);
	return 0;
}

// �����ʽ���
//---------------------------------------------------------------------------------
static int SC_ModifyGuildFund(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nFund			= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (nFund == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nFund > 0)
	{
		pGuild->IncGuildFund(dwRoleID, nFund, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildFund(dwRoleID, -nFund, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// �����ʲı��
//---------------------------------------------------------------------------------
static int SC_ModifyGuildMaterial(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nMaterial		= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (nMaterial == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nMaterial > 0)
	{
		pGuild->IncGuildMaterial(dwRoleID, nMaterial, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildMaterial(dwRoleID, -nMaterial, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɰ����ȱ��
//---------------------------------------------------------------------------------
static int SC_ModifyGuildPeace(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT16 n16Peace		= (INT16)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (n16Peace == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (n16Peace > 0)
	{
		pGuild->IncGuildPeace(dwRoleID, n16Peace, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildPeace(dwRoleID, -n16Peace, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɳ�Ա�������
//---------------------------------------------------------------------------------
static int SC_ModifyGuildReputation(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nReputation	= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	if (nReputation == 0)
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if (nReputation > 0)
	{
		pGuild->IncGuildReputation(dwRoleID, nReputation, dwLogCmdID);
	}
	else
	{
		pGuild->DecGuildReputation(dwRoleID, -nReputation, dwLogCmdID);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɳ�Ա���ױ��
//---------------------------------------------------------------------------------
static int SC_ModifyContribution(lua_State* L)
{
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	INT32 nContribute	= (INT32)lua_tonumber(L, 3);
	DWORD dwLogCmdID	= (DWORD)lua_tointeger(L, 4);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// ������
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// ���ڰ���
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return 0;
	}

	if (nContribute > 0)
	{
		pGuild->ChangeMemberContribution(dwRoleID, nContribute, TRUE);
	}
	else
	{
		pGuild->ChangeMemberContribution(dwRoleID, -nContribute, FALSE);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// ���ɳ�Ա��ѫ���
//---------------------------------------------------------------------------------
static int SC_ModifyMemberExploit(lua_State* L)
{
	DWORD dwGuildID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 2);
	INT32 nExploit		=	lua_tointeger(L, 3);

	Guild* pGuild = NULL;
	if (!GT_VALID(dwGuildID))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pRole))
		{
			// ������
			return 0;
		}
		dwGuildID = pRole->GetGuildID();
		if (!GT_VALID(dwGuildID))
		{
			// ���ڰ���
			return 0;
		}
	}
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return 0;
	}

	pGuild->ChangeMemberExploit(dwRoleID, nExploit);

	return 0;
}


//---------------------------------------------------------------------------------
//�������Ƿ����
//---------------------------------------------------------------------------------
static int SC_IsHaveGuild(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
		lua_pushboolean(L, FALSE);
	else
		lua_pushboolean(L, TRUE);

	return 1;
}

//---------------------------------------------------------------------------------
// �����а��������߳�Ա���BUFF
//---------------------------------------------------------------------------------
static int SC_AddBuffToAllGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwBuffID	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	pGuild->GetMemberMgr().AddBuffToAllOnLineMember(dwBuffID);

	return 0;
}

//---------------------------------------------------------------------------------
// �Ƴ��������߰��ɳ�Ա��BUFF
//---------------------------------------------------------------------------------
static int SC_RemoveBuffFromAllGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwBuffID	= (DWORD)lua_tonumber(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}
	pGuild->GetMemberMgr().RemoveBuffFromAllOnLineMember(dwBuffID);

	return 0;
}

//---------------------------------------------------------------------------------
// �Ƴ�����ָ�������Ա��״̬��EState��
//---------------------------------------------------------------------------------
static int SC_UnsetStateOfAllGuildMember(lua_State* L)
{
	DWORD	dwGuildID		= (DWORD)lua_tonumber(L, 1);
	EState	eState			= (EState)lua_tointeger(L, 2);

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}
	pGuild->GetMemberMgr().UnsetStateOfAllGuildMember(eState);

	return 0;
}

//---------------------------------------------------------------------------------
// �Ƴ�����ս����ս��Ա��״̬��EState��
//---------------------------------------------------------------------------------
static int SC_UnsetStateOfAllRoleInBattleField(lua_State* L)
{
	DWORD	eGuildCapType	= (DWORD)lua_tonumber(L, 1);
	EState	eState			= (EState)lua_tointeger(L, 2);
	DWORD	eGodMiracle		= (DWORD)lua_tonumber(L, 3);

	CGodMiracleMgr* pGodMiracleMgr = CGodMiracleMgr::GetInst();
	if (!P_VALID(pGodMiracleMgr))
	{
		return 0;
	}
	
	CGodMiracle* pGodMiracle = pGodMiracleMgr->GetGodMiracle((EGodMiracle)eGodMiracle);
	if (!P_VALID(pGodMiracle))
	{
		return 0;
	}

	if (eGuildCapType == EGCT_Defend || eGuildCapType == EGCT_Capture)
	{
		pGodMiracle->UnsetStateOfAllRoleInBattleField((EGuildCapType)eGuildCapType, eState);
	}

	return 0;
}

//---------------------------------------------------------------------------------
// �õ����еİ��ɳ�Ա
//---------------------------------------------------------------------------------
static int SC_GetAllGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}
	
	INT16 n16GuildNumber(0);
	DWORD dwRoleID[100];

	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = pGuild->GetMemberMgr().GetMemberMap().Begin();
	while(pGuild->GetMemberMgr().GetMemberMap().PeekNext(iter, pMember))
	{
		dwRoleID[n16GuildNumber] = pMember->dwRoleID;
		n16GuildNumber++;
	}

	lua_newtable(L);
	for (int n=0; n<n16GuildNumber; n++)
	{
		lua_pushnumber(L, dwRoleID[n]);
		lua_rawseti(L, -2, n+1);
	}
	return 1;
}

//---------------------------------------------------------------------------------
// ���ɵĳ�Ա�Ƿ��б仯
//---------------------------------------------------------------------------------
static int SC_InChangeForGuildMember(lua_State* L)
{
	DWORD dwGuildID	= (DWORD)lua_tonumber(L, 1);
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		return 0;
	}

	if ( TRUE == pGuild->GetMemberMgr().GetGuildMemberChangeFlag())
		lua_pushboolean(L, TRUE);
	else
		lua_pushboolean(L, FALSE);
	
	return 1;
}

//---------------------------------------------------------------------------------
// ��ɫ����������
//---------------------------------------------------------------------------------
static int SC_GuildRoleInterGodArea(lua_State* L)
{
	DWORD dwGodID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 2);
	DWORD DOrCFlag	= (DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodID);
	if (!P_VALID(pGodMiracle)) return 0;

	BOOL bRet = pGodMiracle->PushGodRoleInfo(DOrCFlag, dwRoleID);
	if (FALSE == bRet)
		return 0;
	return 0;
}

//---------------------------------------------------------------------------------
// ��ɫ����������
//---------------------------------------------------------------------------------
static int SC_SetRoleStartActiveTimes(lua_State* L)
{
	DWORD dwGodID	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 2);
	DWORD DOrCFlag	= (DWORD)lua_tonumber(L, 3);
	DWORD IsInAreaFlag	= (DWORD)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodID);
	if (!P_VALID(pGodMiracle)) return 0;
	
	tagGodRoleInfo* pGodRoleInfo = pGodMiracle->GetGodRoleInfo(DOrCFlag, dwRoleID);
	if (!P_VALID(pGodRoleInfo)) return 0;

	pGodRoleInfo->SetAreaFlag(IsInAreaFlag);
	
	return 0;
}

//---------------------------------------------------------------------------------
// �õ���ǰ�񼣵�״̬
//---------------------------------------------------------------------------------
static int SC_GetGodMiracleCurState(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;
	
	lua_pushinteger(L, pGodMiracle->GetGodMiracleState());
	return 1;
}


//---------------------------------------------------------------------------------
// �����񼣣���¼��Ծʱ��
//---------------------------------------------------------------------------------
static int SC_UpdateGodMiracleMin(lua_State* L)
{
	 CGodMiracleMgr::GetInst()->UpdateMin();
	 return 0;
}

//---------------------------------------------------------------------------------
// �����񼣣���¼��Ծʱ��
//---------------------------------------------------------------------------------
static int SC_UpdateGodMiracle(lua_State* L)
{
	 CGodMiracleMgr::GetInst()->Update();
	return 0;
}
//---------------------------------------------------------------------------------
// ����ռ���񼣵İ���ID
//---------------------------------------------------------------------------------
static int SC_SetCaptureGodGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 2);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->SetCapGodID(dwGuildID);
	ILOG->Write(_T("SC_SetCaptureGodGuildID::SetCapGodID(dwGuildID):	%d\r\n"), dwGuildID);
	pGodMiracle->SetCapStartTime(GetCurrentDWORDTime());
	pGodMiracle->SaveCapTureInfoToActive();
	return 0;
}

//---------------------------------------------------------------------------------
// ����ռ���񼣵İ���ID
//---------------------------------------------------------------------------------
static int SC_GetCaptureGodGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	lua_pushnumber(L, pGodMiracle->GetCapGodID());
	return 1;
}


//---------------------------------------------------------------------------------
// ��������ռ���񼣵İ���ID
//---------------------------------------------------------------------------------
static int SC_SetApplyGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 2);
	DWORD dwMoney		= (DWORD)lua_tonumber(L, 3);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->SetApplyID(dwGuildID);
	pGodMiracle->SetApplyMoney(dwMoney);
	
	return 0;
}

//---------------------------------------------------------------------------------
// ��������ռ���񼣵İ���ID
//---------------------------------------------------------------------------------
static int SC_GetApplyGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	// ������ɽ�ɢ
	Guild* pGuild = g_guildMgr.GetGuild(pGodMiracle->GetApplyID());
	if ( P_VALID(pGuild))
		lua_pushnumber(L,  pGodMiracle->GetApplyID());
	else
		lua_pushnumber(L, 0);
	return 1;
}

//---------------------------------------------------------------------------------
// ����������ɵ������
//---------------------------------------------------------------------------------
static int SC_SetGuildRoleDie(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRole		= (DWORD)lua_tonumber(L, 2);
	DWORD dwBeKiller	= (DWORD)lua_tonumber(L, 3);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 4);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	tagGodRoleInfo* pGodRoleInfo = pGodMiracle->GetGodRoleInfo(dwDOrCFlag, dwRole);
	if (!P_VALID(pGodRoleInfo)) return 0;

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRole);
	if (!P_VALID(pRoleInfo)) return 0;

	if ( 1 == dwDOrCFlag )
	{
		pGodRoleInfo->SetBeKilledRole(dwBeKiller, pRoleInfo->dwGuildID);
	}
	else if ( 2 == dwDOrCFlag )
	{
		pGodRoleInfo->SetBeKilledRole(dwBeKiller, pRoleInfo->dwGuildID);
	}

	/// ����ӵĽ�ɫ���⴦��
	CGodMiracleMgr::GetInst()->ModteamInfoForkillRole(dwGodIndex, dwRole, dwBeKiller);

	return 0;
}

//---------------------------------------------------------------------------------
// ʤ������
//---------------------------------------------------------------------------------
static int SC_CalCaptureContributionAndExploit(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 2);	// �Ƿ�ʤ����־

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->CalAllRoleContributionAndExploit(dwDOrCFlag);

	return 0;
}

//---------------------------------------------------------------------------------
//��ʼ��ĳ�� 
//---------------------------------------------------------------------------------
static int SC_InitGodMiracle(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;
	
	pGodMiracle->Init();

	return 0;
}

//---------------------------------------------------------------------------------
// ����ĳ�񼣵ĵ�ǰ״̬ 
//---------------------------------------------------------------------------------
static int SC_SetGodCurState(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwCurState	= (DWORD)lua_tonumber(L, 2);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->SetCurState((EGodMiracleState)dwCurState);
	return 0;
}

//---------------------------------------------------------------------------------
// ����ĳ�񼣵ĵ�ǰ״̬ 
//---------------------------------------------------------------------------------
static int SC_RoleLeaveArea(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwRoleID		= (DWORD)lua_tonumber(L, 2);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 3);
	
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->RemoveGodRoleInfo(dwDOrCFlag, dwRoleID);
	return 0;
}
//---------------------------------------------------------------------------------
// �õ����в�ս������������
//---------------------------------------------------------------------------------
static int SC_SendAllCoptureGodGuildMember(lua_State* L)
{
	CGodMiracleMgr::GetInst()->BroadcastAllGoodPlayer();
	return 0;
}

//---------------------------------------------------------------------------------
// �����񼣷����ͽ������Ļ���
//---------------------------------------------------------------------------------
static int SC_SetGodGuildPoint(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwDOrCFlag	= (DWORD)lua_tonumber(L, 2);
	DWORD dwPoint		= (DWORD)lua_tonumber(L, 3);

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	pGodMiracle->ModPoint(dwDOrCFlag, dwPoint);
	return 0;
}

//---------------------------------------------------------------------------------
// ȡ���ع���ID
//---------------------------------------------------------------------------------
static int SC_GetDefendGuildID(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	DWORD dwGuildID = pGodMiracle->GetCapGodID();
	lua_pushnumber(L, dwGuildID);
	return 1;
}

//---------------------------------------------------------------------------------
// ȡ������Ӫ���ͣ�1��ʾ���أ�2��ʾ������0��ʾû�в�սȨ
//---------------------------------------------------------------------------------
static int SC_GetGuildAllianceType(lua_State* L)
{
	DWORD dwGodIndex	= (DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID		= (DWORD)lua_tonumber(L, 2);
	
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))	return 0;

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)dwGodIndex);
	if (!P_VALID(pGodMiracle)) return 0;

	BOOL bAlliance = FALSE;
	EGuildCapType eGuildCapType = pGodMiracle->GetCapType(dwGuildID,bAlliance);
	if (eGuildCapType != EGCT_Capture && eGuildCapType != EGCT_Defend && eGuildCapType != EGCT_NULL)
		return 0;

	if(bAlliance)
	{
		BOOL bConfirmed = pGodMiracle->GetAllianceConfirmed(dwGuildID);
		if (!bConfirmed)
			return 0;
	}

	lua_pushnumber(L, static_cast<DWORD>(eGuildCapType));
	return 1;
}

static int SC_CanGather( lua_State* L )
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 2);

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) || !P_VALID(pRole->GetMap())) return 0;

	Creature* pRes = pRole->GetMap()->FindCreature(dwCreatureID);
	if (!P_VALID(pRes)) return 0;

	INT nRt = pRole->CanGather(pRes);

	lua_pushinteger(L, nRt);

	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ�����ս�İ���
//---------------------------------------------------------------------------------
static int SC_GetConfirmGuild( lua_State* L )
{
	DWORD dwCityID = (DWORD)lua_tonumber(L, 1);
	ECityForceType eForce = (ECityForceType)lua_tointeger(L, 2);

	// ȡ������
	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
	{
		return 0;
	}

	// ��ȡ�������
	INT nGuildNum = MAX_CITY_ATTACK_FORCE;
	DWORD dwGuildID[MAX_CITY_ATTACK_FORCE];
	memset(dwGuildID, 0xFF, sizeof(dwGuildID));

	if (!pCity->GetConfirmGuild(eForce, nGuildNum, dwGuildID))
	{
		return 0;
	}

	//lua_pushinteger(L, nGuildNum);
	if (nGuildNum <= 0)
	{
		return 0;
	}

	lua_newtable(L);
	for (int n=0; n<nGuildNum; n++)
	{
		lua_pushnumber(L, dwGuildID[n]);
		lua_rawseti(L, -2, n+1);
	}
	
	return 1;
}

//---------------------------------------------------------------------------------
// ��ȡ�������� -- �����ڻ�ȡ�������ݣ��޷���ȡ���в�������������Ϣ
//---------------------------------------------------------------------------------
static int SC_GetCityAppointedAtt(lua_State* L)
{
	DWORD dwCityID	=	(DWORD)lua_tonumber(L, 1);
	INT nType		=	lua_tointeger(L, 2);
	DWORD dwData	=	GT_INVALID;

	// ȡ������
	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
	{
		return 0;
	}

	INT nNum = 0;
	pCity->GetCityAppointedAtt(ECityAttType(nType), nNum, &dwData);

	lua_pushnumber(L, dwData);
	return 1;
}
//---------------------------------------------------------------------------------
// ���ݵ�ͼid��ȡ�������� -- �����ڻ�ȡ�������ݣ��޷���ȡ���в�������������Ϣ
//---------------------------------------------------------------------------------
static int SC_GetCityAppointedAttByMapID(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	INT		nType			=	lua_tointeger(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	// ȡ������
	City* pCity = g_cityMgr.GetCity(pMap);
	if (!P_VALID(pCity))
	{
		return 0;
	}
	
	DWORD dwData = GT_INVALID;
	INT nNum = 0;
	pCity->GetCityAppointedAtt(ECityAttType(nType), nNum, &dwData);

	lua_pushnumber(L, dwData);
	return 1;

}
//---------------------------------------------------------------------------------
// �ı���з���ֵ
//---------------------------------------------------------------------------------
static int SC_ChangeCityDefence(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	INT32	nChangeVal		=	lua_tointeger(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	// ȡ������
	City* pCity = g_cityMgr.GetCity(pMap);
	if (!P_VALID(pCity))
	{
		return 0;
	}
	if(nChangeVal > 0)
	{
		pCity->IncCityDefence(nChangeVal);
	}
	else if(nChangeVal < 0)
	{
		pCity->DecCityDefence(-nChangeVal);
	}

	return 1;
}
//---------------------------------------------------------------------------------
// �ı����ͳ�ΰ���
//---------------------------------------------------------------------------------
static int SC_ChangeCityHolder(lua_State* L)
{
	DWORD dwCityID	=	(DWORD)lua_tonumber(L, 1);
	DWORD dwGuildID =	(DWORD)lua_tonumber(L, 2);

	// ȡ������
	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
	{
		return 0;
	}

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		// ���ɲ�����
		return 0;
	}

	pCity->ChangeCityHolder(dwGuildID);

	return 0;
}

//---------------------------------------------------------------------------------
// ���ͳ�ս״̬
//---------------------------------------------------------------------------------
static int SC_SendCityStruggleInfo(lua_State* L)
{
	DWORD dwMapID					=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID				=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID					=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagLongZhuState sLongZhuState;
	sLongZhuState.eForce			=	(ECityForceType)lua_tointeger(L, 4);	// ��ǰռ�����������
	sLongZhuState.bSealing			=	lua_toboolean(L, 5);					// �����Ƿ����ڱ���ӡ
	sLongZhuState.nCountDown		=	lua_tointeger(L, 6);					// �����·��ĵ���ʱ
	
	tagYinYangZhuState sYinYangState[5];
	sYinYangState[0].eForce			=	(ECityForceType)lua_tointeger(L, 7);					// ������ռ��״̬
	sYinYangState[1].eForce			=	(ECityForceType)lua_tointeger(L, 8);
	sYinYangState[2].eForce			=	(ECityForceType)lua_tointeger(L, 9);
	sYinYangState[3].eForce			=	(ECityForceType)lua_tointeger(L, 10);
	sYinYangState[4].eForce			=	(ECityForceType)lua_tointeger(L, 11);
	
	INT32 nWuZiKuNum				=	lua_tointeger(L, 12);					// ���ʿ�����

	tagCityDoorState sCityDoorState[3];											// ����״̬
	sCityDoorState[0].nDoorType		=	lua_tointeger(L, 13);
	sCityDoorState[0].bDestroied	=	lua_tointeger(L, 14);
	sCityDoorState[0].nCountDown	=	lua_tointeger(L, 15);

	sCityDoorState[1].nDoorType		=	lua_tointeger(L, 16);
	sCityDoorState[1].bDestroied	=	lua_tointeger(L, 17);
	sCityDoorState[1].nCountDown	=	lua_tointeger(L, 18);

	sCityDoorState[2].nDoorType		=	lua_tointeger(L, 19);
	sCityDoorState[2].bDestroied	=	lua_tointeger(L, 20);
	sCityDoorState[2].nCountDown	=	lua_tointeger(L, 21);

	INT32 nAtkPlayerNum				=	lua_tointeger(L, 22);					// ����������
	INT32 nDefPlayerNum				=	lua_tointeger(L, 23);

	tagNS_CityStruggleInfo send;
	send.nAtkPlayerNum = nAtkPlayerNum;
	send.nDefPlayerNum = nDefPlayerNum;
	send.nWuZiKuNum = nWuZiKuNum;
	memcpy(send.sDoorState, sCityDoorState, sizeof(sCityDoorState));
	send.sLongZhuState = sLongZhuState;
	memcpy(send.sYinYangZhuState, sYinYangState, sizeof(sYinYangState));
	
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����һ����ս��Ϣ
//-------------------------------------------------------------------------------------
int SC_SendOneCityStruggleInfo(lua_State* L)
{
	DWORD dwMapID					=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID				=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID					=	(DWORD)lua_tonumber(L, 3);
	ECityStruggleChangeType eType1	=	(ECityStruggleChangeType)lua_tointeger(L, 4);	// ����1
	INT32 nType2					=	lua_tointeger(L, 5);							// ����2
	INT32 nData						=	lua_tointeger(L, 6);							// �ı������

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_CityStruggleInfoChange send;
	send.eType1 = eType1;
	send.nType2 = nType2;
	send.nData = nData;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------
// ���ó�ս״̬
//-------------------------------------------------------------------------------------
static int SC_SetCityStruggleState(lua_State* L)
{
	DWORD				dwCityID	=	(DWORD)lua_tonumber(L, 1);
	ECityStruggleState	state		=	(ECityStruggleState)lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->SetCityStruggleState(state);
	return 0;
}

//-------------------------------------------------------------------------------------
// �������˸���ʱ��
//-------------------------------------------------------------------------------------
static int SC_SetReviveTime(lua_State* L)
{
	DWORD			dwCityID	=	(DWORD)lua_tonumber(L, 1);
	ECityForceType	eForceType	=	(ECityForceType)lua_tointeger(L, 2);
	INT				nSecond		=	(INT)lua_tonumber(L, 3);

	if (nSecond < 0)
		return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->SetReviveTime(eForceType, nSecond);

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ�����Ӫ
//-------------------------------------------------------------------------------------
static int SC_GetRoleForce(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	ECityForceType eForce;
	if (TRUE == pCity->GetForceType(pRole, eForce))
	{
		lua_pushnumber(L, eForce);
		return 1;
	}

	lua_pushnumber(L, 2);
	return 1;
}

//-------------------------------------------------------------------------------------
// ��ҽ����뿪��ս����--����ͻ��˷���Ϣ
//-------------------------------------------------------------------------------------
static int SC_EnterLeaveCityStruggle(lua_State* L)
{
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 2);
	BOOL	bEnter			=	(BOOL)lua_tointeger(L, 3);
	ECityForceType	eForce	=	(ECityForceType)lua_tointeger(L, 4);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))	return 0;

	// �ҵ����
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( !P_VALID(pRole) ) return 0;

	// ���ͻ��˷���Ϣ
	if (bEnter)
	{
		// ���͸��ͻ��˵ж������İ���id
		if (eForce == ECFT_Attack)
		{
			// �������Ϊ������Ӫ״̬
			pRole->SetState(ES_CSAttack);
		}
		else
		{
			// �������Ϊ������Ӫ״̬
			pRole->SetState(ES_CSDefence);
		}
	}

	else
	{
		// ȡ������ҵĳ�ս��Ӫ״̬
		pRole->UnsetState(ES_CSAttack);
		pRole->UnsetState(ES_CSDefence);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// �ı������ģ��
//-------------------------------------------------------------------------------------
static int SC_ChangeCityDoor(lua_State* L)
{
	DWORD	dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwTypeID	=	(DWORD)lua_tonumber(L, 2);

	const tagMapInfo* pInfo = g_mapCreator.GetMapInfo(dwMapID);
	if (!P_VALID(pInfo))	return 0;

	tagMapDoor* pMapDoor = NULL;
	pInfo->mapDoor.ResetIterator();
	while (pInfo->mapDoor.PeekNext(pMapDoor))
	{
		pMapDoor->dwTypeID = dwTypeID;
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// ��ʼ����ս���ս��
//-------------------------------------------------------------------------------------
static int SC_InitCSRoleData(lua_State* L)
{
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 1);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().InitCSRoleData();
	return 0;
}

//-------------------------------------------------------------------------------------
// ����һ����ս���ս����¼
//-------------------------------------------------------------------------------------
static int SC_AddRoleDataInfo(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().AddRoleDataInfo(dwRoleID);
	return 0;
}

//-------------------------------------------------------------------------------------
// ������һ�ɱ��
//-------------------------------------------------------------------------------------
static int SC_AddKillRoleNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().AddKillRoleNum(dwRoleID, dwTargetID);
	return 0;
}

//-------------------------------------------------------------------------------------
// �ı�һ�����ս������
//-------------------------------------------------------------------------------------
static int SC_AddRoleData(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);
	INT		nType			=	(INT)lua_tointeger(L, 5);
	DWORD	dwData			=	(DWORD)lua_tointeger(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().AddRoleData(dwRoleID, (ECityStruggleRoleData)nType, dwData);

	return 0;
}

//-------------------------------------------------------------------------------------
// ������ս������
//-------------------------------------------------------------------------------------
static int SC_GetRoleCSScore(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	tagCSRoleData* pData = pCity->GetCityStruggle().GetRoleDataPtr(dwRoleID);
	if (P_VALID(pData))
	{
		lua_pushnumber(L, pData->RoleData.nScore);
		return 1;
	}

	return 0;

}

//-------------------------------------------------------------------------------------
// ��ս�������ս������
//-------------------------------------------------------------------------------------
static int SC_SortByScore(lua_State* L)
{
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 1);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().SortByScore();

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ��ҳ�ս��ɱ����
//-------------------------------------------------------------------------------------
static int SC_GetRoleKillNum(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwCityID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	tagCSRoleData* pData = pCity->GetCityStruggle().GetRoleDataPtr(dwRoleID);
	if (P_VALID(pData))
	{
		lua_pushnumber(L, (DWORD)pData->RoleData.byKills);
		return 1;
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��ȡ��սս���������ID
//-------------------------------------------------------------------------------------
static int SC_GetCSRankedRoleID(lua_State* L)
{
	DWORD	dwCityID	=	(DWORD)lua_tonumber(L, 1);
	INT32	nRankNum	=	lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	if (nRankNum < 1)
		return 0;

	INT32 nSize = pCity->GetCityStruggle().GetRankData().size();
	lua_newtable(L);
	for (int n=0; n<nRankNum && n<nSize; ++n)
	{
		DWORD dwRoleID = (pCity->GetCityStruggle().GetRankData())[n].first;
		lua_pushnumber(L, dwRoleID);
		lua_rawseti(L, -2, n+1);
	}

	return 1;
}

//-------------------------------------------------------------------------------------
// ���ó�ս���
//-------------------------------------------------------------------------------------
static int SC_SetCityStruggleResult(lua_State* L)
{
	DWORD	dwCityID			=	(DWORD)lua_tonumber(L, 1);
	ECityStruggleResult eResult	=	(ECityStruggleResult)lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().SetResult(eResult);

	return 0;
}

//-------------------------------------------------------------------------------------
// �ٻ��ػ��������ı�
//-------------------------------------------------------------------------------------
static int SC_SummonPersonChange(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT32	nPerson			=	lua_tointeger(L, 4);
	INT32	nNeedPerson		=	lua_tointeger(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagNS_SummonPersonChange send;
	send.nPerson = nPerson;
	send.nNeedPerson = nNeedPerson;
	return 0;
}

//-------------------------------------------------------------------------------------
// ��ʼ�ػ����ٻ�����ʱ
//-------------------------------------------------------------------------------------
static int SC_SetSummonTime(lua_State* L)
{
	DWORD	dwCityID	=	(DWORD)lua_tonumber(L, 1);
	INT32	nCountDown	=	lua_tointeger(L, 2);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().SetSummonTime(nCountDown);
	return 0;
}

//-------------------------------------------------------------------------------------
// ��ֹ�ٻ�
//-------------------------------------------------------------------------------------
static int SC_StopSummon(lua_State* L)
{
	DWORD	dwCityID	=	(DWORD)lua_tonumber(L, 1);

	City* pCity = g_cityMgr.GetCity(dwCityID);
	if (!P_VALID(pCity))
		return 0;

	pCity->GetCityStruggle().StopSummon();
	return 0;
}

// Jason 2010-1-19 v1.3.2 �������ʱ��

static int SC_GetPlayerOfflineTime(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		sec = pRole->GetOfflineTime  ();
	}
	lua_pushnumber(L,sec);
	return 1;
}

// Jason 2010-1-27 v1.3.2 �˺�����ʱ��
static int SC_GetPlayerAccountOfflineTime(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		PlayerSession * pSession = pRole->GetSession  ();
		if( P_VALID(pSession) )
		{
			sec = g_PlayerAccountMgr.GetOfflineTime  (pSession->GetSessionID());
		}
	}
	lua_pushnumber(L,sec);
	return 1/*SC_GetPlayerOfflineTime(L)*/;
}
static int SC_CanRecAccountRegressionReward(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	BOOL sec = FALSE;
	if( P_VALID(pRole) )
	{
		//sec = pRole->GetOfflineTime  ();
		PlayerRegression * p = TPlayerRegression::Instance  ();
		if( P_VALID(p) )
		{
			if( p->CanRecRegression  (pRole) )
			{
				sec = TRUE;
				lua_pushboolean(L,1);
				return 1;
			}
		}
	}
	lua_pushboolean(L,0);
	return 1;
}
static int SC_SetRecAccountRegressionRewardFlag(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	bReced			=	(DWORD)lua_tonumber(L, 2);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		//sec = pRole->GetOfflineTime  ();
		PlayerSession * pSession = pRole->GetSession  ();
		if( P_VALID(pSession) )
		{
			if( 1 == bReced )
				g_PlayerAccountMgr.ReceiveRegressionRewardTime(pSession->GetSessionID  (),g_world.GetWorldTime  ());
			else if( 0 == bReced )
				g_PlayerAccountMgr.ReceiveRegressionRewardTime(pSession->GetSessionID  (),0);
		}
	}
	return 0;
}

int SC_OpenFramePage(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT nType = (INT)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_OpenFramePage send;
		send.eType = (EFrameType)nType;
		pRole->SendMessage(&send, send.dwSize);
	}

	return 0;
}
int SC_RemoveSkill(lua_State* L)
{
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwSkillBigID	=	(DWORD)lua_tonumber(L, 2);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	DWORD sec = 0;
	if( P_VALID(pRole) )
	{
		//char buf[20] = {0};
		//sprintf(buf,"%u",dwSkillBigID);
		//if( strlen(buf) >= 7 )
		//{
		//	buf[7] = 0;
		//	INT smallID = atoi(buf);
			pRole->RemoveSkill  (dwSkillBigID);
			lua_pushnumber(L,1);
			return 1;
		//}
	}
	lua_pushnumber(L,0);
	return 1;
}

int SC_GetConsumptiveStrength(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{
		INT nTmp = pRole->GetStrength().nConsumptiveStrength;
		lua_pushinteger(L, nTmp);
	}

	return 1;
}

int SC_SetConsumptiveStrength(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT nValue =	(INT)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{
		pRole->SetConsumptiveStrength(nValue);	
	}

	return 0;
}

int SC_GetRoleMapID(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{	
		Map* pMap = pRole->GetMap();
		if(!P_VALID(pMap))
		{
			return 0;
		}

		DWORD dwMapID = pRole->GetMapID();
		DWORD dwInstanceID = pMap->GetInstanceID();
	
		lua_pushnumber(L, dwMapID);
		lua_pushnumber(L, dwInstanceID);

		return 2;
	}

	return 0;
}

// Jason 2010-3-16 v1.5.0
// role lib
/**
* �����������ӿ�
* \param mapid 
* \param instantid 
* \param roleid ��ɫid 
* \param points ���㣬�����ӣ�������
* \return �ɹ�����true�����򷵻�false
*/
//BOOL AddRoleGiftPoints(DWORD mapid, DWORD instantid, DWORD roleid, int points)

int SC_AddRoleGiftPoints(lua_State * L)
{
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwMapInstantID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			= (DWORD)lua_tonumber(L, 3);
	INT		nGiftPoints		= (INT)	 lua_tointeger(L,4);

	BOOL re = FALSE;
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) && nGiftPoints != 0 )
	{	
		Map* pMap = pRole->GetMap();
		if(!P_VALID(pMap))
		{
			lua_pushnumber(L,re);
			return 1;
		}

		if( nGiftPoints > 0 )
			pRole->GetCurMgr().IncExchangeVolume(nGiftPoints,ELCLD_MALL_LUA_AddGiftPoint);
		else
			pRole->GetCurMgr().DecExchangeVolume(nGiftPoints,ELCLD_MALL_GiftPointCosted);
		re = TRUE;
	}

	lua_pushnumber(L,re);

	return 1;
}

static int SC_Trace(lua_State * L)
{
	ITRACE(lua_tostring(L, 1));
	return 0;
}


//-------------------------------------------------------------------------------------
// ������Ԫ����
//-------------------------------------------------------------------------------------
static int SC_GodMiraclePoints(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	INT32  points = pRole->GetGodMiraclePoints();
	lua_pushnumber(L, points);

	return 1;
}
static int SC_ModifyMiraclePoints(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT   points		=	(INT)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	pRole->GodMiraclePoints(points);
	lua_pushboolean(L, 1);

	return 1;
}

// ���ͻ��˷��ͳ�ս����ս����Ϣ
static int SC_SendCityStruggleRecord2Role(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	pRole->SendCityStruggleRecord();

	return 0;
}
static int SC_PullCreatureToPos(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	FLOAT   x		=	(FLOAT)lua_tonumber(L, 4);
	FLOAT   y		=	(FLOAT)lua_tonumber(L, 5);
	FLOAT   z		=	(FLOAT)lua_tonumber(L, 6);

	x *= TILE_SCALE;
	y *= TILE_SCALE;
	z *= TILE_SCALE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if(!P_VALID(pCreature))
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	pCreature->PulledToPos(Vector3(x,y,z));
	lua_pushboolean(L, 1);

	return 1;
};


static int SC_GetRoleGuildAtt(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	INT   nGuildAttID		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}
	DWORD dwGuildID = pRole->GetGuildID();
	if( !P_VALID(dwGuildID) || EGAN_NULL >= nGuildAttID || nGuildAttID >= EGAN_END )
	{
		return 0;
	}

	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
	{
		return 0;
	}
	const tagGuild & att = pGuild->GetGuildAtt();

	int r = 1;

	switch(nGuildAttID)
	{
	case EGAN_LeaderID:
		lua_pushnumber(L,att.dwLeaderRoleID);
		break;
	case EGAN_Level:
		lua_pushnumber(L,att.byLevel);
		break;
	case EGAN_SpecState:
		lua_pushnumber(L,att.dwSpecState);
		break;
	case EGAN_Fund:
		lua_pushnumber(L,att.nFund);
		break;
	case EGAN_Material:
		lua_pushnumber(L,att.nMaterial);
		break;
	case EGAN_Reputation:
		lua_pushnumber(L,att.nReputation);
		break;
	case EGAN_GroupPurchase:
		lua_pushnumber(L,att.nGroupPurchase);
		break;
	case EGAN_Peace:
		lua_pushnumber(L,att.n16Peace);
		break;
	case EGAN_MemberNum:
		lua_pushnumber(L,att.n16MemberNum);
		break;
	case EGAN_Rank:
		lua_pushnumber(L,att.n16Rank);
		break;
	case EGAN_MinJoinLevel:
		lua_pushnumber(L,att.byMinJoinLevel);
		break;
	case EGAN_Commendation:
		lua_pushnumber(L,att.bCommendation);
		break;
	case EGAN_DailyCost:
		lua_pushnumber(L,att.nDailyCost);
		break;
	default:
		r = 0;
		break;
	}

	return r;
}
static int SC_GetSoarValue(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	lua_pushnumber(L,pRole->GetBaseAttValue(ERA_Soar));

	return 1;
}
static int SC_SetSoarValue(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwVal		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;// ����lua����nil
	}

	pRole->SetSoaringAtt(dwVal);
	lua_pushboolean(L,1);

	return 1;
}
static int SC_GetGuildAtt(lua_State* L)
{
	DWORD   dwGuildID		=	(DWORD)lua_tonumber(L, 1);

	if( !P_VALID(dwGuildID) )
		return 0;

	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
		return 0;

	const tagGuild & att = pGuild->GetGuildAtt();
	lua_pushnumber(L,att.byLevel);

	return 1;
}

static int SC_SyncFlagAttack(lua_State* L)
{
	DWORD		dwGodID						= (DWORD)lua_tonumber(L, 1);// ������
	DWORD	    dwFlagType					= (DWORD)	lua_tonumber(L, 2);// ָ������
	DWORD	    dwGuildCapType				= (DWORD)lua_tonumber(L, 3);// ��������
	DWORD		dwMapID						= (DWORD)lua_tonumber(L, 4);
	DWORD		dwInstanceID				= (DWORD)lua_tonumber(L, 5);
	BOOL		bMapMessage					= (BOOL)lua_tonumber(L, 6);
	INT 		nFlagPosX 					= (INT)lua_tonumber(L, 7);
	INT			nFlagPosY					= (INT)lua_tonumber(L, 8);
	INT			nFlagPosZ 					= (INT)lua_tonumber(L, 9);
	EGodMiracle	eGodID						= (EGodMiracle)dwGodID;
	EFlagType	eFlagType					= (EFlagType)dwFlagType;
	EGuildCapType		eGuildCapType		= (EGuildCapType)dwGuildCapType;

	if (eGodID >= EGM_MAX || eGodID < EGM_NH)
		return 0;
	if (eFlagType < EFT_God || eFlagType > EFT_Monster)
		return 0;
	if (eGuildCapType < EGCT_NULL || eGuildCapType > EGCT_Capture)
		return 0;

	tagNS_SyncFlagAttack send;
	send.eFlagType = eFlagType;
	send.eGuildCapType = eGuildCapType;
	send.eGodID = eGodID;
	send.nFlagPosX = nFlagPosX;
	send.nFlagPosY = nFlagPosY;
	send.nFlagPosZ = nFlagPosZ;
	if (bMapMessage)
	{
		Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
		if(P_VALID(pMap))
			pMap->SendMapMessage(&send, send.dwSize);
	}
	else
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwMapID);
		if (P_VALID(pRole))
			pRole->SendMessage(&send, send.dwSize);
	}
	return 0;
}

static int SC_SendRoleSwitchMsg(lua_State* L)
{
	DWORD ndx		= (DWORD)lua_tonumber(L, 1);
	DWORD dwMapID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwInsID	= (DWORD)lua_tonumber(L, 3);
	DWORD dwRoleID	= (DWORD)lua_tonumber(L, 4);
	DWORD dwAppID	= (DWORD)lua_tonumber(L,5);
	DWORD dwActID	= (DWORD)lua_tonumber(L,6);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	const tagMsgDeliver * pMsgDeliver = g_attRes.GetOneMsgDeliverProto(ndx);
	if( P_VALID(pMsgDeliver) && P_VALID(pRole) )
	{
		//DWORD dwMsgID		= pMsgDeliver->dwIDMsgID;
		//DWORD dwMsgInfoID	= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, (EMsgUnitType)dwMsgID, NULL);
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->X));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Y));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Z));
		//g_MsgInfoMgr.DispatchRoleMsgEvent(dwMsgInfoID, pRole);
		tagNS_PullRoleMsg send;
		send.dwAppID = dwAppID;
		send.dwActID = dwActID;
		send.dwIdMsgID	= pMsgDeliver->dwIDMsgID;
		send.dwMapID	= pMsgDeliver->dwMapID;
		send.x			= pMsgDeliver->X;
		send.y			= pMsgDeliver->Y;
		send.z			= pMsgDeliver->Z;
		pRole->SendMessage(&send,send.dwSize);
	}
	return 0;
}

static int SC_SendWorldSwitchMsg(lua_State* L)
{
	struct tagComp
	{
		INT m_nLevel;
		tagComp()
			:m_nLevel(0){}
		bool operator()(Role *pRole,int minilevel,int maxlevel)
		{
			if( P_VALID(pRole) )
			{
				m_nLevel = pRole->GetLevel();
				if( ( GT_INVALID == minilevel || (m_nLevel >= minilevel) ) &&
					( GT_INVALID == maxlevel || (m_nLevel <= maxlevel) ) )
				{
					return true;
				}
			}
			return false;
		}
	};
	//struct tagSafeFree
	//{
	//	LPVOID m_p;
	//	DWORD m_id;
	//	tagSafeFree(LPVOID p,DWORD id)
	//		:m_p(p),m_id(id){}
	//	~tagSafeFree()
	//	{
	//		if( P_VALID(m_p) && P_VALID(m_id) )
	//			g_MsgInfoMgr.DiscardMsg(m_id,m_p);
	//	}
	//};
	struct SendMsg
	{
		SendMsg(PVOID pMsg, DWORD dwSize)
			:m_pMsg(pMsg), m_dwSize(dwSize){}
		VOID operator()(Role* pRole)
		{
			if (P_VALID(pRole))
			{
				pRole->SendMessage(m_pMsg, m_dwSize);
			}
		}
		PVOID m_pMsg;
		DWORD m_dwSize;
	};


	DWORD ndx		= (DWORD)lua_tonumber(L, 1);
	int minlevel	= (int)lua_tonumber(L, 2);
	int maxlevel	= (int)lua_tonumber(L, 3);
	DWORD dwAppID	= (DWORD)lua_tonumber(L, 4);
	DWORD dwActID	= (DWORD)lua_tonumber(L,5);

	const tagMsgDeliver * pMsgDeliver = g_attRes.GetOneMsgDeliverProto(ndx);
	if( P_VALID(pMsgDeliver) )
	{
		//DWORD dwMsgID		= pMsgDeliver->dwIDMsgID;
		//DWORD dwMsgInfoID	= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, (EMsgUnitType)dwMsgID, NULL);
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->X));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Y));
		//g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, const_cast<int*>(&pMsgDeliver->Z));
		//BYTE * pData;
		//INT size ;
		//g_MsgInfoMgr.GetDispatchMsg(dwMsgInfoID,pData,size);
		tagNS_PullRoleMsg send;
		send.dwAppID = dwAppID;
		send.dwActID = dwActID;
		send.dwIdMsgID	= pMsgDeliver->dwIDMsgID;
		send.dwMapID	= pMsgDeliver->dwMapID;
		send.x			= pMsgDeliver->X;
		send.y			= pMsgDeliver->Y;
		send.z			= pMsgDeliver->Z;
		//if( P_VALID(pData) && size )
		{
			//tagNetCmd * pCmd = (tagNetCmd*)pData;
			//tagSafeFree(pData,dwMsgInfoID);
			//if(pCmd->dwSize == size)
				g_roleMgr.ForEachSpec2(tagComp(),minlevel,maxlevel,SendMsg(&send,send.dwSize));
		}
	}
	return 0;
}

//static int SC_ModSpecialTargetValue(lua_State* L)
//{
//	DWORD	dwRoleID		= (DWORD)lua_tonumber(L, 1);
//	UINT16	u16QuestID		= (UINT16)lua_tonumber(L, 2);
//	BOOL	bValue			= (BOOL)lua_tonumber(L, 3);
//
//	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
//	if (!P_VALID(pRole))
//	{
//		return 0;
//	}
//
//	Quest* pQuest = pRole->GetQuest(u16QuestID);
//	if (!P_VALID(pQuest))
//	{
//		return 0;
//	}
//
//	pQuest->SetSpecTargetFlag(bValue);
//
//	tagNS_QuestSpecComplete send;
//	send.dwRoleID = dwRoleID;
//	send.u16QuestID = u16QuestID;
//	send.bFlag = bValue;
//	pRole->SendMessage(&send, send.dwSize);
//
//	return 0;
//}

static int SC_SetRoleClass(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	EClassType   eClass		=	(EClassType)(DWORD)lua_tonumber(L, 4);

	if(EV_Null>eClass || eClass >= EV_End) return 0;
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	// ��ְ
	pRole->SetClass(eClass);

	// ϴ����
	pRole->ClearTalent((INT64)0, FALSE);

	tagNS_RoleClassChangeBroadcast send;
	send.dwRoleID	= pRole->GetID();
	send.eClass = pRole->GetClass();
	pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);
	lua_pushboolean(L,1);
	return 1;
}

static int SC_GetRoleClass(lua_State* L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	EClassType   eClass	 = pRole->GetClass();

	lua_pushnumber(L, eClass);

	return 1;
}

static int SC_GetRoleCompleteBoardQuestNum(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	BYTE		byType			= (BYTE)lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	EQuestBoardType eQuestBoardType = static_cast<EQuestBoardType>(byType);
	if (!P_VALID(pRole->GetBoardQuestMgr(eQuestBoardType)))
		return 0;

	INT nNum = pRole->GetBoardQuestMgr(eQuestBoardType)->GetCompleteReFreshTime();
	lua_pushinteger(L,nNum);
	return 1;
}

static INT SC_GetRoleYaojingLevel(lua_State * L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagRemoteRoleFabaoInfo info;

	pRole->SynRemoteBodyFabaoInfo(info);
	if( info.byEquipFabaoFlag )
	{
		lua_pushinteger(L,info.byStage);
		
	}
	else
	{
		lua_pushinteger(L,0);
	}
	
	return 1;
}

static int SC_ModSpecialTargetValue(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	UINT16	u16QuestID		=	(UINT16)lua_tonumber(L, 4);
	BYTE	byValue			=	(BYTE)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	const tagQuestProto* pQuestProto = g_questMgr.GetQuestProto(u16QuestID);
	if(!P_VALID(pQuestProto)) return 0;

	if (!pQuestProto->spec_condition) return 0;
	
	Quest* pQuest = pRole->GetQuest(u16QuestID);
	if (!P_VALID(pQuest)) return 0;

	tagNS_RoleSpecQuestCompleteState send;
	send.dwRoleID = dwRoleID;
	send.u16QuestID = u16QuestID;
	send.byState = byValue;
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}

static INT SC_PutDownItem(lua_State * L)
{
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwItemID		=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	tagItem* pItem = pRole->GetItemMgr().GetItemByTypeID(dwItemID);
	if (!P_VALID(pItem)) return 0;

	INT64 n64GroupID = 0;
	pRole->LootItem(pItem->n64Serial, n64GroupID);

	push_64bit_data(L, n64GroupID);

	return 2;
}

// �ᱦ����ű��ӿ�
static int SC_SyncOpenChaseRemainTime(lua_State* L)
{
	DWORD   dwRoleID				=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwOpenChaseRemainTime	=	(DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	tagNS_SyncOpenChaseRemainTime send;
	send.dwOpenChaseRemainTime = dwOpenChaseRemainTime;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

static int SC_ForceLootChase(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	tagNS_ForceLootChase send;
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

static int SC_GetCurMoveState(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	lua_pushnumber(L, pRole->GetMoveData().GetCurMoveState());
	return 1;
}

static int SC_MoveRole2AnotherMap(lua_State* L)
{
	DWORD   dwCurMapID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID = (DWORD)lua_tonumber(L, 2);
	DWORD   dwDestMapID = (DWORD)lua_tonumber(L, 3);
	float   fX = (float)lua_tonumber(L, 4);
	float   fY = (float)lua_tonumber(L, 5);
	float   fZ = (float)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwCurMapID, dwInstanceID);
	if (!P_VALID(pMap)) return 0;


	Map::RoleMap::TMapIterator itRole = pMap->GetRoleMap().Begin();
	Role* pRole = NULL;
	while (pMap->GetRoleMap().PeekNext(itRole, pRole))
	{
		if (!P_VALID(pRole))
			continue;
		pRole->GotoNewMap(dwDestMapID, fX, fY, fZ);
	}
	return 0;
}

static int SC_SetMonsterGenState(lua_State* L)
{
	DWORD		dwCurMapID = (DWORD)lua_tonumber(L, 1);
	DWORD		dwInstanceID = (DWORD)lua_tonumber(L, 2);
	DWORD		dwState = (DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwCurMapID, dwInstanceID);
	if (!P_VALID(pMap)) return 0;

	pMap->GetMonsterGen().SetState((EMonsterGenState)dwState);

	return 1;
}

static int SC_GetTrialCompleteNum(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		lua_pushnumber(L, 0);
		return 1;
	}

	lua_pushnumber(L, pRole->GetTrialCompleteNum(dwTrainID));
	return 1;
}

static int SC_SetTrialCompleteNum(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);
	DWORD	dwCompleteness = (DWORD)lua_tonumber(L, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	pRole->SetTrainCompleteness(dwTrainID, dwCompleteness);

	return 0;
}

static int SC_TrialComplete(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) return 0;

	pRole->CompleteTrain(dwTrainID);

	return 0;
}

static int SC_GetTrialState(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	DWORD   dwTrainID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pRole->GetTrainState(dwTrainID));

	return 1;
}

static int SC_GetFriendNum(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		lua_pushnumber(L, 0);
		return 1;
	}

	lua_pushnumber(L, pRole->GetFriendCount());

	return 1;
}

static int SC_GetEquipIDOnPos(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT16       nPos = (INT16)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	if (nPos < EEP_Equip_Start || nPos > EEP_End)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(nPos);
	if (!P_VALID(pEquip))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pEquip->dwTypeID);
	return 1;
}

static int SC_GetEquipLvLimit(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}
	
	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 1;
		}
	}
	BYTE byUseLvlLimit = 0;
	if (P_VALID(pEquip->pEquipProto))
	{
		if (pEquip->IsFaBao())
		{
			tagFabao* pFabao = (tagFabao*)pEquip;
			if (P_VALID(pFabao))
				byUseLvlLimit = pFabao->nUseLevel;
		}
		else
		{
			byUseLvlLimit = pEquip->pEquipProto->byMinUseLevel;
		}
	}

	lua_pushnumber(L, byUseLvlLimit);
	return 1;
}

static int SC_GetEquipQlty(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 1;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pEquip->equipSpec.byQuality);//pEquip->pEquipProto->byQuality);
	return 1;
}

static int SC_GetEquipStrengthLevel(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 1;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pEquip->equipSpec.byConsolidateLevel);
	return 1;
}

static int SC_SetEquipStrengthLevel(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwLevel = (DWORD)lua_tonumber(L, 4);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	if (!pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	ItemCreator::RecalEquipStrengthAtt(pEquip,dwLevel);
	pEquip->equipSpec.byConsolidateLevel = dwLevel;
	pRole->CalEquipFlare(pEquip);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}


static int SC_SetGrowEquipLevel(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwGrowNum = (DWORD)lua_tonumber(L, 4);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	if (!pEquip->pEquipProto->bOnlyScriptStrengthen)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	const tagEquipGrowProto* pGrowProto = g_attRes.GetEquipGrowProto(pEquip->equipSpec.dwGrowTypeID);
	if (!P_VALID(pGrowProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}


	// ͳ�Ƹ�����ɫ����
	int nGray=0, nGreen=0, nBlue=0, nPurple = 0;
	pEquip->equipSpec.dwGrowValue += dwGrowNum;
	++pEquip->equipSpec.nTotalGrowTimes;

	// ����Ƿ������
	float fpct = 1.0f;
	DWORD dwValueLine = 0;
	for (int i=0; i<pEquip->equipSpec.byGrowLevel+1; ++i)
	{
		dwValueLine += pGrowProto->dwExp[i]*fpct;
		//dwValueNow = (pEquip->equipSpec.dwGrowValue > dwValueLine) ? 0 : (dwValueLine - pEquip->equipSpec.dwGrowValue);

		// ������
		if (pEquip->equipSpec.byGrowLevel <= i && pEquip->equipSpec.dwGrowValue >= dwValueLine)
		{
			EEquipGrowStar eNewStar = (EEquipGrowStar)pRole->OnRoleEquipGrow(pEquip->pProtoType->dwTypeID, pEquip->equipSpec.byQuality, pEquip->equipSpec.nTotalGrowTimes,
				pEquip->equipSpec.nTotalIMUseTimes, pEquip->equipSpec.byGrowLevel+1, nGray, nGreen, nBlue, nPurple, 0);

			if (!IsGrowStarValid(eNewStar))
			{
				DWORD dwError = GT_INVALID;
				lua_pushnumber(L, dwError);
				return 0;
			}

			++(pEquip->equipSpec.byGrowLevel);

			if (pEquip->equipSpec.byGrowLevel < EQUIP_GROW_MAX_LEVEL)
			{
				pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1] = eNewStar;
				switch(eNewStar)
				{
				case EEGS_Gray:
					++nGray;
					break;
				case EEGS_Green:
					++nGreen;
					break;
				case EEGS_Blue:
					++nBlue;
					break;
				case EEGS_Purple:
					++nPurple;
					break;
				default:
					break;
				}
			}		
			else if(pEquip->equipSpec.byGrowLevel == EQUIP_GROW_MAX_LEVEL)
			{
				pEquip->equipSpec.dwGrowValue = dwValueLine;
			}
			
		}
	}

	// ���ӳɳ�����
	for (int i=0; i<EQUIP_GROW_ADD_ATT_MAX_NUM; ++i)
	{
		if (P_VALID(pGrowProto->byAttOpenLevel[i]) && pEquip->equipSpec.byGrowLevel + 1 >= pGrowProto->byAttOpenLevel[i])
		{
			DWORD dwAttValue = 0;
			// �ۻ�֮ǰ���������Ժ�
			for (int j=0; j<pEquip->equipSpec.byGrowLevel; ++j)
			{
				float fpct = 1.0f;
				if(j>0) // ���ǵ�ʱ��
					MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[j-1], fpct);

				dwAttValue += pGrowProto->dwAttValue[i][j]*fpct;
			}
			// �ۼӱ���������ֵ
			DWORD dwExpValue = 0;
			for (int k=0; k<pEquip->equipSpec.byGrowLevel; ++k)
			{
				dwExpValue += pGrowProto->dwExp[k];
			}
			float fpct1 = 1.0f;
			if(pEquip->equipSpec.byGrowLevel>0)
				MGetEquipGrowRatio(pEquip->equipSpec.eGrowStar[pEquip->equipSpec.byGrowLevel-1], fpct1);
			dwAttValue += (pEquip->equipSpec.dwGrowValue - dwExpValue)*fpct1*(pGrowProto->dwAttValue[i][pEquip->equipSpec.byGrowLevel])/(pGrowProto->dwExp[pEquip->equipSpec.byGrowLevel]);

			pEquip->equipSpec.nGrowAttValue[i] = dwAttValue;
		}
	}

	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}
static int SC_DigHoleOnEquip(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}
	if (!pEquip->equipSpec.bCanCut || pEquip->equipSpec.byHoleNum >= MAX_EQUIPHOLE_NUM)
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	++(pEquip->equipSpec.byHoleNum);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);
	// �����ű�
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
		pRoleScript->OnDigHoleOnEquip(pRole, pEquip->dwTypeID, true, pEquip->equipSpec.byHoleNum);

	return 1;
}

static int SC_PutStoneOnEquip(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwItemID = (DWORD)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(dwItemID);
	if(!P_VALID(pConsolidateProto))
		return 0;

	if((INT)pEquip->equipSpec.byHoleNum == 0)
		return 0;

	bool bFull = true;
	for(int i = 0; i < pEquip->equipSpec.byHoleNum;++i)
	{
		if( !P_VALID(pEquip->equipSpec.dwHoleGemID[i]) )
		{
			bFull = false;
			break;
		}
	}
	if( bFull )
		return 0;

	// ��ⱦʯ�Ƿ�����Ƕ��װ����
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return 0;



	// ��Ƕ��ʯ
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
		{
			pEquip->equipSpec.dwHoleGemID[i] = dwItemID;
			break;
		}
	}

	pRole->CalEquipFlare(pEquip);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}


static int SC_SetEquipAdditionalAbility(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);
	DWORD   dwItemID = (DWORD)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	tagEquip* pEquip = pRole->GetItemMgr().GetEquipBarEquip(n64SerialID);
	if (!P_VALID(pEquip))
	{
		pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64SerialID);
		if (!P_VALID(pEquip))
		{
			DWORD dwError = GT_INVALID;
			lua_pushnumber(L, dwError);
			return 0;
		}
	}
	if (!P_VALID(pEquip->pEquipProto))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 0;
	}

	 tagItemProto* pItemProto = g_attRes.GetItemProto(dwItemID);
	 if(!P_VALID(pItemProto))
	 {
		 DWORD dwError = GT_INVALID;
		 lua_pushnumber(L, dwError);
		 return 0;
	 }
	// �ҵ�ǿ������
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(dwItemID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// ���ӡ�ǵȼ��Ƿ�ﵽ����
	if((pEquip->equipSpec.byBrandLevel + pItemProto->byLevel) > MAX_BRAND_LEVEL)
		return E_Consolidate_Brand_TopLevel;

	// ���װ���Ƿ��ܱ�ӡ��
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotBrand;

	// ���װ��Ǳ��ֵ�Ƿ��㹻
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// ��¼ӡ�ǵȼ�
	BYTE byLevel = pItemProto->byLevel;

	DWORD dwBrandItemTypeID = dwItemID;

	// װ����ӡ�ȼ�(������Ϊӡ�ǵȼ�)
	pEquip->equipSpec.byBrandLevel += byLevel;

	// װ�����Ӷ�Ӧǿ������
	for(INT n = 0; n < MAX_CONSOLIDATE_ROLEATT; ++n)
	{
		INT nIdx = 0;
		if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_Derate_Start &&
			pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_Derate_End)
		{
			nIdx = MTransERADerate2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
		}
		else if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_transform_Start &&
			pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_transform_End)
		{
			nIdx = MTransERATransform2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
		}
		pEquip->equipSpec.nBrandAtt[nIdx] += pConsolidateProto->tagRoleAtt[n].nAttVal;
	}


	pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));


	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
		pRoleScript->OnBrandEquip(pRole, pEquip->dwTypeID, dwBrandItemTypeID, TRUE, pEquip->equipSpec.byBrandLevel);

	pRole->CalEquipFlare(pEquip);
	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 1;
}

static int SC_GetBuffWrapTimes(lua_State* L)
{
	// �����id
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	// buff������
	DWORD	dwBuffTypeID = (DWORD)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	DWORD dwSrcID = GT_INVALID;
	INT times = pRole->GetBuffWrapTimes(dwBuffTypeID,dwSrcID);
	lua_pushnumber(L, times);
	lua_pushnumber(L, dwSrcID);
	return 2;
}

static int SC_IsFairyItem(lua_State* L)
{
	DWORD   dwItemTypeID = (DWORD)lua_tonumber(L, 1);
	tagItemProto* pProto = g_attRes.GetEquipProto(dwItemTypeID);
	bool bFairy = false;
	if( P_VALID(pProto) )
	{
		if( EIT_FaBao == pProto->eType )
			bFairy = true;
	}
	lua_pushboolean(L,bFairy);
	return 1;
}
static int SC_NotifyFBBS(lua_State* L)
{
	DWORD   dwRoleID = (DWORD)lua_tonumber(L, 1);
	int   nCmdID	= (int)lua_tonumber(L, 2);
	int   nPram		= (int)lua_tonumber(L, 3);
	g_FBBSSession.NotifyFBBS(dwRoleID,nCmdID,nPram);
	return 0;
}

static int SC_GetRoleQuestNum(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushnumber(L, dwError);
		return 1;
	}

	lua_pushnumber(L, pRole->GetCurrentQuestCount());
	return 1;
}

static int SC_IsOldSerNewPlayer(lua_State* L)
{
	DWORD dwRoleID = (DWORD)lua_tonumber(L, 1);
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		DWORD dwError = GT_INVALID;
		lua_pushboolean(L, false);
		return 1;
	}
	
	lua_pushboolean(L, pRole->IsOldSerNewPlayer());
	return 1;
}

//-------------------------------------------------------------------------------------
// �����Ƿ���ĳbuff
//-------------------------------------------------------------------------------------
int SC_IsHaveBuff(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwBuffID		=	(DWORD)lua_tonumber(L, 4);
	BOOL	bRet			=	FALSE;

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if (!P_VALID(pCreature)) return 0;

	bRet = pCreature->IsHaveBuff(dwBuffID);
	lua_pushboolean(L, bRet);

	return 1;
}

//--------------------------------------------------------------------------------------
//	��ȡ����ĳ���б�
//--------------------------------------------------------------------------------------
int SC_GetEnemyList(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwValue			=	(DWORD)lua_tonumber(L, 5);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;
	
	TMap<DWORD, DWORD> EnemyListMap;
	pCreature->GetAI()->GetEnemyList(EnemyListMap);
	if(EnemyListMap.Empty())
		return 0;

	EnemyListMap.ResetIterator();
	DWORD dwEnemyRoleID;
	DWORD dwEnemyVal;
	int nIndex = 0;
	lua_newtable(L);
	while(EnemyListMap.PeekNext(dwEnemyRoleID, dwEnemyVal))
	{
		lua_pushnumber(L, nIndex++);
		lua_pushnumber(L, dwEnemyRoleID);
		lua_settable(L, -3);
	}	

	return 1;
}

//-------------------------------------------------------------------------------------
//	���ó���б���ĳ��λ�õ��˵ĳ��ֵΪ���ٶ���
//-------------------------------------------------------------------------------------
int SC_SetEnemyValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureID	=	(DWORD)lua_tonumber(L, 3);
	DWORD	dwTargetID		=	(DWORD)lua_tonumber(L, 4);
	DWORD	dwValue			=	(DWORD)lua_tonumber(L, 5);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	Creature* pCreature = pMap->FindCreature(dwCreatureID);
	if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()) ) return 0;

	Unit* pTarget = pMap->FindUnit(dwTargetID);
	if( !P_VALID(pTarget) ) return 0;

	pCreature->GetAI()->SetEnmity(pTarget, dwValue);

	return 0;
}

int SC_GetCampByCreatureTid(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureTid	=	(DWORD)lua_tonumber(L, 3);
	ECampType eType = ECamp_Null;

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	TMap<DWORD, Creature*>& mapCreature = pMap->GetCreatureMap();

	TMap<DWORD, Creature*>::TMapIterator it = mapCreature.Begin();
	Creature* pCreature = NULL;
	while( mapCreature.PeekNext(it, pCreature) )
	{
		if (dwCreatureTid != pCreature->GetTypeID())
		{
			continue;
		}

		eType = pCreature->GetCampType();
		break;
	}

	lua_pushnumber(L, eType);

	return 0;
}

int SC_SetCampByCreatureTid(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwCreatureTid	=	(DWORD)lua_tonumber(L, 3);
	ECampType eType			=	(ECampType)lua_tointeger(L, 4);

	// �ҵ���ͼ�͹���
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if( !P_VALID(pMap) ) return 0;

	TMap<DWORD, Creature*>& mapCreature = pMap->GetCreatureMap();

	TMap<DWORD, Creature*>::TMapIterator it = mapCreature.Begin();
	Creature* pCreature = NULL;
	while( mapCreature.PeekNext(it, pCreature) )
	{
		if (dwCreatureTid != pCreature->GetTypeID())
		{
			continue;
		}

		pCreature->SetCampType(eType);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// ��������ְ
//-------------------------------------------------------------------------------------
int SC_GetRoleClergy(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	DWORD dwClergy = pRole->GetClergy();
	lua_pushnumber(L, dwClergy);
	
	return 1;
}
//-------------------------------------------------------------------------------------
static int SC_GetRoleFamilyID(lua_State* L)
{
	DWORD   dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	DWORD dwFamilyID = pRole->GetFamilyID();
	if (P_VALID(dwFamilyID))
	{
		lua_pushnumber(L, dwFamilyID);
		return 1;
	}
	else
	{
		return 0;
	}	
}

static int SC_GetFamilyMemberID(lua_State* L)
{
	DWORD   dwFamilyID		=	(DWORD)lua_tonumber(L, 1);

	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return 0;

	FamilyMember& pMembers = pFamily->GetFamilyMember();
	
	DWORD dwCount = pMembers.GetFamilyMemberCnt();
	if(dwCount <= 0)
		return 0;

	DWORD MemberIDs[MAX_FAMILY_MEMBER] = {0};
	DWORD dwNum = pMembers.GetFamilyAllMember(MemberIDs);
	for (int i =0; i<dwNum; ++i)
	{
		lua_pushnumber(L, MemberIDs[i]);
	}
	
	return dwNum;
}

static int SC_GetFamilyLeaderID(lua_State* L)
{
	DWORD   dwFamilyID		=	(DWORD)lua_tonumber(L, 1);

	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
		return 0;
	
	lua_pushnumber(L, pFamily->GetFamilyAtt().dwLeaderID);
	
	return 1;
}

static int SC_GetRoleLoverID(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	DWORD dwLoverID = pRole->GetLoverID();
	lua_pushnumber(L, dwLoverID);

	return 1;
}

static int SC_AddRoleAwardData(lua_State* L)
{
	DWORD   dwRoleID =	(DWORD)lua_tonumber(L, 1);
	INT		nData =	(int)lua_tonumber(L, 2);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) 
		return 0;

	pRole->AddRoleAwardPoint(nData);

	return 0;
}

static int SC_GetWardrobeDetailInfo(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	INT nSum = 0; // ����ֵ��
	INT nSuit1Num = 0; // �����ʱװ����
	INT nSuit2Num = 0;
	INT nSuit3Num = 0;
	INT nSuitSum = 0; // ʱװ������
	
	pRole->GetFashionTemperamentSum(nSum, nSuit1Num, nSuit2Num, nSuit3Num, nSuitSum);

	lua_pushnumber(L, nSum);
	lua_pushnumber(L, nSuit1Num);
	lua_pushnumber(L, nSuit2Num);
	lua_pushnumber(L, nSuit3Num);
	lua_pushnumber(L, nSuitSum);
	
	return 5;
}

static int SC_GetRoleAwardPoint(lua_State* L)
{
	DWORD   dwRoleID =	(DWORD)lua_tonumber(L, 1);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole)) 
	{
		lua_pushnumber(L, 0);
	}
	else
	{
		lua_pushnumber( L, pRole->GetAwardPoint() );
	}

	return 1;
}

static int SC_MultiRateReward(lua_State* L)
{
	INT nType	= lua_tonumber(L, 1);
	INT nRate		= lua_tonumber(L, 2);
	DWORD dwLastSecond	=	(DWORD)lua_tonumber(L, 3);

	g_GMPolicy.SetRate(EDoubleType(nType), nRate, (DWORD)GetCurrentDWORDTime(), dwLastSecond);

	return 0;
}

static int SC_GetWorldDataTable(lua_State* L)
{
	return 0;
	INT nIndex	= lua_tonumber(L, 1);

	if (!DataCollection_Index_Valid(nIndex))
	{
		return 0;
	}

	lua_pushnumber(L,g_DataCollection.GetData((BYTE)nIndex));

	return 1;
}

static int SC_SetWorldDataTable(lua_State* L)
{
	return 0;
	INT nIndex	= lua_tonumber(L, 1);
	DWORD dwValue		= (DWORD)lua_tonumber(L, 2);

	if (!DataCollection_Index_Valid(nIndex))
	{
		return 0;
	}

	g_DataCollection.SetData((BYTE)nIndex, dwValue);

	return 0;
}

static int SC_GetRoleDataTable(lua_State* L)
{
	return 0;
	DWORD dwRoleID	= lua_tonumber(L, 1);
	INT nIndex	= lua_tonumber(L, 2);

	if (!DataCollection_Role_Index_Valid(nIndex))
	{
		return 0;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	lua_pushnumber(L, pRole->GetData((BYTE)nIndex));

	return 1;
}

static int SC_SetRoleDataTable(lua_State* L)
{
	return 0;
	DWORD dwRoleID	= lua_tonumber(L, 1);
	INT nIndex	= lua_tonumber(L, 2);
	DWORD dwValue		= (DWORD)lua_tonumber(L, 3);

	if (!DataCollection_Role_Index_Valid(nIndex))
	{
		return 0;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return 0;
	}

	pRole->SetData((BYTE)nIndex, dwValue);

	return 0;
}

static int SC_GetRoleSpecFlag(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	PlayerSession* pSession = pRole->GetSession();

	lua_pushnumber(L, pSession->GetAccountSpec());

	return 1;
}

static int SC_SetRoleSpecFlag(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	INT nValue	= lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	PlayerSession* pSession = pRole->GetSession();

	pSession->SetAccountSpec((EAccountSpec)nValue);

	return 0;
}

static int SC_GetAccountReactiveStatus(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->IsAccountReactiver());
	return 1;
}

static int SC_GetAccountReactiveKey(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetAccountReactiveKey());

	return 1;
}

static int SC_GetAccountReactiveScore(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	lua_pushnumber(L, pRole->GetAccountReactiveScore());

	return 1;
}

static int SC_PopInputReactiveKey(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	PlayerSession* pSession = pRole->GetSession();
	if (P_VALID(pSession))
	{
		tagNS_PlsInputReactiveKeyCode send;
		pSession->SendMessage(&send, send.dwSize);
	}
	return 0;
}


static int SC_GetRoleSoul(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	lua_pushinteger(L, pRole->GetGod());
	lua_pushinteger(L, pRole->GetMonster());

	return 2;
}

static int SC_SetRoleSoul(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	INT nGod	= lua_tonumber(L, 4);
	INT nMonster	= lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	INT nCurGod = pRole->GetGod();
	INT nCurMonster = pRole->GetMonster();

	INT nTempGod = nGod - nCurGod;
	INT nTempMonster = nMonster - nCurMonster;

	pRole->ChangeSoul(nTempGod,nTempMonster);

	//pRole->SetGod(nGod);
	//pRole->SetMonster(nMonster);

	return 0;
}

static int SC_GetRoleSoulPoint(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	lua_pushinteger(L, pRole->GetGodPoint());
	lua_pushinteger(L, pRole->GetMonsterPoint());

	return 2;
}

static int SC_SetRoleSoulPoint(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	INT nGodPoint	= lua_tonumber(L, 4);
	INT nMonsterPoint	= lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	INT nCurGodPoint = pRole->GetGodPoint();
	INT nCurMonsterPoint = pRole->GetMonsterPoint();

	INT nTempGodPoint = nGodPoint - nCurGodPoint;
	INT nTempMonsterPoint = nMonsterPoint - nCurMonsterPoint;

	pRole->ChangeSoulPoint(nTempGodPoint,nTempMonsterPoint);
	//pRole->SetGodPoint(nGodPoint);
	//pRole->SetMonsterPoint(nMonsterPoint);

	return 0;
}

static int SC_GetAccountReactiveLevel(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);

	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	lua_pushinteger(L, pRole->GetRewardReactiveLevel());

	return 1;

}

static int SC_SetAccountReactiveLevel(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID	= lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	DWORD dwLevel	= lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	pRole->SetRewardReactiveLevel((BYTE)dwLevel);

	return 0;
}

static int SC_GetRoleSoulCrystalLvl(lua_State* L)
{
	DWORD dwMapID	= lua_tonumber(L, 1);
	DWORD dwInstanceID = lua_tonumber(L, 2);
	DWORD dwRoleID	= lua_tonumber(L, 3);
	DWORD dwSerialHigh = lua_tonumber(L, 4);
	DWORD dwSerialLow = lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	tagItem* pItem = pRole->GetItemMgr().GetEquipBar().GetItem((INT64)(dwSerialHigh*100000 + dwSerialLow));
	if (!P_VALID(pItem) || !MIsSoulCrystal(pItem))
	{
		return 0;
	}

	tagSoulCrystal* pSoul = (tagSoulCrystal*) pItem;
	
	lua_pushinteger(L, pSoul->SoulSpec.bySoulCrystalLvl);

	return 1;
}


static int SC_GetSkillActiveTime(lua_State* L)
{
	DWORD dwMapID		=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	DWORD dwSkillID		=	(DWORD)lua_tonumber(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if(!P_VALID(pSkill))	
		return 0;

	DWORD dwTime = (DWORD)pSkill->GetActiveTime();
	lua_pushinteger(L, dwTime);

	return 1;
}

static int SC_SetSkillActiveTime(lua_State* L)
{
	DWORD dwMapID				=	(DWORD)lua_tonumber(L, 1);
	DWORD dwInstanceID			=	(DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID				=	(DWORD)lua_tonumber(L, 3);
	DWORD dwSkillID				=	(DWORD)lua_tonumber(L, 4);
	DWORD dwSkillActiveTime		=	(DWORD)lua_tonumber(L, 5);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
		return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
		return 0;

	//Skill* pSkill = pRole->GetSkill(dwSkillID);
	//if(!P_VALID(pSkill))	
	//	return 0;

	//pSkill->SetActiveTime(dwSkillActiveTime);

	pRole->ContinueSoulSkillByBook(dwSkillID,dwSkillActiveTime);

	return 0;
}

//-------------------------------------------------------------------------------------
// ����dota�����Ϣ���ͻ���
//-------------------------------------------------------------------------------------
static int SC_DotaSendMsg(lua_State* L)
{	
	DWORD dwMapID			= (DWORD)lua_tonumber(L, 1);
	DWORD dwMapInstanceID	= (DWORD)lua_tonumber(L, 2);
	DWORD dwRoleID			= (DWORD)lua_tonumber(L, 3);
	DWORD dwTypeID			= (DWORD)lua_tonumber(L, 4);
	const char* pMsg		= lua_tostring(L, 5);
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwMapInstanceID);
	if(!P_VALID(pMap)) return 0;
	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole)) return 0;
	DWORD dwSize = sizeof(tagNS_DotaMsg)  + strlen(pMsg);
	MCREATE_MSG(pSend, dwSize, NS_DotaMsg);
	pSend->dwMsgType = dwTypeID;
	pSend->dwMsgSize = strlen(pMsg);
	strcpy(pSend->msg, pMsg);
	pSend->msg[pSend->dwMsgSize] = 0;
	pRole->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	return 0;
}

static int SC_GetEquipGrowAttID(lua_State* L)
{
	DWORD dwRoleID = lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);	//װ��64λid

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//��ȡ��ɫ
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip) || pEquip->equipSpec.dwGrowTypeID == 0 || pEquip->equipSpec.dwGrowTypeID == GT_INVALID)	//��Ч
	{
		return 0;
	}

	lua_pushinteger(L, pEquip->equipSpec.dwGrowTypeID);
	return 1;
}
static int SC_SetEquipGrowAttID(lua_State* L)
{

	DWORD dwRoleID = lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);	//װ��64λid
	DWORD dwGrowID = lua_tonumber(L, 4);			//�ɳ�id

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//��ȡ��ɫ
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip))	//��Ч
	{
		return 0;
	}

	//���óɳ�id����ճɳ�����
	pEquip->equipSpec.dwGrowTypeID = dwGrowID;	// �ɳ�����ID
	pEquip->equipSpec.dwGrowValue = 0;			// ��ǰ�ɳ�ֵ
	pEquip->equipSpec.byGrowLevel = 0;			// ��ǰ�ɳ��ȼ���0-11
	// nTotalGrowTimes;							// �ܹ��ɳ����Ĵ���		//������ֵ�ݲ����
	// nTotalIMUseTimes;						// �ܹ�ʹ�ù�IM���ߵĴ���
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		pEquip->equipSpec.eGrowStar[i] = EEGS_Null;	//����
	}
	for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
	{
		pEquip->equipSpec.nGrowAttValue[i] = 0;		//����ֵ
	}

	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 0;
}
static int SC_GetEquipStoneAddLevel(lua_State* L)	//��ȡ��ʯ����ֵ
{
	DWORD dwRoleID = lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID = pop_64bit_data(L, 2, 3);	//װ��64λid

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//��ȡ��ɫ
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip))	//��Ч
	{
		return 0;
	}
	lua_pushinteger(L, pEquip->equipSpec.byDiamondLevel);
	return 1;
}
static int SC_SetEquipStoneAddLevel(lua_State* L)
{
	DWORD dwRoleID		= lua_tonumber(L, 1);			//role ID
	INT64 n64SerialID	= pop_64bit_data(L, 2, 3);	//װ��64λid
	BYTE byDiamondLevel = (BYTE)lua_tonumber(L, 4);

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);	//��ȡ��ɫ
	if(!P_VALID(pRole))
		return 0;

	tagEquip* pEquip = (tagEquip*)(pRole->GetItemMgr().GetBagItem(n64SerialID));
	if(!P_VALID(pEquip))	//��Ч
	{
		return 0;
	}
	pEquip->equipSpec.byDiamondLevel = byDiamondLevel;

	ItemMgr & itemMgr = pRole->GetItemMgr();
	itemMgr.UpdateEquipSpec(*pEquip);

	return 0;
}

static int SC_AddMallMark(lua_State* L)
{
	DWORD   dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD   dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD   dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nValue			=	lua_tointeger(L, 4);
	INT     nCmdID			=   lua_tointeger(L, 5);



	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	CurrencyMgr& currencyMgr = pRole->GetCurMgr();
	currencyMgr.IncVIPPoint(nValue, (ELogCmdID)nCmdID);	//1��ʾͨ��ʹ�õ��߲����Ļ���

	return 0;
}

static int SC_SetGuildValue(lua_State* L)
{
	DWORD dwGuildID = lua_tonumber(L, 1);
	DWORD dwGuildValue1 = lua_tonumber(L, 2);
	DWORD dwGuildValue2 = lua_tonumber(L, 3);

	Guild* pGuild = NULL;
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if(!P_VALID(pGuild))
		return 0;

	pGuild->SetGuildValue1(dwGuildValue1);
	pGuild->SetGuildValue2(dwGuildValue2);

	g_guildMgr.SendAllGuild2DB();

	return 0;
}

static int SC_GetGuildValue(lua_State* L)
{
	DWORD dwGuildID = lua_tonumber(L, 1);

	Guild* pGuild = NULL;
	pGuild = g_guildMgr.GetGuild(dwGuildID);
	if(!P_VALID(pGuild))
		return 0;

	DWORD dwGuildValue1 = pGuild->GetGuildValue1();
	DWORD dwGuildValue2 = pGuild->GetGuildValue2();

	lua_pushinteger(L, dwGuildValue1);
	lua_pushinteger(L, dwGuildValue2);

	return 2;
}

//Ԫ��ǰ�ȼ�����ֵ���޵Ļ�ȡ
static int SC_GetSoulMaxLevelExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();
	INT64 n64MaxLevelExp = holySoulInfo.SoulAttEx.n64MaxExp;

	push_64bit_data(L, n64MaxLevelExp);
	return 2;

}

//�ۼ�Ԫ����
static int SC_AddHolySoulExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nAddendExp		=	(INT)lua_tointeger(L,4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	if (nAddendExp <= 0)
	{
		return 0;
	}

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	DWORD dwErrorCode;
	dwErrorCode = holySoul.AddExp((INT64)nAddendExp);

	lua_pushnumber(L, dwErrorCode);
	return 1;
}

//��ȡԪ��ĵ�ǰ�ȼ�
static int SC_GetSoulCurLevel(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();

// 	INT nLevel = holySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	if (nLevel <= 0)
// 	{
// 		return 0;
// 	}
	//lua_pushnumber(L, nLevel);
	return 1;
}

//��ȡԪ���ת���ȼ�
static int SC_GetHolySoulRebornLevel(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();
// 	INT nRebornLevel = (INT)(holySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] / 10000);
// 	if (nRebornLevel <= 0)
// 	{
// 		return 0;
// 	}
// 
// 	lua_pushnumber(L, nRebornLevel);
	return 1;
}

//��ȡԪ���Ĭ��ֵ
static int SC_GetCurCovalue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT nCurCovalue = holySoul.GetHolySoulAtt(EHEAT_Covalue);
	if (nCurCovalue < 0)
	{
		return 0;
	}

	lua_pushnumber(L, nCurCovalue);
	return 1;
}

//�ۼ�Ԫ���Ĭ��ֵ
static int SC_AddCurCovalue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nCoValue		=	lua_tointeger(L, 4);
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}
	holySoul.OnChangeAttVal(EHEAT_Covalue,nCoValue);
	return 0;
}

//��ȡԪ��ֵ
static int SC_GetCurSoulValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT nCurSoulValue = holySoul.GetHolySoulAtt(EHEAT_SoulValue);
	if (nCurSoulValue < 0)
	{
		return 0;
	}

	lua_pushnumber(L, nCurSoulValue);
	return 1;
}

//�ۼ�Ԫ��ֵ
static int SC_AddCurSoulValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nSoulValue		=	lua_tointeger(L, 4);
	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	holySoul.OnChangeAttVal(EHEAT_SoulValue,nSoulValue);
	return 0;
}

//��ȡԪ��ֵ�����ֵ
static int SC_GetMaxSoulValue(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT nMaxSoulValue = holySoul.GetHolySoulAtt(EHEAT_MaxSoulValue);
	if (nMaxSoulValue < 0)
	{
		return 0;
	}

	lua_pushnumber(L, nMaxSoulValue);
	return 1;
}

//��ȡԪ��ǰ�ľ���ֵ
static int SC_GetSoulCurExp(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	HolySoul& holySoul = pRole->GetHolySoul();
	if (!holySoul.IsActivated())
	{
		lua_pushnumber(L, -1);
		return 1;
	}

	INT64 n64CurExp = holySoul.GetCurrExp();
	if (n64CurExp < 0)
	{
		return 0;
	}

	push_64bit_data(L, n64CurExp);
	return 2;
}

static int SC_SetHunJingStrength(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT64	n64ItemID		=	pop_64bit_data(L, 4, 5);
	DWORD	dwStrength		=	(DWORD)lua_tonumber(L, 6);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	tagEquip* pEquip = (tagEquip*)pRole->GetItemMgr().GetBagItem(n64ItemID);
	if(P_VALID(pEquip))
	{
		if(MIsSoulCrystal(pEquip))
		{
			((tagSoulCrystal*)(pEquip))->SoulSpec.dwSoulStrength = dwStrength;
		}
	}
}

static int SC_SetWarGold(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);
	INT		nWarGold		=	lua_tointeger(L, 4);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	WarRole* pWarRole = pRole->GetWarRole();
	if(!P_VALID(pWarRole))	return 0;

	pWarRole->AddGlod(nWarGold);

	return 0;
}

static int SC_GetWarGold(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	return 0;

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	return 0;

	WarRole* pWarRole = pRole->GetWarRole();
	if(!P_VALID(pWarRole))	return 0;

	pWarRole->GetGold();

	return 0;
}

//���¼��ؼ���Ԫ������ֵ,�����ڽ��Ԫ�����Ա�ı�����Ԫ������ֵ����ȷ��bug
static int SC_ReloadHolySoulAtt(lua_State* L)
{
	DWORD	dwMapID			=	(DWORD)lua_tonumber(L, 1);
	DWORD	dwInstanceID	=	(DWORD)lua_tonumber(L, 2);
	DWORD	dwRoleID		=	(DWORD)lua_tonumber(L, 3);

	Map* pMap = g_mapCreator.GetMap(dwMapID, dwInstanceID);
	if(!P_VALID(pMap))	
	{
		return 0;
	}

	Role* pRole = pMap->FindRole(dwRoleID);
	if(!P_VALID(pRole))	
	{
		return 0;
	}

	HolySoul& holySoul = pRole->GetHolySoul();	
	if (!holySoul.IsActivated())
	{
		return 0;
	}

	//���Ԫ��ID
	DWORD dwSoulID = holySoul.GetID();
	const tagSoulAttProto* pCurSoulAttProto = g_attRes.GetSoulAttProto(dwSoulID);
	if (!P_VALID(pCurSoulAttProto))
	{
		return 0;
	}

	//����ReloadProcHolySoulAtt֮ǰ����
	DWORD dwErrCode =holySoul.ReCalHolyChangeRoleAtt(pCurSoulAttProto,true);
	if (dwErrCode != E_Success)
	{
		return 0;
	}	

	dwErrCode = holySoul.ReloadProcHolySoulAtt(pCurSoulAttProto);
	if (dwErrCode != E_Success)
	{
		return 0;
	}	

	//���µ����ݿ�
	holySoul.SaveHolySoulInfoToDB(HolySoul::ESaveType_Update);
	//ͬ���ͻ���Ԫ����Ϣ�ı�
	holySoul.SendHolySoulAttToClient();

	return 0;
}


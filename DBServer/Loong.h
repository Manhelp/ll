//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong.h
// author: Sxg
// actor:
// data: 2008-05-08
// last:
// brief: �����ܶ���
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/base_define.h"
//-----------------------------------------------------------------------------
#define ID_GAMESERVER		1
#define SLEEPTIME			100

//-----------------------------------------------------------------------------
struct tagNDBS_LoadRole;
struct tagNDBS_LoadSimRole;
struct tagNDBS_CreateRole;
struct tagNDBS_SaveRole;
struct tagNDBS_LoadBaiBao;
struct tagNDBS_LoadBaiBaoLog;
struct tagNDBS_GetAllGPInfo;

class RTSession;
class CLoongDB;
class CLoongRank;
#include "../serverdefine/mutex.h"
//-----------------------------------------------------------------------------
// ��Ϣ���ն���(��ֱ��ʹ��MsgQueueTS����һ���ڴ���估����)
//-----------------------------------------------------------------------------
class DBMsgQueue
{
	friend class RTSession;

public:
	DBMsgQueue()	{ m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);}
	~DBMsgQueue()	{ CloseHandle(m_hEvent); }

public:
	// �����Ϣ������β�������Ƿ���ӳɹ�
	VOID AddMsg(LPBYTE pMsg)
	{
		m_MsgQueue.PushBack(pMsg);
		
		// �����߳�
		::SetEvent(m_hEvent);
	}

	// �Ӷ���ͷȡ��Ϣ������踺���ͷ��ڴ�
	LPBYTE	GetMsg()
	{
		return m_MsgQueue.PopFront();
	}

	// ȡ���¼����������м�����Ϣʱ�����¼�������
	HANDLE	GetEvent() { return m_hEvent; }

	// �õ������е���Ϣ��Ŀ
	INT		GetMsgNum() { return m_MsgQueue.Size(); }

private:
	TSafeList<LPBYTE>	m_MsgQueue;
	HANDLE				m_hEvent;
};

//-----------------------------------------------------------------------------
// DB�����������
//-----------------------------------------------------------------------------
class CLoong
{
public:
	CLoong();
	~CLoong();

public:
	// ��ʼ������(���ȡ�����ļ�)
	BOOL Init(HINSTANCE hInst);
	// �Ƿ���Ҫ�ر�
	BOOL IsTerminate() { return m_bTerminate; }
	// ��ѭ��
	VOID MainLoop();
	// �ر��߳�
	VOID ShutDown() { InterlockedExchange((long*)(&m_bTerminate), TRUE); }

	BOOL IsConnect() { return !m_bGameServerLogout && m_bGameServerConnect; }

	VOID SendMsg(PVOID pSend, DWORD dwSize);

public:
	DWORD GetHeartbeat() const { return m_Status.dwHeartbeat; }


private: // �������
	// ����UI����
	BOOL CreateUI(HINSTANCE hInst);
	// ����UI����
	VOID RefreshUI();

private: // ��Ϸ�������������(����dwDummyΪ����NetCmdMgr����Ҫ��)
	// ����������Ҽ�����Ϣ
	DWORD LoadAllRoleInfo(DWORD pDBMsg, DWORD dwDummy);
	// �����ɫ������Ϣ(�������ȼ������κ͵�ǰ���ڵ�ͼ��)
	DWORD LoadSimRole(DWORD pDBMsg, DWORD dwDummy);
	// ������ص�����ɫsimple��Ϣ
	DWORD LoadOneRoleInfo(DWORD pDBMsg, DWORD dwDummy);
	// �����ɫ��Ϣ(ѡ����ɫ������Ϣ-- roledata, skill, buff, item, equip...)
	DWORD LoadRole(DWORD pDBMsg, DWORD dwDummy);
	//// ɾ����ҵ�buff������item/equip��.����������ݿ�����Ҷ���ļ�¼
	//VOID DeleteRecord(DWORD pDBMsg, DWORD dwDummy);
	// ����洢��ɫ����
	VOID SaveRole(DWORD pDBMsg, DWORD dwDummy);
	// ���󴴽���ɫ
	DWORD CreateRole(DWORD pDBMsg, DWORD dwDummy);
	// ����ɾ����ɫ
	DWORD DelRole(DWORD pDBMsg, DWORD dwDummy);
	// ����ָ���ɫ
	DWORD ResumeRole(DWORD pDBMsg, DWORD dwDummy);
	// ����洢��ɫ��������
	VOID SaveRoleInfo(DWORD pDBMsg, DWORD dwDummy);
	
	// �����޸Ľ�ɫ����
	DWORD ChangeRoleName(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeRoleGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeRoleDressMdID(DWORD pDBMsg, DWORD dwDummy);

	// ��ʼ����Ϸ����
	DWORD InitWorldOK(DWORD pDBMsg, DWORD dwDummy);

	// ��Ϸ����ر�
	DWORD WorldShutDown(DWORD pDBMsg, DWORD dwDummy);
	
	// ��LoongWorld���س�ʼ�����
	DWORD LoongWorldInitOK(DWORD pDBMsg, DWORD dwDummy);

	// ��ȡ��Ʒ64λ��ŵ������Ϣ
	DWORD GetItemInfo(DWORD pDBMsg, DWORD dwDummy);	
	// ��ȡ����Ʒ(��item_baibao����)���ٱ�����
	DWORD LoadNewItem2BaiBao(DWORD pDBMsg, DWORD dwDummy);
	// ��ȡ�ٱ�����Ԫ����Ϣ
	DWORD LoadBaiBaoYuanBaoInfo(DWORD pDBMsg, DWORD dwDummy);
	// ��item_baibao����ɾ��ָ����Ʒ
	DWORD DelBaiBaoItem(DWORD pDBMsg, DWORD dwDummy);
	// ��item_baibao����ɾ��ָ��װ��
	DWORD DelBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy);
	// ��ȡ��Ҫ��¼����ƷTypeID
	DWORD LoadItemNeedLog(DWORD pDBMsg, DWORD dwDummy);
	// ��ȡ������ҵ�Ԫ�������˻�
	DWORD LoadAllYBAccount(DWORD pDBMsg, DWORD dwDummy);
	// ��ȡ���е�Ԫ�����׶���
	DWORD LoadAllYBOrder(DWORD pDBMsg, DWORD dwDummy);
	// ����һ��Ԫ�������˻�
	DWORD InsertYBAccount(DWORD pDBMsg, DWORD dwDummy);
	// ����Ԫ�������˻���Ԫ������
	DWORD UpdateAccountYB(DWORD pDBMsg, DWORD dwDummy);
	// ����Ԫ�������˻��Ľ�Ǯ����
	DWORD UpdateAccountSilver(DWORD pDBMsg, DWORD dwDummy);
	// ����Ԫ�������˻����۶������ύ״̬
	DWORD UpdateSubmitSell(DWORD pDBMsg, DWORD dwDummy);
	// ����Ԫ�������˻��չ��������ύ״̬
	DWORD UpdateSubmitBuy(DWORD pDBMsg, DWORD dwDummy);
	// ����Ԫ�����׶���
	DWORD InsertOrder(DWORD pDBMsg, DWORD dwDummy);
	// ������ر�Ԫ�����׶���
	DWORD CompleteOrder(DWORD pDBMsg, DWORD dwDummy);
	// ����Ԫ�����׶���
	DWORD RefreshOrder(DWORD pDBMsg, DWORD dwDummy);
	// ��ѯһ������ҽ��׶���
	DWORD GetRoleYBOrder(DWORD pDBMsg, DWORD dwDummy);
	// ɾ�����ݿ���ʱ�䳬����������ж���
	DWORD DeleteYBOrder(DWORD pDBMsg, DWORD dwDummy);

	// �ٱ���Log���
	// ȡ��ָ���˺���Log��Ϣ
	DWORD GetBaibaoLogInfo(DWORD pDBMsg, DWORD dwDummy);
	// ����һ��Log��¼
	DWORD InsertBaibaoLog(DWORD pDBMsg, DWORD dwDummy);
	
	// �ʺ�ͨ���������
	DWORD SetSafeCode(DWORD pDBMsg, DWORD dwDummy);
	DWORD ResetSafeCode(DWORD pDBMsg, DWORD dwDummy);
	DWORD CancelSafeCodeReset(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateWareSize(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateWareMoney(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateBaiBaoYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateBagYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateBagGold(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdatePersonalSet(DWORD pDBMsg, DWORD dwDummy);

	DWORD ChangeBagPsd(DWORD pDBMsg, DWORD dwDummy);

	// ��ȡ�̶������
	DWORD LoadActivityData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveActivityData(DWORD pDBMsg, DWORD dwDummy);
	DWORD ClearMergedScriptData(DWORD pDBMsg, DWORD dwDummy);

	// ʵ�����а�
	DWORD InitRankings(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadStrengthRankings();
	DWORD LoadLevelRankings();
	DWORD LoadFameRankings();
	DWORD LoadPetRankings();
	DWORD LoadEquipRankings();
    DWORD LoadWuXunRankings();
	DWORD LoadFlowerRankings();
	DWORD LoadEggRankings();
	DWORD UpdateRankings(DWORD pDBMsg, DWORD dwDummy);

	// �����ɫ����
	DWORD UpdateRoleAtt(DWORD pDBMsg, DWORD dwDummy);

	// �����ɫ��
	DWORD UpdateRoleAttPoint(DWORD pDBMsg, DWORD dwDummy);

	// �����ɫ���ʵ�
	DWORD UpdateRoleTalentPoint(DWORD pDBMsg, DWORD dwDummy);

	//�����¼ʽ���ͷ�������Ĵ��͵���Ϣ
	DWORD UpdateRoleItemTransport( DWORD pDBMsg, DWORD dwDummy );
	//���
	DWORD RoleGetMarriage(DWORD pDBMsg, DWORD dwDummy);
	//���
	DWORD RoleBreakMarriage(DWORD pDBMsg, DWORD dwDummy);

	// ��ְ��ѡ
	DWORD LoadClergyRank(DWORD pDBMsg, DWORD);
	VOID LoadClergyMarsRank();
	VOID LoadClergyApolloRank();
	VOID LoadClergyRabbiRank();
	VOID LoadClergyPeaceRank();
	DWORD ChangeCandidateRecvFlag(DWORD pDBMsg, DWORD);
	DWORD GetClergyCandidateCount(DWORD pDBMsg, DWORD);
	DWORD CancelPreClergy(DWORD pDBMsg, DWORD);

	//ϡ������
	DWORD LoadAllRareArea(DWORD pDBMsg,DWORD dwDummy);
	DWORD SaveRareItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD UnSaveRareItem(DWORD pDBMsg, DWORD dwDummy);

	//DWORD CreateRareArea(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateRareArea(DWORD pDBMsg,DWORD dwDummy);
	//DWORD DeleteRareArea(DWORD pDBMsg,DWORD dwDummy);

	//����
	DWORD DeleteRoleArea(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateRoleArea(DWORD pDBMsg, DWORD dwDummy);
	//// ������Ʒ��ȴʱ��
	//DWORD ReplaceItemCDTime(DWORD pDBMsg, DWORD dwDummy);
	
private: // ʵʱ����(Ϊ�˴����Ϸ���)
	// ����
	DWORD InsertFriend(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFriend(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFriend(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteAllFriRel(DWORD pDBMsg, DWORD dwDummy);

	// �Ѻö�
	DWORD InsertFriendship(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFriendship(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFriendship(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteAllFriValRel(DWORD pDBMsg, DWORD dwDummy);

	// ������
	DWORD InsertBlack(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteBlack(DWORD pDBMsg, DWORD dwDummy);

	// ���
	DWORD InsertEnemy(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteEnemy(DWORD pDBMsg, DWORD dwDummy);

	// ȡ���г���б����н�ɫIDΪdwRoleID�Ľ�ɫ��ID
	DWORD GetRoleListOfEnemy(DWORD pDBMsg, DWORD dwDummy);

	// ����
	DWORD AcceptQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD CompleteQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD DailyCompleteQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD DiscardQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD ClearDailyCompleteQuest(DWORD pDBMsg, DWORD dwDummy);

	// ����
	DWORD InsertSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateSkill(DWORD pDBMsg, DWORD dwDummy);
	
	// ��Ʒ&װ��
	DWORD InsertItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertEquip(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelEquip(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelEquipFromRole(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateEquipSpec(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateEquipSingleAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveEquipSignature(DWORD pDBMsg, DWORD dwDummy);

	DWORD DelEquipdelByTime(CHAR *pszTime);
	DWORD DelItemdelByTime(CHAR *pszTime);
	// Jason 2010-4-14 v2.0.0 �������
	DWORD InsertFabao(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateFabaoSpec(DWORD pDBMsg,DWORD dwDummy);
	// �꾧
	DWORD InsertSoulCrystal(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateSoulCrystalSpec(DWORD pDBMsg,DWORD dwDummy);

	// wcy 2012-03-31 ʥ�����
	DWORD InsertHoly(DWORD pDBMsg,DWORD dwDummy);	
	DWORD UpdateHolySpec(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolyCoValue(DWORD pDBMsg,DWORD dwDummy);

	// wcy 2012-04-18 ʥ�����
	DWORD InsertHolyEquip(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolyEquipSpec(DWORD pDBMsg,DWORD dwDummy);
	

	// �ٱ���(item_baibao��)����Ʒ
	DWORD InsertBaiBaoItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy);

	// �ƺ�
	DWORD InsertTitle(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveTitle(DWORD pDBMsg, DWORD dwDummy);
	// ��ɫ����
	DWORD UpdateVCard(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadVCard(DWORD pDBMsg, DWORD dwDummy);

	// �����������á������䱦
	DWORD LoadRepRank(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadFameHallInit(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertActTreasure(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertFameHallEnterSnap(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateRepRstTime(DWORD pDBMsg, DWORD dwDummy);

	// �����������
	
	DWORD LoadAllEmptyGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadAllGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD CheckGuildInitOK(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD DismissGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeGuildTenet(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveGuildAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeGuildName(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertGuildName(DWORD pDBMsg, DWORD dwDummy);
	

	// ���ɳ�Ա�������
	DWORD LoadAllGuildMember(DWORD pDBMsg, DWORD dwDummy);
	DWORD JoinGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD LeaveGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeGuildPos(DWORD pDBMsg, DWORD dwDummy);
	DWORD GuildWarePrivilege(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeMemberContribution(DWORD pDBMsg, DWORD dwDummy);
	// Jason 2010-3-11 v1.4.0���ɹ�ѫ�仯
	DWORD GuildMemberChangeExploit(DWORD pDBMsg,DWORD dwDummy);

	// ���ɲֿ�
	DWORD LoadGuildWareItems(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveGuildWareItems(DWORD pDBMsg, DWORD dwDummy);

	// ������ʩ
	DWORD LoadGuildFacilities(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertGuildFacilities(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateGuildFacilities(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveAllFacilities(DWORD pDBMsg, DWORD dwDummy);

	// ���ɼ���
	DWORD LoadGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeResearchGuildSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);

	// ��������
	DWORD LoadGuildCommerceInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateCommodityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateCommodityTael(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveCommodityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveCommodityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SetCommendation(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy);

	// �������
	DWORD GetPetInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreatePet(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeletePet(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdatePet(DWORD pDBMsg, DWORD dwDummy);

	DWORD InsertPetSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelPetSkill(DWORD pDBMsg, DWORD dwDummy);

	// �̳����
	DWORD LoadAllGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD AddGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveGuildGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateGPTime(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadLimitItemInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveLimitItemInfo(DWORD pDBMsg, DWORD dwDummy);
	
	

	// VIP̯λ���
	DWORD GetAllVIPStallInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD ApplyVIPStall(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateVIPStall(DWORD pDBMsg, DWORD dwDummy);

	DWORD UpdateVNBPlayer(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadVNBData(DWORD pDBMsg, DWORD dwDummy);
	DWORD AddBonusPoint(DWORD pDBMsg, DWORD dwDummy);

	DWORD LoadLeftMsg(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertLeftMsg(DWORD pDBMsg, DWORD dwDummy);
//	DWORD UpdateLeftMsg(DWORD pDBMsg, DWORD dwDummy);
	DWORD ClearLeftMsg(DWORD pDBMsg, DWORD dwDummy);

	//	��������������
	DWORD UpdateTreasureSum(DWORD pDBMsg, DWORD dwDummy);

	// ����ϵͳ
	DWORD LoadCityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateCityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveCityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateCityAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD GetGuildClanValue(DWORD pDBMsg, DWORD dwDummy);

	// ʦͽ
	DWORD LoadMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveOneMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteAnApprentice(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadForceBreakOutApprenticeList(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertForceBreakOutApprentice(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveForceBreakOutApprentice(DWORD pDBMsg, DWORD dwDummy);
	DWORD ModifyApprenticeJingWuPoint(DWORD pDBMsg, DWORD dwDummy);

	//������
	DWORD SaveSerialVal(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveArenaData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadArenaSystemSerialVal(DWORD pDBMsg, DWORD dwDummy);

	//ѡ��
	DWORD SaveShowPlayerData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveShowPlayerPoint(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveVotePlayerPoint(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveShowPlayerDataCount(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveVotePlayerDataCount(DWORD pDBMsg, DWORD dwDummy); 
	
	DWORD GetShowPlayerDataCount();
	DWORD GetVotePlayerDataCount();
	DWORD LoadShowPlayerData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadVotePlayerData(DWORD pDBMsg, DWORD dwDummy);

	//�����
	DWORD LoadActivetyRankData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveActivityRankData(DWORD pDBMsg, DWORD dwDummy);

	//bloodbrother
	DWORD ClearTeatherInfo(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveAccountSpec(DWORD pDBMsg, DWORD dwDummy);

	DWORD AccountReactiveOrNot(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateRewardReactiveLevel(DWORD, DWORD);

private: // log
	DWORD LogSilver(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogExVolume(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogVIPPoint(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogTimeStat(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogMallSell(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogMallSellPack(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogItemTimes(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogGodBid(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogBattlefield(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogAccountOnlineTime(DWORD pDBMsg, DWORD dwDummy);

	DWORD LogFund(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogMaterial(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogReputation(DWORD pDBMsg, DWORD dwDummy);

	DWORD LogGMCmd(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogPet(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogFairyBirth(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogRoleOnlineInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogWorldData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogRolelogData(DWORD pDBMsg, DWORD dwDummy);

	// log ʵ�ʴ���
	DWORD _LogSilver(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogExVolume(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogVIPPoint(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogTimeStat(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogMallSell(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogMallSellPack(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogItemTimes(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogGodBid(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogBattlefield(DWORD pDBMsg, DWORD dwDummy);
	// log ʵ�ʴ���

	DWORD _LogFund(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogMaterial(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogReputation(DWORD pDBMsg, DWORD dwDummy);

	DWORD _LogGMCmd(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogPet(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogFairyBirth(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogRoleOnline(DWORD pDBMsg, DWORD dwDummy);

	//external links from table
	DWORD LoadExtLinks(DWORD pDBMsg, DWORD dwDummy);
	//���߹һ�
	DWORD OfflineExpReward(DWORD pDBMsg,DWORD dwDummy);
	// Jason 2010-1-27 v1.3.2 ���߽���/��һع�
	DWORD LoadAccountInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD SaceAccountInfo(DWORD pDBMsg,DWORD );


	//�ɽ�����
	DWORD SaveMiraclePrcNum(DWORD pDBMsg,DWORD dwDummy);

	DWORD SaveItemLockInfo(DWORD pDBMsg, DWORD);

	DWORD SaveItemBindInfo(DWORD pDBMsg, DWORD);
	//����������ȡ
	DWORD SaveShortCut_QuickBar(DWORD pDBMsg, DWORD);
	DWORD LoadShortCut_QuickBar(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveRoleSpeak(DWORD pDBMsg, DWORD);

	// �����Ѿ�����������״̬
	DWORD SaveOpenTrainState(DWORD pDBMsg, DWORD dwDummy);

	DWORD CheckName(DWORD pDBMsg, DWORD);	

	DWORD SaveAvatarFaceAndHair(DWORD pDBMsg, DWORD);	

	// ����ս��
	DWORD ClearExpireGuildWarDeclare(DWORD pDBMsg, DWORD);	
	DWORD InsertGuildWarDeclare(DWORD pDBMsg, DWORD);	
	DWORD SetGuildWarDeclareEnded(DWORD pDBMsg, DWORD);	
	DWORD LoadGuildWarDeclare(DWORD pDBMsg, DWORD);	

	// ս��
	DWORD LoadBattleFieldAttackSignUpInfo(DWORD pDBMsg, DWORD);	
	DWORD LoadAllicanceGuildInfo(DWORD pDBMsg, DWORD);	
	DWORD InsertIntoAttackSignUpList(DWORD pDBMsg, DWORD);	
	DWORD DeleteFromAttackSignUpList(DWORD pDBMsg, DWORD);	
	DWORD ClearAttackSignUpList(DWORD pDBMsg, DWORD);
	DWORD ModifyBidFund(DWORD pDBMsg, DWORD);	
	DWORD InsertIntoAllianceList(DWORD pDBMsg, DWORD);
	DWORD ClearAllianceList(DWORD pDBMsg, DWORD);
	DWORD ConfirmAlliance(DWORD pDBMsg, DWORD);


	// ��Ϸ���������ͼ����������ͳ��
	DWORD WorldMapOnlineInfo(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveOnlineTimeEachMonth(DWORD pDBMsg, DWORD dwDummy);
	DWORD QuestBoardSaveAllQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD QuestBoardSaveOneQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD SyncCanRankFlag(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveMediaOnlineNum(DWORD pDBMsg, DWORD dwDummy);

	// ������������
	DWORD UpdateFabaoMood(DWORD pDBMsg,DWORD dwDummy);
	// ����Ԫ���˺����ͱ仯
	DWORD	UpdateFabaoEleInjuryType(DWORD pDBMsg,DWORD dwDummy);
	// ����������������
	DWORD UpdateFabaoNirVanaData(DWORD pMsg,DWORD);
	// ��ȡ������������
	DWORD GetFabaoNirVanaData(DWORD,DWORD);

	// ��֮����
	DWORD LoadGodBidInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD SaveOneGodBidInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD ClearGodBidInfo(DWORD pDBMsg,DWORD dwDummy);

	DWORD UpdateFabaoBirthTimes(DWORD pDBMsg,DWORD dwDummy);

	// �ҽ�
	DWORD SaveEggInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD ClearEggInfo(DWORD pDBMsg,DWORD dwDummy);

	// �³�
	DWORD SaveWardrobeLayerInfo(DWORD pDBMsg,DWORD dwDummy);

	DWORD DelRoleOneBuff(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertBuffDir(DWORD pDBMsg,DWORD);


	//��������
	DWORD LoadFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	//DWORD LoadFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteALLFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateRoleSpriteData(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateRoleSpriteData(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteRoleSpriteData(DWORD pDBMsg, DWORD dwDummy);

	// ��������
	DWORD RenewFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeFamilyQuestIssueState(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	//DWORD FamilyMemberRewarded(DWORD pDBMsg, DWORD dwDummy);

	// �������
	DWORD CreateFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD DismissFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadAllFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadFamilyMember(DWORD pDBMsg, DWORD dwDummy);
	DWORD FamilyInitOK(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveFamilyAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveFamilyActive(DWORD pDBMsg, DWORD dwDummy);
	DWORD JoinFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD LeaveFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD SetLeader(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveRoleFamilyQuestRewarded(DWORD pDBMsg, DWORD dwDummy);
	DWORD RequestFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFamilyQuest(DWORD pDBMsg, DWORD dwDummy);

	DWORD LoadWorldData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadRolelogData(DWORD pDBMsg, DWORD dwDummy);

	DWORD LoadTigerProfit(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveTigerProfit(DWORD pDBMsg, DWORD dwDummy);

	//Ԫ��
	DWORD InsertHolySoulInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolySoulInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolySoulExpAtt(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolySoulAttInfo(DWORD pDBMsg,DWORD dwDummy);
	//Ԫ��װ��ǿ��
	DWORD LogHolyEquipCon(DWORD pDBMsg, DWORD dwDummy);	
	// ��ɫPKɱ�˼�Log
	DWORD LogKillrole(DWORD pDBMsg, DWORD dwDummy);	

private: // ��Ϸ��������½/�ǳ��ص�����
	DWORD LoginCallBack(LPBYTE pByte, DWORD dwSize);
	DWORD LogoutCallBack(DWORD dwDummy);

private:
	// ��ȡ�ļ�, ��ʼ����Ա
	BOOL InitConfig();	

	// ע����Ҫ���ݿ����������
	VOID RegisterDBCmd();

	// ����log�� -- ��ͬ������������ֱ�δ�������������в������ݵ��������
	VOID CreateCurDataLogTable();

	// ��ʼ����Ϸ����
	DWORD InitWorld(LPCTSTR szWorldName,BOOL bReload=FALSE);

	// �ر���Ϸ����
	VOID TermWorld();

	VOID RegisterTestCmd(TObjRef<Console> pConsole);
	// ��������
	VOID RegisterRankingIns(TObjRef<Console> pConsole);
private: // �����߳�
	// ����
	DWORD ThreadBroadcast(LPVOID);

	// ���а�
	DWORD ThreadRankings(LPVOID);

	// ��ȡ��Ϸ��������Ϣ
	DWORD ThreadReceiveMsg(LPVOID);

	// �����������Ϣ
	DWORD ThreadProcessMsg(LPVOID);

private: // ѹ��ר��
	DWORD TestLoadRole(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestLoadRoleItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestSaveItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestSaveEquip(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestDBReadFunction();
	DWORD RankingNow();
private:
	TSFPTrunk<CLoong>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<StreamServer>	m_pServer;				// ����Ϸ������������
	TObjRef<Thread>			m_pThread;				// �̹߳�����

	volatile BOOL			m_bTerminate;			// �Ƿ����
	volatile BOOL			m_bSuspendFlag;		// ���߳�ȫ���ȴ����ź�
	volatile LONG			m_lThreadNum;
	enum {EThreadRecv=0,EThreadProc,EThreadBroadcast,ETheadAll};
	server_def::Event 		m_ProcThreadSuspend[ETheadAll];	// �ô������ա��㲥�̵߳ȴ�����������db���ӣ����ÿ���
	server_def::Event		m_eventSuspended;
	VOID 					SuspendAllWorkingThread();
	VOID					ResumeAllWorkingThread();
	BOOL					WaitForWorkingThreadSuspend(INT nTimeout);
	VOID					NotifySuspended();

	volatile BOOL			m_bGameServerLogout;				// ��ǰ��Ϸ�����Ƿ�ر�
	volatile BOOL			m_bGameServerConnect;				// ��ǰ�Ƿ�����Ϸ��������

	TCHAR					m_szWorldName[X_SHORT_NAME];		// ��Ϸ������
	//TCHAR					m_szServerName[X_LONG_STRING*2];	// ��Ϸ��������������
	INT						m_nPort;							// ���������˿ں�
	DWORD					m_dwDBGoldenCode;					// ���������Ľ�ɫ����
	DWORD					m_dwGameGoldenCode;					// ��Ϸ�������Ľ�ɫ����

	CLoongDB*				m_pDB;					// ���ݿ��¼������
	CLoongRank*				m_pDBRank;				// ���а����ݿ������
	BYTE					m_byRankingStartTime;	// ���а�����ʱ��
	tagDWORDTime			m_dwLogTime;			// log��ʱ��
	tagDWORDTime			m_dwLastRanking;		// ��¼���а���µ����ʱ��
	CHAR					m_szLogFilePostfix[16];	// log�����ݿ���׺

	DBMsgQueue				m_MsgQue;				// ��������Ϣ����
	NetCmdMgr				m_MsgCmdMgr;			// ��Ϸ�������뱾����������Ϣ����ӳ��	

	CHAR					m_pBuffer[X_ROLE_BUFF_SIZE];	// ���ڸ���Ϸ������������Ϣ

	tagNDBS_LoadSimRole*	m_pLoadSimRole;			// ����ĳ���ʺ��µĻ�����ɫ��Ϣ,��Ҫ�Ը�����ʹ��ZeroMemory
	tagNDBS_LoadRole*		m_pLoadRole;			// ����ĳ����ɫ�ľ������ݣ���Ҫ�Ը�����ʹ��ZeroMemory
	tagNDBS_CreateRole*		m_pCreateRole;			// �����´����Ľ�ɫ�Ļ�����Ϣ
	tagNDBS_LoadBaiBao*		m_pLoadBaiBao;			// ������ȡ�İٱ�����Ʒ��Ϣ
	tagNDBS_LoadBaiBaoLog*	m_pLoadBaiBaoLog;		// ������ȡ�İٱ�����ƷLog��Ϣ
	
	CResGuard				m_Lock;

	tagDWORDTime			m_dwClergyRankTime;		// ���һ�ξ�����ְ��ѡ��ʱ��

private: // UI���
	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindow;
	TObjRef<GUISystem>		m_pGUI;
	IGUIRenderNative		m_Render;
	TObjRef<Console>		m_pConsole;

public: // ����ͳ�����
	struct tagStatus
	{
		// ϵͳ
		INT					nCpuNum;
		DWORD				dwTotalPhys;
		DWORD				dwAvailPhys;
		INT					nQuotaNonPagedPoolUsage;

		// ������״̬
		volatile DWORD		dwHeartbeat;			// ����
		volatile DWORD		dwRecvMsgNum;			// ���յ�����Ϣ����
		volatile DWORD		dwProcMsgNum;			// �������Ϣ����
		volatile DWORD		dwSendMsgNum;			// �ѷ��͵���Ϣ����

		DWORD				dwRecvMsgSz;			// �������ܵ����ܵ���Ϣ��С
		DWORD				dwSendMsgSz;			// ����㷢�͵��ܵ���Ϣ��С

		volatile DWORD		dwUnRecvMsgNum;			// ������ѽ��ܣ����ϲ�δ��ȡ����Ϣ��
		volatile DWORD		dwUnSendMsgNum;			// �ϲ��ѷ��ͣ��������δ��������Ϣ��

		DWORD				dwReadNum;				// ��DB����
		DWORD				dwWriteNum;				// дDB����

		DWORD				dwLastRecvNum;			// ���һ���յ�������Ϣ��
		DWORD				dwLastProcNum;			// ���һ�δ��������Ϣ��
		DWORD				dwLastSendNum;			// ���һ�η��͵�����Ϣ��

		DWORD				dwLastRecvSz;			// ���һ���յ�����Ϣ��С
		DWORD				dwLastSendSz;			// ���һ�η��͵���Ϣ��С
		
		DWORD				dwLastReadNum;			// ���һ�ζ�DB����
		DWORD				dwLastWriteNum;			// ���һ��дDB����

		DWORD				dwTickRecvNum;			// ��ǰˢ�¼�����յ�������Ϣ��
		DWORD				dwTickProcNum;			// ��ǰˢ�¼���ڴ��������Ϣ��
		DWORD				dwTickSendNum;			// ��ǰˢ�¼���ڷ��͵�����Ϣ��

		DWORD				dwTickRecvSz;			// ��ǰˢ�¼�����յ�����Ϣ��С
		DWORD				dwTickSendSz;			// ��ǰˢ�¼���ڷ��͵���Ϣ��С

		DWORD				dwTickReadNum;			// ��ǰˢ�¼���ڶ�DB����
		DWORD				dwTickWriteNum;			// ��ǰˢ�¼����дDB����

		DWORD				dwLastTick;				// ���һ��ˢ��ʱ��
		DWORD				dwInterval;				// ʱ����(����)

		DWORD				dwMaxSendedMsgSize;		// ���͵������Ϣ��С
		DWORD				dwMaxSendedMsgID;			// ���͵������Ϣid

		// �汾���
		DWORD				dwVerLoongDB;
		DWORD				dwVerBeton;
		DWORD				dwVerVEngine;
		DWORD				dwVerServerDefine;
		DWORD				dwVerXML;
		
		tagStatus()
		{
			Init();
		}
		
		VOID Init()
		{
			ZeroMemory(this, sizeof(*this));
			nCpuNum		= TObjRef<SysInfo>()->GetProcessorNum();
			dwLastTick	= GetTickCount();
		}
	};
	// �õ�������״̬��Ϣ
	const tagStatus* GetStatus();

private:
	tagStatus				m_Status;
	// Jason 2010-11-29
	volatile BOOL			m_bRankingNow;			// �Ƿ���������
	volatile LONG			m_lIniTermWorldCounter;	// init��term world�ļ���������ֹ����Գ�ʼ������ֹ
	BOOL		m_bInited;
private:
	DWORD OnNDBC_KeyCodeAddGift(DWORD pDBMsg, DWORD dwDummy);
	DWORD OnNDBC_ReceiveYuanBao(DWORD pDBMsg, DWORD dwDummy);
};


//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: db_session.h
// author: Sxg
// actor:
// data: 2008-5-19
// last:
// brief: ��Ϸ�����������ݿ�������ĶԻ���
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"

#include "..\WorldDefine\base_define.h"
#include "..\ServerDefine\role_data.h"

// Jason [v1.3.1] �ⲿ����
//#include "external_links.h"
#include "external_links_define.h"

class ExternalLinks;


//��ʼ������
enum EDBSessionInitStep
{
	EDBS_ItemInfo = 0,
	EDBS_ItemNeedLog,
	EDBS_LoadAllRoleInfo,
	EDBS_LoadAllYBAccount,
	EDBS_LoadAllOrder,
	EDBS_LoadAllGuild,
	EDBS_LoadAllGuildMember,

	EDBS_LoadGuildUpgradeInfo,	//��ʩ
	EDBS_LoadGuildSkillIInfo,	//����
	EDBS_LoadGuildCommerceInfo,	//����
	EDBS_LoadGuildWareItems,	//�ֿ�

	EDBS_InitCity,
	EDBS_LoadAllGPInfo,
	EDBS_LoadAllVIPStallInfo,
	EDBS_LoadActivityData,
	EDBS_LoadRanking,
	EDBS_LoadGetVNBData,
	EDBS_ExternalLinksList,
	EDBS_LoadAllLimitNumItemInfo,
	EDBS_InitOk,
};


//-----------------------------------------------------------------------------
class DBSession
{
public:
	DBSession();
	~DBSession();

public:
	BOOL Init();
	VOID Destroy();

	VOID Update();
	BOOL IsWell() { return (m_bInitOK && m_lTransCanUse && P_VALID(m_pTran) && m_pTran->IsConnected()); }

	VOID Send(LPVOID pMsg, DWORD dwMsgSize);	

	LPBYTE Recv(DWORD& dwMsgSize) { if(m_lTransCanUse && P_VALID(m_pTran)) return m_pTran->Recv( dwMsgSize ); return NULL; }

	// ����յ���������Ϣ
	VOID FreeRecved(LPVOID pMsg) { if( m_lTransCanUse && P_VALID(m_pTran) ) m_pTran->FreeRecved((LPBYTE)pMsg); }

	INT	GetUnsendPackageNum() { if(m_lTransCanUse && P_VALID(m_pTran)) return m_pTran->GetUnsendPackageNum(); return -1;}
	INT	GetReceivedPackageNum() { if(m_lTransCanUse && P_VALID(m_pTran)) return m_pTran->GetReceivedPackageNum(); return -1; }
	INT	GetToDBConnected() { if(m_lTransCanUse && P_VALID(m_pTran)) return (int)m_pTran->IsConnected();return 0; }

private:
	// ��ȡ�ļ�, ��ʼ����Ա
	BOOL InitConfig();

	// ע�����е���������
	VOID RegisterAllDBCommand();

private:
	// �������ݿ������
	DWORD ThreadConnectDB();

private:
	// ��֤������
	DWORD HandleCertification(tagNetCmd* pMsg, DWORD);
	DWORD HandleHeartBeat(tagNetCmd* pMsg, DWORD);

	// ��ʼ����Ϣ
	DWORD HandleLoongWorldInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadAllRoleInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleItemInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleItemNeedLog(tagNetCmd* pMsg, DWORD);

	// ���ﴴ����ɾ���Ͷ�ȡ
	DWORD HandleRoleEnum(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleCreate(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleDelete(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleResume(tagNetCmd* pMsg, DWORD) { return 0; }
	DWORD HandleRoleLoad(tagNetCmd* pMsg, DWORD);

	// ��ٱ��������µ���Ʒ
	DWORD HandleBaiBaoLoad(tagNetCmd* pMsg, DWORD);
	// ���°ٱ����е�Ԫ��
	DWORD HandleBaiBaoYuanBaoLoad(tagNetCmd* pMsg, DWORD);

	// Ԫ���������
	DWORD HandleLoadAllYBAccount(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadAllYBOrder(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleGetYBOrder(tagNetCmd* pMsg, DWORD);

	// �ٱ�����ʷ��¼���
	DWORD HandleBaiBaoLoadLog(tagNetCmd* pMsg, DWORD);

	// ������
	DWORD HandleRepRankLoad(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetActTreasureList(tagNetCmd* pMsg, DWORD);
	DWORD HandleRepRstTimeStamp(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetFameHallEnterSnap(tagNetCmd* pMsg, DWORD);

	// �������
	DWORD HandleLoadAllEmptyGuild(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuild(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildMember(tagNetCmd* pMsg, DWORD);
	DWORD HandleGuildMemberInitOk(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildWareItems(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildUpgradeInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildSkillInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildCommerceInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadCommerceRankInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleGuildInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleCreateGuild(tagNetCmd* pMsg, DWORD);
public:
	DWORD HandleInsertGuildName(tagNetCmd* pMsg, DWORD);
private:

	// ����
	DWORD HandleCreatePetSoul(tagNetCmd* pMsg, DWORD);
	
	// �̳����
	DWORD HandleLoadAllGPInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadLimitItemInfo(tagNetCmd* pMsg, DWORD);
	

	// ��ɫ����
	DWORD HandleLoadRoleVCard(tagNetCmd* pMsg, DWORD);

	// VIP̯λ
	DWORD HandleLoadVIPStallInfo(tagNetCmd* pMsg, DWORD);

	// �̶��
	DWORD HandleLoadActivityData(tagNetCmd* pMsg, DWORD);

	// VIP����
	DWORD HandleLoadVNBData(tagNetCmd* pMsg, DWORD);

	// ʵ�����а����
	DWORD HandleInitRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleReloadRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitPetRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitFlowerRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleRetUpdateRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleRenewAdvanceStrength(tagNetCmd* pMsg, DWORD);
	DWORD HandleSyncCanRankFlag(tagNetCmd* pMsg, DWORD);

	// ��ְ��ѡ���
	//DWORD HandleAsk4NewClergy(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitClergyRank(tagNetCmd* pMsg, DWORD);
	DWORD HandleRenewClergyCandidateType(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetClergyCandidateCount(tagNetCmd* pMsg, DWORD);
	DWORD HandleClearAllCompetitiveClergy(tagNetCmd* pMsg, DWORD);

	// ��ȡ����
	DWORD HandleLoadLeftMsg(tagNetCmd* pMsg, DWORD);
	
	// ����ϵͳ
	DWORD HandleLoadCityInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetGuildClanValue(tagNetCmd* pMsg, DWORD);

	// ʦͽ
	DWORD HandleLoadMasterApprenticeData(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadForceBreakOutApprenticeList(tagNetCmd* pMsg, DWORD);

	// ѡ��
    DWORD HandleLoadShowPlayerData(tagNetCmd* pMsg, DWORD);
    DWORD HandleLoadVotePlayerData(tagNetCmd* pMsg, DWORD);

	// �����
    DWORD HandleLoadActiveRankData(tagNetCmd* pMsg, DWORD);
	// ע��ú�������Ҫ�޸�g_roleMgr�е����ݵģ����ø�Э�鴦��������threadupdate�����������ڵ�ͼ�̣߳���˿��԰�ȫ����
	DWORD HandleLoadOneRoleInfo(tagNetCmd* pMsg, DWORD);

	// ϡ����Ʒ
	DWORD HandleLoadGlobalLimitItem(tagNetCmd* pMsg, DWORD);

	//DWORD HandleLoadRareArea(tagNetCmd* pMsg, DWORD);
	// ��������
	DWORD HandleLoadFamilySprite(tagNetCmd* pMsg, DWORD);
	// ��������
	DWORD HandleLoadFamilyQuest(tagNetCmd* pMsg, DWORD);
	
	DWORD HandleLoadAllFamily(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadFamilyMember(tagNetCmd* pMsg, DWORD);
	DWORD HandleFamilyMemberInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleFamilyInitOK(tagNetCmd* pMsg, DWORD);

	DWORD HandleLoadWorldData(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadRoleLogData(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadRoleTigerProfit(tagNetCmd* pMsg, DWORD);

	DWORD HandleAccountReactiveOrNot(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitAccountReactiveInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandelKeyCodeAddGift(tagNetCmd* pMsg, DWORD);
	DWORD HandleNDBS_ReceiveYuanBao(tagNetCmd* pMsg, DWORD);
private:
	TSFPTrunk<DBSession>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<StreamTransport>	m_pTran;
	TObjRef<Thread>				m_pThread;
	TObjRef<NetCmdMgr>			m_pMsgCmdMgr;

	// ���Ӳ���
	CHAR						m_szDBIP[X_IP_LEN];		// ���ݿ������ip
	DWORD						m_dwDBPort;				// ���ݿ������port

	// �������Ӻ��跢�͵�����
	DWORD						m_dwGoldenCode;					// ���ݿ��������ɫ����
	//TCHAR						m_szServerName[X_LONG_STRING];	// ������Ϸ����Ļ�������

	volatile BOOL				m_bTermConnect;					// ��¼��ǰ����״̬
	volatile BOOL				m_bInitOK;						// �Ƿ��ʼ�����

	DWORD						m_dwInitTime;					//��db��ʼ��������������ʱ�� ����

	// Jason v[1.3.1] �ⲿ����
	TSafeList<tagSExternalLink>	m_lstExterLinks;
	ExternalLinks		*	m_elExternalLinks;
	tagDWORDTime			m_timLastPullExtLink;

	VOID RefreshExternalLinksList(tagNetCmd * pCmd);
	DWORD HandleExternalLinksList(tagNetCmd* pMsg, DWORD);
 
	VOID UpdateExternalLinksList(VOID);

	VOID InitialExternalLinksList(VOID);

	//��ӡ��Ϣ
	VOID OutPutInitStepInfo(EDBSessionInitStep stp, BOOL bSuccess, DWORD dwPara1 = GT_INVALID, DWORD dwPara2 = GT_INVALID);

	// Jason 2010-1-27 v1.3.2 ��һع�/���߽������
	DWORD HandleGetAccountInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadShortCut_QuickBar(tagNetCmd* pMsg, DWORD);
	DWORD HandleIsSavedShortCut_QuickBar(tagNetCmd* pMsg, DWORD);

	DWORD HandleGetSerialVal(tagNetCmd* pMsg, DWORD);
	
	BOOL HandleCheckName(tagNetCmd* pMsg, DWORD);

	// ������ߣ�֪ͨ���
	BOOL HandleGetRoleListOfEnemy(tagNetCmd* pMsg, DWORD);
	
	// �������ݿ��еĹ�����ս��ϵ
	BOOL HandleLoadGuildWarDeclare(tagNetCmd* pMsg, DWORD);

	// ����ɫ�����������
	VOID AddInitialSkill2Role(DWORD dwRoleID);

	// ս��
	BOOL HandleLoadBattleFieldAttackSignUpInfo(tagNetCmd* pMsg, DWORD);
	BOOL HandleLoadAllicanceGuildInfo(tagNetCmd* pMsg, DWORD);

	// Jason
	DWORD	m_dwWaitConnectionTick;
	DWORD	m_dwTransDBReconnectTicks;
	BOOL ReconnectDBServer(VOID);

	// �õ�������������
	DWORD HandleGetFabaoNirVanaData(tagNetCmd* pMsg, DWORD);

	// ��֮����
	DWORD HandleLoadGodBidInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleClearGodBidInfo(tagNetCmd* pMsg, DWORD);
	
	volatile LONG m_lTransCanUse;

	INT32		m_nItemNLItemNum;
	DWORD		m_dwItemNLdwTypeID[MAX_ITEM_NEEDLOG_NUM];
public:
	VOID SendMsgToPlayerForExtLinks(Role * ply);

	VOID ReInitItemNeedLog(VOID);

	//ɾ����ɫ����
	VOID			SendDeleteArea2DB( DWORD dwRoleID,DWORD dwAreaID );

	//���ӽ�ɫ����
	VOID			SendCreateArea2DB( DWORD dwRoleID, DWORD dwAreaID, BOOL bActivation );

	//ɾ����ɫ����
	VOID			SendRemoveSkill( DWORD dwRoleID, DWORD dwSkillID );

	//ɾ����ɫһ��buff
	VOID			SendRemoveOneBuff( DWORD dwRoleID, DWORD dwBuffID );

	//�������߽�ɫȡ����������
	VOID			RemoveAreaAbilityOffline( DWORD dwRoleID, DWORD dwAreaID );
};

extern DBSession g_dbSession;		// DB sessionȫ�ֶ���

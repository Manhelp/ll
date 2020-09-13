//-----------------------------------------------------------------------------
//!\file player_session.h
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief ������Ӵ�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "player_net_cmd_mgr.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../WorldDefine/msg_quickbar.h"
#include "Mutex.h"
#include "gm_net_cmd_mgr.h"
#include "fatigue_guarder.h"
#include "xtrap_guarder.h"
#include "db_session.h"
#include "../ServerDefine/role_data.h"

#ifdef ON_GAMEGUARD
#include "../GameGuard/ggsrv30.h"
#endif

class Creature;
class MotionScript;
struct tagQuickData;
enum EQuestBoardType;

const INT CON_LOST	=	-1000000;		// ���ӶϿ���־
const INT RET_TRANS	=	-2000000;		// ���ϲ㴦��

// Jason 2010-1-15 v1.3.2 ���߽���
class OfflineReward;
class IPGDragonfly;

class PlayerSession
{
public:
	friend class DBSession;
	friend class WorldNetCmdMgr;

	PlayerSession(DWORD dwSessionID, DWORD dwInternalIndex, DWORD dwIP, BYTE byPrivilege,
				BOOL bGuard, DWORD dwAccOLSec, LPCSTR tszAccount,char *szKrUserID,tagDWORDTime dwPreLoginTime,
				DWORD dwPreLoginIP,DWORD dwChannelID, CHAR* szMac);
	~PlayerSession();

	//-----------------------------------------------------------------------------
	// ����Get
	//-----------------------------------------------------------------------------

	tagDWORDTime GetPreLoginTime()	const	{ return m_dwPreLoginTime;}
	DWORD		 GetPreLoginIP()	const	{ return m_dwPreLoginIP;}
	DWORD		 GetCurLoginIP()	const	{ return m_dwIP;}

	LPCSTR		GetAccount()		const	{ return m_szAccount;	}
	LPCSTR		GetKrUserID()       const   { return m_szKrUserID;}
	DWORD		GetGetIP()			const	{ return m_dwIP; }
	DWORD		GetSessionID()		const	{ return m_dwAccountID; }
	INT			GetMsgWaitNum()		const	{ return m_nMsgNum; }
	DWORD		GetInternalIndex()	const	{ return m_dwInternalIndex; }
	Role*		GetRole()			const	{ return m_pRole; }
	Role*		GetOtherInMap( DWORD dwRoleID ) const ;
	const FatigueGuarder&	GetFatigueGuarder()	const { return m_FatigueGarder; }
	INT			GetVNBExpRate()		const;
	INT			GetVNBLootRate()		const;
	LPCTSTR		GetVNBName()		const;
	BOOL		IsKicked() const			{ return m_bKicked; }
	INT			GetDistributionID()		const	{ return m_nDistributionID; }

	// �ʺ��½�ɫͨ��������ز���
	const INT	GetBaiBaoYB()		const	{ return m_sAccountCommon.nBaiBaoYuanBao; }
	const INT64	GetWareSilver()		const	{ return m_sAccountCommon.n64WareSilver; }
	const INT16	GetWareSize()		const	{ return m_sAccountCommon.n16WareSize; }
	const tagDWPlayerPersonalSet	GetPersonalSet()		const	{ return m_sAccountCommon.stPersonalSet; }
	const DWORD GetLastUseRoleID()	const	{ return m_sAccountCommon.dwLastUseRoleID; }
	DWORD GetAccountOnlineTime() 
	{ 
		tagDWORDTime now = GetCurrentDWORDTime();
		return CalcTimeDiff(now, m_dwLoginTime); 
	}

	EAccountSpec GetAccountSpec() { return m_sAccountCommon.eSpec; }

	DWORD		GetLastRecvTime(DWORD dwID)	{ return m_mapLastRecvTime.Peek(dwID); }

	VOID GetMac(CHAR* szMac) { if(P_VALID(szMac)) memcpy(szMac, m_szMac, X_SHORT_STRING); };
	//----------------------------------------------------------------------------
	// ����Set
	//----------------------------------------------------------------------------
	VOID		SetRole(Role* pRole)		{ m_pRole = pRole; }
	VOID		SetConnectionLost()			{ InterlockedExchange((LPLONG)(&m_bConnectionLost), TRUE); }
	VOID		SetKicked()					{ InterlockedExchange((LPLONG)(&m_bKicked), TRUE); }

	VOID		SetBaiBaoYB(INT nYuanBao)		{ m_sAccountCommon.nBaiBaoYuanBao	= nYuanBao; }
	VOID		SetWareSilver(INT64 n64Silver)	{ m_sAccountCommon.n64WareSilver	= n64Silver; }
	VOID		SetAccOLMin(DWORD dwState, DWORD dwAccOLMin)	{ m_FatigueGarder.SetAccOLTimeMin(dwState, dwAccOLMin);				}
	VOID		SetRoleLoading(BOOL bLoading=TRUE)				{ m_bRoleLoading = TRUE == bLoading;	}
	VOID		SessionLogout();
	VOID		SetAccountSpec(EAccountSpec espec) 
	{ 
		m_sAccountCommon.eSpec = espec;
		tagNDBC_SaveAccountSpec send;
		send.dwAccountID = m_dwAccountID;
		send.eSpec = espec;
		g_dbSession.Send(&send, send.dwSize);
	}
	VOID		SetLastRecvTime(DWORD dwID)	{ m_mapLastRecvTime.SetValue(dwID, timeGetTime()); }
	//----------------------------------------------------------------------------
	// ��Ϣ�������
	//----------------------------------------------------------------------------
	INT			HandleMessage();
	VOID		SendMessage(LPVOID pMsg, DWORD dwSize);
	VOID		BroadcastCurrChannel(LPCTSTR szMsg);

	//-----------------------------------------------------------------------------
	// ѡ�˽�����ж�
	//-----------------------------------------------------------------------------
	bool		IsRoleLoading()		const	{ return m_bRoleLoading; }
	bool		IsRoleEnuming()		const	{ return m_bRoleEnuming; }
	bool		IsRoleCreating()	const	{ return m_bRoleCreating; }
	bool		IsRoleDeleting()	const	{ return m_bRoleDeleting; }
	bool		IsInWorld()			const	{ return m_bRoleInWorld; }


	//-----------------------------------------------------------------------------
	// ��ɫͨ���������
	//-----------------------------------------------------------------------------
	bool		IsHaveBagPsd()		const	{ return GetBagPsd() != GT_INVALID; }

	//-----------------------------------------------------------------------------
	// ��ɫ���
	//-----------------------------------------------------------------------------
	BOOL		FullLogin(Role* pRole, BOOL bFirst);
	VOID		LogoutPlayer();
	VOID		Refresh();

	//-----------------------------------------------------------------------------
	// ����������غ�GM����
	//----------------------------------------------------------------------------
	static VOID RegisterAllPlayerCmd();
	static VOID RegisterALLSendCmd();
	static VOID UnRegisterALL();

	static VOID LogNetAnalyze();

	//-----------------------------------------------------------------------------
	// GM�������
	//-----------------------------------------------------------------------------
	BOOL		IsPrivilegeEnough(BYTE byPrivilege) const { return byPrivilege <= m_byPrivilege; }
	BYTE GetPriviliege()	{ return m_byPrivilege; }

	//-----------------------------------------------------------------------
	// ������ϢMsg(�����������ã��������ͷ�)
	//-----------------------------------------------------------------------
	VOID		RecycleMsg(LPBYTE pMsg);

	//-----------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------
	INT			Update();

	VOID			SyncOnlineNum(Role* pRole, BOOL bLogout = FALSE);

	DWORD	       HandleRoleCreateFromLogin(tagNetCmd* pCmd);

private:
	//-----------------------------------------------------------------------
	// ��Ҫ�����е�ͼ�߳��ϲ㴦�����Ϣע��
	//-----------------------------------------------------------------------
	static VOID	RegisterWorldMsg(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize, DWORD dwTimeInterval = 0);

	//-----------------------------------------------------------------------
	// ��Ϣ�������
	//-----------------------------------------------------------------------
	VOID			SendSmallMessage(LPVOID pMsg, DWORD dwSize);
	VOID			SendLargeMessage(LPVOID pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// �ײ�����
	//-----------------------------------------------------------------------
	LPBYTE			RecvMsg(DWORD& dwSize);
	VOID			ReturnMsg(LPBYTE pMsg);
	VOID			SendMsg(LPBYTE pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// ѡ�˽������
	//-----------------------------------------------------------------------
	BOOL			IsRoleExist(const DWORD dwRoleID) const;
	BOOL			AddRole(const DWORD dwRoleID);
	BOOL			RemoveRole(const DWORD dwRoleID);
	BOOL			CanSetSafeCode();
	BOOL			CanResetSafeCode() const;
	BOOL			CanCancelSafeCodeReset() const;

	//-----------------------------------------------------------------------
	// �ʺ��½�ɫͨ��������ز���
	//-----------------------------------------------------------------------
	const DWORD		GetBagPsd()		const { return m_sAccountCommon.dwBagPsdCrc; }
	const DWORD		GetSafeCode()	const { return m_sAccountCommon.sSafeCode.dwSafeCodeCrc; }

	VOID			SetBagPsd(DWORD dwNewPsdCrc);

	// �����а��ɳ�Ա�������������仯��Ϣ
	VOID			SyncOnlineNum2AllGuildMumber(DWORD dwGuildID, BOOL bLogout = FALSE);

	EQuestBoardType GetQuestBoardType(DWORD dwNPCID);
	EQuestBoardType GetQuestBoardTypeByTypeID(DWORD dwNPCTypeID);

	/************************************************************************
	** Handlers -- map thread
	*************************************************************************/

	//-----------------------------------------------------------------------
	// ������Ϸ
	//-----------------------------------------------------------------------
	DWORD	HandleJoinGame(tagNetCmd* pCmd);

	//-----------------------------------------------------------------------
	// ��ɫ������ɾ������ȡ��ѡ��
	//-----------------------------------------------------------------------
	DWORD	HandleRoleCreate(tagNetCmd* pCmd);
	DWORD	HandleRoleEnum(tagNetCmd* pCmd);
	DWORD	HandleRoleDelete(tagNetCmd* pCmd);
	DWORD	HandleRoleSelect(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ȫ�봦��
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetSafeCode(tagNetCmd* pCmd);
	DWORD	HandleRoleResetSafeCode(tagNetCmd* pCmd);
	DWORD	HandleRoleCancelSafeCodeReset(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������Ի�ȡ
	//------------------------------------------------------------------------
	DWORD	HandleGetRoleInitAtt(tagNetCmd* pCmd);
	DWORD	HandleGetRemoteUnitAtt(tagNetCmd* pCmd);
	DWORD	HandleChangeRoleName(tagNetCmd* pCmd);
	//DWORD	HandleGetSelfStrength(tagNetCmd* pCmd);
	DWORD	HandleSetPersonalSet(tagNetCmd* pCmd);
	DWORD	HandleCheckRemoteRoleInfoOpenType(tagNetCmd* pCmd);
	DWORD	HandleGetStrengthDetail(tagNetCmd* pCmd);
	DWORD	HandleGetConsumptiveStrength(tagNetCmd* pCmd);
	DWORD	HandleGetPvPStatics(tagNetCmd* pCmd);
	DWORD	HandleRefreshBaoXiang(tagNetCmd* pCmd);
	DWORD	HandleGetWarBaoXiang(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleWalk(tagNetCmd* pCmd);
	DWORD	HandleRoleStopWalk(tagNetCmd* pCmd);
	DWORD	HandleRoleJump(tagNetCmd* pCmd);
	DWORD	HandleRoleDrop(tagNetCmd* pCmd);
	DWORD	HandleRoleVDrop(tagNetCmd* pCmd);
	DWORD	HandleRoleSlide(tagNetCmd* pCmd);
	DWORD	HandleRoleCanGoToPos(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// װ�����
	//------------------------------------------------------------------------
	DWORD	HandleRoleEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleUnequip(tagNetCmd* pCmd);
	DWORD	HandleRoleSwapWeapon(tagNetCmd* pCmd);
	DWORD	HandleRoleIdentifyEquip(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ɫ�����ʾ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetFashion(tagNetCmd* pCmd);
	DWORD	HandleRoleSetDisplay(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��Ʒ�������
	//------------------------------------------------------------------------
	DWORD	HandleRoleChangeItemPos(tagNetCmd* pCmd);
	DWORD	HandleRoleChangeItemPosEx(tagNetCmd* pCmd);
	DWORD	HandleRoleReorderItem(tagNetCmd* pCmd);
	DWORD	HandleRoleReorderItemEx(tagNetCmd* pCmd);
	DWORD	HandleRoleSetTransportMap(tagNetCmd* pCmd); //ʹ�ü�¼ʽ���ͷ�
	DWORD	HandleRoleUseTransportMap(tagNetCmd* pCmd); //ʹ�ü�¼ʽ���ͷ�
	//------------------------------------------------------------------------
	// ��Ҽ佻�����
	//------------------------------------------------------------------------
	DWORD	HandleRoleExchangeReq(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeReqRes(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeAdd(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeDec(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeMoney(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeLock(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeCancel(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeVerify(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �̵����
	//------------------------------------------------------------------------
	DWORD	HandleRoleGetShopItems(tagNetCmd* pCmd);
	DWORD	HandleRoleGetShopEquips(tagNetCmd* pCmd);
	DWORD	HandleRoleBuyShopItem(tagNetCmd* pCmd);
	DWORD	HandleRoleBuyShopEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleSellToShop(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��̯���
	//------------------------------------------------------------------------
	DWORD	HandleRoleStallStart(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetGoods(tagNetCmd* pCmd);
	DWORD	HandleRoleStallUnsetGoods(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetTitle(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetAdText(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetAdFlag(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetFinish(tagNetCmd* pCmd);
	DWORD	HandleRoleStallClose(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGet(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGetTitle(tagNetCmd* pCmd);
	DWORD	HandleRoleStallBuy(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGetSpec(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��վ&Ǭ��ʯ
	//------------------------------------------------------------------------
	DWORD	HandleRoleDak(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʹ��ĥʯ
	//------------------------------------------------------------------------
	DWORD	HandleRoleAbrase(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ɫ�ֿ�
	//------------------------------------------------------------------------
	DWORD	HandleRoleSideWareOpen(tagNetCmd* pCmd);
	DWORD	HandleRoleWareExtend(tagNetCmd* pCmd);
	DWORD	HandleRoleBagExtand(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ɫ�ֿ��д�ȡ��Ǯ&Ԫ��
	//------------------------------------------------------------------------
	DWORD	HandleRoleSaveSilver(tagNetCmd* pCmd);
	DWORD	HandleRoleGetSilver(tagNetCmd* pCmd);
	//DWORD	HandleRoleSaveYuanBao(tagNetCmd* pCmd);
	DWORD	HandleRoleGetYuanBao(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���Ҽ������
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleUnsetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleCheckBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleResetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleOpenBagPsd(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleEnterWorld(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD   HandleRoleChat(tagNetCmd* pCmd);
	DWORD   HandleRoleGetID(tagNetCmd* pCmd);
	DWORD	HandleRoleGetNameByNameID(tagNetCmd* pCmd);
	DWORD	HandleRoleGetSomeName(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// Ⱥ��
	//------------------------------------------------------------------------
	DWORD	HandleCreateGroupChat(tagNetCmd* pCmd);
	DWORD	HandleDeleteGroupChat(tagNetCmd* pCmd);

	DWORD	HandleLoginOutGroupChat(tagNetCmd* pCmd);
	DWORD	HandleAddRoleToGroupChat(tagNetCmd* pCmd);
	DWORD	HandleSendGroupChatMsg(tagNetCmd* pCmd);
	DWORD	HandleInviteRoleToGroupChat(tagNetCmd* pCmd);
	DWORD	HandleKickRoleFromGroupChat(tagNetCmd* pCmd);


	//------------------------------------------------------------------------
	// װ��չʾ
	//------------------------------------------------------------------------
	DWORD   HandleRoleShowEquip(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	// ��Ʒչʾ
	//------------------------------------------------------------------------
	DWORD   HandleRoleShowItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ȡ����
	//------------------------------------------------------------------------
	DWORD   HandleRoleLoadLeftMsg(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ͼ�¼�
	//------------------------------------------------------------------------
	DWORD	HandleRoleMapTrigger(tagNetCmd* pCmd);
	DWORD	HandleRoleInstanceNotify(tagNetCmd* pCmd);
	DWORD	HandleRoleInstanceAgree(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveInstance(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// ���Ե����
	//-------------------------------------------------------------------------
	DWORD	HandleRoleBidAttPoint(tagNetCmd* pCmd);
	DWORD	HandleRoleClearAttPoint(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// ���ʼ������
	//------------------------------------------------------------------------
	DWORD	HandleRoleLearnSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleLevelUpSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleForgetSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleClearTalent(tagNetCmd* pCmd);
    DWORD   HandlePracticeTalent(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ս��ϵͳ
	//------------------------------------------------------------------------
	DWORD	HandleRoleEnterCombat(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveCombat(tagNetCmd* pCmd);
	DWORD	HandleRoleSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleInterruptSkill(tagNetCmd* pCmd);

	DWORD	HandleRoleCancelBuff(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// PKϵͳ
	//------------------------------------------------------------------------
	DWORD	HandleRoleSafeGuard(tagNetCmd* pCmd);
	DWORD	HandleRoleSetPK(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------
	DWORD	HandleRoleBindRebornMap(tagNetCmd* pCmd);
	DWORD	HandleRoleRevive(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ͨ�ú��� -- ���ڵ�ͼ�߳��ϲ㴦�����Ϣ
	//------------------------------------------------------------------------
	DWORD   HandleRoleMsg2World(tagNetCmd* pCmd) { return RET_TRANS; }

	//------------------------------------------------------------------------
	// ͨ������ -- GM
	//------------------------------------------------------------------------
	DWORD   HandleGMCommand(tagNetCmd* pCmd);
	
	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD	HandleRoleNPCAcceptQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleTriggerAcceptQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleCompleteQuest(tagNetCmd* pCmd);
	DWORD   HandleRoleDeleteQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleUpdateQuestNPCTalk(tagNetCmd* pCmd);
	DWORD	HandleGetSingleQuestDoneInfo(tagNetCmd* pCmd);
	
	        
            
	//------------------------------------------------------------------------
	// ��Ʒǿ��
	//------------------------------------------------------------------------
	DWORD	HandleRolePosyEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleEngraveEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleInlayEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleBrandEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleLoongSoul(tagNetCmd* pCmd);
	DWORD	HandleRoleQuench(tagNetCmd* pCmd);
	DWORD	HandleRoleChisel(tagNetCmd* pCmd);
	DWORD	HandleRoleDyeFashion(tagNetCmd* pCmd);
	DWORD	HandleEquipGrow(tagNetCmd* pCmd);
	DWORD   HandleEquipPerfectGrow(tagNetCmd* pCmd);
	DWORD	HandleXiPurpleStar(tagNetCmd* pCmd);
	DWORD	HandleEquipGrowTrans(tagNetCmd* pCmd);
	DWORD	HandleEquipGrowDec(tagNetCmd* pCmd);
	DWORD	HandleSoulCrystalGrow(tagNetCmd* pCmd);
	DWORD	HandleSoulCrystalRenew(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyStrength(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyEquipmentNumber(tagNetCmd* pCmd);	

	//------------------------------------------------------------------------
	// Ԫ��
	//------------------------------------------------------------------------
	DWORD	HandleRoleSoulEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleUnSoulEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyAffuse(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyReborn(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	//Ԫ��װ������
	//------------------------------------------------------------------------
	//DWORD	HandleStarEnhance(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	//Ԫ��װ�����
	//------------------------------------------------------------------------
	//DWORD HandleHolyEquipHole(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	//Ԫ��װ����Ƕ
	//------------------------------------------------------------------------
	//DWORD HandleHolyEquipInlay(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	//Ԫ��װ��ժ��
	//------------------------------------------------------------------------
	//DWORD HandleHolyEquipRemove(tagNetCmd* pCmd);

	//����ʱװ
	DWORD	HandleRoleSpinFashionDress(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	// �����ϳ���Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRoleProduceItem(tagNetCmd* pCmd);
	DWORD	HandleRoleAdvancedCompose(tagNetCmd* pCmd);

	// Jason 2009-12-6
	DWORD	HandleRoleAddEquipPotVal(tagNetCmd* pCmd);
	// Jason 2009-12-7
	DWORD	HandleLessingLoong(tagNetCmd * pCmd);

	// Jason 2009-12-8 ���ʯ
	DWORD HandleGoldStone(tagNetCmd * pCmd);

	// Jason v1.3.1 2009-12-21
	DWORD HandleGemRemoval(tagNetCmd * pCmd);
	DWORD HandleGetGemRemovalInfo(tagNetCmd * pCmd);
	// Jason v1.3.2 ��һع飬������npc����
	DWORD HandlePlayerRegression(tagNetCmd * pCmd);
	DWORD HandleGetOfflineExp(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// �㻯, װ���ֽ�
	//------------------------------------------------------------------------
	DWORD	HandleRoleDeCompose(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��ȡ
	//------------------------------------------------------------------------
	DWORD	HandleRoleExtract(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʹ����Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRoleUseItem(tagNetCmd* pCmd);
	DWORD	HandleRoleInterruptUseItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʰȡ��Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRolePickUpItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �ӵ���Ʒ
	//------------------------------------------------------------------------
	DWORD	HandleRolePutDownItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD   HandleRoleMakeFriend(tagNetCmd* pCmd);
	DWORD	HandleRoleCancelFriend(tagNetCmd* pCmd);
	DWORD	HandleUpdateFriendGroup(tagNetCmd* pCmd);
	DWORD	HandleMoveBlackList(tagNetCmd* pCmd);
	DWORD	HandleDeleteBlackList(tagNetCmd* pCmd);
	DWORD	HandleRoleSendGift(tagNetCmd* pCmd);
	DWORD	HandleRoleSendGiftReply(tagNetCmd* pCmd);
	DWORD   HandleUpdateFriOnline(tagNetCmd* pCmd);
	//Jason 2009-11-25 ����������Э��
	DWORD	HandleApplyJoinTeam(tagNetCmd *pCmd);
	DWORD	HandleReplyForApplingToJoinTeam(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// С�����
	//------------------------------------------------------------------------
	DWORD   HandleRoleJoinTeam(tagNetCmd* pCmd);
	DWORD	HandleRoleJoinTeamReply(tagNetCmd* pCmd);
	DWORD	HandleRoleKickMember(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveTeam(tagNetCmd* pCmd);
	DWORD	HandleRoleSetPickMol(tagNetCmd* pCmd);
	DWORD	HandleRoleChangeLeader(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �ƺ����
	//------------------------------------------------------------------------
	DWORD HandleRoleActiveTitle(tagNetCmd* pCmd);
	DWORD HandleRoleGetTitles(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleRoleGetVCard(tagNetCmd* pCmd);
	DWORD HandleRoleSetVCard(tagNetCmd* pCmd);
	DWORD HandleGetHeadPicUrl(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���������
	//------------------------------------------------------------------------
	DWORD HandleGetFatigueInfo(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���������
	//------------------------------------------------------------------------
	DWORD HandleGetFameHallRoles(tagNetCmd* pCmd);
	DWORD HandleGetReputeTop(tagNetCmd* pCmd);
	DWORD HandleGetActClanTreasure(tagNetCmd* pCmd);
	DWORD HandleActiveTreasure(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleGetRoleClanData(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��������
	//------------------------------------------------------------------------
	DWORD HandleGameGuarderMsg(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleGetPetAttr(tagNetCmd* pCmd);
	DWORD HandlePetSkill(tagNetCmd* pCmd);
	DWORD HandlePetSkillStopWorking(tagNetCmd* pCmd);
	DWORD HandleUsePetEgg(tagNetCmd* pCmd);
	DWORD HandleDeletePet(tagNetCmd* pCmd);
	DWORD HandleNC_RecallPet(tagNetCmd* pCmd);
	DWORD HandleGetPetDispInfo(tagNetCmd* pCmd);
	DWORD HandleSetPetState(tagNetCmd* pCmd);

	DWORD HandlePetEquip(tagNetCmd* pCmd);
	DWORD HandlePetUnEquip(tagNetCmd* pCmd);
	DWORD HandlePetSwapEquipPos(tagNetCmd* pCmd);
	DWORD HandleGetPetPourExpMoneyNeed(tagNetCmd* pCmd);
	DWORD HandlePetPourExp(tagNetCmd* pCmd);
	DWORD HandlePetUpStep(tagNetCmd* pCmd);
	DWORD HandlePetEnhance(tagNetCmd* pCmd);
	DWORD HandlePetLearnSkill(tagNetCmd* pCmd);
	DWORD HandlePetInvite(tagNetCmd* pCmd);
	DWORD HandlePetOnInvite(tagNetCmd* pCmd);
	DWORD HandlePetFood(tagNetCmd* pCmd);
	DWORD HandlePetSetLock(tagNetCmd* pCmd);
	DWORD HandlePetRename(tagNetCmd* pCmd);	

	DWORD	HandleRolePetExchangeReq(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeReqRes(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeAdd(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeDec(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeMoney(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeLock(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeCancel(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeVerify(tagNetCmd* pCmd);

	// Jason 2010-1-30 v1.3.2 ���������
	DWORD	HandlePetPocketExtendRequest(tagNetCmd* pCmd);
	// ��������ݱ������ݿ�
	DWORD	HandleQuickBarDataSaveToServer(tagNetCmd* pCmd);
	DWORD	HandleQuickBarDataLoadFromServer(tagNetCmd* pCmd);
	DWORD	HandleInformSaveQuickBarData(tagNetCmd* pCmd);
	DWORD	HandleIsSavedQuickBarData(tagNetCmd* pCmd);
	                 
	        
	DWORD	HandlePetGetPocketMaxCount(tagNetCmd * pCmd);


	//------------------------------------------------------------------------
	// �ٱ������
	//------------------------------------------------------------------------
	DWORD HandleInitBaiBaoRecord(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// VIP̯λ���
	//------------------------------------------------------------------------
	DWORD HandleGetAllVIPStallInfo(tagNetCmd* pCmd);
	DWORD HandleUpdateVIPStallInfo(tagNetCmd* pCmd);
	DWORD HandleApplyVIPStall(tagNetCmd* pCmd);
	DWORD HandleSpecVIPStallGet(tagNetCmd* pCmd);
	DWORD HandleBuyVIPStallGoods(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// NPC���
	//------------------------------------------------------------------------
	DWORD HandleTalkToNPC(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �����������
	//------------------------------------------------------------------------
	DWORD HandleGetVipNetBarName(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �ɽ�����
	//------------------------------------------------------------------------
	DWORD HandlePracticeMiracleSkill(tagNetCmd* pCmd);
	DWORD HandleUseAddMiraclePrcNumItem(tagNetCmd* pCmd);	

	//------------------------------------------------------------------------
	// ����ϵͳ
	//------------------------------------------------------------------------
	//���
	DWORD HandleGetMarriageReq(tagNetCmd* pCmd);
	DWORD HandleGetMarriageReqRes(tagNetCmd* pCmd);
	//���
	DWORD HandleBreakMarriageReq(tagNetCmd* pCmd);
	DWORD HandleForceBreakMarriageReq(tagNetCmd* pCmd);
	DWORD HandleBreakMarriageReqRes(tagNetCmd* pCmd);
	//����
	DWORD HandleCelebrateWeddingReq(tagNetCmd* pCmd);
	DWORD HandleCelebrateWeddingReqRes(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���Զ���
	//------------------------------------------------------------------------
	DWORD	HandleRoleStyleAction(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionInvite(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionOnInvite(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionStart(tagNetCmd* pCmd);


	/************************************************************************
	** Handlers -- upper all map thread
	*************************************************************************/

	//------------------------------------------------------------------------
	// �̳����
	//------------------------------------------------------------------------
	DWORD HandleRoleMallGet(tagNetCmd* pCmd);
	DWORD HandleRoleMallUpdate(tagNetCmd* pCmd);
	DWORD HandleRoleMallBuyItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallShoppingCart(tagNetCmd* pCmd);
	DWORD HandleRoleMallBuyPack(tagNetCmd* pCmd);
	DWORD HandleRoleMallPresentItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallPresentPack(tagNetCmd* pCmd);
	DWORD HandleRoleMallFreeGetItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallLaunchGroupPurchase(tagNetCmd* pCmd);
	DWORD HandleRoleMallRespondGroupPurchase(tagNetCmd* pCmd);
	DWORD HandleRoleMallGetGroupPurchaseInfo(tagNetCmd* pCmd);
	DWORD HandleRoleMallGetParticipators(tagNetCmd* pCmd);
	DWORD HandleRoleMallItemExchange(tagNetCmd* pCmd);
	DWORD HandleRoleMallPackExchange(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// Ԫ���������
	//------------------------------------------------------------------------
	DWORD HandleRoleSaveYB2Account(tagNetCmd* pCmd);
	DWORD HandleRoleSaveSilver2Account(tagNetCmd* pCmd);
	DWORD HandleRoleDepositYBAccount(tagNetCmd* pCmd);
	DWORD HandleRoleDepositSilver(tagNetCmd* pCmd);
	DWORD HandleRoleGetYBTradeInfo(tagNetCmd* pCmd);
	DWORD HandleRoleSubmitSellOrder(tagNetCmd* pCmd);
	DWORD HandleRoleSubmitBuyOrder(tagNetCmd* pCmd);
	DWORD HandleRoleDeleteOrder(tagNetCmd* pCmd);
	DWORD HandleRoleGetYBOrder(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleCreateGuild(tagNetCmd* pCmd);
	DWORD HandleDismissGuild(tagNetCmd* pCmd);
	DWORD HandleJoinGuildReq(tagNetCmd* pCmd);
	DWORD HandleJoinGuildReqRes(tagNetCmd* pCmd);
	DWORD HandleLeaveGuild(tagNetCmd* pCmd);
	DWORD HandleKickFromGuild(tagNetCmd* pCmd);
	DWORD HandleTurnoverGuild(tagNetCmd* pCmd);
	DWORD HandleDemissFromGuild(tagNetCmd* pCmd);
	DWORD HandleAppointForGuild(tagNetCmd* pCmd);
	DWORD HandleChangeGuildTenet(tagNetCmd* pCmd);
	DWORD HandleGuildChangeName(tagNetCmd* pCmd);
	

	DWORD HandleSyncGuildInfo(tagNetCmd* pCmd);

	DWORD HandleGetGuildMembers(tagNetCmd* pCmd);
	DWORD HandleGetGuildMemberEx(tagNetCmd* pCmd);
	DWORD HandleRefreshGuildMember(tagNetCmd* pCmd);
	DWORD HandleGetGuildName(tagNetCmd* pCmd);
	DWORD HandleGetGuildTenet(tagNetCmd* pCmd);
	DWORD HandleGetSomeGuildName(tagNetCmd* pCmd);

	DWORD HandleGetGuildWareItems(tagNetCmd* pCmd);
	DWORD HandleGetGuildWarePriList(tagNetCmd* pCmd);
	DWORD HandleGuildWarePrivilege(tagNetCmd* pCmd);

	DWORD HandleGetGuildFacilitiesInfo(tagNetCmd* pCmd);
	DWORD HandleHandInItems(tagNetCmd* pCmd);

	DWORD HandleSpreadGuildAffair(tagNetCmd* pCmd);

	DWORD HandleGetGuildSkillInfo(tagNetCmd* pCmd);
	DWORD HandleUpgradeGuildSkill(tagNetCmd* pCmd);
	DWORD HandleLearnGuildSkill(tagNetCmd* pCmd);
	DWORD HandleSetResearchSkill(tagNetCmd* pCmd);
	DWORD HandleActiveGuildSkill(tagNetCmd* pCmd);

	DWORD HandleGetCofCInfo(tagNetCmd* pCmd);
	DWORD HandleCloseCofC(tagNetCmd* pCmd);
	DWORD HandleBuyCofCGoods(tagNetCmd* pCmd);
	DWORD HandleSellCofCGoods(tagNetCmd* pCmd);
	DWORD HandleGetCommodityInfo(tagNetCmd* pCmd);
	DWORD HandleGetTaelInfo(tagNetCmd* pCmd);
	DWORD HandleGetCommerceRank(tagNetCmd* pCmd);

	DWORD HandleAcceptCommerce(tagNetCmd* pCmd);
	DWORD HandleCompleteCommerce(tagNetCmd* pCmd);
	DWORD HandleAbandonCommerce(tagNetCmd* pCmd);
	DWORD HandleSwitchCommendation(tagNetCmd* pCmd);

	// Jason 2010-2-20 v1.3.2 ������������
	// �õ�������ּ
	DWORD HandleGetGuildPurpose(tagNetCmd * pCmd);
	// �õ�ǰһ�������ɼ����Ϣ
	DWORD HandleGetGuildSmallInfo(tagNetCmd * pCmd);
	// ����������
	DWORD HandleApplyToJoinGuild(tagNetCmd * pCmd);
	// ��ϵ����
	DWORD HandleContactGuildLeader(tagNetCmd * pCmd);
	// ��ȡ�����������嵥
	DWORD HandleGetGuildApplierBill(tagNetCmd * pCmd);
	// ���ݰ�����׼�����������Ϣ
	DWORD HandleApproveToJoinGuild(tagNetCmd * pCmd);

	DWORD HandleSearchGuild(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// �������
	//------------------------------------------------------------------------
	DWORD HandleCreateFamily(tagNetCmd * pCmd);
	DWORD HandleDismissFamily(tagNetCmd * pCmd);
	DWORD HandleJoinFamilyReq(tagNetCmd * pCmd);
	DWORD HandleJoinFamilyReqRes(tagNetCmd * pCmd);
	DWORD HandleLeaveFamily(tagNetCmd * pCmd);
	DWORD HandleKickFromFamily(tagNetCmd * pCmd);
	DWORD HandleHandOverFamily(tagNetCmd * pCmd);
	DWORD HandleFamilyApply(tagNetCmd * pCmd);
	DWORD HandleFamilyApplyRes(tagNetCmd * pCmd);
	DWORD HandleGetFamilyName(tagNetCmd * pCmd);
	DWORD HandleGetFamilyInfo(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// �����������
	//------------------------------------------------------------------------
	
	DWORD HandleStartTreasureChest(tagNetCmd* pCmd);
	DWORD HandleOpenTreasureChest(tagNetCmd* pCmd);
	DWORD HandleStopTreasureChest(tagNetCmd* pCmd);
	DWORD HandleAgainTreasureChest(tagNetCmd* pCmd);
	DWORD HandleChestGetItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���ؽ�ɫѡ��
	//------------------------------------------------------------------------
	DWORD HandleReturnRoleSelect(tagNetCmd* pCmd);

	// �ͻ��˶Ի��򷢸������ȱʡ��Ϣ
	DWORD HandleDlgDefaultMsg(tagNetCmd* pCmd);
	// �ͻ��˴����������ű���ȱʡ��Ϣ
	DWORD HandleDefaultRequest(tagNetCmd* pCmd);

	// ���������ҵ�װ����Ϣ
	DWORD HandleGetSomeoneEquip(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ְҵ���
	//------------------------------------------------------------------------
	DWORD HandleSetClass(tagNetCmd* pCmd);
	DWORD HandleChangeClass(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ʵ��ϵͳ���
	//------------------------------------------------------------------------
	DWORD HandleGetRankings(tagNetCmd* pCmd);
	DWORD HandleSynTotalStrength(tagNetCmd* pCmd);
	
	DWORD HandleGetStrengthRankings(INT nPage);
	DWORD HandleGetFameRankings(INT nPage);
	DWORD HandleGetLevelRankings(INT nPage);
	DWORD HandleGetPetRankings(INT nPage);
	DWORD HandleGetEquipRankings(INT nPage);
    DWORD HandleGetWuXunRankings(INT nPage);
	DWORD HandleGetFlowerRankings(INT nPage);
	DWORD HandleGetEggRankings(INT nPage);

	//------------------------------------------------------------------------
	// ������ʾ
	//------------------------------------------------------------------------
	DWORD HandleRoleLeaveNotify(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ����ϵͳ
	//------------------------------------------------------------------------
	DWORD HandleGetCityAttInfo(tagNetCmd* pCmd);
	DWORD HandleGetCityAttInfoByCityID(tagNetCmd* pCmd);
	DWORD HandleGetAppointedCityAtt(tagNetCmd* pCmd);
	DWORD HandleSetTaxRate(tagNetCmd* pCmd);
	DWORD HandleTransformTaxation(tagNetCmd* pCmd);
	DWORD HandleSignupForCitywar(tagNetCmd* pCmd);
	DWORD HandleGetSigupInfo(tagNetCmd* pCmd);
	DWORD HandleConfirmDefenceForce(tagNetCmd* pCmd);
	DWORD HandleGetCityWarInfo(tagNetCmd* pCmd);
	DWORD HandleGetCityWarCity(tagNetCmd* pCmd);
	DWORD HandleUpgradeProlificacy(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��սϵͳ
	//------------------------------------------------------------------------
	DWORD HandleWaitSummon(tagNetCmd* pCmd);
	DWORD HandleCancelSummon(tagNetCmd* pCmd);
	DWORD HandleCSRoleRecord(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ��Ʒϵͳ
	//------------------------------------------------------------------------
	DWORD HandleBindItem(tagNetCmd* pCmd);
	DWORD HandleTracePlayer(tagNetCmd* pCmd);
	DWORD HandleMove2Role(tagNetCmd* pCmd);
	DWORD HandleGetPlayerPos(tagNetCmd* pCmd);
	DWORD HandleEquipSignature(tagNetCmd* pCmd);
	DWORD HandleSpecScriptItem(tagNetCmd* pCmd);
	DWORD HandleAddPetLiveTime(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// ���ս���
	//------------------------------------------------------------------------
	DWORD HandleGetAllGodMiracle(tagNetCmd* pCmd);
	DWORD HandleApplyCapGodMiracle(tagNetCmd* pCmd);
	DWORD HandleGetAllGodInfo(tagNetCmd* pCmd);

	// ����ϵͳ
	DWORD HandleSpiritReward(tagNetCmd* pCmd);
	DWORD HandleStrength2Spirit(tagNetCmd* pCmd);

	// ����������
	DWORD HandleTeamProfit(tagNetCmd* pCmd);

    //����ϵͳ��� 
    DWORD HandlePvPEnterHall(tagNetCmd* pCmd); 
	DWORD HandlePvPLeaveHall(tagNetCmd* pCmd);
	
	DWORD HandlePvPGetRoomList(tagNetCmd* pCmd);
	DWORD HandlePvPGetRoomInfo(tagNetCmd* pCmd);
	DWORD HandlePvPGetSimRoomInfo(tagNetCmd* pCmd);

	DWORD HandlePvPInviteJoin(tagNetCmd* pCmd);
	DWORD HandlePvPInvite(tagNetCmd* pCmd);
	DWORD HandlePvPAutoJoinRoom(tagNetCmd* pCmd);
    DWORD HandlePvPJoinRoom(tagNetCmd* pCmd);
	DWORD HandlePvPCreateRoom(tagNetCmd* pCmd);
	DWORD HandlePvPLeaveRoom(tagNetCmd* pCmd);
	
	DWORD HandlePvPChangePos(tagNetCmd* pCmd);
	DWORD HandlePvPPrepare(tagNetCmd* pCmd);
	DWORD HandlePvPUnPrepare(tagNetCmd* pCmd);
    DWORD HandlePvPKickPlayer(tagNetCmd* pCmd); 
	DWORD HandlePvPStart(tagNetCmd* pCmd);

	//ѡ�����
    DWORD HandleQueryDragonGirlRankings(tagNetCmd* pCmd);
	DWORD HandleQueryDragonDefenderRankings(tagNetCmd* pCmd);
	DWORD HandleVoteForDrangonGirl(tagNetCmd* pCmd);
  
    //------------------------------------------------------------------------
	// ��֮����ϵͳ
	//------------------------------------------------------------------------
	DWORD HandleGetTrainList(tagNetCmd* pCmd);
	DWORD HandleFinishTrain(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// ս���һ�ϵͳ
	//------------------------------------------------------------------------
	DWORD HandleUseZhanDouFu(tagNetCmd* pCmd);
	DWORD HandleSyncAutoFightState(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// ����б������Ϣ
    //------------------------------------------------------------------------
	DWORD HandleDeleteEnemy(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// ��Ԫ
    //------------------------------------------------------------------------
	DWORD HandleGuiYuan(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// ����ս��
    //------------------------------------------------------------------------
	DWORD HandleDeclareGuildWar(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// KeyCodeBridge
    //------------------------------------------------------------------------
	DWORD HandleKeyCode(tagNetCmd* pCmd);
	
	//------------------------------------------------------------------------
	// ʦͽϵͳ
	//------------------------------------------------------------------------
	DWORD HandleApprenticeApplyMaster(tagNetCmd* pCmd);
	DWORD HandleMasterReply(tagNetCmd* pCmd);
	DWORD HandleMasterAcceptApprentice(tagNetCmd* pCmd);
	DWORD HandleApprenticeReply(tagNetCmd* pCmd);
	DWORD HandleMatesInfo(tagNetCmd* pCmd);
	DWORD HandleApprenticeInfo(tagNetCmd* pCmd);
	DWORD HandleGiveJingWuPoint(tagNetCmd* pCmd);
	DWORD HandleJingWu2Client(tagNetCmd* pCmd);
	DWORD HandleUseJingWuPoint(tagNetCmd* pCmd);
	DWORD HandleBreakOutMaster(tagNetCmd* pCmd);
	DWORD HandleForceBreakOutMaster(tagNetCmd* pCmd);
	DWORD HandleBreakOutMasterConfirm(tagNetCmd* pCmd);
	DWORD HandleBreakOutApprentice(tagNetCmd* pCmd);
	DWORD HandleBreakOutApprenticeConfirm(tagNetCmd* pCmd);

	// Jason 2010-4-15 v2.0.0 �������
	DWORD HandleFabaoPractice(tagNetCmd * pCmd);
	DWORD HandleFabaoInlay(tagNetCmd * pCmd);
	// 2010-4-16
	DWORD HandleFabaoWuxingReversal(tagNetCmd * pCmd);
	DWORD HandleFabaoWuxingMerging(tagNetCmd * pCmd);
	// 2010-7-5 v1.0.0 ������ҩ
	DWORD HandleFairyPowder(tagNetCmd * pCmd);
	DWORD HandleFairyRaiseIntelligenceMax(tagNetCmd * pCmd);
	DWORD HandleFairyDoRaiseIntelligence(tagNetCmd * pCmd);

	DWORD HandleEarthTemple(tagNetCmd * pCmd);
	DWORD HandleRaiseEquipPotval(tagNetCmd *);

	// ����ʦ
	DWORD HandleAppearanceTransform(tagNetCmd * pCmd);

	DWORD HandlePracticeSoaringSkill(tagNetCmd*pCmd);

	DWORD HandleBuildEuipment(tagNetCmd * pCmd);
	DWORD HandleCastingEquipment(tagNetCmd * pCmd);
	// Jason 2010-6-2 v2.2.0 �������ű����ˣ��ͻ��˷�����Ϣ
	DWORD HandlePullRoleMsgCallback(tagNetCmd*pCmd);
	// Jason 2010-6-18 v1.0.0
	DWORD HandleStrengthenEquip(tagNetCmd * pCmd);
	DWORD HandleDetachOrAttackSoulEquip(tagNetCmd * pCmd);

	DWORD HandleGetAllianceList(tagNetCmd * pCmd);
	DWORD HandleApplyForAlliance(tagNetCmd * pCmd);
	DWORD HandleConfirmAllianceGuild(tagNetCmd * pCmd);

	// ���������أ�������
	// ����������
	DWORD HandleOpenFairyTrain(tagNetCmd * pCmd);
	// ��׽����
	DWORD HandleArrestingFairy(tagNetCmd * pCmd);
	// �ر�������׽��Ϸ
	DWORD HandleCloseFairyTrain(tagNetCmd * pCmd);

	// ����
	DWORD	HandleGodStrengthen(tagNetCmd * pCmd);
	
	// ʹ��VIP��Ƭ����
	DWORD	HandleUseVipCard(tagNetCmd * pCmd);

	// ��ʯ��ʹ����ʯ��ϴװ��һ�������Լ��������Ը�����
	DWORD	HandleRockStone(tagNetCmd * pCmd);

	// ��֮����
	DWORD	HandleGodBiderShopOpen(tagNetCmd * pCmd);
	DWORD	HandleConformPrice(tagNetCmd * pCmd);
	DWORD	HandleBlackMarketShopOpen(tagNetCmd * pCmd);
	DWORD	HandleBlackConPrice(tagNetCmd * pCmd);

	// Jason 2010-11-29 Ŀ���Ŀ��
	DWORD	HandleTargetOfTarget(tagNetCmd * pCmd);

	// Jason 2010-11-30 ����ͻ�������ʰȡģʽ
	DWORD	HandleSetPickupMode(tagNetCmd * pCmd);

	// ͨ��������������
	DWORD	HandleRoleUseLimitedItem(tagNetCmd* pCmd);

public:
	// Jason 2010-5-18 v2.1.2 �������
	DWORD HandleQuerySoaringSwitch(tagNetCmd * pCmd);
	// ������
	DWORD HandleGetEasyTeamInfo(tagNetCmd * pCmd);
	// ���ɢ��
	DWORD HandleGetFreePlayers(tagNetCmd* pCmd);
	// ��ȡ���������嵥
	DWORD HandleGetPersonalInfo(tagNetCmd* pCmd);
	// �ύ��ӻ�����������
	DWORD HandleNCPutOutInfo(tagNetCmd*pCmd);
	// ɾ�����˵���������������
	DWORD HandleNCDeleteAppTeamInfo(tagNetCmd* pCmd);
	DWORD HandleNCDeleteMyTeamInfo(tagNetCmd* pCmd);

public:
	DWORD HandleOpenQuestBoard(tagNetCmd* pCmd);
	DWORD HandleRefreshQuestBoard(tagNetCmd* pCmd);
	// ����ιʳ
	DWORD HandleFabaoFeeding(tagNetCmd * pCmd);
	// ����Ԫ���˺�ת��
	DWORD HandleFabaoEleInjuryType(tagNetCmd* pCmd);
	// ������������
	DWORD HandleFabaoIncEleInjury(tagNetCmd * pCmd);
	// ������������������
	DWORD HandleFabaoAddPromoteTimes(tagNetCmd * pCmd);
	// �����ؼ�ȷ��
	DWORD HandleFabaoStuntConfirm(tagNetCmd * pCmd);
	// �����ؼ���
	DWORD HandleFabaoStuntBook(tagNetCmd * pCmd);
	// ��������
	DWORD HandleFabaoNirvana(tagNetCmd * pCmd);

	// ս������
	// ��ȡ����ռ��ս������
	DWORD HandleGuildWarOccupyedNum(tagNetCmd * pCmd);
	// ��ȡÿ�ս���
	DWORD HandleGuildWarTakeWarReword(tagNetCmd * pCmd);

	DWORD HandleSyncFaBaoStoreExpInfo(tagNetCmd * pCmd);
	DWORD HandleExpBall2ExpPilular(tagNetCmd * pCmd);
	DWORD HandleExpBallBind2Role(tagNetCmd * pCmd);
	DWORD HandleUseExpPilular(tagNetCmd * pCmd);

	// ������ֳ���
	// �鿴��������״̬
	DWORD	HandleFairyInquireSpirteBornState(tagNetCmd * pCmd);
	// ���� �������
	DWORD	HandleApplyFairyBirth(tagNetCmd * pCmd);
	// Ӧ���������
	DWORD	HandleReplyFairyBirth(tagNetCmd * pCmd);
	// ��������
	DWORD	HandlePutFairyInto(tagNetCmd * pCmd);
	// �������
	DWORD	HandleFairyMating(tagNetCmd * pCmd);
	// ȡ�������������
	DWORD	HandleCancelInvitingFairyMating(tagNetCmd * pCmd);
	// ��������
	DWORD	HandleFairyBorn(tagNetCmd * pCmd);

	// �ҽ�
	DWORD	HandleEggBegin(tagNetCmd * pCmd);
	DWORD	HandleBreakEgg(tagNetCmd * pCmd);
	DWORD	HandleTakeEggTreasure(tagNetCmd * pCmd);
	DWORD	HandleFinishEggGame(tagNetCmd * pCmd);

	// �ر�pk����ģʽ��ʾ��Ϣ
	DWORD	HandleClosePKSafeGuard(tagNetCmd * pCmd);

	// �³�
	DWORD	HandleRehanding(tagNetCmd * pCmd);
	DWORD	HandleWardrobeLevelUp(tagNetCmd * pCmd);
	DWORD	HandleDressUpAnotherRole(tagNetCmd * pCmd);
	DWORD	HandleIsDressUpAgree(tagNetCmd * pCmd);

	// ֹͣ�������
	DWORD	HandleStopGodCondense(tagNetCmd * pCmd);

	// ��ְ
	//�������ֵ
	DWORD HandleRoleGetAttribute(tagNetCmd *pCmd);
	// ��ְ����
	DWORD HandleRolePromotionClergy(tagNetCmd *pCmd);
	// ��ְ��ѡ
	DWORD HandleGetCompetitiveClergyResult(tagNetCmd *pCmd);
	DWORD HandleGetClergyEnrollInfo(tagNetCmd *pCmd);
	DWORD HandleRoleEnrollClergy(tagNetCmd *pCmd);
	DWORD HandleGetClergyRalation(tagNetCmd* pCmd);

	// ��ʼ�������
	DWORD HandleStartGodCondense(tagNetCmd * pCmd);

	//��ְ����
	DWORD HandleActivationArea(tagNetCmd *pCmd);
	DWORD HandleAddArea(tagNetCmd *pCmd);

	// XTRAP�����
	DWORD	HandleXTrap(tagNetCmd * pCmd);

	DWORD	HandleRoleInitDone(tagNetCmd * pCmd);

	//��������

	//��ȡ������������
	DWORD	HandleGetFamilySpriteTrain(tagNetCmd* pCmd);

	//��������
	DWORD	HandleFamilySpriteTrain(tagNetCmd* pCmd);

	//�����Ǽ�
	DWORD	HandleFamilySpriteRegister(tagNetCmd* pCmd);

	//ȡ������
	DWORD	HandleFamilySpriteGetBack(tagNetCmd* pCmd);

	//��ȡ����������Ϣ
	DWORD	HandleGetFamilyRoleSprite(tagNetCmd* pCmd);

	//��ȡ�����Ǽ���Ϣ
	DWORD	HandleGetFamilySpriteAtt(tagNetCmd* pCmd);

	//��ȡ����������Ϣ
	DWORD	HandleGetFamilySpriteInfo(tagNetCmd* pCmd);
	
	// ��ȡ����keycode
	DWORD	HandleRewardByKeyCode(tagNetCmd* pCmd);
	//DWORD HandleRewardReactiveAccount(tagNetCmd* pCmd);
	DWORD	HandleAccountReactive(tagNetCmd* pCmd);

	DWORD	HandleGetServiceInfo(tagNetCmd* pCmd);

	// �����������
	DWORD	HandleRoleOpenFamilyQuestPage(tagNetCmd* pCmd);
	DWORD	HandleRoleGetFamilyQuestYesterdayInfo(tagNetCmd* pCmd);
	DWORD	HandleRoleGiveInFamilyQuestItem(tagNetCmd* pCmd);
	DWORD	HandleRoleGetFamilyQuestReward(tagNetCmd* pCmd);

	DWORD	HandleGetLiLianExp(tagNetCmd* pCmd);
	DWORD	HandleReceiveGift(tagNetCmd* pCmd);

	DWORD	HandleRequestAttackCode(tagNetCmd* pCmd);
	DWORD	HandleServerAttack(tagNetCmd* pCmd);

	// ���
	DWORD	HandleTryMakeBrother(tagNetCmd* pCmd);

	// ����
	DWORD	HandleActivePulse(tagNetCmd* pCmd);

	// �ϻ���
	DWORD	HandleTiger(tagNetCmd* pCmd);

	//��ħ��
	DWORD	HandleActiveSoulSkill(tagNetCmd* pCmd);
	DWORD	HandleContinueSoulSkill(tagNetCmd* pCmd);

	//ˢ�³ɳ������Լ���ʯ��������
	DWORD   HandleUseDiamondEquipGrowIM(tagNetCmd* pCmd);

	//��Key��
	DWORD	HandleNC_GetKeyCodeGift(tagNetCmd* pCmd);

	// �����ں� [3/22/2012 zhangzhihua]
	DWORD	GetFairyQualityValue(BYTE byQuality);
	INT		GetFairyQuality(INT nValue, INT nNum);
	DWORD	HandleNC_FairyFusion(tagNetCmd* pCmd);
	DWORD	HandleNC_GetFairyFusion(tagNetCmd* pCmd);

	// ʥ����������
	DWORD	HandleNC_SLEatFairy(tagNetCmd* pCmd);

	// �ٻ�ʥ��
	DWORD	HandleNC_CallShengLing(tagNetCmd* pCmd);	

	// ��ʥ�鴩װ��
	DWORD	HandleNC_EquipHoly(tagNetCmd* pCmd);	

	// ��ʥ��жװ��
	DWORD	HandleNC_UnEquipHoly(tagNetCmd* pCmd);	

	// ��ʥ�Ƴ�����
	DWORD	HandleNC_LingNeng(tagNetCmd* pCmd);

	// ʥ�ƻ������Գ���
	DWORD	HandleNC_ChongNeng(tagNetCmd* pCmd);	

	// ʥ�Ƴ���ȷ��
	DWORD	HandleNC_ChongNengResult(tagNetCmd* pCmd);

	// ��������ֻ���
	DWORD  HandleNC_SaveTelNum(tagNetCmd* pCmd);

	// ��Ҳ��洢�ֻ���
	DWORD  HandleNC_NotSaveTelNum(tagNetCmd* pCmd);
	
	// ��ȡʥ�Ҿ�
	DWORD HandleNC_ReceiveYuanBao(tagNetCmd* pCmd);

#ifdef ON_GAMEGUARD
	// (����)�ӿͻ��˽���CS��֤��
	DWORD   HandleAnswerKoreaCSApprove(tagNetCmd* pCmd);
	// (����)�ӿͻ��˽���GameGuard����
	DWORD   HandleGameGuardReport(tagNetCmd* pCmd);
#endif

private:
	static PlayerNetCmdMgr	m_PlayerNetMgr;						// ��Ӧ�Ŀͻ�����Ϣ������
	static GMCommandMgr		m_GMCommandMgr;						// GM���������

	TMap<DWORD, DWORD>		m_mapLastRecvTime;					// �ϴν�����Ϣ��ʱ��

	DWORD					m_dwAccountID;						// session id����Ӧ�����ʺ�ID
	DWORD					m_dwInternalIndex;					// �ײ������ID
	INT						m_nMsgNum;							// ����ײ�δ�������Ϣ����

	bool					m_bRoleLoading;						// ѡ������ʱ���ڵȴ����ݿⷵ��
	bool					m_bRoleEnuming;						// ������Ϸʱ�ȴ�������ѡ�˽���ɫ��Ϣ��ȡ����
	bool					m_bRoleEnumDone;					// ��ȡ��ɫ��Ϣ���
	bool					m_bRoleEnumSuccess;					// ��ȡ��ɫ��Ϣ�Ƿ�ɹ�
	bool					m_bRoleCreating;					// �ȴ�������ɫ
	bool					m_bRoleDeleting;					// ɾ������ʱ�ȴ����ݿⷵ��
	BYTE					m_byPrivilege;						// gmȨ��
	INT8					m_n8RoleNum;						// �Ѿ������Ľ�ɫ����

	bool					m_bRoleInWorld;						// ����Ϸ������
	INT					m_nDistributionID;						// �ͻ��˵���Ӫ��ID

	DWORD					m_dwRoleID[MAX_ROLENUM_ONEACCOUNT];	// �ʺ������н�ɫID
	
	char					m_szAccount[X_SHORT_NAME];			// ����˺�

	volatile BOOL			m_bConnectionLost;					// �����Ƿ��Ѿ��Ͽ�
	volatile BOOL			m_bKicked;							// �Ƿ��Ѿ����ߵ�

	tagAccountCommon		m_sAccountCommon;					// �˺�ͨ����Ϣ

	Role*					m_pRole;							// ��Ӧ�Ľ�ɫ����

	DWORD					m_dwIP;								// �ͻ���IP

	FatigueGuarder			m_FatigueGarder;					// ������


	tagDWORDTime			m_dwPreLoginTime;					// �ϴε�¼ʱ��
	DWORD					m_dwPreLoginIP;						// �ϴε�¼ip
	tagDWORDTime		m_dwLoginTime;						// ���ε�¼ʱ��

	FastMutex				m_PlayerNetLock;

	// Jason 2010-1-15 v1.3.2 
	OfflineReward	*		m_pOfflineExperienceReward;			// ���߽���
    volatile INT            m_nCreatedRoleNumLimit ;            // ���ƴ�����ɫ�Ĵ���
	
	INT16                   m_n16QuickSaveCount;                //��������ݵ�����

	// ս���һ�ϵͳ��Ϣ�ļ���crcֵ
	DWORD					m_dwSyncAfs;
	DWORD					m_dwUseZDF;
	BYTE						m_dwExtID;

	tagDWORDTime     m_dwPreXtrapUpdateTime;
	char							m_XtrapBuffSession[XTRAP_CS4_BUFSIZE_SESSION];

	DWORD                   m_dwChannelID;

	CHAR						m_szMac[X_SHORT_STRING];

	CHAR					m_szKrUserID[X_SHORT_STRING];

#ifdef ON_GAMEGUARD
public:
	CCSAuth3					m_CS;                    // �����汾ÿ����Ҷ�Ӧһ��CCSAuth3����
#endif

public:
	VOID RegisterUserOffline(Role * player);
	INT  GetCreatedRoleNumLimit() { return  m_nCreatedRoleNumLimit;}
	VOID DeductCreateRoleNumLimit(){ m_nCreatedRoleNumLimit--; }
	VOID SetCreatedRoleNumLimit(INT nCreateRoleNum) {  m_nCreatedRoleNumLimit = nCreateRoleNum;}
	// Jason 2010-4-26 v2.1.0
	INT						m_nPKRunawayTick;
	VOLATILE BOOL					m_bPKRunaway;

	//DWORD				m_dwInitTime;			// Ϊʥ���ٻ�״̬����������ʥ��ֵ��ʱ
	//DWORD				m_dwCoValueTime;		// Ϊ���߻ָ�ʥ��Ĭ��ֵ��ʱ

	// ʥ�Ƴ�����ȷ���޸ĵ����Լ���ֵ
	bool m_bNeedConfirm;	//�Ƿ���Ҫ����ȷ��
	BYTE m_byIndex;			//��Ҫȷ�ϳ��ܵ�ʥ������
	BYTE m_byAttType;		//��������
	DWORD m_dwAttValue;		//����ֵ
	DWORD m_dwLingNeng;		//���ܼ���ֵ
public:
	BOOL IsInPKRunawayPunishmentState(VOID);
};



//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: �������ݽṹ
//-----------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/move_define.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/pk_define.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/ScriptMsgInfo.h"
#include "../WorldDefine/msg_mall.h"

#include "../WorldDefine/msg_quickbar.h"
#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/msg_pk.h"
#include "../WorldDefine/msg_talent.h"
#include "../WorldDefine/action.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/msg_gm_cmd.h"
#include "../WorldDefine/msg_train.h"

#include "../WorldDefine/msg_guild.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/penalty_define.h"
#include "../WorldDefine/msg_GroupChat.h"

#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/msg_skill.h"
#include "../WorldDefine/quest.h"

#include "../WorldDefine/msg_AutoFight.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/data_collect.h"
#include "../ServerDefine/msg_account_reactive.h"
#include "egg_broker.h"

#include "../WorldDefine/msg_yuanshen.h"
#include "../ServerDefine/msg_holySoul.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "unit.h"
#include "item_mgr.h"
#include "currency.h"
#include "db_session.h"
#include "player_session.h"
#include "exchange.h"
#include "pet_exchange.h"
#include "suit.h"
#include "quest.h"
#include "stall.h"
#include "group_mgr.h"
#include "team.h"
#include "vcard.h"
#include "clandata.h"
#include "state_mgr.h"
#include "mutex.h"
#include "longhun.h"
#include "vip_netbar.h"
#include "role_mgr.h"
#include "quest_board.h"
#include <set>
#include "godarea_mgr.h"
#include "bloodbrother.h"
#include "TigerMgr.h"
#include "war_role.h"
#include "HolySoul.h"

class PlayerSession;
class Map;
class Quest;
class Skill;
class Creature;
class Stall;
class Team;
class RoleScript;
class TitleMgr;
class PetPocket;
class BoardQuestMgr;
class group_chat;

struct tagRoleDataLoad;
struct tagQuestDoneSave;
struct tagRoleTalent;
struct tagNDBC_SaveRole;
struct tagIMEffect;
struct tagQuickData;
struct tagQuickBarState;
struct tagAthleticsProperty;

// Jason 2010-1-15 v1.3.2 ���߹һ�
class OfflineReward;
class IpgCashSession;

enum EAutoFightState;

// �ٻ�ʥ��ʱ����ʥ��ֵ��ʱ����
const INT COST_HOLY_INTER_TICK		= 5 * TICK_PER_SECOND;

// ��������ʥ���Ĭ��ֵ�ĸ���ʱ����
const INT HOLY_COVALUE_INTER_TICK		= 1 * TICK_PER_SECOND * 400;

//--------------------------------------------------------------------------------
// �������ܺ�װ���Ĵ���������
//--------------------------------------------------------------------------------
enum EPassiveSkillAndEquipTrigger
{
	EPSAET_Null				=	-1,
	EPSAET_BeAttacked		=	0,	// ������
	EPSAET_Hit				=	1,	// ����
	EPSAET_Hited			=	2,	// ������
	EPSAET_Dodged			=	3,	// ������
	EPSAET_Dodge			=	4,	// ����
	EPSAET_Blocked			=	5,	// ����
	EPSAET_Block			=	6,	// ��
	EPSAET_Crit				=	7,	// ����
	EPSAET_Crited			=	8,	// ������
	EPSAET_Die				=	9,	// ����
	EPSAET_Random			=	10,	// ���
	EPSAET_Attack			=	11,	// ����
	EPSAET_Kill				=	12,	// ��ɱ
	EPSAET_End				=	13
};

// ʦ�����������仯����
enum EMasterApprenticeNumChg
{
	EMANC_NoChg = 0,	// û�б仯
	EMANC_BaiShi,		// ��ʦ
	EMANC_ShouTu,		// ��ͽ
	EMANC_TuoLiShiMen,	// ͽ������ʦ��
	EMANC_ZhuChuShiMen,	// ʦ����ͽ�����ʦ��
};

// �������������仯����


typedef VOID (Role::*pFun_RegTriggerEquipSet)(DWORD, DWORD, INT16);
//------------------------------------------------------------------------------
// ��ɫ��
//------------------------------------------------------------------------------
class Role : public Unit, public ScriptData<ESD_Role>
{
	friend class OfflineReward;
public:
	friend class PlayerSession;
	friend class GMCommandMgr;
	friend class RoleMgr;

public:
	typedef TMap<UINT16, tagQuestDoneSave*>			QuestDoneMap;
	typedef TMap<UINT16, Quest*>					QuestMap;

	typedef State<DWORD, ERoleState>				RoleState;
	typedef State<DWORD, ERoleStateEx>				RoleStateEx;

	typedef TMap<UINT16, tagTrainState*>			TrainStateMap;

protected:
	//---------------------------------------------------------------------------
	// Constructor&Destructor
	//---------------------------------------------------------------------------
	Role(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession);
	virtual ~Role();
public:
	static Role* Create(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession);
	static VOID	Delete(Role* &pRole);

	//---------------------------------------------------------------------------
	// ��ʼ��
	//---------------------------------------------------------------------------
	VOID Init(const tagRoleDataLoad* pData);

	//---------------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------------
	VOID Online(BOOL bFirst);

	//----------------------------------------------------------------------------
	// ��һ������
	//----------------------------------------------------------------------------
	VOID VirginOnline();

	//----------------------------------------------------------------------------
	// ����Update����
	//----------------------------------------------------------------------------
	virtual VOID Update();

	//-----------------------------------------------------------------------------
	// �������ݿ�
	//-----------------------------------------------------------------------------
	VOID SaveToDB(BOOL bExit = FALSE);

	//-----------------------------------------------------------------------------
	// ���뵽��Ϸ������
	//-----------------------------------------------------------------------------
	BOOL AddToWorld(BOOL bFirst);

	//-----------------------------------------------------------------------------
	// ���͵���ĳ����ͼ��
	//-----------------------------------------------------------------------------
	BOOL GotoNewMap(DWORD dwDestMapID, FLOAT fX, FLOAT fY, FLOAT fZ, DWORD dwMisc=0, BOOL bSameInstance=TRUE, DWORD dwDestMapInstID = 0);

	//------------------------------------------------------------------------------
	// �س�
	//------------------------------------------------------------------------------
	VOID ReturnCity();

	//------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------
	VOID ReName(LPCTSTR szNewName);

	//------------------------------------------------------------------------------
	// ��Ʒ�Ƿ���Ҫ��ȴ
	//------------------------------------------------------------------------------
	BOOL ObjectCoolOff()
	{
		return m_bObjectCoolOff;
	}

	VOID SetObjectCoolMode(BOOL bMode)
	{
		m_bObjectCoolOff = bMode;
	}

	//------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------
	BOOL SpeakOff() { return m_bSpeakOff; }
	VOID SetSpeakOff(BOOL bFlag) { m_bSpeakOff = bFlag; }

	//-----------------------------------------------------------------------
	// ����Ƿ��ڶ�Ӧְ��NPC����
	//-----------------------------------------------------------------------
	DWORD CheckFuncNPC(DWORD dwNPCID, EFunctionNPCType eNPCType, OUT Creature **ppNPC = NULL, OUT Map **ppMap = NULL);

	//------------------------------------------------------------------------------
	// ��ǰ״̬ -- ��״̬���ת������Ҫ�ֶ���ɡ���SetState()�����Զ��������״̬λ��
	//------------------------------------------------------------------------------
	DWORD GetRoleState() const { return m_RoleState.GetState(); }

	BOOL IsInRoleState(ERoleState eState) const
	{
		return m_RoleState.IsInState(eState);
	}

	BOOL IsInRoleStateAll(DWORD dwState) const
	{
		return m_RoleState.IsInStateAll(dwState);
	}

	BOOL IsInRoleStateAny(DWORD dwState) const
	{
		return m_RoleState.IsInStateAny(dwState);
	}

	VOID SetRoleState(ERoleState eState, BOOL bSendMsg=TRUE)
	{
		m_RoleState.SetState(eState);

		if( bSendMsg )
		{
			tagNS_SetRoleState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	VOID UnsetRoleState(ERoleState eState, BOOL bSendMsg=TRUE)
	{
		if( FALSE == IsInRoleState(eState) ) return;

		m_RoleState.UnsetState(eState);

		if( bSendMsg )
		{
			tagNS_UnSetRoleState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	//------------------------------------------------------------------------------
	// ��չ״̬ -- ��״ֻ̬����ұ���֪���Ϳ���
	//------------------------------------------------------------------------------
	const RoleStateEx& GetRoleStateEx() const
	{
		return m_RoleStateEx;
	}
	
	VOID SetRoleStateEx(ERoleStateEx eState, BOOL bSendMsg=TRUE)
	{
		if( TRUE == m_RoleStateEx.IsInState(eState) ) return;

		m_RoleStateEx.SetState(eState);

		if( bSendMsg )
		{
			tagNS_SetRoleStateEx send;
			send.eState = eState;
			SendMessage(&send, send.dwSize);
		}
	}

	VOID UnsetRoleStateEx(ERoleStateEx eState, BOOL bSendMsg=TRUE)
	{
		if( FALSE == m_RoleStateEx.IsInState(eState) ) return;

		m_RoleStateEx.UnsetState(eState);

		if( bSendMsg )
		{
			tagNS_UnSetRoleStateEx send;
			send.eState = eState;
			SendMessage(&send, send.dwSize);
		}
	}

	//------------------------------------------------------------------------------
	// �Ƿ��ڲ����ƶ���״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantMove()
	{
		return Unit::IsInStateCantMove() || IsInRoleState(ERS_Stall) ;
	}

	//------------------------------------------------------------------------------
	// �Ƿ��ڲ���ʹ�ü��ܵ�״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantCastSkill()
	{
		return Unit::IsInStateCantCastSkill() || IsInRoleState(ERS_Stall) || IsInRoleState(ERS_Wedding);
	}

	//------------------------------------------------------------------------------
	// �Ƿ��ڲ��ɱ�����״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantBeSkill()
	{
		return Unit::IsInStateCantBeSkill() || IsInRoleState(ERS_Stall) || IsInRoleState(ERS_Wedding);
	}
	//------------------------------------------------------------------------------
	//�Ƿ��ڲ����Ա�������ӵ�״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantBeInviteJoinTeam()
	{ 
		return Unit::IsInStateCantBeInviteJoinTeam() || IsInRoleState(ERS_Wedding);
	} 
	//------------------------------------------------------------------------------
	// �Ƿ��ڲ��ɼ���״̬
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateInvisible()
	{
		return Unit::IsInStateInvisible();
	}

#ifdef ON_INTERNET_CAFE
	VOID EnableInternetCafe();		// �������ɽ���Ч�� [5/25/2012 zhangzhihua]
	VOID CancelInternetCafe();		// �ر����ɽ���Ч�� [5/25/2012 zhangzhihua]
	VOID UpdateInternetCafe();
	VOID SetInternetCafePingLostTime(BYTE byTime);
	BYTE GetInternetCafePingLostTime();
	VOID EndInternetCafe() { m_bTimeout = TRUE; }
	BOOL IsEnd() { return m_bTimeout; }
#endif

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// ������ڵ�ͼ����
	//------------------------------------------------------------------------------
	VOID CheckMapArea();
	
	//------------------------------------------------------------------------------
	// ��Ǯ����Ʒ
	//------------------------------------------------------------------------------
	CurrencyMgr&	GetCurMgr()		{ return m_CurMgr; }
	ItemMgr&		GetItemMgr()	{ return m_ItemMgr; }
	HolySoul&		GetHolySoul()	{ return m_HolySoul;}

	//-------------------------------------------------------------------------------
	// �������ݻ�ȡ
	//-------------------------------------------------------------------------------
	INT						GetLevelPm()	const { return m_nLevelPm; }
	EClassType				GetClass()		const { return m_eClass; }
	EClassTypeEx			GetClassEx()	const { return m_eClassEx; }
	INT						GetCredit()		const { return m_nCredit; }
	INT						GetIdentity()	const { return m_nIdentity; }
	//INT						GetVIPPoint()	const { return GetCurMgr().GetVIPPoint(); }
	PlayerSession*			GetSession()	const { return m_pSession; }
	const tagAvatarAtt*		GetAvatar()		const { return &m_Avatar; }
	const tagDisplaySet&	GetDisplaySet()	const { return m_DisplaySet; }
	ERolePKState			GetPKState()	const { return m_ePKState; }
	DWORD					GetRebornMapID()const { return m_dwRebornMapID; }
	DWORD					GetNameID()		const { return GetID(); }			//?? �ýӿ����һ������
	DWORD					GetTeamID()		const { return m_dwTeamID; }
	DWORD					GetGroupID()	const { return m_dwGroupID; }
	DWORD					GetTeamInvite()	const { return m_dwTeamInvite; }
	DWORD					GetEarnRate()	const { return GetSession()->GetFatigueGuarder().GetEarnRate();}
	FLOAT					GetSpiRate()	const { return m_fSpiRate; }
	virtual BYTE			GetSex()		const { return m_Avatar.bySex; }
	INT						GetOnlineTime() const { return m_nOnlineTime; }
	tagDWORDTime			GetLogoutTime() const { return m_LogoutTime; }
	tagDWORDTime			GetRoleCreateTime() const {return m_CreatTime; }

	tagDWORDTime			GetLoginTime() const {return m_LoginTime;}

	const tagRemoteOpenSet& GetRemoteOpenSet() const { return m_sRemoteOpenSet; }

	LPCTSTR					GetVNBName() const { return GetSession()->GetVNBName(); }

	const tagAvatarEquip&	GetAvatarEquip()
	{
		if (GetDisplaySet().bFashionDisplay)
		{
			if (GetWardrobe().IsPlayActing() || GetWardrobe().IsBePlayActing())
				return m_AvatarEquipWardrobe;
			else
				return m_AvatarEquipFashion;
		}
		else
		{
			return m_AvatarEquipEquip;
		}
	}

	const tagChestItem&		GetChestItem()	const	{ return m_TreasureState.ChestItem; }
	INT64					GetChestSerial() const	{ return m_TreasureState.nChestSerial; }
	INT64					GetKeySerial() const	{ return m_TreasureState.nKeySerial; }
	DWORD					GetChestTypeID() const	{ return m_TreasureState.dwChestTypeID; }
	DWORD					GetKeyTypeID() const	{ return m_TreasureState.dwKeyTypeID; }
	DWORD					GetMasterID() const		{ return m_dwMasterID; }
	TMap<DWORD, DWORD>&		GetApprenticeData() 	{ return m_mapApprentice; }
	INT						GetJingWuPoint() const	{ return m_nJingWuPoint; }
	INT						GetQinWuPoint() const	{ return m_nQinWuPoint; }
	INT						GetLevelUpBonus() const	{ return m_nLevelUpBonus; }
	INT						GetRewardNum()	const	{ return m_nRewardNum; }
	tagClassMateData&		GetClassMate()			{ return m_ClassMateData; }

	const RoleScript*		GetScript()				{ return m_pScript; } 

	//-------------------------------------------------------------------------------
	// ������������
	//-------------------------------------------------------------------------------

	// ͨ������ʦ�ı�avatar��ò,����TRUE,��ʾ�б仯��FALSE��ʾû�б仯
	BOOL SetAvatarHairAndFace(	WORD	wHairMdlID,			// ����
								WORD	wHairTexID,			// ��ɫ
								WORD	wFaceMdlID,			// �沿���
								WORD	wFaceDetailTexID	// �沿ϸ��
							)
	{
		if (m_Avatar.wHairMdlID != wHairMdlID ||
			m_Avatar.wHairTexID != wHairTexID ||
			m_Avatar.wFaceMdlID != wFaceMdlID ||
			m_Avatar.wFaceDetailTexID != wFaceDetailTexID)
		{
			m_Avatar.wHairMdlID = wHairMdlID;
			m_Avatar.wHairTexID = wHairTexID;
			m_Avatar.wFaceMdlID = wFaceMdlID;
			m_Avatar.wFaceDetailTexID = wFaceDetailTexID;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	VOID SetClass(EClassType eVocation)		
	{
		BOOL bFiltrateSoulSkill = FALSE;
		if ( m_eClass != eVocation )
		{
			bFiltrateSoulSkill = TRUE;
		}
		m_eClass = eVocation;
		g_roleMgr.SetRoleClass(GetID(), static_cast<BYTE>(eVocation));

		if( bFiltrateSoulSkill )
			FiltrateSoulSkill();
	}
	
	VOID SetClassEx(EClassTypeEx eHVocation)	
	{
		m_eClassEx = eHVocation;
	}

	VOID SetBaseStrength(INT nStrength)
	{
		m_Strength.nBaseStrength = nStrength;
	}

	VOID SetStrengthPm(INT nPm)
	{
		m_nStrengthNum = nPm;
	}

	VOID SetChestTypeID(DWORD dwTypeID)
	{
		m_TreasureState.dwChestTypeID = dwTypeID;
	}

	VOID SetKeyTypeID(DWORD dwTypeID)
	{
		m_TreasureState.dwKeyTypeID = dwTypeID;
	}

	VOID SetChestSerial(INT64 nSerial)
	{
		m_TreasureState.nChestSerial = nSerial;
	}

	VOID SetKeySerial(INT64 nSerial)
	{
		m_TreasureState.nKeySerial = nSerial;
	}

	VOID SetChestItem(tagChestItem item)
	{
		m_TreasureState.ChestItem = item;
	}

	VOID SetFashionMode(bool bDisplay)
	{
		m_DisplaySet.bFashionDisplay = bDisplay;
	}

	VOID SetDisplaySet(bool bHead, bool bFace, bool bBack)
	{
		m_DisplaySet.Set(bHead, bFace, bBack);
	}

	VOID SetEquipDisplay(BOOL bFashion, INT nDisplayPos, DWORD dwTypeID, BYTE byFlareVal, INT8 n8ColorID)
	{
		if(bFashion)	// ʱװ
		{
			m_AvatarEquipFashion.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
		}
		else	// ���������
		{
			m_AvatarEquipEquip.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);

			// ������������������⴦��
			if(MIsWeaponByDisplayPos(nDisplayPos))
			{
				m_AvatarEquipFashion.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
				m_AvatarEquipWardrobe.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
			}
		}
	}

	VOID ResetWardrobeDisplay()
	{
		ZeroMemory(&m_AvatarEquipWardrobe, SIZE_AVATAR_EQUIP);
	}
	VOID SetWardrobeDisplay(INT nDisplayPos, DWORD dwTypeID, BYTE byFlareVal, INT8 n8ColorID)
	{
		m_AvatarEquipWardrobe.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
	}

	// ����������Ч��Ҫ��������
	VOID SetEquipEffect(INT nDisplayPos, BYTE byEquipEffect)
	{
		m_AvatarEquipEquip.Set(nDisplayPos, byEquipEffect);
		m_AvatarEquipFashion.Set(nDisplayPos, byEquipEffect);
		m_AvatarEquipWardrobe.Set(nDisplayPos, byEquipEffect);
	}

	// ȡ�ÿɼ�������ID
	LongHun& GetLongHun()				{ return m_LongHun; }

	// ���ü�¼ʽ���ͷ�������
	DWORD SetItemTransportPoint(tagNetCmd* pCmd);

	// ʹ�ü�¼ʽ���ͷ�
	DWORD UseNoteTakingItemTransport(tagNetCmd* pCmd);

	// ����ʦͽ��ϵ
	VOID ModifyMasterApprenticeCount(INT nCount)
	{
		m_nMARelation += nCount;
		if(m_nMARelation < 0) m_nMARelation = 0; 
		g_roleMgr.ModifyMasterApprenticeCount(m_dwID, nCount);
	}

	// ����ʦ��ID
	VOID SetMasterID(DWORD dwMasterID)
	{
		m_dwMasterID = dwMasterID;
	}

	// ���ӵ���
	VOID AddApprentice(DWORD dwApprenticeID)
	{
		m_mutexMaster.Acquire();
		if (m_mapApprentice.Size() >= MAX_APPRENTICE_NUM)
		{
			m_mutexMaster.Release();
			return;
		}

		m_mapApprentice.Add(dwApprenticeID, dwApprenticeID);
		tagApprenticeData appData;

		m_mapApprentice.ResetIterator();
		DWORD dwData = GT_INVALID;
		int i = 0;
		while (m_mapApprentice.PeekNext(dwData))
		{
			appData.dwData[i++] = dwData;
			if (i >= MAX_APPRENTICE_NUM)
			{
				break;
			}
		}

		m_ApprenticeData = appData;
		m_mutexMaster.Release();
	}

	// ɾ������
	VOID DeleteApprentice(DWORD dwApprenticeID)
	{
		m_mutexMaster.Acquire();

		m_mapApprentice.Erase(dwApprenticeID);
		tagApprenticeData appData;

		m_mapApprentice.ResetIterator();
		DWORD dwData = GT_INVALID;
		int i = 0;
		while (m_mapApprentice.PeekNext(dwData))
		{
			appData.dwData[i++] = dwData;
		}

		m_ApprenticeData = appData;
		m_mutexMaster.Release();
	}

	// ����ͬ��
	VOID AddClassMate(DWORD dwMateID)
	{
		m_mutexMate.Acquire();

		for (INT i=0; i!= MAX_APPRENTICE_NUM; ++i)
		{
			// ����ͬ���ظ�
			if (m_ClassMateData.dwData[i] == dwMateID)
			{
				break;
			}
			if (m_ClassMateData.dwData[i] == 0)
			{
				m_ClassMateData.dwData[i] = dwMateID;
				break;
			}
		}

		m_mutexMate.Release();
	}

	// ɾ��ͬ��
	VOID DeleteClassMate(DWORD dwMateID)
	{
		m_mutexMate.Acquire();

		for (INT i=0; i!= MAX_APPRENTICE_NUM; ++i)
		{
			if (m_ClassMateData.dwData[i] == dwMateID)
			{
				m_ClassMateData.dwData[i] = 0;
				break;
			}
		}

		m_mutexMate.Release();
	}

	// ɾ������ͬ��
	VOID DeleteAllClassMate()
	{
		m_mutexMate.Acquire();

		for (INT i=0; i!= MAX_APPRENTICE_NUM; ++i)
		{
			m_ClassMateData.dwData[i] = 0;
		}

		m_mutexMate.Release();
	}

	// �޸ľ������
	VOID SetJingWuPoint(INT nPoint)
	{
		m_nJingWuPoint = nPoint;
	}

	// �޸��������
	VOID SetQinWuPoint(INT nPoint)
	{
		m_nQinWuPoint = nPoint;
	}

	// �޸�ͽ����������
	VOID SetLevelUpBonus(INT nPoint)
	{
		m_nLevelUpBonus = nPoint;
	}

	// �޸ĳ�ʦ��������
	VOID SetRewardNum(INT nNum)
	{
		m_nRewardNum = nNum;
	}

	//-------------------------------------------------------------------------------
	// ��ϰ�ɽ缼��
	//-------------------------------------------------------------------------------
	DWORD PracticeMiracleSkill(DWORD dwSkillID);

	DWORD UseAddMiraclePrcNumItem(INT64 n64ItemID);

	INT16 GetMiraclePrcNum() { return m_n16MiraclePracticeNum; }

	//-------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------
	BOOL	IsInGuild()	const			{ return m_dwGuildID != GT_INVALID; }
	DWORD	GetGuildID() const			{ return m_dwGuildID; }
	VOID	SetGuildID(DWORD dwGuildID);
	DWORD	GetFamilyID() const			{ return m_dwFamilyID; }
	VOID SetFamilyID(DWORD dwFamilyID) { m_dwFamilyID = dwFamilyID; }

	//-------------------------------------------------------------------------------
	// ���������
	//-------------------------------------------------------------------------------
	VOID   InitMapBuffLeftToClient();            //��
    VOID   UpdateSendMapBuffLeftTimeToClient();  //���ͽ���ͼbuff��ʣ��ʱ����ͻ���      
    //���õ�ǰ��ҵ�ͼbuffID
	VOID   SetRoleMapBuffID(DWORD dwBuffId )  {   m_dwMapBuffID = dwBuffId; }
	DWORD  GetRoleMapBuffID()const      {    return m_dwMapBuffID; }
	
	private:
    DWORD  m_dwMapBuffID ;
	INT   m_nIsSended;
	
	public:
	//-------------------------------------------------------------------------------
	// �Ƿ���Ҫ���浽���ݿ�
	//-------------------------------------------------------------------------------
	BOOL IsNeedSave2DB() const { return m_nNeedSave2DBCountDown <= 0; }

	//-------------------------------------------------------------------------------
	// ������Ϣ
	//-------------------------------------------------------------------------------
	VOID SendMessage(LPVOID pMsg, DWORD dwSize, EMessageRange eRange = EMR_Self);

	//---------------------------------------------------------------------------------
	// Ͷ���Ե�
	//---------------------------------------------------------------------------------
	INT BidAttPoint(const INT nAttPointAdd[X_ERA_ATTA_NUM]);

	//---------------------------------------------------------------------------------
	// ϴ���Ե�
	//---------------------------------------------------------------------------------
	INT ClearAttPoint(INT64 n64ItemID, BOOL bCheckItem=TRUE);

	//---------------------------------------------------------------------------------
	// �Ƿ����ѧ����
	//---------------------------------------------------------------------------------
	INT	CanLearnSkill(DWORD dwSkillID, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// ѧ����
	//---------------------------------------------------------------------------------
	VOID LearnSkill(DWORD dwSkillID, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID, BOOL bDelTalentPoint = TRUE);

	//---------------------------------------------------------------------------------
	// ѧ����
	//---------------------------------------------------------------------------------
	INT	LearnSkill(DWORD dwSkillID, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// ְҵ�ж�
	//---------------------------------------------------------------------------------
	DWORD VocationCheck(DWORD dwVocationLimit);

	//---------------------------------------------------------------------------------
	// �Ƿ������������
	//---------------------------------------------------------------------------------
	INT CanLevelUpSkill(Skill* pSkill, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------
	VOID LevelUpSkill(Skill* pSkill, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID, BOOL bDelTalentPoint = TRUE);

	//---------------------------------------------------------------------------------
	// ���ǰ�ü���
	//---------------------------------------------------------------------------------
	INT CheckPreSkill(DWORD dwPreLevelSkillID);

	//---------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------
	INT LevelUpSkill(DWORD dwSkillID, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------------------
	INT ForgetSkill(DWORD dwSkillID, DWORD dwNPCID);

	//---------------------------------------------------------------------------------
	// ϴ��
	//---------------------------------------------------------------------------------
	INT	ClearTalent(INT64 n64ItemID, BOOL bCheckItem=TRUE);

	//---------------------------------------------------------------------------------
	// ����/�ر�PK����
	//---------------------------------------------------------------------------------
	INT OpenSafeGuard(INT64 n64Item);
	INT CloseSafeGuard();

	//---------------------------------------------------------------------------------
	// ����/��������״̬
	//---------------------------------------------------------------------------------
	INT SetPK(BOOL bOnlyPK);
	VOID ResetPK();
	VOID UpdatePK();

	//����ϵͳ
	VOID UpdateSpirit();
	float CalSpiritRate(INT nSpirit);
	INT64  ModFixSpiriteCDTime(INT64 nValue) 
	{
		m_dwFixSpiriteCDTime += nValue;
		if(m_dwFixSpiriteCDTime < 0 ) m_dwFixSpiriteCDTime = 0;
		return m_dwFixSpiriteCDTime;
	}

	//---------------------------------------------------------------------------------
	// ����/����PVP״̬
	//---------------------------------------------------------------------------------
	VOID SetPVP();
	VOID UpdatePVP();

	//---------------------------------------------------------------------------------
	// ����PK״̬
	//---------------------------------------------------------------------------------
	VOID CalPKState(BOOL bSendMsg=TRUE);

	//---------------------------------------------------------------------------------
	// ��������Ϊ0ʱ�سǸ����pk����
	//---------------------------------------------------------------------------------
	VOID SetHostilitySafeGuard();
	VOID UpdateHostilitySafeGuard();

	//---------------------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------------------
	VOID BeRevived(INT nHP, INT nMP, Unit* pSrc);

	//---------------------------------------------------------------------------------
	// ͬ����������Ƿ�����
	//---------------------------------------------------------------------------------
	DWORD UpdateFriOnline();

	//---------------------------------------------------------------------------------
	// Ŀ���Ƿ���ȫ�ɼ�//??
	//---------------------------------------------------------------------------------
	BOOL CanSeeTargetEntirely(Unit *pUnit)
	{
		return (FriendEnemy(pUnit) & ETFE_Friendly) == ETFE_Friendly;
	}

	//---------------------------------------------------------------------------------
	// �������ø����
	//---------------------------------------------------------------------------------
	DWORD QuestSetRebornMap(UINT16 u16QuestID, DWORD &dwBornMapID, Vector3 &vBornPos);

	//---------------------------------------------------------------------------------
	// ��ӣ�ɾ���͸ı似�ܼ���
	//---------------------------------------------------------------------------------
	virtual VOID AddSkill(Skill* pSkill, BOOL bSendMsg=TRUE)
	{
		Unit::AddSkill(pSkill);

		AddSpecSkill(pSkill);

		Map *pMap = GetMap();
		if(P_VALID(pMap))
			UpdateSkillByMap(pSkill->GetID(),pMap->GetMapType(),pMap->GetMapID());

		if( bSendMsg )
		{
			// ���͸��Լ����һ������
			tagNS_AddSkill send;
			pSkill->GenerateMsgInfo(&send.Skill);
			SendMessage(&send, send.dwSize);

			// ���浽���ݿ�
			if(!pSkill->IsExtraSkill())
			{
				tagNDBC_AddSkill send1;
				send1.dwRoleID = GetID();
				send1.Skill.dwID = pSkill->GetID();
				send1.Skill.nSelfLevel = pSkill->GetSelfLevel();
				send1.Skill.nLearnLevel = pSkill->GetLearnLevel();
				send1.Skill.nProficiency = pSkill->GetProficiency();
				send1.Skill.nCoolDown = pSkill->GetCoolDownCountDown();
				g_dbSession.Send(&send1, send1.dwSize);
			}
		}
	};

	virtual VOID RemoveSkill(DWORD dwSkillID)
	{
		BOOL bExtra = FALSE;
		Skill* pSkill = GetSkill(dwSkillID);
		if(P_VALID(pSkill)) bExtra = pSkill->IsExtraSkill();

		if(P_VALID(pSkill))
			Unit::RemoveSkill(dwSkillID,pSkill->GetCanUseByMap());
		else
			Unit::RemoveSkill(dwSkillID);

		RemoveSpecSkill(dwSkillID);

		// ���͸��Լ�ɾ��һ������
		tagNS_RemoveSkill send;
		send.dwSkillID = dwSkillID;
		SendMessage(&send, send.dwSize);

		// ���͵����ݿ�ɾ��
		if(!bExtra)
		{
			tagNDBC_RemoveSkill send1;
			send1.dwRoleID = GetID();
			send1.dwSkillID = dwSkillID;
			g_dbSession.Send(&send1, send1.dwSize);
		}
	}

	virtual VOID ChangeSkillLevel(Skill* pSkill, ESkillLevelChange eType, INT nChange=1)
	{
		INT nLstLevel = pSkill->GetLevel();
		Unit::ChangeSkillLevel(pSkill, eType, nChange);

		// ���͸��Լ�����һ������
		tagNS_UpdateSkill send;
		pSkill->GenerateMsgInfo(&send.Skill);
		SendMessage(&send, send.dwSize);

		if( pSkill->GetLevel() != nLstLevel && !pSkill->IsExtraSkill() )
		{
			tagNDBC_UpdateSkill send1;
			send1.dwRoleID = GetID();
			send1.Skill.dwID = pSkill->GetID();
			send1.Skill.nSelfLevel = pSkill->GetSelfLevel();
			send1.Skill.nLearnLevel = pSkill->GetLearnLevel();
			send1.Skill.nProficiency = pSkill->GetProficiency();
			send1.Skill.nCoolDown = pSkill->GetCoolDownCountDown();
			g_dbSession.Send(&send1, send1.dwSize);
		}
	}

	virtual VOID ChangeSkillExp(Skill *pSkill, INT nValue)
	{
		Unit::ChangeSkillExp(pSkill, nValue);
		
		// ���͸��Լ�����һ������
		tagNS_UpdateSkill send;
		pSkill->GenerateMsgInfo(&send.Skill);
		SendMessage(&send, send.dwSize);
	}

	virtual VOID StartSkillCoolDown(Skill* pSkill);

	virtual VOID ClearSkillCoolDown(DWORD dwSkillID);

	//--------------------------------------------------------------------------------
	// ���Ըı�������������ݸı�
	//--------------------------------------------------------------------------------
	virtual VOID OnAttChange(INT nIndex);
	virtual VOID OnAttChange(bool bRecalFlag[ERA_End]);

	//--------------------------------------------------------------------------------
	// ��û���پ���
	//--------------------------------------------------------------------------------
	VOID ExpChange(INT64 nValue, BOOL bKill=FALSE, BOOL bForce=FALSE,BOOL bShare = FALSE);

	//--------------------------------------------------------------------------------
	// �ı�ȼ�
	//--------------------------------------------------------------------------------
	VOID LevelChange(INT nValue, BOOL bKill=FALSE, BOOL bForce=FALSE);

	//--------------------------------------------------------------------------------
	// ������װ��Ч
	//--------------------------------------------------------------------------------
	VOID SetSuitEffect(DWORD dwSuitEffect)
	{ 
		m_AvatarEquipEquip.dwSuitEffectID = dwSuitEffect;
		m_AvatarEquipFashion.dwSuitEffectID = dwSuitEffect;
		m_AvatarEquipWardrobe.dwSuitEffectID = dwSuitEffect;
	}

	//--------------------------------------------------------------------------------
	// ��װ���
	//--------------------------------------------------------------------------------
	VOID ProcEquipEffect(tagEquip* pNewEquip, tagEquip* pOldEquip, INT16 n16IndexOld, BOOL bDiscard, BOOL bSend, BOOL byFamily = FALSE  );
	INT32 GetEquipDisplayPos(INT16 n16EquipPos);
	DWORD Equip(INT64 n64Serial, EEquipPos ePosDst);
	VOID  ResetWeaponDmg(tagEquip& Equip);				// ո�¶ȱ仯ʱ����
    
	//--------------------------------------------------------------------------------
	// ���װ�����Զ��������л���ʾģʽ
	//--------------------------------------------------------------------------------
	DWORD  EquipAutoDress(tagItem* pNewItem);
	BOOL  RewardEquip(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT32 nNum, EItemQuality eQlty, DWORD dwCmdID);

	//--------------------------------------------------------------------------------
	// ��ɫ�ɼ����ܼӳ�
	//--------------------------------------------------------------------------------
	INT CalGatherRate( Creature* pCreature );

	//--------------------------------------------------------------------------------
	// ��ɫʵ�ʴ�ֻ�þ������
	//--------------------------------------------------------------------------------
	INT64 CalRealGainExp( INT64 nSrcExp );

	//----------------------------------------------------------------------------------
	// ���������õ������б����Ӧλ��
	//----------------------------------------------------------------------------------
	tagFriend GetFriend(INT nIndex)
	{
		ASSERT(nIndex < MAX_FILENAME && nIndex >= 0);
		return m_Friend[nIndex];
	}

	VOID SetFriend(INT nIndex, DWORD dwFriendID, DWORD dwFriVal = 0, BYTE byGroup = 1)
	{
		ASSERT(nIndex < MAX_FRIENDNUM && nIndex >= 0);
		ASSERT(byGroup < 5 && byGroup > 0);

		if(m_Friend[nIndex].dwFriendID == GT_INVALID && dwFriendID != GT_INVALID)
		{
			m_mapFriend.Add(dwFriendID, &m_Friend[nIndex]);
		}
		else if(m_Friend[nIndex].dwFriendID != GT_INVALID && dwFriendID == GT_INVALID)
		{
			m_mapFriend.Erase(m_Friend[nIndex].dwFriendID);
		}

		m_Friend[nIndex].dwFriendID = dwFriendID; 
		if(dwFriendID == GT_INVALID)
			m_Friend[nIndex].dwFriVal = 0;
		else
			m_Friend[nIndex].dwFriVal += dwFriVal;	
		m_Friend[nIndex].byGroup = byGroup;

		m_Friend[nIndex].dwFriVal = (m_Friend[nIndex].dwFriVal > MAX_FRIENDVAL) ? MAX_FRIENDVAL : m_Friend[nIndex].dwFriVal;
	}

	tagFriend* GetFriendPtr(DWORD dwFriendID) { return m_mapFriend.Peek(dwFriendID); }

	//----------------------------------------------------------------------------------
	// ���������õ�����������Ӧλ��
	//----------------------------------------------------------------------------------
	DWORD GetBlackList(INT nIndex)
	{
		ASSERT(nIndex < MAX_BLACKLIST && nIndex >= 0);
		return m_dwBlackList[nIndex];
	}

	VOID SetBlackList(INT nIndex, DWORD dwRoleID)
	{
		ASSERT(nIndex < MAX_BLACKLIST && nIndex >= 0);
		m_dwBlackList[nIndex] = dwRoleID;
	}

	//----------------------------------------------------------------------------------
	// ���������õ������������Ӧλ��
	//----------------------------------------------------------------------------------
	DWORD GetEnemyList(INT nIndex)
	{
		ASSERT(nIndex < MAX_ENEMYNUM && nIndex >= 0);
		return m_dwEnemyList[nIndex];
	}

	VOID SetEnemyList(INT nIndex, DWORD dwRoleID)
	{
		ASSERT(nIndex < MAX_ENEMYNUM && nIndex >= 0);
		m_dwEnemyList[nIndex] = dwRoleID;
	}

	//----------------------------------------------------------------------------------
	// ����С�Ӻ��Ŷ�ID
	//----------------------------------------------------------------------------------
	VOID SetTeamID(DWORD dwTeamID)		{ m_dwTeamID = dwTeamID; }
	VOID SetGroupID(DWORD dwGroupID)	{ m_dwGroupID = dwGroupID; }
	VOID SetTeamInvite(DWORD dwRoleID)	{ m_dwTeamInvite = dwRoleID; }

	BOOL IsTeamMate(Role* pRole)		{ return GetTeamID() != GT_INVALID && GetTeamID() == pRole->GetTeamID(); }
	BOOL IsGuildMate(Role* pRole)		{ return GetGuildID() != GT_INVALID && GetGuildID() == pRole->GetGuildID(); }
	BOOL IsLover(Role* pRole)			{ return GetLoverID() != GT_INVALID && GetLoverID() == pRole->GetID(); }
	BOOL IsMasterApprentice(Role* pRole)	{ return ( ( (GetMasterID() != GT_INVALID) && (GetMasterID() == pRole->GetID()) ) || 
															((pRole->GetMasterID() != GT_INVALID) && (pRole->GetMasterID() == GetID())) ); }
	BOOL IsMasterMate(Role* pRole)		{ return GetMasterID() != GT_INVALID && GetMasterID() == pRole->GetMasterID(); }
	BOOL IsFamilyMate(Role* pRole)		{ return GetFamilyID() != GT_INVALID && GetFamilyID() == pRole->GetFamilyID(); }

	//----------------------------------------------------------------------------------
	// ��ĳ����λ�ǲ����Ѻõ�����ϵ�����ѣ���ݣ�ʦͽ�����ޣ�ͬ�ŵȣ�
	//----------------------------------------------------------------------------------
	BOOL IsFriendlySocial(Role* pRole)	{ return IsTeamMate(pRole) || IsGuildMate(pRole) || IsLover(pRole) || IsMasterApprentice(pRole) || IsMasterMate(pRole) || IsFamilyMate(pRole); }

	//----------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------
	DWORD		GetOwnInstanceID()	const;
	DWORD		GetOwnInstanceMapID() const;
	DWORD		GetMyOwnInstanceID() const			{ return m_dwOwnInstanceID; }
	DWORD		GetMyOwnInstanceMapID() const		{ return m_dwOwnInstanceMapID; }
	DWORD		GetExportMapID() const				{ return m_dwExportMapID; }
	Vector3&	GetExportPoint()					{ return m_vExport; }
	VOID		SetMyOwnInstanceID(DWORD dwID)		{ m_dwOwnInstanceID = dwID; }
	VOID		SetMyOwnInstanceMapID(DWORD dwID)	{ m_dwOwnInstanceMapID = dwID; }
	VOID		SetExportMapID(DWORD dwMapID)		{ m_dwExportMapID = dwMapID; }
	VOID		SetExportPoint(Vector3 &vExport)	{ m_vExport = vExport; }

	//----------------------------------------------------------------------------------
	// �̳����
	//----------------------------------------------------------------------------------
	tagDWORDTime GetLastMallFreeTime() const { return m_LastGetMallFreeTime; }

	VOID SetLastGetMallFreeTime(DWORD dwNewLastGetTime) { m_LastGetMallFreeTime = dwNewLastGetTime; }
	
	//----------------------------------------------------------------------------------
	// ��������״̬����
	//----------------------------------------------------------------------------------
	BOOL OnActiveItemBuffTrigger(tagItem* pItem, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// ��������״̬����
	//----------------------------------------------------------------------------------
	BOOL OnPassiveSkillBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// װ����������
	//----------------------------------------------------------------------------------
	BOOL OnEquipmentBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// ע�ᴥ�������ܹ�����
	//----------------------------------------------------------------------------------
	VOID RegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID);

	//----------------------------------------------------------------------------------
	// ��ע�ᴥ�������ܹ�����
	//----------------------------------------------------------------------------------
	VOID UnRegisterTriggerSkillSet(ETriggerEventType eType, DWORD dwSkillID);

	//----------------------------------------------------------------------------
	// ע�ᴥ����װ��������
	//----------------------------------------------------------------------------
	VOID RegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetEquipTrigger[eTriggerType][n16EquipPos] = true;
	}

	//----------------------------------------------------------------------------
	// ��ע�ᴥ����װ��������
	//----------------------------------------------------------------------------
	VOID UnRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetEquipTrigger[eTriggerType][n16EquipPos] = false;
	}

	//----------------------------------------------------------------------------
	// ע�ᴥ�������������
	//----------------------------------------------------------------------------
	VOID RegisterTriggerLongHunSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetLongHunTrigger[eTriggerType][n16EquipPos] = true;
	}

	//----------------------------------------------------------------------------
	// ��ע�ᴥ�������������
	//----------------------------------------------------------------------------
	VOID UnRegisterTriggerLongHunSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetLongHunTrigger[eTriggerType][n16EquipPos] = false;
	}

	//----------------------------------------------------------------------------------
	// ע�ᴥ������װ������
	//----------------------------------------------------------------------------------
	VOID RegisterTriggerSuitSet(DWORD dwTriggerID, DWORD dwBuffID, DWORD dwSuitID)
	{
		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		m_setSuitTrigger[eTriggerType].insert(dwSuitID);
	}

	//----------------------------------------------------------------------------------
	// ��ע�ᴥ������װ������
	//----------------------------------------------------------------------------------
	VOID UnRegisterTriggerSuitSet(DWORD dwTriggerID, DWORD dwBuffID, DWORD dwSuitID)
	{
		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		m_setSuitTrigger[eTriggerType].erase(dwSuitID);
	}
	
	//----------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------
	VOID	UpdateLoverID(DWORD dwLoverID)
	{ 
		m_dwLoverID = dwLoverID; 
		if(P_VALID(g_roleMgr.GetRoleInfo(GetID())))
			g_roleMgr.GetRoleInfo(GetID())->dwLoverID = dwLoverID;
	}
	DWORD	GetLoverID() { return m_dwLoverID; }

	BOOL IsRoleMarried() { return (m_dwLoverID != GT_INVALID) ? TRUE:FALSE ; } //�Ƿ��ѻ�
	BOOL IsRoleWedded() { return m_bHaveWedding; } //�Ƿ���й�����
	

private:
	//----------------------------------------------------------------------------------
	// װ�����buffԤ��������ȡtigger����
	//----------------------------------------------------------------------------------
	EPassiveSkillAndEquipTrigger PreRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, BOOL bEquip);

	//----------------------------------------------------------------------------------
	// װ����ر���trigger��������
	//----------------------------------------------------------------------------------
	VOID OnEquipmentBuffTriggerCommon(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);
	VOID OnEquipmentBuffTriggerLongHun(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);
	VOID OnEquipmentBuffTriggerSuit(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);

private:
	//------------------------------------------------------------------------------
	// ���Ϳͻ��˳�ʼ����
	//------------------------------------------------------------------------------
	VOID SendInitPersonalSet();
	VOID SendInitStateAtt();
	VOID SendInitStateSkill();
	VOID SendInitStateItem();	// ��Ʒ&װ��
	VOID SendInitStateSuit();	// ��װ
	VOID SendInitStateLongHun();
	VOID SendInitStateCompleteQuest();
	VOID SendInitStateDailyCompleteQuest();
	VOID SendInitStateIncompleteQuest();
	VOID SendInitStateMoney();
	VOID SendInitStateReputation();
	VOID SendFriendAndEnemy();
	VOID SendInitStateGuild();				// ����
	VOID SendFatigueGuardInfo(BYTE byCode);	// ������
	VOID SendInitStateCity();
	VOID SendInitHolySoulInfo();

	//------------------------------------------------------------------------------
	// ��ʼ�������ݿ��ж�ȡ���б�����
	//------------------------------------------------------------------------------
	VOID InitAtt(const tagRoleDataSave* pData);
	VOID InitSkill(const BYTE* &pData, INT32 nNum);
	VOID InitBuff(const BYTE* &pData, INT32 nNum);
	VOID InitItem(const BYTE* &pData, INT32 nNum, INT32 nOffMinute);
	VOID InitItemCDTime(const BYTE* &pData, INT32 nNum);
	VOID InitFriend(const BYTE* &pData, INT32 nNum);
	VOID InitEnemy(const BYTE* &pData, INT32 nNum);	// ����б������Ϣ
	VOID InitFriendValue(const BYTE* &pData, INT32 nNum);
	VOID InitBlackList(const BYTE* &pData, INT32 nNum);
	VOID InitMaster(const BYTE* &pData);
	VOID InitClassMates(const BYTE* &pData);
	VOID InitStrength();
	VOID InitHolySoulInfo(const BYTE* &pData,INT nOffHour/*����Сʱ*/);

	DWORD Put2Container(tagItem* pItem);

	VOID CalInitAtt();

	VOID CalInitState();

	//------------------------------------------------------------------------------
	// ��ʼ���豣�����ݿ������
	//------------------------------------------------------------------------------
	VOID SaveSkill2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveBuff2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveQuest2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveMaster2DB(IN LPVOID pData, OUT LPVOID &pOutPointer);
	VOID SaveBoardQuest2DB(IN LPVOID pData, OUT LPVOID &pOutPointer);	
	VOID SaveHolySoulInfo(IN LPVOID pData, OUT LPVOID &pOutPointer);

	//------------------------------------------------------------------------------
	// ���ñ��浽���ݿ�ʱ�䵹��ʱ
	//------------------------------------------------------------------------------
	VOID ResetNeedSave2DBCD() { m_nNeedSave2DBCountDown = MIN_ROLE_SAVE2DB_INTERVAL; }

	//------------------------------------------------------------------------------
	// ����
	//------------------------------------------------------------------------------
	DWORD SetRebornMap(DWORD dwNPCID, DWORD &dwBornMapID, Vector3 &vBornPos);
	DWORD Revive(ERoleReviveType eType, INT64 n64ItemID, BOOL bNeedItem=TRUE);

	DWORD CityRevive();
	DWORD LocusRevive(INT64 n64ItemID, BOOL bNeedItem=TRUE);
	DWORD AcceptRevive();
	DWORD PrisonRevive();
	DWORD PerfectRevive();
	DWORD IsWarRevive();	//ս������
	//------------------------------------------------------------------------------
	// ��װ
	//------------------------------------------------------------------------------
	DWORD	CanEquip(tagEquip* pEquip, EEquipPos ePosDst);
	DWORD	Unequip(INT64 n64Serial, INT16 n16IndexDst);
	DWORD	SwapWeapon();
	DWORD	MoveRing(INT64 n64SerialSrc, INT16 n16PosDst);
public:
	VOID	ProcEquipEffectPos(tagEquip* pNewEquip, tagEquip* pOldEquip, EItemConType eConTypeNewDst, EItemConType eConTypeNewSrc);
	//VOID	RecalAttEx(BOOL bSend);
	VOID	ActiveHolySoul();
	VOID	ProcEquipEffectAtt(tagEquip* pEquip, bool bEquip, const INT16 n16Index, bool bIgnoreWar = false, bool bRecal = false);
private:
	VOID	ProcEquipEffectAvatar(tagEquip* pNewEquip, INT16 n16IndexOld);

public:
	VOID	RecalEquipEffectAttForWar(bool bEnterWarMap);
	INT32	ResetOneEquipDisplay(tagEquip* pEquip, INT16 n16Index);
	//���ս����Ʒ�����¼���װ������
	VOID	GetWarItemRecalEffect(INT16 nIndex, bool bGet);

public:
	//����󶨺ͷǰ��Ͼ���¶��Ҫ���ĸ���
	//VOID	CalculateItemCostNum(DWORD dwTypeID, TList<tagItem*>& itemListBind,TList<tagItem*>& itemListUnBind,INT& nLjxlNum ,INT& nItemBindNum, INT& nItemUnBindNum);

private:
	VOID	ChangeRoleAtt(const tagRoleAttEffect Effect[], INT32 nArraySz, INT32 nFactor);
	VOID	ChangeRoleAtt(const INT32 nValue[], INT32 nArraySz, INT32 nAttStart, INT32 nFactor, bool bPctIgnore=false);
	VOID	ChangeRoleAtt(const DWORD dwValue[], INT32 nArraySz, INT32 nFactor, BYTE nDiamondAddLevel=0);	//��ӱ�ʯ������أ���һ��������ʯ����ǿ��
	VOID	ChangeRoleAtt(const INT32 nValue[], INT32 nArraySz, INT32 nArrayStartIdx, INT32 nAttStart, INT32 nFactor);
	VOID	ProcEquipBuffTrigger(DWORD dwBuffID, BOOL bEquip);
	VOID	ProcSoulCrystalAttEffect(tagEquip* pEquip, bool bEquip);
	//------------------------------------------------------------------------------
	// �����˺���ؼ���
	//------------------------------------------------------------------------------
	/* ����ĥ��������˺�Ӱ��.����ֵΪ�������˺�Ӱ��İٷֱ�ֵ.*/
	FLOAT	CalAbrasionEffect(const tagEquip& Equip);
	VOID	ResetWeaponDmg(const tagEquip& Equip, BOOL bEquip);	// ��װʱ����

	//------------------------------------------------------------------------------
	// ��Ҽ佻�����
	//------------------------------------------------------------------------------
	ExchangeMgr& GetExchMgr() { return m_ExchMgr; }
	
	BOOL	IsExchanging()					{ return IsInRoleState(ERS_Exchange); }
	BOOL	CanExchange()
	{
		return !IsInRoleStateAny(ERS_Exchange | ERS_Shop | ERS_Stall | ERS_Wedding) 
				&& (GetExchMgr().GetTgtRoleID() == GT_INVALID);
	}
	
	VOID	BeginExchange(DWORD dwTgtRoleID)
	{
		SetRoleState(ERS_Exchange);
		GetExchMgr().CreateData();
		GetExchMgr().SetTgtRoleID(dwTgtRoleID);
	}

	VOID	EndExchange()
	{
		UnsetRoleState(ERS_Exchange);
		GetExchMgr().DeleteData();
		GetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	DWORD	ProcExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD	ProcExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode);
	DWORD	ProcExchangeAdd(OUT Role* &pTarget, OUT tagItem* &pItem, INT32 &nInsIndex, INT64 n64Serial);
	DWORD	ProcExchangeDec(OUT Role* &pTarget, INT64 n64Serial);
	DWORD	ProcExchangeMoney(OUT Role* &pTarget, INT64 n64Silver);
	DWORD	ProcExchangeLock(OUT Role* &pTarget);
	DWORD	ProcExchangeCancel(OUT Role* &pTarget);
	DWORD	ProcExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID);

	DWORD	VerifyExchangeData(OUT tagItem* pItem[]);
	DWORD	ProcExchange();

	DWORD	ProcPrepareExchange(OUT Role* &pTarget);
	public:
	VOID    ClenupExchState();	
	private:


	//---------------------------------------------------------------------------------
	// �̵����
	//---------------------------------------------------------------------------------
	DWORD	GetShopItems(DWORD dwNPCID, BYTE byShelf, DWORD dwNPCTypeID);
	DWORD	GetShopEquips(DWORD dwNPCID, BYTE byShelf, DWORD dwNPCTypeID);
	DWORD	BuyShopItem(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT16 n16ItemNum, DWORD dwNPCTypeID);
	DWORD	BuyShopEquip(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial, DWORD dwNPCTypeID);
	DWORD	SellToShop(DWORD dwNPCID, INT64 n64Serial, DWORD dwNPCTypeID);

	VOID	SendShopFeedbackMsg(DWORD dwErrorCode, DWORD dwNPCID);

	//-----------------------------------------------------------------------------
	// �̳����
	//-----------------------------------------------------------------------------
	DWORD	GetMallAll(OUT DWORD &dwMallTime);
	DWORD	UpdateMallAll(OUT DWORD &dwNewMallTime, IN DWORD dwOldMallTime);
	DWORD	BuyMallItem(DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict, INT16 n16OfferItemLvl, ELogCmdID eCmdID = ELCID_Mall_BuyItem);
	DWORD	BuyMallPack(DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShopingDistrict);
	DWORD	BuyMallItem(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
							DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict);
	DWORD	BuyMallPack(DWORD dwTgtRoleID, LPCTSTR szLeaveWord,
							DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShoppingDistrict);
	DWORD	GetMallFreeItem(DWORD dwID);
	DWORD	BuyMallCart(DWORD* dwID, INT* nIndex, INT16* n16BuyNum);

	//-----------------------------------------------------------------------------
	// �������
	//-----------------------------------------------------------------------------
	//���
	DWORD ProcGetMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID, DWORD dwNpcID);
	DWORD ProcGetMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode);

	//���
	DWORD ProcBreakMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD ProcForceBreakMarriageReq(DWORD dwTgtRoleID);
	DWORD ProcBreakMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode);

	//����
	DWORD ProcCelebrateWeddingReq(OUT Role* &pTarget,OUT Role* &pOfficiatorRole,DWORD dwOfficiatorNameCrc,DWORD dwNpcID);
	DWORD ProcCelebrateWeddingReqRes(OUT Role* &pApplicant, DWORD dwErrorCode);
public:
	DWORD	MallItemExchange(DWORD dwMallID, INT nPrice, INT16 n16BuyNum, INT nIndex);
	DWORD	MallPackExchange(DWORD dwMallID, INT nPrice, INT nIndex);

public:
	DWORD	RoleSetVocation(DWORD dwNPCID, EClassType eVocation);
	DWORD	RoleSetHeroVocation(DWORD dwNPCID, EClassTypeEx eHVocation);
	DWORD	ChangeVocation(DWORD dwNPCID, EClassType eVocation);

private:

	//-----------------------------------------------------------------------------
	// �̳�Ԫ���������
	//-----------------------------------------------------------------------------
	DWORD	SaveYB2Account(DWORD dwID, INT nNum);
	DWORD 	SaveSilver2Account(DWORD dwID, INT64 nNum);
	DWORD 	DepositYBAccout(DWORD dwID, INT nNum);
	DWORD 	DepositSilverAccount(DWORD dwID, INT64 nNum);
	DWORD 	GetYBTradeInfo();
	DWORD 	SubmitSellOrder(DWORD dwRoleID, INT nNum, INT nPrice);
	DWORD 	SubmitBuyOrder(DWORD dwRole, INT nNum, INT nPrice);
	DWORD 	DeleteOrder(DWORD dwRoleID, DWORD dwOrderID, EYBOTYPE eYBOType);
	DWORD 	GetYBOrder(DWORD dwRoleID);

public:
	//------------------------------------------------------------------------
	// ��̯���
	//------------------------------------------------------------------------
	DWORD	StartStall()
	{
		DWORD dwRtv = m_pStall->Init(0);
		if (E_Success == dwRtv)
		{
			StopMount();
		}
		return dwRtv;//?? ����˰��
	}

	DWORD	SetStallGoods(INT64 n64Serial, INT64 n64UnitPrice, BYTE byIndex)
	{
		return m_pStall->SetGoods(n64Serial, n64UnitPrice, byIndex);
	}

	DWORD	UnsetStallGoods(const BYTE byIndex)
	{
		return m_pStall->UnSetGoods(byIndex);
	}

	DWORD	SetStallTitle(LPCTSTR strTitle)
	{
		return m_pStall->SetTitle(strTitle);
	}

	DWORD	SetStallAdText(LPCTSTR strAdText)
	{
		return m_pStall->SetAdText(strAdText);
	}

	DWORD	SetStallAdFlag(bool bAdFlag)
	{
		return m_pStall->SetAdFlag(bAdFlag);
	}

	DWORD	SetStallFinish()
	{
		return m_pStall->SetFinish();
	}

	DWORD	CloseStall()
	{
		return m_pStall->Destroy();
	}

	DWORD	GetStallTitle(OUT LPTSTR pSzTitle)
	{
		return m_pStall->GetStallTitle(pSzTitle);
	}

	DWORD	GetStallSpecGoods(BYTE byIndex, OUT LPVOID pData, OUT INT &nGoodsSz)
	{
		return m_pStall->GetSpecGoods(byIndex, pData, nGoodsSz);
	}
	
	DWORD	GetStallGoods(OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz)
	{
		return m_pStall->GetGoods(pData, byGoodsNum, nGoodsSz);
	}

	DWORD	BuyStallGoods(Role *pBuyer, INT64 n64Serial, INT64 n64UnitPrice, 
							INT16 n16Num, BYTE byIndex, OUT INT16 &n16RemainNum)
	{
		return m_pStall->Sell(pBuyer, n64UnitPrice, n64Serial, byIndex, n16Num, n16RemainNum);
	}

	BYTE	GetStallModeLevel() const
	{
		return m_pStall->GetModeLevel();
	}

	//INT8	GetStallGoodsNum() const
	//{
	//	return m_pStall->GetGoodsNum();
	//}

	BOOL	IsNoGoodsInStall() const
	{
		return m_pStall->IsEmpty();
	}

	BOOL	IsSetGoodsFinish() const
	{
		return IsInRoleState(ERS_Stall);
	}

	INT32	CalStallGoodsMemUsed() const	// ����̯λ����Ʒ����ṹtagMsgStallGoodsʱ�Ĵ�С
	{
		return m_pStall->CalMemSzGoodsUsed();
	}

	DWORD	SendCloseStall();

	DWORD	GainStallExp(INT32 nExp)
	{
		return m_pStall->GainExp(nExp);
	}

	// ����ƣ��ֵ
	VOID ResetWearyValue()
	{
		SetAttValue(ERA_Weary, 0);
	}

public:
	VOID	CalEquipFlare(tagEquip* pEquip);

private:
	//---------------------------------------------------------------------------------
	// ����ְ��NPC����
	//---------------------------------------------------------------------------------
	DWORD	ProcDak(DWORD dwNPCID, INT32 nIndex, DWORD dwMapID);

	//---------------------------------------------------------------------------------
	// ʹ��ĥʯ
	//---------------------------------------------------------------------------------
	DWORD	AbraseWeapon(INT64 n64AbraserSerial, INT64 n64WeaponSerial, OUT INT32 &nAttackTimes);

	//---------------------------------------------------------------------------------
	// ��ͼ������
	//---------------------------------------------------------------------------------
	BOOL	MapTrigger(DWORD dwTriggerID, DWORD dwMisc);

	//---------------------------------------------------------------------------------
	// ֪ͨ���ѽ��븱��
	//---------------------------------------------------------------------------------
	DWORD	InstanceNotify(BOOL bNotify);

	//---------------------------------------------------------------------------------
	// ����Ƿ�ͬ��������ҽ��븱��������
	//---------------------------------------------------------------------------------
	DWORD	InstanceAgree(BOOL bAgree);

	//---------------------------------------------------------------------------------
	// ��������뿪����
	//---------------------------------------------------------------------------------
	DWORD	LeaveInstance();

	//---------------------------------------------------------------------------------
	// ǿ��װ��
	//---------------------------------------------------------------------------------
	// ����
	DWORD	PosyEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);	
	DWORD	GMPosyEquip(DWORD dwFormulaID, INT16 n16ItemIndex);	
	// �Կ�
	DWORD	EngraveEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	GMEngraveEquip(DWORD dwFormulaID, INT16 n16ItemIndex);
	// ��Ƕ
	DWORD	InlayEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID);
	// ��ӡ
	DWORD	BrandEquip(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD dwCmdID);
	// ����
	DWORD	LoongSoulEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID);
	DWORD	GMLoongSoulEquip(INT64 n64SrcItemID, INT16 n16ItemIndex);
	// ���
	DWORD	QuenchEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	// ����
	DWORD	ChiselEquip(INT64 n64SrcItemID, INT64 n64SuffID, DWORD dwCmdID);
	// ʱװȾɫ
	DWORD	DyeFashion(INT64 n64DyeSN, INT64 n64EquipSerial, DWORD dwCmdID);
	// ����ʱװ
	DWORD	SpinFashion(ESuitAttType eSpinType, INT64 n64EquipSerial, DWORD dwStuffTypeID, DWORD dwStuffItemNum, DWORD dwCmdID);
	// ��ȡ
	DWORD	Extraction(DWORD dwNPCID, INT64 n64Item);

	//�õ����Ľ׶�
	INT GetSpinStep(BYTE byMinUseLevel);

	//�õ�����װ����Ʒ��
	EItemQuality GetSpinItemQuality(ESpinStep eStep);

	// ����B�����Զ�ǿ���ɹ��ʵ�Ӱ��
	FLOAT	CalSpecAttEffectSuc(EEquipSpecAtt eSpecAtt, FLOAT fProp, EEquipSpecAtt eSpecType);
	// ����B�����Զ�ǿ�������ʵ�Ӱ��
	FLOAT	CalSpecAttEffectPef(EEquipSpecAtt eSpecAtt, FLOAT fPerfect);
	// ������ֿ�
	DWORD	OpenSideWare(INT64 n64ItemSerial);
	
	//---------------------------------------------------------------------------------
	// �����ϳ���Ʒ
	//---------------------------------------------------------------------------------
	DWORD	ProduceItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	ProduceEquip(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	CheckProduceLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, const tagProduceProtoSer* &pProduceProto, INT64 n64StuffID[], INT32 nArraySz);
	DWORD	DeComposeItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64Item, DWORD dwCmdID);
	DWORD	CheckDeComposeLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, const tagDeComposeProtoSer* &pDeComposeProto, INT64 n64Item, tagEquip *pEquip);
	EProduceType Skill2ProduceType(ESkillTypeEx2 eSkillType);	
	tagItem*	 CreateItem(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num, DWORD dwCreator);
	VOID	CalIMEffect(EConsolidateTypeSer eConType, tagIMEffect &IMEffect, INT64 n64IMID, const LPVOID pProto);
	// Jason 2009-12-6 ����װ��Ǳ��ֵ���ߴ���
	DWORD RaiseEquipPotVal(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD & dwPotValAdded);

	// Jason v1.3.1 date:2009-12-21
	DWORD GetGemRemovalInfo(DWORD dwNPCID,INT64 equipID,INT8 & num,DWORD dwGemIDs[MAX_EQUIPHOLE_NUM]);
	DWORD RemoveGemFromEquip(DWORD dwNPCID,INT64 equipID,INT8 num,INT64 SignIDs[MAX_EQUIPHOLE_NUM]);

	//---------------------------------------------------------------------------------
	// ��ѯĳ���������ڵ�λ�ã����û�У�����GT_INVALD��
	//---------------------------------------------------------------------------------
	INT FindTalentIndex(ETalentType eType)
	{
		INT nBegin = X_MAIN_TALENT_START, nEnd = X_FRESHHAND_TALENT;	// ������ʼ����յ�

		// ���������ϵ���ʣ���ȡ��벿��
		if( ETT_FreshHand == eType)
		{
			nBegin	=	X_FRESHHAND_TALENT;
			nEnd	=	X_MAX_TALENT_PER_ROLE;
		}

		// ����������������ʣ���
		for(INT n = nBegin; n < nEnd; n++)
		{
			if( m_Talent[n].eType == eType )
				return n;
		}

		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// �ҵ�ĳ���ɲ��������λ�ã����û�У�����GT_INVALID��
	//---------------------------------------------------------------------------------
	INT FindBlankTalentIndex(ETalentType eType)
	{
		INT nBegin = X_MAIN_TALENT_START, nEnd = X_FRESHHAND_TALENT;	// ������ʼ����յ�

		// ���������ϵ���ʣ���ȡ��벿��
		if( ETT_FreshHand == eType )
		{
			nBegin	=	X_FRESHHAND_TALENT;
			nEnd	=	X_MAX_TALENT_PER_ROLE;
		}

		for(INT n = nBegin; n < nEnd; n++)
		{
			if( ETT_Null == m_Talent[n].eType )
				return n;
		}

		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// ����һ������
	//---------------------------------------------------------------------------------
	VOID AddTalent(INT nIndex, ETalentType eType, INT nPoint=1)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null == m_Talent[nIndex].eType );
		ASSERT( nPoint > 0 );

		m_Talent[nIndex].eType = eType;
		m_Talent[nIndex].nPoint = nPoint;

		// ͬ��
		tagNS_AddTalent send;
		send.eType = eType;
		send.nPoint = nPoint;
		SendMessage(&send, send.dwSize);
	}

	//---------------------------------------------------------------------------------
	// ɾ��һ������
	//---------------------------------------------------------------------------------
	VOID RemoveTalent(INT nIndex)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );
		
		ETalentType eType = m_Talent[nIndex].eType;

		m_Talent[nIndex].eType = ETT_Null;
		m_Talent[nIndex].nPoint = 0;

		// ͬ��
		tagNS_RemoveTalent send;
		send.eType = eType;
		SendMessage(&send, send.dwSize);
	}


	//---------------------------------------------------------------------------------
	// �õ�ĳ����������Ӧ�����ʵ�ǰͶ����
	//---------------------------------------------------------------------------------
	INT GetTalentPoint(INT nIndex)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );

		return m_Talent[nIndex].nPoint;
	}
    
	//---------------------------------------------------------------------------------
	// �õ���ȡ��ǰ�Ѿ�Ͷ����������
	//---------------------------------------------------------------------------------
	INT GetTotalTalentPoint()
	{
		INT nTotalTalentPoint = 0;
		for (INT nIndex = 0 ; nIndex < X_MAX_TALENT_PER_ROLE; nIndex++)
		{
			if ( m_Talent[nIndex].eType != ETT_Null)
			{
				nTotalTalentPoint += m_Talent[nIndex].nPoint ;
			}
			
		}
        return nTotalTalentPoint;
	}
	//---------------------------------------------------------------------------------
	// �����ʼӵ�
	//---------------------------------------------------------------------------------
	VOID AddTalentPoint(INT nIndex, INT nPoint=1)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );
		ASSERT( nPoint > 0 );

		m_Talent[nIndex].nPoint += nPoint;

		// ͬ��
		tagNS_UpdateTalent send;
		send.eType = m_Talent[nIndex].eType;
		send.nPoint = m_Talent[nIndex].nPoint;
		SendMessage(&send, send.dwSize);
	}

	//---------------------------------------------------------------------------------
	// ������
	//---------------------------------------------------------------------------------
	virtual VOID OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited);

	//---------------------------------------------------------------------------------
	// ����
	//---------------------------------------------------------------------------------
	virtual VOID OnDead(Unit* pSrc, Skill* pSkill=NULL, const tagBuffProto* pBuff=NULL, DWORD dwSerial=GT_INVALID, DWORD dwMisc=0);

	//---------------------------------------------------------------------------------
	// ��ɱ
	//---------------------------------------------------------------------------------
	virtual VOID OnKill(Unit* pSrc);

	//---------------------------------------------------------------------------------
	// �Ƿ���������ͷ�
	//---------------------------------------------------------------------------------
	BOOL IsDeadPenalty(EDeadPenaltyMode eMode);

	//---------------------------------------------------------------------------------
	// �����ͷ�
	//---------------------------------------------------------------------------------
	VOID DeadPenalty(Unit* pSrc, EDeadPenaltyMode eMode);

	//---------------------------------------------------------------------------------
	// �Ƿ�����������ͷ�
	//---------------------------------------------------------------------------------
	VOID CanCancelPenalty(Unit* pSrc, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommercePenalty);

	//---------------------------------------------------------------------------------
	// Ŀ������ͱ�־
	//---------------------------------------------------------------------------------
	virtual DWORD TargetTypeFlag(Unit* pTarget);

	//---------------------------------------------------------------------------------
	// ״̬��־
	//---------------------------------------------------------------------------------
	virtual DWORD GetStateFlag()
	{
		DWORD dwStateFlag = Unit::GetStateFlag();

		dwStateFlag |= ( IsInRoleState(ERS_Mount)		?	ESF_Mount	:	ESF_NoMount );
		dwStateFlag |= ( IsInRoleState(ERS_PrisonArea)	?	ESF_Prison	:	ESF_NoPrison );
		dwStateFlag |= ( IsInRoleState(ERS_Commerce)	?	ESF_Commerce:	ESF_NoCommerce );
		//dwStateFlag |= ( IsInRoleState(ERS_Transform)	?	ESF_Transform:	ESF_NoTransform	);

		return dwStateFlag;
	}

	//---------------------------------------------------------------------------------
	// ��Ŀ��ĵ��������ж�
	//---------------------------------------------------------------------------------
	virtual DWORD FriendEnemy(Unit* pTarget);

	//----------------------------------------------------------------------------------
	// ���������Ͷ�Ӧ�������ܺ�װ������������
	//----------------------------------------------------------------------------------
	EPassiveSkillAndEquipTrigger TriggerTypeToPassiveSkillAndEquipTriggerType(ETriggerEventType eType)
	{   
		switch(eType)
		{
		case ETEE_BeAttacked:
			return EPSAET_BeAttacked;
			break;

		case ETEE_Hit:
			return EPSAET_Hit;
			break;

		case ETEE_Hited:
			return EPSAET_Hited;
			break;

		case ETEE_Dodged:
			return EPSAET_Dodged;
			break;

		case ETEE_Dodge:
			return EPSAET_Dodge;
			break;

		case ETEE_Blocked:
			return EPSAET_Blocked;
			break;

		case ETEE_Block:
			return EPSAET_Block;

		case ETEE_Crit:
			return EPSAET_Crit;
			break;

		case ETEE_Crited:
			return EPSAET_Crited;
			break;

		case ETEE_Die:
			return EPSAET_Die;
			break;

		case ETEE_Random:
			return EPSAET_Random;
			break;

		case ETEE_Attack:
			return EPSAET_Attack;
			break;

		case ETEE_Kill:
			return EPSAET_Kill;

		default:
			return EPSAET_Null;
			break;
		}
	}
	
private:
	DWORD TargetTypeFlag(Role* pTarget);
	DWORD TargetTypeFlag(Creature* pTarget);
	DWORD FriendEnemy(Role* pTarget);
	DWORD FriendEnemy(Creature* pCreature);

private:
	//---------------------------------------------------------------------------------
	// ����ɫ���ݱ��浽���ݿ����
	//---------------------------------------------------------------------------------
	class SaveRole
	{
	public:
		SaveRole();
		~SaveRole();

		VOID Init();
		operator tagNDBC_SaveRole*() { return m_pSaveRole; }

	private:
		tagNDBC_SaveRole*	m_pSaveRole;
	};

	static SaveRole	m_SaveRole;
	static Mutex	m_SaveRoleLock;
	
	//---------------------------------------------------------------------------------
	// �������
	//---------------------------------------------------------------------------------
	struct tagRevive
	{
		INT		nHP;
		INT		nMP;

		DWORD	dwMapID;
		FLOAT	fX;
		FLOAT	fY;
		FLOAT	fZ;

		tagRevive() { ZeroMemory(this, sizeof(*this)); }
	};

	tagRevive m_Revive;

private:
	//---------------------------------------------------------------------------------
	// ��Ӧ��session
	//---------------------------------------------------------------------------------
	PlayerSession*				m_pSession;

protected:
	//-------------------------------------------------------------------------------------
	// ʱ�����
	//-------------------------------------------------------------------------------------
	tagDWORDTime		m_CreatTime;						// ����ʱ��
	tagDWORDTime		m_LoginTime;						// ��½ʱ��
	tagDWORDTime		m_LogoutTime;						// �ǳ�ʱ��
	INT					m_nOnlineTime;						// �ۼ�����ʱ�䣨�룩
	INT					m_nCurOnlineTime;					// ��������ʱ�䣨�룩

	tagDWORDTime		m_dwLastUpgrade;					// ���һ������ʱ��

	tagDWORDTime		m_CloseSafeGuardTime;				// ��һ�ιر�PK������ʱ��
	INT					m_nUnSetPKTickCountDown;			// �ر��������״̬��Tick����ʱ
	INT					m_nUnSetPVPTickCountDown;			// �ر����PVP״̬��Tick����ʱ
	INT					m_nNeedSave2DBCountDown;			// ���Ա������ݿ⵹��ʱ
	INT					m_nDeadUnSetSafeGuardCountDown;		// �ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick

	DWORD				m_dwItemTransportMapID;				//��ǰ��¼���ͷ��󶨵�ͼID
	FLOAT				m_fItemTransportX;					//��ǰ��¼���ͷ��󶨵�x����
	FLOAT				m_fItemTransportZ;					//��ǰ��¼���ͷ��󶨵�z����
	FLOAT				m_fItemTransportY;					//��ǰ��¼���ͷ��󶨵�y����

	tagDWORDTime		m_LastGetMallFreeTime;				// ��һ�δ��̳ǻ�ȡ�����Ʒʱ��

	//-------------------------------------------------------------------------------------
	// ��ҵ�ǰ״̬
	//-------------------------------------------------------------------------------------
	RoleState			m_RoleState;						// ���״̬	-- �仯����֪ͨ��Χ���
	RoleStateEx			m_RoleStateEx;						// ���״̬ -- �仯��ֻ��Ҫ֪ͨ�Լ�
	ERolePKState		m_ePKState;							// ��ǰPK״̬

	//-------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------
	tagAvatarAtt		m_Avatar;							// �������
	tagAvatarEquip		m_AvatarEquipEquip;					// װ�����
	tagAvatarEquip		m_AvatarEquipFashion;				// ʱװ���
	tagAvatarEquip		m_AvatarEquipWardrobe;			// �³����
	tagDisplaySet		m_DisplaySet;						// װ����ʾ����
	EClassType			m_eClass;							// ְҵ
	EClassTypeEx		m_eClassEx;							// ��չְҵ
	INT					m_nCredit;							// ���ö�
	INT					m_nIdentity;						// ���
	INT					m_nVIPPoint;						// ��Ա����
	DWORD				m_dwGuildID;						// ��������ID
	DWORD				m_dwFamilyID;						// ��������ID
	DWORD				m_dwLoverID;						// ���˶�Ӧ��roleid û������GT_INVALID
	BOOL				m_bHaveWedding;						// �Ƿ���й�����
	BOOL				m_bNeedPrisonRevive;				// �Ƿ���Ҫ��������

	INT16				m_n16MiraclePracticeNum;			// �ɽ����д���
	tagDWORDTime		m_dwMiracleResetTime;				// �ɽ����е�������ʱ��

	DWORD				m_dwMasterID;						// ʦ��ID
	tagApprenticeData	m_ApprenticeData;					// ͽ������
	INT					m_nJingWuPoint;						// �������
	INT					m_nQinWuPoint;						// �������
	INT					m_nLevelUpBonus;					// ͽ����������
	INT					m_nRewardNum;						// ��ʦ��������
	TMap<DWORD, DWORD>	m_mapApprentice;					// ͽ��ID����
	tagClassMateData	m_ClassMateData;					// ͬ������
	Mutex				m_mutexMaster;						// ʦ����
	Mutex				m_mutexMate;						// ͬ����
	INT					m_nMARelation;						// ʦͽ������
	BYTE				m_byTeammatesNum;					// ������
	BYTE				m_byTeamFriendNum;					// �����еĺ�����

	//-------------------------------------------------------------------------------------
	// ��Ʒ��ȴ
	//-------------------------------------------------------------------------------------
	BOOL				m_bObjectCoolOff;					// �����ڵ���Ʒ�Ƿ���ȴ

	//-------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------
	BOOL				m_bSpeakOff;						// ����Ƿ񱻽���

	//-------------------------------------------------------------------------------------
	// ���Ե�ͼ��ܵ�
	//-------------------------------------------------------------------------------------
	INT					m_nAttPointAdd[X_ERA_ATTA_NUM];		// Ͷ�����һ�����Ե����Ե�
	tagRoleTalent		m_Talent[X_MAX_TALENT_PER_ROLE];	// ���ʼ��������ʵ�

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	DWORD				m_dwRebornMapID;					// �����ͼID

	//-------------------------------------------------------------------------------------
	// �������ܵĴ�����������
	//-------------------------------------------------------------------------------------
	std::set<DWORD>		m_setPassiveSkillTrigger[EPSAET_End];

	//-------------------------------------------------------------------------------------
	// װ���Ĵ�����������
	//-------------------------------------------------------------------------------------
	typedef std::bitset<X_EQUIP_BAR_SIZE>	BitSetEquipPos;		/*n16EquipPos*/
	typedef std::set<DWORD>					SetSuitTrigger;		/*dwSuitID*/

	BitSetEquipPos		m_bitsetEquipTrigger[EPSAET_End];
	BitSetEquipPos		m_bitsetLongHunTrigger[EPSAET_End];
	SetSuitTrigger		m_setSuitTrigger[EPSAET_End];
	// Jason 2010-5-18 v2.1.0-fix
	VOID CalFabaoMatchTriggerBuff(const tagFabao * pFabao,bool bEquip,INT nPos);
	VOID TriggerFabaoMatchBuff(const tagFabao * pFabao);

	//-------------------------------------------------------------------------------------
	// ��Ǯ
	//-------------------------------------------------------------------------------------
	CurrencyMgr			m_CurMgr;

	//-------------------------------------------------------------------------------------
	// ��Ʒ������ -- �������ҡ�����ҳ��װ��������ɫ�ֿ�Ͱٱ�����
	//-------------------------------------------------------------------------------------
	ItemMgr				m_ItemMgr;							// ��Ʒ������

	//-------------------------------------------------------------------------------------
	// Ԫ��
	//-------------------------------------------------------------------------------------
	HolySoul            m_HolySoul;

	//-------------------------------------------------------------------------------------
	// ��װ����
	//-------------------------------------------------------------------------------------
	Suit				m_Suit;

	//-------------------------------------------------------------------------------------
	// �����������
	//-------------------------------------------------------------------------------------
	LongHun				m_LongHun;

	//-------------------------------------------------------------------------------------
	// ��Ҽ佻�����
	//-------------------------------------------------------------------------------------
	ExchangeMgr			m_ExchMgr;

	//-------------------------------------------------------------------------------------
	// �����ں�
	//-------------------------------------------------------------------------------------
	bool				m_bIsInFairyFusion;
	INT64				m_n64FusionFairySerial;

	//-------------------------------------------------------------------------------------
	// ��̯
	//-------------------------------------------------------------------------------------
	Stall				*m_pStall;

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	tagFriend						m_Friend[MAX_FRIENDNUM];				// �����б�	
	TMap<DWORD, tagFriend*>			m_mapFriend;
	DWORD							m_dwBlackList[MAX_BLACKLIST];			// ������
	DWORD							m_dwEnemyList[MAX_ENEMYNUM];			// ���ӳ������

	//-------------------------------------------------------------------------------------
	// �Ŷ����
	//-------------------------------------------------------------------------------------
	DWORD				m_dwTeamID;											// С��ID
	DWORD				m_dwGroupID;										// �Ŷ�ID
	DWORD				m_dwTeamInvite;										// ������ID
	BOOL				m_bTeamSyn;											// С�����״̬ͬ����־

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	DWORD				m_dwOwnInstanceMapID;								// ����������ĸ����ĵ�ͼID
	DWORD				m_dwOwnInstanceID;									// ��Ҵ����ĸ���ID
	DWORD				m_dwExportMapID;									// ��������ʱ�ĵ�ͼID
	Vector3				m_vExport;											// ��������ʱ��ͼ������		

	//-------------------------------------------------------------------------------------
	// ��Զ����ҹ�����Ϣ����
	//-------------------------------------------------------------------------------------
	tagRemoteOpenSet	m_sRemoteOpenSet;

	//-------------------------------------------------------------------------------------
	// ��ҽű�
	//-------------------------------------------------------------------------------------
	const RoleScript*	m_pScript;											// ��ҽű�

	//-------------------------------------------------------------------------------------
	// ��ɫ�����������
	//-------------------------------------------------------------------------------------
	INT					m_nTreasureSum;										// ����ѿ����ı�����
	
	//-------------------------------------------------------------------------------------
	// ������״̬
	//-------------------------------------------------------------------------------------
	tagChestState		m_TreasureState;

	//-------------------------------------------------------------------------------------
	// �Ƿ���30���ѹر�pk����
	//-------------------------------------------------------------------------------------
// 	BOOL				m_bLevelUpClosedSafeGuard;

	//-------------------------------------------------------------------------------------
	// ʵ��ϵͳ�����
	//-------------------------------------------------------------------------------------
	tagStrength			m_Strength;											// ʵ��ֵ
	INT					m_nStrengthNum;										// ʵ��������ʵ�����а��չ���ʵ����
	INT					m_nEquipPm;											// װ��ʵ������
	DWORD				m_dwLatestOne;										// ��¼��������һ�λ�ɱ����ҵ�ID
	INT					m_nKillScore;										// ��¼�����һ��֮�ڻ�ɱ�ǰ�����һ�õĽ���ʵ��ֵ
	tagDWORDTime		m_dwLastClearKillScore;								// ��¼�ϴν�killscore����ʱ��

public:
	INT					GetPm(ERankingType);
	VOID				SetEquipPm(INT nPm)					{ m_nEquipPm = nPm;			}
	INT					GetEquipPm()						{ return m_nEquipPm;		}
	INT					GetStrengthPm()						{ return m_nStrengthNum;	}
	tagStrength			GetStrength()						{ return m_Strength;		}
	INT					GetTotalStrength()					{ return m_Strength.nAdvanceStrength + m_Strength.nBaseStrength; }
	
	INT					GetActivityStrength()				{ return GetScriptData(99);	}
	INT					GetQuestStrength()					{ return GetScriptData(100);}
	INT					GetKillStrength()					{ return GetScriptData(101);}
	INT					GetInstanceStrength()				{ return GetScriptData(95);	}

	VOID				SetAdvanceStrength(INT nValue)		{ m_Strength.nAdvanceStrength = nValue;		}
	VOID				SetConsumptiveStrength(INT nValue)	{ m_Strength.nConsumptiveStrength = nValue;	}
	VOID				ClearKillScore()					{ m_nKillScore = 0; m_dwLastClearKillScore = GetCurrentDWORDTime(); }
	BOOL				IsInBattleField(Role* pSrcRole, Role* pTargetRole);
	BOOL				IsEquipTimeEquipment();

protected:
	//-------------------------------------------------------------------------------------
	// ����ϵͳ
	//-------------------------------------------------------------------------------------
	INT64				m_dwFixSpiriteCDTime;								// ����ֵ�����ʣ��ʱ��
	DWORD				m_dwSpiUpdateTickCount;								// ���ڼ�ʱ��ÿ300��tick����һ��
	FLOAT				m_fSpiRate;											// ��ǰ����

protected:	
	//-------------------------------------------------------------------------------------
	// ��ְ��ѡ���
	//-------------------------------------------------------------------------------------
	DWORD				m_dwClergyMarsPoint;			// ս��ϵ��ֵ
	DWORD				m_dwClergyApolloPoint;			// ̫����ϵ��ֵ
	DWORD				m_dwClergyRabbiPoint;			// ��ϵ��ϵ��ֵ
	DWORD				m_dwClergyPeacePoint;			// ��ƽ֮��ϵ��ֵ
	ECampaignGodType	m_eClergyCandidate;				// ��ְ��ѡ����״̬
	ECandidateFlag		m_eClergy4SeniorOrJunior;		// ������ְ��ѡ�ʸ�
	DWORD				m_dwPrevClergy;					// ��һ�ڵ���ְ
	DWORD				m_dwClergy;						// ��ǰ����ְ

public:
	VOID				CalClergyPoint()				// ������ϵ��ֵ
	{
		//ս����
		//��ֵ=������2+����+��������3+��������+���������2+��������+��׼+����+����	
		//̫������
		//��ֵ=����+����+��������3+��������+���������2+��������+��׼��3+����+����	
		//��ʦ֮����
		//��ֵ=����+������2+������+����������3+�������+����������2+��׼+����+����	
		//��ƽ֮����
		//��ֵ=������2+����+������+����������2+�������+��������+��׼+����+������5
		m_dwClergyMarsPoint = m_nAtt[ERA_MaxHP]*2+m_nAtt[ERA_MaxMP]+m_nAtt[ERA_ExAttack]*3+m_nAtt[ERA_InAttack]+m_nAtt[ERA_ExDefense]*2+m_nAtt[ERA_InDefense]+m_nAtt[ERA_AttackTec]+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion];
		m_dwClergyApolloPoint = m_nAtt[ERA_MaxHP]+m_nAtt[ERA_MaxMP]+m_nAtt[ERA_ExAttack]*3+m_nAtt[ERA_InAttack]+m_nAtt[ERA_ExDefense]*2+m_nAtt[ERA_InDefense]+m_nAtt[ERA_AttackTec]*3+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion];
		m_dwClergyRabbiPoint = m_nAtt[ERA_MaxHP]+m_nAtt[ERA_MaxMP]*2+m_nAtt[ERA_ExAttack]+m_nAtt[ERA_InAttack]*3+m_nAtt[ERA_ExDefense]+m_nAtt[ERA_InDefense]*2+m_nAtt[ERA_AttackTec]+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion];
		m_dwClergyPeacePoint = m_nAtt[ERA_MaxHP]*2+m_nAtt[ERA_MaxMP]+m_nAtt[ERA_ExAttack]+m_nAtt[ERA_InAttack]*2+m_nAtt[ERA_ExDefense]+m_nAtt[ERA_InDefense]+m_nAtt[ERA_AttackTec]+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion]*5;
	
		if (!ECGT_INVALID(m_eClergyCandidate))
		{
			if(E_Success == CanGetClergy(m_eClergyCandidate, true))
				m_eClergy4SeniorOrJunior = ECF_4BossClergy;
			else if(E_Success == CanGetClergy(m_eClergyCandidate, false))
				m_eClergy4SeniorOrJunior = ECF_4BaseClergy;
			else
				m_eClergy4SeniorOrJunior = ECF_Null;
		}
			
	}
	
	ECampaignGodType	GetClergyCandidateType()
	{
		return m_eClergyCandidate;
	}

	DWORD				CanGetClergy(ECampaignGodType eType, bool bSeniorOrJunior = false/* true ���� false С��*/);
	VOID				SetClergyCandidateType(ECampaignGodType eType)
	{
		m_eClergyCandidate = eType;
	}

	VOID				ClergyBack()
	{
		const tagClergyProto *pPreClergyProto = g_attRes.GetClergyProto(m_dwPrevClergy);
	
		if (P_VALID(pPreClergyProto) &&
			(pPreClergyProto->eType == EGT_WarBoss || pPreClergyProto->eType == EGT_SunBoss ||
			pPreClergyProto->eType == EGT_ManaBoss || pPreClergyProto->eType == EGT_PeaceBoss || 
			pPreClergyProto->eType == EGT_War || pPreClergyProto->eType == EGT_Sun ||
			pPreClergyProto->eType == EGT_Mana || pPreClergyProto->eType == EGT_Peace))
		{
			m_dwPrevClergy = 0;
		}

		if( E_Success != PromotionClergy(m_dwPrevClergy, false))
		{
			m_dwClergy = 0;
		}
	
		//�㲥����Χ�Ľ�ɫ
		tagNS_ChangeClergy send1;
		send1.dwClergy = m_dwClergy;
		send1.dwRoleID = GetID();
		if( P_VALID(GetMap())) GetMap()->SendBigVisTileMsg(this, &send1, send1.dwSize);
	}

	VOID				LoginClergyVoteResultCheck();	// ��ɫ���ߵ�ʱ������ʱ��ѡ��ְ�ɹ�

	DWORD				GetClergy(){return m_dwClergy;}

public:
	//-------------------------------------------------------------------------------------
	// ��ʼ����ǰ����
	//-------------------------------------------------------------------------------------
	VOID InitCurrentQuest(const BYTE* &pData, INT32 nNum);

	//-------------------------------------------------------------------------------------
	// ��ʼ���������������
	//-------------------------------------------------------------------------------------
	VOID InitCompleteQuest(const BYTE* &pData, INT32 nNum);
    
	//-------------------------------------------------------------------------------------
	// ��ʼ�����������ÿ������
	//-------------------------------------------------------------------------------------
	VOID InitDailyCompleteQuest(const BYTE* &pData, INT32 nNum);
	//-------------------------------------------------------------------------------------
	// ͨ��NPC����ȡ����
	//-------------------------------------------------------------------------------------
	INT AcceptQuestFromNPC(UINT16 u16QuestID, DWORD dwNPCID);

	//-------------------------------------------------------------------------------------
	// ͨ����������ȡ����
	//-------------------------------------------------------------------------------------
	INT AcceptQuestFromMapTrigger(UINT16 u16QuestID, DWORD dwMapTriggerID);

	//-------------------------------------------------------------------------------------
	// �Ƿ���Խ�ȡ���񣬷��ز���Ϊ����ɲμӣ���Ӧ������
	//-------------------------------------------------------------------------------------
	INT CanAcceptQuest(UINT16 u16QuestID, INT& nIndex, Creature* pNPC=NULL);
	INT	CanAcceptBoardQuest(UINT16 u16QuestID, INT& nIndex, EQuestBoardType eQuestBoardType);
	//-------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------
	BOOL AddQuest(const tagQuestProto* pProto, INT nIndex);
	BOOL AddBoardQuest(const tagBoardQuestProto* pProto, INT nIndex, UINT16 u16ID);

	//-------------------------------------------------------------------------------------
	// �������
	//-------------------------------------------------------------------------------------
	INT CompleteQuest(UINT16 u16QuestID, DWORD dwNPCID, INT nChoiceItemIndex, EQuestComplteType eComplteType, UINT16& u16NexitQuestID );

	//-------------------------------------------------------------------------------------
	// �Ƿ���Խ�����
	//-------------------------------------------------------------------------------------
	INT CanCompleteQuest(Quest* pQuest, DWORD dwNPCID, INT nChoiceItemIndex);

	//-------------------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------------------
	VOID RewardQuest(Quest* pQuest, INT32 nChoiceItemIndex);
	VOID RewardBoardQuest(UINT16 u16QuestID, EQuestBoardType eQuestBoardType);
	//-------------------------------------------------------------------------------------
	// ɾ������
	//-------------------------------------------------------------------------------------
	INT DeleteQuest(UINT16 u16QuestID);

	//-------------------------------------------------------------------------------------
	// ɾ��һ������
	//-------------------------------------------------------------------------------------
	VOID RemoveQuest(UINT16 u16QuestID, BOOL bComplete);

	//-------------------------------------------------------------------------------------
	// ����NPC�Ի�����״̬
	//-------------------------------------------------------------------------------------
	VOID UpdateQuestNPCTalk(UINT16 u16QuestID, DWORD dwNPCID);

	//-------------------------------------------------------------------------------------
	// ����Trigger����״̬
	//-------------------------------------------------------------------------------------
	VOID UpdateQuestTrigger(UINT16 u16QuestID, DWORD dwTriggerID);

	//-------------------------------------------------------------------------------------
	// ���������¼�
	//-------------------------------------------------------------------------------------
	VOID OnQuestEvent(EQuestEvent eQuestType, DWORD dwEventMisc1=0, DWORD dwEventMisc2=0, DWORD dwEventMisc3=0);

	//-------------------------------------------------------------------------------------
	// ��������ɱ�������¼�
	//-------------------------------------------------------------------------------------
	VOID OnFamilyKillMonsterQuestEvent(DWORD dwMonsterID, INT nMonsterLevel, DWORD dwMapCrc);

	//------------------------------------------------------------------------------------
	// �õ��������ĸ���
	//------------------------------------------------------------------------------------
	INT GetCompleteQuestCount() { return m_mapCompleteQuests.Size(); }

	//------------------------------------------------------------------------------------
	// ����������������
	//------------------------------------------------------------------------------------
	VOID ClearCompleteQuest()
	{
		tagQuestDoneSave* pDoneQuest = NULL;
		QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();
		while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
		{
			// ��ͻ��˷�����Ϣ
			tagNS_GMQuestState send;
			send.u16QuestID = pDoneQuest->u16QuestID;
			send.bDone = FALSE;
			SendMessage(&send, send.dwSize);

			SAFE_DEL(pDoneQuest);
		}
		m_mapCompleteQuests.Clear();
	}
	//------------------------------------------------------------------------------------
	// ������������ÿ������
	//------------------------------------------------------------------------------------
	VOID ClearDailyCompleteQuest();

	//------------------------------------------------------------------------------------
	// ��ӻ�ɾ��һ�����������
	//------------------------------------------------------------------------------------
	VOID AddDelCompleteQuest(UINT16 u16QuestID, BOOL bAddOrDel)
	{
		// ��ָ������������������
		if( bAddOrDel )
		{
			if( m_mapCompleteQuests.IsExist(u16QuestID) )
				return;

			tagQuestDoneSave* pQuestDone = new tagQuestDoneSave;
			pQuestDone->u16QuestID = u16QuestID;
			pQuestDone->nTimes = 1;
			pQuestDone->dwStartTime = g_world.GetWorldTime();

			m_mapCompleteQuests.Add(u16QuestID, pQuestDone);
			if( !m_mapDailyCompleteQuests.IsExist(u16QuestID) )
			{
				tagQuestDoneSave* pQuestDoneTemp = new tagQuestDoneSave;
				pQuestDoneTemp->u16QuestID = u16QuestID;
				pQuestDoneTemp->nTimes = 1;
				pQuestDoneTemp->dwStartTime = g_world.GetWorldTime();
               m_mapDailyCompleteQuests.Add(u16QuestID, pQuestDoneTemp); 
			}
			// ��ͻ��˷�����Ϣ
			tagNS_GMQuestState send;
			send.u16QuestID = u16QuestID;
			send.bDone = TRUE;
			SendMessage(&send, send.dwSize);
		}
		else // ��ָ������Ϊδ�������
		{		
			tagQuestDoneSave* pQuestDone = m_mapCompleteQuests.Peek(u16QuestID);
			if( P_VALID(pQuestDone) )
			{
				m_mapCompleteQuests.Erase(u16QuestID);
				SAFE_DEL(pQuestDone);
			}

			tagQuestDoneSave* pQuestDoneTemp = m_mapDailyCompleteQuests.Peek(u16QuestID);
			if( P_VALID(pQuestDoneTemp) )
			{
				m_mapDailyCompleteQuests.Erase(u16QuestID);
				SAFE_DEL(pQuestDoneTemp);
			}

			// ��ͻ��˷�����Ϣ
			tagNS_GMQuestState send;
			send.u16QuestID = u16QuestID;
			send.bDone = FALSE;
			SendMessage(&send, send.dwSize);
		}
	}

	//------------------------------------------------------------------------------------
	// �õ��������ĸ���
	//------------------------------------------------------------------------------------
	INT GetCurrentQuestCount() { return m_mapCurrentQuests.Size(); }

	//-------------------------------------------------------------------------------------
	// �õ�ָ��������ɵĴ���
	//-------------------------------------------------------------------------------------
	INT GetQuestCompleteTimes(UINT16 u16QuestID)
	{
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) ) return GT_INVALID;

		return pDoneQuest->nTimes;
	}

	//-------------------------------------------------------------------------------------
	// �õ�ָ��������ɵĽ�ȡʱ��
	//-------------------------------------------------------------------------------------
	DWORD GetQuestCompleteAcceptTime(UINT16 u16QuestID)
	{
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) ) return GT_INVALID;

		return pDoneQuest->dwStartTime;
	}

	//-------------------------------------------------------------------------------------
	// �õ�ָ������
	//-------------------------------------------------------------------------------------
	Quest* GetQuest(UINT16 u16QuestID) { return m_mapCurrentQuests.Peek(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// �õ�ָ�������Ƿ����
	//-------------------------------------------------------------------------------------
	BOOL IsHaveQuest(UINT16 u16QuestID) { return m_mapCurrentQuests.IsExist(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// ����Ƿ�����ĳ����
	//-------------------------------------------------------------------------------------
	BOOL IsHaveDoneQuest(UINT16 u16QuestID) { return m_mapCompleteQuests.IsExist(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// �õ����Ѹ���
	//-------------------------------------------------------------------------------------
	INT  GetFriendCount() { return m_mapFriend.Size(); }

	//-------------------------------------------------------------------------------------
	// �������ɿضԻ���ȱʡ�¼�
	//-------------------------------------------------------------------------------------
	VOID OnDlgDefaultEvent(EMsgUnitType	eDlgTarget, DWORD dwTargetID, EDlgOption eDlgOption);

	//-------------------------------------------------------------------------------------
	// �����ý�ɫ�Ķ������ϵĴ�������
	//-------------------------------------------------------------------------------------
	VOID OnChuanDaoQuest(EChuanDaoType eType);

	//-------------------------------------------------------------------------------------
	// �����Ƿ��д�������
	//-------------------------------------------------------------------------------------
	UINT16 IsHaveChuanDaoQuest(INT nLevel)
	{
		Quest* pQuest = NULL;
		QuestMap::TMapIterator it = m_mapCurrentQuests.Begin();
		while (m_mapCurrentQuests.PeekNext(it, pQuest))
		{
			if (!P_VALID(pQuest))
			{
				continue;
			}

			if (!P_VALID (pQuest->GetProto()->chuandao_count))
			{
				continue;
			}

			if(nLevel<pQuest->GetProto()->chuandao_junior_min_level || nLevel>pQuest->GetProto()->chuandao_junior_max_level)
			{
				continue;
			}
			
			if(pQuest->GetChuanDaoCount() >= pQuest->GetProto()->chuandao_count)
			{
				continue;
			}

			return pQuest->GetID();
		}

		return 0;
	}

public:

	// ��������״̬
	VOID SaveTrainState(DWORD dwTrainID, ETrainState eTrainState);

	// ȡ����״̬
	ETrainState GetTrainState(DWORD dwTrainID);

	// ��ȡһ����������ɶ�
	DWORD GetTrialCompleteNum(DWORD dwTrainID);

	// ����һ���ɾ͵���ɶ�
	VOID SetTrainCompleteness(DWORD dwTrainID, DWORD dwCompleteness);

	// ��һ���ɾ�����Ϊ�����״̬
	VOID CompleteTrain(DWORD dwTrainID);
	
	// ������������
	DWORD GiveTrainReward(DWORD dwTrainID);

	// ȡ�����������
	INT GetPassTrainCnt();

	// ���ͻ��˷�������״̬��
	ETrainErrCode SendTrainStateList2Client();

private:

	// ����������Ʒ
	DWORD CreateRewardItem(TList<tagItem *>& pListRewardItem, tagItemReward* pItemReward, INT16* nLapNum);

	// ��ʼ������״̬��
	VOID InitTrainState(const BYTE* &pData, INT32 nNum);
	VOID DelTrainStateMap()
	{
		tagTrainState* pTrainState = NULL;
		TrainStateMap::TMapIterator it = m_mapTrainStates.Begin();
		while( m_mapTrainStates.PeekNext(it, pTrainState) )
		{
			SAFE_DEL(pTrainState);
		}
		m_mapTrainStates.Clear();
	}

private:
	Quest			m_Quests[QUEST_MAX_COUNT];		// ��ҵ�ǰ������
public:
	BOOL			QuestValid(INT nIndex){return m_Quests[nIndex].IsValid();}
private:
	QuestMap		m_mapCurrentQuests;				// ��ҵ�ǰ������
	QuestDoneMap	m_mapCompleteQuests;			// �Ѿ���ɵ����񼯺�
	QuestDoneMap    m_mapDailyCompleteQuests;       // ÿ����ɵ��������
    BOOL          	bIsDailyQuestCleared;           // ÿ���Ƿ��Ѿ�����չ�
	TrainStateMap	m_mapTrainStates;				// �����������״̬��
	BYTE			m_byTrainStateTransfered;

#ifdef ON_INTERNET_CAFE
	DWORD			m_dwInternetCafePingTime;
	BYTE			m_byInternetCafePingLostTime;
	BOOL				m_bTimeout;					// ���ɽ�������

#endif
// ��������
private:
	BoardQuestMgr*	m_pBoardQuestMgr;			// ���������
	BoardQuestMgr*	m_pYellowListMgr;				// ��֮�л������
	UINT16				m_n16YellowListAcceptTimes;		// ��ȡ��֮�л�������������

public:
// �������غ���
	BoardQuestMgr*	GetBoardQuestMgr(EQuestBoardType eQuestBoardType)
	{
		if (eQuestBoardType == EQBT_Reward)
			return m_pBoardQuestMgr;
		else if (eQuestBoardType == EQBT_YellowList)
			return m_pYellowListMgr;
		else
			return NULL;
	}
	EQuestBoardType GetQuestBoardType(UINT16 u16QuestID)
	{
		if (IsRewardBoardQuest(u16QuestID))
			return EQBT_Reward;
		else if (IsYellowListQuest(u16QuestID))
			return EQBT_YellowList;
		else
			return EQBT_Null;
	}
	UINT16 GetYellowListAcceptTimes(){ return m_n16YellowListAcceptTimes; }
	VOID IncrYellowListAcceptTimes()	{ SetScriptData(TodayYellowListQuestTimesIdx, ++m_n16YellowListAcceptTimes); }
	VOID ResetYellowListAcceptTimes() { m_n16YellowListAcceptTimes = 0; SetScriptData(TodayYellowListQuestTimesIdx, 0); }

// �ƺ����
public:
	TitleMgr*		GetTitleMgr() { return m_pTitleMgr; }
private:
	TitleMgr*		m_pTitleMgr;
//��ɫ�������
public:
	tagRoleVCard&	GetVCard() { return m_VCard; }
private:
	tagRoleVCard	m_VCard;
//������Ϣ
public:
	ClanData& GetClanData()	{ return m_ClanData;}
private:
	ClanData		m_ClanData;
// �����
public:
	PetPocket*	GetPetPocket() { return m_pPetPocket; }
private:
	PetPocket*	m_pPetPocket;
public:
	VOID OnLeaveMap(BOOL bChgMap = TRUE);

	void BreakMount();
	VOID OnEnterMap();
public:
	// ���纰����������ʱ��
	BOOL TalkToWorld();
private:
	INT m_nWorldTalkCounter;
	VOID UpdateTalkToWorld();

public:
	// ��������
	BOOL TalkToScene();
private:
	INT m_nSceneTalkCounter;
	VOID UpDateTalkToScene();
	//Ⱥ�� 
public:
	VOID   AddGroupChat(DWORD dwGroupChatId,group_chat* pGroupChat) { m_mapGroupChat.Add(dwGroupChatId,pGroupChat); }
	VOID   DeleteGroupChat(DWORD dwGroupChatId)                     { m_mapGroupChat.Erase(dwGroupChatId); }
	VOID   ClearGroupChat();
private:
	TSafeMap<DWORD,group_chat*>      m_mapGroupChat;
public:
	DWORD CanPickUpItem(INT64 n64GroundID, BOOL bRole = TRUE);
	// ʰȡ��Ʒ
	DWORD PickUpItem(INT64 n64GroundID, DWORD& dwTypeID, BOOL bRole = TRUE);
	// ������Ʒ
	DWORD PutDownItem(INT64 n64Serial, INT64 &n64GroupID, BOOL bAdd2Ground = FALSE);

	// ����ս�������Ʒ
	DWORD PutDownWarItem(INT64 n64Serial, INT64 &n64GroupID, BOOL bAdd2Ground = FALSE);

	DWORD LootItem(INT64 n64Serial, INT64 &n64GroupID);
public:
	// �������״̬
	VOID SetMount(BOOL bSet, INT nMountSpeed, const tagPetProto* pPetProto);

	// ��������ٶ�
	VOID ModMountSpeed(INT nMod);

	// �������
	VOID SetPassenger(Role* pHost);

	typedef MoveData::EMoveRet (MoveData::*PFMoveAction2P)(const Vector3& v1, const Vector3& v2);
	typedef MoveData::EMoveRet (MoveData::*PFMoveAction1P)(const Vector3& v);

	MoveData::EMoveRet	MoveAction(PFMoveAction2P pAction, Vector3& v1, Vector3& v2);
	MoveData::EMoveRet	MoveAction(PFMoveAction1P pAction, Vector3& v);

	//------------------------------------------------------------------------------
	// ��Ҽ���ｻ�����
	//------------------------------------------------------------------------------
	PetExchangeMgr& GetPetExchMgr() { return m_PetExchMgr; }

	BOOL	IsPetExchanging()					{ return IsInRoleState(ERS_PetExchange); }
	BOOL	CanPetExchange()
	{
		return !IsInRoleStateAny(ERS_Exchange | ERS_Shop | ERS_Stall | ERS_PetExchange) 
			&& (GetPetExchMgr().GetTgtRoleID() == GT_INVALID);
	}

	VOID	BeginPetExchange(DWORD dwTgtRoleID)
	{
		SetRoleState(ERS_PetExchange);
		GetPetExchMgr().CreateData();
		GetPetExchMgr().SetTgtRoleID(dwTgtRoleID);
	}

	VOID	EndPetExchange()
	{
		UnsetRoleState(ERS_PetExchange);
		GetPetExchMgr().DeleteData();
		GetPetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	DWORD	ProcPetExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD	ProcPetExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode);
	DWORD ProcPetExchangeAdd(OUT Role* &pTarget, DWORD dwPetID);
	DWORD ProcPetExchangeDec(OUT Role* &pTarget, DWORD dwPetID);
	DWORD	ProcPetExchangeMoney(OUT Role* &pTarget, INT64 n64Silver);
	DWORD	ProcPetExchangeLock(OUT Role* &pTarget);
	DWORD	ProcPetExchangeCancel(OUT Role* &pTarget);
	DWORD	ProcPetExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID);

	DWORD VerifyPetExchangeData();
	DWORD	ProcPetExchange();

	DWORD	ProcPreparePetExchange(OUT Role* &pTarget);
	public:
	VOID     ClenupPetExchState();
	private:

private:
	PetExchangeMgr m_PetExchMgr;
	//	��ɫ��������������
public:

	//	��ȡ��ɫ�����������
	INT GetTreasureSum()		{ return m_nTreasureSum; }
	//	���ӱ������
	VOID IncTreasureSum();
	//	��ʼ���������
	VOID InitChestState();
	//	���ñ��俪����
	VOID SetTreasureSum(INT nSum);

	VOID StopMount();
	VOID ContinueMount();

public:
	// �������ɾ���ӳ�
	FLOAT	GetVNBExpRate();
	// �������ɵ��ʼӳ�
	FLOAT	GetVNBLootRate();
	
public:
	// GM������õļ�������
	DWORD AddSuit(DWORD dwSuitID, INT nQlty);
	DWORD AddEquip(INT nType, INT nLevel, INT nQlty);
public:
	//�����Ƿ���������Ϣ
	VOID	ResetHasLeftMsg()
	{
		m_bHasLeftMsg = FALSE;
	}
private:
	BOOL	m_bHasLeftMsg;
public:	
	// �Ƿ���Բɼ�
	INT CanGather(Creature* pRes);

public:
	//�Ƿ���Բ��Ÿ��Զ���
	DWORD	CanCastMotion(Unit* pDest, DWORD dwActionID);
	//�Ƿ���Բ��ŵ��˸��Ի�����
	DWORD	CanCastMotion(DWORD dwActionID);
	// ���Ÿ��Զ���
	DWORD	CastMotion(Unit* pDest, DWORD dwActionID);
public:	
	// ���������Ϣ	
	tagRevive& GetRevive() 	{ return m_Revive; }
	// �ж�dwRoleID��û��Ȩ�����н���
	BOOL	GetMotionInviteStatus(DWORD dwRoleID);
	// ����Ƿ���Խ��ܻ���������
	VOID	SetMotionInviteStatus(BOOL bSet, DWORD dwRoleID);
	// ����״̬
	VOID	UpdateMotionInviteState();
private:
	INT		m_nMotionCounter;
	DWORD	m_dwPartnerID;
public:
	//���澭�鵽db
	VOID	SaveExp2DB();
	//�������Ե㵽db
	VOID SaveAttPoint2DB();
	//�������ʵ㵽db
	VOID	SaveTalentPoint2DB(INT nIndex);
	//����ȼ���db
	VOID	SaveLevel2DB();
	//�������ֵ�db
	VOID	SaveNewName2DB();

private:
	//̨���������
	OrangeVipNetBarMgr m_clsOrangeVNBMgr;				// ̨��������ɹ���

	// Jason 2009-12-7 �����͸�
	VOID	UpdateBlessingOfLoong(VOID);

	// Jason 2009-12-7 �����͸�
	DWORD	m_timeLastLessing;							// �ϴ������͸�ʱ�䣬ע�⣬��ֵ��������ʱ�䵥λһ�£���λ��
	INT				m_nLessingLevel;					// �͸��������ROLE_MAX_LESSLEVEL_OF_LOONG��
	INT				m_nLessingLevelEx;					// �����͸���չ����Ҳ����8�κ�������͸���
	tagDWORDTime			m_dateLastLessingLoong;		// �����Ϊ�������߽����ĵı�־��

	//==============================================================================================================================
	// Jason 2010-1-7 v1.3.2
	DWORD					m_dwCurrentDayOnlineTime;	// �����ۼ�����ʱ��
	tagDWORDTime		m_timeOneDayFirstLogined;		// �ϴε�¼������(�״ε�½)��ע�⣬����ֶ���Ҫ�����ж��Ƿ���죬��ˣ�����ʱ���е������Ȳ�������Ҫ��ֻҪ�ܱ�ʾĳ�켴��
	tagDWORDTime		m_timeOnlineRewardPerDayRec;	// ������ȡ���߽�����ʱ��
	tagDWORDTime		m_timeLastCalOnlineTime;		// �ϴμ���ʱ��
	//--------------------------------------------------------------------------------------------------------------------------------
	// ����Ĳ�Ҫ���浽���ݿ�
	INT		m_n32UpdateOnlineReward;		// ���߸���ʱ�䣬��Ҫ���浽���ݿ�
	BOOL	m_bOnlineRewardPromptFlag;
	INT		m_nSelfShape;					// �����С��˫�����ʱ�������뷽�豣��ԭ�д�С
	INT		m_nMountShape;					// ���ʱ���仯��Ĵ�С
	INT		m_nLessingExCounter;
	//==============================================================================================================================

	BOOL IsValidOfLessingLoong(VOID);

public:
	VOID NotifyClientToReceiveLessingOfLoong(VOID); // ֪ͨ�������һ��ȡ�����͸�
	VOID NotifyClientIniLessingOfLoong(VOID);		// ֪ͨ��������͸��������ͻ�����ع��ܽ���

	DWORD ReceiveLessingOfLoong(VOID);				// ��ȡ�����͸�������

	// ����������ص����ۼƵ�������
	DWORD GetCurrentOnlineTime(VOID);
	DWORD ProcessGoldStoneMsg( INT64 GoldStoneItemID, INT64 destItemID);

	//==========================================================================================================
	// �������
	//==========================================================================================================
	static const INT      MAX_VALIANT_MERIT =  10;
	INT16                 m_nValiantMeritLevel;  //��ѫ�ȼ�
    //������ѫֵ����ҵ���ѫ�ȼ�
	INT16 GetValiantMeritLevel(INT16 nValiantMeritVal);


	//==========================================================================================================
	// ������������ݿ�
	//==========================================================================================================
private:
	static const int			BAR_COUNT = 10;					//�����������
	static const int			BAR_BUTTON_COUNT = 8;			//ÿ���ݼ�����
	tagQuickData                m_ShortCutBarData[BAR_COUNT][BAR_BUTTON_COUNT]; //���������
    tagQuickBarState            m_QuickBarStateFlags[BAR_COUNT][BAR_BUTTON_COUNT]; //�����Ҫ����Ŀ�������� 
	INT16                       nIsSavedQuickBarData;      //��������ݿ�δ�������ҽ���֪ͨ
	BOOL                        m_bClientReqShortCutBarData;
	BOOL                        m_bLoadFromDBFlag;
	BOOL                        m_bIsSended;
public:
    VOID SetShortCutBarData(INT nLnCount,INT nCol,const tagQuickData& QuickBarData);
	VOID SetShortCutBarDataFlag(INT nLnCount,INT nCol,const tagQuickData& QuickBarData);
    tagQuickData& GetShortCutBarData(INT nLnCount,INT nCol);
	VOID SendShortCutBarDataToClient();
	BOOL IsNeedSave();
	//����������db
	VOID	SaveQuickBarData2DB();  
    VOID    LoadQuickBarData2DB();
	VOID    SetIsSavedQuickBarData(INT16 nIsSaved);
	INT16    GetIsSavedQuickBarData()const;
	VOID    SendHaveNotSavedQuickBarMsg(); 
	VOID    SetShortCutBarReqFlag(BOOL bReqShortCutBarData)  { m_bClientReqShortCutBarData = bReqShortCutBarData ;}
	BOOL    GetShortCutBarReqFlag()  {   return m_bClientReqShortCutBarData;}
	VOID    SetLoadFromDBFlag(BOOL bLoadFromDBFlag)  { m_bLoadFromDBFlag = bLoadFromDBFlag;}
	BOOL    GetLoadFromDBFlag()  {  return m_bLoadFromDBFlag ; }

	//==========================================================================================================
	// Jason 2010-1-15 v1.3.2 �Ƿ������ѻ��ң���״̬Ӧ�ñ��浽���ݿ�
	BOOL IsOfflineHangOpen(VOID);
	//
	BOOL GetOfflineRewardParam(DWORD & hour,DWORD & minute);
	// Jason 2010-1-30 v.1.3.2 ��������������ÿ��ֻ��1��
	BOOL IncFollowPetPocketValve();
	BOOL IncRidingPetPocketValve();
    BOOL IncPetPocketValve(); //�ܵĸ���
    inline UINT16 FollowPetPocketValue()const
    {
        return  m_u16FollowPetPocketValve;
	}
	inline UINT16 RidingPetPocketValue()const
	{
        return  m_u16RidingPetPocketValve;
	}
	inline UINT16 PetPocketValve() const
	{
		return  m_u16PetPocketValve;
	}
	DWORD GetOfflineTime();
private:
	// Jason 2010-1-15 v1.3.2 ���߹һ�����
	BOOL	m_bIsOpenOfflineReward;
	tagDWORDTime		m_iniLogoutTime;						// �ǳ�ʱ��

	// Jason 2010-1-30 v.1.3.2 ��ǰ���������������һ��ҪС�������ļ��еķ�ֵ�����������ֻ��ȡ����ֵ
	UINT16		m_u16PetPocketValve; //�������������
	UINT16      m_u16FollowPetPocketValve;  //�����������
	UINT16      m_u16RidingPetPocketValve;  //��������
    UINT16      m_u16FollowPetCount;        //������������������
    UINT16      m_u16RidingPetCount;        //�������

private:
	// ʵ��ϵͳ���
	VOID	UpdateEquipValue();				// ����װ����ʵ��ֵ
	VOID	UpdateBaseStrength();			// �������ʵ��ֵ
	//VOID	UpdateConsumptiveStrength();	// ���¿�����ʵ��ֵ
	VOID	UpdateAdvanceStrength();		// ���½���ʵ��ֵ
	VOID	UpdateStrength();				// ������ʵ��ֵ
	VOID	CheckRoleAdvanceStrength();		// ��ɫÿ������ʱ���һ���Ƿ���Ҫ������ʵ��ֵ����
	VOID	UpdateYaoJingValue();				// ����������ʵ��ֵ

	// Jason 1.3.2 2010-1-7 ���㵱������ʱ��
	DWORD CalTodayOnlineTime(BOOL notify = FALSE);
	DWORD CalTodayCurrentTime();
	tagDWORDTime m_timeCurrentDayTime;
	//INT					m_nCurrentDayOnlineTime;						// �ۼƵ�������ʱ�䣨�룩

private:
	//�ɽ缼��
	VOID UpdateMiracleSkill();

	//�����������
	DWORD CheckPracticeMiracleSkillLimit(DWORD dwSkillTypeID);

	//�ɽ缼������
	DWORD DoPracticeMiracleSkillCost(DWORD dwSkillTypeID);

private:
	tagDWORDTime	m_dwLastClearAdvenceStrength;
	int				m_nGodMiraclePoints;
public:
	VOID	ClearAdvanceStrength();

public:
	BOOL	CanGetTeamProfit();
	VOID	GetTeamProfit();
	VOID	CancelTeamProfit(bool bLauncheredByDeleteMember = false);
	VOID	UpdateTeamProfit();

	VOID	GodMiraclePoints(INT points);
	INT		GetGodMiraclePoints(VOID);

	// Jason 2010-4-15 v2.0.0 ��������
	DWORD	PracticeFabao(INT64 srcItemID,INT num,INT64 fabaoID,tagFabaoSpec & fabaoSpec);
	DWORD	InlayFabao(INT64 n64FabaoID,INT num,INT64 * pBeadIDs,tagFabaoSpec & fabaoSpec);
	DWORD	ReverseFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec);
	DWORD	MergeFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec);
	
public:
	// ���ͻ��˷��ͳ�ս����ս����Ϣ
	DWORD SendCityStruggleRecord();

	// ���ͻ���ͬ����������������ͬ��������������ͬ����������
	VOID SyncOnlineNum2Role(ESyncOnlineNumType eSyncOnlineNumType, UINT16 nOnlineNum);

	// ͬ���ͻ�����ҵ������� [4/27/2012 zhangzhihua]
	VOID SyncRoleAttr(ERoleAttrSync eAttr, EMessageRange eRange = EMR_Self);

	// ȡ���ߺ�������
	UINT16 GetOnlineFriendCnt();

	// ȡͬ����������
	UINT16 GetOnlineClassmatesCnt();

	// ��ͽ��ͬ��ͬ�������仯
	VOID SyncOnlineNum2Apprentices(BOOL bLogout = FALSE);

	// ��ͬ��ͬ��ͬ�������仯
	VOID SyncOnlineNum2Classmates(BOOL bLogout = FALSE);

	// ��ʦ��ͬ��ͬ��ͬ�������仯
	VOID SyncOnlineNum2MasterApprenticesClassmates(BOOL bLogout = FALSE);

public:
	VOID  SetCurScriptArea(DWORD dwCurScriptArea)	{ m_dwCurScriptArea = dwCurScriptArea; }
	DWORD GetCurScriptArea()						{ return m_dwCurScriptArea;				}
	BOOL  IsInFixArea(DWORD dwAreaID);

private:
	DWORD		m_dwCurScriptArea;		// ��ǰ������ڵĽű�����
	INT			m_nSoaringSkillLearnTimes;	// �������ѧϰ����
	//tagDWORDTime m_timeLearnSoaringSkill;	// ѧϰ�������ܵ�ʱ��
	static CONST INT	m_nCLearnSoaringSkillTotalTimes = 100;

	struct tagAllSpeed
	{
		BOOL	bSetted;
		FLOAT						m_fXZSpeed;							// ��ǰXZ�����ٶ�
		FLOAT						m_fYSpeed;							// ��ǰY�����ٶ�
		FLOAT						m_fSwimXZSpeed;						// ��Ӿ��XZ�����ٶ�
		FLOAT						m_fJumpXZSpeed;						// ��Ծ��XZ�����ٶȣ�����Ծ��ʼʱ��ֵ����Ծ�����в��ı�
		FLOAT						m_fJumpYSpeed;						// ��Ծ��Y�����ٶȣ�����Ծ��ʼʱ��ֵ������Ծ�����в��ı�
		FLOAT						m_fDropXZSpeed;						// ����ʱ��XZ�����ٶ�
		FLOAT						m_fSlideSpeed;						// �����ٶ�
		FLOAT						m_fMountXZSpeed;					// ��ǰ���XZ�����ٶ�
	}	m_ownerSpeeds;

	// ����ṹ��Ҫ������װ������ֱ��ǿ���Ĺ��ܣ���ֹӰ�����ֵ��װ�����ڴ���Ч��ʱ�����µ�ǰֵ
	// �����ط�����ֹ���ʹ��
	struct tagAffectedByThresholdValue
	{
		INT nHP;
		INT nMP;
		INT nEndurance;
		INT nVitality;
	} m_stuValueBeforeStrengthen;
	VOID SaveTheValueBeforeStrengthen(VOID);
	VOID RestoreTheValueAfterStrengthen(VOID);

	// Jason 2010-5-24 v2.1.2
	typedef DWORD (Role::*TSpecSkillUsedFunc)(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
	typedef struct 
	{
		DWORD dwSkillID;
		DWORD dwNPCID;
		INT64 n64ItemID;
		TSpecSkillUsedFunc checkfunc;
		TSpecSkillUsedFunc learnfunc;
		TSpecSkillUsedFunc calcostfunc;
	} tagSpecSkillUsedParam,*PSpecSkillUsedParam;
	DWORD LearnSpecSkillLimit(PSpecSkillUsedParam param);

	DWORD CheckSoaringSkill(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD CalSoaringSkillCost(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD LearnSoaringSkillSpec(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
public:
	VOID	SynetRiderSpeed(Role*pRider);
	VOID	RestorOwnerSpeed(VOID);

	DWORD LearnSoaringSkill(DWORD dwSkillID);
	INT GetPracticeSoaringSkillTimes() const {return m_nSoaringSkillLearnTimes;}

	VOID OnReadyToSoaring(VOID);

	VOID OnCompletePracticeEquipment(DWORD dwEquipTypeID);
	VOID OnCompleteCastingEquipment(DWORD dwEquipTypeID,INT nCastingLevel);

	VOID SynRemoteBodyFabaoInfo(tagRemoteRoleFabaoInfo & info);

	VOID SetSoaringAtt(INT v);

// ��ɫս���һ�ϵͳ״̬��ʼ��
public:
	VOID SetAutoFightState(EAutoFightState eAutoFightState)
	{
		if (eAutoFightState == EAFS_Close ||	// û�С�ս����������״̬
			eAutoFightState == EAFS_Start ||	// ��ʼ�һ�
			eAutoFightState == EAFS_End)		// δ��ʼ
		{
			m_eAutoFightState = eAutoFightState;
			if (eAutoFightState == EAFS_Start)
				TryAddBuffToMyself(AUTO_FIGHT_BUFF_ID);
			else
				RemoveBuff(Buff::GetIDFromTypeID(AUTO_FIGHT_BUFF_ID), TRUE);
		}
	}
	EAutoFightState GetAutoFightState()
	{
		return m_eAutoFightState;
	}

	boolean CanAutoFight()
	{
		if (m_eAutoFightState == EAFS_Start &&
			GetItemMgr().GetBag().GetZhanDouFuCnt() > 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	INT64 GetCurZhanDouFu64ID() { return m_n64CurZhanDouFu; }
	VOID SetCurZhanDouFu64ID(INT64 n64CurZhanDouFu) { m_n64CurZhanDouFu = n64CurZhanDouFu; }
private:
	EAutoFightState m_eAutoFightState;		// ��ɫ��ս���һ�ϵͳ״̬
	INT64 m_n64CurZhanDouFu;					// ��ɫ��ǰʹ�õ�ս����64λID

	// Jason 2010-6-12 v1.0.0װ��ǿ������
	INT		m_nConsolidateTimes;

	set<DWORD> m_AttackSet;		// �����б�pkԤ��ʹ�ã�Jason 2010-7-17 v1.0.0
	set<DWORD> m_DefenseSet;		// �����б�pkԤ��ʹ�ã�Jason 2010-7-17 v1.0.0
public:
	INT		GetConsolidateTimes() const;
	VOID	IncConsolidateTimes(INT add = 1);

	//--------------------------------------------------------------------------------------------
	// �ӽ�ɫװ����ɾ��װ��
	//--------------------------------------------------------------------------------------------
	DWORD DeleteEquipFromEquipBar(DWORD dwTypeID, INT nCount, DWORD dwCmdID);

	// Jason 2010-7-6 v1.0.0
	static VOID OnScriptNoticeOfFabaoAttChange(Role*pRole,tagFabao * pFabao,INT nEventType);
	enum {EFABAO_ChangePractice_Success = 0,EFABAO_ChangePractice_Failse,EFABAO_ChangePractice_AlreadyFull,EFABAO_ChangePractice_RoleLevelLower};
	static INT ChangeFabaoPracticeValue( Role * pRole,tagFabao * pFabao,INT addvalue,INT & nChanged );

	// �ѱ����Ķ����������ٵ��ã�Jason 2010-7-6 v1.0.0
	BOOL ModWoreFabao(INT nAddValue,INT & nChanged );

	BOOL InSamePKState(Role * pRole);
	BOOL InSamePKStateEx(Role * pRole);

	enum {
		EAD_NULL		= 0x00,
		EAD_Attack		= 0x01,
		EAD_Defense	= 0x02
	};
	// Jason 2010-7-17 v1.0.0
	// ����ĳ����ɫ�Ǳ��Լ������ģ����Ƿ�����
	DWORD TestAttackDefenseRelationship(DWORD dwRoleID);
	// ��չ���������Ϣ��������һ����������ս��һ�����ã�
	VOID ClearAttackDefenseRelationship(VOID);
	// ������˴��Լ��Ĺ�������������ɾ�������Է�����ս����������ʱ����
	VOID RemoveAttackDefenseRelationship(DWORD dwRoleID);
	VOID RemoveAttackDefenseRelationshipWithMe();
	// ���ӹ�����
	VOID AddAttackRelationship(DWORD dwRoleID);
	// ���ӷ�����
	VOID AddDefenseRelationship(DWORD dwRoleID);

	// �ж��Ƿ���Teamʰȡ��Χ��
	BOOL IsInTeamPickupRank(DWORD dwMapID, const Vector3& vPos);

	// ������������ֳ�
	VOID ExpChangeWithFabaoExp(INT64 nValue, BOOL bKill=FALSE, BOOL bForce=FALSE, INT nFabaoChanged = 0);

public:
	VOID TryAddBuffToMyself(DWORD dwBuffID);

private:
	VOID KillCountInc(ERoleKillType eRoleKillType);

	DWORD m_dwMonsterKillCount;	// ��ɱ�������
	DWORD m_dwRoleKillCount;	// ��ɱ��ɫ����

private:

	// ��ɫ��ǰ������ʱ��(����ʱ����λΪ����)
	tagMonthOnlineTime m_CurMonthOnlineTime;

	VOID InitCurMonthOnlineTime(const BYTE* &pData, INT nNum);
	VOID UpdateCurMonthOnlineTime(BOOL bForceSave = FALSE);
public:
	// �������ϵ�����
	VOID SaveRoleFabaoChange(VOID);
public:
	INT GetWearyUpperLimit()
	{
		INT nWearyUpperLimit = 100;
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Silver: // ���꼶vip
			nWearyUpperLimit = 120;
			break;
		case EVL_Gold: // ���꼶vip
			nWearyUpperLimit = 150;
			break;
		case EVL_Platinum: // �׽�vip
			nWearyUpperLimit = 200;
			break;
		case EVL_None: // ����vip
		default:
			break;
		}
		return nWearyUpperLimit;
	}
	UINT16 GetMaxFreeWorldTalkTimes()
	{
		UINT16 n16MaxFreeWorldTalkTimes = 0;
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Silver: // ���꼶vip
			n16MaxFreeWorldTalkTimes = 50;
			break;
		case EVL_Gold: // ���꼶vip
			n16MaxFreeWorldTalkTimes = 80;
			break;
		case EVL_Platinum: // �׽�vip
			n16MaxFreeWorldTalkTimes = 100;
			break;
		case EVL_None: // ����vip
		default:
			break;
		}
		return n16MaxFreeWorldTalkTimes;
	}
	INT GetInjuryRateDerate()
	{
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Gold: // ���꼶vip
			return 25;
		case EVL_Platinum: // �׽�vip
			return 50;
		case EVL_Silver: // ���꼶vip
		case EVL_None: // ����vip
		default:
			return 0;
		}
	}

	INT GetPlayActFriendLimit()
	{
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Gold: // ���꼶vip
			return 6;
		case EVL_Platinum: // �׽�vip
			return 8;
		case EVL_Silver: // ���꼶vip
			return 4;
		case EVL_None: // ����vip
		default:
			return 2;
		}
	}

	UINT16 GetFreeWorldTalkTimes()
	{
		return m_n16FreeWorldTalkTimes;
	}
	VOID SetFreeWorldTalkTimes(UINT16 n16FreeWorldTalkTimes)
	{
		m_n16FreeWorldTalkTimes = n16FreeWorldTalkTimes;
	}

	VOID UpdateVipLevel();
	VOID UseVipCard(INT64 n64ItemID);

	VOID CheckVipStateWhenLogin();
	VOID ResetVipState();
	VOID AddVipBuff(EVipLevel eVipLevel);

	// ����������ظ���
	VOID UpdateFabaoMood(VOID);
	// �������齵�Ͷ���
	INT	 ModFabaoMood(INT v);

	// ʥ��������ظ��£����ٻ�ʥ��ʱ��ɫʥ��ֵ����
	VOID UpdateHolyData(VOID);

private:
	DWORD m_dwVipMaxDays;						// Vip����ʱ��(��λΪ��)
	tagDWORDTime m_dwVipStartTime;	// Vip����ʹ��ʱ�䣬û��ʹ�ù������Ѿ�����Ϊ0
	UINT16 m_n16FreeWorldTalkTimes;	// Vip��ɫ�Ѿ�ʹ�õ�������纰���Ĵ���
	// Jason 2010-9-20�Ż�����
	BOOL			m_bSaveExpDirect;		// �Ƿ����ֱ�ӱ��澭�飬���ܼ�����Ӱ��
	static INT		m_nNeedSaveExpCounter;	// ��Ҫ���浱ǰ����ļ�����������֮ǰ���Ż�5-10���ķ�ʽ
	INT		m_nSaveExpCounter;			// ÿ�ε���SaveExp2DB����������һ��

public:
	void UpdateChaseHolderOnWater();
	INT		m_nFabaoMoodUpdateTicks;		// ������������

	INT		m_nHolyTicks;		// ����ʥ��ֵ��Tick
	INT		m_nCoValueTicks;	// ��������ʥ���Ĭ��ֵ���µ�Tick
public:
	BYTE GetBuy50LvlItemFlag()		{ return m_byBuy50LvlItemFlag; }
	BYTE GetBuy60LvlItemFlag()		{ return m_byBuy60LvlItemFlag; }
	BYTE GetBuy70LvlItemFlag()		{ return m_byBuy70LvlItemFlag; }
	BYTE GetBuy80LvlItemFlag()		{ return m_byBuy80LvlItemFlag; }
	VOID SetBuyLvlItemFlag(INT nLevel, BYTE byBuyLvlItemFlag)
	{
		switch (nLevel)
		{
		case 50:
			m_byBuy50LvlItemFlag = byBuyLvlItemFlag;
			break;
		case 60:
			m_byBuy60LvlItemFlag = byBuyLvlItemFlag;
			break;
		case 70:
			m_byBuy70LvlItemFlag = byBuyLvlItemFlag;
			break;
		case 80:
			m_byBuy80LvlItemFlag = byBuyLvlItemFlag;
			break;
		default:
			break;
		}
	}
	BYTE GetBuyLvlItemFlag(INT nLevel)
	{
		BYTE byHasBuy50To80LvlItem = 1;
		switch (nLevel)
		{
		case 50:
			byHasBuy50To80LvlItem = GetBuy50LvlItemFlag();
			break;
		case 60:
			byHasBuy50To80LvlItem = GetBuy60LvlItemFlag();
			break;
		case 70:
			byHasBuy50To80LvlItem = GetBuy70LvlItemFlag();
			break;
		case 80:
			byHasBuy50To80LvlItem = GetBuy80LvlItemFlag();
			break;
		default:
			byHasBuy50To80LvlItem = 1;
			break;
		}
		return byHasBuy50To80LvlItem;
	}

	// ����һԪ�ƻ�ԭ�ͱ���ͻ���
	VOID SyncMallOfferProto();

	// �������ϵ��������ؼ�buff����������ʱ���ã�����ʱ�䲻Ҫ����
	BOOL ActiveFabaoStuntBuff(bool bEquip,tagFabao * pFabao,int nEquipPos);

	// �����ɹ��Ľű��¼�
	VOID OnGodStrengthenSuccess(tagEquip*pEquip);


	BOOL HasTakenGuildDailyReward() const;
	VOID ResetGuildDailyRewardTime();

private:
	BYTE m_byBuy50LvlItemFlag;
	BYTE m_byBuy60LvlItemFlag;
	BYTE m_byBuy70LvlItemFlag;
	BYTE m_byBuy80LvlItemFlag;

	DWORD			m_dwPickupModeSetting;	// ʰȡģʽ������Ϣ��λ��Ӱ�����ʰȡ���Լ�'~'ʰȡ

	tagDWORDTime	m_timeTakeGuildReward;	// ��ȡ����ÿ�ս���ʱ��

	DWORD			m_dwVipNetBarInstBuffID;	// ̨��ʥħ�������������󣬸�������buffid

	// ��������˺�
	virtual FLOAT CalAdditionalDmg(Skill * pSkill);
	// ���Ӽ��⣬����Ԫ�ؿ���
	virtual FLOAT CalAdditionalResistance(Skill * pSkill);
private:
	VOID  NoticeClient();

public:
	DWORD GetExpPilularUseTimes() { return m_dwExpPilularUseTimes; }
	VOID IncExpPilularUseTimes() { m_dwExpPilularUseTimes++; }
	VOID ResetExpPilularUseTimes() { m_dwExpPilularUseTimes = 0; }
	VOID ResetHolyToDayEatNum();

	// ̨���½����������� ��ȡ����buffid
	DWORD	GetVipNetbarInstBuffID(VOID) const {	return m_dwVipNetBarInstBuffID; }
	// ̨���½����������� ���ø���buffid
	VOID	SetVipNetbarInstBuffID(DWORD dwBuffId) { m_dwVipNetBarInstBuffID = dwBuffId; }

	INT		GetBuffWrapTimes(DWORD dwBuffTypeID,DWORD& dwSrcRoleID);

	void OnCreateGuild(DWORD dwGuildID);

	// ͬ������Ŀ��仯���Ź����ڵ����������(����ҵ�Ŀ����������unit�����յ���Ϣ)
	VOID	SynTargetChangeToRolesOfVisTile();

	VOID	SynPickupModeSetting2Client();

	// ʰȡģʽ����
	VOID	PickupMode(DWORD dwMode) {m_dwPickupModeSetting = dwMode;}
	DWORD	PickupMode(VOID) const {return m_dwPickupModeSetting;}

	VOID	SynRemoteFabaoInfo(BOOL bEquip);

private:
	DWORD m_dwExpPilularUseTimes;
	BOOL		m_bPlayerBack;
	tagDWORDTime m_timeRealLogout; // �������ϴεǳ�ʱ��
	UINT16		m_byPlayerBackDay;

public:
	EggBroker& GetEggBroker() { return m_EggBroker; }

	enum {EPlyBack_Recv = 1,EPlyBack_NotRecv = 2, EPlyBack_NotTimeout=0};
	BOOL IsPlayerBack() const {return m_bPlayerBack == EPlyBack_Recv;}
	BOOL IsPlayerBackNotRecv() const {return m_bPlayerBack == EPlyBack_NotRecv;}
	VOID PlayerBack(BOOL b) { m_bPlayerBack = b;}
	UINT16	GetPlayerBackDays() const {return m_byPlayerBackDay;}
	VOID SetPlayerBackDays(UINT16 y) {m_byPlayerBackDay = y;}

private:
	EggBroker m_EggBroker;
	// �Ƿ�ʹ�������ļ��������С�ȼ�
	BOOL		m_bUsingPkMinLevel;

	INT		m_nCondenseCoolDown; // ���۵���ʱ��5s
public:
	bool IsOldSerNewPlayer();
	tagDWORDTime GetRealLogoutTime() const  {return m_timeRealLogout;}

	//---------------------------------------------------------------------------------
	// ���һ����ʱ���ܣ�����
	//---------------------------------------------------------------------------------
	INT	LearnExtraSkill(DWORD dwSkillID, INT nLevel);

	//---------------------------------------------------------------------------------
	// ɾ��һ����ʱ���ܣ�����
	//---------------------------------------------------------------------------------
	INT	RemoveExtraSkill(DWORD dwSkillID);

	BOOL StartCondense(); // ��ʼ����
	VOID StopCondense( INT nType, DWORD dwErr = 0,BOOL bSendMsg = FALSE);	// ������������

	//������ְ���Ǿ�ѡ��
	DWORD DirectPromotionClergy(DWORD dwClergyID);
	
	// ����������
	VOID InterruptCondense();
private:
	VOID UpdateCondense();	// ��������
	DWORD CondenseGodHead();	// �������

public:	
	//������ְ����������ֱ�ӽ������Ǿ�ѡ����������������ӿ�
	DWORD PromotionClergy(DWORD dwClergyID, bool bNeedSavePreClergy = true);

	// �ж��Ƿ���Խ�����ְ
	DWORD CanPromotionClergy(DWORD dwClergyID);
	

	BOOL IsUsingPKConstraintsSafeGuradMinLevel() const {return m_bUsingPkMinLevel;}

	// ����Ƿ�ͬ�⵽20���ر�pk����ģʽ��ע��ú���ֻ��������20����ʾ��Һ����
	VOID ClosePKSafeGuard(BOOL bPlyCloseOrNot);

public:
	Wardrobe& GetWardrobe() { return m_Wardrobe; }
	EFashionState OnFashionChange(DWORD dwIdx, DWORD& dwLayerID);
	VOID GetFashionTemperamentSum(INT& nSum, INT& nSuit1Num, INT& nSuit2Num, INT& nSuit3Num, INT& nSuitSum) 
		{ m_Wardrobe.GetFashionTemperamentSum(nSum, nSuit1Num, nSuit2Num, nSuit3Num, nSuitSum); }

private:
	Wardrobe m_Wardrobe;
	INT			m_nTransformSkinNdx;

	BOOL		m_bCondensing;		//��������
public:
	VOID		SetTransSkinNdx(INT ndx,BOOL bSendBigVisMsg = FALSE, BOOL bSendAvatarEquip = TRUE);

	void			SyncRoleSkin2RemoteRole();
	void			SendAvatarEquip2Client();
	INT			GetTransSkinNdx() const { return m_nTransformSkinNdx; }
	VOID		OnCompleteFashionSuitNumChg() 
	{
		if (P_VALID(m_pScript))
			m_pScript->OnCompleteFashionSuitNumChange(this);
	}
	VOID		OnWardLevelUp()
	{
		if(P_VALID(m_pScript))
			m_pScript->OnRoleWardLevelup(this);
	}

//��ϵͳ�������
private:
	AreaMgr		m_GodAreaMgr;

public:
	AreaMgr&		GetGodAreaMgr()	{ return m_GodAreaMgr; }

	//������ְ��������
	DWORD			ChangeAreaAbility(DWORD dwAreaID, BOOL bActive, BOOL bByCD = TRUE);

	//������ְ����
	DWORD			ChangeArea( DWORD dwAreaID,BOOL bAdd, tagGlobalLimitedItem *pGlobalLimitedItem = NULL, BOOL bDelGodFaith=TRUE );

	// ��ʼ������
	VOID			InitArea(const BYTE* &pData, INT32 nNum);

	// ������ϵͳ����
	VOID			SaveArea2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	// ���������ʼ������ͻ���
	VOID			SendInitStateArea();

	//ĳ�������Ѽ��������
	INT				GetAbilityAreaNum(EGodAreaType eType, BOOL bAll = TRUE);

	//�������һ��ϡ����������
	DWORD			RandCancleAreaAbility();

	//���ɾ��һ��ϡ������
	DWORD			RandDelArea();

	//���ݵ�ͼ���ͺ�ID���½�ɫ�ļ���
	VOID			UpdateSkillByMap( EMapType eType, DWORD dwMapID );

	//���ݵ�ͼ���ͺ�ID���½�ɫ�ļ���
	VOID			UpdateSkillByMap( DWORD dwSkillID, EMapType eType, DWORD dwMapID );

	//���¼����
	VOID			ResumeUseSkillByMap( Skill *pSkill );

	//���ܽ�ֹʹ��
	VOID			ForbidUseSkillByMap( Skill *pSkill );

	//ͨ������ֱ�ӼӼ���
	VOID			AddSkillByArea(DWORD dwSkillID);

	VOID			ReCalRoleAtt();

	VOID			UpdateArea();

	//�������CDʱ��
	DWORD			GetAreaCD(DWORD dwAreaID);

	//ȥ��ϡ��������Чbuff
	VOID			RemoveAreaBuff();
	DWORD			GetRandRareArea();

	BOOL			IsHaveArea(DWORD dwAreaID);

	BOOL			DirectDelOneRareArea(DWORD dwAreaID);

	//�ڸõ�ͼ�����Ƿ����
	BOOL			CanUseSkillByMap(DWORD dwSkillID, EMapType eType, DWORD dwMapID);

public:
	VOID RoleRecalAtt()
	{
		// ���¼�������
		if( NeedRecalAtt() )
			RecalAtt();
	}

//�����������
public:
	//������������
	DWORD	TrainFamilySprite( DWORD dwTrainID, INT &nRet, INT64 n8ItemID, BOOL bAgain = FALSE );

	//�Ǽ��������
	DWORD	FamilySpriteRegister( INT64 n8ID );

	//ȡ���������
	DWORD	FamilySpriteGetBack( );

	//ֱ�ӷ��ظ���ɫ�Ǽǵ�����
	VOID	FamilyRoleSpriteReturn();

	//�ѴӼ��巢�صĵǼǵ������Żر������ֿ⡢�ٱ�����
	VOID	AddRoleSpriteFromFamily( tagFabao *pFabao );

	//ֱ��������ҵǼ�����
	VOID	SetFamilyFabao(tagItem *pItem);

	//��ɫ��½ʱ���µǼ��������������Լӳ�
	VOID	UpdateRoleFamilySprite();

private:
	//�Ƿ����������������
	DWORD	CanTrainFamilySprite( DWORD dwTrainID, INT64 n8ItemID, BOOL bAgain = FALSE );

	//�Ƿ���ԵǼ��������
	DWORD	CanFamilySpriteRegister( INT64 n8ID );

	//�Ƿ����ȡ���������
	DWORD	CanFamilySpriteGetBack( INT64 n8ID );

	//��ʼ����ҵļ����������ӵ���������
	VOID	InitAbilityFromFamilySprite();


private:
	DWORD m_dwKeyCodeRewarded;	// �Ƿ���ȡ��key�뽱��,δ��ȡ�������ֵΪ0����ȡ�������ֵ����ʹ�õ�key��
public:
	DWORD GetKeyCodeRewardedState() { return m_dwKeyCodeRewarded; }
	VOID SetKeyCodeRewardedState(DWORD dwKey) { m_dwKeyCodeRewarded = dwKey; }

private:
	// �Ƿ�����״ε�½
	BOOL m_bTodayFirstLogin;
public:
	BOOL IsTodayFirstLogin()
	{
		return m_bTodayFirstLogin;
	}
	VOID SyncFamilyActive(INT nActive);


	VOID SendInitStateClientConfig();

// �̳���Ʒ�����������
private:
	TMap<DWORD, INT> m_mapBuyMallItemCount;	// ��ɫ�����̳���Ʒ���� <��Ʒid���������>
	VOID SaveBuyMallItemCount2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID InitBuyMallItemCount(const BYTE* &pData, INT32 nNum);
	VOID ClearBuyMallItemCount();

public:
	INT BuyThisMallItemCount(DWORD dwTypeID);
	VOID IncreaseRoleBuyMallItemCount(DWORD dwTypeID, INT nNum);

private:
	BOOL m_bSafeguardFlag;

public:
	VOID SetRoleSafeguardFlag(BOOL bSafeguardFlag){m_bSafeguardFlag = bSafeguardFlag;}
	BOOL GetRoleSafeguardFlag( ){ return m_bSafeguardFlag;}

private:
	DWORD	m_dwTrainDate;
	INT		m_nTrainNum;
	INT		m_nAwardPoint;
	INT		m_nAwardFlag;

public:
	VOID	UpdateFamilyTrainNum();

	VOID	AddRoleAwardPoint(INT nData);

	BOOL	CanAddGiftByAwardPoint(DWORD &dwID, DWORD &dwGiftID);
	DWORD	AddGiftByAwardPoint();

	INT		GetAwardPoint(){return m_nAwardPoint;}
	INT		GetAwardFlag(){return m_nAwardFlag;}
private:
	BloodbrotherMgr m_BrotherMgr;
public:
	BloodbrotherMgr& GetBrotherMgr(){return m_BrotherMgr;}
	// �Ƿ��ǽ���ֵ�
	bool IsBrotherOf(DWORD dwID);
	VOID MakeBorther(DWORD dwID,bool IsTeacher = false);
	VOID MakeBrotherSuccess(DWORD dwRoleID);
	VOID	CancelBrotherProfit();
	VOID	SyncBrotherEndTime();
	VOID	SyncClient2EndBloodBrother();

private:
	DWORD m_dwRoleLogData[DataCollection_Max_RoleData_Num];
public:
	VOID InitLogData(const DWORD* p) { memcpy(m_dwRoleLogData, p, sizeof(DWORD)*DataCollection_Max_RoleData_Num ); }
	DWORD GetData(BYTE byIndex) { if(DataCollection_Role_Index_Valid(byIndex)) return m_dwRoleLogData[byIndex]; }
	VOID SetData(BYTE byIndex, DWORD dwdata) { if(DataCollection_Role_Index_Valid(byIndex)) m_dwRoleLogData[byIndex] = dwdata; }

public:
	DWORD ActivePulse(DWORD dwID);
	VOID ResetPulseCanLearnNum();
	INT GetPulseLearnTime(){return m_nPulseLearnTimes; }
private:
	DWORD CheckPulseSkill(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD CalPulseSkillCost(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD LearnPulseSkillSpec(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID);
	INT			m_nPulseLearnTimes;	// �����ͨ�����Ĵ���

	// װ���ɳ���غ���
public:
	DWORD OnRoleEquipGetGrowID(DWORD dwTypeID, BYTE byQlty);
	BYTE OnRoleEquipGrow(DWORD dwTypeID, BYTE byQlty, INT nGrowTimes, INT nIMUseTimes, BYTE byGrowLevel, 
		BYTE byGrayNum, BYTE byGreenNum, BYTE byBlueNum, BYTE byPurpleNum, DWORD dwIM);
	DWORD OnRoleUseDiamondEquipGrowIM(DWORD dwTypeID, INT64 n64EquipID);
	DWORD OnRefreshBaoXiang();
	DWORD OnGetBaoXiang(BYTE byIndex, DWORD dwItemID);

public:
	void	SetGetCash(BOOL bGetCash){m_bGetCash = bGetCash;}
	BOOL	GetGetCash(){return m_bGetCash;}

	VOID	SetRoleBagYuanBao(INT32 nYuanbao){m_CurMgr.SetCurBagYuanBao(nYuanbao);}
	VOID	SetRoleBaibaoYuanBao(INT32 nYuanbao){m_CurMgr.SetCurBaiBaoYuanBao(nYuanbao);}

	VOID	SendBagYuanBao(){m_CurMgr.SendBagYuanBao();}
	VOID	SendBaibaoYuanBao(){m_CurMgr.SendBaibaoYuanBao();}

private:
	BOOL	m_bGetCash;		//�Ƿ��Ѿ��ɹ����ʥ��

private:
	TigerMgr	m_TigerMgr;
public:
	TigerMgr& GetTigerMgr(){return m_TigerMgr;}

private:
	BOOL	m_bInitPkState;

public:
	INT GetTigerTime(){return m_nTigerTimes; }
	VOID AddTigerTime(){m_nTigerTimes++;}
private:
	INT			m_nTigerTimes;	// ���ϻ����Ĵ���

// �꾧���
public:
	VOID RenewSoulCrystalAtt(DWORD dwTypeID, BYTE byQlty, INT* nAtt, OUT DWORD& dwErrorCode, OUT INT* nRetAtt);
	DWORD GetSkillIDFromSoulCrystal()
	{
		tagEquip* pEquip = GetItemMgr().GetEquipBar().GetItem((INT16)EEP_HunJing);
		if(!P_VALID(pEquip)) return 0;
		
		tagSoulCrystal* pSoul = (tagSoulCrystal*)pEquip;
		return (pSoul->SoulSpec.dwStaticSkillID);
	}

// ��һع�
public:
	tagAccountReactiveInfo m_AccountReactiveInfo;
	VOID InitAccountReactiveInfo(tagAccountReactiveInfo* pData)
	{
		IFASTCODE->MemCpy(&m_AccountReactiveInfo, pData, sizeof(tagAccountReactiveInfo));
	}
	VOID AccountSuccessReactive();
	//DWORD RewardReactive(BYTE byRewardLevel);
	EAccountReactiveIdentity IsAccountReactiver() {return m_AccountReactiveInfo.eIdentity;} // �ý�ɫ�Ƿ�����һع������
	DWORD GetAccountReactiveKey() 
	{
		if(IsAccountReactiver())
			return m_AccountReactiveInfo.dwKey;
		else
			return 0;
	}
	INT GetAccountReactiveScore()
	{
		if(IsAccountReactiver())
			return m_AccountReactiveInfo.nScore;
		else
			return 0;
	}
	VOID AddAccountReactiveScore(INT nScore)
	{
		m_AccountReactiveInfo.nScore += nScore;
	}
	
	VOID SetRewardReactiveLevel(BYTE byLevel) 
	{ 
		if(byLevel<=10) 
			m_AccountReactiveInfo.byRewardLevel = byLevel;

		tagNDBC_UpdateRewardReactiveLevel send;
		send.dwAccountID = m_pSession->GetSessionID();
		send.byLevel = byLevel;
		g_dbSession.Send(&send, send.dwSize);
	}

	BYTE GetRewardReactiveLevel() { return m_AccountReactiveInfo.byRewardLevel; }

//------------------------���ħ�����---------------------------
private:
	INT m_nGod;					//���
	INT m_nMonster;				//ħ��
	INT m_nGodPoint;			//�������
	INT m_nMonsterPoint;		//ħ������
	TList<DWORD>				m_listSkillActiveTime;		// ��ʱ�����б�

public:
	INT		GetGod(){return m_nGod;}
	VOID	SetGod( INT nGod ){m_nGod = nGod;}
	VOID ChgGod(INT nChgGod){m_nGod += nChgGod; if(m_nGod<0) m_nGod=0;}

	INT		GetMonster(){return m_nMonster;}
	VOID	SetMonster( INT nMonster ){m_nMonster = nMonster;}

	INT		GetGodPoint(){return m_nGodPoint;}
	VOID	SetGodPoint( INT nGodPoint ){m_nGodPoint = nGodPoint;}

	INT		GetMonsterPoint(){return m_nMonsterPoint;}
	VOID	SetMonsterPoint( INT nMonsterPoint ){m_nMonsterPoint = nMonsterPoint;}
	
	DWORD	ActiveSoulSkill(DWORD dwSkillID,DWORD &dwTime);
	DWORD	ContinueSoulSkill(DWORD dwSkillID, INT64 n64ItemID, DWORD &dwTime);

	//�ı���ħ��
	VOID	ChangeSoul( INT nGod, INT nMonster );
	//�ı���ħ����
	VOID	ChangeSoulPoint( INT nGodPoint, INT nMonsterPoint );

	BYTE	GetGodSoulLevel();
	BYTE	GetMonsterSoulLevel();

	VOID	ContinueSoulSkillByBook( DWORD dwSkillID, tagDWORDTime time );

private:
	DWORD	CanActiveSoulSkill( DWORD dwSkillID );
	DWORD	CanContinueSoulSkill( DWORD dwSkillID, INT64 n64ItemID );
	//ͨ����ħֱ�ӼӼ���
	DWORD	AddSkillBySoul(DWORD dwSkillID,BOOL bForerver, DWORD &dwTime);
	//������ħ�꼼��
	VOID	FiltrateSoulSkill();
//------------------------���ħ�����end---------------------------

//------------------------����billing���---------------------------
public:
	VOID	UpdateGetCash();
	BOOL	DecIPGCash(int nYuanbao, int nNum=0, DWORD dwItemTypeID=0 );

	int		GetIPGCash();

//------------------------����billing���end---------------------------
public:
	VOID	CheckAllEquip(DWORD dwMapID = GT_INVALID);
//------------------------��Kye��--------------------------------------
public:
	DWORD	AddNewKeyCodeGift(DWORD dwKeyCodeCrc);

	BOOL	IsInWar();
	void	SetWarRole(WarRole* pWarRole);
	WarRole* GetWarRole();
	BOOL    IsUseSkill();
private:
	WarRole* m_pWarRole;
};

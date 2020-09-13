//--------------------------------------------------------------------------------
//!\file script_mgr.h
//!\author Aslan
//!
//!\date 2008-09-01
//! last 2008-09-01
//!
//!\brief Lua�ű�����������Ҫ������ֽű�����֯�����ɼ�����
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#pragma once

extern "C"
{
#include "../lua/src/lua.h"
#include "../lua/src/lauxlib.h"
#include "../lua/src/lualib.h"
#include "../lua/src/lstate.h"
};
#include "mutex.h"
#include "event_mgr.h"
#include "../ServerDefine/activity_define.h"
#include "../WorldDefine/penalty_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/loot.h"

class Unit;
class Creature;
class Role;
class CActivity;
class ScriptMgr;
class Skill;

struct tagMapTriggerInfo;
struct tagMapAreaInfo;

enum ERoleReviveType;

void RegisterTraceCmd();

//-------------------------------------------------------------------------------
// ����ű��¼�ö��
//-------------------------------------------------------------------------------
enum EScriptQuestEvent
{
	ESQE_On_Accept			=	0,			// �����ȡ
	ESQE_On_Complete		=	1,			// �������
	ESQE_On_Cancel			=	2,			// ����ȡ��
	ESQE_On_Creature_Kill	=	3,			// ɱ��һֻ����
	ESQE_On_CheckAccept		=	4,			// ����ȡ
	ESQE_On_CheckComplete	=	5,			// ��⽻
	ESQE_On_NPC_Talk		=	6,			// ��NPC�Ի�
	ESQE_On_Init			=	7,			// �����ʼ��
	ESQE_On_Dlg_Default		=	8,			// �������ɿضԻ���ȱʡ�¼�
	ESQE_On_Invest			=	9,			// �������		
	ESQE_OnClientCallback	=	10,			// �����ȡ�ͻ��˷�����
	ESQE_On_BQComplete		=	11,			// ������������
	ESQE_End				=	12,
};

//---------------------------------------------------------------------------------
// ����ű��¼�ö��
//---------------------------------------------------------------------------------
enum EScriptCreatureEvent
{
	// �ճ��¼�
	ESCAE_On_Load			=	0,			// ����
	ESCAE_On_Respawn		=	1,			// ����
	ESCAE_On_Enter_Combat	=	2,			// ����ս��
	ESCAE_On_Leave_Combat	=	3,			// �뿪ս��
	ESCAE_On_Die			=	4,			// ����
	ESCAE_On_Timer			=	5,			// ��ʱ����
	ESCAE_On_Invest			=	6,			// ������
	ESCAE_On_Talk			=	7,			// ��˵��

	// AI�¼�
	ESCAE_On_UpdateAI		=	8,			// ����AI״̬��
	ESCAE_On_UpdateCurAI	=	9,			// ���µ�ǰAI״̬
	ESCAE_On_EnterCurAI		=	10,			// ���뵱ǰAI״̬
	ESCAE_On_LeaveCurAI		=	11,			// �뿪��ǰAI״̬
	ESCAE_On_EventCurAI		=	12,			// ��ǰAI״̬�¼�����
	ESCAE_On_Disappear		=	13,			// ������ʧ����

	ESCAE_On_BeAttacked		=	14,			// �����ڱ�����

	ESCAE_On_IsShopOpened	=	15,			// �̵��Ƿ��������
	ESCAE_On_CanShopping	=   16,	        // ����Ƿ�������̵깺����Ʒװ��
    ESCAE_On_OnShopping     =   17,         //��ҹ�������Ʒ 

	ESCAE_On_BePulled		=	18,			// ����״̬
	
	ESCAE_End				=	19,
};

//---------------------------------------------------------------------------------
// ��ҽű�
//---------------------------------------------------------------------------------
enum EScriptRoleEvent
{
	ESRE_On_Online				=	0,			// ����
	ESRE_On_FirstOnline			=	1,			// ��һ������
	ESRE_On_IntoWorld			=	2,			// ������Ϸ����
	ESRE_On_FirstIntoWorld		=	3,			// ��һ�ν�����Ϸ����
	ESRE_On_EnterMap			=	4,			// �����ͼ
	ESRE_IsDeadPenalty			=   5,			// �Ƿ����ҽ��������ͷ�
	ESRE_On_LevelChange			=	6,			// �ȼ�����
	ESRE_On_OpenChest			=	7,			// ��������
	ESRE_On_StopChest			=	8,			// ֹͣ���������������Ʒ
	ESRE_On_AgainChest			=	9,			// �ٿ�һ��
	ESRE_On_GetItem				=	10,			// �õ�������Ʒ
	ESRE_On_GetMarriage			=	11,			// ���
	ESRE_On_GetMarriageCheck	=	12,			// �����
	ESRE_On_BreakMarriage		=	13,			// ���
	ESRE_On_BreakMarriageCheck	=	14,			// �����
	ESRE_IsEquipLootPenalty		=   15,			// װ������ͷ�
	ESRE_IsExpPenalty			=	16,			// ����ͷ�
	ESRE_IsInjuryPenalty		=	17,			// ���˳ͷ�
	ESRE_IsMoralityPenalty		=	18,			// ���³ͷ�
	ESRE_IsHostilityPenalty		=	19,			// �����ͷ�
	ESRE_IsCommercePenalty		=	20,			// ���������ͷ�
	ESRE_GetRoleFishingExp		=	21,			// ���㾭�飨�����ã�
	ESRE_CanCancelPenalty		=	22,			// �Ƿ�����������ͷ�
	ESRE_On_CompleteCommerce	=	23,			// �������
	ESRE_WaitSummon				=	24,			// �����ػ�ʯ
	ESRE_CancelSummon			=	25,			// ȡ���ٻ�
	ESRE_UseJingWuPoint			=	26,			// ʹ�þ������
	ESRE_HandleMaster			=	27,			// ��ʦ
	ESRE_IsTrainFinished		=	28,			// ��֮����ϵͳ--�ж��������
	ESME_OnObtainItem			=	29,			// ��һ��һ�����ߵ��¼��ӿ�
	ESME_OnSilverIncrease		=	30,			// �����Ϸ�����ӵ��¼��ӿ�
	ESME_OnObtainTitle			=	31,			// ��һ��һ���ƺŵ��¼��ӿ�
	ESME_OnItemConsolidate		=	32,			// ������һ��װ��ǿ�����¼��ӿ�
	ESME_OnActiveLonghunSuccess	=	33,			// ��Ҽ�������ɹ����¼��ӿ�
	ESRE_OnTrainFinished		=	34,			// ������һ������

	ESRE_OnFabaoAttChange		=	35,			// �������Ա仯�����ס�������á�������������
   
	ESRE_OnRoleJoinToArenaRoom  =   36,         // ��ɫ���뾺��������
    ESRE_OnRoleCreateArenaRoom  =   37,         // ��ɫ��������������
    ESRE_OnRoleAgreeToBeInvited =   38,        // ��ɫͬ�ⱻ������뷿��
	
	ESRE_OnReadyToSoaring		=	39,			// ��ʼ������֪ͨ�ű���Ҫ�����ˣ��ӷ�������
	ESRE_OnCompletePracticeEquipment	=	40,			// �����ɹ���־
	ESRE_OnCompleteCastingEquipment		=	41,			// �����ɹ���־

	ESRE_On_ForceBreakMarriage		=	42,			// ǿ�����
	ESRE_On_ForceBreakMarriageCheck	=	43,			// ǿ�������
	
	ESRE_OnChaseHolderDie			=	44,		// �ᱦ����Я�������߱�ɱ��ʱ����
	ESRE_ForceLootChase				=	45,		// ����ǿ�Ƶ���(�ᱦ���)
	ESRE_OnMakeFriend				=	46,		// ���Ӻ���
	ESRE_OnLearnSkill				=	47,		// ���ѧ���¼���
	ESRE_OnEquipConsolidateLevelUp	=	48,		// װ��ǿ���ȼ�����
	ESRE_OnProduceEquip			=	49,			// ����װ��
	ESRE_OnDigHoleOnEquip		=	50,		// ��ҿ�����Ƕ��
	ESRE_OnCutStone				=	51,		// ����иʯԭʯ
	ESRE_OnDressEquip			=	52,		// ��Ҵ���װ��
	ESRE_OnBrandEquip			=	53,		// ��װ��ʹ��ʥӡ
	ESRE_OnAddItemToBag			=	54,		// �����л����Ʒ
	ESRE_OnGodStrengthenSuccess	=	55,		// �����ɹ�����¼�
	ESRE_OnUseGoldStone			=	56,			// ʹ�õ��ʯ

	ESRE_OnCreateGuild			=	57,			// ��������

	ESRE_CanAddEnemy			=	58,			// �ж��Ƿ���Լ������б�
	ESRE_OnKillMonster			=	59,			// ɱ���¼�

	ESRE_CondenseUpgrade	= 60,	//�������
	ESRE_GetClergy			= 61,	//������ְ
	ESRE_Transform			= 62,	//ʹ�ñ�����
	ESRE_GetRareArea		= 63,	//���ϡ������
	ESRE_WinRareArea		= 64,	//����ϡ������
	ESRE_ActivateArea		= 65,	//��ѡ���򼤻�
	ESRE_IsCanRobRareArea	= 66,	//�Ƿ���Խ���ϡ����������

	ESRE_CanUseDakNPC		= 67,	// �Ƿ����ʹ����վNPC
	ESRE_CanUseEarthTemple	= 68,	// �Ƿ����ʹ��������

	ESRE_CanCalCrit			= 69,	// �Ƿ���㱩��
	ESRE_RoleLogout			= 70,	// ��ɫ���� 

	ESRE_On_StartChest				=	71,			// �����俪ʼ������

	ESRE_On_FashionSuitNumChg		= 72, // ��ɫ�³��ڼ���ʱװ�������ı�
	ESRE_On_WardLevelUp = 73,			// �³�����
	ESRE_Quest_IncSilver = 74,				// ���������ý�Ǯ
	ESRE_BuyItem_DecSilver = 75,			// ͨ������������Ľ��
	ESRE_Strengthen_DecSilver = 76,			// ͨ������װ��ǿ���ȼ����Ľ��
	ESRE_Compose_DecSilver = 77,			// ͨ���ϳ����Ľ��
	ESRE_Can_StrengthenEquip = 78,			// ���ǿ��װ��ʱ��ɹ����ж�
	ESRE_RoleGetBoardQuestReward = 79, // ��ɫ��ȡ���������Ľ���
	ESRE_EquipGetGrowAtt = 80, // װ����óɳ�����
	ESRE_EquipGrowGetStar = 81, // װ���ɳ������µ�����

	ESRE_AccountReactived = 85, // ��ұ�����
	
	ESRE_GetSoulCrystalRenew = 86, // ���û꾧����

	ESRE_MapChangeWithScriptEquip = 87,	// ��Ҵ��� MapLimitΪ1��װ���л���ͼ
	ESRE_TryToWareScriptEquip = 88,				// �����ͼ���� MapLimitΪ1��װ��
	//ESRE_RewardReactive = 87, // ��������ȡ���̽���
	ESRE_UseDiamondAddAndEquipGrowIM = 89,	//ʹ��ˢ�ɳ�id�Լ���ʯ��������
	ESRE_RefreshBaoXiang = 90,	//ˢ��ս�����㽱��
	ESRE_GetHolyDevourNum = 91, //�����װ��ʥ������ɴ���

	ESRE_End,
};

enum ERoleFabaoAttEvent
{
	RFAE_LevelUp		=	0,	// ��������
	RFAE_HoleAttReset	=	1,	// ���������������
	RFAE_WuxingXSReset	=	2,	// ����������������
};

//---------------------------------------------------------------------------------
// ��ͼ�ű�
//---------------------------------------------------------------------------------
enum EScriptMapEvent
{
	ESME_OnInit					=	0,			// ��ʼ��ʱ
	ESME_OnTimer				=	1,			// ��ʱ����
	ESME_OnPlayerEnter			=	2,			// ��ҽ���
	ESME_OnPlayerLeave			=	3,			// ����뿪������������
	ESME_OnCreatureDie			=	4,			// ��������
	ESME_OnRoleDie				=	5,			// �������
	ESME_OnEnterTrigger			=	6,			// ���봥����
	ESME_OnEnterArea			=	7,			// ��������
	ESME_CanInviteJoinTeam		=	8,			// �Ƿ������������
	ESME_CanLeaveTeam			=	9,			// �Ƿ������뿪����
	ESME_CanChangeLeader		=	10,			// �Ƿ����ƽ��ӳ�
	ESME_On_Revive				=	11,			// ��Ҹ���	
	ESME_CanEnterWhenOnline		=   12,			// �������ʱ�Ƿ��ܼ����ͼ
	ESME_GetExportMapAndCoord	=	13,			// �õ�����뿪��ǰ��ͼ��ĵ�ͼ�ɣĺ�����
	ESME_GetOnePerfectMap		=   14,			// �ҵ���ѵĸ���ʵ��
	ESME_CanEnter				=	15,			// ����Ƿ��ܽ���õ�ͼ
	ESME_FriendEnemy			=	16,			// ���������ĵ��ҹ�ϵ
	ESME_CanKickMember			=	17,			// �Ƿ������ߵ�����
	ESME_OnCreatureDisappear	=	18,			// ������ʧ
	ESME_Safeguard				=	19,			// �Ƿ�������ҿ�������ģʽ
	ESME_CanUseItem				=	20,			// �Ƿ�����ʹ����Ʒ
	ESME_CanUseSkill			=	21,			// �Ƿ�����ʹ�ü���
	ESME_CityBroadCast			=	22,			// ���й㲥
	ESME_OnLeaveArea			=	23,			// ����뿪ĳһ����
	ESME_OnCreatePvPMap			=	24,			// ������-����һ���µľ�������ͼ
	ESME_OnPlayerLeaveW			=	25,			// ����뿪����������
	ESME_ExperienceInstance		=   26,			// ��ҽ������鸱���¼�

	ESME_End					=	27,
};

//---------------------------------------------------------------------------------
// �̶���ű�
//---------------------------------------------------------------------------------
enum EScriptActEvent
{
	ESAE_OnInit				=	0,			// ��ʼ��ʱ
	ESAE_OnTimer			=	1,			// ��ʱ����
	ESAE_OnStart			=	2,			// ���ʼ
	ESAE_OnEnd				=	3,			// �����
	ESAE_OnTimerMin			=	4,			// �ÿ���Ӹ���
	ESAE_OnDefaultRequest	=	5,			// �ͻ��˴����������ű���ȱʡ��Ϣ 
	ESAE_Broad				=	6,			// ��㲥
	ESAE_FinishEudemonSummon	=	7,		// ����ػ����ٻ�
	ESAE_OnClientCallback		= 8,		// �����ȡ�ͻ��˷�����
	ESAE_OnActOpenClose	= 9,				// ����̨������رջ
	ESAE_End				=	10,
};

//---------------------------------------------------------------------------------
// ��Ϸ�����¼��ű�
//---------------------------------------------------------------------------------
enum EScriptWorldEvent
{
	ESWE_Adventure			=	0,			// ��������
	ESWE_OnCreatePvPMap3v3  =	1,          // ���������� 
	ESWE_OnLoot				=	2,			// ����
	ESWE_BeKill_DropSilver	=	3,			// ���ﱻ��ɱ������Ǯ
	ESWE_CreateItem			=	4,			// ����һ���µ���
	ESWE_DelItem			=	5,			// ��Ʒ��ʧ�����㻯���߱�ɾ������ʱ�䵽���߳��ۣ�������ֱ�ӽ��׺Ͱ�̯���ף�
	ESWE_ExchangeItem		=	6,			// ��Ʒ�����ף�ͨ����̯������
	
	ESWE_GetSoulCrystalQlty = 7,	// ��û꾧Ʒ��
	ESWE_GetSoulCrystalSkillID = 8, // ��û꾧����id
	ESWE_GetSoulCrystalAtt = 9, // ��û꾧����id

    ESWE_End =	 10,
};

//---------------------------------------------------------------------------------
// ��Ʒ�¼��ű�
//---------------------------------------------------------------------------------
enum EScriptItemEvent
{
	ESIE_CanUse									=	0,			// ��Ʒ�Ƿ����
	ESIE_Use											=	1,			// ��Ʒʹ��
	ESIE_CanDelGroundItem					=	2,			// ���ڵ��ϵ���Ʒ�Ƿ����ɾ��
	ESIE_PutDownItem							=	3,			// ������Ʒ
	ESIE_CanPickUpItem						=	4,			// �ж��Ƿ���Լ�����Ʒ
	ESIE_PickUpItem								=	5,			// ������Ʒʱ����
	ESIE_CanPutDown							= 6,			// �ж���Ʒ�Ƿ���Զ���
	ESIE_CanDelItemOnRoleInit			= 7,			// ��ɫ��ʼ��ʱ�򣬼�鱳������Ʒ��Ҫɾ����0��ʾ��Ҫɾ���� 1��ʾ����Ҫ��
	ESIE_Get		=	8,			// ��Ʒ���

	ESIE_End
};

//---------------------------------------------------------------------------------
// �����¼��ű�
//---------------------------------------------------------------------------------
enum EScriptSkillEvent
{
	ESSE_CanCast			=	0,			// �����Ƿ����
	ESSE_Cast				=	1,			// ����ʹ��	
	ESSE_SealSkill			=	2,			// ��ӡ
	ESSE_CancelSkill		=	3,			// ȡ������ʹ��

	ESSE_OnSkillChange			=	4,			// ��Ҽ��ܸı䣬ѧ�ᣬ�ȼ��仯��

	ESSE_Cast2SingleTarget	=	5,			// ʹ�õ�Ŀ��ļ���

	ESSE_End
};

enum EScriptMotionEvent
{
	ESMOTION_Motion         =   0,          // ���Զ�������
	ESMOTION_End
};

enum EScriptBuffEvent
{
	ESBUFF_Destroy			=	0,			// Buff�����¼�

	ESBUFF_End,
};

//---------------------------------------------------------------------------------
// ͨ�ýű���
//---------------------------------------------------------------------------------
template<INT nSize>
class Script
{
public:
	VOID RegisterFunc(INT nIndex, const CHAR* szFunc);
	VOID Destroy();

	CONST CHAR* operator[] (INT ndx)const 
	{
		if( ndx >= 0 && ndx < nSize )
			return m_szFunc[ndx];
		return NULL;
	}

protected:
	Script();
	~Script();

protected:
	CHAR*	m_szFunc[nSize];		// �ű������ַ���
};

template<INT nSize>
inline Script<nSize>::Script()
{
	ZeroMemory(m_szFunc, sizeof(m_szFunc));
}

template<INT nSize>
inline Script<nSize>::~Script()
{
	Destroy();
}

template<INT nSize>
inline VOID Script<nSize>::RegisterFunc(INT nIndex, const CHAR* szFunc)
{
	if( nIndex < 0  || nIndex >= nSize ) return;
	if( !P_VALID(szFunc) ) return;

	if( P_VALID(m_szFunc[nIndex]) )
	{
		TObjRef<Util> pUtil = "Util";
		IMSG(_T("%s is Registerred twice\r\n"), pUtil->AnsiToUnicode(szFunc));
		free(m_szFunc[nIndex]);
	}

	m_szFunc[nIndex] = _strdup(szFunc);
}

template<INT nSize>
inline VOID Script<nSize>::Destroy()
{
	for(INT n = 0; n < nSize; ++n)
	{
		if( P_VALID(m_szFunc[n]) )
		{
			free(m_szFunc[n]);
			m_szFunc[n] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------
// ���Զ����ű�
//--------------------------------------------------------------------------------
class MotionScript : public Script<ESMOTION_End>
{
	friend class ScriptMgr;
private:
	~MotionScript() {}
public:
	void OnMotion(DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwDestRoleID,DWORD dwMotionID)const;

};
//--------------------------------------------------------------------------------
// buff�ű�
//--------------------------------------------------------------------------------
class BuffScript : public Script<ESBUFF_End>
{
	friend class ScriptMgr;
private:
	~BuffScript() {}
public:
	void OnDestroy(DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwBuffID)const;

};
//---------------------------------------------------------------------------------
// ����ű�
//---------------------------------------------------------------------------------
class QuestScript : public Script<ESQE_End>
{
	friend class ScriptMgr;
private:
	~QuestScript() {}
public:
	INT  CheckAccept(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const;
	INT  CheckComplete(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const;

	VOID OnInit(UINT16 u16QuestID, Role* pOwner) const;
	VOID OnAccept(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnBoardQuestComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnCancel(UINT16 u16QuestID, Role* pOwner) const;
	VOID OnCreatureKill(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const;
	VOID OnNPCTalk(UINT16 u16QuestID, Role* pOwner, DWORD dwNPCID, DWORD dwNPCTypeID) const;
	VOID OnDlgDefault(UINT16 u16QuestID, Role* pOwner, DWORD dwDlgOption) const;
	VOID OnInvest(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const;
	VOID OnClientCallback(INT nMsgID,Role* pPlayer,DWORD param1,DWORD param2,DWORD param3,DWORD param4) const;

	
};

//----------------------------------------------------------------------------------
// ����AI�ű�
//----------------------------------------------------------------------------------
class CreatureScript : public Script<ESCAE_End>
{
	friend class ScriptMgr;
private:
	~CreatureScript() {}
public:
	VOID OnLoad(Creature* pCreature) const;
	VOID OnTalk(Creature* pCreature, Role* pRole, INT nIndex=-1) const;
	VOID OnRespawn(Creature* pCreature) const;
	VOID OnEnterCombat(Creature* pCreature) const;
	VOID OnLeaveCombat(Creature* pCreature) const;
	VOID OnDie(Creature* pCreature, Unit* pKiller, BOOL& bRemove) const;
	VOID OnInvest(Creature* pCreature, Role* pScr) const;

	VOID OnUpdateAI(Creature* pCreature) const;
	VOID OnUpdateCurAI(Creature* pCreature) const;
	VOID OnEnterCurAI(Creature* pCreature) const;
	VOID OnLeaveCurAI(Creature* pCreature) const;
	VOID OnDisappear(Creature* pCreature) const;
	BOOL OnEventCurAI(Creature* pCreature) const;

	BOOL IsShopOpened(Creature* pCreature,Role * player,Map* pMap) const;
	BOOL CanShopping(Creature* pCreature,Role * player,Map* pMap,DWORD dwShopID,DWORD dwItemTypeID ) const;
	VOID OnShopping(Creature* pCreature,Role * player,Map* pMap,DWORD dwShopID,DWORD dwItemTypeID ) const;

	VOID OnBeAttacked(Creature* pCreature, Unit* pSrc) const;

	VOID OnBePulled(Creature * pCreature,const Vector3 & Pos,BOOL result) const;
};

//-------------------------------------------------------------------------------------
// ��ҽű�
//-------------------------------------------------------------------------------------
class RoleScript : public Script<ESRE_End>
{
	friend class ScriptMgr;
private:
	~RoleScript() {}
public:
	VOID	OnRoleOnline(Role* pRole) const;
	VOID	OnRoleFirstOnline(Role* pRole) const;
	VOID	OnRoleIntoWorld(Role* pRole) const;
	VOID	OnRoleFirstIntoWorld(Role* pRole) const;
	VOID	OnRoleEnterMap(Role* pRole) const;
	BOOL	IsDeadPenalty(Role* pRole, EDeadPenaltyMode eMode) const;
	VOID	OnRoleLevelChange(Role* pRole) const;
	VOID	OnOpenChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const;
	VOID	OnStopChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const;
	VOID	OnAgainChest(Role* pRole) const;
	VOID	OnGetItem(Role* pRole, DWORD dwChestID, DWORD dwItemID, INT nNum) const;
	VOID	OnStartChest(Role* pRole) const;
	VOID	OnGetMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnGetMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	VOID	OnBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	VOID	OnForceBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnForceBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	INT		OnGetRoleFishingExp(Role* pRole) const;
	DWORD	CanCancelPenalty(Role* pRole, Unit* pKiller, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommerce) const;
	VOID	OnCompleteCommerce(Role* pRole,DWORD dwTaelProgress) const;
	DWORD	OnWaitSummon(Role* pRole, DWORD dwCreatureID, BOOL& bRet) const;
	DWORD	OnCancelSummon(Role* pRole, DWORD dwCreatureID) const;
	VOID	OnUseJingWuPoint(Role* pRole, INT nPoint) const;
	INT		IsTrainFinished(Role* pRole, DWORD dwTrainID) const;									// ��֮����ϵͳ--�������
	VOID	OnObtainItem(Role* pRole, DWORD dwItemID, EItemCreateMode eCreateMode) const;			// ��һ��һ�����ߵ��¼��ӿ�
	VOID	OnSilverChange(Role* pRole, DWORD dwSilverIncrease) const;								// �����Ϸ�����ӵ��¼��ӿ�
	VOID	OnObtainTitle(Role* pRole, DWORD dwTitleID) const;										// ��һ��һ���ƺŵ��¼��ӿ�
	VOID	OnItemConsolidate(Role* pRole, DWORD dwEquiptID, EAllConsolidateType eConsolidateType,
	INT nConsolidateLevel, BOOL bSuccess) const;							// ������һ��װ��ǿ�����¼��ӿ�
	VOID	OnActiveLonghunSuccess(Role* pRole, DWORD dwEquiptID, DWORD dwLongHunSkillID) const;	// ��Ҽ�������ɹ����¼��ӿ�
	VOID	OnTrainFinished(Role* pRole) const;														// ������һ������

	VOID	OnHandleMaster(Role* pMaster, Role* pApprentice) const;

	VOID	OnFabaoAttChange(Role* pOwner,CONST tagFabao * pFabao,INT nWhat,DWORD dwOther) const;

	VOID    OnRoleJoinToArenaRoom(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,BOOL& bCanJoin)const;
	VOID    OnRoleCreateArenaRoom(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,BOOL& bCanCreate)const;
	VOID    OnRoleAgreeToBeInvited(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,DWORD dwDestRoleID,BOOL& bCanBeInvited)const;

	VOID	OnReadyToSoaring(Role * pRole) const;
	VOID	OnCompletePracticeEquipment(Role* pRole,DWORD dwItemTypeID) const;
	VOID	OnCompleteCastingEquipment(Role* pRole,DWORD dwItemTypeID,INT nCastingLevel) const;

	VOID	OnChaseHolderDie(Role * pRole, DWORD dwKillerID) const;
	VOID	ForceLootChase(Role * pRole) const;
	VOID	OnMakeFriend(Role * pRole, DWORD dwFriendID) const;
	VOID	OnLearnSkill(Role * pRole, DWORD dwSkillID) const;
	VOID	OnEquipConsolidateLevelUp(Role * pRole, DWORD dwTypeID, BYTE byBeforeLvl, BYTE byAfterLvl) const;
	VOID	OnProduceEquip(Role * pRole, DWORD dwTypeID, BYTE byQuality) const;
	VOID	OnDigHoleOnEquip(Role * pRole, DWORD dwTypeID, BOOL bSuccess, BYTE byHoleNum) const;
	DWORD	OnCutStone(Role * pRole, DWORD dwTypeID, DWORD dwNewStoneTypeID, DWORD dwDestTypeID) const;
	VOID	OnDressEquip(Role * pRole, bool bEquipOrNot, DWORD dwTypeID, BYTE byEquipPos, DWORD dwSerialIDHigh, DWORD dwSerialIDLow) const;
	VOID	OnBrandEquip(Role * pRole, DWORD dwTypeID,  DWORD dwBrandItemTypeID, BOOL bSuccess, BYTE byBrandLevel) const;
	VOID	OnAddItemToBag	(Role * pRole, DWORD dwTypeID) const;
	// �����ɹ��¼�
	VOID	OnGodStrengthenSuccess(Role * pRole,tagEquip * pEquip) const;

	DWORD	OnUseGoldStone(Role * pRole,	tagItem* pItem) const;

	VOID	OnCreateGuild(Role * pRole,DWORD dwGuildID) const;
	DWORD	CanAddEnemy(Role * pRole,DWORD dwKillerID) const;
	DWORD	OnKillMonster(Role * pRole, Creature * pCreature) const ;

	VOID	OnRoleCondenseUpgrade(Role* pRole) const;
	VOID	OnRoleGetClergy(Role* pRole) const;
	VOID	OnRoleGetRareArea(Role* pRole, DWORD dwAreaID) const;
	VOID	OnRoleWinRareArea(Role* pRole, DWORD dwRoleID, DWORD dwAreaID) const;
	VOID	OnRoleActivateArea(Role* pRole, DWORD dwAreaID) const;
	BOOL	IsCanRobRareArea(Role* pRole) const;

	BOOL	CanUseDakNPC(Role* pRole, DWORD dwDestMapID, FLOAT x, FLOAT y, FLOAT z) const;
	BOOL	CanUseEarthTemple(Role* pRole, DWORD dwDestMapID, FLOAT x, FLOAT y, FLOAT z) const;

	BOOL	CanCalCrit(Role* pRole, DWORD dwTargetID) const;
	VOID	OnRoleLogout(Role* pRole) const;

	VOID	OnRoleWardLevelup(Role* pRole) const;

	VOID	OnCompleteFashionSuitNumChange(Role* pRole) const;

	// ���������ý�Ǯ
	VOID IncSilverByQuest(Role* pRole, DWORD dwQuestID, INT nSilver) const;

	// ͨ������������Ľ��
	VOID DecSilverByBuyItem(Role* pRole, DWORD dwItemID, INT nSilver) const;

	// ͨ������װ��ǿ���ȼ����Ľ��
	VOID DecSilverByStrengthen(Role* pRole, DWORD dwTypeID, INT nQuality, INT nLevel, INT nSilver ) const;

	// ͨ���ϳ����Ľ��
	VOID DecSilverByCompose( Role* pRole, DWORD dwTypeID, INT nSilver ) const;

	DWORD CanStrengthenEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow, INT nQuality, INT nLevel) const;

	DWORD OnRoleEquipGetGrowID(Role* pRole, DWORD dwTypeID, INT nQlty) const;

	BYTE OnRoleEquipGrow(Role* pRole, DWORD dwTypeID, BYTE byQlty, INT nGrowTimes, INT nIMUseTimes, BYTE byGrowLevel, 
		BYTE byGrayNum, BYTE byGreenNum, BYTE byBlueNum, BYTE byPurpleNum, DWORD dwIM) const;

	VOID OnGetBoardQuestReward(Role*, UINT16, UINT16, INT, INT) const;

	VOID RenewSoulCrystalAtt(Role* pRole, DWORD dwTypeID, BYTE byQlty, INT* nAtt, OUT DWORD& dwErrorCode, OUT INT* nRetAttID) const;

	VOID AccountSuccessReactive(Role*) const;

	DWORD OnRoleMapChangeWithScriptEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow,DWORD dwMapID) const;
	DWORD OnRoleTryToWareScriptEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow) const;

	DWORD OnUseDiamondAddAndEquipGrowIM(Role* pRole, DWORD dwTypeID, INT64 n64EquipID) const;
	//DWORD RewardReactive(Role* pRole, BYTE byRewardLevel) const;
	DWORD OnRefreshBaoXiang(DWORD dwScole, DWORD &dwItemID1, DWORD &dwItemID2, DWORD &dwItemID3) const;
	VOID OnGetHolyDevourNum(Role* pRole, DWORD dwDevourNum) const;
};

//-------------------------------------------------------------------------------------
// ��ͼ�ű�
//-------------------------------------------------------------------------------------
class MapScript : public Script<ESME_End>
{
	friend class ScriptMgr;
private:
	~MapScript() {}
public:
	VOID	OnInit(Map* pMap) const;
	VOID	OnTimer(Map* pMap, INT nMilliSec) const;
	VOID	OnPlayerEnter(Role* pRole, Map* pMap) const;
	VOID	OnPlayerLeave(Role* pRole, Map* pMap, BOOL bLogout) const;
	VOID	OnCreatureDie(Creature* pCreature, Unit* pKiller, Map* pMap) const;
	VOID	OnRoleDie(Role* pRole, Unit* pKiller, Map* pMap) const;
	VOID	Revive(Role* pRole, ERoleReviveType eType, INT &nReviveHP, INT &nReviveMP, FLOAT &fx, FLOAT &fy, FLOAT &fz, DWORD &dwRebornMapID) const;
	VOID	OnEnterTrigger(Role* pRole, tagMapTriggerInfo* pTrigger, Map* pMap) const;
	VOID	OnEnterArea(Role* pRole, tagMapAreaInfo* pArea, Map* pMap) const;
	VOID	OnLeaveArea(Role* pRole, DWORD dwCurScriptArea, Map* pMap) const;
	INT		CanInviteJoinTeam(Map* pMap) const;
	INT		CanLeaveTeam(Map* pMap) const;
	INT		CanChangeLeader(Map* pMap) const;
	INT		CanKickMember(Map* pMap) const;
	DWORD	FriendEnemy(Map* pMap, Unit* pSrc, Unit* pTarget, BOOL& bIgnore) const;
	VOID	OnCreatureDisappear(Creature* pCreature, Map* pMap) const;
	INT		CanSetSafeguard(Map* pMap) const;
	BOOL	CanUseSkill(Map* pMap, DWORD dwTypeID) const;
	BOOL	CanUseItem(Map* pMap, DWORD dwTypeID) const;
	BOOL	OnCityBroadCast(DWORD dwBroadType,DWORD dwPara1 = GT_INVALID,DWORD dwPara2 = GT_INVALID\
							,DWORD dwPara3 = GT_INVALID,DWORD dwPara4 = GT_INVALID,DWORD dwPara5 = GT_INVALID) const;
	BOOL	OnCheckRoleLevel(DWORD dwMapID, DWORD dwInstanceID, DWORD dwRoleID) const;
    
	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���θò��ֹ���
	//--------------------------------------------------------------------------
	// ������-����һ���µľ�������ͼ
	BOOL	OnCreatePvPMap3v3(DWORD dwMapID, DWORD dwInstanceID, INT16 nRoomID, DWORD* pdwPlayerRoleID, INT16 nPlayerCnt) const;

	// �ű������ĸ������нӿ�
	INT		GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const;
	VOID	CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const;
	VOID	GetOnePerfectMap(Role* pRole, DWORD &dwInstanceID) const;
	INT		CanEnter(Role* pRole) const;
};

//-------------------------------------------------------------------------------------
// ��ű�
//-------------------------------------------------------------------------------------
class ActScript : public Script<ESAE_End>
{
	friend class ScriptMgr;
private:
	~ActScript() {}
public:
	VOID OnInit(DWORD dwActID) const;
	VOID OnTimer(DWORD dwActID, INT nSec) const;
	VOID OnTimerMin(DWORD dwActID) const;
	VOID OnActStart(DWORD dwActID) const;
	VOID OnActEnd(DWORD dwActID) const;
	VOID OnDefaultRequest(DWORD dwActID, Role* pRole, DWORD	dwEventType) const;
	VOID BroadActivityState(EActBroadMode eState) const;
	VOID OnClientCallback(INT nMsgID,Role* pPlayer,DWORD param1,DWORD param2,DWORD param3,DWORD param4) const;
	VOID FinishEudemonSummon() const;
	VOID OnActOpenClose(DWORD dwActID, DWORD dwIsOpen) const;
};

//-------------------------------------------------------------------------------------
// ��Ʒ�ű�
//-------------------------------------------------------------------------------------
class ItemScript : public Script<ESIE_End>
{
	friend class ScriptMgr;
private:
	~ItemScript() {}
public:
	BOOL CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, BOOL &bIgnore) const;
	BOOL CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, DWORD dwDesRoleID, BOOL &bIgnore) const;
	// Jason 2010-12-3 v1.3.0 ע�⣬����0ʱ����������ʹ�ã����۳�ʹ�ô�����1�Σ������̣�����1�򲻿�ʹ�ô�����
	INT UseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID) const;
	VOID UseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, DWORD dwDesRoleID) const;
	INT GroundItemCanDel(DWORD dwTypeID, INT64 n64Serial) const;
	VOID PutDownItem(DWORD dwTypeID) const;
	INT		CanPickUpItem(DWORD dwItemID, DWORD dwRoleID) const;
	VOID	OnPickUpItem(DWORD dwItemID, DWORD dwRoleID) const;
	INT		ItemCanPutDown(DWORD dwTypeID, INT64 n64Serial) const;
	BOOL	CanDeleteItem(DWORD dwTypeID, INT64 n64Serial) const;
	VOID GetItem(Map* pMap, DWORD dwTypeID, DWORD dwRoleID, EItemCreateMode eGetMode, DWORD n32High, DWORD n32Low) const;
};

//-------------------------------------------------------------------------------------
// ���ܽű�
//-------------------------------------------------------------------------------------
class SkillScript : public Script<ESSE_End>
{
	friend class ScriptMgr;
private:
	~SkillScript () {}
public:
	DWORD CanCastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwDstUnitID) const;
	VOID CastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, BOOL &bIgnore) const;
	VOID CastSingleTargetSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, BOOL &bIgnore) const;
	VOID SealSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, DWORD dwTargetTypeID) const;
	VOID CancelSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, DWORD dwTargetTypeID) const;

	VOID OnSkillChange(Map* pMap,DWORD dwRoleID,DWORD dwSkillID,INT nPrevVal,INT nCurVal) const;
};


//-------------------------------------------------------------------------------------
// ��Ϸ�����¼��ű�
//-------------------------------------------------------------------------------------
class WorldScript : public Script<ESWE_End>
{
	friend class ScriptMgr;
private:
	~WorldScript() {}
public:
	VOID OnAdventure(Role* pRole) const;
	BOOL OnLoot(DWORD dwSrcID, DWORD dwTargetID, ELootType eType, DWORD dwTypeID, DWORD dwSetID = 0) const;

	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���θò��ֹ���
	//--------------------------------------------------------------------------
	// ������-����һ���µľ�������ͼ
	BOOL	OnCreatePvPMap3v3(DWORD dwMapID, DWORD dwInstanceID, INT16 nRoomID, DWORD* pdwPlayerRoleID, INT16 nPlayerCnt) const;


	// ����һ���µ���
	VOID CreateItem( DWORD dwMapID, DWORD dwInstanceID, DWORD dwTypeID, INT nQuality ) const;

	//��Ʒ��ʧ�����㻯���߱�ɾ������ʱ�䵽���߳��ۣ�������ֱ�ӽ��׺Ͱ�̯���ף�
	VOID DelItem( DWORD dwDelType, DWORD dwTypeID, INT nQuality ) const;

	// ��Ʒ�����ף�ͨ����̯������
	VOID ExchangeItem( Map* pMap, DWORD dwRoleID, DWORD dwBuyRoleID, DWORD dwTypeID, INT nQuality, INT nSilver ) const;

	//���ﱻ��ɱ������Ǯ
	VOID DropSilver(Creature * pCreature, DWORD dwRoleID, DWORD dwType, INT nSilver) const;

	// �꾧���
	VOID GetSoulCrystalQlty(DWORD dwTypeID, BYTE & Qlty) const ;
	DWORD GetSoulCrystalSkillID(DWORD dwTypeID, BYTE byQlty) const;
	VOID GetSoulCrystalAtt(DWORD dwRoleID, DWORD dwTypeID, BYTE byQlty, OUT INT* nRetAttID, bool bFirstCreate = true, INT nRateAdd1 = 0, INT nRateAdd2 = 0,
		INT* nAttID = 0) const ;
};

//-------------------------------------------------------------------------------------
// �ű�������
//-------------------------------------------------------------------------------------
class ScriptMgr : public EventMgr<ScriptMgr>
{
public:
	//---------------------------------------------------------------------------------
	// ��ʼ��������
	//---------------------------------------------------------------------------------
	BOOL Init();
	VOID Update();
	VOID Destroy();
	
	//---------------------------------------------------------------------------------
	// ע��ű�����
	//---------------------------------------------------------------------------------
	VOID RegisterCreatureEvent(DWORD dwID, EScriptCreatureEvent eEvent, const CHAR* szFunction);
	VOID RegisterQuestEvent(UINT16 u16QuestID, EScriptQuestEvent eEvent, const CHAR* szFunction);
	VOID RegisterRoleEvent(EScriptRoleEvent eEvent, const CHAR* szFunction);
	VOID RegisterMapEvent(const CHAR* szMapName, EScriptMapEvent eEvent, const CHAR* szFunction);
	VOID RegisterActEvent(DWORD dwActID, EScriptActEvent eEvent, const CHAR* szFunction);
	VOID RegisterWorldEvent(EScriptWorldEvent eEvent, const CHAR* szFunction);
	VOID RegisterItemEvent(DWORD dwTypeID, EScriptItemEvent eEvent, const CHAR* szFunction);
	VOID RegisterSkillEvent(DWORD dwTypeID, EScriptSkillEvent eEvent, const CHAR* szFunction);	
	VOID RegisterMotionEvent(EScriptMotionEvent eEvent, const CHAR* szFunction);
	VOID RegisterBuffEvent(EScriptBuffEvent eEvent, const CHAR* szFunction);

	//---------------------------------------------------------------------------------
	// ���ɽű�����
	//---------------------------------------------------------------------------------
	const CreatureScript*	GetCreatureScript(DWORD dwCreatureID)	{ return m_mapCreatureScript.Peek(dwCreatureID); }
	const QuestScript*		GetQuestScript(UINT16 u16QuestID)		{ return m_mapQusetScript.Peek(u16QuestID); }
	const MapScript*		GetMapScript(DWORD dwMapID)				{ return m_mapMapScript.Peek(dwMapID); }
	const RoleScript*		GetRoleScript()							{ return m_pRoleScript; }
	const ActScript*		GetActScript(DWORD dwActID)				{ return m_mapActScript.Peek(dwActID); }
	const WorldScript*		GetWorldScript()						{ return m_pWorldScript; }
	const ItemScript*		GetItemScript(DWORD dwTypeID)			{ return m_mapItemScript.Peek(dwTypeID); }
	const SkillScript*		GetSkillScript(DWORD dwTypeID)			{ return m_mapSkillScript.Peek(dwTypeID); }
	const MotionScript*     GetMotionScript()                       { return m_pMotionScript; }
	const BuffScript*		GetBuffScript()							{ return m_pBuffScript; }
	//---------------------------------------------------------------------------------
	// ���ýű�
	//---------------------------------------------------------------------------------
	VOID	CallScriptFunction(CHAR* szFunction, CHAR* szFormat, ...);

	//---------------------------------------------------------------------------------
	// ���ɽű���
	//---------------------------------------------------------------------------------
	DWORD	CreateScriptMutex();

	//---------------------------------------------------------------------------------
	// ��ס���ĳ���ű���
	//---------------------------------------------------------------------------------
	VOID	LockScriptMutex(DWORD dwMutexID);
	VOID	UnLockScriptMutex(DWORD dwMutexID);

	//---------------------------------------------------------------------------------
	// ��ȡȫ�ֱ���
	//---------------------------------------------------------------------------------
	template<typename T>
	VOID	GetGlobal(LPCSTR strVarName, OUT T &res);

private:
	//---------------------------------------------------------------------------------
	// ע���첽�¼�����
	//---------------------------------------------------------------------------------
	static VOID	RegisterScriptEventFunc();

	//---------------------------------------------------------------------------------
	// �첽�¼�������
	//---------------------------------------------------------------------------------
	VOID	OnReload(DWORD dwSenderID, LPVOID pEventMessage);

	//---------------------------------------------------------------------------------
	// ��ʼ��
	//---------------------------------------------------------------------------------
	VOID	LoadScripts();
	VOID	LoadScriptsFromDir(LPCTSTR szDir, std::vector<tstring>& luaFiles, BOOL bFirst=FALSE);
	VOID	RegisterCoreFunctions();

	//-----------------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------------
	VOID	DestroyScripts();
	VOID	DestroyAllStates();
	VOID	UnloadScripts();

	//-----------------------------------------------------------------------------------
	// ���¼��ؽű�
	//-----------------------------------------------------------------------------------
	VOID	Reload();
	
	//-----------------------------------------------------------------------------------
	// �õ�һ���߳�״̬
	//-----------------------------------------------------------------------------------
	lua_State* GetThreadState();
	lua_State* CreateThreadState(DWORD dwThreadID);

	//-----------------------------------------------------------------------------------
	// ������ʾ
	//-----------------------------------------------------------------------------------
	VOID ErrMsg(lua_State* pState);
public:
	VOID MemMsg();
	
private:
	typedef TMap<UINT16, QuestScript*>			QuestScriptMap;
	typedef TMap<DWORD, CreatureScript*>		CreatureScriptMap;
	typedef TMap<DWORD, MapScript*>				MapScriptMap;
	typedef TMap<DWORD, ActScript*>				ActScriptMap;
	typedef TMap<DWORD, ItemScript*>			ItemScriptMap;
	typedef TMap<DWORD, SkillScript*>			SkillScriptMap;

private:
	TObjRef<Util>					m_pUtil;
	Mutex							m_Lock;					// ���������߳�״̬����

	lua_State*						m_pMasterState;			// ��״̬
	TSafeMap<DWORD, lua_State*>		m_mapThreadState;		// �����߳�״̬

	TMap<DWORD, Mutex*>				m_mapScriptMutex;		// �ű�����������Ҫ�������еĽű�����
	DWORD							m_dwScriptMutexIDGen;	// �ű���ID������

	QuestScriptMap					m_mapQusetScript;		// ����ű�������ID---�ű���
	CreatureScriptMap				m_mapCreatureScript;	// ����ű�������ID�����ű���
	MapScriptMap					m_mapMapScript;			// ��ͼ�ű�����ͼ����ID�����ű���
	ActScriptMap					m_mapActScript;			// ��ű����ID�����ű���
	ItemScriptMap					m_mapItemScript;		// ��Ʒ�ű�����ƷTypeID�����ű�)
	SkillScriptMap					m_mapSkillScript;		// ���ܽű�
    
	MotionScript*                   m_pMotionScript;        // ���Զ��������¼��ű�
	BuffScript*						m_pBuffScript;			// Buff�����¼��ű�
	WorldScript*					m_pWorldScript;			// ��Ϸ����ű�
	RoleScript*						m_pRoleScript;			// ��ҽű�
	DWORD							m_dwMaxPcallTime;		// �ű����õ��ʱ��
};


//-----------------------------------------------------------------------------------
// ���ɽű���
//-----------------------------------------------------------------------------------
inline DWORD ScriptMgr::CreateScriptMutex()
{
	Mutex* pMutex = new Mutex;

	++m_dwScriptMutexIDGen;
	m_mapScriptMutex.Add(m_dwScriptMutexIDGen, pMutex);

	return m_dwScriptMutexIDGen;
}

//------------------------------------------------------------------------------------
// ��סĳ���ű���
//------------------------------------------------------------------------------------
inline VOID ScriptMgr::LockScriptMutex(DWORD dwMutexID)
{
	Mutex* pMutex = m_mapScriptMutex.Peek(dwMutexID);

	if( P_VALID(pMutex) )
	{
		pMutex->Acquire();
	}
}

//-------------------------------------------------------------------------------------
// ���ĳ���ű���
//-------------------------------------------------------------------------------------
inline VOID ScriptMgr::UnLockScriptMutex(DWORD dwMutexID)
{
	Mutex* pMutex = m_mapScriptMutex.Peek(dwMutexID);

	if( P_VALID(pMutex) )
	{
		pMutex->Release();
	}
}

//---------------------------------------------------------------------------------
// ��ȡȫ�ֱ���
//---------------------------------------------------------------------------------
template<typename INT>
VOID ScriptMgr::GetGlobal(LPCSTR strVarName, OUT INT &res)
{
	lua_State* pThreadState = GetThreadState();
	if( !P_VALID(pThreadState) ) return;

	m_Lock.Acquire();

	lua_getglobal(pThreadState, strVarName);
	if( lua_isnil(pThreadState, 1) )
	{
		IMSG(_T("Script Error: unknown Global Int Val%s\r\n"), m_pUtil->AnsiToUnicode(strVarName));
		lua_pop(pThreadState, 1);
		return;
	}
	res = luaL_checkint(pThreadState, 1);

	lua_pop(pThreadState, 1);

	m_Lock.Release();
}

extern ScriptMgr g_ScriptMgr;

#define ParseINT64(n64ID, nHigh, nLow) \
{\
	const INT32 nMask	= 0xFFFFFFFF; \
	nHigh	= (INT32)((n64ID >> 32) & nMask); \
	nLow	= (INT32)(n64ID & nMask); \
}
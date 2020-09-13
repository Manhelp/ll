//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res.h
// author: Sxg
// actor:
// data: 2008-05-28
// last:
// brief: ��Դ����������
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/role_data_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/buff_define.h"
#include "../WorldDefine/trigger_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/MapAttDefine.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/variable_len.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/stall_define.h"
#include "../WorldDefine/family_define.h"
#include "../ServerDefine/pet_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/title_define.h"
#include "../ServerDefine/instance_define.h"
#include "../ServerDefine/att_res_define.h"
#include "../WorldDefine/pet_define.h"
#include "../ServerDefine/guild_define.h"
#include "famehall_part.h"
#include "../ServerDefine/item_define.h"
#include "../WorldDefine/master_apprentice.h"
#include "../worlddefine/questdef.h"
#include "../Worlddefine/quest_board.h"
#include "quest_board.h"
#include "../ServerDefine/consolidate_define.h"
#include "wardrobe.h"
#include "..\WorldDefine\pulse_define.h"
#include "..\WorldDefine\msg_serverinfo.h"

struct	tagMotionProto;
struct	tagPetProto;
struct	tagPetSkillProto;
struct	tagPetEquipProto;
struct	tagPetGatherProto;
struct	tagPetWuXingProto;
struct	tagVNBEquipProto;
struct	tagVNBGiftProto;
struct	tagItemProto;
struct	tagShopProto;
struct	tagDakProto;
struct	tagSuitProto;
struct	tagMallItemProto;
struct	tagMallPackProto;
struct	tagSSpawnPointProto;
struct	tagGuildFacilities;
struct	tagGuildUpgradeNeed;
struct	tagCofCSellGood; 
struct	tagCofCBuyGood;
struct	tagCityProto;
struct	tagMiracleSkill;
struct	tagMasterApprenticeProto;
struct  tagTalentAvailCost;
struct  tagWuXunProto;
struct  tagQuestUnimpededToken;
struct	tagBoardQuestProto;
struct	tagFConsolidateLuckySignPct;
struct  tagFAttachSoulProto;
struct  tagRecommendingItemInfo;
struct	tagClergyProto;
struct	tagGodAreaProto;
struct	tagExtractProto;
struct	tagFamilySpriteProto;
struct  tagSpriteTrainProto;
struct  tagSoulAttProto;

const INT                   MAX_WUXUN_LEVEL        = 10;                // ��ɫ�����ѫ�ȼ�
class	Quest;

//-----------------------------------------------------------------------------
class AttRes
{
public:
	typedef VOID* (WINAPI* Fun_p)(LPVOID, LPCTSTR, INT32);

public:
	AttRes();
	BOOL Init();
	VOID Destroy();

public: // �ϲ�ȷ������ -- ������ڵ�ͼ�̵߳��ϲ��߳�ִ��
	// ���¼���ָ�������ļ�
	BOOL ReloadItemProto();

	// ���¼����̳�����
	BOOL ReloadMallProto();

	// ���¼�����֮��������
	BOOL ReloadGodBidProto();

	// ���¼����ҽ𵰸��ʱ�
	BOOL ReLoadGoldEggPctProto();

	// ���¼�����Ҫ��log����Ʒid -- item_needlog��
	// ����ϢtagNDBC_LoadItemNeedLog����

public:
	// ����TypeID���������Ƿ��¼log
	VOID ResetItemLog(DWORD dwTypeID[], INT32 nNum);

public:
	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagLevelUpEffect* GetLevelUpEffect(INT32 nLevel) const { return &m_LevelUpEffect[nLevel]; }
	const tagWuXunProto&  GetWuXunProto(INT nIndex)const   {  ASSERT(nIndex <= MAX_WUXUN_LEVEL && nIndex > -1); return m_WuXunProto[nIndex]; }
	//-------------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------------
     tagTalentAvailCost* GetTalentAvailCost(INT32 nTalentNum) { return m_mapTalentAvailCost.Peek(nTalentNum);}
     
	 //-------------------------------------------------------------------------------------------
	 // ����ͼ��Buff
	 //-------------------------------------------------------------------------------------------
	 tagMapBuffInfo*  GetMapBuffInfo(DWORD dwMapID)      {  return m_mapMapBuffInfo.Peek(dwMapID); }  
	 //-------------------------------------------------------------------------------------------
	 // ���ֿ���
	 //-------------------------------------------------------------------------------------------
	 bool				GetServerFunctionSwitch(EServerFunctionSwitch eVal);
	 DWORD				GetServerFunctionSwitch();
	 //-------------------------------------------------------------------------------------------
	 // Ĭ��ֵ
	 //-------------------------------------------------------------------------------------------
	INT GetAttDefRole(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nDefRole; }
	INT GetAttDefCreature(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nDefCreature; }
	INT GetAttMin(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nMin; }
	INT GetAttMax(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nMax; }

	//-------------------------------------------------------------------------------------------
	// ���˴ʱ�
	//-------------------------------------------------------------------------------------------
	std::vector<tstring>* GetNameFilterWords() 	{ return &m_vectNameFilter; }
	//std::vector<tstring>* GetChatFilterWords() 	{ return &m_vectChatFilter; }

	//-------------------------------------------------------------------------------------------
	// ��ͬ���԰汾���Ƴ���
	//-------------------------------------------------------------------------------------------
	const tagVariableLen& GetVariableLen() const { return m_VarLen; }

	//-------------------------------------------------------------------------------
	// ��ȡȨ�ޱ�
	//-------------------------------------------------------------------------------
	const tagGuildPower& GetGuildPower(EGuildMemberPos eGuildPos) const
	{
		return m_GuildPowerDefault[eGuildPos];
	}

	const tagGuildKick& GetGuildKick(EGuildMemberPos eGuildPos) const
	{
		return m_GuildKickDefault[eGuildPos];
	}

	const tagGuildAppoint& GetGuildAppoint(EGuildMemberPos eGuildPos) const
	{
		return m_GuildAppointDefault[eGuildPos];
	}

	//-------------------------------------------------------------------------------
	// ��ȡ������ʩ����������Ϣ
	//-------------------------------------------------------------------------------
	BOOL GetGuildUpgradeBaseInfo(BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo);
	BOOL GetGuildUpgradeItemInfo(BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo);

	//-------------------------------------------------------------------------------
	// ��ȡ����������Ϣ
	//-------------------------------------------------------------------------------
	const tagGuildAffairInfo* GetGuildAffairInfo(DWORD dwBuffID)	{ return m_GuildAffairInfo.Peek(dwBuffID); }

	//-------------------------------------------------------------------------------
	// ��ȡ���ɼ�����Ϣ
	//-------------------------------------------------------------------------------
	BOOL GetGuildSkillInfo(DWORD dwSkillID, INT nLevel, tagGuildSkill& sGuildSkill);
	BOOL LoadGuildSkillInfo(TMap<DWORD, tagGuildSkill*>& mapGuildSkill);
	const tagGuildSkill* GetGuildSkillProto(DWORD dwSkillID)		{ return m_GuildSkillInfo.Peek(dwSkillID); }
	VOID InitGuildSkillMaxLvlInfo();			// ��ʼ�����Ἴ������map
	INT GetGuildSkillMaxLvl(DWORD dwSkillID);	// ȡ��󹫻Ἴ�����ȼ���û�з���-1

	//-------------------------------------------------------------------------------
	// ��ȡ����������Ϣ
	//-------------------------------------------------------------------------------
	const tagCommerceInfo*		GetGuildCommerceInfo(INT nLevel);
	const tagCommodityProto*	GetCommodityProto(DWORD dwGoodID)	{ return m_GuildCommodityProto.Peek(dwGoodID); }
	const tagCofCSPProto*		GetCofCSPProto(DWORD dwCofCID)		{ return m_CofCSPProto.Peek(dwCofCID); }
	const FLOAT					GetCofCProfit(DWORD dwDstID, DWORD dwSrcID);
	BOOL LoadCofCGoodInfo(DWORD dwCofCID, TMap<DWORD, tagCofCSellGood*>& mapGoodSell,
		TMap<DWORD, tagCofCBuyGood*>& mapGoodBuy);
	
	//-------------------------------------------------------------------------------------------
	// ��Ʒ
	//-------------------------------------------------------------------------------------------
	tagItemProto* GetItemProto(DWORD dwTypeID)			{ return m_mapItemProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// װ��
	//-------------------------------------------------------------------------------------------
	tagEquipProto* GetEquipProto(DWORD dwTypeID)		{ return m_mapEquipProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// ����(��ʯ,ӡ��,�����)
	//-------------------------------------------------------------------------------------------
	tagGemProto* GetGemProto(DWORD dwTypeID)			{ return m_mapGemProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// �̵�
	//-------------------------------------------------------------------------------------------
	tagShopProto* GetShopProto(DWORD dwShopID)			{ return m_mapShopProto.Peek(dwShopID); }

	//-------------------------------------------------------------------------------------------
	// �̳���Ʒ, ��Ʒ���������Ʒ
	//-------------------------------------------------------------------------------------------
	const tagMallItemProto* GetMallItemProto(DWORD dwID){ return m_mapMallItemProto.Peek(dwID); }
	const tagMallPackProto* GetMallPackProto(DWORD dwID){ return m_mapMallPackProto.Peek(dwID); }
	const tagMallFreeItem*	GetMallFreeProto()			{ return &m_MallFreeItemProto; }
	const tagRecommendingItemInfo* GetMallRecommendingItemInfo(DWORD dwID) { return  m_mapMallRecommendingItemInfo.Peek(dwID);}
	const DWORD				GetMallCfg(EMallCfgAttr eAtt){ return (EMCA_End == eAtt) ? 0 : m_dwMallCfg[eAtt]; }
	
	TMap<DWORD, tagMallItemProto*>& GetMallItem()		 { return m_mapMallItemProto; }
	TMap<DWORD, tagMallPackProto*>& GetMallPack()		 { return m_mapMallPackProto; }
	TMap<DWORD, tagRecommendingItemInfo*>& GetMallRecommendingItem() { return m_mapMallRecommendingItemInfo; }

	INT	GetMallItemNum()								{ return m_mapMallItemProto.Size(); }
	INT GetMallPackNum()								{ return m_mapMallPackProto.Size(); }
	INT GetMallRecommendingItemNum()                    { return m_mapMallRecommendingItemInfo.Size(); }

	//-------------------------------------------------------------------------------------------
	// ��վ
	//-------------------------------------------------------------------------------------------
	tagDakProto* GetDakProto(DWORD dwDakID)				{ return m_mapDakProto.Peek(dwDakID); }
	//-------------------------------------------------------------------------------------------
	// װ��Ʒ����������
	//-------------------------------------------------------------------------------------------
	tagEquipQltyPct* GetEquipQltyPct(DWORD dwTypeID)	{ return m_mapEquipQltyPct.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// ��װ��̬����
	//-------------------------------------------------------------------------------------------
	const tagSuitProto* GetSuitProto(DWORD dwSuitID)	{ return m_mapSuitProto.Peek(dwSuitID); }
	//-------------------------------------------------------------------------------------------
	// Ʒ��װ�����Բ���
	//-------------------------------------------------------------------------------------------
	const tagEquipQltyEffect* GetEquipQltyEffect(INT32 nQlty) const { return &m_EquipQltyEffect[nQlty]; }

	//-------------------------------------------------------------------------------------------
	// Ԫ����Ϣ����
	//-------------------------------------------------------------------------------------------
	const tagSoulAttProto* GetSoulAttProto(DWORD dwID) { return m_mapSoulAttProto.Peek(dwID); }
	//-------------------------------------------------------------------------------------------
	// Ԫ��װ��ǿ������
	//-------------------------------------------------------------------------------------------
	const tagStarEnhanceProto* GetStarEnhanceProto(BYTE byID) { return m_mapStarEnhanceProto.Peek(byID); }

	//-------------------------------------------------------------------------------------------
	// Ԫ��ת������
	//-------------------------------------------------------------------------------------------
	const tagHolySoulReborn* GetSoulRebornProto(BYTE byNum) { return m_mapSoulRebornProto.Peek(byNum); }

	// Ԫ���������
	//-------------------------------------------------------------------------------------------
	const tagHolySoulEquipHole* GetSoulEquipHoleProto(BYTE byID) { return m_mapSoulEquipHoleProto.Peek(byID); }

	// Ԫ����Ƕ����
	//-------------------------------------------------------------------------------------------
	const tagHolySoulEquipInlay* GetSoulEquipInlayProto(BYTE byID) { return m_mapSoulEquipInlayProto.Peek(byID); }

	//-------------------------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------------------------
	const tagLongHunProto* GetLongHunProto(DWORD dwID) { return m_mapLongHunProto.Peek(dwID); }
	
	const std::vector<DWORD>& GetLongHunSpecVect(INT nType, INT nTypeEx, INT nQlty) const
	{
		return m_vectLongHun[nType][nTypeEx][nQlty];
	}

	//-------------------------------------------------------------------------------------------
	// ʱװƷ��������Ӱ�����
	//-------------------------------------------------------------------------------------------
	const tagFashionGen* GetFashionQltyEffect(INT32 nQlty) const { return &m_FashionGen[nQlty]; }

	//-------------------------------------------------------------------------------------------
	// ʱװ��ɫ���ɸ��ʲ���
	//-------------------------------------------------------------------------------------------
	const tagFashionColorPct* GetFashionColorPct(INT32 nQlty) const { return &m_FashionColorPct[nQlty]; }
	
	//-------------------------------------------------------------------------------------------
	// �õ�����Ҫ��������Ʒ��ָ���б�
	//-------------------------------------------------------------------------------------------
	TList<tagItemProto*> GetItemProtoList();
	//-------------------------------------------------------------------------------------------
	// �õ��������Զ�Ӧ��װ���Ƿ��ǿ��
	//-------------------------------------------------------------------------------------------
	BOOL IsPosyPos(EPosyAtt ePosyAtt, EEquipPos eEquipPos);
	//-------------------------------------------------------------------------------------------
	// �õ��Կ����Զ�Ӧ��װ���Ƿ��ǿ��
	//-------------------------------------------------------------------------------------------
	BOOL IsEngravePos(EEngraveAtt ePosyAtt, EEquipPos eEquipPos);

	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagSkillProto* GetSkillProto(DWORD dwTypeID) { return m_mapSkillProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ״̬
	//-------------------------------------------------------------------------------------------
	const tagBuffProto* GetBuffProto(DWORD dwTypeID) { return m_mapBuffProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------------------------
	const tagTriggerProto* GetTriggerProto(DWORD dwTriggerID) { return m_mapTriggerProto.Peek(dwTriggerID); }

	//-------------------------------------------------------------------------------------------
	// ĳ�������Ƿ��ܱ���������Ӱ��
	//-------------------------------------------------------------------------------------------
	BOOL CanBeModified(DWORD dwSkillID) { return m_mapSkillModify.IsExist(dwSkillID); }

	//-------------------------------------------------------------------------------------------
	// �õ���Ӱ��ĳ�����ܵļ����б�
	//-------------------------------------------------------------------------------------------
	tagSkillModify* GetSkillModifier(DWORD dwSkillID) { return m_mapSkillModify.Peek(dwSkillID); }

	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagCreatureProto* GetCreatureProto(DWORD dwTypeID) { return m_mapCreatureProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ����AI
	//-------------------------------------------------------------------------------------------
	const tagCreatureAI* GetCreatureAI(DWORD dwAIID) { return m_mapCreatureAI.Peek(dwAIID); }

	//-------------------------------------------------------------------------------------------
	// �����������ͼˢ�ֵ�
	//-------------------------------------------------------------------------------------------
	const tagSSpawnPointProto *GetSSpawnPointProto(DWORD dwSpawnPointID) { return m_mapSSpawnPoint.Peek(dwSpawnPointID); }

	//-------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�
	//-------------------------------------------------------------------------------------------
	const tagRandSpawnPointInfo* GetSpawnPointProto(DWORD dwSpawnPoint) { return m_mapSpawnPointProto.Peek(dwSpawnPoint); }

	//-------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�
	//-------------------------------------------------------------------------------------------
	const tagInstance*	GetInstanceProto(DWORD dwMapID)	{ return m_mapInstanceProto.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// �����в���ʹ�õ���Ʒ
	//-------------------------------------------------------------------------------------------
	const tagInstanceItem* GetInstanceItem(DWORD dwMapID) { return m_mapInstanceItem.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// �����в���ʹ�õļ���
	//-------------------------------------------------------------------------------------------
	const tagInstanceSkill* GetInstanceSkill(DWORD dwMapID) { return m_mapInstanceSkill.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�ȼ�ӳ���
	//-------------------------------------------------------------------------------------------
	const tagLevelMapping* GetLevelMapping(INT nLevel) { return m_mapLevelMapping.Peek(nLevel); }

	//-------------------------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------------------------
	const tagPosyProtoSer* GetPosyProto(DWORD dwPosyID) { return m_mapPosyProto.Peek(dwPosyID); }

	//-------------------------------------------------------------------------------------------
	// �Կ�
	//-------------------------------------------------------------------------------------------
	const tagEngraveProtoSer* GetEngraveProto(DWORD dwEngraveID) { return m_mapEngraveProto.Peek(dwEngraveID); }

	//-------------------------------------------------------------------------------------------
	// ��Ƕ,��ӡ,����
	//-------------------------------------------------------------------------------------------
	const tagConsolidateItem* GetConsolidateProto(DWORD dwTypeID) { return m_mapConsolidateProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// �ϳ�
	//-------------------------------------------------------------------------------------------
	const tagProduceProtoSer* GetProduceProto(DWORD dwFormulaID) { return m_mapProduceProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// �㻯,װ���ֽ�
	//-------------------------------------------------------------------------------------------
	const tagDeComposeProtoSer* GetDeComposeProto(DWORD dwFormulaID) { return m_mapDeComposeProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// ��ȡװ��
	//-------------------------------------------------------------------------------------------
	const tagExtractProto* GetExtractProto(DWORD dwEquipTypeID) { return m_mapExtractProto.Peek(dwEquipTypeID); }

	//-------------------------------------------------------------------------------------------
	// ���
	//-------------------------------------------------------------------------------------------
	const tagQuenchProtoSer *GetQuenchProto(DWORD dwFormulaID) { return m_mapQuenchProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// �ƺ�
	//-------------------------------------------------------------------------------------------
	const tagTitleProto *GetTitleProto(UINT16 u16TitleID) { return &m_TitleProto[u16TitleID]; }

	//-------------------------------------------------------------------------------------------
	// �����䱦
	//-------------------------------------------------------------------------------------------
	const tagClanTreasureProto *GetClanTreasureProto(UINT16 u16TreasureID) { return &m_ClanTreasureProto[u16TreasureID]; }

	VOID GetRandVNBEquipProto(std::list<tagVNBEquipProto*>& listEquips)	
	{	
		m_mapVNBEquipProto.ExportAllValue(listEquips);
	}

	const tagVNBGiftProto*	GetRandVNBGiftProto()	
	{	
		tagVNBGiftProto* pProto = NULL;
		DWORD dwId = GT_INVALID;
		if (!m_mapVNBGiftProto.RandPeek(dwId, pProto) || !P_VALID(pProto))
		{
			return NULL;
		}
		else
		{
			return pProto;
		}
	}

	const tagCreatureAI* RandGetCreatureAI()
	{
		DWORD dwAIID = GT_INVALID;
		tagCreatureAI* pAI = NULL;

		m_mapCreatureAI.RandPeek(dwAIID, pAI);

		return pAI;
	}
	const tagCreatureAI* RandGetCreatureAIInGroup(DWORD dwGroupID)
	{
		TList<DWORD>* pList = m_mapCreatureAIGroup.Peek(dwGroupID);
		if( P_VALID(pList) && pList->Size() > 0 )
		{
			DWORD dwAIID = GT_INVALID;
			pList->RandPeek(dwAIID);

			return m_mapCreatureAI.Peek(dwAIID);
		}
		else
		{
			return NULL;
		}
	}

	//-------------------------------------------------------------------------------------------
	// ��ó�������Ĭ����С���ֵ
	//-------------------------------------------------------------------------------------------
// 	INT GetPetDef(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nDef;	}
// 	INT GetPetMin(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nMin;	}
// 	INT GetPetMax(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nMax;	}

	//-------------------------------------------------------------------------------------------
	// ��ó���ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetProto* GetPetProto(DWORD dwPetTypeID) { return m_mapPetProto.Peek(dwPetTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó�������ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetLvlUpProto* GetPetLvlUpProto(DWORD dwVLevel) { return m_mapPetLvlUpProto.Peek(dwVLevel); }

	//-------------------------------------------------------------------------------------------
	// ��ó�������ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetLvlUpItemProto* GetPetLvlUpItemProto(DWORD dwTypeID) { return m_mapPetLvlUpItemProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó��＼��ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetSkillProto* GetPetSkillProto(DWORD dwPetSkillTypeID) { return m_mapPetSkillProto.Peek(dwPetSkillTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó���װ��ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetEquipProto* GetPetEquipProto(DWORD dwPetEquipTypeID) { return m_mapPetEquipProto.Peek(dwPetEquipTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó�����������ԭ��
	//-------------------------------------------------------------------------------------------
	const tagPetWuXingProto* GetPetWuXingProto(DWORD dwPetWuXingTypeID) { return m_mapPetWuXingProto.Peek(dwPetWuXingTypeID); }

	//-------------------------------------------------------------------------------------------
	// ��ó��＼������
	//-------------------------------------------------------------------------------------------
	const std::list<DWORD>& GetPetNormalSkillList(INT nPetLevel) { return m_PetLevelSkillVec[nPetLevel - 1]; }

	//-------------------------------------------------------------------------------------------
	// VIP̯λ��Ϣ
	//-------------------------------------------------------------------------------------------
	const INT32 GetVIPStallRent(INT nIndex) { return m_nVIPStallRent[nIndex]; }

	//-------------------------------------------------------------------------------------------
	// ��ö���ԭ��
	//-------------------------------------------------------------------------------------------
	const tagMotionProto* GetMotionProto(DWORD dwTypeID) { return m_mapMotionProto.Peek(dwTypeID); }

	
	//-------------------------------------------------------------------------------------------
	// ��ȡ����������ߴ���ԭ�ͱ�
	//-------------------------------------------------------------------------------------------
	const tagQuestUseSpeciItem * GetQuestUseSpeciItem(INT16 n16QuestID) {return m_mapQuestUseSpeciItemProto.Peek(n16QuestID); }


	//-------------------------------------------------------------------------------------------
	// ��ȡ�񼣻�ɱ���ֻ�þ���ԭ�ͱ�
	//-------------------------------------------------------------------------------------------
	const tagKillerExp*	GetKillObjExpProto(INT16 n16Level)	{ return m_mapKillObjExp.Peek(n16Level); }
	//-------------------------------------------------------------------------------------------
	// ����������Ϣ
	//-------------------------------------------------------------------------------------------
	const tagCityProto* GetCityProto(DWORD dwCityID)	{ return m_mapCityProto.Peek(dwCityID); }
	VOID GetCityIDRange(DWORD& dwMinID, DWORD& dwMaxID);

	//-------------------------------------------------------------------------------------------
	// ��ȡʦͽ����ԭ�ͱ�
	//-------------------------------------------------------------------------------------------
	const tagMasterApprenticeProto* GetMasterApprenticeProto(DWORD dwLevelID)	{ return m_mapMasterApprenticeProto.Peek(dwLevelID); }

	//-------------------------------------------------------------------------------------------
	// ��֮����ϵͳ
	//-------------------------------------------------------------------------------------------
	const tagTrainAttProto* GetOneTrainAttProto(DWORD dwTrainID)	{ return m_mapTrainAttProto.Peek(dwTrainID); }
	TMap<DWORD, tagTrainAttProto*>& GetTrainAttProto()	{ return m_mapTrainAttProto; }

	//-------------------------------------------------------------------------------------------
	// ��ȡһ����ɫ��������ԭ��
	//-------------------------------------------------------------------------------------------
	TMap<DWORD, tagInitialSkill*>& GetInitialSkillMap()	{ return m_mapInitialSkill; }
	tagInitialSkill* GetInitialSkillProto(DWORD dwID)		{ return m_mapInitialSkill.Peek(dwID); }
	DWORD GetInitialSkillCount()		{ return m_mapInitialSkill.Size(); }

	// ����Ƶ������
	DWORD GetChatFlag()const {return m_dwChatChannelFlag; }

	//-------------------------------------------------------------------------------------------
	// �ɽ缼��
	//-------------------------------------------------------------------------------------------
	const tagMiracleSkill* GetMSkillProto(DWORD dwSkillTypeID)	{ return m_mapMiracleSkillProto.Peek(dwSkillTypeID); }

	//-------------------------------------------------------------------------------------------
	// ���ĸ���
	//-------------------------------------------------------------------------------------------
	const tagSpinSuccessRateProto* GetSpinRateProto(DWORD dwItemNum);

	//-------------------------------------------------------------------------------------------
	// ����ԭ��
	//-------------------------------------------------------------------------------------------
	const tagSpinProto*	GetSpinProto(tagSpinKey key) { return m_mapSpinProto.Peek(key); }	

private:
	//-------------------------------------------------------------------------------------------
	// ���ó�ʼ��ĳ����Դ��һ����¼�Ĵ�����
	//-------------------------------------------------------------------------------------------
	VOID SetInitOneRes(Fun_p pFun) { m_pFun = pFun; }

	//-------------------------------------------------------------------------------------------
	// ��ʼ����Դ��ģ�溯�� -- ����map��
	//-------------------------------------------------------------------------------------------
	template<class K, class T> 
	BOOL LoadResMap(TMap<K, T*> &mapRes, LPCTSTR szFileName, LPCTSTR szFileName2 = NULL);

	//-------------------------------------------------------------------------------------------
	// �ͷ���Դ��ģ�溯��
	//-------------------------------------------------------------------------------------------
	template<class K, class T> VOID FreeResMap(TMap<K, T*> &mapRes);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����Դ��ģ�溯�� -- ����array��
	//-------------------------------------------------------------------------------------------
	template<class T>
	BOOL LoadResArray(T *arrayRes, INT32 nIndexStart, INT32 nIndexEnd, LPCTSTR szFileName);

private:
	//-------------------------------------------------------------------------------------------
	// ��ʼ�����˴ʱ�
	//-------------------------------------------------------------------------------------------
	BOOL InitFilterWords(OUT vector<tstring>& vectFilterWords, LPCTSTR szFileName);
	BOOL InitChatFilterWords(OUT vector<tstring>& vectFilterWords, LPCTSTR szFileName);

	//-------------------------------------------------------------------------------------------
	// �ӽű��������л�ȡ��������
	//-------------------------------------------------------------------------------------------
	VOID InitVarLen();

private:

	//-------------------------------------------------------------------------------------------
	// ���ع������ʵ���Ӧ��������
	//-------------------------------------------------------------------------------------------
	VOID* InitTalentAvailCost(OUT LPVOID pData, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// �����ܽ�ָ����ͼ��buff����
	//-------------------------------------------------------------------------------------------
	VOID* InitMapBuffInfo(OUT LPVOID pData, IN LPCTSTR szField, INT32 nDummy);
    
	//-------------------------------------------------------------------------------------------
	// ��ʼ����Ʒ��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��װ����һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneEquipProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����ʯ�ȵ�һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneGemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����װ��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSuitProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��һ��װ��Ʒ����������
	//-------------------------------------------------------------------------------------------
	VOID* InitOneEquipQltyPct(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��Ԫ��������Ա�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulAttProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	//-------------------------------------------------------------------------------------------
	// ��ʼ��Ԫ��װ��ǿ�����Ա�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneStarEnhanceProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	//��ʼ��Ԫ��ת�����Ա�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulRebornProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	//��ʼ��Ԫ��������Ա�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulEquipHoleProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	//-------------------------------------------------------------------------------------------
	//��ʼ��Ԫ����Ƕ���Ա�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulEquipInlayProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ������������
	//-------------------------------------------------------------------------------------------
	VOID* InitOneLongHunProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̵�
	//-------------------------------------------------------------------------------------------
	VOID* InitOneShopProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̳���Ʒ����Ʒ��
	//-------------------------------------------------------------------------------------------
	VOID  InitOneMallItemProtoBase(OUT LPVOID pProtoType, IN LPCTSTR szField);
	VOID* InitOneMallItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	VOID* InitOneMallPackProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	VOID* InitOneMallRecommendingItemInfo(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����վ
	//-------------------------------------------------------------------------------------------
	VOID* InitOneDakProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ�����ܵ�һ����¼
	//-------------------------------------------------------------------------------------------
    VOID* InitOneSkillProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��buff��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneBuffProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��trigger��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneTriggerProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��Creature��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCreatureProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��Creature_ai��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCreatureAIProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����Ƶ�һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePosyProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ���Կ̵�һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOneEngraveProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����Ƕ,��ӡ,������һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOneConsolidateProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������ܵ�һ����¼
	//--------------------------------------------------------------------------------------------
	VOID* InitOneProduceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ���,ͨ�÷ֽ�
	//--------------------------------------------------------------------------------------------
	VOID* InitOneDeComposeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ȡ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneExtractProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneQuenchProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// �����в���ʹ�õ���Ʒ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInsItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// �����в���ʹ�õļ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInsSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// �������ˢ�ֵ�
	//--------------------------------------------------------------------------------------------
	VOID* InitOneSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ������̬ˢ�ֵ�ȼ�ӳ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneLevelMapping(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ��������̬����
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInstanceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ���Ǹ���ˢ�ֵ�ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOneSSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����������Ĭ����С���ֵ
	//--------------------------------------------------------------------------------------------
//	VOID InitPetAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����������ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetLvlUpProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ʒԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetLvlUpItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����＼��ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������װ��ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetEquipProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ���������������ԭ��
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetWuXingProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ʒ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneVNBGiftProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ʒ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneVNBEquipProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������ɼ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetGatherProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	//--------------------------------------------------------------------------------------------
	// ��ʼ�����＼��ԭ��
	//--------------------------------------------------------------------------------------------
	VOID InitPetSkillsVec();

	//--------------------------------------------------------------------------------------------
	// ��ʼ��������ʩ����������Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildUpgradeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
    
	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildAffairProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����ɼ�����Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ������������Ϣ
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildCommerceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCommodityProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCofCProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCofCSPProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�����Զ���
	//--------------------------------------------------------------------------------------------
	VOID* InitOneMotionProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ�������������ԭ�ͱ�
	//--------------------------------------------------------------------------------------------
	VOID* InitQuestUseSpeciItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// ��ʼ����ɱ�������þ����
	//--------------------------------------------------------------------------------------------
	VOID* InitKillObjExp(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ����������
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCityProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);


	//-------------------------------------------------------------------------------------------
	// ��ʼ���ɽ缼��
	//-------------------------------------------------------------------------------------------
	VOID* InitMiracleSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��ʦͽ��������
	//-------------------------------------------------------------------------------------------
	VOID* InitOneMasterApprenticeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------------------
	// ��ʼ�����ĳɹ���
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSpinRateProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------
	// ��֮����ϵͳ-��ʼ���������Ա�
	//-------------------------------------------------------------------------------
	VOID* InitOneTrainProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy );

	//-------------------------------------------------------------------------------
	// ��������ԭ��
	//-------------------------------------------------------------------------------
	VOID* InitOneInitialSkillProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy );

	//-------------------------------------------------------------------------------------------
	// ��ʼ������ԭ��
	//-------------------------------------------------------------------------------------------
	BOOL InitSpinProto(OUT TMap<tagSpinKey ,tagSpinProto*>	&mapSpinProto, LPCTSTR szFileName);

	//-------------------------------------------------------------------------------------------
	// ����ϵͳ
	//-------------------------------------------------------------------------------------------
	VOID InitSpiritSystem();

	//-------------------------------------------------------------------------------
	// ֻ�ܷ��ڱ�������Ʒ
	//-------------------------------------------------------------------------------
	VOID InitContainerLimit();
private:
	//-------------------------------------------------------------------------------------------
	// ���������������Ӱ�쾲̬����
	//-------------------------------------------------------------------------------------------
	VOID InitRoleAttLevelUp(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ������ѫ���Ա�����
	//-------------------------------------------------------------------------------------------
	VOID IniWuXunProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ���ظ��ֿ�����������
	//-------------------------------------------------------------------------------------------
	VOID IniServerFunctionSwitch(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��������������Ե�Ĭ��ֵ����Сֵ�����ֵ
	//-------------------------------------------------------------------------------------------
	VOID InitAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ����Ʒ��װ�����Բ���
	//-------------------------------------------------------------------------------------------
	VOID InitEquipQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ����ʱװ���ɲ���
	//-------------------------------------------------------------------------------------------
	VOID InitFashionQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ����ʱװ����ʱ��ɫ����
	//-------------------------------------------------------------------------------------------
	VOID InitFashionColorPct(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ������װ����λ��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID InitOnePosyPosProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// �ƺŵ�һ����¼
	//-------------------------------------------------------------------------------------------
	VOID InitOneTitleProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// �����䱦��һ����¼
	//-------------------------------------------------------------------------------------------
	VOID InitOneClanTreasureProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̳������Ʒ(����һ��)
	//-------------------------------------------------------------------------------------------
	VOID InitOneMallFreeItemProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ���̳������ļ�
	//-------------------------------------------------------------------------------------------
	VOID InitOneMallCfgProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ������ĳ��ְλȨ��
	//-------------------------------------------------------------------------------------------
	VOID InitOnePosGuildPower(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	VOID InitOnePosGuildKick(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	VOID InitOnePosGuildAppoint(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// ��ʼ��VIP̯λ
	//-------------------------------------------------------------------------------------------
	VOID InitOneVIPStallProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

private:
	//-------------------------------------------------------------------------------------------
	// ����Ӱ��ĳһ���ܣ�ĳһ״̬��ĳһ�����������м����б�
	//-------------------------------------------------------------------------------------------
	VOID LoadModifyMap();

	//-------------------------------------------------------------------------------------------
	// ����AI����
	//-------------------------------------------------------------------------------------------
	VOID GroupCreatureAI();

	//-------------------------------------------------------------------------------------------
	// ��ʼ������������������
	//-------------------------------------------------------------------------------------------
	VOID InitLongHunAss();
	VOID InitLongHunCanActivePos();

private:
	TSFPTrunk<AttRes>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	//TObjRef<VarContainer>	m_pVar;
	Fun_p					m_pFun;

	std::vector<tstring>				m_vectNameFilter;
	std::vector<tstring>				m_vectChatFilter;
	//std::vector<tstring>				m_vectChatFilter;

	tagVariableLen						m_VarLen;
	
	TMap<DWORD, tagItemProto*>			m_mapItemProto;
	TMap<DWORD, tagEquipProto*>			m_mapEquipProto;
	//std::vector<DWORD>					m_vecLimitedItem;

	TMap<DWORD, tagGemProto*>			m_mapGemProto;
	TMap<DWORD, tagEquipQltyPct*>		m_mapEquipQltyPct;

	TMap<DWORD, tagSoulAttProto*>       m_mapSoulAttProto;
	TMap<BYTE, tagStarEnhanceProto*>	m_mapStarEnhanceProto;
	TMap<BYTE, tagHolySoulReborn*>		m_mapSoulRebornProto;
	TMap<BYTE, tagHolySoulEquipHole*>	m_mapSoulEquipHoleProto;
	TMap<BYTE, tagHolySoulEquipInlay*>	m_mapSoulEquipInlayProto;	

	TMap<DWORD, tagLongHunProto*>		m_mapLongHunProto;
	std::vector<DWORD>					m_vectLongHun[X_LONGHUN_TYPE_NUM][X_LONGHUN_TYPEEX_NUM][X_EQUIP_QUALITY_NUM]; // ���������������ṹ

	TMap<DWORD, tagShopProto*>			m_mapShopProto;
	TMap<DWORD, tagDakProto*>			m_mapDakProto;
   
	TMap<DWORD, tagMapBuffInfo*>        m_mapMapBuffInfo;   
	TMap<DWORD, tagMallItemProto*>		m_mapMallItemProto;
	TMap<DWORD, tagMallPackProto*>		m_mapMallPackProto;
	tagMallFreeItem						m_MallFreeItemProto;
    TMap<DWORD, tagRecommendingItemInfo*> m_mapMallRecommendingItemInfo;
	DWORD								m_dwMallCfg[EMCA_End];

	TMap<DWORD, tagSuitProto*>			m_mapSuitProto;

    TMap<DWORD, tagSkillProto*>			m_mapSkillProto;
	TMap<DWORD, tagBuffProto*>			m_mapBuffProto;
	TMap<DWORD, tagTriggerProto*>		m_mapTriggerProto;

	TMap<DWORD, tagSkillModify*>		m_mapSkillModify;

	TMap<DWORD, tagCreatureProto*>		m_mapCreatureProto;
	TMap<DWORD, tagCreatureAI*>			m_mapCreatureAI;
	TMap<DWORD, TList<DWORD>*>			m_mapCreatureAIGroup;

	// Jason 2009-12-7 �����͸������ļ���Ϣ
	TMap<int,	tagConfigLessingOfLoong*>	m_mapConfigInfoOfLessingLoong;

	// �������ʵĻ���
	TMap<DWORD,tagTalentAvailCost*>     m_mapTalentAvailCost; 
    
	  
	tagWuXunProto                       m_WuXunProto[MAX_WUXUN_LEVEL+1];
	tagLevelUpEffect					m_LevelUpEffect[MAX_ROLE_LEVEL+1];		// �±�͵ȼ���Ӧ
	BitSetServerFunctionSwitch			m_bsServerFunctionSwitch;				// ���ֿ��� [6/13/2012 zhangzhihua]
	tagAttDefMinMax						m_AttDefMinMax[ERA_End];
	tagEquipQltyEffect					m_EquipQltyEffect[X_EQUIP_QUALITY_NUM];
	tagFashionGen						m_FashionGen[X_EQUIP_QUALITY_NUM];
	tagFashionColorPct					m_FashionColorPct[X_EQUIP_QUALITY_NUM];

	// Ĭ�ϰ��ɳ�ԱȨ�ޱ�
	tagGuildPower						m_GuildPowerDefault[X_GUILD_POS_NUM];
	tagGuildKick						m_GuildKickDefault[X_GUILD_POS_NUM];
	tagGuildAppoint						m_GuildAppointDefault[X_GUILD_POS_NUM];

	// ������ʩ����������Ʒ
	TMap<DWORD, tagGuildUpgradeNeed*>	m_GuildUpgradeNeedInfo;

	// ��������
	TMap<DWORD, tagGuildAffairInfo*>	m_GuildAffairInfo;

	// ���ɼ���
	TMap<DWORD, tagGuildSkill*>			m_GuildSkillInfo;
	TMap<DWORD, INT>					m_mapSkillMaxLvl;

	// ��������
	TMap<DWORD, tagCommerceInfo*>		m_GuildCommerceInfo;
	TMap<DWORD, tagCommodityProto*>		m_GuildCommodityProto;
	TMap<INT64, tagCofCProto*>			m_CofCProto;
	TMap<DWORD, tagCofCSPProto*>		m_CofCSPProto;

	// ������װ����λ 
	tagConsolidatePos					m_PosyPos[EPosyAtt_End][MAX_CONSOLIDATE_POS_QUANTITY];
	TMap<DWORD, tagPosyProtoSer*>		m_mapPosyProto;			// ���ƾ�̬��
	// ���Կ�װ����λ
	tagConsolidatePos					m_EngravePos[EEngraveAtt_End][MAX_CONSOLIDATE_POS_QUANTITY];
	TMap<DWORD, tagEngraveProtoSer*>	m_mapEngraveProto;		// �Կ̾�̬��
	// ��Ƕ,��ӡ,������̬��
	TMap<DWORD, tagConsolidateItem*>	m_mapConsolidateProto;
	// �ϳ�
	TMap<DWORD, tagProduceProtoSer*>	m_mapProduceProto;
	// ���
	TMap<DWORD, tagQuenchProtoSer*>		m_mapQuenchProto;
	// �㻯,װ���ֽ�
	TMap<DWORD, tagDeComposeProtoSer*>	m_mapDeComposeProto;
	//��ȡ
	TMap<DWORD, tagExtractProto*>		m_mapExtractProto;

	// �ƺ�
	// Jason 2010-3-30
	//tagTitleProto						m_TitleProto[MAX_TITLE_NUM];
	tagTitleProto	*					m_TitleProto;
	// �����䱦
	tagClanTreasureProto				m_ClanTreasureProto[CLAN_TREASURE_NUM];
	// �������ˢ�ֵ�
	TMap<DWORD, tagRandSpawnPointInfo*> m_mapSpawnPointProto;
	// ������̬����
	TMap<DWORD, tagInstance*>			m_mapInstanceProto;
	// �����в���ʹ�õ���Ʒ
	TMap<DWORD, tagInstanceItem*>		m_mapInstanceItem;
	// �����в���ʹ�õļ���
	TMap<DWORD, tagInstanceSkill*>		m_mapInstanceSkill;
	// �������ˢ�ֵ�ȼ�ӳ���
	TMap<INT, tagLevelMapping*>			m_mapLevelMapping;
	// �Ǹ���ˢ�ֵ�
	TMap<INT, tagSSpawnPointProto*>		m_mapSSpawnPoint;

	// ����ԭ��
	TMap<DWORD, tagPetProto*>			m_mapPetProto;

	// ��������ԭ��
	TMap<DWORD, tagPetLvlUpProto*>		m_mapPetLvlUpProto;

	// ��������ԭ��
	TMap<DWORD, tagPetLvlUpItemProto*>	m_mapPetLvlUpItemProto;

	// ���＼��ԭ��
	TMap<DWORD, tagPetSkillProto*>		m_mapPetSkillProto;
	
	// ����ȼ���Ӧ�̶�����id
	std::list<DWORD>					m_PetLevelSkillVec[NUM_PET_VLEVEL];

	// ����װ��ԭ��
	TMap<DWORD, tagPetEquipProto*>		m_mapPetEquipProto;

	// ����ɼ���
	TMap<DWORD, tagPetGatherProto*>		m_mapPetGatherProto;

	// �������������
	TMap<DWORD, tagPetWuXingProto*>		m_mapPetWuXingProto;

	TMap<DWORD, tagVNBGiftProto*>		m_mapVNBGiftProto;
	TMap<DWORD, tagVNBEquipProto*>		m_mapVNBEquipProto;
	
	// �����Ӧ�������
	TMap<INT16, tagQuestUseSpeciItem *> m_mapQuestUseSpeciItemProto;
	
	// �������ɱ�������þ���
	TMap<INT16, tagKillerExp *>			m_mapKillObjExp;
	
	// ��������ֵ
//	tagPetDefMinMax						m_nPetAttDefMinMax[EPA_NUM];

	// VIP̯λ���(�±���ID��Ӧ)
	INT32								m_nVIPStallRent[VIP_STALL_MAX_NUM];

	TMap<DWORD, tagMotionProto*>		m_mapMotionProto;

	// ��������
	TMap<DWORD, tagCityProto*>			m_mapCityProto;
	
	// �ɽ缼��
	TMap<DWORD, tagMiracleSkill*>		m_mapMiracleSkillProto;

	//����ϵͳ
	BOOL								m_bSpiSysOn;						// ����ϵͳ�Ƿ���
	std::map<DWORD, float>				m_mapSpiRate;						// ������Ӧ�ļӳɰٷֱ�
	DWORD								m_dwSpiStartLevel;					// ��Ч�ļ���
	DWORD								m_dwSpiRecoverySpeed;				// ���߻ָ��ٶ� ��/����
	DWORD								m_dwSpiReward;						// �콱����
	std::map<DWORD,std::pair<int, EItemQuality> > m_mapSpiReward;			// ��ȡ�Ľ�Ʒ
	std::map<DWORD,int>					m_mapStrengthReward;				// ʵ��ֵ�һ�����

	// ʦͽ��������
	TMap<DWORD, tagMasterApprenticeProto*>		m_mapMasterApprenticeProto;

	// ��֮����ϵͳ-�������Ա�
	TMap<DWORD, tagTrainAttProto*>		m_mapTrainAttProto;

	// ��ɫ��������ԭ�ͱ�
	TMap<DWORD, tagInitialSkill*>		m_mapInitialSkill;

	// ����Ƶ������
	DWORD								m_dwChatChannelFlag;

	TMap<tagSpinKey ,tagSpinProto*>			m_mapSpinProto;				// ����

	TMap<DWORD, tagSpinSuccessRateProto*>		m_mapSpinRateProto;			// ���ĳɹ���

	std::list<DWORD>					m_dwContainerLimitedItems;

	// ��ְԭ�ͱ�
	TMap<DWORD, tagClergyProto*>		m_mapClergyProto;
	
	// ��ѡ��ְ-��ϵ����ְIDӳ������ڸ�����ϵֱ���ҵ���ϵ������ְ��ID
	TMap<ECampaignGodType, DWORD>		m_mapClergyBase;
	TMap<ECampaignGodType, DWORD>		m_mapFemaleClergyBoss;
	TMap<ECampaignGodType, DWORD>		m_mapMaleClergyBoss;

	// ��ְ����ԭ�ͱ�
	TMap<DWORD, tagGodAreaProto*>		m_mapGodAreaProto;

	// ��ͼ����ԭ�ͱ�
	TMap<DWORD, tagMapBoxProto*>		m_mapMapBoxProto;

	// ������Чԭ�ͱ�
	TMap<DWORD, tagAreaEffectProto*>	m_mapAreaEffectProto;
	// �����ж�����ϡ��������Ч
	TMap<DWORD, DWORD>					m_mapALLAreaBuffID;

	// �ػ��������Ա�
	TMap<DWORD, tagFamilySpriteProto*>	m_mapFamilySpriteProto;

	
	// �����ػ������������Ա�
	TMap<DWORD, tagSpriteTrainProto*>	m_mapSpriteTrainProto;
	TMap<DWORD, tagSpriteTrainData*>	m_mapSpriteTrainData;


public:
	std::vector<tstring>* GetChatFilterWordsVec() { return &m_vectChatFilter; }

public:
	// GM�������
	TMap<DWORD, tagEquipProto*>			GetEquipMap(){return m_mapEquipProto;}	


	// Jason 2009-12-7 �����͸������ļ���Ϣ
	tagConfigLessingOfLoong* GetConfigInfoOfLessingLoong(const int & ndx);
private:
	VOID* AttRes::InitLessingLoong(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	TMap<INT64, tagPhilosophersStoneConfig*>		m_mapPhilosophersStoneConfig;
	VOID* InitPhilosophersStoneConfig(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	TObjRef<VarContainer>	m_pVar;
public:
	tagPhilosophersStoneConfig* GetConfigInfoOfPhilosophersStone(INT nStoneType,DWORD srcItemTypeID);

	// Jason 2010-1-18 v1.3.2 ��һع����
	template<class T,class F>
	BOOL LoadResToOtherObject(T * Obj,F f,LPCTSTR szFileName);

	//����ϵͳ
	BOOL IsSpiritSysOn()const { return m_bSpiSysOn; }
	DWORD SpiritStartLevel()const { return m_dwSpiStartLevel; }	
	DWORD GetSpiritRSpeed()const { return m_dwSpiRecoverySpeed; }
	DWORD GetSpiritRewardValue()const { return m_dwSpiReward; }
	const std::map<DWORD, std::pair<int, EItemQuality> >& GetSpiritReward()const { return m_mapSpiReward; }
	const std::map<DWORD, float>&	GetSpiriteRate()const { return m_mapSpiRate; }						
	const std::map<DWORD, int>& GetStrengthReward()const { return m_mapStrengthReward; }

	// ��Ʒ�Ƿ�ֻ�ܷ��ڱ�����
	BOOL IsLimited2Bag(DWORD dwID)const { return std::binary_search(m_dwContainerLimitedItems.begin(), m_dwContainerLimitedItems.end(), dwID);}
// Jason 2010-4-13 v2.0.0����ϵͳ���
private:
	TMap<INT,tagFabaoLevelProto*>	m_mapFabaoLevelProto;		// Jason 2010-4-13 v2.0.0 �����Ƚ�ԭ��
	TMap<INT,tagFabaoWuxingProto*>	m_mapFabaoWuxingProto;		// Jason 2010-4-13 v2.0.0 ����������������ԭ��
	TMap<DWORD,tagFabaoProto*>			m_mapFabaoProto;			// Jason 2010-4-13 v2.0.0 ����ԭ��

	TMap<DWORD,tagHolyProto*>			m_mapHolyProto;			// ʥ��ԭ��
	TMap<DWORD,tagHolyLevelUpProto*>	m_mapHolyLevelUp;		// ʥ������ԭ��	
	TMap<DWORD,tagHolyEquipProto*>		m_mapHolyEquipProto;	// ʥ������
	TMap<DWORD,tagHolyEquipEnhance*>	m_mapHolyEquipEnhance;	// ʥ������ǿ��

	TMap<DWORD,tagQuestUnimpededToken*> m_mapQuestUnimpededToken;	//������������ӳ��
	// Jason 2010-5-24 v2.1.2 ��������ԭ��
	TMap<DWORD,tagLearnSoaringSkillProto *> m_mapLearnSoaringSkill;
	TMap<DWORD,tagCastingProto *> m_mapCastingProto;
	TMap<DWORD,tagMsgDeliver * > m_mapMsgDeliverProto;
	TMap<DWORD,tagSkillBoxProto*> m_mapSkillBoxProto;
	INT m_nFabaoMaxStage;

	VOID* InitOneFabaoLevelProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneFabaoWuxingProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneFabaoProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// ����ʥ�鸽������[wcy 2012-03-30]
	VOID* InitOneHolyProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// ����ʥ��������[wcy 2012-04-09]
	VOID* InitOneHolyLevelUp(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);	

	// ����ʥ�Ƹ�������[wcy 2012-04-17]
	VOID* InitOneHolyEquipProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	// ����ʥ��ǿ������[wcy 2012-04-26]
	VOID* InitOneHolyEquipEnhance(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);
		
	VOID * InitOneQuestUnimpededToken(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);
	// Jason
	VOID * InitOneLearnSoaringSkillProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);
	VOID * InitOneCastingProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);
	VOID * InitOneMsgDeliverProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);

	VOID * InitOneSkillBoxProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
public:
	const tagFabaoProto * GetFabaoProto(DWORD dwTypeID);
	const tagFabaoLevelProto * GetFabaoLevelProto(INT ID);
	const tagFabaoWuxingProto * GetFabaoWuxingProto(INT ID);
	const TMap<INT,tagFabaoWuxingProto*> & GetFabaoWuxingProtoMap() const{return m_mapFabaoWuxingProto;}
	const tagQuestUnimpededToken * GetOneUnimpededToken(DWORD dwQuestID) ;

	const tagLearnSoaringSkillProto * GetOneLearnSoaringSkillProto(DWORD dwID);
	const tagCastingProto * GetOneCastingProto(DWORD dwID);
	const TMap<DWORD,tagCastingProto *> & GetCastingProto(){return m_mapCastingProto;}
	const tagMsgDeliver * GetOneMsgDeliverProto(DWORD dwID);

	const tagSkillBoxProto * GetOneSkillProto(DWORD dwID);

	const tagHolyProto * GetHolyProto(DWORD dwTypeID);

	const tagHolyEquipProto * GetHolyEquipProto(DWORD dwTypeID);

	

// ��������
private:
	// �����������Լ���
	TMap<UINT16, tagBoardQuestProto*> m_mapBoardQuestProto;
	VOID* InitOneBoardQuest(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	// �����������ɹ���
	tagBoardQuestRandomRate	m_BoardQuestRandRule[BOARD_QUEST_MAX_RULE_NUM];
	VOID InitOneBoardQuestRandRule(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	// Jason 2010-6-18 ǿ����
	TMap<DWORD,tagFConsolidateProto*> m_mapFConsolidateProto;
	VOID * InitOneFConsolidateProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ǿ��֮���˷�
	TMap<DWORD,tagFConsolidateLuckySignPct*> m_mapFConsolidateLuckySignPct;
	VOID * InitOneFConsolidateLuckySignPct(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ����
	TMap<DWORD ,tagFAttachSoulProto*> m_mapFAttachSoulStone;
	VOID* InitOneFAttachSoulProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ����
	TMap<DWORD, tagGodStrengthen * > m_mapGodStrengthen;
	VOID* InitOneGodStrengthenProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ����ʳ��ԭ��
	TMap<DWORD, tagFabaoFoodProto*> m_mapFabaoFood;
	VOID * InitFabaoFoodProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ������ʵ
	TMap<DWORD, tagFabaoFruitProto*> m_mapFabaoFruit;
	VOID * InitFabaoFruitProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// �����ؼ�
	TMap<DWORD,tagFabaoStuntProto *> m_mapFabaoStunt;
	VOID * InitFabaoStuntProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ������������ [3/13/2012 zhangzhihua]
	TMap<DWORD,tagFabaoAddPromoteTimesProto *> m_mapFabaoAddPromoteTimes;
	VOID * InitFabaoAddPromoteTimesProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// �����ں� [3/22/2012 zhangzhihua]
	TMap<DWORD,tagFairyFusionQualityProto *> m_mapFairyFusionQuality;
	VOID * InitFairyFusionQualityProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	TMap<DWORD,tagFairyFusionTypeProto *> m_mapFairyFusionType;
	VOID * InitFairyFusionTypeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	TMap<DWORD,tagWarRewardPointProto *> m_mapWarRewardPoint;
	VOID * InitWarRewardPointProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ����ÿ���̵꽱��
	TMap<DWORD,tagGuildDailyRewardProto *> m_mapGuildDailyReward;
	VOID * InitGuildDailyRewardProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// ��֮����
	TMap<DWORD,tagGodBidProto *> m_mapGodBidProto;
	VOID * InitGodBidProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

public:
	TMap<UINT16, tagBoardQuestProto*>& GetBoardQuestMap()			{ return m_mapBoardQuestProto;}
	tagBoardQuestProto* GetBoardQuestProto(UINT16 u16ID)			{ return m_mapBoardQuestProto.Peek(u16ID); }
	tagBoardQuestRandomRate* GetBoardQuestRule(INT nIndex)			{ return &m_BoardQuestRandRule[nIndex]; }

	const tagFConsolidateProto * GetOneFConsolidateProto(DWORD dwConsID);
	TMap<DWORD,tagFConsolidateProto*> & GetFConsolidateProto() {return m_mapFConsolidateProto;}

	const tagFConsolidateLuckySignPct* GetOneFConsolidateLuckySignPct(DWORD dwID);
	const tagFAttachSoulProto* GetOneFuhunStone(DWORD dwConLevelID);
	TMap<DWORD ,tagFAttachSoulProto*> & GetFAttachSoulProto(){return m_mapFAttachSoulStone;}

	const INT GetFabaoMaxStage(VOID) const { return m_nFabaoMaxStage;}

	const tagGodStrengthen * GetGodStrengthenProto(DWORD dwLevel);
	TMap<DWORD, tagGodStrengthen * > & GetGodStrengthenMap(void){return m_mapGodStrengthen;}

	// ����ʳ�����
	const tagFabaoFoodProto* GetFabaoFoodProto(DWORD dwTypeID);
	// ����ĳ�����͵���Ʒ�Ƿ�Ϊ����ʳ��
	BOOL IsFabaoFood(DWORD dwItemTypeID);

	TMap<DWORD,tagGodBidProto *>& GetGodBidProtoMap() { return m_mapGodBidProto; }
	tagGodBidProto *GetGodBidProto(DWORD dwProtoID) { return m_mapGodBidProto.Peek(dwProtoID); }
public:
	TMap<DWORD, tagMallOfferProto*>& GetMallOfferProtoMap() { return m_mapMallOfferProto; }
	tagMallOfferProto* GetMallOfferProto(DWORD dwTypeID) { return m_mapMallOfferProto.Peek(dwTypeID); }
private:
	VOID * InitOneMallOfferProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	TMap<DWORD, tagMallOfferProto*>  m_mapMallOfferProto;

	// ������ֳ���
	TMap<DWORD,TFabaoBirthProto *>		m_mapFabaoBirthProto;
	// ������ֳ����ϸ��
	TMap<DWORD,tagFabaoBirthItemTypeProto *> m_mapFabaoBirthItemTypeProto;

	// �ҽ�
	TMap<DWORD,tagGoldEggProto *> m_mapGoldEggProto;
	TMap<DWORD,tagGoldEggPctProto *> m_mapGoldEggPctProto;

	// ��֮Ѫ��
	TMap<DWORD,tagPulseProto *> m_mapPulseProto;

	// �ϻ���
	TMap<DWORD,tagTigerProto *> m_mapTigerProto;


	VOID * InitOneFabaoBirthProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID * InitOneFabaoBirthItemTypeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// �ҽ�
	VOID * InitOneGoldEggProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID * InitOneGoldEggPctProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	//��ְ����
	VOID * InitOneClergyProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	TMap<DWORD,tagGodHeadProto*> m_mapGodHeadProto;
	VOID * InitOneGodHeadProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//��ְ��������
	VOID * InitOneGodAreaProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// ����
	VOID * InitOnePulseProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// �ϻ���
	VOID * InitOneTigerProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
public:
	// �ҵ�������ʵ
	const tagFabaoFruitProto * GetFabaoFruitProto(DWORD dwTypeID);

	TMap<DWORD,tagFabaoStuntProto *> & GetFabaoStuntProtos(VOID);
	const tagFabaoStuntProto * GetFabaoStuntProto(DWORD dwID);

	TMap<DWORD,tagFabaoAddPromoteTimesProto *> & GetFabaoAddPromoteTimesProtos(VOID);
	const tagFabaoAddPromoteTimesProto * GetFabaoAddPromoteTimesProto(DWORD dwID);

	TMap<DWORD,tagFairyFusionQualityProto *> & GetFairyFusionQualityProtos(VOID);
	const tagFairyFusionQualityProto * GetFairyFusionQualityProto(DWORD dwID);

	TMap<DWORD,tagFairyFusionTypeProto *> & GetFairyFusionTypeProtos(VOID);
	const tagFairyFusionTypeProto * GetFairyFusionTypeProto(DWORD dwID);

	const tagWarRewardPointProto * GetWarRewardPointProto(DWORD dwID);

	const tagGuildDailyRewardProto * GetGuildDailyRewardProto(INT nGuildPos, INT nWinNum);
	const tagGuildDailyRewardProto * GetGuildDailyRewardProto(DWORD dwID);

	// ȡ��������ֳ��������typeid
	DWORD	RandomPeekFabaoBirthItem(INT nFabaoBirthValue,DWORD dwMainFabaoTypeID,DWORD * pFariyBirthType = NULL);

	// �ҽ�
	tagGoldEggProto *GetGoldEggProto(DWORD dwLayer) { return m_mapGoldEggProto.Peek(dwLayer); }
	tagGoldEggPctProto *GetGoldEggPctProto(DWORD dwLayer) { return m_mapGoldEggPctProto.Peek(dwLayer); }

	// ������ְ����ԭ��
	const tagClergyProto *GetClergyProto(DWORD dwClergyID);
	const tagClergyProto *GetClergyBase(ECampaignGodType eType);
	const tagClergyProto *GetClergyBoss(ECampaignGodType eType, ESex eSex);

	const tagGodHeadProto * GetOneGodHeadProto(DWORD dwID);

	// ������ְ��������ԭ��
	const tagGodAreaProto *GetGodAreaProto(DWORD dwGodAreaID);

	//��ͼ��������
	VOID* InitOneMapBoxProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// ���ҵ�ͼ��������ԭ��
	const tagMapBoxProto *GetMapBoxProto(DWORD dwID);

	//������Ч����
	VOID* InitOneAreaEffectProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// ����������Ч����ԭ��
	const tagAreaEffectProto* GetAreaEffectProto(DWORD dwID);

	//�����ػ��������Ա�
	VOID* InitOneFamilySpriteProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// �����ػ���������ԭ��
	const tagFamilySpriteProto *GetFamilySpriteProto(DWORD dwID);

	//���ؼ����ػ������������Ա�
	VOID* InitOneSpriteTrainProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// ���Ҽ����ػ�������������ԭ��
	const tagSpriteTrainProto *GetSpriteTrainProto(DWORD dwID);

	// �����ü����ػ�������������ԭ��
	const tagSpriteTrainProto *RandGetSpriteTrainProto(INT nLevel,EFamilySpritePickType eType);

	// ����
	tagPulseProto *GetPulseProto(DWORD dwID) { return m_mapPulseProto.Peek(dwID); }

	// �ϻ���
	tagTigerProto *GetTigerProto(DWORD dwID) { return m_mapTigerProto.Peek(dwID); }

	// ���������÷ֻ�ȡ�޸�ʥ�����Եķ�Χ
	const tagHolyLevelUpProto * GetHolyLevelUpProto(INT nScore);

	//  ��ȡʥ��ǿ�����߶�Ӧ��ǿ������
	const tagHolyEquipEnhance * GetEquipEnhanceProto(INT dwTypeId);

public:
	DWORD GetWardrobeLayerCnt() { return m_dwWardrobeLayerCnt; }
	tagWardrobeProto* GetWardrobeProto(DWORD dwLayer) { return m_mapWardrobeProto.Peek(dwLayer); }
	tagAdvancedComposeProto* GetAdvancedComposeProto(DWORD dwDesignID) { return m_mapAdvancedComposeProto.Peek(dwDesignID); }
	TMap<DWORD,tagWardrobeProto *>& GetWardrobeProtoMap() { return m_mapWardrobeProto; }
private:
	VOID * InitOneWardrobeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID * InitOneAdvancedComposeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID IncWardrobeLayerCnt() { m_dwWardrobeLayerCnt++; }
	TMap<DWORD,tagWardrobeProto *> m_mapWardrobeProto;
	TMap<DWORD,tagAdvancedComposeProto *> m_mapAdvancedComposeProto;
	DWORD m_dwWardrobeLayerCnt;		// ����

// key����ȡ������
public:
	static const int M_MaxKeyCodeRewardNum = 3;
	struct tagKeyCodeRewardProto
	{
		DWORD dwID;
		BOOL bOnlyOnce;
		DWORD dwItemTypeID[M_MaxKeyCodeRewardNum];
		INT nItemNum[M_MaxKeyCodeRewardNum];
		
		tagKeyCodeRewardProto()
		{
			ZeroMemory(this, sizeof(*this));
		}
	};
	tagKeyCodeRewardProto* GetKeyCodeReward(DWORD dwKeyCodeCrc)	{ return m_mapKeyCodeRewardProto.Peek(dwKeyCodeCrc); }
private:
	TMap<DWORD, tagKeyCodeRewardProto*> m_mapKeyCodeRewardProto;
	VOID* InitOneKeyCodeRewardProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);

// �����������
private:
	TMap<DWORD, tagFamilyQuestAttProto*>	m_mapFamilyQuestCondition;
	TMap<DWORD, tagFamilyQuestProto*>		m_mapFamilyQuests;
	TMap<DWORD, tagFamilyQuestRewardProto*> m_mapFamilyRewardProto;
	VOID* InitOneFamilyQuestCondition(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID* InitOneFamilyQuestProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID* InitOneFamilyRewardProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
public:
	TMap<DWORD, tagFamilyQuestProto*>& GetFamilyQuestProto()							{ return m_mapFamilyQuests; }
	const tagFamilyQuestAttProto* GetOneFamilyQuestCondition(DWORD dwConditionID)		{ return m_mapFamilyQuestCondition.Peek(dwConditionID); }
	const tagFamilyQuestProto* GetOneFamilyQuestProto(DWORD dwQuestID)					{ return m_mapFamilyQuests.Peek(dwQuestID); }
	const tagFamilyQuestRewardProto* GetOneFamilyRewardProto(DWORD dwRoleLvl)			{ return m_mapFamilyRewardProto.Peek(dwRoleLvl);}


private:
	TMap<DWORD, tagIDBuyBack*>	m_mapIdbuybackProto;

public:
	VOID* InitOneIdbuybackProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	const tagIDBuyBack *GetIdbuybackProto(DWORD dwID) { return m_mapIdbuybackProto.Peek(dwID); }

// װ���ɳ�
private:
	TMap<DWORD, tagEquipGrowProto*>	m_mapEquipGrowProto;
public:
	const tagEquipGrowProto* GetEquipGrowProto(DWORD dwID) { return m_mapEquipGrowProto.Peek(dwID); }
	VOID* InitOneEquipGrowProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

//���߾���
private:
	TMap<DWORD,tagExpOfflineProto*> m_mapExpOfflineProto;

public:
	VOID* InitOneExpOfflineProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagExpOfflineProto* GetExpOfflineProto(DWORD dwID) { return m_mapExpOfflineProto.Peek(dwID); }

private:
	TMap<BYTE, tagSoulCrystalLvlLine*> m_mapSoulCrystalLvlLine;
	TMap<INT, tagSoulCrystalAttData*> m_mapSoulCrystalAttData;
public:
	const tagSoulCrystalLvlLine* GetSoulCrystalLvlLine(BYTE byLvl) {return m_mapSoulCrystalLvlLine.Peek(byLvl);}
	VOID* InitOneSoulCrystalLvlLine(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagSoulCrystalAttData* GetSoulCrystalAttData(INT nId) {return m_mapSoulCrystalAttData.Peek(nId);}
	VOID* InitOneSoulCrystalAttData(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

// ��֮ս�鼼��
private:
	TMap<DWORD,tagForceSkillProto*> m_mapForceSkillProto;
	TMap<DWORD,tagForceLevelProto*> m_mapForceLevelProto;

public:
	VOID* InitOneForceSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagForceSkillProto* GetForceSkillProto(DWORD dwID) { return m_mapForceSkillProto.Peek(dwID); }
	const tagForceSkillProto* GetForceSkillProtoEx(DWORD dwID);

	VOID* InitOneForceLevelProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagForceLevelProto* GetForceLevelProto(DWORD dwID) { return m_mapForceLevelProto.Peek(dwID); }
};

//*********************************** ģ�淽�� ************************************

//-----------------------------------------------------------------------------
// ��ȡ��Դ�ļ�ģ�溯��(���ʹ��szFileName2����field�ж�Ӧ���ֶ�Ӧ��Ϊkey)
//-----------------------------------------------------------------------------
template<class K, class T>
BOOL AttRes::LoadResMap(TMap<K, T*> &mapRes, LPCTSTR szFileName, LPCTSTR szFileName2/* = NULL*/)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// �����ļ�
	m_pVar->Clear();
	if(!m_pVar->Load(NULL, szFileName, "id", &ListField))
	{
		IMSG(_T("There is something wrong when loading file<%s>!\r\n"), szFileName);
		return FALSE;
	}

	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		T* pResNode = new T;

		// ��ʼ��
		// Jason 2009-12-12
		K * p = (K*)m_pFun(pResNode, it->c_str(), X_READ_FILE_1);
		if( !P_VALID(p) )
		{
			//IMSG(_T("There is something wrong in reading proto<%s>!\n"), szFileName);
			delete pResNode;
			continue;
		}
		K key = *p;

		// ����map��
		mapRes.Add(key, pResNode);
	}

	// ���Ͷ��������Ʒ��Ŀ��Ϣ
	IMSG(_T("Read %d records from file<%s>!\n"), mapRes.Size(), szFileName);

	// �������
	m_pVar->Clear();

	// ����ļ�2��ΪNULL��Ҳͬʱ����
	if(szFileName2 != NULL)
	{
		ListField.clear();

		// ע�⣺�˴���id2����Ϊ�˱������Fieldʱ���ļ�1��ͻ
		m_pVar->Load(NULL, szFileName2, "id2", &ListField);

		for(it = ListField.begin(); it != ListField.end(); ++it)
		{
			K key = (K)_atoi64(m_pUtil->UnicodeToUnicode8(it->c_str()));
			T* pResNode = mapRes.Peek(key);
			if(!P_VALID(pResNode))
			{
				ASSERT(0);
				//IMSG(_T("There is something wrong in reading proto<%s>!\n"), szFileName2);
				continue;
			}

			// ��ʼ��
			m_pFun(pResNode, it->c_str(), X_READ_FILE_2);
		}

		// ���Ͷ��������Ʒ��Ŀ��Ϣ
		IMSG(_T("Read %d records from file<%s>!\n"), ListField.size(), szFileName2);

		// �������
		m_pVar->Clear();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// �ͷ���Դ��ģ�溯��
//-----------------------------------------------------------------------------
template<class K, class T> 
VOID AttRes::FreeResMap(TMap<K, T*> &mapRes)
{
	T *pResNode = NULL;

	mapRes.ResetIterator();
	while(mapRes.PeekNext(pResNode))
	{
		SAFE_DEL(pResNode);
	}

	mapRes.Clear();
}

//-----------------------------------------------------------------------------
// ��ȡ��Դ�ļ�ģ�溯��
//-----------------------------------------------------------------------------
template<class T>
BOOL AttRes::LoadResArray(T *arrayRes, INT32 nIndexStart, INT32 nIndexEnd, LPCTSTR szFileName)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// �����ļ�
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		return FALSE;
	}

	// ��˳���ȡ
	TCHAR szField[X_SHORT_STRING];
	for(INT32 i = nIndexStart; i <= nIndexEnd; ++i)
	{
		_stprintf(szField, _T("%d"), i);

		// ���ָ��Field�Ƿ���ڣ���������ڣ�˵����ȡ���ļ�������
		for(it = ListField.begin(); it != ListField.end(); ++it)
		{
			if(szField == *it)
			{
				break;
			}
		}

		// ����˴���ס��˵����ȡ���ļ�����������
		// ASSERT(it != ListField.end());
		if(it == ListField.end())
		{
			//IMSG(_T("There is something wrong in reading proto<%s> in <%s>!\n"), szField, szFileName);
			continue;
		}

		// ��ʼ��
		m_pFun(arrayRes, szField, i);
	}

	// �������
	m_pVar->Clear();

	return TRUE;
}

template<class T,class F>
BOOL AttRes::LoadResToOtherObject(T * Obj,F f, LPCTSTR szFileName)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// �����ļ�
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		return FALSE;
	}

	// ��˳���ȡ
	TCHAR szField[X_SHORT_STRING];
	INT nIndexStart = 0,nIndexEnd = m_pVar->GetVarNum();
	for(INT32 i = nIndexStart; i <= nIndexEnd; ++i)
	{
		_stprintf(szField, _T("%d"), i);

		// ���ָ��Field�Ƿ���ڣ���������ڣ�˵����ȡ���ļ�������
		it = find(ListField.begin(),ListField.end(),szField);
		// ����˴���ס��˵����ȡ���ļ�����������
		if(it == ListField.end())
			continue;

		// ��ʼ��
		f(Obj,m_pVar,szField);
	}

	// �������
	m_pVar->Clear();

	return TRUE;
}

extern AttRes g_attRes;

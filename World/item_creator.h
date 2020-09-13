//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_creator.cpp
// author: Sxg
// actor:
// data: 2008-07-29
// last:
// brief: ��Ʒ&װ�������� -- ��Ʒ&װ������,����������(��Ʒ����������)
//-----------------------------------------------------------------------------
#pragma once

#include "mutex.h"

struct tagEquip;
struct tagEquipProto;
struct tagIMEffect;
struct tagFConsolidateProto;
class Role;
//-----------------------------------------------------------------------------
class ItemCreator
{
	friend class GMCommandMgr;

public:
	ItemCreator();
	~ItemCreator();

public:
	// �������ݿ��ȡ�����ݴ�����Ʒ
	static tagItem* CreateItemByData(PVOID pData);
	static tagEquip* CreateEquipByData(PVOID pData);
	// ����������Ʒ�����µĶ���Ʒ
	static tagItem* Create(const tagItem &item, INT16 n16Num);
	
	// ����δ������Ʒ&װ��
	static tagItem* Create(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT nNum = 1,  
							DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000,INT nPotAdding = 0,INT qlty = -1, BOOL bFixQlty = TRUE);
	// Jason 2009-12-1 ������ɫƷ��
	static tagItem* CreateByProduce(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID,EItemQuality eQlty, INT nPotAdding,DWORD dwPurpleQltyIdfPct = 0, INT16 n16Num = 1, 
		DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000,INT qlty = -1);

	// ����δ�����䱦(��Ʒ&װ��)
	static tagItem* CreateTreasure(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num = 1, 
							DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0);

	// ����ָ��Ʒ������Ʒ&װ��
	static tagItem* CreateEx(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
							INT nNum = 1, EItemQuality eQlty = EIQ_Null, DWORD dwCreator = GT_INVALID, 
							const tagIMEffect *pIMEffect = NULL)
	{
		if (eCreateMode < 0 || eCreateMode >= EICM_End)
		{
			return NULL;
		}

		tagItem *pNewItem = Create(eCreateMode, dwCreateID, dwTypeID, nNum,GT_INVALID,0,0,10000,0,eQlty);
		if(!P_VALID(pNewItem))
		{
			return NULL;
		}

		//if(MIsEquipment(dwTypeID)/* && eQlty > EIQ_Start && eQlty < EIQ_End*/)
		//{
		//	IdentifyEquip((tagEquip*)pNewItem, eQlty, pIMEffect);
		//}

		return pNewItem;
	}

	// ����ָ��Ʒ�����䱦(��Ʒ&װ��)
	static tagItem* CreateTreasureEx(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
		INT16 n16Num = 1, EItemQuality eQlty = EIQ_Null, DWORD dwCreator = GT_INVALID, 
		const tagIMEffect *pIMEffect = NULL);


	// ������Ʒװ�����
	static VOID SetItemSerial(INT64 n64Max, INT64 n64Min) { m_n64MaxSerial = n64Max; m_n64MinSerial = n64Min; }

	// ��������Ψһ��(ע��Ҫ����������)
	static VOID CreateItemSerial(INT64 &n64NewSerial);

public:
	static VOID IdentifyEquip(IN OUT tagEquip *pEquip, 
						EItemQuality eQlty = EIQ_Null, const tagIMEffect *pIMEffect = NULL, BOOL bFixQlty = TRUE);

private: 	
	// �����Ϸ������ϲ���ú�����֤
	static VOID InitItem(tagItem &item, EItemCreateMode eCreateMode, const tagItemProto *pProto, DWORD dwCreateID, INT64 n64Serial, INT nNum, DWORD dwCreator, DWORD dwCreateTime, DWORD dwAccountID, DWORD dwOwnerID);
	static VOID InitEquipSpec(tagEquipSpec &equipSpec, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000);

	static INT32 GenBaseEquipQlty(DWORD dwTypeID);
	static INT32 ModEquipQltyByProduce(const tagEquip *pEquip, INT32 nQuality);
	static DWORD GenLongHunRandID(INT nLongHunType, INT nEquipPos, INT nQlty);
	
	static VOID CreateEquipQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, EItemQuality eQlty);
	static BOOL CreateFashionQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, 
									EItemQuality eQlty, const tagIMEffect *pIMEffect = NULL);

	static VOID ProcEquipAttBySpecAtt(tagEquip *pEquip);

	static BOOL IsGMItemNoInit(tagItem* pTmpItem);
	static BOOL InitGMItem(tagItem* pTmpItem);

	// Jason 2010-4-13 v2.0.0
	static VOID IdentifyFabao(tagFabao * pFabao,EItemQuality qlty = EIQ_Null);

	// wcy 2012-03-31 ʥ��
	static VOID IdentifyHoly(tagHolyMan * pHoly,EItemQuality qlty = EIQ_Null);

	// wcy 2012-04-18 ʥ��
	static VOID IdentifyHolyEquip(tagHolyEquip * pHolyEquip,EItemQuality qlty = EIQ_Null);	

	static VOID CreateEquipRoleL1Att(tagEquip * pEquip, const tagEquipProto *pEquipProto,const tagEquipQltyEffect * pEquipQltyEffect);
	static VOID CreateEquipRoleL2Att(tagEquip * pEquip, const tagEquipProto *pEquipProto);
public:
	static VOID RecalFaBaoCurStoredExp(tagFabao * pFabao);
	static BOOL RecalFabaoAtt(tagFabao * pFabao);
	// ���¼���ɳ��Ȼ������ԣ�������ȼ���Ϊ�˻���ʹ��
	static BOOL RecalFabaoAttWithoutCalStage(tagFabao * pFabao);
	static BOOL CanLevelupFabao(tagFabao * pFabao,INT nPracticeValue);
	static INT	RandomFabaoWuxingHole(tagFabao * pFabao);
	static INT RandFabaoWuxingRewardID(INT nDifficulty,INT QltyIntl,ENUM_FabaoType type,INT pos);
	static INT SpecRandFabaoWuxingRewardID(INT nDifficulty,INT QltyIntl,ENUM_FabaoType type,INT pos,INT nWuxingMatchReward[MaxWuxingRewardNUM]);
	static INT MergeWuxingHoleAtt(INT nQvalue,tagFabao * pFabao,tagFabao* pSecFabao);
	enum {EFABAO_RaiseIntelligence_Success = 0,EFABAO_RaiseIntelligence_Failse,EFABAO_RaiseIntelligence_NoChange,EFABAO_RaiseIntelligence_AlreadyFull};
	static INT ChangeFabaoNativeIntelligence(tagFabao * pFabao,INT addvalue = 1,INT nEfficacy = 0);
	static INT	CalFabaoSlotNum(INT stage);

	// Jason 2010-7-12 v1.0.0���¼���ǿ�����ԣ���װ��ǿ���ȼ������仯��ʱ����ã�
	static INT RecalEquipStrengthAtt(tagEquip * pEquip,INT level);

	// ������������
	static INT	ModFabaoMood(tagFabao * pFabao,INT nMood);
	// ��ȡ������������
	struct tagFabaoMoodSection
	{
		INT nMin;
		INT nMax;
	};
	static INT GetFabaoMoodSection(tagFabaoMoodSection * Mood,INT nSize,INT & num);
	// �õ���������ֳ�ϵ��
	static FLOAT GetFabaoExpPercent(tagFabao * pFabao);
	// �õ���������ϵ��
	static FLOAT GetFabaoCapabilityPercent(tagFabao * pFabao);
	// �õ��������齵����
	static INT GetFabaoFeelingdown(tagFabao * pFabao);

	// ���������ؼ�
	static DWORD RealizeFabaoStunt(tagFabao * pFabao,INT nOldStage);

	// ϴװ��һ����������
	static DWORD ResetEquipAtt(tagEquip * pEquip);
private:
	static INT64			m_n64MaxSerial;
	static INT64			m_n64MinSerial;
	static Mutex			m_Mutex;

};
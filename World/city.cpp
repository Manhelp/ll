//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city.cpp
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: ����ϵͳ
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_city.h"
#include "../WorldDefine/msg_city.h"
#include "att_res.h"
#include "item_creator.h"
#include "role.h"
#include "role_mgr.h"
#include "db_session.h"
#include "guild_mgr.h"
#include "guild.h"
#include "city.h"
#include "city_mgr.h"
#include "map_creator.h"
#include "map_mgr.h"
#include "city_struggle.h"
#include "activity_mgr.h"

//-----------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------
City::City():m_pProto(NULL)
{
	m_bInitOK			= FALSE;

	m_pGuild			= NULL;
	m_pShellCity		= NULL;
	m_pKernelCity		= NULL;

	m_dwCityID			= GT_INVALID;
	m_nDefence			= 0;
	m_nEudemonTally		= 0;

	m_nTaxRate			= 0;
	m_nTaxation			= 0;
	m_nProlificacy		= 0;

	m_nFundOutput		= 0;
	m_nMaterialOutput	= 0;
	m_dwMinSigupFare	= 0;
	m_mapItemOutput.Clear();

	m_mapAttackForce.clear();
	m_mapDefenceForce.clear();

	m_bIsCityWarInterval = TRUE; //�Ƿ�Ϊ��ĩ

	m_WarTm = GT_INVALID;


	m_bSigupConfirmed = FALSE;

	m_bSigupInfoClear = FALSE;

	m_bOutputItemed = TRUE;
}

City::~City()
{
	Destory();
}

//-----------------------------------------------------------------------------
// ��ʼ�������¡�����
//-----------------------------------------------------------------------------
BOOL City::Init( DWORD dwCityID )
{
	m_bInitOK	= FALSE;

	// ��ȡ�ó��о�̬����
	m_pProto = g_attRes.GetCityProto(dwCityID);
	if (!P_VALID(m_pProto))
	{
		return FALSE;
	}

	// ����ͼ�Ƿ�����
	MapMgr* pShellMapMgr	= g_mapCreator.GetMapMgr(m_pProto->dwShellID);
	MapMgr* pKernelMapMgr	= g_mapCreator.GetMapMgr(m_pProto->dwKernelID);
	if (!P_VALID(pShellMapMgr) || !P_VALID(pKernelMapMgr))
	{
		return FALSE;
	}
	
	if (!pShellMapMgr->IsNormalMap() || !pKernelMapMgr->IsNormalMap())
	{
		return FALSE;
	}

	m_dwCityID			= dwCityID;
	m_eClanType			= m_pProto->eClanType;
	m_pShellCity		= pShellMapMgr->GetSingleMap();
	m_pKernelCity		= pKernelMapMgr->GetSingleMap();
	m_nFundOutput		= m_pProto->nFundOutput;
	m_nMaterialOutput	= m_pProto->nMaterialOutput;
	m_dwMinSigupFare	= m_pProto->dwMinSigupFare;
	
	CActivity* pActivity = g_activityMgr.GetActivity(m_pProto->dwActivityID);
	if(P_VALID(pActivity))
	{
		m_WarTm.BegHour			= pActivity->GetStartTime().hour;
		m_WarTm.BegMinutemin	= pActivity->GetStartTime().min;
		m_WarTm.EndHour			= pActivity->GetEndTime().hour;
		m_WarTm.EndMinutemin	= pActivity->GetEndTime().min;
		m_WarTm.WarWeekDay		= pActivity->GetWeekDay();
		m_ActMode				= pActivity->GetMode();
	}
	else
	{
		//return FALSE;
		ASSERT(0);
		IMSG(_T("can not find Activity <ActivityID: %u> city system have problem\r\n"), m_pProto->dwActivityID);
	}
	
	for (int n=0; n<MAX_CITY_ITEM_OUTPUT; n++)
	{
		if (!GT_VALID(m_pProto->sItemOutput[n].dwTypeID))
		{
			continue;
		}

		tagCityItemOutput* pItemOutput = new tagCityItemOutput;
		pItemOutput->dwTypeID	= m_pProto->sItemOutput[n].dwTypeID;
		pItemOutput->nItemNum	= m_pProto->sItemOutput[n].nItemNum;
		pItemOutput->nLimit		= m_pProto->sItemOutput[n].nLimit;
		m_mapItemOutput.Add(pItemOutput->dwTypeID, pItemOutput);
	}

	m_CityStruggle.Init(this);
	return TRUE;
}

VOID City::Update(const tagDWORDTime &tm)
{	
	//��ս
	m_CityStruggle.Update();

	//�Լ�
	this->MineUpdate(tm);
}

VOID City::MineUpdate(const tagDWORDTime &tm)
{	
	m_bIsCityWarInterval = IsCityWarInterval(tm);

	//������00����й�����ȷ��
	if(!m_bSigupConfirmed && m_bIsCityWarInterval)
	{
		//��һ�·����б�
		MapSignUp::iterator iter_defence = m_mapDefenceForce.begin();
		for (; iter_defence != m_mapDefenceForce.end(); )
		{
			//�����û�б�ȷ�Ϸ��ص�
			if(iter_defence->second->nFare == 0)
			{
				SAFE_DEL(iter_defence->second);
				iter_defence = m_mapDefenceForce.erase(iter_defence);
			}
			else
			{
				++iter_defence;
			}
		}
		//ȷ�Ͻ���
		this->ConfirmAttackForce();

		m_bSigupConfirmed = TRUE;
	}
	else if(!m_bIsCityWarInterval && m_bSigupConfirmed)
	{
		m_bSigupConfirmed = FALSE;
	}

	//��ձ������� (��ս������ �����Ա���ǰ)
	if(!m_bSigupInfoClear && EWT_AFTER_WAR == GetWarTimeType(tm))
	{
		this->DoHouseKeepingForSigupInfo();
		m_bSigupInfoClear = TRUE;
	}
	else if(!m_bIsCityWarInterval && m_bSigupInfoClear)
	{
		m_bSigupInfoClear = FALSE;
	}

	//ÿ4Сʱ����һ��
	if(!m_bOutputItemed && tm.hour % 4 == 0)
	{
		this->CityProduce();
		m_bOutputItemed = TRUE;
	}
	else if(m_bOutputItemed && tm.hour % 4 == 3 )
	{
		m_bOutputItemed = FALSE;
	}
}

VOID City::Destory()
{
	tagCityItemOutput* pItemOutput = NULL;
	MapItemOutput::TMapIterator iter_item = m_mapItemOutput.Begin();
	while (m_mapItemOutput.PeekNext(iter_item, pItemOutput))
	{
		SAFE_DEL(pItemOutput);
	}
	m_mapItemOutput.Clear();

	tagCitySignUp* pSignUp = NULL;
	MapSignUp::iterator iter_attack = m_mapAttackForce.begin();
	for (; iter_attack != m_mapAttackForce.end(); iter_attack++)
	{
		SAFE_DEL(iter_attack->second);
	}
	m_mapAttackForce.clear();

	MapSignUp::iterator iter_defence = m_mapDefenceForce.begin();
	for (; iter_defence != m_mapDefenceForce.end(); iter_defence++)
	{
		SAFE_DEL(iter_defence->second);
	}
	m_mapDefenceForce.clear();
}
//-----------------------------------------------------------------------------
// ��ձ�����Ϣ
//-----------------------------------------------------------------------------
VOID City::DoHouseKeepingForSigupInfo()
{
	tagCitySignUp* pSignUp = NULL;
	MapSignUp::iterator iter_attack = m_mapAttackForce.begin();
	for (; iter_attack != m_mapAttackForce.end(); ++iter_attack)
	{
		SAFE_DEL(iter_attack->second);
	}
	m_mapAttackForce.clear();

	MapSignUp::iterator iter_defence = m_mapDefenceForce.begin();
	for (; iter_defence != m_mapDefenceForce.end(); ++iter_defence)
	{
		SAFE_DEL(iter_defence->second);
	}
	m_mapDefenceForce.clear();

	//����һ�����ݿ�
	SaveAppointedAtt2DB(ECAT_DefenceSignUp);
}
//-----------------------------------------------------------------------------
// �����ݿ������������(Ӧ�ڰ�������֮��)
//-----------------------------------------------------------------------------
BOOL City::LoadCityInfo( tagCityAtt2DB* pCityInfo, INT nSignUpNum, tagCitySignUp* pSignUpInfo )
{
	if (!P_VALID(pCityInfo))
	{
		return FALSE;
	}

	m_dwCityID		= pCityInfo->dwCityID;
	m_pGuild		= g_guildMgr.GetGuild(pCityInfo->dwGuildID);
	if (!P_VALID(m_pGuild))
	{
		m_pGuild	= NULL;
	}
	m_nDefence		= pCityInfo->nDefence;
	m_nEudemonTally	= pCityInfo->nEudemonTally;
	m_nTaxRate		= pCityInfo->nTaxRate;
	m_nTaxation		= pCityInfo->nTaxation;
	m_nProlificacy	= pCityInfo->nProlificacy;
	m_dwTaxRateTime = pCityInfo->dwTaxRateTime;

	if (nSignUpNum > 0 && !P_VALID(pSignUpInfo))
	{
		return FALSE;
	}

	for (int n=0; n<nSignUpNum; n++)
	{
		tagCitySignUp* pSignUp = new tagCitySignUp(pSignUpInfo[n]);
		if (!P_VALID(pSignUp))
		{
			continue;
		}
		Guild* pGuild = g_guildMgr.GetGuild(pSignUp->dwGuildID);
		if (!P_VALID(pGuild))
		{
			SAFE_DEL(pSignUp);
			continue;
		}
		pGuild->UnsetGuildState(EGDSS_Warfare);
		switch (pSignUp->eCityForceType)
		{
		case ECFT_Defence:
			m_mapDefenceForce.insert(std::pair<DWORD, tagCitySignUp*>(pSignUp->dwGuildID, pSignUp));
			break;

		case ECFT_Request:
		case ECFT_Attack:
			m_mapAttackForce.insert(std::pair<DWORD, tagCitySignUp*>(pSignUp->dwGuildID, pSignUp));
			break;

		default:
			SAFE_DEL(pSignUp);
			break;
		}
	}

	m_bInitOK = TRUE;

	return TRUE;
}

//-------------------------------------------------------------------------
// ��ȡ��������
//-------------------------------------------------------------------------
DWORD City::GetCityAtt( Role* pRole, tagCityAtt2Client* pCityInfo )
{
	if (!P_VALID(pCityInfo) || !P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	// �������
	if (!P_VALID(m_pGuild) || m_pGuild->GetGuildAtt().dwID != pRole->GetGuildID())
	{
		// �����ҽ�Ǯ
		if (pRole->GetCurMgr().GetBagSilver() < EXAMINE_CITY_ATT_COST)
		{
			return E_BagSilver_NotEnough;
		}
		
		// �۳���ҽ�Ǯ
		pRole->GetCurMgr().DecBagSilver(EXAMINE_CITY_ATT_COST, ELCLD_City_Examine);
	}

	// ȡ�ó�����Ϣ
	pCityInfo->dwCityID			= m_dwCityID;
	if (P_VALID(m_pGuild))
	{
		pCityInfo->dwGuildID	= m_pGuild->GetGuildAtt().dwID;

		tagGuildFacilitiesInfo sFundInfo, sMaterialInfo;
		m_pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&sFundInfo, EFT_Fund);
		m_pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&sMaterialInfo, EFT_Material);
		pCityInfo->nFundOutputCur		= (INT32)MCityOutput(m_nFundOutput, m_nProlificacy, sFundInfo.byLevel);
		pCityInfo->nMaterialOutputCur	= (INT32)MCityOutput(m_nMaterialOutput, m_nProlificacy, sMaterialInfo.byLevel);
	}
	else
	{
		// ��ͳ����
		pCityInfo->dwGuildID			= GT_INVALID;
		pCityInfo->nFundOutputCur		= 0;
		pCityInfo->nMaterialOutputCur	= 0;
	}

	pCityInfo->nDefence				= m_nDefence;
	pCityInfo->nEudemonTally		= m_nEudemonTally;
	pCityInfo->nTaxRate				= m_nTaxRate;
	pCityInfo->nProlificacy			= m_nProlificacy;
	
	pCityInfo->eCityGateType		= GetCityGateType();
	pCityInfo->nFundOutputBase		= m_nFundOutput;
	pCityInfo->nMaterialOutputBase	= m_nMaterialOutput;

	pCityInfo->nItemOutputType	= 0;
	tagCityItemOutput* pItemOutput = NULL;
	MapItemOutput::TMapIterator iter = m_mapItemOutput.Begin();
	while (m_mapItemOutput.PeekNext(iter, pItemOutput))
	{
		if (!P_VALID(pItemOutput))
		{
			continue;
		}

		if (pItemOutput->nLimit <= m_nProlificacy)
		{
			pCityInfo->sItemData[pCityInfo->nItemOutputType++]	= *pItemOutput;
		}
	}

	return E_Success;
}

VOID City::GetCityAppointedAtt( ECityAttType eType, INT& nDataNum, LPVOID pData )
{
	if (!IsInitOK())
	{
		return;
	}

	nDataNum	= 1;
	switch (eType)
	{
	case ECAT_CityID:
		*(DWORD*)pData	= m_dwCityID;
		break;

	case ECAT_ClanType:
		*(ECLanType*)pData = m_eClanType;
		break;

	case ECAT_Holder:
		if (P_VALID(m_pGuild))
		{
			*(DWORD*)pData	= m_pGuild->GetGuildAtt().dwID;
		}
		else
		{
			*(DWORD*)pData	= GT_INVALID;
		}
		break;

	case ECAT_Defence:
		*(INT32*)pData	= m_nDefence;
		break;

	case ECAT_EudemonTally:
		*(INT32*)pData	= m_nEudemonTally;
		break;

	case ECAT_TaxRate:
		*(INT32*)pData	= m_nTaxRate;
		break;

	case ECAT_TaxRateTime:
		*(DWORD*)pData	= m_dwTaxRateTime;
		break;

	case ECAT_Taxation:
		*(INT32*)pData	= m_nTaxation;
		break;

	case ECAT_Prolificacy:
		*(INT32*)pData	= m_nProlificacy;
		break;

	case ECAT_FundOutput:
		if (P_VALID(m_pGuild))
		{
			tagGuildFacilitiesInfo sFundInfo;
			m_pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&sFundInfo, EFT_Fund);
			*(INT32*)pData	= (INT32)MCityOutput(m_nFundOutput, m_nProlificacy, sFundInfo.byLevel);	
		}
		else
		{
			*(INT32*)pData	= (INT32)MCityOutput(m_nFundOutput, m_nProlificacy, 0);	
		}
		break;

	case ECAT_MaterialOutput:
		if (P_VALID(m_pGuild))
		{
			tagGuildFacilitiesInfo sMaterialInfo;
			m_pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&sMaterialInfo, EFT_Material);
			*(INT32*)pData	= (INT32)MCityOutput(m_nMaterialOutput, m_nProlificacy, sMaterialInfo.byLevel);
		}
		else
		{
			*(INT32*)pData	= (INT32)MCityOutput(m_nMaterialOutput, m_nProlificacy, 0);	
		}
		break;

	case ECAT_ItemOutput:
		{
			nDataNum	= 0;
			tagCityItemOutput* pItemOutput = NULL;
			MapItemOutput::TMapIterator iter = m_mapItemOutput.Begin();
			while (m_mapItemOutput.PeekNext(iter, pItemOutput))
			{
				if (!P_VALID(pItemOutput))
				{
					continue;
				}
				
				if (pItemOutput->nLimit <= m_nProlificacy)
				{
					((tagCityItemOutput*)pData)[nDataNum++]	= *pItemOutput;
				}
			}
		}
		break;
	case ECAT_AttackSignUp:
	case ECAT_RequestSignUp:
		{
			nDataNum = 0;
			MapSignUp::iterator iter = m_mapAttackForce.begin();
			for (; iter != m_mapAttackForce.end(); iter++)
			{
				((tagCitySignUp*)pData)[nDataNum++] = *(iter->second);
			}
		}
		break;

	case ECAT_DefenceSignUp:
		{
			nDataNum = 0;
			MapSignUp::iterator iter = m_mapDefenceForce.begin();
			for (; iter != m_mapDefenceForce.end(); iter++)
			{
				((tagCitySignUp*)pData)[nDataNum++] = *(iter->second);
			}
		}
		break;
	case ECAT_AttackSignUpGuildID:
		{
			nDataNum = 0;
			MapSignUp::iterator iter = m_mapAttackForce.begin();
			for (; iter != m_mapAttackForce.end(); iter++)
			{
				((DWORD*)pData)[nDataNum++] = (iter->second)->dwGuildID;
			}
		}
		break;
	case ECAT_DefenceSignUpGuildID:
		{
			nDataNum = 0;
			MapSignUp::iterator iter = m_mapDefenceForce.begin();
			for (; iter != m_mapDefenceForce.end(); iter++)
			{
				((DWORD*)pData)[nDataNum++] = (iter->second)->dwGuildID;
			}
		}
		break;
	case ECAT_ConfirmDefenceGuildID:
		{
			nDataNum = 0;
			MapSignUp::iterator iter = m_mapDefenceForce.begin();
			for (; iter != m_mapDefenceForce.end(); iter++)
			{
				if(iter->second->nFare != 0)
				{
					((DWORD*)pData)[nDataNum++] = (iter->second)->dwGuildID;
				}
			}
		}
		break;
	case ECAT_AttackSignUpNum:
		*(INT32*)pData	=	m_mapAttackForce.size();
		break;
	case ECAT_DefenceSignUpNum:
		*(INT32*)pData	=	m_mapDefenceForce.size();
		break;
	case ECAT_DefenceConfirmNum:
		{
			nDataNum = 0;
			MapSignUp::iterator iter = m_mapDefenceForce.begin();
			for (; iter != m_mapDefenceForce.end(); iter++)
			{
				if(iter->second->nFare != 0)
					++nDataNum;
			}
			*(INT32*)pData	= nDataNum;
		}
		break;
	default:
		nDataNum	= 0;
		break;
	}
}
//-------------------------------------------------------------------------
// �õ�������Ϣ
//-------------------------------------------------------------------------
VOID City::GetCitySigupInfo(Role *pRole,tagNetCmd* pCmd)
{
	MGET_MSG(pSend, pCmd, NS_GetSigupInfo);

	if (!IsInitOK())
	{
		return;
	}


	INT32 nDataNum = 0;
	DWORD dwFareTotal = 0;
	MapSignUp::iterator iter = m_mapAttackForce.begin();
	for (; iter != m_mapAttackForce.end(); iter++)
	{
		(pSend->dwData)[nDataNum++] = (iter->second)->dwGuildID;
		dwFareTotal += (iter->second)->nFare;
	}

	INT32 nDivNum = nDataNum ? nDataNum : 1;
	//����ƽ��Ǯ��
	pSend->dwAvgAckMoney = dwFareTotal/nDivNum;

	iter = m_mapDefenceForce.begin();
	for (; iter != m_mapDefenceForce.end(); iter++)
	{
		(pSend->dwData)[nDataNum++] = (iter->second)->dwGuildID;
	}

	//�Ѿ�ȷ�ϵ�
	iter = m_mapDefenceForce.begin();
	for (; iter != m_mapDefenceForce.end(); iter++)
	{
		if(iter->second->nFare != 0)
		{
			(pSend->dwData)[nDataNum++] = (iter->second)->dwGuildID;
		}
	}	

	//��ǰռ��ó��еİ���id �����npcռ����ΪGT_INVALID
	pSend->dwGuildID = P_VALID(m_pGuild)? m_pGuild->GetGuildAtt().dwID : GT_INVALID;

	//����id
	pSend->dwCityID	= m_dwCityID;

	//��ս��ʼ������ʱ��
	pSend->WarTime = m_WarTm;
	
}

//-------------------------------------------------------------------------
// �õ���ս��Ϣ
//-------------------------------------------------------------------------
VOID City::GetCityWarInfo(Role *pRole,tagNetCmd* pCmd)
{
	MGET_MSG(pSend, pCmd, NS_GetCityWarInfo);

	if (!IsInitOK())
	{	
		pSend->dwErrorCode = GT_INVALID;
		return;
	}
	// û�а���ռ��
	if (!P_VALID(m_pGuild))
	{	
		pSend->dwErrorCode = E_City_Not_BeOccupied;
		return;
	}

	//��ʼ�����������б� Ϊ GT_INVALID

	memset(&pSend->dwAttacks,GT_INVALID,sizeof(pSend->dwAttacks));
	memset(&pSend->dwDefenses,GT_INVALID,sizeof(pSend->dwDefenses));

	//���������ĩ
	if(m_bIsCityWarInterval != TRUE)
	{
		//ֻ����ȷ�Ϸ��ص�
		INT32 nDefConfirmNum = 0;
		INT32 nTemp = 0;
		this->GetCityAppointedAtt(ECAT_DefenceConfirmNum,nTemp,&nDefConfirmNum);

		if(nDefConfirmNum > MAX_CITY_DEFENCE_FORCE)
		{
			pSend->dwErrorCode = E_City_Time_Limit;
			return;
		}

		//�õ�����ȷ�ϵ�
		this->GetCityAppointedAtt(ECAT_ConfirmDefenceGuildID,nDefConfirmNum,pSend->dwDefenses);
		pSend->dwDefenses[nDefConfirmNum] = m_pGuild->GetGuildAtt().dwID;

		pSend->dwErrorCode = E_Success;
		return;
	}
	else
	{
		INT32 nAckNum = 0;
		INT32 nDefNum = 0;
		INT nTemp = 0;
		this->GetCityAppointedAtt(ECAT_AttackSignUpNum,nTemp,&nAckNum);
		this->GetCityAppointedAtt(ECAT_DefenceSignUpNum,nTemp,&nDefNum);
	
		if(nAckNum > MAX_CITY_ATTACK_FORCE || nDefNum > MAX_CITY_DEFENCE_FORCE)
		{
			pSend->dwErrorCode = E_City_Time_Limit;
			return;
		}

		//�õ����ط��ж�
		this->GetCityAppointedAtt(ECAT_AttackSignUpGuildID,nAckNum,pSend->dwAttacks);
		this->GetCityAppointedAtt(ECAT_DefenceSignUpGuildID,nDefNum,pSend->dwDefenses);
		pSend->dwDefenses[nDefNum] = m_pGuild->GetGuildAtt().dwID;

		pSend->dwErrorCode = E_Success;
	}
}
//-------------------------------------------------------------------------
// ͳ��Ȩ���
//-------------------------------------------------------------------------
DWORD City::ChangeCityHolder( DWORD dwGuildID )
{
	if (!IsInitOK())
	{
		return GT_INVALID;
	}
	
	//ͳ��Ȩ�������������з���ֵ��������ΪĬ��ֵ
	if(P_VALID(m_pGuild))
	{
		if(m_pGuild->GetGuildAtt().dwID != dwGuildID)
		{
			//�ָ�Ĭ��ֵ
			m_nDefence = m_pProto->dwDefaultDefence;

			//����һ�����ݿ�
			SaveAppointedAtt2DB(ECAT_Defence);
		}
		//����ԭ�������ڵ�ռ������б�
		m_pGuild->ReleaseCity(m_pProto->dwCityID);
	}
	else
	{
		//�״α�ռ�� ���øı����ֵ
	}


	m_pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(m_pGuild))
	{
		m_pGuild = NULL;
	}
	m_pGuild->HoldCity(m_pProto->dwCityID);

	//����һ�����ݿ�
	SaveAppointedAtt2DB(ECAT_Holder);

	//����Ȩ�������
//	g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID())->OnCityBroadCast(ECBC_ChangeHolder, dwGuildID, m_pKernelCity->GetMapID());

	return E_Success;
}

//-------------------------------------------------------------------------
// ���в���
//-------------------------------------------------------------------------
VOID City::CityProduce()
{
	if (!IsInitOK())
	{
		return;
	}

	// û�а���ռ��
	if (!P_VALID(m_pGuild))
	{
		return;
	}

	// �ʽ��ʲĲ���
	tagGuildFacilitiesInfo sFundInfo, sMaterialInfo;
	m_pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&sFundInfo, EFT_Fund);
	m_pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&sMaterialInfo, EFT_Material);
	m_pGuild->IncGuildFund(m_dwCityID, (INT32)MCityOutput(m_nFundOutput, m_nProlificacy, sFundInfo.byLevel), ELCLD_City_Produce);
	m_pGuild->IncGuildMaterial(m_dwCityID, (INT32)MCityOutput(m_nMaterialOutput, m_nProlificacy, sMaterialInfo.byLevel), ELCLD_City_Produce);

	// ��Ʒ����
	tagCityItemOutput* pItemOutput = NULL;
	MapItemOutput::TMapIterator iter = m_mapItemOutput.Begin();
	while (m_mapItemOutput.PeekNext(iter, pItemOutput))
	{
		if (!P_VALID(pItemOutput))
		{
			continue;
		}

		if (pItemOutput->nLimit > m_nProlificacy)
		{
			continue;
		}

		// ������Ʒ
		tagItem *pItem = ItemCreator::CreateEx(EICM_CityProduce, GT_INVALID, pItemOutput->dwTypeID, pItemOutput->nItemNum, EIQ_White);
		if(!P_VALID(pItem))
		{
			continue;
		}

		// ������ɲֿ�
		DWORD dwErrorCode = m_pGuild->GetGuildWarehouse().AddItem(pItem, ELCLD_City_Produce);
		if (dwErrorCode != E_Success)
		{
			::Destroy(pItem);
		}
	}

	// �۳�����������
	DecCityProlificacy(CITY_PRODUCE_COST);
}

//-------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------
DWORD City::UpgradeProlificacy( Role* pRole, EProlificacyUpgrade eType )
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (eType < 0 || eType >= EPU_End)
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	// �ް���ռ��ĳ���
	if (!P_VALID(m_pGuild))
	{
		return E_City_Not_BeOccupied;
	}

	// �����Ұ�����Ϣ
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return E_City_Guild_NotMatch;
	}
	else if (m_pGuild->GetGuildAtt().dwID != pRole->GetGuildID())
	{
		return E_City_Guild_NotMatch;
	}

	//���з�չͭ����ɺ����������������2�㣬���X����ɹ���
	//���з�չ������ɺ����������������5�㣬���X����ɹ���
	//���з�չ������ɺ����������������15�㣬���X����ɹ���

	// �����ұ�����Ϣ
	switch (eType)
	{
	case EPU_Gold:
		if (pRole->GetItemMgr().GetBagSameItemCount(CITY_DEVELOP_GOLD_CARD) > 0)
		{
			pRole->GetItemMgr().RemoveFromRole(CITY_DEVELOP_GOLD_CARD, 1, ELCLD_City_Prolificacy);
			IncCityProlificacy(CITY_DEVELOP_GOLD_ADD);
			//ȫ���ɳ�Ա
			const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
			if(P_VALID(pMapScript))
				pMapScript->OnCityBroadCast(ECBC_UpgradeProlificacy, pRole->GetID(),CITY_DEVELOP_GOLD_CARD\
																				,m_pShellCity->GetMapID(),CITY_DEVELOP_GOLD_ADD,pRole->GetGuildID());
		}
		else
		{
			return E_City_Prolificacy_LackItem;
		}
		break;

	case EPU_Silver:
		if (pRole->GetItemMgr().GetBagSameItemCount(CITY_DEVELOP_SILVER_CARD) > 0)
		{
			pRole->GetItemMgr().RemoveFromRole(CITY_DEVELOP_SILVER_CARD, 1, ELCLD_City_Prolificacy);
			IncCityProlificacy(CITY_DEVELOP_SILVER_ADD);
			//ȫ���ɳ�Ա
			const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
			if(P_VALID(pMapScript))
				pMapScript->OnCityBroadCast(ECBC_UpgradeProlificacy, pRole->GetID(),CITY_DEVELOP_SILVER_CARD\
																				,m_pShellCity->GetMapID(),CITY_DEVELOP_SILVER_ADD,pRole->GetGuildID());
		}
		else
		{
			return E_City_Prolificacy_LackItem;
		}
		break;

	case EPU_Copper:
		if (pRole->GetItemMgr().GetBagSameItemCount(CITY_DEVELOP_COPPER_CARD) > 0)
		{
			pRole->GetItemMgr().RemoveFromRole(CITY_DEVELOP_COPPER_CARD, 1, ELCLD_City_Prolificacy);
			IncCityProlificacy(CITY_DEVELOP_COPPER_ADD);
			//ȫ���ɳ�Ա
			const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
			if(P_VALID(pMapScript))
				pMapScript->OnCityBroadCast(ECBC_UpgradeProlificacy, pRole->GetID(),CITY_DEVELOP_COPPER_CARD\
																				,m_pShellCity->GetMapID(),CITY_DEVELOP_COPPER_ADD,pRole->GetGuildID());
		}
		else
		{
			return E_City_Prolificacy_LackItem;
		}
		break;

	default:
		return E_City_Prolificacy_LackItem;
		break;
	}

	return E_Success;
}

//-------------------------------------------------------------------------
// ����˰��
//-------------------------------------------------------------------------
DWORD City::ModifyTaxRate( Role* pRole, INT32 nTaxRate )
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (nTaxRate < 0 || nTaxRate > MAX_CITY_TAXRATE)
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	// �ް���ռ��ĳ���
	if (!P_VALID(m_pGuild))
	{
		return E_City_Not_BeOccupied;
	}

	// �Ƿ���ͳ�ΰ���
	if (pRole->GetGuildID() != m_pGuild->GetGuildAtt().dwID)
	{
		return E_City_Guild_NotMatch;
	}

	// ������Ȩ��
	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_City_Guild_NotMatch;
	}

	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bSetTaxRate)
	{
		return E_City_Power_NotEnough;
	}

	// ���ʱ��
	if ( (m_dwTaxRateTime != 0) && (CalcTimeDiff(GetCurrentDWORDTime(), m_dwTaxRateTime) < MODIFY_CITY_TAXRATE_CYCLE) )
	{
		return E_City_Time_Limit;
	}

	// ����˰��
	ExchangeCityTaxRate(nTaxRate);
	
	return E_Success;
}

//-------------------------------------------------------------------------
// ��ȡ˰��
//-------------------------------------------------------------------------
DWORD City::TransformTaxation( Role* pRole, ETaxTransformType eType, INT32 nTaxation )
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (nTaxation < 0 || nTaxation > MAX_CITY_ATT_VALUE || nTaxation > m_nTaxation)
	{
		return GT_INVALID;
	}

	if (eType < 0 || eType >= ETTT_End)
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	// �ް���ռ��ĳ���
	if (!P_VALID(m_pGuild))
	{
		return E_City_Not_BeOccupied;
	}

	// �Ƿ���ͳ�ΰ���
	if (pRole->GetGuildID() != m_pGuild->GetGuildAtt().dwID)
	{
		return E_City_Guild_NotMatch;
	}

	// ������Ȩ��
	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_City_Guild_NotMatch;
	}

	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bTransformTax)
	{
		return E_City_Power_NotEnough;
	}

	// ת������˰��
	if (!DecCityTaxation(nTaxation))
	{
		return E_City_Taxation_NotEnough;
	}

	switch (eType)
	{
	case ETTT_GuildFund:
		m_pGuild->IncGuildFund(pRole->GetID(), nTaxation, ELCLD_City_TransformTaxation);
		break;

	case ETTT_PersonalSilver:
		pRole->GetCurMgr().IncBagSilver(nTaxation, ELCLD_City_TransformTaxation);
		break;
	}

	return E_Success;
}

//-------------------------------------------------------------------------
// ȡ�ó�������
//-------------------------------------------------------------------------
ECityGateType City::GetCityGateType()
{
	if (m_nDefence < 0 || m_nDefence > MAX_CITY_DEFENCE)
	{
		return ECGT_End;
	}

	for (int n=1; n<ECGT_End; n++)
	{
		if (m_nDefence < CITY_GATE_DEFENCE[n])
		{
			return (ECityGateType)(n-1);
		}
	}

	return ECGT_Eternity;
}

//-------------------------------------------------------------------------
// �������Բ���
//-------------------------------------------------------------------------
BOOL City::IncCityDefence( INT32 nDefence )
{
	if (nDefence <= 0)
	{
		return FALSE;
	}

	m_nDefence += nDefence;
	if (m_nDefence > MAX_CITY_DEFENCE)
	{
		m_nDefence = MAX_CITY_DEFENCE;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_Defence);

	return TRUE;
}

BOOL City::DecCityDefence( INT32 nDefence )
{
	if (nDefence <= 0)
	{
		return FALSE;
	}

	m_nDefence -= nDefence;
	if (m_nDefence < 0)
	{
		m_nDefence = 0;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_Defence);

	return TRUE;
}

BOOL City::IncEudemonTally( INT32 nEudemonTally )
{
	if (nEudemonTally <= 0)
	{
		return FALSE;
	}

	m_nEudemonTally += nEudemonTally;
	if (m_nEudemonTally > MAX_CITY_EUDEMONTALLY)
	{
		m_nEudemonTally = MAX_CITY_EUDEMONTALLY;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_EudemonTally);

	return TRUE;
}

BOOL City::DecEudemonTally( INT32 nEudemonTally )
{
	if (nEudemonTally <= 0)
	{
		return FALSE;
	}

	m_nEudemonTally -= nEudemonTally;
	if (m_nEudemonTally < 0)
	{
		m_nEudemonTally = 0;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_EudemonTally);

	return TRUE;
}

BOOL City::ExchangeCityTaxRate( INT32 nTaxRate )
{
	if (nTaxRate < 0 || nTaxRate > MAX_CITY_TAXRATE)
	{
		return FALSE;
	}

	m_nTaxRate		= nTaxRate;
	m_dwTaxRateTime	= GetCurrentDWORDTime();

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_TaxRate);
	SaveAppointedAtt2DB(ECAT_TaxRateTime);

	// ȫ���㲥
//	g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID())->OnCityBroadCast(ECBC_ChangeTaxRate,  m_pShellCity->GetMapID(), m_nTaxRate);

	return TRUE;
}

BOOL City::IncCityTaxation( INT32 nTaxation )
{
	if (nTaxation <= 0)
	{
		return FALSE;
	}

	m_nTaxation += nTaxation;
	if (m_nTaxation > MAX_CITY_ATT_VALUE)
	{
		m_nTaxation = MAX_CITY_ATT_VALUE;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_Taxation);

	return TRUE;
}

BOOL City::DecCityTaxation( INT32 nTaxation )
{
	if (nTaxation <= 0)
	{
		return FALSE;
	}

	m_nTaxation -= nTaxation;
	if (m_nTaxation < 0)
	{
		m_nTaxation = 0;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_Taxation);

	return TRUE;
}

BOOL City::IncCityProlificacy( INT32 nProlificacy )
{
	if (nProlificacy <= 0)
	{
		return FALSE;
	}

	m_nProlificacy += nProlificacy;
	if (m_nProlificacy > MAX_CITY_PROLIFICACY)
	{
		m_nProlificacy = MAX_CITY_PROLIFICACY;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_Prolificacy);

	return TRUE;
}

BOOL City::DecCityProlificacy( INT32 nProlificacy )
{
	if (nProlificacy <= 0)
	{
		return FALSE;
	}

	m_nProlificacy -= nProlificacy;
	if (m_nProlificacy < 0)
	{
		m_nProlificacy = 0;
	}

	// ���浽���ݿ�
	SaveAppointedAtt2DB(ECAT_Prolificacy);

	return TRUE;
}

//-------------------------------------------------------------------------
// ���浽���ݿ�
//-------------------------------------------------------------------------
VOID City::SaveCityAtt2DB()
{
	tagNDBC_SaveCityAtt send;
	send.sCityAtt.dwCityID		= m_dwCityID;
	if (P_VALID(m_pGuild))
	{
		send.sCityAtt.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
	}
	else
	{
		send.sCityAtt.dwGuildID = GT_INVALID;
	}
	send.sCityAtt.nDefence		= m_nDefence;
	send.sCityAtt.nEudemonTally	= m_nEudemonTally;
	send.sCityAtt.nProlificacy	= m_nProlificacy;
	send.sCityAtt.nTaxRate		= m_nTaxRate;
	send.sCityAtt.nTaxation		= m_nTaxation;
	send.sCityAtt.dwTaxRateTime	= m_dwTaxRateTime;

	g_dbSession.Send(&send, send.dwSize);
}

VOID City::SaveAppointedAtt2DB( ECityAttType eType )
{
	if (eType < 0 || eType >= ECAT_End)
	{
		return;
	}

	MCREATE_MSG(pSend, MAX_CITY_BUFF_SIZE, NDBC_SaveAppointedAtt);
	pSend->dwCityID				= m_dwCityID;
	pSend->sCityAtt.eType		= eType;

	switch (eType)
	{
	case ECAT_AttackSignUp:
	case ECAT_DefenceSignUp:
	case ECAT_RequestSignUp:
		{
			INT nNum = 0;
			GetCityAppointedAtt(ECAT_AttackSignUp, nNum, pSend->sCityAtt.byData);
			GetCityAppointedAtt(ECAT_DefenceSignUp, pSend->sCityAtt.nDataNum, pSend->sCityAtt.byData + sizeof(tagCitySignUp) * nNum);
			pSend->sCityAtt.nDataNum += nNum;
			pSend->dwSize = sizeof(tagNDBC_SaveAppointedAtt) - 1 + GetCityAppointedAttSize(ECAT_AttackSignUp) + GetCityAppointedAttSize(ECAT_DefenceSignUp);
		}
		break;

	default:
		GetCityAppointedAtt(eType, pSend->sCityAtt.nDataNum, pSend->sCityAtt.byData);
		pSend->dwSize = sizeof(tagNDBC_SaveAppointedAtt) - 1 + GetCityAppointedAttSize(eType);
		break;
	}

	g_dbSession.Send(pSend, pSend->dwSize);
}

//-------------------------------------------------------------------------
// ��ս����
//-------------------------------------------------------------------------
DWORD City::SignupForCitywar( Role* pRole, ECityForceType eForce, INT32 nFare )
{
	if (!P_VALID(pRole) || nFare < 0)
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	// ��Ӫ���ͼ��
	if ((P_VALID(m_pGuild) && eForce == ECFT_Request) || (!P_VALID(m_pGuild) && eForce != ECFT_Request))
	{
		return E_City_SignUp_ForceError;
	}

	// ������Ȩ��
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return E_City_Guild_NotMatch;
	}

	if(pGuild->GetGuildAtt().dwLeaderRoleID != pRole->GetID())
	{
		return E_City_Guild_NotMatch;
	}

	// �����ɺͰ�������
	const tagGuild& sGuildInfo = pGuild->GetGuildAtt();
	if (sGuildInfo.byLevel < 2)
	{
		return E_City_SignUp_LevelLimit;
	}

	tagRoleInfo* pLeaderInfo = g_roleMgr.GetRoleInfo(sGuildInfo.dwLeaderRoleID);
	if (!P_VALID(pLeaderInfo))
	{
		return E_City_Guild_NotMatch;
	}

	if (pLeaderInfo->byLevel < 60)
	{
		return E_City_SignUp_LevelLimit;
	}

	if (pGuild->IsHoldCityMax())
	{
		return E_City_SignUp_Hold_CityMax;
	}

	// ����ʱ���ж�
	if (m_bIsCityWarInterval)
	{
		return E_City_Time_Limit;
	}

	// �ظ������ж�
	if (P_VALID(m_pGuild) && m_pGuild->GetGuildAtt().dwID == sGuildInfo.dwID)
	{
		return E_City_SignUp_HolderConflict;	// ͳ�ΰ��ɲ��ܱ���
	}

	if ( (m_mapAttackForce.find(sGuildInfo.dwID) != m_mapAttackForce.end())
	||	(m_mapDefenceForce.find(sGuildInfo.dwID) != m_mapDefenceForce.end()))
	{
		return E_City_SignUp_InsideCityConflict;	// �����ڳ�ͻ
	}

	if (g_cityMgr.IsAlreadySignup(sGuildInfo.dwID, eForce))
	{
		return E_City_SignUp_InterCityConflict;	// ���м��ͻ
	}

	if(eForce == ECFT_Attack && nFare < m_dwMinSigupFare)
	{
		return E_City_SignUp_Minfare_Limite;
	}

	// �����ʽ��ж�
	if (sGuildInfo.nFund < nFare)
	{
		return E_City_SignUp_Fund_NotEnough;
	}

	//�״����벻��Ǯ
	if(eForce != ECFT_Request)
	{
		// �۳������ʽ�
		pGuild->DecGuildFund(pRole->GetID(), nFare, ELCLD_City_SignUp);
	}


	// �����ɹ�
	tagCitySignUp* pSignUpInfo	= new tagCitySignUp;
	pSignUpInfo->dwGuildID		= sGuildInfo.dwID;
	pSignUpInfo->eCityForceType	= eForce;
	pSignUpInfo->nFare			= nFare;

	switch (eForce)
	{
	case ECFT_Defence:
		m_mapDefenceForce.insert(std::pair<DWORD, tagCitySignUp*>(sGuildInfo.dwID, pSignUpInfo));
		SaveAppointedAtt2DB(ECAT_DefenceSignUp);
		break;

	case ECFT_Request:
	case ECFT_Attack:
		m_mapAttackForce.insert(std::pair<DWORD, tagCitySignUp*>(sGuildInfo.dwID, pSignUpInfo));
		SaveAppointedAtt2DB(ECAT_AttackSignUp);
		break;

	default:
		SAFE_DEL(pSignUpInfo);
		return E_City_SignUp_ForceError;
	}

	// �����ͻ���

	// ȫ���㲥

	// ���ɹ㲥
	if(eForce == ECFT_Defence)
	{
		const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
		if(P_VALID(pMapScript))
			pMapScript->OnCityBroadCast(ECBC_DefenceSignup,  m_pKernelCity->GetMapID(), sGuildInfo.dwID);
	}
	else if(eForce == ECFT_Attack)
	{
		const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
		if(P_VALID(pMapScript))
			pMapScript->OnCityBroadCast(ECBC_AttackSignup,  m_pKernelCity->GetMapID(), sGuildInfo.dwID);
	}
	else if(eForce == ECFT_Request)
	{
		const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
		if(P_VALID(pMapScript))
		pMapScript->OnCityBroadCast(ECBC_RequestSignup,  pRole->GetID(), m_pKernelCity->GetMapID(), sGuildInfo.dwID);
	}

	return E_Success;
}

DWORD City::SignupForCitywar(DWORD dwGuildID, ECityForceType eForce, INT32 nFare)
{
	if (nFare < 0)
	{
		IMSG(_T("nFare is invalid!!\r\n"));
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		IMSG(_T("system not initial ok!!\r\n"));
		return GT_INVALID;
	}

	// ��Ӫ���ͼ��
	if ((P_VALID(m_pGuild) && eForce == ECFT_Request) || (!P_VALID(m_pGuild) && eForce != ECFT_Request))
	{
		IMSG(_T("city force type is invalid!!!\r\n"));
		return GT_INVALID;
	}

	// ������Ȩ��
	Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(pGuild))
	{
		IMSG(_T("guild not exist!!!\r\n"));
		return GT_INVALID;
	}

	// �����ɺͰ�������
	const tagGuild& sGuildInfo = pGuild->GetGuildAtt();
	if (sGuildInfo.byLevel < 2)
	{
		IMSG(_T("level of the guild is %d,Please check the level of guild!!!\r\n"), sGuildInfo.byLevel);
		return GT_INVALID;
	}

	tagRoleInfo* pLeaderInfo = g_roleMgr.GetRoleInfo(sGuildInfo.dwLeaderRoleID);
	if (!P_VALID(pLeaderInfo))
	{
		IMSG(_T("guild has not leader!!!\r\n"));
		return GT_INVALID;
	}

	if (pLeaderInfo->byLevel < 60)
	{
		IMSG(_T("guild leader level is less than 60!!\r\n"));
		return GT_INVALID;
	}

	if (pGuild->IsHoldCityMax())
	{
		IMSG(_T("guild hold city max!!\r\n"));
		return GT_INVALID;
	}

	// �ظ������ж�
	if (P_VALID(m_pGuild) && m_pGuild->GetGuildAtt().dwID == sGuildInfo.dwID)
	{
		IMSG(_T("guild holder can not signup for city war!!\r\n"));
		return GT_INVALID;	// ͳ�ΰ��ɲ��ܱ���
	}

	if ( (m_mapAttackForce.find(sGuildInfo.dwID) != m_mapAttackForce.end())
		||	(m_mapDefenceForce.find(sGuildInfo.dwID) != m_mapDefenceForce.end()))
	{
		IMSG(_T("E_City_SignUp_InsideCityConflict!!\r\n"));
		return GT_INVALID;	// �����ڳ�ͻ
	}

	if (g_cityMgr.IsAlreadySignup(sGuildInfo.dwID, eForce))
	{
		IMSG(_T("E_City_SignUp_InterCityConflict!!\r\n"));
		return GT_INVALID;	// ���м��ͻ
	}

	// �����ɹ�
	tagCitySignUp* pSignUpInfo	= new tagCitySignUp;
	pSignUpInfo->dwGuildID		= sGuildInfo.dwID;
	pSignUpInfo->eCityForceType	= eForce;
	pSignUpInfo->nFare			= nFare;

	switch (eForce)
	{
	case ECFT_Defence:
		m_mapDefenceForce.insert(std::pair<DWORD, tagCitySignUp*>(sGuildInfo.dwID, pSignUpInfo));
		SaveAppointedAtt2DB(ECAT_DefenceSignUp);
		break;

	case ECFT_Request:
	case ECFT_Attack:
		m_mapAttackForce.insert(std::pair<DWORD, tagCitySignUp*>(sGuildInfo.dwID, pSignUpInfo));
		SaveAppointedAtt2DB(ECAT_AttackSignUp);
		break;

	default:
		SAFE_DEL(pSignUpInfo);
		IMSG(_T("city force type is invalid!!!\r\n"));
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------
// ������ȷ��/���������������
//-------------------------------------------------------------------------
DWORD City::ConfirmAttackForce()
{
	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	// �жϳ�������
	if (P_VALID(m_pGuild))
	{
		// ȡ��������ǰ5������
		std::vector<tagCitySignUp*> vecSignUpInfo;
		vecSignUpInfo.clear();
		MapSignUp::iterator iter = m_mapAttackForce.begin();
		for (; iter != m_mapAttackForce.end(); iter++)
			vecSignUpInfo.push_back(iter->second);

		//��������ս�İ���������5
		if(vecSignUpInfo.size() > MAX_CITY_ATTACK_FORCE)
			nth_element(vecSignUpInfo.begin(), vecSignUpInfo.begin()+MAX_CITY_ATTACK_FORCE, vecSignUpInfo.end(), FareCompare());

		// ��ս
		std::vector<tagCitySignUp*>::iterator iter_vec = vecSignUpInfo.begin();
		for (int n=0; n<MAX_CITY_ATTACK_FORCE && iter_vec!=vecSignUpInfo.end(); n++, ++iter_vec)
		{
			//����ս���ɹ㲥
			const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
			if(P_VALID(pMapScript))
				pMapScript->OnCityBroadCast(ECBC_ReadyForAttack,m_pKernelCity->GetMapID(),(*iter_vec)->dwGuildID);
		}

		// ����������
		for (; iter_vec!=vecSignUpInfo.end(); ++iter_vec)
		{
			Guild* pGuild = g_guildMgr.GetGuild((*iter_vec)->dwGuildID);
			if (P_VALID(pGuild))
			{
				pGuild->IncGuildFund(GT_INVALID, (*iter_vec)->nFare, ELCLD_City_SignUp);
			}

			//��һ�½����б�
			iter = m_mapAttackForce.find((*iter_vec)->dwGuildID);
			if(iter != m_mapAttackForce.end())
			{
				SAFE_DEL(iter->second);
				m_mapAttackForce.erase(iter);
			}
		}

		// ȫ���㲥
		const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
		if(P_VALID(pMapScript))
			pMapScript->OnCityBroadCast(ECBC_ConfirmAttack);
	}
	else
	{
		// �����ݿ��������������Ϣ
		DWORD	dwSize = sizeof(tagNDBC_GetGuildClanValue) + (m_mapAttackForce.size() - 1) * sizeof(DWORD);
		MCREATE_MSG(pSend, dwSize, NDBC_GetGuildClanValue);
		pSend->eClanType		= m_eClanType;

		GetCityAppointedAtt(ECAT_AttackSignUpGuildID, pSend->nGuildNum, pSend->dwGuildID);

		g_dbSession.Send(pSend, pSend->dwSize);

		return E_Success;
	}

	SaveAppointedAtt2DB(ECAT_AttackSignUp);

	return E_Success;
}

VOID City::ConfirmAttackForce(INT nNum, tagGuildClanValue *pClanValue)
{
	if (nNum <= 0 || !P_VALID(pClanValue))
		return;

	tagGuildClanValue* pGuildClan = max_element(pClanValue, pClanValue + nNum, ReputationCompare());
	if (!P_VALID(pGuildClan))
		return;

	// ͳ��Ȩ���
	ChangeCityHolder(pGuildClan->dwGuildID);

	//��һ�½����б�
	MapSignUp::iterator iter = m_mapAttackForce.begin();
	for (; iter != m_mapAttackForce.end(); ++iter)
	{
		SAFE_DEL(iter->second);
	}
	m_mapAttackForce.clear();

	//����һ�����ݿ�
	SaveAppointedAtt2DB(ECAT_AttackSignUp);

	// ȫ���㲥
	const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
	if(P_VALID(pMapScript))
		pMapScript->OnCityBroadCast(ECBC_ConfirmAttack);
}

//-------------------------------------------------------------------------
// ��������ȷ��
//-------------------------------------------------------------------------
DWORD City::ConfirmDefenceForce( Role* pRole, DWORD dwGuildID[MAX_CITY_DEFENCE_FORCE] )
{	
	//������ָ��
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	//����Ƿ��ʼ���ɹ�
	if (!IsInitOK())
	{
		return GT_INVALID;
	}

	//ͳ��ȷ�ϰ�����
	INT iGuildNum = 0;
	std::vector <tagCitySignUp*> vecConfirmDefence;
	for(INT iLoop = 0; iLoop<MAX_CITY_DEFENCE_FORCE; iLoop++)
	{
		if(dwGuildID[iGuildNum] != GT_INVALID)
		{
			++iGuildNum;
		}
	}
	if(iGuildNum == 0)
	{
		return GT_INVALID;
	}

	// ���ʱ��
	if (m_bIsCityWarInterval)
	{
		return E_City_Time_Limit;
	}

	//����������������ʣ���ȷ����
	INT32 nDefConfirmNum = 0;
	INT32 nTemp = 0;
	this->GetCityAppointedAtt(ECAT_DefenceConfirmNum,nTemp,&nDefConfirmNum);

	if (iGuildNum > (MAX_CITY_DEFENCE_FORCE - nDefConfirmNum) )
	{
		return E_City_SignUp_Force_Full;
	}

	//ռ��ó��еİ���
	if(!P_VALID(m_pGuild))
	{
		return E_City_Not_BeOccupied;
	}

	//���ȷ����Ҳ��ǰ���
	if(m_pGuild->GetGuildAtt().dwLeaderRoleID != pRole->GetID())
	{
		return E_City_Guild_NotMatch;
	}

	//ȷ��
	for(INT iLoop = 0; iLoop<MAX_CITY_DEFENCE_FORCE; iLoop++)
	{
		//������Ч�İ���id
		if(dwGuildID[iLoop] == GT_INVALID || dwGuildID[iLoop] == 0)
		{
			break;
		}

		// ��鱻���ϰ���
		Guild* pGuild = g_guildMgr.GetGuild(dwGuildID[iLoop]);
		if (!P_VALID(pGuild))
		{
			return E_City_SignUp_Info_Error;
		}

		// �����ȷ����Ϣ
		MapSignUp::iterator iter = m_mapDefenceForce.find(dwGuildID[iLoop]);
		if (iter == m_mapDefenceForce.end())
		{
			return E_City_SignUp_Info_Error;
		}
		//���ð����Ƿ��Ѿ���ȷ��
		if (iter->second->nFare != 0)
		{
			return E_City_SignUp_Already_Confirm;
		}
		//����ȷ���ж�
		vecConfirmDefence.push_back(iter->second);

		// �����ϰ��ɹ㲥
		const MapScript* pMapScript = g_ScriptMgr.GetMapScript(m_pKernelCity->GetMapID());
		if(P_VALID(pMapScript))
			pMapScript->OnCityBroadCast(ECBC_ReadyForDefence,m_pKernelCity->GetMapID(),dwGuildID[iLoop]);
	}
	//����ȷ���жӵ�ֵ
	std::vector<tagCitySignUp*>::iterator itLoop = vecConfirmDefence.begin();
	for(; itLoop != vecConfirmDefence.end(); ++itLoop)
	{
		// ���ϸð��ɵĳ�ս����Ȩ
		(*itLoop)->nFare = 1;
	}

	// �������ݿ�
	SaveAppointedAtt2DB(ECAT_DefenceSignUp);

	return E_Success;
}

//-------------------------------------------------------------------------
// ������Ϣ�ظ��ж�
//-------------------------------------------------------------------------
BOOL City::IsAlreadySignup( DWORD dwGuildID, ECityForceType eForce )
{
	switch (eForce)
	{
	case ECFT_Defence:
		if (m_mapDefenceForce.find(dwGuildID) != m_mapDefenceForce.end())
		{
			return TRUE;
		}
		break;

	case ECFT_Attack:
	case ECFT_Request:
		if (m_mapAttackForce.find(dwGuildID) != m_mapAttackForce.end())
		{
			return TRUE;
		}
		break;

	default:	// ���ͷǺϷ��ڱ��ж�
		break;
	}

	return FALSE;
}

//-------------------------------------------------------------------------
// ���ó�ս״̬
//-------------------------------------------------------------------------
VOID City::SetCityStruggleState(ECityStruggleState eState)
{
	m_CityStruggle.SetCityStruggleState(eState);
}

//-------------------------------------------------------------------------
// ���븴�����
//-------------------------------------------------------------------------
VOID City::AddToReviveMap(Role* pRole)
{
	m_CityStruggle.AddToReviveMap(pRole);
}

//-------------------------------------------------------------------------
// ��������ʱ��
//-------------------------------------------------------------------------
VOID City::SetReviveTime(ECityForceType eForce, INT nSec)
{
	m_CityStruggle.SetReviveTime(eForce, nSec);
}

//-------------------------------------------------------------------------
// �����ػ����ٻ�ʱ��
//-------------------------------------------------------------------------
VOID City::SetSummonTime(INT nSec)
{
	m_CityStruggle.SetSummonTime(nSec);
}

//-------------------------------------------------------------------------
// ��ȡ�����Ӫ
//-------------------------------------------------------------------------
BOOL City::GetForceType(Role* pRole, ECityForceType& eForce)
{
	return m_CityStruggle.GetForceType(pRole, eForce);
}

//-------------------------------------------------------------------------
// �Ƿ��ǳ�սʱ��
//-------------------------------------------------------------------------
EWarTimeType City::GetWarTimeType(const tagDWORDTime &tm)
{
	if (m_ActMode == EAM_Week)
	{
		//���������ĩ
		if(!m_bIsCityWarInterval)
		{
			return EWT_NOT_WARTIME_INTERNAL;
		}

		//����һ��
		if(WhichWeekday(tm) != m_WarTm.WarWeekDay)
		{
			//���������
			if(m_WarTm.WarWeekDay == 0)
			{
				return EWT_BEFOR_WAR;
			}
			//���������
			if(m_WarTm.WarWeekDay == 6)
			{
				return EWT_AFTER_WAR;
			}
		}

	}
	//��ս������
	if((m_WarTm.EndHour < tm.hour) || (m_WarTm.EndHour == tm.hour && m_WarTm.EndMinutemin < tm.min))
	{
		return EWT_AFTER_WAR;
	}

	//��û��ʼ
	if((m_WarTm.BegHour > tm.hour) || (m_WarTm.BegHour == tm.hour && m_WarTm.BegMinutemin > tm.min))
	{
		return EWT_BEFOR_WAR;
	}

	return EWT_WARING;
}
//-------------------------------------------------------------------------
// ��ĩ�ж�
//-------------------------------------------------------------------------
BOOL City::IsCityWarInterval(const tagDWORDTime &tm)
{
	if (m_ActMode == EAM_Day)				// ÿ�ջ
	{
		INT nWarTmStartHour = (m_WarTm.BegHour >= 1)? (m_WarTm.BegHour - 1):23;
		INT nWarTmStartMinute = m_WarTm.BegMinutemin;

		if (tm.hour >= nWarTmStartHour &&
			tm.hour <= m_WarTm.EndHour &&
			tm.min >= nWarTmStartMinute &&
			tm.min <= m_WarTm.EndMinutemin)
		{
			return TRUE;
		}
	}
	else if (m_ActMode == EAM_Week)			// ÿ�ܻ
	{
		DWORD dwWeekDay = WhichWeekday(tm);

		if (dwWeekDay == 0 || dwWeekDay == 6)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------
// �����а�������
//-------------------------------------------------------------------------
BOOL City::GetConfirmGuild( ECityForceType eForce, INT& nNum, DWORD* pGuildID )
{
	if (!P_VALID(pGuildID))	return FALSE;
	
	MapSignUp::iterator iter;

	switch (eForce)
	{
	case ECFT_Attack:
	case ECFT_Request:
		if (nNum < (INT)m_mapAttackForce.size())	
		{
			return FALSE;
		}
		nNum = 0;
		iter = m_mapAttackForce.begin();
		while (iter != m_mapAttackForce.end())
		{
			tagCitySignUp* pSignUp = iter->second;
			if (P_VALID(pSignUp))
			{
				pGuildID[nNum++] = pSignUp->dwGuildID;
			}

			iter++;
		}
		break;

	case ECFT_Defence:
		if (nNum <= (INT)m_mapDefenceForce.size())
		{
			return FALSE;
		}

		nNum = 0;
		if (P_VALID(m_pGuild))
		{
			pGuildID[nNum++] = m_pGuild->GetGuildAtt().dwID;
		}

		iter = m_mapDefenceForce.begin();
		while (iter != m_mapDefenceForce.end())
		{
			tagCitySignUp* pSignUp = iter->second;
			if (P_VALID(pSignUp))
			{
				pGuildID[nNum++] = pSignUp->dwGuildID;
			}

			iter++;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

//-------------------------------------------------------------------------
// ȡ��ָ�����ͳ������Դ�С
//-------------------------------------------------------------------------
INT City::GetCityAppointedAttSize( ECityAttType eType )
{
	if (!IsInitOK())
	{
		return 0;
	}

	INT nAttSize = 0;

	switch (eType)
	{
	case ECAT_CityID:
	case ECAT_ClanType:
	case ECAT_Holder:
	case ECAT_Defence:
	case ECAT_EudemonTally:
	case ECAT_TaxRate:
	case ECAT_TaxRateTime:
	case ECAT_Taxation:
	case ECAT_Prolificacy:
	case ECAT_FundOutput:
	case ECAT_MaterialOutput:
		nAttSize = sizeof(DWORD);
		break;

	case ECAT_ItemOutput:
		{
			tagCityItemOutput* pItemOutput = NULL;
			MapItemOutput::TMapIterator iter = m_mapItemOutput.Begin();
			while (m_mapItemOutput.PeekNext(iter, pItemOutput))
			{
				if (!P_VALID(pItemOutput))
				{
					continue;
				}

				if (pItemOutput->nLimit <= m_nProlificacy)
				{
					nAttSize += sizeof(tagCityItemOutput);
				}
			}
		}
		break;

	case ECAT_AttackSignUp:
	case ECAT_RequestSignUp:
		nAttSize = sizeof(tagCitySignUp) * m_mapAttackForce.size();
		break;

	case ECAT_DefenceSignUp:
		nAttSize = sizeof(tagCitySignUp) * m_mapDefenceForce.size();
		break;
	}

	return nAttSize;
}

//-------------------------------------------------------------------------
// �����Ƚ�
//-------------------------------------------------------------------------
bool ReputationCompare::operator()( const tagGuildClanValue& lhs, const tagGuildClanValue& rhs )
{
	Guild* pGuild_1	= g_guildMgr.GetGuild(lhs.dwGuildID);
	if (!P_VALID(pGuild_1))
	{
		return true;
	}
	Guild* pGuild_2	= g_guildMgr.GetGuild(rhs.dwGuildID);
	if (!P_VALID(pGuild_2))
	{
		return false;
	}

	// �ȽϽ��
	if (lhs.nClanValue < rhs.nClanValue)	return true;
	if (lhs.nClanValue == rhs.nClanValue)
	{
		const tagGuild& sGuildAtt_1 = pGuild_1->GetGuildAtt();
		const tagGuild& sGuildAtt_2 = pGuild_2->GetGuildAtt();	
		if (sGuildAtt_1.byLevel < sGuildAtt_2.byLevel)	return true;
		if (sGuildAtt_1.byLevel == sGuildAtt_2.byLevel)
		{
			if (sGuildAtt_1.nFund < sGuildAtt_2.nFund)	return true;
			if (sGuildAtt_1.nFund == sGuildAtt_2.nFund)
				return sGuildAtt_1.dwCreateTime < sGuildAtt_2.dwCreateTime;
			return false;
		}
		return false;
	}
	return false;
}
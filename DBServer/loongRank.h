//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong.cpp
// author: zr
// actor:
// data: 2009-12-28
// last:
// brief: ���а�
//-------------------------------------------------------------------------------------------------------

#pragma once
#include "../WorldDefine/msg_rankings.h"

class CLoongRank
{
public:
	CLoongRank();
	~CLoongRank();

	BOOL Init(LPCTSTR szIniFileName,BOOL bReload=FALSE);

	DWORD UpdateStrengthRankings();
	DWORD UpdateLevelRankings();
	DWORD UpdateFameRankings();
	DWORD UpdatePetRankings();
	DWORD UpdateEquipRankings();
	DWORD UpdateWuXunRankings();//��ѫ����
	DWORD UpdateLikeRankings();	//�ʻ���
	DWORD UpdateHateRankings();	//������

	// ��ְ��ѡ���
	DWORD UpdateClergyMars();
	DWORD UpdateClergyApollo();
	DWORD UpdateClergyRabbi();
	DWORD UpdateClergyPeace();
	VOID RefreshClergyCandidateType();

	// world_data���
	VOID RefreshWorldLogData();

	// ʵ�����п�����ز���
	BOOL RankingEnabled()					{ return m_bOn; }
	VOID Enable()							{ m_bOn = TRUE; }
	VOID SetStartTime(BYTE byClock)			{ m_byStartTime = byClock; }
	BOOL IsNeedLoad(ERankingType eType)
	{
		if (!m_bOn)
		{
			return FALSE;
		}

		switch(eType)
		{
		case ERT_Equip:
			return m_bEquipOn;
		case ERT_Fame:
			return m_bFameOn;
		case ERT_Level:
			return m_bLevelOn;
		case ERT_Pet:
			return m_bPetOn;
		case ERT_Strength:
			return m_bStrengthOn;
		case ERT_WuXun:  
            return m_bWuXunOn;
		case ERT_Flower:
			return m_bFlowerOn;
		case ERT_Egg:
			return m_bEggOn;
		default:
			return FALSE;
		}
	}

	VOID EnablePerRanking( BOOL bPet, BOOL bEquip, BOOL bFame, BOOL bLevel, BOOL bStrength ,BOOL bWuXun, BOOL bFlower, BOOL bEgg)	
	{
		m_bEquipOn = bEquip;
		m_bFameOn = bFame;
		m_bLevelOn = bLevel;
		m_bPetOn = bPet;
		m_bStrengthOn = bStrength;
		m_bWuXunOn=bWuXun;
		m_bFlowerOn = bFlower;
		m_bEggOn = bEgg;
	}

	VOID OnStart()
	{
		if(m_bFameOn)		UpdateFameRankings();
		if(m_bLevelOn)		UpdateLevelRankings();
		if(m_bPetOn)		UpdatePetRankings();
		if(m_bEquipOn)		UpdateEquipRankings();
		if(m_bStrengthOn)	UpdateStrengthRankings();
		if(m_bWuXunOn ) 	UpdateWuXunRankings();
		if(m_bFlowerOn)		UpdateLikeRankings();
		if(m_bEggOn)		UpdateHateRankings();
	}

private:
	Beton::DataBase*	m_pDBLoongRank;
	BOOL				m_bOn;
	BOOL                m_bWuXunOn;
	BOOL				m_bPetOn;
	BOOL				m_bStrengthOn;
	BOOL				m_bLevelOn;
	BOOL				m_bEquipOn;
	BOOL				m_bFameOn;
	BOOL				m_bFlowerOn;
	BOOL				m_bEggOn;
	BYTE				m_byStartTime;
};
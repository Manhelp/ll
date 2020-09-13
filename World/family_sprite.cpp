//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.cpp
// author: liutingting
// actor:
// data: 2011-03-24
// last:
// brief: ���巨��������ݲ���
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "family_sprite.h"
#include "../ServerDefine/msg_family_sprite.h"
#include "db_session.h"
#include "Role.h"
//---------------------------------------------------------------------------------------------------
FamilySprite::FamilySprite(DWORD dwFimilyID)
{	
	m_dwFimilyID = dwFimilyID;
	m_FamilySpriteData.un2Level = 1;
	m_FamilySpriteData.dwFamilyID = dwFimilyID;
	m_UpdateTraining = FALSE;
	ActivationFamilySpriteAbility();
}

//---------------------------------------------------------------------------------------------------
FamilySprite::~FamilySprite()
{
	Clear();
}

//----------------------------------------------------------------------------------------------------
VOID FamilySprite::SetName(const TCHAR *ptszName)
{
	if(!P_VALID(ptszName))
		return;

	_tcscpy_s(m_FamilySpriteData.tszName,X_SHORT_NAME, ptszName);
}

//----------------------------------------------------------------------------------------------------
//������е���������
VOID FamilySprite::ClearFamilySpriteTrain()
{
	FamilySpriteTrainMap::TMapIterator it = m_mapFamilySpriteTrain.Begin();
	tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = NULL;

	while( m_mapFamilySpriteTrain.PeekNext(it, pFamilySpriteRequirementSave) )
	{
		SAFE_DEL(pFamilySpriteRequirementSave);
	}
	m_mapFamilySpriteTrain.Clear();
}
//----------------------------------------------------------------------------------------------------
//������е��������ݵ�DB
VOID FamilySprite::ClearFamilySpriteTrain2DB()
{
	tagNDBC_DeleteALLFamilySpriteRequirement send;

	g_dbSession.Send(&send,send.dwSize);
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::Clear()
{
	RoleSpriteMap::TMapIterator it = m_mapRoleSprite.Begin();
	tagRoleSpriteData* pRoleSpriteData = NULL;

	while( m_mapRoleSprite.PeekNext(it, pRoleSpriteData) )
	{
		SAFE_DEL(pRoleSpriteData);
	}
	m_mapRoleSprite.Clear();

	ClearFamilySpriteTrain();
}
//----------------------------------------------------------------------------------------------------
//�Ƿ������������
BOOL FamilySprite::IsExistFamilySpriteTrain( DWORD dwID )
{
	return m_mapFamilySpriteTrain.IsExist(dwID);
}

//----------------------------------------------------------------------------------------------------
//���ü�����������
VOID FamilySprite::SetFamilySpriteData(tagFamilySpriteSave *pFamilySpriteSave)
{
	if(!P_VALID(pFamilySpriteSave))
		return;

	if(m_FamilySpriteData.dwFamilyID == pFamilySpriteSave->dwFamilyID)
		m_FamilySpriteData = *pFamilySpriteSave;	
}
//----------------------------------------------------------------------------------------------------
//���ü�������������������
VOID FamilySprite::SetFamilySpriteRequirementData( tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum )
{
	if(!P_VALID(pFamilySpriteRequirementSave))
		return;

	ClearFamilySpriteTrain();

	tagFamilySpriteRequirementSave *p = pFamilySpriteRequirementSave;
	for( int i = 0; i < nNum; ++i )
	{
		tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = new tagFamilySpriteRequirementSave();
		if( P_VALID(pFamilySpriteRequirementSave) )
		{
			pFamilySpriteRequirementSave->dwRequirementID = p->dwRequirementID;
			pFamilySpriteRequirementSave->nRet = p->nRet;
			pFamilySpriteRequirementSave->bComplete = p->bComplete;
			_tcscpy_s( pFamilySpriteRequirementSave->tszRoleName, X_SHORT_NAME, p->tszRoleName);
			m_mapFamilySpriteTrain.Add( p->dwRequirementID ,pFamilySpriteRequirementSave );
		}
		p++;
	}
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::UpdateFamilySpriteData2DB()
{
	tagNDBC_UpdateFamilySprite send;
	send.fs.dwFamilyID = m_FamilySpriteData.dwFamilyID;
	send.fs.un2Level = m_FamilySpriteData.un2Level;
	send.fs.n8Exp = m_FamilySpriteData.n8Exp;
	
	for( int i = 0; i < EFSA_Num; ++i )
	{
		send.fs.nAbility[i] = m_FamilySpriteData.nAbility[i];
	}

	_stprintf( send.fs.tszName, m_FamilySpriteData.tszName );

	g_dbSession.Send(&send,send.dwSize);
}
//----------------------------------------------------------------------------------------------------
//���¼���������������
VOID FamilySprite::UpdateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete, INT nRet, DWORD dwRoleID )
{
	tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = m_mapFamilySpriteTrain.Peek(dwTrainID);
	if( P_VALID(pFamilySpriteRequirementSave) )
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
		if (!P_VALID(pRoleInfo))
			return;

		_tcscpy_s( pFamilySpriteRequirementSave->tszRoleName, X_SHORT_NAME, pRoleInfo->szRoleName);
		pFamilySpriteRequirementSave->nRet = nRet;
		pFamilySpriteRequirementSave->bComplete = bComplete;
		UpdateFamilySpriteRequirementData2DB( dwTrainID, bComplete, nRet, pRoleInfo->szRoleName  );
	}
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::UpdateFamilySpriteRequirementData2DB( DWORD dwTrainID, BOOL bComplete, INT nRet, TCHAR *ptszRoleName )
{
	tagNDBC_UpdateFamilySpriteRequirement send;
	send.dwFamilyID = m_FamilySpriteData.dwFamilyID;
	send.fsr.dwRequirementID = dwTrainID;
	send.fsr.bComplete = bComplete;
	_tcscpy_s( send.fsr.tszRoleName, X_SHORT_NAME, ptszRoleName );
	send.fsr.nRet = nRet;
	g_dbSession.Send(&send,send.dwSize);

}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::CreateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete )
{
	if( m_mapFamilySpriteTrain.IsExist(dwTrainID) )
		return;

	tagFamilySpriteRequirementSave* pRoleSpriteTrainDataNew = new tagFamilySpriteRequirementSave();
	if( P_VALID(pRoleSpriteTrainDataNew) )
	{
		pRoleSpriteTrainDataNew->dwRequirementID = dwTrainID;
		pRoleSpriteTrainDataNew->bComplete = bComplete;
		m_mapFamilySpriteTrain.Add( dwTrainID, pRoleSpriteTrainDataNew );
	}
}
//----------------------------------------------------------------------------------------------------
//�����������������ݷ��͵�DB
VOID FamilySprite::CreateFamilySpriteRequirementData2DB()
{
	INT nNum = m_mapFamilySpriteTrain.Size();
	INT nMsgSz = sizeof(tagNDBC_CreateFamilySpriteRequirement) + sizeof(tagFamilySpriteRequirementSave) * (nNum-1);
	MCREATE_MSG(pSend, nMsgSz, NDBC_CreateFamilySpriteRequirement);

	pSend->dwFamilyID = m_dwFimilyID;
	pSend->nNum = nNum;
	
	tagFamilySpriteRequirementSave *p = pSend->fsr;
	FamilySpriteTrainMap::TMapIterator it = m_mapFamilySpriteTrain.Begin();
	tagFamilySpriteRequirementSave* pRoleSpriteTrainData = NULL;

	while( m_mapFamilySpriteTrain.PeekNext(it, pRoleSpriteTrainData) )
	{
		if( P_VALID(pRoleSpriteTrainData) )
		{
			p->dwRequirementID = pRoleSpriteTrainData->dwRequirementID;
			p->bComplete = pRoleSpriteTrainData->bComplete;
			p->nRet = pRoleSpriteTrainData->nRet;
			_tcscpy_s( p->tszRoleName, X_SHORT_NAME, pRoleSpriteTrainData->tszRoleName);
			p++;
		}	
	}

	g_dbSession.Send( pSend, pSend->dwSize );
	
	MDEL_MSG(pSend);
}
//----------------------------------------------------------------------------------------------------
//��ü���������������
DWORD FamilySprite::GetFamilySpriteRequirement(tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum)
{
	if( !P_VALID(pFamilySpriteRequirementSave) )
		return GT_INVALID;

	tagFamilySpriteRequirementSave *p = pFamilySpriteRequirementSave;
	FamilySpriteTrainMap::TMapIterator it = m_mapFamilySpriteTrain.Begin();
	tagFamilySpriteRequirementSave *pRoleSpriteTrainData = NULL;
	INT nSize = 0;

	while( m_mapFamilySpriteTrain.PeekNext(it, pRoleSpriteTrainData) && nSize < nNum )
	{
		if( !P_VALID(pRoleSpriteTrainData) )
			continue;

		_tcscpy_s( p->tszRoleName, X_SHORT_NAME, pRoleSpriteTrainData->tszRoleName);
		p->nRet = pRoleSpriteTrainData->nRet;
		p->dwRequirementID = pRoleSpriteTrainData->dwRequirementID;
		p->bComplete = pRoleSpriteTrainData->bComplete;

		p++;
		nSize++;
	}

	return E_Success;
}
//----------------------------------------------------------------------------------------------------
//�����������״̬
BOOL FamilySprite::IsFamilySpriteTrainComplete( DWORD dwID )
{
	tagFamilySpriteRequirementSave *pRoleSpriteTrainData = m_mapFamilySpriteTrain.Peek(dwID);
	if(P_VALID(pRoleSpriteTrainData))
	{
		return pRoleSpriteTrainData->bComplete;
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------
//��õ�ǰ����ֵ
INT	FamilySprite::GetAbility(EFamilySpriteAtt eType)
{
	if(eType < 0 || eType >= EFSA_Num)
		return 0;

	return m_FamilySpriteData.nAbility[eType];
}
//----------------------------------------------------------------------------------------------------
//���õ�ǰ����ֵ
VOID FamilySprite::SetAbility(EFamilySpriteAtt eType, INT nData)
{
	if(eType < 0 || eType >= EFSA_Num)
		return;

	m_FamilySpriteData.nAbility[eType] = nData;
}
//----------------------------------------------------------------------------------------------------
//ע����ҷ���
tagFabao *FamilySprite::RegisterRoleSprite( DWORD dwRoleID, tagFabao *pFabao )
{
	if(!P_VALID(pFabao))
		return NULL;

	tagFabao *pFabaoCur = NULL;
	tagRoleSpriteData *pRoleSpriteDataCur = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteDataCur))
	{
		tagRoleSpriteData *pRoleSpriteData = new tagRoleSpriteData( dwRoleID );
		if( P_VALID(pRoleSpriteData) )
		{
			tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
			if (P_VALID(pRoleInfo))
				_tcscpy_s( pRoleSpriteData->RoleSpriteDataSave.tszRoleName, X_SHORT_NAME, pRoleInfo->szRoleName);

			pRoleSpriteData->pFabao = pFabao;
			pRoleSpriteData->RoleSpriteDataSave.un2Level = pFabao->n16Stage;
			pRoleSpriteData->RoleSpriteDataSave.dwSpriteType = pFabao->dwTypeID;
			pRoleSpriteData->RoleSpriteDataSave.bQuality = pFabao->equipSpec.byQuality;


			m_mapRoleSprite.Add(dwRoleID,pRoleSpriteData);

			UpdateRoleAtt( dwRoleID );

			tagNDBC_CreateRoleSpriteData send;
			send.dwFamilyID = m_dwFimilyID;
			send.rsd = pRoleSpriteData->RoleSpriteDataSave;
			g_dbSession.Send( &send, send.dwSize );
		}
	}
	else
	{
		pFabaoCur = pRoleSpriteDataCur->pFabao;
		pRoleSpriteDataCur->pFabao = pFabao;
		pRoleSpriteDataCur->RoleSpriteDataSave.un2Level = pFabao->n16Stage;
		pRoleSpriteDataCur->RoleSpriteDataSave.dwSpriteType = pFabao->dwTypeID;
		pRoleSpriteDataCur->RoleSpriteDataSave.bQuality = pFabao->equipSpec.byQuality;

		UpdateRoleAtt( dwRoleID );

		tagNDBC_UpdateRoleSpriteData send;
		send.dwFamilyID = m_dwFimilyID;
		send.rsd = pRoleSpriteDataCur->RoleSpriteDataSave;
		g_dbSession.Send( &send, send.dwSize );

		return pFabaoCur;
	}

	return NULL;
}
//----------------------------------------------------------------------------------------------------
//������ҷ����������÷��������û��Ҳ���½�
VOID FamilySprite::SetRoleSprite(DWORD dwRoleID, tagFabao *pFabao)
{
	if(!P_VALID(pFabao))
		return;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(P_VALID(pRoleSpriteData))
	{	
		pRoleSpriteData->pFabao = pFabao;
	}
}
//----------------------------------------------------------------------------------------------------
//���㹲��ֵ
FLOAT FamilySprite::CaculateGongming( DWORD dwRoleID )
{
//���ȼ���Ǽ������ȼ����ػ������ȼ���ϵ��A
//ϵ��A=�Ǽ������ȼ����ػ�������ǰ�ȼ���ȡֵ��0%-100%
//�ټ���Ǽ������ȼ����������ǰ�ȼ���ϵ��B
//ϵ��B=�Ǽ������ȼ�����ҵ�ǰ�ȼ���ȡֵ��0%-100%
//�ټ���Ǽ�����ʵ��ֵ��߻��趨��ʵ��ֵ���޵�ϵ��C
//ϵ��C=�Ǽ�����ʵ��ֵ/�߻��趨ֵ��ȡֵ��0-110%
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return 0;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return 0;

	tagFabao *pFabao = pRoleSpriteData->pFabao;
	if(!P_VALID(pFabao))
		return 0;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
		return 0;

	//����Ǽ������ȼ����ػ������ȼ���ϵ��
	FLOAT fA = (FLOAT)( (FLOAT)pFabao->n16Stage / (FLOAT)m_FamilySpriteData.un2Level );
	if( fA > 1.0f )
		fA = 1.0f;
	
	if( fA < 0.000001f )
		fA = 0.0f;

	//����Ǽ������ȼ����������ǰ�ȼ���ϵ��
	FLOAT fB = (FLOAT)( (FLOAT)pFabao->n16Stage / (FLOAT)pRole->GetLevel() );
	if( fB > 1.0f )
		fB = 1.0f;
	
	if( fB < 0.000001f )
		fB = 0.0f;

	//����Ǽ�����ʵ��ֵ��߻��趨��ʵ��ֵ���޵�ϵ��
//����*Ʒ��*3/2 + 5�����Ӻ�(�������ȼ�^2 + �������ȼ�*5) + ������ǰ�ȼ�*3+����Ƕ�ı�ʯ�Ӻͣ���ʯ�ȼ�*��Ӧ��Ȩ*4��

	//const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFabao->n16Stage);
	//if(!P_VALID(pFamilySpriteProto))
	//	return 0;

	INT nWuxingData = 0;
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{
		const tagItemProto *pItemProto = g_attRes.GetItemProto( pFabao->dwWuxingBeadID[i] );
		if(!P_VALID(pItemProto))
			continue;

		BYTE byLevel = pItemProto->byLevel / 10;
		nWuxingData += (byLevel * byLevel + byLevel * 5);
	}

	//Ӳ���룬ֻ���������������а��Ѿ����ڵĶ���
	INT nWeightedlValue[] = {1,1,
		2,1,
		3,1, 
		4,2,
		5,3,
		6,3,
		7,4,
		8,6,
		9,8,
		10,10};

	INT nGemData = 0;
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{
		const tagItemProto *pItemProto = g_attRes.GetItemProto( pFabao->equipSpec.dwHoleGemID[i] );
		if(!P_VALID(pItemProto))
			continue;

		nGemData += ( pItemProto->byLevel * nWeightedlValue[(pItemProto->byLevel-1)*2+1] * 4 );
	}

	FLOAT fShili = ( (FLOAT)pFabao->n16NativeIntelligence * pFabao->equipSpec.byQuality * 3 / 2 + nWuxingData + pFabao->n16Stage * 3 + nGemData );

	const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFabao->n16Stage);
	if(!P_VALID(pFamilySpriteProto))
		return 0;
	FLOAT fC = fShili / pFamilySpriteProto->dwShedingzhi;

//���յĹ����̶�=ϵ��A^2��ϵ��B^1.5��ϵ��C����λΪ��%����ȡֵ��0%-150%֮�䣬ȡС�����1λ
	FLOAT fGongming = pow(fA,2.0f) * pow(fB,1.5f) * fC;

	if (fGongming >= 100)
      fGongming = 66.6;

	pRoleSpriteData->RoleSpriteDataSave.fGongming = fGongming;
	pRoleSpriteData->RoleSpriteDataSave.fShili = fShili;

	return fGongming;
}
//----------------------------------------------------------------------------------------------------
//����������Լӳ�
VOID FamilySprite::CaculateRoleAtt( DWORD dwRoleID, Role *pRoleIn/*=NULL*/ )
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return;

	Role* pRole = pRoleIn;
	if(!P_VALID(pRole))
	{
		pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(!P_VALID(pRole))
			return;
	}

	tagRoleSpriteData *pRoleSpriteDataCur = m_mapRoleSprite.Peek(dwRoleID);
	if(P_VALID(pRoleSpriteDataCur))
	{
		pRole->SetRoleFamilySpriteAtt( pRoleSpriteDataCur->RoleSpriteDataSave.fGongming, m_FamilySpriteData.nAbility );
	}
}
//----------------------------------------------------------------------------------------------------
//������ҽ�ɫ������
VOID FamilySprite::UpdateAllRoleAtt()
{
	RoleSpriteMap::TMapIterator iter = m_mapRoleSprite.Begin();
	while( iter != m_mapRoleSprite.End() )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(iter->first);
		if(!P_VALID(pRole))
		{
			iter++;
			continue;
		}

		CaculateGongming( iter->first );

		CaculateRoleAtt(iter->first);

		pRole->ReCalRoleAtt();

		iter++;
	}
}
//----------------------------------------------------------------------------------------------------
//������ҽ�ɫ������
VOID FamilySprite::UpdateRoleAtt( DWORD dwRoleID )
{
	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
		return;

	CaculateGongming( dwRoleID );

	CaculateRoleAtt(dwRoleID);

	pRole->ReCalRoleAtt();
}
//----------------------------------------------------------------------------------------------------
//�Ƴ���ҽ�ɫ����
tagFabao* FamilySprite::RemoveRoleSprite(DWORD dwRoleID)
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return NULL;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return NULL;

	tagFabao *pFabao = pRoleSpriteData->pFabao;
	SAFE_DELETE(pRoleSpriteData);
	m_mapRoleSprite.Erase(dwRoleID);

	ClearRoleAtt(dwRoleID);

	tagNDBC_DeleteRoleSpriteData send;
	send.dwRoleID = dwRoleID;
	g_dbSession.Send( &send, send.dwSize );

	return pFabao;
}
//----------------------------------------------------------------------------------------------------
//��ȡ��ҽ�ɫ����
tagFabao* FamilySprite::GetRoleSprite(DWORD dwRoleID)
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return NULL;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(P_VALID(pRoleSpriteData))
		return pRoleSpriteData->pFabao;

	return NULL;
}
//----------------------------------------------------------------------------------------------------
//��ȡ��ҵǼǵ�����
DWORD FamilySprite::GetFamilyRoleSprite( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNum )
{
	if(!P_VALID(pRoleSpriteDataSave))
		return GT_INVALID;

	tagRoleSpriteDataSave *p = pRoleSpriteDataSave;
	RoleSpriteMap::TMapIterator it = m_mapRoleSprite.Begin();

	tagRoleSpriteData *pRoleSpriteData = NULL;
	INT nSize = 0;
	while( m_mapRoleSprite.PeekNext(it, pRoleSpriteData) && nSize < nNum )
	{
		if(!P_VALID(pRoleSpriteData->pFabao))
			continue;

		*p = pRoleSpriteData->RoleSpriteDataSave;

		p++;
		nSize++;
	}

	return E_Success;
}
//----------------------------------------------------------------------------------------------------
//�������������Ϣ
VOID FamilySprite::SetRoleSpriteInfo( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite )
{
	if(!P_VALID(pRoleSpriteDataSave))
		return;

	tagRoleSpriteDataSave *p = pRoleSpriteDataSave;
	for( int i = 0; i < nNumSprite; ++i )
	{
		if(!P_VALID(p))
			return;

		tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(p->dwRoleID);
		if(P_VALID(pRoleSpriteData))
		{	
			pRoleSpriteData->RoleSpriteDataSave = *p;
			UpdateRoleAtt( p->dwRoleID );
		}
		else
		{
			tagRoleSpriteData *pRoleSpriteDataNew = new tagRoleSpriteData(p->dwRoleID);
			pRoleSpriteDataNew->RoleSpriteDataSave = *p;
			m_mapRoleSprite.Add(p->dwRoleID,pRoleSpriteDataNew);
			UpdateRoleAtt( p->dwRoleID );
		}
		p++;
	}
}
//----------------------------------------------------------------------------------------------------
//�Ƴ�������ҽ�ɫ����
VOID FamilySprite::RemoveAllRoleSprite( )
{
	RoleSpriteMap::TMapIterator it = m_mapRoleSprite.Begin();
	tagRoleSpriteData* pRoleSpriteData = NULL;

	while( m_mapRoleSprite.PeekNext(it, pRoleSpriteData) )
	{
		if(P_VALID(pRoleSpriteData))
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pRoleSpriteData->RoleSpriteDataSave.dwRoleID);
			if( P_VALID(pRole) )
			{
				tagNDBC_DeleteRoleSpriteData send;
				send.dwRoleID = pRoleSpriteData->RoleSpriteDataSave.dwRoleID;
				g_dbSession.Send( &send, send.dwSize );

				tagFabao *pFabao = pRoleSpriteData->pFabao;
				m_mapRoleSprite.Erase(pRoleSpriteData->RoleSpriteDataSave.dwRoleID);
				SAFE_DELETE(pRoleSpriteData);

				ClearRoleAtt(pRole->GetID());

				pRole->AddRoleSpriteFromFamily(pFabao);
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
//��������
VOID FamilySprite::ActivationFamilySpriteAbility()
{
	const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(m_FamilySpriteData.un2Level);
	if( !P_VALID(pFamilySpriteProto) ) 
		return;

	for( int i = 0; i < EFSA_Num; ++i )
	{
		if( pFamilySpriteProto->dwAbilityUpperLimit[i] != -1 )
		{
			if( -1 == m_FamilySpriteData.nAbility[i] )
			{
				m_FamilySpriteData.nAbility[i] = 0;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::AddExp( int nAddExp )
{
	const tagFamilySpriteProto *pFamilySpriteProtoCur = g_attRes.GetFamilySpriteProto(GetLevel());
	if( !P_VALID(pFamilySpriteProtoCur) ) 
		return;

	SetExp(GetExp() + nAddExp);
	INT nRet = GetExp() - pFamilySpriteProtoCur->dwExp;
	while( nRet >= 0 )
	{
		const tagFamilySpriteProto *pFamilySpriteProtoNext = g_attRes.GetFamilySpriteProto( GetLevel() + 1 );
		if( P_VALID(pFamilySpriteProtoNext) ) 
		{
			SetLevel( GetLevel() + 1 );
			SetExp(nRet);

			//������ʱ����Ҫ���һ����û�б����������
			ActivationFamilySpriteAbility();
		}
		else
		{
			SetExp(nRet);
			break;
		}

		const tagFamilySpriteProto *pFamilySpriteProtoTemp = g_attRes.GetFamilySpriteProto(GetLevel());
		if( !P_VALID(pFamilySpriteProtoTemp) ) 
			break;

		nRet = GetExp() - pFamilySpriteProtoTemp->dwExp;
	}

	//���¼����������ݵ�DB
	UpdateFamilySpriteData2DB();
}
//----------------------------------------------------------------------------------------------------
//���������Լӳ�
VOID FamilySprite::ClearRoleAtt( DWORD dwRoleID )
{
	//if( !m_mapRoleSprite.IsExist(dwRoleID) )
	//	return;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
		return;

	pRole->ClearRoleFamilySpriteAtt();
	pRole->ReCalRoleAtt();
}
//----------------------------------------------------------------------------------------------------
//�Ƴ���ҽ�ɫ����(��ɫ����״̬)
VOID FamilySprite::RemoveRoleSpriteOffline(DWORD dwRoleID)
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return;

	SAFE_DELETE(pRoleSpriteData);
	m_mapRoleSprite.Erase(dwRoleID);

	tagNDBC_DeleteRoleSpriteData send;
	send.dwRoleID = dwRoleID;
	g_dbSession.Send( &send, send.dwSize );
}
//----------------------------------------------------------------------------------------------------













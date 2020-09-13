//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.h
// author: liutingting
// actor:
// data: 2011-03-24
// last:
// brief: ���巨��������ݲ���
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/family_define.h"
#define		FAMILY_NAME_MAX		16
//---------------------------------------------------------------------------
struct tagFabao;

typedef		TMap<DWORD, tagFamilySpriteRequirementSave*>					FamilySpriteTrainMap;

struct tagRoleSpriteData
{
	tagFabao				*pFabao;
	tagRoleSpriteDataSave	RoleSpriteDataSave;

	tagRoleSpriteData( DWORD dwInRoleID )
	{
		Clear();
		RoleSpriteDataSave.dwRoleID = dwInRoleID;
	}

	~tagRoleSpriteData()
	{
		Clear();
	}

	VOID Clear()
	{
		RoleSpriteDataSave.Clear();
		pFabao = NULL;
	}
};

typedef		TMap<DWORD, tagRoleSpriteData*>		RoleSpriteMap;

class FamilySprite
{
public:
	FamilySprite(DWORD dwFimilyID);
	virtual ~FamilySprite();

	//���ü�����������
	VOID	SetName(const TCHAR *ptszName);

	//������е���������
	VOID	ClearFamilySpriteTrain();

	//������е��������ݵ�DB
	VOID	ClearFamilySpriteTrain2DB();

	//���
	VOID	Clear();

	//�Ƿ������������
	BOOL	IsExistFamilySpriteTrain( DWORD dwID );

	//���ü�����������
	VOID	SetFamilySpriteData(tagFamilySpriteSave *pFamilySpriteSave);

	//���ü�������������������
	VOID	SetFamilySpriteRequirementData(tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum);

	//���¼����������ݵ�DB
	VOID	UpdateFamilySpriteData2DB();

	//��������������������
	VOID	CreateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete );

	//�����������������ݷ��͵�DB
	VOID	CreateFamilySpriteRequirementData2DB();
	
	//���¼���������������
	VOID	UpdateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete, INT nRet, DWORD dwRoleID );

	//��ü�������������������
	INT		GetFamilySpriteRequirementNum(){return m_mapFamilySpriteTrain.Size();}

	//��ü���������������
	DWORD	GetFamilySpriteRequirement(tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum);

	//�����������״̬
	BOOL	IsFamilySpriteTrainComplete( DWORD dwID );

	//��õ�ǰ����ֵ
	INT		GetAbility(EFamilySpriteAtt eType);

	//���õ�ǰ����ֵ
	VOID	SetAbility(EFamilySpriteAtt eType, INT nData);

	//�����������飬�㶮��
	VOID	SetExp(INT64 n8Exp){m_FamilySpriteData.n8Exp = n8Exp;}
	INT64	GetExp(){return m_FamilySpriteData.n8Exp;}

	//���������ȼ����㶮��
	VOID	SetLevel(UINT16 un2Level){m_FamilySpriteData.un2Level = un2Level;}
	UINT16	GetLevel(){return m_FamilySpriteData.un2Level;}

	//��ȡ����������������
	tagFamilySpriteSave* GetFamilySpriteData(){return &m_FamilySpriteData;}

	//ע����ҷ���
	tagFabao *RegisterRoleSprite( DWORD dwRoleID, tagFabao *pFabao );

	//ȡ����ҷ���
	tagFabao* ReturnRoleSprite( DWORD dwRoleID );

	//�ж�����Ƿ��Ѿ��еǼǵķ�����
	BOOL	HaveRoleSprite( DWORD dwRoleID ){return m_mapRoleSprite.IsExist(dwRoleID);}

	//������ҷ���
	VOID	SetRoleSprite(DWORD dwRoleID, tagFabao *pFabao);

	//������ҽ�ɫ������
	VOID	UpdateAllRoleAtt();

	//������ҽ�ɫ������
	VOID	UpdateRoleAtt( DWORD dwRoleID );

	//�Ƴ���ҽ�ɫ����
	tagFabao*	RemoveRoleSprite(DWORD dwRoleID);

	//�Ƴ���ҽ�ɫ����(��ɫ����״̬)
	VOID		RemoveRoleSpriteOffline(DWORD dwRoleID);

	//��ȡ��ҽ�ɫ����
	tagFabao*	GetRoleSprite(DWORD dwRoleID);

	//��ȡ��ҵǼǵ���������
	INT		GetFamilyRoleSpriteNum(){return m_mapRoleSprite.Size();}

	//��ȡ��ҵǼǵ�����
	DWORD	GetFamilyRoleSprite( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNum );

	//���ø��¼�������������־
	VOID	SetUpdateSpriteRequirementFlag(BOOL bFlag){m_UpdateTraining = bFlag;} 
	BOOL	GetUpdateSpriteRequirementFlag(){ return m_UpdateTraining; } 

	//�������������Ϣ
	VOID	SetRoleSpriteInfo( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite );

	//�Ƴ�������ҽ�ɫ����
	VOID	RemoveAllRoleSprite();

	//����������Լӳ�
	VOID	CaculateRoleAtt( DWORD dwRoleID, Role *pRoleIn = NULL );

	//��������
	VOID	ActivationFamilySpriteAbility();

	//���Ӽ�����������
	VOID	AddExp( int nAddExp );

	//���������Լӳ�
	VOID	ClearRoleAtt( DWORD dwRoleID );

private:
	//���¼��������������ݵ�DB
	VOID	UpdateFamilySpriteRequirementData2DB( DWORD dwTrainID, BOOL bComplete, INT nRet, TCHAR *ptszRoleName );

	//���㹲��ֵ
	FLOAT	CaculateGongming( DWORD dwRoleID );

private:
	tagFamilySpriteSave		m_FamilySpriteData;					//������������
	FamilySpriteTrainMap	m_mapFamilySpriteTrain;				//�洢��ǰ������������������
	RoleSpriteMap			m_mapRoleSprite;					//��ҽ�ɫ�Ǽǵ�����
	BOOL					m_UpdateTraining;					//���¼�������������־
	DWORD					m_dwFimilyID;						//����ID
	
};


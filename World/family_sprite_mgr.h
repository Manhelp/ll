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
#include "../ServerDefine/msg_family_sprite.h"
#define		FAMILY_NAME_MAX		16
//---------------------------------------------------------------------------
class FamilySprite;
typedef		TMap<DWORD, FamilySprite*>		FamilySpriteMap;
struct tagFabao;

class FamilySpriteMgr
{
public:
	FamilySpriteMgr();
	virtual ~FamilySpriteMgr();

	//���
	VOID	Clear();

	//Update
	VOID	Update();

	//�������������������
	VOID	ClearAllFamilySpriteRequirement();

	//����һ����������
	FamilySprite*	CreateFamilySprite( DWORD dwFamilyID, const TCHAR *pszName );

	//��һ����������
	FamilySprite*	FindFamilySprite( DWORD dwFamilyID );

	//��һ������Ǽ�����
	tagFabao*	FindRoleFamilySprite( DWORD dwFamilyID, DWORD dwRoleID );
	
	//ȥ��һ����������
	VOID	DelFamilySprite(DWORD dwFamilyID);

	//ȫ�����
	VOID	SaveAll2DB();

	//�ӿ��ʼ��
	VOID	InitFromDB(tagFamilySpriteSave *pFamilySpriteSave,tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNumRequirement, tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite);

private:
	//�����ػ���������������
	VOID	CreateFamilySpriteRequirement();
	VOID	CreateFamilySpriteRequirement(DWORD dwFamilyID);

	//�����ü���������������
	VOID	RandGetSpriteTrain( FamilySprite *pFamilySprite, DWORD dwPickTypeNum1, DWORD dwPickTypeNum2 );


private:
	FamilySpriteMap		m_mapFamilySprite;	//��������map
	tagDWORDTime		m_dwRequirementTime;

};

extern FamilySpriteMgr g_FamilySpriteMgr;
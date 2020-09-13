//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.h
// author: liutingting
// actor:
// data: 2011-01-19
// last:
// brief: ��ְ����������ݲ���
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/msg_god.h"
#include "world.h"
#include "global_limited_item.h"

//-----------------------------------------------------------------------------
typedef TMap<DWORD,BOOL> MapArea;
class Role;

struct tagAreaInfo
{
	DWORD	dwAreaID;		//����ID
	DWORD	dwCD;			//CD
	BOOL	bActive;		//�Ƿ��Ѽ���
};

typedef TMap<DWORD,tagAreaInfo*> MapAreaInfo;

class AreaMgr
{
public:
	AreaMgr(Role *pRole);
	virtual ~AreaMgr();

	//�ı�����
	DWORD	ChangeArea( DWORD dwAreaID, BOOL bAdd, tagGlobalLimitedItem *pGlobalLimitedItem = NULL, BOOL bDelGodFaith = TRUE, BOOL bJudgePre =TRUE );
	
	//�����ְ����
	VOID	ClearClergyArea();

	//��ʼ����ְ����
	DWORD	InitGodArea();

	//�ı���������
	DWORD	ChangeAreaAbility( DWORD dwAreaID, BOOL bActive, BOOL bByCD = TRUE );

	//�����ݿ��ʼ��
	DWORD	InitFromDB( const BYTE* &pData, INT nNum );

	//�洢�����ݿ�
	DWORD	Save2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	//�������ݵ��ͻ���
	VOID	SendInitStateArea();

	//�������һ��ϡ����������,ʵ�ʾ��Ǽ��ٵ�һ���ҵ���
	DWORD	RandCancleAreaAbility();

	//�������һ��ϡ������,ʵ�ʾ��Ǽ��ٵ�һ���ҵ���
	DWORD	RandDelArea();

	//����Ѽ����ĳ����������
	INT		GetAbilityAreaNum(EGodAreaType eType,BOOL bAll=TRUE);

	//�������CDʱ��
	DWORD	GetAreaCD(DWORD dwAreaID);

	VOID	Update();

	BOOL	IsHaveGodArea(DWORD dwAreaID) { return m_mapGodArea.IsExist(dwAreaID); }

	//ȥ��ϡ��������Чbuff
	VOID	RemoveAreaBuff();
	//ȥ������ϡ������buff
	VOID	RemoveAllAreaBuff();

	VOID AddAreaBuff();//���ϡ������buff add by xp

	DWORD	GetRandRareArea();

	BOOL	IsHaveArea(DWORD dwAreaID);

	BOOL	DirectDelOneRareArea(DWORD dwAreaID);

private:
	//��������
	DWORD	AddArea( DWORD dwAreaID, tagGlobalLimitedItem *pGlobalLimitedItem = NULL, BOOL bDelGodFaith = TRUE, BOOL bJudgePre = TRUE  );

	//��������
	DWORD	DelArea( DWORD dwAreaID );

	//ֱ�������������û��������������
	DWORD	SetArea( DWORD dwAreaID, BOOL bActive, DWORD dwCD = 0 );

	//�Ƿ���Լ������������
	DWORD	CanActivationAreaAbility( DWORD dwAreaID, BOOL bByCD = TRUE );

	//������������
	DWORD	ActivationAreaAbility( DWORD dwAreaID, BOOL bByCD = TRUE );

	//�Ƿ����ȡ������������
	DWORD	CanCancleAreaAbility(DWORD dwAreaID, BOOL bByCD = TRUE);

	//ȡ����������
	DWORD	CancleAreaAbility( DWORD dwAreaID, BOOL bByCD = TRUE );

	BOOL	DelSameArea(DWORD dwAreaID);

	BOOL	CancleAreaAbilityAndDelArea(DWORD dwAreaID);

	DWORD	CanAddArea(DWORD dwAreaID);

	VOID	SetAreaEffect( INT nNumPre, INT nNumCur );

private:
	Role			*m_pRole;				//��ɫָ��
	MapArea			m_mapGodArea;			//��ְ���򣬸�������Դ����ְ����
	MapAreaInfo		m_mapOptionalArea;		//��ѡ����
	MapAreaInfo		m_mapRareArea;			//ϡ������

	DWORD			m_dwLastTime; 
	DWORD			m_dwCurTime;
};


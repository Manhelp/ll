//-----------------------------------------------------------------------------
//!\file	
//!\brief	
//!
//!\date	
//! last	
//!\author	
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "client_db.h"
#include "tool_define.h"
#include "RoleInfoReader.h"
#include "section.h"

#define TSectionMap		TMap<DWORD,	Section*>

enum Eorder
{
	EO_XMLSelected,		//xml�ļ���ѡ��
	EO_Merge,			//��ʼ�Ϸ�
};

class CTool
{
public:
	BOOL	Init();
	BOOL	Control( INT nOrderID, INT nParaNum, ...);				//���߼�
	BOOL	DoMerge();
	
	VOID	LoadRoleFromXml(LPCTSTR);

	BOOL	SaveAccount();
	BOOL	SaveAccountDetail(const tagRole*, DWORD);
	BOOL	SaveRoleData();
	BOOL	SaveRoleDataDetail(const tagRole*, DWORD, DWORD);
	BOOL	SaveEquip(const tagRole*);
	BOOL	SaveSkill(const tagRole*);
	BOOL	SaveTitle(const tagRole*);
	BOOL	SaveVisitingCard(const tagRole*);
	//BOOL	SaveSkill(tagRole*) {return TRUE;}
	BOOL	SaveAccountCommon();
	//BOOL	SaveEquipDetail(tagRole*, BYTE, tagTWrap_Loong_Item*);

	BOOL	CheckRoleName(tagRole*&, DWORD, DWORD&);
	DWORD	GetRoleAccountID(const tagRole*);

public:
	ClientDB*						m_pdb1;					// Ŀ�����ݿ⣨�������ɵ����ݿ⣩��LoginDB
	ClientDB*						m_pdb2;					// Ŀ�����ݿ��LoongDB

private:
	TSFPTrunk<CTool>				m_Trunk;
	TObjRef<VarContainer>			m_pVar;
	TObjRef<Log>					m_pLog;
	TObjRef<Util>					m_pUtil;
	CRoleInfoReader					m_RoleInfoReader;
	volatile INT64					m_n64ItemIDGen;			// ��ƷID��������ʹ��ǰ��1

	TSectionMap						m_mapSection;
	INT								m_nSectionNum;
};

extern CTool g_Tool;
//-----------------------------------------------------------------------------
//!\file	client_db.h
//!\brief	�ͻ������ݿ������
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

struct tagRole
{
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szAccoutName[X_SHORT_STRING];
	TCHAR	szRoleName[X_SHORT_STRING];
	DWORD	dwOldID;							// ��ԭ���ϵ�RoleID
	DWORD	dwNewID;							// ���·��ϵ�RoleID

	tagRole()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//-----------------------------------------------------------------------------
// �ͻ������ݿ������
//-----------------------------------------------------------------------------
class ClientDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ClientDB(){}
	~ClientDB(){}

	//-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
	BOOL		Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort);
	VOID		Destroy();

	//DataBase	GetDB() { return m_DB; }
	DWORD		GetRoleOldID(tagRole*);

public:
	DataBase		m_DB;		// Mysql���ݿ�
	TObjRef<Util>	m_pUtil;	// Util
};
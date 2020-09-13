//-----------------------------------------------------------------------------
//!\file	client_db.cpp
//!\brief	�ͻ������ݿ������
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "client_db.h"

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ClientDB::Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort)
{
	m_pUtil = "Util";

	BOOL bRet = m_DB.Init(strHost, strUser, strPassword, strDatabase, nPort);

	return bRet;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID ClientDB::Destroy()
{

}

DWORD ClientDB::GetRoleOldID(tagRole* pRole)
{
	// ��ȡһ��Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	// ��ȡһ������
	Connection* pCon = m_DB.GetFreeConnection();

	QueryResult* pResult = NULL;

	pStream->SetSelect("roledata", "RoleID");
	pStream->SetWhere();
	pStream->FillString("RoleName='").FillString(pRole->szRoleName).FillString("'");

	pResult = m_DB.Query(pStream);

	DWORD dwErr = GetLastError();

	if( P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		DWORD dwRoleID = (*pResult)[0].GetDword();
		return dwRoleID;
	}
	else
	{
		// to do ����
		return GT_INVALID;
	}
	

	// �ͷ���Դ
	m_DB.FreeQueryResult(pResult);
	m_DB.ReturnConnection(pCon);
	m_DB.ReturnStream(pStream);
}
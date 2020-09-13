//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_role.cpp
// author: Sxg
// actor:
// data: 2009-05-04
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- ��ɫ��(roledata)�������
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleName(DWORD dwAccountID, DWORD dwRoleID, LPCTSTR szNewRoleName)
{
	BOOL bRet = TRUE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	// ��ʽ������
	pStream->SetUpdate("roledata");
	pStream->FillString("RoleName='").FillString(szNewRoleName, pCon);
	pStream->FillString("',RoleNameCrc=") << m_pUtil->Crc32(szNewRoleName);
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;

	// �ͷ���������
	m_pDBLoong->ReturnConnection(pCon);

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);
	
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ��ɫʹ�ù���������ʷ��¼
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveNameHistory(DWORD dwAccountID, DWORD dwRoleID, LPCTSTR szNewRoleName)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	
	pStream->SetSelect("name_history", "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		INT i = 9;
		while ( i > 0 )
		{
			char* strName = NULL;
			char szTmpColume1[X_SHORT_STRING] = {0};
			sprintf( szTmpColume1, "name_%d", i );
			
			pStream->Clear();
			pStream->SetSelect("name_history", szTmpColume1);
			pStream->SetWhere();
			pStream->FillString("RoleID=") << dwRoleID;
			pStream->FillString(" and AccountID=") << dwAccountID;

			// ��ѯ���ݿ�
			Beton::QueryResult* pRes = m_pDBLoong->Query(pStream);
			if(P_VALID(pRes) && pRes->GetRowCount() > 0)
			{
				strName = (char *)(*pRes)[0].GetString();
				if( *strName == '0' )
				{
					i--;
					m_pDBLoong->FreeQueryResult(pRes);
					continue;
				}
			}
			else // ����ִ�е�����
			{
				if(P_VALID(pRes)) m_pDBLoong->FreeQueryResult(pRes);
				m_pDBLoong->ReturnConnection(pCon);
				m_pDBLoong->ReturnStream(pStream);
				return FALSE;
			}

			// ��λд
			char szTmpColume2[X_SHORT_STRING] = {0};
			sprintf( szTmpColume2, "name_%d", i+1 );
			pStream->Clear();
			pStream->SetUpdate("name_history");
			pStream->FillString(szTmpColume2).FillString("='").FillString(strName,pCon).FillString("'");
			pStream->SetWhere();
			pStream->FillString("RoleID=") << dwRoleID;
			pStream->FillString(" and AccountID=") << dwAccountID;

			m_pDBLoong->FreeQueryResult(pRes);
			// ִ��
			m_pDBLoong->Execute(pStream);

			i--;
		}

		// д������ʹ�õ�����
		pStream->Clear();
		pStream->SetUpdate("name_history");
		pStream->FillString("name_1='").FillString(szNewRoleName, pCon).FillString("'");
		pStream->SetWhere();
		pStream->FillString("RoleID=") << dwRoleID;
		pStream->FillString(" and AccountID=") << dwAccountID;

		// ִ��
		bRet = m_pDBLoong->Execute(pStream);
	}
	else
	{
		// д������ʹ�õ�����
		pStream->Clear();
		pStream->SetInsert("name_history");
		pStream->FillString("name_1='").FillString(szNewRoleName, pCon).FillString("'");
		pStream->FillString(",RoleID=") << dwRoleID;
		pStream->FillString(",AccountID=") << dwAccountID;

		// ִ��
		bRet = m_pDBLoong->Execute(pStream);
	}

	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫ����id
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleGuild(DWORD dwRoleID, DWORD dwGuildID)
{
	return Update(dwRoleID, &dwGuildID, 1, 0, &CLoongDB::FormatUpdateChangeRoleGuild);
}

VOID CLoongDB::FormatUpdateChangeRoleGuild(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("GuildID=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫdressMdID
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleDressMdID(DWORD dwRoleID, WORD wValue)
{
	return Update(dwRoleID, &wValue, 1, 0, &CLoongDB::FormatUpdateChangeRoleDressMdID);
}

VOID CLoongDB::FormatUpdateChangeRoleDressMdID(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("DressModelID=") << *(WORD*)pData;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���ý�ɫɾ����־λ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SetRoleRemoveFlag(DWORD dwRoleID, DWORD dwTime)
{
	return Update(dwRoleID, &dwTime, 1, 0, &CLoongDB::FormatUpdateSetRoleRemoveFlag);
}

BOOL CLoongDB::SetRoleClanRemoveFlag(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, &CLoongDB::FormatUpdateSetRoleClanRemoveFlag);
}

VOID CLoongDB::FormatUpdateSetRoleRemoveFlag(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTime)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("RemoveFlag=") << 1;
	pStream->FillString(",RemoveTime=") << *(DWORD*)pTime;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormatUpdateSetRoleClanRemoveFlag(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("clan_data");
	pStream->FillString("RemoveFlag=") << 1;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}
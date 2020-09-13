//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBLoad.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- ģ�淽��
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "LoongDB.h"


//-------------------------------------------------------------------------------------------------------
// ��ȡ���ݿ���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::Load(OUT LPVOID &pData, OUT INT32 *pNum, DWORD dwAccountID, 
					DWORD dwRoleID, Fun_pFormat3 pFunFormat, Fun_pProcRes pFunProcRes, BOOL b4Log)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = NULL;
	if(b4Log)
		pStream = m_pDBLog->GetStream();
	else
		pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// �����
	pStream->Clear();
	m_bLoadFormatError = FALSE;
	Beton::QueryResult* pResult = NULL;

	// ��ʽ����ѯ�Ľ�ɫ����
	(this->*pFunFormat)(pStream, dwAccountID, dwRoleID);
	if( m_bLoadFormatError )
		goto Exit;

	// ��ѯ���ݿ�
	if(b4Log)
		pResult = m_pDBLog->Query(pStream);
	else
		pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// ���鲢�����ѯ���
	if (pResult->GetRowCount() > 0)
	{
		(this->*pFunProcRes)(pData, pNum, pResult);
	}
	else
	{
		if (P_VALID(pNum))
		{
			*pNum = 0;
		}
		bRet = FALSE;
	}

Exit:
#ifdef _DEBUG
	IMSG(_T("%s\r\n"), m_pUtil->AnsiToUnicode(pStream->GetBuf()));
	IMSG(_T("Results Row Count: %d\n"), P_VALID(pResult) ? pResult->GetRowCount() : 0);
#endif
	
	++m_dwReadTimes;

	// �ͷ����ͽ��
	if(b4Log)
	{
		m_pDBLog->ReturnStream(pStream);
		m_pDBLog->FreeQueryResult(pResult);
	}
	else
	{
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->FreeQueryResult(pResult);
	}
	
	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ���룬ɾ�������µ����ݿ����ģ�淽��(�޽����,�Ҳ���Ҫ���ݿ�����)
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::Update(DWORD dwRoleID, LPVOID pData, INT32 nNum, INT32 nSzOne, 
					  Fun_pFormat3_1 pFunFormat, OUT LPVOID *ppOutPointer/* = NULL*/)
{
	if(nNum <= 0)
	{
		return TRUE;
	}
	
	BOOL bRet = TRUE;
	
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// �����
	pStream->Clear();

	CHAR *pCurData = (CHAR *)pData;
	for(INT32 i=0; i<nNum; ++i)
	{
		// ��ʽ�����ݿ�������,��ִ��
		m_bUpdateFormatError = FALSE;
		(this->*pFunFormat)(pStream, dwRoleID, pCurData);
		if( m_bUpdateFormatError )
			continue;

#ifdef _DEBUG
		IMSG(_T("%s\r\n"), m_pUtil->AnsiToUnicode(pStream->GetBuf()));
#endif

		if(!m_pDBLoong->Execute(pStream))
		{
			bRet = FALSE;
		}

		// ��������ָ��
		pCurData += nSzOne;

		++m_dwWriteTimes;
	}

	if(ppOutPointer != NULL)
	{
		*ppOutPointer = pCurData;
	}
	
	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ���룬ɾ�������µ����ݿ����ģ�淽��(�޽����,�Ҳ���Ҫ���ݿ�����)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::UpdateLog(LPCSTR szTablePostfix, LPVOID pData, Fun_pFormat3_2 pFunFormat)
{
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLog->GetStream();
	ASSERT(P_VALID(pStream));

	// �����
	pStream->Clear();

	// ��ʽ�����ݿ�������,��ִ��
	(this->*pFunFormat)(pStream, szTablePostfix, pData);

#ifdef _DEBUG
	IMSG(_T("%s\r\n"), m_pUtil->AnsiToUnicode(pStream->GetBuf()));
#endif

	m_pDBLog->AddQuery(pStream);

	//++m_dwWriteTimes;

	// �ͷ��� -- �����ڴ˴��ͷ�
}

////-------------------------------------------------------------------------------------------------------
//// ���룬ɾ�������µ����ݿ����ģ�淽��(�޽����,�Ҳ���Ҫ���ݿ�����)
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::UpdateLog(DWORD dwRoleID, LPVOID pData, INT32 nNum, INT32 nSzOne, Fun_pFormat3_1 pFunFormat)
//{
//	if(nNum <= 0)
//	{
//		return;
//	}
//
//	CHAR *pCurData = (CHAR *)pData;
//	for(INT32 i=0; i<nNum; ++i)
//	{
//		// ��ȡ��
//		Beton::MyStream* pStream = m_pDBLog->GetStream();
//		ASSERT(P_VALID(pStream));
//		
//		// ��ʽ�����ݿ�������,��ִ��
//		(this->*pFunFormat)(pStream, dwRoleID, pCurData);
//
//#ifdef _DEBUG
//		IMSG(_T("%s\r\n"), m_pUtil->Unicode8ToUnicode(pStream->GetBuf()));
//#endif
//
//		m_pDBLog->AddQuery(pStream);
//
//		// ��������ָ��
//		pCurData += nSzOne;
//
//		++m_dwWriteTimes;
//	}
//
//	// �ͷ��� -- �����ڴ˴��ͷ�
//}
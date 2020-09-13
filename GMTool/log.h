//-----------------------------------------------------------------------------
//!\file	log.h
//!\brief	��Log����
//!
//!\date	2010-2-5
//! last	2010-2-5
//!\author	zhangrong
//!
//-----------------------------------------------------------------------------
#include <stdio.h>
#pragma once

class CLog
{
	FILE*		m_pLogFile;
	TCHAR		m_tszFilePath[X_LONG_STRING];

public:
	CLog(TCHAR* szFileName)
	{
		m_pLogFile = NULL;

		// ��õ�ǰ��ִ���ļ����е�·��
		ZeroMemory(m_tszFilePath, X_LONG_STRING);
		GetModuleFileName(NULL, m_tszFilePath, X_LONG_STRING);
		int nCount = _tcslen(m_tszFilePath);
		while( m_tszFilePath[nCount-1] != '\\' )
		{
			nCount--;
		}
		m_tszFilePath[nCount] = '\0';

		// ��װ�ɴ�������·�����ļ���
		//_tcscat_s(m_tszFilePath, _T("gmtool_log.txt"));
		_tcscat_s(m_tszFilePath, szFileName);

		// �����ļ�
		//m_pLogFile = _tfopen(m_tszFilePath, _T("a+"));
		m_pLogFile = _tfopen(m_tszFilePath, _T("wt+"));
		if (!P_VALID(m_pLogFile))
		{
			_tprintf(_T("Log Writer Init Failed, No Log Message Will be Saved.\n"));
		}

		//fclose(m_pLogFile);
	}

	~CLog()
	{
		if (P_VALID(m_pLogFile))
		{
			fclose(m_pLogFile);
		}
	}

	VOID Write(LPCTSTR szContent)
	{
		m_pLogFile = _tfopen(m_tszFilePath, _T("at+"));
		
		if (P_VALID(m_pLogFile))
		{
			tagDWORDTime dwNow = GetCurrentDWORDTime();

			TCHAR szTmpStr[X_HUGE_STRING] = {'\0'};
			_sntprintf(szTmpStr, X_HUGE_STRING, _T("\n20%d-%d-%d-%d:%d:%d  "), dwNow.year, dwNow.month, dwNow.day, dwNow.hour, dwNow.min, dwNow.sec);
			_tcscat(szTmpStr, szContent);
			fputws(szTmpStr, m_pLogFile);
		}

		fclose(m_pLogFile);
	}

	VOID Note(LPCTSTR szContent)
	{
		m_pLogFile = _tfopen(m_tszFilePath, _T("at+"));

		if (P_VALID(m_pLogFile))
		{
// 			tagDWORDTime dwNow = GetCurrentDWORDTime();
// 
// 			TCHAR szTmpStr[X_HUGE_STRING] = {'\0'};
// 			_sntprintf(szTmpStr, X_HUGE_STRING, szContent);
			//_tcscat(szTmpStr, szContent);
			fputws(szContent, m_pLogFile);
		}

		fclose(m_pLogFile);
	}
};

extern CLog g_Log;

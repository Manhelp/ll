//-----------------------------------------------------------------------------
// File: log
// Desc: Game Tool log 1.0
// Auth: Lyp
// Date: 2003/12/18
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "log.h"

// ʹ����������
#include "..\system\disciomgr.h"
#include <time.h>
#include "..\system\thread.h"
#include "..\globe\globe_typedefine.h"

namespace vEngine {
#define MAX_LOG_SIZE 10240

#define MAX_LOG_FILE_SIZE 1024*1024*1024
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------

class CSGuard
{
	CRITICAL_SECTION	&	m_cs;
public:
	CSGuard(CRITICAL_SECTION &l)
		:m_cs(l)
	{
		EnterCriticalSection  ( &m_cs );
	}
	~CSGuard(void)
	{
		LeaveCriticalSection  ( &m_cs );
	}
};

static DWORD WINAPI LogThreadProc(
						                LPVOID lpParameter
						)
{
	Log * pThis = (Log*)lpParameter;
	if( P_VALID(pThis) )
	{
		return pThis->ThreadWriteLog(NULL);
	}
	return 0;
}



Log::Log()
{
	m_lTerminated = 0;
	m_dwFileHandle = INVALID_FILE_HANDLE;
	::InitializeCriticalSection  ( &m_cs );
	_stprintf(m_szLogThreadName,_T("LOG_thread_writelog%u"),GetTickCount());
	//if( FALSE == m_pThread->CreateThread(m_szLogThreadName,
	//	(vEngine::THREADPROC)m_Trunk.sfp1(&Log::ThreadWriteLog), this) )
	//	::InterlockedExchange(&m_lTerminated,1);
	DWORD dwThreadID = 0;
	hThead = ::CreateThread(NULL,0,LogThreadProc,this,0,&dwThreadID);
}

Log::~Log()
{
	::InterlockedExchange(&m_lTerminated,1);
	//m_pThread->WaitForThreadDestroy(m_szLogThreadName, INFINITE);
	::WaitForSingleObject( hThead, INFINITE );
	Close();
	::DeleteCriticalSection  ( &m_cs );
}


//-----------------------------------------------------------------------------
// ����log�ļ�
//-----------------------------------------------------------------------------
BOOL Log::Create(LPCTSTR szFileName)
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
		m_pDiscIOMgr->Close( m_dwFileHandle );

	// ���û���ṩ�ļ���,��ʹ�ô���Ĭ���ļ���
	if( NULL == szFileName )
	{
		// ���ɺ��ʵ�log�ļ���
		TCHAR szTime[MAX_PATH], szTemp[MAX_PATH];
		GetModuleFileName(NULL, szTemp, MAX_PATH);

		// ��ȥ·��
		TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
		pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
		_stprintf(m_szDefaultName, _T("log\\%s"), pResult);
		// ��ȥ�ļ���չ��
		pResult = _tcsrchr(m_szDefaultName, _T('.'));
		if( pResult )
			*pResult = '\0';	
		// ����ʱ��
		FILETIME CurrentTime;
		GetSystemTimeAsFileTime(&CurrentTime);
		szTime[0] = _T('\0');
		WORD wDate, wTime;
		if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
				FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
		{
			// �������Լ�ʱ����ͽ���id
			wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d %05d].log"),
					(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
					(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
					GetCurrentProcessId());
			_tcscat(m_szDefaultName, szTime);
		}
	
		szFileName = m_szDefaultName;
	}
	else
	{
		wsprintf(m_szDefaultName, szFileName);
	}


	m_dwFileHandle = m_pDiscIOMgr->Create(szFileName, GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	if( INVALID_FILE_HANDLE != m_dwFileHandle )
	{
		// add M$ BOM
		BYTE BOM[2] = {0xFF, 0xFE};
		DWORD dwWrite = 0;
		m_unFileSize = 0;
		m_pDiscIOMgr->Write(m_dwFileHandle, BOM, sizeof(BOM), &dwWrite, NULL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// �ر��ļ�
//-----------------------------------------------------------------------------
VOID Log::Close()
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
	{
		DumpAllRemained();
		m_pDiscIOMgr->Close( m_dwFileHandle );
		m_dwFileHandle = INVALID_FILE_HANDLE;
	}
}

//-----------------------------------------------------------------------------
// ����򵥵���Ϣ
//-----------------------------------------------------------------------------
BOOL Log::Write(LPCTSTR lpFormat, ...)
{
#if 1
	if( m_lTerminated )
		return FALSE;
	if( INVALID_FILE_HANDLE == m_dwFileHandle )
		return FALSE;

	TCHAR * szBuf = m_szBuf;

	ZeroMemory(szBuf, sizeof(m_szBuf));
	struct tm *_ptm;
	time_t _time;
	time(&_time);
	_ptm = gmtime(&_time);
	LPTSTR pStr = szBuf;
	INT slen = sizeof(m_szBuf)/sizeof(TCHAR);
	if(P_VALID(_ptm))
	{
		int l = 0;
		int day = _ptm->tm_mday, hour = _ptm->tm_hour+8;
		if( hour >= 24 )
		{
			hour -= 24;
			day += 1;
		}
		_stprintf(pStr,_T("[%d-%d-%d %d:%d:%d] "),_ptm->tm_year+1900,_ptm->tm_mon+1,day,hour,_ptm->tm_min,_ptm->tm_sec);
		l = _tcslen(pStr);
		pStr += l;
		slen -= l;
	}
	va_list argptr = NULL;
	va_start(argptr, lpFormat);
	_vstprintf_s(pStr/*szBuf*/, slen/*sizeof(szBuf)/sizeof(TCHAR)*/, lpFormat, argptr);
	UINT unSize = _tcslen(pStr)*sizeof(TCHAR);
	m_unFileSize += unSize;
	va_end(argptr);

	{
		CSGuard g(m_cs);
		m_vecLogs.push_back(szBuf);
	}

	//��ʱ����д��Ϊ�˲���Ժ�Ҫȥ��
	if( m_unFileSize >= MAX_LOG_FILE_SIZE && NULL != _tcsstr( m_szDefaultName,_T("LoginServer") ) )
	{		
		Close();
		Create();
	}
	return TRUE;
#else
	if( INVALID_FILE_HANDLE == m_dwFileHandle )
		return FALSE;

	TCHAR szBuf[MAX_LOG_SIZE];

	ZeroMemory(szBuf, sizeof(szBuf));
	struct tm *_ptm;
	time_t _time;
	time(&_time);
	_ptm = gmtime(&_time);
	LPTSTR pStr = szBuf;
	INT slen = sizeof(szBuf)/sizeof(TCHAR);
	if(P_VALID(_ptm))
	{
		int l = 0;
		int day = _ptm->tm_mday, hour = _ptm->tm_hour+8;
		if( hour >= 24 )
		{
			hour -= 24;
			day += 1;
		}
		_stprintf(pStr,_T("[%d-%d-%d %d:%d:%d] "),_ptm->tm_year+1900,_ptm->tm_mon+1,day,hour,_ptm->tm_min,_ptm->tm_sec);
		l = _tcslen(pStr);
		pStr += l;
		slen -= l;
	}
	va_list argptr = NULL;
	va_start(argptr, lpFormat);
	_vstprintf_s(pStr/*szBuf*/, slen/*sizeof(szBuf)/sizeof(TCHAR)*/, lpFormat, argptr);
	va_end(argptr);

	m_pDiscIOMgr->Seek( m_dwFileHandle, 0, GT_SEEK_END );
	return m_pDiscIOMgr->Write(m_dwFileHandle, szBuf, lstrlen(szBuf)*2);
#endif
}
// ������־�߳�
DWORD Log::ThreadWriteLog(LPVOID pParam)
{
	INT len = 0;
	INT nTryCount = 0;
	//Log * pThis = (Log*)pParam;
	while( /*P_VALID(pThis) && !pThis->*/!m_lTerminated)
	{
		// ����io����
		{
			CSGuard g(/*pThis->*/m_cs);
			len = /*pThis->*/m_vecLogs.size();
		}
		if( len >= 20 || (len > 0 && ++nTryCount > 100) ) // ����10�뱣��һ��
		{
			nTryCount = 0;
			tstringstream strbuf;
			{
				CSGuard g(/*pThis->*/m_cs);
				for( std::vector<tstring>::iterator i = /*pThis->*/m_vecLogs.begin(); i != /*pThis->*/m_vecLogs.end(); ++i )
				{
					strbuf << *i;
				}
				/*pThis->*/m_vecLogs.clear();
			}
			if( INVALID_FILE_HANDLE == m_dwFileHandle )
				return 0;
			if( P_VALID(/*pThis->*/m_pDiscIOMgr) && strbuf.str().length() > 0 )
			{
				/*pThis->*/m_pDiscIOMgr->Seek( /*pThis->*/m_dwFileHandle, 0, GT_SEEK_END );
				/*pThis->*/m_pDiscIOMgr->Write(/*pThis->*/m_dwFileHandle, strbuf.str().c_str(), strbuf.str().length()*sizeof(TCHAR));
			}
		}
		len = 0;
		Sleep(100);
	}
	return 0;
}

VOID	Log::DumpAllRemained(VOID)
{
	if( INVALID_FILE_HANDLE == m_dwFileHandle )
		return ;
	tstringstream strbuf;
	for( std::vector<tstring>::iterator i = m_vecLogs.begin(); i != m_vecLogs.end(); ++i )
	{
		strbuf << *i;
	}
	m_vecLogs.clear();
	if( strbuf.str().length() > 0 )
	{
		m_pDiscIOMgr->Seek( m_dwFileHandle, 0, GT_SEEK_END );
		m_pDiscIOMgr->Write(m_dwFileHandle, strbuf.str().c_str(), strbuf.str().length()*sizeof(TCHAR));
	}
}



//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
ExceptionLog::ExceptionLog()
{
	m_dwFileHandle = INVALID_FILE_HANDLE;
}

ExceptionLog::~ExceptionLog()
{
	Close();
}


//-----------------------------------------------------------------------------
// ����log�ļ�
//-----------------------------------------------------------------------------
BOOL ExceptionLog::Create()
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
		m_pDiscIOMgr->Close( m_dwFileHandle );

	// ���ɺ��ʵ�log�ļ���
	TCHAR szTime[MAX_PATH], szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);

	// ��ȥ·��
	TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
	pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
	_stprintf(m_szDefaultName, _T("log\\%s"), pResult);
	// ��ȥ�ļ���չ��
	pResult = _tcsrchr(m_szDefaultName, _T('.'));
	if( pResult )
		*pResult = '\0';	
	// ����ʱ��
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
			FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����ͽ���id
		wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d %05d]crash.log"),
				(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
				(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
				GetCurrentProcessId());
		_tcscat(m_szDefaultName, szTime);
	}
	
	m_dwFileHandle = m_pDiscIOMgr->Create(m_szDefaultName, GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	return ( INVALID_FILE_HANDLE != m_dwFileHandle );
}


//-----------------------------------------------------------------------------
// �ر��ļ�
//-----------------------------------------------------------------------------
VOID ExceptionLog::Close()
{
	if( m_dwFileHandle != INVALID_FILE_HANDLE )
	{
		m_pDiscIOMgr->Close( m_dwFileHandle );
		m_dwFileHandle = INVALID_FILE_HANDLE;
	}
}


//-----------------------------------------------------------------------------
// ����򵥵���Ϣ
//-----------------------------------------------------------------------------
BOOL ExceptionLog::Write(LPCSTR lpFormat, ...)
{
	if( INVALID_FILE_HANDLE == m_dwFileHandle )
		return FALSE;

	CHAR szBuf[MAX_LOG_SIZE];
	ZeroMemory(szBuf, sizeof(szBuf));
	va_list argptr = NULL;
	va_start(argptr, lpFormat);
	vsprintf_s(szBuf, MAX_LOG_SIZE, lpFormat, argptr);
	va_end(argptr);

	m_pDiscIOMgr->Seek( m_dwFileHandle, 0, GT_SEEK_END );
	return m_pDiscIOMgr->Write(m_dwFileHandle, szBuf, strlen(szBuf));
}
} // namespace vEngine {




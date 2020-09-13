//-----------------------------------------------------------------------------
// File: log
// Desc: Game Tool log 1.0
// Auth: Lyp
// Date: 2003/12/18
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

class Thread;
//-----------------------------------------------------------------------------
// log
//-----------------------------------------------------------------------------
class VENGINE_API Log
{
public:

	BOOL Create(LPCTSTR szFileName=NULL);
	BOOL Write(LPCTSTR lpFormat, ...);
	VOID Close();
	Log();~Log();
	DWORD ThreadWriteLog(LPVOID);
private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;	// ������VFSϵͳ
	DWORD					m_dwFileHandle;
	TCHAR					m_szDefaultName[1024];
	
	// Jason added 2010-10-23
	TSFPTrunk<Log>		m_Trunk;
	// ����io����
	std::vector<tstring>		m_vecLogs;
	CRITICAL_SECTION		m_cs;
	LONG					m_lTerminated;
	TCHAR					m_szBuf[10240];
	TObjRef<Thread>			m_pThread;

	VOID					DumpAllRemained(VOID);

	TCHAR					m_szLogThreadName[MAX_PATH];
	HANDLE	hThead;

	UINT					m_unFileSize;
};


//-----------------------------------------------------------------------------
// ר���ṩ���쳣ģ��ʹ�õ�log,�ⲿӦ���������logģ��
//-----------------------------------------------------------------------------
class ExceptionLog
{
public:

	BOOL Create();
	BOOL Write(LPCSTR lpFormat, ...);
	VOID Close();
	LPCTSTR	GetDefaultFileName() { return m_szDefaultName; }

	ExceptionLog();~ExceptionLog();

private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;	// ������VFSϵͳ
	DWORD					m_dwFileHandle;
	TCHAR					m_szDefaultName[1024];
};


} // namespace vEngine {

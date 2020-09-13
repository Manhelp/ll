#include "stdafx.h"
#include "section.h"

Section::Section(DWORD dwID, LPCTSTR szName)
{
	//ZeroMemory(this, sizeof(*this));
	m_pUtil			=	"Util";

	m_dwSectionID = dwID;
	_tcscpy_s((TCHAR*)m_szSectionName, X_SHORT_STRING, szName);
}

BOOL Section::Init()
{
	// ���·��
	TCHAR tszFilePath[X_LONG_STRING] = {0};
	GetModuleFileName(NULL, tszFilePath, X_LONG_STRING);
	int nCount = _tcslen(tszFilePath);
	while( tszFilePath[nCount-1] != '\\' )
	{
		nCount--;
	}
	tszFilePath[nCount] = '\0';

	// ���ɴ��������ļ���
	TCHAR szSectionCfg[MAX_PATH] = {_T('\0')};
	_stprintf(szSectionCfg, _T("%ssection_%s"), tszFilePath, m_szSectionName);
	
	TObjRef<Util> pUtil = "Util";
	
	TCHAR tszFile[MAX_PATH] = {0};
	if (!pUtil->GetIniPath(tszFile, szSectionCfg))
		return FALSE;

	// ��ȡ���������ļ�
	CreateObj("SectionContainer", "VarContainer");
	TObjRef<VarContainer> pVar = "SectionContainer";
	//TObjRef<VarContainer> pVar = "VarContainer";
	pVar->Load("VirtualFileSys", tszFile);

	// ��ʼ���ô�����loginDB
	LPCTSTR szDBHost = (TCHAR*)pVar->GetString(_T("dbhost"),	_T("section"));
	LPCTSTR szDBName = pVar->GetString(_T("dbname"),	_T("section"));
	LPCTSTR szDBUser = pVar->GetString(_T("user"),		_T("section"));
	LPCTSTR szDBPsd	= pVar->GetString(_T("psd"),		_T("section"));
	INT m_nDBPort = pVar->GetInt(_T("dbport"),		_T("section"));

	ZeroMemory(m_szDBHost, X_LONG_STRING);
	ZeroMemory(m_szDBName, X_LONG_STRING);
	ZeroMemory(m_szDBPsd, X_LONG_STRING);
	ZeroMemory(m_szDBUser, X_LONG_STRING);
	
	_tcscpy_s(m_szDBHost,	X_LONG_STRING, szDBHost);
	_tcscpy_s(m_szDBName,	X_LONG_STRING, szDBName);
	_tcscpy_s(m_szDBUser,	X_LONG_STRING, szDBUser);
	_tcscpy_s(m_szDBPsd,	X_LONG_STRING, szDBPsd);
	
	m_pLoginDB = new ClientDB;
	BOOL bRtv = m_pLoginDB->Init(m_szDBHost, m_szDBUser, m_szDBPsd, m_szDBName, m_nDBPort);
	if (!bRtv)
	{
		// to do ����
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to connect ro Database.\n");
		g_pDlg->Output(szLineTemp);

		SAFE_DEL(m_pLoginDB);
		return FALSE;
	}
	
	// �������е�world
	INT nWorldNum = pVar->GetInt(_T("world_num"), _T("section"));

	for(INT n = 1; n <= nWorldNum; ++n)
	{
		World* pWorld = new World;
		if( P_VALID(pWorld) && !pWorld->Init(m_dwSectionID, n) )
		{
			LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to connect ro Database.\n");
			g_pDlg->Output(szLineTemp);

			SAFE_DEL(pWorld);
			return FALSE;
		}

		// ���뵽�б���
		m_mapWorld.Add( pWorld->GetID(), pWorld );
	}
	
	KillObj("SectionContainer");
	return TRUE;
}

VOID Section::GetAccountDetail(LPCTSTR szAccountName, CHAR* szPsd, BYTE &byPrivilege, EPlayerLoginStatus &eLoginStatus, DWORD &dwWorldNameCrc, DWORD &dwFrobidMask, BOOL &bGuard, CHAR* szMibao)
{
	//InitLoginDB();

	// ��ȡһ��Stream
	MyStream* pStream = m_pLoginDB->m_DB.GetStream();
	if( !P_VALID(pStream) ) return;

	// ��ȡһ������
	Connection* pCon = m_pLoginDB->m_DB.GetFreeConnection();

	// ��װ��ѯ���
	pStream->SetSelect("account", "*");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// ���в�ѯ
	QueryResult* pResult = m_pLoginDB->m_DB.Query(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0) return;

	strncpy(szPsd, (*pResult)[2].GetString(), (*pResult)[2].GetLen()+1);
	byPrivilege =		(*pResult)[3].GetByte();
	eLoginStatus =		(EPlayerLoginStatus)(*pResult)[4].GetInt();
	dwWorldNameCrc =	(*pResult)[5].GetDword();
	dwFrobidMask =		(*pResult)[6].GetDword();
	bGuard =			(*pResult)[7].GetBool();
	if((*pResult)[8].GetLen() > 0 )
	{
		//strncpy(szMibao,(*pResult)[8].GetString(),(*pResult)[8].GetLen()+1);
		(*pResult)[8].GetBlob(szMibao, MIBAO_LEN);
	}

	//CloseLoginDB();
}

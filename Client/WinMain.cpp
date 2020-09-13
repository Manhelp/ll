//-----------------------------------------------------------------------------
//!\file winmain.cpp
//!\author Lyp
//!
//!\date 2008-04-20
//! last 2008-04-22
//!
//!\brief �ͻ�����ں���
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "ClientMain.h"
#include "Render2D.h"

#include "AudioSys.h"
VOID InitDump();

#ifdef ON_X_TRAP
DWORD WINAPI XTrapCallbackAliveThreadFunc(LPVOID lpParam);
#endif
#ifdef ON_GAMEGUARD
CNPGameLib g_npgl(_T("VanOnlineKR"));
#endif
//-----------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------
INT APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR lpCmdLine, INT)
{
	// XTrap
#ifdef ON_X_TRAP
	TCHAR szIniFileName[512] = _T("config\\config.ini");
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[512*2];

	// ��ȡEXE·��
	GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));
	TCHAR *find=::_tcsrchr(szTemp,_T('\\'));
	if(find!=NULL)
	{
		*find=0;
		find=::_tcsrchr(szTemp,_T('\\'));
		if(find!=NULL)
		{
			*(find+1)=0;
			_tcslwr_s(szTemp,sizeof(szTemp)/sizeof(TCHAR));
		}
	}

	tstring m_strAppPath = szTemp;

	tstring m_strIniFileFullName = m_strAppPath + szIniFileName;

	::GetPrivateProfileString(_T("Config"), _T("cfgname"), NULL,  szTemp, 512*2, m_strIniFileFullName.c_str());
	char buf[1024]={0};
	WideCharToMultiByte(CP_ACP, 0, szTemp, wcslen(szTemp), buf, 1024, NULL, NULL);
	XTrap_C_Start((LPCSTR)buf,NULL);
#endif

	//����GameGuard
#ifdef ON_GAMEGUARD
	DWORD dwResult = g_npgl.Init();
	if (dwResult != NPGAMEMON_SUCCESS)
	{
		// ÿ���������Ĵ�������ο����������
		TCHAR msg[256];
		LPCTSTR lpszMsg;
		// �����Ϣ����ʾ��ο���6 ��Ҫ������롱
		switch (dwResult)
		{
		case NPGAMEMON_ERROR_EXIST:
			lpszMsg = _T("GameGuard �������У�������������ִ����Ϸ��");
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			lpszMsg = _T("��Ϸ�ظ�ִ�У���GameGuard�Ѿ����У�������Ϸ�������ԡ�");
			break;
		case NPGAMEMON_ERROR_INIT:
			lpszMsg = _T("GameGuard��ʼ��������������ִ����Ϸ����ر��г�ͻ�ĳ���");
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:
		case NPGAMEMON_ERROR_AUTH_INI:
		case NPGAMEMON_ERROR_NFOUND_INI:
			lpszMsg = _T("GameGuard �ļ������ڻ����𻵣������°�װGameGuard �ļ���");
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			lpszMsg = _T("Windowsϵͳ�ļ��п��ܱ��𻵣������°�װInternet Explorer(IE)��");
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			lpszMsg = _T("GameGuardִ��ʧ�ܡ������°�װGameGuard�ļ���");
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			lpszMsg = _T("��⵽�Ƿ�������ر�����Ϸ�޹صĳ����ִ����Ϸ��");
			break;
		case NPGMUP_ERROR_ABORT:
			lpszMsg = _T("GameGuard���±��жϣ���������״������˷���ǽ���á�");
			break;
		case NPGMUP_ERROR_CONNECT:
			lpszMsg = _T("����GameGuard���·�����ʧ�ܡ������������û��������ӡ�");
			break;
// 		case HOOK_TIMEOUT:
// 			lpszMsg = "���������������Hookʧ�ܣ����������µ�ɱ�����ɨ�����ĵ��ԡ�";
// 			break;
		case NPGAMEMON_ERROR_GAMEGUARD:
			lpszMsg = _T("�ɵİ汾��GameGuard��ʼ�����������°�װGameGuard�ļ�����ִ����Ϸ��");
			break;
		case NPGMUP_ERROR_PARAM:
			lpszMsg = _T("ini �ļ������ڻ򱻸��ġ������°�װGameGuard�ļ���");
			break;
		case NPGMUP_ERROR_INIT:
			lpszMsg = _T("npgmup.des��ʼ��������ɾ��GameGuard�ļ��к�����ִ����Ϸ��");
			break;
		case NPGMUP_ERROR_DOWNCFG:
			lpszMsg = _T("����GameGuard���·�����ʧ�ܡ����Ժ����Ի�����˷���ǽ�����á�");
			break;
		case NPGMUP_ERROR_AUTH:
			lpszMsg = _T("GameGuard����δ��ɡ�����ʱ����ɱ�����������ִ����Ϸ�����������Ե����á�");
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			lpszMsg = _T("���ڴ�ռ䲻����Ѹ�Ⱦ�����������ͼ������ɨ��ģ�����ʧ�ܡ�");
			break;
// 		case NPGAMEMON_ERROR_COLLSION��
// 			IPszMsg="�������д������������GameGuard���ڳ�ͻ����ر���Щ�������ִ����Ϸ";
// 			break;
		default:
			// ��ʾ�����Ϣ.
			lpszMsg = _T(" GameGuard���з��������뷢��GameGuard�ļ����µ�gameservice@3zsafe.com");
			break;
		}
		wsprintf(msg, _T("GameGuard:%lu"), dwResult);
		MessageBox(NULL, _T("Error"), msg, MB_OK);
		return 0;
	}
#endif
	

	// Ϊ����Launcher����ʶ��ͻ����Ƿ������У�����һ��������
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("sm_client"));

	// ��ֹ��Ļ��������Դ����
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	vEngine::InitNetwork();
	vEngine::InitEngine();
	_set_se_translator( vEngine::SEH_Translation );
	InitDump();

	// ��vEngineע��ͻ����Լ�ʵ�ֵ�ģ��
	vEngine::Register(TWrap<Client>(),			"Client");
	vEngine::Register(TWrap<GUIRender>(),		"GUIRender");
	vEngine::Register(TWrap<GameFrameMgr>(),	"GameFrameMgr");
	vEngine::Register(TWrap<GameScriptMgr>(),	"GameScriptMgr");
	vEngine::Register(TWrap<NetSession>(),		"NetSession");

	vEngine::Register(TWrap<GameInputMap>(),	"GameInputMap");
	vEngine::Register(TWrap<MsgBox>(),			"MsgBox");
	vEngine::Register(TWrap<MsgBoxEx>(),		"MsgBoxEx");
	vEngine::Register(TWrap<MsgInputBox>(),		"MsgInputBox");
	vEngine::Register(TWrap<MsgMoneyInputBox>(),"MsgMoneyInputBox");
	vEngine::Register(TWrap<MsgOptionBox>(),    "MsgOptionBox");

	// ������Чϵͳ
	vEngine::Register(TWrap<AudioSys>(),		"AudioSys");

	//try
	//{
		TObjRef<Client> pClient;
		if( pClient->Init(hInst) )
		{
			#ifdef ON_X_TRAP
			DWORD dwThreadId;
			::CreateThread(NULL,0,XTrapCallbackAliveThreadFunc,NULL,0,&dwThreadId);
			#endif

			#ifdef ON_GAMEGUARD
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
			g_npgl.SetHwnd(hWnd);
			#endif
			pClient->Mainloop();
			pClient->Destroy();
		}
	//} 
	//catch(vEngine::Exception)
	//{
	//	throw;	// �������������
	//}

#ifdef ON_X_TRAP
	XTrap_C_Terminate();
#endif

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();

	::CloseHandle(hMutex);
	exit(0);
	return 0;
}


//-----------------------------------------------------------------------------
// �õ�Dump FTP����
//-----------------------------------------------------------------------------
VOID InitDump()
{
	TObjRef<VarContainer> pCfg = CreateObj("DumpConfig", "VarContainer");

	if( pCfg->Load(NULL, _T("config\\DumpCfg.xml")) )
	{
		const int MAX_STRING_SIZE = 512;
		TCHAR szVer[MAX_STRING_SIZE] = {0};
		::GetPrivateProfileString(_T("version"), _T("latest_game"), NULL,  szVer, MAX_STRING_SIZE, _T("launcher\\patchinfo.txt"));

		vEngine::InitDump(pCfg->GetString(_T("ftp")), 
			pCfg->GetString(_T("user")), pCfg->GetString(_T("password")), szVer);
	}

	KillObj("DumpConfig");
}

#ifdef ON_X_TRAP
DWORD WINAPI XTrapCallbackAliveThreadFunc(LPVOID lpParam)
{
	DWORD dwPeriod = 20000;
	while (TRUE)
	{
		Sleep(dwPeriod);
		XTrap_C_CallbackAlive(dwPeriod);
	}
	return 0;
}

#endif 


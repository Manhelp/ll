//--------------------------------------------------------------------------------
//!\file script_mgr.cpp
//!\author Aslan
//!
//!\date 2008-09-01
//! last 2008-09-01
//!
//!\brief Lua�ű�����������Ҫ������ֽű�����֯������
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#include "stdafx.h"

#include "script_mgr.h"
#include "lua_functions.h"
#include "world.h"
#include "unit.h"
#include "role.h"
#include "creature.h"
#include "creature_ai.h"
#include "map.h"
#include "activity_mgr.h"
#include "TimeChecker.h"
#include "../WorldDefine/msg_func_npc.h"

ScriptMgr g_ScriptMgr;

//------------------------------------------------------------------------------------
// ��ʼ��
//------------------------------------------------------------------------------------
BOOL ScriptMgr::Init()
{
	m_pUtil = "Util";

	RegisterScriptEventFunc();

	m_pMasterState = luaL_newstate();
	luaL_openlibs(m_pMasterState);

	m_mapThreadState.Clear();

	m_dwScriptMutexIDGen = 0;
	m_mapScriptMutex.Clear();

	m_mapQusetScript.Clear();
	m_mapCreatureScript.Clear();
	m_mapMapScript.Clear();
	m_mapItemScript.Clear();
	m_mapSkillScript.Clear();
	m_mapActScript.Clear();
	m_pRoleScript = NULL;
	m_pWorldScript = NULL;
	m_dwMaxPcallTime = 0;
    m_pMotionScript = NULL;
	m_pBuffScript = NULL;

	RegisterCoreFunctions();		// ע��C�ӿں���
	LoadScripts();					// ���ؽű�

	return TRUE;
}

//-------------------------------------------------------------------------------------
// ע���첽���ú���
//-------------------------------------------------------------------------------------
VOID ScriptMgr::RegisterScriptEventFunc()
{
	RegisterEventFunc(EVT_Script_Reload,		&ScriptMgr::OnReload);
}

//-------------------------------------------------------------------------------------
// �첽�¼������ű����¼���
//-------------------------------------------------------------------------------------
VOID ScriptMgr::OnReload(DWORD dwSenderID, LPVOID pEventMessage)
{
	Reload();
}

//-------------------------------------------------------------------------------------
// �ű�����������
//-------------------------------------------------------------------------------------
VOID ScriptMgr::Destroy()
{
	DestroyScripts();	// ɾ�����нű�
	DestroyAllStates();	// ɾ������State�������
}

//--------------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------------
VOID ScriptMgr::Update()
{
	CHECK_TIME();
	EventMgr<ScriptMgr>::Update();
}

//--------------------------------------------------------------------------------------------------
// ���¼��ؽű�
//--------------------------------------------------------------------------------------------------
VOID ScriptMgr::Reload()
{
	UnloadScripts();	// ж�����нű�
	DestroyAllStates();	// ɾ������State�������

	// ����������State
	m_pMasterState = luaL_newstate();
	luaL_openlibs(m_pMasterState);

	RegisterCoreFunctions();		// ע��C�ӿں���
	LoadScripts();					// ���ؽű�
}

//--------------------------------------------------------------------------------------------------
// ע�ṩLuaʹ�õĺ���
//--------------------------------------------------------------------------------------------------
VOID ScriptMgr::RegisterCoreFunctions()
{
	LuaOpenCommLibs(m_pMasterState);
}


//----------------------------------------------------------------------------------------------------
// Lua������ؽű�
//-----------------------------------------------------------------------------------------------------
VOID ScriptMgr::LoadScripts()
{
	vector<tstring> luaFiles;

	// �õ��ű���·��
	TObjRef<VarContainer> pVar = "VarContainer";
	tstring strPath = pVar->GetString(_T("path"), _T("script"));

	// ������Lua�ļ��洢����
	LoadScriptsFromDir(strPath.c_str(), luaFiles, TRUE);

	IMSG(_T("Loading %d Scripts...\r\n"), luaFiles.size());

	TCHAR szFileName[MAX_PATH];

	// ���μ��ظ���Lua�ļ���������ִ��
	for(vector<tstring>::iterator itr = luaFiles.begin(); itr != luaFiles.end(); ++itr)
	{
		_sntprintf(szFileName, MAX_PATH, _T("%s"), itr->c_str());
		//IMSG(_T("Loading Script %s...\r\n"), itr->c_str());

		// ���벢�����ļ�
		if( luaL_loadfile(m_pMasterState, m_pUtil->UnicodeToAnsi(szFileName)) != 0 )
		{
			IMSG(_T("Script %s, load failed!!!\r\n"), itr->c_str());
			const CHAR* szMsg = lua_tostring(m_pMasterState, -1);
			if( szMsg != NULL )
			{
				IMSG(_T("LuaEngine, reason: %s\r\n"), m_pUtil->AnsiToUnicode(szMsg));
			}

			lua_pop(m_pMasterState, 1);

			continue;
		}

		// ִ��Lua�ļ�
		if( lua_pcall(m_pMasterState, 0, LUA_MULTRET, 0) != 0 )
		{
			IMSG(_T("LuaEngine, could not run %s!!!!"), itr->c_str());
			const CHAR* szMsg = lua_tostring(m_pMasterState, -1);
			if( szMsg != NULL )
			{
				IMSG(_T("LuaEngine, reason: %s"), m_pUtil->AnsiToUnicode(szMsg));
			}

			lua_pop(m_pMasterState, 1);

			continue;
		}
	}

	// ��ն�ջ
	lua_settop(m_pMasterState, 0);

}

//-----------------------------------------------------------------------------------
// ��Ŀ¼����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::LoadScriptsFromDir(LPCTSTR szDir, std::vector<tstring>& luaFiles, BOOL bFirst)
{
	if( bFirst )
	{
		// ����ǵ�һ�ν���ú�������Ѱ��ȫ�ֽű�Ŀ¼
		TCHAR szGlobalDir[MAX_PATH];
		_tcscpy(szGlobalDir, szDir);
		_tcscat(szGlobalDir, _T("\\"));
		_tcscat(szGlobalDir, _T("gloable"));
		LoadScriptsFromDir(szGlobalDir, luaFiles);
	}

	HANDLE hFile;
	WIN32_FIND_DATA fd;
	memset(&fd, 0, sizeof(fd));

	TCHAR szSearchName[MAX_PATH];

	_tcscpy(szSearchName, szDir);
	_tcscat(szSearchName, _T("\\*.*"));

	hFile = FindFirstFile(szSearchName, &fd);
	FindNextFile(hFile, &fd);

	while( FindNextFile(hFile, &fd) )
	{
		// �����Ŀ¼����ݹ����
		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( !bFirst || 0 != _tcscmp(fd.cFileName, _T("gloable")) )
			{
				_tcscpy(szSearchName, szDir);
				_tcscat(szSearchName, _T("\\"));
				_tcscat(szSearchName, fd.cFileName);
				LoadScriptsFromDir(szSearchName, luaFiles);
			}
		}
		else
		{
			TCHAR* ext = _tcsrchr(fd.cFileName, _T('.'));
			
			if(P_VALID(ext) && !_tcsicmp(ext, _T(".lua")) )
			{
				tstring fname = szDir;
				fname += _T("\\");
				fname += fd.cFileName;

				luaFiles.push_back(fname);
			}
		}
	}
	FindClose(hFile);
}

//-----------------------------------------------------------------------------------
// ɾ�����нű�
//-----------------------------------------------------------------------------------
VOID ScriptMgr::DestroyScripts()
{
	// �������ű�
	QuestScript* pQuestScript = NULL;
	m_mapQusetScript.ResetIterator();
	while( m_mapQusetScript.PeekNext(pQuestScript) )
	{
		SAFE_DEL(pQuestScript);
	}
	m_mapQusetScript.Clear();

	// ��չ���ű�
	CreatureScript* pCreatureScript = NULL;
	m_mapCreatureScript.ResetIterator();
	while( m_mapCreatureScript.PeekNext(pCreatureScript) )
	{
		SAFE_DEL(pCreatureScript);
	}
	m_mapCreatureScript.Clear();

	// ��յ�ͼ�ű�
	MapScript* pMapScript = NULL;
	m_mapMapScript.ResetIterator();
	while( m_mapMapScript.PeekNext(pMapScript) )
	{
		SAFE_DEL(pMapScript);
	}
	m_mapMapScript.Clear();

	// ��ջ�ű�
	ActScript* pActScript = NULL;
	m_mapActScript.ResetIterator();
	while( m_mapActScript.PeekNext(pActScript) )
	{
		SAFE_DEL(pActScript);
	}
	m_mapActScript.Clear();

	// �����Ʒ�ű�
	ItemScript* pItemScript = NULL;
	m_mapItemScript.ResetIterator();
	while( m_mapItemScript.PeekNext(pItemScript) )
	{
		SAFE_DEL(pItemScript);
	}
	m_mapItemScript.Clear();

	// �����Ʒ�ű�
	SkillScript* pSkillScript = NULL;
	m_mapSkillScript.ResetIterator();
	while( m_mapSkillScript.PeekNext(pSkillScript) )
	{
		SAFE_DEL(pSkillScript);
	}
	m_mapSkillScript.Clear();

	// �����ҽű�
	if( P_VALID(m_pRoleScript) )
	{
		SAFE_DEL(m_pRoleScript);
	}

	// �������ű�
	if( P_VALID(m_pWorldScript) )
	{
		SAFE_DEL(m_pWorldScript);
	}

	//��ո��Զ����ű�
    if( P_VALID(m_pMotionScript) )
	{
        SAFE_DEL(m_pMotionScript);  
	}

	if (P_VALID(m_pBuffScript))
	{
		SAFE_DEL(m_pBuffScript);
	}
}

//-----------------------------------------------------------------------------------
// ж�����нű�
//-----------------------------------------------------------------------------------
VOID ScriptMgr::UnloadScripts()
{
	// ��������ű�
	QuestScript* pQuestScript = NULL;
	m_mapQusetScript.ResetIterator();
	while( m_mapQusetScript.PeekNext(pQuestScript) )
	{
		pQuestScript->Destroy();
	}

	// ���ù���ű�
	CreatureScript* pCreatureScript = NULL;
	m_mapCreatureScript.ResetIterator();
	while( m_mapCreatureScript.PeekNext(pCreatureScript) )
	{
		pCreatureScript->Destroy();
	}

	// ���õ�ͼ�ű�
	MapScript* pMapScript = NULL;
	m_mapMapScript.ResetIterator();
	while( m_mapMapScript.PeekNext(pMapScript) )
	{
		pMapScript->Destroy();
	}

	// ���û�ű�
	ActScript* pActScript = NULL;
	m_mapActScript.ResetIterator();
	while( m_mapActScript.PeekNext(pActScript) )
	{
		pActScript->Destroy();
	}

	// ������Ʒ�ű�
	ItemScript* pItemScript = NULL;
	m_mapItemScript.ResetIterator();
	while( m_mapItemScript.PeekNext(pItemScript) )
	{
		pItemScript->Destroy();
	}

	// ������Ʒ�ű�
	SkillScript* pSkillScript = NULL;
	m_mapSkillScript.ResetIterator();
	while( m_mapSkillScript.PeekNext(pSkillScript) )
	{
		pSkillScript->Destroy();
	}

	// ������ҽű�
	if( P_VALID(m_pRoleScript) )
	{
		m_pRoleScript->Destroy();
	}

	// �������ű�
	if( P_VALID(m_pWorldScript) )
	{
		m_pWorldScript->Destroy();
	}

	// ��ո��Զ����ű�
	if( P_VALID(m_pMotionScript) )
	{
		m_pMotionScript->Destroy();
	}

	if (P_VALID(m_pBuffScript))
	{
		m_pBuffScript->Destroy();
	}
}

//-----------------------------------------------------------------------------------
// ж�����е�State�������
//-----------------------------------------------------------------------------------
VOID ScriptMgr::DestroyAllStates()
{
	// ɾ�����е��߳�״̬
	std::list<DWORD> listThreadID;
	m_mapThreadState.ExportAllKey(listThreadID);

	for( std::list<DWORD>::iterator it = listThreadID.begin(); it != listThreadID.end(); ++it )
	{
		DWORD dwThreadID = *it;

		char szBuf[64] = {0};
		_ultoa_s(dwThreadID, szBuf, 10);

		lua_pushnil(m_pMasterState);
		lua_setfield(m_pMasterState, LUA_REGISTRYINDEX, szBuf);
	}
	m_mapThreadState.Clear();

	// ɾ����state
	lua_close(m_pMasterState);

	// ɾ�����нű���
	TMap<DWORD, Mutex*>::TMapIterator it = m_mapScriptMutex.Begin();
	Mutex* pMutex = NULL;

	while( m_mapScriptMutex.PeekNext(it, pMutex) )
	{
		SAFE_DEL(pMutex);
	}
	m_mapScriptMutex.Clear();
}

//-----------------------------------------------------------------------------------
// �õ�һ�����õ�thread
//-----------------------------------------------------------------------------------
lua_State* ScriptMgr::GetThreadState()
{
	//DWORD dwThreadID = GetCurrentThreadId();

	//lua_State* pThreadState = m_mapThreadState.Peek(dwThreadID);

	//if( !P_VALID(pThreadState) )
	//{
	//	pThreadState = CreateThreadState(dwThreadID);
	//	ASSERT( P_VALID(pThreadState) );
	//}

	//return pThreadState;

	return m_pMasterState;
}

//-----------------------------------------------------------------------------------
// ����һ���µ�thread
//-----------------------------------------------------------------------------------
lua_State* ScriptMgr::CreateThreadState(DWORD dwThreadID)
{
	IMSG(_T("create thread state, thread is %u\r\n"), dwThreadID);

	m_Lock.Acquire();

	lua_State* pThreadState = lua_newthread(m_pMasterState);

	// ���µ�stateд��ע�����ֹ�������ռ�
	char szBuf[64] = {0};
	_ultoa_s(dwThreadID, szBuf, 10);
	lua_setfield(m_pMasterState, LUA_REGISTRYINDEX, szBuf);

	// ���µ�state���뵽map��
	m_mapThreadState.Add(dwThreadID, pThreadState);

	m_Lock.Release();

	return pThreadState;
}

//-----------------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------------
VOID ScriptMgr::ErrMsg(lua_State* pState)
{
	IMSG(_T("Script Error: %s\r\n"), m_pUtil->AnsiToUnicode(lua_tostring(pState, -1)));
	lua_pop(pState, 1);
}

VOID ScriptMgr::MemMsg()
{
	IMSG(_T("<<<<Script Use Mem: %d bytes>>>>\r\n"), m_pMasterState->l_G->totalbytes);
}
#ifndef  _NO_SCRIPT_TRACE
//-----------------------------------------------------------------------------------
// �۲�����ű�
//-----------------------------------------------------------------------------------
class _ScriptTrace
{
	static BOOL s_bOn;
	CHAR* m_szFunction;
public:
	static void TurnOn(BOOL bOn = TRUE)
	{
		InterlockedExchange((LPLONG)&s_bOn, bOn);
	}

	_ScriptTrace(CHAR* szFunction)
	{		
		m_szFunction = szFunction;
		if(s_bOn)
		{
			ITRACE("Start to call script %s", szFunction);
		}		
	}

	~_ScriptTrace()
	{
		if(s_bOn)
		{
			ITRACE("End of call script %s", m_szFunction);
		}
	}
};

BOOL _ScriptTrace::s_bOn = FALSE;

DWORD WINAPI TurnOnScriptTrace(LPCTSTR szOn)
{
	_ScriptTrace::TurnOn((BOOL)_ttoi(szOn));
	return 0;
}

void RegisterTraceCmd()
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("scripttrace"), TurnOnScriptTrace, _T("turn on/off script trace"), 1);
}
#define TRACE_SCRIPT(a)		_ScriptTrace _trace(a);
#else
#define TRACE_SCRIPT(a)

void RegisterTraceCmd()
{
}
#endif 

//-----------------------------------------------------------------------------------
// ͨ�ýű����ú�����Ŀǰ�����ܷ����ַ�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::CallScriptFunction(CHAR* szFunction, CHAR* szFormat, ...)
{
	CHECK_TIME_WITH_INFO(szFunction);
	TRACE_SCRIPT(szFunction);

	if( !P_VALID(szFunction) ) return;

	lua_State* pThreadState = GetThreadState();
	if( !P_VALID(pThreadState) ) return;

	m_Lock.Acquire();

	DWORD dwStartTime = timeGetTime();

	lua_getglobal(pThreadState, szFunction);
	if( lua_isnil(pThreadState, -1) )
	{
		IMSG(_T("Script Error: unknown function %s\r\n"), m_pUtil->AnsiToUnicode(szFunction));
		lua_pop(pThreadState, 1);

		m_Lock.Release();
		return;
	}

	// from the pil book
	va_list vl;
	INT nArg = 0, nRes = 0;		// �����ͽ��������

	va_start(vl, szFormat);

	// ѹ�����
	BOOL bArgEnd = FALSE;
	for(nArg = 0; *szFormat && FALSE == bArgEnd; ++nArg)
	{
		luaL_checkstack(pThreadState, 1, "too many arguments");

		switch(*szFormat++)
		{
		case 'd':	// double����
			{
				double d = va_arg(vl, double);
				lua_pushnumber(pThreadState, d);
			}
			break;

		case 'f':	// float����
			{
				double f = va_arg(vl, double);
				lua_pushnumber(pThreadState, f);
			}
			break;

		case 'u':	// dword����
			{
				unsigned int u = va_arg(vl, unsigned int);
				lua_pushnumber(pThreadState, u);
			}
			break;

		case 'i':	// int����
			{
				int i = va_arg(vl, int);
				lua_pushinteger(pThreadState, i);
			}
			break;

		case 's':	// �ַ�������
			{
				char* c = (char*)va_arg(vl, unsigned int);
				lua_pushstring(pThreadState, c);
			}
			break;

		case '>':	// ��������
			bArgEnd = TRUE;
			goto _arg_end; 
			
		default:
			break;
		}
	}

_arg_end:

	// ���ú���
	nRes = strlen(szFormat);
	if( lua_pcall(pThreadState, nArg, nRes, 0) != 0 )
	{
		ErrMsg(pThreadState);

		m_Lock.Release();
		return;
	}

	INT nNum = lua_gettop(pThreadState);

	// �������
	INT nIndex = -nRes;

	while(*szFormat)
	{
		switch(*szFormat++)
		{
		case 'd':		// double���
			*va_arg(vl, double*) = lua_tonumber(pThreadState, nIndex);
			break;

		case 'f':		// float���
			*va_arg(vl, float*)  = lua_tonumber(pThreadState, nIndex);
			break;

		case 'u':		// dword���
			*va_arg(vl, unsigned int*)  = lua_tonumber(pThreadState, nIndex);
			break;

		case 'i':		// int���
			*va_arg(vl, int*)  = lua_tointeger(pThreadState, nIndex);
			break;

		case 'b':		// BOOL���
			*va_arg(vl, BOOL*)  = lua_toboolean(pThreadState, nIndex);
			break;

		default:
			break;
		}

		++nIndex;
	}

	lua_pop(pThreadState, nRes);

	// ����
	va_end(vl);

	DWORD dwEndTime = timeGetTime();

	if(m_dwMaxPcallTime < (dwEndTime - dwStartTime) )
	{
		m_dwMaxPcallTime = dwEndTime - dwStartTime;
		ILOG->Write(_T("Max calling time script function is <%s>, useing %d millisecond.\r\n"), m_pUtil->Unicode8ToUnicode(szFunction), m_dwMaxPcallTime);
	}

	m_Lock.Release();
}

//-----------------------------------------------------------------------------------
// ע�����AI�ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterCreatureEvent(DWORD dwID, EScriptCreatureEvent eEvent, const CHAR* szFunction)
{
	ASSERT( GT_VALID(dwID) && ( eEvent >= 0 && eEvent < ESCAE_End ) && P_VALID(szFunction) );

	// ���Ȳ��Ҹù����Ƿ��Ѿ�����Ӧ�Ľű��ˣ����û�о�����
	CreatureScript* pScript = m_mapCreatureScript.Peek(dwID);

	if( !P_VALID(pScript) )
	{
		pScript = new CreatureScript;
		m_mapCreatureScript.Add(dwID, pScript);
	}

	// ע��
	pScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע����ҽű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterRoleEvent(EScriptRoleEvent eEvent, const CHAR* szFunction)
{
	ASSERT( (eEvent >= 0 && eEvent < ESRE_End) && P_VALID(szFunction) );

	// ���Ȳ��Ҹ�����Ƿ��Ѿ�����Ӧ�Ľű��ˣ����û�о�����
	if( !P_VALID(m_pRoleScript) )
	{
		m_pRoleScript = new RoleScript;
	}

	// ע��
	m_pRoleScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע���ͼ�ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterMapEvent(const CHAR* szMapName, EScriptMapEvent eEvent, const CHAR* szFunction)
{
	ASSERT( P_VALID(szMapName) && (eEvent >= 0 && eEvent < ESME_End) && P_VALID(szFunction) );

	DWORD dwMapID = IUTIL->Crc32(IUTIL->Unicode8ToUnicode(szMapName));

	// ���û�е�ͼ�ű�������
	MapScript* pScript = m_mapMapScript.Peek(dwMapID);
	if( !P_VALID(pScript) )
	{
		pScript = new MapScript;
		m_mapMapScript.Add(dwMapID, pScript);
	}

	// ע��
	pScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע������ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterQuestEvent(UINT16 u16ID, EScriptQuestEvent eEvent, const CHAR* szFunction)
{
	ASSERT( GT_VALID(u16ID) && ( eEvent >= 0 && eEvent < ESQE_End ) && P_VALID(szFunction) );

	// ���Ȳ��Ҹ������Ƿ��Ѿ�����Ӧ�Ľű��ˣ����û�о�����
	QuestScript* pScript = m_mapQusetScript.Peek(u16ID);
	if( !P_VALID(pScript) )
	{
		pScript = new QuestScript;
		m_mapQusetScript.Add(u16ID, pScript);
	}

	// ע��
	pScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע��̶���ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterActEvent(DWORD dwActID, EScriptActEvent eEvent, const CHAR* szFunction)
{
	ASSERT( GT_VALID(dwActID) && ( eEvent >= 0 && eEvent < ESAE_End ) && P_VALID(szFunction) );

	// ���Ȳ��Ҹû�Ƿ��Ѿ�����Ӧ�Ľű��ˣ����û�о�����
	ActScript* pScript = m_mapActScript.Peek(dwActID);
	if( !P_VALID(pScript) )
	{
		pScript = new ActScript;
		m_mapActScript.Add(dwActID, pScript);
	}

	// ע��
	pScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע����Ϸ����ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterWorldEvent(EScriptWorldEvent eEvent, const CHAR* szFunction)
{
	ASSERT(( eEvent >= 0 && eEvent < ESWE_End ) && P_VALID(szFunction) );

	if( !P_VALID(m_pWorldScript) )
	{
		m_pWorldScript = new WorldScript;
	}

	m_pWorldScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע����Ϸ����ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterItemEvent(DWORD dwTypeID, EScriptItemEvent eEvent, const CHAR* szFunction)
{
	ASSERT( GT_VALID(dwTypeID) && ( eEvent >= 0 && eEvent < ESAE_End ) && P_VALID(szFunction) );

	ItemScript* pScript = m_mapItemScript.Peek(dwTypeID);
	if( !P_VALID(pScript) )
	{
		pScript = new ItemScript;
		m_mapItemScript.Add(dwTypeID, pScript);
	}

	pScript->RegisterFunc(eEvent, szFunction);
}

//-----------------------------------------------------------------------------------
// ע����Ϸ����ű�����
//-----------------------------------------------------------------------------------
VOID ScriptMgr::RegisterSkillEvent(DWORD dwTypeID, EScriptSkillEvent eEvent, const CHAR* szFunction)
{
	ASSERT( GT_VALID(dwTypeID) && ( eEvent >= 0 && eEvent < ESAE_End ) && P_VALID(szFunction) );

	SkillScript* pScript = m_mapSkillScript.Peek(dwTypeID);
	if( !P_VALID(pScript) )
	{
		pScript = new SkillScript;
		m_mapSkillScript.Add(dwTypeID, pScript);
	}

	pScript->RegisterFunc(eEvent, szFunction);
}

//-------------------------------------------------------------------------------------
// ע����Զ����ű�����
//-------------------------------------------------------------------------------------
VOID ScriptMgr::RegisterMotionEvent(EScriptMotionEvent eEvent, const CHAR* szFunction)
{
     ASSERT( ( eEvent >= 0 && eEvent < ESMOTION_End ) && P_VALID(szFunction) );
	 if( !P_VALID(m_pMotionScript) )
	 {
          m_pMotionScript = new MotionScript; 
	 }

	 m_pMotionScript->RegisterFunc(eEvent, szFunction);
}

//--------------------------------------------------------------------------------------
// ���Զ�����������¼�����
//--------------------------------------------------------------------------------------
void MotionScript::OnMotion(DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwDestRoleID,DWORD dwMotionID)const
{
	if( !P_VALID(m_szFunc[ESMOTION_Motion]) ) 
	{
		return ;
	}
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESMOTION_Motion], "uuuuu", dwMapID,dwInstanceID, dwRoleID, dwDestRoleID, dwMotionID);
}

//-------------------------------------------------------------------------------------
// ע��buff�ű�����
//-------------------------------------------------------------------------------------
VOID ScriptMgr::RegisterBuffEvent(EScriptBuffEvent eEvent, const CHAR* szFunction)
{
	ASSERT( ( eEvent >= 0 && eEvent < ESBUFF_End ) && P_VALID(szFunction) );
	if( !P_VALID(m_pBuffScript) )
	{
		m_pBuffScript = new BuffScript; 
	}

	m_pBuffScript->RegisterFunc(eEvent, szFunction);
}

//--------------------------------------------------------------------------------
// buff�ű�
//--------------------------------------------------------------------------------
void BuffScript::OnDestroy(DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwBuffID)const
{
	if( !P_VALID(m_szFunc[ESBUFF_Destroy]) ) 
	{
		return ;
	}
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESBUFF_Destroy], "uuuu", dwMapID,dwInstanceID, dwRoleID, dwBuffID);
}

//---------------------------------------------------------------------------------------
// ����ű����������¼�����
//---------------------------------------------------------------------------------------
INT  QuestScript::CheckAccept(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const
{
	if( !P_VALID(m_szFunc[ESQE_On_CheckAccept]) ) return TRUE;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) ) return FALSE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwNPCID = P_VALID(pNPC) ? pNPC->GetID() : GT_INVALID;

	INT nRet = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_CheckAccept], "uuiuu>i", dwMapID, dwInstanceID, u16QuestID, pRole->GetID(), dwNPCID, &nRet);

	return nRet;
}

INT  QuestScript::CheckComplete(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const
{
	if( !P_VALID(m_szFunc[ESQE_On_CheckComplete]) ) return TRUE;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) ) return FALSE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwNPCID = P_VALID(pNPC) ? pNPC->GetID() : GT_INVALID;

	INT nRet = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_CheckComplete], "uuiuu>i", dwMapID, dwInstanceID, u16QuestID, pRole->GetID(), dwNPCID, &nRet);

	return nRet;
}

VOID QuestScript::OnInit(UINT16 u16QuestID, Role* pOwner) const
{
	if( !P_VALID(m_szFunc[ESQE_On_Init]) ) return;

	Map* pMap = pOwner->GetMap();
	if(!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Init], "uuiu", dwMapID,dwInstanceID, u16QuestID, pOwner->GetID());
}

VOID QuestScript::OnAccept(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const
{
	if( !P_VALID(m_szFunc[ESQE_On_Accept]) ) return;

	Map* pMap = pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwNPCID = P_VALID(pNPC) ? pNPC->GetID() : GT_INVALID;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Accept], "uuiuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwNPCID);
}

VOID QuestScript::OnComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const
{
	if( !P_VALID(m_szFunc[ESQE_On_Complete]) ) return;

	Map* pMap = pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwNPCID = P_VALID(pNPC) ? pNPC->GetID() : GT_INVALID;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Complete], "uuiuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwNPCID);
}

VOID QuestScript::OnBoardQuestComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const
{
	if( !P_VALID(m_szFunc[ESQE_On_BQComplete]) ) return;

	Map* pMap = pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwNPCID = P_VALID(pNPC) ? pNPC->GetID() : GT_INVALID;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_BQComplete], "uuiuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwNPCID);
}

VOID QuestScript::OnCancel(UINT16 u16QuestID, Role* pOwner) const
{
	if( !P_VALID(m_szFunc[ESQE_On_Cancel]) ) return;

	Map* pMap = pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Cancel], "uuiu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID());
}

VOID QuestScript::OnCreatureKill(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const
{
	if( !P_VALID(m_szFunc[ESQE_On_Creature_Kill]) ) return;

	Map* pMap = pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Creature_Kill], "uuiuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwCreatureTypeID);
}

VOID QuestScript::OnNPCTalk(UINT16 u16QuestID, Role* pOwner, DWORD dwNPCID, DWORD dwNPCTypeID) const
{
	if( !P_VALID(m_szFunc[ESQE_On_NPC_Talk]) ) return;

	Map* pMap = pOwner->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_NPC_Talk], "uuiuuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwNPCID, dwNPCTypeID);
}

VOID QuestScript::OnDlgDefault(UINT16 u16QuestID, Role* pOwner, DWORD dwDlgOption) const
{
	if( !P_VALID(m_szFunc[ESQE_On_Dlg_Default]) ) return;

	Map* pMap = pOwner->GetMap();
	if(!P_VALID(pMap)) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Dlg_Default], "uuiuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwDlgOption);
}

VOID QuestScript::OnInvest(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const
{
	if(!P_VALID(m_szFunc[ESQE_On_Invest])) return;

	Map* pMap = pOwner->GetMap();
	if(!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_On_Invest], "uuiuu", dwMapID, dwInstanceID, u16QuestID, pOwner->GetID(), dwCreatureTypeID);
}
VOID QuestScript::OnClientCallback(INT nMsgID,Role* pPlayer,DWORD param1,DWORD param2,DWORD param3,DWORD param4) const
{
	if (!P_VALID(m_szFunc[ESQE_OnClientCallback])) return;

	if( !P_VALID(pPlayer) )
		return;

	Map * pMap = pPlayer->GetMap();
	if(!P_VALID(pMap))
		return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESQE_OnClientCallback], "uuuuuuuu",pMap->GetMapID(),
		pMap->GetInstanceID(),pPlayer->GetID(),nMsgID,param1,param2,param3,param4);
}


//-----------------------------------------------------------------------------------------
// ����ű����������¼�����
//-----------------------------------------------------------------------------------------
VOID CreatureScript::OnLoad(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Load]) ) return;

	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Load], "uuu", dwMapID, dwInstanceID, pCreature->GetID());
}

VOID CreatureScript::OnTalk(Creature* pCreature, Role* pRole, INT nIndex) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Talk]) ) return;

	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Talk], "uuuuui", dwMapID, dwInstanceID, pCreature->GetID(), pCreature->GetTypeID(), pRole->GetID(), nIndex);
}

VOID CreatureScript::OnRespawn(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Respawn]) ) return;

	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Respawn], "uuu", dwMapID, dwInstanceID, pCreature->GetID());
}

VOID CreatureScript::OnEnterCombat(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Enter_Combat]) ) return;

	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Enter_Combat], "uuu", dwMapID, dwInstanceID, pCreature->GetID());
}

VOID CreatureScript::OnLeaveCombat(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Leave_Combat]) ) return;

	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Leave_Combat], "uuu", dwMapID, dwInstanceID, pCreature->GetID());
}


VOID CreatureScript::OnDie(Creature* pCreature, Unit* pKiller, BOOL& bRemove) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Die]) ) return;

	Map* pMap = pCreature->GetMap();
	if( !P_VALID(pMap) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwKillerID = P_VALID(pKiller) ? pKiller->GetID() : GT_INVALID;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Die], "uuuuu>b", dwMapID, dwInstanceID, pCreature->GetID(), pCreature->GetTypeID(), dwKillerID, &bRemove);
}

VOID CreatureScript::OnInvest(Creature* pCreature, Role* pScr) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Invest])) return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Invest], "uuuuu", dwMapID, dwInstanceID, pCreature->GetID(), pCreature->GetTypeID(), pScr->GetID());
}

VOID CreatureScript::OnUpdateAI(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_UpdateAI]) ) return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	AIController* pAI = pCreature->GetAI();
	if( !P_VALID(pAI) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_UpdateAI], "uuuu", dwMapID, dwInstanceID, pCreature->GetID(), pAI->GetCurrentStateType());
}

VOID CreatureScript::OnUpdateCurAI(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_UpdateCurAI]) ) return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	AIController* pAI = pCreature->GetAI();
	if( !P_VALID(pAI) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_UpdateCurAI], "uuuu", dwMapID, dwInstanceID, pCreature->GetID(), pAI->GetCurrentStateType());
}

VOID CreatureScript::OnEnterCurAI(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_EnterCurAI]) ) return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	AIController* pAI = pCreature->GetAI();
	if( !P_VALID(pAI) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_EnterCurAI], "uuuu", dwMapID, dwInstanceID, pCreature->GetID(), pAI->GetCurrentStateType());
}

VOID CreatureScript::OnLeaveCurAI(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_LeaveCurAI]) ) return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	AIController* pAI = pCreature->GetAI();
	if( !P_VALID(pAI) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_LeaveCurAI], "uuuu", dwMapID, dwInstanceID, pCreature->GetID(), pAI->GetCurrentStateType());
}

BOOL CreatureScript::OnEventCurAI(Creature* pCreature) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_EventCurAI]) ) return FALSE;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return FALSE;

	AIController* pAI = pCreature->GetAI();
	if( !P_VALID(pAI) ) return FALSE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	BOOL bRet = FALSE;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_EventCurAI], "uuuu>i", dwMapID, dwInstanceID, pCreature->GetID(), pAI->GetCurrentStateType(), &bRet);

	return bRet;
}

// Jason 2010-1-20 v1.3.1 npc�̵��Ƿ�Ӫҵ�ж�
BOOL CreatureScript::IsShopOpened(Creature* pCreature,Role * player,Map* pMap) const
{
	// Ϊ�˲�Ӱ�죬û����������¼��ĺ���
	if( !P_VALID(m_szFunc[ESCAE_On_IsShopOpened]) ) return TRUE;

	if(!P_VALID(pMap))	return FALSE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	BOOL bRet = FALSE;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_IsShopOpened], "uuuuu>i", dwMapID, dwInstanceID, pCreature->GetID(), pCreature->GetTypeID  (),player->GetID(), &bRet);

	return bRet;
}
BOOL CreatureScript::CanShopping(Creature* pCreature,Role * player,Map* pMap,DWORD dwShopID,DWORD dwItemTypeID ) const
{
	// Ϊ�˲�Ӱ�죬û����������¼��ĺ���
	if( !P_VALID(m_szFunc[ESCAE_On_CanShopping]) ) return TRUE;

	if(!P_VALID(pMap))	return FALSE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	BOOL bRet = FALSE;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_CanShopping], "uuuuu>i", dwMapID, dwInstanceID, dwShopID, player->GetID(),dwItemTypeID, &bRet);

	return bRet;
}

VOID CreatureScript::OnShopping(Creature* pCreature,Role * player,Map* pMap,DWORD dwShopID,DWORD dwItemTypeID ) const
{

	
	if( !P_VALID(m_szFunc[ESCAE_On_OnShopping]) ) return ;

	if(!P_VALID(pMap))	return ;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
    g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_OnShopping], "uuuuu", dwMapID, dwInstanceID, dwShopID, player->GetID(),dwItemTypeID);

}
VOID CreatureScript::OnDisappear( Creature* pCreature ) const
{
	if( !P_VALID(m_szFunc[ESCAE_On_Disappear]) ) return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	AIController* pAI = pCreature->GetAI();
	if( !P_VALID(pAI) ) return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_Disappear], "uuuu", dwMapID, dwInstanceID, pCreature->GetID(), pAI->GetCurrentStateType());
}

VOID CreatureScript::OnBeAttacked(Creature* pCreature, Unit* pSrc) const
{
	if (!P_VALID(m_szFunc[ESCAE_On_BeAttacked]))	return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	if (!P_VALID(pSrc))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESCAE_On_BeAttacked], "uuuu", dwMapID, dwInstanceID, pCreature->GetID(), pSrc->GetID());
}

VOID CreatureScript::OnBePulled(Creature * pCreature,const Vector3 & Pos,BOOL result) const
{
	if (!P_VALID(m_szFunc[ESCAE_On_BePulled]))	return;

	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(	m_szFunc[ESCAE_On_BePulled], "uuuuuuu", dwMapID, dwInstanceID, 
									pCreature->GetID(), Pos.x / TILE_SCALE,Pos.y / TILE_SCALE,Pos.z / TILE_SCALE,result);
}

//-------------------------------------------------------------------------------------------
// ��ҽű�
//-------------------------------------------------------------------------------------------
VOID RoleScript::OnRoleOnline(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_On_Online]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_Online], "u", pRole->GetID());
}

VOID RoleScript::OnRoleFirstOnline(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_On_FirstOnline]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_FirstOnline], "u", pRole->GetID());
}

VOID RoleScript::OnRoleIntoWorld(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_On_IntoWorld])) return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_IntoWorld], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

VOID RoleScript::OnRoleFirstIntoWorld(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_On_FirstIntoWorld])) return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_FirstIntoWorld], "uuuu", dwMapID, dwInstanceID, pRole->GetID(),pRole->GetAvatar()->wDressMdlID);
}

VOID RoleScript::OnRoleEnterMap(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_On_EnterMap])) return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_EnterMap], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

VOID RoleScript::OnRoleLevelChange(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_On_LevelChange])) return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_LevelChange], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

BOOL RoleScript::IsDeadPenalty(Role* pRole, EDeadPenaltyMode eMode) const
{
	if( !P_VALID(m_szFunc[ESRE_IsEquipLootPenalty + eMode]) ) return TRUE;
	BOOL  bRet = TRUE;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap))		return TRUE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_IsEquipLootPenalty + eMode], "uuu>b", dwMapID, dwInstanceID, pRole->GetID(), &bRet);

	return bRet;
}

VOID RoleScript::OnOpenChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_OpenChest]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_OpenChest], "uuuuu", dwMapID, dwInstanceID, pRole->GetID(), dwChestID, dwKeyID);
}

VOID RoleScript::OnStopChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_StopChest]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_StopChest], "uuuuu", dwMapID, dwInstanceID, pRole->GetID(), dwChestID, dwKeyID);
}

VOID RoleScript::OnAgainChest(Role *pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_On_AgainChest]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_AgainChest], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

VOID RoleScript::OnGetItem(Role* pRole, DWORD dwChestID, DWORD dwItemID, INT nNum) const
{
	if (!P_VALID(m_szFunc[ESRE_On_GetItem]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_GetItem], "uuuuui", dwMapID, dwInstanceID, pRole->GetID(), dwChestID, dwItemID, nNum);
}

VOID RoleScript::OnStartChest(Role* pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_On_StartChest]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_StartChest], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

VOID RoleScript::OnGetMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_GetMarriage]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_GetMarriage], "uuuu", dwMapID, dwInstanceID, dwApplicantRoleID, dwTargetRoleID);
}
DWORD RoleScript::OnGetMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_GetMarriageCheck]))	return E_Success;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwErrCode = E_Success;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_GetMarriageCheck], "uuuu>u", dwMapID, dwInstanceID, dwApplicantRoleID, dwTargetRoleID,&dwErrCode);

	return dwErrCode;
}
VOID RoleScript::OnBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_BreakMarriage]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_BreakMarriage], "uuuu", dwMapID, dwInstanceID, dwApplicantRoleID, dwTargetRoleID);
}
DWORD RoleScript::OnBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_BreakMarriageCheck]))	return E_SystemError;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwErrCode = E_Success;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_BreakMarriageCheck], "uuuu>u", dwMapID, dwInstanceID, dwApplicantRoleID, dwTargetRoleID,&dwErrCode);

	return dwErrCode;
}
// ESRE_On_ForceBreakMarriage		=	42,			// ǿ�����
VOID RoleScript::OnForceBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_ForceBreakMarriage]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_ForceBreakMarriage], "uuuu", dwMapID, dwInstanceID, dwApplicantRoleID, dwTargetRoleID);
}

// ESRE_On_ForceBreakMarriageCheck	=	43,			// ǿ�������
DWORD RoleScript::OnForceBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const
{
	if (!P_VALID(m_szFunc[ESRE_On_ForceBreakMarriageCheck]))	return E_SystemError;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwErrCode = E_Success;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_ForceBreakMarriageCheck], "uuuu>u", dwMapID, dwInstanceID, dwApplicantRoleID, dwTargetRoleID,&dwErrCode);

	return dwErrCode;
}

// ESRE_OnChaseHolderDie			=	44,		// �ᱦ����Я�������߱�ɱ��ʱ����
VOID RoleScript::OnChaseHolderDie(Role * pRole, DWORD dwKillerID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnChaseHolderDie]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnChaseHolderDie], "uu", pRole->GetID(), dwKillerID);
}

// ESRE_ForceLootChase			=	45,		// ����ǿ�Ƶ���(�ᱦ���)
VOID RoleScript::ForceLootChase(Role * pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_ForceLootChase]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_ForceLootChase], "u", pRole->GetID());
}

// ESRE_OnMakeFriend	= 46,	// ���Ӻ���
VOID RoleScript::OnMakeFriend(Role * pRole, DWORD dwFriendID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnMakeFriend]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnMakeFriend], "uuuu", pRole->GetMapID(), pRole->GetOwnInstanceID(), pRole->GetID(), dwFriendID);
}

//ESRE_OnLearnSkill				=	47,		// ���ѧ���¼���
VOID RoleScript::OnLearnSkill(Role * pRole, DWORD dwSkillID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnLearnSkill]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnLearnSkill], "uuuu", pRole->GetMapID(), pRole->GetOwnInstanceID(), pRole->GetID(), dwSkillID);
}

// ESRE_OnEquipConsolidateLevelUp		=	48,		// װ��ǿ���ȼ�����
VOID RoleScript::OnEquipConsolidateLevelUp(Role * pRole, DWORD dwTypeID, BYTE byBeforeLvl, BYTE byAfterLvl) const
{
	if (!P_VALID(m_szFunc[ESRE_OnEquipConsolidateLevelUp]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnEquipConsolidateLevelUp], "uuuuuu", pRole->GetMapID(), pRole->GetOwnInstanceID(), pRole->GetID(), dwTypeID, byBeforeLvl, byAfterLvl);
}

// ESRE_OnProduceEquip		=	49,		// ����װ��
VOID RoleScript::OnProduceEquip(Role * pRole, DWORD dwTypeID, BYTE byQuality) const
{
	if (!P_VALID(m_szFunc[ESRE_OnProduceEquip]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnProduceEquip], "uuuuu", pRole->GetMapID(), pRole->GetOwnInstanceID(), pRole->GetID(), dwTypeID, byQuality);
}

// OnDigHoleOnEquip		=	50,		// ��ҿ�����Ƕ��(bSuccess:1Ϊ�ɹ���0Ϊʧ��)
VOID RoleScript::OnDigHoleOnEquip(Role * pRole, DWORD dwTypeID, BOOL bSuccess, BYTE byHoleNum) const
{
	if (!P_VALID(m_szFunc[ESRE_OnDigHoleOnEquip]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnDigHoleOnEquip], "uuuuuu", pRole->GetMapID(), pRole->GetOwnInstanceID(), pRole->GetID(), dwTypeID, (DWORD)bSuccess, byHoleNum);
}

// ESRE_OnCutStone				=	51,		// ����иʯԭʯ
DWORD RoleScript::OnCutStone(Role * pRole, DWORD dwStoneTypeID, DWORD dwDesItemID, DWORD dwNewItemTypeID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnCutStone]))	return 0;

	if (!P_VALID(pRole))	return 0;

	DWORD dwID = 0;
	//DWORD dwInstanceID = pRole->GetOwnInstanceID(); 

	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
		return 0;
	//{
	//	if( EMT_ScriptCreate == pMap->GetMapType())
	//	{
	//		dwInstanceID = pMap->get
	//	}
	//}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnCutStone], "uuuuuu>u", pRole->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), dwStoneTypeID, dwDesItemID, dwNewItemTypeID, &dwID);
	return dwID;
}

// ESRE_OnDressEquip			=	52,		// ��Ҵ���װ��
VOID RoleScript::OnDressEquip(Role * pRole, bool bEquipOrNot, DWORD dwTypeID, BYTE byEquipPos, DWORD dwSerialIDHigh, DWORD dwSerialIDLow) const
{
	if (!P_VALID(m_szFunc[ESRE_OnDressEquip]))	return;

	if (!P_VALID(pRole))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnDressEquip], "uuuuiuuu", dwMapID, dwInstanceID, pRole->GetID(), dwTypeID, bEquipOrNot, (DWORD)byEquipPos, dwSerialIDHigh, dwSerialIDLow);
}

// ESRE_OnBrandEquip			=	53,		// ��װ��ʹ��ʥӡ
VOID RoleScript::OnBrandEquip(Role * pRole, DWORD dwTypeID,  DWORD dwBrandItemTypeID, BOOL bSuccess, BYTE byBrandLevel) const
{
	if (!P_VALID(m_szFunc[ESRE_OnBrandEquip]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnBrandEquip], "uuuuuuu", pRole->GetMapID(), pRole->GetOwnInstanceID(), pRole->GetID(), dwTypeID, dwBrandItemTypeID, (DWORD)bSuccess, byBrandLevel);
}

// ESRE_OnAddItemToBag			=	54,		// �����л����Ʒ
VOID RoleScript::OnAddItemToBag	(Role * pRole, DWORD dwTypeID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnAddItemToBag]))	return;

	if (!P_VALID(pRole))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnAddItemToBag], "uu", pRole->GetID(), dwTypeID);
}

INT RoleScript::OnGetRoleFishingExp(Role* pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_GetRoleFishingExp]))	return E_SystemError;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	INT exp = 0;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_GetRoleFishingExp], "uuu>i", dwMapID, dwInstanceID, pRole->GetID(), &exp);

	return exp;
}

DWORD RoleScript::CanCancelPenalty(Role* pRole, Unit* pKiller, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommerce) const
{
	if (!P_VALID(m_szFunc[ESRE_CanCancelPenalty]))	return E_SystemError;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CanCancelPenalty], "uuuu>bbbbb", dwMapID, dwInstanceID, pRole->GetID(), pKiller->GetID(), &bEquip, &bExp, &bInjury, &bHostility, &bCommerce);
	return E_Success;
}

VOID RoleScript::OnCompleteCommerce(Role* pRole,DWORD dwTaelProgress) const
{
	if (!P_VALID(m_szFunc[ESRE_On_CompleteCommerce]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_CompleteCommerce], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), dwTaelProgress);
}


DWORD RoleScript::OnWaitSummon(Role* pRole, DWORD dwCreatureID, BOOL& bRet) const
{
	if (!P_VALID(m_szFunc[ESRE_WaitSummon]))	return E_SystemError;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_WaitSummon], "uuuu>b", dwMapID, dwInstanceID, pRole->GetID(), dwCreatureID, &bRet);

	return E_Success;
}

DWORD RoleScript::OnCancelSummon(Role* pRole, DWORD dwCreatureID) const
{
	if (!P_VALID(m_szFunc[ESRE_CancelSummon]))	return E_SystemError;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CancelSummon], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), dwCreatureID);

	return E_Success;
}

VOID RoleScript::OnUseJingWuPoint(Role* pRole, INT nPoint) const
{
	if (!P_VALID(m_szFunc[ESRE_UseJingWuPoint]))	return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_UseJingWuPoint], "uuui", dwMapID, dwInstanceID, pRole->GetID(), nPoint);
}

DWORD RoleScript::CanAddEnemy(Role * pRole,DWORD dwKillerID) const
{
	if (!P_VALID(m_szFunc[ESRE_CanAddEnemy]))	return GT_INVALID;
	
	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return GT_INVALID;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();
	DWORD dwCanAddEnemy = 0;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CanAddEnemy], "uuuu>u", dwMapID, dwInstanceID, pRole->GetID(), dwKillerID, &dwCanAddEnemy);

	return dwCanAddEnemy;
}

// ��֮����ϵͳ--�������
INT RoleScript::IsTrainFinished(Role* pRole, DWORD dwTrainID) const
{
	INT nRet = 0;
	if (!P_VALID(m_szFunc[ESRE_IsTrainFinished]))	return nRet;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return nRet;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_IsTrainFinished], "uuuu>i", dwMapID, dwInstanceID, pRole->GetID(), dwTrainID, &nRet);

	return nRet;
}

// ��һ��һ�����ߵ��¼��ӿ�
VOID	RoleScript::OnObtainItem(Role* pRole, DWORD dwItemID, EItemCreateMode eCreateMode) const
{
	if (!P_VALID(m_szFunc[ESME_OnObtainItem]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnObtainItem], "uuu", pRole->GetID(), dwItemID, static_cast<DWORD>(eCreateMode));
}

// �����Ϸ�����ӵ��¼��ӿ�
VOID	RoleScript::OnSilverChange(Role* pRole, DWORD dwSilverIncrease) const
{
	if (!P_VALID(m_szFunc[ESME_OnSilverIncrease]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnSilverIncrease], "uu", pRole->GetID(), dwSilverIncrease);
}

// ��һ��һ���ƺŵ��¼��ӿ�
VOID	RoleScript::OnObtainTitle(Role* pRole, DWORD dwTitleID) const
{
	if (!P_VALID(m_szFunc[ESME_OnObtainTitle]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnObtainTitle], "uu", pRole->GetID(), dwTitleID);
}

// ������һ��װ��ǿ�����¼��ӿ�
VOID	RoleScript::OnItemConsolidate(Role* pRole, DWORD dwEquiptID, EAllConsolidateType eConsolidateType,
						  INT nConsolidateLevel, BOOL bSuccess) const
{
	if (!P_VALID(m_szFunc[ESME_OnItemConsolidate]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnItemConsolidate], "uuuub", static_cast<DWORD>(eConsolidateType),pRole->GetID(), dwEquiptID, static_cast<DWORD>(nConsolidateLevel), bSuccess);
}

// ��Ҽ�������ɹ����¼��ӿ�z
VOID	RoleScript::OnActiveLonghunSuccess(Role* pRole, DWORD dwEquiptID, DWORD dwLongHunSkillID) const
{
	if (!P_VALID(m_szFunc[ESME_OnActiveLonghunSuccess]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnActiveLonghunSuccess], "uuu", pRole->GetID(), dwEquiptID, dwLongHunSkillID);
}

// ������һ������
VOID	RoleScript::OnTrainFinished(Role* pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_OnTrainFinished]))	return;

}

VOID RoleScript::OnHandleMaster(Role* pMaster, Role* pApprentice) const
{
	if (!P_VALID(m_szFunc[ESRE_HandleMaster]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_HandleMaster], "uu", pMaster->GetID(), pApprentice->GetID());
}

/*
//void OnFabaoAttChange( DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwFabaoTypeID,INT nWhat,DWORD dwVal );
//����what ȡֵ���£�
//enum ERoleFabaoAttEvent
//{
	//RFAE_LevelUp		=	0,	// ��������
	//RFAE_HoleAttReset	=	1,	// ���������������
	//RFAE_WuxingXSReset	=	2,	// ����������������
//};
//��what = 1����2ʱ��dwValΪ��������typeid��
*/
VOID RoleScript::OnFabaoAttChange(Role* pRole,CONST tagFabao * pFabao,INT nWhat,DWORD dwOther) const
{
	if (!P_VALID(m_szFunc[ESRE_OnFabaoAttChange]))	return ;

	if( !P_VALID(pFabao) )
		return ;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return ;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnFabaoAttChange], "uuuuuu", dwMapID, dwInstanceID, pRole->GetID(),pFabao->pProtoType->dwTypeID,nWhat, dwOther);
}


//---------------------------------------------------------------------------------------------
// ��ɫ���뾺�������� 
//---------------------------------------------------------------------------------------------
VOID   RoleScript::OnRoleJoinToArenaRoom(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,BOOL& bCanJoin)const
{
	if (!P_VALID(m_szFunc[ESRE_OnRoleJoinToArenaRoom]))	return;
    
	if (!P_VALID(pRole))
	{
		return ;
	}
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnRoleJoinToArenaRoom], "uuuuu>b", pMap->GetMapID(), pMap->GetInstanceID(),dwRoomType,dwRoomId,pRole->GetID(),&bCanJoin);  
}

//---------------------------------------------------------------------------------------------
// ��ɫ��������������
//---------------------------------------------------------------------------------------------
VOID   RoleScript::OnRoleCreateArenaRoom(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,BOOL& bCanCreate)const
{
	if (!P_VALID(m_szFunc[ESRE_OnRoleCreateArenaRoom]))	return;

	if (!P_VALID(pRole))
	{
		return ;
	}
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnRoleCreateArenaRoom], "uuuuu>b", pMap->GetMapID(), pMap->GetInstanceID(),dwRoomType,dwRoomId,pRole->GetID(),&bCanCreate);  

}

//---------------------------------------------------------------------------------------------
// ��ɫͬ�ⱻ������뾺����
//---------------------------------------------------------------------------------------------
VOID     RoleScript::OnRoleAgreeToBeInvited(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,DWORD dwDestRoleID,BOOL& bCanBeInvited)const
{
	if (!P_VALID(m_szFunc[ESRE_OnRoleAgreeToBeInvited]))	return;

	if (!P_VALID(pRole))
	{
		return ;
	}
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnRoleAgreeToBeInvited], "uuuuuu>b", pMap->GetMapID(), pMap->GetInstanceID(),dwRoomType,dwRoomId,dwDestRoleID,pRole->GetID(),&bCanBeInvited);  
}

VOID	RoleScript::OnReadyToSoaring(Role * pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_OnReadyToSoaring]))	return;

	if (!P_VALID(pRole))
	{
		return ;
	}
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnReadyToSoaring], "uuu", pMap->GetMapID(), pMap->GetInstanceID(),pRole->GetID());  
}
VOID	RoleScript::OnCompletePracticeEquipment(Role* pRole,DWORD dwItemTypeID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnCompletePracticeEquipment]))	return;

	if (!P_VALID(pRole))
	{
		return ;
	}
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnCompletePracticeEquipment], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(),pRole->GetID(),dwItemTypeID);  
}
VOID	RoleScript::OnCompleteCastingEquipment(Role* pRole,DWORD dwItemTypeID,INT nCastingLevel) const
{
	if (!P_VALID(m_szFunc[ESRE_OnCompleteCastingEquipment]))	return;

	if (!P_VALID(pRole))
	{
		return ;
	}
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnCompleteCastingEquipment], "uuuuu", pMap->GetMapID(), pMap->GetInstanceID(),pRole->GetID(),dwItemTypeID,nCastingLevel);  
}

VOID	RoleScript::OnGodStrengthenSuccess(Role * pRole,tagEquip * pEquip) const
{
	if (!P_VALID(m_szFunc[ESRE_OnGodStrengthenSuccess]))	return;

	if (!P_VALID(pRole))
	{
		return ;
	}
	if( !P_VALID(pEquip) || !P_VALID(pEquip->pEquipProto) )
		return ;
	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnGodStrengthenSuccess], "uuuuu", pMap->GetMapID(), pMap->GetInstanceID(),pRole->GetID(),
		pEquip->pEquipProto->dwTypeID,pEquip->equipSpec.byGodStrengthenLevel);  
}

DWORD RoleScript::OnUseGoldStone(Role * pRole,	tagItem* pItem) const
{
	if (!P_VALID(m_szFunc[ESRE_OnUseGoldStone]))
		return GT_INVALID;

	if (!P_VALID(pRole))
		return GT_INVALID;

	if (!P_VALID(pItem))
		return GT_INVALID;

	if (!P_VALID(pItem->pProtoType))
		return GT_INVALID;

	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
		return GT_INVALID;

	DWORD dwTypeID = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnUseGoldStone], "uuuu>u", pItem->dwTypeID, pRole->GetID(), pMap->GetMapID(), pMap->GetInstanceID(), &dwTypeID);  

	return dwTypeID;
}
VOID	RoleScript::OnCreateGuild(Role * pRole,DWORD dwGuildID) const
{
	if (!P_VALID(m_szFunc[ESRE_OnCreateGuild]))
		return ;

	if (!P_VALID(pRole))
		return ;

	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
		return ;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnCreateGuild], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), dwGuildID);  
}
DWORD	RoleScript::OnKillMonster(Role * pRole, Creature * pCreature) const
{
	if (!P_VALID(m_szFunc[ESRE_OnKillMonster]))
		return GT_INVALID;

	if (!P_VALID(pRole))
		return GT_INVALID;
	if( !P_VALID(pCreature) )
		return GT_INVALID;

	Map* pMap = pRole->GetMap();
	if ( !P_VALID(pMap) )
		return GT_INVALID;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_OnKillMonster], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), pCreature->GetID());  
	return E_Success;
}



//---------------------------------------------------------------------------------------------
// ��ͼ�ű�
//---------------------------------------------------------------------------------------------
VOID MapScript::OnInit(Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnInit]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnInit], "uu", pMap->GetMapID(), pMap->GetInstanceID());
}

VOID MapScript::OnTimer(Map* pMap, INT nMilliSec) const
{
	if( !P_VALID(m_szFunc[ESME_OnTimer]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnTimer], "uui", pMap->GetMapID(), pMap->GetInstanceID(), nMilliSec);
}

VOID MapScript::CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const
{
	if( !P_VALID(m_szFunc[ESME_CanEnterWhenOnline]) ) return;

	// ͳһ�ű�����Ϊ��������
	vOut.x = vOut.x / TILE_SCALE;
	vOut.z = vOut.z / TILE_SCALE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanEnterWhenOnline], "uu>ufff", dwOutMapID,  pRole->GetID(), 
								&dwOutMapID, &vOut.x, &vOut.y, &vOut.z);
	// ͳһ�ű�����Ϊ��������
	vOut.x = vOut.x * TILE_SCALE;
	vOut.z = vOut.z * TILE_SCALE;
}

VOID MapScript::GetOnePerfectMap(Role* pRole, DWORD &dwInstanceID) const
{
	if( !P_VALID(m_szFunc[ESME_GetOnePerfectMap]) ) return;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap))   return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_GetOnePerfectMap], "uuu>u", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), &dwInstanceID);
}

INT MapScript::GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const
{
	if( !P_VALID(m_szFunc[ESME_GetExportMapAndCoord]) ) return 0;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap))   return 0;

	INT nRet = 0;

	// ͳһ�ű�����Ϊ��������
	vOut.x = vOut.x / TILE_SCALE;
	vOut.z = vOut.z / TILE_SCALE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_GetExportMapAndCoord], "uuu>iufff", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), 
								&nRet, &dwOutMapID, &vOut.x, &vOut.y, &vOut.z);

	// ͳһ�ű�����Ϊ��������
	vOut.x = vOut.x * TILE_SCALE;
	vOut.z = vOut.z * TILE_SCALE;

	return nRet;
}

INT MapScript::CanEnter(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESME_CanEnter]) ) return 0;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap))  return 0;

	INT nRet = 0;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanEnter], "uuu>i", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), &nRet);

	return nRet;
}

VOID MapScript::OnPlayerEnter(Role* pRole, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnPlayerEnter]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnPlayerEnter], "uuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID());
}

VOID MapScript::OnPlayerLeave(Role* pRole, Map* pMap, BOOL bLogout) const
{
	if (bLogout)
	{
		if( !P_VALID(m_szFunc[ESME_OnPlayerLeaveW]) ) return;

		g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnPlayerLeaveW], "uuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID());

		return;
	}

	if( !P_VALID(m_szFunc[ESME_OnPlayerLeave]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnPlayerLeave], "uuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID());
}

VOID MapScript::OnCreatureDie(Creature* pCreature, Unit* pKiller, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnCreatureDie]) ) return;
	if( !P_VALID(pCreature) ) return;

	DWORD dwKillerID = GT_INVALID;
	if( P_VALID(pKiller) ) dwKillerID = pKiller->GetID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnCreatureDie], "uuuuu", pMap->GetMapID(), pMap->GetInstanceID(), pCreature->GetID(), pCreature->GetTypeID(), dwKillerID);
}

VOID MapScript::OnCreatureDisappear(Creature* pCreature, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnCreatureDisappear]) ) return;
	if( !P_VALID(pCreature) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnCreatureDisappear], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pCreature->GetID(), pCreature->GetTypeID());
}

VOID MapScript::OnRoleDie(Role* pRole, Unit* pKiller, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnRoleDie]) ) return;
	if( !P_VALID(pRole) ) return;

	DWORD dwKillerID = GT_INVALID;
	if( P_VALID(pKiller) ) dwKillerID = pKiller->GetID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnRoleDie], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), dwKillerID);
}

BOOL MapScript::CanSetSafeguard(Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_Safeguard]) ) return  TRUE;  
	BOOL bRet = TRUE;
                                                                                                                                  
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_Safeguard],"uu>b",pMap->GetMapID(), pMap->GetInstanceID(), &bRet);

	return bRet;
}

BOOL MapScript::CanUseItem(Map* pMap, DWORD dwTypeID) const
{
	if( !P_VALID(m_szFunc[ESME_CanUseItem]) ) return  TRUE;  
	BOOL bRet = TRUE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanUseItem],"uuu>b",pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, &bRet);

	return bRet;
}

BOOL MapScript::OnCityBroadCast(DWORD dwBroadType,DWORD dwPara1,DWORD dwPara2,DWORD dwPara3,DWORD dwPara4,DWORD dwPara5) const
{
	if( !P_VALID(m_szFunc[ESME_CityBroadCast]) ) return  TRUE;  
	BOOL bRet = TRUE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CityBroadCast],"uuuuuu", dwBroadType, dwPara1,dwPara2,dwPara3,dwPara4,dwPara5);

	return bRet;
}

BOOL MapScript::OnCheckRoleLevel(DWORD dwMapID, DWORD dwInstanceID, DWORD dwRoleID) const 
{
	BOOL bRet;
	if(!P_VALID(m_szFunc[ESME_ExperienceInstance])) return FALSE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_ExperienceInstance], "uuu>b", dwMapID, dwInstanceID, dwRoleID, &bRet);

	return bRet;
}

// ������-����һ���µľ�������ͼ
BOOL MapScript::OnCreatePvPMap3v3(DWORD dwMapID, DWORD dwInstanceID, INT16 nRoomID, DWORD* pdwPlayerRoleID, INT16 nPlayerCnt) const
{
	if ( !P_VALID(m_szFunc[ESME_OnCreatePvPMap]) || !P_VALID(pdwPlayerRoleID)) return TRUE;
	BOOL bRet = TRUE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnCreatePvPMap], "uuuuuuuuu", dwMapID, dwInstanceID, nRoomID, pdwPlayerRoleID[0], pdwPlayerRoleID[1], pdwPlayerRoleID[2], pdwPlayerRoleID[3], pdwPlayerRoleID[4], pdwPlayerRoleID[5]);
	return bRet;
}

BOOL MapScript::CanUseSkill(Map* pMap, DWORD dwTypeID) const
{
	if( !P_VALID(m_szFunc[ESME_CanUseSkill]) ) return  TRUE;
	BOOL bRet = TRUE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanUseSkill], "uuu>b", pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, &bRet);

	return bRet;
}

VOID MapScript::Revive(Role* pRole, ERoleReviveType eType, INT &nReviveHP, INT &nReviveMP, FLOAT &fx, FLOAT &fy, FLOAT &fz, DWORD &dwRebornMapID) const 
{
	if( !P_VALID(m_szFunc[ESME_On_Revive]) )  return;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap))   return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	// ͳһ�ű�����Ϊ��������
	fx = fx / TILE_SCALE;
	fz = fz / TILE_SCALE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_On_Revive], "uuuiiifffu>iifffu", dwMapID, dwInstanceID, pRole->GetID(), eType, 
						nReviveHP, nReviveMP, fx, fy, fz, dwRebornMapID, &nReviveHP, &nReviveMP, &fx, &fy, &fz, &dwRebornMapID);


	fx = fx * TILE_SCALE;
	fz = fz * TILE_SCALE;
}

VOID MapScript::OnEnterTrigger(Role* pRole, tagMapTriggerInfo* pTrigger, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnEnterTrigger]) ) return;
	if( !P_VALID(pRole) || !P_VALID(pTrigger) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnEnterTrigger], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), pTrigger->dwObjID);
}

VOID MapScript::OnEnterArea(Role* pRole, tagMapAreaInfo* pArea, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnEnterArea]) ) return;
	if( !P_VALID(pRole) || !P_VALID(pArea) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnEnterArea], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), pArea->dwObjID);
}


VOID MapScript::OnLeaveArea(Role* pRole, DWORD dwCurScriptArea, Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_OnLeaveArea]) ) return;
	if( !P_VALID(pRole) ) return; 

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_OnLeaveArea], "uuuu", pMap->GetMapID(), pMap->GetInstanceID(), pRole->GetID(), dwCurScriptArea );
}


INT  MapScript::CanInviteJoinTeam(Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_CanInviteJoinTeam]) ) return 0;
	INT	 nRet = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanInviteJoinTeam], "uu>i", pMap->GetMapID(), pMap->GetInstanceID(), &nRet);

	return nRet;
}

INT MapScript::CanLeaveTeam(Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_CanLeaveTeam]) ) return 0;
	INT  nRet = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanLeaveTeam], "uu>i", pMap->GetMapID(), pMap->GetInstanceID(), &nRet);

	return nRet;
}

INT MapScript::CanChangeLeader(Map* pMap) const
{
	if( !P_VALID(m_szFunc[ESME_CanChangeLeader]) ) return 0;
	INT  nRet = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanChangeLeader], "uu>i", pMap->GetMapID(), pMap->GetInstanceID(), &nRet);

	return nRet;
}

INT MapScript::CanKickMember(Map* pMap) const
{
	if ( !P_VALID(m_szFunc[ESME_CanKickMember]) ) return 0;
	INT nRet = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_CanKickMember], "uu>i", pMap->GetMapID(), pMap->GetInstanceID(), &nRet);

	return nRet;
}

DWORD MapScript::FriendEnemy(Map* pMap, Unit* pSrc, Unit* pTarget, BOOL& bIgnore) const
{
	if( !P_VALID(m_szFunc[ESME_FriendEnemy]) ) return 0;

	BOOL bFriend		=	FALSE;
	BOOL bHostile		=	FALSE;
	BOOL bIndependent	=	FALSE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESME_FriendEnemy], "uuuu>bbbb", pMap->GetMapID(), pMap->GetInstanceID(), pSrc->GetID(), pTarget->GetID(),
									&bFriend, &bHostile, &bIndependent, &bIgnore);

	DWORD dwFlag = 0;
	if( bFriend )		dwFlag	|=	ETFE_Friendly;
	if( bHostile )		dwFlag	|=	ETFE_Hostile;
	if( bIndependent )	dwFlag	|=	ETFE_Independent;

	return dwFlag;
}

//---------------------------------------------------------------------------------------------
// ��ű�
//---------------------------------------------------------------------------------------------
VOID ActScript::OnInit(DWORD dwActID) const
{
	if(!P_VALID(m_szFunc[ESAE_OnInit]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnInit], "u", dwActID);
}

VOID ActScript::OnTimer(DWORD dwActID, INT nSec) const
{
	if( !P_VALID(m_szFunc[ESAE_OnTimer]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnTimer], "ui", dwActID, nSec);
}

VOID ActScript::OnTimerMin(DWORD dwActID) const
{
	if( !P_VALID(m_szFunc[ESAE_OnTimerMin]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnTimerMin], "u", dwActID);
}

VOID ActScript::OnActStart(DWORD dwActID) const
{
	if( !P_VALID(m_szFunc[ESAE_OnStart]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnStart], "u", dwActID);
}

VOID ActScript::OnActEnd(DWORD dwActID) const
{
	if( !P_VALID(m_szFunc[ESAE_OnEnd]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnEnd], "u", dwActID);
}

VOID ActScript::OnDefaultRequest(DWORD dwActID, Role* pRole, DWORD	dwEventType) const
{
	if( !P_VALID(m_szFunc[ESAE_OnDefaultRequest]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnDefaultRequest], "uuu", dwActID, pRole->GetID(), dwEventType);
}

VOID ActScript::BroadActivityState(EActBroadMode eState) const
{
	if( !P_VALID(m_szFunc[ESAE_Broad]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_Broad], "i", eState);
}

VOID ActScript::FinishEudemonSummon() const
{
	if (!P_VALID(m_szFunc[ESAE_FinishEudemonSummon])) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_FinishEudemonSummon], "");
}
VOID ActScript::OnClientCallback(INT nMsgID,Role* pPlayer,DWORD param1,DWORD param2,DWORD param3,DWORD param4) const
{
	if (!P_VALID(m_szFunc[ESAE_OnClientCallback])) return;

	if( !P_VALID(pPlayer) )
		return;

	Map * pMap = pPlayer->GetMap();
	if(!P_VALID(pMap))
		return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnClientCallback], "uuuuuuuu",pMap->GetMapID(),
		pMap->GetInstanceID(),pPlayer->GetID(),nMsgID,param1,param2,param3,param4);
}
VOID ActScript::OnActOpenClose(DWORD dwActID, DWORD dwIsOpen) const
{
	if( !P_VALID(m_szFunc[ESAE_OnActOpenClose]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESAE_OnActOpenClose], "uu", dwActID, dwIsOpen);
}

//---------------------------------------------------------------------------------------------
// ��Ϸ�����¼��ű�
//---------------------------------------------------------------------------------------------
VOID WorldScript::OnAdventure(Role *pRole) const
{
	if( !P_VALID(m_szFunc[ESWE_Adventure]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_Adventure], "u", pRole->GetID());
}

// ������-����һ���µľ�������ͼ
BOOL WorldScript::OnCreatePvPMap3v3(DWORD dwMapID, DWORD dwInstanceID, INT16 nRoomID, DWORD* pdwPlayerRoleID, INT16 nPlayerCnt) const
{
	if ( !P_VALID(m_szFunc[ESWE_OnCreatePvPMap3v3]) || !P_VALID(pdwPlayerRoleID)) return TRUE;
	BOOL bRet = TRUE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_OnCreatePvPMap3v3], "uuuuuuuuu", dwMapID, dwInstanceID, nRoomID, pdwPlayerRoleID[0], pdwPlayerRoleID[1], pdwPlayerRoleID[2], pdwPlayerRoleID[3], pdwPlayerRoleID[4], pdwPlayerRoleID[5]);
	return bRet;
}

BOOL WorldScript::OnLoot(DWORD dwLootSrcID, DWORD dwLootTargetID, ELootType eLootType, DWORD dwTypeID, DWORD dwSetID) const
{
	if ( !P_VALID(m_szFunc[ESWE_OnLoot])) return TRUE;
	BOOL bRet = TRUE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_OnLoot], "uuuuu>i", dwLootSrcID, dwLootTargetID, (DWORD)eLootType, dwTypeID, dwSetID, &bRet);
	return bRet;
}


//--------------------------------------------------------------------------------------------------------------------------------------------
//���ﱻ��ɱ������Ǯ
VOID WorldScript::DropSilver(Creature * pCreature, DWORD dwRoleID, DWORD dwType, INT nSilver) const
{
	return;
	if (!P_VALID(m_szFunc[ESWE_BeKill_DropSilver]))	return;

	if(!P_VALID(pCreature))	
		return;
	
	Map* pMap = pCreature->GetMap();
	if(!P_VALID(pMap))	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(	m_szFunc[ESWE_BeKill_DropSilver], "uuuuui", dwMapID, dwInstanceID, dwRoleID, pCreature->GetID(), dwType, nSilver );
}

VOID WorldScript::GetSoulCrystalQlty(DWORD dwTypeID, BYTE& byQlty) const
{
	if (!P_VALID(m_szFunc[ESWE_GetSoulCrystalQlty]))	
		return;

	//BYTE byQlty = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_GetSoulCrystalQlty], "u>i", dwTypeID, &byQlty);

}

DWORD WorldScript::GetSoulCrystalSkillID(DWORD dwTypeID, BYTE byQlty) const
{
	if (!P_VALID(m_szFunc[ESWE_GetSoulCrystalSkillID]))	
		return 0;

	DWORD dwSkillID = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_GetSoulCrystalSkillID], "ui>u", dwTypeID, byQlty, &dwSkillID);

	return dwSkillID;
}

VOID WorldScript::GetSoulCrystalAtt(DWORD dwRoleID, DWORD dwTypeID, BYTE byQlty, INT* nRetAttID, bool bFirstCreate, INT nRateAdd1, INT nRateAdd2,
							 INT* nAttID) const
{
	if (!P_VALID(m_szFunc[ESWE_GetSoulCrystalAtt]))	
		return;

	if (!P_VALID(nRetAttID))
	{
		return;
	}
	
	DWORD dwMapID = GT_INVALID;
	DWORD dwInstanceID = GT_INVALID;
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		Map* pMap = pRole->GetMap();
		if (!P_VALID(pMap))	return;

		DWORD dwMapID = pMap->GetMapID();
		DWORD dwInstanceID = pMap->GetInstanceID();
	}
	else
	{
		dwRoleID = 0;
	}

	if(P_VALID(nAttID))
		g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_GetSoulCrystalAtt], "uuuiiiiuiiiiiiii>iiiiiiii", dwMapID, dwInstanceID, 
		dwTypeID, bFirstCreate, byQlty, nRateAdd1, nRateAdd2, dwRoleID, nAttID[0], nAttID[1], nAttID[2], nAttID[3], 
		nAttID[4], nAttID[5], nAttID[6], nAttID[7], 
		&nRetAttID[0], &nRetAttID[1], &nRetAttID[2], &nRetAttID[3], &nRetAttID[4], &nRetAttID[5], &nRetAttID[6], &nRetAttID[7]);
	else
		g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_GetSoulCrystalAtt], "uuuiiiiuiiiiiiii>iiiiiiii", dwMapID, dwInstanceID, 
		dwTypeID, bFirstCreate, byQlty, nRateAdd1, nRateAdd2, dwRoleID, 0, 0, 0, 0, 0, 0, 0, 0,
		&nRetAttID[0], &nRetAttID[1], &nRetAttID[2], &nRetAttID[3], &nRetAttID[4], &nRetAttID[5], &nRetAttID[6], &nRetAttID[7]);
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// ����һ���µ���
VOID WorldScript::CreateItem( DWORD dwMapID, DWORD dwInstanceID, DWORD dwTypeID, INT nQuality ) const
{
	return;
	if( !P_VALID(m_szFunc[ESWE_CreateItem]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_CreateItem], "uuui", dwMapID, dwInstanceID, dwTypeID, nQuality );

}

//--------------------------------------------------------------------------------------------------------------------------------------------
//��Ʒ��ʧ�����㻯���߱�ɾ������ʱ�䵽���߳��ۣ�������ֱ�ӽ��׺Ͱ�̯���ף�
VOID WorldScript::DelItem( DWORD dwDelType, DWORD dwTypeID, INT nQuality ) const
{
	return;
	if( !P_VALID(m_szFunc[ESWE_DelItem]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_DelItem], "uui", dwDelType, dwTypeID, nQuality );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// ��Ʒ�����ף�ͨ����̯������
VOID WorldScript::ExchangeItem( Map* pMap, DWORD dwRoleID, DWORD dwBuyRoleID, DWORD dwTypeID, INT nQuality, INT nSilver ) const
{
	return;
	if( !P_VALID(m_szFunc[ESWE_ExchangeItem]) ) return;

	if(!P_VALID(pMap))
		return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESWE_ExchangeItem], "uuuuuii", pMap->GetMapID(), pMap->GetInstanceID(), dwRoleID, dwBuyRoleID, dwTypeID, nQuality, nSilver );

}

//---------------------------------------------------------------------------------------------
// ��Ʒ�¼��ű�
//---------------------------------------------------------------------------------------------
INT ItemScript::CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, BOOL &bIgnore) const
{
	if( !P_VALID(m_szFunc[ESIE_CanUse]) ) return 0;
	BOOL bUseable = FALSE;
	INT	 nRet = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_CanUse], "uuuu>ii", pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, dwTargetID, &nRet, &bIgnore);

	return nRet;
}

INT ItemScript::CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, DWORD dwDesRoleID, BOOL &bIgnore) const
{
	if( !P_VALID(m_szFunc[ESIE_CanUse]) ) return 0;
	BOOL bUseable = FALSE;
	INT	 nRet = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_CanUse], "uuuuu>ii", pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, dwTargetID, dwDesRoleID, &nRet, &bIgnore);

	return nRet;
}

INT ItemScript::UseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID) const
{
	if( !P_VALID(m_szFunc[ESIE_Use]) ) return 0;

	INT re = 0;
	// ��Ҫע�⣬���luaû�з���ֵ������ȡ����ֵ�Ļ������ȡ��0ֵ��
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_Use], "uuuu>i", pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, dwTargetID,&re);
	return re;
}

VOID ItemScript::UseItem(Map* pMap, DWORD dwTypeID, DWORD dwScrRoleID, DWORD dwDesRoleID) const
{
	if( !P_VALID(m_szFunc[ESIE_Use]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_Use], "uuuuu", pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, dwScrRoleID, dwDesRoleID);
}

INT ItemScript::CanPickUpItem(DWORD dwItemID, DWORD dwRoleID) const
{
	if (!P_VALID(m_szFunc[ESIE_CanPickUpItem]))	return E_Success;

	INT nErrCode = E_Success;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_CanPickUpItem], "uu>i", dwItemID, dwRoleID, &nErrCode);

	return nErrCode;
}

VOID ItemScript::OnPickUpItem(DWORD dwItemID, DWORD dwRoleID) const
{
	if (!P_VALID(m_szFunc[ESIE_PickUpItem]))	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_PickUpItem], "uu", dwItemID, dwRoleID);
}

VOID ItemScript::PutDownItem(DWORD dwTypeID) const
{
	if( !P_VALID(m_szFunc[ESIE_PutDownItem]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_PutDownItem], "u", dwTypeID);
}

INT ItemScript::ItemCanPutDown(DWORD dwTypeID, INT64 n64Serial) const
{
	if( !P_VALID(m_szFunc[ESIE_CanPutDown]) ) return E_Success;

	INT nErrCode = E_Success;
	const INT32 nMask	= 0xFFFFFFFF;
	INT32 n32High	= (INT32)((n64Serial >> 32) & nMask);
	INT32 n32Low	= (INT32)(n64Serial & nMask);

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_CanPutDown], "uuu>i", dwTypeID, n32High, n32Low, &nErrCode);

	return nErrCode;
}

INT ItemScript::GroundItemCanDel(DWORD dwTypeID, INT64 n64Serial) const
{
	if( !P_VALID(m_szFunc[ESIE_CanDelGroundItem]) ) return E_Success;

	INT	 nRet = E_Success;
	const INT32 nMask	= 0xFFFFFFFF;
	INT32 n32High	= (INT32)((n64Serial >> 32) & nMask);
	INT32 n32Low	= (INT32)(n64Serial & nMask);

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_CanDelGroundItem], "uuu>i", dwTypeID, n32High, n32Low, &nRet);

	return nRet;
}

BOOL ItemScript::CanDeleteItem(DWORD dwTypeID, INT64 n64Serial) const
{
	if( !P_VALID(m_szFunc[ESIE_CanDelItemOnRoleInit]) ) return FALSE;

	INT nRet = E_Success;
	const INT32 nMask	= 0xFFFFFFFF;
	INT32 n32High	= (INT32)((n64Serial >> 32) & nMask);
	INT32 n32Low	= (INT32)(n64Serial & nMask);

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_CanDelItemOnRoleInit], "uuu>i", dwTypeID, n32High, n32Low, &nRet);

	return (nRet == E_Success);
}

VOID ItemScript::GetItem(Map* pMap, DWORD dwTypeID, DWORD dwRoleID, EItemCreateMode eGetMode, DWORD n32High, DWORD n32Low) const
{
	if (!P_VALID(m_szFunc[ESIE_Get]))	return;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESIE_Get], "uuuuuuu", pMap->GetMapID(), pMap->GetInstanceID(), dwTypeID, dwRoleID, eGetMode, n32High, n32Low);
	
}

//-------------------------------------------------------------------------------------
// �ܷ�ʹ�ü���
//-------------------------------------------------------------------------------------
DWORD SkillScript::CanCastSkill( Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwDstUnitID ) const
{
	if( !P_VALID(m_szFunc[ESSE_CanCast]) ) return 0;
	BOOL bRet = FALSE;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESSE_CanCast], "uuuuu>i", pMap->GetMapID(), pMap->GetInstanceID(), dwSkillID, dwOwnerID, dwDstUnitID, &bRet);

	return bRet;
}

//-------------------------------------------------------------------------------------
// ʹ�ü���
//-------------------------------------------------------------------------------------
VOID SkillScript::CastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, BOOL &bIgnore) const
{
	if( !P_VALID(m_szFunc[ESSE_Cast]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESSE_Cast], "uuuu>i", pMap->GetMapID(), pMap->GetInstanceID(), dwSkillID, dwOwnerID, &bIgnore);
}

//-------------------------------------------------------------------------------------
// ʹ�õ�Ŀ�꼼�ܣ�����ԭ�ͱ���type2�ֶ�Ϊ36��
//-------------------------------------------------------------------------------------
VOID SkillScript::CastSingleTargetSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, BOOL &bIgnore) const
{
	if( !P_VALID(m_szFunc[ESSE_Cast2SingleTarget]) ) return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESSE_Cast2SingleTarget], "uuuuu>i", pMap->GetMapID(), pMap->GetInstanceID(), dwSkillID, dwOwnerID, dwTargetID, &bIgnore);
}

//-------------------------------------------------------------------------------------
// ��ӡ
//-------------------------------------------------------------------------------------
VOID SkillScript::SealSkill(Map *pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, DWORD dwTargetTypeID) const
{
	if ( !P_VALID(m_szFunc[ESSE_SealSkill]) ) return;

	if ( !P_VALID(dwTargetTypeID) )	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESSE_SealSkill], "uuuuuu", pMap->GetMapID(), 
								pMap->GetInstanceID(), dwSkillID, dwOwnerID, dwTargetID, dwTargetTypeID);
}

//-------------------------------------------------------------------------------------
// ȡ��ʹ�ü���
//-------------------------------------------------------------------------------------
VOID SkillScript::CancelSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, DWORD dwTargetTypeID) const
{
	if ( !P_VALID(m_szFunc[ESSE_CancelSkill]) ) return;

	if ( !P_VALID(dwTargetTypeID) )	return;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESSE_CancelSkill], "uuuuuu", pMap->GetMapID(), 
								pMap->GetInstanceID(), dwSkillID, dwOwnerID, dwTargetID, dwTargetTypeID);
}

VOID	SkillScript::OnSkillChange(Map* pMap,DWORD dwRoleID,DWORD dwSkillID,INT nPrevVal,INT nCurVal) const
{
	if (!P_VALID(m_szFunc[ESSE_OnSkillChange]))	return;

	if (!P_VALID(pMap))
	{
		return ;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESSE_OnSkillChange], "uuuuuu", pMap->GetMapID(), pMap->GetInstanceID(),dwRoleID,dwSkillID,nPrevVal,nCurVal);  
}

VOID RoleScript::OnRoleCondenseUpgrade(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_CondenseUpgrade])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CondenseUpgrade], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), pRole->GetAttValue(ERA_God_Godhead));
}

VOID RoleScript::OnRoleGetClergy(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_GetClergy])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_GetClergy], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), pRole->GetClergy());
}

VOID RoleScript::OnRoleGetRareArea(Role* pRole, DWORD dwAreaID) const
{
	if( !P_VALID(m_szFunc[ESRE_GetRareArea])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_GetRareArea], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), dwAreaID);
}

VOID RoleScript::OnRoleWinRareArea(Role* pRole, DWORD dwRoleID, DWORD dwAreaID) const
{
	if( !P_VALID(m_szFunc[ESRE_WinRareArea])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_WinRareArea], "uuuuu", dwMapID, dwInstanceID, pRole->GetID(), dwRoleID,dwAreaID);
}

VOID RoleScript::OnRoleActivateArea(Role* pRole, DWORD dwAreaID) const
{
	if( !P_VALID(m_szFunc[ESRE_ActivateArea])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_ActivateArea], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), dwAreaID);
}

BOOL RoleScript::IsCanRobRareArea(Role* pRole) const
{
	if( !P_VALID(m_szFunc[ESRE_IsCanRobRareArea])) 
		return FALSE;

	BOOL  bRet = TRUE;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return FALSE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_IsCanRobRareArea], "uuu>b", dwMapID, dwInstanceID, pRole->GetID(), &bRet);

	return bRet;
}

// �Ƿ����ʹ����վNPC
BOOL RoleScript::CanUseDakNPC(Role* pRole, DWORD dwDestMapID, FLOAT x, FLOAT y, FLOAT z) const
{
	if (!P_VALID(m_szFunc[ESRE_CanUseDakNPC]))
		return TRUE;

	BOOL  bRet = TRUE;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return TRUE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CanUseDakNPC], "uuuuuuu>b", dwMapID, dwInstanceID, pRole->GetID(), dwDestMapID, (DWORD)(x/50), (DWORD)y, (DWORD)(z/50), &bRet);

	return bRet;
}

// �Ƿ����ʹ��������
BOOL RoleScript::CanUseEarthTemple(Role* pRole, DWORD dwDestMapID, FLOAT x, FLOAT y, FLOAT z) const
{
	if (!P_VALID(m_szFunc[ESRE_CanUseEarthTemple]))
		return TRUE;

	BOOL  bRet = TRUE;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return TRUE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CanUseEarthTemple], "uuuuuuu>b", dwMapID, dwInstanceID, pRole->GetID(), dwDestMapID, (DWORD)(x/50), (DWORD)y, (DWORD)(z/50), &bRet);

	return bRet;
}

//ESRE_CanCalCrit = 69,	// �Ƿ���㱩��
BOOL	RoleScript::CanCalCrit(Role* pRole, DWORD dwTargetID) const
{
	if (!P_VALID(m_szFunc[ESRE_CanCalCrit]))
		return TRUE;

	BOOL  bRet = TRUE;
	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return TRUE;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_CanCalCrit], "uuuu>b", dwMapID, dwInstanceID, pRole->GetID(), dwTargetID, &bRet);

	return bRet;
}

VOID RoleScript::OnRoleLogout(Role* pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_RoleLogout]))
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_RoleLogout], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

VOID RoleScript::OnCompleteFashionSuitNumChange(Role* pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_On_FashionSuitNumChg]))
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_FashionSuitNumChg], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

VOID RoleScript::OnRoleWardLevelup(Role* pRole) const
{
	if (!P_VALID(m_szFunc[ESRE_On_WardLevelUp]))
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_On_WardLevelUp], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}
	

// ���������ý�Ǯ
VOID RoleScript::IncSilverByQuest(Role* pRole, DWORD dwQuestID, INT nSilver) const
{
	return;
	if( !P_VALID(m_szFunc[ESRE_Quest_IncSilver])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_Quest_IncSilver], "uuuui", dwMapID, dwInstanceID, pRole->GetID(), dwQuestID, nSilver );
}

// ͨ������������Ľ��
VOID RoleScript::DecSilverByBuyItem(Role* pRole, DWORD dwItemID, INT nSilver) const
{
	return;
	if( !P_VALID(m_szFunc[ESRE_BuyItem_DecSilver])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_BuyItem_DecSilver], "uuuui", dwMapID, dwInstanceID, pRole->GetID(), dwItemID, nSilver );
}

// ͨ������װ��ǿ���ȼ����Ľ��
VOID RoleScript::DecSilverByStrengthen(Role* pRole, DWORD dwTypeID, INT nQuality, INT nLevel, INT nSilver ) const
{
	return;
	if( !P_VALID(m_szFunc[ESRE_Strengthen_DecSilver])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_Strengthen_DecSilver], "uuuuiii", dwMapID, dwInstanceID, pRole->GetID(), dwTypeID, nQuality, nLevel, nSilver);
}

// ͨ���ϳ����Ľ��
VOID RoleScript::DecSilverByCompose( Role* pRole, DWORD dwTypeID, INT nSilver ) const
{
	return;
	if( !P_VALID(m_szFunc[ESRE_Compose_DecSilver])) 
		return;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) )	
		return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_Compose_DecSilver], "uuuui", dwMapID, dwInstanceID, pRole->GetID(), dwTypeID, nSilver );
}

DWORD RoleScript::CanStrengthenEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow, INT nQuality, INT nLevel) const
{
	if (!P_VALID(m_szFunc[ESRE_Can_StrengthenEquip]))	
		return 2;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwSucceed = 2;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_Can_StrengthenEquip], "uuuuuuii>u", dwMapID, dwInstanceID, pRole->GetID(), dwTypeID, dwSerialIDHigh, dwSerialIDLow, nQuality, nLevel, &dwSucceed);

	return dwSucceed;
}

DWORD RoleScript::OnRoleMapChangeWithScriptEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow,DWORD dwMapID) const
{
	if (!P_VALID(m_szFunc[ESRE_MapChangeWithScriptEquip]))	
		return 0;

	DWORD dwSucceed = 0;

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_MapChangeWithScriptEquip], "uuuuii>u", dwMapID, GT_INVALID, pRole->GetID(), dwTypeID, dwSerialIDHigh, dwSerialIDLow, &dwSucceed);

	return dwSucceed;
}

DWORD RoleScript::OnRoleTryToWareScriptEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow) const
{
	if (!P_VALID(m_szFunc[ESRE_TryToWareScriptEquip]))	
		return 0;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return E_SystemError;

	DWORD dwSucceed = 0;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_TryToWareScriptEquip], "uuuuii>u", dwMapID, dwInstanceID, pRole->GetID(), dwTypeID, dwSerialIDHigh, dwSerialIDLow, &dwSucceed);

	return dwSucceed;
}

DWORD RoleScript::OnRoleEquipGetGrowID(Role* pRole, DWORD dwTypeID, INT nQlty) const
{
	if (!P_VALID(m_szFunc[ESRE_EquipGetGrowAtt]))	
		return 0;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return 0;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	DWORD dwGrowID = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_EquipGetGrowAtt], "uuuui>u", pRole->GetID(), dwMapID, dwInstanceID, dwTypeID, nQlty, &dwGrowID);

	return dwGrowID;
}

BYTE RoleScript::OnRoleEquipGrow(Role* pRole, DWORD dwTypeID, BYTE byQlty, INT nGrowTimes, INT nIMUseTimes, BYTE byGrowLevel, 
					 BYTE byGrayNum, BYTE byGreenNum, BYTE byBlueNum, BYTE byPurpleNum, DWORD dwIM) const
{
	if (!P_VALID(m_szFunc[ESRE_EquipGrowGetStar]))	
		return GT_INVALID;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return GT_INVALID;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	EEquipGrowStar eStar = EEGS_End;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_EquipGrowGetStar], "uuuuiiiiiiiiu>i", pRole->GetID(), dwMapID, dwInstanceID, dwTypeID, byQlty, 
		nGrowTimes, nIMUseTimes, byGrowLevel, byGrayNum, byGreenNum, byBlueNum, byPurpleNum, dwIM, &eStar);

	return (BYTE)eStar;
}

VOID RoleScript::OnGetBoardQuestReward(Role* pRole, UINT16 u16QuestID, UINT16 u16BoardID, INT nMoney, INT nExp) const
{
	if (!P_VALID(m_szFunc[ESRE_RoleGetBoardQuestReward]))	
		return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_RoleGetBoardQuestReward], "uuuiiii", dwMapID, dwInstanceID, 
		pRole->GetID(), u16QuestID, u16BoardID, nMoney, nExp);
}

VOID RoleScript::RenewSoulCrystalAtt(Role* pRole, DWORD dwTypeID, BYTE byQlty, INT* nAttID, OUT DWORD& dwErrorCode, OUT INT* nRetAttID) const
{
	if (!P_VALID(m_szFunc[ESRE_GetSoulCrystalRenew]))	
		return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	if (!P_VALID(nAttID) || !P_VALID(nRetAttID))
	{
		return;
	}

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_GetSoulCrystalRenew], "uuuiuiiiiiiii>uiiiiiiii", dwMapID, dwInstanceID, 
		dwTypeID, byQlty, pRole->GetID(), nAttID[0], nAttID[1], nAttID[2], nAttID[3], nAttID[4], nAttID[5], nAttID[6], nAttID[7], 
		&dwErrorCode, &nRetAttID[0], &nRetAttID[1], &nRetAttID[2], &nRetAttID[3], &nRetAttID[4], &nRetAttID[5], &nRetAttID[6], &nRetAttID[7]);
}

VOID RoleScript::AccountSuccessReactive(Role* pRole) const 
{
	if (!P_VALID(m_szFunc[ESRE_AccountReactived]))	
		return;

	if(!P_VALID(pRole))
		return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_AccountReactived], "uuu", dwMapID, dwInstanceID, pRole->GetID());
}

DWORD RoleScript::OnUseDiamondAddAndEquipGrowIM(Role* pRole, DWORD dwTypeID, INT64 n64EquipID) const
{
 	if(!P_VALID(m_szFunc[ESRE_UseDiamondAddAndEquipGrowIM]))
 		return 0;
 
  	if(!P_VALID(pRole))
  		return 0;
  
  	Map* pMap = pRole->GetMap();
  	if(!P_VALID(pMap)) return 0;
  	DWORD dwMapID = pMap->GetMapID();
  	DWORD dwInstanceID = pMap->GetInstanceID();
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(n64EquipID);
	DWORD dwEquipTypeID;
	if(P_VALID(pItem))
		dwEquipTypeID = pItem->dwTypeID;
	else
		dwEquipTypeID = GT_INVALID;
//   	INT32 n32High;
//   	INT32 n32Low;
	const INT32 nMask	= 0xFFFFFFFF;
	DWORD n32High	= (DWORD)((n64EquipID >> 32) & nMask);
	DWORD n32Low	= (DWORD)(n64EquipID & nMask);

  	//ParseINT64(n64EquipID, n32High, n32Low);
	DWORD dwErrorCode = 0;
  	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_UseDiamondAddAndEquipGrowIM], "uuuuuuu>u", dwMapID, dwInstanceID, pRole->GetID(), dwTypeID, dwEquipTypeID,n32High, n32Low, &dwErrorCode);
	return dwErrorCode;
}

DWORD RoleScript::OnRefreshBaoXiang(DWORD dwScole, DWORD &dwItemID1, DWORD &dwItemID2, DWORD &dwItemID3) const
{
	if(!P_VALID(m_szFunc[ESRE_RefreshBaoXiang]))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = 0;
	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_RefreshBaoXiang], "u>uuuu", dwScole, &dwItemID1, &dwItemID2, &dwItemID3, &dwErrorCode);
	return dwErrorCode;
}

VOID RoleScript::OnGetHolyDevourNum(Role* pRole, DWORD dwDevourNum) const
{
	if(!P_VALID(m_szFunc[ESRE_GetHolyDevourNum]))
		return;

	if(!P_VALID(pRole))
		return;

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))	return;

	DWORD dwMapID = pMap->GetMapID();
	DWORD dwInstanceID = pMap->GetInstanceID();

	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_GetHolyDevourNum], "uuuu", dwMapID, dwInstanceID, pRole->GetID(), dwDevourNum);
}

// DWORD RoleScript::RewardReactive(Role* pRole, BYTE byRewardLevel) const
// {
// 	if (!P_VALID(m_szFunc[ESRE_RewardReactive]))	
// 		return E_AccountReactive_Error;
// 
// 	if(!P_VALID(pRole))
// 		return E_AccountReactive_Error;
// 
// 	Map* pMap = pRole->GetMap();
// 	if (!P_VALID(pMap))	return E_AccountReactive_Error;
// 
// 	DWORD dwMapID = pMap->GetMapID();
// 	DWORD dwInstanceID = pMap->GetInstanceID();
// 
// 	DWORD dwErrorCode = 0;
// 	g_ScriptMgr.CallScriptFunction(m_szFunc[ESRE_RewardReactive], "uuui>u", dwMapID, dwInstanceID, pRole->GetID(), byRewardLevel, &dwErrorCode);
// 	return dwErrorCode;
// }

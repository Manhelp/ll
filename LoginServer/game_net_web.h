//-----------------------------------------------------------------------------
//!\file game_net_web.h
//!\author liutingting
//!
//!\date 2011-4-29
//! last 2011-4-29
//!
//!\brief http Э������࣬����Russia����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "Singleton.h"
#define		WEB_REQUEST_MAX		2048

class GameNetWeb : public Singleton<GameNetWeb>
{
public:
    //-------------------------------------------------------------------------
	// CONSTURCT
	//-------------------------------------------------------------------------
	GameNetWeb() {Init(); }
	~GameNetWeb() { Destroy(); }

    //-------------------------------------------------------------------------
	// ��ʼ��������
	//-------------------------------------------------------------------------
    BOOL	Init();
    VOID	Destroy();

	DWORD	GetServiceToken( LPCSTR pszToken );
	DWORD	CheckSubscribeStatus( LPCSTR pszToken );

private:
	BOOL	SendRequset( TCHAR *ptszRequest, CHAR *ptszRespones );

};


#define gGameNetWeb GameNetWeb::getSingleton()








//-----------------------------------------------------------------------------
//!\file ClientMain.h
//!\author Lyp
//!
//!\date 2008-03-27
//! last 2008-03-27
//!
//!\brief ���ͻ���
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

struct tagNC_AnswerKoreaCSApprove;
//-----------------------------------------------------------------------------
// Client
//-----------------------------------------------------------------------------
class Client
{
public:
	enum LoadRequiementsLevel
	{
		E_Level_BeforeConnect,
		E_Level_BeforeSelectRole,
		E_Level_BeforeEnterWorld,
	};
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	VOID WaitThreadLoad();
	VOID ExitThreadLoad();
	BOOL IsLoadFinished()const;
	void GetLoadCount(LoadRequiementsLevel level, int& nLoaded, int& nTotal)const;

	Client():m_Trunk(this){}

private:
	TSFPTrunk<Client>			m_Trunk;
	TObjRef<InputDX8>			m_pInput;
	TObjRef<GameInputMap>		m_pInputMap;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<LuaManager>			m_pLuaMgr;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GUIEditor>			m_pGUIEditor;
	TObjRef<Console>			m_pConsole;
	TObjRef<GUIRender>			m_pGUIRender;
	TObjRef<GUIEditor>			m_pEditor;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GameScriptMgr>		m_pGameScriptMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<AudioSys>			m_pAudioSys;


	BOOL InitRenderSystem(HINSTANCE hInst);

	VOID RegisterControl();
	VOID RegisterFrame();

	// GUIWnd��ʾ����
	DWORD SwitchGUIWnd(DWORD dwID, BOOL bDown);
	// ��ͼ
	DWORD ScreenShot(DWORD dwID, BOOL bDown);

	// GM�����
	DWORD GMCommand(LPCTSTR szCommand);

	// ���� 
	DWORD Test();
	
	// ��3D������Դ��״̬д����־�ļ�
	DWORD LogResPool(LPCTSTR szCommand);

	// ��ʾ���ؽ�ɫ��������
	DWORD DisplayLocalRoleAtt(LPCTSTR szCommand);

	//������Ч����С���� add by xp
	DWORD UiCameraTool(LPCTSTR szCommand);

	// �ر�/����ͷ��tips
	DWORD HeadTipsCommand(LPCTSTR szCommand);

	// װ�ط���������log
	DWORD NetCmdViewer(LPCTSTR szCommand, LPCTSTR szStart, LPCTSTR szEnd);

    // ɾ��һ���ļ����������ļ�
    BOOL DeleteDirectory(LPCTSTR szDirName);
	
	HANDLE	m_hThreadLoad;	// ��ȡ�̵߳�HANDLE
	VOLATILE BOOL m_bTerminateThreadLoad; // �Ƿ������ȡ�߳�
	// ��ȡ��Դ�ļ����߳�
	UINT ThreadLoad(LPVOID);

	INT m_nLoadStep;
	BOOL m_bSDOA;			// ʢ��ģʽ
	INT m_nLogin;			// ��½ģʽ
	VOID InitSDOAMode();	
	VOID InitJinShanKuaiKuai();	// ��ɽ���
	VOID LoadJSKKLibary();			// ��̬���ؽ�ɽ���dll

	// ��ȡ��������Ϣ
	DWORD GetServiceInfo(LPCTSTR szCommand);

	// Russia
	HINSTANCE			m_hInstIGWDll;
};


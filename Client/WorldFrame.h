#pragma once
#include "ClientCamera.h"
#include "..\WorldDefine\msg_map.h"
#include "..\WorldDefine\role_info.h"
#include "../WorldDefine/msg_player_preventlost.h"
#include "..\WorldDefine\msg_gm_cmd.h"

/** \class WorldFrame
	\brief ��Ϸ�����߼����
*/
class WorldFrame : public GameFrame
{
public:
	enum EWorldState
	{
		EWState_GotoNewMap,			//���շ����͵�gotomap��Ϣ
		EWState_Playing,			//���Կ�ʼ����

	};

public:
	WorldFrame();
	~WorldFrame();

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Render3D();
	virtual VOID Update();

	//--
	ClientCamera* GetCamera()	{ return &m_camera; }
	
	/** 
		\���͵��µ�ͼ, ����׷�����
	*/
	void GoToMap( DWORD dwMapID, Vector3 vecPos, Vector3 vecFaceTo );
	
	/** ���WorldFrame �ĵ�ǰ״̬
		\ Ϊ����׷�����
	*/
	EWorldState GetCurWorldSate(){ return m_curState; }

private:
	TSFPTrunk<WorldFrame>		m_Trunk;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<AudioSys>			m_pAudioSys;
	TObjRef<GUISystem>			m_pSys;
	ClientCamera				m_camera;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	EWorldState					m_curState;
	TObjRef<Util>				m_pUtil;

	BOOL	LoadRes(const TCHAR* szMapName, DWORD dwMapID, const Vector3& localPlayerPos, const Vector3& localPlayerFaceto);
	void	switchToState(EWorldState state);

	DWORD	NetRecvGotoMap(tagNS_GotoNewMap* pMsg, DWORD pPrama);

	// ��Ӧ������Ϣ
	/*DWORD	NetRecvRoleGetName(tagNS_RoleGetName* pNetCmd, DWORD dwParam);*/
	DWORD	NetRecvRoleGetID(tagNS_RoleGetID* pNetCmd, DWORD dwParam);

	//��ʼ�������͸�ģ��
	DWORD NetRecvLoongBeneInit( tagNS_LoongBenedictionInit* pNetCmd, DWORD dwParam );

	DWORD OnNS_OfflineExp( tagNS_OfflineExperienceReward* pNetCmd, DWORD dwParam );
	
	DWORD OnNS_PlayerRegression( tagNS_PlayerRegression* pNetCmd, DWORD dwParam );

	// �Ͽ������������
	DWORD	OnDisconnectionEvent(tagMsgBoxEvent* pEvent);

	DWORD OnNS_GetServiceInfo( tagNS_GetServiceInfo* pNetCmd, DWORD dwParam );

	// ����ESC��
	DWORD HandleESC( DWORD dwID, BOOL bDown );
};

inline WorldFrame* GetWorldFrm()
{
	return (WorldFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("World"));
}

#pragma once
#include "gameframe.h"

#define MAX_RANPVP_NUM 6
class IconStatic;
enum EClassType;
struct tagNS_PvPRanRoomInfo;
struct tagNS_PvPRanRoomInfoSingleChange;
struct tagNS_PvPJoinRoom;
struct tagNS_PvPLeaveRoom;

struct tagPvPRanSaveInfo
{
	INT			nPlayerLevel;			//Զ����ҵȼ�
	EClassType	ePlayerProfession;		//Զ�����ְҵ
	DWORD		dwPlayerID;				//���ID
	tstring		strName;				//�������
};

struct tagPvPPlayerInfo
{
	GUIStatic* pStcBack;	//����

	GUIStatic* pStcName;		//����
	GUIStatic* pStcLevel;		//�ȼ�
	GUIStatic* pStcProfession;	//ְҵ
	GUIStatic* pStcJi;			//"��"��
	IconStatic* pStcProfessionPic;		//ְҵͼƬ

};

class RandomPvPFrame :
	public GameFrame
{
public:
	RandomPvPFrame(void);
	~RandomPvPFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID ClearInfo(tagPvPPlayerInfo &strPvPinfo);
	VOID ClearSaveInfo(tagPvPRanSaveInfo &strPvPinfo);
	VOID RefreshPlayer(INT pos,INT level,EClassType eType);
	VOID ClearAll();

	tstring& GetPvPmemberName(DWORD dwPlayerID);
	EClassType GetPvPmemberClass(DWORD dwPlayerID);

	VOID HideWnd(){m_pWnd->SetInvisible(true); m_pWndMin->SetInvisible(true);}

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_PvPRanRoomInfo(tagNS_PvPRanRoomInfo* pNetCmd,DWORD dwParam);
	DWORD OnNS_PvPRanRoomInfoSingleChange(tagNS_PvPRanRoomInfoSingleChange* pNetCmd, DWORD dwParam);
	DWORD OnNS_PvPJoinRoom(tagNS_PvPJoinRoom* pNetCmd,DWORD dwParam);
	DWORD OnNS_PvPLeaveRoom(tagNS_PvPLeaveRoom* pNetCmd,DWORD dwParam);
	

private:
	// tools
	TSFPTrunk<RandomPvPFrame>		m_Trunk;
	TObjRef<GUISystem>          m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	GUIWnd*                     m_pWnd;

	GUIWnd*                     m_pWndMin;
	GUIButton*					m_pBnMin;			// ��С��

	GUIButton*					m_pBnHide;			// ����
	GUIButton*					m_pBnExit;			// �˳�
	GUIButton*					m_pBnChangeRoom;			// ��������

	GUIStatic*					m_pStcName;			// ����
	GUIStatic*					m_pStcLevel;		// �ȼ�
	GUIStatic*					m_pStcProfession;	// ְҵ
	GUIStatic*					m_pStcGlory;		// ����
	GUIStatic*					m_pStcRankLevel;	// ����
	GUIStatic*					m_pStcSucessRate;	// ʤ��
	GUIStatic*					m_pStcPvPPoint;		// ��������
	tagPvPPlayerInfo			m_strPvPPlayerInfo[MAX_RANPVP_NUM];
	tagPvPRanSaveInfo			m_strPvPSaveInfo[MAX_RANPVP_NUM];
	tstring						m_strEmpty;
};

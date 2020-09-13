#pragma once

#include "Render2D.h"

struct tagRoomChange;
struct tagRoomListChange;
class RoomListItemCtrl;
enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
struct tagNS_PvPEnterHall;
struct tagNS_PvPLeaveHall;
struct tagNS_PvPGetRoomList;
struct tagNS_PvPGetRoomInfo;
struct tagNS_PvPGetSimRoomInfo;
struct tagNS_PvPJoinRoom;
struct tagNS_PvPCreateRoom;
struct tagNS_PvPLeaveRoom;
struct tagNS_PvPChangePosition;
struct tagNS_PvPPrepare;
struct tagNS_PvPUnPrepare;
struct tagNS_PvPKickPlayer;
struct tagNS_PvPKicked;
struct tagNS_PvPStart;
struct tagNS_PvPStarted;
struct tagInvalidRoomEvent;
struct tagEnterHallEvent;
struct tagNS_PvPSetIntoRoom;
struct tagNS_PvPInvite;
struct tagNS_PvPInviteJoin;
struct tagNS_PvPEnterPvP;
class StaticEx;
class PvPRoomPosCtrl;
class PvPRoomCtrl;
class PvPRoom;
class IconStatic;

#define MAX_PVP_ROOM	10
#define NUM_3V3			6

class PvPFrame : public GameFrame
{
public:
	PvPFrame(void);
	~PvPFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

protected:
	void LeaveHall();
	void JoinRoom(E_PvP_RoomLevel level, INT16 roomID);
	void AutoJoin(E_PvP_RoomLevel level);
	void InviteFriend(LPCTSTR szRoleName);
	void CreateRoom(E_PvP_RoomLevel level, INT iLevelUpperLimit, INT iLevelLowerLimit,	DWORD dwWuXunLimit);
	void LeaveRoom();
	void ChangePos(BYTE newPos);
	void KickPlayer(DWORD dwRoleID);
	void Prepare();
	void UnPrepare();
	void Start();
	void RefuseJoin(E_PvP_RoomType hall, E_PvP_RoomLevel level, INT16 roomID, DWORD invitorID, BOOL bRefused = TRUE);

	void ShowErrorMsg( DWORD dwErrCode );
	void ShowErrorMsg( DWORD dwErrCode, DWORD roleID);
	void SetLevel(E_PvP_RoomLevel level);
	void SetCurrentPage(INT page);
	void SetCurSelect(INT nIndex);
	void ShowCreateRoomWnd();
	void ShowRoom3v3(E_PvP_RoomLevel eLevel, INT16 roomID);
	std::list<INT16> FilterRooms(const std::list<INT16>& original)const;
	void SetCurSelectPos(BYTE pos);
	void SetIntoRoom(E_PvP_RoomType hallType, E_PvP_RoomLevel eLevel, INT16 roomID);
	void FixPostion();	// ��������λ�ã�������ס���촰��	

	void FolderRoom(BOOL bFolder = TRUE);	// �۵�����
	void ToggleFolderRoom();	// �۵�/չ������
	void ShowEnterPvPPic(INT nIndex); // ��ʾ���뾺��������ʱͼƬ
private:
	DWORD RoomListEventHandler(tagGUIEvent* pEvent);	
	DWORD CreateRoomEventHandler(tagGUIEvent* pEvent);	
	DWORD Room3v3EventHandler(tagGUIEvent* pEvent);	
	DWORD OnRoomChange(tagRoomChange* pGameEvent);
	DWORD OnRoomListChange(tagRoomListChange* pGameEvent);
	DWORD OnInvalidRoom(tagInvalidRoomEvent* pGameEvent);
	DWORD OnEnterHall(tagEnterHallEvent* pGameEvent);
	virtual BOOL EscCancel();

	// ������������Ϣ
	DWORD NetRecvEnterHall(tagNS_PvPEnterHall* pMsg, DWORD); 
	// �����뿪������Ϣ
	DWORD NetRecvLeaveHall(tagNS_PvPLeaveHall* pMsg, DWORD); 	
	// �������б���Ϣ
	DWORD NetRecvRoomList(tagNS_PvPGetRoomList* pMsg, DWORD); 
	// �����������Ϣ
	DWORD NetRecvSimRoomInfo(tagNS_PvPGetSimRoomInfo* pMsg, DWORD);
	// ��������ϸ��Ϣ
	DWORD NetRecvRoomInfo(tagNS_PvPGetRoomInfo* pMsg, DWORD);
	// ������뷿����Ϣ
	DWORD NetRecvJoinRoom(tagNS_PvPJoinRoom* pMsg, DWORD);
	// ������������ϸ��Ϣ
	DWORD NetRecvCreatRoom(tagNS_PvPCreateRoom* pMsg, DWORD);
	// �����˳�������Ϣ
	DWORD NetRecvLeaveRoom(tagNS_PvPLeaveRoom* pMsg, DWORD);
	// �����û�λ����Ϣ
	DWORD NetRecvChangePos(tagNS_PvPChangePosition* pMsg, DWORD);
	// ����׼����ϸ��Ϣ
	DWORD NetRecvPrepare(tagNS_PvPPrepare* pMsg, DWORD);
	// ����ȡ��׼����Ϣ
	DWORD NetRecvUnPrepare(tagNS_PvPUnPrepare* pMsg, DWORD);
	// ����������Ϣ
	DWORD NetRecvKickPlayer(tagNS_PvPKickPlayer* pMsg, DWORD);	
	// ��������Ϣ
	DWORD NetRecvKicked(tagNS_PvPKicked* pMsg, DWORD);
	// ��ʼս������
	DWORD NetRecvStart(tagNS_PvPStart* pMsg, DWORD);	
	// ��ʼս��
	DWORD NetRecvStarted(tagNS_PvPStarted* pMsg, DWORD);	
	// ���ط���
	DWORD NetRecvSetInfoRoom(tagNS_PvPSetIntoRoom* pMsg, DWORD);
	// ������
	DWORD NetRecvInvite(tagNS_PvPInvite* pMsg, DWORD);
	// ������
	DWORD NetRecvInviteJoin(tagNS_PvPInviteJoin* pMsg, DWORD);
	// ��ʼ����
	DWORD NetRecvEnterPvP(tagNS_PvPEnterPvP* pMsg, DWORD);

private:
	TSFPTrunk<PvPFrame>						m_Trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<GUIRender>						m_pRender;
	DWORD									m_dwTime;
	DWORD									m_dwTimeDue;				

	GUIWnd*									m_pWndHall;
	GUIWnd*									m_pWnd3v3Room;
	GUIWnd*									m_pWndCreate;
	GUIWnd*									m_pWnd3V3Folder;
	GUIWnd*									m_pWndRuleDescription;
	GUIWnd*									m_pWndInviteConfirm;
	GUIWnd*									m_pWndEnterPvPTimeOut;

	// ���������ϵĿؼ�
	GUIStatic*								m_pStaticPageNum;
	GUIButton*								m_pBtnNextPage;
	GUIButton*								m_pBtnPrePage;
	GUIButton*								m_pBtnRefresh;
	GUIButton*								m_pBtnClose;
	GUIButton*								m_pBtnLeave;
	GUIButton*								m_pBtnCreate;
	GUIButton*								m_pBtnEnter;
	GUIButton*								m_pBtn3v3AutoEnter;
	GUIButton*								m_pBtn3v3Rule;
	GUIPushButton*							m_pRadioFiler;
	GUIEditBox*								m_pEdtSeach;
	GUIButton*								m_pBtnSearch;
	GUIPushButton*							m_pTabJunior;
	GUIPushButton*							m_pTabMid;
	GUIButton*								m_pBtnListItem[MAX_PVP_ROOM];
	StaticEx*								m_pImgListItem[MAX_PVP_ROOM];

	// ������������ϵĿؼ�
	GUIEditBox*								m_pEdtWuXun;
	GUIEditBox*								m_pEdtLevelLower;
	GUIEditBox*								m_pEdtLevelUpper;
	GUIListBox*								m_pLstWuXun;
	GUIListBox*								m_pLstLevelLower;
	GUIListBox*								m_pLstLevelUpper;
	GUIButton*								m_pBtnCreateRoom;
	GUIButton*								m_pBtnCloseCreateWnd;

	// 3v3�����ϵĿؼ�	
	GUIButton*								m_pBtn3v3Kick; // ����
	GUIButton*								m_pBtn3v3Prapare; // ׼��
	GUIButton*								m_pBtn3v3Leave; // �뿪
	GUIButton*								m_pBtn3v3Start; // ��ʼ
	GUIButton*								m_pBtn3v3Invide; // ����
	GUIComboBox*							m_pCmb3v3FriendList; // �����б�
	GUIEditBox*								m_pEdt3v3Friend; // ����
	//GUIButton*							m_pBtn3v3Close; // �ر�
	GUIButton*								m_pBtn3v3Folder; // �۵�����
	PvPRoomCtrl*							m_p3v3RoomCtrl;
	GUIButton*								m_pBtn3v3PosTab[NUM_3V3];
	PvPRoomPosCtrl*							m_p3v3PosCtrl[NUM_3V3];
	StaticEx*								m_p3v3ImgListItem[NUM_3V3];

	// 3v3�۵�����
	GUIButton*								m_pBtn3v3UnFolder;

	// ���򴰿�
	GUIButton*								m_pBtnRuleDescOK;
	GUIButton*								m_pBtnRuleDescCancel;

	// ����ȷ�ϴ���
	GUIButton*								m_pBtnInviteOK;
	GUIButton*								m_pBtnInviteCancel;

	// ���뾺��������ʱ����
	IconStatic*								m_pStaticEnterTimeOut;
	
	DWORD									m_dwNPCID;

	// ���� 
	E_PvP_RoomType							m_enterHallType;
	BOOL									m_bIsEnterring;
	INT										m_iCurSelected;

	// room list
	RoomListItemCtrl*						m_pRoomCtrls[MAX_PVP_ROOM];
	INT										m_iCurPage;
	E_PvP_RoomType							m_curHall;
	E_PvP_RoomLevel							m_curLevel;

	// create room
	BOOL									m_bIsCreatingRoom;

	// join room
	BOOL									m_bIsJoinRoom;
	INT16									m_nJoinRoomID;

	// inside room
	INT16									m_nRoomID;

	// 3v3
	BYTE									m_uc3v3CurSelect;
	BOOL									m_bPreparing;
	BOOL									m_bUnPreparing;
	BOOL									m_b3v3IsSendingMsg;

	BOOL									m_bPositionFixed;

	// invited
	E_PvP_RoomType							m_eInvitedRoomType;
	E_PvP_RoomLevel							m_eInvitedLevel;
	INT16									m_nInvitedRoomID;
	DWORD									m_dwInvitorID;

	// timeout
	DWORD									m_dwPrepareTimeOut;
	DWORD									m_dwEnterPvPTimeOut;

	DWORD									m_dwLastInvite;
};
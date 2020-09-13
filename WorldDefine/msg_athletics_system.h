//-----------------------------------------------------------------------------
//!\file msg_athletics_system.h
//!
//!\date 2010-02-25
//!
//!\brief �ͻ����������֮����ھ���ϵͳ����Ϣ����
//-----------------------------------------------------------------------------


#pragma once


enum E_PvP_RoomState;
enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
enum E_PvP_RoleState;
struct tagPvPRoleInfo;
struct tagPvPRoom;
struct tagPvPSimRoom;

#include "RoleDefine.h"

#pragma pack(push, 1)

const  DWORD  MAX_LEVEL_LIMIT  = 40;     //����������͵ȼ�
struct tagRanPvPInfo
{
	INT			nPos;					//����������е�λ��
	INT			nPlayerLevel;			//Զ����ҵȼ�
	EClassType	ePlayerProfession;		//Զ�����ְҵ
	DWORD		dwPlayerID;				//���ID
	TCHAR		szName[X_SHORT_NAME];	//�������
};

enum EPvPResult
{
	EPR_Win,	//ʤ��
	EPR_Prefect,//��ʤ
	EPR_Lose,	//ʧ��
	EPR_Tie,	//ƽ��
	EPR_Leave,	//�뿪
};

enum EPvPPlayerState
{
	EPPS_Leave = 1,	// �뿪
	EPPS_Dead,		// ����
	EPPS_Alive,		//	����
};
struct tagRanPvPResultInfo
{
	DWORD		dwPlayerID;		//���ID
	INT			nGloryChange;	//�������ֵ�仯
	INT			nKillNum;		//ɱ����
	float		fSuccRate;		//ʤ��
	EPvPResult	eResult;		//���
};
//�μӾ����Ĵ�����
enum EPvPError
{
	E_PvP_Success			= 0,
	E_PvP_Unknown			= 1,	// δ֪����
	E_PvP_TooFar			= 2,	// NPC����̫Զ
	E_PvP_InvalidLevel		= 3,	// �ȼ�������
	E_PvP_NoEnoughWuxin		= 4,	// ��ѫ����
	E_PvP_NotInRoom			= 5,	// ��û�н��뷿��
	E_PvP_PosIsNotEmpty		= 6,	// ��λ���Ѿ�����	
	E_PvP_AlreadyInRoom		= 7,	// �Ѿ���һ�������� ����������ʱ�� 
	E_PvP_NoRoom			= 8,	// ����ķ��䲻����
	E_PvP_CannotKick		= 9,	// �޷�����
	E_PvP_RoomFighting		= 10,	// �޷����뿪��ķ���
	E_PvP_NotInHall			= 11,	// ��δ�������
	E_PvP_RoomIsFull		= 12,	// ��������
	E_PvP_CreateRoomFail	= 13,	// ��������ʧ��
	E_PvP_LevelLimitError	= 14,	// �޷�����������ȼ�������
	E_PvP_WuxunLimitError	= 15,	// �޷�������������ѫ������
	E_PvP_LeaveRoomFail		= 16,	// �뿪ʧ��
	E_PvP_RoleInReadyState	= 17,	// ����״̬���޷��뿪
	E_PvP_RoomInFighting	= 18,	// �Ѿ������޷��˳�
	E_PvP_ChangePosFail		= 19,	// �ı�λ��ʧ��
	E_PvP_PosIsSolid		= 20,	// λ�����Ѿ������
	E_PvP_PrepareFail		= 21,	// ׼��ʧ��
	E_PvP_KickPlayerFail	= 22,	// ����ʧ��
	E_PvP_StartFail			= 23,	// ��սʧ��
	E_PvP_CannotStart		= 24,	// ���ܿ�ս	
	E_PvP_NeedMorePlayer	= 25,   // �������㣬�޷���ս
	E_PvP_PlayerNotReady	= 26,	// ����û��׼���ã��޷���ս
	E_PvP_UnOpened          = 27,   // ������δ����״̬
	E_PvP_InviteNotFriend   = 28,   // ������δ����״̬
	E_PvP_InviteFull		= 29,   // ������δ����״̬
	E_PvP_InviteRefused		= 30,	// �ܾ�����
	E_PvP_InviteCannotJoin	= 31,	// �޷�����
	E_PvP_InviteNotOnLine   = 32,   // ������
	E_PvP_InviteInvaild     =  33,  // ������Ч
	E_PvP_FrindLevelLimitError  =  34,  // ������ҵȼ�������
	E_PvP_FrindWuXunLimitError  = 35 ,  // ���������ѫ�ȼ�������
        E_PvP_CanNotJoinRoom       = 36,    //�޷����뷿��
	E_PvP_End,
};

//-----------------------------------------------------------------------------
//	�������
//-----------------------------------------------------------------------------
CMD_START(NC_PvPEnterHall)
	E_PvP_RoomType eRoomType ;  //��������	
CMD_END;

CMD_START(NS_PvPEnterHall)
	DWORD dwErrorCode; 	
CMD_END;

//-----------------------------------------------------------------------------
//	�뿪����
//-----------------------------------------------------------------------------
CMD_START(NC_PvPLeaveHall)
CMD_END;

CMD_START(NS_PvPLeaveHall)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	��ȡ�����б�
//-----------------------------------------------------------------------------
CMD_START(NC_PvPGetRoomList)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
CMD_END;

CMD_START(NS_PvPGetRoomList)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
	DWORD dwCount;
	INT16  pData[1];
CMD_END;

//-----------------------------------------------------------------------------
//	��ȡ���������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_PvPGetSimRoomInfo)
	E_PvP_RoomLevel eRoomLevel;	//����ȼ�	
	DWORD			dwCount;
	INT16	pData[1];
CMD_END;

CMD_START(NS_PvPGetSimRoomInfo)
	DWORD	dwErrorCode;
	DWORD	dwCount;
	tagPvPSimRoom	pData[1];
CMD_END;

//-----------------------------------------------------------------------------
//	��ȡ������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_PvPGetRoomInfo)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
	DWORD dwCount;
	INT16	pData[1];
CMD_END;

CMD_START(NS_PvPGetRoomInfo)
	DWORD	dwErrorCode;
	DWORD dwCount;
	tagPvPRoom	pData[1];
CMD_END;


//-----------------------------------------------------------------------------
//	�μӾ���
//-----------------------------------------------------------------------------
CMD_START(NC_PvPJoinRoom)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
	INT16	nRoomID; 
CMD_END;

CMD_START(NS_PvPJoinRoom)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
	INT16	nRoomID; 
	DWORD	dwLocalPlayerRankLevel;		//������ҵ� ��������
	float	fLocalPlayerSuccessRate;	//������ҵ� ��ʤ�ʡ�
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	�μӾ���(�Զ�Ѱ�ҷ���)
//-----------------------------------------------------------------------------
CMD_START(NC_PvPAutoJoinRoom)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
CMD_END;

//-----------------------------------------------------------------------------
//	��������
//-----------------------------------------------------------------------------
CMD_START(NC_PvPCreateRoom)
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
	INT		iLevelUpperLimit;		//����ȼ�����
	INT		iLevelLowerLimit;		//����ȼ����� 
	DWORD	dwWuXunLimit;			//��ѫ����
CMD_END;

CMD_START(NS_PvPCreateRoom)
	DWORD	dwErrorCode; 
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�	
	INT16	nRoomID;
CMD_END;

//-----------------------------------------------------------------------------
//	�뿪����
//-----------------------------------------------------------------------------
CMD_START(NC_PvPLeaveRoom)
CMD_END;

CMD_START(NS_PvPLeaveRoom)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	��λ��
//-----------------------------------------------------------------------------
CMD_START(NC_PvPChangePosition)
	BYTE	ucNewPos;
CMD_END;

CMD_START(NS_PvPChangePosition)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	׼��
//-----------------------------------------------------------------------------
CMD_START(NC_PvPPrepare)
CMD_END;

CMD_START(NS_PvPPrepare)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	ȡ��׼��
//-----------------------------------------------------------------------------
CMD_START(NC_PvPUnPrepare)
CMD_END;

CMD_START(NS_PvPUnPrepare)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	����
//-----------------------------------------------------------------------------
CMD_START(NC_PvPKickPlayer)
	DWORD dwPlayerID;
CMD_END;

CMD_START(NS_PvPKickPlayer)
	DWORD	dwErrorCode; 
CMD_END;

CMD_START(NS_PvPKicked)
	DWORD dwManagerID;
CMD_END;

//-----------------------------------------------------------------------------
//	����(A����)
//-----------------------------------------------------------------------------
CMD_START(NC_PvPInvite)
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�
	INT16 nRoomID;
	DWORD dwFriendID;
CMD_END;

CMD_START(NS_PvPInvite)
	DWORD dwErrorCode;
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�
	INT16 nRoomID;
	DWORD dwFriendID;
CMD_END;

//-----------------------------------------------------------------------------
//	���루����������B��
//-----------------------------------------------------------------------------
CMD_START(NS_PvPInviteJoin)
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�
	INT16 nRoomID;
	DWORD dwInvitorID;
CMD_END;

CMD_START(NC_PvPInviteJoin)
	BOOL	bIsAccpeted;		// �Ƿ����
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//����ȼ�
	INT16 nRoomID;
	DWORD dwInvitorID;
CMD_END;

//-----------------------------------------------------------------------------
//	��ս
//-----------------------------------------------------------------------------
CMD_START(NC_PvPStart)
CMD_END;

CMD_START(NS_PvPStart)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	֪ͨ�������пͻ��˿�ս
//-----------------------------------------------------------------------------
CMD_START(NS_PvPStarted)
CMD_END;

//-----------------------------------------------------------------------------
//	֪ͨ�������пͻ��˿�ʼ����
//-----------------------------------------------------------------------------
CMD_START(NS_PvPEnterPvP)
CMD_END;

//-----------------------------------------------------------------------------
//	���¾���������״̬
//-----------------------------------------------------------------------------
CMD_START(NS_PvPGetPvPState)
	DWORD dwRedCount;
	DWORD dwGreenCount;
CMD_END;

//-----------------------------------------------------------------------------
//	ɱ��֪ͨ
//-----------------------------------------------------------------------------
CMD_START(NS_PvPNotifyKill)
	DWORD dwKillCount;
CMD_END;

//-----------------------------------------------------------------------------
//	֪ͨս�����
//-----------------------------------------------------------------------------
CMD_START(NS_PvPResultInfo)
	TCHAR pData[1];
CMD_END;

//-----------------------------------------------------------------------------
//	֪ͨս������뷿��
//-----------------------------------------------------------------------------
CMD_START(NS_PvPSetIntoRoom)
	E_PvP_RoomType hallType;
	E_PvP_RoomLevel eLevel;
	INT16 roomID;
CMD_END;

//  [10/18/2010 zhangsu] ���PVP���� ʥħ֮Ѫ�¼�------------------------------
//-----------------------------------------------------------------------------
//	֪ͨս������뷿��
//-----------------------------------------------------------------------------
CMD_START(NS_PvPRanRoomInfo)
	BYTE	bCount;						//��ǰ���������
	tagRanPvPInfo pInfo[1];				//�ͷ��еľͿ����ˣ���Ϊ������λ����Ϣ
CMD_END;

//-----------------------------------------------------------------------------
//	���䵥�������Ϣ������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NS_PvPRanRoomInfoSingleChange)
	bool	bIsJoin;					//�Ƿ��Ǽ��뷿�䣬������뿪�����򷢣�false��
	tagRanPvPInfo pInfo;				
CMD_END;

//-----------------------------------------------------------------------------
//	PVP���
//-----------------------------------------------------------------------------
CMD_START(NS_PvPRanRoomResultInfo)
	INT					nTodayNum;		//������˶��ٳ���
	tagRanPvPResultInfo	pInfo[1];		// 6 ���˵���Ϣ������������ҵ�  �ǵã���6����������
CMD_END;

//-----------------------------------------------------------------------------
//	PVP�������״̬�仯 �����볡����ͼ���뿪������
//-----------------------------------------------------------------------------
CMD_START(NS_PvPSinglePlayerInfo)
	DWORD				dwPlayerID;
	EPvPPlayerState		eState;
CMD_END;

//-----------------------------------------------------------------------------
//	�л�һ���������
//-----------------------------------------------------------------------------
CMD_START(NS_PvPChangeRandomRoom)
CMD_END;

#pragma pack(pop)

//end msg_athletics_system.h
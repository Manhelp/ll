//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_room.h
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: ����������
//-----------------------------------------------------------------------------


#pragma once
#include "world.h"


struct tagPvPRoom;
struct tagPvPRoleInfo;
struct tagPvPRoomPosState;
const INT REMAIN_TIME      =  60*5 ; 
const INT ROOM_REMAIN_TIME =  6*5;
const INT ROOM_REMAIN_TIME_START = 8*5;

class  Role;


 class RoomBase
 {
 public:
	RoomBase() {}
	virtual ~RoomBase() {}
	virtual BOOL Init( const tagPvPRoom& pvpRoom,DWORD  dwSerialID)=0;
	virtual VOID Destroy()=0;
	virtual VOID Update()=0;

	virtual BYTE GetRoomPersonNumber()const = 0;

	virtual E_PvP_RoomState GetRoomState()const = 0;

    virtual VOID  SetRoomState(E_PvP_RoomState eRoomState) = 0;

	virtual E_PvP_RoomType GetRoomType()const = 0;

    virtual DWORD  GetRoomSerialID()const = 0;

	virtual E_PvP_RoomLevel  GetRoomLevel()const = 0;

	virtual DWORD   GetRoomID()const = 0;

	virtual BOOL  GetIsPvPRoleEnterPvP()const=0;
	virtual VOID  SetIsPvPRoleEnterPvP(BOOL bIsEnterPvP) = 0;
    
	virtual DWORD GetRemainTime()= 0;
	virtual DWORD GetRemainTimeStart() = 0 ;
	virtual VOID  RunRemainTime() = 0;
	virtual VOID  RunRemainTimeStart() = 0;

	virtual VOID  SetRemainTime(DWORD dwRemainTime) = 0;
	virtual VOID  SetRemainTimeStart(DWORD dwRemainTimeStart) = 0 ;

 };


//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------

class Athletics3V3Room : public RoomBase
{
public:

	//---------------------------------------------------------------------
	// ���캯��
	//---------------------------------------------------------------------
	Athletics3V3Room();

	//---------------------------------------------------------------------
	// ��������
	//---------------------------------------------------------------------
	~Athletics3V3Room();
    
	//---------------------------------------------------------------------
	// ��ʼ��
 	//---------------------------------------------------------------------
    virtual BOOL Init( const tagPvPRoom& pvpRoom,DWORD  dwSerialID);
 	virtual VOID Destroy();
 	virtual VOID Update();

	//---------------------------------------------------------------------
	// ��ɫ����,��ɫ�뿪
	//---------------------------------------------------------------------
 	BOOL AddRole(DWORD dwRoleID);
 	BOOL LeaveRole(DWORD dwRoleID);

	BOOL AddRoleFromPvP(const tagPvPRoleInfo* pRoleInfo,INT16 nRoleNum);
    BOOL DeleteRoleFromPvP(DWORD dwRoleID1,DWORD dwRoleID2,DWORD dwRoleID3,DWORD dwRoleID4,DWORD dwRoleID5,DWORD dwRoleID6);
	BOOL Start();
	//---------------------------------------------------------------------
	// �߽�ɫ
	//---------------------------------------------------------------------
	BOOL KickRole(DWORD dwRoleID,BOOL bIsSend = TRUE);
    
	//---------------------------------------------------------------------
	// �ı��ɫ��λ��
	//---------------------------------------------------------------------
	BOOL ChangePos(DWORD dwRoleID,INT nNewPos); 

	//---------------------------------------------------------------------
	// ���úͻ�ý�ɫ��״̬
	//---------------------------------------------------------------------
	BOOL SetRoleState(DWORD dwRoleID,E_PvP_RoleState eRoleState);
	E_PvP_RoleState GetRoleState(DWORD dwRoleID);
    VOID SendRoomInfoToEveryRole();

	//---------------------------------------------------------------------
	//�򾺼�������Ľ�ɫ�㲥Զ�����ԣ���Ѫ������
	//---------------------------------------------------------------------
	VOID  SendRemoteAttToPvPRole(DWORD dwRoleID,BOOL bIsInit = FALSE);

 	E_Role_BothSide GetRoleInSide(DWORD dwRoleID); 
    E_Role_InPvP    GetInPvP(DWORD dwRoleID);

	 E_Role_BothSide  GetPvPSide(INT nPos)
	 {
		 if( nPos < 1 || nPos > P3v3ROOM_TOTAL_PERSON_NUM )
		 {	
			 return ERBS_NULL;
		 }
		 if( (nPos + 1) % 2 == 0)
		 {
			 return ERBS_DefenseSide;
		 }
		 else
		 {
            return ERBS_Attack;
		 }
	 }


	//����������1��ʼ
	//-----------------------------------------------------------------------
	//�跿��λ�õ�״̬
	//-----------------------------------------------------------------------
    VOID SetPosState(INT nPos, E_Room_Pos_State eRoomPosState)
	{
		if ( nPos  <=0 || nPos > P3v3ROOM_TOTAL_PERSON_NUM)
		{
			ILOG->Write(_T("Athletics exception: Athletics3V3Room::SetPosState. nPos =%d ,eRoomPosState =%d"),nPos,(INT)eRoomPosState);
			return ;
		}
        m_PvPRoomPosState[nPos-1].eRoomState = eRoomPosState;
	}
    E_Room_Pos_State GetPosState(INT nPos)const
	{
		if ( nPos  <=0 || nPos > P3v3ROOM_TOTAL_PERSON_NUM)
		{
			ILOG->Write(_T("Athletics exception: Athletics3V3Room::GetPosState. nPos =%d "),nPos);
			return ERPS_Empty;
		}
        return m_PvPRoomPosState[nPos-1].eRoomState; 
	}
    
	//---------------------------------------------------------------------
	// ���úͻ�÷�������ɫ��id
	//---------------------------------------------------------------------
    DWORD GetMasterRoleID()const
	{ 
        return m_PvPRoom.dwManagerID;
	}

	VOID SetMaster(INT nPos,DWORD dwRoleID)
	{
        m_PvPRoom.dwManagerID = dwRoleID ;

	}

    
	//---------------------------------------------------------------------
	// ��÷�������
	//---------------------------------------------------------------------
	BYTE GetRoomPersonNumber()const   //��ȡ��������
	{
		return  m_PvPRoom.ucCurrentPersonNum;
	}
	
	//---------------------------------------------------------------------
	// ��ý�ɫ��λ��
	//---------------------------------------------------------------------
	INT GetRolePos(DWORD dwRoleID);

	//---------------------------------------------------------------------
	// ���úͻ��λ�õĽ�ɫ��Ϣ
	//---------------------------------------------------------------------
	VOID SetPosRoleInfo(INT nPos,const tagPvPRoleInfo& tagRoleInfo)
	{
		if ( nPos  <=0 || nPos > P3v3ROOM_TOTAL_PERSON_NUM)
		{
			ILOG->Write(_T("Athletics exception: Athletics3V3Room::SetPosRoleInfo. nPos =%d,dwRoleID = %u/r/n"),tagRoleInfo.ucPos,tagRoleInfo.dwRoleID);
			return;
		}
		m_PvPRoleInfo[nPos-1].dwRoleID      = tagRoleInfo.dwRoleID;
		m_PvPRoleInfo[nPos-1].eState        = tagRoleInfo.eState;
		m_PvPRoleInfo[nPos-1].ucPos         = tagRoleInfo.ucPos;
		E_Role_BothSide eSide =  GetPvPSide(m_PvPRoleInfo[nPos-1].ucPos);
		m_PvPRoleInfo[nPos-1].eRoleInSide   = eSide;
		m_PvPRoleInfo[nPos-1].eRoleInPvP    = tagRoleInfo.eRoleInPvP;
		_tcscpy_s(m_PvPRoleInfo[nPos-1].szName,X_SHORT_NAME, tagRoleInfo.szName);
        m_PvPRoleInfo[nPos-1].dwRoleLevel   = tagRoleInfo.dwRoleLevel; 
        m_PvPRoleInfo[nPos-1].dwWuXun       = tagRoleInfo.dwWuXun;
        m_PvPRoleInfo[nPos-1].ePlayerProfession       = tagRoleInfo.ePlayerProfession;  
	}
    
    tagPvPRoleInfo& GetPosRoleInfo(INT nPos)
	{
		if ( nPos  <=0 || nPos > P3v3ROOM_TOTAL_PERSON_NUM)
		{
			ILOG->Write(_T("Athletics exception: Athletics3V3Room::GetPosRoleInfo. nPos =%d/r/n "),nPos);
            static tagPvPRoleInfo temp;
			memset(&temp,0,sizeof(temp));
			return temp;
		}
        return m_PvPRoleInfo[nPos-1];
	}
	
	VOID  ClearPosRoleInfo(INT nPos)
	{
		if ( nPos  <=0 || nPos > P3v3ROOM_TOTAL_PERSON_NUM)
		{
			ILOG->Write(_T("Athletics exception: Athletics3V3Room::GetPosRoleInfo. nPos =%d /r/n"),nPos);
			return;
		}
		tagPvPRoleInfo& roleInfo = GetPosRoleInfo( nPos );
		roleInfo.dwRoleID    = 0;
		roleInfo.ucPos       = 0;
		roleInfo.eState      = E_PvP_RoleStateNull;
		roleInfo.eRoleInPvP  = ERIP_NULL ;
		roleInfo.eRoleInSide = ERBS_NULL;
		roleInfo.dwRoleLevel = 0;
		roleInfo.ePlayerProfession = EV_Null;
		roleInfo.dwWuXun     = 0;
		memset(roleInfo.szName,0,X_SHORT_NAME);
	}
	//---------------------------------------------------------------------
	// ��ú����÷���״̬
	//---------------------------------------------------------------------
	E_PvP_RoomState GetRoomState()const
	{
		return  m_PvPRoom.eRoomState ;
	}
	
    VOID  SetRoomState(E_PvP_RoomState eRoomState)
	{
		 m_PvPRoom.eRoomState = eRoomState;
	}
    

	//---------------------------------------------------------------------
	//���÷�������
	//---------------------------------------------------------------------
	E_PvP_RoomType GetRoomType()const
	{
		return m_PvPRoom.eRoomType;
	}
   
	//---------------------------------------------------------------------
	// ��÷������к�
	//---------------------------------------------------------------------
	DWORD  GetRoomSerialID()const
	{
		return m_dwSerialID;
	}

	//---------------------------------------------------------------------
	// ��÷���ĵȼ�
	//---------------------------------------------------------------------
	E_PvP_RoomLevel  GetRoomLevel()const
	{
		return  m_PvPRoom.eLevel;
	}
    
	DWORD  GetRoomValiantMeritLevel()const
	{
		return m_PvPRoom.dwWuXunLimit;
	}
	//---------------------------------------------------------------------
	// ��÷���ID�� 
	//---------------------------------------------------------------------
	DWORD   GetRoomID()const
	{
		return  m_PvPRoom.nRoomID;
	}

	//---------------------------------------------------------------------
	// ��÷��������� 
	//---------------------------------------------------------------------
	TCHAR* GetMasterName()
	{
		return m_PvPRoom.szManagerName;
	}
    
	//---------------------------------------------------------------------
	// ��÷���Ľ�ɫ�������޺����� 
	//---------------------------------------------------------------------
	INT GetRoomUpperRoleLevel()const
	{
        return m_PvPRoom.iLevelUpperLimit; 
	}

	INT GetRoomLowerRoleLevel()const
	{
		return m_PvPRoom.iLevelLowerLimit;
	}
   
	//---------------------------------------------------------------------
	// ��÷�����ѫ�ȼ�
	//---------------------------------------------------------------------
	DWORD GetValiantLevel()const
	{
		return m_PvPRoom.dwWuXunLimit;
	}
    
	//---------------------------------------------------------------------
	// ��÷���Ľ�ɫ�Ƿ��Ѿ����뾺����
	//---------------------------------------------------------------------
	BOOL  GetIsPvPRoleEnterPvP()const
	{
		return m_bIsEnterPvP;
	}

	//---------------------------------------------------------------------
	// ���÷���Ľ�ɫ�����뾺������״̬
	//---------------------------------------------------------------------
	VOID  SetIsPvPRoleEnterPvP(BOOL bIsEnterPvP)
	{
           m_bIsEnterPvP = bIsEnterPvP;
	}

	DWORD  GetRemainTime()   
	{
		return m_dwRemainTime;
	}
	
	VOID  SetRemainTime(DWORD dwRemainTime)
	{
		 m_dwRemainTime = dwRemainTime;
	}

	
	

	DWORD GetRemainTimeStart()
	{
		return m_dwRemainTimeStart;
	}
    
	VOID SetRemainTimeStart(DWORD dwRemainTimeStart)
	{
		m_dwRemainTimeStart = dwRemainTimeStart;
	}
    
	//����ʱ5��
	VOID RunRemainTime();
	VOID RunRemainTimeStart();
	VOID RunRemainTimeKickRole();
    
	VOID  UpdateRoomState();
   
private:
	tagPvPRoleInfo               m_PvPRoleInfo[P3v3ROOM_TOTAL_PERSON_NUM];               //ÿ��λ�õĽ�ɫ����Ϣ
	DWORD						 m_PvPRoleRemainTime[P3v3ROOM_TOTAL_PERSON_NUM];
    tagPvPRoomPosState           m_PvPRoomPosState[P3v3ROOM_TOTAL_PERSON_NUM];           //ÿ��λ�õ�״̬
	tagPvPRoom                   m_PvPRoom;                                              //������Ϣ
	DWORD                        m_dwSerialID;                                           //��������к�Ωһ   

	BOOL                         m_bIsEnterPvP;                                          //�����ɫ�Ƿ��Ѿ�����pvp ������
    DWORD                        m_dwRemainTime;                                         //����ʣ��ʱ�䵹��ʱ 
    DWORD                        m_dwRemainTimeStart;
    
	
    
     
   
};

class Athletics6V6Room
{
     
};
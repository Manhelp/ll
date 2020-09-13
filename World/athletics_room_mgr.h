//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_room_mgr.h
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: ���������������
//-----------------------------------------------------------------------------

#pragma once

struct tagPvPRoom;
struct tagAthRoomId;
class  Mutex;

//-----------------------------------------------------------------------------
// ������ڵķ��� 
//-----------------------------------------------------------------------------
struct tagPvPRoleInRoom
{
	DWORD      dwRoleID;
    DWORD      dwSerialID;

};

//-----------------------------------------------------------------------------
// ���������������
//-----------------------------------------------------------------------------
class AthleticsRoomMgr
{

public:
	//-------------------------------------------------------------------------
	//Constructor &  Destructor
	//-------------------------------------------------------------------------
	AthleticsRoomMgr();
    ~AthleticsRoomMgr();
    
	//-------------------------------------------------------------------------
	//Initialization function
	//-------------------------------------------------------------------------
	BOOL Init();
    BOOL Destroy();
	VOID Update();

	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���Ӹò��ֹ��� 
	//--------------------------------------------------------------------------
	DWORD AddToHall(E_PvP_RoomType eRoomType,DWORD dwRoleID);
	DWORD LeaveHall(DWORD dwRoleID);

	BOOL  IsRoleExist(DWORD dwRoleID);
	//--------------------------------------------------------------------------
	//����
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	//SM ������������޸�
	//���Ӹò��ֹ��� 
	//--------------------------------------------------------------------------
	//�����������
	VOID AthleticsActiveOver();

	//---------------------------------------------------------------------------------------
	//���������ɾ������
	//---------------------------------------------------------------------------------------
    DWORD CreateAthleticsRoom(const tagPvPRoom& pvpRoom);
	BOOL  DeleteAthleticsRoom(DWORD dwSerialID,RoomBase* pRoom);
    
	//---------------------------------------------------------------------------------------
	//�������������б��ɾ�����������б�
	//---------------------------------------------------------------------------------------
	
	BOOL AddListRoom(const tagPvPRoom& pvpRoom,DWORD dwSerialId);
    BOOL DeleteListRoom(E_PvP_RoomType eRoomType,E_PvP_RoomLevel eRoomLevel,INT16 nRoomId); 
     

	//---------------------------------------------------------------------------------------
	//����ܵķ������
	//---------------------------------------------------------------------------------------
	DWORD GetTotalRoomNumber()const
	{
       return m_dwTotalRoomNum;
	}
    
	//---------------------------------------------------------------------------------------
	//����ܵĴ�������
	//---------------------------------------------------------------------------------------
    INT GetTotalPersonNumber()
	{
		return m_MapRoleInAthletisc.Size();
	}

	//---------------------------------------------------------------------------------------
	//��ó����������
	//---------------------------------------------------------------------------------------
	DWORD GetTotalJuniorRoomNumber()const
	{
		return m_dwTotalJuniorRoomNum;
	}

	//---------------------------------------------------------------------------------------
	//����м��������
	//---------------------------------------------------------------------------------------
	DWORD GetTotalInterMediateRoomNumber()const
	{
		return m_dwTotalInterMediateRoomNum;
	}
    
	

	//---------------------------------------------------------------------------------------
	//��ø߼��ķ������
	//---------------------------------------------------------------------------------------
	DWORD GetTotalSeniorRoomNumber()const
	{
		return m_dwTotalSeniorRoomNum;
	}

	//---------------------------------------------------------------------------------------
	//�����Ч�ķ����
    //---------------------------------------------------------------------------------------
    DWORD GetAvailRoomId(E_PvP_RoomLevel eRoomLevel);
	
	//---------------------------------------------------------------------------------------
	//���÷���ŵ�״̬
    //---------------------------------------------------------------------------------------
	VOID  SetRoomIdState(E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId,E_RoomId_State eRoomIdState);
	
	//---------------------------------------------------------------------------------------
	//��÷���ŵ�״̬
	//---------------------------------------------------------------------------------------
	tagAthRoomId*  GetRoomIdState(E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId);

	//---------------------------------------------------------------------------------------
	//��÷����ָ��
	//---------------------------------------------------------------------------------------
    RoomBase*     GetPvPRoom(DWORD dwSerialID);
    RoomBase*     GetPvPRoom(DWORD dwRoleID ,E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId); 
	//---------------------------------------------------------------------------------------
	//�Ѵ����ķ�����Ϣ�����ͻ���
	//---------------------------------------------------------------------------------------
	VOID SendHallRoomInfoToClient();

	//---------------------------------------------------------------------------------------
	//�ѷ���ľ�����Ϣ�����ͻ���;
	//---------------------------------------------------------------------------------------
	VOID    SendRoomInfoToClient();
    DWORD   RoleJoinInRoom(DWORD dwRoleID,E_PvP_RoomLevel eRoomLevel,INT16 nRoomID);
	DWORD   KickRoleToHall(DWORD dwRoleID,BOOL bSendMsg = TRUE);
	DWORD   RoleLeaveRoom(DWORD dwRoleID);
    tagPvPRoleInRoom* GetInRoomRole(DWORD dwRoleID);
	VOID    AddRoleToAthletics(const tagPvPRoleInRoom* pRoleInRoom);
	BOOL    DeleteRoleFromAthletics(DWORD dwRoleID);

	VOID    GetHallRoomId(INT16 nRoomNum, E_PvP_RoomLevel eRoomLevel,/*���*/INT16 nRoomID[]);
	tagPvPRoom* GetHallRoomInfo(INT16 nRoomID, E_PvP_RoomLevel eRoomLevel);
	VOID    GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel,/*���*/tagPvPRoom& pvpRoom);
	VOID    GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel,/*���*/PBYTE& pData);
    VOID    OnOffLine(DWORD dwRoleID,BOOL bIsSend = TRUE);
    VOID    AddPvPRoomToPvPQueue(RoomBase* pRoom);
    BOOL    AddRoleToRoom(DWORD dwSerialId,DWORD dwRoleID1,DWORD dwRoleID2,DWORD dwRoleID3,DWORD dwRoleID4,DWORD dwRoleID5,DWORD dwRoleID6);
    TMap<INT16, tagPvPRoom*>&   GetMapPvPRoomInfo(E_PvP_RoomLevel eRoomLevel);


private:
	//---------------------------------------------------------------------------------------------
	//�����ķ�����Ϣ�б�
	//---------------------------------------------------------------------------------------------
	typedef TMap<INT16, tagPvPRoom*>	        TMapJuniorAthleticsRoom;       //���������
	typedef TMap<INT16, tagPvPRoom*>	        TMapInterMediateAthleticsRoom; //�м������
	typedef TMap<INT16, tagPvPRoom*>	        TMapSeniorAthleticsRoom;       //�߼������


	//---------------------------------------------------------------------------------------------
	//��Ǹ������𷿼���б�
	//---------------------------------------------------------------------------------------------
	typedef TList<tagAthRoomId*>                ListJuniorAthRoomId;             //��ǳ��������Ƿ񱻴���
	typedef TList<tagAthRoomId*>                ListInterMediateAthRoomId;       //����м������Ƿ񱻴���
	typedef TList<tagAthRoomId*>                ListSeniorAthRoomId;             //��Ǹ߼������Ƿ񱻴���

	//---------------------------------------------------------------------------------------------
	//�����б�
	//---------------------------------------------------------------------------------------------     
    typedef TMap<DWORD, RoomBase*>	            MapAthleticsRoom;             //�ܵĸ����б�
	typedef TList<RoomBase*>                    ListEnterPvPRoom;             //���뾺�����ķ������   //�����ͷŷ����ڴ�

	//---------------------------------------------------------------------------------------------
	//������� ��ɫ�б�
	//---------------------------------------------------------------------------------------------    
    typedef TMap<DWORD, tagPvPRoleInRoom*>	                MapRoleInAthletisc;           //���뾺���Ľ�ɫ�б�
public:
	   //---------------------------------------------------------------------------------------
	   //��ý��뾺�����ķ������
	   //---------------------------------------------------------------------------------------
	   ListEnterPvPRoom& GetEnterPvPRoomDequeue()
	   {
		   return m_ListEnterPvPRoom;
	   }
private:
	//Mutex                                       m_Mutex;
	
	//---------------------------------------------------------------------------------------------
	//�����ķ�����Ϣ�б�
	//---------------------------------------------------------------------------------------------
	TMapJuniorAthleticsRoom                     m_MapJuniorAthleticsRoom;
    TMapInterMediateAthleticsRoom               m_MapInterMediateAthleticsRoom;
    TMapSeniorAthleticsRoom                     m_MapSeniorAthleticsRoom;
    
	//---------------------------------------------------------------------------------------------
	//��Ǹ������𷿼���б�
	//---------------------------------------------------------------------------------------------
    ListJuniorAthRoomId                         m_ListJuniorAthRoomId;
    ListInterMediateAthRoomId                   m_ListInterMediateAthRoomId;
    ListSeniorAthRoomId                         m_ListSeniorAthRoomId;   
	
	//---------------------------------------------------------------------------------------------
	//�����б�
	//---------------------------------------------------------------------------------------------
	MapRoleInAthletisc                          m_MapRoleInAthletisc;   
    MapAthleticsRoom                            m_MapAthleticsRoom; 
    ListEnterPvPRoom                            m_ListEnterPvPRoom;

	DWORD                                       m_dwTotalRoomNum;
	DWORD                                       m_dwTotalJuniorRoomNum;
	DWORD                                       m_dwTotalInterMediateRoomNum;
	DWORD                                       m_dwTotalSeniorRoomNum;
	static    DWORD                             m_sdwRoomSerialID;           
};
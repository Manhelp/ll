//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_pvp.h
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: ������
//-----------------------------------------------------------------------------

#pragma once

struct tagAthleticsPvPRoomInfo;
struct tagPvPRoomRoleInfo;
class RoomBase;
class Athletics3V3Room;

const DWORD  PVP_LEFT_TIME  =  60*10 ;      // ����ʱ�� 10����
const DWORD  PVP_COUNT_DOWNTIME = 15 ;      // ����ʱ15��
class AthleticsPvP 
{
public:
   AthleticsPvP();
   ~AthleticsPvP();

   BOOL Init(DWORD dwSerialId,DWORD dwInstanceID,const RoomBase* pRoom);
   VOID Update();
   VOID Destroy();

   DWORD GetMapID()const                 { return  m_dwMapID;     }
   DWORD GetInstanceID()const            { return  m_dwInstanceID;}
   
   DWORD GetRoomSerialID()const          { return  m_dwSerialId;}
   VOID  ClosePvP(BOOL bIsClosePvP)      { m_bIsClosePvP = bIsClosePvP; }
   BOOL  GetIsClosePvP()const            { return  m_bIsClosePvP; }
   

   DWORD GetPvPTime()const               { return  m_dwPvPTime; }

private:
	
	DWORD                                  m_dwMapID;
	DWORD                                  m_dwInstanceID;

    DWORD                                  m_dwRedSidePersonNum;
	DWORD                                  m_dwBlueSidePersonNum;
    
    
	DWORD                                  m_dwLeftTime ;        // ʣ��ʱ��
	BOOL                                   m_bPvPOver;           //������Ϸ����
	DWORD                                  m_dwCountDown;        //����ʱ
	BOOL                                   m_bCountDown;         
	BOOL                                   m_bIsClosePvP;

	DWORD                                  m_dwPvPTime;            //������ʱ�䵥λ����
    DWORD                                  m_dwSerialId;           //��������к�  
	DWORD                                  m_dwPvPPersonNum;
};
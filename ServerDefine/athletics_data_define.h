//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_data_define.h 
// author: Lamoair
// actor:
// data: 2010-03-24
// last:
// brief:  ���������ݱ������ݿ�
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

struct tagPvPSystemInfo
{
	INT16     n16SystemId;
	DWORD     dwSerialVal;      //����id
};

struct tagPvPSystemData
{
	DWORD          dwSerialId; //���浱ǰ��idֵ
	tagDWORDTime   saveTime;
	DWORD          dwHallPersonNum;
	DWORD          dwHallRoomNum;
	DWORD          dwFightRoomNum;
	INT16          n16Below120Sec;
	INT16          n16Below210Sec;
	INT16          n16Below300Sec;
	INT16          n16Over300Sec;
};

//-------------------------------------------------------------------------
// ���澺���������ݵ����ݿ�
//-------------------------------------------------------------------------
CMD_START(NDBC_SaveArenaDataToDB)	// 
   tagPvPSystemInfo   tPvPSystemInfo;
   tagPvPSystemData   tPvPSystemData;
CMD_END;


//-------------------------------------------------------------------------
//���������� Ҫ�����ݿ��ȡ���ֵ
//-------------------------------------------------------------------------
CMD_START(NDBC_GetSerialVal)	
    DWORD   dwSerialId;
CMD_END;

//-------------------------------------------------------------------------
//���ݿ���������������ֵ
//-------------------------------------------------------------------------
CMD_START(NDBS_GetSerialVal)   //��ȡ���ֵ 
DWORD   dwSerialId;
DWORD   dwSerialVal;
CMD_END;

//-------------------------------------------------------------------------
// �������ֵ
//-------------------------------------------------------------------------
CMD_START(NDBC_SaveSerialValToDB)	
DWORD   dwSerialId;
DWORD   dwSerialVal;
CMD_END;


#pragma pack(pop)


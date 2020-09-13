
//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_PvP_define.h
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: ����������
//-----------------------------------------------------------------------------

#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ���뾺����������ڵ�ͼ
//-----------------------------------------------------------------------------
const DWORD  ATHLETICS_ENTER_SPOT1  =  3017299663 ;  //m04��ͼ��ڵ�
const DWORD  ATHLETICS_ENTER_SPOT2  =  2092184628 ;  //a04��ͼ��ڵ�
const DWORD  ATHLETICS_ENTER_SPOT3  =  2092184884 ;  //a05��ͼ��ڵ�
const DWORD  ATHLETICS_MAPID        =  2092185140 ;  //������������ͼID
const DWORD  ATHLETICS_BUFF_ID      =  2025501;      //���������εļ��� 




//-----------------------------------------------------------------------------
// ö����ҵ�����״̬��Ϣ
//----------------------------------------------------------------------------
enum E_Role_Life_State
{
	ERLS_NULL        =  0,
	ERLS_RoleAlive   =  1,
	ERLS_RoleDead    =  2,
	ERLS_End,
};

//-----------------------------------------------------------------------------
// �������ڷ���Ļ�����Ϣ
//-----------------------------------------------------------------------------
struct tagAthleticsPvPRoomInfo
{
	INT16 nRoomId;                //�����
	BYTE  cnInPvPPersonNum;       //������������
    BYTE  cnTotalPersonNum;       //����������
	DWORD dwRoomValiantMeritLevel;//������ѫ�ȼ�
	DWORD dwRoomType;             //��������
	DWORD dwManagerId;            //����
    DWORD dwSerialId;             //�������к�
	
};

//-----------------------------------------------------------------------------
// �������ڷ���Ľ�ɫ�Ļ�����Ϣ
//-----------------------------------------------------------------------------
struct tagPvPRoomRoleInfo
{
    BYTE               cnPos ;
	DWORD              dwRoleId;
	DWORD              dwRoleInSide;
    E_Role_Life_State  eRoleLifeState;
};





#pragma pack(pop)  //end athletics_PvP_define.h
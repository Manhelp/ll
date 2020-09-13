//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_draft_show.h
// author: Lamoair
// actor:
// data: 2010-03-31
// last:
// brief:  ���������ݱ������ݿ�
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//------------------------------------------------------------------------------
// ����ѡ�����б�
//------------------------------------------------------------------------------
struct tagShowPlayer            
{
    TCHAR    tcAthleteRoleName[X_SHORT_NAME];       //ѡ�ֵ�����
	DWORD    dwAthleteRoleId;                       
	DWORD    dwAthleteSignId;                      //ѡ�ֵı�����
	DWORD    dwAthletePoint;
	DWORD    dwAthleteSerialId;                    //���к�
};


//------------------------------------------------------------------------------
// ͶƱ���б�
//------------------------------------------------------------------------------
struct  tagVotePlayer
{
	TCHAR    tcVoteRoleName[X_SHORT_NAME];
	DWORD    dwVoteRoleId;
	DWORD    dwVotePoint;
	DWORD    dwSerialId;
};


//------------------------------------------------------------------------------
//ѡ�����
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveShowPlayerData)
DWORD     dwCount;
tagShowPlayer   data[1];
CMD_END

//------------------------------------------------------------------------------
//ѡ���ߵĵ�Ʊ����
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveShowPlayerPoint)
tagShowPlayer showPlayer;
CMD_END

//------------------------------------------------------------------------------
//����ͶƱ�ĵ÷ֱ���
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveVotePlayerPoint)
tagVotePlayer   VotePlayer;
CMD_END

//------------------------------------------------------------------------------
//ѡ�����������
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveShowPlayerDataCount)
INT16     n16SignId;
DWORD     dwTotalDataCount;
CMD_END

//------------------------------------------------------------------------------
//��Ʊ��������
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveVotePlayerDataCount)
INT16     n16RoleId;
DWORD     dwTotalDataCount;
CMD_END

//------------------------------------------------------------------------------
//���ѡ���߱�
//------------------------------------------------------------------------------
CMD_START(NDBC_GetShowPlayerData)
CMD_END

//------------------------------------------------------------------------------
//��ò���ͶƱ��
//------------------------------------------------------------------------------
CMD_START(NDBC_GetVotePlayerData)
CMD_END


//------------------------------------------------------------------------------
//���ѡ���߱�
//------------------------------------------------------------------------------
CMD_START(NDBS_LoadShowPlayerData)
DWORD           dwCount;
tagShowPlayer   data[1];
CMD_END

//------------------------------------------------------------------------------
//��ò���ͶƱ��
//------------------------------------------------------------------------------
CMD_START(NDBS_LoadVotePlayerData)
DWORD           dwCount;
tagVotePlayer   data[1];
CMD_END


//------------------------------------------------------------------------------
//����ݱ����������
//------------------------------------------------------------------------------
enum E_ActiveDataType
{
	E_DataTypeNull   = 0,
    E_DataType1      = 0,
	E_DataType2      = 1,
	E_DataType3      = 2,
	E_DataType4      = 3,
    E_DataType5      = 4,
	E_DataType6      = 5,
	E_DataType7      = 6,
	E_DataType8      = 7,
	E_DataTypeEnd,
};


struct tagActiveRankData
{
	DWORD  dwRoleId;
	TCHAR  ucRoleName[X_SHORT_NAME];
    INT16  n16RoleLevel;
	INT32  nDataType[E_DataTypeEnd];
	tagActiveRankData()
	{
		memset(this,0,sizeof(tagActiveRankData));
	}

};



CMD_START(NDBC_SaveActiveRankData)
tagActiveRankData   ActiveRankData;
CMD_END


CMD_START(NDBC_GetActiveRankData)
CMD_END

CMD_START(NDBS_LoadActiveRankData)
DWORD               dwCount;
tagActiveRankData   data[1];
CMD_END

#pragma pack(pop)
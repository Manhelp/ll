//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: vip_netbar_define.h
// author: xlguo
// actor:
// data: 2009-8-3
// last:
// brief: �������ɷ���������
//-------------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)

//-------------------------------------------------------------------------
// ���ݿ�洢�˺Žṹ
//-------------------------------------------------------------------------
struct tagDBVNBPlayers
{
	INT		nHisPlayers;
	INT		nTodaysPlayers;
	DWORD	dwAccountIDs[1];
};

//-------------------------------------------------------------------------
// ����������Ʒԭ��
//-------------------------------------------------------------------------
struct tagVNBGiftProto
{
	DWORD	dwID;
	DWORD	dwItemTypeID;
	INT		nNum;
};

//-------------------------------------------------------------------------
// ��������װ��ԭ��
//-------------------------------------------------------------------------
struct tagVNBEquipProto
{
	DWORD	dwID;
	DWORD	dwEquipTypeID;
	INT		nQuality;
};

//-------------------------------------------------------------------------
// �������ɽṹ
//-------------------------------------------------------------------------
struct tagVipNetBar
{
	DWORD	dwID;
	TCHAR	szName[X_SHORT_NAME];
	INT		nPlayerNum;
	DWORD	dwBuffID;		// �������ɵ�¼�����еĳ���ʱ��8Сʱ
	DWORD	dwInstBuffID;	// ���븱�������ӵ�buff

	VOID	OnPlayerLogin(){++nPlayerNum;}
	VOID	OnPlayerLogout(){--nPlayerNum;}

};

//-------------------------------------------------------------------------
// �������ip����Ŀ
//-------------------------------------------------------------------------
const INT MAX_VNB_IP_NUM	= 5;

#pragma pack(pop)
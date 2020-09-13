//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_common.h
// author: 
// actor:
// data: 2009-03-25
// last:
// brief: �˺��½�ɫͨ�����������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ��Ϣ�ṹ
//-----------------------------------------------------------------------------

struct tagGodBidInfo2DB
{
	DWORD	dwRoleID;
	INT64		n64Bidden;
	BYTE		byBidType;
	DWORD	dwBidID;
	tagGodBidInfo2DB()
	{
		dwRoleID = GT_INVALID;
		n64Bidden = 0;
		byBidType = GT_INVALID;
		dwBidID = GT_INVALID;
	}
};

// ���ؾ�����Ϣ
CMD_START(NDBC_LoadGodBidInfo)
CMD_END

CMD_START(NDBS_LoadGodBidInfo)
	DWORD dwBidCount;
	tagGodBidInfo2DB GodBidInfo2DB[1];
CMD_END

// ����һ��������Ϣ
CMD_START(NDBC_SaveOneGodBidInfo)
	tagGodBidInfo2DB GodBidInfo2DB;
CMD_END

// ��վ�����Ϣ
CMD_START(NDBC_ClearGodBidInfo)
	BOOL bBidEnd;
CMD_END
CMD_START(NDBS_ClearGodBidInfo)
	BOOL bClearSuccess;
CMD_END

#pragma pack(pop)

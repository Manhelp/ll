//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_city.h
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: ����ϵͳ
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/city_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// �����ýṹ
//-----------------------------------------------------------------------------

// ����ָ����������
struct tagGuildClanValue
{
	DWORD				dwGuildID;
	INT32				nClanValue;
};

//-----------------------------------------------------------------------------
// �����ݿ��������������
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadCityInfo)
	DWORD				dwCityID;
CMD_END

CMD_START(NDBS_LoadCityInfo)
	tagCityAtt2DB		sCityAtt;
	INT					nSignUpNum;
	tagCitySignUp		sSignUp[1];
CMD_END

//-----------------------------------------------------------------------------
// ����������Ϣ
//-----------------------------------------------------------------------------
CMD_START(NDBC_CreateCityInfo)
	INT					nCityNum;
	DWORD				dwCityID[1];
CMD_END

//-----------------------------------------------------------------------------
// ���桢���³�����Ϣ
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveCityAtt)
	tagCityAtt2DB		sCityAtt;
CMD_END

CMD_START(NDBC_SaveAppointedAtt)
	DWORD				dwCityID;
	tagCityAppointedAtt	sCityAtt;
CMD_END

//-----------------------------------------------------------------------------
// ��ȡ����ָ����������
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetGuildClanValue)
	ECLanType			eClanType;
	INT					nGuildNum;
	DWORD				dwGuildID[1];
CMD_END

CMD_START(NDBS_GetGuildClanValue)
	ECLanType			eClanType;
	INT					nGuildNum;
	tagGuildClanValue	sClanValue[1];
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)
//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_log.h
// author: 
// actor:
// data: 2009-03-23
// last:
// brief: log���
//-----------------------------------------------------------------------------
#pragma once
#include "../World/data_collect.h"

#pragma pack(push, 1)

#include "log_cmdid_define.h"
//-----------------------------------------------------------------------------

CMD_START(NDBC_LogSilver)		// ��Ǯ�仯
	tagLogSilver	sLogSilver;
CMD_END

CMD_START(NDBC_LogYuanBao)		// Ԫ���仯
	tagLogYuanBao	sLogYuanBao;
CMD_END

CMD_START(NDBC_LogExVolume)		// ����仯
	tagLogExVolume	sLogExVolume;
CMD_END

CMD_START(NDBC_LogVIPPoint)		// ���ֱ仯
    tagLogVIPPoint	sLogVIPPoint;
CMD_END

CMD_START(NDBC_LogTimeStat)		// ��ʱͳ��
	tagLogTimeStat	sLogTimeStat;
CMD_END

CMD_START(NDBC_LogItem)			// ��Ʒlog
	tagLogItem		sLogItem;
CMD_END

CMD_START(NDBC_LogItemTimes)	// ��Ʒlog
	tagLogItemTims	sLogItemTimes;
CMD_END

CMD_START(NDBC_LogMallSell)		// �̳ǳ�����Ʒlog
	tagLogMallSell	sLogMallSell;
CMD_END

CMD_START(NDBC_LogMallSellPack)		// �̳ǳ�����Ʒ��log
	tagLogMallSellPack	sLogMallSellPack;
CMD_END

CMD_START(NDBC_LogGodBid)
	tagLogGodBid	 sLogGodBid;
CMD_END

CMD_START(NDBC_LogBattlefield)
	tagLogBattlefield	 sLogBattlefield;
CMD_END

//-----------------------------------------------------------------------------
// ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogFund)
tagLogFund			sLogFund;
CMD_END

CMD_START(NDBC_LogMaterial)
tagLogMaterial		sLogMaterial;
CMD_END

CMD_START(NDBC_LogReputation)
tagLogReputation	sLogReputation;
CMD_END

//-----------------------------------------------------------------------------
// ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogGMCmd)
	tagLogGMCmd		sLogGMCmd;
CMD_END

//-----------------------------------------------------------------------------
// ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogPet)
	tagLogPet	sLogPet;
CMD_END

//-----------------------------------------------------------------------------
// ��������LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogFairyBirth)
	tagLogFairyBirth	sLogFairyBirth;
CMD_END

//-----------------------------------------------------------------------------
// ��ɫ��½LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogRoleOnlineInfo)
	tagLogRoleOnline	sLogRoleOnline;
CMD_END

//-----------------------------------------------------------------------------
// ����������LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogWorldData)
	DWORD dwData[DataCollection_Max_Data_Num];
CMD_END

CMD_START(NDBC_RequestLogWorldData)
CMD_END

CMD_START(NDBS_RequestLogWorldData)
	DWORD dwData[DataCollection_Max_Data_Num];
CMD_END

//-----------------------------------------------------------------------------
// ��ɫ����LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogRoleData)
	DWORD dwRoleID;
	DWORD dwData[DataCollection_Max_RoleData_Num];
CMD_END

CMD_START(NDBC_RequestLogRoleData)
	DWORD dwRoleID;
CMD_END

CMD_START(NDBS_RequestLogRoleData)
	DWORD dwRoleID;
	DWORD dwData[DataCollection_Max_RoleData_Num];
CMD_END

//-----------------------------------------------------------------------------
// �˺�����ʱ��LOG��¼
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogAccountOnlineTime)
	DWORD dwAccoutID;
	DWORD dwOnlineSecond;
CMD_END

//Ԫ��װ��ǿ��log
CMD_START(NDBC_LogHolyEquipCon)
tagLogHolyEquipConsolidate sLogHolyEquip;
CMD_END

// ��ɫPKɱ�˼�Log
CMD_START(NDBC_LogKillrole)
	DWORD dwSrcRoleID;		// ɱ�˽�ɫID
	DWORD dwTargetRoleID;	// ��ɱ��ɫID
CMD_END
//-----------------------------------------------------------------------------
#pragma pack(pop)
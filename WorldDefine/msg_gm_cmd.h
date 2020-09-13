//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_gm_cmd.h
// author: 
// actor:
// data: 
// last:
// brief: GM���������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	E_GMCmd_Low_Privilege,	// Ȩ�޲���

};

enum EDDPType{
	EDDP_Exp		= 0,				//˫������
	EDDP_Item		= 1,				//˫����Ʒ
};

#define MAX_GM_COMMAND_LEN 128 
//----------------------------------------------------------------------------
// ��Ϣ�ṹ
//----------------------------------------------------------------------------
CMD_START(NC_GMCommand)		// ͨ��GM����
	TCHAR	szCommand[MAX_GM_COMMAND_LEN];
CMD_END;

//----------------------------------------------------------------------------
// ��Ϸ���緢���ͻ��ˣ�����ĳ������Ϊ����ɣ�����δ���
//----------------------------------------------------------------------------
CMD_START(NS_GMQuestState)		// ͨ��GM����
	UINT16		u16QuestID;		// ����ID
	BOOL		bDone;			// ����״̬ TRUE������ɣ�FALSE��δ���
CMD_END;


CMD_START( NS_DoubleDropProbabilityChange )	// ˫�����ʱ仯֪ͨ
	EDDPType	eType;			// ˫������
	float			nProbability;	// ������
	BOOL		bOpen;			// �Ƿ���
	DWORD		dwTimeLeft;		// ʣ��ʱ��
CMD_END
//----------------------------------------------------------------------------
CMD_START(NC_GetServiceInfo)		// ��ȡ��������Ϣָ��
CMD_END;

CMD_START(NS_GetServiceInfo)		// ��ȡ��������Ϣָ��
	CHAR szIP[64];
	CHAR szMac[64];
	CHAR szMd5[48];
	CHAR szSign[32];
CMD_END;


#pragma pack(pop)
//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: login_issue.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: ��½����������������������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "..\WorldDefine\msg_common_errorcode.h"
#include "..\WorldDefine\selectrole.h"
#include "..\ServerDefine\role_data.h"
#include "login_define.h"
#include "vip_netbar_define.h"

#pragma pack(push, 1)

enum
{
	E_PlayerWillLogin_PlayerNumLimit	=	1,	// ������������

	E_PlayerLogin_VerifyCodeError		=	11,	// ��֤�����
	E_PlayerLogin_TimeOut				=	12,	// ��ʱ
	E_PlayerLogin_PKRunawayPunishmentState = 13,// PK���ݳͷ�״̬
};

//------------------------------------------------------------------------------
// ������������½��������֤��Ϣ
//------------------------------------------------------------------------------
CMD_START(NLW_Certification)
	DWORD			dwGoldenCode;
	BOOL			bOrangeVipBar;
CMD_END

CMD_START(NWL_Certification)
	DWORD			dwGoldenCode;
	TCHAR			szWorldName[X_SHORT_NAME];
	DWORD			dwPort;	//ZoneServer�˿�
	DWORD			dwIP;	//������IP,���ڹ�����
	INT				nOLAccountNum;
	DWORD			dwOLAccountID[1];// ���е�ǰ�������
CMD_END

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
CMD_START(NLW_HeartBeat)
CMD_END

//-------------------------------------------------------------------------------
// ���������֪ͨ��½��������ǰ������״̬
//-------------------------------------------------------------------------------
CMD_START(NWL_WorldStatus)
	EWorldStatus	eStatus;			// ��ǰ������״̬
	INT				nPlayerNumLimit;	// ��ǰ����������������
	INT				nCurPlayerNum;		// ��ǰ�����������������ߵĺ͵ȴ�����ģ�
	INT				nCurPlayerDisplay;		// ��ǰ�����������������ߵĺ͵ȴ�����ģ�
CMD_END

//--------------------------------------------------------------------------------
// ��½������֪ͨ����������ʺŵ���
//--------------------------------------------------------------------------------
CMD_START(NLW_PlayerWillLogin)
	DWORD				dwAccountID;
	BYTE				byPrivilege;				// Ȩ��
	DWORD				dwVerifyCode;
	DWORD               dwChannelID;                //�����Ӫ��ID
	BOOL				bGuard;						// ������
	DWORD				dwAccOLSec;					// �ۼƵ�¼ʱ��
	char				szAccount[X_SHORT_NAME];	// ����˺�
	CHAR				szKrUserID[X_SHORT_NAME];	//�����汾��

	tagDWORDTime		dwPreLoginTime;				// �ϴε�¼ʱ��
	DWORD				dwPreLoginIP;				// �ϴε�¼ip
CMD_END

//--------------------------------------------------------------------------------
// ���������֪ͨ��½�������ʺſ��Ե���
//--------------------------------------------------------------------------------
CMD_START(NWL_PlayerWillLogin)
	DWORD			dwErrorCode;
	DWORD			dwAccountID;
CMD_END


//--------------------------------------------------------------------------------
// ���������֪ͨ��½��������ҵ���
//--------------------------------------------------------------------------------
CMD_START(NWL_PlayerLogin)
	DWORD			dwAccountID;
	DWORD			dwIP;
	DWORD			dwErrorCode;
	DWORD			dwWorldID;
	CHAR            szMac[MAX_MAC_LENGTH];
CMD_END

//---------------------------------------------------------------------------------
// ���������֪ͨ��½��������ҵǳ�
//---------------------------------------------------------------------------------
CMD_START(NWL_PlayerLogout)
	DWORD			dwAccountID;
	DWORD			dwWorldID;
CMD_END

//---------------------------------------------------------------------------------
// ��½������֪ͨ����������߳�ָ�����(�Ѿ�������������е�����ٴε�¼ʱ, ��Ҫ�߱������������)
//---------------------------------------------------------------------------------
CMD_START(NLW_KickPlayer)
	DWORD			dwAccountID;
CMD_END

//---------------------------------------------------------------------------------
// ���������������ʱ��֪ͨ����½����������¼
//---------------------------------------------------------------------------------
CMD_START(NWL_KickLog)
	DWORD			dwAccountID;	// �˺�
	DWORD			dwKickTime;		// �߳�ʱ��
	DWORD			u16ErrCode;		// �����ϵͳ�ṩ�Ĵ����
	BYTE			bySeal;			// �Ƿ���
CMD_END


//---------------------------------------------------------------------------------
// ������������ر�ʱ����½����������
//---------------------------------------------------------------------------------
CMD_START(NLW_WorldColsed)

CMD_END
//---------------------------------------------------------------------------------
// ������������ر�ʱ��֪ͨ����½�������ı���ҵ�½״̬
//---------------------------------------------------------------------------------
CMD_START(NWL_WorldColsed)

CMD_END

//---------------------------------------------------------------------------------
// ���
//---------------------------------------------------------------------------------
CMD_START(NWL_ForbidAccount)
	DWORD			dwAccountID;	// �ʺ�
	DWORD			dwForbidCode;	// �����
CMD_END

// Jason 2010-4-27 v2.1.0
CMD_START(NWL_PlayerPKRunaway)
	DWORD			dwAccountID;
	DWORD			dwTicksWantedToWait;
CMD_END

//---------------------------------------------------------------------------------
// ��Խ�ϰ汾�����ԣ�֪ͨ��½��������ʼ�ͽ��������Լ�ʱ
//---------------------------------------------------------------------------------
CMD_START(NWL_FatigueTimer)
	DWORD			dwAccountID;	// �˺�
	BYTE			byEnterMap;		// 1:��ɫ�����ͼ 0:��ɫ�뿪��ͼ
CMD_END

//---------------------------------------------------------------------------------
//������ɫ����
//---------------------------------------------------------------------------------
CMD_START(NWL_CreateRole)
DWORD			dwAccountID;	// �˺�
TCHAR			szRoleName[X_SHORT_NAME];
EPreChapStatus	eStatus;
tagAvatarAtt	AvatarAtt;
CMD_END

//---------------------------------------------------------------------------------
//������ɫ����
//---------------------------------------------------------------------------------
CMD_START(NLW_CreateRole)
TCHAR			szRoleName[X_SHORT_NAME];
DWORD			dwAccountID;	// �˺�
DWORD           dwErrorCode;
EPreChapStatus	eStatus;
tagAvatarAtt	AvatarAtt;
CMD_END

//---------------------------------------------------------------------------------
//������������
//---------------------------------------------------------------------------------
CMD_START(NWL_CreateGuild)
DWORD		dwAccountID;	// �˺�
DWORD		dwNPCID;
DWORD       dwRoleID;
TCHAR		szGuildName[X_SHORT_NAME];
CMD_END

//---------------------------------------------------------------------------------
//������������
//---------------------------------------------------------------------------------
CMD_START(NLW_CreateGuild)
DWORD		dwAccountID;	// �˺�
DWORD       dwErrorCode;
DWORD		dwNPCID;
DWORD       dwRoleID;
TCHAR		szGuildName[X_SHORT_NAME];

CMD_END

//---------------------------------------------------------------------------------
//������������
//---------------------------------------------------------------------------------
CMD_START(NWL_CreateFamily)
	DWORD		dwAccountID;	// �˺�
	DWORD		dwNPCID;
	DWORD       dwRoleID;
	TCHAR		szFamilyName[X_SHORT_NAME];
CMD_END

//---------------------------------------------------------------------------------
//������������
//---------------------------------------------------------------------------------
CMD_START(NLW_CreateFamily)
	DWORD		dwAccountID;	// �˺�
	DWORD       dwErrorCode;
	DWORD		dwNPCID;
	DWORD       dwRoleID;
	TCHAR		szFamilyName[X_SHORT_NAME];
CMD_END


// ������������
CMD_START( NWL_GetVipNetbarInfo )
CMD_END

//---------------------------------------------------------------------------------
//��������ֻ���
//---------------------------------------------------------------------------------
CMD_START(NWL_SaveTelNum)
DWORD	dwAccountID;	// �˺�
char	szAccount[X_SHORT_NAME];			// ����˺�
DWORD	dwRoleID;	
INT64	n64TelNum;			// ����ֻ���
CMD_END

//---------------------------------------------------------------------------------
//��������ֻ���
//---------------------------------------------------------------------------------
CMD_START(NLW_SaveTelNum)
DWORD	dwRoleID;	
DWORD   dwErrorCode;	// �������
CMD_END

//---------------------------------------------------------------------------------
//��Ҳ������ֻ���
//---------------------------------------------------------------------------------
CMD_START(NWL_NotSaveTelNum)
DWORD	dwAccountID;	// �˺�
CMD_END



struct tagStrIPRange
{
	TCHAR strIPMin[20]; // xxx.xxx.xxx.xxx
	TCHAR strIPMax[20]; // xxx.xxx.xxx.xxx
};
struct tagVipNetVarDBProto
{
	DWORD	dwID;
	TCHAR	szName[X_SHORT_NAME];
	//INT		nPlayerNum;
	DWORD	dwBuffID;		// �������ɵ�¼�����еĳ���ʱ��8Сʱ
	DWORD	dwInstBuffID;	// ���븱�������ӵ�buff

	tagStrIPRange	IpStrRange[MAX_VNB_IP_NUM];	// һһ��Ӧ��ip�ˣ���dwIpMax��Ӧ
};
// ��ȡ������������
CMD_START( NLW_IssueVipNetbarInfo )
	INT16	nNetbarNum;
	tagVipNetVarDBProto data[1];
CMD_END

#pragma pack(pop)
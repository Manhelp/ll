//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_c.h
// author: 
// actor:
// data: 2009-04-27
// last:
// brief: GM�������GM����֮�����Ϣ����
//-----------------------------------------------------------------------------
#pragma once

//#include "../WorldDefine/base_define.h"
#include "rt_define.h"
#include "../ServerDefine/gm_tool_define.h"

#pragma pack(push, 1)

#define X_SHORT_NAME			32
//-----------------------------------------------------------------------
//GM����
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMCreate)
	TCHAR			szName[32];			//�ʺ�
	DWORD			dwPswCrc;						//����CRC
	EPrivilege		ePrivilege;						//Ȩ��
CMD_END

CMD_START(NGMTS_GMCreate)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//GM�޸�
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMModify)
	TCHAR			szName[32];			//�ʺ�
	EPrivilege		ePrivilege;						//Ȩ��
CMD_END

CMD_START(NGMTS_GMModify)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//GMɾ��
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMDelete)
	TCHAR			szName[32];			//�ʺ�
CMD_END

CMD_START(NGMTS_GMDelete)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//						��¼�鿴msg_rt_c.h 
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//��ɫ�ָ�
//-----------------------------------------------------------------------
CMD_START(NGMTC_RoleResume)
	INT				nOldID;							//��ɫԭ����ID
	TCHAR			szNewName[32];		//��ɫ���������
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
CMD_END

CMD_START(NGMTS_RoleResume)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��ɫ�����޸�
//-----------------------------------------------------------------------
CMD_START(NGMTC_ProModify)
	TCHAR			szName[32];			//��ɫ����
	INT				nLel;							//�ȼ�
	INT				nSkill;							//���ʵ�
	INT				nAttPoint;						//���Ե�
	INT				nYuanbao;						//Ԫ��
	INT64			n64Exp;							//����
	INT64			n64Money;						//Ǯ	
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
CMD_END

CMD_START(NGMTS_ProModify)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��Ʒ����
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemCreate)
	TCHAR			szName[32];			//��ɫ
	DWORD			dwItemTypeID;					//��ƷTypeID
	INT				nItemNum;						//����
	BYTE			byQuality;						//Ʒ��
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
CMD_END

CMD_START(NGMTS_ItemCreate)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��Ʒɾ��
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemDelete)
	TCHAR			szName[32];			//��ɫ
	//INT64			n64ItemID;						//��Ʒȫ��ID
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
	TCHAR			sz64IDString[1];				//Ҫɾ����Ʒ��64λID��ɵĴ�		
CMD_END

CMD_START(NGMTS_ItemDelete)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//��Ʒ�ָ�
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemResume)
	TCHAR			szTargetRole[32];		//Ŀ���ɫ��
	DWORD			dwSectionID;					//����ID
	DWORD			dwWorldID;						//��Ϸ����ID
	TCHAR			sz64IDString[1];				//��Ʒȫ��ID
CMD_END

CMD_START(NGMTS_ItemResume)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �˺ŷ�ͣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_AccountForbid)
	TCHAR			szSectionName[32];		//����
	TCHAR			szWorldName[32];			//��Ϸ����
	INT				nAction;							//0:��� 1����
	TCHAR           szAccountStr[1];					//�˺Ŵ�,���˺�֮�䶺�ŷָ�
CMD_END

CMD_START(NGMTS_AccountForbid)
	//BOOL			bLast;
	TCHAR           szAccount[32];
	INT				nAction;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//--˫��
//-----------------------------------------------------------------------
CMD_START(NGMTC_Double)
	EDoubleType		eDType;
	DWORD			dwOpenTime;						//����ʱ��
	DWORD			dwLastTime;						//����ʱ��
	DWORD			dwRatio;						//����
	DWORD			dwSectionID;					//����ID,
	DWORD			dwWorldID[1];					//��Ϸ����ID[n]     ����ô�����������Ϸ����		
CMD_END

//-----------------------------------------------------------------------
//--���½ǹ���
//-----------------------------------------------------------------------
CMD_START(NGMTC_RightNotice)								
	DWORD			dwNoticeTime;					//����ʱ�� tagDWORDTime 0Ϊ��ʱ
	TCHAR			szTitle[Notice_Title];			//����
	TCHAR			szContent[Notice_Content];		//����
	TCHAR			szLink[Notice_Link];			//���ӵ���վ
	DWORD			dwSectionID;					//����ID,
	DWORD			dwWorldID[1];					//��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����											
CMD_END

//-----------------------------------------------------------------------
//--�Զ�ѭ������
//-----------------------------------------------------------------------
CMD_START(NGMTC_AutoNotice)
	TCHAR			szContent[Notice_Content];		//����
	EColorType		eType;							//��ɫ
	DWORD			dwIssueTime;					//����ʱ��
	INT				nTimeInterval;					//�������
	INT				nCirInterval;					//ѭ��ʱ��
	INT				nCirNum;						//ѭ������
	DWORD			dwSectionID;					//����ID,
	DWORD			dwWorldID[1];					//��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����	
CMD_END

//-----------------------------------------------------------------------
// ȡ���������趨���Զ�ѭ��
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelAutoNotice)
CMD_END

CMD_START(NGMTS_CancelAutoNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//--��ʱ���йصķ�����Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTS_TimeLimitedMsg)
	ETimeLimitedMsg	eType;	
	DWORD			dwErrorCode;					// 0-�ɹ�
	DWORD			dwWorldID;
CMD_END

//-----------------------------------------------------------------------
// ���/ɾ��������IP
//-----------------------------------------------------------------------
CMD_START(NGMTC_Blacklist)
	TCHAR			szSectionName[32];		// ����
	DWORD			dwIP;								// IP
	INT				nAction;							// 0:��� 1����
CMD_END

CMD_START(NGMTS_Blacklist)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ����Ƶ���Զ�����
//-----------------------------------------------------------------------
CMD_START(NGMTC_AutoChatNotice)
	TCHAR			szNotice[X_HUGE_STRING];			// ��������
	BYTE			byHour;								// ����: ʱ
	BYTE			byMinute;							// ����: ��
	DWORD			dwSectionID;						// ����ID
	DWORD			dwWorldID[1];						// ��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����	// ����ʱ��
CMD_END

CMD_START(NGMTS_AutoChatNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ���������趨����Ƶ����Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelAutoChatNotice)
CMD_END

CMD_START(NGMTS_CancelAutoChatNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ�����м�ʱ��Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelTimeLimitedMsg)
CMD_END

CMD_START(NGMTS_CancelTimeLimitedMsg)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ������˫������
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelDoubleMsg)
	//DWORD			dwSectionID;						// ����ID
	//DWORD			dwWorldID[1];						// ��Ϸ����ID[n]      ��Ϸ����ID:GT_INVALID����ô�����������Ϸ����	// ����ʱ��
CMD_END

CMD_START(NGMTS_CancelDoubleMsg)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ȡ���������趨���½�
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelRightNotice)
CMD_END

CMD_START(NGMTS_CancelRightNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ���ָ����ҵİٱ���
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearBaibao)
	TCHAR	szOwnerName[X_SHORT_NAME];
	DWORD	dwSectionID;
	DWORD	dwWorldID;
CMD_END

CMD_START(NGMTS_ClearBaibao)
	DWORD			dwErrorCode;
CMD_END

CMD_START(NGMTS_GMSetFackNumer)
	float			fFackRate;
CMD_END

//-----------------------------------------------------------------------
// ��ɫ������Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_RoleSpeak)
	TCHAR	szRoleName[X_SHORT_NAME];
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	bool	bAction;		// 0 ���� 1���
CMD_END

CMD_START(NGMTS_RoleSpeak)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �����ŵ���Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_KickLockedRole)
	TCHAR	szAccountName[X_SHORT_NAME];
	DWORD	dwSectionID;
CMD_END

CMD_START(NGMTS_KickLockedRole)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ���ƺŵ���Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_IssueTitle)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
	DWORD	dwTitleID; 
CMD_END

CMD_START(NGMTS_IssueTitle)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// ɾ�ƺŵ���Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_DelTitle)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
	DWORD	dwTitleID; 
CMD_END

CMD_START(NGMTS_DelTitle)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �ƶ���ɫ����Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_MoveRole)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
	TCHAR	szMapName[X_SHORT_NAME];
	INT		nX;
	INT		nZ;
CMD_END

CMD_START(NGMTS_MoveRole)
	DWORD	dwErrorCode;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// �����ɫ��������Ե����Ϣ
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearAttPoint)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_ClearAttPoint)
	DWORD	dwErrorCode;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// �޸İ��ɳ�Ա�ĵȼ�
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildChgMemLvl)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szGuildName[X_SHORT_NAME];
	TCHAR	szRoleName[X_SHORT_NAME];
	INT		nLvl;
CMD_END

CMD_START(NGMTS_GuildChgMemLvl)
	DWORD	dwErrorCode;
	//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// �޸İ����ʽ�
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildChgFund)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szGuildName[X_SHORT_NAME];
	INT32	n32Fund;
CMD_END

CMD_START(NGMTS_GuildChgFund)
	DWORD	dwErrorCode;
	//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// �޸İ���״̬
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildChgState)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szGuildName[X_SHORT_NAME];
	DWORD	dwState;
CMD_END

CMD_START(NGMTS_GuildChgState)
	DWORD	dwErrorCode;
	//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
//  PK log
//-----------------------------------------------------------------------
CMD_START(NGMTC_AddNeedPKLogRole)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	DWORD	dwRoleID;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_AddNeedPKLogRole)
	DWORD	dwErrorCode;
CMD_END

enum EActionType;
struct tagLoadLogItem
{
	//EActionType eType;
	TCHAR	szGMName[X_SHORT_NAME];
	TCHAR	szTime[X_SHORT_STRING];
	TCHAR	szIP[X_SHORT_STRING];
	TCHAR	szDetails[X_HUGE_STRING];
};

CMD_START(NGMTC_LoadLog)
	EActionType	eType1;
	EActionType eType2;
CMD_END

CMD_START(NGMTS_LoadLog)
	INT	nNum;
	EActionType eType;
	tagLoadLogItem	item[1];
CMD_END

//��ӹ����˴�
CMD_START(NGMTC_AddFilterWords)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szFilterWords[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_AddFilterWords)
DWORD  dwErrorCode;
CMD_END

//ɾ�������˴�
CMD_START(NGMTC_DelFilterWords)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szFilterWords[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_DelFilterWords)
DWORD  dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �޸İ��ɾݵ�սռ��״̬
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildGodMiracle)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szGuildName[X_SHORT_NAME];
DWORD	dwMiracleType;
CMD_END

CMD_START(NGMTS_GuildGodMiracle)
DWORD	dwErrorCode;
//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// ����ʺŰ�ȫ��
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearSafeCode)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szAccountName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_ClearSafeCode)
DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// �رշ�����
//-----------------------------------------------------------------------
CMD_START(NGMTC_CloseGuard)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szAccountName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_CloseGuard)
DWORD	dwErrorCode;
CMD_END

#pragma pack(pop)

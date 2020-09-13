//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: login_define.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: ��½������������ݽṹ
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/time.h"
#include "../WorldDefine/base_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ��Ϸ����״̬
//-----------------------------------------------------------------------------
enum EWorldStatus
{
	EWS_Well			=	0,		// ����
	EWS_InitNotDone		=	1,		// δ��ʼ�����
	EWS_SystemError		=	2,		// ϵͳ���ֹ���
	EWS_ProofError		=	3,		// ��֤����������
	EWS_DBBlock			=	4,		// ���ݿ�����
};

//------------------------------------------------------------------------------
// ��֤����
//------------------------------------------------------------------------------
enum EProofPolicy
{
	EPP_Null	=  -1,
	EPP_Test	=	0,		// ����ר��
	EPP_Own		=	1,		// �Լ���˾��֤����
	EPP_XunLei	=	2,		// xunlei
	EPP_Orange	=	3,		// game orange
	EPP_QuanQuan=	4,		// ʢ��ȦȦ
	EPP_Corn    =   5,      // ������֤����
	EPP_Vietnam =	6,		// Խ�ϰ汾
	EPP_Omg =	7,			// ̨��OMG�汾
	EPP_Russia =	10,		// ����˹
	EPP_Korea	=	11,		// ��һ�Һ�����˾

	EPP_End		=	12,
};



//------------------------------------------------------------------------------
// ��ҵ�¼ʱ��״̬
//------------------------------------------------------------------------------
enum  EPlayerLoginStatus
{
	EPLS_Null		=	-1,		// ��Ч
	EPLS_OffLine	=	0,		// ����
	EPLS_Online		=	1,		// ����
	EPLS_Loging		=	2,		// ��¼��
	EPLS_Unknown	=	3,		// δ֪
};

//------------------------------------------------------------------------------
// ��ҷ�ͣ����  ��Ա���ķ�ͣ0x01��GM���߷�ͣ0x02���ֻ���0x04���ܱ���0x08
//------------------------------------------------------------------------------
enum  EPlayerFrobidMask
{
	EPLM_MemberCenter	=	0x01,		// ��Ա����
	EPLM_GMTool			=	0x02,		// GM����
	EPLM_CellPhone		=	0x04,		// �ֻ���
	EPLM_MiBao			=	0x08,		// �ܱ�����ͣ
	EPLM_WaiGua			=	0x10,		// ������Զ���ͣ
	EPLM_Level			=	0x20,		// �ȼ��쳣
};
//-----------------------------------------------------------------------------
// ��֤����ṹ
//-----------------------------------------------------------------------------
struct tagProofResult
{
	DWORD					dwClientID;			// �ͻ���ID
	DWORD					dwAccountID;		// �ʺ�ID
	BOOL					bGuard;				// ������
	INT						nGuardAccuTime;		// �������ۻ�ʱ��
	EPlayerLoginStatus		eLoginStatus;		// ��½״̬
	DWORD					dwWorldNameCrc;		// ��һ�ε�½����Ϸ����
	DWORD					dwSeondPsdCrc;		// ��������CRCֵ
	DWORD					dwFrobidMask;		// ��ͣ���� ÿ1λ��Ӧһ�ַ�ͣ����
	BYTE					byPrivilege;		// Ȩ��
	bool					bNeedMibao;			// �Ƿ�ʹ���ܱ�
	bool					bSaveTelNum;		// �Ƿ���д�ֻ���
	bool					bIsFirstLogin;		// �Ƿ��һ�ε���
	DWORD					dwPreLoginIP;		// �ϴε�¼ip
	tagDWORDTime			dwPreLoginTime;		// �ϴε�¼ʱ��
	CHAR					szAccountName[X_SHORT_NAME]; //�û��� �������ӵ�¼
	BYTE					byReserved[2];		// ����������

	CHAR					szUserID[X_SHORT_NAME];		// UserID
	CHAR					szAppKey[X_APPKEY];			// AppKey
	CHAR					szToken[X_TOKEN];			// Token

	tagProofResult()
	{
		dwClientID		=	GT_INVALID;
		dwAccountID		=	GT_INVALID;
		bGuard			=	FALSE;
		nGuardAccuTime	=	0;
		eLoginStatus	=	EPLS_OffLine;
		byPrivilege		=	0;
		dwSeondPsdCrc	=   GT_INVALID;
		bNeedMibao		=	false;
		bSaveTelNum		=   0;
		dwFrobidMask	=   0;
		dwPreLoginIP    =	0;
		dwPreLoginTime	=   0;
		bIsFirstLogin	=	false;
		memset(szAccountName,0,X_SHORT_NAME);
		memset(szUserID,0,X_SHORT_NAME);
		memset(szAppKey,0,X_APPKEY);
		memset(szToken,0,X_TOKEN);
	}
};

struct tagProofResultFull : public tagProofResult
{
	INT nRet;		// ���ؽ��
};

//-----------------------------------------------------------------------------
// ��֤�ص�����
//-----------------------------------------------------------------------------
typedef UINT (WINAPI* PROOFCALLBACK)(INT nRet, tagProofResult* pResult);


#pragma pack(pop)
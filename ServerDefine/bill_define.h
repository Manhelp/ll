//-----------------------------------------------------------------------------
//!\file bill_define.h
//!\author Aslan
//!
//!\date 2009-06-22
//! last 2009-06-22
//!
//!\brief ֱ�仮��������¼�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// ֱ������
//-----------------------------------------------------------------------------
enum EBillType
{
	EBBT_Null			=	-1,

	EBBT_Yuanbao		=	0,		// ֱ��Ԫ��
	EBBT_Item			=	1,		// ֱ����Ʒ

	EBBT_End			=	2,
};

//-----------------------------------------------------------------------------
// ����״̬
//-----------------------------------------------------------------------------
enum EBillStateType
{
	EBST_Null			=	-1,

	EBST_Undeal			=	0,		// δ����
	EBST_Success		=	1,		// ����ɹ�

	EBST_Retry			=	2,		// ����һ��

	EBST_End			=	3,
};

#define MIBAO_LEN			240		// �ܱ����̶�
#define BILLINFO_PERTICK	10		// ÿ����ദ������Ϣ

//-----------------------------------------------------------------------------
// ������Ϣ�ṹ
//-----------------------------------------------------------------------------
struct tagBillInfo
{
	//DWORD		dwTokenID;			// ����ID
	CHAR		szTokenID[X_SHORT_NAME]; //����ID
	EBillType	eBillType;			// ֱ������
	DWORD		dwTypeID;			// ��ƷID(ֱ��Ԫ��ʱ��Ч)
	INT32		nNum;				// ����
	CHAR		szAccountName[X_SHORT_NAME];// �˺���
	EBillStateType eState;			//״̬
	DWORD		dwChannelID;		// ��Ӫ��ID
	tagBillInfo()
	{
		Reset();
	}

	VOID Reset()
	{
		//dwTokenID	= GT_INVALID;
		eBillType	= EBBT_Null;
		dwTypeID	= GT_INVALID;
		nNum		= 0;
		ZeroMemory(szAccountName, sizeof(szAccountName));
		ZeroMemory(szTokenID, sizeof(szTokenID));
		eState		= EBST_Null;
		dwChannelID	= GT_INVALID;
	}
};

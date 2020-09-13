//-----------------------------------------------------------------------------
// filename: msg_prison.h
// author: client/server leili/xlguo
// actor:
// data: 2009-02-10
// last: 
// brief: ������Ϣ����
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)
#include "msg_common_errorcode.h"
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
	E_Prison_Success					= E_Success,	// �ɹ�
	E_Prison_UnknownError					,			// δ֪����
};

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
CMD_START(NS_SendToPrison)
	DWORD			dwCaptorRoleID;			// Ϊ����������
	DWORD			dwCatchedRoleID;		// ��������������
CMD_END
// Jason 2010-6-18 v1.0.0
CMD_START(NS_SendToPrisonEx)
	DWORD			dwCaptorRoleID;			// Ϊ����������
	DWORD			dwCatchedRoleID;		// ��������������
	TCHAR			strCaptorRoleName[X_SHORT_NAME];
	TCHAR			strCatcherRoleName[X_SHORT_NAME];
CMD_END

#pragma pack(pop)
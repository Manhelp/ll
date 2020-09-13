//-----------------------------------------------------------------------------
//!\file msg_pk.h
//!
//!\date 2008-09-27
//!
//!\brief �ͻ����������֮�����PKģʽ��PK״̬��ص���Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "RoleDefine.h"

#pragma pack(push, 1)

enum
{
	E_SafeGuard_Already			=	1,		// �Ѿ��ǵ�ǰ״̬
	E_SafeGuard_PKStateInvalid	=	2,		// PK״̬Ϊ�Ǻ�ƽ״̬�������л�������ģʽ
	E_SafeGuard_TimeLimit		=	3,		// �����ϴο�������ģʽ��ʱ��δ���������л�������ģʽ
	E_SafeGuard_LevelLimit		=	4,		// �ȼ�����
	E_SafeGuard_ItemLimit		=	5,		// ʹ�õ�IM��Ʒ���ƣ��޷��л�������ģʽ
	E_SafeGuard_OpenLimit		=	6,		// �����ڲ��ɵ���PKģʽ��

	E_PK_Already				=	10,		// �Ѿ���������״̬���ܹ��������ˣ�
	E_PK_SafeGuard				=	11,		// ����ģʽ�²��ܿ���
	E_PKEX_Already				=	12,		// �Ѿ���������״̬��ֻ�ܹ���������ң�
	E_PK_PeaceModelAlready		=	13,		// �Ѿ�������ģʽ��
};
//-----------------------------------------------------------------------------
// PK����
//-----------------------------------------------------------------------------
CMD_START(NC_SafeGuard)
	BOOL	bOpen;			// �Ƿ���
	INT64	n64ItemID;		// ������ʹ�õ���Ʒ��64λID
CMD_END

CMD_START(NS_SafeGuard)
	DWORD	dwErrorCode;
	BOOL	bOpen;
CMD_END

//-----------------------------------------------------------------------------
// ��������״̬
//-----------------------------------------------------------------------------
CMD_START(NC_PK)
	BOOL	bOnlyPK;							// TRUE:�Կ�ģʽ; FALSE:ɱ¾ģʽ; -1:����ģʽ
CMD_END

CMD_START(NS_PK)
	DWORD	dwErrorCode;
	BOOL	bReset;								// �Ƿ�Ϊ����������
	BOOL	bOnlyPK;							// TRUE:�Կ�ģʽ; FALSE:ɱ¾ģʽ; -1:����ģʽ
CMD_END

//-----------------------------------------------------------------------------
// PK״̬�ı�
//-----------------------------------------------------------------------------
CMD_START(NS_RolePKStateChange)
	DWORD			dwRoleID;					// ��ɫID
	ERolePKState	eState;						// �µ�PK״̬
CMD_END

//-----------------------------------------------------------------------------
// PK����ֵ�ָ�����
//-----------------------------------------------------------------------------
CMD_START(NS_RolePKMorality)
	INT		nTime;
	INT		nValue;
CMD_END

//-----------------------------------------------------------------------------
// �������������������ҵĹ���
//-----------------------------------------------------------------------------
CMD_START(NS_RoleBeAttacked)
	DWORD	dwEvilRoleID;						// ������ID
	BOOL	bWarning;							// �Ƿ��Ԥ����v1.0.0, Jason 2010-7-17
CMD_END

CMD_START(NS_AskToClosePKSafeGuard)
	INT		nPKSafeGuardMaxLevel;	// pk����
CMD_END
CMD_START(NC_AnswerToClosePKSafeGuard)
	DWORD	dwCloseOrNot;
CMD_END

#pragma pack(pop)
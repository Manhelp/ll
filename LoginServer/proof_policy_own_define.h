//-----------------------------------------------------------------------------
//!\file proof_policy_own_define.h
//!\author Aslan
//!
//!\date 2009-06-07
//! last 2008-06-07
//!
//!\brief ���뼤����ӿڶ���
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

#define X_STRING_LEN	50			// �ַ�������


//-----------------------------------------------------------------------------
// ��ʼ��½ID
//-----------------------------------------------------------------------------
CMD_START2(NC_LOGIN)
	DWORD	dwLoginID;				// login ID
CMD_END2

CMD_START2(NS_LOGIN)
	BYTE	byResult;				// ���ؽ��
CMD_END2

//------------------------------------------------------------------------------
// ������Ϣ
//------------------------------------------------------------------------------
CMD_START2(NC_HEARTBEAT)
	DWORD	dwLoginID;					// login ID
CMD_END2

//------------------------------------------------------------------------------
// ��ҵ���
//------------------------------------------------------------------------------
CMD_START2(NC_USERLOGIN)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_USERNAME_LENEX];	// �ʺ���
	CHAR	szPsd[X_STRING_LEN];			// ����
	DWORD	dwClientID;						// ���к�
	DWORD   dwChannelID;                    // �����Ӫ��ID
CMD_END2

CMD_START2(NS_USERLOGIN)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szPsd[X_STRING_LEN];			// �µ�����
	BYTE	byResult;						// ��֤���
	BYTE	byState;						// 0 �û�״̬����   1�û��˺ű���ͣ
	BYTE	byBlocktype;					// 1����Ա���ķ�ͣ4���ֻ����˺���Ϸ��ͣ 8���ܱ����˺ŷ�ͣ
	BYTE	byGuard;						// ������
	DWORD	dwClientID;						// ���к�
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
CMD_END2

//-------------------------------------------------------------------------------
// �û���������
//-------------------------------------------------------------------------------
CMD_START2(NS_USERUPDATEPWD)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szPsd[X_STRING_LEN];			// ������
CMD_END2

CMD_START2(NC_USERUPDATEPWD)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END2

//---------------------------------------------------------------------------------
// �û����ܱ�
//---------------------------------------------------------------------------------
CMD_START2(NS_USERBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
CMD_END2

CMD_START2(NC_USERBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END2

//---------------------------------------------------------------------------------
// �û�ȡ���ܱ�
//---------------------------------------------------------------------------------
CMD_START2(NS_USERUNBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	CHAR	szMibao[MIBAO_LEN];				// �ܱ�
CMD_END2

CMD_START2(NC_USERUNBINDMIBAO)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END2


//---------------------------------------------------------------------------------
// �˺ŷ�ͣ
//---------------------------------------------------------------------------------
CMD_START2(NS_BLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	BYTE    byBlockType;					//1����Ա���ķ�ͣ4���ֻ����˺���Ϸ��ͣ 8���ܱ����˺ŷ�ͣ
CMD_END2

CMD_START2(NC_BLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���  0 ��ȷ  1ʧ�� 
CMD_END2


//---------------------------------------------------------------------------------
// �˺Ž��
//---------------------------------------------------------------------------------
CMD_START2(NS_UNBLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	BYTE    byUnBlockType;					//1����Ա���Ľ��  4���ֻ����˺���Ϸ���  8���ܱ����˺Ž��
CMD_END2

CMD_START2(NC_UNBLOCKACCOUNT)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END2

//---------------------------------------------------------------------------------
// �˺����÷�����
//---------------------------------------------------------------------------------
CMD_START2(NS_USERUPDATECHENMI)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	DWORD	dwLoginID;						// login ID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	BYTE    byChenMi;						//0�Ƿ����� 1������
CMD_END2

CMD_START2(NC_USERUPDATECHENMI)
	CHAR	szGUID[X_STRING_LEN];			// GUID
	CHAR	szAccountName[X_STRING_LEN];	// �û���
	DWORD	dwLoginID;						// login ID
	BYTE	byResult;						// ���
CMD_END2



#pragma pack(pop)
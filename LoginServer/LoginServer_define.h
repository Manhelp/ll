//-----------------------------------------------------------------------------
//!\file LoginServer_define.h
//!
//!\date 2008-06-04
//!
//!\brief LoginServer �ṹ�ͳ����Ķ���
//!
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/login_define.h"

//-----------------------------------------------------------------------------
// CONSTANT
//-----------------------------------------------------------------------------
#define GUARD_TIME		18000		// ����Ϊ��λ��5Сʱ��18000��
#define MIBAO_LEN		240			// �ܱ����̶�

const INT CON_LOST		= -10000;	// ���ӶϿ���־

//------------------------------------------------------------------------------
// LoongWorld��ǰ��Ϣ
//------------------------------------------------------------------------------
struct tagLoongWorldData 
{
	BOOL			bValid;						// ��ǰ�Ƿ�����
	TCHAR			szName[X_SHORT_NAME];	    // World����
	DWORD			dwNameCrc;					// ����CRCֵ
	EWorldStatus	eStatus;					// ��ǰ״̬
	INT				nPort;						// �˿�
	DWORD			dwIP;						// IP
	INT				nMaxOnlineNum;				// �����������
	INT				nCurOnlineNum;				// ��ǰ��������
	INT				nCurOnlineDisplay;			// ��ʾ����������
	BOOL			bAutoSeal;					// �Ƿ��Զ�
	DWORD			dwWorldID;					// worldid
    TCHAR			szAttachServers[X_HUGE_NAME];	    // ����World����
    
	tagLoongWorldData()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//----------------------------------------------------------------
// ��¼��һ�����Ϣ sizeof = 396
//----------------------------------------------------------------
struct tagPlayerData 
{
	DWORD				dwClientID;							// �ͻ���ID
	DWORD				dwCDIndex;							// ����ID
	DWORD				dwAccountID;						// �ʺ�ID
	CHAR				szAccountName[X_SHORT_NAME];		// �ʺ�����
	CHAR				szKRUseName[X_SHORT_NAME];			// �����汾ʹ�õ�����
	CHAR                szMac[MAX_MAC_LENGTH];              // MAC��ַ
	DWORD				dwMibaoCrc;							// ����
	DWORD				dwSecondPsdCrc;						// ���������CRCֵ
//	BOOL				bDisabled;							// �Ƿ��ͣ
	DWORD				dwFrobidMask;						// ��ͣ���� ÿ1λ��Ӧһ�ַ�ͣ����
	BOOL				bGuard;								// �������û�
	INT					nAccOLSec;							// �ۼƵ�¼ʱ��
	DWORD				dwIP;								// �ͻ���IP
	DWORD				dwWorldNameCrc;						// ��Ϸ��������CRCֵ
	DWORD               dwChannelID;                        // �����Ӫ�̵�ID
	BYTE				byPrivilege;						// Ȩ��
	BYTE				byReserved[3];						// �����־

	tagPlayerData()		{ ZeroMemory(this, sizeof(*this)); }
	VOID Reset()		{ ZeroMemory(this, sizeof(*this)); }
};

//----------------------------------------------------------------
// ��¼��һ�����Ϣ sizeof = 396
//----------------------------------------------------------------
struct tagAccountData
{
	CHAR				szAccountName[X_SHORT_NAME];
	CHAR                szMac[MAX_MAC_LENGTH];
	DWORD				dwIp;
	BOOL				bGuard;
};
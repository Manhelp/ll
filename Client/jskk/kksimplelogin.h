/* -------------------------------------------------------------------------
// �ļ���		:	kksimplelogin/kksimplelogin.h
// ������		:	chenghui
// ����ʱ��		:	2011-4-18 15:19:34
// ��������     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __KKSIMPLELOGIN_KKSIMPLELOGIN_H__
#define __KKSIMPLELOGIN_KKSIMPLELOGIN_H__

// -------------------------------------------------------------------------
// ����ֵ����
#define KK_SIMPLELOGIN_SUCCESS			0	// �ɹ�
#define KK_SIMPLELOGIN_FAILED			1	// δ֪����
#define KK_SIMPLELOGIN_KK_NO_EXIST		2	// ���û����
#define KK_SIMPLELOGIN_DATA_ERROR		3	// ���ݴ���
#define KK_SIMPLELOGIN_UNLOGIN			4	// δ��¼


// -------------------------------------------------------------------------
// ������������
EXTERN_C INT STDAPICALLTYPE kkGetActiveUserToken(
	WCHAR* strTokenBuffer,
	ULONG nBufferLength
	);

// -------------------------------------------------------------------------
// $Log: $

#endif /* __KKSIMPLELOGIN_KKSIMPLELOGIN_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			I/O Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_I_O_H__
#define __JAM_I_O_H__

#include "jam_datatype.h"
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


	/** ����ļ������ļ����Ƿ���� */
	JAM_API bool jam_file_exist(const char* filename);


	/** ��ȡ�ļ���С */
	JAM_API int32_t jam_file_length(const char* filename);
	/** ��ȡ�ļ���С */
	JAM_API int32_t jam_file_length_fp(FILE* fp);


	/** �����ļ���(�Զ�����·���ϲ����ڵ��ļ���) */
	JAM_API bool jam_path_make(const char* path);


	/** ���·��(�ļ�)���Ƿ���ϲ���ϵͳҪ�� */
	JAM_API bool jam_path_valid(const char* path);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_I_O_H__ */

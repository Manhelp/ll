
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Dynamic Link Library Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_DL_H__
#define __JAM_DL_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** ��ȡһ����̬���ӿ� */
	JAM_API handle_t jam_dl_load(const char* filename);


	/** ��ȡ����ָ�� */
	JAM_API void* jam_dl_proc(handle_t handle, const char* func);


	/** �ͷ����ӿ��� */
	JAM_API void jam_dl_free(handle_t handle);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_DL_H__ */

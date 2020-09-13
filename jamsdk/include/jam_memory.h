
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Memroy Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MEMORY_H__
#define __JAM_MEMORY_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** �����ڴ� */
	JAM_API void* __jam_malloc(size_t size, const char* filename, const char* func, int32_t line);


	/** ���·����ڴ� */
	JAM_API void* __jam_realloc(void* mem, size_t size, const char* filename, const char* func, int32_t line);


	/** �ͷ��ڴ� */
	JAM_API void __jam_free(void* p, const char* filename, const char* func, int32_t line);


	/** ��ȡϵͳҳ��С(��λ: �ֽ�) */
	JAM_API size_t jam_page_size();


	/** ��ȡ�ڴ��ܴ�С(��λ: �ֽ�) */
	JAM_API size_t jam_memory_total_size();


	/** ��ȡ�ڴ�ʣ���С(��λ: �ֽ�) */
	JAM_API size_t jam_memory_avail_size();


	#define jam_malloc(n)				__jam_malloc(n, __FILE__, __FUNCTION__, __LINE__)
	#define jam_realloc(m, n)			__jam_realloc(m, n, __FILE__, __FUNCTION__, __LINE__)
	#define jam_free(p)					__jam_free(p, __FILE__, __FUNCTION__, __LINE__)


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MEMORY_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Thread Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_THREAD_H__
#define __JAM_THREAD_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t							thread_t;
	typedef handle_t							threadpool_t;

	typedef void (*task_t)(void* param);


	/** �����̳߳� */
	JAM_API threadpool_t jam_thread_alloc(uint32_t max_thread);


	/** �ͷ��̳߳�, ��ȴ��߳�ȫ����ȫ�˳� */
	JAM_API void jam_thread_free(threadpool_t tp);


	/** ���̳߳�ִ��һ������, ���tpָ��Ϊ0, ���Զ�����һ���߳� */
	JAM_API void jam_thread_exec(threadpool_t tp, task_t task, void* param);


	/** ��ȡ�̳߳�����߳��� */
	JAM_API size_t jam_thread_max_count(threadpool_t tp);


	/** �̳߳ص�ǰ�߳��� */
	JAM_API size_t jam_thread_count(threadpool_t tp);


	/** ��ȡ�̱߳�� */
	JAM_API thread_t jam_thread_self();


	/** �ó�CPU */
	JAM_API void jam_thread_yield();


	/** �ͷ�CPU��Դ */
	JAM_API void jam_thread_sleep(uint32_t millisec);


	#define jam_yield				jam_thread_yield
	#define jam_sleep				jam_thread_sleep


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_THREAD_H__ */

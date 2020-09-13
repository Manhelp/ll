
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Conditional Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_COND_H__
#define __JAM_COND_H__

#include "jam_datatype.h"
#include "jam_mutex.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t							cond_t;


	/** �����ź��� */
	JAM_API cond_t jam_cond_alloc();


	/** �ر��ź��� */
	JAM_API void jam_cond_free(cond_t event);


	/** �ȴ��ź��� */
	JAM_API void jam_cond_wait(cond_t event, mutex_t lock);


	/** ��ʱ�ȴ��ź���, ����: true ���ź���, false ��ʱ */
	JAM_API bool jam_cond_time_wait(cond_t event, mutex_t lock, uint32_t millisec);


	/** ����һ�����������߳� */
	JAM_API void jam_cond_signal(cond_t event);


	/** �������б��������߳� */
	JAM_API void jam_cond_broadcast(cond_t event);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_COND_H__ */

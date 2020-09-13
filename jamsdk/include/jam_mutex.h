
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Mutex Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MUTEX_H__
#define __JAM_MUTEX_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t						mutex_t;


	/** ���以���� */
	JAM_API mutex_t jam_mutex_alloc();


	/** �رջ����� */
	JAM_API void jam_mutex_free(mutex_t lock);


	/** ���� */
	JAM_API void jam_mutex_lock(mutex_t lock);


	/** ���� */
	JAM_API void jam_mutex_unlock(mutex_t lock);


	/** �������� */
	JAM_API bool jam_mutex_trylock(mutex_t lock);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MUTEX_H__ */

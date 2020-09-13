
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Array Set Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ARRAY_SET_H__
#define __JAM_ARRAY_SET_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				arrayset_t;


	/** �������鼯�϶��� */
	JAM_API arrayset_t jam_arrayset_alloc();


	/** �ͷ����鼯�϶��� */
	JAM_API void jam_arrayset_free(arrayset_t set);


	/** ��ȡ���ϴ�С */
	JAM_API size_t jam_arrayset_size(arrayset_t set);


	/** ����һ�����ݵ�����ָ��λ�� */
	JAM_API bool jam_arrayset_put(arrayset_t set, uint32_t key, void* data);


	/** ��ȡָ��λ���ϵ����� */
	JAM_API void* jam_arrayset_get(arrayset_t set, uint32_t key);


	/** ɾ��һ������ */
	JAM_API bool jam_arrayset_erase(arrayset_t set, uint32_t key);


	/** ��һ����¼ */
	JAM_API bool jam_arrayset_first(arrayset_t set, uint32_t* key);


	/** ���һ����¼ */
	JAM_API bool jam_arrayset_last(arrayset_t set, uint32_t* key);


	/** ��һ����¼ */
	JAM_API bool jam_arrayset_next(arrayset_t set, uint32_t* key);


	/** ��һ����¼ */
	JAM_API bool jam_arrayset_prev(arrayset_t set, uint32_t* key);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ARRAY_SET_H__ */

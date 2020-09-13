
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Array List Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ARRAY_H__
#define __JAM_ARRAY_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				array_t;


	/** ���������б���� */
	JAM_API array_t jam_array_alloc();


	/** �ͷ������б���� */
	JAM_API void jam_array_free(array_t array);


	/** ��ȡ�����С */
	JAM_API size_t jam_array_size(array_t array);


	/** ����һ������������ĩβ */
	JAM_API bool jam_array_push(array_t array, void* data);


	/** ����һ�����ݵ�����ָ��λ�� */
	JAM_API bool jam_array_put(array_t array, int32_t index, void* data);


	/** �޸�ָ��λ���ϵ����� */
	JAM_API bool jam_array_set(array_t array, int32_t index, void* data);


	/** ��ȡָ��λ���ϵ����� */
	JAM_API void* jam_array_get(array_t array, int32_t index);


	/** ɾ��һ������ */
	JAM_API bool jam_array_erase(array_t array, int32_t index);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ARRAY_H__ */

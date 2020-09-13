
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			List Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_LIST_H__
#define __JAM_LIST_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				list_t;


	/** �����б��� */
	JAM_API list_t jam_list_alloc();


	/** �ͷ��б���Դ */
	JAM_API void jam_list_free(list_t l);


	/** ��ȡ�б�Ԫ�ظ��� */
	JAM_API size_t jam_list_size(list_t l);


	/** �α��Ƶ��б�ͷ�� */
	JAM_API bool jam_list_cursor_first(list_t l);


	/** �α��Ƶ��б�β�� */
	JAM_API bool jam_list_cursor_last(list_t l);


	/** �α������һ�� */
	JAM_API bool jam_list_cursor_next(list_t l);


	/** �α���ǰ��һ�� */
	JAM_API bool jam_list_cursor_prev(list_t l);


	/** ��ǰ�α���û����� */
	JAM_API void* jam_list_cursor_data(list_t l);


	/** ��ǰ�α�������� */
	JAM_API bool jam_list_cursor_insert(list_t l, void* data);


	/** ɾ����ǰ�α����� */
	JAM_API bool jam_list_cursor_erase(list_t l);


	/** ��һ��Ԫ�ؼ��뵽�б�ͷ�� */
	JAM_API bool jam_list_push_front(list_t l, void* data);


	/** ��һ��Ԫ�ؼ��뵽�б�β�� */
	JAM_API bool jam_list_push_back(list_t l, void* data);


	/** ɾ���б�ͷ��һ��Ԫ�� */
	JAM_API void* jam_list_pop_front(list_t l);


	/** ɾ���б�β��һ��Ԫ�� */
	JAM_API void* jam_list_pop_back(list_t l);


	/** ��ȡ�б�ͷ��Ԫ�� */
	JAM_API void* jam_list_front(list_t l);


	/** ��ȡ�б�β��Ԫ�� */
	JAM_API void* jam_list_back(list_t l);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_LIST_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Hash Database Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_HASHDB_H__
#define __JAM_HASHDB_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_HDB_FILE = 1,					/** �ļ����ݿ�, ���֧�� 2 GB */
		JAM_HDB_MEMORY,						/** �ڴ����ݿ�, ���֧�����ڴ���� */
	}hdb_type_t;


	typedef enum
	{
		JAM_HDB_OVER = 1,					/** �������ݿ��¼ */
		JAM_HDB_KEEP,						/** ����ԭ���ݿ��¼ */
		JAM_HDB_CAT							/** ׷��д�����ݿ��¼ĩβ */
	}hdb_mode_t;

	
	typedef handle_t						hashdb_t;


	/** ����һ�����ݿ��� */
	JAM_API hashdb_t jam_hdb_alloc(hdb_type_t type, const char* open_str);


	/** �ر�һ�����ݿ��� */
	JAM_API void jam_hdb_free(hashdb_t db);


	/** ���ݿ����� */
	JAM_API hdb_type_t jam_hdb_type(hashdb_t db);


	/** ����һ������ */
	JAM_API bool jam_hdb_put(hashdb_t db, const byte_t* key, size_t ksize, const byte_t* data, size_t dsize, hdb_mode_t mode);


	/** ��ȡһ������ */
	JAM_API size_t jam_hdb_get(hashdb_t db, byte_t* data, size_t dsize, const byte_t* key, size_t ksize, size_t start);


	/** ɾ��һ������ */
	JAM_API bool jam_hdb_erase(hashdb_t db, const byte_t* key, size_t ksize);


	/** ��ȡ���ݴ�С */
	JAM_API size_t jam_hdb_data_size(hashdb_t db, const byte_t* key, size_t ksize);


	/** ���ݿ������ݸ��� */
	JAM_API size_t jam_hdb_count(hashdb_t db);


	/** ��һ����¼ */
	JAM_API size_t jam_hdb_first(hashdb_t db, byte_t* key, size_t ksize);


	/** ��һ����¼ */
	JAM_API size_t jam_hdb_next(hashdb_t db, byte_t* key, size_t ksize);


	/** ��ʼ���� */
	JAM_API bool jam_hdb_trans_begin(hashdb_t db);


	/** �ύ���� */
	JAM_API bool jam_hdb_trans_commit(hashdb_t db);


	/** �ع����� */
	JAM_API bool jam_hdb_trans_abort(hashdb_t db);


	/** ˢ�¼�¼��(�ļ����ݿ�Ὣ����д�����) */
	JAM_API void jam_hdb_flush(hashdb_t db);


	/** �Ż������ļ����ݿ� */
	JAM_API bool jam_hdb_optimize(hashdb_t db);


	/** �޸��ļ����ݿ� */
	JAM_API bool jam_hdb_repair(const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_HASHDB_H__ */

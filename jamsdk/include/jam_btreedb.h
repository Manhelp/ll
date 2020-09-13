
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			B+ Tree Database Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_BTREEDB_H__
#define __JAM_BTREEDB_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_BDB_OVER = 1,						/** �������ݿ��¼ */
		JAM_BDB_KEEP,							/** ����ԭ���ݿ��¼ */
		JAM_BDB_CAT,							/** ׷��д�����ݿ��¼ĩβ */
		JAM_BDB_DUP,							/** �����ظ��ؼ��� */
		JAM_BDB_DUPR							/** �����ظ��ؼ��ֲ��������� */
	}bdb_mode_t;

	typedef enum
	{
		JAM_BDB_CURRENT = 1,					/** ���ǵ�ǰ��¼ */
		JAM_BDB_BEFORE,							/** �ڵ�ǰ��¼֮ǰ����һ����¼ */
		JAM_BDB_AFTER							/** �ڵ�ǰ��¼֮�����һ����¼ */
	}bdb_cp_mode_t;

	typedef enum
	{
		JAM_BDB_STRING = 1,						/** ��¼�����ַ������� */
		JAM_BDB_NUMBER,							/** ��¼������������ */
		JAM_BDB_BINARY							/** ��¼�������������� */
	}bdb_sort_type_t;


	typedef handle_t							btreedb_t;


	/** ����һ�����ݿ��� */
	JAM_API btreedb_t jam_bdb_alloc(const char* filename, bdb_sort_type_t sort);


	/** �ر�һ�����ݿ��� */
	JAM_API void jam_bdb_free(btreedb_t db);


	/** ����һ������ */
	JAM_API bool jam_bdb_put(btreedb_t db, const byte_t* key, size_t ksize, const byte_t* data, size_t dsize, bdb_mode_t mode);


	/** ��ȡһ������ */
	JAM_API size_t jam_bdb_get(btreedb_t db, byte_t* data, size_t dsize, const byte_t* key, size_t ksize);


	/** ɾ��һ������ */
	JAM_API bool jam_bdb_erase(btreedb_t db, const byte_t* key, size_t ksize);


	/** ��ȡ���ݴ�С */
	JAM_API size_t jam_bdb_data_size(btreedb_t db, const byte_t* key, size_t ksize);


	/** ���ݿ������ݸ��� */
	JAM_API size_t jam_bdb_count(btreedb_t db);


	/** �޸ĵ�ǰ��¼ */
	JAM_API bool jam_bdb_cursor_put(btreedb_t db, const byte_t* data, size_t dsize, bdb_cp_mode_t mode);


	/** ���Ҷ�λ��¼ */
	JAM_API bool jam_bdb_cursor_find(btreedb_t db, const byte_t* key, size_t size);


	/** ��һ����¼ */
	JAM_API bool jam_bdb_cursor_first(btreedb_t db);


	/** ���һ����¼ */
	JAM_API bool jam_bdb_cursor_last(btreedb_t db);


	/** ��һ����¼ */
	JAM_API bool jam_bdb_cursor_next(btreedb_t db);


	/** ��һ����¼ */
	JAM_API bool jam_bdb_cursor_prev(btreedb_t db);


	/** ɾ����ǰ��¼ */
	JAM_API bool jam_bdb_cursor_erase(btreedb_t db);


	/** ��¼�ؼ��ֳ��� */
	JAM_API size_t jam_bdb_cursor_key_size(btreedb_t db);


	/** ��¼���ݳ��� */
	JAM_API size_t jam_bdb_cursor_data_size(btreedb_t db);


	/** ��¼�ؼ��� */
	JAM_API size_t jam_bdb_cursor_key(btreedb_t db, byte_t* key, size_t size);


	/** ��¼���� */
	JAM_API size_t jam_bdb_cursor_data(btreedb_t db, byte_t* data, size_t size);


	/** ��ʼ���� */
	JAM_API bool jam_bdb_trans_begin(btreedb_t db);


	/** �ύ���� */
	JAM_API bool jam_bdb_trans_commit(btreedb_t db);


	/** �ع����� */
	JAM_API bool jam_bdb_trans_abort(btreedb_t db);


	/** ˢ�¼�¼�� */
	JAM_API void jam_bdb_flush(btreedb_t db);


	/** �Ż������ļ����ݿ� */
	JAM_API bool jam_bdb_optimize(btreedb_t db);


	/** �޸��ļ����ݿ� */
	JAM_API bool jam_bdb_repair(const char* filename, bdb_sort_type_t sort);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_BTREEDB_H__ */

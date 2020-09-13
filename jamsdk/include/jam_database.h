
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Database Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_DATABASE_H__
#define __JAM_DATABASE_H__

#include "jam_hashdb.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				database_t;
	typedef handle_t				db_record_t;


	/** ��һ����¼ */
	JAM_API db_record_t jam_dbr_alloc(const char* name);


	/** �رռ�¼ */
	JAM_API void jam_dbr_free(db_record_t dbr);


	/** ��ȡ��¼���� */
	JAM_API const char* jam_dbr_name(db_record_t dbr);


	/** ���ü�¼�������� */
	JAM_API bool jam_dbr_put(db_record_t dbr, const char* attr, const byte_t* data, size_t size);


	/** ��ȡ��¼�������� */
	JAM_API const byte_t* jam_dbr_get(db_record_t dbr, size_t* size, const char* attr);


	/** ɾ����¼���� */
	JAM_API bool jam_dbr_erase(db_record_t dbr, const char* attr);


	/** ɾ����¼�������� */
	JAM_API size_t jam_dbr_clear(db_record_t dbr);


	/** ��¼������ */
	JAM_API size_t jam_dbr_attr_size(db_record_t dbr);


	/** ��¼��һ������ */
	JAM_API const char* jam_dbr_attr_first(db_record_t dbr);


	/** ��¼��һ������ */
	JAM_API const char* jam_dbr_attr_next(db_record_t dbr);



	/** ����һ�����ݿ��� */
	JAM_API database_t jam_db_alloc(hdb_type_t type, const char* filename);


	/** �ر�һ�����ݿ��� */
	JAM_API void jam_db_free(database_t db);


	/** ����һ����¼ */
	JAM_API bool jam_db_put(database_t db, db_record_t dbr);


	/** ����һ����¼���� */
	JAM_API bool jam_db_put_attr(database_t db, const char* name, const char* attr, const byte_t* data, size_t size);


	/** ��ȡһ����¼ */
	JAM_API db_record_t jam_db_get(database_t db, const char* name);


	/** ��ȡһ����¼���� */
	JAM_API size_t jam_db_get_attr(database_t db, byte_t* data, size_t size, const char* name, const char* attr);


	/** ɾ��һ����¼ */
	JAM_API bool jam_db_erase(database_t db, const char* name);


	/** ɾ��һ����¼���� */
	JAM_API bool jam_db_erase_attr(database_t db, const char* name, const char* attr);


	/** ��ȡ�ĵ����� */
	JAM_API size_t jam_db_count(database_t db);


	/** ��ȡ��һ����¼ */
	JAM_API db_record_t jam_db_first(database_t db);


	/** ��ȡ��һ����¼ */
	JAM_API db_record_t jam_db_next(database_t db);


	/** ˢ�¼�¼�� */
	JAM_API void jam_db_flush(database_t db);


	/** �Ż������ļ����ݿ� */
	JAM_API bool jam_db_optimize(database_t db);


	/** �޸����ݿ� */
	JAM_API bool jam_db_repair(const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_DATABASE_H__ */

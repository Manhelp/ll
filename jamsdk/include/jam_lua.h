
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			LUA Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_LUA_H__
#define __JAM_LUA_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef struct lua_State*					lua_t;
	typedef int (*lua_func_t)(lua_t l);


	typedef enum
	{
		JAM_LUA_FILENAME = 1,
		JAM_LUA_PROGRAM
	}lua_type_t;


	/** ��һ��LUA��� */
	JAM_API lua_t jam_lua_alloc();


	/** �ͷž�� */
	JAM_API void jam_lua_free(lua_t l);


	/** ִ�нű�(���Դ��ļ����ڴ�) */
	JAM_API bool jam_lua_do(lua_t l, const char* str, lua_type_t type);


	/** ���Ĵ�����Ϣ */
	JAM_API const char* jam_lua_last_error(lua_t l);


	/** ��ȡLUA��ջ��С */
	JAM_API size_t jam_lua_stack_size(lua_t l);


	/** ���LUA��ջ */
	JAM_API void jam_lua_stack_zero(lua_t l);


	/** ѹ��һ�����ֵ�ջ�� */
	JAM_API void jam_lua_push_number(lua_t l, double value);


	/** ѹ��һ���ַ�����ջ�� */
	JAM_API void jam_lua_push_string(lua_t l, const char* str);


	/** ѹ��һ������ֵ��ջ�� */
	JAM_API void jam_lua_push_boolean(lua_t l, bool value);


	/** ѹ��һ���û��Զ������ݵ�ջ�� */
	JAM_API void jam_lua_push_object(lua_t l, void* value);


	/** ѹ��һ����ֵ��ջ�� */
	JAM_API void jam_lua_push_null(lua_t l);


	/** ��ջ��ָ��λ�õ�Ԫ��ת������ */
	JAM_API double jam_lua_to_number(lua_t l, int32_t idx);


	/** ��ջ��ָ��λ�õ�Ԫ��ת���ַ��� */
	JAM_API const char* jam_lua_to_string(lua_t l, int32_t idx);


	/** ��ջ��ָ��λ�õ�Ԫ��ת�ɲ���ֵ */
	JAM_API bool jam_lua_to_boolean(lua_t l, int32_t idx);


	/** ��ջ��ָ��λ�õ�Ԫ��ת���û��Զ������� */
	JAM_API void* jam_lua_to_object(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ������� */
	JAM_API bool jam_lua_is_number(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ����ַ��� */
	JAM_API bool jam_lua_is_string(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ��ǲ���ֵ */
	JAM_API bool jam_lua_is_boolean(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ����Զ������� */
	JAM_API bool jam_lua_is_object(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ��Ǳ� */
	JAM_API bool jam_lua_is_table(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ��Ǻ��� */
	JAM_API bool jam_lua_is_function(lua_t l, int32_t idx);


	/** ���ջ��ָ��λ�õ�Ԫ���Ƿ��ǿ� */
	JAM_API bool jam_lua_is_null(lua_t l, int32_t idx);


	/** ��ȡȫ��Ԫ��(����, ������) */
	JAM_API void jam_lua_get_global(lua_t l, const char* name);


	/** ����ȫ��Ԫ��(����, ������) */
	JAM_API void jam_lua_set_global(lua_t l, const char* name);


	/** ע��һ��C���� */
	JAM_API void jam_lua_reg_func(lua_t l, const char* name, lua_func_t func);


	/** ׼������һ��LUA���� */
	JAM_API bool jam_lua_begin_call(lua_t l, const char* func);


	/** ����һ��LUA���� */
	JAM_API bool jam_lua_end_call(lua_t l, int32_t param, int32_t ret);


	/** ��ȡһ�����ֱ��� */
	JAM_API double jam_lua_get_number(lua_t l, const char* name);


	/** ��ȡһ���ַ������� */
	JAM_API const char* jam_lua_get_string(lua_t l, const char* name);


	/** ��ȡһ���������� */
	JAM_API bool jam_lua_get_boolean(lua_t l, const char* name);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_LUA_H__ */

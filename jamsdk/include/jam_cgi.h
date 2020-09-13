
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Easy CGI Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CGI_H__
#define __JAM_CGI_H__

#include "jam_datatype.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef void (*on_page_t)();


	/** ���� FastCGI ���� */
	JAM_API int32_t jam_cgi_run(on_page_t on_page);


	/** ������ݵ������ */
	JAM_API void jam_cgi_output(const char* format, ...);
	/** ������ݵ������ */
	JAM_API void jam_cgi_voutput(const char* format, va_list argv);


	/** HTTP Э��ͷ */
	JAM_API void jam_cgi_header();


	/** HTTP �ض���, ֮ǰ��������κ����ݵ������ */
	JAM_API void jam_cgi_redirect(const char* url);


	/** ����һ����ҳ */
	JAM_API bool jam_cgi_include(const char* file);


	/** ��ȡҳ����� */
	JAM_API const char* jam_cgi_param(const char* name);


	/** ��� COOKIE, time ��λ: ��, Ĭ�� 0 */
	JAM_API bool jam_cgi_cookie_put(const char* name, const char* value, uint32_t time);


	/** ��ѯ COOKIE */
	JAM_API const char* jam_cgi_cookie_get(const char* name);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CGI_H__ */

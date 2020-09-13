
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Trace Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_TRACE_H__
#define __JAM_TRACE_H__

#include "jam_datatype.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t						trace_t;


	/** ����һ����־��� */
	JAM_API trace_t jam_trace_alloc(const char* filename);


	/** �ر���־��� */
	JAM_API void jam_trace_free(trace_t trace);


	/** ��־�Ƿ��������ʱ�� */
	JAM_API void jam_trace_enable_time(trace_t trace, bool enable);


	/** ������־��¼ */
	JAM_API void jam_trace(trace_t trace, const char* prefix, const char* format, ...);
	/** ������־��¼ */
	JAM_API void jam_vtrace(trace_t trace, const char* prefix, const char* format, va_list argv);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_TRACE_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Log System Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_LG_H__
#define __JAM_LG_H__

#include "jam_trace.h"


#ifdef __cplusplus
extern "C" {
#endif


	/**
		��ʼ��������־ϵͳ
		��־�ļ���ʽ: {path}/{yymm}/{dd}/{name}_{hh}_{mm}_{ss}_{ms}.log}
	 */
	JAM_API bool jam_lg_init(const char* path, const char* name, size_t len);


	/** ������־ϵͳ */
	JAM_API bool jam_lg_cleanup();


	/** ��־�Ƿ��������ʱ�� */
	JAM_API void jam_lg_enable_time(bool enable);


	/** ��ӡ��־ */
	JAM_API void jam_lg_enable_print(bool prefix, bool trace);


	/** ��־�ļ��� */
	JAM_API const char* jam_lg_filename();


	/** ������־��¼ */
	JAM_API void jam_lg(const char* prefix, const char* format, ...);
	/** ������־��¼ */
	JAM_API void jam_vlg(const char* prefix, const char* format, va_list argv);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_LG_H__ */

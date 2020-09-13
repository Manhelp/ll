
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Console Utility Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CONSOLE_H__
#define __JAM_CONSOLE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t					csl_color_t;

	#define JAM_FG_RED					(1 << 0)		/** ����̨��ɫ���� */
	#define JAM_FG_GREEN				(1 << 1)		/** ����̨��ɫ���� */
	#define JAM_FG_BLUE					(1 << 2)		/** ����̨��ɫ���� */
	#define JAM_BG_RED					(1 << 3)		/** ����̨��ɫ���� */
	#define JAM_BG_GREEN				(1 << 4)		/** ����̨��ɫ���� */
	#define JAM_BG_BLUE					(1 << 5)		/** ����̨��ɫ���� */



	/** ���ÿ���̨�ϵĹرհ�ť */
	JAM_API void jam_console_no_close();


	/** ���ÿ���̨ CTRL + C �ж� */
	JAM_API void jam_console_no_ctrl_c();


	/** ���ÿ���̨���� */
	JAM_API void jam_console_set_title(const char* title);


	/** ��ȡ����̨���� */
	JAM_API size_t jam_console_get_title(char* title, size_t len);


	/** ���ÿ���̨������ɫ */
	JAM_API void jam_console_set_color(csl_color_t color);


	/** ��ȡ����̨������ɫ */
	JAM_API csl_color_t jam_console_get_color();


	/** �ӿ���̨��ȡ������Ϣ */
	JAM_API size_t jam_console_line(char* buffer, size_t len, bool print);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CONSOLE_H__ */

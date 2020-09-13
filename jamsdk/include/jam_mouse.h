
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Mouse Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MOUSE_H__
#define __JAM_MOUSE_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** ����������� */
	JAM_API void jam_cursor_pos(int32_t x, int32_t y);


	/** ��ȡ������� X �� */
	JAM_API int32_t jam_cursor_x();


	/** ��ȡ������� Y �� */
	JAM_API int32_t jam_cursor_y();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MOUSE_H__ */

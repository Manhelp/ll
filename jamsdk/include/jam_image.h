
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Image Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_IMAGE_H__
#define __JAM_IMAGE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_IMG_BMP = 1,
		JAM_IMG_TGA
	}image_type_t;

	typedef handle_t				image_t;


	/** ��ȡһ��ͼƬ */
	JAM_API image_t jam_img_alloc(const char* filename, image_type_t type);


	/** �ر�һ��ͼƬ */
	JAM_API void jam_img_free(image_t img);


	/** ��ȡͼƬ��� */
	JAM_API int32_t jam_img_width(image_t img);


	/** ��ȡͼƬ�߶� */
	JAM_API int32_t jam_img_height(image_t img);


	/** ��ȡ����λ�� */
	JAM_API int32_t jam_img_bpp(image_t img);


	/** ��ͼƬ���浽������ */
	JAM_API bool jam_img_save(image_t img, const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_IMAGE_H__ */

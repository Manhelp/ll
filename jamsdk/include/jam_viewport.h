
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Viewport Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_VIEWPORT_H__
#define __JAM_VIEWPORT_H__

#include "jam_camera.h"
#include "jam_color.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				viewport_t;


	#define JAM_VP_CLEAR_COLOR							(1 << 0)
	#define JAM_VP_CLEAR_DEPTH							(1 << 1)



	/** �����ӿڱ���ɫ */
	JAM_API void jam_vp_set_background_color(viewport_t vp, color_t argb_clr);


	/** ��ȡ�ӿڱ���ɫ */
	JAM_API color_t jam_vp_get_background_color(viewport_t vp);


	/** �����ӿ���Ⱦǰ���� */
	JAM_API void jam_vp_set_clear_frame(viewport_t vp, uint32_t cf);


	/** ��ȡ�ӿ�������ʽ */
	JAM_API uint32_t jam_vp_get_clear_frame(viewport_t vp);


	/** �����ӿڵ������ */
	JAM_API void jam_vp_set_camera(viewport_t vp, camera_t cam);


	/** ��ȡ�ӿڵ������ */
	JAM_API camera_t jam_vp_get_camera(viewport_t vp);


	/** ��ȡ�ӿڵ� Z ���� */
	JAM_API int32_t jam_vp_z_order(viewport_t vp);


	/** ��ȡ�ӿ�������� (0.0f - 1.0f) */
	JAM_API float jam_vp_left(viewport_t vp);


	/** ��ȡ�ӿڶ������� (0.0f - 1.0f) */
	JAM_API float jam_vp_top(viewport_t vp);


	/** ��ȡ�ӿڿ�� (0.0f - 1.0f) */
	JAM_API float jam_vp_width(viewport_t vp);


	/** ��ȡ�ӿڸ߶� (0.0f - 1.0f) */
	JAM_API float jam_vp_height(viewport_t vp);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_VIEWPORT_H__ */

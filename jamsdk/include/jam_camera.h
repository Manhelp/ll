
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Camera Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CAMERA_H__
#define __JAM_CAMERA_H__

#include "jam_vector.h"
#include "jam_quat.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_CAM_PERSPECTIVE,
		JAM_CAM_ORTHO
	}cam_type_t;

	typedef handle_t				camera_t;


	/** ����һ������� */
	JAM_API camera_t jam_cam_alloc();


	/** �ͷ��������Դ */
	JAM_API void jam_cam_free(camera_t cam);


	/** ���������λ�� */
	JAM_API void jam_cam_set_position(camera_t cam, const vec3_t pos);


	/** ��ȡ�����λ�� */
	JAM_API void jam_cam_get_position(camera_t cam, vec3_t out);


	/** ������������� */
	JAM_API void jam_cam_set_direction(camera_t cam, const vec3_t dir);


	/** ��ȡ��������� */
	JAM_API void jam_cam_get_direction(camera_t cam, vec3_t out);


	/** �ƶ������ */
	JAM_API void jam_cam_move(camera_t cam, const vec3_t pos);


	/** �Զ�������������� */
	JAM_API void jam_cam_lookat(camera_t cam, const vec3_t target);


	/** Z ����ת */
	JAM_API void jam_cam_roll(camera_t cam, float angle);


	/** Y ����ת */
	JAM_API void jam_cam_yaw(camera_t cam, float angle);


	/** X ����ת */
	JAM_API void jam_cam_pitch(camera_t cam, float angle);


	/** ��ת */
	JAM_API void jam_cam_rotate(camera_t cam, const quat_t q);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CAMERA_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Graphics Math Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_XMATH_H__
#define __JAM_XMATH_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** �����Ӻ������ཻ */
	JAM_API bool jam_intersect_box_sphere(const box_t* b, const sphere_t* s);


	/** ���ƽ��׵��ͺ����ཻ */
	JAM_API bool jam_intersect_frustum_box(const frustum_t* f, const box_t* b);


	/** ���ƽ��׵��������ཻ */
	JAM_API bool jam_intersect_frustum_sphere(const frustum_t* f, const sphere_t* s);


	/** �������������ཻ, distance ���ؽ��㵽�������ľ��� */
	JAM_API bool jam_intersect_ray_box(float* distance, const ray_t* r, const box_t* b);


	/** ��������������ཻ, distance ���ؽ��㵽�������ľ��� */
	JAM_API bool jam_intersect_ray_sphere(float* distance, const ray_t* r, const sphere_t* s);


	/** ������������ཻ, distance ���ؽ��㵽�������ľ��� */
	JAM_API bool jam_intersect_ray_plane(float* distance, const ray_t* r, const plane_t* p);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_XMATH_H__ */

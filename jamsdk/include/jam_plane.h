
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Plane Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_PLANE_H__
#define __JAM_PLANE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_SIDE_NO, 
		JAM_SIDE_FRONT, 
		JAM_SIDE_BACK, 
		JAM_SIDE_BOTH
	}side_type_t;


	/** ͨ������������һ��ƽ�� */
	JAM_API void jam_plane_from_point(plane_t* plane, const vec3_t p1, const vec3_t p2, const vec3_t p3);


	/** ͨ��һ�����ߺ�һ��������һ��ƽ�� */
	JAM_API void jam_plane_from_normal(plane_t* plane, const vec3_t norm, const vec3_t p);


	/** ƽ���һ�� */
	JAM_API void jam_plane_normalize(plane_t* out, const plane_t* p);


	/** ƽ�浽��ľ��� */
	JAM_API float jam_plane_distance(const plane_t* plane, const vec3_t p);


	/** ������ƽ���λ�� */
	JAM_API side_type_t jam_plane_side_point(const plane_t* plane, const vec3_t p);


	/** ��������ƽ���λ�� */
	JAM_API side_type_t jam_plane_side_box(const plane_t* plane, const box_t* b);


	/** ���������ƽ���λ�� */
	JAM_API side_type_t jam_plane_side_sphere(const plane_t* plane, const sphere_t* s);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_PLANE_H__ */

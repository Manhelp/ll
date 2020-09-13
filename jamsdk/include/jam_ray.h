
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Ray Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_RAY_H__
#define __JAM_RAY_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** �����������ཻ, distance ���ؽ��㵽�������ľ��� */
	JAM_API bool jam_intersect_ray_point(float* distance, const ray_t* r, const vec3_t p);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_RAY_H__ */

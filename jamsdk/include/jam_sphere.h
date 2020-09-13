
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Sphere Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SPHERE_H__
#define __JAM_SPHERE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** ���һ�����Ƿ��������� */
	JAM_API bool jam_sphere_point_in(const sphere_t* s, const vec3_t p);


	/** ������������Ƿ��ཻ */
	JAM_API bool jam_sphere_intersect(const sphere_t* s1, const sphere_t* s2);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SPHERE_H__ */

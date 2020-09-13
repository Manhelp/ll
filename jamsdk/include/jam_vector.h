
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Vector Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_VECTOR_H__
#define __JAM_VECTOR_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** �ж����������Ƿ���� */
	JAM_API bool jam_vec_compare(const vec_t* a, const vec_t* b, int32_t count);


	/** �������� */
	JAM_API void jam_vec_zero(vec_t* inout, int32_t count);


	/** ����ȡ�� */
	JAM_API void jam_vec_inverse(vec_t* inout, int32_t count);


	/** �������� */
	JAM_API void jam_vec_copy(vec_t* out, const vec_t* in, int32_t count);


	/** ������� */
	JAM_API void jam_vec_add(vec_t* out, const vec_t* a, const vec_t* b, int32_t count);


	/** ������� */
	JAM_API void jam_vec_sub(vec_t* out, const vec_t* a, const vec_t* b, int32_t count);


	/** ������� */
	JAM_API void jam_vec_mul(vec_t* out, const vec_t* a, const vec_t* b, int32_t count);


	/** �������� */
	JAM_API void jam_vec_scale(vec_t* out, const vec_t* a, float s, int32_t count);


	/** ��� */
	JAM_API float jam_vec_dot(const vec_t* a, const vec_t* b, int32_t count);


	/** ��������ƽ�� */
	JAM_API float jam_vec_length_sqr(const vec_t* a, int32_t count);


	/** �������� */
	#define jam_vec_length(a, c)							((float)jam_sqrt(jam_vec_length_sqr(a, c)))


	/** ������һ�� */
	JAM_API void jam_vec_normalize(vec_t* out, const vec_t* a, int32_t count);


	/** ���������ı任 */
	JAM_API void jam_vec_transform_mat(vec4_t out, const vec_t* a, const mat44_t m, int32_t count);


	/** ��������Ԫ���ı任 */
	JAM_API void jam_vec3_trans_quat(vec3_t out, const vec3_t v, const quat_t q);


	/** ��ά������� */
	JAM_API void jam_vec3_cross(vec3_t out, const vec3_t a, const vec3_t b);


	/** ��ά������� */
	JAM_API void jam_vec4_cross(vec4_t out, const vec4_t a, const vec4_t b, const vec4_t c);


	/*
	JAM_API void jam_vec3_project(vec3_t out);
	JAM_API void jam_vec3_unproject(vec3_t out);
	*/


	/** ��ά�����Ƚ� */
	#define jam_vec2_compare(a, b)							jam_vec_compare(a, b, 2)
	/** ��ά�����Ƚ� */
	#define jam_vec3_compare(a, b)							jam_vec_compare(a, b, 3)
	/** ��ά�����Ƚ� */
	#define jam_vec4_compare(a, b)							jam_vec_compare(a, b, 4)


	/** ��ά�������� */
	#define jam_vec2_zero(v)								jam_vec_zero(v, 2)
	/** ��ά�������� */
	#define jam_vec3_zero(v)								jam_vec_zero(v, 3)
	/** ��ά�������� */
	#define jam_vec4_zero(v)								jam_vec_zero(v, 4)


	/** ��ά����ȡ�� */
	#define jam_vec2_inverse(v)								jam_vec_inverse(v, 2)
	/** ��ά����ȡ�� */
	#define jam_vec3_inverse(v)								jam_vec_inverse(v, 3)
	/** ��ά����ȡ�� */
	#define jam_vec4_inverse(v)								jam_vec_inverse(v, 4)


	/** ��ά�������� */
	#define jam_vec2_copy(out, a)							jam_vec_copy(out, a, 2)
	/** ��ά�������� */
	#define jam_vec3_copy(out, a)							jam_vec_copy(out, a, 3)
	/** ��ά�������� */
	#define jam_vec4_copy(out, a)							jam_vec_copy(out, a, 4)


	/** ��ά������� */
	#define jam_vec2_add(out, a, b)							jam_vec_add(out, a, b, 2)
	/** ��ά������� */
	#define jam_vec3_add(out, a, b)							jam_vec_add(out, a, b, 3)
	/** ��ά������� */
	#define jam_vec4_add(out, a, b)							jam_vec_add(out, a, b, 4)


	/** ��ά������� */
	#define jam_vec2_sub(out, a, b)							jam_vec_sub(out, a, b, 2)
	/** ��ά������� */
	#define jam_vec3_sub(out, a, b)							jam_vec_sub(out, a, b, 3)
	/** ��ά������� */
	#define jam_vec4_sub(out, a, b)							jam_vec_sub(out, a, b, 4)


	/** ��ά������� */
	#define jam_vec2_mul(out, a, b)							jam_vec_mul(out, a, b, 2)
	/** ��ά������� */
	#define jam_vec3_mul(out, a, b)							jam_vec_mul(out, a, b, 3)
	/** ��ά������� */
	#define jam_vec4_mul(out, a, b)							jam_vec_mul(out, a, b, 4)


	/** ��ά�������� */
	#define jam_vec2_scale(out, a, s)						jam_vec_scale(out, a, s, 2)
	/** ��ά�������� */
	#define jam_vec3_scale(out, a, s)						jam_vec_scale(out, a, s, 3)
	/** ��ά�������� */
	#define jam_vec4_scale(out, a, s)						jam_vec_scale(out, a, s, 4)


	/** ��ά������� */
	#define jam_vec2_dot(a, b)								jam_vec_dot(a, b, 2)
	/** ��ά������� */
	#define jam_vec3_dot(a, b)								jam_vec_dot(a, b, 3)
	/** ��ά������� */
	#define jam_vec4_dot(a, b)								jam_vec_dot(a, b, 4)


	/** ��ά��������ƽ�� */
	#define jam_vec2_len_sqr(a)								jam_vec_length_sqr(a, 2)
	/** ��ά��������ƽ�� */
	#define jam_vec3_len_sqr(a)								jam_vec_length_sqr(a, 3)
	/** ��ά��������ƽ�� */
	#define jam_vec4_len_sqr(a)								jam_vec_length_sqr(a, 4)


	/** ��ά�������� */
	#define jam_vec2_len(a)									jam_vec_length(a, 2)
	/** ��ά�������� */
	#define jam_vec3_len(a)									jam_vec_length(a, 3)
	/** ��ά�������� */
	#define jam_vec4_len(a)									jam_vec_length(a, 4)


	/** ��ά������һ�� */
	#define jam_vec2_norm(out, a)							jam_vec_normalize(out, a, 2)
	/** ��ά������һ�� */
	#define jam_vec3_norm(out, a)							jam_vec_normalize(out, a, 3)
	/** ��ά������һ�� */
	#define jam_vec4_norm(out, a)							jam_vec_normalize(out, a, 4)


	/** ��ά���������ı任 */
	#define jam_vec2_trans_mat(out, a, m)					jam_vec_transform_mat(out, a, m, 2)
	/** ��ά���������ı任 */
	#define jam_vec3_trans_mat(out, a, m)					jam_vec_transform_mat(out, a, m, 3)
	/** ��ά���������ı任 */
	#define jam_vec4_trans_mat(out, a, m)					jam_vec_transform_mat(out, a, m, 4)


	/** ��ȡ���������ĽǶ�(��λ: ����) */
	#define jam_vec3_angle(a, b)							((float)jam_acos(jam_vec3_dot(a, b) / (jam_vec3_len(a) * jam_vec3_len(b))))



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_VECTOR_H__ */

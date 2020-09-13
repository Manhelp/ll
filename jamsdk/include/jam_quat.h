
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Quaternion Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_QUAT_H__
#define __JAM_QUAT_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** ��Ԫ����� */
	JAM_API void jam_quat_mul(quat_t out, const quat_t a, const quat_t b);


	/** ��Ԫ��ָ�� */
	JAM_API void jam_quat_exp(quat_t out, const quat_t a);


	/** ��Ԫ������ */
	JAM_API void jam_quat_log(quat_t out, const quat_t a);


	/** ��Ԫ���� */
	JAM_API void jam_quat_pow(quat_t out, const quat_t a, float t);


	/** ��Ԫ�����Բ�ֵ */
	JAM_API void jam_quat_slerp(quat_t out, const quat_t a, const quat_t b, float bal);


	/** ͨ�������ͽǶȼ�����Ԫ��, ���������ǹ�һ���� */
	JAM_API void jam_quat_from_angle(quat_t out, const vec3_t v, float angle);


	/** ͨ����Ԫ����ȡ�����ͽǶ� */
	JAM_API void jam_quat_to_angle(vec3_t v, float* angle, const quat_t q);


	/** ͨ����ת�������Ԫ�� */
	JAM_API void jam_quat_from_yaw_pitch_roll(quat_t out, float yaw, float pitch, float roll);


	/** ͨ����Ԫ����ȡ��ת�� */
	JAM_API void jam_quat_to_yaw_pitch_roll(float* yaw, float* pitch, float* roll, const quat_t q);


	/** ͨ����ת�������Ԫ��, ���������ǹ�һ���� */
	JAM_API void jam_quat_from_axes(quat_t out, const vec3_t x, const vec3_t y, const vec3_t z);


	/** ͨ����Ԫ����ȡ��ת�� */
	JAM_API void jam_quat_to_axes(vec3_t x, vec3_t y, vec3_t z, const quat_t q);


	/** ��Ԫ��ת���� */
	JAM_API void jam_quat_to_mat(mat44_t out, const quat_t q);


	/** ��Ԫ������ */
	#define jam_quat_conj(out, a)							do{out[0] = -a[0]; out[1] = -a[1]; out[2] = -a[2]; out[3] = a[3];}while(0)


	/** ��Ԫ���Ƚ� */
	#define jam_quat_compare(a, b)							jam_vec4_compare(a, b)


	/** ��Ԫ������ */
	#define jam_quat_zero(v)								jam_vec4_zero(v)


	/** ��Ԫ��ȡ�� */
	#define jam_quat_inverse(out, a)						do{jam_quat_norm(out, a); jam_quat_conj(out, out);}while(0)


	/** ��Ԫ������ */
	#define jam_quat_copy(out, a)							jam_vec4_copy(out, a)


	/** ��Ԫ����� */
	#define jam_quat_add(out, a, b)							jam_vec4_add(out, a, b)


	/** ��Ԫ����� */
	#define jam_quat_sub(out, a, b)							jam_vec4_sub(out, a, b)


	/** ��Ԫ����� */
	#define jam_quat_dot(a, b)								jam_vec4_dot(a, b)


	/** ��Ԫ������ */
	#define jam_quat_len_sqr(a)								jam_vec4_len_sqr(a)


	/** ��Ԫ�����ȵ�ƽ���� */
	#define jam_quat_len(a)									jam_vec4_len(a)


	/** ��Ԫ����һ�� */
	#define jam_quat_norm(out, a)							jam_vec4_norm(out, a)


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_QUAT_H__ */

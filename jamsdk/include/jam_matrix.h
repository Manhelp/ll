
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Matrix Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MATRIX_H__
#define __JAM_MATRIX_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** �Ƚ����������Ƿ���� */
	JAM_API bool jam_mat_compare(const mat44_t a, const mat44_t b);


	/** �������� */
	JAM_API void jam_mat_zero(mat44_t mat);


	/** ������ */
	JAM_API void jam_mat_copy(mat44_t out, const mat44_t in);


	/** �Ƿ�Ϊ��λ���� */
	JAM_API bool jam_mat_is_identity(mat44_t mat);


	/** ���õ�λ���� */
	JAM_API void jam_mat_set_identity(mat44_t mat);


	/** ������� */
	JAM_API void jam_mat_add(mat44_t out, const mat44_t a, const mat44_t b);


	/** ������� */
	JAM_API void jam_mat_sub(mat44_t out, const mat44_t a, const mat44_t b);


	/** ������� */
	JAM_API void jam_mat_mul(mat44_t out, const mat44_t a, const mat44_t b);


	/** ��������ά������� */
	JAM_API void jam_mat_mul_vec3(vec3_t out, const mat44_t a, const vec3_t b);


	/** ��������ά������� */
	JAM_API void jam_mat_mul_vec4(vec4_t out, const mat44_t a, const vec4_t b);


	/** �������� */
	JAM_API void jam_mat_scale(mat44_t out, const mat44_t a, float s);


	/** ����� */
	JAM_API void jam_mat_inverse(mat44_t out, const mat44_t a);


	/** ������� */
	JAM_API void jam_mat_adjoint(mat44_t out, const mat44_t a);


	/** ���л��� */
	JAM_API void jam_mat_transpose(mat44_t out, const mat44_t mat);


	/** ����ʽ */
	JAM_API float jam_mat_determinant(const mat44_t mat);


	/** �������ž��� */
	JAM_API void jam_mat_set_scaling(mat44_t out, const vec3_t v);


	/** ����ƽ�ƾ��� */
	JAM_API void jam_mat_set_translation(mat44_t out, const vec3_t v);


	/** ͨ����ת��������, ���������ǹ�һ���� */
	JAM_API void jam_mat_from_angle(mat44_t out, const vec3_t axis, float angle);


	/** ͨ�������ȡ�����ͽǶ� */
	JAM_API void jam_mat_to_angle(vec3_t v, float* angle, const mat44_t mat);


	/** ͨ����ת�������� */
	JAM_API void jam_mat_from_yaw_pitch_roll(mat44_t out, float yaw, float pitch, float roll);


	/** ͨ�������ȡ��ת�� */
	JAM_API void jam_mat_to_yaw_pitch_roll(float* yaw, float* pitch, float* roll, const mat44_t mat);


	/** ͨ����ת��������, ���������ǹ�һ���� */
	JAM_API void jam_mat_from_axes(mat44_t out, const vec3_t x, const vec3_t y, const vec3_t z);


	/** ͨ�������ȡ��ת�� */
	JAM_API void jam_mat_to_axes(vec3_t x, vec3_t y, vec3_t z, const mat44_t mat);


	/** ����ת��Ԫ�� */
	JAM_API void jam_mat_to_quat(quat_t out, const mat44_t mat);


	/** ��ȡ�ӽǾ��� */
	JAM_API void jam_mat_from_lookat(mat44_t out, const vec3_t eye, const vec3_t at, const vec3_t up);


	/** ��ȡ͸���Ӿ������ */
	JAM_API void jam_mat_from_perspective(mat44_t out, float w, float h, float nearp, float farp);


	/** ��ȡ͸���Ӿ������ */
	JAM_API void jam_mat_from_perspective_center(mat44_t out, float left, float right, float bottom, float top, float nearp, float farp);


	/** ��ȡ͸���Ӿ������ */
	JAM_API void jam_mat_from_perspective_fov(mat44_t out, float fov, float aspect, float nearp, float farp);


	/** ��ȡƽ���Ӿ������ */
	JAM_API void jam_mat_from_ortho(mat44_t out, float w, float h, float nearp, float farp);


	/** ��ȡƽ���Ӿ������ */
	JAM_API void jam_mat_from_ortho_center(mat44_t out, float left, float right, float bottom, float top, float nearp, float farp);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MATRIX_H__ */

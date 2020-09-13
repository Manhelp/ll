
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Math Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MATH_H__
#define __JAM_MATH_H__


#include "jam_datatype.h"
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_PI											3.14159265358979323846
	#define JAM_EPSILON										(2.2204460492503131e-016)
	#define JAM_DEG2RAD										0.0174532925199432957692		/* JAM_PI / 180.0f */
	#define JAM_RAD2DEG										57.2957795130823208768464		/* 180.0f / JAM_PI */


	/** ��Сֵ */
	#define jam_min(a, b)									((a) < (b) ? (a) : (b))


	/** ���ֵ */
	#define jam_max(a, b)									((a) > (b) ? (a) : (b))


	/** ���� a �� b ��ֵ, t ����ʱ���� */
	#define jam_swap(t, a, b)								do{(t) = (a); (a) = (b); (b) = (t);}while(0)


	/** �޶� v ��ֵ�� a �� b ֮�� */
	#define jam_clamp(v, min_v, max_v)						jam_max(min_v, jam_min(max_v, v))


	/** �������Բ�ֵ */
	#define jam_lerp(min_v, max_v, bal)						((min_v) + (((max_v) - (min_v)) * jam_clamp(bal, 0.0, 1.0)))


	/** �ж��Ƿ�Ϊ���� */
	#define jam_is_odd(x)									(1 == ((x) & 1))


	/** �ж��Ƿ�Ϊż�� */
	#define jam_is_even(x)									(0 == ((x) & 1))


	/** ƽ�� */
	#define jam_sqr(x)										((x) * (x))


	/** ���� */
	#define jam_cube(x)										(jam_sqr(x) * (x))


	/** ƽ���� */
	#define jam_sqrt(x)										sqrt(x)


	/** ƽ�������� */
	#define jam_rsqrt(x)									(1.0 / jam_sqrt(x))


	/** ������ȡģ */
	#define jam_fmod(x, y)									fmod(x, y)


	/** С�� x ֵ��������� */
	#define jam_floor(x)									floor(x)


	/** ���� x ֵ����С���� */
	#define jam_ceil(x)										ceil(x)


	/** �� */
	#define jam_pow(x, y)									pow(x, y)


	/** ָ�� */
	#define jam_exp(x)										exp(x)


	/** ���� */
	#define jam_log(x)										log(x)
	#define jam_log10(x)									log10(x)


	/** ���� */
	#define jam_sin(x)										sin(x)


	/** ���� */
	#define jam_cos(x)										cos(x)


	/** ���� */
	#define jam_tan(x)										tan(x)


	/** ˫������ */
	#define jam_sinh(x)										sinh(x)


	/** ˫������ */
	#define jam_cosh(x)										cosh(x)


	/** ˫������ */
	#define jam_tanh(x)										tanh(x)


	/** ������ */
	#define jam_asin(x)										asin(x)


	/** ������ */
	#define jam_acos(x)										acos(x)


	/** ������ */
	#define jam_atan(x)										atan(x)


	/** ������ (y / x) */
	#define jam_atan2(y, x)									atan2(y, x)

	
	/** �Ƕ�ת���� */
	#define jam_deg_to_rad(deg)								((deg) * JAM_DEG2RAD)


	/** ����ת�Ƕ� */
	#define jam_rad_to_deg(rad)								((rad) * JAM_RAD2DEG)


	/** �Ƚ�2���������Ƿ���� */
	#define jam_fcmp(a, b)									(jam_fabs((a) - (b)) < JAM_EPSILON)


	/** С��2��n�η���������� */
	JAM_API uint32_t jam_next2pow(uint32_t n);


	/** ��һ���������λλ�� */
	JAM_API int32_t jam_high_bit_pos(uint32_t n);


	/** ����ֵ */
	#define jam_abs(x)										(((x) < 0) ? (-x) : (x))


	/** ���Ϳ��پ���ֵ */
	JAM_API int32_t jam_labs(int32_t num);


	/** ������پ���ֵ */
	JAM_API float jam_fabs(float num);


	/** ����� */
	JAM_API int32_t jam_rand();


	/** ��ָ����Χ�ڲ��������.( min_v < x < max_v ) */
	#define jam_random(min_v, max_v)						((jam_rand() % jam_labs(max_v - min_v)) + min_v)


	/** ���������.(0.0f - 1.0f) */
	#define jam_rand_f()									(((float)jam_rand()) / 0x80000000)


	/** ��ָ����Χ�ڲ������������.( min_v < x < max_v ) */
	#define jam_random_f(min_v, max_v)						((jam_rand_f() * jam_fabs(max_v - min_v)) + min_v)


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MATH_H__ */

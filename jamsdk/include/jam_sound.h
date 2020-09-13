
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Sound Runtime Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SOUND_H__
#define __JAM_SOUND_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t						sound_t;

	typedef enum
	{
		JAM_SOUND_WAV = 1,
		JAM_SOUND_OGG,
		JAM_SOUND_MP3
	}sound_type_t;

	typedef enum
	{
		JAM_DISTANCE_MODEL = 1,
		JAM_INVERSE_DISTANCE,
		JAM_INVERSE_DISTANCE_CLAMPED,
		JAM_LINEAR_DISTANCE,
		JAM_LINEAR_DISTANCE_CLAMPED,
		JAM_EXPONENT_DISTANCE,
		JAM_EXPONENT_DISTANCE_CLAMPED
	}distance_model_t;

	typedef enum
	{
		JAM_SOUND_NO_ERROR = 0,
		JAM_SOUND_INVALID_NAME,
		JAM_SOUND_INVALID_ENUM,
		JAM_SOUND_INVALID_VALUE,
		JAM_SOUND_INVALID_COMMAND,
		JAM_SOUND_OUT_OF_MEMORY,
		JAM_SOUND_UNKNOWN
	}sound_error_t;



	/** ��ʼ�������豸 */
	JAM_API bool jam_sound_init();


	/** �ر������豸 */
	JAM_API bool jam_sound_cleanup();


	/** ��ȡ����������Ϣ */
	JAM_API sound_error_t jam_sound_last_error();


	/** ��ȡ�������� */
	JAM_API const char* jam_sound_vendor();


	/** ��ȡ�����汾 */
	JAM_API const char* jam_sound_version();


	/** ��ȡ�������� */
	JAM_API const char* jam_sound_renderer();


	/** ��ȡ������չ */
	JAM_API const char* jam_sound_extensions();


	/** ���ļ�����һ��������� */
	JAM_API sound_t jam_sound_alloc(sound_type_t type, const char* filename);


	/** ���ڴ��һ���������, ��֧�� WAVE */
	JAM_API sound_t jam_sound_memory_alloc(sound_type_t type, const byte_t* buffer, size_t len);


	/** �ر�������� */
	JAM_API void jam_sound_free(sound_t snd);


	/** ��ȡ�������� */
	JAM_API sound_type_t jam_sound_type(sound_t snd);


	/** ���� */
	JAM_API bool jam_sound_play(sound_t snd);


	/** ֹͣ */
	JAM_API bool jam_sound_stop(sound_t snd);


	/** ��ͣ */
	JAM_API bool jam_sound_pause(sound_t snd);


	/** �ָ� */
	JAM_API bool jam_sound_resume(sound_t snd);


	/** ���� */
	JAM_API bool jam_sound_rewind(sound_t snd);


	/** ����ѭ������ */
	JAM_API bool jam_sound_loop(sound_t snd, bool loop);


	/** �ȴ������������ */
	JAM_API void jam_sound_sync(sound_t snd);


	/** �Ƿ����ڲ��� */
	JAM_API bool jam_sound_is_playing(sound_t snd);


	/** �Ƿ���ֹͣ */
	JAM_API bool jam_sound_is_stopped(sound_t snd);


	/** �Ƿ�����ͣ */
	JAM_API bool jam_sound_is_paused(sound_t snd);


	/** ��ȡƵ�� */
	JAM_API int32_t jam_sound_get_frequency(sound_t snd);


	/** ��ȡλ�� */
	JAM_API int32_t jam_sound_get_bits(sound_t snd);


	/** ��ȡƵ�� */
	JAM_API int32_t jam_sound_get_channels(sound_t snd);


	/** ������Դλ�� */
	JAM_API void jam_sound_set_source_position(sound_t snd, float x, float y, float z);


	/** ��ȡ��Դλ�� */
	JAM_API void jam_sound_get_source_position(sound_t snd, float* x, float* y, float* z);


	/** ������Դ�ٶ� */
	JAM_API void jam_sound_set_source_velocity(sound_t snd, float x, float y, float z);


	/** ��ȡ��Դ�ٶ� */
	JAM_API void jam_sound_get_source_velocity(sound_t snd, float* x, float* y, float* z);


	/** ������Դ���� */
	JAM_API void jam_sound_set_source_direction(sound_t snd, float x, float y, float z);


	/** ��ȡ��Դ���� */
	JAM_API void jam_sound_get_source_direction(sound_t snd, float* x, float* y, float* z);


	/** ������Դλ���Ƿ�����������ϵ */
	JAM_API void jam_sound_set_source_relative(sound_t snd, bool rel);


	/** ��ȡ��Դλ���Ƿ�����������ϵ */
	JAM_API bool jam_sound_get_source_relative(sound_t snd);


	/** ��������λ�� */
	JAM_API void jam_sound_set_listener_position(float x, float y, float z);


	/** ��ȡ����λ�� */
	JAM_API void jam_sound_get_listener_position(float* x, float* y, float* z);


	/** ���������ٶ� */
	JAM_API void jam_sound_set_listener_velocity(float x, float y, float z);


	/** ��ȡ�����ٶ� */
	JAM_API void jam_sound_get_listener_velocity(float* x, float* y, float* z);


	/** �������ڷ��� */
	JAM_API void jam_sound_set_listener_orientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);


	/** ��ȡ���ڷ��� */
	JAM_API void jam_sound_get_listener_orientation(float* at_x, float* at_y, float* at_z, float* up_x, float* up_y, float* up_z);


	/** ������Դ���� */
	JAM_API void jam_sound_set_gain(sound_t snd, float gain);


	/** ��ȡ��Դ���� */
	JAM_API float jam_sound_get_gain(sound_t snd);


	/** ������Դ������� */
	JAM_API void jam_sound_set_max_gain(sound_t snd, float gain);


	/** ��ȡ��Դ������� */
	JAM_API float jam_sound_get_max_gain(sound_t snd);


	/** ������Դ��С���� */
	JAM_API void jam_sound_set_min_gain(sound_t snd, float gain);


	/** ��ȡ��Դ��С���� */
	JAM_API float jam_sound_get_min_gain(sound_t snd);


	/** ������ԴƵ��(Ĭ��Ϊ1.0) */
	JAM_API void jam_sound_set_pitch(sound_t snd, float pitch);


	/** ��ȡ��ԴƵ�� */
	JAM_API float jam_sound_get_pitch(sound_t snd);


	/** ������Դ��rolloff�� */
	JAM_API void jam_sound_set_roll_off(sound_t snd, float rolloff);


	/** ��ȡ��Դ��rolloff�� */
	JAM_API float jam_sound_get_roll_off(sound_t snd);


	/** ���þ���ģ�� */
	JAM_API bool jam_sound_set_distance_model(distance_model_t model);


	/** ������Դ�����ھ����ϼ���һ�� */
	JAM_API void jam_sound_set_reference_distance(sound_t snd, float dis);


	/** ��ȡ��Դ�����ھ����ϼ���һ��ľ��� */
	JAM_API float jam_sound_get_reference_distance(sound_t snd);


	/**
	   ����������
		(used with the Inverse Clamped Distance Model to set the distance 
		where there will no longer be any attenuation of the source)
	 */
	JAM_API void jam_sound_set_max_distance(sound_t snd, float dis);


	/** ��ȡ������ */
	JAM_API float jam_sound_get_max_distance(sound_t snd);


	/** ���ö���Բ׶�ⲿ������ */
	JAM_API void jam_sound_set_cone_outer_gain(sound_t snd, float gain);


	/** ��ȡ����Բ׶�ⲿ������ */
	JAM_API float jam_sound_get_cone_outer_gain(sound_t snd);


	/** ���ö���Բ׶�ڲ������� */
	JAM_API void jam_sound_set_cone_inner_gain(sound_t snd, float gain);


	/** ��ȡ����Բ׶�ڲ������� */
	JAM_API float jam_sound_get_cone_inner_gain(sound_t snd);


	/** ����Բ׶�Ƕ�. Ĭ��360�� */
	JAM_API void jam_sound_set_cone_angle(sound_t snd, float angle);


	/** ��ȡԲ׶�Ƕ� */
	JAM_API float jam_sound_get_cone_angle(sound_t snd);


	/**
	   ���ö���������, Ĭ��: 1.0f
		shift = DOPPLER_FACTOR * freq * (DOPPLER_VELOCITY - listener.velocity) / (DOPPLER_VELOCITY + source.velocity)
	 */
	JAM_API bool jam_sound_set_doppler_factor(float factor);


	/** ���ö������ٶ�, Ĭ��: 343.0 m/sec */
	JAM_API bool jam_sound_set_doppler_velocity(float velocity);


	/** ���������ٶ� */
	JAM_API bool jam_sound_set_speed(float speed);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SOUND_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Joystick Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_JOYSTICK_H__
#define __JAM_JOYSTICK_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t							joystick_t;

	#define JAM_JOY_BUTTON_1					(1 << 0)
	#define JAM_JOY_BUTTON_2					(1 << 1)
	#define JAM_JOY_BUTTON_3					(1 << 2)
	#define JAM_JOY_BUTTON_4					(1 << 3)
	#define JAM_JOY_BUTTON_5					(1 << 4)
	#define JAM_JOY_BUTTON_6					(1 << 5)
	#define JAM_JOY_BUTTON_7					(1 << 6)
	#define JAM_JOY_BUTTON_8					(1 << 7)
	#define JAM_JOY_BUTTON_9					(1 << 8)
	#define JAM_JOY_BUTTON_10					(1 << 9)
	#define JAM_JOY_BUTTON_11					(1 << 10)
	#define JAM_JOY_BUTTON_12					(1 << 11)
	#define JAM_JOY_BUTTON_13					(1 << 12)
	#define JAM_JOY_BUTTON_14					(1 << 13)
	#define JAM_JOY_BUTTON_15					(1 << 14)
	#define JAM_JOY_BUTTON_16					(1 << 15)
	#define JAM_JOY_BUTTON_17					(1 << 16)
	#define JAM_JOY_BUTTON_18					(1 << 17)
	#define JAM_JOY_BUTTON_19					(1 << 18)
	#define JAM_JOY_BUTTON_20					(1 << 19)
	#define JAM_JOY_BUTTON_21					(1 << 20)
	#define JAM_JOY_BUTTON_22					(1 << 21)
	#define JAM_JOY_BUTTON_23					(1 << 22)
	#define JAM_JOY_BUTTON_24					(1 << 23)
	#define JAM_JOY_BUTTON_25					(1 << 24)
	#define JAM_JOY_BUTTON_26					(1 << 25)
	#define JAM_JOY_BUTTON_27					(1 << 26)
	#define JAM_JOY_BUTTON_28					(1 << 27)
	#define JAM_JOY_BUTTON_29					(1 << 28)
	#define JAM_JOY_BUTTON_30					(1 << 29)
	#define JAM_JOY_BUTTON_31					(1 << 30)
	#define JAM_JOY_BUTTON_32					(1 << 31)


	#define JAM_JOY_MAX_AXES					6

	#define JAM_JOY_AXES_X						0
	#define JAM_JOY_AXES_Y						1
	#define JAM_JOY_AXES_Z						2
	#define JAM_JOY_AXES_R						3
	#define JAM_JOY_AXES_U						4
	#define JAM_JOY_AXES_V						5



	/** ��ʼ����Ϸ�������豸 */
	JAM_API bool jam_joy_init();


	/** �Ƿ��ʼ����Ϸ�������豸 */
	JAM_API bool jam_joy_is_init();


	/** ������Ϸ�������豸 */
	JAM_API void jam_joy_cleanup();


	/** ��ȡ��ǰ���������� */
	JAM_API uint32_t jam_joy_num();


	/** ��ȡ���֧�ֿ��������� */
	JAM_API uint32_t jam_joy_max_num();


	/** �������İ������� */
	JAM_API int32_t jam_joy_button_num(int32_t joy_id);


	/** ������������ */
	JAM_API int32_t jam_joy_axis_num(int32_t joy_id);

	/** ���������� */
	JAM_API const char* jam_joy_name(int32_t joy_id);


	/** ������ע����Ϣ */
	JAM_API const char* jam_joy_reg_key(int32_t joy_id);


	/** ������������Ϣ */
	JAM_API const char* jam_joy_oem_vxd(int32_t joy_id);


	/** ��������ǰ������ */
	JAM_API int32_t jam_joy_axis(int32_t joy_id, int32_t axis);


	/** ��������������� */
	JAM_API int32_t jam_joy_max_axis(int32_t joy_id, int32_t axis);


	/** ��������С������ */
	JAM_API int32_t jam_joy_min_axis(int32_t joy_id, int32_t axis);


	/** ���Ƿ񱻰��� */
	JAM_API bool jam_joy_is_down(int32_t joy_id, joystick_t joy_key);


	/** ���Ƿ񱻰�ס(ǰһ֡û��, ��ǰ֡����) */
	JAM_API bool jam_joy_is_press(int32_t joy_id, joystick_t joy_key);


	/** ���Ƿ��ɿ�(ǰһ֡����, ��ǰ֡û��) */
	JAM_API bool jam_joy_is_release(int32_t joy_id, joystick_t joy_key);


	/** ���¿�����״̬ */
	JAM_API void jam_joy_update();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_JOYSTICK_H__ */

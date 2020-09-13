
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Screen Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SCREEN_H__
#define __JAM_SCREEN_H__

#include "jam_camera.h"
#include "jam_viewport.h"


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_STYLE_DEFAULT						0
	#define JAM_STYLE_FULLSCREEN					(1 << 0)
	#define JAM_STYLE_NO_CLOSE						(1 << 1)
	#define JAM_STYLE_NO_MAX						(1 << 2)
	#define JAM_STYLE_NO_MIN						(1 << 3)
	#define JAM_STYLE_NO_BORDER						(1 << 4)
	#define JAM_STYLE_NO_SIZE						(1 << 5)



	/** ÿ֡�ص�, ����Ϊ��ʱ��, ��λ�� */
	typedef void (*on_frame_move_t)(float elapse);
	typedef bool (*on_screen_close_t)();


	/** ��ʼ����Ⱦ�������� */
	JAM_API bool jam_screen_init(uint32_t style, int32_t width, int32_t height);


	/** ������Ⱦ�������� */
	JAM_API bool jam_screen_cleanup();


	/** �ر������� */
	JAM_API void jam_screen_close();


	/** ��Ⱦ���� */
	JAM_API void jam_screen_loop(on_frame_move_t frame_move, on_screen_close_t on_close);



	/** ���������ڱ��� */
	JAM_API void jam_screen_set_title(const char* title);


	/** ��ȡ�����ڱ��� */
	JAM_API const char* jam_screen_get_title();


	/** ��ǰ�������Ƿ񱻼��� */
	JAM_API bool jam_screen_active();


	/** ��ǰ��������ʽ */
	JAM_API uint32_t jam_screen_style();


	/** ��ǰ������ X ���� */
	JAM_API int32_t jam_screen_x();


	/** ��ǰ������ Y ���� */
	JAM_API int32_t jam_screen_y();


	/** ��ǰ�����ڿ�� */
	JAM_API int32_t jam_screen_width();


	/** ��ǰ�����ڸ߶� */
	JAM_API int32_t jam_screen_height();


	/** ��ǰ������ɫ�� */
	JAM_API int32_t jam_screen_color_depth();


	/** ����ֱ��ʿ�� */
	JAM_API int32_t jam_screen_desktop_width();


	/** ����ֱ��ʸ߶� */
	JAM_API int32_t jam_screen_desktop_height();


	/** ����������������� */
	JAM_API void jam_screen_cursor_pos(int32_t x, int32_t y);


	/** ��ȡ������������� X �� */
	JAM_API int32_t jam_screen_cursor_x();


	/** ��ȡ������������� Y �� */
	JAM_API int32_t jam_screen_cursor_y();


	/** ��ȡ���������������� */
	JAM_API int32_t jam_screen_cursor_z();


	/** ��ȡ����֡�� */
	JAM_API float jam_screen_fps();


	/** ���һ���ӿ� */
	JAM_API viewport_t jam_screen_viewport_add(camera_t cam, int32_t z_order, float left, float top, float width, float height);


	/** ��ȡһ���ӿ� */
	JAM_API viewport_t jam_screen_viewport_get(int32_t z_order);


	/** ��λ�û�ȡһ���ӿ� */
	JAM_API viewport_t jam_screen_viewport_pos_get(int32_t idx);


	/** ɾ��һ���ӿ� */
	JAM_API bool jam_screen_viewport_erase(int32_t z_order);


	/** �ӿڸ��� */
	JAM_API uint32_t jam_screen_viewport_size();


	/** ɾ�������ӿ� */
	JAM_API void jam_screen_viewport_clear();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SCREEN_H__ */

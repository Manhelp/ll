
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Datatype Definition
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_DATATYPE_H__
#define __JAM_DATATYPE_H__

#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_VERSION							"JamSDK - 1.8.2"
	#define JAM_COPYRIGHT						"Copyright (c) 2008-2009 Jampe.Net. All Right Reserved."
	#define JAM_HOMEPAGE						"http://jampe.net/"


	/** ���ֽڴ��� */
	#define JAM_LITTLE_ENDIAN					1
	/** ���ֽڴ��� */
	#define JAM_BIG_ENDIAN						2
	/** ��ǰ�ֽڴ��� */
	#if defined(__hppa__)
		#define JAM_CURRENT_ENDIAN				JAM_BIG_ENDIAN
	#elif defined(__m68k__) || defined(mc68000) || defined(_M_M68K)
		#define JAM_CURRENT_ENDIAN				JAM_BIG_ENDIAN
	#elif defined(__MIPS__) && defined(__MISPEB__)
		#define JAM_CURRENT_ENDIAN				JAM_BIG_ENDIAN
	#elif defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC)
		#define JAM_CURRENT_ENDIAN				JAM_BIG_ENDIAN
	#elif defined(__sparc__)
		#define JAM_CURRENT_ENDIAN				JAM_BIG_ENDIAN
	#else
		#define JAM_CURRENT_ENDIAN				JAM_LITTLE_ENDIAN
	#endif


	#if defined(_WIN32) || defined(_WIN64)
		/** Windowsϵͳ */
		#define JAM_WIN							1

		#ifdef _MSC_VER
			#define JAM_HAVE_STD_BOOL			0
		#else
			#define JAM_HAVE_STD_BOOL			1
		#endif

		#ifndef _WIN32_WINNT
			/* Specifies that the minimum required platform is Windows Vista. */
			/* Change this to the appropriate value to target other versions of Windows. */
			#define _WIN32_WINNT				0x0600
		#endif
	#elif defined(__linux__)
		/** Linuxϵͳ */
		#define	JAM_LINUX						1

		#define JAM_HAVE_STD_BOOL				1
	#else
		#error Unsupported OS
	#endif


	#if defined(JAM_WIN)
		#define JAM_EXPORT						__declspec(dllexport)
		#define JAM_IMPORT						__declspec(dllimport)
		#define JAM_LOCAL
	#else
		#define JAM_EXPORT						__attribute__((visibility("default")))
		#define JAM_IMPORT						__attribute__((visibility("default")))
		#define JAM_LOCAL						__attribute__((visibility("hidden")))
		#define JAM_SO_INIT						__attribute__((constructor))
		#define JAM_SO_FINI						__attribute__((destructor))
	#endif


	typedef unsigned char						byte_t;
	typedef signed char							int8_t;
	typedef unsigned char						uint8_t;
	typedef signed short						int16_t;
	typedef unsigned short						uint16_t;
	typedef signed int							int32_t;
	typedef unsigned int						uint32_t;
	typedef signed long long					int64_t;
	typedef unsigned long long					uint64_t;
	typedef void*								handle_t;
	typedef float								vec_t;
	typedef vec_t								vec2_t[2];
	typedef vec_t								vec3_t[3];
	typedef vec_t								vec4_t[4];
	typedef vec4_t								quat_t;
	typedef vec_t								mat44_t[4][4];

	#ifndef __cplusplus
		#if JAM_HAVE_STD_BOOL
			#include <stdbool.h>
		#else
			#ifndef __bool_true_false_are_defined
			#define __bool_true_false_are_defined	1

				#undef bool
				#undef true
				#undef false

				#define bool					uint8_t
				#define true					1
				#define false					0

			#endif	/* __bool_true_false_are_defined */
		#endif	/* JAM_HAVE_STD_BOOL */
	#endif	/* __cplusplus */

	typedef struct
	{
		vec3_t		min;
		vec3_t		max;
	}box_t;

	typedef struct
	{
		vec3_t		center;
		vec_t		radius;
	}sphere_t;

	typedef struct
	{
		vec3_t		normal;
		vec_t		d;
	}plane_t;

	typedef struct
	{
		plane_t		plane[6];
	}frustum_t;

	typedef struct
	{
		vec3_t		origin;
		vec3_t		direction;
	}ray_t;


	/** ����ļ�·������ */
	#define JAM_MAX_PATH						260


	#ifdef JAM_EXPORTS_DLL
		#define JAM_API							JAM_EXPORT
	#else
		#define JAM_API							JAM_IMPORT
	#endif  /* JAM_EXPORTS_DLL */


	/** ȡ��32λ���� */
	#define jam_high_dword(x)					((uint32_t)(((uint64_t)(x)) >> 32))
	/** ȡ��32λ���� */
	#define jam_low_dword(x)					((uint32_t)(((uint64_t)(x)) & 0xFFFFFFFF))
	/** ȡ��16λ���� */
	#define jam_high_word(x)					((uint16_t)(((uint32_t)(x)) >> 16))
	/** ȡ��16λ���� */
	#define jam_low_word(x)						((uint16_t)(((uint32_t)(x)) & 0xFFFF))
	/** ȡ��8λ���� */
	#define jam_high_byte(x)					((uint8_t)(((uint16_t)(x)) >> 8))
	/** ȡ��8λ���� */
	#define jam_low_byte(x)						((uint8_t)(((uint16_t)(x)) & 0xFF))


	/** ����һ��16λ���� */
	#define jam_make_word(hb, lb)				((uint16_t)(((((uint16_t)(hb)) & 0xFF) << 8) | (((uint16_t)(lb)) & 0xFF)))
	/** ����һ��32λ���� */
	#define jam_make_dword(hw, lw)				((uint32_t)(((((uint32_t)(hw)) & 0xFFFF) << 16) | (((uint32_t)(lw)) & 0xFFFF)))
	/** ����һ��64λ���� */
	#define jam_make_qword(hd, ld)				((uint64_t)(((((uint64_t)(hd)) & 0xFFFFFFFF) << 32) | (((uint64_t)(ld)) & 0xFFFFFFFF)))


	/** ����16λ�����ĸߵ�λ�� */
	#define jam_byte_swap16(x)					((uint16_t)((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8)))
	/** ����32λ�����ĸߵ�λ�� */
	#define jam_byte_swap32(x)					((uint32_t)((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >>  8) |	\
														(((x) & 0x0000FF00) <<  8) | (((x) & 0x000000FF) << 24)))
	/** ����64λ�����ĸߵ�λ�� */
	#define jam_byte_swap64(x)					((uint64_t)((((x) & 0xFF00000000000000ull) >> 56) | (((x) & 0x00FF000000000000ull) >> 40) |		\
														(((x) & 0x0000FF0000000000ull) >> 24) | (((x) & 0x000000FF00000000ull) >>  8) |			\
														(((x) & 0x00000000FF000000ull) <<  8) | (((x) & 0x0000000000FF0000ull) << 24) |			\
														(((x) & 0x000000000000FF00ull) << 40) | (((x) & 0x00000000000000FFull) << 56)))


	/** �������м�������ֽڴ��� */
	#if JAM_CURRENT_ENDIAN == JAM_BIG_ENDIAN
		#define jam_to_uint16(x)				(x)
		#define jam_to_uint32(x)				(x)
		#define jam_to_uint64(x)				(x)

	#elif JAM_CURRENT_ENDIAN == JAM_LITTLE_ENDIAN
		#define jam_to_uint16(x)				jam_byte_swap16(x)
		#define jam_to_uint32(x)				jam_byte_swap32(x)
		#define jam_to_uint64(x)				jam_byte_swap64(x)

	#endif  /* JAM_CURRENT_ENDIAN */



	/** �����Ա�����ڽṹ���е�ƫ���� */
	#define jam_offsetof(type, member)			((size_t)&((type*)0)->member)


	/** ͨ����Ա������ȡ�ṹ����׵�ַ */
	#define jam_entry(ptr, type, member)		(type*)((byte_t*)(ptr) - jam_offsetof(type, member))


	/** ���ջ������Ĵ�С */
	#define jam_array_bound(array)				(sizeof(array) / sizeof(*array))


	/** �ɱ�������� */
	#ifndef va_copy
		#ifdef __va_copy
			#define va_copy(dest, src)			__va_copy((dest), (src))
		#else
			#define va_copy(dest, src)			memcpy((&dest), (&src), sizeof(va_list))
		#endif
	#endif


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_DATATYPE_H__ */

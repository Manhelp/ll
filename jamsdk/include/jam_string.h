
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			String Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_STRING_H__
#define __JAM_STRING_H__

#include "jam_datatype.h"
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint8_t							utf8_t;
	typedef uint16_t						utf16_t;
	typedef uint32_t						utf32_t;


#if defined(JAM_WIN)
	#define strcasecmp						_stricmp
	#define strncasecmp						_strnicmp
	#define snprintf						_snprintf
#endif


	/** ����ַ����Ƿ�Ϊ���ִ� */
	JAM_API bool jam_is_numeric(const char* str);


	/** �ַ���תСд */
	JAM_API char* jam_lowercase(char* str);


	/** �ַ���ת��д */
	JAM_API char* jam_uppercase(char* str);


	/** ȥ���ַ������ָ���ֽ� */
	JAM_API char* jam_trim_left(char* str, char ch);


	/** ȥ���ַ����ұ�ָ���ֽ� */
	JAM_API char* jam_trim_right(char* str, char ch);


	/** ȥ���ַ���ָ���ֽ� */
	#define jam_trim(str, ch)					jam_trim_left(jam_trim_right(str, ch), ch)


	/** �ַ���ת������ */
	JAM_API size_t jam_string_to_hex(byte_t* bin, size_t size, const char* str);


	/** ������ת�ַ��� */
	JAM_API size_t jam_hex_to_string(char* str, size_t len, const byte_t* bin, size_t size);


	/** ASCII ת UTF8 */
	JAM_API size_t jam_ascii_to_utf8(utf8_t* utf8, size_t len, const char* ascii, size_t count);
	/** ASCII ת UTF16 */
	JAM_API size_t jam_ascii_to_utf16(utf16_t* utf16, size_t len, const char* ascii, size_t count);
	/** ASCII ת UTF32 */
	JAM_API size_t jam_ascii_to_utf32(utf32_t* utf32, size_t len, const char* ascii, size_t count);

	/** UTF8 ת ASCII */
	JAM_API size_t jam_utf8_to_ascii(char* ascii, size_t len, const utf8_t* utf8, size_t count);
	/** UTF8 ת UTF16 */
	JAM_API size_t jam_utf8_to_utf16(utf16_t* utf16, size_t len, const utf8_t* utf8, size_t count);
	/** UTF8 ת UTF32 */
	JAM_API size_t jam_utf8_to_utf32(utf32_t* utf32, size_t len, const utf8_t* utf8, size_t count);

	/** UTF16 ת ASCII */
	JAM_API size_t jam_utf16_to_ascii(char* ascii, size_t len, const utf16_t* utf16, size_t count);
	/** UTF16 ת UTF8 */
	JAM_API size_t jam_utf16_to_utf8(utf8_t* utf8, size_t len, const utf16_t* utf16, size_t count);
	/** UTF16 ת UTF32 */
	JAM_API size_t jam_utf16_to_utf32(utf32_t* utf32, size_t len, const utf16_t* utf16, size_t count);

	/** UTF32 ת ASCII */
	JAM_API size_t jam_utf32_to_ascii(char* ascii, size_t len, const utf32_t* utf32, size_t count);
	/** UTF32 ת UTF8 */
	JAM_API size_t jam_utf32_to_utf8(utf8_t* utf8, size_t len, const utf32_t* utf32, size_t count);
	/** UTF32 ת UTF16 */
	JAM_API size_t jam_utf32_to_utf16(utf16_t* utf16, size_t len, const utf32_t* utf32, size_t count);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_STRING_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Regular Expressions Function (POSIX Regular Expressions)
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_REGEX_H__
#define __JAM_REGEX_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t			regexp_t;


	/** ���벢����һ��������ʽ��� */
	JAM_API regexp_t jam_regex_alloc(const char* pattern);


	/** �ر�������ʽ��� */
	JAM_API void jam_regex_free(regexp_t reg);


	/** ƥ��������ʽ */
	JAM_API bool jam_regex_match(regexp_t reg, int32_t* bof, int32_t* eof, const char* string);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_REGEX_H__ */

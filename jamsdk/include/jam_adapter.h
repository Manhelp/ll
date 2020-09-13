
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Adapter Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ADAPTER_H__
#define __JAM_ADAPTER_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef struct
	{
		uint32_t			id;							/** ��������� */
		char				description[JAM_MAX_PATH];	/** ���������� */
		uint8_t				address[8];					/** ��������ַ(����MAC��ַ) */
		uint32_t			addr_len;					/** ��������ַ���� */
	}adapter_t;


	typedef struct
	{
		adapter_t*			adapt_list;
		size_t				count;
	}adapter_info_t;


	/** ��ȡ��������Ϣ */
	JAM_API adapter_info_t* jam_adapter_info();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ADAPTER_H__ */

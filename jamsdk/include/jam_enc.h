
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Dynamic Encrypt Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ENC_H__
#define __JAM_ENC_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** ��̬������Ҫ���ֽ��� */
	JAM_API size_t jam_enc_dyn_size(size_t src_len, int32_t noise);


	/** ��̬���ܶ����� */
	JAM_API size_t jam_enc_dyn(byte_t* output, size_t output_len, const byte_t* input, size_t input_len, int32_t noise);


	/** ������Ҫ���ֽ��� */
	JAM_API size_t jam_dec_dyn_size(size_t src_len, int32_t noise);


	/** ���ܶ����� */
	JAM_API size_t jam_dec_dyn(byte_t* output, size_t output_len, const byte_t* input, size_t input_len, int32_t noise);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ENC_H__ */


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Packet Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_PACKET_H__
#define __JAM_PACKET_H__

#include "jam_datatype.h"
#include "jam_socket.h"


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_PKT_BROKEN					(-1)		/** ���ݰ������� */
	#define JAM_PKT_ERROR					(-2)		/** ��������ݰ� */


	typedef handle_t						packet_t;


	/** ��������� */
	JAM_API packet_t jam_pkt_alloc(size_t len);


	/** �ͷŴ����� */
	JAM_API void jam_pkt_free(packet_t pkt);


	/** ����һ������ */
	JAM_API packet_t jam_pkt_clone(packet_t pkt);


	/** ���� */
	JAM_API void jam_pkt_reset(packet_t pkt);


	/** ���������С */
	JAM_API void jam_pkt_resize(packet_t pkt, size_t size);


	/** ��ǰ���ݴ�С */
	JAM_API size_t jam_pkt_size(packet_t pkt);


	/** ������� */
	JAM_API size_t jam_pkt_max_size(packet_t pkt);


	/** ��ǰָ��λ�� */
	JAM_API uint32_t jam_pkt_tell(packet_t pkt);


	/** ����ָ�� */
	JAM_API void jam_pkt_seek(packet_t pkt, uint32_t offset);


	/** д��ָ����С�Ķ����� */
	JAM_API bool jam_pkt_write_binary(packet_t pkt, const byte_t* buffer, size_t len);


	/** д��һ��8λ���� */
	JAM_API bool jam_pkt_write_int8(packet_t pkt, uint8_t i8);


	/** д��һ��16λ���� */
	JAM_API bool jam_pkt_write_int16(packet_t pkt, uint16_t i16);


	/** д��һ��32λ���� */
	JAM_API bool jam_pkt_write_int32(packet_t pkt, uint32_t i32);


	/** д��һ��64λ���� */
	JAM_API bool jam_pkt_write_int64(packet_t pkt, uint64_t i64);


	/** д��һ���ַ��� */
	JAM_API bool jam_pkt_write_string(packet_t pkt, const char* str);


	/** ��ȡһ���ַ��� */
	JAM_API const char* jam_pkt_read_string(packet_t pkt);


	/** ��ȡָ����С�Ķ����� */
	JAM_API const byte_t* jam_pkt_read_binary(packet_t pkt, size_t len);


	/** ��ȡһ��8λ���� */
	JAM_API uint8_t jam_pkt_read_int8(packet_t pkt);


	/** ��ȡһ��16λ���� */
	JAM_API uint16_t jam_pkt_read_int16(packet_t pkt);


	/** ��ȡһ��32λ���� */
	JAM_API uint32_t jam_pkt_read_int32(packet_t pkt);


	/** ��ȡһ��64λ���� */
	JAM_API uint64_t jam_pkt_read_int64(packet_t pkt);


	/** ��β����ȡָ����С�Ķ����� */
	JAM_API const byte_t* jam_pkt_reverse_read_binary(packet_t pkt, size_t len);


	/** ��β����ȡһ��8λ���� */
	JAM_API uint8_t jam_pkt_reverse_read_int8(packet_t pkt);


	/** ��β����ȡһ��16λ���� */
	JAM_API uint16_t jam_pkt_reverse_read_int16(packet_t pkt);


	/** ��β����ȡһ��32λ���� */
	JAM_API uint32_t jam_pkt_reverse_read_int32(packet_t pkt);


	/** ��β����ȡһ��64λ���� */
	JAM_API uint64_t jam_pkt_reverse_read_int64(packet_t pkt);


	/** ����һ��������, ���ʧ�ܷ��� JAM_PKT_BROKEN �� JAM_PKT_ERROR, ���򷵻ؽ����ֽ��� */
	JAM_API int32_t jam_pkt_parse(packet_t pkt, const byte_t* buffer, size_t len);


	/** �����ܺ���Ҫ�Ŀռ��ֽ��� */
	JAM_API size_t jam_pkt_build_size(packet_t pkt);


	/** ���ܲ�ѹ������ */
	JAM_API size_t jam_pkt_build(packet_t pkt, byte_t* buffer, size_t len);


	/** TCP �������ݰ� */
	JAM_API void jam_pkt_send(socket_t sock, packet_t pkt, int32_t flag);


	/** UDP �������ݰ� */
	JAM_API void jam_pkt_sendto(socket_t sock, packet_t pkt, sockaddr_t host, uint16_t port, int32_t flag);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_PACKET_H__ */

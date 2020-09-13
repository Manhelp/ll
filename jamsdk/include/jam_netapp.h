
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Network Application Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_NET_APP_H__
#define __JAM_NET_APP_H__

#include "jam_socket.h"
#include "jam_packet.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef void (*on_msg_t)(socket_t sock, sockaddr_t host, uint16_t port, packet_t pkt, void* param);


	/** ��ʼ������ */
	JAM_API bool jam_svc_init();


	/** �رշ��� */
	JAM_API bool jam_svc_cleanup();


	/** �ر������׽��� */
	JAM_API size_t jam_svc_close_all(bool wait);


	/** �й�һ���׽��� */
	JAM_API bool jam_svc_insert(socket_t sock);


	/** �Ƴ�һ���׽��� */
	JAM_API void jam_svc_remove(socket_t sock);


	/** Ϊ�׽��ְ�һ������ */
	JAM_API bool jam_svc_bind(socket_t sock, void* data);


	/** ��ȡ�׽����Զ������� */
	JAM_API void* jam_svc_userdata(socket_t sock);


	/** Ŀǰ�йܵ������� */
	JAM_API size_t jam_svc_size();


	/** �������浽���ݰ� */
	JAM_API bool jam_svc_parse(on_msg_t on_msg, socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_NET_APP_H__ */

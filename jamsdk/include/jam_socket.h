
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Socket Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SOCKET_H__
#define __JAM_SOCKET_H__

#include "jam_addr.h"


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_MAX_CONN_SIZE				65535		/** ���������(����TCP��UDP) */


	typedef enum
	{
		JAM_SERVER_TCP = 1,
		JAM_SERVER_UDP,
		JAM_CLIENT_TCP,
		JAM_CLIENT_UDP,
	}sock_type_t;


	typedef handle_t						socket_t;

	typedef void (*on_accept_t)(socket_t sock, void* param);
	typedef void (*on_lost_t)(socket_t sock, void* param);
	typedef void (*on_data_t)(socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param);


	/** ��ʼ��������Դ */
	JAM_API bool jam_network_init();


	/** ����������Դ */
	JAM_API bool jam_network_cleanup();


	/** ����һ��TCP���������� */
	JAM_API socket_t jam_server_tcp(uint16_t port, void* param, on_accept_t onaccept, on_lost_t onlost, on_data_t ondata);


	/** ����һ��TCP�ͻ������� */
	JAM_API socket_t jam_client_tcp(sockaddr_t host, uint16_t port, void* param, on_lost_t onlost, on_data_t ondata);


	/** ����һ��UDP���������� */
	JAM_API socket_t jam_server_udp(uint16_t port, void* param, on_data_t ondata);


	/** ����һ��UDP�ͻ������� */
	JAM_API socket_t jam_client_udp(void* param, on_data_t ondata);


	/** ��ȡ�������� */
	JAM_API sock_type_t jam_socket_type(socket_t sock);


	/** �ر����� */
	JAM_API bool jam_socket_close(socket_t sock);


	/** ����һ��TCP���Ͳ��� */
	JAM_API void jam_socket_send(socket_t sock, const void* buffer, size_t len, int32_t flag);


	/** ����һ��UDP���Ͳ��� */
	JAM_API void jam_socket_sendto(socket_t sock, const void* buffer, size_t len, sockaddr_t host, uint16_t port, int32_t flag);


	/** ������������(��λ: �ֽ�) */
	JAM_API void jam_socket_limit(socket_t sock, uint32_t speed);


	/** ��ȡ�����ֽ��� */
	JAM_API size_t jam_socket_recv_bytes(socket_t sock);


	/** ��ȡ�����ֽ��� */
	JAM_API size_t jam_socket_sent_bytes(socket_t sock);


	/** ��ȡIP��ַ */
	JAM_API sockaddr_t jam_socket_addr(socket_t sock);


	/** ��ȡԶ�̶˿� */
	JAM_API uint16_t jam_socket_remote_port(socket_t sock);


	/** ��ȡ���ض˿� */
	JAM_API uint16_t jam_socket_local_port(socket_t sock);


	/** �رջ��� */
	JAM_API void jam_socket_no_buffer(socket_t sock, bool enable);


	/** ���û����С */
	JAM_API void jam_socket_buffer_size(socket_t sock, size_t len);


	/** ���ó�ʱʱ�� */
	JAM_API void jam_socket_set_timeout(socket_t sock, uint32_t timeout);


	/** ��ȡ��ʱʱ�� */
	JAM_API uint32_t jam_socket_get_timeout(socket_t sock);


	/** ���ô��ݹر�ʱ�� */
	JAM_API void jam_socket_set_linger(socket_t sock, uint32_t time);


	/** I/O������� */
	JAM_API bool jam_socket_ioctl(socket_t sock, int32_t command, uint32_t* arg);


	/** ����ѡ�� */
	JAM_API bool jam_socket_set_option(socket_t sock, int32_t level, int32_t name, const void* value, int32_t len);


	/** ��ȡѡ�� */
	JAM_API bool jam_socket_get_option(socket_t sock, int32_t level, int32_t name, void* value, int32_t* len);


	/** ��ȡ�׽���ϵͳ��� */
	JAM_API int32_t jam_socket_fd(socket_t sock);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SOCKET_H__ */

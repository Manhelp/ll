
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Message Event Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_EVENT_H__
#define __JAM_EVENT_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				event_t;

	typedef int32_t (*event_proc_t)(uint32_t identify, void* param);


	/** ������Ϣ�¼� */
	JAM_API event_t jam_event_alloc();


	/** �ͷ���Ϣ�¼� */
	JAM_API void jam_event_free(event_t evt);


	/** ע��һ���¼� */
	JAM_API bool jam_event_add(event_t evt, uint32_t msg, event_proc_t do_event);


	/** ע��һ����ʱ���¼�(��λ: ����) */
	JAM_API void jam_event_timer_add(event_t evt, uint32_t id, uint32_t timeout, event_proc_t do_event, void* param);


	/** ע��һ����ʱ���¼� */
	JAM_API void jam_event_timer_remove(event_t evt, uint32_t id);


	/** ͬ��������Ϣ�¼� */
	JAM_API int32_t jam_event_send(event_t evt, uint32_t msg, void* param);


	/** �첽������Ϣ�¼� */
	JAM_API void jam_event_post(event_t evt, uint32_t msg, void* param);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_EVENT_H__ */

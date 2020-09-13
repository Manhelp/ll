
#ifndef __EVENT_H__
#define __EVENT_H__

#include "head.h"


/* ϵͳ�¼���ʼ�� */
bool event_init();


/* ϵͳ�¼����� */
void event_cleanup();


/* ע��ѭ���¼� */
void event_timer_add(uint32_t id, uint32_t timeout, event_proc_t do_event, void* param);


/* ɾ��ѭ���¼� */
void event_timer_remove(uint32_t id);


/* ������֤�¼� */
int32_t validate_conn(uint32_t identify, void* param);


/* �������ʱ�����¼� */
int32_t loop_user(uint32_t identify, void* param);
#endif

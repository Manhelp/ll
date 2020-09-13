
#ifndef __USER_LIST_H__
#define __USER_LIST_H__

#include "head.h"


/* ��Ҷ��г�ʼ�� */
bool user_list_init();


/* ��Ҷ������� */
bool user_list_cleanup();


/* �������ֲ�ѯ������Ϣ */
online_info_t* user_get(const char* name);


/* ��¼����������һ�����������Ϣ */
online_info_t* user_update(const char* name, int32_t sid);


/* ���������� */
bool user_check_offline(online_info_t* info);


/* ����Ƿ�����״̬ */
#define user_is_online(info)				(info->online_time > info->offline_time)


/* ����Ƿ����״̬ */
#define user_standby(info)					(!info->online_time && !info->offline_time && !info->last_time)

/*ö���û�*/
typedef void (*eUserFunc)(online_info_t*, void*);
void enum_user(eUserFunc, void*);
#endif

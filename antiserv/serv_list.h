
#ifndef __SERV_LIST_H__
#define __SERV_LIST_H__

#include "head.h"


/* ���������г�ʼ�� */
bool serv_list_init();


/* �������������� */
bool serv_list_cleanup();


/* ���һ������������ */
serv_t* serv_list_put(socket_t fd, uint32_t section_id);


/* ɾ��һ������������ */
void serv_list_erase(socket_t fd);


/* �����׽��ֲ�ѯһ������������ */
serv_t* serv_list_get(socket_t fd);


/* ���ݱ�Ų�ѯһ�������� */
serv_t* serv_list_get_by_sid(int32_t sid);

/*���ݴ�����ѯһ������������*/
serv_t* serv_list_get_by_sectionid(uint32_t section_id);

/*ö�ٷ�����*/
typedef void (*eFunc)(serv_t*);
void enum_section(eFunc);
#endif

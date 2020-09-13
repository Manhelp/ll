
#ifndef __HEAD_H__
#define __HEAD_H__


#include <jam_sdk.h>

#define LG_SYS						JAM_SRV_LG_SYS
#define LG_INF						"<INF>"
#define LG_WRN						"<WRN>"
#define LG_ERR						"<ERR>"

#define CONFIG_FILE					"antiserv.ini"

#define VERSION_DEFINE			0						/* �汾��0--��½ 1--Խ�� */


#define NAME_LEN					32					/* �û�����󳤶� */
#define MAX_SID						64					/* �û����ɵ�¼�ķ������� */
#define BUFFER_SIZE					8192				/* ����Э��������� */


extern uint32_t OFFLINE_TIMEOUT;				//(120 * 1000)				/* ���߳�ʱ���ʱ�� */
extern uint32_t OFFLINE_CLEAR;				//(5 * 60 * 60 * 1000)		/* �ָ�������Ϸ�����ʱ�� */
extern uint32_t GREEN_TIME;					//(3 * 60 * 60 * 1000)		/* ������Ϸ��ʱ�� */
extern uint32_t RED_TIME;					//(5 * 60 * 60 * 1000)		/* �ǽ�����Ϸ��ʱ�� */
extern uint32_t GREEN_NOTIFY_PERIOD;
extern uint32_t ORANGE_NOTIFY_PERIOD;
extern uint32_t REDNOTIFY_PERIOD;
#if (VERSION_DEFINE==1)
extern uint32_t CHECK_POINT_TIME;						/* ��������ʱ���ʱ��� */
#endif

#define COLOR_GREEN					1					/* ������Ϸʱ��. 3 Сʱ���� */
#define COLOR_ORANGE				2					/* �������. 3 - 5 Сʱ֮�� */
#define COLOR_RED					3					/* ����Ϊ��. 5 Сʱ���� */


#define CMD_NC_LOGIN_CM				4226247465			/* ��������¼Э�� */
#define CMD_NC_ANTIENTHRALL			2591108563			/* ����״̬����Э�� */
#define CMD_NC_ANTIQUERYSTATE		3556684230			/* ��ѯ�û�״̬*/

#define CMD_NS_ANTIENTHRALL			3044263451


#define PTR_TO_ID(p)				((uint32_t)(uint64_t)p)		/* ָ��ת���� */
#define ID_TO_PTR(type, id)			((type*)(uint64_t)id)		/* ����תָ�� */
#define MIN(a,b)					((a)>(b)?(b):(a))
#define MAX(a,b)					((a)>(b)?(a):(b))


/* ������������Ϣ */
typedef struct
{
	int32_t					sid;
	uint32_t				section_id;
	socket_t				fd;
}serv_t;

/* ��¼��������Ϣ */
typedef struct
{
	int32_t					sid;
	datetime_t				last_time;
}online_serv_info_t;

/* ���������Ϣ */
typedef struct
{
	char					name[NAME_LEN];			/* ������� */
	online_serv_info_t		serv[MAX_SID];			/* �ѵ�¼�ķ������б� */
	datetime_t				last_time;				/* ���ͨ��ʱ�� */
	datetime_t				online_time;			/* ����ʱ�� */
	datetime_t				offline_time;			/* ����ʱ�� */
	//datetime_t				notify_time;			/* ��һ��֪ͨ��¼��������ʱ�� */
	uint32_t				online_period;			/* ����ʱ��ͳ��*/
	datetime_t				update_time;			/* ������ʱ�� */
	uint32_t				next_notify;			/* �´�֪ͨʱ����*/
	int8_t					notify_color;			/* ֪ͨ����ɫ(1, 2, 3) */
	uint32_t				offline_period;			/* ����ʱ��*/
}online_info_t;


#endif

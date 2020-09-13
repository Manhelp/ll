
#include "user_list.h"
#include "event.h"


typedef struct
{
	hashdb_t		list;
}user_list_t;


static user_list_t			g_user_list = {0};



static void __user_sid_update(online_info_t* oi, int32_t sid)
{
	int32_t i;
	int32_t first_blank = -1;
	/* ��ѯsid�Ƿ��ѵ�¼ */
	for(i = 0; i < MAX_SID; ++i)
	{
		if(sid == oi->serv[i].sid)
		{
			/* �ѵ�¼��sidֱ�Ӹ���������Ϣ */
			first_blank = -1;
			oi->serv[i].last_time = jam_time_now();		/* ���������ͨ��ʱ�� */
			break;
		}
		if((first_blank < 0) && (!oi->serv[i].sid))
		{
			/* δ��¼��sid */
			first_blank = i;
		}
	}
	if(first_blank >= 0)
	{
		/* �µ�sid��¼��������������Ϣ */
		oi->serv[first_blank].sid = sid;
		oi->serv[first_blank].last_time = jam_time_now();	/* ���������ͨ��ʱ�� */
	}
}


static int32_t __user_sid_size(online_info_t* oi)
{
	int32_t i;
	int32_t c = 0;
	for(i = 0; i < MAX_SID; ++i)
	{
		if(oi->serv[i].sid)
		{
			++c;
		}
	}
	return c;
}


bool user_list_init()
{
	g_user_list.list = jam_hdb_alloc(JAM_HDB_MEMORY, 0);

	return !!g_user_list.list;
}


bool user_list_cleanup()
{
	if(g_user_list.list)
	{
		size_t len;
		char name[NAME_LEN] = {0};
		online_info_t* oi = 0;

		/* ��������������Ϣ */
		if(len = jam_hdb_first(g_user_list.list, (byte_t*)name, NAME_LEN))
		{
			do
			{
				jam_hdb_get(g_user_list.list, (byte_t*)&oi, sizeof(oi), (const byte_t*)name, len, 0);
				if(oi)
				{
					event_timer_remove(PTR_TO_ID(oi));
					jam_free(oi);
				}
				oi = 0;
				memset(name, 0, NAME_LEN);
			}while(len = jam_hdb_next(g_user_list.list, (byte_t*)name, NAME_LEN));
		}
		jam_hdb_free(g_user_list.list);
	}

	return true;
}


online_info_t* user_get(const char* name)
{
	if(g_user_list.list)
	{
		online_info_t* oi = 0;
		jam_hdb_get(g_user_list.list, (byte_t*)&oi, sizeof(oi), (const byte_t*)name, strlen(name), 0);
		return oi;
	}
	return 0;
}


online_info_t* user_update(const char* name, int32_t sid)
{
	if(g_user_list.list)
	{
		online_info_t* oi = user_get(name);
		if(!oi)
		{
			/* ����µ�¼ */
			oi = (online_info_t*)jam_malloc(sizeof(online_info_t));
			memset(oi, 0, sizeof(online_info_t));

			strncpy(oi->name, name, NAME_LEN);
			oi->online_time = jam_time_now();
			oi->update_time = oi->online_time;
			oi->online_period = 0;
			oi->next_notify = 0;
			oi->offline_period = 0;
			oi->notify_color = COLOR_GREEN;

			/* �����������Ϣ���뵽�ڴ����ݿ��� */
			jam_hdb_put(g_user_list.list, (const byte_t*)name, strlen(name), (const byte_t*)&oi, sizeof(oi), JAM_HDB_OVER);
			/* ÿ���Ӵ���һ�����߼�� */
			event_timer_add(PTR_TO_ID(oi), 60 * 1000, loop_user, oi);
		}
		if(user_standby(oi))
		{
			/* ����״̬����ҽ������¼�ʱ */
			oi->online_time = jam_time_now();
			oi->update_time = oi->online_time;
		}
		if(!user_is_online(oi))
		{
			/* ��������������ʱ���� */
			oi->offline_time = 0;
		}
		/* �������������Ϣ */
		__user_sid_update(oi, sid);
		oi->last_time = jam_time_now();
		return oi;
	}
	return 0;
}


bool user_check_offline(online_info_t* info)
{
	if(!__user_sid_size(info))
	{
		info->offline_time = jam_time_now();
		return true;
	}
	return false;
}

void enum_user(eUserFunc pFunc,void* param)
{
	if(g_user_list.list)
	{
		size_t len;
		char name[NAME_LEN] = {0};
		online_info_t* oi = 0;

		/* ���� */
		if(len = jam_hdb_first(g_user_list.list, (byte_t*)name, NAME_LEN))
		{
			do
			{
				jam_hdb_get(g_user_list.list, (byte_t*)&oi, sizeof(oi), (const byte_t*)name, len, 0);
				if(oi)
				{
					pFunc(oi, param);
				}
			}while(len = jam_hdb_next(g_user_list.list, (byte_t*)name, NAME_LEN));
		}
	}
}

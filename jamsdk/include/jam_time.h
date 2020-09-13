
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Time Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_TIME_H__
#define __JAM_TIME_H__

#include "jam_datatype.h"
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint64_t						datetime_t;


	/** ��ȡϵͳ��ǰʱ��� */
	JAM_API datetime_t jam_time_now();


	/** ��ָ����������һ��ʱ��� */
	JAM_API datetime_t jam_time_make(int32_t year, int32_t month, int32_t day, 
									int32_t hour, int32_t minute, int32_t second, int32_t millisecond);


	/** ת��Cʱ�������ǰʱ��� */
	JAM_API datetime_t jam_ctime_to_datetime(time_t time);


	/** ��������ʱ����(��λ: ����) */
	JAM_API uint64_t jam_time_diff(datetime_t a, datetime_t b);


	/** ��(1 - 4095) */
	JAM_API int32_t jam_time_year(datetime_t datetime);


	/** ��(1 - 12) */
	JAM_API int32_t jam_time_month(datetime_t datetime);


	/** ��(1 - 31) */
	JAM_API int32_t jam_time_day(datetime_t datetime);


	/** ʱ(0 - 23) */
	JAM_API int32_t jam_time_hour(datetime_t datetime);


	/** ��(0 - 59) */
	JAM_API int32_t jam_time_minute(datetime_t datetime);


	/** ��(0 - 59) */
	JAM_API int32_t jam_time_second(datetime_t datetime);


	/** ����(0 - 999) */
	JAM_API int32_t jam_time_millisecond(datetime_t datetime);


	/** �ж�ĳ���Ƿ�Ϊ���� */
	JAM_API bool jam_time_is_leap_year(int32_t year);


	/** ��ȡĳ�������� */
	JAM_API int32_t jam_time_day_of_year(int32_t year);


	/** ��ȡĳ��ĳ�������� */
	JAM_API int32_t jam_time_day_of_month(int32_t year, int32_t month);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_TIME_H__ */

/********************************************************************
	created:	2010/08/18
	created:	18:8:2010   11:57
	filename: 	e:\loong\trunk\ServerDefine\msg_buff.h
	file path:	e:\loong\trunk\ServerDefine
	file base:	msg_buff
	file ext:	h
	author:		Jason
	
	purpose:	world��db֮�䣬ֱ���޸�buff������Э��
*********************************************************************/

#ifndef msg_buff_h__
#define msg_buff_h__

#pragma pack(push)
#pragma pack(1)

#include "../WorldDefine/msg_common_errorcode.h"
#include "role_data_define.h"

// ֱ�Ӳ���buff��һ��buff
CMD_START(NDBC_InsertBuff2DB)
	DWORD			dwRoleID;
	tagBuffSave	BuffSave;
CMD_END


#pragma pack(pop)
#endif // msg_buff_h__
/********************************************************************
	created:	2010/08/20
	created:	20:8:2010   11:14
	filename: 	e:\F-Project\src\ServerDefine\msg_olinfo.h
	file path:	e:\F-Project\src\ServerDefine
	file base:	msg_olinfo
	file ext:	h
	author:		Jason
	
	purpose:	world����״̬��Ϣ��¼Э�飬����world��ʱ�ر��Լ���һЩ״̬��Ϣ����¼��db��
*********************************************************************/
#pragma  once

#pragma pack(push)
#pragma pack(1)

#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/base_define.h"

struct tagWorldMapOnlineInfo
{
	DWORD	dwMapID;
	BOOL		bIsInst;
	INT			nOnlineNum;
	INT			nOpenNum;
	// ʱ�����db�Լ���
};
// Jason ��¼world������Ϣ
CMD_START( NDBC_WorldMapOlInfo  )
	INT			nNum;
	BYTE		data[1]; //tagWorldMapOnlineInfo*
CMD_END

#pragma pack(pop)


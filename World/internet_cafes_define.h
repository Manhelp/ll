/********************************************************************
	created:	2012/05/07
	created:	7:5:2012   17:17
	filename: 	e:\code\World\internet_cafes_session.h
	file ext:	h
	author:		zhangzhihua
	
	purpose:	��Ϸ�������뺫������Time�������Ի������Ϣ
*********************************************************************/
#pragma once

#pragma pack(push, 1)

CONST INT SERVECE_TYPE = 79253;
CONST INT GAME_SSN = 355;

CONST INT INTERNET_CAFE_PING_INTERVAL = 5 * 60;

CONST DWORD INTERNET_CAFE_BUFF_LIST[] =
{
	9700101,
	9700201,
	9700301,
	9700401,
};

enum EIC_RET_CODE
{
	EICRC_SUCCESS = 0,
	EICRC_FAILED,
	EICRC_LACK_TIME,
	EICRC_INVALID_IP,
	EICRC_PROTOCOL_ERR,
	EICRC_INVALID_USER,
};

enum EIC_MSG_TYPE
{
	EICMT_Start			= 0,
	EICMT_Logout		= 1,
	EICMT_Ping			= 2,
	EICMT_LeftTime		= 20,
};

//---------------------------------------------------------------------------------
// �������������dummyTs����Ϣ
//---------------------------------------------------------------------------------
struct tagW2T_Package
{
	BYTE			byStart;		// ��ʾ��Ϣ��ͷ�� 1byte ����. �̶�Ϊ 0x0e
	BYTE			byDummy;		// Dummy 1byte
	WORD			wSequence;		// �ֳ����ֵ� 2byte
	WORD			wLength;		// Body length
	BYTE			byData[1];		// ����Header�� body���ֳ��� 2byte -> byte ordering ��Ҫת��
};

//---------------------------------------------------------------------------------
// dummyTs�����������������Ϣ
//---------------------------------------------------------------------------------
struct tagT2W_Package
{
	BYTE			byStart;		// ��ʾ��Ϣ��ͷ�� 1byte ����. �̶�Ϊ 0x0e
	BYTE			byDummy;		// Dummy 1byte
	WORD			wSequence;		// �ֳ����ֵ� 2byte
	WORD			wLength;		// Body length
	BYTE			byData[1];		// ����Header�� body���ֳ��� 2byte -> byte ordering ��Ҫת��
};


#pragma pack(pop)

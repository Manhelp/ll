#pragma once
#include "AutoFight_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------
enum EAutoFightError
{
	E_AutoFight_UseZhanDouFu_Success = 0,	// ʹ�óɹ�
	E_AutoFight_No_ZhanDouFu,				// û��ս��������
	E_AutoFight_ZhanDouFu_TimeOut,			// ս������ʱ����
};

//-----------------------------------------------------------------------
// ʹ��ս����
//-----------------------------------------------------------------------
CMD_START(NC_UseZhanDouFu)
	INT64   n64ItemID;						// ��Ʒ��64λID
CMD_END

CMD_START(NS_UseZhanDouFu)
	DWORD				dwErrorCode;
	INT64					n64ItemID;						// ��Ʒ��64λID
	tagDWORDTime	dw1stGainTime;				// ��һ����һ��ʱ��
	DWORD				dwLeftTicks;					// ��ս����ʣ��tick����Ϊ0��ʾ��Ҫ��ʼ��ʱ
CMD_END

//-----------------------------------------------------------------------
// �ڹ�״̬
//-----------------------------------------------------------------------
CMD_START(NC_SyncAutoFightState)
	INT64			n64ItemID;						// ��Ʒ��64λID
	EAutoFightState eAutoFightState;
CMD_END

CMD_START(NS_SyncAutoFightState)
	EAutoFightState eAutoFightState;
	INT64			n64ItemID;
CMD_END

//-----------------------------------------------------------------------
// ������֪ͨ�ͻ���ս��������ʱ�޵���
//-----------------------------------------------------------------------
CMD_START(NS_ZhanDouFuTimeOut)
	INT64			n64ItemID;
CMD_END

#pragma pack(pop)
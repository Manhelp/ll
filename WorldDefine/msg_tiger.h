#pragma once


#pragma pack(push, 1)

enum
{
	E_Tiger_success = 0,
	E_Tiger_NotEnoughItem,
};
//-----------------------------------------------------------------------------
// �ϻ�����ý��
//-----------------------------------------------------------------------------
CMD_START(NC_GetTigerResult)	
	INT		nType;	// x1,x5,x10,x50 �ֱ� 1 5 10 50
CMD_END

CMD_START(NS_GetTigerResult)
	DWORD	dwErrorCode;
	DWORD	dwResult;		// 1-7    7�Ƚ������Ӹߵ���
	DWORD	dwNum;
CMD_END

//-----------------------------------------------------------------------------
#pragma pack(pop)
//-----------------------------------------------------------------------------
//!\file msg_spirit.h
//!
//!\date 2010-01-15
//!
//!\brief ��������Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

#include <bitset>
using std::bitset;

CMD_START(NS_ServerInfo)				// ��������Ϣ
	TCHAR	szServer[256];
	DWORD	dwLoginMode;
	BOOL	bEnableExpressPay;
	TCHAR	szExpressPayUrl[256];	
	BOOL	bEnableSpiritSys;
	TCHAR	szTodayHotActivityUrl[256];
	BOOL	bEnableCitySys;
	DWORD	dwAutoFightTimeInterval;	// ս���һ�ս��ʩ�ż��ܺ�ʹ����Ʒʱ����
	BOOL bGuildWarOpen; // ����ս����
	DWORD	dwServerFunctionSwitch;		//���ֹ��ܿ���
CMD_END

enum EServerFunctionSwitch
{
	ESFS_Null = -1,
	ESFS_MallCart,

	ESFS_End,
};
typedef bitset<ESFS_End>	BitSetServerFunctionSwitch;

#pragma pack(pop)
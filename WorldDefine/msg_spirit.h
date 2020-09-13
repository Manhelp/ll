//-----------------------------------------------------------------------------
//!\file msg_spirit.h
//!
//!\date 2010-01-15
//!
//!\brief �ͻ����������֮����ھ���ֵ��Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

enum ESpiritError
{
	E_InalidSprirtValue = 1,
	E_BagIsFull,
	E_WrongRequestNum,
	E_NoRequestItem,
	E_StrengthNotEnough,
};

CMD_START(NC_SpiritReward)				// ��ȡ����ֵ�Ļر�
CMD_END

CMD_START(NS_SpiritReward)				// ��ȡ����ֵ�Ļر�
	DWORD	dwError;
CMD_END

CMD_START(NC_StrengthReward)			// ʵ��ֵ��ȡ����ֵ����
	DWORD	dwTypeID;
	INT		nNum;	
CMD_END

CMD_START(NS_StrengthReward)			// ʵ��ֵ��ȡ����ֵ����
	DWORD	dwError;	
CMD_END

#pragma pack(pop)
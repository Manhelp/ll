#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"

CONST	UINT16		DOUBLE_EXP_QUEST_ID		=	10848;		// ˫����������ID
CONST	UINT16		FISHING_QUEST_ID		=	79;			// ��������ID

enum ELoongBenediction
{
	ELBD_Success			= 0,			// �ɹ�
	ELBD_Bag_NotEnoughSpace	= 1,			// �����Ѿ�����
	ELBD_Time_NotArrive		= 2,			// ʱ�仹δ��
	ELBD_No_LessingProto	= 3,			// û���ҵ��͸�ԭ��������Ϣ
	ELBD_Item_Create_Failed	= 4,			// ��Ʒ����ʧ��
	ELBD_No_Benediction		= 5,			// û����

};

enum EOfflineExp
{
	EOFFE_NotEnoughYuanBao = 1
};

//-----------------------------------------------------------------------------
// ��ֹ�����ʧ������������ʾ�������͸�
//-----------------------------------------------------------------------------

CMD_START(NC_ExitGamePrompt)
CMD_END

CMD_START(NS_ExitGamePrompt)
	BOOL		bDualExpQuestDo;				//�����Ƿ����,˫��ɱ�־�������� id = 10848
	BOOL		bFishQuestDo;					//�����Ƿ���ɣ��������� id = 79
	INT32		nExprence;						//����һСʱ��õľ���
	BOOL		bCanGetBenediction;				//�����ǿ�����ȡ�����͸���(�Ѿ�������ǰ20�Σ����ҽ��컹δ��ȡ��)
	DWORD		dwHour;							//������ȡ��һ�������͸���Ҫ����Сʱ������ʱ��
	DWORD		dwMinute;						//������ȡ��һ�������͸���Ҫ���ٷ�������ʱ��
CMD_END


//-----------------------------------------------------------------------------
// �����͸�
//-----------------------------------------------------------------------------
CMD_START(NC_GetLoongBenediction)			//��ȡ����
CMD_END

CMD_START(NS_GetLoongBenediction)
	DWORD		dwErrorCode;
	DWORD		nItemTypeID;				//��������Ʒ��ID
	INT			nItemCount;					//��Ʒ�����ĸ���
	INT64		nExprence;					//�����ľ����ID
CMD_END

CMD_START(NS_SetNextBenediction)
	BOOL			isEnd;					//�����͸��Ƿ�����ı�־
	DWORD			nextServerTime;			//��һ�������͸���ʱ�䣬ʣ������
	DWORD			nItemTypeID;			//��õ���Ʒ����
	INT				nItemCount;				//��Ʒ�����ĸ���
	INT64			n32Exprence;			//���齱��
CMD_END

CMD_START(NS_LoongBenedictionInit)			//���߿ͻ�����Ҫ��ʼ�����ģ��
	INT				nType;					//��ȡģʽ��0 ���մ�����Ĭ��20�Σ�1����ÿ������ʱ����
CMD_END

//-----------------------------------------------------------------------------
// ���߹һ�
//-----------------------------------------------------------------------------
CMD_START(NC_GetOfflineExp)					// ��ȡ���߾���
	BYTE			byType;					// ��ȡ���ͣ�0��ʾ��ȡ��Ѿ��飬1��ʾ��ȡȫ������
CMD_END

CMD_START(NS_GetOfflineExp)
	DWORD			dwErrorCode;
	DWORD			dwExperience;			//���ߵľ���
CMD_END

// Jason 2010-1-15 v1.3.2 ���߹һ� ���վ��齱��Э��
CMD_START(NS_OfflineExperienceReward)
	BYTE			byHour;					// ����Сʱ��
	BYTE			byMinute;				// ���߷�����
	INT64			n64FreeExperience;		// ��Ѿ��齱��
	INT64			n64PayNeededExperience;	// �踶�Ѿ��齱��
	INT				nYuanBao;				// ��ȡ���⾭����Ҫ��Ԫ��
CMD_END

// Jason 2010-1-15 v1.3.2 ��һع�
CMD_START( NS_PlayerRegression )
	DWORD dwItemTypeID;						// ��ȡ��Ʒ��typeID
	INT   nItemNum;							// ��Ʒ����
	BYTE  byQuality;						// ��ƷƷ��
CMD_END

CMD_START( NC_PlayerRegressionOk )			// ����ҵ����ȡ
CMD_END

CMD_START( NS_PlayerRegressionOk )			// ����ҵ����ȡ
	DWORD dwErrorCode;
CMD_END
// �����
//CMD_START( NS_PlayerRegressionEnd ) 
//CMD_END


#pragma pack(pop)

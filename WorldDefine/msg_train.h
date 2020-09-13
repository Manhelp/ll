//-----------------------------------------------------------------------------
// filename: msg_train.h
// author: lfmeng
// actor:
// data: 2010-3-10
// last:
// brief: ��֮����ϵͳ��Ϣ����
//-----------------------------------------------------------------------------

#pragma once

#include "train_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum ETrainErrCode
{
	E_Train_Success	= 0,
	E_Train_List_Null,			// �����б�Ϊ��
	E_Train_Bag_Full,			// ���ҿռ䲻�㣬��Ϊ����������㹻�ռ������µ�����������
	E_Train_Not_Pass,			// ����ͨ������
	E_Train_Pass,				// ͨ������
	E_Train_Unknown,			// δ֪����
};

//-----------------------------------------------------------------------------
// ȡ�����б�
//-----------------------------------------------------------------------------
CMD_START(NC_GetTrainList)
CMD_END

CMD_START(NS_GetTrainList)
	ETrainErrCode		eTrainErrCode;
	INT					nTrainCnt;
	tagTrainState		pTrainAttTable[1];
CMD_END

//-----------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------
CMD_START(NC_FinishTrain)
	DWORD 			dwTrainID;
CMD_END

CMD_START(NS_FinishTrain)
	ETrainErrCode	eTrainErrCode;
	DWORD			dwTrainID;				// ����ID
	ETrainState		eTrainState;			// ����״̬
CMD_END

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CMD_START(NS_OpenTrain)
	DWORD dwTrainID;				// ����ID
CMD_END

//-----------------------------------------------------------------------------
// ͬ��������ɶ�
//-----------------------------------------------------------------------------
CMD_START(NS_SyncTrainCompleteness)
	DWORD dwTrainID;				// ����ID
	DWORD dwCompleteness;
CMD_END

#pragma pack(pop)
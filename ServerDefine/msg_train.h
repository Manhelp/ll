//-----------------------------------------------------------------------------
// filename: msg_train.h
// author: lfmeng
// actor:
// data: 2010-04-02
// last:
// brief: ��֮����ϵͳ--LongDB��LoongWorldͨ��
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/train_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// �����Ѿ�����������״̬
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveOpenTrainState)
	DWORD	dwRoleID;
	DWORD	dwTrainID;
	ETrainState eTrainState;
	DWORD dwCompleteness;
CMD_END

#pragma pack(pop)
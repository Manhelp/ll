#pragma once
#include "fatigue_define.h"
#pragma pack(push,1)

//-------------------------------------------------------------------------
// login ��world�ķ�������Ϣ
//-------------------------------------------------------------------------
CMD_START(NLW_FatigueNotify)
	INT				nNum;
	tagFatigueNotify	notify[1];
CMD_END

#pragma pack(pop)
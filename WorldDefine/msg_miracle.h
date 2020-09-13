//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_miracle.h
// author: songg
// actor:
// data: 2010-01-05
// last:
// brief: �ɽ�����
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/miracle_define.h"
#include "../WorldDefine/msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ��ȡ��ҵ�ǰ����״̬
//----------------------------------------------------------------------------

//��ϰ�ɽ缼��
CMD_START(NC_PracticeMiracleSkill)
	DWORD dwSkillID;			// ��ID
CMD_END

//���ش�����
CMD_START(NS_PracticeMiracleSkill)
	DWORD	dwErrCode;			// ������
	INT16	n16CurPrcNum;		// ��ǰ������������
CMD_END



//0�� �������������������
CMD_START(NS_ResetMiraclePrcNum)

CMD_END



//ʹ���ɽ����з������������
CMD_START(NC_UseAddMiraclePrcNumItem)
	INT64		n64ItemID;			//��Ʒ64λID
CMD_END

//����
CMD_START(NS_UseAddMiraclePrcNumItem)
	DWORD	dwErrCode;			// ������	
	INT16	n16CurPrcNum;		// ��ǰ������������
CMD_END

////��������
//CMD_START(NC_PracticeSoarSkill)
//	DWORD dwSkillID;			// ��ID
//CMD_END
//
////���ش�����
//CMD_START(NS_PracticeSoarSkill)
//	DWORD	dwErrCode;			// ������
//	INT16	n16CurPrcNum;		// ��ǰ������������
//CMD_END

//0�� ����������ҷ������ܿ���������
CMD_START(NS_ResetSoarSkillLearnTimes)
	INT16    nCurrLeftNum;		// ��ǰ�����������Ĵ���
CMD_END

#pragma pack(pop)

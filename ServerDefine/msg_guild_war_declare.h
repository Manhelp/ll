//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: ����ս��ս��ϵworld��db�����Ϣ����
//-----------------------------------------------------------------------------
#pragma once

#include "guild_war_declare_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ������ݿ��й��ڵ���ս��ϵ
//-----------------------------------------------------------------------------
CMD_START(NDBC_ClearExpireGuildWarDeclare)

CMD_END

//-----------------------------------------------------------------------------
// ��������ս��ϵ���浽���ݿ�
//-----------------------------------------------------------------------------
CMD_START(NDBC_InsertGuildWarDeclare)
	DWORD			dwAttackGuildID;			// ��ս����ID
	DWORD			dwDefenceGuildID;			// ����ս����ID
	tagDWORDTime	dwWarDeclareTime;			// ��սʱ��
	BOOL			bWarEnded;					// �ǲ����Ѿ���������
CMD_END

//-----------------------------------------------------------------------------
// �޸����ݿ��е���ս��ϵ��״̬Ϊ��ս��
//-----------------------------------------------------------------------------
CMD_START(NDBC_SetGuildWarDeclareEnded)
	DWORD			dwAttackGuildID;			// ��ս����ID
	DWORD			dwDefenceGuildID;			// ����ս����ID
	tagDWORDTime	dwWarDeclareTime;			// ��սʱ��
CMD_END

//-----------------------------------------------------------------------------
// �������ݿ��еĹ�����ս��ϵ
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadGuildWarDeclare)

CMD_END

CMD_START(NDBS_LoadGuildWarDeclare)
	DWORD					dwGuildWarDeclareCnt;		// ����ս����ս��ϵ����
	tagGuildWarDeclare		pGuildWarDeclare[1];		// ���д������ݿ��еĹ�����ս��ϵ
CMD_END

#pragma pack(pop)

//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: ����ս��ս��ϵ��ض���
//-----------------------------------------------------------------------------
#pragma once

struct tagGuildWarDeclare
{
	DWORD			dwAttackGuildID;			// ��ս����ID
	DWORD			dwDefenceGuildID;			// ����ս����ID
	tagDWORDTime	dwWarDeclareTime;			// ��սʱ��
	BOOL			bWarEnded;					// �ǲ����Ѿ���������
};
//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: ����ս��ض���
//-----------------------------------------------------------------------------
#pragma once

// ����ÿ����ս��������
const INT32 MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY = 5;

// ÿ�������ͬһ������ÿ����ս��������
const INT32 MAX_SAME_GUILD_BE_DECLARE_WAR_TIMES	= 2;

// ����ս״̬buff��ID
const INT32 GUILD_WAR_BUFF_ID = 9000001;
const INT32 GUILD_WAR_BUFF_TYPE_ID = GUILD_WAR_BUFF_ID/100;

// ����ս��ս����(�ݶ���ս����)
const INT32 DECLEAR_WAR_ITEM_TYPE_ID = 4500003;

// ����ս��ս�۳���Ǯ��(1w��)
const INT32 DECLEAR_WAR_SILVER_COST = 10000;

// ������
enum
{
	E_GuildDeclareWar_Success				= 1,		// ��ս�ɹ�

	E_GuildDeclareWar_Gold_Not_Enough		= 2,		// ������Ǯ����
	E_GuildDeclareWar_No_Spec_Item			= 3,		// ��ɫ����û����ս����
	E_GuildDeclareWar_Declare_Times_Full	= 4,		// ��ս�����Ѵ�����
	E_GuildDeclareWar_Not_Guild_Leader		= 5,        // ���ǹ���᳤
	E_GuildDeclareWar_Same_Guild_Full		= 6,		// ��ͬһ������ÿ����ս�����ﵽ����
	E_GuildDeclareWar_Guild_Not_Exist		= 7,		// ����ս���᲻������
	E_GuildDeclareWar_Declare_Exist			= 8,		// �Ѿ���ս����
};

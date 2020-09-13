//-----------------------------------------------------------------------------
//!\file msg_shortcut_quickbar.h
//!
//!\date 2010-02-21
//!
//!\brief �����������ݿ�  ��Ϣ
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push)
#pragma pack(1)
#include "../WorldDefine/msg_quickbar.h"

const int QUICK_BAR_TOTALCOUNT = 10*8 ;

//�����ݿ������
CMD_START(NDBC_SaveShortCut_QuickBar)
	DWORD			dwRoleID;
	tagQuickData	QuickBarData[QUICK_BAR_TOTALCOUNT];
CMD_END;

//�����ݿ�Ҫ����
CMD_START(NDBC_LoadShortCut_QuickBar)
	DWORD						dwRoleID;
CMD_END

//���ݿ��������������
CMD_START(NDBS_LoadShortCut_QuickBar) 
	DWORD						dwRoleID;
	tagQuickData	QuickBarData[QUICK_BAR_TOTALCOUNT];
CMD_END;

//������ݿ�û������,�ͷ�����Ϣ֪ͨ������
CMD_START(NDBS_IsSavedShortCut_QuickBar) 
	DWORD						dwRoleID;
	INT16                       nIsSaved;
CMD_END;
#pragma pack(pop)
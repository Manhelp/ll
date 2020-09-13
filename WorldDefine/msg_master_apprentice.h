//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_master_apprentice.h
// author: twan
// actor:
// data: 2010-01-18
// last: 
// brief: ʦͽ���
//-----------------------------------------------------------------------------
#pragma once

#include "master_apprentice.h"
#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// ��ʦ���
//-----------------------------------------------------------------------------

// ͽ�������ʦ
CMD_START(NC_ApprenticeApplyMaster)
	DWORD		dwRoleID;		// ʦ��ID
CMD_END

// ���ظ�ͽ�ܵĴ�����
CMD_START(NS_ApprenticeApplyMaster)
	DWORD		dwErrorCode;
CMD_END

// ���͸�ʦ������ʾ��Ϣ
CMD_START(NS_MasterNotice)
	DWORD		dwRoleID;		// ͽ��ID
	INT32		nLevel;			// ͽ�ܵȼ�
CMD_END

// ʦ���Ļظ�
CMD_START(NC_MasterReply)
	DWORD		dwRoleID;		// ͽ��ID
	BOOL		bAccept;		// �Ƿ����
CMD_END

// ���ش������ʦ��
CMD_START(NS_MasterReply)
	DWORD		dwErrorCode;
CMD_END

// ʦ���Ƿ��������Ľ�����͸�ͽ��
CMD_START(NS_MasterReplyApprentice)
	DWORD		dwRoleID;		// ͽ��ID
	DWORD		dwErrorCode;
	BOOL		bAccept;
CMD_END


//-----------------------------------------------------------------------------
// ��ͽ���
//-----------------------------------------------------------------------------

// ʦ��������ͽ
CMD_START(NC_MasterAcceptApprentice)
	DWORD		dwRoleID;			// ͽ��ID
CMD_END

// ���ظ�ʦ���Ĵ�����
CMD_START(NS_MasterAcceptApprentice)
	DWORD		dwErrorCode;
CMD_END

// ���͸�ͽ�ܵ���ʾ��Ϣ
CMD_START(NS_ApprenticeNotice)
	DWORD		dwRoleID;		// ʦ��ID
	INT32		nLevel;			// ʦ���ȼ�
CMD_END

// ͽ�ܵĻظ�
CMD_START(NC_ApprenticeReply)
	DWORD		dwRoleID;		// ʦ��ID
	BOOL		bAccept;		// �Ƿ����
CMD_END

// ���ش������ͽ��
CMD_START(NS_ApprenticeReply)
	DWORD		dwErrorCode;
CMD_END

// ͽ���Ƿ��������Ľ�����͸�ʦ��
CMD_START(NS_ApprenticeReplyMaster)
	DWORD		dwRoleID;		// ʦ��ID
	DWORD		dwErrorCode;
	BOOL		bAccept;
CMD_END

//-----------------------------------------------------------------------------
// ʦ�����
//-----------------------------------------------------------------------------

// �鿴�ҵ�ʦ��
CMD_START(NC_MatesInfo)
	
CMD_END

CMD_START(NS_MatesInfo)
	INT							nNum;
	INT32						nJingWuPoint;				// ��ʹ�õ���ҵ����
	tagMasterInfo				sMasterInfo;				// ʦ����Ϣ
	tagMatesAndApprenticeInfo	sMatesInfo[1];				// ͬ����Ϣ
CMD_END

// �鿴�ҵ�ͽ��
CMD_START(NC_ApprenticeInfo)

CMD_END

CMD_START(NS_ApprenticeInfo)
	INT							nNum;
	tagMasterInfo				sMasterInfo;				// ʦ����Ϣ
	tagMatesAndApprenticeInfo	sApprenticeInfo[1];			// ͬ����Ϣ
CMD_END

//-----------------------------------------------------------------------------
// ��ҵ�������
//-----------------------------------------------------------------------------

// ʦ��������ҵ����
CMD_START(NC_GiveJingWuPoint)
	DWORD				dwRoleID;			// ͽ��ID
	INT32				nPoint;				// ����ĵ���				
CMD_END

CMD_START(NS_GiveJingWuPoint)
	DWORD				dwErrorCode;		// ������		
CMD_END

// ͽ�ܴ�ʦ�����õ���ҵ����
CMD_START(NS_GetJingWuPoint)
	INT32				nPoint;				
CMD_END

// ��ҵ��������
CMD_START(NC_JingWu2Client)

CMD_END

CMD_START(NS_JingWu2Client)
	INT32				nNeedJingWuPoint;		// ��Ҫ����ҵ����
	INT32				nJingWuPoint;			// ��ǰ��ҵ����
	INT32				nNeedYuanBao;			// ��Ҫ��Ԫ��
	INT32				nYuanBao;				// ��ǰԪ��
CMD_END

// ͽ��ʹ����ҵ����
CMD_START(NC_UseJingWuPoint)
	
CMD_END

CMD_START(NS_UseJingWuPoint)
	DWORD				dwErrorCode;		// ������
	INT32				nCurJingWuPoint;	// ��ǰ��ҵ����
CMD_END

//-----------------------------------------------------------------------------
// ���ʦͽ��ϵ���
//-----------------------------------------------------------------------------

// ͽ������ʦ��
CMD_START(NC_BreakOutMaster)
	DWORD				dwRoleID;			// ʦ��ID
CMD_END

CMD_START(NS_BreakOutMaster)
	DWORD				dwErrorCode;
CMD_END

CMD_START(NC_ForceBreakOutMaster)
	DWORD				dwRoleID;			// ʦ��ID
CMD_END

CMD_START(NS_ForceBreakOutMaster)
	DWORD				dwErrorCode;
	INT					dwRemainTime;
CMD_END

// �ٴ�ȷ��
CMD_START(NC_BreakOutMasterConfirm)
	DWORD				dwRoleID;			// ʦ��ID
CMD_END

// ����Ϣ�����͸�ʦͽ˫��
CMD_START(NS_BreakOutMasterConfirm)
	DWORD				dwRoleID;
CMD_END


// ʦ�����ͽ��
CMD_START(NC_BreakOutApprentice)
	DWORD				dwRoleID;			// ͽ��ID
CMD_END

CMD_START(NS_BreakOutApprentice)
	DWORD				dwErrorCode;
CMD_END

// �ٴ�ȷ��
CMD_START(NC_BreakOutApprenticeConfirm)
	DWORD				dwRoleID;			// ͽ��ID
CMD_END

// ����Ϣ�����͸�ʦͽ˫��
CMD_START(NS_BreakOutApprenticeConfirm)
	DWORD				dwRoleID;
CMD_END

//-----------------------------------------------------------------------------
// ��ʦ
//-----------------------------------------------------------------------------
CMD_START(NS_GraduateNotice)
	DWORD				dwRoleID;			// ͽ��ID
CMD_END

#pragma pack(pop)

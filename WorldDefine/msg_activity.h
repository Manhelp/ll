//-----------------------------------------------------------------------------
//!\file msg_activity.h
//!
//!\date 2009-03-29
//!
//!\brief �ͻ����������֮����ڻ����Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// �ű���ȱʡ��Ϣ��NC_DefaultRequest)�¼����Ͷ��壨�����Ͷ�����lua��ʹ�ã�
//-----------------------------------------------------------------------------
/*
dwEventType = 1				// �ͻ�������PVP�̶������ 
dwEventType = 2				// ��ѯ����ڹ̶���е�����
*/

CMD_START(NS_ActivityTaskState)				// �㲥���ǰ״̬�����������������
	INT		nState;							// 0-������ʼ 1-��ʼ 2-�������� 3-���� 
	TCHAR	szName[X_SHORT_NAME];			// �����
	BYTE	byHour;
	BYTE	byMinute;
CMD_END

CMD_START(NS_RoleInitOk)				// ��ҳ�ʼ������
CMD_END


CMD_START(NC_AllGodMiracleInfo)		// ��ȡ��ǰ�����񼣵���Ϣ
	DWORD	dwActiveID;				// �ID
CMD_END	

// ����Ϣ�Ľṹ����
struct tagGodMiracleInfo
{
	DWORD	dwGodMiracleID;		// ��ID
	DWORD	dwCaptureGuildID;	// ռ���񼣵İ���ID(����0����ռ�죬����ռ��)
	DWORD	dwLastCpatureTime;	// ʣ��ռ���񼣵�ʱ��(��)
	tagGodMiracleInfo():dwGodMiracleID(0),dwCaptureGuildID(0),dwLastCpatureTime(0)
	{}
};

CMD_START(NS_AllGodMiracleInfo)			// ��ҳ�ʼ������
	int				    nGodNum;		// ����
	tagGodMiracleInfo	sGodMiracle[1];
CMD_END	

//-----------------------------------------------------------------------------
// �ᱦ������Ϣ
//-----------------------------------------------------------------------------
// ͬ������ɿ���ʣ��ʱ��(��λΪ��)
CMD_START(NS_SyncOpenChaseRemainTime)
	DWORD	dwOpenChaseRemainTime;
CMD_END

// ���䱻ǿ�Ƶ���
CMD_START(NS_ForceLootChase)
CMD_END

//-----------------------------------------------------------------------------
// ��ѡ��֮ս�  dwMsgType == 1�ű�����
//-----------------------------------------------------------------------------
CMD_START(NS_DotaMsg)
DWORD dwMsgType;
DWORD dwMsgSize;
CHAR  msg[1];
CMD_END 

#pragma pack(pop)
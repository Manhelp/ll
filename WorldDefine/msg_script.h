//-----------------------------------------------------------------------------
//!\file msg_script.h
//!
//!\date 2009-4-16
//!
//!\brief �ͻ����������֮��ű���Ϣ
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

enum EMsgUnitType;
enum EDlgOption;

//-----------------------------------------------------------------------------
// NS_ScriptBroad��dwMisc�ֶε�λ���Ͷ���
//-----------------------------------------------------------------------------
CMD_START(NS_MsgBlob)
	BYTE byData[1];
CMD_END

//-----------------------------------------------------------------------------
// �ͻ��˶Ի��򷢸������ȱʡ��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_DlgDefaultMsg)
	EMsgUnitType	eDlgTarget;				// �ͻ��˶Ի����������Ӧ��Ŀ�꣨������Ʒ�� ��ң�
	DWORD			dwTargetID;				// Ŀ��ID
	EDlgOption		eDlgOption;				// ��ҷ��صĶԻ���ѡ��
CMD_END

//ֻ�е�RoleID
CMD_START(NS_MsgBroadCastBlob)
	BYTE byData[1];
CMD_END

//-----------------------------------------------------------------------------
// �ͻ��˴����������ű���ȱʡ��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_DefaultRequest)
	EMsgUnitType	eDlgTarget;				// �����������ű��Ķ�ӦĿ�꣨�̶����
	DWORD			dwTargetID;				// Ŀ��ID
	DWORD			dwEventType;			// �������¼�����
CMD_END

// Jason 2010-6-1 ������Ϣ
// ������֪ͨ�ͻ��˵���Ϣ
CMD_START( NS_PullRoleMsg )
	DWORD			dwAppID;		// Ӧ��id���ͻ��ˣ�ֻ��Ҫ�������Э���ﴫ�����ֵ
	DWORD			dwActID;		// �id
	DWORD			dwIdMsgID;		// id_msg.xml�е�idֵ
	DWORD			dwMapID;
	INT				x;				// x��
	INT				y;
	INT				z;
CMD_END
// �ͻ��˷�������Ϣ������ok0��false�����
CMD_START( NC_PullRoleMsgCallback )
	DWORD			dwAppID;
	DWORD			dwActID;
	DWORD			dwIdMsgID;		// id_msg.xml�е�idֵ
	DWORD			param1;
	DWORD			param2;
	DWORD			param3;
	DWORD			param4;
CMD_END

#pragma pack(pop)
/********************************************************************
	created:	2010/09/25
	created:	25:9:2010   19:38
	filename: 	e:\F-Project\src\WorldDefine\msg_easy_team.h
	file path:	e:\F-Project\src\WorldDefine
	file base:	msg_easy_team
	file ext:	h
	author:		Jason
	
	purpose:	��������Ϣ�Լ��ṹ����
*********************************************************************/

#ifndef msg_easy_team_h__
#define msg_easy_team_h__


#pragma pack(push, 1)

// Ŀǰֻ��EETT_KillCreatureĿ�ĵĶ�����Ϣ
enum EEasyTeamType
{
	EETT_KillCreature	= 0 , // ɱ��
	EETT_Instance		= 1 , // ����
	EETT_Activity		= 2 , // �
	EETT_Other			= 3 , // ����
	EETT_All,				  // ����
};

struct tagEasyTeamSimInfo
{
	BYTE	byTeamType;	// ���Ŀ�ģ�����ɱ�֣�EEasyTeamType����
	DWORD	dwLeaderID;	// �ӳ�id
	BYTE	byLeaderLevel;	// �ӳ��ȼ�
	WORD	wRoles;			// ����������ϵ�������ֽڣ����ֽ������������ֽڵ�ǰ����
	BYTE	byMemoLen;		// ɱ������£����ֶε���0��ע����ֶ����ֽڸ���������tchar�ĸ���Ӵ
	BYTE	byInsAndActi;	// �������߻ö��ֵ
	BYTE	byContent[1];	// ���ֶ������Ŀ�ģ�������ɱ��Ŀ���£����û��ʵ�����壬TCHAR*
};

struct tagClientEasyTeamSimInfo
{
	BYTE	byTeamType;	// ���Ŀ�ģ�����ɱ�֣�EEasyTeamType����
	DWORD	dwLeaderID;	// �ӳ�id
	BYTE	byLeaderLevel;	// �ӳ��ȼ�
	WORD	wRoles;			// ����������ϵ�������ֽڣ����ֽ������������ֽڵ�ǰ����
	BYTE	byMemoLen;		// ɱ������£����ֶε���0��ע����ֶ����ֽڸ���������tchar�ĸ���Ӵ
	BYTE	byInsAndActi;	// �������߻ö��ֵ
	tstring strContent;
};

struct tagPersonalMode
{
	BYTE	byType;				// �������ͣ� EEasyTeamType
	DWORD	dwRoleID;			// ���ID
	BYTE	byLevel;			// ��ҵȼ�
	BYTE	byInsAndActi;		// �������߻ö��ֵ
	BYTE	byLen;				//
	BYTE	byContent[1];		// ��ע
};

struct tagClientPersonalMode
{
	BYTE	byType;				// �������ͣ� EEasyTeamType
	DWORD	dwRoleID;			// ���ID
	BYTE	byLevel;			// ��ҵȼ�
	BYTE	byInsAndActi;		// �������߻ö��ֵ
	BYTE	byLen;				//
	tstring	strContent;			// ��ע
};


// ��ȡ��������Ϣ���ͻ����ʵ����ƣ�ˢ�°�ť�ĵ��Ƶ�ʣ������Ӱ�����������
CMD_START( NC_GetEasyTeamInfo )
CMD_END

CMD_START( NS_GetEasyTeamInfo )
	WORD	wTeamNums;
	tagEasyTeamSimInfo	Teams[1];
CMD_END

// ��ȡ����δ������
CMD_START( NC_GetSinglePlayers )
CMD_END

CMD_START( NS_GetSinglePlayers )
	WORD	wNums;
	DWORD	dwRoleID[1];
CMD_END

// ��ȡ��������
CMD_START( NC_GetPersonalInfo )
CMD_END

CMD_START( NS_GetPersonalInfo )
	WORD	wNums;
	tagPersonalMode Person[1];
CMD_END

//  ɾ���Լ��Ķ�����Ϣ�������ñ��˿�����
CMD_START( NC_DeleteMyTeamInfo )
	DWORD	dwRoleID;
CMD_END

//  ɾ���Լ���������Ϣ
CMD_START( NC_DeleteAppTeamInfo )
	DWORD	dwRoleID;
CMD_END


//  �����������������Ϣ
CMD_START( NC_PutOutInfo )
	BOOL bTeam;				// TRUE, ������Ϣ��	FALSE, ������Ϣ
	BYTE byTeamType;
	BYTE byScene;
	BYTE byTeamTypeDetail;
	BYTE byTeamNum;
	BYTE byMinLevel;
	BYTE byMaxLevel;
	BYTE btLen;
	BYTE byContent[1];		// ��ע
CMD_END

CMD_START( NS_PutOutInfo )
	DWORD dwErrorCode;
CMD_END


//////////////////////////////////////////////////////////////////////////
// �����Ƿ���������֪ͨ�ͻ�����
enum EEasyTeamChangeType
{
	EETCT_NewTeam	= 0 , // ������
	EETCT_DelTeam	= 1 , // ɾ����
	EETCT_AddMem	= 2 , // ���ӳ�Ա
	EETCT_DelMem	= 3 , // ɾ����Ա
	EETCT_LeaderChange = 4, // �ƽ��ӳ�
};
CMD_START( NS_EasyTeamEvent )
	BYTE	byTeamEvent;	// ɾ����EEasyTeamChangeType
	BYTE	byTeamInfo[1];	// ����Ϣ������ɾ���ӣ����ֶ�û�����壻�����������ͣ����ֶξ���tagEasyTeamSimInfo*
CMD_END


#pragma pack(pop)

#endif // msg_easy_team_h__





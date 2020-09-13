//-----------------------------------------------------------------------------
// filename: msg_GroupChat.h
// author: Lamoair
// actor:
// data: 2010-6-4
// last:
// brief: Ⱥ����ϵͳ��Ϣ����
//-----------------------------------------------------------------------------

#pragma  once
#pragma pack(push, 1)

#define   MAX_GROUPCHAT_LEN      8 
#define  MAX_GROUPCHAT_ROLENUM   50


enum GROUP_CHAT
{
	GRC_AlreadyInGroupChat         =  1,      //�Ѿ���Ⱥ����
	GRC_AddRoleToGroupChatError    =  2,      //��ӽ�ɫ��Ⱥ��ʧ��
	GRC_LoginOutGroupChatError     =  3,      //�˳�Ⱥ��ʧ��
	GRC_GroupChatNameInvalid       =  4,      //����������Ч
	GRC_RoleNotOnline              =  5,      //��ɫ������
	GRC_RefuseJoinGroupChat        =  6,      //�ܾ�����Ⱥ
	GRC_GroupChatNotExist          =  7,      //Ⱥ������
    GRC_NotGroupChatMaster         =  8,      //����Ⱥ��û��Ȩ��
	GRC_AddRoleNotExist            =  9,      //����Ľ�ɫ������
	GRC_LoginOutGroupRoleNotExist  =  10,     //�ǳ�Ⱥ�Ľ�ɫ������
	GRC_GroupChatRoleOverMaxNum    =  11,     //Ⱥ��������޶�
};

struct tagGroupChatMember
{
	DWORD dwRoleId;
	TCHAR szRoleName[X_SHORT_NAME];
};
//-----------------------------------------------------------------------------
// ��������Ⱥ
//-----------------------------------------------------------------------------
CMD_START(NC_CreateGroupChat)
TCHAR szGroupChatName[MAX_GROUPCHAT_LEN];
CMD_END

CMD_START(NS_CreateGroupChat)
DWORD   dwErrorCode;
TCHAR   szGroupChatName[MAX_GROUPCHAT_LEN];
DWORD   dwGroupChatId;
CMD_END


//-----------------------------------------------------------------------------
// ɾ������Ⱥ
//-----------------------------------------------------------------------------
CMD_START(NC_DeleteGroupChat)
DWORD  dwGroupChatId;
CMD_END

CMD_START(NS_DeleteGroupChat)
DWORD  dwError ;
DWORD  dwGroupChatId;
CMD_END

//-----------------------------------------------------------------------------
// T������
//-----------------------------------------------------------------------------
CMD_START(NC_KickRoleFromGroupChat)
DWORD  dwGroupChatId;
DWORD  dwRoleId     ;          //��T����id
CMD_END

CMD_START(NS_KickRoleFromGroupChat)
DWORD  dwError ;
DWORD  dwRoleId;                //��T����id
DWORD  dwGroupChatId;
CMD_END

//-----------------------------------------------------------------------------
// �˳�����Ⱥ
//-----------------------------------------------------------------------------
CMD_START(NC_LoginOutGroupChat)
DWORD  dwGroupChatId;
CMD_END

CMD_START(NS_LoginOutGroupChat)
DWORD  dwError ;
DWORD  dwRoleId;                             //Ⱥ�������
DWORD  dwGroupChatId;                        //Ⱥ��id
DWORD  dwGroupChatMasterId ;                  //����id
CMD_END

//-----------------------------------------------------------------------------
// �����ɫ������Ⱥ
//-----------------------------------------------------------------------------
CMD_START(NC_InviteRoleToGroupChat)
DWORD   dwGroupChatId;                        //Ⱥ��id
DWORD   dwDestRoleID;                         //��������ID
CMD_END

CMD_START(NS_InviteRoleToGroupChat)
DWORD   dwError ;
TCHAR   szName[X_SHORT_NAME];                 //��ɫ����
CMD_END

CMD_START(NS_InviteRoleJoinToGroupChat)
DWORD   dwInviteRoleId ;                        //������id
DWORD   dwGroupChatId ;                         //Ⱥ��id
TCHAR   szGroupChatName[MAX_GROUPCHAT_LEN];
CMD_END

//-----------------------------------------------------------------------------
// �������ɫ�ӵ�����Ⱥ
//-----------------------------------------------------------------------------
CMD_START(NC_AddRoleToGroupChat)
DWORD   dwGroupChatId;                        //Ⱥ��id
BOOL    bIsAccepted;                          //�Ƿ����
DWORD   dwInviteRoleId;                       //������id
CMD_END

CMD_START(NS_AddRoleToGroupChat)
DWORD                dwError ;                             //�Ƿ��ܳɹ�����
TCHAR                szGroupChatName[MAX_GROUPCHAT_LEN];   //Ⱥ������
DWORD                dwGroupChatId;                        //Ⱥ��id
DWORD                dwGroupChatMasterId;                  //Ⱥ��id   
DWORD                dwMemberNum;                          //Ⱥ��Ա���� 
tagGroupChatMember   GroupChatMember[1];                 //ȺԱ������
CMD_END

//-----------------------------------------------------------------------------
// ��ɫ���뵽����Ⱥ
//-----------------------------------------------------------------------------
CMD_START(NS_RoleJoinToGroupChat)
DWORD    dwRoleID ;                                      //����Ⱥ�Ľ�ɫid
TCHAR    szName[X_SHORT_NAME];                           //��ɫ����
DWORD    dwGroupChatId;
CMD_END

//-----------------------------------------------------------------------------
// ��������Ⱥ��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_SendGroupChatMsg)
CMD_END

#pragma pack(pop)
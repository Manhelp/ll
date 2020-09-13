#pragma once

#include "..\WorldDefine\msg_GroupChat.h"

#pragma pack(push, 1)
#define      MAX_GROUPCHAT_PAGE_SHOW    512
#define      PLAYER_MAX_CREATE_GROUP_NUM  5
#define      GROUP_MAX_MEMBER_NUM		   20
//����Ⱥ�Ľṹ
struct tagLocalChatGroup
{
	DWORD							dwGroupChatId;							//Ⱥ��id
	DWORD							dwGroupChatMasterId;                  //Ⱥ��id   
	TCHAR							szGroupChatName[MAX_GROUPCHAT_LEN];		//Ⱥ������
	DWORD							dwMemberNum;                         //Ⱥ��Ա���� 
	std::list<tagGroupChatMember*>	GroupChatMember;						//Ⱥ��Ա������
	TCHAR							szChatContent[MAX_GROUPCHAT_PAGE_SHOW];  //�����������
	tagLocalChatGroup()
	{
		dwGroupChatId = 0;
		dwGroupChatMasterId = 0;
		dwMemberNum = 0;
		ZeroMemory(szGroupChatName,sizeof(szGroupChatName));
		ZeroMemory(szChatContent,sizeof(szChatContent));
	}
};

struct tagGroupChatButtonGlin: public tagGameEvent
{
	tagGroupChatButtonGlin():tagGameEvent(_T("tagGroupChatButtonGlin"),NULL){}
	tagGroupChatButtonGlin(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

#pragma pack(pop)
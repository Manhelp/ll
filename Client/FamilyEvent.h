#pragma once
#include "..\WorldDefine\family_define.h"
#pragma pack(push, 1)

//����������
struct tagFamilyInviteEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	tstring	strRoleName;
	tagFamilyInviteEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID )
	{}
};

//����������
struct tagFamilyApplyEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	tstring	strRoleName;
	tagFamilyApplyEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID )
	{}
};

//�����������Ϣ
struct tagFamilyEvent : public tagGameEvent
{
	EFamilyEventType eEventType;
	DWORD			 dwRoleIndex; //������Ա������
	tagFamilyEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame),dwRoleIndex(GT_INVALID),eEventType(EFET_NoEvent)
	{}
};

//�����������Ϣ
struct tagFamilyFrameEvent : public tagGameEvent
{
	BOOL	bShowFrame;
	tagFamilyFrameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame),bShowFrame(FALSE)
	{}
};

//�����������Ϣ
struct tagFamilyNameEvent : public tagGameEvent
{
	tstring	strFamilyName;
	tagFamilyNameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame)
	{}
};

#pragma pack(pop)
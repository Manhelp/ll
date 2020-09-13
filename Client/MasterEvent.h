/********************************************************************
purpose:	ʦͽϵͳ��Ϸ�¼�����
*********************************************************************/
#pragma once

#pragma pack(push, 1)

// ��ʦ����
struct tagGetAquireMasterEvent : public tagGameEvent
{
    DWORD roleID;		// ���ID

    tagGetAquireMasterEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame)
        , roleID(GT_INVALID)
    { } 
};

// ��ͽ����
struct tagAquireApprenticeEvent : public tagGameEvent
{
    DWORD roleID;		// ���ID

    tagAquireApprenticeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame)
        , roleID(GT_INVALID)
    { } 
};


// ��ͽ�ɹ���
struct tagAddStudentEvent : public tagGameEvent
{
	DWORD roleID;		// ���ID

	tagAddStudentEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// ��ʦ�ɹ���
struct tagGetMasterEvent : public tagGameEvent
{
	DWORD roleID;		// ���ID

	tagGetMasterEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// ���ʦ��
struct tagBanStudentEvent : public tagGameEvent
{
	DWORD roleID;		// ���ID

	tagBanStudentEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// ����ʦ��
struct tagBaEvent : public tagGameEvent
{
	DWORD roleID;		// ���ID

	tagBaEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};


#pragma pack(pop)
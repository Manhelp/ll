#pragma once
#include "..\WorldDefine\family_define.h"

class FamilyFrame;

struct tagRoleFamilyInfo;

struct tagNS_CreateFamilyBroad;
struct tagNS_FamilyDismiss;
struct tagNS_FamilyJoinReq;
struct tagNS_FamilyJoinReqRes;
struct tagNS_FamilyJoinBroad;
struct tagNS_FamilyApply;
struct tagNS_FamilyApplyReqRes;
struct tagNS_FamilyHandOverBroad;
struct tagNS_FamilyKickBroad;
struct tagNS_FamilyLeaveBroad;
struct tagNS_GetFamilyInfo;
struct tagNS_GetFamilyName;
struct tagNS_FamilyProcFailed;
struct tagNS_FamilySpritTrainBroad;
struct tagNS_CreateFamilyAllBroad;

struct tagFamilyInviteEvent; 
struct tagFamilyApplyEvent;
struct tagFamilyEvent;

struct tagNS_OpenFamilyQuestPage;

struct tagTodayFamilyQuest
{
	DWORD QuestID;
	DWORD dwCurDegree;	//�����
	tagTodayFamilyQuest& operator= (const tagTodayFamilyQuest& tag)
	{
		QuestID = tag.QuestID;
		dwCurDegree = tag.dwCurDegree;
		return *this;
	}
};

struct tagFamilyQuestDisplay
{
	tagTodayFamilyQuest			m_sFamilyQuestInfo[MAX_FAMILY_QUESTS_NUM];
	DWORD						m_dwYesterdayDegree;	//������ɶ�
	BOOL						m_bIsGetReward;			//�Ƿ���ȡ����Ľ���
};

class FamilyMgr
{
public:
	FamilyMgr(void);
	~FamilyMgr(void);
	void Init();
	void Destroy();
	static FamilyMgr* Inst();
// common----------------------------------------------------------------------------------------------------
	enum ENameUsedType
	{
		ENUT_Nothing = 0,	//�������������룬��ʹ�� 
		ENUT_Invite,		//��������
		ENUT_Apply,			//��������
		ENUT_Frame,			//���ڽ�����ʾ
	};
	struct tagID2Name
	{
		DWORD dwRoleID;
		tstring    strName;
	};
	typedef		std::map<DWORD, tagRoleFamilyInfo*>		FamilyMemberMap;
// �����Ա----------------------------------------------------------------------------------------------------
	//������Ϣ����
	VOID SendFamilyInfoReq(DWORD dwID);
	//�����Ѿ����Ź���ID����(key ��0��ʼ��0Ϊ�峤,֮����������õ�)
	const std::map<DWORD, DWORD>& GetFamilyMemberIDList(){ return m_mapFamilyMember;}
	//ID������صĳ�Ա��Ϣ
	const tagRoleFamilyInfo* GetMemberByID(DWORD dwID);
	//�����峤ID
	DWORD GetFamilyLeaderID();
	//��Ա����
	INT GetFamilyMemberNum(){ return (INT)m_mapFamilyMember.size();}
	//��������ü�������
	tstring GetFamilyName(DWORD dwFamilyID,ENameUsedType eType=ENUT_Nothing ,DWORD dwID=GT_INVALID, tstring roleName = _T(""),bool bSendCmd = true );
	//�жϽ�ɫ�Ƿ����Լ���ͬһ����
	bool JudgeIsSameFamily(DWORD dwRoleID);

// ��������----------------------------------------------------------------------------------------------------
	tagFamilyQuestDisplay* GetFamilyQuestInfo()
	{
		return &m_pFamilyQuestInfo;
	}

	BOOL IsFamilyQuestMonster(DWORD dwID);

// ��������----------------------------------------------------------------------------------------------------
	VOID SetSpriteAtt(int pos, DWORD dwValue){m_dwSpriteAtt[pos] = dwValue;}
	DWORD GetSpriteAtt(int pos){return m_dwSpriteAtt[pos];}

	VOID SetSpriteLevel(DWORD dwLevel){m_dwSpriteLevel =dwLevel; }
	DWORD GetSpriteLevel(){return m_dwSpriteLevel;}
private:
	TSFPTrunk<FamilyMgr>				m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	// common----------------------------------------------------------------------------------------------------
	FamilyFrame * m_pFamilyFrame;
// �����Ա----------------------------------------------------------------------------------------------------
	//---����---
	std::set<DWORD>								m_setNameReq;		//�����������󻺴��
	std::map<DWORD, tstring>				m_mapFamilyName;	//����map
	std::map<DWORD, tagID2Name>		m_mapInivteCache;	//���뻺��,<����ID��������ID>
	std::map<DWORD, DWORD>			m_mapApplyCache;	//���뻺��,<����ID��������ID>
	std::map<DWORD, DWORD>			m_mapFamilyMember;	//��ͥ��Ա����<��ţ�ID>�����ڻ���������Ŵ�0��ʼ��0Ϊ�ҳ���
	FamilyMemberMap							m_mapFamilyInfo;	//��ͥ��Ա�����Խṹ(����)
	BOOL												m_bMsgBoxMutex;  // �����򻥳�
	//---����---
	DWORD OnEventFamilyCreate( tagGameEvent* pGameEvent );
	DWORD OnEventFamilyBreak( tagGameEvent* pGameEvent );
	DWORD OnEventFamilyInvite( tagFamilyInviteEvent* pGameEvent );
	DWORD OnEventFamilyApply( tagFamilyApplyEvent* pGameEvent );
	DWORD OnEventFamilyEvent( tagFamilyEvent* pGameEvent );

	DWORD OnMsgbox_CreateFamily( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_InputFamilyName( tagMsgInputBoxEvent* pGameEvent );
	DWORD OnMsgBox_BreakFamily( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_FamilyJoinReq( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_FamilyApplyReq( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_FamilyEvent( tagMsgBoxEvent* pGameEvent );

	DWORD OnNS_FamilyProcFailed(tagNS_FamilyProcFailed* pNetCmd, DWORD);
	DWORD OnNS_CreateFamilyBroad(tagNS_CreateFamilyBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyDismiss(tagNS_FamilyDismiss* pNetCmd, DWORD);
	DWORD OnNS_FamilyJoinReq(tagNS_FamilyJoinReq* pNetCmd, DWORD);
	DWORD OnNS_FamilyJoinReqRes(tagNS_FamilyJoinReqRes* pNetCmd, DWORD);
	DWORD OnNS_FamilyApply(tagNS_FamilyApply* pNetCmd, DWORD);
	DWORD OnNS_FamilyApplyReqRes(tagNS_FamilyApplyReqRes* pNetCmd, DWORD);
	DWORD OnNS_FamilyJoinBroad(tagNS_FamilyJoinBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyHandOverBroad(tagNS_FamilyHandOverBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyKickBroad(tagNS_FamilyKickBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyLeaveBroad(tagNS_FamilyLeaveBroad* pNetCmd, DWORD);
	DWORD OnNS_GetFamilyName(tagNS_GetFamilyName* pNetCmd, DWORD);
	DWORD OnNS_GetFamilyInfo(tagNS_GetFamilyInfo* pNetCmd, DWORD);
	DWORD OnNS_CreateFamilyAllBroad(tagNS_CreateFamilyAllBroad* pNetCmd, DWORD);

	//��ʾ������
	VOID ShowErrorMsg(DWORD dwErrorCode);	
	//kill obj
	VOID  KillObj_NPCTalk();
	//���������
	BOOL  ShowInvite(DWORD dwFamilyID,DWORD dwInviterID,tstring strSrcName = _T("") );
	//���������
	BOOL  ShowApply(DWORD dwApplyID, tstring strAppRoleName);
	//�ڼ���Ƶ�������Ϣ
	void  ShowFamilyMsg(const TCHAR* szFormat,...);
	//��ռ�����Ϣ
	VOID ClearFamilyInfo();
	//��������������Ϣ
	VOID SendFamilyNameReq(DWORD dwID);
	//��ռ����Ա
	VOID ClearFamilyMemberMap();
	
	
// ��������----------------------------------------------------------------------------------------------------
	DWORD OnNS_OpenFamilyQuestPage(tagNS_OpenFamilyQuestPage* pCmd, DWORD);

	tagFamilyQuestDisplay m_pFamilyQuestInfo;

// ��������----------------------------------------------------------------------------------------------------
	DWORD OnNS_FamilySpritTrainBroad(tagNS_FamilySpritTrainBroad* pCmd, DWORD);

	DWORD m_dwSpriteAtt[EFSA_Num];		// ��ǰ��attֵ 

	DWORD m_dwSpriteLevel;

};

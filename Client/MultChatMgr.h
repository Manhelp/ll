#pragma once
#include "MultChatData.h"
#include "..\WorldDefine\chat.h"
#include "..\WorldDefine\chat_define.h"

class MultChatMgr
{
public:
	MultChatMgr(void);
	~MultChatMgr(void);

public:
	static MultChatMgr* Inst();
	void	Init();
	void	Destroy();
//---------------------------������Ϸ�¼�------------------------------------
	DWORD OnOpen_MultChatFrame(tagGameEvent* pEvent);
	DWORD OnClose_MultChatFrame(tagGameEvent* pEvent);
//---------------------------������ѡ����Ӧ����-------------------------------------
	//���õ�ǰѡ�е�Ⱥ���ɵ�ѡ��������λ
	VOID	SetCurPickGroupID( INT index );			 
	//�õ���ǰѡ�е�Ⱥ
	DWORD	GetCurPickGroupID() const;			
	//�õ���ǰѡ�е�Ⱥ��ID,����Ⱥ��ID��-1�����б�գ���0����ЧID��
	DWORD	GetCurPickGroupMasterID() const;	 
	//���õ�ǰѡ�еĳ�Ա
	VOID	SetCurPickMemberID( INT index  );			 
	 //�õ���ǰѡ�еĳ�Ա���ɵ�ѡ��������λ
	DWORD	GetCurPickMemberID() const;	
	//�õ���Ⱥ��ȺԱ��Ŀ
	INT		GetMemberNum(DWORD id) const;
//---------------------------�������������Ϣ�İ�����Ӧ����-------------------------
	VOID   CreateNewGroup(const tstring&);				 //�����µ�Ⱥ����
	VOID   DeleteGurGroup();							 //ɾ��һ��������
	VOID   QuitCurGroup();							    //�˳�����Ⱥ
	VOID   KickMember(INT index);								//�޳�ĳ�����
	VOID   InviteMember(const DWORD);					//�����߷�������
	VOID   AddNewMember(tagMsgBoxEvent* pGameEvent);	//�������߷����Ƿ��������
	VOID   SendMessage(const tstring&, INT );			//������Ϣ
//----------------------------��Frame��������----------------------------
	//���������б�
	std::vector<LPCTSTR>&  GetFriendsNameList();
	//����ID�б�
	std::vector<DWORD>&    GetFriendsIDList();
	//���Ⱥ�б�
	std::vector<DWORD>&    GetGroupIDList();
	//��ó�Ա�б�
	std::vector<DWORD>&   GetMemberIDList();
	//���Ⱥ���ܽṹ
	std::map<DWORD,tagLocalChatGroup*>& GetGroupChatData() { return m_mapGroupChatData; }
	//��ԱIDת����
	const TCHAR*  MemberID2Name(DWORD);
	//ȺIDת����
	const TCHAR*  GroupID2Name(DWORD);
	//���ص�ǰȺ����������
	const TCHAR*   GetCurGroupChatContent();
	//�ҵ���ӦIDȺ��Ⱥ��ID
	DWORD GetGrouptMasterID(DWORD dwGroupID);
	//�����˸Ⱥ
	VOID  AddGlintGroup(DWORD id);
	//������˸Ⱥ
	VOID  DelGlintGroup(DWORD id);
	//�����˸ȺID����
	std::vector<DWORD>& GetGlintGroup(){ return m_vecGroupClint; }
	//��ID��Ⱥ�Ƿ���Ҫ��˸
	BOOL IfNeedGlint(DWORD id);
	//�Ƿ���ҪFrame����
	BOOL  IsNeedRefresh() 
	{ 
		BOOL  b = m_bIsNeedRefresh;
		m_bIsNeedRefresh = FALSE;
		return  b; 
	} 

	VOID SetFrameRefresh( BOOL b)
	{
		m_bIsNeedRefresh = b;
	}
private:
	BOOL  DealWithErrorCode(DWORD); //�����봦����
//---------------------------���ܷ�����������Ϣ����Ӧ����---------------------------
	//�����µ�Ⱥ����
	DWORD NetRecCreateNewGroupMsg( tagNS_CreateGroupChat* pNetCmd, DWORD );
	//ɾ��һ��������
	DWORD NetRecDeleteGurGroupMsg( tagNS_DeleteGroupChat* pNetCmd, DWORD );
	//�˳�����Ⱥ
	DWORD NetRecQuitCurGroupMsg( tagNS_LoginOutGroupChat* pNetCmd, DWORD );
	//�޳�ĳ�����
	DWORD NetRecKickMemberMsg( tagNS_KickRoleFromGroupChat* pNetCmd, DWORD );
	//���������յ���Ϣ
	DWORD NetRecInviteMemberMsg( tagNS_InviteRoleJoinToGroupChat* pNetCmd, DWORD );
	//�����³�Ա,�б��м����³�Ա�������ߡ��������ߣ�����Ⱥ��Ա��
	DWORD NetRecNewMemberJoinMsg( tagNS_AddRoleToGroupChat* pNetCmd, DWORD );
	//���շ��������͵���Ϣ,����-1��ʾ��Ⱥ����Ϣ������0Ϊ��ERROR
	DWORD NetRecNewMessage( tagNS_RoleChat* pNetCmd, DWORD );
	//Ⱥ��Ա���յ������³�Ա
	DWORD NetRecAddNewMemberMsg( tagNS_RoleJoinToGroupChat* pNetCmd, DWORD );
	//������������������ߵķ���
	DWORD NetRecInviteRoleToGroupChat( tagNS_InviteRoleToGroupChat* pNetCmd, DWORD );
	//���º��ѵ�ID��name�б���ʹ�������±��Ӧ
	VOID  UpdateFriendsList(); 
	VOID  KillObjs();
	//�����Ƿ���Ҫˢ�µ�BOOL
	BOOL  RefreshGroupAndMemberList();

private:
	TSFPTrunk<MultChatMgr>					m_trunk;
	TObjRef<NetCmdMgr>						m_pNetCmdMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>		           m_pFrameMgr;
	
	std::map<DWORD,tagLocalChatGroup*>		m_mapGroupChatData;     //���ص�Ⱥ��map��key��ȺID��ֵ��Ⱥ�Ľṹ
	DWORD									m_dwCurChatGroupID;     //ѡ�еĵ�ǰ����ȺID
	DWORD									m_dwCurChatMemberID;    //ѡ�еĵ�ǰȺ��ԱID
	BOOL                                 m_bIsNeedRefresh;
	std::vector<DWORD>						m_vecGroupIndex;		//ȺID����					
	std::vector<DWORD>						m_vecMemberIndex;		//��ԱID����
	
	//���������б��ID�б���ͬ������±���ͬ,UpdateFriendsList()��get������ʵʱ����
	std::vector<LPCTSTR>					m_vecFriendsArray;		//���������б�
	std::vector<DWORD>						m_vecFriendsIDList;		//����ID�б�

	INT										m_iPlayerCreateGroupNum; //������Ҵ���Ⱥ����Ŀ
	std::vector<DWORD>						m_vecGroupClint;		 //��˸��Ⱥ	

};

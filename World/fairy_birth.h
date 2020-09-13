/********************************************************************
	created:	2010/12/07
	created:	7:12:2010   14:44
	filename: 	e:\F-Project\src\World\fairy_birth.h
	file path:	e:\F-Project\src\World
	file base:	fairy_birth
	file ext:	h
	author:		Jason
	
	purpose:	������ֳ������ع�����
*********************************************************************/

#ifndef fairy_birth_h__
#define fairy_birth_h__


struct tagFairyInviteBirthData
{
	DWORD	dwInvitorID;		// ������ID
	DWORD	dwInviteeID;		// ��������ID
	UINT64	u64InvitorFairyID;	// ����������
	UINT64	u64InviteeFairyID;	// ������������
	DWORD	dwTimeout;
	BOOL Isvalid()
	{
		return P_VALID(dwInvitorID) && P_VALID(dwInviteeID) && u64InvitorFairyID > 0 && u64InviteeFairyID > 0;
	}
	BOOL IsTimeout()
	{
		return dwTimeout < 1 ;
	}
	VOID ResetTimeout()
	{
		dwTimeout = TICK_PER_SECOND * 30;
	}
};

class FairyMatingMgr
{
	std::map<DWORD,tagFairyInviteBirthData *>	m_FairyInviteBirthData;	// ���������Ϣ
	std::map<DWORD,DWORD>	m_inviteeInvitor;	// �������� �����Ϣ

	static tagFairyInviteBirthData * _make();
	static void	_destroy(tagFairyInviteBirthData * pData);

	int		m_nUpdateCounter;

	FairyMatingMgr();
	~FairyMatingMgr();

	VOID ClearInnerData();
	bool _mod(DWORD dwInvitorID,tagFairyInviteBirthData * pData,bool _2=false);
	bool _remove(DWORD dwInvitorID,bool _2=false);
	tagFairyInviteBirthData * _find(DWORD dwInvitorID,bool _2=false) const;
public:
	static FairyMatingMgr* Create();
	static VOID Destroy();

	// ������4������Ӧ���ڵ�ͼ�̵߳���
	tagFairyInviteBirthData * Find(DWORD dwInvitorID) const;
	VOID Mod(DWORD dwInvitorID,tagFairyInviteBirthData * pData);
	VOID Remove(DWORD dwInvitorID);
	tagFairyInviteBirthData * Add(DWORD dwInvitorID);
	// ��ʱ���£�֪ͨ������
	// �����������߳��е���
	VOID	Update();
};
class Role;
struct tagFabao;
// ��ȡ�������������Ϣ��ֻҪ�ú�������TRUE����Щָ������϶�����Ч�ģ�����᷵��FALSE��������������������ݣ��ɵ�������������ɾ��
extern BOOL GetFabaoMatingOtherInfo(Role* pRole,Role *& pOther,tagFabao *& pMyFabao , tagFabao *& pHisFabao ,tagFairyInviteBirthData *& pData ,FairyMatingMgr*& pMgr );
// ��ȡĳ����ɫ�Ĵ���������
extern BOOL GetFabaoMatingFromRole( Role * pRole, INT nStatus, tagFabao *& pMyFabao );


#endif // fairy_birth_h__





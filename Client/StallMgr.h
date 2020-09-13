#pragma once

class TemporaryContainer;
class Item;
class LocalPlayer;
struct tagTemporaryItem;
struct tagRolePickEvent;
struct tagVIPStall;
struct tagNS_StallStart;
struct tagNS_StallClose;
struct tagNS_StallGetTitle;
struct tagNS_StallBuyRefresh;
struct tagNS_GetAllVIPStallInfo;
struct tagNS_UpdateVIPStallInfo;
struct tagNS_StallSetFinish;
struct tagRoleGetNameEvent;
struct tagNS_ItemRemove;
struct tagNS_GetRoleInitState_ClientConfig;

struct tagStallAppearEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	Vector3 vPos;
	FLOAT	fYaw;
	BYTE    byStallLevel;

	tagStallAppearEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagStallUpdateItemEvent : public tagGameEvent
{
	INT16	n16Pos;

	tagStallUpdateItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};


struct tagStallStarEvent : public tagGameEvent
{
	INT nLevel;
	tagStallStarEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}

};

enum EStallActionType
{
	E_Stall_Std = 0,
	E_Stall_Idle,
	E_Stall_Wel,
	E_STALL_Bye,
};

struct tagStall
{
	DWORD			dwRoleID;		// ˭��̯λ
	BYTE			byLevel;		// ̯λ�ȼ�
	BYTE			byType;			// ��������
	BYTE			byDummy[2];		
	ObjectSpace		space;			// ̯λ3Dλ��
	EntityNode*		pNode;			// ̯λģ��
	TCHAR			szName[X_SHORT_NAME]; // ̯λ����
    bool            bCustomName;    // �Ƿ��Զ���̯λ����
	bool			bIdle;
	bool			bWel;
	bool			bBye;
	DWORD			dwStartTime;
	DWORD			dwEscapeTime;

    tagStall() :
        dwRoleID(GT_INVALID),
        byLevel(0),
        byType(0),
        pNode(0),
        bCustomName(false),
		dwStartTime(0),
		dwEscapeTime(0)
    {
        byDummy[0]  = 0;
        byDummy[1]  = 0;
        szName[0] = 0;
    }

	VOID PlayAction( EStallActionType eType )
	{
		if( !P_VALID(pNode) )
			return;
		dwStartTime = timeGetTime();
		switch(eType)
		{
		case E_Stall_Std:
			pNode->GetAniCtrl()->OpenTrack(_T("std"), true );
			bIdle = false;
			bWel = false;
			bBye = false;
			dwStartTime = 0;
			break;
		case E_Stall_Idle:
			pNode->GetAniCtrl()->OpenTrack(_T("lei"), false );
			bIdle = true;
			bWel = false;
			bBye = false;
			break;
		case E_Stall_Wel:
			pNode->GetAniCtrl()->OpenTrack(_T("open"), false );
			bIdle = false;
			bWel = true;
			bBye = false;
			dwEscapeTime = 0;
			break;
		case E_STALL_Bye:
			pNode->GetAniCtrl()->OpenTrack(_T("clos"), false );
			bIdle = false;
			bWel = false;
			bBye = true;
			dwEscapeTime = 0;
			break;
		default:
			pNode->GetAniCtrl()->OpenTrack(_T("std"), true );
			bIdle = false;
			bWel = false;
			bBye = false;
			dwStartTime = 0;
			break;
		}
	}

	BOOL IsTrackEnd( EStallActionType eType )
	{
		if( !P_VALID(pNode) )
			return false;
		switch(eType)
		{
		case E_Stall_Idle:
			return bIdle && pNode->GetAniCtrl()->IsTrackEnd(_T("lei"));
		case E_Stall_Wel:
			return bWel && pNode->GetAniCtrl()->IsTrackEnd(_T("open"));
		case E_STALL_Bye:
			return bBye && pNode->GetAniCtrl()->IsTrackEnd(_T("clos"));
		default:
			return false;
		}
		return false;
	}
};

struct tagVipStallEvent : public tagGameEvent
{
	DWORD			dwRoleID;		// vip��̯λ
	BYTE			byIndex;		// vip̯λ���

	tagVipStallEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
	{
		dwRoleID = GT_INVALID;
		byIndex	 = GT_INVALID;
	}
};

class StallMgr
{
public:
	enum EOperateStallType
	{
		EOperateStallType_NULL		= -1,
		EOperateStallType_Normal,				//�����򿪷�ʽ
		EOperateStallType_Vip,					//VIP�򿪷�ʽ
	};

	struct tagStallModelConfig
	{
		int nStallLevel;	// ̯λ�ȼ�
		int nModel;			// ̯λģ��
		int nSignBoard;		// ����ģ��
	};

	typedef std::map<int, tagStallModelConfig> StallModelConfigMap;

public:
	StallMgr(void);
	~StallMgr(void);

	VOID Init();
	VOID Destroy();
	VOID Update();

	tagStall* PickStall(const Ray& rayMouse);

	BOOL AddItem(Item* pItem, INT16 &nIndex, INT64 n64Price = 0);
	BOOL DelItem(INT16 nIndex, BOOL bUnlock = TRUE);
	tagTemporaryItem* GetItem(INT16 nIndex);

	tstring GetStallStars(INT64 nTotalTax);
	
	VOID ShowStallErrorMsg(DWORD dwErrorCode);

	BOOL IsStallState();

	BOOL IsOutValidDist();
	
	// �Ƿ���԰�̯
	BOOL CanStall(BYTE byLevel);

	// ����/��ȡ̯λ����
	VOID SetStallName(DWORD dwRoleID, LPCTSTR szName, bool bCustom);
	LPCTSTR GetStallName(DWORD dwRoleID);

    // ������NameTable��ɫ�����¼����ش���
    DWORD OnGetPlayerName(tagRoleGetNameEvent* pGameEvent);

	// ����/��ȡ������ҹ���־λ
	VOID SetStallAdFlag(BOOL bFlag){m_bSetAdFlag = bFlag;}
	BOOL GetStallAdFlag(){return m_bSetAdFlag;}

	// ��ȡ̯λ�б�
	TMap<DWORD,tagStall*>& GetStallMap() { return m_mapStall; }

	//���ô�̯λ��ʽ
	void SetOperateStallType(EOperateStallType eType=EOperateStallType_Normal){ m_eOperateStallType = eType; }
	EOperateStallType GetOperateStallType() { return m_eOperateStallType; }

	//--���vip̯λ״̬
	BOOL IsCanCheckVipStall(BYTE byIndex);
	BOOL IsCanRentVipStall(BYTE byIndex);

	//����Լ���û��vip̯λ
	BOOL IsSelfVipStall();

	//ˢ��VIP̯����ʾ
	void RefreshVipStallState(DWORD dwRoleID);

	// ��ȡvip̯λ��Ϣ
	map<BYTE, tagVIPStall>& GetVipStallInfo() { return m_mapVipStall; }
	tagVIPStall* FinVipStallInfo(BYTE byKey);

	static StallMgr* Inst();

	// ����̯λ�ȼ�ȡ��̯λģ��·��
	tstring GetStallModelPath(int nStallLevel);

	// ����̯λ�ȼ�ȡ������·��
	tstring GetStallSignBoardPath(int nStallLevel);
	void UpdateVipStallInfo();
	INT16 GetPosition(INT64 n64Serial);
	VOID SetNPCID( DWORD dwID ) { m_dwVIPNPC = dwID; }

private:

	DWORD OnEventStallAppear(tagStallAppearEvent* pGameEvent);
	DWORD OnEventStallDisappear(tagStallAppearEvent* pGameEvent);
	DWORD OnEventPickStall(tagRolePickEvent*	pGameEvent);
	DWORD OnEventOpenStall(tagGameEvent* pGameEvent);
	DWORD OnEventCloseStall(tagGameEvent* pGameEvent);
	DWORD OnEventCloseVend(tagGameEvent* pGameEvent);
	DWORD OnEventOpenVipStall(tagGameEvent* pGameEvent);
	DWORD OnCheckVipStallEvent(tagVipStallEvent* pGameEvent);

	DWORD OnNetStallStart(tagNS_StallStart* pNetCmd, DWORD);
	DWORD OnNetStallClose(tagNS_StallClose* pNetCmd, DWORD);
	DWORD OnNetStallGetTitle(tagNS_StallGetTitle* pNetCmd, DWORD);
	DWORD OnNetStallBuyRefresh(tagNS_StallBuyRefresh* pNetCmd, DWORD);
	DWORD OnNetGetAllVIPStallInfo(tagNS_GetAllVIPStallInfo* pNetCmd, DWORD);
	DWORD OnNetUpdateVIPStallInfo(tagNS_UpdateVIPStallInfo* pNetCmd, DWORD);
	DWORD OnNetStallSetFinish( tagNS_StallSetFinish* pNetCmd, DWORD );
	DWORD OnNetRecvItemUpdate(tagNS_ItemRemove* pMsg, DWORD pPrama);
	DWORD OnNS_GetRoleInitState_ClientConfig(tagNS_GetRoleInitState_ClientConfig* pMsg, DWORD pPrama);

	BOOL LoadStallModelConfig(tstring strConfigPath);


	VOID SendToGetStallTitle(DWORD dwRoleID);

	// ��ʼ��̯λ����
	VOID InitStallContainer();	
	// �洢����������������
	VOID RecordStallList();	

	VOID OpenStallShelf();
	VOID CloseStallShelf();
	VOID OpenStallVend();
	VOID CloseStallVend();

	BOOL ShowStall(tagStall* pStall);
	VOID HideStall(tagStall* pStall);

	BOOL IsAABBCollideStall(const AABBox &box);

	BOOL IsAABBCollideGround(const AABBox &box, Vector3 &wpt);
	
	// ����̯λ�ȼ���ȡһ��̯λ��Ӧ��AABBox
	VOID GetStallAABBox(AABBox& out, Vector3& wpt, BYTE byLevel);

	VOID SendStallTitleRequest();

private:
	TSFPTrunk<StallMgr>			m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	TMap<DWORD, tagStall*>		m_mapStall;	
	TemporaryContainer*			m_pContainerStall;
	TMap<INT64, INT64>			m_mapStallItem;
	LocalPlayer*				m_pLp;
	DWORD						m_dwStallRoleID;
	tstring						m_strLPStallName;
	BOOL						m_bSetAdFlag;

	//--VIP̯λ
	EOperateStallType			m_eOperateStallType;
	map<BYTE, tagVIPStall>		m_mapVipStall;
	DWORD						m_dwVipTime;
	
	StallModelConfigMap			m_mapStallModelConfigs;
	pair<DWORD, list<DWORD> >	m_paReqRole;	

	DWORD						m_dwVIPNPC;	
	DWORD						m_dwStallTileSpace;
};

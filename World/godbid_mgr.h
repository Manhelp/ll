//-----------------------------------------------------------------------
// ��֮����ϵͳ
//-----------------------------------------------------------------------

#pragma once

struct tagGodBidProto;
struct tagGodBidInfo2DB;

#define GodBid_Time_limit 3600		// ��֮�������ʱ�ޣ���λΪ�룩

const DWORD GODBID_START_BROADCAST					= 300011;		// ��֮������ʼ
const DWORD GODBID_END_BROADCAST						= 300012;		// ��֮��������
const DWORD GODBID_NORMAL_END_BROADCAST		= 300013;		// ��ϲxx��xx�𾺵�xx��Ʒ
const DWORD GODBID_BLACK_GOLD_BROADCAST		= 300014;		// ����--��ϲxx��xx�𾺵�xx��Ʒ
const DWORD GODBID_BLACK_SHENGBI_BROADCAST	= 300015;		// ����--��ϲxx��xxʥ�Ҿ���xx��Ʒ
const DWORD GODBID_ALL_END_BROADCAST				= 300016;		// ����������Ʒ�ѷ������ٱ������죬δ������Ʒ����ң������ʽ���ȫ��������������

// ���꾺����Ϣ
struct tagBidInfo 
{
	DWORD	dwRoleID;		// ��ɫID
	INT64		n64Bidden;		// ����
};

typedef TMap<DWORD, INT64> BidInfoMap;
typedef TList<INT16>	BidStartTimeList;

// ���꾺����Ϣ�ڵ�
struct tagBidInfoNode
{
	DWORD			dwGodBidProtoID;
	tagBidInfo		CurHighestBidInfo;				// ��ǰ��ߵĳ�����Ϣ
	BidInfoMap*	pBidInfoMap;

	tagBidInfoNode()
	{
		dwGodBidProtoID = GT_INVALID;
		CurHighestBidInfo.dwRoleID = 0;
		CurHighestBidInfo.n64Bidden = 0;
		pBidInfoMap = NULL;
	}
};

class GodBidMgr
{
public:
	GodBidMgr();
	~GodBidMgr();

	VOID Update();

	DWORD GetBidEndLeftTime();

	// ��ʼ�����꿪ʼʱ��list
	VOID InitFixedBidStartTimeList(TObjRef<VarContainer>& pVar, BOOL bFixed);
	VOID InitGodBidSwitch(TObjRef<VarContainer>& pVar);

	BOOL IsBidTime() { return m_bBidTime; }
	VOID SetIsBidTime(BOOL bBidTime) { m_bBidTime = bBidTime; }
	BOOL IsSwitchOn() { return m_bSwitchOn; }
	VOID SetBidEnding(BOOL bBidEnding) { m_bBidEnding = bBidEnding; }
	BOOL IsBidEnding() { return m_bBidEnding; }
	VOID ClearGodBidInfoFromMemory();

	BOOL ReLoadGodBidProto();

	// ����ͻ�������
	VOID SendNormalBidInfo2Client(Role* pRole);
	VOID SendBlackMarketBidInfo2Client(Role* pRole);
	VOID ProcessNormalBidConformPrice(DWORD dwProtoID, Role* pRole, INT64 n64Price);
	VOID ProcessBlackBidConformPrice(DWORD dwProtoID, Role* pRole, INT64 n64Price);

	// �������ݿ����
	VOID InitGodBidInfoFromDB(DWORD dwBidCount, tagGodBidInfo2DB* pGodBidInfo2DB);
	VOID SaveOneGodBidInfo2DB(DWORD dwRoleID, INT64 n64Bidden, BYTE byBidType, DWORD dwBidID);
	VOID ClearGodBidInfoFromDB(BOOL bBidEnd);
	VOID LoadGodBidInfoFromDB();

	VOID BidEnd();

private:
	VOID BidStart();
	BOOL CheckCurIsBidTime();
	BOOL CheckIsBidTime(BidStartTimeList& listBidStartTime, tagDWORDTime dwTime);
	VOID InitBidItems();
	VOID SendBroadcast(DWORD dwBroadcastID);
	VOID SendBroadcast(DWORD dwBroadcastID, DWORD dwRoleID, DWORD dwMoney, DWORD dwItemID);
	VOID NormalBidEnd();

	// ע�⣬���ô˷���Ҫ���ϲ���eGodBidType��dwIdx�ĺϷ���
	VOID EachBidEnd(EGodBidType eGodBidType, DWORD dwIdx = GT_INVALID);

	void GiveMoneyBack2Player( BidInfoMap* pBidInfoMap, EGodBidType eGodBidType );
	tagBidInfoNode* GetBidInfoNode(EGodBidType eGodBidType, DWORD dwIdx = GT_INVALID);

	VOID LogGodBid(DWORD dwRoleID, DWORD dwAccountID, DWORD dwItemID, BYTE byMoneyType, DWORD dwMoney);

private:
	DWORD m_bBidTime;																			// ��ǰ�Ƿ��ھ���ʱ��
	BidStartTimeList m_FixedBidStartTimeList;											// ÿ�̶ܹ����꿪ʼʱ��list
	BidStartTimeList m_WeekendAddedBidStartTimeList;							// ��ĩ�ӿ��ľ��꿪ʼʱ��list
	tagBidInfoNode m_NormalBidInfoList[MAX_GODBID_ITEM_NUM];	// ��ͨ���꾺����Ϣ�б�
	tagBidInfoNode m_GoldBlackMarketBidInfo;										// ��Һ��о��꾺����Ϣ�б�
	tagBidInfoNode m_ShengBiBlackMarketBidInfo;									// ʥ�Һ��о��꾺����Ϣ�б�
	BOOL m_bBidStartUpdated;																	// ���꿪ʼ���±��
	BOOL m_bBidEndUpdated;																	// ����������±��
	BOOL m_bSwitchOn;																				// ����
	BOOL m_bBidEnding;																				// ���ݿ��������Ͷ���¼
};

extern GodBidMgr g_GodBidMgr;

#pragma once
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\easybuy.h"
/** class MallMgr
	brief �̳ǹ�����
*/

struct tagStartNPCTalkEvent;
class NetSession;
class MallFrame;
class YuanbaoFrame;
class GroupBuyFrame;
class LevelPromoteFrame;
struct tagNS_SyncMallOfferProto;

//1Ԫ�ƻ�
struct tagOpenLevelPromoteFrame : public tagGameEvent
{
	INT nIndex;
	tagOpenLevelPromoteFrame(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		,nIndex(GT_INVALID)
	{} 
};

class MallMgr
{
public:
	typedef std::vector<const tagMallOfferProto2Client*> VecGiftPlan;

public:
	MallMgr(void);
	~MallMgr(void);
	static MallMgr* Inst();

	VOID Init();
	VOID Update();
	VOID Destroy();

	//--����������Ϣ
	DWORD OnNS_OpenMall(tagNS_OpenMall* pMsg, DWORD dwParam);
	DWORD OnNS_MallItem(tagNS_MallItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallFreeItem(tagNS_MallFreeItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallPack(tagNS_MallPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdateItem(tagNS_MallUpdateItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdatePack(tagNS_MallUpdatePack* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdate(tagNS_MallUpdate* pMsg, DWORD dwParam);
	DWORD OnNS_GetSimGPInfo(tagNS_GetSimGPInfo* pMsg, DWORD dwParam);
	//DWORD OnNS_OpenWareEasyBuyFrame(tagNS_WareOpen* pMsg, DWORD dwParam);
	DWORD OnNS_GetPromoteData(tagNS_MallRecommendingItem* pMsg, DWORD dwParam);

	//--������Ϸ�¼�
	DWORD OnOpen_Mall(tagGameEvent* pEvent);
	DWORD OnClose_Mall(tagGameEvent* pEvent);
	DWORD OnOpen_TaskEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_PackageEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_SocialEasyBuyFrame(tagGameEvent* pEvent);
	//DWORD OnOpen_WareEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_ComposeEasyBuyFrame(tagStartNPCTalkEvent* pEvent);
	DWORD OnOpen_PetEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_RoleSkillEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_RoleInfoEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnEventEnterWar(tagGameEvent* pEvent);
	DWORD OnEnterNormalMap(tagGameEvent* pEvent);

	//--
	TMap<DWORD, tagMallItemProto*>& GetMallProto()	{ return m_mapMallItem; }
	TMap<DWORD, tagMallPackProto*>& GetPackItem()	{ return m_mapPackItem; }
	list<tagMallPackProto*> &GetMallPack() { return m_listPack; }
	tagMallFreeItem* GetMallFreeItem()				{ return &m_MallFreeItem; }
	tagMallItemProto* GetMallProItem(int nIndex);
	map<INT64, tagGroupPurchase>& GetGroupBuyInfo() { return m_mapGgroupPur; }
	vector<tagUIItem> &GetEasyBuyItem() { return m_vecUIItem; }

	//--
	INT GetMallProtoSize() { return m_mapMallItem.Size(); }
	INT GetPackProtoSize() { return m_mapPackItem.Size(); } 
	INT GetGroupBuyInfoSize() { return m_mapGgroupPur.size(); }

	//--
	tagMallItemProto* GetMallItemByID(DWORD dwID);
	tagMallPackProto* GetMallPackByID(DWORD dwID);
	tagMallItemProto* GetMallItemByTypeid(DWORD dwTypeID);
	tagGroupPurchase* GetGroupBuyInfoByID(INT64 dwID);
	void DeleGroupBuyInfoByID(INT64 dwID);
	void RefreshGroupBuyInfoByID(INT64 dwID, INT nNum);


	//--�����ȡ��ʱ��
	DWORD GetFreeGetTime()const { return m_dwFreeGetTime; }
	void SetFreeGetTime(DWORD dwTime) { m_dwFreeGetTime = dwTime; } 

	//--
	void ClearMallItem();
	void ClearMallPack();
	void ClearGroupBuyInfo();

	//�̳�frame
	MallFrame* GetMallFrame() { return m_pMallFrame; }

	//�Ƽ���Ʒ
	void LoadPromoteData(); //��ȡ�Ƽ���Ʒ����XML
	map<INT, tagMallItemPrototeGood>& GetPromoteItem()	{ return m_mapMallPromoteItem; }
	VOID PromoteItemShowMgr();//��������ȼ�ɸѡ��Ҫ��ʾ���Ƽ���Ʒ
	map<DWORD, tagMallPromoteItemShow>&	GetProItemShow() { return m_mapMallPromoteItemShow; }    

	
	//--�Ź�
	void CreateGroupBuyFrame(LPCTSTR szFatherName);
	GroupBuyFrame* GetGroupBuyFrame() { return m_pGroupBuyFrame; }
	void SendGetGroupBuyInfo();
	INT GetGroupPurExp()const { return m_nGroupPurExp; }
	void SetGroupPurExp(INT32 exp)  { m_nGroupPurExp = exp; }

	//--���ʹ�Ʒ
	void CreateMallGiftFrame();
	void SetMallGiftFrameMaxBuyNum(INT nMaxBuyNum);
	void InitMallGiftFrameNum();
    //! Ԫ������
    void CreateYuanBaoFrame(LPCTSTR szFatherName);

	// װ�����ͻ�װ���Ի���ӿ�
	INT16 GetEquipDisplayPos(INT16 n16EquipPos);

	//--��ʾtips�õ���ʱ�����
	DWORD GetDay(DWORD& dwTimeDiff)
	{
		return dwTimeDiff / dwOneDay;
	}
	DWORD GetHour(DWORD& dwTimeDiff, DWORD& dwDay)
	{
		return (dwTimeDiff - (dwDay*dwOneDay)) / dwOneHour;
	}
	DWORD GetMin(DWORD& dwTimeDiff, DWORD& dwDay, DWORD& dwHour)
	{
		return (dwTimeDiff - (dwHour*dwOneHour) - (dwDay*dwOneDay)) / 60;
	}

	VOID OpenEasyBuyFrame( LPCTSTR szFather, LPCTSTR szFrame, LPCSTR szAlign, EasyBuyUIType enUIType, LPCSTR alignType, LPCSTR szControlName/* = NULL*/ );
	//����Ƽ���Ʒ���ҳ��
	INT GetMaxPromotePage(){ return m_nMaxPromotePage;}


	//------------------1Ԫ�ƻ�------------------
	std::vector<DWORD>& GetGiftPlanList(INT level);
	
	VOID  SetIndexForClint( INT index , BOOL b );
	//��ѯ�Ƿ��ѵ����������˸������0��ʼ����
	BOOL  InquireIndexForClint( INT index );

	VOID  SetIndexForBuy( INT index ,BOOL b);
	//��ѯ�Ƿ����򹺹��������򣩣���0��ʼ����
	BOOL  InquireIndexForBuy( INT index);

	BOOL  CreateLevelPromote( );
	//---------------------------------------------


private:

	void LoadEasyBuyItem();
	//--------1Ԫ�ƻ�----------
	BOOL LoadPlanFile();
	VOID ClearGiftPlanMap();
	DWORD OnNS_SyncMallOfferProto(tagNS_SyncMallOfferProto* pMsg, DWORD dwParam);

	DWORD InitMallOfferProto(tagNS_SyncMallOfferProto* pMsg, DWORD dwParam);
private:
	TSFPTrunk<MallMgr>					m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<VarContainer>				m_pVarContainer;
		
	MallFrame*							m_pMallFrame;
    YuanbaoFrame*                       m_pYuanbaoFrame;
	GroupBuyFrame*						m_pGroupBuyFrame;

	map<DWORD, tagMallItemProto>		m_mapMallItemEx;			//ȫ���Ǵ����Ʒ
	map<DWORD, tagMallPackProto>		m_mapPackItemEx;			//ȫ�������Ʒ

	TMap<DWORD, tagMallItemProto*>		m_mapMallItem;			
	TMap<DWORD, tagMallPackProto*>		m_mapPackItem;
	list<tagMallPackProto*>				m_listPack;
	list<tagMallItemProto*>				m_listItem;
	tagMallItemProto*					m_pProItem[MAX_PromoteItem];//������Ʒ

	map<INT, tagMallItemPrototeGood>	m_mapMallPromoteItem;			//��ȡ����Ӧ�Ƽ���Ʒ
	tagMallPromoteItemShow*				m_pPromoteItemShow;               
	map<DWORD, tagMallPromoteItemShow>	m_mapMallPromoteItemShow;       //�����Ƽ���Ʒ����ʾ

	tagMallFreeItem						m_MallFreeItem;				//�����ȡ����Ʒ
	DWORD								m_dwFreeGetTime;			//�����ȡ��ʱ��
	DWORD								m_dwMallTime;				//�̳�ʱ��	
	DWORD								m_dwGroupBuyTime;			//�Ź�ʱ��
	BOOL								m_bOpen;

	//--����Ϊ�Ź�
	map<INT64, tagGroupPurchase>		m_mapGgroupPur;				//�Ź�������(���̳���ƷTypdIDΪKey)
	INT									m_nGroupPurExp;				//�����Ź�ָ��
	vector<tagUIItem>					m_vecUIItem;				// �����������ص���Ʒ

	static const DWORD					dwOneDay  = 24 * 60 * 60;
	static const DWORD					dwOneHour = 60 * 60;

	INT									m_nMaxPromotePage;

	//1Ԫ�ƻ�
	VecGiftPlan							m_vecGiftPlan;
	BOOL								m_bLevelBtnGlint[LEVEL_NUM_CHOOSE];
	BOOL								m_bLevelCanBuy[LEVEL_NUM_CHOOSE];
	std::vector<DWORD>					m_vecGiftPlanByLevel;
	
	LevelPromoteFrame*					m_pLevelPromote;

	BOOL								m_bCanUseBag;
};
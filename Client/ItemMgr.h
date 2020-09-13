#pragma once
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\msg_guild.h"
#include "..\WorldDefine\fashion_define.h"
class ItemContainer;
class Item;
class Equipment;
class NetSession;
struct tagNS_BagExtend;
struct tagNS_WareExtend;
struct tagNS_SuitNum;
struct tagNS_Abrase;
struct tagItemContainerExtend;
struct tagNS_LockItem;
struct tagNS_EquipSignature;
struct tagNS_RaiseEquipPotval;
struct tagNS_DetachOrAttackSoul_Equip;
struct tagNS_GuiYuan;
struct tagNS_FairyPowder;
struct tagNS_SyncOpenChaseRemainTime;
struct tagNS_ForceLootChase;
struct tagNS_UnlockItemNotice;
struct tagNS_PlayerRegression;
struct tagNS_SyncFashionInfo;
struct tagNS_Rehanding;
struct tagNS_DressUpAnotherRole;
struct tagNS_FairyDoRaiseIntelligence;
struct tagNS_FairyRaiseIntelligenceMax;
struct tagNS_FabaoChange;
struct tagNS_NewSoulCrystalAdd;
struct tagNS_DiamondEquipGrowIM;
struct tagNS_EquipPerfectGrow;
struct tagNS_XiPurpleStar;
struct tagNS_ShengLingCoValueChg;

#define DefaultIcon _T("data\\ui\\Common\\L_diban-b.bmp")
/** \struct tagBeGoldStoneItemTable
	\breif ���ʯ��Ʒ��
*/
struct tagBeGoldStoneItemTable
{
	DWORD dwSrcItemID;
	DWORD dwDestItemID;
	DWORD dwGoldStoneType;
};

/** \struct tagBaiBaoRecordC
	\breif	�ٱ���������¼�ͻ��˴洢�ṹ
*/
struct tagBaiBaoRecordC
{
	EBaiBaoRecordType	eType;
	DWORD				dwTypeID;
	DWORD				dwRoleID;
	tagDWORDTime		sTime;
	tstring				strWords;
};

/**	\class ItemMgr
	\brief ��Ʒ������

*/
class ItemMgr
{
public:
	enum EPocketEncryptState// ��������״̬
	{
		EPES_Null			=	-1,		
		EPES_EnterPassword	=	0,		// ������������
		EPES_SetupPassword	=	1,		// ������������
		EPES_ModifyPassword	=	2,		// �޸���������
		EPES_CancelPassword	=	3		// ȡ����������
	};
public:
	ItemMgr();
	~ItemMgr();

	static ItemMgr* Inst();


	VOID Init();
	VOID Update();
	VOID Destroy();
		
	// ��ȡ��������ָ��
	ItemContainer* GetPocket(){return m_pPocket;}
	ItemContainer* GetThrone(){return m_pThrone;}
	ItemContainer* GetEquipment(){return m_pEquipment;}
	ItemContainer* GetStorge(){return m_pStorge;}
	ItemContainer* GetQuest(){return m_pQuest;}
    ItemContainer* GetGuildStore(){return m_pGuildStore;}
	ItemContainer* GetClothespress(){return m_pClothespress;}
	ItemContainer* GetTempCpntainer(){return m_pTempContainer;}
	ItemContainer* GetShengLingEquip(){return m_pShengLingEquip;}
	ItemContainer* GetWarBag() {return m_pWarBag;}
	
	// ����ģ��ID��ȡ��Ʒ��ͼ��
	tstring GetItemIconPath(DWORD dwTypeID);

	// ��ȡ�����е�ͬһ��TypeID��Ʒ�ĵ�һ��
	Item* GetPocketFirstItemByTypeID(DWORD dwTypeID);
	// ��ȡ�����е���Ʒ
	Item* GetPocketItem(INT16 n16Pos);
	// ��ñ����е���Ʒ
	Item* GetPocketItemByID(INT64 n64ID);
	// ��ȡװ�����е�װ��
	Equipment* GetCurEquip(EEquipPos ePos);
	// ��ȡװ�����е�װ��
	Equipment* GetCurEquipByID(INT64 n64ID);
	// ��ȡ��Ӧ�����е���Ʒ
	Item* GetConItem(EItemConType ePos, INT16 n16Pos);
	// ��ȡ��Ӧ�����е���Ʒ
	Item* GetConItemByID(EItemConType ePos, INT64 n64ID);
	// �ж�װ�����Ƿ�Ϊ��
	BOOL IsNotEquip(EEquipPos ePos);
	// ��ȡ��Ӧ������Ӧҳ��һ����λ
	INT16 GetConBlank(EItemConType ePos, INT nIndex = 0);

	// ��ȡĳ��װ��ǰ����
	INT8 GetSuitNum(DWORD dwSuitID){return m_mapSuitNum.Peek(dwSuitID);}
	// ��ȡĳ��װ�����������Ƿ񼤻�
	bool GetLonghun(EEquipPos ePos, INT64 id);

	// ��ȡ��ȴʱ��
	VOID GetItemCurCDTime(DWORD dwTypeID, FLOAT& fCDCount, FLOAT& fRestoreRatio);

	// ��ȡ��Ӧ���⹦�����͵���ƷID
	INT64 GetSpecfuncItemID(EItemSpecFunc eType);
	// ��ȡ��ǰ������������
	EItemTypeEx GetHandWeaponType(EEquipPos ePos=EEP_RightHand);
	
	// ��ʾ��Ʒ��ش�����
	VOID ShowItemErrorMsg(DWORD dwErrorCode);

	//��ʾ������Ʒ��ش����룬Ŀǰֻ������17��21�Ĵ����룬
	//����Ҫ��ʹ����ǵ��޸����Ʒ�Χ��17��ǰ�벻Ҫʹ�ã������ͻ��
	VOID ShowSpecItemErrorMsg(DWORD dwErrorCode);

	// ��ȡ�ٱ�����¼�б�
	VOID GetBaiBaoList(TList<tagBaiBaoRecordC> &listRecord);
	
	// ���õ�ǰ���Ҽ��ܵ�״̬������ͻ��˴�����������͵���ϢNS_BagPsd
	void SetPocketEncryptState(EPocketEncryptState eState){m_eEncryptState = eState;}

    // ��ȡ���ɲֿ�
    void OnGetGuildWare(tagNS_GetGuildWare* pMsg);
    // ��հ��ɲֿ�
    void ClearGuildWare();

	//brief �ӱ����еõ���һ�����⹦�ܵ���Ʒ
	Item* GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc );

	// �����Ƿ��ڵ��ʯ��Ʒ����
	bool IsInGoldStoneTable( DWORD dwItemId, DWORD dwGoldStoneType );

	/** \brief �ӱ����еõ���һ�����⹦�ܵ���Ʒ
		\param DWORD dwItemTypeID ���ܲ�ͬ����Ʒ�� ͬ�������⹦�ܣ�������ͨ����͸߼����
		\param bEnableType ���������ڶ���������Ĭ���ǲ�������
	*/
	Item* GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc, DWORD dwItemTypeID, BOOL bEnableType = FALSE );

	//������ֿ����ʽ�򿪲ֿ�
	void OpenWalkWare();

	// �õ������ĵȼ�
	INT GetTrumpLevel( INT nDengJie );
	// �õ������ĵ�ǰ����
	INT GetTrumpCurExp( INT nDengJie );
	// ������������һ����Ҫ�ľ���ֵ
	INT GetTrumpNextLevelExp( INT nDengJie );
	INT GetTrumpRefineExp( INT nPlayerLevel );
	INT GetTrumpMaxExp();
	VOID GetEquipTrumpLevel(INT nExp, INT &nPlayerLevel, INT &DengJie );

	/** \ ǿ��ʹ��Ʒ��CDʱ��
	*/
	VOID FouceUpdateItemCDTime(DWORD dwItemTypeID);
	VOID FouceUpdateItemCDTime(DWORD dwItemTypeID,DWORD dwRemianSeconds);

	TMap<DWORD, tagFashionInfo*>& GetFashionMap() { return m_mapFashion; }

	// ��ȡ�³��е�ʱװ
	Equipment* GetFashion( DWORD dwTypeID, BOOL bDIY = FALSE);
	// ����³��е�ʱװ
	Equipment* GetFashion(INT64 n64ID);

	VOID GetAllSuitableFashion( bool bMan, vector<DWORD> &vecFashion );

	// ��ȡһ��ʱװ������
	BYTE GetSuitMettle(DWORD dwSuitIndex);

protected:
	//--������Ϣ������
	// ��ʼ����Ʒ
	DWORD NetInitLocalPlayerItem(tagNS_GetRoleInitState_Item* pNetCmd, DWORD);
	// ��ʼ����Ʒ��ȴ
	DWORD NetInitLocalPlayerItemCD(tagNS_GetRoleInitState_ItemCDTime* pNetCmd, DWORD);
	// ��ʼ����װ
	DWORD NetInitLocalPlayerSuit(tagNS_GetRoleInitState_Suit* pNetCmd, DWORD);
	// ��ʼ����������
	DWORD NetInitLocalPlayerLonghun(tagNS_GetRoleInitState_Longhun* pNetCmd, DWORD);
	// ��װ������
	DWORD NetEquipment(tagNS_Equip* pNetCmd, DWORD);
	// ��װ������
	DWORD NetUnEquipment(tagNS_Unequip* pNetCmd, DWORD);
	// ����������
	DWORD NetSwapWeapon(tagNS_SwapWeapon* pNetCmd, DWORD);
	// ��Ʒλ�øı䣬ͬһ������
	BOOL NetItemPosChange(tagNS_ItemPosChange* pNetCmd, DWORD);
	// ��Ʒλ�øı䣬��ͬ����֮��
	BOOL NetItemPosChangeEx(tagNS_ItemPosChangeEx* pNetCmd, DWORD);
	// ����һ����Ʒ
	DWORD NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD);
	// ����һ��װ��
	DWORD NetNewEquipAdd(tagNS_NewEquipAdd* pNetCmd, DWORD);
	// ����һ������
	DWORD NetNewTrumpAdd(tagNS_NewFabaoAdd* pNetCmd, DWORD);
	// ����һ��ʥ��
	DWORD NetNewHolyAdd(tagNS_NewHolyAdd* pNetCmd, DWORD);
	// ����һ������װ��
	DWORD NetNewHolyEquipAdd(tagNS_NewHolyEquipAdd* pNetCmd, DWORD);
	// ����һ����֪��Ʒ
	DWORD NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD);
	// ɾ��һ����Ʒ
	DWORD NetItemRemove(tagNS_ItemRemove* pNetCmd, DWORD);
	// ����Ʒ
	DWORD NetItemBind(tagNS_ItemBind* pNetCmd, DWORD);
	// ������Ʒ�İ�״̬
	DWORD NetItemBindStatus(tagNS_ItemBindStatus* pNetCmd, DWORD);

	// װ�����Ըı�
	DWORD NetEquipChange(tagNS_EquipChange* pNetCmd, DWORD);
	// װ���������Ըı�
	DWORD NetEquipSingleChange(tagNS_EquipSingleChange* pMsg, DWORD pPrama);
	// �������Ըı�
	DWORD NetTrumpChange(tagNS_FabaoChange* pNetCmd, DWORD);
	// �꾧���Ըı�
	DWORD NetHunJingChange(tagNS_SoulCrystalChg* pNetCmd, DWORD);
	// ʥ�����Ըı�
	DWORD NetShengLingChange(tagNS_ShengLingChg* pNetCmd, DWORD);
	// ʥ��װ�����Ըı�
	DWORD NetShengLingEquipChange(tagNS_HolyEquipChg* pNetCmd, DWORD);
	// ��ƷCD�ı�
	DWORD NetItemCDUpdate(tagNS_ItemCDUpdate* pNetCmd, DWORD);
	// ��������
	DWORD NetBagExtend(tagNS_BagExtend* pNetCmd, DWORD);
	// �ֿ�����
	DWORD NetWareExtend(tagNS_WareExtend* pNetCmd, DWORD);
	// ��װ����
	DWORD NetSuitNumChange(tagNS_SuitNum* pNetCmd, DWORD);
	// ����װ��
	DWORD NetIdentifyEquip(tagNS_IdentifyEquip* pNetCmd, DWORD);
	// ĥʯ��ĥ
	DWORD NetAbraseWeapon(tagNS_Abrase* pNetCmd, DWORD);
    // ʱװȾɫ
    DWORD NetDyeFashion(tagNS_DyeFashion* pNetCmd, DWORD);
	// ���Ҽ��ܷ���
	DWORD NetBagPassword(tagNS_BagPsd* pNetCmd, DWORD);
	// ����ո�¶ȱ仯
	DWORD NetWeaponNewnessChange(tagNS_RWNewessChange* pNetCmd, DWORD);
	// �����Զ�����
	DWORD NetContainerReorder(tagNS_ItemReorder* pNetCmd, DWORD);
	DWORD NetContainerReorderEx(tagNS_ItemReorderEx* pNetCmd, DWORD);
	// ����װ������������������
	DWORD NetEquipLonghunOn(tagNS_LongHunOn* pNetCmd, DWORD);
	DWORD NetEquipLonghunOff(tagNS_LongHunOff* pNetCmd, DWORD);
	// ����ٱ�����¼����
	DWORD NetInitBaiBaoRecord(tagNS_InitBaiBaoRecord* pNetCmd, DWORD);
	DWORD NetSingleBaiBaoRecord(tagNS_SingleBaiBaoRecord* pNetCmd, DWORD);

	//��Ӧ�򿪲ֿ�
	DWORD NetWareOpen( tagNS_WareOpen* pNetCmd, DWORD );

	DWORD NetBeGoldStone( tagNS_BeGoldStone *pNetCmd, DWORD );

	DWORD NetUpgradeMaxPot( tagNS_RaiseEquipPotval *pNetCmd, DWORD );
	// ���긽��
	DWORD NetSoulChange( tagNS_DetachOrAttackSoul_Equip *pNetCmd, DWORD );
	// ��Ԫ
	DWORD NetGuiYuan( tagNS_GuiYuan *pNetCmd, DWORD );
	// ����֮��
	DWORD NetFairyPowder( tagNS_FairyPowder *pNetCmd, DWORD );

	DWORD OnNS_LockItem( tagNS_LockItem *pNetCmd, DWORD );

	DWORD OnNS_UnLockItem( tagNS_UnlockItemNotice *pNetCmd, DWORD );

	DWORD OnNS_EquipSignature( tagNS_EquipSignature *pNetCmd, DWORD );

	//--������Ϸ�¼�
	// ��Ӧ��������
	DWORD OnEventBagExtend(tagItemContainerExtend* pGameEvent);
	// ��Ӧ�ֿ�����
	DWORD OnEventWareExtend(tagItemContainerExtend* pGameEvent);
	// ȷ����������
	DWORD OnEventBagExtendCheck(tagMsgBoxEvent* pGameEvent);
	// ȷ�ϲֿ�����
	DWORD OnEventWareExtendCheck(tagMsgBoxEvent* pGameEvent);
	
	DWORD OnNS_SyncOpenChaseRemainTime(tagNS_SyncOpenChaseRemainTime* pNetCmd,DWORD);

	DWORD OnNS_ForceLootChase(tagNS_ForceLootChase* pNetCmd,DWORD);

	DWORD OnNS_FairyDoRaiseIntelligence(tagNS_FairyDoRaiseIntelligence* pNetCmd,DWORD);
	DWORD OnNS_FairyRaiseIntelligenceMax(tagNS_FairyRaiseIntelligenceMax* pNetCmd,DWORD);
	
	
	// ��ȡ��Ӧ�������͵�ָ��
	ItemContainer* GetContainer(EItemConType eType);
	
	// ����һ����Ʒ;
	VOID AddNewItem(Item* pItem);

	// ����һ���ٱ�����¼
	VOID AddNewBaiBaoRecord(tagBaiBaoRecord* pRecord);

	// ����һع�
	DWORD OnNS_PlayerRegression( tagNS_PlayerRegression* pNetCmd, DWORD dwParam );

	// ��ʼ�͸������ʱװ��Ϣ
	DWORD OnNS_UpdateFashionInfo( tagNS_SyncFashionInfo* pNetCmd, DWORD dwParam );

	// װ�����
	DWORD OnNS_NetDressUpAnotherRole( tagNS_DressUpAnotherRole* pNetCmd, DWORD dwParam );

	// �꾧
	DWORD OnNS_NewSoulCrystalAdd( tagNS_NewSoulCrystalAdd* pNetCmd, DWORD dwParam );

	// ʹ�ñ�ʯ��������ϴ���Ե��߷��ص���Ϣ
	DWORD OnNS_DiamondEquipGrowIM(tagNS_DiamondEquipGrowIM* pNetCmd, DWORD dwParam);

	// �����ɳ�
	DWORD OnNS_EquipPerfectGrow(tagNS_EquipPerfectGrow* pNetCmd, DWORD dwParam);
	// ���Ǵ�
	DWORD OnNS_XiPurpleStar(tagNS_XiPurpleStar* pNetCmd, DWORD dwParam);
	// ����ʥ��Ĭ��ֵ
	DWORD OnNS_ShengLingCoValueChg(tagNS_ShengLingCoValueChg* pNetCmd, DWORD);

private:
	void LoadBeGoldStoneItemTable();
	void LoadTrumpLevelTable();

private:
	TSFPTrunk<ItemMgr>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<VarContainer>	m_pVarContainer;

	ItemContainer*			m_pPocket;		// ����
	ItemContainer*			m_pStorge;		// �ֿ�
	ItemContainer*			m_pEquipment;	// װ����
	ItemContainer*			m_pThrone;		// �ٱ���
	ItemContainer*			m_pQuest;		// ������Ʒ��
    ItemContainer*          m_pGuildStore;  // ���ɲֿ�
	ItemContainer*          m_pClothespress;  // �³�
	ItemContainer*			m_pTempContainer;	//��ʱ���������������������
	ItemContainer*			m_pShengLingEquip;	//ʥ��װ������
	ItemContainer*			m_pWarBag;			//ս������

	TMap<DWORD, DWORD>		m_mapCurStartTime;		// ��ȴʱ�俪ʼ��
	TMap<DWORD, INT8>		m_mapSuitNum;			// ��װ����
	TMap<DWORD, tagFashionInfo*>	m_mapFashion;		// ����ʱװ����Ϣ
	TMap<INT16, INT8>		m_mapLonghunOn;			// ���꼤�����
	bool					m_bUseSilverExtend;		// ʹ�ý�Ǯ�����־λ
	bool					m_bNotEnoughYuanbao;	// Ԫ�������־λ
	bool					m_bNotEnoughSilver;		// ��Ǯ�����־λ
	bool					m_bNotInitBaiBaoRecord;	// �ٱ������±�־λ
	TList<tagBaiBaoRecordC>	m_listBaiBaoRecord;		// �ٱ�����¼�б�
	vector<tagBeGoldStoneItemTable> m_vecBeGoldStoneItemTable;	// ���ʯ��Ʒ��
	vector<tagFabaoLevelProto> m_vecTrumpLevel;

	EPocketEncryptState		m_eEncryptState;		// �������뵱ǰ����״̬
};

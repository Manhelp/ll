//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_mgr.h
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ/װ�������� -- ������Ʒ����,��ͻ���ͬ��,����¼��Ʒlog
//-----------------------------------------------------------------------------
#pragma once

#include "container.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_item.h"
#include "../ServerDefine/base_define.h"
#include "world.h"
#include "HolySoul_Container.h"

struct	tagRoleData;
struct	tagCDInfo;
//-----------------------------------------------------------------------------

class ItemMgr
{
	typedef TMap<DWORD, tagCDInfo*> MapCDTime;
	typedef TMap<DWORD, INT>		MapMaxHold;
	typedef TMap<INT64, DWORD>		MapUnlockCD;

public:
	ItemMgr(Role* pRole, DWORD dwAcctID, DWORD dwRoleID, INT16 n16BagSize, INT16 n16WareSize);
	~ItemMgr();

public:
	VOID SaveItem2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum, tagFabao *pFabao);
	VOID Update();

	BOOL IsItemCDTime(DWORD dwTypeID);
	VOID CheckCDTimeMap(DWORD dwTypeID);
	VOID Add2CDTimeMap(DWORD dwTypeID, DWORD dwCDTime = GT_INVALID);
	VOID Add2UnlockCDTimeMap(INT64 n64Serial, DWORD);
	VOID SaveCDTime2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID GetSameCDItemList(OUT TMap<DWORD, tagCDInfo*> &mapSameCD, IN DWORD dwTypeID);

	VOID ProcEquipNewness();

	static BOOL CalSpaceUsed(DWORD dwTypeID, INT32 nNum, 
		OUT INT16 &n16UseBagSpace, OUT INT16 &n16UseQuestSpace, OUT INT16 &n16UseWarBagSpace, OUT INT16 &n16MaxLap);

	//-----------------------------------------------------------------------------
	// �������Ƿ����ظ�ID
	//-----------------------------------------------------------------------------
	BOOL IsRepeatID(INT64 n64ItemID[], INT32 nArraySz);

public:
	BOOL CanExchange(const tagItem& item) const;
	BOOL CanSell(const tagItem& item) const;

public:
	DWORD IdentifyEquip(INT64 n64SerialReel, INT64 n64SerialEquip, DWORD dwCmdID);

public:
	//-----------------------------------------------------------------------------
	// ��ɫ��ʼ����Ʒ&װ��
	//-----------------------------------------------------------------------------
	VOID	SendInitStateItem();
	VOID	SendInitStateItemCDTime();

	DWORD	Put2Container(tagItem *pItem);

	VOID	UpdateEquipSpec(const tagEquip &equip,bool bSingleAttChange = false/*�Ƿ񵥸����Ա仯*/,EEquipSignleSpecType eEquipSingleSpecType = EESSType_Null);
	// Jason 2010-4-19 v2.0.0
	VOID	UpdateFabaoSpec(const tagFabao & fabao,BOOL bSave2DB = TRUE);
	VOID	UpdateSoulCrystalSpec(const tagSoulCrystal & obj, BOOL bSave2DB = TRUE);

	VOID	UpdateHolySpec(const tagHolyMan & obj,BOOL bSave2DB = TRUE);
	VOID	UpdateHolyEquipSpec(const tagHolyEquip & obj, BOOL bSave2DB = TRUE);

	// 2010-10-27 1.2.2
	VOID	UpdateFabaoNirVanaData2DB(const tagFabao & fabao);
	VOID	GetFabaoNirVanaDataFromDB(const tagFabao & fabao,BOOL bFirstOrLast);

public:
	//-----------------------------------------------------------------------------
	// �������л�������Ϣ
	//-----------------------------------------------------------------------------
	INT16	GetBagFreeSize();
	INT16	GetBagCurSize();
	INT16	GetBagOneFreeSpace();
	INT32	GetBagSameItemCount(DWORD dwTypeID);
	INT32	GetBagSameItemList(OUT TList<tagItem*> &list, IN DWORD dwTypeID, IN INT32 nNum = INT_MAX);

	INT32	GetBagSameItemCount2(EItemType	eType);

	INT16	GetQuestItemBagFreeSize();
	INT32	GetQuestBagSameItemCount(DWORD dwTypeID);

	INT16	GetBaiBaoFreeSize();
	INT16	GetWareCurSize();

	BOOL	IsBagOneSpaceFree(INT16 n16Index);
	BOOL	GetBagRandom(INT64 &n64Serial);
	BOOL	GetQuestItemBagRandom(INT64 &n64Serial);

	INT16	GetWarBagFreeSize();

	tagItem*	GetBagItem(INT64 n64Serial);
	tagItem*	GetBagItem(INT16 n16Index);
	tagItem*	GetWarBagItem(INT64 n64Serial);
	tagItem*	GetWarBagItem(INT16 n16Index);
	tagEquip*	GetEquipBarEquip(INT64 n64Serial);
	tagEquip*	GetEquipBarEquip(INT16 n16Index);
	tagEquip*	GetEquipBarEquip(DWORD dwTypeID);
	tagItem*	GetDisplayItem(EItemConType eConType, INT64 n64Serial);
	tagEquip*   GetHolySoulBarEquip(INT64 n64Serial);
	tagEquip*	GetHolySoulBarEquip(INT16 n16Index);
	tagEquip*	GetHolySoulBarEquip(DWORD dwTypeID);

	tagItem*	GetHolyManBarItem(INT64 n64Serial);

	
	// ����TypeID�õ������ҵ�����,����ΪNULL
	tagItem*	GetItemByTypeID(DWORD dwTypeID);

	tagItem * GetItemFromAllContainer(INT64 n64Serial);
	tagItem * GetItemFromContainer(INT64 n64Serial,EItemConType eCon);
public:	
	//-----------------------------------------------------------------------------
	// ��һ�װ
	//-----------------------------------------------------------------------------
	DWORD Equip(INT64 n64SerialSrc, EEquipPos ePosDst);
	DWORD EquipByFamily(tagItem *pItem, EEquipPos ePosDst);
	DWORD Unequip(INT64 n64SerialSrc, INT16 n16IndexDst);
	DWORD UnequipOnly(INT64 n64SerialSrc);
	DWORD SwapWeapon();
	DWORD MoveRing(INT64 n64SerialSrc, INT16 n16PosDst);

public:
	//-----------------------------------------------------------------------------
	// ��һ��&ʧȥ��Ʒ&װ�� -- ��ͨ��Ʒ���뱳����������Ʒ����������
	//-----------------------------------------------------------------------------
	BOOL Add2Role(EItemCreateMode eCreateMode, DWORD dwCreateID, 
				DWORD dwTypeID, INT32 nNum, EItemQuality eQlty, DWORD dwCmdID);

	DWORD RemoveFromRole(DWORD dwTypeID, INT32 nNum, DWORD dwCmdID); // nNum == -1 ʱ��ʾȫ��ɾ��

	// �Ӳֿ���ɾ����Ʒ
	DWORD RemoveFromWare(DWORD dwTypeID, INT32 nNum, DWORD dwCmdID); // nNum == -1 ʱ��ʾȫ��ɾ��

	//-----------------------------------------------------------------------------
	// ɾ�����������Ʒ -- ��鱳����������Ʒ��
	//-----------------------------------------------------------------------------
	VOID RemoveFromRole(UINT16 u16QuestID, DWORD dwCmdID);

	//-----------------------------------------------------------------------------
	// ��һ����Ʒ&װ��
	//-----------------------------------------------------------------------------
	DWORD Add2Bag(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB = FALSE, BOOL bCheckAdd = TRUE, BOOL bFromFamily = FALSE );
	DWORD Add2WarBag(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB = FALSE, BOOL bCheckAdd = TRUE, BOOL bFromFamily = FALSE );
	DWORD Add2BagByIndex(tagItem *&pItem, DWORD dwCmdID, INT16 n16Index);
	DWORD Add2BagByIndexAndInsertDB(tagItem *&pItem, DWORD dwCmdID, INT16 n16Index);
	DWORD Add2QuestBag(tagItem *&pItem, DWORD dwCmdID);
	DWORD Add2RoleWare(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB = FALSE, BOOL bCheckAdd = TRUE, BOOL bFromFamily = FALSE);
	DWORD Add2TempBag(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB = FALSE, BOOL bCheckAdd = TRUE, BOOL bFromFamily = FALSE );

	DWORD AddHolyManBar(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB = FALSE, BOOL bCheckAdd = TRUE, BOOL bFromFamily = FALSE );
	
	//-----------------------------------------------------------------------------
	// �ٱ������
	//-----------------------------------------------------------------------------
	VOID Add2BaiBao(tagItem *&pItem, DWORD dwCmdID, BOOL bReadFromDB = FALSE, DWORD dwRoleIDRel = GT_INVALID, BOOL bFromFamily = FALSE);
	DWORD ClearBaibao();
	static VOID InsertBaiBao2DB(tagItem *pItem, DWORD dwRoleID, DWORD dwCmdID);
	static VOID InsertBaiBao2DBEx(tagItem *pItem, DWORD dwAccountId, DWORD dwCmdID);

	static DWORD ProcBaiBaoRecord(DWORD dwTypeID, DWORD dwDstRoleID, DWORD dwSrcRoleID, 
		INT16 n16Type = EBBRT_System, DWORD dwTime = GT_INVALID, LPCTSTR szLeaveWords = _T(""));
public:
	//-----------------------------------------------------------------------------
	// ɾ����Ʒ��װϸ��
	//-----------------------------------------------------------------------------
	// �ӱ�����ȡ������������������ -- �ڴ�û���ͷ�
	DWORD TakeOutFromBag(INT64 n64Serial, DWORD dwCmdID, BOOL bDelFromDB);

	DWORD TakeOutFromHolyBar(INT64 n64Serial, DWORD dwCmdID, BOOL bDelFromDB);
	
	
	// ������Ʒ���� -- n16NumȡĬ��ֵʱ��ʾȫ��ɾ��������db����������ͷ��ڴ�
	DWORD DelFromBag(INT64 n64Serial, DWORD dwCmdID, INT16 n16Num = GT_INVALID, BOOL bCheckDel = FALSE);
	DWORD DelFromTempBag(INT64 n64Serial, DWORD dwCmdID, INT16 n16Num = GT_INVALID, BOOL bCheckDel = FALSE);

	// ��ָ���б���ɾ��ָ����������Ʒ
	DWORD DelBagSameItem(TList<tagItem*> &list, INT32 nNum, DWORD dwCmdID);

	// ������ʹ�õ����ĸ�������� -- �ɶѵ���Ʒ���ĸ��������ɶѵ���Ʒ��Ϣ����
	DWORD ItemUsedFromBag(INT64 n64Serial, INT16 n16Num, DWORD dwCmdID,BOOL bRemoveLimited=TRUE);

	// �ӱ�����ȡ���������� -- ��db�����������Ʒ�ڴ��ͷţ�������Ʒ���ɶ���
	DWORD DiscardFromBag(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut);

	// ������ʹ�õ����ĸ�������� -- �ɶѵ���Ʒ���ĸ��������ɶѵ���Ʒ��Ϣ����
	DWORD ItemUsedFromWarBag(INT64 n64Serial, INT16 n16Num, DWORD dwCmdID);

	// ������Ʒ���� -- n16NumȡĬ��ֵʱ��ʾȫ��ɾ��������db����������ͷ��ڴ�
	DWORD DelFromWarBag(INT64 n64Serial, DWORD dwCmdID, INT16 n16Num = GT_INVALID, BOOL bCheckDel = FALSE);

	// ��ս��������ȡ���������� -- ��db�����������Ʒ�ڴ��ͷţ�������Ʒ���ɶ���
	DWORD DiscardFromWarBag(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut);
	DWORD DiscardFromWarBag(INT64 n64Serial, DWORD dwCmdID);

	// �ӱ����е��� -- ��db��������󶨻�������Ʒ�ڴ��ͷ�
	DWORD LootFromBag(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut);

	// �ӱ����е��� -- ����ɵ�����Ʒ������������Ʒ��Ϣ
	DWORD LootFromBag(INT nLootNum, TList<tagItem*>& listItems, DWORD dwCmdID, TList<DWORD>& listBroadcast);

	// ��װ�����е��� -- ��db��������󶨻�������Ʒ�ڴ��ͷ�
	DWORD LootFromEquipBar(INT64 n64Serial, DWORD dwCmdID, OUT tagItem *&pOut);

	// ��װ�����е��� -- ����ɵ�����Ʒ������������Ʒ��Ϣ
	DWORD LootFromEquipBar(INT nLootNum, TList<tagEquip*>& listItems, DWORD dwCmdID, TList<DWORD>& listBroadcast);

public:
	//-----------------------------------------------------------------------------
	// ������ɾ������Ӷ����Ʒ(��Ҽ佻��,�ʼ�,����)
	//-----------------------------------------------------------------------------
	VOID Add2Bag(tagItem* pItem[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel);
	VOID RemoveFromBag(INT64 n64Serial[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel);
	BOOL CheckExistInBag(OUT tagItem* pItem[], INT64 n64Serial[], INT16 n16Num[], INT32 nSize);

	VOID RemoveFromHolyManBar(INT64 n64Serial[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel);
	VOID AddHolyManBar(tagItem* pItem[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel);
	

	//-----------------------------------------------------------------------------
	// �ƶ���Ʒ
	//-----------------------------------------------------------------------------
	DWORD Move(EItemConType eConType, INT64 n64Serial, INT16 n16Num, INT16 n16PosDst, DWORD dwCmdID);
	DWORD Move2Other(EItemConType eConTypeSrc, INT64 n64Serial1, 
					EItemConType eConTypeDst, INT16 n16PosDst, DWORD dwCmdID);

	//-----------------------------------------------------------------------------
	// ��װ������ֱ��ɾ��һ��װ��
	//-----------------------------------------------------------------------------
	tagItem* RemoveFromEquipBar(INT64 n64Serial, DWORD dwCmdID, BOOL bDelMem = FALSE);

	//-----------------------------------------------------------------------------
	// ��Ԫ��װ������ֱ��ɾ��һ��װ��
	//-----------------------------------------------------------------------------
	tagItem* RemoveFromHolySoulBar(INT64 n64Serial, DWORD dwCmdID, BOOL bDelMem = FALSE);


public:
	//-----------------------------------------------------------------------------
	// ����&��ɫ�ֿ�����
	//-----------------------------------------------------------------------------
	DWORD ExtendBag(INT64 n64serial);
	DWORD ExtendRoleWare(INT64 n64Serial);

	//-----------------------------------------------------------------------------
	// ����&��ɫ�ֿ�����
	//-----------------------------------------------------------------------------
	DWORD ReorderBag(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num);
	DWORD ReorderRoleWare(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num);

	DWORD ReorderBagEx(IN LPVOID pData, OUT LPVOID pOutData, OUT INT16 &n16OutNum, const INT16 n16Num);
	DWORD ReorderRoleWareEx(IN LPVOID pData, OUT LPVOID pOutData, OUT INT16 &n16OutNum, const INT16 n16Num);

public:
	//-----------------------------------------------------------------------------
	// ��ӵ������������Ʒ����
	//-----------------------------------------------------------------------------
	// Jason 2010-3-19 v1.5.0
	INT	 GetOneTypeItemFreeSpace( DWORD dwTypeID );
	BOOL IsMaxHoldLimitItem( DWORD dwTypeID );
	BOOL CanAddMaxHoldItem( DWORD dwTypeID, INT nNum );
	BOOL CanAddMaxHoldItem( const tagItem& item );
	DWORD AddMaxHoldItem( DWORD dwTypeID, INT nNum );
	DWORD AddMaxHoldItem( const tagItem& item );
	VOID RemoveMaxHoldItem( DWORD dwTypeID, INT nNum );

private:
	template<class T> VOID Save2DB(IN Container<T> &con, OUT LPVOID pData, 
								OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	template<class T> VOID GetAllItem(IN Container<T> &con, const INT16 n16ReadNum, 
								OUT LPVOID pData, OUT INT32 &nSize);

	VOID SaveFamilySpriteToDB( tagFabao *pFabao, OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	VOID FormatCDTime(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID UpdateCDTime();
	VOID UpdateUnlockCDTime();

	VOID UpdateContainer(ItemContainer& sItemCon);
	VOID UpdateContainer(EquipContainer& sEquipCon);

public:
	ItemContainer*	GetContainer(EItemConType eConType);

	ItemContainer&	GetBag()			{ return m_Bag; }
	ItemContainer&	GetQuestItemBag()	{ return m_QuestItemBag; }
	ItemContainer&	GetBaiBaoBag()		{ return m_BaiBaoBag; }
	ItemContainer&	GetRoleWare()		{ return m_RoleWare; }
	ItemContainer&	GetTempBag()		{ return m_TempBag; }
	ItemContainer&	GetWarBag()			{ return m_WarBag; }
	
	EquipContainer& GetEquipBar()		{ return m_EquipBar; }
	WardrobeContainer& GetWardrobeContainer()		{ return m_Wardrobe; }
	HolySoulContainer& GetHolySoulBar() 	{ return m_HolySoulBar; }

	ItemContainer&	GetHolyManBar()		{ return m_HolyManBar; }	

	//Jason 2009-11-25
	//�޸�װ��Ǳ��ֵ����Ӱ��Ǳ������ֵ
	BOOL AddEquipPotVal(INT64 n64Serial,INT val);
	// Jason 2010-7-20 v2.3.1
	DWORD DelFromContainer(ItemContainer & con, INT64 n64Serial, DWORD dwCmdID, INT16 n16Num = GT_INVALID, BOOL bCheckDel = FALSE)
	{
		if(GT_INVALID == n16Num)
		{
			return RemoveItem(con, n64Serial, dwCmdID, TRUE, TRUE, bCheckDel);
		}

		return RemoveItem(con, n64Serial, n16Num, dwCmdID, bCheckDel, TRUE);
	}

	// ��Ҷ�����ɾ����ս�������߻��ߡ�ս��������ʱ�޵���
	void OnZhanDouFuRemoved(EItemSpecFunc eItemSpecFunc, INT64 n64ItemID, BOOL bTimeOut=FALSE);

private:
	//-----------------------------------------------------------------------------
	// ��Ʒ -- bDelFromDB		�Ƿ����Ϸ���ݿ���ɾ��,
	//		   bCheckRemove		�Ƿ��жϸ���Ʒ�ɴ�������ɾ��
	//		   bDelMem			�Ƿ������ڴ�
	//		   bIncUseTimes		�Ƿ����ĵ���ʹ�ô���
	//-----------------------------------------------------------------------------
	DWORD AddItem(ItemContainer& container, tagItem *&pItem, DWORD dwCmdID, 
					BOOL bInsert2DB = FALSE, BOOL bCheckAdd = TRUE, DWORD dwRoleIDRel = GT_INVALID, BOOL bChangeOwner = TRUE, BOOL bFromFamily=FALSE);

	DWORD RemoveItem(ItemContainer& container, INT64 n64Serial, DWORD dwCmdID, 
					BOOL bDelFromDB = FALSE, BOOL bDelMem = FALSE, 
					BOOL bCheckRemove = TRUE, DWORD dwRoleIDRel = GT_INVALID, BOOL bTimeOut = FALSE,BOOL bRemoveLimited=TRUE);

	DWORD RemoveItem(ItemContainer& container, INT64 n64Serial, INT16 n16Num, DWORD dwCmdID, 
					BOOL bCheckRemove = TRUE, BOOL bDelete = FALSE,BOOL bRemoveLimited =TRUE);

	//-----------------------------------------------------------------------------
	// ����Ϸ��ɾ��ָ�������Ʒ,�Ҳ�����Ʒ�Ƿ�ɴ�������ɾ���ļ��
	//-----------------------------------------------------------------------------
	DWORD RemoveItems(ItemContainer& container, DWORD dwTypeID, DWORD dwCmdID);
	DWORD RemoveItems(ItemContainer& container, DWORD dwTypeID, INT32 nNum, DWORD dwCmdID);

	//-----------------------------------------------------------------------------
	// ����Ϸ��ɾ����ָ��������ص���Ʒ
	//-----------------------------------------------------------------------------
	VOID RemoveItems(ItemContainer& container, UINT16 u16QuestID, DWORD dwCmdID);
	VOID RemoveItems(EquipContainer& container, UINT16 u16QuestID, DWORD dwCmdID);
	
	//-----------------------------------------------------------------------------
	// ������ɾ������Ӷ����Ʒ(��Ҽ佻��,�ʼ�,����)
	//-----------------------------------------------------------------------------
	VOID AddItems(ItemContainer& container, tagItem* pItem[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel);
	VOID RemoveItems(ItemContainer& container, INT64 n64Serial[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel);

	BOOL CheckItemsExist(OUT tagItem* pItem[], ItemContainer& container, 
						INT64 n64Serial[], INT16 n16Num[], INT32 nSize);

	BOOL IsQuestItem(DWORD dwTypeID);

private:
	//-----------------------------------------------------------------------------
	// ��ͻ��˷�����Ϣ
	//-----------------------------------------------------------------------------
	VOID SendMessage(LPVOID pMsg, DWORD dwSize);
	
	//-----------------------------------------------------------------------------
	// �����������&ɾ����Ʒ�����͵��ͻ�����Ϣ��װ
	//-----------------------------------------------------------------------------
	VOID SendAddItem2Client(EItemConType eConType, INT16 n16Index, INT64 n64Serial, INT16 n16Num, BOOL bOverlap);
	VOID SendDelItem2Client(EItemConType eConType, INT16 n16Index, INT64 n64Serial, INT16 n16Num);
	VOID SendAddNew2Client(const tagItem *pItem);
	VOID SendItemBind(INT64 n64Seria, DWORD dwContTypel);

	VOID InsertItem2DB(tagItem &item);
	VOID DeleteItemFromDB(INT64 n64Serial, INT32 dwTypeID);

	//-----------------------------------------------------------------------------
	// װ�����Ըı�󣬼�ʱ�������ݿ⼰��ͻ��˷�����Ϣ
	//-----------------------------------------------------------------------------
	VOID SendEquipSpec2DB(const tagEquip &equip);
	VOID SendEquipSpec2Client(const tagEquip &equip);
	VOID SendEquipSingleSpecUpdate(const tagEquip &equip,EEquipSignleSpecType eEquipSingleSpecType);
public:
	// Jason 2010-4-15 v2.0.0
	VOID SendFabaoSpec2Client(const tagFabao & fabao);
	VOID SendFabaoSpec2DB(const tagFabao& fabao);

	// wcy 2012-4-09
	VOID SendHolySpec2Client(const tagHolyMan & holy);
	VOID SendHolySpec2DB(const tagHolyMan & holy);

	// wcy 2012-4-26
	VOID SendHolyEquipSpec2Client(const tagHolyEquip & holyEquip);
	VOID SendHolyEquipSpec2DB(const tagHolyEquip & holyEquip);
	
	// װ���İ�״̬�ı�ʱ֪ͨ�ͻ���
	VOID SendItemBindStatus(INT64 n64Seria, DWORD dwContTypel, BYTE byStatus);

	VOID	SynchFabaoMood(const tagFabao & fabao,INT nOldMood = 0,BOOL bSaveToDB=FALSE);
	VOID	SynchHolyCoValue(const tagHolyMan & holy, BOOL bSaveToDB=FALSE);

	// �꾧
	VOID SendSoulCrystalSpec2Client(const tagSoulCrystal& obj);
	VOID SendSoulCrystalSpec2DB(const tagSoulCrystal& obj);

	// ��������Ԫ���˺�
	VOID	UpdateFabaoEleInjuryType2DB(const tagFabao & fabao);

	VOID	UpdateFabaoMating();

	VOID	InsertMatingFabao(tagFabao * pFabao);
	VOID	RemoveMatingFabao(tagFabao * pFabao);

	VOID	SaveMatingFabao2DB();
private:
	__forceinline VOID LogItem(const tagItem &item1, const tagItem *pItem2, 
							  INT16 n16OptNum, DWORD dwCmdID, DWORD dwRoleIDRel = GT_INVALID);
	__forceinline VOID LogItemTimes(const tagItem &item, DWORD dwCmdID);

	BOOL IsItemNeedLog(const tagItem &item) const { return item.pProtoType->bNeedLog; }


private:
	Role*				m_pRole;

	ItemContainer		m_Bag;				// ����
	ItemContainer		m_QuestItemBag;		// ������Ʒ��
	ItemContainer		m_BaiBaoBag;		// �����ݿ������Ʒ, ע��Addʱ�ĵ��ýӿ�
	ItemContainer		m_RoleWare;			// �ŵ���ɫ�ֿ��е���Ʒ,���ı�����(ע��Addʱ�ĵ��ýӿ�)
	ItemContainer		m_TempBag;			// ��ʱ��������������ںϲ����������� [3/29/2012 zhangzhihua]
	EquipContainer		m_EquipBar;			// װ����
	WardrobeContainer   m_Wardrobe;			// �³�
	HolySoulContainer   m_HolySoulBar;         // Ԫ����
	ItemContainer		m_WarBag;			// ר������ [4/25/2012 zhangzhihua]

	ItemContainer		m_HolyManBar;		// ʥ��װ����

	MapUnlockCD			m_mapUnlockCD;		// ������������ʱ�е���Ʒ�ļ���
	MapCDTime			m_mapCDTime;		// ��Ʒ&װ����ȴʱ��<dwTypeID, dwRemainTime>
	MapMaxHold			m_mapMaxHold;		// ���ƿ�ӵ��������Ʒ����

	//struct tagFariyMatingData
	//{
	//	EItemConType _con_type;
	//	UINT64	_fairy_id;
	//	tagFariyMatingData & operator = (const tagFariyMatingData & data)
	//	{
	//		_con_type = data._con_type;
	//		_fairy_id = data._fairy_id;
	//		return *this;
	//	}
	//	tagFariyMatingData()
	//		:_con_type(EICT_Null),_fairy_id(0){}
	//	tagFariyMatingData(const tagFariyMatingData &data)
	//	{
	//		_con_type = data._con_type;
	//		_fairy_id = data._fairy_id;
	//	}
	//};
	TMap<UINT64,EItemConType > m_FairyMating;	// ���������أ������±��
	DWORD	m_dwLastFabaoMatingTick;

public:
	// ͨ����Ʒ�������� 
	DWORD AddAreaByItme( INT64 n64ItemID );
	VOID CheckAllEquip(Role* pRole,DWORD dwMapID);

	// �ж���ҵ�ʥ����Ŀ�Ƿ��Ѵﵽ����
	BOOL IsHolyExceed();
	// �����ҵ�ʥ����Ŀ
	int GetHolyNum();
};


//-------------------------------------------------------------------------------------------------------
// �������л�ȡ���е���Ʒ����,��������С
//-------------------------------------------------------------------------------------------------------
template<class T> 
VOID ItemMgr::GetAllItem(IN Container<T> &con, const INT16 n16ReadNum, OUT LPVOID pData, OUT INT32 &nSize)
{
	nSize	= 0;

	INT16	n16Num	= 0;
	T		*pTemp	= NULL;
	BYTE	*byData = (BYTE*)pData;

	for(INT16 i=0; i<con.GetCurSpaceSize(); ++i)
	{
		pTemp = con.GetItem(i);
		if(P_VALID(pTemp))
		{
			if(MIsEquipment(pTemp->dwTypeID))	// װ��
			{
				// Jason 2010-4-20 v2.0.0
				if( MIsFaBao(pTemp) )
				{
					IFASTCODE->MemCpy(byData, pTemp, SIZE_FABAO);
					((tagEquip*)byData)->equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������
					byData += SIZE_FABAO;
					continue;
				}
				// wcy 2012-4-01 
				if( MIsHoly(pTemp) )
				{
					IFASTCODE->MemCpy(byData, pTemp, SIZE_HOLY);
					((tagEquip*)byData)->equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������
					byData += SIZE_HOLY;
					continue;
				}
				// wcy 2012-4-18 
				if( MIsHolyEquip(pTemp) )
				{
					IFASTCODE->MemCpy(byData, pTemp, SIZE_HOLYEQUIP);
					((tagEquip*)byData)->equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������
					byData += SIZE_HOLYEQUIP;
					continue;
				}
				else if (MIsSoulCrystal(pTemp))
				{
					IFASTCODE->MemCpy(byData, pTemp, SIZE_HUNJING);
					((tagEquip*)byData)->equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������
					byData += SIZE_HUNJING;
					continue;
				}
				IFASTCODE->MemCpy(byData, pTemp, SIZE_EQUIP);
				((tagEquip*)byData)->equipSpec.n16QltyModPctEx = 0;	// �Կͻ������ض���������
				byData += SIZE_EQUIP;
			}
			else	// ��Ʒ
			{
				IFASTCODE->MemCpy(byData, pTemp, SIZE_ITEM);
				byData += SIZE_ITEM;
			}

			++n16Num;
			if(n16ReadNum == n16Num)
			{
				break;
			}
		}
	}

	nSize = byData - (BYTE*)pData;
}

//-------------------------------------------------------------------------------------------------------
// ��LoongDBͬ����Ʒװ����Ϣ(ֻͬ��λ�ü�ʹ�������Ϣ)
//-------------------------------------------------------------------------------------------------------
template<class T> 
VOID ItemMgr::Save2DB(IN Container<T> &con, OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	nNum = 0;

	MTRANS_POINTER(pCurPointer, pData, tagItemUpdate);

	T	*pTemp	= NULL;
	for(INT16 i=0; i<con.GetCurSpaceSize(); ++i)
	{
		pTemp = con.GetItem(i);
		if(P_VALID(pTemp) && pTemp->eStatus != EUDBS_Null)
		{
			pCurPointer[nNum].n64Serial		= pTemp->n64Serial;
			pCurPointer[nNum].dwOwnerID		= pTemp->dwOwnerID;
			pCurPointer[nNum].dwAccountID	= pTemp->dwAccountID;
			pCurPointer[nNum].nUseTimes		= pTemp->nUseTimes;
			pCurPointer[nNum].n16Num		= pTemp->n16Num;
			pCurPointer[nNum].n16Index		= pTemp->n16Index;
			pCurPointer[nNum].byConType		= pTemp->eConType;
			pCurPointer[nNum].byBindType	= pTemp->byBind;
			pCurPointer[nNum].dw1stGainTime = pTemp->dw1stGainTime;
			//pCurPointer[nNum].dwUnlockTime	= pTemp->dwUnlockTime;
			pCurPointer[nNum].dwSpecVal1		= pTemp->dwSpecVal1;
			pCurPointer[nNum].dwSpecVal2		= pTemp->dwSpecVal2;

			pTemp->SetUpdate(EUDBS_Null);

			++nNum;
		}
	}

	pOutPointer = &pCurPointer[nNum];
}


//****************************** ��������ʵ�� **********************************


//-----------------------------------------------------------------------------
// ��ʽ���������ݿ�����
//-----------------------------------------------------------------------------
inline VOID ItemMgr::SaveCDTime2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	FormatCDTime(pData, pOutPointer, nNum);
}

//-----------------------------------------------------------------------------
// �Ƿ���Գ���
//-----------------------------------------------------------------------------
inline BOOL ItemMgr::CanSell(const tagItem& item) const
{
	const tagItem * pItem = &item;
	BOOL bOk = TRUE;
	if(MIsFaBao(pItem))
	{
		const tagFabao * pFabao = (const tagFabao *)pItem;
		bOk = pFabao->CanExchange();
	}
	return bOk && (item.pProtoType->bCanSell && !item.bLock && !MIsQuestItem(item.pProtoType));
}

//-----------------------------------------------------------------------------
// ����װ����̬��Ϣ�������Ϣ��װ
//-----------------------------------------------------------------------------
inline VOID ItemMgr::UpdateEquipSpec(const tagEquip &equip,bool bSingleAttChange,EEquipSignleSpecType eEquipSingleSpecType)
{
	if ( !bSingleAttChange )
	{
		SendEquipSpec2DB(equip);
		SendEquipSpec2Client(equip);
	}
	else
	{
		SendEquipSingleSpecUpdate(equip,eEquipSingleSpecType);
	}

}

//-----------------------------------------------------------------------------
// �������и�����
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetBagFreeSize()
{
	return GetBag().GetFreeSpaceSize();
}

//-----------------------------------------------------------------------------
// ������ǰ�ܸ�����
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetBagCurSize()
{
	return GetBag().GetCurSpaceSize();
}

//-----------------------------------------------------------------------------
// ��ȡ������һ�����и����±�
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetBagOneFreeSpace()
{
	return GetBag().GetOneFreeSpace();
}

//-----------------------------------------------------------------------------
// ��ȡ��������ͬ��Ʒ���ܸ���
//-----------------------------------------------------------------------------
inline INT32 ItemMgr::GetBagSameItemCount(DWORD dwTypeID)
{
	return GetBag().GetSameItemCount(dwTypeID);
}

//----------------------------------------------------------------------
// �����������Ʒ����ΪdwTypeID��lis, ����ʵ�ʻ�ø���t -- ָ��nNumʱ�����ҵ�nNum����Ʒ����
//----------------------------------------------------------------------
inline INT32 ItemMgr::GetBagSameItemList(OUT TList<tagItem*> &list, IN DWORD dwTypeID, IN INT32 nNum)
{
	return GetBag().GetSameItemList(list, dwTypeID, nNum);
}
//-----------------------------------------------------------------------------
// ��ȡ������ĳ������Ʒ���ܸ���
//-----------------------------------------------------------------------------
inline INT32	ItemMgr::GetBagSameItemCount2(EItemType	eType)
{
	return GetBag().GetSameItemCount2(eType);
}
//-----------------------------------------------------------------------------
// ��ȡ��������ͬ��Ʒ���ܸ���
//-----------------------------------------------------------------------------
inline INT32 ItemMgr::GetQuestBagSameItemCount(DWORD dwTypeID)
{
	return GetQuestItemBag().GetSameItemCount(dwTypeID);
}

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ
//-----------------------------------------------------------------------------
inline tagItem *ItemMgr::GetBagItem(INT64 n64Serial)
{
	return GetBag().GetItem(n64Serial);
}

//-----------------------------------------------------------------------------
// ��ȡ��Ʒ
//-----------------------------------------------------------------------------
inline tagItem *ItemMgr::GetBagItem(INT16 n16Index)
{
	return GetBag().GetItem(n16Index);
}

//-----------------------------------------------------------------------------
// ��ȡս��������Ʒ
//-----------------------------------------------------------------------------
inline tagItem* ItemMgr::GetWarBagItem(INT16 n16Index)
{
	return GetWarBag().GetItem(n16Index);
}

//-----------------------------------------------------------------------------
// ��ȡս��������Ʒ
//-----------------------------------------------------------------------------
inline tagItem* ItemMgr::GetWarBagItem(INT64 n64Serial)
{
	return GetWarBag().GetItem(n64Serial);
}

// ��ȡʥ��װ�����е�ʥ��
inline tagItem*	ItemMgr::GetHolyManBarItem(INT64 n64Serial)
{
	return GetHolyManBar().GetItem(n64Serial);
}

//-----------------------------------------------------------------------------
// ������ָ��λ�ø����Ƿ����
//-----------------------------------------------------------------------------
inline BOOL	ItemMgr::IsBagOneSpaceFree(INT16 n16Index)
{
	return GetBag().IsOnePlaceFree(n16Index);
}

//-----------------------------------------------------------------------------
// ���װ��������Ʒ
//-----------------------------------------------------------------------------
inline tagEquip* ItemMgr::GetEquipBarEquip(INT64 n64Serial)
{
	return GetEquipBar().GetItem(n64Serial);
}

//-----------------------------------------------------------------------------
// ���װ��������Ʒ
//-----------------------------------------------------------------------------
inline tagEquip* ItemMgr::GetEquipBarEquip(INT16 n16Index)
{
	return GetEquipBar().GetItem(n16Index);
}

//-----------------------------------------------------------------------------
// ���װ��������Ʒ
//-----------------------------------------------------------------------------
inline tagEquip* ItemMgr::GetEquipBarEquip(DWORD dwTypeID)
{
	return GetEquipBar().GetEquipBarEquip(dwTypeID);
}

//-----------------------------------------------------------------------------
// ���Ԫ����λ����Ʒ
//-----------------------------------------------------------------------------
inline tagEquip*  ItemMgr::GetHolySoulBarEquip(INT64 n64Serial)
{
	return GetHolySoulBar().GetItem(n64Serial);
}

//-----------------------------------------------------------------------------
// ���Ԫ����λ����Ʒ
//-----------------------------------------------------------------------------
inline tagEquip*	ItemMgr::GetHolySoulBarEquip(INT16 n16Index)
{
	return GetHolySoulBar().GetItem(n16Index);
}

//-----------------------------------------------------------------------------
// ���Ԫ����λ����Ʒ
//-----------------------------------------------------------------------------
inline tagEquip*	ItemMgr::GetHolySoulBarEquip(DWORD dwTypeID)
{
	return GetHolySoulBar().GetEquipBarEquip(dwTypeID);
}

//-----------------------------------------------------------------------------
// ��ȡ�ٱ������и�����
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetBaiBaoFreeSize()
{
	return GetBaiBaoBag().GetFreeSpaceSize();
}

//-----------------------------------------------------------------------------
// ��ɫ�ֿ⵱ǰ�ܸ�����
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetWareCurSize()
{
	return GetRoleWare().GetCurSpaceSize();
}

//-----------------------------------------------------------------------------
// ս���������и�����
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetWarBagFreeSize()
{
	return GetWarBag().GetFreeSpaceSize();
}

//-----------------------------------------------------------------------------
// �õ����������ÿռ�
//-----------------------------------------------------------------------------
inline INT16 ItemMgr::GetQuestItemBagFreeSize()
{
	return GetQuestItemBag().GetFreeSpaceSize();
}

//-----------------------------------------------------------------------------
// �ӱ��������ȡ��һ����Ʒ��64λid
//-----------------------------------------------------------------------------
inline BOOL	ItemMgr::GetBagRandom(INT64 &n64Serial)
{
	return GetBag().GetRandom(n64Serial);
}

//-----------------------------------------------------------------------------
// �������������ȡ��һ����Ʒ��64λid
//-----------------------------------------------------------------------------
inline BOOL	ItemMgr::GetQuestItemBagRandom(INT64 &n64Serial)
{
	return GetQuestItemBag().GetRandom(n64Serial);
}

//-----------------------------------------------------------------------------
// ��ȡչʾ��Ʒ����
//-----------------------------------------------------------------------------
inline tagItem* ItemMgr::GetDisplayItem(EItemConType eConType, INT64 n64Serial) 
{ 
	//�Ƿ�װ����
	if(eConType == EICT_Equip)
	{
		//��װ����
		return GetEquipBar().GetItem((INT16)n64Serial);
	}
	else
	{
		ItemContainer* pContainer = GetContainer(eConType); 
		if (!P_VALID(pContainer))
			return NULL;
		return pContainer->GetItem(n64Serial);
	}
}

//-----------------------------------------------------------------------------
// ��װ��
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::Equip(INT64 n64SerialSrc, EEquipPos ePosDst)
{
	return GetEquipBar().Equip(GetBag(), n64SerialSrc, ePosDst);
}

inline DWORD ItemMgr::EquipByFamily(tagItem *pItem, EEquipPos ePosDst)
{
	return GetEquipBar().EquipByFamily( pItem, ePosDst );
}
//-----------------------------------------------------------------------------
// ��װ��
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::Unequip(INT64 n64SerialSrc, INT16 n16IndexDst)
{
	return GetEquipBar().Unequip(n64SerialSrc, GetBag(), n16IndexDst);
}

inline DWORD ItemMgr::UnequipOnly(INT64 n64SerialSrc)
{
	return GetEquipBar().Unequip(n64SerialSrc);
}
//-----------------------------------------------------------------------------
// ��������Ʒ�Ի�
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::SwapWeapon()
{
	return GetEquipBar().SwapWeapon();
}

//-----------------------------------------------------------------------------
// ������ָ����
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::MoveRing(INT64 n64SerialSrc, INT16 n16PosDst)
{
	return GetEquipBar().MoveTo(n64SerialSrc, (EEquipPos)n16PosDst);
}

//-----------------------------------------------------------------------------
// ɾ�����������Ʒ -- ��鱳����������Ʒ��
//-----------------------------------------------------------------------------
inline VOID ItemMgr::RemoveFromRole(UINT16 u16QuestID, DWORD dwCmdID)
{
	RemoveItems(GetBag(), u16QuestID, dwCmdID);
	RemoveItems(GetQuestItemBag(), u16QuestID, dwCmdID);
	RemoveItems(GetEquipBar(), u16QuestID, dwCmdID);
}

//-----------------------------------------------------------------------------
// ��һ����Ʒ&װ��
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::Add2Bag(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB, BOOL bCheckAdd, BOOL bFromFamily/* = FALSE*/ )
{
	// �����ս����Ʒ
	if (MIsWarItem(pItem->pProtoType))
	{
		return AddItem(GetWarBag(), pItem, dwCmdID, FALSE, bCheckAdd,-1,TRUE,TRUE);
	}

	return AddItem(GetBag(), pItem, dwCmdID, bInsert2DB, bCheckAdd,-1,TRUE,bFromFamily);
}

inline DWORD ItemMgr::AddHolyManBar(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB, BOOL bCheckAdd, BOOL bFromFamily )
{
	return AddItem(GetHolyManBar(), pItem, dwCmdID, bInsert2DB, bCheckAdd,-1,TRUE,bFromFamily);
}

inline DWORD ItemMgr::Add2WarBag(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB, BOOL bCheckAdd, BOOL bFromFamily/* = FALSE*/)
{
	return AddItem(GetWarBag(), pItem, dwCmdID, FALSE, bCheckAdd, TRUE);
}

inline DWORD ItemMgr::Add2QuestBag(tagItem *&pItem, DWORD dwCmdID)
{
	// �����ս����Ʒ
	if (MIsWarItem(pItem->pProtoType))
	{
		return AddItem(GetWarBag(), pItem, dwCmdID, FALSE, TRUE);
	}

	return AddItem(GetQuestItemBag(), pItem, dwCmdID, TRUE, TRUE);
}

inline DWORD ItemMgr::Add2RoleWare(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB, BOOL bCheckAdd, BOOL bFromFamily/*=FALSE*/)
{
	return AddItem(GetRoleWare(), pItem, dwCmdID, bInsert2DB, bCheckAdd,bFromFamily);
}

inline DWORD ItemMgr::Add2TempBag(tagItem *&pItem, DWORD dwCmdID, BOOL bInsert2DB/* = FALSE*/, BOOL bCheckAdd/* = TRUE*/, BOOL bFromFamily/* = FALSE*/ )
{
	return AddItem(GetTempBag(), pItem, dwCmdID, bInsert2DB, bCheckAdd, GT_INVALID, TRUE, TRUE);
}

//-----------------------------------------------------------------------------
// �ӱ�����ȡ������������������ -- �ڴ�û���ͷ�
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::TakeOutFromBag(INT64 n64Serial, DWORD dwCmdID, BOOL bDelFromDB)
{
	return RemoveItem(GetBag(), n64Serial, dwCmdID, bDelFromDB, FALSE, TRUE);
}
inline DWORD ItemMgr::TakeOutFromHolyBar(INT64 n64Serial, DWORD dwCmdID, BOOL bDelFromDB)
{
	return RemoveItem(GetHolyManBar(), n64Serial, dwCmdID, bDelFromDB, FALSE, TRUE);
}
//-----------------------------------------------------------------------------
// ������Ʒ���� -- n16NumȡĬ��ֵʱ��ʾȫ��ɾ��������db����������ͷ��ڴ�
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::DelFromBag(INT64 n64Serial, DWORD dwCmdID, INT16 n16Num, BOOL bCheckDel)
{
	if(GT_INVALID == n16Num)
	{
		return RemoveItem(GetBag(), n64Serial, dwCmdID, TRUE, TRUE, bCheckDel);
	}

	return RemoveItem(GetBag(), n64Serial, n16Num, dwCmdID, bCheckDel, TRUE);
}

inline DWORD ItemMgr::DelFromTempBag(INT64 n64Serial, DWORD dwCmdID, INT16 n16Num, BOOL bCheckDel)
{
	if(GT_INVALID == n16Num)
	{
		return RemoveItem(GetTempBag(), n64Serial, dwCmdID, TRUE, TRUE, bCheckDel);
	}

	return RemoveItem(GetTempBag(), n64Serial, n16Num, dwCmdID, bCheckDel, TRUE);
}

//-----------------------------------------------------------------------------
// ������Ʒ(ʹ�ô��������) -- ʣ��ʹ�ô���Ϊ0ʱ����db��ɾ�������ͷ��ڴ�
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::ItemUsedFromBag(INT64 n64Serial, INT16 n16Num, DWORD dwCmdID, BOOL bRemoveLimited/*=TRUE*/)
{
	return RemoveItem(GetBag(), n64Serial, n16Num, dwCmdID, TRUE, FALSE, bRemoveLimited);
}

//-----------------------------------------------------------------------------
// ������Ʒ���� -- n16NumȡĬ��ֵʱ��ʾȫ��ɾ��������db����������ͷ��ڴ�
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::DelFromWarBag(INT64 n64Serial, DWORD dwCmdID, INT16 n16Num, BOOL bCheckDel)
{
	if(GT_INVALID == n16Num)
	{
		return RemoveItem(GetWarBag(), n64Serial, dwCmdID, TRUE, TRUE, bCheckDel);
	}

	return RemoveItem(GetWarBag(), n64Serial, n16Num, dwCmdID, bCheckDel, TRUE);
}

//-----------------------------------------------------------------------------
// ������Ʒ(ʹ�ô��������) -- ʣ��ʹ�ô���Ϊ0ʱ����db��ɾ�������ͷ��ڴ�
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::ItemUsedFromWarBag(INT64 n64Serial, INT16 n16Num, DWORD dwCmdID)
{
	return RemoveItem(GetWarBag(), n64Serial, n16Num, dwCmdID, TRUE, FALSE);
}

//-----------------------------------------------------------------------------
// ������ɾ������Ӷ����Ʒ(��Ҽ佻��,�ʼ�,����)
//-----------------------------------------------------------------------------
inline VOID ItemMgr::Add2Bag(tagItem* pItem[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel)
{
	AddItems(GetBag(), pItem, nSize, dwCmdID, dwRoleIDRel);
}

inline VOID ItemMgr::RemoveFromBag(INT64 n64Serial[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel)
{
	RemoveItems(GetBag(), n64Serial, nSize, dwCmdID, dwRoleIDRel);
}

inline BOOL ItemMgr::CheckExistInBag(OUT tagItem* pItem[], INT64 n64Serial[], INT16 n16Num[], INT32 nSize)
{
	return CheckItemsExist(pItem, GetBag(), n64Serial, n16Num, nSize);
}

inline VOID ItemMgr::RemoveFromHolyManBar(INT64 n64Serial[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel)
{
	RemoveItems(GetHolyManBar(), n64Serial, nSize, dwCmdID, dwRoleIDRel);
}

inline VOID ItemMgr::AddHolyManBar(tagItem* pItem[], INT32 nSize, DWORD dwCmdID, DWORD dwRoleIDRel)
{
	AddItems(GetHolyManBar(), pItem, nSize, dwCmdID, dwRoleIDRel);
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::ReorderBag(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num)
{
	return GetBag().Reorder(pData, pOutData, n16Num) ? E_Success : GT_INVALID;
}

inline DWORD ItemMgr::ReorderBagEx(IN LPVOID pData, OUT LPVOID pOutData, 
							OUT INT16 &n16OutNum, const INT16 n16Num)
{
	return GetBag().ReorderEx(pData, pOutData, n16OutNum, n16Num) ? E_Success : GT_INVALID;
}

//-----------------------------------------------------------------------------
// ��ɫ�ֿ�����
//-----------------------------------------------------------------------------
inline DWORD ItemMgr::ReorderRoleWare(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num)
{
	return GetRoleWare().Reorder(pData, pOutData, n16Num) ? E_Success : GT_INVALID;
}

inline DWORD ItemMgr::ReorderRoleWareEx(IN LPVOID pData, OUT LPVOID pOutData, 
							   OUT INT16 &n16OutNum, const INT16 n16Num)
{
	return GetRoleWare().ReorderEx(pData, pOutData, n16OutNum, n16Num) ? E_Success : GT_INVALID;
}
//-----------------------------------------------------------------------------



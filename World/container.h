//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Container.h
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: ��Ʒ������
//-----------------------------------------------------------------------------
#pragma once

#include "container_template.h"
#include "time_limit_mgr.h"
#include "../WorldDefine/ItemDefine.h"

class ContainerRestrict;

struct tagItemMove;
//-----------------------------------------------------------------------------
// ��Ʒ&װ������
//-----------------------------------------------------------------------------
class ItemContainer: public Container<tagItem, INT64>
{
public:
	ItemContainer(EItemConType eConType, INT16 n16CurSize, INT16 n16MaxSize, 
					DWORD dwOwnerID, DWORD dwAccountID, ContainerRestrict *pRestrict = NULL);
	~ItemContainer();

	VOID Update();

public:
	// ���������������Ʒ,����ָ�����λ��,���ش�����
	DWORD Add(tagItem* pItem, OUT INT16 &n16Index, OUT tagItemMove &ItemMove, BOOL bCheckAdd = TRUE, BOOL bChangeOwner = TRUE);
	
	// ��������ָ��λ���������Ʒ,���ش�����(ָ��λ�ñ���Ϊ��).
	DWORD Add(tagItem* pItem, INT16 n16NewIndex, BOOL bChangeOwner = TRUE, BOOL bCheckAdd = TRUE);

	// ��������ɾ��ָ����Ʒ,���ش�����
	DWORD Remove(INT64 n64Serial, BOOL bChangeOwner = FALSE, BOOL bCheckRemove = TRUE);
	DWORD Remove(INT64 n64Serial, INT16 n16Num, BOOL bCheckRemove = TRUE);
	
	// ����Ʒ�ƶ���ָ��λ��(ͬһ������)
	DWORD MoveTo(INT64 n64Serial1, INT16 n16Index2, OUT tagItemMove &ItemMove);
	DWORD MoveTo(INT64 n64Serial1, INT16 n16NumMove, INT16 n16Index2, OUT tagItemMove &ItemMove);

	// ����Ʒ�ƶ�������������(�ֿ�ͱ�����)
	DWORD MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, OUT tagItemMove &ItemMove, OUT INT16 &n16IndexDst);
	DWORD MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, INT16 n16IndexDst, OUT tagItemMove &ItemMove);

	// ����
	VOID IncreaseSize(INT16 n16Size);

	// ��ָ��λ������
	BOOL Reorder(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num);
	BOOL ReorderEx(IN LPVOID pData, OUT LPVOID pOutData, OUT INT16 &n16OutNum, const INT16 n16Num);

public:
	// ���һ����λ��ʧ�ܷ���GT_INVALID
	INT16 GetOneFreeSpace();

	// ���������������Ʒ����ΪdwTypeID����Ʒ����
	INT32 GetSameItemCount(IN DWORD dwTypeID);

	// ���������ĳ������Ʒ���ܸ���
	INT32 GetSameItemCount2(IN EItemType	eType);
	

	// �����������Ʒ����ΪdwTypeID��lis, ����ʵ�ʻ�ø���t -- ָ��nNumʱ�����ҵ�nNum����Ʒ����
	INT32 GetSameItemList(OUT TList<tagItem*> &list, IN DWORD dwTypeID, IN INT32 nNum = INT_MAX);

	// ��ȡ������ָ�����͵�������Ʒ�ĸ���
	INT32 GetSameFuncSpecItemCount(EItemSpecFunc eItemSpecFunc);

	// ��ȡ������һ��ָ������������ĵ���
	tagItem* GetOneSpecItemBySpecFunEnum(EItemSpecFunc eItemSpecFunc);

	ContainerRestrict*	GetRestrict()	const;
	DWORD				GetOwnerID()	const;

	// ʱ�޹���ӿ�
	TList<INT64>& GetNeedDelList();
	VOID ClearNeedDelList();

	VOID Add2TimeLeftList(INT64, DWORD, tagDWORDTime);

	DWORD GetZhanDouFuCnt() {	return m_nZhanDouFuItemCnt;		}

	BOOL IsInTimeLimitMgr(INT64 n64Serial)	{	return m_TimeLimitMgr.IsExist(n64Serial);		}

	// ������Ʒ��ʼ��ʱ���Զѵ���Ʒ�����⴦��
	BOOL OverLappedItemHandler(tagItem* pItem);	

private:
	// ���������п���λ�õ���С�±�
	VOID ResetMinFreeIndex();

protected:
	DWORD				m_dwOwnerID;			// ����������
	DWORD				m_dwAccountID;			// ����˺�ID, �ٱ����ͽ�ɫ�ֿ���(ͬһ�ʺ��½�ɫ����)

private:
	// ������Ա
	INT16				m_n16MinFreeIndex;		// �����п���λ�õ���С�±�(== m_n16CurSizeʱ���޿���λ��)
	ContainerRestrict*	m_pRestrict;			// ��Ʒ��������Լ����
	TimeLimitMgr<INT64>	m_TimeLimitMgr;			// ʱ����Ʒ������

	// ���⴦��ս����,��¼ս�������߸���
	INT16				m_nZhanDouFuItemCnt;
};


//-----------------------------------------------------------------------------
// װ������
//-----------------------------------------------------------------------------
class EquipContainer: public Container<tagEquip, INT64>
{
public:
	EquipContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize);
	virtual ~EquipContainer();

	VOID Update();

public: 
	// ����װ��
	DWORD Equip(ItemContainer &bagSrc, INT64 n64SerialSrc, EEquipPos ePosDst);
	DWORD EquipByFamily(tagItem *pItemNew, EEquipPos ePosDst);
	// ����װ��
	DWORD Unequip(INT64 n64SerialSrc, ItemContainer &bagDst);
	// ����װ��(ָ��������λ��)
	DWORD Unequip(INT64 n64SerialSrc, ItemContainer &bagDst, INT16 n16IndexDst);
	// �ƶ�(����������ָλ)
	DWORD MoveTo(INT64 n64SerialSrc, EEquipPos ePosDst);
	// �����ֽ���
	DWORD SwapWeapon();

	//����װ�����������κ�������
	DWORD Unequip( INT64 n64SerialSrc );

	// ʱ�޹���ӿ�
	TList<INT64>& GetNeedDelList();
	VOID ClearNeedDelList();

	tagEquip* GetEquipBarEquip(DWORD dwTypeID);

public:
	// װ��
	DWORD Add(tagEquip *pEquip, EEquipPos ePos);

private:
	// ����
	TimeLimitMgr<INT64>	m_TimeLimitMgr;			// ʱ����Ʒ������
};

//-----------------------------------------------------------------------------
// �³�����
//-----------------------------------------------------------------------------
class WardrobeContainer: public ItemContainer
{
public:
	WardrobeContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize, 
		DWORD dwOwnerID, DWORD dwAccountID, ContainerRestrict *pRestrict = NULL)
		:ItemContainer(eConType, n16CurSize, n16MaxSize, dwOwnerID, dwAccountID)
	{
	}
	virtual ~WardrobeContainer()
	{
	}

	// �Ƿ�����ƶ�ʱװ(ͬһ������)
	DWORD CanMove(INT64 n64Serial, INT16 n16TargetIndex);

	// �Ƿ��������������ӵ���
	DWORD CanAdd2Wardrobe(INT64 n64Serial, ItemContainer& RoleBag, INT16 &n16DstIndex);
	DWORD CanAdd2Wardrobe(tagItem *pItem, INT16 &n16DstIndex);
	EAvatarElement EWFP2EAE(EWardrobeFashionPos eWardrobeFashionPos)
	{
		EAvatarElement eAvatarElement = EAE_Null;
		switch(eWardrobeFashionPos)
		{
		case EWFP_Head:		 // ͷ��
			eAvatarElement = EAE_Head;
			break;
		case EWFP_Body:		 // �·�
			eAvatarElement = EAE_Upper;
			break;
		case EWFP_Back:		 // ����
			eAvatarElement = EAE_Back;
			break;
		case EWFP_Wrist:		 // ��
			eAvatarElement = EAE_Glove;
			break;
		case EWFP_Feet:			 // �㲿
			eAvatarElement = EAE_Boot;
			break;
		default:
			break;
		}
		return eAvatarElement;
	}
private:
	// ���һ����λ��ʧ�ܷ���GT_INVALID
	INT16 GetOneFreeSpace(DWORD dwTypeID);
	DWORD CanAddNormalCheck( tagItem * pItem, INT16 n16TargetIndex );
	BOOL CanAddItem(tagItem* pItem);
	EWardrobeFashionPos EquipPos2WardrobeFashionPos(EEquipPos eEquipPos)
	{
		EWardrobeFashionPos eWardrobeFashionPos = EWFP_Null;
		switch(eEquipPos)
		{
		case EEP_FashionHead:		 // ͷ��
			eWardrobeFashionPos = EWFP_Head;
			break;
		case EEP_FashionBody:		 // �·�
			eWardrobeFashionPos = EWFP_Body;
			break;
		case EEP_FashionBack:		 // ����
			eWardrobeFashionPos = EWFP_Back;
			break;
		case EEP_FashionWrist:		 // ��
			eWardrobeFashionPos = EWFP_Wrist;
			break;
		case EEP_FashionFeet:			 // �㲿
			eWardrobeFashionPos = EWFP_Feet;
			break;
		default:
			break;
		}
		return eWardrobeFashionPos;
	}
};

//************************ ��ͨ��������ʵ�� ************************//

//----------------------------------------------------------------------
// ����
//----------------------------------------------------------------------
inline VOID ItemContainer::IncreaseSize(INT16 n16Size) 
{ 
	INT16 n16CurSize = GetCurSpaceSize();

	if(n16Size <= 0 || n16Size + n16CurSize > GetMaxSpaceSize())
	{
		ASSERT(n16Size > 0);
		ASSERT(n16Size + n16CurSize <= GetMaxSpaceSize());
		return ;
	}

	if(GetOneFreeSpace() == GT_INVALID)
	{
		m_n16MinFreeIndex = n16CurSize;
	}

	Container<tagItem>::Resize(n16Size + n16CurSize);
}

//----------------------------------------------------------------------
// ��ָ��λ������
//----------------------------------------------------------------------
inline BOOL ItemContainer::Reorder(IN LPVOID pData, OUT LPVOID pOutData, const INT16 n16Num)
{
	if(Container::Reorder(pData, pOutData, n16Num))
	{
		// ��С�����±�������
		m_n16MinFreeIndex = n16Num;
		ResetMinFreeIndex();
		return TRUE;
	}

	return FALSE;
}

inline BOOL ItemContainer::ReorderEx(IN LPVOID pData, OUT LPVOID pOutData, 
									 OUT INT16 &n16OutNum, const INT16 n16Num)
{
	if(Container::ReorderEx(pData, pOutData, n16OutNum, n16Num))
	{
		// ��С�����±�������
		ResetMinFreeIndex();
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------
// ���һ����λ��ʧ�ܷ���GT_INVALID
//----------------------------------------------------------------------
inline INT16 ItemContainer::GetOneFreeSpace()
{
	ResetMinFreeIndex();
	return m_n16MinFreeIndex >= GetCurSpaceSize() ? GT_INVALID : m_n16MinFreeIndex;
}

//----------------------------------------------------------------------
// ���������������Ʒ����ΪdwTypeID����Ʒ����
//----------------------------------------------------------------------
__forceinline INT32 ItemContainer::GetSameItemCount(IN DWORD dwTypeID)
{
	INT32 nNumRet = 0;

	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		if(!Container::IsOnePlaceFree(i) && (Container::GetItem(i))->dwTypeID == dwTypeID)
		{
			nNumRet += Container::GetItem(i)->n16Num;
		}
	}

	return nNumRet;
}

// ���������ĳ������Ʒ���ܸ���
__forceinline INT32 ItemContainer::GetSameItemCount2(IN EItemType	eType)
{
	INT32 nNumRet = 0;

	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		if(!Container::IsOnePlaceFree(i) && (Container::GetItem(i))->pProtoType->eType == eType)
		{
			nNumRet += Container::GetItem(i)->n16Num;
		}
	}

	return nNumRet;	
}

//----------------------------------------------------------------------
// �����������Ʒ����ΪdwTypeID��lis, ����ʵ�ʻ�ø���t -- ָ��nNumʱ�����ҵ�nNum����Ʒ����
//----------------------------------------------------------------------
__forceinline INT32 ItemContainer::GetSameItemList(OUT TList<tagItem*> &list, 
												   IN DWORD dwTypeID, IN INT32 nNum/* = INT_MAX*/)
{
	INT32 nNumRet = 0;

	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		if(!Container::IsOnePlaceFree(i) && (Container::GetItem(i))->dwTypeID == dwTypeID)
		{
			nNumRet += Container::GetItem(i)->n16Num;
			list.PushBack(Container::GetItem(i));

			if(nNumRet > nNum)
			{
				break;
			}
		}
	}

	return nNumRet;
}

//----------------------------------------------------------------------
// ��ȡ������ָ�����͵�������Ʒ�ĸ���
//----------------------------------------------------------------------
__forceinline INT32 ItemContainer::GetSameFuncSpecItemCount(EItemSpecFunc eItemSpecFunc)
{
	INT32 nNumRet = 0;

	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		tagItem* pItem = Container::GetItem(i);
		if(!Container::IsOnePlaceFree(i) && pItem && pItem->pProtoType && pItem->pProtoType->eSpecFunc == eItemSpecFunc)
		{
			nNumRet += Container::GetItem(i)->n16Num;
		}
	}

	return nNumRet;
}

// ��ȡ������һ��ָ������������ĵ���
__forceinline tagItem* ItemContainer::GetOneSpecItemBySpecFunEnum(EItemSpecFunc eItemSpecFunc)
{
	for(INT16 i=0; i<Container::GetCurSpaceSize(); ++i)
	{
		tagItem* pItem = Container::GetItem(i);
		if(!Container::IsOnePlaceFree(i) && pItem && pItem->pProtoType && pItem->pProtoType->eSpecFunc == eItemSpecFunc)
		{
			return pItem;
		}
	}
	return NULL;
}

//----------------------------------------------------------------------
// ��ȡԼ����ָ��
//----------------------------------------------------------------------
inline ContainerRestrict* ItemContainer::GetRestrict() const
{
	return m_pRestrict;
}

//----------------------------------------------------------------------
// ��ȡ����������RoleID
//----------------------------------------------------------------------
inline DWORD ItemContainer::GetOwnerID() const
{
	return m_dwOwnerID;
}

//----------------------------------------------------------------------
// ���������п���λ�õ���С�±�
//----------------------------------------------------------------------
__forceinline VOID ItemContainer::ResetMinFreeIndex()
{
	if(GetFreeSpaceSize() <= 0)
	{
		m_n16MinFreeIndex = GT_INVALID;
		return;
	}

	while(m_n16MinFreeIndex < GetCurSpaceSize())
	{
		if(IsOnePlaceFree(m_n16MinFreeIndex))
		{
			return;
		}

		++m_n16MinFreeIndex;
	}

	// ����д���������������bug��������ѭ��
	if(GetCurSpaceSize() == m_n16MinFreeIndex)
	{
		m_n16MinFreeIndex = 0;
	}

	while(m_n16MinFreeIndex < GetCurSpaceSize())
	{
		if(IsOnePlaceFree(m_n16MinFreeIndex))
		{
			return;
		}

		++m_n16MinFreeIndex;
	}

	// ��Ҫ��ִ�е��˴���˵�������е�m_n16RemainSize������
	ASSERT(0);
}

//----------------------------------------------------------------------
// update
//----------------------------------------------------------------------
inline VOID ItemContainer::Update()
{
	m_TimeLimitMgr.Update();
}

//----------------------------------------------------------------------
// ��ȡ��ɾ���б�
//----------------------------------------------------------------------
inline TList<INT64>& ItemContainer::GetNeedDelList()
{
	return m_TimeLimitMgr.GetNeedDelList();
}

//----------------------------------------------------------------------
// ��մ�ɾ���б�
//----------------------------------------------------------------------
inline VOID ItemContainer::ClearNeedDelList()
{
	m_TimeLimitMgr.ClearNeedDelList();
}


//************************ װ��������ʵ�� ************************//

//----------------------------------------------------------------------
// update
//----------------------------------------------------------------------
inline VOID EquipContainer::Update()
{
	m_TimeLimitMgr.Update();
}

//----------------------------------------------------------------------
// ��ȡ��ɾ���б�
//----------------------------------------------------------------------
inline TList<INT64>& EquipContainer::GetNeedDelList()
{
	return m_TimeLimitMgr.GetNeedDelList();
}

//----------------------------------------------------------------------
// ��մ�ɾ���б�
//----------------------------------------------------------------------
inline VOID EquipContainer::ClearNeedDelList()
{
	m_TimeLimitMgr.ClearNeedDelList();
}

// Jason 2010-7-20 v2.3.1 ���ε�����£������ѱ���������һ��Ҫ��֤�����������Ҫ̫Ƶ������Ч�ʺܵ�
struct tagFindItemTypeEx
{
	bool operator () (const tagItem* pItem,DWORD dwTypeEx)
	{
		if(!P_VALID(pItem) || !P_VALID(pItem->pProtoType))
			return false;
		return pItem->pProtoType->eTypeEx == dwTypeEx;
	}
};

struct tagFindItemSpecFunc
{
	bool operator () (const tagItem* pItem,EItemSpecFunc eSpecFunc)
	{
		if(!P_VALID(pItem) || !P_VALID(pItem->pProtoType))
			return false;
		return pItem->pProtoType->eSpecFunc == eSpecFunc;
	}
};

template<typename T,typename V,typename F>
INT SearchPack( Container<T> & con,TList<T*> &list,V val, F f)
{
	INT re = 0;
	INT size = con.GetCurSpaceSize();
	for( INT16 i = 0; i < size ; ++i )
	{
		T * p = con.GetItem((INT16)i);
		if( P_VALID(p) && f(p,val) )
		{
			list.PushBack(p);
			re += p->n16Num;
		}
	}
	return re;
}
template<typename T,typename V,typename F>
INT SearchPackCount( Container<T> & con,V val, F f)
{
	INT re = 0;
	INT size = con.GetCurSpaceSize();
	for( INT16 i = 0; i < size ; ++i )
	{
		T * p = con.GetItem((INT16)i);
		if( P_VALID(p) && f(p,val) )
		{
			re += p->n16Num;
		}
	}
	return re;
}
template<typename T,typename Con,typename Mgr>
VOID RemoveFromContainer(Mgr & mgr,Con & con,TList<T*> & list,INT nNum,DWORD dwCmdID)
{
	INT n16NumDel = 0;
	T * pItem = NULL;
	INT32 nNumNeedDel = nNum;
	DWORD dwErrorCode;
	list.ResetIterator();
	while(list.PeekNext(pItem) && nNumNeedDel != 0)
	{
		if(pItem->n16Num <= nNumNeedDel)
		{
			n16NumDel = pItem->n16Num;
			dwErrorCode = mgr.DelFromContainer(con, pItem->n64Serial, dwCmdID);
			if(dwErrorCode != E_Success)
			{
				ASSERT(E_Success == dwErrorCode);
				n16NumDel = 0;
			}

			nNumNeedDel -= n16NumDel;
		}
		else
		{
			n16NumDel = nNumNeedDel;
			dwErrorCode = mgr.DelFromContainer(con, pItem->n64Serial,dwCmdID, nNumNeedDel);
			if(dwErrorCode != E_Success)
			{
				ASSERT(E_Success == dwErrorCode);
				n16NumDel = 0;
			}

			nNumNeedDel -= n16NumDel;
		}
	}
}


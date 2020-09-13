#pragma once
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\specfunc_define.h"
#include "GameClientDefine.h"

/** \���������Ʒ����
*/
struct tagUpdateLPItemEvent : public tagGameEvent
{
	EItemConType	eType;
	DWORD			dwTypeID;
	INT16			n16Index;

	tagUpdateLPItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = EICT_Null; n16Index = GT_INVALID;}
};

/** \�Ҽ���������е���Ʒ
*/
struct tagClickItemEvent : public tagGameEvent
{
	EItemConType	eType;
	INT16			n16Index;
	DWORD			dwTypeID;
    INT64			n64ItemID;
	tagClickItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = EICT_Null; n16Index = GT_INVALID; dwTypeID = GT_INVALID;n64ItemID = GT_INVALID;}
};

/** \��Ʒ��ͬ�������ƶ�
*/
struct tagItemPosChangeExEvent : public tagGameEvent
{
	EItemConType	eTypeSrc;
	EItemConType	eTypeDst;
	INT16			n16IndexSrc;
	DWORD			dwTypeIDSrc;

	tagItemPosChangeExEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eTypeSrc = eTypeDst = EICT_Null; n16IndexSrc = GT_INVALID; dwTypeIDSrc = GT_INVALID;}
};

/** \���⹦����Ʒʹ�õ�ѡ��ƷΪ�����������
*/
struct tagOnTheRoleOfItemEvent : public tagGameEvent
{
	EItemConType		eTypeSrc;
	EItemConType		eTypeDst;
	INT16				n16IndexSrc;
	INT16				n16IndexDst;
	DWORD				dwTypeIDSrc;
	DWORD				dwTypeIDDst;
	EItemSpecFunc		eSpecFuncSrc;

	tagOnTheRoleOfItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		eTypeSrc = eTypeDst = EICT_Null;
		n16IndexSrc = n16IndexDst = GT_INVALID;
		dwTypeIDSrc = dwTypeIDDst = GT_INVALID;
		eSpecFuncSrc = EISF_Null;
	}
};

struct tagSpecItemEvent : public tagGameEvent
{
	INT64				n64Serial;
	EItemSpecFunc		eSpecFuncSrc;

	tagSpecItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		n64Serial    = GT_INVALID;
		eSpecFuncSrc = EISF_Null;
	}
};


/** \������Ʒ
*/
struct tagPutDownItemEvent : public tagGameEvent
{
	EDragType	eType;
	DWORD		dwParam;
	DWORD		dwTypeID;
	
	tagPutDownItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \������Ʒ
*/
struct tagBuyItemEvent : public tagGameEvent
{
	INT16			n16Pos;
	INT16			n16Num;
	DWORD			dwTypeID;

	tagBuyItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

/** \���ӻ�ɾ����Ʒlog
*/
struct tagCombatMsgForItemChange : public tagGameEvent
{
	DWORD			dwTypeID;	// ��Ʒԭ��ID
	INT16			n16Num;		// ��Ʒ����
	INT16			nQuality;	// ��ƷƷ��
	

	tagCombatMsgForItemChange(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

/** \�������һ�ֿ�
*/
struct tagItemContainerExtend : public tagGameEvent
{
	bool bUseSilver;	// �Ƿ����Ľ�Ǯ���ǣ����Ľ�Ǯ��������Ԫ��
	bool bWalkWare;		// true ����ֿ⣬false �����ģ�ͨ��NPC���ʲֿ�
	tagItemContainerExtend(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent( szEventName, pSenderFrame ){}
};

/** \װ��������������淢�͵���Ϣ
*/
struct tagEquipDropEvent: public tagGameEvent
{
	INT64 n64ItemID;

	tagEquipDropEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame){}
};

/** \װ�����ѽ���
*/
struct tagBeautyFriendsEvent: public tagGameEvent
{
	DWORD dwTargetID;
	DWORD dwSuitIndex;
	tagBeautyFriendsEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame){}
};
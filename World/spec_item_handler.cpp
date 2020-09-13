//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: sepc_item_handler.cpp
// author: zr
// actor:
// data: 2010-01
// last:
// brief: ��Ʒϵͳ-������Ʒʵ�ֺ���
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "map.h"
#include "activity_mgr.h"
#include "GuildBattle.h"
#include "pet_pocket.h"
#include "pet_att.h"
#include "pet_soul.h"
#include "pet_define.h"
#include "../WorldDefine/filter.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_spec_item.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/msg_activity.h"
#include "../ServerDefine/msg_item.h"
#include "../WorldDefine/msg_GuildBattle.h"
#include "../WorldDefine/city_struggle_define.h"
#include "../WorldDefine/msg_pet.h"

static DWORD dwNormalMapIDS[] =
{
	// s01			s02				s03				s04
	3424072744,		3424073512,		3424073256,		3424071976,
	//s05			s06				s07				s08
	3424071720,		3424072488,		3424072232,		3424075048,
	//d01			d02				d03				d04
	3951677761,		3951678017,		3951678273,		3951678529,
	//d05			d06				d07				d08
	3951678785,		3951679041,		3951679297,		3951679553,
	//d09			d10				s02_1		
	3951679809,		3934900289,		881225009
};

static BOOL _IsNormalMap(DWORD dwMapID)
{
	static INT nNormalMapNum = sizeof(dwNormalMapIDS) / sizeof(dwNormalMapIDS[0]);
	for( INT i = 0; i < nNormalMapNum; ++i )
		if( dwNormalMapIDS[i] == dwMapID )
			return TRUE;
	return FALSE;
}


//-------------------------------------------------------------------------------------
// ��Ʒ��������
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleBindItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LockItem);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	// ʹ�õĵ��߿ɷ����ж�
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Item);
	if (!P_VALID(pItem) || ( pItem->pProtoType->eSpecFunc != EISF_ItemUnLock && pItem->pProtoType->eSpecFunc != EISF_ItemLock ))
	{
		dwErrorCode = E_ItemLock_HaveNotItem;
	}

	// ��ò�������
	tagItem* pTarget = pRole->GetItemMgr().GetBagItem(pRecv->n64Target);
	if (!P_VALID(pTarget))
	{
		dwErrorCode = E_ItemLock_TargetError;
	}

	if ( dwErrorCode != E_Success )
	{
		tagNS_LockItem ret;
		ret.dwErrorCode = dwErrorCode;
		SendMessage( &ret, ret.dwSize );

		return dwErrorCode;
	}

	// �����������Ƿ��Ǳ�ʯ����װ��
	if( !MIsEquipment(pTarget->dwTypeID) && pTarget->pProtoType->eSpecFunc != EISF_HoleGem )
	{
		tagNS_LockItem ret;
		ret.dwErrorCode = E_ItemLock_TargetInvalid;
		SendMessage( &ret, ret.dwSize );

		return E_ItemLock_TargetInvalid;
	}

	if ( pItem->pProtoType->eSpecFunc == EISF_ItemUnLock )	// �������
	{
		DWORD dwTmp = GetCurrentDWORDTime();
		DWORD dwUnlockTime = 0;

		if( !pTarget->bLock || ( pTarget->bLock  && pTarget->dwUnlockTime!=0 )) // �Ѿ����ڷ�����״̬
		{
			dwErrorCode = E_ItemLock_NoLocked;
			dwTmp = 0;
		}
		else // �ɽ��н���
		{
			dwUnlockTime = IncreaseTime((tagDWORDTime)dwTmp, 3 * 24 * 60 * 60);
			pTarget->dwUnlockTime = dwUnlockTime;

			pRole->GetItemMgr().Add2UnlockCDTimeMap(pTarget->n64Serial, dwUnlockTime);			
		}

		tagNDBC_LockItem send;
		send.n64Item = pTarget->n64Serial;
		send.byType = pTarget->bLock;
		send.dwUnlockTime = pTarget->dwUnlockTime;
		g_dbSession.Send(&send, send.dwSize);

		tagNS_LockItem ret;
		ret.dwErrorCode = dwErrorCode;
		ret.byType = 1;
		ret.dwUnlockTime = dwUnlockTime;
		ret.n64Serial = pTarget->n64Serial;
		SendMessage( &ret, ret.dwSize );
	} 
	else if( pItem->pProtoType->eSpecFunc == EISF_ItemLock)	// ��������
	{
		if( !pTarget->bLock )
		{
			pTarget->bLock = 1;
			pTarget->dwUnlockTime = 0;
		}
		else if( pTarget->bLock && pTarget->dwUnlockTime == 0) // �Ѵ�������״̬
		{
			dwErrorCode = E_ItemLock_HaveLocked;
		}
		else if(pTarget->dwUnlockTime != 0) // ��������ʱ��
		{
			pTarget->dwUnlockTime = 0;
		}

		tagNDBC_LockItem send;
		send.n64Item = pTarget->n64Serial;
		send.byType = pTarget->bLock;
		send.dwUnlockTime = pTarget->dwUnlockTime;
		g_dbSession.Send(&send, send.dwSize);

		tagNS_LockItem ret;
		ret.dwErrorCode = dwErrorCode;
		ret.byType = 0;
		ret.dwUnlockTime = 0;
		ret.n64Serial = pTarget->n64Serial;
		SendMessage( &ret, ret.dwSize );
	}
	else
	{
		dwErrorCode = E_ItemLock_OtherError;
	}
	
	// ʹ�ô�������ɾ������
	if ( E_Success == dwErrorCode )
	{
		pRole->GetItemMgr().DelFromBag(pItem->n64Serial, ELCLD_Item_Use, 1);
	}

	return dwErrorCode;
}


//-------------------------------------------------------------------------------------
// Ѱ�ٵ���-Ѱ��
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleTracePlayer(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_TrackPlayerPos);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	// ʹ�õĵ��߿ɷ����ж�
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Item);
	if (!P_VALID(pItem) || ( pItem->pProtoType->eSpecFunc != EISF_PlayerTrack ))
	{
		dwErrorCode = E_ItemTrack_HaveNotItem;
	}
	/*else if ( (pItem->nUseTimes == pItem->pProtoType->nMaxUseTimes-1) && ( pItem->n16Dummy == 2 || pItem->n16Dummy == 1 ))
	{
		dwErrorCode = E_ItemTrack_ItemUseTimeLimit;
	}*/

	if (pRole->IsHaveBuff(PLAYER_TRACK_LIMIT_BUFF))
	{
		dwErrorCode = E_ItemTrack_BuffLimit;
	}

	// ��鱻׷�ٽ�ɫ��״̬
	TCHAR buf[X_SHORT_NAME] = {0};
	_tcsncpy(buf, pRecv->szPlayerName, X_SHORT_NAME);
	buf[X_SHORT_NAME-1] = _T('\0');
	_tcslwr(buf);

	if(E_Success != dwErrorCode)
	{
		tagNS_TrackPlayerPos ret;
		ret.dwErrorCode = dwErrorCode;
		_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, buf );   // ׷��ʧ��
		SendMessage( &ret, ret.dwSize );

		return dwErrorCode;
	}

	DWORD dwTargetRoleID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(buf));
	Role* pTarget = g_roleMgr.GetRolePtrByID(dwTargetRoleID);
	if (!P_VALID(pTarget))
	{
		//Ҫ׷�ٵĽ�ɫ�����߻��߲�����
		dwErrorCode = E_ItemTrack_PlayerOffLine;

		tagNS_TrackPlayerPos ret;
		ret.dwErrorCode = dwErrorCode;
		_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, buf );   // ׷��ʧ��
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}
	
	tagNS_TrackPlayerPos ret;
	ret.dwErrorCode = dwErrorCode;
	_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, buf );
	ret.dwMapID = pTarget->GetMapID();
	ret.vPosition = pTarget->GetCurPos();
	SendMessage( &ret, ret.dwSize );

	// �۳�����ʹ�ô���
	if(pItem->pProtoType->nMaxUseTimes != GT_INVALID)
	{
		pRole->GetItemMgr().ItemUsedFromBag(pItem->n64Serial, 1, ELCLD_Item_Use);
	}

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
// Ѱ�ٵ���-����
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleMove2Role(tagNetCmd* pCmd)
{

	MGET_MSG(pRecv, pCmd, NC_Move2Player4Track);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	if (pRole->IsHaveBuff(PLAYER_TRACK_LIMIT_BUFF))
	{
		dwErrorCode = E_ItemTrack_BuffLimit;
	}

	// ʹ�õĵ��߿ɷ����ж�
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemTrack);
	if ( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_PlayerTrack || pItem->pProtoType->nSpecFuncVal1 != 1 )
	{
		dwErrorCode = E_ItemTrack_HaveNotItem;
	}
	/*else if ( (pItem->nUseTimes == pItem->pProtoType->nMaxUseTimes-1) && ( pItem->n16Dummy == 2 || pItem->n16Dummy == 1 ))
	{
		dwErrorCode = E_ItemTrack_ItemUseTimeLimit;
	}*/

	pRecv->szPlayerName[X_SHORT_NAME-1] = 0;
	if(E_Success != dwErrorCode)
	{
		tagNS_Move2Player4Track ret;
		ret.dwErrorCode = dwErrorCode;
		_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, pRecv->szPlayerName );
		SendMessage( &ret, ret.dwSize );

		return dwErrorCode;
	}
	
	// ����״̬�жϣ����̣���������̯��pk����ѣ����˯���������״̬�в��ɴ���
	if (pRole->IsInState(ES_Dead))
	{
		dwErrorCode = E_ItemTrack_DeadState;
	}
	else if(pRole->IsInState(ES_Dizzy))
	{
		dwErrorCode = E_ItemTrack_DizzyState;
	}
	else if (pRole->IsInState(ES_Spor))
	{
		dwErrorCode = E_ItemTrack_SporState;
	}
	else if (pRole->IsInState(ES_Tie))
	{
		dwErrorCode = E_ItemTrack_TieState;
	}
	else if (pRole->IsInRoleState(ERS_PK))
	{
		dwErrorCode = E_ItemTrack_PKState;
	}
	else if (pRole->IsInRoleState(ERS_Commerce))
	{
		dwErrorCode = E_ItemTrack_CommerceState;
	}
	else if (pRole->IsInRoleState(ERS_Stall))
	{
		dwErrorCode = E_ItemTrack_StallState;
	}
	else if (pRole->IsInRoleState(ERS_PrisonArea))
	{
		dwErrorCode = E_ItemTrack_PrisonAreaState;
	}
	else if (pRole->IsInRoleState(ERS_Mount))
	{
		dwErrorCode = E_ItemTrack_Mount;
	}
	else if (pRole->IsInState(ES_CSAttack) || pRole->IsInState(ES_CSDefence))
	{
		// ����а���,���Ҹð��������������У����ܴ�
		dwErrorCode = E_ItemTrack_ShenJi;
	}
	
	if (E_Success!=dwErrorCode)
	{
		tagNS_Move2Player4Track ret;
		ret.dwErrorCode = dwErrorCode;
		_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, pRecv->szPlayerName );
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}
	
	// ��鴫�͵�Ŀ�����
	TCHAR buf[X_SHORT_NAME] = {0};
	_tcsncpy(buf, pRecv->szPlayerName, X_SHORT_NAME);
	buf[X_SHORT_NAME-1] = _T('\0');
	_tcslwr(buf);

	DWORD dwTargetRoleID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(buf));
	Role* pTarget = g_roleMgr.GetRolePtrByID(dwTargetRoleID);
	if (!P_VALID(pTarget))
	{
		//Ҫ׷�ٵĽ�ɫ�����߻��߲�����
		dwErrorCode = E_ItemTrack_PlayerOffLine;
	}
	else
	{
		if (pTarget->IsInRoleState(ERS_PrisonArea))
		{
			dwErrorCode = E_ItemTrack_TargetPrisonAreaState;
		}
		else if (IsGuildBattleMap(pTarget->GetMapID()))
		{
			// ����а���,���Ҹð��������������У����ܴ�
			dwErrorCode = E_ItemTrack_ShenJi;
		}
	}

	// ���ʹ�ô��͵��ߣ������ű��¼����ᱦ�����ã�
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
	{
		pRoleScript->ForceLootChase(pRole);
	}

	if (E_Success != dwErrorCode)
	{
		tagNS_Move2Player4Track ret;
		ret.dwErrorCode = dwErrorCode;
		_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, pRecv->szPlayerName );
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}

	Map* pMap = pTarget->GetMap();
	//if ( P_VALID(pMap) && ( pMap->GetMapID() == m02ID || pMap->GetMapID() == m03ID || pMap->GetMapID() == m04ID || pMap->GetMapID() == m05ID ||
	//	pMap->GetMapID() == m06ID || pMap->GetMapID() == m061ID || pMap->GetMapID() == m07ID || pMap->GetMapID() == m20ID) ) // ֻ�е���������ͨ��ͼ��ʱ���Դ���
	if( P_VALID(pMap) && _IsNormalMap(pMap->GetMapID()) )
	{
		pRole->GotoNewMap(pTarget->GetMapID(), pTarget->GetCurPos().x, pTarget->GetCurPos().y, pTarget->GetCurPos().z);

		// ʹ�õĵ��������Ǻţ������Ѿ�ʹ���������˴��ͣ�����׷����ʾ׷��Ŀ���λ��δ��ɣ�
		
		//pItem->n16Dummy = 2; // 2��ʾ����׷����ʾ����
		
		// �۳�����ʹ�ô���
		if(pItem->pProtoType->nMaxUseTimes != GT_INVALID)
		{
			pRole->GetItemMgr().ItemUsedFromBag(pItem->n64Serial, 1, ELCLD_Item_Use);
		}
	}
	else
	{
		//	E_ItemTrack_XiulianChang,		//Ŀ�������������
		//	E_ItemTrack_Instance,			//Ŀ������ڸ���
		//	E_ItemTrack_JinCity,			//Ŀ������ڽ���

		if(pMap->GetInstanceID() != GT_INVALID)
		{
			dwErrorCode = E_ItemTrack_Instance;
		}
		else if ( pMap->GetNormalMapType() == ENMT_TrainingField )
		{
			dwErrorCode = E_ItemTrack_XiulianChang;
		}
		else if (IsCityStruggleMap(pMap->GetMapID()))
		{
			dwErrorCode = E_ItemTrack_JinCity;
		}
		else
			dwErrorCode = GT_INVALID;
	}
	
	tagNS_Move2Player4Track ret;
	ret.dwErrorCode = dwErrorCode;
	ret.dwPlayerID = pTarget->GetID();
	_tcscpy_s( ret.szPlayerName, X_SHORT_NAME, pRecv->szPlayerName );
	ret.dwMapID = pMap->GetMapID();
	ret.pos = pRole->GetCurPos();
	ret.faceTo = pRole->GetFaceTo();
	SendMessage( &ret, ret.dwSize );
	
	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
// Ѱ�ٵ���-׷����ʾ��׷����λ��
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleGetPlayerPos(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetPositionAfterTrack);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	if (pRole->IsHaveBuff(PLAYER_TRACK_LIMIT_BUFF))
	{
		tagNS_GetPositionAfterTrack ret;
		ret.dwErrorCode = E_ItemTrack_BuffLimit;
		SendMessage( &ret, ret.dwSize );
		return E_ItemTrack_BuffLimit;
	}

	// ��鱳�����Ƿ��кϷ�����
	/*tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Item);
	if ( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_PlayerTrack || pItem->pProtoType->nSpecFuncVal1 != 1 )
	{
		dwErrorCode = E_ItemTrack_HaveNotItem;

		tagNS_GetPositionAfterTrack ret;
		ret.dwErrorCode = dwErrorCode;
		SendMessage( &ret, ret.dwSize );

		return dwErrorCode;	
	}*/

	//pItem->n16Dummy--;

	Role* pTarget = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pTarget))
	{
		dwErrorCode = E_ItemTrack_PlayerOffLine;
	
		tagNS_GetPositionAfterTrack ret;
		ret.dwErrorCode = dwErrorCode;
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}
	
	tagNS_GetPositionAfterTrack ret;
	ret.dwErrorCode = dwErrorCode;
	g_roleMgr.GetRoleNameByID(pRecv->dwRoleID, ret.szPlayerName);
	ret.dwMapID = pTarget->GetMapID();
	ret.pos = pTarget->GetCurPos();
	SendMessage( &ret, ret.dwSize );

	return dwErrorCode;
}

DWORD PlayerSession::HandleEquipSignature(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_EquipSignature);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	pRecv->szContent[10] = 0; // ǩ�����10���ַ�

	// ��鱳�����Ƿ��кϷ�����
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Item);
	if ( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_EquipSignature )
	{
		dwErrorCode = E_EquipSig_HaveNoItem;

		tagNS_EquipSignature ret;
		ret.dwErrorCode = dwErrorCode;
		ret.n64EquipID = pRecv->n64EquipID;
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}

	// ���Ҫ����װ��
	tagItem* pTarget = pRole->GetItemMgr().GetBagItem(pRecv->n64EquipID);
	if (!P_VALID(pTarget))
	{
		dwErrorCode = E_EquipSig_NoTarget;
	}
	else
	{
		if (!MIsEquipment(pTarget->dwTypeID))
		{
			dwErrorCode = E_EquipSig_NotEquip;
		}
		else
		{
			// ���װ���Ƿ��Ѿ�������
			tagEquip* pEquip = reinterpret_cast<tagEquip*> (pTarget);
			if( pEquip->equipSpec.bySigned )
			{
				dwErrorCode = E_EquipSig_AlreadySig;
			}
		}
	}

	if (dwErrorCode != E_Success)
	{
		tagNS_EquipSignature ret;
		ret.dwErrorCode = dwErrorCode;
		ret.n64EquipID = pRecv->n64EquipID;
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}

	// �ж�ǩ�����ݵĺϷ���
	TCHAR szContent[11] = {0}; // ǩ�����10���ַ�
	_tcscpy_s(szContent, 11, pRecv->szContent);
	dwErrorCode = Filter::CheckName(szContent, 11, 1, g_attRes.GetNameFilterWords());
	if (dwErrorCode != E_Success)
	{
		tagNS_EquipSignature ret;
		ret.dwErrorCode = E_EquipSig_ContentLimit;
		ret.n64EquipID = pRecv->n64EquipID;
		SendMessage( &ret, ret.dwSize );
		return dwErrorCode;
	}

	// �Ե��߽�������
	tagEquip* pEquip = reinterpret_cast<tagEquip*> (pTarget);
	pEquip->Signature(szContent);
	pEquip->equipSpec.dwSignatruePlayerID = pRole->GetID();
	pEquip->equipSpec.bySigned = 1;
	if (pItem->pProtoType->nSpecFuncVal1)
	{
		pEquip->equipSpec.bySigned = 2;
		pEquip->byBind = EBS_Bind;

		tagNS_ItemBind msg;
		msg.eConType = pTarget->eConType;
		msg.n64Serial = pTarget->n64Serial;
		SendMessage(&msg, msg.dwSize);
	}
	
	// ֪ͨ���ݿⱣ��������Ϣ
	//tagNDBC_EquipSignature msg2db;
	//msg2db.n64Serial = pEquip->n64Serial;
	//msg2db.dwSignRoleID = pRole->GetID();
	//_tcscpy_s(msg2db.szContent, 10, szContent);
	//g_dbSession.Send(&msg2db, msg2db.dwSize);
	
	// ���ݿ���°�״̬
	tagNDBC_BindItem bind;
	bind.n64Item = pTarget->n64Serial;
	bind.byType = pTarget->byBind;
	g_dbSession.Send(&bind, bind.dwSize);

	// ���ݿ����
	pRole->GetItemMgr().UpdateEquipSpec(*pEquip);

	// ������Ʒ
	pRole->GetItemMgr().ItemUsedFromBag(pItem->n64Serial, 1, ELCLD_Item_Use);

	tagNS_EquipSignature ret;
	ret.dwErrorCode = dwErrorCode;
	ret.n64EquipID = pRecv->n64EquipID;
	SendMessage( &ret, ret.dwSize );

	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
// �õ������������״��
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleGetAllGodMiracle(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_AllGodMiracleInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	CActivity* pActive = g_activityMgr.GetActivity(pRecv->dwActiveID);
	if ( !P_VALID(pActive))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	dwErrorCode = pActive->SendAllGodMiracleToPlayer(static_cast<Unit*>(pRole));
	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
// ��ȡ�񼣱��� 
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleApplyCapGodMiracle(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApplyCapGodMiracle);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRecv->dwGodID >= EGM_MAX || pRecv->dwGodID < EGM_NH)
		return GT_INVALID;

	tagNS_ApplyCapGodMiracle pSend;
	DWORD dwErrorCode = E_Success;
	pSend.dwMoney = pRecv->dwMoney;
	dwErrorCode = CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->dwGodID)->HandleApplyCapGodMiracle(pRole->GetID(), pSend.dwMoney, pRecv->bAddBid);
	pSend.dwErrorCode = dwErrorCode;
	pSend.dwGodID = pRecv->dwGodID;
	pRole->SendMessage(&pSend, pSend.dwSize);
	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
//  ��ȡ�񼣵�������Ϣ
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleGetAllGodInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetAllGodInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;
	dwErrorCode = CGodMiracleMgr::GetInst()->HandleSendAllGodInfoToPlayer(pRole->GetID());
	return dwErrorCode;
}

//-------------------------------------------------------------------------------------
//  ��Ҫ���ýű���������Ʒ�Ĺ�������ӿ�
//-------------------------------------------------------------------------------------
DWORD PlayerSession::HandleSpecScriptItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SpecScriptItem);

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	DWORD dwErrorCode = E_Success;

	// ��鱳�����Ƿ��кϷ�����
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if ( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_LoadScript )
	{
		dwErrorCode = E_ScriptItem_ItemInvalid;

		tagNS_SpecScriptItem send;
		send.dwErrCode = dwErrorCode;
		SendMessage(&send,send.dwSize);

		return dwErrorCode;
	}

	// �ж�Ŀ���ɫ�Ƿ����ߣ��Ƿ�Ϸ���������ʹ����Ʒ�Ľ�ɫ����
	TCHAR buf[X_SHORT_NAME] = {0};
	_tcsncpy(buf, pRecv->szDesRoleName, X_SHORT_NAME);
	buf[X_SHORT_NAME-1] = _T('\0');
	_tcslwr(buf);

	//DWORD dwNameCrc = IUTIL->Crc32(buf);
	DWORD dwDesRoleID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(buf));
	if (dwDesRoleID == pRole->GetID())
	{
		dwErrorCode = E_ScriptItem_DesRoleError;
	}
	else
	{
		Role* pDesRole = g_roleMgr.GetRolePtrByID(dwDesRoleID);
		if(!P_VALID(pDesRole)) dwErrorCode = E_ScriptItem_DesRoleOffline;
	}

	if( dwErrorCode != E_Success)
	{
		tagNS_SpecScriptItem send;
		send.dwErrCode = dwErrorCode;
		SendMessage(&send,send.dwSize);

		return dwErrorCode;
	}

	// ��ʼ������Ʒ�Ķ�Ӧ�Ľű�����
	const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(pItem->dwTypeID);
	if(P_VALID(pItemScript))
	{
		// �ô����ж�Ϊ�˷�ֹ��û��ע��ű���������Ʒ��ʹ��ʱ����˿�
		//const ItemScript* pTmpScript = g_ScriptMgr.GetItemScript(pItem->dwTypeID);
		//if(!P_VALID(pTmpScript))
		//{
		//	dwErrorCode = E_ScriptItem_ScriptUnreg;

		//	tagNS_SpecScriptItem send;
		//	send.dwErrCode = dwErrorCode;
		//	SendMessage(&send,send.dwSize);

		//	return dwErrorCode;
		//}

		// ���ýű�
		// const ItemScript* pItemScript = pItem->pScript;
		BOOL bTmp = FALSE;
		INT nRet = pItemScript->CanUseItem(pRole->GetMap(), pItem->dwTypeID, pRole->GetID(), dwDesRoleID, bTmp);
		if (bTmp || nRet == E_Success)
		{
			pItemScript->UseItem(pRole->GetMap(), pItem->dwTypeID, pRole->GetID(), dwDesRoleID);

			// ������Ʒ
			pRole->GetItemMgr().ItemUsedFromBag(pItem->n64Serial, 1, ELCLD_Item_Use);
		}
		else
		{
			dwErrorCode = E_ScriptItem_UseLimit;
		}
	}
	else
	{
		dwErrorCode = E_ScriptItem_ScriptUnreg;
	}

	tagNS_SpecScriptItem send;
	send.dwErrCode = dwErrorCode;
	SendMessage(&send,send.dwSize);

	return dwErrorCode;

}

DWORD PlayerSession::HandleAddPetLiveTime(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_AddPetLiveTime);

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	DWORD dwErrorCode = E_Success;

	// ��鱳�����Ƿ��кϷ�����
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if ( !P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_PetFood || pItem->pProtoType->nSpecFuncVal1 != 2 )
	{
		dwErrorCode = E_PetLifeTime_ItemInvalid;

		tagNS_AddPetLiveTime send;
		send.dwErrCode = dwErrorCode;
		SendMessage(&send,send.dwSize);

		return dwErrorCode;
	}

	// �ҵ�Ҫ���������ĳ���
	PetPocket* pPetPocket = pRole->GetPetPocket();
	if (!P_VALID(pPetPocket))
	{
		dwErrorCode = E_PetLifeTime_SoulNotExist;
	}

	PetSoul* pSoul = pPetPocket->GetPetSoul(pRecv->dwPetID);
	if (!P_VALID(pSoul))
	{
		dwErrorCode = E_PetLifeTime_SoulNotExist;
	}
	else if ( GT_INVALID == pSoul->GetProto()->dwLifeTime )
	{
		dwErrorCode = E_PetLifeTime_NotTimeLimit;
	}

	if (E_Success != dwErrorCode)
	{
		tagNS_AddPetLiveTime send;
		send.dwErrCode = dwErrorCode;
		SendMessage(&send,send.dwSize);

		return dwErrorCode;
	}

	// ��������
	INT nAdd = pItem->pProtoType->nSpecFuncVal2;

	const tagPetProto* pProto = pSoul->GetProto();
	if (!P_VALID(pProto))
	{
		dwErrorCode = E_PetLifeTime_Failed;
	}
	
	if (E_Success != dwErrorCode)
	{
		tagNS_AddPetLiveTime send;
		send.dwErrCode = dwErrorCode;
		SendMessage(&send,send.dwSize);

		return dwErrorCode;
	}

	// ���ø�������ֵ
	INT nAddedLife = pSoul->GetAddedLife();	
	pSoul->SetAddedLife( nAddedLife + nAdd );
	
	tagDWORDTime dwNow = GetCurrentDWORDTime();
	if (!pSoul->GetLiveState())
	{
		// ����ó��������������Ѿ�������Ҫ���������ʱ��
		pSoul->ChangeBornDate( DecreaseTime(dwNow, (pProto->dwLifeTime)) );	
	}
	else
	{
		tagDWORDTime dwBornData = pSoul->GetBornDate();
		DWORD dwLifetime = pProto->dwLifeTime + pSoul->GetAddedLife();
		tagDWORDTime dwDeadLine = IncreaseTime(dwBornData, dwLifetime); 

		tagNS_PetLifeExtend send;
		send.dwPetID = pSoul->GetID();
		send.dwLife = CalcTimeDiff(dwDeadLine, dwNow);
		SendMessage(&send, send.dwSize);
	}

	// ������Ʒ
	pRole->GetItemMgr().ItemUsedFromBag(pItem->n64Serial, 1, ELCLD_Item_Use);

	tagNS_AddPetLiveTime send;
	send.dwErrCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

DWORD PlayerSession::HandleUseExpPilular(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseExpPilular);

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	DWORD dwErrorCode = E_Success;
	tagItem* pExpPilular = pRole->GetItemMgr().GetBagItem(pRecv->n64ExpPilularSerialID);
	if (!P_VALID(pExpPilular))
	{
		dwErrorCode = E_FabaoStoreExp_NoExpPililar;
		goto end;
	}

	const tagItemProto* pItemProto = pExpPilular->pProtoType;
	if (!P_VALID(pItemProto))
		return GT_INVALID;

	if (pItemProto->eSpecFunc != EISF_ExpPilular)
		return GT_INVALID;

	// ����ɫ���쾭�鵤ʹ�ô���
	INT16 nMaxExpPillularUseTimes = MaxExpPillularUseTimes(pRole->GetAttValue(ERA_VipLevel));
	if (pRole->GetExpPilularUseTimes() >= nMaxExpPillularUseTimes)
	{
		dwErrorCode = E_FabaoStoreExp_ExpPililarUseTimesLimit;
		goto end;
	}

	// ����ǲ���Լ����ˮ��֮��
	if (pExpPilular->dwSpecVal1 != 0 && pExpPilular->dwSpecVal1 != GT_INVALID && pExpPilular->dwSpecVal1 != pRole->GetID())
	{
		dwErrorCode = E_FabaoStoreExp_NotYourExpPililar;
		goto end;
	}

	pRole->IncExpPilularUseTimes();

	// ɾ�����鵤
	pRole->GetItemMgr().DelFromBag(pRecv->n64ExpPilularSerialID, ELCID_Fabao_StoreExp_Costed, 1);

	pRole->ExpChange((INT64)pItemProto->nSpecFuncVal1);

end:
	tagNS_UseExpPilular send;
	send.dwErrorCode = dwErrorCode;
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

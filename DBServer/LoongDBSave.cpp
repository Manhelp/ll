//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBSave.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ�� -- �����������
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/selectrole.h"
#include "../WorldDefine/vcard_define.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/item_define.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/msg_city.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/athletics_data_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../ServerDefine/msg_common.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/msg_quest_board.h"
#include "../WorldDefine/godbid_define.h"
#include "../ServerDefine/msg_godbid.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_fashion.h"
#include "../WorldDefine/mall_define.h"
#include "../ServerDefine/msg_holySoul.h"

#include "LoongDB.h"

//-------------------------------------------------------------------------------------------------------
// ɾ����Ʒװ����ض���
//-------------------------------------------------------------------------------------------------------
enum EDelItemTable
{
	EDIT_Item		= 0,	// item��
	EDIT_ItemBaiBao	= 1,	// item_baibao��
};

struct tagDelItem
{
	INT64	n64Serial;
	CHAR*	pTable;
};
struct tagDelItemFromRole
{
	DWORD dwRoleID;
	DWORD dwTypeID;
	CHAR* pTable;
};

struct tagNewItemEquip
{
	union
	{
		const tagItem*	pItem;
		const tagEquip* pEquip;
	};

	const CHAR*		pTable;
};


//#define MGET_TABLE(pTable, pDelItem)		\
//	char *pTable = "item";					\
//	if(EDIT_ItemBaiBao == pDelItem->eTable)	\
//		pTable = "item_baibao";

////-------------------------------------------------------------------------------------------------------
//// ɾ����ҵ�buff������item/equip��.�����������ǰ�����������ļ�¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::DeleteRecord(DWORD dwRoleID, tagRecordDel *pRecord)
//{
//	/*************************************************************************************
//	* ����ɾ��������ָ���ļ�¼: �����б�״̬�б���Ʒ�б�װ���б������б�
//	**************************************************************************************/
//	
//	BYTE *pData = pRecord->byData; // ��¼��ǰָ��λ��
//
//	// 1.�����б�
//	Update(dwRoleID, pData, pRecord->nSkillNum, sizeof(DWORD), (&CLoongDB::FormatDelRecSkill));
//	pData += pRecord->nSkillNum * sizeof(DWORD);
//
//	// 2.״̬�б�
//	Update(dwRoleID, pData, pRecord->nBuffNum, sizeof(DWORD), (&CLoongDB::FormatDelRecBuff));
//	pData += pRecord->nBuffNum * sizeof(DWORD);
//
//	//// 4.��Ʒ�б�
//	//Update(dwRoleID, pData, pRecord->nItemNum, sizeof(INT64), (&CLoongDB::FormatDelRecItem));
//	//pData += pRecord->nItemNum * sizeof(INT64);
//
//	//// 5.װ���б�
//	//Update(dwRoleID, pData, pRecord->nEquipNum, sizeof(INT64), (&CLoongDB::FormatDelRecEquip));
//	//pData += pRecord->nEquipNum * sizeof(INT64);
//
//	// 6.�����б�
//	Update(dwRoleID, pData, pRecord->nQuestNum, sizeof(DWORD), (&CLoongDB::FormatDelRecTask));
//	pData += pRecord->nQuestNum * sizeof(DWORD);
//}


//-------------------------------------------------------------------------------------------------------
// �洢��ɫ�����Ϣ -- ���輴ʱ�洢�ģ��������Ծ��ڴ˴���ʼ���洢����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveRole(DWORD dwRoleID, tagRoleDataSave *pRoleData)
{
	// �����ɫ����������Ϣ
	SaveRoleData(dwRoleID, pRoleData);

	/**************************************************************************
	* ���α���: �����б�״̬�б��ƺ��б���Ʒ�б�װ���б�
	*			�����б�����б������б�����������б� ��Ʒ��ȴʱ���
	*			
	***************************************************************************/

	LPVOID pData = pRoleData->byData; // ��¼��ǰָ��λ��

	// 1.�����б�
	SaveSkill(dwRoleID, pData, pRoleData->nSkillNum, &pData);

	// 2.״̬�б�
	SaveRoleBuff(dwRoleID, pData, pRoleData->nBuffNum, &pData);

	// 4.�ƺ��б�
	SaveTitle(dwRoleID, pData, pRoleData->nTitleNum, &pData);

	// 5.��Ʒ�б� 6.װ���б� // ����Ϣ����
	//SaveItemEquip(dwRoleID, pData, pRoleData->nItemNum, &pData);
	UpdateItemPosAndUseRel(dwRoleID, pData, pRoleData->nItemNum, &pData);

	// 7.�����б� 
	// 8.����б� // ��2�����е������Ǽ�ʱ�����

	// 9.�����б�
	SaveTask(dwRoleID, pData, pRoleData->nQuestNum, &pData);

	// 10.����������б�
	//SaveTaskDone(dwRoleID, pData, pRoleData->nQuestDoneNum);

	// 11.��Ʒ��ȴʱ���
	ReplaceItemCDTime(dwRoleID, pData, pRoleData->nItemCDTimeNum, &pData);

	// 12.������������
	SaveClanData(dwRoleID, pData, pRoleData->bClanDataChg, &pData);

	// 13.�������
	SavePets(dwRoleID, pData, pRoleData->nPetsNum, &pData);

	// 14.����ʦͽ
	SaveMaster(dwRoleID, pData, &pData);

	// ���������
// 	SaveBoardQuest(dwRoleID, pData, pRoleData->nBoardQuestNum, &pData);

	// 15.������ϵͳ����
	SaveArea(dwRoleID, pData, pRoleData->nAreaNum, &pData);

	// 16.�̳�������Ʒ�������
	SaveBuyMallItemCount(dwRoleID, pData, pRoleData->nBuyMallItemLogCount, &pData);


	// 17.Ԫ��
	//if ( pRoleData->bIsHolySoulActivated )
	//{
	//	SaveHolySoulInfo(pData,dwRoleID);
	//}
}


//********************************************************************************************************
//-------------------------------------------------------------------------------------------------------
// ��������Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertItem(const tagItem &item, const CHAR* pTable)
{
	tagNewItemEquip sNewItem;
	sNewItem.pItem	= &item;
	sNewItem.pTable	= pTable;
	return Update(GT_INVALID, (LPVOID)&sNewItem, 1, SIZE_ITEM, (&CLoongDB::FormatInsertItem));
}

//-------------------------------------------------------------------------------------------------------
// ������װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEquipSpec(const tagEquip &equip, const CHAR* pTable)
{
	tagNewItemEquip sNewItem;
	sNewItem.pEquip	= &equip;
	sNewItem.pTable	= pTable;
	return Update(GT_INVALID, (LPVOID)&sNewItem, 1, SIZE_EQUIP, (&CLoongDB::FormatInsertEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// ������װ�� -- װ��Ҫ����item���equip����Ҫ��֤���ݵ�������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEquip(const tagEquip &equip, BOOL bBaiBao/* = FALSE*/)
{
	CHAR* pTableItem	= "item";
	CHAR* pTableEquip	= "equip";

	if(bBaiBao)
	{
		pTableItem	= "item_baibao";
		pTableEquip = "equip_baibao";
	}
	
	BOOL bRes = InsertItem(equip, pTableItem);
	if(!bRes)
	{
		return bRes;
	}

	bRes = InsertEquipSpec(equip, pTableEquip);
	if(!bRes)
	{
		// equip�����ʧ�ܣ���item����ɾ����Ӧ��¼
		DelItem(equip.n64Serial, pTableItem);
	}

	return bRes;
}

// Jason 2010-4-14 v2.0.0
BOOL CLoongDB::InsertFabao(const tagFabao & fabao, BOOL bBaiBao )
{
	InsertEquip(fabao,bBaiBao);
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();

		InitFabaoUpdate(TRUE,pStream,pCon,&fabao);

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}
BOOL CLoongDB::SaveFabao(const tagFabao & fabao)
{
	if( SaveEquip(fabao) )
	{
		Beton::MyStream* pStream = m_pDBLoong->GetStream();
		if( P_VALID(pStream) )
		{
			Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
			if( !P_VALID(pCon) )
			{
				m_pDBLoong->ReturnStream(pStream);
				return FALSE;
			}

			BOOL r = TRUE;
			pStream->Clear();

			InitFabaoUpdate(FALSE,pStream,pCon,&fabao);

			pStream->FillString(" WHERE SerialNum=") << fabao.n64Serial;

			if( !m_pDBLoong->Execute(pStream) )
				r = FALSE;
			m_pDBLoong->ReturnStream(pStream);

			m_pDBLoong->ReturnConnection(pCon);
			return r;
		}
	}
	return FALSE;
}
BOOL CLoongDB::UpdateFabaoSpec(INT64 id,const tagFabaoSpec & fabao)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE fabao SET ") ;
		InitFabaoSpecUpdate(pStream,pCon,(tagFabaoSpec*)&fabao);
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

BOOL CLoongDB::InsertSoulCrystal(const tagSoulCrystal & soul, BOOL bBaiBao )
{
	InsertEquip(soul,bBaiBao);
	
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		
		pStream->Clear();
		pStream->SetInsert("soulcrystal");
		pStream->FillString("SerialNum=") << soul.n64Serial;
		pStream->FillString(",SoulLevel=") << soul.SoulSpec.bySoulCrystalLvl; 
		pStream->FillString(",SoulQlty=") << soul.SoulSpec.bySoulCrystalQlty;
		pStream->FillString(",SoulSkillID=") << soul.SoulSpec.dwStaticSkillID;
		for (int i=0; i<SOUL_CRYSTAL_ATT_MAX_NUM; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T(",SoulAttID%d="), i+1);
			pStream->FillString(szTmp)<<soul.SoulSpec.nSoulAttID[i];
		}

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

//BOOL SaveFabao(const tagFabao & fabao);
BOOL CLoongDB::UpdateSoulCrystalSpec(INT64 id,const tagSoulCrystalSpec & SoulSpec)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE soulcrystal SET ") ;
		pStream->FillString("SoulLevel=") << SoulSpec.bySoulCrystalLvl; 
		pStream->FillString(",SoulQlty=") << SoulSpec.bySoulCrystalQlty;
		pStream->FillString(",SoulSkillID=") << SoulSpec.dwStaticSkillID;
		for (int i=0; i<SOUL_CRYSTAL_ATT_MAX_NUM; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T(",SoulAttID%d="), i+1);
			pStream->FillString(szTmp)<<SoulSpec.nSoulAttID[i];
		}
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;

		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

// wcy 2012-03-31 ʥ�鱣��
BOOL CLoongDB::InsertHoly(const tagHolyMan & holy, BOOL bBaiBao)
{
	InsertEquip(holy,bBaiBao);
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		
		InitHolyUpdate(TRUE,pStream,pCon,&holy);


		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}


// wcy 2012-04-18 ʥ�Ʊ���
BOOL CLoongDB::InsertHolyEquip(const tagHolyEquip & holyEquip, BOOL bBaiBao )
{
	InsertEquip(holyEquip,bBaiBao);
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();

		InitHolyEquipUpdate(TRUE,pStream,pCon,&holyEquip);


		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

BOOL CLoongDB::UpdateHolySpec(INT64 id,const tagHolySpec & holy)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE holyman SET ") ;
		InitHolySpecUpdate(pStream,pCon,(tagHolySpec*)&holy);
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
	
}

BOOL CLoongDB::UpdateHolyEquipSpec(INT64 id,const tagHolyEquipSpec & holyEquip)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE holyequip SET ") ;
		InitHolyEquipSpecUpdate(pStream,pCon,(tagHolyEquipSpec*)&holyEquip);
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}
//-------------------------------------------------------------------------------------------------------
// ��Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveItem(const tagItem &item)
{
	return Update(GT_INVALID, (LPVOID)&item, 1, SIZE_ITEM, (&CLoongDB::FormatSaveItem));
}

//-------------------------------------------------------------------------------------------------------
// װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquipSpec(const tagEquip &equip)
{
	return Update(GT_INVALID, (LPVOID)&equip, 1, SIZE_EQUIP, (&CLoongDB::FormatSaveEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateEquipSpec(const tagEquipSpecUpdate &equipSpecUpdate)
{
	return Update(GT_INVALID, (LPVOID)&equipSpecUpdate, 1, sizeof(tagEquipSpecUpdate), (&CLoongDB::FormatUpdateEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// װ��ר�õ�����������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateEquipSingleAtt(const tagEquipSingleSpec &equipSingleSpecUpdate)
{
	return Update(GT_INVALID, (LPVOID)&equipSingleSpecUpdate, 1, sizeof(tagEquipSingleSpec), (&CLoongDB::FormatUpdateEquipSingleSpec));
}

//-------------------------------------------------------------------------------------------------------
// ����װ������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquipSignature(INT64 n64Serial, DWORD dwSignRoleID, TCHAR* szContent)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->SetUpdate("equip");
	pStream->FillString("Signature='").FillString(szContent).FillString("'");
	pStream->FillString(",SignRoleID=") << dwSignRoleID;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// װ�� -- װ��Ҫ����item���equip����Ҫ��֤���ݵ�������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquip(const tagEquip &equip)
{
	BOOL bRes = SaveItem(equip);
	if(!bRes)
	{
		return bRes;
	}

	bRes = SaveEquipSpec(equip);
	if(!bRes)
	{
		// equip�����ʧ�ܣ���item����ɾ����Ӧ��¼
		//DelItem(equip.n64Serial);//??
	}

	return bRes;
}

//-------------------------------------------------------------------------------------------------------
// ɾ����Ʒ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelItem(INT64 n64Serial, CHAR *pTable/* = "item"*/)
{
	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelItem));
}

//-------------------------------------------------------------------------------------------------------
// ��װ������ɾ��װ��ר������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquipSpec(INT64 n64Serial, CHAR *pTable/* = "equip"*/)
{
	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��װ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquip(INT64 n64Serial)
{
	tagDWORDTime now = GetCurrentDWORDTime();
	CHAR szTime[X_DATATIME_LEN + 1] = {0};
	if(!DwordTime2DataTime(szTime, sizeof(szTime), now))
	{
		return FALSE;
	}

	if( !(UpdateDelEquipTime(n64Serial, szTime) && UpdateDelItemTime(n64Serial, szTime)) )
		return FALSE;

	if ( !(SaveDelEquipInfo(n64Serial) && SaveDelItemInfo(n64Serial)) )
		return FALSE;	

	if(DelItem(n64Serial))
	{
		return DelEquipSpec(n64Serial);
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ��ɾ������ָ����ɫ��ָ��������ָ������װ��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquipFromRole(DWORD dwRoleID, DWORD dwTypeID)
{
	tagDWORDTime now = GetCurrentDWORDTime();
	CHAR szTime[X_DATATIME_LEN + 1] = {0};
	if(!DwordTime2DataTime(szTime, sizeof(szTime), now))
	{
		return FALSE;
	}

	if( !(UpdateDelEquipTime(dwRoleID, dwTypeID, szTime) && UpdateDelItemTime(dwRoleID, dwTypeID, szTime)) )
		return FALSE;

	if ( !(SaveDelEquipInfo(dwRoleID, dwTypeID) && SaveDelItemInfo(dwRoleID, dwTypeID)) )
		return FALSE;	

	if(DelItem(dwRoleID, dwTypeID))
	{
		return DelEquipSpec(dwRoleID, dwTypeID);
	}

	return FALSE;
}

INT64 CLoongDB::GetDelEquip64Serial(DWORD dwRoleID, DWORD dwTypeID)
{
	INT64 n64Serial = 0;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	pStream->FillString("select SerialNum from item where TypeID=") << dwTypeID;
	pStream->FillString(" and OwnerID=") << dwRoleID;
	pStream->SetLimit(1);

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		// �ͷ���
		m_pDBLoong->ReturnStream(pStream);
		return 0;
	}

	// ���鲢�����ѯ���
	if (pResult->GetRowCount() > 0)
	{
		n64Serial = (*pResult)[0].GetLong();
	}
	else
	{
		n64Serial = 0;
	}

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return n64Serial;
}

BOOL CLoongDB::SaveDelEquipInfo(DWORD dwRoleID, DWORD dwTypeID)
{
	INT64 n64Serial = GetDelEquip64Serial(dwRoleID, dwTypeID);
	if (n64Serial == 0)
	{
		return FALSE;
	}

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// ɾ������
	pStream->SetCopyItem("equip_del", "*", "equip");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
BOOL CLoongDB::SaveDelItemInfo(DWORD dwRoleID, DWORD dwTypeID)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// ɾ������
	pStream->SetCopyItem("item_del", "*", "item");
	pStream->SetWhere();
	pStream->FillString("TypeID=") << dwTypeID;
	pStream->FillString(" and OwnerID=") << dwRoleID;
	pStream->SetLimit(1);

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
BOOL CLoongDB::DelItem(DWORD dwRoleID, DWORD dwTypeID, CHAR *pTable/* = "item"*/)
{
	tagDelItemFromRole delItem;
	delItem.dwRoleID	= dwRoleID;
	delItem.dwTypeID	= dwTypeID;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelItemFromRole));
}
BOOL CLoongDB::DelEquipSpec(DWORD dwRoleID, DWORD dwTypeID, CHAR *pTable/* = "equip"*/)
{
	INT64 n64Serial = GetDelEquip64Serial(dwRoleID, dwTypeID);
	if (n64Serial == 0)
	{
		return FALSE;
	}

	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelEquipSpec));
}

VOID CLoongDB::FormatDelItemFromRole(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItemFromRole);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("TypeID=") << p->dwTypeID;
	pStream->FillString(" and OwnerID=") << p->dwRoleID;
	pStream->SetLimit(1);
}
VOID CLoongDB::FormatDelEquipSpecFromRole(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItemFromRole);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("TypeID=") << p->dwTypeID;
	pStream->FillString(" and OwnerID=") << p->dwRoleID;
	pStream->SetLimit(1);
}

BOOL CLoongDB::SaveDelEquipInfo(INT64 n64Serial)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ɾ������
	pStream->SetCopyItem("equip_del", "*", "equip");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SaveDelItemInfo(INT64 n64Serial)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ɾ������
	pStream->SetCopyItem("item_del", "*", "item");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// ��ɾ��ʱ��
// 	INT64 n64SerialSet[1] = {n64Serial};
// 	bRet = bRet && LogItemDelTime(pStream, "item_del", n64SerialSet, 1);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleData(DWORD dwRoleID, const tagRoleDataSave *pRoleData)
{
	return Update(dwRoleID, (LPVOID)pRoleData, 1, GT_INVALID, (&CLoongDB::FormatSaveRoleData));
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleInfo(DWORD dwRoleID, const tagRoleInfo *pRoleInfo)
{
	return Update(dwRoleID, (LPVOID)pRoleInfo, 1, GT_INVALID, (&CLoongDB::FormatSaveRoleInfo));
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveClanData(DWORD dwRoleID, IN LPVOID pData, BOOL bChange, OUT LPVOID* ppOutPointer)
{
	if (!bChange)
	{
		*ppOutPointer = pData;
		return TRUE;
	}
	*ppOutPointer = reinterpret_cast<BYTE*>(pData) + sizeof(tagDBReputeData);
	return Update(dwRoleID, pData, 1, sizeof(tagDBReputeData), (&CLoongDB::FormatUpdateClanData));
}


//-------------------------------------------------------------------------------------------------------
// ������Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
//BOOL CLoongDB::SaveItemEquip(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
//{
//	tagItem *pItem;
//	BYTE	*pCurPointer = (BYTE*)pData;
//	for(INT32 i=0; i<nNum; ++i)
//	{
//		pItem = (tagItem*)pCurPointer;
//		if(MIsEquipment(pItem->dwTypeID))
//		{
//			SaveEquip(*(tagEquip*)pItem);
//			pCurPointer += SIZE_EQUIP;
//		}
//		else
//		{
//			SaveItem(*pItem);
//			pCurPointer += SIZE_ITEM;
//		}
//	}
//
//	*ppOutPointer = pCurPointer;
//
//	return TRUE;
//}

//-------------------------------------------------------------------------------------------------------
// ������Ʒλ�ü�ʹ���������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateItemPosAndUseRel(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagItemUpdate), (&CLoongDB::FormatUpdateItemPosAndUseRel), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertSkill(DWORD dwRoleID, tagSkillSave *pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagSkillSave), (&CLoongDB::FormatInsertSkill));
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelSkill(DWORD dwRoleID, DWORD dwSkillID)
{
	return Update(dwRoleID, &dwSkillID, 1, sizeof(DWORD), (&CLoongDB::FormatDelSkill));
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSkill(DWORD dwRoleID, tagSkillSave *pData)
{
	LPVOID pSkillData = pData;
	return SaveSkill(dwRoleID, pSkillData, 1, &pSkillData);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveSkill(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagSkillSave), (&CLoongDB::FormatSaveSkill), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫbuff��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleBuff(DWORD dwRoleID, LPVOID pBuff, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	// ��ɾ��buff
	Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelAllRoleBuff));

	BYTE		*pCurPointer = (BYTE*)pBuff;
	tagBuffSave *pBuffSave;
	for(INT32 i=0; i<nNum; ++i)
	{
		pBuffSave = (tagBuffSave *)pCurPointer;
		Update(dwRoleID, pBuffSave, 1, sizeof(tagBuffSave), (&CLoongDB::FormatSaveRoleBuff));
		pCurPointer = pBuffSave->byData + pBuffSave->n8ModifierNum * sizeof(DWORD);
	}

	*ppOutPointer = pCurPointer;
	
	return TRUE;
}

//Ԫ��
VOID CLoongDB::SaveHolySoulInfo(LPVOID& pHolySoulInfo, DWORD dwID)
{
	if ( UpdateHolySoulInfo(pHolySoulInfo,dwID) )
	{
		pHolySoulInfo = ((BYTE*)pHolySoulInfo + sizeof(tagHolySoulInfoEx));
	}
}

BOOL CLoongDB::LoadHolySoulInfo(DWORD dwID,LPVOID& pData)
{
	return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadHolySoulInfo, &CLoongDB::ProcResLoadHolySoulInfo);  
}

BOOL CLoongDB::InsertHolySoulInfo(LPVOID pHolySoulInfo, DWORD dwID)
{
	return Update(dwID, pHolySoulInfo, 1, sizeof(tagHolySoulInfoEx), (&CLoongDB::FormatInsertHolySoulInfo));
}

BOOL CLoongDB::UpdateHolySoulInfo(LPVOID pHolySoulInfo, DWORD dwID)
{
	return Update(dwID, pHolySoulInfo, 1, sizeof(tagHolySoulInfoEx), (&CLoongDB::FormatUpdateHolySoulInfo));
}

BOOL CLoongDB::UpdateHolySoulExpAtt(LPVOID pData, DWORD dwID)
{
	return Update(dwID, pData, 1, sizeof(tagNDBC_UpdateHolySoulExpAtt), (&CLoongDB::FormatUpdateHolySoulExpAtt));
}

VOID CLoongDB::FormatLoadHolySoulInfo(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("HolySoulInfo", "SoulID,CurLevelExp,CurSoulValue,CurCovalue,HolySoulState,RebornLv,SoulLevel,Demage,Hit,Crit,CritAmount,Exattack,Inattack,Accrate,Exdamage,Toughness,Morale");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwID;
}

VOID CLoongDB::ProcResLoadHolySoulInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pHolySoulInfo, pData, tagHolySoulInfoEx);
	if (pRes->GetRowCount() <= 0)
	{
		return;
	}
	pHolySoulInfo->dwSoulID																	= (*pRes)[0].GetDword();
	pHolySoulInfo->n64CurLevelExp															= (*pRes)[1].GetLong();
	pHolySoulInfo->nCurSoulValue															= (*pRes)[2].GetDword();
	pHolySoulInfo->nCurCovalue																= (*pRes)[3].GetDword();
	pHolySoulInfo->eHolySoulState															= (EHolySoulState)((*pRes)[4].GetDword());
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_RebornLv]										= (*pRes)[5].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_SoulLevel]										= (*pRes)[6].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Demage]										= (*pRes)[7].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Hit]											= (*pRes)[8].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Crit]											= (*pRes)[9].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_CritAmount]									= (*pRes)[10].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Exattack]										= (*pRes)[11].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Inattack]										= (*pRes)[12].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Accrate]										= (*pRes)[13].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Exdamage]										= (*pRes)[14].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Toughness]										= (*pRes)[15].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Morale]										= (*pRes)[16].GetDword();

	pData =  ((BYTE*)pData + sizeof(tagHolySoulInfoEx)); 
}


VOID CLoongDB::FormatInsertHolySoulInfo(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{  
	MTRANS_POINTER(p, pData, tagHolySoulInfoEx); 

	pStream->Clear();
	pStream->SetInsert("HolySoulInfo");
	pStream->FillString("SoulID=") << p->dwSoulID;
	pStream->FillString(",RoleID=") << dwID;
	pStream->FillString(",CurLevelExp=") << p->n64CurLevelExp;
	pStream->FillString(",CurSoulValue=") << p->nCurSoulValue;
	pStream->FillString(",CurCovalue=") << p->nCurCovalue;
	pStream->FillString(",HolySoulState=") << p->eHolySoulState;
// 	pStream->FillString(",RebornLv=") << p->HolySoulAtt.nSoulAtt[ESAT_RebornLv];
// 	pStream->FillString(",SoulLevel=") << p->HolySoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	pStream->FillString(",Demage=") << p->HolySoulAtt.nSoulAtt[ESAT_Demage];
// 	pStream->FillString(",Hit=") << p->HolySoulAtt.nSoulAtt[ESAT_Hit];
// 	pStream->FillString(",Crit=") << p->HolySoulAtt.nSoulAtt[ESAT_Crit];
// 	pStream->FillString(",CritAmount=") << p->HolySoulAtt.nSoulAtt[ESAT_CritAmount];
// 	pStream->FillString(",Exattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Exattack];
// 	pStream->FillString(",Inattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Inattack];
// 	pStream->FillString(",Accrate=") << p->HolySoulAtt.nSoulAtt[ESAT_Accrate];
// 	pStream->FillString(",Exdamage=") << p->HolySoulAtt.nSoulAtt[ESAT_Exdamage];
// 	pStream->FillString(",Toughness=") << p->HolySoulAtt.nSoulAtt[ESAT_Toughness];
// 	pStream->FillString(",Morale=") << p->HolySoulAtt.nSoulAtt[ESAT_Morale];


}

VOID CLoongDB::FormatUpdateHolySoulInfo(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{

	MTRANS_POINTER(p, pData, tagHolySoulInfoEx); 

	pStream->Clear();
	pStream->SetUpdate("HolySoulInfo");
	pStream->FillString("SoulID=") << p->dwSoulID;
	pStream->FillString(",CurLevelExp=") << p->n64CurLevelExp;
	pStream->FillString(",CurSoulValue=") << p->nCurSoulValue;
	pStream->FillString(",CurCovalue=") << p->nCurCovalue;
	pStream->FillString(",HolySoulState=") << p->eHolySoulState;
// 	pStream->FillString(",RebornLv=") << p->HolySoulAtt.nSoulAtt[ESAT_RebornLv];
// 	pStream->FillString(",SoulLevel=") << p->HolySoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	pStream->FillString(",Demage=") << p->HolySoulAtt.nSoulAtt[ESAT_Demage];
// 	pStream->FillString(",Hit=") << p->HolySoulAtt.nSoulAtt[ESAT_Hit];
// 	pStream->FillString(",Crit=") << p->HolySoulAtt.nSoulAtt[ESAT_Crit];
// 	pStream->FillString(",CritAmount=") << p->HolySoulAtt.nSoulAtt[ESAT_CritAmount];
// 	pStream->FillString(",Exattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Exattack];
// 	pStream->FillString(",Inattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Inattack];
// 	pStream->FillString(",Accrate=") << p->HolySoulAtt.nSoulAtt[ESAT_Accrate];
// 	pStream->FillString(",Exdamage=") << p->HolySoulAtt.nSoulAtt[ESAT_Exdamage];
// 	pStream->FillString(",Toughness=") << p->HolySoulAtt.nSoulAtt[ESAT_Toughness];
// 	pStream->FillString(",Morale=") << p->HolySoulAtt.nSoulAtt[ESAT_Morale];


	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwID;
}

VOID CLoongDB::FormatUpdateHolySoulExpAtt(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_UpdateHolySoulExpAtt); 

	pStream->Clear();
	pStream->SetUpdate("HolySoulInfo");
	pStream->FillString("CurLevelExp=") << p->n64Exp;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << p->dwRoleID;	
}

BOOL CLoongDB::UpdateHolySoulAttInfo(LPVOID pData, DWORD dwID)
{
	return Update(dwID, pData, 1, sizeof(tagNDBC_UpdateHolySoulAttInfo), (&CLoongDB::FormatUpdateHolySoulAttInfo));
}

VOID CLoongDB::FormatUpdateHolySoulAttInfo(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_UpdateHolySoulAttInfo); 

	pStream->Clear();
	pStream->SetUpdate("HolySoulInfo");

	if ( ESoulAttEx_SoulVal == p->eSoulAttTypeEx )
	{
		pStream->FillString("CurSoulValue=") << p->dwAttVal;
	}
	else if ( ESoulAttEx_Covalue == p->eSoulAttTypeEx )
	{
		pStream->FillString("CurCovalue=") << p->dwAttVal;
	}


	pStream->SetWhere();
	pStream->FillString("RoleID=") << p->dwRoleID;	

}

//-------------------------------------------------------------------------------------------------------
// ����ƺ���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTitle(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagTitleSave), (&CLoongDB::FormatSaveTitle), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertTask(DWORD dwRoleID, LPVOID pAcceptQuest)
{
	return Update(dwRoleID, pAcceptQuest, 1, sizeof(tagAcceptQuest), (&CLoongDB::FormatInsertTask));
}


//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTask(DWORD dwRoleID, LPVOID pTask, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pTask, nNum, sizeof(tagQuestSave), (&CLoongDB::FormatSaveTask), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// ���������������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTaskDone(DWORD dwRoleID, LPVOID pTask, INT32 nNum)
{
	return Update(dwRoleID, pTask, nNum, sizeof(tagQuestDoneSave), (&CLoongDB::FormatSaveTaskDone));
}

//-------------------------------------------------------------------------------------------------------
// ����������ճ�������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveDailyTaskDone(DWORD dwRoleID, LPVOID pTask, INT32 nNum)
{ 
    return Update(dwRoleID, pTask, nNum, sizeof(tagQuestDoneSave), (&CLoongDB::FormatSaveDailyTaskDone));
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertVCard(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertVCard));
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateVCard(DWORD dwRoleID, const PVOID pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagVCardData), (&CLoongDB::FormatUpdateVCard));
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelVCard(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelVCard));
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertClanData(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertClanData));
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
// BOOL CLoongDB::UpdateClanData(DWORD dwRoleID, const PVOID pData)
// {
// 	return Update(dwRoleID, pData, 1, sizeof(tagDBReputeData), (&CLoongDB::FormatUpdateClanData));
// }

//-------------------------------------------------------------------------------------------------------
// ɾ��������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelClanData(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelClanData));
}

BOOL CLoongDB::SaveActivityData(DWORD dwID, LPVOID pActivityData)
{
	return Update(dwID, pActivityData, 1, sizeof(tagActivityData), (&CLoongDB::FormatSaveActivity));
}

//-------------------------------------------------------------------------------------------------------
// ������Ϣ����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::CreateCityInfo(INT nCityNum, LPVOID pData)
{
	// ������ѯ�͸��µĲ�������Ҫ����
	LockTables("city");

	for (int n=0; n<nCityNum; n++)
	{
		BOOL bCreate = FALSE;

		// ��ȡ��
		Beton::MyStream* pStream = m_pDBLoong->GetStream();
		ASSERT(P_VALID(pStream));

		// ��ʽ����ѯ�Ľ�ɫ����
		pStream->Clear();
		pStream->SetSelect("city", "id");
		pStream->SetWhere();
		pStream->FillString("id=") << ((DWORD*)pData)[n];

		// ��ѯ���ݿ�
		Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
		if(!P_VALID(pResult))
		{
			bCreate = TRUE;
		}
		else if (pResult->GetRowCount() <= 0)
		{
			bCreate = TRUE;
		}
		++m_dwReadTimes;

		// �ͷ����ͽ��
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->FreeQueryResult(pResult);

		// ����������Ϣ
		if (bCreate)
		{
			Update(((DWORD*)pData)[n], NULL, 1, 0, &CLoongDB::FormatCreateCityInfo);
		}
	}

	// ����
	UnlockTables();

	return TRUE;
}

BOOL CLoongDB::SaveCityInfo(LPVOID pData)
{
	return Update(GT_INVALID, pData, 1, 0, &CLoongDB::FormatSaveCityInfo);
}

BOOL CLoongDB::UpdateCityInfo(DWORD dwCityID, LPVOID pData)
{
	return Update(dwCityID, pData, 1, 0, &CLoongDB::FormatUpdateCityInfo);
}

VOID CLoongDB::SaveMaster(DWORD dwRoleID, IN LPVOID pData, OUT LPVOID* ppOutPointer)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	pData = (BYTE*)pData + sizeof(tagMasterApprentice2DB);
	*ppOutPointer = pData;
}

BOOL CLoongDB::SaveAllMasterApprenticeData(LPVOID pData)
{
	return Update(GT_INVALID, pData, 1, 0, &CLoongDB::FormatSaveAllMasterApprenticeData);
}

BOOL CLoongDB::SaveOneMasterApprenticeData(DWORD dwID, LPVOID pData)
{
	tagOneMASaveData tmpData;
	MTRANS_POINTER(pLoadData, &tmpData.loadData, void);
	BOOL bRet = Load(pLoadData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadMasterApprenticeData, &CLoongDB::ProcResLoadMasterApprenticeData);

	if (!bRet)
	{
		return FALSE;
	}

	// ��pData��ֵ��tmpDataȻ�󴫽�FormatSaveOneMasterApprenticeData
	MTRANS_POINTER(pOneMAData, pData, tagOneMAData2DB);
	tmpData.oneMAData = *pOneMAData;				

	MTRANS_POINTER(pSaveData, &tmpData, void);
	return Update(dwID, pSaveData, 1, 0, &CLoongDB::FormatSaveOneMasterApprenticeData);
}

BOOL CLoongDB::InsertMasterApprenticeData(DWORD dwID, LPVOID pData)
{
	
	//LockTables("master");

	BOOL bInsert = FALSE;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ����ѯ�Ľ�ɫ����
	pStream->Clear();
	pStream->SetSelect("master", "dwID");
	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bInsert = TRUE;
	}
	else if (pResult->GetRowCount() <= 0)
	{
		bInsert = TRUE;
	}
	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	// ����ʦͽ��Ϣ
	if (bInsert)
	{
		Update( dwID, pData, 1, 0, &CLoongDB::FormatInsertMasterApprenticeData );
	}


	// ����
	//UnlockTables();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��һ��ͽ����Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteAnApprentice(DWORD dwID, DWORD dwApprenticeID)
{
	//LockTables("master");

	BOOL bDelete = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ����ѯ�Ľ�ɫ����
	pStream->Clear();
	pStream->SetSelect("master", "dwID,JingWuPoint,QinWuPoint,LevelUpBonus,RewardNum,ApprenticeData");
	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bDelete = FALSE;
	}
	else if (pResult->GetRowCount() <= 0)
	{
		bDelete = FALSE;
	}
	++m_dwReadTimes;
	
	// ɾ��һ��ͽ����Ϣ
	if (bDelete)
	{
		tagMasterApprentice2DB appData;
		appData.dwID			=	(*pResult)[0].GetInt();
		appData.nJingWuPoint	=	(*pResult)[1].GetInt();
		appData.nQinWuPoint		=	(*pResult)[2].GetInt();
		appData.nLevelUpBonus	=	(*pResult)[3].GetInt();
		appData.nRewardNum		=	(*pResult)[4].GetInt();
		(*pResult)[5].GetBlob(appData.sApprentice.dwData, (*pResult)[5].GetLen());

		// ��ȥ��ͽ�ܼ�¼
		int nSize = sizeof(appData.sApprentice.dwData) / sizeof(DWORD);
		DWORD	dwTmp[MAX_APPRENTICE_NUM];
		ZeroMemory(dwTmp, sizeof(dwTmp));
		int n = 0;
		for (int i=0; i!=nSize; ++i)
		{
			if (dwApprenticeID == appData.sApprentice.dwData[i])
			{
				continue;
			}
			dwTmp[n++] = appData.sApprentice.dwData[i];
		}

		memcpy(appData.sApprentice.dwData, dwTmp, sizeof(dwTmp));

		Update( dwID, &appData, 1, 0, &CLoongDB::FormatSaveAllMasterApprenticeData );
	}


	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	// ����
	//UnlockTables();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveArenaData(DWORD dwSerialID,LPVOID pData)
{
	return Update(dwSerialID, pData, 1, 0, &CLoongDB::FormatSaveArenaData);
	
}

BOOL  CLoongDB::SaveSerialVal(DWORD dwSerialID,LPVOID pData)
{
    return Update(dwSerialID, pData, 1, 0, &CLoongDB::FormatSaveSerialVal);
}

VOID CLoongDB::FormatSaveSerialVal(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveSerialValToDB);
	pStream->Clear();
	pStream->SetUpdate("arena_data");
	pStream->FillString("serial_val=") << p->dwSerialVal;
	pStream->SetWhere();
	pStream->FillString("serial_id=") << p->dwSerialId;
}
VOID CLoongDB::FormatSaveArenaData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{

	MTRANS_POINTER(p, pData, tagNDBC_SaveArenaDataToDB);
    pStream->Clear();
	pStream->SetReplace("arena_data");
    
	pStream->FillString("serial_id=") << p->tPvPSystemData.dwSerialId;
	pStream->FillString(",system_id=") << p->tPvPSystemInfo.n16SystemId;
	pStream->FillString(",serial_val=") << p->tPvPSystemInfo.dwSerialVal;
    CHAR cTimeRange[32];
	DwordTime2DataTime(cTimeRange, sizeof(cTimeRange), p->tPvPSystemData.saveTime);
	pStream->FillString(",time_range='").FillString(cTimeRange);

	
	pStream->FillString("'");
	pStream->FillString(",hall_persom_num=") << p->tPvPSystemData.dwHallPersonNum;

	pStream->FillString(",hall_room_num=") << p->tPvPSystemData.dwHallRoomNum;
	pStream->FillString(",fighting_room_num=") << p->tPvPSystemData.dwFightRoomNum;
	pStream->FillString(",below120Second=") << p->tPvPSystemData.n16Below120Sec;
	pStream->FillString(",below210Second=") << p->tPvPSystemData.n16Below210Sec;
    pStream->FillString(",below300Second=") << p->tPvPSystemData.n16Below300Sec;
	pStream->FillString(",over300Second=") << p->tPvPSystemData.n16Over300Sec;
	
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveShortCut_QuickBar(DWORD dwRoleID,LPVOID pData)
{
	return Update(dwRoleID, pData, 1, 0, &CLoongDB::FormatSaveShortCut_QuickBar);
}

//-------------------------------------------------------------------------------------------------------
// ��������SQL��ʽ�����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveShortCut_QuickBar(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{  
	
	MTRANS_POINTER(p, pData, tagNDBC_SaveShortCut_QuickBar);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetReplace("shortcut_quickbar");
	pStream->FillString("RoleID=") << dwID;
	pStream->FillString(",quickbardata='").FillBlob(p->QuickBarData, sizeof(tagQuickData)*QUICK_BAR_TOTALCOUNT, pCon);
	pStream->FillString("'");

	m_pDBLoong->ReturnConnection(pCon);

}

//-------------------------------------------------------------------------------------------------------
// ѡ�������Ϣ
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// ���汨��ѡ�����Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveShowPlayerData(DWORD dwID,LPVOID pData)
{
	return Update(dwID, pData,1, 0, &CLoongDB::FormatSaveShowPlayerData);
}


//-------------------------------------------------------------------------------------------------------
// ����ѡ����ѡ�ֵ÷���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveShowPlayerPoint(DWORD dwID,LPVOID pData)
{
    return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveShowPlayerPoint);
}

//-------------------------------------------------------------------------------------------------------
// ����ͶƱ�˵÷���Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveVotePlayerPoint(DWORD dwID,LPVOID pData)
{
    return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveVotePlayerPoint);
}

BOOL CLoongDB::SaveShowPlayerDataCount(DWORD dwID,LPVOID pData)
{
     return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveShowPlayerDataCount);
}

BOOL CLoongDB::SaveVotePlayerDataCount(DWORD dwID,LPVOID pData)
{
      return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveVotePlayerDataCount);
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------

BOOL CLoongDB::SaveActivityRankData(DWORD dwID, LPVOID pActivityRankData)
{
	 return Update(dwID, pActivityRankData, 1, 0, &CLoongDB::FormatSaveActivityRankData);
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------

VOID CLoongDB::FormatSaveActivityRankData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	
	//��MTRANS_POINTER(p, pData, tagNDBC_SaveActiveRankData);
      MTRANS_POINTER(p, pData, tagActiveRankData);
	  pStream->Clear();
	  pStream->SetReplace("activity_rankdata");
	  pStream->FillString("role_id=") << dwID;
	  pStream->FillString(",role_name='").FillString(p->ucRoleName);
      pStream->FillString("'");

	  pStream->FillString(",role_level=") << p->n16RoleLevel;
	  pStream->FillString(",datatype1=") << p->nDataType[0];
	  pStream->FillString(",datatype2=") << p->nDataType[1];
	  pStream->FillString(",datatype3=") << p->nDataType[2];
	  pStream->FillString(",datatype4=") << p->nDataType[3];
	  pStream->FillString(",datatype5=") << p->nDataType[4];
	  pStream->FillString(",datatype6=") << p->nDataType[5];
	  pStream->FillString(",datatype7=") << p->nDataType[6];
	  pStream->FillString(",datatype8=") << p->nDataType[7];
}




VOID CLoongDB::FormatSaveShowPlayerDataCount(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveShowPlayerDataCount);

	pStream->Clear();
	pStream->SetUpdate("dragongirl_data");
	pStream->FillString("serial_id=") << dwID;
	pStream->FillString(",data_count=") << p->dwTotalDataCount;
}

VOID CLoongDB::FormatSaveVotePlayerDataCount(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{  
	MTRANS_POINTER(p, pData, tagNDBC_SaveVotePlayerDataCount);

	pStream->Clear();
	pStream->SetReplace("dragondefender_data");
    pStream->FillString("serial_id=")<<0;
	pStream->FillString(",role_id=") << 0;
	pStream->FillString(",role_name='").FillString(_T("Record data count flags"));
	pStream->FillString("'");
	pStream->FillString(",point=") <<0;
	pStream->FillString(",data_count=") <<p->dwTotalDataCount;
}
//-------------------------------------------------------------------------------------------------------
// ���汨��ѡ�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveShowPlayerData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagShowPlayer);
	pStream->Clear();
	pStream->SetReplace("dragongirl_data");
	pStream->FillString("sign_id=") << dwID;
	pStream->FillString(",role_id=") <<p->dwAthleteRoleId;

	pStream->FillString(",role_name='").FillString(p->tcAthleteRoleName);
	pStream->FillString("'");
    pStream->FillString(",point=") <<p->dwAthletePoint;
	pStream->FillString(",serial_id=") <<p->dwAthleteSerialId;
	pStream->FillString(",data_count=") <<(DWORD)0;
}

//-------------------------------------------------------------------------------------------------------
// ����ѡ����ѡ�ֵ÷���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveShowPlayerPoint(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveShowPlayerPoint);  

	pStream->Clear();

	pStream->SetReplace("dragongirl_data");
	pStream->FillString("sign_id=") << dwID;
	pStream->FillString(",role_id=") <<p->showPlayer.dwAthleteRoleId;
	pStream->FillString(",role_name='").FillString(p->showPlayer.tcAthleteRoleName);
	pStream->FillString("'");
	pStream->FillString(",point=") <<p->showPlayer.dwAthletePoint;
    pStream->FillString(",serial_id=") <<p->showPlayer.dwAthleteSerialId;
	pStream->FillString(",data_count=") <<(DWORD)0;
}

//-------------------------------------------------------------------------------------------------------
// ����ͶƱ�˵÷���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveVotePlayerPoint(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveVotePlayerPoint);

	pStream->Clear();
	pStream->SetReplace("dragondefender_data");
    pStream->FillString("serial_id=") << p->VotePlayer.dwSerialId;
	pStream->FillString(",role_id=") << dwID;
	pStream->FillString(",role_name='").FillString(p->VotePlayer.tcVoteRoleName);
	pStream->FillString("'");
	pStream->FillString(",point=") <<p->VotePlayer.dwVotePoint;
	pStream->FillString(",data_count=") <<(DWORD)0;
}

//********************************************************************************************************
//-------------------------------------------------------------------------------------------------------
// ��������Ʒ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(pEx, pData, tagNewItemEquip);
	const tagItem *p = pEx->pItem;

	pStream->SetInsert(pEx->pTable);

	pStream->FillString("SerialNum=") << p->n64Serial;
	pStream->FillString(",TypeID=") << p->dwTypeID;

	pStream->FillString(",Num=") << p->n16Num;
	pStream->FillString(",Bind=") << p->byBind;
	pStream->FillString(",LockStat=") << p->bLock;
	pStream->FillString(",UnlockTime=") << p->dwUnlockTime;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	
	pStream->FillString(",CreateMode=") << p->eCreateMode;
	pStream->FillString(",CreateID=") << p->dwCreateID;

	pStream->FillString(",CreatorID=") << p->dwCreatorID;
	pStream->FillString(",NameID=") << p->dwNameID;

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwCreateTime);
	pStream->FillString(",CreateTime='").FillString(m_szTime);
	pStream->FillString("'");

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->eConType;
	pStream->FillString(",Suffix=") << p->n16Index;

	pStream->FillString(",ItemSpecVal1=") << p->dwSpecVal1;
	pStream->FillString(",ItemSpecVal2=") << p->dwSpecVal2;
}

//-------------------------------------------------------------------------------------------------------
// ��ʽ��װ������ͨ�ò���(insert,update)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatCommonEquipSpec(Beton::MyStream *pStream, const tagEquipSpec *pEquipSpec)
{
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->FillString("Quality=") << pEquipSpec->byQuality;
	pStream->FillString(",MinUseLevel=") << pEquipSpec->byMinUseLevel;
	pStream->FillString(",MaxUseLevel=") << pEquipSpec->byMaxUseLevel;
	pStream->FillString(",AttALimMod=") << pEquipSpec->n16AttALimMod;
	pStream->FillString(",AttALimModPct=") << pEquipSpec->n16AttALimModPct;

	pStream->FillString(",WuHun=") << pEquipSpec->n16WuHun;
	pStream->FillString(",MinDmg=") << pEquipSpec->n16MinDmg;
	pStream->FillString(",MaxDmg=") << pEquipSpec->n16MaxDmg;
	pStream->FillString(",Armor=") << pEquipSpec->n16Armor;
	pStream->FillString(",PotVal=") << pEquipSpec->nPotVal;
	pStream->FillString(",PotValUsed=") << pEquipSpec->nPotValUsed;
	// Jason 2009-12-1װ��Ǳ��ֵ����������Ĭ��0
	pStream->FillString(",PotIncTimes=") << pEquipSpec->nPotIncTimes;
	// Jason 2009-12-2 ����װ����ɫƷ���������ʣ�Ĭ��0
	pStream->FillString  (",PurpleQltyIdfPct=") << pEquipSpec->dwPurpleQltyIdfPct;
	// Jason 2010-6-11
	pStream->FillString(",RoleAtt2EffectCount=") << pEquipSpec->nRoleL2AttCount;

	pStream->FillString(",PosyTimes=") << pEquipSpec->byPosyTimes;
	pStream->FillString(",EngraveTimes=") << pEquipSpec->byEngraveTimes;
	pStream->FillString(",HoleNum=") << pEquipSpec->byHoleNum;
	pStream->FillString(",BrandLevel=") << pEquipSpec->byBrandLevel;
	pStream->FillString(",LongFuLevel=") << pEquipSpec->byLongfuLevel;

	pStream->FillString(",CanCut=") << pEquipSpec->bCanCut;
	pStream->FillString(",LongInnerID=") << pEquipSpec->dwLongHunInnerID;
	pStream->FillString(",LongOuterID=") << pEquipSpec->dwLongHunOuterID;
	pStream->FillString(",SpecAtt=") << pEquipSpec->bySpecAtt;
	pStream->FillString(",FlareVal=") << pEquipSpec->byFlareVal;
	pStream->FillString(",QltyModPct=") << pEquipSpec->n16QltyModPct;
	pStream->FillString(",QltyModPctEx=") << pEquipSpec->n16QltyModPctEx;
	pStream->FillString(",PotValModPct=") << pEquipSpec->n16PotValModPct;

	pStream->FillString(",Signed=") << pEquipSpec->bySigned;
	pStream->FillString(",SignRoleID=") << pEquipSpec->dwSignatruePlayerID;
	pStream->FillString(",Signature='").FillString(pEquipSpec->szSignature).FillString("'");

	// ʱװ����
	pStream->FillString(",Appearance=") << pEquipSpec->n16Appearance;
	pStream->FillString(",Rein=") << pEquipSpec->byRein;
	pStream->FillString(",Savvy=") << pEquipSpec->bySavvy;
	pStream->FillString(",Fortune=") << pEquipSpec->byFortune;
	pStream->FillString(",ColorID=") << pEquipSpec->n8ColorID;
	// Jason 2010-5-18 v2.1.2
	pStream->FillString(",SoaringFlag=") << pEquipSpec->bSoaringFlag;
	pStream->FillString(",CastingTimes=") << pEquipSpec->nCastingTimes;
	// Jason 2010-6-13 v1.0.0
	pStream->FillString(",ConsolidateLevel=") << pEquipSpec->byConsolidateLevel;

	pStream->FillString(",RoleAttEffect='").FillBlob(pEquipSpec->nRoleAttEffect, sizeof(pEquipSpec->nRoleAttEffect), pCon);
	pStream->FillString("',PosyEffect='").FillBlob(pEquipSpec->PosyEffect, sizeof(pEquipSpec->PosyEffect), pCon);
	pStream->FillString("',EngraveAtt='").FillBlob(pEquipSpec->nEngraveAtt, sizeof(pEquipSpec->nEngraveAtt), pCon);
	pStream->FillString("',HoleGemID='").FillBlob(pEquipSpec->dwHoleGemID, sizeof(pEquipSpec->dwHoleGemID), pCon);
	pStream->FillString("',DerateVal='").FillBlob(pEquipSpec->nBrandAtt, sizeof(pEquipSpec->nBrandAtt), pCon);
	pStream->FillString("',WuXing='").FillBlob(pEquipSpec->byLongfuAtt, sizeof(pEquipSpec->byLongfuAtt), pCon);
	pStream->FillString("'");
	// Jason 2010-6-19 v1.0.0 ǿ������Ӱ��
	pStream->FillString(",ConsolidateAttEffect='").FillBlob(pEquipSpec->ConsolidateEffect, sizeof(pEquipSpec->ConsolidateEffect), pCon);
	pStream->FillString("'");

	// Jason 2010-9-7 ����
	pStream->FillString(",LastGodStrengthenLevel=")	<< pEquipSpec->byLastGodStrengthenLevel;
	pStream->FillString(",GodStrengthenLevel=")		<< pEquipSpec->byGodStrengthenLevel;
	pStream->FillString(",CurGodStrengthenExp=")		<< pEquipSpec->dwCurGodStrengthenExp;
	pStream->FillString(",GodStrengthenTime=")		<< ((DWORD)const_cast<tagEquipSpec*>(pEquipSpec)->dwGodStrengthenTime);

	// װ���ɳ�
	pStream->FillString(",GrowID=") << pEquipSpec->dwGrowTypeID;
	pStream->FillString(",GrowValue=") << pEquipSpec->dwGrowValue;
	pStream->FillString(",GrowLevel=") << pEquipSpec->byGrowLevel;
	pStream->FillString(",GrowTimes=") << pEquipSpec->nTotalGrowTimes;
	pStream->FillString(",GrowUseIMTimes=") << pEquipSpec->nTotalIMUseTimes;
	pStream->FillString(",GrowStars='").FillBlob(pEquipSpec->eGrowStar, sizeof(pEquipSpec->eGrowStar), pCon);
	pStream->FillString("',GrowAtt='").FillBlob(pEquipSpec->nGrowAttValue, sizeof(pEquipSpec->nGrowAttValue), pCon);
	pStream->FillString("'");
	// ��ʯ��������
	pStream->FillString(",DiamondLevel=")<< pEquipSpec->byDiamondLevel;
	//Ԫ��ǿ��
	pStream->FillString(",byStarEnhanceLvl=") << pEquipSpec->byStarEnhanceLv;
	pStream->FillString(",byHolyHoleNum=") << pEquipSpec->byHolyHoleNum;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ������װ��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNewItemEquip);

	pStream->SetInsert(p->pTable);
	
	pStream->FillString("SerialNum=") << p->pEquip->n64Serial;
	pStream->FillString(",");
	FormatCommonEquipSpec(pStream, &p->pEquip->equipSpec);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ����Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pItem)
{
	MTRANS_POINTER(p, pItem, tagItem);

	pStream->SetUpdate("item");

	//pStream->FillString("SerialNum=") << p->n64Serial;
	//pStream->FillString(",TypeID=") << p->dwTypeID;

	pStream->FillString("Num=") << p->n16Num;
	pStream->FillString(",Bind=") << p->byBind;
	pStream->FillString(",UnlockTime=") << p->dwUnlockTime;
	pStream->FillString(",LockStat=") << p->bLock;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	
	//pStream->FillString(",CreateMode=") << p->eCreateMode;
	//pStream->FillString(",CreateID=") << p->dwCreateID;

	//pStream->FillString(",CreatorID=") << p->dwCreatorID;

	//DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwCreateTime);
	//pStream->FillString(",CreateTime='").FillString(m_szTime);
	//pStream->FillString("'");

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->eConType;
	pStream->FillString(",Suffix=") << p->n16Index;

	pStream->FillString(",ItemSpecVal1=") << p->dwSpecVal1;
	pStream->FillString(",ItemSpecVal2=") << p->dwSpecVal2;

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// ����װ����Ϣ -- �޸�equip��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquip)
{
	MTRANS_POINTER(p, pEquip, tagEquip);

	pStream->SetUpdate("equip");
	FormatCommonEquipSpec(pStream, &p->equipSpec);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// ����װ������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquipSpecUpdate)
{
	MTRANS_POINTER(p, pEquipSpecUpdate, tagEquipSpecUpdate);

	pStream->SetUpdate("equip");
	FormatCommonEquipSpec(pStream, &p->equipSpec);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}
//-------------------------------------------------------------------------------------------------------
// ����װ����������
//-------------------------------------------------------------------------------------------------------

VOID CLoongDB::FormatUpdateEquipSingleSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquipSingleSpecUpdate)
{
	MTRANS_POINTER(p, pEquipSingleSpecUpdate, tagEquipSingleSpec);
	if ( EESSType_Null == p->eEquipSingleSpecType )
	{
		return ;
	}
	pStream->SetUpdate("equip");
	switch( p->eEquipSingleSpecType )
	{
	case EESSType_StarEnhance:
		pStream->FillString("byStarEnhanceLvl=") << p->dwVal;
		break;
	case EESSType_HolyEquipHole:
		pStream->FillString("byHolyHoleNum=") << p->dwVal;
		break;
	default:
		return;
	}	
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItem);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItem);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}


//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
// Jason 2010-1-0 v1.3.2 ����ÿ�����߽�������ֶ�
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveRoleData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	tagRoleDataSave *p = (tagRoleDataSave *)pData;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	
	pStream->SetUpdate("roledata");
	pStream->FillString("AvatarEquip='").FillBlob(&p->AvatarEquip, SIZE_AVATAR_EQUIP, pCon);
	pStream->FillString("',DisplaySet=")<< (BYTE)p->DisplaySet;
	pStream->FillString(",MapID=") << p->dwMapID;
	pStream->FillString(",X=") << p->fCoordinate[0];// ����
	pStream->FillString(",Y=") << p->fCoordinate[1];
	pStream->FillString(",Z=") << p->fCoordinate[2];
	pStream->FillString(",FaceX=") << p->fFace[0];// ����
	pStream->FillString(",FaceY=") << p->fFace[1];
	pStream->FillString(",FaceZ=") << p->fFace[2];

	pStream->FillString(",RebornMapID=") << p->dwRebornMapID;
	pStream->FillString(",SpeakOff=") << p->bSpeakOff;
	pStream->FillString(",Class=") << (INT32)p->eClass;
	pStream->FillString(",ClassEx=") << p->eClassEx;
	pStream->FillString(",Level=") << p->nLevel;
	pStream->FillString(",LastUpgrade=") << p->dwLastUpgrade;
	pStream->FillString(",ExpCurLevel=") << p->nCurExp;

	pStream->FillString(",HP=") << p->nHP;
	pStream->FillString(",MP=") << p->nMP;
	pStream->FillString(",Rage=") << p->nRage;
	pStream->FillString(",Endurance=") << p->nEndurance;
	pStream->FillString(",Vitality=") << p->nVitality;
	pStream->FillString(",Injury=") << p->nInjury;
	pStream->FillString(",Knowledge=") << p->nKnowledge;
	pStream->FillString(",Morale=") << p->nMorale;
	pStream->FillString(",Morality=") << p->nMorality;
	pStream->FillString(",Culture=") << p->nCulture;
	pStream->FillString(",Hostility=") << p->nHostility;
	pStream->FillString(",Credit=") << p->nCredit;
	pStream->FillString(",Identity=") << p->nIdentity;
	pStream->FillString(",Spirit=") << p->nSpirit;
	pStream->FillString(",WuXun=") << p->nWunXun;
	pStream->FillString(",WuJi=") << p->nWunJi;
	pStream->FillString(",Weary=") << p->nWeary;
	pStream->FillString(",FixSpirit=") << p->dwFixSpiritCD;
	pStream->FillString(",VIPPoint=") << p->nVIPPoint;
	pStream->FillString(",VipLevel=") << p->nVipLevel;
	pStream->FillString(",VipStartTime=") << p->dwVipStartTime;
	pStream->FillString(",VipMaxDays=") << p->dwVipMaxDays;
	pStream->FillString(",FreeWorldTalkTimes=") << p->n16FreeWorldTalkTimes;
	pStream->FillString(",AttPtAvail=") << p->nAttPoint;
	pStream->FillString(",TalentPtAvail=") << p->nTalentPoint;

	pStream->FillString(",Strength=") << p->nStrength;
	pStream->FillString(",EquipValue=") << p->nEquipValue;
	pStream->FillString(",YaoJingValue=") << p->nYaoJingValue;
	pStream->FillString(",AdvanceStrength=") << p->nAdvanceStrength;
	pStream->FillString(",ConsumptiveStrength=") << p->nConsumptiveStrength;
	pStream->FillString(",KillCount=") << p->nKillScore;

	pStream->FillString(",PhysiqueAdded=") << p->nAttPointAdd[ERA_Physique];
	pStream->FillString(",StrengthAdded=") << p->nAttPointAdd[ERA_Strength];
	pStream->FillString(",PneumaAdded=") << p->nAttPointAdd[ERA_Pneuma];
	pStream->FillString(",InnerforceAdded=") << p->nAttPointAdd[ERA_InnerForce];
	pStream->FillString(",TechniqueAdded=") << p->nAttPointAdd[ERA_Technique];
	pStream->FillString(",AgilityAdded=") << p->nAttPointAdd[ERA_Agility];

	pStream->FillString(",TalentType1=") << p->talent[0].eType;
	pStream->FillString(",TalentType2=") << p->talent[1].eType;
	//pStream->FillString(",TalentType3=") << p->talent[2].eType;
	//pStream->FillString(",TalentType4=") << p->talent[3].eType;
	pStream->FillString(",TalentVal1=") << p->talent[0].nPoint;
	pStream->FillString(",TalentVal2=") << p->talent[1].nPoint;
	//pStream->FillString(",TalentVal3=") << p->talent[2].nPoint;
	//pStream->FillString(",TalentVal4=") << p->talent[3].nPoint;

	pStream->FillString(",SGFlag=") << p->bSafeGuard;
	
	pStream->FillString(",BagSize=") << p->n16BagSize;		// ����
	pStream->FillString(",BagGold=") << p->nBagGold;
	pStream->FillString(",BagSilver=") << p->nBagSilver;
	pStream->FillString(",BagYuanBao=") << p->nBagYuanBao;
	pStream->FillString(",ExchangeVolume=") << p->nExVolume;

	//pStream->FillString(",WareSize=") << p->n16WareSize;		// �˺Ųֿ�
	//pStream->FillString(",WareGold=") << p->nWareGold;
	//pStream->FillString(",WareSilver=") << p->nWareSilver;
	//pStream->FillString(",WareYuanBao=") << p->nWareYuanBao;

	pStream->FillString(",TotalTax=") << p->nTotalTax;	// ��˰�ܶ�
	pStream->FillString(",RemoteOpenSet=") << (DWORD)p->sRemoteOpenSet;
	pStream->FillString(",CurTitleID=") << p->u16ActiveTitleID;

	if(DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->LoginTime))
	{
		pStream->FillString(",LoginTime='").FillString(m_szTime);
		pStream->FillString("'");

		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->LogoutTime);
		pStream->FillString(",LogoutTime='").FillString(m_szTime);
		pStream->FillString("'");
		
		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->CloseSafeGuardTime);
		pStream->FillString(",CloseSGTime='").FillString(m_szTime);
		pStream->FillString("'");

		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwTimeGetMallFree);
		pStream->FillString(",GetMallFreeTime='").FillString(m_szTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
		ASSERT(0);
	}

	pStream->FillString(",OnlineTime=") << p->nOnlineTime;
	pStream->FillString(",CurOnlineTime=") << p->nCurOnlineTime;

	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Role, pCon);
	pStream->FillString("'");

	pStream->FillString(",StallLevel=") << p->byStallLevel;
	pStream->FillString(",StallDailyExp=") << p->nStallDailyExp;
	pStream->FillString(",StallCurExp=") << p->nStallCurExp;
	pStream->FillString(",StallLastTime=") << p->dwLastStallTime;

	pStream->FillString(",DeadUnSetSafeGuardCountDown=") << p->nDeadUnSetSafeGuardCountDown;
	pStream->FillString(",NeedPrisonRevive=") << p->bNeedPrisonRevive;

	pStream->FillString(",LoverID=") << p->dwLoverID;
	pStream->FillString(",HaveWedding=") << p->bHaveWedding;

	// Jason 2009-12-7 ���������͸�����ֶ�
	pStream->FillString(",LastLessingTime=") << (DWORD)p->timeLastLessing;
	pStream->FillString(",LastLessingLevel=") << p->nLessingLevel;
	//===============================ÿ�����߽���============================================
	// Jason 2010-3-15 v1.5.0�����͸���չ����
	pStream->FillString(",LastLessingLevelEx=") << p->nLessingLevelEx;
	pStream->FillString(",LastLessingLoongDate=") << (DWORD)p->dateLastLessingLoong;
	pStream->FillString(",CurrentDayOnlineTime=") << p->dwCurrentDayOnlineTime;
	pStream->FillString(",OneDayFirstLoginedTime=") << (DWORD)p->timeOneDayFirstLogined;
	pStream->FillString(",OnlineRewardPerDayRecTime=") << (DWORD)p->timeOnlineRewardPerDayRec;
	//===============================ÿ�����߽���============================================
	//--------------------------------------------------------------------------------------------------
	// Jason 2010-1-30 v1.3.2��������
	pStream->FillString(",FollowPetPocketValue=") << (DWORD)p->u16FollowPetPocketValve;
	pStream->FillString(",RidingPetPocketValue=") << (DWORD)p->u16RidingPetPocketValve;
	//--------------------------------------------------------------------------------------------------

	// ʦͽ���
	pStream->FillString(",MasterID=") << (DWORD)p->dwMasterID;

	int nQinWuPoint = p->nQinWuPoint;
	int nJingWuPoint = p->nJingWuPoint;

	// 2147483647=2��31�η���1
	if (nJingWuPoint < 0)
	{
		nJingWuPoint = 0;
	}
	else if (nJingWuPoint > 2147483647)
	{
		nJingWuPoint = 2147483647;
	}
	// 2147483647=2��31�η���1
	if (nQinWuPoint < 0)
	{
		nQinWuPoint = 0;
	}
	else if (nQinWuPoint > 2147483647)
	{
		nQinWuPoint = 2147483647;
	}

	pStream->FillString(",JingWuPoint=") << nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << nQinWuPoint;
	pStream->FillString(",MARelation=") << p->nMARelation;
	pStream->FillString(",MiraclePrcNum=") << (INT16)p->n16MiraclePracticeNum;

	pStream->FillString(",Buy50LvlItemFlag=") << p->byBuy50LvlItemFlag;
	pStream->FillString(",Buy60LvlItemFlag=") << p->byBuy60LvlItemFlag;
	pStream->FillString(",Buy70LvlItemFlag=") << p->byBuy70LvlItemFlag;
	pStream->FillString(",Buy80LvlItemFlag=") << p->byBuy80LvlItemFlag;
	pStream->FillString(",ExpPilularUseTimes=") << p->dwExpPilularUseTimes;

	// Jason 2010-3-29 v1.5.1 �񼣻���
	pStream->FillString(",GodMiraclePoints=") << (INT16)p->nGodMiraclePoints;

	pStream->FillString(",TrainStateTransfered=") << p->byTrainStatesTransfered;

	// Jason 2010-5-18 v2.1.2 ������־
	pStream->FillString(",SoaringValue =") << p->nSoaringValue;
	pStream->FillString(",SoaringSkillLearnTimes =") << p->nSoaringSkillLearnTimes;
	pStream->FillString(",Toughness =") << p->nToughness;

	// ͨ��������WorkedLevel�ֶΣ����ֶ���DB����
	INT64 n64Tmp = 0;
	if ( p->dwLastUpgrade != 0)
	{
		n64Tmp = (X_LONG_STR_4WORKED_LEVEL * p->nLevel) + (X_LONG_STR_4WORKED_LEVEL - p->dwLastUpgrade);
		pStream->FillString(",WorkedLevel=") << n64Tmp;
	}
	else
	{
		n64Tmp = (X_LONG_STR_4WORKED_LEVEL * p->nLevel) + (X_LONG_STR_4WORKED_LEVEL - 1);
		pStream->FillString(",WorkedLevel=") << n64Tmp;
	}
	
	pStream->FillString(",CompleteRefreshTime=") << p->nCompleteRefreshTime;
	pStream->FillString(",IMRefreshTime=") << p->nIMRefreshTime;
	pStream->FillString(",YListCompleteRefreshTime=") << p->nYellowListCompleteRefreshTime;
	pStream->FillString(",YListIMRefreshTime=") << p->nYellowListIMRefreshTime;

	// Jason 2010-6-12 v1.0.0
	pStream->FillString(",ConsolidateTimes=") << p->nConsolidateTimes;
	pStream->FillString(",RoleState=") << p->dwRoleState;

	// Jason 2010-11-10 v1.2.2 ����ս��ÿ����ȡ���߽���ʱ���¼
	pStream->FillString(",TakeGuildRewardTime=") << (DWORD)p->timeTakeGuildReward;

	pStream->FillString(",PickupModeSetting=") << p->dwPickupModeSetting;

	pStream->FillString(",PlayerBack=") << p->byPlayerBack;
	pStream->FillString(",PlayerBackDays=") << p->byPlayerBackDays;

	pStream->FillString(",UseConstraintsMaxPKSafeGuardLevel=") << p->byUsingConstraintsPKSafeGuardLevel;
	pStream->FillString(",BePlayActLayerID=") << p->dwBePlayActLayerID;
	pStream->FillString(",BePlayActLevel=") << p->byBePlayActLevel;

	pStream->FillString(",GodHead=") << p->nGodhead;
	pStream->FillString(",GodFaith=") << p->nGodFaith;
	pStream->FillString(",GodCondenced=") << p->nGodCondense;

	// ��ְ���
	pStream->FillString(",Clergy=") << p->dwClergy;
	pStream->FillString(",PreClergy=") << p->dwPreClergy;
	pStream->FillString(",ClergyMarsPoint=") << p->dwClergyMarsPoint;
	pStream->FillString(",ClergyApolloPoint=") << p->dwClergyApolloPoint;
	pStream->FillString(",ClergyRabbiPoint=") << p->dwClergyRabbiPoint;
	pStream->FillString(",ClergyPeacePoint=") << p->dwClergyPeacePoint;
	pStream->FillString(",ClergyCandidateType=") << p->eClergyCandidate;
	pStream->FillString(",Clergy4SeniorOrJunior=") << (p->eSeniorOrJunior);

	// KeyCode
	pStream->FillString(",KeyCodeRewarded=") << p->dwKeyCodeRewarded;

	// ����ID
	pStream->FillString(",FamilyID=") << p->dwFamilyID;

	pStream->FillString(",TrainDate=") << p->dwTrainDate;

	pStream->FillString(",TrainNum=") << p->nTrainNum;

	pStream->FillString(",award_point=") << p->nAwardPoint;

	pStream->FillString(",award_flag=") << p->nAwardFlag;

	pStream->FillString(",BrotherTeacherID=") << p->dwBrotherTeatherID;

	pStream->FillString(",BrotherEndTime=") << p->timeBloodBrother;

	pStream->FillString(",PulseRemainTime=") << p->nPulseLearnRemainTime;

	pStream->FillString(",TigerTime=") << p->nTigerTime;

	pStream->FillString(",god_soul=") << p->nGod;
	pStream->FillString(",monster_soul=") << p->nMonster;
	pStream->FillString(",god_point=") << p->nGodPoint;
	pStream->FillString(",monster_point=") << p->nMonsterPoint;

	//�ʻ���������by lib
	pStream->FillString(",flower_num=") << p->nFlower;
	pStream->FillString(",egg_num=") << p->nEgg;

	// ����ʥ��ֵ
	pStream->FillString(",holy_value=") << p->nHolyValue;	
	// ����
	pStream->FillString(",role_hit_add=") << p->nHidAdd;
	// Ԫ��ȫ��
	pStream->FillString(",role_eei_all=") << p->nEEI_All;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	m_pDBLoong->ReturnConnection(pCon);
}

VOID CLoongDB::FormatSaveRoleInfo(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	tagRoleInfo *p = (tagRoleInfo *)pData;

	pStream->SetUpdate("roledata");
	pStream->FillString("MapID=") << p->dwMapID;
	pStream->FillString(",LoverID=") << p->dwLoverID;
	pStream->FillString(",MARelation=") << p->nMARelation;
	pStream->FillString(",VipLevel=") << p->nVIPLevel;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertVCard(Beton::MyStream *pStream, DWORD dwRoleID, PVOID pDum)
{
	ASSERT(!P_VALID(pDum));

	pStream->SetInsert("visiting_card");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateVCard(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagVCardData);
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetUpdate("visiting_card");

	pStream->FillString("Level=") << (INT32)p->dwLevel;
	pStream->FillString(",Job=") << (INT32)p->dwJob;
	pStream->FillString(",MateRoleID=") << (INT32)p->dwMateRoleID;
	pStream->FillString(",FactionID=") << (INT32)p->dwFactionID;
	pStream->FillString(",Position=") << (INT32)p->dwPosition;

	pStream->FillString(",Visibility=") << p->customVCardData.bVisibility;
	pStream->FillString(",Sex=") << p->customVCardData.bySex;
	pStream->FillString(",Constellation=") << p->customVCardData.byConstellation;
	pStream->FillString(",ChineseZodiac=") << p->customVCardData.byChineseZodiac;
	pStream->FillString(",Area=") << p->customVCardData.byArea;
	pStream->FillString(",Province=") << p->customVCardData.byProvince;
	pStream->FillString(",City='").FillString(p->customVCardData.chCity, pCon).FillString("'");
	pStream->FillString(",HeadUrl='").FillString(reinterpret_cast<TCHAR *>(p->customVCardData.byData), pCon).FillString("'");
	pStream->FillString(",Signature='").FillString(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize])), pCon).FillString("'");
	pStream->FillString(",NameHistory='").FillString(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize + p->customVCardData.bySignatureSize])), pCon).FillString("'");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelVCard(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));
	pStream->SetDelete("visiting_card");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));
	
	pStream->SetInsert("clan_data");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	ASSERT(P_VALID(pData));
	MTRANS_POINTER(pDBRepData, pData, tagDBReputeData);

	pStream->SetUpdate("clan_data");

	pStream->FillString("RepXuanYuan=")		<< pDBRepData->m_nReputation[ECLT_XuanYuan];
	pStream->FillString(",ConXuanYuan=")	<< pDBRepData->m_nContribution[ECLT_XuanYuan];
	pStream->FillString(",ActCntXuanYuan=")	<< pDBRepData->m_n8ActCount[ECLT_XuanYuan];

	pStream->FillString(",RepShenNong=")	<< pDBRepData->m_nReputation[ECLT_ShenNong];
	pStream->FillString(",ConShenNong=")	<< pDBRepData->m_nContribution[ECLT_ShenNong];
	pStream->FillString(",ActCntShenNong=")	<< pDBRepData->m_n8ActCount[ECLT_ShenNong];

	pStream->FillString(",RepFuXi=")		<< pDBRepData->m_nReputation[ECLT_FuXi];
	pStream->FillString(",ConFuXi=")		<< pDBRepData->m_nContribution[ECLT_FuXi];
	pStream->FillString(",ActCntFuXi=")		<< pDBRepData->m_n8ActCount[ECLT_FuXi];

	pStream->FillString(",RepSanMiao=")		<< pDBRepData->m_nReputation[ECLT_SanMiao];
	pStream->FillString(",ConSanMiao=")		<< pDBRepData->m_nContribution[ECLT_SanMiao];
	pStream->FillString(",ActCntSanMiao=")	<< pDBRepData->m_n8ActCount[ECLT_SanMiao];

	pStream->FillString(",RepJiuLi=")		<< pDBRepData->m_nReputation[ECLT_JiuLi];
	pStream->FillString(",ConJiuLi=")		<< pDBRepData->m_nContribution[ECLT_JiuLi];
	pStream->FillString(",ActCntJiuLi=")	<< pDBRepData->m_n8ActCount[ECLT_JiuLi];

	pStream->FillString(",RepYueZhi=")		<< pDBRepData->m_nReputation[ECLT_YueZhi];
	pStream->FillString(",ConYueZhi=")		<< pDBRepData->m_nContribution[ECLT_YueZhi];
	pStream->FillString(",ActCntYueZhi=")	<< pDBRepData->m_n8ActCount[ECLT_YueZhi];

	pStream->FillString(",RepNvWa=")		<< pDBRepData->m_nReputation[ECLT_NvWa];
	pStream->FillString(",ConNvWa=")		<< pDBRepData->m_nContribution[ECLT_NvWa];
	pStream->FillString(",ActCntNvWa=")		<< pDBRepData->m_n8ActCount[ECLT_NvWa];

	pStream->FillString(",RepGongGong=")	<< pDBRepData->m_nReputation[ECLT_GongGong];
	pStream->FillString(",ConGongGong=")	<< pDBRepData->m_nContribution[ECLT_GongGong];
	pStream->FillString(",ActCntGongGong=")	<< pDBRepData->m_n8ActCount[ECLT_GongGong];

	pStream->FillString(",FameMask=")		<< pDBRepData->m_u16FameMask;
	pStream->FillString(",RepRstTimeStamp=")<< pDBRepData->m_dwtLastResetTime;

	pStream->FillString(",TotalFame=")		<< pDBRepData->m_dwTotalRep;
	//pStream->FillString(",FamePm=")			<< pDBRepData->m_nRepPm;

	pStream->SetWhere();
	pStream->FillString("RoleID=")			<< dwRoleID;

}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));

	pStream->SetDelete("clan_data");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateItemPosAndUseRel(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagItemUpdate);

	pStream->SetUpdate("item");
	pStream->FillString("Num=") << p->n16Num;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->byConType;
	pStream->FillString(",Suffix=") << p->n16Index;
	pStream->FillString(",Bind=") << p->byBindType;
// 	pStream->FillString(",UnlockTime=") << p->dwUnlockTime;
	pStream->FillString(",ItemSpecVal1=") << p->dwSpecVal1;
	pStream->FillString(",ItemSpecVal2=") << p->dwSpecVal2;

	CHAR szTime[X_DATATIME_LEN + 1];	// ʱ��ת�����ַ���
	DwordTime2DataTime(szTime, sizeof(szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(szTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagSkillSave);

	pStream->SetInsert("skill");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->dwID;
	pStream->FillString(",BiddenLevel=") << p->nLearnLevel;
	pStream->FillString(",SelfLevel=") << p->nSelfLevel;
	pStream->FillString(",Proficiency=") << p->nProficiency;
	pStream->FillString(",CoolDown=") << p->nCoolDown;
	pStream->FillString(",active_time=") << p->dwActiveTime;
	
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetDelete("skill");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << (*(DWORD*)pData);
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagSkillSave);

	pStream->SetReplace("skill");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->dwID;
	pStream->FillString(",BiddenLevel=") << p->nLearnLevel;
	pStream->FillString(",SelfLevel=") << p->nSelfLevel;
	pStream->FillString(",Proficiency=") << p->nProficiency;
	pStream->FillString(",CoolDown=") << p->nCoolDown;
	pStream->FillString(",active_time=") << p->dwActiveTime;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫbuff��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveRoleBuff(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pBuff)
{
	MTRANS_POINTER(p, pBuff, tagBuffSave);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("buff");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",SrcUnitID=") << p->dwSrcUnitID;
	pStream->FillString(",SrcSkillID=") << p->dwSrcSkillID;
	pStream->FillString(",ItemTypeID=") << p->dwItemTypeID;
	pStream->FillString(",ItemSerialID=") << p->n64Serial;

	pStream->FillString(",BuffID=") << p->dwBuffID;
	pStream->FillString(",CurTick=") << p->nPersistTick;
	pStream->FillString(",Level=") << p->n8Level;
	pStream->FillString(",CurLapTimes=") << p->n8CurLapTimes;

	if(p->n8ModifierNum > 0)
	{
		pStream->FillString(",EffectSkillID='");
		pStream->FillBlob(p->byData, p->n8ModifierNum * sizeof(DWORD), pCon);
		pStream->FillString("'");
	}

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ����ƺ���Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTitle(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagTitleSave);

	pStream->SetUpdate("title");
	pStream->FillString("Count=") << p->dwStateMark;
	pStream->FillString(",TimeRemain=")<< p->dwTimeRemain;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << p->u16TitleID;
}

//-------------------------------------------------------------------------------------------------------
// ����������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestSave *p = (tagQuestSave *)pTask;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetReplace("task");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",TaskFlag=") << p->bQuestFlag;
	pStream->FillString(",StartTime=") << p->dwAcceptTime;

	for(INT32 i=0; i<QUEST_CREATURES_COUNT; ++i)
	{
		pStream->FillString(",Monster") << i;
		pStream->FillString("Num=") << (INT16)p->n16MonsterNum[i];
	}

	pStream->FillString(",ChuandaoCount=") << p->nChuandaoCount;
	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Quest, pCon);
	pStream->FillString("'");

	if(p->DynamicTarget.eTargetType != EQTT_NULL)
	{
		pStream->FillString(",DynamicTarget='");
		pStream->FillBlob(&p->DynamicTarget, sizeof(tagQuestDynamicTarget), pCon);
		pStream->FillString("'");
	}
	
	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ���������������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTaskDone(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestDoneSave *p = (tagQuestDoneSave *)pTask;

	pStream->SetReplace("task_done");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",StartTime=") << p->dwStartTime;
	pStream->FillString(",Times=") << p->nTimes;
}

//-------------------------------------------------------------------------------------------------------
// ����������ճ�������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveDailyTaskDone(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestDoneSave *p = (tagQuestDoneSave *)pTask;

	pStream->SetReplace("dailytask_done");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",StartTime=") << p->dwStartTime;
	pStream->FillString(",Times=") << p->nTimes;
}

//-------------------------------------------------------------------------------------------------------
// ��״̬����ɾ��ָ����¼
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelAllRoleBuff(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDummy)
{
	pStream->SetDelete("buff");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// ����̶���ű�����
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveActivity(Beton::MyStream *pStream, DWORD dwID, LPVOID pActivityData)
{
	tagActivityData *p = (tagActivityData *)pActivityData;
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	pStream->SetReplace("activity");
	pStream->FillString("dwID=") << dwID;
	pStream->FillString(",ScriptData='");
	pStream->FillBlob(&p->Data.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData1='");
	pStream->FillBlob(&p->MergedData[0].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData2='");
	pStream->FillBlob(&p->MergedData[1].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData3='");
	pStream->FillBlob(&p->MergedData[2].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData4='");
	pStream->FillBlob(&p->MergedData[3].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	m_pDBLoong->ReturnConnection(pCon);
}
//-------------------------------------------------------------------------------------------------------
// ����Ĭ�ϳ�����Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatCreateCityInfo(Beton::MyStream *pStream, DWORD dwCityID, PVOID pData)
{
	pStream->Clear();
	pStream->SetInsert("city");
	pStream->FillString("id=") << dwCityID;
}

//-------------------------------------------------------------------------------------------------------
// ���������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveCityInfo(Beton::MyStream *pStream, DWORD dwCityID, PVOID pData)
{
	MTRANS_POINTER(p, pData, tagCityAtt2DB);

	pStream->Clear();
	pStream->SetUpdate("city");
	pStream->FillString("guild_id=") << p->dwGuildID;
	pStream->FillString(",defence=") << p->nDefence;
	pStream->FillString(",eudemon_tally=") << p->nEudemonTally;
	pStream->FillString(",tax_rate=") << p->nTaxRate;
	pStream->FillString(",tax_rate_time=") << p->dwTaxRateTime;
	pStream->FillString(",taxation=") << p->nTaxation;
	pStream->FillString(",prolificacy=") << p->nProlificacy;
	pStream->SetWhere();
	pStream->FillString("id=") << p->dwCityID;
}

//-------------------------------------------------------------------------------------------------------
// ����ָ����������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateCityInfo(Beton::MyStream *pStream, DWORD dwCityID, PVOID pData)
{
	MTRANS_POINTER(p, pData, tagCityAppointedAtt);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("city");

	switch (p->eType)
	{
	case ECAT_Holder:
		pStream->FillString("guild_id=") << *(DWORD*)(p->byData);
		break;

	case ECAT_Defence:
		pStream->FillString("defence=") << *(INT32*)(p->byData);
		break;

	case ECAT_EudemonTally:
		pStream->FillString("eudemon_tally=") << *(INT32*)(p->byData);
		break;

	case ECAT_TaxRate:
		pStream->FillString("tax_rate=") << *(INT32*)(p->byData);
		break;
		
	case ECAT_TaxRateTime:
		pStream->FillString("tax_rate_time=") << *(DWORD*)(p->byData);
		break;

	case ECAT_Taxation:
		pStream->FillString("taxation=") << *(INT32*)(p->byData);
		break;

	case ECAT_Prolificacy:
		pStream->FillString("prolificacy=") << *(INT32*)(p->byData);
		break;

	case ECAT_AttackSignUp:
	case ECAT_DefenceSignUp:
	case ECAT_RequestSignUp:
		{
			pStream->FillString("signup_list='").FillBlob(p->byData, sizeof(tagCitySignUp) * p->nDataNum, pCon);
			pStream->FillString("'");
		}
		break;
	default:
		pStream->FillString("id=") << dwCityID;
		break;
	}

	pStream->SetWhere();
	pStream->FillString("id=") << dwCityID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ����ʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveAllMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	int nQinWuPoint = p->nQinWuPoint;
	int nJingWuPoint = p->nJingWuPoint;

	// 2147483647=2��31�η���1
	if (nJingWuPoint < 0)
	{
		nJingWuPoint = 0;
	}
	else if (nJingWuPoint > 2147483647)
	{
		nJingWuPoint = 2147483647;
	}
	// 2147483647=2��31�η���1
	if (nQinWuPoint < 0)
	{
		nQinWuPoint = 0;
	}
	else if (nQinWuPoint > 2147483647)
	{
		nQinWuPoint = 2147483647;
	}

	pStream->Clear();
	pStream->SetUpdate("master");
	pStream->FillString("dwID=") << p->dwID;
	pStream->FillString(",JingWuPoint=") << nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << nQinWuPoint;
	pStream->FillString(",LevelUpBonus=") << p->nLevelUpBonus;
	pStream->FillString(",ApprenticeData='").FillBlob(p->sApprentice.dwData, sizeof(tagApprenticeData), pCon);
	pStream->FillString("'");
	pStream->SetWhere();
	pStream->FillString("dwID=") << p->dwID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ����һ��ʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveOneMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagOneMASaveData);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("master");

	switch(p->oneMAData.eType)
	{
	case EOMAD_JingWu:
		{
			INT nJingWuPoint = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nJingWuPoint = p->loadData.nJingWuPoint + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nJingWuPoint = *(INT*)p->oneMAData.byData;
			}

			// 2147483647=2��31�η���1
			if (nJingWuPoint < 0)
			{
				nJingWuPoint = 0;
			}
			else if (nJingWuPoint > 2147483647)
			{
				nJingWuPoint = 2147483647;
			}
			pStream->FillString("JingWuPoint=") << nJingWuPoint;
			break;
		}
		

	case EOMAD_QinWu:
		{
			INT nQinWuPoint = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nQinWuPoint = p->loadData.nQinWuPoint + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nQinWuPoint = *(INT*)(p->oneMAData.byData);
			}
			// 2147483647=2��31�η���1
			if (nQinWuPoint < 0)
			{
				nQinWuPoint = 0;
			}
			else if (nQinWuPoint > 2147483647)
			{
				nQinWuPoint = 2147483647;
			}
			pStream->FillString("QinWuPoint=") << nQinWuPoint;
			break;
		}
		

	case EOMAD_LevelUpBonus:
		{
			INT nLevelUpBonus = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nLevelUpBonus = p->loadData.nLevelUpBonus + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nLevelUpBonus = *(INT*)(p->oneMAData.byData);
			}

			pStream->FillString("LevelUpBonus=") << nLevelUpBonus;
			break;
		}


	case EOMAD_RewardNum:
		{
			INT nRewardNum = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nRewardNum = p->loadData.nRewardNum + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nRewardNum = *(INT*)(p->oneMAData.byData);
			}

			pStream->FillString("RewardNum=") << nRewardNum;
			break;
		}


	case EOMAD_Apprentice:
		{
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				int nNum = sizeof(p->loadData.sApprentice.dwData) / sizeof(DWORD);
				int nIndex;
				for (nIndex=0; nIndex!=nNum; ++nIndex)
				{
					if (p->loadData.sApprentice.dwData[nIndex] == 0)
					{
						break;
					}
				}

				p->loadData.sApprentice.dwData[nIndex] = *(DWORD*)(p->oneMAData.byData);
				pStream->FillString("ApprenticeData='").FillBlob(p->loadData.sApprentice.dwData, sizeof(tagApprenticeData), pCon);
				pStream->FillString("'");
			}

			else
			{
				pStream->FillString("ApprenticeData='").FillBlob((DWORD*)(p->oneMAData.byData), sizeof(tagApprenticeData), pCon);
				pStream->FillString("'");
			}

			break;
		}
		

	default:
		break;
	}

	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ����ʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	int nQinWuPoint = p->nQinWuPoint;
	int nJingWuPoint = p->nJingWuPoint;

	// 2147483647=2��31�η���1
	if (nJingWuPoint < 0)
	{
		nJingWuPoint = 0;
	}
	else if (nJingWuPoint > 2147483647)
	{
		nJingWuPoint = 2147483647;
	}
	// 2147483647=2��31�η���1
	if (nQinWuPoint < 0)
	{
		nQinWuPoint = 0;
	}
	else if (nQinWuPoint > 2147483647)
	{
		nQinWuPoint = 2147483647;
	}

	pStream->Clear();
	pStream->SetInsert("master");
	pStream->FillString("dwID=") << p->dwID;
	pStream->FillString(",JingWuPoint=") << nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << nQinWuPoint;
	pStream->FillString(",LevelUpBonus=") << p->nLevelUpBonus;
	pStream->FillString(",ApprenticeData='").FillBlob(p->sApprentice.dwData, sizeof(tagApprenticeData), pCon);
	pStream->FillString("'");

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ���������Ϣ
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveItemLockInfo(INT64 n64ItemID, BYTE byType, DWORD dwUnlockTime)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("item");
	pStream->FillString("LockStat=") << byType;
	pStream->FillString(",UnlockTime=") << dwUnlockTime;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64ItemID;

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);

	// ��ѯ
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
}

//-------------------------------------------------------------------------------------------------------
// ������Ʒ��״̬
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveItemBindInfo(INT64 n64ItemID, BYTE byType)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("item");
	pStream->FillString("Bind=") << byType;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64ItemID;

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);

	// ��ѯ
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
}

VOID CLoongDB::SaveRoleSpeak(DWORD dwRoleID, bool bState)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	INT nTmp = (bState) ? 0:1;

	pStream->SetUpdate("roledata");
	pStream->FillString("SpeakOff=") << nTmp;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// ִ��
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
}

DWORD CLoongDB::CheckName(LPCTSTR szName)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return E_SystemError;

	pStream->SetSelect("roledata", "*");
	pStream->SetWhere();
	pStream->FillString("RoleName='").FillString(szName).FillString("'");

	DWORD dwErr = E_Success;
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		dwErr = E_SystemError;
	}
	else if (pResult->GetRowCount() > 0)
	{
		dwErr = E_CreateRole_NameExist;
	}

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return dwErr;
}

////-------------------------------------------------------------------------------------------------------
//// ����Ʒ����ɾ��ָ����¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	INT64 *p = (INT64 *)pData;
//
//	pStream->SetDelete("item");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and SerialNum=") << *p;
//}
//
////-------------------------------------------------------------------------------------------------------
//// ��װ������ɾ��ָ����¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecEquip(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	INT64 *p = (INT64 *)pData;
//
//	pStream->SetDelete("equip");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and SerialNum=") << *p;
//}
//
////-------------------------------------------------------------------------------------------------------
//// ���������ɾ��ָ����¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	DWORD *p = (DWORD *)pData;
//
//	pStream->SetDelete("task");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and TaskID=") << *p;
//}

//-------------------------------------------------------------------------------------------------------
//  ����һ��������״̬��nTrainState--0:δ����; 1:������; 2:��ͨ��(�Ѿ���ȡ�꽱��); 3:�����(û����ȡ����)��
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveOpenTrainState(DWORD dwRoleID, DWORD dwTrainID, INT nTrainState, DWORD dwCompleteness)
{
	if (nTrainState < 0 || nTrainState >3)
		return;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	pStream->Clear();
	pStream->SetReplace("train_state");
	pStream->FillString("TrainState=") << nTrainState;
	pStream->FillString(",Completeness=") << dwCompleteness;
	pStream->FillString(",RoleID=") << dwRoleID;
	pStream->FillString(",TrainID=") <<dwTrainID;

	// ִ��
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
}

VOID CLoongDB::SaveAvatarFaceAndHair(DWORD dwRoleID,
						   WORD wHairMdlID,			// ����
						   WORD wHairTexID,			// ��ɫ
						   WORD wFaceMdlID,			// �沿���
						   WORD wFaceDetailTexID)	// �沿ϸ��
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	pStream->Clear();
	pStream->FillString("update roledata set HairModelID=") << wHairMdlID;
	pStream->FillString(",HairColorID=") << wHairTexID;
	pStream->FillString(",FaceModelID=") << wFaceMdlID;
	pStream->FillString(",FaceDetailID=") << wFaceDetailTexID;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// ִ��
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
}

// BOOL CLoongDB::SaveBoardQuest(DWORD dwID, LPVOID pData, INT32 nNum, LPVOID* ppOutPointer)
// {
// 	if ( nNum < QUEST_BOARD_MIN_CAPACITY || nNum > QUEST_BOARD_MAX_CAPACITY )
// 	{
// 		return FALSE;
// 	}
// 
// 	Beton::MyStream* pStream = m_pDBLoong->GetStream();
// 	if(!P_VALID(pStream)) return FALSE;
// 
// 	tagBoardQuestSaveData *pCurPointer = (tagBoardQuestSaveData*)pData;
// 
// 	for(INT32 i=0; i<nNum; ++i)
// 	{
// 		if ( !P_VALID(pCurPointer->u16BoardID) || !P_VALID(pCurPointer->u16QuestID) 
// 			|| pCurPointer->u16QuestID > QUEST_PANEL_END_TASK_ID
// 			|| pCurPointer->u16QuestID < QUEST_PANEL_START_TASK_ID 
// 			|| pCurPointer->byType <= EQBT_Null
// 			|| pCurPointer->byType > EQBT_Guild)
// 		{
// 			continue;
// 		}
// 
// 		pStream->Clear();
// 		pStream->SetReplace("quest_board");
// 		pStream->FillString("BoardID=") << pCurPointer->u16BoardID;
// 		pStream->FillString(",State=") << pCurPointer->eState;
// 		pStream->FillString(",RoleID=") << dwID;
// 		pStream->FillString(",QuestID=") << pCurPointer->u16QuestID;
// 		pStream->FillString(",BoardType=") << pCurPointer->byType;
// 
// 		// ִ��
// 		m_pDBLoong->Execute(pStream);
// 
// 		++pCurPointer;
// 	}
// 
// 	*ppOutPointer = pCurPointer;
// 
// 	// �ͷ���
// 	m_pDBLoong->ReturnStream(pStream);
// 
// 	return TRUE;
// }

BOOL CLoongDB::InsertGuildWarDeclare(DWORD			dwAttackGuildID,			// ��ս����ID
									  DWORD			dwDefenceGuildID,			// ����ս����ID
									  tagDWORDTime	dwWarDeclareTime,			// ��սʱ��
									  BOOL			bWarEnded)					// �ǲ����Ѿ���������
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;
	INT nWarEnded = bWarEnded?1:0;

	pStream->Clear();
	pStream->SetInsert("guild_war_declare");
	pStream->FillString("AttackGuildID=") << dwAttackGuildID;
	pStream->FillString(",DefenceGuildID=") << dwDefenceGuildID;
	pStream->FillString(",WarDeclareTime=") << dwWarDeclareTime;
	pStream->FillString(",WarEnded=") << nWarEnded;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SetGuildWarDeclareEnded(DWORD		dwAttackGuildID,			// ��ս����ID
									   DWORD		dwDefenceGuildID,			// ����ս����ID
									   tagDWORDTime	dwWarDeclareTime)			// ��սʱ��
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetUpdate("guild_war_declare");
	pStream->FillString("WarEnded=") << 1;
	pStream->SetWhere();
	pStream->FillString("AttackGuildID=") << dwAttackGuildID;
	pStream->FillString(" and DefenceGuildID=") << dwDefenceGuildID;
	pStream->FillString(" and WarDeclareTime=") << dwWarDeclareTime;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::InsertIntoAttackSignUpList(EGodMiracle eGodID, DWORD dwGuildID, DWORD dwBidFund)
{
	// �����Ϸ��Լ��һ��
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetInsert("battlefield_attack_signup");
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(",GuildID=") << dwGuildID;
	pStream->FillString(",BidFund=") << dwBidFund;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::InsertIntoAllianceList(EGodMiracle eGodID, DWORD dwGuildID, EGuildCapType eAllianceType, BOOL bConfirmed)
{
	// �����Ϸ��Լ��һ��
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;
	if (eAllianceType != EGCT_Defend && eAllianceType != EGCT_Capture)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	DWORD dwBeConfirmed = (bConfirmed==FALSE)?0:1;
	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetInsert("battlefield_alliance_signup");
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(",GuildID=") << dwGuildID;
	pStream->FillString(",GuildCapType=") << static_cast<DWORD>(eAllianceType);
	pStream->FillString(",BeConfirmed=") << dwBeConfirmed;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::ConfirmAlliance(EGodMiracle eGodID, DWORD dwGuildID)
{
	// �����Ϸ��Լ��һ��
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetUpdate("battlefield_alliance_signup");
	pStream->FillString("BeConfirmed=") << 1;
	pStream->SetWhere();
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(" and GuildID=") << dwGuildID;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::ModifyBidFund(EGodMiracle eGodID, DWORD dwGuildID, DWORD dwBidFund)
{
	// �����Ϸ��Լ��һ��
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetUpdate("battlefield_attack_signup");
	pStream->FillString("BidFund=") << dwBidFund;
	pStream->SetWhere();
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(" and GuildID=") << dwGuildID;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveOnlineTimeEachMonth(DWORD dwAccountID, DWORD dwRoleID, BYTE byYear, UINT16* pn16OnlineTime)
{
	if (!P_VALID(pn16OnlineTime))
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;
	pStream->Clear();
	pStream->SetReplace("role_online_time_each_month");
	pStream->FillString("AccountID=")<<dwAccountID;
	pStream->FillString(",RoleID=")<<dwRoleID;
	pStream->FillString(",Year=")<<byYear;
	pStream->FillString(",January=")<<pn16OnlineTime[EM_January];
	pStream->FillString(",February=")<<pn16OnlineTime[EM_February];
	pStream->FillString(",March=")<<pn16OnlineTime[EM_March];
	pStream->FillString(",April=")<<pn16OnlineTime[EM_April];
	pStream->FillString(",May=")<<pn16OnlineTime[EM_May];
	pStream->FillString(",June=")<<pn16OnlineTime[EM_June];
	pStream->FillString(",July=")<<pn16OnlineTime[EM_July];
	pStream->FillString(",August=")<<pn16OnlineTime[EM_August];
	pStream->FillString(",September=")<<pn16OnlineTime[EM_September];
	pStream->FillString(",October=")<<pn16OnlineTime[EM_October];
	pStream->FillString(",November=")<<pn16OnlineTime[EM_November];
	pStream->FillString(",December=")<<pn16OnlineTime[EM_December];
	pStream->FillString(",LastUpdateTime=now()");

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::QuestBoardSaveAllQuest(LPVOID pData, INT32 nNum)
{
	if ( nNum < QUEST_BOARD_MIN_CAPACITY || nNum > QUEST_BOARD_MAX_CAPACITY )
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	tagBoardQuestSaveData *pCurPointer = (tagBoardQuestSaveData*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		if ( !P_VALID(pCurPointer->u16BoardID) || !P_VALID(pCurPointer->u16QuestID) 
			|| pCurPointer->u16QuestID > QUEST_PANEL_END_TASK_ID
			|| pCurPointer->u16QuestID < QUEST_PANEL_START_TASK_ID
 			|| pCurPointer->byType <= EQBT_Null
 			|| pCurPointer->byType > EQBT_Guild)
		{
			continue;
		}

		pStream->Clear();
		pStream->SetReplace("quest_board");
		pStream->FillString("BoardID=") << pCurPointer->u16BoardID;
		pStream->FillString(",State=") << pCurPointer->eState;
		pStream->FillString(",RoleID=") << pCurPointer->dwRoleID;
		pStream->FillString(",QuestID=") << pCurPointer->u16QuestID;
		pStream->FillString(",BoardType=") << pCurPointer->byType;

		// ִ��
		m_pDBLoong->Execute(pStream);

		++pCurPointer;
	}

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

DWORD CLoongDB::QuestBoardSaveOneQuest(LPVOID pData)
{
	tagBoardQuestSaveData *pCurPointer = (tagBoardQuestSaveData*)pData;
	if ( !P_VALID(pCurPointer->u16BoardID) || !P_VALID(pCurPointer->u16QuestID) 
		|| pCurPointer->u16QuestID > QUEST_PANEL_END_TASK_ID
		|| pCurPointer->u16QuestID < QUEST_PANEL_START_TASK_ID
		|| pCurPointer->byType <= EQBT_Null
		|| pCurPointer->byType > EQBT_Guild)
	{
		return FALSE;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetReplace("quest_board");
	pStream->FillString("BoardID=") << pCurPointer->u16BoardID;
	pStream->FillString(",State=") << pCurPointer->eState;
	pStream->FillString(",RoleID=") << pCurPointer->dwRoleID;
	pStream->FillString(",QuestID=") << pCurPointer->u16QuestID;
	pStream->FillString(",BoardType=") << pCurPointer->byType;

	// ִ��
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

DWORD CLoongDB::SyncCanRankFlag(DWORD dwRoleID, BYTE byCanRankFlag)
{
	if (byCanRankFlag != 0 && byCanRankFlag != 1)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetUpdate("roledata");
	pStream->FillString("CanRankFlag=") << byCanRankFlag;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// ִ��
	m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

DWORD CLoongDB::SaveMediaOnlineNum(DWORD num,VOID * pData)
{
	if( num <= 0 || !P_VALID(pData))
		return GT_INVALID;

	tagMediaOnlineNum * pOlData = (tagMediaOnlineNum*)pData;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		while( num-- > 0 )
		{
			pStream->Clear();
			pStream->FillString  ("REPLACE mediaonline SET DistributionID = ") << pOlData->nDistributionID;
			pStream->FillString(",OnlineNum = ") << pOlData->dwOnlineNum;
			pStream->FillString(" ,RecTime = now()") ;
			m_pDBLoong->Execute  ( pStream ) ;
			++pOlData;
		}
		m_pDBLoong->ReturnStream(pStream);
		return 0;
	}
	return GT_INVALID;
}

DWORD CLoongDB::UpdateFabaoMood(DWORD pDBMsg,DWORD dwDummy)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_FabaoMoodChanged * pCmd = (tagNDBC_FabaoMoodChanged*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("UPDATE fabao SET Mood = ") << pCmd->nMood;
		pStream->FillString(",MoodTicks = ") << pCmd->nMoodTicks;
		pStream->FillString(" WHERE SerialNum = ") << pCmd->n64SerialID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

DWORD CLoongDB::UpdateHolyCoValue(DWORD pDBMsg,DWORD dwDummy)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_HolyCoValue * pCmd = (tagNDBC_HolyCoValue*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("UPDATE holyman SET CoValue = ") << pCmd->nCoValue;		
		pStream->FillString(" WHERE SerialNum = ") << pCmd->n64SerialID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

DWORD	CLoongDB::UpdateFabaoEleInjuryType(DWORD pDBMsg,DWORD dwDummy)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_UpdateFabaoEleInjuryType * pCmd = (tagNDBC_UpdateFabaoEleInjuryType*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("UPDATE fabao SET EleInjuryType = ") << pCmd->eInjury;
		pStream->FillString(" WHERE SerialNum = ") << pCmd->n64FabaoID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

DWORD CLoongDB::UpdateFabaoNirVanaData(DWORD pDBMsg,DWORD)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_UpdateFabaoNirVanaData * pCmd = (tagNDBC_UpdateFabaoNirVanaData*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("Replace fabao_extend_his SET SerialNum = ") << pCmd->_n64ID;
		pStream->FillString(" ,Stage = ") << pCmd->_data.nStage;
		pStream->FillString(" ,EleInjuryType = ") << pCmd->_data.nEleInjuryType;
		pStream->FillString(" ,EleInjury = ") << pCmd->_data.nEleInjury;
		pStream->FillString(" ,ElePierce = ") << pCmd->_data.nElePierce;
		pStream->FillString(" ,EleResisFire = ") << pCmd->_data.nEleResistance[EER_Fire];
		pStream->FillString(" ,EleResisWater = ") << pCmd->_data.nEleResistance[EER_Water];
		pStream->FillString(" ,EleResisEarth = ") << pCmd->_data.nEleResistance[EER_Earth];
		pStream->FillString(" ,EleResisWind = ") << pCmd->_data.nEleResistance[EER_Wind];
		pStream->FillString(" ,EleResisBright = ") << pCmd->_data.nEleResistance[EER_Bright];
		pStream->FillString(" ,EleResisDark = ") << pCmd->_data.nEleResistance[EER_Dark];
		pStream->FillString(" ,DamageUpgradeTimes = ") << pCmd->_data.wDamageUpgradeTimes;
		pStream->FillString(" ,ResistanceUpgradeTimes = ") << pCmd->_data.wResistanceUpgradeTimes;
		pStream->FillString(" ,EleInjPromoteTimes = ") << pCmd->_data.wEleInjPromoteTimes;
		pStream->FillString(" ,EleResPromoteTimes = ") << pCmd->_data.wEleResPromoteTimes;
		pStream->FillString(",RoleID = ") << pCmd->dwRoleID;
		pStream->FillString(",AccountID = ") << pCmd->dwAccountID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

VOID CLoongDB::InitFabaoUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagFabao * pFabao)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO fabao SET SerialNum=") << pFabao->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE fabao SET ") ;
	InitFabaoSpecUpdate(pStream,pCon,(tagFabaoSpec *)pFabao);
}

VOID CLoongDB::InitFabaoSpecUpdate(Beton::MyStream *pStream,Beton::Connection* pCon,const tagFabaoSpec * pFabao)
{
	pStream->FillString  (" NativeIntelligence=") << pFabao->n16NativeIntelligence;
	pStream->FillString  (", Stage=") << pFabao->n16Stage;
	pStream->FillString  (", PracticeValue=") << pFabao->nPracticeValue;
	DWORD dwStoredExpOpen = (pFabao->bStoredExpOpen == TRUE)?1:0;
	pStream->FillString  (", StoredExpOpen=") << dwStoredExpOpen;
	pStream->FillString  (", CurStoredExp=") << pFabao->nCurStoredExp;
	pStream->FillString  (", AttTypes='") ;
	pStream->FillBlob((LPVOID)pFabao->eAttType, sizeof(pFabao->eAttType),pCon);
	pStream->FillString  ("', AttGrowings='") ;
	pStream->FillBlob((LPVOID)pFabao->nAttGrowing, sizeof(pFabao->nAttGrowing),pCon);
	pStream->FillString  ("', WuxingType=") << pFabao->eWuxing ;

	pStream->FillString  (", WuxingHoles='") ;
	pStream->FillBlob((LPVOID)pFabao->eWuxingHole, sizeof(pFabao->eWuxingHole),pCon);
	pStream->FillString  ("', WuxingBeadIDs='") ;
	pStream->FillBlob((LPVOID)pFabao->dwWuxingBeadID, sizeof(pFabao->dwWuxingBeadID),pCon);

	pStream->FillString  ("', SlotAttTypes='") ;
	pStream->FillBlob((LPVOID)pFabao->eSlotAttType, sizeof(pFabao->eSlotAttType),pCon);
	pStream->FillString  ("', SlotAttValue='") ;
	pStream->FillBlob((LPVOID)pFabao->nSlotAttValue, sizeof(pFabao->nSlotAttValue),pCon);

	//pStream->FillString  ("', WuxingMatchReward='") ;
	//pStream->FillBlob((LPVOID)pFabao->nWuxingMatchReward, sizeof(pFabao->nWuxingMatchReward),pCon);
	pStream->FillString  ("', UseLevel=") << pFabao->nUseLevel;
	pStream->FillString(", SlotNum=") << pFabao->nSlotNum;

	pStream->FillString  (", AttGrowings0='") ;
	pStream->FillBlob((LPVOID)pFabao->nAttGrowing0, sizeof(pFabao->nAttGrowing0),pCon);
	pStream->FillString  ("'") ;

	pStream->FillString  (", SlotLingzhuMatch=") << pFabao->bySlotLingzhuMatch;
	pStream->FillString  (", SlotMacthNdx='") ;
	pStream->FillBlob((LPVOID)pFabao->bySlotMacthNdx, sizeof(pFabao->bySlotMacthNdx),pCon);
	pStream->FillString  ("'") ;

	// 1.2.1 ��������
	pStream->FillString(", Mood=") << pFabao->nMood;
	pStream->FillString(", MoodTicks=") << pFabao->nMoodTicks;
	pStream->FillString(", MoodBuffID=") << pFabao->dwMoodBuffID;

	// 1.2.2 ����Ԫ���˺��Լ����ԣ��ؼ���
	pStream->FillString(",EleInjuryType=") << pFabao->eEleInjury;
	pStream->FillString(",EleInjury=") << pFabao->nEleInjury;
	pStream->FillString(",ElePierce=") << pFabao->nElePierce;
	pStream->FillString(",EleInjPromoteTimes=") << pFabao->wEleInjPromoteTimes;
	pStream->FillString(",EleResPromoteTimes=") << pFabao->wEleResPromoteTimes;
	pStream->FillString(",EleResisFir=") << pFabao->nEleResistance[EER_Fire];
	pStream->FillString(",EleResisWater=") << pFabao->nEleResistance[EER_Water];
	pStream->FillString(",EleResisEarth=") << pFabao->nEleResistance[EER_Earth];
	pStream->FillString(",EleResisWind=") << pFabao->nEleResistance[EER_Wind];
	pStream->FillString(",EleResisBright=") << pFabao->nEleResistance[EER_Bright];
	pStream->FillString(",EleResisDark=") << pFabao->nEleResistance[EER_Dark];
	pStream->FillString(",DamageUpgradeTimes=") << pFabao->wDamageUpgradeTimes;
	pStream->FillString(",ResistanceUpgradeTimes=") << pFabao->wResistanceUpgradeTimes;
	pStream->FillString(",FairySkill1=") << pFabao->dwFairySkill[0];
	pStream->FillString(",FairySkill2=") << pFabao->dwFairySkill[1];
	pStream->FillString(",FairySkill3=") << pFabao->dwFairySkill[2];
	pStream->FillString(",FairySkill4=") << pFabao->dwFairySkill[3];
	pStream->FillString(",FairySkill5=") << pFabao->dwFairySkill[4];
	pStream->FillString(",UnsureSkill=") << pFabao->dwUnsureSkill;

	// 1.3.0 Jason 2010-12-6 ������ֳ�������
	pStream->FillString(",FabaoStatus=") << pFabao->eFabaoStatus;
	pStream->FillString(",StatusChangeTime=") << pFabao->dwStatusChangeTime;
	pStream->FillString(",BirthValue=") << pFabao->nBirthValue;
	pStream->FillString(",BirthTimes=") << pFabao->nBirthTimes;
	pStream->FillString(",IsGivenBirthTimes=") << pFabao->byIsGivenBirthTimes;

	pStream->FillString  (", LevelBirth='") ;
	pStream->FillBlob((LPVOID)pFabao->byBirthLevel, sizeof(pFabao->byBirthLevel),pCon);
	pStream->FillString  ("'") ;
	pStream->FillString(",MaxIntelligence=") << pFabao->byMaxIntelligence;
}

// wcy 2012-03-31 ��ʼ������ʥ��sql
VOID CLoongDB::InitHolyUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolyMan * pHoly)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO holyman SET SerialNum=") << pHoly->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE holyman SET ") ;
	InitHolySpecUpdate(pStream,pCon,(tagHolySpec *)pHoly);
}

// wcy 2012-04-18 ��ʼ������ʥ��sql
VOID CLoongDB::InitHolyEquipUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolyEquip * pHolyEquip)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO holyequip SET SerialNum=") << pHolyEquip->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE holyequip SET ") ;
	InitHolyEquipSpecUpdate(pStream,pCon,(tagHolyEquipSpec *)pHolyEquip);
}

VOID CLoongDB::InitHolySpecUpdate(Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolySpec * pHoly)
{
	pStream->FillString  (" DevourNum=") << pHoly->nDevourNum;
	pStream->FillString  (" ,EquipmentNumber=") << pHoly->n16EquipmentNumber;
	pStream->FillString  (" ,ToDayDevourNum=") << pHoly->nToDayEatNum;
	pStream->FillString  (" , EquipSerialIDs='") ;
	pStream->FillBlob((LPVOID)pHoly->n64EquipSerialIDs, sizeof(pHoly->n64EquipSerialIDs),pCon);
	pStream->FillString  ("',CoValue=") << pHoly->nCoValue;
	pStream->FillString  (" ,CostHoly=") << pHoly->nCostHoly;
	pStream->FillString  (" ,HolyDmg=") << pHoly->dwLevelUpAtt[ESAT_Demage];
	pStream->FillString  (" ,HolyDef=") << pHoly->dwLevelUpAtt[ESAT_HolyDef];
	pStream->FillString  (" ,Crit=") << pHoly->dwLevelUpAtt[ESAT_Crit];
	pStream->FillString  (" ,HolyCritRate=") << pHoly->dwLevelUpAtt[ESAT_CritRate];
	pStream->FillString  (" ,ExDamage=") << pHoly->dwLevelUpAtt[ESAT_ExDmg];
	pStream->FillString  (" ,AttackTec=") << pHoly->dwLevelUpAtt[ESAT_AttackTec];
	pStream->FillString  (" ,NeglectToughness=") << pHoly->dwLevelUpAtt[ESAT_NeglectToughness];
	pStream->FillString  (" ,HolyValue=") << pHoly->dwLevelUpAtt[ESAT_LingNeng];
	pStream->FillString  (" ,MaxDevourNum=") << pHoly->nMaxDevourNum;
}

VOID CLoongDB::InitHolyEquipSpecUpdate(Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolyEquipSpec * pHolyEquip)
{
	pStream->FillString  (" CostHoly=") << pHolyEquip->nCostHoly;
	pStream->FillString  (" ,EnhanceCount=") << pHolyEquip->n16EnhanceCount;	
	pStream->FillString  (" ,HolyDmgChg=") << pHolyEquip->nHolyAttEffect[ESAT_Demage];
	pStream->FillString  (" ,HolyDefChg=") << pHolyEquip->nHolyAttEffect[ESAT_HolyDef];
	pStream->FillString  (" ,CritChg=") << pHolyEquip->nHolyAttEffect[ESAT_Crit];
	pStream->FillString  (" ,HolyCritRateChg=") << pHolyEquip->nHolyAttEffect[ESAT_CritRate];
	pStream->FillString  (" ,ExDamageChg=") << pHolyEquip->nHolyAttEffect[ESAT_ExDmg];
	pStream->FillString  (" ,AttackTecChg=") << pHolyEquip->nHolyAttEffect[ESAT_AttackTec];
	pStream->FillString  (" ,NeglectToughnessChg=") << pHolyEquip->nHolyAttEffect[ESAT_NeglectToughness];	
}


DWORD CLoongDB::SaveOneGodBidInfo(DWORD dwRoleID, INT64 n64Bidden, BYTE byBidType, DWORD dwBidID)
{
	if (byBidType >= EGBT_End || byBidType == EGBT_Start)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetReplace("god_bid");
	pStream->FillString("RoleID=")<<dwRoleID;
	pStream->FillString(",Bidden=")<<n64Bidden;
	pStream->FillString(",BidType=")<<byBidType;
	pStream->FillString(",BidID=")<<dwBidID;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::UpdateFabaoGivenBirthTimes(INT64 n64FabaoID,INT nTimes)
{
	if (nTimes < 1)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->FillString("UPDATE fabao SET IsGivenBirthTimes =1,BirthTimes=")<<nTimes;
	pStream->FillString(" WHERE SerialNum=")<<n64FabaoID;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveEggInfo(DWORD dwRoleID, INT nCurLayer, INT16 n16ColourEggPos1, INT16 n16ColourEggPos2, tagEggInfo* pEggInfo)
{
	if (!P_VALID(pEggInfo))
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetReplace("egg_broker");
	pStream->FillString("RoleID=")<<dwRoleID;
	pStream->FillString(",CurLayer=")<<nCurLayer;
	pStream->FillString(",ColourEggPos1=")<<n16ColourEggPos1;
	pStream->FillString(",ColourEggPos2=")<<n16ColourEggPos2;
	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		char szItemID[100] = {0};
		char szBrokePos[100] = {0};
		sprintf(szItemID, ",ItemID%d=%d", nCnt+1, pEggInfo[nCnt].dwItemID);
		sprintf(szBrokePos, ",BrokePos%d=%d", nCnt+1, pEggInfo[nCnt].nBrokePos);
		pStream->FillString((const char*)szItemID);
		pStream->FillString((const char*)szBrokePos);
	}

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SaveWardrobeLayerInfo(tagWardrobeLayer2DB& WardrobeLayerInfo)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;
	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetReplace("wardrobe");
	pStream->FillString("RoleID=")<<WardrobeLayerInfo.dwRoleID;
	pStream->FillString(",LayerID=")<<WardrobeLayerInfo.dwLayerID;
	pStream->FillString(",Level=")<<WardrobeLayerInfo.byLevel;
	pStream->FillString(",State=")<<WardrobeLayerInfo.byState;
	pStream->FillString(",PlayActStartTime=")<<WardrobeLayerInfo.dwPlayActStartTime;
	pStream->FillString(",PlayActFriendID=")<<WardrobeLayerInfo.dwFriendID;

	// ִ��
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
BOOL CLoongDB::SaveArea(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagAreaSave), (&CLoongDB::FormatSaveArea), ppOutPointer);
}
//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
VOID CLoongDB::FormatSaveArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagAreaSave);

	pStream->SetReplace("area");
	pStream->FillString("AreaID=") << p->dwAreaID;
	pStream->FillString(",RoleID=") << dwRoleID;
	pStream->FillString(",Activation=") << p->bActivation;
	pStream->FillString(",CDTime=") << p->dwCD;
}
//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
BOOL CLoongDB::DelArea(DWORD dwRoleID, DWORD dwAreaID)
{
	return Update(dwRoleID, &dwAreaID, 1, sizeof(DWORD), (&CLoongDB::FormatDelArea));
}
//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
VOID CLoongDB::FormatDelArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetDelete("area");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AreaID=") << (*(DWORD*)pData);
}
//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
BOOL CLoongDB::InsertArea(DWORD dwRoleID, tagAreaSave *pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagAreaSave), (&CLoongDB::FormatInsertArea));
}
//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ
VOID CLoongDB::FormatInsertArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagAreaSave);

	pStream->SetInsert("area");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",AreaID=") << p->dwAreaID;
	pStream->FillString(",Activation=") << p->bActivation;
	pStream->FillString(",CDTime=") << p->dwCD;
}
//-------------------------------------------------------------------------------------------------------




BOOL CLoongDB::LogItemDelTime(Beton::MyStream* pStream, const char* szTableName, const INT64 n64SerialNum[], INT nNum)
{
	CHAR szTime[X_DATATIME_LEN+1] = {0};
	DwordTime2DataTime(szTime, sizeof(szTime), GetCurrentDWORDTime());

	BOOL bRet = TRUE;
	for (int i=0;i<nNum;++i)
	{
		pStream->Clear();

		pStream->SetUpdate(szTableName);
		pStream->FillString("CreateTime='").FillString(szTime).FillString("'");
		pStream->SetWhere();
		pStream->FillString("SerialNum=") << n64SerialNum[i];

		bRet = bRet && m_pDBLoong->Execute(pStream);
	}

	return bRet;
}

//-------------------------------------------------------------------------------
BOOL CLoongDB::DelItemDel( const CHAR* pszTime )
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// ɾ������
	pStream->SetDelete("item_del");
	pStream->SetWhere();
	pStream->FillString("del_time < '").FillString(pszTime).FillString("'");

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}


BOOL CLoongDB::DelEquipDel( const CHAR* pszTime )
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// ɾ������
	pStream->SetDelete("equip_del");
	pStream->SetWhere();
	pStream->FillString("deltime < '").FillString(pszTime).FillString("'");

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//---------------------------------------------------------
BOOL CLoongDB::UpdateDelEquipTime(INT64 n64Serial, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ���²���
	pStream->SetUpdate("equip");
	pStream->FillString("deltime='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::UpdateDelItemTime(INT64 n64Serial, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ���²���
	pStream->SetUpdate("item");
	pStream->FillString("del_time='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::UpdateDelEquipTime(DWORD dwRoleID, DWORD dwTypeID, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	INT64 n64Serial = GetDelEquip64Serial(dwRoleID, dwTypeID);
	if (n64Serial == 0)
	{
		return FALSE;
	}

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// ���²���
	pStream->SetUpdate("equip");
	pStream->FillString("deltime='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::UpdateDelItemTime(DWORD dwRoleID, DWORD dwTypeID, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();


	// ���²���
	pStream->SetUpdate("item");
	pStream->FillString("del_time='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("TypeID=") << dwTypeID;
	pStream->FillString(" and OwnerID=") << dwRoleID;
	pStream->SetLimit(1);

	// ִ��
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::SaveBuyMallItemCount(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagBuyMallItemCountSaveData), (&CLoongDB::FormatSaveBuyMallItemCount), ppOutPointer);
}

// �����ɫ������Ϣ
VOID CLoongDB::FormatSaveBuyMallItemCount(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagBuyMallItemCountSaveData);

	pStream->SetReplace("role_buy_mall_item_count");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",MallItemID=") << p->dwTypeID;
	pStream->FillString(",Num=") << p->nNum;
}

BOOL CLoongDB::SaveAccountSpec(DWORD dwAccountID, EAccountSpec eSpec)
{
	return Update(dwAccountID, &eSpec, 1, 0, (&CLoongDB::FormatSaveAccountSpec));
}

VOID CLoongDB::FormatSaveAccountSpec(Beton::MyStream* pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->Clear();
	pStream->SetUpdate("account_common");
	pStream->FillString("EMacc=") << INT(*(EAccountSpec*)pData);
	pStream->SetWhere();
	pStream->FillString("accountid=") << dwAccountID;
}

//������ȡ��Key����Ʒ�Ľ�ɫid
BOOL CLoongDB::InsertUseKeyCodeRoleID(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertUseKeyCodeRoleID));
}
//������ȡ��Key����Ʒ�Ľ�ɫid
VOID CLoongDB::FormatInsertUseKeyCodeRoleID(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->Clear();
	pStream->SetInsert("UseKeyCode_roleID");
	pStream->FillString("RoleID=") << dwRoleID;
}
#include "StdAfx.h"

#include "wx/wx.h"
#include "servermergetoolapp.h"
#include "Loong_OutputCtrl.h"
#include "Loong_LoongDBCtrl.h"
#include "../WorldDefine/time.h"

template<class T>
VOID TMethod_VecDateDel(T &vec)
{
	for(UINT i=0; i<vec.size(); i++)
	{
		SAFE_DEL(vec[i]);
	}
	vec.swap(T());
}

CLoongLoongDBCtrl::CLoongLoongDBCtrl(void)
{
}
//-------------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::Init(tagConfigDetail stConfig[EM_DB_TYPE_END])
{
	//��ʼ��LoongDB
	for(int iLoop=EM_DB_SRC_ONE;iLoop<EM_DB_TYPE_END;iLoop++)
	{
		BOOL bRet = m_LoongDB[iLoop].Init(stConfig[iLoop].strIp.c_str(),
			stConfig[iLoop].strUser.c_str(),
			stConfig[iLoop].strPsd.c_str(),
			stConfig[iLoop].strDBName.c_str(),
			stConfig[iLoop].dwPort);
		if(bRet == FALSE)
		{
			tstring strErr;
			strErr += stConfig[iLoop].strIp;
			strErr += _T("		");
			strErr += stConfig[iLoop].strUser;
			strErr += _T("		");
			strErr += stConfig[iLoop].strPsd;
			strErr += _T("		");
			strErr += stConfig[iLoop].strDBName;
			strErr += _T("		");
			strErr += stConfig[iLoop].dwPort;
			strErr += _T("��ʼ��ʧ�ܣ�");

			ILOG->Write(strErr.c_str());
			return FALSE;
		}
	}

	// ���ñ����ص�����
	for(int iLoop=EM_DB_SRC_ONE;iLoop<EM_DB_TYPE_END;iLoop++)
	{
		m_LoongDB[iLoop].SetWarningCallBack((WARNINGCALLBACK)LoongDBCallBack);
	}
	return TRUE;
}
//-------------------------------------------------------------------------------
// ���ٺ���
//-------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::Destroy()
{
	for(int iLoop=EM_DB_SRC_ONE;iLoop<EM_DB_TYPE_END;iLoop++)
	{
		m_LoongDB[iLoop].ShutDown();
	}
}
BOOL CLoongLoongDBCtrl::Load(OUT LPVOID pData,EDBType eDBType, 
							 Fun_pFormat pFunFormat, Fun_pProcRes pFunProcRes)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_LoongDB[eDBType].GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ����ѯ�Ľ�ɫ����
	(this->*pFunFormat)(pStream,eDBType);

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// ���鲢�����ѯ���
	if (pResult->GetRowCount() > 0)
	{
		(this->*pFunProcRes)(pData,pResult);
	}
Exit:
	// �ͷ����ͽ��
	m_LoongDB[eDBType].ReturnStream(pStream);
	m_LoongDB[eDBType].FreeQueryResult(pResult);

	return bRet;
}
//-------------------------------------------------------------------------------------------------------
// ���û�id load����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::AccountLoad(OUT LPVOID pData,DWORD dwAccountID,EDBType eDBType,ETBQueryExtraOption eEOption,
										Fun_pAccountFormat pFunFormat, Fun_pProcRes pFunProcRes)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_LoongDB[eDBType].GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ����ѯ�Ľ�ɫ����
	(this->*pFunFormat)(pStream,dwAccountID,eDBType,eEOption);

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// ���鲢�����ѯ���
	if (pResult->GetRowCount() > 0)
	{
		(this->*pFunProcRes)(pData,pResult);
	}
Exit:
	// �ͷ����ͽ��
	m_LoongDB[eDBType].ReturnStream(pStream);
	m_LoongDB[eDBType].FreeQueryResult(pResult);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ���û�n64Limit ��ҳload����
//-------------------------------------------------------------------------------------------------------
EPagingState CLoongLoongDBCtrl::PagingLoad(OUT LPVOID pData,INT64 &n64Limit,EDBType eDBType,
									Fun_pPagingFormat pFunFormat, Fun_pProcRes pFunProcRes)
{
	EPagingState eReturnVal = EPS_DB_NULL;

	// ��ȡ��
	Beton::MyStream* pStream = m_LoongDB[eDBType].GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ����ѯ�Ľ�ɫ����
	(this->*pFunFormat)(pStream,n64Limit,eDBType);

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);
	if(!P_VALID(pResult))
	{
		eReturnVal = EPS_DB_ERROR;
		goto Exit;
	}

	// ���鲢�����ѯ���
	if (pResult->GetRowCount() > 0)
	{
		(this->*pFunProcRes)(pData,pResult);
	}
	else
	{		
		eReturnVal = EPS_DB_RESULT_NULL;
		goto Exit;
	}
Exit:
	// �ͷ����ͽ��
	m_LoongDB[eDBType].ReturnStream(pStream);
	m_LoongDB[eDBType].FreeQueryResult(pResult);

	return eReturnVal;
}


//-------------------------------------------------------------------------------------------------------
// �������ݿ����ģ�淽��(�޽����,�Ҳ���Ҫ���ݿ�����)
//-------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::Insert(LPVOID pData, EDBType eDBType,Fun_pFormat3 pFunFormat)
{
	ASSERT(pData);
	if(pData == NULL)return FALSE;
	if(eDBType>EM_DB_TYPE_END)return FALSE;

	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_LoongDB[eDBType].GetStream();
	ASSERT(P_VALID(pStream));
	
	// ��ʽ�����ݿ�������,��ִ��
	bRet = (this->*pFunFormat)(pStream, pData,eDBType);

	// �ͷ���
	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRet;
}
BOOL CLoongLoongDBCtrl::Insert(LPVOID pData, EDBType eDBType,LPCSTR pszTable,Fun_pFormat4 pFunFormat)
{
	ASSERT(pData);
	if(pData == NULL)return FALSE;
	if(eDBType>EM_DB_TYPE_END)return FALSE;

	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_LoongDB[eDBType].GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ�����ݿ�������,��ִ��
	bRet = (this->*pFunFormat)(pStream, pData,pszTable,eDBType);

	// �ͷ���
	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRet;
}
//-------------------------------------------------------------------------------
//�õ�roledata��������к�
//-------------------------------------------------------------------------------
DWORD CLoongLoongDBCtrl::GetRoleDataMaxNum(EDBType eDBType)
{
	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("roledata","max(RoleID)");
	// ��ѯ
	QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0) return GT_INVALID;

	return (*pResult)[0].GetDword();
}
//-------------------------------------------------------------------------------
//�õ�petdata��������к�
//-------------------------------------------------------------------------------
DWORD CLoongLoongDBCtrl::GetPetDataMaxNum(EDBType eDBType)
{
	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect("pet_data","max(pet_id)");
	// ��ѯ
	QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0) return GT_INVALID;

	return (*pResult)[0].GetDword();
}
//-------------------------------------------------------------------------------
//�õ��ڲ���Ʒ������
//-------------------------------------------------------------------------------
INT64 CLoongLoongDBCtrl::GetMaxSerialInternal(EDBType eDBType)
{
	INT64 n64ItemSerial = MIN_ITEM_SERIAL_INTERNAL;

	INT64 nMaxItem			=	MIN_ITEM_SERIAL_INTERNAL;
	INT64 nMaxItemDel		=	MIN_ITEM_SERIAL_INTERNAL;
	INT64 nMaxItemBaibao	=	MIN_ITEM_SERIAL_INTERNAL;

	BOOL bRet = GetMaxItemSerialInTable("item", nMaxItem, eDBType) &&
		GetMaxItemSerialInTable("item_del", nMaxItemDel, eDBType) &&
		GetMaxItemSerialInTable("item_baibao", nMaxItemBaibao, eDBType);

	if( !bRet )
	{
		return n64ItemSerial = GT_INVALID;
	}

	INT64 nMax = max(nMaxItem, nMaxItemDel);
	nMax = max(nMax, nMaxItemBaibao);

	return n64ItemSerial = max(nMax, n64ItemSerial);
}

//-------------------------------------------------------------------------------
//�õ��ⲿ��Ʒ��С���
//-------------------------------------------------------------------------------
INT64 CLoongLoongDBCtrl::GetMinSerialOutter(EDBType eDBType)
{
	INT64 n64ItemSerial = MAX_ITEM_SERIAL_OUTTER;

	INT64 nMinItem			=	MAX_ITEM_SERIAL_OUTTER;
	INT64 nMinItemDel		=	MAX_ITEM_SERIAL_OUTTER;
	INT64 nMinItemBaibao	=	MAX_ITEM_SERIAL_OUTTER;

	BOOL bRet = GetMinItemSerialInTable("item", nMinItem, eDBType, MIN_ITEM_SERIAL_OUTTER) &&
		GetMinItemSerialInTable("item_del", nMinItemDel, eDBType, MIN_ITEM_SERIAL_OUTTER) &&
		GetMinItemSerialInTable("item_baibao", nMinItemBaibao, eDBType, MIN_ITEM_SERIAL_OUTTER);

	if( !bRet )
	{
		return n64ItemSerial = GT_INVALID;
	}

	INT64 nMin = min(nMinItem, nMinItemDel);
	nMin = min(nMin, nMinItemBaibao);

	return n64ItemSerial = min(nMin, n64ItemSerial);

}


//-------------------------------------------------------------------------------
//�õ���Ʒ������
//-------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::GetMaxItemSerialInTable(LPCSTR szTable, INT64& n64ItemSerial,EDBType eDBType,INT64 n64UpLimit)
{
	MyStream* pStream  = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect(szTable, "max(SerialNum)");
	if(n64UpLimit != GT_INVALID)
	{
		pStream->SetWhere();
		pStream->FillString("SerialNum<=") << n64UpLimit;
	}


	QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	if( !P_VALID(pResult) ) return FALSE;

	if( 0 == pResult->GetRowCount() )
	{
		m_LoongDB[eDBType].FreeQueryResult(pResult);
		return TRUE;
	}

	if( (*pResult)[0].IsNull() )
	{
		m_LoongDB[eDBType].FreeQueryResult(pResult);
		return TRUE;
	}

	n64ItemSerial = (*pResult)[0].GetLong();

	m_LoongDB[eDBType].FreeQueryResult(pResult);

	return TRUE;
}

//-------------------------------------------------------------------------------
//�õ�������Ʒ��С���
//-------------------------------------------------------------------------------
INT64 CLoongLoongDBCtrl::GetItemMinSerialBill(EDBType eDBType)
{
	INT64 n64ItemSerial = MAX_ITEM_SERIAL_BILL;

	INT64 nMinItem			=	MAX_ITEM_SERIAL_BILL;
	INT64 nMinItemDel		=	MAX_ITEM_SERIAL_BILL;
	INT64 nMinItemBaibao	=	MAX_ITEM_SERIAL_BILL;

	BOOL bRet = GetMinItemSerialInTable("item", nMinItem, eDBType) &&
		GetMinItemSerialInTable("item_del", nMinItemDel, eDBType) &&
		GetMinItemSerialInTable("item_baibao", nMinItemBaibao, eDBType);

	if( !bRet )
	{
		return n64ItemSerial = GT_INVALID;
	}

	INT64 nMin = min(nMinItem, nMinItemDel);
	nMin = min(nMin, nMinItemBaibao);

	return n64ItemSerial = min(nMin, n64ItemSerial);
}
//---------------------------------------------------------------------------------------
// �õ�ĳ��������С��64λID
//---------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::GetMinItemSerialInTable(LPCSTR szTable, INT64& n64ItemSerial,EDBType eDBType,INT64 n64LowLimit)
{

	MyStream* pStream  = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetSelect(szTable, "min(SerialNum)");
	if(n64LowLimit != GT_INVALID)
	{
		pStream->SetWhere();
		pStream->FillString("SerialNum>=") << n64LowLimit;
	}

	QueryResult* pResult = m_LoongDB[eDBType].Query(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	if( !P_VALID(pResult) ) return FALSE;

	if( 0 == pResult->GetRowCount() )
	{
		m_LoongDB[eDBType].FreeQueryResult(pResult);
		return TRUE;
	}

	if( (*pResult)[0].IsNull() )
	{
		m_LoongDB[eDBType].FreeQueryResult(pResult);
		return TRUE;
	}

	n64ItemSerial = (*pResult)[0].GetLong();

	m_LoongDB[eDBType].FreeQueryResult(pResult);

	return TRUE;
}

// ������Ʒ
BOOL CLoongLoongDBCtrl::CreateItem(INT64 n64SerialNum, DWORD ItemTypdID, DWORD AccountID, DWORD RoleID)
{
	std::vector<tagTWrap_Loong_ItemBaiBao*> vecData;
	tagTWrap_Loong_ItemBaiBao* pItem = new tagTWrap_Loong_ItemBaiBao;

	pItem->stOthers.n64Serial		= n64SerialNum;
	pItem->stOthers.dwTypeID		= ItemTypdID;
	pItem->stOthers.dwAccountID		= AccountID;
	pItem->stOthers.dwCreateTime	= GetCurrentDWORDTime(); 
	pItem->stOthers.eCreateMode		= EICM_ServerMerge;
	pItem->stOthers.eConType		= EICT_Baibao;
	pItem->stOthers.byBind			= EBS_Unknown;
	pItem->stOthers.dw1stGainTime	= GetCurrentDWORDTime();
	pItem->stOthers.n16Num			= 1;
	pItem->stOthers.dwCreateID		= GT_INVALID;
	pItem->stOthers.dwCreatorID		= GT_INVALID;
	pItem->stOthers.dwOwnerID		= RoleID;
	pItem->stOthers.dwNameID		= GT_INVALID;
	pItem->stOthers.nUseTimes		= 0;
	pItem->stOthers.byBind			= 0;
	pItem->stOthers.bLock			= 0;
	pItem->stOthers.n16Index		= 0;

	vecData.push_back(pItem);

	//������Ʒ
	BOOL bRet = Loong_Insert(vecData,EM_DB_TAR);
	if(bRet == FALSE)
	{
		delete pItem;
		return FALSE;
	}

	delete pItem;
	return TRUE;
}


//	��ȡAccountCommon��
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_AccountCommon*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryAccountCommon,&CLoongLoongDBCtrl::ProcResQueryAccountCommon);
}
//	��ȡBlackList��
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_BlackList*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryBlackList,&CLoongLoongDBCtrl::ProcResQueryBlackList);
}
//	��ȡBuff��
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Buff*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryBuff,&CLoongLoongDBCtrl::ProcResQueryBuff);
}
//	����
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ClanData*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryClanData,&CLoongLoongDBCtrl::ProcResQueryClanData);
}
//	enemy
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Enemy*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryEnemy,&CLoongLoongDBCtrl::ProcResQueryEnemy);
}
/*
//	����
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Equip*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryEquip,&CLoongLoongDBCtrl::ProcResQueryEquip);
}
//����ɾ��
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_EquipDel*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryEquipDel,&CLoongLoongDBCtrl::ProcResQueryEquip);
}
*/
//����baibao
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_EquipBaiBao*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryEquipBaiBao,&CLoongLoongDBCtrl::ProcResQueryEquip);
}

//�����䱦�����
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ActClanTreasure*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryActClanTreasure,&CLoongLoongDBCtrl::ProcResQueryActClanTreasure);
}
//����������
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFameHallEnterSnap,&CLoongLoongDBCtrl::ProcResQueryFameHallEnterSnap);
}
//������������ʱ�����
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_RepRstTime*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryRepRstTime,&CLoongLoongDBCtrl::ProcResQueryRepRstTime);
}
//�����б�(����)
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Friend*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFriendSave,&CLoongLoongDBCtrl::ProcResQueryFriendSave);
}
//�Ѻö�
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_FriendshipSave*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFriendshipSave,&CLoongLoongDBCtrl::ProcResQueryFriendshipSave);
}
//�Ź�
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_GroupPurchase*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGroupPurchase,&CLoongLoongDBCtrl::ProcResQueryGroupPurchase);
}
//����
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Guild*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGuild,&CLoongLoongDBCtrl::ProcResQueryGuild);
}
//���ɳ�Ա
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_GuildMem*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGuildMem,&CLoongLoongDBCtrl::ProcResQueryGuildMem);
}
/*
//item
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Item*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryItem,&CLoongLoongDBCtrl::ProcResQueryItem);
}
*/
//item baibao
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ItemBaiBao*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryItemBaiBao,&CLoongLoongDBCtrl::ProcResQueryItem);
}
/*
//item del
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ItemDel*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryItemDel,&CLoongLoongDBCtrl::ProcResQueryItem);
}
*/
//item cd time
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ItemCDTime*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryItemCDTime,&CLoongLoongDBCtrl::ProcResQueryItemCDTime);
}
//item need log
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ItemNeedLog*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryItemNeedLog,&CLoongLoongDBCtrl::ProcResQueryItemNeedLog);
}
//item log baibao
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_LogBaiBao*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryLogBaiBao,&CLoongLoongDBCtrl::ProcResQueryLogBaiBao);
}
//item role del
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_RoleDel*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryRoleDel,&CLoongLoongDBCtrl::ProcResQueryRoleDel);
}
/*
//role data
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_RoleData*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryRoleData,&CLoongLoongDBCtrl::ProcResQueryRoleData);
}

//role data del
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_RoleDataDel*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryRoleDataDel,&CLoongLoongDBCtrl::ProcResQueryRoleData);
}
*/

//task
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Task*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryTask,&CLoongLoongDBCtrl::ProcResQueryTask);
}
//task done
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_TaskDone*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryTaskDone,&CLoongLoongDBCtrl::ProcResQueryTaskDone);
}
//title
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Title*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryTitle,&CLoongLoongDBCtrl::ProcResQueryTitle);
}
//Ԫ��
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_YBAccount*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryYBAccount,&CLoongLoongDBCtrl::ProcResQueryYBAccount);
}
//Ԫ�����׶���
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryYuanBaoOrder,&CLoongLoongDBCtrl::ProcResQueryYuanBaoOrder);
}

//bill_item
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_BillItem*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryBillItem,&CLoongLoongDBCtrl::ProcResQueryBillItem);
}
//bill_yuanbao
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_BillYuanBao*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryBillYuanbao,&CLoongLoongDBCtrl::ProcResQueryBillYuanbao);
}
//bill_yuanbao_log
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryBillYuanbaoLog,&CLoongLoongDBCtrl::ProcResQueryBillYuanbaoLog);
}
//pet_data
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_PetData*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryPetData,&CLoongLoongDBCtrl::ProcResQueryPetData);
}
//pet skill
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_PetSkill*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryPetSkill,&CLoongLoongDBCtrl::ProcResQueryPetSkill);
}
//vip_stall
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_VipStall*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryVipStall,&CLoongLoongDBCtrl::ProcResQueryVipStall);
}
//guild_commodity
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_GuildCommodity*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGuildCommodity,&CLoongLoongDBCtrl::ProcResQueryGuildCommodity);
}
//guild_skill
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_GuildSkill*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGuildSkill,&CLoongLoongDBCtrl::ProcResQueryGuildSkill);
}
//vip_netbar
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_VipNetBar*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryVipBar,&CLoongLoongDBCtrl::ProcResQueryVipBar);
}
//guild_upgrade
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_GuildFacilities*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGuildUpgrade,&CLoongLoongDBCtrl::ProcResQueryGuildUpgrade);
}
//commerce_rank
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_CommerceRank*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryCommerceRank,&CLoongLoongDBCtrl::ProcResQueryCommerceRank);
}
//activity
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ActivityData*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryActivity,&CLoongLoongDBCtrl::ProcResQueryActivity);
}
//left msg
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_LeftMsg*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryLeftMsg,&CLoongLoongDBCtrl::ProcResQueryLeftMsg);
}


//1.3.1����
//�ⲿ����
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ExternalLink*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryExternalLink,&CLoongLoongDBCtrl::ProcResQueryExternalLink);
}
//��¼������¼�ı�  
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_NameHistory*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryNameHistory,&CLoongLoongDBCtrl::ProcResQueryNameHistory);
}
//city 
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_City*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryCity,&CLoongLoongDBCtrl::ProcResQueryCity);
}

//Master 
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Master*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryMaster,&CLoongLoongDBCtrl::ProcResQueryMaster);
}

//short cut bar 
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ShortCutBar*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryShortCutBar,&CLoongLoongDBCtrl::ProcResQueryShortCutBar);
}

//������־
BOOL CLoongLoongDBCtrl::Loong_QueryCentralbillingLog(std::set <string>& setData,EDBType eDBType)
{
	return Load(&setData,eDBType,&CLoongLoongDBCtrl::FormatQueryCentralbillingLog,&CLoongLoongDBCtrl::ProcResQueryCentralbillingLog);
}

//roledataname
BOOL CLoongLoongDBCtrl::Loong_QueryRoleDataName(std::set <tstring>& setData,EDBType eDBType)
{
	return Load(&setData,eDBType,&CLoongLoongDBCtrl::FormatQueryRoleDataName,&CLoongLoongDBCtrl::ProcResQueryRoleDataName);
}

//roledatanamecrc
BOOL CLoongLoongDBCtrl::Loong_QueryRoleDataNameCrc(std::set <DWORD>& setData,EDBType eDBType)
{
	return Load(&setData,eDBType,&CLoongLoongDBCtrl::FormatQueryRoleDataNameCrc,&CLoongLoongDBCtrl::ProcResQueryRoleDataNameCrc);
}

//��ְ��ɫID
BOOL CLoongLoongDBCtrl::Loong_QueryClergyRoleID(std::set <DWORD>& setData,EDBType eDBType)
{
	return Load(&setData,eDBType,&CLoongLoongDBCtrl::FormatQueryClergyRoleID,&CLoongLoongDBCtrl::ProcResQueryRoleDataNameCrc);
}

//ActiveRankData
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ActiveRankData*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryActiveRankData,&CLoongLoongDBCtrl::ProcResQueryActiveRankData);
}

//ForceBreakOutApprentice
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryForceBreakOutApprentice,&CLoongLoongDBCtrl::ProcResQueryForceBreakOutApprentice);
}

//guild_war_declare
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGuildWarDeclare,&CLoongLoongDBCtrl::ProcResQueryGuildWarDeclare);
}

// �³�
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Sheng_Wardrobe*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryWardrobe,&CLoongLoongDBCtrl::ProcResQueryWardrobe);
}

//GmServerData
BOOL CLoongLoongDBCtrl::Loong_Query( std::vector <tagTWrap_Loong_GmServerData*> &vecData, EDBType eDBType )
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryGmServerData,&CLoongLoongDBCtrl::ProcResQueryGmServerData);
}

// F-Plan 1.5.0 account_reactive, account_reactiver
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_AccountReactive*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryAccountReactive,&CLoongLoongDBCtrl::ProcResQueryAccountReactive);
}

BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_AccountReactiver*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryAccountReactiver,&CLoongLoongDBCtrl::ProcResQueryAccountReactiver);
}


// F-Plan 1.5.12  UseKeyCode_roleID
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_UseKeyCode*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryUseKeyCode,&CLoongLoongDBCtrl::ProcResQueryUseKeyCode);

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//���û���ѯitem��
BOOL CLoongLoongDBCtrl::Loong_AccountQuery(std::vector <tagTWrap_Loong_Item*>&vecData,DWORD dwAccountID,EDBType eDBType,ETBQueryExtraOption eEOption)
{
	return AccountLoad(&vecData,dwAccountID,eDBType,eEOption,&CLoongLoongDBCtrl::FormatAccountQueryItem,&CLoongLoongDBCtrl::ProcResQueryItem);
}

//���û���ѯroledata��
BOOL CLoongLoongDBCtrl::Loong_AccountQuery(std::vector <tagTWrap_Loong_RoleData*>&vecData,DWORD dwAccountID,EDBType eDBType,ETBQueryExtraOption eEOption)
{
	return AccountLoad(&vecData,dwAccountID,eDBType,eEOption,&CLoongLoongDBCtrl::FormatAccountQueryRoleData,&CLoongLoongDBCtrl::ProcResQueryRoleData);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//��ҳ��ѯ
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_Equip*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryEquip,&CLoongLoongDBCtrl::ProcResQueryEquip);
}

EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_EquipDel*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryEquipDel,&CLoongLoongDBCtrl::ProcResQueryEquip);
}
//��ҳ��ѯ
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_TaskDone*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryTaskDone,&CLoongLoongDBCtrl::ProcResQueryTaskDone);
}
//��ҳ��ѯ
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_Task*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryTask,&CLoongLoongDBCtrl::ProcResQueryTask);
}

EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_ItemDel*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryItemDel,&CLoongLoongDBCtrl::ProcResQueryItem);
}

//��ҳ��ѯ
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_Fabao*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryFabao,&CLoongLoongDBCtrl::ProcResQueryFabao);
}

EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_Holy*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryHoly,&CLoongLoongDBCtrl::ProcResQueryHoly);

}

EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_HolyEquip*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryHolyEquip,&CLoongLoongDBCtrl::ProcResQueryHolyEquip);
}

//��ҳ��ѯ train_state
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_TrainState*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryTrainState,&CLoongLoongDBCtrl::ProcResQueryTrainState);
}

//��ҳ��ѯ skill
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_Skill*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQuerySkill,&CLoongLoongDBCtrl::ProcResQuerySkill);
}

// ��ҳ��ѯ shortcut_quickbar
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_ShortCutBar*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryShortCutBar,&CLoongLoongDBCtrl::ProcResQueryShortCutBar);
}

//��ҳ��ѯ
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_VisitingCard*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryVisitingCard,&CLoongLoongDBCtrl::ProcResQueryVisitingCard);
}

//��ɫ���������
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_BoardQuestSaveData*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryBoardQuest,&CLoongLoongDBCtrl::ProcResQueryBoardQuest);
}

//��ɫ����ʱ��ͳ��
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryCurMonthOnlineTime,
		&CLoongLoongDBCtrl::ProcResQueryCurMonthOnlineTime);
}

//��������
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_FabaoNirVanaData*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryFabaoNirVanaData,
		&CLoongLoongDBCtrl::ProcResQueryFabaoNirVanaData);
}

//��֮����
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_GodBid*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryGodBid, &CLoongLoongDBCtrl::ProcResQueryGodBid);
}

//�ҵ�
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_EggInfo*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryEggInfo, &CLoongLoongDBCtrl::ProcResQueryEggInfo);
}

//����
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_Area*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryArea, &CLoongLoongDBCtrl::ProcResQueryArea);
}

//ϡ������
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Sheng_RareArea*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQueryRareArea, &CLoongLoongDBCtrl::ProcResQueryRareArea);
}

//��ҳ��ѯ F-Plan 1.5.0 SoulCrystal�꾧
EPagingState CLoongLoongDBCtrl::Loong_PagingQuery(std::vector <tagTWrap_Loong_SoulCrystal*>&vecData,INT64 n64Limit,EDBType eDBType)
{
	return PagingLoad(&vecData,n64Limit,eDBType,&CLoongLoongDBCtrl::FormatPagingQuerySoulCrystal, &CLoongLoongDBCtrl::ProcResQuerySoulCrystal);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//AccountCommon��
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_AccountCommon*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertAccountCommon);
}
//BlackList��
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_BlackList*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertBlackList);
}
//Buff��
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Buff*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertBuff);
}
//	����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ClanData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertClanData);
}
//	enemy
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Enemy*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertEnemy);
}

//	����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Equip*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"equip",&CLoongLoongDBCtrl::FormatInsertEquip);
}
//����baibao
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_EquipBaiBao*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"equip_baibao",&CLoongLoongDBCtrl::FormatInsertEquip);
}
//����ɾ��
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_EquipDel*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"equip_del",&CLoongLoongDBCtrl::FormatInsertEquip);
}
//�����䱦�����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ActClanTreasure*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertActClanTreasure);
}
//����������
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFameHallEnterSnap);
}
//������������ʱ�����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_RepRstTime*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertRepRstTime);
}
//�����б�(����)
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Friend*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFriendSave);
}
//�Ѻö�
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_FriendshipSave*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFriendshipSave);
}
//�Ź�
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_GroupPurchase*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGroupPurchase);
}
//����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Guild*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGuild);
}
//���ɳ�Ա
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_GuildMem*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGuildMem);
}
//item
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Item*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"item",&CLoongLoongDBCtrl::FormatInsertItem);
}
//item baibao
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ItemBaiBao*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"item_baibao",&CLoongLoongDBCtrl::FormatInsertItem);
}
//item del
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ItemDel*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"item_del",&CLoongLoongDBCtrl::FormatInsertItem);
}
//item cd time
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ItemCDTime*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertItemCDTime);
}
//item need log
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ItemNeedLog*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertItemNeedLog);
}
//item log baibao
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_LogBaiBao*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertLogBaiBao);
}
//item role del
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_RoleDel*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertRoleDel);
}
//item role data
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_RoleData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"roledata",&CLoongLoongDBCtrl::FormatInsertRoleData);
}

//item role data del
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_RoleDataDel*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,"roledata_del",&CLoongLoongDBCtrl::FormatInsertRoleData);
}
//skill
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Skill*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertSkill);
}
//task
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Task*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertTask);
}
//task done
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_TaskDone*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertTaskDone);
}
//title
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Title*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertTitle);
}
//����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_VisitingCard*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertVisitingCard);
}
//Ԫ��
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_YBAccount*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertYBAccount);
}
//Ԫ�����׶���
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertYuanBaoOrder);
}

//bill_item
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_BillItem*> &vecData,EDBType eDBType)
{
return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertBillItem);
}
//bill_yuanbao
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_BillYuanBao*> &vecData,EDBType eDBType)
{
return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertBillYuanbao);
}
//bill_yuanbao_log
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecData,EDBType eDBType)
{
return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertBillYuanbaoLog);
}
//pet_data
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_PetData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertPetData);
}
//pet skill
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_PetSkill*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertPetSkill);
}
//vip_stall
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_VipStall*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertVipStall);
}
//guild_commodity
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_GuildCommodity*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGuildCommodity);
}
//guild_skill
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_GuildSkill*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGuildSkill);
}
//vip_netbar
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_VipNetBar*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertVipBar);
}
//guild_upgrade
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_GuildFacilities*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGuildUpgrade);
}
//commerce_rank
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_CommerceRank*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertCommerceRank);
}
//activity
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ActivityData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertActivity);
}
//left_msg
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_LeftMsg*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertLeftMsg);
}

//left_msg
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ExternalLink*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertExternalLink);
}
//left_msg
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_NameHistory*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertNameHistory);
}
//city
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_City*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertCity);
}
//Master
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Master*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertMaster);
}
//short cut bar
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ShortCutBar*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertShortCutBar);
}
//train state
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_TrainState*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertTrainState);
}

//ActiveRankData
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ActiveRankData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertActiveRankData);
}

//ForceBreakOutApprentice
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertForceBreakOutApprentice);
}

//fabao
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Fabao*>&vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFabao);
}

// F-Plan 1.5.0  SoulCrystal
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_SoulCrystal*>&vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertSoulCrystal);
}

//F-Plan 1.6.0 holy
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Holy*>&vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertHoly);
}

BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_HolyEquip*>&vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertHolyEquip);
}



// F-Plan 1.5.0 account_reactive, account_reactiver
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_AccountReactive*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertAccountReactive);
}

BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_AccountReactiver*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertAccountReactiver);
}

//BoardQuest
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertBoardQuest);
}

//GuildWarDeclare
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGuildWarDeclare);
}

//CurMonthOnlineTime
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertCurMonthOnlineTime);
}

//������������
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFabaoNirVanaData);
}

//��֮����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_GodBid*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGodBid);
}

// �ҵ�
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_EggInfo*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertEggInfo);
}

// �³�
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_Wardrobe*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertWardrobe);
}

// ����
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_Area*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertArea);
}

// ϡ������
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Sheng_RareArea*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertRareArea);
}

// F-Plan 1.5.12
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_UseKeyCode*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertUseKeyCode);
}

//------------------------------------------------------------------------------------
// ���ݿ�Ԥ���ص�����
//------------------------------------------------------------------------------------
VOID LoongDBCallBack(DataBase* pDB, INT nReason, INT nParam)
{
	TCHAR cTemp[200];

	if (nReason == DataBase::EDBE_System)
	{
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason  %s , nParam = %u\r\n"), _T("EDBE_System"), nParam);
		g_clsOutput.Output(cTemp);
	}
	else if(nReason == DataBase::EDBE_QueueFull)
	{
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason  %s , nParam = %u\r\n"), _T("EDBE_QueueFull"), nParam);
		g_clsOutput.Output(cTemp);
	}
	else if(nReason == DataBase::EDBE_PoolFull)
	{
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason  %s , nParam = %u\r\n"), _T("EDBE_PoolFull"), nParam);
		g_clsOutput.Output(cTemp);
	}
	else
	{	
		_stprintf(cTemp,_T("Beton Warning CallBack: Reason Unknow,nReason = %u, nParam = %u\r\n"), nReason, nParam);
		g_clsOutput.Output(cTemp);
	}
}

//--------------------------------------------------------------------------------------------------------------------
//Family
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_Family*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFamily,&CLoongLoongDBCtrl::ProcResQueryFamily);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilyMem
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_FamilyMem*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFamilyMem,&CLoongLoongDBCtrl::ProcResQueryFamilyMem);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilyQuset
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_FamilyQuset*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFamilyQuset,&CLoongLoongDBCtrl::ProcResQueryFamilyQuset);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilyRoleSprite
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFamilyRoleSprite,&CLoongLoongDBCtrl::ProcResQueryFamilyRoleSprite);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilySprite
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_FamilySprite*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQueryFamilySprite,&CLoongLoongDBCtrl::ProcResQueryFamilySprite);
}
//--------------------------------------------------------------------------------------------------------------------
//SpriteRequirement
BOOL CLoongLoongDBCtrl::Loong_Query(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecData,EDBType eDBType)
{
	return Load(&vecData,eDBType,&CLoongLoongDBCtrl::FormatQuerySpriteRequirement,&CLoongLoongDBCtrl::ProcResQuerySpriteRequirement);
}
//--------------------------------------------------------------------------------------------------------------------
//Family
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_Family*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFamily);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilyMem
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_FamilyMem*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFamilyMem);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilyQuset
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_FamilyQuset*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFamilyQuset);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilyRoleSprite
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFamilyRoleSprite);
}
//--------------------------------------------------------------------------------------------------------------------
//FamilySprite
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_FamilySprite*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertFamilySprite);
}
//--------------------------------------------------------------------------------------------------------------------
//SpriteRequirement
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertSpriteRequirement);
}
//--------------------------------------------------------------------------------------------------------------------
//GmServerData
BOOL CLoongLoongDBCtrl::Loong_Insert(std::vector <tagTWrap_Loong_GmServerData*> &vecData,EDBType eDBType)
{
	return Insert(&vecData,eDBType,&CLoongLoongDBCtrl::FormatInsertGmServerData);
}
//--------------------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::DeleteRarearea( EDBType eDBType )
{
	if( eDBType >= EM_DB_TYPE_END || eDBType <= EM_DB_NULL )
		return FALSE;

	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) 
		return FALSE;

	pStream->SetDelete("rarearea");

	BOOL bRes = m_LoongDB[eDBType].Execute(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRes;
}
//--------------------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::DaleteArea( EDBType eDBType )
{
	if( eDBType >= EM_DB_TYPE_END || eDBType <= EM_DB_NULL )
		return FALSE;

	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) 
		return FALSE;

	pStream->Clear();
	pStream->FillString("delete from area where AreaID in (3000001,3000101,3000201,3000301,3000401,3000501,3000601,3000701,3000801,3000901,3001001,3001101,3001201,3001301,3001401,3001501,3001601,3001701,3001801,3001901,3002001,3002101,3002201,3002301,3002401,3002501,3002601,3002701,3002801,3002901,3003001,3003101,3003201,3003301);");

	BOOL bRes = m_LoongDB[eDBType].Execute(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRes;
}

//--------------------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::DeleteSkill( EDBType eDBType )
{
	if( eDBType >= EM_DB_TYPE_END || eDBType <= EM_DB_NULL )
		return FALSE;

	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) 
		return FALSE;

	pStream->Clear();
	pStream->FillString("delete from skill where ID in (22000,22001,22002,22003,22004,22005,22006,22007,22008,22009,22010,22011,22012,22013,22014,22015,22016,22017,22018,22019,22020,22021,22022,22023,22024,22025,22026,22027,22028,22029,22030,22031,22032,22033);");

	BOOL bRes = m_LoongDB[eDBType].Execute(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRes;
}

//--------------------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::DeleteBuff( EDBType eDBType )
{
	if( eDBType >= EM_DB_TYPE_END || eDBType <= EM_DB_NULL )
		return FALSE;

	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) 
		return FALSE;

	pStream->Clear();
	pStream->FillString("delete from buff where BuffID in (35067,35068,35069,35070,35071);");

	BOOL bRes = m_LoongDB[eDBType].Execute(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRes;
}

//--------------------------------------------------------------------------------------------------------------------
BOOL CLoongLoongDBCtrl::DeleteItem( EDBType eDBType )
{
	if( eDBType >= EM_DB_TYPE_END || eDBType <= EM_DB_NULL )
		return FALSE;

	MyStream* pStream = m_LoongDB[eDBType].GetStream();
	if( !P_VALID(pStream) ) 
		return FALSE;

	pStream->Clear();
	pStream->FillString("delete from item where TypeID in (4640001,4640101,4640201,4640301,4640401,4640501,4640601,4640701,4640801,4640901,4641001,4641101,4641201,4641301,4641401,4641501,4641601,4641701,4641801,4641901,4642001,4642101,4642201,4642301,4642401,4642501,4642601,4642701,4642801,4642901,4643001,4643101,4643201,4643301);");

	BOOL bRes = m_LoongDB[eDBType].Execute(pStream);

	m_LoongDB[eDBType].ReturnStream(pStream);

	return bRes;
}
//--------------------------------------------------------------------------------------------------------------------
//ɾ����ϡ�������������
BOOL CLoongLoongDBCtrl::DeleteRareareaData(EDBType eDBType)
{
	if( 	!sLoongBeton.DaleteArea(eDBType)		||
			!sLoongBeton.DeleteSkill(eDBType)		||
			!sLoongBeton.DeleteBuff(eDBType)		||
			!sLoongBeton.DeleteItem(eDBType) )
	{
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------


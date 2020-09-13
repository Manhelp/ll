//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDB.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: ���ݿ����Ӧ�ò�ʵ��
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/RoleDefine.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/msg_leftmsg.h"
#include "../ServerDefine/item_define.h"
#include "LoongDB.h"
#include "../WorldDefine/msg_external_links.h"

#include "../ServerDefine/msg_olinfo.h"


TBetonCallback CLoongDB::m_pDBCallback = NULL;
VOID CLoongDB::SetDBCallback(TBetonCallback func)
{
	m_pDBCallback = func;
}
VOID WINAPI CLoongDB::BetonLoginCallback(Beton::DataBase* pDB,INT Reason,INT Param,LPVOID p)
{
	if( P_VALID(p) )
	{
		CLoongDB * pThis = (CLoongDB*)p;
		if(pThis->m_pDBCallback)
		{
			if( pDB == pThis->m_pDBLoong )
				pThis->m_pDBCallback(Reason,Param,_T("gamedb"));
			else if( pDB == pThis->m_pDBLog )
				pThis->m_pDBCallback(Reason,Param,_T("logdb"));
			else if( pDB == pThis->m_pDBLogin )
				pThis->m_pDBCallback(Reason,Param,_T("logindb"));
			else
				pThis->m_pDBCallback(Reason,Param,_T("db"));
		}
	}
}
//-------------------------------------------------------------------------------------------------------
// ���캯��
//-------------------------------------------------------------------------------------------------------
CLoongDB::CLoongDB()
{
	m_pDBLoong = new Beton::DataBase;
	m_pDBLog = new Beton::DataBase;
    m_pDBLogin = new Beton::DataBase;

	m_bInitOK = FALSE;

	m_dwMaxPetID	= MIN_PET_ID;
	m_nPetNum		= 0;

	m_dwMaxRoleID	= 0;
	m_nRoleNum		= 0;
	m_nLoadNumLim	= 0;

	m_dwReadTimes	= 0;
	m_dwWriteTimes	= 0;

	m_dwTempChannelID = 0;

	//m_bOpenShengLing = true;

	m_pDBLoong->SetWarningCallBack(BetonLoginCallback,this);
	m_pDBLog->SetWarningCallBack(BetonLoginCallback,this);
	m_pDBLogin->SetWarningCallBack(BetonLoginCallback,this);

	m_hKoreaLogFile = INVALID_HANDLE_VALUE;
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
CLoongDB::~CLoongDB()
{
	SAFE_DEL(m_pDBLoong);
	SAFE_DEL(m_pDBLog);
	SAFE_DEL(m_pDBLogin);

	CloseHandle(m_hKoreaLogFile);
	m_hKoreaLogFile = INVALID_HANDLE_VALUE;
}


//-------------------------------------------------------------------------------------------------------
// ��ȡ�����ļ�,����ʼ�����ݿ�����
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::Init(LPCTSTR szIniFileName,BOOL bReload)
{
	// ��������,�����������ļ�
	if( !bReload )
	{
		CreateObj("LoongDBVar", "VarContainer");
		TObjRef<VarContainer> pVar = "LoongDBVar";
		if(!P_VALID(pVar))
		{
			ERR(_T("Create LoongDBVar(VarContainer) obj failed!"));
			return FALSE;
		}

		pVar->Load("VirtualFileSys", szIniFileName);

		// ��������ʼ����Ϸ���ݿ�����
		/*if(!m_pDBLoong->Init(_T("172.17.1.201"), _T("root"), _T("kungfu"), _T("loong_base"), 3306))*/
		if(!m_pDBLoong->Init(pVar->GetString(_T("host_name db_game")),
			pVar->GetString(_T("user_name db_game")), 
			pVar->GetString(_T("password db_game")), 
			pVar->GetString(_T("db_name db_game")), 
			(INT)pVar->GetDword(_T("port db_game"))))
		{
			ERR(_T("Game Database initialize failed! Please check profile..."));
			SAFE_DEL(m_pDBLoong);
			return FALSE;
		}

		// ��������ʼ��log���ݿ�����
		if(!m_pDBLog->Init(pVar->GetString(_T("host_name db_log")), 
			pVar->GetString(_T("user_name db_log")), 
			pVar->GetString(_T("password db_log")), 
			pVar->GetString(_T("db_name db_log")), 
			(INT)pVar->GetDword(_T("port db_log"))))
		{
			ERR(_T("Log Database initialize failed! Please check profile..."));
			SAFE_DEL(m_pDBLoong);
			SAFE_DEL(m_pDBLog);
			return FALSE;
		}

		//��������ʼ��login��Ϸ���ݿ�����
		if(!m_pDBLogin->Init(pVar->GetString(_T("host_name db_login")), 
			pVar->GetString(_T("user_name db_login")), 
			pVar->GetString(_T("password db_login")), 
			pVar->GetString(_T("db_name db_login")), 
			(INT)pVar->GetDword(_T("port db_login"))))
		{
			ERR(_T("Login Database initialize failed! Please check profile..."));
			SAFE_DEL(m_pDBLoong);
			SAFE_DEL(m_pDBLog);
			SAFE_DEL(m_pDBLogin);
			return FALSE;
		}

		//m_bOpenShengLing = pVar->GetInt(_T("open open_shengling"));

		// �������
		pVar->Clear();
		// ��������
		KillObj("LoongDBVar");
	}
	else
	{
		do 
		{
			if( P_VALID(m_pDBLoong) )
				m_bInitOK = m_pDBLoong->Reconnect(FALSE);
			if( !m_bInitOK )
				break;
			if( P_VALID(m_pDBLog) )
				m_bInitOK = m_pDBLog->Reconnect(FALSE);
			if( !m_bInitOK )
				break;
			if( P_VALID(m_pDBLogin) )
				m_bInitOK = m_pDBLogin->Reconnect(FALSE);
			if( !m_bInitOK )
				break;
		} while (0);
		if( !m_bInitOK )
		{
			SAFE_DEL(m_pDBLoong);
			SAFE_DEL(m_pDBLog);
			SAFE_DEL(m_pDBLogin);
			return FALSE;
		}
	}

	GetRoleCountAndMaxRoleID(m_nRoleNum, m_dwMaxRoleID);
	GetYBAccountNum(m_nAccountNum);
	GetYBOrderNum(m_nOrderNum);
	GetActivityNum(m_nActivityNum);
	GetActivityRankNum(m_nActivityRankNum);
	GetMaxOrderIndex(m_dwMaxOrderIndex);	
	GetPetCountAndMaxPetID(m_nPetNum, m_dwMaxPetID);
	GetGPInfoNumAndSize(m_nGroupPurchaseInfoNum, m_nGroupPurchaseInfoSize);
	GetLimitItemInfoNumAndSize(m_nLimitItemInfoNum,m_nLimitItemInfoSize);
    
	m_dwReadTimes	= 0;
	m_dwWriteTimes	= 0;

	m_bInitOK = TRUE;

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ��Դ���պ���
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::Destroy()
{
	if(m_bInitOK)
	{
		m_dwReadTimes	= 0;
		m_dwWriteTimes	= 0;
		m_bInitOK		= FALSE;

		m_pDBLoong->ShutDown();
		m_pDBLog->ShutDown();
		m_pDBLogin->ShutDown();

		return;
	}
}

//-------------------------------------------------------------------------------------------------------
// ��ȡroledata��¼���������RoleID
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetRoleCountAndMaxRoleID(INT32 &nCount, DWORD &dwMaxRoleID)
{
	nCount = 0;
	dwMaxRoleID = 0;
	
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("roledata", "count(RoleID),max(RoleID)");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nCount = (*pResult)[0].GetInt();
		dwMaxRoleID = (*pResult)[1].GetDword();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡroledata��¼���������RoleID
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetMaxMinSerial(INT64 &n64Max, INT64 &n64Min)
{
	INT64 n64MaxSerial = MIN_ITEM_SERIAL_INTERNAL;
	INT64 n64MinSerial = MAX_ITEM_SERIAL_OUTTER;

	n64Max = n64MaxSerial;
	n64Min = n64MinSerial;

	GetMaxMinSerial("item", n64MaxSerial, n64MinSerial);
	n64Max = max(n64MaxSerial, n64Max);
	n64Min = min(n64MinSerial, n64Min);

	GetMaxMinSerial("item_baibao", n64MaxSerial, n64MinSerial);
	n64Max = max(n64MaxSerial, n64Max);
	n64Min = min(n64MinSerial, n64Min);

	GetMaxMinSerial("item_del", n64MaxSerial, n64MinSerial);
	n64Max = max(n64MaxSerial, n64Max);
	n64Min = min(n64MinSerial, n64Min);
}


//-------------------------------------------------------------------------------------------------------
// ��ȡroledata��¼���������RoleID
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetMaxMinSerial(const char* pTable, INT64 &n64Max, INT64 &n64Min)
{
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect(pTable, "max(SerialNum),min(SerialNum)");
	pStream->SetWhere();
	pStream->FillString("SerialNum>") << 0;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		n64Max = (*pResult)[0].GetLong();
		n64Min = (*pResult)[1].GetLong();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//��longinDB�����ɫ����
BOOL CLoongDB::InsertRoleName(DWORD dwID, LPVOID pNameData)
{
	LPCTSTR pName = (LPCTSTR)pNameData;
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLogin->GetStream();
	Beton::Connection* pCon = m_pDBLogin->GetFreeConnection();
	// ��ʽ����ɫ����
	pStream->SetInsert("role_name");
	pStream->FillString("name='").FillString(pName, pCon).FillString("'");
	// �ͷ�����
	m_pDBLogin->ReturnConnection(pCon);

	// ���뵽���ݿ�
	BOOL bRet = m_pDBLogin->Execute(pStream);
	m_pDBLogin->ReturnStream(pStream);
	++m_dwWriteTimes;
	return bRet;
}

//��longinDB�����������
BOOL CLoongDB::InsertGuildName(DWORD dwID,LPVOID pNameData)
{
	LPCTSTR pName = (LPCTSTR)pNameData;
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLogin->GetStream();
	Beton::Connection* pCon = m_pDBLogin->GetFreeConnection();
	// ��ʽ����ɫ����
	pStream->SetInsert("guild_name");
	pStream->FillString("name='").FillString(pName, pCon).FillString("'");
	// �ͷ�����
	m_pDBLogin->ReturnConnection(pCon);
	// ���뵽���ݿ�
	BOOL bRet = m_pDBLogin->Execute(pStream);
	m_pDBLogin->ReturnStream(pStream);
	++m_dwWriteTimes;
	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ������ɫ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::CreateRole(OUT DWORD &dwNewRoleID, DWORD dwAccountID, 
						  const tagCreateRoleInfo &RoleInfo, DWORD &dwRoleNameCrc)
{
	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	// ��ʽ����ɫ����
	pStream->SetInsert("roledata");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",RoleID=") << m_dwMaxRoleID + 1;
	pStream->FillString(",RoleName='").FillString(RoleInfo.szRoleName, pCon);
	
	// ת��Сд����CRC32
	int cpylen = min(sizeof(RoleInfo.szRoleName),sizeof(m_szRoleName));
	_tcscpy_s(m_szRoleName, cpylen, RoleInfo.szRoleName);
	_tcslwr(m_szRoleName);
	dwRoleNameCrc = m_pUtil->Crc32(m_szRoleName);
	pStream->FillString("',RoleNameCrc=") << dwRoleNameCrc;

	pStream->FillString(",Sex=") << RoleInfo.Avatar.bySex;
	pStream->FillString(",HairModelID=") << RoleInfo.Avatar.wHairMdlID;
	pStream->FillString(",HairColorID=") << RoleInfo.Avatar.wHairTexID;
	pStream->FillString(",FaceModelID=") << RoleInfo.Avatar.wFaceMdlID;
	pStream->FillString(",FaceDetailID=") << RoleInfo.Avatar.wFaceDetailTexID;
	pStream->FillString(",DressModelID=") << RoleInfo.Avatar.wDressMdlID;

	pStream->FillString(",MapID=") << RoleInfo.dwMapID;
	pStream->FillString(",X=") << RoleInfo.fX;			// ����
	pStream->FillString(",Y=") << RoleInfo.fY;
	pStream->FillString(",Z=") << RoleInfo.fZ;
	pStream->FillString(",FaceX=") << RoleInfo.fFaceX;	// ����
	pStream->FillString(",FaceY=") << RoleInfo.fFaceY;
	pStream->FillString(",FaceZ=") << RoleInfo.fFaceZ;

	pStream->FillString(",Level=") << RoleInfo.byLevel;

	// LoongDB����ʱ��
	if(DwordTime2DataTime(m_szTime, sizeof(m_szTime), RoleInfo.CreateTime))
	{
		pStream->FillString(",CreateTime='").FillString(m_szTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::CreateRole()!!!!!\r\n"));
		ASSERT(0);
	}

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);

	// ���뵽���ݿ�
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	if(bRet)
	{
		++m_dwMaxRoleID;
		++m_nRoleNum;
		dwNewRoleID = m_dwMaxRoleID;
	}

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// �ָ���ɫ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ResumeRole(DWORD dwRoleID)
{
	//?? Ŀǰ���ṩ֧��
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// ���ݽ�ɫ�������//??������ر�����
//-------------------------------------------------------------------------------------------------------
// DWORD CLoongDB::BackupRole(DWORD dwAccountID, DWORD dwRoleID)
// {
// 	DWORD dwErrorCode;
// 
// 	// ����roledata�����������
// 	dwErrorCode = BackupRoleData(dwAccountID, dwRoleID);
// 	if(E_Success != dwErrorCode)
// 	{
// 		return dwErrorCode;
// 	}
// 
// 	// ����item�����������
// 	dwErrorCode = BackupItem(dwRoleID);
// 	if(E_Success != dwErrorCode)
// 	{
// 		return dwErrorCode;
// 	}
// 
// 	return dwErrorCode;
// }


//********************************** ��������(���ֱ�ɾ����ɫ��) ***************************************

//-------------------------------------------------------------------------------------------------------
// ���ݽ�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::BackupRoleData(DWORD dwAccountID, DWORD dwRoleID)
{
	BOOL bRet = TRUE;
	
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ�����ݿ�������
	pStream->Clear();
	pStream->FillString("insert into roledata_del select * from roledata where AccountID=") << dwAccountID;
	pStream->FillString(" and RoleID=") << dwRoleID;

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ���ݽ�ɫ��Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
// BOOL CLoongDB::BackupItem(DWORD dwRoleID)
// {
// 	BOOL bRet = TRUE;
// 	
// 	// ��ȡ��
// 	Beton::MyStream* pStream = m_pDBLoong->GetStream();
// 	ASSERT(P_VALID(pStream));
// 
// 	// ��ʽ�����ݿ�������
// 	pStream->Clear();
// 	pStream->FillString("insert into item_del select * from item where OwnerID=") << dwRoleID;
// 
// 	// ִ��
// 	bRet = m_pDBLoong->Execute(pStream);
// 	
// 	// �ͷ���
// 	m_pDBLoong->ReturnStream(pStream);
// 
// 	++m_dwWriteTimes;
// 
// 	return bRet;
// }


//************************************* ɾ����ɫ����������� ********************************************

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ//??������ر�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoongDB::DelRole(DWORD dwAccountID, DWORD dwRoleID)
{
	if(DelRoleData(dwAccountID, dwRoleID))
	{
		// ɾ��buff����Ʒ��ʧ��, ��������//??
		DelRoleBuff(dwRoleID);
		DelRoleItem(dwRoleID);
	}
	else
	{
		return E_DBDelete_RoleFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelRoleData(DWORD dwAccountID, DWORD dwRoleID)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ�����ݿ�������
	pStream->SetDelete("roledata");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫbuff��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelRoleBuff(DWORD dwRoleID)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ�����ݿ�������
	pStream->SetDelete("buff");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ɾ����ɫ��Ʒ��Ϣ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelRoleItem(DWORD dwRoleID)
{
	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// ��ʽ�����ݿ�������
	pStream->SetDelete("item");
	pStream->SetWhere();
	pStream->FillString("OwnerID=") << dwRoleID;
	pStream->FillString(" and (ContainerTypeID!=") << EICT_RoleWare;
	pStream->FillString(" or (ContainerTypeID=") << EICT_RoleWare;
	pStream->FillString(" and Bind=") << EBS_Bind;
	pStream->FillString("))");

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}


//-------------------------------------------------------------------------------------------------------
// ��¼ɾ����ɫʱ�Ľ�ɫ���ƣ�Crc��ʱ�估IP��
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::RecordInfoDelRole(DWORD dwAccountID, DWORD dwRoleID, LPCTSTR szRoleName, LPCSTR szIP)
{
	BOOL bRet = TRUE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	ASSERT(P_VALID(pStream));
	ASSERT(P_VALID(pCon));
	
	// ��¼ɾ�����������Ϣ
	pStream->SetInsert("role_del");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",RoleName='").FillString(szRoleName, pCon);
	pStream->FillString("',RoleID=") << dwRoleID;
	pStream->FillString(",IP='").FillString(szIP);
	pStream->FillString("',DeleteTime=now()");

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);
	
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ��Ʒ��ȴ
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ReplaceItemCDTime(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	BOOL bRet = TRUE;

	// ��ȡ��������
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	*ppOutPointer = pData;

	// ��ʽ������
	pStream->SetReplace("item_cdtime");
	pStream->FillString("RoleID=") << dwRoleID;
	if(nNum > 0)
	{
		pStream->FillString(",CDTime='");
		pStream->FillBlob(pData, sizeof(tagCDTime) * nNum, pCon);
		pStream->FillString("'");

		*ppOutPointer = (BYTE*)pData + sizeof(tagCDTime) * nNum;
	}

	// �ͷ�����
	m_pDBLoong->ReturnConnection(pCon);

	// ִ��
	bRet = m_pDBLoong->Execute(pStream);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡYBAccount��¼������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetYBAccountNum(INT32 &nAccountNum)
{
	nAccountNum = 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("ybaccount", "count(RoleID)");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nAccountNum = (*pResult)[0].GetInt();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡyuanbaoorder��ί�ж�����¼������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetYBOrderNum(INT32 &nOrderNum)
{
	nOrderNum = 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("yuanbaoorder", "count(dwID)");
	pStream->SetWhere();
	pStream->FillString("OrderMode=") << 0;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nOrderNum = (*pResult)[0].GetInt();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡһ�����yuanbaoorder��¼������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetRoleYBOrderNum(DWORD dwRoleID, INT32 &nOrderNum)
{
	nOrderNum = 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("yuanbaoorder", "count(dwID)");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nOrderNum = (*pResult)[0].GetInt();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ù̶������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetActivityNum(INT32 &nActivityNum)
{
	nActivityNum = 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("activity", "count(dwID)");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nActivityNum = (*pResult)[0].GetInt();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

VOID CLoongDB::GetActivityRankNum(INT32 &nActivityRankNum)
{
	nActivityRankNum = 0 ;
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	if( !P_VALID(pStream) )
		return;

	pStream->Clear();

	// ��ʽ��
	pStream->SetSelect("activity_rankdata", "count(role_id)");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nActivityRankNum = (*pResult)[0].GetInt();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡһ�����yuanbaoorder��¼���Index
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetMaxOrderIndex( DWORD& dwMaxIndex )
{
	dwMaxIndex	= 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("yuanbaoorder", "max(dwID)");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		dwMaxIndex = (*pResult)[0].GetDword();
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

VOID CLoongDB::GetPetCountAndMaxPetID( INT32 &nCount, DWORD &dwMaxPetID )
{
	nCount = 0;
	dwMaxPetID = MIN_PET_ID;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("pet_data", "count(pet_id),max(pet_id)");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nCount = (*pResult)[0].GetInt();
		dwMaxPetID = (*pResult)[1].GetDword();
		if (!IS_PET(dwMaxPetID))
		{
			dwMaxPetID = MIN_PET_ID;
		}
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����������Ʒ��Ϣ������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetLimitItemInfoNumAndSize(INT32 &nInfoNum, INT32 &nDataSize)
{
	nInfoNum	= 0;
	nDataSize	= 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("LimitNumItemInfo", "Id");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && (pResult->GetRowCount() > 0))
	{
		nInfoNum = pResult->GetRowCount();		
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���а����Ź���Ϣ������
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::GetGPInfoNumAndSize( INT32 &nGPInfoNum, INT32 &nDataSize )
{
	nGPInfoNum	= 0;
	nDataSize	= 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("group_purchase", "ParticipatorList");

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nGPInfoNum = pResult->GetRowCount();
		nDataSize = (sizeof(tagGPInfo)-sizeof(DWORD)) * nGPInfoNum;

		for (int i=0; i<nGPInfoNum; i++)
		{
			nDataSize += (*pResult)[0].GetLen();

			pResult->NextRow();
		}
	}

	++m_dwReadTimes;

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
}

BOOL CLoongDB::LoadLeftMsg( DWORD dwRoleID, PVOID pLeftMsg, DWORD &dwSize )
{
	INT nMsgNum = 0;
	PVOID pData = pLeftMsg;
	BOOL bRtv = Load(pData, &nMsgNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadLeftMsg, &CLoongDB::ProcResLoadLeftMsg);
	if (bRtv)
	{
		dwSize = (BYTE*)pData - (BYTE*)pLeftMsg;
	}
	return bRtv;
}

VOID CLoongDB::FormatLoadLeftMsg( Beton::MyStream *pStream, DWORD dwDumy, DWORD dwRoleID )
{
	pStream->Clear();
	pStream->FillString("select msg_data from left_msg where roleid=") << dwRoleID;
	pStream->FillString(" order by msg_id asc");
}

VOID CLoongDB::ProcResLoadLeftMsg( OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes )
{
	if(!P_VALID(pRes) || pRes->GetRowCount() <= 0)
		return;

	INT nRowNum = pRes->GetRowCount();

	INT i;

	for (i=0; i<nRowNum; ++i)
	{
		UINT uLen = (*pRes)[0].GetLen();

		BOOL bRtv = (*pRes)[0].GetBlob(pData, uLen);

		if (!bRtv)
			continue;

		pData = (BYTE*) pData + uLen;

		pRes->NextRow();
	}

	if (P_VALID(pNum))
	{
		*pNum = i;
	}
}

BOOL CLoongDB::DelLeftMsg( DWORD dwRoleID, DWORD dwDateTime )
{
	return Update(dwRoleID, (LPVOID)&dwDateTime, 1, sizeof(DWORD), &CLoongDB::FormatDelLeftMsg, NULL);
}

VOID CLoongDB::FormatDelLeftMsg( Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDumy )
{
	DWORD dwDateTime = *((DWORD*)pDumy);
	pStream->Clear();
	pStream->FillString("delete from left_msg where roleid=") << dwRoleID;
	pStream->FillString(" and msg_id=") << dwDateTime;
}

BOOL CLoongDB::InsertLeftMsg( DWORD dwRoleID, DWORD dwDateTime, PVOID pLeftMsg )
{
	return Update(dwRoleID, pLeftMsg, 1, 0,&CLoongDB::FormatInsertLeftMsg, NULL); 
}

VOID CLoongDB::FormatInsertLeftMsg( Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData )
{
	MTRANS_POINTER(pMsgInsert, pData, tagDBLeftMsg);
	Beton::Connection* con = m_pDBLoong->GetFreeConnection();
	pStream->Clear();
	pStream->FillString("insert into left_msg set roleid=") << pMsgInsert->dwRoleID;
	pStream->FillString(", msg_id=")<< pMsgInsert->dwDateTime;
	tagNetCmd* pCmd = (tagNetCmd*)(pMsgInsert->byData);
	pStream->FillString(", msg_data='");
	pStream->FillBlob(pCmd, pCmd->dwSize, con);
	pStream->FillString("'");
	m_pDBLoong->ReturnConnection(con);
}

VOID CLoongDB::FormatClearLeftMsg( Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData )
{
	pStream->Clear();
	pStream->FillString("delete from left_msg where roleid=")<<dwRoleID;
}

BOOL CLoongDB::ClearLeftMsg( DWORD dwRoleID )
{
	return Update(dwRoleID, NULL, 1, 0, &CLoongDB::FormatClearLeftMsg);
}

BOOL CLoongDB::LoadLeftMsgIndexes( DWORD* pIndexes, DWORD dwRoleID, INT &nNum )
{
	LPVOID pData = (LPVOID)pIndexes;
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadLeftMsgIndexes, &CLoongDB::ProcResLoadLeftMsgIndexes);
}

VOID CLoongDB::FormatLoadLeftMsgIndexes( Beton::MyStream *pStream, DWORD dwDumy, DWORD dwRoleID )
{
	pStream->FillString("select msg_id from left_msg where roleid=") << dwRoleID;
	pStream->FillString(" order by msg_id asc");
}

VOID CLoongDB::ProcResLoadLeftMsgIndexes( OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes )
{
	if (!P_VALID(pRes))
		return;

	DWORD* pIndexes = (DWORD*)pData;

	INT nNum = pRes->GetRowCount();
	INT i;
	for (i=0; i<nNum; ++i)
	{
		pIndexes[i] = (*pRes)[0].GetDword();
		pRes->NextRow();
	}
	if (P_VALID(pNum))
	{
		*pNum = i;
	}
}

BOOL CLoongDB::UpdateRoleAtt( DWORD dwRoleID, PVOID pData )
{
	return Update(dwRoleID, pData, 1, sizeof(tagNDBC_UpdateRoleAtt), &CLoongDB::FormateUpdateRoleAtt);
}

BOOL CLoongDB::UpdateRoleAttPoint( DWORD dwRoleID, LPVOID pMsg )
{
	return Update(dwRoleID, pMsg, 1, sizeof(tagNDBC_UpdateRoleAttPoint), &CLoongDB::FormateUpdateRoleAttPoint);
}

BOOL CLoongDB::UpdateRoleTalentPoint( DWORD dwRoleID, LPVOID pMsg )
{
	return Update(dwRoleID, pMsg, 1, sizeof(tagNDBC_UpdateRoleTalentPoint), &CLoongDB::FormateUpdateRoleTalentPoint);
}

BOOL CLoongDB::UpdateRoleItemTranport( DWORD dwRoleID, LPVOID pMsg )
{
	return Update(dwRoleID, pMsg, 1, sizeof(tagNDBC_UpdateRoleItemTransport), &CLoongDB::FormateUpdateRoleItemTransport);
}

VOID CLoongDB::FormateUpdateRoleAtt( Beton::MyStream* pStream, DWORD dwRoleID, PVOID pData )
{
	MTRANS_POINTER(pRecv, pData, tagNDBC_UpdateRoleAtt);

	pStream->Clear();
	pStream->SetUpdate("roledata");
	switch(pRecv->nType)
	{
	case ERTSA_Exp:
		pStream->FillString("ExpCurLevel");
		break;
	case ERTSA_Level:
		pStream->FillString("Level");
		break;
	default:
		ASSERT(0);
		break;
	}
	pStream->FillString("=") << pRecv->nValue;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormateUpdateRoleAttPoint( Beton::MyStream* pStream, DWORD dwRoleID, PVOID pData )
{
	MTRANS_POINTER(pRecv, pData, tagNDBC_UpdateRoleAttPoint);

	pStream->Clear();
	pStream->SetUpdate("roledata");

	pStream->FillString("AttPtAvail=") << pRecv->nAttPointLeft;

	pStream->FillString(",PhysiqueAdded=") << pRecv->nAttPointAdd[0];
	pStream->FillString(",StrengthAdded=") << pRecv->nAttPointAdd[1];
	pStream->FillString(",PneumaAdded=") << pRecv->nAttPointAdd[2];
	pStream->FillString(",InnerforceAdded=") << pRecv->nAttPointAdd[3];
	pStream->FillString(",TechniqueAdded=") << pRecv->nAttPointAdd[4];
	pStream->FillString(",AgilityAdded=") << pRecv->nAttPointAdd[5];

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormateUpdateRoleTalentPoint( Beton::MyStream* pStream, DWORD dwRoleID, PVOID pData )
{
	MTRANS_POINTER(pRecv, pData, tagNDBC_UpdateRoleTalentPoint);
	pStream->Clear();
	pStream->SetUpdate("roledata");

	std::stringstream str;

	str << "TalentType" << pRecv->nIndex+1 << "=" << pRecv->Talent.eType;
	str << ",TalentVal" << pRecv->nIndex+1 << "=" << pRecv->Talent.nPoint;
	pStream->FillString(str.str().c_str());

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}
VOID CLoongDB::FormateUpdateRoleItemTransport( Beton::MyStream* pStream, DWORD dwRoleID, PVOID pData )
{
	MTRANS_POINTER(pRecv, pData, tagNDBC_UpdateRoleItemTransport);
	pStream->Clear();
	pStream->SetUpdate("roledata");
	pStream->FillString("ItemTransportMapID=") << pRecv->dwMapID;
	pStream->FillString(",ItemTransportX=") << pRecv->fX;
	pStream->FillString(",ItemTransportZ=") << pRecv->fZ;
	pStream->FillString(",ItemTransportY=") << pRecv->fY;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}
INT32 CLoongDB::TestDBReadFunction()
{
	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("roledata", "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << m_dwMaxRoleID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{

	}
	else
	{
		// �ͷ����ͽ��
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->FreeQueryResult(pResult);
		return E_SystemError;
	}

	// �ͷ����ͽ��
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return E_Success;
}

BOOL CLoongDB::LoadRoleItemSerial( DWORD dwRoleID, PVOID &pData, INT &nNum, DWORD dwConType )
{
	return Load(pData, &nNum, dwConType, dwRoleID, &CLoongDB::FormatLoadRoleItemSerials, &CLoongDB::ProcResLoadRoleItemSerials);
}

// Jason �ⲿ���� v1.3.1
BOOL CLoongDB::LoadExtLinks(tagExternalLink *& links,int & num)
{
	BOOL bRet = TRUE;
	num = 0;
	do 
	{
		Beton::MyStream* pStream = m_pDBLoong->GetStream();
		//ASSERT(P_VALID(pStream));
		if( !P_VALID(pStream) )
			break;

		pStream->Clear();

		pStream->FillString  ("select LinkName,LinkURL from external_links order by ID desc");

		// ��ѯ���ݿ�
		Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
		if(!P_VALID(pResult))
		{
			bRet = FALSE;
			m_pDBLoong->ReturnStream(pStream);
			break;
		}

		num = pResult->GetRowCount();
		if (num > 0)
		{
			void * p = g_MemPoolSafe.Alloc  ( sizeof(tagExternalLink) * num );
			memset(p,0, sizeof(tagExternalLink) * num );
			tagExternalLink * pLink = (tagExternalLink *)p;
			links = pLink;
			int l = 8;

			LPCTSTR pStr = NULL;
			
			Util util;
			num = num > 6 ? 6 : num;
			for(int i = 0; i < num ; ++i)
			{
				pStr = util.Unicode8ToUnicode  ((*pResult)[0].GetString());
				if( _tcslen(pStr) > 8 - 1 )
					l = 8 - 1;
				else
					l = _tcslen(pStr);
				_tcsncpy(pLink[i].linkName , pStr, l) ;
				pStr = util.Unicode8ToUnicode  ((*pResult)[1].GetString());
				if( _tcslen(pStr) > X_LONG_NAME - 1 )
					l = X_LONG_NAME - 1;
				else
					l = _tcslen(pStr);
				_tcsncpy(pLink[i].linkValue , pStr, l) ;

				pResult->NextRow  ();
			}
		}
		else
		{
			bRet = FALSE;
		}

		// �ͷ����ͽ��
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->FreeQueryResult(pResult);

	} while (0);

	return bRet;
}

VOID CLoongDB::FormatLoadRoleItemSerials( Beton::MyStream *pStream, DWORD dwMisc, DWORD dwRoleID )
{
	DWORD dwConType = dwMisc & 0xff;
	DWORD dwBind	= (dwMisc >> 16) & 0xff;

	pStream->Clear();
	pStream->SetSelect("item", "SerialNum, TypeID");
	pStream->SetWhere();
	pStream->FillString("OwnerID=") << dwRoleID;
	pStream->FillString(" and ContainerTypeID=") << dwConType;
	pStream->FillString(" and Bind=") << dwBind;

}

VOID CLoongDB::ProcResLoadRoleItemSerials( OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes )
{
	if (!P_VALID(pRes))
		return;

	tagItemData* pItemData = (tagItemData*)pData;

	INT nNum = pRes->GetRowCount();
	INT i = 0;
	for (i=0; i<nNum; ++i)
	{
		pItemData[i].n64Serial = (*pRes)[0].GetLong();
		pItemData[i].dwTypeID = (*pRes)[1].GetDword();

		pRes->NextRow();
	}

	if (P_VALID(pNum))
	{
		*pNum = i;
	}	
}

// Jason 2010-1-15 v1.3.2 ���߹һ�
VOID CLoongDB::OfflineExperienceReward(DWORD AccountID,DWORD RoleID)
{
#if 0
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		//for(int i = 0; i < 3; ++i)
		//{
			pStream->Clear();
			pStream->FillString  ("update roledata set OfflineExperienceRewardFlag=0 where AccountID = ") << AccountID ;
			pStream->FillString( " and RoleID != " ) << RoleID;
			m_pDBLoong->Execute(pStream);
			//if( m_pDBLoong->Execute(pStream) )
			//	break;
		//}
		//for(int i = 0; i < 3; ++i)
		//{
			pStream->Clear();
			pStream->FillString  ("update roledata set OfflineExperienceRewardFlag=1 where AccountID = ") << AccountID ;
			pStream->FillString( " and RoleID = " ) << RoleID;
			m_pDBLoong->Execute(pStream);
			//if( m_pDBLoong->Execute(pStream) )
			//	break; 
		//}
	}
#endif
}

DWORD CLoongDB::GetAccountInfo(	DWORD	dwAccountID,tagDWORDTime&	timeLastLogin,tagDWORDTime&	timeLastLogout,	tagDWORDTime&	timeLastReceiveDailyOfflineReward,	tagDWORDTime&	timeLastReceiveRegression )
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("select LastLoginTime ,LastLogoutTime ,LastReceiveDailyOfflineRewardTime ,LastReceiveRegressionTime from account_common where AccountID = ") << dwAccountID ;
		Beton::QueryResult* Query = m_pDBLoong->Query(pStream);
		if( P_VALID(Query) )
		{
			if( Query->GetRowCount() > 0 )
			{
				timeLastLogin = (*Query)[0].GetDword();
				timeLastLogout = (*Query)[1].GetDword();
				timeLastReceiveDailyOfflineReward = (*Query)[2].GetDword();
				timeLastReceiveRegression = (*Query)[3].GetDword();
				m_pDBLoong->FreeQueryResult(Query);

				if( timeLastLogin == 0 || timeLastLogout == 0 )
				{
					pStream->Clear();
					pStream->FillString  ("select LoginTime ,LogoutTime from roledata where AccountID = ") << dwAccountID ;
					pStream->FillString  (" order by LogoutTime desc");
					Beton::QueryResult* Query1 = m_pDBLoong->Query(pStream);
					if( P_VALID(Query1) )
					{
						if( Query1->GetRowCount  () > 0 )
						{
							tagDWORDTime tem;
							const CHAR * buf = (*Query1)[0].GetString();
							if( DataTime2DwordTime(tem,buf,strlen(buf)) )
								timeLastLogin = tem;
							buf = (*Query1)[1].GetString();
							if( DataTime2DwordTime(tem,buf,strlen(buf)) )
								timeLastLogout = tem;
						}
						m_pDBLoong->FreeQueryResult(Query1);
					}
				}
				m_pDBLoong->ReturnStream(pStream);
				return 0;
			}	
			m_pDBLoong->FreeQueryResult(Query);
		}
		m_pDBLoong->ReturnStream(pStream);
	}
	return GT_INVALID;
}

DWORD CLoongDB::SaveAccountInfo(	DWORD	dwAccountID,tagDWORDTime	timeLastLogin,tagDWORDTime	timeLastLogout,	tagDWORDTime	timeLastReceiveDailyOfflineReward,	tagDWORDTime	timeLastReceiveRegression )
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("update account_common set LastLoginTime = ") << timeLastLogin;
		pStream->FillString(",LastLogoutTime = ") << timeLastLogout;
		pStream->FillString(",LastReceiveDailyOfflineRewardTime = ") << timeLastReceiveDailyOfflineReward;
		pStream->FillString(",LastReceiveRegressionTime = ") << timeLastReceiveRegression;
		pStream->FillString(" where AccountID = ") << dwAccountID ;
		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return 0;
		}
		m_pDBLoong->ReturnStream(pStream);
	}
	return GT_INVALID;
}
VOID CLoongDB::DoWhenWorldCrashDown( )
{
	//Beton::MyStream* pStream = m_pDBLoong->GetStream();
	////ASSERT(P_VALID(pStream));
	//if( P_VALID(pStream) )
	//{
	//	pStream->Clear();
	//	pStream->FillString  ("update account_common set LastLogoutTime = ") << (DWORD)GetCurrentDWORDTime();
	//	pStream->FillString(" where LastLoginTime > LastLogoutTime");
	//	m_pDBLoong->Execute  ( pStream );
	//	m_pDBLoong->ReturnStream(pStream);
	//}
}


// ȡ���г���б����н�ɫIDΪdwRoleID�Ľ�ɫ��ID
BOOL CLoongDB::GetRoleListOfEnemy(DWORD dwRoleID, DWORD dwRoleIDList[], DWORD &dwRoleCnt)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( !P_VALID(pStream) )
	{
		return FALSE;
	}

	pStream->Clear();
	pStream->FillString  ("select RoleID from enemy where EnemyID=") << dwRoleID;

	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		// �ͷ���
		m_pDBLoong->ReturnStream(pStream);
		dwRoleCnt = 0;
		return FALSE;
	}

	if( pResult->GetRowCount() > 0 )
	{
		dwRoleCnt = (pResult->GetRowCount() > dwRoleCnt) ? dwRoleCnt : pResult->GetRowCount();
		for (int nCnt = 0; nCnt < dwRoleCnt; nCnt++)
		{
			dwRoleIDList[nCnt] = (*pResult)[0].GetDword();
			pResult->NextRow();
		}
	}
	else
	{
		dwRoleCnt = 0;
	}

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return TRUE;
}

DWORD CLoongDB::SaveOlInfo(INT num,VOID * pData)
{
	if( num <= 0 || !P_VALID(pData))
		return GT_INVALID;

	tagWorldMapOnlineInfo * pOlData = (tagWorldMapOnlineInfo*)pData;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		while( num-- > 0 )
		{
			pStream->Clear();
			pStream->FillString  ("REPLACE mapolinfo SET MapID = ") << pOlData->dwMapID;
			pStream->FillString(",IsInst = ") << pOlData->bIsInst;
			pStream->FillString(",OnlineNum = ") << pOlData->nOnlineNum;
			pStream->FillString(",OpenNum = ") << pOlData->nOpenNum;
			pStream->FillString(" ,RecTime = now()") ;
			m_pDBLoong->Execute  ( pStream ) ;
			++pOlData;
		}
		m_pDBLoong->ReturnStream(pStream);
		return 0;
	}
	return GT_INVALID;
}
//------------------------------------------------------------------------------
DWORD CLoongDB::DelOneBuff( DWORD dwRoleID, DWORD dwBuffID )
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetDelete("buff");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and BuffID=") << dwBuffID;

	BOOL bRes = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------


DWORD CLoongDB::InsertBuffDirectly(DWORD dwRoleID,tagBuffSave * pBuffSave)
{
	if( !P_VALID(pBuffSave) )
		return GT_INVALID;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("INSERT INTO buff set RoleID = ") << dwRoleID;
		pStream->FillString(",SrcUnitID = ") << pBuffSave->dwSrcUnitID;
		pStream->FillString(",SrcSkillID = ") << pBuffSave->dwSrcSkillID;
		pStream->FillString(",ItemTypeID = ") << pBuffSave->dwItemTypeID;
		pStream->FillString(",ItemSerialID = ") << pBuffSave->n64Serial ;

		pStream->FillString(",BuffID = ") << pBuffSave->dwBuffID;
		pStream->FillString(",CurTick = ") << pBuffSave->nPersistTick ;
		pStream->FillString(",Level = ") << pBuffSave->n8Level;
		pStream->FillString(",CurLapTimes = ") << pBuffSave->n8CurLapTimes ;

		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return E_Success;
		}
		m_pDBLoong->ReturnStream(pStream);
	}
	return GT_INVALID;
}

//-----------------------------------------------------------------
DWORD CLoongDB::DelItemdelByTime(const CHAR *pszTime)
{
	if( !P_VALID(pszTime) )
		return GT_INVALID;	

	if(DelEquipDel(pszTime))
	{
		return E_Success;
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------
DWORD CLoongDB::DelEquipdelByTime(const CHAR *pszTime)
{
	if( !P_VALID(pszTime) )
		return GT_INVALID;

	if( DelEquipDel(pszTime) )
	{
		if(DelItemDel(pszTime))
		{
			return E_Success;
		}
	}

	return GT_INVALID;
}
//-----------------------------------------------------------------

// VOID CLoongDB::IsLoadShengLing(Beton::MyStream *pStream)
// {
//	if(!isOpenShengLing())
// 	{
// 		//�����Ҫ��
// 		//pStream->FillString(" and TypeID not in (9800001,9800002,9800003,9800004,9850001,9850002,9850003,9850004,9850005,9850006,9850007,9850008,9850101,9850102,9850103,9850104,9850105,9850106,9850107,9850108,9850201,9850202,9850203,9850204,9850205,9850206,9850207,9850208,3500001,3500002,3500003,3500004,3500005,3500010,3500011,3500012,3500013,3500014,3500015,3500021,3500022,3500031)");
// 	}
// }

VOID CLoongDB::GetYuanBaoDaiBiNum(INT32 &nNum, DWORD dwRoleID)
{
	nNum = 0;

	// ��ȡ��
	Beton::MyStream* pStream = m_pDBLoong->GetStream();

	// ��ʽ��
	pStream->SetSelect("ReciveYuanBaoDaiBi", "Num");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// ��ѯ���ݿ�
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		nNum = (*pResult)[0].GetInt();
	}

	//++m_dwReadTimes;

	BOOL bRet = TRUE;

	// ��ȡ��
	Beton::MyStream* pStream1 = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream1));

	// ��ʽ�����ݿ�������
	pStream1->SetDelete("ReciveYuanBaoDaiBi");
	pStream1->SetWhere();
	pStream1->FillString("RoleID=") << dwRoleID;

	// ִ��
	bRet = m_pDBLoong->Execute(pStream1);

	// �ͷ���
	m_pDBLoong->ReturnStream(pStream1);
}



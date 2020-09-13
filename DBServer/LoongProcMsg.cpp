//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongProcMsg.cpp
// author: Sxg
// actor:
// data: 2008-06-04
// last:
// brief: �������Ϸ���������յ�����Ϣ(�����Ϣ���ݵĺϷ��ԡ�����DB��������)
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/rankings_define.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_quest.h"
#include "../ServerDefine/msg_skill.h"
#include "../ServerDefine/msg_title.h"
#include "../ServerDefine/msg_vcard.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/msg_rankings.h"
#include "../ServerDefine/msg_marriage.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/pet_define.h"
#include "../ServerDefine/msg_city.h"
#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/msg_miracle.h"
#include "../ServerDefine/athletics_data_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../ServerDefine/guild_define.h"
#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/msg_common.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/msg_log.h"
#include "../ServerDefine/msg_account_reactive.h"
#include "LoongDB.h"
#include "Loong.h"
#include "../ServerDefine/msg_holySoul.h"

#include "../ServerDefine/msg_sexternal_links.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/msg_train.h"
#include "../ServerDefine/msg_guild_war_declare.h"
#include "../WorldDefine/SocialDef.h"
#include "../ServerDefine/msg_olinfo.h"

#include "../ServerDefine/msg_godbid.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_fashion.h"
#include "../ServerDefine/msg_god.h"
#include "../ServerDefine/msg_family_sprite.h"
#include "../ServerDefine/msg_buff.h"
#include "../ServerDefine/msg_family.h"
#include "../ServerDefine/msg_account_reactive.h"

const INT MAX_ROLEINFO_NUM_PER_MSG		= 100000;	// ÿ����Ϣ��roleinfo��������
const INT MAX_ROLEYBORDER_NUM_PER_MSG   = 100;		// ÿ����Ϣ��tagyuanbaoorder��������
const INT MAX_GROUPPURCHASE_CAPACITY_PER_MSG	= 1000000;	// ÿ����Ϣ��tagGPInfo��ռ�ĵ��������
const INT MAX_ACTIVITY_NUM_PER_MSG = 50;			// ÿ����Ϣ��tagActivityData��������
const INT32 MAX_RAREAREA_BUFF_SIZE			= 100 * 1000 * 1000;	// ϡ�����������ʱ����
const INT32 MAX_FAMILYSPRITE_BUFF_SIZE		= 100 * 1000 * 1000;	// �������������ʱ����
const INT32 MAX_FAMILYSPRITERE_BUFF_SIZE		= 100 * 1000 * 1000;	// ���������������������ʱ����
const INT32 MAX_FAMILYROLESPRITE_BUFF_SIZE		= 100 * 1000 * 1000;	// ����Ǽ����������ʱ����
//-------------------------------------------------------------------------------------------------------
// ��ʼ����Ϸ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InitWorldOK(DWORD pDBMsg, DWORD dwDummy)
{
	// ����Ϸ����������
	SendMsg( (LPVOID)pDBMsg, ((tagN_Certification*)pDBMsg)->dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��Ϸ����ر�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::WorldShutDown(DWORD pDBMsg, DWORD dwDummy)
{
	// ����Ϸ����������
	tagNDBS_WorldShutDown send;

	SendMsg( &send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��LoongWorld���س�ʼ�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoongWorldInitOK(DWORD pDBMsg, DWORD dwDummy)
{
	tagNDBS_LoongWorldInitOK	send;
	SendMsg( &send, send.dwSize); 
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����������Ҽ�����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllRoleInfo(DWORD pDBMsg, DWORD dwDummy)
{
	//	MGET_MSG(pRecv, pDBMsg, NDBC_LoadAllRoleInfo);

	INT32 nRoleNum = m_pDB->GetRoleCount();
	if(0 == nRoleNum)
	{
		tagNDBS_LoadAllRoleInfo Load;
		Load.dwErrorCode = E_Success;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagRoleInfo *pRoleInfo = new tagRoleInfo[nRoleNum];
	tagRoleInfo *pCur = pRoleInfo;
	INT nNum;
	if(!m_pDB->LoadAllRoleInfo(pRoleInfo, nNum))
	{
		delete []pRoleInfo;

		tagNDBS_LoadAllRoleInfo Load;
		Load.dwErrorCode = E_SystemError;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagRoleInfo) + sizeof(tagNDBS_LoadAllRoleInfo)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadAllRoleInfo LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadAllRoleInfo *pLoad = (tagNDBS_LoadAllRoleInfo *)pBuff;
	pLoad->dwErrorCode = E_Success;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	//if(nRoleNum != nNum)
	//{
	//	// ���ط���Ϣ //??
	//	
	//	ASSERT(nRoleNum == nNum);
	//}

	// �ְ�(�������ݰ�ͷ4���ֽ��еĸ�8λ�������ܣ����Զ԰��Ĵ�С������)
	while(nRoleNum > 0)
	{
		if(nRoleNum > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(pLoad->RoleInfo, pCur, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagRoleInfo));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagRoleInfo) + sizeof(tagNDBS_LoadAllRoleInfo);
			pLoad->nNum = MAX_ROLEINFO_NUM_PER_MSG;

			nRoleNum -= MAX_ROLEINFO_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->RoleInfo, pCur, nRoleNum * sizeof(tagRoleInfo));

			// ����������Ϣ��С
			pLoad->dwSize = (nRoleNum - 1) 
				* sizeof(tagRoleInfo) + sizeof(tagNDBS_LoadAllRoleInfo);
			pLoad->nNum = nRoleNum;

			nRoleNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pRoleInfo;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������ҵ�Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllYBAccount(DWORD pDBMsg, DWORD dwDummy)
{
	INT nAccountNum = m_pDB->GetAccountCount();
	if(0 == nAccountNum)
	{
		tagNDBS_LoadAllYBAccount Load;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagYBAccount *pYBAccount = new tagYBAccount[nAccountNum];
	tagYBAccount *pCur = pYBAccount;
	INT nNum;
	if(!m_pDB->LoadAllYBAccount(pYBAccount, nNum))
	{
		delete []pYBAccount;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYBAccount) + sizeof(tagNDBS_LoadAllYBAccount)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadAllYBAccount LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadAllYBAccount *pLoad = (tagNDBS_LoadAllYBAccount*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nAccountNum > 0)
	{
		if(nAccountNum > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(pLoad->Account, pCur, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYBAccount));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagYBAccount) + sizeof(tagNDBS_LoadAllYBAccount);
			pLoad->nNum = MAX_ROLEINFO_NUM_PER_MSG;

			nAccountNum -= MAX_ROLEINFO_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->Account, pCur, nAccountNum * sizeof(tagYBAccount));

			// ����������Ϣ��С
			pLoad->dwSize = (nAccountNum - 1) 
				* sizeof(tagYBAccount) + sizeof(tagNDBS_LoadAllYBAccount);
			pLoad->nNum = nAccountNum;

			nAccountNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pYBAccount;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���е�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllYBOrder(DWORD pDBMsg, DWORD dwDummy)
{
	INT nOrderNum = m_pDB->GetOrderCount();
	if(0 == nOrderNum)
	{
		tagNDBS_LoadAllOrder Load;
		Load.nNum = 0;
		Load.dwMaxOrderID = m_pDB->GetMaxOrderID();
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagYuanBaoOrder *pYBOrder = new tagYuanBaoOrder[nOrderNum];
	tagYuanBaoOrder *pCur = pYBOrder;
	INT nNum;
	if(!m_pDB->LoadAllYBOrder(pYBOrder, nNum))
	{
		delete []pYBOrder;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_LoadAllOrder)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadAllOrder LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadAllOrder *pLoad = (tagNDBS_LoadAllOrder*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nOrderNum > 0)
	{
		if(nOrderNum > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(pLoad->YBOrder, pCur, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_LoadAllOrder);
			pLoad->nNum = MAX_ROLEINFO_NUM_PER_MSG;

			nOrderNum -= MAX_ROLEINFO_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->YBOrder, pCur, nOrderNum * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (nOrderNum - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_LoadAllOrder);
			pLoad->nNum = nOrderNum;

			nOrderNum = 0;
			pCur = NULL;
		}
		pLoad->dwMaxOrderID	= m_pDB->GetMaxOrderID();

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pYBOrder;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ѯһ������ҽ��׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetRoleYBOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetRoleYBOrder);
	INT nOrderNum = 0;
	m_pDB->GetRoleYBOrderNum(pRecv->dwRoleID, nOrderNum);

	if(0 == nOrderNum)
	{
		tagNDBS_GetRoleYBOrder Load;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagYuanBaoOrder *pYBOrder = new tagYuanBaoOrder[nOrderNum];
	tagYuanBaoOrder *pCur = pYBOrder;
	INT nNum;
	if(!m_pDB->LoadRoleAllYBOrder(pYBOrder, nNum, pRecv->dwRoleID))
	{
		delete []pYBOrder;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[nOrderNum * sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_GetRoleYBOrder)];

	// ��ʼ����Ϣͷ
	tagNDBS_GetRoleYBOrder LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_GetRoleYBOrder *pLoad = (tagNDBS_GetRoleYBOrder*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nOrderNum > 0)
	{
		if(nOrderNum > MAX_ROLEYBORDER_NUM_PER_MSG)
		{
			memcpy(pLoad->YBOrder, pCur, MAX_ROLEYBORDER_NUM_PER_MSG * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ROLEYBORDER_NUM_PER_MSG - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_GetRoleYBOrder);
			pLoad->nNum = MAX_ROLEYBORDER_NUM_PER_MSG;

			nOrderNum -= MAX_ROLEYBORDER_NUM_PER_MSG;
			pCur = &pCur[MAX_ROLEYBORDER_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->YBOrder, pCur, nOrderNum * sizeof(tagYuanBaoOrder));

			// ����������Ϣ��С
			pLoad->dwSize = (nOrderNum - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(tagNDBS_GetRoleYBOrder);
			pLoad->nNum = nOrderNum;

			nOrderNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pYBOrder;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �����ɫ������Ϣ(�������ȼ������κ͵�ǰ���ڵ�ͼ��)
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadSimRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadSimRole);

	m_pLoadSimRole->dwErrorCode = E_Success;
	m_pLoadSimRole->dwAccountID = pRecv->dwAccountID;
	m_pLoadSimRole->sAccountCommon.Init();

	if(!m_pDB->LoadSimRole(m_pLoadSimRole->dwAccountID, m_pLoadSimRole->nNum, m_pLoadSimRole->SimRole))
	{
		// û�н�ɫʱ��Ҳ��ִ�е��ô�
		//m_pLoadSimRole->dwErrorCode = E_DBLoad_SimRoleFailed;
		m_pLoadSimRole->nNum = 0;		// ��ȡ����ʱ����С������С
	}
	
	
	DWORD dwChannelID = 0;
	if( m_pDB->LoadAccountChannelIDCommon(pRecv->dwAccountID,&dwChannelID))
	{
		//���ǻ���Ļ�
		if ( 0 != pRecv->dwChannelID )
		{
			if( 0 == dwChannelID)
			{
				// ����
				m_pDB->LockAccountCommon();

				tagNewAccountCommon met;
				met._dwChannelID = pRecv->dwChannelID;
				memcpy(met.szNewAccountName,pRecv->szAccount,sizeof(pRecv->szAccount));
				if ( !m_pDB->UpdateNewAccountData(&met,pRecv->dwAccountID))
				{
					// ����
					IMSG(_T("\n\n\n///////////////////////////////////////\n"));
					IMSG(_T("Caution: \n\tNew AccountData Update Failed! \n"));
					IMSG(_T("///////////////////////////////////////\n\n\n"));

					m_pLoadSimRole->dwErrorCode = E_DBLoad_SimRoleFailed;
				}

				// ����
				m_pDB->UnlockTables();
			}
		}
	}

	// ��ȡ��ȫ��(accountrel���б�����ؼ�¼������ȡʧ�ܣ���˵�����ݿ����ӳ�������)
	if(!m_pDB->LoadAccountCommon(&m_pLoadSimRole->sAccountCommon, pRecv->dwAccountID))
	{
		// ����
		m_pDB->LockAccountCommon();

		
		if(!m_pDB->LoadAccountCommon(&m_pLoadSimRole->sAccountCommon, pRecv->dwAccountID))
		{
			tagNewAccountCommon ment;
			ment._dwChannelID = pRecv->dwChannelID;
			memcpy(ment.szNewAccountName,pRecv->szAccount,sizeof(pRecv->szAccount));
			// ��accountrel���в����¼
			if(!m_pDB->InsertAccountCommon(pRecv->dwAccountID, &ment))
			{
				// ���ط�����Ϣ

				// ����
				IMSG(_T("\n\n\n///////////////////////////////////////\n"));
				IMSG(_T("Caution: \n\tCritical Error! DB connect lost! May be need to restart server!\n"));
				IMSG(_T("///////////////////////////////////////\n\n\n"));

				m_pLoadSimRole->dwErrorCode = E_DBLoad_SimRoleFailed;
			}
		}

		// ����
		m_pDB->UnlockTables();

		// �ٴζ�ȡ
		m_pDB->LoadAccountCommon(&m_pLoadSimRole->sAccountCommon, pRecv->dwAccountID);
	}

	

	// ����������Ϣ��С
	m_pLoadSimRole->dwSize = sizeof(tagNDBS_LoadSimRole) 
		- sizeof(tagSimRole) * (MAX_ROLENUM_ONEACCOUNT - m_pLoadSimRole->nNum);

	// ��������Ϸ������
	SendMsg( m_pLoadSimRole, m_pLoadSimRole->dwSize);

	return m_pLoadSimRole->dwErrorCode;
}
DWORD CLoong::LoadOneRoleInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadOneRoleInfo);
	if(!P_VALID(pRecv))
		return GT_INVALID;
	tagNDBS_LoadOneRoleInfo msg;
	msg.dwErrorCode = E_Success;
	if(m_pDB->LoadOneRoleInfo(pRecv->dwAccountID,pRecv->dwRoleID,&msg.RoleInfo))
	{
		SendMsg( &msg, msg.dwSize );
	}

	return GT_INVALID;
}
//-------------------------------------------------------------------------------------------------------
// �����ɫ��Ϣ(ѡ����ɫ������Ϣ)
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadRole);

	ZeroMemory(m_pBuffer, X_ROLE_BUFF_SIZE);
	m_pLoadRole->dwID = m_pLoadRole->Crc32("NDBS_LoadRole");
	m_pLoadRole->dwAccountID = pRecv->dwAccountID;
	m_pLoadRole->dwErrorCode = E_Success;
	m_pLoadRole->dwRoleID = pRecv->dwRoleID;
	if(!m_pDB->LoadRole(pRecv->dwAccountID, pRecv->dwRoleID, &m_pLoadRole->RoleData))
	{
		m_pLoadRole->dwErrorCode = E_DBLoad_RoleDataFailed;
	}

	//�����ϴ�ʹ�õ����id
	m_pDB->UpdateLastUseRoleID(pRecv->dwRoleID,&pRecv->dwAccountID);

	// ���¼���m_LoadRole�Ĵ�С
	m_pLoadRole->dwSize = sizeof(tagNDBS_LoadRole) + m_pLoadRole->RoleData.dwSize - sizeof(m_pLoadRole->RoleData);

	// ��������Ϸ������
	SendMsg( m_pLoadRole, m_pLoadRole->dwSize);

	// ������ɫ�Ļ�����Ϣ
	tagNDBS_InitAccountReactiveInfo send;
	send.dwAccountID = pRecv->dwAccountID;
	send.dwRoleID = pRecv->dwRoleID;
	if( m_pDB->LoadAccountReactiveInfo(pRecv->dwAccountID, (tagAccountReactiveInfo*)(&send.info)) )
	{
		SendMsg( &send, send.dwSize );
	}

	return m_pLoadRole->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ����洢��ɫ����
//-------------------------------------------------------------------------------------------------------
VOID CLoong::SaveRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveRole);

	m_pDB->SaveRole(pRecv->dwRoleID, &pRecv->RoleData);
}

// ����洢��ɫ��������
VOID CLoong::SaveRoleInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveOneRoleInfo);

	m_pDB->SaveRoleInfo(pRecv->RoleInfo.dwRoleID, &pRecv->RoleInfo);
}

////-------------------------------------------------------------------------------------------------------
//// ɾ����ҵ�buff������item/equip��.����������ݿ�����Ҷ���ļ�¼
////-------------------------------------------------------------------------------------------------------
//VOID CLoong::DeleteRecord(DWORD pDBMsg, DWORD dwDummy)
//{
//	MGET_MSG(pRecv, pDBMsg, NDBC_RecordDel);
//
//	m_pDB->DeleteRecord(pRecv->dwRoleID, &pRecv->Record);
//}

//-------------------------------------------------------------------------------------------------------
// ���󴴽���ɫ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CreateRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CreateRole);

	m_pCreateRole->dwErrorCode = E_Success;
	m_pCreateRole->dwAccountID = pRecv->dwAccountID;
	memcpy(m_pCreateRole->SimRole.szRoleName, 
		pRecv->RoleInfo.szRoleName, sizeof(m_pCreateRole->SimRole.szRoleName));

	DWORD dwRoleNameCrc = GT_INVALID;
	DWORD dwNewRoleID = GT_INVALID;
	if(!m_pDB->CreateRole(dwNewRoleID, pRecv->dwAccountID, pRecv->RoleInfo, dwRoleNameCrc))
	{
		m_pCreateRole->dwErrorCode = E_DBInsert_RoleFailed;
	}
	else // ������ɫ�ɹ�
	{
		// ����������Ϣ
		m_pDB->InsertClanData(dwNewRoleID);

		// ������ɫ����
		m_pDB->InsertVCard(dwNewRoleID);

		// ��ʼ����ɫ������Ϣ
		memcpy(&m_pCreateRole->SimRole.Avatar, &pRecv->RoleInfo.Avatar, sizeof(m_pCreateRole->SimRole.Avatar));
		m_pCreateRole->SimRole.dwRoleID = dwNewRoleID;
		m_pCreateRole->SimRole.dwMapID = pRecv->RoleInfo.dwMapID;
		m_pCreateRole->SimRole.byLevel = pRecv->RoleInfo.byLevel;

		// ��ʼ��tagRoleInfo
		memcpy(m_pCreateRole->RoleInfo.szRoleName, 
			pRecv->RoleInfo.szRoleName, sizeof(m_pCreateRole->RoleInfo.szRoleName));
		m_pCreateRole->RoleInfo.dwAccountID = pRecv->dwAccountID;
		m_pCreateRole->RoleInfo.dwRoleID = dwNewRoleID;
		m_pCreateRole->RoleInfo.dwRoleNameCrc = dwRoleNameCrc;
		m_pCreateRole->RoleInfo.dwMapID = pRecv->RoleInfo.dwMapID;
		m_pCreateRole->RoleInfo.byLevel = pRecv->RoleInfo.byLevel;
		m_pCreateRole->RoleInfo.bySex = pRecv->RoleInfo.Avatar.bySex;
		m_pCreateRole->RoleInfo.dwLoverID = GT_INVALID;
		m_pCreateRole->RoleInfo.byClass = 1; //Ĭ������
		// Jason 2010-3-2
		m_pCreateRole->RoleInfo.dwGuildID = GT_INVALID;
		m_pCreateRole->RoleInfo.nMARelation = 0;
		m_pCreateRole->RoleInfo.dwFamilyID = GT_INVALID;
	}

	// ��������Ϸ������
	SendMsg( m_pCreateRole, m_pCreateRole->dwSize);

	return m_pLoadSimRole->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ����ɾ����ɫ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelRole);

	//// ��������������
	//if(m_pDB->BackupRole(pRecv->dwAccountID, pRecv->dwRoleID) != E_Success)
	//{
	//	// ���ط���Ϣ
	//}

	// ɾ������������
	//SendDelRole.dwErrorCode = m_pDB->DelRole(pRecv->dwAccountID, pRecv->dwRoleID);
	//m_pDB->DelClanData(pRecv->dwRoleID);
	//m_pDB->DelVCard(pRecv->dwRoleID);
	//
	//if(SendDelRole.dwErrorCode != E_Success)
	//{
	//	// ���ط���Ϣ
	//}
	//else
	//{
	//	// ��¼��������Ϣ -- ��֤��,�ü�¼��ʧ��,�������⴦��
	//	m_pDB->RecordInfoDelRole(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szRoleName, pRecv->szIP);
	//}
	// �ֿ����Ʒ
	DWORD dwMiscArg = (EICT_RoleWare & 0xff);
	dwMiscArg |= (EBS_Bind & 0xff) << 16;

	tagItemData ItemSerials[1000] = {0};
	PVOID pItemSerials = ItemSerials;
	INT nNum = 0;
	if (m_pDB->LoadRoleItemSerial(pRecv->dwRoleID, pItemSerials, nNum, dwMiscArg))
	{
		for(INT i=0; i<nNum; ++i)
		{
			if (MIsEquipment(ItemSerials[i].dwTypeID))
			{
				m_pDB->DelEquip(ItemSerials[i].n64Serial);
			}
			else
			{
				m_pDB->DelItem(ItemSerials[i].n64Serial);
			}
		}
	}

	//DWORD dwPetIDs[MAX_PETSOUL_NUM * 2] = {GT_INVALID};
	TArray<DWORD> dwPetIDs(MAX_PETSOUL_NUM * 2);
	INT nPetIDNum = 0;
	if (m_pDB->LoadRolePetIDs(dwPetIDs, pRecv->dwRoleID, nPetIDNum))
	{
		for(INT i=0; i<nPetIDNum && GT_VALID(dwPetIDs[i]); ++i)
		{
			// tbd:ɾ�����＼��
			m_pDB->DelPetSkill(GT_INVALID, dwPetIDs[i]);
			// ɾ������װ��
			m_pDB->DelPetEquip(dwPetIDs[i]);
			// ɾ����������
			m_pDB->DeletePetSoul(dwPetIDs[i]);
		}
	}

	// ���ý�ɫ����ɾ����־λ
	m_pDB->SetRoleRemoveFlag(pRecv->dwRoleID, pRecv->dwTime);
	m_pDB->SetRoleClanRemoveFlag(pRecv->dwRoleID);

	// ��¼��������Ϣ -- ��֤��,�ü�¼��ʧ��,�������⴦��
	m_pDB->RecordInfoDelRole(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szRoleName, pRecv->szIP);

	// ��������Ϸ������
	tagNDBS_DelRole SendDelRole;
	SendDelRole.dwErrorCode = E_Success;
	SendDelRole.dwAccountID = pRecv->dwAccountID;
	SendDelRole.dwRoleID = pRecv->dwRoleID;

	// ��������Ϸ������ 
	SendMsg( &SendDelRole, SendDelRole.dwSize);

	return SendDelRole.dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ����ָ���ɫ//??
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ResumeRole(DWORD pDBMsg, DWORD dwDummy)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------
// �����޸Ľ�ɫ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeRoleName(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveRenameInfo);

	//tagNDBS_ChangeRoleName	Send;	// ���ô���Ӧ�ò���ܶ࣬��������ʱ��������
	//Send.dwAccountID = pRecv->dwAccountID;
	//Send.dwRoleID = pRecv->dwRoleID;
	//Send.dwErrorCode = E_Success;
	//memcpy(Send.szNewRoleName, pRecv->szName, sizeof(Send.szNewRoleName));

	if(m_pDB->ChangeRoleName(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szName))
	{
		m_pDB->SaveNameHistory(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->szName);	// ��¼�ý�ɫʹ�ù�������
	}

	// ��������Ϸ������
	//SendMsg( &Send, Send.dwSize);
	//return Send.dwErrorCode;
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �����޸Ľ�ɫ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeRoleGuild(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ChangeRoleGuild);

	if(!m_pDB->ChangeRoleGuild(pRecv->dwRoleID, pRecv->dwGuildID))
	{
		ASSERT(0);
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> guildid change failed!\n"), pRecv->dwRoleID);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �޸Ľ�ɫDressMdID
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ChangeRoleDressMdID(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearRoleDressMdID);

	if(!m_pDB->ChangeRoleDressMdID(pRecv->dwRoleID, pRecv->wNewVal))
	{
		ASSERT(0);
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> dressmdid change failed!\n"), pRecv->dwRoleID);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵ�׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertFriend(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertFriend);

	if(!m_pDB->InsertFriend(pRecv->dwRoleID, &pRecv->FriendSave))
	{
		return E_DBInsert_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵĸ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateFriend(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateFriend);

	if(!m_pDB->UpdateFriend(pRecv->dwRoleID, &pRecv->FriendSave))
	{
		return E_DBInsert_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���ѵ�ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteFriend(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteFriend);

	if(!m_pDB->DeleteFriend(pRecv->dwRoleID, pRecv->dwFriendID))
	{
		return E_DBDelete_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ӻ��ѱ���ɾ�����к͸ý�ɫ��صļ�¼
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteAllFriRel(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelAllFriRel);

	if(!m_pDB->DeleteAllFriRel(pRecv->dwRoleID))
	{
		return E_DBDelete_FriendFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertFriendship(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertFriVal);

	if(!m_pDB->InsertFriendship(pRecv->dwRoleID, &pRecv->FriendshipSave))
	{
		return E_DBInsert_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵĸ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateFriendship(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateFriVal);

	if(!m_pDB->UpdateFriendship(pRecv->dwRoleID, &pRecv->FriendshipSave))
	{
		return E_DBInsert_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ѻöȵ�ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteFriendship(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteFriend);

	if(!m_pDB->DeleteFriendship(pRecv->dwRoleID, pRecv->dwFriendID))
	{
		return E_DBDelete_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ClearTeatherInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearBloodBrother);

	if(!m_pDB->ClearTeatherInfo(pRecv->dwTeatherD,pRecv->dwBrotherID))
	{
		return E_DBDelete_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �Ӻ��Ѷ��б���ɾ��������ý�ɫ��صļ�¼
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteAllFriValRel(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelAllFriValRel);

	if(!m_pDB->DeleteAllFriValRel(pRecv->dwRoleID))
	{
		return E_DBDelete_FriendshipFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBlack(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertBlack);

	if(!m_pDB->InsertBlack(pRecv->dwRoleID, pRecv->dwBlackID))
	{
		return E_DBInsert_BlackFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteBlack(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteBlack);


	if(!m_pDB->DeleteBlack(pRecv->dwRoleID, pRecv->dwBlackID))
	{
		return E_DBDelete_BlackFailed;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��е�׷��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertEnemy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertEnemy);

	tagNDBS_InsertEnemy Send;
	Send.dwErrorCode = E_Success;
	Send.dwRoleID = pRecv->dwRoleID;
	Send.dwEnemyID = pRecv->dwEnemyID;

	if(!m_pDB->InsertEnemy(pRecv->dwRoleID, pRecv->dwEnemyID))
	{
		Send.dwErrorCode = E_DBInsert_EnemyFailed;
	}

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return Send.dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��е�ɾ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteEnemy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteEnemy);

	tagNDBS_DeleteEnemy Send;
	Send.dwErrorCode = E_Success;
	Send.dwRoleID = pRecv->dwRoleID;
	Send.dwEnemyID = pRecv->dwEnemyID;

	if(!m_pDB->DeleteEnemy(pRecv->dwRoleID, pRecv->dwEnemyID))
	{
		Send.dwErrorCode = E_DBDelete_EnemyFailed;
	}

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return Send.dwErrorCode;
}

// ȡ���г���б����г�н�ɫIDΪdwRoleID�Ľ�ɫ��ID
DWORD CLoong::GetRoleListOfEnemy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetRoleListOfEnemy);
	
	DWORD	dwEnemyRoleList[MAX_ENEMYNUM] = {0};
	DWORD	dwRoleCnt = MAX_ENEMYNUM;
	
	if (!m_pDB->GetRoleListOfEnemy(pRecv->dwRoleID, dwEnemyRoleList, dwRoleCnt))
	{
		tagNDBS_GetRoleListOfEnemy send;
		send.dwRoleCount = 0;
		send.dwEnemyID = pRecv->dwRoleID;
		send.bEnemyLogin = pRecv->bEnemyLogin;
		SendMsg(&send, send.dwSize);
		return E_Success;
	}

	DWORD dwMsgSize = sizeof(tagNDBS_GetRoleListOfEnemy) + (dwRoleCnt - 1) * sizeof(DWORD);
	
	MCREATE_MSG(pSend, dwMsgSize, NDBS_GetRoleListOfEnemy);
	if (!P_VALID(pSend))
	{
		return GT_INVALID;
	}

	pSend->dwRoleCount = dwRoleCnt;
	pSend->dwEnemyID = pRecv->dwRoleID;
	pSend->bEnemyLogin = pRecv->bEnemyLogin;
	memcpy(pSend->pRoleIDData, dwEnemyRoleList, pSend->dwRoleCount * sizeof(DWORD));
	SendMsg(pSend, pSend->dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::AcceptQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AcceptQuest);

	if(!m_pDB->InsertTask(pRecv->dwRoleID, &pRecv->acceptQuest))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CompleteQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CompleteQuest);

	if(!m_pDB->SaveTaskDone(pRecv->dwRoleID, &pRecv->QuestDone, 1)
		|| !m_pDB->DeleteTask(pRecv->dwRoleID, pRecv->QuestDone.u16QuestID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����ճ�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DailyCompleteQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DailyCompleteQuest);

	if(!m_pDB->SaveDailyTaskDone(pRecv->dwRoleID, &pRecv->QuestDone, 1))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����ճ���������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ClearDailyCompleteQuest(DWORD pDBMsg, DWORD dwDummy)
{
   MGET_MSG(pRecv, pDBMsg, NDBC_ClearDailyCompleteQuest);

   for (DWORD dwI = 0 ; dwI <  pRecv->dwCount ; dwI++)
   {
	   if(!m_pDB->SaveDailyTaskDone(pRecv->dwRoleID, &pRecv->pData[dwI], 1))
	   {
		   continue;
	   }
   }
   return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DiscardQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DiscardQuest);

	if(!m_pDB->DeleteTask(pRecv->dwRoleID, pRecv->u16QuestID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ʒ64λ��ŵ������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetItemInfo(DWORD pDBMsg, DWORD dwDummy)
{
	//MGET_MSG(pRecv, pDBMsg, NDBC_GetItemInfo);

	tagNDBS_GetItemInfo send;

	m_pDB->GetMaxMinSerial(send.n64MaxSerial, send.n64MinSerial);

	// ��������Ϸ������
	SendMsg( &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����Ʒ���ٱ�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadNewItem2BaiBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBaiBao);

	if(pRecv->nFreeSpaceSize <=0 || pRecv->nFreeSpaceSize > SPACE_BAIBAO_BAG)
	{
		return GT_INVALID;
	}

	m_pLoadBaiBao->dwAccountID = pRecv->dwAccountID;
	m_pLoadBaiBao->dwErrorCode = E_Success;

	INT32 nTmpSize = 0;

	if(!m_pDB->LoadItemBaiBao(pRecv->dwAccountID, pRecv->nFreeSpaceSize, pRecv->n64Serial, m_pLoadBaiBao->nRetNum, nTmpSize, m_pLoadBaiBao->byData))
	{
		m_pLoadBaiBao->dwErrorCode = E_DBLoad_BaiBao_Failed;
	}

	// ���¼�����Ϣ��С
	m_pLoadBaiBao->dwSize = sizeof(tagNDBS_LoadBaiBao) + nTmpSize - sizeof(m_pLoadBaiBao->byData);

	// ��������Ϸ������
	SendMsg( m_pLoadBaiBao, m_pLoadBaiBao->dwSize);

	return m_pLoadBaiBao->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�ٱ�����Ԫ����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadBaiBaoYuanBaoInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBaiBaoYuanBao);

	INT32 nYuanBao;
	if (!m_pDB->LoadBaiBaoYuanBao(pRecv->dwAccountID, nYuanBao))
	{
		return GT_INVALID;
	}

	tagNDBS_LoadBaiBaoYuanBao send;
	send.dwAccountID	= pRecv->dwAccountID;
	send.nBaiBaoYuanBao	= nYuanBao;

	// ��������Ϸ������
	SendMsg( &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��item_baibao����ɾ��ָ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelBaiBaoItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelBaiBaoItem);

	if(!m_pDB->DelItem(pRecv->n64Serial, "item_baibao"))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��item_baibao����ɾ��ָ��װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DelBaiBaoEquip);

	if(m_pDB->DelItem(pRecv->n64Serial, "item_baibao"))
	{
		return m_pDB->DelEquipSpec(pRecv->n64Serial, "equip_baibao");
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ��Ҫ��¼����ƷTypeID
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadItemNeedLog(DWORD pDBMsg, DWORD dwDummy)
{
	//	MGET_MSG(pRecv, pDBMsg, NDBC_LoadItemNeedLog);

	tagNDBS_LoadItemNeedLog Send;
	Send.nItemNum		= 0;
	Send.dwErrorCode	= E_Success;

	if(!m_pDB->LoadItemNeedLog(Send.dwTypeID, Send.nItemNum))
	{
		Send.dwErrorCode = E_DBLoad_ItemNeedLog_Failed;
	}

	// ���¼����С
	Send.dwSize += (Send.nItemNum - MAX_ITEM_NEEDLOG_NUM) * sizeof(DWORD);

	// ��������Ϸ������
	SendMsg( &Send, Send.dwSize);

	return 0;
}


//-------------------------------------------------------------------------------------------------------
// ����Ʒ���в�������Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewItem);

	if(!m_pDB->InsertItem(p->item))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ���в�����װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewEquip);

	if(!m_pDB->InsertEquip(p->equip))
	{
		return GT_INVALID;
	}

	return 0;
}
// Jason 2010-4-14 v2.0.0 �������
DWORD CLoong::InsertFabao(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewFabao);
	if(!m_pDB->InsertFabao(p->fabao))
	{
		return GT_INVALID;
	}
	return 0;
}
DWORD CLoong::UpdateFabaoSpec(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateFabaoAtt);
	if(!m_pDB->UpdateFabaoSpec(p->n64FabaoID,p->fabaoSpec))
	{
		return GT_INVALID;
	}
	return 0;
}
DWORD CLoong::InsertSoulCrystal(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewSoulCrystal);
	if(!m_pDB->InsertSoulCrystal(p->obj))
	{
		return GT_INVALID;
	}
	return 0;
}
DWORD CLoong::UpdateSoulCrystalSpec(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateSoulCrystal);
	if(!m_pDB->UpdateSoulCrystalSpec(p->n64ID,p->objspec))
	{
		return GT_INVALID;
	}
	return 0;
}

// wcy 2012-03-31 ʥ�����
DWORD CLoong::InsertHoly(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewHoly);
	if(!m_pDB->InsertHoly(p->holy))
	{
		return GT_INVALID;
	}
	return 0;
}

// wcy 2012-04-18 ʥ�����
DWORD CLoong::InsertHolyEquip(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewHolyEquip);
	if(!m_pDB->InsertHolyEquip(p->holyEquip))
	{
		return GT_INVALID;
	}
	return 0;
}

DWORD CLoong::UpdateHolySpec(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateHolyAtt);
	if(!m_pDB->UpdateHolySpec(p->n64HolyID,p->holySpec))
	{
		return GT_INVALID;
	}
	return 0;
}

DWORD CLoong::UpdateHolyCoValue(DWORD pDBMsg,DWORD dwDummy)
{
	if (P_VALID(pDBMsg))
		return m_pDB->UpdateHolyCoValue(pDBMsg, dwDummy);

	return E_Success;
}


DWORD CLoong::UpdateHolyEquipSpec(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateHolyEquipAtt);
	if(!m_pDB->UpdateHolyEquipSpec(p->n64ID,p->holyEquipSpec))
	{
		return GT_INVALID;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ����ɾ����Ʒ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DelItem);

	if(!m_pDB->DelItem(p->n64Serial))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ��ɾ��װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DelEquip);

	if(!m_pDB->DelEquip(p->n64Serial))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ��ɾ������ָ����ɫ��ָ��������ָ������װ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DelEquipFromRole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DelEquipFromRole);

	if(!m_pDB->DelEquipFromRole(p->dwRoleID, p->dwTypeID))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����װ��ר������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateEquipSpec(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateEquipAtt);

	if(!m_pDB->UpdateEquipSpec(p->equipSpecUpdate))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����װ��ר�õ�����������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateEquipSingleAtt(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateEquipSingleAtt);

	if(!m_pDB->UpdateEquipSingleAtt(p->equipSingleSpecUpdate))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����װ��ǩ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveEquipSignature(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_EquipSignature);

	if(!m_pDB->SaveEquipSignature(p->n64Serial, p->dwSignRoleID, p->szContent))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ʒ���в�������Ʒ -- item_baibao
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBaiBaoItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewBaiBaoItem);

	if(!m_pDB->InsertItem(p->item, "item_baibao"))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��װ�������Ʒ���в�����װ�� -- item_baibao,equip_baibao
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_NewBaiBaoEquip);

	if(!m_pDB->InsertEquip(p->equip, TRUE))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ƺű��в����³ƺż�¼
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertTitle(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_TitleInsert);

	if(!m_pDB->InsertTitle(p->dwRoleID, &p->titleSave))
	{
		return GT_INVALID;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------------
// ɾ���ƺ�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveTitle(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_TitleDelete);

	if(!m_pDB->RemoveTitle(p->dwRoleID, &p->titleSave))
	{
		return GT_INVALID;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------------
// ���½�ɫ������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateVCard(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_VCardChg);

	if (!m_pDB->UpdateVCard(p->dwRoleID, &(p->VCardData)))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadVCard(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetOffLineVCard);

	MCREATE_MSG(pSend, 2000, NDBS_GetOffLineVCard);

	pSend->dwRoleID = pRecv->dwRoleID;
	pSend->dwQueryID = pRecv->dwQueryID;
	PVOID pDataLoad = pSend->byData;
	
	if (m_pDB->LoadVCard(pDataLoad, pRecv->dwRoleID))
	{
		pSend->dwErrCode = E_VCard_Success;
		DWORD dwVCardSize = (BYTE*)pDataLoad - pSend->byData;
		pSend->dwSize = sizeof(tagNDBS_GetOffLineVCard) - 1 + dwVCardSize;
	}
	else
	{
		pSend->dwErrCode = E_VCard_Failed;
		pSend->dwSize = sizeof(tagNDBS_GetOffLineVCard);
	}

	SendMsg( pSend, pSend->dwSize);		
	MDEL_MSG(pSend);

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ��Ӽ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_AddSkill);

	if(!m_pDB->InsertSkill(p->dwRoleID, &p->Skill))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_RemoveSkill);

	if(!m_pDB->DelSkill(p->dwRoleID, p->dwSkillID))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ɾ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateSkill(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateSkill);

	PVOID pData = &(p->Skill);

	if(!m_pDB->UpdateSkill(p->dwRoleID, &p->Skill))
	{
		return GT_INVALID;
	}

	return 0;
}


//-------------------------------------------------------------------------------------------------------
// ������Ʒ��ȴʱ��
//-------------------------------------------------------------------------------------------------------
//DWORD CLoong::ReplaceItemCDTime(DWORD pDBMsg, DWORD dwDummy)
//{
//	MGET_MSG(p, pDBMsg, NDBC_ReplaceItemCDTime);
//
//	if(!m_pDB->ReplaceItemCDTime(p->dwRoleID, p->byCDTime, p->nNum))
//	{
//		return GT_INVALID;
//	}
//
//	return 0;
//}

//-------------------------------------------------------------------------------------------------------
// ����һ��Ԫ�������˻�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertYBAccount(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertAccount);

	if(!m_pDB->InsertYBAccount(pRecv->dwRoleID))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻���Ԫ������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateAccountYB(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynAccountYB);

	if(!m_pDB->UpdateAccountYB(pRecv->dwRoleID, pRecv->nYuanBao))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��Ľ�Ǯ����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateAccountSilver(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynAccountSilver);

	if(!m_pDB->UpdateAccountSilver(pRecv->dwRoleID, &pRecv->Money))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻����۶������ύ״̬
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateSubmitSell(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynSubmitSell);

	if(!m_pDB->UpdateSubmitSell(pRecv->dwRoleID, pRecv->bSellOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�������˻��չ��������ύ״̬
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateSubmitBuy(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SynSubmitBuy);

	if (!m_pDB->UpdateSubmitBuy(pRecv->dwRoleID, pRecv->bBuyOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertOrder);

	if(!m_pDB->InsertOrder(pRecv->YBOrder.dwRoleID, &pRecv->YBOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ������ر�Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CompleteOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CompleteOrder);

	if(!m_pDB->CompleteOrder(GT_INVALID, &pRecv->CompleteOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ����Ԫ�����׶���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RefreshOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RefreshOrder);

	if(!m_pDB->RefreshOrder(GT_INVALID, &pRecv->RefreshOrder))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ɾ�����ݿ���ʱ�䳬����������ж���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteYBOrder(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteYBOrder);
	if(!m_pDB->DeleteYBOrder(GT_INVALID, &pRecv->dwDeleteTime))
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ȡ�����ݿ���ָ���˺ŵİٱ���Log��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetBaibaoLogInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBaiBaoLog);

	ZeroMemory(m_pBuffer, X_ROLE_BUFF_SIZE);
	m_pLoadBaiBaoLog->dwID = m_pLoadBaiBaoLog->Crc32("NDBS_LoadBaiBaoLog");
	m_pLoadBaiBaoLog->dwAccountID = pRecv->dwAccountID;
	m_pLoadBaiBaoLog->dwErrorCode = E_Success;

	INT32 nTmpSize = 0;

	if (!m_pDB->LoadBaibaoLogInfo(pRecv->dwAccountID, m_pLoadBaiBaoLog->nLogNum, nTmpSize, m_pLoadBaiBaoLog->byData))
	{
		m_pLoadBaiBaoLog->dwErrorCode = E_DBLoad_BaiBaoLog_Failed;
	}

	// ���¼�����Ϣ��С
	m_pLoadBaiBaoLog->dwSize = sizeof(tagNDBS_LoadBaiBaoLog) + nTmpSize - sizeof(m_pLoadBaiBaoLog->byData);

	// ��������Ϸ������
	SendMsg( m_pLoadBaiBaoLog, m_pLoadBaiBaoLog->dwSize);

	return m_pLoadBaiBaoLog->dwErrorCode;
}

//-------------------------------------------------------------------------------------------------------
// ��ָ���˻��в���һ��Log��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertBaibaoLog(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AddBaiBaoLog);

	return m_pDB->InsertBaibaoLogInfo(pRecv->dwAccountID, (LPVOID)&pRecv->LogInfo);
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ���а����Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadAllGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	INT nGPInfoNum = m_pDB->GetGPInfoCount();
	if(0 == nGPInfoNum)
	{
		tagNDBS_GetAllGPInfo Load;
		Load.dwErrorCode = E_Success;
		Load.nGPInfoNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagGPInfo *pGPInfo = (tagGPInfo*)(new BYTE[m_pDB->GetGPInfoSize()]);
	tagGPInfo *pCur = pGPInfo;

	if (!m_pDB->LoadAllGPInfo(pGPInfo, nGPInfoNum))
	{
		tagNDBS_GetAllGPInfo Load;
		Load.dwErrorCode = E_DBLoad_GroupPurchase_Faild;
		Load.nGPInfoNum = 0;
		SendMsg( &Load, Load.dwSize);		

		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_GROUPPURCHASE_CAPACITY_PER_MSG + sizeof(tagNDBS_GetAllGPInfo)];
	ZeroMemory(pBuff, MAX_GROUPPURCHASE_CAPACITY_PER_MSG + sizeof(tagNDBS_GetAllGPInfo));

	// ��ʼ����Ϣͷ
	tagNDBS_GetAllGPInfo LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_GetAllGPInfo *pLoad = (tagNDBS_GetAllGPInfo*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize - sizeof(tagGPInfo);
	pLoad->nGPInfoNum = 0;

	tagGPInfo *pTmpGPInfo = pCur;
	while (nGPInfoNum > 0)
	{
		pLoad->dwSize = LoadTemp.dwSize - sizeof(tagGPInfo);
		pLoad->nGPInfoNum = 0;

		INT32 nTmpSize = 0;

		while (nTmpSize < MAX_GROUPPURCHASE_CAPACITY_PER_MSG)
		{
			INT32 nCurSize = pTmpGPInfo->Size();

			// һ���Ź���Ϣ�����ܳ����������������������ѭ��
			ASSERT(nCurSize <= MAX_GROUPPURCHASE_CAPACITY_PER_MSG);

			if (nTmpSize + nCurSize > MAX_GROUPPURCHASE_CAPACITY_PER_MSG)
			{
				break;
			}
			else
			{
				nTmpSize += nCurSize;
				pTmpGPInfo = (tagGPInfo*)((BYTE*)pTmpGPInfo + nCurSize);

				pLoad->nGPInfoNum++;

				nGPInfoNum--;
				if (nGPInfoNum <= 0)
				{
					break;
				}
			}
		}

		pLoad->dwSize += nTmpSize;
		memcpy(pLoad->GPInfoData, pCur, nTmpSize);
		pCur = pTmpGPInfo;

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	SAFE_DEL_ARRAY(pBuff);
	SAFE_DEL_ARRAY(pGPInfo);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ׷��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::AddGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AddGPInfo);

	return m_pDB->AddGPInfo(&(pRecv->GPInfo));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��һ���Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RemoveGPInfo);

	return m_pDB->RemoveGPInfo(&(pRecv->GpInfoKey));
}

//-------------------------------------------------------------------------------------------------------
// ɾ��ָ���������е��Ź���Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveGuildGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RemoveGuildGPInfo);

	return m_pDB->RemoveGuildGPInfo(pRecv->dwGuildID);
}

//-------------------------------------------------------------------------------------------------------
// �����Ź���Ӧ���б�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateGPInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateGPInfo);

	return m_pDB->UpdateGPInfo(&(pRecv->GpInfoKey), pRecv->dwNewParticipator);
}

//-------------------------------------------------------------------------------------------------------
// �����Ź�ʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateGPTime(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateGPTime);

	return m_pDB->UpdateGPTime();
}

DWORD CLoong::SaveLimitItemInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateLimitItemInfo);

	return m_pDB->SaveLimitItemInfo(pRecv->LimitItemInfo.dwID,&pRecv->LimitItemInfo);
}

DWORD CLoong::LoadLimitItemInfo(DWORD pDBMsg, DWORD dwDummy)
{
    MGET_MSG(pRecv, pDBMsg, NDBC_LoadLimitItemInfo);
	INT nInfoNum = m_pDB->GetLimitItemInfoCount();
	if(0 == nInfoNum)
	{
		tagNDBS_LoadLimitItemInfo Load;
		Load.dwErrorCode = E_Success;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}
	else
	{
		DWORD dwSize = sizeof(tagNDBS_LoadLimitItemInfo) + (nInfoNum-1)*sizeof(tagMallItemLimitNum);
		MCREATE_MSG(pSend,dwSize, NDBS_LoadLimitItemInfo); 
        pSend->nNum = nInfoNum;
	    BOOL bRet = m_pDB->LoadLimitItemInfo(&pSend->LimitItemInfo,nInfoNum);
		if ( bRet )
		{
			pSend->dwErrorCode = E_Success;
		}
		else
		{
			pSend->dwErrorCode = E_DBLoad_LimitItemInfo_Faild;
		}

		SendMsg(pSend,pSend->dwSize);
		MDEL_MSG(pSend);

	}
	return E_Success;



}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����VIP̯λ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetAllVIPStallInfo(DWORD pDBMsg, DWORD dwDummy)
{
	tagNDBS_GetAllVIPStallInfo send;

	// ��ʼ������VIP̯λ����
	for (int i=0; i<VIP_STALL_MAX_NUM; i++)
	{
		send.VipStallInfo[i].byStallIndex	= GT_INVALID;
		send.VipStallInfo[i].dwOwnerID		= GT_INVALID;
		send.VipStallInfo[i].nRemainTime	= GT_INVALID;
	}

	if (!m_pDB->GetAllVIPStallInfo(send.VipStallInfo))
	{
		return GT_INVALID;
	}

	SendMsg( &send, send.dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����һ��VIP̯λ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::ApplyVIPStall(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ApplyVIPStall);

	return m_pDB->ApplyVIPStall(&(pRecv->VipStallInfo));
}

//-------------------------------------------------------------------------------------------------------
// ����VIP̯λʣ��ʱ��
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateVIPStall(DWORD pDBMsg, DWORD dwDummy)
{
	return m_pDB->UpdateVIPStallTime();
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ�̶������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadActivityData(DWORD pDBMsg, DWORD dwDummy)
{
	INT nActivityNum = m_pDB->GetActivityCount();

	if(0==nActivityNum)
	{
		tagNDBS_LoadActivityData Load;
		Load.nNum = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagActivityData *pActivityData = new tagActivityData[nActivityNum];
	tagActivityData *pCur = pActivityData;

	INT nNum;
	if(!m_pDB->LoadActivityData(pActivityData, nNum))
	{
		delete []pActivityData;
		return GT_INVALID;
	}

	// ����ռ�
	CHAR *pBuff = new CHAR[MAX_ACTIVITY_NUM_PER_MSG * sizeof(tagActivityData) + sizeof(tagNDBS_LoadActivityData)];

	// ��ʼ����Ϣͷ
	tagNDBS_LoadActivityData LoadTemp; // �����ڳ�ʼ����Ϣͷ
	tagNDBS_LoadActivityData *pLoad = (tagNDBS_LoadActivityData*)pBuff;
	pLoad->dwID = LoadTemp.dwID;
	pLoad->dwSize = LoadTemp.dwSize;
	pLoad->nNum = 0;

	while(nActivityNum > 0)
	{
		if(nActivityNum > MAX_ACTIVITY_NUM_PER_MSG)
		{
			memcpy(pLoad->Activity, pCur, MAX_ACTIVITY_NUM_PER_MSG * sizeof(tagActivityData));

			// ����������Ϣ��С
			pLoad->dwSize = (MAX_ACTIVITY_NUM_PER_MSG - 1) 
				* sizeof(tagActivityData) + sizeof(tagNDBS_LoadActivityData);
			pLoad->nNum = MAX_ACTIVITY_NUM_PER_MSG;

			nActivityNum -= MAX_ACTIVITY_NUM_PER_MSG;
			pCur = &pCur[MAX_ACTIVITY_NUM_PER_MSG];
		}
		else
		{
			memcpy(pLoad->Activity, pCur, nActivityNum * sizeof(tagActivityData));

			// ����������Ϣ��С
			pLoad->dwSize = (nActivityNum - 1) 
				* sizeof(tagActivityData) + sizeof(tagNDBS_LoadActivityData);
			pLoad->nNum = nActivityNum;

			nActivityNum = 0;
			pCur = NULL;
		}

		SendMsg( pLoad, pLoad->dwSize);
	}

	// �ͷ��ڴ�
	delete []pBuff;
	delete []pActivityData;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �洢�̶���ű�����
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveActivityData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveActivityData);

	if(!m_pDB->SaveActivityData(pRecv->Activity.dwID, &pRecv->Activity))
	{
		return GT_INVALID;
	}

	return 0;
}

DWORD CLoong::ClearMergedScriptData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearMergedScriptData);

	if(!m_pDB->ClearMergedScriptData())
	{
		return GT_INVALID;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���¿����������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateTreasureSum(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateTreasureSum);

	if(!m_pDB->UpdateTreasureSum(pRecv->dwRoleID, pRecv->nSum))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RoleGetMarriage(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RoleGetMarriage);

	//����role1��db
	if(!m_pDB->RoleGetMarriage(pRecv->dwRoleID1, pRecv->dwRoleID2))
	{
		return GT_INVALID;
	}
	//����role2��db
	if(!m_pDB->RoleGetMarriage(pRecv->dwRoleID2, pRecv->dwRoleID1))
	{
		return GT_INVALID;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------
// ���
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RoleBreakMarriage(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RoleBreakMarriage);

	//����role1��db
	if(!m_pDB->RoleBreakMarriage(pRecv->dwRoleID1, pRecv->dwRoleID2))
	{
		return GT_INVALID;
	}
	//����role2��db
	if(!m_pDB->RoleBreakMarriage(pRecv->dwRoleID2, pRecv->dwRoleID1))
	{
		return GT_INVALID;
	}
	return 0;
}

DWORD CLoong::LoadExtLinks(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetExternalLinks);

	if( P_VALID(pRecv) )
	{
		tagExternalLink * p = NULL;
		int num = 0;
		if( m_pDB->LoadExtLinks( p , num ) )
		{
			int size = sizeof(tagNDBS_GetExternalLinks) + (num - 1) * sizeof(tagExternalLink);
			MCREATE_MSG(pSend,size,NDBS_GetExternalLinks);
			pSend-> dwErrorCode = 0;
			pSend-> dwElementNum = num;
			memcpy(pSend->links , p, num * sizeof(tagExternalLink));
			SendMsg(pSend,pSend->dwSize);
			//g_MemPoolSafe.Free  (pSend);
			g_MemPoolSafe.Free  (p);
			return 0;
		}
	}
	tagNDBS_GetExternalLinks msg;
	msg.dwErrorCode = GT_INVALID;
	msg.dwElementNum = 0;
	memset(msg.links,0,sizeof(tagExternalLink));
	SendMsg(&msg,msg.dwSize);
	return GT_INVALID;
}

DWORD CLoong::OfflineExpReward(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateAccountOfflineRewardInfo);

	if( P_VALID(pRecv) )
	{
		m_pDB->OfflineExperienceReward( pRecv->dwAccountID , pRecv->dwRoleID );
		return 0;
	}
	return GT_INVALID;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadCityInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadCityInfo);

	// ������Ϣ
	MCREATE_MSG(pSend, MAX_CITY_BUFF_SIZE, NDBS_LoadCityInfo);

	if(!m_pDB->LoadCityInfo(pRecv->dwCityID, pSend))
	{
		return GT_INVALID;
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNDBS_LoadCityInfo) + (pSend->nSignUpNum - 1) * sizeof(tagCitySignUp);

	// ������Ϸ���������
	m_pServer->Send(ID_GAMESERVER, pSend, pSend->dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����Ĭ�ϳ�����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::CreateCityInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CreateCityInfo);

	if(!m_pDB->CreateCityInfo(pRecv->nCityNum, pRecv->dwCityID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ���������Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveCityInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveCityAtt);

	if(!m_pDB->SaveCityInfo(&(pRecv->sCityAtt)))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����ָ����������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::UpdateCityAtt(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveAppointedAtt);

	if(!m_pDB->UpdateCityInfo(pRecv->dwCityID, &(pRecv->sCityAtt)))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡ����ָ����������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::GetGuildClanValue(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetGuildClanValue);

	// ����������Ϣ
	INT nMsgSize = sizeof(tagNDBS_GetGuildClanValue) + sizeof(tagGuildClanValue) * (pRecv->nGuildNum - 1);
	MCREATE_MSG(pSend, nMsgSize, NDBS_GetGuildClanValue);

	pSend->eClanType	= pRecv->eClanType;
	pSend->nGuildNum	= pRecv->nGuildNum;

	if(!m_pDB->GetGuildClanVal(pSend->eClanType, pSend->nGuildNum, pRecv->dwGuildID, pSend->sClanValue))
	{
		return GT_INVALID;
	}

	// ���¼�����Ϣ��С
	pSend->dwSize = sizeof(tagNDBS_GetGuildClanValue) + sizeof(tagGuildClanValue) * (pSend->nGuildNum - 1);

	// ��������Ϸ����
	m_pServer->Send(ID_GAMESERVER, pSend, pSend->dwSize);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// �ɽ缼���������
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveMiraclePrcNum(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveMiraclePrcNum);

	if(!m_pDB->UpdateRoleMiraclePrcNum(pRecv->dwRoleID, pRecv->n16MiraclePrcNum))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ��ȡʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadMasterApprenticeData);

	tagNDBS_LoadMasterApprenticeData send;

	if(!m_pDB->LoadMasterApprenticeData(pRecv->dwMasterID, &send))
	{
		return GT_INVALID;
	}

	// ������Ϸ���������
	SendMsg(&send, send.dwSize);
	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����ʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveAllMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveAllMasterApprenticeData);

	if (!m_pDB->SaveAllMasterApprenticeData(&(pRecv->sMasterApprentice)))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����һ��ʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::SaveOneMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveOneMasterApprenticeData);

	if (!m_pDB->SaveOneMasterApprenticeData(pRecv->dwMasterID, &(pRecv->sOneMAData)))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����һ��ʦͽ��Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertMasterApprenticeData);

	if(!m_pDB->InsertMasterApprenticeData(pRecv->sMasterApprentice.dwID, &(pRecv->sMasterApprentice)))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ɾ��һ��ͽ����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::DeleteAnApprentice(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteAnApprentice);

	if (!m_pDB->DeleteAnApprentice(pRecv->dwMasterID, pRecv->dwApprenticeID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// ����ǿ������ʦ�������б�
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::LoadForceBreakOutApprenticeList(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadForceBreakOutApprenticeList);

	// ����������Ϣ
	tagNDBS_LoadForceBreakOutApprenticeList* pSend = NULL;

	if (!m_pDB->LoadForceBreakOutApprenticeList((VOID*)this))
	{
		return GT_INVALID;
	}

	return GT_INVALID;
}
//-------------------------------------------------------------------------------------------------------
// ��ǿ������ʦ�������б��в���һ��ͽ����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::InsertForceBreakOutApprentice(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertForceBreakOutApprentice);
	
	if (!m_pDB->InsertForceBreakOutApprentice(pRecv->sForceBreakOutApprentice))
	{
		return GT_INVALID;
	}
	return E_Success;
}
//-------------------------------------------------------------------------------------------------------
// ��ǿ������ʦ�������б���ɾ��һ��ͽ����Ϣ
//-------------------------------------------------------------------------------------------------------
DWORD CLoong::RemoveForceBreakOutApprentice(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RemoveForceBreakOutApprentice);
	
	if (!m_pDB->RemoveForceBreakOutApprentice(pRecv->sForceBreakOutApprentice))
	{
		return GT_INVALID;
	}
	return E_Success;
}

DWORD CLoong::ModifyApprenticeJingWuPoint(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ModifyApprenticeJingWuPoint);

	if (!m_pDB->ModifyApprenticeJingWuPoint(pRecv->dwApprenticeID, pRecv->dwJingWuPoint))
	{
		return GT_INVALID;
	}
	return E_Success;
}

DWORD CLoong::SaveSerialVal(DWORD pDBMsg, DWORD dwDummy)
{

	MGET_MSG(pRecv, pDBMsg, NDBC_SaveSerialValToDB);
	if ( P_VALID(pRecv))
	{
		if(!m_pDB->SaveSerialVal(pRecv->dwSerialId,pRecv))
		{
			return GT_INVALID;
		}
	}
	return E_Success;
}

DWORD CLoong::SaveArenaData(DWORD pDBMsg, DWORD dwDummy)
{
     MGET_MSG(pRecv, pDBMsg, NDBC_SaveArenaDataToDB);
	 if ( P_VALID(pRecv))
	 {
          if(!m_pDB->SaveArenaData(pRecv->tPvPSystemData.dwSerialId,pRecv))
		  {
			  return GT_INVALID;
		  }
	 }
	 return E_Success;
}

DWORD CLoong::LoadArenaSystemSerialVal(DWORD pDBMsg, DWORD dwDummy)
{
     MGET_MSG(pRecv, pDBMsg, NDBC_GetSerialVal);
	 if ( P_VALID(pRecv))
	 { 
		 tagNDBS_GetSerialVal MsgSerialVal;
         if(!m_pDB->LoadArenaSystemSerialVal( pRecv->dwSerialId,&MsgSerialVal))
		 {
			 return GT_INVALID;
		 }  
		 SendMsg(&MsgSerialVal,MsgSerialVal.dwSize);
	 }
	 return E_Success;
}

DWORD CLoong::SaveShowPlayerData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveShowPlayerData);
	if ( P_VALID(pRecv))
	{ 
		if ( !P_VALID(pRecv->dwCount))
		{
			return GT_INVALID;
		}
		for( DWORD dwI = 0 ; dwI < pRecv->dwCount; dwI++)
		{
            if(!m_pDB->SaveShowPlayerData(pRecv->data[dwI].dwAthleteSignId,&pRecv->data[dwI]))
			{
				return GT_INVALID;
			}
		}
	}

     return E_Success;
}
DWORD CLoong::SaveShowPlayerPoint(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveShowPlayerPoint);
	if ( P_VALID(pRecv))
	{ 
 		if(!m_pDB->SaveShowPlayerPoint(pRecv->showPlayer.dwAthleteSignId,pRecv))
		{
			return GT_INVALID;
		}
	}
     return E_Success;
}

DWORD CLoong::SaveVotePlayerPoint(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveVotePlayerPoint);
	if ( P_VALID(pRecv))
	{ 
		if(!m_pDB->SaveVotePlayerPoint(pRecv->VotePlayer.dwVoteRoleId,pRecv))
		{
			return GT_INVALID;
		}
	}
     return E_Success;
}

DWORD CLoong::SaveShowPlayerDataCount(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveShowPlayerDataCount);
	if ( P_VALID(pRecv))
	{ 
		if(!m_pDB->SaveShowPlayerDataCount(0,pRecv))
		{
			return GT_INVALID;
		}
	}
	return E_Success;
}     

DWORD CLoong::SaveVotePlayerDataCount(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveVotePlayerDataCount);
	if ( P_VALID(pRecv))
	{ 
		if(!m_pDB->SaveVotePlayerDataCount(0,pRecv))
		{
			return GT_INVALID;
		}
	}
	return E_Success;
}




DWORD CLoong::LoadShowPlayerData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetShowPlayerData);
	if ( P_VALID(pRecv))
	{ 
		
		DWORD dwDataCount = GetShowPlayerDataCount();
		if ( !GT_VALID(dwDataCount)  )
		{
			tagNDBS_LoadShowPlayerData msg;
			msg.dwCount = 0;
			SendMsg(&msg,msg.dwSize);
			return 0;
		}

		if ( dwDataCount == 0)
		{ 
            tagNDBS_LoadShowPlayerData msg;
			msg.dwCount = 0;
			SendMsg(&msg,msg.dwSize);
			return 0;
		}
		INT32 nMsgSz  = sizeof(tagNDBS_LoadShowPlayerData) + (dwDataCount - 1)*sizeof(tagShowPlayer);
        MCREATE_MSG(pSend, nMsgSz, NDBS_LoadShowPlayerData);
		pSend->dwCount = dwDataCount ;
		for( DWORD dwI = 0 ; dwI < dwDataCount ; dwI++)
		{
     		if(!m_pDB->LoadShowPlayerData( dwI+1,&pSend->data[dwI]))
			{
				continue;
			}
		}
		SendMsg(pSend,pSend->dwSize);
		MDEL_MSG(pSend);
	}
     return E_Success;
}

DWORD CLoong::LoadVotePlayerData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_GetVotePlayerData);
	if ( P_VALID(pRecv))
	{ 
		DWORD dwDataCount = GetVotePlayerDataCount();
		if ( !GT_VALID(dwDataCount)  )
		{
			tagNDBS_LoadVotePlayerData msg;
			msg.dwCount = 0;
			SendMsg(&msg,msg.dwSize);
			return 0;
		}

		if ( dwDataCount == 0)
		{ 
			tagNDBS_LoadVotePlayerData msg;
			msg.dwCount = 0;
			SendMsg(&msg,msg.dwSize);
			return 0;
		}
		INT32 nMsgSz  = sizeof(tagNDBS_LoadVotePlayerData) + (dwDataCount - 1)*sizeof(tagVotePlayer);
		MCREATE_MSG(pSend, nMsgSz, NDBS_LoadVotePlayerData);
		pSend->dwCount = dwDataCount ;
		for( DWORD dwI = 0 ; dwI < dwDataCount ; dwI++)
		{
			if(!m_pDB->LoadVotePlayerData( dwI+1,&pSend->data[dwI]))
			{
				continue;
			}
		}
		SendMsg(pSend,pSend->dwSize);
		MDEL_MSG(pSend);
	}
     return E_Success;
}

DWORD CLoong::GetShowPlayerDataCount()
{
	DWORD dwCount = 0;

	if(!m_pDB->GetShowPlayerDataCount( 0,&dwCount))
	{
		return GT_INVALID;
	} 
	return dwCount;
}

DWORD CLoong::GetVotePlayerDataCount()
{
	DWORD dwCount = 0;

	if(!m_pDB->GetVotePlayerDataCount( 0,&dwCount))
	{
		return GT_INVALID;
	} 
	return dwCount;
}

DWORD CLoong::LoadActivetyRankData(DWORD pDBMsg, DWORD dwDummy)
{
	INT nActivityRankNum = m_pDB->GetActivityRankCount();

	if(0 == nActivityRankNum)
	{
		tagNDBS_LoadActiveRankData Load;
		Load.dwCount = 0;
		SendMsg( &Load, Load.dwSize);

		return E_Success;
	}

	tagActiveRankData *pActivityRankData = new tagActiveRankData[nActivityRankNum];
	tagActiveRankData *pCur = pActivityRankData;

	INT nNum;
	if(!m_pDB->LoadActivetyRankData(pActivityRankData, nNum))
	{
		delete []pActivityRankData;
		return GT_INVALID;
	}
    
	DWORD dwSize = sizeof(tagNDBS_LoadActiveRankData) + sizeof(tagActiveRankData)*( nActivityRankNum - 1);
	MCREATE_MSG(pSend,dwSize, NDBS_LoadActiveRankData);
    pSend->dwCount = nActivityRankNum ;
	memcpy(pSend->data, pCur, sizeof(tagActiveRankData)*nActivityRankNum);
	SendMsg(pSend,pSend->dwSize);
	MDEL_MSG(pSend);
	delete [] pCur;
	
   return  0;
}

DWORD CLoong::SaveActivityRankData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveActiveRankData);

	if(!m_pDB->SaveActivityRankData(pRecv->ActiveRankData.dwRoleId, &pRecv->ActiveRankData))
	{
		return GT_INVALID;
	}

   return 0;
}

// Jason 2010-1-27 v1.3.2 ���߽���/��һع�
DWORD CLoong::LoadAccountInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadAccountInfo);

	tagNDBS_LoadAccountInfo msg;
	msg.dwErrorCode							= 0;
	msg.dwAccountID							= pRecv->dwAccountID;
	msg.timeLastLogin						= 0;
	msg.timeLastLogout						= 0;
	msg.timeLastReceiveDailyOfflineReward	= 0;
	msg.timeLastReceiveRegression			= 0;

	if ( 0 != m_pDB->GetAccountInfo(pRecv->dwAccountID, msg.timeLastLogin,msg.timeLastLogout,msg.timeLastReceiveDailyOfflineReward,msg.timeLastReceiveRegression) )
	{
		msg.dwErrorCode = 1;
	}
	SendMsg(&msg, msg.dwSize);

	return E_Success;
}

DWORD CLoong::SaceAccountInfo(DWORD pDBMsg,DWORD )
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveAccountInfo);

	m_pDB->SaveAccountInfo(pRecv->dwAccountID, pRecv->timeLastLogin,pRecv->timeLastLogout,pRecv->timeLastReceiveDailyOfflineReward,pRecv->timeLastReceiveRegression);

	return E_Success;
}

// ��Ʒ������ʱ����������״̬������ʱ�������ʱ��
DWORD CLoong::SaveItemLockInfo(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LockItem);

	m_pDB->SaveItemLockInfo(pRecv->n64Item, pRecv->byType, pRecv->dwUnlockTime);
	
	return E_Success;
}

// ������Ʒ�İ�״̬
DWORD CLoong::SaveItemBindInfo(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_BindItem);

	m_pDB->SaveItemBindInfo(pRecv->n64Item, pRecv->byType);

	return E_Success;
}
// ��ɫ����
DWORD CLoong::SaveRoleSpeak(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RoleSpeak);

	m_pDB->SaveRoleSpeak(pRecv->dwRoleID, pRecv->bAction);

	return E_Success;
}

// ������������
DWORD CLoong::SaveShortCut_QuickBar(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveShortCut_QuickBar);

	if(!m_pDB->SaveShortCut_QuickBar(pRecv->dwRoleID,(LPVOID)pDBMsg))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//��ȡ���������
DWORD CLoong::LoadShortCut_QuickBar(DWORD pDBMsg, DWORD dwDummy)
{

	MGET_MSG(pRecv, pDBMsg, NDBC_LoadShortCut_QuickBar);

	tagNDBS_LoadShortCut_QuickBar send;

	if(!m_pDB->LoadShortCut_QuickBar(pRecv->dwRoleID, &send))
	{
		tagNDBS_IsSavedShortCut_QuickBar msgSend;
		msgSend.dwRoleID = pRecv->dwRoleID ;
		msgSend.nIsSaved = Enum_NotSaved ;
        SendMsg(&msgSend, msgSend.dwSize); 
		return GT_INVALID;
	}
     
	// ������Ϸ���������
	SendMsg(&send, send.dwSize);
	
		return E_Success;
}

// �����Ѿ�����������״̬
DWORD CLoong::SaveOpenTrainState(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveOpenTrainState);

	m_pDB->SaveOpenTrainState(pRecv->dwRoleID, pRecv->dwTrainID, (DWORD)pRecv->eTrainState, pRecv->dwCompleteness);

	return E_Success;
}

DWORD CLoong::CheckName(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CheckName);

	DWORD dwErr = m_pDB->CheckName(pRecv->szNewName);

	tagNDBS_CheckName send;
	send.dwErrorCode = dwErr;
	send.dwRoleID = pRecv->dwRoleID;
	send.n64ItemSerial = pRecv->n64ItemSerial;
	_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
	_tcscpy_s(send.szOldName, X_SHORT_NAME, pRecv->szOldName);

	SendMsg(&send, send.dwSize);

	return dwErr;
}

DWORD CLoong::SaveAvatarFaceAndHair(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveAvatarFaceAndHair);
	
	m_pDB->SaveAvatarFaceAndHair(pRecv->dwRoleID,
		pRecv->wHairMdlID,
		pRecv->wHairTexID,
		pRecv->wFaceMdlID,
		pRecv->wFaceDetailTexID);
	return E_Success;
}

// ����ս��
DWORD CLoong::ClearExpireGuildWarDeclare(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearExpireGuildWarDeclare);

	if (P_VALID(pRecv))
	{
		m_pDB->ClearExpireGuildWarDeclare();
	}
	return E_Success;
}

DWORD CLoong::InsertGuildWarDeclare(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertGuildWarDeclare);
	
	if (P_VALID(pRecv))
	{
		m_pDB->InsertGuildWarDeclare(pRecv->dwAttackGuildID,			// ��ս����ID
									 pRecv->dwDefenceGuildID,			// ����ս����ID
									 pRecv->dwWarDeclareTime,			// ��սʱ��
									 pRecv->bWarEnded);
	}
	return E_Success;
}

DWORD CLoong::SetGuildWarDeclareEnded(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SetGuildWarDeclareEnded);
	
	if (P_VALID(pRecv))
	{
		m_pDB->SetGuildWarDeclareEnded(pRecv->dwAttackGuildID,			// ��ս����ID
									 pRecv->dwDefenceGuildID,			// ����ս����ID
									 pRecv->dwWarDeclareTime);			// ��սʱ��
	}
	return E_Success;
}

DWORD CLoong::LoadGuildWarDeclare(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadGuildWarDeclare);

	if (!m_pDB->LoadGuildWarDeclare((VOID*)this))
	{
		return GT_INVALID;
	}

	return GT_INVALID;
}

// ս��
DWORD CLoong::LoadBattleFieldAttackSignUpInfo(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadBattleFieldAttackSignUpInfo);

	if (!m_pDB->LoadBattleFieldAttackSignUpInfo((VOID*)this))
	{
		return GT_INVALID;
	}

	return GT_INVALID;
}

DWORD CLoong::LoadAllicanceGuildInfo(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadAllicanceGuildInfo);

	if (!m_pDB->LoadAllicanceGuildInfo((VOID*)this))
	{
		return GT_INVALID;
	}

	return GT_INVALID;
}
DWORD CLoong::InsertIntoAttackSignUpList(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertIntoAttackSignUpList);

	if (P_VALID(pRecv))
		m_pDB->InsertIntoAttackSignUpList(pRecv->eGodID, pRecv->dwGuildID, pRecv->dwBidFund);

	return E_Success;
}
DWORD CLoong::DeleteFromAttackSignUpList(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteFromAttackSignUpList);

	if (P_VALID(pRecv))
		m_pDB->DeleteFromAttackSignUpList(pRecv->eGodID, pRecv->dwGuildID);

	return E_Success;
}
DWORD CLoong::ClearAttackSignUpList(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearAttackSignUpList);

	if (P_VALID(pRecv))
		m_pDB->ClearAttackSignUpList(pRecv->eGodID);

	return E_Success;
}
DWORD CLoong::ModifyBidFund(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ModifyBidFund);

	if (P_VALID(pRecv))
		m_pDB->ModifyBidFund(pRecv->eGodID, pRecv->dwGuildID, pRecv->dwBidFund);

	return E_Success;
}
DWORD CLoong::InsertIntoAllianceList(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertIntoAllianceList);

	if (P_VALID(pRecv))
		m_pDB->InsertIntoAllianceList(pRecv->eGodID, pRecv->dwGuildID, pRecv->eAllianceType, pRecv->bConfirmed);

	return E_Success;
}
DWORD CLoong::ClearAllianceList(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearAllianceList);

	if (P_VALID(pRecv))
		m_pDB->ClearAllianceList(pRecv->eGodID);

	return E_Success;
}
DWORD CLoong::ConfirmAlliance(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ConfirmAlliance);

	if (P_VALID(pRecv))
		m_pDB->ConfirmAlliance(pRecv->eGodID, pRecv->dwGuildID);

	return E_Success;
}
DWORD CLoong::WorldMapOnlineInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_WorldMapOlInfo);

	if (P_VALID(pRecv))
		return m_pDB->SaveOlInfo(pRecv->nNum, pRecv->data);

	return E_Success;
}

DWORD CLoong::SaveOnlineTimeEachMonth(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveOnlineTimeEachMonth);

	if (P_VALID(pRecv))
		return m_pDB->SaveOnlineTimeEachMonth(pRecv->dwAccountID, pRecv->dwRoleID, pRecv->byYear, pRecv->n16OnlineTime);

	return E_Success;
}

DWORD CLoong::QuestBoardSaveAllQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_QuestBoardSaveAllQuest);

	if (P_VALID(pRecv))
		return m_pDB->QuestBoardSaveAllQuest((LPVOID)pRecv->pData, pRecv->n16QuestCnt);

	return E_Success;
}

DWORD CLoong::QuestBoardSaveOneQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_QuestBoardSaveOneQuest);

	if (P_VALID(pRecv))
		return m_pDB->QuestBoardSaveOneQuest((LPVOID)(&pRecv->sQuestSaveData));

	return E_Success;
}

DWORD CLoong::SyncCanRankFlag(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SyncCanRankFlag);

	if (P_VALID(pRecv))
		return m_pDB->SyncCanRankFlag(pRecv->dwRoleID, pRecv->byCanRankFlag);

	return E_Success;
}

DWORD CLoong::SaveMediaOnlineNum(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveMediaOnlineNum);

	if (P_VALID(pRecv))
		return m_pDB->SaveMediaOnlineNum(pRecv->dwNum, pRecv->pData);

	return E_Success;
}

DWORD CLoong::UpdateFabaoMood(DWORD pDBMsg,DWORD dwDummy)
{
	if (P_VALID(pDBMsg))
		return m_pDB->UpdateFabaoMood(pDBMsg, dwDummy);

	return E_Success;
}
DWORD	CLoong::UpdateFabaoEleInjuryType(DWORD pDBMsg,DWORD dwDummy)
{
	if (P_VALID(pDBMsg))
		return m_pDB->UpdateFabaoEleInjuryType(pDBMsg, dwDummy);

	return E_Success;
}
// ����������������
DWORD CLoong::UpdateFabaoNirVanaData(DWORD pMsg,DWORD dwDummy)
{
	if (P_VALID(pMsg))
		return m_pDB->UpdateFabaoNirVanaData(pMsg, dwDummy);

	return E_Success;
}
// ��ȡ������������
DWORD CLoong::GetFabaoNirVanaData(DWORD pDBMsg,DWORD dwDummy)
{
	tagNDBS_GetFabaoNirVanaData msg;
	if (P_VALID(pDBMsg))
	{
		m_pDB->GetFabaoNirVanaData(pDBMsg, dwDummy ,&msg);
		SendMsg(&msg, msg.dwSize);
	}

	return E_Success;
}

// ��֮����
DWORD CLoong::LoadGodBidInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadGodBidInfo);

	if (!m_pDB->LoadGodBidInfo((VOID*)this))
	{
		return GT_INVALID;
	}

	return GT_INVALID;
}
DWORD CLoong::SaveOneGodBidInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveOneGodBidInfo);

	if (P_VALID(pRecv))
		return m_pDB->SaveOneGodBidInfo(pRecv->GodBidInfo2DB.dwRoleID, pRecv->GodBidInfo2DB.n64Bidden, pRecv->GodBidInfo2DB.byBidType, pRecv->GodBidInfo2DB.dwBidID);

	return E_Success;
}
DWORD CLoong::ClearGodBidInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearGodBidInfo);

	BOOL bSuccess = TRUE;
	if (P_VALID(pRecv))
	{
		bSuccess = m_pDB->ClearGodBidInfo();
		if (pRecv->bBidEnd)
		{
			tagNDBS_ClearGodBidInfo send;
			send.bClearSuccess = bSuccess;
			SendMsg(&send, send.dwSize);
		}
	}

	return bSuccess;
}
DWORD CLoong::UpdateFabaoBirthTimes(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateFabaoGivenBirthTimes);

	if (P_VALID(pRecv))
		return m_pDB->UpdateFabaoGivenBirthTimes(pRecv->_n64ID,pRecv->nTimes);

	return E_Success;
}

DWORD CLoong::SaveEggInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveEggInfo);

	if (P_VALID(pRecv))
		return m_pDB->SaveEggInfo(pRecv->dwRoleID,pRecv->nCurLayer, pRecv->n16ColourEggPos1, pRecv->n16ColourEggPos2, pRecv->sBrokenEggInfo);

	return E_Success;
}

DWORD CLoong::ClearEggInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ClearEggInfo);

	if (P_VALID(pRecv))
		return m_pDB->ClearEggInfo(pRecv->dwRoleID);

	return E_Success;
}

DWORD CLoong::SaveWardrobeLayerInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveWardrobeLayerInfo);

	if (P_VALID(pRecv))
		return m_pDB->SaveWardrobeLayerInfo(pRecv->WardrobeLayerInfo);

	return E_Success;
}

//��������ϡ������
DWORD CLoong::LoadAllRareArea(DWORD pDBMsg,DWORD dwDummy)
{
	// Ԥ�ȷ���һ���㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[MAX_RAREAREA_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_RAREAREA_BUFF_SIZE);
	
	INT32 nRareAreaNum = 0;
	if(m_pDB->LoadAllRareArea(pBuffLoad, nRareAreaNum))
	{
		MTRANS_POINTER(pGlobalLimitedItem, pBuffLoad, tagGlobalLimitedItem);
		tagNDBS_LoadGlobalLimitedItem send;
		send.nNum = 1;
		for(INT32 i=0; i<nRareAreaNum; ++i)
		{
			memcpy(&send.data, pGlobalLimitedItem + i, sizeof(tagGlobalLimitedItem));

			SendMsg( &send, send.dwSize);
		}
	}

	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}

DWORD CLoong::SaveRareItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveGlobalLimitedItem);

	if(P_VALID(pRecv))
		m_pDB->SaveRareItem(&(pRecv->data));

	return E_Success;
}

DWORD CLoong::UnSaveRareItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UnSaveGlobalLimitedItem);

	if(P_VALID(pRecv))
		m_pDB->UnSaveRareItem(pRecv->dwItemID, pRecv->n64Serial);

	return E_Success;
}

//-------------------------------------------------------------------
// ����ϡ������
//DWORD CLoong::CreateRareArea(DWORD pDBMsg, DWORD dwDummy)
//{
//	MGET_MSG(p, pDBMsg, NDBC_CreateGlobalLimitedItem);
//
//	if(!m_pDB->InsertRareArea(&p->item))
//	{
//		return GT_INVALID;
//	}
//
//	return E_Success;
//}
//
////-------------------------------------------------------------------
//// ɾ��ϡ������
//DWORD CLoong::DeleteRareArea(DWORD pDBMsg, DWORD dwDummy)
//{
//	MGET_MSG(p, pDBMsg, NDBC_DeleteGlobalLimitedItem);
//
//	if(!m_pDB->DeleteRareArea(&p->item))
//	{
//		return GT_INVALID;
//	}
//
//	return E_Success;
//}
//-------------------------------------------------------------------
// ����ϡ������
DWORD CLoong::UpdateRareArea(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateGlobalLimitedItem);

	if(!m_pDB->UpdateRareArea(&p->item,p->nOriginal64ID))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DeleteRoleArea(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DeleteRoleArea);

	if(!m_pDB->DelArea(p->dwRoleID,p->dwAreaID))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//-------------------------------------------------------------------
DWORD CLoong::CreateRoleArea(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateRoleArea);

	if(!m_pDB->InsertArea(p->area.dwRoleID,&p->area))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DelRoleOneBuff(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DelRoleOneBuff);

	if(!m_pDB->DelOneBuff(p->dwRoleID,p->dwBuffID))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::LoadFamilySprite(DWORD pDBMsg, DWORD dwDummy)
{
	// Ԥ�ȷ���һ���㹻��Ŀռ�
	CHAR *pBuffLoad = new CHAR[MAX_FAMILYSPRITE_BUFF_SIZE];
	CHAR *pBuffLoad1 = new CHAR[MAX_FAMILYSPRITERE_BUFF_SIZE];
	CHAR *pBuffLoad2 = new CHAR[MAX_FAMILYROLESPRITE_BUFF_SIZE];

	if( !P_VALID(pBuffLoad) || !P_VALID(pBuffLoad1) || !P_VALID(pBuffLoad2) ) 
	{
		SAFE_DEL_ARRAY(pBuffLoad);
		SAFE_DEL_ARRAY(pBuffLoad1);
		SAFE_DEL_ARRAY(pBuffLoad2);
		return GT_INVALID;
	}

	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_FAMILYSPRITE_BUFF_SIZE);
	
	INT32 nFamilySpriteNum = 0;
	if(m_pDB->LoadFamilySprite(pBuffLoad, nFamilySpriteNum))
	{
		MTRANS_POINTER(pFamilySpriteSave, pBuffLoad, tagFamilySpriteSave);

		for( INT32 i = 0; i < nFamilySpriteNum; ++i )
		{
			INT nMsgSz = 0;
			INT nMsgSz1 = 0;
			INT nMsgSz2 = 0;
			INT32 nFamilySpriteRequirementNum = 0;
			INT32 nSpriteNum = 0;
			
			//��ȡ�ü�����������������
			ZeroMemory(pBuffLoad1, sizeof(CHAR) * MAX_FAMILYSPRITERE_BUFF_SIZE);
			if(m_pDB->LoadFamilySpriteRequirement( pBuffLoad1, nFamilySpriteRequirementNum, (pFamilySpriteSave+i)->dwFamilyID))
			{
				if( nFamilySpriteRequirementNum > 0)
					nMsgSz1 = sizeof(tagFamilySpriteRequirementSave) * (nFamilySpriteRequirementNum-1);
			}

			//��ȡ�ü�����ҵǼǵ���������Ϣ
			ZeroMemory(pBuffLoad2, sizeof(CHAR) * MAX_FAMILYROLESPRITE_BUFF_SIZE);
			if(m_pDB->LoadFamilyRoleSprite( pBuffLoad2, nSpriteNum,(pFamilySpriteSave+i)->dwFamilyID))
			{
				if( nSpriteNum > 0)
					nMsgSz2 = sizeof(tagRoleSpriteDataSave) * (nSpriteNum-1);
			}

			//�ܵİ���С
			nMsgSz = sizeof(tagNDBS_LoadFamilySprite) + nMsgSz1 + nMsgSz2;
			MCREATE_MSG(pSend, nMsgSz, NDBS_LoadFamilySprite);
			pSend->nNumRequirement = nFamilySpriteRequirementNum;
			pSend->nNumSprite = nSpriteNum;

			//����������Ϣ
			MTRANS_POINTER(pFamilySpriteRequirementSave, pBuffLoad1, tagFamilySpriteRequirementSave);
			memcpy(pSend->fsr, 
				pFamilySpriteRequirementSave, 
				sizeof(tagFamilySpriteRequirementSave) * nFamilySpriteRequirementNum);

			//����Ǽ�������Ϣ
			MTRANS_POINTER(pRoleSpriteDataSave, pBuffLoad2, tagRoleSpriteDataSave);
			if( nFamilySpriteRequirementNum > 0 )
			{
				memcpy((char*)pSend + sizeof(tagNDBS_LoadFamilySprite) - sizeof(tagRoleSpriteDataSave) + sizeof(tagFamilySpriteRequirementSave) * (nFamilySpriteRequirementNum - 1), 
					pRoleSpriteDataSave, 
					sizeof(tagRoleSpriteDataSave) * nSpriteNum);
			}
			else
			{
				memcpy((char*)pSend + sizeof(tagNDBS_LoadFamilySprite) - sizeof(tagRoleSpriteDataSave), 
					pRoleSpriteDataSave, 
					sizeof(tagRoleSpriteDataSave) * nSpriteNum);
			}

			memcpy( &pSend->fs, pFamilySpriteSave+i, sizeof(tagFamilySpriteSave) );

			SendMsg( pSend, pSend->dwSize);

			MDEL_MSG(pSend);
		}
	}

	SAFE_DEL_ARRAY(pBuffLoad);
	SAFE_DEL_ARRAY(pBuffLoad1);
	SAFE_DEL_ARRAY(pBuffLoad2);

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::CreateFamilySprite(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateFamilySprite);

	if(!m_pDB->InsertFamilySprite(&p->fs))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::UpdateFamilySprite(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateFamilySprite);

	if(!m_pDB->UpdateFamilySprite(&p->fs))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DeleteFamilySprite(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DeleteFamilySprite);

	if(!m_pDB->DeleteFamilySprite(p->dwFamilyID))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
//DWORD CLoong::LoadFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy)
//{
//	// Ԥ�ȷ���һ���㹻��Ŀռ�
//	CHAR *pBuffLoad = new CHAR[MAX_FAMILYSPRITERE_BUFF_SIZE];
//	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_FAMILYSPRITERE_BUFF_SIZE);
//	
//	INT32 nFamilySpriteRequirementNum = 0;
//	if(m_pDB->LoadFamilySpriteRequirement(pBuffLoad, nFamilySpriteRequirementNum))
//	{
//		MTRANS_POINTER(pFamilySpriteRequirementSave, pBuffLoad, tagFamilySpriteRequirementSave);
//		tagNDBS_LoadFamilySpriteRequirement send;
//
//		for( INT32 i = 0; i < nFamilySpriteRequirementNum; ++i )
//		{
//			memcpy(&send.fsr, pFamilySpriteRequirementSave + i, sizeof(tagFamilySpriteRequirementSave));
//
//			SendMsg( &send, send.dwSize);
//		}
//	}
//
//	SAFE_DEL_ARRAY(pBuffLoad);
//
//	return E_Success;
//}
//-------------------------------------------------------------------
DWORD CLoong::CreateFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateFamilySpriteRequirement);

	if(!m_pDB->InsertFamilySpriteRequirement(p->dwFamilyID, &p->fsr, p->nNum))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::UpdateFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateFamilySpriteRequirement);

	if(!m_pDB->UpdateFamilySpriteRequirement(p->dwFamilyID, &p->fsr))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DeleteFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DeleteFamilySpriteRequirement);

	if(!m_pDB->DeleteFamilySpriteRequirement(p->dwFamilyID))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DeleteALLFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DeleteALLFamilySpriteRequirement);

	if(!m_pDB->DeleteALLFamilySpriteRequirement())
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::SaveAllFamilySprite(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_SaveAllFamilySprite);

	for(int i = 0; i < p->nNum; ++i)
	{
		if(!m_pDB->UpdateFamilySprite(&p->fs[i]))
		{
			return GT_INVALID;
		}
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::InsertBuffDir(DWORD pDBMsg,DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertBuff2DB);

	return m_pDB->InsertBuffDirectly(pRecv->dwRoleID,&pRecv->BuffSave);
}

//-------------------------------------------------------------------
DWORD CLoong::RenewFamilyQuest(DWORD pDBMsg, DWORD dwDummy)
{
	if (!P_VALID(pDBMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pDBMsg, NDBC_RenewFamilyQuest);

	 if(FALSE == m_pDB->RenewFamilyQuest(pRecv->dwFamilyID, pRecv->nProcess, pRecv->byNum, pRecv->Quests) 
		 || FALSE == m_pDB->ClearFamilyMemRewarded(pRecv->dwFamilyID))
	 {
		 IMSG(_T("\r\n Error occur when refresh family(id:%d) quest data.\n"), pRecv->dwFamilyID);
		 return GT_INVALID;
	 }

	 return E_Success;
}

DWORD CLoong::SaveFamilyQuest(DWORD pDBMsg, DWORD dwDummy)
{
	if (!P_VALID(pDBMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pDBMsg, NDBC_SaveFamilyQuestDetail);

	if (!P_VALID(pRecv->dwFamilyID) || !P_VALID(pRecv->Quests) 
		|| FALSE == m_pDB->SaveFamilyQuest(pRecv->dwFamilyID, pRecv->Quests))
	{
		 IMSG(_T("\r\n Error occur when save family(id:%d) quest data.\n"), pRecv->dwFamilyID);
		return GT_INVALID;
	}

	return E_Success;
}

DWORD CLoong::ChangeFamilyQuestIssueState(DWORD pDBMsg, DWORD dwDummy)
{
	if (!P_VALID(pDBMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pDBMsg, NDBC_ChangFamilyQuestIssueState);

	return m_pDB->ChangeFamilyQuestIssueState(pRecv->dwFamilyID, pRecv->bIssueFlag);
}

DWORD CLoong::LoadFamilyQuest(DWORD pDBMsg, DWORD dwDummy)
{
	INT nNum = 0;
	BYTE* pData = new BYTE[1024* 1024*X_SHORT_STRING];
	if (!P_VALID(pData))
	{
		return GT_INVALID;
	}

	ZeroMemory(pData, sizeof(BYTE)*1024*1024*X_SHORT_STRING);

	if(TRUE != m_pDB->LoadFamilyQuest(nNum, pData))
	{
		tagNDBS_LoadFamilyQuest send;
		send.dwErrorCode = GT_INVALID;
		SendMsg(&send, send.dwSize);
		SAFE_DEL_ARRAY(pData);
		return GT_INVALID;
	}

	if (nNum <= 0)
	{
		tagNDBS_LoadFamilyQuest send;
		send.dwErrorCode = E_Success;
		send.nNum = 0;
		SendMsg(&send, send.dwSize);
		SAFE_DEL_ARRAY(pData);
		return E_Success;
	}

	if (nNum*sizeof(tagFamilyQuestLoadData) > 1024*1024*X_SHORT_STRING)
	{
		IMSG(_T("\r\n Memory space for loading family quest data is not enough, parts of data lost.\n"));
		SAFE_DEL_ARRAY(pData);
		return GT_INVALID;
	}

	tagFamilyQuestLoadData* p = (tagFamilyQuestLoadData*)pData;
	INT nCount = nNum/FAMILY_QUEST_MAX_LOAD_ITEM_ONCE + 1; // ��Ҫ���Ͷ��ٴ�
	INT nLastNum = nNum%FAMILY_QUEST_MAX_LOAD_ITEM_ONCE; // ���һ�η��ͼ�������
	bool bLastTime = false;
	for (int i=0; i<nCount; ++i)
	{
		INT nSize = 0; // ��Ϣ��С
		INT nTmpCount = 0; // �����漰�ļ������ݵ�����
		if (i<nCount-1)
		{
			nTmpCount = FAMILY_QUEST_MAX_LOAD_ITEM_ONCE;
			nSize = (FAMILY_QUEST_MAX_LOAD_ITEM_ONCE-1)*sizeof(tagFamilyQuestLoadData) + sizeof(tagNDBS_LoadFamilyQuest);
		}
		else
		{
			// ���һ��
			bLastTime = true;
			if(nLastNum<=0)
				break;
			nTmpCount = nLastNum;
			nSize = (nLastNum-1)*sizeof(tagFamilyQuestLoadData) + sizeof(tagNDBS_LoadFamilyQuest);
		}
		
		MCREATE_MSG(pSend, nSize, NDBS_LoadFamilyQuest);
		pSend->dwErrorCode = E_Success;
		for (int i=0; i< nTmpCount; ++i)
		{
			if (!P_VALID(p))
			{
				nTmpCount = i;
				break;
			}
			memcpy(&(pSend->QuestData[i]), p, sizeof(tagFamilyQuestLoadData));
			++p;
		}
		pSend->nNum = nTmpCount;
		pSend->bLastOne = bLastTime;
		SendMsg(pSend, pSend->dwSize);
	}

	SAFE_DEL_ARRAY(pData);
	return E_Success;
}
// DWORD CLoong::FamilyMemberRewarded(DWORD pDBMsg, DWORD dwDummy)
// {
// 	if (!P_VALID(pDBMsg))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	MGET_MSG(pRecv, pDBMsg, NDBC_MemberRewarded);
// 
// 	return m_pDB->FamilyMemberRewarded(pRecv->dwFamilyID, pRecv->dwRoleID);
// }
//-------------------------------------------------------------------
DWORD CLoong::CreateRoleSpriteData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_CreateRoleSpriteData);

	if( !m_pDB->InsertRoleSpriteData( p->dwFamilyID, &p->rsd ) )
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::UpdateRoleSpriteData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_UpdateRoleSpriteData);

	if( !m_pDB->UpdateRoleSpriteData( p->dwFamilyID, &p->rsd ) )
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DeleteRoleSpriteData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_DeleteRoleSpriteData);

	if(!m_pDB->DeleteRoleSpriteData( p->dwRoleID ))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::DelEquipdelByTime(CHAR *pszTime)
{
	if(!m_pDB->DelEquipDel( pszTime ))
	{
		return GT_INVALID;
	}

	return E_Success; 
}
//-------------------------------------------------------------------
DWORD CLoong::DelItemdelByTime(CHAR *pszTime)
{
	if(!m_pDB->DelItemDel( pszTime ))
	{
		return GT_INVALID;
	}

	return E_Success;
}
//-------------------------------------------------------------------
DWORD CLoong::LoadWorldData(DWORD pDBMsg, DWORD dwDummy)
{
	tagNDBS_RequestLogWorldData send;
	INT32 nNum = 0;
	if( !m_pDB->LoadWorldData((DWORD*)(send.dwData), nNum))
	{
		if (0==nNum)
		{
			m_pDB->InsertWorldData();
		}
		return GT_INVALID;
	}

	SendMsg(&send, send.dwSize);
	return E_Success;	
}

DWORD CLoong::LoadRolelogData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RequestLogRoleData);

	tagNDBS_RequestLogRoleData send;
	send.dwRoleID = pRecv->dwRoleID;
	if( !m_pDB->LoadRoleLogData((DWORD*)(send.dwData),  pRecv->dwRoleID))
	{
		return GT_INVALID;
	}

	SendMsg(&send, send.dwSize);
	return E_Success;	
}

DWORD CLoong::SaveAccountSpec(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveAccountSpec);
	return m_pDB->SaveAccountSpec(pRecv->dwAccountID, pRecv->eSpec);
}

DWORD CLoong::LoadTigerProfit(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_TigerProfit);

	tagNDBS_TigerProfit send;
	if( !m_pDB->LoadRoleTigerData(send.n64Profit))
	{
		return GT_INVALID;
	}

	SendMsg(&send, send.dwSize);
	return E_Success;	
}

DWORD CLoong::SaveTigerProfit(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveTigerProfit);
	return m_pDB->SaveTigerProfit(pRecv->n64Profit);	
}

DWORD CLoong::AccountReactiveOrNot(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_AccountReactiveCode);

	tagNDBS_AccountReactiveCode send;
	send.dwAccountID = pRecv->dwAccountID;	
	if(TRUE == m_pDB->AccountReactiveOrNot(pRecv->dwAccountID, pRecv->dwKeyCode,  pRecv->byLevel, pRecv->dwExp,
		send.dwCAccountID, send.dwErrorCode, send.nScore))
		SendMsg(&send, send.dwSize);

	return 0;
}

DWORD CLoong::UpdateRewardReactiveLevel(DWORD pDBMsg, DWORD)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateRewardReactiveLevel);

	return m_pDB->UpdateRewardReactiveLevel(pRecv->dwAccountID, pRecv->byLevel);
}

DWORD CLoong::OnNDBC_KeyCodeAddGift(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_KeyCodeAddGift);

	BOOL bRet = m_pDB->InsertUseKeyCodeRoleID(pRecv->dwRoleID);
	tagNDBS_KeyCodeAddGift send;
	send.dwRoleID = pRecv->dwRoleID;
	send.bIsSuccessful = bRet;
	SendMsg(&send, send.dwSize);

	return 0;
}

DWORD CLoong::OnNDBC_ReceiveYuanBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_ReceiveYuanBao);
	int nNum = 0;
	m_pDB->GetYuanBaoDaiBiNum(nNum, pRecv->dwRoleID);
	tagNDBS_ReceiveYuanBao send;
	send.dwRoleID = pRecv->dwRoleID;
	send.nNum = nNum;
	SendMsg(&send, send.dwSize);

	return 0;
}

//---------------------Ԫ�����---------------------------------------------------
DWORD CLoong::InsertHolySoulInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_InsertHolySoulInfo);

	if (P_VALID(pRecv))
		return m_pDB->InsertHolySoulInfo(&pRecv->SoulAtt,pRecv->dwRoleID);

	return E_Success;
}

DWORD CLoong::UpdateHolySoulInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateHolySoulInfo);

	if (P_VALID(pRecv))
		return m_pDB->UpdateHolySoulInfo(&pRecv->SoulAtt,pRecv->dwRoleID);

	return E_Success;
}

DWORD CLoong::UpdateHolySoulExpAtt(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateHolySoulExpAtt);

	if (P_VALID(pRecv))
		return m_pDB->UpdateHolySoulExpAtt(pRecv,pRecv->dwRoleID);

	return E_Success;
}

DWORD CLoong::UpdateHolySoulAttInfo(DWORD pDBMsg,DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_UpdateHolySoulAttInfo);

	if (P_VALID(pRecv))
		return m_pDB->UpdateHolySoulAttInfo(pRecv,pRecv->dwRoleID);

	return E_Success;
}
//---------------------------------------------------------------------------------

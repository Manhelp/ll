//-----------------------------------------------------------------------------
// filename: force_breakout_apprentice_mgr.cpp
// author: 
// data: 
// last:
// brief: ǿ��������ɵ�ͽ�ܹ�����
//-----------------------------------------------------------------------------
#include "Stdafx.h"
#include "force_breakout_apprentice_mgr.h"
#include "db_session.h"
#include "role_mgr.h"
#include "role.h"

// ����ֵ TRUE����ʱ����  FALSE����û��ʱ����
BOOL CForceBreakOutApprentice::OnMinute()
{
	if (CalcTimeDiff(GetCurrentDWORDTime(), m_BreakoutInsertTime) >= DAY_SECOND_COUNT)
		return TRUE;

	return FALSE;
}

// ִ�н�ɫ�˳�ʦ�Ų���
VOID CForceBreakOutApprentice::ApprenticeBreakOut()
{
	Role* pMaster = g_roleMgr.GetRolePtrByID(m_dwMasterID);
	Role* pApprentice = g_roleMgr.GetRolePtrByID(m_dwApprenticeID);
	BOOL bNotifyClassmate = FALSE;

	// ���ʦ������
	if (P_VALID(pMaster))
	{
		pMaster->DeleteApprentice(m_dwApprenticeID);

		pMaster->ModifyMasterApprenticeCount(-1);
		// ��ʦ����������ʦ�Ź�ϵ��Ϣ
		tagNS_BreakOutMasterConfirm send;
		send.dwRoleID = m_dwApprenticeID;
		pMaster->GetSession()->SendMessage(&send, send.dwSize);

		// ֪ͨʦ������ʦ���������ܷ����仯
		pMaster->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMaster->GetOnlineClassmatesCnt());

		if (bNotifyClassmate == FALSE)
		{
			// ֪ͨͽ����������ʦ��
			TMap<DWORD, DWORD>::TMapIterator iterator = pMaster->GetApprenticeData().Begin();
			DWORD dwApprenticeID = GT_INVALID;
			while (pMaster->GetApprenticeData().PeekNext(iterator, dwApprenticeID))
			{
				Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
				if (P_VALID(pApprentice))
				{
					pApprentice->DeleteClassMate(m_dwApprenticeID);

					// ֪ͨͽ������ʦ�����������仯
					pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
				}
			}

			bNotifyClassmate = TRUE;
		}
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(m_dwMasterID, -1);
		g_roleMgr.SaveRoleInfoToDB(m_dwMasterID);
	}

	// ���ͽ������
	if (P_VALID(pApprentice))
	{
		if (bNotifyClassmate == FALSE)
		{
			// ֪ͨͬ����������ʦ��
			DWORD dwMateID = GT_INVALID;
			for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
			{
				dwMateID = pApprentice->GetClassMate().dwData[i];
				Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
				if (P_VALID(pMate))
				{
					pMate->DeleteClassMate(pApprentice->GetID());
		 			pMate->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMate->GetOnlineClassmatesCnt()-1);
				}
			}

			bNotifyClassmate = TRUE;
		}
		pApprentice->SetMasterID(GT_INVALID);
		pApprentice->SetJingWuPoint(0);			// ��վ������
		pApprentice->ModifyMasterApprenticeCount(-1);		

		// ɾ������ͬ��
		pApprentice->DeleteAllClassMate();

		// ��ͽ�ܷ�������ʦ�Ź�ϵ��Ϣ
		tagNS_BreakOutApprenticeConfirm send;
		send.dwRoleID = m_dwMasterID;
		pApprentice->GetSession()->SendMessage(&send, send.dwSize);

		// ֪ͨ�����ʦ�ŵ�ͽ��ʦ���������������仯
		pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(m_dwApprenticeID, -1);
		g_roleMgr.SaveRoleInfoToDB(m_dwApprenticeID);

		// ��վ������
		tagNDBC_ModifyApprenticeJingWuPoint sendDB;
		sendDB.dwApprenticeID = m_dwApprenticeID;
		sendDB.dwJingWuPoint = 0;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}

	// ʦ��ͽ�ܶ������ߣ������ݿ��ж�ȡʦ�����ݣ�֪ͨʦ��������ͽ��
// 	if (bNotifyClassmate == FALSE)
// 	{
// 		tagNDBC_LoadMasterApprenticeData
// 	}

	// �������ݿ�
	tagNDBC_DeleteAnApprentice sendDB;
	sendDB.dwMasterID = m_dwMasterID;
	sendDB.dwApprenticeID = m_dwApprenticeID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

// �����ݴ������ݿ�
VOID CForceBreakOutApprentice::InsertApprenticeData2DB()
{
	tagNDBC_InsertForceBreakOutApprentice send;
	send.sForceBreakOutApprentice.BreakoutInsertTime = m_BreakoutInsertTime;
	send.sForceBreakOutApprentice.dwMasterID = m_dwMasterID;
	send.sForceBreakOutApprentice.dwApprenticeID = m_dwApprenticeID;

	g_dbSession.Send(&send, send.dwSize);
}

// ɾ�����ݿ��е�����
VOID CForceBreakOutApprentice::RemoveApprenticeFromDB()
{
	tagNDBC_RemoveForceBreakOutApprentice send;
	send.sForceBreakOutApprentice.BreakoutInsertTime = m_BreakoutInsertTime;
	send.sForceBreakOutApprentice.dwMasterID = m_dwMasterID;
	send.sForceBreakOutApprentice.dwApprenticeID = m_dwApprenticeID;

	g_dbSession.Send(&send, send.dwSize);
}

CForceBreakOutApprenticeMgr::~CForceBreakOutApprenticeMgr()
{
	// �ͷ�ǿ������ʦ�������б�
	DestoryApprenticeList();
}

VOID CForceBreakOutApprenticeMgr::OnMinute()
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = NULL;
	TList<CForceBreakOutApprentice*>::TListIterator iterator = m_listForceBreakOutApprentice.Begin();
	while(m_listForceBreakOutApprentice.PeekNext(iterator, pForceBreakOutApprentice))
	{
		if (P_VALID(pForceBreakOutApprentice))
		{
			// �����б��нڵ�,����Ƿ�ʱ����
			if (TRUE == pForceBreakOutApprentice->OnMinute())
			{
				// ��ʱ������ʦ���ˣ���ִ���˳�ʦ�Ų���
				pForceBreakOutApprentice->ApprenticeBreakOut();
				
				// ɾ�����ݿ��е�����
				pForceBreakOutApprentice->RemoveApprenticeFromDB();

				// ��ɾ���ڵ�
				m_listForceBreakOutApprentice.Erase(pForceBreakOutApprentice);
				SAFE_DEL(pForceBreakOutApprentice);
			}
		}
	}
}

// ���ǿ������ʦ�ŵ�ͽ�ܵ�����
VOID CForceBreakOutApprenticeMgr::InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID)
{
	InsertApprentice2List(dwApprenticeID, dwMasterID, GetCurrentDWORDTime());
}

VOID CForceBreakOutApprenticeMgr::InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID, tagDWORDTime dwordBreakoutTime, BOOL bNeedSave2DB/* = TRUE*/)
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = new CForceBreakOutApprentice(dwApprenticeID, dwMasterID, dwordBreakoutTime);
	if (P_VALID(pForceBreakOutApprentice))
	{
		m_listForceBreakOutApprentice.PushBack(pForceBreakOutApprentice);

		// �����ݴ������ݿ�
		if (bNeedSave2DB)
		{
			pForceBreakOutApprentice->InsertApprenticeData2DB();
		}
	}
}

VOID CForceBreakOutApprenticeMgr::Init()
{
	// �������ݿ��д洢��ǿ������ʦ�������б�
	LoadApprenticeListFromDB();
}

VOID CForceBreakOutApprenticeMgr::InitApprenticeList(DWORD dwForceBreakOutApprenceCnt, tagForceBreakOutApprentice *pForceBreakOutApprentice)
{
	for (DWORD dwCnt = 0; dwCnt < dwForceBreakOutApprenceCnt; dwCnt++)
	{
		InsertApprentice2List(pForceBreakOutApprentice[dwCnt].dwApprenticeID, pForceBreakOutApprentice[dwCnt].dwMasterID, pForceBreakOutApprentice[dwCnt].BreakoutInsertTime, FALSE);		
	}
}

// �������ݿ��д洢��ǿ������ʦ�������б�
VOID CForceBreakOutApprenticeMgr::LoadApprenticeListFromDB()
{
	// �����ݿⷢ����
	tagNDBC_LoadForceBreakOutApprenticeList send;
	g_dbSession.Send(&send, send.dwSize);
}

// �ͷ�ǿ������ʦ�������б�
VOID CForceBreakOutApprenticeMgr::DestoryApprenticeList()
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = NULL;
	TList<CForceBreakOutApprentice*>::TListIterator iterator = m_listForceBreakOutApprentice.Begin();
	while(m_listForceBreakOutApprentice.PeekNext(iterator, pForceBreakOutApprentice))
	{
		SAFE_DEL(pForceBreakOutApprentice);
	}
	m_listForceBreakOutApprentice.Clear();
}

BOOL CForceBreakOutApprenticeMgr::FindApprentice(DWORD dwApprenticeID, INT& dwRemainTime)
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = NULL;
	TList<CForceBreakOutApprentice*>::TListIterator iterator = m_listForceBreakOutApprentice.Begin();
	while(m_listForceBreakOutApprentice.PeekNext(iterator, pForceBreakOutApprentice))
	{
		if (pForceBreakOutApprentice->GetApprenticeID() == dwApprenticeID)
		{
			dwRemainTime = pForceBreakOutApprentice->GetRemainTime();
			return TRUE;
		}
	}
	dwRemainTime = GT_INVALID;
	return FALSE;
}

CForceBreakOutApprenticeMgr g_forceBreakOutApprenticeMgr;
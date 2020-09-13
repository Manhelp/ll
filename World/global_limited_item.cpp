#include "StdAfx.h"
#include "global_limited_item.h"
#include "Role.h"
#include "../ServerDefine/msg_item.h"

GlobalLimitedItemMgr g_GlobalLimitedItemMgr;

//-------------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------------
VOID GlobalLimitedItemMgr::Init(tagGlobalLimitedItem* pData, INT32 nNum)
{
	tagGlobalLimitedItem* pSave = pData;

	if (!P_VALID(pSave))
	{ 
		ASSERT(0);
		return;
	}

	for (int i = 0; i<nNum; ++i)
	{
		tagGlobalLimitedItem* pTmp = pSave + i;

		tagGlobalLimitedItem* pItem = new tagGlobalLimitedItem;
		if (!P_VALID(pItem))
		{
			ASSERT(0);
			return;
		}

		pItem->dwID = pTmp->dwID;
		pItem->eType = pTmp->eType;
		pItem->n64ID = pTmp->n64ID;
		pItem->dwAreaID = pTmp->dwAreaID;	
		pItem->dwRecordTime = pTmp->dwRecordTime;
		pItem->nTime = pTmp->nTime;

		pItem->nTime -= (g_world.GetWorldTime() - pTmp->dwRecordTime);
		if( pItem->nTime < 0 )
			pItem->nTime = 0;	

		if (m_mapLimitedItem.IsExist(pItem->dwID))
		{
			std::vector<tagGlobalLimitedItem*>* pTempVec = m_mapLimitedItem.Peek(pItem->dwID);

			if(P_VALID(pTempVec))
				(*pTempVec).push_back(pItem);
			else
				ASSERT(0);
		}
		else
		{
			tagItemProto* pProto = g_attRes.GetItemProto(pItem->dwID);
			if(!P_VALID(pProto)) 
			{
				SAFE_DELETE(pItem);
				return;
			}

			if(pProto->nSpecFuncVal1<=0)
			{
				SAFE_DELETE(pItem);
				return;
			}

			std::vector<tagGlobalLimitedItem*>* pTempVec = new std::vector<tagGlobalLimitedItem*>;
			if (P_VALID(pTempVec))
			{
				(*pTempVec).push_back(pItem);
				m_mapLimitedItem.Add(pItem->dwID, pTempVec);
			}
			else	
				ASSERT(0);		
		}
	}

	m_dwLastTime = timeGetTime();
	m_dwCurTime = m_dwLastTime;
}

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
VOID GlobalLimitedItemMgr::Destroy()
{

	TMap<DWORD, std::vector<tagGlobalLimitedItem*>*>::TMapIterator itr = m_mapLimitedItem.Begin();
	std::vector<tagGlobalLimitedItem*>* pTmp = NULL;
	while(m_mapLimitedItem.PeekNext(itr, pTmp))
	{
		if (!P_VALID(pTmp))
		{
			continue;
		}

		std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
		while(it != pTmp->end())
		{
			tagGlobalLimitedItem* p = *it;
			pTmp->erase(it);
			SAFE_DEL(p);	
		}
	}
}

VOID GlobalLimitedItemMgr::Update()
{
	m_dwCurTime = timeGetTime();
	if( m_dwCurTime - m_dwLastTime >= 1000 )
	{//����Ϊ��λ���
		m_dwLastTime = m_dwCurTime;

		//TMap<DWORD, std::vector<tagGlobalLimitedItem*>*>::TMapIterator itr = m_mapLimitedItem.Begin();
		std::vector<tagGlobalLimitedItem*>* pTmp = NULL;
		DWORD dwTypeID = 0;
		std::vector<tagDelLinitedItem> vecDel;

		m_mapLimitedItem.ResetIterator();
		while(m_mapLimitedItem.PeekNext(dwTypeID, pTmp))
		{
			if (!P_VALID(dwTypeID) || !P_VALID(pTmp))
			{
				m_mapLimitedItem.Erase(dwTypeID);
				continue;
			}

			std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
			while(it != pTmp->end())
			{
				tagGlobalLimitedItem* p = *it;
				if (!P_VALID(p)) 
				{
					it++;
					continue;
				}
				
				p->nTime--;

				if( p->nTime > 0 ) 
				{
					it++;
					continue;
				}

				//�����ˣ������ˣ��ջ���
				if(EGLIET_Skill == p->eType)
				{
					RecycleRareArea(p->n64ID,p->dwAreaID);
				}
				
				tagDelLinitedItem del;
				del.dwItemID = p->dwID;
				del.n64ID = p->n64ID;
				vecDel.push_back(del);
			
				it++;
			}
		}

		std::vector<tagDelLinitedItem>::iterator it1;
		for(it1 = vecDel.begin(); it1 != vecDel.end(); ++it1)
		{
			DelFromLimitedItemMap((*it1).dwItemID,(*it1).n64ID);
		}
	}
}

//----------------------------------------------------------------------------
//����ϡ������
VOID GlobalLimitedItemMgr::RecycleRareArea( DWORD dwRoleID, DWORD dwAreaID )
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID) ;
	if (P_VALID(pRole))
	{//��ɫ����
		//ȡ����������
		pRole->ChangeAreaAbility(dwAreaID,FALSE,FALSE);

		//ȡ������
		tagNS_AddArea send;
		send.dwAreaID = dwAreaID;
		send.dwErrorCode = pRole->ChangeArea(dwAreaID,FALSE);
		send.bAdd = FALSE;
		if( E_Success != send.dwErrorCode)
		{
			return;
		}
		pRole->SendMessage(&send, send.dwSize);
	}
	else
	{//��ɫ�����ߣ�ֱ��ɾ��
		g_dbSession.RemoveAreaAbilityOffline(dwRoleID,dwAreaID);
	}

	if( dwRoleID > 0 )
	{
		g_dbSession.SendDeleteArea2DB(dwRoleID,dwAreaID);
	}
}
//----------------------------------------------------------------------------
//�½�����
//VOID GlobalLimitedItemMgr::SendCreateRareArea2DB( tagGlobalLimitedItem *pGlobalLimitedItem )
//{
//	if (!P_VALID(pGlobalLimitedItem))
//		return;
//
//	MCREATE_MSG(pSend, sizeof(tagNDBC_CreateGlobalLimitedItem), NDBC_CreateGlobalLimitedItem);
//
//	if (!P_VALID(pSend))
//		return;
//
//	pSend->item.n64ID = pGlobalLimitedItem->n64ID;
//	pSend->item.dwAreaID = pGlobalLimitedItem->dwAreaID;
//	pSend->item.eType = pGlobalLimitedItem->eType;
//	pSend->item.dwID = pGlobalLimitedItem->dwID;
//	pSend->item.nTime = pGlobalLimitedItem->nTime;
//	pSend->item.dwRecordTime = pGlobalLimitedItem->dwRecordTime;
//	
//	g_dbSession.Send(pSend, pSend->dwSize);
//
//	MDEL_MSG(pSend);
//}

//----------------------------------------------------------------------------
//��������
VOID GlobalLimitedItemMgr::SendUpdateRareArea2DB( tagGlobalLimitedItem *pGlobalLimitedItem, INT64 n64ID )
{
	if (!P_VALID(pGlobalLimitedItem))
		return;

	MCREATE_MSG(pSend, sizeof(tagNDBC_UpdateGlobalLimitedItem), NDBC_UpdateGlobalLimitedItem);
	
	if (!P_VALID(pSend))
		return;

	pSend->item.n64ID = pGlobalLimitedItem->n64ID;
	pSend->item.dwAreaID = pGlobalLimitedItem->dwAreaID;
	pSend->item.eType = pGlobalLimitedItem->eType;
	pSend->item.dwID = pGlobalLimitedItem->dwID;
	pSend->item.nTime = pGlobalLimitedItem->nTime;
	pSend->item.dwRecordTime = g_world.GetWorldTime();
	pSend->nOriginal64ID = n64ID;

	g_dbSession.Send(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}
//----------------------------------------------------------------------------
//ɾ������
//VOID GlobalLimitedItemMgr::SendDeleteRareArea2DB(INT64 n64ID, DWORD dwAreaID)
//{
//	MCREATE_MSG(pSend, sizeof(tagNDBC_DeleteGlobalLimitedItem), NDBC_DeleteGlobalLimitedItem);
//
//	if (!P_VALID(pSend))
//		return;
//
//	pSend->item.n64ID = n64ID;
//	pSend->item.dwAreaID = dwAreaID;
//
//	g_dbSession.Send(pSend, pSend->dwSize);
//
//	MDEL_MSG(pSend);
//}
//----------------------------------------------------------------------------
//���һ������
tagGlobalLimitedItem *GlobalLimitedItemMgr::GetLimitedItemFromMap( DWORD dwItemID, INT64 n64Serial)
{
	std::vector<tagGlobalLimitedItem*>* pTmp = m_mapLimitedItem.Peek(dwItemID);
	if (P_VALID(pTmp) && !pTmp->empty())
	{
		std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
		tagGlobalLimitedItem* pItem = NULL;
		while(it != pTmp->end())
		{
			tagGlobalLimitedItem* p = *it;
			if(!P_VALID(p))
			{
				it++;
				continue;
			}

			if(dwItemID == p->dwID && n64Serial == p->n64ID)
			{
				return p;
			}
			else
			{
				it++;
			}
		}
	}

	return NULL;
}
//----------------------------------------------------------------------------
//���һ������
tagGlobalLimitedItem *GlobalLimitedItemMgr::GetLimitedItemFromMapByAreaID( DWORD dwAreaID, INT64 n64Serial)
{
	DWORD dwItemTypeID = 0;
	std::vector<tagGlobalLimitedItem*>* pTmp = NULL;
	m_mapLimitedItem.ResetIterator();

	while(m_mapLimitedItem.PeekNext(dwItemTypeID, pTmp))
	{
		if (P_VALID(pTmp) && !pTmp->empty())
		{
			std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
			tagGlobalLimitedItem* pItem = NULL;
			while(it != pTmp->end())
			{
				tagGlobalLimitedItem* p = *it;
				if(!P_VALID(p))
				{
					it++;
					continue;
				}

				if(dwAreaID == p->dwAreaID && n64Serial == p->n64ID)
				{
					return p;
				}
				else
				{
					it++;
				}
			}
		}
	}

	return NULL;
}
//----------------------------------------------------------------------------
VOID GlobalLimitedItemMgr::DelFromLimitedItemMap(DWORD dwTypeID, INT64 n64Serial)
{
	std::vector<tagGlobalLimitedItem*>* pTmp = m_mapLimitedItem.Peek(dwTypeID);
	if (P_VALID(pTmp) && !pTmp->empty())
	{
		std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
		tagGlobalLimitedItem* pItem = NULL;
		while(it != pTmp->end())
		{
			tagGlobalLimitedItem* p = *it;
			if(!P_VALID(p))
			{
				it++;
				continue;
			}

			if(dwTypeID == p->dwID && n64Serial == p->n64ID)
			{
				pTmp->erase(it);
				SAFE_DEL(p);

				tagNDBC_UnSaveGlobalLimitedItem send;
				send.dwItemID = dwTypeID;
				send.n64Serial = n64Serial;
				g_dbSession.Send(&send, send.dwSize);

	/*			tagNDBC_DelGlobalLimitedItem send2;
				send2.dwItemID = dwTypeID;
				send.n64Serial = n64Serial;
				g_dbSession.Send(&send2, send2.dwSize);*/

				if (pTmp->empty())
				{
					m_mapLimitedItem.Erase(dwTypeID);
				}

				break;
			}
			else
			{
				it++;
			}
		}
	}
}
//----------------------------------------------------------------------------
//�洢�������ݵ�DB
VOID GlobalLimitedItemMgr::SaveAllData2DB()
{
	DWORD dwItemTypeID = 0;
	std::vector<tagGlobalLimitedItem*>* pTmp = NULL;
	m_mapLimitedItem.ResetIterator();

	while(m_mapLimitedItem.PeekNext(dwItemTypeID, pTmp))
	{
		if (P_VALID(pTmp) && !pTmp->empty())
		{
			std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
			tagGlobalLimitedItem* pItem = NULL;
			while(it != pTmp->end())
			{
				tagGlobalLimitedItem* pGlobalLimitedItem = *it;
				if(!P_VALID(pGlobalLimitedItem))
				{
					it++;
					continue;
				}

				SendUpdateRareArea2DB(pGlobalLimitedItem,pGlobalLimitedItem->n64ID);
				it++;
			}
		}
	}
}
//----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//!\file title_handler.cpp
//!\author xlguo
//!
//!\date 2009-01-20
//! last 2009-02-02
//!
//!\brief �ƺ���Ϣ�������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/msg_role_title.h"


#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "role_mgr.h"
#include "title_mgr.h"
//-----------------------------------------------------------------------------
// ���ý�ɫʹ��ĳ�ƺ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleActiveTitle(tagNetCmd* pCmd)
{
	tagNC_ActiveRoleTitle* pRecv = (tagNC_ActiveRoleTitle*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;
	
	// ֻ�����ñ���ɫ
	if (GetRole()->GetID() != pRecv->dwRoleID)
		return GT_INVALID;

	// ����ƺ�
	TitleMgr* pTitleMgr = GetRole()->GetTitleMgr();
	DWORD dwRtv = pTitleMgr->ActiviteTitle(pRecv->u16TitleID);


	// ���ͽ��
	tagNS_ActiveRoleTitle send;
	send.dwRoleID = GetRole()->GetID();
	send.u16TitleID = pTitleMgr->GetActiviteTitle();
	send.dwErrorCode = dwRtv;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// �����ý�ɫӵ�е����гƺ�
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetTitles(tagNetCmd* pCmd)
{
	tagNC_GetRoleTitles* pRecv = (tagNC_GetRoleTitles*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	Role* pRole = pRecv->dwRoleID == GetRole()->GetID() ? GetRole() : GetOtherInMap(pRecv->dwRoleID);
	if ( !P_VALID(pRole) )
		return 0;

	TitleMgr* pTitleMgr = pRole->GetTitleMgr();

// 	// �鿴Ȩ��
// 	if ( pRole->GetID() != m_pRole->GetID() && !pTitleMgr.Visibility() )
// 	{	// ���ɷ���
// 		tagNS_GetRoleTitles send;
// 		send.dwErrorCode = E_Title_NotVisible;
// 		send.dwRoleID = pRecv->dwRoleID;
// 		send.u16TitleNum = 0;
// 		SendMessage(&send, send.dwSize);
// 	}
// 	else
// 	{	// �ɷ���
		DWORD dwTitlesSize = pTitleMgr->GetObtainedTitlesNum() * sizeof(UINT16);
		DWORD dwMsgSize = sizeof(tagNS_GetRoleTitles) - 1 + (dwTitlesSize > 0 ? dwTitlesSize : 1);

		ASSERT(dwMsgSize >= sizeof(tagNS_GetRoleTitles));

		MCREATE_MSG(pSend, dwMsgSize, NS_GetRoleTitles);
		//tagNS_GetRoleTitles* pSend;
		UINT16* pu16 = reinterpret_cast<UINT16 *>(pSend->byData);
		DWORD dwRtv = pTitleMgr->GetObtainedTitleIDs(pu16, pSend->u16TitleNum);
		pSend->dwErrorCode = dwRtv;
		pSend->dwRoleID = pRecv->dwRoleID;
		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
//	}

	//�����ѹ��ڵĳƺ�------------------------
	TMap<UINT16, DOUBLE> mapTimeRemainTemp;
	pTitleMgr->GetRemainTimeMap( mapTimeRemainTemp );
	if ( mapTimeRemainTemp.Size() > 0 )
	{
		TMap<UINT16, DOUBLE>::TMapIterator it = mapTimeRemainTemp.Begin();
		DOUBLE dwRemainTime = NULL;

		INT		nCount = 0;
		UINT16	u16TitleIDTemp;
		DWORD   dwTempCount = 0 ; // ʣ��ʱ��ĳƺŸ���

		//Ϊ��Ϣ��ֵ
		while( mapTimeRemainTemp.PeekNext(it, u16TitleIDTemp, dwRemainTime ) )
		{
			if ( 0 >= dwRemainTime )
			{
				++nCount;
			}
			else if( dwRemainTime > 0 )
			{
				dwTempCount++;
			}
		}

	    //-----------------------------------------------------
		// ���ӷ���δ���ڵ�ʱ���Ƴƺŵ�ʣ��ʱ�� 
		//------------------------------------------------------
		DWORD dwTempSize = sizeof(tagNS_TitleRemainTime) + (dwTempCount - 1 )*sizeof(tagTitleRemainTime);
        MCREATE_MSG(pSendMsg, dwTempSize, NS_TitleRemainTime);
        pSendMsg->dwRoleID    = pRecv->dwRoleID;
		pSendMsg->u16TitleNum = dwTempCount ;

		UINT16 u16TitleNum = nCount;
          
		DWORD dwSize = sizeof(tagNS_OutOfDataTitle) - 1 + u16TitleNum * sizeof(UINT16);
		MCREATE_MSG(pSend, dwSize, NS_OutOfDataTitle);

		UINT16* pu16TitleID = reinterpret_cast<UINT16*>(pSend->byData);

		INT u16CountTemp = 0;
		INT nI = 0 ;
		TMap<UINT16, DOUBLE>::TMapIterator it2 = mapTimeRemainTemp.Begin();
		while( mapTimeRemainTemp.PeekNext(it2, u16TitleIDTemp, dwRemainTime ) )
		{
			if ( 0 == dwRemainTime )
			{
				pu16TitleID[u16CountTemp] = u16TitleIDTemp;
				//++nCount;
				++u16CountTemp;
			}	
			else if ( dwRemainTime > 0)
			{
				if ( nI  < dwTempCount )
				{

					pSendMsg->byData[nI].dwTitleID    = u16TitleIDTemp;
					pSendMsg->byData[nI].dwRemainTime = dwRemainTime;
				}
				nI++ ;
			}
		}


		pSend->dwRoleID = pRecv->dwRoleID;
		pSend->u16TitleNum = u16TitleNum;

		//������Ϣ
		m_pRole->SendMessage(pSend, pSend->dwSize);

		m_pRole->SendMessage(pSendMsg,pSendMsg->dwSize);
        
		MDEL_MSG(pSend);
		MDEL_MSG(pSendMsg);

	}	
	//------------------------------------------
	return 0;
}

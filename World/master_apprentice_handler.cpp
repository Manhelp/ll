//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: master_apprentice_handler.h
// author: twan
// actor:
// data: 2010-01-20
// last: 
// brief: ʦͽ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "social_mgr.h"
#include "att_res.h"
#include "currency.h"
#include "../WorldDefine/msg_master_apprentice.h"
#include "../WorldDefine/msg_social.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/chat.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "force_breakout_apprentice_mgr.h"

//--------------------------------------------------------------------------
// ͽ�������ʦ
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleApprenticeApplyMaster(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApprenticeApplyMaster);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	tagNS_ApprenticeApplyMaster send;

	// ����ʦ��
	if (pRole->GetMasterID() != GT_INVALID)
	{
		send.dwErrorCode = E_MA_Already_Has_Master;
	}

	// �ȼ����ڵ���50��
	else if (pRole->GetLevel() >= MASTER_QUALIFICATION_LEVEL)
	{
		send.dwErrorCode = E_MA_Level_Limit;
	}

	// �Է������ʸ���ͽ
	else if (!pDestRole->IsHaveDoneQuest(MASTER_QUALIFICATION_QUEST_ID))
	{
		send.dwErrorCode = E_MA_DestRole_Have_No_Qualification;
	}

	// �Է�ͽ����������
	else if (pDestRole->GetApprenticeData().Size() >= MAX_APPRENTICE_NUM)
	{
		send.dwErrorCode = E_MA_DestRole_Apprentice_Full;
	}

	// �Լ��ĺ�����λ�����ҶԷ���Ϊ�Լ�����
	else if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
	{
		send.dwErrorCode = E_MA_Friend_Full;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
	}

	SendMessage(&send, send.dwSize);

	if (send.dwErrorCode == E_MA_Success)
	{
		// ����֪ͨ��ʦ����
		tagNS_MasterNotice sendMaster;
		sendMaster.dwRoleID = pRole->GetID();
		sendMaster.nLevel = pRole->GetLevel();
		pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
	}

	return 0;
}

//--------------------------------------------------------------------------
// ʦ���Ļظ�
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleMasterReply(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MasterReply);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	// ����Ѿ���ʦͽ��ϵ
	if (pDestRole->GetMasterID() == pRole->GetID())
	{
		return GT_INVALID;
	}

	tagNS_MasterReply				sendMaster;			// ����ʦ������Ϣ
	tagNS_MasterReplyApprentice		sendApprentice;		// ����˫������Ϣ

	sendApprentice.bAccept = pRecv->bAccept;
	sendApprentice.dwRoleID = pRecv->dwRoleID;			// ͽ��ID

	// ����ܾ�
	if (!pRecv->bAccept)
	{
		sendMaster.dwErrorCode = E_MA_Success;
		sendApprentice.dwErrorCode = E_MA_Dest_Refuse_BaiShi;
		
	}

	// �������
	else
	{
		// �ж�˫�����������Ƿ�����
		if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
		{
			sendMaster.dwErrorCode = E_MA_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Dest_Friend_Full;
			// ���͸�ʦ��
			SendMessage(&sendMaster, sendMaster.dwSize);

			// ���͸�ͽ��
			sendApprentice.bAccept = false;
			sendApprentice.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);
			return 0;
		}

		else if ( pDestRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pDestRole->GetFriendPtr(pRole->GetID())) )
		{
			sendMaster.dwErrorCode = E_MA_Dest_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Friend_Full;
			// ���͸�ʦ��
			SendMessage(&sendMaster, sendMaster.dwSize);

			// ���͸�ͽ��
			sendApprentice.bAccept = false;
			sendApprentice.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);
			return 0;
		}

		// �ɹ�
		else
		{
			sendMaster.dwErrorCode = E_MA_Success;
			sendApprentice.dwErrorCode = E_MA_Success;

			// ����Ϊ����
			tagNC_RoleMakeFriend sendMasterMakeFriend;
			tagNC_RoleMakeFriend sendApprenticeMakeFriend;
			sendMasterMakeFriend.dwDestRoleID = pDestRole->GetID();
			sendApprenticeMakeFriend.dwDestRoleID = pRole->GetID();
			g_socialMgr.AddEvent(pRole->GetID(), EVT_MakeFriend, sendMasterMakeFriend.dwSize, &sendMasterMakeFriend);
			g_socialMgr.AddEvent(pDestRole->GetID(), EVT_MakeFriend, sendApprenticeMakeFriend.dwSize, &sendApprenticeMakeFriend);

			// ����Ϊʦͽ
			pRole->AddApprentice(pDestRole->GetID());
			pDestRole->SetMasterID(pRole->GetID());

			// ��ͬ����Ϣͬ����ͽ��
			pRole->GetApprenticeData().ResetIterator();
			DWORD dwMateID = 0;
			while (pRole->GetApprenticeData().PeekNext(dwMateID))
			{
				// ��Ϊͬ��
				pDestRole->AddClassMate(dwMateID);
				Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
				if (P_VALID(pMate))
				{
					pMate->AddClassMate(pRecv->dwRoleID);
				}
			}
			pDestRole->GetClassMate().nJingWuPoint = pRole->GetJingWuPoint();
			pDestRole->GetClassMate().nQinWuPoint = pRole->GetQinWuPoint();

			// ���浽���ݿ�
			//DWORD dwSize = sizeof(DWORD) + sizeof(tagOneMAData2DB) + sizeof(tagApprenticeData);
			//MCREATE_MSG(pSend, dwSize, NDBC_SaveOneMasterApprenticeData);
			tagNDBC_SaveOneMasterApprenticeData send;

			send.dwMasterID = pRole->GetID();
			send.sOneMAData.eType = EOMAD_Apprentice;
			send.sOneMAData.eMode = EMAMM_Mod;
			*(DWORD*)(send.sOneMAData.byData) = pDestRole->GetID();

			g_dbSession.Send(&send, send.dwSize);

			// ����ƹ���
			//tstringstream tszMsg;
			//TCHAR tszRoleName[X_SHORT_NAME] = {0};
			//g_roleMgr.GetRoleNameByID(pRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("�ѽ�");
			//g_roleMgr.GetRoleNameByID(pDestRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("��Ϊͽ�ܣ�");

			//DWORD dwLen = sizeof(tagNS_AutoNotice) - sizeof(TCHAR) + 2*(tszMsg.str().length() + 1);
			//MCREATE_MSG(pSendMsg, dwLen, NS_AutoNotice);

			//pSendMsg->nCirInterval = 60;
			//pSendMsg->nType = 2;		// ��ɫ����
			//_tcscpy(pSendMsg->szContent, tszMsg.str().c_str());
			//g_roleMgr.SendWorldMsg(pSendMsg, dwLen);
			//MDEL_MSG(pSendMsg);
			const RoleScript* pScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pScript))
			{
				pScript->OnHandleMaster(pRole, pDestRole);
			}

			// ����˫����ʦͽ��ϵ
			pRole->ModifyMasterApprenticeCount(1);
			pDestRole->ModifyMasterApprenticeCount(1);

			// ͬ��ʦ��������ͽ�ܵ�
			pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pRole->GetOnlineClassmatesCnt());
			pRole->SyncOnlineNum2Apprentices();
		}
	}

	// ���͸�ʦ��
	SendMessage(&sendMaster, sendMaster.dwSize);

	// ���͸�˫��
	sendApprentice.dwRoleID = pRole->GetID();
	pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);

	if (sendApprentice.dwErrorCode != E_MA_Success)
	{
		sendApprentice.dwErrorCode = E_MA_You_Refuse_BaiShi;
	}
	
	sendApprentice.dwRoleID = pDestRole->GetID();
	SendMessage(&sendApprentice, sendApprentice.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// ʦ��������ͽ
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleMasterAcceptApprentice(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MasterAcceptApprentice);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	tagNS_MasterAcceptApprentice send;

	// �Է�����ʦ��
	if (pDestRole->GetMasterID() != GT_INVALID)
	{
		send.dwErrorCode = E_MA_Dest_Already_Has_Master;
	}

	// �Է��ȼ����ڵ���50��
	else if (pDestRole->GetLevel() >= MASTER_QUALIFICATION_LEVEL)
	{
		send.dwErrorCode = E_MA_Dest_Level_Limit;
	}

	// �������ʸ���ͽ
	else if (!pRole->IsHaveDoneQuest(MASTER_QUALIFICATION_QUEST_ID))
	{
		send.dwErrorCode = E_MA_Have_No_Qualification;
	}

	// ��ͽ����������
	else if (pRole->GetApprenticeData().Size() >= MAX_APPRENTICE_NUM)
	{
		send.dwErrorCode = E_MA_Apprentice_Full;
	}

	// �Լ��ĺ�����λ�����ҶԷ���Ϊ�Լ�����
	else if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
	{
		send.dwErrorCode = E_MA_Friend_Full;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
	}

	SendMessage(&send, send.dwSize);

	if (send.dwErrorCode == E_MA_Success)
	{
		// ����֪ͨ��ͽ�ܷ�
		tagNS_ApprenticeNotice sendApprentice;
		sendApprentice.dwRoleID = pRole->GetID();
		sendApprentice.nLevel = pRole->GetLevel();
		pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);
	}

	return 0;
}

//--------------------------------------------------------------------------
// ͽ�ܵĻظ�
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleApprenticeReply(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApprenticeReply);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	// �Ѿ���ʦͽ��ϵ
	if (pRole->GetMasterID() == pDestRole->GetID())
	{
		return GT_INVALID;
	}

	tagNS_ApprenticeReply				sendApprentice;			// ����ͽ�ܵ���Ϣ
	tagNS_ApprenticeReplyMaster			sendMaster;				// ����˫������Ϣ

	sendMaster.bAccept = pRecv->bAccept;
	sendMaster.dwRoleID = pRecv->dwRoleID;

	// ����ܾ�
	if (!pRecv->bAccept)
	{
		sendMaster.dwErrorCode = E_MA_Dest_Refuse_ShouTu;
		sendApprentice.dwErrorCode = E_MA_Success;

	}

	// �������
	else
	{
		// �ж�˫�����������Ƿ�����
		if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
		{
			sendMaster.dwErrorCode = E_MA_Dest_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Friend_Full;

			// ���͸�ͽ��
			SendMessage(&sendApprentice, sendApprentice.dwSize);

			// ���͸�ʦ��
			sendMaster.bAccept = false;
			sendMaster.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
			return 0;
		}

		else if ( pDestRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pDestRole->GetFriendPtr(pRole->GetID())) )
		{
			sendMaster.dwErrorCode = E_MA_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Dest_Friend_Full;

			// ���͸�ͽ��
			SendMessage(&sendApprentice, sendApprentice.dwSize);

			// ���͸�ʦ��
			sendMaster.bAccept = false;
			sendMaster.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
			return 0;
		}

		// �ɹ�
		else
		{
			sendMaster.dwErrorCode = E_MA_Success;
			sendApprentice.dwErrorCode = E_MA_Success;

			// ����Ϊ����
			tagNC_RoleMakeFriend sendMasterMakeFriend;
			tagNC_RoleMakeFriend sendApprenticeMakeFriend;
			sendMasterMakeFriend.dwDestRoleID = pRole->GetID();
			sendApprenticeMakeFriend.dwDestRoleID = pDestRole->GetID();
			g_socialMgr.AddEvent(pDestRole->GetID(), EVT_MakeFriend, sendMasterMakeFriend.dwSize, &sendMasterMakeFriend);
			g_socialMgr.AddEvent(pRole->GetID(), EVT_MakeFriend, sendApprenticeMakeFriend.dwSize, &sendApprenticeMakeFriend);

			// ����Ϊʦͽ
			pRole->SetMasterID(pDestRole->GetID());
			pDestRole->AddApprentice(pRole->GetID());

			// ��ͬ����Ϣͬ����ͽ��
			pDestRole->GetApprenticeData().ResetIterator();
			DWORD dwMateID = 0;
			while (pDestRole->GetApprenticeData().PeekNext(dwMateID))
			{
				pRole->AddClassMate(dwMateID);
				Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
				if (P_VALID(pMate))
				{
					pMate->AddClassMate(pRole->GetID());
				}
			}
			pRole->GetClassMate().nJingWuPoint = pDestRole->GetJingWuPoint();
			pRole->GetClassMate().nQinWuPoint = pDestRole->GetQinWuPoint();

			// ���浽���ݿ�
			//DWORD dwSize = sizeof(DWORD) + sizeof(tagOneMAData2DB) + sizeof(tagApprenticeData);
			//MCREATE_MSG(pSend, dwSize, NDBC_SaveOneMasterApprenticeData);
			tagNDBC_SaveOneMasterApprenticeData send;
			
			send.dwMasterID = pDestRole->GetID();
			send.sOneMAData.eType = EOMAD_Apprentice;
			send.sOneMAData.eMode = EMAMM_Mod;
			*(DWORD*)(send.sOneMAData.byData) = pRole->GetID();
			
			g_dbSession.Send(&send, send.dwSize);

			// ����ƹ���
			//tstringstream tszMsg;
			//TCHAR tszRoleName[X_SHORT_NAME] = {0};
			//g_roleMgr.GetRoleNameByID(pDestRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("�ѽ�");
			//g_roleMgr.GetRoleNameByID(pRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("��Ϊͽ�ܣ�");

			//DWORD dwLen = sizeof(tagNS_AutoNotice) - sizeof(TCHAR) + 2*(tszMsg.str().length() + 1);
			//MCREATE_MSG(pSendMsg, dwLen, NS_AutoNotice);

			//pSendMsg->nCirInterval = 60;
			//pSendMsg->nType = 2;		// ��ɫ����
			//_tcscpy(pSendMsg->szContent, tszMsg.str().c_str());
			//g_roleMgr.SendWorldMsg(pSendMsg, dwLen);
			//MDEL_MSG(pSendMsg);

			const RoleScript* pScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pScript))
			{
				pScript->OnHandleMaster(pDestRole, pRole);
			}

			// ����˫����ʦͽ��ϵ
			pRole->ModifyMasterApprenticeCount(1);
			pDestRole->ModifyMasterApprenticeCount(1);

			// ͬ��ʦ��������ͽ�ܵ�
			pDestRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pDestRole->GetOnlineClassmatesCnt());
			pDestRole->SyncOnlineNum2Apprentices();
		}
	}

	// ���͸�ͽ��
	SendMessage(&sendApprentice, sendApprentice.dwSize);

	// ���͸�˫��
	sendMaster.dwRoleID = pRole->GetID();
	pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);

	if (sendMaster.dwErrorCode != E_MA_Success)
	{
		sendMaster.dwErrorCode = E_MA_You_Refuse_ShouTu;
	}
	
	sendMaster.dwRoleID = pDestRole->GetID();
	SendMessage(&sendMaster, sendMaster.dwSize);
	return 0;
}

//--------------------------------------------------------------------------
// �鿴�ҵ�ʦ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleMatesInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MatesInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// �õ�ͬ������
	INT nNum = 0;
	for (INT i=0; i != MAX_APPRENTICE_NUM; ++i)
	{
		if (pRole->GetClassMate().dwData[i] != 0)
		{
			++nNum;
		}
	}

	// ��ȡʦ����������
	tagRoleInfo* pMasterInfo = g_roleMgr.GetRoleInfo(pRole->GetMasterID());
	if (!P_VALID(pMasterInfo))
	{
		return GT_INVALID;
	}
	
	DWORD dwSize = sizeof(tagNS_MatesInfo) + (nNum - 1) * sizeof(tagMatesAndApprenticeInfo);
	MCREATE_MSG(pSend, dwSize, NS_MatesInfo);
	
	pSend->nNum = nNum;
	pSend->nJingWuPoint = pRole->GetJingWuPoint();
	pSend->sMasterInfo.dwRoleID = pMasterInfo->dwRoleID;
	pSend->sMasterInfo.nLevel = (INT32)pMasterInfo->byLevel;
	pSend->sMasterInfo.dwJingWuPoint = pRole->GetClassMate().nJingWuPoint;
	pSend->sMasterInfo.dwQinWuPoint = pRole->GetClassMate().nQinWuPoint;

	for (INT i=0; i != nNum; ++i)
	{
		// ��ȡ��������
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetClassMate().dwData[i]);
		if (!P_VALID(pRoleInfo))
		{
			continue;
		}

		pSend->sMatesInfo[i].dwRoleID = pRoleInfo->dwRoleID;
		pSend->sMatesInfo[i].nLevel = (INT)pRoleInfo->byLevel;
		pSend->sMatesInfo[i].bIsOnLine = pRoleInfo->bOnline;
		pSend->sMatesInfo[i].lastOnLineTime = pRoleInfo->bOnline?GetCurrentDWORDTime(): pRoleInfo->dwTimeLastLogout;
		pSend->sMatesInfo[i].lastQuestTime = tagDWORDTime();			// �ݲ�����ʦͽ����ʱ��
	}

	SendMessage(pSend, pSend->dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// �鿴�ҵ�ͽ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleApprenticeInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApprenticeInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	INT nNum = pRole->GetApprenticeData().Size();
	if (nNum <= 0)
	{
		return GT_INVALID;
	}

	DWORD dwSize = sizeof(tagNS_ApprenticeInfo) + (nNum - 1) * sizeof(tagMatesAndApprenticeInfo);
	MCREATE_MSG(pSend, dwSize, NS_ApprenticeInfo);

	pSend->nNum = nNum;
	pSend->sMasterInfo.dwRoleID = pRole->GetID();
	pSend->sMasterInfo.nLevel = pRole->GetLevel();
	pSend->sMasterInfo.dwJingWuPoint = pRole->GetJingWuPoint();
	pSend->sMasterInfo.dwQinWuPoint = pRole->GetQinWuPoint();

	(pRole->GetApprenticeData()).ResetIterator();
	DWORD dwApprenticeID = GT_INVALID;
	nNum = 0;
	while (pRole->GetApprenticeData().PeekNext(dwApprenticeID))
	{
		// ��ȡ��������
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwApprenticeID);
		if (!P_VALID(pRoleInfo))
		{
			continue;
		}

		pSend->sApprenticeInfo[nNum].dwRoleID = pRoleInfo->dwRoleID;
		pSend->sApprenticeInfo[nNum].nLevel = (INT)pRoleInfo->byLevel;
		pSend->sApprenticeInfo[nNum].bIsOnLine = pRoleInfo->bOnline;
		pSend->sApprenticeInfo[nNum].lastOnLineTime = pRoleInfo->bOnline?GetCurrentDWORDTime():pRoleInfo->dwTimeLastLogout;
		pSend->sApprenticeInfo[nNum].lastQuestTime = tagDWORDTime();			// �ݲ�����ʦͽ����ʱ��

		++nNum;
	}

	SendMessage(pSend, pSend->dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// ʦ�����侫�����
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleGiveJingWuPoint(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GiveJingWuPoint);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRecv->nPoint <= 0)
	{
		return GT_INVALID;
	}

	tagNS_GiveJingWuPoint send;

	// ��ȡͽ��
	Role* pApprentice = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pApprentice))
	{
		// ͽ�ܲ�����
		send.dwErrorCode = E_MA_Not_OnLine;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	if (pRole->GetJingWuPoint() < pRecv->nPoint)
	{
		send.dwErrorCode = E_MA_Not_Enough_JingWuPoint;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	pRole->SetJingWuPoint(pRole->GetJingWuPoint() - pRecv->nPoint);
	pApprentice->SetJingWuPoint(pApprentice->GetJingWuPoint() + pRecv->nPoint);
	send.dwErrorCode = E_MA_Success;
	SendMessage(&send, send.dwSize);

	// ����֪ͨ��ͽ��
	tagNS_GetJingWuPoint sendApprentice;
	sendApprentice.nPoint = pRecv->nPoint;
	pApprentice->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);

	// �������ݿ�
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_JingWu;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// ��������
	*(INT*)(sendDB.sOneMAData.byData) = -pRecv->nPoint;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// ʹ�þ����������ͻ�������
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleJingWu2Client(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRole->GetLevel() >= 50)
	{
		return GT_INVALID;
	}

	tagNS_JingWu2Client send;
	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	send.nNeedJingWuPoint = pProto->nLevelUpNeedPoint;
	send.nNeedYuanBao = pProto->nLevelUpNeedYuanBao;
	send.nJingWuPoint = pRole->GetJingWuPoint();
	send.nYuanBao = pRole->GetCurMgr().GetBagYuanBao();
	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// ͽ��ʹ�þ������
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleUseJingWuPoint(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	tagNS_UseJingWuPoint send;

	// �����������
	if (pRole->GetJingWuPoint() < pProto->nLevelUpNeedPoint)
	{
		send.dwErrorCode = E_MA_Not_Enough_JingWuPoint;
	}

	// ��������
	//else if (pRole->GetCurMgr().GetBagYuanBao() < pProto->nLevelUpNeedYuanBao)
	else if (pRole->GetItemMgr().GetBagSameItemCount(APPRENTICE_LEVEL_UP_ITEM) < pProto->nLevelUpNeedYuanBao)
	{
		send.dwErrorCode = E_MA_Not_Enough_BaoJian;
	}

	else
	{
		pRole->SetJingWuPoint(pRole->GetJingWuPoint() - pProto->nLevelUpNeedPoint);
		pRole->GetItemMgr().RemoveFromRole(APPRENTICE_LEVEL_UP_ITEM, pProto->nLevelUpNeedYuanBao, ELCLD_Apprentice_UseBaoJian);
		
		const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(pRole->GetLevel());
		if (!P_VALID(pEffect))
		{
			return GT_INVALID;
		}

		// ���ýű�
		g_ScriptMgr.GetRoleScript()->OnUseJingWuPoint(pRole, pRole->GetJingWuPoint());

		pRole->ExpChange(pEffect->nExpLevelUp - pRole->GetCurLevelExp());

		// ʹ�õľ����������ת��Ϊʦ�����������
		Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
		if (P_VALID(pMaster))		// ���ʦ������
		{
			pMaster->SetQinWuPoint(pMaster->GetQinWuPoint() + pProto->nLevelUpNeedPoint);
		}

		// �������ݿ�
		tagNDBC_SaveOneMasterApprenticeData sendDB;
		sendDB.dwMasterID = pRole->GetMasterID();
		sendDB.sOneMAData.eType = EOMAD_QinWu;
		sendDB.sOneMAData.eMode = EMAMM_Mod;		// ��������
		*(INT*)(sendDB.sOneMAData.byData) = pProto->nLevelUpNeedPoint;

		g_dbSession.Send(&sendDB, sendDB.dwSize);

		send.dwErrorCode = E_MA_Success;
		send.nCurJingWuPoint = pRole->GetJingWuPoint();

		// ��������ͬ����ʦ����������
		for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
		{
			DWORD dwMateID = pRole->GetClassMate().dwData[i];
			Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
			if (P_VALID(pMate))
			{
				pMate->GetClassMate().nQinWuPoint += pProto->nLevelUpNeedPoint;
			}
		}
	}

	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// ͽ������ʦ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutMaster(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutMaster);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BreakOutMaster send;

	// ��ȡʦ���������ݽṹ
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// �Լ��ѳ�ʦ
	if (pRole->GetLevel() >= GOOD_STU_CAN_LEAVE_LEVEL)
	{
		send.dwErrorCode = E_MA_Success;
	}

	// ������ʦ��3��δ����
	else if (pInfo->bOnline || CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600)
	{
		send.dwErrorCode = E_MA_OnLine_Day_Limit;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
		
	}

	SendMessage(&send, send.dwSize);
	
	return 0;
}

//--------------------------------------------------------------------------
// ǿ������ʦ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleForceBreakOutMaster(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ForceBreakOutMaster);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_ForceBreakOutMaster send;

	// ǿ������ʦ������
	if (g_forceBreakOutApprenticeMgr.FindApprentice(pRole->GetID(), send.dwRemainTime) == FALSE)
	{
		g_forceBreakOutApprenticeMgr.InsertApprentice2List(pRole->GetID(), pRecv->dwRoleID);
		send.dwErrorCode = E_Success;
	}
	else
	{
		send.dwErrorCode = E_MA_Already_Force_BreakOut;
	}
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// �ٴ�ȷ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutMasterConfirm(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutMasterConfirm);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// ��ȡʦ���������ݽṹ
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// ������ɾ������
	if (pRole->GetLevel() < GOOD_STU_CAN_LEAVE_LEVEL && (pInfo->bOnline ||  CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600))
	{
		return GT_INVALID;
	}

	pRole->SetJingWuPoint(0);			// ��վ������
	pRole->SetMasterID(GT_INVALID);		// �ÿ�ʦ��ID

	Role* pMaster = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);

	// ���ʦ������
	if (P_VALID(pMaster))
	{
		pMaster->DeleteApprentice(pRole->GetID());

		pMaster->ModifyMasterApprenticeCount(-1);
		// ��ʦ����������ʦ�Ź�ϵ��Ϣ
		tagNS_BreakOutMasterConfirm send;
		send.dwRoleID = pRole->GetID();
		pMaster->GetSession()->SendMessage(&send, send.dwSize);

		// ֪ͨʦ������ʦ���������ܷ����仯
		pMaster->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMaster->GetOnlineClassmatesCnt());
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(pRecv->dwRoleID, -1);
		g_roleMgr.SaveRoleInfoToDB(pRecv->dwRoleID);
	}

	tagNS_BreakOutMasterConfirm send;
	send.dwRoleID = pRecv->dwRoleID;
	SendMessage(&send, send.dwSize);

	// �������ݿ�
	tagNDBC_DeleteAnApprentice sendDB;
	sendDB.dwMasterID = pRecv->dwRoleID;
	sendDB.dwApprenticeID = pRole->GetID();
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// ͽ�ܸ���ͬ����Ϣ
	DWORD dwMateID = GT_INVALID;
	for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
	{
		dwMateID = pRole->GetClassMate().dwData[i];
		Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
		if (P_VALID(pMate))
		{
			pMate->DeleteClassMate(pRole->GetID());
// 			pMate->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMate->GetOnlineClassmatesCnt());
		}
	}

 	// ����ɾ����ͽ��ͬ��ͬ�������仯
 	pRole->SyncOnlineNum2Classmates();

	// ɾ������ͬ��
	pRole->DeleteAllClassMate();

	// ��֪ͨ�Լ�
	pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pRole->GetOnlineClassmatesCnt());

	// ����һ��ʦͽ��ϵ
	pRole->ModifyMasterApprenticeCount(-1);
	
	return 0;
}

//--------------------------------------------------------------------------
// ʦ�����ͽ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutApprentice(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutApprentice);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BreakOutApprentice send;

	// ��ȡͽ���������ݽṹ
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// ͽ���ѳ�ʦ
	if (pInfo->byLevel >= GOOD_STU_CAN_LEAVE_LEVEL)
	{
		send.dwErrorCode = E_MA_Success;
	}

	// ������ͽ��3��δ����
	else if (pInfo->bOnline || CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600)
	{
		send.dwErrorCode = E_MA_OnLine_Day_Limit;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
		
	}

	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// �ٴ�ȷ��
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutApprenticeConfirm(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutApprenticeConfirm);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// ������ɾ������
	if (pInfo->byLevel < GOOD_STU_CAN_LEAVE_LEVEL && (pInfo->bOnline ||  CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600))
	{
		return GT_INVALID;
	}
	

	Role* pApprentice = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);

	// ���ͽ������
	if (P_VALID(pApprentice))
	{
		pApprentice->SetMasterID(GT_INVALID);
		pApprentice->SetJingWuPoint(0);			// ��վ������
		pApprentice->ModifyMasterApprenticeCount(-1);		

		// ɾ������ͬ��
		pApprentice->DeleteAllClassMate();

		// ��ͽ�ܷ�������ʦ�Ź�ϵ��Ϣ
		tagNS_BreakOutApprenticeConfirm send;
		send.dwRoleID = pRole->GetID();
		pApprentice->GetSession()->SendMessage(&send, send.dwSize);

		// ֪ͨ�����ʦ�ŵ�ͽ��ʦ���������������仯
		pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(pRecv->dwRoleID, -1);
		g_roleMgr.SaveRoleInfoToDB(pRecv->dwRoleID);
	}

	tagNS_BreakOutApprenticeConfirm send;
	send.dwRoleID = pRecv->dwRoleID;
	SendMessage(&send, send.dwSize);

	// �������ݿ�
	tagNDBC_DeleteAnApprentice sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.dwApprenticeID = pRecv->dwRoleID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// ����ͽ����Ϣ
	pRole->DeleteApprentice(pRecv->dwRoleID);

	// ͽ�ܸ���ͬ����Ϣ
	pRole->GetApprenticeData().ResetIterator();
	DWORD dwApprenticeID = GT_INVALID;
	while (pRole->GetApprenticeData().PeekNext(dwApprenticeID))
	{
		Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
		if (P_VALID(pApprentice))
		{
			pApprentice->DeleteClassMate(pRecv->dwRoleID);

			// ֪ͨͽ������ʦ�����������仯
			pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
		}
	}

	// ֪ͨʦ������ʦ�����������仯
	pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pRole->GetOnlineClassmatesCnt());

// 	// ֪ͨͽ������ʦ�����������仯
// 	pRole->SyncOnlineNum2Apprentices();


	// ����һ��ʦͽ��ϵ
	pRole->ModifyMasterApprenticeCount(-1);

	return 0;
}
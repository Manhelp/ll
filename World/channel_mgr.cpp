#include "stdafx.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/msg_GroupChat.h"
#include "channel_mgr.h"
#include "chat_mgr.h"
#include "role.h"
#include "ps_bomb.h"
#include "role_mgr.h"
#include "map.h"
#include "guild.h"
#include "guild_mgr.h"
#include "title_mgr.h"
#include "chat_logger.h"
#include "group_chat.h"
#include "../ServerDefine/family_define.h"
#include "family_member.h"
#include "family.h"
#include "family_join.h"
#include "family_mgr.h"

ChannelMgr g_channelMgr;
// struct SendFilter
// {
// 	SendFilter(ESendChatChannel eChannel)
// 		:m_eChannel(eChannel){}
// 	BOOL operator()(Unit* pUnit)
// 	{
// 		Role* pRole = dynamic_cast<Role*>(pUnit);
// 		if (!P_VALID(pRole))
// 		{
// 			return FALSE;
// 		}
// 		
// 		return TRUE;//pRole->GetChatData()->IsFit(m_eChannel);
// 	}
// 
// private:
// 	ESendChatChannel m_eChannel;
// };

/*
0x1 ˽��
0x2 ����
0x4 ����
0x8 ����
0x10 ����
0x20 ս��
0x40 ����
0x80 ϵͳ\����
0xFF ��ǰ
0x0 NULL
*/
inline DWORD Channel2Flag(ESendChatChannel eChannel)
{
	switch(eChannel)
	{
	case ESCC_NULL:
		return 0;
		break;
	case ESCC_CastTalk:		//����:
		return 0x04;
		break;
	case ESCC_World:		//����:
		return 0x02;
		break;
	case ESCC_Pvt:			//˽��:
		return 0x01;
		break;
	case ESCC_Guild:		//����:
		return 0x08;
		break;
	case ESCC_Troop:		//����:
		return 0x10;
		break;
	case ESCC_Current:		//��ǰ:
		return 0xFF;		//����ȫ��
		break;
	case ESCC_Leave:		//����:
		return 0x40;
		break;
	case ESCC_Combat:		//ս��:
		return 0x20;
		break;
	case ESCC_Affiche:		//����:
		return 0x80;
		break;
	case ESCC_GM:			//GM:
		return 0x0;
		break;
	case ESCC_System:		//ϵͳ:
		return 0x80;
		break;
	case ESCC_Scene:       //����
		return 0x60;
	case ESCC_End:
		return 0;
		break;
	}
	return 0xFF;
}

DWORD ChannelMgr::SendMsgCmd(MsgCmd* pMsgCmd)
{
	MTRANS_ELSE_RET(pSender, pMsgCmd->GetSender(), Role, E_SystemError);
	MTRANS_ELSE_RET(pMap, pSender->GetMap(), Map, E_SystemError);
	MTRANS_ELSE_RET(pSend, pMsgCmd->GetSendMsg(), tagNetCmd, E_SystemError);
	MTRANS_ELSE_RET(pRecv, pMsgCmd->GetRecvMsg(), tagNetCmd, E_SystemError);
	ESendChatChannel eChannel = pMsgCmd->GetChannelType();
	tagNC_RoleChat* pRoleChat = static_cast<tagNC_RoleChat*>(pRecv);

	//��������
	if ( pMsgCmd->GetContentSize() > GetContentSizeLimit(pSend, eChannel) )
		return E_RoleChat_TooLength;

	DWORD dwFlag = Channel2Flag(eChannel);
	if( (dwFlag & g_attRes.GetChatFlag()) != dwFlag)
	{
		// ը�����ܱ�����
		if(eChannel == ESCC_Current)
		{
			tagNC_RoleChat* pRolcChat = static_cast<tagNC_RoleChat*>(pRecv);
			g_pSGuarder.OnMsg(pRolcChat->szMsg, 0);
		}
		return E_RoleChat_ForbidChat;
	}

	// ��������Ƶ����ͬ�ֱ���
	switch( eChannel )
	{
		// ��ǰƵ��
	case ESCC_Current:
		{
			// ���ͻ��˷�����Ϣ
			tagNC_RoleChat* pRolcChat = static_cast<tagNC_RoleChat*>(pRecv);
			if(!g_pSGuarder.OnMsg(pRolcChat->szMsg, 0))
			{
				pMap->ForEachUnitInBigVisTile(pSender, SendAction(eChannel, pSend));
				//pMap->SendBigVisTileMsg(pSender, (LPVOID)pSend, pSend->dwSize);
			}
		}
		break;
		// ˽��
	case ESCC_Pvt:
		{
			//�ж�����Ƿ����
			MTRANS_ELSE_RET(pDestRoleInfo, g_roleMgr.GetRoleInfo(pMsgCmd->GetReceiverID()), tagRoleInfo, E_RoleChat_Pvt_NoRoleName);
			//�ж�����Ƿ�����
			MTRANS_ELSE_RET(pDestRole, g_roleMgr.GetRolePtrByID(pMsgCmd->GetReceiverID()), Role, E_RoleChat_Pvt_RemoteRoleLeave);
			pDestRole->SendMessage(pSend, pSend->dwSize);
			if (pSender != pDestRole)
			{
				pSender->SendMessage(pSend, pSend->dwSize);	
			}			
		}
		break;
		// ����
	case ESCC_World:
		{
			// �ж�ʱ����
			if (!pSender->TalkToWorld())
				return E_RoleChat_World_Frequently;

			// ��ҵȼ��ж�
			//if (pSender->GetLevel() < WORLD_CHANNEL_ROLE_LVL)
			//	E_RoleChat_World_RoleNo10;

			//������Ƶ��������Ҫ�۳� 100 ��
			//�õ�������ϵ��� ����
			/*
			INT64 currSilver = pSender->GetCurMgr().GetBagSilver();
						if ( currSilver < WORLD_CHANNEL_DEC_SLIVER )
						{
							//������
							return E_RoleChat_World_NoMoney;
						}
						else
						{
							//�۳���ұ����� 100��
							pSender->GetCurMgr().DecBagSilver(WORLD_CHANNEL_DEC_SLIVER, ELCLD_CHAT_WORLD_CHANNEL);
						}*/
			
			UINT16 n16FreeWorldTalkTimes = pSender->GetFreeWorldTalkTimes();
			if (n16FreeWorldTalkTimes < pSender->GetMaxFreeWorldTalkTimes())
			{
				n16FreeWorldTalkTimes++;
				pSender->SetFreeWorldTalkTimes(n16FreeWorldTalkTimes);
				if (pSend->dwID == TObjRef<Util>()->Crc32("NS_RoleChat"))
				{
					tagNS_RoleChat* pRoleChat = (tagNS_RoleChat*)pSend;
					pRoleChat->nLeftTimes = pSender->GetMaxFreeWorldTalkTimes() - n16FreeWorldTalkTimes;
				}
				else if (pSend->dwID == TObjRef<Util>()->Crc32("NS_RoleShowEquip"))
				{
					tagNS_RoleShowEquip* pRoleChat = (tagNS_RoleShowEquip*)pSend;
					pRoleChat->nLeftTimes = pSender->GetMaxFreeWorldTalkTimes() - n16FreeWorldTalkTimes;
				}
				else if (pSend->dwID == TObjRef<Util>()->Crc32("NS_RoleShowItem"))
				{
					tagNS_RoleShowItem* pRoleChat = (tagNS_RoleShowItem*)pSend;
					pRoleChat->nLeftTimes = pSender->GetMaxFreeWorldTalkTimes() - n16FreeWorldTalkTimes;
				}
			}
			else
			{
				if (pSend->dwID == TObjRef<Util>()->Crc32("NS_RoleChat"))
				{
					tagNS_RoleChat* pRoleChat = (tagNS_RoleChat*)pSend;
					pRoleChat->nLeftTimes = 0;
				}
				else if (pSend->dwID == TObjRef<Util>()->Crc32("NS_RoleShowEquip"))
				{
					tagNS_RoleShowEquip* pRoleChat = (tagNS_RoleShowEquip*)pSend;
					pRoleChat->nLeftTimes = 0;
				}
				else if (pSend->dwID == TObjRef<Util>()->Crc32("NS_RoleShowItem"))
				{
					tagNS_RoleShowItem* pRoleChat = (tagNS_RoleShowItem*)pSend;
					pRoleChat->nLeftTimes = 0;
				}

				TList<tagItem*> listItems;
				INT nNum = pSender->GetItemMgr().GetBagSameItemList(listItems, TYPEID_IM_WORLD_NETBAR);

				//����ʹ����������
				if (!listItems.Empty())
				{
					// ʹ����Ʒ
					TList<tagItem*>::TListIterator itr = listItems.Begin();
					pSender->GetItemMgr().ItemUsedFromBag((*itr)->GetKey(), (INT16)1, (DWORD)ELCLD_WORLD_TALK_IM_NETBAR);
				}
				else
				{
					pSender->GetItemMgr().GetBagSameItemList(listItems, TYPEID_IM_WORLD); 
					if (!listItems.Empty())
					{
						TList<tagItem*>::TListIterator itr = listItems.Begin();
						pSender->GetItemMgr().ItemUsedFromBag((*itr)->GetKey(), (INT16)1, (DWORD)ELCLD_WORLD_TALK_IM);
					}
					else
					{
						pSender->GetItemMgr().GetBagSameItemList(listItems, TYPEID_IM_WORLD_UNBIND); 
						if (!listItems.Empty())
						{
							TList<tagItem*>::TListIterator itr = listItems.Begin();
							pSender->GetItemMgr().ItemUsedFromBag((*itr)->GetKey(), (INT16)1, (DWORD)ELCLD_WORLD_TALK_IM_UNBIND);
						}
						else
						{
							return E_RoleChat_World_NoIMItem;
						}
					}
				}
			}
			// ���ͻ��˷�����Ϣ
			g_roleMgr.ForEachRoleInWorld(SendAction(eChannel, pSend));
			//g_roleMgr.SendWorldMsg((LPVOID)pSend, pSend->dwSize);
		}
		break;
		//����
	case ESCC_Affiche:
		{
			//�жϷ������ǲ���GMȨ��
			if (pSender->GetSession()->IsPrivilegeEnough(1) == FALSE)
				return E_RoleChat_Not_GM;

			g_roleMgr.ForEachRoleInWorld(SendAction(eChannel, pSend));
			//g_roleMgr.SendWorldMsg((LPVOID)pSend, pSend->dwSize);
		}
		break;
		// ����
	case ESCC_Troop:
		{
			//������Ҷ���ID�ҵ����Ӧ�Ķ���,���������г�Ա������Ϣ
			DWORD dwTeamID = pSender->GetTeamID();
			const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
			if (P_VALID(pTeam))
			{
				pTeam->ForEachRoleInTeam(SendAction(eChannel, pSend));
			}
			//g_groupMgr.SendTeamMessage(dwTeamID, (LPVOID)pSend, pSend->dwSize);
		}
		break;
		// ����
	case ESCC_CastTalk:
		{
			return E_RoleChat_NotOpened;
			TList<tagItem*> listItems;
			INT nNum = pSender->GetItemMgr().GetBagSameItemList(listItems, TYPEID_IM_CASTTALK);


			
			if (!listItems.Empty()){
				// ʹ����Ʒ (����ʹ�ð󶨵���)
				TList<tagItem*>::TListIterator itr = listItems.Begin();
				pSender->GetItemMgr().ItemUsedFromBag((*itr)->GetKey(), (INT16)1, (DWORD)ELCLD_CAST_TALK_IM);

			}else{
                pSender->GetItemMgr().GetBagSameItemList(listItems, TYPEID_IM_CASTTALK_UNBIND);
				if(!listItems.Empty()){
					TList<tagItem*>::TListIterator itr = listItems.Begin(); 
					pSender->GetItemMgr().ItemUsedFromBag((*itr)->GetKey(), (INT16)1, (DWORD)ELCLD_CAST_TALK_IM_UNBIND);
				}else{
                   return E_RoleChat_CastTalk_NoIMItem;
				}



			}

				






			// �ƺ��¼�����
			pSender->GetTitleMgr()->SigEvent(ETE_SHOUT, GT_INVALID, GT_INVALID);

			// ���ͻ��˷�����Ϣ
			g_roleMgr.ForEachRoleInWorld(SendAction(eChannel, pSend));
			//g_roleMgr.SendWorldMsg((LPVOID)pSend, pSend->dwSize);
		}
		break;
		// ����
	case ESCC_Guild:
		{
			// ���ͻ��˷�����Ϣ
			MTRANS_ELSE_RET(pGuild, g_guildMgr.GetGuild(pSender->GetGuildID()), Guild, E_RoleChat_Guild_NoJoin);
			pGuild->ForEachRoleInGuild(SendAction(eChannel, pSend));
			//pGuild->SendGuildMsg(pSend, pSend->dwSize);
		}
		break;
		//����
	case ESCC_Scene:
		// �ж�ʱ����
		if (!pSender->TalkToScene())
			return E_RoleChat_Scene_Frequently;

		if(!g_pSGuarder.OnMsg(pRoleChat->szMsg, 0))
		{
			pMap->ForAllRoleInMap(SendAction(eChannel, pSend));
		}
		break;
	case ESCC_Family:
		{
			// ���ͻ��˷�����Ϣ
			MTRANS_ELSE_RET(pFamily, g_FamilyMgr.GetFamily(pSender->GetFamilyID()), Family, E_RoleChat_NoFamily);
			pFamily->GetFamilyMember().ForEachFamilyMember(SendMsg2Member(pSend));
		}
		break;
		//Ⱥ��
	case ESCC_Group:

		group_chat* pGroupChat = g_GroupChatMgr.GetGroupChat(pRoleChat->dwGroupChatID);
		if( !P_VALID(pGroupChat) )
			return E_RoleChat_GroupChatNotExist;
		if ( !P_VALID(E_RoleChat_GroupChatNotExist) )
		{
			return E_RoleChat_GroupChatNotExist;
		}
		if(!g_pSGuarder.OnMsg(pRoleChat->szMsg, 0))
		{
			pGroupChat->ForEachRoleInGroupChat(SendAction(eChannel, pSend));

		}          
		break;
	}

	TheChatLogger.Log(*pMsgCmd);
	return E_Success;
}

DWORD ChannelMgr::GetContentSizeLimit( tagNetCmd* pNetCmd, BYTE byChannelType )
{
	static const tagNS_RoleChat			msgChat;
	static const tagNS_RoleShowEquip	msgShowEquip;
	static const tagNS_RoleShowItem		msgShowItem;

	DWORD dwSizeLimit = 1024;
	if (msgChat.dwID == pNetCmd->dwID)
	{
		switch(ESendChatChannel(byChannelType))
		{
		case ESCC_CastTalk:
			dwSizeLimit = WORLD_CHANNEL_MSG_LEN;
			break;
		case ESCC_World:
			dwSizeLimit = MAX_CHAT_LEN;
			break;
		case ESCC_Pvt:
		case ESCC_Guild:
		case ESCC_Troop:
		case ESCC_Current:
		case ESCC_Family:
			dwSizeLimit = MAX_CHAT_LEN;
			break;
		case ESCC_Leave:
		case ESCC_Combat:
		case ESCC_Affiche:
		case ESCC_GM:
		case ESCC_System:
		default:
			break;
		}
	}
	else if (msgShowEquip.dwID == pNetCmd->dwID)
	{
		dwSizeLimit = sizeof(tagFabao);
	}
	else if (msgShowItem.dwID == pNetCmd->dwID)
	{
		dwSizeLimit = sizeof(tagItem);
	}

	return dwSizeLimit;
}


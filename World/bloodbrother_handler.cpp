#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "..\WorldDefine\msg_bloodbrother.h"
#include "..\WorldDefine\brother_define.h"
#include "../ServerDefine/log_cmdid_define.h"

DWORD PlayerSession::HandleTryMakeBrother(tagNetCmd * pCmd)
{
	// ����������Ϣ
	MGET_MSG(pRecv, pCmd, NC_TryMakeBrother);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// ��ȡ��ɫ
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwErrorCode = 0;
	tagNS_TryMakeBrother send;


	Team* pTemp = (Team*)g_groupMgr.GetTeamPtr(pRole->GetTeamID());
	if ( !P_VALID(pTemp))
	{
		dwErrorCode = E_bloodbrother_TeamNotHave2;
	}
	else
	{
		if(pTemp->GetMemNum() == 2)
		{
			Role* pRole1 = pTemp->GetMemPtr(0);
			Role* pRole2 = pTemp->GetMemPtr(1);
			if(!P_VALID(pRole1) || !P_VALID(pRole2))
				return GT_INVALID;

			// �ú����Ѵ����ں����б�
			tagFriend *pFriend1 = pRole1->GetFriendPtr(pRole2->GetID());
			tagFriend *pFriend2 = pRole2->GetFriendPtr(pRole1->GetID());
			// �໥������
			if(P_VALID(pFriend1) && P_VALID(pFriend2))
			{
				// �ȼ�Ҫ��
				if ( (pRole1->GetLevel() >= BloodBrother_DistinguishLevel && pRole2->GetLevel() < BloodBrother_DistinguishLevel )
					|| (pRole1->GetLevel() < BloodBrother_DistinguishLevel && pRole2->GetLevel() >= BloodBrother_DistinguishLevel) )
				{
					Role* pBrother = pRole1->GetLevel() >=BloodBrother_DistinguishLevel ? pRole2 :pRole1;
					Role* pTeather = pRole1->GetLevel() >=BloodBrother_DistinguishLevel ? pRole1 :pRole2;
					// �Ƿ���Խ��
					if(pBrother->GetBrotherMgr().CanMakeBrother())
					{
						TList<tagItem*> ItemList;
						tagItem* pItem;
						// ���߲鿴
						if (SearchPack(pBrother->GetItemMgr().GetBag(), ItemList, EISF_BloodBrother, tagFindItemSpecFunc()) == 0)
						{
							dwErrorCode = E_bloodbrother_NotHaveItem;
						}
						else
						{
							bool bFind = false;
							ItemList.ResetIterator();
							while( ItemList.PeekNext(pItem) )
							{
								if (pItem->pProtoType->nSpecFuncVal1 == pRecv->dwDays )
								{
									bFind = true;
								}
							}
							if(bFind)
							{
								tagDWORDTime time = GetCurrentDWORDTime();
								tagDWORDTime timeEnd = IncreaseTime(time,pItem->pProtoType->nSpecFuncVal1*24*60*60);
								// ����ƷҪ��С�����ϵ�
								if (E_Success == pBrother->GetItemMgr().DelFromBag(pItem->n64Serial, ELCID_BloodBrother,1))
								{
									// ����ROLE����ֶ�
									pBrother->MakeBorther(pTeather->GetID());
									pTeather->MakeBorther(0,true);

									pBrother->GetBrotherMgr().SetEndTime(timeEnd);
									pBrother->SyncBrotherEndTime();
									// ���buff
									pBrother->TryAddBuffToMyself(BloodBrotherBuffBrother);
									pTeather->TryAddBuffToMyself(BloodBrotherBuffTeather);

									if( pBrother->GetID() == pRole->GetID())
									{
										pTeather->MakeBrotherSuccess(pBrother->GetID() );
										send.dwRoleID = pTeather->GetID();
									}
									else
									{
										pBrother->MakeBrotherSuccess(pTeather->GetID() );
										send.dwRoleID = pBrother->GetID();
									}
								}
								else
								{
									IMSG(_T("LevelUpWardrobeLayer::DelFromBagFailed!!!\r\n"));
									return GT_INVALID;
								}
							}
							else
								dwErrorCode = E_bloodbrother_NotHaveItem;
						}
					}
					else
						dwErrorCode = E_bloodbrother_AlreadyHaveTeather;
				}
				else
					dwErrorCode = E_bloodbrother_TeamLevelNotFeet;
			}
			else
				dwErrorCode = E_bloodbrother_NotFriend;
		}
		else
			dwErrorCode = E_bloodbrother_TeamNotHave2;
		
	}

	send.dwErrorCode = dwErrorCode;
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}

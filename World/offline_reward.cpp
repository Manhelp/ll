#include "stdafx.h"
#include "offline_reward.h"
#include "player_session.h"
#include "db_session.h"
//#include "../WorldDefine/RoleDefine.h"
//#include "../WorldDefine/role_att.h"
//#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/role_data.h"
#include "../worlddefine/msg_player_preventlost.h"
#include "role.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "player_account.h"
#include "att_res.h"


static INT GetExtraExpOfRole(Role * pPlayer, INT nMinute);

static LONG volatile n32OfflineRewardSwitch = 1;

void OfflineReward::OfflineRewardSwitch(INT switch_v)
{
	//n32OfflineRewardSwitch = switch_v;
	::InterlockedExchange((LPLONG)&n32OfflineRewardSwitch,switch_v);
}
INT OfflineReward::OfflineRewardSwitch()
{
	return n32OfflineRewardSwitch;
}

DWORD OfflineReward::PlayerLogin(Role * player)
{
	if( OfflineRewardSwitch() != 1 )
		return GT_INVALID;

	if( P_VALID(player) )
	{
		PlayerSession * pSession = player->GetSession  ();
		if( P_VALID( pSession ) )
		{
			if( player->IsOfflineHangOpen() )
				return E_OfflineHang;

			// �����ɫ�պ����ϴ����ߵ��Ǹ�
			//if( player->m_bIsOpenOfflineReward )
			if( g_PlayerAccountMgr.CanReceiveDailyOfflineReward  (pSession->GetSessionID  ()) )
			{
				tagNS_OfflineExperienceReward msg;
				INT nAllMinute	= CalOfflineMinutes(player);
				INT hour	= nAllMinute / 60;
				INT minute	= nAllMinute % 60;

				//if( /*minute > 0 || */hour > 0 )
				if( nAllMinute > 0 )
				{
					msg.byHour	= hour;
					msg.byMinute = minute;
					msg.n64FreeExperience	= CalOfflineRewardFreeExp(player,nAllMinute);
					//msg.n64PayNeededExperience = msg.n64FreeExperience;//CalOfflineRewardPayedExp(player);
					msg.n64PayNeededExperience = CalOfflineRewardPayedExp(player,nAllMinute);
					msg.nYuanBao	= GetExtraExpOfRole(player,nAllMinute);

					//if( msg.n64FreeExperience > 0/* || msg.n32PayNeededExperience > 0*/ )
					if( msg.n64FreeExperience > 0 || msg.n64PayNeededExperience > 0 )
						pSession->SendMessage((LPBYTE)&msg,msg.dwSize);
				}
				return E_Success;
			}
			//return E_RefuseOfflineReward;
		}
	}
	return E_Offline_NotFindPlayer;
}
DWORD OfflineReward::PlayerLogout(Role * player,BOOL exitGame)
{
	if( OfflineRewardSwitch() != 1 )
		return GT_INVALID;

	if( P_VALID(player) )
	{
		PlayerSession * pSession = player->GetSession  ();
		if( P_VALID( pSession ) )
		{
			if( player->IsOfflineHangOpen() )
				return E_OfflineHang;

			//if(  !player->m_bIsOpenOfflineReward && exitGame == false )
			//	return E_RefuseOfflineReward;

			//player->m_bIsOpenOfflineReward = 1;

			//m_dwAccountID	= pSession->GetSessionID  ();
			//m_dwRoleID		= player->GetID  ();
			//if( exitGame )
			//{
			//	//���浽db����dbsession������Ϣ
			//	tagNDBC_UpdateAccountOfflineRewardInfo msg;
			//	msg.dwAccountID	= m_dwAccountID;
			//	msg.dwRoleID	= m_dwRoleID;
			//	g_dbSession.Send  (&msg,msg.dwSize);
			//}
			return E_Success;
		}
	}
	return E_Offline_NotFindPlayer;
}
INT64 OfflineReward::CalOfflineRewardFreeExp(Role * player, INT nMinute)
{
	if( player->GetLevel() < 20 )
		return 0;

	const tagExpOfflineProto* pExpOfflineProto = g_attRes.GetExpOfflineProto(player->GetLevel());
	if(!P_VALID(pExpOfflineProto))
		return 0;


	return pExpOfflineProto->dwExpbase * nMinute;

	//return CalOfflineExp(player);
}
INT64 OfflineReward::CalOfflineRewardPayedExp(Role * player, INT nMinute)
{
	if( player->GetLevel() < 20 )
		return 0;

	const tagExpOfflineProto* pExpOfflineProto = g_attRes.GetExpOfflineProto(player->GetLevel());
	if(!P_VALID(pExpOfflineProto))
		return 0;


	return pExpOfflineProto->dwExpAdditional * nMinute;

	//return CalOfflineExp(player);
}
INT64 OfflineReward::CalOfflineExp(Role * player)
{
	// ���ָ��Ϸ���������ҿ��������߹һ�����(tagNC_ExitGamePrompt������tagNC_ExitGamePromptEx�б�ʾһ�¾Ϳ�����)
	//if( P_VALID(player) )
	// �ϲ��жϣ���ߵ�Ч��
	{
		//if( !player->m_bIsOpenOfflineReward )
		//	return 0;

		//DWORD deltaTime = 0;
		//if( player->m_LogoutTime < player->m_LoginTime )
		{
			//deltaTime = CalcTimeDiff( player->m_LoginTime, player->m_LogoutTime );
			INT minute = CalOfflineMinutes(player);//deltaTime / 60 ;
			if( minute >= 60/*0*/ )
			{
				const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(player->GetLevel  ());
				if( P_VALID( pEffect ) )
				{
					INT64 levelExp = pEffect->nExpLevelUp;
					if( levelExp > 0 )
					{
						INT t0 = player->GetLevel  () + 10;
						// ������
						INT64 t = (INT64)(( (double)minute / E_OfflineTimeout * levelExp * 150 ) / ( t0 * t0 ));

						return t;
					}
				}

			}
		}
	}
	return 0;
}

INT OfflineReward::CalOfflineMinutes(Role * player)
{
	//if( player->m_iniLogoutTime < player->m_LoginTime )
	{
		//DWORD deltaTime = CalcTimeDiff( player->m_LoginTime, player->m_iniLogoutTime );
		//INT minute = deltaTime / 60 ;
		//if( minute > E_OfflineTimeout )
		//	minute = E_OfflineTimeout;
		//return minute;
		PlayerSession * pSession = player->GetSession  ();
		if( P_VALID(pSession) )
		{
			DWORD deltaTime = g_PlayerAccountMgr.GetDailyOfflineTime(pSession->GetSessionID  ());//CalcTimeDiff( player->m_LoginTime, player->m_iniLogoutTime );
			INT minute = deltaTime / 60 ;
			if( minute > E_OfflineTimeout )
				minute = E_OfflineTimeout;
			return minute;
		}
	}
	return 0;
}

DWORD OfflineReward::RecExp(Role * player,BYTE what,INT & exp1)
{
	exp1 = 0;
	if( OfflineRewardSwitch() != 1 )
		return GT_INVALID;
	if( P_VALID(player) && ( 0==what || 1==what) )
	{
		PlayerSession * pSession = player->GetSession  ();
		if( P_VALID( pSession ) )
		{
			if( player->IsOfflineHangOpen() )
				return E_OfflineHang;

			// �����ɫ�պ����ϴ����ߵ��Ǹ�
			//if( player->m_bIsOpenOfflineReward )
			if( g_PlayerAccountMgr.CanReceiveDailyOfflineReward  (pSession->GetSessionID  ()) )
			{
				//tagNS_OfflineExperienceReward msg;
				INT minute	= CalOfflineMinutes(player);
				//INT hour	= minute / 60;
				//minute		= minute % 60;

				//if( /*minute > 0 || */hour > 0 )
				if( minute > 0 )
				{
					//if( minute > 2880 )
					//	minute = 2880;

					INT64 n64FreeExperience	= CalOfflineRewardFreeExp(player,minute);
					//INT nYuanBao	= GetExtraExpOfRole(player);

					INT64 n64Experience	= CalOfflineRewardPayedExp(player,minute);
					
					if( 1 == what )
					{
						INT costed = GetExtraExpOfRole(player,minute);
						if( player->GetCurMgr().GetBagSilver() < costed )
							return E_NotEnoughMoney;

						player->GetCurMgr().DecBagSilver(costed,ELCID_PlayerOfflineReward);
					}

					INT exp = 0 == what ? n64FreeExperience : n64FreeExperience * 2;
					exp1 = exp;
					player->ExpChange((INT64)exp);
					g_PlayerAccountMgr.ReceiveDailyOfflineRewardTime(pSession->GetSessionID  (),g_world.GetWorldTime  ());
					return E_Success;
				}
			}
			return E_RefuseOfflineReward;
		}
	}
	return GT_INVALID;
}



static INT GetExtraExpOfRole(Role * pPlayer, INT nMinute)
{
	//if( P_VALID(pPlayer) )
	//{
	//	INT level = pPlayer->GetLevel  ();
	//	INT minute = 0;
	//	PlayerSession * pSession = pPlayer->GetSession  ();
	//	if( P_VALID(pSession) )
	//	{
	//		DWORD deltaTime = g_PlayerAccountMgr.GetDailyOfflineTime(pSession->GetSessionID  ());//CalcTimeDiff( player->m_LoginTime, player->m_iniLogoutTime );
	//		minute = deltaTime / 60 ;
	//		if( minute > 24 * 60 )
	//			minute = 24 * 60;
	//	}
	//	
	//	if( 1 <= level && 19 >= level )
	//	{
	//		return 5 * ( (double)minute / 1440 );
	//	}
	//	else if( 20 <= level && 39 >= level )
	//	{
	//		return 20 * ( (double)minute / 1440 );
	//	}
	//	else if( 40 <= level && 59 >= level )
	//	{
	//		return 50 * ( (double)minute / 1440 );
	//	}
	//	else 
	//		return 80 * ( (double)minute / 1440 );
	//}
	//return 0;

	if(!P_VALID(pPlayer))
		return 0;

	INT nLevel = pPlayer->GetLevel();

	const tagExpOfflineProto* pExpOfflineProto = g_attRes.GetExpOfflineProto(nLevel);
	if(!P_VALID(pExpOfflineProto))
		return 0;

/*
����Ľ�ǮΪ����ʱ��Ϊ��
С�ڵ���720���ӵ���ң���������Ϊ����ֵ��exp_offline.xml�е�gold��* 1
С�ڵ���1440���ӵ���ң���������Ϊ����ֵ* 2
С�ڵ���2160���ӵ���ң���������Ϊ����ֵ* 3
����2160���ӵ���ң���������Ϊ����ֵ* 4
*/
	INT nBei = 1;
	if(nMinute > 2160)
	{
		nBei = 4;
	}
	else
	{
		if( nMinute > 1440 )
		{
			nBei = 3;
		}
		else
		{
			if( nMinute > 720 )
			{
				nBei = 2;
			}
			else
			{
				nBei = 1;
			}
		}
	}

	return pExpOfflineProto->dwGold * nBei;

}

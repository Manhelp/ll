#include "stdafx.h"
#include "RoleFloatTipsFrame.h"
#include ".\RoleMgr.h"
#include ".\LocalPlayer.h"
#include "WorldFrame.h"
#include "CombatEvent.h"
#include "RoleEvent.h"
#include "SkillMgr.h"
#include "..\WorldDefine\msg_talent.h"
#include "TeamEvent.h"
#include "GameSet.h"
#include "SkillProtoData.h"
#include "ReputeMgr.h"
#include "..\WorldDefine\msg_pet_skill.h"
#include ".\Pet.h"
#include "PetProtoData.h"

const float DelayTime = 0.5f;				//ͣ���೤ʱ���ʼ����
const float BombTime = 0.2f;				//������ըЧ��ʱ��
const int nInfoRaisePixel = 250;			//����������ٸ�����

class RoleFloatTipSortFunc
{
public:
	bool operator() (const tagFloatInfo& tip1,const tagFloatInfo& tip2)
	{
		return tip1.srcPos.z > tip2.srcPos.z;
	}
};

RoleFloatTipsFrame::RoleFloatTipsFrame(void):m_Trunk(this)
{
	m_pFont				= NULL;
	m_nMP				= GT_INVALID;
	m_nHP               = GT_INVALID;
	m_bHPPer10			= TRUE;
	m_bMPPer10			= TRUE;
}

RoleFloatTipsFrame::~RoleFloatTipsFrame(void)
{

}

BOOL RoleFloatTipsFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	
	m_pMgr->RegisterEventHandle( _T("tagShowHPChangeEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnShowHPChangeEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdateLPLevelExp"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateLPLevelExp));
	m_pMgr->RegisterEventHandle( _T("UpdataBelief"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdataBelief));
	m_pMgr->RegisterEventHandle( _T("UpdateShenHunValue"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateShenHunValue) );
	m_pMgr->RegisterEventHandle( _T("UpdateMohHunValue"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateMohHunValue));
	m_pMgr->RegisterEventHandle( _T("tagLRoleCombatState"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnLRoleCombatState));
	m_pMgr->RegisterEventHandle( _T("tagAddTeammateEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnAddTeammateEvent));
	m_pMgr->RegisterEventHandle( _T("tagLeaveTeamEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnLeaveTeamEvent));
	m_pMgr->RegisterEventHandle( _T("tagPctNExpAchieve"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnPctNExpAchieve));
	m_pMgr->RegisterEventHandle( _T("ReputeChange"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnReputeChange));
	m_pMgr->RegisterEventHandle( _T("tagReputeLvlChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnReputeLvlChange));
	m_pMgr->RegisterEventHandle( _T("tagUpdateLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateLPAttEvent));
	m_pMgr->RegisterEventHandle( _T("UpdateLPLiLianEvent"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateLPLiLianEvent));


	m_pCmdMgr->Register("NS_Skill",			(NETMSGPROC)m_Trunk.sfp2(&RoleFloatTipsFrame::OnNS_Skill), _T("NS_Skill"));
	m_pCmdMgr->Register("NS_LearnSkill",	(NETMSGPROC)m_Trunk.sfp2(&RoleFloatTipsFrame::OnNS_LearnSkill), _T("NS_LearnSkill"));
	m_pCmdMgr->Register("NS_PetSkillUse",   (NETMSGPROC)m_Trunk.sfp2(&RoleFloatTipsFrame::OnNS_PetSkillUse),   _T("NS_PetSkillUse"));

	m_pFont = Device()->NewDepthFont();
	m_pFont->Create(12, 25, 0, FALSE, g_StrTable[_T("Font_Hanyi")]);

	return TRUE;
}

BOOL RoleFloatTipsFrame::Destroy()
{
	for(vector<tagFloatInfo>::iterator iter=m_vFloatInfoArray.begin();
		iter!=m_vFloatInfoArray.end();)
	{
		iter = m_vFloatInfoArray.erase(iter);
	}
	m_pMgr->UnRegisterEventHandler( _T("tagShowHPChangeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnShowHPChangeEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPLevelExp"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateLPLevelExp));
	m_pMgr->UnRegisterEventHandler( _T("UpdataBelief"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdataBelief));
	m_pMgr->UnRegisterEventHandler( _T("UpdateShenHunValue"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateShenHunValue));
	m_pMgr->UnRegisterEventHandler( _T("UpdateMohHunValue"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateMohHunValue));
	m_pMgr->UnRegisterEventHandler( _T("tagLRoleCombatState"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnLRoleCombatState));
	m_pMgr->UnRegisterEventHandler( _T("tagAddTeammateEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnAddTeammateEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagLeaveTeamEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnLeaveTeamEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagPctNExpAchieve"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnPctNExpAchieve));
	m_pMgr->UnRegisterEventHandler( _T("ReputeChange"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnReputeChange));
	m_pMgr->UnRegisterEventHandler( _T("tagReputeLvlChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnReputeLvlChange));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPAttEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateLPAttEvent));
	m_pMgr->UnRegisterEventHandler( _T("UpdateLPLiLianEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RoleFloatTipsFrame::OnUpdateLPLiLianEvent));


	m_pCmdMgr->UnRegister("NS_Skill",		(NETMSGPROC)m_Trunk.sfp2(&RoleFloatTipsFrame::OnNS_Skill));
	m_pCmdMgr->UnRegister("NS_LearnSkill",	(NETMSGPROC)m_Trunk.sfp2(&RoleFloatTipsFrame::OnNS_LearnSkill));
	m_pCmdMgr->UnRegister("NS_PetSkillUse", (NETMSGPROC)m_Trunk.sfp2(&RoleFloatTipsFrame::OnNS_PetSkillUse));
	
	m_AttackOwnPicFont.Destroy();
	m_AttackOtherPicFont.Destroy();
	m_AttackHolyPicFont.Destroy();
	SAFE_DELETE(m_pFont);
	return GameFrame::Destroy();
}

VOID RoleFloatTipsFrame::OnEvent(tagGameEvent* pEvent)
{

}

DWORD RoleFloatTipsFrame::OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent)
{
	DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();
	
	if( pEvent->eType==ERA_MP && (m_nMP!=GT_INVALID && m_nMP!=0) )
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( P_VALID(pLocalPlayer) ) 
		{
			int nCurMP = pLocalPlayer->GetAttribute( ERA_MP );
			int nChange = nCurMP - m_nMP;
			m_nMP  = nCurMP;
			if( nChange!=0 )
				PushMPInfo( dwLocalID, nChange);
		}
	}
	else if( pEvent->eType==ERA_MP )
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( P_VALID(pLocalPlayer) ) 
			m_nMP = pLocalPlayer->GetAttribute( ERA_MP );
		else 
			m_nMP = GT_INVALID;
	}

	//------------------------------------------------------------------
	if( pEvent->eType==ERA_HP && (m_nHP!=GT_INVALID && m_nHP!=0) )
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( P_VALID(pLocalPlayer) ) 
		{
			int nCurHP = pLocalPlayer->GetAttribute( ERA_HP );
			int nChange = nCurHP - m_nHP;
			m_nHP  = nCurHP;
			if( nChange>0 )
				PushHPInfo( dwLocalID, nChange);
		}
	}
	else if( pEvent->eType==ERA_HP )
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( P_VALID(pLocalPlayer) ) 
			m_nHP = pLocalPlayer->GetAttribute( ERA_HP );
		else 
			m_nHP = GT_INVALID;
	}
	//-------------------------------------------------------------------

	if( pEvent->eType==ERA_HP || pEvent->eType==ERA_MP )
		OnHPorMpExcessPer10();
	return 0;
}

DWORD RoleFloatTipsFrame::OnShowHPChangeEvent(tagShowHPChangeEvent* pEvent)
{
	DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();
	if(pEvent->dwRoleID==dwLocalID || pEvent->dwSrcRoleID==dwLocalID)
	{
		PushHPInfo(pEvent->dwRoleID, pEvent->nHPChange, pEvent->nFairyElementDamage, pEvent->bMiss, pEvent->bCrit, pEvent->bBlocked,pEvent->bEleCrit, pEvent->eCause);
	}
	return 0;
}

DWORD RoleFloatTipsFrame::OnUpdateLPLevelExp(tagUpdateLPLevelExp* pEvent)
{
	PushUpdateLPLevelExp(pEvent->nExpInc,pEvent->bShare);
	return 0;
}

DWORD RoleFloatTipsFrame::OnUpdataBelief(tagPctNExpAchieve* pEvent)
{
	if( pEvent->nPctMultiplied>0 )
	{
		tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
		hpInfo.eTypeChange = EChange_Belief;
		_stprintf(hpInfo.szInfo, _T("%d %s"), pEvent->nPctMultiplied, g_StrTable[_T("ShowFloatTips_Belief")]);
		CreateFloatInfo(hpInfo);
	}
// 	else if( nExpInc<0 )
// 	{
// 		tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
// 		hpInfo.eTypeChange = EChange_Exp;
// 		_stprintf(hpInfo.szInfo, _T("%d %s"), nExpInc, g_StrTable[_T("ShowFloatTips_Exp")]);
// 		CreateFloatInfo(hpInfo);
// 	}
	
	return 0;
}

DWORD RoleFloatTipsFrame::OnUpdateShenHunValue(tagPctNExpAchieve* pEvent)
{
	tagFloatInfo ShenmoInfo(RoleMgr::Inst()->GetLocalPlayerID());
	ShenmoInfo.eTypeChange = EChange_Shen;
	_stprintf(ShenmoInfo.szInfo, _T("%d %s"), pEvent->nPctMultiplied, g_StrTable[_T("ShowFloatTips_Shenhun")]);
	CreateFloatInfo(ShenmoInfo);
	
	return 0;
}

DWORD RoleFloatTipsFrame::OnUpdateMohHunValue(tagPctNExpAchieve* pEvent)
{
	tagFloatInfo ShenmoInfo(RoleMgr::Inst()->GetLocalPlayerID());
	ShenmoInfo.eTypeChange = EChange_Mo;
	_stprintf(ShenmoInfo.szInfo, _T("%d %s"), pEvent->nPctMultiplied, g_StrTable[_T("ShowFloatTips_Mohun")]);
	CreateFloatInfo(ShenmoInfo);

	return 0;
}

DWORD RoleFloatTipsFrame::OnUpdateLPLiLianEvent(tagPctNExpAchieve* pEvent)
{
	if( pEvent->nPctMultiplied>0 )
	{
		tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
		hpInfo.eTypeChange = EChange_LiLian;
		_stprintf(hpInfo.szInfo, _T("%d %s"), pEvent->nPctMultiplied, g_StrTable[_T("ShowFloatTips_LiLian")]);
		CreateFloatInfo(hpInfo);
	}

	return 0;
}

DWORD RoleFloatTipsFrame::OnPctNExpAchieve(tagPctNExpAchieve* pEvent)
{
	tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
	hpInfo.eTypeChange = EChange_NExp;
	_stprintf(hpInfo.szInfo, _T("%d%s"), pEvent->nPctMultiplied, g_StrTable[_T("ShowFloatTips_NExp")]);
	CreateFloatInfo(hpInfo);
	return 0;
}

DWORD RoleFloatTipsFrame::OnLRoleCombatState(tagLRoleCombatState* pEvent)
{	
	tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
	if(pEvent->bEnterCombat)
	{
		hpInfo.eTypeChange = EChange_EnterCombat;
		_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_EnterCombat")]);
	}
	else 
	{
		hpInfo.eTypeChange = EChange_ExitCombat;
		_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_ExitCombat")]);
	}
	CreateFloatInfo(hpInfo);
	return 0;
}

DWORD RoleFloatTipsFrame::OnAddTeammateEvent(tagAddTeammateEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		tagFloatInfo hpInfo(pEvent->dwRoleID);
		hpInfo.eTypeChange = EChange_EnterTeam;
		_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_EnterTeam")]);
		CreateFloatInfo(hpInfo);
	}
	return 0;
}

DWORD RoleFloatTipsFrame::OnLeaveTeamEvent(tagLeaveTeamEvent* pEvent)
{
	tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
	hpInfo.eTypeChange = EChange_ExitTeam;
	_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_ExitTeam")]);
	CreateFloatInfo(hpInfo);
	return 0;
}

DWORD RoleFloatTipsFrame::OnReputeChange(tagReputeChange* pEvent)
{
	//�����ı仯
	tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
	hpInfo.eTypeChange = EChange_Reputation;
	TCHAR szBuff[X_SHORT_NAME] = {0};
	_sntprintf( szBuff, X_SHORT_NAME, _T("QuestMain_RepName2%d"), pEvent->eClanType );
	if( pEvent->nChange >= 0 )
	{
		_stprintf(hpInfo.szInfo, _T("+%d%s%s"), pEvent->nChange, g_StrTable[szBuff], g_StrTable[_T("Reputation")]);
	}
	else 
	{
		_stprintf(hpInfo.szInfo, _T("%d%s%s"), pEvent->nChange, g_StrTable[szBuff], g_StrTable[_T("Reputation")]);
	}
	
	CreateFloatInfo(hpInfo);
	return 0;
}

DWORD RoleFloatTipsFrame::OnReputeLvlChange(tagReputeLvlChange* pEvent)
{
	//�����ȼ��仯
	tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
	hpInfo.eTypeChange = EChange_ReputaLel;
	TCHAR szRepuLev[X_SHORT_NAME] = {0};
	TCHAR szRepu[X_SHORT_NAME] = {0};
	_sntprintf( szRepu, X_SHORT_NAME, _T("QuestMain_RepName2%d"), pEvent->eClanType );
	_sntprintf( szRepuLev, X_SHORT_NAME, _T("ReputeLevel%d"), pEvent->eLevel );
	_stprintf(hpInfo.szInfo, _T("%s%s%s"), g_StrTable[szRepu], g_StrTable[_T("ReputeIs")], g_StrTable[szRepuLev]);
	CreateFloatInfo(hpInfo);
	return 0;
}

DWORD RoleFloatTipsFrame::OnNS_LearnSkill(tagNS_LearnSkill* pMsg, DWORD dwParam)
{
	if(pMsg->dwErrorCode==E_Success)
	{
		PushStudySkill(pMsg->dwSkillID);
	}
	return 0;
}

DWORD RoleFloatTipsFrame::OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam)
{
	if( GameSet::Inst()->Get(EGS_ShowSkillNameFloatTip) )
	{
		DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();
		if(pMsg->dwErrorCode!=E_Success)
			return 0; 
		if(pMsg->dwSrcRoleID==dwLocalID)
			PushLocalRoleUseSkill(pMsg->dwSkillID);
	}
	return 0;
}

VOID RoleFloatTipsFrame::Update()
{
	
}

VOID RoleFloatTipsFrame::Render3D()
{
	DrawFloatInfo();
}

void RoleFloatTipsFrame::DrawFloatInfo()
{
	float nowTime = Kernel::Inst()->GetAccumTime();
	ClientCamera* pCamera = GetWorldFrm()->GetCamera();
	
	for(vector<tagFloatInfo>::iterator iter=m_vFloatInfoArray.begin();
		iter!=m_vFloatInfoArray.end();)
	{
		tagFloatInfo& info = (*iter);
		Role *pRole = RoleMgr::Inst()->FindRole(info.roleID,false,true);
		tagPoint ptView;
		if(nowTime-info.startTime>info.stayTime || pRole==NULL )
		{
			iter = m_vFloatInfoArray.erase(iter);
		}
		else
		{	
			if( !info.bFrist )
			{
				pRole->GetHeadPos(info.worldPos);	
				info.bFrist = true;
			}

			info.pt = pCamera->WorldToScreen(info.worldPos);
			info.srcPos = pCamera->WorldToScreenF(info.worldPos);
			info.srcPos.z /= 3;
			switch( info.eTypeChange )
			{
			case EChange_NExp:
				info.pt.y += 75;
				break;
			case EChange_ReputaLel:
				info.pt.y -= 40;
				break;
			case EChange_Reputation:
				info.pt.y -= 20;
				break;
			}
			++iter;
		}
	}

	//����Zֵ����
	RoleFloatTipSortFunc sortFunc;
	std::sort(m_vFloatInfoArray.begin(), m_vFloatInfoArray.end(), sortFunc);

	//���λ���
	int clientW=Device()->GetClientWidth();
	int clientH=Device()->GetClientHeight();

	IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);
	for(vector<tagFloatInfo>::iterator iter=m_vFloatInfoArray.begin(); 
		iter!=m_vFloatInfoArray.end(); ++iter)
	{
		tagFloatInfo& info=(*iter);
		info.pt.y -= 25;
		if( (IS_PLAYER(info.roleID) || IS_CREATURE(info.roleID)) )
			info.pt.y -= 30;
		
		//��ֵ
		float fRaiseR = 0.0f;
		float fZoom = 0.8f;
		if( info.eTypeChange == EChange_HPDec && info.roleID != RoleMgr::Inst()->GetLocalPlayerID() )
		{
			if((nowTime-info.startTime) > BombTime)
			{
				fRaiseR = (nowTime-info.startTime) / info.stayTime;	
				info.fontColor.A = BYTE(255*(1.2f-fRaiseR));
				float fZoom = (nowTime-info.startTime)*0.8f/ BombTime;	
				info.pt.y -= (INT)((fZoom - 1.0f) * info.nHeight / 2);
			}
		}
		else
		{
			fZoom = 0.6f;
			if((nowTime-info.startTime) > BombTime)
			{
				fRaiseR = (nowTime-info.startTime) / info.stayTime;	
			}
			else
			{
				fZoom = (nowTime-info.startTime)*0.6f / BombTime;	
				info.pt.x -= (INT)((fZoom - 1.0f) * info.nWidth / 2);
				info.pt.y -= (INT)((fZoom - 1.0f) * info.nHeight / 2);
			}

			//0.5��󵭳�
			if( (nowTime-info.startTime) > DelayTime )
			{
				info.fontColor.A = BYTE(255*(1.2f-fRaiseR));
			}
		}
		
		fRaiseR = CosLerp(0.0f, 1.0f, fRaiseR);
		CalFontPos(info, (int(nInfoRaisePixel * fRaiseR)), info.pt);
	
		switch( info.eTypeChange )
		{
		case EChange_HPInc:
		case EChange_HPDec:
		case EChange_MPInc:
		case EChange_MPDec:
		case EChange_Miss:
		case EChange_EleCrit://add by xp
			{
				if( info.roleID == RoleMgr::Inst()->GetLocalPlayer()->GetID() )
				{
					m_AttackOwnPicFont.DrawString(info.szInfo,info.pt.x,info.pt.y,info.srcPos.z,info.fontColor,-1,1,fZoom);
				}
				else
				{
					if ( ERHPCC_HolySoulDamage == info.eCause )
						m_AttackHolyPicFont.DrawString(info.szInfo,info.pt.x,info.pt.y,info.srcPos.z,info.fontColor,-1,1,fZoom);
					else
						m_AttackOtherPicFont.DrawString(info.szInfo,info.pt.x,info.pt.y,info.srcPos.z,info.fontColor,-1,1,fZoom);	
				}
			}
			break;
		case EChange_Crit:
			{	
				if( info.roleID == RoleMgr::Inst()->GetLocalPlayer()->GetID() )
					m_AttackOwnPicFont.DrawString(info.szInfo,info.pt.x,info.pt.y,info.srcPos.z,info.fontColor,-1,1,1.1f);
				else
				{
					if ( ERHPCC_HolySoulDamage == info.eCause )
						m_AttackHolyPicFont.DrawString(info.szInfo,info.pt.x,info.pt.y,info.srcPos.z,info.fontColor,-1,1,fZoom);
					else
						m_AttackOtherPicFont.DrawString(info.szInfo,info.pt.x,info.pt.y,info.srcPos.z,info.fontColor,-1,1,fZoom);	
				}
			}
			break;
		case EChange_Blocked:			//��
		case EChange_EnterSafe:			//���밲ȫ��
		case EChange_ExitSafe:			//�뿪��ȫ��
		case EChange_Exp:
		case EChange_NExp:
		case EChange_Belief:
		case EChange_Shen:
		case EChange_Mo:
		case EChange_LiLian:
		case EChange_Skill:
		case EChange_StudySkill:
		case EChange_EnterCombat:
		case EChange_ExitCombat:	
		case EChange_EnterTeam:
		case EChange_ExitTeam:
		case EChange_Reputation:
		case EChange_ReputaLel:
		case EChange_HPPer10:
		case EChange_MPPer10:
			{
				m_pFont->Begin();
				RECT rcTitle;
				rcTitle.left = info.pt.x - info.ptTxtSize.x;
				rcTitle.right = info.pt.x + info.ptTxtSize.x;
				rcTitle.top = info.pt.y - info.ptTxtSize.y-8;
				rcTitle.bottom = info.pt.y - 8;

				if(rcTitle.left >=0 && rcTitle.top >= 0
					&& rcTitle.right < clientW && rcTitle.bottom < clientH)
				{
					RECT rect1;
					rect1.top = rcTitle.top-1;
					rect1.bottom = rcTitle.bottom-1;
					rect1.left = rcTitle.left;
					rect1.right = rcTitle.right;
					m_pFont->DrawText(info.szInfo, &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, info.srcPos.z);

					rect1.top = rcTitle.top+1;
					rect1.bottom = rcTitle.bottom+1;
					rect1.left = rcTitle.left;
					rect1.right = rcTitle.right;
					m_pFont->DrawText(info.szInfo, &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, info.srcPos.z);

					rect1.top = rcTitle.top;
					rect1.bottom = rcTitle.bottom;
					rect1.left = rcTitle.left-1;
					rect1.right = rcTitle.right-1;
					m_pFont->DrawText(info.szInfo, &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, info.srcPos.z);

					rect1.top = rcTitle.top;
					rect1.bottom = rcTitle.bottom;
					rect1.left = rcTitle.left+1;
					rect1.right = rcTitle.right+1;
					m_pFont->DrawText(info.szInfo, &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, info.srcPos.z);

					info.fontColor.A = BYTE(255);
					m_pFont->DrawText(info.szInfo, &rcTitle, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, info.fontColor, info.srcPos.z);
				}
				m_pFont->End();				
			}
		}
	}
	IDraw2D::Inst()->EndDraw();
	
}


void RoleFloatTipsFrame::PushHPInfo(DWORD roleID, int damage, INT nFairyDamage /* = 0 */, bool bMiss/* =false */, bool bCrit/* =false */, bool bBlocked/* =false */,bool bEleCrit/* =false */, ERoleHPChangeCause eDamage /*=ERHPCC_SkillDamage*/)
{
	//--����ͷ��������
	tagFloatInfo hpInfo(roleID);
	if(damage > 0)															//��Ѫ
	{
		hpInfo.eTypeChange = EChange_HPInc;		
		TCHAR szBuff[64]={0};
		_itot(damage, szBuff, 10);
		tstringstream strBuff;
		strBuff << _T("+") << szBuff;
		_stprintf(hpInfo.szInfo, _T("%s"), strBuff.str().c_str());
	}
	else if( damage < 0)													//��Ѫ
	{
		
		TCHAR szBuff[64]={0};
		_itot(damage, szBuff, 10);
		tstringstream strBuff;
		strBuff << szBuff;
		if(bCrit)
		{
			hpInfo.eTypeChange = EChange_Crit;	
			_stprintf(hpInfo.szInfo, _T("%s%s"),  g_StrTable[_T("ShowFloatTips_Crit")], strBuff.str().c_str());
		}
		else if(bEleCrit)//add by xpԪ�ر���
		{
			hpInfo.eTypeChange = EChange_EleCrit;	
			_stprintf(hpInfo.szInfo, _T("%s%s"),  g_StrTable[_T("ShowFloatTips_EleCrit")], strBuff.str().c_str());
		}//add end
		else if( nFairyDamage >= 10 )
		{
			hpInfo.eTypeChange = EChange_HPDec;	
			_stprintf(hpInfo.szInfo, _T("%s%s"),  g_StrTable[_T("ShowFloatTips_FairyAtt")], strBuff.str().c_str());
		}
		else if( nFairyDamage <= -10 )
		{
			hpInfo.eTypeChange = EChange_HPDec;	
			_stprintf(hpInfo.szInfo, _T("%s%s"),  g_StrTable[_T("ShowFloatTips_FairyDef")], strBuff.str().c_str());
		}
		else
		{
			hpInfo.eTypeChange = EChange_HPDec;	
			_stprintf(hpInfo.szInfo, _T("%s"), strBuff.str().c_str());
		}



		//2010-08-12  Alex ��¼���ﱻ���е�ʱ��
		Role* pRole=RoleMgr::Inst()->FindRole(roleID);
		if(P_VALID(pRole)){
			pRole->SetMulColor(2.0f,0.0f,0.0f);
			pRole->SetLastHitedTime(timeGetTime());			
		} 


	}
	else if( damage==0 )
	{
		if(bMiss)
		{
			hpInfo.eTypeChange = EChange_Miss;	
			_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_Miss")]);
		}
		else if(bBlocked)
		{
			hpInfo.eTypeChange = EChange_Blocked;
			_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_Blocked")]);
		}
	}

	if( hpInfo.eTypeChange == EChange_Blocked )
	{	
		CreateFloatInfo(hpInfo);
		return;
	}
	//�����ֲ���ʱ��λ��
	CalCreateFontPos(hpInfo);
	//�����ֵ���ɫ
	CalFontColor(hpInfo.roleID, hpInfo.eTypeChange, hpInfo.fontColor);

	hpInfo.eCause = eDamage;
	if ( ERHPCC_HolySoulDamage == eDamage )
	{	//ֻ���Լ���Ԫ��Ż�����
		m_AttackHolyPicFont.GetDrawStringSize(hpInfo.szInfo, hpInfo.nWidth, hpInfo.nHeight);
	}
	else
	{
		m_AttackOwnPicFont.GetDrawStringSize(hpInfo.szInfo, hpInfo.nWidth, hpInfo.nHeight);
	}

	//����
	m_vFloatInfoArray.push_back(hpInfo);
	
}

void RoleFloatTipsFrame::PushMPInfo(DWORD roleID, int damage)
{
	//--����ͷ��������
	tagFloatInfo hpInfo(roleID);
	if(damage > 0)															//��MP
	{
		hpInfo.eTypeChange = EChange_MPInc;		
		TCHAR szBuff[64]={0};
		_itot(damage, szBuff, 10);
		tstringstream strBuff;
		strBuff << _T("+") << szBuff;
		_stprintf(hpInfo.szInfo, _T("%s"), strBuff.str().c_str());
	}
	else if( damage < 0)													//��MP
	{
		hpInfo.eTypeChange = EChange_MPDec;	
		TCHAR szBuff[64]={0};
		_itot(damage, szBuff, 10);
		tstringstream strBuff;
		strBuff << szBuff;
		_stprintf(hpInfo.szInfo, _T("%s"), strBuff.str().c_str());
	}

	//�����ֲ���ʱ��λ��
	CalCreateFontPos(hpInfo);
	//�����ֵ���ɫ
	CalFontColor(hpInfo.roleID, hpInfo.eTypeChange, hpInfo.fontColor);
	m_AttackOwnPicFont.GetDrawStringSize(hpInfo.szInfo, hpInfo.nWidth, hpInfo.nHeight);
	//����
	m_vFloatInfoArray.push_back(hpInfo);

}

void RoleFloatTipsFrame::PushEnterOrExitSafe(DWORD roleID, bool bEnter)
{
	tagFloatInfo hpInfo(roleID);
	if(bEnter)
	{
		hpInfo.eTypeChange = EChange_EnterSafe;		
		_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_EnterSafe")]);
	}
	else
	{
		hpInfo.eTypeChange = EChange_ExitSafe;		
		_stprintf(hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_ExitSafe")]);
	}
	CreateFloatInfo(hpInfo);
}

void RoleFloatTipsFrame::PushUpdateLPLevelExp(INT nExpInc,bool bShare)
{
	if( nExpInc>0 )
	{
		tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
		hpInfo.eTypeChange = EChange_Exp;
		if(bShare)
			_stprintf(hpInfo.szInfo, _T("%d %s"), nExpInc, g_StrTable[_T("ShowFloatTips_ShareExp")]);
		else
			_stprintf(hpInfo.szInfo, _T("%d %s"), nExpInc, g_StrTable[_T("ShowFloatTips_Exp")]);
		CreateFloatInfo(hpInfo);
	}
	else if( nExpInc<0 )
	{
		tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
		hpInfo.eTypeChange = EChange_Exp;
		if(bShare)
			_stprintf(hpInfo.szInfo, _T("%d %s"), nExpInc, g_StrTable[_T("ShowFloatTips_ShareExp")]);
		else
			_stprintf(hpInfo.szInfo, _T("%d %s"), nExpInc, g_StrTable[_T("ShowFloatTips_Exp")]);
		CreateFloatInfo(hpInfo);
	}
}

void RoleFloatTipsFrame::PushLocalRoleUseSkill(DWORD dwSkill)
{
	//��ͨ��������ʾ
	DWORD dwSkillID = MTransSkillID(dwSkill);
	if( IsNormalAttackSkill(dwSkillID) )
		return;

	//���������Լ�����˵�����صļ��ܲ���ʾ
	const tagSkillProtoClient* pSkillData = SkillProtoData::Inst()->FindSkillProto(dwSkill);
	if( !P_VALID(pSkillData) )
		return;
	if( pSkillData->nType2 == ESSTE_Produce )
		return;
	if( pSkillData->nType3 == ESSTE2_Hidden )
		return;

	const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillDataByTypeID(dwSkill);
	if( !P_VALID(pSkill) )
		return;

	tagFloatInfo skillInfo(RoleMgr::Inst()->GetLocalPlayerID());
	skillInfo.eTypeChange = EChange_Skill;
	_stprintf(skillInfo.szInfo, _T("%s"), pSkill->pProto->szName);
	CreateFloatInfo(skillInfo);
}

void RoleFloatTipsFrame::PushStudySkill(DWORD dwSkillID)
{
	const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillDataByTypeID(dwSkillID);
	if( !P_VALID(pSkill) )
		return;
	tagFloatInfo skillInfo(RoleMgr::Inst()->GetLocalPlayerID());
	skillInfo.eTypeChange = EChange_StudySkill;
	_stprintf(skillInfo.szInfo, g_StrTable[_T("LearnSkill_Success_Float")], pSkill->pProto->szName);
	CreateFloatInfo(skillInfo);
}

void RoleFloatTipsFrame::CreateFloatInfo(tagFloatInfo& info)
{
	switch(info.eTypeChange)
	{
	case EChange_Blocked:		
	case EChange_EnterSafe:
	case EChange_ExitSafe:
	case EChange_EnterCombat:
	case EChange_ExitCombat:
	case EChange_EnterTeam:
	case EChange_ExitTeam:
	case EChange_Exp:
        {
            info.stayTime   = 2.0f;
        }
        break;
	case EChange_NExp:
	case EChange_Belief:
	case EChange_Shen:
	case EChange_Mo:
	case EChange_Skill:
	case EChange_StudySkill:
	case EChange_Reputation:
	case EChange_ReputaLel:
	case EChange_HPPer10:
	case EChange_MPPer10:
		{

			tagPoint txtSize;	
			m_pRender->GetTextSize(info.szInfo, NULL, txtSize);
			info.ptTxtSize.x = txtSize.x*2;
			info.ptTxtSize.y = txtSize.y*2;
		}
		break;
	case EChange_LiLian:
		{
			tagPoint txtSize;	
			m_pRender->GetTextSize(info.szInfo, NULL, txtSize);
			info.ptTxtSize.x = txtSize.x*4;
			info.ptTxtSize.y = txtSize.y*4;
		}
		break;
	}
	//�����ֵ���ɫ
	CalFontColor(info.roleID, info.eTypeChange, info.fontColor);
	//�����ֲ���ʱ��λ��
	CalCreateFontPos(info);
	//����
	m_vFloatInfoArray.push_back(info);
}

void RoleFloatTipsFrame::CalCreateFontPos(tagFloatInfo& info)
{
	for(vector<tagFloatInfo>::iterator iter=m_vFloatInfoArray.begin(); 
		iter!=m_vFloatInfoArray.end(); ++iter)
	{
		//��ͬ��ɫ��ͬ���͵�Ʈ����Ϣ����ƫ��ֵ
		if( info.roleID == iter->roleID )
		{
			switch(info.eTypeChange)
			{
			case EChange_HPInc:
			case EChange_HPDec:
			case EChange_MPInc:
			case EChange_MPDec:
			case EChange_Miss:
			case EChange_Crit:
			case EChange_EleCrit://add by xp
				{
					if( RoleMgr::Inst()->GetLocalPlayerID() == info.roleID )
						info.ptOffset.x = m_pUtil->Rand()%(200-150)+50;
					else
						info.ptOffset.x = -(m_pUtil->Rand()%(200-150)+50);
					info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? m_pUtil->Rand()%10 : m_pUtil->Rand()%(80-20)+20); 
				}
				return;
				break;
			case EChange_Blocked:
			case EChange_ReputaLel:
				{
					info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%(20-10)+10)) : m_pUtil->Rand()%(20-10)+10); 
					info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%10)) : m_pUtil->Rand()%50); 
				}
				return;
				break;
			case EChange_Exp:
			case EChange_NExp:
			case EChange_Belief:
			case EChange_Shen:
			case EChange_Mo:
			case EChange_LiLian:
			case EChange_EnterSafe:
			case EChange_ExitSafe:
			case EChange_EnterCombat:
			case EChange_ExitCombat:
			case EChange_EnterTeam:
			case EChange_ExitTeam:
			case EChange_Reputation:
				{
					info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%5)) : m_pUtil->Rand()%5); 
					info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%10)) : m_pUtil->Rand()%50); 
				}
				return;
				break;
			case EChange_Skill:
			case EChange_StudySkill:	
			case EChange_HPPer10:
			case EChange_MPPer10:
				{
					info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%(150-120)+120)) : m_pUtil->Rand()%(150-120)+120); 
					info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%20)) : m_pUtil->Rand()%60); 
				}
				return;
				break;
			default:
				{
					info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%120)) : m_pUtil->Rand()%120); 
					info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%10)) : m_pUtil->Rand()%50); 
				}
				return;
				break;
			}
		}
	}

	switch(info.eTypeChange)
	{
	case EChange_HPInc:
	case EChange_HPDec:
	case EChange_MPInc:
	case EChange_MPDec:
	case EChange_Miss:
	case EChange_Crit:
	case EChange_EleCrit://add by xp
		{
			if( RoleMgr::Inst()->GetLocalPlayerID() == info.roleID )
				info.ptOffset.x = m_pUtil->Rand()%(200-150)+50;
			else
				info.ptOffset.x = -(m_pUtil->Rand()%(200-150)+50);
			info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? m_pUtil->Rand()%10 : m_pUtil->Rand()%(80-20)+20); 
		}
		break;
	case EChange_Blocked:
	case EChange_ReputaLel:
		{
			info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%(20-10)+10)) : m_pUtil->Rand()%(20-10)+10); 
			info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%10)) : m_pUtil->Rand()%50); 
		}
		break;
	case EChange_Exp:
	case EChange_NExp:
	case EChange_Belief:
	case EChange_Shen:
	case EChange_Mo:
	case EChange_LiLian:
	case EChange_EnterSafe:
	case EChange_ExitSafe:
	case EChange_EnterCombat:
	case EChange_ExitCombat:
	case EChange_EnterTeam:
	case EChange_ExitTeam:
	case EChange_Reputation:
		{
			info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%5)) : m_pUtil->Rand()%5); 
			info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%10)) : m_pUtil->Rand()%50); 
		}
		break;
	case EChange_Skill:
	case EChange_StudySkill:	
	case EChange_HPPer10:
	case EChange_MPPer10:
		{
			info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%(150-120)+120)) : m_pUtil->Rand()%(150-120)+120); 
			info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%20)) : m_pUtil->Rand()%60); 
		}
		break;
	default:
		{
			info.ptOffset.x = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%120)) : m_pUtil->Rand()%120); 
			info.ptOffset.y = (((m_pUtil->Rand()%100)>50) ? (-(m_pUtil->Rand()%10)) : m_pUtil->Rand()%50); 
		}
		break;
	}
}

void RoleFloatTipsFrame::CalFontPos(const tagFloatInfo& info,int nChange, POINT& pt )
{
	pt.x += info.ptOffset.x;
	pt.y += info.ptOffset.y;
}

void RoleFloatTipsFrame::CalFontColor( DWORD dwRoleID, EChangeType eType, Color4ub &fontColor )
{	
	static Color4ub pinkColor = Color4ub(255, 255, 229, 165);			//��ɫ
	static Color4ub redColor = Color4ub(255, 238, 0, 0 );				//��ɫ
	static Color4ub greenColor = Color4ub(255, 0, 182, 94);				//��ɫ
	static Color4ub greenColor1 = Color4ub(255, 17, 235, 0);			//����ɫ
	static Color4ub greenColor2 = Color4ub(255, 198, 255, 0);			//ƻ����ɫ
	static Color4ub blueColor = Color4ub(255, 0, 162, 255);				//��ɫ
	static Color4ub buleColor1 = Color4ub(255, 35, 221, 255);			//����ɫ
	static Color4ub purpleColor = Color4ub(255, 185, 137, 251);			//��ɫ
	static Color4ub whiteColor = Color4ub(255, 255, 247, 224);			//��ɫ
	static Color4ub goldenColor = Color4ub(255, 187, 173, 55);			//��ɫ
	static Color4ub brownColor = Color4ub(255, 233, 174, 33);			//��ɫ
																		//÷��ɫ
	static Color4ub yellowColor = Color4ub(255, 255, 226, 94);			//��ɫ
	static Color4ub yellowColor1 = Color4ub(255, 255, 235, 144);		//����ɫ


	// �������ɫ����
	if(dwRoleID==RoleMgr::Inst()->GetLocalPlayer()->GetID())
	{
		//��ұ���
		switch( eType )
		{
		case EChange_HPDec:
			fontColor = whiteColor;
			break;
		case EChange_EnterCombat:
			fontColor = redColor;
			break;
		case EChange_HPInc:
		case EChange_ExitCombat:
			fontColor = greenColor;
			break;
		case EChange_Crit:
			fontColor = whiteColor;
			break;
		case EChange_EleCrit://add by xp
			fontColor = whiteColor;
			break;//add end
		case EChange_Miss:
			fontColor = whiteColor;
			break;
		case EChange_MPInc:
		case EChange_MPDec:
		case EChange_EnterTeam:
		case EChange_ExitTeam:
			fontColor = blueColor;
			break;
		case EChange_Blocked:
			fontColor = brownColor;
			break;
		case EChange_Exp:
			fontColor = 0xFF79028E;
			break;
		case EChange_NExp:
			fontColor = goldenColor;
			break;
		case EChange_EnterSafe:
			fontColor = Color4ub(255, 0, 255, 0);
			break;
		case EChange_ExitSafe:
			fontColor = Color4ub(255, 255, 0, 0);
			break;
		case EChange_Skill:
		case EChange_StudySkill:
			fontColor = goldenColor;
			break;
		case EChange_Reputation:
			fontColor = brownColor;
			break;
		case EChange_ReputaLel:
			fontColor = greenColor;
			break;
		case EChange_HPPer10:
			fontColor = redColor;
			break;
		case EChange_MPPer10:
			fontColor = blueColor;
			break;
		case EChange_Belief:
			fontColor = greenColor2;
			break;
		case EChange_LiLian:
			fontColor = greenColor2;
			break;
		case EChange_Shen:
			fontColor = greenColor2;
			break;
		case EChange_Mo:
			fontColor = greenColor2;
			break;
		}
	}
	//������һ��
	else if(IS_PLAYER(dwRoleID) || IS_CREATURE(dwRoleID))
	{
		switch( eType )
		{
		case EChange_HPDec:
			fontColor = whiteColor;
			break;
		case EChange_HPInc:
			fontColor = greenColor;
			break;
		case EChange_Crit:
			fontColor = whiteColor;
			break;
		case EChange_EleCrit://add by xp
			fontColor = whiteColor;
			break;//add end
		case EChange_Miss:
			fontColor = whiteColor;
			break;
		case EChange_Blocked:
			fontColor = pinkColor;
			break;
		case EChange_MPInc:
		case EChange_MPDec:
			fontColor = blueColor;
			break;
		}
	}
	//����ǳ���
	else if ( IS_PET (dwRoleID))
	{
		switch( eType )
		{
		case EChange_Skill:
			fontColor = 0xFF009999;
			break;
		}
	}
}

BOOL RoleFloatTipsFrame::ReloadUI()
{
	IFS* pFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");

	m_AttackOtherPicFont.LoadPic( _T('-'), _T("data\\ui\\mark\\battle\\L_yel-minus.bmp"), pFS );
	m_AttackOtherPicFont.LoadPic( _T('+'), _T("data\\ui\\mark\\battle\\L_yel-plus.bmp"), pFS );
	m_AttackOtherPicFont.LoadPic( 'A', _T("data\\ui\\mark\\battle\\L_yel-critical.bmp"), pFS );		//����
	m_AttackOtherPicFont.LoadPic( 'B', _T("data\\ui\\mark\\battle\\L_yel-miss.bmp"), pFS );			//��
	m_AttackOtherPicFont.LoadPic( 'C', _T("data\\ui\\mark\\battle\\SM_damage_1.bmp"), pFS );		// Ԫ�ؼ���
	m_AttackOtherPicFont.LoadPic( 'D', _T("data\\ui\\mark\\battle\\SM_damage_2.bmp"), pFS );		// Ԫ�صֿ�
	m_AttackOtherPicFont.LoadPic( 'E', _T("data\\ui\\mark\\battle\\SM_damage_4.bmp"), pFS );		// Ԫ�ر���add by xp

	m_AttackOwnPicFont.LoadPic( _T('-'), _T("data\\ui\\mark\\battle\\L_red-minus.bmp"), pFS );
	m_AttackOwnPicFont.LoadPic( _T('+'), _T("data\\ui\\mark\\battle\\L_red-plus.bmp"), pFS );
	m_AttackOwnPicFont.LoadPic( 'A', _T("data\\ui\\mark\\battle\\L_red-critical.bmp"), pFS );		//����
	m_AttackOwnPicFont.LoadPic( 'B', _T("data\\ui\\mark\\battle\\L_red-miss.bmp"), pFS );			//��
	m_AttackOwnPicFont.LoadPic( 'C', _T("data\\ui\\mark\\battle\\SM_damage_1.bmp"), pFS );		// Ԫ�ؼ���
	m_AttackOwnPicFont.LoadPic( 'D', _T("data\\ui\\mark\\battle\\SM_damage_2.bmp"), pFS );		// Ԫ�صֿ�
	m_AttackOwnPicFont.LoadPic( 'E', _T("data\\ui\\mark\\battle\\SM_damage_4.bmp"), pFS );		// Ԫ�ر���add by xp

	m_AttackHolyPicFont.LoadPic( _T('-'), _T("data\\ui\\mark\\battle\\kill-minus.bmp"), pFS );
	m_AttackHolyPicFont.LoadPic( _T('+'), _T("data\\ui\\mark\\battle\\kill-plus.bmp"), pFS );
	m_AttackHolyPicFont.LoadPic( 'A', _T("data\\ui\\mark\\battle\\kill-critical.bmp"), pFS );		//����
	m_AttackHolyPicFont.LoadPic( 'B', _T("data\\ui\\mark\\battle\\kill-miss.bmp"), pFS );			//��

	TCHAR szTemp[_MAX_PATH]={0};
	for (int i=0; i<10; i++)
	{
		_sntprintf( szTemp, _MAX_PATH, _T("data\\ui\\mark\\battle\\L_yel-%c.bmp"), i+_T('0') );
		m_AttackOtherPicFont.LoadPic( TCHAR(i+_T('0')), szTemp, pFS );

		_sntprintf( szTemp, _MAX_PATH, _T("data\\ui\\mark\\battle\\L_red-%c.bmp"), i+_T('0') );
		m_AttackOwnPicFont.LoadPic( TCHAR(i+_T('0')), szTemp, pFS );

		_sntprintf( szTemp, _MAX_PATH, _T("data\\ui\\mark\\battle\\kill-%c.bmp"), i+_T('0') );
		m_AttackHolyPicFont.LoadPic( TCHAR(i+_T('0')), szTemp, pFS );
	}
	return TRUE;
}

void RoleFloatTipsFrame::OnHPorMpExcessPer10()
{
	LocalPlayer* pLP = NULL;
	pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pLP) )
	{
		//�ж�����
		const int nMaxHp	= pLP->GetAttribute( ERA_MaxHP );
		const int nHp		= pLP->GetAttribute( ERA_HP );
		if( (nHp!=GT_INVALID && nMaxHp!=GT_INVALID) && nHp<nMaxHp*0.2 && !m_bHPPer10 )
		{
			m_bHPPer10 = TRUE;
			tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
			hpInfo.eTypeChange = EChange_HPPer10;
			_stprintf( hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_HpExcess10")] );
			CreateFloatInfo(hpInfo);
		}
		else if( (nHp!=GT_INVALID && nMaxHp!=GT_INVALID) && nHp>nMaxHp*0.2 && m_bHPPer10 )
		{
			m_bHPPer10 = FALSE;
		}

		//�ж�����
		const int nMaxMp	= pLP->GetAttribute( ERA_MaxMP );
		const int nMp		= pLP->GetAttribute( ERA_MP );
		if( (nMp!=GT_INVALID && nMaxMp!=GT_INVALID) && nMp<nMaxMp*0.2 && !m_bMPPer10 )
		{
			m_bMPPer10 = TRUE;
			tagFloatInfo hpInfo(RoleMgr::Inst()->GetLocalPlayerID());
			hpInfo.eTypeChange = EChange_MPPer10;
			_stprintf( hpInfo.szInfo, _T("%s"), g_StrTable[_T("ShowFloatTips_MpExcess10")]);
			CreateFloatInfo(hpInfo);
		}
		else if( (nMp!=GT_INVALID && nMaxMp!=GT_INVALID) && nMp>nMaxMp*0.2 && m_bMPPer10 )
		{
			m_bMPPer10 = FALSE;
		}
	}
}

void RoleFloatTipsFrame::PushPetSkill(DWORD dwPetID,DWORD dwSkillID)
{
	const tagPetSkillData* pSkillData = PetProtoData::Inst()->FindPetSkill(dwSkillID);
	if (!P_VALID(pSkillData))
		return;

	tagFloatInfo skillInfo(dwPetID);
	skillInfo.eTypeChange = EChange_Skill;
	ZeroMemory( skillInfo.szInfo, sizeof(skillInfo.szInfo) );
	_tcsncpy( skillInfo.szInfo, pSkillData->strName.c_str(), sizeof(skillInfo.szInfo)/sizeof(TCHAR) );
	CreateFloatInfo(skillInfo);

}
DWORD RoleFloatTipsFrame::OnNS_PetSkillUse(tagNS_PetSkillUse* pMsg, DWORD)
{	//���߻�Ҫ��ȥ�����＼�ܵ�����
	//if( GameSet::Inst()->Get(EGS_ShowSkillNameFloatTip) )
	//{
	//	DWORD dwPetID = pMsg->dwPetID;
	//	if(pMsg->dwErrCode!=E_Success)
	//		return 0; 
	//	//��Ϊ���ܵ���PET������skills�����������ת���ǰ�ȫ�ġ�
	//	Pet* nNpc = static_cast<Pet*>(RoleMgr::Inst()->FindRole( dwPetID ));
	//	if (!P_VALID(nNpc))
	//		return 0;
	//	//�ж��ǲ��Ǳ��س���
	//	if( nNpc->IsBelongToLP())
	//	{
	//		PushPetSkill(pMsg->dwPetID,pMsg->dwSkillID);
	//	}
	//}
	return 0;
}
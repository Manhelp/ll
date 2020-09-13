#include "StdAfx.h"
#include "LocalPlayer.h"
#include "FSM_LP.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_pk.h"
#include "..\WorldDefine\msg_combat.h"
#include "RoleEvent.h"
#include "MapMgr.h"
#include "NetSession.h"
#include "PlayerNameTab.h"
#include "LevelUpData.h"
#include "Pet.h"
#include "EffectMgr.h"
#include "ItemProtoData.h"
#include "QuestQuery.h"
#include "QuestMgr.h"
#include "GUIEffectMgr.h"
#include "FamilyMgr.h"
#include "RoleMgr.h"
#include "VirtualRole.h"


IMP_RTTI(LocalPlayer,Player);

LocalPlayer::LocalPlayer(void)
: m_pFSM(0)
, m_rolecurlevelexp(0)
, m_roleStateEx(0)
, m_nSoarSkillLearnTimesToday(0)
, m_bHooking(false)
, bNeedRePathFinding(false)
, dwReFindNPCid(0)
, m_nLPActiveDegree(0)
{
}

LocalPlayer::~LocalPlayer(void)
{
	SAFE_DELETE(m_pFSM);
}

void LocalPlayer::InitFSM()
{
	SAFE_DELETE(m_pFSM);
	m_pFSM=new FSM_LP(this);
	m_pFSM->Init();
}

void LocalPlayer::Update()
{
	//--������Ϣ��
	NodeAnimationCtrl* pAniCtrl = GetAniCtrl();
	HandleMsgCode(pAniCtrl);

	//--update rotater
	m_rotaterYaw.Update();
	m_rotaterPitch.Update();
	m_upperRotater.Update();
	m_headRotater.Update();
	m_scaler.Update();

	//--update FSM
	m_pFSM->Update();

	//--update animation player
	m_aniPlayer.Update();

	//--update world matrix
	if( NULL != m_pAvatarNode )
	{
		if( P_VALID(m_pMount) )//���״̬����Ҫ���¼����������
		{
			if( m_pMount->GetOwnerID() == m_id )//�����״̬
			{
				Matrix4 matRide;
				if( CalRideMat(m_pMount,matRide,0) )
				{
					m_pAvatarNode->SetWorldMat( &matRide );
				}
			}
			else//���״̬
			{
				Matrix4 matRide;
				if( CalRideMat(m_pMount,matRide,1) )
				{
					m_pAvatarNode->SetWorldMat( &matRide );
				}
			}
		}
		else//��ͨ״̬
		{
			m_pAvatarNode->SetWorldMat(m_space.GetMatrix());
		}
	}
	if( IsInState(ES_Dead) == FALSE )
		UpdateTeamBuffer();

	// ����������Ч��
	UpdatePlayerEffect();

	//͸��״̬�»�������װ������Ҫʵʱˢ��
	float alpha = this->GetTransparent();
	if ( alpha < 1.0f )
	{
		this->SetTransparent(alpha); 
	}

	if(m_dwTime != 0)
	{
		if(Kernel::Inst()->GetAccumTimeDW() - m_dwTime >= 5000)
		{
			UpdateAvatar(m_tempAtt, m_tempEp, m_pTempSG);
#ifdef ON_GAMEGUARD
			//��������� 5����һ�����
			if (g_npgl.Check() != NPGAMEMON_SUCCESS) 
			{
				MessageBox(NULL, _T("��⵽��ң������˳�"), _T("nProtect GameGuard"), MB_OK);
				exit(0);
			}
#endif
		}

	}
}

void LocalPlayer::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_SetRoleState"))
	{
		tagNS_SetRoleState* pCmd=(tagNS_SetRoleState*)pNetCmd;
		SetRoleState(pCmd->eState, true);
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_UnSetRoleState"))
	{
		tagNS_UnSetRoleState* pCmd=(tagNS_UnSetRoleState*)pNetCmd;
		SetRoleState(pCmd->eState, false);
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_RolePKStateChange"))
	{
		tagNS_RolePKStateChange* pCmd=(tagNS_RolePKStateChange*)pNetCmd;
		m_ePKLastState = m_ePKState;
		m_ePKState=pCmd->eState;

		tagLPPKState event(_T("tagLPPKState"),NULL);
		event.eState = m_ePKState;
		event.eOldState = m_ePKLastState;
		event.dwRoleID = m_id;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_UseItem"))
	{
		tagNS_UseItem* pCmd=(tagNS_UseItem*)pNetCmd;
		if (pCmd->dwTypeID == 4500028)	// �ᱦ�������typeID
		{
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui018"),pDesktop->GetView(),pDesktop->GetSize());
		}
	}
	m_pFSM->OnNetCmd(pNetCmd);

	VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(GetID());
	if (P_VALID(pVp))
		pVp->OnNetCmd(pNetCmd);

}

void LocalPlayer::OnGameEvent( tagGameEvent* pGameEvent )
{
	m_pFSM->OnGameEvent(pGameEvent);

	VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(GetID());
	if (P_VALID(pVp))
		pVp->OnGameEvent(pGameEvent);

}

bool LocalPlayer::IsReadyState()
{
	return m_pFSM->IsReadyState();
}

bool LocalPlayer::IsStandState()
{
	return m_pFSM->IsStandState();
}

bool LocalPlayer::IsStyleActionState()
{
	return m_pFSM->IsStyleActionState();
}

bool LocalPlayer::IsCombatState()
{
	return m_pFSM->IsCombatState();
}

bool LocalPlayer::IsSkillState()
{
	return m_pFSM->IsSkillState();
}

bool LocalPlayer::IsDead()
{
	return m_pFSM->IsDeadState();
}

bool LocalPlayer::IsSwimState()
{
	return m_pFSM->IsSwimState();
}
bool LocalPlayer::IsPursueState()
{
	return m_pFSM->IsPursueState();
}
bool LocalPlayer::IsMoveState()
{
	return m_pFSM->IsMoveState();
}
bool LocalPlayer::IsOnWaterState()
{
	return m_pFSM->IsOnWaterState();
}
bool LocalPlayer::IsNormalSkillState()
{
	return m_pFSM->IsNormalSkillState();
}
bool LocalPlayer::IsIdleState()
{
	return m_pFSM->IsUpperIdleState()
		&& (m_pFSM->IsStandState()||m_pFSM->IsFloatState()||m_pFSM->IsWaterStandState());
}

VOID LocalPlayer::InitAttribute(tagNS_GetRoleInitState_Att* pCmd)
{
	// ȷ��SeneneNode�Ѿ�����
	ASSERT( P_VALID( m_pAvatarNode ) );

	//// ���ý�ɫID������
	//SetID(pCmd->RoleData.dwID);
	//SetPlayerName(pCmd->RoleData.szRoleName, pCmd->RoleData.dwRoleNameCrc);

	// ���ý�ɫλ�úͳ���
	//Vector3 Pos(pCmd->RoleData.fPos[0], pCmd->RoleData.fPos[1], pCmd->RoleData.fPos[2]);
	//Vector3 FaceTo(pCmd->RoleData.fFaceTo[0], pCmd->RoleData.fFaceTo[1], pCmd->RoleData.fFaceTo[2]);
	//SetPos(Pos);
	//SetYaw(CalcYaw(FaceTo));
	
	// ���ý�ɫ����
	INT i;
	for (i=0; i<ERA_End; ++i)
	{
		SetAttribute((ERoleAttribute)i, pCmd->nAtt[i], true);			
		
		// һ���������¼����Ͷ�ŵ���
		if (ERA_AttA_Start <= i && ERA_AttA_End >= i)
		{
			SetAttPointAdd((ERoleAttribute)i, pCmd->nAttPointAdd[i]);
		}
	}
	
	// ����
	if( pCmd->bHasLeftMsg )
	{
		tagGameEvent evt(_T("tagLeaveMsg"), NULL);
		TObjRef<GameFrameMgr>()->SendEvent(&evt);
	}

	// ���ý�ɫ����
	tstring strName = PlayerNameTab::Inst()->FindNameByID(m_id);
	if(strName != _T(""))
	{
		SetRoleName(strName);
	}

	//// �������ְҵ
	//SetPlayerClass(pCmd->RoleData.eClass, pCmd->RoleData.eClassEx);

	// �������״̬
	SetRoleState(pCmd->dwRoleState);
	SetRoleStateEx(pCmd->dwRoleStateEx);

    // ���ý�ɫ��ǰ�ƺ�
    SetPlayerTitle(pCmd->u16ActTitleID);

    // ����ְҵ
    SetPlayerClass(pCmd->eClassType);
    SetPlayerClassEx(pCmd->eClassTypeEx);

	// ������ְ
	m_dwClergyID = pCmd->dwClergyID;

	// ����̯λ�ȼ�
	SetStallLevel(pCmd->byStallLevel);
	SetMiraclePrcNum(pCmd->n16MiraclePrcNum);

    // ������ż
    SetPlayerLoverID(pCmd->dwLoverID);
    SetPlayerIsHasWedding(pCmd->bHaveWedding);

    // ���ø����
    SetRebornPoint(pCmd->dwRebornMapID, pCmd->vRebornPoint);

    // ���ÿɼ�¼���͵�
    SetTransportPoint(pCmd->dwTransportID, pCmd->vTransportPoint);

	// ����PK״̬
	m_ePKState=pCmd->ePKState;
	m_ePKLastState = m_ePKState;
	tagLPPKState event(_T("tagLPPKState"),NULL);
	event.eState = pCmd->ePKState;
	event.eOldState = m_ePKLastState;
	event.dwRoleID = m_id;
	TObjRef<GameFrameMgr>()->SendEvent(&event);

	// ���ý�ɫ״̬
	m_state.Init(pCmd->dwState);

	// ���ý�ɫ�ȼ�
	SetRoleLevel(pCmd->nLevel);
	SetRoleLevelExp(pCmd->nCurLevelExp);
	//SetRoleFamilyID(pCmd->dwFamilyID);
	// �����ɫ����ʱ��������ֱ�ӽ�������״̬
	if(IsInState(ES_Dead))
	{
		m_pFSM->Change2LowerState_AutoStand();
		m_pFSM->Change2UpperState_Dead(false);
	}

	// �������������Ϣ
	SetRoleGuildID(pCmd->dwGuildID);
	
	// ������Ҽ�����Ϣ
	SetRoleFamilyID(pCmd->dwFamilyID);

	// ���buff�б�
	FreeRoleBuff();
	for(INT j=0; j<pCmd->nBuffNum; ++j)
	{
		AddRoleBuff(pCmd->Buff[j]);
	}

	// ��״̬��α��AddBuff��Ϣ
	tagNS_AddRoleBuff tmpCmd;
	for(i=0; i<pCmd->nBuffNum; ++i)
	{
		tmpCmd.dwRoleID=GetID();
		tmpCmd.Buff=pCmd->Buff[i];

		if( !P_VALID(tmpCmd.Buff.dwBuffTypeID) )
			continue;

		m_pFSM->OnNetCmd(&tmpCmd);
	}

	// ��״̬��α��SetState��Ϣ
	tagNS_SetState setStateCmd;
	setStateCmd.dwRoleID=GetID();
	if(IsInState(ES_Dizzy))
	{
		setStateCmd.eState=ES_Dizzy;
		m_pFSM->OnNetCmd(&setStateCmd);
	}
	if(IsInState(ES_Tie))
	{
		setStateCmd.eState=ES_Tie;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_Spor))
	{
		setStateCmd.eState=ES_Spor;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_Invincible))
	{
		setStateCmd.eState=ES_Invincible;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_Lurk))
	{
		setStateCmd.eState=ES_Lurk;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_DisArm))
	{
		setStateCmd.eState=ES_DisArm;
		m_pFSM->OnNetCmd(&setStateCmd);
	}			
	if(IsInState(ES_NoSkill))
	{
		setStateCmd.eState=ES_NoSkill;
		m_pFSM->OnNetCmd(&setStateCmd);
	}

	// ��������

	m_bAttributeInited = true;

	EffectMgr::Inst()->PlayCacheBuff( m_id );
	FamilyMgr::Inst()->GetFamilyName(pCmd->dwFamilyID);
	tagNC_RoleInitDone cmd;
	cmd.dwRoleID = GetID();
	TObjRef<NetSession>()->Send(&cmd);


	//���̨��xtrap setuserinfoEx
#ifdef ON_X_TRAP
 	TObjRef<VarContainer> pVarContainer;
 	pVarContainer = CreateObj("last_login_server", "VarContainer");
	BOOL bIsSuccess = pVarContainer->Load(NULL, _T("config\\lastlogin.xml"));
 	tstring strSeverName;
	if(bIsSuccess)
	{
		strSeverName = pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
	}
	KillObj("last_login_server");
 
 	TCHAR szTempVal[128] = {0};
 	TCHAR szRoleID[512] = {0};
 	_stprintf(szRoleID, _T("%d"), m_id);
 	TCHAR szServerName[X_LONG_NAME]={0};
 	_stprintf(szServerName, _T("%s"), strSeverName.c_str());
 	TCHAR szName[X_LONG_NAME] = {0};
 	_stprintf(szName, _T("%s"), strName.c_str());
 	TCHAR szClassName[512] = {0};
 	_stprintf(szTempVal, _T("ClassName_%d"), pCmd->eClassType);
 	_stprintf(szClassName, _T("%s"), g_StrTable[szTempVal]);
 	XTrap_C_SetUserInfoEx((LPCSTR)szRoleID, (LPCSTR)szServerName, (LPCSTR)szName, (LPCSTR)szClassName, 0, NULL);
#endif

#ifdef ON_GAMEGUARD
	TCHAR szTextRoleID[52] = {0};
	_stprintf(szTextRoleID, _T("%d"), GetID());
	g_npgl.Send(szTextRoleID);
#endif
}

VOID LocalPlayer::SetAttPointAdd(ERoleAttribute eType, INT nValue)
{
	m_rolepointadd[eType] = nValue;
	
	// ���͸����¼�
	tagUpdateLPAttEvent event(_T("tagUpdateLPBidAttEvent"), NULL);
	event.eType = eType;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

VOID LocalPlayer::SetRoleLevelExp(INT64 nValue, INT64 nInc /* = 0 */, INT nFairyExp /* = 0 */, INT nFairyLvCha /* = 0 */ ,bool bShare/* = false*/)
{
	m_rolecurlevelexp = nValue;

	// ���͸����¼�
	tagUpdateLPLevelExp event(_T("tagUpdateLPLevelExp"), NULL);
	event.nCurLevelExp = nValue;
	event.nExpInc = nInc;
	event.nFairyExp = nFairyExp;
	event.nFairyLvChg = nFairyLvCha;
	event.bShare = bShare;
	
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

void LocalPlayer::OnGotoMap(const Vector3& pos,const Vector3 faceTo)
{
	//--����λ��
	SetPos(pos);

	//--���ó���
	float yaw=CalcYaw(faceTo);
	SetMoveYaw(yaw);
	SetYaw(yaw);
	
	//--״̬���л�״̬
	m_pFSM->Change2LowerState_AutoStand();
	if( !IsDead() )
		m_pFSM->Change2UpperState_Idle();

	//VirtualRole *pVirtualRole = RoleMgr::Inst()->GetVirtualRole(GetID());
	//if( NULL != pVirtualRole && NULL != pVirtualRole->GetSceneNode() )
	//{
	//	pVirtualRole->OnGotoMap(pos,faceTo);
	//	RoleMgr::Inst()->SetVirtualRoleFaceTo(faceTo);
	//}

}

INT LocalPlayer::GetOriginAtt( ERoleAttribute eType )
{
	ASSERT(eType >= ERA_AttA_Start && eType <= ERA_AttA_End);
	if(eType >= ERA_AttA_Start && eType <= ERA_AttA_End)
	{
		const tagExperience* pLevelup = LevelUpData::Inst()->FindExpData(m_nRoleLevel);
		if (P_VALID(pLevelup))
		{
			return pLevelup->nAtt[MTransERAAttA2Index(eType)] + m_rolepointadd[MTransERAAttA2Index(eType)];
		}
	}
	return NULL;
}

void LocalPlayer::SetRoleStateEx( DWORD dwValue )
{
	m_roleStateEx = dwValue;
}

void LocalPlayer::SetRoleStateEx( ERoleStateEx eType, bool bFlag )
{
	if (bFlag)
		m_roleStateEx |= eType;
	else
		m_roleStateEx ^= eType;
}

EActionState LocalPlayer::GetActionState()
{
	return m_pFSM->GetActionState();
}

void LocalPlayer::ReplayLowerAni()
{
	m_pFSM->ReplayLowerAni();
}


void LocalPlayer::DontPlayEaseSkeleton()
{
	if( P_VALID(m_pFSM) )
		m_pFSM->DontPlayEaseSkeleton();
}


void LocalPlayer::ChangeUpperToIdle()
{
	if( P_VALID(m_pFSM) )
		m_pFSM->Change2UpperState_Idle();
}

bool LocalPlayer::IsUpperIdleState()
{
	if( P_VALID(m_pFSM) )
		return m_pFSM->IsUpperIdleState();
	else 
		return false;
}

DWORD LocalPlayer::GetUsingItemTypeID()
{ 
	if( P_VALID(m_pFSM) )
		return m_pFSM->GetUsingItemTypeID();
	else
		return 0;
}

void LocalPlayer::SetMiraclePrcNum(INT16 nNum)
{
	m_n16MiraclePrcNum = nNum;
}

void LocalPlayer::SetSoarSkillLearnTimes(INT16 nNum)
{
	m_nSoarSkillLearnTimesToday = nNum;
}

void LocalPlayer::UpdatePlayerEffect()
{
	if (!m_bAttributeInited)
		return;

	if (m_roledata[ERA_Soar] != 1)
		return;

	m_dwCurrSoarTime += Kernel::Inst()->GetDeltaTimeDW();
	if (m_dwCurrSoarTime > m_dwSoarEffectTime )
	{
		/*EffectMgr::Inst()->PlayEffect(m_space.GetPos(),m_space.GetYaw(),_T("txjn01_3"));*/
		EffectMgr::Inst()->PlayRoleEffect( m_id, _T("txjn01_3") );
		m_dwCurrSoarTime = 0;
	}
}

bool LocalPlayer::IsHookState()
{
	return m_bHooking;
}

void LocalPlayer::SetHookState(bool state)
{
	m_bHooking = state;
}

void LocalPlayer::SetPlayerClass(EClassType eClass)
{
	tagRoleClassChange event(_T("tagRoleClassChange"), NULL);
	event.eOld = GetPlayerClass();

	Player::SetPlayerClass(eClass);

	event.eClass = eClass;
	TObjRef<GameFrameMgr>()->SendEvent(&event);

}

void LocalPlayer::LoadEquipEffect(const tagAvatarEquip& ep)
{

	for (int i = 0; i< X_AVATAR_ELEMENT_NUM; i++)
	{
		const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay( ep.AvatarEquip[i].dwTypeID );
		const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( ep.AvatarEquip[i].dwTypeID );
		if (!P_VALID(pInfo) && !P_VALID(pProto))
			continue;

		tstring strEffectName = EffectMgr::Inst()->GetGlowName( pInfo->szBladeGlowSFX, ep.AvatarEquip[i].byFlareVal );

		if (EAE_Head == ep.AvatarEquip[i].byDisplayPos)
		{	
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 Head") ,EffectMgr::EBET_Head);
		}
		else if (EAE_Upper == ep.AvatarEquip[i].byDisplayPos)
		{
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 Spine2") ,EffectMgr::EBET_Chest);
		}
		else if (EAE_Lower == ep.AvatarEquip[i].byDisplayPos)
		{
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 R Calf") ,EffectMgr::EBET_Leg_R);
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 L Calf") ,EffectMgr::EBET_Leg_L);
		}
		else if (EAE_Glove == ep.AvatarEquip[i].byDisplayPos)
		{
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 R Forearm") ,EffectMgr::EBET_Wrist_R);
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 L Forearm") ,EffectMgr::EBET_Wrist_L);
		}
		else if (EAE_Boot == ep.AvatarEquip[i].byDisplayPos)
		{
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 R Foot") ,EffectMgr::EBET_Foot_R);
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 L Foot") ,EffectMgr::EBET_Foot_L);
		}
		else if (EAE_Back == ep.AvatarEquip[i].byDisplayPos)
		{
			EffectMgr::Inst()->PlayRoleEquipEffect(GetID(),strEffectName.c_str(), _T("Bip01 Spine2") ,EffectMgr::EBET_Mantle);
		}

	}

}

void LocalPlayer::SetRePathFinding(bool bReFind,DWORD npcID ,tstring strMapName)
{
	bNeedRePathFinding = bReFind;
	dwReFindNPCid =npcID;
	m_strMapName = strMapName;
}

void LocalPlayer::KeepMoving()
{
	Vector3 endEx;
	if (!bNeedRePathFinding)
		return;

	if( m_strMapName != MapMgr::Inst()->GetCurMapName() )//���ڱ���ͼ�����п糡��Ѱ·
	{
		QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
		ASSERT( P_VALID(pQuery) );
		if( QuestMgr::Inst()->MoveToMap(m_strMapName.c_str(),endEx) != TRUE )
			return;

		tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
		event.start = GetPos();
		event.end = endEx;
		event.validDist = 100.0f;
		event.dwNpcTypeID = dwReFindNPCid;
		event.bNotThisMap = true;
		event.strMapName = m_strMapName;

		TObjRef<GameFrameMgr>()->SendEvent( &event );
	}
	else
	{
		QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
		ASSERT( P_VALID(pQuery) );
		if( !pQuery->GetNPCPosition( dwReFindNPCid, endEx ) )
			return;

		tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
		event.start = GetPos();
		event.end = endEx;
		event.validDist = 100.0f;
		event.dwNpcTypeID = dwReFindNPCid;

		TObjRef<GameFrameMgr>()->SendEvent( &event );
	}

}

bool LocalPlayer::IsCrazyState()
{
	TList<tagRoleBuff*> playerBuffList = this->GetBuffList();
	if (!playerBuffList.Empty())
	{
		tagRoleBuff* pBuff=NULL;
		playerBuffList.ResetIterator();
		while(playerBuffList.PeekNext(pBuff))
		{
			if (pBuff->dwBuffTypeID == 9001301) // �з����BUFF
			{
				return true;
			}
		}	
	}
	return false;
}

void LocalPlayer::SetVipDays(tagDWORDTime nNum)
{
	m_nVipDays.day  = nNum.day;
	m_nVipDays.hour = nNum.hour;
	m_nVipDays.min = nNum.min;
	m_nVipDays.month = nNum.month;
	m_nVipDays.sec = nNum.sec;
	m_nVipDays.year = nNum.year;
}

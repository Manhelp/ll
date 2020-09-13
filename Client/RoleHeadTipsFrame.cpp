#include "stdafx.h"
#include "RoleHeadTipsFrame.h"
#include "WorldFrame.h"
#include "Role.h"
#include "NPC.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "Pet.h"
#include "PetManager.h"
#include "PlayerNameTab.h"
#include "CreatureData.h"
#include "PlayerStateFrame.h"
#include "WorldPickerFrame.h"
#include "QuestMgr.h"
#include "StallMgr.h"
#include "GameSet.h"
#include "GuildMgr.h"
#include "VipNetBarMgr.h"
#include "CombatSys.h"
#include "TeamSys.h"
#include "../WorldDefine/RoleDefine.h"
#include "ClergyProtoData.h"
#include "FamilyMgr.h"
#include "SaveAccount.h"

const Color4ub NPCNameColor = Color4ub(255,138,248,255);			//NPC����,

const Color4ub GrayColor = 0xFFB0E6F2;								// ��ɫ
const Color4ub GreenColor = 0xFF00FF00;								// ��ɫ
const Color4ub BlueColor = 0xFF0000FF;								// ��ɫ
const Color4ub RedColor = 0xFFFF0000;								// ��ɫ

const Color4ub PinkRedColor = 0xFFF89ECB;							// �Ϸۺ�ɫ

//PK״̬
const Color4ub PlayerPeaceColor = Color4ub(255,255,255,255);		//��ƽ
const Color4ub PlayerWarnningColor = Color4ub(255,255,228,0);		//����
const Color4ub PlayerWickedColor = Color4ub(255,255,0,0);			//а��
const Color4ub PlayerWantedColor = Color4ub(255,197,0,254);			//ͨ��

//��ɫ״̬
const Color4ub PlayerSafeguardColor = Color4ub(255,154,234,133);	//������
const Color4ub PlayerPKColor = Color4ub(255,255,54,0);				//����

// ��ս״̬ 
const Color4ub PlayerCityAttackColor = Color4ub(255, 255, 0, 0);	// ������ 
const Color4ub PlayerCityDefenceColor = Color4ub(255, 0, 0, 255);	// ���ط�

// ����ս��״̬ 
const Color4ub PlayerGuildWarFriend = Color4ub(255, 0, 162, 255);	// �Լ����� ��ɫ
const Color4ub PlayerGuildWarEnemy = 0xFFFFAE00;	// �жԹ��� ��ɫ

// ս��״̬
const Color4ub PlayerWarEvilColor = Color4ub(255, 0, 255, 0);	//а��
const Color4ub PlayerWarPeaceColor = Color4ub(255, 255, 0, 0);	//����
const Color4ub PlayerWarNeutralColor = Color4ub(255, 255, 255, 255);	//����

//������ʾ��ͼ
const TCHAR* szTeam = _T("data\\ui\\main\\L_main-leader-c.bmp");

const float scalePerUnit = 2.0e-4f;//ÿ�����絥λ���������ű���

class NpcNameColorTable
{
	enum MNCTConst
	{
		NumColor=5,
	};
private:
	Color4ub m_colorTab[NumColor];

public:
	NpcNameColorTable()
	{
		m_colorTab[0]=Color4ub(255,255,255,255);	//ͬ�����ȼ�+1���ȼ�-1��������
		m_colorTab[1]=Color4ub(255,255,188,0);		//�Ը߼����ȼ�+2���ȼ�+3��������
		m_colorTab[2]=Color4ub(255,255,66,0);		//�߼����ȼ���3�����ϣ���������3����������
		m_colorTab[3]=Color4ub(255,50,206,164);		//�Եͼ����ȼ�-2���ȼ�-3����������
		m_colorTab[4]=Color4ub(255,174,174,174);	//�ͼ����ȼ�����3������������3����������
	}
	~NpcNameColorTable()	{}

	Color4ub GetColor(int monsterLv)
	{
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		int lpLv = pLocalPlayer->GetRoleLevel();		//��ҵȼ�

		int diff=monsterLv-lpLv;
		if(diff==0
			|| diff==1
			|| diff==-1)
		{
			return m_colorTab[0];
		}
		if(diff>1)
		{
				if(diff>3)
					return m_colorTab[2];
				else 
					return m_colorTab[1];
		}
		if(diff<-1)
		{
			if(diff<-3)
				return m_colorTab[4];
			else 
				return m_colorTab[3];
		}

		return m_colorTab[0];
	}
};

NpcNameColorTable g_npcColorTab;

class RoleHeadTipSortFunc
{
public:
	bool operator() (const tagHeadTip* tip1,const tagHeadTip* tip2)
	{
		return tip1->scrPos.z > tip2->scrPos.z;
	}
};


RoleHeadTipsFrame::RoleHeadTipsFrame( void ):m_Trunk(this)
{
	m_pFont				= NULL;
	m_bShowTips			= TRUE;
	m_bShowHPorMP		= GameSet::Inst()->Get(EGS_ShowHPMPBar);
	m_pTempFont = new tagGUIFont();

}

RoleHeadTipsFrame::~RoleHeadTipsFrame( void )
{
	SAFE_DELETE(m_pTempFont);
}

BOOL RoleHeadTipsFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{

	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	Create();

	m_pKeyMap->Register(0, g_StrTable[_T("HotKey_HPorMP")],	
		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&RoleHeadTipsFrame::OnKey), DIK_HOME, TRUE, FALSE, 0);

	return TRUE;
}

BOOL RoleHeadTipsFrame::Destroy()
{
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pFontTitle);
	SAFE_DELETE(m_pFmFont);
	
	IDraw2D::Inst()->ReleaseTexture(m_pResHP);
	IDraw2D::Inst()->ReleaseTexture(m_pResMP);	
	ClearAllTips();
	return GameFrame::Destroy();
}

VOID RoleHeadTipsFrame::Render3D()
{
	if( m_bShowTips )
	{
		UpdateRoleHeadTips();
		DrawAllRoleHeadTips();
		m_questFlag.Update();
	}
}

VOID RoleHeadTipsFrame::OnEvent( tagGameEvent* pEvent )
{
	
}

VOID RoleHeadTipsFrame::Update()
{
	
}

void RoleHeadTipsFrame::Create()
{
	m_pFont = Device()->NewDepthFont();
	
	if(SaveAccount::Inst()->GetLoginMode() == 11)	//������½Ҫ�Ӵ�
	{
		m_pFont->Create(8, 19, 0, FALSE, g_StrTable[_T("Font_MSYH")]);
	}
	else
	{
		m_pFont->Create(8, 17, 0, FALSE, g_StrTable[_T("Font_MSYH")]);
	}

	m_pFontTitle = Device()->NewDepthFont();
	m_pFontTitle->Create(6, 15, 0, FALSE, g_StrTable[_T("Font_MSYH")] );

	m_pFmFont = Device()->NewDepthFont();
	m_pFmFont->Create(0, 14, 0, FALSE, g_StrTable[_T("Font_MSYH")] );

	m_questFlag.Create();

	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	m_pResHP = IDraw2D::Inst()->NewTexture(_T("data\\ui\\common\\L_jindu-glass-e-j.bmp"),pIFS);
	m_pResMP = IDraw2D::Inst()->NewTexture(_T("data\\ui\\common\\L_jindu-glass-d-j.bmp"),pIFS);
}

void RoleHeadTipsFrame::UpdateRoleHeadTips()
{
	ClearAllTips();
	ClientCamera* pCamera = GetWorldFrm()->GetCamera();
	const Vector3& lookFrom = pCamera->GetPos();
	Vector3 scrPos;
	Vector3	worldPos;
	const map<DWORD,Role*>& roleMap = RoleMgr::Inst()->GetRoleMap();
	for(map<DWORD,Role*>::const_iterator it=roleMap.begin();
		it!=roleMap.end(); ++it)
	{
		Role* pRole = const_cast<Role*>(it->second);

		// ��Ұ֮��Ľ�ɫ����ʾͷ����Ϣ
		if( !RoleMgr::Inst()->IsInViewDist(pRole) )
			continue;

		if( pRole->IS_KIND_OF(Player) )
		{
			Player* pPlayer = (Player*)pRole;
			if( pPlayer->GetRoleState(ERS_Stall) )
				continue;
		}
		else if( pRole->IS_KIND_OF(Pet) ) // ��̯��ҵĳ������
		{
			Pet* pPet = (Pet*)pRole;
			DWORD dwOwnerID = pPet->GetOwnerID();
			Player* pPlayer = (Player*)RoleMgr::Inst()->FindRole(dwOwnerID);
			if( P_VALID(pPlayer) && pPlayer->GetRoleState( ERS_Stall ) )
				continue;
		}

		pRole->GetHeadPos(worldPos);
		scrPos = pCamera->WorldToScreenF(worldPos);
		scrPos.z /= 2;
		if( scrPos.z<0.0f || scrPos.z>0.5f )
			continue;

		float dist = D3DXVec3Length(&(lookFrom-worldPos));
		float scale = 1.0f;
		if( dist > 6000.0f )
			continue;
	

		DWORD dwRoleID = pRole->GetID();
		tstring strRoleName ;
		tstring strRoleTitle;
		tstring strGuildTip;
		tstring strVipNetBarTip;
        tstring strSpouse;
		tstring strTemp;
		tstring strFamilyName;
		BOOL bPKModel = (BOOL)(GT_INVALID*2);
		tagHeadTip::ECityWarState eState = tagHeadTip::ECWS_NULL;

		EQuestFlag qf = EQF_Null;
		if( IS_PLAYER(dwRoleID) )
		{
			//������ʾ�������,�ƺŵ�
			if( GameSet::Inst()->Get(EGS_ShowOtherPlayerName) )
			{
				strRoleName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
				strRoleTitle = GetRoleTitleTips(pRole);

				strFamilyName =  FamilyMgr::Inst()->GetFamilyName(static_cast<Player*>(pRole)->GetRoleFamilyID(), FamilyMgr::ENUT_Nothing , GT_INVALID,  _T(""),true );
				
// 				const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto(((Player*)pRole)->GetPlayerClergy()); //add by bli ������ǰ�������ְ����
// 				if(P_VALID(pClergyName))
// 				{
// 					strTemp = strRoleName;
// 					strRoleName = pClergyName->szName;
// 					strRoleName += _T("-");
// 					strRoleName += strTemp;
// 				}

				if( GameSet::Inst()->Get(EGS_ShowFactionName) )
					strGuildTip =  GetRoleGuildTips(pRole);

				strVipNetBarTip = GetRoleVipNetBarTips(pRole);
				bPKModel = GetRolePKMode(pRole);
                strSpouse = GetRoleSpouse(pRole);

				if (pRole->IsInState(ES_CSAttack)  || pRole->IsInState(ES_PvPAttack))
				{
					eState= tagHeadTip::ECWS_Attack;
				}
				else if (pRole->IsInState(ES_CSDefence) || pRole->IsInState(ES_PvPDefence))
				{
					eState = tagHeadTip::ECWS_Defence;
				}
				else
				{
					eState = tagHeadTip::ECWS_NULL;
				}

			}
			//��ʾ�����ָ�������,�ƺŵ�
			else if(GetWorldPickerFrm()->GetCurMousePointRoleID()==dwRoleID)
			{
				strRoleName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
				strRoleTitle = GetRoleTitleTips(pRole);

				strFamilyName =  FamilyMgr::Inst()->GetFamilyName(static_cast<Player*>(pRole)->GetRoleFamilyID(), FamilyMgr::ENUT_Nothing , GT_INVALID,  _T(""),false);
// 				const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto(((Player*)pRole)->GetPlayerClergy()); //add by bli ������ǰ�������ְ����
// 				if(P_VALID(pClergyName))
// 				{
// 					strTemp = strRoleName;
// 					strRoleName = pClergyName->szName;
// 					strRoleName += _T("-");
// 					strRoleName += strTemp;
// 				}

				if( GameSet::Inst()->Get(EGS_ShowFactionName) )
					strGuildTip =  GetRoleGuildTips(pRole);

				strVipNetBarTip = GetRoleVipNetBarTips(pRole);
				bPKModel = GetRolePKMode(pRole);
                strSpouse = GetRoleSpouse(pRole);

				if (pRole->IsInState(ES_CSAttack) || pRole->IsInState(ES_PvPAttack))
				{
					eState = tagHeadTip::ECWS_Attack;
				}
				else if (pRole->IsInState(ES_CSDefence)|| pRole->IsInState(ES_PvPDefence))
				{
					eState = tagHeadTip::ECWS_Defence;
				}
				else
				{
					eState = tagHeadTip::ECWS_NULL;
				}

			}
			//��ʾ��ǰѡ���������,�ƺŵ�
			else 
			{
				CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
				if( !P_VALID( pCombatSys ) )
					continue;
				if( pCombatSys->GetCurTargetID() == dwRoleID )
				{
					strRoleName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
					strRoleTitle = GetRoleTitleTips(pRole);

					strFamilyName =  FamilyMgr::Inst()->GetFamilyName(static_cast<Player*>(pRole)->GetRoleFamilyID(), FamilyMgr::ENUT_Nothing , GT_INVALID,  _T(""),false);
// 					const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto(((Player*)pRole)->GetPlayerClergy()); //add by bli ������ǰ�������ְ����
// 					if(P_VALID(pClergyName))
// 					{
// 						strTemp = strRoleName;
// 						strRoleName = pClergyName->szName;
// 						strRoleName += _T("-");
// 						strRoleName += strTemp;
// 					}
					if( GameSet::Inst()->Get(EGS_ShowFactionName) )
						strGuildTip =  GetRoleGuildTips(pRole);

					strVipNetBarTip = GetRoleVipNetBarTips(pRole);
					bPKModel = GetRolePKMode(pRole);
                    strSpouse = GetRoleSpouse(pRole);

					if (pRole->IsInState(ES_CSAttack)|| pRole->IsInState(ES_PvPAttack))
					{
						eState = tagHeadTip::ECWS_Attack;
					}
					else if (pRole->IsInState(ES_CSDefence) || pRole->IsInState(ES_PvPDefence))
					{
						eState = tagHeadTip::ECWS_Defence;
					}
					else
					{
						eState = tagHeadTip::ECWS_NULL;
					}

				}
			}
		}
		else if( IS_CREATURE(dwRoleID) )
		{
			NPC* pNpc = (NPC*)pRole;
			const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pNpc->GetTypeID() );
			if( !P_VALID(pProto) )
				continue;
				
			if( pProto->eType==ECT_NPC )
			{	
				if(GameSet::Inst()->Get(EGS_ShowNPCName))
				{
					strRoleName = pProto->szName;
					strRoleTitle = GetRoleTitleTips(pRole);
				}
			
				//������ʾ
				if( QuestMgr::Inst()->GetQuery()->IsAcceptNPC(((NPC*)pRole)->GetTypeID()) )
				{
					qf = EQF_Accept;

                    // �Ƿ���ظ�����
                    vector<UINT16> quests;
                    QuestMgr::Inst()->GetData()->GetNPCAcceptQuests(((NPC*)pRole)->GetTypeID(), quests);

                    const tagQuestProto* pQProto = NULL;

                    if (quests.size() > 0)
                        pQProto = QuestMgr::Inst()->GetData()->GetQuestProto(quests[0]);

                    if (P_VALID(pQProto) && pQProto->repeatable)
                        qf = EQF_RepeatAccept;
					
					// �鿴��NPC�ɽ������Ƿ񱻹��˵�
					if (QuestMgr::Inst()->GetData()->IsNPCHasQuestByCurrentFilter(((NPC*)pRole)->GetTypeID()) == FALSE)
						qf = EQF_Null;
					// �Ƿ���ݵȼ�����
					if (QuestMgr::Inst()->GetData()->IsFilterByPlayerLevel() == TRUE)
					{
						if (QuestMgr::Inst()->GetData()->IsNPCHasQuestByCurrentFilterByPlayerLevel(((NPC*)pRole)->GetTypeID()) == FALSE)
							qf = EQF_Null;
					}
				}
				if( QuestMgr::Inst()->GetQuery()->IsCompleteNPC(((NPC*)pRole)->GetTypeID()) 
					|| QuestMgr::Inst()->GetQuery()->IsUpdateNPC(((NPC*)pRole)->GetTypeID()) )
                {
				    qf = EQF_Finish;

                    // �Ƿ���ظ�����
                    vector<UINT16> quests;
                    QuestMgr::Inst()->GetData()->GetNPCCompleteQuests(((NPC*)pRole)->GetTypeID(), quests);

                    const tagQuestProto* pQProto = NULL;

                    if (quests.size() > 0)
                        pQProto = QuestMgr::Inst()->GetData()->GetQuestProto(quests[0]);

                    if (P_VALID(pQProto) && pQProto->repeatable)
                        qf = EQF_RepeatFinish;
                }
			}
			//�����вɼ�������������Ϻ�Ӧ��ʾ����
			if( (pNpc->IsQuestInvesGO() || pNpc->IsQuestTalkGameObject() || pNpc->IsCommonInvesGO())
				&& QuestMgr::Inst()->GetQuery()->IsInvesObject(pProto->dwTypeID)
				&& GetWorldPickerFrm()->GetCurMousePointRoleID()==dwRoleID )
			{
				strRoleName = pProto->szName;
				strRoleTitle = GetRoleTitleTips(pRole);

				//������ʾ
				if( QuestMgr::Inst()->GetQuery()->IsAcceptNPC(((NPC*)pRole)->GetTypeID()) )
					qf = EQF_Accept;
				if( QuestMgr::Inst()->GetQuery()->IsCompleteNPC(((NPC*)pRole)->GetTypeID()) 
					|| QuestMgr::Inst()->GetQuery()->IsUpdateNPC(((NPC*)pRole)->GetTypeID()) )
					qf = EQF_Finish;
			}
			else if( pProto->eType==ECT_Monster )
			{	
				//��Ѩ����ʾ,�ӳ���ʾ
				if( pProto->nType3==6 )
					continue;
				else if(GameSet::Inst()->Get(EGS_ShowMonsterName))
				{
					strRoleName = pProto->szName;
					strRoleTitle = GetRoleTitleTips(pRole);
				}
				else if( GetWorldPickerFrm()->GetCurMousePointRoleID()==dwRoleID )
				{
					strRoleName = pProto->szName;
					strRoleTitle = GetRoleTitleTips(pRole);
				}
				else 
				{
					CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
					if( !P_VALID( pCombatSys ) )
						continue;
					if( pCombatSys->GetCurTargetID() == dwRoleID )
					{
						strRoleName = pProto->szName;
						strRoleTitle = GetRoleTitleTips(pRole);
					}
				}
			}
			else if( pProto->eType==ECT_GameObject
				&& pProto->nType2==EGOT_Gather )
			{
				strRoleName = pProto->szName;
			}
			else if( (pNpc->IsCommonInvesGO() || pNpc->IsQuestTalkGameObject() || pNpc->IsChest() ) && 
					 GetWorldPickerFrm()->GetCurMousePointRoleID()==dwRoleID )
			{
				strRoleName = pProto->szName;
			}
			else if (pProto->eType == ECT_GameObject && pProto->nType2 == EGOT_Remnant)
			{
				strRoleName = pProto->szName;
			}
		}
        else if (IS_PET(dwRoleID))
        {
            Pet* pPet = static_cast<Pet*>(pRole);

            // ֻ��ʾ��ͨ����̬��ģ�����֣����ʱ����ʾ
            if (pPet->IsFSMEnabled())
            {
                const tagPetDispInfo* pDispInfo = NULL;
                if (pPet->IsAttributeInited())
                    pDispInfo = PetManager::Inst()->FindPetDispInfo(pPet->GetOwnerID(), dwRoleID);

                if (P_VALID(pDispInfo))
                {
                    strRoleName = pDispInfo->szName;
                }
            }
        }

		tagHeadTip *pTip = new tagHeadTip; 
		pTip->eCityWarState = eState;
		
		pRole->GetHeadPos(pTip->worldPos);
		pTip->roleID = dwRoleID;
		pTip->strNameTip = strRoleName;
		pTip->qeustFlag = qf;
		pTip->txtNameColor = GetRoleTipsColor(pRole);
		//added by hyu
		pTip->strTitleTip = strRoleTitle;
		pTip->txtTitleColor = GetRoleTitleColor(pRole);
        pTip->bPicTitle = IsRolePicTitle(pRole);
		//end added by hyu
		pTip->strResTex = GetRoleTitleIcon(pRole);
		//added by hqzhang
		if( GameSet::Inst()->Get(EGS_ShowFactionName) )
			pTip->strGuildTip = strGuildTip;
		//end added by hqzhang
        //added by hyu
        pTip->strSpouse = strSpouse;
        pTip->spouseColor = GetRoleSpouseColor(pRole);

		pTip->nVIPLevel = GetRoleVIPLevel(pRole);

		pTip->strVipNetBarTip = strVipNetBarTip;
		pTip->bPKMode = bPKModel;

		pTip->strResGodTex = GetRoleGodJobIcon(pRole);
		pTip->strFamilyNameTip = strFamilyName;

		m_vecTipsList.push_back(pTip);
	}

	if( GameSet::Inst()->Get(EGS_ShowSelfName) )
	{
		DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();
		tstring strRoleName = PlayerNameTab::Inst()->FindNameByID(dwLocalID);
		
		tstring strTemp;

		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
			return;

		tstring strFamilyName =  FamilyMgr::Inst()->GetFamilyName(pLocalPlayer->GetRoleFamilyID(), FamilyMgr::ENUT_Nothing , GT_INVALID,  _T(""),false);
		if( pLocalPlayer->GetRoleState(ERS_Stall) )
			return;
	
		tstring strRoleTitle = GetRoleTitleTips(pLocalPlayer);

		if(strRoleName==_T("") && strRoleTitle == _T(""))	//altered by hyu [if(strRoleName == _T(""))]
			return;

// 		const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto(pLocalPlayer->GetPlayerClergy()); //add by bli ������ǰ�������ְ����
// 		if(P_VALID(pClergyName))
// 		{
// 			strTemp = strRoleName;
// 			strRoleName = pClergyName->szName;
// 			strRoleName += _T("-");
// 			strRoleName += strTemp;
// 		}

		tagHeadTip *pTip = new tagHeadTip;
		
		if (pLocalPlayer->IsInState(ES_CSAttack) || pLocalPlayer->IsInState(ES_CSAttack))
		{
			pTip->eCityWarState= tagHeadTip::ECWS_Attack;
		}
		else if (pLocalPlayer->IsInState(ES_CSDefence) || pLocalPlayer->IsInState(ES_PvPDefence))
		{
			pTip->eCityWarState = tagHeadTip::ECWS_Defence;
		}
		else
		{
			pTip->eCityWarState = tagHeadTip::ECWS_NULL;
		}


		pLocalPlayer->GetHeadPos(pTip->worldPos);
		pTip->roleID = dwLocalID;
		pTip->strNameTip = strRoleName;
		pTip->qeustFlag = EQF_Null;
		pTip->txtNameColor = GetRoleTipsColor(pLocalPlayer);
		//added by hyu
		pTip->strTitleTip = strRoleTitle;
		pTip->txtTitleColor = GetRoleTitleColor(pLocalPlayer);
        pTip->bPicTitle = IsRolePicTitle(pLocalPlayer);
		//end added by hyu
        pTip->strResTex = GetRoleTitleIcon(pLocalPlayer);

		//added by hqzhang
		if( GameSet::Inst()->Get(EGS_ShowFactionName) )
			pTip->strGuildTip = GetRoleGuildTips(pLocalPlayer);
		//end added by hqzhang

		pTip->strVipNetBarTip = GetRoleVipNetBarTips(pLocalPlayer);
		pTip->bPKMode = GetRolePKMode(pLocalPlayer);

        pTip->strSpouse = GetRoleSpouse(pLocalPlayer);
        pTip->spouseColor = GetRoleSpouseColor(pLocalPlayer);

		pTip->nVIPLevel = GetRoleVIPLevel(pLocalPlayer);
		pTip->strResGodTex = GetRoleGodJobIcon(pLocalPlayer);

        pTip->rcTitle.bottom    = 0;
        pTip->rcTitle.top       = 0;
        pTip->rcTitle.left      = 0;
        pTip->rcTitle.right     = 0;

		pTip->rcGodJob.bottom    = 0;
		pTip->rcGodJob.top       = 0;
		pTip->rcGodJob.left      = 0;
		pTip->rcGodJob.right     = 0;

		pTip->strFamilyNameTip = strFamilyName;

		m_vecTipsList.push_back(pTip);
	}
}

void RoleHeadTipsFrame::DrawAllRoleHeadTips()
{
	float nowTime = Kernel::Inst()->GetAccumTime();
	ClientCamera* pCamera = GetWorldFrm()->GetCamera();
	const Vector3& lookFrom = pCamera->GetPos();
	
	//�������Ļ����
	vector<tagHeadTip*>::iterator iter;
	for(iter=m_vecTipsList.begin(); iter!=m_vecTipsList.end(); ++iter)
	{
		tagHeadTip* tip = (*iter);
		tip->scrPos = pCamera->WorldToScreenF(tip->worldPos);
		tip->scrPos.z /= 2;

		tagPoint ptSize;
		m_pRender->GetTextSize(tip->strNameTip.c_str(), NULL, ptSize);

		float dist = D3DXVec3Length(&(lookFrom-tip->worldPos));
		float scale = 1.0f;
		if( dist > 2000.0f )
		{
			scale = 1.0f - (dist-2000.0f)*scalePerUnit;
			if( scale < 0.1f )
				scale = 0.1f;
		}
		tip->scale = scale;
	}

	//����Zֵ����
	RoleHeadTipSortFunc sortFunc;
	std::sort(m_vecTipsList.begin(), m_vecTipsList.end(), sortFunc);

	//���λ���
	int clientW=Device()->GetClientWidth();
	int clientH=Device()->GetClientHeight();
	
	m_pFont->Begin();
	m_pFontTitle->Begin();
	m_pFmFont->Begin();
	for(iter=m_vecTipsList.begin(); iter!=m_vecTipsList.end(); ++iter)
	{
        (*iter)->rcTitle.bottom     = 0;
        (*iter)->rcTitle.top        = 0;
        (*iter)->rcTitle.left       = 0;
        (*iter)->rcTitle.right      = 0;

		(*iter)->rcGodJob.bottom     = 0;
		(*iter)->rcGodJob.top        = 0;
		(*iter)->rcGodJob.left       = 0;
		(*iter)->rcGodJob.right      = 0;

		tagHeadTip& tip=(*(*iter));
		if(tip.scrPos.z<0.0f || tip.scrPos.z>0.5f)			//��׶����
			continue;
		POINT pt = pCamera->WorldToScreen(tip.worldPos);

		tagPoint ptSize;
		m_pRender->GetTextSize(tip.strNameTip.c_str(), NULL, ptSize);

		m_pTempFont->dwHandle =  (DWORD)m_pFmFont;
		tagPoint ptSizeFm;
		m_pRender->GetTextSize_Dp(tip.strFamilyNameTip.c_str(),m_pTempFont , ptSizeFm);
		//����λ������
		if( IS_PLAYER(tip.roleID) || IS_CREATURE(tip.roleID) || IS_PET(tip.roleID) )
		{
			pt.y -= (LONG)(25.0f * tip.scale);
		}
        //������������
        if ( IS_PET(tip.roleID) )
        {
            tip.scale = tip.scale * 0.75;
				//pt.y -= (LONG)(25.0f * tip.scale);
        }

		RECT rcName;
		rcName.left = pt.x - ptSize.x/2;
		rcName.right = pt.x + ptSize.x/2;
		rcName.top = pt.y - ptSize.y;
		rcName.bottom = pt.y;
		tip.rectName = rcName;
		
		// ��ְλ������
		if( IS_PLAYER(tip.roleID) && tip.strResGodTex != _T("") )
		{
			pt.y -= (LONG)(16.0f * tip.scale);
		}

		RECT rcGodJob;
		rcGodJob.left = pt.x - 32*tip.scale;
		rcGodJob.right = pt.x + 32*tip.scale;
		rcGodJob.top = pt.y - 16*tip.scale;
		rcGodJob.bottom = pt.y;
		tip.rcGodJob = rcGodJob;



		//���޳�νλ��������
		if( IS_PLAYER(tip.roleID) && !tip.strSpouse.empty() )
		{
				pt.y -= (LONG)(14.0f * tip.scale);
		}

		RECT rcSpouse;
		rcSpouse.left = pt.x - ptSizeFm.x/2;
		rcSpouse.right = pt.x + ptSizeFm.x/2;
		rcSpouse.top = pt.y - ptSizeFm.y;
		rcSpouse.bottom = pt.y;


		// ����
		if( IS_PLAYER(tip.roleID) && tip.strFamilyNameTip != _T("") )
		{
			pt.y -= ptSizeFm.y;
		}
		RECT rcFmName;
		rcFmName.left = pt.x - ptSizeFm.x/2;
		rcFmName.right = pt.x + ptSizeFm.x/2;
		rcFmName.top = pt.y - ptSizeFm.y;
		rcFmName.bottom = pt.y;
		tip.rectFamilyName = rcFmName;

		//added by hqzhang
		//����λ��������
		if( IS_PLAYER(tip.roleID) && tip.strGuildTip != _T("") )
		{
			pt.y -= (LONG)(18.0f * tip.scale);
		}

		m_pRender->GetTextSize(tip.strGuildTip.c_str(), NULL, ptSize);

		RECT rcGuild;
		rcGuild.left = pt.x - ptSize.x/2;
		rcGuild.right = pt.x + ptSize.x/2;
		rcGuild.top = pt.y - ptSize.y;
		rcGuild.bottom = pt.y;
		//end added by hqzhang

		//added by hyu
		//�ƺ�λ��������
		if( ( IS_PLAYER(tip.roleID) || IS_CREATURE(tip.roleID) ) && !tip.strTitleTip.empty() )
		{
			pt.y -= (LONG)(18.0f * tip.scale);
		}

        if (tip.bPicTitle)
        {
            ptSize.Set((LONG)(128.0f * tip.scale), (LONG)(32.0f * tip.scale));
        }
        else
        {
		    m_pRender->GetTextSize(tip.strTitleTip.c_str(), NULL, ptSize);
        }

		RECT rcTitle;
		rcTitle.left = pt.x - ptSize.x/2;
		rcTitle.right = pt.x + ptSize.x/2;
		rcTitle.top = pt.y - ptSize.y;
		rcTitle.bottom = pt.y;

		//��������λ��������
		if( IS_PLAYER(tip.roleID) && !tip.strVipNetBarTip.empty() )
		{
			pt.y -= (LONG)(18.0f * tip.scale);
		}

		m_pRender->GetTextSize(tip.strVipNetBarTip.c_str(), NULL, ptSize);

		RECT rcVipNetBar;
		rcVipNetBar.left = pt.x - ptSize.x/2;
		rcVipNetBar.right = pt.x + ptSize.x/2;
		rcVipNetBar.top = pt.y - ptSize.y;
		rcVipNetBar.bottom = pt.y;


		//�Ƿ�����Ļ��Χ��
		if(rcName.left >=0 && rcName.top >= 0
			&& rcName.right < clientW && rcName.bottom < clientH
			&& tip.strNameTip!=_T(""))
		{
			RECT rect1;
			rect1.top = rcName.top-1;
			rect1.bottom = rcName.bottom-1;
			rect1.left = rcName.left;
			rect1.right = rcName.right;
			m_pFont->DrawText(tip.strNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcName.top+1;
			rect1.bottom = rcName.bottom+1;
			rect1.left = rcName.left;
			rect1.right = rcName.right;
			m_pFont->DrawText(tip.strNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcName.top;
			rect1.bottom = rcName.bottom;
			rect1.left = rcName.left-1;
			rect1.right = rcName.right-1;
			m_pFont->DrawText(tip.strNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcName.top;
			rect1.bottom = rcName.bottom;
			rect1.left = rcName.left+1;
			rect1.right = rcName.right+1;
			m_pFont->DrawText(tip.strNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			//��������
			m_pFont->DrawText(tip.strNameTip.c_str(), &rcName, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.txtNameColor, tip.scrPos.z, tip.scale);

		}
		if (rcSpouse.left >=0 && rcSpouse.top >= 0
			&& rcSpouse.right < clientW && rcSpouse.bottom < clientH
			&& tip.strSpouse!=_T("") )
		{
			RECT rect1;
			rect1.top = rcSpouse.top-1;
			rect1.bottom = rcSpouse.bottom-1;
			rect1.left = rcSpouse.left;
			rect1.right = rcSpouse.right;
			m_pFmFont->DrawText(tip.strSpouse.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcSpouse.top+1;
			rect1.bottom = rcSpouse.bottom+1;
			rect1.left = rcSpouse.left;
			rect1.right = rcSpouse.right;
			m_pFmFont->DrawText(tip.strSpouse.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcSpouse.top;
			rect1.bottom = rcSpouse.bottom;
			rect1.left = rcSpouse.left-1;
			rect1.right = rcSpouse.right-1;
			m_pFmFont->DrawText(tip.strSpouse.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcSpouse.top;
			rect1.bottom = rcSpouse.bottom;
			rect1.left = rcSpouse.left+1;
			rect1.right = rcSpouse.right+1;
			m_pFmFont->DrawText(tip.strSpouse.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			//���Ʒ��޳�ν
			m_pFmFont->DrawText(tip.strSpouse.c_str(), &rcSpouse, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.spouseColor, tip.scrPos.z, tip.scale);
		}
		//added by hyu
		if (rcTitle.left >=0 && rcTitle.top >= 0
			&& rcTitle.right < clientW && rcTitle.bottom < clientH
			&& tip.strTitleTip!=_T("") )
		{
            if (!tip.bPicTitle)
            {
                RECT rect1;
                rect1.top = rcTitle.top-1;
                rect1.bottom = rcTitle.bottom-1;
                rect1.left = rcTitle.left;
                rect1.right = rcTitle.right;
                if( IS_PLAYER(tip.roleID) )
                    m_pFont->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);
                else
                    m_pFontTitle->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

                rect1.top = rcTitle.top+1;
                rect1.bottom = rcTitle.bottom+1;
                rect1.left = rcTitle.left;
                rect1.right = rcTitle.right;
                if( IS_PLAYER(tip.roleID) )
                    m_pFont->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);
                else 
                    m_pFontTitle->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

                rect1.top = rcTitle.top;
                rect1.bottom = rcTitle.bottom;
                rect1.left = rcTitle.left-1;
                rect1.right = rcTitle.right-1;
                if( IS_PLAYER(tip.roleID) )
                    m_pFont->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);
                else
                    m_pFontTitle->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

                rect1.top = rcTitle.top;
                rect1.bottom = rcTitle.bottom;
                rect1.left = rcTitle.left+1;
                rect1.right = rcTitle.right+1;
                if( IS_PLAYER(tip.roleID) )
                    m_pFont->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);
                else
                    m_pFontTitle->DrawText(tip.strTitleTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

                //���Ƴƺ�
                if( IS_PLAYER(tip.roleID) )
                    m_pFont->DrawText(tip.strTitleTip.c_str(), &rcTitle, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.txtTitleColor, tip.scrPos.z, tip.scale);
                else
                    m_pFontTitle->DrawText(tip.strTitleTip.c_str(), &rcTitle, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.txtTitleColor, tip.scrPos.z, tip.scale);
            }
            else
            {
                // ��סͼƬʽ�ƺŵ�λ��
                (*iter)->rcTitle = rcTitle;
            }

		}
		if(rcFmName.left >=0 && rcFmName.top >= 0
			&& rcFmName.right < clientW && rcFmName.bottom < clientH
			&& tip.strFamilyNameTip!=_T(""))
		{
			RECT rect1;
			rect1.top = rcFmName.top-1;
			rect1.bottom = rcFmName.bottom-1;
			rect1.left = rcFmName.left;
			rect1.right = rcFmName.right;
			m_pFmFont->DrawText(tip.strFamilyNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcFmName.top+1;
			rect1.bottom = rcFmName.bottom+1;
			rect1.left = rcFmName.left;
			rect1.right = rcFmName.right;
			m_pFmFont->DrawText(tip.strFamilyNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcFmName.top;
			rect1.bottom = rcFmName.bottom;
			rect1.left = rcFmName.left-1;
			rect1.right = rcFmName.right-1;
			m_pFmFont->DrawText(tip.strFamilyNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcFmName.top;
			rect1.bottom = rcFmName.bottom;
			rect1.left = rcFmName.left+1;
			rect1.right = rcFmName.right+1;
			m_pFmFont->DrawText(tip.strFamilyNameTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			//����
			m_pFmFont->DrawText(tip.strFamilyNameTip.c_str(), &rcFmName, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.txtNameColor, tip.scrPos.z, tip.scale);
		}
		//added by hqzhang
		if (rcGuild.left >=0 && rcGuild.top >= 0
			&& rcGuild.right < clientW && rcGuild.bottom < clientH
			&& tip.strGuildTip!=_T("") )
		{
			RECT rect1;
			rect1.top = rcGuild.top-1;
			rect1.bottom = rcGuild.bottom-1;
			rect1.left = rcGuild.left;
			rect1.right = rcGuild.right;
			m_pFont->DrawText(tip.strGuildTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcGuild.top+1;
			rect1.bottom = rcGuild.bottom+1;
			rect1.left = rcGuild.left;
			rect1.right = rcGuild.right;
			m_pFont->DrawText(tip.strGuildTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcGuild.top;
			rect1.bottom = rcGuild.bottom;
			rect1.left = rcGuild.left-1;
			rect1.right = rcGuild.right-1;
			m_pFont->DrawText(tip.strGuildTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcGuild.top;
			rect1.bottom = rcGuild.bottom;
			rect1.left = rcGuild.left+1;
			rect1.right = rcGuild.right+1;
			m_pFont->DrawText(tip.strGuildTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			//���ư���
			m_pFont->DrawText(tip.strGuildTip.c_str(), &rcGuild, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.txtNameColor, tip.scrPos.z, tip.scale);
		}
		//end added by hqzhang

		if(rcVipNetBar.left >=0 && rcVipNetBar.top >= 0
			&& rcVipNetBar.right < clientW && rcVipNetBar.bottom < clientH
			&& tip.strVipNetBarTip!=_T(""))
		{
			RECT rect1;
			rect1.top = rcVipNetBar.top-1;
			rect1.bottom = rcVipNetBar.bottom-1;
			rect1.left = rcVipNetBar.left;
			rect1.right = rcVipNetBar.right;
			m_pFont->DrawText(tip.strVipNetBarTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcVipNetBar.top+1;
			rect1.bottom = rcVipNetBar.bottom+1;
			rect1.left = rcVipNetBar.left;
			rect1.right = rcVipNetBar.right;
			m_pFont->DrawText(tip.strVipNetBarTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcVipNetBar.top;
			rect1.bottom = rcVipNetBar.bottom;
			rect1.left = rcVipNetBar.left-1;
			rect1.right = rcVipNetBar.right-1;
			m_pFont->DrawText(tip.strVipNetBarTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			rect1.top = rcVipNetBar.top;
			rect1.bottom = rcVipNetBar.bottom;
			rect1.left = rcVipNetBar.left+1;
			rect1.right = rcVipNetBar.right+1;
			m_pFont->DrawText(tip.strVipNetBarTip.c_str(), &rect1, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, 0xFF290404, tip.scrPos.z, tip.scale);

			//���ƽ�����������
			m_pFont->DrawText(tip.strVipNetBarTip.c_str(), &rcVipNetBar, DT_TOP|DT_CENTER|DT_SINGLELINE|DT_NOCLIP, tip.txtNameColor, tip.scrPos.z, tip.scale);
		}
	}
	m_pFmFont->End();
	m_pFontTitle->End();
	m_pFont->End();

	IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);
	for(iter=m_vecTipsList.begin(); iter!=m_vecTipsList.end(); iter++)
	{
		tagHeadTip& tip=(*(*iter));

		if(tip.scrPos.z<0.0f || tip.scrPos.z>0.5f)			//��׶����
			continue;
		POINT pt = pCamera->WorldToScreen(tip.worldPos);

		RECT rc;
		rc.left = pt.x - 35;
		rc.right = pt.x + 35;
		rc.top = pt.y - 35;
		rc.bottom = pt.y + 35;

		if(rc.left>=0 && rc.top>=0
			&& rc.right<clientW && rc.bottom<clientH)
		{
			//����������ʾ
			m_questFlag.Draw(pt, tip.scrPos.z, tip.scale, tip.qeustFlag);
		}
		// ͼ��
		TCHAR* szHostility = _T("");

		TCHAR* szVIP = _T("");

		if (tip.eCityWarState != tagHeadTip::ECWS_NULL)
		{
			// ����ͼ��
			if (tip.eCityWarState == tagHeadTip::ECWS_Attack)
				szHostility = _T("data\\ui\\city\\l_jian.dds");
			if (tip.eCityWarState == tagHeadTip::ECWS_Defence)
				szHostility = _T("data\\ui\\city\\l_dun.dds");

		}
		else if( !tip.strNameTip.empty() )
		{
			BOOL bPKMode = tip.bPKMode;
			if( bPKMode == TRUE )
				szHostility = _T("data\\ui\\pk\\L_pk_duikang.bmp");
			else if( bPKMode == FALSE )
				szHostility = _T("data\\ui\\pk\\L_pk_shalu.bmp");

			INT nLevel = tip.nVIPLevel;
			if( nLevel == 1 )
				szVIP = _T("data\\ui\\Main\\L_main_silvervip.bmp");
			else if( nLevel == 2 )
				szVIP = _T("data\\ui\\Main\\L_main_goldvip.bmp");
			else if( nLevel == 3 )
				szVIP = _T("data\\ui\\Main\\L_main_supervip.bmp");
		}

			bool needReLocationPos = true;
			if( tip.bPKMode == TRUE || tip.bPKMode == FALSE )
			{
				tagPoint ptSize;
				m_pRender->GetTextSize(tip.strNameTip.c_str(), NULL, ptSize);

				float teamSize	= 15.0f*tip.scale;
				pt.y -= (LONG)(23 * tip.scale);
				needReLocationPos = false;
				float centerY	= (float)pt.y - 7.5f;
				RECT rcTeam;
				rcTeam.left		= LONG(pt.x - (ptSize.x + 50.0f)*tip.scale / 2);
				rcTeam.right	= LONG(rcTeam.left + teamSize);
				rcTeam.top		= (LONG)(centerY - teamSize/2.0f - 1 );
				rcTeam.bottom	= (LONG)(centerY + teamSize/2.0f - 1 );

				if(  rcTeam.left>=0 && rcTeam.top>=0
					&& rcTeam.right<clientW && rcTeam.bottom<clientH )
				{
					m_titleFlag.DrawTeamFlag(rcTeam, tip.scrPos.z, szHostility);
				}
			}

			// VIP
			bool needReLocationX = false;
			if( tip.nVIPLevel != 0 )
			{
				needReLocationX = true;
				tagPoint ptSize;
				m_pRender->GetTextSize(tip.strNameTip.c_str(), NULL, ptSize);

				float teamSize	= 32.0f*tip.scale;

				if(needReLocationPos)
				{
					pt.y -= (LONG)(23 * tip.scale);
					needReLocationPos = false;
				}
				
				float centerY	= (float)pt.y + 0.0f;
				RECT rcTeam;
				rcTeam.left		= LONG(pt.x + (ptSize.x + 20.0f)*tip.scale/2);
				rcTeam.right	= LONG(rcTeam.left + teamSize);
				rcTeam.top		= (LONG)(centerY - teamSize/2.0f - 1) ;
				rcTeam.bottom	= (LONG)(centerY + teamSize/2.0f - 1);


				if(  rcTeam.left>=0 && rcTeam.top>=0
					&& rcTeam.right<clientW && rcTeam.bottom<clientH )
				{
					m_titleFlag.DrawVipFlag(rcTeam, tip.scrPos.z, szVIP);
				}
			}

			//�����ʾ
			if( (tip.roleID == RoleMgr::Inst()->GetLocalPlayerID() && TeamSys::Inst()->IsInTeam() )
				|| TeamSys::Inst()->IsTeammate(tip.roleID) )
			{
				tagPoint ptSize;
				m_pRender->GetTextSize(tip.strNameTip.c_str(), NULL, ptSize);

				float teamSize	= 15.0f*tip.scale;

				if(needReLocationPos)
					pt.y -= (LONG)(23 * tip.scale);

				if (needReLocationX)
					pt.x += (LONG)(37 * tip.scale);

				float centerY	= (float)pt.y - 7.5f;
				RECT rcTeam;
				rcTeam.left		= LONG(pt.x + (ptSize.x + 20.0f)*tip.scale/2);
				rcTeam.right	= LONG(rcTeam.left + teamSize);
				rcTeam.top		= (LONG)(centerY - teamSize/2.0f - 1) ;
				rcTeam.bottom	= (LONG)(centerY + teamSize/2.0f - 1);


				if(  rcTeam.left>=0 && rcTeam.top>=0
					&& rcTeam.right<clientW && rcTeam.bottom<clientH )
				{
					m_titleFlag.DrawTeamFlag(rcTeam, tip.scrPos.z, szTeam);
				}
		
				//if( tip.bPKMode == TRUE || tip.bPKMode == FALSE || tip.bPKMode == GT_INVALID )
				//{
				//	RECT rcHostility;
				//	rcHostility.left	= LONG(rcTeam.right + 1.0f);
				//	rcHostility.right	= LONG(rcTeam.right + rcTeam.right - rcTeam.left + 1.0f);
				//	rcHostility.top		= rcTeam.top;
				//	rcHostility.bottom	= rcTeam.bottom;

				//	if(  rcHostility.left>=0 && rcHostility.top>=0
				//		&& rcHostility.right<clientW && rcHostility.bottom<clientH )
				//	{
				//		m_titleFlag.DrawTeamFlag(rcHostility, tip.scrPos.z, szHostility);
				//	}
				//}	
			}
			//else if( tip.bPKMode == TRUE || tip.bPKMode == FALSE || tip.bPKMode == GT_INVALID )
			//{
			//	tagPoint ptSize;
			//	m_pRender->GetTextSize(tip.strNameTip.c_str(), NULL, ptSize);

			//	float teamSize	= 15.0f*tip.scale;
			//	pt.y -= (LONG)(23 * tip.scale);
			//	float centerY	= (float)pt.y - 7.5f;
			//	RECT rcTeam;
			//	rcTeam.left		= LONG(pt.x + ptSize.x*tip.scale/2+5);
			//	rcTeam.right	= LONG(rcTeam.left + teamSize);
			//	rcTeam.top		= (LONG)(centerY - teamSize/2.0f);
			//	rcTeam.bottom	= (LONG)(centerY + teamSize/2.0f);

			//	if(  rcTeam.left>=0 && rcTeam.top>=0
			//		&& rcTeam.right<clientW && rcTeam.bottom<clientH )
			//	{
			//		m_titleFlag.DrawTeamFlag(rcTeam, tip.scrPos.z, szHostility);
			//	}
			//}
			// ��ְ��ͼ
			if ( tip.rcGodJob.left>=0 && tip.rcGodJob.top>=0
				&& tip.rcGodJob.right<clientW && tip.rcGodJob.bottom<clientH
				&& tip.strResGodTex!=_T(""))
				m_titleFlag.DrawGodJob(tip.rcGodJob, tip.scrPos.z, tip.strResGodTex.c_str());

			if(tip.strTitleTip == _T(""))
				continue;
			tagPoint ptSize;
			m_pRender->GetTextSize(tip.strTitleTip.c_str(), NULL, ptSize);
			if( IS_CREATURE(tip.roleID) )
			{
				pt.y -= (LONG)(43 * tip.scale);
			}
			else
			{
				if( tip.strGuildTip==_T("") )
				{
					pt.y -= (LONG)(43 * tip.scale);
				}
				else
				{
					pt.y -= (LONG)(61 * tip.scale);
				}
			}
		float titleSize = 15.0f*tip.scale;
		RECT rcTitleIcon;
		rcTitleIcon.right = pt.x - (LONG)(ptSize.x*tip.scale)/2;
		rcTitleIcon.left = rcTitleIcon.right - (LONG)titleSize;
		float centerY = (float)pt.y - 7.5f;
		rcTitleIcon.bottom = (LONG)(centerY + titleSize/2.0f);
		rcTitleIcon.top = (LONG)(centerY - titleSize/2.0f);
		if( !IS_PLAYER(tip.roleID) 
            && rcTitleIcon.left>=0 && rcTitleIcon.top>=0
			&& rcTitleIcon.right<clientW && rcTitleIcon.bottom<clientH
			&& tip.strResTex!=_T(""))
		{
			//�ƺ���ͼ
			m_titleFlag.DrawRoleTitleFlag(rcTitleIcon, tip.scrPos.z, tip.strResTex.c_str());
		}

		tip.rcTitle.top -= 32;
        //ͼƬʽ�ƺ�
        if (tip.bPicTitle
            && tip.rcTitle.left>=0 && tip.rcTitle.top>=0
            && tip.rcTitle.right<clientW && tip.rcTitle.bottom<clientH
            && tip.strResTex!=_T(""))
            m_titleFlag.DrawPicTitle(tip.rcTitle, tip.scrPos.z, tip.strResTex.c_str());

	}
	if( GameSet::Inst()->Get(EGS_ShowHPMPBar))
	{
		UpdateLocalHPorMPProgress();
		UpdateOtherHPorMPProgress();
	}
	IDraw2D::Inst()->EndDraw();
}

void RoleHeadTipsFrame::ClearAllTips()
{
	vector<tagHeadTip*>::iterator iter;

	for(iter=m_vecTipsList.begin(); iter!=m_vecTipsList.end(); ++iter)
	{
		tagHeadTip* pTip = (*iter);
		delete pTip;
	}

	m_vecTipsList.clear();
}

Cool3D::Color4ub RoleHeadTipsFrame::GetRoleTipsColor( Role* pRole )
{
	Color4ub color = 0xFFFFFFFF;		//Ĭ�ϻ�ɫ

	if( IS_PLAYER(pRole->GetID()) )
	{
		Player* pPlayer = (Player*)pRole;
		LocalPlayer *pSelf = RoleMgr::Inst()->GetLocalPlayer();
		if( pPlayer->GetID() == pSelf->GetID() )
		{
			if( !pPlayer->GetRoleState( ERS_Safeguard ) )
			{
				switch(pPlayer->GetPKState())
				{
				case ERPKS_Peace:
					color = PlayerPeaceColor;
					break;
				case ERPKS_Warnning:
					color = PlayerWarnningColor;
					break;
				case ERPKS_Wicked:
					color = PlayerWickedColor;
					break;
				case ERPKS_Wanted:
					color = PlayerWantedColor;
					break;
				}
			}
		}
		else
		{
			if( TeamSys::Inst()->IsTeammate(pPlayer->GetID()) || 
				pSelf->GetPlayerLoverID() == pPlayer->GetID() || 
				RoleMgr::Inst()->IsInSameMasterGuild(pPlayer) )
				color = GreenColor;
			else if( pSelf->GetRoleGuildID() == pPlayer->GetRoleGuildID() && pSelf->GetRoleGuildID() != GT_INVALID )
				color = BlueColor;
			else if( ((ERoleState)pPlayer->GetRoleState()&ERS_Safeguard) || ((ERoleState)pPlayer->GetRoleState()&ERS_SafeArea) )
			{
				color = GrayColor;
			}
			//else if( ((ERoleState)pPlayer->GetRoleState()&ERS_PK) != 0x0 || ((ERoleState)pPlayer->GetRoleState()&ERS_PKEX) != 0x0 )
			//{
			//	color = PlayerPKColor;
			//}
			else if( RoleMgr::Inst()->IsEnemy(pRole->GetID()) )
			{
				color = PinkRedColor;
			}
			else 
			{
				switch(pPlayer->GetPKState())
				{
				case ERPKS_Peace:
					color = PlayerPeaceColor;
					break;
				case ERPKS_Warnning:
					color = PlayerWarnningColor;
					break;
				case ERPKS_Wicked:
					color = PlayerWickedColor;
					break;
				case ERPKS_Wanted:
					color = PlayerWantedColor;
					break;
				}
			}
		}
	}
	else if (IS_CREATURE(pRole->GetID()))
	{
		NPC* pNpc = (NPC*)pRole;
		if(pNpc->IsNPC())
			color = NPCNameColor;
		else if(pNpc->IsMonster())
			color = g_npcColorTab.GetColor( pNpc->GetRoleLevel() );
	}
    else if (IS_PET(pRole->GetID()))
    {

    }
    else
    {

    }

	// ����ս
	if( GuildMgr::Inst()->IsInGuildWarState() )
	{
		if( IS_PLAYER(pRole->GetID()) )
		{

			LocalPlayer *pSelf = RoleMgr::Inst()->GetLocalPlayer();
			Player* pPlayer = (Player*)pRole;
			DWORD dwMyGuild = pPlayer->GetRoleGuildID();
			if (GuildMgr::Inst()->IsGuildWarEnemy(dwMyGuild))
			{
				color = PlayerGuildWarEnemy;
			}
			else if(pSelf->GetRoleGuildID() == pPlayer->GetRoleGuildID())	//ͬһ������֮��
			{
				color = PlayerGuildWarFriend;
			}

		}
	}

	// ������
	if( GuildMgr::Inst()->IsInGodBattle() )
	{
		if( IS_PLAYER(pRole->GetID()) )
		{
			Player* pPlayer = (Player*)pRole;
			DWORD dwMyGuild = pPlayer->GetRoleGuildID();
			DWORD dwDefGuild = GuildMgr::Inst()->GetGodBattleDefGuild();
			if ( dwMyGuild != GT_INVALID && dwMyGuild == dwDefGuild )
			{
				color = PlayerCityDefenceColor;
			}
		}
	}

	// ��ս״̬���ȼ���ߣ�����������ɫ
	// ����ǽ�����
	if (pRole->IsInState(ES_CSAttack)  || pRole->IsInState(ES_PvPAttack))
	{
		color = PlayerCityAttackColor;
	}
	else if (pRole->IsInState(ES_CSDefence) || pRole->IsInState(ES_PvPDefence))
	{
		color = PlayerCityDefenceColor;
	}

	// ս��״̬���ȼ���ߣ�����������ɫ������
	if(pRole->GetRoleCampType() == ECamp_Justice)	//����
	{
		color = PlayerWarPeaceColor;
	}
	else if(pRole->GetRoleCampType() == ECamp_Evil)	//а��
	{
		color = PlayerWarEvilColor;
	}
	else if(pRole->GetRoleCampType() == ECamp_Neutrality)	//����
	{
		color = PlayerWarNeutralColor;
	}

	return color;
}

const tstring RoleHeadTipsFrame::GetRoleTitleTips( Role* pRole )
{
    if( IS_PLAYER(pRole->GetID()) )
    {
        Player * pPlayer = static_cast<Player*>(pRole);
		DWORD dwTitleID = pPlayer->GetTitleID();

		if( pPlayer->GetID()==RoleMgr::Inst()->GetLocalPlayerID() 
			&&  GameSet::Inst()->Get(EGS_ShowSelfName) )
		{
			return pPlayer->GetTitleName();
		}
		else if( GameSet::Inst()->Get(EGS_ShowOtherPalyerTitle) )
		{
			return pPlayer->GetTitleName();
		}
		else
		{
			CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
			if( !P_VALID( pCombatSys ) )
				return _T("");
			if( pCombatSys->GetCurTargetID() == pPlayer->GetID() )
			{	
				return  pPlayer->GetTitleName();
			}
		}
    }
    else if( IS_CREATURE( pRole->GetID() ) )
    {
		NPC* pNpc = (NPC*)pRole;
		const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pNpc->GetTypeID() );
		if( !P_VALID(pProto) )
			return _T("");

		if( pProto->eType==ECT_NPC )
		{
			if(GameSet::Inst()->Get(EGS_ShowNPCName))
				return pProto->szTitle;
			else
				return _T("");
		}
		else if( pProto->eType==ECT_Monster )
		{	
			//���ɼ��ĳ�Ѩ����ʾ����
			if( pProto->nType2==EMTT_Nest && !pProto->bVisble)
				return _T("");
			else if(GameSet::Inst()->Get(EGS_ShowMonsterName))
				return pProto->szTitle;
			else if( GetWorldPickerFrm()->GetCurMousePointRoleID()==pRole->GetID() )
				return pProto->szTitle;
			else 
			{
				CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
				if( !P_VALID( pCombatSys ) )
					return _T("");
				if( pCombatSys->GetCurTargetID() == pNpc->GetID() )
				{	
					return pProto->szTitle;
				}
			}
		}
    }
    
	return _T("");
}

const tstring RoleHeadTipsFrame::GetRoleTitleIcon(Role* pRole)
{
	if( IS_CREATURE( pRole->GetID() ) )
	{
		NPC* pNpc = (NPC*)pRole;
		const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pNpc->GetTypeID() );
		if( !P_VALID(pProto) )
			return _T("");
		if( pProto->eType==ECT_NPC )
		{
			return pProto->szTitleIcon;
		}
		else if( pProto->eType==ECT_Monster )
		{
			if( GameSet::Inst()->Get(EGS_ShowMonsterName) )
				return pProto->szTitleIcon;
			else if( GetWorldPickerFrm()->GetCurMousePointRoleID()==pRole->GetID() )
				return pProto->szTitleIcon;
		}
	}
    else if( IS_PLAYER(pRole->GetID()) )
    {
        Player* pPlayer = static_cast<Player*>(pRole);

        DWORD dwTitleID = pPlayer->GetTitleID();
        return RoleTitleProtoData::Inst()->GetPicTitle(dwTitleID);
    }

	return _T("");
}

const tstring RoleHeadTipsFrame::GetRoleGodJobIcon(Role* pRole)
{
	if( IS_PLAYER(pRole->GetID()) )
	{
		Player* pPlayer = static_cast<Player*>(pRole);

		DWORD dwGodID = pPlayer->GetPlayerClergy();
		if (0 == dwGodID || GT_INVALID == dwGodID)
			return _T("");

		TCHAR szPath[512];
		_stprintf(szPath,_T("data\\ui\\pic_title\\god\\godtitle%02d.bmp"),dwGodID);
		tstring ss= szPath;
		return ss;
	}

	return _T("");
}
Cool3D::Color4ub RoleHeadTipsFrame::GetRoleTitleColor( Role* pRole )
{
	Color4ub color = Color4ub(255,255,255,255);		//Ĭ�ϰ�ɫ

    if( IS_PLAYER(pRole->GetID()) )
    {
        Player* pPlayer = static_cast<Player*>(pRole);

        DWORD dwTitleID = pPlayer->GetTitleID();
        DWORD dwTitleColor = RoleTitleProtoData::Inst()->GetTitleColorbyID(dwTitleID);

        return Color4ub(dwTitleColor);
    }
    else if (IS_CREATURE(pRole->GetID()))
    {
		NPC* pNpc = (NPC*)pRole;
		if(pNpc->IsNPC())
			color = NPCNameColor;
		else if(pNpc->IsMonster())
			color = g_npcColorTab.GetColor( pNpc->GetRoleLevel() );
    }
    else if (IS_PET(pRole->GetID()))
    {

    }
    else
    {

    }

	return color;
}

const tstring RoleHeadTipsFrame::GetRoleGuildTips( Role* pRole )
{
	if( IS_PLAYER(pRole->GetID()) )
	{
		Player * pPlayer = static_cast<Player*>(pRole);
		DWORD dwGuildID = pPlayer->GetRoleGuildID();
		// ���ID��Ч��ֱ�ӷ���
		if(!GT_VALID(dwGuildID))
			return _T("");

		tstring strName = GuildMgr::Inst()->GetGuildName(dwGuildID);
		if(strName.size() > 0)
		{
			TCHAR szTmp[X_SHORT_NAME];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR),
				g_StrTable[_T("GuildHead_NameNPos")], strName.c_str(),
				GuildMgr::Inst()->GetMemberPos(pPlayer->GetRoleGuildPos()).c_str());
			return szTmp;
		}
	}
	return _T("");
}

const tstring RoleHeadTipsFrame::GetRoleVipNetBarTips( Role* pRole )
{
	if( IS_PLAYER(pRole->GetID()) )
	{
		if( pRole->GetID()==RoleMgr::Inst()->GetLocalPlayerID() )
		{
			if( GameSet::Inst()->Get(EGS_ShowSelfName) )
				return VipNetBarMgr::Inst()->GetVipNetBarName(pRole->GetID());
		}
		else if( GameSet::Inst()->Get(EGS_ShowOtherPalyerTitle) )
		{
			return VipNetBarMgr::Inst()->GetVipNetBarName(pRole->GetID());
		}
		else
		{
			CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
			if( !P_VALID( pCombatSys ) )
				return _T("");
			if( pCombatSys->GetCurTargetID() == pRole->GetID() )
			{	
				return  VipNetBarMgr::Inst()->GetVipNetBarName(pRole->GetID());
			}
		}
	}
	return _T("");
}

void RoleHeadTipsFrame::UpdateLocalHPorMPProgress()
{
	static int nWidth = 75;
	static int nHeight = 8;

	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID(pLP) )
		return;
	if( pLP->GetRoleState(ERS_Stall) )
		return;

	
	//��ɫ����״̬ʱ�����º����
	if( pLP->IsInState( ES_Dead ) )
		return;

	ClientCamera* pCamera = GetWorldFrm()->GetCamera();
	if(!P_VALID(pCamera))
		return;
	const Vector3& lookFrom = pCamera->GetPos();

	Vector3 worldPos,srcPos;
	pLP->GetHeadPos(worldPos);

	srcPos = pCamera->WorldToScreenF(worldPos);
	srcPos.z *= 0.5f;

	if(srcPos.z<=0 || srcPos.z>=0.5f)
		return;

	POINT pt = pCamera->WorldToScreen(worldPos);
	pt.y -= 15;

	//Ѫ
	const int nMaxHp	= pLP->GetAttribute( ERA_MaxHP );
	const int nHp		= pLP->GetAttribute( ERA_HP );
	const int nPerHp	= (int)(((float)nHp)/((float)nMaxHp)*((float)nWidth));

	RECT destRC,srcRC;
	
	srcRC.left = srcRC.top = 0;
	srcRC.right = srcRC.left+nWidth;
	srcRC.bottom = nHeight;

	destRC.left = pt.x - nWidth/2;
	destRC.right = destRC.left + nPerHp;
	destRC.top = pt.y-nHeight/2;
	destRC.bottom = destRC.top+nHeight/2;

	IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResHP, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);

	//����
	pt.y += 8;
	const int nMaxMp	= pLP->GetAttribute( ERA_MaxMP );
	const int nMp		= pLP->GetAttribute( ERA_MP );
	const int nPerMp	= (int)(((float)nMp)/((float)nMaxMp)*((float)nWidth));

	destRC.left = pt.x - nWidth/2;
	destRC.right = destRC.left + nPerMp;
	destRC.top = pt.y-nHeight/2;
	destRC.bottom = destRC.top+nHeight/2;

	IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResMP, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);

}

void RoleHeadTipsFrame::UpdateOtherHPorMPProgress()
{
	static float nWidth = 75.0f;
	static float nHeight = 8.0f;

	static INT nClientWidth = Device()->GetClientWidth();
	static INT nClientHeight = Device()->GetClientHeight();


	DWORD dwMouseIntoID = GT_INVALID;
	CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
	if( P_VALID( pCombatSys ) )
		dwMouseIntoID = pCombatSys->GetCurTargetID();

	ClientCamera* pCamera = GetWorldFrm()->GetCamera();
	if(!P_VALID(pCamera))
		return;
	const Vector3& lookFrom = pCamera->GetPos();

	Role* pRole = RoleMgr::Inst()->FindRole(dwMouseIntoID);
	if( !P_VALID(pRole) )
		return;
	if( pRole->IS_KIND_OF(Player) )
	{
		Player* pPlayer = (Player*)pRole;
		if( pPlayer->GetRoleState(ERS_Stall) )
			return;
	}
	
	//��ɫ����״̬ʱ�����º����
	if( pRole->IsInState( ES_Dead ) )
		return;

	if( IS_PLAYER(dwMouseIntoID)  )
	{
		Vector3 worldPos,srcPos;
		pRole->GetHeadPos(worldPos);

		srcPos = pCamera->WorldToScreenF(worldPos);
		srcPos.z *= 0.5f;

		if(srcPos.z<=0 || srcPos.z>=0.5f)
			return;

		float dist = D3DXVec3Length(&(lookFrom-worldPos));
		float scale = 1.0f;
		if( dist > 2000.0f )
		{
			scale = 1.0f - (dist-2000.0f)*scalePerUnit;
			if( scale < 0.1f )
				scale = 0.1f;
		}

		POINT pt = pCamera->WorldToScreen(worldPos);
		pt.y = pt.y-(LONG)(15.0f*scale);

		//Ѫ
		const int nMaxHp	= pRole->GetAttribute( ERA_MaxHP );
		const int nHp		= pRole->GetAttribute( ERA_HP );
		const float nPerHp	= (((float)nHp)/((float)nMaxHp)*(nWidth));

		RECT destRC,srcRC;

		srcRC.left = srcRC.top = 0;
		srcRC.right = srcRC.left+(LONG)nWidth;
		srcRC.bottom = (LONG)nHeight;

		destRC.left = pt.x - (LONG)(nWidth*scale/2);
		destRC.right = destRC.left + (LONG)(nPerHp*scale);
		destRC.top = pt.y-(LONG)(nHeight*scale/2);
		destRC.bottom = destRC.top+(LONG)(nHeight*scale/2);

		if( destRC.left>=0 && destRC.top>=0 && destRC.right<=nClientWidth && destRC.bottom<=nClientHeight )
			IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResHP, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);

		//����
		pt.y = pt.y + (LONG)(8.0f*scale);
		const int nMaxMp	= pRole->GetAttribute( ERA_MaxMP );
		const int nMp		= pRole->GetAttribute( ERA_MP );
		const float nPerMp	= (((float)nMp)/((float)nMaxMp)*(nWidth));

		destRC.left = pt.x - (LONG)(nWidth*scale/2);
		destRC.right = destRC.left + (LONG)(nPerMp*scale);
		destRC.top = pt.y-(LONG)(nHeight*scale/2);
		destRC.bottom = destRC.top+(LONG)(nHeight*scale/2);

		if( destRC.left>=0 && destRC.top>=0 && destRC.right<=nClientWidth && destRC.bottom<=nClientHeight )
			IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResMP, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);
	}
	else if( IS_CREATURE(dwMouseIntoID) )
	{
		NPC* pNpc = (NPC*)pRole;
		const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pNpc->GetTypeID() );
		if( !P_VALID(pProto) )
			return;

		 if(pProto->eType==ECT_NPC || pProto->eType==ECT_Monster)
		 {
			 Vector3 worldPos,srcPos;
			 pRole->GetHeadPos(worldPos);

			 srcPos = pCamera->WorldToScreenF(worldPos);
			 srcPos.z *= 0.5f;

			 if(srcPos.z<=0 || srcPos.z>=0.5f)
				 return;

			 float dist = D3DXVec3Length(&(lookFrom-worldPos));
			 float scale = 1.0f;
			 if( dist > 2000.0f )
			 {
				 scale = 1.0f - (dist-2000.0f)*scalePerUnit;
				 if( scale < 0.1f )
					 scale = 0.1f;
			 }

			 POINT pt = pCamera->WorldToScreen(worldPos);
			 pt.y = pt.y-(LONG)(15.0f*scale);

			 //Ѫ
			 const int nMaxHp	= pRole->GetAttribute( ERA_MaxHP );
			 const int nHp		= pRole->GetAttribute( ERA_HP );
			 const float nPerHp	= (((float)nHp)/((float)nMaxHp)*(nWidth));

			 RECT destRC,srcRC;

			 srcRC.left = srcRC.top = 0;
			 srcRC.right = srcRC.left+(LONG)nWidth;
			 srcRC.bottom = (LONG)nHeight;

			 destRC.left = pt.x - (LONG)(nWidth*scale/2);
			 destRC.right = destRC.left + (LONG)(nPerHp*scale);
			 destRC.top = pt.y-(LONG)(nHeight*scale/2);
			 destRC.bottom = destRC.top+(LONG)(nHeight*scale/2);

			 if( destRC.left>=0 && destRC.top>=0 && destRC.right<=nClientWidth && destRC.bottom<=nClientHeight )
				 IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResHP, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);

			 //����
			 pt.y = pt.y + (LONG)(8.0f*scale);
			 const int nMaxMp	= pRole->GetAttribute( ERA_MaxMP );
			 const int nMp		= pRole->GetAttribute( ERA_MP );
			 const float nPerMp	= (((float)nMp)/((float)nMaxMp)*(nWidth));

			 destRC.left = pt.x - (LONG)(nWidth*scale/2);
			 destRC.right = destRC.left + (LONG)(nPerMp*scale);
			 destRC.top = pt.y-(LONG)(nHeight*scale/2);
			 destRC.bottom = destRC.top+(LONG)(nHeight*scale/2);

			 if( destRC.left>=0 && destRC.top>=0 && destRC.right<=nClientWidth && destRC.bottom<=nClientHeight )
				 IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResMP, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);
		 }
	}
}

DWORD RoleHeadTipsFrame::OnKey( DWORD dwID, BOOL bDown )
{
	if( bDown )
	{
		m_bShowHPorMP = !m_bShowHPorMP;
		GameSet::Inst()->Set(EGS_ShowHPMPBar, (m_bShowHPorMP ? TRUE : FALSE));
	}

	return 0;
}

const DWORD RoleHeadTipsFrame::GetRoleHostilityTips( Role* pRole )
{
	if( IS_PLAYER(pRole->GetID()) )
	{
		Player * pPlayer = static_cast<Player*>(pRole);
		if( P_VALID(pPlayer) )
			return pPlayer->GetAttribute( ERA_Hostility );
	}
	return 0;
}

const BOOL RoleHeadTipsFrame::GetRolePKMode( Role* pRole )
{
	if( IS_PLAYER(pRole->GetID()) )
	{
		Player * pPlayer = static_cast<Player*>(pRole);
		if( P_VALID(pPlayer) )
		{
			if( pPlayer->GetRoleState() & ERS_PK )
				return FALSE;
			else if( pPlayer->GetRoleState() & ERS_PKEX )
				return TRUE;
			else if( pPlayer->GetRoleState() & ERS_PeaceModel )
				return (BOOL)GT_INVALID;
		}
	}
	return GT_INVALID*2;
}

const tstring RoleHeadTipsFrame::GetRoleSpouse( Role* pRole )
{
    DWORD spouseId = GT_INVALID;

    if (IS_PLAYER(pRole->GetID()))
    {
        Player* pPlayer = static_cast<Player*>(pRole);
        if (P_VALID(pPlayer) && pPlayer->IsAttributeInited())
        {
            spouseId = pPlayer->GetPlayerLoverID();
            if (GT_INVALID == spouseId)
                return _T("");

            
            tstring spouseName = PlayerNameTab::Inst()->FindNameByID(spouseId);
            if (spouseName.empty())
                return _T("");

            tstringstream tss;
            tss << spouseName  << (0 == pPlayer->GetAvatarAtt().bySex ? g_StrTable[_T("MarriageTitleFemale")] : g_StrTable[_T("MarriageTitleMale")]);

            return tss.str();
            
        }
    }

    return _T("");
}

const INT RoleHeadTipsFrame::GetRoleVIPLevel(Role* pRole)
{
	if( IS_PLAYER(pRole->GetID()) )
	{
		Player * pPlayer = static_cast<Player*>(pRole);
		if( P_VALID(pPlayer) )
		{
			return pPlayer->GetAttribute(ERA_VipLevel);
		}
	}
	return 0;
}

Cool3D::Color4ub RoleHeadTipsFrame::GetRoleSpouseColor( Role* pRole )
{
    Color4ub color = Color4ub(255,255,128,192);		//Ĭ�Ϸ�ɫ

    if( IS_PLAYER(pRole->GetID()) )
    {
    }

    return color;
}

BOOL RoleHeadTipsFrame::IsRolePicTitle( Role* pRole )
{
    if (!P_VALID(pRole))
        return FALSE;

    if (!IS_PLAYER(pRole->GetID()))
        return FALSE;

    Player* pPlayer = static_cast<Player*>(pRole);
    DWORD dwTitleID = pPlayer->GetTitleID();
    return RoleTitleProtoData::Inst()->IsPicTitle(dwTitleID);

    return FALSE;
}
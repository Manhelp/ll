#include "StdAfx.h"
#include "StateDead_NPC.h"
#include "FSM_NPC.h"
#include "NPC.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "SkillProtoData.h"
#include "CombatSysUtil.h"
#include "EffectMgr.h"

StateDead_NPC::StateDead_NPC(void)
{}

StateDead_NPC::~StateDead_NPC(void)
{}

void StateDead_NPC::Active( tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole )
{

    m_lastCDTime=Kernel::Inst()->GetAccumTime();


	if(pRole->GetAniCtrl()!=NULL)
	{
		INT nDeadType = rand()%2;
		FLOAT fSkillRandPow = (FLOAT)(rand()%500);
		if (pNetCmd->dwID==pNetCmd->Crc32("NS_RoleDead"))
		{
			// ͨ�����ܲ鿴���Ŷ���
			tagNS_RoleDead* pCmd = (tagNS_RoleDead*)pNetCmd;
			const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(pCmd->dwMisc);
			if (P_VALID(pSkill))
			{
				switch(pSkill->dwBatkAction[pCmd->dwMisc2 % 5])
				{
				case 0:
					nDeadType = 0;
					break;
				case 1:
					nDeadType = 1;
					break;
				case 2:
					nDeadType = 1;
					break;
				default:
					nDeadType = 1;
					break;
				}
			}

			//--���Ŷ���
			Role* pSrcRole = RoleMgr::Inst()->FindRole(pCmd->dwSrcRoleID);
			//-- �ù��泯��ɫ
			if (P_VALID(pSrcRole))
			{
				pRole->SetYaw(CalcYaw(pSrcRole->GetPos() - pRole->GetPos()),false);
			}
			else// û�н�ɫ������������
				nDeadType = 0;

			// ֹͣ��������Ч
			EffectMgr::Inst()->StopBeAttackEffect(pRole->GetID(),pCmd->dwSrcRoleID,EHTC_Skill,pCmd->dwMisc,pCmd->dwSerial);
			
		}
		
		//--������Ч
		CombatSysUtil::Inst()->PlayRoleDeadSound(pRole);

		

		// ��������������3��ʱ��������Ч��
		////FLOAT fVIndex = max(0.3f,(pRole->GetRoleSize().x * pRole->GetRoleSize().y * pRole->GetRoleSize().z) / (lpSize.x * lpSize.y * lpSize.z));
		//if (fVIndex > 3.0f)
		//	nDeadType = 0;
		const tagCreatureProto* pProto = pRole->GetProto();
		if(P_VALID(pProto))
		{
			if (pProto->eLite >= ER_Boss)
			{
				nDeadType = 0;
			}

		}

		switch(nDeadType)
		{
		case 0:
			{
				//--��ʼ����ײ�����
				m_collider.Init(pRole->GetPos(), Vector3(0.0f, -1.0f, 0.0f) ,pRole->GetRoleSize(),pRole->GetHitFlyYSpeed(),0.0f);
				pRole->PlayTrack(_T("dead"),false);
				m_type = EDead_Nomal;
			}
			break;
		case 1:
			{
				Vector3 tmp;
				Yaw2Dir(pRole->GetYaw(),tmp);
				//--��ʼ����ײ��
				m_collider_fly.Init(pRole->GetPos(),//start pos
					-1.0f*tmp,//dir
					pRole->GetRoleSize(),//role size
					pRole->GetHitFlyXzSpeed() + fSkillRandPow,//xz speed
					pRole->GetHitFlyYSpeed(),//y speed
					2000.0f,//g
					2000.0f,//time out
					true,//terrain only?
					true);//ignore carrier?

				//--���ó���
				//pRole->SetYaw(CalcYaw(pRole->GetYaw()));
				pRole->PlayTrack(_T("dfly"),false);
				m_type = EDead_Fly;
			}
			break;
		}
		
		m_step=EStep_Drop;
	}
	else
	{
		m_startFadeTime=Kernel::Inst()->GetAccumTime();
		m_step=EStep_Fadeout;
	}

	//--��������󶨵���Ч
	EffectMgr::Inst()->ShowBindRoleEffect( pRole->GetID(), false );

	//--����������Ч
	if( pRole->IsAttributeInited() && pRole->IsMonster() && !EffectMgr::Inst()->IsEffectNumFull() && m_type != EDead_Fly)
	{
		AABBox box;
		pRole->GetSceneNode()->GetBox(box);
		Vector3 size = box.max-box.min;
		EffectMgr::Inst()->PlayEffect( pRole->GetPos(), 0.0f, _T("Txxy07"), &size, EffectMgr::EEST_YOutOnly );
	}
}

void StateDead_NPC::Update(NavMap* pNav,NPC* pRole)
{
    //--��ײ���
    Vector3 nowPos;
    float endTime;
    DWORD dwCarrierObjID;
    float cdCarrierTime;

	ENavResult result = ENR_Failed;
	switch(m_type)
	{
	case EDead_Nomal:
		 result = m_collider.Update(pNav->GetCollider(),m_lastCDTime,Kernel::Inst()->GetAccumTime()-m_lastCDTime,NULL,nowPos,endTime,dwCarrierObjID,cdCarrierTime,false);
		break;
	case EDead_Fly:
		{
			BitMap* pSpecialCango=NULL;
			pSpecialCango=pNav->GetNPCNavMap()->GetCanGoMap();
			result = m_collider_fly.Update(pNav->GetCollider(),pRole->GetPos(),m_lastCDTime,Kernel::Inst()->GetAccumTime()-m_lastCDTime,pSpecialCango,nowPos,endTime,dwCarrierObjID,false);
		}
		break;
	}
   // ENavResult result=m_collider.Update(pNav->GetCollider(),m_lastCDTime,Kernel::Inst()->GetAccumTime()-m_lastCDTime,NULL,nowPos,endTime,dwCarrierObjID,cdCarrierTime,false);
    m_lastCDTime=Kernel::Inst()->GetAccumTime();

    pRole->SetPos(nowPos);

    if (m_step == EStep_Drop)
    {
        if(result!=ENR_ToBeContinued)
        {
            m_step=EStep_FallDown;
        }
    }
	else if(m_step==EStep_FallDown)
	{
		//--����ˤ����Ч
		const DWORD dwFallMsg = _MakeFourCC(_T("fall"));
		const vector<DWORD>& msgCodes = pRole->GetMsgCodes();
		for( size_t nMsgCode = 0; nMsgCode < msgCodes.size(); nMsgCode++ )
		{
			if( dwFallMsg == msgCodes[nMsgCode] )
			{
				CombatSysUtil::Inst()->PlayFallSound(pRole->GetPos());
			}
		}
		if(m_type == EDead_Nomal && pRole->IsTrackEnd(_T("dead")))
		{
			m_step=EStep_Lie;
			m_startLieTime=Kernel::Inst()->GetAccumTime();
		}
		if(m_type == EDead_Fly && ENR_ToBeContinued != result && ENR_WillDrop != result)// pRole->IsTrackEnd(_T("dfly")))
		{
			m_step=EStep_Lie;
			m_startLieTime=Kernel::Inst()->GetAccumTime();
		}
	}
	else if(m_step==EStep_Lie)
	{
		float fTime = 0.0f;
		if (EDead_Nomal == m_type)
		{
			fTime = 5.0f;
		}
		else
		{
			fTime = 2.0f;
		}
		if(Kernel::Inst()->GetAccumTime()-m_startLieTime>fTime)// ʵ�屣��ʱ���Ϊ5��
		{
			m_startFadeTime=Kernel::Inst()->GetAccumTime();
			m_step=EStep_Fadeout;
		}
	}
	else if(m_step==EStep_Fadeout)
	{
		const float FadeOutTime=2.0f;
		float time=Kernel::Inst()->GetAccumTime()-m_startFadeTime;
		float alpha=1.0f-time/FadeOutTime;
		if(alpha<0)alpha=0;
		if(alpha>1)alpha=1;
		pRole->SetTransparent(alpha);
		if(alpha==0)
		{
			pRole->Free();
			m_step=EStep_Closed;
		}
	}
}

void StateDead_NPC::OnNetCmd( tagNetCmd* pNetCmd )
{}
#include "StdAfx.h"
#include "WorldPickerFrame.h"
#include "MapMgr.h"
#include "Role.h"
#include "RoleMgr.h"
#include "GroundItemMgr.h"
#include "NPC.h"
#include "CursorMgr.h"
#include "player.h"
#include "LocalPlayer.h"
#include "StallMgr.h"
#include "Render2D.h"
#include "CombatActionChecker.h"
#include "QuestMgr.h"
#include "QuestQuery.h"
#include "SDOAIgw.h"
#include "ClientCamera.h"
#include "WorldFrame.h"
#include "CombatSysUtil.h"

WorldPickerFrame::WorldPickerFrame(void) : 
m_trunk(this), 
m_eMousePointType( EMPT_None ),
m_dwMouseLastPointRoleID( GT_INVALID ),
m_dwMousePointRoleID( GT_INVALID ),
m_vMousePointScenePos( 0.0f, 0.0f, 0.0f ),
m_vMousePointWaterPos( 0.0f, 0.0f, 0.0f ),
m_pGroundItem( NULL ),
m_pStall( NULL ),
m_pScenePickDecal( NULL ),
m_bDecalAttached( false ),
m_pScenePickDecal2( NULL ),
m_bDecalAttached2( false ),
m_dwSelectedRoleID( GT_INVALID )
{
}

WorldPickerFrame::~WorldPickerFrame(void)
{
}

BOOL WorldPickerFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr,pFather,dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle( _T("OpenMap"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnOpenMap));
	m_pMgr->RegisterEventHandle( _T("CloseMap"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnCloseMap));
	m_pMgr->RegisterEventHandle( _T("tagShowScenePickDecalEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnShowScenePickDecalEvent));
	m_pMgr->RegisterEventHandle( _T("tagRolePickEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnRolePickEvent));

	return TRUE;
}

BOOL WorldPickerFrame::Destroy()
{
	m_pMgr->UnRegisterEventHandler( _T("OpenMap"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnOpenMap));
	m_pMgr->UnRegisterEventHandler( _T("CloseMap"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnCloseMap));
	m_pMgr->UnRegisterEventHandler( _T("tagShowScenePickDecalEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnShowScenePickDecalEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagRolePickEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&WorldPickerFrame::OnRolePickEvent));

	CursorMgr::Inst()->SetIdleCursor( ECIN_Default );

	return GameFrame::Destroy();
}

VOID WorldPickerFrame::Update()
{
	if( SDOAIgw::Inst()->GetSDOAFlag() )
	{
		if( SDOAIgw::Inst()->IsInIGW() )
			return;
	}

	if( NULL != m_pScenePickDecal && !m_bDecalAttached )
	{
		SceneGraph* pSG = MapMgr::Inst()->GetSceneGraph();
		if( NULL != pSG )
		{
			pSG->AttachDynamicNode( m_pScenePickDecal );
			m_bDecalAttached = true;
			m_pScenePickDecal->Close();
		}
	}

    if( NULL != m_pScenePickDecal2 && !m_bDecalAttached2 )
    {
        SceneGraph* pSG = MapMgr::Inst()->GetSceneGraph();
        if( NULL != pSG )
        {
            pSG->AttachDynamicNode( m_pScenePickDecal2 );
            m_bDecalAttached2 = true;
            m_pScenePickDecal2->Close();
        }
    }

	CursorMgr::Inst()->SetIdleCursor( ECIN_Default );
	if( m_pGUI->GetMousePoint() != m_pGUI->GetDesktop() )
		return;


	if( NULL == MapMgr::Inst()->GetSceneGraph() )
		return;

	CameraBase *pCamera = MapMgr::Inst()->GetSceneGraph()->GetCamera();
	if( NULL == pCamera )
		return;

	NavMap* pNav = MapMgr::Inst()->GetNavMap();
	if( NULL == pNav )
		return;
	NavCollider* pCollider = pNav->GetCollider();
	if( NULL == pCollider || !pCollider->IsLoaded() )
		return;

	const tagPoint& ptCursor = CursorMgr::Inst()->GetCursorPos();
	Ray ray;
	pCamera->GetMouseRay( ptCursor.x, ptCursor.y, ray );
	UpdataMousePoint( ray, m_pInput->IsKeyDown( DIK_LSHIFT ) );
	UpdataCursor();

	// �������
	if( m_pInput->IsMouseDownOnce(0) )
	{
		if( EMPT_Role == m_eMousePointType )									// ����ѡ�н�ɫ��Ϸ�¼�
		{
			// �Ѿ�������NPC����ѡ��
			Role* pRole = RoleMgr::Inst()->FindRole( m_dwMousePointRoleID );
			if( P_VALID( pRole ) && 
				!( pRole->IS_KIND_OF( NPC ) && pRole->IsDead() ) )
			{
				tagRolePickEvent evt( _T("tagRolePickEvent"), this );
				evt.eType = EWPT_LClick;
				evt.dwRoleID = m_dwMousePointRoleID;
				m_pMgr->SendEvent( &evt );
			}
		}		
		else if( EMPT_Scene == m_eMousePointType )								// ���ͳ��������Ϸ�¼�
		{
			tagScenePickEvent evt( _T("tagScenePickEvent"), this );
			evt.eType = EWPT_LClick;
			evt.vPickPos = m_vMousePointScenePos;
			m_pMgr->SendEvent( &evt );

            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            bool bCanGo = true;
            if (P_VALID(pLp) && pLp->IsAttributeInited())
            {
                //--���Կ��Ƿ����ֱ�ߵ���
                if(m_collider.IfCanGo(pNav->GetCollider(),pLp->GetPos(), evt.vPickPos, pLp->GetRoleSize(),NULL,100)!=ENR_Arrive)
                {
                    bCanGo = false;

                    // ����Ѱ·
                    if (m_pathFinder.Init(pLp->GetPos(), evt.vPickPos, pNav, pLp, true))
                    {
                        Vector3 point;
                        if(m_pathFinder.GetPathPoint(pNav,pLp,point))
                        {
                            if (m_collider.IfCanGo(pNav->GetCollider(),pLp->GetPos(), point, pLp->GetRoleSize(),NULL,100)==ENR_Arrive)
                            {
                                bCanGo = true;
                            }
                        }
                    }
                }
            }

            if (bCanGo)
			    ReBuildScenePickDecal( m_pScenePickDecal, m_vMousePointScenePos, ray );
            else
			{
				ReBuildScenePickDecal( m_pScenePickDecal2, m_vMousePointScenePos, ray );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Picked_Cannot_goto")]);
			}
                
		}
		else if( EMPT_Water == m_eMousePointType )								// ���ͳ��������Ϸ�¼�(ˮ��)
		{
			tagScenePickEvent evt( _T("tagScenePickEvent"), this );
			evt.eType = EWPT_LClick;
			evt.vPickPos = m_vMousePointWaterPos;
			evt.vPickPos.y = MapMgr::Inst()->GetNavMap()->GetCollider()->CalcSwimFootHeight( m_vMousePointWaterPos, RoleMgr::Inst()->GetLocalPlayer()->GetRoleSize().y, m_vMousePointWaterPos.y );
			m_pMgr->SendEvent( &evt );
			ReBuildScenePickDecal( m_pScenePickDecal, m_vMousePointWaterPos, ray, true );
		}
		else if( EMPT_GroundItem == m_eMousePointType )							// ���͵�����Ʒ�����Ϸ�¼�
		{
			tagGroundItemPickEvent evt( _T("tagGroundItemPickEvent"), this );
			evt.eType = EWPT_LClick;
			evt.pGroundItem = m_pGroundItem;
			m_pMgr->SendEvent( &evt );
		}
		else if( EMPT_Stall == m_eMousePointType )								// ���͵���̯λ�����Ϸ�¼�
		{
			tagRolePickEvent evt( _T("WorldEvent_StallPick"), this );
			evt.eType = EWPT_LClick;
			evt.dwRoleID = m_pStall->dwRoleID;
			m_pMgr->SendEvent(&evt);
		}
	}
	else if( m_pInput->IsMouseDownOnce(1) )
	{
		if( EMPT_Role == m_eMousePointType )									// �����Ҽ�ѡ�н�ɫ��Ϸ�¼�
		{
			// �Ѿ�������NPC����ѡ��
			Role* pRole = RoleMgr::Inst()->FindRole( m_dwMousePointRoleID );
			if( P_VALID( pRole ) && !( pRole->IS_KIND_OF( NPC ) && pRole->IsDead() ) )
			{
				if( IS_PLAYER(pRole->GetID() ) )
				{
					if( pRole->GetID() == RoleMgr::Inst()->GetLocalPlayerID() )
						return;
					
					tagRoleRClickEvent evt( _T("tagRoleRClickEvent"), this );
					evt.dwRoleID = m_dwMousePointRoleID;
					evt.pt = ptCursor;
					m_pMgr->SendEvent( &evt );
				}
			}
		}
	}
}

VOID WorldPickerFrame::UpdataMousePoint( const Ray& ray, const BOOL bIgnorePlayer )
{
	// �������
	m_eMousePointType = EMPT_None;
	// ���ϴε����ָ���ɫ����һ��
	m_dwMouseLastPointRoleID = m_dwMousePointRoleID;
	m_dwMousePointRoleID = GT_INVALID;
	m_vMousePointScenePos = Vector3( 0.0f, 0.0f, 0.0f );
	m_vMousePointWaterPos = Vector3( 0.0f, 0.0f, 0.0f );
	m_pGroundItem = NULL;
	m_pStall = NULL;

	tagMousePointObject mpo[EMPT_Num];

	// ʰȡ����
	NavMap* pNav = MapMgr::Inst()->GetNavMap();
	NavCollider* pCollider = pNav->GetCollider();
	mpo[EMPT_Scene].bValid = pCollider->RayCollideBoxAndTrn( ray, mpo[EMPT_Scene].vPos );
	m_vMousePointScenePos = mpo[EMPT_Scene].vPos;

	// ʰȡˮ��
	mpo[EMPT_Water].bValid = pCollider->RayCollideWater( ray, mpo[EMPT_Water].vPos );
	m_vMousePointWaterPos = mpo[EMPT_Water].vPos;

	// ʰȡ��ɫ
	DWORD dwPickRoleID = RoleMgr::Inst()->PickRole( ray, bIgnorePlayer );
	Role* pRole = RoleMgr::Inst()->FindRole( dwPickRoleID );
	if( P_VALID( pRole ) &&
		!( pRole->IS_KIND_OF( NPC ) && pRole->IsDead() ) )
	{
		mpo[EMPT_Role].bValid = true;
		mpo[EMPT_Role].vPos = pRole->GetPos();
		m_dwMousePointRoleID = dwPickRoleID;
	}
	
	if (m_dwMouseLastPointRoleID != m_dwMousePointRoleID)
	{
		tagMousePointRoleIDChangeEvent event(_T("tagMousePointRoleIDChangeEvent"), NULL);
		event.dwLastPointRoleID = m_dwMouseLastPointRoleID;
		event.dwCurPointRoleID = m_dwMousePointRoleID;
		m_pMgr->SendEvent(&event);
	}

	// ʰȡ������Ʒ
	tagGroundItem* pGroundItem = GroundItemMgr::Inst()->PickItem( ray );
	if( NULL != pGroundItem )
	{
		mpo[EMPT_GroundItem].bValid = true;
		mpo[EMPT_GroundItem].vPos = pGroundItem->space.GetPos();
		m_pGroundItem = pGroundItem;
	}
	
	// ʰȡ̯λ
	tagStall* pStall = StallMgr::Inst()->PickStall( ray );
	if (NULL != pStall )
	{
		mpo[EMPT_Stall].bValid = true;
		mpo[EMPT_Stall].vPos = pStall->space.GetPos();
		m_pStall = pStall;
	}

	// ȡ�����
	float fPickDistSq = FLOAT_MAX;
	for( int i = 0; i < EMPT_Num; i++ )
	{
		// �Ȳ�ȡˮ��
		if( EMPT_Water == i )
			continue; 
		if( mpo[i].bValid )
		{
			if (EMPT_Role == i)
			{
				if (ray.origin.x <= mpo[i].vPos.x)
					mpo[i].vPos.x -= pRole->GetRoleSize().x / 2;
				else
					mpo[i].vPos.x += pRole->GetRoleSize().x / 2;
				if (ray.origin.z <= mpo[i].vPos.z)
					mpo[i].vPos.z -= pRole->GetRoleSize().z / 2;
				else
					mpo[i].vPos.z += pRole->GetRoleSize().z / 2;

				mpo[i].vPos.y += pRole->GetRoleSize().y;
			}


			float fDistSq = Vec3DistSq( ray.origin, mpo[i].vPos );
			if( fDistSq < fPickDistSq)
			{
				m_eMousePointType = (EMousePointType)i;
				fPickDistSq = fDistSq;
			}
		}
	}
	// �㵽����ʱ����ȡһ��ˮ��
	if( EMPT_Scene == m_eMousePointType && mpo[EMPT_Water].bValid )
	{
		const float fDistWaterSq = Vec3DistSq( ray.origin, mpo[EMPT_Water].vPos );
		if( fDistWaterSq < fPickDistSq )
			m_eMousePointType = EMPT_Water;
	}
}

VOID WorldPickerFrame::UpdataCursor()
{
	if( EMPT_Role == m_eMousePointType )
	{
		Role* pRole = RoleMgr::Inst()->FindRole( m_dwMousePointRoleID );
		if( !P_VALID( pRole ) )
			return;
		if( pRole->IsDead() )
			return;

		// ���Թ�����Ŀ��
		if( CombatActionChecker::Inst()->IfCanAttack( pRole->GetID() ) )
		{
			CursorMgr::Inst()->SetIdleCursor( ECIN_Attack );
			return;
		}

		if( pRole->IS_STATIC_CLASS( NPC ) )
		{
			NPC* pNPC = (NPC*)pRole;
			const tagCreatureProto* pProto = pNPC->GetProto();
			if( !P_VALID( pProto ) )
				return;

			if( ECT_GameObject == pProto->eType )                               
			{
				if( EGOT_Gather == pProto->nType2 )								// �ɼ���
					CursorMgr::Inst()->SetIdleCursor( ECIN_Gather );
				else if( EGOT_Normal == pProto->nType2 )						// ��ͨ�ɽ�������
					CursorMgr::Inst()->SetIdleCursor( ECIN_NPCTalk );
				else if ( EGOT_Chest == pProto->nType2 )                        // ����
					CursorMgr::Inst()->SetIdleCursor( ECIN_DigBao );
				else if ( EGOT_CommonInves == pProto->nType2 || ( EGOT_QuestInves == pProto->nType2 && QuestMgr::Inst()->GetQuery()->IsInvesObject(pProto->dwTypeID) ) || EGOT_Remnant == pProto->nType2)
					CursorMgr::Inst()->SetIdleCursor( ECIN_NPCTalk );           // �ɵ������
				else if ( EGOT_QuestTalk == pProto->nType2 )
					CursorMgr::Inst()->SetIdleCursor( ECIN_NPCTalk );           // ����Ի�����
			}
			else if( ECT_NPC == pProto->eType )
			{
				if( EFNPCT_Shop == pProto->eFunctionType )                      // �̵�NPC
					CursorMgr::Inst()->SetIdleCursor( ECIN_NPCShop );
				else															// ����NPC
					CursorMgr::Inst()->SetIdleCursor( ECIN_NPCTalk );
			}
		}
	}
	else if( EMPT_GroundItem == m_eMousePointType )
	{
		CursorMgr::Inst()->SetIdleCursor( ECIN_PickGroundItem );
	}
	else if( EMPT_Stall == m_eMousePointType )
	{
		CursorMgr::Inst()->SetIdleCursor( ECIN_NPCShop );
	}
}

DWORD WorldPickerFrame::GetCurMousePointRoleID() const
{
	if( EMPT_Role == m_eMousePointType )
		return m_dwMousePointRoleID;
	else
		return GT_INVALID;
}

tagGroundItem* WorldPickerFrame::GetCurMousePointGroundItem() const
{
	if( EMPT_GroundItem == m_eMousePointType )
		return m_pGroundItem;
	else
		return NULL;
}

DWORD WorldPickerFrame::OnOpenMap( tagGameEvent* )
{
	m_pScenePickDecal = new SGEffectNode;
	m_pScenePickDecal->LoadRes( _T("data\\system\\Effect\\combat\\Txqt02.sfx") );


    m_pScenePickDecal2 = new SGEffectNode;
    m_pScenePickDecal2->LoadRes( _T("data\\system\\Effect\\combat\\Txqt02_1.sfx") );

	return 0;
}

DWORD WorldPickerFrame::OnCloseMap( tagGameEvent* )
{
	if( !m_bDecalAttached )
		SAFE_DEL( m_pScenePickDecal );
	m_pScenePickDecal = NULL;
	m_bDecalAttached = false;

    if( !m_bDecalAttached2 )
        SAFE_DEL( m_pScenePickDecal2 );
    m_pScenePickDecal2 = NULL;
    m_bDecalAttached2 = false;

	return 0;
}

DWORD WorldPickerFrame::OnShowScenePickDecalEvent( tagShowScenePickDecalEvent* pEvent )
{
	if( NULL != m_pScenePickDecal )
	{
		if( pEvent->bShow )
			m_pScenePickDecal->Play();
		else
			m_pScenePickDecal->Close();
	}
    if( NULL != m_pScenePickDecal2 )
    {
        if( !pEvent->bShow )
            m_pScenePickDecal2->Close();
    }
	return 0;
}

DWORD WorldPickerFrame::OnRolePickEvent( tagRolePickEvent* pEvent )
{
	if( EWPT_LClick == pEvent->eType )
		m_dwSelectedRoleID = pEvent->dwRoleID;

	return 0;
}

VOID WorldPickerFrame::ReBuildScenePickDecal( SGEffectNode* pSfNode, const Vector3& pickPos, const Ray& ray, const bool bWater/* = false*/ )
{
    if( NULL == pSfNode )
        return;

    vector<EffectBase*> vecEff;
    pSfNode->GetEffectArray( vecEff );
    if( 0 == vecEff.size() )
        return;

    Transform trans;
    trans.Translate(pickPos);
    pSfNode->SetWorldMat( trans.GetMatrix() );

    if( bWater )
    {
        for( size_t i = 0; i < vecEff.size(); i++ )
        {
            EffectBase* pEff = vecEff[i];
            if( !pEff->IS_STATIC_CLASS( EffectSceneDecal ) )
                continue;
            EffectSceneDecal* pDecal = (EffectSceneDecal*)pEff;
            pDecal->SetDir( -MathConst::AxisY, MathConst::AxisZ );
        }
        pSfNode->Play();
        return;
    }

    // ������������
    float fHalfDecalWidth = 0.0f;
    float fHalfDecalHeight = 0.0f;
    for( size_t i = 0; i < vecEff.size(); i++ )
    {
        EffectBase* pEff = vecEff[i];
        if( !pEff->IS_STATIC_CLASS( EffectSceneDecal ) )
            continue;
        EffectSceneDecal* pDecal = (EffectSceneDecal*)pEff;
        if( fHalfDecalWidth < pDecal->GetDecalSize().x * 0.5f )
            fHalfDecalWidth = pDecal->GetDecalSize().x * 0.5f;
        if( fHalfDecalHeight < pDecal->GetDecalSize().y * 0.5f )
            fHalfDecalHeight = pDecal->GetDecalSize().y * 0.5f;
    }

    // �����������򣬼���������ߴ�ֱ��ƽ�����������ε��ĸ�����,�����ĸ��㷢��4������ʰȡ�����е�4����
    const Vector3 vRef = MathConst::AxisY;
    float d = D3DXVec3Dot( &ray.origin, &ray.dir );
    Vector3 vRefPointInPlane = vRef + ray.dir * ( d - D3DXVec3Dot( &vRef, &ray.dir ) );
    Vector3 vTop, vRight;

    D3DXVec3Normalize( &vTop, &( vRefPointInPlane - ray.origin ) );
    vTop *= fHalfDecalHeight;
    D3DXVec3Cross( &vRight, &ray.dir, &vTop );
    D3DXVec3Normalize( &vRight, &vRight );
    vRight *= fHalfDecalWidth;

    Vector3 vTopRight = vTop + vRight;
    Vector3 vTopLeft = vTop - vRight;

    Ray rayPick[4];
    Vector3 vPickPos[4];
    bool bCollideOK[4];
    rayPick[0].origin = ray.origin + vTopLeft;
    rayPick[1].origin = ray.origin + vTopRight;
    rayPick[2].origin = ray.origin - vTopLeft;
    rayPick[3].origin = ray.origin - vTopRight;

    NavMap* pNav = MapMgr::Inst()->GetNavMap();
    for( int i = 0; i < 4; i++ )
    {
        rayPick[i].dir = ray.dir;
        bCollideOK[i] = pNav->GetCollider()->RayCollideBoxAndTrn( rayPick[i], vPickPos[i] );
    }

    // ����4����������������ķ���
    Vector3 vDecalNormal( 0.0f, 0.0f, 0.0f );
    for( int i = 0; i < 4; i++ )
    {
        int nNext = ( i + 1 ) % 4;
        if( !bCollideOK[i] || !bCollideOK[nNext] )
            continue;

        // Ϊ�˼�������Ť��������ʹ�ò����������ĵ�ľ�����Ʒ���
        float fLen0 =  D3DXVec3Length( &( vPickPos[i] - pickPos ) );
        float fLen1 =  D3DXVec3Length( &( vPickPos[nNext] - pickPos ) );
        float fInvLen = 1.0f / max( fLen0, fLen1 );

        Vector3 vNormal;
        D3DXVec3Cross( &vNormal, &( vPickPos[i] - pickPos ), &( vPickPos[nNext] - pickPos ) );
        D3DXVec3Normalize( &vNormal, &vNormal );
        vNormal *= fInvLen;
        vDecalNormal += vNormal;
    }
    D3DXVec3Normalize( &vDecalNormal, &vDecalNormal );

    for( size_t i = 0; i < vecEff.size(); i++ )
    {
        EffectBase* pEff = vecEff[i];
        if( !pEff->IS_STATIC_CLASS( EffectSceneDecal ) )
            continue;
        EffectSceneDecal* pDecal = (EffectSceneDecal*)pEff;
        pDecal->SetDir( -vDecalNormal, vTop );
    }

    pSfNode->Play();
}
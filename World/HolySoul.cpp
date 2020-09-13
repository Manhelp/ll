
#include "StdAfx.h"
#include "container_template.h"
#include "../WorldDefine/container_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_combat.h"

#include "../WorldDefine/msg_yuanshen.h"
#include "../ServerDefine/msg_holySoul.h"
#include "container_restrict.h"
#include "HolySoul_Container.h"
#include "att_res.h"
#include "world.h"
#include "unit.h"
#include "role.h"
#include "db_session.h"
#include "event_mgr.h"
#include "HolySoul.h"



HolySoul::HolySoul(Role* pRole):m_pRole(pRole),m_bIsActivated(false),m_eHolySoulState(EHSS_Asleep),m_dwInitTime(0),m_bySecondTime(0),m_bSaveToDB(false)
{
	memset(&m_nEquipAtt,0,sizeof(m_nEquipAtt));
	Init();
}

HolySoul::~HolySoul()
{
   m_pRole = NULL;
}


DWORD  HolySoul::Init(void)
{
	EventMgr<HolySoul>::RegisterEventFunc(EVT_SetSoul_State,&HolySoul::SetState);
	return E_Success;
}


VOID   HolySoul::Update()
{
	if ( !IsActivated() )
	{
		return ;
	}
	CHECK_TIME();
	EventMgr<HolySoul>::Update();

	//ÿ��ִ��һ��
	if ( timeGetTime() - m_dwInitTime >= 1000)
	{
		OnSecond();
		m_dwInitTime = timeGetTime();

	}	
}

VOID   HolySoul::OnMinute()
{
	 	
}

VOID   HolySoul::OnSecond()
{
	m_bySecondTime++;
	m_bSaveToDB = false;
	if ( 20 == m_bySecondTime )
	{
		m_bSaveToDB = true;
	}
	if ( m_bySecondTime >=60 )
	{
		OnMinute();
		m_bySecondTime = 0;
		
	}
	if ( 0 == ((m_bySecondTime+ADD_SOULVAL_TIME) % ADD_SOULVAL_TIME) )
	{

		if ( !IsHolySoulAwake() )
		{
			OnChangeAttVal(EHEAT_SoulValue,ADD_SOULVAL_PER_MINUTE,true,m_bSaveToDB);
		}
	}

	if ( IsHolySoulAwake() )
	{
		OnChangeAttVal(EHEAT_SoulValue,-DEC_SOULVAL_PER_SECOND,true,m_bSaveToDB);
		OnChangeAttVal(EHEAT_Covalue,COVALUE_ONTIME_PER_SECOND,true,m_bSaveToDB);
	}

// 	if ( GetHolySoulAtt(EHEAT_SoulValue) <= 0 )
// 	{
// 		m_pRole->RemoveBuff(HOLYSOUL_BUFFID, FALSE);
// 	}
}


//--------------------------------------------------------------------------------------------
// ���Ԫ��װ������
//--------------------------------------------------------------------------------------------
UINT   HolySoul::GetHolySoulAtt(EHolyEquipAttType eEquipAttType)const
{
	if ( eEquipAttType < EHEAT_Demage || eEquipAttType >= EHEAT_End)
	{
		IMSG(_T("Ԫ��װ�����Գ�����Χ: eEquipAttType = %u\n"),(DWORD)eEquipAttType);
		ILOG->Write(_T("Ԫ��װ�����Գ�����Χ: eEquipAttType = %u\n"),(DWORD)eEquipAttType);
		return 0;
	}
	UINT nRet = 0;
// 	switch ( eEquipAttType )
// 	{
// 	case EHEAT_Demage:
// 		nRet = m_nEquipAtt[EHEAT_Demage] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage];
// 		break;
// 	case EHEAT_Hit:
// 		nRet = m_nEquipAtt[EHEAT_Hit] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit];
// 		break;
// 	case EHEAT_Crit:
// 		nRet = m_nEquipAtt[EHEAT_Crit] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit];
// 		break;
// 	case EHEAT_CritAmount:
// 		nRet = m_nEquipAtt[EHEAT_CritAmount] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount];
// 		break;
// 	case EHEAT_Exattack:
// 		nRet = m_nEquipAtt[EHEAT_Exattack] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack];
// 		break;
// 	case EHEAT_InExattack:
// 		nRet = m_nEquipAtt[EHEAT_InExattack] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack];
// 		break;
// 	case EHEAT_Accurate:
// 		nRet = m_nEquipAtt[EHEAT_Accurate] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate];
// 		break;
// 	case EHEAT_Exdamage:
// 		nRet = m_nEquipAtt[EHEAT_Exdamage] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage];
// 		break;
// 	case EHEAT_Toughness:
// 		nRet = m_nEquipAtt[EHEAT_Toughness] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness];
// 		break;
// 	case EHEAT_Morale:
// 		nRet = m_nEquipAtt[EHEAT_Morale] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale];
// 		break;
// 	case EHEAT_Covalue:
// 		nRet = m_nEquipAtt[EHEAT_Covalue] + m_HolySoulInfo.SoulAttEx.nCurCovalue;
// 		break;
// 	case EHEAT_SoulValue:
// 		nRet = m_nEquipAtt[EHEAT_SoulValue] + m_HolySoulInfo.SoulAttEx.nCurSoulValue;
// 		break;
// 	case EHEAT_MaxSoulValue:
// 		nRet = m_nEquipAtt[EHEAT_MaxSoulValue] + m_HolySoulInfo.SoulAttEx.nMaxSoulValue;
// 		break;
// 	default:
// 		nRet = 0;
// 		break;
// 	}

	return nRet;
}

//--------------------------------------------------------------------------------------------
// Ԫ�񼤻��ʼ��
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::Init(const tagSoulAttProto* pHolySoulInfo)
{
	if (!P_VALID(pHolySoulInfo))
	{
		return EHolySoul_InitFailed;
	}
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv]     = pHolySoulInfo->n8RebornLv;   //Ԫ��ת���ȼ�
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel]    = pHolySoulInfo->n8SoulLevel;  //Ԫ��ȼ� 150 
// 
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage]       = pHolySoulInfo->nDamage;      //Ԫ��������˺�
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit]          = pHolySoulInfo->nHit;         //��������
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit]         = pHolySoulInfo->nCrit;		 //��������	
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount]   = pHolySoulInfo->nCritAmount;  //����������
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack]     = pHolySoulInfo->nExattack;	 //�⹦����
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack]     = pHolySoulInfo->nInattack;	 //�ڹ�����
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate]      = pHolySoulInfo->nAccrate;	 //��׼
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage]     = pHolySoulInfo->nExdamage;	 //�����˺�
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness]    = pHolySoulInfo->nToughness;	 //��������
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale]       = pHolySoulInfo->n8Morale;	 //ʿ��	
// 
//     m_HolySoulInfo.SoulAttEx.dwSoulID                  = pHolySoulInfo->dwID;		 //Ԫ��ID	
//     m_HolySoulInfo.SoulAttEx.n64LevelExp               = 0;  //Ԫ����ȼ�
// 	m_HolySoulInfo.SoulAttEx.n64MaxExp                 = pHolySoulInfo->n64UpLvExp;   //�ܵ���������
// 	m_HolySoulInfo.SoulAttEx.nCurCovalue               = pHolySoulInfo->n8Covalue;   //��ǰ��Ĭ��ֵ
//     m_HolySoulInfo.SoulAttEx.nMaxCovalue               = pHolySoulInfo->n8Covalue;   //Ĭ��ֵ��ǰ���ֵ
//     m_HolySoulInfo.SoulAttEx.nCurSoulValue             = pHolySoulInfo->dwSoulValue; //��ǰ��Ԫ��ֵ
//     m_HolySoulInfo.SoulAttEx.nMaxSoulValue             = pHolySoulInfo->dwSoulValue; //��ǰ��Ԫ��ֵ
// 
// 	m_eHolySoulState   =  EHSS_Asleep;
// 	m_bIsActivated     =  true;

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// Ԫ�����ݿ��ʼ��
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::Init(const tagHolySoulInfoEx* pHolySoulInfoEx,INT nOffHour/*����Сʱ*/)
{
// 	if ( !P_VALID( pHolySoulInfoEx ) )
// 	{
// 		return EHolySoul_InitFailed;
// 	}
// 
// 	//��ȡԪ������
// 	const tagSoulAttProto* pSoulAttProto = g_attRes.GetSoulAttProto(pHolySoulInfoEx->dwSoulID);
// 	if ( !P_VALID(pSoulAttProto) )
// 	{
// 		return EHolySoul_NoExist;
// 	}
// 
// 	m_bIsActivated                                     = true;
// 	m_eHolySoulState								   = pHolySoulInfoEx->eHolySoulState;         //Ԫ��״̬	
//     
// 	memcpy(&m_HolySoulInfo.SoulAtt,&pHolySoulInfoEx->HolySoulAtt,sizeof(tagSoulAtt));             //����Ԫ������
// 
// 	m_HolySoulInfo.SoulAttEx.dwSoulID                  = pHolySoulInfoEx->dwSoulID;				  //Ԫ��ID	
// 	m_HolySoulInfo.SoulAttEx.n64LevelExp               = pHolySoulInfoEx->n64CurLevelExp;		  //Ԫ����ȼ�
// 	m_HolySoulInfo.SoulAttEx.n64MaxExp                 = pSoulAttProto->n64UpLvExp;				  //�ܵ���������
// 	m_HolySoulInfo.SoulAttEx.nCurCovalue               = pHolySoulInfoEx->nCurCovalue;			  //��ǰ��Ĭ��ֵ
// 	m_HolySoulInfo.SoulAttEx.nMaxCovalue               = pSoulAttProto->n8Covalue;				  //Ĭ��ֵ��ǰ���ֵ
// 	m_HolySoulInfo.SoulAttEx.nCurSoulValue             = pHolySoulInfoEx->nCurSoulValue;		  //��ǰ��Ԫ��ֵ
// 	m_HolySoulInfo.SoulAttEx.nMaxSoulValue             = pSoulAttProto->dwSoulValue;			  //��ǰ����Ԫ��ֵ
// 
// 	//����1Сʱ���Ͽ۳���Ӧ��Ԫ��Ĭ��
// 	if ( nOffHour > 1)
// 	{
// 		OnChangeAttVal(EHEAT_Covalue,-COVALUE_OFFTIME_PER_HOUR*nOffHour,false);
// 	}




	return E_Success;
}

//--------------------------------------------------------------------------------------------
// ��ע����
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::PourExp(INT64 n64ExpLvUp)
{
//     if ( m_pRole->GetCurLevelExp() < n64ExpLvUp || n64ExpLvUp <= 0)
//     {
// 		return GT_INVALID;
//     }
// 	if ( EHSS_Awake == GetState() )
// 	{
// 		return EHolySoul_AwakePourExpFaied;
// 	}
// 	//�ж��ȼ�����
// 	if ( m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel] >= HOLYSOUL_MAX_LEVEL )
// 	{
// 		return EHolySoul_LevelUpMaxLvl;
// 	}
// 
// 	INT64 n64ExpLvUpTemp = n64ExpLvUp;
// 	bool bSendToClient = false;
// 	while( n64ExpLvUpTemp > 0 )
// 	{
//          DWORD dwRet = ExpChange(n64ExpLvUpTemp,bSendToClient);
// 		 if ( E_Success != dwRet )
// 		 {
// 			 break;
// 		 }
// 	}
//     m_pRole->ExpChange(-(n64ExpLvUp-n64ExpLvUpTemp));
// 
// 	if ( bSendToClient )
// 	{
// 		//���µ����ݿ�
// 		SaveHolySoulInfoToDB(ESaveType_Update);
// 		//Ԫ��������ʾ
// 		tagNS_HolySoulLvUp msg;
// 		m_pRole->SendMessage(&msg,msg.dwSize);
// 		//ͬ���ͻ���Ԫ����Ϣ�ı�
// 		SendHolySoulAttToClient();
// 	}
// 	else
// 	{
// 		//���µ����ݿ�
// 		SaveHolySoulInfoToDB(ESaveType_ExpAttUpdate);
// 	}
   
	return E_Success;
}

//-------------------------------------------------------------------------------------------
// Ԫ��Ԫ����ı�
//-------------------------------------------------------------------------------------------
DWORD	HolySoul::ExpChange(INT64& n64ExpMod, bool& bSend)
{
//       const tagSoulAttProto* pSoulAttProto = g_attRes.GetSoulAttProto(GetID());
// 	  if (!P_VALID(pSoulAttProto))
// 	  {
// 		  return EHolySoul_NoExist;
// 	  }
// 	  
// 	  INT64 n64Exp = pSoulAttProto->n64UpLvExp - GetCurrExp();
// 	  if ( n64Exp > n64ExpMod)
// 	  {
//           m_HolySoulInfo.SoulAttEx.n64LevelExp += n64ExpMod;
// 		  n64ExpMod = 0;
// 	  }
// 	  else
// 	  {
//            n64ExpMod -= n64Exp;
// 		   DWORD dwErrorID = LevelUpSoul();
// 		   bSend = true;
// 		   if (E_Success != dwErrorID)
// 		   {
// 			   return dwErrorID;
// 		   }
// 	  }
	  return E_Success;	   
}

//-------------------------------------------------------------------------------------------
// ����Ԫ��Ԫ��״̬
//-------------------------------------------------------------------------------------------
VOID  HolySoul::SetState(DWORD dwSenderID, LPVOID pEventMessage)
{
	if ( !IsActivated())
	{
		return ;
	}

	if ( EHSS_Awake == (EHolySoulState)dwSenderID)
	{
		CallHolySoul((EHolySoulState)dwSenderID);
	}
	else if ( EHSS_Asleep == (EHolySoulState)dwSenderID )
	{
		CancelCallSoul((EHolySoulState)dwSenderID);
	}
	
	
}

//-------------------------------------------------------------------------------------------
// Ԫ��Ԫ��ֵ�ָ�
//-------------------------------------------------------------------------------------------
VOID  HolySoul::OnChangeSoulVal(const INT& nVal,bool bSend )
{
	if ( 0 == nVal) return ;
	bool bIsChangeVal = true;
	INT nOriginVal = m_HolySoulInfo.SoulAttEx.nCurSoulValue;

	m_HolySoulInfo.SoulAttEx.nCurSoulValue += nVal;
	if ( m_HolySoulInfo.SoulAttEx.nCurSoulValue > m_HolySoulInfo.SoulAttEx.nMaxSoulValue ) 
		m_HolySoulInfo.SoulAttEx.nCurSoulValue = m_HolySoulInfo.SoulAttEx.nMaxSoulValue;
	if ( m_HolySoulInfo.SoulAttEx.nCurSoulValue < 0) m_HolySoulInfo.SoulAttEx.nCurSoulValue = 0;
    if ( nOriginVal == m_HolySoulInfo.SoulAttEx.nCurSoulValue)
    {
		bIsChangeVal = false;
    }

	if ( bIsChangeVal )
	{
		if ( bSend )
		{
			tagNS_HolySoulAttChange msg;
			msg.dwVal = m_HolySoulInfo.SoulAttEx.nCurSoulValue;
			msg.eHolyEquipAttType = EHEAT_SoulValue;
			m_pRole->SendMessage(&msg,msg.dwSize);
		}
		
		SaveHolySoulInfoToDB(ESaveType_AttInfoUpdate,NULL,ESoulAttEx_SoulVal,m_HolySoulInfo.SoulAttEx.nCurSoulValue);
		
		
	}


}

//-------------------------------------------------------------------------------------------
// �ٻ�Ԫ��
//-------------------------------------------------------------------------------------------
VOID  HolySoul::CallHolySoul(EHolySoulState eHolySoulState)
{
	if ( EHSS_Awake == GetState())	return ;

	if ( GetHolySoulAtt(EHEAT_SoulValue) < CALLSOUL_MIN_SOULVAL)
	{
		return ;
	}
	
	m_eHolySoulState = eHolySoulState;	

	tagNS_RemoteHolySoulAttChange msg;
	msg.dwSoulID = GetID();
	msg.dwRoleID = m_pRole->GetID();
	msg.eOptType = ESAOType_Call;
	if ( m_pRole->GetMap() )
	{
		m_pRole->GetMap()->SendBigVisTileMsg((Unit*)m_pRole, &msg, msg.dwSize);

	}
}

//-------------------------------------------------------------------------------------------
// ȡ���ٻ�Ԫ��
//-------------------------------------------------------------------------------------------
VOID  HolySoul::CancelCallSoul(EHolySoulState eHolySoulState)
{
	if ( EHSS_Asleep == GetState())	return ;

	m_eHolySoulState = eHolySoulState;	

	tagNS_RemoteHolySoulAttChange msg;
	msg.dwSoulID = GetID();
	msg.dwRoleID = m_pRole->GetID();
	msg.eOptType = ESAOType_CancelCall;
	if ( m_pRole->GetMap() )
	{
		m_pRole->GetMap()->SendBigVisTileMsg((Unit*)m_pRole, &msg, msg.dwSize);

	}
}

//--------------------------------------------------------------------------------------------
// ����Ԫ��
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::ActivateSoul(bool bSend)
{
	
     DWORD dwSoulID  = HOLYSOUL_INITID;
	 const tagSoulAttProto* pSoulAttProto = g_attRes.GetSoulAttProto(dwSoulID);
	 if ( !P_VALID(pSoulAttProto) )
	 {
        return EHolySoul_NoExist;
	 }

	 Init(pSoulAttProto);

	 SaveHolySoulInfoToDB(ESaveType_Insert);

	 if ( bSend )
	 {
		  SendHolySoulAttToClient();
	 }
	 
	 HolyChangeRoleAtt();
     return E_Success;
}



//--------------------------------------------------------------------------------------------
// ����Ԫ��
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::LevelUpSoul()
{
//     if ( !m_bIsActivated )
//     {
// 		return EHolySoul_NotActivated;
//     }
// 	//δ�ж��ȼ�����
// 	if ( m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel] >= HOLYSOUL_MAX_LEVEL )
// 	{
// 		return EHolySoul_LevelUpMaxLvl;
// 	}
// 
//     //Ԫ��ȼ���ת����Ԫ��ID�Ĺ�ϵ
// 	//Ԫ��ID =  ת�� + Ԫ��ȼ�
// 	DWORD dwOldID = m_HolySoulInfo.SoulAttEx.dwSoulID;
// 	DWORD dwNewID = m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel]+1;
//     const tagSoulAttProto* pOldSoulAttProto = g_attRes.GetSoulAttProto(dwOldID);
// 	if ( !P_VALID(pOldSoulAttProto) )
// 	{
// 
// 		return EHolySoul_NoExist;
// 	}
// 	const tagSoulAttProto* pNewSoulAttProto = g_attRes.GetSoulAttProto(dwNewID);
// 	if ( !P_VALID(pNewSoulAttProto) )
// 	{
// 		return EHolySoul_NextLevelNoExist;
// 	}
// 	DWORD dwErrorCode = ProcHolySoulAtt(pNewSoulAttProto,pOldSoulAttProto);
// 	if ( E_Success != dwErrorCode)
// 	{
// 		return dwErrorCode;
// 	}
// 
// 	HolyChangeRoleAtt(pNewSoulAttProto,pOldSoulAttProto,false,true);
    
    return E_Success;
}

//--------------------------------------------------------------------------------------------
// ����Ԫ��װ��
//--------------------------------------------------------------------------------------------
DWORD HolySoul::EquipHolySoul(INT64 n64Serial, DWORD ePosDst)
{
	if ( !P_VALID(m_pRole) )
	{
		return EHolySoul_EquipHolySoulFailed;
	}

	if( m_pRole->IsInState(ES_PvPAttack) || m_pRole->IsInState(ES_PvPDefence))
	{
		return EHolySoul_EquipHolySoulFailed;
	}

	// ��ô���װ��
	tagItem *pItem = m_pRole->GetItemMgr().GetBagItem(n64Serial);
	if(!P_VALID(pItem))
	{
		return EHolySoul_BagItemNotFound;
	}

	// �ж���װ����Ʒ�Ƿ�Ϊװ��
	if(!MIsEquipment(pItem->dwTypeID))
	{
		return EHolySoul_NotEquipment;
	}

	tagEquip *pEquip = (tagEquip *)pItem;
	
	// ����Ƿ����װ������
	DWORD dwErrorCode = CanEquip(pEquip, ePosDst);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}
	// ��õ�ǰװ��λ�õ�װ��
	tagEquip *pEquipOld = m_pRole->GetItemMgr().GetHolySoulBar().GetItem((INT16)ePosDst);

	// ��װ
	dwErrorCode = m_pRole->GetItemMgr().GetHolySoulBar().Equip(m_pRole->GetItemMgr().GetBag(),n64Serial,(EEquipPos)ePosDst);
	if(E_Success == dwErrorCode)
	{
		ProcEquipEffect(pEquip, pEquipOld, (INT16)ePosDst, FALSE, TRUE);
	}
	else
	{
		return dwErrorCode;
	}

    return E_Success;
}

//--------------------------------------------------------------------------------------------
// ���� -- Ŀ��λ��ӦΪ�գ����տͻ���Ӧ������װ����Ϣ
//--------------------------------------------------------------------------------------------
DWORD HolySoul::UnEquipHolySoul(INT64 n64Serial, INT16 n16IndexDst)
{

	if ( !P_VALID(m_pRole) )
	{
		return EHolySoul_UnEquipHolySoulFailed;
	}

	// ���Ŀ��λ���Ƿ�Ϊ�գ����վ�ֱ�ӷ���
	if(n16IndexDst != GT_INVALID && !m_pRole->GetItemMgr().IsBagOneSpaceFree(n16IndexDst))
	{
		return GT_INVALID;
	}

	// ��ô���װ��
	tagEquip *pEquipOld = m_pRole->GetItemMgr().GetHolySoulBarEquip(n64Serial);
	if(!P_VALID(pEquipOld))
	{
		return EHolySoul_EquipNotExist;
	}

	INT16 n16IndexOld = pEquipOld->n16Index;
	// ��װ
	if(GT_INVALID == n16IndexDst)
	{
		n16IndexDst = m_pRole->GetItemMgr().GetBagOneFreeSpace();
		if(GT_INVALID == n16IndexDst)
		{
			return EHolySoul_BagNoEnoughSpace;
		}
	}

	DWORD dwErrorCode = m_pRole->GetItemMgr().GetHolySoulBar().Unequip(n64Serial,m_pRole->GetItemMgr().GetBag(), n16IndexDst);
	if( E_Success == dwErrorCode )
	{
		ProcEquipEffect(NULL, pEquipOld, n16IndexOld, FALSE, TRUE);
	}
	else
	{
		return dwErrorCode;
	}
    return E_Success;
}


//--------------------------------------------------------------------------------------------
// �ܷ�װ��
//--------------------------------------------------------------------------------------------
DWORD	HolySoul::CanEquip(tagEquip* pEquip, DWORD ePosDst)
{

// 	if ( !MIsHolySoulEquipType(pEquip))
// 	{
// 		return EHolySoul_NotHolySoulEquip;
// 	}
// 
// 	const tagEquipProto* pEquipProto = pEquip->pEquipProto;
// 	if( !P_VALID(pEquipProto) )
// 	{
// 		return GT_INVALID;
// 	}
// 	
// 	bool bIsCanEquip = true;
// 	switch( ePosDst )
// 	{
// 	case EHEFI_TianChong:
// 		if (  EITE_TianChong != pEquipProto->eTypeEx)
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_LingHui:
// 		if ( EITE_LingHui != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_QiPo:
// 		if ( EITE_QiPo != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_LiPo:
// 		if ( EITE_LiPo != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_ZhongShu:
// 		if ( EITE_ZhongShu != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_JingPo:
// 		if ( EITE_JingPo != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_YingPo:
// 		if ( EITE_YingPo != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_TianHun:
// 		if ( EITE_TianHun != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_DiHun:
// 		if ( EITE_DiHun != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	case EHEFI_MingHun:
// 		if ( EITE_MingHun != pEquipProto->eTypeEx )
// 		{
// 			bIsCanEquip = false;
// 		}
// 		break;
// 	default:
// 		bIsCanEquip = false;
// 		break;
// 
// 	}
// 
// 	if ( !bIsCanEquip )
// 	{
// 		return EHolySoul_EquipPosNotExist;
// 	}
// 	// ��ȥbyte��ɴ����Ŀ�����
// 	INT byMinLevel = 1;
// 	INT byMaxLevel = 0x7fffffff;
// 	
// 	byMinLevel = pEquip->equipSpec.byMinUseLevel & 0xFF;
// 	byMaxLevel = pEquip->equipSpec.byMaxUseLevel & 0xFF;
// 	
// 	if( EESA_LevelLim_None != pEquip->equipSpec.bySpecAtt )
// 	{
// 		if( EESA_LevelLim_Simple == pEquip->equipSpec.bySpecAtt )
// 		{
// 			byMinLevel -= 5;
// 		}
// 		else if( EESA_LevelLim_Fine == pEquip->equipSpec.bySpecAtt )
// 		{
// 			byMinLevel -= 10;
// 		}
// 		if( byMinLevel < 0 )
// 			byMinLevel = 0;
// 		if( m_pRole->GetLevel() < byMinLevel || ( m_pRole->GetLevel() > byMaxLevel && byMaxLevel > 0 ) )
// 		{
// 			return E_Item_LevelLimit;
// 		}
// 	}
    return E_Success;
}


//-------------------------------------------------------------------------------------------
// ������Ԫ������
//-------------------------------------------------------------------------------------------
DWORD HolySoul::ProcHolySoulAtt(const tagSoulAttProto* pNewHolySoul, const tagSoulAttProto* pOldHolySoul, bool bSend )
{
// 	if ( !P_VALID(pNewHolySoul) || !P_VALID(pOldHolySoul) )
// 	{
// 		return EHolySoul_NoExist;
// 	}
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv]     = pNewHolySoul->n8RebornLv;   //Ԫ��ת���ȼ�
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel]    = pNewHolySoul->n8SoulLevel;  //Ԫ��ȼ� 150 
// 	
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage]      += (pNewHolySoul->nDamage - pOldHolySoul->nDamage);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit]         += (pNewHolySoul->nHit - pOldHolySoul->nHit);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit]        += (pNewHolySoul->nCrit - pOldHolySoul->nCrit);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount]  += (pNewHolySoul->nCritAmount - pOldHolySoul->nCritAmount);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack]    += (pNewHolySoul->nExattack - pOldHolySoul->nExattack);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack]    += (pNewHolySoul->nInattack - pOldHolySoul->nInattack);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate]     += (pNewHolySoul->nAccrate - pOldHolySoul->nAccrate);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage]    += (pNewHolySoul->nExdamage - pOldHolySoul->nExdamage);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness]   += (pNewHolySoul->nToughness - pOldHolySoul->nToughness);
//     m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale]      += (pNewHolySoul->n8Morale - pOldHolySoul->n8Morale);
// 
// 
// 	m_HolySoulInfo.SoulAttEx.dwSoulID                  = pNewHolySoul->dwID;		 //Ԫ��ID	
// 	m_HolySoulInfo.SoulAttEx.n64LevelExp               = 0;  //Ԫ����ȼ�
// 	m_HolySoulInfo.SoulAttEx.n64MaxExp                 = pNewHolySoul->n64UpLvExp;   //�ܵ���������
// 	m_HolySoulInfo.SoulAttEx.nCurCovalue               = pNewHolySoul->n8Covalue;   //��ǰ��Ĭ��ֵ
// 	m_HolySoulInfo.SoulAttEx.nMaxCovalue               = pNewHolySoul->n8Covalue;   //Ĭ��ֵ��ǰ���ֵ
// 	m_HolySoulInfo.SoulAttEx.nCurSoulValue             = pNewHolySoul->dwSoulValue; //��ǰ��Ԫ��ֵ
// 	m_HolySoulInfo.SoulAttEx.nMaxSoulValue             = pNewHolySoul->dwSoulValue; //��ǰ��Ԫ��ֵ

	return E_Success;
}

//-------------------------------------------------------------------------------------------
// ���¼��ؼ���Ԫ������
//-------------------------------------------------------------------------------------------
DWORD HolySoul::ReloadProcHolySoulAtt(const tagSoulAttProto* pNewHolySoul )
{
// 	if ( !P_VALID(pNewHolySoul) )
// 	{
// 		return EHolySoul_NoExist;
// 	}
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv]     = pNewHolySoul->n8RebornLv;   //Ԫ��ת���ȼ�
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel]    = pNewHolySoul->n8SoulLevel;  //Ԫ��ȼ� 150 
// 
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage]	   = pNewHolySoul->nDamage;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit]		   = pNewHolySoul->nHit;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit]         = pNewHolySoul->nCrit;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount]   = pNewHolySoul->nCritAmount;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack]     = pNewHolySoul->nExattack;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack]     = pNewHolySoul->nInattack;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate]      = pNewHolySoul->nAccrate;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage]     = pNewHolySoul->nExdamage;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness]    = pNewHolySoul->nToughness;
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale]       = pNewHolySoul->n8Morale;
// 
// 
// 	m_HolySoulInfo.SoulAttEx.dwSoulID                  = pNewHolySoul->dwID;		 //Ԫ��ID
// 	if (m_HolySoulInfo.SoulAttEx.n64LevelExp < 0)
// 	{
// 		m_HolySoulInfo.SoulAttEx.n64LevelExp               = 0;  //Ԫ����ȼ�
// 	}
// 	m_HolySoulInfo.SoulAttEx.n64MaxExp                 = pNewHolySoul->n64UpLvExp;   //�ܵ���������
// 	m_HolySoulInfo.SoulAttEx.nCurCovalue               = pNewHolySoul->n8Covalue;   //��ǰ��Ĭ��ֵ
// 	m_HolySoulInfo.SoulAttEx.nMaxCovalue               = pNewHolySoul->n8Covalue;   //Ĭ��ֵ��ǰ���ֵ
// 	m_HolySoulInfo.SoulAttEx.nCurSoulValue             = pNewHolySoul->dwSoulValue; //��ǰ��Ԫ��ֵ
// 	m_HolySoulInfo.SoulAttEx.nMaxSoulValue             = pNewHolySoul->dwSoulValue; //��ǰ��Ԫ��ֵ

	return E_Success;
}

//-------------------------------------------------------------------------------------------
// ����ǿ�����Լӳ�
//-------------------------------------------------------------------------------------------
DWORD HolySoul::ProcStarEnhanceAtt(const tagStarEnhanceProto* pStarEnhanceProto,INT nFactor,EHolyEquipAttType eHolyEquipPosAtt)
{
	if ( !P_VALID(pStarEnhanceProto) )
	{
		return GT_INVALID;
	}
	if ( eHolyEquipPosAtt < EHEAT_Demage || eHolyEquipPosAtt >= EHEAT_End)
	{
		return GT_INVALID;
	}

	switch (eHolyEquipPosAtt)
	{
	case EHEAT_Demage:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nStunt*nFactor;	
		break;
	case EHEAT_Hit:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nHitRate*nFactor;	
		break;
	case EHEAT_Crit:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nCritRate*nFactor;	
		break;
	case EHEAT_CritAmount:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nCritaMount*nFactor;	
		break;
	case EHEAT_Exattack:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nExAttack*nFactor;	
		break;
	case EHEAT_InExattack:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nInAttack*nFactor;	
		break;
	case EHEAT_Accurate:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nAccuRate*nFactor;	
		break;
	case EHEAT_Exdamage:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nExDamage*nFactor;	
		break;
	case EHEAT_Toughness:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nNegToughness*nFactor;	
		break;
	case EHEAT_Morale:
		m_nEquipAtt[eHolyEquipPosAtt] += pStarEnhanceProto->nMorale*nFactor;	
		break;
	}
		
	
	return E_Success;
}

//-------------------------------------------------------------------------------------------
// ����Ԫ������
//-------------------------------------------------------------------------------------------
DWORD HolySoul::SaveHolySoulInfoToDB(SaveType eSaveType,OUT LPVOID pOutPointer,ESoulAttTypeEx eSoulAttTypeEx,DWORD dwVal)
{
	if ( ESaveType_Insert == eSaveType)
	{
		tagNDBC_InsertHolySoulInfo msg;
		msg.dwRoleID = m_pRole->GetID();
		memcpy(&msg.SoulAtt.HolySoulAtt,&m_HolySoulInfo.SoulAtt,sizeof(m_HolySoulInfo.SoulAtt));
		msg.SoulAtt.dwSoulID				= m_HolySoulInfo.SoulAttEx.dwSoulID;
		msg.SoulAtt.eHolySoulState			= EHSS_Asleep;//m_eHolySoulState;
		msg.SoulAtt.n64CurLevelExp			= m_HolySoulInfo.SoulAttEx.n64LevelExp;
		msg.SoulAtt.nCurCovalue				= m_HolySoulInfo.SoulAttEx.nCurCovalue;
		msg.SoulAtt.nCurSoulValue			= m_HolySoulInfo.SoulAttEx.nCurSoulValue;

		g_dbSession.Send(&msg,msg.dwSize);
	}
	else if ( ESaveType_Update == eSaveType )
	{
		tagNDBC_UpdateHolySoulInfo msg;
		msg.dwRoleID = m_pRole->GetID();
		memcpy(&msg.SoulAtt.HolySoulAtt,&m_HolySoulInfo.SoulAtt,sizeof(m_HolySoulInfo.SoulAtt));
		msg.SoulAtt.dwSoulID				= m_HolySoulInfo.SoulAttEx.dwSoulID;
		msg.SoulAtt.eHolySoulState			= EHSS_Asleep;//m_eHolySoulState;
		msg.SoulAtt.n64CurLevelExp			= m_HolySoulInfo.SoulAttEx.n64LevelExp;
		msg.SoulAtt.nCurCovalue				= m_HolySoulInfo.SoulAttEx.nCurCovalue;
		msg.SoulAtt.nCurSoulValue			= m_HolySoulInfo.SoulAttEx.nCurSoulValue;

		g_dbSession.Send(&msg,msg.dwSize);

	}
	else if ( ESaveType_SystemUpdate == eSaveType)
	{
		if ( !P_VALID(pOutPointer) )
		{
			return GT_INVALID;
		}
		MTRANS_POINTER(pSave, pOutPointer, tagHolySoulInfoEx);
		memcpy(&pSave->HolySoulAtt,&m_HolySoulInfo.SoulAtt,sizeof(m_HolySoulInfo.SoulAtt));
		pSave->dwSoulID					= m_HolySoulInfo.SoulAttEx.dwSoulID;
		pSave->eHolySoulState			= EHSS_Asleep;//m_eHolySoulState;
		pSave->n64CurLevelExp			= m_HolySoulInfo.SoulAttEx.n64LevelExp;
		pSave->nCurCovalue				= m_HolySoulInfo.SoulAttEx.nCurCovalue;
		pSave->nCurSoulValue			= m_HolySoulInfo.SoulAttEx.nCurSoulValue;
        
	}
	else if ( ESaveType_ExpAttUpdate == eSaveType )
	{
		tagNDBC_UpdateHolySoulExpAtt	msg;
		msg.dwRoleID    = m_pRole->GetID();
		msg.n64Exp		= m_HolySoulInfo.SoulAttEx.n64LevelExp;

		g_dbSession.Send(&msg,msg.dwSize);
	}

	else if ( ESaveType_AttInfoUpdate == eSaveType)
	{
		if ( eSoulAttTypeEx <=  ESoulAttEx_Null || eSoulAttTypeEx >= ESoulAttEx_End)
		{
			return GT_INVALID;
		}
		tagNDBC_UpdateHolySoulAttInfo  msg;
		msg.dwRoleID          = m_pRole->GetID();
		msg.eSoulAttTypeEx    = eSoulAttTypeEx;
		msg.dwAttVal		  = dwVal;

		g_dbSession.Send(&msg,msg.dwSize);
	}
    return E_Success;
}

//-------------------------------------------------------------------------------------------
// ��ͻ��˹㲥Ԫ������
//-------------------------------------------------------------------------------------------
DWORD HolySoul::SendHolySoulAttToClient()
{
     tagNS_GetSoulInfo Msg;
	 memcpy(&Msg.HolySoulInfo,&m_HolySoulInfo,sizeof(m_HolySoulInfo));

// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage]			+= m_nEquipAtt[EHEAT_Demage];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit]			+= m_nEquipAtt[EHEAT_Hit];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit]		    += m_nEquipAtt[EHEAT_Crit];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount]		+= m_nEquipAtt[EHEAT_CritAmount];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack]		+= m_nEquipAtt[EHEAT_Exattack];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack]		+= m_nEquipAtt[EHEAT_InExattack];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate]		+= m_nEquipAtt[EHEAT_Accurate];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage]		+= m_nEquipAtt[EHEAT_Exdamage];
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness]		+= m_nEquipAtt[EHEAT_Toughness];
// 
// 	 Msg.HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale]			+= m_nEquipAtt[EHEAT_Morale];
// 	 Msg.HolySoulInfo.SoulAttEx.nCurCovalue					+= m_nEquipAtt[EHEAT_Covalue];
// 	 if ( Msg.HolySoulInfo.SoulAttEx.nCurCovalue > m_HolySoulInfo.SoulAttEx.nMaxCovalue)
// 	 {
// 		Msg.HolySoulInfo.SoulAttEx.nCurCovalue				= m_HolySoulInfo.SoulAttEx.nMaxCovalue;	
// 	 }
// 	 Msg.HolySoulInfo.SoulAttEx.nCurSoulValue				+= m_nEquipAtt[EHEAT_SoulValue];
// 	 if ( Msg.HolySoulInfo.SoulAttEx.nCurSoulValue > Msg.HolySoulInfo.SoulAttEx.nMaxSoulValue)
// 	 {
// 		 Msg.HolySoulInfo.SoulAttEx.nCurSoulValue				= Msg.HolySoulInfo.SoulAttEx.nMaxSoulValue;	
// 	 }

	
	 m_pRole->SendMessage(&Msg,Msg.dwSize);
	 return E_Success;
}

//-------------------------------------------------------------------------------------------
// ����Ԫ������
//-------------------------------------------------------------------------------------------
VOID HolySoul::ProcEquipEffect(tagEquip* pNewEquip, tagEquip* pOldEquip, INT16 n16IndexOld, BOOL bDiscard, BOOL bSend)
{
	// ��װ��λ����Ϣ���͸��ͻ���
	if(!bDiscard)
	{
		m_pRole->ProcEquipEffectPos(pNewEquip, pOldEquip, EICT_HolyEquip, EICT_Bag);
	}
	// ����װ��Ӱ���������� -- ע��Ҫ�����ٴ�
	if(P_VALID(pOldEquip))
	{
		ProcEquipEffectAtt(pOldEquip, FALSE, n16IndexOld);
		
	}

	if(P_VALID(pNewEquip))
	{
		ProcEquipEffectAtt(pNewEquip, TRUE, n16IndexOld);
	}

	SendHolySoulAttToClient();

}

//-------------------------------------------------------------------------------------------
// װ������Ԫ������
//-------------------------------------------------------------------------------------------
DWORD  HolySoul::ProcEquipEffectAtt(tagEquip* pEquip, bool bEquip, const INT16 n16Index)
{

	if ( !P_VALID(pEquip) )
	{
		return GT_INVALID;
	}
	INT nFactor = 1 ;
	if ( !bEquip )
	{
		nFactor = -nFactor;
	}
	const tagEquipProto* pEquipProto = pEquip->pEquipProto;
	if ( !P_VALID(pEquipProto) )
	{
		return GT_INVALID;
	}


	EHolyEquipAttType eHolyEquipPosAtt = (EHolyEquipAttType)(((DWORD)pEquipProto->BaseEffect[0].eRoleAtt % 1000)+EHEAT_NULLEX);
	INT    nVal                 =  (pEquipProto->BaseEffect[0].nValue);
	if ( eHolyEquipPosAtt < EHEAT_Demage || eHolyEquipPosAtt >= EHEAT_End)
	{
		return EHolySoul_ConsidatePosNotExist;
	}
	m_nEquipAtt[eHolyEquipPosAtt] += nVal*nFactor ;

	//�������Լӳ�
	const tagStarEnhanceProto* pStarEnhanceProto = g_attRes.GetStarEnhanceProto(pEquip->equipSpec.byStarEnhanceLv);
	if ( P_VALID(pStarEnhanceProto) )
	{
		ProcStarEnhanceAtt(pStarEnhanceProto,nFactor,eHolyEquipPosAtt);
	}
	
	//Ԫ��װ����Ƕ�ӳ�   ��ʯ�������Ȳ�����
// 	if(pEquip->equipSpec.byDiamondLevel > 0 && pEquip->equipSpec.byDiamondLevel <= 10)
// 	{
// 		ProcHolySoulEquipInlayEx(pEquip,nFactor);
// 	}
// 	else
	//{
		ProcHolySoulEquipInlay(pEquip,nFactor);
	//}

	//Ԫ��װ�������ӳ�
	ProcHolySoulEquipConsolidate(pEquip,nFactor);

	return E_Success;
}

//-------------------------------------------------------------------------------------------
// ����Ԫ���˺�
//-------------------------------------------------------------------------------------------
DWORD  HolySoul::CalculateHolySoulDmg(Skill* pSkill,Unit* pSrc, Unit* pTarget)
{
	// Ŀ���Ѿ�������ֱ�ӷ���
	if( pTarget->IsDead() ) return GT_INVALID;

	DWORD dwTargetID = pTarget->GetID();

	// ��������
	bool bCrit = false;
	FLOAT fCrit = 0.0f;

	bool bHit = CalculateHit(pSkill,pSrc,pTarget);
	// �����жϸ�Ŀ���Ƿ�������
	if( !bHit )
	{
		// ����δ������Ϣ
		tagNS_RoleHPChange send;
		send.dwRoleID = pTarget->GetID();
		send.dwSrcRoleID = pSrc->GetID();
		send.eCause = ERHPCC_HolySoulDamage;
		send.bMiss = true;
		send.dwMisc = pSkill->GetTypeID();
		send.dwMisc2 = 0;
		send.dwSerial = 0;

		if( P_VALID(pTarget->GetMap()) )
		{
			pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		}
		 return E_Success;
	}

	//���㱩��
	bCrit = CalculateCrit(pSkill,pSrc,pTarget);
	if(bCrit)
	{
		//�����˺����ӳ�
		fCrit = CalculateCritAmount(pSrc);
	}
	if (fCrit < 0.0f)
	{
		fCrit = 0.0f;
	}

	// Ԫ�񹥻��˺�
	float fDmg = CalculateDmg(pSkill,pSrc,pTarget);

	// �����˺� = Ԫ�񹥻��˺� * (1 + �����˺����ӳ�)
	INT nFinalDmg = (INT)(fDmg * (1 + fCrit));

	// ��Ѫ
	pTarget->ChangeHP(-nFinalDmg, pSrc, pSkill, NULL, bCrit, -1, -1,true);

   return E_Success;
}

//-------------------------------------------------------------------------------------------
// Ԫ�������޸�
//-------------------------------------------------------------------------------------------

DWORD	HolySoul::OnChangeAttVal(EHolyEquipAttType eEquipAttType,const INT& nVal,bool bSend,bool bSaveToDB)
{
    if ( eEquipAttType < EHEAT_NULL ||  eEquipAttType >= EHEAT_End )   {	 return GT_INVALID;  }
	if ( nVal == 0 ) {	 return GT_INVALID;  }
	
	INT nOriginVal					= 0;
	INT nRetVal						= 0;
	bool bIsChangeVal				= true;
	ESoulAttTypeEx	eSoulAttTypeEx	= ESoulAttEx_Null;
		
	switch(eEquipAttType)
	{
	case EHEAT_Covalue:
		nOriginVal = m_HolySoulInfo.SoulAttEx.nCurCovalue;
		m_HolySoulInfo.SoulAttEx.nCurCovalue += nVal ;
		if ( m_HolySoulInfo.SoulAttEx.nCurCovalue > m_HolySoulInfo.SoulAttEx.nMaxCovalue ) 	
			m_HolySoulInfo.SoulAttEx.nCurCovalue = m_HolySoulInfo.SoulAttEx.nMaxCovalue;
		if ( m_HolySoulInfo.SoulAttEx.nCurCovalue < 0 ) m_HolySoulInfo.SoulAttEx.nCurCovalue = 0;
		nRetVal = m_HolySoulInfo.SoulAttEx.nCurCovalue;	
		if ( nOriginVal ==  nRetVal )
		{
			bIsChangeVal = false;	
		}
		eSoulAttTypeEx = ESoulAttEx_Covalue;
		break;
	case EHEAT_SoulValue:
		OnChangeSoulVal(nVal,bSend);
		return E_Success;
		break;
		
	}

	if ( bSend && bIsChangeVal )
	{
		tagNS_HolySoulAttChange msg;
		msg.dwVal = nRetVal;
		msg.eHolyEquipAttType = eEquipAttType;
		m_pRole->SendMessage(&msg,msg.dwSize);

		if ( bSaveToDB )
		{
			SaveHolySoulInfoToDB(ESaveType_AttInfoUpdate,NULL,eSoulAttTypeEx,nRetVal);
		}
		
	}

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// �ۼ�Ԫ���飬�ű��ӿ�
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::AddExp(INT64 n64ExpValue)
{
// 	if ( n64ExpValue <= 0)
// 	{
// 		return GT_INVALID;
// 	}
// 	if ( EHSS_Awake == GetState() )
// 	{
// 		return EHolySoul_AwakePourExpFaied;
// 	}
	//�ж��ȼ�����
// 	if ( m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel] >= HOLYSOUL_MAX_LEVEL )
// 	{
// 		return EHolySoul_LevelUpMaxLvl;
// 	}
// 
// 	INT64 n64ExpLvUpTemp = n64ExpValue;
// 	bool bSendToClient = false;
// 	while( n64ExpLvUpTemp > 0 )
// 	{
// 		DWORD dwRet = ExpChange(n64ExpLvUpTemp,bSendToClient);
// 		if ( E_Success != dwRet )
// 		{
// 			break;
// 		}
// 	}
// 
// 	if ( bSendToClient )
// 	{
// 		//���µ����ݿ�
// 		SaveHolySoulInfoToDB(ESaveType_Update);
// 		//Ԫ��������ʾ
// 		tagNS_HolySoulLvUp msg;
// 		m_pRole->SendMessage(&msg,msg.dwSize);
// 		//ͬ���ͻ���Ԫ����Ϣ�ı�
// 		SendHolySoulAttToClient();
// 	}
// 	else
// 	{
// 		//���µ����ݿ�
// 		SaveHolySoulInfoToDB(ESaveType_ExpAttUpdate);
// 
// 		//��ͻ��˸���Ԫ����
// 		tagNS_UpdateHolySoulExp msg;
// 		msg.n64Exp = GetCurrExp();
// 		m_pRole->SendMessage(&msg,msg.dwSize);
// 	}

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// Ԫ��ӳɽ�ɫ����
//--------------------------------------------------------------------------------------------
//�ӳɵ���ɫ���ϵļ���
const float fFactor = 0.15f;   
DWORD  HolySoul::HolyChangeRoleAtt(const tagSoulAttProto* pNewHolySoul, const tagSoulAttProto* pOldHolySoul,bool bInit,bool bSend)
{
// 	if ( !P_VALID(m_pRole) )
// 	{
// 		return GT_INVALID;
// 	}
// 	INT nDemage				= 0;
// 	INT nHit				= 0;
// 	INT nCrit				= 0;
// 	INT nCritAmount			= 0;
// 	INT nExattack			= 0;
// 	INT nInattack			= 0;
// 	INT nAccrate			= 0;
// 	INT nExdamage			= 0;
// 	INT nToughness			= 0;
// 	INT nMorale				= 0;
// 
// 
// 	if ( bInit )
// 	{
// 		float fDemage				= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage] * fFactor);	
// 		float fHit				= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit] * fFactor);
// 		float fCrit				= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit] * fFactor);
// 		float fCritAmount			= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount] * fFactor);
// 		float fExattack			= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack] * fFactor);
// 		float fInattack			= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack] * fFactor);
// 		float fAccrate			= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate] * fFactor);
// 		float fExdamage			= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage] * fFactor);
// 		float fToughness			= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness] * fFactor);
// 		float fMorale				= ((float)m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale] * fFactor);
// 		nDemage		    = (fDemage > 0.0f && fDemage < 1.0f) ?  1 : (INT)fDemage;
// 		nHit			= (fHit > 0.0f && fHit < 1.0f) ?  1 : (INT)fHit;
// 		nCrit			= (fCrit > 0.0f && fCrit < 1.0f) ?  1 : (INT)fCrit;
// 		nCritAmount		= (fCritAmount > 0.0f && fCritAmount < 1.0f) ?  1 : (INT)fCritAmount;
// 		nExattack		= (fExattack > 0.0f && fExattack < 1.0f) ?  1 : (INT)fExattack;
// 		nInattack		= (fInattack > 0.0f && fInattack < 1.0f) ?  1 : (INT)fInattack;
// 		nAccrate		= (fAccrate > 0.0f && fAccrate < 1.0f) ?  1 : (INT)fAccrate;
// 		nExdamage		= (fExdamage > 0.0f && fExdamage < 1.0f) ?  1 : (INT)fExdamage;
// 		nToughness		= (fToughness > 0.0f && fToughness < 1.0f) ?  1 : (INT)fToughness;
// 		nMorale			= (fMorale > 0.0f && fMorale < 1.0f) ?  1 : (INT)fMorale;
// 		
// 
// 	}
// 	else
// 	{
// 		if ( !P_VALID( pNewHolySoul ) || !P_VALID(pOldHolySoul) )
// 		{
// 			return GT_INVALID;
// 		}
// 		//��ȥ�ɵ�Ԫ�����Լӳ�
// 		float fDemage				= ((float)(pOldHolySoul->nDamage) * fFactor);	
// 		float fHit					= ((float)(pOldHolySoul->nHit) * fFactor);
// 		float fCrit					= ((float)(pOldHolySoul->nCrit) * fFactor);
// 		float fCritAmount			= ((float)(pOldHolySoul->nCritAmount) * fFactor);
// 		float fExattack				= ((float)(pOldHolySoul->nExattack) * fFactor);
// 		float fInattack				= ((float)(pOldHolySoul->nInattack) * fFactor);
// 		float fAccrate				= ((float)(pOldHolySoul->nAccrate) * fFactor);
// 		float fExdamage				= ((float)(pOldHolySoul->nExdamage) * fFactor);
// 		float fToughness			= ((float)(pOldHolySoul->nToughness) * fFactor);
// 		float fMorale				= ((float)(pOldHolySoul->n8Morale) * fFactor);
// 
// 		nDemage		    = (fDemage > 0.0f && fDemage < 1.0f) ?  1 : (INT)fDemage;
// 		nHit			= (fHit > 0.0f && fHit < 1.0f) ?  1 : (INT)fHit;
// 		nCrit			= (fCrit > 0.0f && fCrit < 1.0f) ?  1 : (INT)fCrit;
// 		nCritAmount		= (fCritAmount > 0.0f && fCritAmount < 1.0f) ?  1 : (INT)fCritAmount;
// 		nExattack		= (fExattack > 0.0f && fExattack < 1.0f) ?  1 : (INT)fExattack;
// 		nInattack		= (fInattack > 0.0f && fInattack < 1.0f) ?  1 : (INT)fInattack;
// 		nAccrate		= (fAccrate > 0.0f && fAccrate < 1.0f) ?  1 : (INT)fAccrate;
// 		nExdamage		= (fExdamage > 0.0f && fExdamage < 1.0f) ?  1 : (INT)fExdamage;
// 		nToughness		= (fToughness > 0.0f && fToughness < 1.0f) ?  1 : (INT)fToughness;
// 		nMorale			= (fMorale > 0.0f && fMorale < 1.0f) ?  1 : (INT)fMorale;
// 
// 		if ( 0 != nDemage )
// 		{
// 			m_pRole->ModAttModValue(ERA_WeaponDmgMin,-nDemage);
// 			m_pRole->ModAttModValue(ERA_WeaponDmgMax,-nDemage);
// 			m_pRole->ModAttModValue(ERA_WeaponSoul,-nDemage);
// 		}
// 		if ( 0 != nHit ) m_pRole->ModAttModValue(ERA_HitRate,-nHit);
// 		if ( 0 != nCrit ) m_pRole->ModAttModValue(ERA_Crit_Rate,-nCrit);
// 		if ( 0 != nCritAmount ) m_pRole->ModAttModValue(ERA_Crit_Amount,-nCritAmount);
// 		if ( 0 != nExattack ) m_pRole->ModAttModValue(ERA_ExAttack,-nExattack);
// 		if ( 0 != nInattack ) m_pRole->ModAttModValue(ERA_InAttack,-nInattack);
// 		if ( 0 != nAccrate ) m_pRole->ModAttModValue(ERA_AttackTec,-nAccrate);
// 		if ( 0 != nExdamage ) m_pRole->ModAttModValue(ERA_ExDamage,-nExdamage);
// 		//if ( 0 != nToughness ) m_pRole->ModAttModValue(ERA_Neglect_Toughness,-nToughness);	//���ò��ʥħľ��
// 		//if ( 0 != nMorale ) m_pRole->ModAttModValue(ERA_Morale,-nMorale);
// 		
// 
// 		//�ӳɾɵ�Ԫ�����Լӳ�
// 		fDemage					= ((float)(pNewHolySoul->nDamage) * fFactor);	
// 		fHit					= ((float)(pNewHolySoul->nHit) * fFactor);
// 		fCrit					= ((float)(pNewHolySoul->nCrit) * fFactor);
// 		fCritAmount				= ((float)(pNewHolySoul->nCritAmount) * fFactor);
// 		fExattack				= ((float)(pNewHolySoul->nExattack) * fFactor);
// 		fInattack				= ((float)(pNewHolySoul->nInattack) * fFactor);
// 		fAccrate				= ((float)(pNewHolySoul->nAccrate) * fFactor);
// 		fExdamage				= ((float)(pNewHolySoul->nExdamage) * fFactor);
// 		fToughness				= ((float)(pNewHolySoul->nToughness) * fFactor);
// 		fMorale					= ((float)(pNewHolySoul->n8Morale) * fFactor);
// 
// 		nDemage		    = (fDemage > 0.0f && fDemage < 1.0f) ?  1 : (INT)fDemage;
// 		nHit			= (fHit > 0.0f && fHit < 1.0f) ?  1 : (INT)fHit;
// 		nCrit			= (fCrit > 0.0f && fCrit < 1.0f) ?  1 : (INT)fCrit;
// 		nCritAmount		= (fCritAmount > 0.0f && fCritAmount < 1.0f) ?  1 : (INT)fCritAmount;
// 		nExattack		= (fExattack > 0.0f && fExattack < 1.0f) ?  1 : (INT)fExattack;
// 		nInattack		= (fInattack > 0.0f && fInattack < 1.0f) ?  1 : (INT)fInattack;
// 		nAccrate		= (fAccrate > 0.0f && fAccrate < 1.0f) ?  1 : (INT)fAccrate;
// 		nExdamage		= (fExdamage > 0.0f && fExdamage < 1.0f) ?  1 : (INT)fExdamage;
// 		nToughness		= (fToughness > 0.0f && fToughness < 1.0f) ?  1 : (INT)fToughness;
// 		nMorale			= (fMorale > 0.0f && fMorale < 1.0f) ?  1 : (INT)fMorale;
// 	}
// 	
// 
// 	if ( 0 != nDemage )
// 	{
// 		m_pRole->ModAttModValue(ERA_WeaponDmgMin,nDemage);
// 		m_pRole->ModAttModValue(ERA_WeaponDmgMax,nDemage);
// 		m_pRole->ModAttModValue(ERA_WeaponSoul,nDemage);
// 	}
// 	
// 
// 	if ( 0 != nHit ) m_pRole->ModAttModValue(ERA_HitRate,nHit);
// 	if ( 0 != nCrit ) m_pRole->ModAttModValue(ERA_Crit_Rate,nCrit);
// 	if ( 0 != nCritAmount ) m_pRole->ModAttModValue(ERA_Crit_Amount,nCritAmount);
// 	if ( 0 != nExattack ) m_pRole->ModAttModValue(ERA_ExAttack,nExattack);
// 	if ( 0 != nInattack ) m_pRole->ModAttModValue(ERA_InAttack,nInattack);
// 	if ( 0 != nAccrate ) m_pRole->ModAttModValue(ERA_AttackTec,nAccrate);
// 	if ( 0 != nExdamage ) m_pRole->ModAttModValue(ERA_ExDamage,nExdamage);
// 	//if ( 0 != nToughness ) m_pRole->ModAttModValue(ERA_Neglect_Toughness,nToughness);
// 	//if ( 0 != nMorale ) m_pRole->ModAttModValue(ERA_Morale,nMorale);
// 	
// 
// 	// ���¼�����Ӱ������ﵱǰ״̬
// 	if ( bSend )
// 	{
// 		m_pRole->RecalAttEx(bSend);
// 	}
	

	return E_Success;

}

//���¼���Ԫ��ӳɽ�ɫ���ԣ������ڽ��Ԫ�����Ա�ı�����Ԫ������ֵ����ȷ��bug
//������ReloadProcHolySoulAtt֮ǰ���ã�����
DWORD  HolySoul::ReCalHolyChangeRoleAtt(const tagSoulAttProto* pNewHolySoul,bool bSend)
{
// 	if ( !P_VALID(m_pRole) )
// 	{
// 		return GT_INVALID;
// 	}
// 	INT nDemage				= 0;
// 	INT nHit				= 0;
// 	INT nCrit				= 0;
// 	INT nCritAmount			= 0;
// 	INT nExattack			= 0;
// 	INT nInattack			= 0;
// 	INT nAccrate			= 0;
// 	INT nExdamage			= 0;
// 	INT nToughness			= 0;
// 	INT nMorale				= 0;	
// 	
// 	if ( !P_VALID( pNewHolySoul ))
// 	{
// 		return GT_INVALID;
// 	}
	
	//�¾�Ԫ�����Բ�ֵ�ӳ�
// 	float fDemage				= ((float)(pNewHolySoul->nDamage - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Demage]) * fFactor);	
// 	float fHit					= ((float)(pNewHolySoul->nHit - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Hit]) * fFactor);
// 	float fCrit					= ((float)(pNewHolySoul->nCrit - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Crit]) * fFactor);
// 	float fCritAmount			= ((float)(pNewHolySoul->nCritAmount - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_CritAmount]) * fFactor);
// 	float fExattack				= ((float)(pNewHolySoul->nExattack - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exattack]) * fFactor);
// 	float fInattack				= ((float)(pNewHolySoul->nInattack - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Inattack]) * fFactor);
// 	float fAccrate				= ((float)(pNewHolySoul->nAccrate - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Accrate]) * fFactor);
// 	float fExdamage				= ((float)(pNewHolySoul->nExdamage - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Exdamage]) * fFactor);
// 	float fToughness			= ((float)(pNewHolySoul->nToughness - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Toughness]) * fFactor);
// 	float fMorale				= ((float)(pNewHolySoul->n8Morale - m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_Morale]) * fFactor);

// 	nDemage		    = (fDemage > 0.0f && fDemage < 1.0f) ?  1 : (INT)fDemage;
// 	nHit			= (fHit > 0.0f && fHit < 1.0f) ?  1 : (INT)fHit;
// 	nCrit			= (fCrit > 0.0f && fCrit < 1.0f) ?  1 : (INT)fCrit;
// 	nCritAmount		= (fCritAmount > 0.0f && fCritAmount < 1.0f) ?  1 : (INT)fCritAmount;
// 	nExattack		= (fExattack > 0.0f && fExattack < 1.0f) ?  1 : (INT)fExattack;
// 	nInattack		= (fInattack > 0.0f && fInattack < 1.0f) ?  1 : (INT)fInattack;
// 	nAccrate		= (fAccrate > 0.0f && fAccrate < 1.0f) ?  1 : (INT)fAccrate;
// 	nExdamage		= (fExdamage > 0.0f && fExdamage < 1.0f) ?  1 : (INT)fExdamage;
// 	nToughness		= (fToughness > 0.0f && fToughness < 1.0f) ?  1 : (INT)fToughness;
// 	nMorale			= (fMorale > 0.0f && fMorale < 1.0f) ?  1 : (INT)fMorale;
// 
// 
// 
// 	if ( 0 != nDemage )
// 	{
// 		m_pRole->ModAttModValue(ERA_WeaponDmgMin,nDemage);
// 		m_pRole->ModAttModValue(ERA_WeaponDmgMax,nDemage);
// 		m_pRole->ModAttModValue(ERA_WeaponSoul,nDemage);
// 	}
// 
// 
// 	if ( 0 != nHit ) m_pRole->ModAttModValue(ERA_HitRate,nHit);
// 	if ( 0 != nCrit ) m_pRole->ModAttModValue(ERA_Crit_Rate,nCrit);
// 	if ( 0 != nCritAmount ) m_pRole->ModAttModValue(ERA_Crit_Amount,nCritAmount);
// 	if ( 0 != nExattack ) m_pRole->ModAttModValue(ERA_ExAttack,nExattack);
// 	if ( 0 != nInattack ) m_pRole->ModAttModValue(ERA_InAttack,nInattack);
// 	if ( 0 != nAccrate ) m_pRole->ModAttModValue(ERA_AttackTec,nAccrate);
// 	if ( 0 != nExdamage ) m_pRole->ModAttModValue(ERA_ExDamage,nExdamage);
// 	//if ( 0 != nToughness ) m_pRole->ModAttModValue(ERA_Neglect_Toughness,nToughness);
// 	//if ( 0 != nMorale ) m_pRole->ModAttModValue(ERA_Morale,nMorale);
// 
// 
// 	// ���¼�����Ӱ������ﵱǰ״̬
// 	if ( bSend )
// 	{
// 		m_pRole->RecalAttEx(bSend);
// 	}


	return E_Success;

}

DWORD  HolySoul::HolySoulReborn()
{
// 	if ( !m_bIsActivated )
// 	{
// 		return EHolySoul_NotActivated;
// 	}
// 	if ( EHSS_Awake == GetState() )
// 	{
// 		return EHolySoul_AwakePourExpFaied;
// 	}
// 	if (m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] >= REBORN_MAX_LEVEL * 10000)
// 	{
// 		return EHolySoul_RebormMaxLvl;
// 	}
// 	////Ԫ��ת���ȼ�+10000
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] += 10000;
// 	//Ԫ��ȼ���Ϊת�����Ӧ��1��
// 	m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel] = 1;
// 
// 	//Ԫ��ID =  ת�� + Ԫ��ȼ�
// 	DWORD dwOldID = m_HolySoulInfo.SoulAttEx.dwSoulID;
// 	DWORD dwNewID = m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	const tagSoulAttProto* pOldSoulAttProto = g_attRes.GetSoulAttProto(dwOldID);
// 	if ( !P_VALID(pOldSoulAttProto) )
// 	{
// 		return EHolySoul_NoExist;
// 	}
// 	const tagSoulAttProto* pNewSoulAttProto = g_attRes.GetSoulAttProto(dwNewID);
// 	if ( !P_VALID(pNewSoulAttProto) )
// 	{
// 		return EHolySoul_NextLevelNoExist;
// 	}
// 	DWORD dwErrorCode = ProcHolySoulAtt(pNewSoulAttProto,pOldSoulAttProto);
// 	if ( E_Success != dwErrorCode)
// 	{
// 		return dwErrorCode;
// 	}
// 
// 	HolyChangeRoleAtt(pNewSoulAttProto,pOldSoulAttProto,false,true);
// 
// 	//���µ����ݿ�
// 	SaveHolySoulInfoToDB(ESaveType_Update);
// 	//ͬ���ͻ���Ԫ����Ϣ�ı�
// 	SendHolySoulAttToClient();

	return E_Success;
}

//--------------------------------------------------------------------------------------------
// Ԫ�񽵼�
//--------------------------------------------------------------------------------------------
DWORD  HolySoul::LevelDownSoul(BYTE byLevel)
{
// 	if ( !m_bIsActivated )
// 	{
// 		return EHolySoul_NotActivated;
// 	}
// 
// 	//������ͼ������ڵ�ǰ�ȼ�
// 	if (byLevel >= m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel])
// 	{
// 		return GT_INVALID;
// 	}
// 	//Ԫ��ID =  ת�� + Ԫ��ȼ�
// 	DWORD dwOldID = m_HolySoulInfo.SoulAttEx.dwSoulID;
// 	DWORD dwNewID = m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] + m_HolySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel] - byLevel;
// 	const tagSoulAttProto* pOldSoulAttProto = g_attRes.GetSoulAttProto(dwOldID);
// 	if ( !P_VALID(pOldSoulAttProto) )
// 	{
// 		return EHolySoul_NoExist;
// 	}
// 	const tagSoulAttProto* pNewSoulAttProto = g_attRes.GetSoulAttProto(dwNewID);
// 	if ( !P_VALID(pNewSoulAttProto) )
// 	{
// 		return EHolySoul_NextLevelNoExist;
// 	}
// 	DWORD dwErrorCode = ProcHolySoulAtt(pNewSoulAttProto,pOldSoulAttProto);
// 	if ( E_Success != dwErrorCode)
// 	{
// 		return dwErrorCode;
// 	}
// 
// 	HolyChangeRoleAtt(pNewSoulAttProto,pOldSoulAttProto,false,true);
// 
// 	//���µ����ݿ�
// 	SaveHolySoulInfoToDB(ESaveType_Update);
// 
// 	//ͬ���ͻ���Ԫ����Ϣ�ı�
// 	SendHolySoulAttToClient();

	return E_Success;
}

//����Ԫ��װ����Ƕ�ӳ�
DWORD HolySoul::ProcHolySoulEquipInlay(tagEquip* pEquip, INT nFactor)
{
	if (!P_VALID(pEquip))
	{
		return GT_INVALID;
	}
	for(INT i = 0; i < pEquip->equipSpec.byHolyHoleNum; i++)
	{
		if (!P_VALID(pEquip->equipSpec.dwHoleGemID[i]))
		{
			continue;
		}
		const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pEquip->equipSpec.dwHoleGemID[i]);
		if (P_VALID(pConsolidateProto))
		{
			for (INT i = 0; i < MAX_CONSOLIDATE_ROLEATT; i++)
			{
				EHolyEquipAttType eHolyEquipPosAtt = (EHolyEquipAttType)(((DWORD)pConsolidateProto->tagRoleAtt[i].eRoleAtt % 1000) + EHEAT_NULLEX);
				INT nVal =  (pConsolidateProto->tagRoleAtt[i].nAttVal);
				//��Ƕ,��ΪԪ��װ����ǿ�����ԣ�ժ����ʯʱ��Ӧ����
				if ( eHolyEquipPosAtt < EHEAT_Demage || eHolyEquipPosAtt >= EHEAT_End)
				{
					continue;
				}
				
				m_nEquipAtt[eHolyEquipPosAtt] += nFactor * nVal;


			}
		}

	}
	return E_Success;
}

//����Ԫ��װ����Ƕ�����ӳ�
DWORD HolySoul::ProcHolySoulEquipInlayEx(tagEquip* pEquip, INT nFactor)
{
	if (!P_VALID(pEquip))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	const tagStoneEnhanceProto * pEnhanceProto = g_attRes.GetStoneEnhanceProto(pEquip->equipSpec.nEnhanceLevel);
// 	if (!P_VALID(pEnhanceProto))
// 	{
// 		return GT_INVALID;
// 	}
// 	//���������ٷֱ�
// 	FLOAT fMultiple = (FLOAT)(pEnhanceProto->nEnhancePct) / 10000.0f;
// 
// 	FLOAT fEquipEnhanceAtt[EHEAT_End]; //��ʱ���飬�ݴ�Ԫ��װ���ӳɣ���ֹÿ������ȡ������������ֵ����
// 	ZeroMemory(fEquipEnhanceAtt,sizeof(fEquipEnhanceAtt));
// 
// 	for(INT i = 0; i < pEquip->equipSpec.byHolyHoleNum; i++)
// 	{
// 		if (!P_VALID(pEquip->equipSpec.dwHoleGemID[i]))
// 		{
// 			continue;
// 		}
// 		const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pEquip->equipSpec.dwHoleGemID[i]);
// 		if (P_VALID(pConsolidateProto))
// 		{
// 			for (INT i = 0; i < MAX_CONSOLIDATE_ROLEATT; i++)
// 			{
// 				EHolyEquipAttType eHolyEquipPosAtt = (EHolyEquipAttType)(((DWORD)pConsolidateProto->tagRoleAtt[i].eRoleAtt % 1000) + EHEAT_NULLEX);
// 				INT nVal =  (pConsolidateProto->tagRoleAtt[i].nAttVal);
// 				//��Ƕ,��ΪԪ��װ����ǿ�����ԣ�ժ����ʯʱ��Ӧ����
// 				if ( eHolyEquipPosAtt < EHEAT_Demage || eHolyEquipPosAtt >= EHEAT_End)
// 				{
// 					continue;
// 				}
// 
// 				fEquipEnhanceAtt[eHolyEquipPosAtt] += (nFactor * nVal * (1 + fMultiple));
// 
// 			}
// 		}
// 
// 	}
// 
// 	//����ʱ�����ֵ����װ���ӳ�����
// 	for (INT i = 0; i < EHEAT_End; i++)
// 	{
// 		m_nEquipAtt[i] += (UINT)fEquipEnhanceAtt[i];
// 	}
	return E_Success;
}

//--------------------------------------------------------------------------------------------
//����Ԫ��װ�������ӳ�
//--------------------------------------------------------------------------------------------
DWORD HolySoul::ProcHolySoulEquipConsolidate(tagEquip* pEquip,INT nFactor)
{
// 	if (!P_VALID(pEquip))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	DWORD nVal = 0;
// 	FLOAT fTimeRate = 0.0f;
// 	for (INT i = 0; i < MAX_WINGDART_LEVEL; i++)
// 	{
// 		if (!P_VALID(pEquip->equipSpec.dwWingDartConsolidate[i]))
// 		{
// 			continue;
// 		}
// 		const tagWingComposeProto* pProto = g_attRes.GetSoulEquipConsolidateProto(pEquip->equipSpec.dwWingDartConsolidate[i]);
// 		if (P_VALID(pProto))
// 		{
// 			fTimeRate = pProto->fMultipleValue;
// 			if (fTimeRate > MAX_WINGDART_MULTRATE)
// 			{
// 				fTimeRate = (FLOAT)MAX_WINGDART_MULTRATE;
// 			}
// 			
// 			nVal = (INT)(pProto->nProtoValue * fTimeRate + 0.5); //�������룬��ֹ�������⵼�����ݲ�׼ȷ
// 			EHolyEquipAttType eHolyEquipPosAtt = (EHolyEquipAttType)(((DWORD)pProto->EAtt % 1000) + EHEAT_NULLEX);
// 			if ( eHolyEquipPosAtt < EHEAT_Demage || eHolyEquipPosAtt >= EHEAT_End)
// 			{
// 				continue;
// 			}
// 			m_nEquipAtt[eHolyEquipPosAtt] += nFactor * nVal;
// 		}
// 	}

	return E_Success;
}


//-----------------------------------------------------------------------
// ����ʿ��Ӱ��
//-----------------------------------------------------------------------
inline float HolySoul::CalMoraleCoef(Unit* pSrc ,Unit* pTarget)
{
	if ( !pSrc->IsRole())
	{
		return 0.0f;
	}

	float fMoralCoef = 0.0f;
	float fTemp = 0.0f;
	// Ŀ��Ϊ��ɫ��ʿ��Ӱ��=[1+(��������ʿ��-������Ԫ��ʿ����^��1/3��)/8]
	if (pTarget->IsRole())
	{
		Role* pTargetRole = dynamic_cast<Role*>(pTarget);
		//���������߽�����
		if (!P_VALID(pTargetRole))
		{
			return 0.0f;
		}
		//�����Ŀ����������⣬�����������ֵ�Ŀ�������Ȼ�������������
		fTemp = pow(fabs((float)GetHolySoulAtt(EHEAT_Morale) - (float)pTargetRole->GetHolySoul().GetHolySoulAtt(EHEAT_Morale)),1.0f / 3);
		if (GetHolySoulAtt(EHEAT_Morale) < pTargetRole->GetHolySoul().GetHolySoulAtt(EHEAT_Morale))
		{
			fTemp *= -1.0f;
		}
		fMoralCoef = 1.0f + fTemp / 8.0f;
	}
	//Ŀ��Ϊ���ʿ��Ӱ��=[1+(��������ʿ��-������ʿ����^��1/3��)/8]
	else
	{
		fTemp = pow(fabs((float)GetHolySoulAtt(EHEAT_Morale) - (float)pTarget->GetAttValue(ERA_Morale)),1.0f / 3);
		if (GetHolySoulAtt(EHEAT_Morale) < (float)pTarget->GetAttValue(ERA_Morale))
		{
			fTemp *= -1.0f;
		}
		fMoralCoef = 1.0f + fTemp / 8.0f;
	}

	//fMoralCoef ȡֵ[0.5,9]
	if (fMoralCoef > 9.0f)
	{
		fMoralCoef = 9.0f;
	}
	if (fMoralCoef < 0.5f)
	{
		fMoralCoef = 0.5f;
	}
	return fMoralCoef;
}
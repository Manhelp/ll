#include "stdafx.h"
#include "pet_exchange.h"
#include "../WorldDefine/ItemDefine.h"
#include "pet_define.h"
#include "att_res.h"
#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/msg_pet_exchange.h"

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
PetExchangeMgr::PetExchangeMgr()
{
	m_pExData		   = NULL;
	m_dwTgtRoleID	   = GT_INVALID;
	m_byFollowPetCount = 0;
	m_byRidingPetCount = 0;
}

PetExchangeMgr::~PetExchangeMgr()
{
	SAFE_DEL(m_pExData);
}

//-----------------------------------------------------------------------------
// �������׿ռ� -- ����Ѵ��ڣ������³�ʼ��
//-----------------------------------------------------------------------------
VOID PetExchangeMgr::CreateData()
{
	if(NULL == m_pExData)
	{
		m_pExData = new tagPetExchangeData;
	}

	ASSERT(m_pExData != NULL);

	ZeroMemory(m_pExData, sizeof(tagPetExchangeData));
	m_byFollowPetCount = 0;
	m_byRidingPetCount = 0;
}

//-----------------------------------------------------------------------------
// �������ݽṹ����ӽ�����Ʒ -- ���� -- �ɹ�:����λ���±ꣻʧ��:GT_INVALID
//-----------------------------------------------------------------------------
INT32 PetExchangeMgr::AddPet(DWORD dwPetID,DWORD dwPetTypeID)
{
	ASSERT(m_pExData != NULL);

	INT32 nInsIndex = GT_INVALID;
	for(INT32 i=0; i<MAX_EXCHANGE_ITEM; ++i)
	{
		if(m_pExData->dwPetIDs[i] == dwPetID)
		{
			IMSG(_T("Add the same pet<id: %lu> to exchange!!!!\r\n"), dwPetID);
			return GT_INVALID;
		}
		else if(0 == m_pExData->dwPetIDs[i] && GT_INVALID == nInsIndex)
		{
			nInsIndex = i;
		}
	}

	if(nInsIndex != GT_INVALID)
	{
		m_pExData->dwPetIDs[nInsIndex] = dwPetID;

		++m_pExData->byPetNum;
	}
	const tagPetProto*  pPetProto = g_attRes.GetPetProto(dwPetTypeID);
	if ( P_VALID(pPetProto) )
	{
		//�������
		if( 6 == pPetProto->nType3)
		{
			m_byFollowPetCount++;
		}
		else
		{
			m_byRidingPetCount++;
		}
	}


	return E_Pets_Success;
}

//-----------------------------------------------------------------------------
// �ӽ�����Ʒ��ȡ��ָ����Ʒ -- �ɹ�����E_Success��ʧ�ܷ�����Ӧ������
//-----------------------------------------------------------------------------
DWORD PetExchangeMgr::DecPet(DWORD dwPetID,DWORD dwPetTypeID)
{
	ASSERT(m_pExData != NULL);


	for(INT32 i=0; i<MAX_EXCHANGE_ITEM; ++i)
	{
		if(m_pExData->dwPetIDs[i] == dwPetID)
		{
			m_pExData->dwPetIDs[i] = 0;
			--m_pExData->byPetNum;

			const tagPetProto*  pPetProto = g_attRes.GetPetProto(dwPetTypeID);
			if ( P_VALID(pPetProto) )
			{
				//�������
				if( 6 == pPetProto->nType3)
				{
					m_byFollowPetCount--;
				}
				else
				{
					m_byRidingPetCount--;
				}
			}
			return E_Success;
		}
	}

	return GT_INVALID;
}
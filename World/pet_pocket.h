//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_pocket.h
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: �����
//-------------------------------------------------------------------------------------------------------
#pragma once

class PetSoul;
class Pet;
class Role;
struct tagDBPet;

const INT CONST_MAX_PETSOUL_NUM = 100;      //�����ܵ�����

//----------------------------------------------------------------------------------------------------
// �����
//----------------------------------------------------------------------------------------------------
class PetPocket
{
	typedef	TMap<DWORD, PetSoul*>	SoulTMap;
	typedef	TMap<DWORD, Pet*>		PetTMap;
	typedef TList<PetSoul*>			SoulTList;

public:
	//----------------------------------------------------------------------------------------------------
	// ���������
	//----------------------------------------------------------------------------------------------------
	~PetPocket(){	Destroy();	}

	//----------------------------------------------------------------------------------------------------
	// �����ӿ�
	//----------------------------------------------------------------------------------------------------
	BOOL		Init(const BYTE* &pData, INT nNum, Role* pRole);
	BOOL		SaveToDB(IN LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID		Destroy();
	VOID		Update();

public:
	//----------------------------------------------------------------------------------------------------
	// ���߷���
	//----------------------------------------------------------------------------------------------------
	PetSoul*	GetAway(DWORD dwPetID, BOOL bSync = FALSE);
	PetSoul*	Recall(DWORD dwPetID, BOOL bSync = FALSE);
	DWORD		PutIn(PetSoul* pSoul, BOOL bSaveDB, BOOL bSend = TRUE);
	DWORD		CanPutIn(PetSoul* pSoul);

	//----------------------------------------------------------------------------------------------------
	// ���
	//----------------------------------------------------------------------------------------------------
	VOID		GetAllPetID(DWORD* dwPetIDbuf, INT& num);
	PetSoul*	GetPetSoul(DWORD dwPetID);
	PetSoul*	GetCalledPetSoul();
	PetSoul*	GetPreparingPetSoul();
	PetSoul*	GetMountPetSoul();

	// Jason 2010-3-9 v1.4.0
	VOID CancleLastPetPreparingStatus();
	
	INT  GetCalledPetNum()	{ return m_nCalledPets; }

	//----------------------------------------------------------------------------------------------------
	// ���ɳ���
	//----------------------------------------------------------------------------------------------------
	DWORD		HatchEgg(INT64 n64ItemID, LPCTSTR tszName);

	//----------------------------------------------------------------------------------------------------
	// ��������
	//----------------------------------------------------------------------------------------------------
	DWORD		LockPet(DWORD dwPetID, INT64 n64ItemID);
	DWORD		UnLockPet(DWORD dwPetID, INT64 n64ItemID);

	//----------------------------------------------------------------------------------------------------
	// ��ǿ����
	//----------------------------------------------------------------------------------------------------
	DWORD		CalledSoulEnhance(INT64 n64ItemID);
	DWORD		CalledSoulUpStep(INT64 n64ItemID, DWORD &dwSkillID, INT &nDstStep);

	//----------------------------------------------------------------------------------------------------
	// �ٻ���Ϣ
	//----------------------------------------------------------------------------------------------------
	DWORD		CallPet(DWORD dwPetID);
	DWORD		RestPet(DWORD dwPetID);
	DWORD		RestAPet();

	//----------------------------------------------------------------------------------------------------
	// Ԥ����Ԧ
	//----------------------------------------------------------------------------------------------------
	DWORD		PreparePet(DWORD dwPetID);
	DWORD		UnPreparePet(DWORD dwPetID);

	//----------------------------------------------------------------------------------------------------
	// ��Ԧ
	//----------------------------------------------------------------------------------------------------
	DWORD		CanRidePet();
	DWORD		RidePet();
	DWORD		UnRidePet();

	VOID		SetKeepMount(BOOL bSet)	{	m_bKeepMount = bSet;	}
	BOOL		GetKeepMount()			{	return m_bKeepMount;	}

	//----------------------------------------------------------------------------------------------------
	// ˫����˸���ʻ
	//----------------------------------------------------------------------------------------------------
	BOOL		IsRideAfter();
	DWORD		GetHostPetID()	{return m_dwOthersPetID;}
	DWORD		CanSetRideAfter(Role* pHost, BOOL bSet);
	DWORD		SetRideAfter(Role* pHost, BOOL bSet);

	DWORD		RideAfter(Role* pMaster);// �������������
	DWORD		StopRideAfter(Role* pMaster);

	//----------------------------------------------------------------------------------------------------
	// ˫���������ʻ
	//----------------------------------------------------------------------------------------------------
	DWORD		CanAddPassenger(Role* pPassenger);
	DWORD		CanRemovePassenger(Role* pPassenger);
	DWORD		AddPassenger(Role* pPassenger);
	DWORD		RemovePassenger(Role* pPassenger);

	//----------------------------------------------------------------------------------------------------
	// �ٻ��ĳ���ιʳ
	//----------------------------------------------------------------------------------------------------
	DWORD		CalledPetFeed(INT64 n64ItemID);

public:
	//----------------------------------------------------------------------------------------------------
	// �ٻ�������������
	//----------------------------------------------------------------------------------------------------
	VOID		CalledPetEnterPocket();
	VOID		CalledPetLeavePocket();

	//----------------------------------------------------------------------------------------------------
	// ���ｻ�����
	//----------------------------------------------------------------------------------------------------
	BOOL		CheckExistInPocket(DWORD *dwPetIDs, INT nNum );
	DWORD		CanExchange(DWORD dwPetID);
	BOOL		GetFreeSize();
	UINT16		GetFreeFollowPetSize()const;
    UINT16		GetFreeRidingPetSize()const;
	UINT16      GetFollowPetCount()const;
	UINT16      GetRidingPetCount()const;
	VOID        RefreshPetCount();
	VOID		TakeFromPocket(PetSoul* *pSouls, INT nSizeSouls, DWORD* dwPetIDs, INT nNumPetID);
	VOID		PutInPocket(PetSoul* *pSouls, INT nSizeSouls);

private:
	Role*			m_pMaster;			// ����
	SoulTMap		m_mapAllSoul;		// ����PetSoul
	INT				m_nCalledPets;		// �Ѿ��ٻ��ĳ�������
	DWORD			m_dwPetIDForUpStep;	// ���׵ĳ���id
	BOOL			m_bKeepMount;		// ���֮���Ƿ�������
	DWORD			m_dwOthersPetID;	// ˫������У����ŵ�������ҵĳ���
	UINT16          m_n16FollowPetCount; //�������ĸ���
	UINT16          m_n16RidingPetCount; //����ĸ���

public:
	//----------------------------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------------------------
	BOOL		DynamicTest(INT nTestNo, INT nArg1, INT nArg2);

};



#pragma once
#pragma pack(push, 1)

#define m02ID		3017298127
#define m03ID		3017298383
#define m04ID		3017299663
#define m05ID		3017299919
#define m06ID		3017299151
#define m061ID		1146339967
#define m07ID		3017299407
#define m20ID		2983744207

#define PLAYER_TRACK_LIMIT_BUFF 10922

enum ESpecItemUseError
{
	E_ItemLock_HaveLocked = 1,		//��Ʒ�Ѿ�������
	E_ItemLock_NoLocked,			//��Ʒ��δ������
	E_ItemLock_TargetError,			//Ŀ�겻�Ϸ�
	E_ItemLock_TargetInvalid,		//Ŀ����Ʒ����װ����ʯ
	E_ItemLock_HaveNotItem,			//û���������߽��� ����
	E_ItemLock_OtherError,			//��������
	
	E_ItemTrack_HaveNotItem,		//��û��׷�ٵ���
	E_ItemTrack_ItemUseTimeLimit,	//���ߴﵽ���ʹ�ô���	
	E_ItemTrack_PlayerOffLine,		//Ŀ����Ҳ�����
	E_ItemTrack_PlayerNotExists,	//��Ҳ�����
	E_ItemTrack_BuffLimit,			// �����н�ֹ�ô˹��ܵ�buff
	//E_ItemTrack_PetsLimit,			//���ų������Ҳ��ܴ�

	E_ItemTrack_DeadState,			//����״̬���ܴ���
	E_ItemTrack_DizzyState,			//ѣ��״̬���ܴ���
	E_ItemTrack_TieState,			//����״̬���ܴ���
	E_ItemTrack_SporState,			//��˯״̬���ܴ���
	E_ItemTrack_PKState,			//PK״̬���ܴ���
	E_ItemTrack_CommerceState,		//����״̬���ܴ���
	E_ItemTrack_StallState,			//��̯״̬���ܴ���
	E_ItemTrack_PrisonAreaState,		//�������������
	E_ItemTrack_TargetPrisonAreaState,	//Ŀ�������������
	E_ItemTrack_Mount,				//���״̬�²��ܴ���
	E_ItemTrack_XiulianChang,		//Ŀ�������������
	E_ItemTrack_Instance,			//Ŀ������ڸ���
	E_ItemTrack_JinCity,			//Ŀ������ڽ���
	E_ItemTrack_ShenJi,				//Ŀ����Ҵ���������

	E_EquipSig_HaveNoItem,			//���߷Ƿ�
	E_EquipSig_NoTarget,			//Ҫ�����Ķ��󲻴���
	E_EquipSig_NotEquip,			//Ҫ�����Ķ�����װ��
	E_EquipSig_AlreadySig,			//Ҫ������װ���ѱ�����
	E_EquipSig_ContentLimit,		//�������ݺ�Υ������

	E_ScriptItem_ItemInvalid,		// �õ��߲��Ϸ�
	E_ScriptItem_ScriptUnreg,		// û��ע����õĽű�
	E_ScriptItem_UseLimit,			// ��Ʒ����ʹ��
	E_ScriptItem_DesRoleOffline,	// Ŀ���ɫ������
	E_ScriptItem_DesRoleError,		// Ŀ���ɫ��Ӧ��ʹ����Ʒ�Ľ�ɫ

	E_PetLifeTime_ItemInvalid,		// ���߲��Ϸ�
	E_PetLifeTime_SoulNotExist,		// �ó��ﲻ����
	E_PetLifeTime_NotTimeLimit,		// ����ʱ���Ƴ���
	E_PetLifeTime_Failed,			// ����ʧ��
};


//-----------------------------------------------------------------------
// ��Ʒ��������
//-----------------------------------------------------------------------

CMD_START(NC_LockItem)
	INT64			n64Item;		// ���ߵ�64λID
	INT64			n64Target;		// Ҫ����Ʒ��64λID
CMD_END

CMD_START(NS_LockItem)
	DWORD			dwErrorCode;	// �������
	BYTE			byType;			// 0�����������ķ��أ�1�����������ķ���
	DWORD			dwUnlockTime;	// ��������ʱ�����ؽ���ʱ��
	INT64			n64Serial;		// Ŀ����ƷID
CMD_END

// Jason 2010-8-25
CMD_START( NS_UnlockItemNotice )
	UINT64		u64ItemID;
	BYTE			byContainerType;
CMD_END


//-----------------------------------------------------------------------
// ��ɫ׷�ٵ���
//-----------------------------------------------------------------------
CMD_START(NC_TrackPlayerPos)
	INT64			n64Item;						// ���ߵ�64λID
	TCHAR			szPlayerName[X_SHORT_NAME];		// ��ҵ�����
CMD_END

CMD_START(NS_TrackPlayerPos)
	DWORD			dwErrorCode;					// �������
	TCHAR			szPlayerName[X_SHORT_NAME];		// ��ѯ����ҵ�����
	DWORD			dwMapID;						// ������ڵĴ��ͼ��ID
	Vector3			vPosition;						// ����
CMD_END

CMD_START(NC_Move2Player4Track)
	INT64			n64ItemTrack;					// ʹ��׷�ٵ��ߵ�64λID
	TCHAR			szPlayerName[X_SHORT_NAME];		// Ҫ���͵�����Ҹ���
CMD_END

CMD_START(NS_Move2Player4Track)
	DWORD			dwErrorCode;					// ������
	DWORD			dwPlayerID;						// Ҫ׷�ٵ����ID
	TCHAR			szPlayerName[X_SHORT_NAME];		// Ҫ׷�ٵ���ҵ�����
	DWORD			dwMapID;						// ���͵��ĵ�ͼ��ID
	Vector3			pos;							// ���͹�ȥ������λ�ã�����ʧ��ʱ���ֶ�������
	Vector3			faceTo;							// ���͹�ȥ��ĳ��򣬴���ʧ��ʱ���ֶ�������
CMD_END

CMD_START(NC_GetPositionAfterTrack)					// ���͹�ȥ��ÿ��10������һ����ҵ�����λ��
	INT64			n64Item;						// ���ߵ�64λID
	DWORD			dwRoleID;						// Ҫ׷�ٵ���ҵ�ID	
CMD_END

CMD_START(NS_GetPositionAfterTrack)
	DWORD			dwErrorCode;					// ������
	TCHAR			szPlayerName[X_SHORT_NAME];		// Ҫ׷�ٵ���ҵ�����
	DWORD			dwMapID;						// ��ҵ�ǰ���ڵĵ�ͼ��ID
	Vector3			pos;							// ��ҵ�����λ��
CMD_END

//-----------------------------------------------------------------------
// װ����������
//-----------------------------------------------------------------------
CMD_START(NC_EquipSignature)
	INT64			n64Item;			// ���ߵ�64λid
	INT64			n64EquipID;			// Ҫ����װ����id
	TCHAR			szContent[11];		// �����������10���ַ�
CMD_END

CMD_START(NS_EquipSignature)
	DWORD			dwErrorCode;		
	INT64			n64EquipID;			// Ҫ����װ����64λid 
CMD_END


//-----------------------------------------------------------------------
// �ɽ����з� ����NS_UseItem
//-----------------------------------------------------------------------
CMD_START(NC_AddMiraclePrcNumItem)
	INT64   n64ItemID;      // �ɽ����з�
CMD_END

//-----------------------------------------------------------------------
// ��Ҫ���ýű���������Ʒ�Ĺ�����Ϣ
//-----------------------------------------------------------------------
CMD_START(NC_SpecScriptItem)
	INT64   n64ItemID;						// ��Ʒ��64λID
	TCHAR	szDesRoleName[X_SHORT_NAME];	// ��Ʒ�����ڵĽ�ɫ������
CMD_END

CMD_START(NS_SpecScriptItem)
	DWORD	dwErrCode;
CMD_END

//-----------------------------------------------------------------------
// �������޵�����
//-----------------------------------------------------------------------
CMD_START(NC_AddPetLiveTime)
	INT64   n64ItemID;						// ��Ʒ��64λID
	DWORD	dwPetID;
CMD_END

CMD_START(NS_AddPetLiveTime)
	DWORD	dwErrCode;
CMD_END

#pragma pack(pop)
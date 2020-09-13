#pragma once

#pragma pack(push, 1)

#define IsFashionStateValid(state) (((state)>EFS_NULL) && (state)<EFS_End)

enum EFashionErrorCode
{
	EFEC_Success = 0,
	EFEC_None = 1, // һ��ʱװҲû��
	EFEC_Incomplete = 2, // δ����
	EFEC_UseBySelf = 3, // �Լ�������
	EFEC_UseForFriends = 4, // ����װ����
	EFEC_Sex_NotMatch = 5, // ʱװ�Ա�ƥ��
	EFEC_NoLevelUpItem = 6, // ȱ�������³���λ�������
	EFEC_FriendNotOnLine = 7, // ���Ѳ�����
	EFEC_IsPlayActing = 8, // �Ѿ��������Լ��³������ʱװ
	EFEC_IsWaitingPlayActFriendReq = 9, // ���ڵȴ����ѵ�������
	EFEC_PlayActCountLimit = 10, // �ѵ������װ����ѵ������
	EFEC_DiyLayerPlayActLimit = 11, // ������λ������װ�����

	EFEC_IsBePlayActing = 4, // �Ѿ���װ��
};

// �³���״̬
enum EFashionState
{
	EFS_NULL = -1,
	EFS_None = 0,					// �գ�һ��ʱװҲû��
	EFS_Incomplete = 1,		// û�м��룬�м���ʱװ
	EFS_Complete = 2,			// ���룬δʹ��
	EFS_UseBySelf = 3,			// �Լ�������
	EFS_UseForFriends = 4,	// ����װ����
	EFS_End,
};

#define MIsFashionSuitComplete(state) (state == EFS_Complete || state == EFS_UseBySelf || state == EFS_UseForFriends)

struct tagFashionInfo
{
	DWORD dwIndex;					// �³�������
	BYTE byLevel;					// �³����ȼ�
	EFashionState eState;			// �³���״̬
};

#pragma pack(pop)
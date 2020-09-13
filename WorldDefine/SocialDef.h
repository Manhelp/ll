#pragma once

#pragma pack(push, 1)

const INT MAX_FRIENDNUM					= 100;		// ����������
const INT MAX_BLACKLIST					= 20;		// �����������
const INT MAX_FRIENDVAL					= 9999;		// ���Ѷ����ֵ
const INT MAX_ENEMYNUM					= 20;		// ���������
const INT GIFT_STORE_TIME				= 60 * TICK_PER_SECOND;	// �����ڹܵ��ﱣ��ʱ��

struct tagFriend
{
	DWORD	dwFriendID;								// ����ID
	DWORD	dwFriVal;								// ���Ѷ�
	BYTE	byGroup;								// ����
	tagFriend():dwFriendID(GT_INVALID),dwFriVal(0),byGroup(1){}
};

struct tagFriendInfo
{
	DWORD	dwFriendID;								// ����ID
	DWORD	dwFriVal;								// ���Ѷ�
	BYTE	byGroup;								// ����
	BOOL    bOnline;								// �����Ƿ�����
	INT		nLevel;									// ��ɫ�ȼ�
	INT		nVIPLevel;								// vip�ȼ���0���޵ȼ���1����2��3�׽�
};

struct tagEnemyInfo
{
	DWORD	dwEnemyID;								// ���ID
	INT		nLevel;									// ��м���
	tagEnemyInfo():dwEnemyID(GT_INVALID),nLevel(0){}
};

struct tagSendGift				
{
	DWORD	dwSrcRoleID;							// ������
	DWORD	dwDestRoleID;							// ������
	DWORD	dwTypeID;								// ����TypeID
	INT 	nStoreTick;								// ����ʱ��
	tagSendGift(DWORD dwSrcRoleID, DWORD dwDestRoleID, DWORD dwTypeID):
		dwSrcRoleID(dwSrcRoleID),dwDestRoleID(dwDestRoleID),dwTypeID(dwTypeID)
	{
		nStoreTick = GIFT_STORE_TIME;
	}
};

struct tagFriUpdate
{
	DWORD   dwRoleID;								// ����ID
	BOOL	bOnline;								// �Ƿ�����
	INT		nLevel;									// �ȼ�
	INT		nVIPLevel;								// vip�ȼ���0���޵ȼ���1����2��3�׽�
};

// �����������buff
//2024401	����	����		����Э��	����������2���������ԭʼ�������ֵ2%��ԭʼ�������ֵ2%�Ľ�����
//2024402	����	����		����Э��	����������3���������ԭʼ�������ֵ4%��ԭʼ�������ֵ4%�Ľ�����
//2024403	����	����		����Э��	����������4���������ԭʼ�������ֵ6%��ԭʼ�������ֵ6%�Ľ�����
//2024404	����	����		����Э��	����������5���������ԭʼ�������ֵ8%��ԭʼ�������ֵ8%�Ľ�����
//2024405	����	����		����Э��	����������6���������ԭʼ�������ֵ10%��ԭʼ�������ֵ10%�Ľ�����
//2024501	����	����		����Э��	�����к���������2�����2%����ӳ�
//2024502	����	����		����Э��	�����к���������3�����4%����ӳ�
//2024503	����	����		����Э��	�����к���������4�����6%����ӳ�
//2024504	����	����		����Э��	�����к���������5�����8%����ӳ�
//2024505	����	����		����Э��	�����к���������6�����10%����ӳ�
//2024601	����	����		�����޼�	�����к���������2���������ԭʼ�������ֵ4%��ԭʼ�������ֵ4%�Ľ�����
//2024602	����	����		�����޼�	�����к���������3���������ԭʼ�������ֵ8%��ԭʼ�������ֵ8%�Ľ�����
//2024603	����	����		�����޼�	�����к���������4���������ԭʼ�������ֵ12%��ԭʼ�������ֵ12%�Ľ�����
//2024604	����	����		�����޼�	�����к���������5���������ԭʼ�������ֵ16%��ԭʼ�������ֵ16%�Ľ�����
//2024605	����	����		�����޼�	�����к���������6���������ԭʼ�������ֵ20%��ԭʼ�������ֵ20%�Ľ�����

#define BuffTeamProfit		90001
#define BuffTeamProfit2		9000101
#define BuffTeamProfit3		9000102
#define BuffTeamProfit4		9000103
#define BuffTeamProfit5		9000104
#define BuffTeamProfit6		9000105

#define BuffFriExpProfit	90002
#define BuffFriExpProfit2	9000201
#define BuffFriExpProfit3	9000202
#define BuffFriExpProfit4	9000203
#define BuffFriExpProfit5	9000204
#define BuffFriExpProfit6	9000205

#define BuffFriBaseProfit	90003
#define BuffFriBaseProfit2	9000301
#define BuffFriBaseProfit3	9000302
#define BuffFriBaseProfit4	9000303
#define BuffFriBaseProfit5	9000304
#define BuffFriBaseProfit6	9000305

#pragma pack(pop)
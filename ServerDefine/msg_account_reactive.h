#pragma once
#pragma pack(push, 1)

enum EAccountReactiveIdentity
{
	EARI_Null = 0,
	EARI_Intro = 1,
	EARI_BeIntro = 2,
};

struct tagAccountReactiveInfo
{
	EAccountReactiveIdentity eIdentity; // 1: ������ 2:�������� 0:�޲����ʸ����
	DWORD dwKey;	// �ǽ����˵Ļ����������Ļ�����
	INT nScore; // �ǽ����˵Ļ����������Ļ��̻���
	BYTE byRewardLevel; // ��ȡ���˼�������

	tagAccountReactiveInfo() { ZeroMemory(this, sizeof(this));}
};

// �Ϸ����ߺϲ�account_reactiver
struct tagAccountReactiverInfo
{
	DWORD dwKeyCode; 
	BYTE byLevel;
	DWORD dwExp;

	tagAccountReactiverInfo() { ZeroMemory(this, sizeof(this));}
};

CMD_START(NDBC_AccountReactiveCode)
	DWORD dwAccountID;
	DWORD dwKeyCode;
	BYTE byLevel;
	DWORD dwExp;
CMD_END

CMD_START(NDBS_AccountReactiveCode)
	DWORD dwErrorCode;
	INT nScore;
	DWORD dwAccountID; // ������
	DWORD dwCAccountID; // ������
CMD_END

CMD_START(NDBC_InitAccountReactiveInfo)
	DWORD dwAccountID;
CMD_END

CMD_START(NDBS_InitAccountReactiveInfo)
	DWORD dwAccountID;
	DWORD dwRoleID; 
	tagAccountReactiveInfo info;
CMD_END

CMD_START(NDBC_UpdateRewardReactiveLevel)
	DWORD dwAccountID;
	BYTE byLevel;
CMD_END

#pragma pack(pop)
#pragma once
#pragma pack(push, 1)


#define MIN_PULSE_LEVEL 50
#define MAX_PULSE_TIMEONEDAY 100

struct tagPulseProto
{
	DWORD	dwId;
	
	DWORD	dwPageType;	// ��ҳ����
	DWORD	dwPos;				// ҳ��λ��
	DWORD	dwLevel;

	DWORD	dwGodLock;
	DWORD	dwLevelLock;
	DWORD	dwMaxLevel;
	DWORD	dwSkillID;
	bool			bIsAciveSkill;
	DWORD	dwSuccessRate;
	DWORD	dwItemID;
	DWORD	dwItemNum;
	DWORD	dwItemID2;	//�¼ӵĵ���
	DWORD	dwItemNum2;	//�¼ӵĵ�����Ŀ
	DWORD	dwMoney;

	DWORD	dwPerSkill1;
	DWORD	dwPerSkill2;
	DWORD	dwPerSkill3;

	DWORD	dwNextSkill1;
	DWORD	dwNextSkill2;
	DWORD	dwNextSkill3;
};

struct tagPulseClientProto : tagPulseProto
{					
	bool					bIsSpecSkill;							// �Ƿ���������������ͼ�꣩
	TCHAR				szName[X_LONG_NAME];		//����
	TCHAR				szIcon[X_LONG_NAME];		//ͼ��
	TCHAR				szSkillDesc[X_LONG_NAME];		//˵��
	TCHAR				szNextSkillDesc[X_LONG_NAME];	//������������
};
#pragma pack(pop)
//-----------------------------------------------------------------------------
//!\brief  ��ħ�������
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)

//�����ȡʧ��ԭ��
enum
{
	E_SOUL_JOB								= 1,		//ְҵ������
	E_SOUL_POINT							= 2,		//����������
	E_SOUL_PRESKILL							= 3,		//ǰ�ü��ܲ�����
	E_SOUL_SOUL								= 4,		//��ħ�겻����
	E_SOUL_MONEY							= 5,		//��Ǯ������
	E_SOUL_SKILL							= 6,		//���ܲ�����
	E_SOUL_ITEM								= 7,		//���߲�����
};

//���ħ���Լ��������ı�
CMD_START(NS_ChangeSoul)
	INT nGod;					//���
	INT nMonster;				//ħ��
	INT nGodPoint;				//�������
	INT nMonsterPoint;			//ħ������
CMD_END


//������ħ����
CMD_START(NC_ActiveSoulSkill)
	DWORD dwSkillID;			//����ID
CMD_END

CMD_START(NS_ActiveSoulSkill)
	DWORD	dwSkillID;			//����ID
	DWORD	dwTime;				//������ʱ��
	DWORD	dwResult;			//���
CMD_END


//������ħ����ʱ��
CMD_START(NC_ContinueSoulSkill)
	DWORD dwSkillID;			//����ID
	INT64 n64ItemID;			//��ƷID
CMD_END

CMD_START(NS_ContinueSoulSkill)
	DWORD dwSkillID;			//����ID
	DWORD dwResult;				//���
	DWORD dwTime;				//��Լ���ʱ��(��)
CMD_END

#pragma pack(pop)


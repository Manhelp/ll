#pragma once
#include "BaseData.h"
#include "..\WorldDefine\skill_define.h"
struct tagExperience
{
	INT			nLevel;					//�ȼ�
	INT64			nExp;					//��һ����Ҫ�ľ���
	INT			nAtt[X_ERA_ATTA_NUM];	//һ������
	INT			nCost[ESCT_End];		//���������Ե����ֵ
    INT         nRoleAttAvail;          // ���Ե�
    INT         nTalentAvail;           // ���ʵ�
	INT			nTalentAvail_limit;		// ��ǰ�������ʵ�����
	
	tagExperience(void)
	{
		memset(this, 0x0, sizeof(tagExperience));
		nTalentAvail_limit = GT_INVALID;
	}
};

class LevelUpData :
	public BaseData
{
public:
	LevelUpData(void);
	~LevelUpData(void);

	static LevelUpData* Inst();

	/** �����ɫ�������������ļ�
	*/
	virtual void LoadFromFile();

	const tagExperience* FindExpData(DWORD dwLevel)
	{
		return FindInMap(m_mapExp, dwLevel);
	}

    // ����������ӵ�����ֵ
    int GetRoleAttInc(ERoleAttribute eType, DWORD dwLevel);
    

    

private:
	map<DWORD, tagExperience>			m_mapExp;
};
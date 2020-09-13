#pragma once
//#include "world.h"
//#include "../WorldDefine/family_define.h"

struct tagFamilyQuestLoadData;

// ���ݼ��徫��ȼ���ü����������
#define GetFamilyQuestNum(spirite_level, num) \
	if(spirite_level>=1 && spirite_level<20) num = 1; \
	else if(spirite_level>=20 && spirite_level<40) num = 2; \
	else if(spirite_level>=40 && spirite_level<60) num = 3; \
	else if(spirite_level>=60) num = 4;

struct tagFamilyQuestDetail : tagFamilyQuest
{
	const tagFamilyQuestProto*		m_pQuestProto;

	tagFamilyQuestDetail()
	{
		ZeroMemory(this, sizeof(this));
	}
};

class Family;
class FamilyQuest
{
	// ���ˢ����ز���
	VOID	SetRenewFlag();
	
	// ���㵱ǰ�������������ɶ�
	INT		CalCompletePercent();

public:
	~FamilyQuest(){ Destroy(); }

	VOID	Init(Family* p, bool bNewFamily = false);
	VOID	Init(tagFamilyQuestLoadData* pData);
	VOID	Destroy();
	
	VOID	SaveFamilyQuestDetail();
	VOID	Update();

	VOID	RenewQuest();
	INT		GetYesterdayFamilyQuestProcess() { return m_nProcess; }
	VOID	FillFamilyQuestData(BYTE*);
	BYTE	GetQuestNum() { return m_byQuestNum; }
	VOID	SetInitOK() { m_bInitOk = TRUE; }

	VOID	IncFamilySpiriteExp();

	// ɱ�ּ���
	VOID	OnAnyMemberKillMonster(DWORD dwMonsterID, INT nMonsterLevel, DWORD dwMapCrc);
	// �Ͻ���Ʒ
	DWORD	OnAnyMemberGiveInItem(DWORD dwRoleID, DWORD dwQuestID, DWORD dwTypeID, INT64 n64Serial, INT nNum, INT& nSum);

	DWORD	CanGetReward(DWORD dwRoleID);
	BOOL	CanGetItemRewardI();
	BOOL	CanGetItemRewardII();
	
private:
	Family*					m_pFamily;						// ��������
	BOOL						m_bInitOk;						// �Ƿ��Գɹ���ʼ��
	tagFamilyQuest*		m_pQuests;						// �����еļ�������
	BYTE						m_byQuestNum;				// �����еļ����������
	INT							m_nProcess;					// ���ڵ�������ɶ�
	bool							m_bIssueState;				// ����״̬,falseδ���� true�ѷ���
	tagDWORDTime		m_dwLatestRenewTime;	// һ����ֹ�ظ�ˢ�������ʱ��
	DWORD					m_dwLatestSaveTick;		// ���һ�α������ݿ��tick

	CRITICAL_SECTION	m_renewlock;
};

inline VOID FamilyQuest::FillFamilyQuestData(BYTE* pTargetMem)
{
	if(!P_VALID(pTargetMem))
		return;

	tagFamilyQuest* pMem = (tagFamilyQuest*)pTargetMem;
	for (int i=0;i<m_byQuestNum;++i)
	{
		if(P_VALID(m_pQuests+i))
		{
			IFASTCODE->MemCpy(pMem, m_pQuests+i, sizeof(tagFamilyQuest));
			++pMem;
		}
	}
}
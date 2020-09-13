//--------------------------------------------------------------------------------
//!\file quest_board.h
//!\author zhangrong
//!
//!\date 2010-06
//! last 2010-06
//!
//!\brief �����ͷ�ļ�
//!
//--------------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/msg_quest_board.h"
#include "../ServerDefine/msg_quest.h"
#include "db_session.h"

//class Role;

#define BOARD_QUEST_MAX_RULE_NUM	17	// ���ɹ�������
#define	IsBoardQuest(u16ID)			( u16ID >= QUEST_PANEL_START_TASK_ID && u16ID <= QUEST_PANEL_END_TASK_ID )
#define	IsRewardBoardQuest(u16ID)	( u16ID >= QUEST_PANEL_START_TASK_ID && u16ID < QUEST_PANEL_START_TASK_ID + QUEST_BOARD_MAX_CAPACITY)
#define	IsYellowListQuest(u16ID)	( u16ID >= YELLOW_LIST_START_TASK_ID && u16ID < YELLOW_LIST_START_TASK_ID + QUEST_BOARD_MAX_CAPACITY)

// �����ˢ�·�ʽ
enum EBoardRefreshType
{
	EBRT_CompleteQuest	= 0,			// ������
	EBRT_IM				= 1,			// ʹ�õ���
	EBRT_Level			= 2,			// �ȼ���Ӧ
};

// ������ϻ�õ����������Ľṹ
struct tagRoleBoardQuest
{
	UINT16				u16ID;			// ��ӦԤ������������ID
	UINT16				u16BoardID;		// �����ID
	EBoardQuestState	eState;			// ״̬
	
	tagRoleBoardQuest(INT16 a, UINT16 b, EBoardQuestState c)
	{
		u16ID = a;
		u16BoardID = b;
		eState = c;
	}
};

typedef TMap<UINT16, tagRoleBoardQuest*> RoleBoardQuestMap;

// �����������ɷ���ṹ
struct tagBoardQuestRandomRate
{
	BYTE byIndex;
	INT16 n16WhiteRate;
	INT16 n16YellowRate;
	INT16 n16GreenRate;
	INT16 n16BlueRate;
	INT16 n16OrangeRate;
	INT16 n16PurpleRate;
};

// ��������������
class BoardQuestMgr
{
public:
	BoardQuestMgr(Role*);
	~BoardQuestMgr();
	
	// ��ʼ��
	VOID Init(BYTE byLevel, EQuestBoardType eQuestBoardType)
	{
		m_byLevel = byLevel;
		m_eQuestBoardType = eQuestBoardType;
		if (eQuestBoardType == EQBT_Reward)
			m_n16QuestPanelStartTaskID = QUEST_PANEL_START_TASK_ID;
		else if (eQuestBoardType == EQBT_YellowList)
			m_n16QuestPanelStartTaskID = YELLOW_LIST_START_TASK_ID;
	}
	BOOL Init(const BYTE* &pData, INT nNum, Role* pRole, BOOL bMovePointer = TRUE);

	// ������Ƿ�Ϊ��
	BOOL IsBoardEmpty()						{ return m_BoardQuest.Empty();		}	
	BOOL IsHaveThisQuest(UINT16);

	// ����ɾ������
	VOID Add(UINT16 u16ID, INT16 u16BoardID, EBoardQuestState eState = EQS_NotAccept);
	VOID Del(UINT16 u16ID);

	// Get����
	RoleBoardQuestMap& GetRoleBoardQuest()	{ return m_BoardQuest;				}

	// �޸�����״̬�Ľӿ�
	VOID ChangeQuestState(UINT16 u16ID, EBoardQuestState eState);

	// ��������ID������Ӧ�������ID
	UINT16 GetBoardIDFromID(UINT16 u16ID);
	BYTE GetQuestBoardLevel()				{ return m_byLevel;					}
	INT GetCompleteReFreshTime()			{ return m_nCompleteRefreshTime;	}
	INT GetIMRefreshTime()					{ return m_nIMRefreshTime;			}
	INT GetQuestMaxCnt(Role* pRole);

	// ˢ�������
	VOID RefreshBoard(Role*pRole, EBoardRefreshType eType, INT64 n64Serial=0, DWORD dwNPCID=GT_INVALID);
	BOOL RefreshBoardByQuestColor(Role* pRole, BYTE byLevel, EBoardQuestQuality eColor, INT nNum, UINT16 nQuestID);

	// ˢ�´����ۼӼ���ֵ
	VOID IncreaseCompleteRefreshTime()		{ m_nCompleteRefreshTime += 1;		}
	VOID IncreaseIMRefreshTime()			{ m_nIMRefreshTime += 1;			}
	VOID SetCompleteRefreshTime(INT nValue)	{ m_nCompleteRefreshTime = nValue;	}
	VOID SetIMRefreshTime(INT nValue)		{ m_nIMRefreshTime = nValue;		}
	VOID SetBoardQuestLevel(BYTE byLevel)	{ m_byLevel = byLevel;				}

	// ���ݵ�ǰ��ˢ�´�����ñ���ˢ��Ҫʹ�õĹ��� 
	tagBoardQuestRandomRate* GetNowtimeRule(EBoardRefreshType eType, BOOL bSecond/* �Ƿ�Ҫʹ�ö���ˢ�¹���*/);
	tagBoardQuestRandomRate* GetRewardQuestRule(EBoardRefreshType eType, BOOL bSecond/* �Ƿ�Ҫʹ�ö���ˢ�¹���*/);
	tagBoardQuestRandomRate* GetYellowListRule(EBoardRefreshType eType, BOOL bSecond/* �Ƿ�Ҫʹ�ö���ˢ�¹���*/);

	VOID SaveBoardQuest2DB();

	INT GetScriptDataIdx()
	{
		if (m_eQuestBoardType == EQBT_Reward)
			return RewardBoardQuestLvlIdx;
		else if (m_eQuestBoardType == EQBT_Guild)
			return GuildBoardQuestLvlIdx;
		else if (m_eQuestBoardType == EQBT_YellowList)
			return YellowListBoardQuestLvlIdx;
		return -1;
	}
private:
	Role*				m_pRole;					// ��������������
	BYTE				m_byLevel;					// �����ȼ�����¼����������еȼ���С������ĵȼ�
	RoleBoardQuestMap	m_BoardQuest;				// ��ɫ���ϵ����������
	INT					m_nCompleteRefreshTime;		// ͨ�����������ˢ�µĴ���
	INT					m_nIMRefreshTime;			// ͨ��ʹ��IM���߻��ˢ�µĴ���
	EQuestBoardType m_eQuestBoardType;	// ����������
	UINT16			m_n16QuestPanelStartTaskID;	// �������ʼ����ID
};

inline VOID BoardQuestMgr::Add(UINT16 u16ID, INT16 u16BoardID, EBoardQuestState eState /* = EQS_NotAccept */)
{
	tagRoleBoardQuest* p = new tagRoleBoardQuest(u16ID, u16BoardID, eState);
	if (!P_VALID(p))
	{
		return;
	}

	if(m_BoardQuest.IsExist(u16ID))
		return;

	m_BoardQuest.Add(u16ID, p);
}

inline VOID BoardQuestMgr::Del(UINT16 u16ID)
{
	tagRoleBoardQuest* p = m_BoardQuest.Peek(u16ID);
	if (P_VALID(p))
	{
		SAFE_DEL(p);
	}

	m_BoardQuest.Erase(u16ID);
}

inline UINT16 BoardQuestMgr::GetBoardIDFromID(UINT16 u16ID)
{
	tagRoleBoardQuest* p = m_BoardQuest.Peek(u16ID);
	if(!P_VALID(p)) return (UINT16)-1;
	return p->u16BoardID;
}
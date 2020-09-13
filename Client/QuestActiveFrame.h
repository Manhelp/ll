/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:36
	filename: 	d:\loong_client\Client\QuestActiveFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestActiveFrame
	file ext:	h
	author:		leili
	
	purpose:	�̶�������������
*********************************************************************/
#pragma once

#include "QuestQuery.h"
#include "ActivityMgr.h"

class QuestActiveFrame : public GameFrame
{
public:
	QuestActiveFrame(void);
	~QuestActiveFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	VOID		 Show(void);
	VOID		 Hide(void)		{ if( P_VALID(m_pWnd) ) m_pWnd->SetInvisible(TRUE); }

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	DWORD		 GetActivityColour(ENewActivityType eType);
	VOID		 ShowQuest(void);
	VOID		 ShowDesc(UINT16 questID);
	VOID		 ShowDailyDesc(UINT16 questID); //��ʾ�ճ�����˵��ר��
	VOID		 ShowFestival();
	bool		 CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
	{
		if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) > 0)
			return true;
		return false;
	}
	bool		 SameStartTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
	{
		if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) == 0)
			return true;
		return false;
	}
	bool		 CompareDay(int SrcMonth, int SrcDay, int DestMonth, int DestDay)
	{
		if(SrcMonth > DestMonth
		||(SrcMonth == DestMonth && SrcDay >= DestDay )) 
			return true;

		return false;
	}

private:
	TSFPTrunk<QuestActiveFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;

	GUIWnd						*m_pWnd;				//������
	GUIListBox					*m_pLbActivityList;		// �����б�
	GUIPushButton				*m_pPbToday;            // ���ջ��ť
	GUIPushButton				*m_pPbDaily;			// �ճ�����ť

	GUIPatch					*m_pStTodayDescOne;          // ��һ������
	GUIPatch					*m_pStTodayDescTwo;          // �ڶ�������
	GUIPatch					*m_pStTodayDescThree;        // ����������

	GUIPatch					*m_pStDailyDescOne;          // ��һ������
	GUIPatch					*m_pStDailyDescTwo;          // �ڶ�������
	GUIPatch					*m_pStDailyDescThree;        // ����������

	GUIListBox					*m_pLbTodayList;		// ���ջ�б�
	GUIListBox					*m_pLbDailyList;		// �ճ���б�
	GUIStaticEx					*m_pStDesc;             // ������������
	GUIPushButton				*m_pPshSwitchCan;		// ���˿����
	GUIButton					*m_pBtnCloseWnd;		// �رմ���X��
	GUIButton					*m_pBtnCloseWndButton;	// �رմ��ڰ�ť

	BOOL						m_bIfPbTodayFlash;        //���ջ��ť�Ƿ���Ҫ��˸�ж�

	ActivityMgr::ESortType		m_curSortType;
	DWORD						m_curRow;
	DWORD						m_currentRowforActivity;
	bool						m_showCan;
	ActivityMgr::ESortType		m_sortType;
	std::vector<int>			m_curShowQuestIDs; 
	std::map<int,int>           m_curShowQuestPosIDMap; // ��ǰ����λ�ú�ID���ձ�,Ϊ���ջ�б��ر�����
	std::map<int,int>           m_curShowActivityIDMap; // ��ǰ����λ�ú�ID���ձ�,Ϊ���ջ�б��ر�����
};
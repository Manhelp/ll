//-----------------------------------------------------------------------------
// filename: force_breakout_apprentice_mgr.h
// author: 
// data: 
// last:
// brief: ǿ��������ɵ�ͽ�ܹ�����
//-----------------------------------------------------------------------------
#pragma once

#include "..\WorldDefine\master_apprentice.h"
#include "..\ServerDefine\master_apprentice.h"
#include "..\WorldDefine\msg_master_apprentice.h"

#define DAY_SECOND_COUNT 86400  // 24x60x60

class CForceBreakOutApprentice
{
public:
	CForceBreakOutApprentice(DWORD dwApprenticeID, DWORD dwMasterID, tagDWORDTime dwordBreakoutTime)
		:m_dwApprenticeID(dwApprenticeID),
		m_dwMasterID(dwMasterID),
		m_BreakoutInsertTime(dwordBreakoutTime)
	{
	}

	~CForceBreakOutApprentice(){}

	// ����ֵ TRUE����ʱ����  FALSE����û��ʱ����
	BOOL	OnMinute();

	// ִ�н�ɫ�˳�ʦ�Ų���
	VOID	ApprenticeBreakOut();

	// �����ݴ������ݿ�
	VOID	InsertApprenticeData2DB();

	// ɾ�����ݿ��е�����
	VOID	RemoveApprenticeFromDB();

	DWORD	GetApprenticeID() { return m_dwApprenticeID; }

	DWORD	GetRemainTime()
	{
		DWORD dwRemainTime = CalcTimeDiff(GetCurrentDWORDTime(), m_BreakoutInsertTime);
		if (dwRemainTime > DAY_SECOND_COUNT)
		{
			dwRemainTime = DAY_SECOND_COUNT;
		}
		// ʱ��Ҫת��ΪСʱ
		dwRemainTime = 24 - ((dwRemainTime)/3600);
		return dwRemainTime;
	}

private:
	DWORD			m_dwApprenticeID;			// ͽ��ID
	DWORD			m_dwMasterID;				// ʦ��ID
	tagDWORDTime	m_BreakoutInsertTime;		// ͽ������ǿ������ʦ��ʱ���ʱ��
};

class CForceBreakOutApprenticeMgr
{
public:
	CForceBreakOutApprenticeMgr(){}
	~CForceBreakOutApprenticeMgr();

	VOID	OnMinute();

	// ���ǿ������ʦ�ŵ�ͽ�ܵ�����
	VOID	InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID);

	VOID	Init();

	VOID	InitApprenticeList(DWORD dwForceBreakOutApprenceCnt, tagForceBreakOutApprentice *pForceBreakOutApprentice);
	
	BOOL	FindApprentice(DWORD dwApprenticeID, INT& dwRemainTime);

private:
	
	// �������ݿ��д洢��ǿ������ʦ�������б�
	VOID	LoadApprenticeListFromDB();

	// �ͷ�ǿ������ʦ�������б�
	VOID	DestoryApprenticeList();

	VOID	InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID, tagDWORDTime dwordBreakoutTime, BOOL bNeedSave2DB = TRUE);

private:

	TList<CForceBreakOutApprentice*> m_listForceBreakOutApprentice; // �Ѿ�������ǿ������ʦ�ŵ�ͽ�ܵ�ID�б�
};

extern CForceBreakOutApprenticeMgr g_forceBreakOutApprenticeMgr;
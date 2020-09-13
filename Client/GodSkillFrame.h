#pragma once
#include "GodSkillDataMgr.h"


/*********************
//!brief:�ɽ����м��ܽ���
*********************/



class IconStatic;

struct tagNS_GetConsumptiveStrength;

const int SKILL_NUM_PER_PAGE = 12;

class GodSkillFrame : public GameFrame
{
public:
	GodSkillFrame();
	~GodSkillFrame();
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	void OnSkillButtonClick(int nPos);
	void OnSkillLearnClick();
	void SetDefaultUI();
	void UpdateUI();
	DWORD OnEventRefresh(tagGameEvent *pEvent);
	DWORD OnNetGetConsumptiveStrength(tagNS_GetConsumptiveStrength *pNetCmd, DWORD);

private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodSkillFrame> m_trunk;
	GUIWnd *m_pWnd;
	IconStatic *m_pIstSkillImg;		// ����ͼƬ

	GUIStatic  *m_pStcSkillName;	// ��������
	GUIStatic  *m_pStcSkillLv;		// ���ܵȼ�
	GUIStatic  *m_pStcOwnGold;		// ��ǰӵ�н�
	GUIStatic  *m_pStcOwnSilver;	// ��ǰӵ����
	GUIStatic  *m_pStcCurStrength;	// ��ǰʵ��ֵ
	GUIStatic  *m_pStcCurCount;		// ��ǰ����������
	GUIProgress *m_pProgress;		// ��������
	
	GUIButton  *m_pBtnPre;			// ��ǰ��ҳ
	GUIButton  *m_pBtnNext;			// ���ҳ
	GUIStatic  *m_pStcCurPageNum;	// ��ǰҳ��
	
	GUIStatic  *m_pStcGold;			// �������������
	GUIStatic  *m_pStcSilver;		// ��������������
	GUIStatic  *m_pStcStrength;		// ������������ʵ��
	GUIStatic  *m_pStcItem;			// �����������

	GUIButton  *m_pBtnLearn;		// ��������
	GUIButton  *m_pBtnClose;		// �ر�
	GUIButton  *m_pBtnX;			// X
	
	IconStatic *m_pIstSkillBacks[SKILL_NUM_PER_PAGE];
	IconStatic *m_pIstSkills[SKILL_NUM_PER_PAGE];	
	GUIStatic  *m_pStcSkillNames[SKILL_NUM_PER_PAGE];
	GUIStatic  *m_pStcSkillGrades[SKILL_NUM_PER_PAGE];

	int m_nCurItem;				// ��ǰѡ�еļ���
	GodSkillDataMgr::EGodSkillLevel  m_eCurGodSkillLv;		// �ɽ缼�ܵȼ�
	int m_nCurPageNum;				// ��ǰҳ��
	int m_nTotalPageNum;			// ��ҳ��
	DWORD m_dwCurTime;
};

#pragma once
#include "gameframe.h"


/*********************
//!brief:����������������
*********************/

class IconStatic;
struct tagNS_RoleExpChange;
const int SOAR_SKILL_NUM_PER_PAGE = 12;

class SoarSkillFrame :
	public GameFrame
{
public:
	SoarSkillFrame(void);
	~SoarSkillFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	void OnSkillButtonClick(int nPos);
	void OnSkillLearnClick();
	void SetDefaultUI();
	void UpdateUI();
	DWORD OnEventRefresh(tagGameEvent *pEvent);
	DWORD OnNS_RoleExpChange(tagNS_RoleExpChange* pNetCmd, DWORD);
public:
	VOID SetInvisable();

private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<SoarSkillFrame> m_trunk;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	GUIWnd *m_pWnd;
	IconStatic *m_pIstSkillImg;		// ����ͼƬ

	GUIStatic  *m_pStcSkillName;	// ��������
	GUIStatic  *m_pStcSkillLv;		// ���ܵȼ�
	GUIStatic  *m_pStcOwnGold;		// ��ǰӵ�н�
	GUIStatic  *m_pStcOwnSilver;	// ��ǰӵ����
	GUIStatic  *m_pStcCurStrength;	// ��ǰ����ֵ
	GUIStatic  *m_pStcCurCount;		// ��ǰ����������
	GUIProgress *m_pProgress;		// ��������

	GUIButton  *m_pBtnPre;			// ��ǰ��ҳ
	GUIButton  *m_pBtnNext;			// ���ҳ
	GUIStatic  *m_pStcCurPageNum;	// ��ǰҳ��

	GUIStatic  *m_pStcGold;			// �������������
	GUIStatic  *m_pStcSilver;		// ��������������
	GUIStatic  *m_pStcStrength;		// ������������ʵ��
	GUIStatic  *m_pStcItem;			// �����������
	GUIStatic  *m_pStcTips;			// С��ʾ

	GUIButton  *m_pBtnLearn;		// ��������
	GUIButton  *m_pBtnClose;		// �ر�
	GUIButton  *m_pBtnX;			// X

	IconStatic *m_pIstSkillBacks[SOAR_SKILL_NUM_PER_PAGE];
	IconStatic *m_pIstSkills[SOAR_SKILL_NUM_PER_PAGE];	
	GUIStatic  *m_pStcSkillNames[SOAR_SKILL_NUM_PER_PAGE];
	GUIStatic  *m_pStcSkillGrades[SOAR_SKILL_NUM_PER_PAGE];

	GUIStatic  *m_pStcNowExpText;		
	GUIStatic  *m_pStcNeedExpText;			
	int m_nCurItem;				// ��ǰѡ�еļ���
	int m_nCurPageNum;				// ��ǰҳ��
	int m_nTotalPageNum;			// ��ҳ��
	DWORD m_dwCurTime;

};

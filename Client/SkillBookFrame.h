#pragma once
#include "SkillItem.h"
struct tagUpdateLPSkillEvent;

const INT MAX_SKILL_BOOK_NUM = 8;


class SkillBookFrame : public GameFrame
{
	enum ECurDisplaySkillType
	{
		ECDST_Active,			//��������
		ECDST_Passive,			//��������
		ECDST_Produce,			//��������
		ECDST_Guild,			//���Ἴ��
		ECDST_GodArea,			//������
		ECDST_OtherPassive		//������������
	};

public:
	SkillBookFrame();
	~SkillBookFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	
	// �����ܽ����
	VOID OnSkillBookOpen();

private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);

	// ������Ϸ�¼�
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnWeaponSwap(tagGameEvent* pGameEvent);
	

	// �Ƿ��ǵ�ǰҳ
	BOOL IsCurPage(INT nIndex){return nIndex >= m_nCurPage*MAX_SKILL_BOOK_NUM 
		&& nIndex < (m_nCurPage+1)*MAX_SKILL_BOOK_NUM;}
	
	// ���õ�ǰҳ��
	VOID SetCurPage(INT nPage);
	
	// ����ҳ�¼�
	VOID OnPageUp(BOOL bUpPage);

	// ˢ�µ�ǰҳ
	VOID ReflashCurPage();

	/** ��ǰ��ʾ�ļ������ͣ�������ҳ
	*/
	INT GetMaxPages();

	/** У����ǰҳ
	*/
	void CheckCurPage();

private:

	TSFPTrunk<SkillBookFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pStcPage;
	GUIButton*					m_pBtnPageUp;
	GUIButton*					m_pBtnPageDown;

	GUIPushButton*              m_activesSkill;			//�������ܰ�ť
	GUIPushButton*              m_passiveSkill;			//�������ܰ�ť
	GUIPushButton*				m_produceSkill;			//��������
	GUIPushButton*				m_GuildSkill;			//��������
	GUIPushButton*				m_GuildGodArea;			//������
	GUIStaticEx*                m_pProduceNotice;       //�����������ûѧ�Ļ�������ʾ��ʾ��Ϣ��

	SkillItem					m_SkillItem[MAX_SKILL_BOOK_NUM];

	TList<DWORD>				m_skillList;			//���������б���
	//TList<DWORD>::TListIterator m_skillIter;			//���������б������

	TList<DWORD>                m_PassiveSkillList;		//���������б���
	//TList<DWORD>::TListIterator m_PassiveSkillIter;		//���������б������

	TList<DWORD>				m_lstProduceSkill;		//���������б�
	//TList<DWORD>::TListIterator m_iterProduceSkill;		//���������б������

	TList<DWORD>				m_lstGuildSkill;			//���Ἴ��

	TList<DWORD>				m_lstGodSkill;			//������

	TList<DWORD>				m_lstOtherPassiveSkill;			//�������������б�
	//TList<DWORD>::TListIterator m_iterlstOtherPassiveSkill;		//�������������б������


	INT							m_nCurPage;
	//BOOL                        m_isActiveSkill;    //--ֵΪTRUE��ʾ��ʾ�������ܣ�ΪFALSE��ʾ��ʾ�������ܣ���ʼΪTRUE��ʾ��������
	
	ECurDisplaySkillType		m_eCurSkillType;
};

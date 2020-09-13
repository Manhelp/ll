#pragma once
#include "gameframe.h"

class SpecialTalkFrame :public GameFrame
{

public:
	enum ESpecialTalkOptionType
	{
		ESTOT_None			= -1,		
		ESTOT_WalkWare					// ����ֿ⣬û�취������Ӧ�üӵ��⣬������ֿ��Ҫ����Ҫ���������
	};

public:
	SpecialTalkFrame(void);
	~SpecialTalkFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 SetCaption( LPCTSTR szCaption );
	VOID		 SetContent( LPCTSTR szContent );
	VOID		 ClearOptions();
	VOID		 AddTalkOption( LPCTSTR szOption, DWORD param1, DWORD param2, ESpecialTalkOptionType optionType );

	VOID		 SetTarget(INT targetType, DWORD targetID) { m_targetType=targetType;m_targetID=targetID; }
	VOID Show();

private:
	BOOL		 EventHandler( tagGUIEvent* pEvent );

private:
	TSFPTrunk<SpecialTalkFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd						*m_pWnd;				//������
	GUIPatch					*m_pCaption;			//����
	GUIStaticEx					*m_pStcCondition;		//�Ի����ݴ���
	GUIListBox					*m_pListOptions;		//�Ի�ѡ��
	GUIButton					*m_pBtnCloseWnd;		//�رս��水ť����

	DWORD						m_curRow;
	INT							m_targetType;
	DWORD						m_targetID;

	struct tagOptAttr 
	{
		DWORD		id;
		DWORD		step;
		ESpecialTalkOptionType	type;
	};
	vector<tagOptAttr>			m_optAttrs;
};

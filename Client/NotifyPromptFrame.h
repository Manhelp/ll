#pragma once
#include "gameframe.h"
#include "QuestMgr.h"


/** ��������������Ϣ�����ƴ�ģ���Ƿ񱻵�������Ҫ���ڷ������������Ĺ㲥
	\brief �ͻ����ɽű����ƴ�ģ��
*/
class NotifyPromptFrame : public GameFrame
{

public:
	NotifyPromptFrame(void);
	~NotifyPromptFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	VOID Show( const tstring& strContent );

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID Navigate( tagGUIEvent* pEvent );
	
	
private:
	TSFPTrunk<NotifyPromptFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pMgr;

	vector<tagIDLink>					m_vecIdLinks;			//NPC����������
	vector<tagPosLink>					m_vecPosLinks;			//λ�õ���������

	GUIWnd*								m_pWnd;					//������
	GUIButton*							m_pOk;					//ȷ��
	GUIStaticEx*						m_pStcContent;			//��ʾ���ݵ����
	GUIScrollBar*						m_pScrollBar;			//������
};

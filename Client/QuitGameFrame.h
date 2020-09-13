#pragma once
#include "gameframe.h"


class StaticEx;
struct tagNS_ReturnRoleSelect;
//-----------------------------------------------------------------------------
//!\brief loading frame
//!
//!
//-----------------------------------------------------------------------------
class QuitGameFrame : public GameFrame
{
public:
	QuitGameFrame();
	~QuitGameFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

private:
	TSFPTrunk<QuitGameFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pWndText;				//����ʱ
	GUIButton*					m_pBtnGodCat;			//�򿪾���è
	GUIButton*					m_pBtnQuit;				//�˳���Ϸ
	GUIButton*					m_pBtnCancel;			//ȡ��
	GUIButton*					m_pBtnClose;			//�رհ�ť

	StaticEx*					m_pStcBackImg;			//����ͼƬ
	GUIButton*					m_pBtnOne;				//adv 1
	GUIButton*					m_pBtnTwo;				//adv 2
	GUIButton*					m_pBtnThree;			//adv 3

	DWORD						m_dwStartTime;
	BOOL						m_bSendMsg;

	DWORD						m_dwSwitchTime;			//�л�ͼƬ��ʱ���
	int							m_nBkImgIndex;			//��ʾͼƬ������
	std::vector<tstring>		m_vecImgs;				//����ͼƬ

	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID Logout();
};

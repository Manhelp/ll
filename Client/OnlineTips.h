#pragma once

/*************
\!brief (40������)��ɫ��½��ʾ����
*************/
#include "..\WorldDefine\role_att.h"

class FlashButton;
class OnlineTips : public GameFrame
{
public:
	OnlineTips();
	~OnlineTips();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	BOOL IsShow() const { return m_bShow; }
	void ShowWnd(BOOL bShow);
	

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnRefreshTips(tagGUIEvent *pEvent);
	DWORD NetRecvServerOLInfo(tagNS_SyncOnlineNum* pNetCmd, DWORD dwParam);
	void RefreshNotCompleteQuestsNum();
	void RefreshCanAssignAttNum();
	void RefreshCanAssignTalentNum();
	void RefreshBBYuanBaoNum();
	void RefreshOLNum();

private:
	TSFPTrunk<OnlineTips> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr>	 m_pCmdMgr;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<Util>					m_pUtil;

	GUIWnd *m_pWnd;
	GUIButton *m_pBtnNotCompleteQuestsNum;	// δ�����������
	FlashButton *m_pBtnCanAssignAttNum;		// �ɷ������Ե�
	FlashButton *m_pBtnCanAssignTalentNum;	// �ɷ������ʵ�
	GUIButton *m_pBtnBBYuanBaoNum;			// �ٱ���Ԫ������
	FlashButton *m_pBtnTodayFocus;            // �������Ż
	bool      m_bFirstClick;              //�״ε���������Ż
	GUIButton *m_pBtnTaskQuery;				//�̶�����
  	GUIButton *m_pBtnClose;					// �ر�

    GUIStatic *m_pStcFriendsOL;            //���ߺ���
	GUIStatic *m_pStcGuildMemberOL;            //����ͬ��
    GUIStatic *m_pStcClassMateOL;             //����ʦ��
	INT			m_iFriendsNumOL;            //���ߺ�������
	INT			m_iGuildMemberNumOL;			 //����ͬ������
	INT			m_iClassMateNumOL;				//����ʦ������	

	int m_nLastAttNum;					// �ϴε����Ե�
	int m_nLastTalentNum;				// �ϴε����ʵ�

	GUIButton* m_BnFriendsOnline;
	GUIButton* m_BnGuildOnline;
	GUIButton* m_BnTeacherOnline;

	BOOL m_bShow;
};


#pragma once
#include "gameframe.h"

struct tagNS_GetOfflineExp;

class OfflineExpFrame : public GameFrame
{
	enum EHarvestExpWndType
	{
		EHET_Init,
		EHET_Free,
		EHET_All
	};
public:
	OfflineExpFrame(void);
	~OfflineExpFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();

	void SetData( DWORD dwHour, DWORD dwMinute, INT64 dwFreeExp, INT64 dwTotalExp, DWORD dwYuanbao )
	{
		m_dwOffHour = dwHour;
		m_dwOffMinute = dwMinute;
		m_dwFreeExp = dwFreeExp;
		m_dwTotalExp = dwTotalExp;
		m_dwYuanBao = dwYuanbao;
	}

	void ShowWnd( BOOL bShow )
	{
		if( P_VALID(m_pWnd) )
		{
			m_pWnd->SetInvisible( !bShow );
			m_pWnd->SetRefresh( TRUE );
		}

		if( bShow )
			RefreshContent();
	}

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	
	void RefreshContent();
	
	void SendHarvestExpMessage();
	
	DWORD OnNS_GetOfflineExp( tagNS_GetOfflineExp* pMsg, DWORD dwParam );
	
private:
	TSFPTrunk<OfflineExpFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	EHarvestExpWndType			m_eWndType;				//��ǰ��ʾ���ڵ�����
	bool						m_bWaitServerReturn;	//���ڵȷ���������Ϣ

	GUIWnd*						m_pWnd;
	GUIButton*					m_pBtnGet;				//��ð�ť
	GUIButton*					m_pBtnReturn;			//���ذ�ť �Լ� ��ȡȫ������
	//GUIButton*				m_pBtnClose;			//�رհ�ť
	GUIStatic*					m_pStcTitle;			//����title
	GUIStaticEx*				m_pStcContent;			//��������

	DWORD						m_dwOffHour;			//����Сʱ
	DWORD						m_dwOffMinute;			//���߷���
	INT64						m_dwTotalExp;			//�ܹ�����
	INT64						m_dwFreeExp;			//��Ѿ���
	DWORD						m_dwYuanBao;			//���⾭����Ҫ��Ԫ��
};

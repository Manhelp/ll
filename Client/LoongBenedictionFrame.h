#pragma once
#include "gameframe.h"

/**
	\brief �����͸� �����ֵ�¼��Ϸ����10����Ϊ��λ�������������
		   ��Ҫ��Ϊ�˷�ֹ���ֵ���ʧ
    \auth  dyb
*/
class IconStatic;
class ServerTime;
class GlintStatic;
class FlashButton;
struct tagNS_GetLoongBenediction;
struct tagNS_SetNextBenediction;

class LoongBenedictionFrame : public GameFrame
{
	enum ELoongBenedictionStep
	{
		EBeneStep_Null = -1,
		EBeneStep_Count,			//���մ�����
		EBeneStep_Time				//����ÿ������ʱ����
	};

public:
	LoongBenedictionFrame(void);
	~LoongBenedictionFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	
	void SetBenedictionStep( INT nType )
	{
		m_step = (ELoongBenedictionStep)nType;
		if( P_VALID(m_pBtnClose) )
		{
			if( m_step == EBeneStep_Time )
				m_pBtnClose->SetInvisible( FALSE );
			else
				m_pBtnClose->SetInvisible( TRUE );
		}
	}

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID SafeDestroyUI();

	/** ��ý���
	*/
	VOID GetBenediction();
	
	/** ��������Ի�ȡ�����ķ����¼�
	*/
	DWORD OnNS_GetBenediction( tagNS_GetLoongBenediction* pMsg, DWORD dwParam );

	/** ������������Ϣ������һ�δ͸�ʱ�䣬�Լ���Ʒ��Ϣ
	*/
	DWORD OnNS_SetNextBenediction( tagNS_SetNextBenediction* pMsg, DWORD dwParam );

	/** �жϵ�ǰ�����Ƿ���չ����
	*/
	BOOL IsWindowExpand();

	VOID ShowWnd( GUIWnd* pWnd, BOOL bShow );

	VOID EnableWnd( GUIButton* pBtn, bool bEnable );

	void BeginGlint();

	void EndGlint();

private:
	TSFPTrunk<LoongBenedictionFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	GUIWnd*							m_pWndExpand;
	GUIStatic*						m_pStcWaitTime;
	FlashButton*					m_pBtnGetBenediction;
	GUIButton*						m_pBtnArrow;			//�۵�
	//GUIStatic*						m_pStcExprence;			//��õľ���

	IconStatic*						m_pStcItem;				//��õĵ���
	GUIButton*						m_pBtnClose;			//�رհ�ť

	GUIWnd*							m_pWndFold;					//�۵�����֮��Ľ���
	//GlintStatic*					m_pStcFoldWaitTime;
	FlashButton*					m_pBtnFoldGetBenediction;
	GUIButton*						m_pBtnFoldArrow;			//�۵�
	GUIButton*						m_pBtnFoldClose;			//�رհ�ť
	GUIWnd*							m_pWndCurDisplay;			//�õ���ǰ������ʾ�Ŀؼ�

	DWORD							m_nItemTypeId;				//��Ҫ��õĵ��ߵ�ID
	INT								m_nItemCount;				//������Ʒ�ĸ���

	DWORD							m_dwNeedTime;				//����һ�ν�����Ҫ������
	float							m_fStepTime;				//����ʱ
	DWORD							m_dwStartTimer;				//��ʱ��

	BOOL							m_bCanUpdate;				//�ܷ���£���û���յ������������� ������һ�δ͸�ʱ�� ֮ǰ�ǲ��ܸ��µ�

	ELoongBenedictionStep			m_step;
	BOOL							m_bSendAtZero;				//���󣬻�ͷ�����ͬ��һ��ʱ�䣬�������ݿ��ʱ������һ�ξ�����

};

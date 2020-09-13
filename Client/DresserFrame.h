#pragma once
class PlayerTryDressWnd;

class DresserFrame :
	public GameFrame
{
public:
	DresserFrame(void);
	~DresserFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	virtual VOID Render3D();
	VOID SetNPCID(DWORD id) { m_dwNpcID = id; }

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<DresserFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd						*m_pWnd;				//������
	GUIPatch					*m_pCaption;			//����
	PlayerTryDressWnd			*m_pPlayerView;

	GUIButton*					m_pLBtnHType;		//������ť
	GUIButton*					m_pRBtnHType;		//�����Ұ�ť
	GUIStatic*					m_pStcHType;		//���ͺ���ʾstatic

	GUIButton*					m_pLBtnHColor;		//��ɫ��ť
	GUIButton*					m_pRBtnHColor;		//��ɫ�Ұ�ť
	GUIStatic*					m_pStcHColor;		//��ɫ����ʾstatic

	GUIButton*					m_pLBtnFace;		//�沿��ť
	GUIButton*					m_pRBtnFace;		//�沿�Ұ�ť
	GUIStatic*					m_pStcFace;			//�沿����ʾstatic

	GUIButton*					m_pLBtnFDetail;		//�沿ϸ����ť
	GUIButton*					m_pRBtnFDetail;		//�沿ϸ���Ұ�ť
	GUIStatic*					m_pStcFDetail;		//�沿ϸ�ں���ʾstatic

	GUIButton*					m_pBtnOK;
	GUIButton*					m_pBtnQuit;
	GUIButton*					m_pBtnExit;

	DWORD						m_dwNpcID;
	DWORD						m_escapeTime;

	tagAvatarAtt				m_avatarAtt;		//��ɫ����
};
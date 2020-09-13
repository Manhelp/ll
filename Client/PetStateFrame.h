#pragma once
#include "gameframe.h"

class StaticEx;
class IconStatic;

//! \class  PetStateFrame
//! \brief  ������ҳ���״̬��ʾ
//! \author hyu
//! \base   GameFrame
//! \sa     PetManager
class PetStateFrame :
    public GameFrame
{
public:
    PetStateFrame(void);
    ~PetStateFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
    virtual BOOL EscCancel();
	VOID SetFairyMoodPic(INT nMood);

private:
    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    void ShowWnd(bool bVisable);

    //! ���³�����Ϣ
    void UpdatePet();

	tstring GetFairyMoodState();

    TSFPTrunk<PetStateFrame>    m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    DWORD           m_dwStartTime;          //!< ��ʼ����ʱ���

    //GUI�ؼ�
    GUIWnd*						m_pWnd;
    //GUIStatic*                  m_pStcName;     //!< ����
    StaticEx*                   m_pStcIcon;     //!< ͼ��
    GUIProgress*                m_pPgssEnergy;  //!< ���� // ����
    GUIProgress*                m_pPgssExp;     //!< ���� // ����
	GUIStatic*					m_pStcSaveExp;
	IconStatic *m_pIconFeed;
	IconStatic *m_pIconMood;
	IconStatic *m_pIconLearn;

	INT16 m_nFairyMood;
	INT16 m_nFairyRealMood;
};

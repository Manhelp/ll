#pragma once
#include "gameframe.h"

const int TEXT_MAX_COLUMN = 12;

struct tagNS_RoleLevelChange;

//! \class  LevelUpShowFrame
//! \brief  ����������ʾ
//! \base   GameFrame
//! \author hyu
//! \date   2010-1-18
class LevelUpShowFrame :
    public GameFrame
{
public:
    LevelUpShowFrame(void);
    ~LevelUpShowFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

    void ShowWnd(bool bVisible);
	BOOL IsWndInvisible(){ return m_pWnd->IsInvisible();}

    void OnLevelUp(tagNS_RoleLevelChange* pNetCmd);

private:
    DWORD EventHandler(tagGUIEvent* pEvent);

    // GUI Controls
    GUIWnd*                     m_pWnd;

    //GUIStatic*                  m_pStcMsg[TEXT_MAX_COLUMN];      //!< ��Ϣ��һ��

    //GUIStatic*                  m_pStcCongratulation;            //!< ף��

    GUIButton*                  m_pBtExit;       //!< �ر�

    vector<tstring>             m_vecText;       //!< �����ı�����
    int                         m_nBeginRow;     //!< ��ʼ����

    DWORD                       m_dwStartShowTime;  //!< ��ʼˢ��ʱ��
    DWORD                       m_dwTextUpdate;     //!< ����ˢ�¼��

    // tools
    TSFPTrunk<LevelUpShowFrame> m_Trunk;
    TObjRef<GUISystem>          m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;



};

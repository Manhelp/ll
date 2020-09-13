#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "..\WorldDefine\compose_define.h"
#include "..\WorldDefine\msg_compose.h"

class FashionComposeFrame :
    public GameFrame
{
public:
    FashionComposeFrame(void);
    ~FashionComposeFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
    virtual BOOL EscCancel();

    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

    VOID SetItem(Item* pItem);

private:
    //! ���ķ�����Ϣ
    DWORD OnNS_ConsolidateSpin(tagNS_ConsolidateSpin* pMsg, DWORD);

    //! ���þ�����Ϣ
    //! \para bLeft,�Ƿ���ʾ�����
    VOID SetFashionMsg(bool bLeft, ESpinStep eStep, ESuitAttType eSuitType);

    //! ���ý���
    VOID ClearUI();

    VOID ClearData();
    VOID SetSuccessRate(int rate, bool bClear = false);
    VOID RefreshSuiteState(bool bClear = false);
    VOID RefreshSuiteAttrBegin(int i, bool bClear = false);
    VOID RefreshSuiteAttrEnd(int i, bool bClear = false);

    ESpinStep GetCurrFashionSpinSetp();
    ESuitAttType GetCurrFashionSuitType();
    ESuitAttType GetSelectedFashionSuitType();
    int GetSelectedFashionSpinRate();

    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ֧���������ͻ���ת��
    template<typename T>
    tstring TtoString(T nSrc);

    DWORD                       m_dwNpcId;          // ��ǰnpcid

    INT64                       m_pCurrFashionID;   // ��ǰʱװID
    INT64                       m_pCurrMatearial;   // ��ǰ����ID
    DWORD                       m_pCurrMatearialType;   // ��ǰ����type id
    int                         m_pMatearialCount;  // ��������

    bool                        m_bBegin;           // �Ƿ�ʼ����
    DWORD                       m_dwStartTime;      // ���Ŀ�ʼʱ��
    DWORD                       m_updateTimeDistance;   // �����npc����

    //GUI�ؼ�
    GUIWnd*						m_pWnd;

    ItemButton*                 m_pIbtFashion;      // Ҫ���ĵ�ʱװ
    GUIProgress*                m_pProgress;        // ������

    ItemButton*                 m_pIbtMateiral;     // ���Ĳ���
    GUIButton*                  m_pBtInc;           // �Ӻ�
    GUIButton*                  m_pBtDec;           // ����
    GUIStatic*                  m_pStcSucessRate;   // �ɹ���

    GUIPushButton*              m_pPbtSuit[4];      // ��װѡ��
    GUIStatic*                  m_pStcSuit[4];      // ��װ����

    GUIStatic*                  m_pStcHP[2];        // ����
    GUIStatic*                  m_pStcMP[2];        // ����
    GUIStatic*                  m_pStcVitality[2];  // ����

    GUIStatic*                  m_pStcSavvy[2];     // ����
    GUIStatic*                  m_pStcFortune[2];   // ��»
    GUIStatic*                  m_pStcRein[2];      // ͳ��

    GUIStatic*                  m_pStcPhysique[2];  // ���
    GUIStatic*                  m_pStcStrength[2];  // ����
    GUIStatic*                  m_pStcTechnique[2]; // ����
    GUIStatic*                  m_pStcPneuma[2];    // Ԫ��
    GUIStatic*                  m_pStcInnerForce[2];// ����
    GUIStatic*                  m_pStcAgility[2];   // ��

    GUIStatic*                  m_pStcExAttack[2];  // �⹦����
    GUIStatic*                  m_pStcExDefense[2]; // �⹦����
    GUIStatic*                  m_pStcAttackTec[2]; // ��������
    GUIStatic*                  m_pStcArmor[2];     // ���߻���
    GUIStatic*                  m_pStcDefenseTec[2];// ��������
    GUIStatic*                  m_pStcInAttack[2];  // �ڹ�����
    GUIStatic*                  m_pStcInDefense[2]; // �ڹ�����
    GUIStatic*                  m_pStcWeaponDmgMax[2];// �����������
    GUIStatic*                  m_pStcExDamage[2];    // �����˺�
    GUIStatic*                  m_pStcDerateALL[2]; // ȫ���˺�����
    GUIStatic*                  m_pStcExDamageAbsorb[2];    // �����˺�����
    GUIStatic*                  m_pStcHitRate[2];   // ����
    GUIStatic*                  m_pStcWeaponSoul[2];// ���
    GUIStatic*                  m_pStcDodge[2];     // ����
    GUIStatic*                  m_pStcCritRate[2];  // ����
    GUIStatic*                  m_pStcRegainAddtion[2];      // ����
    GUIStatic*                  m_pStcCritAmount[2];// ������
    GUIStatic*                  m_pStcBlockRate[2]; // ����

    GUIButton*                  m_pBtStart;         // ��ʼ
    GUIButton*                  m_pBtClose;         // �ر�
    GUIButton*                  m_pBtCancel;        // �ر�


    TSFPTrunk<FashionComposeFrame> m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
};

template<typename T>
tstring FashionComposeFrame::TtoString( T nSrc )
{
    if (nSrc <= 0)
        return _T("");

    tstringstream sstream;
    sstream << nSrc;
    tstring ret;
    sstream >> ret;
    ret = _T("+") + ret;
    return ret;
}
#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "PetDef.h"
#include "PetSkillFrame.h"
#include "PetSkillButton.h"
#include "../WorldDefine/pet_skill_define.h"

class StaticEx;

const int MAX_PET_ONLINE_SKILL = 5;
const int MAX_PET_EGG_BUY   = 6;
const tstring STR_SELECT_GOOD   = _T("data\\ui\\Common\\L_icon-l.bmp");
const tstring STR_BACK_GOOD     = _T("data\\ui\\sys\\l_diban-a.bmp");

//! \class  PetSkillAssistanceFrame
//! \brief  �����ڹҼ�������
//! \detail
//! \author hyu
//! \date   2009-1-14
//! \base   GameFrame
//! \sa     PetSkillFrame
class PetSkillAssistanceFrame :
    public GameFrame
{
public:
    PetSkillAssistanceFrame(void);
    ~PetSkillAssistanceFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

    void ShowWnd(bool bVisible);
    bool IsOpen();

private:
    //! ��ʼ��UI
    void InitalUI();
    //! resize combobox
    void ResizeCombobox(GUIComboBox* pComboBox, int rowCount);

    // ˢ�����г���
    void RefreshPets();
    // ��ռ���
    void ClearSkill();
    // ��ѵ��
    void TurnOnSkillUI(EPetskillType eType, DWORD dwTypeID, bool bisOpen);
    // ˢ�µ�ǰ���＼��
    void RefreshPetSkill();
    // ��ȡ������������
    void LoadPetSkillCfg(EPetskillType ePetSkillType);

    // ��ʾ����������
    void ShowBuyWnd(bool visable);

    // ��ȡ���޹����б�
    void LoadPets();

    // ��չ������
    void ClearBuyUI();

    // ˢ����Ʒ�б�
    void RefreshItemBuy();

    //! �������Է����仯
    DWORD OnPetAttrChange(tagPetAttrChange* pEvent);

    DWORD EventHandler(tagGUIEvent* pEvent);

    DWORD BuyEventHandler(tagGUIEvent* pEvent);

    DWORD                       m_CurrPetID;        //!< ��ǰ����ID
    int                         m_nSelectedRow;     //!< ��ǰѡ����
    map<int, DWORD>             m_mapRow2Pet;       //!< ��������ID���ձ�

    int                         m_nCurrItem;        //!< ��ǰѡ�еĵڼ���item

    // GUI Controls
    GUIWnd*                     m_pWnd;

    GUIButton*                  m_pBtClose;         //!< �رմ���
    GUIButton*                  m_pBtBuy;           //!< ��������
    
    GUIComboBox*                m_pCbPetList;       //!< ӵ������

    GUIStatic*                  m_pStcSkillLeft;    //!< ����x���δϰ��

    GUIPushButton*              m_pPbtFight;        //!< ս��
    GUIPatch*                   m_pPchFight;        //!< ս��
    GUIPushButton*              m_pPbtProduction;   //!< ����
    GUIPatch*                   m_pPchProduction;   //!< ����
    GUIPushButton*              m_pPbtOther;        //!< ����
    GUIPatch*                   m_pPchOther;        //!< ����

    // �ɼ�
    PetSkillButton*             m_pPetSkillGather;  //!< �ɼ�����ͼ��
    GUIButton*                  m_pPetLearnGather;  //!< ѧϰ�ɼ�

    // ѵ��
    PetSkillButton*             m_pPetSkillTraining;    //!< ѵ������ͼ��
    GUIButton*                  m_pPetLearnTraining;    //!< ѧϰѵ��
    GUIPushButton*              m_pPbtComposeSkill[ESSTE_End]; //!< �������ܸ�ѡ��
    GUIButton*                  m_pBtTrainingConfirm;       //!< ѵ��

    // �Զ�ʰȡ
    PetSkillButton*             m_pPetSkillPickUp;    //!< ʰȡ����ͼ��
    GUIButton*                  m_pPetLearnPickUp;    //!< ѧϰʰȡ
    GUIPushButton*              m_pBtPickUpFunc[MAX_PICKUP_FUNC]; //!< ���ܿ���
    GUIButton*                  m_pBtPickConfirm;       //!< �ر�

    // ����
    PetSkillButton*             m_pPetSkillMedicare;    //!< ������ͼ��
    GUIButton*                  m_pPetLearnMedicare;    //!< ѧϰ����

    GUIComboBox*                m_pCbRedCondition;      //!< ��ҩ����
    GUIPushButton*              m_pPbtRedEnable;        //!< ��ҩ���ܿ���
    ItemButton*                 m_pBtRed;               //!< ��ҩ

    GUIComboBox*                m_pCbBlueCondition;     //!< ��ҩ����
    GUIPushButton*              m_pPbtBlueEnable;       //!< ��ҩ���ܿ���
    ItemButton*                 m_pBtBlue;              //!< ��ҩ

    GUIComboBox*                m_pCbToughnessCondition;    //!< �־�������
    GUIPushButton*              m_pPbtToughnessEnable;  //!< �־������ܿ���
    ItemButton*                 m_pBtToughness;         //!< �־���ҩ

    // ����
    PetSkillButton*             m_pPetSkillSale;        //!< ��������ͼ��
    GUIButton*                  m_pPetLearnSale;        //!< ѧϰ����

    // ���޹������
    GUIWnd*                     m_pWndBuy;              //!< ������
    GUIStatic*                  m_pStcTitle;            //!< ˵��
    StaticEx*                   m_pStcSelect[MAX_PET_EGG_BUY];  //!< ѡ��װ�
    ItemButton*                 m_pIbtEgg[MAX_PET_EGG_BUY]; //!< ���ﾫ��
    DWORD                       m_dwItemID[MAX_PET_EGG_BUY];//!< ����ID
    GUIStatic*                  m_pStcPrice[MAX_PET_EGG_BUY];   //!< ���ﵰ�۸�
    GUIButton*                  m_pBtBuyConfirm;        //!< ����
    GUIButton*                  m_pBtBuyCanel;          //!< ȡ��
    GUIButton*                  m_pBtBuyExit;           //!< �ر�

    TSFPTrunk<PetSkillAssistanceFrame>  m_Trunk;
    TObjRef<GUISystem>			        m_pGUI;
    TObjRef<Util>				        m_pUtil;
    TObjRef<GameFrameMgr>		        m_pFrameMgr;
    TObjRef<NetSession>			        m_pSession;

    DWORD                       m_dwUpdateTime;         //!< �Զ�ˢ��ʱ��
};

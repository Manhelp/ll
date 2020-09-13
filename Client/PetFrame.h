#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "PetSkillButton.h"
#include "StaticEx.h"
#include "../WorldDefine/msg_pet.h"
#include "PetEvent.h"
#include "../WorldDefine/time.h"

const INT MAX_PET_NUM           =   5;	//��ʼ�������˸��Ե������λ��
const INT MAX_PET_EQUIP         =   4;
const INT MAX_PET_SKILL         =   4;
const INT MAX_PET_RIDER_TOTAL_NUM  =   20; ////���ճ������˹��õ������λ��
const tstring SELECT_ICON_PATH  =   _T("data\\ui\\Common\\L_icon-l2.bmp");
const tstring SELECT_DIBAN      =   _T("data\\ui\\sys\\l_diban-a.bmp");
const tstring SELECT_LOCK       =   _T("data\\ui\\pet\\L_hui.bmp");
const tstring ZUOQI_PIC			=   _T("data\\ui\\pet\\zuoqi.bmp");
const tstring FUZHU_PIC			=   _T("data\\ui\\pet\\fuzhu.bmp");
const tstring GREY_PIC			=	_T("data\\ui\\pet\\l_xiaofangkuai.dds");
class PetShowWindow;
class PetBagFrame;

//��ǰ�����ҳö��
enum E_Click_Page
{
	Click_Pet_Page = 0,
	Click_Rider_Page =1,
};
//! \class  PetFrame
//! \brief  ������ҳ���������
//! \author hyu
//! \base   GameFrame
//! \sa     PetManager
class PetFrame :
    public GameFrame
{
public:
    PetFrame(void);
    ~PetFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
	virtual BOOL EscCancel();

    bool IsWindowVisible();
    VOID SetPageType(INT type)
	{
		m_eCurClickPage = (E_Click_Page)type;
	}
private:
    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ��/�رճ��������Ϸ�¼�
    DWORD OnOpen_Pet(tagGameEvent* pMsg);
	DWORD OnClose_Pet(tagGameEvent* pMsg);
    
    DWORD OnOpen_PetByID(tagOpenPetbyID* pMsg);
    
	//��Ӧ����������λ����仯
	DWORD OnMaxNumChanged();
    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

    //! ˢ�³����б�
    VOID RefreshPetList();
	//! ���³�������
	VOID UpdateData();
    //! ˢ�½�������
    VOID RefreshToUI();
    ////! ˢ�µ�ǰװ��
    //VOID RefreshEquip();
    //! ˢ�µ�ǰ����
    VOID RefreshSkill();
    //! ˢ�µ�ǰģ��
    VOID RefreshPetShow();

    //! ��õȼ�����
    tstring GetLevelText(int nLevel);
    //! ���Ʒ������
    LPCTSTR GetQualityText(int nQuality);

    //! ���ý���
    VOID ClearUI();
    //! ���Æ΂������
    VOID ClearUIData();
	//����տͻ�������
	VOID ClearPetInfo();
    //! ȷ��ɾ������
    DWORD OnCommitDeletePet(tagMsgBoxEvent* pEvent);
	DWORD OnCommitCallBackPet(tagMsgBoxEvent* pEvent);//ȷ�����ճ���
    //! �����������Ϣ�򷵻ص��¼�
    DWORD OnCommitPetName(tagMsgInputBoxEvent* pEvent);
    //! ������ķ���
    DWORD OnNS_PetRename(tagNS_PetRename* pMsg, DWORD);

    //! ȷ����ע����
    DWORD OnCommitPourExp(tagMsgBoxEvent* pEvent);

    //! ������ﴩװ���¼�
    DWORD OnUsePetEquipHandler(tagUsePetEquip* pEvent);

    //! �������Է����仯
    DWORD OnPetAttrChange(tagPetAttrChange* pEvent);

	DWORD OnEvent_Refresh_PetUI(tagGameEvent* pEvent);

	DWORD OnPetDispInfo(tagPetDispChange* pEvent );

	DWORD OnMsgBoxClickEvent(tagMsgBoxEvent* pGameEvent);
    //! ֧���������ͻ���ת��
    template<typename T>
    tstring TtoString(T nSrc);

	//��ID��ó������ʣ��ʱ���ִ����������Ƿ�����
	BOOL ID2RestTime(DWORD dwID,tstring *refTstr);
	//��ҳ�����ˢ��
	VOID PageChangeRefresh();

	E_Click_Page				  m_eCurClickPage;         //!< ��ǰѡ��ҳ������
	E_Click_Page                 m_bCurExtendMutex;		//!< ��ǰѡ���������
	//�����ҳ(pet)
    DWORD                       m_dwCurrSelectPetID;     //!< ��ǰѡ�еĳ���ID(����ѡ����ΪGT_INVALID)
    INT                         m_nSelectRow;           //!< ��ǰѡ�е���
    map<INT, DWORD>              m_mapRow2Pet;           //!< λ�úͳ���id���ձ�
    INT                         m_nCurrPage;            //!< ��ǰҳ
	BOOL						  m_bCurrPetDead;			//!< ���ڵĳ����Ƿ��Ѿ�����ʱ������
	INT							  m_iCurPetNum;				//!< ���ڵĳ�������		
	INT							  m_iCurPetGridNum;			//!< ��ǰ�ĳ���������
	//�����ҳ(pet1)
	DWORD                       m_dwCurrSelectPetID1;     //!< ��ǰѡ�еĳ���ID(����ѡ����ΪGT_INVALID)
	INT                         m_nSelectRow1;           //!< ��ǰѡ�е���
	map<INT, DWORD>              m_mapRow2Pet1;           //!< λ�úͳ���id���ձ�
	INT                         m_nCurrPage1;            //!< ��ǰҳ
	BOOL						  m_bCurrPetDead1;			 //!< ���ڵĳ����Ƿ��Ѿ�����ʱ������
	INT							  m_iCurRiderNum1;			 //!< ���ڵĳ�������		
	INT							  m_iCurRiderGridNum;		 //!< ��ǰ�ĳ���������

    TSFPTrunk<PetFrame>          m_Trunk;
    TObjRef<GUISystem>			  m_pGUI;
    TObjRef<Util>				  m_pUtil;
    TObjRef<NetCmdMgr>			  m_pCmdMgr;
    TObjRef<GameFrameMgr>		  m_pFrameMgr;
    TObjRef<NetSession>			  m_pSession;

    //GUI�ؼ�
    GUIWnd*						  m_pWnd;


	//�����ҳ
	GUIPatch*                   m_pPchPetPage;                 //�����ҳwnd
	GUIPushButton*				  m_pBtPetPage;					 //�����ҳ��ť
	//GUIStatic*                  m_pStcState;                    //!< ״̬
    //GUIListBox*                 m_pLbPetList;                   //!< �����б�
    GUIButton*                  m_pBtPrevPage;                  //!< ��һҳ
    GUIButton*                  m_pBtNextPage;                  //!< ��һҳ
    GUIStatic*                  m_pStcPage;                     //!< ��ʾҳ��
    //GUIButton*                  m_pBtPerfuse;                   //!< ��ע
    //ItemButton*                 m_pBtEquipA[MAX_PET_EQUIP];     //!< ����װ��
    PetSkillButton*             m_pBtSkillA[MAX_PET_SKILL];     //!< ���＼��
	PetShowWindow*              m_pStcPetShow;					  //!< ����Avata
	//ItemButton*                 m_pBtSkillB[MAX_PET_SKILL];     //!< ����װ��
    //GUIStatic*                  m_pStcCarryLevel;               //!< Я���ȼ�...
    //GUIStatic*                  m_pStcQuality;                  //!< ����Ʒ��
    //GUIStatic*                  m_pStcMatureTalent;             //!< �ɳ�����
    //GUIStatic*                  m_pStcPotential;                //!< Ǳ��
    //GUIStatic*                  m_pStcMagic;                    //!< ����֮��(ԭ������)
    //GUIStatic*                  m_pStcRidingSpeed;              //!< ����ٶ�
    GUIPatch*                   m_pPchBack[MAX_PET_NUM];        //!< ����ѡ��װ�...
    GUIButton*                  m_pBtBack[MAX_PET_NUM];        //!< ����ѡ��ť...
    StaticEx*                   m_pStcOption[MAX_PET_NUM];      //!< ����ѡ������װ�...
    StaticEx*                   m_pStcIcon[MAX_PET_NUM];        //!< ����ͼ��...
    GUIStatic*                  m_pStcNick[MAX_PET_NUM];        //!< �����ǳ�
    GUIStatic*                  m_pStcIsCalled[MAX_PET_NUM];    //!< �Ƿ��ٻ���...
    GUIButton*                  m_pBtCall[MAX_PET_NUM];        //!< �ٻ� 
	GUIStatic*                  m_pStcIsBinding[MAX_PET_NUM];   //!< �Ƿ��...
	GUIStatic*                  m_pStcTime[MAX_PET_NUM];		  //!< ��������
	//GUIStatic*                  m_pStcLevel[MAX_PET_NUM];       //!< ����
    //GUIStatic*                  m_pStcEnergy[MAX_PET_NUM];      //!< ����(ԭ����)
    //GUIProgress*                m_pProgressEnergy[MAX_PET_NUM]; //!< ����������
    //GUIStatic*                  m_pStcExp[MAX_PET_NUM];         //!< ����
    //GUIProgress*                m_pProgressExp[MAX_PET_NUM];    //!< ����������
	//ItemButton*				  m_pStcRideing[MAX_PET_NUM];		//!< ����ͼ��

	//�����ҳ
	GUIPatch*                   m_pPchRiderPage;                 //�����ҳwnd
	GUIPushButton*				  m_pBtRiderPage;					  //�����ҳ��ť
	GUIPatch*                   m_pPchBack1[MAX_PET_NUM];         //!< ����ѡ��װ�...
	GUIButton*                  m_pBtBack1[MAX_PET_NUM];          //!< ����ѡ��ť...
	StaticEx*                   m_pStcOption1[MAX_PET_NUM];       //!< ����ѡ������װ�...
	StaticEx*                   m_pStcIcon1[MAX_PET_NUM];         //!< ����ͼ��...
	GUIStatic*                  m_pStcNick1[MAX_PET_NUM];         //!< �����ǳ�
	GUIStatic*                  m_pStcIsPreparing1[MAX_PET_NUM];   //!< �Ƿ�Ԥ����Ԧ��...
	GUIButton*                  m_pBtRide1[MAX_PET_NUM];          //!< ���   
	GUIStatic*                  m_pStcIsBinding1[MAX_PET_NUM];     //!< �Ƿ��...
	GUIStatic*                  m_pStcTime1[MAX_PET_NUM];			 //!< ��������
	PetShowWindow*              m_pStcPetShow1; 
	GUIButton*                  m_pBtPrevPage1;                  //!< ��һҳ
	GUIButton*                  m_pBtNextPage1;                  //!< ��һҳ
	GUIStatic*                  m_pStcPage1;                     //!< ��ʾҳ��
	PetSkillButton*             m_pBtSkillA1[MAX_PET_SKILL];     //!< ���＼��

	GUIButton*                  m_pBtTrade;                     //!< ����
	GUIButton*                  m_pBtExtend;                     //!< ����
	GUIButton*                  m_pBtChangeName;                //!< ����...
	GUIButton*                  m_pBtSetFree;                   //!< ����
	GUIButton*                  m_pBtCallBack;                   //!< ����

    GUIButton*                  m_pBtClose;                     //!< �ر�

    PetBagFrame*                m_pPetBagFrame;                 //!< pet bag frame

    DWORD                       m_dwStartTime;                  //!< ���¿�ʼʱ��

};

template<typename T>
tstring PetFrame::TtoString( T nSrc )
{
    tstringstream sstream;
    sstream << nSrc;
    tstring ret;
    sstream >> ret;
    return ret;
}
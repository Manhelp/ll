#pragma once
#include "gameframe.h"
#include "..\WorldDefine\ItemDefine.h"

class StaticEx;
class Item;
class  ItemButton;
struct tagSpecItemEvent;
struct tagItemPosChangeExEvent;
struct tagEquipDropEvent;
struct tagRefreshMaintainValueUIEvent;
struct tagNS_ItemRemove;
struct tagNS_StarEnhance;
struct tagNS_EquipSingleChange;
struct tagNS_HolyEquipHole;				//Ԫ��װ�����
struct tagNS_HolyEquipInlay;				//Ԫ��װ����Ƕ
struct tagNS_HolyEquipRemove;		//Ԫ��װ��ժ��

enum EHolyEquipCtrlType
{
	EECT_Inlay			=0,			//��Ƕ
	EECT_Chisel			=1,			//���
	EECT_Compose	=2,			//����

	EECT_End				=3,			//������Ŀ
};

class HolyEquipFrame :
	public GameFrame
{
public:
	HolyEquipFrame(void);
	~HolyEquipFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update(VOID);

	//�򿪽��� 
	VOID ShowWnd(BOOL bVal);

private:
	//����
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnUseItemEvent(tagSpecItemEvent* pGameEvent);
	DWORD OnEquipPosEvent(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnItemDropCheck(tagEquipDropEvent* pGameEvent);
	DWORD OnChangeItemEvent(tagItemPosChangeExEvent* pGameEvent);
	//DWORD RevRefreshFrame( tagRefreshMaintainValueUIEvent* pGameEvent);
	DWORD NetItemRemove(tagNS_ItemRemove* pMsg, DWORD pPrama);
	DWORD OnNS_StarEnhance(tagNS_StarEnhance* pMsg, DWORD pPrama);

	DWORD OnNS_HolyEquipHole(tagNS_HolyEquipHole* pMsg, DWORD pPrama);
	DWORD OnNS_HolyEquipInlay(tagNS_HolyEquipInlay* pMsg, DWORD pPrama);
	DWORD OnNS_HolyEquipRemove(tagNS_HolyEquipRemove* pMsg, DWORD pPrama);
	
	BOOL	AddItem(INT16 nPos);
	VOID	RefreshUI();
	BOOL	ClearEquip();
	VOID	ClearSubData();
	VOID	LockItem(bool bOperable,	EHolyEquipCtrlType eType=EECT_End);
	BOOL	ClearUI(EHolyEquipCtrlType eType =EECT_End);
	VOID	SwitchPage(EHolyEquipCtrlType eType =EECT_End);
	tstring	GetPicFileByQuality(INT nQuality);
	tstring  GetHoleInfo( INT index, EHolyEquipCtrlType eType,DWORD &dwQuality );

	//����
	BOOL	CanHolyCompose(); //�ܹ� 
	VOID	KeepHolyCompose(); //���� 
	VOID	BeginHolyCompose(); //��ʼ 
	VOID	StopHolyCompose(); //�ж� 
	VOID	ChangeItemNum();
	VOID	SendHolyComposeMsg();
	VOID	RefreshComposeUI();

	//��Ƕ
	DWORD	IfPosCanInlay(INT nIndex);		//λ���Ƿ����Ƕ. ��λ�����ѿ�����δ��Ƕ, index ��0��ʼ,���ش�����
	BOOL	ChooseInlayPos(INT nIndex);		//ѡ����Ƕλ��
	BOOL	CanHolyInlay(); //�ܹ�
	VOID	BeginHolyInlay(); //��ʼ��Ƕ
	BOOL	CanHolyUninlay(); //�ܹ�ժ�� 
	VOID	BeginHolyUninlay(); //��ʼժ�� 
	VOID	StopHolyInlay(); //�ж� 
	VOID	RefreshInlayUI();
	DWORD OnMsgBox_UninlayConfirm(tagMsgBoxEvent* pGameEvent);  
	VOID	SendHolyInlayMsg();
	VOID	SendHolyUninlayMsg();

	//���
	BOOL	CanHolyChisel(); //�ܹ�
	VOID	BeginHolyChisel(); //��ʼ 
	VOID   KeepHolyChisel(); //���� 
	VOID	StopHolyChisel(); //�ж� 
	VOID	RefreshChiselUI();
	VOID	SendHolyChiselMsg();

private:
	TSFPTrunk<HolyEquipFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>								m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	//------Ԫ��װ��ǿ������
	GUIWnd*							m_pWndFather;
	GUIWnd*							m_pWnd;
	ItemButton*						m_pIBEquip;
	Item*								m_pEquip;
	GUIStatic*						m_pStcSucc;	//�ɹ�
	GUIStatic*						m_pStcFail;	//ʧ��
	EHolyEquipCtrlType		m_eCurCtrlType;
	BOOL								m_bIfProgress;
	DWORD							m_escapeTime;

	//���Ƿ�ҳ
	GUIPatch*						m_pPthCompose;
	ItemButton*						m_pIBComposeItem;	//��ҫʯ
	Item*								m_pItemCompose;
	GUIPushButton*				m_pBtnPageCompose[EECT_End];		//���Ƿ�ҳ3����ҳ��ť
	GUIStatic*						m_pStcStarLevel;
	GUIStatic*						m_pStcStarNum[MAX_STARENHANCE_LEVEL];
	GUIStatic*						m_pStcSuccRate;
	GUIStatic*						m_pStcReturnLv;
	GUIStatic*						m_pStcAddAttName;
	GUIStatic*						m_pStcAddAttValue;
	GUIStatic*						m_pStcCostGold;
	GUIStatic*						m_pStcCostSilver;
	GUIProgress*					m_pProHolyCompose;
	GUIButton*						m_pBtnStartComp;
	GUIButton*						m_pBtnCancelComp;
	GUIPushButton*				m_pPBItemNum;
	GUIPushButton*				m_pPBToLevel;
	GUIEditBox*					m_pEBItemNum;
	GUIEditBox*					m_pEBToLevel;
	INT									m_nItemCostNum;

	//��Ƕ��ҳ
	GUIPatch*						m_pPthInlay;
	ItemButton*						m_pIBInlayItem;  //��ʯ				 
	Item*								m_pItemInlayItem;
	ItemButton*						m_pIBInlayStuff;	//�Ͼ�			 
	Item*								m_pItemInlayStuff;
	GUIPushButton*				m_pBtnPageInlay[EECT_End];		//��Ƕ��ҳ3����ҳ��ť
	StaticEx*							m_pSteInlayHole[MAX_EQUIPHOLE_NUM];  
	GUIPushButton*				m_pBtnInlayHole[MAX_EQUIPHOLE_NUM];  
	GUIButton*						m_pBtnInlayMinStuff;
	GUIButton*						m_pBtnInlayAddStuff;
	GUIStatic*						m_pStcInlayRate;
	GUIStatic*						m_pStcInlayGold;
	GUIStatic*						m_pStcInlaySilver;
	GUIProgress*					m_pProInlayOnce;
	GUIButton*						m_pBtnInlayBegin;
	GUIButton*						m_pBtnInlayExtirpate;
	GUIButton*						m_pBtnInlayCancel;
	INT									m_nInlayStuffNum;
	INT									m_nInlayPos;			// 0 �� ���ֵ-1
	enum EHolyInlayType
	{
		EHIT_Null			= 0,			//�޲���
		EHIT_Inlay		= 1,			//��Ƕ
		EHIT_Uninlay	= 2,			//ժ��
	};
	EHolyInlayType				m_eHolyInlayType;

	//��׷�ҳ
	GUIPatch*						m_pPthChisel;
	ItemButton*						m_pIBChiselItem;//����	 
	Item*								m_pItemChisel;
	GUIPushButton*				m_pBtnPageChisel[EECT_End];		//��׷�ҳ3����ҳ��ť
	StaticEx*							m_pSteChiselHole[MAX_EQUIPHOLE_NUM];  
	GUIStatic*						m_pStcHaveHoles;
	GUIStatic*						m_pStcChiselRate;
	GUIStatic*						m_pStcChiselGold;
	GUIStatic*						m_pStcChiselSilver;
	GUIProgress*					m_pProChiselOnce;
	GUIButton*						m_pBtnChiselBegin;
	GUIButton*						m_pBtnChiselCancel;

};

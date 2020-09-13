#pragma once
#include "gameframe.h"
#include "..\WorldDefine\msg_compose.h"

class IconStatic;
class ItemButton;
class Item;
/*********************
//!brief:����װ����������
*********************/

#define  FOALT_ZERO  0.000001

struct tagNS_ItemRemove;
struct tagEquipDropEvent;

class SoarEquipFrame :
	public GameFrame
{
public:
	SoarEquipFrame(void);
	~SoarEquipFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	VOID  ShowMixFrame();
	VOID  ReceiveNPCID(DWORD dwNPCID);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD NetRecMixEquipMsg( tagNS_BuildEuipment *pNetCmd, DWORD );
	DWORD NetItemRemove(tagNS_ItemRemove *pNetCmd,DWORD);
	DWORD OnCloseSoarEquipFrame(tagGameEvent* pGameEvent);
	DWORD PocketEquipDrop( tagEquipDropEvent* pGameEvent);

	BOOL  EquipMixing();                        //����
	VOID  TestSoarEquip(INT nPos);     //��������װ���Ƿ�ϸ�
	VOID  TestMixEquip(INT nPos);    //�������ϳ�װ���ĵ����Ƿ�ϸ�
	VOID  TestMixStuff(INT nPos);    //�������ϳ�װ����װ���Ƿ�ϸ�
	VOID  MixPercentCompute();     //���������ٷֱȣ�����������ʾ����
	VOID  RefreshBtnUI();         //ˢ������3������װ����
	VOID  RefreshProUI();		 	//ˢ�½��������UI
	VOID  DragBtnInMixFrame(ItemButton*	 btn,Item* itm);       //��������dragBtn
	VOID  MixDataInit();                 //��ʼ���������棬����ȡ��������
	VOID  LockItem( bool bOperable );	// ������Ʒ
	VOID  UpdateItem(Item *pItem);		// ������Ʒ
	VOID  SendMixInfoToServer();      //�����������������Ϣ
	VOID  DestroyMsgBox();
	VOID  ShowItemTips(GUIWnd* pWnd, Item *pTempItem); 

private:

	TObjRef<GUISystem>			m_pGUI;
	TSFPTrunk<SoarEquipFrame>	m_trunk;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	
	GUIWnd*						m_pWnd;
	ItemButton*					m_pBtnSoarEquip;		//������װ����itembtn
	ItemButton*					m_pBtnMixEquip;
	ItemButton*					m_pBtnMixStuff;
	GUIButton*					m_pBtnConfirmMix;
	GUIButton*					m_pBtnCancel;
	GUIProgress*				m_pProMix;
	GUIPatch*					m_pPatPercentShow;
	GUIStatic*					m_pStcSoarEquip;
	GUIStatic*					m_pStcMixEquip;
	GUIStatic*					m_pStcMixStuff;
	IconStatic*					m_pStcMixResaultShow;
	Item*						m_pItemSoarEquip;		//������װ����item
	Item* 						m_pItemMixEquip;	
	Item*						m_pItemMixStuff;	
	
	DWORD						m_escapeTime;		//��ʱ�������ж��Ƿ���NPC��Զ
	BOOL						m_bIfMix;			//����ͨ���������
	FLOAT                     m_fSoarPercent;    //�����ٷֱ�
	INT64						m_iSoarEquipType;	
	INT							m_iSoarEquipLevel; //������װ���ȼ�
	INT							m_iSoarEquipQuality;//������װ��Ʒ��
	INT64						m_iEquipType;
	INT                       m_iEquipLevel;    //�����ϳɵ��ߵ�װ���ȼ�
	INT                       m_iEquipQuality;  //�����ϳɵ���װ��Ʒ��

	BOOL						m_bIfProgressing;  //�Ƿ��ڶ�������
	BOOL						m_bSpeedNormal;   //�ж϶����ٶ��Ƿ�Ϊ0.25
	DWORD						m_dwNPCID;

	//tagSaveEquipAndStuffData   m_sSaveInfo;
};

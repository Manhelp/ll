#pragma once
#include "..\WorldDefine\msg_frame.h"
#include "TrainFrame.h"

#define MAX_DailyExp 2000
/** \class SysMenuFrame
    \brief ���½�ϵͳ�˵����棨������չ�˵���
*/
struct tagNS_QuerySoaringSwitch;
class FlashPushButton;
class ActiveProgress;
struct tagUpdateLPAttEvent;
struct tagFamilyFrameEvent;

class SysMenuFrame : public GameFrame
{
public:
	SysMenuFrame();
	~SysMenuFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual void Update();
	enum EMenuExItem
	{
		//EMI_Help,
		EMI_Sociality,
		EMI_Faction,
		EMI_Pet,
		EMI_ZoneMap,
		EMI_WorldMap,
		EMI_Encrypt,
		EMI_SysOptionMenu,
		EMI_Num,
		EMI_Help,
	};
private:
	/** \��Ϸ�¼���Ӧ����
	*/
	DWORD OnClose_Role_UIEvent(tagGameEvent* );
	DWORD OnClose_Package_UIEvent(tagGameEvent* );
	DWORD OnClose_Dower_UIEvent(tagGameEvent* );
	DWORD OnOpen_Task_UIEvent(tagGameEvent* );
	DWORD OnClose_Task_UIEvent(tagGameEvent* );
	DWORD OnClose_Mall_UIEvent(tagGameEvent* );
	DWORD OnOpen_Mall_UIEvent(tagGameEvent* );
	DWORD OnOpen_Role_UIEvent(tagGameEvent*);
	DWORD OnOpen_Dower_UIEvent(tagGameEvent*);
	DWORD OnOpen_Guild_UIEvent(tagGameEvent*);
	DWORD OnClose_Guild_UIEvent(tagGameEvent*);
	DWORD OnClose_Pet_UIEvent(tagGameEvent*);
	DWORD OnClose_Sociality_UIEvent(tagGameEvent*);
	DWORD OnClose_Train_UIEvent(tagGameEvent* );
	DWORD OnClose_Soar_UIEvent(tagGameEvent*);
	DWORD OnForceOpen_Package_UIEvent(tagGameEvent*);
	DWORD OnOpen_Area_UIEvent(tagGameEvent* );
	DWORD OnClose_Area_UIEvent(tagGameEvent* );
	DWORD OnOpen_Pulse_UIEvent(tagGameEvent* );
	DWORD OnClose_Pulse_UIEvent(tagGameEvent* );

	DWORD OnGameEventLevelUp(tagGameEvent*);
	DWORD TryOpenSoarQuest(tagMsgBoxEvent* pGameEvent);

	DWORD FlashTrainButton(tagGameEvent*);

	DWORD OnOpen_Holy(tagGameEvent* pEvent);

	/** \�ȼ���Ӧ����
	*/
	DWORD OnOpenClose_ShopHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_RoleHotKey( DWORD dwID, BOOL bDown );
    DWORD OnOpenClose_RoleInfoKey( DWORD dwID, BOOL bDown );
    DWORD OnOpenClose_RoleEquipKey( DWORD dwID, BOOL bDown );
    DWORD OnOpenClose_RoleTitleKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_PackageHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_DowerHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_TaskHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_ZoneMapHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_WorldMapHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_SocialityHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_HelpHotKey(DWORD dwID, BOOL bDown);
    DWORD OnOpenClose_PetHotKey(DWORD dwID, BOOL bDown);
	DWORD OnOpenClose_Guild(DWORD dwID, BOOL bDown);
	DWORD OnOpenClose_TrainHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_AreaHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_SoarHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_FamilyHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_RoleHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_PackageHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_DowerHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_TaskHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_GuildChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_PetHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_SocialityHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_HelpHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_ZoneMapHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_WorldMapHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_ShopHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_TrainHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_SoarHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_AreaHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_FamilyHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);


	// �򿪸�����
	DWORD OnNS_OpenFramePage( tagNS_OpenFramePage *pNetCmd, DWORD );
	DWORD OnNS_QuerySoaringSwitch(tagNS_QuerySoaringSwitch *pNetCmd, DWORD );

	// ��ÿ����������
	DWORD OnOpenDialyExpFrame(tagGameEvent* pEvent);					//�����ť��ʾ����
	DWORD OnOpenDialyExpFrameByTime(tagUpdateLPAttEvent* pEvent);		//ʮ����Զ��رյ�
	DWORD OnFamilyEvent_Frame(tagFamilyFrameEvent* pEvent);

private:
	TSFPTrunk<SysMenuFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameInputMap>		m_pKeyMap;
	TObjRef<Util>				m_pUtil;
	TObjRef<GUIRender>			m_pRender;

	GUIWnd*                     m_pWnd;                   //ϵͳ�˵�����
	GUIWnd*						m_pPanel;                 //�װ�
	GUIPushButton*				m_pBn_Role;               //��ɫ��ť
	GUIPushButton*				m_pBn_Package;            //���Ұ�ť
	GUIPushButton*				m_pBn_Dower;              //���ʰ�ť
	GUIPushButton*				m_pBn_Task;               //����ť
	GUIPushButton*				m_pBn_Mall;               //�̳ǰ�ť
	GUIPushButton*				m_pBn_Guild;			  //���ɰ�ť
    GUIPushButton*              m_pBn_Pet;                //���ް�ť
	GUIPushButton*              m_pBn_Sociality;          //�˼ʰ�ť
	GUIButton*		            m_pBn_Expand;             //�˵���ť
	FlashPushButton*			m_pBn_Train;			  //������ť
	GUIPushButton*				m_pBn_PvP;				  //��������ť
	GUIPushButton*              m_pBn_Soar;				  //������ť
	GUIButton*					m_pBn_Hide;				  //���ذ�ť
	GUIPushButton*				m_pBn_God;               //����ť
	GUIPushButton*				m_pBn_Family;            //���尴ť
	GUIPushButton*				m_pBn_Pulse;            //Ѫ����ť
	
	// TIPS�������������ȼ�
	tstring						m_strTip_Role;               //��ɫ
	tstring						m_strTip_Package;            //����
	tstring						m_strTip_Dower;              //����
	tstring						m_strTip_Task;               //����
	tstring						m_strTip_Mall;               //�̳�
	tstring						m_strTip_Guild;				 //����
	tstring						m_strTip_Pet;                //����
	tstring						m_strTip_Sociality;          //�˼�
	tstring						m_strTip_Train;				 //����
	tstring						m_strTip_Soar;				 //����
	tstring						m_strTip_Hide;				 //����
	tstring						m_strTip_God;				 //����
	tstring						m_strTip_Family;			//����
	tstring						m_strTip_Pulse;			//Ѫ��

	bool						m_bSoarOpen;

	GUIWnd*						m_pHideWnd;
	GUIPushButton*				m_pBtnMall;
	GUIButton*					m_pBtnHide;

	GUIWnd*						m_pWndGodDailyExp;
	ActiveProgress*				m_pProcess;
	GUIStatic*					m_pStcText;
	INT							m_nShowTime;	//ֵΪGT_INVALID��ʾ���Զ��ر�
	DWORD                       m_escapeTime;

	BOOL GUIEventHandlerHideWnd(tagGUIEvent* pEvent);

	BOOL GUIEventHandlerMenu(tagGUIEvent* pEvent);        //ϵͳ�˵��¼���Ӧ�ص�����

	GUIWnd*                     m_pWndMenuEx;             //ϵͳ��չ�˵�����
	GUIListBox*                 m_pListMenuEx;            //ϵͳ��չ�˵���Ŀ
	tstring                     m_strMenuExItemText[EMI_Num];

	BOOL GUIEventHandlerMenuEx(tagGUIEvent* pEvent);      //ϵͳ��չ�˵��¼���Ӧ�ص�����
	VOID SafeDestroyUI();
	VOID UpdataMenuSize();
};


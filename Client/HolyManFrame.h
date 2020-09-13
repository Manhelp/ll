#pragma once
#include "gameframe.h"
#include "..\WorldDefine\container_define.h"

//#define  HolyMan_Test_Define

class Item;
class ItemButton;
class HolyEquipFrame;
struct tagUpdateHolyManEvent;
struct tagNS_GetHolyAffuse ;
struct tagNS_GetHolyReborn;
struct tagUpdateLPSkillEvent;
struct tagSpecItemEvent;
struct tagClickItemEvent;
struct tagEquipDropEvent;
struct tagItemPosChangeExEvent;
struct tagRefreshMaintainValueUIEvent;
struct tagNS_ItemRemove;

//Ԫ��Ԥ����
class HolyManView 
	:public GUIStatic
{
public:
	HolyManView(void);
	virtual ~HolyManView(void);

	virtual BOOL Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement );
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();
	virtual VOID RefreshViewCoord();
	virtual VOID SetInvisible( BOOL b );
	virtual VOID AddChild(GUIWnd* pChild);

	void Render3D();
	void SetHolyManShow( );
	void UpdateCamera();
	void RotateYaw( const FLOAT fAngle );
private:
	tstring GetTransformSkinPath(const TCHAR* pModelName, int index );
	EntityNode* LoadEntity( LPCTSTR szFileName );
	void LoadSkin( LPCTSTR szFileName, EntityNode* pEntityNode );

	DWORD TestOnKey( DWORD dwID, BOOL bDown );

private:
	tagGUIImage*			m_pNewCanvas;			// RenderTarget
	EntityNode*				m_pHolyMenNode;				// SceneNode
	NullSceneGraph*		m_pSG;							// ���ڱ�������������յ���Ϣ,��������Ⱦ
	CameraEuler				m_Camera;

	GUIButton*              m_pBnRotaLeft;
	GUIButton*              m_pBnRotaRight;

	float							m_fYaw;

	TSFPTrunk<HolyManView>	m_Trunk;
	Vector3									m_LookAt;
	Vector3									m_LookFrom;
#ifdef HolyMan_Test_Define
	TObjRef<InputDX8>				m_pInput;
	TObjRef<GameInputMap>	m_pTestKeyMap; 
#endif

};  //end of��HolyManView��

//Ԫ����Ϣ����
class HolyManFrame :
	public GameFrame
{
public:
	enum //��Ϊ������ö��ֵ 
	{ 
		STATE_VALUE_RATE			= 4,		//Ԫ��ֵ���� 
		STATE_VALUE_COMPARE	= 25,	 //1�������Ԫ��ֵ

	};

public:
	HolyManFrame(void);
	~HolyManFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	virtual VOID Render3D();
	
	VOID ShowWnd(BOOL bShow);
	BOOL IsShowing();

	tstring GetGUIFullName()
	{
		if(P_VALID(m_pWndCaption))
			return m_pWndCaption->GetFullName();
		else 
			return _T("");
	};

private:

	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD StateEventHandler(tagGUIEvent* pEvent);
	/*
	Int �ͱ�Ϊ string��
	��nNum2��ΪGT_INVALID����ת��Ϊ�ַ���"nNum1 / nNum2"   
	*/
	LPCTSTR Int2String(tstring& strOut,INT nNum1, INT nNum2 =GT_INVALID);
	
	BOOL  AddItem(INT16 nPos);
	VOID  LockItem(bool bOperable);

	BOOL	ClearUI_Att();		//ʧ�ܷ���TRUE
	BOOL	ClearUI_Lv();			//ʧ�ܷ���TRUE
	BOOL	ClearUI_State();		//ʧ�ܷ���TRUE

	VOID	RefreshUI_Att();
	VOID	RefreshUI_Lv();
	VOID	RefreshUI_State();
	VOID    RefreshUI_Reborn();

	VOID	Update2DHolyValue();	//�����е�Ԫ��ֵ
	VOID	Update3DHolyValue();	//�����е�Ԫ��ֵ
	VOID	UpdateCovalue();

	VOID	SendCallHolyManMsg();
	VOID	SendHolyAffuseMsg();
	VOID	SendHolyRebornMsg();
	BOOL	CanReborn();

	DWORD OnUpdateHolyManEvent(tagUpdateHolyManEvent* pEvent);
	
	//--������Ʒ��λ�¼�
	DWORD OnChangeItemEvent(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnActiveHolySkillEvent(tagUpdateLPSkillEvent* pEvent);

	DWORD OnNS_GetHolyAffuse( tagNS_GetHolyAffuse *pNetCmd, DWORD );
	DWORD OnNS_GetHolyReborn( tagNS_GetHolyReborn *pNetCmd, DWORD );

	DWORD OnKey( DWORD dwID, BOOL bDown );

	DWORD OnUseSpecItemEvent(tagSpecItemEvent* pGameEvent);
	DWORD ClickItemEvent(tagClickItemEvent* pGameEvent);

	DWORD OnItemDropCheck(tagEquipDropEvent* pGameEvent);
	//DWORD RevRefreshFrame( tagRefreshMaintainValueUIEvent* pGameEvent);
	DWORD NetItemRemove(tagNS_ItemRemove* pMsg, DWORD pPrama);

private:
	TSFPTrunk<HolyManFrame>				m_Trunk;
	TObjRef<GUISystem>							m_pGUI;
	TObjRef<Util>										m_pUtil;
	TObjRef<NetCmdMgr>							m_pCmdMgr;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>							m_pSession;
	TObjRef<GameInputMap>					m_pKeyMap;
	
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIButton*					m_pBtnClose;

	GUIPatch*						m_pPthAtt;
	GUIPatch*						m_pPthLv;
	GUIPushButton*			m_pPBAtt;
	GUIPushButton*			m_pPBLv;

	//------Ԫ�����Խ���
	HolyManView*				m_pHolyManView;
	GUIButton*					m_pBtnLeft;
	GUIButton*					m_pBtnRight;

	GUIStatic*						m_pStcHolyLv_1;
	GUIStatic*						m_pStcHolyLv_2;
	GUIStatic*						m_pStcHolyLv_3;
	GUIStatic*						m_pStcHolyLvData;

	GUIProgress*				m_pProHolyExp;
	GUIProgress*				m_pProHolyValue;
	GUIProgress*				m_pProHolyPrivity;

	GUIStatic*						m_pStcDamage;
	GUIStatic*						m_pStcHitRate;
	GUIStatic*						m_pStcDeath;
	GUIStatic*						m_pStcDeathValue;
	GUIStatic*						m_pStcOutAttack;
	GUIStatic*						m_pStcInAttack;
	GUIStatic*						m_pStcExact;
	GUIStatic*						m_pStcExtraDamage;
	GUIStatic*						m_pStcIgnoreTough;
	GUIStatic*						m_pStcMorale;

	ItemButton*					m_pIBHolyEquip[EHEFI_End];

	//------Ԫ����������
	GUIStatic*						m_pStcHolyLv1_1;
	GUIStatic*						m_pStcHolyLv1_2;
	GUIStatic*						m_pStcHolyLv1_3;
	GUIStatic*						m_pStcHolyLvData1;

	GUIProgress*				m_pProHolyExp1;
	GUIProgress*				m_pProHolyValue1;
	GUIProgress*				m_pProHolyPrivity1;

	GUIEditBox*					m_pEBAffuseExp;
	GUIButton*					m_pBtnAffuseMax;
	GUIButton*					m_pBtnAffuseOK;
	GUIButton*					m_pBtnAffuseCancel;

	ItemButton*					m_pIBRebornItem;	//��Ԫ����
	ItemButton*					m_pIBRebornStuff;   //�Ͼ���¶ change xml
	GUIButton*					m_pBtnRebornOK;
	GUIButton*					m_pBtnRebCancel;
	GUIButton*					m_pBtnAddStuff;		//�� �Ͼ���¶
	GUIButton*					m_pBtnMinuStuff;		//�� �Ͼ���¶
	GUIProgress*				m_pProReborn;
	GUIStatic*						m_PStcRebornRate;
	GUIStatic*						m_pStcSucceed;				//�ɹ�

	Item*							m_pRebornItem;
	Item*							m_pRebornStuff;
	INT								m_nRebornStuffNum;
	//Ԫ��ֵ������
	ResTexture*						m_pResHolyMenValue;
	
	//------Ԫ��״̬����
	GUIWnd*							m_pWndHolyState;
	GUIStatic*							m_pStcStateRate[STATE_VALUE_RATE];
	GUIStatic*							m_pStcNormalState;
	GUIButton*						m_pBtnHolyState;

	//------Ԫ��װ��
	GUIPushButton*				m_pPBEquip;
	HolyEquipFrame*				m_pHolyEquip;

	DWORD							m_dwCurTime;						
	DWORD							m_dwTimeEclipse;
	DWORD							m_dwCurTimeState;				

	INT64								m_n64AffuseHolyExp;

	BOOL								m_bIfProgress;

};

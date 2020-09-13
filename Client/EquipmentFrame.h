#pragma once

#include "..\WorldDefine\container_define.h"
class PlayerViewWindow;
class ItemButton;
class NetSession;
struct tagUpdateLPItemEvent;
struct tagItemPosChangeExEvent;
struct tagUpdataAvatarEvent;
struct tagFashionEvent;
struct tagChangeFashionEvent;
struct tagUpdataAvatarDisplaySetEvent;
struct tagRehanding;
class BlinkStatic;

/** \class EquipmentFrame
	\brief װ���������
*/
class EquipmentFrame :
	public GameFrame
{
public:
	EquipmentFrame(void);
	~EquipmentFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID Render3D();
	//--Avatarˢ��
	VOID UpdateEquipAvatar();

private:
	//--����GUI�¼�
	DWORD EventHandler(tagGUIEvent* pEvent);
	
	//--������Ʒˢ���¼�
	DWORD OnUpdateItemEvent(tagUpdateLPItemEvent* pGameEvent);
	//--������Ʒ��λ�¼�
	DWORD OnChangeItemEvent(tagItemPosChangeExEvent* pGameEvent);
	//--����Avatar�仯�¼�
	DWORD OnUpdateAvatarEvent(tagUpdataAvatarEvent* pGameEvent);
	/*! ����װ��/ʱװģʽ�ı��¼� */
	DWORD OnChangDispPattern(tagFashionEvent *pGameEvent);
	DWORD OnChangeFashionPattern(tagChangeFashionEvent *pGameEvent);
           
	DWORD OnKeySwapWeapon(DWORD dwID, BOOL bDown);
	
	//--װ��ˢ��
	VOID UpdateEquipment(EEquipPos ePos);
	/*! \brief ˢ����Ʒ��ť
		\param bFashion �Ƿ��װģʽ
	*/
	VOID UpdateAll();
	//--��ȡװ����λ
	EEquipPos GetEquipPos(INT nIndex);
	//--�Ƿ���ƶ�
	BOOL IsEquipPosFix(EEquipPos eEquipPos, EEquipPos &eDstPos);
	
	//--��װ��
	VOID OnEquipment(INT16 nItemPos, EEquipPos ePos = EEP_End);
	//--װ���л�
	VOID OnEquipSwap(EEquipPos ePosSrc, EEquipPos ePosDst, BOOL bHotKey = false );
	//--��ʾ���øı�
	DWORD OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent);

	// �����³��е�ʱװ
	DWORD OntagRehanding(tagRehanding* pGameEvent);

    // ��ʱװģʽ�²���Ҫ��װ����
    void TurnFashionPos(bool on);

	void UpdataBlick();
private:
	TSFPTrunk<EquipmentFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		    m_pSession;
	TObjRef<GameInputMap>		m_pKeyMap;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	GUIStatic*					m_pStcEquipBack[EEP_End];
	ItemButton*					m_pBtnEquip[EEP_End];
	BlinkStatic*					m_pBlkEquip[EEP_End];

	BlinkStatic*					m_pBlkEquipFb;

	PlayerViewWindow*			m_pWndEquipView;
	GUIStatic*					m_pStcFashion;

	BOOL						m_bIsFashionDisplay;	/*!< �Ƿ���ʱװģʽ */
	DWORD					m_dwTimer;

};

#pragma once
#include "..\WorldDefine\msg_digbao.h"

class IconStatic;
class ItemButton;
class TreasureChestFrame : 
	public GameFrame
{	
public:
	TreasureChestFrame( void );
	virtual ~TreasureChestFrame( void );
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	bool CanUseAnother(){return  m_eOperateState == EOperateState_NULL ;}

public:
	static const int					sci_ItemNum = 16;	

private:
	BOOL EventHandler(tagGUIEvent* pEvent);

	// ��ʼ���������
	void InitTreasureChestUI();
	void RefreshUI();
	void SetTreasureChestUI();

	//--��Ϸ�¼�
	DWORD OnEOSAaginEvent(tagMsgBoxEvent* pGameEvent);

	//--������Ϣ
	DWORD OnNS_StartTreasureChest(tagNS_StartTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_TreasureChest(tagNS_TreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_StopTreasureChest(tagNS_StopTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_AgainTreasureChest(tagNS_AgainTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_ChestGetItem(tagNS_ChestGetItem* pMsg, DWORD dwParam);

	//--����״̬
	enum EOperateState
	{
		EOperateState_NULL		= 0,
		EOperateState_Begin,
		EOperateState_Stop,
		EOperateState_Again,
	};
	void SetOperateState( EOperateState eState );
	void SetCaption(DWORD dwChestTypeID);
	void SendStopTreasureMsg();
	void SendGetTreasureMsg();

private:
	
	TSFPTrunk<TreasureChestFrame>		m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<GameFrameMgr>				m_pFrameMgr;

	GUIWnd*								m_pWnd;
	GUIStatic*							m_pPatWaption;
	GUIButton*							m_pBtnMFunction;
	//GUIButton*							m_pBtnAgain;
	GUIButton*							m_pBtnGetItem;
	IconStatic*							m_pStcAuto[sci_ItemNum];
	ItemButton*							m_pStcItem[sci_ItemNum];
	IconStatic*							m_pLastAuto;
	GUIStatic*							m_pStcFinalBack;
	ItemButton*							m_pStcFinalItem;

	EOperateState						m_eOperateState;				//����״̬
	DWORD								m_dwRunTime;
	DWORD								m_dwStartTime;					//��ʼʱ��(1���Ӻ�ֹͣ)
	DWORD								m_dwStopTime;					//ֹͣ��ʱ��
	
	DWORD								m_dwItemTypeID;					//��õ���Ʒ
	INT									m_nItemCount;					//�����Ʒ����
	INT									m_nCurPos;						// ��ǰѡ�е���Ʒλ��
	INT									m_nFinalPos;					// ����Ӧ��ѡ�е���Ʒλ��
	BOOL								m_bCircleAgain;					// �Ƿ���Ҫ��תһȦ

	struct tagTreasure
	{
		DWORD dwTypeId;
		INT nNum;
		BYTE byQuality;
		tagTreasure()
		{
			dwTypeId = GT_INVALID;
			nNum = 0;
			byQuality = 0;
		}
	};
	//INT									m_dwStopCount;					//ֹͣ����
	vector<tagTreasure>					m_vecItem;						//���������Ʒ
	INT64								m_dwChestID;					// ����ID
	
};
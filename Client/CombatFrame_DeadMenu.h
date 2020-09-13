#pragma once
#include "gameframe.h"
#include "..\WorldDefine\msg_combat.h"

/** \class CombatFrame_DeadMenu
	\brief �����˵�
*/
class IconStatic;
class MallButton;
struct tagUpdateLPItemEvent;

class CombatFrame_DeadMenu :
	public GameFrame
{
public:
	CombatFrame_DeadMenu(void);
	virtual ~CombatFrame_DeadMenu(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();

	VOID  ShowTool();

	DWORD NetRecvNS_RoleRevive(tagNS_RoleRevive* pMsg, DWORD dwParam);
	DWORD NetRecvNS_RoleReviveNotify(tagNS_RoleReviveNotify* pMsg, DWORD dwParam);
	DWORD NetRecvCityStruggleRevive(tagNS_CityStruggleRevive *pMsg, DWORD dwParam);
private:
	BOOL  EventHandler(tagGUIEvent* pEvent);
	DWORD OnPvPDeadMenuConfirmMsgBox(tagMsgBoxEvent* pEvent);
	DWORD RefreshDeadMenu( tagGameEvent* pGameEvent);
	DWORD OnUpdateLPItem(tagUpdateLPItemEvent* pGameEvent);
private:
	TSFPTrunk<CombatFrame_DeadMenu>			m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*						m_pWnd;				//������
	GUIButton*					m_pBtnLocalRise;	//ԭ�ظ���
	GUIButton*					m_pBtnAcceptRise;	//���ܸ���
	GUIButton*					m_pBtnBackRise;		//�سǸ���
	GUIStatic*					m_pStcHint;			//��ʾ
	GUIButton*                  m_pBtnPerfectRise;  //��������

	//�������
	GUIStatic*					m_pStcRorReborn;
	IconStatic*					m_pRebornBack;		//item����
	MallButton*					m_pReborn;			//item��ť
	GUIStatic*					m_pStcMoney;		//Ԫ������
	GUIButton*					m_pBtnRebornBuy;	//����ť
	GUIStatic*					m_pStcRebornRemain; //ʣ�����
	//����wawa
	GUIStatic*					m_pStcRorScapegoat;
	IconStatic*					m_pScapegoatBack;		//item����
	MallButton*					m_pScapegoat;			//item��ť
	GUIStatic*					m_pStcScapegoatMoney;	//Ԫ������
	GUIButton*					m_pBtnScapegoatBuy;	    //����ť
	GUIStatic*					m_pStcScapegoatRemain;  //ʣ�����

	bool						m_bUpdateTime;		// �Ƿ���Ҫˢ�¸���ʱ��
	int							m_nReviveTime;		// ��������ʱ��
	BOOL						m_bRefresh;

	int						    m_nReviewWite;		// ����ȴ�ʱ��
	bool						m_bIsWar;			// �Ƿ����ػ���ͼ��~
};

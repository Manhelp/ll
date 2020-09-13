#pragma once
#include "ComposeStatic.h"
#include "ItemEvent.h"
#include "..\WorldDefine\msg_compose.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "Item.h"
#include "ActiveProgress.h"

#define MAX_GROWATT_NUM 5
struct tagItemPosChangeExEvent;

class EquipGrowFrame : public GameFrame
{
private:
	enum EDeComposeMsg
	{
		EDeCompose_NULL = 0,
		EDeCompose_SendMsg,		
		EDeCompose_Waiting,
		EDeCompose_RecvMsg,
	};
public:
	EquipGrowFrame(void);
	~EquipGrowFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	//�������
	VOID ClearUI();
	//����̳гɹ�������
	VOID ClearTransUI();
	//�϶��ͷ�
	VOID HandleDragStuff(tagGUIEvent* pEvent);
	//��ʾ
	VOID Show();
	//����
	VOID Hide();
	//������������������Ƭ
	//VOID FindQiSoulFromBag();
	//�����������ҳɳ����˷�
	VOID FindGrowLuckSign();
	//�����������Ҽ̳з�
	VOID FindInheritSign();
	//����map��������ȡ��Ʒ��TypeID
	DWORD GetItemTypeIDByMap(map<DWORD, INT64>& DestMap, INT nIndex);
	INT64 GetItem64IDByMap(map<DWORD, INT64>& DestMap, INT nIndex);
	//ˢ����Ϣ
	VOID RefreshInfo();
	//��ʾ������
	VOID ShowErrorCode(DWORD dwErrorCode);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventProduceAddItem(tagItemPosChangeExEvent* pEvent);
	DWORD OnOpen_EquipGrow(tagGameEvent* pEvent);
	DWORD OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pGameEvent);

	DWORD OnNS_EquipGrow(tagNS_EquipGrow* pCmd, DWORD);
	DWORD OnNS_EquipGrowTrans(tagNS_EquipGrowTrans* pCmd, DWORD);
	DWORD OnNS_EquipGrowDec(tagNS_EquipGrowDec* pCmd, DWORD);

private:
	TSFPTrunk<EquipGrowFrame>				m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*									m_pWndMain;
	GUIPushButton*							m_pPBtnEquipGrow;		//װ���ɳ�PushButton
	GUIPushButton*							m_pPBtnEquipInherit;	//װ���̳�PushButton
	GUIButton*								m_pBtnClose;
	//--------------------װ���ɳ�-----------------------
	GUIWnd*									m_pWndEquipGrow;		//װ���ɳ����
	ComposeStatic*							m_pCStcGrowEquip;		//װ���ɳ�װ��ͼ��
	//IconStatic*								m_pIStcQiSoulIcon;		//������Ƭͼ��
	//GUIListBox*								m_pLBoxQiSoul;			//�����б�
	//GUIEditBox*								m_pEBoxQiSoul;	
	IconStatic*								m_pIStcLuckSign;		//���˷�ͼ��		
	GUIListBox*								m_pLBoxLuckSign;		//���˷��б�
	GUIEditBox*								m_pEBoxLuckSign;			
	GUIStatic*								m_pStcEquipAtt[MAX_GROWATT_NUM];
	GUIEditBox*								m_pEBoxGrowNum;
	IconStatic*								m_pIStcGrowStar[EQUIP_GROW_MAX_LEVEL-1];
	ActiveProgress*							m_pProGroExp;
	GUIStatic*								m_pStcGrowExp;
	GUIButton*								m_pBtnBeginGrow;		//��ʼ�ɳ�
	GUIButton*								m_pBtnStarReduice;		//���ǰ�ť
	GUIStatic*								m_pStcGrowTips;			//��ʾ����ٵ�����
	//--------------------װ���̳�-----------------------
	GUIWnd*									m_pWndEquipInherit;		//װ���̳����
	ComposeStatic*							m_pCStcOriginalEquip;	//ԭʼװ�������ɳ����Ե�װ����
	ComposeStatic*							m_pCStcDestEquip;		//Ŀ��װ����Ҫ���ӳɳ����Ե�װ����
	IconStatic*								m_pIStcGrowInheritSign;	//�ɳ��̳з�ͼ��
	GUIListBox*								m_pLBoxGrowInheritSign;	//�ɳ��̳з��б�
	GUIEditBox*								m_pEBoxGrowInheritSign;
	GUIStatic*								m_pEBoxGrowInheritAtt[MAX_GROWATT_NUM];
	IconStatic*								m_pIStcInheritStar[EQUIP_GROW_MAX_LEVEL-1];
	GUIStatic*								m_pStcInheritExp;
	GUIButton*								m_pBtnBeginInherit;		//��ʼ�̳�

	std::list<Item*>						m_listQiHunItem;
	std::list<Item*>::iterator				m_QlistiHunIt;
	std::map<DWORD, INT64>					m_mapQiHunItem;			//������Ƭmap
	std::map<DWORD, INT64>					m_mapLuckSignItem;		//���˷�map
	std::map<DWORD, INT64>					m_mapInheritItem;		//װ���̳е���
	std::map<DWORD, INT64>::iterator		m_Tempit;

	INT										m_nListQiHunIndex;		//����ѡ�еڼ���
	INT										m_nListLuckSignIndex;	//���˷�ѡ�еڼ���
	INT										m_nListInheritIndex;	//�̳з�ѡ�еڼ���

	INT64									m_n64GrowEquipID;		//�ɳ�װ��ID
	INT64									m_n64InheritEquipID;	//�̳�װ��ID�����ɳ����Ե��Ǽ�
	INT64									m_n64DestEquipID;		//�̳�װ��ID�������Ǹ�..

	//DWORD									m_dwEquipGrowItemID;	//������ƬTypeID	
	DWORD									m_dwGrowValue;			//�ɳ�ֵ
	DWORD									m_dwEquipGrowLuckSignID;//���˷�ID
	INT64									m_n64EquipInheritID;	//װ���̳е���ID

	DWORD									m_updateTimeDistance;
};
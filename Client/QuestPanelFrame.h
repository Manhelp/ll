/********************************************************************

** �ļ���: QuestPanelFrame.h

** Copyright (c) �Ϻ�������缼�����޹�˾�����ֹ�˾ All Right Reserved

** ������: yfw

** ��  ��: 06/03/2010

** �޸���: yfw

** ��  ��: 06/05/2010

** ��  ��: ����棬�������

*********************************************************************/
#pragma once
#include "QuestMgr.h"

class IconStatic;
class ListBoxEx;
struct tagNS_OpenQuestBoard;

class QuestPanelFrame : public GameFrame
{
public:
	QuestPanelFrame(void);
	~QuestPanelFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	BOOL EscCancel();
	VOID SetNPCID(DWORD id, EFunctionNPCType eType, DWORD dwNpcTypeID = 0 ) { m_dwNpcID = id; m_eNPCType = eType; m_dwNpcTypeID = dwNpcTypeID; }

public:
	VOID UpdateQuestList();
	UINT16 GetQuestStartID();

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	// ����������Ϣ
	VOID UpdateQuestsDetail( UINT id );
	VOID UpdateQuestReward( UINT16 id );

private:
	TSFPTrunk<QuestPanelFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;

	GUIWnd *m_pWnd;										// ������
	GUIPatch *m_pCaption;								
	ListBoxEx *m_pLBQuestPanel;							// �����б�
	GUIStaticEx *m_pStcExQuestDeatail;					// ��������
	GUIStatic *m_pStcExpAwarad;							// ���齱��
	GUIStatic *m_pStcGoldAward;							// ��ҽ���
	GUIStatic *m_pStcSilverAward;						// ���ҽ���
	IconStatic *m_pPicItemAward[QUEST_REW_ITEM];		// ��Ʒ����
	DWORD m_dwRewardItemID[QUEST_REW_ITEM];				// ��ƷID

	GUIButton *m_pBtnRefreshQuest;						// ˢ�������б�
	GUIButton *m_pBtnAcceptQuest;						// ��ȡ����
	GUIButton *m_pBtnDeliverQuest;						// ��������
	GUIButton *m_pBtnExit;								// �˳�

	DWORD m_dwNpcID;									// NPC ID
	DWORD m_dwNpcTypeID;	
	DWORD m_dwEscapeTime;
	INT m_nSelRow;										// ѡ���������
	EFunctionNPCType m_eNPCType;
	vector<tagIDLink> m_idLinks;
	vector<tagPosLink> m_posLinks;
};
/********************************************************************
	created:	2008/04/30
	created:	30:4:2008   12:01
	filename: 	d:\loong_client\Client\QuestMainFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestMainFrame
	file ext:	h
	author:		leili
	
	purpose:	��������������
*********************************************************************/
#pragma once
#include "AppliedTree.h"
#include "IconStatic.h"
#include "QuestMgr.h"

class QuestMainFrame : public GameFrame
{
public:
	QuestMainFrame(void);
	~QuestMainFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 InitFocus(void);
	VOID		 Show(bool bReset=false);
	VOID		 Hide(void);
	BOOL		 IsVisible(void) const;

	VOID		 DeleteQuest(UINT16 questID);
	VOID		 AddQuest(UINT16 questID,bool sel=false);
	VOID		 SetQuestCompleteFlag(UINT16 questID);
	VOID		 SetQuestTrackFlag(UINT16 questID);
	VOID		 UpdateQuestDesc(UINT16 questID);
	VOID		 ChangeColor(void);
	VOID		 UpdateQuickDoneQuest();
	VOID		 ShowQuestNum();

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	VOID		 SetQuestDesc(UINT16 questID);

	DWORD		 OnEvent_CloseTrack(tagGameEvent* pEvent);
	DWORD		 OnEvent_CloseNPC(tagGameEvent* pEvent);
	DWORD		 OnEvent_OpenNPC(tagGameEvent* pEvent);
	DWORD		 OnEvent_CancelQuest(tagMsgBoxEvent* pEvent);

	VOID		 _ChangeColor(tagGUITreeItem* pItem);
	VOID		InsertWnd( GUIWnd *pWnd );

private:
	TSFPTrunk<QuestMainFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<InputDX8>				m_pInput;
	TObjRef<MsgBox>					m_pMsgBox;

	GUIWnd							*m_pWnd;				//������
	AppliedTree						*m_pTreeTaskList;		//�����б�Tree
	GUIButton						*m_pBtnQuestNpc;		//��ʾ/���ؿɽ�����NPCTree��ť
	GUIButton						*m_pBtnActiveQuest;		//��ʾ/���ػ���񴰿ڰ�ť
	GUIButton						*m_pBtnQuestTrack;		//��ʾ/��������׷�ٴ��ڰ�ť
	GUIButton						*m_pBtnDelQuest;		//ɾ������ť
	GUIButton						*m_pBtnCloseWnd1;		//�ر����������水ť����
	GUIButton						*m_pBtnCloseWnd2;		//�ر����������水ť����

	GUIStaticEx						*m_pStcBack;
	tagPoint						m_ptOriSize;
	GUIStaticEx						*m_pStcRewardBack;
	tagPoint						m_ptOriReWSize;
	GUIStaticEx						*m_pStcCondition;		//��ʾ������������
	GUIStatic						*m_pStcMoneyReward;
	GUIStatic						*m_pStcFixReward;
	GUIStatic						*m_pStcOptionReward;
	GUIStatic						*m_pStcBonusExp;		//���齱��
	GUIStatic						*m_pStcBonusMoney1;		//��Ǯ����1
	GUIStatic						*m_pStcBonusMoney2;		//��Ǯ����2
	//vector<GUIStatic*>				m_pStcBonusSpecT;
	//vector<GUIStatic*>				m_pStcBonusSpecV;
	GUIScrollBar					*m_pSrlCondition;
	GUIStaticEx						*m_pStcExQuestNum;

	typedef std::vector<IconStatic*> GUIStaticList;
	typedef std::vector<GUIStatic*> GUIStaticVec;
	GUIStaticList					m_stcBonusGoods;		//��Ʒ����
	GUIStaticList					m_stcBonusSels;			//��ѡ����
	GUIStaticVec					m_stcBonusGoodsBack;	// ��Ʒ��������
	GUIStaticVec					m_stcBonusSelsBack;		// ��ѡ��������
	DWORD							m_BonusGoodsID[QUEST_REW_ITEM];
	DWORD							m_BonusSelsID[QUEST_REW_ITEM];

	typedef std::map<DWORD, UINT16> TreeQuestMap;
	TreeQuestMap					m_tree2quest;

	bool							m_showNPC;
	bool							m_showTrack;

	vector<tagIDLink>				m_idLinks;
	vector<tagPosLink>				m_posLinks;

	tagPoint						m_ptPos;
};

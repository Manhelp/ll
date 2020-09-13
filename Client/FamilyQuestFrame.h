#pragma once
#include "..\WorldDefine\family_define.h"
#include "FamilyMgr.h"
/** \class FamilyQuestFrame
\brief ��������
*/

class IconStatic;
struct tagNS_OpenFamilyQuestPage;
struct tagNS_RewardFamilyMember;
struct tagNS_GiveInFamilyQuestItem;

class FamilyQuestFrame :
	public GameFrame
{
public:
	FamilyQuestFrame();
	~FamilyQuestFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	void SetFamilyQuestInfo(tagFamilyQuestDisplay* pData);	//ˢ�½�����Ϣ
	void ShowFrame(BOOL bIsShow);
	void ClearHandInWnd();
	void ClearRewardWnd();
	void ClearInfo();
	void SetRewardInfo();
	void RefrashInfo();
	void ShowErrorCode(DWORD dwErrCode);

private:
	//--����GUI�¼�
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnSetFamilyQuestInfo(tagGameEvent* pEvent);

	//--������Ϣ
	DWORD OnNS_RewardFamilyMember(tagNS_RewardFamilyMember* pCmd, DWORD);
	DWORD OnNS_GiveInFamilyQuestItem(tagNS_GiveInFamilyQuestItem* pCmd, DWORD);

private:
	TSFPTrunk<FamilyQuestFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		    m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	GUIStatic*					m_pStcYFinishDegree;						//������ɶ�
	GUIButton*					m_pBtnReceive;								//��ȡ����
	GUIStatic*					m_pStcFinishTip;							//�Ƿ����ȡ������ʾ

	GUIStatic*					m_pStcTFinishDegree;						//������ɶ�
	GUIStatic*					m_pStcPersent[MAX_FAMILY_QUESTS_NUM];		//������ɶ�
	GUIStatic*					m_pStcQuestDesc[MAX_FAMILY_QUESTS_NUM];		//��������
	GUIStatic*					m_pStcCurDegree[MAX_FAMILY_QUESTS_NUM];		//��ǰ��ɽ���
	GUIButton*					m_pBtnHandIn[MAX_FAMILY_QUESTS_NUM];		//�Ͻ�
	GUIStatic*					m_pStcKillTip[MAX_FAMILY_QUESTS_NUM];		//��ɱ��Ч..
	GUIStatic*					m_pStcHasFinishIcon[MAX_FAMILY_QUESTS_NUM];	//����ɵ��Ǹ���־..

	GUIWnd*						m_pWndReward;								//��ȡ�������
	GUIStatic*					m_pStcExp;
	GUIStatic*					m_pStcGold;
	GUIStatic*					m_pStcSilver;
	GUIButton*					m_pBtnClose;
	GUIButton*					m_pBtnGetReward;
	GUIButton*					m_pBtnCancel;
	IconStatic*					m_pIStcItem[MAX_FQ_RewardNum];	
	IconStatic*					m_pIStcItemEx[MAX_FQ_RewardNum];
	GUIStatic*					m_pStc80Got;	//���80%�ı�ǩ
	GUIStatic*					m_pStc100Got;	//���100%�ı�ǩ

	GUIWnd*						m_pWndHandIn;								//�Ͻ���Ʒ
	GUIPatch*					m_pWndHandInCaption;						//
	IconStatic*					m_pIStcItemIcon;
	GUIStatic*					m_pStcItemName;
	GUIButton*					m_pBtnHandInClose;
	GUIButton*					m_pBtnOK;
	GUIEditBox*					m_pEBoxNum;

	INT8						m_n8QuestIndex;
	DWORD 						m_dwItemTypeID;
	INT64						m_n64EquipID;


	tagTodayFamilyQuest			m_sFamilyQuestInfo[MAX_FAMILY_QUESTS_NUM];
	DWORD						m_dwYesterdayDegree;	//������ɶ�
	BOOL						m_bIsGetReward;			//�Ƿ���ȡ����Ľ���

};

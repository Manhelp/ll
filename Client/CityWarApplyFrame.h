#pragma once

#include "IconStatic.h"
struct tagCityWarApplyInfoRefreshEvent;
struct tagGetSomeGuildNameEvent;
struct tagNS_SignupForCitywar;


class CityWarApplyFrame : public GameFrame
{
public:
	enum EApplyType
	{
		EAT_ApplyManage,	// ������й���
		EAT_ApplyAttack,	// �����ս
		EAT_ApplyDefence	// �������
	};


	CityWarApplyFrame();
	~CityWarApplyFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	
private:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnEventApplyInfoRefresh(tagCityWarApplyInfoRefreshEvent *pEvent);
	DWORD OnEventGetSomeGuildName(tagGetSomeGuildNameEvent *pEvent);
	DWORD OnMsgBoxEventApplyManage(tagMsgBoxEvent *pEvent);
	DWORD OnMsgBoxEventConfirmDefence(tagMsgBoxEvent *pEvent);

	DWORD OnNetSignupForCitywar(tagNS_SignupForCitywar *pNetCmd);


	VOID UpdateUI();
private:
	TObjRef<NetSession> m_pSession;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TSFPTrunk<CityWarApplyFrame> m_Trunk;


	GUIWnd *m_pWnd;
	GUIStatic *m_pStcCityName;			// ��������
	IconStatic *m_pIstCityIcon;			// ����ͼ��
	GUIButton *m_pBtnCityInfo;			// ������Ϣ
	GUIStatic *m_pStcOwnGuild;			// ͳ��Ȩ��������
	GUIStatic *m_pStcNextWarTime;		// �´γ�սʱ��
	GUIStatic *m_pStcApplyCostLowLimit;	// ��������������
	GUIStatic *m_pStcApplycostAverage;	// ������ƽ������

	GUIListBox *m_pLstAttackGuildList;	// �������������б�
	GUIListBox *m_pLstDefenseGuildList;	// �������������б�

	GUIButton *m_pBtnApplyManage;		// ����������
	GUIButton *m_pBtnConfirmAssistDefense;	// ȷ��Э����������
	GUIButton *m_pBtnAttackApply;		// ��������
	GUIButton *m_pBtnDefenseApply;		// ��������
	GUIButton *m_pBtnCancel;			// ȡ��
	GUIButton *m_pBtnClose;				// �ر� 
	int m_nCurRowAttack;
	int m_nCurRowDefense;
	
	int m_nGuildNameGetFlag;	
	DWORD m_dwCityID;
	DWORD m_dwOwnGuildID;
	std::vector<DWORD> m_vecAttackIDs;
	std::vector<DWORD> m_vecDefenseIDs;
	std::vector<DWORD> m_vecConfirmIDs;
	EApplyType m_eApplyType;

	DWORD m_dwAvgMoney;
	DWORD m_dwTime;
};


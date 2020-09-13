/********************************************************************

** �ļ���: FairyAbilityFrame.h

** Copyright (c) �Ϻ�������缼�����޹�˾�����ֹ�˾ All Right Reserved

** ������: yfw

** ��  ��: 10/22/2010

** �޸���: yfw

** ��  ��: 10/22/2010

** ��  ��: ������������

*********************************************************************/
#include "..\WorldDefine\ItemDefine.h"
#include ".\BaseData.h"//add by xp
#pragma once

class ProgressEx;
struct tagNS_FairyStuntBook;
struct tagNS_FairyStuntConfirm;
struct tagNS_FairyAddPromoteTimes;


class FairyAbilityFrame : public GameFrame
{
public:
	FairyAbilityFrame(void);
	~FairyAbilityFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	VOID ShowUI(bool bShow);
	VOID UpdateUI();

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	VOID SetProsessColor( ProgressEx *pPro, INT nValue);
	DWORD NetLearnFairySkill( tagNS_FairyStuntConfirm *pNetCmd, DWORD );
	DWORD NetLearnFairyBook( tagNS_FairyStuntBook *pNetCmd, DWORD );
	DWORD NetAddFairyPromoteTimes( tagNS_FairyAddPromoteTimes *pNetCmd, DWORD );//add by xp
	DWORD OnLearnFairySkill( tagMsgBoxEvent *pGameEvent );
	DWORD OnQuitFairySkill( tagMsgBoxEvent *pGameEvent );
	DWORD OnUpdateFairyAbility( tagGameEvent *pGameEvent );
	DWORD OnUpgradeFairyInjTimes( tagMsgBoxEvent *pGameEvent );//add by xp
	DWORD OnUpgradeFairyResTimes( tagMsgBoxEvent *pGameEvent );//add by xp

private:
	TSFPTrunk<FairyAbilityFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;					// ������
	GUIStatic *m_pStcUpgradeDamageTimes;
	GUIStaticEx *m_pStcEleType;
	GUIStaticEx *m_pStcElePierceType;//Ԫ�ش�͸����  
	ProgressEx *m_pProDamage;
	ProgressEx *m_pProPierce;//Ԫ�ش�͸������  
	GUIStatic *m_pStcDamgeValue;
	GUIStatic *m_pStcPierceValue;//Ԫ�ش�͸��ֵ   
	GUIButton *m_pBtnUpgradeDamage;
	GUIButton *m_pBtnUpgradePierce;//Ԫ�ش�͸������ť
	GUIButton *m_pBtnAddEleDamgeTimes;//Ԫ���˺�\��͸ ���Ӵ�����ť
	GUIButton *m_pBtnAddEleDefenceTimes;//Ԫ�صֿ�  ���Ӵ�����ť

	GUIStatic *m_pStcUpgradeDefenceTimes;
	GUIStatic *m_pStcElement[EER_End];
	GUIStatic *m_pStcElementDefenceValue[EER_End];
	ProgressEx *m_pProDefence[EER_End];
	GUIButton *m_pBtnUpgradeDefence[EER_End];

	GUIStaticEx *m_pStcFairySkill[MaxFairySkillNum];
	GUIStaticEx *m_pStcCurSkill;
	GUIButton *m_pBtnLearn;
	GUIButton *m_pBtnQuit;

	DWORD m_dwFairySkill[MaxFairySkillNum];
	DWORD m_dwUnsureSkill;
	DWORD m_dwFairyUpgradeTimes;//�����˺��������� add by xp
	DWORD m_dwFairyUpgradeResTimes;//�����ֿ��������� add by xp

	BYTE m_byType;
	WORD m_wDamageTimes;
	WORD m_wDefenceTimes;
	INT m_nDamageValue;
	INT m_nPierceValue;//Ԫ�ش�͸��ֵ
	INT m_nDefenceValue[EER_End];

	
};
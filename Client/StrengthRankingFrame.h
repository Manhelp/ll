#pragma once

#include "..\WorldDefine\msg_rankings.h"
#include "SocialEvent.h"

struct tagNS_GetRankingInfo;
struct tagNS_GetOfflineEquipInfo;
struct tagNS_GetOfflineReputation;

class StrengthRankingFrame : public GameFrame
{
public:
	StrengthRankingFrame();
	~StrengthRankingFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	VOID SendGetRankInfo(int nPage, ERankingType eType);
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnNetGetRankInfo(tagNS_GetRankingInfo *pNetCmd);
	DWORD OnNetGetPetRankInfo(tagNS_GetPetRankingInfo *pNetCmd);
	DWORD OnNetGetOfflineEquipInfo(tagNS_GetOfflineEquipInfo *pNetCmd);
	DWORD OnNetGetOfflineReputation(tagNS_GetOfflineReputation *pNetCmd);
	VOID UpdateUI();

private:
	TSFPTrunk<StrengthRankingFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<NetSession> m_pSession;
	TObjRef<NetCmdMgr> m_pCmdMgr;

	GUIWnd *m_pWnd;


	GUIPushButton *m_pPbnStrength;	// ʵ��
	GUIPushButton *m_pPbnLevel;		// �ȼ�
	GUIPushButton *m_pBtnEquip;		// װ��
	GUIPushButton *m_pPbnFame;		// ����
	GUIPushButton *m_pPbnPet;		// ����
	GUIPushButton *m_pPbnFlower;	// ��Ů��~
	GUIPushButton *m_pPbnEgg;		// ������

	GUIStatic *m_pStcCommon;
	GUIListBox *m_pLstRanking;
	GUIButton *m_pBtnPre;		// ǰһҳ
	GUIButton *m_pBtnNext;		// ��һҳ
	GUIStatic *m_pStcCurNum;	// ��ǰҳ��
	GUIStatic *m_pStcLocalRank;	// �Լ����� 
	GUIButton *m_pBtnClose1;	// ���Ͻǹرհ�ť
	GUIButton *m_pBtnClose2;	// ���½ǹرհ�ť
	GUIButton *m_pBtnInfo;		// ����˵��

	int m_nCurPageNum;			// ��ǰҳ��
	ERankingType m_eCurType;	// ��ǰ����

	DWORD m_dwRank[RANKINGS_PAGE_LEN];		// ��¼��ǰ������ҳ��ROLEID
	DWORD m_dwLastRow;						// �ϴ�ѡ�����
};







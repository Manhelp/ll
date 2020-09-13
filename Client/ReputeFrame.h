#pragma once

#include "CurrencyMgr.h"
#include "ReputeMgr.h"
#include "ProgressEx.h"
#include "..\WorldDefine\reputation.h"
#include "..\WorldDefine\msg_famehall.h"

class FameHallFrame;
struct tagNS_SynTotalStrength;
struct tagNS_GetStrengthDetail;
struct tagNS_SynPvPStatics;
struct tagUpdateLPAttEvent;

/*!
	\class ReputeFrame
	\brief ����ϵͳ����
	\author hyu
	\base GameFrame
	\remark
*/
class ReputeFrame : public GameFrame
{
public:
	ReputeFrame(void);
	~ReputeFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	
	//ˢ��������ʾ
	VOID UpdateRepute();

private:
	DWORD OnNS_SynTotalStrength(tagNS_SynTotalStrength* pMsg, DWORD);
    DWORD OnNS_GetStrengthDetail(tagNS_GetStrengthDetail* pMsg, DWORD);
	DWORD OnNS_SynPvPStatics(tagNS_SynPvPStatics* pMsg, DWORD);

	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	//DWORD OnFamousUIOpen(tagGameEvent* pMsg);
	//DWORD OnFamousUIClose(tagGameEvent* pMsg);

    DWORD OnReputeChange(tagReputeChange* pMsg);
	DWORD OnAttibuteChange(tagUpdateLPAttEvent* pMsg);
    DWORD OnBecomeEvent(tagBecomeEvent* pMsg);
    DWORD OnRoleClanContributeChange(tagRoleClanContributeChange* pMsg);

	DWORD EventHandler(tagGUIEvent* pEvent);

	TSFPTrunk<ReputeFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI�ؼ�
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;

	//GUIButton*					m_pBtEnterFameHall;
    GUIButton*                  m_pBtRanking;

    GUIStatic*                  m_pStcGuildExploit;     //!< ���ɹ�ѫ
    //GUIStatic*                  m_pStcGuildExploitTxt;  //!< ���ɹ�ѫ����
    GUIStatic*                  m_pStcGuildContribute;  //!< ���ɹ���
    //GUIStatic*                  m_pStcExperiace;        //!< ��������

    GUIStatic*                  m_pStcStrengthValue;    //!< ʵ����ֵ
	GUIStatic*					m_pStcLevel;			// �ȼ�ʵ��ֵ
	GUIStatic*					m_pStcEquip;			// װ��ʵ��ֵ
	GUIStatic*					m_pStcSprite;			// ����ʵ��ֵ
	//GUIStatic*					m_pStcTitle;			// �ƺ�ʵ��ֵ
	//GUIStatic*					m_pStcFriend;			// ����ʵ��ֵ
    //GUIStatic*                  m_pStcStrengthBase;     //!< ����ʵ��
    //GUIStatic*                  m_pStcStrengthEx;       //!< ����ʵ��
    //GUIButton*                  m_pBtStrengthDetail;    //!< ʵ����ť

    GUIStatic*                  m_pStcWuXun;            //!< ��ѫ
	//GUIStatic*					m_pStcWuXunNextLevel;	//!< �¼���ѫֵ
    //GUIStatic*                  m_pStcWuXunTxt;         //!< ��ѫtxt
    GUIStatic*                  m_pStcWuJi;             //!< �似
	//GUIStatic*                  m_pStcWuJiMax;          //!< ��������似
    GUIStatic*                  m_pStcTotal;	        //!< 3v3 �ܳ���
    GUIStatic*                  m_pStcWinCount;         //!< 3v3 ʤ��

	GUIStatic*					m_pFlowerNum;	//�ʻ���
	GUIStatic*					m_pEggNum;		//������

	//struct tagReputeGUICollect;
	//tagReputeGUICollect* m_pReputeGUICollect;

	/*! ���� */
	//bool						m_bisFamousOpen;	/*!< �����ô���״̬ */
	//FameHallFrame*				m_pFameHallFrame;	/*!< �����ô���frame */

    bool                        m_bneedRefreshRepute;
    DWORD                       m_timeFirst;
};

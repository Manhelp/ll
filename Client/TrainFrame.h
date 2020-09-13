#pragma once
#include "gameframe.h"
#include "AppliedTree.h"
#include "ItemButton.h"
#include "TrainMgr.h"
#include "StaticEx.h"
#include "SysMenuFrame.h"
#include "..\WorldDefine\msg_train.h"
/** \class	TrainFrame

\brief ��	��֮�����������

\author:	Su.Zhang

\remarks ��
*/
struct tagNS_GetTrainList;
struct tagNS_FinishTrain;
struct tagNS_OpenTrain;

const tstring DEFAULT_PIC			=   _T("data\\ui\\Bag\\L-main-icon-lock.bmp");
const tstring PASS_PIC				=   _T("data\\ui\\Common\\pass.bmp");
const tstring PROCEED_PIC			=   _T("data\\ui\\Common\\proceed.bmp");
const tstring LOCK_PIC				=   _T("data\\ui\\Common\\lock.bmp");
const INT SHOWTIME = 3000;

class TrainFrame :
	public GameFrame
{
public:
	TrainFrame(void);
	~TrainFrame(void);
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

public:
	BOOL IsWindowVisible();
	VOID SetInvisibleOrNot();
	const VOID ShowPicCenter(tstring str);

private:

	VOID SafeDestroyUI();
	//��ݼ�����
	DWORD OnNS_FirstOpenTrainList(tagNS_GetTrainList* pNetCmd,DWORD dwParam);
	DWORD OnNS_FinishTrain(tagNS_FinishTrain* pNetCmd, DWORD dwParam);
	DWORD OnNS_OpenTrain(tagNS_OpenTrain* pNetCmd, DWORD deParam);

	BOOL  OnWnd_TrainOpen(tagGUIEvent* pEvent);
	BOOL  OnWnd_LoongTrainMain(tagGUIEvent* pEvent);

	void OnEvent_CloseTrainMain(tagGameEvent *pEvent);
	void OnEvent_OpenTrainMain(tagGameEvent *pEvent);

	VOID AutoMessage( DWORD dwTimedelay);

	VOID InitAppliedTrees();
	VOID ReloadTree();
	VOID RefreshUI();
	VOID ShowErrorTrainMessage(ETrainErrCode error);
private:

	TSFPTrunk<TrainFrame>			m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	//����������ʾͼƬ
	GUIWnd*							m_pWnd_Pic;

	GUIStatic*						m_pStc_Pic;
	//����������ʾ����
	GUIWnd*							m_pWnd_TrainOpen;	

	GUIStatic*						m_pStc_TrainName;	// �������ֱ�
	GUIButton*						m_pBn_Ok;			// ȷ����ť
	GUIButton*						m_pBn_Cancel;		// ȡ����ť

	//����������
	GUIWnd*							m_pWnd_TrainMain;

	GUIPushButton*					m_pPb_UnOpen;
	GUIPushButton*					m_pPb_Open;
	AppliedTree*					m_pTe_Title;
	GUIStaticEx*					m_pStcE_Content;
	GUIStatic*						m_pStc_TrainNameMain;
	GUIStatic*						m_pStc_Reward_Capabilitys;
	ItemButton*						m_pStc_CurrentState;
	GUIStatic*						m_pStc_AlreadyPassed;
	ItemButton*						m_pBn_Reward_Item1;
	ItemButton*						m_pBn_Reward_Item2;
	GUIButton*						m_pBn_Finish;
	GUIButton*						m_pBn_CloseMain;
	GUIButton*						m_pBn_X;	
	GUIScrollBar*					m_pSrlCondition;
	GUIScrollBar*					m_pSrlContent;

	BOOL							m_bIsFirstOpen;

	typedef std::map<DWORD, DWORD>	TrainTreeMap;
	TrainTreeMap					m_mapTrainTreeMap;  // ����ͼkey,appliedtree�е�ID��value����ID
	DWORD							m_dwCurrTrainID;	// appliedtree�е�Ŀǰѡ�е�ID

	BOOL							TimeMark;
	INT								timeCounter;		// ����ʱ���־λ
	
};

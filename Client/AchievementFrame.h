#pragma once
#include "gameframe.h"
#include "TrainMgr.h"
#include "StaticEx.h"
#include "SysMenuFrame.h"

#define MAX_SHOW_NUM 3

class ItemButton;
class AppliedTree;
class IconStatic;
struct tagNS_GetTrainList;
struct tagNS_FinishTrain;
struct tagNS_OpenTrain;
struct tagNS_SyncTrainCompleteness;

struct tagAchievement
{
	GUIStatic*		pStcName;			// �ɾ�����
	GUIStaticEx*	pStcOpenCondition;	// �ɾͿ�������
	GUIStaticEx*	pStcDesc;			// �ɾ�����
	GUIStaticEx*	pStcEContent;		// ����
	ItemButton*		pIbRewardOne;		// ����1
	ItemButton*		pIbRewardTwo;		// ����2
	GUIStatic*		pStcExp;			// ��������
	GUIStatic*		pStcGold;			// ��
	GUIStatic*		pStcSilver;			// ��
	GUIButton*		pBnTake;			// ��ȡ������ť
	IconStatic*		pStcFinish;			// ������ʾͼƬ
	GUIStatic*		pStcAlreadyTake;	// �Ƿ���ȡ����ͼƬ
	GUIStatic*		pStcItem1Back;		// ��Ʒ����һ��ͼ
	GUIStatic*		pStcItem2Back;		// ��Ʒ��������ͼ

	GUIPatch*		pPthBackDown;		// ˵������
	GUIStatic*		pPthBackUp;			// title����
};


class AchievementFrame :
	public GameFrame
{
public:
	AchievementFrame(void);
	~AchievementFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	BOOL IsWindowVisible();
private:
	VOID InitAppliedTrees();
	VOID ReloadTrees();
	VOID ReloadIDVector();
	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID RefreshUI();
	void OnEvent_CloseTrainMain(tagGameEvent *pEvent);
	void OnEvent_OpenTrainMain(tagGameEvent *pEvent);

	DWORD OnNS_FirstOpenTrainList(tagNS_GetTrainList* pNetCmd,DWORD dwParam);
	DWORD OnNS_FinishTrain(tagNS_FinishTrain* pNetCmd, DWORD dwParam);
	DWORD OnNS_OpenTrain(tagNS_OpenTrain* pNetCmd, DWORD deParam);
	DWORD OnNS_SyncTrainCompleteness(tagNS_SyncTrainCompleteness* pNetCmd, DWORD deParam);
	VOID ShowErrorTrainMessage(ETrainErrCode error);

	VOID ClearInfo(tagAchievement &strAch);
	bool IfCanShow();
	VOID ShowPic(tstring str);

private:
	TSFPTrunk<AchievementFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	// �ؼ�����
	GUIWnd*							m_pMainWnd;
	tagAchievement					m_strAchievement[MAX_SHOW_NUM];
	GUIPushButton*					m_pPbShowDone;	//����ɵĳɾ�
	GUIPushButton*					m_pPbShowUnOpen;//δ�����ĳɾ�
	AppliedTree*					m_pTree;
	GUIStatic*						m_pStcPercent;	//��ɱ���
	GUIStatic*						m_pStcCurentPage;	//��ǰҳ��
	GUIButton*						m_pPageUp;		
	GUIButton*						m_pPageDown;
	GUIButton*						m_pClose;

	// �߼�����
	INT								m_nCurPage;
	std::vector<DWORD>				m_vecAchieID;	// ��ǰ��Ҫ��ʾ�����й�ʽID����,������ʾ3����Ϣ
	typedef std::map<DWORD, DWORD>	TrainTreeMap;
	TrainTreeMap					m_mapTrainTreeMap;  // ����ͼkey,appliedtree�е�ID��value����type2
	DWORD							m_dwCurrTrainID;	// appliedtree�е�Ŀǰѡ�е�ID��2����

	std::list<tstring>				m_vecShowVector;	// ��Ҫ��ʾ��ɵĳɾ�
	DWORD							m_dwTimeMark;
	bool							m_bNeedSpace;

	GUIWnd*							m_pOpenWnd;
	GUIStatic*						m_pOpenText;
};

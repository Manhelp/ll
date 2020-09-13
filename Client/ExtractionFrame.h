//��ȡ
#pragma once
#include "ExtractionFrame.h"
#include "CompClidateDefine.h"
#include "ComColidateEvent.h"
#include "ItemEvent.h"

class ComposeStatic;
struct tagNS_Extraction;

class ExtractionFrame : public GameFrame
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
	ExtractionFrame(void);
	~ExtractionFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	//������漰��������
	void ClearUIandStuff();
	//�ж��䷽�Ƿ�����
	void JudgeDecompositonFormula(DWORD dwItemID, INT64 n64ID, EEquipPos ePos);
	//����
	void JudgeSendDecompose();
	//�ж�װ���Ƿ�ɵ㻯
	DWORD HandleDragStuff( tagGUIEvent* pEvent );
	//���ý���״̬
	void ResetStartUIState();
	//������ȡ��Ϣ
	void SendDecomposeMsg();
	//��ʾ����
	void Show();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventProduceAddItem( tagItemPosChangeExEvent* pGameEvent );
	DWORD OnOpenExtractFrame(tagGameEvent* pGameEvent);

	DWORD OnNS_Extraction(tagNS_Extraction* pCmd, DWORD);

private:
	TSFPTrunk<ExtractionFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*								m_pWndMain;
	GUIPatch*							m_pWndCaption;
	GUIButton*							m_pBtnExit;
	GUIButton*							m_pBtnCancel;
	GUIButton*							m_pBtnOk;
	GUIListBox*							m_pDeStuffList;			//�ɷֳ�����
	ComposeStatic*						m_pDeUnit;				//�ֽ���Ʒ
	GUIStatic*							m_pStcPotVal;			//����
	GUIProgress*						m_pProTime;				//������
	GUIStatic*							m_pStcTip;				//�㻯���Ǹ���ʾ��Ϣ
	GUIStatic*							m_pStcGoldCost;			//���Ľ���
	GUIStatic*							m_pStcSilverCost;		//��������

	DWORD								m_dwEquipID;						//��ǰ��ȡ��װ��ID
	DWORD								m_dwRecvMsgTime;					//�ɹ�\ʧ��\����ʱ��ʾ��ʱ��
	CompConsoContainer					m_Container;						//��������
	EDeComposeMsg						m_eCurState;						//��ǰ��Ϣ״̬
	DWORD								m_escapeTime;
	DWORD								m_dwNPCGolobelID;		//NPCȫ��ID
};
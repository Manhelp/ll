#pragma once

enum MallGiftFlag
{
	MGF_NULL	= 0,
	MGF_OK		= 1,	// ȷ��
	MGF_Cancel	= 2,	// ȡ��
};

struct tagMsgMallGiftEvent : public tagGameEvent
{
	tagMsgMallGiftEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	MallGiftFlag	eRet;
	tstring			strName;			//�������
	INT16			n16Num;				//������Ʒ����
	tstring			strWord;			//����
	DWORD			dwTime;
};

struct tagRefreshFriendsList : public tagGameEvent
{
	tagRefreshFriendsList(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
};

class MallGiftFrame : public GameFrame
{
public:
	MallGiftFrame(void);
	~MallGiftFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID UpdateNameList();
	void SetBuyNum();
	void SetMaxBuyNum(INT nMaxNum);
	void ShowGiftFrame(BOOL bShow);
private:
	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID ClearUIData();
	BOOL UpdateListData();
	BOOL UpdateListFrame();
	DWORD OnRefreshFriendsList(tagRefreshFriendsList* pEvent);
	DWORD OnClose_UIEvent(tagGameEvent* pEvent);
	VOID  DeleteRETURNS(tstring& strText);
private:

	TSFPTrunk<MallGiftFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;		
	GUIButton*					m_pBtnOK;
	GUIButton*					m_pBtnCancel;
	GUIEditBox*					m_pEdtName;					//�������
	GUIListBox*					m_pLBNameList;	
	//GUIComboBox*				m_pCbNameBox;				//��������б���Ͽ�
	GUIEditBox*					m_pEdtNum;					//����
	GUIEditBox*					m_pEditWord;				//����
	//DWORD						m_curRow;
	std::vector<LPCTSTR>		m_vecFriendsArray;			//��������б�
};
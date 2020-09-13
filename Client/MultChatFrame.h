#pragma once
#include "gameframe.h"

//-----------------------------------------------------------------------------
//!class MultChatFrame.h
//!brief Ⱥ�Ľ���  
//-----------------------------------------------------------------------------
class RichEditBoxForChatInput;
class RichEditBoxForChatShow;
class GlintListBox;
enum E_Click_Btn_Type
{
	Click_Btn_NULL = 0,
	Click_Btn_Create,
	Click_Btn_Delete,
	Click_Btn_Quit,
	Click_Btn_Kick,
	Click_Btn_AddMember
};
class MultChatFrame :
	public GameFrame
{
public:
	MultChatFrame(void);
	~MultChatFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	VOID   ShowWnd(BOOL b);
	BOOL   FrameIsInvisible();
	   
private:
	//m_pWnd��Ӧ
	DWORD	EventHandler(tagGUIEvent* pEvent);
	//�Ҽ������Ա�б�Ĳ˵���Ӧ
	DWORD	EventHandlerMenberMenu(tagGUIEvent* pEvent);
	//����³�Աm_pWndAddNew��Ӧ
	DWORD	OnWnd_AddNew(tagGUIEvent* pEvent);
	//����Ⱥ��msgbox��Ӧ
	DWORD	OnInputStringMsgbox(tagMsgInputBoxEvent* pGameEvent);
	//ɾ�����˳���msgbox��Ӧ
	DWORD   OnMsgBoxClickEvent(tagMsgBoxEvent* pGameEvent);
	VOID	KillObject();
	VOID    RefreshGroupList();
	VOID    RefreshMemberList();
	VOID    RefreshChatBoard();
    VOID    RefreshGlint();
	VOID    RefreshTitle();
	VOID    DeleteRETURNS(tstring& strText);
	VOID    RefreshAddMenberButton();

private:
	TObjRef<GUISystem>			m_pGUI;
	TSFPTrunk<MultChatFrame>	m_trunk;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;


	GUIWnd*						m_pWnd;
	GUIButton*                m_BtnNewChatGroup;		//��������Ⱥ
	GUIButton*			       m_BtnDelChatGroup;		//ɾ������Ⱥ
	GUIButton*                m_BtnQuitChatGroup;		//�˳�����Ⱥ
	GUIButton*					m_BtnAddMember;			//����³�Ա
	GUIButton*					m_BtnSendMessage;		//������Ϣ
	GUIButton*					m_BtnExitFrame;			//�˳�Ⱥ�Ľ���
	GlintListBox*             m_LBGroupNameList;		//Ⱥ���������б�
	GUIListBox*               m_LBMemberNameList;		//Ⱥ��Ⱥ��Ա�б�
	GUIStaticEx*              m_StcShowGroupIn;		//��ʾ���Ǹ�Ⱥ
	//RichEditBoxForChatShow*	   m_REBChatBlank;			//Ⱥ���촰��
	GUIRichEditBox*			   m_REBChatBlank;			//Ⱥ���촰��	
	RichEditBoxForChatInput*   m_REBLocalMessage;		//������Ϣ���ʹ���

	INT							m_clickGroupRow;				// ��ǰѡ���������Row
	INT							m_clickMemberRow;				//��ǰѡ�г�Ա��ROW			
	E_Click_Btn_Type			m_eClickBtnType;


	//----------------------------����³�Ա�Ӵ���-------------------------------
	GUIWnd*						 m_pWndAddNew;
	GUIButton*                 m_pBn_Ok_Set;                 //ȷ��
	GUIButton*                 m_pBn_Cancel_Set;             //ȡ��
	GUIButton*                 m_pBn_Close_Set;              //�ر�
	GUIEditBox*					 m_pEb_FriendName;			//�����
	GUIStatic*					 m_pStc_Title;				//�����
	GUIComboBox*				 m_pCb_Friends_Box;				//�����б���Ͽ�
	DWORD						m_curRow;					//�����б����		
	//----------------------------�Ҽ�����������Ӳ˵�----------------------------
	GUIWnd						*m_pWndMenberMenu;			// һ���Ӳ˵�
	GUIListBox					*m_pListMenberMenu;			// һ���б�

};

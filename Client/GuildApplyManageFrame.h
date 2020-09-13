#pragma once
#include "gameframe.h"
#include "..\WorldDefine\guild_define.h"
#include "..\WorldDefine\msg_guild.h"

class GuildApplyManageFrame : public GameFrame
{
public:
	GuildApplyManageFrame();
	~GuildApplyManageFrame();

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();

	void AddPlayerInfo( tagGuildApplyInfo& info );
	
	void ShowWnd( BOOL bShow );

private:
	typedef std::list<tagGuildApplierBillInfo>::iterator PlayerListIterator;

	DWORD OnNS_GuildApplierBill( tagNS_GuildApplierBill* pMsg, DWORD dwParam );

	BOOL EventHandler( tagGUIEvent* pEvent );
	
	tstring GetClassName( EClassType eClassType );

	int GetPages(){ return (m_lstPlayers.size() / GUILD_APPLY_MAX_ITEMS_PER_PAGE) + 1 ; }

	/** �жϵ�ǰҳ�Ƿ�Ϸ�
	*/
	bool IsCurPageGood(){ return m_nCurPage >= 0 && m_nCurPage < GetPages(); }

	/** ���ݵ�ǰҳ����ǰ�б���е�ѡ���������������б������
	*/
	int GetPlayerDataIndex();

	void UpdateGuildList( tagNS_GuildApplierBill* pMsg );

	void RefreshDisplay();

	/** ����ѡ�а����б��е���Ŀ�¼�
	*/
	void SelectListBoxItem( INT nSelectItem );

	GuildApplyManageFrame::PlayerListIterator FindGuildInfo( int nPlayerDataIndex );

	void OnChat();

	void OnRefuseApply();

	void OnAcceptApply();

	void AddPlayerInfo( std::list<tagGuildApplyInfo >& lstInfo );
	
	/** ����б��
	*/
	void ClearListBox();

	void DeletePlayer( DWORD dwRoleID );

	tstring GetOnlineText( DWORD dwTime );

private:
	TSFPTrunk<GuildApplyManageFrame> m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	GUIWnd*							m_pWnd;
	GUIPatch*						m_pPatchPlayers;		//����б�ҳ
	GUIPushButton*					m_pPbnGuildTenet;		//������ּ
	GUIPushButton*					m_pPbnGuildApply;		//�������
	GUIButton*						m_pBtnClose;			//�رհ�ť
	GUIButton*						m_pBtnChat;				//��Ϊ˽�Ķ���
	GUIButton*						m_pBtnAccept;			//ͬ��
	GUIButton*						m_pBtnRefuse;			//�ܾ�
	GUIListBox*						m_pLstPlayers;			//����б�

	GUIButton*						m_pBtnPageBack;			//��һҳ
	GUIButton*						m_pBtnPageFront;		//��һҳ
	GUIStatic*						m_pStcPageNum;			//��ǰҳ/���ҳ��ʾ
	
	//��ּ����
	GUIPatch*						m_pPatchTenet;			//������ּ
	GUIEditBoxEx*					m_pEditBoxTenet;		//��ּ�����
	GUIButton*						m_pBtnModify;			//�޸���ּ
	GUIButton*						m_pBtnCancel;			//ȡ����ť

	std::list<tagGuildApplierBillInfo>	m_lstPlayers;		//���������ɵ������Ϣ�б�
	int								m_nCurPage;
	int								m_nCurSelectItem;		//����б��б�ѡ����Ŀ������
};

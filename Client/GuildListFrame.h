#pragma once
#include "gameframe.h"
#include "..\WorldDefine\guild_define.h"

struct tagNS_GetAllGuildSmallInfo;
struct tagNS_GetGuildPurpose;
struct tagNS_WantToJoinGuild;
struct tagNS_ContactGuildLeader;
struct tagNS_SearchGuild;

class GuildListFrame : public GameFrame
{
public:
	GuildListFrame(void);
	~GuildListFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();

	void Show( bool bShow )
	{
		m_pWndApply->SetInvisible( !bShow );
		m_pWndApply->SetRefresh( TRUE );
	}

private:
	typedef std::list<tagGuildSmallInfo>::iterator GuildListIterator;

	BOOL EventHandler( tagGUIEvent* pEvent );

	/** ��ð�����Ϣ
	*/
	DWORD OnNS_GetAllGuildSmallInfo( tagNS_GetAllGuildSmallInfo* pMsg, DWORD dwParam );

	/** ��ð�����ּ 
	*/
	DWORD OnNS_GetGuildPurpose( tagNS_GetGuildPurpose* pMsg, DWORD dwParam );

	/** ���������ɵĻظ�
	*/
	DWORD OnNS_WantToJoinGuild( tagNS_WantToJoinGuild* pMsg, DWORD dwParam );

	/** ��������
	*/
	DWORD OnNS_SearchGuild( tagNS_SearchGuild* pMsg, DWORD dwParam );

	/** ��ϵ����
	*/
	DWORD OnNS_ContactGuildLeader( tagNS_ContactGuildLeader* pMsg, DWORD dwParam );

	/** �����������Ĵ�����
	*/
	DWORD OnApplyToJoinGuild( tagMsgBoxEvent* pEvent );
	
	/** �õ���ҳ��
	*/
	int GetPages(){ return (m_lstGuildInfo.size() / GUILD_APPLY_MAX_ITEMS_PER_PAGE) + 1 ; }
	
	/** �жϵ�ǰҳ�Ƿ�Ϸ�
	*/
	bool IsCurPageGood(){ return m_nCurPage >= 0 && m_nCurPage < GetPages(); }

	void UpdateGuildList( tagNS_GetAllGuildSmallInfo* pMsg );

	void AddGuildInfo( tagGuildSmallInfo& info );

	/** ˢ���б������ʾ�İ��ɵ���Ϣ
	*/
	void RefreshGuildsDisplay();

	/** �������ʵİ���
		\param nType 0 Ϊ�ǰ��հ��ɵ�����������1�ǰ��հ�������������
		\return ����ҵ����ʵİ��ɷ�������listBox�е����������򷵻�GT_INVALID
	*/
	INT SearchGuild( INT type, tstring strName );

	
	/** ���ݰ��������б������ֵ���Ҷ�Ӧ������
		\return ����ֵ Ҫ�ж��Ƿ���� m_lstGuildInfo.end() ��Ϊ
				�б�����ڴ��ڼ䱻�ı�
	*/
	GuildListFrame::GuildListIterator FindGuildInfo( int nGuildDataIndex );

	/** ���ݰ��ɵ�ID���Ұ�����Ϣ
	*/
	GuildListFrame::GuildListIterator FindGuildInfoByID( DWORD dwGuildID );

	/** �����������ɶԻ���
	*/
	void OpenApplyMsgBox();

	/** �򿪰�����ּ��
	*/
	void OpenGuildPurpose();

	/** ��ϵ����
	*/
	void ContactGuildLeader();
	
	/** ���ݵ�ǰҳ����ǰ�б���е�ѡ���������������б������
	*/
	int GetGuildDataIndex();

	/** ����ѡ�а����б��е���Ŀ�¼�
	*/
	void SelectListBoxItem( INT nSelectItem );

	/** ����б��
	*/
	void ClearListBox();

private:
	TSFPTrunk<GuildListFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	
	//�����б����
	GUIWnd*							m_pWndApply;
	GUIButton*						m_pBtnPageBack;			//��һҳ
	GUIButton*						m_pBtnPageFront;		//��һҳ
	GUIButton*						m_pBtnSearch;			//��������
	GUIButton*						m_pBtnPurpose;			//������ּ
	GUIButton*						m_pBtnApply;			//�������
	GUIButton*						m_pBtnCancel;			//ȡ��
	GUIButton*						m_pBtnClose;			//�رհ�ť
	GUIStatic*						m_pStcPageNum;			//��ǰҳ/���ҳ��ʾ
	GUIListBox*						m_pLstGuilds;			//�����б�

	//��������
	GUIWnd*							m_pWndSearch;			//������������
	GUIEditBox*						m_pWndInput;			//����Ҫ����������
	GUIButton*						m_pBtnSearchOk;			//ȷ��
	GUIButton*						m_pBtnSearchCancel;		//ȡ��
	GUIButton*						m_pBtnSearchClose;		//�ر�
	GUIPushButton*					m_pPbtnLeader;			//���հ�����������
	GUIPushButton*					m_pPbtnGuild;			//���հ��ɵ���������

	//������ּ����
	GUIWnd*							m_pWndTenet;			//������ּ����
	GUIButton*						m_pBtnContactLeader;	//��ϵ����
	GUIButton*						m_pBtnTenetCancel;		//ȡ��
	GUIStaticEx*					m_pStcTenet;			//������ּ����

	std::list<tagGuildSmallInfo>	m_lstGuildInfo;			//���а�����Ϣ
	INT								m_nMaxPage;				//���ҳ
	INT								m_nCurPage;				//��ǰҳ

	tstring							m_strSearchName;		//��������������
	int								m_nCurSelectItem;		//�����������İ�����listBox�е���Ŀ����

	DWORD							m_dwLookingPurposeGuildID;	//���ڲ鿴�˰��ɵİ�����ּ
};

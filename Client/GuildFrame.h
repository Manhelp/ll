#pragma once
#include "gameframe.h"
#include "GuildMembers.h"
#include "..\WorldDefine\msg_GuildBattle.h"
#include "..\WorldDefine\msg_guild.h"
#include "..\WorldDefine\guild_define.h"


struct tagRoleGetNameEvent;
struct tagGetNameByNameID;
struct tagGuildPosAppointEvent;
struct tagGuildInfoReflashEvent;
struct tagGuildNameGetEvent;
struct tagGuildBase;
struct tagGuildMemberClient;
struct tagUseGuildSkillBookEvent;
struct tagRefreshRemnantStateEvent;
struct tagGetSomeGuildNameEvent;
struct tagNS_GetAllGuildSmallInfo;
struct tagNS_SearchGuild;
struct tagNS_DeclareGuildWar;
struct tagNS_GuildWarEnd;
struct tagNS_SyncWarStateGuild;
struct tagNS_GetGuildUpgradeInfo;
struct tagNS_UpdateFacilitiesInfo;

class IconStatic;
struct tagGuildSkill;
class Item;
class GlintButton;
struct tagSureAllianceEvent;
struct tagMoneyConfirmApply;
class ItemButton;
struct tagNS_ActiveGuildSkill;

#define GUILD_SKILL_ITEM_NUM_PERPAGE 12
#define GUILD_UPGRADE_NUM 4
#define GUILD_WAR_MAXALLY_NUM 5

class GuildFrame :
	public GameFrame
{
public:
	typedef vector<GUIButton*> GUIButtonList;
	
	struct tagMemberNameReq
	{
		INT		nIndex;
		DWORD	dwColor;

		tagMemberNameReq(INT nIdx, DWORD dwClr){nIndex = nIdx; dwColor = dwClr;}
	};

	struct tagTempItem
	{
		DWORD dwID;
		INT64 n64ID;
		tagTempItem():dwID(0),n64ID(0)
		{}

		VOID clear()
		{
			dwID = 0;
			n64ID = 0;
		}
	};

	typedef map<DWORD, tagMemberNameReq> NameReqList;  
	
	// ���ɽ���״̬
	enum EGuildFrameState
	{
		EGFS_MemberList,	//��Ա���
		EGFS_Remnant,		//��״̬��׼������ʵ��ս��״̬��
		EGFS_GuildSkill,	//���Ἴ��
		EGFS_Construction,	//������ʩ
		EGFS_War			//����ս��
	};

public:
	GuildFrame(void);
	~GuildFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	EGuildFrameState GetFrameState() const { return m_eGuildFrameState; }
	VOID ShowWnd(bool bShow){ m_bShow = bShow; m_pWnd->SetInvisible(!bShow);}
	bool IsShow() const { return m_bShow; }
private:

	typedef std::list<tagGuildSmallInfo>::iterator GuildListIterator;

	VOID EventHandler(tagGUIEvent* pEvent);

	// ˢ�°���
	VOID ReflashGuild();
	// ˢ���б�
	VOID ReflashList();
	// ˢ���б�IMPL
	VOID ReflashListImpl();
	// ˢ�¹��ܰ�ť
	VOID ReflashButton();
	// ˢ�¼���
	void RefreshSkills();
	// ˢ����״̬
	void RefreshGodState();

	void UpdateSelectGodInfo(EGodMiracle eGod);

	// ��ȡ����ʱ����ʾ
	tstring GetOfflineTime(bool bIsOnline, DWORD dwOfflineTime = GT_INVALID);
	
	// ����������Ϣ
	VOID SendGuildAppoint(DWORD dwDstRoleID, INT nPos);
	VOID SendGuildLeave();
	VOID SendGuildKick(DWORD dwDstRoleID);
	VOID SendGuildTurnover(DWORD dwDstRoleID);
	VOID SendGuildDemiss();
	VOID SendGuildTenetChange(tstring strTenetInfo);
	
	// ������Ϸ�¼�
	DWORD OnEventGetRoleName(tagRoleGetNameEvent* pGameEvent);
	DWORD OnEventGetNameByNameID(tagGetNameByNameID* pGameEvent);
	DWORD OnEventGetGuildName(tagGuildNameGetEvent* pGameEvent);
	DWORD OnEventGetGuildTenet(tagGameEvent* pGameEvent);
	DWORD OnEventGuildPosAppoint(tagGuildPosAppointEvent* pGameEvent);
	DWORD OnEventGuildInfoReflash(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildListReflash(tagGameEvent* pGameEvent);
	DWORD OnEventGuildInfoReq(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventMsgBoxInputTenet(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxLeaveCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxTurnoverCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxKickCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxDemissCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventRefreshGuildSkills(tagGameEvent *pEvent);
	DWORD OnEventShowGuildFrame(tagGameEvent *pEvent);
	DWORD OnEventHideGuildFrame(tagGameEvent *pEvent);
	DWORD OnEventRefreshGuild(tagGameEvent *pEvent);
	DWORD OnEventUsGuildSkillBook(tagUseGuildSkillBookEvent *pGameEvent);
	DWORD OnEventRefreshBook(tagGameEvent *pGameEvent);
	DWORD OnEventCommerceWarningMsgBox(tagMsgBoxEvent *pEvent);
	DWORD OnEventRefreshClanRemnantState(tagGameEvent *pGameEvent);
	DWORD OnEventApply(tagMsgBoxEvent *pEvent);
	DWORD OnEventAddMoney(tagMsgBoxEvent *pEvent);
	DWORD OnEventShowRemnant(tagGameEvent *pEvent);
	DWORD OnEventKickGuildMember(tagGameEvent *pEvent);
	DWORD OnEventOpenGuildAllianceList(tagSureAllianceEvent *pEvent);
	DWORD OnEventMsgBoxWarCheck(tagMsgBoxEvent* pGameEvent);

	DWORD OnEventApplyAttackAlly(tagMsgBoxEvent *pEvent);
	DWORD OnEventApplyDefenceAlly(tagMsgBoxEvent *pEvent);

	DWORD OnEventMoneyConfirmApply(tagMoneyConfirmApply *pGameEvent);
	DWORD OnEventTurnOverHuiZhang(tagGameEvent *pGameEvent);


	DWORD OnNS_GuildApplierBill( tagNS_GuildApplierBill* pMsg, DWORD dwParam );

	DWORD OnNS_DeclareGuildWar(tagNS_DeclareGuildWar* pMsg, DWORD dwParam);
	DWORD OnNS_GuildWarEnd(tagNS_GuildWarEnd* pMsg, DWORD dwParam);
	DWORD OnNS_SyncWarStateGuild(tagNS_SyncWarStateGuild* pMsg, DWORD dwParam);

	DWORD OnNS_GetGuildUpgradeInfo( tagNS_GetGuildUpgradeInfo* pCmd, DWORD );
	DWORD OnNS_UpdateFacilitiesInfo( tagNS_UpdateFacilitiesInfo* pCmd, DWORD );
	DWORD OnNS_ActiveGuildSkill( tagNS_ActiveGuildSkill* pCmd, DWORD );

	// ����ť�����¼�
	VOID OnClickedLookupInfo();
	VOID OnClickedLeaveGuild();
	VOID OnClickedTurnoverLeader();
	VOID OnClickedAppointMember();
	
	//VOID OnClickedModifyTenet();
	void OnClickManageGuild();
	VOID OnClickedKickMember();
	VOID OnClickedDemissPostion();

	VOID DestroyMsgBox();
	void SetSkillDefaultUI();
	void OnSkillButtonClick(int nButton);		// ѡ��ĳ������button
	void OnClickPreOrNextButton(bool bOrder);	// trueΪ���ˣ�falseΪǰ��

	// ��ȡ��ǰ�еĳ�Ա
	const tagGuildMemberClient* GetMember(INT nIndex);

	/**
		\brief �õ��� nRow ��������ʾ�ĳ�Ա ���������ɱ��е�ʵ������
		\param nRow ���ͣ������
		\return ʵ�ʵ�����
	*/
	int GetMember2GuildTips( int nRow );

	//  [7/12/2010 zhangsu]

	/** ��ð�����Ϣ
	*/
	DWORD OnNS_GetAllGuildSmallInfo( tagNS_GetAllGuildSmallInfo* pMsg, DWORD dwParam );

	/** ��������
	*/
	DWORD OnNS_SearchGuild( tagNS_SearchGuild* pMsg, DWORD dwParam );

	void UpdateGuildList( tagNS_GetAllGuildSmallInfo* pMsg );

	void AddGuildInfo( tagGuildSmallInfo& info );

	/** ˢ���б������ʾ�İ��ɵ���Ϣ
	*/
	void RefreshGuildsDisplay();

	/** ����б��
	*/
	void ClearListBox();

	/** �õ���ҳ��
	*/
	int GetPages(){ return (m_lstGuildInfo.size() / GUILD_APPLY_MAX_ITEMS_PER_PAGE) + 1 ; }

	/** ����ѡ�а����б��е���Ŀ�¼�
	*/
	void SelectListBoxItem( INT nSelectItem );

	/** �������ʵİ���
	\param nType 0 Ϊ�ǰ��հ��ɵ�����������1�ǰ��հ�������������
	\return ����ҵ����ʵİ��ɷ�������listBox�е����������򷵻�GT_INVALID
	*/
	INT SearchGuild( INT type, tstring strName );

	VOID OnStartWar();
	VOID UpdataFacilitiesInfoUI( const EFacilitiesType eType );

	VOID HandInItems(const EFacilitiesType eType);

	// ˢ��ͬ���б�
	VOID RefreshAlliance(EGodMiracle eType);

	VOID SureAlliance();

	VOID SelectListBoxAlly(INT nSelectItem);

	VOID RefreshAllianceDisplay();

	VOID RefreshGuildXuanZhan();
private:
	TSFPTrunk<GuildFrame>		m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GUISystem>			m_pGUI;	
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	NameReqList					m_mapTemp;
	vector<INT>					m_vecIndex;			// ��Ա��������
	GuildMembers::EListColType	m_eSortType;		// ����ʽ


	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndMembers;
	GUIButton*					m_pBtnClose;
	GUIListBox*					m_pLstMembers;		// ��Ա�б�
	GUIStatic*					m_pStcName;			// ��������
	GUIStatic*					m_pStcLevel;		// �ȼ�
	IconStatic*					m_pStcSymbol;		// ���ɱ�־
	GUIStatic*					m_pStcFounder;		// ��ʼ��
	GUIStatic*					m_pStcLeader;		// ����
	GUIStatic*					m_pStcMemberNum;	// ��Ա����	
	GUIStatic*					m_pStcReputation;	// ����

	GUIButton*					m_pBtnZhongzhi;		//������ּ
	

	GUIStatic*					m_pStcFund;			// �ʽ�
	GUIStatic*					m_pStcMaterial;		// �ʲ�
	GUIStatic*					m_pStcPeace;		// ������
	GUIStatic*					m_pStcDailyCost;	// ÿ��ά����
	GUIEditBoxEx*				m_pEdtTenet;		// ������ּ
	GUIPushButton*				m_pPbnMembers;		// ��Ա�б�
	GUIPushButton*				m_pPbnSkills;		// ���ɼ���
	GUIPushButton*				m_pPbnInfomation;	// ս��״̬
	GUIPushButton*				m_pPbnWar;			// ������ս
	GUIPushButton*				m_pPbnConstruction;	// ������ʩ
	
	
	GUIButtonList				m_pBtnListIndex;	// ��Ա�б��ͷ
	GUIButton*					m_pBtnLookupInfo;	// �鿴��Ϣ
	GUIButton*					m_pBtnLeaveGuild;	// �뿪����
	GUIButton*					m_pBtnTurnLeader;	// �ƽ�����
	GUIButton*					m_pBtnAppointMember;// ����ְλ
	GlintButton*				m_pBtnManageGuild;	// �޸���ּ,�޸�Ϊ֧����˸
	GUIButton*					m_pBtnKickMember;	// �����Ա
	GUIButton*					m_pBtnDemissPostion;// ��ȥְ��
	GUIPushButton*				m_pPbnOffline;		// ��ʾ����

	GUIPatch*					m_pGuildMemberPatch;
	GUIListBox*					m_pGuildMemberListBox;

	const tagGuildBase*			m_pGuild;			// ������Ϣ

	DWORD						m_dwDstRoleID;		// ������Ŀ��ID

	GUIWnd *m_pWndGuild;			// ������
	IconStatic *m_pBtnCurSkillIcon;	// ��ǰ�о�����ͼ��
	GUIStatic *m_pStcCurSkillName;	// ��ǰ�о���������
	GUIStatic *m_pStcCurSkillLevel; // ��ǰ�о����ܵȼ�
	GUIStatic *m_pStcFundCost;		// ���İ����ʽ�
	GUIStatic *m_pStcMaterialCost;	// ���İ����ʲ�
	GUIProgress * m_pProResearchProgress; //��ǰ�����о�����

	ItemButton *m_pBtnBookIcon;		// ����伮
	GUIStatic *m_pProgressIncrement; // ��������
	GUIButton *m_pLearnBook;		// ���е伮

	GUIButton *m_pBtnBack;
	GUIButton *m_pBtnNext;

	GUIStatic *m_pStcLearnGoldCost;			// ѧϰ�������
	GUIStatic *m_pStcLearnSilverCost;		// ѧϰ��������
	GUIStatic *m_pStcLearnContributionNeed; // ѧϰ������ɹ���
	GUIStatic *m_pStcLearnGuildFundCost;	// ѧϰ��������ʽ�
	GUIStatic *m_pStcLearnGuildMaterialCost;// ѧϰ��������ʲ�

	GUIButton *m_pBtnActiveResearch;		// �����о�
	GUIButton *m_pBtnLearnSkill;			// ѧϰ����
	GUIButton *m_pBtnCancelResearch;		// ȡ���о�
	GUIButton *m_pBtnStartResearch;			// ��ʼ�о�
	

	GUIPatch *m_pSkillPatch;
	IconStatic *m_pIstState[5];				// ����״̬
	
	IconStatic *m_pSkillItemBack[GUILD_SKILL_ITEM_NUM_PERPAGE];
	IconStatic *m_pSkillItem[GUILD_SKILL_ITEM_NUM_PERPAGE];
	GUIStatic *m_pSkillName[GUILD_SKILL_ITEM_NUM_PERPAGE];
	GUIStatic *m_pSkillLevel[GUILD_SKILL_ITEM_NUM_PERPAGE];
	
	GUIPatch *m_pRemnantStatePatch;			// ��״̬
	
	GUIListBox *m_pLstClanState;			// ������״̬�б�

	GUIStatic *m_pStcBattleDesc;			
	GUIStatic *m_pStcBattleTime;			
	
	GUIStatic *m_pStcAttackGuildName;		// ������������
	GUIStatic *m_pStcAttackGuildLv;			// �������ɵȼ�
	GUIStatic *m_pStcAttackGuildMember;		// �������ɳ�Ա��
	//GUIStatic *m_pStcAttackMoney;			// ��ǰ���ۣ�F�ƻ����ģ�

	GUIStatic *m_pStcDefenceGuildName;		// ������������
	GUIStatic *m_pStcDefenceGuildLv;		// �������ɵȼ�
	GUIStatic *m_pStcDefenceGuildMember;	// �������ɳ�Ա��
	//GUIStatic *m_pStcDefenceDay;			// ����ռ������
	
	GUIEditBox *m_pEdtMoney;				// ���������
	GUIButton *m_pBtnApply;					// ����
	GUIStatic *m_pStcNowBegMoney;			// �Ը�ս���ĵ�ǰ����

	GUIButton *m_pBtnBackMoney;				// ׷�Ӿ���
	GUIButton *m_pBtnApplyAttackAlly;		// ���������ͬ��
	GUIButton *m_pBtnApplyDefenceAlly;		// ���������ͬ��
	GUIButton *confirmAllys;				// ȷ��ͬ��

	GUIStatic *m_pStcAttackAllies[GUILD_WAR_MAXALLY_NUM];			// ����ͬ����
	GUIStatic *m_pStcDefenceAllies[GUILD_WAR_MAXALLY_NUM];			// ����ͬ����

	int m_nCurChooseRenmant;				// ��ǰѡ����
	bool m_bNeedUpdate;

	std::vector<DWORD> m_vecClanStateID;	// ��¼��ID
	int m_nClanSateFlag;

	int m_nCurPageNum;	// ��ǰ��ҳ��
	DWORD m_dwCurChooseSkill; // ��ǰѡ�а��ɼ���ID;
	std::vector<tagGuildSkill*> m_vecCurPageItem;
	//  [7/12/2010 zhangsu] ����ս����������
	tstring							m_strSearchName;		//��������������
	std::list<tagGuildSmallInfo>	m_lstGuildInfo;			//���а�����Ϣ

	//std::set<DWORD>					m_setWarGuilds;			//����ս���еĹ���
	
	EGuildFrameState m_eGuildFrameState;
	bool m_bShow;
	Item *m_pSkillBook;			// ����伮��ָ��

	DWORD m_dwTempRoleID; // ��ʱ�����ɫID
	INT m_nTempPOS;     // ��ʱ����POSID
	BOOL m_bIsHaveGuildApply;  //�Ƿ�����������TRUEΪ��FALSEΪ��

	//  [7/12/2010 zhangsu]
	GUIPatch*					m_pGuildConstruction;	//������ʩ

	tagGuildFacilitiesInfo	        m_sFacilitiesInfo[MAX_GUILD_FACILITIES_TYPE];  // ��ʩ������Ϣ
	GUIStatic *m_pStcConstrLevel[EFT_End];		//�ȼ�
	GUIStatic *m_pStcConstrGold[EFT_End];		//��
	GUIStatic *m_pStcConstrSilver[EFT_End];		//��
	GUIStatic *m_pStcConstrResource[EFT_End];	//��Դ
	GUIProgress * m_pProConstResearch[EFT_End];	//��������

	GUIButton *m_pBtnLevelUp[EFT_End];			//����

	//����ս��

	GUIPatch*					m_pGuildWar;	//����ս��

	GUIListBox*					m_pLstGuilds;		// ��Ա�б�
	GUIButton *m_pBtnPageBack;
	GUIButton *m_pBtnPageFront;

	GUIButton *m_pBtnSearch;			// ��������
	GUIButton *m_pBtnPartyWarStart;			// ��ս
	GUIButton *m_pBtnPartyWarCancel;		// ȡ��

	GUIStatic *m_pStcWarTimes;	//������սʣ�����

	GUIStatic *m_pStcPageNum;	
  	INT								m_nMaxPage;				//���ҳ
  	INT								m_nCurPage;				//��ǰҳ 
  	int								m_nCurSelectItem;		//�����������İ�����listBox�е���Ŀ����

	//��������
	GUIWnd*							m_pWndSearch;			//������������
	GUIEditBox*						m_pWndInput;			//����Ҫ����������
	GUIButton*						m_pBtnSearchOk;			//ȷ��
	GUIButton*						m_pBtnSearchCancel;		//ȡ��
	GUIButton*						m_pBtnSearchClose;		//�ر�
	GUIPushButton*					m_pPbtnLeader;			//���հ�����������
	GUIPushButton*					m_pPbtnGuild;			//���հ��ɵ���������

	//������ּ����
	GUIWnd*							m_pWndtenet;			
	GUIEditBoxEx*					m_pEditBoxTenet;		//��ּ�����
	GUIButton*						m_pBtnModifyTenet;			//�޸���ּ
	GUIButton*						m_pBtnCancelTenet;			//ȡ����ť
	GUIButton*						m_pBtnCloseTenet;			//ȡ����ť

	//ȷ��ͬ�˽���
	GUIWnd*							m_pWndAlliance;
	GUIListBox*						m_pLstAlliance;				//ͬ���б�
	GUIButton*						m_pBtnAllianceOk;			//ȷ��
	GUIButton*						m_pBtnAllianceCancel;		//ȡ��
	GUIButton*						m_pBtnAllianceXclose;		//X�Źرռ�
	GUIStatic*						m_pStcNowAllies;			//��ǰͬ������
	GUIStatic*						m_pStcRemainAllies;			//ʣ���ѡͬ������
	int								m_nCurAlly;
	DWORD							m_nCurAllyID;
	bool							needRefresh;
	EGodMiracle						m_eTypeEx;					// ˢ�¸�������

	// ����Ἴ�ܽ���
	GUIWnd*							m_pWndActiveSkill;
	GUIButton*						m_pBnActive;				// ���ť
	ItemButton*						m_pIbnActiveItem;			// ������Ʒ
	GUIButton*						m_pBnActiveClose;			// �رհ�ť
	tagTempItem						m_strActiveItem;			//����ʹ����Ʒ��ID
	// ˢ�±�־λ
	bool							m_bNeedRefreshAlliance;

	
};


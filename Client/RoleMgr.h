#pragma once


#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\remote_role.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\msg_role_title.h"
#include "PetEvent.h"
#include "..\WorldDefine\action.h"

class Role;
class Player;
class LocalPlayer;
class NetSession;
class ShadowPool;
class KeyCodeFrame;
struct tagNS_SetTransportMap;
struct tagLPRoleStateChangeEvent;
struct tagMousePointRoleIDChangeEvent;
struct tagRolePickEvent;
struct tagLeaveCombatEvent;
struct tagNS_AppearanceTransform;
struct tagNS_SyncAvataHairAndFace;
struct tagNS_RoleBeAttacked;
struct tagEnterCombatEvent;
struct tagNPCTalkEvent;
struct tagNS_WillShutdownWorld;
struct tagNS_IsDressUpAgree;
struct tagNS_ChangeClergy;
struct tagNS_RemoteRoleSkin;
struct tagNS_Skill;
struct tagNS_SyncFamilyActive;
struct tagNS_PlsInputReactiveKeyCode;
struct tagNS_ReactiveKeyCode;
struct tagNS_RemoteHolySoulAttChange;
struct tagNS_CallShengLing;
struct tagNS_CallShengLingDisappear;
class VirtualRole;
enum EPLayActionType;
enum ESoulAttOptType;
/** \class RoleMgr
	\brief ��ɫ������
*/
class RoleMgr
{
public:
	enum{MAX_VIEW_DIST=8};
public:
	RoleMgr(void);
	~RoleMgr(void);

	/** ������Ϸ����ʱ��ʼ��
	*/
	void Init(DWORD localPlayerID);
	/** �뿪��Ϸ����ʱ�ͷ���������
	*/
	void Destroy();
	/** ������Ϸ��ͼ
	*/
	void OnOpenMap(SceneGraph* pSG, const Vector3& localPlayerPos, const Vector3& localPlayerFaceto);
	/** �رյ�ͼʱ�ͷų����������������н�ɫ����
	*/
	void OnCloseMap(SceneGraph* pSG);


	DWORD OnHideOtherPlayers( tagGameEvent *pEvent )
	{
		m_bHideOtherPlayers = true; 
		return 0; 
	}
	DWORD OnShowOtherPlayers( tagGameEvent *pEvent )
	{ 
		m_bHideOtherPlayers = false;
		return 0;
	}

	map<DWORD, tagClientRemoteRoleFabaoInfo>& GetTrumpEffectMap() { return m_mapTrumpEffect; }

	map<DWORD, DWORD>& GetFashionEffectMap() { return m_mapFashionEffect; }

	/** ������Ұ���뼶��
	*/
	void SetViewDistLevel( const int viewDistLevel );
	int  GetViewDistLevel() const { return m_viewDistLevel; }
	/** ���ݽ�ɫID���ҽ�ɫ
		\param bAddReqIfNotFind:�Ƿ�������������ɫ����
	*/
	Role* FindRole(DWORD roleID,bool bAddReqIfNotFind=false,bool bFindDeadCreatureList=false);
	/** ��ñ�����ҽ�ɫ
	*/
	LocalPlayer* GetLocalPlayer(){ return m_pLocalPlayer; }
	/** ��ȡ��ɫ����
	*/
	const map<DWORD,Role*>& GetRoleMap() { return m_roleMap; }
	/** ��ȡ�������ID
	*/
	DWORD GetLocalPlayerID();
	/** ÿ֡����
	*/
	void Update(SceneGraph* pSG);
	
	/** ��ɫʰȡ
	    \bIgnorePlayer �Ƿ�������
	*/
	DWORD PickRole(const Ray& mouseRay, const BOOL bIgnorePlayer);

	// �Զ�ʰȡ��ɫ
	bool AutoPickRole(DWORD dwRoleID);
	/** ��ʾ/������Ӱ
	*/
	void EnalbeShadow( const BOOL bEnalbe ) { m_bEnalbeShadow = bEnalbe; }	
	/** ��ɫ�Ƿ��ڽ�ɫ��Ұ��
	*/
	bool IsInViewDist( Role* pRole );

	/** ��������Ƿ�Զ��NPC/���
	*/
	bool IsOutValidDist(DWORD dwRoleID);
	bool IsOutFaceDist(DWORD dwRoleID);
	// �����Ƿ�����Ч��Χ��
	bool IsInTeamBufDis( DWORD );
	/** �ж�AABBox���Ƿ��н�ɫ
	*/
	bool IsAABBoxCollideRole(const AABBox &box, bool bOnlyNPC = true);
	/** ���¿ɽ����������������Ч
	*/
	void UpdataGameObjectQuestEffect();
	static RoleMgr* Inst();
	// �õ������λ��
	const vector<tagACreaturePosInfo> &GetMonsterInfo() { return m_vecActiveMons; }
	// �Ƿ���ͬһʦ��
	bool IsInSameMasterGuild( Player *pPlayer );
	VOID PopUpVoteUI();
	VOID DestroyVoteUI();
	BOOL IsEnemy( DWORD dwRoleID );
	bool IsTransforming(Player* pPlayer);

	//DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);

	/** ͨ����ɫid��ȡ�������ɫ(Ԫ�����)*/
	VirtualRole* GetVirtualRole(DWORD dwRoleID);
	/** ͨ����ɫid��ȡ�Ƿ��������ɫ(Ԫ�����)*/
	BOOL HasVirtualRole(DWORD dwRoleID);
	/** ͬ�������ɫ��������ս����(Ԫ�����)*/
	VOID SynVRCombatAction( DWORD dwRoleID, tstring strActionName );
	//Ԫ�����
	VOID HolyManCtrl(DWORD dwRoleID, EPLayActionType eType, DWORD dwBuffID=GT_INVALID);
	//����Ԫ��ģ��, �����Ƿ�����   
	BOOL ReloadHolyManModel(DWORD dwRoleID, DWORD dwHolyID, ESoulAttOptType eOpt );

private:
	// �����
	DWORD OnNS_ActiveMoster( tagNS_SyncActiveCreaturesPos *pNetCmd, DWORD );

	/** ��ӵ���ɫ�����б�
	*/
	void AddReqRole(DWORD dwRoleID);
	/** ���ͽ�ɫ����
	*/
	void SendReqRoleList();
	/** �ͷ�һ����ɫ���������ͷ��������
	*/
	void FreeRole(DWORD roleID,SceneGraph* pSG);
	//---------------------------------------------------------------------
	//  ������Ϣ����                                                              
	//---------------------------------------------------------------------
	/** ͬ��������Ҽ����Ծ��
	*/
	DWORD OnNS_SyncFamilyActive(tagNS_SyncFamilyActive* pNetCmd, DWORD);
	/** ��ȡԶ����ұ���Ƥ��
	*/
	DWORD NetGetRemoteRoleTransSkin(tagNS_RemoteRoleSkin* pNetCmd, DWORD);
	/** ɾ��Զ�����
	*/
	DWORD NetRemoveRemote(tagNS_RemoveRemote* pNetCmd, DWORD);
	/** ��ʼ�������������
	*/
	DWORD NetInitLocalPlayerAtt(tagNS_GetRoleInitState_Att* pNetCmd, DWORD);
	/** ��ȡԶ�������Ϣ
	*/
	DWORD NetGetRemoteRoleState(tagNS_GetRemoteRoleState* pNetCmd, DWORD);
	/** ��ȡ��Χ������Ϣ
	*/
	DWORD NetGetRemoteCreatureState(tagNS_GetRemoteCreatureState* pNetCmd, DWORD);
    /** ��ȡ��Χ������Ϣ
    */
    DWORD NetGetRemotePetState(tagNS_GetRemotePetState* pNetCmd, DWORD);
    /** ��ȡ��Χ������Ϣ
    */
    DWORD OnGetPetMount(tagEnterMount* pGameEvent);
    /** ɾ������
    */
    DWORD OnLeavePetMount(tagLeaveMount* pGameEvent);
	/** ������ҵ������Ա仯
	*/
	DWORD NetRoleAttChangeSingle(tagNS_RoleAttChangeSingle* pNetCmd, DWORD);
	/** ������Ҷ������Ա仯 
	*/
	DWORD NetRoleAttChangeMutiple(tagNS_RoleAttChangeMutiple* pNetCmd, DWORD);
	/** Զ�̽�ɫ�������Ա仯
	*/
	DWORD NetRemoteAttChangeSingle(tagNS_RemoteAttChangeSingle* pNetCmd, DWORD);
	/** Զ�̽�ɫ�������Ա仯
	*/
	DWORD NetRemoteAttChangeMutiple(tagNS_RemoteAttChangeMutiple* pNetCmd, DWORD);
	/** Զ�̽�ɫ������Ϣ�仯
	*/
	DWORD NetRemoteGuildInfoChange(tagNS_RemoteRoleGuildInfoChange *pNetCmd, DWORD);
	/** ���Avatar�仯
	*/
	DWORD NetAvatarEquipChange(tagNS_AvatarEquipChange* pNetCmd, DWORD);
    //! ��ҳƺű仯
    DWORD NetRoleTitleChange(tagNS_RoleTitleChangeBroadcast* pNetCmd, DWORD);
	/** ���Avatarȫ�����£�ʱװ/װ���л���
	*/
	DWORD NetAvatarEquip(tagNS_AvatarEquip* pNetCmd, DWORD);
	/** ����Ч���ı�
	*/
	DWORD NetEquipEffectChange(tagNS_EquipEffectChange* pNetCmd, DWORD);
	/** ��װ��Ч�ı�
	*/
	DWORD NetSuitEffectChange(tagNS_SuitEffect* pNetCmd, DWORD);
	/** ���ӽ�ɫBuff
	*/
	DWORD NetAddRoleBuff(tagNS_AddRoleBuff* pNetCmd, DWORD);
	/** �Ƴ���ɫBuff
	*/
	DWORD NetRemoveRoleBuff(tagNS_RemoveRoleBuff* pNetCmd, DWORD);
	/** ���½�ɫBuff
	*/
	DWORD NetUpdateRoleBuff(tagNS_UpdateRoleBuff* pNetCmd, DWORD);
	/** ���±����������Ͷ��
	*/
	DWORD NetRoleAttPointAddChange(tagNS_RoleAttPointAddChange* pNetCmd, DWORD);
	/** Ͷ�㷵��
	*/
	DWORD NetRoleBidAttPoint(tagNS_RoleBidAttPoint* pNetCmd, DWORD);
	/** ϴ�㷵��
	*/
	DWORD NetRoleClearAttPoint(tagNS_RoleClearAttPoint* pNetCmd, DWORD);
	/** ��ӽ�ɫ����״̬
	*/
	DWORD NetSetRoleStateEx(tagNS_SetRoleStateEx* pNetCmd, DWORD);
	/** ȡ����ɫ����״̬
	*/
	DWORD NetUnsetRoleStateEx(tagNS_UnSetRoleStateEx* pNetCmd, DWORD);
	/** ������ҵ���ʾ����
	*/
	DWORD NetRoleSetDisplay(tagNS_RoleSetDisplay* pNetCmd, DWORD);
	/** ���ñ�����ҵĸ�������
	*/
	DWORD NetSetPersonalSet(tagNS_SetPersonalSet* pNetCmd, DWORD);
	/** ���������ı�
	*/
	DWORD NetCreatureTaggedOwnerChange(tagNS_CreatureTaggedOwnerChange* pNetCmd, DWORD);
	/** ��ɫ��Ӱ����
	*/
    /** ��ɫ�ɼ�¼���ͷ��ı�
    */
    DWORD OnNS_SetTransportMap(tagNS_SetTransportMap* pNetCmd, DWORD);

	//���������ʾ����Ҫ��Ϊ������ָ��
	DWORD OnNS_RoleLevelChange( tagNS_RoleLevelChange* pNetCmd, DWORD );

	// ������ָı�
	DWORD OnNetRename( tagNS_LPRename *pNetCmd, DWORD );
	// ������ָı�,֪ͨ��Χ���
	DWORD NetRoleRenameBroadcast( tagNS_RoleRename *pNetCmd, DWORD );
	// �����ְ�ı䣬֪ͨ��Χ���
	DWORD OnNS_ChangeClergy(tagNS_ChangeClergy* pNetCmd, DWORD);
	// ���Ԫ�����仯
	DWORD OnNS_RemoteHolySoulAttChange( tagNS_RemoteHolySoulAttChange *pNetCmd, DWORD );


	DWORD OnNS_PlsInputReactiveKeyCode(tagNS_PlsInputReactiveKeyCode* pNetCmd, DWORD);
	DWORD OnNS_ReactiveKeyCode(tagNS_ReactiveKeyCode* pNetCmd, DWORD);
    void ShowSetTransportErrorMsg(DWORD dwErrCode);

	VOID UpdataRoleShadow();
	/** ��ɫ�Ƿ���Ҫ����
	*/
	bool GetRoleHide( Role* pRole );
	/** ���¸�����ʾ
	*/
	void UpdataHightLight( Role* pRole );
	/** ������ȫ����ʾ
	*/
	DWORD OnLpChangeStateEvent( tagLPRoleStateChangeEvent *pEvent );
	// ��Ӧ���ָ��ı��¼�
	DWORD OnMousePointRoleIDChangeEvent(tagMousePointRoleIDChangeEvent *pEvent);
	// ��Ӧ��ɫѡ���¼�
	DWORD OnRolePickEvent(tagRolePickEvent *pEvent);
	// С��ŮͶƱ
	DWORD OnRoleVote( tagMsgInputBoxEvent *pEvent );
	DWORD OnNetVote( tagNS_VoteForDrangonGirl *pNetCmd, DWORD );
	DWORD OnNetChangeAvatar( tagNS_AppearanceTransform *pNetCmd, DWORD );
	DWORD OnEvent( tagGameEvent* pEvent );
	// ������Чͬ��
	DWORD OnNetRRoleChangeFabao( tagNS_RemoteRoleChangeFabao *pNetCmd, DWORD );
	// ��ұ�����
	DWORD OnNetBeAttacked( tagNS_RoleBeAttacked *pNetCmd, DWORD );
	// ����ս��״̬
	DWORD OnRoleLeaveCombatEvent( tagLeaveCombatEvent *pEvent );
	// ����ս��״̬
	DWORD OnRoleEnterCombatEvent( tagEnterCombatEvent *pEvent );
	// NPC����
	DWORD OnNPCTalkEvent( tagNPCTalkEvent *pEvent );
	// �ط�����ʱ
	DWORD OnNetShutDownServer( tagNS_WillShutdownWorld *pNetCmd, DWORD );
	// �Ծ��鵤
	DWORD OnNetUseExpPilular( tagNS_UseExpPilular *pNetCmd, DWORD );
	// ����װ���Լ�
	DWORD OnNetFriendBeautyMe( tagNS_IsDressUpAgree *pNetCmd, DWORD );
	// װ���Լ�
	DWORD OnEventFriendBeautyMe( tagMsgBoxEvent *pGameEvent );
	// �ٻ�ʥ�飨����������Һ�Զ����ң�
	DWORD OnNS_CallShengLing(tagNS_CallShengLing* pCmd, DWORD);
	// ʥ����ʧ������������Һ�Զ����ң�
	DWORD OnNS_CallShengLingDisappear(tagNS_CallShengLingDisappear*pCmd, DWORD);
	// ʥ����ʧ������������Һ�Զ����ң�
	//DWORD OnNS_UnSetRoleState(tagNS_UnSetRoleState* pCmd, DWORD);

	//ͬ����һ��߹�����Ӫ��Ϣ
	DWORD OnNS_SyncCamp(tagNS_SyncCamp* pCmd, DWORD);
	/**����Ԫ���VirtualRoleģ��*/
	BOOL LoadHolyManModel(Player* pPlayer, Vector3 v3FaceTo, DWORD dwModelID/*, tagAvatarAtt	Avatar, tagAvatarEquip	AvatarEquip, tagDisplaySet	DisplaySet*/ );
public:
	//ʥħʥ��VirtualRoleģ�ͼ��أ������Ǹ��ᱻ�����
	BOOL LoadHolyManModel(BOOL bIsHide = TRUE);
	//����Զ����ҵ�����ģ��
	BOOL LoadHolyManModel(DWORD dwRoleID, DWORD dwHolyTypeID, DWORD dwEatFairyNum);
	// Ԫ�����Ա仯���������Ե�Ӱ��
	//DWORD OnNS_GetRoleAttHolySoulEffect( tagNS_GetRoleAttHolySoulEffect *pNetCmd, DWORD );
	//����������ͣ��Ϸhttp����
	void SendyunLianPauthGame();
private:
	TSFPTrunk<RoleMgr>			m_trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
    TObjRef<GameFrameMgr>       m_pFrameMgr;
	ShadowPool*                 m_pShadowPool;
	BOOL                        m_bEnalbeShadow;

	LocalPlayer*				m_pLocalPlayer;		 // �������
	map<DWORD,Role*>			m_roleMap;			 // ��ɫmap
	list<Role*>					m_listDeadCreature;	 // ���������б�
	pair<DWORD, list<DWORD> >	m_paReqRole;		 // ��ɫ�����б�

	bool                        m_bHideOtherPlayers; // �Ƿ������������
	int							m_viewDistLevel;     // ��ɫ��Ұ����
	float                       m_fViewDist;         // ��ɫ��Ұ����
	DWORD						m_dwRolePointEffectID;// ��ɫѡ����ЧID
	vector<tagACreaturePosInfo>		m_vecActiveMons;		// �����λ��
	
	map<DWORD, tagClientRemoteRoleFabaoInfo>	m_mapTrumpEffect;		// ������ҷ�����Ч
	map<DWORD, DWORD> m_mapFashionEffect;			// �������ʱװ��Ч
	set<DWORD>				m_setEnemy;				// �����Լ��ĳ����б�
	DWORD					m_dwRecivMsgTime;		// �յ���Ϣ��ʱ��			
	KeyCodeFrame*			m_pKeyCodeFrame; //key��

	//VirtualRole*				m_pVirtualRole;
	map<DWORD, VirtualRole*> m_mapVirtualRole;
	//Vector3						m_v3FaceTo; //������ҳ���

};

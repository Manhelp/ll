#pragma once
#include "Loong_CommonDef.h"
class CLoongMergeLogicCtrl
{
public:
	CLoongMergeLogicCtrl();
	~CLoongMergeLogicCtrl(void);

	//����set get
public:
	//�ϲ�����
	VOID SetMergeType(EDBMergeType eMergeType)		{ m_eMergeType = eMergeType;}

	//�Ƿ������������
	VOID SetIfCleanData(BOOL bDataClean)		{ m_bDataClean = bDataClean; }

	// ����ʱ��
	VOID SetSurviveDay(INT nSurviveDay)		{ m_nItemDelEquipDelSurviveDay = nSurviveDay; }

	//Դ2����Ϸ������
	VOID SetSrc2WorldName(tstring strSrc2WorldName)	{ m_strSrc2WorldName = strSrc2WorldName;}

	//Դ1��account�����num
	VOID SetSrc1LoginMaxNum(DWORD dwSrc1LoginMaxNum)		{ m_dwSrc1LoginMaxNum = dwSrc1LoginMaxNum;}

	//Դ1roledata�����num
	VOID SetSrc1RoleMaxNum(DWORD dwSrc1RoleMaxNum)		{ m_dwSrc1RoleMaxNum = dwSrc1RoleMaxNum;}

	//Դ1petdata�����num
	VOID SetSrc1PetMaxNum(DWORD dwSrc1PetMaxNum)		{ m_dwSrc1PetMaxNum = dwSrc1PetMaxNum;}

	//�ڲ���Ʒ������
	VOID SetSrc1ItemMaxSerialInternal(INT64 n64ItemMaxSerialInternal)
	{
		m_n64Src1ItemMaxSerialInternal	=	n64ItemMaxSerialInternal;
		m_n64ItemCurSerialInternal		=	n64ItemMaxSerialInternal;
	}

	//�ⲿ��Ʒ��С���
	VOID SetSrc1ItemMinSerialOutter(INT64 n64ItemMinSerialOutter)	
	{
		m_n64Src1ItemMinSerialOutter	=	n64ItemMinSerialOutter;
		m_n64ItemCurSerialOutter		=	n64ItemMinSerialOutter;
	}

	//������Ʒ��С���
	VOID SetSrc1ItemMinSerialBill(INT64 n64ItemMinSerialBill)
	{
		m_n64Src1ItemMinSerialBill	= n64ItemMinSerialBill;
		m_n64ItemCurSerialBill		= n64ItemMinSerialBill;
	}

	//�õ�����Դ1�� ��id
	const std::vector <DWORD> &GetAllOldSrc1AccountID()		{ return m_vecOldSrc1AccountID; }

	//Դ1�У�Դ2Ҳ�е��û�����id
	const std::map <DWORD ,DWORD> &GetOldSrc1VsSrc2()		{ return m_mpOldSrc1VsSrc2; }
	//Դ2�У�Դ1Ҳ�е��û�����id
	const std::map <DWORD ,DWORD> &GetOldSrc2VsSrc1()		{ return m_mpOldSrc2VsSrc1; }

	//Դ2���е��û����ϡ� ��id
	const std::vector <DWORD> &GetOldSrc2OnlyAccountID()		{ return m_vecOldSrc2OnlyAccountID; }

	//�õ����а���id
	const std::set <DWORD> &GetAllOldGuildID()			{ return m_setOldGuildID; }

	//�õ���Ҳ���
	const std::vector <tagRoleCompensation> &GetRoleCompensation() { return m_vecRoleCompensation; }

	//�õ��µ��ⲿ������Ʒ64λid
	INT64 GetNewItemSerialOutter()	{ return --m_n64ItemCurSerialOutter; }

	//�õ���ֵ�˺�
	std::set<string>& GetCANameSet() { return m_setCAName; }

	//�õ�Դ1��ɫ��
	std::set<tstring>& GetSrc1RoleName() { return m_setSrc1RoleName; }

	//�õ�Դ1��ɫ��crc
	std::set<DWORD>& GetSrc1RoleNameCrc() { return m_setSrc1RoleNameCrc; }

	//�õ�Դ1��ְ��ɫID
	std::set<DWORD>& GetSrc1ClergyRoleID() { return m_setSrc1ClergyRoleID; }
	//�õ�Դ2��ְ��ɫID 
	std::set<DWORD>& GetSrc2ClergyRoleID() { return m_setSrc2ClergyRoleID; }

public:
	//login �������� �ȵ��������
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_Account*> &vecSrc1,
						std::vector <tagTWrap_Longin_Account*> &vecSrc2,
						std::vector <tagTWrap_Longin_Account*> &vecTar);
	//login worldforbid��
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc1,
						std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc2,
						std::vector <tagTWrap_Longin_WorldForbid*> &vecTar);
	//login ������
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_BlackList*> &vecSrc1,
						std::vector <tagTWrap_Longin_BlackList*> &vecSrc2,
						std::vector <tagTWrap_Longin_BlackList*> &vecTar);
	//login ������
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc1,
						std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc2,
						std::vector <tagTWrap_Longin_FatigueTime*> &vecTar);
	//login ȫ�ֽ�ɫ����
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_RoleName*> &vecSrc1,
						std::vector <tagTWrap_Longin_RoleName*> &vecSrc2,
						std::vector <tagTWrap_Longin_RoleName*> &vecTar);
	//login ȫ�ְ�������
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_GuildName*> &vecSrc1,
						std::vector <tagTWrap_Longin_GuildName*> &vecSrc2,
						std::vector <tagTWrap_Longin_GuildName*> &vecTar);

	//login ȫ�ּ�����
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_FamilyName*> &vecSrc1,
						std::vector <tagTWrap_Longin_FamilyName*> &vecSrc2,
						std::vector <tagTWrap_Longin_FamilyName*> &vecTar);
	
public:
	//loong ���û�п���� �ȵ��������
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc1,
						std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc2,
						std::vector <tagTWrap_Loong_AccountCommon*> &vecTar);

	//role data
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleData*> &vecSrc1,
						std::vector <tagTWrap_Loong_RoleData*> &vecSrc2,
						std::vector <tagTWrap_Loong_RoleData*> &vecTar);

	//����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Guild*> &vecSrc1,
						std::vector <tagTWrap_Loong_Guild*> &vecSrc2,
						std::vector <tagTWrap_Loong_Guild*> &vecTar);

	//item �ڲ���������������
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
						std::vector <tagTWrap_Loong_Item*> &vecSrc2,
						std::vector <tagTWrap_Loong_Item*> &vecTar);
	//item ʵ�ʴ�����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
						std::vector <tagTWrap_Loong_Item*> &vecSrc2,
						std::vector <tagTWrap_Loong_Item*> &vecTar,
						ETBItemType emTBType);
	//itembaibao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemBaiBao*> &vecTar);
	//itemdel
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemDel*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemDel*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemDel*> &vecTar);
	//item_cdtime
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemCDTime*> &vecTar);
	//buff
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Buff*> &vecSrc1,
						std::vector <tagTWrap_Loong_Buff*> &vecSrc2,
						std::vector <tagTWrap_Loong_Buff*> &vecTar);

	//BlackList
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BlackList*> &vecSrc1,
						std::vector <tagTWrap_Loong_BlackList*> &vecSrc2,
						std::vector <tagTWrap_Loong_BlackList*> &vecTar);

	//enemy
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Enemy*> &vecSrc1,
						std::vector <tagTWrap_Loong_Enemy*> &vecSrc2,
						std::vector <tagTWrap_Loong_Enemy*> &vecTar);

	//equip ʵ�ʴ�����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
						std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
						std::vector <tagTWrap_Loong_Equip*> &vecTar,
						ETBEquipType emTBType);

	//equip
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
						std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
						std::vector <tagTWrap_Loong_Equip*> &vecTar);

	//EquipDel
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipDel*> &vecSrc1,
						std::vector <tagTWrap_Loong_EquipDel*> &vecSrc2,
						std::vector <tagTWrap_Loong_EquipDel*> &vecTar);

	//EquipBaiBao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_EquipBaiBao*> &vecTar);

	//����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ClanData*> &vecSrc1,
						std::vector <tagTWrap_Loong_ClanData*> &vecSrc2,
						std::vector <tagTWrap_Loong_ClanData*> &vecTar);

	//������
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc1,
						std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc2,
						std::vector <tagTWrap_Loong_ActClanTreasure*> &vecTar);
	
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc1,
						std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc2,
						std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecTar);

	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc1,
						std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc2,
						std::vector <tagTWrap_Loong_RepRstTime*> &vecTar);

	//friend
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Friend*> &vecSrc1,
						std::vector <tagTWrap_Loong_Friend*> &vecSrc2,
						std::vector <tagTWrap_Loong_Friend*> &vecTar);

	//friendship
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc1,
						std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc2,
						std::vector <tagTWrap_Loong_FriendshipSave*> &vecTar);

	//GroupPurchase
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc1,
						std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc2,
						std::vector <tagTWrap_Loong_GroupPurchase*> &vecTar);

	//GuildMem
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildMem*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildMem*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildMem*> &vecTar);

	//item_needlog
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemNeedLog*> &vecTar);

	//log_baibao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_LogBaiBao*> &vecTar);
	//RoleDel
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleDel*> &vecSrc1,
						std::vector <tagTWrap_Loong_RoleDel*> &vecSrc2,
						std::vector <tagTWrap_Loong_RoleDel*> &vecTar);

	//pet_data
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_PetData*> &vecSrc1,
						std::vector <tagTWrap_Loong_PetData*> &vecSrc2,
						std::vector <tagTWrap_Loong_PetData*> &vecTar);
	//pet skill
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_PetSkill*> &vecSrc1,
						std::vector <tagTWrap_Loong_PetSkill*> &vecSrc2,
						std::vector <tagTWrap_Loong_PetSkill*> &vecTar);
	//skill
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Skill*> &vecSrc1,
						std::vector <tagTWrap_Loong_Skill*> &vecSrc2,
						std::vector <tagTWrap_Loong_Skill*> &vecTar);
	//task
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_Task*> &vecSrc1,
						std::vector <tagTWrap_Loong_Task*> &vecSrc2,
						std::vector <tagTWrap_Loong_Task*> &vecTar);
	//taskdone
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_TaskDone*> &vecSrc1,
						std::vector <tagTWrap_Loong_TaskDone*> &vecSrc2,
						std::vector <tagTWrap_Loong_TaskDone*> &vecTar);

	//title
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Title*> &vecSrc1,
						std::vector <tagTWrap_Loong_Title*> &vecSrc2,
						std::vector <tagTWrap_Loong_Title*> &vecTar);

	//vip_stall
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_VipStall*> &vecSrc1,
						std::vector <tagTWrap_Loong_VipStall*> &vecSrc2,
						std::vector <tagTWrap_Loong_VipStall*> &vecTar);

	//VisitingCard
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc1,
						std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc2,
						std::vector <tagTWrap_Loong_VisitingCard*> &vecTar);

	//ybaccount
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_YBAccount*> &vecSrc1,
						std::vector <tagTWrap_Loong_YBAccount*> &vecSrc2,
						std::vector <tagTWrap_Loong_YBAccount*> &vecTar);

	//yuanbaoorder
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc1,
						std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc2,
						std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecTar);

	//bill item
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BillItem*> &vecSrc1,
						std::vector <tagTWrap_Loong_BillItem*> &vecSrc2,
						std::vector <tagTWrap_Loong_BillItem*> &vecTar);
	//bill yuanbao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_BillYuanBao*> &vecTar);
	//bill yuanbao log
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc1,
						std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc2,
						std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecTar);
	//guild_commodity
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildCommodity*> &vecTar);
	//guild_skill
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildSkill*> &vecTar);
	//vip_netbar
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc1,
						std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc2,
						std::vector <tagTWrap_Loong_VipNetBar*> &vecTar);
	//guild_upgrade
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildFacilities*> &vecTar);
	//commerce_rank
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc1,
						std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc2,
						std::vector <tagTWrap_Loong_CommerceRank*> &vecTar);
	//activity
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ActivityData*> &vecSrc1,
						std::vector <tagTWrap_Loong_ActivityData*> &vecSrc2,
						std::vector <tagTWrap_Loong_ActivityData*> &vecTar);
	//left msg
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc1,
						std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc2,
						std::vector <tagTWrap_Loong_LeftMsg*> &vecTar);

	//�ⲿ����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ExternalLink*> &vecSrc1,
						std::vector <tagTWrap_Loong_ExternalLink*> &vecSrc2,
						std::vector <tagTWrap_Loong_ExternalLink*> &vecTar);

	//name history
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_NameHistory*> &vecSrc1,
						std::vector <tagTWrap_Loong_NameHistory*> &vecSrc2,
						std::vector <tagTWrap_Loong_NameHistory*> &vecTar);

	//city
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_City*> &vecSrc1,
						std::vector <tagTWrap_Loong_City*> &vecSrc2,
						std::vector <tagTWrap_Loong_City*> &vecTar);

	//Master
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Master*> &vecSrc1,
						std::vector <tagTWrap_Loong_Master*> &vecSrc2,
						std::vector <tagTWrap_Loong_Master*> &vecTar);

	//short cut
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ShortCutBar*> &vecSrc1,
						std::vector <tagTWrap_Loong_ShortCutBar*> &vecSrc2,
						std::vector <tagTWrap_Loong_ShortCutBar*> &vecTar);

	//����״̬
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_TrainState*> &vecSrc1,
						std::vector <tagTWrap_Loong_TrainState*> &vecSrc2,
						std::vector <tagTWrap_Loong_TrainState*> &vecTar);

	//ActiveRankData
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ActiveRankData*> &vecSrc1,
						std::vector <tagTWrap_Loong_ActiveRankData*> &vecSrc2,
						std::vector <tagTWrap_Loong_ActiveRankData*> &vecTar);

	//fabao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Fabao*> &vecSrc1,
						std::vector <tagTWrap_Loong_Fabao*> &vecSrc2,
						std::vector <tagTWrap_Loong_Fabao*> &vecTar);

	// F-Plan 1.5.0 SoulCrystal
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_SoulCrystal*> &vecSrc1,
		std::vector <tagTWrap_Loong_SoulCrystal*> &vecSrc2,
		std::vector <tagTWrap_Loong_SoulCrystal*> &vecTar);

	// F-Plan 1.5.0 account_reactive, account_reactiver
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountReactive*> &vecSrc1,
		std::vector <tagTWrap_Loong_AccountReactive*> &vecSrc2,
		std::vector <tagTWrap_Loong_AccountReactive*> &vecTar);

	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountReactiver*> &vecSrc1,
		std::vector <tagTWrap_Loong_AccountReactiver*> &vecSrc2,
		std::vector <tagTWrap_Loong_AccountReactiver*> &vecTar);

	//ForceBreakOutApprentice
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecSrc1,
						std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecSrc2,
						std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecTar);

	//�����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecSrc1,
						std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecSrc2,
						std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecTar);
	
	//������ս
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecSrc1,
						std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecSrc2,
						std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecTar);

	//��ɫ����ʱ��ͳ��
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecSrc1,
						std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecSrc2,
						std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecTar);

	//������������
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecSrc1,
						std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecSrc2,
						std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecTar);

	//��֮����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_GodBid*> &vecSrc1,
						std::vector <tagTWrap_Sheng_GodBid*> &vecSrc2,
						std::vector <tagTWrap_Sheng_GodBid*> &vecTar);

	//�ҵ�
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_EggInfo*> &vecSrc1,
						std::vector <tagTWrap_Sheng_EggInfo*> &vecSrc2,
						std::vector <tagTWrap_Sheng_EggInfo*> &vecTar);

	//�³�
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_Wardrobe*> &vecSrc1,
						std::vector <tagTWrap_Sheng_Wardrobe*> &vecSrc2,
						std::vector <tagTWrap_Sheng_Wardrobe*> &vecTar);

	//����
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_Area*> &vecSrc1,
		std::vector <tagTWrap_Sheng_Area*> &vecSrc2,
		std::vector <tagTWrap_Sheng_Area*> &vecTar);

	//ϡ������
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_RareArea*> &vecSrc1,
		std::vector <tagTWrap_Sheng_RareArea*> &vecSrc2,
		std::vector <tagTWrap_Sheng_RareArea*> &vecTar);

	//Family
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_Family*> &vecSrc1,
												 std::vector <tagTWrap_Loong_Family*> &vecSrc2,
												 std::vector <tagTWrap_Loong_Family*> &vecTar);
	//FamilyMem
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyMem*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilyMem*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilyMem*> &vecTar);
	//FamilyQuset
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyQuset*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilyQuset*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilyQuset*> &vecTar);
	//FamilyRoleSprite
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecTar);
	//FamilySprite
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilySprite*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilySprite*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilySprite*> &vecTar);
	//SpriteRequirement
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecSrc1,
												 std::vector <tagTWrap_Loong_SpriteRequirement*> &vecSrc2,
												 std::vector <tagTWrap_Loong_SpriteRequirement*> &vecTar);

	//GmServerData
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_GmServerData*> &vecSrc1,
												 std::vector <tagTWrap_Loong_GmServerData*> &vecSrc2,
												 std::vector <tagTWrap_Loong_GmServerData*> &vecTar);


	//UseKeyCode_roleID
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_UseKeyCode*> &vecSrc1,
		std::vector <tagTWrap_Loong_UseKeyCode*> &vecSrc2,
		std::vector <tagTWrap_Loong_UseKeyCode*> &vecTar);

	// F-Plan 1.6.0 holy
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Holy*> &vecSrc1,
		std::vector <tagTWrap_Loong_Holy*> &vecSrc2,
		std::vector <tagTWrap_Loong_Holy*> &vecTar);

	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_HolyEquip*> &vecSrc1,
		std::vector <tagTWrap_Loong_HolyEquip*> &vecSrc2,
		std::vector <tagTWrap_Loong_HolyEquip*> &vecTar);


public:
	//�ɵ������м����� ���³�ʼ������
	VOID ClearMemory();

private:
	
	// �ű������������
	VOID UpdateSpecialActivityData(DWORD dwActID, tagActivityData& stAData);

	// �ƶ������
	VOID MoveActivityData(DWORD dwActID, std::vector<tagTWrap_Loong_ActivityData*>::iterator itSrc,
		std::vector<tagTWrap_Loong_ActivityData*>::iterator itTar, INT nValidPos, BOOL bSameContainer);

	//���˻����
	VOID FilterActivityData(std::vector <tagTWrap_Loong_ActivityData*> &vecData);

	//����roleid ��Դ2ʹ��
	VOID Update_Loong_RoleID(DWORD &dwRoleID);

	//������Ʒ64λid
	VOID Update_Loong_Item64SerialNum(INT64 &n64Serial);

	//ת����Сд���crc32
	DWORD LowerCrc32(LPCTSTR str,DWORD strLen);

	//������ݹ���
	VOID Loong_RoleDataFilter(std::vector<tagTWrap_Loong_RoleData*> & vecTar);

	//����roledata��
	ETBRoleRemoveType Loong_RoleDataDoHouseKeeping(std::vector <tagTWrap_Loong_RoleData*>::iterator itTar,
												tagDWORDTime dwCurTm,
												BOOL bFirstRole);

	//����role�Ƿ�Ϊ������
	BOOL IsDirtyRoleData(DWORD dwRoleID);

public:
	std::vector <tagRoleCompensation> m_vecRoleCompensation; //��Ҳ���

	std::set <string> m_setCAName; //������ֵ�����˺�

	std::set <tstring> m_setSrc1RoleName;		//Դ1�Ľ�ɫ����

	std::set<DWORD> m_setSrc1RoleNameCrc;		//Դ1��ɫ���ֵ�crc

	std::set<DWORD> m_setSrc1ClergyRoleID;		//Դ1��ְ��ɫID
	std::set<DWORD> m_setSrc2ClergyRoleID;		//Դ2��ְ��ɫID

//�Ϸ��ö��ձ�
private:
	//std::map<tagItemNumKey ,INT64> m_mpOldItemNumvsNewItemNum;//�¾�item��Ŷ��ձ�

	std::map <tagAccountIDKey,DWORD> m_mpOldAccountVsNewAccount; //������ϲ���Ҫ �ɵ�accountid���µ�accountid�Ķ��ձ�

	std::map <tagGuildIDKey,DWORD> m_mpOldGuildIDvsNewGuildID; //����id

	std::map <tagFamilyIDKey,DWORD> m_mpOldFamilyIDvsNewFamilyID; //����id(Ŀǰ�Ȳ��������ظ�crc�����)

	std::map <DWORD ,DWORD> m_mpAccountIDvsWareSize;//�µ�accountid�Ͳֿ�size�Ķ��ձ�

	stdext::hash_map <DWORD ,ETBRoleRemoveType> m_mpRoleIDvsRemoveType; //�µ�roleid����remove���͵Ķ��ձ�

	std::set <DWORD> m_setOldGuildID; //�����ϵİ���id

	std::map <DWORD ,DWORD> m_mpAccountIDvsRoleIDforCB; //���ڴ����Ϸ�ʱ��ɾ����ɫ����ɫ�ֿ��а���Ʒ �İ�(change bind)ʹ��

	//m_vecSrc1AccountID��m_mpSrc1VsSrc2��m_vecSrc2OnlyAccountID ����������account��ת���Ķ��ձ�
	std::vector <DWORD> m_vecOldSrc1AccountID; //Դ1���û����ϡ� ��id

	std::map <DWORD ,DWORD> m_mpOldSrc1VsSrc2; //Դ1�У�Դ2Ҳ�е��û�����id
											//key:����Դ1��AccountID��value:������Դ1��Ӧ��ͬһ�û�,��Դ2�е�AccountID��
	std::map <DWORD ,DWORD> m_mpOldSrc2VsSrc1; 	//key:����Դ2��AccountID��value:������Դ2��Ӧ��ͬһ�û�,��Դ1�е�AccountID��
			
	std::vector <DWORD> m_vecOldSrc2OnlyAccountID; //Դ2���е��û����ϡ� ��id

	std::set <DWORD> m_setChangeNameRoleID; //������ɫ ��roleid ����º��

//�Ϸ����м�����
private:

	std::vector <tagTWrap_Loong_Item> m_vecOverItem; //������Ʒ ����д��item_baibao

	std::set <INT64> m_setOverEquipSerialNum;	//�������������

	std::vector <tagTWrap_Loong_Equip> m_vecOverEquip; //����װ�� ����д��equip_baibao

	std::set <DWORD> m_setCAccountID; //������ֵ�����˺�id

	std::set <DWORD> m_mapGuildLeader; //���а��� �µ�roleid

	stdext::hash_set <INT64> m_setDirtyItem;	//�����Ʒ

	stdext::hash_set <INT64> m_setItemDelEquipDelSurvive;	//�ɾ���itemdel equipdel
private:

	INT		m_nItemDelEquipDelSurviveDay; //������������Ʒɾ�����ݺ�װ��ɾ������

	BOOL	m_bDataClean; //�Ƿ������������

	EDBMergeType m_eMergeType;

	tstring		m_strSrc2WorldName; //Դ2������

	INT64		m_n64Src1ItemMaxSerialInternal;		//Դ1 �ڲ�������Ʒ		����Ŵ�500000000000���ϼ� ������500000000000
	INT64		m_n64Src1ItemMinSerialOutter;		//Դ1 �ⲿ������Ʒ		����Ŵ�400000000000���¼� ����400000000000
	INT64		m_n64Src1ItemMinSerialBill;			//Դ1 ֱ��������Ʒ��С���	����Ŵ�300000000000���¼� ������300000000000

	INT64		m_n64ItemCurSerialInternal;			//��ǰ�ڲ�������Ʒ		����Ŵ�500000000000���ϼ� ������500000000000
	INT64		m_n64ItemCurSerialOutter;			//��ǰ�ⲿ������Ʒ		����Ŵ�400000000000���¼� ����400000000000
	INT64		m_n64ItemCurSerialBill;				//��ǰֱ��������Ʒ��С���	����Ŵ�300000000000���¼� ������300000000000

	DWORD		m_dwSrc1LoginMaxNum;				//src1 login account�� id�����

	DWORD		m_dwSrc1RoleMaxNum;					//Դ1�������� ����roleid׷��

	DWORD		m_dwSrc1PetMaxNum;					//Դ1 �������petid			����Ŵ�3000000001���ϼӡ�������3000000001

//	DWORD		m_dwSrc1VipStallMaxNum;				//Դ1 vip̯λ�����

	DWORD		m_dwSrc1WorldNameCrc;				//Դ1 ��Ϸ��������crc

	CRITICAL_SECTION		m_ContainerLock;					//���̺߳ϲ�ʱ��������ʱ��Ҫ����

private:
	vector<tagFamilyNeedChangeName*>	m_vecNeedChangeFamilyName;

public:
	vector<tagFamilyNeedChangeName*>* GetNeedChangNameFamily(){ return &m_vecNeedChangeFamilyName; }

	VOID  Update_Loong_FamilyNameCRC(DWORD &dwFamilyID);
};


extern string Loong_CheckAccountName(CHAR szAccountName[X_SHORT_NAME]);

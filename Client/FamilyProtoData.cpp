#include "StdAfx.h"
#include "FamilyProtoData.h"
#include "..\WorldDefine\family_define.h"

FamilyProtoData::FamilyProtoData(void)
{
}

FamilyProtoData::~FamilyProtoData(void)
{
}

FamilyProtoData m_familyProto;
FamilyProtoData* FamilyProtoData::Inst()
{
	return &m_familyProto;
}

VOID FamilyProtoData::LoadFromFile()
{
	LPCTSTR szFamilySpriteProto = _T("data\\system\\attdata\\sprite_proto.xml");
	LPCTSTR szSpriteTrainProto = _T("data\\system\\attdata\\sprite_training_proto.xml");
	tstring strPath = g_strLocalPath + _T("\\attdata\\xxx.xml");
	LPCTSTR FamilySpriteName = strPath.c_str(); 
	
	LPCTSTR szFamilyQuestProto = _T("data\\system\\attdata\\FamilyQuest_Proto.xml");
	tstring strPath1 = g_strLocalPath + _T("\\attdata\\FamilyQuest_Name.xml");
	LPCTSTR szFamilyQuestName = strPath1.c_str();
	LPCTSTR szFamilyQuestAttProto = _T("data\\system\\attdata\\FamilyQuest_Att_Proto.xml");
	LPCTSTR szFamilyQuestRewardProto = _T("data\\system\\attdata\\FamilyQuest_Reward_Proto.xml");

	list<tstring> FamilySpriteProtoFieldList;
	list<tstring> FamilySpriteNameFieldList;
	list<tstring> FamilyTrainFieldList;
	list<tstring>::iterator iter;

	TCHAR szTemp[X_LONG_NAME] = {0};

	//////////////////////////////////////////////////////////////////////////
	// ��������
	//////////////////////////////////////////////////////////////////////////
	TObjRef<VarContainer> VarClergyProto = CreateObj("familySpriteProto", "VarContainer");
	if(!VarClergyProto->Load("VFS_System", szFamilySpriteProto, "id", &FamilySpriteProtoFieldList))
		IMSG(_T("Load file familySpriteProto.xml failed\r\n"));

	for(iter = FamilySpriteProtoFieldList.begin(); iter != FamilySpriteProtoFieldList.end(); ++iter)
	{
		tagFamilySpriteProto pFamilySpriteProto;
		//pFamilySpriteProto.nID								= (DWORD)VarClergyProto->GetInt(_T("id"), (*iter).c_str(), 0);								//�����ȼ�	��Ϊid���ڲ���
		////pFamilySpriteProto->nLevel						= m_pVar->GetInt(_T("level"), (*iter).c_str(), 0);									// �ȼ�
		//pFamilySpriteProto.dwExp							= (DWORD)VarClergyProto->GetDword(_T("exp"), (*iter).c_str(), 0);							// �ɳ���
		//pFamilySpriteProto.dwHP							= (DWORD)VarClergyProto->GetDword(_T("hp"), (*iter).c_str(), 0);							// ��������
		//pFamilySpriteProto.dwTrainNum						= (DWORD)VarClergyProto->GetDword(_T("train_num"), (*iter).c_str(), 0);						// �������󷢲�����
		//pFamilySpriteProto.dwExAttack						= (DWORD)VarClergyProto->GetDword(_T("ex_attack"), (*iter).c_str(), 0);						// ��������������
		//pFamilySpriteProto.dwInAttack						= (DWORD)VarClergyProto->GetDword(_T("in_attack"), (*iter).c_str(), 0);						// ����������������
		//pFamilySpriteProto.dwExDefense						= (DWORD)VarClergyProto->GetDword(_T("ex_defense"), (*iter).c_str(), 0);					// ���������������
		//pFamilySpriteProto.dwInDefense						= (DWORD)VarClergyProto->GetDword(_T("in_defense"), (*iter).c_str(), 0);					// ����������������
		//pFamilySpriteProto.dwExAttackAdd					= (DWORD)VarClergyProto->GetDword(_T("ex_attack_add"), (*iter).c_str(), 0);					// �����˺���������
		//pFamilySpriteProto.dwInAttackAdd					= (DWORD)VarClergyProto->GetDword(_T("in_attack_add"), (*iter).c_str(), 0);					// �����˺���������
		//pFamilySpriteProto.dwExDefenseDes					= (DWORD)VarClergyProto->GetDword(_T("ex_defense_des"), (*iter).c_str(), 0);				// ��������������
		//pFamilySpriteProto.dwInDefenseDes					= (DWORD)VarClergyProto->GetDword(_T("in_defense_des"), (*iter).c_str(), 0);				// ���������������
		//pFamilySpriteProto.dwToughness						= (DWORD)VarClergyProto->GetDword(_T("toughness"), (*iter).c_str(), 0);						// ��������
		//pFamilySpriteProto.dwCritDes						= (DWORD)VarClergyProto->GetDword(_T("crit_des"), (*iter).c_str(), 0);						// ������������
		//pFamilySpriteProto.dwControleffectDeepen			= (DWORD)VarClergyProto->GetDword(_T("control_effect_deepen"), (*iter).c_str(), 0);			// ����Ч����������
		//pFamilySpriteProto.dwControleffectResistance		= (DWORD)VarClergyProto->GetDword(_T("control_effect_resistance"), (*iter).c_str(), 0);		// ���Ƶֿ���������
		//pFamilySpriteProto.dwSlowingeffectDeepen			= (DWORD)VarClergyProto->GetDword(_T("slowing_effect_deepen"), (*iter).c_str(), 0);			// ����Ч����������
		//pFamilySpriteProto.dwSlowingeffectResistance		= (DWORD)VarClergyProto->GetDword(_T("slowing_effect_resistance"), (*iter).c_str(), 0);		// ���ٵֿ���������
		//pFamilySpriteProto.dwFixedeffectDeepen				= (DWORD)VarClergyProto->GetDword(_T("fixed_effect_deepen"), (*iter).c_str(), 0);			// �̶�Ч����������
		//pFamilySpriteProto.dwFixedeffectResistance			= (DWORD)VarClergyProto->GetDword(_T("fixed_effect_resistance"), (*iter).c_str(), 0);		// �̶��ֿ���������
		//pFamilySpriteProto.dwAgingeffectDeepen				= (DWORD)VarClergyProto->GetDword(_T("aging_effect_deepen"), (*iter).c_str(), 0);			// ˥��Ч����������
		//pFamilySpriteProto.dwAgingeffectResistance			= (DWORD)VarClergyProto->GetDword(_T("aging_effect_resistance"), (*iter).c_str(), 0);		// ˥���ֿ���������

		pFamilySpriteProto.nID													= (DWORD)VarClergyProto->GetInt(_T("id"), (*iter).c_str(), 0);								//�����ȼ�	��Ϊid���ڲ���
		//pFamilySpriteProto->nLevel											= m_pVar->GetInt(_T("level"), szField, 0);									// �ȼ�
		pFamilySpriteProto.dwExp												= (DWORD)VarClergyProto->GetDword(_T("exp"), (*iter).c_str(), 0);							// �ɳ���
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_HP]							= (DWORD)VarClergyProto->GetDword(_T("hp"), (*iter).c_str(), 0);							// ��������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_EXAttack]					= (DWORD)VarClergyProto->GetDword(_T("ex_attack"), (*iter).c_str(), 0);						// ��������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_InAttack]					= (DWORD)VarClergyProto->GetDword(_T("in_attack"), (*iter).c_str(), 0);						// ����������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_EXDefense]					= (DWORD)VarClergyProto->GetDword(_T("ex_defense"), (*iter).c_str(), 0);					// ���������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_InDefense]					= (DWORD)VarClergyProto->GetDword(_T("in_defense"), (*iter).c_str(), 0);					// ����������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_EXAttackDeeper]				= (DWORD)VarClergyProto->GetDword(_T("ex_attack_add"), (*iter).c_str(), 0);					// �����˺���������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_InAttackDeeper]				= (DWORD)VarClergyProto->GetDword(_T("in_attack_add"), (*iter).c_str(), 0);					// �����˺���������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_EXAttackResistance]			= (DWORD)VarClergyProto->GetDword(_T("ex_defense_des"), (*iter).c_str(), 0);				// ��������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_InAttackResistance]			= (DWORD)VarClergyProto->GetDword(_T("in_defense_des"), (*iter).c_str(), 0);				// ���������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_Toughness]					= (DWORD)VarClergyProto->GetDword(_T("toughness"), (*iter).c_str(), 0);						// ��������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_CritDes]					= (DWORD)VarClergyProto->GetDword(_T("crit_des"), (*iter).c_str(), 0);						// ������������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_ControleffectDeepen]		= (DWORD)VarClergyProto->GetDword(_T("control_effect_deepen"), (*iter).c_str(), 0);			// ����Ч����������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_ControleffectResistance]	= (DWORD)VarClergyProto->GetDword(_T("control_effect_resistance"), (*iter).c_str(), 0);		// ���Ƶֿ���������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_SlowingeffectDeepen]		= (DWORD)VarClergyProto->GetDword(_T("slowing_effect_deepen"), (*iter).c_str(), 0);			// ����Ч����������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_SlowingeffectResistance]	= (DWORD)VarClergyProto->GetDword(_T("slowing_effect_resistance"), (*iter).c_str(), 0);		// ���ٵֿ���������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_FixedeffectDeepen]			= (DWORD)VarClergyProto->GetDword(_T("fixed_effect_deepen"), (*iter).c_str(), 0);			// �̶�Ч����������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_FixedeffectResistance]		= (DWORD)VarClergyProto->GetDword(_T("fixed_effect_resistance"), (*iter).c_str(), 0);		// �̶��ֿ���������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_AgingeffectDeepen]			= (DWORD)VarClergyProto->GetDword(_T("aging_effect_deepen"), (*iter).c_str(), 0);			// ˥��Ч����������
		pFamilySpriteProto.dwAbilityUpperLimit[EFSA_AgingeffectResistance]		= (DWORD)VarClergyProto->GetDword(_T("aging_effect_resistance"), (*iter).c_str(), 0);		// ˥���ֿ���������
		pFamilySpriteProto.dwPickTypeNum[0]										= (DWORD)VarClergyProto->GetDword(_T("pick_type_1"), (*iter).c_str(), 0);					// ��ȡ����1����
		pFamilySpriteProto.dwPickTypeNum[1]										= (DWORD)VarClergyProto->GetDword(_T("pick_type_2"), (*iter).c_str(), 0);					// ��ȡ����2����

		m_mapFamilySpriteProto.insert(make_pair(pFamilySpriteProto.nID, pFamilySpriteProto));
	}

	KillObj("familySpriteProto");

	//////////////////////////////////////////////////////////////////////////
	// ��������
	//////////////////////////////////////////////////////////////////////////
	TObjRef<VarContainer> VarTrainProto = CreateObj("SpriteTrainProto", "VarContainer");
	if(!VarTrainProto->Load("VFS_System", szSpriteTrainProto, "id", &FamilyTrainFieldList))
		IMSG(_T("Load file SpriteTrainProto.xml failed\r\n"));

	for(iter = FamilyTrainFieldList.begin(); iter != FamilyTrainFieldList.end(); ++iter)
	{
		tagSpriteTrainClientProto pSpriteTrainProto;
	
		pSpriteTrainProto.nID						= (DWORD)VarTrainProto->GetInt(_T("id"), (*iter).c_str(), 0);										
		pSpriteTrainProto.nPageType				= (DWORD)VarTrainProto->GetInt(_T("page_type"), (*iter).c_str(), 0);				// ��ҳ����
		pSpriteTrainProto.nLevelLimit				= (DWORD)VarTrainProto->GetInt(_T("level_limit"), (*iter).c_str(), 0);				// �ȼ�����
		pSpriteTrainProto.dwAbilityType			= (DWORD)VarTrainProto->GetDword(_T("ability_type"), (*iter).c_str(), 0);			// ��������
		pSpriteTrainProto.dwPowerUpLimit			= (DWORD)VarTrainProto->GetDword(_T("power_up_limit"), (*iter).c_str(), 0);		// ������������
		pSpriteTrainProto.dwPowerLowLimit			= (DWORD)VarTrainProto->GetDword(_T("power_low_limit"), (*iter).c_str(), 0);		// ������������
		pSpriteTrainProto.dwItemID					= (DWORD)VarTrainProto->GetDword(_T("itemid"), (*iter).c_str(), 0);				// ������Ʒ
		pSpriteTrainProto.dwItemNum				= (DWORD)VarTrainProto->GetDword(_T("item_num"), (*iter).c_str(), 0);				// ������Ʒ����
		pSpriteTrainProto.dwItemQuality			= (DWORD)VarTrainProto->GetDword(_T("item_quality"), (*iter).c_str(), 0);			// ������ƷƷ��(��װ����ʱ��Ŷ�)
		pSpriteTrainProto.dwShowUpLimit			= (DWORD)VarTrainProto->GetDword(_T("show_up_limit"), (*iter).c_str(), 0);			// ���ֵȼ�����
		pSpriteTrainProto.dwShowLowLimit			= (DWORD)VarTrainProto->GetDword(_T("show_low_limit"), (*iter).c_str(), 0);		// ���ֵȼ�����
		pSpriteTrainProto.dwPickType				= (DWORD)VarTrainProto->GetDword(_T("pick_type"), (*iter).c_str(), 0);				// ��ȡ����
		pSpriteTrainProto.dwSpriteExpReword		= (DWORD)VarTrainProto->GetDword(_T("sprite_exp_reword"), (*iter).c_str(), 0);		// �����ɳ��Ƚ���
		pSpriteTrainProto.dwExpReword				= (DWORD)VarTrainProto->GetDword(_T("exp_reword"), (*iter).c_str(), 0);			// ���齱��
		pSpriteTrainProto.dwItemReword				= (DWORD)VarTrainProto->GetDword(_T("item_reword"), (*iter).c_str(), 0);			// ��Ʒ����
		pSpriteTrainProto.dwItemRewordNum			= (DWORD)VarTrainProto->GetDword(_T("item_reword_num"), (*iter).c_str(), 0);		// ��Ʒ��������
		pSpriteTrainProto.dwItemRewordQuality		= (DWORD)VarTrainProto->GetDword(_T("item_reword_quality"), (*iter).c_str(), 0);	// ��Ʒ����Ʒ��(��װ����ʱ��Ŷ�)


		m_mapFamilySpriteTrain.insert(make_pair(pSpriteTrainProto.nID, pSpriteTrainProto));
	}

	KillObj("SpriteTrainProto");

	list<tstring> FamilyQuestList;
	list<tstring> FamilyQuestAttList;
	list<tstring> FamilyQuestRewardList;
	list<tstring> FamilyQuestNameList;
	///////////////////////////////////////////////////////////////////////////
	// ���������
	///////////////////////////////////////////////////////////////////////////
	TObjRef<VarContainer> VarFamilyQuestProto = CreateObj("FamilyQuestProto", "VarContainer");
	if(!VarFamilyQuestProto->Load("VFS_System", szFamilyQuestProto, "id", &FamilyQuestList))
		IMSG(_T("Load file FamilyQuestProto.xml failed\r\n"));

	for(iter = FamilyQuestList.begin(); iter != FamilyQuestList.end(); ++iter)
	{
		tagFamilyQuestName data;

		data.dwID				= VarFamilyQuestProto->GetDword( _T("id"),								(*iter).c_str(), 0);
		data.eType				= (EFamilyQuestType)VarFamilyQuestProto->GetDword(_T("type"),			(*iter).c_str(), 0);
		data.nHighLevel			= VarFamilyQuestProto->GetDword(_T("highLevel"),						(*iter).c_str(), 0);
		data.nLowLevel			= VarFamilyQuestProto->GetDword(_T("lowLevel"),							(*iter).c_str(), 0);
		data.nQuestNum			= VarFamilyQuestProto->GetDword(_T("num"),								(*iter).c_str(), 0);
		data.dwConditionID		= VarFamilyQuestProto->GetDword(_T("questid"),							(*iter).c_str(), 0);
		m_mapFamilyQuestName.insert(make_pair(data.dwID, data));
	}

	KillObj("FamilyQuestProto");
	///////////////////////////////////////////////////////////////////////////
	// ���ϱ�
	///////////////////////////////////////////////////////////////////////////
	TObjRef<VarContainer> VarFamilyQuestAttProto = CreateObj("FamilyQuestAttProto", "VarContainer");
	if(!VarFamilyQuestAttProto->Load("VFS_System", szFamilyQuestAttProto, "id", &FamilyQuestAttList))
		IMSG(_T("Load file FamilyQuestAttProto.xml failed\r\n"));

	for(iter = FamilyQuestAttList.begin(); iter != FamilyQuestAttList.end(); ++iter)
	{
		tagFamilyQuestAttProto data;

		data.dwID				= VarFamilyQuestAttProto->GetDword(_T("id"), (*iter).c_str(), 0);
		for(int i = 0 ; i < MAX_ItemOrMonsterID; i++)
		{
			_stprintf(szTemp, _T("id%d"), i);
			data.dwItemOrMonsterID[i]	= VarFamilyQuestAttProto->GetDword(szTemp, (*iter).c_str(), 0);
		}
		data.dwMapcrc			= VarFamilyQuestAttProto->GetDword(_T("mapCrc"),								(*iter).c_str(), 0);
		data.eFinishType		= (EFamilyQuestFinishType)VarFamilyQuestAttProto->GetDword(_T("questType"),		(*iter).c_str(), 0);
		data.eJudgeType			= (EJudgeType)VarFamilyQuestAttProto->GetDword(_T("monsterType"),				(*iter).c_str(), 0);
		data.nMonsterRank		= VarFamilyQuestAttProto->GetDword(_T("monsterLevel"),					(*iter).c_str(), 0);
		data.nHighLevel			= VarFamilyQuestAttProto->GetDword(_T("highLevel"),								(*iter).c_str(), 0);
		data.nLowLevel			= VarFamilyQuestAttProto->GetDword(_T("lowLevel"),								(*iter).c_str(), 0);
		m_mapFamilyQuestAttProto.insert(make_pair(data.dwID, data));
	}

	KillObj("FamilyQuestAttProto");

	TObjRef<VarContainer> VarFamilyQuestName = CreateObj( "FamilyQuestName", "VarContainer" );
	if(!VarFamilyQuestName->Load("VFS_System", szFamilyQuestName, "id", &FamilyQuestNameList))
		IMSG(_T("Load file compose_proto.xml failed\r\n"));
	for(iter=FamilyQuestNameList.begin(); iter!=FamilyQuestNameList.end(); iter++)
	{
		DWORD dwID = VarFamilyQuestName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagFamilyQuestName* data = FindQuestName(dwID);
		if( P_VALID(data) )
		{
			_tcscpy( data->szDesc, VarFamilyQuestName->GetString(_T("desc"), (*iter).c_str(), _T("")) );
		}
	}

	KillObj( "FamilyQuestName" );

	///////////////////////////////////////////////////////////////////////////
	// ������
	///////////////////////////////////////////////////////////////////////////
	TObjRef<VarContainer> VarFamilyQuestRewardProto = CreateObj("FamilyQuestRewardProto", "VarContainer");
	if(!VarFamilyQuestRewardProto->Load("VFS_System", szFamilyQuestRewardProto, "id", &FamilyQuestRewardList))
		IMSG(_T("Load file FamilyQuestRewardProto.xml failed\r\n"));

	for(iter = FamilyQuestRewardList.begin(); iter != FamilyQuestRewardList.end(); ++iter)
	{
		tagFamilyQuestRewardProto data;

		data.dwID = VarFamilyQuestRewardProto->GetDword(_T("id"), (*iter).c_str(), 0);
		data.dwExp = VarFamilyQuestRewardProto->GetDword(_T("exp"), (*iter).c_str(), 0);
		for(int i = 0; i < MAX_FQ_RewardNum; i++)
		{
			_stprintf(szTemp, _T("item%d"), i);
			data.dwItemID[i]		= VarFamilyQuestRewardProto->GetDword(szTemp, (*iter).c_str(), 0);
			_stprintf(szTemp, _T("itemEx%d"), i);
			data.dwItemIDEx[i]		= VarFamilyQuestRewardProto->GetDword(szTemp, (*iter).c_str(), 0);
			_stprintf(szTemp, _T("num%d"), i);
			data.dwItemNum[i]		= VarFamilyQuestRewardProto->GetDword(szTemp, (*iter).c_str(), 0);
			_stprintf(szTemp, _T("numEx%d"), i);
 			data.dwItemNumEx[i]		= VarFamilyQuestRewardProto->GetDword(szTemp, (*iter).c_str(), 0);
		}

		data.dwMoney = VarFamilyQuestRewardProto->GetDword(_T("money"), (*iter).c_str(), 0);
		m_mapFamilyQuestRewardProto.insert(make_pair(data.dwID, data));
	}

	KillObj("FamilyQuestRewardProto");
}

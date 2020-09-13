#include "StdAfx.h"
#include "wx/wx.h"
#include "Resource.h"
#include "Loong_ServerMerge.h"
#include "Loong_LoginDBCtrl.h"
#include "Loong_LoongDBCtrl.h"
#include "Loong_OutputCtrl.h"
#include "Loong_TMethod.h"


CLoongServerMerge::CLoongServerMerge(void)
{
	m_bDataClean = FALSE;
	m_eMergeType = EM_MERGE_NULL;
	m_strSrc2WorldName = _T("");

	//������������Ʒɾ�����ݺ�װ��ɾ������
	m_nItemDelEquipDelSurviveDay = 30;

}

CLoongServerMerge::~CLoongServerMerge(void)
{	
	//�رվ��
	std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
	for(;itH != m_listSimpleThreadPool.end(); ++itH)
	{
		if(*itH != NULL)
		{
			TerminateThread(*itH,0);
			CloseHandle(*itH);
			*itH = 0;
		}
	}

	SAFE_CLOSE_HANDLE(m_hPoolMergeEvent);

	//�������ݿ�
	sLoginBeton.Destroy();
	sLoongBeton.Destroy();

	m_clsThreadTaskP.Clear();
}
/*
/	��ʼ��DB
/
*/
BOOL CLoongServerMerge::Init(tagConfigDetail	stConfigOne[EM_DB_TYPE_END],
							 tagConfigDetail	stConfigTwo[EM_DB_TYPE_END],
							 EDBMergeType		eMergeType,
							 BOOL				bDataClean,
							 tstring			strSrc2WorldName,
							 INT				nSurviveDay,
							 const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation)
{
	BOOL bRet = FALSE;

	//����ǿ���ϲ�
	if(eMergeType == EM_MERGE_CROSS_SECTION)
	{
		//��ʼ��Login
		bRet = sLoginBeton.Init(stConfigOne);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	//��ʼ��Loong
	bRet = sLoongBeton.Init(stConfigTwo);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	//�ϲ�����
	m_bDataClean = bDataClean;
	m_eMergeType = eMergeType;
	m_strSrc2WorldName = strSrc2WorldName;

	//������������Ʒɾ�����ݺ�װ��ɾ������
	m_nItemDelEquipDelSurviveDay = nSurviveDay;

	//�̳߳غϲ�ͬ��
	m_hPoolMergeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//�رվ��
	std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
	for(;itH != m_listSimpleThreadPool.end(); ++itH)
	{
		if(*itH != NULL)
		{
			TerminateThread(*itH,0);
			CloseHandle(*itH);
			*itH = 0;
		}
	}

	for(INT32 nLoop = 0; nLoop<THREAD_POOL_NUM; nLoop++)
	{
		HANDLE thread_work_handle_ = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&MyThreadPool, (LPVOID)this, 0, NULL);
		m_listSimpleThreadPool.push_back(thread_work_handle_);
	}

	//��Ҳ�����Ϣ
	m_mmCompensation.insert(mmCompensation.begin(),mmCompensation.end());

	return TRUE;
}
/*
/	��ʼ���ϲ��߼�
*/
BOOL CLoongServerMerge::LoongMergeLogicCtrlInit(CLoongMergeLogicCtrl &clsMergeLogicCtrl)
{	
	//�ϲ�����
	clsMergeLogicCtrl.SetMergeType(m_eMergeType);

	//�Ƿ������������
	clsMergeLogicCtrl.SetIfCleanData(m_bDataClean);

	//Դ2����Ϸ������
	clsMergeLogicCtrl.SetSrc2WorldName(m_strSrc2WorldName);

	//����ʱ��
	clsMergeLogicCtrl.SetSurviveDay(m_nItemDelEquipDelSurviveDay);

	//����ǿ�����ϲ�
	if(m_eMergeType == EM_MERGE_CROSS_SECTION)
	{
		//Դ1��account�����num
		DWORD dwSrc1LoginMaxNum = sLoginBeton.GetAccountMaxNum(EM_DB_SRC_ONE);
		if(dwSrc1LoginMaxNum == GT_INVALID)
		{
			return FALSE;
		}
		clsMergeLogicCtrl.SetSrc1LoginMaxNum(dwSrc1LoginMaxNum);
		g_clsOutput.Output(_T("InitOK--LoginAccount!\r\n"));
	}

	//Դ1roledate�����num
	DWORD dwSrc1RoleMaxNum = sLoongBeton.GetRoleDataMaxNum(EM_DB_SRC_ONE);
	if(dwSrc1RoleMaxNum == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1RoleMaxNum(dwSrc1RoleMaxNum);
	g_clsOutput.Output(_T("InitOK--RoleMaxNum!\r\n"));

	//Դ1petdata�����num
	DWORD dwSrc1PetMaxNum = sLoongBeton.GetPetDataMaxNum(EM_DB_SRC_ONE);
	if(dwSrc1PetMaxNum == GT_INVALID)
	{
		return FALSE;
	}

	// ��һ�κ�����ʱ��dwSrc1PetMaxNumΪ0��Ӧ���丳��ֵ����������pet_id������Ч��Χ
	if (dwSrc1PetMaxNum == 0)
	{
		dwSrc1PetMaxNum = MIN_PET_ID;
	}
	clsMergeLogicCtrl.SetSrc1PetMaxNum(dwSrc1PetMaxNum);
	g_clsOutput.Output(_T("InitOK--PetMaxNum!\r\n"));

	//Դ1������Ʒ��С���
	INT64 n64ItemMinSerialBill = sLoongBeton.GetItemMinSerialBill(EM_DB_SRC_ONE);
	if(n64ItemMinSerialBill == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMinSerialBill(n64ItemMinSerialBill);
	g_clsOutput.Output(_T("InitOK--ItemMinSerialBill!\r\n"));

	//Դ1�ڲ���Ʒ��������
	INT64 n64ItemMaxSerialInternal = sLoongBeton.GetMaxSerialInternal(EM_DB_SRC_ONE);
	if(n64ItemMaxSerialInternal == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMaxSerialInternal(n64ItemMaxSerialInternal);
	g_clsOutput.Output(_T("InitOK--ItemMaxSerialInternal!\r\n"));

	//Դ1�ⲿ��Ʒ����С���
	INT64 n64ItemMinSerialOutter = sLoongBeton.GetMinSerialOutter(EM_DB_SRC_ONE);
	if(n64ItemMinSerialOutter == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMinSerialOutter(n64ItemMinSerialOutter);
	g_clsOutput.Output(_T("InitOK--ItemMinSerialOutter!\r\n"));

	//��ʼ���������
	std::set<string> &setData = clsMergeLogicCtrl.GetCANameSet();
	BOOL bRet = sLoongBeton.Loong_QueryCentralbillingLog(setData, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	bRet = sLoongBeton.Loong_QueryCentralbillingLog(setData, EM_DB_SRC_TWO);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--CentralbillingLog!\r\n"));

	//��ʼ��Դ1��ɫ����
	std::set<tstring> &setRoleDataName = clsMergeLogicCtrl.GetSrc1RoleName();
	bRet = sLoongBeton.Loong_QueryRoleDataName(setRoleDataName, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--RoleDataName!\r\n"));

	//��ʼ��Դ1��ɫ����crc
	std::set<DWORD> &setRoleDataNameCrc = clsMergeLogicCtrl.GetSrc1RoleNameCrc();
	bRet = sLoongBeton.Loong_QueryRoleDataNameCrc(setRoleDataNameCrc, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--RoleDataNameCrc!\r\n"));

	// SRC1 ��ְ��ɫ
	std::set<DWORD> &setSrc1ClergyRoleID = clsMergeLogicCtrl.GetSrc1ClergyRoleID();
	bRet = sLoongBeton.Loong_QueryClergyRoleID(setSrc1ClergyRoleID, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	// SRC2 ��ְ��ɫ
	std::set<DWORD> &setSrc2ClergyRoleID = clsMergeLogicCtrl.GetSrc2ClergyRoleID();
	bRet = sLoongBeton.Loong_QueryClergyRoleID(setSrc2ClergyRoleID, EM_DB_SRC_TWO);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--ClergyRoleID!\r\n"));

	// ע���̴߳�����
	m_clsThreadTaskP.Clear();
	// m_clsThreadTaskP.RegisterTaskProc(Loong_ThreadTaskDel);
	m_clsThreadTaskP.RegisterTaskProc(Loong_ThreadTaskItem);
	// m_clsThreadTaskP.RegisterTaskProc(Loong_ThreadTaskTask);

	return TRUE;
}

/*
/	�����ֵĲ���
*/
BOOL CLoongServerMerge::LoongRoleCompensationChangeRoleName(const tagRoleCompensation &st)
{
	//���߲���
	std::multimap<ERoleCompensationType, tagCompensation*>::iterator itLoop =  m_mmCompensation.find(st.Type);

	if(itLoop != m_mmCompensation.end())
	{
		//Ѱ�Ҷ�Ӧ�Ĳ�������
		INT32 nCount = m_mmCompensation.count(st.Type);
		for(INT32 nLoop = 0; nLoop != nCount; ++nLoop,++itLoop)
		{
			//��Ʒ����
			INT64 n64SerialNum = m_clsMergeLogicCtrl.GetNewItemSerialOutter();
			if(typeid(*(itLoop->second)) == typeid(tagItemCompensation))
			{
				INT64 n64SerialNum = m_clsMergeLogicCtrl.GetNewItemSerialOutter();
				BOOL bRet = sLoongBeton.CreateItem(n64SerialNum,
													((tagItemCompensation*)(itLoop->second))->dwItemTypeID,
													st.dwAccountID,
													st.dwRoleID);
				if(bRet == FALSE)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/*
/	��Ҳ���
*/
BOOL CLoongServerMerge::LoongRoleCompensation()
{
	const std::vector<tagRoleCompensation> &vecRoleCompensation = m_clsMergeLogicCtrl.GetRoleCompensation();
	std::vector<tagRoleCompensation>::const_iterator itLoop =  vecRoleCompensation.begin();
	for(;itLoop != vecRoleCompensation.end(); ++itLoop)
	{
		switch((*itLoop).Type)
		{
		case ERC_CHANGE_ROLE_NAME:
			{
				BOOL bRet = LoongRoleCompensationChangeRoleName((*itLoop));
				if(bRet == FALSE)
				{
					return FALSE;
				}
			}
			break;
		default:
			break;
		}
	}

	return TRUE;
}

/*
/	���̳߳�
*/
UINT MyThreadPool(LPVOID pParam)
{
	//�쳣����
	EXCEPTION_PROTECT;

	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	WaitForSingleObject(pPara->m_hPoolMergeEvent, INFINITE);

	return pPara->m_clsThreadTaskP.Excute(pPara);
}

/*
/	��ʼ�ϲ�
*/
BOOL CLoongServerMerge::ServerMerge()
{	
	//��ʼ���ϲ��߼�����

	BOOL bRet = LoongMergeLogicCtrlInit(m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ��߼����Ƴ�ʼ��ʧ��!\r\n"));
		return FALSE;
	}

//---------------------------------------------------------------------------------------
//���·�����Ҫ��˳���ںϲ�ʱ���ȵ��� 
//---------------------------------------------------------------------------------------
	if(m_eMergeType == EM_MERGE_CROSS_SECTION)
	{
		g_clsOutput.Output(_T("��ʼ�ϲ�--LoginAccount!\r\n"));
		tagTWrap_Longin_Account *TypeAccount = NULL;
		bRet = TMethod_Login_ServerMerge(TypeAccount,m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{
			g_clsOutput.Output(_T("�ϲ�ʧ��--LoginAccount!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("�ϲ��ɹ�--LoginAccount!\r\n"));
	}
	
//---------------------------------------------------------------------------------------
	g_clsOutput.Output(_T("��ʼ�ϲ�--AccountCommon!\r\n"));
	tagTWrap_Loong_AccountCommon *TypeAccountCommon = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeAccountCommon,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--AccountCommon!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--AccountCommon!\r\n"));
//---------------------------------------------------------------------------------------
	////����
	//g_clsOutput.Output(_T("��ʼ�ϲ�--Guild!\r\n"));
	//tagTWrap_Loong_Guild *TypeGuild = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuild,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--Guild!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--Guild!\r\n"));
//---------------------------------------------------------------------------------------
//FPlan 1.4.3

	//Family
	//g_clsOutput.Output(_T("��ʼ�ϲ�--Family!\r\n"));
	//tagTWrap_Loong_Family *pTypeFamily = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamily,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--Family!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--Family!\r\n"));

//---------------------------------------------------------------------------------------
	//����role data
	g_clsOutput.Output(_T("��ʼ�ϲ�--RoleData!\r\n"));
	tagTWrap_Loong_RoleData *TypeRoleData = NULL;
	bRet = TMethod_Loong_AccountServerMerge(TypeRoleData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--RoleData!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--RoleData!\r\n"));

//---------------------------------------------------------------------------------------
//���Ϸ�����Ҫ��˳���ںϲ�ʱ���ȵ��� ��˳���ܱ�
//---------------------------------------------------------------------------------------

//�����ںϲ���������login�ϲ�
if(m_eMergeType == EM_MERGE_INSIDE_SECTION)
	goto INSIDE_SECTION;
//---------------------------------------------------------------------------------------
//���·���Ϊ�����ʱlogin��ĺϲ�

//	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginRoleName!\r\n"));
//	tagTWrap_Longin_RoleName *TypeRoleName = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeRoleName,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginRoleName!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginRoleName!\r\n"));
////---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginGuildName!\r\n"));
//	tagTWrap_Longin_GuildName *TypeGuildName = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeGuildName,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginGuildName!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginGuildName!\r\n"));
//---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginWorldForbid!\r\n"));
//	tagTWrap_Longin_WorldForbid *TypeWorldForbid = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeWorldForbid,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginWorldForbid!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginWorldForbid!\r\n"));
////---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginBlackList!\r\n"));
//	tagTWrap_Longin_BlackList *TypeLoginBlackList = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeLoginBlackList,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginBlackList!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginBlackList!\r\n"));
//---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginFatigueTime!\r\n"));
//	tagTWrap_Longin_FatigueTime *TypeLoginFatigueTime = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeLoginFatigueTime,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginFatigueTime!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginFatigueTime!\r\n"));
////---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("��ʼ�ϲ�--LoginFamilyName!\r\n"));
//	tagTWrap_Longin_FamilyName *TypeFamilyName = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeFamilyName,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("�ϲ�ʧ��--LoginFamilyName!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("�ϲ��ɹ�--LoginFamilyName!\r\n"));

//---------------------------------------------------------------------------------------
//���Ϸ���Ϊlogin��ı�
//---------------------------------------------------------------------------------------

INSIDE_SECTION://�����ںϲ���������login�ϲ�

//---------------------------------------------------------------------------------------
	//FPlan 1.4.7
	//GmServerData
	//g_clsOutput.Output(_T("��ʼ�ϲ�--GmServerData!\r\n"));
	//tagTWrap_Loong_GmServerData *pGmServerData = NULL;
	//bRet = TMethod_Loong_ServerMerge(pGmServerData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--GmServerData!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--GmServerData!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan 1.4.3

	//---------------------------------------------------------------------------------------
	//family_member
	//g_clsOutput.Output(_T("��ʼ�ϲ�--family_member!\r\n"));
	//tagTWrap_Loong_FamilyMem *pTypeFamilyMem = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilyMem,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--family_member!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--family_member!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_quest
	//g_clsOutput.Output(_T("��ʼ�ϲ�--family_quest!\r\n"));
	//tagTWrap_Loong_FamilyQuset *pTypeFamilyQuset = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilyQuset,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--family_quest!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--family_quest!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_role_sprite
	//g_clsOutput.Output(_T("��ʼ�ϲ�--family_role_sprite!\r\n"));
	//tagTWrap_Loong_FamilyRoleSprite *pTypeFamilyRoleSprite = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilyRoleSprite,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--family_role_sprite!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--family_role_sprite!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_sprite
	//g_clsOutput.Output(_T("��ʼ�ϲ�--family_sprite!\r\n"));
	//tagTWrap_Loong_FamilySprite *pTypeFamilySprite = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilySprite,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--family_sprite!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--family_sprite!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_sprite_requirement
	//g_clsOutput.Output(_T("��ʼ�ϲ�--family_sprite_requirement!\r\n"));
	//tagTWrap_Loong_SpriteRequirement *pTypeSpriteRequirement = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeSpriteRequirement,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--family_sprite_requirement!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--family_sprite_requirement!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan 1.4.0
	//����
	g_clsOutput.Output(_T("��ʼ�ϲ�--Area!\r\n"));
	tagTWrap_Sheng_Area *TypeArea = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeArea,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--Area!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Area!\r\n"));

	//ϡ������
	//g_clsOutput.Output(_T("��ʼ�ϲ�--RareArea!\r\n"));
	//tagTWrap_Sheng_RareArea *TypeRareArea = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeRareArea,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--RareArea!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--RareArea!\r\n"));


//---------------------------------------------------------------------------------------
//FPlan 1.3.6
	//�³�
	g_clsOutput.Output(_T("��ʼ�ϲ�--Wardrobe!\r\n"));
	tagTWrap_Sheng_Wardrobe *TypeWardrobe = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeWardrobe,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--Wardrobe!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Wardrobe!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan1.3.2
	//�ҵ�
	//g_clsOutput.Output(_T("��ʼ�ϲ�--EggInfo!\r\n"));
	//tagTWrap_Sheng_EggInfo *TypeEggInfo = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeEggInfo,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--EggInfo!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--EggInfo!\r\n"));


//---------------------------------------------------------------------------------------
//FPlan1.3.0
	//��֮����GodBid
	//g_clsOutput.Output(_T("��ʼ�ϲ�--GodBid!\r\n"));
	//tagTWrap_Sheng_GodBid *TypeGodBid = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeGodBid,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--GodBid!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--GodBid!\r\n"));

	
	//activity
	//g_clsOutput.Output(_T("��ʼ�ϲ�--activity!\r\n"));
	//tagTWrap_Loong_ActivityData *TypeActivityData = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeActivityData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//g_clsOutput.Output(_T("�ϲ�ʧ��--activity!\r\n"));
	//return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--activity!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan1.2.1

	//������������tagTWrap_Sheng_FabaoNirVanaData
	g_clsOutput.Output(_T("��ʼ�ϲ�--FabaoNirVanaData!\r\n"));
	tagTWrap_Sheng_FabaoNirVanaData *TypeFabaoNirVanaData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFabaoNirVanaData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--FabaoNirVanaData!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--FabaoNirVanaData!\r\n"));

	//�������ʱ��ͳ��tagTWrap_Sheng_CurMonthOnlineTime
	//g_clsOutput.Output(_T("��ʼ�ϲ�--CurMonthOnlineTime!\r\n"));
	//tagTWrap_Sheng_CurMonthOnlineTime *TypeCurMonthOnlineTime = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeCurMonthOnlineTime,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--CurMonthOnlineTime!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--CurMonthOnlineTime!\r\n"));

	//tagTWrap_Sheng_GuildWarDeclare
	//g_clsOutput.Output(_T("��ʼ�ϲ�--GuildWarDeclare!\r\n"));
	//tagTWrap_Sheng_GuildWarDeclare *TypeGuildWarDeclare = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildWarDeclare,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--GuildWarDeclare!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--GuildWarDeclare!\r\n"));


	//tagTWrap_Sheng_BoardQuestSaveData
	//g_clsOutput.Output(_T("��ʼ�ϲ�--BoardQuestSaveData!\r\n"));
	//tagTWrap_Sheng_BoardQuestSaveData *TypeBoardQuestSaveData = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeBoardQuestSaveData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--BoardQuestSaveData!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--BoardQuestSaveData!\r\n"));



//---------------------------------------------------------------------------------------
//2.2.1-������

	//tagTWrap_Loong_ForceBreakOutApprentice
	//g_clsOutput.Output(_T("��ʼ�ϲ�--ForceBreakOutApprentice!\r\n"));
	//tagTWrap_Loong_ForceBreakOutApprentice *TypeForceBreakOutApprentice = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeForceBreakOutApprentice,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--ForceBreakOutApprentice!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--ForceBreakOutApprentice!\r\n"));

//---------------------------------------------------------------------------------------
//2.0.0-������

	//fabao
	g_clsOutput.Output(_T("��ʼ�ϲ�--fabao!\r\n"));
	tagTWrap_Loong_Fabao *TypeFabao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFabao,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--fabao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--fabao!\r\n"));


	// train_state
	//g_clsOutput.Output(_T("��ʼ�ϲ�--train_state!\r\n"));
	//tagTWrap_Loong_TrainState *TypeTrainState = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeTrainState,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--train_state!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--train_state!\r\n"));

	//ActiveRankData
	//g_clsOutput.Output(_T("��ʼ�ϲ�--ActiveRankData!\r\n"));
	//tagTWrap_Loong_ActiveRankData *TypeActiveRankData = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeActiveRankData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--ActiveRankData!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--ActiveRankData!\r\n"));

//---------------------------------------------------------------------------------------
//1.3.2-������

	//Master
	//g_clsOutput.Output(_T("��ʼ�ϲ�--Master!\r\n"));
	//tagTWrap_Loong_Master *TypeMaster = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeMaster,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--Master!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--Master!\r\n"));

	//shortcut_quickbar
	g_clsOutput.Output(_T("��ʼ�ϲ�--shortcut_quickbar!\r\n"));
	tagTWrap_Loong_ShortCutBar *TypeShortCutBar = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeShortCutBar,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--shortcut_quickbar!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--shortcut_quickbar!\r\n"));


//1.3.2-������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//1.3.1-������
/*
	//ExternalLink
	g_clsOutput.Output(_T("��ʼ�ϲ�--ExternalLink!\r\n"));
	tagTWrap_Loong_ExternalLink *TypeExternalLink = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeExternalLink,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--ExternalLink!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--ExternalLink!\r\n"));		
*/
	//NameHistory
/*	g_clsOutput.Output(_T("��ʼ�ϲ�--NameHistory!\r\n"));
	tagTWrap_Loong_NameHistory *TypeNameHistory = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeNameHistory,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--NameHistory!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--NameHistory!\r\n"));	*/	


	//City
	//g_clsOutput.Output(_T("��ʼ�ϲ�--City!\r\n"));
	//tagTWrap_Loong_City *TypeCity = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeCity,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--City!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--City!\r\n"));	

//---------------------------------------------------------------------------------------
	//ActClanTreasure
	//g_clsOutput.Output(_T("��ʼ�ϲ�--ActClanTreasure!\r\n"));
	//tagTWrap_Loong_ActClanTreasure *TypeActClanTreasure = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeActClanTreasure,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--ActClanTreasure!\r\n"));
	//	return FALSE;
	//}	
	//g_clsOutput.Output(_T("�ϲ��ɹ�--ActClanTreasure!\r\n"));
//---------------------------------------------------------------------------------------
	//g_clsOutput.Output(_T("��ʼ�ϲ�--RepRstTime!\r\n"));
	//tagTWrap_Loong_RepRstTime *TypeRepRstTime = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeRepRstTime,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--RepRstTime!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--RepRstTime!\r\n"));
//---------------------------------------------------------------------------------------
	//g_clsOutput.Output(_T("��ʼ�ϲ�--FameHallEnterSnap!\r\n"));
	//tagTWrap_Loong_FameHallEnterSnap *TypeFameHallEnterSnap = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeFameHallEnterSnap,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--FameHallEnterSnap!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--FameHallEnterSnap!\r\n"));
//---------------------------------------------------------------------------------------
	//yuanbaoorder
	//g_clsOutput.Output(_T("��ʼ�ϲ�--yuanbaoorder!\r\n"));
	//tagTWrap_Loong_YuanBaoOrder *TypeYuanBaoOrder = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeYuanBaoOrder,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--yuanbaoorder!\r\n"));
	//	return FALSE;
	//}	
	//g_clsOutput.Output(_T("�ϲ��ɹ�--yuanbaoorder!\r\n"));
//---------------------------------------------------------------------------------------
	//vip_stall
/*	g_clsOutput.Output(_T("��ʼ�ϲ�--VipStall!\r\n"));
	tagTWrap_Loong_VipStall *TypeVipStall = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVipStall,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--VipStall!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--VipStall!\r\n"))*/;
//---------------------------------------------------------------------------------------
	//pet_skill
	g_clsOutput.Output(_T("��ʼ�ϲ�--PetSkill!\r\n"));
	tagTWrap_Loong_PetSkill *TypePetSkill = NULL;
	bRet = TMethod_Loong_ServerMerge(TypePetSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--PetSkill!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--PetSkill!\r\n"));
//---------------------------------------------------------------------------------------
	//pet_data
	g_clsOutput.Output(_T("��ʼ�ϲ�--PetData!\r\n"));
	tagTWrap_Loong_PetData *TypePetData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypePetData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--PetData!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--PetData!\r\n"));
//---------------------------------------------------------------------------------------
	//BlackList
	//g_clsOutput.Output(_T("��ʼ�ϲ�--BlackList!\r\n"));
	//tagTWrap_Loong_BlackList *TypeBlackList = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeBlackList,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--BlackList!\r\n"));
	//	return FALSE;
	//}	
	//g_clsOutput.Output(_T("�ϲ��ɹ�--BlackList!\r\n"));
//---------------------------------------------------------------------------------------
	//left msg
	//g_clsOutput.Output(_T("��ʼ�ϲ�--left_msg!\r\n"));
	//tagTWrap_Loong_LeftMsg *TypeLeftMsg = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeLeftMsg,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--left_msg!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--left_msg!\r\n"));
//---------------------------------------------------------------------------------------
	//buff
	g_clsOutput.Output(_T("��ʼ�ϲ�--buff!\r\n"));
	tagTWrap_Loong_Buff *TypeBuff = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeBuff,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--buff!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--buff!\r\n"));	

//---------------------------------------------------------------------------------------
	//clan_data
/*	g_clsOutput.Output(_T("��ʼ�ϲ�--clan_data!\r\n"));
	tagTWrap_Loong_ClanData *TypeClanDate = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeClanDate,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--clan_data!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("�ϲ��ɹ�--clan_data!\r\n"));*/	
//---------------------------------------------------------------------------------------
	//enemy
	//g_clsOutput.Output(_T("��ʼ�ϲ�--enemy!\r\n"));
	//tagTWrap_Loong_Enemy *TypeEnemy = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeEnemy,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--enemy!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--enemy!\r\n"));
//---------------------------------------------------------------------------------------
	//friend
	//g_clsOutput.Output(_T("��ʼ�ϲ�--Friend!\r\n"));
	//tagTWrap_Loong_Friend *TypeFriend = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeFriend,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--Friend!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--Friend!\r\n"));

//---------------------------------------------------------------------------------------
	//friendship
	//g_clsOutput.Output(_T("��ʼ�ϲ�--FriendShip!\r\n"));
	//tagTWrap_Loong_FriendshipSave *TypeFriendShip = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeFriendShip,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�FriendShipʧ��!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--FriendShip!\r\n"));

//---------------------------------------------------------------------------------------
	//GroupPurchase
	//g_clsOutput.Output(_T("��ʼ�ϲ�--GroupPuichase!\r\n"));
	//tagTWrap_Loong_GroupPurchase *TypeGroupPurchase = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGroupPurchase,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�GroupPuichaseʧ��!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--GroupPuichase!\r\n"));

//---------------------------------------------------------------------------------------
	//GuildMem
	//g_clsOutput.Output(_T("��ʼ�ϲ�--GuildMem!\r\n"));
	//tagTWrap_Loong_GuildMem *TypeGuildMem = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildMem,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--GuildMem!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--GuildMem!\r\n"));

//---------------------------------------------------------------------------------------
	//item_cdtime
	g_clsOutput.Output(_T("��ʼ�ϲ�--ItemCDTime!\r\n"));
	tagTWrap_Loong_ItemCDTime *TypeItemCDTime = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeItemCDTime,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--ItemCDTime!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--ItemCDTime!\r\n"));

//---------------------------------------------------------------------------------------
	//item_needlog
	//g_clsOutput.Output(_T("��ʼ�ϲ�--ItemNeedLog!\r\n"));
	//tagTWrap_Loong_ItemNeedLog *TypeItemNeedLog = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeItemNeedLog,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--ItemNeedLog!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--ItemNeedLog!\r\n"));
//---------------------------------------------------------------------------------------
	//log_baibao
	g_clsOutput.Output(_T("��ʼ�ϲ�--log_baibao!\r\n"));
	tagTWrap_Loong_LogBaiBao *TypeLogBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeLogBaiBao,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--log_baibao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--log_baibao!\r\n"));

//---------------------------------------------------------------------------------------
	//role_del
	//g_clsOutput.Output(_T("��ʼ�ϲ�--RoleDel!\r\n"));
	//tagTWrap_Loong_RoleDel *TypeRoleDel = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeRoleDel,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--RoleDel!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--RoleDel!\r\n"));

//---------------------------------------------------------------------------------------
	//skill
	g_clsOutput.Output(_T("��ʼ�ϲ�--Skill!\r\n"));
	tagTWrap_Loong_Skill *TypeSkill = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--Skill!\r\n"));
		return FALSE;
	}		
	g_clsOutput.Output(_T("�ϲ��ɹ�--Skill!\r\n"));

//---------------------------------------------------------------------------------------
	//title
	g_clsOutput.Output(_T("��ʼ�ϲ�--Title!\r\n"));
	tagTWrap_Loong_Title *TypeTitle = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeTitle,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--Title!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Title!\r\n"));

//---------------------------------------------------------------------------------------
	//��Ƭ
	g_clsOutput.Output(_T("��ʼ�ϲ�--VisitingCard!\r\n"));
	tagTWrap_Loong_VisitingCard *TypeVisitingCard = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVisitingCard,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--VisitingCard!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--VisitingCard!\r\n"));

//---------------------------------------------------------------------------------------
	//Ԫ���˺�
	//g_clsOutput.Output(_T("��ʼ�ϲ�--FameYBAccount!\r\n"));
	//tagTWrap_Loong_YBAccount *TypeYBAccount = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeYBAccount,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--FameYBAccount!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--FameYBAccount!\r\n"));

//---------------------------------------------------------------------------------------
	//guild_commodity
	//g_clsOutput.Output(_T("��ʼ�ϲ�--guild_commodity!\r\n"));
	//tagTWrap_Loong_GuildCommodity *TypeGuildCommodity = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildCommodity,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--guild_commodity!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--guild_commodity!\r\n"));

//---------------------------------------------------------------------------------------
	//guild_skill
	//g_clsOutput.Output(_T("��ʼ�ϲ�--guild_skill!\r\n"));
	//tagTWrap_Loong_GuildSkill *TypeGuildSkill = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildSkill,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--guild_skill!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--guild_skill!\r\n"));

//---------------------------------------------------------------------------------------
	//vip_netbar
	//g_clsOutput.Output(_T("��ʼ�ϲ�--vip_netbar!\r\n"));
	//tagTWrap_Loong_VipNetBar *TypeVipNetBar = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeVipNetBar,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--vip_netbar!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--vip_netbar!\r\n"));

//---------------------------------------------------------------------------------------
	//guild_upgrade
	//g_clsOutput.Output(_T("��ʼ�ϲ�--guild_upgrade!\r\n"));
	//tagTWrap_Loong_GuildFacilities *TypeGuildFacilities = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildFacilities,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--guild_upgrade!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--guild_upgrade!\r\n"));	

//---------------------------------------------------------------------------------------
	//commerce_rank
/*	g_clsOutput.Output(_T("��ʼ�ϲ�--commerce_rank!\r\n"));
	tagTWrap_Loong_CommerceRank *TypeCommerceRank = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeCommerceRank,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--commerce_rank!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--commerce_rank!\r\n"));*/	

	// F-Plan 1.5.0 ������ soulcrystal
	g_clsOutput.Output(_T("��ʼ�ϲ�--soulcrystal!\r\n"));
	tagTWrap_Loong_SoulCrystal *TypeSoulCrystal = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeSoulCrystal,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--soulcrystal!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--soulcrystal!\r\n"));

	// F-Plan 1.5.0 ������ Account_Reactive
	//g_clsOutput.Output(_T("��ʼ�ϲ�--account_reactive!\r\n"));
	//tagTWrap_Loong_AccountReactive *TypeAccountReactive = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeAccountReactive,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--account_reactive!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--account_reactive!\r\n"));

	// F-Plan 1.5.0 ������ Account_Reactiver
	//g_clsOutput.Output(_T("��ʼ�ϲ�--account_reactiver!\r\n"));
	//tagTWrap_Loong_AccountReactiver *TypeAccountReactiver = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeAccountReactiver,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("�ϲ�ʧ��--account_reactiver!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("�ϲ��ɹ�--account_reactiver!\r\n"));

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//�������������ܴ�ļ����� �����̳߳غϲ�
//---------------------------------------------------------------------------------------

	//�����̳߳غϲ�
	SetEvent(m_hPoolMergeEvent);

	//�ȴ��̳߳��������߳�
	EThreadPoolState eThreadState = this->LoongWaitThreadTask();

	//����߼�
	m_clsMergeLogicCtrl.ClearMemory();

	//ɾ����ϡ��������ص�����
	if(eThreadState == ETPS_SUCCESS)
	{
		if( !sLoongBeton.DeleteRareareaData(EM_DB_SRC_ONE) )
		{
			eThreadState == ETPS_FAILURE;
		}
	}

	if(eThreadState == ETPS_SUCCESS)
	{
		g_clsOutput.Output(_T("�ɹ�!\r\n"));
	}
	else if(eThreadState == ETPS_FAILURE)
	{
		//�رվ��
		std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
		for(;itH != m_listSimpleThreadPool.end(); ++itH)
		{
			if(*itH != NULL)
			{
				TerminateThread(*itH,0);
				CloseHandle(*itH);
				*itH = 0;
			}
		}
		g_clsOutput.Output(_T("ʧ�� ���������� ���ºϲ�!\r\n"));
	}

	return TRUE;
}

/*
/	�ȴ��߳�
*/
EThreadPoolState CLoongServerMerge::LoongWaitThreadTask()
{
	EThreadPoolState e = ETPS_NULL;
	while(TRUE)
	{
		e = GetThreadPoolState();
		if(e == ETPS_STILL_HAVE_ACTIVE)
		{
			Sleep(500); //0.5��
			continue;
		}
		else if(e == ETPS_FAILURE)
		{
			break;
		}
		else if(e == ETPS_SUCCESS)
		{
			break;
		}
	}

	//����Ϸ��ɹ� ���������Ӧ�Ĳ���
	//if(e == ETPS_SUCCESS)
	//{
	//	g_clsOutput.Output(_T("��ʼ��Ҳ���!\r\n"));
	//	BOOL bRet = LoongRoleCompensation();
	//	if(bRet  == FALSE)
	//	{
	//		g_clsOutput.Output(_T("��Ҳ���ʧ��!\r\n"));
	//		e = ETPS_FAILURE;
	//		goto END_MERGE;
	//	}	
	//	g_clsOutput.Output(_T("��Ҳ����ɹ�!\r\n"));
	//}

	//�����ϲ�
END_MERGE:

	return e;
}


/*
/	�̳߳����Ƿ����̻߳���
*/
EThreadPoolState CLoongServerMerge::GetThreadPoolState()
{
	std::list<HANDLE>::iterator itLoop = m_listSimpleThreadPool.begin();

	EThreadPoolState eTPState = ETPS_SUCCESS;

	for(;itLoop != m_listSimpleThreadPool.end(); ++itLoop)
	{
		HANDLE hHandle = *itLoop;
		if( hHandle )
		{
			DWORD dwConnectExitCode = STILL_ACTIVE;
			GetExitCodeThread(hHandle, &dwConnectExitCode);
			//ʧ��
			if(FALSE == dwConnectExitCode)
				return ETPS_FAILURE;
			else if( STILL_ACTIVE == dwConnectExitCode )
			{
				eTPState = ETPS_STILL_HAVE_ACTIVE;
			}
		}
	}

	return eTPState;
}
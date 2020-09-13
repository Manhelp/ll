//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong_ThreadTask.cpp
// author: songg
// actor:
// data: 2010-11-16
// last:
// brief: �߳�����
//-------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Loong_ThreadTask.h"
#include "ServerMerge.h"
#include "Loong_ServerMerge.h"
#include "Loong_OutputCtrl.h"
#include "Loong_TMethod.h"


//---------------------------------------------------------------------------------------
// itemdel�� equipdel��
//---------------------------------------------------------------------------------------
BOOL Loong_ThreadTaskDel(LPVOID pParam)
{
	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	//////////////////////////////////////////////////////////////////////////
	//itemdel
	g_clsOutput.Output(_T("��ʼ�ϲ�--itemdel!\r\n"));
	tagTWrap_Loong_ItemDel *TypeItemDel = NULL;
	BOOL bRet = TMethod_Loong_PagingServerMerge(TypeItemDel,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--itemdel!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--itemdel!\r\n"));

	//equipdel
	g_clsOutput.Output(_T("��ʼ�ϲ�--equipdel!\r\n"));
	tagTWrap_Loong_EquipDel *TypeEquipDel = NULL;
	bRet = TMethod_Loong_PagingServerMerge(TypeEquipDel,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--equipdel!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--equipdel!\r\n"));

	return TRUE;
}

//---------------------------------------------------------------------------------------
// item�� item_baibao�� equip�� equip_baibao  (item itembaibao equip equipbaibao˳���ܱ�)
//---------------------------------------------------------------------------------------
BOOL Loong_ThreadTaskItem(LPVOID pParam)
{
	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	//////////////////////////////////////////////////////////////////////////
	//item
	g_clsOutput.Output(_T("��ʼ�ϲ�--item!\r\n"));
	tagTWrap_Loong_Item *TypeItem = NULL;
	BOOL bRet = TMethod_Loong_AccountServerMerge(TypeItem,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--item!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--item!\r\n"));

	//////////////////////////////////////////////////////////////////////////
	//itembaibao
	g_clsOutput.Output(_T("��ʼ�ϲ�--itembaibao!\r\n"));
	tagTWrap_Loong_ItemBaiBao *TypeItemBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeItemBaiBao,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�itembaibaoʧ��!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--itembaibao!\r\n"));

	//equip
	g_clsOutput.Output(_T("��ʼ�ϲ�--equip!\r\n"));
	tagTWrap_Loong_Equip *TypeEquip = NULL;
	bRet = TMethod_Loong_PagingServerMerge(TypeEquip,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--equip!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--equip!\r\n"));

	//equipbaibao
	g_clsOutput.Output(_T("��ʼ�ϲ�--Equipbaibao!\r\n"));
	tagTWrap_Loong_EquipBaiBao *TypeEquipBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeEquipBaiBao,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--Equipbaibao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Equipbaibao!\r\n"));

	return TRUE;
}

//---------------------------------------------------------------------------------------
// task�� task_done��
//---------------------------------------------------------------------------------------
BOOL Loong_ThreadTaskTask(LPVOID pParam)
{
	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	//task
	g_clsOutput.Output(_T("��ʼ�ϲ�--Task!\r\n"));
	tagTWrap_Loong_Task *TypeTask = NULL;
	BOOL bRet = TMethod_Loong_PagingServerMerge(TypeTask,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("�ϲ�ʧ��--Task!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--Task!\r\n"));

	//taskdone
	g_clsOutput.Output(_T("��ʼ�ϲ�--TaskDone!\r\n"));
	tagTWrap_Loong_TaskDone *TypeTaskDone = NULL;
	bRet = TMethod_Loong_PagingServerMerge(TypeTaskDone,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("�ϲ�ʧ��--TaskDone!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("�ϲ��ɹ�--TaskDone!\r\n"));

	return TRUE;
}
#pragma once
#include "servermerge.h"
#include "Loong_CommonDef.h"
#include "Loong_MergeLogicCtrl.h"
#include "Loong_ThreadTask.h"

//ǰ������
class CLoongMergeLogicCtrl;
struct tagThreadPara;

class CLoongServerMerge :public CServerMerge
{
public:
	CLoongServerMerge(void);
	virtual ~CLoongServerMerge(void);

//�ӿ�ʵ��
public:
	/*
	/	��ʼ��
	*/
	BOOL Init(	tagConfigDetail	stConfigOne[EM_DB_TYPE_END],
				tagConfigDetail stConfigTwo[EM_DB_TYPE_END],
				EDBMergeType	eMergeType,
				BOOL			bDataClean,
				tstring			strSrc2WorldName,
				INT				nSurviveDay,
				const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation);
	/*
	/	��ʼ�ϲ�
	*/
	BOOL ServerMerge();

public:
	//�ϲ��߼�
	CLoongMergeLogicCtrl m_clsMergeLogicCtrl;

	//�����̳߳غϲ����¼�
	HANDLE				m_hPoolMergeEvent;

	ThreadTaskPool		m_clsThreadTaskP;			// �߳�����

private:
	//��ʼ���ϲ��߼�
	BOOL LoongMergeLogicCtrlInit(CLoongMergeLogicCtrl &clsMergeLogicCtrl);

	//�õ��̳߳�״̬
	EThreadPoolState GetThreadPoolState();

	//��Ҳ���
	BOOL LoongRoleCompensation();

	//��������
	BOOL LoongRoleCompensationChangeRoleName(const tagRoleCompensation &st);

	//�ȴ��߳�
	EThreadPoolState LoongWaitThreadTask();
private:  

	BOOL				m_bDataClean;				// ��������

	EDBMergeType		m_eMergeType;				// db�ϲ�����

	INT					m_nItemDelEquipDelSurviveDay; //������������Ʒɾ�����ݺ�װ��ɾ������

	tstring				m_strSrc2WorldName;			// Դ2������

	DataBase			m_LoongDB[EM_DB_TYPE_END];	// Beton���ݿ�

	std::list<HANDLE>	m_listSimpleThreadPool;		// ���̳߳�	

	std::multimap<ERoleCompensationType, tagCompensation*> m_mmCompensation; // ��Ҳ���
};

UINT MyThreadPool(LPVOID pParam);

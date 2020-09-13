//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong_ThreadTask.h
// author: songg
// actor:
// data: 2010-11-16
// last:
// brief: �߳�����
//-------------------------------------------------------------------------------
#pragma once
#include "Loong_CommonDef.h"

// �������
class ThreadTaskBase
{
public:
	// ������
	typedef BOOL (*TaskProc)(LPVOID pParam);

public:
	ThreadTaskBase() {}
	virtual ~ThreadTaskBase(){}

public:
	// ע������ִ�к���
	VOID RegisterTaskProc(TaskProc tp)
	{
		if(P_VALID(tp))
			m_lisTasks.PushBack(tp);
	}

	VOID Clear()
	{
		m_lisTasks.Clear();
	}

public:
	virtual BOOL Excute(LPVOID pParam) = 0;

protected:
	TSafeList<TaskProc> m_lisTasks; //����
};

// ����ִ��ʵ��
class ThreadTaskPool: public ThreadTaskBase
{
public:
	ThreadTaskPool(){}
	virtual ~ThreadTaskPool(){}

public:
	virtual BOOL Excute(LPVOID pParam)
	{
		TaskProc tp = m_lisTasks.PopFront();
		if(!P_VALID(tp))// �Ҳ���������
			return TRUE;

		return (*tp)(pParam);
	}
};

// ���񷽷�����
BOOL Loong_ThreadTaskDel(LPVOID pParam);
BOOL Loong_ThreadTaskItem(LPVOID pParam);
BOOL Loong_ThreadTaskTask(LPVOID pParam);
/********************************************************************
	created:	2012/05/31
	created:	31:5:2012   18:00
	filename: 	e:\code\World\globedata.h
	file ext:	h
	author:		zhangzhihua
	
	purpose:	��Ϸȫ�ֱ���������
*********************************************************************/
#pragma once

enum EGlobeData
{
	EGD_MallActivityState					= 0,			//�̳ǻ״̬ 0����

	EGD_End,
};

//����ģ����
template <class T>
class Singleton
{
public:
	static T* getSingletonPtr()
	{
		if (NULL == m_Instance)
		{
			m_Instance = new T();
		}

		return m_Instance;
	}

protected:
	Singleton() {};
	~Singleton()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

private:
	static T* m_Instance;
};
template <class T> T* Singleton<T>::m_Instance = NULL;

class GlobeData : public Singleton<GlobeData>
{
public:
	GlobeData();
	~GlobeData();

	VOID					Init();

public:
	INT						Get(EGlobeData eKey);

private:
	INT						m_nValue[EGD_End];
};


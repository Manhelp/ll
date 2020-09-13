// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

//-----------------------------------------------------------------------------
// �������ݿ�����(�������ͷ�ļ����Ѱ���vEngine)
//-----------------------------------------------------------------------------
#define _SECURE_SCL 1
#define _SECURE_SCL_THROWS 1

#include "../Beton/Beton.h"
using namespace vEngine;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
#pragma comment(lib, "../vsout/Beton/debug/Beton.lib")
#pragma comment(lib, "../vsout/ServerDefine/debug/ServerDefine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib, "../vsout/Beton/release/Beton.lib")
#pragma comment(lib, "../vsout/ServerDefine/release/ServerDefine.lib")
#endif

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")


#include "ResGuard.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../ServerDefine/base_define.h"
#include "../WorldDefine/time.h"
//-----------------------------------------------------------------

// Jason 2010-1-30 v1.3.2 �����������
extern INT MAX_PETSOUL_NUM;


template<typename T>
struct JMArrayAllocAdaptor
{
	typedef T * pointer;
	static pointer construct(size_t dem, void * p = NULL)
	{
		size_t size = dem;
		if( size > 0 )
			return new T[dem];
		return NULL;
	}
	static void destroy(pointer p){if(p) delete[] p;}
};

template<class T,class Alloc=JMArrayAllocAdaptor<T> >
class TArray
{
	typename Alloc::pointer _data;
	size_t _size;
public:
	TArray(size_t demension)
		: _size(demension)
	{
		_data = Alloc::construct(_size);
	}
	~TArray(){Alloc::destroy(_data);}
	T& operator[] (int index)
	{
		return _data[index];
	}

	size_t Size() const
	{
		return _size;
	}

	operator T*()
	{
		return _data;
	}
};


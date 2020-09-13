//-----------------------------------------------------------------------------
//!\file title_mgr.h
//!\author xlguo
//!
//!\date 2009-01-18
//! last 2009-02-02
//!
//!\brief �ƺ���Ϣ�������
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

// project
#include "..\ServerDefine\title_define.h"
#include "..\WorldDefine\msg_role_title.h"
#include "att_res.h"

// stl
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>

using std::bitset;
using std::vector;
using std::list;
using std::find;

// Jason 2010-3-30 v1.5.1
INT GetMaxTitleNum(void);
#define MAX_TITLE_NUM_EX	GetMaxTitleNum()
bool TITLEID_VALID_EX( INT id );
VOID SetMaxTitleNum(INT v);
// ��������������ǿ���bitset��Ŀǰֻ�ܲ���ʵ������bitset�Ĺ��ܣ���֧�ָ�ֵ�Ȳ���
//template <class T, class Con = vector<T> >
class TBitSet : public vector<bool>
{
	int _size;
	bool _validate(int ndx)
	{
		return ndx >= 0 && ndx < _size ;
	}
public:
	TBitSet(INT ncon = MAX_TITLE_NUM)
		:vector<bool>(ncon),_size(ncon)
	{
		reset();
	}
	void arrage(int size)
	{
		resize(size);
		reset();
		_size = size;
	}
	void reset(void)
	{
		for(vector<bool>::iterator i = begin(); i != end() ; ++i)
			*i = false;
	}
	void reset(int ndx)
	{
		if( _validate(ndx) )
			(*this)[ndx] = false;
	}
	void set(int ndx)
	{
		if( _validate(ndx) )
			(*this)[ndx] = true;
	}
	bool test(int ndx)
	{
		if( _validate(ndx) )
			return (*this)[ndx];
		return false;
	}
	int count()
	{
		int r = 0;
		for(vector<bool>::iterator i = begin(); i != end() ; ++i)
			if( *i )
				++r;
		return r;
	}
};



class TitleMgr
{
	typedef list<UINT16>					ListUINT16;
	typedef vector<UINT16>					VecUINT16;
	// Jason 2010-3-30
	//typedef bitset<MAX_TITLE_NUM>			BitSet;
	//typedef vector<bool> BitSet;
	typedef TBitSet BitSet;

public:
	~TitleMgr(){	Destroy();	}

	// 1��ʼ��ѡ��
	void InitOpts(Role* pRole, UINT16 u16ActTitleID, BOOL bVisibility);

	// 2��ʼ���ƺ�����
	void InitTitles(const BYTE* &pData, const INT32 nNum);

	// ��������
	void Destroy();

	// ���£���Ϊʱ���Ƴƺţ������ʣ��ʱ��
	void Update();

	void Resize(int size)
	{
		m_bitsetObtainedMark.arrage(size);
		m_bitsetDBInserted.arrage(size);
		m_bitsetNeedSaveDB.arrage(size);
		m_ArrPtrCondition.resize(size);
	}

public:
	// �洢�����ݿ�
	void SaveTitlesToDB(IN LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	// ���ź�
	bool SigEvent(EnumTitleEvent titleEvent, DWORD dwArg1, DWORD dwArg2);

	// ���óƺſɼ���
	DWORD SetVisibility(bool bVisibility)	{	m_bVisibility = bVisibility;	return E_Title_Success;	}

	// ���ؿɼ���
	bool Visibility() const { return TRUE == m_bVisibility; }

	// ����ƺ�
	DWORD ActiviteTitle(const UINT16 u16TitleID);
	
	// ��ǰʹ�óƺ�
	UINT16 GetActiviteTitle() const {	return m_u16ActiveID;	}

	// ȡ�������ѻ�óƺ�����
	DWORD GetObtainedTitleIDs(UINT16* &pData, UINT16 &u16Num);

	// �ѻ�óƺ���Ŀ
	DWORD GetObtainedTitlesNum() {	return m_bitsetObtainedMark.count();}

	//���ʣ��ʱ��map
	VOID GetRemainTimeMap( TMap<UINT16,DOUBLE> &mapTimeRemain){ mapTimeRemain = m_mapTimeRemain;}

	// �����ƺ�����
	DWORD ReturnTitlesData(LPVOID pData) {	SAFE_DEL(pData);	return E_Title_Success;	}

	//ɾ���ƺ�
	BOOL SigRemoveEvent( EnumTitleEvent titleEvent, DWORD dwArg1 = GT_INVALID);

	// ���ƺ��Ƿ��Ѿ���ȡ
	BOOL IsTitleObtained(UINT16 u16TitleID) { return m_bitsetObtainedMark.test(u16TitleID); }

private:
	// �ƺŲ��뵽���ݿ�
	VOID InsertTitle2DB( UINT16 u16TitleID, BOOL bNew );

	// �ƺŴ����ݿ�ɾ��
	VOID RemoveTitleFromDB( UINT16 u16TitleID);

	// �����ϵ
	void CleanRelation(const UINT16 u16TitleID);

	// ֪ͨ�ͻ����»�õĳƺ�
	void NotifyClient(VecUINT16& vecNewObtTitles);
	// ֪ͨ�ͻ��� ��Ҷ�ʧ�˳ƺ�
	void NotifyClientDelete(VecUINT16& vecDelTitleIDs);
private:
	UINT16								m_u16ActiveID;						// ��ǰ�ƺ�
	BOOL								m_bVisibility;						// �ɼ���

	ListUINT16							m_EventMap[ETE_MAX_EVENTNUM];		// �¼�ӳ��
	// Jason 2010-3-30
	//Condition*							m_ArrPtrCondition[MAX_TITLE_NUM];	// ����ָ������
	vector<Condition*>						m_ArrPtrCondition;

	BitSet								m_bitsetObtainedMark;				// �ƺ��Ƿ��ѻ��
	BitSet								m_bitsetDBInserted;					// �Ѳ������ݿ�
	BitSet								m_bitsetNeedSaveDB;					// ��Ҫ���浽���ݿ�
	//TMap<UINT16,DWORD>					m_mapTimeRemain;					// ID��Ӧ��ʣ��ʱ�䣬���ڼ�¼��ʱ�ƺ�
	TMap<UINT16,DOUBLE>					m_mapTimeRemain;					// ID��Ӧ��ʣ��ʱ�䣬���ڼ�¼��ʱ�ƺ�

																			// ��msΪ��λ

	BOOL								m_bDayHasSent;						//ʣ��һ�����Ϣ�ѷ���
	BOOL								m_bHourHasSent;						//ʣ��һСʱ����Ϣ�ѷ���

	Role*								m_pRole;							// ��ɫָ��

};


#pragma once
#include ".\BaseData.h"

struct tagMallTitleData
{
	INT			nIndex;
	tstring		strTitleName;
};

struct tagMallTitle2Data
{
	INT			nIndex;
	tstring		strTitle2Name;
};

class MallData : public BaseData
{
public:
	MallData(void);
	~MallData(void);

	static MallData* Inst();

	/** �����̳������ļ�
	*/
	virtual void LoadFromFile();

	tagMallTitleData* FindMallTitleData(DWORD dwID)
	{
		return FindInMap(m_mapMallData, dwID);
	}

	tagMallTitle2Data* FindMall2Data(DWORD dwID)
	{
		return FindInMap(m_mapMall2Data, dwID);
	}

	// �õ�һ�����������Ķ�������
	const multimap<DWORD, tagMallTitle2Data>* GetMall1to2Data()
	{
		return &m_mmapMall1to2Data;
	}

	DWORD GetMallTitleSize() { return m_dwSize; }

private:
	map<DWORD, tagMallTitleData>		m_mapMallData;
	map<DWORD, tagMallTitle2Data>		m_mapMall2Data;
	multimap<DWORD, tagMallTitle2Data>	m_mmapMall1to2Data;
	DWORD								m_dwSize;
};
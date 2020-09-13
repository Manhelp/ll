#pragma once
#include "BaseData.h"


class BuyTalentData : public BaseData
{
public:
	BuyTalentData(void);
	~BuyTalentData(void);

	static BuyTalentData* Inst();

	/** ���ع����N��������Ҫ�Ļ���
	*/
	virtual void LoadFromFile();

	/** param  ��ʾ��dwTalentNum������
	*/
	const INT64 FindCost( DWORD dwTalentNum )
	{
		std::map<DWORD, INT64>::iterator iter = m_mapCost.find( dwTalentNum );
		if( iter != m_mapCost.end() )
			return iter->second;
		else
			return 0;
	}

private:
	std::map<DWORD, INT64>	m_mapCost;

};

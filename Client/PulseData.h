#pragma once
#include "..\WorldDefine\pulse_define.h"
#include "basedata.h"
class PulseData : public BaseData
{
public:
	PulseData(void);
	~PulseData(void);

	static PulseData* Inst();
	VOID LoadFromFile();
	// IDΪ����
	tagPulseClientProto* FindPulse(DWORD dwID) {return FindInMap(m_mapPulse, dwID);}

	// posΪ����
	tagPulseClientProto* FindFirePulseByPos(DWORD dwPos) {return FindInMap(m_mapFire, dwPos);}
	tagPulseClientProto* FindWaterPulseByPos(DWORD dwPos) {return FindInMap(m_mapWater, dwPos);}
	tagPulseClientProto* FindThunderPulseByPos(DWORD dwPos) {return FindInMap(m_mapThunder, dwPos);}
	tagPulseClientProto* FindWindPulseByPos(DWORD dwPos) {return FindInMap(m_mapWind, dwPos);}
	tagPulseClientProto* FindElementPulseByPos(DWORD dwPos){return FindInMap(m_mapElement, dwPos);}

	// SkillIDΪ���� 7λ��
	tagPulseClientProto* FindPulseBySkillID(DWORD dwID) {return FindInMap(m_mapPulseSkillIDIndex, dwID);}

private:
	map<DWORD, tagPulseClientProto> m_mapPulse;	// IDΪ����
	map<DWORD, tagPulseClientProto> m_mapPulseSkillIDIndex;	// SkillIDΪ����

	map<DWORD, tagPulseClientProto> m_mapFire;		// posΪ����
	map<DWORD, tagPulseClientProto> m_mapWater;	// posΪ����
	map<DWORD, tagPulseClientProto> m_mapThunder;		// posΪ����
	map<DWORD, tagPulseClientProto> m_mapWind;	// posΪ����
	map<DWORD, tagPulseClientProto> m_mapElement;	// posΪ����
	
};

#pragma once
#include "basedata.h"
#include "..\WorldDefine\family_define.h"


class FamilyProtoData : public BaseData
{
public:
	FamilyProtoData(void);
	~FamilyProtoData(void);
	VOID LoadFromFile();
	static FamilyProtoData* Inst();
public:
	// common----------------------------------------------------------------------------------------------------

	// �����Ա----------------------------------------------------------------------------------------------------

	// ��������----------------------------------------------------------------------------------------------------
	tagFamilyQuestName* FindQuestName(DWORD dwID) {return FindInMap(m_mapFamilyQuestName, dwID);}
	const tagFamilyQuestAttProto* FindQuestAttProto(DWORD dwID) {return FindInMap(m_mapFamilyQuestAttProto, dwID);}
	const tagFamilyQuestRewardProto* FindQuestRewardProto(DWORD dwID){return FindInMap(m_mapFamilyQuestRewardProto, dwID);}
	// ��������----------------------------------------------------------------------------------------------------
	const tagFamilySpriteProto* FindFamilySpriteProto(DWORD dwID){return FindInMap(m_mapFamilySpriteProto, dwID);}
	// ���ﲻ��const ��Ϊ���ܻ��޸Ĳ�������
	tagSpriteTrainClientProto* FindSpriteTrainProto(DWORD dwID){return FindInMap(m_mapFamilySpriteTrain, dwID);}
	const map<DWORD, tagSpriteTrainClientProto>& GetTrainMap(){return m_mapFamilySpriteTrain;}
private:
	// common----------------------------------------------------------------------------------------------------

	// �����Ա----------------------------------------------------------------------------------------------------

	// ��������----------------------------------------------------------------------------------------------------
	map<DWORD, tagFamilyQuestName> m_mapFamilyQuestName;				//���������
	map<DWORD, tagFamilyQuestAttProto> m_mapFamilyQuestAttProto;		//���ϱ�
	map<DWORD, tagFamilyQuestRewardProto> m_mapFamilyQuestRewardProto;			//������

	// ��������----------------------------------------------------------------------------------------------------
	map<DWORD, tagFamilySpriteProto>	m_mapFamilySpriteProto;
	map<DWORD, tagSpriteTrainClientProto>	m_mapFamilySpriteTrain;
};

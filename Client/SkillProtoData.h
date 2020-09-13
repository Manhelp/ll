#pragma once

#include ".\BaseData.h"
#include "..\WorldDefine\trigger_define.h"
#include "..\WorldDefine\skill_define.h"
#include "..\WorldDefine\buff_define.h"

#define MAX_CONTINUE_SKILL_NUM 4
//���Ӽ��ṹ
struct tagContinuteSkill
{
	DWORD		dwTypeID;							//���Ӽ����typeid	
	INT			nTarget;							//�����Ŀ��
	DWORD		dwSkill[MAX_CONTINUE_SKILL_NUM];	//��Ӧbuff�����5λid
};

/** \class	SkillProtoData
	\brief	���ܡ�״̬��������ԭʼ���Խṹ�洢
*/
class SkillProtoData
	: public BaseData
{
public:
	SkillProtoData(void);
	~SkillProtoData(void);

	static SkillProtoData* Inst();

	/** \���������ļ�
	*/
	virtual void LoadFromFile();
	/** \��ȡ���ܻ����ṹ
	*/
	const tagSkillProtoClient* FindSkillProto(DWORD dwID)
	{
		return FindInMap(m_mapSkillProto, dwID);
	}
	/** \��ȡ״̬�����ṹ
	*/
	const tagBuffProtoClient* FindBuffProto(DWORD dwID)
	{
		return FindInMap(m_mapBuffProto, dwID);
	}
	/**	\��ȡ�����������ṹ
	*/
	const tagTriggerProto* FindTriggerProto(DWORD dwID)
	{
		return FindInMap(m_mapTriggerProto, dwID);
	}
	/**	\��ȡ���Ӽ������ṹ
	*/
	const tagContinuteSkill* FindContinuteSkillProto(DWORD dwID)
	{
		return FindInMap(m_mapConSkillProto, dwID);
	}

	/**	\��ȡ���ܺ���
	*/
	const tagSkillBoxProto* FindSkillBoxProto(DWORD dwID)
	{
		return FindInMap(m_mapSkillBox, dwID);
	}

	/**	\��ȡ���ܵ�ͼ
	*/
	const tagSkillableMaps* FindSkillMapProto(DWORD dwID)
	{
		return FindInMap(m_mapSkillMaps, dwID);
	}
private:
	/** \���Ӽ�������
	*/
	VOID AddSkillRoleAtt(tagSkillProto& sSkill, ERoleAttribute eType, INT nValue);
    /** \����buff����
    */
    VOID AddBuffRoleAtt(tagBuffProto& sBuff, ERoleAttribute eType, INT nValue);
	
private:
	map<DWORD, tagSkillProtoClient>		m_mapSkillProto;		// ����ԭʼ�ṹ
	map<DWORD, tagBuffProtoClient>		m_mapBuffProto;			// ״̬ԭʼ�ṹ
	map<DWORD, tagTriggerProto>			m_mapTriggerProto;		// ������ԭʼ�ṹ
	map<DWORD, tagContinuteSkill>		m_mapConSkillProto;		// ��������ԭʼ�ṹ
	map<DWORD, tagSkillBoxProto>		m_mapSkillBox;			// ���ܺ���ԭʼ�ṹ
	map<DWORD, tagSkillableMaps>		m_mapSkillMaps;		// �����ͷŶ�Ӧ�ĵ�ͼ
};
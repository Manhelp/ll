#pragma once
#include "..\WorldDefine\creature_define.h"
#include ".\BaseData.h"

enum ETagNode
{
	ETN_Start,
	ETN_Face = ETN_Start,	// �沿
	ETN_Hat,				// ͷ��
	ETN_Back,				// ����
	ETN_RSld,				// �Ҽ�
	ETN_LSld,				// ���
	ETN_RHd,				// ����
	ETN_LHd,				// ����
	ETN_Wep1, 				// ������
	ETN_Wep2,				// ������
	ETN_Wep3,				// ����1
	ETN_Wep4,				// ����2
	ETN_Wep5,				// ����
	ETN_Wep6,				// ������
	ETN_Num,
};
const tstring g_strTagNodeName[ETN_Num] = 
{
	_T("tag_Face"),       // �沿
	_T("tag_Hat"),        // ͷ��
	_T("tag_Back"),       // ����
	_T("tag_RSld"),       // �Ҽ�
	_T("tag_LSld"),       // ���
	_T("tag_RHd"),        // ����
	_T("tag_LHd"),        // ����
	_T("tag_Wep1"),       // ������
	_T("tag_Wep2"),       // ������
	_T("tag_Wep3"),       // ����1
	_T("tag_Wep4"),       // ����2
	_T("tag_Wep5"),       // ����
	_T("tag_Wep6"),       // ������
};

/** \����ģ���������
*/
struct tagCreatureMdlData
{
	float   fBeAttackSfxScale;		// ��������Ч����ϵ��
	bool    bCanPitch;				// �Ƿ���
	tstring szDeadSound;			// ������Ч
	tstring szCombatSound;			// ����ս����Ч
	tstring szRoarSound;			// ����������Ч
	tstring szHowlSound;			// ���򺰽���Ч
	tstring szFootSound;			// �Ų�����ǰ׺����׺Ϊ�������ͣ�
	tstring szNormalAttackBeAtkSfx;	// ��ͨ�����Է���������Ч
	Vector3 vec3Camera;				// �۲�ģ�͵������λ��
	float   fMoveSpeedFactor;		// ģ�Ͳ������ܶ���ʱ�Ĳ����ٶ�
};

/** class	CreatureData
	brief	���NPC���������ԭʼ���Խṹ�洢
*/
class CreatureData 
	: public BaseData
{
public:
	CreatureData(void);
	~CreatureData(void);
	
	static CreatureData* Inst();
	
	/** ���������ļ�
	*/
	virtual void LoadFromFile();
	/** ��ȡNPCģ���ļ�·��
	*/
	static tstring GetNpcMdlPath(const tagCreatureProto *pNpcAtt);
	/**	��ȡNPC��Ƥ�ļ�·��
	*/
	static tstring GetNpcSkinPath(const tagCreatureProto *pNpcAtt, int index);
	/** ����NPC��ԭʼ����
	*/
	const tagCreatureProto* FindNpcAtt(DWORD attID)
	{
		return FindInMap(m_creatureMap,attID);
	}
	/** ��ȡ���е�NPCԭʼ����
	*/
	void GetNpcMap(map<DWORD,tagCreatureProto>& npcMap)
	{
		npcMap = m_creatureMap;
	}
	/** ��ȡģ���������
	*/
	const tagCreatureMdlData* FindMdlData(const TCHAR* szMdlName);
	/** ��ȡ��������
	*/
	const tstring FindNpcBoneName( const DWORD dwTypeID, const TCHAR* szTagNodeName );
	const tstring FindNpcBoneName( const TCHAR* szMdlName, const TCHAR* szTagNodeName );
	/**��ȡ���ֵ
	*/
	INT FindCreatureInspiration(DWORD dwTypeID);
private:
	DWORD MakeNpcBoneNameKey( const TCHAR* szMdlName, const TCHAR* szTagNodeName );
private:
	map<DWORD,tagCreatureProto>			m_creatureMap;
	map<DWORD,tagCreatureMdlData>		m_mdlData;
	map<DWORD,tstring>					m_npcBoneNameMap;
	map<DWORD,INT>						m_mapInspiration;
};

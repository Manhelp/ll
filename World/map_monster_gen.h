/********************************************************************
	created:	2012/04/17
	created:	17:4:2012   11:36
	filename: 	e:\code\World\map_monster_gen.h
	file path:	e:\code\World
	file base:	map_monster_gen
	file ext:	h
	author:		zhangzhihua
	
	purpose:	����ˢ�¹�����
*********************************************************************/
#pragma once
#include "../WorldDefine/time.h"
#include "world.h"

class Creature;
class Unit;

enum EMonsterGenState
{
	EMGS_UPDATE = 0,		// ˢ��
	EMGS_START,				// ��ʼ
	EMGS_WAITING,			// �ȴ�
	EMGS_STOP,				// ֹͣ
	EMGS_CONTINUE,			// ����
	EMGS_NOTHING,			// ʲôҲ����

	EMGS_END,
};

class MapMonsterGenMgr
{
public:
	MapMonsterGenMgr();
	~MapMonsterGenMgr();

public:
	BOOL					Init(const Map* pMap);
	VOID					Destroy();
	Map*					GetMap();
	tagMapMonsterGen*		GetNextMonsterGenTable();

	BOOL					SetState(EMonsterGenState eState);
	EMonsterGenState		GetState();

	DWORD					GetID();
	DWORD					GetWave();

	VOID					DoUpdate();				// ˢ��
	VOID					DoStart();				// ��ʼˢ��
	VOID					DoStop();				// ����ˢ��
	VOID					DoWating();				// ����ȴ�
	VOID					DoContinue();			// ���������ˢ��

	VOID					Update();
	VOID					OnCreatureDie(Creature* pCreature, Unit* pKiller);// ��������

private:
	struct tagMGenInfo
	{
		WORD				wDeadMonster;			// ɱ����������
		tagMapMonsterGen*	pMapMonsterGenTB;		// ����ˢ�±��еĶ�Ӧ������
		TList<Creature*>	listCreature;
		tagMGenInfo()
		{
			wDeadMonster = 0;
			pMapMonsterGenTB = NULL;
			listCreature.Clear();
		}
	};
	typedef TMap<DWORD, tagMGenInfo*>			MonsterGenRecordMap;			// ˢ�ּ�¼

	DWORD					m_dwNextWaveGenTime;	// �²�����ˢ��ʱ��
	DWORD					m_dwNextMonsterTime;	// ��һ���ֵ�ˢ��ʱ��
	WORD					m_wGenNum;				// �������ﵱǰ�Ѿ�ˢ�µ�����
	tagMapMonsterGen*		m_pMapMonsterGenTB;		// ����ˢ�±��еĶ�Ӧ������

	DWORD					m_dwCurTime;			// ��ǰʱ��
	DWORD					m_dwWaitingTime;		// ����ȴ�ʱ���

	EMonsterGenState		m_eState;				// ״̬����
	Map*					m_pMap;
	MonsterGenRecordMap		m_mapMonsterGenRecord;	// ˢ�ּ�¼
};
















//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: shielding_mgr.h
// author: Jason
// actor:
// data: 2010-12-15
// last:
// brief: ����npc����Ʒ���䡢�����䷽��
//-------------------------------------------------------------------------------

#ifndef SHIELDING_MGR_H
#define SHIELDING_MGR_H

class ShieldingMgr
{
	typedef std::set<DWORD> TMaskSet;
	TMaskSet 	_NPCShieldingSet;
	TMaskSet 	_ItemShieldingSet;
	TMaskSet 	_TaskShieldingSet;
	TMaskSet 	_FormulaShieldingSet;
	
	static ShieldingMgr * _create();
	static void _destroy(ShieldingMgr *& );
	const TMaskSet & _get_shieldingset(INT nType) const;
	void _push_shieldingitem(INT nType,DWORD dwParam);
	
	ShieldingMgr();
	~ShieldingMgr();
public:
	VOID Init(VarContainer * pVar,LPCTSTR field);

	static ShieldingMgr * Create();
	static VOID FinalRelease();

	enum { SMBlock_NPC = 0, SMBlock_Item = 1,  SMBlock_Formula = 2, SMBlock_Task = 3 };
	BOOL IsBlocked(INT nType,DWORD dwQueryID) const;
};

#define ShieldMgrCall( Func, Param ) \
	do { ShieldingMgr * p = ShieldingMgr::Create(); \
		if( P_VALID(p) ) 				\
		{							\
			p->Func Param ;			\
		}							\
	} while(0)

#define ShieldMgrCallWithRetValue( Func, Param, Ret ) \
	do { ShieldingMgr * p = ShieldingMgr::Create(); \
		if( P_VALID(p) ) 				\
		{							\
			Ret = p->Func Param ;	\
		}							\
	} while(0)

class Unit;
extern BOOL IsShieldingNPC( Unit * pUnit );
extern BOOL IsShieldingItem( DWORD dwTypeID );
extern BOOL IsShieldingTask(DWORD dwTaskID);
extern BOOL IsShieldingFormula(DWORD dwFormulaID);


#endif // SHIELDING_MGR_H


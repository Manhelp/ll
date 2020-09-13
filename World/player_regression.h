/********************************************************************
	created:	2010/01/18
	created:	18:1:2010   18:33
	filename: 	e:\loong\trunk\LoongWorld\player_regression.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	player_regression
	file ext:	h
	author:		Jason
	
	purpose:	
*********************************************************************/

#ifndef player_regression_h__
#define player_regression_h__

#include "player_regression_define.h"
#define __USE_JM_TOOL_TEMP
#include "../ServerDefine/mini_tool.h"
#undef  __USE_JM_TOOL_TEMP
#include "../vEngine/util/util.h"

using namespace jm_tool;



class PlayerRegression
{
	typedef std::map<DWORD,tagPlayRegressionRule> TRegMap;
	TRegMap m_mapRules;
	tagPlayRegressionRule m_curRule;
	Util m_util;
	CSCreatedLock m_cs;
	LONG volatile  m_lOpen;
	LONG volatile  m_lCounter;
	//DWORD	m_dwNPC_ID;
	//DWORD	m_dwMapID;
public:
	VOID ClearRules();
	INT		Size()
	{
		return m_mapRules.size();
	}
	// ���̰߳�ȫ������������att_res.init�е��ã������ط�����
	VOID AddRule(VarContainer * pVar,LPCTSTR field);
	BOOL Initial();	// 
	BOOL IsOpen() const //��һع��Ƿ����ˣ����ܲ�ֹһ��Ӵ
	{
		return m_lOpen;
	}
	VOID Open(BOOL b){::InterlockedExchange((LPLONG)&m_lOpen,b);}
	BOOL IniRole(Role * pRole); // �жϽ�ɫ�Ƿ������ȡ�ع齱����������ԣ��ͷ�����ȡ��Ϣ���ڽ�ɫ��ʼ��ok�󣬵���
	BOOL IniPlayer(PlayerSession * pRole); // �жϽ�ɫ�Ƿ������ȡ�ع齱����������ԣ��ͷ�����ȡ��Ϣ���ڽ�ɫ��ʼ��ok�󣬵���
	BOOL IniMap(Map * pMap); //ˢnpc
	VOID UpdateMap(Map * pMap); // ���£������ջ��ʼʱˢ���֣������ʱ��ɾ����

	BOOL CanRecRegression(Role*pRole);
	BOOL CanRecRegression(tagDWORDTime timeLogout,BYTE byBackFlag,UINT16 days,BYTE byLevel);

	DWORD RoleGotoNpc(Role * pRole);
	DWORD RewardPlayerBack(Role * pRole);

	PlayerRegression();
	~PlayerRegression();
};

typedef JMSingleton<PlayerRegression> TPlayerRegression;

#endif // player_regression_h__


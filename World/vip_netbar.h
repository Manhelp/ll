//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: vip_netbar.h
// author: xlguo
// actor:
// data: 2009-8-3
//		2010-11-23 �޸Ľ����������� Jason
// last:
// brief: ��������
//-------------------------------------------------------------------------------
#pragma once
#include <vector>


class Role;
//-------------------------------------------------------------------------
// ip��
//-------------------------------------------------------------------------
class IpRange
{
public:
	IpRange(DWORD dwIpMin, DWORD dwIpMax, DWORD dwVNBId)
		:m_IPMin(dwIpMin), m_IPMax(dwIpMax), m_dwVNBId(dwVNBId){}
	BOOL	Fit(DWORD dwIP)			const	{	return dwIP >= m_IPMin && dwIP <= m_IPMax;	}
	BOOL	OnLeftOf(DWORD dwIP)	const	{	return dwIP > m_IPMax;	}
	BOOL	OnRightOf(DWORD dwIP)	const	{	return dwIP < m_IPMin;	}
	DWORD	GetVNBId()				const	{	return m_dwVNBId;		}
	DWORD	GetIpMin()				const	{	return m_IPMin;			}
	DWORD	GetIpMax()				const	{	return m_IPMax;			}
private:
	DWORD	m_IPMin;
	DWORD	m_IPMax;
	DWORD	m_dwVNBId;
};


struct tagDBVNBPlayers;
struct tagVipNetBar;

//-------------------------------------------------------------------------
// �������ɹ�����
//-------------------------------------------------------------------------
class VipNerBarMgr
{
	typedef TMap<DWORD, tagVipNetBar*>	VipNetBarMap;
	typedef std::vector<IpRange>		IpRangeVector;
	typedef std::set<DWORD>				AccountIDSet;
	typedef TMap<DWORD, DWORD>			IP2VNBIdMap;
	typedef std::set<DWORD>				NotifySet;
	std::set<DWORD> _INetbarBuffs;
	std::set<DWORD> _INetbarInstBuffs;

public:
	BOOL	Init();
	VOID	Destroy();
	VOID	InitData(tagDBVNBPlayers* pInitData, BOOL bFirst=FALSE);
	VOID	PlayerLogin(DWORD dwAccountID, DWORD dwIP);
	VOID	PlayerLogout(DWORD dwIP);
	VOID	PlayerNotify(DWORD dwAccountID);
	INT		GetRate(DWORD dwIP, INT nType);
	LPCTSTR	GetVNBName(DWORD dwIP);

	BOOL	RecReward(Role* pRole);
	BOOL	AddInstBuff(Role * pRole);
	BOOL	RemoveInstBuff(Role * pRole);
	BOOL	IsInVipNetbar(Role* pRole,DWORD * pdwBuff = NULL,DWORD * pdwInstBuffID = NULL);
	BOOL	IsOpen() const { return m_bOpen; }
	BOOL	IsVipNetbarBuff(DWORD dwBuffID);
	BOOL	IsVipNetbarInstBuff(DWORD dwInstBuffID);

	VOID	ReloadVipNetbarFromLoginSever(VOID * pLoginMsg,INT num);

private:
	DWORD	TransTSIp2DWORD(LPCTSTR szIP);
	tagVipNetBar* GetVipNetBar(DWORD dwIP);
	DWORD	GetVNBId(DWORD dwIp);
	DWORD	FitNetBar(DWORD dwIp);
	VOID	UpdateDbPlayerLogin(DWORD dwAccountID, DWORD dwTime);
	VOID	GeneralzeIP(DWORD &dwIP);

private:
	IP2VNBIdMap			m_mapIp2VNBId;
	VipNetBarMap		m_mapVipNetBars;
	IpRangeVector		m_vecIpRanges;				// ����
	AccountIDSet		m_setHistoryAccountID;
	AccountIDSet		m_setTodayAccountID;
	NotifySet			m_setNotify;		

	BOOL				m_bOpen;
public:
	BOOL DynamicTest(DWORD dwTestNo, DWORD dwArg1, LPCTSTR szArg2);
};



//-------------------------------------------------------------------------
// ���ӽ�������
//-------------------------------------------------------------------------
class OrangeVipNetBarMgr
{
public:

	OrangeVipNetBarMgr():m_nTickToAddBonusPolint(0){}

	~OrangeVipNetBarMgr(){}


	VOID Init(const TCHAR RoleName[X_SHORT_NAME],const CHAR szAccount[X_SHORT_NAME],const DWORD IP);

	//��̬����
public:
	//��
	static VOID OpenOrangeVipNetBar();

	//��
	static VOID CloseOrangeVipNetBar();

	//���ӽ��������Ƿ��
	static BOOL IfOrangeVipNetBarOpen(){ return m_bOrangeVipBar; }

	//�õ����ʱ��
	static INT GetIntervalTime() { return m_nIntervalTm; }

	//�õ�һ�����ʱ��Ӧ�Ӻ�����
	static INT GetBonusPoint() { return m_nBonusPoint; }
private:

	volatile static BOOL		m_bOrangeVipBar;						// �Ƿ�������vip���ɷ�����

	volatile static INT32		m_nIntervalTm;							// �Ӻ�����ļ��ʱ�� ��λ(����)

	volatile static INT32		m_nBonusPoint;							// ÿ�����ʱ��Ӻ��������


public:
	VOID Update();

private:
	//��ָ����ҼӺ�����
	VOID AddBonusPoint();

private:
	INT32						m_nTickToAddBonusPolint; // ��ʱ

	TCHAR						m_szRoleName[X_SHORT_NAME];

	CHAR						m_szAccountName[X_SHORT_NAME];

	DWORD						m_dwIP;

};

extern VipNerBarMgr g_VipNetBarMgr;
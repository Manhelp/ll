//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city.h
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: ����ϵͳ
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/city_define.h"
#include "../ServerDefine/activity_define.h"
#include "city_struggle.h"

struct tagGuildClanValue;

enum EWarTimeType 
{
	EWT_NOT_WARTIME_INTERNAL		= 0,  //����ĩ

	EWT_BEFOR_WAR		= 1,  //������ĩ ��սǰ
	EWT_WARING			= 2,  //������ĩ ��ս��	
	EWT_AFTER_WAR		= 3,  //������ĩ ��ս��

};


class FareCompare
{
public:
	bool operator()(const tagCitySignUp* lhs, const tagCitySignUp* rhs)
	{
		return lhs->nFare > rhs->nFare;
	}
};

class ReputationCompare
{
public:
	bool operator()(const tagGuildClanValue& lhs, const tagGuildClanValue& rhs);
};

class Map;
class Guild;

class City
{
public:
	typedef TMap<DWORD, tagCityItemOutput*>		MapItemOutput;		// ��Ʒ���͡�������Ϣ
	typedef std::map<DWORD, tagCitySignUp*>		MapSignUp;			// ����ID��������Ϣ

public:
	City();
	~City();

	// ��ʼ�������¡�����
	BOOL	Init(DWORD dwCityID);
	VOID	Update(const tagDWORDTime &tm);
	VOID	Destory();

	BOOL	IsInitOK()		{ return m_bInitOK; }

	// �����ݿ������������
	BOOL	LoadCityInfo(tagCityAtt2DB* pCityInfo, INT nSignUpNum, tagCitySignUp* pSignUpInfo);

//-------------------------------------------------------------------------
// �������Բ���
//-------------------------------------------------------------------------
public:
	// ��ȡ��������
	DWORD	GetCityAtt(Role* pRole, tagCityAtt2Client* pCityInfo);
	VOID	GetCityAppointedAtt(ECityAttType eType, INT& nDataNum, LPVOID pData);
	INT		GetCityAppointedAttSize(ECityAttType eType);

	Map*	GetShellCity()		{ return m_pShellCity; }
	Map*	GetKernelCity()		{ return m_pKernelCity; }

	// ͳ��Ȩ���
	DWORD	ChangeCityHolder(DWORD dwGuildID);

	// ���в���
	VOID	CityProduce();

	// ����������
	DWORD	UpgradeProlificacy(Role* pRole, EProlificacyUpgrade eType);

	// ����˰��
	DWORD	ModifyTaxRate(Role* pRole, INT32 nTaxRate);

	// ��ȡ˰��
	DWORD	TransformTaxation(Role* pRole, ETaxTransformType eType, INT32 nTaxation);

	// todo:�ػ������(����?)

public:
	// ȡ�ó�������
	ECityGateType GetCityGateType();

	// ������
	BOOL	IncCityDefence(INT32 nDefence);
	BOOL	DecCityDefence(INT32 nDefence);

	// �ػ������϶�
	BOOL	IncEudemonTally(INT32 nEudemonTally);
	BOOL	DecEudemonTally(INT32 nEudemonTally);

	// ˰��
	BOOL	ExchangeCityTaxRate(INT32 nTaxRate);

	// ˰��
	BOOL	IncCityTaxation(INT32 nTaxation);
	BOOL	DecCityTaxation(INT32 nTaxation);

	// ������
	BOOL	IncCityProlificacy(INT32 nProlificacy);
	BOOL	DecCityProlificacy(INT32 nProlificacy);

private:

	VOID	MineUpdate(const tagDWORDTime &tm);
	VOID	DoHouseKeepingForSigupInfo();

	// ���浽���ݿ�
	VOID	SaveCityAtt2DB();
	VOID	SaveAppointedAtt2DB(ECityAttType eType);

//-------------------------------------------------------------------------
// ��ս���
//-------------------------------------------------------------------------
public:
	// �õ�������Ϣ
	VOID	GetCitySigupInfo(Role *pRole,tagNetCmd* pCmd);

	// �õ���ս��Ϣ
	VOID	GetCityWarInfo(Role *pRole,tagNetCmd* pCmd);

	// ����
	DWORD	SignupForCitywar(Role* pRole, ECityForceType eForce, INT32 nFare);
	DWORD	SignupForCitywar(DWORD dwGuildID, ECityForceType eForce, INT32 nFare);

	// ����Ȩȷ��
	DWORD	ConfirmAttackForce();
	VOID	ConfirmAttackForce(INT nNum, tagGuildClanValue* pClanValue);

	// ����Ȩȷ��
	DWORD	ConfirmDefenceForce(Role* pRole, DWORD dwGuildID[MAX_CITY_DEFENCE_FORCE]);

	// ������ս�������
	BOOL	GetConfirmGuild(ECityForceType eForce, INT& nNum, DWORD* pGuildID);

	// ������Ϣ���
	BOOL	IsAlreadySignup(DWORD dwGuildID, ECityForceType eForce);

	// ���ó�ս״̬
	VOID	SetCityStruggleState(ECityStruggleState eState);

	// ���븴�����
	VOID	AddToReviveMap(Role* pRole);

	// ���ø���ȴ�ʱ��
	VOID	SetReviveTime(ECityForceType eForce, INT nSec);

	// �����ػ����ٻ�ʱ��
	VOID	SetSummonTime(INT nSec);

	// ��ȡ�����Ӫ
	BOOL	GetForceType(Role* pRole, ECityForceType& eForce);

	// ��սʱ������
	EWarTimeType	GetWarTimeType(const tagDWORDTime &tm);

	//�Ƿ񱻰���ռ��
	BOOL IsGuildOccupied() {return P_VALID(m_pGuild);}

	// ��ȡ��ս����
	CityStruggle&	GetCityStruggle()	{ return m_CityStruggle; }

private:
	// ʱ���ж�
	BOOL	IsCityWarInterval(const tagDWORDTime &tm);

private:
	//ԭ������
	const tagCityProto*	m_pProto;

	BOOL			m_bIsCityWarInterval;

	BOOL			m_bInitOK;

	// ��������ǵ�ͼ��Ϣ
	Map*			m_pShellCity;
	Map*			m_pKernelCity;

	// ͳ������Ϣ
	Guild*			m_pGuild;

	// ��������
	DWORD			m_dwCityID;			// ����ID
	ECLanType		m_eClanType;		// ��������

	INT32			m_nDefence;			// ������
	INT32			m_nEudemonTally;	// �ػ������϶�

	INT32			m_nTaxRate;			// ˰��
	DWORD			m_dwTaxRateTime;	// ˰�ʸ���ʱ��
	INT32			m_nTaxation;		// ˰��
	INT32			m_nProlificacy;		// ������

	INT32			m_nFundOutput;		// �����ʽ����
	INT32			m_nMaterialOutput;	// �����ʲĲ���
	MapItemOutput	m_mapItemOutput;	// ��Ʒ����

	DWORD			m_dwMinSigupFare;	// ��ͱ�������
	// ��ս����
	MapSignUp		m_mapAttackForce;	// ����������Ϣ
	MapSignUp		m_mapDefenceForce;	// �ط�������Ϣ

	// ��ս
	CityStruggle	m_CityStruggle;

	// ��ս��ʼʱ��
	tagWarTime		m_WarTm;

	// ��ս�����
	EActMode		m_ActMode;

	BOOL m_bSigupConfirmed;

	BOOL m_bSigupInfoClear;

	BOOL m_bOutputItemed;

};
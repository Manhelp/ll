#pragma once
#include "player.h"
#include "../WorldDefine/time.h"

class FSM_LP;
struct tagNS_GetRoleInitState_Att;

/** \class LocalPlayer
	\brief ������ҽ�ɫ
*/
class LocalPlayer : public Player
{
public:
	LocalPlayer(void);
	virtual ~LocalPlayer(void);

	//--Role
	virtual void InitFSM();
	virtual void Update();
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual bool IsDead();

	//--Player
	virtual bool IsReadyState();
	virtual bool IsStandState();
	virtual bool IsStyleActionState();
	virtual bool IsIdleState();
	virtual EActionState GetActionState();
	virtual void ReplayLowerAni();
	/** \���ؽ�ɫ�Ƿ���׷��״̬
	*/
	virtual bool IsMoveState();

	//--��ɫ�������
	/** \��ȡ��д�����Ͷ��
	*/
	INT GetAttPointAdd(ERoleAttribute eType){return m_rolepointadd[MTransERAAttA2Index(eType)];}
	VOID SetAttPointAdd(ERoleAttribute eType, INT nValue);
	/** \��ȡ���ԭʼһ������
	*/
	INT GetOriginAtt(ERoleAttribute eType);
	/** \������������Գ�ʼ��
	*/
	VOID InitAttribute(tagNS_GetRoleInitState_Att* pCmd);
	/** \��ȡ��д����ҵ�ǰ����
	*/
	INT64 GetRoleLevelExp(){return m_rolecurlevelexp;}
	VOID SetRoleLevelExp(INT64 nValue, INT64 nInc = 0, INT nFairyExp = 0, INT nFairyLvCha = 0,bool bShare = false );

	virtual void SetPlayerClass(EClassType eClass);

	//--
	/** \���ؽ�ɫ�Ƿ���ս��״̬
	*/
	bool IsCombatState();
	/** \���ؽ�ɫ�Ƿ���ʹ�ü���״̬
	*/
	bool IsSkillState();
	/** \���ؽ�ɫ�Ƿ�����Ӿ״̬
	*/
	bool IsSwimState();
	/** \���ؽ�ɫ�Ƿ���ˮ������״̬
	*/
	bool IsPursueState();
	/** \���ؽ�ɫ�Ƿ����ƶ�״̬
	*/
	bool IsOnWaterState();
	/**  ��ͨ��������״̬
	*/
	bool IsNormalSkillState();
	/** \���ý�ɫ����״̬
	*/
	void SetRoleStateEx(DWORD dwValue);
	/** \���ý�ɫ����״̬
	*/
	void SetRoleStateEx(ERoleStateEx eType, bool bFlag);
	/** \��ȡ��ɫ����״̬
	*/
	bool GetRoleStateEx(ERoleStateEx eType){return (m_roleStateEx&eType) != 0;}
	/** \�л���ͼ���ͺ��������ꡢ�����
	*/
	void OnGotoMap(const Vector3& pos,const Vector3 faceTo);

	/** \��ҵĸ�������
	*/
	const tagDWPlayerPersonalSet& GetPersonalSet() const { return m_personalSet; }
	void SetPersonalSet( const tagDWPlayerPersonalSet& pps ) { m_personalSet = pps; }

	/** \����,����һ�����ж���
	*/
	void DontPlayEaseSkeleton();

	/** \����,ǿ��ת����idle״̬, Ϊ����ϵͳ���
	*/
	void ChangeUpperToIdle();

	/** \��ǰ״̬�Ƿ�Ϊidle
	*/
	bool IsUpperIdleState();

	/** 
		\ �õ�����ʹ�õ���Ʒ�� typeid�������ǰ����ʹ����Ʒ״̬����0
	*/
	DWORD GetUsingItemTypeID();

	void SetMiraclePrcNum(INT16 nNum);
	INT16 GetMiraclePrcNum() const { return m_n16MiraclePrcNum; }

	void SetSoarSkillLearnTimes(INT16 nNum);
	INT16 GetSoarSkillLearnTimes() const { return m_nSoarSkillLearnTimesToday; }

	/*	\������Ч��Ŀǰ��Ϊ������Ч
	*/
	void UpdatePlayerEffect();

	bool IsHookState();

	void SetHookState(bool state);

	void LoadEquipEffect(const tagAvatarEquip& ep);

	//  [8/4/2010 zhangsu] �糡��Ѱ·
	void SetRePathFinding(bool bReFind,DWORD npcID = 0 ,tstring strMapName = _T(""));
	void KeepMoving();

	bool IsCrazyState();

	void SetVipDays(tagDWORDTime nNum);
	tagDWORDTime GetVipDays() const { return m_nVipDays; }

	void SetVipDaysCanLast(const INT& day){m_nVipMaxDay = day;}
	INT GetVipDaysCanLast() const { return m_nVipMaxDay;}
	VOID SetLPActiveDegree(INT nDegree){ m_nLPActiveDegree = nDegree;}
	INT	GetLPActiveDegree(){ return m_nLPActiveDegree;}
protected:
	FSM_LP*					m_pFSM;
	INT						m_rolepointadd[X_ERA_ATTA_NUM];
	INT64						m_rolecurlevelexp;
	DWORD					m_roleStateEx;
	INT16                   m_n16MiraclePrcNum;
	INT16					m_nSoarSkillLearnTimesToday;

	bool					m_bHooking;

	tagDWPlayerPersonalSet  m_personalSet;

	bool					bNeedRePathFinding;
	DWORD					dwReFindNPCid;
	tstring					m_strMapName;
	tagDWORDTime			m_nVipDays;		// vip��ʼ��ʱ��
	INT						m_nVipMaxDay;	// vip�ܳ���������
	INT						m_nLPActiveDegree; //������Ҽ����Ծ��

	Vector3							m_v3OldPos;

	DECL_RTTI(LocalPlayer);

};



#pragma once
#include "player.h"
#include "AvatarEquipNode.h"

//class Role;
class FSM_RP;

#define VR_HEIGHT_OFFSET		(-15.0f)
#define VR_X_Z_OFFSET			(60.0f)

enum EPLayActionType
{
	EPAT_NULL,			//�޶���
	EPAT_MOVE,			//�ƶ�����
	EPAT_FIGHT,			//��ս����
	EPAT_STAND,		//վ������
	EPAT_CALL,			//�ٻ�����
	EPAT_DISPPEAR,	//��ʧ����
};

enum EPLaySoundType
{
	EPST_NULL = -1, 			//---------------------������

	//��ʱ��������
	EPST_Fighting			=	0, 		//����
	EPST_Winning			=	1, 		//ʤ��
	EPST_Leisure				=	2, 		//����
	EPST_BeAttack			= 3,		//������
	EPST_TimeEnd			=	3,		//------ʱ�������Ž�β
	//��ʱ����
	EPST_Disppear			=	4,		//��ʧ
	EPST_Calling				=	5, 		//�ٻ�
	EPST_TypeNum			= 6,		//------������Ŀ

};

struct tagVirtualRoleEvent : public tagGameEvent
{
	EPLayActionType  ePlayAct;
	tagVirtualRoleEvent()
	:tagGameEvent(_T("tagVirtualRoleEvent"),NULL)
	,ePlayAct(EPAT_NULL)
	{}
	tagVirtualRoleEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		,ePlayAct(EPAT_NULL)
	{}
};

//��ɱ��Ϣ
struct tagVRSkillEvent : public tagGameEvent
{
	tagVRSkillEvent():tagGameEvent(_T("tagVRSkillEvent"),NULL)
	{}
	tagVRSkillEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
	: tagGameEvent(szEventName,pSenderFrame)
	{}
};


/** \class VirtualRole
\brief �����ɫ
��������ģ���������Ч��
*/
class VirtualRole :	public Player
{
private:
	

public:
	VirtualRole(void);
	virtual ~VirtualRole(void);

	//--Role-------------------------------------------------------------------
	virtual void Update();
		
	virtual void InitFSM(DWORD dwRoleID);
	
	BOOL IsInitFSM() const { return m_bIsInitFSM;} 

	void OnGotoMap(const Vector3& pos,const Vector3 faceTo);

	void OnGameEvent(tagGameEvent* pGameEvent);
	void OnNetCmd(tagNetCmd* pNetCmd);

	virtual void Hide( const bool bHide );

	//ͬ�������ɫ��������ս����
	VOID SynCombatAction(tstring strActionName);

	//���û��������ɫID
	VOID SetHolyManID( DWORD dwID) { m_dwVRID = dwID; }
	DWORD GetHolyManID() { return m_dwVRID; }

	VOID	SetState(EPLayActionType eType, BOOL bForce = FALSE);

	/*	\�������ɫ���� ---���������ɫ
	*/
	bool		LoadVirtualAvatar( tstring strResName);
	VOID   LoadVirtualEffect( tstring strName );
	//�ٻ�ʱ��ʼ��Ԫ��ģ��
	VOID CallHolyManInit();
private:
	//--���ó���
	BOOL RotateVRole( Vector3 srcPos,  Vector3 dstPos );
	//���Ŷ���
	VOID  PlayAction();
	//ͬ��λ��
	VOID SynchPos();
	//ˢ����Ч����
	VOID UpdateSound();
	//���ó���
	VOID FaceToTarget( Player* pSelf,Role* pTarget,BOOL bMoveable, BOOL bAniBlend );
	//�������
	const TCHAR* GetVRMoveTrackName( float speed );
	//������Ч
	VOID PlayVRSound( EPLaySoundType eType );
	//���Ա����Ч��Ŀѡ�����������Ч  
	//  nMaleRandNum,   nFemaleRandNum �ɲ߻��ṩ����Դ��Ŀ����
	VOID PlayRandSound( INT nSound, INT nMaleRandNum, INT nFemaleRandNum );
	//��ʾģ����Ч
	VOID ShowVREffect();
	//����ʾģ����Ч
	VOID CloseVREffect();
	//���ñ������Ԫ����Ч
	VOID SetLocalSoundType( EPLaySoundType eType );
private:
	TObjRef<AudioSys>		m_pAudioSys;
	TObjRef<Util>				m_pUtil;

	DWORD					m_dwVRID;				//�����ɫģ��ID(Ԫ��ID)

	DWORD					m_dwRoleID;				//�������id

	BOOL						m_bIsInitFSM;			//fsmʱ����ʼ����
	
	EPLayActionType		m_eActionType;		//������������

	EPLaySoundType		m_eSoundType;		//��Ч��������

	DWORD					m_dwTimer/*[EPST_TypeNum]*/; //��Чʱ��

	DWORD					m_dwEscapeTime;

	tstring						m_strCombatAction;

	tstring						m_strActionName;

	std::map<tstring, tstring>		m_mapVREffect;	//�����ɫ��Ч��<��Ч�󶨹���, ��Ч����>

	DECL_RTTI(VirtualRole);
};

#pragma once
#include "..\WorldDefine\msg_combat.h"
#include "CombatEvent.h"
#include "GUIEffectMgr.h"

namespace Cool3D
{
	class EffectTarget;
};
class Role;
class Player;

/** \class EffectMgr
	\brief ��Ч������
*/
class EffectMgr
{
public:
	/** \��Ч����
	*/
	enum EEffectType
	{
		EET_Common,         // ��ͨ��Ч
		EET_Role,           // ��ɫ��Ч
		EET_Launch,         // ������Ч
		EET_Buff,           // Buff��Ч
		EET_BeAttack,       // ��������Ч
		EET_Bone,			// �����󶨵���Ч
		EET_UiSound,		// ������Ч����Ԫ��
		EET_Win,				// �����Ч
		EET_VirtualRole,       // �����ɫ��Ч��Ԫ��
	};
	/** \��Ч����������
	*/
	enum EEffectScaleType
	{
		EEST_Null,           // ������
		EEST_Y,				 // ���ݸ߶�����
		EEST_RoleScale,      // ʹ�ý�ɫ��������������Ч
		EEST_XYZ,			 // ʹ��X,Y,Z��ƽ��ֵ����
		EEST_YOutOnly,       // ���ݸ߶ȷŴ󣬲���С
		EEST_XZ,			 // ʹ��X,Z��ƽ��ֵ����
	};

	/** \��Ч����
	*/
	enum EBoneEffectType
	{
		EBET_Head,         // ͷ
		EBET_Chest,        // ��
		EBET_Neck,         // ����
		EBET_Mantle,       // ����
		EBET_Wrist_R,        // ����
		EBET_Wrist_L,
		EBET_Waist,        // ����
		EBET_Leg_R,		   // ��
		EBET_Leg_L,	
		EBET_Foot_R,		   // ��
		EBET_Foot_L,
	};

private:
	/** \��Ч��������
	*/
	struct tagBaseEffectData
	{
		const EEffectType       eType;                // ��Ч����
		SGAttachableEffectNode*	pSGNode;              // ��ЧScene node
		DWORD				    dwBindRoleID;         // ��Ч�󶨵Ľ�ɫID�����û�а󶨽�ɫ��ID==GT_INVALID��
		bool                    bBindTagNode;		  // �Ƿ���˹��ص�����
		EEffectScaleType		eScaleType;           // ��������
		bool                    bPullOnWater;         // �����ˮ�£�����ˮ������
		tagBaseEffectData( EEffectType type ) : eType( type ), pSGNode( NULL ), dwBindRoleID( GT_INVALID ), bBindTagNode( false )
			, eScaleType( EEST_Null ), bPullOnWater( false ){}
	};
	typedef map<DWORD, tagBaseEffectData*> EffecMap;

	/** \��ɫ��Ч
	*/
	struct tagRoleEffectData : public tagBaseEffectData
	{
		tagRoleEffectData() : tagBaseEffectData( EET_Role ) {}
	};

	/** \������Ч
	*/
	struct tagLaunchEffectData : public tagBaseEffectData
	{
		tagHitTargetEvent evtHitTarget;
		bool              bEventSended;
		tagLaunchEffectData() : tagBaseEffectData( EET_Launch ), bEventSended( false ) {}
	};

	/** \Buff��Ч
	*/
	struct tagBuffEffectData : public tagBaseEffectData
	{
		DWORD dwBuffID;								// BUFF ID
		tagBuffEffectData() : tagBaseEffectData( EET_Buff ), dwBuffID( GT_INVALID ) {}
	};

	// �����Ч
	struct tagWinEffectData : public tagBaseEffectData
	{
		DWORD dwSrcRoleID;
		DWORD dwTarRoleID;

		tagWinEffectData() : tagBaseEffectData( EET_Win ),dwSrcRoleID(GT_INVALID),dwTarRoleID(GT_INVALID) {}
	}; 

	/** \��������Ч
	*/
	struct tagBeAttackEffectData : public tagBaseEffectData
	{
		DWORD					dwSrcRoleID;	//������ID
		DWORD					dwTarRoleID;	//Ŀ��ID	
		EHitTargetCause			eCause;			//����Ŀ��ԭ��
		DWORD					dwMisc;			//����Ǽ��ܣ�Ϊ����ID;�������Ʒ����ƷID
		DWORD					dwMisc2;		//�ܵ����
		DWORD					dwSerial;		//���ܻ�ʹ����Ʒ���к�
		tagBeAttackEffectData() : tagBaseEffectData( EET_BeAttack ) {}
	};

	/** \�����ɫ��Ч
	*/
	struct tagVirtualRoleEffectData : public tagBaseEffectData
	{
		tagVirtualRoleEffectData() : tagBaseEffectData( EET_VirtualRole ) {}
	};


	/** \��ɫ������Ч
	*/
	struct tagBoneEffectData : public tagBaseEffectData
	{
		EBoneEffectType		eType;		 // λ��
		bool				bIsBoneNode; 
		tstring				szBoneName;  //��������
		tagBoneEffectData() : tagBaseEffectData( EET_Bone ) {}
	};

	/** \UIsound��Ч
	*/
	struct tagUISoundEffectData : public tagBaseEffectData
	{
		GUIEffectMgr::EEffectType eSoundType;								// ��������
		tagUISoundEffectData() : tagBaseEffectData( EET_UiSound ), eSoundType( GUIEffectMgr::EET_Common ) {}
	};

	enum { EFFECT_START_ID = 1 };
	enum { MIN_EFFECT_NUM = 30, MAX_EFFECT_NUM = 150 };  // ������Чϸ���Լ���ǰ��Ч��������ĳЩ��Ч����

public:
	EffectMgr(void);
	~EffectMgr(void);

	/** \��ʼ������ս��ϵͳ����
	*/
	VOID Init();
	
	/** \�ͷ����ݣ���ս��ϵͳ����
	*/
	VOID Destroy();
	
	/** \������Ч����ս��ϵͳ����
	*/
	VOID Update();

	/** \������Ч��ָ��λ�á����š���ת
	*/
	DWORD PlayEffect(const TCHAR* szPath, const Vector3& vPos, const Vector3& vScale, const Vector3& vRot );
	
	/** \��ָ��λ�ò���һ����Ч
	    \szFolderName-��Ч�����ļ������ƣ����ΪNULL������ΪszEffectName������Ч��·��
	*/
	DWORD PlayEffect(const Vector3& vPos,float yaw,const TCHAR* szEffectName, const Vector3* pSize=NULL, EEffectScaleType eScaleType = EEST_Null, const TCHAR* szFolderName = _T("combat") );
	
	/** \����һ����ɫ��Ч,������ЧID,����ʧ�ܷ���GT_INVALID
	*/
	DWORD PlayRoleEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL, EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	
	/** \����һ�������ɫ��Ч,������ЧID,����ʧ�ܷ���GT_INVALID
	*/
	DWORD PlayVirtualEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL, EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	
	/** \ֹͣ���������ɫ�ϵ�������Ч
	*/
	VOID StopVirtualEffect(DWORD dwRoleID);

	/** \���ŷ�����Ч,������ЧID
	*/
	DWORD PlayLaunchEffect(const tagHitTargetEvent& evt,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL);
	
	/** \����nNumEffect��szEffectBaseName�������һ����������Ч��������ЧID
	*/
	DWORD PlayBeAttackEffect(DWORD dwRoleID,const TCHAR* szEffectBaseName,const int nNumEffect, 
		DWORD dwSrcRoleID, EHitTargetCause eCause, DWORD dwMisc, DWORD dwSerial, EEffectScaleType eScaleType = EEST_YOutOnly );
	
	/** \����dwRoleID,dwSrcRoleID��ָ���ļ���ֹͣ���Ž�ɫ���иü��ܲ����ı�������Ч
	*/
	VOID StopBeAttackEffect(DWORD dwRoleID, DWORD dwSrcRoleID, EHitTargetCause eCause, DWORD dwMisc, DWORD dwSerial);
	
	// ���Ż����Ч
	DWORD PlayWinEffect( DWORD dwFromRoleID, DWORD dwTargetID, const TCHAR* szEffectName );

	/** \����һ��Buff��Ч,������ЧID
	*/
	VOID PlayBuffEffect(DWORD dwRoleID,DWORD dwBuffID, EEffectScaleType eScaleType = EEST_Y );
	
	/** \ֹͣһ��Buff��Ч
	*/
	VOID StopBuffEffect(DWORD dwRoleID,DWORD dwBuffID);
	
	/** \ֹͣһ����Ч
	*/
	VOID StopEffect( const DWORD dwEffectID );

    /** \ɾ��һ����Ч
    */
    VOID RemoveEffect(const DWORD dwEffectID);

	/** \ɾ�������г��ﶨ�ڱ������֮���������Ч
	*/
	VOID ClearSenceEffect();

	/** \��ʾ\������Ч
	*/
	VOID ShowEffect( const DWORD dwEffectID, const bool bShow );
	
	/** \��ʾ\���ذ��ڽ�ɫ���ϵ�������Ч
	*/
	VOID ShowBindRoleEffect( const DWORD dwBindRoleID, const bool bShow );

	/** \��ȡ��������Ч������
	*/
	size_t GetNumEffect() const { return m_mapEffect.size(); }
	/** \��Ч�����Ƿ��Ѿ��ﵽ���ޣ���Щ����Ҫ����Ч�Ϳ��Բ����ţ�
	*/
	bool                 IsEffectNumFull();

	void PlayCacheBuff( DWORD dwRoleID );

	/*	\��Ч�󶨵����� add by zsu
	*/
	DWORD PlayRoleEquipEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szBoneName=NULL, EBoneEffectType eType = EBET_Mantle,EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	VOID StopRoleEquipEffect(DWORD dwRoleID,EBoneEffectType eType);
	const tstring GetGlowName( const TCHAR* szBaseName, const BYTE byVal ) const;

	/*  \ui��Чϵͳ�����ʹ�õ�������Ч�ļ�
	*/
	DWORD PlayUiSoundEffect(const TCHAR* szEffectName,GUIEffectMgr::EEffectType eType,const TCHAR* szFolderName = _T("ui"));
	VOID StopUiSoundEffect(GUIEffectMgr::EEffectType eType);

	static EffectMgr* Inst();

private:

	SGAttachableEffectNode*		LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName = _T("combat") );
	DWORD						AddEffect( tagBaseEffectData* pData, Role* pBindRole, const TCHAR* szTagNodeName );
	VOID						DeleteEffect( tagBaseEffectData* pData );    // ����ɾ��map�еĽڵ�
	VOID						OnCloseMap( tagGameEvent* );
	BOOL                        UpdataRoleEffect( tagBaseEffectData* pData );
	VOID                        UpdataTargetPath( tagLaunchEffectData* pData, const float fTimeDelta );
	VOID                        PullOnWater( tagBaseEffectData* pData );
	Vector3						GetRoleEffectScale( const EEffectScaleType eScaleType, Role* pRole );
	float 						GetEffectScale( const EEffectScaleType eScaleType, const Vector3& vSize );
	Vector3						GetBeAttackPos( Role* pRole );
	bool                        SFXCanMove( const TCHAR* szFileName );
	VOID                        UpdataEffectMatrix( tagBaseEffectData* pData, Role* pRole );

	DWORD						AddBoneEffect( tagBoneEffectData* pData, Role* pBindRole, const TCHAR* szBoneName );
	VOID                        UpdataBoneEffectMatrix( tagBaseEffectData* pData, Role* pRole );
	VOID						UpdateBeAtkEffectMsg(tagBeAttackEffectData* pData);
	VOID						UpdataVirtualEffectMatrix( tagBaseEffectData* pData, Player* pRole, const TCHAR* szBoneName );

	// �����Чʹ�õ����λ��
	Vector3						GetWinEffectPos( Role* pRole, bool bTar );
	void						UpdateTargetPath4Win( tagWinEffectData* pData, const float fTimeDelta );

private:
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TSFPTrunk<EffectMgr>		m_Trunk;
	EffecMap					m_mapEffect;				// ��Ч����
	DWORD						m_dwCurEffectID;            // ��¼��ǰ��ЧID

	// �ڽ�ɫ���Գ�ʼ��Ϊ��ɵ�ʱ���buffӦ�û��棬�����Գ�ʼ�����Ҫһ����ʾ����
	std::map<DWORD,DWORD>		m_mapNeedProcessBuff;
};

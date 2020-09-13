#pragma once

#include "GUICameraData.h"

class GUIEffectMgr
{
public:
	//! ��Ч����
	enum EEffectType
	{
		EET_Common,			// ��ͨ��Ч
		EET_HP,				// ����ֵ
		EET_MP,				// ħ��ֵ
		EET_Anger,			// ŭ��
		EET_Enforcing,		// ǿ����
		EET_EnforcSucess,	// ǿ���ɹ�
		EET_EnforcFail,		// ǿ��ʧ��
		EET_ComposSucess,	// �ϳɳɹ�
		EET_ComposFail,		// �ϳ�ʧ��
		EET_Sprite,			// ������Լ����
		EET_ShengLingCall,	// ʥ���ٻ�
	};

	//! ��Ч����������
	enum EEffectScaleType
	{
		EEST_Null,           // ������
		EEST_Y,				 // ���ݸ߶�����
		EEST_XYZ,			 // ʹ��X,Y,Z��ƽ��ֵ����
		EEST_XZ,			 // ʹ��X,Z��ƽ��ֵ����
	};

	//! ���ģʽ����
	enum EEffectBlendMode
	{
		EEBM_None = 0,		
		EEBM_Alpha_InAlpha,	// alpha���
		EEBM_One_One,		// 1:1���
	};

private:
	//! ��Ч��������
	struct tagBaseEffectData
	{
		EEffectType				eType;			// ��Ч����
		EEffectScaleType		eScaleType;     // ��������
		SGAttachableEffectNode*	pSGNode;        // ��ЧScene node
		GUIWnd*					pWnd;			// Ҫ��Ⱦ��Ŀ��λ��, ����
		tagRect					destRect;		// Ҫ��Ⱦ��Ŀ��λ��
		DWORD					dwHandle;		// �Լ�����Ⱦ����,ֻ�н�����Чʹ���Լ��Ļ�������ȫ����Ч����
		EEffectBlendMode		eBlendMode;		// ��Ч�Ļ��ģʽ
		tstring					strEffectName;

		tagBaseEffectData( EEffectType type ) : 
		eType( type ), pSGNode( NULL ) , eScaleType( EEST_Null ),pWnd(NULL),dwHandle(GT_INVALID),eBlendMode(EEBM_One_One),strEffectName(_T(""))
		{
		}
	};
	typedef map<DWORD, tagBaseEffectData*> EffecMap;

public:
	GUIEffectMgr(void);
	~GUIEffectMgr(void);

	//! ��ʼ������ս��ϵͳ����
	VOID Init();

	//! �ͷ����ݣ���ս��ϵͳ����
	VOID Destroy();

	//! ������Ч����ս��ϵͳ����
	VOID Update();

	//! ��FrameMgr.render3d ��ʱ����Ⱦ
	void Render3D();

	//! ��������Ч����ɫ�����ϵ�������
	void Render();

	//! ������Ч��ָ��λ�á�����
	DWORD PlayGuiEffect( const TCHAR* szEffectName, GUIWnd* pWnd, tagRect& rect,const Vector3& vPos, const Vector3& vScale, const TCHAR* szEffectFolder = _T("ui") ,bool bReSetCamera = false);
	//! ͨ�������ļ���ȡ��Ч��Ϣ
	DWORD PlayGuiEffect( const TCHAR* szEffectName, GUIWnd* pWnd,EEffectType eType= EET_Common,const TCHAR* szEffectFolder = _T("ui"));
	DWORD PlayScreenEffect( const TCHAR* szEffectName, const tagPoint& ptView, const tagPoint& ptSize, const Vector3& vPos, const Vector3& vScale, const TCHAR* szEffectFolder = _T("ui"),bool bReSetCamera = false );
	//! ͨ�������ļ���ȡ��Ч��Ϣx
	DWORD PlayScreenEffect( const TCHAR* szEffectName,const tagPoint& ptView, const tagPoint& ptSize,EEffectType eType= EET_Common,const TCHAR* szEffectFolder = _T("ui"));

	//! ����alpha��Ч
	DWORD PlayScreenEffect_a( const TCHAR* szEffectName,const tagPoint& ptView, const tagPoint& ptSize,EEffectType eType= EET_Common,const TCHAR* szEffectFolder = _T("ui"));
	
	//! ֹͣһ����Ч
	VOID StopEffect( const DWORD dwEffectID );
	//ֹͣ�����ϵ�ĳ����Ч,������Ч����ֹͣ�����ϵ�������Ч
	VOID StopGuiEffect( GUIWnd* pWnd , const TCHAR* szEffectName = NULL );

	VOID StopEffectByType( EEffectType eType );

	VOID RemoveEffect(const DWORD dwEffectID);

	VOID RemoveOneEffectByType(EEffectType eType);

	//! ��ʾ\������Ч
	VOID ShowEffect( const DWORD dwEffectID, const bool bShow );
	// �����Ƿ��Ѿ�������
	bool WndIsHide(GUIWnd* pWnd);

	VOID SetUICamera( const Vector3& vPos, const Vector3& vLookAt);

	VOID CloseAllEffect();

	VOID	SetLoadingFlag( BOOL b ){ m_bLoading = b; }

	static GUIEffectMgr* Inst();

private:
	SGAttachableEffectNode*		LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName = _T("ui") );
	DWORD						AddEffect( tagBaseEffectData* pData );
	VOID						DeleteEffect( tagBaseEffectData* pData );    // ����ɾ��map�еĽڵ�
	

private:
	TObjRef<AudioSys>		m_pAudioSys;

	IGUIRender*				m_pRender;
	GUIWnd*					m_pDeskTop;
	NullSceneGraph*			m_pSG;					 // ���ڱ�������������յ���Ϣ,��������Ⱦ
	CameraBase*				m_pCamera;				 // ���ڸ��������
	tagGUIImage*			m_pCanvasImg;			 // ����

	EffecMap				m_mapEffect;			 // ��Ч����
	DWORD					m_dwCurEffectID;         // ��¼��ǰ��ЧID
	GUICameraData			m_CameraData;
	BOOL						m_bLoading;	 //loading������
};

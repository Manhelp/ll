#pragma once

/** ��ת��ͷ��Ϸ�¼�
*/
struct tagRotateCameraEvent : public tagGameEvent
{
	tagRotateCameraEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

class Role;
/**	\class ClientCamera 
	\brief �ͻ��˾�ͷ����
*/

/** ������켣���� */
//@{
struct TCamPathNode {
	float mSpeed;		/**< ������ƶ��ٶ� */
	Vector3 mPos;		/**< ·��λ�� */
	Quaternion mRot;	/**< ������ƶ����ýڵ�ʱ�ĳ��� */
	bool mUseShortestPath;/**< ����̻���ת */
	TCamPathNode():mSpeed(0),mUseShortestPath(true){}
};

enum ECamreaControlState
{
	ECCS_Null,
	ECCS_Roam,			// ����״̬
	ECCS_UserContril,	// �û��������״̬
};
class ClientCamera : public CameraEuler
{
public:
	VOID SetDefault(FLOAT fZFar);
	VOID SetFov(float deg);

	VOID Update(Role *pLookRole,NavMap *pNavMap);
	
	VOID PlayQuake();	//! ��ͷ��

	VOID EnableFlyMode(bool enable)	{m_bFly=enable;}
	bool IsFly() { return m_bFly; }

    VOID SetHeightRate(FLOAT rate) { 
        m_HeightScaleRate = rate;
    }
	VOID SetStopUpdate(bool bStop)	{StopUpdate=bStop;}
	bool IsStopUpdate(){ return StopUpdate;}
	
	// �����������
	// ���ı�������Ŀ���ģʽ
	VOID SetCamCtrlState(ECamreaControlState eState) {m_eCamCtrlState = eState;}

	// ��ѹ��������ڵ�����
	VOID AddCamPoint(TCamPathNode node){m_vecCamPathNodes.push_back(node);}

	// ������ɾ������
	VOID GenerateCamPathData();


	ClientCamera();

private:
	TObjRef<InputDX8>			m_pInput;
	TObjRef<Console>			m_pConsole;
	TObjRef<GameFrameMgr>		m_pFrmMgr;

	POINT m_ptLast;
	
	float m_fExpectDist;		// ��������
	float m_fOccultDist;		// ��ͷ����סʱ��dist
	DWORD m_dwLastOccultTime;	// ��ͷ�ϴα���ס��ʱ��
	

	bool m_bFly;				// �Ƿ��Ƿ���ģʽ
	bool m_bRotating;			// �����������ת��ͷ

	bool m_bQuaking;			// �Ƿ���о�ͷ�𶯴���
	DWORD m_dwQuakeStartTime;	// ��ͷ�𶯿�ʼʱ��

	BOOL m_bDecTransparent;		
	FLOAT m_fTransparent;

    FLOAT m_HeightScaleRate;    //! ��ͷLookAt�ĸ߶����ű��� add by hyu

	virtual void Occult(NavMap *pNavMap);
	void UpdateQuake(Vector3& nextLookAt);

	void UpdateRoleTransparent();

	//�� ��������ι�����Ҫ
	bool				StopUpdate;			
	bool				mbLoop;				/**< �Ƿ�ѭ�����Ź켣���� */
	ECamreaControlState m_eCamCtrlState;	// ������ܿ�״̬
	SimpleSpline		mCurve;				//��������position ��������
	RotationalSpline	mRotCurve;			//�������������������Ԫ����ת����������
	std::vector<TCamPathNode> m_vecCamPathNodes;

	/** ���������·�㶯�� */
	VOID PlayPathAni( bool loop );
	VOID StopPathAni();
	VOID UpatePathAni( float deltaTime );
	/** ���ù켣����һ���������������durationΪ-1����ʾ������ʧЧ�� */
	VOID SetPlayDuration( float duration );

	float mCurAniTime;/**< ��ǰ����ʱ�䣨�룩*/
	float mTotalDuration;/**< ����һ�ι켣�����������ʱ�� */
};






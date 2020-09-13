#pragma once

/** \class MapMgr
	\brief ��Ϸ����������
*/

struct	PromptInfo
{
	POINT pt;
	tstring desc;
	DWORD mapID;
};
class MapMgr
{
public:
	enum{MAX_VIEW_DIST=8};
	typedef map<DWORD,list<PromptInfo>> MAPPROMPT;
public:
	MapMgr(void);
	~MapMgr(void);

	/** ��ʼ����������Ϸ����ʱ����
	*/
	void Init();
	/** �ͷ����ݣ��뿪��Ϸ����ʱ����
	*/
	void Destroy();

	void OpenMap(const TCHAR* szMapName,DWORD dwMapID,CameraBase* pBindCamera);
	void CloseMap();

	DWORD GetCurMapID(){ return m_curMapID;}
	const tstring& GetCurMapName() { return m_curMapName; }

	/** \Զ����Ұ
	*/
	void SetViewDist(int viewDist);
	int GetViewDist(){ return m_viewDist;}
	/** \����ϸ��
	*/
	void SetSceneDetailLevel(int level);
	int GetSceneDetailLevel(){ return m_sceneDetailLevel;}
	/** \̫������
	*/
	void EnableSunFlare( const bool bEnable );

	void Update();
	DWORD OnGameEvent(tagGameEvent* pEvent);
	DWORD OnEnterLoadingFrame(tagGameEvent* pEvent);
	DWORD OnExitLoadingFrame(tagGameEvent* pEvent);

	/** ͬ�����¿�����
	*/
	void SyncInvalidViewZone();

	SceneGraph* GetSceneGraph(){ return m_pSG;}
	GameMap& GetGameMap(){ return m_gameMap;}
	NavMap*  GetNavMap() { return &m_navMap; }

	int		GetMapAreaIndex( const Vector3& vPos, const Vector3& vSize, EMapArea eAreaType = EMA_Null );
	DWORD	GetCurTriggerID() const;
	BOOL	GetTriggerPos(DWORD questSerial,Vector3& pos,AABBox& box);
	BOOL	GetTriggerPos(DWORD triggerID, Vector3& pos);
	static MapMgr* Inst();
	//! �����ӽ�2.5D
	void SetCamera25D( bool b25D );
private:
	void SetViewZoneSize();
	void UpdateTriggers();
	void UpdateMapArea();
	void ResetSceneFog( const bool bFade );
	/*
	*	ע���Ӧ��ͼ����ʾ��ϢmapID�ǵ�ͼ��ID����CRCֵ��PromptInfo�����ж�Ӧ�������Ҫ��ʾ����Ϣ
	*/
	//void RegisterMapPrompt(DWORD mapID,PromptInfo mapInfo);
	void InitMapPrompt(DWORD mapID,float posX,float posZ,tstring desc);
	void PromptMapInit();
	void UpdateMapPrompt();
	BOOL IsInPromptArea(POINT pos);

	/*
	*  ���ִ壬����FPS���͵�����ʾ�޸�Ĭ������
	*/
	void FPScheck();
	void InitFPScheckMap();
	void SendFixFPSsuggestionEvent();

private:
	int						m_viewDist;
	int                     m_sceneDetailLevel;
	bool                    m_bEnableSunFlare;
	HugeQuadTreeSG*			m_pSG;
	GameMap					m_gameMap;
	NavMap					m_navMap;
	DWORD					m_curMapID;
	tstring					m_curMapName;

	vector<HRGN>			m_triggerRgn;			//����������
	vector<HRGN>			m_mapAreaRgn;			//��ͼ����
	bool					m_bInTrigger;			//��ҵ�ǰ�Ƿ��ڴ�������
	int						m_curTriggerIndex;		//��ǰ������ڴ���������
	int						m_curMapAreaIndex;		//��ǰ������ڵ�ͼ��������

	//MAPPROMPT				m_mapPrompt;			//��ͼ������ʾ��Ϣ

	float					m_lastUpdateMapAreaTime;//���һ�θ���MapArea��ʱ��
	int						m_PromptTimeDelta;		//����Prompt��ʱ���ʶλ

	vector<DWORD>			m_vecFPSWatchmaps;		//����FPS�ĵ�ͼ
	int						m_nFPSmark;				//��ǰ��FPS
	bool                    m_bFPSCanShow;			//FPS�Ƿ���ʾ���ı�־λ
	float					m_FPStimeMark;
	bool					m_bIsLoading;
	vector<PromptInfo>		m_vecPrompt;			//��ͼ������ʾ��Ϣ

	TObjRef<NetSession>		m_pZoneSession;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
	TSFPTrunk<MapMgr>		m_trunk;
};

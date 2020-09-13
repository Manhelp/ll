#pragma once

class MiniMapStatic;
struct tagLeaveMapAreaEvent;
struct tagEnterMapAreaEvent;
struct tagSetFlagEvent;
struct tagMouseMoveFinishEvent;
struct tagMouseMoveEvent;
struct tagNS_CanGoToPos;
class  FlashButton;
struct tagSaveTime                                           //��ʱ��¼ʱ��Ľṹ
{
	INT 				savehour;
	INT					saveMin;
	tagSaveTime():		savehour(0), saveMin(0){}
};
/** \class MiniMapFrame
    \brief С��ͼ����
*/
class MiniMapFrame : public GameFrame
{
	//friend class CommKeyCode;//add by xp
public:
	MiniMapFrame();
	~MiniMapFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
private:
	VOID SafeDestroyUI();
	BOOL OnWnd_MaxSize(tagGUIEvent* pEvent);				      //��󻯴����¼���Ӧ����
	BOOL OnWnd_MiniSize(tagGUIEvent* pEvent);				      //��С�������¼���Ӧ����
	BOOL OnWnd_Set(tagGUIEvent* pEvent);				          //С��ͼ���ô����¼���Ӧ����
	BOOL OnWnd_PosQuery(tagGUIEvent* pEvent);					  //�ֶ��������괰���¼���Ӧ����
	VOID ResetMapData();
	/** \��Ϸ�¼���Ӧ
	*/
	DWORD RefreshQueryBtnFlash(tagGameEvent*);
	DWORD OnOpenMap(tagGameEvent*);
	DWORD OnLeaveMapArea( tagLeaveMapAreaEvent* pEvent );
	DWORD OnEnterMapArea( tagEnterMapAreaEvent* pEvent );
	DWORD OnSetFlagEvent( tagSetFlagEvent* pEvent );
	DWORD OnMouseMoveFinishEvent( tagMouseMoveFinishEvent* pEvent );

	/** \�ȼ���Ӧ����
	*/
	DWORD OnZoomIn_HotKey( DWORD dwID, BOOL bDown );
	DWORD OnZoomOut_HotKey( DWORD dwID, BOOL bDown );

	DWORD OnNS_CanGoToPos(tagNS_CanGoToPos* pNetCmd, DWORD dwParam);//�����Ӧ�ֶ���������¼�

	INT EGStoEMME( const INT nEGS ) const;


	bool  CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute); //�Ƚ��Ƿ�Ϊ�ʱ��(for �̶����ť)
	VOID  TaskQueryBtnFlash();		//�̶����ť ��˸�������

	VOID  StartPathfinding();

private:
	TSFPTrunk<MiniMapFrame>	        m_Trunk;
	TObjRef<GameInputMap>			m_pKeyMap;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>			m_pGameFrameMgr;

	GUIWnd*                         m_pWnd_MaxSize;               //С��ͼ��󻯴���
	MiniMapStatic*                  m_pMSt_MiniMap;               //С��ͼ�ؼ�
	GUIButton*                      m_pBn_PosGoTo;                //�ֶ���������Ѱ·���水ť
	GUIStatic*                      m_pSt_MapName;                //��ͼ���ֱ�ǩ
	GUIStatic*                      m_pSt_PosX;                   //���X�����ǩ
	GUIStatic*                      m_pSt_PosZ;                   //���Z�����ǩ
	GUIButton*                      m_pBn_MiniSize;               //��С����ť
	GUIButton*                      m_pBn_Setting;                //С��ͼ���ð�ť
	GUIButton*                      m_pBn_AreaMap;                //�����ͼ��ť
	tagSaveTime                     m_clickTaskQueTime;			  //��¼���������ѯ��ť��ʱ��
	FlashButton*                    m_pBn_TaskQuery;              //������ѯ��ť
	GUIButton*                      m_pBn_TaskTrace;              //����׷�ٽ��水ť
	GUIButton*                      m_pBn_ZoomIn;                 //�Ŵ�ť
	GUIButton*                      m_pBn_ZoomOut;                //��С��ť
	GUIButton*						m_pBn_VipStall;				  //VIP̯λ
	GUIButton*						m_pBn_OnlineTip;			  //������ʾ
	GUIButton*						m_pBn_CityWarInfo;			  // ��վ��Ϣ
	//GUIPushButton*					m_pPBtn_ContinuteSkill;		  //���Ӽ�����   //delete by lib ����ʾ����������ť��ͻ
	GUIButton*                      m_pBn_StyleAction;            //���Զ�����ť
	GUIButton*						m_pBtnQuestHelp;			  //����
	GUIButton*						m_pBtnStrengthRanking;		  // ʵ������
	GUIButton*						m_pBtnKilling;				  // ��ɱ���ְ�
	GUIButton*						m_pBtnShowHideDialyExp;		  //��ʾ����ÿ������
	GUIButton*						m_pBtnPaidashi;					//���İ�ť

	GUIWnd*                         m_pWnd_MiniSize;              //С��ͼ��С������
	//GUIStatic*                      m_pSt_MapName_MiniSize;       //��С�������еĵ�ͼ���ֱ�ǩ
	//GUIStatic*                      m_pSt_PosX_MiniSize;          //��С�������е�X�����ǩ
	//GUIStatic*                      m_pSt_PosZ_MiniSize;          //��С�������е�Z�����ǩ
	GUIButton*                      m_pBn_MaxSize;                //��󻯰�ť

    //GUIButton*                      m_pBt_PetSkillOnline;         // �����ڹ� //<!F�ƻ��иù��ܷ���

	GUIWnd*                         m_pWnd_Set;                   //С��ͼ���ô���
	GUIPushButton*                  m_pBn_EleShow[EMME_Num-1];      //�Ƿ���ʾ��ť
	GUIButton*                      m_pBn_Ok_Set;                 //ȷ��
	GUIButton*                      m_pBn_Cancel_Set;             //ȡ��
	GUIButton*                      m_pBn_Close_Set;              //�ر�

	GUIWnd*                         m_pWnd_PosQuery;                   //�ֶ����������ѯ����
	GUIButton*                      m_pBn_Pos_Ok_Set;                 //ȷ��
	GUIButton*                      m_pBn_Pos_Cancel_Set;             //�ر�
	GUIEditBox*						m_pEd_PosX;						 //X���������
	GUIEditBox*						m_pEd_PosZ;						//Z���������

	tagPoint                        m_ptLocalPlayerPos;
	DWORD							m_dwSpaceTime;

	GUIButton*                      m_pBnFaceBook;                 //ȷ��
	GUIButton*                      m_pBnPickStrategy;                 //ʰȡģʽ
	GUIButton*                      m_pBnSinaWeiBo;                  //����΢��
	GUIButton*                      m_pBnSinaWeiBoCard;         //����΢����
};

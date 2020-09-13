#pragma once

#define RECOMMEND_GAMEWORLD 8
#define BOROUGH_MAX			12
#define GAMEWORLD_MAX		24
#define BOROUGH_MAX_KOREA		14
#define GAMEWORLD_MAX_KOREA		14

class IconStatic;

enum EServerState
{
	ESS_NULL = 0,
	ESS_New,
	ESS_Hot,
	ESS_All
};

struct tagGameWorld											//��Ϸ����
{
	DWORD	dwWorldRank;									//��Ϸ����ı��
	TCHAR	szWorldName[X_SHORT_NAME];						//��Ϸ�������
	TCHAR	szBoroughName[X_SHORT_NAME];					// ������
	TCHAR	szRecommendName[X_SHORT_NAME];					// �Ƽ���������
	DWORD	dwWorldID;										//����ID
	INT		nStatus;										//��Ϸ�����״̬				
	DWORD	dwStatus;										//״̬����ɫ
	DWORD	dwWorldIP;										//��Ϸ�����IP
	DWORD	dwWorldPort;									//��Ϸ�����port
	BYTE	byRecommend;									//�Ƿ�Ϊ�Ƽ���Ϸ����
	DWORD	dwRecoRank;										//�Ƽ���Ϸ����ı��
	BYTE	byGreenFlag;									//��ɫ���
	EServerState    eState;
	TCHAR	szIcon[5][128];									//ͼ��
	TCHAR	szTips[5][128];									//tips	
	TCHAR	szHttp[5][128];									//����
};

struct tagBorough											//����
{
	DWORD					dwBoroughRank;					//��Ϸ����ı��
	TCHAR					szBoroughName[X_SHORT_NAME];	//������
	DWORD					dwBoroughID;					//����ID
	INT						dwWorldNum;						//��������Ϸ�������
	vector<tagGameWorld>	vGameWorld;						//��������Ϸ����
};

struct tagServerState
{
	TCHAR szIcon[128];
	TCHAR szTips[128];
	TCHAR szHttp[128];
};

/** ������������ѡ��
*/

class IconStatic;
class Client;
class ServerSelectFrame : public GameFrame
{
public:
	ServerSelectFrame();
	~ServerSelectFrame();

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	BOOL OnFreeStyleWnd(tagGUIEvent* pEvent);
	// �õ����������ص���Ϸ����״̬��ɫ
	DWORD GetWorldStatusColor(const INT nWorldStatus);

	// ��������
	void CalDate();

	//ȷ�ϵ�¼
	void ConfirmLogin();

	//�ʺ�ע��
	void AccountRegister();

	//�ʺų�ֵ
	void AccountAbundance();

	//�˳���Ϸ
	void ExitGame();

	//��ȡ������Ϸ����������ļ�
	void LoadConfigFile();

	//���ݷ������õ�IP  alex 2010-06-17 
	void GetWorldIDByWorldName(tstring strWorldName,tstring& strIp, tstring& strPort, tstring& strWorldID);

	//��ȡ�ϴε�¼������Ϸ����
	void LoadLastLogin();

	//�����Ƽ���Ϸ����״̬
	void SetRecommedPushButtonState();

	//������Ϸ���簴ť״̬
	void SetWorldPushButtonState();

	//��ʾ��Ϸ���簴ť
	void ShowWorldPushButton();

	//��ʾ����\����Ļ����
	void ShowServerActive(); 

	// ��ʼ����½ģʽ
	void InitLoginMode();

	INT GetBorough( INT nGameWorld );
	INT GetWolrd( INT nGameWorld );

	VOID GetSpecialValue();


private:
	TSFPTrunk<ServerSelectFrame>m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<StreamTransport>	m_pStream;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<VarContainer>		m_pVarIPSaver;
	TObjRef<Client>				m_pClient;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIPushButton*				m_pRecommend[RECOMMEND_GAMEWORLD];	//�Ƽ���Ϸ���簴ť
	IconStatic*					m_pRecIconHot[RECOMMEND_GAMEWORLD];
	IconStatic*					m_pRecIconNew[RECOMMEND_GAMEWORLD];
	GUIPushButton*				m_pBorough[BOROUGH_MAX];			//������ť
	GUIPushButton*				m_pGameWorld[GAMEWORLD_MAX];		//��Ϸ���簴ť
	IconStatic*					m_pWorldIconHot[GAMEWORLD_MAX];
	IconStatic*					m_pWorldIconNew[GAMEWORLD_MAX];
	GUIPushButton*				m_pLastLogin;						//�ϴε�¼����Ϸ���簴ť		
	GUIStatic*					m_pLastServer;						//�ϴε�¼�Ĵ���
	GUIButton*					m_pOK;								//ȷ�ϵ�¼
	GUIButton*					m_pRegister;						//ע��
	GUIButton*					m_pAbundance;						//��ֵ
	GUIButton*					m_pExit;							//�뿪
	//GUIListBox*				m_pToday;							//���ջ
	//GUIListBox*				m_pTomorrow;
	IconStatic*					m_pStcTip[20];						//15��tip
	GUIButton*					m_pBtnHttp;							//����ҳ��ť

	GUIWnd*						m_pWnd_Back;						//�󱳾�
	GUIStatic*					m_pStc_Pic;							//������ͼ

	INT							m_nBorough;							//ѡ��Ĵ�ű��
	INT							m_nGameWorld;						//ѡ�����Ϸ������
	INT							m_nRecoWorld;						//ѡ����Ƽ���Ϸ������
	vector<tagBorough>			m_vBrough;							//����
	vector<tagGameWorld>		m_vRecommend;						//�Ƽ�����Ϸ����
	vector<tagServerState>		m_vTip;								//�����Ƽ�����Ӧ15����ʾ
	TCHAR						szButtHppt[128];					//��ť����		

	tstring						m_strRegister;						//ע���ʺŵ���ַ
	tstring						m_strAbundance;						//�ʺų�ֵ����ַ

	tstring						m_strLastIP;						//�ϴε�¼��ip
	DWORD						m_dwLastPort;						//�ϴε�¼��port
	tstring						m_strLastWorldName;					//�ϴε�¼����Ϸ����
	tstring						m_strLastServer;					//�ϴε�¼�Ĵ���

	tstring						m_strGameWorld;						//ѡ�����Ϸ������
	tstring						m_strIP;							//ѡ�����Ϸ����IP
	DWORD						m_dwPort;							//ѡ�����Ϸ����Port
	
	INT							m_nLoginMode;						// ��½ģʽ
	bool						m_bOnlyGameWord;					// ����ʾ������ֻ��ʾ��Ϸ����

	bool                      m_bIfKoreaServer;                  //�Ƿ��Ǻ���������������ʾ�Ƽ���Ϸ����,�ҽ����б仯����

	//��г��Ϸ���
	GUIWnd*						m_pFreeStyleWnd;                  //���ɶ�ս����
	GUIPushButton*				m_pPBFreeStyleChoose;				//��ѡ���ɶ�ս
	GUIButton*					m_pBtnChoose;						//ȷ�����˳���ť

	INT m_nLauncherValue;
	INT m_nDispValue;

	GUIWnd*						m_p360Wnd;                  //360
	GUIButton*					m_pBn360On;
	GUIButton*					m_pBn360Off;
	GUIPushButton*				m_pBn360Again;

	GUIStatic*					m_pStcKoreaPic;
	DWORD						m_dwKoreaPicShowTime;
	BOOL						m_bKoreaEnterGame;

private:
	BOOL						m_bYunLianOn;
};
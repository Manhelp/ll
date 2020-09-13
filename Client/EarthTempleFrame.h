#pragma once
#include "gameframe.h"
#include "AppliedTree.h"
/** \class EarthTempleFrame

\brief ��������

*/
struct tagSpecItemEvent;
struct tagNS_EarthTemple;
struct tagNS_Skill;

class EarthTempleFrame :
	public GameFrame
{
private:
	struct tagZoneMapShow
	{
		DWORD 				dwMapID;
		tstring             strShowName;
		tagZoneMapShow() :  dwMapID( GT_INVALID ) {}
	};
	struct tagMarkIndex
	{
		int nType;
		int nMask;
		tagMarkIndex() : nType( 0 ), nMask( 0 ){}
		tagMarkIndex( const int type, const int mask ) : nType( type ), nMask( mask ) {}
	};
	struct tagEarthTempleUnit
	{
		DWORD		dwMapID;
		Vector3		position;
		tstring     name;
		tagEarthTempleUnit() : dwMapID(0),name(_T("")){ position = Vector3(0,0,0);}
	};
public:
	EarthTempleFrame(void);
	~EarthTempleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	BOOL IsInSendingState();

private:
	BOOL OnWnd_Temple(tagGUIEvent* pEvent);
	VOID InitMapInfo();
	VOID FillMapListBox();
	VOID ReBuidMarksTree( const DWORD dwMapID );
	DWORD OnOpenEarthTemple(tagSpecItemEvent* pEvent);
	bool CanGoto();
	VOID SendMsg();
	VOID StopSendMsg();
	bool PlayerStateCheck();
	VOID SortUnits();
	DWORD OnNS_EarthTemple(tagNS_EarthTemple* pMsg,DWORD pParam);

	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);

private:

	TSFPTrunk<EarthTempleFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;			//������
	GUIButton*                      m_pBn_Ok_Set;					//ȷ��
	GUIButton*                      m_pBn_Cancel_Set;				//ȡ��
	GUIButton*                      m_pBn_Close_Set;				//�ر�

	GUIComboBox*					m_pCb_Earth_Box;				//��Ͽ�

	AppliedTree*					m_pTree;							

	vector<tagZoneMapShow>          m_vecEarthMapShow;				// �����ͼ�б�

	DWORD 							dwSelectMapID;
	INT64							CurrItemID;		//��ǰ��������ߵ�64ID
	Vector3							m_PosVector3;		//��ǰѡ��ĵص����������

	map<DWORD, tagMarkIndex>        m_mapMarkIndex;       // KEY:ID in AppliedTree

	tagMarkIndex					m_strTempMark;		//Ϊ�����������ص����

	bool							m_bIsSpelling;		//�Ƿ�������

	DWORD							m_dwTimeCounter;

	GUIPushButton*					m_pBnFrist;			//�������һ������
	GUIPushButton*					m_pBnSecond;		//�������2������

	tagEarthTempleUnit				m_strFristUnit;		//�������һ�������Ӧ��Ϣ
	tagEarthTempleUnit				m_strSecondUnit;	//�������2�������Ӧ��Ϣ

	tstring							m_szTempName;		//��ǰ����Ŀ�ĵص�����

	BOOL							m_bDirectOpen;		//�Ƿ��ɼ���ֱ�Ӵ򿪵Ľ���
	
};

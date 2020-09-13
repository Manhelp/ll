/********************************************************************
	created:	2010/09/25
	created:	25:9:2010   17:10
	filename: 	e:\F-Project\src\World\convenient_team.h
	file path:	e:\F-Project\src\World
	file base:	convenient_team
	file ext:	h
	author:		Jason
	
	purpose:	������
*********************************************************************/


#ifndef convenient_team_h__
#define convenient_team_h__

class Map;
class Team;
class Unit;
struct tagNS_GetEasyTeamInfo;
struct tagNS_GetSinglePlayers;
struct tagNS_GetPersonalInfo;
struct tagNC_PutOutInfo;
/**
 * \ingroup World
 *
 *
 *
 * \version 1.0
 * ʵ��ɱ�ֵı�������Ϣ��ʾ����
 * \version 1.1
 * ������ı�������ʾ
 *
 * \date 2010-09-25
 * \date 2010-10-10 ���븱���Լ����ع���
 *
 * \author jpma
 */
class ConvenientTeam
{
	BOOL	m_bOpen;
	BOOL	m_bSendTeamChange;
	ConvenientTeam();
	~ConvenientTeam();

	INT		m_nPlyNumLimit;
	INT		m_nPckLimit;

public:
	struct tagCTInfo
	{
		BYTE byTeamType;
		BYTE byScene;
		BYTE byTeamTypeDetail;
		BYTE byTeamNum;
		BYTE byMinLevel;
		BYTE byMaxLevel;
		DWORD dwRoleID; // ������id�����߶ӳ���������δ���ģ�
		tstring strContent;
	};// 36bytes
private:

	// ��ͼCRCID�볡��id��ӳ�䣬�ڳ�ʼ��ʱ����������룻����ʱֻ�Ƕ�ȡ������Ҫͬ����ȫ���ԡ�
	typedef map<DWORD,vector<INT> >	TMapScene;
	TMapScene	m_MapIDScene;

	// ���id�������Ϣ�����int�Ǹ���ʱ����ʾ������ģ�Ҫ��һ��0-����������m_QzCTInfos��������m_ZdCTInfos
	// ��Ҫ�̰߳�ȫ
	typedef map<DWORD,INT>	TPlayerInfoID;
	TPlayerInfoID	m_PlayerInfoID;

	// ��group_scene.xml��صģ���SceneID,�����Ϣid/��������ġ�
	// ��Ҫ�̰߳�ȫ
	typedef map<INT,vector<INT> >		TSceneCTInfos;
	TSceneCTInfos	m_SceneCTInfos;

	typedef map<INT,tagCTInfo*> TCTInfo;
	// ά�������Ϣ������
	TCTInfo m_ZdCTInfos;
	// ά��������Ϣ������
	TCTInfo m_QzCTInfos;

	tagCTInfo * GetCTInfo(INT ndx); // ���������Ϣ��
	INT	InsertCTInfo(tagCTInfo * pInfo,bool bIsZudui);	// ���������Ϣ��
	VOID	DeleteCTInfo(INT CTInfoNdx,INT nSceneNdx = GT_INVALID);

	BOOL GetZuduiInfo(tagNS_GetEasyTeamInfo*& pMsg,Unit * pUnit,ServerMemPool* pMemPool,set<DWORD> & team_leaders);

public:

	// ������ϵͳ��ʼ��ʱ���ã�������ϵͳ����ʱ���ֱ������õ��������������̰߳�ȫ��
	static ConvenientTeam& GetInstance();
	// ������ϵͳ����ֹǰ���ã��÷����������̰߳�ȫ��
	static VOID Destroy();

	VOID Open(BOOL bOpen){m_bOpen = bOpen;}
	VOID OpenTeamNotice(BOOL bOpen) {m_bSendTeamChange = bOpen;}

	// ɱ�֣��������
	enum ECTType{ ECT_Creature = 0, ECT_Instance, ECT_Activity };

	
	/**	�����ϡ��ͻ�����ȡʱ�ٷ�
	 * ֪ͨ��Ҹ����ģ��Ź����ڣ��Ķ�����Ϣ����Ȼ���ڸ����йֵ�����£�û�ж�����Ϣ���ᷢ
	 * �÷���ֻ���㷨�������������ݣ�������̰߳�ȫ��
	 * \param pMap ��ҵ�ͼ
	 * \param pNewMap ���Ҫȥ���µ�ͼ
	 * \param nOldPos ԭ������Ϣ���������Ƕӳ�����֪ͨԭ�Ź����ڵ���ң�������ʧ��
	 * \param nNewVisTileNdx �¸�����Ϣ�������¸����ڶ�����Ϣ�������
	 */
	VOID OnRoleVisTileChange( Unit * pUnit, Map * pMap,Map * pNewMap,INT nOldPos, INT nNewVisTileNdx );

	enum ETeamEvent{ ETE_New = 0,ETE_Del,ETE_MemAdd,ETE_MemRemove,ETE_ChangeLeader };
	// ��С�����������仯ʱ������
	VOID OnTeamChanged( ECTType eChangeType, INT nEventType,Team * pTeam );

	// ���ͻ�������ʱ���ã�������Ҫ���͵���Ϣ��
	BOOL GetEasyTeamInfo(tagNS_GetEasyTeamInfo*& pMsg,Unit * pUnit,ServerMemPool* pMemPool = NULL);

	// �õ�ɢ����û����ӵ�����б�
	BOOL GetFreePlayers(tagNS_GetSinglePlayers*& pMsg,Unit * pUnit,ServerMemPool* pMemPool = NULL);

	BOOL IsOpen() CONST {return m_bOpen;}

	// ��ʼ���������������ļ���ȡ��������Ϣ
	VOID Init(VarContainer * pVar,LPCTSTR field)	;

	// ��ȡ��������
	BOOL GetPersonalInfo(tagNS_GetPersonalInfo *& pMsg, Unit * ply,ServerMemPool* pMemPool = NULL);
	// ��ҷ������/������Ϣ
	DWORD IssueCTInfo(tagNC_PutOutInfo * pMsg, Unit * ply);

	// ɾ����ӻ��߸���������Ϣ
	VOID DeleteCTInfoOfPlayer(Unit * ply,BOOL bTeamOrNot = TRUE);

	// ���Ʊ������������������������
	VOID LimitTeamInfoNum(INT num);
	VOID LimitPckSize(INT num) {m_nPckLimit = num;}
};



#endif // convenient_team_h__



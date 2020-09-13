#pragma once

enum EPvPHallType;
enum EPvPLevel;

class PvPRoom
{
public:
	typedef struct _RoleInfo
	{
		_RoleInfo(){ memset(this, 0, sizeof(_RoleInfo)); }
		_RoleInfo(const _RoleInfo& a){ dwRoleID=a.dwRoleID; ucPos=a.ucPos; eState=a.eState; }
		DWORD dwRoleID;
		BYTE ucPos;
		E_PvP_RoleState eState;
		TCHAR szName[X_SHORT_NAME];
		DWORD dwLevel;
		DWORD dwWuXun;
	} RoleInfo, *PRoleInfo;

	PvPRoom(E_PvP_RoomType hallType, E_PvP_RoomLevel level, INT16 roomPos);
	~PvPRoom();
	BOOL IsInitialized()const { return m_bInitialized;}
	BOOL NeedUpdate();
	void RefreshUpdateTime();
	void Update();
	void ForceUpdate();
	void Update(const tagPvPRoom& roomInfo);
	void Update(const tagPvPSimRoom& roomInfo);
	RoleInfo* GetRoleInfo(DWORD roleID);
	RoleInfo* GetRoleInfoAtPos(BYTE pos);
	E_PvP_RoomState	GetRoomState()const { return m_eRoomState;}
	E_PvP_RoomType GetHallType()const { return m_eHallType;}
	E_PvP_RoomLevel GetRoomLevel()const { return m_eLevel;}
	INT16 GetRoomID()const {return m_nRoomID; }	
	DWORD GetNumLimit()const {return m_dwNumLimit;}
	DWORD GetCount()const {return m_dwCount;}
	INT	  GetLevelLowerBound()const{ return m_iLevelLowerBound;}
	INT	  GetLevelUpperBound()const{ return m_iLevelUpperBound;}
	DWORD	GetWuXunLimit()const { return m_dwWuXunLimit;}
	DWORD GetManagerID()const { return m_dwManagerID;}
	TCHAR* GetName()const { return (TCHAR*)m_szManagerName; }
private:
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	tagDWORDTime				m_lastUpdate;

	E_PvP_RoomType				m_eHallType;
	E_PvP_RoomLevel				m_eLevel;
	INT16	                    m_nRoomID ;	//�����
	E_PvP_RoomState	            m_eRoomState;			//����״̬
	DWORD                       m_dwNumLimit;		//����������
	INT			                m_iLevelUpperBound;		//����ȼ�����
	INT							m_iLevelLowerBound;		//����ȼ����� 
	DWORD						m_dwWuXunLimit;			//��ѫ����
	DWORD						m_dwManagerID;			//����
	DWORD						m_dwCount;	//��ǰ����	
	BOOL						m_bInitialized;
	TCHAR						m_szManagerName[X_SHORT_NAME];
	TSafeMap<BYTE, PRoleInfo>	m_RoleMap;
};

inline PvPRoom::PRoleInfo PvPRoom::GetRoleInfoAtPos(BYTE pos)
{	
	return m_RoleMap.Peek(pos);
}

inline PvPRoom::PRoleInfo PvPRoom::GetRoleInfo(DWORD roleID)
{
	std::list<PRoleInfo> roleList;
	m_RoleMap.ExportAllValue(roleList);
	for(std::list<PRoleInfo>::iterator itor=roleList.begin(); itor!=roleList.end(); itor++)
	{
		if((*itor)->dwRoleID == roleID) return *itor;
	}
	return NULL;
}
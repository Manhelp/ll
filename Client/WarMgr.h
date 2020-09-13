#pragma once

#include "..\WorldDefine\role_att.h"

#define MAX_War_Role_Num 6	//ս������
struct tagRefreshWarInfo;
struct tagWarnfo
{
	INT AWarInfo[ERoleAttrEnd];
	tagWarnfo()
	{
		for(int i = 0; i < ERoleAttrEnd; i++)
		{
			AWarInfo[i] = 0;
		}
	}
};
struct tagWarResult
{
	DWORD dwItem1;
	DWORD dwItem2;
	DWORD dwItem3;
	BOOL bIsSuccess;
};
class WarMgr
{
public:
	WarMgr(void);
	~WarMgr(void);

	static WarMgr* Inst();
	/** ������Ϸ����ʱ��ʼ��
	*/
	void Init();
	/** �뿪��Ϸ����ʱ�ͷ���������
	*/
	void Destroy();

	void ClearData();

	DWORD OnNS_SyncRoleAttr(tagNS_SyncRoleAttr* pCmd, DWORD);
	DWORD OnNS_SyncRoleAttrToOther(tagNS_SyncRoleAttrToOther* pCmd, DWORD);
	DWORD OnNS_ShowWarResult(tagNS_ShowWarResult* pCmd, DWORD);
	DWORD OnNS_RefreshBaoXiang(tagNS_RefreshBaoXiang* pCmd, DWORD);
	DWORD OnNS_GetWarBaoXiang(tagNS_GetWarBaoXiang* pCmd, DWORD);

	//��ȡ�������ս���������
	DWORD GetWarMoneyNum();
	//��ȡս������map
	const std::map<DWORD, tagWarnfo>& GetWarInfo() {return m_mapWarInfo;};
	//��ȡս�����
	tagWarResult* GetWarResult(){return &m_sResult;}

private:
	TSFPTrunk<WarMgr>			m_trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;

	std::map<DWORD, tagWarnfo>  m_mapWarInfo;
	DWORD						m_dwRoleID;
	tagWarResult				m_sResult;		//ս�����
};
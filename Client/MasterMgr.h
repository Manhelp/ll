#pragma once

//! \class  MasterMgr
//! \brief  ʦͽ������
//! \author hyu
//! \date   2010-1-31
#include "..\WorldDefine\msg_master_apprentice.h"

struct tagStuLevelUpInfo
{
	DWORD dwLevel;				// �ȼ�
	DWORD dwAddJingWu;			// �������ӵ���ҵ����(ʦ��)
	DWORD dwNeedJingWu;			// �������ĵ���ҵ����(�Լ�)
	DWORD dwNeedNum;			// �������ĵĵ��߸���
};

class MasterMgr
{
public:
    MasterMgr(void);
    ~MasterMgr(void);

    void Init();
    void Destroy();
    void Update();

    //! singleton
    static MasterMgr* inst();

    //! show some errorcode
    bool ShowErrMsg(DWORD dwErrCode);

public:
	// ������ҵ����
	void SetJingWuPoint( DWORD dwPoint ) { m_dwMyJingWu = dwPoint; }
	// ����ʦ�µ���
	void SetQinWuPoint( DWORD dwPoint ) { m_dwMyQinWu = dwPoint; }
	// ��ɫ��ҵ����
	DWORD GetMyJingWu() { return m_dwMyJingWu; }
	// ��ɫʦ�µ���
	DWORD GetMyQinWu() { return m_dwMyQinWu; }
	// ����������ҵ����
	DWORD NeedJingWuCurLevel( DWORD dwLevel );
	// ����������Ʒ����
	DWORD NeedItemNumCurLevel( DWORD dwLevel );

private:
	// ��ȡ������
	void LoadStuLevelUpInfoFile();
	
	// �õ���ҵ����
	DWORD OnNetGetJingWuPoint( tagNS_GetJingWuPoint *pMsg, DWORD );

	// ʹ����ҵ����
	DWORD OnUseJingWuPoint( tagMsgBoxEvent *pGameEvent );

	// ͽ�ܳ�ʦ,ʦ��������ʾ
	DWORD OnStuGraduate( tagNS_GraduateNotice *pMsg, DWORD );

private:
    //------------------------------------------------------------------------
    // ����
    //------------------------------------------------------------------------
    TSFPTrunk<MasterMgr>		m_Trunk;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
	TObjRef<VarContainer>		m_pVarContainer;

	DWORD m_dwMyJingWu;			// ��ҵ���ҵ����
	DWORD m_dwMyQinWu;			// ��ҵ�ʦ�µ���
	vector<tagStuLevelUpInfo> m_vecStuLevelUpInfo;		// ��������
};

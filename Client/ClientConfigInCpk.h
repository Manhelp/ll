#pragma once

class ClientConfigInCpk
{
	static ClientConfigInCpk s_clientConfigMgr;
	ClientConfigInCpk(void);
	~ClientConfigInCpk(void);
public:
	VOID Init();
	static ClientConfigInCpk* Inst();
	BOOL IsMallOpen(){return (BOOL)m_dwOpenMall;}
	DWORD GetFaceBookMode(){return m_dwFaceBookMode;}
	BOOL Is360Open(){return (BOOL)m_dw360;}
private:
	TSFPTrunk<ClientConfigInCpk>				m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<VarContainer>           m_pCfgVars;              

	TCHAR                           m_szFilename[MAX_PATH];       //�����ļ���

	DWORD								m_dwOpenMall;				// �̳ǿ��أ�0�ǹأ�1�ǿ�
	DWORD								m_dwFaceBookMode;	// facebook ģʽ  0̨�壬1���
	DWORD								m_dw360;	// 360 ��ʾ
};

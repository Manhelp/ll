#pragma once

class SaveAccount 
{
public:
	SaveAccount(void);
	~SaveAccount(void);

	static SaveAccount* Inst();
	void SetAccount(const TCHAR* szAccount);
	tstring& GetAccount() { return m_account; }
	void SetWorldID(const TCHAR* world_id){ m_world_id = world_id; }
	tstring& GetWorldID() { return m_world_id; }

	void SetWorldName(const TCHAR* world_name){ m_world_name = world_name; }
	tstring& GetWorldName() { return m_world_name; }

	void SetLoginID(const DWORD dwLoginID){ m_dwLoginID = dwLoginID; }
	DWORD GetLoginID() { return m_dwLoginID; }

	INT	GetCurMusic() { return m_nMusic; }
	void SetCurMusic(INT nMusic) { m_nMusic = nMusic; }

	void GoBack(bool b){m_bGoBack = b;}
	bool	IsGoBack(){return m_bGoBack;}

	int	GetLoginMode(){return m_nCurLoginMode ;}
	void SetLoginMode(int mode){ m_nCurLoginMode = mode ;}

	void SetAccountID(DWORD id){ m_dwAccountID = id;}
	DWORD GetAccountID(){return m_dwAccountID;}

	void SetCropID(DWORD id){ m_dwCropid = id;}
	DWORD GetCropID( bool bCommandLine = TRUE );

	void SetIsNeedSetTelNum(BOOL bIsSetTelNum) { m_bIsNeedSetTelNum = bIsSetTelNum; }
	BOOL GetIsNeedSetTelNum() { return m_bIsNeedSetTelNum; }

	// ������� ------------------------------------------------------------------------------------------------ 
	//���YY��½����Ӫ��ID���������YY��½����0
	DWORD GetYYCropID();
	// �Ƿ��������������Ŀͻ��ˣ�����ǲ�洢�����������Ϣ
	bool HaveCommandLine(char* cmd);
	// ���ô�YY��õ�GUID��������½
	void SetYY_GUID(string ss){m_yyGUID = ss;}
	// �����Ƿ���ͨ��YY��½
	void SetYYLoginMode(bool b){m_bIsYYlogin = b;}
	bool IsYYLoginMode(){return m_bIsYYlogin;}
	string& GetYY_GUID(){return m_yyGUID;}
	// ��ý�ɽ����������еĲ�����Ϣ
	string& GetJSKK_Channel(){return m_jskkPm;}
	// open passport��� 
	VOID  SetChargeUrl(string  str);
	string  GetChargeUrl(){ return m_ChargeUrl; }
	VOID  SetOpenPassport(BOOL b){ m_bIsOpenPassPort = b; }
	BOOL IsOpenPassport(){ return m_bIsOpenPassPort; }
	const std::vector<DWORD>&  GetCropIDInfo();

	void SetYunLianOn(BOOL bOn){m_bYunLianOn = bOn;}
	BOOL GetYunLianOn(){return m_bYunLianOn;}
private:
	tstring				m_account;
	tstring				m_world_id;
	tstring				m_world_name;

	DWORD				m_dwLoginID;

	INT					m_nMusic;
	bool					m_bGoBack;

	int					m_nCurLoginMode;
	DWORD			m_dwAccountID;
	DWORD			m_dwCropid;

	string				m_yyGUID;
	string				m_jskkPm;
	bool					m_bIsYYlogin;

	string				m_ChargeUrl;
	BOOL				m_bIsOpenPassPort;

	BOOL				m_bIsNeedSetTelNum;

	std::vector<DWORD> m_vecCropIDInfo;

	BOOL				m_bYunLianOn;		//�����Ŀ���

};


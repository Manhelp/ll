#pragma once

enum
{
	EbMoney =0,	// Ǯ
	EbEquip,			// װ��
	EbMedicine ,	// ҩˮ
	EbStrengthen,	// ǿ������
	EbGem,			// ��ʯʥӡ
	EbQuest ,			// ����
	EbSprite ,			// �������
	EbOthers ,		// ������Ʒ

	// ʰȡƷ������
	EbWrite,			// ��
	EbYellow ,		// ��
	EbGreen ,			// ��
	EbBlue,			// ��
	EbOrange ,		// ��
	EbPurple ,		// ��
	EbPink ,			// ��
};


struct tagPickStrategy
{
		// ʰȡ��������
	DWORD	bMoney :1,			// Ǯ
		bEquip :1,			// װ��
		bMedicine :1,		// ҩˮ
		bStrengthen :1,	// ǿ������
		bGem :1,				// ��ʯʥӡ
		bQuest :1,			// ����
		bSprite :1,			// �������
		bOthers :1,			// ������Ʒ
									
		// ʰȡƷ������
		bWrite :1,				// ��
		bYellow :1,			// ��
		bGreen :1,			// ��
		bBlue :1,				// ��
		bOrange :1,			// ��
		bPurple :1,			// ��
		bPink :1;				// ��

	tagPickStrategy()
	{
		//	Ĭ��ȫ��ѡ��
		memset( this,-1,sizeof(*this) );
		//ZeroMemory(this,sizeof(*this));
		
	}
	tagPickStrategy& operator=(const tagPickStrategy& tag)
	{
		if(this==&tag)
			return *this;

		ZeroMemory(this,sizeof(*this));
		memcpy( this, &tag, sizeof(tagPickStrategy) );

		return *this;
	}
};

struct tagNS_PickItemStrategy;
class PickupMgr
{
	static PickupMgr s_guiPickMgr;
	PickupMgr(void);
	~PickupMgr(void);
	PickupMgr& operator=(PickupMgr&);
	PickupMgr(const PickupMgr&);
public:
	VOID Init();
	VOID Destroy();
	
	VOID OpenUI();
	VOID CloseUI();

	VOID SavePickStrategy();
	bool CanPick(DWORD itemID,int bQuality);
	tagPickStrategy& GetPickStrategy(){return m_PickStrategy;}
	static PickupMgr* Inst();
private:
	// ��ɫ���ߵ�ʱ��ͬ��ʰȡ����
	DWORD OnNS_PickItemStrategy(tagNS_PickItemStrategy* pNetCmd,DWORD);
private:
	TSFPTrunk<PickupMgr>			m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	tagPickStrategy						m_PickStrategy;
};

#pragma once
#include "..\WorldDefine\chat_define.h"
#include "BroadCastProtoData.h"

struct tagNS_MsgBroadCastBlob;
struct tagRoleGetNameEvent;
struct tagGuildNameGetEvent;
struct tagEnterPromptEvent;
struct tagPushSpecInfoEvent;

const DWORD BROADCAST_OVERTIME_SET = 5 * 1000; // ��ʱ���� 5 ��

class BroadCastMgr
{
public:
	enum EMsgCacheType
	{
		EMCT_Broadcast,		// �㲥
		EMCT_BigBroadcast,	// ��㲥
		EMCT_Chat,			// ����Ƶ��

		//�¹㲥ϵͳ��Ҫ

		EMCT_BigNewBroadcast,   // ��������
		EMCT_SmallRunHorse, 	// С�������
		EMCT_BigRunHorse,		// ���������
		EMCT_ChatNew		    // ����������
		
	};

	struct tagMsgCache
	{
		tstring strContent;
		EMsgCacheType eType;		
		ESendChatChannel eChannel;
		bool bHasRoleOrGuild;
		INT  npriority;
		DWORD color;
		tagMsgCache()
		{
			eType = EMCT_Broadcast;
			eChannel = ESCC_NULL;
		}
	};
	

public:
	BroadCastMgr(void);
	~BroadCastMgr(void);

	static BroadCastMgr* Inst();

	void Init();
	void Destroy();
	void Update();

	void BroadCast(LPCTSTR szContent,BOOL bHasRoleOrGuild);
	void ToChatChannel(LPCTSTR szContent, ESendChatChannel eChannel, BOOL bHasRoleOrGuid);
	void BigBroadCast(LPCTSTR szContent, BOOL bHasRoleOrGuild);
	void NewBroadCast(LPCTSTR szContent, BOOL bHasRoleOrGuild,DWORD msgID);

	const BroadCastProtoData&	GetBroadCastData() const { return m_BroadcastData; }

private:
	//�����㲥
	bool ParseBroadCast(tstring& strContext);

	void PushBroadCast(LPCTSTR szContent);
	void PushChatInfo(LPCTSTR szContent, ESendChatChannel eChannel);
	void PushBigBroadCast(LPCTSTR szContent);

	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pGameEvent);
	DWORD OnGuildGetNameEvent(tagGuildNameGetEvent *pGameEvent);
	DWORD OnEnterPromptEvent(tagEnterPromptEvent* pGameEvent);
	DWORD OnPushSpecInfo(tagPushSpecInfoEvent* pGameEvent);
	

	bool LoadBroadCastProto();

	//�õ���ӦƵ����������ɫ
	DWORD GetChannelColor(ENewChannelType escc);
private:
	TSFPTrunk<BroadCastMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GameScriptMgr>		m_pGSMgr;

	typedef std::vector<tagMsgCache> MsgCache;
	MsgCache					m_msgCache;			// �㲥��Ϣ����

	typedef std::vector<pair<bool,DWORD>> RoleCache;
	RoleCache					m_roleCache;		// �㲥��Ϣ�еĽ�ɫ����

	typedef std::vector<pair<bool, DWORD>> GuildCache;
	GuildCache					m_guildCache;		// �㲥��Ϣ�еİ��ɻ���

	bool m_bAllRoleFinded;
	bool m_bAllGuildFinded;

	DWORD m_dwLastParseTime;	// �ϴδ�������ʱ��
	DWORD m_dwCurTime;

	BroadCastProtoData         m_BroadcastData;     // �㲥ԭʼ����ͼ
};
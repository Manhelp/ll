/********************************************************************
	created:	2010/01/27
	created:	27:1:2010   13:21
	filename: 	e:\loong\trunk\LoongWorld\player_account.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	player_account
	file ext:	h
	author:		Jason
	
	purpose:	
*********************************************************************/


#ifndef player_account_h__
#define player_account_h__


#include "../ServerDefine/mini_tool.h"

class PlayerSession;

/**
 * \ingroup LoongWorld
 *
 *	������ʱ��ͳ����ص��û��˺���Ϣ
 *	���ų�������չΪ������Ϣ�Ŀ���
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-27
 *
 * \author Jason
 *
 *
 */
struct PlayerAccount
{
	DWORD			m_dwAccountID;
	tagDWORDTime	m_timeLastLoginTime; // �˺��˳�ʱ�����޸ģ������浽���ݿ�
	tagDWORDTime	m_timeLastLogoutTime;
	tagDWORDTime	m_timeLastReceiveDailyOfflineReward;
	tagDWORDTime	m_timeLastReceiveRegression;
	tagDWORDTime	m_timeTemLogin; // ������ڻ��浱ǰ�״ε�¼ʱ�䣬������ʱ���ж�

	PlayerAccount();
	~PlayerAccount();

	PlayerAccount(const PlayerAccount & account);
	PlayerAccount & operator= (const PlayerAccount & account);

	/**
	 * ��ʼ���������ʱ�������Ϣ
	 * \param accountid �˺�id
	 * \param logintime �ϴ��˻���ң��˳���Ϸʱ��
	 * \param logouttime �ϴ�����˺ţ��˳�ʱ��
	 * \param dailyoffline �ϴ���ȡÿ�����߾���ʱ��
	 * \param regression �ϴ���ȡ�ع齱����ʱ��
	 */
	VOID	Assign( DWORD accountid,tagDWORDTime logintime,tagDWORDTime logouttime,tagDWORDTime dailyoffline,tagDWORDTime regression,tagDWORDTime timeTemLogin );
	// ��Ӱ���˺ŵ�½ʱ��
	VOID	Login( PlayerSession * pPly );	
	// ��Ӱ���˺ŵǳ�ʱ�䣬������db server ��������˺��еĵǳ�ʱ��
	VOID	Logout( PlayerSession * pPly );
	// �õ��˻�������ʱ��
	DWORD	GetOfflineTime();
	// �����˻�����������ʱ��
	DWORD	GetDailyOfflineTime();

	BOOL	CanReceiveDailyOfflineReward();
	BOOL	CanReceiveRegressionReward(DWORD delta);

};

/**
 * \ingroup LoongWorld
 *
 *	�˺Ź�������Ϊ��ȷ��Ч�ʺ��̰߳�ȫ��û��ֱ�ӵ���PlayerAccount��ָ������ã������ǲ����˱Ƚϵͼ����ٷ�װ�İ취
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-27
 *
 * \author Jason
 *
 *
 */
class PlayerAccountMgr
{
	jm_tool::CSCreatedLock m_cs;
	typedef std::map<DWORD,PlayerAccount> TAccountMap;
	TAccountMap	m_mapAccount;
	PlayerAccountMgr(const PlayerAccountMgr &);
	PlayerAccountMgr & operator= (const PlayerAccountMgr & );
public:
	PlayerAccountMgr();
	~PlayerAccountMgr();

	// �յ�db server ���ص���Ϣ�󣬵��ã���ʼ��һ���˺�ʱ����Ϣ
	VOID OnRecAccountInfo(const PlayerAccount &);

	// Ӱ������˺��У���¼�ĵ�¼ʱ�䣬������״ε�½ʱ������
	VOID RoleLogin(DWORD accountid);
	// Ӱ������˺��У���¼�ĵǳ�ʱ�䣬����Ҽ����˳���Ϸʱ������
	VOID RoleLogout(DWORD accountid);
	// �õ��˻�������ʱ��
	DWORD	GetOfflineTime(DWORD accountid);
	// �����˻�����������ʱ��
	DWORD	GetDailyOfflineTime(DWORD accountid);

	BOOL	CanReceiveDailyOfflineReward(DWORD accountid);
	BOOL	CanReceiveRegressionReward(DWORD accountid,DWORD delta);

	// ���� �������߽�����ȡʱ������/��ȡ
	VOID	ReceiveDailyOfflineRewardTime( DWORD accountid, tagDWORDTime  rec_time ) ;
	tagDWORDTime	ReceiveDailyOfflineRewardTime(DWORD accountid);

	// ���� �ع齱����ȡ����/��ȡ
	VOID	ReceiveRegressionRewardTime( DWORD accountid, tagDWORDTime  rec_time);
	tagDWORDTime ReceiveRegressionRewardTime(DWORD accountid);

	VOID	SaveAccountTimeInfoOneTick( DWORD accountID );
};


extern PlayerAccountMgr g_PlayerAccountMgr;

#endif // player_account_h__



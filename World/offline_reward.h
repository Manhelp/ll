/********************************************************************
	created:	2010/01/15
	created:	15:1:2010   15:41
	filename: 	e:\loong\trunk\LoongWorld\offline_reward.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	offline_reward
	file ext:	h
	author:		Jason
	
	purpose:	���߹һ�����
*********************************************************************/

#ifndef offline_reward_h__
#define offline_reward_h__


class Role;

/**
 * \ingroup LoongWorld
 *
 *	���߽����ۼ���
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-15
 *
 * \author jpma
 *
 * \bug 
 *
 */
class OfflineReward
{
	DWORD	m_dwAccountID;	// ��ǰ����˺�
	DWORD	m_dwRoleID;		// �������ʱ��ɫid
	enum {E_OfflineTimeout = 24 * 60 * 2};	//���߹һ��ʱ��
public:
	OfflineReward()
		: m_dwAccountID(0),m_dwRoleID(0)
	{
	}
	~OfflineReward(){}

	enum{ E_Offline_NotFindPlayer = 1, E_ConfigFileNotComplete = 2, E_OfflineHang = 3, E_RefuseOfflineReward = 4, E_NotEnoughMoney = 5};

	/**
	 *	����ҵ�½��ͼ��Ҳ����role_init�ɹ����ú��ٵ����������
	 * \param player ��ҽ�ɫ
	 * \return �ɹ�����E_Success
	 */
	DWORD PlayerLogin(Role * player);
	// �÷�ͬ��,������Ϊ�˷�ֹ���ã�����һ�������������������Ҫ
	DWORD PlayerLogout(Role * player,BOOL exitGame = FALSE);

	// what��ȡ���ͣ�0��ʾ��ȡ��Ѿ��飬1��ʾ��ȡȫ������
	DWORD RecExp(Role * player,BYTE what,INT & exp);

	static INT OfflineRewardSwitch();
	static void OfflineRewardSwitch(INT switch_v);

private:
	/**
	 * �������������߹ң�����
	 * \param player 
	 * \return �ɻ�þ���ֵ
	 */
	INT64 CalOfflineRewardFreeExp(Role * player, INT nMinute);
	/**
	 * ������Ҹ��Ѿ���ֵ
	 * \param player 
	 * \return ���Ѿ���ֵ
	 */
	INT64 CalOfflineRewardPayedExp(Role * player, INT nMinute);
	INT64 CalOfflineExp(Role * player);

	INT CalOfflineMinutes(Role * player);
};


#endif // offline_reward_h__



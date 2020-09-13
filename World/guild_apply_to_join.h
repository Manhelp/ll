/********************************************************************
	created:	2010/02/08
	created:	8:2:2010   10:25
	filename: 	e:\loong\trunk\LoongWorld\guild_apply_to_join.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	guild_apply_to_join
	file ext:	h
	author:		Jason
	
	purpose:	����������
*********************************************************************/


#ifndef guild_apply_to_join_h__
#define guild_apply_to_join_h__


class Role;

struct tagGuildPriority
{
	DWORD	dwGuildID;
	tagGuildPriority()
		:dwGuildID(0){}
	tagGuildPriority(DWORD dwV)
		:dwGuildID(dwV){}
	virtual ~tagGuildPriority(){}

	tagGuildPriority& operator = ( const tagGuildPriority & t )
	{
		dwGuildID = t.dwGuildID;
		return *this;
	}
	tagGuildPriority(const tagGuildPriority & t)
		:dwGuildID(t.dwGuildID){}
};
bool operator < (const tagGuildPriority & t,const tagGuildPriority & p);
bool operator == (const tagGuildPriority & t,const tagGuildPriority & p);




/**
 * \ingroup LoongWorld
 * ���������ɸ���������
 *
 * \par requirements
 *
 * \version 1.0
 * first version
 *
 * \date 2010-02-09
 *
 * \author Jason
 *
 * 
 * \todo 
 *
 * \bug 
 *
 */
class ApplyToJoinGuildToolkit
{
protected:
	ApplyToJoinGuildToolkit(){}
	virtual ~ApplyToJoinGuildToolkit(){}
public:
	
	/**
	 * �����������̰߳�ȫ�ĵ���ģʽ
	 * \return 
	 */
	static ApplyToJoinGuildToolkit * GetApplyToJoinGuildToolkit();

	/**
	 * �ڰ��ɳ�ʼ����after having loaded all guild informations from db server������ʼ��ʱ������
	 * \param dwGuildID ����id
	 */
	virtual VOID AddGuild(DWORD dwGuildID) = 0;
	virtual VOID DelGuild(DWORD dwGuildID) = 0;

	/**
	 * �õ��������б�ֻ��ǰ100��
	 * \param pRole 
	 * \return 
	 */
	virtual DWORD GetGuildRanking(Role * pRole) = 0;

	/**
	* �������У�ֻ��ǰ100��
	* \param pRole 
	* \return 
	*/
	virtual VOID GuildRanking() = 0;
	
	/**
	 * ����������ĳ�����ɣ���������������뵥�����ظ���Ϣ��role
	 * \param pRole 
	 * \return 
	 */
	virtual DWORD ApplyToJoinGuild(Role * pRole,DWORD dwGuildID) = 0;

	/**
	 * �����ظ�
	 * \param pGuildOwner 
	 * \param dwSrcRoleID 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD ResponseApplierToJoinGuild(Role * pGuildOwner,DWORD dwSrcRoleID,DWORD dwGuildID) = 0;

	/**
	 * ����ȡ�����������ɰ����嵥�������嵥��role
	 * \param pRole 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD GetApplyBill(Role * pRole,DWORD dwGuildID) = 0;

	/**
	 * �õ�������ּ����������Ϣ��role
	 * \param pRole 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD GetGuildTenet( Role * pRole,DWORD dwGuildID ) = 0;

	virtual VOID Update() = 0;

	/**
	 * ��ϵ���� 
	 * \param pRole 
	 * \param dwGuildID 
	 * \return 
	 */
	virtual DWORD ConstractGuildMaster( Role * pRole,DWORD dwGuildID ) = 0;

	virtual DWORD OnGuildMasterNotice( Role * pRole ) = 0;

	/**
	 * ͬ����߾ܾ������������������Է����ߣ���֪ͨ�Է�
	 * \param pMaster 
	 * \param dwApplier 
	 * \param agreement 
	 * \return 
	 */
	virtual DWORD ApproveToJoinGuild( Role * pMaster , DWORD dwApplier , BOOL agreement = FALSE ) = 0;

	virtual DWORD SearchGuild(Role * pRole, INT findType,const tstring & strFindWhat) = 0;

	virtual DWORD ChangeGuildMember( DWORD dwGuildID, DWORD dwRoleID ) = 0;
};



#endif // guild_apply_to_join_h__




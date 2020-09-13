//-----------------------------------------------------------------------------
// Copyright (c) 2004 Dacheng Entertainment All rights reserved.
// filename	:	ScriptMsgInfo.h
// author		:	Jason
// actor			:		
// data			:	2010/08/26
// last			:	
// brief			:	������Լ
//-----------------------------------------------------------------------------

#ifndef fairy_contract_h__
#define fairy_contract_h__


/**
 * \ingroup World
 *	��Լ����
 *
 * \par requirements
 *
 * \version 1.0
 * first version
 *
 * \date 2010-08-27
 *
 * \author Jason
 * 
 */
class Role;
class CContractItem
{
	DWORD		m_dwRoleID;				// ��Լ��ؽ�ɫid
	DWORD		m_dwCurItemTypeID;		// ��ǰ��Լ������Ʒ
	INT			m_nTicks;					// ��ǰ��Ʒ�����ĵ�tick��ÿ���л���Ʒʱ�����Ҫ����
	INT			m_nScore;					// �÷����
	DWORD		m_dwUserSelected;			// �û�ѡ���
	BOOL		m_bSelected;				// ���û�ѡ��ʱ����TRUE����update����FALSE
	BOOL		m_bStart;					// ��Ҳ�����Լ�󣬵�һ��updateʱ���������������Ϣ���ͻ���
	INT			m_nSkillLevel;				// ǩ����Լ�ļ��ܵȼ�
	INT			m_nContractTimeout;		// (TICK_PER_SECOND * 3)����(TICK_PER_SECOND * 2)
	Role		*	m_pRole;
	BOOL		m_bDiscarded;				// �Ƿ����
	INT			m_nTotalTimeout;			// �ܼ�ʱ������10���Ӻ�û�з�Ӧ���������������Լ������
	INT			m_nClientUndoTimeout;		// ���û���յ��ͻ��˵���ʱ��
	INT			m_nFullScore;				// �ܷ�
public:
	CContractItem();
	CContractItem(const CContractItem & );
	CContractItem & operator = (const CContractItem &);

	DWORD			GetRoleID( void ) const{return m_dwRoleID;}
	void			SetRoleID( DWORD dwRoleID ){m_dwRoleID = dwRoleID;}

	DWORD			GetCurItemID( void ) const { return m_dwCurItemTypeID; }
	
	INT				GetScore(void) const { return m_nScore; }

	void			SetUserSelectedItem( DWORD dwItemID ) ; //{m_dwUserSelected = dwItemID;����tickʱ��}

	__inline BOOL	IsDiscarded() { return m_bDiscarded; }
	__inline void	SetDiscarded(BOOL b) { m_bDiscarded = b;}

	// ���û�ѡ���˷�������Ϊclientû�з���Ϣ��������
	// �����ٴδ�ʱ�������������������������ԭ�������ݿ�����
	void			Reset( DWORD dwRoleID );

	//void			Start(void);
	// ��ȡ��Լ����
	INT			GetContractContent(INT & nMaxV,DWORD &dwCurItemID,INT & num ,DWORD * pdwItemIDs,INT size);

	void			SetSkillLevel( INT level ){ m_nSkillLevel = level; }

	INT				GetTotalScore(void) const {return m_nFullScore;}
	void			SetTotalScore(INT n) {m_nFullScore = n;}

	
	/**
	 * ÿtick����
	 * \param void 
	 * \return ���û�ѡ������Լ��Ʒ��update���ж�ѡ����߳�ʱ�����Ӽ��ִ�����֪ͨ����û�����һ���û������Լ�󣬷���TRUE����������update�ͷ��أ���ȫ���㲥�������򷵻�FALSE��
	 */
	BOOL			Update(void) ;	// ÿ��tick��Ҫ���µ�
private:
	BOOL			IsCompleted() const ;
	// ��ItemsOfFairyContract(itemdefine.h�ж���)���һ����Ʒ�����m_dwCurItemTypeID��������֮
	DWORD			RandomItemID( void );	 
	static void		_copy(const CContractItem & src,CContractItem&des);
	void			RecalTimeoutValue(void);
	void			_update_timeout(void);
	BOOL			_update_select(BOOL Timeout=FALSE);
	INT			_mod_score(INT );
};

class MapFairyContract;
/**
* \ingroup World
* ��Լ������ÿ���������ǣ�ǩ����Լ���嵥
* \version 1.0
* first version
*
* \date 2010-08-30
*
* \author Jason
*
*/
class FairyContract
{
	MapFairyContract * m_pOwner;
	DWORD		m_dwFairyID;	// �������id
	BOOL		m_bCompleted;
	BOOL		m_bStarted;
	map<DWORD,CContractItem*>	m_mappItem;	// <��ɫid������>
	DWORD		m_dwWinner;	// Ӯ��
public:
	CContractItem * GetItem( DWORD dwRoleID )
	{
		map<DWORD,CContractItem*>::iterator i = m_mappItem.find(dwRoleID);
		if( i != m_mappItem.end() )
			return i->second;
		return NULL;
	}
	void AddItem( DWORD dwRoleID, CContractItem * pItem )
	{
		m_mappItem[dwRoleID] = pItem ;
	}

	void Start(void);

	void Update(void);

	BOOL IsStarted(void) {return m_bStarted;}

	BOOL IsCompleted() const {return m_bCompleted;}

	// ��Ϊnpc������ԭ�򣬶��ж�������Լ��
	void Terminate(BOOL bDelCreature = FALSE);

	DWORD	GetWinner(void){ return m_dwWinner; }

	FairyContract(MapFairyContract *p,DWORD dwFairyID);

	~FairyContract();

private:
	// �����Լʱ����
	void OnComplete(void);
	// ������Լʱ�ã��������������ʱ����
	void OnDestroy(void);
};


/**
* \ingroup World
* ÿ����ͼ��ص�������Լ
* \version 1.0
* first version
*
* \date 2010-08-30
*
* \author Jason
*	������Լʱ��������deletecreature���������������Ҫ�ڴ�����ʱ������ɾ��creature����ˣ���Ҫһ��������������mapʱ���ٴ��������б��вд��creature
*/
class MapFairyContract
{
	DWORD	m_dwMapID;	// ��ͼid
	DWORD	m_dwInstanceID;	// ����id
	map<DWORD,FairyContract* > m_mapFairyContract; // <�������id����Լ��>
public:
	MapFairyContract(DWORD dwMapID);

	~MapFairyContract();
	
	DWORD GetMapID(void) const {return m_dwMapID;}
	// ���Ӹ���id�ӿ�
	DWORD MapInstanceID(void)const {return m_dwInstanceID;}
	void MapInstanceID(DWORD dwInstID){m_dwInstanceID = dwInstID;}

	// �õ�ĳ����������Լ��
	FairyContract * GetContract( DWORD dwFairyID );
	// ����һ����Լ���������ʹ�õ��飨��Լopen������ʱ������
	FairyContract* AddContract( DWORD dwFairyID );

	// ������Լ����ʱ�ޣ�ͨ��ʱ��npc���ƣ����ú���������npc��ʧʱ��������Լ�
	void Terminate(DWORD dwFairyID,BOOL bDelCreature = FALSE);
	// ���������ֻ������Ҫʱ�����������Լ��������Ŀǰֻ���������е���
	void Clear(void);

	// ÿ��tick���£��ж�������Լ������������ʱ֪ͨ���
	void Update(void);

	BOOL IsOver(void);	// �Ƿ�ȫ��������

	CContractItem * GetContractItem( DWORD dwRoleID );
};


#endif // fairy_contract_h__






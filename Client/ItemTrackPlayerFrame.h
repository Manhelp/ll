#pragma once
#include "gameframe.h"
#include "Item.h"

struct tagNS_TrackPlayerPos;
struct tagNS_Move2Player4Track;
struct tagNS_GetPositionAfterTrack;

class ItemTrackPlayerFrame : public GameFrame
{
	enum EItemTrackStep
	{
		EStep_Null,						//��ʵ״̬
		EStep_ShowInputMsgBox,			//�����������
		EStep_Wait,						//�ȴ���������Ϣ
		EStep_ShowTransferMsgBox,		//ѯ����Ҿ����Ƿ� ����
		EStep_Loading,					//���ڴ���
		EStep_RTSHowPlayerInfo,			//ʵʱ��ʾĿ����ҵ�����
		EStep_End						//����
	};

	typedef std::pair<tstring,tstring> MapName;

public:
	ItemTrackPlayerFrame(void);
	~ItemTrackPlayerFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual VOID Update();

	bool SetTrackItem( Item* pItem )
	{
		if( !P_VALID(pItem) )
			return false;

		if( pItem->GetItemSpecFunc() != EISF_PlayerTrack )
			return false;

		m_pItem = pItem;
		m_n64ItemID = pItem->GetItemId();
		m_TrackItemSpecVal = pItem->GetProto()->nSpecFuncVal1;

		m_step = EStep_ShowInputMsgBox;
		return true;
	};

private:

	//׷�����λ�� ��Ϣ����
	DWORD OnNS_TrackPlayerPos( tagNS_TrackPlayerPos *pNetCmd, DWORD );

	//���ʹ�����
	DWORD OnNS_TrackPlayerTransfer( tagNS_Move2Player4Track *pNetCmd, DWORD );

	// ����֮��Ҫÿ��10�������������һ����Ϣ�������ҵ�����λ��
	// ��������
	DWORD OnNS_GetPositionAfterTrack( tagNS_GetPositionAfterTrack *pNetCmd, DWORD );

	//��Ӧ������������
	DWORD OnTryToTrackPlayer( tagMsgInputBoxEvent* pGameEvent );
	
	// ���ʹ��׷�ٵ��߽��д���
	DWORD OnEvent_PlayerTrackMove2Player( tagMsgBoxEvent* pGameEvent );

private:

	/** \����Ƿ����ʾ �Ƿ���
		\param ������Դ��ͣ�ʹ�õ���Ʒָ��
		\return �Ƿ���Դ���
	*/
	bool CheckTransfer( Item* & pTransferItem );
	
	/**
		\ �õ����ݵ�ͼ��ID,��λ�� �õ���ͼ���ƺ���������
		\ return ���ص�ͼ�Ƿ��ǳ����ͼ EMT_Normal
	*/
	MapName GetMapName( DWORD dwMapId, Vector3 pos, LPBOOL pNormal );
	
	/**
		\ ����״̬�Ƿ� �ܹ������ͣ��ܱ����ͷ��� true�����򷵻�false
	*/
	bool CheckLPState();

	//���͵�Ŀ���ͼ
	void GotoNewMap( DWORD dwMapID, Vector3 vecPos,Vector3 vecFaceto );

private:
	TSFPTrunk<ItemTrackPlayerFrame> m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	EItemTrackStep			m_step;							//׷�ٲ�����ǰ�Ľ׶�
	
	//ʹ�õ�׷�ٵ��ߵ�64λID��׷�ٽ׶α�ʾ׷�ٵ��ߵ�ID, ���ͽ׶κʹ��ͺ󶼱�ʾ���д��͹��ܵ�׷�ٵ��ߵ�id
	//m_pItem Ҳһ��
	INT64					m_n64ItemID;					
	Item*					m_pItem;
	INT32					m_TrackItemSpecVal;				//׷�ٵ��ߵ����ͣ��ܷ��ͣ�Ĭ���ǲ��ܴ���
	
	TCHAR					m_szPlayerName[X_LONG_NAME];	//Ҫ׷�ٵ���ҵ�����

	INT						m_nTellPositionAfterTransfer;	//��ʾ������, ����֮��Ҫÿ��10����ʾһ�� Ŀ����ҵ�λ��,�ܹ�����
	DWORD					m_dwTimer;						//ʱ�����������10����ʾһ��

	DWORD					m_dwTargetRoleID;				//Ŀ����ҵ�ID
};

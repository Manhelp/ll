#pragma once
#include "gameframe.h"

struct tagSpecItemEvent;
struct tagNS_SpecScriptItem;
struct tagNC_AddPetLiveTime;

/** \class SpecialItemFrame

	\brief ��������Ʒ�Ĳ���ʹ�����

	\remarks ����������Ʒ�Ĳ��� ����Ҫ��Ϊ�˷�����Ժ�û�а취�����
	��������Ʒ��һ�����ʵĵط���ֲ�����
*/
class SpecialItemFrame :
	public GameFrame
{
public:
	SpecialItemFrame(void);
	~SpecialItemFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	//�¼���Ӧ
	//���˸�Ů�ڻ����¼�
	BOOL  OnWnd_WomenFestival( tagGUIEvent* pEvent );
	BOOL  On_WomenFestivalShow( tagSpecItemEvent* pEvent );
	BOOL  OnEvent_OpenSpecFrame(tagGameEvent *pEvent);
	BOOL  OnEvent_CloseSpecFrame(tagGameEvent *pEvent);

	//������Ϣ�¼�
	//���˸�Ů�ڻ����¼�
	DWORD NS_SpecScriptItem(tagNS_SpecScriptItem* pNetCmd, DWORD dwParam);

	VOID SafeDestroyUI();
private:

	TSFPTrunk<SpecialItemFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	//38��Ů�ڴ���
	GUIWnd*							m_pWnd_WomenFestval;	//������
	GUIButton*                      m_pBn_Ok_Set;                 //ȷ��
	GUIButton*                      m_pBn_Cancel_Set;             //ȡ��
	GUIButton*                      m_pBn_Close_Set;              //�ر�
	GUIEditBox*						m_pEb_FriendName;			//�����
	GUIStatic*						m_pStc_Title;				//�����
	GUIComboBox*					m_pCb_Friends_Box;				//�����б���Ͽ�
	DWORD							m_curRow;
	std::vector<LPCTSTR>			m_vecFriendsArray;
	std::vector<tstring>			m_vecPetArray;
	INT64				            m_n64NUM;						//��ʱ�洢64λ��ƷID������

	//map<tstring ,DWORD>				m_mapName2pet;
	BOOL							m_bIsPetFood;
};

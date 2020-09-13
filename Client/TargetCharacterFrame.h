#pragma once
#include "gameframe.h"
#include "SocialEvent.h"
#include "ReputeMgr.h"
#include "RoleTitleProtoData.h"
#include "RoleTitleFrame.h"
#include "TreeEx.h"
class RemoteRoleStateFrame;
#include "..\WorldDefine\msg_role_title.h"
#include "..\WorldDefine\msg_reputation.h"

class IconStatic;
struct tagNS_GetRoleVCard;

class TargetCharacterFrame :
    public GameFrame
{
public:
    TargetCharacterFrame(void);
    ~TargetCharacterFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
	virtual BOOL EscCancel();
	VOID Show(BOOL bVal) { ShowWnd(bVal); }

    tstring GetGUIFullName()
    {
        if(P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else 
            return _T("");
    };
	
private:
    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ��ʾĿ�����װ��,����,�ƺŽ���
    DWORD OnDispPlayerState(tagDispPlayerState* pMsg);

    //! ��ʾ����
    VOID ShowWnd(BOOL bVal);

    //! ����������ʾ
    VOID ResetRepute();
    //! ���ͻ����������
    VOID BeginNC_GetReputation(DWORD dwRoleID);
    //! �����������
    DWORD OnNS_GetReputation(tagNS_GetReputation* pMsg, DWORD);

	//�鿴�����Ϣ����
	DWORD OnNS_CheckRoleInfo(tagNS_CheckRemoteRoleInfoOpenType* pMsg, DWORD);
    //! ���óƺ���ʾ
    VOID ResetTitle();
    //! ���ͳƺŻ������
    VOID BeginNC_GetRoleTitles(DWORD dwRoleID);
    //! ���سƺŽ��
    DWORD OnNS_GetRoleTitles(tagNS_GetRoleTitles* pMsg, DWORD);
    //! ��ʼ�����гƺ�xml���ݵ��ؼ�
    VOID InitTree();
    //! ���ݳƺű仯ˢ�¿ؼ�
    VOID UpdateTree();
    //! ѡ��ĳһ�ƺ�
    VOID SelectRoleTitle();

	void ShowErrorMsg( DWORD dwError );

	// ����װ��
	VOID ResetRemoteRoleEquip();

	// ��������ҳ�¼�
	DWORD OnEventOpenRepute(tagOpenReputeEvent *pEvent);
	// ��װ����ҳ�¼�
	DWORD OnEventOpenEquip(tagOpenEquipEvent *pEvent);
	// ����������Ҳ鿴װ���¼�
	DWORD OnEventOfflinePlayerEquip(tagGameEvent *pEvent);

	//�����ȡ��ɫ��������������Ϣ
	DWORD OnNS_GetRoleVCard(tagNS_GetRoleVCard* pMsg, DWORD dwParam);

	VOID SendGetRoleVCard();

    map<DWORD, tagTitleList>		m_mapOwnedTitles;		/*!< �����ɫ�ѻ�óƺŵ����� */
    map<DWORD, tagTitleTypeActivedCount> m_mapActivedCount; /*!< ����title��õ����� */

    TSFPTrunk<TargetCharacterFrame> m_Trunk;
    TObjRef<GUISystem>			    m_pGUI;
    TObjRef<Util>				    m_pUtil;
    TObjRef<GameFrameMgr>		    m_pFrameMgr;
    TObjRef<NetCmdMgr>				m_pCmdMgr;
    TObjRef<NetSession>				m_pSession;

    // ������

    GUIWnd*						    m_pWnd;             //!< ������
    GUIWnd*                         m_pEquip;           //!< װ���Ӵ���
    GUIWnd*                         m_pRepute;          //!< ��������
    GUIWnd*                         m_pTitle;           //!< �ƺŴ���
	GUIWnd*                         m_pInfo;           //!< ��������
    
    GUIPushButton*                  m_pBtEquip;         //!< ����װ��
    GUIPushButton*                  m_pBtReput;         //!< �������� 
    GUIPushButton*                  m_pBtTitle;         //!< ����ƺ�
	GUIPushButton*                  m_pBtInfo;         //!< ��������

    GUIButton*                      m_pBtClose;         //!< �ر�

    // �������
	GUIStatic*                  m_pStcGuildExploit;     //!< ���ɹ�ѫ
	GUIStatic*                  m_pStcGuildContribute;  //!< ���ɹ���

	GUIStatic*                  m_pStcStrengthValue;    //!< ʵ����ֵ
	GUIStatic*					m_pStcLevel;			// �ȼ�ʵ��ֵ
	GUIStatic*					m_pStcEquip;			// װ��ʵ��ֵ
	GUIStatic*					m_pStcSprite;			// ����ʵ��ֵ
	//GUIStatic*					m_pStcTitle;			// �ƺ�ʵ��ֵ
	//GUIStatic*					m_pStcFriend;			// ����ʵ��ֵ

	GUIStatic*                  m_pStcWuXun;            //!< ��ѫ
	GUIStatic*                  m_pStcWuJi;             //!< �似
	GUIStatic*                  m_pStcTotal;	        //!< 3v3 �ܳ���
	GUIStatic*                  m_pStcWinCount;         //!< 3v3 ʤ��

	GUIStatic*					m_pStcFlowerNum;		//�ʻ���
	GUIStatic*					m_pStcEggNum;			//������

    // �ƺ����
    TreeEx*                         m_pTreeTitleList;
    GUIStaticEx*                    m_pStcexDescription;

    DWORD                           m_dwCurrPlayerID;    //!< ��ǰ�鿴���ID
	RemoteRoleStateFrame*			m_pRRSF;

    bool                            m_bneedRefreshRepute;
    DWORD                           m_timeFirst;


	GUIStatic*							m_pStcSpouse;			//��ż
	GUIStatic*							m_pStcGender;			//�Ա�
	GUIStatic*							m_pStcConstellation;	//����
	GUIStatic*							m_pStcCity;				//����
	GUIStatic*							m_pStcChZodiac;			//��Ф
	GUIEditBoxEx*					m_pEditBoxExPresent;	//����ǩ��
	GUIEditBoxEx*					m_pEditNameHistory;		//������¼
	IconStatic*						m_pStcHeadPic;			//ͷ��
	GUIStatic*							m_pStcHeadPicC;			//ͷ���

	GUIButton*						m_pBtRefresh;			//ˢ��
	GUIButton*						m_pBtClose2;				//�ر�
};

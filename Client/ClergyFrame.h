#pragma once
#include "..\WorldDefine\clergy_define.h"

/**\��ְ����Frame
*/
class IconStatic;
class LocalPlayer;
struct tagNS_GetAttribute;
struct tagNS_PromotionClergy;

const DWORD dwGreenColor = 0xff67ff4b;	//��ɫ
const DWORD dwRedColor = 0xffff4b4b;	//��ɫ
const DWORD dwWhiteColor = 0xffffffff;	//��ɫ
const DWORD dwObtainColor = 0xff4be1d5;	//�����е���ɫ

class ClergyFrame : GameFrame
{
public:
	ClergyFrame();
	~ClergyFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID RefrashInfo(DWORD dwID);	//ͨ����ְIDˢ�½���
	VOID ClearInfo();				//�����Ϣ
	VOID Show();
	VOID Hide();
	VOID LockButt();				//������ť
	VOID UnLockButt();				//������ť
	VOID SetIsObtain();				//�����Ƿ�����

	DWORD OnGameEvent(tagGameEvent* pGameEvent);


private:
	TSFPTrunk<ClergyFrame> m_Trunk;
	TObjRef<GUISystem>		   m_pGUI;
	TObjRef<GameFrameMgr>	   m_pFrameMgr;
	TObjRef<NetSession>        m_pNetSession;
	TObjRef<NetCmdMgr>         m_pNetCmdMgr;

	GUIWnd                  *m_pWnd;
	GUIWnd                  *m_pWndCaption;
	GUIStaticEx             *m_pStcGodHeadLimit;				//�������
	GUIStaticEx             *m_pStcSexLimit;					//�Ա�����
	GUIStaticEx             *m_pStcAttLimit;					//��������
	GUIStaticEx             *m_pStcLevelLimit;					//��������
	GUIStaticEx             *m_pStcClassLimit;					//ְҵ����
	GUIStaticEx             *m_pStcAttValue;					//������ֵ
	IconStatic              *m_pIStcGodIcon;					//ͼ��
	GUIButton               *m_pBtnUp;							//��һ��
	GUIButton               *m_pBtnDown;						//��һ��
	GUIStatic				*m_pStcClergyText;					//��ְ�ı�(��λ��=)
	GUIStatic				*m_pStcClergyName;					//��ְ����
	GUIStatic				*m_pStcHasReached;					//�Ƿ���������
	GUIStatic				*m_pStcDesc;						//����
	GUIButton				*m_pBtnClose;						//�ر�
	GUIButton				*m_pBtnPromotion;					//������ť
	IconStatic				*m_pIStcFieldIcon[MAX_FieldNum];	//����ͼ��

	DWORD					m_dwClergyID;						//��ְID

	const tagClergyName*	pClergyInfo;

	DWORD                   m_dwMapNum;					//��ְ����
	bool					m_bIsReach;					//�Ƿ���
	DWORD                   m_dwRoleAtt[ERA_End];		//���������ݴ�
	LocalPlayer*			m_pLocalPlayer;				//�������ָ��
	DWORD					m_escapeTime;

	BOOL					m_bIsTransform;				//�Ƿ����״̬


private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_GetAttribute(tagNS_GetAttribute* pCmd, DWORD);
	DWORD OnNS_PromotionClergy(tagNS_PromotionClergy* pCmd, DWORD);

	DWORD OnEventOpenClergyFrame(tagGameEvent* pEvent);

public:
	tstring GetClergyIconPath(DWORD dwClergyID);
};
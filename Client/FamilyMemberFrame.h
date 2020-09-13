#pragma once
#include "gameframe.h"
#include "..\WorldDefine\family_define.h"

#define  MAX_HEART_NUM     6
class IconStatic;
class FamilyMemberFrame :
	public GameFrame
{
public:
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowWnd(BOOL bShow = TRUE);
	VOID RefreshUI();
	BOOL IsShowing(); 
	FamilyMemberFrame(void);
	~FamilyMemberFrame(void);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID  ClearUI();
	const TCHAR* GetClassTypePic(EClassType eType);
	
	DWORD OnMsgBox_InputRoleName( tagMsgInputBoxEvent* pGameEvent );
	
	VOID  SendTeamApply();
	VOID  SendInviteInput();
	VOID  SendLeaderChange();
	VOID  SendDismisPlayer();
	VOID  SendMemberLeave();

private:
	TSFPTrunk<FamilyMemberFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<GUISystem>						m_pGUI;

	GUIWnd*									m_pWnd;
	GUIWnd*									m_pWndFather;

	GUIPatch*								m_pPchMember[MAX_FAMILY_MEMBER];		// ��	 Ŀ			
	IconStatic*								m_pStcProfes[MAX_FAMILY_MEMBER];		// ְ	 ҵ
	GUIStatic*								m_pStcLevel[MAX_FAMILY_MEMBER];			// ��	 ��
	IconStatic*								m_pStcSex[MAX_FAMILY_MEMBER];			// ��	 ��		
	GUIStatic*								m_pStcName[MAX_FAMILY_MEMBER];			// ��	 ��
	GUIStatic*								m_pStcPlace[MAX_FAMILY_MEMBER];			// ��	 ��
	GUIStatic*								m_pStcTips[MAX_FAMILY_MEMBER];			// λ����ʾ
	GUIStatic*								m_pStcCurPick[MAX_FAMILY_MEMBER];		// ѡ �� ��

	GUIProgress*							m_pProHP[MAX_FAMILY_MEMBER];			// H	P
	GUIProgress*							m_pProMP[MAX_FAMILY_MEMBER];			// M	P

	GUIStatic*								m_pStcHeart[MAX_HEART_NUM];				// ����ͼ��

	GUIButton*								m_pBtnTeam;			//���
	GUIButton*								m_pBtnInvite;		//����
	GUIButton*								m_pBtnReplace;		//�ƽ�
	GUIButton*								m_pBtnDismiss;		//����
	GUIButton*								m_pBtnLeave;		//����


	DWORD									m_dwCurIndex;		//��ǰѡ���������

};

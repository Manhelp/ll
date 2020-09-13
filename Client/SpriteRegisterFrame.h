#pragma once
#include "gameframe.h"
#include "..\WorldDefine\family_define.h"
/*
	�����Ǽ�
*/
class ItemButton;
struct tagItemPosChangeExEvent;
struct tagNS_FamilySpriteRegister;
struct tagNS_FamilySpriteGetBack;
struct tagNS_GetFamilyRoleSprite;
struct tagNS_GetFamilySpriteAtt;

class Item;

struct GUIFamilySprite
{
	ItemButton*		pIbPic;					// ����ͼƬ
	GUIStaticEx*	pStPlayerName;	// �������
	GUIStatic*		pStSpriteName;	// ��������
	GUIStatic*		pStLevel;				// �ȼ�
	GUIStatic*		pStStrength;		// ʵ��ֵ
	GUIStatic*		pStResonance;		// ����
	GUIPatch*		pPMainBack;

	DWORD	dwID;
	BYTE		bQuality;
	
	void clear();
	void RefreshUI(DWORD dwItemID = 0,BYTE byQuality = 0,INT nLevel = 0, DWORD dwStrength = 0, DWORD dwResonance = 0,LPCTSTR szPlayerName = _T(""),LPCTSTR szSpriteName =  _T(""));
};


class SpriteRegisterFrame :
	public GameFrame
{
public:
	SpriteRegisterFrame(void);
	~SpriteRegisterFrame(void);
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowFrame(BOOL bShow = TRUE);
	DWORD EventHandler(tagGUIEvent* pEvent);
	BOOL IsInvisable(){return m_pWnd->IsInvisible(); }
	VOID LockItem( bool bOperable );
	
private:
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnClose_Family_AllUi(tagGameEvent* pGameEvent);
	DWORD OnNS_FamilySpriteRegister(tagNS_FamilySpriteRegister* pNetCmd,DWORD);
	DWORD OnNS_FamilySpriteGetBack(tagNS_FamilySpriteGetBack* pNetCmd,DWORD);
	DWORD OnNS_GetFamilyRoleSprite(tagNS_GetFamilyRoleSprite* pNetCmd,DWORD);
	DWORD OnNS_GetFamilySpriteAtt(tagNS_GetFamilySpriteAtt* pNetCmd,DWORD);
	VOID PutSprite( INT16 nIndex  );
	DWORD OnMsgBox_SpriteRegister_Check(tagMsgBoxEvent* pGameEvent);
	VOID RefreshUI();
private:
	TSFPTrunk<SpriteRegisterFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*										m_pWnd;
	GUIButton*									m_pClose;
	GUIButton*									m_pCloseX;
	GUIButton*									m_pGetBack;
	GUIButton*									m_pRegister;
	ItemButton*									m_pPic;

	GUIStaticEx*								m_pStSptireAtt[EFSA_Num];
	GUIStatic*									m_pStSptireAttClose[EFSA_Num];

	GUIFamilySprite							m_pFamilySpriteInfo[MAX_FAMILY_MEMBER];		// ��һ���Ǳ��˵�����

	INT64											m_n64SpriteID;
	DWORD										m_dwSpriteTypeID;
	BYTE											m_bSpriteQuality;
	INT16											m_n16Index;

	tagRoleSpriteDataSave				m_sFamilySprites[MAX_FAMILY_MEMBER];

	Item*											m_pItem;		//�Ǽ��е�����

};

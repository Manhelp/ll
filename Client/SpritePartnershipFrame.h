#pragma once
#include "gameframe.h"
#include "ItemButton.h"
class Item;
struct tagItemPosChangeExEvent;
struct tagNS_PutFairyInto;
struct tagNS_TransmitPutFairyInto;
struct tagNS_TransmitFairyMating;
struct tagNS_FairyMating;
struct tagNS_FairyMatingCancelAll;

struct tagSpriteInfo
{
	DWORD dwTypeID;
	int	nQuality;			// Ʒ��
	int	nIntelligence;	// ����
	int	nLeftTimes;		// �ɽ������
	tagSpriteInfo():dwTypeID(0),nQuality(0),nIntelligence(0),nLeftTimes(0){}
	void SetInfo(DWORD dwID,int nQuali,int nIntel,int nLeft)
	{
		dwTypeID = dwID;
		nQuality = nQuali;
		nIntelligence = nIntel;
		nLeftTimes = nLeft;
	}

	void clear()
	{
		dwTypeID = 0;
		nQuality = 0;
		nIntelligence = 0;
		nLeftTimes =0;
	}
};

struct GUISpritePartner
{
	tagSpriteInfo		strSpriteInfo;

	ItemButton*			pItbItemPic;
	GUIStatic*			pStItemName;
	GUIStaticEx*		pStLeftTimes;
	GUIStaticEx*		pStQuality;
	GUIStaticEx*		pStIntelligence;
	GUIButton*			pBnConform;	

	void clear()
	{
		strSpriteInfo.clear();
		pItbItemPic->RefreshItem();
		pStItemName->SetText(_T(""));
		pStLeftTimes->SetText(_T(""));
		pStQuality->SetText(_T(""));
		pStIntelligence->SetText(_T(""));
		pBnConform->SetText(g_StrTable[_T("SpriteBorn_ConformPart")]);
	}

	bool haveInfo()
	{
		if(strSpriteInfo.dwTypeID != 0)
			return true;
		else
			return false;
	}
};

enum EBronState
{
	EBS_WaitSpriteConform,	// �ȴ���������ȷ��
	EBS_WaitProssing,			// �ȴ���������������رս��棩
};

enum
{
	MainSprite = 0,
	AssistantSprite = 1,
	SpriteNum = 2,
};

class SpritePartnershipFrame :
	public GameFrame
{
public:

	SpritePartnershipFrame(void);
	~SpritePartnershipFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID  Update();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	VOID LockItem( bool bOperable );
	VOID ClearUI();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID ConformPart();
	VOID CheckResult();
	VOID PutSprite(int eType,DWORD dwID,int nQlty,int nIntell,int nleftTime);
	VOID SyncSprite();
	VOID ChangeQ(int & q);
	DWORD OnEventMove2SpritePart(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnNS_PutFairyInto(tagNS_PutFairyInto* pNetCmd,DWORD);
	DWORD OnNS_TransmitPutFairyInto(tagNS_TransmitPutFairyInto* pNetCmd,DWORD);
	DWORD OnNS_TransmitFairyMating(tagNS_TransmitFairyMating* pNetCmd,DWORD);
	DWORD OnNS_FairyMating(tagNS_FairyMating* pNetCmd,DWORD);
	DWORD OnNS_FairyMatingCancelAll(tagNS_FairyMatingCancelAll* pNetCmd,DWORD);
	VOID PutInMainSpriteCache(DWORD dwID,int nQlty,int nIntell,int nleftTime);
private:
	TSFPTrunk<SpritePartnershipFrame>	m_Trunk;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<GUISystem>						m_pGUI;

	GUIWnd*												m_pWnd;
	GUIStaticEx*										pStResult;			// ���Ч��
	GUIProgress*										pStProgressing;	// ������
	GUIButton*											pBnClose;
	GUISpritePartner									m_GUISprite[SpriteNum];
	tagSpriteInfo										m_MainSpriteCache;

	DWORD												m_dwNPCID;				// NPC��ID
	DWORD												m_escapeTime;
	bool														m_bOtherPalyerConform;	// �Է��Ƿ�ȷ����
	Item*													m_pItem;						//	�Լ��������ƷID
	EBronState											m_eState;

	bool														m_bCancelConform;
};

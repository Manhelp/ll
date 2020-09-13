#pragma once
#include "gameframe.h"
#include "..\WorldDefine\pet_define.h"
#include "PetShowWindow.h"
//! \class   PetShowWindow
//! \brief   ����ģ��Ԥ������
//! \author  JGuo
//! \date    2010-9


//------------����������------------
struct tagStartNPCTalkEvent;
//----------------------------------

struct tagNS_PetCamera;
// ��Ϣ����petframe��ȡpetID
struct tagPetCameraUI: public tagGameEvent
{
	DWORD  dwPetID; 
	tagPetCameraUI(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame) ,dwPetID(GT_INVALID){}
};

enum ESetAxis
{
	E_Axis_X = 0,
	E_Axis_Y = 1,
	E_Axis_Z = 2,
};

// <! class 1:  PetCameraWindow;
// ��д PetShowWindow �� UpdateCamera()
class PetCameraWindow :
	public PetShowWindow
{
public:
	PetCameraWindow(void);
	virtual ~PetCameraWindow(void){};

	VOID SetCamera(ESetAxis ESAxis,float fValue);
protected:
	virtual void UpdateCamera();
private:
	float				m_fCamX;
	float				m_fCamY;
	float				m_fCamZ;
};

//<!  class 2: PetPreviewCamera 
//���ڽ���
class PetPreviewCamera :
	public GameFrame
{
public:
	
	PetPreviewCamera(void);
	~PetPreviewCamera(void){};

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	virtual VOID Render3D();

	void Show( bool bShow )
	{
		if (P_VALID(m_pWnd))
			m_pWnd->SetInvisible( !bShow );
	}
private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD SetAxis(ESetAxis eAxis);
	DWORD RevPetCameraUI(tagPetCameraUI* pGameEvent);

private:
	TSFPTrunk<PetPreviewCamera>	m_Trunk;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<NetCmdMgr>		m_pCmdMgr;

	GUIWnd*					m_pWnd;     
	PetCameraWindow*		m_pPteWindow;
	GUIEditBox*				m_pEBCameraX;
	GUIEditBox*				m_pEBCameraY;
	GUIEditBox*				m_pEBCameraZ;
	GUIButton*				m_pBtnExit;

	DWORD					m_dwPetID;
};

//<! class 3:  PetPreviewMgr
//Frame ȫ�ֹ�����
class PetPreviewMgr
{
public:
	PetPreviewMgr(void);
	~PetPreviewMgr(void){};
	static PetPreviewMgr* Inst();
	void Init();
	void Destroy();
private:
	DWORD OnOpenFrame(tagNS_PetCamera* pEvt, DWORD);



	//------------����������------------
	DWORD Test_OnOpenFrame(tagStartNPCTalkEvent* pEvt);
	//----------------------------------
	
private:
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TSFPTrunk<PetPreviewMgr>	m_Trunk;
	PetPreviewCamera*			m_pPetCamera;
};

//һ��H��CPP�˶�����3���࣬ͦ2�ģ�
//�����̳��ࣨclass 1����Mgr�ࣨclass 3����ͦ�򵥣����ԡ�����

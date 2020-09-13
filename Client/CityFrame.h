#pragma once

#include "IconStaticEx.h"

struct tagCityAttEvent;

// �鿴������Ϣ
class CityFrame : public GameFrame
{
public:
	CityFrame();
	~CityFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID Show(BOOL bShow);

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnEventRefreshCityAtt(tagCityAttEvent *pEvent);
	VOID LoadDefaultProductIcon(DWORD dwCityID);
private:
	TSFPTrunk<CityFrame> m_Trunk;
	TObjRef<NetSession> m_pSession;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;

	GUIWnd *m_pWnd;
	GUIButton *m_pBtnClose;					// �رհ�ť
	IconStatic *m_pIstCityIcon;				// ����ͼ��
	GUIStatic *m_pStcCityName;				// ��������
	IconStatic *m_pIstGuildIcon;			// ����ͼ��
	GUIStatic *m_pStcGuildName;				// ��������
	
	GUIStatic *m_pStcDoorType;				// ��������
	GUIStatic *m_pStcPatronGod;				// �ػ���
	GUIStatic *m_pStcTaxRate;				// ����˰��
	GUIStatic *m_pStcBaseFundProduction;	// �����ʽ������
	GUIStatic *m_pStcCurrentFundProduction;	// ��ǰ�ʽ������
	GUIStatic *m_pStcDoorDefence;			// ���ŷ���ֵ
	GUIStatic *m_pStcGodPrivity;			// �ػ������϶�
	GUIStatic *m_pStcProductivity;			// ������
	GUIStatic *m_pStcBaseMaterialProduction;// �����ʲĲ�����
	GUIStatic *m_pStcCurrentMaterialProduction;//��ǰ�ʲĲ�����
	
	IconStaticEx *m_pIstItems[5];			// ������Ʒ����
	GUIStatic  *m_pStcProText[5];			// ���������Ҫ��
	DWORD m_Items[5];						// ��ƷID
	DWORD m_dwCount;
	DWORD m_dwTime;
	
};
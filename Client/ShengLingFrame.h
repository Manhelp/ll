#pragma once
#include "IconStatic.h"
#include "IconStaticHL.h"
#include "ActiveProgress.h"
#include "ComposeStatic.h"
#include "..\WorldDefine\ItemDefine.h"

/*!
\class ShengLingFrame
\brief ʥ�����
\author lib
\base GameFrame
\remark
*/
#define MAX_MOQI  100
#define MAX_DAY_EAT 5
struct tagUpdateLPItemEvent;
struct tagNS_SLEatFairy;
struct tagNS_RoleAttChangeSingle;
struct tagNS_EquipHoly;
struct tagNS_UnEquipHoly;
struct tagUpdateLPItemEvent;
struct tagNS_ChongNeng;
struct tagNS_LingNeng;
struct tagNS_HolyEquipChg;
struct tagNS_HolyStrength;
struct tagNS_HolyEquipmentNumber;
class Equipment;
class ShengLingFrame : public GameFrame
{
public:
	ShengLingFrame(void);
	~ShengLingFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID RefreshUI();							//ˢ��ʥ�����
	VOID RefreshTunShi();						//ˢ�����ɽ���
	VOID RefreshKeYin();						//ˢ�¿�ӡ����
	VOID RefreshXiYin(BOOL bIsSelectAll);		//ˢ��ϴӡ����
	VOID RefreshChongNeng();					//ˢ�³��ܽ���
	DWORD IsItemEnough(BYTE byQuality);
	VOID CloseShengLingWnd();
	VOID ChongNengEnable(BOOL bIsEnable);
	VOID ClearChongNeng();
private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnShengLingEquipEvent(tagGameEvent* pGameEvent);
	DWORD OnShengLingUnEquipEvent(tagGameEvent* pGameEvent);
	DWORD OnItemEvent_EquipChange(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnChaiEquipConfirmEvent(tagMsgBoxEvent* pMsgBoxEvent);
	DWORD OnUpdateHolyAtt(tagGameEvent* pEvent);
	DWORD OnShengWenKeYinConfirmEvent(tagMsgBoxEvent* pMsgBoxEvent);

	DWORD OnNS_SLEatFairy(tagNS_SLEatFairy* pMsg, LPVOID pParam);
	DWORD OnNS_RoleAttChangeSingle(tagNS_RoleAttChangeSingle* pMsg, LPVOID pParam);
	DWORD OnNS_EquipHoly(tagNS_EquipHoly* pMsg, LPVOID pParam);
	DWORD OnNS_UnEquipHoly(tagNS_UnEquipHoly* pMsg, LPVOID pParam);
	DWORD OnNS_ChongNeng(tagNS_ChongNeng* pMsg, LPVOID pParam);
	DWORD OnNS_LingNeng(tagNS_LingNeng* pmsg, LPVOID pParam);
	DWORD OnNS_HolyEquipChg(tagNS_HolyEquipChg* pMsg, LPVOID pParam);
	DWORD OnNS_HolyStrength(tagNS_HolyStrength* pmsg, LPVOID pPrarm);
	DWORD OnNS_HolyEquipmentNumber(tagNS_HolyEquipmentNumber* pmsg, LPVOID pPrarm);

	TSFPTrunk<ShengLingFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI�ؼ�
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	IconStatic*					m_pIStcItemPic;		//ʥ��ͼ��
	GUIStatic*					m_pStcItemName;		//ʥ������
	ActiveProgress*				m_pProTunShi;		//���ɽ�����
	GUIStatic*					m_pStcTunShi;		//���ɴ���
	ActiveProgress*				m_pProLingNeng;		//���ܽ�����
	GUIStatic*					m_pStcLingNeng;		//������
	ActiveProgress*				m_pProMoQi;			//Ĭ��������
	GUIStatic*					m_pStcMoQi;			//Ĭ����
	GUIButton*					m_pBtnTunShi;		//���ɰ�ť
	GUIButton*					m_pBtnFusion;		//�ںϰ�ť(�˹���ò���Ȳ���)
	GUIStatic*					m_pStcShengLingDmg;	//ʥ���˺�
	GUIStatic*					m_pStcShengLingDef;	//ʥ�����
	GUIStatic*					m_pStcCrit;			//����
	GUIStatic*					m_pStcCritValue;	//������
	GUIStatic*					m_pStcDmgAdd;		//�˺�����
	GUIStatic*					m_pStcJingZhun;		//��׼
	GUIStatic*					m_pStcLingQiao;		//����
	IconStatic*					m_pIStcEquip[MaxHolyEquipNum];	//ʥ���6��װ��
	GUIButton*					m_pBtnChong[MaxHolyEquipNum];		//װ�����ܵ�6����ť
	GUIButton*					m_pBtnChai[MaxHolyEquipNum];		//��װ����6����ť
	GUIButton*					m_pBtnKeYin;		//��ӡ
	GUIStatic*					m_pStcShengLingName;

	//���ɿؼ�
	GUIWnd*						m_pWndTunShi;		
	ActiveProgress*				m_pProgress;			//���ɽ�����
	IconStatic*					m_pIStcShengLingIcon;	//ʥ��ͼ��
	ComposeStatic*				m_pCStcFairy;			//����ͼ��
	GUIButton*					m_pBtnEat;				//���ɰ�ť
	GUIButton*					m_pBtnTunShiCancel;		//ȡ��
	GUIButton*					m_pBtnTunShiClose;		//�رհ�ť
	GUIStatic*					m_pStcEatNum;			//���ɴ���
	GUIStatic*					m_pStcTunShiResult;		//���ɽ�������ã�һ��ɶ�ģ�
	GUIStatic*					m_pStcDmgNum;			//ʥ���˺���ֵ
	GUIStatic*					m_pStcDefNum;			//ʥ�������ֵ
	GUIStatic*					m_pStcCirtNum;			//������ֵ
	GUIStatic*					m_pStcCritValueNum;		//��������ֵ
	GUIStatic*					m_pStcAttackTecNum;		//��׼��ֵ
	GUIStatic*					m_pStcNeglectNum;		//������ֵ
	GUIStatic*					m_pStcDmgAddNum;		//�˺�����
	GUIStatic*					m_pStcLingNengNum;		

	//�ٻ��ؼ�
	GUIWnd*						m_pWndCall;	
	IconStatic*					m_pIStcCall;
	ActiveProgress*				m_pProShengLingValue;	//ʥ��ֵ������
	GUIStatic*					m_pStcShengLingValue;	//ʥ��ֵ

	//��ӡ�ؼ�
	GUIWnd*						m_pWndKeYin;
	IconStatic*					m_pIStcKShengLingIcon;	//ʥ��ͼ��
	ActiveProgress*				m_pProKTunShi;			//���ɽ�����
	GUIStatic*					m_pStcKTunShi;			
	ActiveProgress*				m_pProKLingNeng;		//���ܽ�����
	GUIStatic*					m_pStcKLingNeng;
	ComposeStatic*				m_pCStcNewEquipIcon;	//����װ��ͼƬ
	IconStatic*					m_pIStcOldEquipIcon[MaxHolyEquipNum];	//�Ѵ���װ��ͼ��
	GUIStatic*					m_pStcEquipName[MaxHolyEquipNum];		//װ������
	GUIStatic*					m_pStcShengLingLevel;	//Ҫ��ʥ���������
	GUIStatic*					m_pStcLingNengCost;		//��������
	GUIStatic*					m_pSteEquipAttName[X_ESAT_ATTA_NUM];
	GUIStatic*					m_pStcEquipAttValue[X_ESAT_ATTA_NUM];
	GUIButton*					m_pBtnEquipClose;		//�رս���
	GUIButton*					m_pBtnBeginKeYin;		//��ӡ
	GUIButton*					m_pBtnCancelKeYin;		//ȡ����ӡ
	GUIStatic*					m_pStcShengWenName;		//��ӡ����ʥ�����֡�����

	//���ܿؼ�
	GUIWnd*						m_pWndChongNeng;		
	GUIButton*					m_pBtnCClose;			//���ܹرհ�ť
	GUIButton*					m_pBtnBeginChongNeng;	//���ܰ�ť
	GUIButton*					m_pBtnChongNengCancel;	//ȡ������
	GUIStatic*					m_pStcRemainTimes;		//����ʣ�����
	GUIPushButton*				m_pPBtnBaseAtt;			//�������Գ���
	GUIPushButton*				m_pPBtnSpecAtt;			//�������Գ���
	GUIPatch*					m_pPchBastAtt;
	GUIPatch*					m_pPchSpecAtt;
	IconStatic*					m_pIStcCEquip;			//����װ��ͼƬ
	ComposeStatic*				m_pCStcCIMIcon;			//���ܵ���
	ComposeStatic*				m_pCStcCDisIMIcon;		//ƫ�����
	GUIStatic*					m_pStcAttAddValue;		//��������ֵ
	ComposeStatic*				m_pCStcCLingIMIcon;		//���ܵ���
	GUIStatic*					m_pStcCurLingNeng;		//��ǰ����
	GUIStatic*					m_pStcLingCost;			//�������ļ�����
	GUIComboBox*				m_pComBoxSelectAtt;		//ѡ��������Ͽ�
	ActiveProgress*				m_pProChongNengTime;	//���ܶ���

	//���ܳɹ�����
	GUIWnd*						m_pWndChongNengSucc;	//���ܳɹ�����
	GUIStatic*					m_pStcChongNengAtt1;	//ǿ������1
	GUIStatic*					m_pStcChongNengAtt2;	//ǿ������2
	GUIStatic*					m_pStcChongNengAttValue1;//ǿ���������ֵ1
	GUIStatic*					m_pStcChongNengAttValue2;//ǿ���������ֵ2
	GUIButton*					m_pBtnConfirmChongNeng;	//ȷ�ϳ���
	GUIButton*					m_pBtnAbortChongNeng;	//��������

	INT64						m_n64FairyID;			//�������Ե�������id
	INT64						m_n64KeYinEquipID;		//��ӡ�����װ��id

	BYTE						m_byXiYinIndex;			//��Ҫϴӡ������
	DWORD						m_dwXiYinItemID;		//���ĵ���

	Equipment*					m_pTempChongnengEquip;	//��Ҫ���ܵ�װ��
	INT64						m_n64ChongNengItemID;	//���ܵ���id
	INT64						m_n64PianXiangItemID;	//ƫ�����id
	INT64						m_n64LingNengItemID;	//���ܶ������id
	BYTE						m_byChongNengIndex;		//����װ��������
	BYTE						m_byLastSelectIndex;	//���һ��ѡ�������
	map<BYTE, BYTE>				m_mapSelectIndex;
	BOOL						m_bIsSendMsg;			//�Ƿ�����ʥ��ǿ������
	BOOL						m_bIsLingNengChongNeng;	//�Ƿ�ǿ������
};

#pragma once
#include "AvatarAniMixer.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\ItemDefine.h"

class Item;
struct tagNS_SoulEquip;
struct tagNS_UnSoulEquip;
struct tagNS_GetSoulInfo;
struct tagNS_ActivateHolySoul;
struct tagNS_HolySoulAttChange;
struct tagStarEnhanceProto;
struct tagNS_UpdateHolySoulExp;

struct tagHolyModelInfo
{
	DWORD	dwHolyManID;
	DWORD	dwSoulValue;
	tstring		strModelMen;
	tstring		strModelWomen;
	tstring		strModelEffect;
	INT			nModelScale;
};

struct tagUpdateHolyManEvent : public tagGameEvent
{
	//Ϊfalse����ȫ��ˢ��
	EHolyEquipAttType eAttType;  //ֻˢ��Ԫ��ֵ
	tagUpdateHolyManEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) 
		: tagGameEvent(szEventName,pSenderFrame)
		,eAttType(EHEAT_NULL)
	{}
};

enum EHolyEquipType		//Ԫ��װ�� type ex
{
	EHET_NULL				= 0,				//������
	EHET_TIANHUN		= 42,			//���
	EHET_DIHUN				= 43,			//�ػ�
	EHET_MINGHUN		= 44,			//����
	EHET_TIANCHONG	= 45,			//���
	EHET_LINGHUI			= 46,			//���
	EHET_QIPO				= 47,			//����
	EHET_LIPO				= 48,			//����
	EHET_ZHONGSHU		= 49,			//����
	EHET_JINGPO			= 50,			//����
	EHET_YINGPO			= 51,			//Ӣ��
};

//�������Ԫ�����
class HolyManMgr
{
public:
	HolyManMgr(void);
	~HolyManMgr(void);

	enum
	{ 
		HolyMan_SkillID1			=	99905,		//Ԫ���ٻ����� id  LV1
		//HolyMan_SkillID2			=	11126,		//Ԫ���ٻ����� id	LV2
		//HolyMan_SkillID3			=	11127,		//Ԫ���ٻ����� id	LV3
	};

	enum HolyCombatStateCheckType //ս��״̬���ܲ�������
	{
		EHCSCT_EQUIP		= 0,		//��װ
		EHCSCT_AFFUSE	= 1,		//��ע
		EHCSCT_REBORN	= 2,		//ת��
	};

	static HolyManMgr* Inst();
	void Init();
	void Destroy();

	//��������Ƿ�Ԫ�����
	BOOL IsHolyAwake(); 

	VOID CheckAwakeState();

	VOID AddSoulExp(INT64 AddValue);

	tstring GetMatchCombatAction(tstring strAct);

	EHolyEquipType IndexToType(INT nIndex);
	INT TypeToIndex( EHolyEquipType eType);

	//�ж�һ��װ���Ƿ�ΪԪ��װ��
	BOOL IsHolyManEquip(DWORD dwTypeID);
	//���Ԫ��װ���ӳ�������
	tstring GetHolyEquipAttTypeName(DWORD dwTypeID);
	//���Ԫ��װ���ӳ�����
	EHolyEquipAttType GetHolyEquipAttType(DWORD dwTypeID);
	//���Ԫ��װ���ӳ�����
	INT	GetHolyEquipAttValue(DWORD dwTypeID);
	//װ��Ԫ��װ��λ��
	EHolyEquipType GetHolyManEquipPos(DWORD dwTypeID);
	//ս��״̬���
	BOOL IsCombatState( HolyCombatStateCheckType eType = EHCSCT_EQUIP );
	//��װ��  ePos��Ԫ��װ����λ�� n16Pos�Ǳ���λ��
	VOID SendMsg_HolyEquip(EHolyEquipPos ePos, INT16 n16Pos);
	//��װ��  ePos��Ԫ��װ����λ�� n16Pos�Ǳ���λ��
	VOID SendMsg_UnHolyEquip(EHolyEquipPos ePos,INT16 n16Pos=GT_INVALID);
	//ˢ��Ԫ������ֵ��Ϣ
	VOID UpdateSoulAttValue( EHolyEquipAttType eType = EHEAT_NULL );

	//�� �Ա� �õ���Ӧ��Ԫ��ģ����Ϣ
	tstring GetHolyManSkelPath(DWORD dwModelID,BYTE  bySex);
	tstring GetHolyManSkelName(DWORD dwModelID,BYTE  bySex);
	float	  GetModelScale(DWORD dwHolyManID);

	VOID	ShowErrorTips(DWORD dwErr);

	VOID	ActiveHolyMan(){ m_bHolyManActive = TRUE;}
	BOOL	IsActiveHolyMan() { return m_bHolyManActive;}

	//Ԫ������ֵ
	VOID SetHolyManAtt(const tagHolySoulInfo info );
	const tagHolySoulInfo&	GetHolyManAtt(){ return m_SoulAttInfo; }
	//��ȡԪ��ֵ
	INT GetHolyManValue();
	//��Ԫ����Ϣ����
	VOID OpenHolyManFrame(BOOL bOpen = TRUE);

	BOOL HasUpdate(){ return  m_bIsUpdateExp;}
	VOID  NoUpdate(){ m_bIsUpdateExp = FALSE; }

	//Ԫ��װ��ǿ��,����Ϊװ�����ǵȼ�
	const tagStarEnhanceProto*	GetHolyComposeInfo(BYTE byLv);
	INT	GetStarEnhanceAttValue(BYTE byLv, EHolyEquipAttType eAttType);

	//ͨ��ת������(�Ͼ���¶)ID ���ת������ 
	INT  GetRateByItemNum(INT nNum);
	//��õ�ǰת���ȼ��ﵽ100����ת��������������
	INT  GetFullRateMinNum();
	//���ת���ȼ�
	INT  GetRebornLevel();
	//���Ԫ��ȼ�
	INT  GetHolyManLevel();
	//����item
	VOID UpdateItem(Item **pItem);
	//�Ƿ���Ԫ����Ѽ���
	BOOL IsHolyAwakeSkillID( DWORD dwID);
	//�Ƿ���Ҫ���Ԫ���ʼ����Ϣ
	VOID SetHolyInitState( BOOL b){ m_bLoginInit =b; }
	//�����λ����
	INT GetChiselCost(BYTE index);
	//�����λ����
	INT GetChiselRate(BYTE index);
	//��Ƕ��λ����
	INT GetInlayCost(BYTE index);
	//��Ƕ��λ����
	INT GetInlayRate(BYTE index);

private:
	BOOL LoadFromFile();

	DWORD OnNS_SoulEquip( tagNS_SoulEquip *pNetCmd, DWORD );
	DWORD OnNS_UnSoulEquip( tagNS_UnSoulEquip *pNetCmd, DWORD );
	DWORD OnNS_GetSoulInfo( tagNS_GetSoulInfo *pNetCmd, DWORD );
	DWORD OnNS_ActivateHolySoul( tagNS_ActivateHolySoul *pNetCmd, DWORD );
	DWORD OnNS_HolySoulAttChange( tagNS_HolySoulAttChange *pNetCmd, DWORD );
	DWORD OnNS_UpdateHolySoulExp(tagNS_UpdateHolySoulExp* pMsg, DWORD pPrama);

	VOID ClearHolyManInfoMap();
	VOID ClearHolyComposeMap();
	VOID HolyManMgr::ClearHolyRebornMap()
	{
		m_mapHolyRebornRate.clear();
	}
	VOID HolyManMgr::ClearHolyChiselMap()
	{
		m_mapHolyChisel.clear();
	}

	VOID HolyManMgr::ClearHolyInlayMap()
	{
		m_mapHolyInlay.clear();
	}


private:

	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TSFPTrunk<HolyManMgr>		m_trunk;

	BOOL										m_bHolyAwake;  // local player ��Ԫ��״̬�Ƿ����

	//��ɫ������ԭ����ӳ��
	std::map<tstring,tstring>			m_mapActionSyn; 

	std::map<DWORD,const tagHolyModelInfo*>  m_mapHolyManInfo;

	BOOL										m_bHolyManActive; //15���Ժ󼤻�Ԫ��

	tagHolySoulInfo		m_SoulAttInfo;

	BOOL								m_bIsUpdateExp; //�Ƿ��Ѿ�ˢ�¾������

	BOOL								m_bLoginInit;//�Ƿ������߳�ʼ��

	std::map<DWORD,const tagStarEnhanceProto*> m_mapHolyCompose;

	std::map<INT, tagHolySoulReborn>			m_mapHolyRebornRate;

	std::map<BYTE, tagHolySoulEquipHole>			m_mapHolyChisel;

	std::map<BYTE, tagHolySoulEquipInlay>			m_mapHolyInlay;


};

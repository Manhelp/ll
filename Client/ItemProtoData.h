#pragma once

#include ".\BaseData.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\suit_define.h"
#include "..\WorldDefine\pet_equip_define.h"
#include "..\WorldDefine\longhun_define.h"
#include "..\WorldDefine\compose_define.h"

struct tagSpinProtoList
{
    DWORD   dwBasic;
    DWORD   dwMiddle;
    DWORD   dwSenior;
};

// ������ʵ
struct tagFairyFruit
{
	DWORD dwTypeID;
	BYTE byMin;
	BYTE byMax;
	BYTE byType;
	BYTE byValue;
};
//������������add by xp
struct tagFairyUpgradeTimes
{
	DWORD dwID;
	BYTE byType;
	INT32 n32Money;
};

//! \struct tagTreasureProto
//! \brief �����䱦�б�,��itemid�������п�������ƷҲ�п�����װ��
//! \author hyu
struct tagTreasureProto
{
    DWORD		dwTreasureID;	//!< �����䱦id,����Ʒid�޹�
    DWORD		dwItemID;		//!< ��Ʒid,��itemid����
    INT			nConNeed;		//!< �����������幱��ֵ
    ECLanType	eCtype;			//!< ������������
    DWORD		dwShopID;		//!< �̵�ID

    tagTreasureProto() {ZeroMemory(this, sizeof(*this));}
};
//! \struct tagEquipQltyEffect
//! \brief װ��Ʒ��Ӱ���
//! \author hqzhang
struct tagEquipQltyEffect
{
	// ��������Ӱ��ϵ��
	FLOAT fWeaponFactor;
	FLOAT fArmorFactor;

	// һ������
	INT32 nAttAFactor;
	FLOAT fAttAFactor;
	INT32 nAttANumEffect;

	// Ǳ��ֵ
	FLOAT fPotFactor;

	// ��Ƕ������ -- ��¼���ֵļ���
	INT32 nHoleNumPct[MAX_EQUIPHOLE_NUM + 1];

	// �������� -- ����
	INT32 nLonghunPct[X_LONGHUN_TYPE_NUM];

	// ��������
	INT32 nSpecAttPct;

	// ����ǿ���ȼ�
	INT32 nMaxStrengthenLevel;
};

/** \class	ItemProtoData
	\brief	��Ʒ��װ��ԭʼ���Խṹ�洢
*/
class ItemProtoData
	: public BaseData
{
public:
	ItemProtoData(void);
	~ItemProtoData(void);

	static ItemProtoData* Inst();

	/** ���������ļ�
	*/
	virtual void LoadFromFile();
	/** ��ȡ��Ʒ�����ṹ
	*/
	const tagItemProto* FindItemProto(DWORD dwID)
	{
		if(MIsEquipment(dwID))
			return FindInMap(m_mapEquipProto, dwID);
	
		return FindInMap(m_mapItemProto, dwID);
	}
	/** ��ȡװ�������ṹ
	*/
	const tagEquipProto* FindEquipProto(DWORD dwID)
	{
		return FindInMap(m_mapEquipProto, dwID);
	}
	/** ��ȡ���������ṹ
	*/
	const tagFabaoProto* FindTrumpProto(DWORD dwID)
	{
		return FindInMap(m_mapTrumpProto, dwID);
	}
	/** ��ȡ��װ�����ṹ
	*/
	const tagSuitProtoClient* FindSuitProto(DWORD dwID)
	{
		return FindInMap(m_mapSuitProto, dwID);
	}
	/** ��ȡ��Ʒ����ʾ�ṹ
	*/
	const tagItemDisplayInfo* FindItemDisplay(DWORD dwID)
	{
		const tagItemProto* pProto = FindItemProto(dwID);
		if(P_VALID(pProto))
		{
			return pProto->pDisplayInfo;
		}
		return NULL;
	}
	//! ����䱦
	const tagTreasureProto* FindTreasureByTrID(DWORD dwID)
	{
		return FindInMap(m_mapTreasureProto, dwID);
	}

    //! ͨ��itemid���������ͻ�õ�һ���䱦ԭ��
	const tagTreasureProto* FindTreasureByItemID(DWORD dwID, ECLanType eType)
	{
        multimap<DWORD, tagTreasureProto>::_Paircc range;
        multimap<DWORD, tagTreasureProto>::const_iterator it;

        range = m_mapTreasureProtoByItemID.equal_range(dwID);
        for (it = range.first; it != range.second; ++it)
        {
            if (it->second.eCtype == eType)
            {
                return &it->second;
            }
        }

        return NULL;
	}

	//zxzhang  ͨ��װ��ID����ȡװ���������� 2010-1-8
	const tagEquipQltyPct* FindEquipQltyPctByItemID(DWORD dwID)
	{
		return FindInMap(m_mapEquipQltyPct, dwID);
	}
             

	const map<DWORD, tagTreasureProto>* GetTreasureProto()
	{
		return &m_mapTreasureProto;
	}

    //! ͨ����Ʒԭ��id��ó���װ��ԭ��
    const tagPetEquipProto* FindPetEquipByItemID(DWORD dwID);
    //! ͨ������װ��ԭ��id��ó���װ��
    const tagPetEquipProto* FindPetEquipByEquipID(DWORD dwID);

	const tagEquipQltyEffect* FindEquipQltyEffect(INT nQlty)
	{
		return FindInMap(m_mapEquipQlty, nQlty);
	}

	const tagHolyEquipProto* FindHolyEquipProto(DWORD dwID)
	{
		return FindInMap(m_mapHolyEquipProto, dwID);
	}

    ESuitAttType FindFashionSuitType(DWORD dwTypeID);
    ESpinStep FindFashionSpinStep(DWORD dwTypeID);
    DWORD FindNextFashionSpinID(DWORD dwTypeID, ESuitAttType eType);

    int FindFashionSpinRate(int itemCount, ESpinStep eStep);

	tstring FindTrumpAwardByID( DWORD dwID );

	tstring GetFairyEffect( DWORD dwTypeID, INT nLevel );
	tstring GetFairyPic( DWORD dwTypeID, INT nLevel );
	const tagFairyFruit* FindFairyFruit(DWORD dwTypeID);
	const tagFairySkill* FindFairySkill(DWORD dwTypeID);
	const tagFairyUpgradeTimes* FindFairyUpgradeTimes(DWORD dwTypeID);//add by xp

	const vector<DWORD>& GetFairyExpBall()
	{
		return m_vecFairyExpBall;
	}

	const tagIDBuyBack* FindIDBuyBack(DWORD dwID)	//����id��ѯ�ṹ��
	{
		return FindInMap(m_mapIDBuyBack, dwID);
	}
	int GetIDBuyBackLevel(DWORD dwExp)
	{
		const tagIDBuyBack* pTemp1 = FindIDBuyBack(1);
		const tagIDBuyBack* pTemp2;
		if(!P_VALID(pTemp1) || dwExp <= 0)
			return 0;

		if(dwExp <= pTemp1->dwExp)
			return 1;

		for(int i = 2; i <= 100; i++)
		{
			pTemp1 = FindIDBuyBack(i-1);
			pTemp2 = FindIDBuyBack(i);
			if(P_VALID(pTemp1) && P_VALID(pTemp2))
			{
				if(dwExp >= pTemp1->dwExp && dwExp < pTemp2->dwExp)
					return i-1;
			}
			if( i == 100)
				return 100;
		}

		return 0;
	}
	tagIDBuyBackEx* GetIDBuyBackEx()
	{
		return &m_tagIDBuyBackEx;
	}

	const tagEquipGrowName* FindEquipGrowProto(DWORD dwID)	//��ȡװ���ɳ�Proto
	{
		return FindInMap(m_mapEquipGrow, dwID);
	}

	BYTE GetEquipGrowLevel(DWORD dwExp, const tagEquipGrowName* pEquipGrowName)
	{
		if(!P_VALID(pEquipGrowName))
			return 0;

		DWORD dwTotalValue = 0;
		for(int i = 0; i < EQUIP_GROW_MAX_LEVEL; i++)
		{
			dwTotalValue = GetEquipGrowCurMaxExp(i, pEquipGrowName);
			if( dwExp < dwTotalValue)
				return i;

			if(i == EQUIP_GROW_MAX_LEVEL-1)
				return EQUIP_GROW_MAX_LEVEL - 1;
		}

		return 0;
	}

	DWORD GetEquipGrowCurMaxExp(BYTE byLevel, const tagEquipGrowName* pEquipGrowname)
	{
		if(byLevel < 0 || byLevel == 255)
			return 0;

		DWORD dwResult = 0;
		for(int i = 0; i <= byLevel; i++)
		{
			dwResult += pEquipGrowname->dwExp[i];
		}
		return dwResult;
	}

	const tagFairyFusionRetProto* GetFauryFusionRet(INT byScore)
	{
		map<DWORD, tagFairyFusionRetProto>::iterator it;
		for(it = m_mapFairyFusionRet.begin(); it != m_mapFairyFusionRet.end(); ++it)
		{
			if(byScore >= (*it).second.byLowScore && byScore <= (*it).second.byHighScore)
				return &((*it).second);
		}
		return NULL;
	}

	const tagSoulAttName* FindShenLingName(DWORD dwID)	//ʥ�����Ա�
	{
		return FindInMap(m_mapShengLingName, dwID);
	}

	const tagHolyEquipEnhance* FindHolyEquipEnhance(DWORD dwID)	//ʥ��ǿ����
	{
		return FindInMap(m_mapHolyEquipEnhance, dwID);
	}

	const tagHolyLevelUpProto* GetHolyLevelProto(DWORD dwSocre)
	{
		map<DWORD, tagHolyLevelUpProto>::iterator it;
		for(it = m_mapShengLingLevelUp.begin();it != m_mapShengLingLevelUp.end(); ++it)
		{
			if( dwSocre <= (*it).second.dwID )
				return &((*it).second);
		}

		return NULL;
	}

	const tagWarReward* GetWarReward(DWORD dwLevel)
	{
		return FindInMap(m_mapWarReward, dwLevel);
	}

private:
	map<DWORD, tagItemProto>		    m_mapItemProto;				//��Ʒԭʼ�ṹ
	map<DWORD, tagEquipProto>		    m_mapEquipProto;			//װ��ԭʼ�ṹ
	map<DWORD, tagFabaoProto>			m_mapTrumpProto;			//����ԭʼ�ṹ
	map<DWORD, tagSuitProtoClient>	    m_mapSuitProto;				//��װԭʼ�ṹ
	map<DWORD, tagTreasureProto>	    m_mapTreasureProto;			//�����䱦ԭʼ����
	map<DWORD, tagTrumpWuXingAward>	    m_mapTrumpWuXingAward;		//���н���ԭʼ�ṹ
	map<DWORD, tagFairyEffect>			m_mapFairyEffect;			//������Ч
	multimap<DWORD, tagTreasureProto>	m_mapTreasureProtoByItemID;	//ͨ��itemid���������䱦
    map<DWORD, tagPetEquipProto>        m_mapPetEquipProtoByID;     //ͨ������װ��id����װ��
	map<INT, tagEquipQltyEffect>		m_mapEquipQlty;				//װ��Ʒ��Ӱ���
	map<DWORD, tagSoulAttName>			m_mapShengLingName;			//ʥ�����Ա� 
	map<DWORD, tagHolyLevelUpProto>		m_mapShengLingLevelUp;		//ʥ��������
	map<DWORD, tagHolyEquipProto>		m_mapHolyEquipProto;		//ʥ��װ����
	map<DWORD, tagHolyEquipEnhance>		m_mapHolyEquipEnhance;		//ʥ��ǿ�����߱�
	map<DWORD, tagWarReward>			m_mapWarReward;				//ս��������

	//zxzhang װ����������ԭʼ���Ա�  2010-1-8
	map<DWORD, tagEquipQltyPct>			m_mapEquipQltyPct;		// װ����������ԭʼ���Ա�

    // ʱװ����ԭʼ����
    map<DWORD, ESuitAttType>            m_mapSpinSuitType;      // ʱװTypeId����װ���Ͷ��ձ�

    map<pair<DWORD, ESuitAttType>, tagSpinProtoList> m_mapDirector; // ����ʱװ�ɳ�����

    map<pair<DWORD, ESuitAttType>, DWORD>        m_mapSpinNull;          // �޾���ʱװ(valueΪ��������ʱװ)
    map<pair<DWORD, ESuitAttType>, DWORD>        m_mapSpinBasic;         // ��������ʱװ(valueΪ�м�����ʱװ)
    map<pair<DWORD, ESuitAttType>, DWORD>        m_mapSpinMiddle;        // �м�����ʱװ(valueΪ�߼�����ʱװ)

    set<DWORD>                          m_SetSpinBasic;         // �������ĵ�ʱװ
    set<DWORD>                          m_setSpinMiddle;        // �м����ĵ�ʱװ
    set<DWORD>                          m_setSpinSenior;        // �߼����ĵ�ʱװ

    map<pair<int, ESpinStep>, int>      m_mapSpinRate;          // ʱװ���Ļ���

	map<DWORD, tagFairyFruit> m_mapFairyFruit;	// ����ˮ��
	map<DWORD, tagFairySkill> m_mapFairySkill;	// ��������
	map<DWORD, tagFairyUpgradeTimes> m_mapFairyUpgradeTimes;	// �������Ӵ���add by xp
	vector<DWORD> m_vecFairyExpBall;

	map<DWORD, tagIDBuyBack> m_mapIDBuyBack;	//�˺Żع�
	tagIDBuyBackEx m_tagIDBuyBackEx;

	map<DWORD, tagEquipGrowName> m_mapEquipGrow;	//װ���ɳ����Ա�
	map<DWORD, tagFairyFusionRetProto> m_mapFairyFusionRet;	//�����ںϼ��ʱ�
};
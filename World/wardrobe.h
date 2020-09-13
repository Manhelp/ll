//-------------------------------------------------------------------------------------
// �³�ϵͳ
//-------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/fashion_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/msg_fashion.h"

enum EWardrobeLayerType
{
	EWLT_Null = 0,
	EWLT_Normal = 1, // �����ָ��ʱװ
	EWLT_Diy = 2,		   // �����³���

	EWLT_MAX
};

enum EWardrobeLayerSex
{
	EWLS_Null = 0,
	EWLS_Men = 1,
	EWLS_Women = 2,

	EWLS_MAX
};

#define IsWardrobeLayerTypeValid(type) (((type) > EWLT_Null) && ((type) < EWLT_MAX))
#define IsWardrobeLayerSexValid(type,sex) ((((sex) > EWLS_Null) && ((sex) < EWLS_MAX)) || (((type)==EWLT_Diy)&&((sex)==EWLS_Null)))

const INT32 MAX_FASHION_PERLAYER = EWFP_End;		// ÿ����λʱװ����
const INT32 MAX_LEVEL_PERLAYER = 3;		// ÿ����λ��ߵȼ�
#define PLAYACT_CDTIME 3600
#define PLAYACT_REQUEST_TIMEOUT 30
#define CalLayerID(Idx) (DWORD)((Idx/MAX_FASHION_PERLAYER)+1)

struct tagWardrobeProto
{
	DWORD dwLayerID;			// �³���λID
	EWardrobeLayerType eLayerType;
	EWardrobeLayerSex eLayerSex;
	DWORD dwFashionID[MAX_FASHION_PERLAYER];
	DWORD dwBuffIDByLvl[MAX_LEVEL_PERLAYER];
	DWORD dwOtherBuffIDByLvl[MAX_LEVEL_PERLAYER];
};

struct tagWardrobeLayer
{
	DWORD dwLayerID;
	BYTE byLevel;						// �³����ȼ�
	EFashionState eState;			// �³���״̬
	tagDWORDTime dwPlayActStartTime;		// װ�翪ʼʱ��
	DWORD dwFriendID;									// װ��ĺ���ID
	BYTE byFashionTemperament;		// ʱװ����ֵ
	tagWardrobeLayer(DWORD dwLayer)
		:byLevel(1),eState(EFS_None),dwPlayActStartTime(0),dwFriendID(GT_INVALID),dwLayerID(dwLayer),byFashionTemperament(0)
	{}
	tagWardrobeLayer(BYTE byLvl, EFashionState e, tagDWORDTime dwStartTime, DWORD dwLayer, BYTE byFT, DWORD dwFID)
	{
		byLevel = byLvl;
		eState = e;
		dwPlayActStartTime = dwStartTime;
		dwLayerID = dwLayer;
		byFashionTemperament = byFT;
		dwFriendID = dwFID;
	}
};

class Wardrobe
{
public:
	typedef TMap<DWORD, tagWardrobeLayer*> WardrobeInfoMap;
	typedef TMap<DWORD, DWORD> PlayActMap;
public:
	Wardrobe(DWORD dwRoleID);
	~Wardrobe();

	DWORD PlayActMyself(DWORD dwLayerID);

	// �ı��³���λ״̬
	VOID ChangeLayerState(DWORD dwLayer, EFashionState eFashionState);

	// �����³�
	DWORD LevelUpWardrobeLayer(Role* pRole, DWORD dwLayerID);

	// ��ʼװ�����
	DWORD StartPlayActFriend(DWORD dwLayerID, DWORD dwFriendID, INT nPlayActFriendLimit);

	// ����װ�����
	DWORD EndPlayActFriend(DWORD dwFriendID, BYTE& byLevel, DWORD& dwLayerID, BOOL bAgree);

	// �����³�״̬
	VOID Update();

	// ��ǰʱװ��װ�ʹ�װ����
	DWORD PutOffCurFashion();
	DWORD PutOnCurFashion(Role* pRole, BOOL bInitPutOnFashion = FALSE);
	DWORD PutOnBePlayActFashion(Role* pRole, BOOL bInitPutOnFashion = FALSE);

	// ��װ����
	DWORD PutOnFashion(Role* pRole, DWORD dwWardrobeLayerID, BYTE byLevel, BOOL bSelf = TRUE, BOOL bSendMsg = TRUE, BOOL bInitPutOnFashion = FALSE);

	BOOL CanAddFashion(INT16 n16TargetIndex, tagItem* pItem);
	VOID SetFashionTemperament(DWORD dwLayerID, BYTE byFashionTemperament);
	BYTE GetCurFashionTemperament();
	VOID SyncWardrobeInfo(Role* pRole);
	VOID InitWardrobe(const BYTE* &pData, DWORD dwLayerCnt, Role* pRole);
	VOID CancelBePlayActing(Role* pRole);

	DWORD GetCurWareLayer() { return m_dwCurWareLayer; }
	tagWardrobeLayer* GetWardrobeLayer(DWORD dwLayerID) { return m_mapWardrobeInfo.Peek(dwLayerID); }
	BOOL IsPlayActing() { return (m_dwCurWareLayer != GT_INVALID); }
	BOOL IsBePlayActing() { return m_dwBePlayActLayerID != GT_INVALID; }
	VOID SetBePlayActLayerID(DWORD dwBePlayActLayerID) { m_dwBePlayActLayerID = dwBePlayActLayerID; }
	VOID SetBePlayActLevel(BYTE byBePlayActLevel) { m_byBePlayActLevel = byBePlayActLevel; }
	DWORD GetBePlayActLayerID() { return m_dwBePlayActLayerID; }
	BYTE GetBePlayActLevel() { return m_byBePlayActLevel; }

	// ��黻װ
	VOID MaggriageMakeUp(DWORD dwRoleID, DWORD dwLayerID);

	VOID GetFashionTemperamentSum(INT& mQZSum, INT& nSuit1Sum, INT& nSuit2Sum, INT& nSuit3Sum, INT& nFashionNum);
private:
	VOID InitWardrobe(Role* pRole, DWORD dwLayerCnt, tagWardrobeLayer2DB* pWardrobeLayer);
	VOID UpdatePlayActCDTime();
	VOID UpdatePlayActRequest();
	VOID UpdateBePlayAct();
	VOID SyncLayerInfo(DWORD dwLayerID, BYTE byLevel, EFashionState eFashionState);
	VOID FixWardrobeLayerState(tagWardrobeLayer* pNewWardrobeLayer, Role* pRole);
	VOID SaveWardrobeLayerInfo2DB(DWORD dwLayerID);
private:
	DWORD m_dwRoleID;
	WardrobeInfoMap m_mapWardrobeInfo;
	DWORD m_dwCurWareLayer;				// ��ǰ������λ
	PlayActMap m_mapPlayAct;				// ��ǰ������װ����б�
	
	// ����װ���������
	DWORD m_dwPlayActingFriend;		// ��װ����ѵ�ID
	DWORD m_dwPlayActingLayerID;		// ��װ����ѵ�LayerID
	tagDWORDTime m_dwPlayActRequestStartTime;

	// ��������װ��
	DWORD m_dwBePlayActLayerID;			// ������װ��
	BYTE m_byBePlayActLevel;				// ������װ��
};


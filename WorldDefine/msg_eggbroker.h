//////////////////////////////////////////////////////////////////////////
//�ҽ�
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push, 1)

#define MAX_EGG_LAYER 7
#define MAX_EGG_PER_LAYER 7
#define MAX_COLOUR_EGG 2
#define IsEggPosValid(layer, pos) (((pos)>=0)&&(pos)<(MAX_EGG_PER_LAYER-(layer)))
#define IsEggLayerValid(layer) (((layer)>=0)&&((layer)<MAX_EGG_LAYER))

struct tagEggStartInfo
{
	int nLayer;				// ����	����-1��ʾû����Ϣ��
	int nPos;					// λ��
	bool bIsColourEgg;	// �ǲ��ǲʵ�
	DWORD dwItemID;	// ��ƷID
};

enum EEggBrokerErrorCode
{
	EEBEC_Success = E_Success,
	EEBEC_TopLayer,		// �Ѿ��ҵ�����
	EEBEC_EmptyEgg,	// �ҵ��յ�
	EEBEC_HammerNotEnough,	// ���Ӳ���
	EEBEC_CannotBreak,		// �˵�������
	EEBEC_ChooseNoItem,	// ѡ�еĲ���û�б���
	EEBEC_BaiBao,		// ���͵��ٱ�������
};

//-----------------------------------------------------------------------------
// ���ҽ𵰽���
//-----------------------------------------------------------------------------
CMD_START(NC_EggBegin)
CMD_END

CMD_START(NS_EggBegin)
	int	nCurrLayer;		// ��ǰ�ǵڼ���(����ǵ�һ���������Ϣ���ټ��ؽ��ͻ���)
	int	nColourEggPos_1;		//	�ʵ�λ��1��-1Ϊû�У�	��ǰ���вʵ���λ��
	int	nColourEggPos_2;		// �ʵ�λ��2							��ǰ���вʵ���λ��
	tagEggStartInfo	EggStartInfo[MAX_EGG_LAYER];
CMD_END

//-----------------------------------------------------------------------------
// �ҵ�
//-----------------------------------------------------------------------------
CMD_START(NC_BreakEgg)
	int nLayer;
	int nPos;
CMD_END

CMD_START(NS_BreakEgg)
	DWORD dwErrorCode;
	int nLastEggLayer;			// �ղ��ҵĵ�����һ�е�
	int nLastEggPos;				// �ղ��ҵĵ�λ��
	bool bLastEggIsColour;		// �ղ��ҹ��ĵ��ǲ��ǲʵ����������true��
	int nNextLayer;				// ����Ӧ���ҵĲ���
	int nColourEggPos_1;		//	�ʵ�λ��1��-1Ϊû�У�
	int nColourEggPos_2;		// �ʵ�λ��2
	DWORD dwItemID;			// �ո��ҵ���õ���ƷID
CMD_END

//-----------------------------------------------------------------------------
// ȡ����
//-----------------------------------------------------------------------------
CMD_START(NC_TakeEggTreasure)
	int nlayer;		// �ڼ���ı���
CMD_END

CMD_START(NS_TakeEggTreasure)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// ȡ�������߼�������ر�
//-----------------------------------------------------------------------------
CMD_START(NC_FinishEggGame)
CMD_END

#pragma pack(pop)
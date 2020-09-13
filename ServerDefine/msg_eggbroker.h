//-----------------------------------------------------------------------------
// brief: �ҽ�
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

struct tagEggInfo
{
	int nBrokePos;					// �˲����е�λ��
	DWORD dwItemID;			// �ҳ�����ƷID
	tagEggInfo():nBrokePos(GT_INVALID),dwItemID(0)
	{}
};

struct tagEggInfoInDB
{
	INT nCurLayer;			// ��ǰ�����ҵĲ�
	INT16 n16ColourEggPos1;       // �ʵ�λ��1
	INT16 n16ColourEggPos2;       // �ʵ�λ��2
	tagEggInfo sBrokenEggInfo[MAX_EGG_LAYER];     // �Ѿ��ҹ��Ľ���Ϣ
};

// ��Ϣ����
CMD_START(NDBC_SaveEggInfo)
	DWORD dwRoleID;
	INT nCurLayer;			// ��ǰ�����ҵĲ�
	INT16 n16ColourEggPos1;       // �ʵ�λ��1
	INT16 n16ColourEggPos2;       // �ʵ�λ��2
	tagEggInfo sBrokenEggInfo[MAX_EGG_LAYER];     // �Ѿ��ҹ��Ľ���Ϣ
CMD_END

CMD_START(NDBC_ClearEggInfo)
	DWORD dwRoleID;
CMD_END


#pragma pack(pop)
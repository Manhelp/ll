// �³����DBServer��Ϣ
#pragma once

#pragma pack(push, 1)

struct tagWardrobeLayer2DB
{
	DWORD dwRoleID;
	DWORD dwLayerID;
	BYTE byLevel;						// �³����ȼ�
	BYTE byState;						// �³���״̬
	tagDWORDTime dwPlayActStartTime;		// װ�翪ʼʱ��
	DWORD dwFriendID;									// װ��ĺ���ID
};

// ����һ����¼
CMD_START(NDBC_SaveWardrobeLayerInfo)
	tagWardrobeLayer2DB WardrobeLayerInfo;
CMD_END

#pragma pack(pop)
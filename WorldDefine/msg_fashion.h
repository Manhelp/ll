// ʱװϵͳ�ḻ��2010.12.29��
#pragma once


#pragma pack(push, 1)
//--------------------------------------------------------------
// ��Ϣ����
//--------------------------------------------------------------

enum eClothespressValue
{
	E_WardrobeLevelUp_Fail = 1, // �����³�ʧ��
	E_WardrobeLevelUp_NoItem1, // û����̴ľ
	E_WardrobeLevelUp_NoItem2, // û����ľ
};
// ��װ
CMD_START(NC_Rehanding)
	DWORD dwWardrobeLayerID;
	bool bTakeOffFashion; // �Ƿ�����װ
CMD_END
CMD_START(NS_Rehanding)
	DWORD dwErrorCode;
CMD_END

// �����³���λ
CMD_START(NC_WardrobeLevelUp)
	DWORD dwWardrobeLayerID;
CMD_END
CMD_START(NS_WardrobeLevelUp)
	DWORD dwErrorCode;
CMD_END

// װ����ѣ��ͻ���֪ͨ��������Ҫ������һ���ͻ���װ�磩
CMD_START(NC_DressUpAnotherRole)
	DWORD dwTargetRoleID;
	DWORD dwWardrobeLayerID;
CMD_END

// װ����ѣ�������֪ͨ�ͻ��ˣ�Ŀ��ͻ����Ƿ�ͬ��װ�磩
CMD_START(NS_DressUpAnotherRole)
	DWORD 	dwErrorCode;
	DWORD 	dwSrcRoleID;
	DWORD 	dwWardrobeLayerID;
	bool			bAgree;
CMD_END

// װ����ѣ�������֪ͨ�ͻ��ˣ�ѯ�ʿͻ����Ƿ�ͬ��װ�磩
CMD_START(NS_IsDressUpAgree)
	DWORD 	dwSrcRoleID;
	DWORD 	dwWardrobeLayerID;
CMD_END

// װ����ѣ��ͻ���֪ͨ���������Ƿ�ͬ��װ�磩
CMD_START(NC_IsDressUpAgree)
	DWORD dwTargetRoleID;
	bool			bAgree;
CMD_END

// ͬ���³�ʱװ��Ϣ
CMD_START(NS_SyncFashionInfo)
	INT nSuitNum;				// ʱװ����
	BYTE FashionInfo[1];		// ����ʱװ��Ϣ
CMD_END

#pragma pack(pop)
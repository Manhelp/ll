//-----------------------------------------------------------------------------
//!\file msg_appearance_transform.h
//!
//!\date 2010-5-26
//!
//!\brief �ͻ����������֮���������ʦ����Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

#define APPEARANCE_TRANSFORM_ITEM		3070066	// ���ݾ����ID
#define APPEARANCE_TRANS_ITEM_EXPEND	1		// ÿ�������������ݾ����


// ������ش����붨��
enum E_Appearance_Transform_Error
{
	EATE_Success = 0,
	EATE_NoTransformItem = 1,	// û�����ݾ����
	EATE_SendBigVisTileMsg = 2,	// ���Ź����������ҷ��������������
};


// �ͻ���-->������ ��ɫ��������
CMD_START(NC_AppearanceTransform)
	WORD	wHairMdlID;			// ����
	WORD	wHairTexID;			// ��ɫ
	WORD	wFaceMdlID;			// �沿���
	WORD	wFaceDetailTexID;	// �沿ϸ��
	DWORD	dwNPCID;			// NPCID
CMD_END


// ������-->�ͻ��� �������ݽ��
CMD_START(NS_AppearanceTransform)
	DWORD	dwErrorCode;
	DWORD	dwRoleID;			// ���ݽ�ɫ�Ľ�ɫID
	WORD	wHairMdlID;			// ����
	WORD	wHairTexID;			// ��ɫ
	WORD	wFaceMdlID;			// �沿���
	WORD	wFaceDetailTexID;	// �沿ϸ��
CMD_END


#pragma pack(pop)
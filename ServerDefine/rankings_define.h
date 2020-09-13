#pragma once

#pragma pack(push, 1)

#include "../WorldDefine/msg_rankings.h"
#include "../WorldDefine/clergy_define.h"

struct tagRankingsNode
{
	INT				nNum;						// �ý�ɫ����	
	INT				nPreNum;					// �ý�ɫ�ϴ�����	
	TCHAR			szRoleName[X_SHORT_NAME];	// ��ɫ����
	DWORD			dwRoleID;					// ��ɫID
	ERankingType	eType;						// ��������
	INT64			n64Value;					// ֵ

	tagRankingsNode()
	{
		ZeroMemory( this, sizeof(tagRankingsNode) );
	}
};

struct tagPetRankingsNode
{
	INT				nNum;						// �ý�ɫ����	
	INT				nPreNum;					// �ý�ɫ�ϴ�����	
	TCHAR			szPetName[X_SHORT_NAME];	// ��������
	DWORD			dwRoleID;					// ��ɫID
	ERankingType	eType;						// ��������
	DWORD			dwPetID;					// ����ID
	INT64			n64Value;					// ֵ
	//INT64			n64Test;

	tagPetRankingsNode()
	{
		ZeroMemory( this, sizeof(tagPetRankingsNode) );
	}
};

struct tagClergyNode
{
	INT				nNum;						// �ý�ɫ����	
	//INT			nPreNum;					// �ý�ɫ�ϴ�����	
	//TCHAR			szRoleName[X_SHORT_NAME];	// ��ɫ����
	DWORD			dwRoleID;					// ��ɫID
	EClergyType		eClergy;					// ְλ
	DWORD			dwPoint;					// ֵ
	BOOL			bRecv;						// �Ƿ���ȡ

	tagClergyNode()
	{
		ZeroMemory( this, sizeof(tagClergyNode) );
	}
};

#pragma pack(pop)
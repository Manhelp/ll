//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_define.h
// author: twan
// actor:
// data: 2010-01-28
// last:
// brief: ��սϵͳ����
//-----------------------------------------------------------------------------
#pragma once
#include "msg_common_errorcode.h"
#include "city_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------
// �ж��Ƿ�Ϊ��ս��ͼ�����������ӣ�
//----------------------------------------------------------------
#define		IsCityStruggleMap(dwMapID)	( (dwMapID) == TObjRef<Util>()->Crc32(_T("c03")) )

const INT32 LONGZHU_SEIZED_COUNT_DOWN		= 20;				// ���鱻��ӡ���ս�����ĵ���ʱ�����ӣ�
const INT32 CITY_DOOR_REPAIR_COUNT_DOWN		= 20;				// ���Ż��ƺ��޸�����ʱ�����ӣ�
const INT32 MAX_YINYANGZHU_NUM				= 5;				// ��ս����������
const INT32 MAX_CITY_DOOR_NUM				= 3;				// ��������
const INT32 NOT_REPEAT_LIMIT_ROLE_NUM		= 3;				// ���ӻ�ɱ���������ʱҪ���Ǵ�ǰ���ظ���n�����
const INT32 MAX_CS_RANK_NUM					= 20;				// ��սս��������а������
const INT32 CITY_STRUGGLE_ACT_ID			= 70;				// ��ս��Ӧ�ID

const INT32 ADD_SCORE_LIMIT[7]				= { 60, 100, 255, 255, 30, 10, 3 };		// ���ӳ�ս���ֵĸ�������

// ��սʱ״̬�ı������
enum ECityStruggleChangeType
{
	ECSCT_PlayerSumChange,					// ��սһ�������ı�
	ECSCT_LongZhuBeSealing,					// ���鱻��ӡ��
	ECSCT_LongZhuCancelSeal,				// ����ȡ����ӡ
	ECSCT_LongZhuBeSeized,					// ���鱻��һ��ռ��
	ECSCT_YinYangZhuSeized,					// ����������һ��ռ��
	ECSCT_WuZiKu,							// �����������ʿ�
	ECSCT_DoorDestroied,					// ���ű�����
	ECSCT_DoorRepaired,						// �����޸�
};

// ����״̬�ṹ
struct tagLongZhuState
{
	ECityForceType		eForce;				// ��ǰռ�����������
	BOOL				bSealing;			// �Ƿ����ڱ���ӡ
	INT32				nCountDown;			// ���ս��������ʱ�������·��ĵ���ʱ��
};

// ������״̬�ṹ
struct tagYinYangZhuState
{
	ECityForceType		eForce;				// ��ǰռ��������������
};

// ����״̬�ṹ
struct tagCityDoorState
{
	BOOL					bDestroied;		// �Ƿ��ѱ�����
	INT32					nCountDown;		// ���Ÿ�ԭ����ʱ�����ӣ�
	INT32					nDoorType;		// �������ͣ�1-���ţ�2-���ţ�3-����		���˴�û����ö�٣���֪��ϵͳ����vs��bug��
};

// ��ս���ͬ���ͻ������ݽṹ
struct tagCSRoleData2Client
{
	INT					nScore;				// ��ս����
	BYTE				byActiveTime;		// ��Ծʱ��
	BYTE				byKills;			// ��ɱ����
	BYTE				byDestroyDoor;		// ���ݳ��Ŵ���
	BYTE				byDestroyMaterial;	// �������ʿ����
	BYTE				byUseRepairStone;	// ʹ���޸�ʯ�ϴ���
	BYTE				byOpenYinYang;		// ��������������
	BYTE				byOpenLongZhu;		// �����������

	tagCSRoleData2Client()
	{
		ZeroMemory(this, sizeof(*this));
	}

	tagCSRoleData2Client& operator= (tagCSRoleData2Client& other)
	{
		if (this == &other)
			return *this;

		memcpy(this, &other, sizeof(other));
		return *this;
	}
};

// ��սս��������Ϣ
struct tagCSRoleRankData
{
	DWORD						dwRoleID;			// ���ID
	ECityForceType				eForce;				// ��������
	INT32						nScore;				// ����
	BYTE						byKills;			// ��ɱ����
};

// ��ս������ݽṹ
struct tagCSRoleData
{
	tagCSRoleData2Client		RoleData;			// ͬ�����ͻ��˵�����
	TList<DWORD>				listKill;			// ��Ҵ�ǰɱ����n�����ظ����RoleID

	INT							nExploit;			// ���ɹ�ѫ
	INT							nContribute;		// ���ɹ���
	INT							nMoney;				// ��Ǯ����

	tagCSRoleData():RoleData(), listKill(), nExploit(0), nContribute(0), nMoney(0)	{}
	
	tagCSRoleData& operator= (tagCSRoleData& other)
	{
		if (this == &other)
			return *this;

		this->RoleData = other.RoleData;
		this->listKill = other.listKill;
		this->nExploit = other.nExploit;			// ���ɹ�ѫ
		this->nContribute = other.nContribute;		// ���ɹ���
		this->nMoney = other.nMoney;				// ��Ǯ����

		return *this;
	}
};

// ��ս������ݽṹö��
enum ECityStruggleRoleData
{
	ECSRD_ActiveTime,
	ECSRD_Kills,
	ECSRD_DestroyDoor,
	ECSRD_DestroyMaterial,
	ECSRD_UseRepairStone,
	ECSRD_OpenYinYang,
	ECSRD_OpenLongZhu,
	ECSRD_GuildExploit,
	ECSRD_GuildContribute,
	ECSRD_GuildMoney,
};

// ��ս���
enum ECityStruggleResult
{
	ECSRT_None,
	ECSRT_Atk,
	ECSRT_Def,
};

#pragma pack(pop)
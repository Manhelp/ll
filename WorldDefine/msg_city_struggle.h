//-----------------------------------------------------------------------------
//!\file msg_city_struggle.h
//!
//!\date 2009-09-24
//!\author	twan
//!\brief �ͻ����������֮����ڳ�ս����Ϣ
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"
#include "city_struggle_define.h"

//----------------------------------------------------------------------------
// ��ҽ����ս����
//----------------------------------------------------------------------------
CMD_START(NS_RoleEnterCityStruggle)
	INT32	nNum;						// �ж���Ӫ��������
	DWORD	dwEnemyGuildID[1];			// �ж���Ӫ����ID
CMD_END

//----------------------------------------------------------------------------
// ����뿪��ս����
//----------------------------------------------------------------------------
CMD_START(NS_RoleLeaveCityStruggle)

CMD_END

//----------------------------------------------------------------------------
// ��ս��Ϣͬ�����ͻ��ˣ�������Ϣ��
//----------------------------------------------------------------------------
CMD_START(NS_CityStruggleInfo)
	tagLongZhuState		sLongZhuState;		// ���鵱ǰ״̬
	tagYinYangZhuState	sYinYangZhuState[MAX_YINYANGZHU_NUM];		// ��������ǰ״̬
	tagCityDoorState	sDoorState[MAX_CITY_DOOR_NUM];				// ��������
	INT32				nAtkPlayerNum;		// �������������
	INT32				nDefPlayerNum;		// �������������
	INT32				nWuZiKuNum;			// �����������ʿ�����
CMD_END

//----------------------------------------------------------------------------
// ��ս��Ϣͬ�����ͻ��ˣ������ı����Ϣ��
//----------------------------------------------------------------------------
/*
	��ͬ�����������ݲ�ͬ��
	ECSCT_PlayerSumChange		-->		nType2Ϊ����ö�٣�nDataΪ����
	ECSCT_LongZhuBeSealing		-->		nType2Ĭ��Ϊ0�������壬nDataΪECityForceTypeö��
	ECSCT_LongZhuCancelSeal		-->		nType2Ĭ��Ϊ0�������壬nDataΪECityForceTypeö��
	ECSCT_LongZhuBeSeized		-->		nType2Ϊ�����ӡ�󵹼�ʱ����λ���ӣ���nDataΪECityForceTypeö��
	ECSCT_YinYangZhuSeized		-->		nType2Ϊ��������������TypeID��nDataΪ������ռ������ECityForceTypeö��
	ECSCT_WuZiKu				-->		nType2Ĭ��Ϊ0�������壬nDataΪ���ʿ�����
	ECSCT_DoorDestroied			-->		nType2ΪECityStruggleDoorPosö�٣���ʾ���ŷ�λ��nDataΪ���޸�ʱ�䣬��Ϊ-1���޷��ٴ��޸�
	ECSCT_DoorRepaired			-->		nType2ΪECityStruggleDoorPosö�٣���ʾ���ŷ�λ��nDataĬ��Ϊ0
*/
CMD_START(NS_CityStruggleInfoChange)
	ECityStruggleChangeType		eType1;			// ����1
	INT32						nType2;			// ����2
	INT32						nData;			// ״̬�ı������
CMD_END

//----------------------------------------------------------------------------
// ��ս����ս��
//----------------------------------------------------------------------------
CMD_START(NC_CSRoleRecord)

CMD_END

CMD_START(NS_CSRoleRecord)
	tagCSRoleRankData			RoleRank[MAX_CS_RANK_NUM];	// ս������
	tagCSRoleData2Client		RoleData;					// ����ս��
	ECityStruggleResult			eResult;					// ��ս���
	INT							nExploit;					// ���ɹ�ѫ
	INT							nContribute;				// ���ɹ���
	INT							nMoney;						// ��Ǯ����
CMD_END

//----------------------------------------------------------------------------
// �ȴ��ٻ�
//----------------------------------------------------------------------------
CMD_START(NC_WaitSummon)
	DWORD	dwCreatureID;			// �ػ�ʯID
CMD_END

CMD_START(NS_WaitSummon)
	BOOL	bSuccess;				// �Ƿ�ɹ������ػ�ʯ
CMD_END

//----------------------------------------------------------------------------
// ȡ���ٻ�
//----------------------------------------------------------------------------
CMD_START(NC_CancelSummon)
	DWORD	dwCreatureID;			// �ػ�ʯID
CMD_END

//----------------------------------------------------------------------------
// �ٻ������ı�
//----------------------------------------------------------------------------
CMD_START(NS_SummonPersonChange)
	INT32	nPerson;				// ��ǰ�ٻ�����
	INT32	nNeedPerson;			// ��Ҫ����
CMD_END

//----------------------------------------------------------------------------
// ��ʼ�ٻ�
//----------------------------------------------------------------------------
CMD_START(NS_BeginSummon)
	INT		nCountDown;				// �ٻ�ʱ��
CMD_END

#pragma pack(pop)
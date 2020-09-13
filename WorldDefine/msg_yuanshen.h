//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_soul.h
// author: 
// actor:
// data: 2011-06-9
// last:
// brief: �ͻ��˺ͷ������˼���Ϣ -- Ԫ��
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "ItemDefine.h"
#include "container_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// Ԫ��װ�������Ϣ(ע��ṹ���ֶ�˳����ñ�֤�ڴ����)
//----------------------------------------------------------------------------
enum EHolySoulErrorCode
{

	EHolySoul_NotActivated               =  1,           //Ԫ��Ϊ������
	EHolySoul_NextLevelNoExist           =  2,           //��һ�ȼ���Ԫ�񲻴���
	EHolySoul_EquipHolySoulFailed		 =  3,           //����Ԫ��װ��ʧ��
	EHolySoul_UnEquipHolySoulFailed      =  4,			 //����Ԫ��װ��ʧ��
	EHolySoul_BagItemNotFound            =  5,           //����û��Ԫ��װ�� 
	EHolySoul_NotEquipment               =  6,           //����Ԫ��װ��
	EHolySoul_InitFailed                 =  7,           //Ԫ���ʼ��ʧ��
	EHolySoul_NoExist                    =  8,           //��Ԫ�񲻴���
	EHolySoul_PourExpFailed				 =  9,			 //��עʧ��
	EHolySoul_NotHolySoulEquip			 =  10,			 //��Ԫ��װ�����ܴ�
	EHolySoul_EquipPosNotExist			 =  11,			 //װ��λ�ò�ƥ�䲻�ܴ�
	EHolySoul_EquipNotExist				 =  12,			 //Ԫ��װ��������
	EHolySoul_BagNoEnoughSpace			 =  13,			 //����û���㹻�ռ�
	EHolySoul_ConsidatePosNotExist       =  14,          //Ԫ��װ���ӳ�Ԫ�����Ե����Ͳ���ȷ
	EHolySoul_AwakePourExpFaied			 =  15,			 //Ԫ�����״̬��ע����ʧ��
	EHolySoul_LevelUpMaxLvl				 =  16,			 //������ߵȼ�
	EHolySoul_RebormMaxLvl				 =  17,			 //ת���ﵽ���ȼ�
};

//Ԫ���������
enum ESoulAttOptType
{
	ESAOType_Call		=  1,  //�ٻ�
	ESAOType_CancelCall =  2,  //ȡ���ٻ�
};

//----------------------------------------------------------------------------
// ����Ԫ��
//----------------------------------------------------------------------------
CMD_START(NS_ActivateHolySoul)	
CMD_END

//----------------------------------------------------------------------------
// Ԫ�����Է����仯
//----------------------------------------------------------------------------
CMD_START(NS_HolySoulAttChange)	
EHolyEquipAttType eHolyEquipAttType;
DWORD		      dwVal;
CMD_END

//----------------------------------------------------------------------------
// Ԫ��Զ�����Է����仯
//----------------------------------------------------------------------------
CMD_START(NS_RemoteHolySoulAttChange)
ESoulAttOptType		eOptType; 
DWORD				dwSoulID;		//Ԫ��ID
DWORD				dwRoleID;		//��ɫID
CMD_END

//----------------------------------------------------------------------------
// ����Ԫ��װ��
//----------------------------------------------------------------------------
CMD_START(NC_SoulEquip)	// װ��
EHolyEquipPos		ePosDst;		// װ����λ
INT64					n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NS_SoulEquip) // ����װ�����
DWORD	dwErrorCode;
INT64		n64Serial;		// װ��64λ���к�
CMD_END

//----------------------------------------------------------------------------
// ��Ԫ��װ��
//----------------------------------------------------------------------------
CMD_START(NC_UnSoulEquip)
INT16		n16PosDst;	// �����뱳����λ��
INT64		n64Serial;		// װ��64λ���к�
CMD_END

CMD_START(NS_UnSoulEquip)
DWORD	dwErrorCode;
INT64		n64Serial;		// װ��64λ���к�
CMD_END

//----------------------------------------------------------------------------
// ��ʼ��Ԫ����Ϣ
//----------------------------------------------------------------------------
CMD_START(NS_GetSoulInfo)
tagHolySoulInfo	 HolySoulInfo;
CMD_END

//----------------------------------------------------------------------------
//  Ԫ���ע
//----------------------------------------------------------------------------
CMD_START(NC_GetHolyAffuse)
INT64 n64AffuseExp;		//��ע����
CMD_END

CMD_START(NS_GetHolyAffuse)
DWORD dwErrCode;
CMD_END

//----------------------------------------------------------------------------
//  Ԫ������
//----------------------------------------------------------------------------
CMD_START(NS_HolySoulLvUp)
CMD_END


//----------------------------------------------------------------------------
//  Ԫ��ת��
//----------------------------------------------------------------------------
CMD_START(NC_GetHolyReborn)
INT64 n64ItemID;			//ת������ ��Ԫ����
INT64 n64StuffID;			//ת������ �Ͼ���¶
INT	 nNumStuff;
CMD_END

CMD_START(NS_GetHolyReborn)
DWORD dwErrCode;
CMD_END

//----------------------------------------------------------------------------
//  Ԫ��װ������
//----------------------------------------------------------------------------
CMD_START(NC_StarEnhance)
INT64 n64EquipId;			
CMD_END

CMD_START(NS_StarEnhance)
DWORD dwErrCode;
CMD_END

//����Ԫ����
CMD_START(NS_UpdateHolySoulExp)
INT64	n64Exp;
CMD_END

//Ԫ��װ�����
CMD_START(NC_HolyEquipHole)
INT64				n64SrcEquipID;	//װ��64ID
INT64				n64ItemID;		//���Ƽ�64ID
CMD_END

CMD_START(NS_HolyEquipHole)
DWORD				dwErrorCode;	//������
CMD_END

//Ԫ��װ����Ƕ
CMD_START(NC_HolyEquipInlay)
INT64				n64SrcEquipID;	//װ��64ID
INT64				n64StoneID;		//��ʯ64ID
INT64				n64ItemID;		//�Ͼ���¶64ID
INT					nItemNum;		//�����Ͼ���¶�ĸ���
BYTE				byHoleID;		//ѡ��Ŀ�λ
CMD_END

CMD_START(NS_HolyEquipInlay)
DWORD				dwErrorCode;	//������
CMD_END

//Ԫ��װ��ժ��
CMD_START(NC_HolyEquipRemove)
INT64				n64SrcEquipID;	//װ��64ID
BYTE				byHoleID;		//ѡ��Ŀ�λ
CMD_END

CMD_START(NS_HolyEquipRemove)
DWORD				dwErrorCode;	//������
CMD_END

#pragma pack(pop)

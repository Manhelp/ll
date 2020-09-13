//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_define.h
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ����ԭ�Ͷ���
//-------------------------------------------------------------------------------------------------------
#pragma once

struct tagPetProto
{
	DWORD	dwTypeID;		// ����TypeID 
	INT		nType3;			// ��������
	INT		nRoleLvlLim;	// ����Я���ȼ�
	INT		nMountable;		// �ܷ����
	INT		nMountSpeed;	// ����ٶ�

	Vector3	vSize;			// ��ײ�ߴ�
	FLOAT	fScale;			// ��˳ߴ�����

	BOOL	bBind;			// �Ƿ��
	DWORD	dwLifeTime;		// ���� ����Ϊ��λ
	BOOL	bFlyable;		// �Ƿ��Ƿ�������

	INT		nAptitudeMin[EIQ_End];
	INT		nAptitudeMax[EIQ_End];

	DWORD	dwAddBuffID;	// �����ٻ�ʱ��ӵ�buffid
	DWORD	dwItemID;

};

extern INT MAX_PETSOUL_NUM;
extern INT MAX_FOLLOWPETSOUL_NUM;
extern INT MAX_RIDINGPETSOUL_NUM;
extern INT FOLLOWPETTYPE;

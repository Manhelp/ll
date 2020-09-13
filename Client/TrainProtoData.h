//--------------------------------------------------------------------------------------------
// Copyright (c) 2010 DACHENG Entertainment All rights reserved.
// filename: TrainProtoData.h
// author: Su.Zhang
// actor:
// data: 2010-3-10
// last:
// brief: ��֮�������ݽṹ����
//--------------------------------------------------------------------------------------------

#pragma once

#include "..\WorldDefine\train_define.h"

// ��֮������̬����
struct tagTrainProtoData
{
	DWORD					dwTrainID;							// ����ID
	tstring					Name;								// ��������

	mutable ETrainState		eType;								// ����״̬

	INT						nCapability;						// ����ʵ��ֵ
	DWORD					dwItem1ID;							// ��ƷID
	INT						nItem1Num;							// ��Ʒ����
	BYTE					byItem1Quality;						// ��ƷƷ��
	DWORD					dwItem2ID;							// ��ƷID
	INT						nItem2Num;							// ��Ʒ����
	BYTE					byItem2Quality;						// ��ƷƷ��

	INT						nLevelOneType;						// һ������
	INT						nLevelTwoType;						// ��������
	DWORD					dwMoney;							// ��Ǯ
	DWORD					dwExp;								// ����
	INT						nMaxNum;							// ����׷���е�������
	INT						bVisible;							// �Ƿ�ɼ�
	mutable INT				nCurrentNum;						// ��ǰ���������

	tstring					GoalDescription;					// Ŀ��˵��
	tstring					FinishDescription;					// ���˵��
	tstring					StartDescription;					// ����˵��

	tstring					DescLevelOne;						// һ��˵������
	tstring					DescLevelTwo;						// ����˵������

};


class TrainProtoData
{
public:
	typedef std::map<DWORD, tagTrainProtoData> TrainProtoMap;
public:

	BOOL LoadFromFile();
	const TrainProtoMap& GetData() const;
	const tagTrainProtoData *GetTrainProto(DWORD id) const;
private:
	TrainProtoMap m_mapTrainProtoData;	// �ID����Ӧ�����Ա�
};

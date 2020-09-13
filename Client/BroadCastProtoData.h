#pragma once
//--------------------------------------------------------------------------------------------
// Copyright (c) 2010 DACHENG Entertainment All rights reserved.
// filename: BroadCastProtoData.h
// author: Su.Zhang
// actor:
// data: 2010-3-31
// last:
// brief: �㲥���ݽṹ����
//--------------------------------------------------------------------------------------------

enum ENewBroadcastType 
{
	ENBT_BigNewBroadcast,   // ��������
	ENBT_SmallRunHorse, 	// С�������
	ENBT_BigRunHorse,		// ���������
	ENBT_ChatNew		    // ����������

};
enum ENewChannelType 
{
	ENCT_CityWar = 1,       // ��սƵ��
	ENCT_Guild,				// ����Ƶ��
	ENCT_Affiche,			// ����Ƶ��
	ENCT_Special,			// ����Ƶ��
	ENCT_Activity			// �Ƶ��
};

// �㲥��̬���ݽṹ
struct tagBroadCastProtoData
{
	DWORD					dwBoardcastID;
	ENewBroadcastType		eBoardcastType;
	ENewChannelType			eChannelType;
	tstring					Content;

};

struct tagChannelProtoData
{
	ENewChannelType			eChannelType;
	tstring					ChannelName;
	DWORD					dwColour;
	INT						npriority;
};


class BroadCastProtoData
{
public:
	typedef std::map<DWORD, tagBroadCastProtoData> BroadCastProtoMap;
	typedef std::map<ENewChannelType, tagChannelProtoData> ChannelProtoMap;
public:

	BOOL LoadFromFile();
	const BroadCastProtoMap& GetBroadCastProtoMap() const;
	const tagBroadCastProtoData *GetBroadCastProto(DWORD id) const;
	const tagChannelProtoData *GetChannelProto(ENewChannelType channel) const;
private:
	BroadCastProtoMap m_mapBroadCastProtoData;	
	ChannelProtoMap   m_mapChannelProtoMap;
};


#pragma once

#include "..\WorldDefine\city_define.h"
struct tagCityAtt2Client;



struct tagCityAttEvent : public tagGameEvent
{
	tagCityAtt2Client *pCityAtt;
	tagCityAttEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) 
		: tagGameEvent(szEventName, pSenderFrame), pCityAtt(NULL) {}
};

struct tagCityWarApplyInfoRefreshEvent : public tagGameEvent
{
	bool bAllNameCanFind;				// ֪ͨ�¼����������нṹ����İ��������Ƿ����ҵ�
	int nGetGuildNameFlag;				// ��ȡ���а������Ƶı��
	DWORD dwCityID;						// ����ID
	DWORD dwOwnGuild;					// ������������ID
	DWORD dwAvgAckMoney;				// ��ս����ƽ������
	tagWarTime warTime;					//��ս��ʼʱ��
	std::vector<DWORD> vecAttackIDs;	// ����������ID����
	std::vector<DWORD> vecDefenseIDs;	// ���ط�����ID����
	std::vector<DWORD> vecConfirmIDs;	// �Ѿ�ȷ��
	tagCityWarApplyInfoRefreshEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame) { }
};

struct tagCityWarAssistConfirmRefreshEvent : public tagGameEvent
{
	std::vector<DWORD> vecDefenseIDs;	// ���ط�����ID
	tagCityWarAssistConfirmRefreshEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagCityWarCityRefreshEvent : public tagGameEvent
{
	std::vector<DWORD> vecCitys;
	tagCityWarCityRefreshEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagOpenCityEvent : public tagGameEvent
{
	DWORD dwCityID;
	tagOpenCityEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagOpenCityWarResult: public tagGameEvent
{
	tagOpenCityWarResult(GameFrame *pSenderFrame) : tagGameEvent(_T("tagOpenCityWarResult"), pSenderFrame){}
};

struct tagLeaveCityWarEvent: public tagGameEvent
{
	tagLeaveCityWarEvent(GameFrame *pSenderFrame) : tagGameEvent(_T("tagLeaveCityWarEvent"), pSenderFrame){}
};
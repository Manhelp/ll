// Xtrap����ҷ�װ
#pragma once
#include "Xtrap_S_Interface.h"

#define XTRAP_NOTIFY_CLIENT_SECOND 20
#define CSFILE_NUM 2
#define  XTRAP_TIMEOUT 600

class XTrapGuarder
{
public:
	XTrapGuarder();
	~XTrapGuarder();

	// ��������ʼ��
	VOID OnServerStart();

	// PlayerSession��ʼ��
	VOID OnPlayerSessionInit(char* pSessionBuff);

	// ÿ20�����XTrap_CS_Step1������������buff��ͻ��˷���
	VOID UpdateCSStep1(void* pSession, tagDWORDTime& dwPreXtrapUpdateTime, char* pSessionBuff);

	// ����ͻ��˷��ص�buffer
	VOID OnRecvStep2DataFromClient(void* pSession, void* pMsg, char* pSessionBuff);

	VOID SetEnable(BOOL bEnable) { m_bEnable = bEnable; }
	BOOL IsEnable() { return m_bEnable; }

private:
	VOID ReadMapFile();

private:
	BOOL m_bEnable;        // �����Ƿ���
};

extern XTrapGuarder g_XTrapGuarder;
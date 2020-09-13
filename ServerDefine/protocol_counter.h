//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: protocol_counter.h
// author: Jason
// actor:
// data: 2010-12-31
// last:
// brief: ����Э�������
//-------------------------------------------------------------------------------

#ifndef PROTOCOL_COUNTER_H
#define PROTOCOL_COUNTER_H
#include <map>
#include <string>
namespace vEngine{

class ProtocolCounter
{
	std::map<DWORD ,std::string> _map_proid_name; // Э��id��Э������Ӧ
	std::map<DWORD,DWORD> _map_proid_counter;// Э��id��Э�鷢�ͻ��߽��յ�����
	std::map<DWORD,DWORD> _map_proid_zero;
	std::map<DWORD,DWORD> _map_proid_size;
	char _buf[100];
public:
	// ���ļ���ȡЭ��id�����ƶ�Ӧ��ϵ
	BOOL LoadIDNameFromFile(LPCSTR strFileName);
	// ��������ĳ��Э���1���÷���Ŀǰ���̰߳�ȫ��
	DWORD Count(DWORD dwCmdID, DWORD dwSize);
	// ������ļ�
	BOOL DumpCountersToFile(LPCSTR strFile);
	// ���
	VOID Clear();

	VOID SetLocale(LPCTSTR strLanguage = NULL);
};
}
#endif // PROTOCOL_COUNTER_H


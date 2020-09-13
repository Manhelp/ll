#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------
enum
{
	E_Pulse_Success = 0,						// �ɹ�
	E_Pulse_NoItem,							// ���߲���	
	E_Pulse_NoMoney,						// ��Ǯ����
	E_Pulse_LessLevel,						// �ȼ�����	
	E_Pulse_NoPerSkill,						// ǰ�ü�����δѧ��
	E_Pulse_LessGodLevel,					// �����
	E_Pulse_MaxLevel,						// �ȼ��Ѿ��ﵽ��ߵȼ�
	E_Pulse_NoProto,							// �Ҳ�������Բ��
	E_Pulse_MaxTimePerDay,				// ��������Ѿ�����
	E_Pulse_Fail,									// ����ʧ��
};

//-----------------------------------------------------------------------------
//	����Ѫ��
//-----------------------------------------------------------------------------
CMD_START(NC_ActivePulse)	
	DWORD dwPulseID;				// Ҫ����ľ�����ID
CMD_END

CMD_START(NS_ActivePulse)	
	DWORD dwid;				// ����ľ�����ID
	INT		nLearnTime;
	DWORD dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
//	ʣ�༤��Ѫ������
//-----------------------------------------------------------------------------
CMD_START(NS_PulseCanLearnNum)	
	DWORD	dwNum;
CMD_END
#pragma once

#pragma pack(push,1)

//-------------------------------------------------------------------------
// login�����ļ���������ķ�������Ϣ
//-------------------------------------------------------------------------
CMD_START2(NC_LOGIN_CM)
	DWORD	dwSectionID;		// �������
CMD_END2

CMD_START2(NC_ANTIQUERYSTATE)
	CHAR	szAccountName[16];	// �ʺ���
CMD_END2

CMD_START2(NC_ANTIENTHRALL)
	DWORD	dwAccountNum;		// �ʺ���Ŀ
	CHAR	szAccountName[1];	// �ʺ���
CMD_END2

CMD_START2(NS_ANTIENTHRALL)
	CHAR	szAccountName[16];	// �ʺ���
	BYTE	byColor;			// ״̬
	DWORD	dwAccOLSeconds;		// �ۼ�����ʱ����
CMD_END2

#pragma pack(pop)
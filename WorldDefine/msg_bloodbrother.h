#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------
enum
{
	E_bloodbrother_Success = 0,						// �ɹ�
	E_bloodbrother_AlreadyHaveTeather,		// �Ѿ��д�����
	E_bloodbrother_NotHaveItem,					// ȱ�ٵ���
	E_bloodbrother_TeamNotHave2,				// ������ֻ����2����
	E_bloodbrother_NotFriend,						// 2���˲��Ǻ���
	E_bloodbrother_TeamLevelNotFeet,			// �����еȼ�����������
};

//-----------------------------------------------------------------------------
//	������
//-----------------------------------------------------------------------------
CMD_START(NC_TryMakeBrother)	
	DWORD dwDays;				// �����ݵ�����
CMD_END

CMD_START(NS_TryMakeBrother)				
	DWORD dwErrorCode;
	DWORD dwRoleID;	// ��ݵ�ID�����㲥����Чʹ��
CMD_END

//-----------------------------------------------------------------------------
//	���ʣ������
//-----------------------------------------------------------------------------
CMD_START(NS_BrotherLeftSecond)	
	DWORD	dwTeacherID;
	tagDWORDTime dwEndTime;
CMD_END

//-----------------------------------------------------------------------------
//	���ʣ������
//-----------------------------------------------------------------------------
CMD_START(NS_BrotherEnd)	
	BOOL bReason;			// ����ԭ�� 0 �ǳ�ʱ���ڣ�1�� ����80������
CMD_END
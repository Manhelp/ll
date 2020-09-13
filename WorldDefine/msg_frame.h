//-----------------------------------------------------------------------------
//!\file msg_frame.h
//!
//!\date 2010-01
//!
//!\brief
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

enum EFrameType
{
	//��ɫ���桢���ҽ��桢���ʽ��桢������桢���޽��桢���ѽ��桢���ɽ��桢�ƺŽ��桢�������桢�̶������
	EFRT_Role = 1,
	EFRT_Bag,
	EFRT_TalentPoint,
	EFRT_Quest,
	EFRT_Pet,
	EFRT_Friend,
	EFRT_Guild,
	EFRT_Title,
	EFRT_Fame,
	EFRT_Activity,
	EFRT_Area,
};

CMD_START(NS_OpenFramePage)
	EFrameType		eType;
CMD_END

#pragma pack(pop)
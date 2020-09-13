#pragma once

#include "msg_common_errorcode.h"
#include "creature_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ��Ӫ
//----------------------------------------------------------------------------
CMD_START(NS_SyncCamp)
	DWORD		dwRoleID;
	ECampType	eCamp;
CMD_END

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
CMD_START(NC_MouseWalk)			//�������
	Vector3	srcPos;
	Vector3	dstPos;
	bool	bSwim;				//��Ӿ״̬��ʶ
	FLOAT	fSpeed;
CMD_END   

CMD_START(NC_KeyboardWalk)		//��������
	Vector3	srcPos;
	Vector3	dstPos;
	bool	bSwim;				//��Ӿ״̬��ʶ
	FLOAT	fSpeed;
CMD_END

CMD_START(NC_StopWalk)			//ֹͣ����
	Vector3	curPos;
	bool	bSwim;				//��Ӿ״̬��ʶ
	FLOAT	fSpeed;
CMD_END

CMD_START(NS_StopWalk)			//�������յ�NC_StopWalk������Χ��ҹ㲥
	Vector3	curPos;
	bool	bSwim;				//��Ӿ״̬��ʶ
CMD_END

CMD_START(NS_SyncWalk)
	DWORD	dwRoleID;
	Vector3	srcPos;
	Vector3	dstPos;
	FLOAT	curTime;
	FLOAT	fXZSpeed;
	bool	bCollide;
	bool	bSwim;				//��Ӿ״̬��ʶ
CMD_END

//----------------------------------------------------------------------------
// ��Ծ
//----------------------------------------------------------------------------
CMD_START(NC_Jump)
	Vector3	srcPos;
	Vector3	dir;
	FLOAT   totalTime;
	float		fYSpeed;	//	Y�ٶȣ���������
CMD_END

CMD_START(NS_SyncJump)
	DWORD	dwRoleID;
	Vector3	srcPos;
	Vector3	dir;
	FLOAT	curTime;
	FLOAT	fXZSpeed;
	FLOAT	fYSpeed;
CMD_END

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
CMD_START(NC_Drop)
	Vector3 srcPos;
	Vector3 dir;
	FLOAT   totalTime;
CMD_END

CMD_START(NS_SyncDrop)
	DWORD 	dwRoleID;
	Vector3	srcPos;
	Vector3	dir;
	FLOAT	curTime;
	FLOAT	fXZSpeed;
CMD_END

//----------------------------------------------------------------------------
// ��ֱ����
//----------------------------------------------------------------------------
CMD_START(NC_VDrop)
	Vector3	srcPos;
	FLOAT   totalTime;
CMD_END

CMD_START(NS_SyncVDrop)
	DWORD	dwRoleID;
	Vector3	srcPos;
	FLOAT	curTime;
CMD_END

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
CMD_START(NC_Slide)
	Vector3	srcPos;
	Vector3 dstPos;				//�ͻ��˼���Ļ����յ�
CMD_END

CMD_START(NS_SyncSlide)
	DWORD	dwRoleID;
	Vector3	srcPos;
	FLOAT	curTime;
	FLOAT	fXZSpeed;
CMD_END

//----------------------------------------------------------------------------
// վ��ͬ����Զ�̽�ɫ������Ұʱ����
//----------------------------------------------------------------------------
CMD_START(NS_SyncStand)
	DWORD	dwRoleID;
	Vector3	curPos;
	Vector3	faceTo;
	bool	bSwim;				//��Ӿ״̬��ʶ
CMD_END

//----------------------------------------------------------------------------
// �ƶ�ʧ��
//----------------------------------------------------------------------------
CMD_START(NS_MoveFailed)
	Vector3 curPos;
	Vector3 faceTo;
	bool	bSwim;				//��Ӿ״̬��ʶ
CMD_END

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
CMD_START(NS_HitFly)
	DWORD	dwRoleID;
	Vector3 curPos;
	Vector3 destPos;
	Vector3 faceTo;	
	DWORD	dwSkillID;			//����ID
	DWORD   dwSrcRoleID;		//������ID
	DWORD	dwChannelIndex;		//�ܵ����
	bool	bCollide;			//ʹ����ײ
CMD_END

//----------------------------------------------------------------------------
// ������
//----------------------------------------------------------------------------
CMD_START(NS_NPCBeAttacked)
DWORD dwBatkAction;
CMD_END

//----------------------------------------------------------------------------
// �ƶ��ٶȸı�
//----------------------------------------------------------------------------
CMD_START(NS_MoveSpeedChange)
	DWORD	dwRoleID;
	Vector3 curPos;
	Vector3 faceTo;
CMD_END

//----------------------------------------------------------------------------
// ����ʹ��ʩ��
//----------------------------------------------------------------------------
CMD_START(NS_PetCastSkill)
	DWORD   dwPetID;
	DWORD   dwPetSkillID;
CMD_END

//----------------------------------------------------------------------------
// �ٻ�����
//----------------------------------------------------------------------------
CMD_START(NS_CallPet)
	DWORD	dwPetID;
CMD_END

//----------------------------------------------------------------------------
// ͬ��ȫ��ͼ�����λ��
//----------------------------------------------------------------------------
CMD_START(NS_SyncActiveCreaturesPos)
	INT32 nCreaturesNum;	//������
	tagACreaturePosInfo stData[1];
CMD_END

//-----------------------------------------------------------------------------
//Ŀ����ܷ񵽴�
//-----------------------------------------------------------------------------
//Ŀ����x zֵ
CMD_START(NC_CanGoToPos)
	INT fX;
	INT fZ;
CMD_END

//�жϽ��
CMD_START(NS_CanGoToPos)
	INT fX;				//Ŀ���Xֵ
	INT fZ;				//Ŀ���Yֵ
	DWORD mapID;
	DWORD dwErrorCode;
CMD_END;

//-----------------------------------------------------------------------------
//������
//-----------------------------------------------------------------------------

CMD_START(NC_EarthTemple)
	INT64   n64ItemID;	//��������ߵ�ID
	DWORD   dwMapID;	//Ŀ���ͼ��ID
	Vector3 destPos;	//Ŀ��ص����������
	BOOL	bDirectOpen;
CMD_END


CMD_START(NS_EarthTemple)
	DWORD dwErrorCode;
CMD_END;



#pragma pack(pop)

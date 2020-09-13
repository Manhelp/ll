#pragma once
#include "BaseData.h"

struct tagCameraData
{
	DWORD dwID;
	DWORD dwLoginCameraPos_x;		//��½���������
	DWORD dwLoginCameraPos_y;
	DWORD dwLoginCameraPos_z;
	DWORD dwLoginCameraLookAt_x;	//���������
	DWORD dwLoginCameraLookAt_y;
	DWORD dwLoginCameraLookAt_z;
	DWORD dwSelectRoleCameraPos_x;	//ѡ�����������
	DWORD dwSelectRoleCameraPos_y;
	DWORD dwSelectRoleCameraPos_z;
	DWORD dwSelectRoleCameraLookAt_x;//ѡ�����������
	DWORD dwSelectRoleCameraLookAt_y;
	DWORD dwSelectRoleCameraLookAt_z;
	DWORD dwRole1Pos_x;				//��ɫ1λ��
	DWORD dwRole1Pos_y;
	DWORD dwRole1Pos_z;
	DWORD dwRole2Pos_x;				//��ɫ2λ��
	DWORD dwRole2Pos_y;
	DWORD dwRole2Pos_z;
	DWORD dwRole3Pos_x;				//��ɫ3λ��
	DWORD dwRole3Pos_y;
	DWORD dwRole3Pos_z;
	BOOL bIsShake;					//������Ƿ�ζ�
	DWORD dwCreateRolePos_x;		//�����������λ��
	DWORD dwCreateRolePos_y;
	DWORD dwCreateRolePos_z;
	DWORD dwCreateCameraLookat_x;	//�����������������
	DWORD dwCreateCameraLookat_y;
	DWORD dwCreateCameraLookat_z;
	DWORD dwCreateCameraNearLookat_x;//����ͷ����
	DWORD dwCreateCameraNearLookat_y;
	DWORD dwCreateCameraNearLookat_z;
	DWORD dwCreateCamYaw;			//ƫ���
	DWORD dwCreateCamPitch;
	DWORD dwCreateCamRoll;
	DWORD dwCreateMAXCamDist;		//����

};

class CameraSetData : public BaseData
{
public:
	CameraSetData(void);
	~CameraSetData(void);

	static CameraSetData* Inst();

	/** �������������
	*/
	virtual void LoadFromFile();

	const tagCameraData* GetData()
	{
		return &m_pData;
	}

private:
	tagCameraData m_pData;

};

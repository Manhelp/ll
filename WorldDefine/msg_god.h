#pragma once
#include "RoleDefine.h"
#include "clergy_define.h"

#pragma pack(push, 1)

CMD_START(NC_GetAttribute)  //�������ֵ
	ERoleAttribute eType;
CMD_END

CMD_START(NS_GetAttribute)  //�������ֵ����
	DWORD dwValue;
CMD_END

CMD_START(NC_PromotionClergy) //����
	DWORD dwClergy;				  //��ְID
CMD_END

CMD_START(NS_PromotionClergy) //��������
	DWORD dwErrorCode;        //������
	DWORD dwClergy;               //��ְID
CMD_END

//��ְ�ı���Ϣ���㲥�ã�
CMD_START(NS_ChangeClergy)		
	DWORD dwRoleID;					//��ɫID
	DWORD dwClergy;					//��ְID
CMD_END

//////////////////////////////////////////////////////////////////////////
//	��ѡ���
//////////////////////////////////////////////////////////////////////////
CMD_START(NC_CampaignResult) 
	ECampaignGodType eType;	// ��ѯ������
CMD_END

CMD_START(NS_CampaignResult)
	ECampaignGodType eType;		// ��ѯ������
	INT nNum;
	tagGodCampaignInfo sResult;	// ��ѯ�Ľ��
CMD_END

/////////////////////////////////////////////////////////////////////////
//	������ѡ
//////////////////////////////////////////////////////////////////////////
CMD_START(NC_Campaign)
	ECampaignGodType eType;
CMD_END

CMD_START(NS_Campaign)
	DWORD dwErrorCode;
CMD_END

/////////////////////////////////////////////////////////////////////////
//	��ѡ��Ϣ
//////////////////////////////////////////////////////////////////////////
CMD_START(NC_CampaignInfo)
CMD_END

CMD_START(NS_CampaignInfo)
	ECampaignGodType eType;	// �Ѿ���������ְ�����û�б�����ECGT_Null
	DWORD dwNum[ECGT_Num];	// ÿһ����ְ������
CMD_END

//----------------------------------------------------------------------
//�������
/////////////////////////////////////////////////////////////////////////
//	������������
/////////////////////////////////////////////////////////////////////////
CMD_START(NC_ActivationArea)	
	DWORD		dwAreaID;				//����ID
	bool		bActive;				// �Ƿ񼤻�
CMD_END

CMD_START(NS_ActivationArea)
	bool			bActive;			// �Ƿ񼤻�
	DWORD			dwErrorCode;		//������
	DWORD			dwAreaID;			//����ID
	DWORD			dwCD;
CMD_END

/////////////////////////////////////////////////////////////////////////
//	��������
/////////////////////////////////////////////////////////////////////////
CMD_START(NC_AddArea)			
	DWORD	dwAreaID;				//����ID
	bool    bAdd;					//�Ƿ����ӣ�����Ϣ�����ã���������Ϊ�������ӣ�
CMD_END

CMD_START(NS_AddArea)			
	bool		   bAdd;			// �Ƿ������ӣ������false����ʧȥ
	DWORD dwErrorCode;				//������
	DWORD dwAreaID;					//����ID
		DWORD	dwTime;				//ʣ��ʱ�䣨�룩��ϡ������ר�ã�
CMD_END

//��ȡ��֮ϵ�������Ϣ
CMD_START(NC_GetGodRelation)
CMD_END

CMD_START(NS_GetGodRelation)
	ECampaignGodType eType;		// ��ѯ������
	INT nNum;
	tagGodCampaignInfo sResult;	// ��ѯ�Ľ��
CMD_END

//ϡ������������Ϣ
CMD_START(NS_RareAreaInfo)
	DWORD	dwAreaID;						// ����ID
	BOOL	bSuccess;						// �Ƿ�����ɹ�
	DWORD	dwRoleID;						// ��������ID
	DWORD	dwRobRoleID;					// ������ID
	TCHAR	szRoalName[X_SHORT_NAME];		// ����������
CMD_END


#pragma pack(pop)
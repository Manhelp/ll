#pragma once

#pragma pack(push, 1)

//������췢��Ƶ��
enum ESendChatChannel
{
	ESCC_NULL = -1,
	
	ESCC_World = 0,				//����
	ESCC_Pvt,				//˽��
	ESCC_Guild,				//����
	ESCC_Troop,				//����
	ESCC_Current,			//��ǰ
	ESCC_Scene ,            //����
	ESCC_Family,			//����
	ESCC_Combat,			//ս��
	ESCC_Leave,				//����
	ESCC_Affiche,			//����
	ESCC_GM,				//GM
	ESCC_System,			//ϵͳ
	ESCC_Group ,            //Ⱥ��
	ESCC_CastTalk ,		    //����
	ESCC_End,		
};

//����Ƶ��ʱ����
const INT	WORLD_CHANNEL_INTERVAL	= TICK_PER_SECOND * 30;

//����Ƶ��ʱ����
const INT	SCENE_CHANNEL_INTERVAL	= TICK_PER_SECOND * 10;


//����Ƶ����Ϣ��������
const INT	WORLD_CHANNEL_MSG_LEN	= 200;

//����Ƶ����Ϣ��ҵȼ�����
const INT	WORLD_CHANNEL_ROLE_LVL	= 10;

//����IM��ƷID 3070001 ������Ʒ��
const DWORD TYPEID_IM_CASTTALK		= 3070001 ;//����Ʒ  �� 1360001

//����IM��ƷID 3070002 ���ǰ���Ʒ��
const DWORD TYPEID_IM_CASTTALK_UNBIND		= 3070002 ;//�ǰ���Ʒ  ��1360001

//����Ƶ����ƷID 3070001 ������Ʒ��
const DWORD TYPEID_IM_WORLD         = 3070001;
 
//����Ƶ����ƷID 3070002 ���ǰ���Ʒ�� 
const DWORD TYPEID_IM_WORLD_UNBIND         = 3070002;

//����Ƶ����ƷID ������ר�ã����Ҫ�����õ���
const DWORD TYPEID_IM_WORLD_NETBAR		= 3070096;

#pragma pack(pop) 
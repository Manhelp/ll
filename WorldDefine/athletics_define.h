//-----------------------------------------------------------------------------
// filename: athletics_define.h
// author: Lamoair
// actor:
// data: 2010-2-25
// last:
// brief: ����ϵͳ����
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//����״̬
enum E_PvP_RoomState
{
	E_PvP_RoomStateNull = 0,
	E_PvP_Waiting,
	E_PvP_Fighting,
	E_PvP_FullPerson,   //�����Ѿ���   
	E_PvP_CountDown,   //����ʱ״̬
	E_PvP_Delete,
};

//��������
enum E_PvP_RoomType
{
	E_PvP_RoomTypeNull = 0,
    E_PvP_Room3v3 = 1,
	E_PvP_Room6v6 = 2,
};

//����ȼ�
enum E_PvP_RoomLevel
{
	E_PvP_RoomLevelNull = 0,
	E_PvP_Junior,	//����(���־���)     
	E_PvP_Middle,	//�м�(��ͨ���� )
	E_PvP_Senior,	//�߼�
};

enum E_PvP_RoleState
{
	E_PvP_RoleStateNull = 0,
	E_PvP_Preparing,       //׼��  
	E_PvP_Ready,           //����
	//E_PvP_Start,
};

enum E_Role_BothSide
{
	ERBS_NULL           =  0,
	ERBS_DefenseSide    =  1,  //���������
	ERBS_Attack         =  2,  //�����ż��
	ERBS_End,
};

enum E_Role_InPvP
{
	ERIP_NULL           = 0,
	ERIP_InPvP          = 1,
	ERIP_OffPvP         = 2,
};


//-----------------------------------------------------------------------------
// �����ɫ��Ϣ
//-----------------------------------------------------------------------------


struct tagPvPRoleInfo
{
	DWORD			dwRoleID;
	E_PvP_RoleState eState;
	BYTE			ucPos;
	E_Role_BothSide eRoleInSide;         
	E_Role_InPvP    eRoleInPvP;  
	TCHAR			szName[X_SHORT_NAME];//��ɫ����
	DWORD			dwRoleLevel;        //��ɫ�ȼ�
	DWORD			dwWuXun;            //��ѫ 
	EClassType	    ePlayerProfession;	//Զ�����ְҵ
};


//-----------------------------------------------------------------------------
// �����б�����ṹ
//-----------------------------------------------------------------------------
struct tagPvPRoom
{
	E_PvP_RoomType				eRoomType;
	E_PvP_RoomLevel				eLevel;
	DWORD                       dwSerialID;             //�������к�
    INT16	                    nRoomID ;	             //�����
	E_PvP_RoomState             eRoomState;			    //����״̬
    BYTE                        ucTotalPersonNum;		//����������
	INT			                iLevelUpperLimit;		//����ȼ�����
	INT							iLevelLowerLimit;		//����ȼ����� 
	DWORD						dwWuXunLimit;			//��ѫ����
	DWORD						dwManagerID;				//����
	TCHAR						szManagerName[X_SHORT_NAME];
	BYTE						ucCurrentPersonNum;		//��ǰ����
	tagPvPRoleInfo				pData[1];
};

//-----------------------------------------------------------------------------
// ���������Ϣ�ṹ
//-----------------------------------------------------------------------------
struct tagPvPSimRoom
{
	E_PvP_RoomType				eRoomType;
	E_PvP_RoomLevel				eLevel;
	DWORD                       dwSerialID;             //�������к�
	INT16	                    nRoomID ;	             //�����
	E_PvP_RoomState             eRoomState;			    //����״̬
	BYTE                        ucTotalPersonNum;		//����������
	INT			                iLevelUpperLimit;		//����ȼ�����
	INT							iLevelLowerLimit;		//����ȼ����� 
	DWORD						dwWuXunLimit;			//��ѫ����
	DWORD						dwManagerID;				//����
	TCHAR						szManagerName[X_SHORT_NAME];
	BYTE						ucCurrentPersonNum;		//��ǰ����
};

//-----------------------------------------------------------------------------
//ö�� ����ÿ��λ��              ���� ��ͻ��˲���ز��øĶ�������������
//-----------------------------------------------------------------------------
const INT    P3v3ROOM_TOTAL_PERSON_NUM       =  6;        //3v3�ķ�������
const INT    P6v6ROOM_TOTAL_PERSON_NUM       =  12;       //6v6�ķ�������
const INT    PVPROOM_ROLE_LOWERLEVEL         =  30;       //���뾺���Ľ�ɫ��ͼ��� 

enum E_PvP_Room_Pos
{
	ERP_RoomPos_1        =    0,
	ERP_RoomPos_2        =    1,
	ERP_RoomPos_3        =    2,
	ERP_RoomPos_4        =    3,
	ERP_RoomPos_5        =    4,
	ERP_RoomPos_6        =    5,
};

//-----------------------------------------------------------------------------
// ö�ٷ���ÿ��λ�õ�״̬
//-----------------------------------------------------------------------------
enum E_Room_Pos_State
{
	ERPS_Empty           =  0,      //û����ռ
	ERPS_Solid           =  1,      //�ѱ�ռ
};

//-----------------------------------------------------------------------------
// ö�ٷ���ID��״̬
//-----------------------------------------------------------------------------
enum E_RoomId_State
{
	ERIDS_NotCreated =    0, // û�б�������  
	ERIDS_Vacant     =    1, // ����״̬
	ERIDS_Created    =   2, // ������
};



//-----------------------------------------------------------------------------
// ����ÿ��λ�õ�״̬
//-----------------------------------------------------------------------------
struct tagPvPRoomPosState
{
	BYTE               ucPos;        //����λ��
	E_Room_Pos_State   eRoomState;   //����λ��״̬
};


//-----------------------------------------------------------------------------
// ����ID״̬
//-----------------------------------------------------------------------------
struct tagAthRoomId
{
	INT16                           nRoomID ;
	E_PvP_RoomLevel               eRoomLevel;
	E_RoomId_State                eRoomIDState;
};

#pragma pack(pop)

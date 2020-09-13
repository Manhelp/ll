#pragma once

#include "msg_common_errorcode.h"
#define MAX_MD5_ARRAY	36
#define MAX_MIBAO		6

#pragma pack(push, 1)

//--------------------------------------------------------------------------------------------
// Login Mode
//--------------------------------------------------------------------------------------------
enum
{
	LoginMode_InnerTest = 0,					//��		��¼ģʽ��0			�ڲ�ʹ��
	LoginMode_MainLand = 1,						//��		��¼ģʽ��1			��½
	LoginMode_Thunder = 2,						//��		��¼ģʽ��2			Ѹ��
	LoginMode_Taiwan_Orange = 3,				//��		��¼ģʽ��3			̨��_Loong		�����ӣ�
	LoginMode_SDOA = 4,							//��		��¼ģʽ��4			ʢ��
	LoginMode_Korea = 5,						//��		��¼ģʽ��5			����
	LoginMode_Vietnam = 6,						//��		��¼ģʽ��6			Խ��
	LoginMode_Taiwan_OMG = 7,					//��		��¼ģʽ��7			̨��_SM			��OMG��
	LoginMode_Singapore_OMG = 8,				//��		��¼ģʽ��8			�¼���				��OMG��	���ڵ�¼ģʽ0�Ļ����������˱�������Ĺ��ܣ�
	LoginMode_Douke = 9,						//!			��¼ģʽ��9			���͵�¼ģʽ
	LoginMode_JinshanKK = 10,					//!			��¼ģʽ��10		��ɽ���
	LoginMode_Korea_SM = 11,					//!			��½ģʽ��11		����_SM
};


//------------------------------------------------------------------------------
// ��Ӫ��ID
//------------------------------------------------------------------------------
enum EChannel_Type
{
	ECT_JinShanKuaiKuai =  10013,   //��ɽ���
	ECT_YYYuYinPingTai  =  10113 ,   //YY����ƽ̨
};


#define CROP_NAME_MAX_LEN 10

//�����ʽ�� �ڵ�1��С + �ڵ�1���� + �ڵ�2��С + �ڵ�2����
//--------------------------------------------------------------------------------------------
// Login �������
//--------------------------------------------------------------------------------------------

enum
{
	E_ProofResult_Account_No_Match				= 1,	//���ʺŷǷ�(�ʻ������ڻ������벻��,�ܱ�����)
	E_ProofResult_Account_In_Use				= 2,	//���ʺŵ�ǰ�Ѿ���ʹ��
	E_ProofResult_Account_Freezed				= 3,	//���ʺű�ͣ��
	E_ProofResult_Wrong_Build_Number			= 4,	//�汾�Ų���
	E_ProofResult_Disabled						= 5,	//���ʺ��Ѿ�����ͣ
	E_ProofResult_Queue							= 6,	//��ʼ�Ŷ�	
	E_ProofResult_Mibao_Error					= 7,	//�ܱ�����
	E_ProofResult_Wrong_Type					= 8,	//�Ƿ���֤����
	E_ProofResult_Proofing						= 9,	//������֤���Ѿ���֤���

	E_SelectWorld_Server_Full					= 11,	//�÷�������ǰӵ��
	E_SelectWorld_Server_Suspended				= 12,	//�÷�������ǰ����
	E_SelectWorld_Server_Maintenance			= 13,	//�÷�������ǰά��
	E_SelectWorld_Failed						= 14,	//�����쳣��ѡ��ʧ��
	E_SelectWorld_Short_Time					= 15,	//���ʱ��̫��,���Ժ�����
	E_SelectWorld_No_Select						= 16,	//���Ѿ�ѡ��ɹ�,������ѡ
	E_SelectWorld_GameWorldName_Wrong			= 17,	//�Ƿ�����Ϸ������


	E_ProofResult_Forbid_MemberCenter			= 18,	//���Ļ�Ա�����˺��ѱ���ͣ������ϵ�ͷ�
	E_ProofResult_Forbid_GMTool					= 19,	//������Ϸ�˺��ѱ���ͣ������ϵ�ͷ�	
	E_ProofResult_Forbid_CellPhone				= 20,	//�����˺��ѱ�����������ϵ�ͷ�
	E_ProofResult_Forbid_MiBao					= 21,	//�����˺��Ѿ���ʧ�ܱ���������ɽ����¼

	E_ProofResult_Forbid_Wrong_OTP				= 22,	//�tϵ�y�؂���Ո���P�]�[����뮋�棬�K�M��GASH��̖��������beanfun!�����[�򣬾Wַ��tw.beanfun.gamania.com��
	E_ProofResult_Forbid_Wrong_Event_OTP		= 23,	//������������beanfun!��̖��Ո��beanFun���먕

	E_ProofResult_Orange_VipBar_Limited			= 24,	//"���������ǽ�������ר�ã���ѡ������������"
	E_ProofResult_AuthServer_Unreachable		= 25,	//"�޷����ӵ���֤������"
	E_ProofResult_EmptyField					= 26,	//�ֶβ���Ϊ��
	E_PrrofResult_WrongFormat					= 27,	//��ʽ����
	// Jason 2010-4-26 v2.1.0
	E_PrrofResult_PKRunawayPunishmentState		= 28,	// PK���ݳͷ�״̬
	E_ProofResult_TimeOut                       = 29,

	E_ProofResult_GameNet_CanNotConnect_WEB     = 30,	//���ܺ���֤GameNet����ͨ��
	E_ProofResult_GameNet_Error_Token			= 31,	//�Ƿ���token
	E_ProofResult_GameNet_Block					= 32,	//�˻��Ѿ���GameNet����
	E_ProofResult_GameNet_Frozen				= 33,	//�˻��Ѿ���GameNet����
	E_ProofResult_GameNet_ERROR					= 34,	//GameNet��֤����
	E_ProofResult_GameNet_XML					= 35,	//����XML����
	E_ProofResult_GameNet_InsertAccount			= 35,	//����account����


	E_YYYuYinPingTai_GetTokenID                 = 100,  // YY����TokenID��ʶ��
	E_KoreaProof_InputOldPsd_Error				= 110,	// ��������������
	E_KoreaProof_InputNewPsd_Error				= 111,  // ��������������
	E_KoreaProof_TwicePsd_ChgError				= 112,	// �����޸�����
	E_KoreaProof_TwicePsd_SetError				= 113,	// ������������
	E_KoreaProof_TwicePsd_InputError			= 114,  // ������������

	E_WebLoginMode_GetURL						= 200,	// Web����URL��ַ
};




//---------------------------------------------------------------------------------------------
// �Ŷ�ʱ��10����
//---------------------------------------------------------------------------------------------
#define QUEUE_TIME	10*60

enum ELM_LoginMode
{
	ELM_NULL				= 0,
	ELM_Web_LoginMode		= 100314789,	//Web���뷽ʽ
};

#define CROP_NAME_MAX_LEN 10

struct tagWeb_URLInfo
{
	DWORD dwCropID;
	TCHAR szDescName[CROP_NAME_MAX_LEN];
	DWORD dwSize;
	CHAR  szURL[1];
};

//�����ʽ�� �ڵ�1��С + �ڵ�1���� + �ڵ�2��С + �ڵ�2����

//-----------------------------------------------------------------------------
// ��֤��Ϣ
//-----------------------------------------------------------------------------
CMD_START(NLC_SafeProof)
	DWORD	dwProofKey;						// ��ȫ��֤key
	CHAR	szUserName[X_SHORT_NAME];		// �û���
	DWORD	dwType;							// ��֤��ʽ
	DWORD	dwCurVersionID;					// ��ǰ�汾ID
	DWORD	dwWorldNameCrc;					// ��Ϸ��������crc
	CHAR	szPsd[MAX_MD5_ARRAY];			// ����
	DWORD	dwChannelID;					// ��Ӫ��ID
	ELM_LoginMode	dwLoginMode;
	bool    bIsGetTokenID;                  // �Ƿ�YY����TokenID
	bool	bIsGetURL;						// �Ƿ�Web����URL��ַ
	CHAR    szMac[MAX_MAC_LENGTH];          // mac����
	CHAR	szUserID[X_SHORT_NAME];			// UserID
	CHAR	szAppKey[X_APPKEY];				// AppKey
	CHAR	szToken[X_TOKEN];				// Token
	CHAR	szGUID[X_LONG_NAME];			// GUIDֵ  ������������
	DWORD	dwConnID;					// ��ID ��YY��
	DWORD	dwReserved;				// �����ֶΣ�YY��ʹ��(dwReserved ������Ϣ��С��byData����GUID)  
	BYTE    byData[1];
CMD_END;

CMD_START(NLS_ProofResult)
	DWORD	dwErrorCode;					// ������
	DWORD   dwLoginSessionID;				// ����ID
	BOOL	bGuard;							// ������
	DWORD	dwIndex;						// �Ⱥ����к�
	DWORD	dwVerifyCode;					// ��½���������͸��ͻ��˵���֤�룬����ZoneServerʱҪ�õ�
	DWORD	dwAccountID;					// �ʺ�ID
	DWORD   dwIP;							// ZoneServer IP ��ַ
	DWORD   dwPort;							// ZoneServer �˿ں�
	DWORD	dwConnID;						// ��ID ��YY��
	CHAR		szUserName[X_SHORT_NAME];		// �û���
	DWORD	dwReserved;						// �����ֶΣ�ʢ��ȦȦ��ʹ��(1.��ʾȱ����Ϣ��0��ʾ����)
    DWORD	dwURLNum;						// Web�ĵ�ַ�����Ӹ���
	BOOL	bNeedTelNum;					// �Ƿ���Ҫ�û���д�ֻ�����Ϣ
	BYTE    byData[1];                      // �����ֶΣ�YY��ʹ��(dwReserved ������Ϣ��С��byData����tokenID)  
CMD_END;

//------------------------------------------------------------------------------
// Web����
//------------------------------------------------------------------------------
CMD_START(NLS_WebLoginProof)
DWORD dwTokenID;
DWORD dwLoginID;
CHAR  szSign[MAX_MD5_ARRAY*2];
CHAR  szLoginURL[MAX_MD5_ARRAY*2];
DWORD dwURLSize;
CHAR  szURL[1];
CMD_END

//------------------------------------------------------------------------------
// �ܱ�
//------------------------------------------------------------------------------
CMD_START(NLS_MiBao)
	CHAR	szMiBao[MAX_MIBAO];				// �ܱ�������ʾ��
CMD_END

CMD_START(NLC_MiBao)
	DWORD	dwMBCrc;						// �ܱ�crc
CMD_END

//------------------------------------------------------------------------------
// (����)����������֤
//------------------------------------------------------------------------------
CMD_START(NLS_SecondPsdProof)
	bool	bIsFirstLogin;
CMD_END

//------------------------------------------------------------------------------
// (����)���ö�������
//------------------------------------------------------------------------------
CMD_START(NLS_SetSecondPsd)
	DWORD	dwEorrorCode;
CMD_END

CMD_START(NLC_SetSecondPsd)
	DWORD	dwOldSecondPsdCrc;
	CHAR	czNewSecondPsd[X_SHORT_NAME];	//��������
	bool	bIsChangeTwPsd;					//�Ƿ��޸Ķ�������
CMD_END

//------------------------------------------------------------------------------
// (����)�����������
//------------------------------------------------------------------------------
CMD_START(NLS_InputSecondPsd)
	DWORD	dwEorrorCode;
CMD_END

CMD_START(NLC_InputSecondPsd)
	DWORD	dwTwicePsdCrc;
CMD_END

//-------------------------------------------------------------------------------
// �Ŷ�
//-------------------------------------------------------------------------------
CMD_START(NLS_Queue)
	DWORD	dwPosition;						// �Ŷӵ�λ��
	DWORD	dwTime;							// �Ŷӵ�ʣ��ʱ��
CMD_END;

//------------------------------------------------------------------------------
// �Ŷ���Ϣ�㲥
//------------------------------------------------------------------------------
CMD_START(NLS_Broadcast)
TCHAR	szBMsg[X_LONG_NAME];				// �Ŷ���Ϣ�㲥
CMD_END

//-------------------------------------------------------------------------------
//	��¼Э����֤��			by: Jampe
//-------------------------------------------------------------------------------

#define PK_StrToID(n, s)		do{													\
									n = (DWORD)strlen(s);							\
									DWORD step = (n >> 5) + 1;						\
									for(DWORD i = n; i >= step; i -= step)			\
										n = n ^ ((n << 5) + (n >> 2) + s[i - 1]);	\
								}while(0)

#define ProofKey(k, p)			do{												\
									DWORD n[3];									\
									PK_StrToID(n[0], p->szUserName);			\
									PK_StrToID(n[1], p->szPsd);					\
									PK_StrToID(n[2], p->szGUID);				\
									k = p->dwWorldNameCrc + p->dwCurVersionID;	\
									k = (k << 2) + p->dwType;					\
									k = k + n[0] + n[1] + n[2];					\
								}while(0)


#pragma pack(pop)


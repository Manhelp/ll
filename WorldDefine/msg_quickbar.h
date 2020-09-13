//-----------------------------------------------------------------------------
//!\file msg_quickbar.h
//!
//!\date 20010-02-5
//!
//!\brief �ͻ�����������������������������Ϣ
//-----------------------------------------------------------------------------
// ��������ݱ���˵����
// ������ܹ���80�����������ݴ�����������ڷֳ�5�η��ͣ��ͻ�������������ͣ�ÿ�η���2�ţ�ÿ��8��
//  
#pragma once
#pragma pack(push, 1)

const INT QUICK_BAR_COUNT = 2 ;  //�����ÿ��2��
const INT QUICK_BAR_BUTTON_COUNT = 8 ;//ÿһ���а˸��ո�
const INT QUICK_BAR_SEND_COUNT = 5 ;  //��������ݷ��ʹ��� 

enum ENUM_QUICK_BAR
{
	QUICK_BAR_ONE = 0 ,
	QUICK_BAR_TWO = 1 ,
	QUICK_BAR_THREE = 2,
	QUICK_BAR_FOUR  = 3,
	QUICK_BAR_FIVE  = 4,
	QUICK_BAR_END,
};

//������Ƿ�����ݿ������
enum ENUM_NEEDTOLOAD
{
	ENUMQUICK_TOLOAD = 0 ,
	ENUMQUICK_NOTTOLOAD = 1,
	ENUMQUICK_END,
};

//�Ƿ񱣴�� 
enum ENUM_QUICKBAR_ISSAVED
{
	Enum_Saved = 0 ,    //���ݿ��Ѿ���� 
	Enum_NotSaved = 1,  //û�д��
	Enum_End,
};

struct tagQuickData
{
	INT16 	            m_eType;		//	��ť����
	DWORD				m_dwParam1;		//	��Ӧ����1(skill:id,	item:��Ŀ+λ��, petskill:petid)
	DWORD				m_dwParam2;		//	��Ӧ����2(skill:NULL,	item:typeid, petskill:skillid(���ȼ�))
	INT64				m_n64Param3;		//	(skill:NULL, item: 64id, petskill:null)

	tagQuickData()
	{
		m_eType = 0;		
		m_dwParam1 = 0;		
		m_dwParam2 = 0;		
		m_n64Param3 = 0;
	}

};

struct tagQuickBarState
{
   INT16 	            m_eType;		//	��ť����
   BOOL                 m_bIsChange;
   DWORD				m_dwParam1;		
   DWORD				m_dwParam2;		
   INT64				m_n64Param3;
};
//-----------------------------------------------------------------------------
// ���������(���͸�������) ��һ�汾
//-----------------------------------------------------------------------------
CMD_START(NC_MsgQuickBarData)
DWORD dwRoleID;
INT16 nLn;  //��������ڵ���
INT16 nCol; //��������ڵ���
tagQuickData btnData; //���������
CMD_END

//-----------------------------------------------------------------------------
// ֪ͨ��������������(���͸�������) ��һ�汾
//-----------------------------------------------------------------------------
CMD_START(NC_MsgInformSaveQuickBarData)

CMD_END

//-----------------------------------------------------------------------------
// ���������(���͸��ͻ���)
//-----------------------------------------------------------------------------
CMD_START(NS_MsgQuickBarData)
DWORD dwRoleID;
INT16 eQuickSendCount;    //��������ʹ���
tagQuickData btnData[QUICK_BAR_COUNT][QUICK_BAR_BUTTON_COUNT]; //���������
CMD_END

//-----------------------------------------------------------------------------
// �����ݿ��ȡ���ݸ��ͻ���
//-----------------------------------------------------------------------------
CMD_START(NC_MsgQuickBarDataLoadFromDB)
INT16 n16IsSave;    //�Ƿ�Ҫ��
DWORD dwRoleID;
CMD_END


CMD_START(NS_MsgIsSavedQuickBarData)
DWORD dwRoleID;
INT16 nIsSaved;    //�Ƿ��Ѿ����
CMD_END

CMD_START(NC_MsgIsSavedQuickBarData)
DWORD dwRoleID;
CMD_END
#pragma pack(pop) 
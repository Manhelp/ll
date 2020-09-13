/***************************************************************************
 * Copyright (C) 2011 by Soldier Of Fortune Online
 * Email : duccati@dragonflygame.com
 *
 * Name : IPGDragonfly.h
 * Description : IPG for DragonFly
 * Create : 	2011-07-25   	Ver 1.0
 * Modified: 	
 ****************************************************************************/
#ifndef IPGDragonfly_H
#define IPGDragonfly_H

#include "stdafx.h"
//#include <pthread.h>
//#include <string.h>
//#include "Map.h"
//#include "UserProtocol.h"
//#include "ServerProtocol.h"
//#include "UserManager.h"
//#include "InformationFile.h"
//#include "Packet.h"
class World;
////////////////////////////////////////////////////////////////////////////
//
//	PreDefined Variant Definition
//
////////////////////////////////////////////////////////////////////////////
#define PKT_PG_LOG_DATA				0x00001000	//PG pack.type
#define USERID_LEN					40
#define USERNO_LEN					40
#define PKT_PG_AUTH_DATA			100
#define PKT_PG_PAY_DATA				110
#define PKT_PG_AUTHPAY_DATA			120
#define PKT_PG_CANCEL_DATA			200

//socket ���� ���
#define MAX_WAIT_MILISEC			5000
#define MAX_WAIT_SECOND				5
//#define NO_ERROR					0

#define FALSE						0
#define TRUE						1

////////////////////////////////////////////////////////////////////////////
//
//	ErrorCode Definition
//
////////////////////////////////////////////////////////////////////////////
#define INVALID_IP_ADDRESS			999
#define RECEIVE_PACKET_ERROR		211
#define INVALID_PACKET_TYPE			212
#define INVALID_CONTENTS_ID			213
#define INVALID_ECPACKET_TYPE		214

#define PEERSOCKCLOSED				20000

#define LOGIN_ERROR_SOCKET_INIT		110
#define LOGIN_ERROR_SOCKET_CONN		111
#define LOGIN_ERROR_SEND_PACKET		120
#define LOGIN_ERROR_RECV_PACKET		121
#define LOGIN_ERROR_WRONG_PACKET	123
#define LOGIN_ERROR_UNKNOWN			199

#define H_INPUT						"100"
#define H_INPUT_CANCEL				"101"
#define H_OUTPUT					"200"
#define H_OUTPUT_CANCEL				"201"

#define IPG_INOUT_PORT				23100 
#define IPG_GETCASH_PORT			23200
#define IPG_BUFFER_SIZE				16384 

#define IPG_TIME_OUT				10000 // IPG ���� ���ð� 10��  

////////////////////////////////////////////////////////////////////////////
//
//	Packet Definition
//
////////////////////////////////////////////////////////////////////////////

struct PACKET_INOUT
{
	char	pack_type[4];			// �Ա�:100, �Ա���� :101, ���:200, ������:201 [in] 
	char	UserNo[USERNO_LEN];		//�̿��� ��ȣ					[in]
	char	UserId[USERID_LEN];		//�̿��� id					[in]
	char	CompanyCd[4+1];			//����Ʈ �ڵ�					[in]
	char	NickName[30];			//�̿��� ����					[in]
	char	UserIp[15+1];			//�̿��� ip					[in]
	char	OrgBxaid[20+1];			//��ҽ� ���� �ֹ���ȣ			[in]
	char	GoodsNo[6];				//��ǰ ��ȣ					[in]
	char	EventNo[6];				//�̺�Ʈ ��ȣ					[in]
	char	Amt[12];				//�̺�Ʈ, ��ǰ �ݾ�			[in]
	char	AgencyNo[20+1];			//������ �ֹ� ��ȣ				[in]
	char	Etc1[100];				//��������1					[in]
	char	Etc2[100];				//��������2					[in]
	char	Etc3[100];				//��������3					[in]
	char	Etc4[100];				//��������4					[in]
	char	UserName[50];			//�̸�						[in]
	char	SocialNo[13+1];			//�ֹι�ȣ - null���('-'����)	[in]
	char	Email[50];				//�̸��� - null���			[in]
	char	ZipCd[6+1];				//�����ȣ - null���('-'����)	[in]
	char	TelNo[20];				//��ȭ��ȣ(�޴���) - null���	[in]
	char	Sex[2];					//����						[in]
	char	BirthdayDt[9];			//���� - null���				[in]
	char	NationCd[3];			//ISO �����ڵ� - null���		[in] 
	char	SponsorCd[20];			//������ �ڵ� - null���		[in]
	
	char	UserGrade[2+1];			//�̿��� ���					[out]
	char	Bxaid[20+1];			//BillFlex IPG �ֹ���ȣ		[out]
	char	RetCode[4+1];			//BillFlex IPG �����	 		[out]
	
	PACKET_INOUT() { memset(this, 0x00, sizeof(PACKET_INOUT)); strcpy(pack_type, "200"); strcpy(CompanyCd, "DFLY"); }  
};

struct INOUT_RET
{
	char UserGrade[2+1];	
	int Bxaid;
	int RetCode;
	
	INOUT_RET() { memset( this, 0x00, sizeof(INOUT_RET) ); } 
};

struct PACKET_GETCASH
{
	char	UserNo[USERNO_LEN];		//�̿��� ��ȣ					[in]
	char	UserId[USERID_LEN];		//�̿��� id					[in]
	char	CompanyCd[4+1];			//����Ʈ �ڵ�					[in]
	char	NickName[30];			//�̿��� ����					[in]
	
	char	CashShop[12];			//Shopping-Mall Cash		[out]
	char	CashContents[12];		//Contents Cash				[out]
	char	CashBonus[12];			//Bonus Cash				[out]
	char	CashEtc[12];			//Mileage Point				[out]
	char	MileageShop[12];		//Shopping-Mall Mileage		[out]
	char	MileageContents[12];	//Contents Mileage			[out]
	char	RetCode[4+1];			//BillFlex IPG ��� code. 	[out]
	
	PACKET_GETCASH() { memset( this, 0x00, sizeof(PACKET_GETCASH) ); strcpy( CompanyCd, "DFLY" ); } 
};

class IPGDragonfly
{	
private:
	bool 		m_bInitialize;					// �ʱ�ȭ �۵�����
	bool	 	m_bRunning;
			
	char		m_IPGServerIP[20];			// IPG ���� IP 
	int			m_GetCashPort;					// GetCash Port 
	int			m_IOCashPort;					// IOCash Port 

	static IPGDragonfly* m_SelfInstance;

	IPGDragonfly();
	~IPGDragonfly();
		
public:
	static IPGDragonfly* GetInstance();
	static void ReleaseInstance();

	bool InitIPGDragonfly( char *pszIP, int nGetCashPort, int nIoCashPort);
	
	bool IsRunning() { return m_bRunning;}
	bool IsInitialize() { return m_bInitialize; }
	
	// Network Function 
	BOOL ConnectToServer( SOCKET* sock, char *IPAddr, int Port );
	BOOL Send( SOCKET sock, char* Buffer, int Length );
	BOOL Receive( SOCKET sock, char* Buffer, int Length );
	BOOL CloseMySocket( SOCKET sock );
	
	// GetCash 	
	bool GetDFCashReq( const char* usn, const char* accid, const char* nick, int& cash );	
	// OutCash	
	bool OutDFCashReq( INOUT_RET& ret, const char* usn, const char* accid, char* nick, char* ip, int shopid, int cash, int sex, char* gift_accid, int nNum );
	
};

#endif //~IPGDragonfly_H

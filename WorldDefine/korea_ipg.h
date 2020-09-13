//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2011-9-6
// last:
// brief: ����ipg��Ϣ�ṹ
//-----------------------------------------------------------------------------
#pragma once

//#define	CHECK_CASH_PORT		23100
//#define	CHECK_BALANCE_PORT	23200
#define USERNO_LEN 40
#define USERID_LEN 40

#define IPG_PACKET_TYPE_SAVECASH  100			//���(��100��)
#define IPG_PACKET_TYPE_TAKECASH  200			//���(��200��)
#define IPG_PACKET_TYPE_TAKECASHCANCEL  201		//���ȡ��(��201��)

#pragma pack(push, 1)
/*----------port����---------------------- 
����					Port
���(ȡ��)/���(ȡ��)	23100
����ѯ				23200
Cash����				23300
��ֵ					23400
-----------------------------------------*/


/*------------------���(ȡ��)/���(ȡ��)-----------------------------------
name			client		bytes	require	description
packet_type		char		4		Y		Packet ����(in) => ��100��
user_no			char		40		Y		�û����� (in)
user_id			char		40		Y		�û� ID (in)
company_cd		char		4		Y		��˾code (in) ��DFLY��
nick_name		char		30		Y		�û��ǳ� (in)
event_no		long		6		Y		���� (in)
event_amt		long		12		Y		���� (in)
agency_no		char		20		N		���˵궩����(in)
etc1			char		100		N		����1 (in)
user_name		char		50		Y		���� (in)
social_no		char		13		N		���֤�� (in)   ��-������
email			char		50		N		E-MAIL (in)
zip_cd			char		6		N		�������� (in)   ��-������
tel_no			char		20		N		�绰���� (in)
sex				char		1		Y		�Ա� (in)
birthday_dt		char		8		N		���� (in)
nation_cd		char		2		Y		ISO ����code (in)
sponsor_cd		char		20		Y		����code (in)
user_grade		char		2				�û��ȼ�(out)
bxaid			char		20				������ (out)
retcode			char		4				BillFlexIPG ���code (retval)

* packet_type : ���(��100��), ���ȡ�� (��101��), ���(��200��), ���ȡ��(��201��)
* sex : ��F��, ��M��
----------------------------------------------------------------------------*/
//typedef struct
//{
//	char	pack_type[4];			// '200'
//	char	UserNo[USERNO_LEN];		//�û�����						[in]
//	char	UserId[USERID_LEN];		//�û�id	        			[in]
//	char	CompanyCd[4+1];			//��վcode						[in]
//	char	NickName[30];			//�û��ǳ�						[in]
//	char	UserIp[15+1];			//�û� ip   					[in]
//	char	OrgBxaid[20+1];			//ȡ��ʱԭ��������				[in]
//	char	GoodsNo[6];				//��Ʒ��� 						[in]
//	char	EventNo[6];				//����						[in]
//	char	Amt[12];			     //�,��Ʒ���    			[in]
//	char	AgencyNo[20+1];			//���˵궩����					[in]
//	char	Etc1[100];				//���б���1						[in]
//	char	Etc2[100];				//���б���2						[in]
//	char	Etc3[100];				//���б���3						[in]
//	char	Etc4[100];				//���б���4						[in]
//	char	UserName[50];			//����							[in]
//	char	SocialNo[13+1];			//������ �C null����('-'����)	[in]
//	char	Email[50];				//E-MAIL �C null����			[in]
//	char	ZipCd[6+1];				//�������� �C null����('-'����)	[in]
//	char	TelNo[20];				//�绰����(�ֻ�) �C null����	[in]
//	char	Sex[2];					//�Ա�							[in]
//	char	BirthdayDt[9];			//���� �C null����				[in]
//	char	NationCd[3];			//ISO ����code �C null����		[in] 
//	char	SponsorCd[20];			//����code- null����			[in]
//	char	UserGrade[2+1];			//�û��ȼ�						[out]
//	char	Bxaid[20+1];			//BillFlex IPG ������			[out]
//	char	RetCode[4+1];			//BillFlex IPG ���ֵ			[out]
//} PACKET_INOUT, *pPACKET_INOUT;

//��ȡcash
/*----------------------------------------------------------------------------
name				client		bytes	require		Description
user_no				char		40		Y			�û�����(in)
user_id				char		40		Y			�û� ID (in)
company_cd			char		4		Y			��˾code (in) ��DFLY��
nick_name			char		30		Y			�û��ǳ� (in)
cash_shop			long		12					�̳�cash���(out)
cash_contents		long		12					Cash�����Ϣ(out)
cash_bonus			long		12					����cash���(out)
cash_etc			long		12					����cash���(out)
mileage_shop		long		12					�̳ǻ��� (����) ��� (out)
mileage_contents	long		12					������Ϣ (��Ϣ) ��� (out)
retcode				char		4					BillFlexIPG ���code(retval)
----------------------------------------------------------------------------*/

//struct PACKET_GETCASH
//{
//	char	UserNo[USERNO_LEN];		//�̿��� ��ȣ					[in]
//	char	UserId[USERID_LEN];		//�̿��� id					[in]
//	char	CompanyCd[4+1];			//����Ʈ �ڵ�					[in]
//	char	NickName[30];			//�̿��� ����					[in]
//	
//	char	CashShop[12];			//Shopping-Mall Cash		[out]
//	char	CashContents[12];		//Contents Cash				[out]
//	char	CashBonus[12];			//Bonus Cash				[out]
//	char	CashEtc[12];			//Mileage Point				[out]
//	char	MileageShop[12];		//Shopping-Mall Mileage		[out]
//	char	MileageContents[12];	//Contents Mileage			[out]
//	char	RetCode[4+1];			//BillFlex IPG ��� code. 	[out]
//	
//	PACKET_GETCASH() { memset( this, 0x00, sizeof(PACKET_GETCASH) ); strcpy( CompanyCd, "DFLY" ); } 
//};

typedef struct tagGetCash
{
	char	UserNo[USERNO_LEN];		//�û�����						[in]
	char	UserId[USERID_LEN];		//�û�id	        			[in]
	char	CompanyCd[4+1];			//��վcode						[in]
	char	NickName[30];			//�û��ǳ�						[in]
	char	CashShop[12];			//�̳�cash���					[out]
	char	CashContents[12];		//Cash�����Ϣ					[out]
	char	CashBonus[12];			//����cash���					[out]
	char	CashEtc[12];			//����cash���					[out]
	char	MileageShop[12];		//�̳ǻ��� (����) ���			[out]
	char	MileageContents[12];	//������Ϣ (��Ϣ) ���			[out]
	char	RetCode[4+1];			//BillFlex IPG ���ֵ			[out]

	tagGetCash() { memset( this, 0x00, sizeof(tagGetCash) ); strcpy( CompanyCd, "DFLY" ); } 

} GETCASH, *PGETCASH;



//���
/*----------------------------------------------------------------------------
name			client	bytes	require		description
packet_type		char	4		Y			Packet ���� (in) => ��200��
user_no			char	40		Y			�û����� (in)
user_id			char	40		Y			�û� ID (in)
company_cd		char	4		Y			��˾code (in) ��DFLY��
nick_name		char	30		Y			�û��ǳ� (in)
user_ip			char	15		Y			�û� IP (in)
goods_no		long	6		Y			��Ʒ���(in)
pay_amt			long	12		Y			��Ʒ��� (in)
agency_no		char	20		N			���˵궩����(in)
etc2			char	100		N			����2 (in)
etc3			char	100		N			����3 (in)
etc4			char	100		N			����4 (in)
bxaid			char	20					������(out)
retcode			char	4					BillFlexIPG ���code (retval)
----------------------------------------------------------------------------*/
typedef struct tagTakeCash
{
	char	pack_type[4];			// '200'
	char	UserNo[USERNO_LEN];		//�û�����						[in]
	char	UserId[USERID_LEN];		//�û�id	        			[in]
	char	CompanyCd[4+1];			//��վcode						[in]
	char	NickName[30];			//�û��ǳ�						[in]
	char	UserIp[15+1];			//�û� ip   					[in]
	char	GoodsNo[6];				//��Ʒ��� 						[in]
	char	Amt[12];			     //�,��Ʒ���    			[in]
	char	AgencyNo[20+1];			//���˵궩����					[in]
	char	Etc2[100];				//���б���2						[in]
	char	Etc3[100];				//���б���3						[in]
	char	Etc4[100];				//���б���4						[in]
	char	Bxaid[20+1];			//BillFlex IPG ������			[out]
	char	RetCode[4+1];			//BillFlex IPG ���ֵ			[out]
} TACKCASH, *PTACKCASH;

//���ȡ��
/*----------------------------------------------------------------------------
name			client	bytes	require	description
packet_type		char	4		Y		Packet ����(in) => ��201��
user_no			char	40		Y		�û�����(in)
user_id			char	40		Y		�û� ID (in)
company_cd		char	4		Y		��˾code (in) ��DFLY��
nick_name		char	30		Y		�û��ǳ� (in)
user_ip			char	15		Y		�û� IP (in)
org_bxaid		char	20		Y		���ʱ������(in)
goods_no		long	6		Y		��Ʒ���� (in)
agency_no		char	20		N		���˵궩����(in)
etc2			char	100		N		����2 (in)
etc3			char	100		N		����3 (in)
etc4			char	100		N		����4 (in)
bxaid			char	20				������ (out)
retcode			char	4				BillFlexIPG ���code (retval)
----------------------------------------------------------------------------*/
typedef struct tagTakeCashCancel
{
	char	pack_type[4];			// '200'
	char	UserNo[USERNO_LEN];		//�û�����						[in]
	char	UserId[USERID_LEN];		//�û�id	        			[in]
	char	CompanyCd[4+1];			//��վcode						[in]
	char	NickName[30];			//�û��ǳ�						[in]
	char	UserIp[15+1];			//�û� ip   					[in]
	char    OrgBxaId[20+1];			//���ʱ������					[in]
	char	GoodsNo[6];				//��Ʒ��� 						[in]
	char	AgencyNo[20+1];			//���˵궩����					[in]
	char	Etc2[100];				//���б���2						[in]
	char	Etc3[100];				//���б���3						[in]
	char	Etc4[100];				//���б���4						[in]
	char	Bxaid[20+1];			//BillFlex IPG ������			[out]
	char	RetCode[4+1];			//BillFlex IPG ���ֵ			[out]
} TACKCASHCANCEL, *PTACKCASHCANCEL;

//����
/*----------------------------------------------------------------------------
name				client		bytes		require		description
packet_type			char		4			Y			Packet ����(in) => ��100��
user_no				char		40			Y			�û����� (in)
user_id				char		40			Y			�û� ID (in)
company_cd			char		4			Y			��˾code (in) ��DFLY��
nick_name			char		30			Y			�û��ǳ� (in)
event_no			long		6			Y			���� (in)
event_amt			long		12			Y			���� (in)
agency_no			char		20			N			���˵궩����(in)
etc1				char		100			N			����1 (in)
user_name			char		50			Y			���� (in)
social_no			char		13			N			���֤�� (in)   ��-������
email				char		50			N			E-MAIL (in)
zip_cd				char		6			N			�������� (in)   ��-������
tel_no				char		20			N			�绰���� (in)
sex					char		1			Y			�Ա� (in)
birthday_dt			char		8			N			���� (in)
nation_cd			char		2			Y			ISO ����code (in)
sponsor_cd			char		20			Y			����code (in)
user_grade			char		2						�û��ȼ�(out)
bxaid				char		20						������ (out)
retcode				char		4						BillFlexIPG ���code (retval)
----------------------------------------------------------------------------*/
typedef struct tagSaveCash
{
	char	pack_type[4];			// '200'
	char	UserNo[USERNO_LEN];		//�û�����						[in]
	char	UserId[USERID_LEN];		//�û�id						[in]
	char	CompanyCd[4+1];			//��վcode						[in]
	char	NickName[30];			//�û��ǳ�						[in]
	char	EventNo[6];				//����						[in]
	char	Amt[12];			    //�,��Ʒ���					[in]
	char	AgencyNo[20+1];			//���˵궩����					[in]
	char	Etc1[100];				//���б���1						[in]
	char	UserName[50];			//����							[in]
	char	SocialNo[13+1];			//������ �C null����('-'����)	[in]
	char	Email[50];				//E-MAIL �C null����			[in]
	char	ZipCd[6+1];				//�������� �C null����('-'����)	[in]
	char	TelNo[20];				//�绰����(�ֻ�) �C null����	[in]
	char	Sex[2];					//�Ա�							[in]
	char	BirthdayDt[9];			//���� �C null����				[in]
	char	NationCd[3];			//ISO ����code �C null����		[in] 
	char	SponsorCd[20];			//����code- null����			[in]
	char	UserGrade[2+1];			//�û��ȼ�						[out]
	char	Bxaid[20+1];			//BillFlex IPG ������			[out]
	char	RetCode[4+1];			//BillFlex IPG ���ֵ			[out]
} SAVECASH, *PSAVECASH;

#pragma pack(pop)

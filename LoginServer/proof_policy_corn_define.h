//-----------------------------------------------------------------------------
//!\file proof_policy_corn_define.h
//!\author Lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief ���������� ��֤���ԵĶ��� 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.

#pragma once
#pragma pack(push, 1)

struct tagDecryptData{

	string	NationCode;		    //���Ҵ���
	string	PortalSiteCode;			
	string	UserID;			   
	string	SecurityCode;				
	string	UserAge;			
    string	GameStartTime;
	tagDecryptData()
	{
			
	}

};

#define  MAXSTR_SIZE   256       //�����ַ�������󳤶�
#define  SECKEY_STRCOUNT 6       //�����ַ����ܺ�õ�6���ַ�
#define  SECKEY_FLAG   22        //�����ַ���־

#pragma pack(pop)
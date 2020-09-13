//-----------------------------------------------------------------------------
//!\file proof_policy_russia.cpp
//!\author liutingting
//!
//!\date 2011-5-10
//! last 2011-5-10
//!
//!\brief ����˹��֤����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/msg_rt_errorcode.h"
#include "proof_policy_own_define.h"
#include "proof_policy.h"
#include "proof_policy_russia.h"
#include "login_server.h"
#include "db.h"
#include <fstream>
//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyRussiaDB::Init()
{
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	if( !P_VALID(pVar) ) return FALSE;

	BOOL bRet = m_DB.Init(pVar->GetString(_T("ip database")), 
		pVar->GetString(_T("user database")),
		pVar->GetString(_T("psd database")), 
		pVar->GetString(_T("name database")),
		(INT)pVar->GetDword(_T("port database")) );

	return bRet;
}

//-------------------------------------------------------------------------
// ��ѯ
//-------------------------------------------------------------------------
BOOL ProofPolicyRussiaDB::QueryAccount(LPCSTR szAccountName, tagProofResult* pProofResult)
{
	if( !P_VALID(szAccountName) || !P_VALID(pProofResult) ) 
		return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("QueryAccount FAILED1.\r\n"));
		return FALSE;
	}

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	char szIP[X_IP_LEN] = "";
	char szDateTime[X_DATATIME_LEN + 1]= "";
	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,psd,mibao,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0)
	{
		m_DB.FreeQueryResult( pResult );
		ILOG->Write(_T("QueryAccount FAILED2.\r\n"));
		return FALSE;
	}
	// ���ý��
	pProofResult->dwAccountID		=	(*pResult)[0].GetDword();
	pProofResult->dwFrobidMask		=	(*pResult)[1].GetDword();
	pProofResult->byPrivilege		=	(*pResult)[2].GetByte();
	pProofResult->bGuard			=	(*pResult)[3].GetBool();
	pProofResult->nGuardAccuTime	=	0;
	pProofResult->eLoginStatus		=	(EPlayerLoginStatus)(*pResult)[4].GetInt();
	pProofResult->dwWorldNameCrc	=	(*pResult)[5].GetDword();
	memcpy(pProofResult->szAccountName,szAccountName,X_SHORT_NAME);

	// ��¼����
	//strPsd = (*pResult)[6].GetString();

	// ��¼�ܱ�
	CHAR szMibao[MIBAO_LEN] = {'\0'};
	BOOL bRet = (*pResult)[7].GetBlob(szMibao, MIBAO_LEN);
	if( '\0' == szMibao[0] )
	{
		pProofResult->bNeedMibao = FALSE;
	}
	else
	{
		pProofResult->bNeedMibao = TRUE;
	}

	//�ϴε�¼��ʱ���ip
	memcpy(szIP,(*pResult)[8].GetString(),(*pResult)[8].GetLen());
	memcpy(szDateTime,(*pResult)[9].GetString(),(*pResult)[9].GetLen());

	pProofResult->dwPreLoginIP = pTrans->StringIP2IP(szIP);
	DataTime2DwordTime(pProofResult->dwPreLoginTime,szDateTime,(*pResult)[9].GetLen());

	// �黹�����
	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-------------------------------------------------------------------------
// ������Ҽ�¼
//-------------------------------------------------------------------------
BOOL ProofPolicyRussiaDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) 
	{
		ILOG->Write(_T("InsertAccout FAILED1.\r\n"));
		return FALSE;
	}

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("InsertAccout FAILED2.\r\n"));
		return FALSE;
	}

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetInsert("account");
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("',");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	ILOG->Write(_T("InsertAccout Ret:%d.\r\n"),bRet);
	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}

//----------------------------------------------------------------------------
// ������Ҽ�¼
//----------------------------------------------------------------------------
BOOL ProofPolicyRussiaDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}
//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyRussia::Init(PROOFCALLBACK fn)
{
	m_pUtil		=	"Util";

	m_fnCallback = fn;

	// ��ȡ�����ļ�
	if( !InitConfig() )
	{
		return FALSE;
	}

	// ��ʼ�����ݿ�
	if( !m_ProofDB.Init() )
	{
		return FALSE;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("ProofPolicyRussiaThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyRussia::ThreadUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyRussiaThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}

//-------------------------------------------------------------------------
// ��ʼ������
//-------------------------------------------------------------------------
BOOL ProofPolicyRussia::InitConfig()
{
	// ���������ļ�
	//CreateObj("ProofPolicyRussia", "VarContainer");
	//TObjRef<VarContainer> pVar = "ProofPolicyRussia";

	//if( !P_VALID(pVar) ) return FALSE;

	//pVar->Load("VirtualFileSys", _T("server_config/login/center_proof.ini"));

	// ��ȡIP�Ͷ˿�
	//TCHAR szIP[X_IP_LEN];
	//_tcsncpy(szIP, pVar->GetString(_T("ip server")), MCalTCharArrayNum(szIP) - 1);
	//memcpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	//m_nPort = pVar->GetInt(_T("port server"));

	//TCHAR szUrl[256] = {0};
	//_tcsncpy(szUrl, pVar->GetString(_T("url server")), MCalTCharArrayNum(szUrl) - 1);
	//memcpy(m_szUrl, m_pUtil->UnicodeToUnicode8(szUrl), sizeof(m_szUrl) - 1);

	//m_toCentralEveryTime=0;
	//m_toCentralEveryTime = pVar->GetInt(_T("toCentralEveryTime option")); 

	
	// ɾ�������ļ�����
	//KillObj("ProofPolicyRussia");
	
	return TRUE;
}

//-------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------
VOID ProofPolicyRussia::Destroy()
{
	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyRussiaThreadUpdate"), INFINITE);

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}
}

//-------------------------------------------------------------------------
// �����߳�
//-------------------------------------------------------------------------
VOID ProofPolicyRussia::ThreadUpdate()
{
DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			//ILOG->Write(_T("------Thread Russia-----.\r\n"));  

			// �õ���ǰʱ��
			dwTime = timeGetTime();

			tagProofResult result;
			// ���б���ȡ����֤����			
			tagPlayerProofData* pData = m_listProofData.PopFront();

			while( P_VALID(pData) )
			{
				TCHAR tszUserID[32] = {0};
				TCHAR tszAppKey[48] = {0};
				TCHAR tszToken[48] = {0};
				MultiByteToWideChar(CP_ACP,NULL,pData->szUserID,-1,tszUserID,32);
				MultiByteToWideChar(CP_ACP,NULL,pData->szAppKey,-1,tszAppKey,48);
				MultiByteToWideChar(CP_ACP,NULL,pData->szToken,-1,tszToken,48);

				ILOG->Write(_T("ProofData UserID:%s,AppKey:%s,Token:%s.\r\n"),tszUserID, tszAppKey, tszToken);

				result.dwClientID = pData->dwClientID;
				lstrcpynA( result.szUserID, pData->szUserID, X_SHORT_NAME );

				INT nRet = E_Success;

				//ͨ�����͵��Է���web service��֤
				nRet = m_GameNetWeb.GetServiceToken( pData->szToken );
				if( E_Success == nRet )
				{
					ILOG->Write(_T("GetServiceAccount Success!\r\n"));
					nRet = m_GameNetWeb.CheckSubscribeStatus( pData->szToken );
					if( E_Success == nRet )
					{
						ILOG->Write(_T("CheckSubscribeStatus Success!\r\n"));
						//web��֤ͨ���ڲ����Ƿ��Ѿ��н�ɫ����

						int nLen = strlen(pData->szUserID);
						CHAR szUserID[X_SHORT_NAME] = {0};
						if( nLen > 18 )
						{
							lstrcpynA( szUserID, &pData->szUserID[1], X_SHORT_NAME );
						}
						else
						{
							lstrcpynA( szUserID, pData->szUserID, X_SHORT_NAME );
						}

						BOOL bRet = m_ProofDB.QueryAccount( szUserID, &result );
						if( bRet )
						{
							ILOG->Write(_T("QueryAccount Success!\r\n"));
							if( EPLS_OffLine != result.eLoginStatus )
							{
								nRet = E_ProofResult_Account_In_Use;
							}
							else if( result.dwFrobidMask != 0 )
							{
								do{
									if(result.dwFrobidMask & EPLM_MemberCenter)
									{
										nRet = E_ProofResult_Forbid_MemberCenter;	
										break;
									}
									if(result.dwFrobidMask & EPLM_GMTool)
									{
										//������������ûص�����ʱ��Ҫworld_forbid������һ��
										nRet = E_ProofResult_Forbid_GMTool;						
										break;
									}
									if(result.dwFrobidMask & EPLM_CellPhone)
									{
										nRet = E_ProofResult_Forbid_CellPhone;						
										break;
									}
									if(result.dwFrobidMask & EPLM_MiBao)
									{
										nRet = E_ProofResult_Forbid_MiBao;						
										break;
									}
									if(result.dwFrobidMask & EPLM_WaiGua)
									{
										nRet = E_ProofResult_Disabled;						
										break;
									}
								}while(0);
							}
						}
						else
						{//û������˺žͲ���һ��
							//��ôд�ܶ��ģ����Ƕ����������Ƿ��������⣬ֻ������������
							ILOG->Write(_T("QueryAccount Failed1!\r\n"));
							int nLen = strlen(pData->szUserID);
							CHAR szUserID[X_SHORT_NAME] = {0};
							if( nLen > 18 )
							{
								lstrcpynA( szUserID, &pData->szUserID[1], X_SHORT_NAME );
							}
							else
							{
								lstrcpynA( szUserID, pData->szUserID, X_SHORT_NAME );
							}

							BOOL bRetInsert = m_ProofDB.InsertAccout(szUserID, "", "", FALSE);
							if(bRetInsert)
							{
								TCHAR tszUserID[X_SHORT_NAME] = {0};
								MultiByteToWideChar(CP_ACP,NULL,pData->szUserID,-1,tszUserID,32);
								ILOG->Write(_T("InsertAccout Success!:userid id %s, len id:%d.\r\n"),tszUserID,nLen);

								//�ٴβ�ѯ
								BOOL bRet = m_ProofDB.QueryAccount( szUserID, &result );
								if( bRet )
								{
									ILOG->Write(_T("QueryAccount Success2!\r\n"));
									if( EPLS_OffLine != result.eLoginStatus )
									{
										nRet = E_ProofResult_Account_In_Use;
									}
									else if( result.dwFrobidMask != 0 )
									{
										do{
											if(result.dwFrobidMask & EPLM_MemberCenter)
											{
												nRet = E_ProofResult_Forbid_MemberCenter;	
												break;
											}
											if(result.dwFrobidMask & EPLM_GMTool)
											{
												//������������ûص�����ʱ��Ҫworld_forbid������һ��
												nRet = E_ProofResult_Forbid_GMTool;						
												break;
											}
											if(result.dwFrobidMask & EPLM_CellPhone)
											{
												nRet = E_ProofResult_Forbid_CellPhone;						
												break;
											}
											if(result.dwFrobidMask & EPLM_MiBao)
											{
												nRet = E_ProofResult_Forbid_MiBao;						
												break;
											}
											if(result.dwFrobidMask & EPLM_WaiGua)
											{
												nRet = E_ProofResult_Disabled;						
												break;
											}
										}while(0);
									}
								}
								else
								{
									ILOG->Write(_T("QueryAccount Failed2!\r\n"));
									nRet = E_ProofResult_GameNet_InsertAccount;
								}
							}
							else
							{
								ILOG->Write(_T("InsertAccout Failed!\r\n"));
								nRet = E_ProofResult_GameNet_InsertAccount;
							}
						}
					}
					else
					{//��֤Statusʧ��
					}
				}
				else
				{//��֤Tokenʧ��
				}

				ILOG->Write(_T("ProofPolicy result:%d!\r\n"),nRet);

				// ���ûص�����
				(*m_fnCallback)(nRet, &result);

				SAFE_DEL(pData);

				pData = m_listProofData.PopFront();		// ȡ����һ��
			}

			WaitForSingleObject(m_listProofData.GetEvent(), 500);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test13.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//����ǵ��ԣ����׳��쳣
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	_endthreadex(0);
}

//-------------------------------------------------------------------------
// ��֤�ӿ�
//-------------------------------------------------------------------------
VOID ProofPolicyRussia::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*�����Ӫ��ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) 
		return;

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData( dwClientID, szUserID, szAppKey, szToken );

	m_listProofData.PushBack(pData);
}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyRussia::GetProofServerStatus()
{
	return EWS_Well;
}
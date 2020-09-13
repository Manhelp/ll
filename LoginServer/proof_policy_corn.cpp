
#include "stdafx.h"
#include "proof_policy.h"
#include "proof_policy_corn.h"
#include "player_mgr.h"
#include "login_server.h"
#include "kr_util.h"
#include  "rijndael.h"
#include <fstream>
extern Rijndael  g_KrUtil;
//-----------------------------------------------------------------------------
// DB��ʼ��
//-----------------------------------------------------------------------------
BOOL ProofPocilyCornDB::Init()
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

BOOL   ProofPocilyCornDB::QueryAccount(const string& name,string& seckey)
{
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	pStream->SetSelect("zinaccount", "secrandkey");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(name.c_str()).FillString("'");

	QueryResult* pResult = m_DB.Query(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)|| pResult->GetRowCount() <= 0 )
	{
		m_DB.FreeQueryResult( pResult );
		return FALSE;
	}
	
	// ��������
	seckey   =  (*pResult)[0].GetString();
	
	// �黹�����
	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-----------------------------------------------------------------------------
// DB��ѯ���
//-----------------------------------------------------------------------------
BOOL ProofPocilyCornDB::QueryAccount(LPCSTR szAccountName,LPCSTR szPsd, tagProofResult* pProofResult)
{
	if( !P_VALID(szAccountName) || !P_VALID(pProofResult) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;


	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	char szIP[X_IP_LEN] = "";
	char szDateTime[X_DATATIME_LEN + 1]= "";
	// ��������
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,mibao,SecondPsd,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");
	//pStream->FillString(" AND psd='").FillString(szPsd, pCon).FillString("'"); 
	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0)
	{
		m_DB.FreeQueryResult( pResult );
		return FALSE;
	}
	// ���ý��
	pProofResult->dwAccountID		=	(*pResult)[0].GetDword();
	pProofResult->dwFrobidMask		=	(*pResult)[1].GetDword();
	pProofResult->byPrivilege		=	(*pResult)[2].GetByte();
	pProofResult->bGuard			=	0;
	pProofResult->nGuardAccuTime	=	0;
	pProofResult->eLoginStatus		=	(EPlayerLoginStatus)(*pResult)[4].GetInt();
	pProofResult->dwWorldNameCrc	=	(*pResult)[5].GetDword();

    pProofResult->bNeedMibao		=	FALSE;

	pProofResult->dwSeondPsdCrc     =	IUTIL->Crc32((*pResult)[7].GetString());
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
BOOL ProofPocilyCornDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// ��������
	Connection* pCon = m_DB.GetFreeConnection();
	char czSecondPsd[10]= {0};

	pStream->SetInsert("account");
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("',");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("SecondPsd='").FillString(czSecondPsd, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;
	

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	// ��ѯ
	BOOL bRet = m_DB.Execute(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}

//----------------------------------------------------------------------------
// ������Ҽ�¼
//----------------------------------------------------------------------------
BOOL ProofPocilyCornDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ProofPolicyCorn::Init(PROOFCALLBACK fn)
{
	m_pThread = "Thread";

	SetProofCallback(fn);		// ������֤�ص�����
    
	// ��ȡ�����ļ�
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	m_nTimeOut  = (INT)pVar->GetInt(_T("timeout"), _T("prooftime"));
	// ��ʼ�����ݿ�
	if( FALSE == m_ProofDB.Init() )
	{
		return FALSE;
	}

	// ��ʼ���߳�
	m_bTerminateUpdate = FALSE;

	if( FALSE == m_pThread->CreateThread( _T("thread_update_proof_corn"), 
		(THREADPROC)m_Trunk.sfp1(&ProofPolicyCorn::ThreadUpdate), NULL) )
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID ProofPolicyCorn::Destroy()
{
	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("thread_update_proof_corn"), INFINITE);

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}
}

//-------------------------------------------------------------------------
//�Ѽ�������ת�����ַ���
//-------------------------------------------------------------------------
BOOL   ProofPolicyCorn::DecryptDataToString(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc,const CHAR* pPsd)
{
	  CHAR* pStrData = new CHAR[MAXSTR_SIZE];
	  memset(pStrData,0,sizeof(CHAR)*MAXSTR_SIZE);
	  if( !P_VALID(pStrData) || !P_VALID(pSrc) || !P_VALID(pDecryptData)|| !P_VALID(pPsd) )
	  {
		  delete [] pStrData;
		  pStrData = NULL;
          return FALSE;
	  }
      INT nLength = atoi(pPsd);
	  if ( nLength <= 0 || nLength >= MAXSTR_SIZE)
	  {
		  return FALSE;
	  }
      CHAR cipherBuff[MAXSTR_SIZE];
	  memset(cipherBuff,0,sizeof(cipherBuff));
	  INT nStrSize = strlen(pSrc)/2;
	  if ( nStrSize > MAXSTR_SIZE)
	  {
		  return  FALSE ;
	  }
	  HexStr2CharStr(pSrc, (byte*)cipherBuff, nStrSize);
	  g_KrUtil.padDecrypt((PBYTE)cipherBuff,nLength,(PBYTE)pStrData);
	  *(pStrData+nLength+1) = SECKEY_FLAG;
      if( FALSE == ParseStringFromDecryptData(pDecryptData,pStrData))
	  {
		  delete [] pStrData;
		  pStrData = NULL;
		  return FALSE ;
	  }
	  else
	  {
		  delete [] pStrData;
		  pStrData = NULL;
		  return TRUE;
	  }
}

//-------------------------------------------------------------------------
//�Ӽ������ݽ������ַ���
//-------------------------------------------------------------------------
BOOL    ProofPolicyCorn::ParseStringFromDecryptData(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc)
{
	DWORD dwStrSize = strlen(pSrc);
	if ( dwStrSize > MAXSTR_SIZE)
	{
		return FALSE;
	}
	INT i = 0;
	INT nStrCount = 0;
	INT nFirstPos = 0,nLastPos = 0;
	CHAR szDescStr[MAXSTR_SIZE];
	memset(szDescStr,0,sizeof(szDescStr));
	string str[6];
	memset(str,0,sizeof(str));
	//�ҵ���Ч���ַ�����ͷ
	while( pSrc[i] == SECKEY_FLAG || pSrc[i] == ' ')
	{
		if(i >= MAXSTR_SIZE)
		{
			return FALSE;
		}
		i++;
	}
	nFirstPos = i;
	while ( i < MAXSTR_SIZE )//���ܴ������ַ���ĩβ�Ƿ�Ϊ/0 ������
	{
		if( pSrc[i] == SECKEY_FLAG)
		{
			if (pSrc[i+1] == SECKEY_FLAG)  //����������־�� ˵���ַ���������
			{
				return FALSE;
			}
			nLastPos = i;
			memcpy(szDescStr,(pSrc+nFirstPos),(nLastPos - nFirstPos));//ȥ����־�� 22
			str[nStrCount] = szDescStr ;
			nStrCount++;
			memset(szDescStr,0,sizeof(szDescStr));
			nFirstPos = i + 1 ;
			if( nStrCount >= SECKEY_STRCOUNT )
			{
				break;
			}

		}	
		i++;

	}
	if ( nStrCount < SECKEY_STRCOUNT)  //����������ַ���С��6��˵�� �ַ��������� �Ź�
	{
		return FALSE;
	}

	pDecryptData->NationCode      = str[0].c_str();
	pDecryptData->PortalSiteCode  = str[1].c_str();
	pDecryptData->UserID          = str[2].c_str();
	pDecryptData->SecurityCode    = str[3].c_str();
	pDecryptData->UserAge         = str[4].c_str();
	pDecryptData->GameStartTime   = str[5].c_str();

	return TRUE;


}

//------------------------------------------------------------------------------
// �����߳�
//------------------------------------------------------------------------------
VOID ProofPolicyCorn::ThreadUpdate()
{
	DWORD dwTime = 0;
	// ���÷�����
	INT nRet = E_Success;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			// �õ���ǰʱ��
			dwTime = timeGetTime();
			// ���б���ȡ����֤����
			tagProofResult result;
			// ��������
            tagDecryptData DecryptData;
            string strPsd = "";   
			tagPlayerProofData* pData = m_listProofData.PopFront();
           
			while( P_VALID(pData) )
			{
				result.dwClientID = pData->dwClientID;

				string seckey   = "";
                //--------------------------------------------------
				//�Ѵ��������ַ��������
				//--------------------------------------------------
				//BOOL bRet = DecryptDataToString(&DecryptData,pData->strGUID.c_str(),pData->strPsd.c_str());
				// 				if ( !bRet)
				// 				{
				// 					nRet =  E_ProofResult_Account_No_Match ;
				// 					(*m_fnCallback)(nRet, &result);
				// 					SAFE_DEL(pData);
				// 					pData = m_listProofData.PopFront();
				// 					continue;  
				// 				}
				
				// 				INT nTime[6];
				// 				memset(&nTime,0,sizeof(nTime));
				// 				__int64  dwTime = _atoi64(DecryptData.GameStartTime.c_str());
				// 				for (INT i=0; i < 6; i++)
				// 				{
				// 					nTime[i] = dwTime % 100 ;
				// 					dwTime /= 100;
				// 				}
				// 				tagDWORDTime SrcTime(nTime[0],nTime[1],nTime[2],nTime[3],nTime[4],nTime[5]);
				// 				tagDWORDTime CurretTime = GetCurrentDWORDTime();
				// 				DWORD dwDisTime = CalcTimeDiff(CurretTime,SrcTime);
				// 				if( dwDisTime > m_nTimeOut   )
				// 				{
				// 					nRet =  E_ProofResult_TimeOut ;
				// 					(*m_fnCallback)(nRet, &result);
				// 					SAFE_DEL(pData);
				// 					pData = m_listProofData.PopFront();
				// 					continue; 
				// 				}

					//--------------------------------------------------
					//��ѯ���ݿ���������֤
					//--------------------------------------------------
			   BOOL  bRet = true;//m_ProofDB.QueryAccount(DecryptData.UserID,seckey);


				if ( !bRet )
				{
					nRet =  E_ProofResult_Account_No_Match ;
					(*m_fnCallback)(nRet, &result);
					SAFE_DEL(pData);
                    pData = m_listProofData.PopFront();
                    continue;  
				}
				else
				{
				 	if ( seckey != DecryptData.SecurityCode)
				    {
					  nRet =  E_ProofResult_Account_No_Match ;
					  (*m_fnCallback)(nRet, &result);
					  SAFE_DEL(pData);
					  pData = m_listProofData.PopFront();
					  continue;

				    }
					else
				    {
					    nRet = E_Success;
				    }

				 }
				result.bGuard = FALSE;
				strcpy(result.szAccountName, "test17"/*DecryptData.UserID.c_str()*/);
				CHAR szMibao[MIBAO_LEN] = ""; //û���ܱ�
				//�������ݿ�
				bRet = m_ProofDB.InsertAccout(result.szAccountName,strPsd.c_str(), szMibao, result.bGuard);
				if( !bRet )
				{
					bRet = m_ProofDB.UpdateAccout(result.szAccountName,strPsd.c_str(), szMibao, result.bGuard);
					
				}


				bRet = m_ProofDB.QueryAccount(result.szAccountName,pData->strPsd.c_str(), &result);
				if ( GT_INVALID == result.dwSeondPsdCrc || 0 == result.dwSeondPsdCrc)
				{
						result.bIsFirstLogin = true;
				}
				else
				{
						result.bIsFirstLogin = false;
				}
				if( bRet )
				{
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
					nRet = E_ProofResult_Account_No_Match;
				}

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
		wFileStream.open("log\\LoginError_Test3.txt",ios::out|ios::app/*|ios::trunc*/);
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

//---------------------------------------------------------------------------------
// ��֤�ӿ�
//---------------------------------------------------------------------------------
VOID	ProofPolicyCorn::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*�����Ӫ��ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szGUID) ) return;

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName,szPsd,szGUID,szMac);

	m_listProofData.PushBack(pData);
}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyCorn::GetProofServerStatus()
{
	return EWS_Well;
}
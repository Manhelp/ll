#include "StdAfx.h"
#include "proof_policy_vietnam.h"
#include "login_server.h"
#include "loongworld.h"
#include "../ServerDefine/base_define.h"
#include <fstream>

//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnamDB::Init()
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
BOOL ProofPolicyVietnamDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
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

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,psd,mibao,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// �ͷ�����
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// �ͷ���
	m_DB.ReturnStream(pStream);

	// ��ѯ���Ϊ��
	if( !P_VALID(pResult)|| pResult->GetRowCount() <= 0 )
	{
		m_DB.FreeQueryResult( pResult );
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

	//�û���
	strncpy_s(pProofResult->szAccountName,szAccountName,X_SHORT_NAME);

	// ��¼����
	strPsd = (*pResult)[6].GetString();

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
BOOL ProofPolicyVietnamDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

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

	// �ͷ���
	m_DB.ReturnStream(pStream);

	return bRet;
}

//----------------------------------------------------------------------------
// ������Ҽ�¼
//----------------------------------------------------------------------------
BOOL ProofPolicyVietnamDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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


/*
/	Խ����֤����
/
*/
//-------------------------------------------------------------------------
// ��ʼ��
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnam::Init(PROOFCALLBACK fn)
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";

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

	// ��ʼ������

	//����
	CreateObj("StreamTransPPVietnamLogin", "StreamTransport");
	m_pTransLogin = "StreamTransPPVietnamLogin";
	if( !P_VALID(m_pTransLogin) ) return FALSE;


	m_pTransLogin->Init(TRUE,ESTM_Stream); //
	// ���������߳�
	if(!m_pThread->CreateThread(_T("PPVietnamConnectLoginServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadConnectLoginServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPVietnamConnectLoginServer")))
	{
		continue;
	}

	// ���������߳�
	if(!m_pThread->CreateThread(_T("PPVietnamLoginThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadLoginServerUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPVietnamLoginThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}


//-------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::Destroy()
{

	// �����߳�
	InterlockedExchange((LPLONG)&m_bTerminateLoginUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPVietnamLoginThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateLoginConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPVietnamConnectLoginServer"), INFINITE);

	// ��������
	m_pTransLogin->Destroy();
	KillObj("StreamTransPPVietnamLogin");

	

	// ɾ������list�е�����
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}

    pData = m_listProofSolvedData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofSolvedData.PopFront();
	}

	// ɾ�������ļ�����
	KillObj("ProofPolicyVietnam");
}
//-------------------------------------------------------------------------
// ���ӶԷ�login�������߳�
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::ThreadConnectLoginServer()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateLoginConnect )
	{
		if( !m_pTransLogin->IsConnected() )
		{
			if( !m_pTransLogin->IsTryingToConnect() )
			{
				m_pTransLogin->TryToConnect(m_szLoginIP, m_nLoginPort);
			}

			Sleep(100);
			continue;	// ���¼������
		}

		IMSG(_T("Contected to center login server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szLoginIP), m_nLoginPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
}

//-------------------------------------------------------------------------
// ����login�߳�
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::ThreadLoginServerUpdate()
{
	DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateLoginUpdate )
		{
			// �õ���ǰʱ��
			dwTime = timeGetTime();

			// ������֤�б�
			UpdateProofList();

			// ������Ϣ
			UpdateLoginSession();

			// ��Ϣ
			Sleep(50);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test14.txt",ios::out|ios::app/*|ios::trunc*/);
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
// ��ʼ������
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnam::InitConfig()
{
	// ���������ļ�
	CreateObj("ProofPolicyVietnam", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyVietnam";

	if( !P_VALID(pVar) ) return FALSE;
	pVar->Load("VirtualFileSys", _T("server_config/login/vietnam_proof.ini"));
	//��¼��
	TCHAR szIP[X_IP_LEN] = {'\0'};
	_tcsncpy(szIP, pVar->GetString(_T("ip login_server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szLoginIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szLoginIP) - 1);

	m_nLoginPort = pVar->GetInt(_T("port login_server"));

	return TRUE;
}

//-------------------------------------------------------------------------
// ������֤�б�
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::UpdateProofList()
{
	//
	if(!m_pTransLogin->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif


	// ���б���ȡ����֤����
	tagProofResult result;

	tagPlayerProofData* pData = m_listProofData.PopFront();
	int iNum = 0;

	//һ�δ���50��
	while( P_VALID(pData) && iNum<50)
	{

		++iNum;

		//////////////////////////////////////////////////////////////////////////
		//�������ݶ���ansi

		TObjRef<StreamTransport> pTrans;
		pTrans = "StreamTransport";

		//ƴ��֤����
		stringstream strLoginFrame;
        strLoginFrame<<pData->strAccoutName.c_str();
        strLoginFrame<<X_OG_RECEIVE_DIVIDE_SIGN; 
		strLoginFrame<<pData->strPsd.c_str();
		strLoginFrame<<"\r\n";

		//��������
		m_pTransLogin->Send((void *)strLoginFrame.str().c_str(),strLoginFrame.str().length());


		m_listProofSolvedData.PushBack(pData);
		pData = m_listProofData.PopFront();		// ȡ����һ��
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}

//-------------------------------------------------------------------------
// ����Login��Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::UpdateLoginSession()
{
	if( NULL == m_pTransLogin )
		return;

	// ������
	if(!m_pTransLogin->IsConnected() && !m_pThread->IsThreadActive(_T("PPVietnamConnectLoginServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, TRUE);
		m_pTransLogin->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("PPVietnamConnectLoginServer"), INFINITE);

		// ����������֤�����������߳�
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, FALSE);
		m_pThread->CreateThread(_T("PPVietnamConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadConnectLoginServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("PPVietnamConnectLoginServer")))
		{
			m_pThread->CreateThread(_T("PPVietnamConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadConnectLoginServer), NULL);
			continue;
		}

		return;
	}

	// ��������Խ�Ϸ���������Ϣ
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransLogin->Recv(dwSize);

	while( P_VALID(pRecv) )
	{	
		//����Խ����֤������Ϣ
		HandleVietnamLogin(pRecv);

		//Խ����֤�ķ�����Ϣ
		m_pTransLogin->FreeRecved(pRecv);

		pRecv = m_pTransLogin->Recv(dwSize);
	}

}

//-------------------------------------------------------------------------
// ����Խ����֤��Ϣ
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::HandleVietnamLogin(LPBYTE pRecv)
{
	


	tagPlayerProofData* pData = m_listProofSolvedData.PopFront();
	
	tagProofResult result;
	if ( P_VALID( pData ) )
	{
		result.dwClientID     = pData->dwClientID;
		result.dwWorldNameCrc = pData->dwWorldNameCrc;
		result.dwPreLoginIP   = pData->dwIP ;
		SAFE_DEL(pData);
	}
	string strAccountName;
	//��������
	string strPsd = "***";
	// �鿴������
	INT nRet = E_Success;	

	vector <string> vecVietnamData;
	//ֱ��ת
	string strVietnamRecv = (CHAR*)pRecv;
	//��������
	ParseVietnamProofData(strVietnamRecv,vecVietnamData,X_OG_RECEIVE_DIVIDE_SIGN);
	//���������������
	/*
if (vecVietnamData.size() < 4)
	{
		return;
	}
*/

	//////////////////////////////////////////////////////////////////////////
	if(0 != memcmp(vecVietnamData[1].c_str(),X_OG_PROOF_SIGN,vecVietnamData[1].size()))
	{
	     nRet = E_ProofResult_Account_No_Match;		
	}
	else 
	{
		do 
		{
			strAccountName = vecVietnamData[0];

			//������
			BOOL bGuard = FALSE;
			CHAR szMibao[MIBAO_LEN] = ""; //û���ܱ�
			//�������ݿ�
			BOOL bRet = m_ProofDB.InsertAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			if( !bRet )
			{
				bRet = m_ProofDB.UpdateAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			}

		} while (0);
	}
	if( E_ProofResult_Account_No_Match != nRet && E_ProofResult_Forbid_MemberCenter != nRet )
	{

		BOOL bRet = m_ProofDB.QueryAccount(strAccountName.c_str(), strPsd, &result);

		if( bRet )
		{
			if( EPLS_OffLine != result.eLoginStatus )
			{
				nRet = E_ProofResult_Account_In_Use;
			}
			else if( result.dwFrobidMask != 0 )
			{
				do{
					if(result.dwFrobidMask & EPLM_GMTool)
					{
						//������������ûص�����ʱ��Ҫworld_forbid������һ��
						nRet = E_ProofResult_Forbid_GMTool;						
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
	}

	// ���ûص�����
	(*m_fnCallback)(nRet, &result);
}
//-----------------------------------------------------------------------------------------------
// ȥ���س�����
//-----------------------------------------------------------------------------------------------
BOOL ProofPolicyVietnam::CheckString(string& str)
{
	// �Իس����н���
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// ����û����Ƿ�Ϊ��
	return str.empty();
}
//-----------------------------------------------------------------------------------------------
// �õ����������� �����ŷָ�
//-----------------------------------------------------------------------------------------------
VOID ProofPolicyVietnam::ParseVietnamProofData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(cDivideSign,iprepos);
		//���һ��
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//������һ���ַ���
			CheckString(strTemp);

			vecOrangeData.push_back(strTemp);
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);

		vecOrangeData.push_back(strTemp);
		iprepos = icurpos + 1;
	}
}

//-------------------------------------------------------------------------
// ��֤�ӿ�
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*�����Ӫ��ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	// ����һ���µ���֤��Ϣ
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName, szPsd, szGUILD,dwIP,dwWorldNameCrc,szMac);

	m_listProofData.PushBack(pData);

}

//-------------------------------------------------------------------------
// ��֤����������״̬
//-------------------------------------------------------------------------
INT ProofPolicyVietnam::GetProofServerStatus()
{
	if (P_VALID(m_pTransLogin) && m_pTransLogin->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}

#include "StdAfx.h"
#include "InformationFrame.h"
#include "CharacterFrame.h"
#include "NetSession.h"
#include "RoleMgr.h"
#include "HttpPicEx.h"
#include "FilterData.h"
#include "LocalPlayer.h"
#include "IconStatic.h"
#include "SaveAccount.h"

const int MAX_RENAME_HISTORY = 5;

InformationFrame::InformationFrame(void) : m_Trunk(this),
	m_pWnd(NULL), m_pWndFather(NULL), m_bisRefreshing(false)
{
}

InformationFrame::~InformationFrame(void)
{
}

BOOL InformationFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	//ע����������������
	m_pCmdMgr->Register("NS_GetRoleVCard", (NETMSGPROC)m_Trunk.sfp2(&InformationFrame::OnNS_GetRoleVCard), _T("NS_GetRoleVCard"));
	m_pCmdMgr->Register("NS_SetRoleVCard", (NETMSGPROC)m_Trunk.sfp2(&InformationFrame::OnNS_SetRoleVCard), _T("NS_SetRoleVCard"));
	m_pCmdMgr->Register("NS_LPRename",	   (NETMSGPROC)m_Trunk.sfp2(&InformationFrame::OnNS_LPRename),	   _T("NS_LPRename"));

	return bRet;
}

BOOL InformationFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	//ע��GUI�¼�������
	m_pGUI->UnRegisterEventHandler(m_pWndFather->GetFullName().c_str(), m_Trunk.sfp1(&InformationFrame::EventHandler));

	//ע����������������
	m_pCmdMgr->UnRegister("NS_GetRoleVCard", (NETMSGPROC)m_Trunk.sfp2(&InformationFrame::OnNS_GetRoleVCard));
	m_pCmdMgr->UnRegister("NS_SetRoleVCard", (NETMSGPROC)m_Trunk.sfp2(&InformationFrame::OnNS_SetRoleVCard));
	m_pCmdMgr->UnRegister("NS_LPRename",	 (NETMSGPROC)m_Trunk.sfp2(&InformationFrame::OnNS_LPRename));

	return bRet;
}

BOOL InformationFrame::ReloadUI()
{
	// ����
	m_pWndFather				=	m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd						=	m_pWndFather->GetChild(_T("informationpic"));

	m_pStcName					=	(GUIStatic*)m_pWnd->GetChild(_T("rname"));											//����
	m_pStcParty					=	(GUIStatic*)m_pWnd->GetChild(_T("rparty"));											//����
	m_pStcLevel					=	(GUIStatic*)m_pWnd->GetChild(_T("rgrad1"));											//�ȼ�
	m_pStcCareer				=	(GUIStatic*)m_pWnd->GetChild(_T("rcareer"));										//ְҵ
	m_pStcPosition1				=	(GUIStatic*)m_pWnd->GetChild(_T("rpost1"));											//����ְλ
	m_pStcPosition2				=	(GUIStatic*)m_pWnd->GetChild(_T("rpost2"));

	m_pComboBoxGender			=	(GUIComboBox*)m_pWnd->GetChild(_T("information\\gender"));							//�Ա�
	m_pComboBoxConstellation	=	(GUIComboBox*)m_pWnd->GetChild(_T("information\\constellation"));					//����
	m_pComboBoxChZodiac			=	(GUIComboBox*)m_pWnd->GetChild(_T("information\\zodiac"));							//��Ф
// 	m_pComboBoxArea				=	(GUIComboBox*)m_pWnd->GetChild(_T("information\\area"));							//����
// 	m_pComboBoxProvince			=	(GUIComboBox*)m_pWnd->GetChild(_T("information\\city"));							//ʡ
	m_pBtnArea					=	(GUIButton*)m_pWnd->GetChild(_T("information\\area\\butt1"));						//������ť
	m_pBtnProvince				=	(GUIButton*)m_pWnd->GetChild(_T("information\\city\\butt1"));						//ʡ��ť
	m_pLstArea					=	(GUIListBox*)m_pWnd->GetChild(_T("information\\area\\numlist1"));					//�����б�
	m_pLstProvince				=	(GUIListBox*)m_pWnd->GetChild(_T("information\\city\\numlist1"));					//ʡ�б�
	m_pStcArea					=   (GUIStatic*)m_pWnd->GetChild(_T("information\\area\\num1"));						//����
	m_pStcProvince				=	(GUIStatic*)m_pWnd->GetChild(_T("information\\city\\num1"));						//ʡ
	m_pScbArea					=	(GUIScrollBar*)m_pWnd->GetChild(_T("information\\area\\numlist1\\scr"));
	m_pScbProvince				=	(GUIScrollBar*)m_pWnd->GetChild(_T("information\\city\\numlist1\\scr"));
	if(SaveAccount::Inst()->GetLoginMode() != 11)
	{
		m_pScbArea->SetInvisible(TRUE);
		m_pScbProvince->SetInvisible(TRUE);
	}

	m_pEditCityName				=	(GUIEditBox*)m_pWnd->GetChild(_T("information\\cityname\\cityname_input"));			//��

	m_pEditBoxExPresentation	=	(GUIEditBoxEx*)m_pWnd->GetChild(_T("information\\presentationpic\\presentation"));	//����ǩ��
	m_pEditRenameHistory		=	(GUIEditBoxEx*)m_pWnd->GetChild(_T("information\\presentationpic2\\presentation"));	//��ʷ����

	m_pStcHeadPic				=	(IconStatic*)m_pWnd->GetChild(_T("information\\headpicback"));				//ͷ��
	m_pStcHeadPicC				=	(GUIStatic*)m_pWnd->GetChild(_T("information\\headpicback\\headpic"));				//ͷ��

	m_pBtRefresh				=	(GUIButton*)m_pWnd->GetChild(_T("information\\new"));								//ˢ��

	m_pPbtVisible				=	(GUIPushButton*)m_pWnd->GetChild(_T("visible\\optnutt1"));							//������ҿɼ�

	m_pBtSave					=	(GUIButton*)m_pWnd->GetChild(_T("save"));											//����

	//ע��GUI�¼�������
	m_pGUI->RegisterEventHandler(m_pWndFather->GetFullName().c_str(), m_Trunk.sfp1(&InformationFrame::EventHandler));

	m_pStcPosition2->FlipToTop();
	m_pStcHeadPicC->SetInvisible(FALSE);

	//��ʼ���ؼ�����
	InitInfoData();

	return TRUE;
}

VOID InformationFrame::Update()
{
    /*! �����ۻ�ʱ�䣬����ҵ��ˢ��ʱ�����ӳ� */
    static DWORD dwAccumTime = 0;
    if (m_bisRefreshing)
    {
        dwAccumTime += Kernel::Inst()->GetDeltaTimeDW();
        if (dwAccumTime >= REFRESH_DELAY)
        {
            dwAccumTime = 0;
            m_bisRefreshing = false;
            m_pBtRefresh->SetEnable(true);
            m_pBtRefresh->SetRefresh();
        }
    }
}

VOID InformationFrame::UpdateInformation()
{
	//������Զ�������������
	tagNC_GetRoleVCard msg;
	msg.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();

	m_pSession->Send(&msg);
}

DWORD InformationFrame::OnGameEvent( tagGameEvent* pGameEvent )
{
	return 0;
}

DWORD InformationFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
    case EGUIE_Active:
        if( pWnd == m_pComboBoxGender->GetEditBox() )
            m_pGUI->SetActive( m_pComboBoxGender->GetListBox() );
        else if( pWnd == m_pComboBoxConstellation->GetEditBox() )
            m_pGUI->SetActive( m_pComboBoxConstellation->GetListBox() );
        else if( pWnd == m_pComboBoxChZodiac->GetEditBox() )
            m_pGUI->SetActive( m_pComboBoxChZodiac->GetListBox() );
        else if( pWnd == m_pStcArea )
		{
             m_pGUI->SetActive( m_pLstArea );
			 m_pLstArea->SetInvisible(!m_pLstArea->IsInvisible());
		}
        else if( pWnd == m_pStcProvince )
		{
             m_pGUI->SetActive( m_pLstProvince );
			 m_pLstProvince->SetInvisible(!m_pLstProvince->IsInvisible());
		}
		else if( pWnd ==  m_pEditBoxExPresentation || pWnd == m_pEditRenameHistory || pWnd == m_pEditCityName)
		{
			m_pLstProvince->SetInvisible(TRUE);
			m_pLstArea->SetInvisible(TRUE);
		}
        break;
	case EGUIE_Click:
		if (pWnd == m_pBtRefresh)	//ˢ��
		{
			UpdateInformation();
            m_bisRefreshing = true;
            m_pBtRefresh->SetEnable(false);
            m_pBtRefresh->SetRefresh();
		}
		else if (pWnd == m_pBtSave)	//����
		{
			SaveAll();
		}
		else if(pWnd == m_pBtnArea)
		{
			m_pLstArea->SetInvisible(!m_pLstArea->IsInvisible());
			m_pLstProvince->SetInvisible(TRUE);
		}
		else if(pWnd == m_pBtnProvince)
		{
			m_pLstProvince->SetInvisible(!m_pLstProvince->IsInvisible());
			m_pLstArea->SetInvisible(TRUE);
		}
		break;
	case EGUIE_PushButtonDown:
		if (pWnd == m_pPbtVisible)	//������ҿɼ�
		{
		}
		break;
	case EGUIE_ItemClick:
		if (pWnd == m_pComboBoxConstellation->GetListBox())	//����ѡ��
		{
			int i = pEvent->dwParam1;
		}
        else if (pWnd == m_pComboBoxGender->GetListBox())    // �Ա�ѡ��
        {
			/*
            switch (pEvent->dwParam1)
            {
            case 0:
                m_pStcHeadPic->SetDefaultPic(MALE_PIC_PATH);
            	break;
            case 1:
                m_pStcHeadPic->SetDefaultPic(FEMALE_PIC_PATH);
                break;
            case 2:
                m_pStcHeadPic->SetDefaultPic(_T(""));
            }
			*/
        }
		else if(pWnd == m_pLstArea)
		{
			m_pStcArea->SetText(m_pLstArea->GetText(pEvent->dwParam1, 0));
			m_pLstArea->SetInvisible(TRUE);
			//ˢ��ʡ���б�
			ChangeProvince((INT)pEvent->dwParam1);
		}
		else if(pWnd == m_pLstProvince)
		{
			if(m_pLstProvince->GetText(pEvent->dwParam1, 0) != (LPCTSTR)GT_INVALID)
			{
				m_pStcProvince->SetText(m_pLstProvince->GetText(pEvent->dwParam1, 0));
				m_pLstProvince->SetInvisible(TRUE);
			}
		}
		break;
	case EGUIE_Deactive:
		if(pWnd == m_pLstArea || pWnd == m_pStcArea)
		{
			m_pLstArea->SetInvisible(TRUE);
		}
		else if(pWnd == m_pLstProvince || pWnd == m_pStcProvince)
		{
			m_pLstProvince->SetInvisible(TRUE);
		}
		break;

	}
	return 0;
}

VOID InformationFrame::InitInfoData()
{
	//����Ա�����
	{
		//��
		m_pComboBoxGender->GetListBox()->SetText(0, 0, g_StrTable[_T("RoleInfoMale")]);
		m_mapGenderRow.insert(make_pair(0, 1));
		m_mapGenderID.insert(make_pair(1, 0));
		//Ů
		m_pComboBoxGender->GetListBox()->SetText(1, 0, g_StrTable[_T("RoleInfoFemale")]);
		m_mapGenderRow.insert(make_pair(1, 2));
		m_mapGenderID.insert(make_pair(2, 1));
		//����
		m_pComboBoxGender->GetListBox()->SetText(2, 0, g_StrTable[_T("RoleInfoDefault")]);
		m_mapGenderRow.insert(make_pair(2, 3));
		m_mapGenderID.insert(make_pair(3, 2));

		//����������
		tagPoint ptSize	= m_pComboBoxGender->GetListBox()->GetSize();

		ptSize.x = m_pComboBoxGender->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxGender->GetListBox()->GetRowHeight() * 3;

		m_pComboBoxGender->GetListBox()->SetResizable(TRUE);
		m_pComboBoxGender->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxGender->GetListBox()->Resize(ptSize);
		m_pComboBoxGender->GetListBox()->SetResizable(FALSE);
	}
	

	//�����������
	int i = 0;
	const map<DWORD, tagConstellationProto>* pCstMap =
		RoleInfoProtoData::inst()->GetConstellation();

	for (map<DWORD, tagConstellationProto>::const_iterator citer = pCstMap->begin();
		citer != pCstMap->end(); ++citer)
	{
		//����������i��
		m_pComboBoxConstellation->GetListBox()->SetText(i, 0,
			citer->second.strName.c_str());

		//�������б������id���ձ����ݲ���
		m_mapConstellationRow.insert(make_pair(i, citer->first));
		m_mapConstellationID.insert(make_pair(citer->first, i));
		++i;

		//����������
		tagPoint ptSize = m_pComboBoxConstellation->GetListBox()->GetSize();

		ptSize.x = m_pComboBoxConstellation->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxConstellation->GetListBox()->GetRowHeight() * pCstMap->size();

		m_pComboBoxConstellation->GetListBox()->SetResizable(TRUE);
		m_pComboBoxConstellation->GetListBox()->SetColWidth(0, ptSize.x	);
		m_pComboBoxConstellation->GetListBox()->Resize(ptSize);
		m_pComboBoxConstellation->GetListBox()->SetResizable(FALSE);
	}

	//�����Ф����
	i = 0;
	const map<DWORD, tagChinesZodiacProto>* pCzcMap =
		RoleInfoProtoData::inst()->GetChineseZodiac();

	for (map<DWORD, tagChinesZodiacProto>::const_iterator citer = pCzcMap->begin();
		citer != pCzcMap->end(); ++citer)
	{
		//����������i��
		m_pComboBoxChZodiac->GetListBox()->SetText(i, 0,
			citer->second.strName.c_str());

		//�������б������id���ձ����ݲ���
		m_mapChineseZodiacRow.insert(make_pair(i, citer->first));
		m_mapChineseZodiacID.insert(make_pair(citer->first, i));
		++i;

		//����������
		tagPoint ptSize = m_pComboBoxChZodiac->GetListBox()->GetSize();

		ptSize.y = m_pComboBoxChZodiac->GetListBox()->GetRowHeight() *
			RoleInfoProtoData::inst()->GetChineseZodiac()->size();

		m_pComboBoxChZodiac->GetListBox()->SetResizable(TRUE);
		m_pComboBoxChZodiac->GetListBox()->Resize(ptSize);
		m_pComboBoxChZodiac->GetListBox()->SetResizable(FALSE);
	}

	//�����������
	i = 0;
	const map<DWORD, tagAreaProto>* pAreaMap = RoleInfoProtoData::inst()->GetArea();

	for (map<DWORD, tagAreaProto>::const_iterator citer = pAreaMap->begin();
		citer != pAreaMap->end(); ++citer)
	{
		//����������i��
		m_pLstArea->SetText(i, 0,
			citer->second.strName.c_str());
		
		//�������б������id���ձ����ݲ���
		m_mapAreaRow.insert(make_pair(i, citer->first));
		m_mapAreaID.insert(make_pair(citer->first, i));
		++i;

		if(SaveAccount::Inst()->GetLoginMode() != 11)
		{
			//����������
			tagPoint ptSize = m_pLstArea->GetSize();

			ptSize.y = m_pLstArea->GetRowHeight() *
				RoleInfoProtoData::inst()->GetArea()->size();

			m_pLstArea->SetResizable(TRUE);
			m_pLstArea->Resize(ptSize);
			m_pLstArea->SetResizable(FALSE);
		}

	}
}

//��������������Ķ�����ʡ������������
VOID InformationFrame::ChangeProvince( INT areaRow )
{
	//���ʡ��������
	m_pLstProvince->Clear();
	m_pStcProvince->SetText(_T(""));

	//�����id����map
	m_mapProvinceRow.clear();
	m_mapProvinceID.clear();

	//����ѡ�е�����id
	map<INT, DWORD>::const_iterator citerAreaRow = m_mapAreaRow.find(areaRow);
	if (citerAreaRow == m_mapAreaRow.end())
		return;

	DWORD areaID = citerAreaRow->second;

	//��������id�������ԭ������
	const map<DWORD, tagAreaProto> *mapArea = RoleInfoProtoData::inst()->GetArea();
	map<DWORD, tagAreaProto>::const_iterator citerArea = mapArea->find(areaID);
	if (citerArea == mapArea->end())
		return;

	//���ʡ������
	int i = 0;
	for (map<DWORD, tagProvinceProto>::const_iterator citer =
		citerArea->second.mapProvince.begin();
		citer != citerArea->second.mapProvince.end(); ++citer)
	{
		m_pLstProvince->SetText(i, 0, citer->second.strName.c_str());
		//������/id���ձ�
		m_mapProvinceRow.insert(make_pair(i, citer->first));
		m_mapProvinceID.insert(make_pair(citer->first, i));
		++i;

		if(SaveAccount::Inst()->GetLoginMode() != 11)
		{
			//����������
			tagPoint ptSize = m_pLstProvince->GetSize();

			ptSize.y = m_pLstProvince->GetRowHeight() *
				citerArea->second.mapProvince.size();

			m_pLstProvince->SetResizable(TRUE);
			m_pLstProvince->Resize(ptSize);
			m_pLstProvince->SetResizable(FALSE);
		}

	}
}

DWORD InformationFrame::OnNS_GetRoleVCard( tagNS_GetRoleVCard* pMsg, DWORD dwParam )
{
	if(pMsg->dwRoleID != RoleMgr::Inst()->GetLocalPlayerID())
		return 0;

	//�������������
	tagVCardData* pVCardData = (tagVCardData*)pMsg->pData;

	map<DWORD, INT>::iterator it;

	//д���û��Ѵ��ڷ�����������

	//�Ա�
	it = m_mapGenderID.find((DWORD)pVCardData->customVCardData.bySex);
	if (it != m_mapGenderID.end())
	{
		m_pComboBoxGender->GetListBox()->SetCurSelectedRow(it->second);
		LPCTSTR szTemp = m_pComboBoxGender->GetListBox()->GetText(it->second, 0);
		m_pComboBoxGender->GetEditBox()->SetText(szTemp);
	}

	//����
	it = m_mapConstellationID.find((DWORD)pVCardData->customVCardData.byConstellation);
	if (it != m_mapConstellationID.end())
	{
		m_pComboBoxConstellation->GetListBox()->SetCurSelectedRow(it->second);
		LPCTSTR szTemp = m_pComboBoxConstellation->GetListBox()->GetText(it->second, 0);
		m_pComboBoxConstellation->GetEditBox()->SetText(szTemp);
	}

	//��Ф
	it = m_mapChineseZodiacID.find((DWORD)pVCardData->customVCardData.byChineseZodiac);
	if (it != m_mapChineseZodiacID.end())
	{
		m_pComboBoxChZodiac->GetListBox()->SetCurSelectedRow(it->second);
		LPCTSTR szTemp = m_pComboBoxChZodiac->GetListBox()->GetText(it->second, 0);
		m_pComboBoxChZodiac->GetEditBox()->SetText(szTemp);
	}

	//����
	it = m_mapAreaID.find((DWORD)pVCardData->customVCardData.byArea);
	if (it != m_mapAreaID.end())
	{
		m_pLstArea->SetCurSelectedRow(it->second);
		LPCTSTR szTemp = m_pLstArea->GetText(it->second, 0);
		m_pStcArea->SetText(szTemp);
		
		//����ʡ�������б�
		ChangeProvince(it->second);

		//ʡ��
		map<DWORD, INT>::iterator iter;
		iter = m_mapProvinceID.find((DWORD)pVCardData->customVCardData.byProvince);
		if (iter != m_mapProvinceID.end())
		{
			m_pLstProvince->SetCurSelectedRow(iter->second);
			szTemp = m_pLstProvince->GetText(iter->second, 0);
			m_pStcProvince->SetText(szTemp);
		}
	}

	//����
	m_pEditCityName->SetText(pVCardData->customVCardData.chCity);
	char szout[X_LONG_NAME] = {0};
	m_pUtil->UnicodeToUnicode8(m_pEditCityName->GetText(),szout);
	if(memcmp(szout,"N/A",3) == 0)
		m_pEditCityName->SetText(_T(""));
/*
	//ͷ��URL
	TCHAR szImageURL[200] = {_T('\0')};
	memcpy(szImageURL, pVCardData->customVCardData.byData, (INT)pVCardData->customVCardData.byHeadUrlSize);
    tstring strImageURL = szImageURL;
    m_pStcHeadPic->SetPicURL(strImageURL);

    if (pVCardData->customVCardData.bySex == 1)
        m_pStcHeadPic->SetDefaultPic(MALE_PIC_PATH);
    else if (pVCardData->customVCardData.bySex == 2)
        m_pStcHeadPic->SetDefaultPic(FEMALE_PIC_PATH);
    else
        m_pStcHeadPic->SetDefaultPic(_T(""));

    m_pStcHeadPic->SetInvisible(FALSE);
    m_pStcHeadPic->SetRefresh(TRUE);
*/


	TCHAR szMMPic[X_LONG_NAME] ;
	TCHAR szFullPath[X_LONG_NAME];
	_stprintf( szFullPath, _T("data\\ui\\photo\\%d_%d.jpg"), SaveAccount::Inst()->GetLoginID(),SaveAccount::Inst()->GetAccountID());
	tstring strPath;
	strPath += szFullPath;
	DWORD dwFileSize = TObjRef<VirtualFileSys>("VFS_System")->GetSize(strPath.c_str());
	if(dwFileSize != GT_INVALID && dwFileSize != (DWORD)GT_INVALID)	//�ļ�����
	{
		_stprintf( szMMPic, _T("data\\ui\\photo\\%d_%d.jpg"), SaveAccount::Inst()->GetLoginID(),SaveAccount::Inst()->GetAccountID());
		m_pStcHeadPic->SetPic(szMMPic);
	}
	else	//�ļ�������
	{
		m_pStcHeadPic->SetPic(DEFAULT_PIC_PATH);
	}
	m_pStcHeadPicC->SetRefresh(TRUE);

	//����ǩ��
	TCHAR szbuff[200] = {_T('\0')};
	INT nOffset = (INT)pVCardData->customVCardData.byHeadUrlSize;
	INT nSize = (INT)pVCardData->customVCardData.bySignatureSize;
	memcpy(szbuff, pVCardData->customVCardData.byData + nOffset, nSize);
	m_pEditBoxExPresentation->SetText(szbuff);
	if(m_pEditBoxExPresentation->GetText(false) == _T("N/A"))
		m_pEditBoxExPresentation->SetText(_T(""));

	// ������¼
	TCHAR szNameHistory[X_HUGE_NAME] = {_T('\0')};
	nOffset += nSize;
	nSize = (INT)pVCardData->customVCardData.byNameHistory;
	memcpy(szNameHistory, pVCardData->customVCardData.byData + nOffset, nSize);
	m_pEditRenameHistory->SetText(szNameHistory);
	if(m_pEditRenameHistory->GetText(false) == _T("N/A"))
		m_pEditRenameHistory->SetText(_T(""));

	//�ɼ��Ե�ѡ��
	m_pPbtVisible->SetState(pVCardData->customVCardData.bVisibility ? EGUIBS_PushDown : EGUIBS_PopUp);
	return 0;
}

DWORD InformationFrame::OnNS_SetRoleVCard( tagNS_SetRoleVCard* pMsg, DWORD dwParam )
{
	//��Ӧ������������
	if (RoleMgr::Inst()->GetLocalPlayerID() != pMsg->dwRoleID)
		return 0;

	//�������
	switch (pMsg->dwErrorCode)
	{
	case 0:
		break;
	}
	return 0;
}

VOID InformationFrame::SaveAll()
{
	//image url������ȥ�մ�����Ӫ�������������ݿ���棬��������ȥ�մ���
	LPCTSTR szImageURL = _T("");

	//url�ֽڳ���
	INT nImageURLByteSize = (_tcslen(szImageURL) + 1) * sizeof(TCHAR);

	//ȡ�ø���ǩ��
    tstring strText = m_pEditBoxExPresentation->GetText(FALSE);
    strText = strText.substr(0, 52);
    FilterData::Inst()->WordsFilteror(strText);
	LPCTSTR szText = strText.c_str();

	//����ǩ���ֽڳ���
	INT nSignatureByteSize = ( _tcslen(szText) + 1 ) * sizeof(TCHAR);

	// ȡ�ø�����¼
	tstring strNameText = m_pEditRenameHistory->GetText(FALSE);
	LPCTSTR szNameText = strNameText.c_str();

	// ������¼����
	INT nNameHisByteSize = ( _tcslen(szNameText) + 1 ) * sizeof(TCHAR);

	//���·���ṹ��ռ�(����䳤��Ϣ)
	DWORD					dwSize = ( sizeof(tagNC_SetRoleVCard) - 1 ) + nImageURLByteSize + nSignatureByteSize + nNameHisByteSize;
	BYTE*					pbyTemp = new BYTE[dwSize];
	ZeroMemory(pbyTemp, dwSize);

	//ԭ��Ϣ�ṹ�ռ�
	tagNC_SetRoleVCard		vCardDataTemp;

	//����Ϣ�ṹ��ַ
	tagNC_SetRoleVCard*		pVCardData = (tagNC_SetRoleVCard*)pbyTemp;

	//����ԭ�ṹ��(������byData[1])
	memcpy(pbyTemp, &vCardDataTemp, sizeof(vCardDataTemp));

	//����image url
	memcpy(pbyTemp + sizeof(vCardDataTemp) - 1, szImageURL, nImageURLByteSize);

	//��������ǩ��
	memcpy(pbyTemp + sizeof(vCardDataTemp) - 1 + nImageURLByteSize, szText, nSignatureByteSize);	//������'\0'

	//����������¼
	memcpy(pbyTemp + sizeof(vCardDataTemp) - 1 + nImageURLByteSize + nSignatureByteSize , szNameText, nNameHisByteSize);

	//�ṹ���Ա��ֵ
	pVCardData->dwSize								=	dwSize;
	pVCardData->dwRoleID							=	RoleMgr::Inst()->GetLocalPlayerID();
	pVCardData->customVCardData.byHeadUrlSize		=	(BYTE)nImageURLByteSize;
	pVCardData->customVCardData.bySignatureSize		=	(BYTE)nSignatureByteSize;
	pVCardData->customVCardData.byNameHistory		=	(BYTE)nNameHisByteSize;
	pVCardData->customVCardData.bVisibility			=	m_pPbtVisible->GetState() == EGUIBS_PushDown ? TRUE : FALSE;
	pVCardData->customVCardData.bySex				=	(BYTE)FindValuebyKey<INT, DWORD>(&m_mapGenderRow, m_pComboBoxGender->GetListBox()->GetCurSelectedRow());
	pVCardData->customVCardData.byConstellation		=	(BYTE)FindValuebyKey<INT, DWORD>(&m_mapConstellationRow, m_pComboBoxConstellation->GetListBox()->GetCurSelectedRow());
	pVCardData->customVCardData.byChineseZodiac		=	(BYTE)FindValuebyKey<INT, DWORD>(&m_mapChineseZodiacRow, m_pComboBoxChZodiac->GetListBox()->GetCurSelectedRow());
	pVCardData->customVCardData.byArea				=	(BYTE)FindValuebyKey<INT, DWORD>(&m_mapAreaRow, m_pLstArea->GetCurSelectedRow());
	pVCardData->customVCardData.byProvince			=	(INT)FindValuebyKey<INT, DWORD>(&m_mapProvinceRow, m_pLstProvince->GetCurSelectedRow());
    tstring strCity = m_pEditCityName->GetText();
    strCity = strCity.substr(0, LEN_CITY_NAME - 1);
    FilterData::Inst()->WordsFilteror(strCity);
	_tcscpy_s(pVCardData->customVCardData.chCity, sizeof(pVCardData->customVCardData.chCity)/sizeof(TCHAR), strCity.c_str());

	//������Ϣ
	m_pSession->Send(pVCardData);

	//�ͷſռ�
	SAFE_DELETEA(pbyTemp);


}

template<class TKey, class TValue>
TValue InformationFrame::FindValuebyKey( map<TKey, TValue>* pMap, TKey tKey )
{
	map<TKey, TValue>::const_iterator citer = pMap->find(tKey);
	if (citer == pMap->end())
		return 0;

	return citer->second;
}

tstring InformationFrame::GetRenameHis()
{
	tstring strHis = m_pEditRenameHistory->GetText(false);
	if( strHis == _T("N/A"))
		return _T("");
	int nPos = 0;
	int nNum = 0;
	tstring StrNew;
	while( ( nPos = strHis.find( '\r', nPos + 1 ) ) != strHis.npos )
	{
		nNum++;
		if( MAX_RENAME_HISTORY == nNum )
		{
			int nLength = strHis.length();
			int nFirstPos = strHis.find( '\r', 0 );
			StrNew = strHis.substr( nFirstPos + 2 , nLength - nFirstPos );
			return StrNew;
		}
	}
	return strHis;
}


DWORD InformationFrame::OnNS_LPRename( tagNS_LPRename *pNetCmd, DWORD )
{
	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLP))
		return 0;
	if( pNetCmd->dwRoleID != pLP->GetID() )
		return 0;

	if( pNetCmd->dwErrorCode == E_Success )
	{
		tstring strName = GetRenameHis();
		TCHAR szName[X_LONG_NAME] = {0};
		_sntprintf(szName, X_LONG_NAME, g_StrTable[_T("RenameHistory")], pNetCmd->szOldName, pNetCmd->szNewName );
		tstringstream info;
		info << strName << szName << _T("\n");
		m_pEditRenameHistory->SetText(info.str().c_str());
		SaveAll();
	}
	
	return 0;
}
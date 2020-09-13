#include "stdafx.h"
#include "TalentTree.h"
#include "SkillProtoData.h"
#include "SkillMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "TalentButton.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "FlashPushButton.h"

const SIZE TALENT_FRAME_SIZE	=	{28, 18};
const SIZE TALENT_SPACE_SIZE	=	{14, 14};
const SIZE TALENT_LEAF_SIZE		=	{37, 37};
const SIZE TALENT_BIDDEN_SIZE	=	{17, 7};
const SIZE TALENT_BIDDEN_OFFSET =	{0,  39};
const SIZE TALENT_BRANCH_SIZE	=	{3,  9};
const SIZE TALENT_BRANCH_OFFSET =	{17, 42};
const SIZE TALENT_WINDOW_SIZE   =	{481,475};


TalentTree::TalentTree(void)
{
	for (INT i=0; i<ETT_End; ++i)
	{
		m_pWndTalent[i]=NULL;
	}

	m_nCurPage = GT_INVALID;
	m_bSimpleDisplayMode = FALSE;
}

TalentTree::~TalentTree(void)
{
	
}

VOID TalentTree::Create(GUIWnd* pWnd, GUISystem* pGUI)
{
	// ����ǰ���������
	FreeMap(m_mapTalent);
	FreeMap(m_mapTalentSkillEffect);
	FreeMap(m_mapRoleLevelEffect);
	FreeMap(m_mapRoleClassEffect);
	m_mapIndex.Clear();
	for (int i=0; i<ETT_End; ++i)
	{
		FreeMap(m_mapTalentPointEffect[i]);
		m_listActiveTalent[i].Clear();
	}
	
	tstring strPath = g_strLocalPath + _T("\\attdata\\talent_tree.xml");
	LoadTalentConfig(strPath.c_str());
	
	for(INT i=0; i<ETT_End; ++i)
	{
		//���ȴ����ײ��ؼ�
		CreateWnd((ETalentType)i, pWnd, pGUI);
	}
	
	// ����������
	DWORD dwID;
	tagTalentLeaf* pLeaf = NULL;
	m_mapTalent.ResetIterator();
	while (m_mapTalent.PeekNext(dwID, pLeaf))
	{
		ASSERT(pLeaf->eTalentType > ETT_Null && pLeaf->eTalentType < ETT_End);
		if(pLeaf->eTalentType <= ETT_Null || pLeaf->eTalentType >= ETT_End) continue;

		CreateLeaf(pLeaf, m_pWndTalent[pLeaf->eTalentType], pGUI);
		
		if (P_VALID(pLeaf->dwChildLeaf) && m_mapTalent.IsExist(pLeaf->dwChildLeaf))
		{
			CreateBranch(pLeaf, m_mapTalent.Peek(pLeaf->dwChildLeaf)->Pos, m_pWndTalent[pLeaf->eTalentType], pGUI);
		}

		if (P_VALID(pLeaf->dwParentLeaf) && m_mapTalent.IsExist(pLeaf->dwParentLeaf))
		{
			CreateBranch(pLeaf, m_mapTalent.Peek(pLeaf->dwParentLeaf)->Pos, m_pWndTalent[pLeaf->eTalentType], pGUI);
		}

		/*m_mapTalent.ChangeValue(dwID, pLeaf);*/

		// ����dwID
		ASSERT(P_VALID(pLeaf->pBtnTalentLeaf));
		m_mapIndex.Add(pLeaf->pBtnTalentLeaf->GetID(), dwID);
	}		
	
}

VOID TalentTree::Destroy()
{
	// �������
	FreeMap(m_mapTalent);
	FreeMap(m_mapTalentSkillEffect);
	FreeMap(m_mapRoleLevelEffect);
	FreeMap(m_mapRoleClassEffect);
	m_mapIndex.Clear();
	m_mapBoxFirst2Talent.Clear();
	for (int i=0; i<ETT_End; ++i)
	{
		FreeMap(m_mapTalentPointEffect[i]);
		m_listActiveTalent[i].Clear();
	}

	m_nCurPage = GT_INVALID;
}

VOID TalentTree::LoadTalentConfig(LPCTSTR szFile)
{
	XmlDocument doc;
	if( !doc.LoadFile("VFS_System", szFile) )
		return;

	XmlHandle docHandle( &doc );
	XmlHandle eleHandle = docHandle.FirstChildElement("desktop").FirstChildElement("skill");
	XmlElement* pElement = eleHandle.Element();
	if( !pElement )	
		return;

	while( pElement )
	{
		LoadTalentLeaf(pElement);
		pElement = pElement->NextSiblingElement("skill");
	}
}

VOID TalentTree::LoadTalentLeaf(XmlElement* pElement)
{
	XmlAttribute* pAttrib = pElement->FirstAttribute();
	tstring strValue;
	tagTalentLeaf* pLeaf = new tagTalentLeaf;
	BOOL bBox = FALSE;
	DWORD skillID = GT_INVALID;

	while(pAttrib)
	{
		const std::string& strName = pAttrib->NameStr();
		strValue = m_pUtil->Unicode8ToUnicode(pAttrib->Value());

		if ( "id" == strName )
		{
			skillID = (DWORD)_ttoi(strValue.c_str());
			pLeaf->dwTalentLeaf = skillID;
			if(skillID < 10000)
			{
				const tagSkillBoxProto* pSkillBox = SkillProtoData::Inst()->FindSkillBoxProto(skillID);
				if(!P_VALID(pSkillBox) || !pSkillBox->bCanBeLearned)
				{
					SAFE_DEL(pLeaf);
					return;
				}
				skillID = pSkillBox->dwSkillIDs[0];
				bBox = TRUE;
			}
			const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(MTransSkillTypeID(skillID, 1));
			if(P_VALID(pSkill) && pSkill->eTalentType > ETT_Null && pSkill->eTalentType < ETT_End)
			{
				pLeaf->eTalentType = pSkill->eTalentType;
				pLeaf->eUseType = pSkill->eUseType;
				pLeaf->nMaxLevel = pSkill->nMaxLearnLevel;
				pLeaf->eLevelUpType = pSkill->eLevelUpType;
				
				//���¸ü������еȼ��Ŀ���Ӱ���
				for (INT i=1; i<=pSkill->nMaxLearnLevel; ++i)
				{
					AddTalentActiveEffect(MTransSkillTypeID(skillID, i));
				}

			}
			else
			{
				SAFE_DEL(pLeaf);
				return;
			}
		}
		else if( "pic" == strName )
		{
			_tcsncpy(pLeaf->szIconPath, strValue.c_str(), X_SHORT_NAME/sizeof(TCHAR));
		}
		else if ("pre_id" == strName)
		{
			pLeaf->dwParentLeaf = (DWORD)_ttoi(strValue.c_str());
		}
		else if( "follow_id" == strName )
		{
			pLeaf->dwChildLeaf = (DWORD)_ttoi(strValue.c_str());
		}
		else if( "col" == strName )
		{
			pLeaf->Pos.y = _ttoi(strValue.c_str()) - 1;
		}
		else if( "row" == strName )
		{
			pLeaf->Pos.x = _ttoi(strValue.c_str()) - 1;
		}
		else if( "icon_ctrl_id" == strName )
		{
			pLeaf->dwIconCtrlId = (DWORD)_ttoi( strValue.c_str() );
		}

		pAttrib = pAttrib->Next();
	}

	m_mapTalent.Add(pLeaf->dwTalentLeaf, pLeaf);
	if(bBox) m_mapBoxFirst2Talent.Add(skillID, pLeaf);
}

VOID TalentTree::CreateWnd(ETalentType eType, GUIWnd* pFather, GUISystem* pGUI)
{
	XmlElement ele;
	CHAR szTmp[X_SHORT_NAME]={0};

	{
		sprintf(szTmp, "talentwindow%d", eType);
		ele.SetAttribute("Name",		szTmp);
		ele.SetAttribute("ClassName",	"CGUIStatic");
		ele.SetAttribute("Size_x",		TALENT_WINDOW_SIZE.cx);
		ele.SetAttribute("Size_y",		TALENT_WINDOW_SIZE.cy);
		ele.SetAttribute("LogicPos_x",	0);
		ele.SetAttribute("LogicPos_y",	0);
		ele.SetAttribute("PicColor",	0xFFFFFF);
	}
	m_pWndTalent[eType] = pGUI->CreateWnd(pFather->GetFullName().c_str(), &ele);
	ASSERT(P_VALID(m_pWndTalent[eType]));
	m_pWndTalent[eType]->SetInvisible(TRUE);
	
}

VOID TalentTree::CreateLeaf(tagTalentLeaf* pLeaf, GUIWnd* pFather, GUISystem* pGUI)
{
	XmlElement ele;
	CHAR szTemp[X_SHORT_NAME];
	TCHAR szTmp[X_LONG_NAME];

	SIZE LeafSize;
	GetCurLeafPos(LeafSize, pLeaf->Pos);

	// �������ʰ�ť
	{
		sprintf(szTemp, "talentleaf%d%d", pLeaf->Pos.x, pLeaf->Pos.y);
		ele.SetAttribute("Name", szTemp);
		ele.SetAttribute("ClassName", "TalentButton");
		ele.SetAttribute("PicColor", 0xFF9A9A9A);
		ele.SetAttribute("PushPicColor", 0xFF9A9A9A);
		ele.SetAttribute("MovePicColor", 0xFF9A9A9A);
		ele.SetAttribute("Size_x", TALENT_LEAF_SIZE.cx);
		ele.SetAttribute("Size_y", TALENT_LEAF_SIZE.cy);
		
		if (pLeaf->eUseType == ESUT_Active)
		{
			ele.SetAttribute("LogicPos_x", LeafSize.cx - 1);
			ele.SetAttribute("LogicPos_y", LeafSize.cy - 1);
			ele.SetAttribute("PicFileName", "data\\ui\\Skill\\L-skill-icon-a.bmp");			
		}
		else if(pLeaf->eUseType == ESUT_Passive)
		{
			ele.SetAttribute("LogicPos_x", LeafSize.cx);
			ele.SetAttribute("LogicPos_y", LeafSize.cy);
			ele.SetAttribute("PicFileName", "data\\ui\\Skill\\L-skill-icon-b.bmp");
		}
		else 
		{
			return;
		}
	}
	pLeaf->pBtnTalentLeaf = (TalentButton*)pGUI->CreateWnd(pFather->GetFullName().c_str(), &ele);
	ASSERT(P_VALID(pLeaf->pBtnTalentLeaf));
	
	// ��������ͼ��
	_stprintf(szTmp, _T("data\\ui\\Icon\\%s.tga"), pLeaf->szIconPath);
	pLeaf->pBtnTalentLeaf->SetIcon(szTmp);
	
	
	if (pLeaf->eUseType == ESUT_Active)
	{
		pLeaf->pBtnTalentLeaf->SetAlphaTex(_T("data\\ui\\Skill\\L-skill-icon-c.bmp"));			
	}
	else
	{
		pLeaf->pBtnTalentLeaf->SetAlphaTex(_T("data\\ui\\Skill\\L-skill-icon-d.bmp"));
	}

	ele.SetAttribute("PicFileName", "");
	
	// ����������Ͷ����
	{
		sprintf(szTemp, "talentbidden%d%d", pLeaf->Pos.x, pLeaf->Pos.y);
		ele.SetAttribute("Name", szTemp);
		ele.SetAttribute("ClassName", "FlashStatic");
		ele.SetAttribute("Size_x", TALENT_BIDDEN_SIZE.cx);
		ele.SetAttribute("Size_y", TALENT_BIDDEN_SIZE.cy);
		ele.SetAttribute("LogicPos_x", LeafSize.cx + TALENT_BIDDEN_OFFSET.cx);
		ele.SetAttribute("LogicPos_y", LeafSize.cy + TALENT_BIDDEN_OFFSET.cy);
		ele.SetAttribute("PicColor", 0xFFFFFF);
		ele.SetAttribute("Font", m_pUtil->UnicodeToUnicode8(g_StrTable[_T("Font_Kaiti")]));
		ele.SetAttribute("FontHeight", 12);
		ele.SetAttribute("TextRect_left", 0);
		ele.SetAttribute("TextRect_top", 0);
		ele.SetAttribute("TextRect_right", TALENT_BIDDEN_SIZE.cx);
		ele.SetAttribute("TextRect_bottom", TALENT_BIDDEN_SIZE.cy);
		ele.SetAttribute("TextColor", 0xFF9A9A9A);
		ele.SetAttribute("TextAlign", "RightCenter");
	}	
	pLeaf->pStcBidden = (FlashStatic*)pGUI->CreateWnd(pFather->GetFullName().c_str(), &ele);
	ASSERT(P_VALID(pLeaf->pStcBidden));

	// ������������Ͷ����
	{
		_snprintf(szTemp, sizeof(szTemp), "talentmaxbid%d%d", pLeaf->Pos.x, pLeaf->Pos.y);
		ele.SetAttribute("Name", szTemp);
		ele.SetAttribute("LogicPos_x", LeafSize.cx + TALENT_BIDDEN_OFFSET.cx + TALENT_BIDDEN_SIZE.cx);
		ele.SetAttribute("LogicPos_y", LeafSize.cy + TALENT_BIDDEN_OFFSET.cy);
		ele.SetAttribute("TextAlign", "LeftCenter");
	}
	pLeaf->pStcMaxPoint = (FlashStatic*)pGUI->CreateWnd(pFather->GetFullName().c_str(), &ele);
	ASSERT(P_VALID(pLeaf->pStcMaxPoint));
	
	//--��д����Ͷ��
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("/%d"), pLeaf->nMaxLevel);
	pLeaf->pStcMaxPoint->SetText(szTmp);
	
	//--ˢ�µ�ǰ��Ͷ����
	pLeaf->bActive = IsTalentLeafActive(pLeaf);
	ReflashActiveTalentList(pLeaf);
	SetBidden(pLeaf);
}	

VOID TalentTree::CreateBranch(tagTalentLeaf* pLeaf, tagPoint ChildPos, GUIWnd* pFather, GUISystem* pGUI)
{
	XmlElement ele;
	CHAR szTemp[X_SHORT_NAME];
    
	tagPoint ChildPosTemp(pLeaf->Pos.x, ChildPos.y);
    SIZE BranchSize = {0, 0};

	// �Ȼ�����
	if(ChildPosTemp.y != pLeaf->Pos.y)
	{
		GetCurBranchSize(BranchSize, pLeaf->Pos, ChildPosTemp);		

		SIZE LeafSize = {0, 0};
		if(pLeaf->Pos.y < ChildPosTemp.y)
			GetCurLeafPos(LeafSize, pLeaf->Pos);
		else
			GetCurLeafPos(LeafSize, ChildPosTemp);

		// ����������֦
		{
			sprintf(szTemp, "talentbranch%d%d", pLeaf->Pos.x, pLeaf->Pos.y);
			ele.SetAttribute("Name", szTemp);
			ele.SetAttribute("ClassName", "CGUIStatic");
			ele.SetAttribute("Size_x", BranchSize.cx);
			ele.SetAttribute("Size_y", BranchSize.cy);
			ele.SetAttribute("LogicPos_x", LeafSize.cx + TALENT_BRANCH_OFFSET.cx);
			ele.SetAttribute("LogicPos_y", LeafSize.cy + TALENT_LEAF_SIZE.cy / 2);
			ele.SetAttribute("PicColor", 0xFFFFFFFF);
			ele.SetAttribute("BottomMost", "yes");
			ele.SetAttribute("PicRect_Left", 1);
			ele.SetAttribute("PicRect_right", 2);
			ele.SetAttribute("PicRect_top", 1);
			ele.SetAttribute("PicRect_bottom", 8);
			ele.SetAttribute("PicFileName", "data\\ui\\Skill\\L-skill-tiao.bmp");
		}
		pLeaf->pStcTalentBranch = (GUIStatic*)pGUI->CreateWnd(pFather->GetFullName().c_str(), &ele);
		ASSERT(P_VALID(pLeaf->pStcTalentBranch));
	}
	//�ٻ�����
	if(ChildPosTemp.x != ChildPos.x)
	{
		GetCurBranchSize(BranchSize, ChildPos, ChildPosTemp);		

		SIZE LeafSize = {0, 0};
		if(ChildPos.x < ChildPosTemp.x)
			GetCurLeafPos(LeafSize, ChildPos);
		else
			GetCurLeafPos(LeafSize, ChildPosTemp);

		// ����������֦
		{
			ele = XmlElement();
			sprintf(szTemp, "talentbranchx%d%d", pLeaf->Pos.x, pLeaf->Pos.y);
			ele.SetAttribute("Name", szTemp);
			ele.SetAttribute("ClassName", "CGUIStatic");
			ele.SetAttribute("Size_x", BranchSize.cx+abs(ChildPosTemp.x - ChildPos.x)+4);
			ele.SetAttribute("Size_y", BranchSize.cy);
			ele.SetAttribute("LogicPos_x", LeafSize.cx + TALENT_BRANCH_OFFSET.cx-abs(ChildPosTemp.x - ChildPos.x)-4);
			ele.SetAttribute("LogicPos_y", LeafSize.cy + TALENT_LEAF_SIZE.cy / 2);
			ele.SetAttribute("PicColor", 0xFFFFFFFF);
			ele.SetAttribute("BottomMost", "yes");
			ele.SetAttribute("PicRect_Left", 1);
			ele.SetAttribute("PicRect_right", 8);
			ele.SetAttribute("PicRect_top", 0);
			ele.SetAttribute("PicRect_bottom", 3);
			ele.SetAttribute("PicFileName", "data\\ui\\Skill\\L-skill-tiao-01.bmp");
		}
		pGUI->CreateWnd(pFather->GetFullName().c_str(), &ele);
	}
}

VOID TalentTree::GetCurLeafPos(SIZE& size, tagPoint pos)
{
	size.cx = TALENT_FRAME_SIZE.cx 
		+ pos.x * TALENT_LEAF_SIZE.cx 
		+ pos.x * TALENT_SPACE_SIZE.cx;
	size.cy = TALENT_FRAME_SIZE.cy 
		+ pos.y * TALENT_LEAF_SIZE.cy 
		+ pos.y * TALENT_SPACE_SIZE.cy;
}

VOID TalentTree::GetCurBranchSize(SIZE& size, tagPoint pos, tagPoint child)
{
	INT col,row;
	col = abs(child.x - pos.x);
	row = abs(child.y - pos.y);
	if (0 == col)
	{
		if(row > 0)
		{
			size.cx = TALENT_BRANCH_SIZE.cx;
			size.cy = (TALENT_LEAF_SIZE.cy + TALENT_SPACE_SIZE.cy)* row + 1;
		}
	} else if (0 == row)
	{
		if(col > 0)
		{
			size.cx = (TALENT_LEAF_SIZE.cx + TALENT_SPACE_SIZE.cx) * col + 1;
			size.cy = TALENT_BRANCH_SIZE.cx;
		}
	}
}

VOID TalentTree::SetCurPage( INT nPage )
{
	ASSERT(nPage > ETT_Null && nPage < ETT_End);
	
	// ���ԭ�е�ǰҳΪ��Чֵ������ԭ�е�ǰҳ
	if(ETT_Null < m_nCurPage && ETT_End > m_nCurPage)
	{
		m_pWndTalent[m_nCurPage]->SetInvisible(TRUE);
	}
	
	// ��ʾ��ǰҳ
	m_nCurPage = nPage;
	m_pWndTalent[m_nCurPage]->SetInvisible(FALSE);
	m_pWndTalent[m_nCurPage]->SetRefresh(TRUE);
	m_pWndTalent[m_nCurPage]->FlipToTop();

}

VOID TalentTree::HideCurPage()       //���ص�ǰҳ������δ�ӵ�ʱ����ʾ����
{
	if(ETT_Null < m_nCurPage && ETT_End > m_nCurPage)
	{
		m_pWndTalent[m_nCurPage]->SetInvisible(TRUE);
		m_pWndTalent[m_nCurPage]->SetRefresh(TRUE);
	}
}

VOID TalentTree::UpdateTalentLeaf( DWORD dwID, BOOL bAdd )
{
	const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(dwID);
	// �����ʼ��ܲ�����
	if(P_VALID(pSkill))
	{
		if(ETT_Null != pSkill->pProto->eTalentType && ETT_End != pSkill->pProto->eTalentType )
		{
			tagTalentLeaf* pLeaf = m_mapTalent.Peek(dwID);
			if(!P_VALID(pLeaf)) pLeaf = m_mapBoxFirst2Talent.Peek(dwID);
			if(P_VALID(pLeaf))
			{
				pLeaf->nCurLevel = pSkill->nLevel;
				pLeaf->nLearnLevel = pSkill->nLearnLevel;
				pLeaf->eLevelUpType = pSkill->pProto->eLevelUpType;
				pLeaf->bActive = IsTalentLeafActive(pLeaf);
				ReflashActiveTalentList(pLeaf);
				SetBidden(pLeaf);
				
				//��ѧ�Ἴ�ܵ�ʱ��Ҫ�������������ܵ�ͼ��
				if( bAdd )
					RefreshTalentIcon( dwID );

				// �������ĸ�������
				if(bAdd && 1 != pSkill->nLearnLevel)
				{
					//������¼ӣ��Ҽ��ܲ�����ѧ�ᣬ����Ҫ����һ�¸ü��ܵĵ�һ��
					OnTalentLevelUp(MTransSkillTypeID(pSkill->dwSkillID, 1));
				}
				OnTalentLevelUp(MTransSkillTypeID(pSkill->dwSkillID, pSkill->nLearnLevel));
				
				//ˢ������Tips
				ReflashTalentTips(pLeaf);
			}
		}
	}
	else
	{
		//������ܲ������Ҽ���Ϊ
		tagTalentLeaf* pLeaf = m_mapTalent.Peek(dwID);
		if(P_VALID(pLeaf))
		{
			pLeaf->nCurLevel = 0;
			pLeaf->nLearnLevel = 0;
			pLeaf->bActive = IsTalentLeafActive(pLeaf);
			ReflashActiveTalentList(pLeaf);
			SetBidden(pLeaf);

			//ˢ������Tips
			ReflashTalentTips(pLeaf);
		}
	}
}

tagTalentLeaf* TalentTree::GetCurLeaf( DWORD dwWndID )
{
	DWORD dwSkillID = m_mapIndex.Peek(dwWndID);
	return m_mapTalent.Peek(dwSkillID);
	
}

BOOL TalentTree::IfSkillCanLevelUp(tagTalentLeaf* pLeaf)
{
	BOOL bResult = TRUE;
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(pLeaf->dwTalentLeaf);
	if(P_VALID(pSkill) && P_VALID(pLp))
	{
		if(ETT_Null != pSkill->pProto->eTalentType && ETT_End != pSkill->pProto->eTalentType )
		{
			const tagSkillProtoClient* pNext = SkillProtoData::Inst()->FindSkillProto(pSkill->dwTypeID + 1);
			if(P_VALID(pNext) )
			{
				if( pLp->GetRoleLevel() < pNext->nNeedRoleLevel )
					bResult = FALSE;
			}
			else if( !(pSkill->pProto->eClassLimit & ( 1 << (pLp->GetPlayerClass() - 1) )) )
			{
				bResult = FALSE;
			}
			else
				bResult = FALSE;
		}
	}
	else
	{
		const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(MTransSkillTypeID(pLeaf->dwTalentLeaf, 1));
		if( P_VALID(pLp) && P_VALID(pSkill) && pSkill->eTalentType > ETT_Null && pSkill->eTalentType < ETT_End )
		{
			if( !(pSkill->eClassLimit & ( 1 << (pLp->GetPlayerClass() - 1) )) )
			{
				bResult = FALSE;
			}
		}
	}

	INT nPoint = RoleMgr::Inst()->GetLocalPlayer()->GetAttribute(ERA_TalentPoint);
	if( nPoint == 0 || 
		( pLeaf->nLearnLevel == pLeaf->nMaxLevel || pLeaf->nCurLevel == pLeaf->nMaxLevel ) )	
		bResult = FALSE;
	return bResult;
}

VOID TalentTree::SetBidden(tagTalentLeaf* pLeaf)
{
	if( IfSkillCanLevelUp(pLeaf) && pLeaf->bActive )
	{
		pLeaf->pStcBidden->SetFlash(true);
		pLeaf->pStcMaxPoint->SetFlash(true);
	}
	else
	{
		pLeaf->pStcBidden->SetFlash(false);
		pLeaf->pStcMaxPoint->SetFlash(false);
	}
	
	//--�����Ƿ�Ͷ������������ͼ�����ɫ
	if(0 < pLeaf->nLearnLevel)
	{
		pLeaf->pBtnTalentLeaf->SetPicColor(0xFFFFFFFF);
	}
	else
	{
		pLeaf->pBtnTalentLeaf->SetPicColor(0xFF9A9A9A);
	}

	//--�����Ƿ��Ͷ����������Ͷ�����Ϳ�Ͷ��������ɫ
	if(pLeaf->bActive)
	{
		pLeaf->pStcBidden->SetTextColor(0xFF00FF00);
		pLeaf->pStcMaxPoint->SetTextColor(0xFF00FF00);
	}
	else
	{
		//--����ϰ�õȼ������ò���Ͷ������µ���Ͷ�����Ϳ�Ͷ��������ɫ
		if(0 == pLeaf->nLearnLevel)
		{
			pLeaf->pStcBidden->SetTextColor(0xFF9A9A9A);
			pLeaf->pStcMaxPoint->SetTextColor(0xFF9A9A9A);
		}
		else if(pLeaf->nMaxLevel == pLeaf->nLearnLevel)
		{
			pLeaf->pStcBidden->SetTextColor(0xFFFFFF00);
			pLeaf->pStcMaxPoint->SetTextColor(0xFFFFFF00);
		}
		else
		{
			pLeaf->pStcBidden->SetTextColor(0xFF00FF00);
			pLeaf->pStcMaxPoint->SetTextColor(0xFF00FF00);
		}
	}
		
	//--���ݵ�ǰ�ȼ���ϰ�õȼ�����������Ͷ��������ɫ
	if(pLeaf->nCurLevel > pLeaf->nLearnLevel)
	{
		pLeaf->pStcBidden->SetTextColor(0xFF00FFFF);
	}
	else if(pLeaf->nCurLevel < pLeaf->nLearnLevel)
	{
		pLeaf->pStcBidden->SetTextColor(0xFFFF0000);	
	}
	
	//--���ÿ�Ͷ��������ֵ
	TCHAR szTmp[X_SHORT_NAME]={0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), pLeaf->nCurLevel);
	pLeaf->pStcBidden->SetText(szTmp);

	pLeaf->pBtnTalentLeaf->SetRefresh();
	pLeaf->pStcBidden->SetRefresh(TRUE);
	pLeaf->pStcMaxPoint->SetRefresh();

}

template<typename K, typename V>
VOID TalentTree::FreeMap( TMap<K, V*> &mapList )
{
	V* pNode = NULL;

	mapList.ResetIterator();
	while (mapList.PeekNext(pNode))
	{
		SAFE_DEL(pNode);
	}

	mapList.Clear();
}

template<typename A, typename B>
VOID TalentTree::AddNode( TMap<A, TList<B>*> &mapList, A key, B value )
{
	TList<B>* pList = mapList.Peek(key);
	if(P_VALID(pList))
	{
		pList->PushBack(value);
	}
	else
	{
		pList = new TList<B>;
		pList->PushBack(value);
		mapList.Add(key, pList);
	}
}

VOID TalentTree::AddTalentActiveEffect( DWORD dwTypeID )
{
	const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(dwTypeID);
	if(!P_VALID(pSkill))
		return;

	// д�뼼�ܵȼ��Ժ������ܿ�����Ӱ��
	if(GT_INVALID != pSkill->dwPreLevelSkillID)
	{
		DWORD preSkillID = pSkill->dwPreLevelSkillID;
		if(preSkillID < 10000)
		{
			const tagSkillBoxProto* pSkillBox = SkillProtoData::Inst()->FindSkillBoxProto(preSkillID);
			if(P_VALID(pSkillBox))
			{
				if(pSkillBox->bCanBeLearned)
					AddNode(m_mapTalentSkillEffect, pSkillBox->dwSkillIDs[0], pSkill->dwID);
				else
				{
					for(int i=0; i< tagSkillBoxProto::E_SBP_Size; i++)
						if(P_VALID( pSkillBox->dwSkillIDs[i]))
							AddNode(m_mapTalentSkillEffect, pSkillBox->dwSkillIDs[i], pSkill->dwID);
				}
			}
		}
		else
			AddNode(m_mapTalentSkillEffect, preSkillID, pSkill->dwID);
	}
	// д�뵱ǰ���ʵ����Լ��ܿ�����Ӱ��
	if(0 != pSkill->nNeedTalentPoint)
	{
		ASSERT(pSkill->eTalentType > ETT_Null && pSkill->eTalentType < ETT_End);
		AddNode(m_mapTalentPointEffect[pSkill->eTalentType], pSkill->nNeedTalentPoint, pSkill->dwID);
	}
	// д����ҵȼ��Լ��ܿ�����Ӱ��
	if(0 != pSkill->nNeedRoleLevel && 1 != pSkill->nNeedRoleLevel)
	{
		AddNode(m_mapRoleLevelEffect, pSkill->nNeedRoleLevel, pSkill->dwID);
	}
	//// д�����ְҵ�Լ��ܿ�����Ӱ��
	//if(EV_Base != pSkill->eNeedClassType)
	//{
	//	AddNode(m_mapRoleClassEffect, pSkill->eNeedClassType, pSkill->dwID);
	//}
}

BOOL TalentTree::IsTalentLeafActive(tagTalentLeaf* pLeaf)
{
	//--�Ƿ��п�Ͷ����
	//if(0 >= pLp->GetAttribute(ERA_TalentPoint))
	//	return FALSE;

	//--�Ƿ�Ϊ�������ʷ���
	//if(!SkillMgr::Inst()->IsInsideOfMaxTalent(pLeaf->eTalentType))
	//	return FALSE;

	//--���������������Ͳ��ǹ̶�,�򲻿�Ͷ��
	if(ESLUT_Fixed != pLeaf->eLevelUpType)
		return FALSE;
	
	//--��������ѵ��������򲻿���Ͷ
	if(pLeaf->nMaxLevel <= pLeaf->nLearnLevel)
		return FALSE;
	DWORD skillID = pLeaf->dwTalentLeaf;
	// ���Ӵ���
	if(skillID < 10000)
	{
		const tagSkillBoxProto* pSkillBox = SkillProtoData::Inst()->FindSkillBoxProto(skillID);
		if(!P_VALID(pSkillBox) || !pSkillBox->bCanBeLearned) return FALSE;
		skillID = pSkillBox->dwSkillIDs[0];
	}
	//--�ж���һ�������Ƿ��ѧ
	if(E_Success != SkillMgr::Inst()->IsSkillCanLearn(MTransSkillTypeID(skillID, pLeaf->nLearnLevel+1)))
		return FALSE;
	
	return TRUE;
}

VOID TalentTree::OnRoleLevelUp( INT nLevel, INT nInc )
{
	//ֻ������������������
	TList<DWORD>* pList;
	for(INT i=nLevel-nInc; i<nLevel; ++i)
	{
		pList = m_mapRoleLevelEffect.Peek(i+1);
		if(P_VALID(pList))
		{
			DWORD dwID;
			pList->ResetIterator();
			while (pList->PeekNext(dwID))
			{
				tagTalentLeaf* pLeaf = m_mapTalent.Peek(MTransSkillID(dwID));
				if(!P_VALID(pLeaf)) pLeaf = m_mapBoxFirst2Talent.Peek(MTransSkillID(dwID));
				if(P_VALID(pLeaf) && dwID == GetNextLevelTalentTypeID(pLeaf, MTransSkillID(dwID)))
				{
					pLeaf->bActive = IsTalentLeafActive(pLeaf);
					ReflashActiveTalentList(pLeaf);
					SetBidden(pLeaf);
				}
			}
		}
	}
}

VOID TalentTree::OnRoleClassChange( EClassType eCur, EClassType ePast )
{
	ETalentType eTalent = SkillMgr::Inst()->GetMainTalentType();
	if(eTalent != ETT_Null)
	{
		OnTalentPointChange(eTalent, SkillMgr::Inst()->GetTalentPoint(eTalent) );
		DWORD dwID;
		tagTalentLeaf* pLeaf = NULL;
		m_mapTalent.ResetIterator();
		while (m_mapTalent.PeekNext(dwID, pLeaf))
		{
			ASSERT(pLeaf->eTalentType > ETT_Null && pLeaf->eTalentType < ETT_End);
			if(pLeaf->eTalentType == eTalent)
			{
				pLeaf->bActive = IsTalentLeafActive(pLeaf);
				ReflashActiveTalentList(pLeaf);
				SetBidden(pLeaf);
			}
		}
	}
}

VOID TalentTree::OnTalentPointChange( ETalentType eTalent, INT nCurPoint )
{
	if(0 != nCurPoint)// ���ʵ�����Ϊ����Ҫ���д������ﲻ����
	{
		for(int i=1; i<= nCurPoint; i++)
		{
			TList<DWORD>* pList = m_mapTalentPointEffect[eTalent].Peek(i);
			if(P_VALID(pList))
			{
				DWORD dwID;
				pList->ResetIterator();
				while (pList->PeekNext(dwID))
				{
					tagTalentLeaf* pLeaf = m_mapTalent.Peek(MTransSkillID(dwID));
					if(!P_VALID(pLeaf)) pLeaf = m_mapBoxFirst2Talent.Peek(MTransSkillID(dwID));
					if(P_VALID(pLeaf) && dwID == GetNextLevelTalentTypeID(pLeaf, MTransSkillID(dwID)))
					{
						pLeaf->bActive = IsTalentLeafActive(pLeaf);
						ReflashActiveTalentList(pLeaf);
						SetBidden(pLeaf);
					}
				}
			}
		}
	}
}

VOID TalentTree::OnTalentLevelUp( DWORD dwPreTalentID )
{
	TList<DWORD>* pList = m_mapTalentSkillEffect.Peek(dwPreTalentID);
	if(P_VALID(pList))
	{
		DWORD dwID;
		pList->ResetIterator();
		while (pList->PeekNext(dwID))
		{
			tagTalentLeaf* pLeaf = m_mapTalent.Peek(MTransSkillID(dwID));
			if(!P_VALID(pLeaf)) pLeaf = m_mapBoxFirst2Talent.Peek(MTransSkillID(dwID));
			if(P_VALID(pLeaf) && dwID == GetNextLevelTalentTypeID(pLeaf, MTransSkillID(dwID)))
			{
				pLeaf->bActive = IsTalentLeafActive(pLeaf);
				ReflashActiveTalentList(pLeaf);
				SetBidden(pLeaf);
			}
		}
	}
}

VOID TalentTree::OnNoTalentPoint()
{
	DWORD dwID;
	for(INT i=0; i<ETT_End; ++i)
	{
		// ��
		m_listActiveTalent[i].ResetIterator();
		while(m_listActiveTalent[i].PeekNext(dwID))
		{
			tagTalentLeaf* pLeaf = m_mapTalent.Peek(dwID);
			if(P_VALID(pLeaf))
			{
				pLeaf->bActive = FALSE;
				SetBidden(pLeaf);
			}
		}
	}
	
}

VOID TalentTree::OnNoTalentTypeCanChoose()
{
	DWORD dwID;
	for(INT i=0; i<ETT_End; ++i)
	{
		if(!SkillMgr::Inst()->IsInsideOfMaxTalent((ETalentType)i))
		{
			m_listActiveTalent[i].ResetIterator();
			while(m_listActiveTalent[i].PeekNext(dwID))
			{
				tagTalentLeaf* pLeaf = m_mapTalent.Peek(dwID);
				if(P_VALID(pLeaf))
				{
					pLeaf->bActive = FALSE;
					SetBidden(pLeaf);
				}
			}
		}
	}
}


VOID TalentTree::OnHaveTalentPoint()
{
	DWORD dwID;
	for(INT i=0; i<ETT_End; ++i)
	{
		if(SkillMgr::Inst()->IsInsideOfMaxTalent((ETalentType)i))
		{
			m_listActiveTalent[i].ResetIterator();
			while(m_listActiveTalent[i].PeekNext(dwID))
			{
				tagTalentLeaf* pLeaf = m_mapTalent.Peek(dwID);
				if(P_VALID(pLeaf))
				{
					pLeaf->bActive = TRUE;
					SetBidden(pLeaf);
				}
			}
		}
	}
}

VOID TalentTree::ReflashActiveTalentList( tagTalentLeaf* pLeaf )
{
	BOOL bRet = m_listActiveTalent[pLeaf->eTalentType].IsExist(pLeaf->dwTalentLeaf);
	if(bRet)
	{
		if(!pLeaf->bActive)
		{
			m_listActiveTalent[pLeaf->eTalentType].Erase(pLeaf->dwTalentLeaf);
		}
	}
	else
	{
		if(pLeaf->bActive)
		{
			m_listActiveTalent[pLeaf->eTalentType].PushBack(pLeaf->dwTalentLeaf);
		}
	}
}

DWORD TalentTree::GetNextLevelTalentTypeID( tagTalentLeaf* pLeaf, DWORD dwSkillType)
{
	if(pLeaf->nCurLevel < pLeaf->nMaxLevel)
	{
		return MTransSkillTypeID(dwSkillType, pLeaf->nCurLevel + 1);
	}
	return GT_INVALID;
}

VOID TalentTree::ReflashTalentTips( tagTalentLeaf* pLeaf )
{
	//�����������ʼ��ܰ�ť�ϣ���������ʾTips
	if( P_VALID(pLeaf) && P_VALID(pLeaf->pBtnTalentLeaf) && pLeaf->pBtnTalentLeaf == TObjRef<GUISystem>()->GetMousePoint() )
	{
		//�����ʾģʽ�Ǿ���ģʽ��������������������ĸ����ܻ�ûѧ�ᣬ����ʾtips
		if( m_bSimpleDisplayMode == TRUE && pLeaf->dwIconCtrlId != 0 && !SkillMgr::Inst()->IsSkillLearned( pLeaf->dwIconCtrlId ) )
			return;

		tstring strTips;
		if( 0 == pLeaf->nCurLevel )
		{
			strTips = ToolTipCreator::Inst()->GetTalentTips( MTransSkillTypeID(pLeaf->dwTalentLeaf, 1) );
		}
		else
		{
			strTips = ToolTipCreator::Inst()->GetTalentTips( MTransSkillTypeID(pLeaf->dwTalentLeaf, pLeaf->nCurLevel) );
		}
		ShowTip( pLeaf->pBtnTalentLeaf, strTips.c_str() );	
	}
}

VOID TalentTree::OnClearAllTalent()
{
	tagTalentLeaf* pLeaf = NULL;
	m_mapTalent.ResetIterator();
	while (m_mapTalent.PeekNext(pLeaf))
	{
		//--ˢ�µ�ǰ��Ͷ����
		pLeaf->bActive = IsTalentLeafActive(pLeaf);
		ReflashActiveTalentList(pLeaf);
		SetBidden(pLeaf);
	}
}

VOID TalentTree::Refresh4DisplayMode()
{
	tagTalentLeaf* pLeaf = NULL;
	TCHAR szTmp[X_LONG_NAME] = {0};
	
	tstring str = _T("data\\ui\\state\\L_status-image-none.bmp");

	m_mapTalent.ResetIterator();
	while( m_mapTalent.PeekNext( pLeaf ) )
	{
		if( !P_VALID(pLeaf) )
			continue;

		if( TRUE == m_bSimpleDisplayMode )
		{
			if( 0 != pLeaf->dwIconCtrlId && !SkillMgr::Inst()->IsSkillLearned( pLeaf->dwIconCtrlId) )
			{
				pLeaf->pBtnTalentLeaf->SetIcon( str.c_str() );
			}
		}
		else
		{
			ZeroMemory( szTmp, sizeof(szTmp) );
			_stprintf( szTmp, _T("data\\ui\\Icon\\%s.tga"), pLeaf->szIconPath );
			pLeaf->pBtnTalentLeaf->SetIcon( szTmp );
		}
	}
}

VOID TalentTree::RefreshTalentIcon( DWORD dwLearnedSkillTypeID )
{
	if( TRUE == m_bSimpleDisplayMode )
	{
		tagTalentLeaf* pLeaf = NULL;

		m_mapTalent.ResetIterator();
		while( m_mapTalent.PeekNext( pLeaf ) )
		{
			if( !P_VALID(pLeaf) )
				continue;

			if( dwLearnedSkillTypeID == pLeaf->dwIconCtrlId )
			{
				TCHAR szTmp[X_LONG_NAME] = {0};
				_stprintf( szTmp, _T("data\\ui\\Icon\\%s.tga"), pLeaf->szIconPath );
				pLeaf->pBtnTalentLeaf->SetIcon( szTmp );
			}
		}
	}
}
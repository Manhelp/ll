#include "StdAfx.h"
#include "PocketFrame.h"
#include "ItemFrame.h"
#include "DragBtn.h"
#include "ItemMgr.h"
#include "Item.h"
#include "Container.h"
#include "ItemButton.h"
#include "ItemMenuButton.h"
#include "ToolTipCreator.h"
#include "ItemEvent.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CursorMgr.h"
#include "CurrencyMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "QuestMgr.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\currency_define.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\shop_define.h"
#include "StallMgr.h"
#include "CombatSysUtil.h"
#include "PetDef.h"
#include "PetManager.h"
#include "StorageFrame.h"
#include "EasyBuyFrame.h"
#include "ClothespressFrame.h"
#include "..\WorldDefine\msg_func_npc.h"
#include "..\WorldDefine\test.h"
#include "HolyManMgr.h"
#include "WarMgr.h"
#include "RoleEvent.h"

const INT MAX_COL = 8;
const INT MAX_ROW = 5;
const SIZE UINT_SIZE = {36,36};
const SIZE SPACE_SIZE = {8,8};

PocketFrame::PocketFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndCaption = NULL;
	m_pBtnThorne = NULL;
	m_pBtnSettle = NULL;
	m_pBtnEncrypt = NULL;
	m_pBtnExit = NULL;
	m_pBtnStall = NULL;

	for(INT i = 0; i < MAX_BAG_NUM; ++i)
	{
		m_pPbnPocketIndex[i] = NULL;
	}

	m_pStcTreasure = NULL;
	m_pStcGold = NULL;
	m_pStcSilver = NULL;

	m_pSliptItem = NULL;
	
	m_n16CurPage = 0;

	m_dwNewPsdCrc	= GT_INVALID;
	m_dwOldPsdCrc	= GT_INVALID;
	m_pBtnClothespress = NULL;
}

PocketFrame::~PocketFrame()
{

}

BOOL PocketFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnUpdateLPItem));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2Bag"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnItemPosChangeEvent));
	m_pFrameMgr->RegisterEventHandle(_T("Bag_Silver"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnBagSilverChange));
	m_pFrameMgr->RegisterEventHandle(_T("Bag_Yuanbao"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnBagYuanbaoChange));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_ItemNumber"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnInputItemNumber));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_EquipChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnUpdateEQDataChange));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateItemCDEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnUpdateItemCD));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_BagExtend"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventBagExtend));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_SetPsdFirst"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventSetPsdFirst));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_SetPsdSecond"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventSetPsdSecond));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_ModPsdFirst"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventModPsdFirst));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_ModPsdSecond"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventModPsdSecond));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_ModPsdThird"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventModPsdThird));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_DelPsdFirst"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventDelPsdFirst));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_OpenBagPsd"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventOpenBagPsd));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Encrypt"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnOpen_Encrypt));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_PocketExtern_Check"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnMsgBox_PocketExtern_Check));
	m_pFrameMgr->RegisterEventHandle(_T("EnterWar"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventEnterWar));
	m_pFrameMgr->RegisterEventHandle(_T("EnterNormalMap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEnterNormalMap));
	m_pFrameMgr->RegisterEventHandle(_T("RefreshWarInfo_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::On_RefreshWarInfo));
	//m_pCmdMgr->Register("NS_RequestAttackCode",			(NETMSGPROC)m_Trunk.sfp2(&PocketFrame::OnNS_RequestAttackCode),	_T("NS_RequestAttackCode"));
	
	return bRet;
}

BOOL PocketFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	if(P_VALID(m_pWndWarBag))
	{
		m_pGUI->AddToDestroyList(m_pWndWarBag);
		m_pWndWarBag = NULL;
	}

	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnUpdateLPItem));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Bag"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnItemPosChangeEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("Bag_Silver"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnBagSilverChange));
	m_pFrameMgr->UnRegisterEventHandler(_T("Bag_Yuanbao"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnBagYuanbaoChange));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_ItemNumber"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnInputItemNumber));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_EquipChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnUpdateEQDataChange));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateItemCDEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnUpdateItemCD));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_BagExtend"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventBagExtend));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_SetPsdFirst"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventSetPsdFirst));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_SetPsdSecond"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventSetPsdSecond));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_ModPsdFirst"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventModPsdFirst));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_ModPsdSecond"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventModPsdSecond));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_ModPsdThird"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventModPsdThird));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_DelPsdFirst"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventDelPsdFirst));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_OpenBagPsd"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventOpenBagPsd));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Encrypt"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnOpen_Encrypt));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_PocketExtern_Check"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnMsgBox_PocketExtern_Check));
	m_pFrameMgr->UnRegisterEventHandler(_T("EnterWar"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEventEnterWar));
	m_pFrameMgr->UnRegisterEventHandler(_T("EnterNormalMap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::OnEnterNormalMap));
	m_pFrameMgr->UnRegisterEventHandler(_T("RefreshWarInfo_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PocketFrame::On_RefreshWarInfo));
	//m_pCmdMgr->UnRegister("NS_RequestAttackCode",			(NETMSGPROC)m_Trunk.sfp2(&PocketFrame::OnNS_RequestAttackCode));
	
	DestroyMsgBox();

	return bRet;
}

BOOL PocketFrame::ReloadUI()
{
	// ����
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\bag.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	
	INT i,j;
	for(i = 0; i < MAX_BAG_NUM + 1; ++i)
	{
		CHAR szTmp[X_LONG_NAME] = {0};
		_snprintf(szTmp, sizeof(szTmp), "bagback\\bagpic\\bagchoose\\bagchoose%d" ,i+1);
		m_pGUI->ChangeXml(&ele, szTmp, "ClassName", "ItemMenuButton");
	}

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PocketFrame::EventHandler));

	m_pWndCaption = m_pWnd->GetChild(_T("bagpic"));
	
	m_pBtnThorne = (GUIButton*)m_pWndCaption->GetChild(_T("boxbutt"));
	m_pBtnSettle = (GUIButton*)m_pWndCaption->GetChild(_T("settlebutt"));
	m_pBtnEncrypt = (GUIButton*)m_pWndCaption->GetChild(_T("encbutt"));
	m_pBtnExit = (GUIButton*)m_pWndCaption->GetChild(_T("closebutton"));
	m_pBtnStall = (GUIButton*)m_pWndCaption->GetChild(_T("button"));
	m_pBtnBagEx = (GUIButton*)m_pWndCaption->GetChild(_T("extension"));
	m_pBtnClothespress = (GUIButton*)m_pWndCaption->GetChild(_T("clothbutt"));
	
	for(i = 0; i < MAX_BAG_NUM + 1; ++i)
	{
		TCHAR szTmp[X_SHORT_NAME] = {0};
		_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("bagchoose\\bagchoose%d") ,i+1);
		m_pPbnPocketIndex[i] = (ItemMenuButton*)m_pWndCaption->GetChild(szTmp);
	}
	
	m_pStcTreasure = (GUIButton*)m_pWndCaption->GetChild(_T("yuanbaoback\\yuanum"));
	m_pStcGold = (GUIButton*)m_pWndCaption->GetChild(_T("moneyback\\goldnum"));
	m_pStcSilver = (GUIButton*)m_pWndCaption->GetChild(_T("moneyback\\silvernum"));

	m_pStcPocket = (GUIStatic*)m_pWndCaption->GetChild(_T("bagpic1\\bags"));
	// ������Ԫ��
	for(i = 0; i < MAX_COL; ++i)
	{
		for (j = 0; j < MAX_ROW; ++j)
		{
			XmlElement ele;
			CHAR szTemp[X_SHORT_NAME]={0};
			_snprintf(szTemp, sizeof(szTemp), "unititem%d", MAX_COL * j + i);
			ele.SetAttribute("Name", szTemp);
			ele.SetAttribute("ClassName", "ItemButton");
			ele.SetAttribute("Size_x", UINT_SIZE.cx);
			ele.SetAttribute("Size_y", UINT_SIZE.cy);
			ele.SetAttribute("LogicPos_x", 9 + SPACE_SIZE.cx*i + UINT_SIZE.cx*i);
			ele.SetAttribute("LogicPos_y", 9 + SPACE_SIZE.cy*j + UINT_SIZE.cy*j);
			ele.SetAttribute("Font", m_pUtil->UnicodeToUnicode8(g_StrTable[_T("Font_MSYH")]));
			/*ele.SetAttribute("FontWidth", 8);
			ele.SetAttribute("FontHeight", 16);*/
			ele.SetAttribute("TextRect_left", 0);
			ele.SetAttribute("TextRect_top", 0);
			ele.SetAttribute("TextRect_right", UINT_SIZE.cx);
			ele.SetAttribute("TextRect_bottom", UINT_SIZE.cy);
			ele.SetAttribute("TextColor", 0xFFFFFFFF);
			ele.SetAttribute("TextAlign", "RightBottom");
			m_pUnitItem[MAX_COL * j + i] = (ItemButton*)m_pGUI->CreateWnd(m_pStcPocket->GetFullName().c_str(), &ele);
		}
	}
	// �������ҳ��
	//UpdateMaxPage();
	// ������ǰҳ
	ShowCurPage(m_n16CurPage);
	m_pPbnPocketIndex[m_n16CurPage]->SetState(EGUIBS_PushDown, FALSE);
	/*m_pBtnEncrypt->SetEnable(false);
	m_pBtnSettle->SetEnable(false);*/
	
	m_pWnd->SetInvisible(TRUE);

	//ս������
	XmlElement ele1;
	tstring strPath1 = g_strLocalPath + _T("\\ui\\sp_props.xml");
	m_pGUI->LoadXml(&ele1, "VFS_System", strPath1.c_str());

	for(int i = 0; i < SPACE_WAR_BAG; i++)
	{
		CHAR szTmp[X_LONG_NAME] = {0};
		_snprintf(szTmp, sizeof(szTmp), "sp_props\\sp_props\\sp\\back\\item%d" ,i);
		m_pGUI->ChangeXml(&ele1, szTmp, "ClassName", "ItemButton");
	}

	m_pWndWarBag = m_pGUI->CreateWnd(_T("\\desktop"), &ele1);
	TCHAR szTemp[128] = {0};
	for(int i = 0; i < SPACE_WAR_BAG; i++)
	{
		_stprintf(szTemp, _T("sp_props\\sp\\back\\item%d"), i);
		m_pIBtnWarItem[i] = (ItemButton*)m_pWndWarBag->GetChild(szTemp);
		m_pIBtnWarItem[i]->SetInvisible(TRUE);
	}
	m_pStcWarMoeny = (GUIStatic*)m_pWndWarBag->GetChild(_T("sp_props\\goldnum"));

	// ע���¼�������
	m_pGUI->RegisterEventHandler(m_pWndWarBag->GetFullName().c_str(), m_Trunk.sfp1(&PocketFrame::EventHandlerWar));


	return TRUE;
}

DWORD PocketFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnExit)//�˳�
			{
				m_pMgr->SendEvent(&tagGameEvent(_T("Close_Package_UI"), this));
			} 
			else if (pWnd == m_pBtnStall)//��̯
			{
				m_pMgr->SendEvent(&tagGameEvent(_T("StallEvent_OpenWindow"), this));
			}
			else if (pWnd == m_pBtnEncrypt)//����
			{
				GameFrame* pFrame = m_pFrameMgr->GetFrame(_T("PocketEncrypt"));
				if(!P_VALID(pFrame))
				{
					pFrame = m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PocketEncrypt"), _T("PocketEncryptFrame"), 0);
				}
				else
				{
					m_pFrameMgr->AddToDestroyList(pFrame);
				}
			}
			else if (pWnd == m_pBtnThorne)//�����ٱ���
			{
				GameFrame* pFrame = m_pFrameMgr->GetFrame(_T("Throne"));
				if(!P_VALID(pFrame))
				{
					pFrame = m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Throne"), _T("ThroneFrame"), 0);
				}
				else
				{
					m_pFrameMgr->AddToDestroyList(pFrame);
				}
				
			}
			else if (pWnd == m_pBtnClothespress)//�����ٱ���
			{
				ClothespressFrame* pFrame = (ClothespressFrame*)m_pFrameMgr->GetFrame(_T("Clothespress"));
				if(!P_VALID(pFrame))
					pFrame = (ClothespressFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("Clothespress"), _T("ClothespressFrame"), 0);
				if(P_VALID(pFrame))
					pFrame->ShowUI();
			}
			else if (pWnd == m_pBtnSettle)//������
			{
				ItemContainer* pItemCon = ItemMgr::Inst()->GetPocket();
				if(P_VALID(pItemCon))
				{
					pItemCon->AutoSettle();
				}
			}
			else if (pWnd == m_pBtnBagEx)//���䱳��
			{
				if(P_VALID(GetObj("PocketExtern_Check")))
					KillObj("PocketExtern_Check");

				CreateObj("PocketExtern_Check", "MsgBox");
				TObjRef<MsgBox>("PocketExtern_Check")->Init(_T(""), g_StrTable[_T("PocketExtern_Check")], _T("MsgBox_PocketExtern_Check"));

			}
			else if (_tcsnccmp(pWnd->GetName().c_str(), _T("unititem"), 8) == 0)//ʹ�ñ����е���Ʒ
			{
				for (INT i = 0; i < SPACE_ONE_BAG; ++i)
				{
					if (pWnd == (GUIWnd*)m_pUnitItem[i])
					{
						TryToUseItem((INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
						ShowTip(NULL, NULL);
						break;
					}
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("PocketEasyBuyFrame"));
			if (MAX_BAG_NUM == m_n16CurPage)//�����������϶�
			{
				if( P_VALID(pFrame) )
					pFrame->Align();
				return 0;
			}

			bool bDrag = true;
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("unititem"), 8) == 0)//�϶������е���Ʒ
			{
				for (INT i = 0; i < SPACE_ONE_BAG; ++i)
				{
					if (pWnd == (GUIWnd*)m_pUnitItem[i])
					{
						TryToDragItem(m_n16CurPage*SPACE_ONE_BAG + i, (GetKeyState(VK_SHIFT)&0x80)!=0);
						bDrag = false;
						break;
					}
				}
			}

			if( P_VALID(pFrame) && bDrag )
			{
				pFrame->Align();
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pEvent->dwParam1 != 1)//���ǰ��²�����
				return 0;
			
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("bagchoose"), 9) == 0)//������
			{
				for (INT i = 0; i < MAX_BAG_NUM + 1; ++i)
				{
					if(pWnd == m_pPbnPocketIndex[i])
					{
						ShowCurPage(i);
						return 0;
					}
				}
			}
		}
		break;
	case EGUISCB_Drop:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("unititem"), 8) == 0)
			{
				for(INT i = 0; i < SPACE_ONE_BAG; ++i)
				{
					if(pWnd == (GUIWnd*)m_pUnitItem[i])
					{
						// ���жϸ�λ�����Ƿ�����Ʒ���������Ҫ����Ʒ�Ƿ���Ա�����
						// ���Ŀ��λ���ϵ���Ʒ���ܲ���������������Ʒ
						Item* pItem = GetItem(i);
						if (P_VALID(pItem) && !pItem->IsOperable())
							return 0;

						if( EDT_Item == pEvent->dwParam1 )
						{
							OnItemChange((INT16)LOWORD(pEvent->dwParam2), (INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
						}
						else if( EDT_ItemEquip == pEvent->dwParam1 )
						{
							OnUnEquipment((EEquipPos)LOWORD(pEvent->dwParam2), (INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
						}
						else if( EDT_ItemWare == pEvent->dwParam1)
						{
							OnItemChangeEx(EICT_RoleWare, (INT16)LOWORD(pEvent->dwParam2), (INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
						}
						else if( EDT_ItemSplit == pEvent->dwParam1)
						{
							OnItemChange((INT16)LOWORD(pEvent->dwParam2), (INT16)(m_n16CurPage*SPACE_ONE_BAG + i), (INT16)HIWORD(pEvent->dwParam2));
							//�������ȡ���϶�
							DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
							ASSERT(P_VALID(pDrag));
							pDrag->EndDrag();
						}
						else if( EDT_ItemShop == pEvent->dwParam1)
						{
							OnBuyItem((INT)LOWORD(pEvent->dwParam2), pEvent->dwParam3, (INT16)HIWORD(pEvent->dwParam2));
						}
                        else if (EDT_PetEquip == pEvent->dwParam1)
                        {
                            OnPetUnEquipment(pEvent->dwParam2, (INT8)pEvent->dwParam3, (INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
                        }
                        else if( EDT_GuildWare == pEvent->dwParam1)
                        {
                            OnItemChangeEx(EICT_GuildWare, (INT16)LOWORD(pEvent->dwParam2), (INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
                        }
						else if ( EDT_HolyEquip == pEvent->dwParam1 )
						{
							HolyManMgr::Inst()->SendMsg_UnHolyEquip( EHolyEquipPos(pEvent->dwParam2) );
						}
						break;
					}
				}
			}
			else if(_tcsnccmp(pWnd->GetName().c_str(), _T("bagchoose"), 9) == 0)
			{
				for (INT i = 0; i < MAX_BAG_NUM; ++i)
				{
					if(pWnd == m_pPbnPocketIndex[i])
					{
						INT16 n16PosDst = ItemMgr::Inst()->GetConBlank(EICT_Bag, i);
						if(!GT_VALID(n16PosDst))
							break;

						if( EDT_Item == pEvent->dwParam1 )
						{
							OnItemChange((INT16)LOWORD(pEvent->dwParam2), n16PosDst);
						}
						else if( EDT_ItemEquip == pEvent->dwParam1 )
						{
							OnUnEquipment((EEquipPos)LOWORD(pEvent->dwParam2), n16PosDst);
						}
						else if( EDT_ItemWare == pEvent->dwParam1)
						{
							OnItemChangeEx(EICT_RoleWare, (INT16)LOWORD(pEvent->dwParam2), n16PosDst);
						}
						else if( EDT_ItemSplit == pEvent->dwParam1)
						{
							OnItemChange((INT16)LOWORD(pEvent->dwParam2), n16PosDst, (INT16)HIWORD(pEvent->dwParam2));
						}
						break;
					}
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("unititem"), 8) == 0)
			{
				for(INT i = 0; i < SPACE_ONE_BAG; ++i)
				{
					if(pWnd == (GUIWnd*)m_pUnitItem[i])
					{
						ShowItemTip(m_pUnitItem[i], (INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
						break;
					}
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUISCB_UseSpecFunc:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("unititem"), 8) == 0)
			{
				for(INT i = 0; i < SPACE_ONE_BAG; ++i)
				{
					if(pWnd == (GUIWnd*)m_pUnitItem[i])
					{
						Item* pItemDst = GetItem((INT16)(m_n16CurPage*SPACE_ONE_BAG + i));
						if(P_VALID(pItemDst))
						{
							// �ж��Ƿ񷢳�����Դ�����Ƿ�����Ʒ
							if(pEvent->dwParam1 > ECAN_ItemStart)
							{
								// ������Ʒ�������
								Item* pItemSrc = ItemMgr::Inst()->GetPocketItem((INT16)pEvent->dwParam2);
								if(P_VALID(pItemSrc))
								{
									tagOnTheRoleOfItemEvent event(_T("ItemEvent_TargetItem"), this);
									event.eTypeSrc = pItemSrc->GetConType();
									event.n16IndexSrc = pItemSrc->GetPos();
									event.dwTypeIDSrc = pItemSrc->GetItemTypeID();
									event.eSpecFuncSrc = pItemSrc->GetItemSpecFunc();
									event.eTypeDst = pItemDst->GetConType();
									event.n16IndexDst = pItemDst->GetPos();
									event.dwTypeIDDst = pItemDst->GetItemTypeID();
									m_pFrameMgr->SendEvent(&event);
								}
							}
							else
							{
								// ���������������
							}
						}
						break;
					}
				}
			}
		}
	default :
		break;
	}
	return 0;
}

DWORD PocketFrame::EventHandlerWar(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)//ʹ��ս�������е���Ʒ
			{
				for (INT i = 0; i < SPACE_WAR_BAG; ++i)
				{
					if (pWnd == (GUIWnd*)m_pIBtnWarItem[i])
					{
						TryToUseWarItem((INT16)(i));
						ShowTip(NULL, NULL);
						break;
					}
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			bool bDrag = true;
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)//�϶�ս�������е���Ʒ
			{
				for (INT i = 0; i < SPACE_WAR_BAG; ++i)
				{
					if (pWnd == (GUIWnd*)m_pIBtnWarItem[i])
					{
						TryToDragWarItem(i, (GetKeyState(VK_SHIFT)&0x80)!=0);
						bDrag = false;
						break;
					}
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for(INT i = 0; i < SPACE_WAR_BAG; ++i)
				{
					if(pWnd == (GUIWnd*)m_pIBtnWarItem[i])
					{
						ShowWarItemTip(m_pIBtnWarItem[i], (INT16)(i));
						break;
					}
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUISCB_Drop:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for(INT i = 0; i < SPACE_WAR_BAG; ++i)
				{
					if(pWnd == (GUIWnd*)m_pIBtnWarItem[i])
					{
						// ���жϸ�λ�����Ƿ�����Ʒ���������Ҫ����Ʒ�Ƿ���Ա�����
						// ���Ŀ��λ���ϵ���Ʒ���ܲ���������������Ʒ
						Item* pItem = GetWarItem(i);
						if (P_VALID(pItem) && !pItem->IsOperable())
							return 0;

						if( EDT_WarBag == pEvent->dwParam1 )
						{
							OnWarItemChange((INT16)LOWORD(pEvent->dwParam2), (INT16)( i));
						}
						break;
					}
				}
			}
			
		}
		break;
	default :
		break;
	}
	return 0;
}

VOID PocketFrame::SetMoneyValue(INT64 n64Value)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), n64Value/GOLD2SILVER);
	m_pStcGold->SetText(szTmp);
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), n64Value%GOLD2SILVER);
	m_pStcSilver->SetText(szTmp);

	m_pStcGold->SetRefresh(TRUE);
	m_pStcSilver->SetRefresh(TRUE);
}

VOID PocketFrame::SetTreasureValue(INT nValue)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp), _T("%d"), nValue);
	m_pStcTreasure->SetText(szTmp);

	m_pStcTreasure->SetRefresh(TRUE);
}

VOID PocketFrame::ShowCurPage(INT16 page)
{
	m_n16CurPage = page;
	for (INT i=0; i<SPACE_ONE_BAG; ++i)
	{
		RefreshItem(page * SPACE_ONE_BAG + i);
	}
}

Item* PocketFrame::GetItem(INT16 index)
{
	if (m_n16CurPage == MAX_BAG_NUM)
	{
		return ItemMgr::Inst()->GetQuest()->GetValue(index%SPACE_QUEST_BAG);
	}
	return ItemMgr::Inst()->GetPocket()->GetValue(index);
}

Item* PocketFrame::GetWarItem(INT16 index)
{
	return ItemMgr::Inst()->GetWarBag()->GetValue(index);
}

VOID PocketFrame::UpdateItem(INT16 index)
{
	if (IsCurPage(index))//���Ϊ��ǰҳ����£����ǵ�ǰҳ�򲻸���
	{
		RefreshItem(index);
	}
}

VOID PocketFrame::RefreshItem(INT16 index)
{
	DWORD dwTypeID = GT_INVALID;
	INT16 n16Quantity = 0;
	BYTE byQuality = 0;
	

	INT nMaxSize = ItemMgr::Inst()->GetPocket()->MaxSize();
	if(index >= nMaxSize && index < SPACE_ALL_BAG)
	{
		dwTypeID = 0;
	}
	else
	{
		Item* pItem = GetItem(index);
		if (P_VALID(pItem))
		{
			//��ȡ��ƷTypeID
			dwTypeID = pItem->GetItemTypeID();
			//��ȡ��Ʒ����
			n16Quantity = pItem->GetItemQuantity();
			//��ȡ��ƷƷ��
			byQuality = pItem->GetItemQuality();
		}
	}
	
	INT i = index%SPACE_ONE_BAG;
	m_pUnitItem[i]->RefreshItem(dwTypeID, n16Quantity, byQuality);
	if(m_pGUI->GetMousePoint() == m_pUnitItem[i])
	{
		ShowItemTip(m_pUnitItem[i], index);
	}
}

VOID PocketFrame::UpdateMaxPage()
{
	// ������󱳰���С�͵���������С�����󱳰�����
	INT nMaxSize = ItemMgr::Inst()->GetPocket()->MaxSize();
	INT nMaxPage = nMaxSize/SPACE_ONE_BAG;
	
	// �������ñ����л�״̬
	for(INT i=0; i<MAX_BAG_NUM; ++i)
	{
		m_pPbnPocketIndex[i]->SetEnable(i < nMaxPage);
	}
}

VOID PocketFrame::UpdateMoney()
{
	
}

VOID PocketFrame::OnItemChange(INT16 n16PosSrc, INT16 n16PosDst /* = GT_INVALID */, INT16 n16NumDst /* = 0 */)
{
	if(n16PosSrc == n16PosDst)
		return;

	Item* pItem = GetItem(n16PosSrc);
	if(P_VALID(pItem))
	{
		tagNC_ItemPosChange m;
		m.eConType = EICT_Bag;
		m.n16Num = n16NumDst;
		m.n16PosDst = n16PosDst;
		m.n64Serial = pItem->GetItemId();
		m_pSession->Send(&m);
	}
}

VOID PocketFrame::OnWarItemChange(INT16 n16PosSrc, INT16 n16PosDst /*= GT_INVALID*/, INT16 n16NumDst/* = 0*/)
{
	if(n16PosSrc == n16PosDst)
		return;

	Item* pItem = GetWarItem(n16PosSrc);
	if(P_VALID(pItem))
	{
		tagNC_ItemPosChange m;
		m.eConType = EICT_WarBag;
		m.n16Num = n16NumDst;
		m.n16PosDst = n16PosDst;
		m.n64Serial = pItem->GetItemId();
		m_pSession->Send(&m);
	}
}

VOID PocketFrame::OnItemChangeEx( EItemConType ePosSrc, INT16 n16PosSrc, INT16 n16PosDst /*= GT_INVALID*/ )
{
	Item* pItem = ItemMgr::Inst()->GetConItem(ePosSrc, n16PosSrc);
	if(P_VALID(pItem))
	{
		tagNC_ItemPosChangeEx m;
		m.eConTypeDst = EICT_Bag;
		m.eConTypeSrc = ePosSrc;
		m.n16PosDst = n16PosDst;
		m.n64Serial1 = pItem->GetItemId();
		
		StorageFrame* pFrame = (StorageFrame*)(m_pFrameMgr->GetFrame( _T("Storage") ));
		if( P_VALID(pFrame) && pFrame->GetWalkWareMode() )
		{
			m.dwNPCID = GT_INVALID;
		}
		else if (ePosSrc == EICT_RoleWare || ePosSrc == EICT_GuildWare)
		{
			m.dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
		}
		m_pSession->Send(&m);
	}
}


VOID PocketFrame::OnBuyItem( INT nShelfPos, DWORD dwTypeID, INT16 n16Num /*= 1*/ )
{
	tagBuyItemEvent event(_T("ItemEvent_Buy"), this);
	event.n16Pos = (INT16)nShelfPos;
	event.n16Num = n16Num;
	event.dwTypeID = dwTypeID;
	m_pFrameMgr->SendEvent(&event);
}

VOID PocketFrame::OnUnEquipment(EEquipPos ePos, INT16 n16PosDst /* = GT_INVALID */)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID(pLp) )
		return;

	//�����������ڱ�ս״̬��ʹ�ü��ܲ�����װ
	if( pLp->IsSkillState() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatEquip")]);
		return;
	}
	else if( pLp->IsReadyState() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_ReadyEquip")]);
		return;
	}

	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
	if(P_VALID(pEquip))
	{
		tagNC_Unequip m;
		m.eType     = ePos;
		m.n16PosDst = n16PosDst;
		m.n64Serial = pEquip->GetItemId();
		m_pSession->Send(&m);
	}
}

DWORD PocketFrame::OnUpdateLPItem( tagUpdateLPItemEvent* pGameEvent )
{
	if(EICT_Bag == pGameEvent->eType)
	{
		//����ǵ�ǰҳ�����
		if(IsCurPage(pGameEvent->n16Index))
		{
			RefreshItem(pGameEvent->n16Index);
		}
	}
	else if(EICT_Quest == pGameEvent->eType)
	{
		if(m_n16CurPage == MAX_BAG_NUM)
		{
			RefreshItem(pGameEvent->n16Index);
		}
	}
	else if(EICT_WarBag == pGameEvent->eType)
	{
		RefreshWarItem(pGameEvent->n16Index);
	}
	return 0;
}

DWORD PocketFrame::OnItemPosChangeEvent( tagItemPosChangeExEvent* pGameEvent )
{
	if(EICT_Bag == pGameEvent->eTypeDst || EICT_Quest == pGameEvent->eTypeDst)
	{
		if(EICT_Equip == pGameEvent->eTypeSrc)
		{
			OnUnEquipment((EEquipPos)pGameEvent->n16IndexSrc);
		}
		else 
		{
			OnItemChangeEx(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
		}
	}
	return 0;
}


DWORD PocketFrame::OnInputItemNumber( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult)
	{
		Item* pItem = m_pSliptItem;
		if(P_VALID(pItem)&& MBIT_Number == pGameEvent->eInputType)
		{
			INT16 n16Num = (INT16)pGameEvent->nInputNum;
			//�������Ϸ�������������
			if(n16Num < 0 || n16Num > pItem->GetItemQuantity())
			{
				n16Num = pItem->GetItemQuantity();
			}
			OnItemDrag(pItem, n16Num, true);
		}
	}
	m_pSliptItem = NULL;
	return 0;
}


DWORD PocketFrame::OnUpdateEQDataChange( tagUpdateLPItemEvent* pGameEvent )
{
	if(EICT_Bag == pGameEvent->eType && IsCurPage(pGameEvent->n16Index))
	{
		//����ǵ�ǰҳ�е���Ʒ���������ƷTips
		Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16Index);
		if(P_VALID(pItem))
		{
			INT i = pGameEvent->n16Index%SPACE_ONE_BAG;
			ReflashItemTips(pItem, m_pUnitItem[i]);
			m_pUnitItem[i]->SetQuality(pItem->GetItemQuality());
		}
	}
	return 0;
}

VOID PocketFrame::ShowWnd( BOOL bShow )
{
	m_pWnd->SetInvisible(!bShow);
	if(bShow)
	{
		m_pWnd->SetRefresh();
		m_pWnd->FlipToTop();
	}
	else
	{
		DestroyChild();
		DestroyMsgBox();
	}
}

VOID PocketFrame::TryToUseItem( INT16 index )
{
	Item* pItem = GetItem(index);
	if(P_VALID(pItem))
	{
		if(!pItem->IsOperable())
			return;

		tagClickItemEvent event(_T("tagClickItemEvent"),this);
		event.eType = pItem->GetConType();
		event.n16Index = pItem->GetPos();
		event.dwTypeID = pItem->GetItemTypeID();
		event.n64ItemID = pItem->GetItemId();
		m_pFrameMgr->SendEvent(&event);
	}
	
}

VOID PocketFrame::TryToUseWarItem(INT16 index)
{
	Item* pItem = GetWarItem(index);
	if(P_VALID(pItem))
	{
		if(!pItem->IsOperable())
			return;

		tagClickItemEvent event(_T("tagClickItemEvent"),this);
		event.eType = pItem->GetConType();
		event.n16Index = pItem->GetPos();
		event.dwTypeID = pItem->GetItemTypeID();
		event.n64ItemID = pItem->GetItemId();
		m_pFrameMgr->SendEvent(&event);
	}
}


VOID PocketFrame::TryToDragItem( INT16 index, bool bShift )
{
	Item* pItem = GetItem(index);
	if(P_VALID(pItem))
	{	
		if(!pItem->IsOperable())
			return;

		INT16 n16Num = pItem->GetItemQuantity();
		//����shift����Ϊ��Ʒ���
		if(bShift)
		{
			// ֻ��
			if(pItem->IsStackable(n16Num))
			{
				if(P_VALID(GetObj("ItemNumber")))
					KillObj("ItemNumber");

				CreateObj("ItemNumber", "MsgInputBox");
				TObjRef<MsgInputBox>("ItemNumber")->Init(
					_T(""), g_StrTable[_T("MsgBoxText_InputSplitNumber")], _T("MsgBox_ItemNumber"), pItem->GetItemQuantity() );
				
				m_pSliptItem = pItem;
				return;
			}
		}

		OnItemDrag(pItem, n16Num);
	}
}

VOID PocketFrame::TryToDragWarItem(INT16 index, bool bShift)
{
	Item* pItem = GetWarItem(index);
	if(P_VALID(pItem))
	{	
		if(!pItem->IsOperable())
			return;

		INT16 n16Num = pItem->GetItemQuantity();
		//����shift����Ϊ��Ʒ���
		if(bShift)
		{
			// ֻ��
			if(pItem->IsStackable(n16Num))
			{
				if(P_VALID(GetObj("ItemNumber")))
					KillObj("ItemNumber");

				CreateObj("ItemNumber", "MsgInputBox");
				TObjRef<MsgInputBox>("ItemNumber")->Init(
					_T(""), g_StrTable[_T("MsgBoxText_InputSplitNumber")], _T("MsgBox_ItemNumber"), pItem->GetItemQuantity() );

				m_pSliptItem = pItem;
				return;
			}
		}

		OnWarItemDrag(pItem, n16Num);
	}
}

DWORD PocketFrame::OnBagSilverChange( tagRoleSilverChange* pGameEvent )
{
	SetMoneyValue(pGameEvent->n64Silver);
	return 0;
}

DWORD PocketFrame::OnBagYuanbaoChange( tagRoleYuanbaoChange* pGameEvent )
{
	SetTreasureValue(pGameEvent->nYuanbao);
	return 0;
}

VOID PocketFrame::OnItemDrag( Item* pItem, INT16 n16Num, bool bSplit )
{
	DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
	if (P_VALID(pDrag))
	{
		//������Ϸ�¼�
		tagGUIEvent event(m_pUnitItem[pItem->GetPos()%SPACE_ONE_BAG], EGUIE_Drag, NULL);
		POINT pt;
		::GetCursorPos(&pt);
		event.dwParam1 = EDT_Item;
		event.dwParam2 = MAKELONG(pItem->GetPos(), n16Num);
		event.dwParam3 = pItem->GetItemTypeID();
		event.dwParam4 = MAKELONG(pt.x, pt.y);
		if(bSplit)
		{
			event.dwParam1 = EDT_ItemSplit;
		}
		pDrag->OnEvent(&event);
	}
}


VOID PocketFrame::OnWarItemDrag( Item* pItem, INT16 n16Num, bool bSplit )
{
	DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
	if (P_VALID(pDrag))
	{
		//������Ϸ�¼�
		tagGUIEvent event(m_pIBtnWarItem[pItem->GetPos()], EGUIE_Drag, NULL);
		POINT pt;
		::GetCursorPos(&pt);
		event.dwParam1 = EDT_WarBag;
		event.dwParam2 = MAKELONG(pItem->GetPos(), n16Num);
		event.dwParam3 = pItem->GetItemTypeID();
		event.dwParam4 = MAKELONG(pt.x, pt.y);
		if(bSplit)
		{
			event.dwParam1 = EDT_ItemSplit;
		}
		pDrag->OnEvent(&event);
	}
}

VOID PocketFrame::Update()
{
	float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();

	GameFrame::Update();
}

VOID PocketFrame::ReflashItemTips(Item* pItem, GUIWnd* pWnd)
{
	//�����������ʼ��ܰ�ť�ϣ���������ʾTips
	if( P_VALID(pItem) && pWnd == m_pGUI->GetMousePoint())
	{
		tstring strTips = ToolTipCreator::Inst()->GetItemTips(pItem);
		ShowTip(pWnd, strTips.c_str());
		if(MIsEquipment(pItem->GetItemTypeID()))
		{
			ToolTipCreator::Inst()->ShowEquipCompare(pItem->GetItemTypeID());
		}
	}
}

DWORD PocketFrame::OnUpdateItemCD( tagGameEvent* pGameEvent )
{
	ShowCurPage(m_n16CurPage);
	return 0;
}

DWORD PocketFrame::OnEventBagExtend( tagGameEvent* pGameEvent )
{
	ShowCurPage(m_n16CurPage);
	return 0;
}

DWORD PocketFrame::OnOpen_Encrypt(tagGameEvent* pGameEvent)
{
	GameFrame* pFrame = m_pFrameMgr->GetFrame(_T("PocketEncrypt"));
	if(!P_VALID(pFrame))
	{
		pFrame = m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("PocketEncrypt"), _T("PocketEncryptFrame"), 0);
	}
	return 0;
}

DWORD PocketFrame::OnEventSetPsdFirst( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		m_dwNewPsdCrc = m_pUtil->Crc32(pGameEvent->strInputName.c_str());

		if(P_VALID(GetObj("SetPsdSecond")))
			KillObj("SetPsdSecond");

		CreateObj("SetPsdSecond", "MsgInputBox");
		TObjRef<MsgInputBox>("SetPsdSecond")->Init(
			g_StrTable[_T("PocketEncrypt_CaptionSetup")],g_StrTable[_T("PocketEncrypt_TextNewPasswordAgain")],
			_T("MsgBox_SetPsdSecond"),GT_INVALID, MBIT_Password);
	}
	return 0;
}

DWORD PocketFrame::OnEventSetPsdSecond( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		if(m_dwNewPsdCrc == m_pUtil->Crc32(pGameEvent->strInputName.c_str()))
		{
			tagNC_SetBagPsd cmd;
			cmd.dwBagPsdCrc = m_dwNewPsdCrc;
			m_pSession->Send(&cmd);
			m_dwNewPsdCrc = GT_INVALID;
			// �����������״̬
			ItemMgr::Inst()->SetPocketEncryptState(ItemMgr::EPES_SetupPassword);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_DoublePsdDiff")]);

			if(P_VALID(GetObj("SetPsdFirst")))
				KillObj("SetPsdFirst");

			CreateObj("SetPsdFirst", "MsgInputBox");
			TObjRef<MsgInputBox>("SetPsdFirst")->Init(
				g_StrTable[_T("PocketEncrypt_CaptionSetup")],g_StrTable[_T("PocketEncrypt_TextNewPassword")],
				_T("MsgBox_SetPsdFirst"),GT_INVALID, MBIT_Password);
		}
	}
	return 0;
}

DWORD PocketFrame::OnEventModPsdFirst( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		m_dwOldPsdCrc = m_pUtil->Crc32(pGameEvent->strInputName.c_str());

		tagNC_OldBagPsd cmd;
		cmd.dwOldBagPsdCrc = m_dwOldPsdCrc;
		m_pSession->Send(&cmd);

		/*if(P_VALID(GetObj("ModPsdSecond")))
			KillObj("ModPsdSecond");

		CreateObj("ModPsdSecond", "MsgInputBox");
		TObjRef<MsgInputBox>("ModPsdSecond")->Init(
			g_StrTable[_T("PocketEncrypt_CaptionModify")],g_StrTable[_T("PocketEncrypt_TextNewPassword")],
			_T("MsgBox_ModPsdSecond"),GT_INVALID, MBIT_Password);*/
	}
	return 0;
}

DWORD PocketFrame::OnEventModPsdSecond( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		m_dwNewPsdCrc = m_pUtil->Crc32(pGameEvent->strInputName.c_str());

		if(P_VALID(GetObj("ModPsdThird")))
			KillObj("ModPsdThird");

		CreateObj("ModPsdThird", "MsgInputBox");
		TObjRef<MsgInputBox>("ModPsdThird")->Init(
			g_StrTable[_T("PocketEncrypt_CaptionModify")],g_StrTable[_T("PocketEncrypt_TextNewPasswordAgain")],
			_T("MsgBox_ModPsdThird"),GT_INVALID, MBIT_Password);
	}
	return 0;
}

DWORD PocketFrame::OnEventModPsdThird( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		if(m_dwNewPsdCrc == m_pUtil->Crc32(pGameEvent->strInputName.c_str()))
		{
			tagNC_ResetBagPsd cmd;
			cmd.dwNewBagPsdCrc = m_dwNewPsdCrc;
			cmd.dwOldBagPsdCrc = m_dwOldPsdCrc;
			m_pSession->Send(&cmd);

			m_dwNewPsdCrc = GT_INVALID;
			m_dwOldPsdCrc = GT_INVALID;
			// �����������״̬
			ItemMgr::Inst()->SetPocketEncryptState(ItemMgr::EPES_ModifyPassword);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_DoublePsdDiff")]);

			if(P_VALID(GetObj("ModPsdSecond")))
				KillObj("ModPsdSecond");

			CreateObj("ModPsdSecond", "MsgInputBox");
			TObjRef<MsgInputBox>("ModPsdSecond")->Init(
				g_StrTable[_T("PocketEncrypt_CaptionModify")],g_StrTable[_T("PocketEncrypt_TextNewPassword")],
				_T("MsgBox_ModPsdSecond"),GT_INVALID, MBIT_Password);
		}
	}
	return 0;
}

DWORD PocketFrame::OnEventDelPsdFirst( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		tagNC_UnsetBagPsd cmd;
		cmd.dwSafeCodeCrc = m_pUtil->Crc32(pGameEvent->strInputName.c_str());
		m_pSession->Send(&cmd);

		// �����������״̬
		ItemMgr::Inst()->SetPocketEncryptState(ItemMgr::EPES_CancelPassword);
	}
	return 0;
}

DWORD PocketFrame::OnEventOpenBagPsd( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Password == pGameEvent->eInputType)
	{
		tagNC_OpenBagPsd cmd;
		cmd.dwBagPsdCrc = m_pUtil->Crc32(pGameEvent->strInputName.c_str());
		m_pSession->Send(&cmd);

		// �����������״̬
		ItemMgr::Inst()->SetPocketEncryptState(ItemMgr::EPES_EnterPassword);
	}
	return 0;
}

DWORD PocketFrame::OnMsgBox_PocketExtern_Check(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult )
	{
		ItemContainer* pItemCon = ItemMgr::Inst()->GetPocket();
		if(P_VALID(pItemCon))
		{
			Item* pItem = pItemCon->GetSpecFuncItem( EICT_Bag, EISF_BagExtend, true );
			if( !P_VALID(pItem) )
				pItem = pItemCon->GetSpecFuncItem( EICT_Bag, EISF_BagExtend, false );
			if (P_VALID(pItem))
			{
				tagNC_BagExtend m;
				m.n64Serial = pItem->GetItemId();
				m.dwNPCID = GT_INVALID;
				m_pSession->Send( &m );
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotHave_ExternedBag")]);
			}
		}
	}
	return 0;
}

BOOL PocketFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;

	m_pMgr->SendEvent(&tagGameEvent(_T("Close_Package_UI"), this));
	return TRUE;
}

VOID PocketFrame::DestroyChild()
{
	// ����������ϵͳ
	m_listChild.ResetIterator();
	GameFrame* pFrame = NULL;
	while(m_listChild.PeekNext(pFrame))
	{
		m_pMgr->AddToDestroyList(pFrame);
	}
}

VOID PocketFrame::DestroyMsgBox()
{
	if(P_VALID(GetObj("ItemNumber")))
		KillObj("ItemNumber");

	if(P_VALID(GetObj("SetPsdFirst")))
		KillObj("SetPsdFirst");

	if(P_VALID(GetObj("SetPsdSecond")))
		KillObj("SetPsdSecond");

	if(P_VALID(GetObj("ModPsdFirst")))
		KillObj("ModPsdFirst");

	if(P_VALID(GetObj("ModPsdSecond")))
		KillObj("ModPsdSecond");
	
	if(P_VALID(GetObj("ModPsdThird")))
		KillObj("ModPsdThird");

	if(P_VALID(GetObj("DelPsdFirst")))
		KillObj("DelPsdFirst");

	if(P_VALID(GetObj("PocketExtern_Check")))
		KillObj("PocketExtern_Check");
}


VOID PocketFrame::OnPetUnEquipment( DWORD dwPetID, INT8 n8PebPos, INT16 n16PosDst /*= GT_INVALID*/ )
{
    const tagPetEquip* petEquip = PetManager::Inst()->FindPetEquip(dwPetID, n8PebPos);
    if (P_VALID(petEquip))
    {
        PetManager::Inst()->BeginNC_PetUnequip(dwPetID, petEquip->n64ItemID, n16PosDst);
    }
}

VOID PocketFrame::ShowItemTip( GUIWnd* pWnd, INT16 n16Index )
{
	Item* pItem = GetItem(n16Index);
	if(P_VALID(pItem))
	{
		tstring strPrice = _T("");
		//if(EIST_ItemShopVend == ((ItemFrame*)m_pFather)->GetCurStrategy())
		//{
			tagItemCost cost(pItem->IsVendible() ? pItem->GetItemPrice() : GT_INVALID, ECCT_Null, 0, 0, GT_INVALID);
			strPrice = ToolTipCreator::Inst()->GetPriceTips(&cost);
		//}
		ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str(), strPrice.c_str());

		// �����װ����ʾװ���Ƚ�
		if(MIsEquipment(pItem->GetItemTypeID()))
		{
			ToolTipCreator::Inst()->ShowEquipCompare(pItem->GetItemTypeID());
		}
	}
}

VOID PocketFrame::ShowWarItemTip(GUIWnd* pWnd, INT16 n16Index)
{
	Item* pItem = GetWarItem(n16Index);
	if(P_VALID(pItem))
	{
		tstring strPrice = _T("");
		//if(EIST_ItemShopVend == ((ItemFrame*)m_pFather)->GetCurStrategy())
		//{
		tagItemCost cost(pItem->IsVendible() ? pItem->GetItemPrice() : GT_INVALID, ECCT_Null, 0, 0, GT_INVALID);
		strPrice = ToolTipCreator::Inst()->GetPriceTips(&cost);
		//}
		ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str(), strPrice.c_str());
	}
}


DWORD PocketFrame::OnNS_RequestAttackCode( tagNS_RequestAttackCode *pNetCmd, DWORD )
{
	/*
	tagNC_ServerAttack cmd;
	cmd.dwCode = 3*((DWORD)sqrt((double)pNetCmd->dwCode)) + 55;
	m_pSession->Send(&cmd);
	*/
	return 0;
}

VOID PocketFrame::RefreshWarItem(INT16 index)
{
	DWORD dwTypeID = GT_INVALID;
	INT16 n16Quantity = 0;
	BYTE byQuality = 0;

	if(index >= SPACE_WAR_BAG || index < 0)	//λ�ò��Ϸ�����
	{
		return;
	}
	else
	{
		Item* pItem = GetWarItem(index);
		if (P_VALID(pItem))
		{
			//��ȡ��ƷTypeID
			dwTypeID = pItem->GetItemTypeID();
			//��ȡ��Ʒ����
			n16Quantity = pItem->GetItemQuantity();
			//��ȡ��ƷƷ��
			byQuality = pItem->GetItemQuality();
		}
	}

	if(dwTypeID == GT_INVALID)
	{
		m_pIBtnWarItem[index]->SetInvisible(TRUE);
	}
	else
	{
		m_pIBtnWarItem[index]->RefreshItem(dwTypeID, n16Quantity, byQuality);
		m_pIBtnWarItem[index]->SetInvisible(FALSE);
	}
	

}

DWORD PocketFrame::OnEventEnterWar(tagGameEvent* pGameEvent)
{
	m_pWndWarBag->SetInvisible(FALSE);
	m_pWnd->SetInvisible(TRUE);
	m_pStcWarMoeny->SetText(_T("0"));
	m_pMgr->SendEvent(&tagGameEvent(_T("Close_Package_UI"), this));

	return 0;
}

DWORD PocketFrame::OnEnterNormalMap(tagGameEvent* pGameEvent)
{
	m_pWndWarBag->SetInvisible(TRUE);

	return 0;
}

DWORD PocketFrame::On_RefreshWarInfo(tagRefreshWarInfo* evt)
{
	if(evt->eType == ERoleAttrWarGold)
	{
		DWORD dwWarMoney = WarMgr::Inst()->GetWarMoneyNum();
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("%d"), dwWarMoney);
		m_pStcWarMoeny->SetText(szText);
	}

	return 0;
}
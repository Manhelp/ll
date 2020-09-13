#pragma once
#include "buttonex.h"

class EggButton :
	public ButtonEx
{
public:
	EggButton(void);
	~EggButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID OnEvent(tagGUIEvent* pEvent);
	VOID InitEggData(int layer,int pos,BOOL IsColour = FALSE);
	VOID Change2NormalEgg();
	VOID Change2ColuorEgg();
private:
	int		m_nLayer;					// ����
	int		m_nPos;						// λ��
	BOOL	m_bIsColorEgg;			//	�ǲ��ǲʵ�
};

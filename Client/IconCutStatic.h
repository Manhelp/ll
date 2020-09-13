#pragma once
#include "iconstatic.h"


class IconCutStatic :
	public IconStatic
{
public:
	IconCutStatic(void);
	~IconCutStatic(void);

	virtual VOID Update();
	virtual VOID Render();
	virtual VOID Destroy();
	
	VOID SetScaleValue(DWORD dwScale){m_ScaleValue = dwScale; }		// �ǰٷ�֮���ٵ���С  �����Ҫ����0 С�ڵ���100	ֻӰ�쵽��Ⱦ
	VOID SetCurrValue(DWORD dwScale){m_CurrentValue = dwScale; }	
	VOID SetCutNums(DWORD dwNum){m_CutNums = dwNum; }

private:

	DWORD m_ScaleValue;				// ��������ֵ
	DWORD m_CurrentValue;			// ��ǰ����ֵ
	DWORD m_CutNums;
};

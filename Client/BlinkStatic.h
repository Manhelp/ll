#pragma once
#include "iconstatic.h"

class BlinkStatic :
	public IconStatic
{
public:
	BlinkStatic(void);
	~BlinkStatic(void);

	VOID Update();

	//���ÿ�ݰ�ť��ͼƬʹ�õ���������
	void SetWndPicRect( const tagRect& rect){m_rcPic = rect;}
	void StartBlink();

	void SetStartPoint(LONG x2, LONG y2){	m_pPtStart.Set(x2,y2);}
	void SetSize(LONG x2, LONG y2){m_pPtSizeOfPic.Set(x2,y2);}
	void SetLoopNum(double dwNum){m_dwPicLoopNum = dwNum; m_bBlinking = false;}
private:
	double		m_dwPicLoopNum;
	tagPoint		m_pPtStart;
	tagPoint		m_pPtSizeOfPic;
	int				m_row;
	int				m_column;
	bool				m_bBlinking;
	
};

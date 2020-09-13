#pragma once

/** \class   CircleProgress
\brief   CircleProgress���̳�GUIProgress
\remarks ��ͼʹ��GUIStatic::m_pPic����ת���������߼���������ӿ���ת��ͼƬ
		 ��������Ⱦ������Ⱦ�ٷֱ�
*/
struct tagCircleImage
{
	tagGUIImage*	img;			// ͼƬ
	FLOAT			fSpeed;			// ת��
	tagRect			rcDest;			// Ŀ������
	BOOL			bIsClockwise;	// ת��
};

class CircleProgress : public GUIProgress
{
public:
	CircleProgress(void);
	~CircleProgress(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	
	//! �����ͼƬ������ת��Ϊ������ת
	VOID AddNewPic(const tstring strPath, tagRect rcDest, tagRect picRect, BOOL isClockwise = TRUE, FLOAT dwSpeed = 0);
private:
	vector<tagCircleImage*>		m_vecPicList;		
	FLOAT						m_fLastTime;
	INT							m_nCircleCount;
	FLOAT						m_fLastValue;
	FLOAT						m_fLastAngel;
};

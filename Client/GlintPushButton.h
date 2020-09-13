#pragma once

/**
	\brief ����˸�İ�ť������������˸ʱ������֣����ֵ���ɫ����˸�ļ����
		   ��˸ʱ������Ĭ���ǰ�ť������
		   ��˸ʱ���ֵ���ɫ�Ǳ�����ʱ���ֵ���ɫ
		   ��˸��ͼƬĬ���ǰ�ť�����µ�ͼƬ
	\autoer dyb
*/
class GlintPushButton : public GUIPushButton
{

public:
	GlintPushButton();
	~GlintPushButton();

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Update();
	virtual VOID Destroy();
	virtual VOID Render();

	bool IsGlintNow(){ return m_bShowGlint; }
	
	void SetGlintStart(bool bGlint )
	{ 
		m_bShowGlint = bGlint;
		if( m_bShowGlint )
			m_dwLastGlint = timeGetTime();
	}
	
	/** ������˸ʱ���ͼƬ
	*/
	void SetGlintPic( tstring strPath )
	{
		if( !P_VALID(m_pGlintPic) )
		{
			m_pRender->DestroyImage( m_pGlintPic );
			m_pGlintPic = NULL;
		}
		
		if( strPath.empty() )
			return;

		m_pGlintPic = m_pRender->CreateImage( strPath, m_rcPushPic );
	}

	/** ������˸ͼƬ����ɫ
	*/
	void SetGlintPicColor( DWORD dwColor )
	{
		m_dwGlintPicColor = dwColor;
	}

	/**  ������˸ʱ�������
	*/
	void SetGlintText( tstring strText )
	{
		m_strGlintText = strText;
	}
	
	/** ������˸ʱ���ֵ���ɫ
	*/
	void SetGlintTextColor( DWORD dwColor ){ m_dwGlintTextColor = dwColor; }

	/**
		���ö೤ʱ����˸һ��, ��λ�Ǻ���
	*/
	void SetGlintInterval( DWORD dwInterval ){ m_dwGlintInterval = dwInterval; }

protected:
	void DrawGlint();

protected:
	bool m_bShowGlint;							//�Ƿ���˸
	bool m_bShowInterval;

	tagGUIImage*	m_pGlintPic;				//��˸ͼƬ
	DWORD			m_dwGlintPicColor;			//��˸ͼƬ��ɫ

	tstring			m_strGlintText;				//��˸ʱ�������
	DWORD			m_dwGlintTextColor;			//������˸����ɫ
	DWORD			m_dwGlintInterval;			//�೤ʱ����˸һ�£�����
	DWORD			m_dwLastGlint;				//�ϴμ�ʱ��ʱ��
};
#pragma once

class GlintButton : public GUIButton
{
public:
	GlintButton(void);
	~GlintButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Update();
	virtual VOID Destroy();
	virtual VOID Render();

	bool IsGlintNow(){ return m_bShowGlint; }

	void BeginGlint()
	{ 
		m_bShowGlint = true;
		m_dwLastGlint = timeGetTime();
	}

	void EndGlint()
	{ 
		m_bShowGlint = false;
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

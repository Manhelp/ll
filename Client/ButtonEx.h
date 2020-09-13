#pragma once
#include "RotaterInRect.h"
#include ".\GameClientDefine.h"

//ͼƬö��
enum eWndPicType				
{
	EWPICTYPE_CURPIC = 0,			//��ǰ(������)
	EWPICTYPE_PUSHPIC,				//����
	EWPICTYPE_MOVEPIC,				//��꾭��
	EWPICTYPE_DISABLEPIC,			//����
};

class ButtonEx :
	public GUIButton
{
public:
	ButtonEx(void);
	~ButtonEx(void);
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	//--�߼������ݵĶ�д 
	const BtnData& GetBtnData() const { return m_BtnData ; }
	void SetBtnData2(DWORD dwParam) { m_BtnData.SetData2(dwParam); }
	void SetBtnData(EShortcutBtnType eType, DWORD dwParam1, DWORD dwParam2, INT64 n64Param3 );
	void SetBtnData( const BtnData& data ){ 
		SetBtnData( data.GetType(), data.GetData1(), data.GetData2(), data.GetData3() );
	}

	//���ƶ����
	VOID Draw2DPrimitiveUP( EDrawType eType, LPPOINT lpPoints, int nVerCount, DWORD color=0xFFFFFFFF);

	//������ȴ�ָ�ʱ��
	int GetBtnRestoreTime(float& fRestoreRatio);

    //�õ����ܿ���״̬
    void UpdateSparking();

	//--��������״̬ͼƬcolor
	void SetDisablePicColor(DWORD color){ m_dwDisablePicColor = color; }
	void SetPushPicColor(DWORD color){ m_dwPushPicColor = color; }
	void SetMovePicColor(DWORD color){ m_dwMovePicColor = color; }
	void SetDisablePic(DWORD color){ m_dwDisablePicColor = color; }

	//��ñ���pic
	tstring GetPicFileName() { return m_strPic; }
	void SetPicFileName(const tstring& picName)
	{
		SetWndPic(EWPICTYPE_CURPIC, picName);
	}
	void ClearPic()
	{	
		SetWndPic(EWPICTYPE_CURPIC, _T("data\\ui\\Main\\L_icon_back.bmp"));
		m_dwStartPressTime = 0;
		m_bCanUse = true;
		m_fCDCount = 0.0f;
		SetRefresh(TRUE); 
	}

	virtual INT64 GetItem64ID() { return -1;}

	//������Ϸ�¼�
	void InvokeGameEvent( );	

    //����ͼ����˸
    void SetSpark(bool bEnabled) { m_bisRouding = bEnabled; }

protected:

	//���ÿ�ݰ�ť��ͼƬ
	void SetWndPic(eWndPicType ePicType, const tstring& strPic);

	//���ÿ�ݰ�ť��ͼƬʹ�õ���������
	void SetWndPicRect(eWndPicType ePicType, const tagRect& rect);

	//����ֵ  ʣ��ʱ�� ����CD��ȴ��ת��ͼƬ
	int DrawShadow( int nSquareWidth );	

	//������ȴʱ�����ʾ
	void DrawShadowText();

	//����ͼƬ
	void DrawImg();

	//ת��CDʱ��(����)
	void CalCDTime(int nTime);

	//�϶�����ת���ɿ�ݰ�ť����
	EShortcutBtnType  DragType2SCBtnType(EDragType eDragType)
	{
		if( eDragType > ESBT_End )
			return ESBT_Null;
		EShortcutBtnType eSbtArr[EDT_End] =	{ ESBT_Null,ESBT_Skill,ESBT_Item,ESBT_PetSkill,ESBT_StyleAction,ESBT_ItemSplit,ESBT_ItemEquip,ESBT_ItemWare,ESBT_ItemShop,ESBT_PetEquip,ESBT_GuildWare,ESBT_HolyEquip,ESBT_WarBag };

		
		return eSbtArr[eDragType];
	}

protected:

	BtnData				m_BtnData;					//��ݰ�ť���������
	tstring				m_strCDTime;				//������ȴʱ��
	float				m_fCDCount;					//CDʱ��ͳ��
	float				m_fRestoreRatio;			//��ת����
	static	RotaterInRect	m_Rotater;				//��ת��ļ���	
	tagGUIFont*			m_pShadowFont;
	DWORD				m_dwStartPressTime;			//��ʼ�������ť��ͼƬ

    DWORD               m_dwStartSparkTime;         //��˸���¼��

public:

	bool				m_bCanUse;					//�Ƿ������ܵ��ʹ�ô˼���
    bool                m_bisRouding;               //�Ƿ�⻷��˸
};

#pragma once

#include "..\WorldDefine\pet_define.h"

//! \class  PetShowWindow
//! \brief  ����ģ��չ̨
//! \author hyu
//! \date   2009-6-17
//! \last   2009-6-17
class PetShowWindow :
    public GUIStatic
{
public:
    PetShowWindow(void);
    virtual ~PetShowWindow(void);

    // ��д����
    virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
    virtual VOID Destroy();
    virtual VOID Update();
    virtual VOID Render();
    virtual VOID RefreshViewCoord();
    virtual VOID SetInvisible(BOOL b);
	virtual VOID AddChild(GUIWnd* pChild);//add by xp

    //! \brief  ��Ⱦ����ģ��
    void Render3D();

    //! \brief  ��ת,�ۻ��ͱ���
    void RotateYaw(FLOAT fAngle);

    //! \func   SetPetShow
    //! \param  dwPetTypeID     ����ԭ��id
    //! \param  eModelType      ����ģ����Ū
    //! \brief  ������ʾ����
    void SetPetShow(DWORD dwPetTypeID, EPetModelType eModelType = EPMT_Follow);

	VOID SetModelYaw(float fNum);

protected:

    //! \brief  ���������
    virtual void UpdateCamera();

    //! \brief  ����ģ��
    EntityNode* LoadEntity(LPCTSTR szFileName);
    void LoadSkin(LPCTSTR szFileName, EntityNode* pEntityNode);

protected:
	tagGUIImage*			m_pNewCanvas;       // RenderTarget
	EntityNode*				m_pPetNode;         // SceneNode
	NullSceneGraph*			m_pSG;              // ���ڱ�������������յ���Ϣ,��������Ⱦ
    CameraBase				m_Camera;

private:
    DWORD                   m_dwCurrPetTypeId;  // ��ǰ����id
    FLOAT                   m_fModelYaw;

	GUIButton*              m_pBnRotaLeft;
	GUIButton*              m_pBnRotaRight;

};

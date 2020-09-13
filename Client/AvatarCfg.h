#pragma once

/**
	class: AvatarCfg
	brief: ���õ���ģʽ���ɳ�����ָ��ģ�͡�������ͼ��·��
*/
#define AVATAR_HAIR_TYPE 8
#define AVATAR_HAIR_COLOR 5
#define AVATAR_FACE 10
#define AVATAR_FACE_DETAIL 7

#define AVATAR_HAIR_TYPE_EX 8
#define AVATAR_FACE_EX 10

#define AVATAR_DRESS 2          // ��ѡ����ʱװ��ʽ����Ŀ

class AvatarCfg
{
public:
	AvatarCfg(void);
	~AvatarCfg(void);
	static AvatarCfg* Inst();

	VOID LoadFromFile();

	int GetNumHairType()const	{ return AVATAR_HAIR_TYPE; }
	int GetNumHairTypeEx()const	{ return AVATAR_HAIR_TYPE_EX; }
	int GetNumHairColor()const	{ return AVATAR_HAIR_COLOR; }
	int GetNumFace()const		{ return AVATAR_FACE; }
	int GetNumFaceEx()const		{ return AVATAR_FACE_EX; }
	int GetNumFaceDetail()const	{ return AVATAR_FACE_DETAIL; }
	int GetNumDress()const		{ return AVATAR_DRESS; }

	const TCHAR* GetHairTypeMdlPath(BYTE bySex, WORD id, BYTE byTransSkinIndex = 0);						//ͷ��ģ��·��
	const TCHAR* GetHairColorTexPath(BYTE bySex, WORD hairID, WORD colorID, BYTE byTransSkinIndex = 0);	//ͷ��������ͼ·��
	const TCHAR* GetFaceMdlPath(BYTE bySex, WORD id, BYTE byTransSkinIndex = 0);							//�沿ģ��·��
	const TCHAR* GetFaceDtlTexPath(BYTE bySex, WORD id);						//�沿������ͼ·��
	/** \��ȡ����ģ�͵�·��
	*/
	const TCHAR* GetUpperUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex = 0);					//����ģ��·��
	const TCHAR* GetLowerUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex = 0);					//����ģ��·��
	const TCHAR* GetBootUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex = 0);					//Ьģ��·��	
	const TCHAR* GetGloveUWMdlPath(BYTE bySex,BOOL bRight,BOOL bFashion, BYTE byTransSkinIndex = 0);		//������··��(������)
	const TCHAR* GetBackUWMdlPath(BYTE bySex, BYTE byTransSkinIndex = 0);		// ����

	/** \�����Ա��������һ������ʱװ�ṹ
	*/
	void MakeNewPlayerFahion( const BYTE bySex, const int n, tagAvatarEquip& out ) const;

	// ����Ƥ��
	struct tagTransSkin
	{
		DWORD dwIndex;
		tstring strTSFace;
		tstring strTSHat;
		tstring strTSHair;
		tstring strTSHairColor;
		tstring strTSUpper;
		tstring strTSLower;
		tstring strTSGlove_r;
		tstring strTSGlove_l;
		tstring strTSBoot;
		tstring strTSMantle;
	};
private:
	tstring							m_strPath;
	map<DWORD, tagTransSkin>		m_mapTransSkin;
};

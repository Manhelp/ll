#pragma once

struct tagCreateRolePreviewItem
{
	BYTE bSex;					// �Ա�
	INT nCareerType;			// ְҵ
	DWORD dwWeapon;				// ����
	tstring strWeaponEffect1;	// ������Ч1
	tstring strWeaponEffect2;	// ������Ч2
	tstring strWeaponEffect3;	// ������Ч3
	DWORD dwHat;				// ͷ��
	DWORD dwUpper;				// ����
	DWORD dwLower;				// ����
	DWORD dwGlove;				// ����
	DWORD dwBoot;				// Ь��
	DWORD dwEffectID;			// װ����Ч
	tagCreateRolePreviewItem()
	{
		memset( this, 0, sizeof(*this) );
	}
};

class CreateRolePreviewData
{

public:
	CreateRolePreviewData(void);
	~CreateRolePreviewData(void);

	virtual void LoadFromFile();

	const tagCreateRolePreviewItem* FindPreviewInfoBySexAndCareer( BYTE bSex, INT nCareerType )
	{
		map<INT,  tagCreateRolePreviewItem>::const_iterator iter = m_mapPreviewCfg.begin();
		for( iter; iter != m_mapPreviewCfg.end(); iter++ )
		{
			if( iter->second.bSex == bSex && iter->second.nCareerType == nCareerType )
			{
				return &(iter->second);
			}
		}
		return NULL;
	}

private:
	map<INT,  tagCreateRolePreviewItem>	m_mapPreviewCfg;		// ְҵװ��չʾ
};

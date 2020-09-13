#pragma once

#include "../WorldDefine/family_define.h"

#pragma pack(push, 1)

//--------------------------------------------------------------------------
//��family_sprite���
//����һ����������
CMD_START( NDBC_CreateFamilySprite )
	tagFamilySpriteSave		fs;
CMD_END

//����һ����������
CMD_START( NDBC_UpdateFamilySprite )
	tagFamilySpriteSave		fs;
CMD_END

//ɾ��һ����������
CMD_START( NDBC_DeleteFamilySprite )
	DWORD	dwFamilyID;						//����ID
CMD_END

//��ȡ����������Ϣ
CMD_START( NDBC_LoadFamilySprite )
CMD_END

CMD_START( NDBS_LoadFamilySprite )
	tagFamilySpriteSave				fs;
	INT								nNumRequirement;
	INT								nNumSprite;
	tagFamilySpriteRequirementSave	fsr[1];
	tagRoleSpriteDataSave			rsds[1];
CMD_END


//�洢���м���������Ϣ
CMD_START( NDBC_SaveAllFamilySprite )
	INT								nNum;
	tagFamilySpriteSave				fs[1];
CMD_END

//--------------------------------------------------------------------------
//��family_sprite_requirement���
//����һ������������������
CMD_START( NDBC_CreateFamilySpriteRequirement )
	DWORD	dwFamilyID;					//����ID
	INT		nNum;						//����
	tagFamilySpriteRequirementSave	fsr[1];
CMD_END

//���¼���������һ����������
CMD_START( NDBC_UpdateFamilySpriteRequirement )
	DWORD dwFamilyID;					//����ID
	tagFamilySpriteRequirementSave	fsr;
CMD_END

//ɾ��һ�������������������
CMD_START( NDBC_DeleteFamilySpriteRequirement )
	DWORD	dwFamilyID;						//����ID
CMD_END

//��ոñ�
CMD_START( NDBC_DeleteALLFamilySpriteRequirement )
CMD_END
//--------------------------------------------------------------------------
//��family_role_sprite���
//--------------------------------------------------------------------------
//�����Ǽ�������Ϣ
CMD_START( NDBC_CreateRoleSpriteData )
	DWORD	dwFamilyID;
	tagRoleSpriteDataSave	rsd;
CMD_END

//���µǼǵ�������Ϣ
CMD_START( NDBC_UpdateRoleSpriteData )
	DWORD	dwFamilyID;
	tagRoleSpriteDataSave	rsd;
CMD_END

//ɾ���Ǽǵ�������Ϣ
CMD_START( NDBC_DeleteRoleSpriteData )
	DWORD	dwRoleID;
CMD_END

#pragma pack(pop)


--�س����ȼ��ж�
function S1012401_CanUsehuicheng(MapID, InstanceID, SkillID , RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�ж���ҵȼ��Ƿ�С��10��
	local level = role.GetRoleLevel(MapID, InstanceID , RoleID )
	if(level < 10) then
		--��ʾ����Ϊ0����ʹ��
		bRet = 29
	end
	return bRet, bIgnore
end

aux.RegisterSkillEvent(1012401, 0, "S1012401_CanUsehuicheng")




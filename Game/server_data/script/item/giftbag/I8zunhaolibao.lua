--I8������
function I4530197_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)

end

function I4530197_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530197, 1, "I4530197_OnUse")
aux.RegisterItemEvent(4530197, 0, "I4530197_CanUse")

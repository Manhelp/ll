--
function I4820173_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820085, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820173, 20, 101)
	return 1
end

function I4820173_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,4820173)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		return 40, false
	end
	if b < 20 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820173, 1, "I4820173_Use")
aux.RegisterItemEvent(4820173, 0, "I4820173_CanUse")

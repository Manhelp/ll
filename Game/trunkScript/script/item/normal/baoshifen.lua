--
function I3050084_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050085, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3050084, 10, 101)
	return 1
end

function I3050084_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,3050084)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		return 40, false
	end
	if b <= 9 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(3050084, 1, "I3050084_Use")
aux.RegisterItemEvent(3050084, 0, "I3050084_CanUse")

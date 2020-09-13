--��֮�׼���Ƭ
function I4400416_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400415, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4400416, 15, 101)
	return 1
end

function I4400416_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,4400416)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		return 40, false
	end
	if b <= 14 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4400416, 1, "I4400416_Use")
aux.RegisterItemEvent(4400416, 0, "I4400416_CanUse")


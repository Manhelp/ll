--�����Ƭ�Ҽ�ʹ��
function I3530001_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3530002, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3530001, 20, 101)
	return 1
end

function I3530001_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,3530001)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		return 40, false
	end
	if b <= 19 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(3530001, 1, "I3530001_Use")
aux.RegisterItemEvent(3530001, 0, "I3530001_CanUse")
--�ڲ�Ա����
function I4700018_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020025, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020029, 12, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220904, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211803, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
end

function I4700018_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 23) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700018, 1, "I4700018_GiftBag")
aux.RegisterItemEvent(4700018, 0, "I4700018_CanUseGiftBag")









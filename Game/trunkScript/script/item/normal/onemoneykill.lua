--һԪ��ɱ��

function I4700017_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211101, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220401, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
end

function I4700017_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700017, 1, "I4700017_GiftBag")
aux.RegisterItemEvent(4700017, 0, "I4700017_CanUseGiftBag")









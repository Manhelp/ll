--360�����������
function I4830080_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4212019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4222003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800287, 120, -1, 8, 420)

end

function I4830080_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830080, 1, "I4830080_GiftBag")
aux.RegisterItemEvent(4830080, 0, "I4830080_CanUseGiftBag")

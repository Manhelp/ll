--
function I4700044_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220902,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800098,1, -1, 8, 420)
end

function I4700044_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700044, 1, "I4700044_GiftBag")
aux.RegisterItemEvent(4700044, 0, "I4700044_CanUseGiftBag")









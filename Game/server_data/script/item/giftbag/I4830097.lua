function I4830097_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local r = math.random(7)
	if r ==1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040028,1, -1, 8, 420)
	elseif r ==2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040033,1, -1, 8, 420)
	elseif r ==3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040038,1, -1, 8, 420)
	elseif r ==4 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040043,1, -1, 8, 420)
	elseif r ==5 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040048,1, -1, 8, 420)
	elseif r ==6 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040053,1, -1, 8, 420)
	elseif r ==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3040058,1, -1, 8, 420)
	end
end

function I4830097_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 1 then
		bRet = 40
	end
	
	return bRet, bIgnore
end

aux.RegisterItemEvent(4830097, 1, "I4830097_GiftBag")
aux.RegisterItemEvent(4830097, 0, "I4830097_CanUseGiftBag")
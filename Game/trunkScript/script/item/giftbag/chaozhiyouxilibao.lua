function I4530151_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530152, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 10, -1, 8, 420)

end

function I4530151_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530151, 1, "I4530151_GiftBag")
aux.RegisterItemEvent(4530151, 0, "I4530151_CanUseGiftBag")




function I4530152_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530153, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 20, -1, 8, 420)
end

function I4530152_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530152, 1, "I4530152_GiftBag")
aux.RegisterItemEvent(4530152, 0, "I4530152_CanUseGiftBag")




function I4530153_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060002, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530154, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 20, -1, 8, 420)
end

function I4530153_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530153, 1, "I4530153_GiftBag")
aux.RegisterItemEvent(4530153, 0, "I4530153_CanUseGiftBag")




function I4530154_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060003, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530155, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 20, -1, 8, 420)
end

function I4530154_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530154, 1, "I4530154_GiftBag")
aux.RegisterItemEvent(4530154, 0, "I4530154_CanUseGiftBag")




function I4530155_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060004, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100004, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3100014, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530156, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 50, -1, 8, 420)
end

function I4530155_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530155, 1, "I4530155_GiftBag")
aux.RegisterItemEvent(4530155, 0, "I4530155_CanUseGiftBag")




function I4530156_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090008, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090010, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090011, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 50, -1, 8, 420)
end

function I4530156_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530156, 1, "I4530156_GiftBag")
aux.RegisterItemEvent(4530156, 0, "I4530156_CanUseGiftBag")

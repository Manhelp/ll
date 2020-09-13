function I4700150_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530100, 1, -1, 8, 420)
    if r <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 1, -1, 8, 420)
	end

end

function I4700150_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700150, 1, "I4700150_GiftBag")
aux.RegisterItemEvent(4700150, 0, "I4700150_CanUseGiftBag")



function I4700151_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530101, 1, -1, 8, 420)
    if r <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 2, -1, 8, 420)
	end

end

function I4700151_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700151, 1, "I4700151_GiftBag")
aux.RegisterItemEvent(4700151, 0, "I4700151_CanUseGiftBag")




function I4700152_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530102, 1, -1, 8, 420)
	if r <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 3, -1, 8, 420)
	end
    if r <= 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060060, 1, -1, 8, 420)
	end

end

function I4700152_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700152, 1, "I4700152_GiftBag")
aux.RegisterItemEvent(4700152, 0, "I4700152_CanUseGiftBag")

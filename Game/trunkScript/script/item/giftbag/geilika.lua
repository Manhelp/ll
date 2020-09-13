--������1��
function I4530125_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530128, 1, -1, 8, 420)
end

function I4530125_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530125, 1, "I4530125_GiftBag")
aux.RegisterItemEvent(4530125, 0, "I4530125_CanUseGiftBag")

--������30��
function I4530128_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530129, 1, -1, 8, 420)
end

function I4530128_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530128, 1, "I4530128_GiftBag")
aux.RegisterItemEvent(4530128, 0, "I4530128_CanUseGiftBag")

--������60��
function I4530129_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050017, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070069, 2, -1, 8, 420)
end

function I4530129_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530129, 1, "I4530129_GiftBag")
aux.RegisterItemEvent(4530129, 0, "I4530129_CanUseGiftBag")

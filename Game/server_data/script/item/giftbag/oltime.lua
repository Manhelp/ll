--���߽������4h
function I4700051_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4800131, 1, -1, 8, 420)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local 	rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if			rolelevel < 40	then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,15, -1, 8, 420)
	elseif		rolelevel >= 40	and	 rolelevel <= 59 then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,15, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4090001,1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4800287,2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4700292,1, -1, 8, 420)
	elseif		rolelevel >= 60	and	 rolelevel <= 79 then
				local num = math.random(2,5)
				role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,15, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820028,num, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820044,20, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4800287,2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4700292,1, -1, 8, 420)
	elseif		rolelevel >= 80	and	 rolelevel <= 89 then
				local num = math.random(2,5)
				role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,15, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820024,num, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820044,20, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4800287,2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4700292,1, -1, 8, 420)
	elseif		rolelevel >= 90	and	 rolelevel <= 100 then
				local num = math.random(2,3)
				role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,15, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820029,num, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820044,20, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4800287,2, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4700292,1, -1, 8, 420)
	end
end

function I4700051_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700051, 1, "I4700051_GiftBag")
aux.RegisterItemEvent(4700051, 0, "I4700051_CanUseGiftBag")



--���߽������6h
function I4700052_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local 	rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if			rolelevel < 40	then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3090101,1, -1, 8, 420)
	elseif		rolelevel >= 40	and	 rolelevel <= 59 then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3090101,1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4090001,1, -1, 8, 420)
	elseif		rolelevel >= 60	and	 rolelevel <= 79 then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3090101,1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820028,3, -1, 8, 420)
	elseif		rolelevel >= 80	and	 rolelevel <= 89 then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3090101,1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820024,2, -1, 8, 420)
	elseif		rolelevel >= 90	and	 rolelevel <= 100 then
				role.AddRoleItem(MapID, InstanceID, TargetID, 3090101,1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, TargetID, 4820029,2, -1, 8, 420)
	end
end

function I4700052_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700052, 1, "I4700052_GiftBag")
aux.RegisterItemEvent(4700052, 0, "I4700052_CanUseGiftBag")

function I4700222_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,1, -1, 8, 420)
	local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if(chudonglibao == 1) then
		if(level >= 40 and level <= 85) then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4820028,10, -1, 8, 420)
		end
	end
end

function I4700222_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local temp =  true
	if(chudonglibao == 1 and FreeSize < 5) then
		temp = false
	end
	if(chudonglibao ~= 1 and FreeSize < 4) then
		temp = false
	end
	if(temp == false) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700222, 1, "I4700222_GiftBag")
aux.RegisterItemEvent(4700222, 0, "I4700222_CanUseGiftBag")

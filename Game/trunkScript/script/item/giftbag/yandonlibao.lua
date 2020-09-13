
--�϶������200��
function I4530093_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 3, -1, 8, 420)
	local k = math.random(1,100)
	if k < 4 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
	end

end

function I4530093_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530093, 1, "I4530093_GiftBag")
aux.RegisterItemEvent(4530093, 0, "I4530093_CanUseGiftBag")





--�϶������2000�㣩
function I4530094_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 6, -1, 8, 420)
	local k = math.random(1,100)
	if k <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 1, -1, 8, 420)
	end
	local k = math.random(1,100)
	if k <= 20 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
	end

end

function I4530094_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530094, 1, "I4530094_GiftBag")
aux.RegisterItemEvent(4530094, 0, "I4530094_CanUseGiftBag")







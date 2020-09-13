
--�����������600�㣩
function I4530091_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060004, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010036, 1, -1, 8, 420)
	local k = math.random(1,10)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 1, -1, 8, 420)
	end

end

function I4530091_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530091, 1, "I4530091_GiftBag")
aux.RegisterItemEvent(4530091, 0, "I4530091_CanUseGiftBag")





--�����������1000�㣩
function I4530092_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010036, 2, -1, 8, 420)
	local k = math.random(1,6)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 1, -1, 8, 420)
	end

end

function I4530092_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530092, 1, "I4530092_GiftBag")
aux.RegisterItemEvent(4530092, 0, "I4530092_CanUseGiftBag")







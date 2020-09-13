--4Сʱ
function I4820025_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	if math.random(100) > 90 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 1, -1, 8, 420)
	end

end

function I4820025_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820025, 1, "I4820025_GiftBag")
aux.RegisterItemEvent(4820025, 0, "I4820025_CanUseGiftBag")



--6Сʱ
function I4820026_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 80)
	if math.random(100) > 75 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 1, -1, 8, 420)
	end

end

function I4820026_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820026, 1, "I4820026_GiftBag")
aux.RegisterItemEvent(4820026, 0, "I4820026_CanUseGiftBag")

--�������Ƭ
function I4820027_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020082, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820027, 10, 101)
	return 1
end

function I4820027_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,4820027)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		return 40, false
	end
	if b <= 9 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820027, 1, "I4820027_Use")
aux.RegisterItemEvent(4820027, 0, "I4820027_CanUse")

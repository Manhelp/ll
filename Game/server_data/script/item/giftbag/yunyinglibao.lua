--10��VIP���
function I4530011_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530012, 1, -1, 8, 420)
end

function I4530011_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530011, 1, "I4530011_GiftBag")
aux.RegisterItemEvent(4530011, 0, "I4530011_CanUseGiftBag")



--20��VIP���
function I4530012_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530013, 1, -1, 8, 420)
end

function I4530012_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530012, 1, "I4530012_GiftBag")
aux.RegisterItemEvent(4530012, 0, "I4530012_CanUseGiftBag")



--30��VIP���
function I4530013_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530014, 1, -1, 8, 420)
end

function I4530013_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530013, 1, "I4530013_GiftBag")
aux.RegisterItemEvent(4530013, 0, "I4530013_CanUseGiftBag")


--40��VIP���
function I4530014_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530015, 1, -1, 8, 420)
end

function I4530014_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530014, 1, "I4530014_GiftBag")
aux.RegisterItemEvent(4530014, 0, "I4530014_CanUseGiftBag")




--50��VIP���
function I4530015_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
end

function I4530015_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530015, 1, "I4530015_GiftBag")
aux.RegisterItemEvent(4530015, 0, "I4530015_CanUseGiftBag")




--10���������
function I4530021_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530022, 1, -1, 8, 420)
end

function I4530021_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530021, 1, "I4530021_GiftBag")
aux.RegisterItemEvent(4530021, 0, "I4530021_CanUseGiftBag")




--20���������
function I4530022_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530023, 1, -1, 8, 420)
end

function I4530022_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530022, 1, "I4530022_GiftBag")
aux.RegisterItemEvent(4530022, 0, "I4530022_CanUseGiftBag")



--30���������
function I4530023_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
end

function I4530023_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530023, 1, "I4530023_GiftBag")
aux.RegisterItemEvent(4530023, 0, "I4530023_CanUseGiftBag")




--28���������
function I4530031_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530032, 1, -1, 8, 420)
end

function I4530031_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530031, 1, "I4530031_GiftBag")
aux.RegisterItemEvent(4530031, 0, "I4530031_CanUseGiftBag")



--38���������
function I4530032_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530033, 1, -1, 8, 420)
end

function I4530032_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530032, 1, "I4530032_GiftBag")
aux.RegisterItemEvent(4530032, 0, "I4530032_CanUseGiftBag")


--48���������
function I4530033_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4210202, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220102, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
end

function I4530033_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530033, 1, "I4530033_GiftBag")
aux.RegisterItemEvent(4530033, 0, "I4530033_CanUseGiftBag")





--�ͷ״������600�㣩
function I4530041_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070027, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 3, -1, 8, 420)
	local k = math.random(1,20)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
	end

end

function I4530041_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530041, 1, "I4530041_GiftBag")
aux.RegisterItemEvent(4530041, 0, "I4530041_CanUseGiftBag")



--�ͷ״������1000�㣩
function I4530042_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070027, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 5, -1, 8, 420)
	local k = math.random(1,10)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
	end

end

function I4530042_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530042, 1, "I4530042_GiftBag")
aux.RegisterItemEvent(4530042, 0, "I4530042_CanUseGiftBag")



--�ͷ״������1000�㣩
function I4530051_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 3, -1, 8, 420)

end

function I4530051_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530051, 1, "I4530051_GiftBag")
aux.RegisterItemEvent(4530051, 0, "I4530051_CanUseGiftBag")






























--4700187	��ҫһ�����
function I4700187_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030003, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030011, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030031, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030039, 2, -1, 8, 420)
end

function I4700187_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700187, 1, "I4700187_GiftBag")
aux.RegisterItemEvent(4700187, 0, "I4700187_CanUseGiftBag")


--4700188	��ҫ�������
function I4700188_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030004, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030012, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030032, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030040, 2, -1, 8, 420)
end

function I4700188_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700188, 1, "I4700188_GiftBag")
aux.RegisterItemEvent(4700188, 0, "I4700188_CanUseGiftBag")


--4700189	��ҫ�������
function I4700189_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030004, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030012, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030032, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030040, 5, -1, 8, 420)
end

function I4700189_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700189, 1, "I4700189_GiftBag")
aux.RegisterItemEvent(4700189, 0, "I4700189_CanUseGiftBag")


--4700190	��ҫ�������
function I4700190_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030005, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030013, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030033, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030041, 2, -1, 8, 420)
end

function I4700190_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700190, 1, "I4700190_GiftBag")
aux.RegisterItemEvent(4700190, 0, "I4700190_CanUseGiftBag")


--4700191	��ҫ�������
function I4700191_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030005, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030013, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030033, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030041, 4, -1, 8, 420)
end

function I4700191_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700191, 1, "I4700191_GiftBag")
aux.RegisterItemEvent(4700191, 0, "I4700191_CanUseGiftBag")



--4700192	��ҫ�������
function I4700192_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020122, 1, -1, 8, 420)

end

function I4700192_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700192, 1, "I4700192_GiftBag")
aux.RegisterItemEvent(4700192, 0, "I4700192_CanUseGiftBag")



--4700193	��ҫ�������
function I4700193_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030006, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030014, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020123, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020122, 2, -1, 8, 420)
end

function I4700193_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700193, 1, "I4700193_GiftBag")
aux.RegisterItemEvent(4700193, 0, "I4700193_CanUseGiftBag")



--4700194	��ҫ�������
function I4700194_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030007, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030015, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020122, 2, -1, 8, 420)
end

function I4700194_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700194, 1, "I4700194_GiftBag")
aux.RegisterItemEvent(4700194, 0, "I4700194_CanUseGiftBag")



--4700195	��ҫ�������
function I4700195_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030016, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020125, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 2, -1, 8, 420)
end

function I4700195_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700195, 1, "I4700195_GiftBag")
aux.RegisterItemEvent(4700195, 0, "I4700195_CanUseGiftBag")



--4700196	��ҫʮ�����
function I4700196_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020126, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020125, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 2, -1, 8, 420)

end

function I4700196_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700196, 1, "I4700196_GiftBag")
aux.RegisterItemEvent(4700196, 0, "I4700196_CanUseGiftBag")

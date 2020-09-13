--�᳤ר����
function I4500101_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500056, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020066, 2, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 1000000, 420)
end

function I4500101_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500101, 1, "I4500101_GiftBag")
aux.RegisterItemEvent(4500101, 0, "I4500101_CanUseGiftBag")



--�߼��᳤ר����
function I4500102_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500056, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020066, 2, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 1250000, 420)
end

function I4500102_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500102, 1, "I4500102_GiftBag")
aux.RegisterItemEvent(4500102, 0, "I4500102_CanUseGiftBag")


--�ؼ��᳤ר����
function I4500103_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500056, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020066, 3, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 1500000, 420)
end

function I4500103_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500103, 1, "I4500103_GiftBag")
aux.RegisterItemEvent(4500103, 0, "I4500103_CanUseGiftBag")


--����߽װ�
function I4500104_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020066, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 750000, 420)
end

function I4500104_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500104, 1, "I4500104_GiftBag")
aux.RegisterItemEvent(4500104, 0, "I4500104_CanUseGiftBag")


--�߼�����߽װ�
function I4500105_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020066, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 1000000, 420)
end

function I4500105_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500105, 1, "I4500105_GiftBag")
aux.RegisterItemEvent(4500105, 0, "I4500105_CanUseGiftBag")


--�ؼ�����߽װ�
function I4500106_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500056, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020066, 2, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 1200000, 420)
end

function I4500106_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500106, 1, "I4500106_GiftBag")
aux.RegisterItemEvent(4500106, 0, "I4500106_CanUseGiftBag")


--����Ǹɰ�
function I4500107_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 2, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 400000, 420)
end

function I4500107_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500107, 1, "I4500107_GiftBag")
aux.RegisterItemEvent(4500107, 0, "I4500107_CanUseGiftBag")


--�߼�����Ǹɰ�
function I4500108_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 3, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 600000, 420)
end

function I4500108_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500108, 1, "I4500108_GiftBag")
aux.RegisterItemEvent(4500108, 0, "I4500108_CanUseGiftBag")

--�ؼ�����Ǹɰ�
function I4500109_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 4, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 800000, 420)
end

function I4500109_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500109, 1, "I4500109_GiftBag")
aux.RegisterItemEvent(4500109, 0, "I4500109_CanUseGiftBag")



--���ᾫӢ��
function I4500110_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 250000, 420)
end

function I4500110_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500110, 1, "I4500110_GiftBag")
aux.RegisterItemEvent(4500110, 0, "I4500110_CanUseGiftBag")


--�߼����ᾫӢ��
function I4500111_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 2, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 400000, 420)
end

function I4500111_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500111, 1, "I4500111_GiftBag")
aux.RegisterItemEvent(4500111, 0, "I4500111_CanUseGiftBag")

--�ؼ����ᾫӢ��
function I4500112_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 3, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 600000, 420)
end

function I4500112_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500112, 1, "I4500112_GiftBag")
aux.RegisterItemEvent(4500112, 0, "I4500112_CanUseGiftBag")


--�����Ա��
function I4500113_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 100000, 420)
end

function I4500113_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500113, 1, "I4500113_GiftBag")
aux.RegisterItemEvent(4500113, 0, "I4500113_CanUseGiftBag")


--�߼������Ա��
function I4500114_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 200000, 420)
end

function I4500114_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500114, 1, "I4500114_GiftBag")
aux.RegisterItemEvent(4500114, 0, "I4500114_CanUseGiftBag")



--�ؼ������Ա��
function I4500115_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4500057, 2, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 300000, 420)
end

function I4500115_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4500115, 1, "I4500115_GiftBag")
aux.RegisterItemEvent(4500115, 0, "I4500115_CanUseGiftBag")






















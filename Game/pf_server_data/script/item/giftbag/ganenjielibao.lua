--�����
function I4800195_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
end

function I4800195_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800195, 1, "I4800195_GiftBag")
aux.RegisterItemEvent(4800195, 0, "I4800195_CanUseGiftBag")



--�ж������
function I4800196_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820043,1, -1, 8, 420)
end

function I4800196_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800196, 1, "I4800196_GiftBag")
aux.RegisterItemEvent(4800196, 0, "I4800196_CanUseGiftBag")



--ʥ�������
function I4800197_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008,1, -1, 8, 420)
end

function I4800197_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800197, 1, "I4800197_GiftBag")
aux.RegisterItemEvent(4800197, 0, "I4800197_CanUseGiftBag")




--���������
function I4800198_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020087,1, -1, 8, 420)
end

function I4800198_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800198, 1, "I4800198_GiftBag")
aux.RegisterItemEvent(4800198, 0, "I4800198_CanUseGiftBag")

--30����ʥʱװ��
function I3090031_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152004, 1, -1, 8, 420)
end

function I3090031_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090031, 1, "I3090031_GiftBag")
aux.RegisterItemEvent(3090031, 0, "I3090031_CanUseGiftBag")



--������ʥʱװ��
function I3090032_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8152014, 1, -1, 8, 420)
end

function I3090032_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090032, 1, "I3090032_GiftBag")
aux.RegisterItemEvent(3090032, 0, "I3090032_CanUseGiftBag")



--30����ʥʱװŮ
function I3090033_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153004, 1, -1, 8, 420)
end

function I3090033_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090033, 1, "I3090033_GiftBag")
aux.RegisterItemEvent(3090033, 0, "I3090033_CanUseGiftBag")




--������ʥʱװŮ
function I3090034_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8153014, 1, -1, 8, 420)
end

function I3090034_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3090034, 1, "I3090034_GiftBag")
aux.RegisterItemEvent(3090034, 0, "I3090034_CanUseGiftBag")

























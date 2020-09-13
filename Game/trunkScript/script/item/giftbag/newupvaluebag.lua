--��ֵ���





--50��1Ԫ��
function I4700053_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070064,1, -1, 8, 420)
end

function I4700053_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700053, 1, "I4700053_GiftBag")
aux.RegisterItemEvent(4700053, 0, "I4700053_CanUseGiftBag")







--50��5Ԫ��
function I4700054_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8160006, 1, -1, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8170007, 1, -1, 8, 420)
	end
end

function I4700054_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700054, 1, "I4700054_GiftBag")
aux.RegisterItemEvent(4700054, 0, "I4700054_CanUseGiftBag")









--50��10Ԫ��
function I4700055_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,4211704,1, -1, 8, 420)

end

function I4700055_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700055, 1, "I4700055_GiftBag")
aux.RegisterItemEvent(4700055, 0, "I4700055_CanUseGiftBag")






--60��1Ԫ��
function I4700056_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,3070064,1, -1, 8, 420)

end

function I4700056_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700056, 1, "I4700056_GiftBag")
aux.RegisterItemEvent(4700056, 0, "I4700056_CanUseGiftBag")




--60��5Ԫ��
function I4700057_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,4090002,16, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4090008,36, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4090015,36, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4090016,72, -1, 8, 420)
end

function I4700057_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700057, 1, "I4700057_GiftBag")
aux.RegisterItemEvent(4700057, 0, "I4700057_CanUseGiftBag")







--60��10Ԫ��
function I4700058_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,4820028,77, -1, 8, 420)
end

function I4700058_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700058, 1, "I4700058_GiftBag")
aux.RegisterItemEvent(4700058, 0, "I4700058_CanUseGiftBag")










--70��1Ԫ��
function I4700059_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,3070064,1, -1, 8, 420)
end

function I4700059_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700059, 1, "I4700059_GiftBag")
aux.RegisterItemEvent(4700059, 0, "I4700059_CanUseGiftBag")







--70��5Ԫ��
function I4700060_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8160010, 1, -1, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8170011, 1, -1, 8, 420)
	end
end

function I4700060_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700060, 1, "I4700060_GiftBag")
aux.RegisterItemEvent(4700060, 0, "I4700060_CanUseGiftBag")









--70��10Ԫ��
function I4700061_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,4300002,1, -1, 8, 420)
end

function I4700061_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700061, 1, "I4700061_GiftBag")
aux.RegisterItemEvent(4700061, 0, "I4700061_CanUseGiftBag")





--80��1Ԫ��
function I4700062_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID,3070064,1, -1, 8, 420)
end

function I4700062_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700062, 1, "I4700062_GiftBag")
aux.RegisterItemEvent(4700062, 0, "I4700062_CanUseGiftBag")







--80��5Ԫ��
function I4700063_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090003,36, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090009,36, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090015,36, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090016,72, -1, 8, 420)
end

function I4700063_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700063, 1, "I4700063_GiftBag")
aux.RegisterItemEvent(4700063, 0, "I4700063_CanUseGiftBag")









--80��10Ԫ��
function I4700064_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820024,107, -1, 8, 420)
end

function I4700064_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700064, 1, "I4700064_GiftBag")
aux.RegisterItemEvent(4700064, 0, "I4700064_CanUseGiftBag")




function I4700065_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--�ж�����Ա�
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8383001, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8383002, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8383004, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 8383011, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8383012, 1, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8383014, 1, -1, 8, 420)
	end
end

function I4700065_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700065, 1, "I4700065_GiftBag")
aux.RegisterItemEvent(4700065, 0, "I4700065_CanUseGiftBag")














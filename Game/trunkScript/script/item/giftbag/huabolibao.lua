--������Ƭ�ؼ���
function I4700251_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820048, 50, -1, 8, 420)
end

function I4700251_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700251, 1, "I4700251_OnUse")
aux.RegisterItemEvent(4700251, 0, "I4700251_CanUse")




--��ֵ�������
function I4700252_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 10, -1, 8, 420)
end

function I4700252_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700252, 1, "I4700252_OnUse")
aux.RegisterItemEvent(4700252, 0, "I4700252_CanUse")



--������Ƭ������
function I4700253_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820048, 120, -1, 8, 420)
end

function I4700253_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700253, 1, "I4700253_OnUse")
aux.RegisterItemEvent(4700253, 0, "I4700253_CanUse")




--�ɳ����˷��ؼ���
function I4700254_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820046, 6, -1, 8, 420)
end

function I4700254_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700254, 1, "I4700254_OnUse")
aux.RegisterItemEvent(4700254, 0, "I4700254_CanUse")




--����ʯ�ؼ���
function I4700255_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 6, -1, 8, 420)
end

function I4700255_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700255, 1, "I4700255_OnUse")
aux.RegisterItemEvent(4700255, 0, "I4700255_CanUse")




--�ɳ����˷�������
function I4700256_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820046, 20, -1, 8, 420)
end

function I4700256_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700256, 1, "I4700256_OnUse")
aux.RegisterItemEvent(4700256, 0, "I4700256_CanUse")




--����ʯ������
function I4700257_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 20, -1, 8, 420)
end

function I4700257_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700257, 1, "I4700257_OnUse")
aux.RegisterItemEvent(4700257, 0, "I4700257_CanUse")




--������ҩ���
function I4700258_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 2, -1, 8, 420)
end

function I4700258_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700258, 1, "I4700258_OnUse")
aux.RegisterItemEvent(4700258, 0, "I4700258_CanUse")




--�߼���ʯ�������
function I4700259_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820252, 5, -1, 8, 420)
end

function I4700259_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4700259, 1, "I4700259_OnUse")
aux.RegisterItemEvent(4700259, 0, "I4700259_CanUse")


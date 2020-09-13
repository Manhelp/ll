--׷ɱGM�ı���
function I4530001_Use(MapID, InstanceID, TypeID, TargetID)
	local itemid = {3020027,3020011,3020007,3020008,3020015,3010021,3010026,3010006,3010012}
    for i = 1,9 do
		local num = role.GetRoleItemNum(TargetID, itemid[i])
		if num ~= nil and num > 0 then
			role.RemoveFromRole(MapID, InstanceID, TargetID, itemid[i], num, 420)
		end
	end
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020015, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010006, 99, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010012, 99, -1, 8, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 99, -1, 8, 420)
	--[[���ֵ -3000     74
���   50000     13
����   50000     15
�﹥   10000     12
����   10000     14
����   80��      592238960
Ѫ��   500000    8 9
ħ��   500000    10 11
�ƶ��ٶ� 20000   28]]
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	while rolelevel < 80 do
	    role.AddRoleExp(MapID, InstanceID, TargetID, 50000000)
	    rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	end
	local k1 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 74)
	local k2 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 13)
	local k3 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 15)
	local k4 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 12)
	local k5 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 14)
	local k6 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 8)
	local k7 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 9)
	local k8 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 10)
	local k9 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 11)
	local k10 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 28)
	local k11 = role.GetRoleAttValue(MapID, InstanceID, TargetID, 66)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 74, -6000-k1)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 13, 50000-k2)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 15, 50000-k3)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 12, 10000-k4)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 14, 10000-k5)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 8, 500000-k6)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 9, 500000-k7)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 10, 500000-k8)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 11, 500000-k9)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 28, 20000-k10)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 66, 20000-k11)


end

function I4530001_CanUse(MapID, InstanceID, TypeID, TargetID)
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--��ʾ��ұ����ռ䲻��
		return 40, false
	end
	return 0,false
end

aux.RegisterItemEvent(4530001, 1, "I4530001_Use")
aux.RegisterItemEvent(4530001, 0, "I4530001_CanUse")


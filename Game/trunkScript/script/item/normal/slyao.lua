function I3500001_OnUse(MapID, InstanceID, TypeID, TargetID)
	--ʹ��100��ʥ��ҩˮ
	local slpoint = role.GetRoleAttValue(MapID, InstanceID, TargetID, 133)
	if slpoint <= 500 then
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 100)
	else
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 600-slpoint)
	end
end

aux.RegisterItemEvent(3500001, 1 , "I3500001_OnUse")
aux.RegisterItemEvent(3500002, 1 , "I3500001_OnUse")

function I3500003_OnUse(MapID, InstanceID, TypeID, TargetID)
	--ʹ��200��ʥ��ҩˮ
	local slpoint = role.GetRoleAttValue(MapID, InstanceID, TargetID, 133)
	if slpoint <= 400 then
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 200)
	else
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 600-slpoint)
	end
end

aux.RegisterItemEvent(3500003, 1 , "I3500003_OnUse")

function I3500004_OnUse(MapID, InstanceID, TypeID, TargetID)
	--ʹ��300��ʥ��ҩˮ
	local slpoint = role.GetRoleAttValue(MapID, InstanceID, TargetID, 133)
	if slpoint <= 300 then
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 300)
	else
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 600-slpoint)
	end
end

aux.RegisterItemEvent(3500004, 1 , "I3500004_OnUse")

function I3500005_OnUse(MapID, InstanceID, TypeID, TargetID)
	--ʹ��600��ʥ��ҩˮ
	local slpoint = role.GetRoleAttValue(MapID, InstanceID, TargetID, 133)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 133, 600-slpoint)
end

aux.RegisterItemEvent(3500005, 1 , "I3500005_OnUse")
--30ƣ��ҩ
function I3010033_onUse(MapID, InstanceID, TypeID, TargetID)
--����30ƣ��ֵ
	local pilao = role.GetRoleAttValue(MapID, InstanceID, TargetID, 81)
	if pilao <= 30 then
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 81, -pilao)
	else
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 81, -30)
	end
end
aux.RegisterItemEvent(3010033, 1 , "I3010033_onUse")

--100ƣ��ҩ
function I3010034_onUse(MapID, InstanceID, TypeID, TargetID)
--����100ƣ��ֵ
	local pilao = role.GetRoleAttValue(MapID, InstanceID, TargetID, 81)
	if pilao <= 100 then
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 81, -pilao)
	else
		role.ModRoleAttValue(MapID, InstanceID, TargetID, 81, -100)
	end
end

aux.RegisterItemEvent(3010034, 1 , "I3010034_onUse")

--������30ҩ
aux.RegisterItemEvent(3010053, 1 , "I3010033_onUse")

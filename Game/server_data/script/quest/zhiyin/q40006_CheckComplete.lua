--�����ȡʱ����
function q40006_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local a = role.GetYaojingLevel(MapID, InstanceID, RoleID)
	local CanComplete = 0
	if QuestID == 40006 then
		if a >= 30 then CanComplete =1 end
	end

	if CanComplete == 1 then
	role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40006_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local b = role.GetYaojingLevel(MapID, InstanceID, RoleID)

	if b < 30 then
		bRet = 0
	else
		bRet = 1
	end

	return bRet

end

aux.RegisterQuestEvent(40006, 0, "q40006_OnAccept")
aux.RegisterQuestEvent(40006, 5, "q40006_CheckComplete")

--�ҵ�����ʵĸ���ʵ��
function Arena_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return GetNowArenaInstanceID(RoleID)

end
aux.RegisterMapEvent("a01",  14, "Arena_GetOnePerfectMap")
aux.RegisterMapEvent("a02",  14, "Arena_GetOnePerfectMap")
aux.RegisterMapEvent("a03",  14, "Arena_GetOnePerfectMap")

--����ڻ�е��ߺ�����
function Arena_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	local k = math.random(0,2)
	return 3424073512,ArenaMap[k].ExitPoint.x, ArenaMap[k].ExitPoint.y*51,ArenaMap[k].ExitPoint.z
end

aux.RegisterMapEvent("a01",  12, "Arena_CanTakeOverWhenOnline")
aux.RegisterMapEvent("a02",  12, "Arena_CanTakeOverWhenOnline")
aux.RegisterMapEvent("a03",  12, "Arena_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function Arena_GetExporMapAndCoord(MapID, InstanceID, RoleID)
    local k = math.random(0,2)
	return 1, 3424073512, ArenaMap[k].ExitPoint.x, ArenaMap[k].ExitPoint.y*51,ArenaMap[k].ExitPoint.z
end

aux.RegisterMapEvent("a01",  13, "Arena_GetExporMapAndCoord")
aux.RegisterMapEvent("a02",  13, "Arena_GetExporMapAndCoord")
aux.RegisterMapEvent("a03",  13, "Arena_GetExporMapAndCoord")

-------------------------------------------------------------------------------------------------------
--�������Ƿ��ܹ���������
function Arena_CanEnter(MapID, InstanceID, RoleID)

	if ArenaPlayer[RoleID] ~= nil and GetNowArenaInstanceID(RoleID) ~= 0 then
		return 1
	else
	    return 0
	end

end

aux.RegisterMapEvent("a01",  15, "Arena_CanEnter")
aux.RegisterMapEvent("a02",  15, "Arena_CanEnter")
aux.RegisterMapEvent("a03",  15, "Arena_CanEnter")

-------------------------------------------------------------------------------------------------------
--[[����޷���������������
function Arena_CanInviteJoinTeam(MapID, InstanceID)
    return 9
end

aux.RegisterMapEvent("a01", 8, "Arena_CanInviteJoinTeam")
aux.RegisterMapEvent("a02", 8, "Arena_CanInviteJoinTeam")
aux.RegisterMapEvent("a03", 8, "Arena_CanInviteJoinTeam")]]

-------------------------------------------------------------------------------------------------------
--[[�������޷��뿪С��
function Arena_CanLeaveTeam(MapID, InstanceID)
    return 10
end

aux.RegisterMapEvent("a01", 9, "Arena_CanLeaveTeam")
aux.RegisterMapEvent("a02", 9, "Arena_CanLeaveTeam")
aux.RegisterMapEvent("a03", 9, "Arena_CanLeaveTeam")]]

-------------------------------------------------------------------------------------------------------
--[[����޷��ƽ��ӳ�
function Arena_CanChangeLeader(MapID, InstanceID)
    return 11
end

aux.RegisterMapEvent("a01", 10, "Arena_CanChangeLeader")
aux.RegisterMapEvent("a02", 10, "Arena_CanChangeLeader")
aux.RegisterMapEvent("a03", 10, "Arena_CanChangeLeader")]]

-------------------------------------------------------------------------------------------------------
--[[���С���޷�����
function Arena_CanKickMember(MapID, InstanceID)
    return 12
end
aux.RegisterMapEvent("a01", 17, "Arena_CanKickMember")
aux.RegisterMapEvent("a02", 17, "Arena_CanKickMember")
aux.RegisterMapEvent("a03", 17, "Arena_CanKickMember")]]

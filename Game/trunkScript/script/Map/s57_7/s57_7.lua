--����ڻ�е��ߺ�����
function s57_7_CanTakeOverWhenOnline(MapID, RoleID)
	return 52649462, 621, 40, 135
end
aux.RegisterMapEvent("s57_7",  12, "s57_7_CanTakeOverWhenOnline")

--�ҵ���ͼ�ĳ��ں�����
function s57_7_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512, 610+math.random(5), 23094, 669+math.random(5)
end
aux.RegisterMapEvent("s57_7",  13, "s57_7_GetExporMapAndCoord")

--�ҵ�����ʵĸ���ʵ��
function s57_7GetOnePerfectMap(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s57_7",  14, "s57_7GetOnePerfectMap")

--�������Ƿ��ܹ���������
function s57_7_CanEnter(MapID, InstanceID, RoleID)
	if role.GetRoleLevel(MapID, InstanceID, RoleID) >= 80 then
		return 1
	else
		return 0
	end
end
aux.RegisterMapEvent("s57_7",  15, "s57_7_CanEnter")

--������
function s57_7_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 865 then --to s57_6
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 52649206, 889, 1, 394)
	end
end
aux.RegisterMapEvent("s57_7",6,"s57_7_OnEnterTrigger")

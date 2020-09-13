function s61_OnPlayerEnter(MapID, InstanceID, RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if act.GetActIsStart(53) and level >= 40 then
		local sex = role.GetRoleSex(RoleID)
		if sex == 1 then
			unit.AddBuff(MapID, InstanceID, RoleID,6940102,RoleID) --40���������ÿ�����߻�õ����ڿ���buff
		elseif sex == 0 then
			unit.AddBuff(MapID, InstanceID, RoleID,6940202,RoleID) --40���������ÿ�����߻�õ����ڿ���buff
		end
	end
end
--ע��
aux.RegisterMapEvent("s61", 2, "s61_OnPlayerEnter")



function s61_OnPlayerLeave(MapID, InstanceID, RoleID)
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID,69401) then
		unit.CancelBuff(MapID,InstanceID,TargetID,6940102)
	end
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID,69402) then
		unit.CancelBuff(MapID,InstanceID,TargetID,6940202)
	end
end
--ע��
aux.RegisterMapEvent("s61", 3, "s61_OnPlayerLeave")


--�ҵ�����ʵĸ���ʵ��
function s61_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return 1

end
aux.RegisterMapEvent("s61",  14, "s61_GetOnePerfectMap")

--����ڻ�е��ߺ�����
function s61_CanEnterWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("s61",  12, "s61_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function s61_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512,610,23094,669
end

aux.RegisterMapEvent("s61",  13, "s61_GetExporMapAndCoord")

--�������Ƿ��ܹ���������
function s61_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("s61",  15, "s61_CanEnter")

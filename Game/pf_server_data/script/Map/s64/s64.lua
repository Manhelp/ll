
function s64_OnPlayerEnter(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
end
aux.RegisterMapEvent("s64", 2, "s64_OnPlayerEnter")


--�ҵ�����ʵĸ���ʵ��
function s64_GetOnePerfectMap(MapID, InstanceID, RoleID)
	return curMSIns
end
aux.RegisterMapEvent("s64",  14, "s64_GetOnePerfectMap")

--����ڻ�е��ߺ�����
function s64_CanEnterWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("s64",  12, "s64_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function s64_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3390518312,213,2543,726
end

aux.RegisterMapEvent("s64",  13, "s64_GetExporMapAndCoord")

--�������Ƿ��ܹ���������
function s64_CanEnter(MapID, InstanceID, RoleID)
	return 1
end
aux.RegisterMapEvent("s64",  15, "s64_CanEnter")

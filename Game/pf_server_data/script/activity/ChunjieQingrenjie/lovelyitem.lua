--���˽���Ʒ��BUFF
--����ʹ�ø��������Լ�buff


--ʹ��Ч��
function i4800077_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
    local sex = role.GetRoleSex(TargetID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i]) --��ȡ���ѵ����ڵ�ͼ
				if MapID == Target_MapID and role.GetRoleSex(Role[i]) == 1 then
					unit.AddBuff(MapID, InstanceID, Role[i], 6000801, Role[i])--��BUFF
				end
			end
		end
	elseif TeamID == 4294967295 and sex == 1 then
	     unit.AddBuff(MapID, InstanceID,TargetID, 6000801, TargetID)--��BUFF
	end
end
aux.RegisterItemEvent(4800077, 1, "i4800077_QuestUsable")





--Ů��ʹ����Ʒ���Ը�Ů�Զ��Ѽ�buff


--ʹ��Ч��
function i4800079_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
    local sex = role.GetRoleSex(TargetID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i]) --��ȡ���ѵ����ڵ�ͼ
				if MapID == Target_MapID and role.GetRoleSex(Role[i]) == 0 then
					unit.AddBuff(MapID, InstanceID, Role[i], 6000901, Role[i])--��BUFF
				end
			end
		end
	elseif TeamID == 4294967295 and sex == 0 then
	     unit.AddBuff(MapID, InstanceID, TargetID, 6000901, TargetID)--��BUFF
	end
end
aux.RegisterItemEvent(4800079, 1, "i4800079_QuestUsable")



--�氮�����ͬ������Ѽ�buff
function i4800078_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i]) --��ȡ���ѵ����ڵ�ͼ
				if MapID == Target_MapID  then
					unit.AddBuff(MapID, InstanceID, Role[i], 6001001, Role[i])--��BUFF
				end
			end
		end
	elseif TeamID == 4294967295  then
	     unit.AddBuff(MapID, InstanceID, TargetID,6001001, TargetID)--��BUFF
	end
end


aux.RegisterItemEvent(4800078, 1, "i4800078_QuestUsable")


--�̶�����а�֮�
--���ɸ�������
function MoKu_level(MapID, InstanceID, RoleID)
	local monid = 0
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local monlevel = 0
	--���û��С��
	if 4294967295 == TeamID  then
		local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if rolelevel > 100 then
			rolelevel = 100
		end
		local m = math.fmod(rolelevel,5)
		rolelevel = math.floor(rolelevel/5)*5
		if m > 3 then
			rolelevel = rolelevel + 5
		end
		return  rolelevel
	else
		--���С�����ID
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		local maxlevel = 0
		local totallevel = 0
		local teamnum = 0
		for k in pairs(Role) do
			if 4294967295 ~= Role[k] then
			    local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
				local rolelevel = role.GetRoleLevel(Role_MapID, Role_InstanceID, Role[k])
				if rolelevel > 100 then
					rolelevel = 100
				end
				if rolelevel >= 30 then
					--��������ܵȼ�֮��
					totallevel = totallevel + rolelevel
					teamnum = teamnum + 1
					if rolelevel > maxlevel then
						maxlevel = rolelevel
					end
				end
			end
		end
		if teamnum == 1 then--���ȼ�30���ϵ�ֻ��һ��ʱ
			local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if rolelevel > 100 then
				rolelevel = 100
			end
			monlevel = math.floor(rolelevel/5)*5
		else
			local avglevel = (totallevel - maxlevel)/(teamnum-1)
			monlevel = avglevel + (maxlevel-avglevel)/8
			local m = math.fmod(monlevel,5)
			monlevel = math.floor(monlevel/5)*5
			if m > 3 then
				monlevel = monlevel + 5
			end
		end
		return monlevel
	end
end

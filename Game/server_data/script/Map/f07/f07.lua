----0 ��¼���븱���Ƿ�ˢ�¹���


function f07_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100110)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	-------------��������ʾ
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,0)
	if monsterhave==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040140, 471, 3840, 411)
		map.SetMapScriptData(MapID,InstanceID,1,0,1)
	end
	--����ͳ��
	--instanceData(7, RoleID, 30, 37)
	--���ͳ��
	--UpdateRoleData(RoleID, 16, 1)
	--������buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 5 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109708, RoleID)
				elseif lev == 6 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109711, RoleID)
				elseif lev == 7 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109713, RoleID)
				elseif lev == 8 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109714, RoleID)
				end
				unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
				break
			end
		end
	end
end

--ע��
aux.RegisterMapEvent("f07", 2, "f07_OnPlayerEnter")

---mapscriptdata[1] ��¼���������Ƿ���ˢ������



function f04_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100068)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,1)
	if monsterhave==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040044, 444, 5845, 114)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	end
	--����ͳ��
	--instanceData(4, RoleID, 27, 34)
	--���ͳ��
	--UpdateRoleData(RoleID, 15, 1)
	--������buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109702, RoleID)
				elseif lev == 5 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109705, RoleID)
				end
				unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
				break
			end
		end
	end
end

--ע��
aux.RegisterMapEvent("f04", 2, "f04_OnPlayerEnter")

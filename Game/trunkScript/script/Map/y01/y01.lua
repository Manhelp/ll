---mapscriptdata[0] ��¼���������Ƿ���ˢ������


function y01_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100133)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,0)
	if monsterhave==0 then

	        --[[	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
			map.OpenCloseDoor(MapID, InstanceID, 1111, nil)
		]]--
		
		map.MapCreateCreature(MapID, InstanceID, 3040306, 326, 50, 56)
		map.MapCreateCreature(MapID, InstanceID, 3040307, 342, 50, 66)

		map.SetMapScriptData(MapID,InstanceID,1,0,1)
		
	end
	
	
	


	
end

--ע��
aux.RegisterMapEvent("y01", 2, "y01_OnPlayerEnter")




---mapscriptdata[1] ��¼���������Ƿ���ˢ������
--[2]��¼��һ�Ƿ���ɱ��
--[3]��¼�϶��Ƿ���ɱ��
--[4]��¼�����Ƿ���ɱ��
--[5]��¼�����Ƿ���ɱ��

function f01_OnPlayerEnter(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100076)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,1)
	if monsterhave == 0 then
		--[[
			map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
			map.OpenCloseDoor(MapID, InstanceID, 1111, nil)
		--]]
		map.MapCreateCreature(MapID, InstanceID, 3030003, 336, 50, 58)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	end
	--����ͳ��
	--instanceData(1, RoleID, 24, 31)
	--���ͳ��
	--UpdateRoleData(RoleID, 15, 1)
end

--ע��
aux.RegisterMapEvent("f01", 2, "f01_OnPlayerEnter")

function f01_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1116 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100078)
		--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424071976,1283, 20830, 1347)
	end
	if ObjID == 1120 then
		local a= map.GetMapScriptData(MapID,InstanceID,1,2)
		if a==0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100077)
			--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		--role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424071976,1283, 20830, 1347)
	end
end
aux.RegisterMapEvent("f01", 6, "f01_OnEnterTrigger")

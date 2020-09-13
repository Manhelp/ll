----scriptdata[1]��¼3040018�Ƿ�����
----scriptdata[2]��¼3040019�Ƿ�����
----scriptdata[3]��¼�Ƿ�ˢ��4������
----scriptdata[4]��¼3040020�Ƿ���������������
----scriptdata[5]��¼3040021�Ƿ���������������
----scriptdata[6]��¼3040024�Ƿ�����
----scriptdata[7]��¼3040025�Ƿ�����
----scriptdata[8]��Ǳ���
----scriptdata[9]��ǽ������Ƿ�ˢ��
function f03_OnPlayerEnter(MapID, InstanceID, RoleID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100052)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local freshmonster = map.GetMapScriptData(MapID, InstanceID, 1,9)
	if freshmonster ==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040014, 685, 2621, 942)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 675, 2621, 942)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 685, 2621, 952)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 675, 2621, 952)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 737, 2698, 738)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 503, 2624, 888)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 525, 2623, 784)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 517, 2623, 783)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 523, 2623, 775)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 514, 2623, 780)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 445, 2624, 838)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 502, 2642, 684)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 429, 2714, 676)
		map.MapCreateCreature(MapID, InstanceID, 3040014, 418, 2614, 583)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 418, 2614, 588)
		map.MapCreateCreature(MapID, InstanceID, 3040015, 413, 2614, 583)
		map.MapCreateCreature(MapID, InstanceID, 3040016, 413, 2595, 592)
		map.SetMapScriptData(MapID, InstanceID, 1,9,1)
	end
	--����ͳ��
	--instanceData(3, RoleID, 26, 33)
	--���ͳ��
	--UpdateRoleData(RoleID, 15, 1)
	--������buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
					unit.AddBuff(MapID, InstanceID, RoleID, 6109701, RoleID)
				end
				break
			end
		end
	end
end
--ע��
aux.RegisterMapEvent("f03", 2, "f03_OnPlayerEnter")


function f03_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID==3498 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100046)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
end
aux.RegisterMapEvent("f03", 6, "f03_OnEnterTrigger")

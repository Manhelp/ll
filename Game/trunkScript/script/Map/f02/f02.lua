---mapscriptdata[1] ��¼���������Ƿ���ˢ������
----[2]��¼3040008�Ƿ�ɱ��
----[3]��¼3040009�Ƿ�ɱ��
----[4]��¼�Ƿ��й���Χ��
----[5]��¼�Ƿ��������
----[6]��¼3040010�Ƿ�ɱ��



function f02_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100038)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,1)
	if monsterhave==0 then
		local x,y,z = 858, 4200, 623
		map.MapCreateCreature(MapID, InstanceID, 3040007, x, y, z)
		map.MapCreateCreature(MapID, InstanceID, 3040006, x+10, y, z-20)
		map.MapCreateCreature(MapID, InstanceID, 3040004, x-10, y, z-20)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 803, 5590, 742)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 817, 4754, 706)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 338, 5378, 798)
		map.MapCreateCreature(MapID, InstanceID, 3040007, 338, 5378, 783)
		map.MapCreateCreature(MapID, InstanceID, 3040006, 612, 4394, 441)
		map.MapCreateCreature(MapID, InstanceID, 3040005, 603, 4394, 447)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	end
	-----��ȡ��������
	if not (role.IsRoleCompleteQuest(MapID,InstanceID,RoleID,29000) or role.IsRoleHaveQuest(MapID,InstanceID,RoleID,29000)) then
		role.AddQuest(RoleID,29000)
	end
	--����ͳ��
	--instanceData(2, RoleID, 25, 32)
	--���ͳ��
	--UpdateRoleData(RoleID, 15, 1)
end

--ע��
aux.RegisterMapEvent("f02", 2, "f02_OnPlayerEnter")

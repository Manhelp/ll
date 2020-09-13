----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]���3040075
----scriptdata[2]���3040076
----scriptdata[3]���3040077
----scriptdata[4]���3040078


function c3040076_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10025)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040076, 2, "c3040076_OnEnterCombat")






function c3040076_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	local a = math.random(1,100)
	if a>50 then
		local x,y,z=unit.GetPosition(MapID, InstanceID,TargetID)
		map.MapCreateCreature(MapID, InstanceID, 3040091, x, y, z)
		map.MapCreateCreature(MapID, InstanceID, 3040091, x, y, z)
		map.MapCreateCreature(MapID, InstanceID, 3040091, x, y, z)
	end

	map.SetMapScriptData(MapID, InstanceID, 1, 2, 1)
	
	local bossdeath_1=map.GetMapScriptData(MapID,InstanceID,1,1)
	local bossdeath_3=map.GetMapScriptData(MapID,InstanceID,1,3)
	local bossdeath_4=map.GetMapScriptData(MapID,InstanceID,1,4)
	
	if bossdeath_2 == 1 and bossdeath_3 == 1 and bossdeath_4 == 1 then
		map.OpenCloseDoor(MapID, InstanceID, 4588, 1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100086)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100085)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
	
		


end

aux.RegisterCreatureEvent(3040076, 4, "c3040076_OnDie")

----scriptdata[0]��ǽ������Ƿ�ˢ��
----scriptdata[1]����ٻ������ڼ�����
----scriptdata[2]���3040141�Ƿ�����
----scriptdata[3]���3040143�Ƿ�����
----scriptdata[4]���3040150�Ƿ�����
----scriptdata[5]���3040151�Ƿ�����
----scriptdata[6]��¼3040152��ID
----scriptdata[7]���3040164�Ƿ�����
----scriptdata[8]���3040165�Ƿ�����
----scriptdata[9]���3040166�Ƿ�����
----scriptdata[10]����Ƿ�ˢ������




function c3040164_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10063)
	
end

aux.RegisterCreatureEvent(3040164, 2, "c3040164_OnEnterCombat")






function c3040164_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	
	

	local fenshenisdead_01 = map.GetMapScriptData(MapID,InstanceID,1,8)
	local fenshenisdead_02 = map.GetMapScriptData(MapID,InstanceID,1,9)

	map.SetMapScriptData(MapID,InstanceID,1,7,1)

	if fenshenisdead_01 == 1 and fenshenisdead_02 == 1 then
	
		map.OpenCloseDoor(MapID, InstanceID, 2218, 1)
		map.OpenCloseDoor(MapID, InstanceID, 1853, 1)

		map.MapCreateCreature(MapID, InstanceID, 3040153, 476, 5664, 1790)
		map.MapCreateCreature(MapID, InstanceID, 3040154, 501, 5664, 1790)


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100116)
		--�������غ�����ʦˢ��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	

	end

end

aux.RegisterCreatureEvent(3040164, 4, "c3040164_OnDie")

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


function c3040140_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10089)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040140, 2, "c3040140_OnEnterCombat")


function s5054505_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	cre.MonsterSay(MapID, InstanceID, OwnerID, 10088)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	
	local crelvl = map.GetMapScriptData(MapID,InstanceID,1,1)

	if crelvl == 0 then
		map.MapCreateCreature(MapID, InstanceID, 3040155, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040159, x-5, y, z+5)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	elseif crelvl == 1 then
		map.MapCreateCreature(MapID, InstanceID, 3040156, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040160, x-5, y, z+5)
		map.SetMapScriptData(MapID,InstanceID,1,1,2)
	elseif crelvl == 2 then
		map.MapCreateCreature(MapID, InstanceID, 3040157, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040161, x-5, y, z+5)
		map.SetMapScriptData(MapID,InstanceID,1,1,3)
	else 
		map.MapCreateCreature(MapID, InstanceID, 3040158, x+5, y, z+5)
		map.MapCreateCreature(MapID, InstanceID, 3040162, x-5, y, z+5)
	end
			
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--ͬ����޸���ΧΪ30�����ӣ��߶�Ϊ20���з�����
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 30, 20)
	
end
aux.RegisterSkillEvent(5054505, 1, "s5054505_Cast")



function c3040140_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10090)
	
	map.OpenCloseDoor(MapID, InstanceID, 2216, 1)
	map.MapCreateCreature(MapID, InstanceID, 3040142, 270, 3840, 613)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100111)
	--�Ŵ�
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040140, 4, "c3040140_OnDie")

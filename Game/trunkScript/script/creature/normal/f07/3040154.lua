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


function c3040154_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10063)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--ͬ����޸���ΧΪ40�����ӣ��߶�Ϊ20���з�����
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040154, 2, "c3040154_OnEnterCombat")


function s5054507_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	cre.MonsterSay(MapID, InstanceID, OwnerID, 10109)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)

	delta_x = math.random(1,18)
	delta_z = math.random(1,18)

	map.MapCreateCreature(MapID, InstanceID, 3040163, x+9-delta_x, 5664, z+9-delta_z)

	
end
aux.RegisterSkillEvent(5054507, 1, "s5054507_Cast")



function c3040154_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10108)

	map.MapCreateCreature(MapID, InstanceID, 6010039, 501, 5664, 1790)

	
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100086)
	
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040154, 4, "c3040154_OnDie")

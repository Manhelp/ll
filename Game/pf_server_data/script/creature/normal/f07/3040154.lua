----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记召唤术到第几级了
----scriptdata[2]标记3040141是否死亡
----scriptdata[3]标记3040143是否死亡
----scriptdata[4]标记3040150是否死亡
----scriptdata[5]标记3040151是否死亡
----scriptdata[6]记录3040152的ID
----scriptdata[7]标记3040164是否死亡
----scriptdata[8]标记3040165是否死亡
----scriptdata[9]标记3040166是否死亡
----scriptdata[10]标记是否刷过宝箱


function c3040154_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10063)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
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
